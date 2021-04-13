#include "Stdafx.h"
#include "AndroidUserItemSink.h"
#include "math.h"
#include <locale>

//////////////////////////////////////////////////////////////////////////

//����ʱ��
#define TIME_LESS						2									//����ʱ��

//��Ϸʱ��
#define TIME_USER_CALL_BANKER			3									//��ׯʱ��
#define TIME_USER_START_GAME			3									//��ʼʱ��
#define TIME_USER_ADD_SCORE				3									//��עʱ��
#define TIME_USER_OPEN_CARD				3									//̯��ʱ��

//��Ϸʱ��
#define IDI_START_GAME					(100)									//��ʼ��ʱ��
#define IDI_CALL_BANKER					(101)									//��ׯ��ʱ��
#define IDI_USER_ADD_SCORE				(102)									//��ע��ʱ��
#define IDI_OPEN_CARD					(103)									//���ƶ�ʱ��
#define IDI_DELAY_TIME					(105)									//��ʱ��ʱ��

//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	m_nRobotBankStorageMul=0;
	m_lRobotBankGetScore=0;
	m_lRobotBankGetScoreBanker=0;
	ZeroMemory(m_lRobotScoreRange,sizeof(m_lRobotScoreRange));
	
	//�ӿڱ���
	m_pIAndroidUserItem=NULL;
	srand((unsigned)time(NULL));   
	
	m_bForbidOperate = true;
	return;
}

//��������
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//�ӿڲ�ѯ
void * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ�ӿ�
bool CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}

//���ýӿ�
bool CAndroidUserItemSink::RepositionSink()
{
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	m_bForbidOperate = true;
	return true;
}

//ʱ����Ϣ
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	try
	{
		switch (nTimerID)
		{
		case IDI_DELAY_TIME:
			{
				//��ʼʱ��
				UINT nElapse= rand() % TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

				return true;
			}

		case IDI_START_GAME:		//��ʼ��ʱ��
			{			
				//����׼��
				m_pIAndroidUserItem->SendUserReady(NULL,0);

				return true;
			}
		case IDI_CALL_BANKER:		//��ׯ��ʱ
			{
				//ɾ����ʱ��
				m_pIAndroidUserItem->KillGameTimer(IDI_CALL_BANKER);
				//���ñ���
				CMD_C_CallBanker CallBanker;
				ZeroMemory(&CallBanker, sizeof(CallBanker));

				bool bBanker = rand () % 100 > 50;
				if (bBanker == false)
				{
					CallBanker.cbBankerTimes = 0;
				}
				else
				{
					BYTE cbTimes[3] = {1,2,4};
					CallBanker.cbBankerTimes = cbTimes[rand() % 3];
				}

				//������Ϣ
				m_pIAndroidUserItem->SendSocketData(SUB_C_CALL_BANKER,&CallBanker, sizeof(CallBanker));

				return true;
			}
		case IDI_USER_ADD_SCORE:	//��ע��ʱ
			{
				//ɾ����ʱ��
				m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);
				CMD_C_AddScore AddScore;
				ZeroMemory(&AddScore, sizeof(AddScore));

				int lAddScore = 0LL;
				
				lAddScore = m_lAddChip[rand() % MAX_CONFIG];

				if (lAddScore <= 0 || lAddScore > 20)
				{
					lAddScore = 5;
				}
				
				AddScore.nScore = lAddScore;
		
				//������Ϣ
				m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));

				return true;	
			}
		}
	}
	catch (...)
	{

	}	
	
	return false;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:	//��Ϸ��ʼ
		{
			//��Ϣ����
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_PLAYER_BET:	//�û���ע
		{
			//��Ϣ����
			return OnSubAddScore(pData,wDataSize);
		}
	case SUB_S_DISPATCH_CARD:	//������Ϣ
		{
			//��Ϣ����
			return OnSubSendCard(pData,wDataSize);
		}
	case SUB_S_GAME_END:	//��Ϸ����
		{
			//��Ϣ����
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_CALL_BANKER:	//�û���ׯ
		{
			//��Ϣ����
			return OnSubCallBanker(pData,wDataSize);
		}
	case SUB_S_SET_BANKER:	//�û���ׯ��Ϣ
		{
			//��Ϣ����
			return OnSubCallBankerInfo(pData,wDataSize);
		}
	case SUB_S_CUSTOM_ANDROID:
		{
			//��Ϣ����
			if (wDataSize != sizeof(tagCustomAndroid)) return false;
			tagCustomAndroid * pCustomAndroid = (tagCustomAndroid *)pData;
			//��ȡ����
			ReadConfigInformation(pCustomAndroid);
			return true;
		}
	}

	return true;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_TK_FREE:		//����״̬
	case GS_TK_WAIT:
	case GS_TK_END:
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��Ϣ����
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			BankOperate(2);

			return true;
		}
	case GS_TK_CALL:	// ��ׯ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusCall)) return false;
			CMD_S_StatusCall * pStatusCall=(CMD_S_StatusCall *)pData;

			BankOperate(2);
			
			if (pStatusCall->cbPlayerStatus[m_pIAndroidUserItem->GetChairID()] == FALSE)
				m_bForbidOperate = true;

			if (pStatusCall->cbCallBankerTimes[m_pIAndroidUserItem->GetChairID()] == 0xFF && !m_bForbidOperate)
			{
				//��ׯʱ��
				UINT nElapse = TIME_LESS + (rand() % TIME_USER_CALL_BANKER);
				m_pIAndroidUserItem->SetGameTimer(IDI_CALL_BANKER, nElapse);
			}

			return true;
		}
	case GS_TK_SCORE:	//��ע״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusScore)) return false;
			CMD_S_StatusScore * pStatusScore=(CMD_S_StatusScore *)pData;

			BankOperate(2);
			
			//���ñ���
			WORD wMeChairId = m_pIAndroidUserItem->GetChairID();

			if (pStatusScore->cbPlayerStatus[wMeChairId] == FALSE)
				m_bForbidOperate = true;

			//���ó���
			if (pStatusScore->lBetTimes[wMeChairId] == 0L && wMeChairId != pStatusScore->wBankerUser && !m_bForbidOperate)
			{
				//��עʱ��
				UINT nElapse = TIME_LESS + (rand() % TIME_USER_ADD_SCORE + 1);
				m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE, nElapse);
			}

			return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//�û�����
void CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û��뿪
void CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�����
void CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�״̬
void CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û���λ
void CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	

	return true;
}

//�û���ע
bool CAndroidUserItemSink::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
	//�����˲������û���ע
	return true;
}

//�û�ǿ��
bool CAndroidUserItemSink::OnSubPlayerExit(const void * pBuffer, WORD wDataSize)
{
	//�����˲������û�ǿ��
	return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//ɾ����ʱ��
	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_BANKER);
	m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);

	//��������
	WORD wMeChiarID=m_pIAndroidUserItem->GetChairID();
	CopyMemory(m_cbHandCardData, pSendCard->cbCardData[wMeChiarID], sizeof(m_cbHandCardData));

	if (pSendCard->cbPlayerStatus[wMeChiarID] == TRUE)
		m_bForbidOperate = false;

	if (!m_bForbidOperate)
	{
		//����ʱ��
		UINT nElapse = TIME_LESS + 2 + (rand() % TIME_USER_CALL_BANKER);
		m_pIAndroidUserItem->SetGameTimer(IDI_CALL_BANKER, nElapse);
	}

	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//ɾ����ʱ��
	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_BANKER);
	m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);

	//�������
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	return true;
}

//�û���ׯ
bool CAndroidUserItemSink::OnSubCallBanker(const void * pBuffer, WORD wDataSize)
{
	return true;
}

//�û���ׯ��Ϣ
bool CAndroidUserItemSink::OnSubCallBankerInfo(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize != sizeof(CMD_S_SetBanker)) return false;
	CMD_S_SetBanker * pSetBanker = (CMD_S_SetBanker *)pBuffer;

	//ɾ����ʱ��
	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_BANKER);
	m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);

	//��������
	WORD wMeChiarID = m_pIAndroidUserItem->GetChairID();
	
	if (wMeChiarID != pSetBanker->wBankerUser && !m_bForbidOperate)
	{
		//����ʱ��
		UINT nElapse = TIME_LESS + 2 + (rand() % TIME_USER_ADD_SCORE);
		m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE, nElapse);
	}

	return true;
}

//���в���
void CAndroidUserItemSink::BankOperate(BYTE cbType)
{
	//��������
	SCORE lRobotScore = m_pIAndroidUserItem->GetMeUserItem()->GetUserScore();

	//�жϴ�ȡ
	if (lRobotScore > m_lRobotScoreRange[1])
	{			
		SCORE lSaveScore = 0;

		lSaveScore = SCORE(lRobotScore*m_nRobotBankStorageMul / 100);
		if (lSaveScore > lRobotScore)  lSaveScore = lRobotScore;

		if (lSaveScore > 0)
			m_pIAndroidUserItem->PerformSaveScore(lSaveScore);

		SCORE lRobotNewScore = m_pIAndroidUserItem->GetMeUserItem()->GetUserScore();
	}
	else if (lRobotScore < m_lRobotScoreRange[0])
	{
		CString strInfo;
		
		SCORE lScore=rand()%m_lRobotBankGetScoreBanker+m_lRobotBankGetScore;
		if (lScore > 0)
			m_pIAndroidUserItem->PerformTakeScore(lScore);

		SCORE lRobotNewScore = m_pIAndroidUserItem->GetMeUserItem()->GetUserScore();
		
	}
}

//��ȡ����
void CAndroidUserItemSink::ReadConfigInformation(tagCustomAndroid *pCustomAndroid)
{
	m_lRobotScoreRange[0] = pCustomAndroid->lRobotScoreMin;
	m_lRobotScoreRange[1] = pCustomAndroid->lRobotScoreMax;

	if (m_lRobotScoreRange[1] < m_lRobotScoreRange[0])	
		m_lRobotScoreRange[1] = m_lRobotScoreRange[0];

	m_lRobotBankGetScore = pCustomAndroid->lRobotBankGet;
	m_lRobotBankGetScoreBanker = pCustomAndroid->lRobotBankGetBanker;
	m_nRobotBankStorageMul = pCustomAndroid->lRobotBankStoMul;
	
	if (m_nRobotBankStorageMul<0||m_nRobotBankStorageMul>100) 
		m_nRobotBankStorageMul =20;

	CopyMemory(m_lAddChip, pCustomAndroid->lAddChip, sizeof(m_lAddChip));
}


//�����������
DECLARE_CREATE_MODULE(AndroidUserItemSink);

//////////////////////////////////////////////////////////////////////////
