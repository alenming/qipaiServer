#include "Stdafx.h"
#include "AndroidUserItemSink.h"
#include "math.h"
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//ʱ�䶨��
	m_cbTimeRangeCard = 0;

	//�ӿڱ���
	m_pIAndroidUserItem=NULL;

	return;
}

//��������
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//�ӿڲ�ѯ
VOID * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
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
	return true;
}

//ʱ����Ϣ
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_RANGE_CARD:  //�Զ�����
		{
			AutomaticOutCard();
			return true;
		}
	}

	return false;
}


//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_SENDCARD:			//�����˿�
		{
			return OnSubSendCardEx(pData,wDataSize);
		}
	case SUB_S_GAME_END:				//��Ϸ����
		{
			return OnSubGameEnd(pData,wDataSize);
		}
	}

	return true;

}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_WK_FREE:	//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��������
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			m_lCellScore = pStatusFree->lCellScore;
			//ʱ�䶨��
			m_cbTimeRangeCard = pStatusFree->cbDelayTime;

			//���в���
			BankOperate(2);

			return true;
		}
	case GS_WK_PLAYING: //��Ϸ״̬
		{
			//��֤����
			ASSERT( sizeof( CMD_S_StatusPlay ) == wDataSize );
			if ( sizeof( CMD_S_StatusPlay ) != wDataSize ) return false;

			CMD_S_StatusPlay *pStatusPlay = (CMD_S_StatusPlay *)pData;

			m_lCellScore = pStatusPlay->lCellScore;
			//ʱ�䶨��
			m_cbTimeRangeCard = pStatusPlay->cbDelayTime;

			//���в���
			BankOperate(2);

			if (pStatusPlay->cbPlayerStatus[m_pIAndroidUserItem->GetChairID()] != Range_End)
			{
				//ϴ��ʱ��
				UINT nElapse = rand() % m_cbTimeRangeCard + TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_RANGE_CARD, nElapse);
			}

			return true;
		}
	case GS_WK_END: //��Ϸ״̬
		{
			//��֤����
			ASSERT(sizeof(CMD_S_StatusEnd) == wDataSize);
			if (sizeof(CMD_S_StatusEnd) != wDataSize) return false;

			CMD_S_StatusEnd *pStatusPlay = (CMD_S_StatusEnd *)pData;

			m_lCellScore = pStatusPlay->lCellScore;
			//ʱ�䶨��
			m_cbTimeRangeCard = pStatusPlay->cbDelayTime;

			//���в���
			BankOperate(2);
			return true;
		}
	}
	return true;
}

//�û�����
VOID CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û��뿪
VOID CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�����
VOID CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�״̬
VOID CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û���λ
VOID CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}


//�����˿�
bool CAndroidUserItemSink::OnSubSendCardEx( const void * pBuffer, WORD wDataSize )
{
	if (m_pIAndroidUserItem->GetMeUserItem()->GetUserStatus() != US_PLAYING )
	{
		return true;
	}

	ASSERT(wDataSize == sizeof(CMD_S_SendCard));
	if (wDataSize != sizeof(CMD_S_SendCard)) return true;

	//��������
	CMD_S_SendCard * pSendCard = (CMD_S_SendCard *)pBuffer;
	IServerUserItem * pIServerUserItem = m_pIAndroidUserItem->GetMeUserItem();


	//�����˿�
	if (m_pIAndroidUserItem->GetMeUserItem()->GetUserStatus() == US_PLAYING)
	{
		CopyMemory(m_bHandCardData, pSendCard->cbCard, sizeof(BYTE)*HAND_CARD_COUNT);
		m_GameLogic.SortCardList(m_bHandCardData, HAND_CARD_COUNT);

		//ϴ��ʱ��
		UINT nElapse = rand() % m_cbTimeRangeCard + TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_RANGE_CARD,nElapse);
	}

	return true;
}

//�Զ�����
void CAndroidUserItemSink::AutomaticOutCard()
{
	IServerUserItem* pIServerUserItem = m_pIAndroidUserItem->GetMeUserItem();

	BYTE cbHandCard[MAX_COUNT];
	BYTE cbHandLen = 0;

	CopyMemory(cbHandCard, m_bHandCardData, HAND_CARD_COUNT);
	cbHandLen = HAND_CARD_COUNT;

	m_GameLogic.SortCardList(cbHandCard, cbHandLen, enDescend);

	//�Զ�����
	BYTE bAutoPutCard[13], bIndex[13];
	ZeroMemory(bAutoPutCard, sizeof(bAutoPutCard));
	ZeroMemory(bIndex, sizeof(bIndex));

	BYTE cbType = m_GameLogic.GetSpecialType(cbHandCard, 13);
	if (cbType != CT_INVALID)
	{
		BYTE cbSegCardData[13] = {0};
		if (0 != m_GameLogic.GetSpecialCardData(cbHandCard, cbHandLen, cbSegCardData))
		{
			CMD_C_ShowCard AllSegmentCard;
			ZeroMemory(&AllSegmentCard, sizeof(AllSegmentCard));

			CopyMemory(AllSegmentCard.cbCardData, cbSegCardData, sizeof(BYTE) * 13);
			m_GameLogic.SortCardList(AllSegmentCard.cbCardData, 13);

			AllSegmentCard.cbCardType[0] = cbType;

			m_pIAndroidUserItem->SendSocketData(SUB_C_PLAYER_SHOWCARD, &AllSegmentCard, sizeof(AllSegmentCard));

			return;
		}
	}

	m_GameLogic.AutoPutCard(cbHandCard, bAutoPutCard);

	//��Ӧ����
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			if (bAutoPutCard[i] == cbHandCard[j])
			{
				bIndex[i] = j;
				break;
			}
		}
	}

	BYTE tmpCard[3][5];
	ZeroMemory(tmpCard, sizeof(tmpCard));

	CopyMemory(tmpCard[0], &bAutoPutCard[0],sizeof(BYTE) * 3);
	m_GameLogic.SortCardList(tmpCard[0], 3, enDescend);

	CopyMemory(tmpCard[1], &bAutoPutCard[3], sizeof(BYTE) * 5);
	m_GameLogic.SortCardList(tmpCard[1], 5, enDescend);

	CopyMemory(tmpCard[2], &bAutoPutCard[8], sizeof(BYTE) * 5);
	m_GameLogic.SortCardList(tmpCard[2], 5, enDescend);

	CMD_C_ShowCard AllSegmentCard;
	ZeroMemory(&AllSegmentCard, sizeof(AllSegmentCard));

	CopyMemory(&AllSegmentCard.cbCardData[0], tmpCard[0], sizeof(BYTE) * 3);
	CopyMemory(&AllSegmentCard.cbCardData[3], tmpCard[1], sizeof(BYTE) * 5);
	CopyMemory(&AllSegmentCard.cbCardData[8], tmpCard[2], sizeof(BYTE) * 5);

	AllSegmentCard.cbCardType[1] = m_GameLogic.GetCardType(tmpCard[0],3);
	AllSegmentCard.cbCardType[2] = m_GameLogic.GetCardType(tmpCard[1], 5);
	AllSegmentCard.cbCardType[3] = m_GameLogic.GetCardType(tmpCard[2], 5);

	m_pIAndroidUserItem->SendSocketData(SUB_C_PLAYER_SHOWCARD, &AllSegmentCard, sizeof(AllSegmentCard));

}

bool CAndroidUserItemSink::OnSubGameEnd( const void * pBuffer, WORD wDataSize )

{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;


	//ɾ��ʱ��
	m_pIAndroidUserItem->KillGameTimer(IDI_RANGE_CARD);

	//�˿˱���
	ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));

	return true;
}

//���в���
void CAndroidUserItemSink::BankOperate(BYTE cbType)
{
	IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
	if(pUserItem->GetUserStatus()>=US_SIT)
	{
		if(cbType==1)
		{
			return;
		}
	}

	LONGLONG lRobotScoreRange[2] = {0};
	lRobotScoreRange[0] = m_lCellScore * 100;
	lRobotScoreRange[1] = m_lCellScore * 500;

	LONGLONG lRobotBankGetScoreBanker = m_lCellScore * 150;
	LONGLONG RobotBankGetScore = m_lCellScore * 450;

	int nRobotBankStorageMul = 30;

	//��������
	LONGLONG lRobotScore = pUserItem->GetUserScore();

	//�жϴ�ȡ
	if (lRobotScore > lRobotScoreRange[1])
	{
		LONGLONG lSaveScore=0L;
		lSaveScore = lRobotScore - lRobotScoreRange[1];
		SCORE lScore = rand() % (RobotBankGetScore - lRobotBankGetScoreBanker*2);
		lSaveScore += lScore;
		if (lSaveScore > lRobotScore) lSaveScore = lRobotScore;
		if (lSaveScore > 0) 
			m_pIAndroidUserItem->PerformSaveScore(lSaveScore);
	}
	else if (lRobotScore < lRobotScoreRange[0])
	{
		SCORE lScore = rand() % (RobotBankGetScore - lRobotBankGetScoreBanker) + lRobotBankGetScoreBanker;
		if (lScore > 0)
			m_pIAndroidUserItem->PerformTakeScore(lScore);
	}
	return;
}

//////////////////////////////////////////////////////////////////////////
DECLARE_CREATE_MODULE(AndroidUserItemSink);