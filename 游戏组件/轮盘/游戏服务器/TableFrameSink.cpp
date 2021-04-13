#include "StdAfx.h"
#include "TableFrameSink.h"
#include <locale>
#include "DlgCustomRule.h"
//////////////////////////////////////////////////////////////////////////

//��������
#define SEND_COUNT					300									//���ʹ���

//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//��עʱ��
#define IDI_FREE					1									//����ʱ��
#define TIME_FREE					5									//����ʱ��

//��עʱ��
#define IDI_PLACE_JETTON			2									//��עʱ��

#define TIME_PLACE_JETTON			15									//��עʱ��

//����ʱ��
#define IDI_GAME_END				3									//����ʱ��
#define TIME_GAME_END				20									//����ʱ��
#define TIME_GAME_ADD               10                                  //����ʱ��
//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//��Ϸ����
//ConsoleWindow	    CTableFrameSink::m_DebugWindow;

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//����ע��
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//������ע
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	ZeroMemory(m_bUserListWin, sizeof(m_bUserListWin));
	ZeroMemory(m_lUserListScore, sizeof(m_lUserListScore));
	ZeroMemory(m_cbUserPlayCount, sizeof(m_cbUserPlayCount));
	ZeroMemory(m_lUserTypeScore, sizeof(m_lUserTypeScore));
	ZeroMemory(m_lPlayScore, sizeof(m_lPlayScore));

	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));
	ZeroMemory(m_lPreviousAreaScore, sizeof(m_lPreviousAreaScore));

	//�˿���Ϣ
	m_cbTableCard = 0;
	//״̬����
	m_dwJettonTime=0L;

	//��¼����
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;
	m_nRecordLast=0;

	//ʱ�����
	m_cbBetTime = TIME_PLACE_JETTON;
	m_cbEndTime = TIME_GAME_END;


	srand(GetTickCount());

	//��ʼ����������
	InitAreaNums();

	m_cbJuControl = FALSE;
	m_cbJuControlTimes = 0;
	m_cbJuControlArea = 255;

	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
}

//�ӿڲ�ѯ
void *  CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{

	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);	
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;

}

//��ʼ��
bool  CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{

	//��ѯ�ӿ�
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//��ѯ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();

	//��ʼģʽ
	//m_pITableFrame->SetStartMode(START_MODE_ALL_READY);

	m_pITableFrame->SetStartMode(START_MODE_TIME_CONTROL);
    ReadConfigInformation();

	//�����ļ���
	TCHAR szPath[MAX_PATH] = TEXT("");
	GetCurrentDirectory(sizeof(szPath), szPath);
	_sntprintf(m_szConfigFileName, sizeof(m_szConfigFileName), TEXT("%s\\BaiRenConfig\\RouletteConfig-%d-%d.ini"), szPath, m_pGameServiceOption->wServerID, m_pITableFrame->GetTableID());

	m_nChips[0] = GetPrivateProfileInt(TEXT("����"), TEXT("nChip0"), 1, m_szConfigFileName);
	m_nChips[1] = GetPrivateProfileInt(TEXT("����"), TEXT("nChip1"), 10, m_szConfigFileName);
	m_nChips[2] = GetPrivateProfileInt(TEXT("����"), TEXT("nChip2"), 100, m_szConfigFileName);
	m_nChips[3] = GetPrivateProfileInt(TEXT("����"), TEXT("nChip3"), 500, m_szConfigFileName);

	m_lTypeLimitScore[0] = GetPrivateProfileInt(TEXT("����"), TEXT("nStraight"), -1, m_szConfigFileName);
	m_lTypeLimitScore[1] = GetPrivateProfileInt(TEXT("����"), TEXT("nSplit"), -1, m_szConfigFileName);
	m_lTypeLimitScore[2] = GetPrivateProfileInt(TEXT("����"), TEXT("nStreet"), -1, m_szConfigFileName);
	m_lTypeLimitScore[3] = GetPrivateProfileInt(TEXT("����"), TEXT("nCorner"), -1, m_szConfigFileName);
	m_lTypeLimitScore[4] = GetPrivateProfileInt(TEXT("����"), TEXT("nLine"), -1, m_szConfigFileName);
	m_lTypeLimitScore[5] = GetPrivateProfileInt(TEXT("����"), TEXT("nThree"), -1, m_szConfigFileName);
	m_lTypeLimitScore[6] = GetPrivateProfileInt(TEXT("����"), TEXT("nDouble"), -1, m_szConfigFileName);

	for (int i = 0; i < 7; i++)
	{
		if (m_lTypeLimitScore[i] == -1)
		{
			CTraceService::TraceString(TEXT("���������������ô���"),TraceLevel_Warning);
			return false;
		}
	}

	return true;
}

//��λ����
VOID  CTableFrameSink::RepositionSink()
{
	//����ע��
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));
	
	//������ע
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
	ZeroMemory(m_lPlayScore, sizeof(m_lPlayScore));

	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	ZeroMemory(m_lUserTypeScore, sizeof(m_lUserTypeScore));

	m_cbTableCard = 255;
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

//��ѯ�����
bool CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
	{
		if (m_lUserJettonScore[wChairID][nAreaIndex]>0)
			return true;
	}
	return false;
}

//��Ϸ��ʼ
bool  CTableFrameSink::OnEventGameStart()
{
	//���˥��
	m_pITableFrame->CalculateStorageDeduct();

	CMD_S_GameStart GameStart;
	GameStart.cbTimeLeave = m_cbBetTime;

	FillMemory(GameStart.wSeatUser, sizeof(GameStart.wSeatUser), INVALID_CHAIR);

	OnGetUserListGameID(GameStart.wSeatUser);

    //�Թ����
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_START, &GameStart, sizeof(GameStart));
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GAME_START, &GameStart, sizeof(GameStart));

	return true;
}

//��Ϸ����
bool  CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������	
	{
		if (m_cbJuControl == TRUE)
		{
			m_cbTableCard = m_cbJuControlArea;
		}
		else
		{
			tag_LP_Parameter LP_Parameter;
			ZeroMemory(&LP_Parameter, sizeof(LP_Parameter));

			CopyMemory(LP_Parameter.lUserJettonScore, m_lUserJettonScore, sizeof(LP_Parameter.lUserJettonScore));

			tag_LP_Result pLP_Result;
			ZeroMemory(&pLP_Result, sizeof(pLP_Result));
			m_pITableFrame->GetControlResult(&LP_Parameter, sizeof(LP_Parameter), &pLP_Result);

			m_cbTableCard = pLP_Result.cbTableCardArray;
		}

		if (m_cbJuControlTimes > 0)
			m_cbJuControlTimes--;
		if (m_cbJuControlTimes == 0)
		{
			m_cbJuControl = FALSE;
			m_cbJuControlArea = 255;
		}

		//�������
		CalculateScore();

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
			for (BYTE j = 0; j < AREA_COUNT; j++)
			{
				lAddScore += m_lUserJettonScore[i][j];
			}
			m_lUserListScore[i][m_cbUserPlayCount[i]] = lAddScore;
			m_cbUserPlayCount[i] ++;

			if (m_cbUserPlayCount[i] == USER_LIST_COUNT)
				m_cbUserPlayCount[i] = 0;
		}

		//������Ϣ
		CMD_S_GameEnd GameEnd;
		ZeroMemory(&GameEnd,sizeof(GameEnd));

		CopyMemory(GameEnd.lAllJettonScore, m_lAllJettonScore, sizeof(GameEnd.lAllJettonScore));

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

		GameEnd.cbResult = m_cbTableCard;
      
		for(WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex)
		{
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
			if(pIServerUserItem == NULL)
			{
				continue;
			}

			CopyMemory(GameEnd.lPlayAreaScore, m_lPlayScore[wUserIndex], sizeof(GameEnd.lPlayAreaScore));
			GameEnd.lPlayerWinScore = m_lUserWinScore[wUserIndex];
			GameEnd.lPlayerRestScore = pIServerUserItem->GetUserScore() + GameEnd.lPlayerWinScore;

			CopyMemory(GameEnd.lPlayerJettonScore, m_lUserJettonScore[wUserIndex], sizeof(GameEnd.lPlayerJettonScore));

			//������Ϣ
			m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			if (!pIServerUserItem->IsAndroidUser())
			{
				m_pGameServiceOption->pRoomStorageOption.lStorageCurrent -= GameEnd.lPlayerWinScore;
			}
		}

		tagRolettePerGameRecord GameRecord;
		GameRecord.wTableID = m_pITableFrame->GetTableID() + 1;
		GameRecord.cbResult = GameEnd.cbResult;
		m_pITableFrame->SendTableDataToLookOnUser(SUB_GF_BJL_GAME_RESULT, &GameRecord, sizeof(GameRecord));

		return true;
	}
	case GER_USER_LEAVE:		//�û��뿪
	case GER_NETWORK_ERROR:
		{
			return true;
		}
	}

	return false;
}

//���ͳ���
bool  CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_PLACE_JETTON:		//��Ϸ״̬
	case GS_GAME_END:			//����״̬
		{

			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay, sizeof(StatusPlay));

			FillMemory(StatusPlay.wSeatUser, sizeof(StatusPlay.wSeatUser), INVALID_CHAIR);

			CopyMemory(StatusPlay.lChips, m_nChips, sizeof(StatusPlay.lChips));

			CopyMemory(StatusPlay.lTypeLimit, m_lTypeLimitScore, sizeof(StatusPlay.lTypeLimit));

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			if (cbGameStatus == GS_PLACE_JETTON)
				StatusPlay.cbTimeLeave=(BYTE)(m_cbBetTime-__min(dwPassTime,m_cbBetTime));
			else
				StatusPlay.cbTimeLeave = (BYTE)(m_cbEndTime - __min(dwPassTime, m_cbEndTime));

			StatusPlay.nPlayTimes = m_cbBetTime;
			StatusPlay.nEndTimes = m_cbEndTime;

			OnGetUserListGameID(StatusPlay.wSeatUser);

			for (WORD i = 0; i < MAX_SEAT_COUNT; i++)
			{
				if (StatusPlay.wSeatUser[i] != INVALID_CHAIR)
				{
					WORD wSeatChair = StatusPlay.wSeatUser[i];
					CopyMemory(StatusPlay.lSeatUserAreaScore[i], m_lUserJettonScore[wSeatChair], sizeof(StatusPlay.lSeatUserAreaScore[i]));
				}
			}

			//�����ж�
			if(cbGameStatus==GS_GAME_END)
			{
				StatusPlay.cbResult = m_cbTableCard;
				StatusPlay.lPlayerWinScore = m_lUserWinScore[wChiarID];

				//ռλ��ҳɼ�
				for (WORD i = 0; i < MAX_SEAT_COUNT; i++)
				{
					if (StatusPlay.wSeatUser[i] != INVALID_CHAIR)
					{
						StatusPlay.lSeatUserWinScore[i] = m_lUserWinScore[StatusPlay.wSeatUser[i]];
					}
				}

				StatusPlay.lPlayerRestScore = pIServerUserItem->GetUserScore() + m_lUserWinScore[pIServerUserItem->GetChairID()];
			}

			FillMemory(StatusPlay.cbWinMen, sizeof(StatusPlay.cbWinMen),255);
			int nIndex = m_nRecordFirst;
			int nAddNum = 0;

			//�ͻ���ֻ��ʾ40��
			BYTE cbRecordCount = m_nRecordLast - m_nRecordFirst;
			if (cbRecordCount > 40)
			{
				nIndex = m_nRecordLast - 40;
			}
			if (m_nRecordLast < m_nRecordFirst)
			{
				nIndex = (m_nRecordLast + MAX_SCORE_HISTORY - 40) % MAX_SCORE_HISTORY;
			}

			while (nIndex != m_nRecordLast)
			{
				if (nAddNum >= 40)
					break;

				tagServerGameRecord ServerGameRecord = m_GameRecordArrary[nIndex];
				StatusPlay.cbWinMen[nAddNum++] = ServerGameRecord.cbResult;

				nIndex = (nIndex + 1) % MAX_SCORE_HISTORY;
			}

			for (BYTE i = 0; i < AREA_COUNT; i++)
			{
				StatusPlay.lPlayerJettonScore[i] = m_lUserJettonScore[wChiarID][i];
			}
			CopyMemory(StatusPlay.lAllJettonScore, m_lAllJettonScore, sizeof(StatusPlay.lAllJettonScore));

			//���ͳ���
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

			//����������
			if (pIServerUserItem->IsAndroidUser())
			{
				tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pITableFrame->GetCustomRule();
				ASSERT(pCustomConfig);

				m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_ANDROID_CONFIG, &pCustomConfig->CustomAndroid, sizeof(tagCustomAndroid));
			}

			return bSuccess;
		}
	}

	return false;
}

//��ʱ���¼�
bool  CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_PLACE_JETTON:		//��עʱ��
		{
			//״̬�ж�(��ֹǿ���ظ�����)
			if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
			{
				//����״̬
				m_pITableFrame->SetGameStatus(GS_GAME_END);			

				//������Ϸ
				OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);

				//����ʱ��
				m_dwJettonTime=(DWORD)time(NULL);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);	

				tagChangeStatus ChangeStatus;
				ChangeStatus.wTableID = m_pITableFrame->GetTableID() + 1;
				ChangeStatus.cbGameStatus = m_pITableFrame->GetGameStatus();
				m_pITableFrame->SendTableDataToLookOnUser(SUB_GF_BJL_CHANGE_STATUS, &ChangeStatus, sizeof(ChangeStatus));
			}

			return true;
		}
	case IDI_GAME_END:			//������Ϸ
		{
			tagScoreInfo ScoreInfo;

			//д�����
			for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);
				if ( pIServerUserItem == NULL ) continue;
				ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));

				if(m_lUserWinScore[wUserChairID] != 0)
				{
					ScoreInfo.cbType=(m_lUserWinScore[wUserChairID]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
					ScoreInfo.lRevenue=m_lUserRevenue[wUserChairID];
					ScoreInfo.lScore=m_lUserWinScore[wUserChairID];
					
					m_pITableFrame->WriteUserScore(wUserChairID,ScoreInfo);
				}
			}

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
			m_pITableFrame->ConcludeGame(GS_PLACE_JETTON);

			//��ʼ��Ϸ
			m_pITableFrame->StartGame();

			//����ʱ��
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON, m_cbBetTime * 1000, 1, 0L);

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
	//������ע
	for (int nAreaIndex = 0; nAreaIndex < AREA_COUNT; ++nAreaIndex)
	{
		if (m_lUserJettonScore[wChairID][nAreaIndex] != 0)
		{
			return true;
		}
	}

	return false;
}

//������Ϸ��ȡ��Ϸ��¼
void CTableFrameSink::OnGetGameRecord(VOID *GameRecord)
{
	CMD_GF_RoletteRoomStatus *pRoomStatus = (CMD_GF_RoletteRoomStatus *)GameRecord;

	pRoomStatus->tagGameInfo.wTableID = m_pITableFrame->GetTableID() + 1;
	pRoomStatus->tagGameInfo.cbGameStatus = m_pITableFrame->GetGameStatus();

	pRoomStatus->tagTimeInfo.cbBetTime = m_cbBetTime;
	pRoomStatus->tagTimeInfo.cbEndTime = m_cbEndTime;
	pRoomStatus->tagTimeInfo.cbPassTime = (DWORD)time(NULL) - m_dwJettonTime;

	int nIndex = m_nRecordFirst;
	int nArrayIndex = 0;

	pRoomStatus->cbRecordCount = m_nRecordLast - m_nRecordFirst;
	if (m_nRecordLast < m_nRecordFirst)
	{
		pRoomStatus->cbRecordCount = m_nRecordLast + MAX_SCORE_HISTORY - m_nRecordFirst;
	}

	while (nIndex != m_nRecordLast)
	{
		if (nArrayIndex >= 72)
			break;
		pRoomStatus->GameRecordArrary[nArrayIndex].cbResult = m_GameRecordArrary[nIndex].cbResult;

		nArrayIndex++;
		nIndex = (nIndex + 1) % MAX_SCORE_HISTORY;
	}
}

//��Ϸ��Ϣ����
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	
	switch (wSubCmdID)
	{
		case SUB_C_PLACE_JETTON:		//�û���ע
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_PlaceJetton));
			if (wDataSize!=sizeof(CMD_C_PlaceJetton)) return true;

			//�û�Ч��
			
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//��Ϣ����
			CMD_C_PlaceJetton * pPlaceJetton=(CMD_C_PlaceJetton *)pDataBuffer;
	


			return OnUserPlaceJetton(pUserData->wChairID, pPlaceJetton->cbBetType, pPlaceJetton->cbBetArea, pPlaceJetton->lBetScore, pPlaceJetton->nBetKeys);
		}
		//������Ϣ
		case SUB_C_CHEHUI:
		{
			WORD wChairID = pIServerUserItem->GetChairID();

			for (int i = 0; i < AREA_COUNT; i++)
			{
				m_lAllJettonScore[i] -= m_lUserJettonScore[wChairID][i];
			}
			ZeroMemory(m_lUserJettonScore[wChairID], sizeof(m_lUserJettonScore[wChairID]));
			ZeroMemory(m_lUserTypeScore[wChairID], sizeof(m_lUserTypeScore[wChairID]));

			CMD_S_CheHui CheHui;
			ZeroMemory(&CheHui, sizeof(CheHui));
			CheHui.wChairID = wChairID;
			CheHui.lUserRestScore = pIServerUserItem->GetUserScore();

			m_pITableFrame->SendTableData(wChairID, SUB_S_CHEHUI, &CheHui, sizeof(CheHui));
			m_pITableFrame->SendLookonData(wChairID, SUB_S_CHEHUI, &CheHui, sizeof(CheHui));
			return true;
		}
		//��ȡ����б�
		case SUB_C_ONLINE_UESER:
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
			ControlInfo.cbJuControlArea = m_cbJuControlArea;

			BYTE cbIndex = 0;
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);
				if (pServerUser && !pServerUser->IsAndroidUser())
				{
					for (BYTE j = 0; j < AREA_COUNT; j++)
					{
						ControlInfo.lAreaJettonScore[j] += m_lUserJettonScore[i][j];
					}

					cbIndex++;
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

			CMD_C_ControlWinLost *pControlWinLost = (CMD_C_ControlWinLost *)pDataBuffer;

			m_cbJuControlArea = pControlWinLost->cbJuControlArea;

			m_cbJuControl = TRUE;
			m_cbJuControlTimes = pControlWinLost->cbControlTimes;

			if (m_cbJuControlTimes < 0 || m_cbJuControlTimes > 5 || m_cbJuControlArea >= 37)
			{
				m_cbJuControl = FALSE;
				m_cbJuControlTimes = 0;
				m_cbJuControlArea = 255;
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
			m_cbJuControlArea = 255;

			return true;
		}
	}

	return false;
}

//�����Ϣ����
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�����¼�
bool CTableFrameSink::OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//�û�����
bool  CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//����ʱ��
	if ((bLookonUser==false)&&(m_dwJettonTime==0L))
	{
		m_dwJettonTime=(DWORD)time(NULL);
		m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON, m_cbBetTime * 1000, 1, NULL);
		m_pITableFrame->SetGameStatus(GS_PLACE_JETTON);
		m_pITableFrame->StartGame();
	}

	ZeroMemory(m_lPreviousAreaScore[wChairID], sizeof(m_lPreviousAreaScore[wChairID]));

	return true;
}

//�û�����
bool  CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��ʼ����

	ZeroMemory(m_bUserListWin[wChairID], sizeof(m_bUserListWin[wChairID]));
	ZeroMemory(m_lUserListScore[wChairID], sizeof(m_lUserListScore[wChairID]));
	m_cbUserPlayCount[wChairID] = 0;


	return true;
}

//��ע�¼�
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonType, BYTE cbJettonArea[], SCORE lJettonScore, int nKeys)
{
	//Ч�����
	if (lJettonScore != m_nChips[0] && lJettonScore != m_nChips[1] && lJettonScore != m_nChips[2] && lJettonScore != m_nChips[3])
	{
		CMD_S_PlaceJetton_Fail PlaceJettonFail;
		ZeroMemory(&PlaceJettonFail, sizeof(PlaceJettonFail));
		
		CopyMemory(PlaceJettonFail.cbJettonArea, cbJettonArea, sizeof(PlaceJettonFail.cbJettonArea));
		_sntprintf(PlaceJettonFail.szFailReason, sizeof(PlaceJettonFail.szFailReason),TEXT("��ע��Ϣ����"));

		//������Ϣ
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceJettonFail, sizeof(PlaceJettonFail));
		return true;
	}

	//Ч�����
	if (cbJettonType < BET_TYPE_STRAIGHT || cbJettonType > BET_TYPE_HI)
	{
		CMD_S_PlaceJetton_Fail PlaceJettonFail;
		ZeroMemory(&PlaceJettonFail, sizeof(PlaceJettonFail));

		CopyMemory(PlaceJettonFail.cbJettonArea, cbJettonArea, sizeof(PlaceJettonFail.cbJettonArea));
		_sntprintf(PlaceJettonFail.szFailReason, sizeof(PlaceJettonFail.szFailReason), TEXT("��ע��Ϣ����"));

		//������Ϣ
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceJettonFail, sizeof(PlaceJettonFail));
		return true;
	}

	if(m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON)
	{
		CMD_S_PlaceJetton_Fail PlaceJettonFail;
		ZeroMemory(&PlaceJettonFail, sizeof(PlaceJettonFail));

		CopyMemory(PlaceJettonFail.cbJettonArea, cbJettonArea, sizeof(PlaceJettonFail.cbJettonArea));
		_sntprintf(PlaceJettonFail.szFailReason, sizeof(PlaceJettonFail.szFailReason), TEXT("����עʱ�䣬������ע��"));

		//������Ϣ
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceJettonFail, sizeof(PlaceJettonFail));
		return true;
	}

	//��������
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	SCORE lJettonCount = 0;
	SCORE lJettonPlace = 0;
	for (int nAreaIndex = 0; nAreaIndex < AREA_COUNT; ++nAreaIndex)
	{
		lJettonCount += m_lUserJettonScore[wChairID][nAreaIndex];
	}

	//��һ���
	SCORE lUserScore = pIServerUserItem->GetUserScore();

	//�Ϸ�У��
	if(lUserScore < lJettonCount + lJettonScore)
	{
		CMD_S_PlaceJetton_Fail PlaceJettonFail;
		ZeroMemory(&PlaceJettonFail, sizeof(PlaceJettonFail));

		CopyMemory(PlaceJettonFail.cbJettonArea, cbJettonArea, sizeof(PlaceJettonFail.cbJettonArea));
		_sntprintf(PlaceJettonFail.szFailReason, sizeof(PlaceJettonFail.szFailReason), TEXT("��Ҳ��㣬������ע��"));

		//������Ϣ
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceJettonFail, sizeof(PlaceJettonFail));
		return true;
	}
	if(m_lUserLimitScore < lJettonCount + lJettonScore)
	{
		CMD_S_PlaceJetton_Fail PlaceJettonFail;
		ZeroMemory(&PlaceJettonFail, sizeof(PlaceJettonFail));

		CopyMemory(PlaceJettonFail.cbJettonArea, cbJettonArea, sizeof(PlaceJettonFail.cbJettonArea));
		_sntprintf(PlaceJettonFail.szFailReason, sizeof(PlaceJettonFail.szFailReason), TEXT("��ע�����Ѵﵽ���ƣ�������ע��"));

		//������Ϣ
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceJettonFail, sizeof(PlaceJettonFail));
		return true;
	}

	//�ɹ���ʶ
	bool bPlaceJettonSuccess=true;

	//�Ϸ���֤
	if (GetUserMaxJetton(wChairID, cbJettonArea, cbJettonType) >= lJettonScore)
	{
		BYTE cbBetArea[6] = {0};
		FillMemory(cbBetArea, sizeof(cbBetArea),255);
		BYTE cbBetCount = 0;

		SCORE cbAreaScore = lJettonScore;

		if (cbJettonType == BET_TYPE_STRAIGHT)
		{
			cbBetArea[cbBetCount++] = cbJettonArea[0];

			m_lUserTypeScore[wChairID][0] += lJettonScore;
		}
		else if (cbJettonType == BET_TYPE_SPLIT)
		{
			cbBetArea[cbBetCount++] = cbJettonArea[0];
			cbBetArea[cbBetCount++] = cbJettonArea[1];
			cbAreaScore = cbAreaScore / 2;

			m_lUserTypeScore[wChairID][1] += lJettonScore;
		}
		else if (cbJettonType == BET_TYPE_STREET || cbJettonType == BET_TYPE_THREE)
		{
			cbBetArea[cbBetCount++] = cbJettonArea[0];
			cbBetArea[cbBetCount++] = cbJettonArea[1];
			cbBetArea[cbBetCount++] = cbJettonArea[2];
			cbAreaScore = cbAreaScore / 3;

			m_lUserTypeScore[wChairID][2] += lJettonScore;
		}
		else if (cbJettonType == BET_TYPE_CORNER || cbJettonType == BET_TYPE_FOUR)
		{
			cbBetArea[cbBetCount++] = cbJettonArea[0];
			cbBetArea[cbBetCount++] = cbJettonArea[1];
			cbBetArea[cbBetCount++] = cbJettonArea[2];
			cbBetArea[cbBetCount++] = cbJettonArea[3];
			cbAreaScore = cbAreaScore / 4;

			m_lUserTypeScore[wChairID][3] += lJettonScore;
		}
		else if (cbJettonType == BET_TYPE_LINE)
		{
			cbBetArea[cbBetCount++] = cbJettonArea[0];
			cbBetArea[cbBetCount++] = cbJettonArea[1];
			cbBetArea[cbBetCount++] = cbJettonArea[2];
			cbBetArea[cbBetCount++] = cbJettonArea[3];
			cbBetArea[cbBetCount++] = cbJettonArea[4];
			cbBetArea[cbBetCount++] = cbJettonArea[5];
			cbAreaScore = cbAreaScore / 6;

			m_lUserTypeScore[wChairID][4] += lJettonScore;
		}
		else if (cbJettonType == BET_TYPE_COLUMN)
		{
			cbBetArea[cbBetCount++] = COLUMN_1;
			if (cbJettonArea[0] == m_nAreaNums[COLUMN_2][0])
				cbBetArea[0] = COLUMN_2;
			else if (cbJettonArea[0] == m_nAreaNums[COLUMN_3][0])
				cbBetArea[0] = COLUMN_3;

			m_lUserTypeScore[wChairID][5] += lJettonScore;
		}
		else if (cbJettonType == BET_TYPE_DOZEN)
		{
			cbBetArea[cbBetCount++] = DOZEN_1;
			if (cbJettonArea[0] == m_nAreaNums[DOZEN_2][0])
				cbBetArea[0] = DOZEN_2;
			else if (cbJettonArea[0] == m_nAreaNums[DOZEN_3][0])
				cbBetArea[0] = DOZEN_3;

			m_lUserTypeScore[wChairID][5] += lJettonScore;
		}
		else if (cbJettonType == BET_TYPE_BLACK)
		{
			cbBetArea[cbBetCount++] = BLACK_1;

			m_lUserTypeScore[wChairID][6] += lJettonScore;
		}
		else if (cbJettonType == BET_TYPE_RED)
		{
			cbBetArea[cbBetCount++] = RED_1;

			m_lUserTypeScore[wChairID][6] += lJettonScore;
		}
		else if (cbJettonType == BET_TYPE_ODD)
		{
			cbBetArea[cbBetCount++] = ODD_1;
			m_lUserTypeScore[wChairID][6] += lJettonScore;
		}
		else if (cbJettonType == BET_TYPE_EVEN)
		{
			cbBetArea[cbBetCount++] = EVEN_1;
			m_lUserTypeScore[wChairID][6] += lJettonScore;
		}
		else if (cbJettonType == BET_TYPE_LOW)
		{
			cbBetArea[cbBetCount++] = LOW_1;
			m_lUserTypeScore[wChairID][6] += lJettonScore;
		}
		else if (cbJettonType == BET_TYPE_HI)
		{
			cbBetArea[cbBetCount++] = HI_1;
			m_lUserTypeScore[wChairID][6] += lJettonScore;
		}

		for (int i = 0; i < cbBetCount; i++)
		{
			//ȫ����ע
			m_lAllJettonScore[cbBetArea[i]] += cbAreaScore;
			//������ע
			m_lUserJettonScore[wChairID][cbBetArea[i]] += cbAreaScore;
		}
	}
	else
	{
		bPlaceJettonSuccess=false;
	}

	if(bPlaceJettonSuccess)
	{
		//��������
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

		PlaceJetton.cbJettonType = cbJettonType;
		CopyMemory(PlaceJetton.cbJettonArea, cbJettonArea, sizeof(PlaceJetton.cbJettonArea));
		PlaceJetton.lJettonScore=lJettonScore;
		PlaceJetton.wChairID = wChairID;
		PlaceJetton.nKeys = nKeys;
		for (WORD i = 0; i < GAME_PLAYER; i++)
		{
			IServerUserItem *pUser = m_pITableFrame->GetTableUserItem(i);
			if (pUser == NULL)
				continue;

			if (i == wChairID)
			{
				SCORE lAllBet = 0;
				for (BYTE j = 0; j < AREA_COUNT; j++)
				{
					lAllBet += m_lUserJettonScore[i][j];
				}

				PlaceJetton.lUserRestScore = pUser->GetUserScore() - lAllBet;
			}

			//������Ϣ
			m_pITableFrame->SendTableData(i, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
			m_pITableFrame->SendLookonData(i, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));

			PlaceJetton.lUserRestScore = 0;
		}
	}
	else
	{
		CMD_S_PlaceJetton_Fail PlaceJettonFail;
		ZeroMemory(&PlaceJettonFail, sizeof(PlaceJettonFail));

		CopyMemory(PlaceJettonFail.cbJettonArea, cbJettonArea, sizeof(PlaceJettonFail.cbJettonArea));
		_sntprintf(PlaceJettonFail.szFailReason, sizeof(PlaceJettonFail.szFailReason), TEXT("��ע�����Ѵﵽ���ƣ�������ע��"));

		//������Ϣ
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceJettonFail, sizeof(PlaceJettonFail));
		return true;
	}

	return true;
}


//�����˿�
void CTableFrameSink::DispatchTableCard()
{
  //����˿�
  m_cbTableCard = 0;

  //��ȡ�˿�
  m_cbTableCard = rand() % 37;


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

//�û�����
bool  CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) 
{
	return true;
}

//�����ע
SCORE CTableFrameSink::GetUserMaxJetton(WORD wChairID, BYTE Area[], BYTE cbType)
{
	IServerUserItem *pIMeServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	if(NULL==pIMeServerUserItem)
	{
		return 0L;
	}

	//����ע��
	SCORE lNowJetton = 0;
	for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
	{
		lNowJetton += m_lUserJettonScore[wChairID][nAreaIndex];
	}

	//��������
	SCORE lMeMaxScore = min((pIMeServerUserItem->GetUserScore() - lNowJetton), m_lUserLimitScore);

	//��������
	lMeMaxScore=min(lMeMaxScore,m_lAreaLimitScore);

	//��������
	if (cbType == BET_TYPE_STRAIGHT)
		lMeMaxScore = min(lMeMaxScore, m_lTypeLimitScore[0] - m_lUserTypeScore[wChairID][0]);
	else if (cbType == BET_TYPE_SPLIT)
		lMeMaxScore = min(lMeMaxScore, m_lTypeLimitScore[1] - m_lUserTypeScore[wChairID][1]);
	else if (cbType == BET_TYPE_STREET || cbType == BET_TYPE_THREE)
		lMeMaxScore = min(lMeMaxScore, m_lTypeLimitScore[2] - m_lUserTypeScore[wChairID][2]);
	else if (cbType == BET_TYPE_CORNER || cbType == BET_TYPE_FOUR)
		lMeMaxScore = min(lMeMaxScore, m_lTypeLimitScore[3] - m_lUserTypeScore[wChairID][3]);
	else if (cbType == BET_TYPE_LINE)
		lMeMaxScore = min(lMeMaxScore, m_lTypeLimitScore[4] - m_lUserTypeScore[wChairID][4]);
	else if (cbType == BET_TYPE_COLUMN || cbType == BET_TYPE_DOZEN)
		lMeMaxScore = min(lMeMaxScore, m_lTypeLimitScore[5] - m_lUserTypeScore[wChairID][5]);
	else if (cbType >= BET_TYPE_BLACK && cbType <= BET_TYPE_HI)
		lMeMaxScore = min(lMeMaxScore, m_lTypeLimitScore[6] - m_lUserTypeScore[wChairID][6]);

	//��������
	lMeMaxScore = max(lMeMaxScore, 0);

	return (SCORE)(lMeMaxScore);
}
//����÷�
SCORE CTableFrameSink::CalculateScore()
{	
    //��Ϸ��¼
    tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.cbResult = m_cbTableCard;

    //�ƶ��±�
    m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
    if(m_nRecordLast == m_nRecordFirst)
    {
      m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
    }

	BYTE cbWinArea[AREA_COUNT] = {FALSE};
	cbGetWinArea(m_cbTableCard,cbWinArea);

    //��ҳɼ�
    ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
    ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	SCORE lUserLostScore[GAME_PLAYER];
    ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	SCORE lUserWinScore = 0;

    //�������
    for(WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
    {
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if(pIServerUserItem==NULL)
		{
			continue;
		}

		for (WORD wAreaIndex = 0; wAreaIndex < AREA_COUNT; ++wAreaIndex)
		{
			if (m_lUserJettonScore[wChairID][wAreaIndex] > 0)
			{
				if (cbWinArea[wAreaIndex] == TRUE)
				{
					SCORE Score = m_lUserJettonScore[wChairID][wAreaIndex] * cbGetAreaTimes(wAreaIndex);

					m_lUserWinScore[wChairID] += Score;
					m_lPlayScore[wChairID][wAreaIndex] += Score;
				}
				else
				{
					m_lUserWinScore[wChairID] -= m_lUserJettonScore[wChairID][wAreaIndex];

					m_lPlayScore[wChairID][wAreaIndex] -= m_lUserJettonScore[wChairID][wAreaIndex];
				}
			}
		}
     
    }

	//Ҫ����Ӯ�ְ�1ȡ������Ϊ��ע�����������Ĵ���
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		if (m_lUserWinScore[i] != 0)
		{
			SCORE lRestScore = m_lUserWinScore[i];
			LONGLONG nRestScore = m_lUserWinScore[i];
			lRestScore = lRestScore - nRestScore;
			if (lRestScore > 0)
			{
				m_lUserWinScore[i] += 1;
				m_lUserWinScore[i] -= lRestScore;
			}
		}

		for (WORD wAreaIndex = 0; wAreaIndex < AREA_COUNT; ++wAreaIndex)
		{
			if (m_lPlayScore[i][wAreaIndex] != 0)
			{
				SCORE lRestScore = m_lPlayScore[i][wAreaIndex];
				LONGLONG nRestScore = m_lPlayScore[i][wAreaIndex];
				lRestScore = lRestScore - nRestScore;
				if (lRestScore > 0)
				{
					m_lPlayScore[i][wAreaIndex] += 1;
					m_lPlayScore[i][wAreaIndex] -= lRestScore;
				}
			}
		}
	}

    //����˰��
    for(WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
    {
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if(pIServerUserItem==NULL)
		{
			continue;
		}
		if(m_lUserWinScore[wChairID] > 0)
		{
			SCORE Revenue = m_pITableFrame->CalculateRevenue(wChairID,m_lUserWinScore[wChairID]);
			m_lUserRevenue[wChairID] = Revenue;
			m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];

		}
    }
	return 0;
}

//���ͼ�¼
void CTableFrameSink::SendGameRecord(IServerUserItem *pIServerUserItem)
{
	WORD wBufferSize=0;
	BYTE cbBuffer[SOCKET_TCP_BUFFER];
	int nIndex = m_nRecordFirst;
	while ( nIndex != m_nRecordLast )
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
	if(wBufferSize>0)
	{
		m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
	}
}

//��ȡ����
void CTableFrameSink::ReadConfigInformation()
{
	//��ȡ�Զ�������
	tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pGameServiceOption->cbCustomRule;
	ASSERT(pCustomConfig);

	//ʱ��
	m_cbBetTime = pCustomConfig->CustomGeneral.cbBetTime;
	m_cbEndTime = pCustomConfig->CustomGeneral.cbEndTime;
	if(m_cbBetTime < TIME_PLACE_JETTON || m_cbBetTime > 99)
	{
		m_cbBetTime = TIME_PLACE_JETTON;
	}
	if(m_cbEndTime < TIME_GAME_END || m_cbEndTime > 99)
	{
		m_cbEndTime = TIME_GAME_END;
	}

	//��ע
	m_lAreaLimitScore = pCustomConfig->CustomGeneral.lAreaLimitScore;
	m_lUserLimitScore = pCustomConfig->CustomGeneral.lUserLimitScore;
}

//��ȡʤ������
BYTE CTableFrameSink::cbGetWinArea(BYTE cbTableCard, BYTE cbWinArea[])
{
	for (int i = 0; i < AREA_COUNT; i++)
	{
		for (int j = 0; j < 18; j++)
		{
			if (m_nAreaNums[i][j] == cbTableCard)
			{
				cbWinArea[i] = TRUE;
				break;
			}
			else if (m_nAreaNums[i][j] == 255)
				break;
		}
	}

	return 0;
}

//��ȡ������
BYTE CTableFrameSink::cbGetAreaTimes(BYTE cbArea)
{
	if (cbArea >= STRAIGHT_0 && cbArea <= STRAIGHT_36)
		return 35;
	else if (cbArea >= COLUMN_1 && cbArea <= DOZEN_3)
		return 2;
	else
		return 1;
}

//��ʼ����������
void CTableFrameSink::InitAreaNums()
{
	for (int i = STRAIGHT_0; i <= STRAIGHT_36; i++)
	{
		m_nAreaNums[i][0] = i;
		for (int j = 1; j < 18; j++)
		{
			m_nAreaNums[i][j] = 255;
		}
	}

	for (int i = COLUMN_1; i <= COLUMN_3; i++)
	{
		m_nAreaNums[i][0] = (i - COLUMN_1 + 1);
		m_nAreaNums[i][1] = (i - COLUMN_1 + 1) + 3;
		m_nAreaNums[i][2] = (i - COLUMN_1 + 1) + 6;
		m_nAreaNums[i][3] = (i - COLUMN_1 + 1) + 9;
		m_nAreaNums[i][4] = (i - COLUMN_1 + 1) + 12;
		m_nAreaNums[i][5] = (i - COLUMN_1 + 1) + 15;
		m_nAreaNums[i][6] = (i - COLUMN_1 + 1) + 18;
		m_nAreaNums[i][7] = (i - COLUMN_1 + 1) + 21;
		m_nAreaNums[i][8] = (i - COLUMN_1 + 1) + 24;
		m_nAreaNums[i][9] = (i - COLUMN_1 + 1) + 27;
		m_nAreaNums[i][10] = (i - COLUMN_1 + 1) + 30;
		m_nAreaNums[i][11] = (i - COLUMN_1 + 1) + 33;
		for (int j = 12; j < 18; j++)
		{
			m_nAreaNums[i][j] = 255;
		}
	}

	for (int i = DOZEN_1; i <= DOZEN_3; i++)
	{
		m_nAreaNums[i][0] = (i - DOZEN_1) * 12 + 1;
		m_nAreaNums[i][1] = (i - DOZEN_1) * 12 + 2;
		m_nAreaNums[i][2] = (i - DOZEN_1) * 12 + 3;
		m_nAreaNums[i][3] = (i - DOZEN_1) * 12 + 4;
		m_nAreaNums[i][4] = (i - DOZEN_1) * 12 + 5;
		m_nAreaNums[i][5] = (i - DOZEN_1) * 12 + 6;
		m_nAreaNums[i][6] = (i - DOZEN_1) * 12 + 7;
		m_nAreaNums[i][7] = (i - DOZEN_1) * 12 + 8;
		m_nAreaNums[i][8] = (i - DOZEN_1) * 12 + 9;
		m_nAreaNums[i][9] = (i - DOZEN_1) * 12 + 10;
		m_nAreaNums[i][10] = (i - DOZEN_1) * 12 + 11;
		m_nAreaNums[i][11] = (i - DOZEN_1) * 12 + 12;
		for (int j = 12; j < 18; j++)
		{
			m_nAreaNums[i][j] = 255;
		}
	}

	m_nAreaNums[BLACK_1][0] = 2;
	m_nAreaNums[BLACK_1][1] = 4;
	m_nAreaNums[BLACK_1][2] = 6;
	m_nAreaNums[BLACK_1][3] = 8;
	m_nAreaNums[BLACK_1][4] = 10;
	m_nAreaNums[BLACK_1][5] = 11;
	m_nAreaNums[BLACK_1][6] = 13;
	m_nAreaNums[BLACK_1][7] = 15;
	m_nAreaNums[BLACK_1][8] = 17;
	m_nAreaNums[BLACK_1][9] = 20;
	m_nAreaNums[BLACK_1][10] = 22;
	m_nAreaNums[BLACK_1][11] = 24;
	m_nAreaNums[BLACK_1][12] = 26;
	m_nAreaNums[BLACK_1][13] = 28;
	m_nAreaNums[BLACK_1][14] = 29;
	m_nAreaNums[BLACK_1][15] = 31;
	m_nAreaNums[BLACK_1][16] = 33;
	m_nAreaNums[BLACK_1][17] = 35;

	m_nAreaNums[RED_1][0] = 1;
	m_nAreaNums[RED_1][1] = 3;
	m_nAreaNums[RED_1][2] = 5;
	m_nAreaNums[RED_1][3] = 7;
	m_nAreaNums[RED_1][4] = 9;
	m_nAreaNums[RED_1][5] = 12;
	m_nAreaNums[RED_1][6] = 14;
	m_nAreaNums[RED_1][7] = 16;
	m_nAreaNums[RED_1][8] = 18;
	m_nAreaNums[RED_1][9] = 19;
	m_nAreaNums[RED_1][10] = 21;
	m_nAreaNums[RED_1][11] = 23;
	m_nAreaNums[RED_1][12] = 25;
	m_nAreaNums[RED_1][13] = 27;
	m_nAreaNums[RED_1][14] = 30;
	m_nAreaNums[RED_1][15] = 32;
	m_nAreaNums[RED_1][16] = 34;
	m_nAreaNums[RED_1][17] = 36;

	m_nAreaNums[ODD_1][0] = 1;
	m_nAreaNums[ODD_1][1] = 3;
	m_nAreaNums[ODD_1][2] = 5;
	m_nAreaNums[ODD_1][3] = 7;
	m_nAreaNums[ODD_1][4] = 9;
	m_nAreaNums[ODD_1][5] = 11;
	m_nAreaNums[ODD_1][6] = 13;
	m_nAreaNums[ODD_1][7] = 15;
	m_nAreaNums[ODD_1][8] = 17;
	m_nAreaNums[ODD_1][9] = 19;
	m_nAreaNums[ODD_1][10] = 21;
	m_nAreaNums[ODD_1][11] = 23;
	m_nAreaNums[ODD_1][12] = 25;
	m_nAreaNums[ODD_1][13] = 27;
	m_nAreaNums[ODD_1][14] = 29;
	m_nAreaNums[ODD_1][15] = 31;
	m_nAreaNums[ODD_1][16] = 33;
	m_nAreaNums[ODD_1][17] = 35;

	m_nAreaNums[EVEN_1][0] = 2;
	m_nAreaNums[EVEN_1][1] = 4;
	m_nAreaNums[EVEN_1][2] = 6;
	m_nAreaNums[EVEN_1][3] = 8;
	m_nAreaNums[EVEN_1][4] = 10;
	m_nAreaNums[EVEN_1][5] = 12;
	m_nAreaNums[EVEN_1][6] = 14;
	m_nAreaNums[EVEN_1][7] = 16;
	m_nAreaNums[EVEN_1][8] = 18;
	m_nAreaNums[EVEN_1][9] = 20;
	m_nAreaNums[EVEN_1][10] = 22;
	m_nAreaNums[EVEN_1][11] = 24;
	m_nAreaNums[EVEN_1][12] = 26;
	m_nAreaNums[EVEN_1][13] = 28;
	m_nAreaNums[EVEN_1][14] = 30;
	m_nAreaNums[EVEN_1][15] = 32;
	m_nAreaNums[EVEN_1][16] = 34;
	m_nAreaNums[EVEN_1][17] = 36;

	m_nAreaNums[LOW_1][0] = 1;
	m_nAreaNums[LOW_1][1] = 2;
	m_nAreaNums[LOW_1][2] = 3;
	m_nAreaNums[LOW_1][3] = 4;
	m_nAreaNums[LOW_1][4] = 5;
	m_nAreaNums[LOW_1][5] = 6;
	m_nAreaNums[LOW_1][6] = 7;
	m_nAreaNums[LOW_1][7] = 8;
	m_nAreaNums[LOW_1][8] = 9;
	m_nAreaNums[LOW_1][9] = 10;
	m_nAreaNums[LOW_1][10] = 11;
	m_nAreaNums[LOW_1][11] = 12;
	m_nAreaNums[LOW_1][12] = 13;
	m_nAreaNums[LOW_1][13] = 14;
	m_nAreaNums[LOW_1][14] = 15;
	m_nAreaNums[LOW_1][15] = 16;
	m_nAreaNums[LOW_1][16] = 17;
	m_nAreaNums[LOW_1][17] = 18;

	m_nAreaNums[HI_1][0] = 19;
	m_nAreaNums[HI_1][1] = 20;
	m_nAreaNums[HI_1][2] = 21;
	m_nAreaNums[HI_1][3] = 22;
	m_nAreaNums[HI_1][4] = 23;
	m_nAreaNums[HI_1][5] = 24;
	m_nAreaNums[HI_1][6] = 25;
	m_nAreaNums[HI_1][7] = 26;
	m_nAreaNums[HI_1][8] = 27;
	m_nAreaNums[HI_1][9] = 28;
	m_nAreaNums[HI_1][10] = 29;
	m_nAreaNums[HI_1][11] = 30;
	m_nAreaNums[HI_1][12] = 31;
	m_nAreaNums[HI_1][13] = 32;
	m_nAreaNums[HI_1][14] = 33;
	m_nAreaNums[HI_1][15] = 34;
	m_nAreaNums[HI_1][16] = 35;
	m_nAreaNums[HI_1][17] = 36;
}