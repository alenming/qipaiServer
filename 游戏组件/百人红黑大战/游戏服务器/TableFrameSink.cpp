#include "StdAfx.h"
#include "DlgCustomRule.h"
#include "TableFrameSink.h"
#include <locale>
//////////////////////////////////////////////////////////////////////////

//��������
#define SEND_COUNT					300									//���ʹ���

//��������
#define INDEX_PLAYER				0									//�������
#define INDEX_BANKER				1									//�ڼ�����

//��עʱ��
#define IDI_FREE					1									//����ʱ��
#define TIME_FREE					1									//����ʱ��

//��עʱ��
#define IDI_PLACE_JETTON			2									//��עʱ��
#define TIME_PLACE_JETTON			5									//��עʱ��

//����ʱ��
#define IDI_GAME_END				3									//����ʱ��
#define TIME_GAME_END				8									//����ʱ��



//////////////////////////////////////////////////////////////////////////


//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��ʼ����
	ZeroMemory(m_lUserStartScore, sizeof(m_lUserStartScore));

	//��ע��
	ZeroMemory(m_lAllBet, sizeof(m_lAllBet));
	ZeroMemory(m_lPlayBet, sizeof(m_lPlayBet));

	ZeroMemory(m_bUserListWin, sizeof(m_bUserListWin));
	ZeroMemory(m_lUserListScore, sizeof(m_lUserListScore));
	ZeroMemory(m_cbUserPlayCount, sizeof(m_cbUserPlayCount));

	//����
	m_lBankerScore = 0l;
	ZeroMemory(m_lPlayScore, sizeof(m_lPlayScore));
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));

	//�˿���Ϣ
	ZeroMemory(m_cbCardCount, sizeof(m_cbCardCount));
	ZeroMemory(m_cbTableCardArray, sizeof(m_cbTableCardArray));

	//״̬����
	m_dwBetTime = 0L;
	CopyMemory(m_szControlName, TEXT("����"), sizeof(m_szControlName));

	//ׯ����Ϣ
	m_ApplyUserArray.RemoveAll();
	m_wCurrentBanker = INVALID_CHAIR;
	m_wOfflineBanker = INVALID_CHAIR;
	m_wBankerTime = 0;
	m_lBankerWinScore = 0L;
	m_lBankerCurGameScore = 0L;
	m_bEnableSysBanker = true;

	//��¼����
	ZeroMemory(m_GameRecordArrary, sizeof(m_GameRecordArrary));
	m_nRecordFirst = 0;
	m_nRecordLast = 0;
	m_dwRecordCount = 0;

	//ׯ������
	m_nBankerTimeLimit = 0l;
	m_nBankerTimeAdd = 0l;
	m_lExtraBankerScore = 0l;
	m_nExtraBankerTime = 0l;
	m_lPlayerBankerMAX = 0l;
	m_bExchangeBanker = true;

	ZeroMemory(m_cbOpenResult, sizeof(m_cbOpenResult));

	//ʱ�����
	m_cbFreeTime = TIME_FREE;
	m_cbBetTime = TIME_PLACE_JETTON;
	m_cbEndTime = TIME_GAME_END;

	//�����˿���
	m_nChipRobotCount = 0;
	m_nRobotListMaxCount = 0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

	m_cbJuControl = FALSE;
	m_cbJuControlTimes = 0;
	ZeroMemory(m_cbJuControlArea, sizeof(m_cbJuControlArea));

	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{

}

//�ͷŶ���
VOID CTableFrameSink::Release()
{

}

//�ӿڲ�ѯ
void * CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink, Guid, dwQueryVer);
	QUERYINTERFACE(ITableUserAction, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink, Guid, dwQueryVer);
	return NULL;
}

//��ʼ��
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(pIUnknownEx != NULL);
	m_pITableFrame = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, ITableFrame);
	if (m_pITableFrame == NULL) return false;

	//��ȡ����
	m_pGameServiceOption = m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption != NULL);

	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_TIME_CONTROL);

	//��ȡ����
	memcpy(m_szGameRoomName, m_pGameServiceOption->szServerName, sizeof(m_szGameRoomName));

	//�����ļ���
	TCHAR szPath[MAX_PATH] = TEXT("");
	GetCurrentDirectory(sizeof(szPath), szPath);
	_sntprintf(m_szConfigFileName, sizeof(m_szConfigFileName), TEXT("%s\\BaiRenConfig\\RBBattleConfig-%d-%d.ini"), szPath, m_pGameServiceOption->wServerID, m_pITableFrame->GetTableID());

	m_nChip[0] = GetPrivateProfileInt(TEXT("���"), TEXT("nChip0"), 1, m_szConfigFileName);
	m_nChip[1] = GetPrivateProfileInt(TEXT("���"), TEXT("nChip1"), 10, m_szConfigFileName);
	m_nChip[2] = GetPrivateProfileInt(TEXT("���"), TEXT("nChip2"), 50, m_szConfigFileName);
	m_nChip[3] = GetPrivateProfileInt(TEXT("���"), TEXT("nChip3"), 100, m_szConfigFileName);
	m_nChip[4] = GetPrivateProfileInt(TEXT("���"), TEXT("nChip4"), 1000, m_szConfigFileName);

	m_lUserLimitScore = GetPrivateProfileInt(TEXT("���"), TEXT("nLimitMax"), 20000, m_szConfigFileName);

	ReadConfigInformation();

	return true;
}

//��λ����
void CTableFrameSink::RepositionSink()
{
	m_wOfflineBanker = INVALID_CHAIR;

	ZeroMemory(m_lAllBet, sizeof(m_lAllBet));
	ZeroMemory(m_lPlayBet, sizeof(m_lPlayBet));


	//����
	m_lBankerScore = 0l;
	ZeroMemory(m_lPlayScore, sizeof(m_lPlayScore));
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));

	//�����˿���
	m_nChipRobotCount = 0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

	ZeroMemory(m_cbOpenResult, sizeof(m_cbOpenResult));

	return;
}

//��ѯ�޶�
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	if (pIServerUserItem->GetUserStatus() == US_PLAYING)
	{
		return 0L;
	}
	else
	{
		return __max(pIServerUserItem->GetUserScore() - m_pGameServiceOption->lMinTableScore, 0L);
	}
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{
	ZeroMemory(m_cbOpenResult, sizeof(m_cbOpenResult));

	//��ȡׯ��
	IServerUserItem* pIBankerServerUserItem = NULL;
	if (m_wCurrentBanker == INVALID_CHAIR)
	{
		m_lBankerScore = 1000000000;
	}
	else
	{
		IServerUserItem* pIBankerServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if (pIBankerServerUserItem != NULL)
		{
			m_lBankerScore = pIBankerServerUserItem->GetUserScore();
		}
	}
	
	//�����˿���
	m_nChipRobotCount = 0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

	//��������
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart, sizeof(GameStart));

	//���ñ���
	GameStart.cbTimeLeave = m_cbBetTime;

	FillMemory(GameStart.wSeatUser, sizeof(GameStart.wSeatUser), INVALID_CHAIR);

	OnGetUserListGameID(GameStart.wSeatUser);

	//�Թ����
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_START, &GameStart, sizeof(GameStart));
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GAME_START, &GameStart, sizeof(GameStart));

	return true;
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������	
	{
		//�������
		SCORE lBankerWinScore = GameOver();

		//��������
		m_wBankerTime++;

		//������Ϣ
		CMD_S_GameEnd GameEnd;
		ZeroMemory(&GameEnd, sizeof(GameEnd));

		//�б������Ϣ
		for (WORD i = 0; i < GAME_PLAYER; i++)
		{
			IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);
			if (!pServerUser) continue;

			if (m_lUserWinScore[i] > 0)
			{
				m_bUserListWin[i][m_cbUserPlayCount[i]] = true;
			}

			SCORE lAddScore = 0;
			for (BYTE j = 0; j < AREA_MAX; j++)
			{
				lAddScore += m_lPlayBet[i][j];
			}
			m_lUserListScore[i][m_cbUserPlayCount[i]] = lAddScore;
			m_cbUserPlayCount[i] ++;

			if (m_cbUserPlayCount[i] == USER_LIST_COUNT)
				m_cbUserPlayCount[i] = 0;
		}

		//ռλ��ҳɼ�
		WORD wSeatUser[MAX_SEAT_COUNT] = { INVALID_CHAIR };
		FillMemory(wSeatUser, sizeof(wSeatUser), INVALID_CHAIR);
		OnGetUserListGameID(wSeatUser);

		for (WORD i = 0; i < MAX_SEAT_COUNT; i++)
		{
			if (wSeatUser[i] != INVALID_CHAIR)
			{
				GameEnd.lSeatUserWinScore[i] = m_lUserWinScore[wSeatUser[i]];
			}
		}

		//ׯ����Ϣ
		GameEnd.lBankerWinScore = lBankerWinScore;

		if (m_wCurrentBanker == INVALID_CHAIR)
			GameEnd.lBankerRestScore = 100000000;
		else
		{
			IServerUserItem* pBankerUser = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
			if (pBankerUser)
				GameEnd.lBankerRestScore = pBankerUser->GetUserScore() + lBankerWinScore;
		}

		//�˿���Ϣ
		CopyMemory(GameEnd.cbTableCardArray, m_cbTableCardArray, sizeof(m_cbTableCardArray));
		
		CopyMemory(GameEnd.cbResult, m_cbOpenResult, sizeof(GameEnd.cbResult));
		CopyMemory(GameEnd.lAllJettonScore, m_lAllBet, sizeof(GameEnd.lAllJettonScore));

		for (WORD i = 0; i < GAME_PLAYER; ++i)
		{
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
			if (pIServerUserItem == NULL) continue;

			//���óɼ�
			CopyMemory(GameEnd.lPlayAreaScore, m_lPlayScore[i], sizeof(GameEnd.lPlayAreaScore));
			GameEnd.lPlayerWinScore = m_lUserWinScore[i];
			GameEnd.lPlayerRestScore = pIServerUserItem->GetUserScore() + GameEnd.lPlayerWinScore;

			CopyMemory(GameEnd.lPlayerJettonScore, m_lPlayBet[i], sizeof(GameEnd.lPlayerJettonScore));
			
			//������Ϣ					
			m_pITableFrame->SendTableData(i, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));
			m_pITableFrame->SendLookonData(i, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));

			if (!pIServerUserItem->IsAndroidUser())
			{
				m_pGameServiceOption->pRoomStorageOption.lStorageCurrent -= GameEnd.lPlayerWinScore;
			}
		}

		tagRBPerGameRecord GameRecord;
		GameRecord.wTableID = m_pITableFrame->GetTableID() + 1;
		GameRecord.cbAreaWin[0] = m_cbOpenResult[0];
		GameRecord.cbAreaWin[1] = m_cbOpenResult[1];
		GameRecord.cbAreaWin[2] = m_cbOpenResult[2];
		GameRecord.cbAreaWin[3] = m_cbOpenResult[3];
		m_pITableFrame->SendTableDataToLookOnUser(SUB_GF_BJL_GAME_RESULT, &GameRecord, sizeof(GameRecord));

		return true;
	}
	case GER_NETWORK_ERROR:		//�����ж�
	case GER_USER_LEAVE:		//�û��뿪
	{
		return true;
		//�м��ж�
		if (m_wCurrentBanker != wChairID)
		{
			//��������
			SCORE lRevenue = 0;

			//д�����
			if (m_pITableFrame->GetGameStatus() != GAME_SCENE_END)
			{
				for (WORD wAreaIndex = AREA_XIAN; wAreaIndex <= AREA_MAX; ++wAreaIndex)
				{
					if (m_lPlayBet[wChairID][wAreaIndex] != 0)
					{
						m_lAllBet[wAreaIndex] -= m_lPlayBet[wChairID][wAreaIndex];
						m_lPlayBet[wChairID][wAreaIndex] = 0;
					}
				}
			}
			else
			{

				//�Ƿ�й��
				bool bWritePoints = false;
				for (WORD wAreaIndex = AREA_XIAN; wAreaIndex <= AREA_MAX; ++wAreaIndex)
				{
					if (m_lPlayBet[wChairID][wAreaIndex] != 0)
					{
						bWritePoints = true;
						break;
					}
				}

				//��д��
				if (!bWritePoints)
				{
					return true;
				}

				//д�����
				if (pIServerUserItem->GetUserScore()  + m_lUserWinScore[wChairID] < 0L)
					m_lUserWinScore[wChairID] = -pIServerUserItem->GetUserScore();

				//�����
				if (!pIServerUserItem->IsAndroidUser())
					m_pGameServiceOption->pRoomStorageOption.lStorageCurrent -= m_lUserWinScore[wChairID];

				tagScoreInfo ScoreInfoArray;
				ZeroMemory(&ScoreInfoArray, sizeof(ScoreInfoArray));
				ScoreInfoArray.lScore = m_lUserWinScore[wChairID];
				ScoreInfoArray.lRevenue = m_lUserRevenue[wChairID];

				if (ScoreInfoArray.lScore > 0)
					ScoreInfoArray.cbType = SCORE_TYPE_WIN;
				else if (ScoreInfoArray.lScore < 0)
					ScoreInfoArray.cbType = SCORE_TYPE_LOSE;
				else
					ScoreInfoArray.cbType = SCORE_TYPE_DRAW;

				m_pITableFrame->WriteUserScore(wChairID, ScoreInfoArray);
				m_lUserWinScore[wChairID] = 0;
				m_lUserRevenue[wChairID] = 0;
				ZeroMemory(m_lPlayScore[wChairID], sizeof(m_lPlayScore[wChairID]));


				//�����ע
				for (WORD wAreaIndex = AREA_XIAN; wAreaIndex <= AREA_MAX; ++wAreaIndex)
				{
					m_lPlayBet[wChairID][wAreaIndex] = 0;
				}
			}

			return true;
		}

		return true;
	}
	}

	return false;
}

//���ͳ���
bool CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_BET:		//��Ϸ״̬
	case GAME_SCENE_END:		//����״̬
	{
		//��������
		CMD_S_StatusPlay StatusPlay;
		ZeroMemory(&StatusPlay, sizeof(StatusPlay));

		FillMemory(StatusPlay.wApplyUser, sizeof(StatusPlay.wApplyUser), INVALID_CHAIR);
		FillMemory(StatusPlay.wSeatUser, sizeof(StatusPlay.wSeatUser), INVALID_CHAIR);

		//ȫ����Ϣ
		DWORD dwPassTime = (DWORD)time(NULL) - m_dwBetTime;
		int	nTotalTime = ((cbGameStatus == GAME_SCENE_BET) ? m_cbBetTime : m_cbEndTime);
		StatusPlay.cbTimeLeave = (BYTE)(nTotalTime - __min(dwPassTime, (DWORD)nTotalTime));
		
		//ׯ����Ϣ
		StatusPlay.wBankerUser = m_wCurrentBanker;
		StatusPlay.lBankerWinScore = m_lBankerWinScore;

		//��ȡׯ��
		IServerUserItem* pIBankerServerUserItem = NULL;
		if (m_wCurrentBanker == INVALID_CHAIR)
		{
			m_lBankerScore = 1000000000;
		}
		else
		{
			IServerUserItem* pIBankerServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
			if (pIBankerServerUserItem != NULL)
			{
				m_lBankerScore = pIBankerServerUserItem->GetUserScore();
			}
		}

		StatusPlay.lBankerScore = m_lBankerScore;

		//������Ϣ
		StatusPlay.lApplyBankerCondition = m_lApplyBankerCondition;
		StatusPlay.lAreaLimitScore = m_lAreaLimitScore;

		StatusPlay.cbFreeTime = m_cbFreeTime;
		StatusPlay.cbPlayTime = m_cbBetTime;
		StatusPlay.cbEndTime = m_cbEndTime;

		StatusPlay.lMinXianHong = m_nChip[0];
		StatusPlay.lMaxXianHong = m_lUserLimitScore;

		CopyMemory(StatusPlay.lPlayerJettonScore, m_lPlayBet[wChiarID], sizeof(StatusPlay.lPlayerJettonScore));
		CopyMemory(StatusPlay.lAllJettonScore, m_lAllBet, sizeof(StatusPlay.lAllJettonScore));

		

		//�����ж�
		if (cbGameStatus == GAME_SCENE_END)
		{
			//�˿���Ϣ
			CopyMemory(StatusPlay.cbTableCardArray, m_cbTableCardArray, sizeof(m_cbTableCardArray));

			StatusPlay.lPlayerWinScore = m_lUserWinScore[pIServerUserItem->GetChairID()];

			CopyMemory(StatusPlay.cbResult, m_cbOpenResult, sizeof(StatusPlay.cbResult));

			//ռλ��ҳɼ�
			WORD wSeatUser[MAX_SEAT_COUNT] = { INVALID_CHAIR };
			FillMemory(wSeatUser, sizeof(wSeatUser), INVALID_CHAIR);
			OnGetUserListGameID(wSeatUser);

			for (WORD i = 0; i < MAX_SEAT_COUNT; i++)
			{
				if (wSeatUser[i] != INVALID_CHAIR)
				{
					StatusPlay.lSeatUserWinScore[i] = m_lUserWinScore[wSeatUser[i]];
				}
			}

			if (m_wCurrentBanker != INVALID_CHAIR)
			{
				IServerUserItem* pIBankerServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				if (pIBankerServerUserItem != NULL)
					StatusPlay.lBankerRestScore = pIBankerServerUserItem->GetUserScore() + m_lUserWinScore[m_wCurrentBanker];
			}
			else
				StatusPlay.lBankerRestScore = 1000000000;

			StatusPlay.lPlayerRestScore = pIServerUserItem->GetUserScore() + m_lUserWinScore[pIServerUserItem->GetChairID()];
		}
		else
		{
			if (m_wCurrentBanker != INVALID_CHAIR)
			{
				IServerUserItem* pIBankerServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				if (pIBankerServerUserItem != NULL)
					StatusPlay.lBankerRestScore = pIBankerServerUserItem->GetUserScore();
			}
			else
				StatusPlay.lBankerRestScore = 1000000000;
		}

		CopyMemory(StatusPlay.nChip, m_nChip, sizeof(StatusPlay.nChip));

		for (INT_PTR nUserIdx = 0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
		{
			WORD wChairID = m_ApplyUserArray[nUserIdx];

			//��ȡ���
			IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
			if (!pServerUserItem) continue;

			//ׯ���ж�
			if (pServerUserItem->GetChairID() == m_wCurrentBanker) continue;

			StatusPlay.wApplyUser[nUserIdx] = wChairID;
		}

		OnGetUserListGameID(StatusPlay.wSeatUser);

		for (WORD i = 0; i < MAX_SEAT_COUNT; i++)
		{
			if (StatusPlay.wSeatUser[i] != INVALID_CHAIR)
			{
				WORD wSeatChair = StatusPlay.wSeatUser[i];
				CopyMemory(StatusPlay.lSeatUserAreaScore[i], m_lPlayBet[wSeatChair], sizeof(StatusPlay.lSeatUserAreaScore[i]));
			}
		}

		//���ͳ���
		bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem, &StatusPlay, sizeof(StatusPlay));

		//����������
		if (pIServerUserItem->IsAndroidUser())
		{
			tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pITableFrame->GetCustomRule();
			ASSERT(pCustomConfig);

			m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_ANDROID_CONFIG, &pCustomConfig->CustomAndroid, sizeof(tagCustomAndroid));
		}

		//���ͼ�¼
		SendGameRecord(pIServerUserItem);

		return bSuccess;
	}
	}

	return false;
}

//��ʱ���¼�
bool  CTableFrameSink::OnTimerMessage(DWORD dwTimerID, WPARAM dwBindParameter)
{
	switch (dwTimerID)
	{
	case IDI_PLACE_JETTON:		//��עʱ��
	{
		//״̬�ж�(��ֹǿ���ظ�����)
		if (m_pITableFrame->GetGameStatus() != GAME_SCENE_END)
		{
			//����״̬
			m_pITableFrame->SetGameStatus(GAME_SCENE_END);

			//������Ϸ
			OnEventGameConclude(INVALID_CHAIR, NULL, GER_NORMAL);

			//����ʱ��
			m_dwBetTime = (DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_GAME_END, m_cbEndTime * 1000, 1, 0L);

			tagChangeStatus ChangeStatus;
			ChangeStatus.wTableID = m_pITableFrame->GetTableID() + 1;
			ChangeStatus.cbGameStatus = m_pITableFrame->GetGameStatus();
			m_pITableFrame->SendTableDataToLookOnUser(SUB_GF_BJL_CHANGE_STATUS, &ChangeStatus, sizeof(ChangeStatus));
		}

		return true;
	}
	case IDI_GAME_END:			//������Ϸ
	{
		tagScoreInfo ScoreInfoArray[GAME_PLAYER];
		ZeroMemory(&ScoreInfoArray, sizeof(ScoreInfoArray));
		SCORE TempStartScore = 0;
		//д�����
		for (WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID)
		{
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);
			if (pIServerUserItem == NULL) continue;

			//�ж��Ƿ�д��
			bool bXie = false;
			for (WORD iA = 0; iA < AREA_MAX; ++iA)
			{
				if (m_lPlayBet[wUserChairID][iA] != 0)
				{
					bXie = true;
					break;
				}
			}

			//��ǰׯ��
			if (wUserChairID == m_wCurrentBanker)
			{
				bXie = true;
			}

			//����ׯ��
			if (wUserChairID == m_wOfflineBanker)
			{
				bXie = true;
			}

			if (!bXie)
			{
				continue;
			}

			//��ֹ����
			if ((pIServerUserItem->GetUserScore() + m_lUserWinScore[wUserChairID]) < 0L)
				m_lUserWinScore[wUserChairID] = -pIServerUserItem->GetUserScore();

			//д�����				
			ScoreInfoArray[wUserChairID].lScore = m_lUserWinScore[wUserChairID];
			ScoreInfoArray[wUserChairID].lRevenue = m_lUserRevenue[wUserChairID];

			if (ScoreInfoArray[wUserChairID].lScore > 0)
			{
				ScoreInfoArray[wUserChairID].cbType = SCORE_TYPE_WIN;
			}
			else if (ScoreInfoArray[wUserChairID].lScore < 0)
				ScoreInfoArray[wUserChairID].cbType = SCORE_TYPE_LOSE;
			else
				ScoreInfoArray[wUserChairID].cbType = SCORE_TYPE_DRAW;

		}

		m_pITableFrame->WriteTableScore(ScoreInfoArray, CountArray(ScoreInfoArray));

		//�����������Ϣ
		for (WORD i = 0; i < GAME_PLAYER; i++)
		{
			IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
			if (pServerUser && m_lUserWinScore[i] > 1000)
			{
				CMD_GR_GameRadioMessage GameRadioMessage;
				ZeroMemory(&GameRadioMessage, sizeof(GameRadioMessage));

				GameRadioMessage.cbCardType = 0;
				GameRadioMessage.wKindID = m_pGameServiceOption->wKindID;
				GameRadioMessage.lScore = m_lUserWinScore[i];
				lstrcpyn(GameRadioMessage.szNickName, pServerUser->GetNickName(), CountArray(GameRadioMessage.szNickName));
				m_pITableFrame->SendHallRadioMessage(&GameRadioMessage, sizeof(GameRadioMessage));
			}
		}

		//������Ϸ
		m_pITableFrame->ConcludeGame(GAME_SCENE_BET);

		//��ʼ��Ϸ
		m_pITableFrame->StartGame();

		//����ʱ��
		m_dwBetTime = (DWORD)time(NULL);
		m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON, m_cbBetTime * 1000, 1, 0L);

		//���˥��
		m_pITableFrame->CalculateStorageDeduct();

		tagChangeStatus ChangeStatus;
		ChangeStatus.wTableID = m_pITableFrame->GetTableID() + 1;
		ChangeStatus.cbGameStatus = m_pITableFrame->GetGameStatus();
		m_pITableFrame->SendTableDataToLookOnUser(SUB_GF_BJL_CHANGE_STATUS, &ChangeStatus, sizeof(ChangeStatus));

		return true;
	}
	}

	return false;
}

//��ȡ������Ϸ�Ƿ���ע״̬���������ע��
bool CTableFrameSink::OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	for (WORD wAreaIndex = AREA_XIAN; wAreaIndex <= AREA_MAX; ++wAreaIndex)
	{
		if (m_lPlayBet[wChairID][wAreaIndex] != 0)
		{
			return true;
		}
	}

	if (wChairID == m_wCurrentBanker)
		return true;

	return false;
}

//�����û��б�
bool CTableFrameSink::OnUserRequestUserList(WORD wChairID)
{
	CMD_S_UserListInfo UserListInfo[GAME_PLAYER];
	ZeroMemory(UserListInfo, sizeof(UserListInfo));

	BYTE cbListUserCount = 0;

	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);
		if (!pServerUser) continue;

		SCORE lAddScore = 0;
		BYTE cbWinCount = 0;

		for (BYTE j = 0; j < USER_LIST_COUNT; j++)
		{
			if (m_bUserListWin[i][j])
				cbWinCount++;
			lAddScore += m_lUserListScore[i][j];
		}

		UserListInfo[cbListUserCount].wWinNum = cbWinCount;
		UserListInfo[cbListUserCount].lAllBet = lAddScore;
		UserListInfo[cbListUserCount].lUserScore = pServerUser->GetUserScore();
		lstrcpyn(UserListInfo[cbListUserCount].szNickName, pServerUser->GetNickName(), CountArray(UserListInfo[cbListUserCount].szNickName));
		UserListInfo[cbListUserCount].wFaceID = pServerUser->GetUserInfo()->wFaceID;
		cbListUserCount++;
	}

	//�ҳ�һ����ʤ�������ķŵ�һ��
	for (WORD i = 1; i < cbListUserCount; i++)
	{
		bool bExchange = false;
		if (UserListInfo[i].wWinNum > UserListInfo[0].wWinNum)
			bExchange = true;
		else if (UserListInfo[i].wWinNum == UserListInfo[0].wWinNum)
		{
			if (UserListInfo[i].lAllBet > UserListInfo[0].lAllBet)
				bExchange = true;
		}

		if (bExchange)
		{
			CMD_S_UserListInfo UserTempInfo;
			ZeroMemory(&UserTempInfo, sizeof(UserTempInfo));

			CopyMemory(&UserTempInfo, &UserListInfo[i], sizeof(UserTempInfo));
			CopyMemory(&UserListInfo[i], &UserListInfo[0], sizeof(UserTempInfo));
			CopyMemory(&UserListInfo[0], &UserTempInfo, sizeof(UserTempInfo));
		}
	}

	//ʣ�µİ���ע������
	for (WORD i = 1; i < cbListUserCount; i++)
	{
		for (WORD j = i + 1; j < cbListUserCount; j++)
		{
			if (UserListInfo[j].lAllBet > UserListInfo[i].lAllBet)
			{
				CMD_S_UserListInfo UserTempInfo;
				ZeroMemory(&UserTempInfo, sizeof(UserTempInfo));

				CopyMemory(&UserTempInfo, &UserListInfo[i], sizeof(UserTempInfo));
				CopyMemory(&UserListInfo[i], &UserListInfo[j], sizeof(UserTempInfo));
				CopyMemory(&UserListInfo[j], &UserTempInfo, sizeof(UserTempInfo));
			}
		}
	}

	//Ȼ��ʼ�����ˡ�����
	BYTE cbMaxTimes = cbListUserCount / 20;
	if (cbListUserCount % 20 != 0)
		cbMaxTimes += 1;

	for (int i = 0; i < cbMaxTimes; i++)
	{
		BYTE cbRestCount = 20;
		if (i == (cbMaxTimes - 1))
			cbRestCount = cbListUserCount % 20;

		CMD_S_UserList UpdateUserList;
		ZeroMemory(&UpdateUserList, sizeof(UpdateUserList));
		UpdateUserList.wCount = cbRestCount;
		if (i != (cbMaxTimes - 1))
			UpdateUserList.bEnd = false;
		else
			UpdateUserList.bEnd = true;

		for (int j = 0; j < cbRestCount; j++)
		{
			UpdateUserList.cbIndex[j] = i * 20 + j;
		}

		for (int j = 0; j < cbRestCount; j++)
			UpdateUserList.cbWinTimes[j] = UserListInfo[i * 20 + j].wWinNum;
		for (int j = 0; j < cbRestCount; j++)
			UpdateUserList.lBetScore[j] = UserListInfo[i * 20 + j].lAllBet;
		for (int j = 0; j < cbRestCount; j++)
			UpdateUserList.lUserScore[j] = UserListInfo[i * 20 + j].lUserScore;


		for (int j = 0; j < cbRestCount; j++)
			lstrcpyn(UpdateUserList.szUserNick[j], UserListInfo[i * 20 + j].szNickName, CountArray(UpdateUserList.szUserNick[j]));

		for (int j = 0; j < cbRestCount; j++)
			UpdateUserList.wFaceID[j] = UserListInfo[i * 20 + j].wFaceID;

		m_pITableFrame->SendTableData(wChairID, SUB_S_ONLINE_PLAYER, &UpdateUserList, sizeof(UpdateUserList));
		m_pITableFrame->SendLookonData(wChairID, SUB_S_ONLINE_PLAYER, &UpdateUserList, sizeof(UpdateUserList));
	}

	return true;
}

//��ȡǰ6���û��б�����Ӻ�
bool CTableFrameSink::OnGetUserListGameID(WORD wSeatUser[MAX_SEAT_COUNT])
{
	CMD_S_UserListInfo UserListInfo[GAME_PLAYER];
	ZeroMemory(UserListInfo, sizeof(UserListInfo));

	BYTE cbListUserCount = 0;

	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);
		if (!pServerUser) continue;

		SCORE lAddScore = 0;
		BYTE cbWinCount = 0;

		for (BYTE j = 0; j < USER_LIST_COUNT; j++)
		{
			if (m_bUserListWin[i][j])
				cbWinCount++;
			lAddScore += m_lUserListScore[i][j];
		}

		UserListInfo[cbListUserCount].wWinNum = cbWinCount;
		UserListInfo[cbListUserCount].lAllBet = lAddScore;
		UserListInfo[cbListUserCount].lUserScore = pServerUser->GetUserScore();
		UserListInfo[cbListUserCount].wChairID = pServerUser->GetChairID();
		lstrcpyn(UserListInfo[cbListUserCount].szNickName, pServerUser->GetNickName(), CountArray(UserListInfo[cbListUserCount].szNickName));
		cbListUserCount++;
	}

	//�ҳ�һ����ʤ�������ķŵ�һ��
	for (WORD i = 1; i < cbListUserCount; i++)
	{
		bool bExchange = false;
		if (UserListInfo[i].wWinNum > UserListInfo[0].wWinNum)
			bExchange = true;
		else if (UserListInfo[i].wWinNum == UserListInfo[0].wWinNum)
		{
			if (UserListInfo[i].lAllBet > UserListInfo[0].lAllBet)
				bExchange = true;
		}

		if (bExchange)
		{
			CMD_S_UserListInfo UserTempInfo;
			ZeroMemory(&UserTempInfo, sizeof(UserTempInfo));

			CopyMemory(&UserTempInfo, &UserListInfo[i], sizeof(UserTempInfo));
			CopyMemory(&UserListInfo[i], &UserListInfo[0], sizeof(UserTempInfo));
			CopyMemory(&UserListInfo[0], &UserTempInfo, sizeof(UserTempInfo));
		}
	}

	//ʣ�µİ���ע������
	for (WORD i = 1; i < cbListUserCount; i++)
	{
		for (WORD j = i + 1; j < cbListUserCount; j++)
		{
			if (UserListInfo[j].lAllBet > UserListInfo[i].lAllBet)
			{
				CMD_S_UserListInfo UserTempInfo;
				ZeroMemory(&UserTempInfo, sizeof(UserTempInfo));

				CopyMemory(&UserTempInfo, &UserListInfo[i], sizeof(UserTempInfo));
				CopyMemory(&UserListInfo[i], &UserListInfo[j], sizeof(UserTempInfo));
				CopyMemory(&UserListInfo[j], &UserTempInfo, sizeof(UserTempInfo));
			}
		}
	}

	for (int i = 0; i < MAX_SEAT_COUNT; i++)
	{
		if (cbListUserCount > i)
			wSeatUser[i] = UserListInfo[i].wChairID;
	}

	return true;
}

//������Ϸ��ȡ��Ϸ��¼
void CTableFrameSink::OnGetGameRecord(VOID *GameRecord)
{
	CMD_GF_RBRoomStatus *pRoomStatus = (CMD_GF_RBRoomStatus *)GameRecord;

	pRoomStatus->tagGameInfo.wTableID = m_pITableFrame->GetTableID() + 1;
	pRoomStatus->tagGameInfo.cbGameStatus = m_pITableFrame->GetGameStatus();

	pRoomStatus->tagTimeInfo.cbBetTime = m_cbBetTime;
	pRoomStatus->tagTimeInfo.cbEndTime = m_cbEndTime;
	pRoomStatus->tagTimeInfo.cbPassTime = (DWORD)time(NULL) - m_dwBetTime;
	pRoomStatus->tagTimeInfo.lMinXianHong = m_nChip[0];
	pRoomStatus->tagTimeInfo.lMaxXianHong = m_lUserLimitScore;

	//�ͻ���ֻ��ʾ48��
	int nIndex = m_nRecordFirst;

	pRoomStatus->cbRecordCount = m_nRecordLast - m_nRecordFirst;
	if (pRoomStatus->cbRecordCount > 48)
	{
		nIndex = m_nRecordLast - 48;
		pRoomStatus->cbRecordCount = 48;
	}

	int nArrayIndex = 0;

	while (nIndex != m_nRecordLast)
	{
		if (nArrayIndex >= 48)
			break;
		pRoomStatus->GameRecordArrary[nArrayIndex].cbAreaWin[0] = m_GameRecordArrary[nIndex].bPlayer;
		pRoomStatus->GameRecordArrary[nArrayIndex].cbAreaWin[1] = m_GameRecordArrary[nIndex].bBanker;
		pRoomStatus->GameRecordArrary[nArrayIndex].cbAreaWin[2] = m_GameRecordArrary[nIndex].bPing;
		pRoomStatus->GameRecordArrary[nArrayIndex].cbAreaWin[3] = m_GameRecordArrary[nIndex].cbCardType;

		nArrayIndex++;
		nIndex = (nIndex + 1) % MAX_SCORE_HISTORY;
	}
}

//��Ϸ��Ϣ����
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_PLACE_JETTON:		//�û���ע
	{
		//Ч������
		ASSERT(wDataSize == sizeof(CMD_C_PlaceBet));
		if (wDataSize != sizeof(CMD_C_PlaceBet)) return true;

		//��Ϣ����
		CMD_C_PlaceBet * pPlaceBet = (CMD_C_PlaceBet *)pData;

		return OnUserPlayBet(pIServerUserItem->GetChairID(), pPlaceBet->cbBetArea, pPlaceBet->lBetScore);
	}
	case SUB_C_APPLY_BANKER:		//������ׯ
	{
		return true;
	}
	case SUB_C_CANCEL_BANKER:		//ȡ����ׯ
	{
		return true;
	}
	case SUB_C_ONLINE_PLAYER:		//��ȡ�û��б�
		{
			return OnUserRequestUserList(pIServerUserItem->GetChairID());
		}
	case SUB_C_REQUEST_CONTROL:		//������Ϣ����
		{
			//Ȩ���ж�
			if (CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) == false)
				return true;

			CMD_S_ControlInfo ControlInfo;
			ZeroMemory(&ControlInfo, sizeof(ControlInfo));

			ControlInfo.cbJuControl = m_cbJuControl;
			ControlInfo.cbJuControlTimes = m_cbJuControlTimes;

			CopyMemory(ControlInfo.cbJuControlArea, m_cbJuControlArea, sizeof(ControlInfo.cbJuControlArea));
			LONGLONG lUserBetAllScore[GAME_PLAYER] = { 0 };
			BYTE cbIndex = 0;
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);
				if (pServerUser && !pServerUser->IsAndroidUser())
				{
					CopyMemory(ControlInfo.lUserJettonScore[cbIndex], m_lPlayBet[i], sizeof(ControlInfo.lUserJettonScore[cbIndex]));
					lstrcpyn(ControlInfo.szNickName[cbIndex], pServerUser->GetNickName(), sizeof(ControlInfo.szNickName[cbIndex]));

					for (BYTE j = 0; j < AREA_MAX; j++)
					{
						lUserBetAllScore[cbIndex] += m_lPlayBet[i][j];
						ControlInfo.lAreaJettonScore[j] += m_lPlayBet[i][j];
					}

					cbIndex++;
				}
			}

			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				for (WORD j = i + 1; j < GAME_PLAYER; j++)
				{
					if (lUserBetAllScore[i] < lUserBetAllScore[j])
					{
						LONGLONG lTempScore = lUserBetAllScore[i];
						lUserBetAllScore[i] = lUserBetAllScore[j];
						lUserBetAllScore[j] = lTempScore;

						LONGLONG lAreaTempScore[AREA_MAX] = { 0 };
						CopyMemory(lAreaTempScore, ControlInfo.lUserJettonScore[i], sizeof(lAreaTempScore));
						CopyMemory(ControlInfo.lUserJettonScore[i], ControlInfo.lUserJettonScore[j], sizeof(lAreaTempScore));
						CopyMemory(ControlInfo.lUserJettonScore[j], lAreaTempScore, sizeof(lAreaTempScore));

						TCHAR szTempNick[32] = { 0 };
						CopyMemory(szTempNick, ControlInfo.szNickName[i], sizeof(szTempNick));
						CopyMemory(ControlInfo.szNickName[i], ControlInfo.szNickName[j], sizeof(szTempNick));
						CopyMemory(ControlInfo.szNickName[j], szTempNick, sizeof(szTempNick));
					}
				}
			}

			m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_RESPONSE_CONTROL, &ControlInfo, sizeof(ControlInfo));

			return true;
		}
	case SUB_C_CONTROL:				//���ֿ�����Ӯ
		{
			//Ȩ���ж�
			if (CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) == false)
				return true;

			CMD_C_ControlWinLost *pControlWinLost = (CMD_C_ControlWinLost *)pData;

			CopyMemory(m_cbJuControlArea, pControlWinLost->cbJuControlArea, sizeof(m_cbJuControlArea));

			m_cbJuControl = TRUE;
			m_cbJuControlTimes = pControlWinLost->cbControlTimes;

			if (m_cbJuControlArea[AREA_XIAN] == 1 && m_cbJuControlArea[AREA_ZHUANG] == 1)
			{
				m_cbJuControlArea[AREA_ZHUANG] = 0;
			}

			if (m_cbJuControlTimes < 0 || m_cbJuControlTimes > 5)
			{
				m_cbJuControl = FALSE;
				m_cbJuControlTimes = 0;
				ZeroMemory(m_cbJuControlArea, sizeof(m_cbJuControlArea));
			}

			return true;
		}
	case SUB_C_CANCEL_CONTROL:		//ȡ�����ֿ���
		{
			//Ȩ���ж�
			if (CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) == false)
				return true;

			m_cbJuControl = FALSE;
			m_cbJuControlTimes = 0;
			ZeroMemory(m_cbJuControlArea, sizeof(m_cbJuControlArea));

			return true;
		}
	}

	return false;
}

//�����Ϣ����
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�����
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��ʼ����
	m_lUserStartScore[wChairID] = pIServerUserItem->GetUserScore();

	//����ʱ��
	if ((bLookonUser == false) && (m_dwBetTime == 0L))
	{
		m_dwBetTime = (DWORD)time(NULL);
		m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON, m_cbBetTime * 1000, 1, NULL);
		m_pITableFrame->SetGameStatus(GAME_SCENE_BET);
		m_pITableFrame->StartGame();
	}

	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��ʼ����
	m_lUserStartScore[wChairID] = 0;

	ZeroMemory(m_bUserListWin[wChairID], sizeof(m_bUserListWin[wChairID]));
	ZeroMemory(m_lUserListScore[wChairID], sizeof(m_lUserListScore[wChairID]));
	m_cbUserPlayCount[wChairID] = 0;


	return true;
}

//��ע�¼�
bool CTableFrameSink::OnUserPlayBet(WORD wChairID, BYTE cbBetArea, SCORE lBetScore)
{
	//Ч�����
	ASSERT((cbBetArea <= AREA_MAX) && (lBetScore > 0L));
	if ((cbBetArea > AREA_MAX) || (lBetScore <= 0L))
	{
		CMD_S_PlaceBetFail PlaceBetFail;
		_sntprintf(PlaceBetFail.szBuffer, sizeof(PlaceBetFail.szBuffer), TEXT("��ע��Ϣ����"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceBetFail, sizeof(PlaceBetFail));
		return true;
	}

	//Ч��״̬
	if (m_pITableFrame->GetGameStatus() != GAME_SCENE_BET)
	{
		CMD_S_PlaceBetFail PlaceBetFail;
		_sntprintf(PlaceBetFail.szBuffer, sizeof(PlaceBetFail.szBuffer), TEXT("��ǰ������ע״̬��������ע��"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceBetFail, sizeof(PlaceBetFail));
		return true;
	}

	//ׯ���ж�
	if (m_wCurrentBanker == wChairID)
	{
		CMD_S_PlaceBetFail PlaceBetFail;
		_sntprintf(PlaceBetFail.szBuffer, sizeof(PlaceBetFail.szBuffer), TEXT("ׯ�Ҳ�����ע��"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceBetFail, sizeof(PlaceBetFail));
		return true;
	}
	if (m_bEnableSysBanker == false && m_wCurrentBanker == INVALID_CHAIR)
	{
		CMD_S_PlaceBetFail PlaceBetFail;
		_sntprintf(PlaceBetFail.szBuffer, sizeof(PlaceBetFail.szBuffer), TEXT("�÷���ϵͳ��ׯ������ע��"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceBetFail, sizeof(PlaceBetFail));
		return true;
	}

	//ׯ�һ���
	SCORE lBankerScore = 0;
	if (INVALID_CHAIR != m_wCurrentBanker)
	{
		IServerUserItem * pIServerUserItemBanker = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		lBankerScore = pIServerUserItemBanker->GetUserScore();
	}

	//��������
	IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
	SCORE lUserScore = pIServerUserItem->GetUserScore();
	SCORE lBetCount = 0;
	for (int i = 0; i < AREA_MAX; ++i)
	{
		lBetCount += m_lPlayBet[wChairID][i];
	}

	SCORE lAllBetCount = 0L;
	for (int nAreaIndex = 0; nAreaIndex < AREA_MAX; ++nAreaIndex)
	{
		lAllBetCount += m_lAllBet[nAreaIndex];
	}

	//�ɹ���ʶ
	bool bPlaceBetSuccess = true;

	//�Ϸ�У��
	if (lUserScore < lBetCount + lBetScore)
	{
		CMD_S_PlaceBetFail PlaceBetFail;
		_sntprintf(PlaceBetFail.szBuffer, sizeof(PlaceBetFail.szBuffer), TEXT("���Ľ�Ҳ��㣡"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceBetFail, sizeof(PlaceBetFail));
		return true;
	}
	else
	{
		//��������֤
		if (pIServerUserItem->IsAndroidUser())
		{
			//��������
			if (m_lRobotAreaScore[cbBetArea] + lBetScore > m_lRobotAreaLimit)
				return true;

			//��Ŀ����
			bool bHaveChip = false;
			bHaveChip = (lBetCount > 0);

			//ͳ�Ʒ���
			if (bPlaceBetSuccess)
				m_lRobotAreaScore[cbBetArea] += lBetScore;
		}
	}

	if (m_lUserLimitScore < lBetCount + lBetScore)
	{
		CMD_S_PlaceBetFail PlaceBetFail;
		_sntprintf(PlaceBetFail.szBuffer, sizeof(PlaceBetFail.szBuffer), TEXT("������ע�ѵ������ƣ�"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceBetFail, sizeof(PlaceBetFail));
		return true;
	}


	//�Ϸ���֤
	if (GetMaxPlayerScore(cbBetArea, wChairID) >= lBetScore)
	{
		m_lAllBet[cbBetArea] += lBetScore;
		m_lPlayBet[wChairID][cbBetArea] += lBetScore;
	}
	else
	{
		bPlaceBetSuccess = false;
	}

	if (bPlaceBetSuccess)
	{
		//��������
		CMD_S_PlaceBet PlaceBet;
		ZeroMemory(&PlaceBet, sizeof(PlaceBet));

		//�������
		PlaceBet.wChairID = wChairID;
		PlaceBet.cbBetArea = cbBetArea;
		PlaceBet.lBetScore = lBetScore;
		
		SCORE lPlayerTableScore = m_lPlayBet[wChairID][0] + m_lPlayBet[wChairID][1] + m_lPlayBet[wChairID][2];

		PlaceBet.lPlayerRestScore = pIServerUserItem->GetUserScore() - lPlayerTableScore;

		WORD wSeatUser[MAX_SEAT_COUNT] = { 0 };
		FillMemory(wSeatUser, sizeof(wSeatUser), INVALID_CHAIR);
		OnGetUserListGameID(wSeatUser);

		for (WORD i = 0; i < GAME_PLAYER; i++)
		{
			bool bIsSeatUser = false;
			for (WORD j = 0; j < MAX_SEAT_COUNT; j++)
			{
				if (wSeatUser[j] == wChairID)
					bIsSeatUser = true;
			}
			//������Ϣ
			if (bIsSeatUser)
				m_pITableFrame->SendTableData(i, SUB_S_SEAT_JETTON, &PlaceBet, sizeof(PlaceBet));
			else if (i == wChairID)
				m_pITableFrame->SendTableData(i, SUB_S_PLACE_JETTON, &PlaceBet, sizeof(PlaceBet));
			else
				m_pITableFrame->SendTableData(i, SUB_S_OTHER_JETTON, &PlaceBet, sizeof(PlaceBet));
		}
		
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_PLACE_JETTON, &PlaceBet, sizeof(PlaceBet));
	}
	else
	{
		CMD_S_PlaceBetFail PlaceBetFail;
		_sntprintf(PlaceBetFail.szBuffer, sizeof(PlaceBetFail.szBuffer), TEXT("������ע�ѵ������ƣ�"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceBetFail, sizeof(PlaceBetFail));
	}

	return true;
}

//�����˿�
bool CTableFrameSink::DispatchTableCard()
{
	//����˿�
	m_GameLogic.RandCardList(m_cbTableCardArray[0], sizeof(m_cbTableCardArray) / sizeof(m_cbTableCardArray[0][0]));

	//�״η���
	m_cbCardCount[INDEX_PLAYER] = MAX_COUNT;
	m_cbCardCount[INDEX_BANKER] = MAX_COUNT;

	//�������
	BYTE cbBankerCount = m_GameLogic.GetCardType(m_cbTableCardArray[INDEX_BANKER], m_cbCardCount[INDEX_BANKER]);
	BYTE cbPlayerTwoCardCount = m_GameLogic.GetCardType(m_cbTableCardArray[INDEX_PLAYER], m_cbCardCount[INDEX_PLAYER]);

	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return true;
}

//�����ע
SCORE CTableFrameSink::GetMaxPlayerScore(BYTE cbBetArea, WORD wChairID)
{
	//��ȡ���
	IServerUserItem *pIMeServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
	if (NULL == pIMeServerUserItem)
		return 0L;

	if (cbBetArea >= AREA_MAX)
		return 0L;

	//����ע��
	SCORE lNowBet = 0l;
	for (int nAreaIndex = 0; nAreaIndex < AREA_MAX; ++nAreaIndex)
		lNowBet += m_lPlayBet[wChairID][nAreaIndex];

	//�����ע
	SCORE lMaxBet = 0L;

	//�����ע
	lMaxBet = min(pIMeServerUserItem->GetUserScore() - lNowBet, m_lUserLimitScore - m_lPlayBet[wChairID][cbBetArea]);
	lMaxBet = min(lMaxBet, m_lAreaLimitScore - m_lAllBet[cbBetArea]);

	//��������
	ASSERT(lMaxBet >= 0);
	lMaxBet = max(lMaxBet, 0);

	return lMaxBet;
}


//����÷�
SCORE CTableFrameSink::CalculateScore(OUT SCORE& lBankerWinScore, OUT tagServerGameRecord& GameRecord)
{
	//�����Ƶ�
	BYTE cbPlayerCount = m_GameLogic.GetCardType(m_cbTableCardArray[INDEX_PLAYER], m_cbCardCount[INDEX_PLAYER]);
	BYTE cbBankerCount = m_GameLogic.GetCardType(m_cbTableCardArray[INDEX_BANKER], m_cbCardCount[INDEX_BANKER]);

	//ϵͳ��Ӯ
	SCORE lSystemScore = 0;

	//�ƶ����
	BYTE cbWinArea[AREA_MAX] = { FALSE };
	DeduceWinner(cbWinArea);

	CopyMemory(m_cbOpenResult, cbWinArea, sizeof(cbWinArea));

	//��Ϸ��¼
	GameRecord.bPlayer = cbWinArea[AREA_XIAN] == TRUE;
	GameRecord.bBanker = cbWinArea[AREA_ZHUANG] == TRUE;
	//GameRecord.bPing = cbWinArea[AREA_PING] == TRUE;


	//��ҳɼ�
	SCORE lUserLostScore[GAME_PLAYER];
	ZeroMemory(m_lPlayScore, sizeof(m_lPlayScore));
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	//����һ������
	int pingPoint = 0;
	int pingType = 0;

	/*
	#define CT_SINGLE					1									//��������
	#define CT_DOUBLE					2									//��������
	#define CT_DOUBLE_SHINE				3									//���������
	#define	CT_SHUN_ZI					4									//˳������
	#define CT_JIN_HUA					5									//������
	#define	CT_SHUN_JIN					6									//˳������
	#define	CT_BAO_ZI					7									//��������
	#define CT_SPECIAL					8									//��������
	*/

	GameRecord.cbCardType = 1;

	if (cbPlayerCount == 7 || cbBankerCount == 7)
	{
		pingPoint = 11;
		pingType = 7;
		GameRecord.cbCardType = 6;
	}
	else if (cbPlayerCount == 6 || cbBankerCount == 6)
	{
		pingPoint = 6;
		pingType = 6;
		GameRecord.cbCardType = 5;
	}
	else if (cbPlayerCount == 5 || cbBankerCount == 5)
	{
		pingPoint = 4;
		pingType = 5;
		GameRecord.cbCardType = 4;
	}
	else if (cbPlayerCount == 4 || cbBankerCount == 4)
	{
		pingPoint = 3;
		pingType = 4;
		GameRecord.cbCardType = 3;
	}
	else if (cbPlayerCount == 3 || cbBankerCount == 3)
	{
		pingPoint = 2;
		pingType = 3;
		GameRecord.cbCardType = 2;
	}

	GameRecord.bPing = pingType > 0 ? TRUE:FALSE;

	m_cbOpenResult[AREA_MAX] = GameRecord.cbCardType;

	//ׯ���ǲ��ǻ�����
	bool bIsBankerAndroidUser = false;
	if (m_wCurrentBanker != INVALID_CHAIR)
	{
		IServerUserItem * pIBankerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if (pIBankerUserItem != NULL)
		{
			bIsBankerAndroidUser = pIBankerUserItem->IsAndroidUser();
		}
	}

	//180820�������
	for (WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
	{
		//ׯ���ж�
		if (m_wCurrentBanker == wChairID) continue;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem == NULL) continue;

		for (WORD wAreaIndex = 0; wAreaIndex < AREA_MAX; ++wAreaIndex)
		{

			if (cbWinArea[wAreaIndex] == TRUE)
			{

				if (wAreaIndex == AREA_XIAN || wAreaIndex == AREA_ZHUANG)
				{
					m_lUserWinScore[wChairID] += m_lPlayBet[wChairID][wAreaIndex];
					m_lPlayScore[wChairID][wAreaIndex] += m_lPlayBet[wChairID][wAreaIndex];
					lBankerWinScore -= m_lPlayBet[wChairID][wAreaIndex];
				}
				else 
				{
					m_lUserWinScore[wChairID] += (m_lPlayBet[wChairID][wAreaIndex] * (pingPoint - 1));
					m_lPlayScore[wChairID][wAreaIndex] += (m_lPlayBet[wChairID][wAreaIndex] * (pingPoint - 1));
					lBankerWinScore -= (m_lPlayBet[wChairID][wAreaIndex] * (pingPoint - 1));
				}
			}
			else
			{

				lUserLostScore[wChairID] -= m_lPlayBet[wChairID][wAreaIndex];
				m_lPlayScore[wChairID][wAreaIndex] -= m_lPlayBet[wChairID][wAreaIndex];
				lBankerWinScore += m_lPlayBet[wChairID][wAreaIndex];
			}
		}

		//�ܵķ���
		m_lUserWinScore[wChairID] += lUserLostScore[wChairID];
		//����˰��
		if (m_lUserWinScore[wChairID] > 0)
		{
			float fRevenuePer = float(m_pGameServiceOption->wRevenueRatio / 1000.0);
			m_lUserRevenue[wChairID] = SCORE(m_lUserWinScore[wChairID] * fRevenuePer);
			m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];
		}
	}

	return lSystemScore;
}

//��Ϸ��������
SCORE CTableFrameSink::GameOver()
{
	//�������
	SCORE lBankerWinScore = 0;
	bool bSuccess = false;

	//��Ϸ��¼
	tagServerGameRecord& GameRecord = m_GameRecordArrary[m_nRecordLast];

	tag_Redblack_Parameter Redblack_Parameter;
	ZeroMemory(&Redblack_Parameter, sizeof(Redblack_Parameter));

	Redblack_Parameter.wBankerUser = m_wCurrentBanker;
	Redblack_Parameter.cbJuControl = m_cbJuControl;
	CopyMemory(Redblack_Parameter.cbJuControlArea, m_cbJuControlArea, sizeof(Redblack_Parameter.cbJuControlArea));
	CopyMemory(Redblack_Parameter.lPlayBet, m_lPlayBet, sizeof(Redblack_Parameter.lPlayBet));

	tag_Redblack_Result pRedblack_Result;
	ZeroMemory(&pRedblack_Result, sizeof(pRedblack_Result));
	m_pITableFrame->GetControlResult(&Redblack_Parameter, sizeof(Redblack_Parameter), &pRedblack_Result);

	if (pRedblack_Result.cbCardCount[0] != 0 && pRedblack_Result.cbCardCount[1] != 0)
	{
		CopyMemory(m_cbTableCardArray, pRedblack_Result.cbTableCardArray, sizeof(m_cbTableCardArray));
		CopyMemory(m_cbCardCount, pRedblack_Result.cbCardCount, sizeof(m_cbCardCount));
	}
	else
		DispatchTableCard();

	if (m_cbJuControlTimes > 0)
		m_cbJuControlTimes--;
	if (m_cbJuControlTimes == 0)
	{
		m_cbJuControl = FALSE;
		ZeroMemory(m_cbJuControlArea, sizeof(m_cbJuControlArea));
	}

	CalculateScore(lBankerWinScore, GameRecord);

	//�ۼƻ���
	m_lBankerWinScore += lBankerWinScore;

	//��ǰ����
	m_lBankerCurGameScore = lBankerWinScore;

	//�ƶ��±�
	m_nRecordLast = (m_nRecordLast + 1) % MAX_SCORE_HISTORY;
	if (m_nRecordLast == m_nRecordFirst)
	{
		m_nRecordLast = m_nRecordFirst;
		ZeroMemory(m_GameRecordArrary, sizeof(m_GameRecordArrary));
	}

	return lBankerWinScore;
}

//�ƶ�Ӯ��
void CTableFrameSink::DeduceWinner(BYTE* pWinArea)
{
	//�����Ƶ�
	BYTE cbPlayerCount = m_GameLogic.GetCardType(m_cbTableCardArray[INDEX_PLAYER], m_cbCardCount[INDEX_PLAYER]);
	BYTE cbBankerCount = m_GameLogic.GetCardType(m_cbTableCardArray[INDEX_BANKER], m_cbCardCount[INDEX_BANKER]);

	BYTE cbWin = m_GameLogic.CompareCard(m_cbTableCardArray[INDEX_PLAYER], m_cbTableCardArray[INDEX_BANKER], MAX_COUNT);


	////////////////////////////////2018-3-26
	if (cbWin)
		pWinArea[AREA_XIAN] = TRUE;
	else
		pWinArea[AREA_ZHUANG] = TRUE;

	BYTE cbPointRed = m_GameLogic.GetCardType(m_cbTableCardArray[INDEX_PLAYER], MAX_COUNT);
	BYTE cbPointBlack = m_GameLogic.GetCardType(m_cbTableCardArray[INDEX_BANKER], MAX_COUNT);

	if (cbPointRed > 2 || cbPointBlack > 2)
		pWinArea[AREA_PING] = TRUE;
	////////////////////////////////2018-3-26
}

//���ͼ�¼
void CTableFrameSink::SendGameRecord(IServerUserItem *pIServerUserItem)
{
	WORD wBufferSize = 0;
	BYTE cbBuffer[SOCKET_TCP_BUFFER];
	int nIndex = m_nRecordFirst;
	while (nIndex != m_nRecordLast)
	{
		if ((wBufferSize + sizeof(tagServerGameRecord)) > sizeof(cbBuffer))
		{
			m_pITableFrame->SendUserItemData(pIServerUserItem, SUB_S_SEND_RECORD, cbBuffer, wBufferSize);
			wBufferSize = 0;
		}
		CopyMemory(cbBuffer + wBufferSize, &m_GameRecordArrary[nIndex], sizeof(tagServerGameRecord));
		wBufferSize += sizeof(tagServerGameRecord);

		nIndex = (nIndex + 1) % MAX_SCORE_HISTORY;
	}
	if (wBufferSize > 0) m_pITableFrame->SendUserItemData(pIServerUserItem, SUB_S_SEND_RECORD, cbBuffer, wBufferSize);
}

//��ȡ����
void CTableFrameSink::ReadConfigInformation()
{
	//��ȡ�Զ�������
	tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pGameServiceOption->cbCustomRule;
	ASSERT(pCustomConfig);

	//��ׯ
	m_lApplyBankerCondition = pCustomConfig->CustomGeneral.lApplyBankerCondition;
	m_nBankerTimeLimit = pCustomConfig->CustomGeneral.lBankerTime;
	m_nBankerTimeAdd = pCustomConfig->CustomGeneral.lBankerTimeAdd;
	m_lExtraBankerScore = pCustomConfig->CustomGeneral.lBankerScoreMAX;
	m_nExtraBankerTime = pCustomConfig->CustomGeneral.lBankerTimeExtra;
	//m_bEnableSysBanker = (pCustomConfig->CustomGeneral.nEnableSysBanker == TRUE)?true:false;
	m_bEnableSysBanker = (pCustomConfig->CustomGeneral.nEnableSysBanker == TRUE) ? true : true;

	//ʱ��	
	m_cbFreeTime = pCustomConfig->CustomGeneral.lFreeTime;
	m_cbBetTime = pCustomConfig->CustomGeneral.lBetTime;
	m_cbEndTime = pCustomConfig->CustomGeneral.lEndTime;
	if (m_cbFreeTime < TIME_FREE || m_cbFreeTime > 99) m_cbFreeTime = TIME_FREE;
	if (m_cbBetTime < TIME_PLACE_JETTON || m_cbBetTime > 99) m_cbBetTime = TIME_PLACE_JETTON;
	if (m_cbEndTime < TIME_GAME_END || m_cbEndTime > 99) m_cbEndTime = TIME_GAME_END;

	//��ע
	m_lAreaLimitScore = pCustomConfig->CustomGeneral.lAreaLimitScore;

	//������
	m_nRobotListMaxCount = pCustomConfig->CustomAndroid.lRobotListMaxCount;

	LONGLONG lRobotBetMinCount = pCustomConfig->CustomAndroid.lRobotBetMinCount;
	LONGLONG lRobotBetMaxCount = pCustomConfig->CustomAndroid.lRobotBetMaxCount;
	m_nMaxChipRobot = rand() % (lRobotBetMaxCount - lRobotBetMinCount + 1) + lRobotBetMinCount;
	if (m_nMaxChipRobot < 0)	m_nMaxChipRobot = 8;
	m_lRobotAreaLimit = pCustomConfig->CustomAndroid.lRobotAreaLimit;

	return;
}

//��ѯ�Ƿ�۷����
bool CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{
	// ׯ���ж�
	if (wChairID == m_wCurrentBanker)
	{
		for (int i = 0; i < GAME_PLAYER; ++i)
		{
			for (int j = 0; j < AREA_MAX; ++j)
			{
				if (m_lPlayBet[i][j] != 0)
					return true;
			}
		}
		return false;
	}

	// һ�����
	for (int i = 0; i < AREA_MAX; ++i)
	{
		if (m_lPlayBet[wChairID][i] != 0)
			return true;
	}

	return false;
}

//�����¼�
bool CTableFrameSink::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	return true;
}


/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661//////////