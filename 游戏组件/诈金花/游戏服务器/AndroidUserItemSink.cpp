#include "Stdafx.h"
#include "AndroidUserItemSink.h"
#include "AESEncrypt.h"

//////////////////////////////////////////////////////////////////////////

//辅助时间
#define TIME_LESS						2									//最少时间
#define TIME_DELAY_TIME					3									//延时时间

//游戏时间
#define TIME_START_GAME					4									//开始时间
#define TIME_USER_ADD_SCORE				4									//下注时间
#define TIME_USER_COMPARE_CARD			4									//比牌时间
#define	TIME_USER_FINISH_FLASH			4									//比牌时间

//游戏时间
#define IDI_START_GAME					(100)								//开始定时器
#define IDI_USER_ADD_SCORE				(101)								//下注定时器
#define IDI_LOOK_CARD					(102)								//看牌定时器
#define IDI_DELAY_SHOWHAND				(103)								//延迟全压
#define IDI_GIVE_UP						(104)								//放弃定时器
//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//游戏变量
	m_lStockScore = 0;
	m_wBankerUser = INVALID_CHAIR;
	m_wCurrentUser = INVALID_CHAIR;
	m_cbCardType = 0;

	m_bShowHandStatus = false;

	//加注信息
	m_bMingZhu = false;
	m_lCellScore = 0;
	m_lMaxCellScore = 0;
	m_lUserMaxScore = 0;
	m_lCurrentTimes = 0;
	ZeroMemory(m_lTableScore, sizeof(m_lTableScore));

	//库存操作
	m_nRobotBankStorageMul=0;
	m_lRobotBankGetScore=0;
	m_lRobotBankGetScoreBanker=0;
	ZeroMemory(m_lRobotScoreRange, sizeof(m_lRobotScoreRange));

	//游戏状态
	ZeroMemory(m_cbPlayStatus, sizeof(m_cbPlayStatus));
	ZeroMemory(m_cbRealPlayer, sizeof(m_cbRealPlayer));
	ZeroMemory(m_cbAndroidStatus, sizeof(m_cbAndroidStatus));

	//用户扑克
	ZeroMemory(m_cbHandCardData, sizeof(m_cbHandCardData));
	ZeroMemory(m_cbAllHandCardData, sizeof(m_cbAllHandCardData));

	//接口变量
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

//析构函数
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//接口查询
void * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//初始接口
bool CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}

//重置接口
bool CAndroidUserItemSink::RepositionSink()
{
	//游戏变量
	m_lStockScore = 0;
	m_wBankerUser = INVALID_CHAIR;
	m_wCurrentUser = INVALID_CHAIR;
	m_cbCardType = 0;

	m_bShowHandStatus = false;

	//加注信息
	m_bMingZhu = false;
	m_lMaxCellScore = 0;
	m_lUserMaxScore = 0;
	m_lCurrentTimes = 0;
	ZeroMemory(m_lTableScore, sizeof(m_lTableScore));

	m_cbGameTurns = 0;

	//游戏状态
	ZeroMemory(m_cbPlayStatus, sizeof(m_cbPlayStatus));
	ZeroMemory(m_cbRealPlayer, sizeof(m_cbRealPlayer));
	ZeroMemory(m_cbAndroidStatus, sizeof(m_cbAndroidStatus));

	//用户扑克
	ZeroMemory(m_cbHandCardData, sizeof(m_cbHandCardData));
	ZeroMemory(m_cbAllHandCardData, sizeof(m_cbAllHandCardData));

	return true;
}

//时间消息
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:		//开始定时器
		{
			//发送准备
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
	case IDI_USER_ADD_SCORE:	//加注定时器
		{
			//删除定时器
			m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);

			//数据效验
			if (m_pIAndroidUserItem->GetChairID() != m_wCurrentUser) return false;
			
			//定义变量
			BYTE cbProbability = 0;
			SCORE lCurrentScore = 0;
			SCORE lAndroidMaxScore = m_pIAndroidUserItem->GetMeUserItem()->GetUserScore();
			WORD wMeChairID = m_pIAndroidUserItem->GetChairID();

			//玩家人数
			BYTE cbPlayerCount = 0;
			for (WORD i=0; i<GAME_PLAYER; i++)
			{
				if (m_cbPlayStatus[i] == PlayerPlaying || m_cbPlayStatus[i] == PlayerLookCard) 
					cbPlayerCount++;
			}

			//全压状态
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

			//判断机器人是否钱跟住后就没钱比牌了
			SCORE lMinCompareScore = 10 * m_lCellScore * 4 * (cbPlayerCount - 1);
			lCurrentScore = m_lCurrentTimes * m_lCellScore;
			if (m_bMingZhu == true)
				lCurrentScore *= 2;
			if ((lAndroidMaxScore - m_lTableScore[wMeChairID] - lCurrentScore) <= lMinCompareScore && m_cbGameTurns > 2)
			{
				//牌太小就放弃
				if (m_cbCardType < CT_SHUN_ZI && EstimateWinner() != wMeChairID)
				{
					m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP, NULL, 0);
					return true;
				}

				if (cbPlayerCount == 2)
				{
					//全压
					m_pIAndroidUserItem->SendSocketData(SUB_C_PLAYER_ALLIN);
				}
				else
				{
					//构造消息
					CMD_C_CompareCard CompareCard;
					ZeroMemory(&CompareCard, sizeof(CompareCard));

					bool bFindCompare = false;
					WORD wRandPlayer = rand() % GAME_PLAYER;
					//查找玩家
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
						//发送消息
						CMD_C_AddScore AddScore;
						AddScore.lScore = lCurrentScore;

						m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));
					}
				}
				return true;
			}

			//首次操作
			if (m_lTableScore[wMeChairID] == m_lCellScore)
			{
				//随机概率
				cbProbability = rand()%(10);

				BYTE cbCardType = m_GameLogic.GetCardType(m_cbAllHandCardData[wMeChairID],MAX_COUNT);
				int nRandIndex = rand() % 10;
				int nAddFlag = cbCardType * 2;
				if (cbCardType >= CT_DOUBLE && cbCardType != CT_SPECIAL && nRandIndex < nAddFlag)
				{
					//首轮加注
					ASSERT(m_lCurrentTimes <= 10 && m_lCurrentTimes >= 0);
					lCurrentScore = m_lCellScore * (GetAddScoreTimes());

					//发送消息
					CMD_C_AddScore AddScore;
					AddScore.lScore = lCurrentScore;

					m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));
				}
				//跟注
				else
				{
					//发送消息
					CMD_C_AddScore AddScore;
					AddScore.lScore = m_lCellScore*m_lCurrentTimes;

					m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));
				}
				return true;
			}
			else
			{
				//是否看牌
				if (m_bMingZhu == false)
				{
					//随机概率
					cbProbability = rand()%(10);

					//用户加注
					if (m_lTableScore[wMeChairID] < 2 * m_lMaxCellScore && (4 * cbPlayerCount * m_lMaxCellScore + m_lCellScore + m_lTableScore[wMeChairID]) < lAndroidMaxScore)
					{
						//加注积分
						ASSERT(m_lCurrentTimes <= 10 && m_lCurrentTimes >= 0);
						lCurrentScore = m_lCellScore * GetAddScoreTimes();

						//发送消息
						CMD_C_AddScore AddScore;
						AddScore.lScore = lCurrentScore;

						m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));
						return true;
					}
					//跟注
					else
					{
						//发送消息
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

					//用户找一个输的比牌
					if (bMeLose && m_cbCardType >= CT_DOUBLE && m_cbGameTurns > 2)
					{
						//构造消息
						CMD_C_CompareCard CompareCard;
						ZeroMemory(&CompareCard, sizeof(CompareCard));

						bool bFindCompare = false;
						WORD wRandPlayer = rand() % GAME_PLAYER;
						//查找玩家
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
							//发送消息
							CMD_C_AddScore AddScore;
							AddScore.lScore = lCurrentScore;

							m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));
						}
						
						return true;
					}
					//随机找一个人比牌
					else if (m_cbCardType >= CT_DOUBLE && cbProbability < nWinCompareFlag && m_cbGameTurns > 2)
					{
						//构造消息
						CMD_C_CompareCard CompareCard;
						ZeroMemory(&CompareCard, sizeof(CompareCard));

						bool bFindCompare = false;
						WORD wRandPlayer = rand() % GAME_PLAYER;
						//查找玩家
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
							//发送消息
							CMD_C_AddScore AddScore;
							AddScore.lScore = lCurrentScore;

							m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));
						}

						return true;
					}
					//加注概率
					else if (m_cbCardType < CT_JIN_HUA && m_lTableScore[wMeChairID] < (1 * 2 * m_lMaxCellScore * m_cbCardType)
						|| m_cbCardType == CT_JIN_HUA && m_lTableScore[wMeChairID] < (3 * 2 * m_lMaxCellScore * m_cbCardType)
						|| m_cbCardType > CT_JIN_HUA && m_lTableScore[wMeChairID] < (5 * 2 * m_lMaxCellScore * m_cbCardType)
						&& (4 * cbPlayerCount * m_lMaxCellScore + m_lCellScore + m_lTableScore[wMeChairID]) < lAndroidMaxScore)
					{
						//下注积分
						ASSERT(m_lCurrentTimes <= 10 && m_lCurrentTimes >= 0);
						lCurrentScore = m_lCellScore * GetAddScoreTimes();

						//明注加倍
						if (m_bMingZhu == true) lCurrentScore *= 2;

						//发送消息
						CMD_C_AddScore AddScore;
						AddScore.lScore = lCurrentScore;

						m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));
						return true;
					}
					//跟注
					else
					{
						SCORE lCurrentScore = m_lCurrentTimes*m_lCellScore;
						if (m_bMingZhu)
							lCurrentScore *= 2;
						//发送消息
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

//游戏消息
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:	//游戏开始
		{
			//消息处理
			return OnSubGameStart(pData, wDataSize);
		}
	case SUB_S_ADD_SCORE:	//用户下注
		{
			//消息处理
			return OnSubAddScore(pData, wDataSize);
		}
	case SUB_S_SHOW_HAND:	//用户全压
		{
			return OnSubShowHand(pData, wDataSize);
		}
	case SUB_S_GIVE_UP:		//用户放弃
		{
			//消息处理
			return OnSubGiveUp(pData, wDataSize);
		}
	case SUB_S_GAME_END:	//游戏结束
		{
			//消息处理
			return OnSubGameEnd(pData, wDataSize);
		}
	case SUB_S_COMPARE_CARD://用户比牌
		{
			//消息处理
			return OnSubCompareCard(pData, wDataSize);
		}
	case SUB_S_LOOK_CARD:	//用户看牌
		{
			//消息处理
			return OnSubLookCard(pData, wDataSize);
		}
	case SUB_S_OPEN_CARD:	//用户摊牌
		{
			//消息处理
			return OnSubOpenCard(pData, wDataSize);
		}
	case SUB_S_ANDROID_CARD:
		{
			return OnSubAndroidCard(pData, wDataSize);
		}
	case SUB_S_ANDROID_CONFIG:
		{
			//消息处理
			if (wDataSize != sizeof(tagCustomAndroid)) return false;
			tagCustomAndroid * pCustomAndroid = (tagCustomAndroid *)pData;
			//读取配置
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

//游戏消息
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	return true;
}

//场景消息
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:		//空闲状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//消息处理
			CMD_S_StatusFree * pStatusFree = (CMD_S_StatusFree *)pData;

			m_lCellScore = pStatusFree->lCellScore;

			//银行操作
			BankOperate(2);

			//设置定时器
			//UINT nElapse=TIME_LESS + (rand() % TIME_START_GAME);
			//m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME, nElapse);

			return true;
		}
	case GAME_STATUS_PLAY:		//游戏状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay = (CMD_S_StatusPlay *)pData;

			m_lCellScore = pStatusPlay->lCellScore;

			//银行操作
			BankOperate(2);

			//用户信息
			m_wBankerUser = pStatusPlay->wBankerUser;
			m_wCurrentUser = pStatusPlay->wCurrentUser;

			m_lCurrentTimes = pStatusPlay->lMinScore;

			//加注信息
			m_lUserMaxScore = pStatusPlay->lPlayerScore;

			//游戏状态
			CopyMemory(m_cbPlayStatus, pStatusPlay->cbPlayStatus, sizeof(m_cbPlayStatus));

			m_bMingZhu = m_cbPlayStatus[m_pIAndroidUserItem->GetChairID()] == PlayerLookCard;

			//用户扑克
			CopyMemory(m_cbHandCardData, pStatusPlay->cbHandCardData, sizeof(m_cbHandCardData));

			//判断用户
			if (m_pIAndroidUserItem->GetChairID() == m_wCurrentUser)
			{
				//设置定时器
				UINT nElapse = TIME_LESS + 1 + (rand() % TIME_USER_ADD_SCORE);
				m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE, nElapse);
			}

			return true;
		}
	}

	return true;
}

//用户进入
void CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户离开
void CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户积分
void CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户状态
void CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户段位
void CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//机器人消息
bool CAndroidUserItemSink::OnSubAndroidCard(const void * pBuffer, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize == sizeof(CMD_S_AndroidCard));
	if (wDataSize != sizeof(CMD_S_AndroidCard)) return true;

	//变量定义
	CMD_S_AndroidCard * pAndroidCard = (CMD_S_AndroidCard *)pBuffer;

	//设置变量
	//m_lStockScore = pAndroidCard->lStockScore;
	CopyMemory(m_cbRealPlayer, pAndroidCard->cbRealPlayer, sizeof(m_cbRealPlayer));
	CopyMemory(m_cbAllHandCardData, pAndroidCard->cbAllHandCardData, sizeof(m_cbAllHandCardData));
	CopyMemory(m_cbAndroidStatus, pAndroidCard->cbAndroidStatus, sizeof(m_cbAndroidStatus));
	CopyMemory(m_cbPlayStatus, pAndroidCard->cbPlayStatus, sizeof(m_cbPlayStatus));

	//用户状态
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		//获取用户
		if (m_cbPlayStatus[i] != PlayerNoPlay)
		{
			m_lTableScore[i] = m_lCellScore;
		}
	}

	return true;
}

//游戏开始
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize != sizeof(CMD_S_GameStart)) return true;

	//变量定义
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//用户信息
	m_wBankerUser = pGameStart->wBankerUser;
	m_wCurrentUser = pGameStart->wCurrentUser;

	m_cbGameTurns = 1;
	m_bShowHandStatus = false;

	//加注信息
	m_bMingZhu = false;
	m_lMaxCellScore = pGameStart->lMaxScore;
	m_lUserMaxScore = pGameStart->lUserMaxScore;
	m_lCurrentTimes = pGameStart->lMinScore;

	//玩家处理
	ASSERT(m_pIAndroidUserItem != NULL);
	if (m_pIAndroidUserItem->GetChairID()==pGameStart->wCurrentUser)
	{
		//设置定时器
		UINT nElapse = TIME_LESS + 1 + TIME_LESS + (rand() % TIME_USER_ADD_SCORE);
		m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE, nElapse);
	}

	return true;
}

//用户放弃
bool CAndroidUserItemSink::OnSubGiveUp(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize == sizeof(CMD_S_GiveUp));
	if (wDataSize != sizeof(CMD_S_GiveUp)) return true;

	//处理消息
	CMD_S_GiveUp * pGiveUp = (CMD_S_GiveUp *)pBuffer;

	//设置变量
	m_cbRealPlayer[pGiveUp->wGiveUpUser] = FALSE;
	m_cbPlayStatus[pGiveUp->wGiveUpUser] = PlayerGiveUp;
	//m_cbAndroidStatus[pGiveUp->wGiveUpUser] = FALSE;

	if (pGiveUp->wCurrentUser != INVALID_CHAIR)
	{
		m_wCurrentUser = pGiveUp->wCurrentUser;
	}

	//玩家人数
	BYTE cbUserCount = 0;
	for (WORD i = 0; i<GAME_PLAYER; i++)
	{
		if (m_cbPlayStatus[i] == PlayerPlaying || m_cbPlayStatus[i] == PlayerLookCard) cbUserCount++;
	}

	if (cbUserCount > 1)
	{
		if (pGiveUp->wCurrentUser == m_pIAndroidUserItem->GetChairID())
		{
			//设置定时器
			UINT nElapse = TIME_LESS + 1 + (rand() % TIME_USER_ADD_SCORE);
			m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE, nElapse + TIME_DELAY_TIME);
		}
	}

	return true;
}

//用户下注
bool CAndroidUserItemSink::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize == sizeof(CMD_S_AddScore));
	if (wDataSize != sizeof(CMD_S_AddScore)) return true;

	//处理消息
	CMD_S_AddScore * pAddScore = (CMD_S_AddScore *)pBuffer;

	m_cbGameTurns = pAddScore->cbGameTurns;

	m_lCurrentTimes = pAddScore->lMinScore;

	//设置变量
	m_wCurrentUser = pAddScore->wCurrentUser;
	m_lTableScore[pAddScore->wAddScoreUser] += pAddScore->lAddScoreCount;
	WORD wMeChairID = m_pIAndroidUserItem->GetChairID();

	//用户动作
	if (wMeChairID==m_wCurrentUser)
	{
		//设置定时器
		UINT nElapse=TIME_LESS + 1 + (rand() % TIME_USER_ADD_SCORE);
		m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE, nElapse);
	}

	//随机一定几率看牌
	if (m_cbGameTurns > 1 && !m_bMingZhu)
	{
		//随机概率
		int cbProbability = rand() % (10);

		BYTE cbCardType = m_GameLogic.GetCardType(m_cbAllHandCardData[wMeChairID], MAX_COUNT);

		if (cbCardType == CT_SPECIAL)
			cbCardType = CT_SINGLE;

		int nLookFlag = m_nLookCard - (cbCardType - CT_SINGLE);

		SCORE lHaveScore = m_lUserMaxScore - m_lTableScore[m_pIAndroidUserItem->GetChairID()];

		//看牌概率 如果钱太少或者进入看牌几率
		if (cbProbability < nLookFlag || lHaveScore < 4*20*m_lCellScore)
		{
			UINT nElapse = TIME_LESS + 1 + (rand() % TIME_USER_ADD_SCORE);
			m_pIAndroidUserItem->SetGameTimer(IDI_LOOK_CARD, nElapse);
			return true;
		}
	}

	//随机一定几率放弃
	if (m_bMingZhu && pAddScore->wAddScoreUser != wMeChairID)
	{
		//随机概率
		int cbProbability = rand() % (10);

		//散牌放弃
		if (m_cbCardType == CT_SINGLE)
		{
			//放弃概率
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

//用户全压
bool CAndroidUserItemSink::OnSubShowHand(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize == sizeof(CMD_S_AddScore));
	if (wDataSize != sizeof(CMD_S_AddScore)) return true;

	//处理消息
	CMD_S_AddScore * pAddScore = (CMD_S_AddScore *)pBuffer;

	m_cbGameTurns = pAddScore->cbGameTurns;

	m_bShowHandStatus = pAddScore->cbShowHandStatus;

	m_lCurrentTimes = pAddScore->lMinScore;

	//设置变量
	m_wCurrentUser = pAddScore->wCurrentUser;
	m_lTableScore[pAddScore->wAddScoreUser] += pAddScore->lAddScoreCount;
	WORD wMeChairID = m_pIAndroidUserItem->GetChairID();

	//用户动作
	if (wMeChairID == m_wCurrentUser)
	{
		//设置定时器
		UINT nElapse = TIME_LESS + 1 + (rand() % TIME_USER_ADD_SCORE);
		m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE, nElapse);
	}
	return true;
}
//用户看牌
bool CAndroidUserItemSink::OnSubLookCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize == sizeof(CMD_S_LookCard));
	if (wDataSize != sizeof(CMD_S_LookCard)) return true;

	//处理消息
	CMD_S_LookCard * pLookCard = (CMD_S_LookCard *)pBuffer;

	m_cbPlayStatus[pLookCard->wLookCardUser] = PlayerLookCard;

	//看牌用户
	if (pLookCard->wLookCardUser == m_pIAndroidUserItem->GetChairID())
	{
		m_bMingZhu = true;
		//用户扑克
		CopyMemory(m_cbHandCardData, pLookCard->cbCardData, sizeof(m_cbHandCardData));

		//获取类型
		m_GameLogic.SortCardList(m_cbHandCardData, MAX_COUNT);
		m_cbCardType = m_GameLogic.GetCardType(m_cbHandCardData, MAX_COUNT);
	}

	return true;
}

//用户比牌
bool CAndroidUserItemSink::OnSubCompareCard(const void * pBuffer, WORD wDataSize)
{
	//数据效验
	ASSERT(wDataSize == sizeof(CMD_S_CompareCard));
	if (wDataSize != sizeof(CMD_S_CompareCard)) return true;

	//处理消息
	CMD_S_CompareCard * pCompareCard = (CMD_S_CompareCard *)pBuffer;

	//设置变量
	if (pCompareCard->wCurrentUser != INVALID_CHAIR)
	{
		m_wCurrentUser = pCompareCard->wCurrentUser;
	}

	m_cbPlayStatus[pCompareCard->wCompareUser[1]] = PlayerComFail;

	//玩家人数
	BYTE cbUserCount = 0;
	for (WORD i=0; i<GAME_PLAYER; i++)
	{
		if (m_cbPlayStatus[i] == PlayerPlaying || m_cbPlayStatus[i] == PlayerLookCard) cbUserCount++;
	}

	m_lTableScore[pCompareCard->wCompareUser[0]] += pCompareCard->lCompareScore;

	//判断结束
	if (cbUserCount > 1)
	{
		//当前用户
		if (m_pIAndroidUserItem->GetChairID() == m_wCurrentUser)
		{
			//设置定时器
			UINT nElapse=TIME_LESS + 1 + (rand() % TIME_USER_ADD_SCORE);
			m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE, nElapse+TIME_DELAY_TIME);
		}
	}

	return true;
}

//用户开牌
bool CAndroidUserItemSink::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
	//数据效验
	ASSERT(wDataSize == sizeof(CMD_S_OpenCard));
	if (wDataSize != sizeof(CMD_S_OpenCard)) return true;

	return true;
}

//用户强退
bool CAndroidUserItemSink::OnSubPlayerExit(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	//ASSERT(wDataSize == sizeof(CMD_S_PlayerExit));
	//if (wDataSize != sizeof(CMD_S_PlayerExit)) return false;

	////处理消息
	//CMD_S_PlayerExit * pPlayerExit=(CMD_S_PlayerExit *)pBuffer;

	////设置变量
	//m_cbPlayStatus[pPlayerExit->wPlayerID] = FALSE;

	return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验参数
	if (wDataSize != sizeof(CMD_S_GameEnd)) return true;
	CMD_S_GameEnd * pGameEnd = (CMD_S_GameEnd *)pBuffer;

	//删除定时器
	m_pIAndroidUserItem->KillGameTimer(IDI_START_GAME);
	m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);
	m_pIAndroidUserItem->KillGameTimer(IDI_LOOK_CARD);
	m_pIAndroidUserItem->KillGameTimer(IDI_DELAY_SHOWHAND);
	m_pIAndroidUserItem->KillGameTimer(IDI_GIVE_UP);
	

	//清理变量
	m_lMaxCellScore = 0;
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	return true;
}



//银行操作
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

	//变量定义
	SCORE lRobotScore = pUserItem->GetUserScore();

	{
		

		//判断存取
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

//读取配置
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

//推断胜者
WORD CAndroidUserItemSink::EstimateWinner()
{
	//保存扑克
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData, m_cbAllHandCardData, sizeof(cbUserCardData));

	//排列扑克
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		m_GameLogic.SortCardList(cbUserCardData[i], MAX_COUNT);
	}

	//变量定义
	WORD wWinUser = INVALID_CHAIR;

	//查找数据
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		//用户过滤
		if (m_cbPlayStatus[i] == PlayerNoPlay || m_cbPlayStatus[i] == PlayerGiveUp || PlayerGiveUp == PlayerComFail) continue;

		//设置用户
		if (wWinUser == INVALID_CHAIR)
		{
			wWinUser=i;
			continue;
		}

		//对比扑克
		if (m_GameLogic.CompareCard(cbUserCardData[i], cbUserCardData[wWinUser], MAX_COUNT) >= TRUE)
		{
			wWinUser = i;
		}
	}

	return wWinUser;
}

//加注倍数
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
//组件创建函数
DECLARE_CREATE_MODULE(AndroidUserItemSink);

//////////////////////////////////////////////////////////////////////////
