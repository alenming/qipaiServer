#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//��������
#define SEND_COUNT					600									//���ʹ���

//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//��עʱ��
#define IDI_FREE					1									//����ʱ��
#ifdef _DEBUG
#define TIME_FREE					6									//����ʱ��
#else
#define TIME_FREE					10									//����ʱ��
#endif

//��עʱ��
#define IDI_PLACE_JETTON			2									//��עʱ��

#ifdef _DEBUG
#define TIME_PLACE_JETTON			5									//��עʱ��
#else
#define TIME_PLACE_JETTON			15									//��עʱ��
#endif

//����ʱ��
#define IDI_GAME_END				3									//����ʱ��

#ifdef _DEBUG
#define TIME_GAME_END				20									//����ʱ��
#else
#define TIME_GAME_END				20									//����ʱ��
#endif

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//��Ϸ����

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//����ע��
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));
	
	//������ע
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));


	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//�˿���Ϣ
	ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

	ZeroMemory(m_cbTableHavaSendCardArray,sizeof(m_cbTableHavaSendCardArray));
	ZeroMemory(m_cbTableHavaSendCount,sizeof(m_cbTableHavaSendCount));

	ZeroMemory(m_bUserListWin, sizeof(m_bUserListWin));
	ZeroMemory(m_lUserListScore, sizeof(m_lUserListScore));
	ZeroMemory(m_cbUserPlayCount, sizeof(m_cbUserPlayCount));

	//״̬����
	m_dwJettonTime=0L;

	m_cbJuControl = FALSE;
	m_cbJuControlTimes = 0;
	ZeroMemory(m_cbJuControlArea, sizeof(m_cbJuControlArea));

	//ׯ����Ϣ
	m_ApplyUserArray.RemoveAll();
	m_wCurrentBanker=INVALID_CHAIR;
	m_wBankerTime=0;
	m_lBankerWinScore=0L;		
	m_lBankerCurGameScore=0L;
	m_cbLeftCardCount=0;

	m_lSysBankerScore = 10000000;

	//��¼����
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;
	m_nRecordLast=0;

	//�����˿���
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{

}

//�ͷŶ���
VOID  CTableFrameSink::Release()
{
	delete this;
}

//�ӿڲ�ѯ
void *  CTableFrameSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
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

	m_pITableFrame->SetStartMode(START_MODE_ALL_READY);

	//��Ϸ����
	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();

	//�����ļ���
	TCHAR szPath[MAX_PATH] = TEXT("");
	GetCurrentDirectory(sizeof(szPath), szPath);
	myprintf(m_szConfigFileName, sizeof(m_szConfigFileName), TEXT("%s\\BaiRenConfig\\TongziConfig-%d-%d.ini"), szPath, m_pGameServiceOption->wServerID, m_pITableFrame->GetTableID());

	ReadConfigInformation(true);


	return true;
}

//��λ����
VOID  CTableFrameSink::RepositionSink()
{
	//����ע��
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));
	
	//������ע
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//�����˿���
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

	ZeroMemory(m_cbTableCardArray, sizeof(m_cbTableCardArray));

	return;
}

//��Ϸ��ʼ
bool  CTableFrameSink::OnEventGameStart()
{
	//���ʣ���Ʋ�����  ����ϴ������
	if (m_cbLeftCardCount < 8)
	{
		m_GameLogic.RandCardList(m_cbTableCard, CountArray(m_cbTableCard));
		m_cbLeftCardCount = CountArray(m_cbTableCard);

		ZeroMemory(m_cbTableHavaSendCardArray, sizeof(m_cbTableHavaSendCardArray));
		ZeroMemory(m_cbTableHavaSendCount, sizeof(m_cbTableHavaSendCount));
	}

	//��������
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));

	//��ȡׯ��
	IServerUserItem *pIBankerServerUserItem=NULL;
	if (INVALID_CHAIR!=m_wCurrentBanker) 
	{
		pIBankerServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		m_lBankerScore=pIBankerServerUserItem->GetUserScore();
	}

	//���ñ���
	GameStart.cbTimeLeave=m_nPlaceJettonTime;
	GameStart.wBankerUser=m_wCurrentBanker;
	if (pIBankerServerUserItem!=NULL) 
		GameStart.lBankerScore=m_lBankerScore;
	else
		GameStart.lBankerScore = m_lSysBankerScore;

	GameStart.bContiueCard=true;

	//m_wBankerTime

	GameStart.cbLeftCardCount=m_cbLeftCardCount+8;

	for (int i = 0;i<4;i++)
	{	
		CopyMemory(GameStart.cbTableHavaSendCardArray[i],m_cbTableHavaSendCardArray[i],CountArray(m_cbTableHavaSendCardArray[i]));
	}

	CopyMemory(GameStart.cbTableHavaSendCount,m_cbTableHavaSendCount,CountArray(m_cbTableHavaSendCount));

    
	//��ע����������
	int nChipRobotCount = 0;
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
			nChipRobotCount++;
	}

	//�����˿���
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

	FillMemory(GameStart.wSeatUser, sizeof(GameStart.wSeatUser), INVALID_CHAIR);

	OnGetUserListGameID(GameStart.wSeatUser);
	
	//�Թ����
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	

	//��Ϸ���
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;

		//���û���
		GameStart.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore);

		m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	
	}

	return true;
}

//��Ϸ����
bool  CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������	
		{

			tag_TongZi_Parameter TongZi_Parameter;
			ZeroMemory(&TongZi_Parameter, sizeof(TongZi_Parameter));

			TongZi_Parameter.wBankerUser = m_wCurrentBanker;
			TongZi_Parameter.cbJuControl = m_cbJuControl;
			CopyMemory(TongZi_Parameter.cbJuControlArea, m_cbJuControlArea, sizeof(TongZi_Parameter.cbJuControlArea));
			CopyMemory(TongZi_Parameter.lPlayBet, m_lUserJettonScore, sizeof(TongZi_Parameter.lPlayBet));
			TongZi_Parameter.cbLeftCardCount = m_cbLeftCardCount;
			CopyMemory(TongZi_Parameter.cbTableCard, m_cbTableCard, sizeof(TongZi_Parameter.cbTableCard));

			tag_TongZi_Result pTongZi_Result;
			ZeroMemory(&pTongZi_Result, sizeof(pTongZi_Result));
			m_pITableFrame->GetControlResult(&TongZi_Parameter, sizeof(TongZi_Parameter), &pTongZi_Result);

			if (pTongZi_Result.cbTableCardArray[0][0] != 0)
			{
				CopyMemory(m_cbTableCardArray, pTongZi_Result.cbTableCardArray, sizeof(m_cbTableCardArray));
				m_cbCardCount[0] = 2;
				m_cbCardCount[1] = 2;
				m_cbCardCount[2] = 2;
				m_cbCardCount[3] = 2;
			}
			else
				CopyMemory(&m_cbTableCardArray[0][0], m_cbTableCard, sizeof(m_cbTableCardArray));

			if (m_cbJuControlTimes > 0)
				m_cbJuControlTimes--;
			if (m_cbJuControlTimes == 0)
			{
				m_cbJuControl = FALSE;
				ZeroMemory(m_cbJuControlArea, sizeof(m_cbJuControlArea));
			}

			//��������
			{
				BYTE cbCardBuffer[8] = {0};
				CopyMemory(cbCardBuffer, m_cbTableCardArray, sizeof(cbCardBuffer));
				m_GameLogic.RemoveCard(cbCardBuffer,8,m_cbTableCard,m_cbLeftCardCount);
				m_cbLeftCardCount -= 8;
			}

			m_cbTableCardArray[0][0] = 0x04;
			m_cbTableCardArray[0][1] = 0x06;

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

			bool static bWinShangMen, bWinTianMen, bWinXiaMen;
			DeduceWinner(bWinShangMen, bWinTianMen, bWinXiaMen);

			if (bWinShangMen && bWinTianMen && bWinXiaMen)
				GameEnd.cbBankerTong = 2;
			else if (!bWinShangMen && !bWinTianMen && !bWinXiaMen)
				GameEnd.cbBankerTong = 1;

			GameEnd.cbWinArea[1] = bWinShangMen ? TRUE : FALSE;
			GameEnd.cbWinArea[2] = bWinTianMen ? TRUE : FALSE;
			GameEnd.cbWinArea[3] = bWinXiaMen ? TRUE : FALSE;

			//ׯ����Ϣ
			GameEnd.nBankerTime = m_wBankerTime;
			GameEnd.lBankerTotallScore=m_lBankerWinScore;
			GameEnd.lBankerScore=lBankerWinScore;

			if (m_wCurrentBanker == INVALID_CHAIR)
			{
				m_lSysBankerScore += lBankerWinScore;
				GameEnd.lBankerHaveScore = m_lSysBankerScore;
			}

			//�˿���Ϣ
			CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));

			GameEnd.cbLeftCardCount=m_cbLeftCardCount;
			for (int i = 0;i<4;i++)
			{
				CopyMemory(&m_cbTableHavaSendCardArray[i][m_cbTableHavaSendCount[i]],GameEnd.cbTableCardArray[i],2);		
				m_cbTableHavaSendCount[i]+=2;
				CopyMemory(GameEnd.cbTableHavaSendCardArray[i],m_cbTableHavaSendCardArray[i],sizeof(m_cbTableHavaSendCardArray[i]));
			}
			
			CopyMemory(GameEnd.cbTableHavaSendCount,m_cbTableHavaSendCount,sizeof(m_cbTableHavaSendCount));

			for (int i = 0; i < 4; i++)
			{
				GameEnd.cbCardType[i] = m_GameLogic.GetCardType(m_cbTableCardArray[i],2);
			}

			//ռλ��ҳɼ�
			WORD wSeatUser[MAX_SEAT_COUNT];
			FillMemory(wSeatUser, sizeof(wSeatUser), INVALID_CHAIR);
			OnGetUserListGameID(wSeatUser);

			for (WORD i = 0; i < MAX_SEAT_COUNT; i++)
			{
				if (wSeatUser[i] != INVALID_CHAIR)
				{
					GameEnd.lSeatScore[i] = m_lUserWinScore[wSeatUser[i]];
				}
			}

			//���ͻ���
			GameEnd.cbTimeLeave=m_nGameEndTime;	

			for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
				if ( pIServerUserItem == NULL ) continue;

				//���óɼ�
				GameEnd.lUserScore=m_lUserWinScore[wUserIndex];

				//��������
				GameEnd.lUserReturnScore=m_lUserReturnScore[wUserIndex];

				//����˰��
				if (m_lUserRevenue[wUserIndex]>0) GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
				else if (m_wCurrentBanker!=INVALID_CHAIR) GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
				else GameEnd.lRevenue=0;

				//������Ϣ					
				m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

				if (!pIServerUserItem->IsAndroidUser())
				{
					m_pGameServiceOption->pRoomStorageOption.lStorageCurrent -= GameEnd.lUserScore;
				}
			}

			//�޸Ļ���
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(ScoreInfoArray, sizeof(ScoreInfoArray));

			//д�����
			for (WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID)
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);
				if (pIServerUserItem == NULL) continue;

				//ʤ������
				//enScoreKind ScoreKind=(m_lUserWinScore[wUserChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;

				ScoreInfoArray[wUserChairID].lScore = m_lUserWinScore[wUserChairID];
				ScoreInfoArray[wUserChairID].cbType = (m_lUserWinScore[wUserChairID]>0L) ? SCORE_TYPE_WIN : SCORE_TYPE_LOSE;;
				ScoreInfoArray[wUserChairID].lRevenue = m_lUserRevenue[wUserChairID];
				//д�����
				//if (m_lUserWinScore[wUserChairID]!=0L) m_pITableFrame->WriteUserScore(wUserChairID,m_lUserWinScore[wUserChairID], m_lUserRevenue[wUserChairID], ScoreKind);

			}

			m_pITableFrame->WriteTableScore(ScoreInfoArray, CountArray(ScoreInfoArray));

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
	case GAME_STATUS_FREE:			//����״̬
		{
			//���ͼ�¼
			SendGameRecord(pIServerUserItem);

			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));			

			FillMemory(StatusFree.wSeatUser, sizeof(StatusFree.wSeatUser), INVALID_CHAIR);

			//������Ϣ
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
			StatusFree.lAreaLimitScore = m_lAreaLimitScore;

			//�˿���Ϣ
			StatusFree.cbLeftCardCount=m_cbLeftCardCount;
			for (int i = 0;i<4;i++)
			{	
				CopyMemory(StatusFree.cbTableHavaSendCardArray[i],m_cbTableHavaSendCardArray[i],CountArray(m_cbTableHavaSendCardArray[i]));
			}

			CopyMemory(StatusFree.cbTableHavaSendCount,m_cbTableHavaSendCount,CountArray(m_cbTableHavaSendCount));

			//ׯ����Ϣ
			StatusFree.wBankerUser=m_wCurrentBanker;	
			StatusFree.cbBankerTime=m_wBankerTime;
			StatusFree.lBankerWinScore=m_lBankerWinScore;
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				StatusFree.lBankerScore=pIServerUserItem->GetUserScore();
			}
			else
				StatusFree.lBankerScore = m_lSysBankerScore;

			//�����Ϣ
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusFree.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore); 
			}

			//��������
			CopyMemory(StatusFree.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusFree.szGameRoomName));

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusFree.cbTimeLeave=(BYTE)(m_nFreeTime-(__min(dwPassTime,m_nFreeTime)));

			OnGetUserListGameID(StatusFree.wSeatUser);

			//���ͳ���
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
						
			//����������
			SendApplyUser(pIServerUserItem);

			//���ͻ�������Ϣ
			if (pIServerUserItem && pIServerUserItem->IsAndroidUser())
			{
				CMD_S_AndroidInit AndroidInit;
				ZeroMemory(&AndroidInit, sizeof(AndroidInit));
				memcpy(AndroidInit.szRoomName, TEXT("���˶��˸�"), sizeof(AndroidInit.szRoomName));
				memcpy(AndroidInit.szConfigName, m_szConfigFileName, sizeof(AndroidInit.szConfigName));
				m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_ANDROID_INIT, &AndroidInit, sizeof(AndroidInit));
			}

			return bSuccess;
		}
	case GS_PLACE_JETTON:		//��Ϸ״̬
	case GS_GAME_END:			//����״̬
		{
			//���ͼ�¼
			SendGameRecord(pIServerUserItem);		

			//��������
			CMD_S_StatusPlay StatusPlay={0};

			FillMemory(StatusPlay.wSeatUser, sizeof(StatusPlay.wSeatUser), INVALID_CHAIR);

			//ȫ����ע
			CopyMemory(StatusPlay.lAllJettonScore,m_lAllJettonScore,sizeof(StatusPlay.lAllJettonScore));

			//�����ע
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
					StatusPlay.lUserJettonScore[nAreaIndex] = m_lUserJettonScore[wChiarID][nAreaIndex];

				//�����ע
				StatusPlay.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore);
			}

			//������Ϣ
			StatusPlay.lApplyBankerCondition=m_lApplyBankerCondition;		
			StatusPlay.lAreaLimitScore=m_lAreaLimitScore;		

			//ׯ����Ϣ
			StatusPlay.wBankerUser=m_wCurrentBanker;			
			StatusPlay.cbBankerTime=m_wBankerTime;
			StatusPlay.lBankerWinScore=m_lBankerWinScore;	


			StatusPlay.cbLeftCardCount=m_cbLeftCardCount;
			for (int i = 0;i<4;i++)
			{
				CopyMemory(StatusPlay.cbTableHavaSendCardArray[i],m_cbTableHavaSendCardArray[i],CountArray(m_cbTableHavaSendCardArray[i]));
			}

			CopyMemory(StatusPlay.cbTableHavaSendCount,m_cbTableHavaSendCount,CountArray(m_cbTableHavaSendCount));


			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				StatusPlay.lBankerScore=pIServerUserItem->GetUserScore();
			}
			else
				StatusPlay.lBankerScore = m_lSysBankerScore;

			//��������
			CopyMemory(StatusPlay.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusPlay.szGameRoomName));

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusPlay.cbTimeLeave = (BYTE)(m_nGameEndTime - (__min(dwPassTime, m_nGameEndTime)));
			if (cbGameStatus == GS_PLACE_JETTON)
				StatusPlay.cbTimeLeave = (BYTE)(m_nPlaceJettonTime - (__min(dwPassTime, m_nPlaceJettonTime)));
			StatusPlay.cbGameStatus=m_pITableFrame->GetGameStatus();			

			//�����ж�
			if (cbGameStatus==GS_GAME_END)
			{
				//���óɼ�
				StatusPlay.lEndUserScore=m_lUserWinScore[wChiarID];

				bool static bWinShangMen, bWinTianMen, bWinXiaMen;
				DeduceWinner(bWinShangMen, bWinTianMen, bWinXiaMen);

				if (bWinShangMen && bWinTianMen && bWinXiaMen)
					StatusPlay.cbBankerTong = 2;
				else if (!bWinShangMen && !bWinTianMen && !bWinXiaMen)
					StatusPlay.cbBankerTong = 1;

				StatusPlay.cbWinArea[1] = bWinShangMen ? TRUE : FALSE;
				StatusPlay.cbWinArea[2] = bWinTianMen ? TRUE : FALSE;
				StatusPlay.cbWinArea[3] = bWinXiaMen ? TRUE : FALSE;

				//��������
				StatusPlay.lEndUserReturnScore=m_lUserReturnScore[wChiarID];

				//����˰��
				if (m_lUserRevenue[wChiarID]>0) StatusPlay.lEndRevenue=m_lUserRevenue[wChiarID];
				else if (m_wCurrentBanker!=INVALID_CHAIR) StatusPlay.lEndRevenue=m_lUserRevenue[m_wCurrentBanker];
				else StatusPlay.lEndRevenue=0;

				//ׯ�ҳɼ�
				StatusPlay.lEndBankerScore=m_lBankerCurGameScore;

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

				//�˿���Ϣ
				CopyMemory(StatusPlay.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));

				for (int i = 0; i < 4; i++)
				{
					StatusPlay.cbCardType[i] = m_GameLogic.GetCardType(m_cbTableCardArray[i],2);
				}
			}

			//����������
			SendApplyUser( pIServerUserItem );

			OnGetUserListGameID(StatusPlay.wSeatUser);
				
			for (WORD i = 0; i < MAX_SEAT_COUNT; i++)
			{
				if (StatusPlay.wSeatUser[i] != INVALID_CHAIR)
				{
					WORD wSeatChair = StatusPlay.wSeatUser[i];
					CopyMemory(StatusPlay.lSeatUserAreaScore[i], m_lUserJettonScore[wSeatChair], sizeof(StatusPlay.lSeatUserAreaScore[i]));
				}
			}

			//���ͳ���
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem, &StatusPlay, sizeof(StatusPlay));

			//���ͻ�������Ϣ
			if (pIServerUserItem && pIServerUserItem->IsAndroidUser())
			{
				CMD_S_AndroidInit AndroidInit;
				ZeroMemory(&AndroidInit, sizeof(AndroidInit));
				memcpy(AndroidInit.szRoomName, TEXT("���˶��˸�"), sizeof(AndroidInit.szRoomName));
				memcpy(AndroidInit.szConfigName, m_szConfigFileName, sizeof(AndroidInit.szConfigName));
				m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_ANDROID_INIT, &AndroidInit, sizeof(AndroidInit));
			}
			
			return bSuccess;
		}
	}

	return false;
}

//������Ϸ��ȡ��Ϸ��¼
void CTableFrameSink::OnGetGameRecord(VOID *GameRecord)
{
	CMD_GF_TongRoomStatus *pRoomStatus = (CMD_GF_TongRoomStatus *)GameRecord;

	pRoomStatus->tagGameInfo.wTableID = m_pITableFrame->GetTableID() + 1;
	pRoomStatus->tagGameInfo.cbGameStatus = m_pITableFrame->GetGameStatus();

	pRoomStatus->tagTimeInfo.cbFreeTime = m_nFreeTime;
	pRoomStatus->tagTimeInfo.cbBetTime = m_nPlaceJettonTime;
	pRoomStatus->tagTimeInfo.cbEndTime = m_nGameEndTime;
	pRoomStatus->tagTimeInfo.cbPassTime = (DWORD)time(NULL) - m_dwJettonTime;

	//�ͻ���ֻ��ʾ48��
	int nIndex = m_nRecordFirst;

	pRoomStatus->cbRecordCount = m_nRecordLast - m_nRecordFirst;
	if (pRoomStatus->cbRecordCount > 20 || m_nRecordLast < m_nRecordFirst)
	{
		pRoomStatus->cbRecordCount = 20;
	}

	int nArrayIndex = 0;

	int nEndIndex = (m_nRecordLast + 1) % MAX_SCORE_HISTORY;

	do
	{
		if (nArrayIndex >= 20)
			break;
		pRoomStatus->GameRecordArrary[nArrayIndex].cbAreaWin[0] = m_GameRecordArrary[nIndex].bWinShangMen;
		pRoomStatus->GameRecordArrary[nArrayIndex].cbAreaWin[1] = m_GameRecordArrary[nIndex].bWinTianMen;
		pRoomStatus->GameRecordArrary[nArrayIndex].cbAreaWin[2] = m_GameRecordArrary[nIndex].bWinXiaMen;

		nArrayIndex++;
		nIndex = (nIndex + 1) % MAX_SCORE_HISTORY;
	} while (nIndex != nEndIndex);
}

//��ȡ������Ϸ�Ƿ���ע״̬���������ע��
bool CTableFrameSink::OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if (m_pITableFrame->GetGameStatus() == GAME_SCENE_FREE)
		return false;

	for (WORD wAreaIndex = 1; wAreaIndex < AREA_MAX + 1; ++wAreaIndex)
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

//��ʱ���¼�
bool  CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_FREE:		//����ʱ��
		{
			if (m_wCurrentBanker==INVALID_CHAIR)
			{
				TrySelectBanker();
			}

			m_pITableFrame->StartGame();

			//����ʱ��
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,m_nPlaceJettonTime*1000,1,0L);

			//����״̬
			m_pITableFrame->SetGameStatus(GS_PLACE_JETTON);

			//���˥��
			m_pITableFrame->CalculateStorageDeduct();

			return true;
		}
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
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_nGameEndTime*1000,1,0L);			
			}

			return true;
		}
	case IDI_GAME_END:			//������Ϸ
		{			
			//д�����
			for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);
				if ( pIServerUserItem == NULL ) continue;

				//��ׯ�ж�
				SCORE lUserScore = pIServerUserItem->GetUserScore();
				if (wUserChairID!=m_wCurrentBanker && lUserScore<m_lApplyBankerCondition)
				{
					for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
					{
						if (wUserChairID!=m_ApplyUserArray[i]) continue;

						//ɾ�����
						m_ApplyUserArray.RemoveAt(i);

						//������Ϣ
						CMD_S_CancelBanker CancelBanker;
						ZeroMemory(&CancelBanker,sizeof(CancelBanker));

						CancelBanker.wCancelUser = pIServerUserItem->GetChairID();
						
						//������Ϣ
						m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
						m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

						break;
					}
				}
			}

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);

			//�л�ׯ��
			ChangeBanker(false);

			TrySelectBanker();

			//����ʱ��
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE,m_nFreeTime*1000,1,0L);

			//������Ϣ
			CMD_S_GameFree GameFree;
			ZeroMemory(&GameFree,sizeof(GameFree));
			GameFree.cbTimeLeave=m_nFreeTime;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));

			return true;
		}
	}

	return false;
}

//ѡ��ׯ��
void CTableFrameSink::TrySelectBanker()
{
	if (m_wCurrentBanker==INVALID_CHAIR&&m_ApplyUserArray.GetCount()>0)
	{		
		m_wCurrentBanker = m_ApplyUserArray[0];

		//�л��ж�
		{
			//���ñ���
			m_wBankerTime = 0;
			m_lBankerWinScore=0;

			//������Ϣ
			CMD_S_ChangeBanker ChangeBanker;
			ZeroMemory(&ChangeBanker,sizeof(ChangeBanker));
			ChangeBanker.wBankerUser=m_wCurrentBanker;
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				ChangeBanker.lBankerScore=pIServerUserItem->GetUserScore();
			}
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&ChangeBanker,sizeof(ChangeBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&ChangeBanker,sizeof(ChangeBanker));
		}
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
			return OnUserPlaceJetton(pUserData->wChairID,pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);
		}
	case SUB_C_APPLY_BANKER:		//������ׯ
		{
			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus==US_LOOKON) return true;

			return OnUserApplyBanker(pIServerUserItem);	
		}
	case SUB_C_CANCEL_BANKER:		//ȡ����ׯ
		{
			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus==US_LOOKON) return true;

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

					for (BYTE j = 0; j < AREA_MAX; j++)
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

			CMD_C_ControlWinLost *pControlWinLost = (CMD_C_ControlWinLost *)pDataBuffer;

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

	return true;
}

//�����Ϣ����
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
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
		m_pITableFrame->SetGameTimer(IDI_FREE,m_nFreeTime*1000,1,NULL);
		m_pITableFrame->SetGameStatus(GAME_STATUS_FREE);
	}

	//���ͻ�������Ϣ
	if (pIServerUserItem && pIServerUserItem->IsAndroidUser())
	{
		CMD_S_AndroidInit AndroidInit;
		ZeroMemory(&AndroidInit, sizeof(AndroidInit));
		memcpy(AndroidInit.szRoomName, TEXT("���˶��˸�"), sizeof(AndroidInit.szRoomName));
		memcpy(AndroidInit.szConfigName, m_szConfigFileName, sizeof(AndroidInit.szConfigName));
		m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_ANDROID_INIT, &AndroidInit, sizeof(AndroidInit));
	}

	return true;
}

//�û�����
bool  CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{

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

			CancelBanker.wCancelUser = pIServerUserItem->GetChairID();

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

			break;
		}

		return true;
	}

	return true;
}

//��ע�¼�
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, SCORE lJettonScore)
{
	//Ч�����
	ASSERT((cbJettonArea <= ID_DI_MEN && cbJettonArea >= ID_SHUN_MEN) && (lJettonScore>0L));
	if ((cbJettonArea>ID_DI_MEN) || (lJettonScore <= 0L) || cbJettonArea<ID_SHUN_MEN) 
		return true;

	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON)
	{
		CMD_S_PlaceJettonFail PlaceJettonFail;
		ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
		PlaceJettonFail.lJettonArea=cbJettonArea;
		PlaceJettonFail.lPlaceScore=lJettonScore;
		PlaceJettonFail.wPlaceUser=wChairID;

		//������Ϣ
		m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
		return true;
	}

	//ׯ���ж�
	if (m_wCurrentBanker==wChairID) return true;

	//��������
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	SCORE lJettonCount = 0L;
	for (int nAreaIndex = 1; nAreaIndex <= AREA_COUNT; ++nAreaIndex) 
		lJettonCount += m_lUserJettonScore[wChairID][nAreaIndex];

	//��һ���
	SCORE lUserScore = pIServerUserItem->GetUserScore();

	//�Ϸ�У��
	if (lUserScore < lJettonCount + lJettonScore) return true;
	if (m_lUserLimitScore < lJettonCount + lJettonScore) return true;

	//�ɹ���ʶ
	bool bPlaceJettonSuccess=true;

	//�Ϸ���֤
	if (GetUserMaxJetton(wChairID) >= lJettonScore)
	{
		
		//��������֤
		if(pIServerUserItem->IsAndroidUser())
		{
			//long lTemp = lJettonScore;
			//MyDebug(_T(" [������%d ,���:%d,����: %I64d]\r\n"),pIServerUserItem->GetUserID(),GetUserMaxJetton(wChairID),lJettonScore);
			
			//��������
			if (m_lRobotAreaScore[cbJettonArea] + lJettonScore > m_lAreaLimitScore)//m_lRobotAreaLimit
				return true;

			//���ƻ�����ֻ����ע����
			if (m_wCurrentBanker != INVALID_CHAIR && bPlaceJettonSuccess)
			{
				IServerUserItem *pUser = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				if (pUser)
				{
					SCORE lBankerScore = pUser->GetUserScore();
					SCORE lRobotAllJetton = 0;
					for (int nAreaIndex = 1; nAreaIndex <= AREA_COUNT; ++nAreaIndex)
					{
						lRobotAllJetton += m_lRobotAreaScore[nAreaIndex];
					}

					if (lRobotAllJetton > ((lBankerScore * 30) / 100))
					{
						bPlaceJettonSuccess = false;
					}
				}
			}

			//ͳ�Ʒ���
			if (bPlaceJettonSuccess)
				m_lRobotAreaScore[cbJettonArea] += lJettonScore;
		}

		
		
		if (bPlaceJettonSuccess)
		{
			//������ע
			m_lAllJettonScore[cbJettonArea] += lJettonScore;
			m_lUserJettonScore[wChairID][cbJettonArea] += lJettonScore;
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
		PlaceJetton.cbJettonArea=cbJettonArea;
		PlaceJetton.lJettonScore=lJettonScore;
		PlaceJetton.bIsAndroid=pIServerUserItem->IsAndroidUser();

		PlaceJetton.lUserRestScore = lUserScore - lJettonCount;

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
	}
	else
	{
		CMD_S_PlaceJettonFail PlaceJettonFail;
		ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
		PlaceJettonFail.lJettonArea=cbJettonArea;
		PlaceJettonFail.lPlaceScore=lJettonScore;
		PlaceJettonFail.wPlaceUser=wChairID;

		//������Ϣ
		m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
	}

	return true;
}

//����ׯ��
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	//�Ϸ��ж�
	SCORE lUserScore = pIApplyServerUserItem->GetUserScore();
	if (lUserScore<m_lApplyBankerCondition)
	{
		return true;
	}

	//�����ж�
	WORD wApplyUserChairID=pIApplyServerUserItem->GetChairID();
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];
		if (wChairID==wApplyUserChairID)
		{
			return true;
		}
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
	if (m_pITableFrame->GetGameStatus()==GAME_STATUS_FREE && m_ApplyUserArray.GetCount()==1)
	{
		ChangeBanker(false);
//#ifdef _DEBUG
//
//		WORD wChairID=m_ApplyUserArray[0];
//		m_wCurrentBanker = wChairID;
//		//������Ϣ
//		CMD_S_ChangeBanker ChangeBanker;
//		ZeroMemory(&ChangeBanker,sizeof(ChangeBanker));
//		ChangeBanker.wBankerUser=m_wCurrentBanker;
//		if (m_wCurrentBanker!=INVALID_CHAIR)
//		{
//			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
//			ChangeBanker.lBankerScore=pIServerUserItem->GetUserScore();
//		}
//		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&ChangeBanker,sizeof(ChangeBanker));
//		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&ChangeBanker,sizeof(ChangeBanker));
//
//#endif
	}

	return true;
}

//ȡ������
bool CTableFrameSink::OnUserCancelBanker(IServerUserItem *pICancelServerUserItem)
{
	//��ǰׯ��
	if (pICancelServerUserItem->GetChairID()==m_wCurrentBanker && m_pITableFrame->GetGameStatus()!=GAME_STATUS_FREE)
	{
		return true;
	}

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

		if (m_wCurrentBanker!=wChairID)
		{
			//�������
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

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
	//��ׯ����
	WORD wBankerTime = m_nBankerTimeLimit;
	SCORE lBigScore = 0;

	bool bChangeBanker =false;

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

		m_wBankerTime = 0;
		m_lBankerWinScore=0;
		bChangeBanker=true;
	}

	//��ׯ�ж�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//��ȡׯ��
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

		if(pIServerUserItem!= NULL)
		{
			SCORE lBankerScore = pIServerUserItem->GetUserScore();

			//�����ж�
			if (wBankerTime<=m_wBankerTime || lBankerScore<m_lApplyBankerCondition)
			{
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

				//����ׯ��
				m_wCurrentBanker=INVALID_CHAIR;
	
				bChangeBanker=true;
			}

		}
		else
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
		}
		
	}

	if (bChangeBanker)
	{
		//���ñ���
		m_wBankerTime = 0;
		m_lBankerWinScore=0;

		//������Ϣ
		CMD_S_ChangeBanker ChangeBanker;
		ZeroMemory(&ChangeBanker,sizeof(ChangeBanker));
		ChangeBanker.wBankerUser=m_wCurrentBanker;
		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
			ChangeBanker.lBankerScore=pIServerUserItem->GetUserScore();
		}
		else
		{
			m_lSysBankerScore = 10000000;
			ChangeBanker.lBankerScore = m_lSysBankerScore;
		}
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&ChangeBanker,sizeof(ChangeBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&ChangeBanker,sizeof(ChangeBanker));

	}
	

	return true;
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

//�û�����
bool  CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) 
{
	//�л�ׯ��
//	if (wChairID==m_wCurrentBanker) ChangeBanker(true);

	//ȡ������
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		if (wChairID!=m_ApplyUserArray[i]) continue;

		//ɾ�����
		m_ApplyUserArray.RemoveAt(i);

		//�������
		CMD_S_CancelBanker CancelBanker;
		ZeroMemory(&CancelBanker,sizeof(CancelBanker));

		CancelBanker.wCancelUser = pIServerUserItem->GetChairID(); 

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

		break;
	}

	return true;
}

//�����ע
SCORE CTableFrameSink::GetUserMaxJetton(WORD wChairID)
{
	//��ȡ���
	IServerUserItem *pIMeServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	if (NULL==pIMeServerUserItem) return 0L;

	//����ע��
	SCORE lNowJetton = 0;
	ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
	
	//ׯ�ҽ��
	SCORE lBankerScore = 10000000;
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		lBankerScore=m_lBankerScore;
	}

	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
	{
		lBankerScore-=m_lAllJettonScore[nAreaIndex];
		lNowJetton += m_lUserJettonScore[wChairID][nAreaIndex];
	}

	//��������
	SCORE lMeMaxScore = min((pIMeServerUserItem->GetUserScore() - lNowJetton), m_lUserLimitScore);

	//��������
	lMeMaxScore=min(lMeMaxScore,m_lAreaLimitScore);

	//ׯ������
	lMeMaxScore=min(lMeMaxScore,lBankerScore);
	
	lMeMaxScore = max(lMeMaxScore, 0);

	return lMeMaxScore;
}

//����÷�
SCORE CTableFrameSink::CalculateScore()
{
	//��������
	float static cbRevenue=m_pGameServiceOption->wRevenueRatio;

	//�ƶ����
	bool static bWinShangMen, bWinTianMen, bWinXiaMen;
	DeduceWinner(bWinShangMen, bWinTianMen, bWinXiaMen);

	//��Ϸ��¼
	tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.bWinShangMen=bWinShangMen;
	GameRecord.bWinTianMen=bWinTianMen;
	GameRecord.bWinXiaMen=bWinXiaMen;

	//�ƶ��±�
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst ) m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
	
	//ׯ������
	SCORE lBankerWinScore = 0;

	//��ҳɼ�
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	SCORE lUserLostScore[GAME_PLAYER];
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));


	//ʤ����ʶ
	bool static bWinFlag[AREA_MAX];
	bWinFlag[0]=false;
	bWinFlag[ID_SHUN_MEN] = bWinShangMen;
	bWinFlag[ID_TIAN_MEN] = (true == bWinShangMen && true == bWinTianMen) ? true : false;
	bWinFlag[ID_DI_MEN] = (true == bWinShangMen && true == bWinXiaMen) ? true : false;

	//�������
	for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//ׯ���ж�
		if (m_wCurrentBanker==wChairID) continue;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;

		for (WORD wAreaIndex = ID_SHUN_MEN; wAreaIndex <= ID_DI_MEN; ++wAreaIndex)
		{
			if (true==bWinFlag[wAreaIndex]) 
			{
				m_lUserWinScore[wChairID] += m_lUserJettonScore[wChairID][wAreaIndex];
				lBankerWinScore -= m_lUserJettonScore[wChairID][wAreaIndex];
			}
			else
			{
				lUserLostScore[wChairID] -= m_lUserJettonScore[wChairID][wAreaIndex];
				lBankerWinScore += m_lUserJettonScore[wChairID][wAreaIndex];
			}
		}

		//����˰��
		if (0 < m_lUserWinScore[wChairID])
		{
			float fRevenuePer=float(cbRevenue/1000);
			m_lUserRevenue[wChairID] = SCORE(m_lUserWinScore[wChairID] * fRevenuePer + 0.5);
			m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];
		}
	  //	�ܵķ���
		m_lUserWinScore[wChairID] += lUserLostScore[wChairID];
	}

	//ׯ�ҳɼ�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;

		//����˰��
		if (0 < m_lUserWinScore[m_wCurrentBanker])
		{
			float fRevenuePer=float(cbRevenue/1000);
			if(cbRevenue<=0)
			{
				fRevenuePer = 0;
			}
			m_lUserRevenue[m_wCurrentBanker] = SCORE(m_lUserWinScore[m_wCurrentBanker] * fRevenuePer + 0.5);
			m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
			lBankerWinScore = m_lUserWinScore[m_wCurrentBanker];
		}		
	}


	//�ۼƻ���
	m_lBankerWinScore += lBankerWinScore;

	//��ǰ����
	m_lBankerCurGameScore=lBankerWinScore;	
	
	

	return lBankerWinScore;
}

//����ϵͳ��Ӯ��
SCORE CTableFrameSink::CalculateSystemScore()
{
	SCORE lSystemScore = 0;

	IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
	//�����ׯ
	if (pIServerUserItem != NULL && !pIServerUserItem->IsAndroidUser())
	{
		for (WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex)
		{
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
			if (pIServerUserItem == NULL) continue;

			if (pIServerUserItem->IsAndroidUser())
				lSystemScore += m_lUserWinScore[wUserIndex];
		}
	}
	else
	{
		for (WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex)
		{
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
			if (pIServerUserItem == NULL) continue;

			if (!pIServerUserItem->IsAndroidUser())
				lSystemScore -= m_lUserWinScore[wUserIndex];
		}
	}

	return lSystemScore;
}

//�Ƿ����
BYTE CTableFrameSink::AnalyseControl()
{
	//��������
	WORD wAiCount = 0;
	WORD wPlayerCount = 0;
	for (WORD i = 0; i < m_wPlayerCount; i++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem != NULL)
		{
			if (pIServerUserItem->IsAndroidUser())
			{
				wAiCount++;
			}
			wPlayerCount++;
		}
	}

	//ȫ������
	if (wPlayerCount == wAiCount || wAiCount == 0)
	{
		return 0;
	}

	SCORE				llRiseScore[STORAGE_COUNT] = { 0 };	//�����ʼֵ
	SCORE				llEndScore[STORAGE_COUNT] = { 0 };	//������ֵ
	INT						nProbability[STORAGE_COUNT] = { 0 };	//���Ƹ���

	TCHAR outBuff[64] = { 0 };
	for (int i = 0; i < STORAGE_COUNT; i++)
	{
		memset(outBuff, 0, sizeof(outBuff));
		_sntprintf_s(outBuff, sizeof(outBuff), _T("%s"), m_pGameServiceOption->pRoomStorageOption.szStorageControl[i]);

		long long llValue[3] = { 0 };
		this->GetValueFromCombStr(llValue, CountArray(llValue), outBuff, _tcslen(outBuff));
		llRiseScore[i] = llValue[0];
		llEndScore[i] = llValue[1];
		nProbability[i] = llValue[2];
	}

	SCORE lStorageCurrent = m_pGameServiceOption->pRoomStorageOption.lStorageCurrent;
	SCORE lStorageMax = m_pGameServiceOption->pRoomStorageOption.lStorageMax;
	int lStorageMul = m_pGameServiceOption->pRoomStorageOption.lStorageMul;

	//�����ж�
	int a = 0;			//���Ƶ�λ
	for (; a < STORAGE_COUNT; a++)
	{
		if (llRiseScore[a] < lStorageCurrent && llEndScore[a] >= lStorageCurrent)
			break;
	}

	//���������
	if ((a < STORAGE_COUNT && nProbability[a] > 100) || lStorageCurrent < 0)
	{
		if ((rand() % 100) < (nProbability[a] - 100) || lStorageCurrent < 0)
		{
			return 1;
		}
	}

	// �ͷ��ж�
	bool bStorageSend = a < STORAGE_COUNT && nProbability[a] <= 100 && nProbability[a] > 0 && (rand() % 100) < nProbability[a];
	if ((lStorageCurrent > 0 && lStorageCurrent > lStorageMax && rand() % 100 < lStorageMul) || bStorageSend)
	{
		return 2;
	}

	return 0;
}

void CTableFrameSink::DeduceWinner(bool &bWinShangMen, bool &bWinTianMen, bool &bWinXiaMen)
{
	//��С�Ƚ�
	bWinShangMen=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[SHANG_MEN_INDEX],2)==1?true:false;
	bWinTianMen=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[TIAN_MEN_INDEX],2)==1?true:false;
	bWinXiaMen=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],2,m_cbTableCardArray[XIA_MEN_INDEX],2)==1?true:false;
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
	if (wBufferSize>0) m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
}

//��ȡ����
void CTableFrameSink::ReadConfigInformation(bool bReadFresh,bool bReadBaseConfig)
{
	//��������
	TCHAR OutBuf[255] = {};

	//��ׯ����
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(TEXT("���˶��˸�"), TEXT("ApplyBankerCondition"), _T("5000"), OutBuf, 255, m_szConfigFileName);
	myscanf(OutBuf, mystrlen(OutBuf), _T("%lf"), &m_lApplyBankerCondition);

	//��ׯ����
	m_nBankerTimeLimit = GetPrivateProfileInt(TEXT("���˶��˸�"), TEXT("BankerTimes"), 10, m_szConfigFileName);
	
	//��ע����
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(TEXT("���˶��˸�"), TEXT("AreaLimitScore"), _T("1000000"), OutBuf, 255, m_szConfigFileName);
	myscanf(OutBuf, mystrlen(OutBuf), _T("%lf"), &m_lAreaLimitScore);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(TEXT("���˶��˸�"), TEXT("UserLimitScore"), _T("1000000"), OutBuf, 255, m_szConfigFileName);
	myscanf(OutBuf, mystrlen(OutBuf), _T("%lf"), &m_lUserLimitScore);

	//ʱ������
	m_nFreeTime = GetPrivateProfileInt(TEXT("���˶��˸�"), TEXT("FreeTime"), 10, m_szConfigFileName);
	m_nPlaceJettonTime = GetPrivateProfileInt(TEXT("���˶��˸�"), TEXT("BetTime"), 15, m_szConfigFileName);
	m_nGameEndTime = GetPrivateProfileInt(TEXT("���˶��˸�"), TEXT("EndTime"), 15, m_szConfigFileName);
	if (m_nFreeTime <= 5		|| m_nFreeTime > 99)			m_nFreeTime = 5;
	if (m_nPlaceJettonTime <= 10 || m_nPlaceJettonTime > 99)		m_nPlaceJettonTime = 10;
	if (m_nGameEndTime <= 10 || m_nGameEndTime > 99)			m_nGameEndTime = 10;
}

//��ѯ�����
bool  CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{
	for (BYTE i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem *pUserItem=m_pITableFrame->GetTableUserItem(i);
		if(pUserItem==NULL) continue;
		if(wChairID==i)
		{
			//������ע
			for (int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex) 
			{

				if (m_lUserJettonScore[wChairID][nAreaIndex] != 0)
				{
					return true;
				}
			}
			break;
		}
	}
	return false;
}
//��ѯ�޶�
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	SCORE consumeQuota=0L;
	SCORE lMinTableScore=m_pGameServiceOption->lMinTableScore;
	if(m_pITableFrame->GetGameStatus()==GAME_STATUS_FREE)
	{
		consumeQuota=pIServerUserItem->GetUserScore()-lMinTableScore;

	}
	return consumeQuota;
}

void CTableFrameSink::GetValueFromCombStr(LONGLONG llData[], int nDataLen, LPCTSTR sTr, int nStrLen)
{
	int nIndex = 0;							// ����
	TCHAR szStr[500] = TEXT("");

	int nCount = 0;							// ��ȡ��������
	for (int i = 0; i<nStrLen; i++)
	{
		if (sTr[i] != '|')
		{
			szStr[nIndex] = sTr[i];
			nIndex++;
			if (i == (int)(_tcslen(sTr) - 1))
			{
				llData[nCount] = _ttoi64(szStr);
				nCount++;
				nIndex = 0;
				memset(szStr, NULL, sizeof(szStr));
			}
			continue;
		}

		if (nCount >= nDataLen)
			return;

		llData[nCount] = _ttoi64(szStr);
		nCount++;
		nIndex = 0;
		memset(szStr, NULL, sizeof(szStr));
	}
}
//////////////////////////////////////////////////////////////////////////
