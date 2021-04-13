#include "StdAfx.h"
#include "TableFrameSink.h"
#include "DlgCustomRule.h"
#include <conio.h>
#include <locale>

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

#define	IDI_SO_OPERATE							1							//代打定时器

#define IDI_WAIT_START							2							//等待开始

#define IDI_AUTO_START							3							//自动开始
//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink()
{
	m_pGameServiceOption = NULL;
	m_pGameServiceAttrib = NULL;

	//游戏变量	
	m_wPlayerCount=GAME_PLAYER;	

	m_lExitScore=0;	

	m_wBankerUser=INVALID_CHAIR;

	//用户状态
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	FillMemory(m_cbCallBankerTimes,sizeof(m_cbCallBankerTimes),0xFF);
	m_cbMaxBankerTimes = 0;
	ZeroMemory(m_bFleeUser, sizeof(m_bFleeUser));

	m_bWriteRoomStorage = true;

	//扑克变量
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	
	m_lMaxCardTimes	= 0;

	time(NULL);

	m_cbStartTime = 5;
	m_cbCallTime = 10;
	m_cbAddTime = 10;
	m_cbEndTime = 5;
	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{

}

//接口查询--检测相关信息版本
void * CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//初始化
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	m_pITableFrame->SetStartMode(START_MODE_ALL_READY);

	//游戏配置
	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	
	m_lAddChip[0] = 5;
	m_lAddChip[1] = 10;
	m_lAddChip[2] = 15;
	m_lAddChip[3] = 20;

	//获取自定义配置
	tagCustomRule *pCustomRule = (tagCustomRule *)m_pGameServiceOption->cbCustomRule;

	for (int i = 0; i < MAX_CONFIG; i++)
	{
		if (pCustomRule->lFreeConfig[i] != 0)
			m_lAddChip[i] = pCustomRule->lFreeConfig[i];
	}

	//读取配置
	ReadConfigInformation();	

	m_dwOperateTime = (DWORD)time(NULL);


	return true;
}

//复位桌子
void CTableFrameSink::RepositionSink()
{
	//游戏变量
	m_lExitScore=0;	
	m_wBankerUser = INVALID_CHAIR;

	//用户状态
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	FillMemory(m_cbCallBankerTimes, sizeof(m_cbCallBankerTimes), 0xFF);
	m_cbMaxBankerTimes = 0;

	//扑克变量
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	m_bWriteRoomStorage = true;

	return;
}

//用户断线
bool CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{

	return true;
}

//用户坐下
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	return true;
}

//用户起立
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	return true;
}

//用户同意
bool CTableFrameSink::OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{
	return true;
}

//游戏开始
bool CTableFrameSink::OnEventGameStart()
{
	ZeroMemory(m_bFleeUser, sizeof(m_bFleeUser));

	CMD_S_GameStart GameStart;

	GameStart.cbTime = m_cbStartTime;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_START, &GameStart, sizeof(GameStart));

	m_pITableFrame->SetGameTimer(IDI_AUTO_START, m_cbStartTime*1000, 1, 0L);
	
	m_dwOperateTime = (DWORD)time(NULL);

	//设置状态
	m_pITableFrame->SetGameStatus(GS_TK_WAIT);

	return true;
}

//自动开始
bool CTableFrameSink::OnGameAutoStart()
{
	BYTE cbAiCount = 0;
	BYTE cbPlayCount = 0;

	//用户状态
	for (WORD i = 0; i<m_wPlayerCount; i++)
	{
		//获取用户
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem == NULL)
		{
			m_cbPlayStatus[i] = FALSE;
		}
		else
		{
			m_cbPlayStatus[i] = TRUE;

			cbPlayCount++;
			if (pIServerUserItem->IsAndroidUser())
			{
				cbAiCount++;
			}
		}
	}

	if (cbAiCount == 0 || cbAiCount == cbPlayCount)
		m_bWriteRoomStorage = false;

	//库存衰减
	if (m_bWriteRoomStorage)
		m_pITableFrame->CalculateStorageDeduct();

	//随机扑克
	BYTE bTempArray[GAME_PLAYER*MAX_CARDCOUNT];
	m_GameLogic.RandCardList(bTempArray, sizeof(bTempArray));

	for (WORD i = 0; i<m_wPlayerCount; i++)
	{
		IServerUserItem *pIServerUser = m_pITableFrame->GetTableUserItem(i);
		if (pIServerUser == NULL)
		{
			continue;
		}

		//派发扑克
		CopyMemory(m_cbHandCardData[i], &bTempArray[i*MAX_CARDCOUNT], MAX_CARDCOUNT);
	}

	CMD_S_SendCard SendCard;
	ZeroMemory(&SendCard, sizeof(SendCard));

	CopyMemory(SendCard.cbPlayerStatus, m_cbPlayStatus, sizeof(SendCard.cbPlayerStatus));

	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		if (m_cbPlayStatus == FALSE)
			continue;

		CopyMemory(SendCard.cbCardData[i], m_cbHandCardData[i], sizeof(BYTE) * SEND_COUNT);
		m_pITableFrame->SendTableData(i, SUB_S_DISPATCH_CARD, &SendCard, sizeof(SendCard));

		ZeroMemory(SendCard.cbCardData, sizeof(SendCard.cbCardData));
	}
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_DISPATCH_CARD, &SendCard, sizeof(SendCard));
	
	m_pITableFrame->SetGameStatus(GS_TK_CALL);

	m_pITableFrame->SetGameTimer(IDI_SO_OPERATE, m_cbCallTime * 1000, 1, 0);

	m_dwOperateTime = (DWORD)time(NULL);

	return true;
}

//游戏结束
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	BYTE cbNowPlayerCount = 0;
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		if (m_cbPlayStatus[i] == TRUE)
			cbNowPlayerCount++;
	}

	BYTE cbEndTime = cbNowPlayerCount + m_cbEndTime + 5;

	switch (cbReason)
	{
	case GER_DISMISS:		//游戏解散
		{
			//删除时间
			m_pITableFrame->KillGameTimer(IDI_SO_OPERATE);

			//构造数据
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd, sizeof(GameEnd));

			GameEnd.cbFleeEnd = TRUE;

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			m_pITableFrame->SetGameTimer(IDI_WAIT_START, cbEndTime * 1000, 1, 0L);

			m_pITableFrame->SetGameStatus(GS_TK_END);

			m_dwOperateTime = (DWORD)time(NULL);

			m_wBankerUser = INVALID_CHAIR;

			return true;
		}
	case GER_NORMAL:		//常规结束
		{
			//删除时间
			m_pITableFrame->KillGameTimer(IDI_SO_OPERATE);

			//库存控制
			tag_OxSix_Parameter OxSix_Parameter;
			ZeroMemory(&OxSix_Parameter, sizeof(OxSix_Parameter));

			OxSix_Parameter.wBankerUser = m_wBankerUser;
			CopyMemory(OxSix_Parameter.cbPlayerStatus, m_cbPlayStatus, sizeof(OxSix_Parameter.cbPlayerStatus));
			OxSix_Parameter.lExitScore = m_lExitScore;
			CopyMemory(OxSix_Parameter.lTableScore, m_lTableScore, sizeof(OxSix_Parameter.lTableScore));
			CopyMemory(OxSix_Parameter.cbCallBankerTimes, m_cbCallBankerTimes, sizeof(OxSix_Parameter.cbCallBankerTimes));
			CopyMemory(OxSix_Parameter.cbTableCardArray, m_cbHandCardData, sizeof(OxSix_Parameter.cbTableCardArray));

			tag_OxSix_Result pOxSix_Result;
			ZeroMemory(&pOxSix_Result, sizeof(pOxSix_Result));
			m_pITableFrame->GetControlResult(&OxSix_Parameter, sizeof(OxSix_Parameter), &pOxSix_Result);
			CopyMemory(m_cbHandCardData, pOxSix_Result.cbTableCardArray, sizeof(m_cbHandCardData));

			//定义变量
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			WORD wWinTimes[GAME_PLAYER];
			WORD wWinCount[GAME_PLAYER];
			ZeroMemory(wWinCount,sizeof(wWinCount));
			ZeroMemory(wWinTimes,sizeof(wWinTimes));

			//保存扑克
			BYTE cbUserCardData[GAME_PLAYER][MAX_CARDCOUNT] = {0};
			CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));
			
			//庄家倍数
			wWinTimes[m_wBankerUser] = m_GameLogic.GetTimes(cbUserCardData[m_wBankerUser], MAX_CARDCOUNT);

			//对比玩家
			for (WORD i = 0; i < m_wPlayerCount; i++)
			{
				if (i == m_wBankerUser || m_cbPlayStatus[i] == FALSE)continue;

				//对比扑克
				if (m_GameLogic.CompareCard(cbUserCardData[i], cbUserCardData[m_wBankerUser], MAX_CARDCOUNT))
				{
					wWinCount[i]++;
					//获取倍数
					wWinTimes[i] = m_GameLogic.GetTimes(cbUserCardData[i], MAX_CARDCOUNT);
				}
				else
				{
					wWinCount[m_wBankerUser]++;
				}
			}

			//统计得分
			for (WORD i = 0; i < m_wPlayerCount; i++)
			{
				if (i == m_wBankerUser || m_cbPlayStatus[i] == FALSE)continue;

				if (wWinCount[i] > 0)	//闲家胜利
				{
					GameEnd.lGameScore[i] = m_lTableScore[i] * wWinTimes[i] * m_pITableFrame->GetCellScore() * m_cbMaxBankerTimes;
					GameEnd.lGameScore[m_wBankerUser] -= GameEnd.lGameScore[i];
					m_lTableScore[i] = 0;
				}
				else					//庄家胜利
				{
					GameEnd.lGameScore[i] = (-1)*m_lTableScore[i] * wWinTimes[m_wBankerUser] * m_pITableFrame->GetCellScore() * m_cbMaxBankerTimes;

					IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);
					if (pServerUser && (pServerUser->GetUserScore() + GameEnd.lGameScore[i]) < 0)
					{
						GameEnd.lGameScore[i] = (-1) * pServerUser->GetUserScore();
					}


					GameEnd.lGameScore[m_wBankerUser] += (-1)*GameEnd.lGameScore[i];
					m_lTableScore[i] = 0;
				}
			}

			//闲家强退分数	
			GameEnd.lGameScore[m_wBankerUser] += m_lExitScore;

			//离开用户
			for (WORD i = 0; i < m_wPlayerCount; i++)
			{
				if (m_lTableScore[i] > 0)
					GameEnd.lGameScore[i] = -m_lTableScore[i];
			}

			//庄家钱不够赔，就按赢了的人的比例分庄家的所有钱
			if (m_wBankerUser != INVALID_CHAIR)
			{
				IServerUserItem *pBankerUserItem = m_pITableFrame->GetTableUserItem(m_wBankerUser);

				if (pBankerUserItem)
				{
					SCORE lBankerScore = pBankerUserItem->GetUserScore();
					if (GameEnd.lGameScore[m_wBankerUser] < 0 && (GameEnd.lGameScore[m_wBankerUser] + lBankerScore) < 0)
					{
						SCORE lAllWinScore = 0;
						SCORE lAllLostScore = 0;
						for (WORD i = 0; i < GAME_PLAYER; i++)
						{
							if (i == m_wBankerUser)
								continue;
							if (GameEnd.lGameScore[i] > 0)
							{
								lAllWinScore += GameEnd.lGameScore[i];
							}
							else if (GameEnd.lGameScore[i] < 0)
							{
								lAllLostScore += GameEnd.lGameScore[i];
							}
						}

						GameEnd.lGameScore[m_wBankerUser] = (-1) * lBankerScore;
						lAllLostScore = lAllLostScore * (-1);
						for (WORD i = 0; i < GAME_PLAYER; i++)
						{
							if (i == m_wBankerUser) continue;
							if (GameEnd.lGameScore[i] > 0)
							{
								GameEnd.lGameScore[i] = (GameEnd.lGameScore[i] / lAllWinScore) * (lBankerScore + lAllLostScore);
							}
						}
					}
				}
			}
			
			//修改积分
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));

			//积分税收
			for(WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_cbPlayStatus[i]==FALSE)
					continue;

				SCORE lRevenue = 0;

				if(GameEnd.lGameScore[i]>0L)
				{
					lRevenue = m_pITableFrame->CalculateRevenue(i,GameEnd.lGameScore[i]);
					if (lRevenue>0)
						GameEnd.lGameScore[i] -= lRevenue;
				}

				//保存积分
				ScoreInfoArray[i].lScore = GameEnd.lGameScore[i];
				ScoreInfoArray[i].lRevenue = lRevenue;
				ScoreInfoArray[i].cbType = (GameEnd.lGameScore[i]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;

				IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);
				if (pServerUser)
				{
					GameEnd.lPlayerScore[i] = pServerUser->GetUserScore() + GameEnd.lGameScore[i];

					//库存值
					if (pServerUser->IsAndroidUser() && m_bWriteRoomStorage)
						m_pGameServiceOption->pRoomStorageOption.lStorageCurrent += GameEnd.lGameScore[i];
				}
			}
		
			CopyMemory(GameEnd.cbHandCardData, m_cbHandCardData, sizeof(m_cbHandCardData));

			//获取玩家牌型			
			for(WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_cbPlayStatus[i]==FALSE)
				{
					continue;
				}
				
				GameEnd.cbCardType[i] = m_GameLogic.GetCardType(GameEnd.cbHandCardData[i],MAX_CARDCOUNT);
			}

			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			
			m_pITableFrame->SetGameStatus(GS_TK_END);

			TryWriteTableScore(ScoreInfoArray);

			//大厅跑马灯消息
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
				if (pServerUser && GameEnd.lGameScore[i] > 500)
				{
					CMD_GR_GameRadioMessage GameRadioMessage;
					ZeroMemory(&GameRadioMessage, sizeof(GameRadioMessage));

					BYTE cbCardType = m_GameLogic.GetCardType(GameEnd.cbHandCardData[i], MAX_CARDCOUNT);

					GameRadioMessage.cbCardType = cbCardType + 1;
					GameRadioMessage.wKindID = m_pGameServiceOption->wKindID;
					GameRadioMessage.lScore = GameEnd.lGameScore[i];
					lstrcpyn(GameRadioMessage.szNickName, pServerUser->GetNickName(), CountArray(GameRadioMessage.szNickName));
					m_pITableFrame->SendHallRadioMessage(&GameRadioMessage, sizeof(GameRadioMessage));
				}
			}

			m_pITableFrame->SetGameTimer(IDI_WAIT_START, cbEndTime * 1000, 1, 0L);

			m_dwOperateTime = (DWORD)time(NULL);
			
			return true;
		}
	case GER_USER_LEAVE:		//用户强退
	case GER_NETWORK_ERROR:
		{
			//效验参数
			ASSERT(pIServerUserItem!=NULL);

			if(m_cbPlayStatus[wChairID]==FALSE) 
				return true;
			
			//下注逃跑
			if (m_pITableFrame->GetGameStatus() == GS_TK_SCORE)
			{
				if (m_lTableScore[wChairID] == 0 && wChairID != m_wBankerUser)
				{
					OnUserAddScore(wChairID,m_lAddChip[0]);
				}
				return true;
			}
			//叫庄状态逃跑
			else if (m_pITableFrame->GetGameStatus() == GS_TK_CALL)
			{
				if (m_cbCallBankerTimes[wChairID] == 0xFF)
				{
					OnUserCallBanker(wChairID,0);
				}
				return true;
			}

			return true;
		}
	}

	return false;
}

//发送场景
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:		//空闲状态
	case GS_TK_WAIT:
	case GS_TK_END:
		{
			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//设置变量
			StatusFree.lCellScore = m_pITableFrame->GetCellScore();
			
			CopyMemory(StatusFree.cbPlayerStatus, m_cbPlayStatus, sizeof(StatusFree.cbPlayerStatus));
			StatusFree.cbStartTime = m_cbStartTime;
			StatusFree.cbCallTime = m_cbCallTime;
			StatusFree.cbAddTime = m_cbAddTime;
			StatusFree.cbEndTime = m_cbEndTime;

			DWORD dwPassTime = (DWORD)time(NULL) - m_dwOperateTime;
			if (cbGameStatus == GS_TK_WAIT)
				StatusFree.cbTimeLeave = (BYTE)(m_cbStartTime - __min(dwPassTime, (DWORD)m_cbStartTime));
			else if (cbGameStatus == GS_TK_END)
				StatusFree.cbTimeLeave = (BYTE)(m_cbEndTime - __min(dwPassTime, (DWORD)m_cbEndTime));

			bool bStatus = m_pITableFrame->SendGameScene(pIServerUserItem, &StatusFree, sizeof(StatusFree));

			//获取自定义配置
			tagCustomRule *pCustomRule = (tagCustomRule *)m_pGameServiceOption->cbCustomRule;
			ASSERT(pCustomRule);

			tagCustomAndroid CustomAndroid;
			ZeroMemory(&CustomAndroid, sizeof(CustomAndroid));
			CustomAndroid.lRobotBankGet = pCustomRule->lRobotBankGet;
			CustomAndroid.lRobotBankGetBanker = pCustomRule->lRobotBankGetBanker;
			CustomAndroid.lRobotBankStoMul = pCustomRule->lRobotBankStoMul;
			CustomAndroid.lRobotScoreMax = pCustomRule->lRobotScoreMax;
			CustomAndroid.lRobotScoreMin = pCustomRule->lRobotScoreMin;
			CopyMemory(CustomAndroid.lAddChip, m_lAddChip, sizeof(CustomAndroid.lAddChip));
			m_pITableFrame->SendTableData(wChairID, SUB_S_CUSTOM_ANDROID, &CustomAndroid, sizeof(CustomAndroid));

			//发送场景
			return bStatus;
		}
	case GS_TK_CALL:	//叫庄状态
		{
			//构造数据
			CMD_S_StatusCall StatusCall;
			ZeroMemory(&StatusCall,sizeof(StatusCall));

			StatusCall.lCellScore = m_pITableFrame->GetCellScore();

			//设置变量
			CopyMemory(StatusCall.cbPlayerStatus, m_cbPlayStatus, sizeof(StatusCall.cbPlayerStatus));
			
			StatusCall.cbStartTime = m_cbStartTime;
			StatusCall.cbCallTime = m_cbCallTime;
			StatusCall.cbAddTime = m_cbAddTime;
			StatusCall.cbEndTime = m_cbEndTime;

			DWORD dwPassTime = (DWORD)time(NULL) - m_dwOperateTime;
			StatusCall.cbTimeLeave = (BYTE)(m_cbCallTime - __min(dwPassTime, (DWORD)m_cbCallTime));
			
			CopyMemory(StatusCall.cbCallBankerTimes, m_cbCallBankerTimes, sizeof(StatusCall.cbCallBankerTimes));
			//派发扑克(开始只发三张牌)
			CopyMemory(StatusCall.cbCardData[wChairID], m_cbHandCardData[wChairID], sizeof(BYTE) * 3);

			bool bStatus = m_pITableFrame->SendGameScene(pIServerUserItem, &StatusCall, sizeof(StatusCall));

			//获取自定义配置
			tagCustomRule *pCustomRule = (tagCustomRule *)m_pGameServiceOption->cbCustomRule;
			ASSERT(pCustomRule);
			tagCustomAndroid CustomAndroid;
			ZeroMemory(&CustomAndroid, sizeof(CustomAndroid));
			CustomAndroid.lRobotBankGet = pCustomRule->lRobotBankGet;
			CustomAndroid.lRobotBankGetBanker = pCustomRule->lRobotBankGetBanker;
			CustomAndroid.lRobotBankStoMul = pCustomRule->lRobotBankStoMul;
			CustomAndroid.lRobotScoreMax = pCustomRule->lRobotScoreMax;
			CustomAndroid.lRobotScoreMin = pCustomRule->lRobotScoreMin;
			CopyMemory(CustomAndroid.lAddChip, m_lAddChip, sizeof(CustomAndroid.lAddChip));
			m_pITableFrame->SendTableData(wChairID, SUB_S_CUSTOM_ANDROID, &CustomAndroid, sizeof(CustomAndroid));

			//发送场景
			return bStatus;
		}
	case GS_TK_SCORE:	//下注状态
		{
			//构造数据
			CMD_S_StatusScore StatusScore;
			ZeroMemory(&StatusScore,sizeof(StatusScore));

			StatusScore.lCellScore = m_pITableFrame->GetCellScore();

			//加注信息
			StatusScore.wBankerUser=m_wBankerUser;
			StatusScore.cbBankerTimes = m_cbMaxBankerTimes;
			CopyMemory(StatusScore.cbPlayerStatus, m_cbPlayStatus, sizeof(StatusScore.cbPlayerStatus));

			StatusScore.cbStartTime = m_cbStartTime;
			StatusScore.cbCallTime = m_cbCallTime;
			StatusScore.cbAddTime = m_cbAddTime;
			StatusScore.cbEndTime = m_cbEndTime;

			DWORD dwPassTime = (DWORD)time(NULL) - m_dwOperateTime;
			StatusScore.cbTimeLeave = (BYTE)(m_cbAddTime - __min(dwPassTime, (DWORD)m_cbAddTime));

			CopyMemory(StatusScore.lBetTimes, m_lTableScore, sizeof(StatusScore.lBetTimes));
			//派发扑克(开始只发三张牌)
			CopyMemory(StatusScore.cbCardData[wChairID], m_cbHandCardData[wChairID], sizeof(BYTE) * 3);

			bool bStatus = m_pITableFrame->SendGameScene(pIServerUserItem, &StatusScore, sizeof(StatusScore));

			//获取自定义配置
			tagCustomRule *pCustomRule = (tagCustomRule *)m_pGameServiceOption->cbCustomRule;
			ASSERT(pCustomRule);
			tagCustomAndroid CustomAndroid;
			ZeroMemory(&CustomAndroid, sizeof(CustomAndroid));
			CustomAndroid.lRobotBankGet = pCustomRule->lRobotBankGet;
			CustomAndroid.lRobotBankGetBanker = pCustomRule->lRobotBankGetBanker;
			CustomAndroid.lRobotBankStoMul = pCustomRule->lRobotBankStoMul;
			CustomAndroid.lRobotScoreMax = pCustomRule->lRobotScoreMax;
			CustomAndroid.lRobotScoreMin = pCustomRule->lRobotScoreMin;
			CopyMemory(CustomAndroid.lAddChip, m_lAddChip, sizeof(CustomAndroid.lAddChip));
			m_pITableFrame->SendTableData(wChairID, SUB_S_CUSTOM_ANDROID, &CustomAndroid, sizeof(CustomAndroid));

			//发送场景
			return bStatus;
		}
	}
	//效验错误
	ASSERT(FALSE);

	return false;
}

//定时器事件
bool CTableFrameSink::OnTimerMessage(DWORD dwTimerID, WPARAM wBindParam)
{

	switch(dwTimerID)
	{
	case IDI_WAIT_START:
	{
		m_pITableFrame->KillGameTimer(IDI_WAIT_START);
		
		m_pITableFrame->ConcludeGame(GS_TK_FREE);

		for (WORD i = 0; i < GAME_PLAYER; i++)
		{
			if (m_bFleeUser[i] == 2)
			{
				IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
				if (pServerUser)
					m_pITableFrame->PerformStandUpAction(pServerUser);
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

		if (cbUserCount >= 2)
			m_pITableFrame->StartGame();
		return true;
	}
	case IDI_AUTO_START:
		{
			//删除时间
			m_pITableFrame->KillGameTimer(IDI_AUTO_START);
			OnGameAutoStart();
			return true;
		}
	case IDI_SO_OPERATE:
		{
			//删除时间
			m_pITableFrame->KillGameTimer(IDI_SO_OPERATE);

			//游戏状态
			switch( m_pITableFrame->GetGameStatus() )
			{
				case GS_TK_CALL:			//用户叫庄
					{
						for (WORD i=0;i<m_wPlayerCount;i++)
						{
							if(m_cbPlayStatus[i]!=TRUE) 
							{
								continue;
							}
							if (m_cbCallBankerTimes[i] != 0xFF)
							{
								continue;
							}
							OnUserCallBanker(i, 0);
							//设置状态
							m_bFleeUser[i] ++;
						}

						break;
					}
				case GS_TK_SCORE:			//下注操作
					{
						for(WORD i=0;i<m_wPlayerCount;i++)
						{
							if(m_lTableScore[i]>0L || m_cbPlayStatus[i]==FALSE || i==m_wBankerUser)
								continue;

							OnUserAddScore(i, m_lAddChip[0]);
							//设置状态
							m_bFleeUser[i] ++;
						}

						break;
					}
			}
			return true;
		}
	}
	return false;
}

//游戏消息处理 
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_CALL_BANKER:			//用户叫庄
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_CallBanker));
			if (wDataSize != sizeof(CMD_C_CallBanker)) return true;

			//变量定义
			CMD_C_CallBanker * pCallBanker=(CMD_C_CallBanker *)pDataBuffer;

			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

			//状态判断
			ASSERT (m_cbPlayStatus[pUserData->wChairID] == TRUE);
			if (m_cbPlayStatus[pUserData->wChairID] != TRUE) 
			{
				return true;
			}

			//消息处理
			return OnUserCallBanker(pUserData->wChairID, pCallBanker->cbBankerTimes);
		}
	case SUB_C_ADD_SCORE:			//用户加注
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_AddScore));
			if (wDataSize!=sizeof(CMD_C_AddScore)) 
			{
				return true;
			}

			//变量定义
			CMD_C_AddScore * pAddScore=(CMD_C_AddScore *)pDataBuffer;

			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

			//状态判断
			ASSERT (m_cbPlayStatus[pUserData->wChairID] == TRUE);
			if (m_cbPlayStatus[pUserData->wChairID] != TRUE) 
			{
				return true;
			}

			//消息处理
			return OnUserAddScore(pUserData->wChairID, pAddScore->nScore);
		}
	}

	return true;
}

//框架消息处理
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	
	return false;
}

//叫庄事件
bool CTableFrameSink::OnUserCallBanker(WORD wChairID, BYTE cbBankerTimes)
{
	//状态效验
	BYTE cbGameStatus = m_pITableFrame->GetGameStatus();
	ASSERT(cbGameStatus==GS_TK_CALL);
	if (cbGameStatus!=GS_TK_CALL) return true;

	if (cbBankerTimes != 0 && cbBankerTimes != 1 && cbBankerTimes != 2 && cbBankerTimes != 4)
		return true;

	//设置变量
	m_cbCallBankerTimes[wChairID] = cbBankerTimes;

	//设置变量
	CMD_S_CallBankerInfo CallBanker;
	ZeroMemory(&CallBanker, sizeof(CallBanker));

	CallBanker.wCallUser = wChairID;
	CallBanker.cbCallBankerTimes = cbBankerTimes;


	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CALL_BANKER, &CallBanker, sizeof(CallBanker));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CALL_BANKER, &CallBanker, sizeof(CallBanker));

	//叫庄人数
	WORD wCallUserCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbCallBankerTimes[i] != 0xFF) 
			wCallUserCount++;
		else if(m_cbPlayStatus[i]!=TRUE) 
			wCallUserCount++;
	}

	//全部人叫完庄，下注开始
	if(wCallUserCount==m_wPlayerCount)
	{
		//叫庄最大倍数
		BYTE cbMaxBankerTimes = cbBankerTimes;
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_cbPlayStatus[i]==TRUE && m_cbCallBankerTimes[i] > cbMaxBankerTimes) 
			{
				cbMaxBankerTimes = m_cbCallBankerTimes[i];
			}
		}

		m_cbMaxBankerTimes = cbMaxBankerTimes;

		//叫庄最大倍数的人数和CHAIRID
		BYTE cbMaxBankerCount = 0;
		WORD pwMaxBankerTimesChairID[GAME_PLAYER];

		FillMemory(pwMaxBankerTimesChairID, sizeof(pwMaxBankerTimesChairID), INVALID_CHAIR);

		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_cbPlayStatus[i]==TRUE && m_cbCallBankerTimes[i] == cbMaxBankerTimes) 
			{
				pwMaxBankerTimesChairID[cbMaxBankerCount++] = i;
			}
		}
		
		m_wBankerUser = pwMaxBankerTimesChairID[rand() % cbMaxBankerCount];

		if (m_cbCallBankerTimes[m_wBankerUser] == 0)
		{
			m_cbCallBankerTimes[m_wBankerUser] = 1;
			m_cbMaxBankerTimes = 1;
		}

		//设置状态
		m_pITableFrame->SetGameStatus(GS_TK_SCORE);

		//设置变量
		CMD_S_SetBanker SetBanker;
		ZeroMemory(&SetBanker, sizeof(SetBanker));
		
		SetBanker.wBankerUser = m_wBankerUser;
		CopyMemory(SetBanker.wCallList, pwMaxBankerTimesChairID, sizeof(SetBanker.wCallList));
	
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_SET_BANKER, &SetBanker, sizeof(SetBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_SET_BANKER, &SetBanker, sizeof(SetBanker));

		m_pITableFrame->SetGameTimer(IDI_SO_OPERATE, m_cbAddTime*1000, 1, 0);

		m_dwOperateTime = (DWORD)time(NULL);
	}

	return true;
}

//加注事件
bool CTableFrameSink::OnUserAddScore(WORD wChairID, LONGLONG lScore)
{
	//状态效验
	BYTE cbGameStatus = m_pITableFrame->GetGameStatus();
	ASSERT(cbGameStatus==GS_TK_SCORE);
	if (cbGameStatus!=GS_TK_SCORE) 
	{
		return true;
	}
	
	//庄家校验
	if (wChairID == m_wBankerUser)
	{
		return true;
	}

	//金币效验
	if (lScore != m_lAddChip[0] && lScore != m_lAddChip[1] && lScore != m_lAddChip[2] && lScore != m_lAddChip[3])
	{
		return true;
	}

	//下注金币
	m_lTableScore[wChairID]=lScore;
	//构造数据
	CMD_S_AddScore AddScore;
	AddScore.wBetUser = wChairID;
	AddScore.nBetTimes = m_lTableScore[wChairID];

	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_PLAYER_BET, &AddScore, sizeof(AddScore));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_PLAYER_BET, &AddScore, sizeof(AddScore));

	//下注人数
	BYTE bUserCount=0;
	for(WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_lTableScore[i]>0L)
		{
			bUserCount++;
		}
		else if(m_cbPlayStatus[i]==FALSE || i==m_wBankerUser)
		{
			bUserCount++;
		}
	}
	
	//闲家全到
	if(bUserCount==m_wPlayerCount)
	{
		OnEventGameConclude(INVALID_CHAIR, NULL, GER_NORMAL);
	}

	return true;
}

bool CTableFrameSink::TryWriteTableScore(tagScoreInfo ScoreInfoArray[])
{
	//修改积分
	tagScoreInfo ScoreInfo[GAME_PLAYER];
	ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
	CopyMemory(ScoreInfo,ScoreInfoArray,sizeof(ScoreInfo));

	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		IServerUserItem *pItem=m_pITableFrame->GetTableUserItem(i);
		if (pItem != NULL && m_cbPlayStatus[i] == TRUE)
		{
			m_pITableFrame->WriteUserScore(i, ScoreInfo[i]);
		}
	}
	return true;
}

//获取百人游戏是否下注状态而且玩家下注了 返回true就有断线重连
bool CTableFrameSink::OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if (m_cbPlayStatus[wChairID] == TRUE && (m_pITableFrame->GetGameStatus() == GS_TK_CALL || m_pITableFrame->GetGameStatus() == GS_TK_SCORE))
		return true;
	else
		return false;
}

//查询限额
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	return 0L;
}

//读取配置
void CTableFrameSink::ReadConfigInformation()
{	
	//获取自定义配置
	tagCustomRule *pCustomRule = (tagCustomRule *)m_pGameServiceOption->cbCustomRule;
	ASSERT(pCustomRule);
	
}

//////////////////////////////////////////////////////////////////////////
