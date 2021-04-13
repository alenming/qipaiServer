#include "Stdafx.h"
#include "AndroidUserItemSink.h"
#include "math.h"
//////////////////////////////////////////////////////////////////////////

//����ʱ��
#define TIME_LESS           2                 //����ʱ��

//��Ϸʱ��
#define TIME_USER_CALL_BANKER     3                 //��ׯʱ��
#define TIME_USER_START_GAME      3                 //��ʼʱ��
#define TIME_USER_ADD_SCORE       3                 //��עʱ��
#define TIME_USER_OPEN_CARD       3                 //̯��ʱ��

#define TIME_CHECK_BANKER       30                  //̯��ʱ��

//��Ϸʱ��
#define IDI_START_GAME          (100)					//��ʼ��ʱ��
#define IDI_CALL_BANKER         (101)					//��ׯ��ʱ��
#define IDI_USER_ADD_SCORE      (102)					//��ע��ʱ��
//////////////////////////////////////////////////////////////////////////



//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
  ZeroMemory(m_HandCardData,sizeof(m_HandCardData));

  m_nRobotBankStorageMul=0;
  m_lRobotBankGetScore=0;
  m_lRobotBankGetScoreBanker=0;
  ZeroMemory(m_lRobotScoreRange,sizeof(m_lRobotScoreRange));

  //�ӿڱ���
  m_pIAndroidUserItem=NULL;
  srand((unsigned)time(NULL));

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
  if(m_pIAndroidUserItem==NULL)
  {
    return false;
  }
  return true;
}

//���ýӿ�
bool CAndroidUserItemSink::RepositionSink()
{
  ZeroMemory(m_HandCardData,sizeof(m_HandCardData));


  return true;
}

//ʱ����Ϣ
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
  
	switch(nTimerID)
	{
		case IDI_START_GAME:    //��ʼ��ʱ��
		{
			//����׼��
			m_pIAndroidUserItem->SendUserReady(NULL,0);

			return true;
		}
		case IDI_CALL_BANKER:   //��ׯ��ʱ
		{
			//���ñ���
			CMD_C_CallBanker CallBanker;

			BYTE cbValue = m_GameLogic.GetCardType(m_HandCardData, MAX_COUNT);

			if (cbValue >= 7)
				CallBanker.bBanker = ((rand() % 100) < 80) ? TRUE : FALSE;
			else
				CallBanker.bBanker = ((rand() % 100) < 20) ? TRUE : FALSE;

			//������Ϣ
			m_pIAndroidUserItem->SendSocketData(SUB_C_CALL_BANKER,&CallBanker,sizeof(CallBanker));

			return true;
		}
		case IDI_USER_ADD_SCORE:  //��ע��ʱ
		{
			//��ע���� 60%����ѡ���д�ע��
			BYTE cbAddScoreIndex = 0;
			BYTE cbRand1 = rand() % 100;
			BYTE cbRand2 = cbRand1 % 2 + 1;
			
			BYTE cbValue = m_GameLogic.GetCardType(m_HandCardData, MAX_COUNT);

			cbAddScoreIndex = (cbValue >= 7) ? (2 + cbRand2) : cbRand2;

			//������Ϣ
			CMD_C_AddScore AddScore;
			AddScore.lScore = cbAddScoreIndex;
			m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

			return true;
		}
	}

  

  return true;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
  switch(wSubCmdID)
  {
    case SUB_S_CALL_BANKER: //�û���ׯ
    {
		//��Ϣ����
		return OnSubCallBanker(pData,wDataSize);
    }
    case SUB_S_GAME_START:  //��Ϸ��ʼ
    {
		//��Ϣ����
		return OnSubGameStart(pData,wDataSize);
    }
	case SUB_S_BANKER_END:	//��ׯ���
	{
		//��Ϣ����
		return OnSubBankerEnd(pData, wDataSize);			
	}
    case SUB_S_ADD_SCORE: //�û���ע
    {
		//��Ϣ����
		return OnSubAddScore(pData,wDataSize);
    }
    case SUB_S_GAME_END:  //��Ϸ����
    {
		//��Ϣ����
		return OnSubGameEnd(pData,wDataSize);
    }
	case SUB_S_ANDROID_CONFIG:
	{
		 //Ч�����
		 ASSERT(wDataSize == sizeof(tagRobotConfig));
		 if (wDataSize != sizeof(tagRobotConfig)) return true;

		 //��������
		 tagRobotConfig * pAndroidConfig = (tagRobotConfig *)pData;

		 m_lRobotScoreRange[0] = pAndroidConfig->lRobotScoreMin;
		 m_lRobotScoreRange[1] = pAndroidConfig->lRobotScoreMax;

		 if (m_lRobotScoreRange[1] < m_lRobotScoreRange[0])
		 {
			 m_lRobotScoreRange[1] = m_lRobotScoreRange[0];
		 }

		 m_lRobotBankGetScore = pAndroidConfig->lRobotBankGet;
		 m_lRobotBankGetScoreBanker = pAndroidConfig->lRobotBankGetBanker;
		 m_nRobotBankStorageMul = pAndroidConfig->lRobotBankStoMul;

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
  switch(cbGameStatus)
  {
    case GAME_STATUS_FREE:    //����״̬
    {
		//Ч������
		if(wDataSize!=sizeof(CMD_S_StatusFree))
		{
			return true;
		}

		//��Ϣ����
		CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

		BankOperate(2);

		return true;
    }
    case GS_TK_CALL:  // ��ׯ״̬
    {
		//Ч������
		if(wDataSize!=sizeof(CMD_S_StatusCall))
		{
			return true;
		}
		CMD_S_StatusCall * pStatusCall=(CMD_S_StatusCall *)pData;

		BankOperate(2);

		//ʼ���û�
		if (pStatusCall->m_cbCallStatus[m_pIAndroidUserItem->GetChairID()] == INVALID_BANKER)
		{
			//��ׯʱ��
			UINT nElapse = TIME_LESS + (rand() % TIME_USER_CALL_BANKER);
			m_pIAndroidUserItem->SetGameTimer(IDI_CALL_BANKER,nElapse);
		}

		return true;
    }
    case GS_TK_SCORE: //��ע״̬
    {
		//Ч������
		if(wDataSize!=sizeof(CMD_S_StatusScore))
		{
			return true;
		}
		CMD_S_StatusScore * pStatusScore=(CMD_S_StatusScore *)pData;

		BankOperate(2);

		//���ñ���
		WORD wMeChairId = m_pIAndroidUserItem->GetChairID();

		//���ó���
		if(pStatusScore->lTableScore[wMeChairId]==0L)
		{
			//��עʱ��
			UINT nElapse = TIME_LESS + (rand() % TIME_USER_ADD_SCORE);
			m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE,nElapse);
		}

		return true;
    }
  }

  ASSERT(FALSE);

  return true;
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

//�û���ׯ
bool CAndroidUserItemSink::OnSubCallBanker(const void * pBuffer, WORD wDataSize)
{

	//Ч������
	if (wDataSize != sizeof(CMD_S_Call_Banker))
	{
		return true;
	}
	CMD_S_Call_Banker * pCallBanker = (CMD_S_Call_Banker *)pBuffer;
	return true;
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  if(wDataSize!=sizeof(CMD_S_GameStart))
  {
	  return true;
  }
  CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

  BYTE cbUserCount = 0;
  for (WORD i = 0; i < GAME_PLAYER; i++)
  {
	  if (pGameStart->cbPlayStatus[i] == TRUE)
		  cbUserCount++;
  }

  CopyMemory(m_HandCardData, pGameStart->cbCardData, sizeof(m_HandCardData));

  //��ׯʱ��
  UINT nUserPlayTime = (cbUserCount > 5) ? 2 : 1;
  UINT nElapse = TIME_LESS + (rand() % TIME_USER_CALL_BANKER) + nUserPlayTime;
  m_pIAndroidUserItem->SetGameTimer(IDI_CALL_BANKER, nElapse);
	  
  return true;
}

//��ׯ���
bool CAndroidUserItemSink::OnSubBankerEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize != sizeof(CMD_S_Banker_End))
	{
		return true;
	}
	CMD_S_Banker_End * pBankerEnd = (CMD_S_Banker_End *)pBuffer;

	//���ó���
	if (pBankerEnd->cbBanker != m_pIAndroidUserItem->GetChairID())
	{
		//��עʱ��
		UINT nElapse = TIME_LESS + (rand() % TIME_USER_ADD_SCORE);
		m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE, nElapse);
	}

	return true;
}


//�û���ע
bool CAndroidUserItemSink::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  if(wDataSize!=sizeof(CMD_S_AddScore))
  {
	  return true;
  }
  CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pBuffer;

  return true;
}


//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	if(wDataSize!=sizeof(CMD_S_GameEnd))
	{
	  return true;
	}
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//ɾ����ʱ��
	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_BANKER);
	m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);
	m_pIAndroidUserItem->KillGameTimer(IDI_START_GAME);
	

	//�������

	ZeroMemory(m_HandCardData,sizeof(m_HandCardData));


  return true;
}

//���в���
void CAndroidUserItemSink::BankOperate(BYTE cbType)
{
  if(cbType == 3)
  {
    if(rand() % 100 > 33)
    {
      return;
    }
  }
  IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();

  //��������
  LONGLONG lRobotScore = pUserItem->GetUserScore();

  {

    //�жϴ�ȡ
    if(lRobotScore > m_lRobotScoreRange[1])
    {
      LONGLONG lSaveScore=0L;

      lSaveScore = LONGLONG(lRobotScore*m_nRobotBankStorageMul/100);
      if(lSaveScore > lRobotScore)
      {
        lSaveScore = lRobotScore;
      }

      if(lSaveScore > 0)
      {
        m_pIAndroidUserItem->PerformSaveScore(lSaveScore);
      }

   
    }
    else if(lRobotScore < m_lRobotScoreRange[0])
    {

      SCORE lScore=rand()%m_lRobotBankGetScoreBanker+m_lRobotBankGetScore;
      if(lScore > 0)
      {
        m_pIAndroidUserItem->PerformTakeScore(lScore);
      }

    }
  }
}

//��ȡ����
void CAndroidUserItemSink::ReadConfigInformation()
{
}


//�����������
DECLARE_CREATE_MODULE(AndroidUserItemSink);

//////////////////////////////////////////////////////////////////////////
