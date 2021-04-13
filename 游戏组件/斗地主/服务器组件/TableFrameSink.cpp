#include "StdAfx.h"
#include "TableFrameSink.h"

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
#define IDI_CHECK_TABLE				1					//�������
#define IDI_USER_OUT_TIME			6					//��ҳ�ʱ

#define IDI_AUTO_START				7					//�Զ���ʼ

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//�������
	m_pITableFrame=NULL;
	m_pGameCustomRule=NULL;
	m_pGameServiceOption=NULL;
	m_pGameServiceAttrib=NULL;
	m_bOffLineTrustee = false;

	m_cbHaveOutMissile = FALSE;

	//ը������
	m_wFirstUser=INVALID_CHAIR;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));
	ZeroMemory(m_cbUserTrustee,sizeof(m_cbUserTrustee));

	ZeroMemory(m_cbLeftCardNum, sizeof(m_cbLeftCardNum));
	ZeroMemory(m_cbPlayerMultiple, sizeof(m_cbPlayerMultiple));

	m_dwOperateTime = 0L;

	m_bWriteRoomStorage = true;

	//��Ϸ����
	m_wTimerControl=0;
	m_cbBombCount=0;
	ZeroMemory(m_cbEachBombCount,sizeof(m_cbEachBombCount));

	//�з���Ϣ
	m_cbBankerScore=0;
	FillMemory(m_cbScoreInfo, sizeof(m_cbScoreInfo), 0xFF);
	m_cbCallScoreCount=0;

	//������Ϣ
	m_cbTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

	//�˿���Ϣ
	ZeroMemory(m_cbBankerCard,sizeof(m_cbBankerCard));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));



	return;
}

//��������
CTableFrameSink::~CTableFrameSink()
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

//��λ����
VOID CTableFrameSink::RepositionSink()
{
	m_cbHaveOutMissile = FALSE;

	//��Ϸ����
	m_cbBombCount=0;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));
	ZeroMemory(m_cbEachBombCount,sizeof(m_cbEachBombCount));

	ZeroMemory(m_cbLeftCardNum, sizeof(m_cbLeftCardNum));
	ZeroMemory(m_cbPlayerMultiple, sizeof(m_cbPlayerMultiple));

	m_bWriteRoomStorage = true;

	//�з���Ϣ
	m_cbBankerScore=0;
	FillMemory(m_cbScoreInfo, sizeof(m_cbScoreInfo),0xFF);
	m_cbCallScoreCount=0;

	//������Ϣ
	m_cbTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

	//�˿���Ϣ
	ZeroMemory(m_cbBankerCard,sizeof(m_cbBankerCard));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

	return;
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
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);

	//��Ϸ����
	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();

	//�Զ�����
	ASSERT(m_pITableFrame->GetCustomRule()!=NULL);
	m_pGameCustomRule=(tagCustomRule *)m_pITableFrame->GetCustomRule();

	m_bOffLineTrustee = true;
	return true;
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
	ZeroMemory(m_cbUserTrustee, sizeof(m_cbUserTrustee));
	m_pITableFrame->KillGameTimer(IDI_AUTO_START);

	BYTE cbAiCount = 0;
	BYTE cbPlayCount = 0;

	//�û�״̬
	for (WORD i = 0; i<GAME_PLAYER; i++)
	{
		//��ȡ�û�
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

	//���˥��
	if (m_bWriteRoomStorage)
		m_pITableFrame->CalculateStorageDeduct();

	//������Ϣ
	m_cbTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

	//����״̬
	m_pITableFrame->SetGameStatus(GAME_SCENE_CALL);

	//�����˿�
	BYTE cbRandCard[FULL_COUNT];
	m_GameLogic.RandCardList(cbRandCard,CountArray(cbRandCard));
	

	tag_Land_Result Land_Result;
	ZeroMemory(&Land_Result, sizeof(Land_Result));
	m_pITableFrame->GetControlResult(NULL, 0, &Land_Result);

	if (Land_Result.cbTableCardArray[0] != 0)
		CopyMemory(cbRandCard, Land_Result.cbTableCardArray, sizeof(cbRandCard));
	

	//�������������һ������з�
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)!=0 )
	{
		m_wFirstUser = INVALID_CHAIR;
	}

	//��ȡ����
	BYTE cbValidCardData=0;
	BYTE cbValidCardIndex=0;
	WORD wStartUser=m_wFirstUser;
	WORD wCurrentUser=m_wFirstUser;

	//��ȡ���
	if (wStartUser==INVALID_CHAIR)
	{
		//��ȡ�˿�
		cbValidCardIndex=rand()%DISPATCH_COUNT;
		cbValidCardData=cbRandCard[cbValidCardIndex];

		//�����û�
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
	
	//�û��˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_cbHandCardCount[i]=NORMAL_COUNT;
		CopyMemory(&m_cbHandCardData[i],&cbRandCard[i*m_cbHandCardCount[i]],sizeof(BYTE)*m_cbHandCardCount[i]);
	}

	//���õ���
	CopyMemory(m_cbBankerCard,&cbRandCard[DISPATCH_COUNT],sizeof(m_cbBankerCard));

	//����������
	CMD_S_AndroidCard AndroidCard ;
	ZeroMemory(&AndroidCard, sizeof(AndroidCard)) ;

	//�û��˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		CopyMemory(&AndroidCard.cbHandCard[i], m_cbHandCardData[i], sizeof(BYTE)*m_cbHandCardCount[i]) ;
	}

	//�����û�
	m_wFirstUser=wCurrentUser;
	m_wCurrentUser=wCurrentUser;

	//�������
	CMD_S_GameStart GameStart;
	GameStart.wCurrentUser=wCurrentUser;

	AndroidCard.wCurrentUser = m_wCurrentUser;
	CopyMemory(AndroidCard.cbBankerCard,m_cbBankerCard,3*sizeof(BYTE));

	//��������
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//�����˿�
		ASSERT(CountArray(GameStart.cbCardData)>=m_cbHandCardCount[i]);
		CopyMemory(GameStart.cbCardData,m_cbHandCardData[i],sizeof(BYTE)*m_cbHandCardCount[i]);

		//��������
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

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameLogic.SortCardList(m_cbHandCardData[i],m_cbHandCardCount[i],ST_ORDER);
	}
	
	m_dwOperateTime = (DWORD)time(NULL);

	IsOfflineTrustee();
	return true;
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			//��������
			CMD_S_GameConclude GameConclude;
			ZeroMemory(&GameConclude,sizeof(GameConclude));

			//���ñ���

			//ը����Ϣ
			GameConclude.cbBombCount=m_cbBombCount;
		
			//�û��˿�
			CopyMemory(GameConclude.cbCardCount, m_cbHandCardCount, sizeof(GameConclude.cbCardCount));
			CopyMemory(GameConclude.cbHandCardData, m_cbHandCardData, sizeof(GameConclude.cbHandCardData));

			//ը��ͳ��
			LONG lScoreTimes=1L * m_cbBankerScore;
			for (BYTE i=0;i<m_cbBombCount;i++) lScoreTimes*=2L;

			//�����ж�
			if (wChairID==m_wBankerUser)
			{
				//�û�����
				WORD wUser1=(m_wBankerUser+1)%GAME_PLAYER;
				WORD wUser2=(m_wBankerUser+2)%GAME_PLAYER;

				//�û��ж�
				if ((m_cbOutCardCount[wUser1]==0)&&(m_cbOutCardCount[wUser2]==0)) 
				{
					lScoreTimes*=2L;
					GameConclude.bChunTian=TRUE;
				}
			}

			//�����ж�
			if (wChairID!=m_wBankerUser)
			{
				if (m_cbOutCardCount[m_wBankerUser]==1)
				{
					lScoreTimes*=2L;
					GameConclude.bFanChunTian=TRUE;
				}
			}

			//��������
			lScoreTimes=__min(m_pGameCustomRule->wMaxScoreTimes,lScoreTimes);

			//ũ���й�����
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

			//���ֱ���
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));

			//ͳ�ƻ���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//��������
				SCORE lUserScore=0L;
				SCORE lCellScore=m_pITableFrame->GetCellScore();
				bool bTrusteeUser=m_pITableFrame->GetTableUserItem(i)->IsTrusteeUser();
				
				//���ֻ���
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

				//�������
				ScoreInfoArray[i].lScore = lUserScore*lCellScore*lScoreTimes;
				ScoreInfoArray[i].cbType=(ScoreInfoArray[i].lScore>=0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;

				//����˰��
				ScoreInfoArray[i].lRevenue=m_pITableFrame->CalculateRevenue(i,ScoreInfoArray[i].lScore);
				if (ScoreInfoArray[i].lRevenue>0L) ScoreInfoArray[i].lScore-=ScoreInfoArray[i].lRevenue;

				//���û���
				GameConclude.lGameScore[i]=ScoreInfoArray[i].lScore;

				//���ֵ
				IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);
				if (pServerUser && pServerUser->IsAndroidUser() && m_bWriteRoomStorage)
					m_pGameServiceOption->pRoomStorageOption.lStorageCurrent += ScoreInfoArray[i].lScore;
			}

			//��������
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_CONCLUDE,&GameConclude,sizeof(GameConclude));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_CONCLUDE,&GameConclude,sizeof(GameConclude));

			//д�����
			m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

			//�л��û�
			m_wFirstUser=wChairID;

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);

			m_pITableFrame->SetGameTimer(IDI_AUTO_START,m_pGameCustomRule->cbTimeStartGame*1000,1,0L);

			return true;
		}
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			//��������
			CMD_S_GameConclude GameConclude;
			ZeroMemory(&GameConclude,sizeof(GameConclude));

			//�û��˿�
			BYTE cbCardIndex=0;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//�����˿�
				GameConclude.cbCardCount[i]=m_cbHandCardCount[i];
				CopyMemory(&GameConclude.cbHandCardData[cbCardIndex],m_cbHandCardData[i],m_cbHandCardCount[i]*sizeof(BYTE));

				//��������
				cbCardIndex+=m_cbHandCardCount[i];
			}


			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_CONCLUDE,&GameConclude,sizeof(GameConclude));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_CONCLUDE,&GameConclude,sizeof(GameConclude));

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);

			m_pITableFrame->SetGameTimer(IDI_AUTO_START, m_pGameCustomRule->cbTimeStartGame*1000, 1, 0L);

			return true;
		}
	case GER_USER_LEAVE:	//�û�ǿ��
	case GER_NETWORK_ERROR:	//�����ж�
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

	//�������
	ASSERT(FALSE);

	return false;
}

//���ͳ���
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:	//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			StatusFree.cbCallTimes = m_pGameCustomRule->cbTimeCallScore;
			StatusFree.cbOutTimes = m_pGameCustomRule->cbTimeOutCard;
			StatusFree.cbDoubleTimes = m_pGameCustomRule->cbTimeHeadOutCard;
			StatusFree.cbEndTimes = m_pGameCustomRule->cbTimeStartGame;

			CopyMemory(StatusFree.nTrusteeStatus, m_cbUserTrustee, sizeof(StatusFree.nTrusteeStatus));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GAME_SCENE_CALL:	//�з�״̬
		{
			//��������
			CMD_S_StatusCall StatusCall;
			ZeroMemory(&StatusCall,sizeof(StatusCall));

			StatusCall.cbCallTimes = m_pGameCustomRule->cbTimeCallScore;
			StatusCall.cbOutTimes = m_pGameCustomRule->cbTimeOutCard;
			StatusCall.cbDoubleTimes = m_pGameCustomRule->cbTimeHeadOutCard;
			StatusCall.cbEndTimes = m_pGameCustomRule->cbTimeStartGame;

			CopyMemory(StatusCall.nTrusteeStatus, m_cbUserTrustee, sizeof(StatusCall.nTrusteeStatus));

			DWORD dwPassTime = (DWORD)time(NULL) - m_dwOperateTime;
			StatusCall.cbTimeLeave = (BYTE)(StatusCall.cbCallTimes - __min(dwPassTime, StatusCall.cbCallTimes));

			//��Ϸ��Ϣ
			StatusCall.wCurrentUser=m_wCurrentUser;
			CopyMemory(StatusCall.cbScoreInfo,m_cbScoreInfo,sizeof(m_cbScoreInfo));
			CopyMemory(StatusCall.cbHandCardData,m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID]*sizeof(BYTE));

			//���ͳ���
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
	case GAME_SCENE_MULTIPLE:	//�ӱ�״̬
		{
			//��������
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

			//���ͳ���
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
	case GAME_SCENE_PLAY:	//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			StatusPlay.cbCallTimes = m_pGameCustomRule->cbTimeCallScore;
			StatusPlay.cbOutTimes = m_pGameCustomRule->cbTimeOutCard;
			StatusPlay.cbDoubleTimes = m_pGameCustomRule->cbTimeHeadOutCard;
			StatusPlay.cbEndTimes = m_pGameCustomRule->cbTimeStartGame;

			CopyMemory(StatusPlay.nTrusteeStatus, m_cbUserTrustee, sizeof(StatusPlay.nTrusteeStatus));

			//��Ϸ����
			StatusPlay.cbBombCount=m_cbBombCount;
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.cbBankerScore=m_cbBankerScore;

			DWORD dwPassTime = (DWORD)time(NULL) - m_dwOperateTime;
			StatusPlay.cbTimeLeave = (BYTE)(StatusPlay.cbOutTimes - __min(dwPassTime, StatusPlay.cbOutTimes));

			//������Ϣ
			StatusPlay.wTurnWiner=m_wTurnWiner;
			StatusPlay.cbTurnCardCount=m_cbTurnCardCount;
			CopyMemory(StatusPlay.cbTurnCardData,m_cbTurnCardData,m_cbTurnCardCount*sizeof(BYTE));

			//�˿���Ϣ
			CopyMemory(StatusPlay.cbBankerCard,m_cbBankerCard,sizeof(m_cbBankerCard));
			CopyMemory(StatusPlay.cbHandCardCount,m_cbHandCardCount,sizeof(m_cbHandCardCount));
			CopyMemory(StatusPlay.cbHandCardData,m_cbHandCardData[wChairID],sizeof(BYTE)*m_cbHandCardCount[wChairID]);

			CopyMemory(StatusPlay.cbPlayerMultiple, m_cbPlayerMultiple, sizeof(StatusPlay.cbPlayerMultiple));

			//���ͳ���
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

	//�������
	ASSERT(FALSE);

	return false;
}

//��ȡ������Ϸ�Ƿ���ע״̬���������ע�� ����true���ж�������
bool CTableFrameSink::OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if (m_pITableFrame->GetGameStatus() == GAME_SCENE_PLAY || m_pITableFrame->GetGameStatus() == GAME_SCENE_MULTIPLE || m_pITableFrame->GetGameStatus() == GAME_SCENE_CALL)
		return true;
	else
		return false;
}

//ʱ���¼�
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

//�����¼�
bool CTableFrameSink::OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//�����¼�
bool CTableFrameSink::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	return false;
}

//��Ϸ��Ϣ
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	m_cbWaitTime=0;
	switch (wSubCmdID)
	{
	case SUB_C_CALL_SCORE:	//�û��з�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_CallScore));
			if (wDataSize!=sizeof(CMD_C_CallScore)) return true;

			//״̬Ч��
			//ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_CALL);
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_CALL) return true;

			//�û�Ч��
			//ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��������
			CMD_C_CallScore * pCallScore=(CMD_C_CallScore *)pData;

			//��Ϣ����
			WORD wChairID=pIServerUserItem->GetChairID();
			return OnUserCallScore(wChairID,pCallScore->cbCallScore);
		}
	case SUB_C_DOUBLE:		//�û��ӱ�
		{
			//Ч������
			ASSERT(wDataSize == sizeof(CMD_C_CallDouble));
			if (wDataSize != sizeof(CMD_C_CallDouble)) return true;

			//״̬Ч��
			//ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_CALL);
			if (m_pITableFrame->GetGameStatus() != GAME_SCENE_MULTIPLE) return true;

			//�û�Ч��
			//ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus() != US_PLAYING) return true;

			//��������
			CMD_C_CallDouble * pCallDouble = (CMD_C_CallDouble *)pData;

			//��Ϣ����
			WORD wChairID = pIServerUserItem->GetChairID();
			return OnUserDouble(wChairID, pCallDouble->cbPlayerMultiple);
		}
	case SUB_C_OUT_CARD:	//�û�����
		{
			//��������
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pData;
			WORD wHeadSize=sizeof(CMD_C_OutCard)-sizeof(pOutCard->cbCardData);

			//Ч������
			ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pOutCard->cbCardCount*sizeof(BYTE))));
			if ((wDataSize<wHeadSize) || (wDataSize != (wHeadSize + pOutCard->cbCardCount*sizeof(BYTE)))) return true;

			//״̬Ч��
			//ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;

			//�û�Ч��
			//ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��Ϣ����
			WORD wChairID=pIServerUserItem->GetChairID();
			return OnUserOutCard(wChairID,pOutCard->cbCardData,pOutCard->cbCardCount);
		}
	case SUB_C_PASS_CARD:	//�û�����
		{
			//״̬Ч��
			//ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;

			//�û�Ч��
			//ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��Ϣ����
			return OnUserPassCard(pIServerUserItem->GetChairID());
		}
	case SUB_C_TRUSTEE:
		{
			//Ч������
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

//�����Ϣ
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�����
bool CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if(m_bOffLineTrustee && wChairID == m_wCurrentUser)
	{
		IsOfflineTrustee();
	}
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
	//��ʷ����
	if (bLookonUser==false)
	{

	}

	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��ʷ����
	if (bLookonUser==false)
	{

	}
	m_cbUserTrustee[wChairID] = 0;
	pIServerUserItem->SetTrusteeUser(false);
	return true;
}

//�û�����
bool CTableFrameSink::OnUserPassCard(WORD wChairID)
{
	//Ч��״̬
	ASSERT((wChairID==m_wCurrentUser)&&(m_cbTurnCardCount!=0));
	if ((wChairID!=m_wCurrentUser)||(m_cbTurnCardCount==0)) return true;

	//���ñ���
	m_wCurrentUser=(m_wCurrentUser+1)%GAME_PLAYER;
	if (m_wCurrentUser==m_wTurnWiner) m_cbTurnCardCount=0;

	//������Ϣ
	CMD_S_PassCard PassCard;
	PassCard.wPassCardUser=wChairID;
	PassCard.wCurrentUser=m_wCurrentUser;

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));

	IsOfflineTrustee();

	m_dwOperateTime = (DWORD)time(NULL);

	return true;
}

//�û��з�
bool CTableFrameSink::OnUserCallScore(WORD wChairID, BYTE cbCallScore)
{
	//Ч��״̬
	ASSERT(wChairID==m_wCurrentUser);
	if (wChairID!=m_wCurrentUser) return true;

	//Ч�����
	//ASSERT(((cbCallScore>=1)&&(cbCallScore<=3)&&(cbCallScore>m_cbBankerScore))||(cbCallScore==255));
	if ((cbCallScore<0)||(cbCallScore>3)||(cbCallScore<=m_cbBankerScore))
		cbCallScore = 0;

	//����״̬
	if (cbCallScore!=0)
	{
		m_cbBankerScore=cbCallScore;
		m_wBankerUser=m_wCurrentUser;
	}

	//���ýз�
	m_cbScoreInfo[wChairID]=cbCallScore;

	//�����û�
	if ((m_cbBankerScore==3)||(m_wFirstUser==(wChairID+1)%GAME_PLAYER))
	{
		m_wCurrentUser=INVALID_CHAIR;
	}
	else
	{
		m_wCurrentUser=(wChairID+1)%GAME_PLAYER;
	}

	//�������
	CMD_S_CallScore CallScore;
	CallScore.wCallScoreUser=wChairID;
	CallScore.wCurrentUser=m_wCurrentUser;
	CallScore.cbUserCallScore=cbCallScore;
	CallScore.cbCurrentScore=m_cbBankerScore;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CALL_SCORE,&CallScore,sizeof(CallScore));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CALL_SCORE,&CallScore,sizeof(CallScore));

	//��ʼ�ж�
	if ((m_cbBankerScore==3)||(m_wFirstUser==(wChairID+1)%GAME_PLAYER))
	{
		//���˽з�
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

		//����״̬
		m_pITableFrame->SetGameStatus(GAME_SCENE_MULTIPLE);

		//���ñ���
		if (m_cbBankerScore==0) m_cbBankerScore=1;
		if (m_wBankerUser==INVALID_CHAIR) m_wBankerUser=m_wFirstUser;
		m_cbCallScoreCount=0;

		m_cbPlayerMultiple[m_wBankerUser] = 1;

		//���͵���
		m_cbHandCardCount[m_wBankerUser]+=CountArray(m_cbBankerCard);
		CopyMemory(&m_cbHandCardData[m_wBankerUser][NORMAL_COUNT],m_cbBankerCard,sizeof(m_cbBankerCard));

		//�����˿�
		m_GameLogic.SortCardList(m_cbHandCardData[m_wBankerUser],m_cbHandCardCount[m_wBankerUser],ST_ORDER);


		//�����û�
		m_wTurnWiner=m_wBankerUser;
		m_wCurrentUser=m_wBankerUser;

		//������Ϣ
		CMD_S_BankerInfo BankerInfo;
		BankerInfo.wBankerUser=m_wBankerUser;
		BankerInfo.cbBankerScore=m_cbBankerScore;
		CopyMemory(BankerInfo.cbBankerCard,m_cbBankerCard,sizeof(m_cbBankerCard));

		//������Ϣ
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

//�û��ӱ�
bool CTableFrameSink::OnUserDouble(WORD wChairID, BYTE cbDoubleTimes)
{
	if (wChairID == m_wBankerUser)
		return true;

	//Ч�����
	if (cbDoubleTimes != 1 && cbDoubleTimes != 2)
		cbDoubleTimes = 1;

	//���ýз�
	m_cbPlayerMultiple[wChairID] = cbDoubleTimes;

	//�������
	CMD_S_Double CallDouble;
	CallDouble.wDoubleUser = wChairID;
	CallDouble.cbPlayerMultiple = cbDoubleTimes;

	//������Ϣ
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

	//��ʼ�ж�
	if (cbDoubleCount == 2)
	{
		m_wCurrentUser = m_wBankerUser;

		//����״̬
		m_pITableFrame->SetGameStatus(GAME_SCENE_PLAY);

		//������Ϣ
		CMD_S_BankerOut BankerOut;
		BankerOut.wBankerUser = m_wBankerUser;

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_BANKER_OUT, &BankerOut, sizeof(BankerOut));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_BANKER_OUT, &BankerOut, sizeof(BankerOut));

		IsOfflineTrustee();

		m_dwOperateTime = (DWORD)time(NULL);

		return true;
	}

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData[], BYTE cbCardCount)
{
	//Ч��״̬
	ASSERT(wChairID==m_wCurrentUser);
	if (wChairID!=m_wCurrentUser) return true;

	//��ȡ����
	BYTE cbCardType=m_GameLogic.GetCardType(cbCardData,cbCardCount);

	//�����ж�
	if (cbCardType==CT_ERROR) 
	{
		ASSERT(FALSE);
		return true;
	}

	//�����ж�
	if (m_cbTurnCardCount!=0)
	{
		//�Ա��˿�
		if (m_GameLogic.CompareCard(m_cbTurnCardData,cbCardData,m_cbTurnCardCount,cbCardCount)==false)
		{
			ASSERT(FALSE);
			return true;
		}
	}

	//ɾ���˿�
	if (m_GameLogic.RemoveCardList(cbCardData,cbCardCount,m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false)
	{
		ASSERT(FALSE);
		return true;
	}

	//������ʣ������
	for (int i = 0; i < cbCardCount; i++)
	{
		BYTE cbColor = m_GameLogic.GetCardColor(cbCardData[i]);
		BYTE cbValue = m_GameLogic.GetCardValue(cbCardData[i]);
		if (cbValue > 0)
		{
			m_cbLeftCardNum[cbValue - 1] -= 1;
		}
	}

	//���Ʊ���
	m_cbOutCardCount[wChairID]++;

	//���ñ���
	m_cbTurnCardCount=cbCardCount;
	m_cbHandCardCount[wChairID]-=cbCardCount;
	CopyMemory(m_cbTurnCardData,cbCardData,sizeof(BYTE)*cbCardCount);

	//ը���ж�
	if ((cbCardType==CT_BOMB_CARD)||(cbCardType==CT_MISSILE_CARD)) 
	{
		m_cbBombCount++;
		m_cbEachBombCount[wChairID]++;
	}
	
	//����ж�
	if (cbCardType == CT_MISSILE_CARD)
		m_cbHaveOutMissile = TRUE;

	//�л��û�
	m_wTurnWiner=wChairID;
	if (m_cbHandCardCount[wChairID]!=0)
	{
		if (cbCardType!=CT_MISSILE_CARD)
		{
			m_wCurrentUser=(m_wCurrentUser+1)%GAME_PLAYER;
		}
	}
	else m_wCurrentUser=INVALID_CHAIR;

	//��������
	CMD_S_OutCard OutCard;
	ZeroMemory(&OutCard, sizeof(OutCard));
	OutCard.wOutCardUser=wChairID;
	OutCard.cbCardCount=cbCardCount;
	OutCard.wCurrentUser=m_wCurrentUser;
	CopyMemory(OutCard.cbCardData,m_cbTurnCardData,m_cbTurnCardCount*sizeof(BYTE));

	//��������
	WORD wHeadSize=sizeof(OutCard)-sizeof(OutCard.cbCardData);
	WORD wSendSize=wHeadSize+OutCard.cbCardCount*sizeof(BYTE);
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		m_pITableFrame->SendTableData(i, SUB_S_OUT_CARD, &OutCard, wSendSize);
		m_pITableFrame->SendLookonData(i, SUB_S_OUT_CARD, &OutCard, wSendSize);
	}

	//�������
	if (cbCardType==CT_MISSILE_CARD) m_cbTurnCardCount=0;

	//�����ж�
	if (m_wCurrentUser==INVALID_CHAIR) OnEventGameConclude(wChairID,NULL,GER_NORMAL);
	else
	{
		IsOfflineTrustee();
	}

	m_dwOperateTime = (DWORD)time(NULL);

	return true;
}

//���û���
void CTableFrameSink::SetGameBaseScore(SCORE lBaseScore)
{
	
}

//�Ƿ��й�
bool CTableFrameSink::IsOfflineTrustee()
{
	//�ǳ�����λ
	if(m_wCurrentUser==INVALID_CHAIR) return false;

	//�������
	if (m_bOffLineTrustee)
	{
		IServerUserItem *pServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentUser);
		ASSERT(pServerUserItem!=NULL);
		if (pServerUserItem==NULL) return false;

		//ɾ����ʱ��ʱ��
		m_pITableFrame->KillGameTimer(IDI_USER_OUT_TIME);

		//��������
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

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
