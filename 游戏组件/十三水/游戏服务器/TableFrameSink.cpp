#include "StdAfx.h"
#include "TableFrameSink.h"


//////////////////////////////////////////////////////////////////////////
#define IDI_WAIT_START				1					//等待开始
#define IDI_AUTO_START				2					//自动开始
#define IDI_AUTO_OUT_CARD			3					//定时摆牌

#define TIME_AUTO_START				2000				//开始时间
//////////////////////////////////////////////////////////////////////////
//构造函数
CTableFrameSink::CTableFrameSink()
{
	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;
	m_pGameCustomRule=NULL;

	//游戏变量
	ZeroMemory(m_lScoreTimes, sizeof(m_lScoreTimes));
	ZeroMemory(m_lGunScoreTimes, sizeof(m_lGunScoreTimes));
	ZeroMemory(m_lGameScore, sizeof(m_lGameScore));

	ZeroMemory(m_lAddScoreTimes, sizeof(m_lAddScoreTimes));
	ZeroMemory(m_lSpecialScoreTimes, sizeof(m_lSpecialScoreTimes));
	ZeroMemory(m_lHomeRunsScoreTimes, sizeof(m_lHomeRunsScoreTimes));

	ZeroMemory(m_bSpecialType, sizeof(m_bSpecialType));
	ZeroMemory(m_bUserCardData , sizeof(m_bUserCardData));
	ZeroMemory(m_bSegmentCard , sizeof(m_bSegmentCard));

	FillMemory(m_wFireList, sizeof(m_wFireList),INVALID_CHAIR);
	ZeroMemory(m_wFireCount, sizeof(m_wFireCount));
	ZeroMemory(m_cbPlayerStatus, sizeof(m_cbPlayerStatus));

	ZeroMemory(m_bAutoUser, sizeof(m_bAutoUser));

	//计数变量
	m_wShowCardCount = 0;

	m_bWriteRoomStorage = true;

	//状态变量
	ZeroMemory(m_bFinishSegment , sizeof(m_bFinishSegment));

	srand(time(NULL));

	//自定义规则
	m_cbPlayerCount = 0;
	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
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

//初始化
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//自定规则
	ASSERT(m_pITableFrame->GetCustomRule()!=NULL);
	m_pGameCustomRule=(tagCustomRule *)m_pITableFrame->GetCustomRule();

	m_lCellScore = m_pITableFrame->GetCellScore();
	m_cbPlayerCount = GAME_PLAYER;

	//开始模式
	m_pITableFrame->SetStartMode(START_MODE_ALL_READY);

	return true;
}

//复位桌子
VOID CTableFrameSink::RepositionSink()
{
	//游戏变量
	ZeroMemory(m_lScoreTimes, sizeof(m_lScoreTimes));
	ZeroMemory(m_bSpecialType, sizeof(m_bSpecialType));
	ZeroMemory(m_bUserCardData , sizeof(m_bUserCardData));
	ZeroMemory(m_bSegmentCard , sizeof(m_bSegmentCard));
	ZeroMemory(m_cbPlayerStatus, sizeof(m_cbPlayerStatus));
	ZeroMemory(m_lGameScore, sizeof(m_lGameScore));

	//计数变量
	m_wShowCardCount=0;

	m_bWriteRoomStorage = true;

	//状态变量
	ZeroMemory(m_bFinishSegment , sizeof(m_bFinishSegment));
	ZeroMemory(m_lGunScoreTimes, sizeof(m_lGunScoreTimes));
	ZeroMemory(m_lAddScoreTimes, sizeof(m_lAddScoreTimes));
	ZeroMemory(m_lSpecialScoreTimes, sizeof(m_lSpecialScoreTimes));
	ZeroMemory(m_lHomeRunsScoreTimes, sizeof(m_lHomeRunsScoreTimes));

	FillMemory(m_wFireList, sizeof(m_wFireList), INVALID_CHAIR);
	ZeroMemory(m_wFireCount, sizeof(m_wFireCount));

	return;
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
	ZeroMemory(m_bAutoUser, sizeof(m_bAutoUser));

	m_pITableFrame->KillGameTimer(IDI_WAIT_START);

	m_pITableFrame->SetGameTimer(IDI_AUTO_START, TIME_AUTO_START, 1, 0);

	m_dwOperateTime = (DWORD)time(NULL);

	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START);
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_START);

	return true;
}

//开始发牌
bool CTableFrameSink::OnGameAutoStart()
{
	//设置状态
	m_pITableFrame->SetGameStatus(GS_WK_PLAYING);


	tag_Thirteen_Result Thirteen_Result;
	ZeroMemory(&Thirteen_Result, sizeof(Thirteen_Result));
	m_pITableFrame->GetControlResult(NULL, 0, &Thirteen_Result);

	if (Thirteen_Result.cbTableCardArray[0][0] != 0)
		CopyMemory(m_bUserCardData, Thirteen_Result.cbTableCardArray, sizeof(m_bUserCardData));

	BYTE cbAiCount = 0;
	BYTE cbPlayCount = 0;

	//设置默认下注(底分)
	for (int i = 0; i < m_cbPlayerCount; i++)
	{
		IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if (pServerUserItem == NULL) continue;

		m_GameLogic.SortCardList(m_bUserCardData[i], HAND_CARD_COUNT);
		m_cbPlayerStatus[i] = Range_Card;

		cbPlayCount++;
		if (pServerUserItem->IsAndroidUser())
		{
			cbAiCount++;
		}
	}

	if (cbAiCount == 0 || cbAiCount == cbPlayCount)
		m_bWriteRoomStorage = false;

	//库存衰减
	if (m_bWriteRoomStorage)
		m_pITableFrame->CalculateStorageDeduct();

	CMD_S_SendCard SendCard;
	ZeroMemory(&SendCard, sizeof(SendCard));

	SendCard.cbLeaveTime = m_pGameCustomRule->cbTimeRangeCard;
	CopyMemory(SendCard.cbPlayerStatus, m_cbPlayerStatus, sizeof(SendCard.cbPlayerStatus));

	//发送扑克
	for (WORD i = 0; i < m_cbPlayerCount; i++)
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if ((pIServerUserItem != NULL))
		{
			CopyMemory(SendCard.cbCard, m_bUserCardData[i], sizeof(SendCard.cbCard));

			SendCard.cbSpecialType = m_GameLogic.GetSpecialType(SendCard.cbCard, HAND_CARD_COUNT);
			if (SendCard.cbSpecialType != CT_INVALID)
			{
				m_GameLogic.GetSpecialCardData(SendCard.cbCard, HAND_CARD_COUNT, SendCard.cbSpecialCard);
			}

			m_GameLogic.AutoSixPutCard(SendCard.cbCard, SendCard.cbBestCardList);

			//发送消息
			m_pITableFrame->SendTableData(i, SUB_S_GAME_SENDCARD, &SendCard, sizeof(SendCard));
			m_pITableFrame->SendLookonData(i, SUB_S_GAME_SENDCARD, &SendCard, sizeof(SendCard));
		}

		ZeroMemory(SendCard.cbCard, sizeof(SendCard.cbCard));
		SendCard.cbSpecialType = 0;
		ZeroMemory(SendCard.cbSpecialCard, sizeof(SendCard.cbSpecialCard));
		ZeroMemory(SendCard.cbBestCardList, sizeof(SendCard.cbBestCardList));
	}

	m_pITableFrame->SetGameTimer(IDI_AUTO_OUT_CARD, m_pGameCustomRule->cbTimeRangeCard * 1000, 1, 0);

	m_dwOperateTime = (DWORD)time(NULL);

	return true;
}

//游戏结束
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_DISMISS:			//游戏解散
		{
			m_pITableFrame->KillGameTimer(IDI_AUTO_OUT_CARD);

			//构造数据
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd, sizeof(GameEnd));

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			m_pITableFrame->SetGameTimer(IDI_WAIT_START, m_pGameCustomRule->cbTimeStartGame*1000, 1, NULL);
			m_dwOperateTime = (DWORD)time(NULL);

			return true;
		}
	case GER_NORMAL:			//常规结束
		{
			m_pITableFrame->KillGameTimer(IDI_AUTO_OUT_CARD);

			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				if (m_cbPlayerStatus[i] == Range_End)
					m_cbPlayerStatus[i] = Show_Card;
			}

			//计算基础得分
			CompareCardGetTimes();

			//构造数据
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd, sizeof(GameEnd));

			for(WORD i=0; i<m_cbPlayerCount; ++i)
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
				if (m_cbPlayerStatus[i] == LookOn || m_cbPlayerStatus[i] == NoPlayer) continue;

				if (pIServerUserItem)
				{
					for (WORD j = 0; j < m_cbPlayerCount; j++)
					{
						if (i != j)
						{
							IServerUserItem *pOtherIServerUserItem = m_pITableFrame->GetTableUserItem(j);
							if (m_cbPlayerStatus[j] == LookOn || m_cbPlayerStatus[i] == NoPlayer) continue;

							if (pOtherIServerUserItem)
							{
								//特殊牌型
								if (m_bSpecialType[j] != CT_INVALID || m_bSpecialType[i] != CT_INVALID)
								{
									int nMulOnSD = m_GameLogic.nGetSpecialTimes(m_bSpecialType[j], m_bSpecialType[i]);
									m_lSpecialScoreTimes[i] -= nMulOnSD;
									m_lGameScore[i] -= nMulOnSD * m_lCellScore;
								}
								//普通牌型
								else
								{
									//最终结算
									INT nMultiple = (m_lScoreTimes[j][0] + m_lScoreTimes[j][1] + m_lScoreTimes[j][2] + m_lGunScoreTimes[j] + m_lHomeRunsScoreTimes[j]);
									nMultiple += (m_lAddScoreTimes[j][0] + m_lAddScoreTimes[j][1] + m_lAddScoreTimes[j][2]);

									m_lGameScore[i] -= nMultiple * m_lCellScore;
								}
							}
						}
					}
				}
			}
			
			BYTE byLosePlayerCount = 0; //输了金币不够的
			BYTE byWinPlayerCount = 0; //赢了金币的
			WORD wPlayerIndexs[4];
			SCORE lWinScore = 0;
			SCORE lLoseScore = 0;
			ZeroMemory(wPlayerIndexs, sizeof(wPlayerIndexs));
			//防止数据库负分
			for (WORD i = 0; i<m_cbPlayerCount; i++)
			{
				if (m_cbPlayerStatus[i] == LookOn || m_cbPlayerStatus[i] == NoPlayer) continue;
				if (m_lGameScore[i] < 0)
				{
					SCORE lUserScore = m_pITableFrame->GetTableUserItem(i)->GetUserScore();
					if (lUserScore < -(m_lGameScore[i]))
					{
						lLoseScore += (lUserScore + m_lGameScore[i]);

						m_lGameScore[i] = -lUserScore;
						byLosePlayerCount++;
					}
				}
				else if (m_lGameScore[i] > 0)
				{
					wPlayerIndexs[byWinPlayerCount] = i;
					byWinPlayerCount++;
					lWinScore += m_lGameScore[i];
				}
			}

			if (byLosePlayerCount > 0)
			{
				SCORE lTotalScore = 0;
				for (WORD i = 0; i < byWinPlayerCount; i++)
				{
					WORD index = wPlayerIndexs[i];
					if ((i + 1) == byWinPlayerCount)
					{
						m_lGameScore[index] += (lLoseScore - lTotalScore);
					}
					else
					{
						SCORE lTempScore = (lLoseScore * m_lGameScore[index] / lWinScore);
						lTotalScore += lTempScore;
						m_lGameScore[index] += lTempScore;
					}
				}
			}
			
			//计算税收
			LONGLONG lRobotGameScore = 0;
			
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));

			for (WORD i=0;i<m_cbPlayerCount;i++)
			{

				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
				if (m_cbPlayerStatus[i] == LookOn || m_cbPlayerStatus[i] == NoPlayer) continue;

				if (pIServerUserItem)
				{
					LONGLONG lTmpTax = 0;
					if (m_lGameScore[i] > 0)
						lTmpTax = m_pITableFrame->CalculateRevenue(i, m_lGameScore[i]);
	
					m_lGameScore[i] -= lTmpTax;

					ScoreInfoArray[i].lScore = m_lGameScore[i];
					ScoreInfoArray[i].cbType = (m_lGameScore[i]>0L) ? SCORE_TYPE_WIN : SCORE_TYPE_LOSE;
					ScoreInfoArray[i].lRevenue = lTmpTax;

					//库存值
					if (pIServerUserItem->IsAndroidUser() && m_bWriteRoomStorage)
						m_pGameServiceOption->pRoomStorageOption.lStorageCurrent += ScoreInfoArray[i].lScore;
				}
			}

			m_pITableFrame->WriteTableScore(ScoreInfoArray, m_cbPlayerCount);

			CopyMemory(GameEnd.lGameScore, m_lGameScore,sizeof(GameEnd.lGameScore));

			CopyMemory(GameEnd.wFireList, m_wFireList, sizeof(GameEnd.wFireList));

			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				if (m_cbPlayerStatus[i] != LookOn && m_cbPlayerStatus[i] != NoPlayer)
				{
					CopyMemory(&GameEnd.cbEndCardList[i][0], m_bSegmentCard[i][0],3);
					CopyMemory(&GameEnd.cbEndCardList[i][3], m_bSegmentCard[i][1], 5);
					CopyMemory(&GameEnd.cbEndCardList[i][8], m_bSegmentCard[i][2], 5);

					GameEnd.cbEndCardType[i][0] = m_bSpecialType[i];
					GameEnd.cbEndCardType[i][1] = m_GameLogic.GetCardType(m_bSegmentCard[i][0], 3);
					GameEnd.cbEndCardType[i][2] = m_GameLogic.GetCardType(m_bSegmentCard[i][1], 5);
					GameEnd.cbEndCardType[i][3] = m_GameLogic.GetCardType(m_bSegmentCard[i][2], 5);

					GameEnd.lPerScore[i][0] = m_lSpecialScoreTimes[i];
					GameEnd.lPerScore[i][1] = m_lScoreTimes[i][0];
					GameEnd.lPerScore[i][2] = m_lScoreTimes[i][1];
					GameEnd.lPerScore[i][3] = m_lScoreTimes[i][2];
					GameEnd.lPerScore[i][4] = m_lAddScoreTimes[i][0];
					GameEnd.lPerScore[i][5] = m_lAddScoreTimes[i][1];
					GameEnd.lPerScore[i][6] = m_lAddScoreTimes[i][2];
					GameEnd.lPerScore[i][7] = m_lGunScoreTimes[i];
					GameEnd.lPerScore[i][8] = m_lHomeRunsScoreTimes[i];
				}
			}


			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			m_pITableFrame->SetGameTimer(IDI_WAIT_START,m_pGameCustomRule->cbTimeStartGame*1000,1,NULL);

			m_dwOperateTime = (DWORD)time(NULL);

			return true;
		}
	case GER_USER_LEAVE:		//用户强退
	case GER_NETWORK_ERROR:
		{
			return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//发送场景
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_WK_FREE:		//空闲状态
		{
			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree, sizeof(StatusFree));

			//设置变量
			StatusFree.lCellScore = m_pITableFrame->GetCellScore();

			//自定义规则
			StatusFree.cbDelayTime = m_pGameCustomRule->cbTimeRangeCard;
			StatusFree.cbEndTime = m_pGameCustomRule->cbTimeShowCard;

			//发送场景
			bool bSuccess =  m_pITableFrame->SendGameScene( pIServerUserItem, &StatusFree, sizeof(StatusFree));

			return bSuccess;
		}
	case GS_WK_PLAYING:		//游戏状态
		{
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay, sizeof(StatusPlay));

			//设置变量
			StatusPlay.lCellScore = m_pITableFrame->GetCellScore();

			//自定义规则
			StatusPlay.cbDelayTime = m_pGameCustomRule->cbTimeRangeCard;
			StatusPlay.cbEndTime = m_pGameCustomRule->cbTimeShowCard;

			CopyMemory(StatusPlay.cbPlayerStatus, m_cbPlayerStatus, sizeof(StatusPlay.cbPlayerStatus));

			DWORD dwRestTime = (DWORD)time(NULL) - m_dwOperateTime;
			StatusPlay.cbLeaveTime = (DWORD)(m_pGameCustomRule->cbTimeRangeCard - (__min(m_pGameCustomRule->cbTimeRangeCard, dwRestTime)));

			CopyMemory(StatusPlay.cbCardData, m_bUserCardData[wChairID], sizeof(StatusPlay.cbCardData));

			//还未完成理牌
			if (!m_bFinishSegment[wChairID])
			{
				StatusPlay.cbSpecialType = m_GameLogic.GetSpecialType(m_bUserCardData[wChairID], HAND_CARD_COUNT);
				if (StatusPlay.cbSpecialType != CT_INVALID)
				{
					m_GameLogic.GetSpecialCardData(StatusPlay.cbCardData, HAND_CARD_COUNT, StatusPlay.cbSpecialCard);
				}
				m_GameLogic.AutoSixPutCard(StatusPlay.cbCardData, StatusPlay.cbBestCardList);
			}

			//发送场景
			bool bSuccess =  m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

			return bSuccess;
		}
	case GS_WK_END:		//结束状态
		{
			//构造数据
			CMD_S_StatusEnd StatusEnd;
			ZeroMemory(&StatusEnd, sizeof(StatusEnd));

			//设置变量
			StatusEnd.lCellScore = m_pITableFrame->GetCellScore();

			//自定义规则
			StatusEnd.cbDelayTime = m_pGameCustomRule->cbTimeRangeCard;
			StatusEnd.cbEndTime = m_pGameCustomRule->cbTimeShowCard;

			CopyMemory(StatusEnd.cbPlayerStatus, m_cbPlayerStatus, sizeof(StatusEnd.cbPlayerStatus));

			DWORD dwRestTime = (DWORD)time(NULL) - m_dwOperateTime;
			StatusEnd.cbLeaveTime = (DWORD)(m_pGameCustomRule->cbTimeStartGame - (__min(m_pGameCustomRule->cbTimeStartGame, dwRestTime)));

			CopyMemory(StatusEnd.lGameScore, m_lGameScore, sizeof(StatusEnd.lGameScore));

			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				if (m_cbPlayerStatus[i] != LookOn && m_cbPlayerStatus[i] != NoPlayer)
				{
					CopyMemory(&StatusEnd.cbEndCardList[i][0], m_bSegmentCard[i][0], 3);
					CopyMemory(&StatusEnd.cbEndCardList[i][3], m_bSegmentCard[i][1], 5);
					CopyMemory(&StatusEnd.cbEndCardList[i][8], m_bSegmentCard[i][2], 5);

					StatusEnd.cbEndCardType[i][0] = m_bSpecialType[i];
					StatusEnd.cbEndCardType[i][1] = m_GameLogic.GetCardType(m_bSegmentCard[i][0], 3);
					StatusEnd.cbEndCardType[i][2] = m_GameLogic.GetCardType(m_bSegmentCard[i][1], 5);
					StatusEnd.cbEndCardType[i][3] = m_GameLogic.GetCardType(m_bSegmentCard[i][2], 5);

					StatusEnd.lPerScore[i][0] = m_lSpecialScoreTimes[i];
					StatusEnd.lPerScore[i][1] = m_lScoreTimes[i][0];
					StatusEnd.lPerScore[i][2] = m_lScoreTimes[i][1];
					StatusEnd.lPerScore[i][3] = m_lScoreTimes[i][2];
					StatusEnd.lPerScore[i][4] = m_lAddScoreTimes[i][0];
					StatusEnd.lPerScore[i][5] = m_lAddScoreTimes[i][1];
					StatusEnd.lPerScore[i][6] = m_lAddScoreTimes[i][2];
					StatusEnd.lPerScore[i][7] = m_lGunScoreTimes[i];
					StatusEnd.lPerScore[i][8] = m_lHomeRunsScoreTimes[i];
				}
			}

			//发送场景
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem, &StatusEnd, sizeof(StatusEnd));

			return bSuccess;
		}
	}

	return false;
}

//获取百人游戏是否下注状态而且玩家下注了
bool CTableFrameSink::OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if (m_pITableFrame->GetGameStatus() == GS_WK_PLAYING || m_pITableFrame->GetGameStatus() == GS_WK_END)
	{
		if (m_cbPlayerStatus[wChairID] >= Range_Card)
			return true;
	}
	return false;
}

//时间事件
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	if (wTimerID == IDI_WAIT_START)
	{
		m_pITableFrame->KillGameTimer(wTimerID);

		//结束游戏
		m_pITableFrame->ConcludeGame(GS_WK_FREE);

		for (WORD i = 0; i < GAME_PLAYER; i++)
		{
			if (m_bAutoUser[i])
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
	else if (wTimerID == IDI_AUTO_START)
	{
		m_pITableFrame->KillGameTimer(wTimerID);

		BYTE cbUserCount = 0;
		for (WORD i = 0; i < GAME_PLAYER; i++)
		{
			IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
			if (pServerUser)
			{
				cbUserCount++;
			}
		}

		if (cbUserCount >= 2)
			OnGameAutoStart();

		return true;
	}
	else if (wTimerID == IDI_AUTO_OUT_CARD)
	{
		m_pITableFrame->KillGameTimer(wTimerID);
		if (m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
		{
			for (BYTE i = 0; i < m_cbPlayerCount; i++)
			{
				if (m_bFinishSegment[i] || m_cbPlayerStatus[i] == LookOn || m_cbPlayerStatus[i] == NoPlayer) continue;
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
				if ((pIServerUserItem == NULL)) continue;
				//自动摆牌
				BYTE bPutCard[13] = { 0 };
				BYTE cbTempCard[3][5] = {0};
				m_GameLogic.AutoPutCard(m_bUserCardData[i], bPutCard);

				CopyMemory(cbTempCard[0], &bPutCard[0],3);
				CopyMemory(cbTempCard[1], &bPutCard[3], 5);
				CopyMemory(cbTempCard[2], &bPutCard[8], 5);

				//构造数据
				CMD_C_ShowCard pSetSeg;
				ZeroMemory(&pSetSeg, sizeof(pSetSeg));
				CopyMemory(pSetSeg.cbCardData, bPutCard, 13);

				pSetSeg.cbCardType[0] = m_GameLogic.GetSpecialType(pSetSeg.cbCardData,HAND_CARD_COUNT);
	
				pSetSeg.cbCardType[1] = m_GameLogic.GetCardType(cbTempCard[0], 3);
				pSetSeg.cbCardType[2] = m_GameLogic.GetCardType(cbTempCard[1], 5);
				pSetSeg.cbCardType[3] = m_GameLogic.GetCardType(cbTempCard[2], 5);

				OnUserShowCard(i, &pSetSeg, sizeof(pSetSeg));

				m_bAutoUser[i] = true;
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
	return true;
}

//游戏消息处理
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{

	switch (wSubCmdID)
	{
	case SUB_C_PLAYER_SHOWCARD:				//玩家摊牌
		{
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (m_bFinishSegment[pUserData->wChairID]) return true;
			return OnUserShowCard(pUserData->wChairID , pData, wDataSize);
		}
	}

	return true;
}

//框架消息
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//玩家摊牌
bool CTableFrameSink::OnUserShowCard(const WORD wChairID , const void * pDataBuffer, WORD wDataSize)
{
	//数据验证
	ASSERT(INVALID_CHAIR!=wChairID);
	if(INVALID_CHAIR==wChairID) return true;
	ASSERT(!m_bFinishSegment[wChairID]);
	if (m_bFinishSegment[wChairID]) return true;

	ASSERT(sizeof(CMD_C_ShowCard)==wDataSize);
	if(sizeof(CMD_C_ShowCard)!=wDataSize) return false;

	CMD_C_ShowCard *pSetSeg = (CMD_C_ShowCard*) pDataBuffer;

	//校验扑克
	BYTE cbCardDataTemp[HAND_CARD_COUNT];
	ZeroMemory(cbCardDataTemp, sizeof(cbCardDataTemp));
	CopyMemory(cbCardDataTemp, pSetSeg->cbCardData, sizeof(cbCardDataTemp));

	for(BYTE i=0; i<HAND_CARD_COUNT; i++)
	{
		BYTE j=0;
		for(j=0; j<HAND_CARD_COUNT; j++)
		{
			if(cbCardDataTemp[i] == m_bUserCardData[wChairID][j]) break;
		}
		if(HAND_CARD_COUNT == j) 
			return true;
	}

	//验证是否为特殊牌型
	if (pSetSeg->cbCardType[0] != CT_INVALID)
	{
		BYTE cbSpecial = m_GameLogic.GetSpecialType(m_bUserCardData[wChairID], HAND_CARD_COUNT);
		if (cbSpecial != pSetSeg->cbCardType[0])
			return true;
	}

	//设置数据
	CopyMemory(m_bSegmentCard[wChairID][0], &pSetSeg->cbCardData[0], 3);
	CopyMemory(m_bSegmentCard[wChairID][1], &pSetSeg->cbCardData[3], 5);
	CopyMemory(m_bSegmentCard[wChairID][2], &pSetSeg->cbCardData[8], 5);
	m_bFinishSegment[wChairID] = true;
	m_wShowCardCount++;

	m_cbPlayerStatus[wChairID] = Range_End;
	m_bSpecialType[wChairID] = pSetSeg->cbCardType[0];

	//通知开牌
	CMD_S_OutCard  OutCard;
	ZeroMemory(&OutCard, sizeof(OutCard));

	OutCard.wChairID = wChairID;
	OutCard.cbSpecialType = pSetSeg->cbCardType[0];

	for (WORD i = 0; i < m_cbPlayerCount; i++)
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if ((pIServerUserItem == NULL) || (m_cbPlayerStatus[i] == NoPlayer))	continue;

		m_pITableFrame->SendTableData(i, SUB_S_GAME_OUTCARD, &OutCard, sizeof(OutCard));
		m_pITableFrame->SendLookonData(i, SUB_S_GAME_OUTCARD, &OutCard, sizeof(OutCard));
	}

	//开始比牌
	StartCompareCard();

	return true;
}

//获取倍数
void CTableFrameSink::CompareCardGetTimes()
{
	//计算输赢
	for (WORD i = 0; i < m_cbPlayerCount; i++)
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if ((pIServerUserItem == NULL) || (m_cbPlayerStatus[i] == LookOn) || m_cbPlayerStatus[i] == NoPlayer) continue;
		//打枪次数
		WORD wWinCount = 0;

		if (m_bSpecialType[i] > 0)
			continue;

		for (int j = 0; j < m_cbPlayerCount; j++)
		{

			if (j != i && m_bSpecialType[j] == CT_INVALID)
			{
				IServerUserItem *pOtherIServerUserItem = m_pITableFrame->GetTableUserItem(j);
				if ((pOtherIServerUserItem == NULL) || (m_cbPlayerStatus[j] == LookOn) || m_cbPlayerStatus[i] == NoPlayer) continue;
				if (m_bSpecialType[j] != 0 || m_bSpecialType[i] != 0)
				{
					continue;
				}

				int bLoseWinCount = 0;
				//变量定义
				BYTE bWindInfo[3] = { 0 };

				//检查胜负
				bWindInfo[0] = m_GameLogic.CompareCard(m_bSegmentCard[i][0], m_bSegmentCard[j][0], 3, 3, true);
				bWindInfo[1] = m_GameLogic.CompareCard(m_bSegmentCard[i][1], m_bSegmentCard[j][1], 5, 5, true);
				bWindInfo[2] = m_GameLogic.CompareCard(m_bSegmentCard[i][2], m_bSegmentCard[j][2], 5, 5, true);

				//判断输赢
				for (BYTE bIdx = 0; bIdx < 3; bIdx++)
				{
					if (bWindInfo[bIdx] == enCREqual) continue;
					WORD wWinUser = (bWindInfo[bIdx] == enCRGreater) ? j : i;
					INT nMultiple = (bWindInfo[bIdx] == enCRGreater) ? 1 : -1;
					if (bWindInfo[bIdx] == enCRGreater) bLoseWinCount++;
					else bLoseWinCount--;

					//牌型翻倍
					BYTE bFrontCT = m_GameLogic.GetCardType(m_bSegmentCard[wWinUser][0], 3);
					BYTE bMidCT = m_GameLogic.GetCardType(m_bSegmentCard[wWinUser][1], 5);
					BYTE bBackCT = m_GameLogic.GetCardType(m_bSegmentCard[wWinUser][2], 5);

					//第一注
					if (bIdx == 0)
					{
						//第一注三条
						if (bFrontCT == CT_THREE)
						{
							m_lAddScoreTimes[i][0] -= nMultiple*3;
						}
					}

					//第二注
					if (bIdx == 1)
					{
						//同花顺
						if (bMidCT >= CT_FIVE_TONGHUASHUN)
						{
							m_lAddScoreTimes[i][1] -= nMultiple*10;
						}

						//铁支
						if (bMidCT == CT_FIVE_FOUR_ONE)
						{
							m_lAddScoreTimes[i][1] -= nMultiple*8;
						}

						//葫芦
						if (bMidCT == CT_FIVE_THREE_DEOUBLE)
						{
							m_lAddScoreTimes[i][1] -= nMultiple*2;
						}
					}

					//第三注
					if (bIdx == 2)
					{
						//同花顺
						if (bBackCT >= CT_FIVE_TONGHUASHUN)
						{
							m_lAddScoreTimes[i][2] -= nMultiple*5;
						}

						//铁支
						if (bBackCT == CT_FIVE_FOUR_ONE)
						{
							m_lAddScoreTimes[i][2] -= nMultiple*4;
						}
					}
					m_lScoreTimes[i][bIdx] -= nMultiple;
				}

				//判断是否打枪
				if (bLoseWinCount == 3)
				{
					m_lGunScoreTimes[i] -= 3;
				}
				else if (bLoseWinCount == -3)
				{
					wWinCount++;
					m_lGunScoreTimes[i] += 3;

					m_wFireList[i][m_wFireCount[i]++] = j;
				}
			}
		}

		//全垒打,分数翻倍。
		if (wWinCount == (GAME_PLAYER - 1))
		{
			//计算自家翻倍分
			m_lHomeRunsScoreTimes[i] = 18 * 3;

			//计算其他三家翻倍的分
			for (BYTE j = 0; j < m_cbPlayerCount; j++)
			{
				if (j != i)
					m_lHomeRunsScoreTimes[j] = -18;
			}
		}
	}
}


//用户坐下
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if (m_cbPlayerStatus[wChairID] == NoPlayer)
		m_cbPlayerStatus[wChairID] = LookOn;
	return true;
}

//用户起来
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if (m_cbPlayerStatus[wChairID] == LookOn)
		m_cbPlayerStatus[wChairID] = NoPlayer;
	return true;
}

//开始比牌
VOID CTableFrameSink::StartCompareCard()
{
	//游戏人数
	
		//计算人数
	WORD dwPlayerCount = 0;
	for (WORD i = 0; i < m_cbPlayerCount; i++)
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if (m_cbPlayerStatus[i] == LookOn || m_cbPlayerStatus[i] == NoPlayer)
			continue;
		if (pIServerUserItem) 
			dwPlayerCount++;
	}
	
	if (dwPlayerCount == m_wShowCardCount)
	{
		m_pITableFrame->SetGameStatus(GS_WK_END);
		OnEventGameConclude(INVALID_CHAIR, NULL, GER_NORMAL);
	}

	return;
}

bool CTableFrameSink::OnActionUserOnReady( WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize )
{
	return true;
}


//////////////////////////////////////////////////////////////////////////

