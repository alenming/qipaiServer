#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//����ʱ��
#define TIME_LESS					1									//����ʱ��
#define TIME_DISPATCH				5									//����ʱ��

//��Ϸʱ��
#define TIME_OUT_CARD				3									//����ʱ��
#define TIME_START_GAME				8									//��ʼʱ��
#define TIME_CALL_SCORE				5									//�з�ʱ��

//��Ϸʱ��
#define IDI_OUT_CARD				(0)			//����ʱ��
#define IDI_START_GAME				(1)			//��ʼʱ��
#define IDI_CALL_SCORE				(2)			//�з�ʱ��
#define IDI_CALL_DOUBLE				(3)			//�ӱ�ʱ��

//////////////////////////////////////////////////////////////////////////
//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_cbCurrentLandScore = 255 ;				//�ѽз���
	m_wOutCardUser = INVALID_CHAIR ;

	//�˿˱���
	m_cbTurnCardCount=0;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

	//�����˿�
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

	//�ӿڱ���
	m_pIAndroidUserItem=NULL;;

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
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;

	//�˿˱���
	m_cbTurnCardCount=0;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

	//�����˿�
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

	return true;
}

//ʱ����Ϣ
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:	//��ʼ��Ϸ
		{
			//��ʼ�ж�
			m_pIAndroidUserItem->SendUserReady(NULL,0);

			return true;
		}
	case IDI_CALL_SCORE:	//�û��з�
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_CALL_SCORE);
			//�������
			CMD_C_CallScore CallScore;
			ZeroMemory(&CallScore,sizeof(CallScore));

			//���ñ���
			CallScore.cbCallScore=m_GameLogic.LandScore(m_pIAndroidUserItem->GetChairID(), m_cbCurrentLandScore);

			//��������
			m_pIAndroidUserItem->SendSocketData(SUB_C_CALL_SCORE,&CallScore,sizeof(CallScore));

			return true;
		}
	case IDI_CALL_DOUBLE:	//�û��ӱ�
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_CALL_DOUBLE);
			//�������
			CMD_C_CallDouble CallDouble;
			ZeroMemory(&CallDouble, sizeof(CallDouble));

			//������Ŀ
			BYTE cbLargeCardCount = 0;
			BYTE Index = 0;
			while (m_GameLogic.GetCardLogicValue(m_cbHandCardData[Index++]) >= 15) ++cbLargeCardCount;

			BYTE cbScore = (cbLargeCardCount >= 4) ? 2 : 1;

			//���ñ���
			CallDouble.cbPlayerMultiple = cbScore;

			if (CallDouble.cbPlayerMultiple != 1 && CallDouble.cbPlayerMultiple != 2)
				CallDouble.cbPlayerMultiple = 1;

			//��������
			m_pIAndroidUserItem->SendSocketData(SUB_C_DOUBLE, &CallDouble, sizeof(CallDouble));

			return true;
		}
	case IDI_OUT_CARD:		//�û�����
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_OUT_CARD);
			//�˿˷���
			tagOutCardResult OutCardResult;
			try
			{
				WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
				
				m_GameLogic.SearchOutCard(m_cbHandCardData,m_cbHandCardCount[wMeChairID],m_cbTurnCardData,m_cbTurnCardCount, m_wOutCardUser, m_pIAndroidUserItem->GetChairID(), OutCardResult);
			}
			catch(...)
			{
				//��������ã�ʹ�ý��������if����
				ZeroMemory(OutCardResult.cbResultCard, sizeof(OutCardResult.cbResultCard)) ;
				OutCardResult.cbCardCount = 10 ;
			}
			//���ͺϷ��ж�
			if(OutCardResult.cbCardCount>0 && CT_ERROR==m_GameLogic.GetCardType(OutCardResult.cbResultCard, OutCardResult.cbCardCount))
			{
				ASSERT(false) ;
				ZeroMemory(&OutCardResult, sizeof(OutCardResult)) ;
			}

			//�ȳ��Ʋ���Ϊ��
			if(m_cbTurnCardCount==0)
			{
				ASSERT(OutCardResult.cbCardCount>0) ;
				if(OutCardResult.cbCardCount==0)
				{
					WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
					//��Сһ��
					OutCardResult.cbCardCount = 1 ;
					OutCardResult.cbResultCard[0]=m_cbHandCardData[m_cbHandCardCount[wMeChairID]-1] ;
				}
			}
			else
			{
				if(!m_GameLogic.CompareCard(m_cbTurnCardData,OutCardResult.cbResultCard,m_cbTurnCardCount,OutCardResult.cbCardCount))
				{
					//��������
					m_pIAndroidUserItem->SendSocketData(SUB_C_PASS_CARD);
					return true;
				}				
			}

			//����ж�
			if (OutCardResult.cbCardCount>0)
			{
				WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
				
				//�������
				CMD_C_OutCard OutCard;
				ZeroMemory(&OutCard,sizeof(OutCard));

				//���ñ���
				OutCard.cbCardCount=OutCardResult.cbCardCount;
				CopyMemory(OutCard.cbCardData,OutCardResult.cbResultCard,OutCardResult.cbCardCount*sizeof(BYTE));

				//��������
				WORD wHeadSize=sizeof(OutCard)-sizeof(OutCard.cbCardData);
				m_pIAndroidUserItem->SendSocketData(SUB_C_OUT_CARD,&OutCard,wHeadSize+OutCard.cbCardCount*sizeof(BYTE));
			}
			else
			{
				//��������
				m_pIAndroidUserItem->SendSocketData(SUB_C_PASS_CARD);
			}

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
	case SUB_S_GAME_START:	//��Ϸ��ʼ
		{
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_CALL_SCORE:	//�û��з�
		{
			return OnSubCallScore(pData,wDataSize);
		}
	case SUB_S_BANKER_INFO:	//ׯ����Ϣ
		{
			return OnSubBankerInfo(pData,wDataSize);
		}
	case SUB_S_BANKER_OUT:	//ׯ�ҳ���
		{
			return OnSubBankerOutCard(pData, wDataSize);
		}
	case SUB_S_OUT_CARD:	//�û�����
		{
			return OnSubOutCard(pData,wDataSize);
		}
	case SUB_S_PASS_CARD:	//�û�����
		{
			return OnSubPassCard(pData,wDataSize);
		}
	case SUB_S_GAME_CONCLUDE:	//��Ϸ����
		{
			return OnSubGameEnd(pData,wDataSize);
		}
	}

	//�������
//	ASSERT(FALSE);

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
	case GAME_SCENE_FREE:	//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��������
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;
			IServerUserItem * pIServerUserItem=m_pIAndroidUserItem->GetMeUserItem();

			//�������
			//if (pIServerUserItem->GetUserStatus()!=US_READY)
			//{
			//	UINT nElapse=rand()%TIME_START_GAME+TIME_LESS;
			//	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
			//}

			return true;
		}
	case GAME_SCENE_CALL:	//�з�״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusCall)) return false;
			CMD_S_StatusCall * pStatusCall=(CMD_S_StatusCall *)pData;

			//�˿�����
			for (WORD i=0;i<GAME_PLAYER;i++) m_cbHandCardCount[i]=NORMAL_COUNT;
			CopyMemory(m_cbHandCardData,pStatusCall->cbHandCardData,sizeof(BYTE)*NORMAL_COUNT);

			//�з�����
			if (m_pIAndroidUserItem->GetChairID()==pStatusCall->wCurrentUser)
			{
				UINT nElapse=rand()%TIME_CALL_SCORE+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_CALL_SCORE,nElapse+TIME_DISPATCH);
			}

			return true;
		}
	case GAME_SCENE_PLAY:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//���Ʊ���
			m_cbTurnCardCount=pStatusPlay->cbTurnCardCount;
			CopyMemory(m_cbTurnCardData,pStatusPlay->cbTurnCardData,m_cbTurnCardCount*sizeof(BYTE));

			//�˿�����
			WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
			CopyMemory(m_cbHandCardCount,pStatusPlay->cbHandCardCount,sizeof(m_cbHandCardCount));
			CopyMemory(m_cbHandCardData,pStatusPlay->cbHandCardData,sizeof(BYTE)*m_cbHandCardCount[wMeChairID]);

			//�������
			if (pStatusPlay->wCurrentUser==m_pIAndroidUserItem->GetChairID())
			{
				UINT nElapse=rand()%TIME_OUT_CARD+TIME_LESS+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_OUT_CARD,nElapse);
			}

			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
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

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_S_AndroidCard));
	if (wDataSize!=sizeof(CMD_S_AndroidCard)) return false;

	//��������
	CMD_S_AndroidCard * pAndroidCard=(CMD_S_AndroidCard *)pData;

	//����״̬
	m_pIAndroidUserItem->SetGameStatus(GAME_SCENE_CALL);
	m_cbCurrentLandScore = 255 ;

	//�˿˱���
	m_cbTurnCardCount=0;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

	//�����˿�
	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
	for (WORD i=0;i<GAME_PLAYER;i++) m_cbHandCardCount[i]=NORMAL_COUNT;

	for(WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		if(wChairID==wMeChairID)
            CopyMemory(m_cbHandCardData,pAndroidCard->cbHandCard[wChairID],sizeof(BYTE)*NORMAL_COUNT);

		m_GameLogic.SetUserCard(wChairID, pAndroidCard->cbHandCard[wChairID], NORMAL_COUNT) ;
	}

	//�����˿�
	BYTE cbLandScoreCardData[MAX_COUNT] ;
	CopyMemory(cbLandScoreCardData, m_cbHandCardData,m_cbHandCardCount[wMeChairID]) ;
	CopyMemory(cbLandScoreCardData+NORMAL_COUNT, pAndroidCard->cbBankerCard, 3*sizeof(BYTE)) ;
	m_GameLogic.SetLandScoreCardData(cbLandScoreCardData, sizeof(cbLandScoreCardData)) ;

	//�����˿�
	m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount[wMeChairID],ST_ORDER);

	//��Ҵ���
	if (m_pIAndroidUserItem->GetChairID()==pAndroidCard->wCurrentUser)
	{
		UINT nElapse=rand()%TIME_CALL_SCORE+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_CALL_SCORE,nElapse+TIME_DISPATCH);
	}

	return true;
}

//�û��з�
bool CAndroidUserItemSink::OnSubCallScore(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_S_CallScore));
	if (wDataSize!=sizeof(CMD_S_CallScore)) return false;

	m_pIAndroidUserItem->KillGameTimer(IDI_OUT_CARD);
	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_SCORE);
	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_DOUBLE);

	//��������
	CMD_S_CallScore * pCallScore=(CMD_S_CallScore *)pData;
	m_cbCurrentLandScore = pCallScore->cbCurrentScore ;

	//�û�����
	if (m_pIAndroidUserItem->GetChairID()==pCallScore->wCurrentUser)
	{
		UINT nElapse=rand()%TIME_CALL_SCORE+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_CALL_SCORE,nElapse);
	}

	return true;
}

//ׯ����Ϣ
bool CAndroidUserItemSink::OnSubBankerInfo(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_S_BankerInfo));
	if (wDataSize!=sizeof(CMD_S_BankerInfo)) return false;

	m_pIAndroidUserItem->KillGameTimer(IDI_OUT_CARD);
	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_SCORE);
	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_DOUBLE);

	//��������
	CMD_S_BankerInfo * pBankerInfo=(CMD_S_BankerInfo *)pData;

	//����״̬
	m_pIAndroidUserItem->SetGameStatus(GAME_SCENE_MULTIPLE);

	//���ñ���
	m_wBankerUser=pBankerInfo->wBankerUser;
	m_cbHandCardCount[m_wBankerUser]+=CountArray(pBankerInfo->cbBankerCard);

	//�����˿�
	if (pBankerInfo->wBankerUser==m_pIAndroidUserItem->GetChairID())
	{
		//�����˿�
		CopyMemory(&m_cbHandCardData[NORMAL_COUNT],pBankerInfo->cbBankerCard,sizeof(pBankerInfo->cbBankerCard));

		//�����˿�
		WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
		m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount[wMeChairID],ST_ORDER);
	}
	//���õ���
	m_GameLogic.SetBackCard(pBankerInfo->wBankerUser, pBankerInfo->cbBankerCard, 3) ;

	//�������
	if (pBankerInfo->wBankerUser != m_pIAndroidUserItem->GetChairID())
	{
		UINT nElapse = rand() % TIME_OUT_CARD + TIME_LESS + TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_CALL_DOUBLE, nElapse);
	}
	m_GameLogic.SetBanker(pBankerInfo->wBankerUser) ;

	return true;
}

//ׯ�ҳ���
bool CAndroidUserItemSink::OnSubBankerOutCard(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize == sizeof(CMD_S_BankerOut));
	if (wDataSize != sizeof(CMD_S_BankerOut)) return false;

	m_pIAndroidUserItem->KillGameTimer(IDI_OUT_CARD);
	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_SCORE);
	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_DOUBLE);

	//��������
	CMD_S_BankerOut * pBankerInfo = (CMD_S_BankerOut *)pData;

	//����״̬
	m_pIAndroidUserItem->SetGameStatus(GAME_SCENE_PLAY);

	
	//�������
	if (pBankerInfo->wBankerUser == m_pIAndroidUserItem->GetChairID())
	{
		UINT nElapse = rand() % TIME_OUT_CARD + TIME_LESS + TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_OUT_CARD, nElapse);
	}

	return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubOutCard(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pData;
	WORD wHeadSize=sizeof(CMD_S_OutCard)-sizeof(pOutCard->cbCardData);

	//Ч������
	ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pOutCard->cbCardCount*sizeof(BYTE))));
	if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pOutCard->cbCardCount*sizeof(BYTE)))) return false;

	m_pIAndroidUserItem->KillGameTimer(IDI_OUT_CARD);
	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_SCORE);
	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_DOUBLE);

	//���Ʊ���
	if (pOutCard->wCurrentUser==pOutCard->wOutCardUser)
	{
		m_cbTurnCardCount=0;
		ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
	}
	else
	{
		m_cbTurnCardCount=pOutCard->cbCardCount;
		CopyMemory(m_cbTurnCardData,pOutCard->cbCardData,pOutCard->cbCardCount*sizeof(BYTE));
		m_wOutCardUser = pOutCard->wOutCardUser ;
	}

	m_wOutCardUser = pOutCard->wOutCardUser;

	//ɾ���˿�
	if (pOutCard->wOutCardUser == m_pIAndroidUserItem->GetChairID())
		m_GameLogic.RemoveCard(pOutCard->cbCardData, pOutCard->cbCardCount, m_cbHandCardData, m_cbHandCardCount[m_pIAndroidUserItem->GetChairID()]);

	//�˿���Ŀ
	m_cbHandCardCount[pOutCard->wOutCardUser] -= pOutCard->cbCardCount;

	//�������
	if (m_pIAndroidUserItem->GetChairID()==pOutCard->wCurrentUser)
	{
		UINT nElapse=rand()%TIME_OUT_CARD+TIME_LESS+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_OUT_CARD,nElapse);
	}

	//���ñ���
	m_GameLogic.RemoveUserCardData(pOutCard->wOutCardUser, pOutCard->cbCardData, pOutCard->cbCardCount) ;

	return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubPassCard(VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PassCard));
	if (wDataSize!=sizeof(CMD_S_PassCard)) return false;

	m_pIAndroidUserItem->KillGameTimer(IDI_OUT_CARD);
	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_SCORE);
	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_DOUBLE);

	//��������
	CMD_S_PassCard * pPassCard=(CMD_S_PassCard *)pData;

	//һ���ж�
	if (m_wOutCardUser == pPassCard->wCurrentUser)
	{
		m_cbTurnCardCount=0;
		ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
	}

	//�������
	if (m_pIAndroidUserItem->GetChairID()==pPassCard->wCurrentUser)
	{
		UINT nElapse=rand()%TIME_OUT_CARD+TIME_LESS+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_OUT_CARD,nElapse);
	}

	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameConclude));
	if (wDataSize!=sizeof(CMD_S_GameConclude)) return false;

	//��������
	CMD_S_GameConclude * pGameEnd=(CMD_S_GameConclude *)pData;

	//����״̬
	m_pIAndroidUserItem->SetGameStatus(GAME_SCENE_FREE);

	//���ñ���
	m_cbTurnCardCount=0;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

	//ɾ��ʱ��
	m_pIAndroidUserItem->KillGameTimer(IDI_OUT_CARD);
	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_SCORE);
	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_DOUBLE);

	//��ʼ����
	//UINT nElapse=rand()%TIME_START_GAME+TIME_LESS;
	//m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
	

	return true;
}

//////////////////////////////////////////////////////////////////////////
