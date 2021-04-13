#include "StdAfx.h"
#include "TableFrameSink.h"
#include "DlgCustomRule.h"
#include <conio.h>
#include <locale>

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

#define	IDI_SO_OPERATE							1							//����ʱ��

#define IDI_WAIT_START							2							//�ȴ���ʼ

#define IDI_AUTO_START							3							//�Զ���ʼ
//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	m_pGameServiceOption = NULL;
	m_pGameServiceAttrib = NULL;

	//��Ϸ����	
	m_wPlayerCount=GAME_PLAYER;	

	m_lExitScore=0;	

	m_wBankerUser=INVALID_CHAIR;

	//�û�״̬
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	FillMemory(m_cbCallBankerTimes,sizeof(m_cbCallBankerTimes),0xFF);
	m_cbMaxBankerTimes = 0;
	ZeroMemory(m_bFleeUser, sizeof(m_bFleeUser));

	m_bWriteRoomStorage = true;

	//�˿˱���
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//�������
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

//��������
CTableFrameSink::~CTableFrameSink(void)
{

}

//�ӿڲ�ѯ--��������Ϣ�汾
void * CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
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

	m_pITableFrame->SetStartMode(START_MODE_ALL_READY);

	//��Ϸ����
	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	
	m_lAddChip[0] = 5;
	m_lAddChip[1] = 10;
	m_lAddChip[2] = 15;
	m_lAddChip[3] = 20;

	//��ȡ�Զ�������
	tagCustomRule *pCustomRule = (tagCustomRule *)m_pGameServiceOption->cbCustomRule;

	for (int i = 0; i < MAX_CONFIG; i++)
	{
		if (pCustomRule->lFreeConfig[i] != 0)
			m_lAddChip[i] = pCustomRule->lFreeConfig[i];
	}

	//��ȡ����
	ReadConfigInformation();	

	m_dwOperateTime = (DWORD)time(NULL);


	return true;
}

//��λ����
void CTableFrameSink::RepositionSink()
{
	//��Ϸ����
	m_lExitScore=0;	
	m_wBankerUser = INVALID_CHAIR;

	//�û�״̬
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	FillMemory(m_cbCallBankerTimes, sizeof(m_cbCallBankerTimes), 0xFF);
	m_cbMaxBankerTimes = 0;

	//�˿˱���
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	m_bWriteRoomStorage = true;

	return;
}

//�û�����
bool CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{

	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	return true;
}

//�û�ͬ��
bool CTableFrameSink::OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{
	return true;
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{
	ZeroMemory(m_bFleeUser, sizeof(m_bFleeUser));

	CMD_S_GameStart GameStart;

	GameStart.cbTime = m_cbStartTime;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_START, &GameStart, sizeof(GameStart));

	m_pITableFrame->SetGameTimer(IDI_AUTO_START, m_cbStartTime*1000, 1, 0L);
	
	m_dwOperateTime = (DWORD)time(NULL);

	//����״̬
	m_pITableFrame->SetGameStatus(GS_TK_WAIT);

	return true;
}

//�Զ���ʼ
bool CTableFrameSink::OnGameAutoStart()
{
	BYTE cbAiCount = 0;
	BYTE cbPlayCount = 0;

	//�û�״̬
	for (WORD i = 0; i<m_wPlayerCount; i++)
	{
		//��ȡ�û�
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

	//���˥��
	if (m_bWriteRoomStorage)
		m_pITableFrame->CalculateStorageDeduct();

	//����˿�
	BYTE bTempArray[GAME_PLAYER*MAX_CARDCOUNT];
	m_GameLogic.RandCardList(bTempArray, sizeof(bTempArray));

	for (WORD i = 0; i<m_wPlayerCount; i++)
	{
		IServerUserItem *pIServerUser = m_pITableFrame->GetTableUserItem(i);
		if (pIServerUser == NULL)
		{
			continue;
		}

		//�ɷ��˿�
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

//��Ϸ����
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
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			//ɾ��ʱ��
			m_pITableFrame->KillGameTimer(IDI_SO_OPERATE);

			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd, sizeof(GameEnd));

			GameEnd.cbFleeEnd = TRUE;

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			m_pITableFrame->SetGameTimer(IDI_WAIT_START, cbEndTime * 1000, 1, 0L);

			m_pITableFrame->SetGameStatus(GS_TK_END);

			m_dwOperateTime = (DWORD)time(NULL);

			m_wBankerUser = INVALID_CHAIR;

			return true;
		}
	case GER_NORMAL:		//�������
		{
			//ɾ��ʱ��
			m_pITableFrame->KillGameTimer(IDI_SO_OPERATE);

			//������
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

			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			WORD wWinTimes[GAME_PLAYER];
			WORD wWinCount[GAME_PLAYER];
			ZeroMemory(wWinCount,sizeof(wWinCount));
			ZeroMemory(wWinTimes,sizeof(wWinTimes));

			//�����˿�
			BYTE cbUserCardData[GAME_PLAYER][MAX_CARDCOUNT] = {0};
			CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));
			
			//ׯ�ұ���
			wWinTimes[m_wBankerUser] = m_GameLogic.GetTimes(cbUserCardData[m_wBankerUser], MAX_CARDCOUNT);

			//�Ա����
			for (WORD i = 0; i < m_wPlayerCount; i++)
			{
				if (i == m_wBankerUser || m_cbPlayStatus[i] == FALSE)continue;

				//�Ա��˿�
				if (m_GameLogic.CompareCard(cbUserCardData[i], cbUserCardData[m_wBankerUser], MAX_CARDCOUNT))
				{
					wWinCount[i]++;
					//��ȡ����
					wWinTimes[i] = m_GameLogic.GetTimes(cbUserCardData[i], MAX_CARDCOUNT);
				}
				else
				{
					wWinCount[m_wBankerUser]++;
				}
			}

			//ͳ�Ƶ÷�
			for (WORD i = 0; i < m_wPlayerCount; i++)
			{
				if (i == m_wBankerUser || m_cbPlayStatus[i] == FALSE)continue;

				if (wWinCount[i] > 0)	//�м�ʤ��
				{
					GameEnd.lGameScore[i] = m_lTableScore[i] * wWinTimes[i] * m_pITableFrame->GetCellScore() * m_cbMaxBankerTimes;
					GameEnd.lGameScore[m_wBankerUser] -= GameEnd.lGameScore[i];
					m_lTableScore[i] = 0;
				}
				else					//ׯ��ʤ��
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

			//�м�ǿ�˷���	
			GameEnd.lGameScore[m_wBankerUser] += m_lExitScore;

			//�뿪�û�
			for (WORD i = 0; i < m_wPlayerCount; i++)
			{
				if (m_lTableScore[i] > 0)
					GameEnd.lGameScore[i] = -m_lTableScore[i];
			}

			//ׯ��Ǯ�����⣬�Ͱ�Ӯ�˵��˵ı�����ׯ�ҵ�����Ǯ
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
			
			//�޸Ļ���
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));

			//����˰��
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

				//�������
				ScoreInfoArray[i].lScore = GameEnd.lGameScore[i];
				ScoreInfoArray[i].lRevenue = lRevenue;
				ScoreInfoArray[i].cbType = (GameEnd.lGameScore[i]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;

				IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);
				if (pServerUser)
				{
					GameEnd.lPlayerScore[i] = pServerUser->GetUserScore() + GameEnd.lGameScore[i];

					//���ֵ
					if (pServerUser->IsAndroidUser() && m_bWriteRoomStorage)
						m_pGameServiceOption->pRoomStorageOption.lStorageCurrent += GameEnd.lGameScore[i];
				}
			}
		
			CopyMemory(GameEnd.cbHandCardData, m_cbHandCardData, sizeof(m_cbHandCardData));

			//��ȡ�������			
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

			//�����������Ϣ
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
	case GER_USER_LEAVE:		//�û�ǿ��
	case GER_NETWORK_ERROR:
		{
			//Ч�����
			ASSERT(pIServerUserItem!=NULL);

			if(m_cbPlayStatus[wChairID]==FALSE) 
				return true;
			
			//��ע����
			if (m_pITableFrame->GetGameStatus() == GS_TK_SCORE)
			{
				if (m_lTableScore[wChairID] == 0 && wChairID != m_wBankerUser)
				{
					OnUserAddScore(wChairID,m_lAddChip[0]);
				}
				return true;
			}
			//��ׯ״̬����
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

//���ͳ���
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:		//����״̬
	case GS_TK_WAIT:
	case GS_TK_END:
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//���ñ���
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

			//��ȡ�Զ�������
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

			//���ͳ���
			return bStatus;
		}
	case GS_TK_CALL:	//��ׯ״̬
		{
			//��������
			CMD_S_StatusCall StatusCall;
			ZeroMemory(&StatusCall,sizeof(StatusCall));

			StatusCall.lCellScore = m_pITableFrame->GetCellScore();

			//���ñ���
			CopyMemory(StatusCall.cbPlayerStatus, m_cbPlayStatus, sizeof(StatusCall.cbPlayerStatus));
			
			StatusCall.cbStartTime = m_cbStartTime;
			StatusCall.cbCallTime = m_cbCallTime;
			StatusCall.cbAddTime = m_cbAddTime;
			StatusCall.cbEndTime = m_cbEndTime;

			DWORD dwPassTime = (DWORD)time(NULL) - m_dwOperateTime;
			StatusCall.cbTimeLeave = (BYTE)(m_cbCallTime - __min(dwPassTime, (DWORD)m_cbCallTime));
			
			CopyMemory(StatusCall.cbCallBankerTimes, m_cbCallBankerTimes, sizeof(StatusCall.cbCallBankerTimes));
			//�ɷ��˿�(��ʼֻ��������)
			CopyMemory(StatusCall.cbCardData[wChairID], m_cbHandCardData[wChairID], sizeof(BYTE) * 3);

			bool bStatus = m_pITableFrame->SendGameScene(pIServerUserItem, &StatusCall, sizeof(StatusCall));

			//��ȡ�Զ�������
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

			//���ͳ���
			return bStatus;
		}
	case GS_TK_SCORE:	//��ע״̬
		{
			//��������
			CMD_S_StatusScore StatusScore;
			ZeroMemory(&StatusScore,sizeof(StatusScore));

			StatusScore.lCellScore = m_pITableFrame->GetCellScore();

			//��ע��Ϣ
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
			//�ɷ��˿�(��ʼֻ��������)
			CopyMemory(StatusScore.cbCardData[wChairID], m_cbHandCardData[wChairID], sizeof(BYTE) * 3);

			bool bStatus = m_pITableFrame->SendGameScene(pIServerUserItem, &StatusScore, sizeof(StatusScore));

			//��ȡ�Զ�������
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

			//���ͳ���
			return bStatus;
		}
	}
	//Ч�����
	ASSERT(FALSE);

	return false;
}

//��ʱ���¼�
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
			//ɾ��ʱ��
			m_pITableFrame->KillGameTimer(IDI_AUTO_START);
			OnGameAutoStart();
			return true;
		}
	case IDI_SO_OPERATE:
		{
			//ɾ��ʱ��
			m_pITableFrame->KillGameTimer(IDI_SO_OPERATE);

			//��Ϸ״̬
			switch( m_pITableFrame->GetGameStatus() )
			{
				case GS_TK_CALL:			//�û���ׯ
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
							//����״̬
							m_bFleeUser[i] ++;
						}

						break;
					}
				case GS_TK_SCORE:			//��ע����
					{
						for(WORD i=0;i<m_wPlayerCount;i++)
						{
							if(m_lTableScore[i]>0L || m_cbPlayStatus[i]==FALSE || i==m_wBankerUser)
								continue;

							OnUserAddScore(i, m_lAddChip[0]);
							//����״̬
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

//��Ϸ��Ϣ���� 
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_CALL_BANKER:			//�û���ׯ
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_CallBanker));
			if (wDataSize != sizeof(CMD_C_CallBanker)) return true;

			//��������
			CMD_C_CallBanker * pCallBanker=(CMD_C_CallBanker *)pDataBuffer;

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

			//״̬�ж�
			ASSERT (m_cbPlayStatus[pUserData->wChairID] == TRUE);
			if (m_cbPlayStatus[pUserData->wChairID] != TRUE) 
			{
				return true;
			}

			//��Ϣ����
			return OnUserCallBanker(pUserData->wChairID, pCallBanker->cbBankerTimes);
		}
	case SUB_C_ADD_SCORE:			//�û���ע
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_AddScore));
			if (wDataSize!=sizeof(CMD_C_AddScore)) 
			{
				return true;
			}

			//��������
			CMD_C_AddScore * pAddScore=(CMD_C_AddScore *)pDataBuffer;

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

			//״̬�ж�
			ASSERT (m_cbPlayStatus[pUserData->wChairID] == TRUE);
			if (m_cbPlayStatus[pUserData->wChairID] != TRUE) 
			{
				return true;
			}

			//��Ϣ����
			return OnUserAddScore(pUserData->wChairID, pAddScore->nScore);
		}
	}

	return true;
}

//�����Ϣ����
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	
	return false;
}

//��ׯ�¼�
bool CTableFrameSink::OnUserCallBanker(WORD wChairID, BYTE cbBankerTimes)
{
	//״̬Ч��
	BYTE cbGameStatus = m_pITableFrame->GetGameStatus();
	ASSERT(cbGameStatus==GS_TK_CALL);
	if (cbGameStatus!=GS_TK_CALL) return true;

	if (cbBankerTimes != 0 && cbBankerTimes != 1 && cbBankerTimes != 2 && cbBankerTimes != 4)
		return true;

	//���ñ���
	m_cbCallBankerTimes[wChairID] = cbBankerTimes;

	//���ñ���
	CMD_S_CallBankerInfo CallBanker;
	ZeroMemory(&CallBanker, sizeof(CallBanker));

	CallBanker.wCallUser = wChairID;
	CallBanker.cbCallBankerTimes = cbBankerTimes;


	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CALL_BANKER, &CallBanker, sizeof(CallBanker));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CALL_BANKER, &CallBanker, sizeof(CallBanker));

	//��ׯ����
	WORD wCallUserCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbCallBankerTimes[i] != 0xFF) 
			wCallUserCount++;
		else if(m_cbPlayStatus[i]!=TRUE) 
			wCallUserCount++;
	}

	//ȫ���˽���ׯ����ע��ʼ
	if(wCallUserCount==m_wPlayerCount)
	{
		//��ׯ�����
		BYTE cbMaxBankerTimes = cbBankerTimes;
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_cbPlayStatus[i]==TRUE && m_cbCallBankerTimes[i] > cbMaxBankerTimes) 
			{
				cbMaxBankerTimes = m_cbCallBankerTimes[i];
			}
		}

		m_cbMaxBankerTimes = cbMaxBankerTimes;

		//��ׯ�������������CHAIRID
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

		//����״̬
		m_pITableFrame->SetGameStatus(GS_TK_SCORE);

		//���ñ���
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

//��ע�¼�
bool CTableFrameSink::OnUserAddScore(WORD wChairID, LONGLONG lScore)
{
	//״̬Ч��
	BYTE cbGameStatus = m_pITableFrame->GetGameStatus();
	ASSERT(cbGameStatus==GS_TK_SCORE);
	if (cbGameStatus!=GS_TK_SCORE) 
	{
		return true;
	}
	
	//ׯ��У��
	if (wChairID == m_wBankerUser)
	{
		return true;
	}

	//���Ч��
	if (lScore != m_lAddChip[0] && lScore != m_lAddChip[1] && lScore != m_lAddChip[2] && lScore != m_lAddChip[3])
	{
		return true;
	}

	//��ע���
	m_lTableScore[wChairID]=lScore;
	//��������
	CMD_S_AddScore AddScore;
	AddScore.wBetUser = wChairID;
	AddScore.nBetTimes = m_lTableScore[wChairID];

	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_PLAYER_BET, &AddScore, sizeof(AddScore));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_PLAYER_BET, &AddScore, sizeof(AddScore));

	//��ע����
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
	
	//�м�ȫ��
	if(bUserCount==m_wPlayerCount)
	{
		OnEventGameConclude(INVALID_CHAIR, NULL, GER_NORMAL);
	}

	return true;
}

bool CTableFrameSink::TryWriteTableScore(tagScoreInfo ScoreInfoArray[])
{
	//�޸Ļ���
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

//��ȡ������Ϸ�Ƿ���ע״̬���������ע�� ����true���ж�������
bool CTableFrameSink::OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if (m_cbPlayStatus[wChairID] == TRUE && (m_pITableFrame->GetGameStatus() == GS_TK_CALL || m_pITableFrame->GetGameStatus() == GS_TK_SCORE))
		return true;
	else
		return false;
}

//��ѯ�޶�
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	return 0L;
}

//��ȡ����
void CTableFrameSink::ReadConfigInformation()
{	
	//��ȡ�Զ�������
	tagCustomRule *pCustomRule = (tagCustomRule *)m_pGameServiceOption->cbCustomRule;
	ASSERT(pCustomRule);
	
}

//////////////////////////////////////////////////////////////////////////
