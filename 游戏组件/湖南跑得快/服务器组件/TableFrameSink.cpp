#include "StdAfx.h"
#include "TableFrameSink.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//游戏人数

#define IDI_GIVE_UP				1											//要不起自动弃牌
#define TIME_GIVE_UP			1000										//自动弃牌时间

#define IDI_AUTO_START			2											//开始定时器
#define TIME_AUTO_START			3000										//开始时间

#define IDI_WAIT_START			3											//等待开始定时器
#define TIME_WAIT_START			8000										//等待开始时间

#define IDI_AUTO_OUT			4											//托管出牌
#define TIME_AUTO_OUT			1000										//托管出牌时间

#define IDI_OVER_TIME			5											//超时处理
#define TIME_OVER_TIME			30000										//超时时间

//构造函数
CTableFrameSink::CTableFrameSink()
{
	//游戏变量
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_bBombCount,sizeof(m_bBombCount));
	ZeroMemory(m_lGameScore,sizeof(m_lGameScore));

	ZeroMemory(m_bAutoStatus, sizeof(m_bAutoStatus));

	//历史积分
	ZeroMemory(m_lAllTurnScore,sizeof(m_lAllTurnScore));
	ZeroMemory(m_lLastTurnScore,sizeof(m_lLastTurnScore));

	//运行信息
	m_bBaoPei=false;
	m_bTurnCardCount=0;
	m_bTurnCardType=CT_ERROR;
	m_wTurnWiner=INVALID_CHAIR;
	ZeroMemory(m_bTurnCardData,sizeof(m_bTurnCardData));

	//扑克信息
	ZeroMemory(m_bCardCount,sizeof(m_bCardCount));
	ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));

	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

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
	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	m_lCellScore=m_pGameServiceOption->lCellScore;
	if(m_lCellScore<=0) m_lCellScore=1;
	//开始模式
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);

	return true;
}

//复位桌子
VOID CTableFrameSink::RepositionSink()
{
	//游戏变量
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_bBombCount,sizeof(m_bBombCount));
	ZeroMemory(m_lGameScore,sizeof(m_lGameScore));

	//运行信息
	m_bBaoPei=false;
	m_bTurnCardCount=0;
	m_bTurnCardType=CT_ERROR;
	m_wTurnWiner=INVALID_CHAIR;
	ZeroMemory(m_bTurnCardData,sizeof(m_bTurnCardData));

	//扑克信息
	ZeroMemory(m_bCardCount,sizeof(m_bCardCount));
	ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));


	return;
}

//游戏开始
bool CTableFrameSink::OnEventGameStart()
{
	m_pITableFrame->KillGameTimer(IDI_AUTO_START);
	m_pITableFrame->KillGameTimer(IDI_WAIT_START);

	ZeroMemory(m_bAutoStatus, sizeof(m_bAutoStatus));

	//设置状态
	m_pITableFrame->SetGameStatus(GAME_SCENE_PLAY);

	//混乱扑克
	BYTE bRandCard[48];
	m_GameLogic.RandCardList(bRandCard,CountArray(bRandCard));

	//分发扑克
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_bCardCount[i]=16;
		CopyMemory(&m_bHandCardData[i],&bRandCard[i*m_bCardCount[i]],sizeof(BYTE)*m_bCardCount[i]);
		m_GameLogic.SortCardList(m_bHandCardData[i],m_bCardCount[i]);
	}

	//设置用户
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		for (BYTE j=0;j<m_bCardCount[i];j++)
		{
			if (m_bHandCardData[i][j]==0x33) 
			{
				//设置用户
				m_wBankerUser=i;
				m_wCurrentUser=i;
			}
		}
	}

	//发送扑克
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart, sizeof(GameStart));
	GameStart.wBankerUser=m_wBankerUser;

	GameStart.wEndTime = 30;
	GameStart.wOutCardTime = 30;

	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		CopyMemory(GameStart.cbCardData, m_bHandCardData[i], sizeof(GameStart.cbCardData));
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		ZeroMemory(GameStart.cbCardData, sizeof(GameStart.cbCardData));
	}

	IsOffLineTrustee();
	return true;
}

//游戏结束
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_DISMISS:		//游戏解散
		{
			//效验参数
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount);

			//构造数据
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//剩余扑克
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.cbCardCount[i] = m_bCardCount[i];
				CopyMemory(GameEnd.bCardData[i],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
			}

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//结束游戏
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);

			m_pITableFrame->SetGameTimer(IDI_WAIT_START, TIME_WAIT_START, 1, 0);

			return true;
		}
	case GER_NORMAL:		//常规结束
		{
			//定义变量
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//剩余扑克
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.cbCardCount[i] = m_bCardCount[i];
				CopyMemory(GameEnd.bCardData[i],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));

				if (m_bCardCount[i] == 0)
					GameEnd.wWinUser = i;
			}

			//统计积分
			SCORE lCellScore = m_pGameServiceOption->lCellScore;
			
			//积分统计
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//游戏积分
				if (m_bCardCount[i]>1)
				{
					LONGLONG lScoreTimes=(m_bCardCount[i]==16)?2L:1L;

					SCORE lScore = (SCORE)m_bCardCount[i] * lScoreTimes;

					GameEnd.lGameScore[i] -= lScore*lCellScore;
					GameEnd.lGameScore[wChairID] += lScore*lCellScore;

					if (lScoreTimes == 2)
					{
						GameEnd.cbQuanGuan[i] = true;
					}
				}
			}

			//包赔处理
			if (m_bBaoPei==true)
			{
				WORD wBaoPeiUser=(wChairID+m_wPlayerCount-1)%m_wPlayerCount;
				GameEnd.lGameScore[wBaoPeiUser]=-GameEnd.lGameScore[wChairID];
				GameEnd.lGameScore[(wBaoPeiUser+2)%m_wPlayerCount]=0L;
				GameEnd.cbBaopei[wBaoPeiUser] = true;
			}

			//炸弹积分
			for (WORD i = 0; i < m_wPlayerCount; i++)
			{
				GameEnd.lGameScore[i] += m_lGameScore[i];
				GameEnd.lBombScore[i] += m_lGameScore[i];
				GameEnd.cbBombCount[i] = m_bBombCount[i];

				IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);
				if (pServerUser)
				{
					lstrcpyn(GameEnd.szPlayerName[i], pServerUser->GetNickName(), sizeof(GameEnd.szPlayerName[i]));
				}
			}
			

			//积分统计
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				m_lAllTurnScore[i]+=GameEnd.lGameScore[i];
				m_lLastTurnScore[i]=GameEnd.lGameScore[i];
			}


			//变量定义
			SCORE lRevenue = 0;
			SCORE lUserScore[GAME_PLAYER]; ZeroMemory(lUserScore, sizeof(lUserScore));
			CopyMemory(lUserScore,GameEnd.lGameScore,sizeof(lUserScore));

			//统计积分
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//设置积分
				if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0&&lUserScore[i]>0)
				{
					lRevenue=lUserScore[i]*m_pGameServiceOption->wRevenueRatio/1000;
					GameEnd.lGameScore[i]=lUserScore[i]-lRevenue;
				}
				else
				{
					lRevenue=0;
					GameEnd.lGameScore[i]=lUserScore[i];
				}
				
				BYTE ScoreKind=(GameEnd.lGameScore[i]>=0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
				ScoreInfo[i].cbType = ScoreKind;
				ScoreInfo[i].lRevenue = lRevenue;
				ScoreInfo[i].lScore = GameEnd.lGameScore[i];
			}
			//写入积分
			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//结束游戏
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);

			m_pITableFrame->SetGameTimer(IDI_WAIT_START, TIME_WAIT_START, 1, 0);

			return true;
		}
	case GER_NETWORK_ERROR:		//用户断线
	case GER_USER_LEAVE:		//用户强退
		{
			return true;
			//定义变量
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//剩余扑克
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.cbCardCount[i]=m_bCardCount[i];
				CopyMemory(GameEnd.bCardData[i],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
			}

			//统计积分
			GameEnd.lGameScore[wChairID]=-32L*m_pGameServiceOption->lCellScore;
			GameEnd.lGameScore[(wChairID+1)%m_wPlayerCount]=16L*m_pGameServiceOption->lCellScore;
			GameEnd.lGameScore[(wChairID+2)%m_wPlayerCount]=16L*m_pGameServiceOption->lCellScore;

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//修改积分
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(i==wChairID)
				{
					ScoreInfo[i].lScore=GameEnd.lGameScore[wChairID];
					ScoreInfo[i].cbType=SCORE_TYPE_FLEE;
				}
				else
				{
					ScoreInfo[i].lScore=GameEnd.lGameScore[i];
					ScoreInfo[i].cbType=SCORE_TYPE_WIN;
				}
			}			
			
			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));
			//结束游戏
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);

			m_pITableFrame->SetGameTimer(IDI_WAIT_START, TIME_WAIT_START, 1, 0);
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
	case GAME_SCENE_FREE:		//空闲状态
		{
			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//设置变量
			StatusFree.lBaseScore=m_lCellScore;

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GAME_SCENE_PLAY:		//游戏状态
		{
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//设置变量
			StatusPlay.wLastOutUser=m_wTurnWiner;
			StatusPlay.wCurrentUser=m_wCurrentUser;

			CopyMemory(StatusPlay.bCardCount,m_bCardCount,sizeof(m_bCardCount));
			CopyMemory(StatusPlay.bTurnCardData,m_bTurnCardData,sizeof(BYTE)*m_bTurnCardCount);
			CopyMemory(StatusPlay.bCardData,m_bHandCardData[wChairID],sizeof(BYTE)*m_bCardCount[wChairID]);

			CopyMemory(StatusPlay.bAutoStatus, m_bAutoStatus, sizeof(StatusPlay.bAutoStatus));

			StatusPlay.wOutCardTime = 30;
			StatusPlay.wEndTime = 30;

			//发送场景
			bool bStatus = m_pITableFrame->SendGameScene(pIServerUserItem, &StatusPlay, sizeof(StatusPlay));

			if (m_bAutoStatus[wChairID])
			{
				m_bAutoStatus[wChairID] = false;
				CMD_S_UserAutomatism UserAutomatism;
				UserAutomatism.wChairID = wChairID;
				UserAutomatism.bTrusee = m_bAutoStatus[wChairID];

				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_AUTOMATISM, &UserAutomatism, sizeof(UserAutomatism));
				m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_AUTOMATISM, &UserAutomatism, sizeof(UserAutomatism));
			}

			return bStatus;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//获取百人游戏是否下注状态而且玩家下注了 返回true就有断线重连
bool CTableFrameSink::OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if (m_pITableFrame->GetGameStatus() == GAME_SCENE_PLAY)
		return true;
	else
		return false;
}

//定时器事件
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_GIVE_UP:
	{
		m_pITableFrame->KillGameTimer(IDI_GIVE_UP);
		if (m_wCurrentUser != INVALID_CHAIR)
			OnUserPassCard(m_wCurrentUser);
		return true;
	}
	case IDI_WAIT_START:
	{
		m_pITableFrame->KillGameTimer(IDI_WAIT_START);

		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GAME_FREE);
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_FREE);

		m_pITableFrame->SetGameTimer(IDI_AUTO_START, TIME_AUTO_START, 1, 0);

		for (WORD i = 0; i < GAME_PLAYER; i++)
		{
			if (m_bAutoStatus[i])
			{
				IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
				if (pServerUser)
					m_pITableFrame->PerformStandUpAction(pServerUser);
			}
		}

		return true;
	}
	case IDI_AUTO_START:
	{
		m_pITableFrame->KillGameTimer(IDI_AUTO_START);

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

		if (cbUserCount >= GAME_PLAYER)
			m_pITableFrame->StartGame();
		return true;
	}
	case IDI_AUTO_OUT:
	{
		m_pITableFrame->KillGameTimer(IDI_AUTO_OUT);

		if (m_wCurrentUser == INVALID_CHAIR)
			return true;

		if (m_bAutoStatus[m_wCurrentUser] == false)
		{
			m_bAutoStatus[m_wCurrentUser] = true;
			CMD_S_UserAutomatism UserAutomatism;
			UserAutomatism.wChairID = m_wCurrentUser;
			UserAutomatism.bTrusee = m_bAutoStatus[m_wCurrentUser];

			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_AUTOMATISM, &UserAutomatism, sizeof(UserAutomatism));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_AUTOMATISM, &UserAutomatism, sizeof(UserAutomatism));
		}

		if (m_bTurnCardCount == 0)
		{
			BYTE cbCardData[16] = { 0 };
			cbCardData[0] = m_bHandCardData[m_wCurrentUser][m_bCardCount[m_wCurrentUser] - 1];
			OnUserOutCard(m_wCurrentUser, cbCardData, 1);
		}
		else
		{
			tagSearchCardResult	tagTipResult;
			ZeroMemory(&tagTipResult, sizeof(tagTipResult));
			m_GameLogic.SearchOutCard(m_bHandCardData[m_wCurrentUser], m_bCardCount[m_wCurrentUser], m_bTurnCardData, m_bTurnCardCount, &tagTipResult);
			if (tagTipResult.cbSearchCount == 0)
				OnUserPassCard(m_wCurrentUser);
			else
				OnUserOutCard(m_wCurrentUser, tagTipResult.cbResultCard[0], tagTipResult.cbCardCount[0]);
		}
		return true;
	}
	default:
		break;
	}
	return false;
}

//游戏消息
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_OUT_CART:		//用户出牌
		{
			//变量定义
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pData;
			WORD wHeadSize=sizeof(CMD_C_OutCard)-sizeof(pOutCard->bCardData);

			//效验数据
			ASSERT(wDataSize>=wHeadSize);
			if (wDataSize<wHeadSize) return false;
			ASSERT(wDataSize==(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0])));
			if (wDataSize!=(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0]))) return false;

			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//消息处理
			return OnUserOutCard(pUserData->wChairID,pOutCard->bCardData,pOutCard->bCardCount);
		}
	case SUB_C_PASS_CARD:
		{
			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//消息处理
			return OnUserPassCard(pUserData->wChairID);
		}
	case SUB_C_AUTOMATISM:
		{
			//变量定义
			CMD_C_Automatism * pAutomatism = (CMD_C_Automatism *)pData;
			
			WORD wChairID = pIServerUserItem->GetChairID();

			m_bAutoStatus[wChairID] = pAutomatism->bAutomatism == TRUE;

			CMD_S_UserAutomatism UserAutomatism;
			UserAutomatism.wChairID = wChairID;
			UserAutomatism.bTrusee = m_bAutoStatus[wChairID];

			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_AUTOMATISM, &UserAutomatism, sizeof(UserAutomatism));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_AUTOMATISM, &UserAutomatism, sizeof(UserAutomatism));
		
			if (wChairID == m_wCurrentUser && m_bAutoStatus[wChairID])
				IsOffLineTrustee();
			return true;
		}
	}


	return false;
}

//框架消息
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//用户重入
bool CTableFrameSink::OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem) 
{
	return true; 
}

//用户出牌
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount)
{
	//效验状态
	if (m_pITableFrame->GetGameStatus() != GAME_SCENE_PLAY)
	{
		//构造数据
		CMD_S_OutCard OutCard;
		ZeroMemory(&OutCard, sizeof(OutCard));
		OutCard.bResult = 2;

		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_OUT_CARD, &OutCard, sizeof(OutCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_OUT_CARD, &OutCard, sizeof(OutCard));
		return true;
	}
	if (wChairID != m_wCurrentUser)
	{
		//构造数据
		CMD_S_OutCard OutCard;
		ZeroMemory(&OutCard, sizeof(OutCard));
		OutCard.bResult = 2;

		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_OUT_CARD, &OutCard, sizeof(OutCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_OUT_CARD, &OutCard, sizeof(OutCard));
		return true;
	}

	//类型判断
	BYTE bCardType=m_GameLogic.GetCardType(bCardData,bCardCount);
	if (bCardType == CT_ERROR)
	{
		//构造数据
		CMD_S_OutCard OutCard;
		ZeroMemory(&OutCard, sizeof(OutCard));
		OutCard.bResult = 2;

		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_OUT_CARD, &OutCard, sizeof(OutCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_OUT_CARD, &OutCard, sizeof(OutCard));
		return true;
	}

	//首出判断
	//if ((wChairID==m_wBankerUser)&&(m_bCardCount[wChairID]==16))
	//{
	//	BYTE i=0;
	//	for (i=0;i<bCardCount;i++)
	//	{
	//		if (bCardData[i]==0x33) break;
	//	}
	//	if (i == bCardCount)
	//	{
	//		//构造数据
	//		CMD_S_OutCard OutCard;
	//		ZeroMemory(&OutCard, sizeof(OutCard));
	//		OutCard.bResult = 2;

	//		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_OUT_CARD, &OutCard, sizeof(OutCard));
	//		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_OUT_CARD, &OutCard, sizeof(OutCard));
	//		return true;
	//	}
	//}

	//更随出牌
	if (m_bTurnCardCount==0) m_bTurnCardCount=bCardCount;
	else if (m_GameLogic.CompareCard(m_bTurnCardData, bCardData, m_bTurnCardCount, bCardCount) == false)
	{
		//构造数据
		CMD_S_OutCard OutCard;
		ZeroMemory(&OutCard, sizeof(OutCard));
		OutCard.bResult = 2;

		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_OUT_CARD, &OutCard, sizeof(OutCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_OUT_CARD, &OutCard, sizeof(OutCard));
		return true;
	}

	//删除扑克
	if (m_GameLogic.RemoveCard(bCardData, bCardCount, m_bHandCardData[wChairID], m_bCardCount[wChairID]) == false)
	{
		//构造数据
		CMD_S_OutCard OutCard;
		ZeroMemory(&OutCard, sizeof(OutCard));
		OutCard.bResult = 2;

		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_OUT_CARD, &OutCard, sizeof(OutCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_OUT_CARD, &OutCard, sizeof(OutCard));
		return true;
	}
	m_bCardCount[wChairID]-=bCardCount;

	//包赔判断
	if ((m_bCardCount[wChairID]!=0))
	{
		//获取用户
		WORD wNextPlayer=(wChairID+1)%m_wPlayerCount;

		//包赔判断
		if ((bCardCount==1)&&(m_bCardCount[wNextPlayer]==1))
		{
			m_bBaoPei=(m_GameLogic.CompareCard(m_bHandCardData[wChairID],bCardData,1,1)==true);
		}
		else 
		{
			//设置变量
			m_bBaoPei=false;
		}
	}

	//出牌记录
	m_bTurnCardType=bCardType;
	m_bTurnCardCount=bCardCount;

	ZeroMemory(&m_bTurnCardData, m_bTurnCardCount);
	CopyMemory(m_bTurnCardData,bCardData,sizeof(BYTE)*bCardCount);

	//切换用户
	m_wTurnWiner=wChairID;
	if (m_bCardCount[wChairID]!=0) m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
	else m_wCurrentUser=INVALID_CHAIR;

	//构造数据
	CMD_S_OutCard OutCard;
	ZeroMemory(&OutCard, sizeof(OutCard));
	OutCard.bCardCount=bCardCount;
	OutCard.wOutCardUser=wChairID;
	OutCard.wCurrentUser=m_wCurrentUser;
	ZeroMemory(&OutCard.bCardData, 16);
	CopyMemory(OutCard.bCardData,m_bTurnCardData,m_bTurnCardCount*sizeof(BYTE));
	OutCard.bResult = 1;
	

	//发送数据
	WORD wSendSize=sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE);


	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);

	//结束判断
	if (m_wCurrentUser==INVALID_CHAIR) OnEventGameConclude(wChairID,NULL,GER_NORMAL);
	else
	{
		tagSearchCardResult	tagTipResult;
		ZeroMemory(&tagTipResult, sizeof(tagTipResult));
		if (m_wCurrentUser != INVALID_CHAIR)
			m_GameLogic.SearchOutCard(m_bHandCardData[m_wCurrentUser], m_bCardCount[m_wCurrentUser], m_bTurnCardData, m_bTurnCardCount, &tagTipResult);
		if (tagTipResult.cbSearchCount == 0 && m_wCurrentUser != INVALID_CHAIR)
		{
			m_pITableFrame->SetGameTimer(IDI_GIVE_UP, TIME_GIVE_UP, 1, 0L);
		}
		else
			IsOffLineTrustee();
	}

	return true;
}

//用户放弃
bool CTableFrameSink::OnUserPassCard(WORD wChairID)
{
	//效验状态
	if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;
	if ((wChairID!=m_wCurrentUser)||(m_bTurnCardCount==0)) return true;

	//扑克分析
	tagSearchCardResult	tagTipResult;
	ZeroMemory(&tagTipResult, sizeof(tagTipResult));
	m_GameLogic.SearchOutCard(m_bHandCardData[wChairID], m_bCardCount[wChairID], m_bTurnCardData, m_bTurnCardCount, &tagTipResult);

	//放弃判断
	if (tagTipResult.cbSearchCount != 0) return true;

	//设置变量
	m_bBaoPei=false;
	m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
	if (m_wCurrentUser==m_wTurnWiner)
	{
		//炸弹判断
		if (m_GameLogic.GetCardType(m_bTurnCardData,m_bTurnCardCount)==CT_BOMB)
		{
			m_bBombCount[m_wTurnWiner]++;
			m_lGameScore[m_wTurnWiner]+=10*m_pGameServiceOption->lCellScore;
			m_lGameScore[(m_wTurnWiner+1)%m_wPlayerCount]+=-5*m_pGameServiceOption->lCellScore;
			m_lGameScore[(m_wTurnWiner+2)%m_wPlayerCount]+=-5*m_pGameServiceOption->lCellScore;
		}

		//设置变量
		m_bTurnCardCount=0;
	}

	//发送数据
	CMD_S_PassCard PassCard;
	PassCard.wPassUser=wChairID;
	PassCard.wCurrentUser=m_wCurrentUser;
	PassCard.bNewTurn=(m_bTurnCardCount==0)?true:false;


	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));

	if (m_bTurnCardCount != 0 && m_wCurrentUser != INVALID_CHAIR)
	{
		tagSearchCardResult	tagTipResult;
		ZeroMemory(&tagTipResult, sizeof(tagTipResult));
		m_GameLogic.SearchOutCard(m_bHandCardData[m_wCurrentUser], m_bCardCount[m_wCurrentUser], m_bTurnCardData, m_bTurnCardCount, &tagTipResult);
		if (tagTipResult.cbSearchCount == 0)
			m_pITableFrame->SetGameTimer(IDI_GIVE_UP, TIME_GIVE_UP, 1, 0L);
		else
			IsOffLineTrustee();
	}
	else if (m_bTurnCardCount == 0)
		IsOffLineTrustee();

	return true;
}

//用户托管
void CTableFrameSink::IsOffLineTrustee()
{
	if (m_wCurrentUser == INVALID_CHAIR)
		return;

	if (m_bAutoStatus[m_wCurrentUser] == false)
	{
		m_pITableFrame->SetGameTimer(IDI_AUTO_OUT, TIME_OVER_TIME, 1, 0L);
		return;
	}

	m_pITableFrame->SetGameTimer(IDI_AUTO_OUT,TIME_AUTO_OUT,1,0L);
}
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
