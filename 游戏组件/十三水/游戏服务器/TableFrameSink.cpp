#include "StdAfx.h"
#include "TableFrameSink.h"


//////////////////////////////////////////////////////////////////////////
#define IDI_WAIT_START				1					//�ȴ���ʼ
#define IDI_AUTO_START				2					//�Զ���ʼ
#define IDI_AUTO_OUT_CARD			3					//��ʱ����

#define TIME_AUTO_START				2000				//��ʼʱ��
//////////////////////////////////////////////////////////////////////////
//���캯��
CTableFrameSink::CTableFrameSink()
{
	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;
	m_pGameCustomRule=NULL;

	//��Ϸ����
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

	//��������
	m_wShowCardCount = 0;

	m_bWriteRoomStorage = true;

	//״̬����
	ZeroMemory(m_bFinishSegment , sizeof(m_bFinishSegment));

	srand(time(NULL));

	//�Զ������
	m_cbPlayerCount = 0;
	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{

}

//�ӿڲ�ѯ
VOID * CTableFrameSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ��
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//�Զ�����
	ASSERT(m_pITableFrame->GetCustomRule()!=NULL);
	m_pGameCustomRule=(tagCustomRule *)m_pITableFrame->GetCustomRule();

	m_lCellScore = m_pITableFrame->GetCellScore();
	m_cbPlayerCount = GAME_PLAYER;

	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_ALL_READY);

	return true;
}

//��λ����
VOID CTableFrameSink::RepositionSink()
{
	//��Ϸ����
	ZeroMemory(m_lScoreTimes, sizeof(m_lScoreTimes));
	ZeroMemory(m_bSpecialType, sizeof(m_bSpecialType));
	ZeroMemory(m_bUserCardData , sizeof(m_bUserCardData));
	ZeroMemory(m_bSegmentCard , sizeof(m_bSegmentCard));
	ZeroMemory(m_cbPlayerStatus, sizeof(m_cbPlayerStatus));
	ZeroMemory(m_lGameScore, sizeof(m_lGameScore));

	//��������
	m_wShowCardCount=0;

	m_bWriteRoomStorage = true;

	//״̬����
	ZeroMemory(m_bFinishSegment , sizeof(m_bFinishSegment));
	ZeroMemory(m_lGunScoreTimes, sizeof(m_lGunScoreTimes));
	ZeroMemory(m_lAddScoreTimes, sizeof(m_lAddScoreTimes));
	ZeroMemory(m_lSpecialScoreTimes, sizeof(m_lSpecialScoreTimes));
	ZeroMemory(m_lHomeRunsScoreTimes, sizeof(m_lHomeRunsScoreTimes));

	FillMemory(m_wFireList, sizeof(m_wFireList), INVALID_CHAIR);
	ZeroMemory(m_wFireCount, sizeof(m_wFireCount));

	return;
}

//��������
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	return 0L;
}

//���ٻ���
SCORE CTableFrameSink::QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return 0L;
}

//��Ϸ��ʼ
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

//��ʼ����
bool CTableFrameSink::OnGameAutoStart()
{
	//����״̬
	m_pITableFrame->SetGameStatus(GS_WK_PLAYING);


	tag_Thirteen_Result Thirteen_Result;
	ZeroMemory(&Thirteen_Result, sizeof(Thirteen_Result));
	m_pITableFrame->GetControlResult(NULL, 0, &Thirteen_Result);

	if (Thirteen_Result.cbTableCardArray[0][0] != 0)
		CopyMemory(m_bUserCardData, Thirteen_Result.cbTableCardArray, sizeof(m_bUserCardData));

	BYTE cbAiCount = 0;
	BYTE cbPlayCount = 0;

	//����Ĭ����ע(�׷�)
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

	//���˥��
	if (m_bWriteRoomStorage)
		m_pITableFrame->CalculateStorageDeduct();

	CMD_S_SendCard SendCard;
	ZeroMemory(&SendCard, sizeof(SendCard));

	SendCard.cbLeaveTime = m_pGameCustomRule->cbTimeRangeCard;
	CopyMemory(SendCard.cbPlayerStatus, m_cbPlayerStatus, sizeof(SendCard.cbPlayerStatus));

	//�����˿�
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

			//������Ϣ
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

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_DISMISS:			//��Ϸ��ɢ
		{
			m_pITableFrame->KillGameTimer(IDI_AUTO_OUT_CARD);

			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd, sizeof(GameEnd));

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			m_pITableFrame->SetGameTimer(IDI_WAIT_START, m_pGameCustomRule->cbTimeStartGame*1000, 1, NULL);
			m_dwOperateTime = (DWORD)time(NULL);

			return true;
		}
	case GER_NORMAL:			//�������
		{
			m_pITableFrame->KillGameTimer(IDI_AUTO_OUT_CARD);

			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				if (m_cbPlayerStatus[i] == Range_End)
					m_cbPlayerStatus[i] = Show_Card;
			}

			//��������÷�
			CompareCardGetTimes();

			//��������
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
								//��������
								if (m_bSpecialType[j] != CT_INVALID || m_bSpecialType[i] != CT_INVALID)
								{
									int nMulOnSD = m_GameLogic.nGetSpecialTimes(m_bSpecialType[j], m_bSpecialType[i]);
									m_lSpecialScoreTimes[i] -= nMulOnSD;
									m_lGameScore[i] -= nMulOnSD * m_lCellScore;
								}
								//��ͨ����
								else
								{
									//���ս���
									INT nMultiple = (m_lScoreTimes[j][0] + m_lScoreTimes[j][1] + m_lScoreTimes[j][2] + m_lGunScoreTimes[j] + m_lHomeRunsScoreTimes[j]);
									nMultiple += (m_lAddScoreTimes[j][0] + m_lAddScoreTimes[j][1] + m_lAddScoreTimes[j][2]);

									m_lGameScore[i] -= nMultiple * m_lCellScore;
								}
							}
						}
					}
				}
			}
			
			BYTE byLosePlayerCount = 0; //���˽�Ҳ�����
			BYTE byWinPlayerCount = 0; //Ӯ�˽�ҵ�
			WORD wPlayerIndexs[4];
			SCORE lWinScore = 0;
			SCORE lLoseScore = 0;
			ZeroMemory(wPlayerIndexs, sizeof(wPlayerIndexs));
			//��ֹ���ݿ⸺��
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
			
			//����˰��
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

					//���ֵ
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


			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			m_pITableFrame->SetGameTimer(IDI_WAIT_START,m_pGameCustomRule->cbTimeStartGame*1000,1,NULL);

			m_dwOperateTime = (DWORD)time(NULL);

			return true;
		}
	case GER_USER_LEAVE:		//�û�ǿ��
	case GER_NETWORK_ERROR:
		{
			return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//���ͳ���
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_WK_FREE:		//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree, sizeof(StatusFree));

			//���ñ���
			StatusFree.lCellScore = m_pITableFrame->GetCellScore();

			//�Զ������
			StatusFree.cbDelayTime = m_pGameCustomRule->cbTimeRangeCard;
			StatusFree.cbEndTime = m_pGameCustomRule->cbTimeShowCard;

			//���ͳ���
			bool bSuccess =  m_pITableFrame->SendGameScene( pIServerUserItem, &StatusFree, sizeof(StatusFree));

			return bSuccess;
		}
	case GS_WK_PLAYING:		//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay, sizeof(StatusPlay));

			//���ñ���
			StatusPlay.lCellScore = m_pITableFrame->GetCellScore();

			//�Զ������
			StatusPlay.cbDelayTime = m_pGameCustomRule->cbTimeRangeCard;
			StatusPlay.cbEndTime = m_pGameCustomRule->cbTimeShowCard;

			CopyMemory(StatusPlay.cbPlayerStatus, m_cbPlayerStatus, sizeof(StatusPlay.cbPlayerStatus));

			DWORD dwRestTime = (DWORD)time(NULL) - m_dwOperateTime;
			StatusPlay.cbLeaveTime = (DWORD)(m_pGameCustomRule->cbTimeRangeCard - (__min(m_pGameCustomRule->cbTimeRangeCard, dwRestTime)));

			CopyMemory(StatusPlay.cbCardData, m_bUserCardData[wChairID], sizeof(StatusPlay.cbCardData));

			//��δ�������
			if (!m_bFinishSegment[wChairID])
			{
				StatusPlay.cbSpecialType = m_GameLogic.GetSpecialType(m_bUserCardData[wChairID], HAND_CARD_COUNT);
				if (StatusPlay.cbSpecialType != CT_INVALID)
				{
					m_GameLogic.GetSpecialCardData(StatusPlay.cbCardData, HAND_CARD_COUNT, StatusPlay.cbSpecialCard);
				}
				m_GameLogic.AutoSixPutCard(StatusPlay.cbCardData, StatusPlay.cbBestCardList);
			}

			//���ͳ���
			bool bSuccess =  m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

			return bSuccess;
		}
	case GS_WK_END:		//����״̬
		{
			//��������
			CMD_S_StatusEnd StatusEnd;
			ZeroMemory(&StatusEnd, sizeof(StatusEnd));

			//���ñ���
			StatusEnd.lCellScore = m_pITableFrame->GetCellScore();

			//�Զ������
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

			//���ͳ���
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem, &StatusEnd, sizeof(StatusEnd));

			return bSuccess;
		}
	}

	return false;
}

//��ȡ������Ϸ�Ƿ���ע״̬���������ע��
bool CTableFrameSink::OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if (m_pITableFrame->GetGameStatus() == GS_WK_PLAYING || m_pITableFrame->GetGameStatus() == GS_WK_END)
	{
		if (m_cbPlayerStatus[wChairID] >= Range_Card)
			return true;
	}
	return false;
}

//ʱ���¼�
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	if (wTimerID == IDI_WAIT_START)
	{
		m_pITableFrame->KillGameTimer(wTimerID);

		//������Ϸ
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
				//�Զ�����
				BYTE bPutCard[13] = { 0 };
				BYTE cbTempCard[3][5] = {0};
				m_GameLogic.AutoPutCard(m_bUserCardData[i], bPutCard);

				CopyMemory(cbTempCard[0], &bPutCard[0],3);
				CopyMemory(cbTempCard[1], &bPutCard[3], 5);
				CopyMemory(cbTempCard[2], &bPutCard[8], 5);

				//��������
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

//�����¼�
bool CTableFrameSink::OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}
//�����¼�
bool CTableFrameSink::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	return true;
}

//��Ϸ��Ϣ����
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{

	switch (wSubCmdID)
	{
	case SUB_C_PLAYER_SHOWCARD:				//���̯��
		{
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (m_bFinishSegment[pUserData->wChairID]) return true;
			return OnUserShowCard(pUserData->wChairID , pData, wDataSize);
		}
	}

	return true;
}

//�����Ϣ
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//���̯��
bool CTableFrameSink::OnUserShowCard(const WORD wChairID , const void * pDataBuffer, WORD wDataSize)
{
	//������֤
	ASSERT(INVALID_CHAIR!=wChairID);
	if(INVALID_CHAIR==wChairID) return true;
	ASSERT(!m_bFinishSegment[wChairID]);
	if (m_bFinishSegment[wChairID]) return true;

	ASSERT(sizeof(CMD_C_ShowCard)==wDataSize);
	if(sizeof(CMD_C_ShowCard)!=wDataSize) return false;

	CMD_C_ShowCard *pSetSeg = (CMD_C_ShowCard*) pDataBuffer;

	//У���˿�
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

	//��֤�Ƿ�Ϊ��������
	if (pSetSeg->cbCardType[0] != CT_INVALID)
	{
		BYTE cbSpecial = m_GameLogic.GetSpecialType(m_bUserCardData[wChairID], HAND_CARD_COUNT);
		if (cbSpecial != pSetSeg->cbCardType[0])
			return true;
	}

	//��������
	CopyMemory(m_bSegmentCard[wChairID][0], &pSetSeg->cbCardData[0], 3);
	CopyMemory(m_bSegmentCard[wChairID][1], &pSetSeg->cbCardData[3], 5);
	CopyMemory(m_bSegmentCard[wChairID][2], &pSetSeg->cbCardData[8], 5);
	m_bFinishSegment[wChairID] = true;
	m_wShowCardCount++;

	m_cbPlayerStatus[wChairID] = Range_End;
	m_bSpecialType[wChairID] = pSetSeg->cbCardType[0];

	//֪ͨ����
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

	//��ʼ����
	StartCompareCard();

	return true;
}

//��ȡ����
void CTableFrameSink::CompareCardGetTimes()
{
	//������Ӯ
	for (WORD i = 0; i < m_cbPlayerCount; i++)
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if ((pIServerUserItem == NULL) || (m_cbPlayerStatus[i] == LookOn) || m_cbPlayerStatus[i] == NoPlayer) continue;
		//��ǹ����
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
				//��������
				BYTE bWindInfo[3] = { 0 };

				//���ʤ��
				bWindInfo[0] = m_GameLogic.CompareCard(m_bSegmentCard[i][0], m_bSegmentCard[j][0], 3, 3, true);
				bWindInfo[1] = m_GameLogic.CompareCard(m_bSegmentCard[i][1], m_bSegmentCard[j][1], 5, 5, true);
				bWindInfo[2] = m_GameLogic.CompareCard(m_bSegmentCard[i][2], m_bSegmentCard[j][2], 5, 5, true);

				//�ж���Ӯ
				for (BYTE bIdx = 0; bIdx < 3; bIdx++)
				{
					if (bWindInfo[bIdx] == enCREqual) continue;
					WORD wWinUser = (bWindInfo[bIdx] == enCRGreater) ? j : i;
					INT nMultiple = (bWindInfo[bIdx] == enCRGreater) ? 1 : -1;
					if (bWindInfo[bIdx] == enCRGreater) bLoseWinCount++;
					else bLoseWinCount--;

					//���ͷ���
					BYTE bFrontCT = m_GameLogic.GetCardType(m_bSegmentCard[wWinUser][0], 3);
					BYTE bMidCT = m_GameLogic.GetCardType(m_bSegmentCard[wWinUser][1], 5);
					BYTE bBackCT = m_GameLogic.GetCardType(m_bSegmentCard[wWinUser][2], 5);

					//��һע
					if (bIdx == 0)
					{
						//��һע����
						if (bFrontCT == CT_THREE)
						{
							m_lAddScoreTimes[i][0] -= nMultiple*3;
						}
					}

					//�ڶ�ע
					if (bIdx == 1)
					{
						//ͬ��˳
						if (bMidCT >= CT_FIVE_TONGHUASHUN)
						{
							m_lAddScoreTimes[i][1] -= nMultiple*10;
						}

						//��֧
						if (bMidCT == CT_FIVE_FOUR_ONE)
						{
							m_lAddScoreTimes[i][1] -= nMultiple*8;
						}

						//��«
						if (bMidCT == CT_FIVE_THREE_DEOUBLE)
						{
							m_lAddScoreTimes[i][1] -= nMultiple*2;
						}
					}

					//����ע
					if (bIdx == 2)
					{
						//ͬ��˳
						if (bBackCT >= CT_FIVE_TONGHUASHUN)
						{
							m_lAddScoreTimes[i][2] -= nMultiple*5;
						}

						//��֧
						if (bBackCT == CT_FIVE_FOUR_ONE)
						{
							m_lAddScoreTimes[i][2] -= nMultiple*4;
						}
					}
					m_lScoreTimes[i][bIdx] -= nMultiple;
				}

				//�ж��Ƿ��ǹ
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

		//ȫ�ݴ�,����������
		if (wWinCount == (GAME_PLAYER - 1))
		{
			//�����Լҷ�����
			m_lHomeRunsScoreTimes[i] = 18 * 3;

			//�����������ҷ����ķ�
			for (BYTE j = 0; j < m_cbPlayerCount; j++)
			{
				if (j != i)
					m_lHomeRunsScoreTimes[j] = -18;
			}
		}
	}
}


//�û�����
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if (m_cbPlayerStatus[wChairID] == NoPlayer)
		m_cbPlayerStatus[wChairID] = LookOn;
	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if (m_cbPlayerStatus[wChairID] == LookOn)
		m_cbPlayerStatus[wChairID] = NoPlayer;
	return true;
}

//��ʼ����
VOID CTableFrameSink::StartCompareCard()
{
	//��Ϸ����
	
		//��������
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

