#include "StdAfx.h"
#include "TableFrameSink.h"
#include "DlgCustomRule.h"
#include <conio.h>
#include <locale>

//////////////////////////////////////////////////////////////////////////


//��̬����
const BYTE			CTableFrameSink::m_GameStartMode=START_MODE_ALL_READY;	//��ʼģʽ


//��ʱ�� 0~30
#define IDI_GAME_COMPAREEND					1									//������ʱ��
#define IDI_GAME_OPENEND					2									//������ʱ��
#define IDI_ADD_SCORE						3									//������ʱ��
#define IDI_AUTO_ADD						4									//�����׶�ʱ��
#define IDI_AUTO_START						5									//��ʼ��ʱ��
#define IDI_WAIT_START						6									//�ȴ���ʼ��ʱ��


#define TIME_GAME_COMPAREEND				2000								//������ʱ��
#define TIME_GAME_OPENEND					1000								//������ʱ��
#define TIME_ADD_SCORE						60000								//������ʱ��
#define TIME_AUTO_ADD						1000								//������ʱ��
#define TIME_AUTO_START						5000								//��ʼʱ��
#define TIME_WAIT_START						4000								//�ȴ���ʼʱ��
//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	m_wPlayerCount = GAME_PLAYER;
	m_bOperaCount=0;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_cbShowHandStatus = false;

	m_cbGameTurns = 1;

	m_bWriteRoomStorage = true;

	//�û�״̬
	ZeroMemory(&m_StGameEnd,sizeof(m_StGameEnd));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_cbGiveUpUser,sizeof(m_cbGiveUpUser));
	for(int i=0;i<GAME_PLAYER;i++)
	{
		m_bMingZhu[i]=false;
	}
	ZeroMemory(m_cbRealPlayer, sizeof(m_cbRealPlayer));
	ZeroMemory(m_cbAndroidStatus, sizeof(m_cbAndroidStatus));

	ZeroMemory(m_bFleeUser, sizeof(m_bFleeUser));

	//�˿˱���
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	FillMemory(m_wCompardUser, sizeof(m_wCompardUser),INVALID_CHAIR);
	ZeroMemory(m_cbCompareCount, sizeof(m_cbCompareCount));

	//��ע��Ϣ
	m_lMaxCellScore=0L;
	m_lCurrentTimes=0L;		
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lUserMaxScore,sizeof(m_lUserMaxScore));
	ZeroMemory(m_lShowHandScore, sizeof(m_lShowHandScore));
	ZeroMemory(m_bFollowStatus, sizeof(m_bFollowStatus));

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;	

	ZeroMemory(&m_pCustomAndroid, sizeof(m_pCustomAndroid));

	//�������
	srand(time(NULL));

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

//�ӿڲ�ѯ--��������Ϣ�汾
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

	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_ALL_READY);

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//��ȡ����
	ReadConfigInformation();

	//��ע����
	m_lCellScore = m_pITableFrame->GetCellScore();

	TCHAR szConfigFileName[128] = TEXT("");
	//�����ļ���
	TCHAR szPath[MAX_PATH] = TEXT("");
	GetCurrentDirectory(sizeof(szPath), szPath);
	_sntprintf(szConfigFileName, sizeof(szConfigFileName), TEXT("%s\\BaiRenConfig\\ZaJinHuaServer-%d.ini"), szPath, m_pGameServiceOption->wServerID);

	m_pCustomAndroid.nLookCard = GetPrivateProfileInt(TEXT("����"), TEXT("nLookCard"), -1, szConfigFileName);
	m_pCustomAndroid.nSingleGiveup = GetPrivateProfileInt(TEXT("����"), TEXT("nSingleGiveup"), -1, szConfigFileName);
	m_pCustomAndroid.nShunziGiveup = GetPrivateProfileInt(TEXT("����"), TEXT("nShunziGiveup"), -1, szConfigFileName);
	m_pCustomAndroid.nLoserGiveup = GetPrivateProfileInt(TEXT("����"), TEXT("nLoserGiveup"), -1, szConfigFileName);
	m_pCustomAndroid.nLoserCompare = GetPrivateProfileInt(TEXT("����"), TEXT("nLoserCompare"), -1, szConfigFileName);
	m_pCustomAndroid.nWinnerCompare = GetPrivateProfileInt(TEXT("����"), TEXT("nWinnerCompare"), -1, szConfigFileName);

	if (m_pCustomAndroid.nLookCard == -1 || m_pCustomAndroid.nSingleGiveup == -1 || m_pCustomAndroid.nShunziGiveup == -1 ||
		m_pCustomAndroid.nLoserGiveup == -1 || m_pCustomAndroid.nLoserCompare == -1 || m_pCustomAndroid.nWinnerCompare == -1)
	{
		CTraceService::TraceString(TEXT("��ȡ�����ļ�ʧ�ܣ�"),TraceLevel_Warning);
		return false;
	}

	return true;
}

//��λ����
void  CTableFrameSink::RepositionSink()
{
	//��Ϸ����
	m_bOperaCount=0;
	m_wCurrentUser=INVALID_CHAIR;

	m_cbGameTurns = 1;
	m_cbShowHandStatus = false;

	m_bWriteRoomStorage = true;

	//�û�״̬
	ZeroMemory(&m_StGameEnd,sizeof(m_StGameEnd));
	ZeroMemory(m_cbGiveUpUser,sizeof(m_cbGiveUpUser));
	for(int i=0;i<m_wPlayerCount;i++)
	{
		m_bMingZhu[i]=false;	
	}
	ZeroMemory(m_cbRealPlayer, sizeof(m_cbRealPlayer));
	ZeroMemory(m_cbAndroidStatus, sizeof(m_cbAndroidStatus));

	//��ע��Ϣ
	m_lMaxCellScore=0L;							
	m_lCurrentTimes=0L;		
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lUserMaxScore,sizeof(m_lUserMaxScore));
	ZeroMemory(m_lShowHandScore, sizeof(m_lShowHandScore));
	ZeroMemory(m_bFollowStatus, sizeof(m_bFollowStatus));

	return;
}

//�����¼�
bool CTableFrameSink::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	

	return false;
}

//��ʼģʽ
BYTE  CTableFrameSink::GetGameStartMode()
{
	return m_GameStartMode;
}

//�û�����
bool CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{

	return true;
}

//��Ϸ��ʼ
bool  CTableFrameSink::OnEventGameStart()
{
	BYTE cbUserCount = 0;
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
		if (pServerUser)
		{
			cbUserCount++;
		}
	}

	FillMemory(m_wCompardUser, sizeof(m_wCompardUser), INVALID_CHAIR);
	ZeroMemory(m_cbCompareCount, sizeof(m_cbCompareCount));
	ZeroMemory(m_cbPlayStatus, sizeof(m_cbPlayStatus));

	ZeroMemory(m_bFleeUser, sizeof(m_bFleeUser));

	//�˿˱���
	ZeroMemory(m_cbHandCardData, sizeof(m_cbHandCardData));
	m_cbGameTurns = 1;
	m_cbShowHandStatus = false;

	CMD_S_WaitStart WaitStart;
	ZeroMemory(&WaitStart, sizeof(WaitStart));

	WaitStart.cbUserCount = cbUserCount;

	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
		if (pServerUser)
		{
			WaitStart.lPlayerScore = pServerUser->GetUserScore();
			m_pITableFrame->SendTableData(i, SUB_S_WAIT_START, &WaitStart, sizeof(WaitStart));
			m_pITableFrame->SendLookonData(i, SUB_S_WAIT_START);
		}
	}

	m_pITableFrame->SetGameTimer(IDI_AUTO_START, TIME_AUTO_START, 1, 0);
	
	m_dwJettonTime = (DWORD)time(NULL);

	//����״̬
	m_pITableFrame->SetGameStatus(GAME_STATUS_WAIT);
	return true;
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_COMPARECARD:	//���ƽ���
	case GER_NO_PLAYER:		//û�����
		{
			m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);
			m_pITableFrame->KillGameTimer(IDI_AUTO_ADD);

			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
	
			//Ψһ���
			WORD wWinner,wUserCount=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{	
				if (m_cbPlayStatus[i] == PlayerPlaying || m_cbPlayStatus[i] == PlayerLookCard)
				{
					wUserCount++;
					wWinner=i;
					m_wBankerUser=i;
				}
			}

			//ʤ����ǿ��
			if (wUserCount==0)
			{
				wWinner=m_wBankerUser;
			}

			//�ӳٿ�ʼ
			bool bDelayOverGame = false;

			//������ע
			SCORE lWinnerScore = 0;
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{
				if(i==wWinner)continue;
				GameEnd.lGameScore[i]=-m_lTableScore[i];
				lWinnerScore+=m_lTableScore[i];
			}

			//����˰��
			SCORE lGameTax = m_pITableFrame->CalculateRevenue(wWinner, lWinnerScore);
			GameEnd.lGameScore[wWinner]=lWinnerScore-lGameTax;

			//ʤ����ǿ��
			if (wUserCount==1)
			{
				//�޸Ļ���
				tagScoreInfo ScoreInfo;
				
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));

					if (m_cbPlayStatus[i] != PlayerPlaying && m_cbPlayStatus[i] != PlayerLookCard)
						continue;
					ScoreInfo.cbType =(GameEnd.lGameScore[i]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
					ScoreInfo.lRevenue=lGameTax;                                  
					ScoreInfo.lScore=GameEnd.lGameScore[i];

					if (ScoreInfo.cbType == SCORE_TYPE_LOSE && bDelayOverGame == false)
					{
						IServerUserItem * pUserItem = m_pITableFrame->GetTableUserItem(i);
						if (pUserItem!=NULL && (pUserItem->GetUserScore()+GameEnd.lGameScore[i])<m_pGameServiceOption->lMinTableScore)
						{
							bDelayOverGame = true;
						}
					}

					m_pITableFrame->WriteUserScore(i, ScoreInfo);

					//���ֵ
					IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);
					if (pServerUser && pServerUser->IsAndroidUser() && m_bWriteRoomStorage)
						m_pGameServiceOption->pRoomStorageOption.lStorageCurrent += ScoreInfo.lScore;
				}
			}

			//�����������Ϣ
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
				if (pServerUser && GameEnd.lGameScore[i] > 500)
				{
					CMD_GR_GameRadioMessage GameRadioMessage;
					ZeroMemory(&GameRadioMessage, sizeof(GameRadioMessage));

					BYTE cbCardType = m_GameLogic.GetCardType(m_cbHandCardData[i], MAX_COUNT);

					if (cbCardType == CT_SPECIAL)
						cbCardType = CT_SINGLE;

					GameRadioMessage.cbCardType = cbCardType;
					GameRadioMessage.wKindID = m_pGameServiceOption->wKindID;
					GameRadioMessage.lScore = GameEnd.lGameScore[i];
					lstrcpyn(GameRadioMessage.szNickName, pServerUser->GetNickName(), CountArray(GameRadioMessage.szNickName));
					m_pITableFrame->SendHallRadioMessage(&GameRadioMessage, sizeof(GameRadioMessage));
				}
			}

			if(wChairID==GAME_PLAYER)
			{
				//��������
				CopyMemory(&m_StGameEnd,&GameEnd,sizeof(m_StGameEnd));
				m_pITableFrame->SetGameTimer(IDI_GAME_COMPAREEND,TIME_GAME_COMPAREEND,1,0);
			}
			else
			{
				//������Ϣ
				for (WORD i = 0; i < GAME_PLAYER; i++)
				{
					if (m_cbPlayStatus[i] == PlayerNoPlay)
						continue;

					IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);

					if (pServerUser == NULL)
						continue;

					CopyMemory(GameEnd.cbCardData[i], m_cbHandCardData[i], sizeof(GameEnd.cbCardData[i]));
					GameEnd.cbCardType[i] = m_GameLogic.GetCardType(GameEnd.cbCardData[i], MAX_COUNT);

					for (int j = 0; j < m_cbCompareCount[i]; j++)
					{
						if (m_wCompardUser[i][j] != INVALID_CHAIR)
						{
							WORD cbUser = m_wCompardUser[i][j];
							CopyMemory(GameEnd.cbCardData[cbUser], m_cbHandCardData[cbUser], sizeof(GameEnd.cbCardData[cbUser]));
							GameEnd.cbCardType[cbUser] = m_GameLogic.GetCardType(GameEnd.cbCardData[cbUser], MAX_COUNT);
						}
					}

					for (WORD j = 0; j < GAME_PLAYER; j++)
					{
						if (j == i)
							continue;
						if (m_cbPlayStatus[i] == PlayerPlaying || m_cbPlayStatus[i] == PlayerLookCard)
						{
							if (m_cbPlayStatus[j] == PlayerPlaying || m_cbPlayStatus[j] == PlayerLookCard)
							{
								CopyMemory(GameEnd.cbCardData[j], m_cbHandCardData[j], sizeof(GameEnd.cbCardData[j]));
								GameEnd.cbCardType[j] = m_GameLogic.GetCardType(GameEnd.cbCardData[j], MAX_COUNT);
							}
						}
					}

					m_pITableFrame->SendTableData(i, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));

					ZeroMemory(GameEnd.cbCardData, sizeof(GameEnd.cbCardData));
					ZeroMemory(GameEnd.cbCardType, sizeof(GameEnd.cbCardType));
				}
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

				m_pITableFrame->SetGameStatus(GAME_STATUS_WAIT);

				m_pITableFrame->SetGameTimer(IDI_WAIT_START, TIME_WAIT_START, 1, 0);
			}

			

			return true;
		}
	case GER_USER_LEAVE:		//�û�ǿ��
	case GER_NETWORK_ERROR:		//�����ж�
		{
			if (m_cbPlayStatus[wChairID] == PlayerNoPlay || m_cbPlayStatus[wChairID] == PlayerGiveUp || m_cbPlayStatus[wChairID] == PlayerComFail)
				return true;

			if (m_pITableFrame->GetGameStatus() != GAME_STATUS_PLAY)
				return true;

			//��������
			m_cbGiveUpUser[wChairID] = TRUE;
			m_cbPlayStatus[wChairID] = PlayerGiveUp;

			bool bCurrentGiveUp = false;

			if (wChairID == m_wCurrentUser)
			{
				bCurrentGiveUp = true;

				//��ȡ���һ����ҵ�λ��
				WORD wFinalUser = m_wBankerUser;
				for (BYTE i = 0; i < m_wPlayerCount; i++)
				{
					if (m_cbPlayStatus[wFinalUser] == PlayerNoPlay || m_cbPlayStatus[wFinalUser] == PlayerGiveUp || m_cbPlayStatus[wFinalUser] == PlayerComFail)
					{
						wFinalUser = (wFinalUser - 1 + m_wPlayerCount) % m_wPlayerCount;
					}
					else
						break;
				}

				//���һ����ҷ���������++
				if (wChairID == wFinalUser)
				{
					m_cbGameTurns++;
				}

				//�û��л�
				WORD wNextPlayer = INVALID_CHAIR;
				for (WORD i = 1; i < m_wPlayerCount; i++)
				{
					//���ñ���
					wNextPlayer = (m_wCurrentUser + i) % m_wPlayerCount;

					//�����ж�
					if (m_cbPlayStatus[wNextPlayer] == PlayerPlaying || m_cbPlayStatus[wNextPlayer] == PlayerLookCard)
						break;
				}
				m_wCurrentUser = wNextPlayer;

			}

			if (m_cbGameTurns >= MAX_ROUND)
			{
				m_wCurrentUser = INVALID_CHAIR;
			}

			//������Ϣ
			CMD_S_GiveUp GiveUp;
			GiveUp.wGiveUpUser = wChairID;
			GiveUp.wCurrentUser = bCurrentGiveUp ? m_wCurrentUser : INVALID_CHAIR;
			GiveUp.cbGameTurns = m_cbGameTurns;
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GIVE_UP, &GiveUp, sizeof(GiveUp));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GIVE_UP, &GiveUp, sizeof(GiveUp));

			//�޸Ļ���
			tagScoreInfo ScoreInfo;
			ZeroMemory(&ScoreInfo, sizeof(ScoreInfo));
			ScoreInfo.cbType = SCORE_TYPE_FLEE;
			ScoreInfo.lRevenue = 0;
			ScoreInfo.lScore = -m_lTableScore[wChairID];

			m_pITableFrame->WriteUserScore(wChairID, ScoreInfo);

			//���ֵ
			IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(wChairID);
			if (pServerUser && pServerUser->IsAndroidUser() && m_bWriteRoomStorage)
				m_pGameServiceOption->pRoomStorageOption.lStorageCurrent += ScoreInfo.lScore;

			//����ͳ��
			WORD wPlayerCount = 0;
			for (WORD i = 0; i<m_wPlayerCount; i++)
			{
				if (m_cbPlayStatus[i] == PlayerPlaying || m_cbPlayStatus[i] == PlayerLookCard) wPlayerCount++;
			}

			//�жϽ���
			if (wPlayerCount <= 1)
				OnEventGameConclude(INVALID_CHAIR, NULL, GER_NO_PLAYER);

			//�����ж�
			if (m_cbGameTurns >= MAX_ROUND)
			{
				OnEventGameConclude(GAME_PLAYER, NULL, GER_OPENCARD);
				return true;
			}

			//ǿ�˴���
			return true;
		}
	case GER_OPENCARD:		//���ƽ���   
		{
			m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);
			m_pITableFrame->KillGameTimer(IDI_AUTO_ADD);

			//��������
			m_wCurrentUser = INVALID_CHAIR;

			//�����˿�
			BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
			CopyMemory(cbUserCardData, m_cbHandCardData, sizeof(cbUserCardData));

			//�������
			WORD wWinner = 0;

			//����������
			for (WORD i = 1; i<m_wPlayerCount; i++)
			{
				WORD w = (wWinner + i) % m_wPlayerCount;

				//�û�����
				if (m_cbPlayStatus[w] == PlayerNoPlay || m_cbPlayStatus[w] == PlayerGiveUp || m_cbPlayStatus[w] == PlayerComFail)
					continue;

				if (m_cbPlayStatus[wWinner] == PlayerNoPlay || m_cbPlayStatus[wWinner] == PlayerGiveUp || m_cbPlayStatus[wWinner] == PlayerComFail)
				{
					wWinner = w;
					continue;
				}

				//�Ա��˿�
				if (m_GameLogic.CompareCard(cbUserCardData[w], cbUserCardData[wWinner], MAX_COUNT) >= TRUE)
				{
					wWinner = w;
				}
			}
			
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//���ȫѹ״̬ Ҫ��Я����ҵĲ���������
			if (m_cbShowHandStatus)
			{
				SCORE lWinnerShowScore = m_lShowHandScore[wWinner];
				for (WORD i = 0; i < GAME_PLAYER; i++)
				{
					if (m_lShowHandScore[i] > 0 && i != wWinner)
					{
						if (m_lShowHandScore[i] > m_lShowHandScore[wWinner])
						{
							SCORE lSubShowScore = m_lShowHandScore[i] - m_lShowHandScore[wWinner];
							m_lTableScore[i] -= lSubShowScore;
						}
					}
				}
			}

			//�������
			SCORE lWinnerScore = 0;
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{
				if(i==wWinner)continue;
				lWinnerScore+=m_lTableScore[i];
				GameEnd.lGameScore[i]=-m_lTableScore[i];
			}

			//����˰��
			SCORE lGameTax = m_pITableFrame->CalculateRevenue(wWinner, lWinnerScore);
			GameEnd.lGameScore[wWinner]=lWinnerScore-lGameTax;

			//�޸Ļ���
			tagScoreInfo ScoreInfo;
			ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
			for (WORD i=0; i<m_wPlayerCount; i++) 
			{			
				ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
				if (i == wWinner || m_cbPlayStatus[i] == PlayerNoPlay || m_cbPlayStatus[i] == PlayerGiveUp || m_cbPlayStatus[i] == PlayerComFail)
					continue;
				ScoreInfo.lScore=GameEnd.lGameScore[i];
				ScoreInfo.cbType=SCORE_TYPE_LOSE;

				m_pITableFrame->WriteUserScore(i, ScoreInfo);

				//���ֵ
				IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);
				if (pServerUser && pServerUser->IsAndroidUser() && m_bWriteRoomStorage)
					m_pGameServiceOption->pRoomStorageOption.lStorageCurrent += ScoreInfo.lScore;
			}

			ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
			ScoreInfo.lScore=GameEnd.lGameScore[wWinner];
			ScoreInfo.cbType=SCORE_TYPE_WIN;
			ScoreInfo.lRevenue=lGameTax;
	
			m_pITableFrame->WriteUserScore(wWinner, ScoreInfo);

			//���ֵ
			IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(wWinner);
			if (pServerUser && pServerUser->IsAndroidUser() && m_bWriteRoomStorage)
				m_pGameServiceOption->pRoomStorageOption.lStorageCurrent += ScoreInfo.lScore;

			//�����������Ϣ
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
				if (pServerUser && GameEnd.lGameScore[i] > 500)
				{
					CMD_GR_GameRadioMessage GameRadioMessage;
					ZeroMemory(&GameRadioMessage, sizeof(GameRadioMessage));

					BYTE cbCardType = m_GameLogic.GetCardType(m_cbHandCardData[i], MAX_COUNT);

					if (cbCardType == CT_SPECIAL)
						cbCardType = CT_SINGLE;

					GameRadioMessage.cbCardType = cbCardType;
					GameRadioMessage.wKindID = m_pGameServiceOption->wKindID;
					GameRadioMessage.lScore = GameEnd.lGameScore[i];
					lstrcpyn(GameRadioMessage.szNickName, pServerUser->GetNickName(), CountArray(GameRadioMessage.szNickName));
					m_pITableFrame->SendHallRadioMessage(&GameRadioMessage, sizeof(GameRadioMessage));
				}
			}

			if(wChairID==GAME_PLAYER)
			{
				//��������
				CopyMemory(&m_StGameEnd,&GameEnd,sizeof(m_StGameEnd));
				m_pITableFrame->SetGameTimer(IDI_GAME_OPENEND,TIME_GAME_OPENEND,1,0);
			}
			else
			{
				//������Ϣ
				for (WORD i = 0; i < GAME_PLAYER; i++)
				{
					if (m_cbPlayStatus[i] == PlayerNoPlay)
						continue;

					IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);

					if (pServerUser == NULL)
						continue;

					CopyMemory(GameEnd.cbCardData[i], m_cbHandCardData[i], sizeof(GameEnd.cbCardData[i]));
					GameEnd.cbCardType[i] = m_GameLogic.GetCardType(GameEnd.cbCardData[i], MAX_COUNT);

					for (int j = 0; j < m_cbCompareCount[i]; j++)
					{
						if (m_wCompardUser[i][j] != INVALID_CHAIR)
						{
							WORD cbUser = m_wCompardUser[i][j];
							CopyMemory(GameEnd.cbCardData[cbUser], m_cbHandCardData[cbUser], sizeof(GameEnd.cbCardData[cbUser]));
							GameEnd.cbCardType[cbUser] = m_GameLogic.GetCardType(GameEnd.cbCardData[cbUser], MAX_COUNT);
						}
					}

					for (WORD j = 0; j < GAME_PLAYER; j++)
					{
						if (j == i)
							continue;

						if (m_cbPlayStatus[i] == PlayerPlaying || m_cbPlayStatus[i] == PlayerLookCard)
						{
							if (m_cbPlayStatus[j] == PlayerPlaying || m_cbPlayStatus[j] == PlayerLookCard)
							{
								CopyMemory(GameEnd.cbCardData[j], m_cbHandCardData[j], sizeof(GameEnd.cbCardData[j]));
								GameEnd.cbCardType[j] = m_GameLogic.GetCardType(GameEnd.cbCardData[j], MAX_COUNT);
							}
						}
					}

					m_pITableFrame->SendTableData(i, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));

					ZeroMemory(GameEnd.cbCardData, sizeof(GameEnd.cbCardData));
					ZeroMemory(GameEnd.cbCardType, sizeof(GameEnd.cbCardType));
				}
				m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));

				m_pITableFrame->SetGameStatus(GAME_STATUS_WAIT);

				m_pITableFrame->SetGameTimer(IDI_WAIT_START, TIME_WAIT_START, 1, 0);
			}
			
			
			return true;
		}
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);
			m_pITableFrame->KillGameTimer(IDI_AUTO_ADD);

			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			m_pITableFrame->SetGameStatus(GAME_STATUS_WAIT);

			m_pITableFrame->SetGameTimer(IDI_WAIT_START, TIME_WAIT_START, 1, 0);

			return true;
		}
	}

	return false;
}

//���ͳ���
bool  CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:		//����״̬
	case GAME_STATUS_WAIT:		//�ȴ���ʼ״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			if (cbGameStatus == GAME_STATUS_WAIT)
			{
				DWORD dwRestTime = (DWORD)time(NULL) - m_dwJettonTime;
				StatusFree.nTimeLeave = (DWORD)(TIME_AUTO_START/1000 - __min(TIME_AUTO_START / 1000, dwRestTime));
			}

			//���ñ���
			StatusFree.nTotalTime = TIME_ADD_SCORE / 1000;
			
			StatusFree.lCellScore = m_lCellScore;
			StatusFree.lPlayerScore = pIServerUserItem->GetUserScore();
			StatusFree.nMaxRound = MAX_ROUND;

			//��ȡ�Զ�������			
			if (pIServerUserItem->IsAndroidUser())
			{
				m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_ANDROID_CONFIG, &m_pCustomAndroid, sizeof(m_pCustomAndroid));
			}
			

			//���ͳ���
			bool bResult = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

			return bResult;
		}
	case GAME_STATUS_PLAY:	//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay, sizeof(StatusPlay));
			
			DWORD dwRestTime = (DWORD)time(NULL) - m_dwJettonTime;
			StatusPlay.nTimeLeave = (DWORD)(TIME_ADD_SCORE/1000 - (__min(TIME_ADD_SCORE / 1000, dwRestTime)));
			StatusPlay.nTotalTime = TIME_ADD_SCORE / 1000;
			//��ע��Ϣ
			StatusPlay.lPlayerScore = m_lUserMaxScore[wChairID];
			StatusPlay.lCellScore=m_lCellScore;

			StatusPlay.lMinScore = m_lCurrentTimes;
			StatusPlay.lMaxScore = __min(10 * m_lCellScore, m_lUserMaxScore[wChairID]);

			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				StatusPlay.lAllTableScore += m_lTableScore[i];
			}

			StatusPlay.nCurrentRound = m_cbGameTurns;
			StatusPlay.nMaxRound = MAX_ROUND;

			StatusPlay.cbFollowStatus = m_bFollowStatus[wChairID] ? TRUE : FALSE;

			//���ñ���
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			CopyMemory(StatusPlay.cbPlayStatus,m_cbPlayStatus,sizeof(m_cbPlayStatus));

			CopyMemory(StatusPlay.lPlayerBetScore, m_lTableScore, sizeof(StatusPlay.lPlayerBetScore));

			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				if (m_lShowHandScore[i] > 0)
				{
					StatusPlay.cbPlayStatus[i] = PlayerShowHand;
				}
			}

			//��ȡ�Զ�������			
			if (pIServerUserItem->IsAndroidUser())
			{
				m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_ANDROID_CONFIG, &m_pCustomAndroid, sizeof(m_pCustomAndroid));
			}

			//�����˿�
			if(m_bMingZhu[wChairID])
				CopyMemory(&StatusPlay.cbHandCardData,&m_cbHandCardData[wChairID],MAX_COUNT);

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	//Ч�����
	ASSERT(FALSE);

	return false;
}

//��ȡ������Ϸ�Ƿ���ע״̬���������ע�� ����true���ж�������
bool CTableFrameSink::OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	bool bPlay = m_cbPlayStatus[wChairID] == PlayerPlaying || m_cbPlayStatus[wChairID] == PlayerLookCard || m_cbPlayStatus[wChairID] == PlayerShowHand;
	if ((bPlay && m_pITableFrame->GetGameStatus() == GAME_STATUS_PLAY) || m_pITableFrame->GetGameStatus() == GAME_STATUS_WAIT)
		return true;
	else
		return false;
}

//��ʱ���¼� 
bool  CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	//������ʱ
	if(wTimerID==IDI_GAME_COMPAREEND || IDI_GAME_OPENEND==wTimerID)
	{
		//ɾ��ʱ��
		if(wTimerID==IDI_GAME_COMPAREEND)m_pITableFrame->KillGameTimer(IDI_GAME_COMPAREEND);
		else m_pITableFrame->KillGameTimer(IDI_GAME_OPENEND);

		//������Ϣ
		for (WORD i = 0; i < GAME_PLAYER; i++)
		{
			if (m_cbPlayStatus[i] == PlayerNoPlay)
				continue;

			IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);

			if (pServerUser == NULL)
				continue;

			CopyMemory(m_StGameEnd.cbCardData[i], m_cbHandCardData[i], sizeof(m_StGameEnd.cbCardData[i]));
			m_StGameEnd.cbCardType[i] = m_GameLogic.GetCardType(m_StGameEnd.cbCardData[i], MAX_COUNT);

			for (int j = 0; j < m_cbCompareCount[i]; j++)
			{
				if (m_wCompardUser[i][j] != INVALID_CHAIR)
				{
					WORD cbUser = m_wCompardUser[i][j];
					CopyMemory(m_StGameEnd.cbCardData[cbUser], m_cbHandCardData[cbUser], sizeof(m_StGameEnd.cbCardData[cbUser]));
					m_StGameEnd.cbCardType[cbUser] = m_GameLogic.GetCardType(m_StGameEnd.cbCardData[cbUser], MAX_COUNT);
				}
			}

			for (WORD j = 0; j < GAME_PLAYER; j++)
			{
				if (j == i)
					continue;

				if (m_cbPlayStatus[i] == PlayerPlaying || m_cbPlayStatus[i] == PlayerLookCard)
				{
					if (m_cbPlayStatus[j] == PlayerPlaying || m_cbPlayStatus[j] == PlayerLookCard)
					{
						CopyMemory(m_StGameEnd.cbCardData[j], m_cbHandCardData[j], sizeof(m_StGameEnd.cbCardData[j]));
						m_StGameEnd.cbCardType[j] = m_GameLogic.GetCardType(m_StGameEnd.cbCardData[j], MAX_COUNT);
					}
				}
			}

			m_pITableFrame->SendTableData(i, SUB_S_GAME_END, &m_StGameEnd, sizeof(m_StGameEnd));

			ZeroMemory(m_StGameEnd.cbCardData, sizeof(m_StGameEnd.cbCardData));
			ZeroMemory(m_StGameEnd.cbCardType, sizeof(m_StGameEnd.cbCardType));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_END, &m_StGameEnd, sizeof(m_StGameEnd));

		m_pITableFrame->SetGameStatus(GAME_STATUS_WAIT);

		m_pITableFrame->SetGameTimer(IDI_WAIT_START, TIME_WAIT_START, 1, 0);
		return true;
	}
	if(wTimerID==IDI_ADD_SCORE)
	{
		m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);
		if (m_wCurrentUser!=INVALID_CHAIR)
		{
			m_bFleeUser[m_wCurrentUser] = true;
			OnUserGiveUp(m_wCurrentUser,false);
		}		
		return true;
	}
	if (wTimerID == IDI_AUTO_ADD)
	{
		m_pITableFrame->KillGameTimer(IDI_AUTO_ADD);
		if (m_wCurrentUser != INVALID_CHAIR && !m_cbShowHandStatus)
		{
			SCORE lScore = m_lCurrentTimes * m_lCellScore;
			if (m_bMingZhu[m_wCurrentUser])
				lScore *= 2;
			OnUserAddScore(m_wCurrentUser, lScore);
		}
		return true;
	}
	if (wTimerID == IDI_WAIT_START)
	{
		m_pITableFrame->KillGameTimer(IDI_WAIT_START);

		//������Ϸ
		m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);

		for (WORD i = 0; i < GAME_PLAYER; i++)
		{
			if (m_bFleeUser[i])
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
	if (wTimerID == IDI_AUTO_START)
	{
		m_pITableFrame->KillGameTimer(IDI_AUTO_START);

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
	return false;
}

//��Ϸ��Ϣ����
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_GIVE_UP:			//�û�����
		{
			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

			//״̬�ж�
			if (m_cbPlayStatus[pUserData->wChairID] == PlayerNoPlay || m_cbPlayStatus[pUserData->wChairID] == PlayerGiveUp || m_cbPlayStatus[pUserData->wChairID] == PlayerComFail)
			{
				return true;
			}

			//��Ϣ����
			return OnUserGiveUp(pUserData->wChairID);
		}
	case SUB_C_LOOK_CARD:		//�û�����
		{
			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

			//״̬�ж�
			if (m_cbPlayStatus[pUserData->wChairID] != PlayerPlaying)
				return true;

			//��Ϣ����
			return OnUserLookCard(pUserData->wChairID);
		}
	case SUB_C_COMPARE_CARD:	//�û�����
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_CompareCard));
			if (wDataSize!=sizeof(CMD_C_CompareCard)) return false;

			//��������
			CMD_C_CompareCard * pCompareCard=(CMD_C_CompareCard *)pData;

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

			//����Ч��
			ASSERT(pUserData->wChairID==m_wCurrentUser);
			if (pUserData->wChairID != m_wCurrentUser)
			{
				CMD_S_FailResult FailResult;
				_sntprintf(FailResult.szInfo, sizeof(FailResult.szInfo), TEXT("��û���ֵ���˵�������ܱ��ƣ�"));
				m_pITableFrame->SendTableData(pUserData->wChairID, SUB_S_PLAYER_COMMPARE_FALURE, &FailResult, sizeof(FailResult));
				return true;
			}

			//״̬�ж�
			if ( (m_cbPlayStatus[pUserData->wChairID] != PlayerPlaying && m_cbPlayStatus[pUserData->wChairID] != PlayerLookCard)
				|| (m_cbPlayStatus[pCompareCard->wCompareUser] != PlayerPlaying && m_cbPlayStatus[pCompareCard->wCompareUser] != PlayerLookCard))
			{
				CMD_S_FailResult FailResult;
				_sntprintf(FailResult.szInfo, sizeof(FailResult.szInfo), TEXT("������Ҫ���ƵĶ�������Ϸ״̬�����ܱ��ƣ�"));
				m_pITableFrame->SendTableData(pUserData->wChairID, SUB_S_PLAYER_COMMPARE_FALURE, &FailResult, sizeof(FailResult));
				return true;
			}

			//��Ϣ����
			return OnUserCompareCard(pUserData->wChairID,pCompareCard->wCompareUser);
		}
	case SUB_C_ADD_SCORE:		//�û���ע
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_AddScore));
			if (wDataSize!=sizeof(CMD_C_AddScore)) 
				return false;

			//��������
			CMD_C_AddScore * pAddScore=(CMD_C_AddScore *)pData;

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

			//״̬�ж�
			if (m_cbPlayStatus[pUserData->wChairID] == PlayerNoPlay || m_cbPlayStatus[pUserData->wChairID] == PlayerGiveUp || m_cbPlayStatus[pUserData->wChairID] == PlayerComFail)
			{
				CMD_S_FailResult FailResult;
				_sntprintf(FailResult.szInfo, sizeof(FailResult.szInfo),TEXT("����ǰ������Ϸ״̬��������ע��"));
				m_pITableFrame->SendTableData(pUserData->wChairID, SUB_S_PLAYER_BET_FALURE, &FailResult, sizeof(FailResult));
				return true;
			}

			//��Ϣ����
			return OnUserAddScore(pUserData->wChairID,pAddScore->lScore);
		}
	case SUB_C_OPEN_CARD:
		{
			//�û�Ч��
			tagUserInfo * pUserData = pIServerUserItem->GetUserInfo();

			return OnUserOpenCard(pUserData->wChairID);
		}
	case SUB_C_FOLLOW_END:
		{
			//�û�Ч��
			tagUserInfo * pUserData = pIServerUserItem->GetUserInfo();

			WORD wChairID = pUserData->wChairID;

			if (m_cbPlayStatus[wChairID] != PlayerPlaying && m_cbPlayStatus[wChairID] != PlayerLookCard)
				return true;

			m_bFollowStatus[wChairID] = m_bFollowStatus[wChairID] ? false : true;

			CMD_S_Follow pFollow;
			pFollow.cbFollowStatus = m_bFollowStatus[wChairID] ? TRUE : FALSE;

			m_pITableFrame->SendTableData(wChairID, SUB_S_PLAYER_FOLLOW_END, &pFollow, sizeof(pFollow));

			return true;
		}
	case SUB_C_PLAYER_ALLIN:
		{
			//�û�Ч��
			tagUserInfo * pUserData = pIServerUserItem->GetUserInfo();

			WORD wChairID = pUserData->wChairID;

			if (m_cbPlayStatus[wChairID] != PlayerPlaying && m_cbPlayStatus[wChairID] != PlayerLookCard)
				return true;

			return OnUserShowHand(wChairID);
		}
	case SUB_C_REQUEST_CONTROL:		//������Ϣ����
		{
			//Ȩ���ж�
			if (CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) == false)
				return true;

			CMD_S_ControlInfo ControlInfo;
			ZeroMemory(&ControlInfo, sizeof(ControlInfo));

			CopyMemory(ControlInfo.cbHandCardData, m_cbHandCardData, sizeof(ControlInfo.cbHandCardData));

			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				IServerUserItem *pUser = m_pITableFrame->GetTableUserItem(i);
				if (pUser && pUser->IsAndroidUser())
					ControlInfo.bIsAndroid[i] = true;
			}

			BYTE cbCardListData[52] =
			{
				0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,	//���� A - K
				0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,	//÷�� A - K
				0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,	//���� A - K
				0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D	//���� A - K
			};

			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				if (m_cbPlayStatus[i] == FALSE && m_cbGiveUpUser[i] == FALSE)
					continue;

				for (BYTE j = 0; j < MAX_COUNT; j++)
				{
					for (BYTE k = 0; k < 52; k++)
					{
						if (cbCardListData[k] == m_cbHandCardData[i][j])
						{
							cbCardListData[k] = 0;
							break;
						}
					}
				}
			}

			BYTE cbCardPos = 0;
			for (BYTE i = 0; i < 52; i++)
			{
				if (cbCardListData[i] != 0)
					ControlInfo.cbCanChangeCard[cbCardPos++] = cbCardListData[i];
			}

			m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_RESPONSE_CONTROL, &ControlInfo, sizeof(ControlInfo));

			return true;
		}
	case SUB_C_CHANGE_CARD:				//�ı�����
		{
			//Ȩ���ж�
			if (CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) == false)
				return true;

			CMD_C_ChangeCard *pChangeCard = (CMD_C_ChangeCard *)pData;

			WORD wChairID = pIServerUserItem->GetChairID();

			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				if (m_cbPlayStatus[i] == PlayerNoPlay)
					continue;
				if (i == wChairID)
					continue;

				for (BYTE j = 0; j < MAX_COUNT; j++)
				{
					for (BYTE k = 0; k < MAX_COUNT; k++)
					{
						if (m_cbHandCardData[i][j] == pChangeCard->cbCardData[k])
						{
							return true;
						}
					}
				}
			}

			CopyMemory(m_cbHandCardData[wChairID], pChangeCard->cbCardData, sizeof(m_cbHandCardData[wChairID]));

			CMD_S_ControlChangeCard ControlChangeCard;
			ZeroMemory(&ControlChangeCard, sizeof(ControlChangeCard));

			m_pITableFrame->SendTableData(wChairID, SUB_S_CHANGE_SUCCESS, &ControlChangeCard, sizeof(ControlChangeCard));

			return true;
		}
	}

	return false;
}

//�����Ϣ����
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}



//�û�����
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	
	
	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{


	return true;
}

//�û�ͬ��
bool CTableFrameSink::OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{
	return true;
}

//��Ϸ��ʼ
bool CTableFrameSink::OnGameAutoStart()
{
	//ɾ����ʱ��
	m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);
	m_pITableFrame->KillGameTimer(IDI_AUTO_ADD);
	m_pITableFrame->KillGameTimer(IDI_WAIT_START);

	//����״̬
	m_pITableFrame->SetGameStatus(GAME_STATUS_PLAY);

	//�����ע
	BYTE cbAiCount = 0;
	BYTE cbPlayCount = 0;

	for (WORD i = 0; i<m_wPlayerCount; i++)
	{
		//��ȡ�û�
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem == NULL) continue;

		const SCORE lUserScore = pIServerUserItem->GetUserScore();

		//���ñ���
		m_cbPlayStatus[i] = PlayerPlaying;

		m_lUserMaxScore[i] = lUserScore;

		cbPlayCount++;
		if (pIServerUserItem->IsAndroidUser() == true)
		{
			cbAiCount++;
			m_cbAndroidStatus[i] = TRUE;
		}
		else
		{
			m_cbRealPlayer[i] = TRUE;
		}
	}

	if (cbAiCount == 0 || cbAiCount == cbPlayCount)
		m_bWriteRoomStorage = false;

	m_lCurrentTimes = 1;
	m_lMaxCellScore = m_lCellScore * 10;

	//���˥��
	if (m_bWriteRoomStorage)
		m_pITableFrame->CalculateStorageDeduct();

	//�ַ��˿�
	m_GameLogic.RandCardList(m_cbHandCardData[0], sizeof(m_cbHandCardData) / sizeof(m_cbHandCardData[0][0]));

	//��Ϸ��ʼ
	tag_ZhaJinHua_Result ZhaJinHua_Result;
	ZeroMemory(&ZhaJinHua_Result, sizeof(ZhaJinHua_Result));
	m_pITableFrame->GetControlResult(NULL, 0, &ZhaJinHua_Result);


	if (ZhaJinHua_Result.cbTableCardArray[0][0] != 0)
	{
		CopyMemory(m_cbHandCardData, ZhaJinHua_Result.cbTableCardArray, sizeof(m_cbHandCardData));
	}

	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		m_GameLogic.SortCardList(m_cbHandCardData[i], MAX_COUNT);
	}



	//��������Ϣ
	for (WORD i = 0; i < m_wPlayerCount; i++)
	{
		//����������
		IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if (pServerUserItem == NULL) continue;
		if (pServerUserItem->IsAndroidUser() == true)
		{
			//����������
			CMD_S_AndroidCard AndroidCard;
			ZeroMemory(&AndroidCard, sizeof(AndroidCard));

			//���ñ���
			CopyMemory(AndroidCard.cbRealPlayer, m_cbRealPlayer, sizeof(m_cbRealPlayer));
			CopyMemory(AndroidCard.cbAndroidStatus, m_cbAndroidStatus, sizeof(m_cbAndroidStatus));
			CopyMemory(AndroidCard.cbAllHandCardData, m_cbHandCardData, sizeof(m_cbHandCardData));
			CopyMemory(AndroidCard.cbPlayStatus, m_cbPlayStatus, sizeof(AndroidCard.cbPlayStatus));
			m_pITableFrame->SendTableData(i, SUB_S_ANDROID_CARD, &AndroidCard, sizeof(AndroidCard));
		}
	}


	//����ׯ��
	if (m_wBankerUser == INVALID_CHAIR) m_wBankerUser = rand() % m_wPlayerCount;

	//ׯ���뿪
	if (m_wBankerUser<m_wPlayerCount && m_cbPlayStatus[m_wBankerUser] == PlayerNoPlay)
		m_wBankerUser = rand() % m_wPlayerCount;

	//ȷ��ׯ��
	while (m_cbPlayStatus[m_wBankerUser] == PlayerNoPlay)
	{
		m_wBankerUser = (m_wBankerUser + 1) % m_wPlayerCount;
	}

	//�û�����
	for (WORD i = 0; i<m_wPlayerCount; i++)
	{
		if (m_cbPlayStatus[i] == PlayerPlaying)
		{
			m_lTableScore[i] = m_lCellScore;
		}
	}

	//��ǰ�û�
	m_wCurrentUser = (m_wBankerUser + 1) % m_wPlayerCount;
	while (m_cbPlayStatus[m_wCurrentUser] == PlayerNoPlay)
	{
		m_wCurrentUser = (m_wCurrentUser + 1) % m_wPlayerCount;
	}

	//��������
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart, sizeof(GameStart));
	GameStart.wCurrentUser = m_wCurrentUser;
	GameStart.wBankerUser = m_wBankerUser;
	GameStart.lMinScore = m_lCurrentTimes;

	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem * pServerUser = m_pITableFrame->GetTableUserItem(i);
		if (pServerUser)
		{
			GameStart.lPlayerRestScore[i] = pServerUser->GetUserScore();
		}
	}

	//��������
	for (WORD i = 0; i < m_wPlayerCount; i++)
	{
		if (m_cbPlayStatus[i] == PlayerPlaying)
		{
			GameStart.lUserMaxScore = m_lUserMaxScore[i];
			GameStart.lMaxScore = __min(10 * m_lCellScore, GameStart.lUserMaxScore);
			m_pITableFrame->SendTableData(i, SUB_S_GAME_START, &GameStart, sizeof(GameStart));
		}
		m_pITableFrame->SendLookonData(i, SUB_S_GAME_START, &GameStart, sizeof(GameStart));
	}

	//���ö�ʱ�� �Ƿ�������
	m_pITableFrame->SetGameTimer(IDI_ADD_SCORE, TIME_ADD_SCORE, 1, 0);
	m_dwJettonTime = (DWORD)time(NULL);
	return true;
}

//�����¼�
bool CTableFrameSink::OnUserGiveUp(WORD wChairID,bool bExit)
{
	//��������
	m_cbGiveUpUser[wChairID] = TRUE;
	m_cbPlayStatus[wChairID] = PlayerGiveUp;

	bool bExchangeCurrentUser = false;
	if (wChairID == m_wCurrentUser)
	{
		bExchangeCurrentUser = true;
		//��ȡ���һ����ҵ�λ��
		WORD wFinalUser = m_wBankerUser;
		for (BYTE i = 0; i < m_wPlayerCount; i++)
		{
			if (m_cbPlayStatus[wFinalUser] == PlayerNoPlay || m_cbPlayStatus[wFinalUser] == PlayerGiveUp || m_cbPlayStatus[wFinalUser] == PlayerComFail)
			{
				wFinalUser = (wFinalUser - 1 + m_wPlayerCount) % m_wPlayerCount;
			}
			else
				break;
		}

		//���һ����ҷ���������++
		if (wChairID == wFinalUser)
		{
			m_cbGameTurns++;
		}

		//�û��л�
		WORD wNextPlayer = INVALID_CHAIR;
		for (WORD i = 1; i < m_wPlayerCount; i++)
		{
			//���ñ���
			wNextPlayer = (m_wCurrentUser + i) % m_wPlayerCount;

			//�����ж�
			if (m_cbPlayStatus[wNextPlayer] == PlayerPlaying || m_cbPlayStatus[wNextPlayer] == PlayerLookCard)
				break;
		}
		m_wCurrentUser = wNextPlayer;
	}

	if (m_cbGameTurns >= MAX_ROUND)
	{
		m_wCurrentUser = INVALID_CHAIR;
	}

	//������Ϣ
	CMD_S_GiveUp GiveUp;
	GiveUp.wGiveUpUser=wChairID;
	GiveUp.wCurrentUser = bExchangeCurrentUser ? m_wCurrentUser : INVALID_CHAIR;
	GiveUp.cbGameTurns = m_cbGameTurns;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));

	//�޸Ļ���
	tagScoreInfo ScoreInfo;
	ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
	ScoreInfo.cbType =(bExit)?SCORE_TYPE_FLEE:SCORE_TYPE_LOSE;
	ScoreInfo.lRevenue=0;  
	ScoreInfo.lScore=-m_lTableScore[wChairID];

	m_pITableFrame->WriteUserScore(wChairID, ScoreInfo);

	//���ֵ
	IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(wChairID);
	if (pServerUser && pServerUser->IsAndroidUser() && m_bWriteRoomStorage)
		m_pGameServiceOption->pRoomStorageOption.lStorageCurrent += ScoreInfo.lScore;

	//����ͳ��
	WORD wPlayerCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i] == PlayerPlaying || m_cbPlayStatus[i] == PlayerLookCard) wPlayerCount++;
	}

	//�жϽ���
	if (wPlayerCount<=1)
		OnEventGameConclude(INVALID_CHAIR,NULL,GER_NO_PLAYER);

	//�����ж�
	if (m_cbGameTurns >= MAX_ROUND)
	{
		OnEventGameConclude(GAME_PLAYER, NULL, GER_OPENCARD);
		return true;
	}

	if (m_wCurrentUser != INVALID_CHAIR && bExchangeCurrentUser && m_bFollowStatus[m_wCurrentUser])
		m_pITableFrame->SetGameTimer(IDI_AUTO_ADD, TIME_AUTO_ADD, 1, 0);

	if (bExchangeCurrentUser)
	{
		//ɾ����ʱ��
		m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);

		//���ö�ʱ��
		m_pITableFrame->SetGameTimer(IDI_ADD_SCORE, TIME_ADD_SCORE, 1, 0);

		m_dwJettonTime = (DWORD)time(NULL);
	}

	return true;
}

//�����¼�
bool CTableFrameSink::OnUserLookCard(WORD wChairID)
{
	//����Ч��
	ASSERT(!m_bMingZhu[wChairID]);
	if (m_bMingZhu[wChairID]) return true;

	//���ò���
	m_bMingZhu[wChairID]=true;
	m_cbPlayStatus[wChairID] = PlayerLookCard;

	//��������
	CMD_S_LookCard LookCard;
	ZeroMemory(&LookCard,sizeof(LookCard));
	LookCard.wLookCardUser=wChairID;

	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_LOOK_CARD,&LookCard,sizeof(LookCard));

	for(WORD i = 0;i<m_wPlayerCount;i++)
	{
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if(pIServerUserItem == NULL ) continue;
		if(i == wChairID)
		{
			CopyMemory(LookCard.cbCardData,m_cbHandCardData[wChairID],sizeof(m_cbHandCardData[0]));
			LookCard.cbCardType = m_GameLogic.GetCardType(LookCard.cbCardData,MAX_COUNT);
		}
		else
		{
			ZeroMemory(LookCard.cbCardData,sizeof(LookCard.cbCardData));
		}
		m_pITableFrame->SendTableData(i,SUB_S_LOOK_CARD,&LookCard,sizeof(LookCard));
	}

	return true;
}

//�����¼�
bool CTableFrameSink::OnUserCompareCard(WORD wFirstChairID, WORD wNextChairID, bool bNeedScore)
{
	//if (m_cbShowHandStatus)
	//{
	//	CMD_S_FailResult FailResult;
	//	_sntprintf(FailResult.szInfo, sizeof(FailResult.szInfo), TEXT("��ǰ����ȫѹ״̬�����ܱ��ƣ�"));
	//	m_pITableFrame->SendTableData(wFirstChairID, SUB_S_PLAYER_COMMPARE_FALURE, &FailResult, sizeof(FailResult));
	//	return true;
	//}

	//��ǰ����
	LONGLONG lTimes = m_bMingZhu[wFirstChairID] ? 4 : 2;
	
	SCORE lCompareScore = lTimes * m_lCellScore * m_lCurrentTimes;

	//ȫѹҲ����������ÿ�Ǯ
	if (!bNeedScore)
		lCompareScore = 0;

	if ((lCompareScore + m_lTableScore[wFirstChairID]) > m_lUserMaxScore[wFirstChairID])
	{
		CMD_S_FailResult FailResult;
		_sntprintf(FailResult.szInfo, sizeof(FailResult.szInfo), TEXT("������Я�����������ܱ��ƣ�"));
		m_pITableFrame->SendTableData(wFirstChairID, SUB_S_PLAYER_COMMPARE_FALURE, &FailResult, sizeof(FailResult));
		return true;
	}

	//�û�ע��
	m_lTableScore[wFirstChairID] += lCompareScore;

	//�Ƚϴ�С
	BYTE cbWin=m_GameLogic.CompareCard(m_cbHandCardData[wFirstChairID],m_cbHandCardData[wNextChairID],MAX_COUNT);

	//ʤ���û�
	WORD wLostUser,wWinUser;
	if(cbWin==TRUE) 
	{
		wWinUser=wFirstChairID;
		wLostUser=wNextChairID;
	}
	else
	{
		wWinUser=wNextChairID;
		wLostUser=wFirstChairID;
	}

	//��������
	m_cbPlayStatus[wLostUser] = PlayerComFail;
	m_wCompardUser[wLostUser][m_cbCompareCount[wLostUser]++] = wWinUser;
	m_wCompardUser[wWinUser][m_cbCompareCount[wWinUser]++] = wLostUser;

	//����ͳ��
	WORD wPlayerCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i] == PlayerPlaying || m_cbPlayStatus[i] == PlayerLookCard) wPlayerCount++;
	}

	//������Ϸ
	if(wPlayerCount>=2)
	{
		//��ȡ���һ����ҵ�λ��
		WORD wFinalUser = m_wBankerUser;
		for (BYTE i = 0; i < m_wPlayerCount; i++)
		{
			if (m_cbPlayStatus[wFinalUser] == PlayerNoPlay || m_cbPlayStatus[wFinalUser] == PlayerGiveUp || m_cbPlayStatus[wFinalUser] == PlayerComFail)
			{
				wFinalUser = (wFinalUser - 1 + m_wPlayerCount) % m_wPlayerCount;
			}
			else
				break;
		}

		//���һ����Ҽ�ע������++
		if (wFirstChairID == wFinalUser)
		{
			m_cbGameTurns++;
		}

		//�û��л�
		WORD wNextPlayer=INVALID_CHAIR;
		for (WORD i=1;i<m_wPlayerCount;i++)
		{
			//���ñ���
			wNextPlayer=(m_wCurrentUser+i)%m_wPlayerCount;

			//�����ж�
			if (m_cbPlayStatus[wNextPlayer] == PlayerPlaying || m_cbPlayStatus[wNextPlayer] == PlayerLookCard)
				break;
		}
		//�����û�
		m_wCurrentUser=wNextPlayer;
	}
	else m_wCurrentUser=INVALID_CHAIR;

	if (m_cbGameTurns >= MAX_ROUND)
	{
		m_wCurrentUser = INVALID_CHAIR;
	}

	//��������
	CMD_S_CompareCard CompareCard;
	ZeroMemory(&CompareCard, sizeof(CompareCard));
	CompareCard.wCurrentUser = m_wCurrentUser;
	CompareCard.wCompareUser[0] = wFirstChairID;
	CompareCard.wCompareUser[1] = wNextChairID;
	CompareCard.lCompareScore = lCompareScore;
	CompareCard.lPlayerScore = m_lUserMaxScore[wFirstChairID] - m_lTableScore[wFirstChairID];
	CompareCard.lPlayerTableScore = m_lTableScore[wFirstChairID];

	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		CompareCard.lAllTableScore += m_lTableScore[i];
	}

	CompareCard.cbGameTurns = m_cbGameTurns;
	CompareCard.wWinUser = wWinUser;

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_COMPARE_CARD, &CompareCard, sizeof(CompareCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_COMPARE_CARD, &CompareCard, sizeof(CompareCard));
	

	//�޸Ļ���
	tagScoreInfo ScoreInfo;
	ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
	ScoreInfo.cbType =SCORE_TYPE_LOSE;
	ScoreInfo.lRevenue=0;
	ScoreInfo.lScore=-m_lTableScore[wLostUser];

	m_pITableFrame->WriteUserScore(wLostUser, ScoreInfo);

	//���ֵ
	IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(wLostUser);
	if (pServerUser && pServerUser->IsAndroidUser() && m_bWriteRoomStorage)
		m_pGameServiceOption->pRoomStorageOption.lStorageCurrent += ScoreInfo.lScore;

	//������Ϸ
	if (wPlayerCount<2)
	{
		m_wBankerUser=wWinUser;
		OnEventGameConclude(GAME_PLAYER,NULL,GER_COMPARECARD);
		return true;
	}
	else
	{
		if (m_cbGameTurns >= MAX_ROUND)
		{
			OnEventGameConclude(GAME_PLAYER, NULL, GER_OPENCARD);
			return true;
		}

		//ɾ����ʱ��
		m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);

		if (m_wCurrentUser != INVALID_CHAIR && m_bFollowStatus[m_wCurrentUser])
			m_pITableFrame->SetGameTimer(IDI_AUTO_ADD, TIME_AUTO_ADD, 1, 0);

		//���ö�ʱ��
		m_pITableFrame->SetGameTimer(IDI_ADD_SCORE, TIME_ADD_SCORE, 1, 0);

		m_dwJettonTime = (DWORD)time(NULL);
	}
	return true;
}

//�����¼�
bool CTableFrameSink::OnUserOpenCard(WORD wUserID)
{
	//��������
	CMD_S_OpenCard OpenCard;
	OpenCard.wOpenUser = wUserID;
	CopyMemory(OpenCard.cbCardData, m_cbHandCardData[wUserID], sizeof(OpenCard.cbCardData));

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_OPEN_CARD, &OpenCard, sizeof(OpenCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_OPEN_CARD, &OpenCard, sizeof(OpenCard));
	return true;
}


//ȫѹ�¼�
bool CTableFrameSink::OnUserShowHand(WORD wChairID)
{

	//״̬Ч��
	ASSERT(m_wCurrentUser == wChairID);
	if (m_wCurrentUser != wChairID) return true;

	BYTE cbRestCount = 0;

	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		if (m_cbPlayStatus[i] == PlayerPlaying || m_cbPlayStatus[i] == PlayerLookCard)
		{
			cbRestCount++;
		}
	}

	//�����˷�������
	IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(wChairID);
	if (pServerUser && pServerUser->IsAndroidUser())
	{
		if (m_cbGameTurns < 3 || cbRestCount != 2)
		{
			SCORE lScore = m_lCurrentTimes * m_lCellScore;
			if (m_bMingZhu[wChairID])
				lScore *= 2;
			OnUserAddScore(wChairID, lScore);
			return true;
		}
	}

	if (m_cbGameTurns < 3 || cbRestCount != 2)
		return true;

	SCORE lMaxBetScore = 10 * 20 * m_lCellScore;

	if (m_bMingZhu[wChairID])
		lMaxBetScore *= 2;

	lMaxBetScore = __min(lMaxBetScore, m_lUserMaxScore[wChairID]);

	SCORE lScore = lMaxBetScore - m_lTableScore[wChairID];

	//�û�ע��
	m_lTableScore[wChairID] += lScore;
	m_lShowHandScore[wChairID] = lScore;

	//�����û�
	//�û��л�
	WORD wNextPlayer = INVALID_CHAIR;
	for (WORD i = 1; i<m_wPlayerCount; i++)
	{
		//���ñ���
		wNextPlayer = (m_wCurrentUser + i) % m_wPlayerCount;

		//�����ж�
		if (m_cbPlayStatus[wNextPlayer] == PlayerPlaying || m_cbPlayStatus[wNextPlayer] == PlayerLookCard)
			break;
	}
	m_wCurrentUser = wNextPlayer;

	//��ȡʣ��ȫѹ���������
	BYTE cbShowHandCount = 0;
	WORD wShowHandUser[GAME_PLAYER];
	FillMemory(wShowHandUser, sizeof(wShowHandUser),INVALID_CHAIR);
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		if (m_cbPlayStatus[i] == PlayerPlaying || m_cbPlayStatus[i] == PlayerLookCard)
		{
			if (m_lShowHandScore[i] > 0)
			{
				wShowHandUser[cbShowHandCount] = i;
				cbShowHandCount++;
			}
		}
	}

	if (cbShowHandCount >= 2)
		m_wCurrentUser = INVALID_CHAIR;

	//��������
	CMD_S_AddScore AddScore;
	ZeroMemory(&AddScore, sizeof(AddScore));
	AddScore.wCurrentUser = m_wCurrentUser;
	AddScore.wAddScoreUser = wChairID;
	AddScore.lAddScoreCount = lScore;
	AddScore.cbGameTurns = m_cbGameTurns;

	m_cbShowHandStatus = true;
	AddScore.cbShowHandStatus = m_cbShowHandStatus;
	
	AddScore.lPlayerScore = m_lUserMaxScore[wChairID] - m_lTableScore[wChairID];
	AddScore.lPlayerTableScore = m_lTableScore[wChairID];

	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		AddScore.lAllTableScore += m_lTableScore[i];
	}

	//��������
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);

		if (!pServerUser) continue;

		AddScore.lMinScore = m_lUserMaxScore[i] - m_lTableScore[i];
		AddScore.lMaxScore = m_lUserMaxScore[i] - m_lTableScore[i];

		m_pITableFrame->SendTableData(i, SUB_S_SHOW_HAND, &AddScore, sizeof(AddScore));
		m_pITableFrame->SendLookonData(i, SUB_S_SHOW_HAND, &AddScore, sizeof(AddScore));
	}

	if (cbShowHandCount >= 2)
	{
		if (cbShowHandCount == 2)
		{
			OnUserCompareCard(wShowHandUser[0], wShowHandUser[1],false);
			return true;
		}
		else
		{
			OnEventGameConclude(GAME_PLAYER, NULL, GER_OPENCARD);
			return true;
		}
	}
	else
	{
		//ɾ����ʱ��
		m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);

		//���ö�ʱ��
		m_pITableFrame->SetGameTimer(IDI_ADD_SCORE, TIME_ADD_SCORE, 1, 0);

		m_dwJettonTime = (DWORD)time(NULL);
	}

	return true;
}


//��ע�¼�
bool CTableFrameSink::OnUserAddScore(WORD wChairID, SCORE lScore)
{
	if (m_cbShowHandStatus)
	{
		CMD_S_FailResult FailResult;
		_sntprintf(FailResult.szInfo, sizeof(FailResult.szInfo), TEXT("��ǰ����ȫѹ״̬��������ע��"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLAYER_BET_FALURE, &FailResult, sizeof(FailResult));
		return true;
	}
	//״̬Ч��
	ASSERT(m_wCurrentUser==wChairID);
	if (m_wCurrentUser != wChairID)
	{
		CMD_S_FailResult FailResult;
		_sntprintf(FailResult.szInfo, sizeof(FailResult.szInfo), TEXT("��û���ֵ���˵����������ע��"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLAYER_BET_FALURE, &FailResult, sizeof(FailResult));
		return true;
	}

	//���Ч��
	ASSERT((lScore+m_lTableScore[wChairID])<=m_lUserMaxScore[wChairID]);
	DOUBLE dMor = lScore / m_lCellScore;
	int nMor = lScore / m_lCellScore;
	if (lScore < 0 || (dMor - nMor) > 0)
	{
		CMD_S_FailResult FailResult;
		_sntprintf(FailResult.szInfo, sizeof(FailResult.szInfo), TEXT("��ע�����󣬲�����ע��"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLAYER_BET_FALURE, &FailResult, sizeof(FailResult));
		return true;
	}
	if ((lScore + m_lTableScore[wChairID]) > m_lUserMaxScore[wChairID])
	{
		CMD_S_FailResult FailResult;
		_sntprintf(FailResult.szInfo, sizeof(FailResult.szInfo), TEXT("��ע������Я��������������ע��"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLAYER_BET_FALURE, &FailResult, sizeof(FailResult));
		return true;
	}

	//��ǰ����
	LONGLONG lTimes=m_bMingZhu[wChairID]?2:1;
	LONGLONG lTemp=lScore/m_lCellScore/lTimes;

	if (!(m_lCurrentTimes <= lTemp && lTemp <= m_lMaxCellScore / m_lCellScore))
	{
		IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(wChairID);

		//��������˷�����һ����ע�����������Ϣ�������ĳɸ�ע
		if (pServerUser && pServerUser->IsAndroidUser())
		{
			lScore = m_lCellScore * m_lCurrentTimes;
			lTemp = m_lCurrentTimes;
			if (m_bMingZhu[wChairID])
				lScore *= 2;
		}
		else
		{
			CMD_S_FailResult FailResult;
			_sntprintf(FailResult.szInfo, sizeof(FailResult.szInfo), TEXT("��ע�����󣬲�����ע��"));
			m_pITableFrame->SendTableData(wChairID, SUB_S_PLAYER_BET_FALURE, &FailResult, sizeof(FailResult));
			return true;
		}
	}

	//�ж�����Ƿ��ע��Ǯ������ʣ�µ���ұ����ˣ�����ǵĻ� �����Զ�����
	//{
	//	BYTE cbUserCount = 0;
	//	for (WORD i = 0; i < GAME_PLAYER; i++)
	//	{
	//		if (m_cbPlayStatus[i] == PlayerPlaying || m_cbPlayStatus[i] == PlayerLookCard)
	//			cbUserCount++;
	//	}
	//	if (cbUserCount > 2)
	//	{
	//		LONGLONG lUserRestScore = m_lUserMaxScore[wChairID] - m_lTableScore[wChairID];
	//		LONGLONG lRestCompareScore = cbUserCount * 40 * m_lCellScore;
	//		if ((lUserRestScore - lScore) < lRestCompareScore)
	//		{
	//			WORD wCompareUser = INVALID_CHAIR;
	//			for (WORD i = 1; i < GAME_PLAYER; i++)
	//			{
	//				WORD wTempUser = (wChairID + i) % GAME_PLAYER;
	//				if (wTempUser == wChairID)
	//					continue;
	//				if (m_cbPlayStatus[wTempUser] == PlayerPlaying || m_cbPlayStatus[wTempUser] == PlayerLookCard)
	//				{
	//					wCompareUser = wTempUser;
	//				}
	//			}

	//			if (wCompareUser != INVALID_CHAIR)
	//			{
	//				OnUserCompareCard(wChairID,wCompareUser);
	//				return true;
	//			}
	//		}
	//	}
	//}

	if (lTemp <= 10)
		m_lCurrentTimes = lTemp;

	//�û�ע��
	m_lTableScore[wChairID]+=lScore;
	

	//��ȡ���һ����ҵ�λ��
	WORD wFinalUser = m_wBankerUser;
	for (BYTE i = 0; i < m_wPlayerCount; i++)
	{
		if (m_cbPlayStatus[wFinalUser] == PlayerNoPlay || m_cbPlayStatus[wFinalUser] == PlayerGiveUp || m_cbPlayStatus[wFinalUser] == PlayerComFail)
		{
			wFinalUser = (wFinalUser - 1 + m_wPlayerCount) % m_wPlayerCount;
		}
		else
			break;
	}

	//���һ����Ҽ�ע������++
	if (wChairID == wFinalUser)
	{
		m_cbGameTurns++;
	}
	
	//�û��л�
	WORD wNextPlayer=INVALID_CHAIR;
	for (WORD i=1;i<m_wPlayerCount;i++)
	{
		//���ñ���
		wNextPlayer=(m_wCurrentUser+i)%m_wPlayerCount;

		//�����ж�
		if (m_cbPlayStatus[wNextPlayer] == PlayerPlaying || m_cbPlayStatus[wNextPlayer] == PlayerLookCard)
			break;
	}
	m_wCurrentUser=wNextPlayer;
	
	if (m_cbGameTurns >= MAX_ROUND)
	{
		m_wCurrentUser = INVALID_CHAIR;
	}

	//��������
	CMD_S_AddScore AddScore;
	ZeroMemory(&AddScore, sizeof(AddScore));
	AddScore.wCurrentUser=m_wCurrentUser;
	AddScore.wAddScoreUser=wChairID;
	AddScore.lAddScoreCount=lScore;
	AddScore.cbGameTurns = m_cbGameTurns;

	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		AddScore.lAllTableScore += m_lTableScore[i];
	}

	AddScore.lMinScore = m_lCurrentTimes;
	AddScore.lPlayerTableScore = m_lTableScore[wChairID];
	AddScore.lPlayerScore = m_lUserMaxScore[wChairID] - m_lTableScore[wChairID];

	//��������
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);

		if (!pServerUser) continue;

		AddScore.lMaxScore = __min(10 * m_lCellScore, AddScore.lPlayerScore);

		m_pITableFrame->SendTableData(i, SUB_S_ADD_SCORE, &AddScore, sizeof(AddScore));
		m_pITableFrame->SendLookonData(i, SUB_S_ADD_SCORE, &AddScore, sizeof(AddScore));
	}

	if (m_cbGameTurns >= MAX_ROUND)
	{
		OnEventGameConclude(GAME_PLAYER, NULL, GER_OPENCARD);
		return true;
	}

	//ɾ����ʱ��
	m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);

	if (m_wCurrentUser != INVALID_CHAIR && m_bFollowStatus[m_wCurrentUser])
		m_pITableFrame->SetGameTimer(IDI_AUTO_ADD, TIME_AUTO_ADD, 1, 0);

	//���ö�ʱ��
	m_pITableFrame->SetGameTimer(IDI_ADD_SCORE,TIME_ADD_SCORE,1,0);

	m_dwJettonTime = (DWORD)time(NULL);
	
	
	return true;
}

//��ȡ����
void CTableFrameSink::ReadConfigInformation()
{	
	//��ȡ�Զ�������
	tagCustomRule *pCustomRule = (tagCustomRule *)m_pGameServiceOption->cbCustomRule;
	ASSERT(pCustomRule);

	m_pCustomAndroid.lRobotScoreMin = pCustomRule->lRobotScoreMin;
	m_pCustomAndroid.lRobotScoreMax = pCustomRule->lRobotScoreMax;
	m_pCustomAndroid.lRobotBankGet = pCustomRule->lRobotBankGet;
	m_pCustomAndroid.lRobotBankGetBanker = pCustomRule->lRobotBankGetBanker;
	m_pCustomAndroid.lRobotBankStoMul = pCustomRule->lRobotBankStoMul;
}
//////////////////////////////////////////////////////////////////////////
