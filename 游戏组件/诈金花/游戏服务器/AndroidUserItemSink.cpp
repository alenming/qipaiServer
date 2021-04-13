#include "Stdafx.h"
#include "AndroidUserItemSink.h"
#include "AESEncrypt.h"

//////////////////////////////////////////////////////////////////////////

//����ʱ��
#define TIME_LESS						2									//����ʱ��
#define TIME_DELAY_TIME					3									//��ʱʱ��

//��Ϸʱ��
#define TIME_START_GAME					4									//��ʼʱ��
#define TIME_USER_ADD_SCORE				4									//��עʱ��
#define TIME_USER_COMPARE_CARD			4									//����ʱ��
#define	TIME_USER_FINISH_FLASH			4									//����ʱ��

//��Ϸʱ��
#define IDI_START_GAME					(100)								//��ʼ��ʱ��
#define IDI_USER_ADD_SCORE				(101)								//��ע��ʱ��
#define IDI_LOOK_CARD					(102)								//���ƶ�ʱ��
#define IDI_DELAY_SHOWHAND				(103)								//�ӳ�ȫѹ
#define IDI_GIVE_UP						(104)								//������ʱ��
//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//��Ϸ����
	m_lStockScore = 0;
	m_wBankerUser = INVALID_CHAIR;
	m_wCurrentUser = INVALID_CHAIR;
	m_cbCardType = 0;

	m_bShowHandStatus = false;

	//��ע��Ϣ
	m_bMingZhu = false;
	m_lCellScore = 0;
	m_lMaxCellScore = 0;
	m_lUserMaxScore = 0;
	m_lCurrentTimes = 0;
	ZeroMemory(m_lTableScore, sizeof(m_lTableScore));

	//������
	m_nRobotBankStorageMul=0;
	m_lRobotBankGetScore=0;
	m_lRobotBankGetScoreBanker=0;
	ZeroMemory(m_lRobotScoreRange, sizeof(m_lRobotScoreRange));

	//��Ϸ״̬
	ZeroMemory(m_cbPlayStatus, sizeof(m_cbPlayStatus));
	ZeroMemory(m_cbRealPlayer, sizeof(m_cbRealPlayer));
	ZeroMemory(m_cbAndroidStatus, sizeof(m_cbAndroidStatus));

	//�û��˿�
	ZeroMemory(m_cbHandCardData, sizeof(m_cbHandCardData));
	ZeroMemory(m_cbAllHandCardData, sizeof(m_cbAllHandCardData));

	//�ӿڱ���
	m_pIAndroidUserItem=NULL;
	srand((unsigned)time(NULL));   
	

	m_nLookCard = 6;
	m_nSingleGiveup = 8;
	m_nShunziGiveup = 3;
	m_nLoserGiveup = 3;
	m_nLoserCompare = 7;
	m_nWinnerCompare = 3;
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
	//��Ϸ����
	m_lStockScore = 0;
	m_wBankerUser = INVALID_CHAIR;
	m_wCurrentUser = INVALID_CHAIR;
	m_cbCardType = 0;

	m_bShowHandStatus = false;

	//��ע��Ϣ
	m_bMingZhu = false;
	m_lMaxCellScore = 0;
	m_lUserMaxScore = 0;
	m_lCurrentTimes = 0;
	ZeroMemory(m_lTableScore, sizeof(m_lTableScore));

	m_cbGameTurns = 0;

	//��Ϸ״̬
	ZeroMemory(m_cbPlayStatus, sizeof(m_cbPlayStatus));
	ZeroMemory(m_cbRealPlayer, sizeof(m_cbRealPlayer));
	ZeroMemory(m_cbAndroidStatus, sizeof(m_cbAndroidStatus));

	//�û��˿�
	ZeroMemory(m_cbHandCardData, sizeof(m_cbHandCardData));
	ZeroMemory(m_cbAllHandCardData, sizeof(m_cbAllHandCardData));

	return true;
}

//ʱ����Ϣ
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:		//��ʼ��ʱ��
		{
			//����׼��
			m_pIAndroidUserItem->SendUserReady(NULL, 0);

			return true;
		}
	case IDI_LOOK_CARD:
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_LOOK_CARD);
			m_pIAndroidUserItem->SendSocketData(SUB_C_LOOK_CARD);
			return true;
		}
	case IDI_DELAY_SHOWHAND:
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_DELAY_SHOWHAND);
			WORD wMeChairID = m_pIAndroidUserItem->GetChairID();
			if ( ((wMeChairID == EstimateWinner() && m_cbCardType > 1) || m_cbCardType >= CT_SHUN_ZI) && m_cbGameTurns > 2)
			{
				m_pIAndroidUserItem->SendSocketData(SUB_C_PLAYER_ALLIN);
			}
			else
			{
				m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP);
			}
			return true;
		}
	case IDI_GIVE_UP:
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_GIVE_UP);
			m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP, NULL, 0);
			return true;
		}
	case IDI_USER_ADD_SCORE:	//��ע��ʱ��
		{
			//ɾ����ʱ��
			m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);

			//����Ч��
			if (m_pIAndroidUserItem->GetChairID() != m_wCurrentUser) return false;
			
			//�������
			BYTE cbProbability = 0;
			SCORE lCurrentScore = 0;
			SCORE lAndroidMaxScore = m_pIAndroidUserItem->GetMeUserItem()->GetUserScore();
			WORD wMeChairID = m_pIAndroidUserItem->GetChairID();

			//�������
			BYTE cbPlayerCount = 0;
			for (WORD i=0; i<GAME_PLAYER; i++)
			{
				if (m_cbPlayStatus[i] == PlayerPlaying || m_cbPlayStatus[i] == PlayerLookCard) 
					cbPlayerCount++;
			}

			//ȫѹ״̬
			if (m_bShowHandStatus)
			{
				if (!m_bMingZhu)
				{
					m_pIAndroidUserItem->SendSocketData(SUB_C_LOOK_CARD);
				}
				
				int nElaps = rand() % TIME_LESS + TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_DELAY_SHOWHAND, nElaps);
				return true;
			}

			//�жϻ������Ƿ�Ǯ��ס���ûǮ������
			SCORE lMinCompareScore = 10 * m_lCellScore * 4 * (cbPlayerCount - 1);
			lCurrentScore = m_lCurrentTimes * m_lCellScore;
			if (m_bMingZhu == true)
				lCurrentScore *= 2;
			if ((lAndroidMaxScore - m_lTableScore[wMeChairID] - lCurrentScore) <= lMinCompareScore && m_cbGameTurns > 2)
			{
				//��̫С�ͷ���
				if (m_cbCardType < CT_SHUN_ZI && EstimateWinner() != wMeChairID)
				{
					m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP, NULL, 0);
					return true;
				}

				if (cbPlayerCount == 2)
				{
					//ȫѹ
					m_pIAndroidUserItem->SendSocketData(SUB_C_PLAYER_ALLIN);
				}
				else
				{
					//������Ϣ
					CMD_C_CompareCard CompareCard;
					ZeroMemory(&CompareCard, sizeof(CompareCard));

					bool bFindCompare = false;
					WORD wRandPlayer = rand() % GAME_PLAYER;
					//�������
					for (WORD i = 0; i < GAME_PLAYER; i++)
					{
						WORD wCompareUser = (wRandPlayer + i) % GAME_PLAYER;
						if ((m_cbPlayStatus[wCompareUser] == PlayerPlaying || m_cbPlayStatus[wCompareUser] == PlayerLookCard) && wCompareUser != wMeChairID)
						{
							if (m_GameLogic.CompareCard(m_cbAllHandCardData[wCompareUser], m_cbAllHandCardData[wMeChairID], MAX_COUNT) || wMeChairID == EstimateWinner())
							{
								CompareCard.wCompareUser = wCompareUser;
								bFindCompare = true;
								break;
							}
						}
					}

					if (bFindCompare)
					{
						m_pIAndroidUserItem->SendSocketData(SUB_C_COMPARE_CARD, &CompareCard, sizeof(CompareCard));
					}
					else
					{
						SCORE lCurrentScore = m_lCurrentTimes*m_lCellScore;
						if (m_bMingZhu)
							lCurrentScore *= 2;
						//������Ϣ
						CMD_C_AddScore AddScore;
						AddScore.lScore = lCurrentScore;

						m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));
					}
				}
				return true;
			}

			//�״β���
			if (m_lTableScore[wMeChairID] == m_lCellScore)
			{
				//�������
				cbProbability = rand()%(10);

				BYTE cbCardType = m_GameLogic.GetCardType(m_cbAllHandCardData[wMeChairID],MAX_COUNT);
				int nRandIndex = rand() % 10;
				int nAddFlag = cbCardType * 2;
				if (cbCardType >= CT_DOUBLE && cbCardType != CT_SPECIAL && nRandIndex < nAddFlag)
				{
					//���ּ�ע
					ASSERT(m_lCurrentTimes <= 10 && m_lCurrentTimes >= 0);
					lCurrentScore = m_lCellScore * (GetAddScoreTimes());

					//������Ϣ
					CMD_C_AddScore AddScore;
					AddScore.lScore = lCurrentScore;

					m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));
				}
				//��ע
				else
				{
					//������Ϣ
					CMD_C_AddScore AddScore;
					AddScore.lScore = m_lCellScore*m_lCurrentTimes;

					m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));
				}
				return true;
			}
			else
			{
				//�Ƿ���
				if (m_bMingZhu == false)
				{
					//�������
					cbProbability = rand()%(10);

					//�û���ע
					if (m_lTableScore[wMeChairID] < 2 * m_lMaxCellScore && (4 * cbPlayerCount * m_lMaxCellScore + m_lCellScore + m_lTableScore[wMeChairID]) < lAndroidMaxScore)
					{
						//��ע����
						ASSERT(m_lCurrentTimes <= 10 && m_lCurrentTimes >= 0);
						lCurrentScore = m_lCellScore * GetAddScoreTimes();

						//������Ϣ
						CMD_C_AddScore AddScore;
						AddScore.lScore = lCurrentScore;

						m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));
						return true;
					}
					//��ע
					else
					{
						//������Ϣ
						CMD_C_AddScore AddScore;
						AddScore.lScore = m_lCurrentTimes*m_lCellScore;

						m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));
						return true;
					}
				}
				else
				{
					bool bMeLose = wMeChairID != EstimateWinner();
					int nWinCompareFlag = m_nWinnerCompare;
					if (bMeLose)
						nWinCompareFlag = m_nLoserCompare;

					//�û���һ����ı���
					if (bMeLose && m_cbCardType >= CT_DOUBLE && m_cbGameTurns > 2)
					{
						//������Ϣ
						CMD_C_CompareCard CompareCard;
						ZeroMemory(&CompareCard, sizeof(CompareCard));

						bool bFindCompare = false;
						WORD wRandPlayer = rand() % GAME_PLAYER;
						//�������
						for (WORD i = 0; i < GAME_PLAYER; i++)
						{
							WORD wCompareUser = (wRandPlayer + i) % GAME_PLAYER;
							if ((m_cbPlayStatus[wCompareUser] == PlayerPlaying || m_cbPlayStatus[wCompareUser] == PlayerLookCard) && wCompareUser != wMeChairID)
							{
								if (m_GameLogic.CompareCard(m_cbAllHandCardData[wCompareUser], m_cbAllHandCardData[wMeChairID], MAX_COUNT))
								{
									CompareCard.wCompareUser = wCompareUser;
									bFindCompare = true;
									break;
								}
							}
						}

						if (bFindCompare)
						{
							m_pIAndroidUserItem->SendSocketData(SUB_C_COMPARE_CARD, &CompareCard, sizeof(CompareCard));
						}
						else
						{
							SCORE lCurrentScore = m_lCurrentTimes*m_lCellScore;
							if (m_bMingZhu)
								lCurrentScore *= 2;
							//������Ϣ
							CMD_C_AddScore AddScore;
							AddScore.lScore = lCurrentScore;

							m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));
						}
						
						return true;
					}
					//�����һ���˱���
					else if (m_cbCardType >= CT_DOUBLE && cbProbability < nWinCompareFlag && m_cbGameTurns > 2)
					{
						//������Ϣ
						CMD_C_CompareCard CompareCard;
						ZeroMemory(&CompareCard, sizeof(CompareCard));

						bool bFindCompare = false;
						WORD wRandPlayer = rand() % GAME_PLAYER;
						//�������
						for (WORD i = 0; i < GAME_PLAYER; i++)
						{
							WORD wCompareUser = (wRandPlayer + i) % GAME_PLAYER;
							if ((m_cbPlayStatus[wCompareUser] == PlayerPlaying || m_cbPlayStatus[wCompareUser] == PlayerLookCard) && wCompareUser != wMeChairID)
							{
								CompareCard.wCompareUser = wCompareUser;
								bFindCompare = true;
								break;
							}
						}

						if (bFindCompare)
						{
							m_pIAndroidUserItem->SendSocketData(SUB_C_COMPARE_CARD, &CompareCard, sizeof(CompareCard));
						}
						else
						{
							SCORE lCurrentScore = m_lCurrentTimes*m_lCellScore;
							if (m_bMingZhu)
								lCurrentScore *= 2;
							//������Ϣ
							CMD_C_AddScore AddScore;
							AddScore.lScore = lCurrentScore;

							m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));
						}

						return true;
					}
					//��ע����
					else if (m_cbCardType < CT_JIN_HUA && m_lTableScore[wMeChairID] < (1 * 2 * m_lMaxCellScore * m_cbCardType)
						|| m_cbCardType == CT_JIN_HUA && m_lTableScore[wMeChairID] < (3 * 2 * m_lMaxCellScore * m_cbCardType)
						|| m_cbCardType > CT_JIN_HUA && m_lTableScore[wMeChairID] < (5 * 2 * m_lMaxCellScore * m_cbCardType)
						&& (4 * cbPlayerCount * m_lMaxCellScore + m_lCellScore + m_lTableScore[wMeChairID]) < lAndroidMaxScore)
					{
						//��ע����
						ASSERT(m_lCurrentTimes <= 10 && m_lCurrentTimes >= 0);
						lCurrentScore = m_lCellScore * GetAddScoreTimes();

						//��ע�ӱ�
						if (m_bMingZhu == true) lCurrentScore *= 2;

						//������Ϣ
						CMD_C_AddScore AddScore;
						AddScore.lScore = lCurrentScore;

						m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));
						return true;
					}
					//��ע
					else
					{
						SCORE lCurrentScore = m_lCurrentTimes*m_lCellScore;
						if (m_bMingZhu)
							lCurrentScore *= 2;
						//������Ϣ
						CMD_C_AddScore AddScore;
						AddScore.lScore = lCurrentScore;

						m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));
					}
				}
			}

			return false;
		}
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
			return OnSubGameStart(pData, wDataSize);
		}
	case SUB_S_ADD_SCORE:	//�û���ע
		{
			//��Ϣ����
			return OnSubAddScore(pData, wDataSize);
		}
	case SUB_S_SHOW_HAND:	//�û�ȫѹ
		{
			return OnSubShowHand(pData, wDataSize);
		}
	case SUB_S_GIVE_UP:		//�û�����
		{
			//��Ϣ����
			return OnSubGiveUp(pData, wDataSize);
		}
	case SUB_S_GAME_END:	//��Ϸ����
		{
			//��Ϣ����
			return OnSubGameEnd(pData, wDataSize);
		}
	case SUB_S_COMPARE_CARD://�û�����
		{
			//��Ϣ����
			return OnSubCompareCard(pData, wDataSize);
		}
	case SUB_S_LOOK_CARD:	//�û�����
		{
			//��Ϣ����
			return OnSubLookCard(pData, wDataSize);
		}
	case SUB_S_OPEN_CARD:	//�û�̯��
		{
			//��Ϣ����
			return OnSubOpenCard(pData, wDataSize);
		}
	case SUB_S_ANDROID_CARD:
		{
			return OnSubAndroidCard(pData, wDataSize);
		}
	case SUB_S_ANDROID_CONFIG:
		{
			//��Ϣ����
			if (wDataSize != sizeof(tagCustomAndroid)) return false;
			tagCustomAndroid * pCustomAndroid = (tagCustomAndroid *)pData;
			//��ȡ����
			ReadConfigInformation(pCustomAndroid);
			return true;
		}
	case SUB_S_PLAYER_BET_FALURE:
		{
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
	case GAME_STATUS_FREE:		//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��Ϣ����
			CMD_S_StatusFree * pStatusFree = (CMD_S_StatusFree *)pData;

			m_lCellScore = pStatusFree->lCellScore;

			//���в���
			BankOperate(2);

			//���ö�ʱ��
			//UINT nElapse=TIME_LESS + (rand() % TIME_START_GAME);
			//m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME, nElapse);

			return true;
		}
	case GAME_STATUS_PLAY:		//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay = (CMD_S_StatusPlay *)pData;

			m_lCellScore = pStatusPlay->lCellScore;

			//���в���
			BankOperate(2);

			//�û���Ϣ
			m_wBankerUser = pStatusPlay->wBankerUser;
			m_wCurrentUser = pStatusPlay->wCurrentUser;

			m_lCurrentTimes = pStatusPlay->lMinScore;

			//��ע��Ϣ
			m_lUserMaxScore = pStatusPlay->lPlayerScore;

			//��Ϸ״̬
			CopyMemory(m_cbPlayStatus, pStatusPlay->cbPlayStatus, sizeof(m_cbPlayStatus));

			m_bMingZhu = m_cbPlayStatus[m_pIAndroidUserItem->GetChairID()] == PlayerLookCard;

			//�û��˿�
			CopyMemory(m_cbHandCardData, pStatusPlay->cbHandCardData, sizeof(m_cbHandCardData));

			//�ж��û�
			if (m_pIAndroidUserItem->GetChairID() == m_wCurrentUser)
			{
				//���ö�ʱ��
				UINT nElapse = TIME_LESS + 1 + (rand() % TIME_USER_ADD_SCORE);
				m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE, nElapse);
			}

			return true;
		}
	}

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

//��������Ϣ
bool CAndroidUserItemSink::OnSubAndroidCard(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize == sizeof(CMD_S_AndroidCard));
	if (wDataSize != sizeof(CMD_S_AndroidCard)) return true;

	//��������
	CMD_S_AndroidCard * pAndroidCard = (CMD_S_AndroidCard *)pBuffer;

	//���ñ���
	//m_lStockScore = pAndroidCard->lStockScore;
	CopyMemory(m_cbRealPlayer, pAndroidCard->cbRealPlayer, sizeof(m_cbRealPlayer));
	CopyMemory(m_cbAllHandCardData, pAndroidCard->cbAllHandCardData, sizeof(m_cbAllHandCardData));
	CopyMemory(m_cbAndroidStatus, pAndroidCard->cbAndroidStatus, sizeof(m_cbAndroidStatus));
	CopyMemory(m_cbPlayStatus, pAndroidCard->cbPlayStatus, sizeof(m_cbPlayStatus));

	//�û�״̬
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		//��ȡ�û�
		if (m_cbPlayStatus[i] != PlayerNoPlay)
		{
			m_lTableScore[i] = m_lCellScore;
		}
	}

	return true;
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize != sizeof(CMD_S_GameStart)) return true;

	//��������
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//�û���Ϣ
	m_wBankerUser = pGameStart->wBankerUser;
	m_wCurrentUser = pGameStart->wCurrentUser;

	m_cbGameTurns = 1;
	m_bShowHandStatus = false;

	//��ע��Ϣ
	m_bMingZhu = false;
	m_lMaxCellScore = pGameStart->lMaxScore;
	m_lUserMaxScore = pGameStart->lUserMaxScore;
	m_lCurrentTimes = pGameStart->lMinScore;

	//��Ҵ���
	ASSERT(m_pIAndroidUserItem != NULL);
	if (m_pIAndroidUserItem->GetChairID()==pGameStart->wCurrentUser)
	{
		//���ö�ʱ��
		UINT nElapse = TIME_LESS + 1 + TIME_LESS + (rand() % TIME_USER_ADD_SCORE);
		m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE, nElapse);
	}

	return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubGiveUp(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize == sizeof(CMD_S_GiveUp));
	if (wDataSize != sizeof(CMD_S_GiveUp)) return true;

	//������Ϣ
	CMD_S_GiveUp * pGiveUp = (CMD_S_GiveUp *)pBuffer;

	//���ñ���
	m_cbRealPlayer[pGiveUp->wGiveUpUser] = FALSE;
	m_cbPlayStatus[pGiveUp->wGiveUpUser] = PlayerGiveUp;
	//m_cbAndroidStatus[pGiveUp->wGiveUpUser] = FALSE;

	if (pGiveUp->wCurrentUser != INVALID_CHAIR)
	{
		m_wCurrentUser = pGiveUp->wCurrentUser;
	}

	//�������
	BYTE cbUserCount = 0;
	for (WORD i = 0; i<GAME_PLAYER; i++)
	{
		if (m_cbPlayStatus[i] == PlayerPlaying || m_cbPlayStatus[i] == PlayerLookCard) cbUserCount++;
	}

	if (cbUserCount > 1)
	{
		if (pGiveUp->wCurrentUser == m_pIAndroidUserItem->GetChairID())
		{
			//���ö�ʱ��
			UINT nElapse = TIME_LESS + 1 + (rand() % TIME_USER_ADD_SCORE);
			m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE, nElapse + TIME_DELAY_TIME);
		}
	}

	return true;
}

//�û���ע
bool CAndroidUserItemSink::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize == sizeof(CMD_S_AddScore));
	if (wDataSize != sizeof(CMD_S_AddScore)) return true;

	//������Ϣ
	CMD_S_AddScore * pAddScore = (CMD_S_AddScore *)pBuffer;

	m_cbGameTurns = pAddScore->cbGameTurns;

	m_lCurrentTimes = pAddScore->lMinScore;

	//���ñ���
	m_wCurrentUser = pAddScore->wCurrentUser;
	m_lTableScore[pAddScore->wAddScoreUser] += pAddScore->lAddScoreCount;
	WORD wMeChairID = m_pIAndroidUserItem->GetChairID();

	//�û�����
	if (wMeChairID==m_wCurrentUser)
	{
		//���ö�ʱ��
		UINT nElapse=TIME_LESS + 1 + (rand() % TIME_USER_ADD_SCORE);
		m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE, nElapse);
	}

	//���һ�����ʿ���
	if (m_cbGameTurns > 1 && !m_bMingZhu)
	{
		//�������
		int cbProbability = rand() % (10);

		BYTE cbCardType = m_GameLogic.GetCardType(m_cbAllHandCardData[wMeChairID], MAX_COUNT);

		if (cbCardType == CT_SPECIAL)
			cbCardType = CT_SINGLE;

		int nLookFlag = m_nLookCard - (cbCardType - CT_SINGLE);

		SCORE lHaveScore = m_lUserMaxScore - m_lTableScore[m_pIAndroidUserItem->GetChairID()];

		//���Ƹ��� ���Ǯ̫�ٻ��߽��뿴�Ƽ���
		if (cbProbability < nLookFlag || lHaveScore < 4*20*m_lCellScore)
		{
			UINT nElapse = TIME_LESS + 1 + (rand() % TIME_USER_ADD_SCORE);
			m_pIAndroidUserItem->SetGameTimer(IDI_LOOK_CARD, nElapse);
			return true;
		}
	}

	//���һ�����ʷ���
	if (m_bMingZhu && pAddScore->wAddScoreUser != wMeChairID)
	{
		//�������
		int cbProbability = rand() % (10);

		//ɢ�Ʒ���
		if (m_cbCardType == CT_SINGLE)
		{
			//��������
			if (cbProbability < m_nSingleGiveup && wMeChairID != EstimateWinner())
			{
				m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);
				UINT nElapse = TIME_LESS + (rand() % TIME_USER_ADD_SCORE);
				m_pIAndroidUserItem->SetGameTimer(IDI_GIVE_UP, nElapse);
				return true;
			}
		}
		else if (m_cbCardType < CT_SHUN_ZI && m_cbGameTurns > 5 && wMeChairID != EstimateWinner())
		{
			if (cbProbability < m_nShunziGiveup)
			{
				m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);
				UINT nElapse = TIME_LESS + (rand() % TIME_USER_ADD_SCORE);
				m_pIAndroidUserItem->SetGameTimer(IDI_GIVE_UP, nElapse);
				return true;
			}
		}
		else if (wMeChairID != EstimateWinner())
		{
			if (cbProbability < m_nLoserGiveup)
			{
				m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);
				UINT nElapse = TIME_LESS + (rand() % TIME_USER_ADD_SCORE);
				m_pIAndroidUserItem->SetGameTimer(IDI_GIVE_UP, nElapse);
				return true;
			}
		}
	}

	return true;
}

//�û�ȫѹ
bool CAndroidUserItemSink::OnSubShowHand(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize == sizeof(CMD_S_AddScore));
	if (wDataSize != sizeof(CMD_S_AddScore)) return true;

	//������Ϣ
	CMD_S_AddScore * pAddScore = (CMD_S_AddScore *)pBuffer;

	m_cbGameTurns = pAddScore->cbGameTurns;

	m_bShowHandStatus = pAddScore->cbShowHandStatus;

	m_lCurrentTimes = pAddScore->lMinScore;

	//���ñ���
	m_wCurrentUser = pAddScore->wCurrentUser;
	m_lTableScore[pAddScore->wAddScoreUser] += pAddScore->lAddScoreCount;
	WORD wMeChairID = m_pIAndroidUserItem->GetChairID();

	//�û�����
	if (wMeChairID == m_wCurrentUser)
	{
		//���ö�ʱ��
		UINT nElapse = TIME_LESS + 1 + (rand() % TIME_USER_ADD_SCORE);
		m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE, nElapse);
	}
	return true;
}
//�û�����
bool CAndroidUserItemSink::OnSubLookCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize == sizeof(CMD_S_LookCard));
	if (wDataSize != sizeof(CMD_S_LookCard)) return true;

	//������Ϣ
	CMD_S_LookCard * pLookCard = (CMD_S_LookCard *)pBuffer;

	m_cbPlayStatus[pLookCard->wLookCardUser] = PlayerLookCard;

	//�����û�
	if (pLookCard->wLookCardUser == m_pIAndroidUserItem->GetChairID())
	{
		m_bMingZhu = true;
		//�û��˿�
		CopyMemory(m_cbHandCardData, pLookCard->cbCardData, sizeof(m_cbHandCardData));

		//��ȡ����
		m_GameLogic.SortCardList(m_cbHandCardData, MAX_COUNT);
		m_cbCardType = m_GameLogic.GetCardType(m_cbHandCardData, MAX_COUNT);
	}

	return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubCompareCard(const void * pBuffer, WORD wDataSize)
{
	//����Ч��
	ASSERT(wDataSize == sizeof(CMD_S_CompareCard));
	if (wDataSize != sizeof(CMD_S_CompareCard)) return true;

	//������Ϣ
	CMD_S_CompareCard * pCompareCard = (CMD_S_CompareCard *)pBuffer;

	//���ñ���
	if (pCompareCard->wCurrentUser != INVALID_CHAIR)
	{
		m_wCurrentUser = pCompareCard->wCurrentUser;
	}

	m_cbPlayStatus[pCompareCard->wCompareUser[1]] = PlayerComFail;

	//�������
	BYTE cbUserCount = 0;
	for (WORD i=0; i<GAME_PLAYER; i++)
	{
		if (m_cbPlayStatus[i] == PlayerPlaying || m_cbPlayStatus[i] == PlayerLookCard) cbUserCount++;
	}

	m_lTableScore[pCompareCard->wCompareUser[0]] += pCompareCard->lCompareScore;

	//�жϽ���
	if (cbUserCount > 1)
	{
		//��ǰ�û�
		if (m_pIAndroidUserItem->GetChairID() == m_wCurrentUser)
		{
			//���ö�ʱ��
			UINT nElapse=TIME_LESS + 1 + (rand() % TIME_USER_ADD_SCORE);
			m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE, nElapse+TIME_DELAY_TIME);
		}
	}

	return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
	//����Ч��
	ASSERT(wDataSize == sizeof(CMD_S_OpenCard));
	if (wDataSize != sizeof(CMD_S_OpenCard)) return true;

	return true;
}

//�û�ǿ��
bool CAndroidUserItemSink::OnSubPlayerExit(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	//ASSERT(wDataSize == sizeof(CMD_S_PlayerExit));
	//if (wDataSize != sizeof(CMD_S_PlayerExit)) return false;

	////������Ϣ
	//CMD_S_PlayerExit * pPlayerExit=(CMD_S_PlayerExit *)pBuffer;

	////���ñ���
	//m_cbPlayStatus[pPlayerExit->wPlayerID] = FALSE;

	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	if (wDataSize != sizeof(CMD_S_GameEnd)) return true;
	CMD_S_GameEnd * pGameEnd = (CMD_S_GameEnd *)pBuffer;

	//ɾ����ʱ��
	m_pIAndroidUserItem->KillGameTimer(IDI_START_GAME);
	m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);
	m_pIAndroidUserItem->KillGameTimer(IDI_LOOK_CARD);
	m_pIAndroidUserItem->KillGameTimer(IDI_DELAY_SHOWHAND);
	m_pIAndroidUserItem->KillGameTimer(IDI_GIVE_UP);
	

	//�������
	m_lMaxCellScore = 0;
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

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

	//��������
	SCORE lRobotScore = pUserItem->GetUserScore();

	{
		

		//�жϴ�ȡ
		if (lRobotScore > m_lRobotScoreRange[1])
		{			
			SCORE lSaveScore = 0;

			lSaveScore = SCORE(lRobotScore*m_nRobotBankStorageMul / 100);
			if (lSaveScore > lRobotScore) lSaveScore = lRobotScore;
			if (lSaveScore > 0) m_pIAndroidUserItem->PerformSaveScore(lSaveScore);
		}
		else if (lRobotScore < m_lRobotScoreRange[0])
		{

			SCORE lScore=rand()%m_lRobotBankGetScoreBanker+m_lRobotBankGetScore;
			if (lScore > 0)
				m_pIAndroidUserItem->PerformTakeScore(lScore);

		}
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

	m_nLookCard = pCustomAndroid->nLookCard;
	m_nSingleGiveup = pCustomAndroid->nSingleGiveup;
	m_nShunziGiveup = pCustomAndroid->nShunziGiveup;
	m_nLoserGiveup = pCustomAndroid->nLoserGiveup;
	m_nLoserCompare = pCustomAndroid->nLoserCompare;
	m_nWinnerCompare = pCustomAndroid->nWinnerCompare;
}

//�ƶ�ʤ��
WORD CAndroidUserItemSink::EstimateWinner()
{
	//�����˿�
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData, m_cbAllHandCardData, sizeof(cbUserCardData));

	//�����˿�
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		m_GameLogic.SortCardList(cbUserCardData[i], MAX_COUNT);
	}

	//��������
	WORD wWinUser = INVALID_CHAIR;

	//��������
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		//�û�����
		if (m_cbPlayStatus[i] == PlayerNoPlay || m_cbPlayStatus[i] == PlayerGiveUp || PlayerGiveUp == PlayerComFail) continue;

		//�����û�
		if (wWinUser == INVALID_CHAIR)
		{
			wWinUser=i;
			continue;
		}

		//�Ա��˿�
		if (m_GameLogic.CompareCard(cbUserCardData[i], cbUserCardData[wWinUser], MAX_COUNT) >= TRUE)
		{
			wWinUser = i;
		}
	}

	return wWinUser;
}

//��ע����
LONGLONG CAndroidUserItemSink::GetAddScoreTimes()
{
	LONGLONG lTimesSelect[3] = {2,5,10};
	LONGLONG lTimes = m_lCurrentTimes;
	if(lTimes < lTimesSelect[0])
		lTimes = lTimesSelect[rand()%3];
	else if(lTimes < lTimesSelect[1])
		lTimes = lTimesSelect[rand()%2+1];
	else if (lTimes < lTimesSelect[2])
		lTimes = lTimesSelect[2];

	return lTimes;
}
//�����������
DECLARE_CREATE_MODULE(AndroidUserItemSink);

//////////////////////////////////////////////////////////////////////////
