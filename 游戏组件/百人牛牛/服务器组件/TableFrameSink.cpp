#include "StdAfx.h"
#include "DlgCustomRule.h"
#include "TableFrameSink.h"
#include <locale>

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//��������
#define SEND_COUNT					100									//���ʹ���

//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//ʱ�䶨��
#define IDI_FREE					1									//����ʱ��
#define IDI_PLACE_JETTON			2									//��עʱ��
#define IDI_GAME_END				3									//����ʱ��

#define TIME_FREE					1									//����ʱ��
#define TIME_PLACE_JETTON			5									//��עʱ��
#define TIME_GAME_END				8									//����ʱ��
//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//��Ϸ����

//////////////////////////////////////////////////////////////////////////
//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��ʼ����
	ZeroMemory(m_lUserStartScore,sizeof(m_lUserStartScore));

	//����ע��
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//������ע
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	ZeroMemory(m_bUserListWin, sizeof(m_bUserListWin));
	ZeroMemory(m_lUserListScore, sizeof(m_lUserListScore));
	ZeroMemory(m_cbUserPlayCount, sizeof(m_cbUserPlayCount));

	//���Ʊ���
	m_cbWinSideControl=0;
	m_nSendCardCount=0;

	m_cbJuControl = FALSE;
	m_cbJuControlTimes = 0;
	ZeroMemory(m_cbJuControlArea, sizeof(m_cbJuControlArea));

	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//�˿���Ϣ
	ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

	//״̬����
	m_dwJettonTime=0L;
	m_bExchangeBanker=true;
	m_wAddTime=0;

	//ׯ����Ϣ
	m_ApplyUserArray.RemoveAll();
	m_wCurrentBanker=INVALID_CHAIR;
	m_wOfflineBanker = INVALID_CHAIR;
	m_wBankerTime=0;
	m_lPlayerBankerMAX = 0l;
	m_lBankerWinScore=0L;		
	m_lBankerCurGameScore=0L;
	m_bEnableSysBanker=true;
	m_cbLeftCardCount=0;
	m_bContiueCard=false;
	m_lBankerScore = 0l;

	//��¼����
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;
	m_nRecordLast=0;
	m_dwRecordCount=0;

	//�����˿���
	m_nChipRobotCount = 0;
	m_nRobotListMaxCount =0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));
	
	return;
}

//��������
CTableFrameSink::~CTableFrameSink()
{
}

VOID CTableFrameSink::Release()
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

//��������
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);

	//�����ж�
	if (m_pITableFrame==NULL)
	{
		CTraceService::TraceString(TEXT("��Ϸ���� CTableFrameSink ��ѯ ITableFrame �ӿ�ʧ��"),TraceLevel_Exception);
		return false;
	}

	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_TIME_CONTROL);

	m_pGameServiceOption = m_pITableFrame->GetGameServiceOption();
	m_pGameServiceAttrib = m_pITableFrame->GetGameServiceAttrib();

	ASSERT(m_pGameServiceOption!=NULL);
	ASSERT(m_pGameServiceAttrib!=NULL);
	//�����ļ���
	TCHAR szPath[MAX_PATH] = TEXT("");
	GetCurrentDirectory(sizeof(szPath), szPath);
	_sntprintf(m_szConfigFileName, sizeof(m_szConfigFileName), TEXT("%s\\BaiRenConfig\\OxBattleConfig-%d-%d.ini"), szPath, m_pGameServiceOption->wServerID, m_pITableFrame->GetTableID());

	m_nChip[0] = GetPrivateProfileInt(TEXT("����ţţ"), TEXT("nChip0"), 1, m_szConfigFileName);
	m_nChip[1] = GetPrivateProfileInt(TEXT("����ţţ"), TEXT("nChip1"), 10, m_szConfigFileName);
	m_nChip[2] = GetPrivateProfileInt(TEXT("����ţţ"), TEXT("nChip2"), 50, m_szConfigFileName);
	m_nChip[3] = GetPrivateProfileInt(TEXT("����ţţ"), TEXT("nChip3"), 100, m_szConfigFileName);
	m_nChip[4] = GetPrivateProfileInt(TEXT("����ţţ"), TEXT("nChip4"), 1000, m_szConfigFileName);

	m_lUserLimitScore = GetPrivateProfileInt(TEXT("����ţţ"), TEXT("nLimitMax"), 20000, m_szConfigFileName);
	m_nMaxTimes = GetPrivateProfileInt(TEXT("����ţţ"), TEXT("nMaxTimes"), 5, m_szConfigFileName);

	//��������
	memcpy(m_szRoomName, m_pGameServiceOption->szServerName, sizeof(m_pGameServiceOption->szServerName));

	ReadConfigInformation();
	return true;
}

//��λ����
VOID CTableFrameSink::RepositionSink()
{
	m_wOfflineBanker = INVALID_CHAIR;

	//����ע��
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//������ע
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//�����˿���
	m_nChipRobotCount = 0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));
	
	m_lBankerScore = 0l;

	return;
}

//��ѯ�޶�
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	if(pIServerUserItem->GetUserStatus() == US_PLAYING)
	{
		return 0L;
	}
	else
	{
		return __max(pIServerUserItem->GetUserScore()-m_pGameServiceOption->lMinTableScore, 0L);
	}
}

//�����¼�
bool CTableFrameSink::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	//��ׯ�ҷ����ڿ���ʱ��䶯ʱ(��ׯ�ҽ����˴�ȡ��)У��ׯ�ҵ���ׯ����
	if(wChairID == m_wCurrentBanker && m_pITableFrame->GetGameStatus()==GAME_SCENE_FREE)
	{
		ChangeBanker(false);
	}
	return true;
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{
	ZeroMemory(m_cbTableCardArray, sizeof(m_cbTableCardArray));

	ZeroMemory(m_cbOpenResult, sizeof(m_cbOpenResult));
	m_cbEarlyOpen = FALSE;

	DispatchTableCard();

	//��ȡׯ��
	IServerUserItem* pIBankerServerUserItem = NULL;
	if ( m_wCurrentBanker == INVALID_CHAIR )
	{
		m_lBankerScore = 10000000;
	}
	else
	{
		IServerUserItem* pIBankerServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if ( pIBankerServerUserItem != NULL )
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
	GameStart.cbTimeLeave=m_nPlaceJettonTime;

	FillMemory(GameStart.wSeatUser, sizeof(GameStart.wSeatUser), INVALID_CHAIR);

	OnGetUserListGameID(GameStart.wSeatUser);

	//�Թ����
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	
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
			tag_OxBattle_Parameter OxBattle_Parameter;
			ZeroMemory(&OxBattle_Parameter, sizeof(OxBattle_Parameter));

			OxBattle_Parameter.wBankerUser = m_wCurrentBanker;
			OxBattle_Parameter.cbJuControl = m_cbJuControl;
			CopyMemory(OxBattle_Parameter.cbJuControlArea, m_cbJuControlArea, sizeof(OxBattle_Parameter.cbJuControlArea));
			CopyMemory(OxBattle_Parameter.lPlayBet, m_lUserJettonScore, sizeof(OxBattle_Parameter.lPlayBet));
			CopyMemory(OxBattle_Parameter.cbTableCardArray, m_cbTableCardArray, sizeof(OxBattle_Parameter.cbTableCardArray));

			tag_OxBattle_Result pOxBattle_Result;
			ZeroMemory(&pOxBattle_Result, sizeof(pOxBattle_Result));
			m_pITableFrame->GetControlResult(&OxBattle_Parameter, sizeof(OxBattle_Parameter), &pOxBattle_Result);

			if (pOxBattle_Result.cbTableCardArray[0][2] != 0 && pOxBattle_Result.cbTableCardArray[1][2] != 0)
			{
				CopyMemory(m_cbTableCardArray, pOxBattle_Result.cbTableCardArray, sizeof(m_cbTableCardArray));
				m_cbCardCount[0] = 5;
				m_cbCardCount[1] = 5;
				m_cbCardCount[2] = 5;
				m_cbCardCount[3] = 5;
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

			//�������
			SCORE lBankerWinScore = CalculateScore();

			//��������
			m_wBankerTime++;

			//������Ϣ
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

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
				for (BYTE j = 1; j < AREA_MAX + 1; j++)
				{
					lAddScore += m_lUserJettonScore[i][j];
				}
				m_lUserListScore[i][m_cbUserPlayCount[i]] = lAddScore;
				m_cbUserPlayCount[i] ++;

				if (m_cbUserPlayCount[i] == USER_LIST_COUNT)
					m_cbUserPlayCount[i] = 0;
			}

			//ռλ��ҳɼ�
			WORD wSeatUser[MAX_SEAT_COUNT];
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
				GameEnd.lBankerRestScore = 10000000;
			else
			{
				IServerUserItem* pBankerUser = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				if (pBankerUser)
					GameEnd.lBankerRestScore = pBankerUser->GetUserScore() + lBankerWinScore;
			}

			//�˿���Ϣ
			CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
			for (int i = 0; i < AREA_MAX + 1; i++)
			{
				GameEnd.cbCardType[i] = m_GameLogic.GetCardType(GameEnd.cbTableCardArray[i], MAX_CARD);
			}
			CopyMemory(GameEnd.cbResult, m_cbOpenResult, sizeof(GameEnd.cbResult));
			GameEnd.cbEarlyOpenFlag = m_cbEarlyOpen;

			for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
				if ( pIServerUserItem == NULL ) continue;

				//���óɼ�
				CopyMemory(GameEnd.lPlayAreaScore, &m_lUserJettonScore[wUserIndex][1], sizeof(GameEnd.lPlayAreaScore));
				GameEnd.lPlayerWinScore = m_lUserWinScore[wUserIndex];
				GameEnd.lPlayerRestScore = pIServerUserItem->GetUserScore() + GameEnd.lPlayerWinScore;

				//������Ϣ					
				m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				//�����
				if (!pIServerUserItem->IsAndroidUser())
					m_pGameServiceOption->pRoomStorageOption.lStorageCurrent -= m_lUserWinScore[wUserIndex];
			}
			
			tagOxPerGameRecord GameRecord;
			GameRecord.wTableID = m_pITableFrame->GetTableID() + 1;
			CopyMemory(GameRecord.bAreaWin, m_cbOpenResult, sizeof(GameRecord.bAreaWin));
			m_pITableFrame->SendTableDataToLookOnUser(SUB_GF_BJL_GAME_RESULT, &GameRecord, sizeof(GameRecord));

			return true;
		}
	case GER_USER_LEAVE:		//�û��뿪
	case GER_NETWORK_ERROR:
		{
			return true;
			//�м��ж�
			if (m_wCurrentBanker!=wChairID)
			{
				//��������
				SCORE lRevenue = 0;

				//�Ƿ���
				if (m_pITableFrame->GetGameStatus() == GAME_SCENE_BET)
				{
					//������ע
					for (int nAreaIndex = 1; nAreaIndex <= AREA_MAX; ++nAreaIndex)
					{
						if (m_lUserJettonScore[wChairID][nAreaIndex] != 0)
						{
							m_lAllJettonScore[nAreaIndex] -= m_lUserJettonScore[wChairID][nAreaIndex];
							m_lUserJettonScore[wChairID][nAreaIndex] = 0;
						}
					}
				}
				else
				{
					//д�����
					if (m_lUserWinScore[wChairID]!=0L) 
					{
						tagScoreInfo ScoreInfo[GAME_PLAYER];
						ZeroMemory(ScoreInfo,sizeof(ScoreInfo));

						//�����
						if (!pIServerUserItem->IsAndroidUser())
							m_pGameServiceOption->pRoomStorageOption.lStorageCurrent -= m_lUserWinScore[wChairID];
					
						ScoreInfo[wChairID].lScore = m_lUserWinScore[wChairID];
						ScoreInfo[wChairID].cbType=m_lUserWinScore[wChairID]>0?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
						ScoreInfo[wChairID].lRevenue = m_lUserRevenue[wChairID];
						m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));
						m_lUserWinScore[wChairID] = 0;
					}
					//�����ע
					for (int nAreaIndex = 1; nAreaIndex <= AREA_MAX; nAreaIndex++)
					{
						m_lUserJettonScore[wChairID][nAreaIndex] = 0;
					}
				}

				return true;
			}

			//״̬�ж�
			if (m_pITableFrame->GetGameStatus() != GAME_SCENE_END)
			{
				//����״̬
				m_pITableFrame->SetGameStatus(GAME_SCENE_END);

				//����ʱ��
				m_dwJettonTime=(DWORD)time(NULL);
				m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_nGameEndTime*1000,1,0L);

				//�������
				SCORE lBankerWinScore = CalculateScore();

				//������Ϣ
				CMD_S_GameEnd GameEnd;
				ZeroMemory(&GameEnd,sizeof(GameEnd));

				//ׯ����Ϣ
				GameEnd.lBankerWinScore = lBankerWinScore;

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

				if (m_wCurrentBanker == INVALID_CHAIR)
					GameEnd.lBankerRestScore = 100000000;
				else
				{
					IServerUserItem* pBankerUser = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
					if (pBankerUser)
						GameEnd.lBankerRestScore = pBankerUser->GetUserScore() + lBankerWinScore;
				}

				//�˿���Ϣ
				CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
				for (int i = 0; i < AREA_MAX + 1; i++)
				{
					GameEnd.cbCardType[i] = m_GameLogic.GetCardType(GameEnd.cbTableCardArray[i], MAX_CARD);
				}
				CopyMemory(GameEnd.cbResult, m_cbOpenResult, sizeof(GameEnd.cbResult));
				GameEnd.cbEarlyOpenFlag = m_cbEarlyOpen;

				for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
				{
					IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
					if ( pIServerUserItem == NULL ) continue;

					CopyMemory(GameEnd.lPlayAreaScore, m_lUserJettonScore[wUserIndex], sizeof(GameEnd.lPlayAreaScore));
					GameEnd.lPlayerWinScore = m_lUserWinScore[wUserIndex];
					GameEnd.lPlayerRestScore = pIServerUserItem->GetUserScore() + GameEnd.lPlayerWinScore;

					//������Ϣ					
					m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

					//�����
					if (!pIServerUserItem->IsAndroidUser())
						m_pGameServiceOption->pRoomStorageOption.lStorageCurrent -= m_lUserWinScore[wUserIndex];
				}
			}

			//�۳�����
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
			ScoreInfo[m_wCurrentBanker].lScore =m_lUserWinScore[m_wCurrentBanker];
			ScoreInfo[m_wCurrentBanker].lRevenue = m_lUserRevenue[m_wCurrentBanker];
			ScoreInfo[m_wCurrentBanker].cbType = (m_lUserWinScore[m_wCurrentBanker]>0?SCORE_TYPE_WIN:SCORE_TYPE_LOSE);
			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

			if (m_wCurrentBanker != INVALID_CHAIR)
			{
				IServerUserItem *pBankerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				if (pBankerUserItem && !pBankerUserItem->IsAndroidUser())
				{
					m_pGameServiceOption->pRoomStorageOption.lStorageCurrent -= m_lUserWinScore[m_wCurrentBanker];
				}
			}

			m_lUserWinScore[m_wCurrentBanker] = 0;

			//�л�ׯ��
			ChangeBanker(true);

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
	case GAME_SCENE_FREE:			//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));			
			FillMemory(StatusFree.wApplyUser, sizeof(StatusFree.wApplyUser), INVALID_CHAIR);
			FillMemory(StatusFree.wSeatUser, sizeof(StatusFree.wSeatUser), INVALID_CHAIR);

			StatusFree.cbHighTimes = m_nMaxTimes;

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusFree.cbTimeLeave=(BYTE)(m_nFreeTime-__min(dwPassTime,(DWORD)m_nFreeTime));
			
			//��ȡׯ��
			IServerUserItem* pIBankerServerUserItem = NULL;
			if (m_wCurrentBanker == INVALID_CHAIR)
			{
				m_lBankerScore = 10000000;
			}
			else
			{
				IServerUserItem* pIBankerServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				if (pIBankerServerUserItem != NULL)
				{
					m_lBankerScore = pIBankerServerUserItem->GetUserScore();

				}
			}
			//ׯ����Ϣ
			StatusFree.wBankerUser = m_wCurrentBanker;
			StatusFree.lBankerScore = m_lBankerScore;
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
			StatusFree.lAreaLimitScore = m_lAreaLimitScore;

			StatusFree.cbFreeTime = m_nFreeTime;
			StatusFree.cbPlayTime = m_nPlaceJettonTime;
			StatusFree.cbEndTime = m_nGameEndTime;

			StatusFree.lMinXianHong = m_nChip[0];
			StatusFree.lMaxXianHong = m_lUserLimitScore;

			CopyMemory(StatusFree.nChip, m_nChip, sizeof(StatusFree.nChip));

			for (INT_PTR nUserIdx = 0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
			{
				WORD wChairID = m_ApplyUserArray[nUserIdx];

				//��ȡ���
				IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
				if (!pServerUserItem) continue;

				//ׯ���ж�
				if (pServerUserItem->GetChairID() == m_wCurrentBanker) continue;

				StatusFree.wApplyUser[nUserIdx] = wChairID;
			}

			OnGetUserListGameID(StatusFree.wSeatUser);

			//���ͳ���
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

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
	case GAME_SCENE_BET:				//��Ϸ״̬
	case GAME_SCENE_END:				//����״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay, sizeof(StatusPlay));

			FillMemory(StatusPlay.wApplyUser, sizeof(StatusPlay.wApplyUser), INVALID_CHAIR);
			FillMemory(StatusPlay.wSeatUser, sizeof(StatusPlay.wSeatUser), INVALID_CHAIR);

			StatusPlay.cbHighTimes = m_nMaxTimes;

			//ȫ����Ϣ
			DWORD dwPassTime = (DWORD)time(NULL) - m_dwJettonTime;
			int	nTotalTime = (cbGameStatus == GAME_SCENE_BET ? m_nPlaceJettonTime : m_nGameEndTime);
			StatusPlay.cbTimeLeave = (BYTE)(nTotalTime - __min(dwPassTime, (DWORD)nTotalTime));

			StatusPlay.lBankerWinScore = m_lBankerWinScore;

			//��ȡׯ��
			IServerUserItem* pIBankerServerUserItem = NULL;
			if (m_wCurrentBanker == INVALID_CHAIR)
			{
				m_lBankerScore = 10000000;
			}
			else
			{
				IServerUserItem* pIBankerServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				if (pIBankerServerUserItem != NULL)
				{
					m_lBankerScore = pIBankerServerUserItem->GetUserScore();

				}
			}
			//ׯ����Ϣ
			StatusPlay.wBankerUser = m_wCurrentBanker;
			StatusPlay.lBankerScore = m_lBankerScore;

			//�����ж�
			if (cbGameStatus == GAME_SCENE_END)
			{
				//�˿���Ϣ
				CopyMemory(StatusPlay.cbTableCardArray, m_cbTableCardArray, sizeof(m_cbTableCardArray));

				StatusPlay.lPlayerWinScore = m_lUserWinScore[pIServerUserItem->GetChairID()];
				for (int i = 0; i < AREA_MAX + 1; i++)
				{
					StatusPlay.cbCardType[i] = m_GameLogic.GetCardType(StatusPlay.cbTableCardArray[i],MAX_CARD);
				}
				CopyMemory(StatusPlay.cbResult, m_cbOpenResult, sizeof(StatusPlay.cbResult));

				//ռλ��ҳɼ�
				WORD wSeatUser[MAX_SEAT_COUNT];
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

			StatusPlay.lBankerScore = m_lBankerScore;
			StatusPlay.lApplyBankerCondition = m_lApplyBankerCondition;
			StatusPlay.lAreaLimitScore = m_lAreaLimitScore;

			StatusPlay.cbFreeTime = m_nFreeTime;
			StatusPlay.cbPlayTime = m_nPlaceJettonTime;
			StatusPlay.cbEndTime = m_nGameEndTime;

			StatusPlay.lMinXianHong = m_nChip[0];
			StatusPlay.lMaxXianHong = m_lUserLimitScore;

			CopyMemory(StatusPlay.lPlayerJettonScore, m_lUserJettonScore[wChairID], sizeof(StatusPlay.lPlayerJettonScore));
			CopyMemory(StatusPlay.lAllJettonScore, m_lAllJettonScore, sizeof(StatusPlay.lAllJettonScore));

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

			//���ͳ���
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

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
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_FREE:		//����ʱ��
		{
			//��ʼ��Ϸ
			m_pITableFrame->StartGame();

			//����ʱ��
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,m_nPlaceJettonTime*1000,1,0L);

			//����״̬
			m_pITableFrame->SetGameStatus(GAME_SCENE_BET);

			//���˥��
			m_pITableFrame->CalculateStorageDeduct();

			tagChangeStatus ChangeStatus;
			ChangeStatus.wTableID = m_pITableFrame->GetTableID() + 1;
			ChangeStatus.cbGameStatus = m_pITableFrame->GetGameStatus();
			m_pITableFrame->SendTableDataToLookOnUser(SUB_GF_BJL_CHANGE_STATUS, &ChangeStatus, sizeof(ChangeStatus));

			return true;
		}
	case IDI_PLACE_JETTON:		//��עʱ��
		{
			//״̬�ж�(��ֹǿ���ظ�����)
			if (m_pITableFrame->GetGameStatus() != GAME_SCENE_END)
			{
				//����״̬
				m_pITableFrame->SetGameStatus(GAME_SCENE_END);

				DispatchTableCard();

				//������Ϸ
				OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);

				//����ʱ��
				m_dwJettonTime=(DWORD)time(NULL);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_nGameEndTime*1000,1,0L);	

				tagChangeStatus ChangeStatus;
				ChangeStatus.wTableID = m_pITableFrame->GetTableID() + 1;
				ChangeStatus.cbGameStatus = m_pITableFrame->GetGameStatus();
				m_pITableFrame->SendTableDataToLookOnUser(SUB_GF_BJL_CHANGE_STATUS, &ChangeStatus, sizeof(ChangeStatus));
			}

			return true;
		}
	case IDI_GAME_END:			//������Ϸ
		{
			//д�����
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
			for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);				
				if ( pIServerUserItem == NULL||(QueryBuckleServiceCharge(wUserChairID)==false)) continue;				
				
				//ʤ������
				BYTE ScoreKind=(m_lUserWinScore[wUserChairID]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
				ScoreInfo[wUserChairID].lScore = m_lUserWinScore[wUserChairID];
				ScoreInfo[wUserChairID].lRevenue = m_lUserRevenue[wUserChairID];
				ScoreInfo[wUserChairID].cbType = ScoreKind;

			}

			//д�����
			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

			//����ʱ��
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE,m_nFreeTime*1000,1,0L);

			//������Ϣ
			
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_FREE);
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_FREE);
			
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
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);
			
			//�л�ׯ��
			ChangeBanker(false);

			tagChangeStatus ChangeStatus;
			ChangeStatus.wTableID = m_pITableFrame->GetTableID() + 1;
			ChangeStatus.cbGameStatus = m_pITableFrame->GetGameStatus();
			m_pITableFrame->SendTableDataToLookOnUser(SUB_GF_BJL_CHANGE_STATUS, &ChangeStatus, sizeof(ChangeStatus));

			return true;
		}
	}

	return false;
}

//������Ϸ��ȡ��Ϸ��¼
void CTableFrameSink::OnGetGameRecord(VOID *GameRecord)
{
	CMD_GF_OxRoomStatus *pRoomStatus = (CMD_GF_OxRoomStatus *)GameRecord;

	pRoomStatus->tagGameInfo.wTableID = m_pITableFrame->GetTableID() + 1;
	pRoomStatus->tagGameInfo.cbGameStatus = m_pITableFrame->GetGameStatus();

	pRoomStatus->tagTimeInfo.cbFreeTime = m_nFreeTime;
	pRoomStatus->tagTimeInfo.cbBetTime = m_nPlaceJettonTime;
	pRoomStatus->tagTimeInfo.cbEndTime = m_nGameEndTime;
	pRoomStatus->tagTimeInfo.cbPassTime = (DWORD)time(NULL) - m_dwJettonTime;
	pRoomStatus->tagTimeInfo.lMinXianHong = m_nChip[0];
	pRoomStatus->tagTimeInfo.lMaxXianHong = m_lUserLimitScore;

	//�ͻ���ֻ��ʾ20��
	int nIndex = m_nRecordFirst;

	pRoomStatus->cbRecordCount = m_nRecordLast - m_nRecordFirst;
	if (pRoomStatus->cbRecordCount > 20)
	{
		pRoomStatus->cbRecordCount = 20;
		nIndex = m_nRecordLast - 20;
	}

	if (m_nRecordLast < m_nRecordFirst)
	{
		nIndex = (m_nRecordLast + MAX_SCORE_HISTORY - 20) % MAX_SCORE_HISTORY;
		pRoomStatus->cbRecordCount = 20;
	}

	pRoomStatus->cbMultiple = m_nMaxTimes;

	int nArrayIndex = 0;

	while (nIndex != m_nRecordLast)
	{
		if (nArrayIndex >= 20)
			break;
		pRoomStatus->GameRecordArrary[nArrayIndex].bAreaWin[0] = m_GameRecordArrary[nIndex].bWinShunMen;
		pRoomStatus->GameRecordArrary[nArrayIndex].bAreaWin[1] = m_GameRecordArrary[nIndex].bWinDuiMen;
		pRoomStatus->GameRecordArrary[nArrayIndex].bAreaWin[2] = m_GameRecordArrary[nIndex].bWinDaoMen;
		pRoomStatus->GameRecordArrary[nArrayIndex].bAreaWin[3] = m_GameRecordArrary[nIndex].bWinHuang;

		nArrayIndex++;
		nIndex = (nIndex + 1) % MAX_SCORE_HISTORY;
	}

}

//��ȡ������Ϸ�Ƿ���ע״̬���������ע��
bool CTableFrameSink::OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if (m_pITableFrame->GetGameStatus() == GAME_SCENE_FREE)
		return false;

	for (WORD wAreaIndex = 1; wAreaIndex < AREA_MAX+1; ++wAreaIndex)
	{
		if (m_lUserJettonScore[wChairID][wAreaIndex] != 0)
		{
			return true;
		}
	}

	if (wChairID == m_wCurrentBanker)
		return true;

	return false;
}

//��Ϸ��Ϣ
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_PLACE_JETTON:		//�û���ע
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_PlaceJetton));
			if (wDataSize!=sizeof(CMD_C_PlaceJetton)) return false;

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//��Ϣ����
			CMD_C_PlaceJetton * pPlaceJetton=(CMD_C_PlaceJetton *)pData;
			return OnUserPlaceJetton(pUserData->wChairID,pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);
		}
	case SUB_C_APPLY_BANKER:		//������ׯ
		{
			return OnUserApplyBanker(pIServerUserItem);	
		}
	case SUB_C_CANCEL_BANKER:		//ȡ����ׯ
		{
			return OnUserCancelBanker(pIServerUserItem);	
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
					CopyMemory(ControlInfo.lUserJettonScore[cbIndex], m_lUserJettonScore[i], sizeof(ControlInfo.lUserJettonScore[cbIndex]));
					lstrcpyn(ControlInfo.szNickName[cbIndex], pServerUser->GetNickName(), sizeof(ControlInfo.szNickName[cbIndex]));

					for (BYTE j = 0; j < AREA_MAX+1; j++)
					{
						lUserBetAllScore[cbIndex] += m_lUserJettonScore[i][j];
						ControlInfo.lAreaJettonScore[j] += m_lUserJettonScore[i][j];
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

//�����Ϣ
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

bool CTableFrameSink::OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem)
{
	//����ׯ��
	if (wChairID == m_wCurrentBanker && pIServerUserItem->IsAndroidUser() == false)
	{
		m_wOfflineBanker = wChairID;
	}

	if(pIServerUserItem == NULL) return false;
	//�л�ׯ��
	if (wChairID==m_wCurrentBanker) ChangeBanker(true);

	//ȡ������
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		if (wChairID!=m_ApplyUserArray[i]) continue;

		//ɾ�����
		m_ApplyUserArray.RemoveAt(i);

		//�������
		CMD_S_CancelBanker CancelBanker;
		ZeroMemory(&CancelBanker,sizeof(CancelBanker));

		//���ñ���
		CancelBanker.wCancelUser=wChairID;

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

		break;
	}

	return true;

}
//�û�����
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��ʼ����
	m_lUserStartScore[wChairID] = pIServerUserItem->GetUserScore();

	//����ʱ��
	if ((bLookonUser==false)&&(m_dwJettonTime==0L))
	{
		m_dwJettonTime=(DWORD)time(NULL);
		m_pITableFrame->SetGameTimer(IDI_FREE,m_nFreeTime*1000,1,NULL);
		m_pITableFrame->SetGameStatus(GAME_SCENE_FREE);
	}
	

	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	ASSERT(pIServerUserItem !=NULL);

	//��ʼ����
	m_lUserStartScore[wChairID] = 0;
	
	ZeroMemory(m_bUserListWin[wChairID], sizeof(m_bUserListWin[wChairID]));
	ZeroMemory(m_lUserListScore[wChairID], sizeof(m_lUserListScore[wChairID]));
	m_cbUserPlayCount[wChairID] = 0;

	//��¼�ɼ�
	if (bLookonUser==false)
	{
		//�л�ׯ��
		if (wChairID==m_wCurrentBanker)
		{
			ChangeBanker(true);
			m_bContiueCard=false;
		}

		//ȡ������
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			if (wChairID!=m_ApplyUserArray[i]) continue;

			//ɾ�����
			m_ApplyUserArray.RemoveAt(i);

			//�������
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//���ñ���
			CancelBanker.wCancelUser=wChairID;

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

			break;
		}
	}

	return true;
}


//��ע�¼�
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, SCORE lJettonScore)
{
	BYTE cbServerBetArea = cbJettonArea + 1;

	//Ч�����
	ASSERT((cbServerBetArea<=ID_HUANG_MEN && cbServerBetArea>=ID_TIAN_MEN)&&(lJettonScore>0L));
	if ((cbServerBetArea>ID_HUANG_MEN)||(lJettonScore<=0L) || cbServerBetArea<ID_TIAN_MEN)
	{
		CMD_S_PlaceBetFail PlaceBetFail;
		_sntprintf(PlaceBetFail.szBuffer, sizeof(PlaceBetFail.szBuffer), TEXT("��ע��Ϣ����"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceBetFail, sizeof(PlaceBetFail));
		return true;
	}

	if (m_pITableFrame->GetGameStatus() != GAME_SCENE_BET)
	{
		CMD_S_PlaceBetFail PlaceBetFail;
		_sntprintf(PlaceBetFail.szBuffer, sizeof(PlaceBetFail.szBuffer), TEXT("��ǰ������ע״̬��������ע��"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceBetFail, sizeof(PlaceBetFail));
		return true;
	}
	//ׯ���ж�
	if (m_wCurrentBanker==wChairID)
	{
		CMD_S_PlaceBetFail PlaceBetFail;
		_sntprintf(PlaceBetFail.szBuffer, sizeof(PlaceBetFail.szBuffer), TEXT("ׯ�Ҳ�����ע��"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceBetFail, sizeof(PlaceBetFail));
		return true;
	}
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR) 
	{
		CMD_S_PlaceBetFail PlaceBetFail;
		_sntprintf(PlaceBetFail.szBuffer, sizeof(PlaceBetFail.szBuffer), TEXT("�÷���ϵͳ��ׯ������ע��"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceBetFail, sizeof(PlaceBetFail));
		return true;
	}

	//ׯ�һ���
	SCORE lBankerScore = 0;
	if(INVALID_CHAIR != m_wCurrentBanker)
	{
		IServerUserItem * pIServerUserItemBanker = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		lBankerScore = pIServerUserItemBanker->GetUserScore();
	}

	//��������
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	SCORE lJettonCount = 0L;
	for (int nAreaIndex = 1; nAreaIndex <= AREA_MAX; ++nAreaIndex) lJettonCount += m_lUserJettonScore[wChairID][nAreaIndex];
	SCORE lAllJettonCount = 0L;
	for (int nAreaIndex = 1; nAreaIndex <= AREA_MAX; ++nAreaIndex) lAllJettonCount += m_lAllJettonScore[nAreaIndex];

	//��һ���
	SCORE lUserScore = pIServerUserItem->GetUserScore();

	//�Ϸ�У��
	if (lUserScore < lJettonCount + lJettonScore)
	{
		CMD_S_PlaceBetFail PlaceBetFail;
		_sntprintf(PlaceBetFail.szBuffer, sizeof(PlaceBetFail.szBuffer), TEXT("���Ľ�Ҳ��㣡"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceBetFail, sizeof(PlaceBetFail));
		return true;
	}
	if (m_lUserLimitScore < lJettonCount + lJettonScore)
	{
		CMD_S_PlaceBetFail PlaceBetFail;
		_sntprintf(PlaceBetFail.szBuffer, sizeof(PlaceBetFail.szBuffer), TEXT("������ע�ѵ������ƣ�"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceBetFail, sizeof(PlaceBetFail));
		return true;
	}

	//�ɹ���ʶ
	bool bPlaceJettonSuccess=true;

	//�Ϸ���֤
	if (GetUserMaxJetton(wChairID, cbServerBetArea) >= lJettonScore)
	{
		//��������֤
		if(pIServerUserItem->IsAndroidUser())
		{
			//��������
			if (m_lRobotAreaScore[cbServerBetArea] + lJettonScore > m_lRobotAreaLimit)
				return true;

			//��Ŀ����
			bool bHaveChip = false;
			for (int i = 0; i <= AREA_MAX; i++)
			{
				if (m_lUserJettonScore[wChairID][i + 1] != 0)
					bHaveChip = true;
			}

			//ͳ�Ʒ���
			if (bPlaceJettonSuccess)
				m_lRobotAreaScore[cbServerBetArea] += lJettonScore;
		}

		if (bPlaceJettonSuccess)
		{
			//������ע
			m_lAllJettonScore[cbServerBetArea] += lJettonScore;
			m_lUserJettonScore[wChairID][cbServerBetArea] += lJettonScore;
		}
	}
	else
	{
		bPlaceJettonSuccess=false;
	}

	if (bPlaceJettonSuccess)
	{
		//��������
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

		//�������
		PlaceJetton.wChairID=wChairID;
		PlaceJetton.cbBetArea = cbJettonArea;
		PlaceJetton.lBetScore = lJettonScore;

		PlaceJetton.lPlayerRestScore = pIServerUserItem->GetUserScore() - (lJettonCount + lJettonScore);


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
				m_pITableFrame->SendTableData(i, SUB_S_SEAT_JETTON, &PlaceJetton, sizeof(PlaceJetton));
			else if (i == wChairID)
				m_pITableFrame->SendTableData(i, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
			else
				m_pITableFrame->SendTableData(i, SUB_S_OTHER_JETTON, &PlaceJetton, sizeof(PlaceJetton));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
		
		//��ע�ܶ��ׯ����֧������ʱ��ǰ������Ϸ
		SCORE lBankerMaxScore = 100000000;
		if (INVALID_CHAIR != m_wCurrentBanker)
		{
			lBankerMaxScore = lBankerScore;
		}

		if (lBankerMaxScore <= (lAllJettonCount + lJettonScore + 100) * m_nMaxTimes)
		{
			if (m_pITableFrame->GetGameStatus() != GAME_SCENE_END)
			{
				//����״̬
				m_pITableFrame->SetGameStatus(GAME_SCENE_END);

				DispatchTableCard();

				m_cbEarlyOpen = TRUE;

				//������Ϸ
				OnEventGameConclude(INVALID_CHAIR, NULL, GER_NORMAL);

				//����ʱ��
				m_dwJettonTime = (DWORD)time(NULL);
				m_pITableFrame->SetGameTimer(IDI_GAME_END, m_nGameEndTime * 1000, 1, 0L);
			}
		}
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
	//����ϴ��
	ZeroMemory(m_cbTableCard, sizeof(m_cbTableCard));

	BYTE cbCardData[CountArray(m_GameLogic.m_cbCardListData)];
	CopyMemory(cbCardData, m_GameLogic.m_cbCardListData, sizeof(m_GameLogic.m_cbCardListData));

	//�����˿�
	BYTE cbRandCount = 0, cbPosition = 0;
	do
	{
		cbPosition = rand() % (CountArray(cbCardData) - cbRandCount);
		m_cbTableCard[cbRandCount++] = cbCardData[cbPosition];
		cbCardData[cbPosition] = cbCardData[CountArray(cbCardData) - cbRandCount];
	} while (cbRandCount < CountArray(m_cbTableCard));
	
	for (WORD i = 0; i < 5; i++)
		CopyMemory(m_cbTableCardArray[i], &m_cbTableCard[i * 5], sizeof(BYTE) * 5);

	//������Ŀ
	for (int i=0; i<CountArray(m_cbCardCount); ++i) m_cbCardCount[i]=5;

	return true;
}

//����ׯ��
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	//�Ϸ��ж�
	SCORE lUserScore = pIApplyServerUserItem->GetUserScore();
	if (lUserScore<m_lApplyBankerCondition)
	{
		CMD_S_ApplyFailed ApplyFailed;
		_sntprintf(ApplyFailed.szBuffer, sizeof(ApplyFailed.szBuffer), TEXT("��Ľ�Ҳ���������ׯ�ң�����ʧ�ܣ�"));
		m_pITableFrame->SendTableData(pIApplyServerUserItem->GetChairID(), SUB_S_APPLEY_BANKER_FAILURE, &ApplyFailed, sizeof(ApplyFailed));
		return true;
	}

	//�����ж�
	WORD wApplyUserChairID=pIApplyServerUserItem->GetChairID();
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];
		if (wChairID==wApplyUserChairID)
		{
			CMD_S_ApplyFailed ApplyFailed;
			_sntprintf(ApplyFailed.szBuffer, sizeof(ApplyFailed.szBuffer), TEXT("���Ѿ�������ׯ�ң�����Ҫ�ٴ����룡"));
			m_pITableFrame->SendTableData(pIApplyServerUserItem->GetChairID(), SUB_S_APPLEY_BANKER_FAILURE, &ApplyFailed, sizeof(ApplyFailed));
			return true;
		}
	}

	if (pIApplyServerUserItem->IsAndroidUser()&&(m_ApplyUserArray.GetCount())>m_nRobotListMaxCount)
	{
		return true;
	}

	if (m_ApplyUserArray.GetCount() >= 5)
	{
		CMD_S_ApplyFailed ApplyFailed;
		_sntprintf(ApplyFailed.szBuffer, sizeof(ApplyFailed.szBuffer), TEXT("��ׯ������������������ׯ�ң�"));
		m_pITableFrame->SendTableData(pIApplyServerUserItem->GetChairID(), SUB_S_APPLEY_BANKER_FAILURE, &ApplyFailed, sizeof(ApplyFailed));
		return true;
	}

	//������Ϣ 
	m_ApplyUserArray.Add(wApplyUserChairID);

	//�������
	CMD_S_ApplyBanker ApplyBanker;
	ZeroMemory(&ApplyBanker,sizeof(ApplyBanker));

	//���ñ���
	ApplyBanker.wApplyUser=wApplyUserChairID;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

	//�л��ж�
	if (m_pITableFrame->GetGameStatus()==GAME_SCENE_FREE && m_ApplyUserArray.GetCount()==1)
	{
		ChangeBanker(false);
	}

	return true;
}

//ȡ������
bool CTableFrameSink::OnUserCancelBanker(IServerUserItem *pICancelServerUserItem)
{
	//��ǰׯ��
	if (pICancelServerUserItem->GetChairID()==m_wCurrentBanker && m_pITableFrame->GetGameStatus()!=GAME_SCENE_FREE)
	{
		m_pITableFrame->SendTableData(pICancelServerUserItem->GetChairID(), SUB_S_CANCEL_FAILURE);
		return true;
	}
	
	bool bValidQuitBanker = true;
	//�����ж�
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		//��ȡ���
		WORD wChairID=m_ApplyUserArray[i];
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);

		//��������
		if (pIServerUserItem==NULL) continue;
		if (pIServerUserItem->GetUserID()!=pICancelServerUserItem->GetUserID()) continue;

		//ɾ�����
		m_ApplyUserArray.RemoveAt(i);

		bValidQuitBanker = false;

		if (m_wCurrentBanker!=wChairID)
		{
			//�������
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//���ñ���
			CancelBanker.wCancelUser = pIServerUserItem->GetChairID();

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		}
		else if (m_wCurrentBanker==wChairID)
		{
			//�л�ׯ�� 
			m_wCurrentBanker=INVALID_CHAIR;
			ChangeBanker(true);
		}

		return true;
	}
	
	if (bValidQuitBanker == true)
	{
		m_wCurrentBanker=INVALID_CHAIR;
		ChangeBanker(true);
		return true;
	}

	return true;
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

//����ׯ��
bool CTableFrameSink::ChangeBanker(bool bCancelCurrentBanker)
{
	//�л���ʶ
	bool bChangeBanker=false;

	WORD wPreviousBanker = m_wCurrentBanker;

	//ȡ����ǰ
	if (bCancelCurrentBanker)
	{
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			//��ȡ���
			WORD wChairID=m_ApplyUserArray[i];

			//��������
			if (wChairID!=m_wCurrentBanker) continue;

			//ɾ�����
			m_ApplyUserArray.RemoveAt(i);

			break;
		}

		//����ׯ��
		m_wCurrentBanker=INVALID_CHAIR;

		//�ֻ��ж�
		TakeTurns();

		

		//���ñ���
		bChangeBanker=true;
		m_bExchangeBanker = true;
	}
	//��ׯ�ж�
	else if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		
		//��ȡׯ��
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if(pIServerUserItem)
		{
			SCORE lBankerScore = pIServerUserItem->GetUserScore();

			//�����ж�
			if (m_lPlayerBankerMAX<=m_wBankerTime || lBankerScore<m_lApplyBankerCondition)
			{
				//ׯ�������ж� ͬһ��ׯ�������ֻ�ж�һ��
				if(m_lPlayerBankerMAX <= m_wBankerTime && m_bExchangeBanker && lBankerScore>=m_lApplyBankerCondition)
				{
					//��ׯ�������ã���ׯ�������趨�ľ���֮��(m_nBankerTimeLimit)��
					//�������ֵ��������������ׯ���б�����������ҽ��ʱ��
					//�����ټ���ׯm_nBankerTimeAdd�֣���ׯ���������á�

					//��ҳ���m_lExtraBankerScore֮��
					//������������ҵĽ��ֵ�������Ľ��ֵ����Ҳ�����ټ�ׯm_nExtraBankerTime�֡�
					bool bScoreMAX = true;
					m_bExchangeBanker = false;

					for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
					{
						//��ȡ���
						WORD wChairID = m_ApplyUserArray[i];
						IServerUserItem *pIUserItem = m_pITableFrame->GetTableUserItem(wChairID);
						SCORE lScore = pIServerUserItem->GetUserScore();

						if ( wChairID != m_wCurrentBanker && lBankerScore <= lScore )
						{
							bScoreMAX = false;
							break;
						}
					}

					if ( bScoreMAX || (lBankerScore > m_lExtraBankerScore && m_lExtraBankerScore != 0l) )
					{
						if ( bScoreMAX )
						{
							m_lPlayerBankerMAX += m_nBankerTimeAdd;
						}
						if ( lBankerScore > m_lExtraBankerScore && m_lExtraBankerScore != 0l )
						{
							m_lPlayerBankerMAX += m_nExtraBankerTime;
						}
						return true;
					}
				}

				//����ׯ��
				m_wCurrentBanker=INVALID_CHAIR;

				//�ֻ��ж�
				TakeTurns();

				//�������
				for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
				{
					//��ȡ���
					WORD wChairID=m_ApplyUserArray[i];

					//��������
					if (wChairID!=m_wCurrentBanker) continue;

					//ɾ�����
					m_ApplyUserArray.RemoveAt(i);

					break;
				}

				bChangeBanker=true;
				m_bExchangeBanker = true;	
			}
		}
	}
	//ϵͳ��ׯ
	else if (m_wCurrentBanker == INVALID_CHAIR && m_ApplyUserArray.GetCount()!=0)
	{
		//�ֻ��ж�
		TakeTurns();

		//�������
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			//��ȡ���
			WORD wChairID=m_ApplyUserArray[i];

			//��������
			if (wChairID!=m_wCurrentBanker) continue;

			//ɾ�����
			m_ApplyUserArray.RemoveAt(i);

			break;
		}

		bChangeBanker=true;
		m_bExchangeBanker = true;
	}

	//�л��ж�
	if (bChangeBanker)
	{
		//�����ׯ��
		m_lPlayerBankerMAX = m_nBankerTimeLimit;

		//���ñ���
		m_wBankerTime = 0;
		m_lBankerWinScore=0;

		//������Ϣ
		CMD_S_ChangeBanker stChangeBanker;
		ZeroMemory(&stChangeBanker,sizeof(stChangeBanker));
		stChangeBanker.wPreviousBanker = wPreviousBanker;
		stChangeBanker.wBankerUser = m_wCurrentBanker;
		if (m_wCurrentBanker != INVALID_CHAIR)
		{
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
			stChangeBanker.lBankerScore = pIServerUserItem->GetUserScore();
		}
		else
		{
			stChangeBanker.lBankerScore = 10000000;
		}

		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&stChangeBanker,sizeof(CMD_S_ChangeBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&stChangeBanker,sizeof(CMD_S_ChangeBanker));
	}

	return bChangeBanker;
}

//�ֻ��ж�
void CTableFrameSink::TakeTurns()
{
	//��������
	int nInvalidApply = 0;

	for (int i = 0; i < m_ApplyUserArray.GetCount(); i++)
	{
		if (m_pITableFrame->GetGameStatus() == GAME_SCENE_FREE)
		{
			//��ȡ����
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_ApplyUserArray[i]);
			if (pIServerUserItem != NULL)
			{
				if (pIServerUserItem->GetUserScore() >= m_lApplyBankerCondition)
				{
					m_wCurrentBanker=m_ApplyUserArray[i];
					break;
				}
				else
				{
					nInvalidApply = i + 1;

					//������Ϣ
					CMD_S_CancelBanker CancelBanker = {};

					//���ñ���
					CancelBanker.wCancelUser=pIServerUserItem->GetChairID();					

					//������Ϣ
					m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
					m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
				}
			}
		}
	}

	//ɾ�����
	if (nInvalidApply != 0)
		m_ApplyUserArray.RemoveAt(0, nInvalidApply);
}

//����ׯ��
void CTableFrameSink::SendApplyUser( IServerUserItem *pRcvServerUserItem )
{
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];

		//��ȡ���
		IServerUserItem *pServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (!pServerUserItem) continue;

		//ׯ���ж�
		if (pServerUserItem->GetChairID()==m_wCurrentBanker) continue;

		//�������
		CMD_S_ApplyBanker ApplyBanker;
		ApplyBanker.wApplyUser=wChairID;

		//������Ϣ
		m_pITableFrame->SendUserItemData(pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	}
}


//�����ע
SCORE CTableFrameSink::GetUserMaxJetton(WORD wChairID, BYTE cbJettonArea)
{
	IServerUserItem *pIMeServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	if (NULL==pIMeServerUserItem) return 0L;

	int iTimer = m_nMaxTimes;

	//����ע��
	SCORE lNowJetton = 0;

	for (int nAreaIndex = 1; nAreaIndex <= AREA_MAX; ++nAreaIndex) lNowJetton += m_lUserJettonScore[wChairID][nAreaIndex];

	//ׯ�ҽ��
	SCORE lBankerScore = m_lBankerScore*m_nEndGameMul / 100;;
	for (int nAreaIndex = 1; nAreaIndex <= AREA_MAX; ++nAreaIndex) lBankerScore -= m_lAllJettonScore[nAreaIndex] * iTimer;

	//������
	SCORE lAreaLimitScore = m_lAreaLimitScore - m_lAllJettonScore[cbJettonArea];

	//��������
	SCORE lMeMaxScore = min((pIMeServerUserItem->GetUserScore() - lNowJetton*iTimer) / iTimer, m_lUserLimitScore);

//	const tagUserScore *Info = pIMeServerUserItem->GetUserScore();

//	const tagUserInfo *Data = pIMeServerUserItem->GetUserInfo();

	//��������
	lMeMaxScore=min(lMeMaxScore,lAreaLimitScore);

	//ׯ������
	lMeMaxScore=min(lMeMaxScore,lBankerScore/iTimer);

	//��������
	ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = max(lMeMaxScore, 0);

	return (lMeMaxScore);

}
//����÷�
SCORE CTableFrameSink::CalculateScore()
{
	//	return 1;
	//��������
	SCORE static wRevenue = m_pGameServiceOption->wRevenueRatio;

	//�ƶ����
	bool static bWinTianMen, bWinDiMen, bWinXuanMen,bWinHuang;
	BYTE TianMultiple,diMultiple,TianXuanltiple,HuangMultiple;
	TianMultiple  = 1;
	diMultiple = 1 ;
	TianXuanltiple = 1;
	HuangMultiple = 1;
	DeduceWinner(bWinTianMen, bWinDiMen, bWinXuanMen,bWinHuang,TianMultiple,diMultiple,TianXuanltiple,HuangMultiple );

	//��Ϸ��¼
	tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.bWinShunMen = bWinTianMen ? TRUE : FALSE;
	GameRecord.bWinDuiMen = bWinDiMen ? TRUE : FALSE;
	GameRecord.bWinDaoMen = bWinXuanMen ? TRUE : FALSE;
	GameRecord.bWinHuang = bWinHuang ? TRUE : FALSE;

	m_cbOpenResult[0] = bWinTianMen ? TRUE : FALSE;
	m_cbOpenResult[1] = bWinDiMen ? TRUE : FALSE;
	m_cbOpenResult[2] = bWinXuanMen ? TRUE : FALSE;
	m_cbOpenResult[3] = bWinHuang ? TRUE : FALSE;

	BYTE  cbMultiple[] = {0, 1, 1, 1, 1};

	cbMultiple[1] = TianMultiple;
	cbMultiple[2] = diMultiple;
	cbMultiple[3] = TianXuanltiple;
	cbMultiple[4] = HuangMultiple;

	//�ƶ��±�
	m_nRecordLast = (m_nRecordLast + 1) % MAX_SCORE_HISTORY;
	if (m_nRecordLast == m_nRecordFirst)
	{
		m_nRecordFirst = (m_nRecordFirst + 1) % MAX_SCORE_HISTORY;
	}

	//ׯ������
	SCORE lBankerWinScore = 0;

	//��ҳɼ�
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	SCORE lUserLostScore[GAME_PLAYER];
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	//������
	//BYTE static const cbMultiple[] = {0, 1, 1, 1, 1, 1, 1};

	//ʤ����ʶ
	bool static bWinFlag[AREA_MAX + 1];
	bWinFlag[0]=false;
	bWinFlag[ID_TIAN_MEN]=bWinTianMen;
	bWinFlag[ID_DI_MEN]=bWinDiMen;
	bWinFlag[ID_XUAN_MEN]=bWinXuanMen;
	bWinFlag[ID_HUANG_MEN]=bWinHuang;

	//FILE *pf = fopen("C://ServLong.txt","ab+");

	//�������
	for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//ׯ���ж�
		if (m_wCurrentBanker==wChairID) continue;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;

		for (WORD wAreaIndex = ID_TIAN_MEN; wAreaIndex <= ID_HUANG_MEN; ++wAreaIndex)
		{

			if (true==bWinFlag[wAreaIndex]) 
			{
				m_lUserWinScore[wChairID] += (m_lUserJettonScore[wChairID][wAreaIndex] * cbMultiple[wAreaIndex]);
				m_lUserReturnScore[wChairID] += m_lUserJettonScore[wChairID][wAreaIndex];
				lBankerWinScore -= (m_lUserJettonScore[wChairID][wAreaIndex] * cbMultiple[wAreaIndex]);
			}
			else
			{
				lUserLostScore[wChairID] -= m_lUserJettonScore[wChairID][wAreaIndex] * cbMultiple[wAreaIndex];
				lBankerWinScore += m_lUserJettonScore[wChairID][wAreaIndex] * cbMultiple[wAreaIndex];
			}
		}
		
		//�ܵķ���
		m_lUserWinScore[wChairID] += lUserLostScore[wChairID];

		//����˰��
		if (0 < m_lUserWinScore[wChairID])
		{
			double fRevenuePer=double(wRevenue/1000.0);
			m_lUserRevenue[wChairID] = SCORE(m_lUserWinScore[wChairID] * fRevenuePer);
			m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];
		}
	}

	//ׯ�ҳɼ�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;

		//����˰��
		if (0 < m_lUserWinScore[m_wCurrentBanker])
		{
			double fRevenuePer=double(wRevenue/1000.0);
			m_lUserRevenue[m_wCurrentBanker] = SCORE(m_lUserWinScore[m_wCurrentBanker] * fRevenuePer);
			m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
			lBankerWinScore = m_lUserWinScore[m_wCurrentBanker];
		}	
	}

	//����ׯ�ҳɼ�
	if (m_wOfflineBanker!=INVALID_CHAIR)
	{
		m_lUserWinScore[m_wOfflineBanker] = lBankerWinScore;

		//����˰��
		if (0 < m_lUserWinScore[m_wOfflineBanker])
		{
			double fRevenuePer=double(wRevenue/1000.0);
			m_lUserRevenue[m_wOfflineBanker] = SCORE(m_lUserWinScore[m_wOfflineBanker] * fRevenuePer);
			m_lUserWinScore[m_wOfflineBanker] -= m_lUserRevenue[m_wOfflineBanker];
			lBankerWinScore = m_lUserWinScore[m_wOfflineBanker];
		}	
	}

	//�ۼƻ���
	m_lBankerWinScore += lBankerWinScore;

	//��ǰ����
	m_lBankerCurGameScore=lBankerWinScore;

	return lBankerWinScore;
}


void CTableFrameSink::DeduceWinner(bool &bWinTian, bool &bWinDi, bool &bWinXuan,bool &bWinHuan,BYTE &TianMultiple,BYTE &diMultiple,BYTE &TianXuanltiple,BYTE &HuangMultiple)
{
	//��С�Ƚ�
	bWinTian = m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX], 5, m_cbTableCardArray[SHUN_MEN_INDEX], 5, m_nMaxTimes, TianMultiple) == 1 ? true : false;
	bWinDi = m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX], 5, m_cbTableCardArray[DUI_MEN_INDEX], 5, m_nMaxTimes, diMultiple) == 1 ? true : false;
	bWinXuan = m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX], 5, m_cbTableCardArray[DAO_MEN_INDEX], 5, m_nMaxTimes, TianXuanltiple) == 1 ? true : false;
	bWinHuan = m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX], 5, m_cbTableCardArray[HUAN_MEN_INDEX], 5, m_nMaxTimes, HuangMultiple) == 1 ? true : false;
}

//���ͼ�¼
void CTableFrameSink::SendGameRecord(IServerUserItem *pIServerUserItem)
{
	WORD wBufferSize=0;
	BYTE cbBuffer[SOCKET_TCP_BUFFER];
	int nIndex = m_nRecordFirst;
	BYTE cbRecordCount = m_nRecordLast - m_nRecordFirst;

	if (cbRecordCount > 20)
	{
		nIndex = m_nRecordLast - 20;
	}

	if (m_nRecordLast < m_nRecordFirst)
	{
		nIndex = (m_nRecordLast + MAX_SCORE_HISTORY - 20) % MAX_SCORE_HISTORY;
	}

	while (nIndex != m_nRecordLast)
	{
		if ((wBufferSize+sizeof(tagServerGameRecord))>sizeof(cbBuffer))
		{
			m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
			wBufferSize=0;
		}
		CopyMemory(cbBuffer+wBufferSize,&m_GameRecordArrary[nIndex],sizeof(tagServerGameRecord));
		wBufferSize+=sizeof(tagServerGameRecord);

		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
	} 

	if (wBufferSize>0) m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
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
	m_bEnableSysBanker = (pCustomConfig->CustomGeneral.nEnableSysBanker == TRUE)?true:false;
	
	//ʱ��
	m_nFreeTime = pCustomConfig->CustomGeneral.lFreeTime;
	m_nPlaceJettonTime = pCustomConfig->CustomGeneral.lBetTime;
	m_nGameEndTime = pCustomConfig->CustomGeneral.lEndTime;
	if(m_nFreeTime < TIME_FREE	|| m_nFreeTime > 99) m_nFreeTime = TIME_FREE;
	if(m_nPlaceJettonTime < TIME_PLACE_JETTON || m_nPlaceJettonTime > 99) m_nPlaceJettonTime = TIME_PLACE_JETTON;
	if(m_nGameEndTime < TIME_GAME_END || m_nGameEndTime > 99) m_nGameEndTime = TIME_GAME_END;

	//��ע
	m_lAreaLimitScore = pCustomConfig->CustomGeneral.lAreaLimitScore;
	m_nEndGameMul = pCustomConfig->CustomGeneral.lEndGameMul;
	if(m_nEndGameMul < 1 || m_nEndGameMul > 100 ) m_nEndGameMul = 80;
	
	//������
	m_nRobotListMaxCount = pCustomConfig->CustomAndroid.lRobotListMaxCount;

	LONGLONG lRobotBetMinCount = pCustomConfig->CustomAndroid.lRobotBetMinCount;
	LONGLONG lRobotBetMaxCount = pCustomConfig->CustomAndroid.lRobotBetMaxCount;
	m_nMaxChipRobot = rand()%(lRobotBetMaxCount-lRobotBetMinCount+1) + lRobotBetMinCount;
	if (m_nMaxChipRobot < 0)	m_nMaxChipRobot = 8;
	m_lRobotAreaLimit = pCustomConfig->CustomAndroid.lRobotAreaLimit;
	
	return;
}

//����ϵͳ����������ׯʱ������ҵ÷�
SCORE CTableFrameSink::CalSysOrAndroidBankerScore()
{
	ASSERT ((m_wCurrentBanker == INVALID_CHAIR && m_bEnableSysBanker == true)
		|| (m_wCurrentBanker != INVALID_CHAIR && m_pITableFrame->GetTableUserItem(m_wCurrentBanker)->IsAndroidUser() == true));

	//�Ƚϱ���
	bool bXianWin[4] = { false, false, false, false };							//�Ƚ���Ӯ
	BYTE bMultiple[4] = { 1, 1, 1, 1 };											//�Ƚϱ���
	for (int i = 0; i < 4; i++)
	{
		bXianWin[i] = (m_GameLogic.CompareCard(m_cbTableCardArray[0], 5, m_cbTableCardArray[i + 1], 5, m_nMaxTimes, bMultiple[i]) == 1);
	}

	//ϵͳ����������ׯʱ������ҵ÷�
	SCORE lRealPlayerWinScore = 0;

	// ����ϵͳ��Ӯ
	for (WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
	{
		// ��ȡ���
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);

		// ����ׯ��
		if (wChairID == m_wCurrentBanker || pIServerUserItem == NULL)
		{
			continue;
		}

		//���˻�����
		if (pIServerUserItem->IsAndroidUser() == true)
		{
			continue;
		}

		// ���������ע
		for (int nAarea = 0; nAarea < 4; nAarea++)
		{
			if (m_lUserJettonScore[wChairID][nAarea + 1] != 0)
			{
				if( bXianWin[nAarea] )
				{				
					lRealPlayerWinScore += m_lUserJettonScore[wChairID][nAarea + 1] * bMultiple[nAarea];
				}
				else
				{
					lRealPlayerWinScore -= m_lUserJettonScore[wChairID][nAarea + 1] * bMultiple[nAarea];
				}
			}
		}
	}

	return lRealPlayerWinScore;
}

//�������������ׯʱ���˵ĵ÷�
SCORE CTableFrameSink::CalRealPlayerScore()
{
	SCORE lPlayerUserScore = 0;

	//�Ƚϱ���
	bool bXianWin[4] = { false, false, false, false };							//�Ƚ���Ӯ
	BYTE bMultiple[4] = { 1, 1, 1, 1 };											//�Ƚϱ���
	for (int i = 0; i < 4; i++)
	{
		bXianWin[i] = (m_GameLogic.CompareCard(m_cbTableCardArray[0], 5, m_cbTableCardArray[i + 1], 5, m_nMaxTimes, bMultiple[i]) == 1);
	}

	//ϵͳ��ׯ���߻����˵�ׯ
	if ((m_wCurrentBanker != INVALID_CHAIR && m_pITableFrame->GetTableUserItem(m_wCurrentBanker)->IsAndroidUser() == true) || (m_wCurrentBanker == INVALID_CHAIR))
	{
		// ����ϵͳ��Ӯ
		for (WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
		{
			// ��ȡ���
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);

			// ����ׯ��
			if (wChairID == m_wCurrentBanker || pIServerUserItem == NULL || pIServerUserItem->IsAndroidUser() == true)
			{
				continue;
			}

			// ���������ע
			for (int nAarea = 0; nAarea < 4; nAarea++)
			{
				if (m_lUserJettonScore[wChairID][nAarea + 1] != 0)
				{
					if (bXianWin[nAarea])
					{
						lPlayerUserScore += m_lUserJettonScore[wChairID][nAarea + 1] * bMultiple[nAarea];
					}
					else
					{
						lPlayerUserScore -= m_lUserJettonScore[wChairID][nAarea + 1] * bMultiple[nAarea];
					}
				}
			}
		}
	}
	//��ҵ�ׯ
	else
	{
		// ����ϵͳ��Ӯ
		for (WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
		{
			// ��ȡ���
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);

			// ����ׯ��
			if (wChairID == m_wCurrentBanker || pIServerUserItem == NULL || pIServerUserItem->IsAndroidUser() == false)
			{
				continue;
			}

			// ���������ע
			for (int nAarea = 0; nAarea < 4; nAarea++)
			{
				if (m_lUserJettonScore[wChairID][nAarea + 1] != 0)
				{
					if (bXianWin[nAarea])
					{
						lPlayerUserScore -= m_lUserJettonScore[wChairID][nAarea + 1] * bMultiple[nAarea];
					}
					else
					{
						lPlayerUserScore += m_lUserJettonScore[wChairID][nAarea + 1] * bMultiple[nAarea];
					}
				}
			}
		}
	}
	
	return lPlayerUserScore;
}

//��ѯ�Ƿ�۷����
bool CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem *pUserItem=m_pITableFrame->GetTableUserItem(i);
		if(pUserItem==NULL) continue;
		if(wChairID==i)
		{
			//������ע
			for (int nAreaIndex = 0; nAreaIndex <= AREA_MAX; ++nAreaIndex)
			{

				if (m_lUserJettonScore[wChairID][nAreaIndex] != 0)
				{
					return true;
				}
			}
			break;
		}
	}
	if (wChairID==m_wCurrentBanker)
	{
		return true;
	}

	//����ׯ��
	if (wChairID == m_wOfflineBanker)
	{
		return true;
	}

	return false;
}

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
