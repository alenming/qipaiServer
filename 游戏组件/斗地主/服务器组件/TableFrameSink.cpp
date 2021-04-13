#include "StdAfx.h"
#include "TableFrameSink.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
#define IDI_CHECK_TABLE				1					//检查桌子
#define IDI_USER_OUT_TIME			6					//玩家超时

#define IDI_AUTO_START				7					//自动开始

//构造函数
CTableFrameSink::CTableFrameSink()
{
	//组件变量
	m_pITableFrame=NULL;
	m_pGameCustomRule=NULL;
	m_pGameServiceOption=NULL;
	m_pGameServiceAttrib=NULL;
	m_bOffLineTrustee = false;

	m_cbHaveOutMissile = FALSE;

	//炸弹变量
	m_wFirstUser=INVALID_CHAIR;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));
	ZeroMemory(m_cbUserTrustee,sizeof(m_cbUserTrustee));

	ZeroMemory(m_cbLeftCardNum, sizeof(m_cbLeftCardNum));
	ZeroMemory(m_cbPlayerMultiple, sizeof(m_cbPlayerMultiple));

	m_dwOperateTime = 0L;

	m_bWriteRoomStorage = true;

	//游戏变量
	m_wTimerControl=0;
	m_cbBombCount=0;
	ZeroMemory(m_cbEachBombCount,sizeof(m_cbEachBombCount));

	//叫分信息
	m_cbBankerScore=0;
	FillMemory(m_cbScoreInfo, sizeof(m_cbScoreInfo), 0xFF);
	m_cbCallScoreCount=0;

	//出牌信息
	m_cbTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

	//扑克信息
	ZeroMemory(m_cbBankerCard,sizeof(m_cbBankerCard));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));



	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink()
{

}

//接口查询
VOID * CTableFrameSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//复位桌子
VOID CTableFrameSink::RepositionSink()
{
	m_cbHaveOutMissile = FALSE;

	//游戏变量
	m_cbBombCount=0;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));
	ZeroMemory(m_cbEachBombCount,sizeof(m_cbEachBombCount));

	ZeroMemory(m_cbLeftCardNum, sizeof(m_cbLeftCardNum));
	ZeroMemory(m_cbPlayerMultiple, sizeof(m_cbPlayerMultiple));

	m_bWriteRoomStorage = true;

	//叫分信息
	m_cbBankerScore=0;
	FillMemory(m_cbScoreInfo, sizeof(m_cbScoreInfo),0xFF);
	m_cbCallScoreCount=0;

	//出牌信息
	m_cbTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

	//扑克信息
	ZeroMemory(m_cbBankerCard,sizeof(m_cbBankerCard));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

	return;
}

//配置桌子
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);

	//错误判断
	if (m_pITableFrame==NULL)
	{
		CTraceService::TraceString(TEXT("游戏桌子 CTableFrameSink 查询 ITableFrame 接口失败"),TraceLevel_Exception);
		return false;
	}

	//开始模式
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);

	//游戏配置
	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();

	//自定规则
	ASSERT(m_pITableFrame->GetCustomRule()!=NULL);
	m_pGameCustomRule=(tagCustomRule *)m_pITableFrame->GetCustomRule();

	m_bOffLineTrustee = true;
	return true;
}

//消费能力
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	return 0L;
}

//最少积分
SCORE CTableFrameSink::QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return 0L;
}

//游戏开始
bool CTableFrameSink::OnEventGameStart()
{
	ZeroMemory(m_cbUserTrustee, sizeof(m_cbUserTrustee));
	m_pITableFrame->KillGameTimer(IDI_AUTO_START);

	BYTE cbAiCount = 0;
	BYTE cbPlayCount = 0;

	//用户状态
	for (WORD i = 0; i<GAME_PLAYER; i++)
	{
		//获取用户
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem == NULL)
			continue;

		cbPlayCount++;
		if (pIServerUserItem->IsAndroidUser())
		{
			cbAiCount++;
		}
	}

	if (cbAiCount == 0 || cbAiCount == cbPlayCount)
		m_bWriteRoomStorage = false;

	//库存衰减
	if (m_bWriteRoomStorage)
		m_pITableFrame->CalculateStorageDeduct();

	//出牌信息
	m_cbTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

	//设置状态
	m_pITableFrame->SetGameStatus(GAME_SCENE_CALL);

	//混乱扑克
	BYTE cbRandCard[FULL_COUNT];
	m_GameLogic.RandCardList(cbRandCard,CountArray(cbRandCard));
	

	tag_Land_Result Land_Result;
	ZeroMemory(&Land_Result, sizeof(Land_Result));
	m_pITableFrame->GetControlResult(NULL, 0, &Land_Result);

	if (Land_Result.cbTableCardArray[0] != 0)
		CopyMemory(cbRandCard, Land_Result.cbTableCardArray, sizeof(cbRandCard));
	

	//比赛房间随机第一个随机叫分
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)!=0 )
	{
		m_wFirstUser = INVALID_CHAIR;
	}

	//抽取明牌
	BYTE cbValidCardData=0;
	BYTE cbValidCardIndex=0;
	WORD wStartUser=m_wFirstUser;
	WORD wCurrentUser=m_wFirstUser;

	//抽取玩家
	if (wStartUser==INVALID_CHAIR)
	{
		//抽取扑克
		cbValidCardIndex=rand()%DISPATCH_COUNT;
		cbValidCardData=cbRandCard[cbValidCardIndex];

		//设置用户
		wStartUser=m_GameLogic.GetCardValue(cbValidCardData)%GAME_PLAYER;
		wCurrentUser=(wStartUser+cbValidCardIndex/NORMAL_COUNT)%GAME_PLAYER;
	}

	for (int i = 0; i < 15; i++)
	{
		if (i < 13)
			m_cbLeftCardNum[i] = 4;
		else
			m_cbLeftCardNum[i] = 1;
	}
	
	//用户扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_cbHandCardCount[i]=NORMAL_COUNT;
		CopyMemory(&m_cbHandCardData[i],&cbRandCard[i*m_cbHandCardCount[i]],sizeof(BYTE)*m_cbHandCardCount[i]);
	}

	//设置底牌
	CopyMemory(m_cbBankerCard,&cbRandCard[DISPATCH_COUNT],sizeof(m_cbBankerCard));

	//机器人数据
	CMD_S_AndroidCard AndroidCard ;
	ZeroMemory(&AndroidCard, sizeof(AndroidCard)) ;

	//用户扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		CopyMemory(&AndroidCard.cbHandCard[i], m_cbHandCardData[i], sizeof(BYTE)*m_cbHandCardCount[i]) ;
	}

	//设置用户
	m_wFirstUser=wCurrentUser;
	m_wCurrentUser=wCurrentUser;

	//构造变量
	CMD_S_GameStart GameStart;
	GameStart.wCurrentUser=wCurrentUser;

	AndroidCard.wCurrentUser = m_wCurrentUser;
	CopyMemory(AndroidCard.cbBankerCard,m_cbBankerCard,3*sizeof(BYTE));

	//发送数据
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//构造扑克
		ASSERT(CountArray(GameStart.cbCardData)>=m_cbHandCardCount[i]);
		CopyMemory(GameStart.cbCardData,m_cbHandCardData[i],sizeof(BYTE)*m_cbHandCardCount[i]);

		//发送数据
		IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(i) ;
		if(!pServerUserItem->IsAndroidUser())
		{
			m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
			m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		}
		else 
		{
			m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&AndroidCard,sizeof(AndroidCard));
			m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		}
	}

	//排列扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameLogic.SortCardList(m_cbHandCardData[i],m_cbHandCardCount[i],ST_ORDER);
	}
	
	m_dwOperateTime = (DWORD)time(NULL);

	IsOfflineTrustee();
	return true;
}

//游戏结束
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束
		{
			//变量定义
			CMD_S_GameConclude GameConclude;
			ZeroMemory(&GameConclude,sizeof(GameConclude));

			//设置变量

			//炸弹信息
			GameConclude.cbBombCount=m_cbBombCount;
		
			//用户扑克
			CopyMemory(GameConclude.cbCardCount, m_cbHandCardCount, sizeof(GameConclude.cbCardCount));
			CopyMemory(GameConclude.cbHandCardData, m_cbHandCardData, sizeof(GameConclude.cbHandCardData));

			//炸弹统计
			LONG lScoreTimes=1L * m_cbBankerScore;
			for (BYTE i=0;i<m_cbBombCount;i++) lScoreTimes*=2L;

			//春天判断
			if (wChairID==m_wBankerUser)
			{
				//用户定义
				WORD wUser1=(m_wBankerUser+1)%GAME_PLAYER;
				WORD wUser2=(m_wBankerUser+2)%GAME_PLAYER;

				//用户判断
				if ((m_cbOutCardCount[wUser1]==0)&&(m_cbOutCardCount[wUser2]==0)) 
				{
					lScoreTimes*=2L;
					GameConclude.bChunTian=TRUE;
				}
			}

			//春天判断
			if (wChairID!=m_wBankerUser)
			{
				if (m_cbOutCardCount[m_wBankerUser]==1)
				{
					lScoreTimes*=2L;
					GameConclude.bFanChunTian=TRUE;
				}
			}

			//调整倍数
			lScoreTimes=__min(m_pGameCustomRule->wMaxScoreTimes,lScoreTimes);

			//农民托管数量
			WORD wTrusteeCount=0;
			if ((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)!=0)
			{
				for(WORD i = 0; i < GAME_PLAYER; i++)
				{
					if (i!=m_wBankerUser)
					{
						if (m_pITableFrame->GetTableUserItem(i)->IsTrusteeUser()==true) wTrusteeCount++;
					}
				}
			}

			//积分变量
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));

			//统计积分
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//变量定义
				SCORE lUserScore=0L;
				SCORE lCellScore=m_pITableFrame->GetCellScore();
				bool bTrusteeUser=m_pITableFrame->GetTableUserItem(i)->IsTrusteeUser();
				
				//积分基数
				if (i==m_wBankerUser)
				{
					lUserScore=(m_cbHandCardCount[m_wBankerUser]==0)?2L:-2L;
				}
				else
				{
					if (m_cbHandCardCount[m_wBankerUser]==0)
					{
						lUserScore = -1L;
					}
					else
					{
						lUserScore = 1L;
					}
				}

				//计算积分
				ScoreInfoArray[i].lScore = lUserScore*lCellScore*lScoreTimes;
				ScoreInfoArray[i].cbType=(ScoreInfoArray[i].lScore>=0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;

				//计算税收
				ScoreInfoArray[i].lRevenue=m_pITableFrame->CalculateRevenue(i,ScoreInfoArray[i].lScore);
				if (ScoreInfoArray[i].lRevenue>0L) ScoreInfoArray[i].lScore-=ScoreInfoArray[i].lRevenue;

				//设置积分
				GameConclude.lGameScore[i]=ScoreInfoArray[i].lScore;

				//库存值
				IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);
				if (pServerUser && pServerUser->IsAndroidUser() && m_bWriteRoomStorage)
					m_pGameServiceOption->pRoomStorageOption.lStorageCurrent += ScoreInfoArray[i].lScore;
			}

			//发送数据
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_CONCLUDE,&GameConclude,sizeof(GameConclude));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_CONCLUDE,&GameConclude,sizeof(GameConclude));

			//写入积分
			m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

			//切换用户
			m_wFirstUser=wChairID;

			//结束游戏
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);

			m_pITableFrame->SetGameTimer(IDI_AUTO_START,m_pGameCustomRule->cbTimeStartGame*1000,1,0L);

			return true;
		}
	case GER_DISMISS:		//游戏解散
		{
			//变量定义
			CMD_S_GameConclude GameConclude;
			ZeroMemory(&GameConclude,sizeof(GameConclude));

			//用户扑克
			BYTE cbCardIndex=0;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//拷贝扑克
				GameConclude.cbCardCount[i]=m_cbHandCardCount[i];
				CopyMemory(&GameConclude.cbHandCardData[cbCardIndex],m_cbHandCardData[i],m_cbHandCardCount[i]*sizeof(BYTE));

				//设置索引
				cbCardIndex+=m_cbHandCardCount[i];
			}


			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_CONCLUDE,&GameConclude,sizeof(GameConclude));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_CONCLUDE,&GameConclude,sizeof(GameConclude));

			//结束游戏
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);

			m_pITableFrame->SetGameTimer(IDI_AUTO_START, m_pGameCustomRule->cbTimeStartGame*1000, 1, 0L);

			return true;
		}
	case GER_USER_LEAVE:	//用户强退
	case GER_NETWORK_ERROR:	//网络中断
		{
			if (m_pITableFrame->GetGameStatus() == GAME_SCENE_CALL)
			{
				if (m_wCurrentUser == wChairID)
				{
					OnUserCallScore(wChairID,0);
				}
			}
			else if (m_pITableFrame->GetGameStatus() == GAME_SCENE_MULTIPLE)
			{
				if (m_cbPlayerMultiple[wChairID] == 0)
					OnUserDouble(wChairID, 1);
			}
			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//发送场景
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:	//空闲状态
		{
			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			StatusFree.cbCallTimes = m_pGameCustomRule->cbTimeCallScore;
			StatusFree.cbOutTimes = m_pGameCustomRule->cbTimeOutCard;
			StatusFree.cbDoubleTimes = m_pGameCustomRule->cbTimeHeadOutCard;
			StatusFree.cbEndTimes = m_pGameCustomRule->cbTimeStartGame;

			CopyMemory(StatusFree.nTrusteeStatus, m_cbUserTrustee, sizeof(StatusFree.nTrusteeStatus));

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GAME_SCENE_CALL:	//叫分状态
		{
			//构造数据
			CMD_S_StatusCall StatusCall;
			ZeroMemory(&StatusCall,sizeof(StatusCall));

			StatusCall.cbCallTimes = m_pGameCustomRule->cbTimeCallScore;
			StatusCall.cbOutTimes = m_pGameCustomRule->cbTimeOutCard;
			StatusCall.cbDoubleTimes = m_pGameCustomRule->cbTimeHeadOutCard;
			StatusCall.cbEndTimes = m_pGameCustomRule->cbTimeStartGame;

			CopyMemory(StatusCall.nTrusteeStatus, m_cbUserTrustee, sizeof(StatusCall.nTrusteeStatus));

			DWORD dwPassTime = (DWORD)time(NULL) - m_dwOperateTime;
			StatusCall.cbTimeLeave = (BYTE)(StatusCall.cbCallTimes - __min(dwPassTime, StatusCall.cbCallTimes));

			//游戏信息
			StatusCall.wCurrentUser=m_wCurrentUser;
			CopyMemory(StatusCall.cbScoreInfo,m_cbScoreInfo,sizeof(m_cbScoreInfo));
			CopyMemory(StatusCall.cbHandCardData,m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID]*sizeof(BYTE));

			//发送场景
			bool bSendResult = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusCall,sizeof(StatusCall));

			if (m_cbUserTrustee[wChairID] == TRUE)
			{
				CMD_S_TRUSTEE pSTrustee;
				pSTrustee.wTrusteeUser = wChairID;
				pSTrustee.bTrustee = FALSE;

				m_cbUserTrustee[wChairID] = FALSE;
				pIServerUserItem->SetTrusteeUser(false);

				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_TRUSTEE, &pSTrustee, sizeof(CMD_S_TRUSTEE));
				m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_TRUSTEE, &pSTrustee, sizeof(CMD_S_TRUSTEE));
			}

			return bSendResult;
		}
	case GAME_SCENE_MULTIPLE:	//加倍状态
		{
			//构造数据
			CMD_S_StatusDouble StatusDouble;
			ZeroMemory(&StatusDouble, sizeof(StatusDouble));

			StatusDouble.cbCallTimes = m_pGameCustomRule->cbTimeCallScore;
			StatusDouble.cbOutTimes = m_pGameCustomRule->cbTimeOutCard;
			StatusDouble.cbDoubleTimes = m_pGameCustomRule->cbTimeHeadOutCard;
			StatusDouble.cbEndTimes = m_pGameCustomRule->cbTimeStartGame;

			CopyMemory(StatusDouble.nTrusteeStatus, m_cbUserTrustee, sizeof(StatusDouble.nTrusteeStatus));

			StatusDouble.cbBankerScore = m_cbBankerScore;
			CopyMemory(StatusDouble.cbPlayerMultiple, m_cbPlayerMultiple, sizeof(StatusDouble.cbPlayerMultiple));

			StatusDouble.wBankerUser = m_wBankerUser;

			DWORD dwPassTime = (DWORD)time(NULL) - m_dwOperateTime;
			StatusDouble.cbTimeLeave = (BYTE)(StatusDouble.cbDoubleTimes - __min(dwPassTime, StatusDouble.cbDoubleTimes));

			CopyMemory(StatusDouble.cbBankerCard, m_cbBankerCard, sizeof(StatusDouble.cbBankerCard));
			CopyMemory(StatusDouble.cbHandCardData, m_cbHandCardData[wChairID], m_cbHandCardCount[wChairID] * sizeof(BYTE));

			//发送场景
			bool bSendResult = m_pITableFrame->SendGameScene(pIServerUserItem, &StatusDouble, sizeof(StatusDouble));

			if (m_cbUserTrustee[wChairID] == TRUE)
			{
				CMD_S_TRUSTEE pSTrustee;
				pSTrustee.wTrusteeUser = wChairID;
				pSTrustee.bTrustee = FALSE;

				m_cbUserTrustee[wChairID] = FALSE;
				pIServerUserItem->SetTrusteeUser(false);

				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_TRUSTEE, &pSTrustee, sizeof(CMD_S_TRUSTEE));
				m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_TRUSTEE, &pSTrustee, sizeof(CMD_S_TRUSTEE));
			}

			return bSendResult;
		}
	case GAME_SCENE_PLAY:	//游戏状态
		{
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			StatusPlay.cbCallTimes = m_pGameCustomRule->cbTimeCallScore;
			StatusPlay.cbOutTimes = m_pGameCustomRule->cbTimeOutCard;
			StatusPlay.cbDoubleTimes = m_pGameCustomRule->cbTimeHeadOutCard;
			StatusPlay.cbEndTimes = m_pGameCustomRule->cbTimeStartGame;

			CopyMemory(StatusPlay.nTrusteeStatus, m_cbUserTrustee, sizeof(StatusPlay.nTrusteeStatus));

			//游戏变量
			StatusPlay.cbBombCount=m_cbBombCount;
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.cbBankerScore=m_cbBankerScore;

			DWORD dwPassTime = (DWORD)time(NULL) - m_dwOperateTime;
			StatusPlay.cbTimeLeave = (BYTE)(StatusPlay.cbOutTimes - __min(dwPassTime, StatusPlay.cbOutTimes));

			//出牌信息
			StatusPlay.wTurnWiner=m_wTurnWiner;
			StatusPlay.cbTurnCardCount=m_cbTurnCardCount;
			CopyMemory(StatusPlay.cbTurnCardData,m_cbTurnCardData,m_cbTurnCardCount*sizeof(BYTE));

			//扑克信息
			CopyMemory(StatusPlay.cbBankerCard,m_cbBankerCard,sizeof(m_cbBankerCard));
			CopyMemory(StatusPlay.cbHandCardCount,m_cbHandCardCount,sizeof(m_cbHandCardCount));
			CopyMemory(StatusPlay.cbHandCardData,m_cbHandCardData[wChairID],sizeof(BYTE)*m_cbHandCardCount[wChairID]);

			CopyMemory(StatusPlay.cbPlayerMultiple, m_cbPlayerMultiple, sizeof(StatusPlay.cbPlayerMultiple));

			//发送场景
			bool bSendResult = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

			if (m_cbUserTrustee[wChairID] == TRUE)
			{
				CMD_S_TRUSTEE pSTrustee;
				pSTrustee.wTrusteeUser = wChairID;
				pSTrustee.bTrustee = FALSE;

				m_cbUserTrustee[wChairID] = FALSE;
				pIServerUserItem->SetTrusteeUser(false);

				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_TRUSTEE, &pSTrustee, sizeof(CMD_S_TRUSTEE));
				m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_TRUSTEE, &pSTrustee, sizeof(CMD_S_TRUSTEE));
			}

			return bSendResult;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//获取百人游戏是否下注状态而且玩家下注了 返回true就有断线重连
bool CTableFrameSink::OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if (m_pITableFrame->GetGameStatus() == GAME_SCENE_PLAY || m_pITableFrame->GetGameStatus() == GAME_SCENE_MULTIPLE || m_pITableFrame->GetGameStatus() == GAME_SCENE_CALL)
		return true;
	else
		return false;
}

//时间事件
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	if (wTimerID == IDI_AUTO_START)
	{
		m_pITableFrame->KillGameTimer(IDI_AUTO_START);

		bool bHaveStandUp = false;
		for (WORD i = 0; i < GAME_PLAYER; i++)
		{
			if (m_cbUserTrustee[i] == TRUE)
			{
				IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
				if (pServerUser)
				{
					m_pITableFrame->PerformStandUpAction(pServerUser);
					bHaveStandUp = true;
				}
			}
		}

		BYTE cbUserCount = 0;
		for (WORD i = 0; i < GAME_PLAYER; i++)
		{
			IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
			if (pServerUser)
			{
				pServerUser->SetUserStatus(US_READY, pServerUser->GetTableID(), pServerUser->GetChairID());
				cbUserCount++;
			}
		}

		if (cbUserCount >= GAME_PLAYER && !bHaveStandUp)
			m_pITableFrame->StartGame();

		return true;
	}
	if(wTimerID==IDI_USER_OUT_TIME)
	{
		m_pITableFrame->KillGameTimer(IDI_USER_OUT_TIME);

		if (m_wCurrentUser==INVALID_CHAIR) return true;

		IServerUserItem *pServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentUser);
		ASSERT(pServerUserItem!=NULL);
		if (pServerUserItem==NULL) return false;

		if (m_pITableFrame->GetGameStatus() != GAME_SCENE_MULTIPLE)
		{
			if (m_cbUserTrustee[m_wCurrentUser] == FALSE)
			{
				CMD_S_TRUSTEE pSTrustee;
				pSTrustee.wTrusteeUser = m_wCurrentUser;
				pSTrustee.bTrustee = TRUE;

				m_cbUserTrustee[m_wCurrentUser] = TRUE;
				pServerUserItem->SetTrusteeUser(true);

				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_TRUSTEE, &pSTrustee, sizeof(CMD_S_TRUSTEE));
				m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_TRUSTEE, &pSTrustee, sizeof(CMD_S_TRUSTEE));
			}
		}

		bool bTrusteeUser=pServerUserItem->IsTrusteeUser();
		BYTE cbGameStatus = m_pITableFrame->GetGameStatus();
		switch(cbGameStatus)
		{
		case GAME_SCENE_CALL:
			{
				if (m_cbBankerScore==0)
					OnUserCallScore(m_wCurrentUser,0x01);
				else
					OnUserCallScore(m_wCurrentUser,0);
				break;
			}
		case GAME_SCENE_MULTIPLE:
			{
				for (WORD i = 0; i < GAME_PLAYER; i++)
				{
					if (i == m_wBankerUser)
						continue;

					if (m_cbPlayerMultiple[i] == 0)
						OnUserDouble(i, 1);
				}
				break;
			}
		case GAME_SCENE_PLAY:
			{
				if(m_cbTurnCardCount == 0)
				{
					tagSearchCardResult SearchCardResult;
					m_GameLogic.SearchOutCard( m_cbHandCardData[m_wCurrentUser],m_cbHandCardCount[m_wCurrentUser],NULL,0,&SearchCardResult);
					if (SearchCardResult.cbSearchCount > 0)
					{
						OnUserOutCard(m_wCurrentUser, SearchCardResult.cbResultCard[0], SearchCardResult.cbCardCount[0]);
					}
					else
					{
						ASSERT(FALSE);
					}

				}
				else
				{
					tagSearchCardResult SearchCardResult;
					m_GameLogic.SearchOutCard(m_cbHandCardData[m_wCurrentUser], m_cbHandCardCount[m_wCurrentUser], m_cbTurnCardData, m_cbTurnCardCount, &SearchCardResult);
					if (SearchCardResult.cbSearchCount > 0)
					{
						OnUserOutCard(m_wCurrentUser, SearchCardResult.cbResultCard[0], SearchCardResult.cbCardCount[0]);
					}
					else
					{
						OnUserPassCard(m_wCurrentUser);
					}
				}					
				break;
			}
		}

		return true;
	}

	return false;
}

//数据事件
bool CTableFrameSink::OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//积分事件
bool CTableFrameSink::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	return false;
}

//游戏消息
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	m_cbWaitTime=0;
	switch (wSubCmdID)
	{
	case SUB_C_CALL_SCORE:	//用户叫分
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_CallScore));
			if (wDataSize!=sizeof(CMD_C_CallScore)) return true;

			//状态效验
			//ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_CALL);
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_CALL) return true;

			//用户效验
			//ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//变量定义
			CMD_C_CallScore * pCallScore=(CMD_C_CallScore *)pData;

			//消息处理
			WORD wChairID=pIServerUserItem->GetChairID();
			return OnUserCallScore(wChairID,pCallScore->cbCallScore);
		}
	case SUB_C_DOUBLE:		//用户加倍
		{
			//效验数据
			ASSERT(wDataSize == sizeof(CMD_C_CallDouble));
			if (wDataSize != sizeof(CMD_C_CallDouble)) return true;

			//状态效验
			//ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_CALL);
			if (m_pITableFrame->GetGameStatus() != GAME_SCENE_MULTIPLE) return true;

			//用户效验
			//ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus() != US_PLAYING) return true;

			//变量定义
			CMD_C_CallDouble * pCallDouble = (CMD_C_CallDouble *)pData;

			//消息处理
			WORD wChairID = pIServerUserItem->GetChairID();
			return OnUserDouble(wChairID, pCallDouble->cbPlayerMultiple);
		}
	case SUB_C_OUT_CARD:	//用户出牌
		{
			//变量定义
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pData;
			WORD wHeadSize=sizeof(CMD_C_OutCard)-sizeof(pOutCard->cbCardData);

			//效验数据
			ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pOutCard->cbCardCount*sizeof(BYTE))));
			if ((wDataSize<wHeadSize) || (wDataSize != (wHeadSize + pOutCard->cbCardCount*sizeof(BYTE)))) return true;

			//状态效验
			//ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;

			//用户效验
			//ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//消息处理
			WORD wChairID=pIServerUserItem->GetChairID();
			return OnUserOutCard(wChairID,pOutCard->cbCardData,pOutCard->cbCardCount);
		}
	case SUB_C_PASS_CARD:	//用户放弃
		{
			//状态效验
			//ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;

			//用户效验
			//ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//消息处理
			return OnUserPassCard(pIServerUserItem->GetChairID());
		}
	case SUB_C_TRUSTEE:
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_TRUSTEE));
			if (wDataSize != sizeof(CMD_C_TRUSTEE)) return true;

			CMD_C_TRUSTEE* pCTrustee = (CMD_C_TRUSTEE*)pData;
			CMD_S_TRUSTEE pSTrustee;
			pSTrustee.wTrusteeUser = pIServerUserItem->GetChairID();
			pSTrustee.bTrustee = pCTrustee->bTrustee;

			m_cbUserTrustee[pIServerUserItem->GetChairID()] = pCTrustee->bTrustee;

			if (pCTrustee->bTrustee == TRUE)
				pIServerUserItem->SetTrusteeUser(true);
			else
				pIServerUserItem->SetTrusteeUser(false);
			
			if (pIServerUserItem->GetChairID() == m_wCurrentUser)
				m_pITableFrame->SetGameTimer(IDI_USER_OUT_TIME, 1000, -1, NULL);

			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&pSTrustee,sizeof(CMD_S_TRUSTEE));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&pSTrustee,sizeof(CMD_S_TRUSTEE));
			return true;
		}
	}

	return true;
}

//框架消息
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//用户断线
bool CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if(m_bOffLineTrustee && wChairID == m_wCurrentUser)
	{
		IsOfflineTrustee();
	}
	return true;
}

//用户重入
bool CTableFrameSink::OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return true; 
}

//用户坐下
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//历史积分
	if (bLookonUser==false)
	{

	}

	return true;
}

//用户起立
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//历史积分
	if (bLookonUser==false)
	{

	}
	m_cbUserTrustee[wChairID] = 0;
	pIServerUserItem->SetTrusteeUser(false);
	return true;
}

//用户放弃
bool CTableFrameSink::OnUserPassCard(WORD wChairID)
{
	//效验状态
	ASSERT((wChairID==m_wCurrentUser)&&(m_cbTurnCardCount!=0));
	if ((wChairID!=m_wCurrentUser)||(m_cbTurnCardCount==0)) return true;

	//设置变量
	m_wCurrentUser=(m_wCurrentUser+1)%GAME_PLAYER;
	if (m_wCurrentUser==m_wTurnWiner) m_cbTurnCardCount=0;

	//构造消息
	CMD_S_PassCard PassCard;
	PassCard.wPassCardUser=wChairID;
	PassCard.wCurrentUser=m_wCurrentUser;

	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));

	IsOfflineTrustee();

	m_dwOperateTime = (DWORD)time(NULL);

	return true;
}

//用户叫分
bool CTableFrameSink::OnUserCallScore(WORD wChairID, BYTE cbCallScore)
{
	//效验状态
	ASSERT(wChairID==m_wCurrentUser);
	if (wChairID!=m_wCurrentUser) return true;

	//效验参数
	//ASSERT(((cbCallScore>=1)&&(cbCallScore<=3)&&(cbCallScore>m_cbBankerScore))||(cbCallScore==255));
	if ((cbCallScore<0)||(cbCallScore>3)||(cbCallScore<=m_cbBankerScore))
		cbCallScore = 0;

	//设置状态
	if (cbCallScore!=0)
	{
		m_cbBankerScore=cbCallScore;
		m_wBankerUser=m_wCurrentUser;
	}

	//设置叫分
	m_cbScoreInfo[wChairID]=cbCallScore;

	//设置用户
	if ((m_cbBankerScore==3)||(m_wFirstUser==(wChairID+1)%GAME_PLAYER))
	{
		m_wCurrentUser=INVALID_CHAIR;
	}
	else
	{
		m_wCurrentUser=(wChairID+1)%GAME_PLAYER;
	}

	//构造变量
	CMD_S_CallScore CallScore;
	CallScore.wCallScoreUser=wChairID;
	CallScore.wCurrentUser=m_wCurrentUser;
	CallScore.cbUserCallScore=cbCallScore;
	CallScore.cbCurrentScore=m_cbBankerScore;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CALL_SCORE,&CallScore,sizeof(CallScore));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CALL_SCORE,&CallScore,sizeof(CallScore));

	//开始判断
	if ((m_cbBankerScore==3)||(m_wFirstUser==(wChairID+1)%GAME_PLAYER))
	{
		//无人叫分
		if ( m_cbBankerScore == 0 )
		{
			if ((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)==0)
			{
				if(m_cbCallScoreCount<2)
				{
					m_cbCallScoreCount++;
					return OnEventGameStart();
				}
			}
			m_wBankerUser=m_wFirstUser;
			m_cbBankerScore=1;
		}

		//设置状态
		m_pITableFrame->SetGameStatus(GAME_SCENE_MULTIPLE);

		//设置变量
		if (m_cbBankerScore==0) m_cbBankerScore=1;
		if (m_wBankerUser==INVALID_CHAIR) m_wBankerUser=m_wFirstUser;
		m_cbCallScoreCount=0;

		m_cbPlayerMultiple[m_wBankerUser] = 1;

		//发送底牌
		m_cbHandCardCount[m_wBankerUser]+=CountArray(m_cbBankerCard);
		CopyMemory(&m_cbHandCardData[m_wBankerUser][NORMAL_COUNT],m_cbBankerCard,sizeof(m_cbBankerCard));

		//排列扑克
		m_GameLogic.SortCardList(m_cbHandCardData[m_wBankerUser],m_cbHandCardCount[m_wBankerUser],ST_ORDER);


		//设置用户
		m_wTurnWiner=m_wBankerUser;
		m_wCurrentUser=m_wBankerUser;

		//发送消息
		CMD_S_BankerInfo BankerInfo;
		BankerInfo.wBankerUser=m_wBankerUser;
		BankerInfo.cbBankerScore=m_cbBankerScore;
		CopyMemory(BankerInfo.cbBankerCard,m_cbBankerCard,sizeof(m_cbBankerCard));

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_BANKER_INFO,&BankerInfo,sizeof(BankerInfo));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_BANKER_INFO,&BankerInfo,sizeof(BankerInfo));

		
		IsOfflineTrustee();

		m_dwOperateTime = (DWORD)time(NULL);

		return true;

	}
	
	IsOfflineTrustee();

	m_dwOperateTime = (DWORD)time(NULL);

	return true;
}

//用户加倍
bool CTableFrameSink::OnUserDouble(WORD wChairID, BYTE cbDoubleTimes)
{
	if (wChairID == m_wBankerUser)
		return true;

	//效验参数
	if (cbDoubleTimes != 1 && cbDoubleTimes != 2)
		cbDoubleTimes = 1;

	//设置叫分
	m_cbPlayerMultiple[wChairID] = cbDoubleTimes;

	//构造变量
	CMD_S_Double CallDouble;
	CallDouble.wDoubleUser = wChairID;
	CallDouble.cbPlayerMultiple = cbDoubleTimes;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GAME_CALL_DOUBLE, &CallDouble, sizeof(CallDouble));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_CALL_DOUBLE, &CallDouble, sizeof(CallDouble));

	BYTE cbDoubleCount = 0;
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		if (i == m_wBankerUser)
			continue;

		if (m_cbPlayerMultiple[i] > 0)
			cbDoubleCount++;
	}

	//开始判断
	if (cbDoubleCount == 2)
	{
		m_wCurrentUser = m_wBankerUser;

		//设置状态
		m_pITableFrame->SetGameStatus(GAME_SCENE_PLAY);

		//发送消息
		CMD_S_BankerOut BankerOut;
		BankerOut.wBankerUser = m_wBankerUser;

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_BANKER_OUT, &BankerOut, sizeof(BankerOut));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_BANKER_OUT, &BankerOut, sizeof(BankerOut));

		IsOfflineTrustee();

		m_dwOperateTime = (DWORD)time(NULL);

		return true;
	}

	return true;
}

//用户出牌
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData[], BYTE cbCardCount)
{
	//效验状态
	ASSERT(wChairID==m_wCurrentUser);
	if (wChairID!=m_wCurrentUser) return true;

	//获取类型
	BYTE cbCardType=m_GameLogic.GetCardType(cbCardData,cbCardCount);

	//类型判断
	if (cbCardType==CT_ERROR) 
	{
		ASSERT(FALSE);
		return true;
	}

	//出牌判断
	if (m_cbTurnCardCount!=0)
	{
		//对比扑克
		if (m_GameLogic.CompareCard(m_cbTurnCardData,cbCardData,m_cbTurnCardCount,cbCardCount)==false)
		{
			ASSERT(FALSE);
			return true;
		}
	}

	//删除扑克
	if (m_GameLogic.RemoveCardList(cbCardData,cbCardCount,m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false)
	{
		ASSERT(FALSE);
		return true;
	}

	//记牌器剩余牌数
	for (int i = 0; i < cbCardCount; i++)
	{
		BYTE cbColor = m_GameLogic.GetCardColor(cbCardData[i]);
		BYTE cbValue = m_GameLogic.GetCardValue(cbCardData[i]);
		if (cbValue > 0)
		{
			m_cbLeftCardNum[cbValue - 1] -= 1;
		}
	}

	//出牌变量
	m_cbOutCardCount[wChairID]++;

	//设置变量
	m_cbTurnCardCount=cbCardCount;
	m_cbHandCardCount[wChairID]-=cbCardCount;
	CopyMemory(m_cbTurnCardData,cbCardData,sizeof(BYTE)*cbCardCount);

	//炸弹判断
	if ((cbCardType==CT_BOMB_CARD)||(cbCardType==CT_MISSILE_CARD)) 
	{
		m_cbBombCount++;
		m_cbEachBombCount[wChairID]++;
	}
	
	//火箭判断
	if (cbCardType == CT_MISSILE_CARD)
		m_cbHaveOutMissile = TRUE;

	//切换用户
	m_wTurnWiner=wChairID;
	if (m_cbHandCardCount[wChairID]!=0)
	{
		if (cbCardType!=CT_MISSILE_CARD)
		{
			m_wCurrentUser=(m_wCurrentUser+1)%GAME_PLAYER;
		}
	}
	else m_wCurrentUser=INVALID_CHAIR;

	//构造数据
	CMD_S_OutCard OutCard;
	ZeroMemory(&OutCard, sizeof(OutCard));
	OutCard.wOutCardUser=wChairID;
	OutCard.cbCardCount=cbCardCount;
	OutCard.wCurrentUser=m_wCurrentUser;
	CopyMemory(OutCard.cbCardData,m_cbTurnCardData,m_cbTurnCardCount*sizeof(BYTE));

	//发送数据
	WORD wHeadSize=sizeof(OutCard)-sizeof(OutCard.cbCardData);
	WORD wSendSize=wHeadSize+OutCard.cbCardCount*sizeof(BYTE);
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		m_pITableFrame->SendTableData(i, SUB_S_OUT_CARD, &OutCard, wSendSize);
		m_pITableFrame->SendLookonData(i, SUB_S_OUT_CARD, &OutCard, wSendSize);
	}

	//出牌最大
	if (cbCardType==CT_MISSILE_CARD) m_cbTurnCardCount=0;

	//结束判断
	if (m_wCurrentUser==INVALID_CHAIR) OnEventGameConclude(wChairID,NULL,GER_NORMAL);
	else
	{
		IsOfflineTrustee();
	}

	m_dwOperateTime = (DWORD)time(NULL);

	return true;
}

//设置基数
void CTableFrameSink::SetGameBaseScore(SCORE lBaseScore)
{
	
}

//是否托管
bool CTableFrameSink::IsOfflineTrustee()
{
	//非常规座位
	if(m_wCurrentUser==INVALID_CHAIR) return false;

	//允许代打
	if (m_bOffLineTrustee)
	{
		IServerUserItem *pServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentUser);
		ASSERT(pServerUserItem!=NULL);
		if (pServerUserItem==NULL) return false;

		//删除超时定时器
		m_pITableFrame->KillGameTimer(IDI_USER_OUT_TIME);

		//变量定义
		bool bTrusteeUser=pServerUserItem->IsTrusteeUser();
		DWORD dwTimerTime=0;
		
		if (bTrusteeUser == true && m_pITableFrame->GetGameStatus() != GAME_SCENE_MULTIPLE)
		{
			dwTimerTime+=2;
		}
		else
		{
			BYTE cbGameStatus=m_pITableFrame->GetGameStatus();
			if (cbGameStatus == GAME_SCENE_CALL)
				dwTimerTime=m_pGameCustomRule->cbTimeCallScore;
			else if (cbGameStatus == GAME_SCENE_MULTIPLE)
				dwTimerTime = m_pGameCustomRule->cbTimeHeadOutCard;
			else if (cbGameStatus==GAME_SCENE_PLAY)
				dwTimerTime=m_pGameCustomRule->cbTimeOutCard;
			

			dwTimerTime+= 2;
		}

		m_pITableFrame->SetGameTimer(IDI_USER_OUT_TIME,dwTimerTime*1000,-1,NULL);
		return true;
	}

	return false;
}

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
