#include "Stdafx.h"
#include "AndroidUserItemSink.h"
#include "math.h"
//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_BANK_OPERATE			3									//���ж�ʱ
#define IDI_PLACE_JETTON1			103									//��ע��ʱ
#define IDI_PLACE_JETTON2			104									//��ע��ʱ
#define IDI_PLACE_JETTON3			105									//��ע��ʱ
#define IDI_PLACE_JETTON4			106									//��ע��ʱ
#define IDI_PLACE_JETTON5			107									//��ע��ʱ
#define IDI_CHECK_BANKER			108									//�����ׯ
#define IDI_REQUEST_BANKER			101									//���붨ʱ
#define IDI_GIVEUP_BANKER			102									//��ׯ��ʱ
#define IDI_PLACE_JETTON			110									//��ע���� (Ԥ��110-160)

//////////////////////////////////////////////////////////////////////////

int CAndroidUserItemSink::m_stlApplyBanker = 0L;

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//��Ϸ����
	m_lMaxChipBanker = 0;
	m_lMaxChipUser = 0;
	m_wCurrentBanker = 0;
	m_nChipTime = 0;
	m_nChipTimeCount = 0;
	m_cbTimeLeave = 0;
	m_nListUserCount = 0;
	ZeroMemory(m_lAreaChip, sizeof(m_lAreaChip));
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));
	m_nRobotBetTimeLimit[0] = 3;
	m_nRobotBetTimeLimit[1] = 8;
	m_lRobotJettonLimit[0] = 10;
	m_lRobotJettonLimit[1] = 1000000;

	//��ׯ����
	m_bMeApplyBanker = false;
	m_nWaitBanker = 0;
	m_nBankerCount = 0;

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


  //�������
  //UINT nElapse=TIME_CHECK_BANKER+rand()%100;
  //m_pIAndroidUserItem->SetGameTimer(IDI_CHECK_BANKER_OPERATE,nElapse);

  return true;
}

//���ýӿ�
bool CAndroidUserItemSink::RepositionSink()
{
	//��Ϸ����
	m_lMaxChipBanker = 0;
	m_lMaxChipUser = 0;
	m_wCurrentBanker = 0;
	m_nChipTime = 0;
	m_nChipTimeCount = 0;
	m_cbTimeLeave = 0;
	ZeroMemory(m_lAreaChip, sizeof(m_lAreaChip));
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));

	//��ׯ����
	m_bMeApplyBanker = false;
	//m_nWaitBanker = 0;
	m_nBankerCount = 0;

	return true;
}

//ʱ����Ϣ
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_CHECK_BANKER:		//�����ׯ
	{
		m_pIAndroidUserItem->KillGameTimer(nTimerID);

		if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID())
			return false;

		if (m_wCurrentBanker == INVALID_CHAIR || m_nListUserCount < m_nRobotApplyBanker)
		{
			//��ׯ
			m_nWaitBanker++;

			//��������ׯ
			if (m_bRobotBanker && m_nWaitBanker > m_nRobotWaitBanker && m_stlApplyBanker < m_nRobotApplyBanker)
			{
				//�Ϸ��ж�
				IServerUserItem *pIUserItemBanker = m_pIAndroidUserItem->GetMeUserItem();
				if (pIUserItemBanker->GetUserScore() > m_lBankerCondition)
				{
					//��������ׯ
					m_nBankerCount = 0;
					m_stlApplyBanker++;

					m_pIAndroidUserItem->SendSocketData(SUB_C_APPLY_BANKER);
				}
			}
		}

		return false;
	}
	case IDI_REQUEST_BANKER:	//������ׯ
	{
		m_pIAndroidUserItem->KillGameTimer(nTimerID);

		m_pIAndroidUserItem->SendSocketData(SUB_C_APPLY_BANKER);

		return false;
	}
	case IDI_GIVEUP_BANKER:		//������ׯ
	{
		m_pIAndroidUserItem->KillGameTimer(nTimerID);

		m_pIAndroidUserItem->SendSocketData(SUB_C_CANCEL_BANKER);

		return false;
	}
	case IDI_BANK_OPERATE:		//���в���
	{
		m_pIAndroidUserItem->KillGameTimer(nTimerID);

		//��������
		IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
		LONGLONG lRobotScore = pUserItem->GetUserScore();
		{

			//�жϴ�ȡ
			if (lRobotScore > m_lRobotScoreRange[1])
			{
				LONGLONG lSaveScore = 0L;

				lSaveScore = LONGLONG(lRobotScore*m_nRobotBankStorageMul / 100);
				if (lSaveScore > lRobotScore)  lSaveScore = lRobotScore;

				if (lSaveScore > 0 && m_wCurrentBanker != m_pIAndroidUserItem->GetChairID())
					m_pIAndroidUserItem->PerformSaveScore(lSaveScore);

			}
			else if (lRobotScore < m_lRobotScoreRange[0])
			{
				SCORE lScore = rand() % m_lRobotBankGetScoreBanker + m_lRobotBankGetScore;
				if (lScore > 0)
					m_pIAndroidUserItem->PerformTakeScore(lScore);
			}
		}
		return false;
	}
	default:
	{
		if (nTimerID >= IDI_PLACE_JETTON && nTimerID <= IDI_PLACE_JETTON + MAX_CHIP_TIME)
		{
			srand(GetTickCount());

			//��������
			int nRandNum = 0, nChipArea = 0, nCurChip = 0, nACTotal = 0, nCurJetLmt[2] = {};
			LONGLONG lMaxChipLmt = __min(m_lMaxChipBanker, m_lMaxChipUser);			//������עֵ
			WORD wMyID = m_pIAndroidUserItem->GetChairID();
			for (int i = 0; i <AREA_COUNT; i++)
				nACTotal += m_RobotInfo.nAreaChance[i];

			//ͳ�ƴ���
			m_nChipTimeCount++;

			//����˳�
			if (lMaxChipLmt < m_RobotInfo.nChip[m_nChipLimit[0]])	return false;
			for (int i = 0; i < AREA_COUNT; i++)
			{
				if (m_lAreaChip[i] >= m_RobotInfo.nChip[m_nChipLimit[0]])	break;
				if (i == AREA_COUNT - 1)	return false;
			}

			//��ע����
			ASSERT(nACTotal>0);
			static int nStFluc = 1;				//�������
			if (nACTotal <= 0)	return false;
			do {
				nRandNum = (rand() + wMyID + nStFluc * 3) % nACTotal;
				for (int i = 0; i < AREA_COUNT; i++)
				{
					nRandNum -= m_RobotInfo.nAreaChance[i];
					if (nRandNum < 0)
					{
						nChipArea = i;
						break;
					}
				}
			} while (m_lAreaChip[nChipArea] < m_RobotInfo.nChip[m_nChipLimit[0]]);
			nStFluc = nStFluc % 3 + 1;

			//��ע��С
			//if (m_nChipLimit[0] == m_nChipLimit[1])
			//	nCurChip = m_nChipLimit[0];
			//else
			//{
			//	//���ñ���
			//	lMaxChipLmt = __min(lMaxChipLmt, m_lAreaChip[nChipArea]);
			//	nCurJetLmt[0] = m_nChipLimit[0];
			//	nCurJetLmt[1] = m_nChipLimit[0];

			//	//���㵱ǰ������
			//	for (int i = m_nChipLimit[1]; i > m_nChipLimit[0]; i--)
			//	{
			//		if (lMaxChipLmt > m_RobotInfo.nChip[i]) 
			//		{
			//			nCurJetLmt[1] = i;
			//			break;
			//		}
			//	}

			//	//�����ע
			//	nRandNum = (rand()+wMyID) % (nCurJetLmt[1]-nCurJetLmt[0]+1);
			//	nCurChip = nCurJetLmt[0] + nRandNum;

			//	//���¿��� (��ׯ�ҽ�ҽ���ʱ�ᾡ����֤�������)
			//	if (m_nChipTimeCount < m_nChipTime)
			//	{
			//		LONGLONG lLeftJetton = LONGLONG( (lMaxChipLmt-m_RobotInfo.nChip[nCurChip])/(m_nChipTime-m_nChipTimeCount) );

			//		//�������� (��ȫ����С���Ƴ�����עҲ����)
			//		if (lLeftJetton < m_RobotInfo.nChip[m_nChipLimit[0]] && nCurChip > m_nChipLimit[0])
			//			nCurChip--;
			//	}
			//}

			nCurChip = rand() % 1000;
			if (nCurChip >= 998)
				nCurChip = 6;
			else if (nCurChip >= 995)
				nCurChip = 5;
			else if (nCurChip >= 990)
				nCurChip = 4;
			if (nCurChip >= 980)
				nCurChip = 3;
			else if (nCurChip >= 950)
				nCurChip = 2;
			else if (nCurChip >= 500)
				nCurChip = 1;
			else
				nCurChip = 0;

			//��������
			CMD_C_PlaceJetton PlaceJetton = {};

			//�������
			BYTE cbArea[3] = {1,4,5};
			PlaceJetton.cbJettonArea = cbArea[rand() % 3];		//������1��ʼ
			PlaceJetton.lJettonScore = m_RobotInfo.nChip[nCurChip % 7];

			//������Ϣ
			m_pIAndroidUserItem->SendSocketData(SUB_C_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
		}

		m_pIAndroidUserItem->KillGameTimer(nTimerID);
		return false;
	}
	}

  return false;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	switch(wSubCmdID)
	{
		case SUB_S_GAME_FREE:			//��Ϸ���� 
		{
			return OnSubGameFree(pData, wDataSize);
		}
		case SUB_S_GAME_START:  //��Ϸ��ʼ
		{
			//��Ϣ����
			return OnSubGameStart(pData,wDataSize);
		}
		case SUB_S_PLACE_JETTON:		//�û���ע
		{
			return OnSubPlaceJetton(pData, wDataSize);
		}
		case SUB_S_APPLY_BANKER:		//������ׯ 
		{
			return OnSubUserApplyBanker(pData,wDataSize);
		}
		case SUB_S_CANCEL_BANKER:		//ȡ����ׯ 
		{
			return OnSubUserCancelBanker(pData,wDataSize);
		}
		case SUB_S_CHANGE_BANKER:		//�л�ׯ�� 
		{
			return OnSubChangeBanker(pData,wDataSize);
		}
		case SUB_S_GAME_END:			//��Ϸ���� 
		{
			return OnSubGameEnd(pData, wDataSize);
		}
		case SUB_S_SEND_RECORD:			//��Ϸ��¼ (����)
		{
			return true;
		}
		case SUB_S_PLACE_JETTON_FAIL:	//��עʧ�� (����)
		{
			return true;
		}
		case SUB_ANDROID_INIT:			//��������Ϣ
		{
			CMD_S_AndroidInit * pAndroidInit = (CMD_S_AndroidInit *)pData;
			memcpy(m_szRoomName, pAndroidInit->szRoomName, sizeof(m_szRoomName));
			memcpy(m_RobotInfo.szCfgFileName, pAndroidInit->szConfigName, sizeof(m_RobotInfo.szCfgFileName));
			ReadConfigInformation(m_RobotInfo.szCfgFileName, m_szRoomName, true);
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
		case GAME_STATUS_FREE:			//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��Ϣ����
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			m_lBankerCondition = pStatusFree->lApplyBankerCondition;

			ReadConfigInformation(m_RobotInfo.szCfgFileName, m_szRoomName, true);


			return true;
		}
		case GAME_STATUS_PLAY:		//��Ϸ״̬
		case GAME_STATUS_WAIT:		//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//��Ϣ����
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			m_lBankerCondition = pStatusPlay->lApplyBankerCondition;

			ReadConfigInformation(m_RobotInfo.szCfgFileName, m_szRoomName, true);

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

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameFree(const VOID * pBuffer, WORD wDataSize)
{
	//��ȡ����
	m_stlApplyBanker = 0;

	//��Ϣ����
	CMD_S_GameFree* pGameFree = (CMD_S_GameFree *)pBuffer;

	m_cbTimeLeave = pGameFree->cbTimeLeave;
	//m_nListUserCount = pGameFree->nListUserCount;

	//���в���
	if (pGameFree->cbTimeLeave > 1)
		m_pIAndroidUserItem->SetGameTimer(IDI_BANK_OPERATE, (rand() % (pGameFree->cbTimeLeave - 1)) + 1);

	bool bMeGiveUp = false;
	if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID())
	{
		m_nBankerCount++;
		if (m_nBankerCount >= m_nRobotBankerCount)
		{
			//��������ׯ
			m_nBankerCount = 0;
			m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP_BANKER, rand() % 2 + 1);

			bMeGiveUp = true;
		}
	}

	//�����ׯ
	if (m_wCurrentBanker != m_pIAndroidUserItem->GetChairID() /*|| bMeGiveUp*/)
	{
		m_cbTimeLeave = pGameFree->cbTimeLeave - 3;
		m_pIAndroidUserItem->SetGameTimer(IDI_CHECK_BANKER, 3);
	}

	return true;
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize == sizeof(CMD_S_GameStart));
	if (wDataSize != sizeof(CMD_S_GameStart)) return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart = (CMD_S_GameStart *)pBuffer;

	srand(GetTickCount());

	//���ñ���
	m_lMaxChipBanker = pGameStart->lBankerScore / m_RobotInfo.nMaxTime;
	m_lMaxChipUser = m_pIAndroidUserItem->GetMeUserItem()->GetUserScore() / m_RobotInfo.nMaxTime;
	m_nRobotApplyBanker = rand() % m_nRobotApplyBanker + 1;
	m_wCurrentBanker = pGameStart->wBankerUser;
	m_nChipTimeCount = 0;
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));
	for (int i = 0; i <AREA_COUNT; i++)
		m_lAreaChip[i] = m_lAreaLimitScore;

	//�Լ���ׯ������ע������
	if (pGameStart->wBankerUser == m_pIAndroidUserItem->GetChairID())
		return true;

	//ϵͳ��ׯ
	if (pGameStart->wBankerUser == INVALID_CHAIR)
	{
		m_stlApplyBanker = 0;
		m_lMaxChipBanker = 0;// 2147483647 / m_RobotInfo.nMaxTime;
		return true;
	}
	else
		m_lMaxChipUser = __min(m_lMaxChipUser, m_lMaxChipBanker);

	//������ע����
	int nElapse = 0;
	WORD wMyID = m_pIAndroidUserItem->GetChairID();

	if (m_nRobotBetTimeLimit[0] == m_nRobotBetTimeLimit[1])
		m_nChipTime = m_nRobotBetTimeLimit[0];
	else
		m_nChipTime = (rand() + wMyID) % (m_nRobotBetTimeLimit[1] - m_nRobotBetTimeLimit[0] + 1) + m_nRobotBetTimeLimit[0];
	ASSERT(m_nChipTime >= 0);
	if (m_nChipTime <= 0)	return true;								//��ȷ,2����������
	if (m_nChipTime > MAX_CHIP_TIME)	m_nChipTime = MAX_CHIP_TIME;	//�޶�MAX_CHIP����ע

	//���㷶Χ
	if (!CalcJettonRange(__min(m_lMaxChipBanker, m_lMaxChipUser), m_lRobotJettonLimit, m_nChipTime, m_nChipLimit))
		return true;

	//����ʱ��
	int nTimeGrid = int(pGameStart->cbTimeLeave - 2) * 800 / m_nChipTime;		//ʱ���,ǰ2�벻��ע,����-2,800��ʾ��������עʱ�䷶Χǧ�ֱ�
	for (int i = 0; i < m_nChipTime; i++)
	{
		int nRandRage = int(nTimeGrid * i / (1500 * sqrt((double)m_nChipTime))) + 1;		//������Χ
		nElapse = 2 + (nTimeGrid*i) / 1000 + ((rand() + wMyID) % (nRandRage * 2) - (nRandRage - 1));
		ASSERT(nElapse >= 2 && nElapse <= pGameStart->cbTimeLeave);
		if (nElapse < 2 || nElapse > pGameStart->cbTimeLeave)	continue;

		m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON + i + 1, nElapse);
	}

	return true;
}

//�û���ע
bool CAndroidUserItemSink::OnSubPlaceJetton(const VOID * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize == sizeof(CMD_S_PlaceJetton));
	if (wDataSize != sizeof(CMD_S_PlaceJetton)) return false;

	//��Ϣ����
	CMD_S_PlaceJetton * pPlaceJetton = (CMD_S_PlaceJetton *)pBuffer;

	//���ñ���
	m_lMaxChipBanker -= pPlaceJetton->lJettonScore;
	m_lAreaChip[pPlaceJetton->cbJettonArea - 1] -= pPlaceJetton->lJettonScore;
	if (pPlaceJetton->wChairID == m_pIAndroidUserItem->GetChairID())
		m_lMaxChipUser -= pPlaceJetton->lJettonScore;

	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	BankOperate(2);
	return true;
}

//������ׯ
bool CAndroidUserItemSink::OnSubUserApplyBanker(const VOID * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize == sizeof(CMD_S_ApplyBanker));
	if (wDataSize != sizeof(CMD_S_ApplyBanker)) return false;

	//��Ϣ����
	CMD_S_ApplyBanker * pApplyBanker = (CMD_S_ApplyBanker *)pBuffer;

	//�Լ��ж�
	if (m_pIAndroidUserItem->GetChairID() == pApplyBanker->wApplyUser)
		m_bMeApplyBanker = true;

	return true;
}

//ȡ����ׯ
bool CAndroidUserItemSink::OnSubUserCancelBanker(const VOID * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize == sizeof(CMD_S_CancelBanker));
	if (wDataSize != sizeof(CMD_S_CancelBanker)) return false;

	//��Ϣ����
	CMD_S_CancelBanker * pCancelBanker = (CMD_S_CancelBanker *)pBuffer;

	//�Լ��ж�
	if (m_pIAndroidUserItem->GetMeUserItem()->GetChairID() == pCancelBanker->wCancelUser)
		m_bMeApplyBanker = false;

	return true;
}

//�л�ׯ��
bool CAndroidUserItemSink::OnSubChangeBanker(const VOID * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize == sizeof(CMD_S_ChangeBanker));
	if (wDataSize != sizeof(CMD_S_ChangeBanker)) return false;

	//��Ϣ����
	CMD_S_ChangeBanker * pChangeBanker = (CMD_S_ChangeBanker *)pBuffer;

	if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID() && m_wCurrentBanker != pChangeBanker->wBankerUser)
	{
		m_stlApplyBanker--;
	}
	m_wCurrentBanker = pChangeBanker->wBankerUser;
	//m_nWaitBanker = 0;

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

  if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID())
	  return;

  IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
  if(pUserItem->GetUserStatus()>=US_SIT)
  {
    if(cbType==1)
    {
      CString strInfo;
      strInfo.Format(TEXT("������״̬���ԣ���ִ�д�ȡ��"));
      //NcaTextOut(strInfo);

      return;

    }
  }

  //��������
  LONGLONG lRobotScore = pUserItem->GetUserScore();

  {
    CString strInfo;
    strInfo.Format(TEXT("[%s] ����(%I64d)"),pUserItem->GetNickName(),lRobotScore);

    if(lRobotScore > m_lRobotScoreRange[1])
    {
      CString strInfo1;
      strInfo1.Format(TEXT("����������(%I64d)"),m_lRobotScoreRange[1]);
      strInfo+=strInfo1;
      //if(cbType==1)
      //NcaTextOut(strInfo);
    }
    else if(lRobotScore < m_lRobotScoreRange[0])
    {
      CString strInfo1;
      strInfo1.Format(TEXT("����ȡ������(%I64d)"),m_lRobotScoreRange[0]);
      strInfo+=strInfo1;
      //if(cbType==1)
      //NcaTextOut(strInfo);
    }

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

      LONGLONG lRobotNewScore = pUserItem->GetUserScore();

      CString strInfo;
      strInfo.Format(TEXT("[%s] ִ�д����ǰ���(%I64d)��������(%I64d)"),pUserItem->GetNickName(),lRobotScore,lRobotNewScore);

      //if(cbType==1)
      //NcaTextOut(strInfo);

    }
    else if(lRobotScore < m_lRobotScoreRange[0])
    {

      CString strInfo;
      //strInfo.Format(TEXT("������Ϣ��ȡ����Сֵ(%I64d)��ȡ�����ֵ(%I64d)"),m_lRobotBankGetScore,m_lRobotBankGetScoreBanker);

      //if(cbType==1)
      //  NcaTextOut(strInfo);

      SCORE lScore=rand()%m_lRobotBankGetScoreBanker+m_lRobotBankGetScore;
      if(lScore > 0)
      {
        m_pIAndroidUserItem->PerformTakeScore(lScore);
      }

      LONGLONG lRobotNewScore = pUserItem->GetUserScore();

      //CString strInfo;
      strInfo.Format(TEXT("[%s] ִ��ȡ�ȡ��ǰ���(%I64d)��ȡ�����(%I64d)"),pUserItem->GetNickName(),lRobotScore,lRobotNewScore);

      //if(cbType==1)
      //NcaTextOut(strInfo);

    }
  }
}

//��ȡ����
VOID CAndroidUserItemSink::ReadConfigInformation(TCHAR szFileName[], TCHAR szRoomName[], bool bReadFresh)
{
	//�����ļ���
	TCHAR szPath[MAX_PATH] = TEXT("");
	TCHAR szConfigFileName[MAX_PATH] = TEXT("");
	TCHAR OutBuf[255] = TEXT("");
	GetCurrentDirectory(sizeof(szPath), szPath);
	swprintf(szConfigFileName, sizeof(szConfigFileName), _T("%s"), szFileName);

	//��������
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotMaxJetton"), _T("500"), OutBuf, 255, szConfigFileName);
	_snwscanf(OutBuf, lstrlen(OutBuf), _T("%I64d"), &m_lRobotJettonLimit[1]);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotMinJetton"), _T("1"), OutBuf, 255, szConfigFileName);
	_snwscanf(OutBuf, lstrlen(OutBuf), _T("%I64d"), &m_lRobotJettonLimit[0]);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("AreaLimitScore"), _T("100000"), OutBuf, 255, szConfigFileName);
	_snwscanf(OutBuf, lstrlen(OutBuf), _T("%I64d"), &m_lAreaLimitScore);


	if (m_lRobotJettonLimit[1] > 500)					m_lRobotJettonLimit[1] = 500;
	if (m_lRobotJettonLimit[0] < 1)						m_lRobotJettonLimit[0] = 1;
	if (m_lRobotJettonLimit[1] < m_lRobotJettonLimit[0])	m_lRobotJettonLimit[1] = m_lRobotJettonLimit[0];

	//��������
	m_nRobotBetTimeLimit[0] = GetPrivateProfileInt(szRoomName, _T("RobotMinBetTime"), 4, szConfigFileName);;
	m_nRobotBetTimeLimit[1] = GetPrivateProfileInt(szRoomName, _T("RobotMaxBetTime"), 8, szConfigFileName);;

	if (m_nRobotBetTimeLimit[0] < 0)							m_nRobotBetTimeLimit[0] = 0;
	if (m_nRobotBetTimeLimit[1] < m_nRobotBetTimeLimit[0])		m_nRobotBetTimeLimit[1] = m_nRobotBetTimeLimit[0];

	//�Ƿ���ׯ
	m_bRobotBanker = (GetPrivateProfileInt(szRoomName, _T("RobotBanker"), 1, szConfigFileName) == 1);

	//��ׯ����
	m_nRobotBankerCount = GetPrivateProfileInt(szRoomName, _T("RobotBankerCount"), 3, szConfigFileName);

	//��������
	m_nRobotWaitBanker = GetPrivateProfileInt(szRoomName, _T("RobotWaitBanker"), 1, szConfigFileName);

	//������
	m_nRobotApplyBanker = GetPrivateProfileInt(szRoomName, _T("RobotApplyBanker"), 5, szConfigFileName);

	//��������
	m_lRobotScoreRange[0] = GetPrivateProfileInt64(szRoomName, _T("RobotScoreMin"), 10000, szConfigFileName);
	m_lRobotScoreRange[1] = GetPrivateProfileInt64(szRoomName, _T("RobotScoreMax"), 20000, szConfigFileName);

	if (m_lRobotScoreRange[1] < m_lRobotScoreRange[0])	m_lRobotScoreRange[1] = m_lRobotScoreRange[0];

	//�������
	m_lRobotBankGetScore = GetPrivateProfileInt64(szRoomName, _T("RobotMinGet"), 10000, szConfigFileName);

	//������� (ׯ��)
	m_lRobotBankGetScoreBanker = GetPrivateProfileInt64(szRoomName, _T("RobotMaxGet"), 20000, szConfigFileName);

	//����
	m_nRobotBankStorageMul = GetPrivateProfileInt(szRoomName, _T("RobotBankStoMul"), 20, szConfigFileName);

	if (m_nRobotBankStorageMul<0 || m_nRobotBankStorageMul>100) m_nRobotBankStorageMul = 20;

	for (int i = 0; i < AREA_COUNT; i++)
	{
		m_RobotInfo.nAreaChance[i] = 1;
	}
}

//��ȡ����
LONGLONG CAndroidUserItemSink::GetPrivateProfileInt64(LPCTSTR lpAppName, LPCTSTR lpKeyName, LONGLONG lDefault, LPCTSTR lpFileName)
{
	//��������
	TCHAR OutBuf[255] = _T("");
	TCHAR DefaultBuf[255] = {};
	LONGLONG lNumber = 0;

	GetPrivateProfileString(lpAppName, lpKeyName, DefaultBuf, OutBuf, 255, lpFileName);

	if (OutBuf[0] != 0)
		_snwscanf(OutBuf, lstrlen(OutBuf), _T("%I64d"), &lNumber);
	else
		lNumber = lDefault;

	return lNumber;
}

//���㷶Χ	(����ֵ��ʾ�Ƿ����ͨ���½����޴ﵽ��ע)
bool CAndroidUserItemSink::CalcJettonRange(LONGLONG lMaxScore, LONGLONG lChipLmt[], int & nChipTime, int lJetLmt[])
{
	//�������
	bool bHaveSetMinChip = false;

	//����һע
	if (lMaxScore < m_RobotInfo.nChip[0])	return false;

	//���÷�Χ
	for (int i = 0; i < CountArray(m_RobotInfo.nChip); i++)
	{
		if (!bHaveSetMinChip && m_RobotInfo.nChip[i] >= lChipLmt[0])
		{
			lJetLmt[0] = i;
			bHaveSetMinChip = true;
		}
		if (m_RobotInfo.nChip[i] <= lChipLmt[1])
			lJetLmt[1] = i;
	}
	if (lJetLmt[0] > lJetLmt[1])	lJetLmt[0] = lJetLmt[1];

	return true;
}

//�����������
DECLARE_CREATE_MODULE(AndroidUserItemSink);

//////////////////////////////////////////////////////////////////////////
