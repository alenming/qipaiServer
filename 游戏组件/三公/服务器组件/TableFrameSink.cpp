#include "StdAfx.h"
#include "TableFrameSink.h"
#include <conio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////

#define IDI_SO_OPERATE          1				//����ʱ��
#define TIME_SO_OVERTIME        30*1000			//��ʱʱ��

//������ʶ
#define IDI_DELAY_ENDGAME		2				//������ʶ
#define TIME_DELAY_END			3000			//��ʱʱ��

#define IDI_AUTO_START			3				//�Զ���ʼ
#define TIME_START				5000			//�Զ���ʼʱ��
//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	m_RealPlayers = 0;
	m_wPlayerNum = GAME_PLAYER;

	//��Ϸ����
	m_wBankerUser = INVALID_CHAIR;
	m_wCurrentUser = INVALID_CHAIR;

	//�û�״̬
	ZeroMemory(m_lCurrentScore, sizeof(m_lCurrentScore));
	ZeroMemory(m_lTableScore, sizeof(m_lTableScore));
	ZeroMemory(m_cbPlayStatus, sizeof(m_cbPlayStatus));
	ZeroMemory(m_cbPlayerVlue, sizeof(m_cbPlayerVlue));
	FillMemory(m_cbCallStatus, sizeof(m_cbCallStatus), 0xFF);

	m_dwPassedTime = 0;

	m_bWriteRoomStorage = true;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
}

//�ӿڲ�ѯ--��������Ϣ�汾
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
	if (m_pITableFrame == NULL)
	{
		return false;
	}

	m_pITableFrame->SetStartMode(START_MODE_ALL_READY);

	//��Ϸ����
	m_pGameServiceAttrib = m_pITableFrame->GetGameServiceAttrib();
	m_pGameServiceOption = m_pITableFrame->GetGameServiceOption();

	//�Զ�������
	tagCustomRule *pGameCustomRule = (tagCustomRule *)m_pITableFrame->GetCustomRule();
	ASSERT(pGameCustomRule != NULL);

	return true;
}

//��λ����
void CTableFrameSink::RepositionSink()
{
	//��Ϸ����
	m_wBankerUser = INVALID_BANKER;
	m_wCurrentUser = INVALID_CHAIR;
	m_RealPlayers = 0;
	ZeroMemory(m_lTableScore, sizeof(m_lTableScore));
	ZeroMemory(m_lCurrentScore, sizeof(m_lCurrentScore));
	ZeroMemory(m_cbHandCardData, sizeof(m_cbHandCardData));
	FillMemory(m_cbCallStatus, sizeof(m_cbCallStatus), 0xFF);

	m_bWriteRoomStorage = true;

	return;
}

//�û�����
bool CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem)
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

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{
	ZeroMemory(m_bFleeUser, sizeof(m_bFleeUser));
	ZeroMemory(m_cbPlayStatus, sizeof(m_cbPlayStatus));
	//����״̬
	m_pITableFrame->SetGameStatus(GS_TK_CALL);

	BYTE cbAiCount = 0;
	BYTE cbPlayCount = 0;

	//�û�״̬
	for (WORD i = 0; i < m_wPlayerNum; i++)
	{
		//��ȡ�û�
		IServerUserItem *pIServerUser = m_pITableFrame->GetTableUserItem(i);
		if (pIServerUser == NULL)
		{
			m_cbPlayStatus[i] = FALSE;
		}
		else
		{
			m_cbPlayStatus[i] = TRUE;
			m_RealPlayers++;

			cbPlayCount++;
			if (pIServerUser->IsAndroidUser())
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
	BYTE cbRandCard[FULL_COUNT];
	m_GameLogic.RandCardList(cbRandCard, FULL_COUNT);

	for (WORD i = 0; i < m_wPlayerNum; i++)
	{
		if (m_cbPlayStatus[i])
		{
			CopyMemory(m_cbHandCardData[i], &cbRandCard[i * MAX_COUNT], MAX_COUNT);
		}
	}	

	//���ñ���
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart, sizeof(GameStart));
	CopyMemory(GameStart.cbPlayStatus, m_cbPlayStatus, sizeof(GameStart.cbPlayStatus));

	//��������
	for (WORD i = 0; i < m_wPlayerNum; i++)
	{
		if (m_cbPlayStatus[i] != TRUE)
			continue;
		ZeroMemory(GameStart.cbCardData, sizeof(GameStart.cbCardData));
		IServerUserItem *pIServerUser = m_pITableFrame->GetTableUserItem(i);
		if (pIServerUser && pIServerUser->IsAndroidUser())
			CopyMemory(GameStart.cbCardData, m_cbHandCardData[i], MAX_COUNT);
		else
			CopyMemory(GameStart.cbCardData, m_cbHandCardData[i], MAX_COUNT-1);

		m_pITableFrame->SendTableData(i, SUB_S_GAME_START, &GameStart, sizeof(GameStart));
	}

	m_pITableFrame->KillGameTimer(IDI_SO_OPERATE);
	m_pITableFrame->SetGameTimer(IDI_SO_OPERATE, TIME_SO_OVERTIME, 1, 0);

	m_dwPassedTime = (DWORD)time(NULL);

	return true;
}


//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_DISMISS:   //��Ϸ��ɢ
	{
		//��������
		CMD_S_GameEnd GameEnd;
		ZeroMemory(&GameEnd, sizeof(GameEnd));

		CopyMemory(GameEnd.cbCardData, m_cbHandCardData, sizeof(GameEnd.cbCardData));
		CopyMemory(GameEnd.cbPlayerValue, m_cbPlayerVlue, sizeof(GameEnd.cbPlayerValue));
		CopyMemory(GameEnd.lGameScore, m_lCurrentScore, sizeof(GameEnd.lGameScore));

		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));

		m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);


		return true;
	}
	case GER_NORMAL:    //�������
	{
		//ɾ��ʱ��
		m_pITableFrame->KillGameTimer(IDI_SO_OPERATE);

		//������
		tag_SG_Parameter SG_Parameter;
		ZeroMemory(&SG_Parameter, sizeof(SG_Parameter));

		SG_Parameter.wBankerUser = m_wBankerUser;
		CopyMemory(SG_Parameter.cbPlayerStatus, m_cbPlayStatus, sizeof(SG_Parameter.cbPlayerStatus));
		CopyMemory(SG_Parameter.lTableScore, m_lTableScore, sizeof(SG_Parameter.lTableScore));
		CopyMemory(SG_Parameter.cbCallBankerTimes, m_cbCallStatus, sizeof(SG_Parameter.cbCallBankerTimes));
		CopyMemory(SG_Parameter.cbTableCardArray, m_cbHandCardData, sizeof(SG_Parameter.cbTableCardArray));

		tag_SG_Result pSG_Result;
		ZeroMemory(&pSG_Result, sizeof(pSG_Result));
		m_pITableFrame->GetControlResult(&SG_Parameter, sizeof(SG_Parameter), &pSG_Result);
		CopyMemory(m_cbHandCardData, pSG_Result.cbTableCardArray, sizeof(m_cbHandCardData));

		//�������
		CMD_S_GameEnd GameEnd;
		ZeroMemory(&GameEnd, sizeof(GameEnd));

		CopyMemory(GameEnd.cbPlayerStatus,m_cbPlayStatus,sizeof(GameEnd.cbPlayerStatus));

		//�����˿�
		BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
		CopyMemory(cbUserCardData, m_cbHandCardData, sizeof(cbUserCardData));
		CopyMemory(GameEnd.cbCardData, cbUserCardData, sizeof(GameEnd.cbCardData));

		//��ҵ���
		ZeroMemory(m_cbPlayerVlue, sizeof(m_cbPlayerVlue));
		for (WORD i = 0; i < m_wPlayerNum; i++)
		{
			if (!m_cbPlayStatus[i])
				continue;

			m_cbPlayerVlue[i] = m_GameLogic.GetCardType(cbUserCardData[i], MAX_COUNT);
		}

		//������
		SCORE lBankerScore = 0;
		SCORE lPlayerScore = 0;

		SCORE lCellScore = m_pITableFrame->GetCellScore();

		for (WORD i = 0; i < m_wPlayerNum; i++)
		{
			if (i == m_wBankerUser || m_cbPlayStatus[i] == FALSE)
				continue;

			lBankerScore = m_lTableScore[i] * m_GameLogic.GetTimes(cbUserCardData[m_wBankerUser], MAX_COUNT) * lCellScore * m_cbCallStatus[m_wBankerUser];
			lPlayerScore = m_lTableScore[i] * m_GameLogic.GetTimes(cbUserCardData[i], MAX_COUNT) * lCellScore * m_cbCallStatus[m_wBankerUser];
			
			if (m_GameLogic.CompareCard(cbUserCardData[m_wBankerUser], cbUserCardData[i], MAX_COUNT))
			{
				m_lCurrentScore[m_wBankerUser] += lBankerScore;
				m_lCurrentScore[i] -= lBankerScore;
			}
			else
			{
				m_lCurrentScore[m_wBankerUser] -= lPlayerScore;
				m_lCurrentScore[i] += lPlayerScore;
			}

		}

		//���ֱ���
		tagScoreInfo ScoreInfoArray[GAME_PLAYER];
		ZeroMemory(&ScoreInfoArray, sizeof(ScoreInfoArray));

		//��ʷ����
		for (WORD i = 0; i < m_wPlayerNum; i++)
		{
			//��˰
			SCORE lRevenue = 0;
			if (m_lCurrentScore[i]>0)
			{
				lRevenue = m_pITableFrame->CalculateRevenue(i, m_lCurrentScore[i]);
				m_lCurrentScore[i] -= lRevenue;
				ScoreInfoArray[i].lRevenue = lRevenue;
			}
			//�������
			ScoreInfoArray[i].lScore = m_lCurrentScore[i];
			ScoreInfoArray[i].cbType = (ScoreInfoArray[i].lScore >= 0L) ? SCORE_TYPE_WIN : SCORE_TYPE_LOSE;
		}

		//д�����
		m_pITableFrame->WriteTableScore(ScoreInfoArray, CountArray(ScoreInfoArray));

		CopyMemory(GameEnd.cbPlayerValue, m_cbPlayerVlue, sizeof(GameEnd.cbPlayerValue));
		CopyMemory(GameEnd.lGameScore, m_lCurrentScore, sizeof(GameEnd.lGameScore));

		//���ͳ��
		for (WORD i = 0; i < m_wPlayerNum; i++)
		{
			//��ȡ�û�
			IServerUserItem * pIServerUserIte = m_pITableFrame->GetTableUserItem(i);
			if (pIServerUserIte == NULL)
			{
				continue;
			}

			//����ۼ�
			if (pIServerUserIte->IsAndroidUser() && m_bWriteRoomStorage)
			{
				m_pGameServiceOption->pRoomStorageOption.lStorageCurrent += GameEnd.lGameScore[i];
			}

		}

		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));

		m_pITableFrame->SetGameTimer(IDI_DELAY_ENDGAME, TIME_DELAY_END, 1, 0);


		return true;
	}
	case GER_USER_LEAVE:    //�û�ǿ��
	case GER_NETWORK_ERROR:
	{
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
	case GAME_STATUS_FREE:    //����״̬
	{
		//��������
		CMD_S_StatusFree StatusFree;
		ZeroMemory(&StatusFree, sizeof(StatusFree));

		StatusFree.lCellScore = m_pITableFrame->GetCellScore();

		if (pIServerUserItem->IsAndroidUser())
		{
			tagCustomRule *pGameCustomRule = (tagCustomRule *)m_pITableFrame->GetCustomRule();

			tagRobotConfig RobotConfig;
			RobotConfig.lRobotScoreMin = pGameCustomRule->lRobotScoreMin;
			RobotConfig.lRobotScoreMax = pGameCustomRule->lRobotScoreMax;
			RobotConfig.lRobotBankGet = pGameCustomRule->lRobotBankGet;
			RobotConfig.lRobotBankGetBanker = pGameCustomRule->lRobotBankGetBanker;
			RobotConfig.lRobotBankStoMul = pGameCustomRule->lRobotBankStoMul;

			m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_ANDROID_CONFIG, &RobotConfig, sizeof(RobotConfig));
		}

		//���ͳ���
		return m_pITableFrame->SendGameScene(pIServerUserItem, &StatusFree, sizeof(StatusFree));
	}
	case GS_TK_CALL:  //��ׯ״̬
	{
		//��������
		CMD_S_StatusCall StatusCall;
		ZeroMemory(&StatusCall, sizeof(StatusCall));

		StatusCall.cbTimeLeave = (DWORD)time(NULL) - m_dwPassedTime;

		StatusCall.lCellScore = m_pITableFrame->GetCellScore();
		CopyMemory(StatusCall.cbCardData, m_cbHandCardData[wChairID], MAX_COUNT - 1);
		CopyMemory(StatusCall.cbPlayStatus, m_cbPlayStatus, sizeof(StatusCall.cbPlayStatus));
		CopyMemory(StatusCall.m_cbCallStatus, m_cbCallStatus, sizeof(StatusCall.m_cbCallStatus));

		if (pIServerUserItem->IsAndroidUser())
		{
			tagCustomRule *pGameCustomRule = (tagCustomRule *)m_pITableFrame->GetCustomRule();

			tagRobotConfig RobotConfig;
			RobotConfig.lRobotScoreMin = pGameCustomRule->lRobotScoreMin;
			RobotConfig.lRobotScoreMax = pGameCustomRule->lRobotScoreMax;
			RobotConfig.lRobotBankGet = pGameCustomRule->lRobotBankGet;
			RobotConfig.lRobotBankGetBanker = pGameCustomRule->lRobotBankGetBanker;
			RobotConfig.lRobotBankStoMul = pGameCustomRule->lRobotBankStoMul;

			m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_ANDROID_CONFIG, &RobotConfig, sizeof(RobotConfig));
		}

		//���ͳ���
		return m_pITableFrame->SendGameScene(pIServerUserItem, &StatusCall, sizeof(StatusCall));
	}
	case GS_TK_SCORE: //��ע״̬
	{
		//��������
		CMD_S_StatusScore StatusScore;
		ZeroMemory(&StatusScore, sizeof(StatusScore));

		StatusScore.cbTimeLeave = (DWORD)time(NULL) - m_dwPassedTime;

		StatusScore.lCellScore = m_pITableFrame->GetCellScore();
		StatusScore.wBankerUser = m_wBankerUser;
		CopyMemory(StatusScore.cbCallStatus, m_cbCallStatus, sizeof(StatusScore.cbCallStatus));
		CopyMemory(StatusScore.cbCardData, m_cbHandCardData[wChairID], MAX_COUNT - 1);
		CopyMemory(StatusScore.cbPlayStatus, m_cbPlayStatus, sizeof(StatusScore.cbPlayStatus));
		CopyMemory(StatusScore.lTableScore, m_lTableScore, sizeof(StatusScore.lTableScore));

		if (pIServerUserItem->IsAndroidUser())
		{
			tagCustomRule *pGameCustomRule = (tagCustomRule *)m_pITableFrame->GetCustomRule();

			tagRobotConfig RobotConfig;
			RobotConfig.lRobotScoreMin = pGameCustomRule->lRobotScoreMin;
			RobotConfig.lRobotScoreMax = pGameCustomRule->lRobotScoreMax;
			RobotConfig.lRobotBankGet = pGameCustomRule->lRobotBankGet;
			RobotConfig.lRobotBankGetBanker = pGameCustomRule->lRobotBankGetBanker;
			RobotConfig.lRobotBankStoMul = pGameCustomRule->lRobotBankStoMul;

			m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_ANDROID_CONFIG, &RobotConfig, sizeof(RobotConfig));
		}

		//���ͳ���
		return m_pITableFrame->SendGameScene(pIServerUserItem, &StatusScore, sizeof(StatusScore));
	}

	}
	//Ч�����
	ASSERT(FALSE);

	return false;
}

//��ȡ������Ϸ�Ƿ���ע״̬���������ע�� ����true���ж�������
bool CTableFrameSink::OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if (m_cbPlayStatus[wChairID] == TRUE && (m_pITableFrame->GetGameStatus() == GS_TK_CALL || m_pITableFrame->GetGameStatus() == GS_TK_SCORE))
		return true;
	else
		return false;
}

//��ʱ���¼�
bool CTableFrameSink::OnTimerMessage(DWORD dwTimerID, WPARAM wBindParam)
{
	switch (dwTimerID)
	{
	case IDI_DELAY_ENDGAME:
	{
		m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);
		m_pITableFrame->KillGameTimer(IDI_DELAY_ENDGAME);

		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_WAIT_START);

		m_pITableFrame->SetGameTimer(IDI_AUTO_START,TIME_START,1,0);
		return true;
	}
	case IDI_AUTO_START:
	{
		m_pITableFrame->KillGameTimer(IDI_AUTO_START);

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
	case IDI_SO_OPERATE:
	{
		//ɾ��ʱ��
		m_pITableFrame->KillGameTimer(IDI_SO_OPERATE);

		//��Ϸ״̬
		switch (m_pITableFrame->GetGameStatus())
		{
		case GS_TK_CALL:  //�û���ׯ
		{
			for (WORD i = 0; i < m_wPlayerNum; i++)
			{
				if (m_cbPlayStatus[i] == TRUE && m_cbCallStatus[i] == INVALID_BANKER)
				{
					OnUserCallBanker(i, OPERATE_NO_BANKERER);
					m_bFleeUser[i] ++;
				}
			}
			break;
		}
		case GS_TK_SCORE:  //��ע����
		{
			for (WORD i = 0; i < m_wPlayerNum; i++)
			{
				if (m_cbPlayStatus[i] == TRUE && m_lTableScore[i] == 0 && m_wBankerUser != i)
				{
					OnUserAddScore(i, OPERATE_SCORE_ONE);
					m_bFleeUser[i] ++;
				}
			}
			break;
		}

		default:
		{
			break;
		}

		}

		return true;
	}
	}

	return true;
}

//��Ϸ��Ϣ����
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_CALL_BANKER:     //�û���ׯ
		{
			//Ч������
			ASSERT(wDataSize == sizeof(CMD_C_CallBanker));
			if (wDataSize != sizeof(CMD_C_CallBanker))
			{
				return true;
			}

			//��������
			CMD_C_CallBanker * pCallBanker = (CMD_C_CallBanker *)pDataBuffer;

			//��Ϣ����
			return OnUserCallBanker(pIServerUserItem->GetChairID(), pCallBanker->bBanker);
		}
	case SUB_C_ADD_SCORE:     //�û���ע
		{
			//Ч������
			ASSERT(wDataSize == sizeof(CMD_C_AddScore));
			if (wDataSize != sizeof(CMD_C_AddScore))
			{
				return true;
			}

			//��������
			CMD_C_AddScore * pAddScore = (CMD_C_AddScore *)pDataBuffer;

			//��Ϣ����
			return OnUserAddScore(pIServerUserItem->GetChairID(), pAddScore->lScore);
		}

	}

	return true;
}

//�����Ϣ����
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

bool CTableFrameSink::OnUserCallBanker(WORD wChairID, BYTE bBanker)
{
	if (m_cbCallStatus[wChairID] != INVALID_BANKER)
		return true;

	//������ׯ������0 ��ʾ����
	m_cbCallStatus[wChairID] = bBanker;

	//������ׯ����
	CMD_S_Call_Banker CallBanker;
	ZeroMemory(&CallBanker, sizeof(CallBanker));
	CallBanker.cbBanker = wChairID;
	CallBanker.cbStatus = bBanker;
	for (WORD i = 0; i < m_wPlayerNum; i++)
	{
		if (!m_cbPlayStatus[i])
			continue;

		m_pITableFrame->SendTableData(i, SUB_S_CALL_BANKER, &CallBanker, sizeof(CallBanker));
	}

	BYTE cbCallCount = 0;
	BYTE cbRealCount = 0;
	for (WORD i = 0; i < m_wPlayerNum; i++)
	{
		if (m_cbPlayStatus[i] == TRUE)
		{
			cbRealCount++;

			if (m_cbCallStatus[i] != INVALID_BANKER)
				cbCallCount++;
		}
	}

	//����ׯ��
	if (cbCallCount == cbRealCount)
	{
		WORD wCallBanker = 0;
		BYTE vbBanker[GAME_PLAYER];

		BYTE cbCall[GAME_PLAYER];
		CopyMemory(cbCall, m_cbCallStatus, sizeof(cbCall));

		//��ȡ����ׯ����
		BYTE cbMaxTimes = 0;
		for (WORD i = 0; i < m_wPlayerNum; i++)
		{
			if (cbCall[i] > cbMaxTimes && cbCall[i] != INVALID_BANKER)
				cbMaxTimes = cbCall[i];
		}

		//��ȡ��ׯ����
		for (WORD i = 0; i < m_wPlayerNum; i++)
		{
			if (cbCall[i] == cbMaxTimes && cbCall[i] != INVALID_BANKER)
				vbBanker[wCallBanker++] = i;
		}

		if (wCallBanker > 0)
		{
			m_wBankerUser = vbBanker[rand() % wCallBanker];
			if (m_cbCallStatus[m_wBankerUser] == 0)
				m_cbCallStatus[m_wBankerUser] = 1;
		}
		else
		{
			m_wBankerUser = INVALID_BANKER;
		}

		if (cbMaxTimes == 0)
			cbMaxTimes = 1;

		//��ׯ���
		CMD_S_Banker_End BankerEnd;
		ZeroMemory(&BankerEnd, sizeof(BankerEnd));
		BankerEnd.cbBanker = m_wBankerUser;

		CopyMemory(BankerEnd.cbCallStatus,m_cbCallStatus,sizeof(BankerEnd.cbCallStatus));


		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_BANKER_END, &BankerEnd, sizeof(BankerEnd));
		

		//������ע״̬
		m_pITableFrame->SetGameStatus(GS_TK_SCORE);

		m_pITableFrame->KillGameTimer(IDI_SO_OPERATE);
		m_pITableFrame->SetGameTimer(IDI_SO_OPERATE, TIME_SO_OVERTIME, 1, 0);

		m_dwPassedTime = (DWORD)time(NULL);

		return true;
	}
	return true;

}

//��ע�¼�
bool CTableFrameSink::OnUserAddScore(WORD wChairID, SCORE lScore)
{
	//״̬Ч��
	if (m_pITableFrame->GetGameStatus() != GS_TK_SCORE && m_lTableScore[wChairID] != 0)
	{
		return true;
	}

	m_lTableScore[wChairID] = lScore;

	CMD_S_AddScore addScore;
	ZeroMemory(&addScore, sizeof(addScore));
	addScore.lAddScoreCount = lScore;
	addScore.wAddScoreUser = wChairID;

	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_ADD_SCORE, &addScore, sizeof(addScore));
	

	//������Ϸ
	BYTE cbScoreAddCount = 0;
	BYTE cbRealCount = 0;
	for (WORD i = 0; i < m_wPlayerNum; i++)
	{
		if (m_cbPlayStatus[i] == TRUE)
		{
			cbRealCount++;
			if (m_lTableScore[i] > 0)
				cbScoreAddCount++;
		}
	}
	
	if (cbScoreAddCount == (cbRealCount - 1))
	{
		return OnEventGameConclude(INVALID_CHAIR, NULL, GER_NORMAL);
	}
	return true;
}

//��������
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	return 0L;
}

//��ѯ�Ƿ�۷����
bool CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{
	
	if (m_lTableScore[wChairID] > 0 || m_cbCallStatus[wChairID] != INVALID_BANKER)
	{
		return true;
	}

	return false;
}
