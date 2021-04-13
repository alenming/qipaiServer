#include "StdAfx.h"
#include "DlgCustomRule.h"
#include "TableFrameSink.h"
#include <locale>
//////////////////////////////////////////////////////////////////////////

//常量定义
#define SEND_COUNT					300									//发送次数

//索引定义
#define INDEX_PLAYER				    0									//红家索引
#define INDEX_BANKER				1									//黑家索引

//下注时间
#define IDI_FREE					1									//空闲时间
#define TIME_FREE					1									//空闲时间

//下注时间
#define IDI_PLACE_JETTON			2									//下注时间
#define TIME_PLACE_JETTON			5									//下注时间

//结束时间
#define IDI_GAME_END				3									//结束时间
#define TIME_GAME_END				8									//结束时间


//////////////////////////////////////////////////////////////////////////


//构造函数
CTableFrameSink::CTableFrameSink()
{
	//起始分数
	ZeroMemory(m_lUserStartScore,sizeof(m_lUserStartScore));

	//下注数
	ZeroMemory(m_lAllBet,sizeof(m_lAllBet));
	ZeroMemory(m_lPlayBet,sizeof(m_lPlayBet));
	
	ZeroMemory(m_bUserListWin, sizeof(m_bUserListWin));
	ZeroMemory(m_lUserListScore, sizeof(m_lUserListScore));
	ZeroMemory(m_cbUserPlayCount, sizeof(m_cbUserPlayCount));

	//分数
	m_lBankerScore = 0l;
	ZeroMemory(m_lPlayScore,sizeof(m_lPlayScore));
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//扑克信息
	ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

	//状态变量
	m_dwBetTime=0L;
	CopyMemory(m_szControlName,TEXT("无人"),sizeof(m_szControlName));

	//庄家信息
	m_ApplyUserArray.RemoveAll();
	m_wCurrentBanker=INVALID_CHAIR;
	m_wOfflineBanker = INVALID_CHAIR;
	m_wBankerTime=0;
	m_lBankerWinScore=0L;		
	m_lBankerCurGameScore=0L;
	m_bEnableSysBanker=true;

	//记录变量
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;
	m_nRecordLast=0;
	m_dwRecordCount=0;

	//庄家设置
	m_nBankerTimeLimit = 0l;
	m_nBankerTimeAdd = 0l;							
	m_lExtraBankerScore = 0l;
	m_nExtraBankerTime = 0l;
	m_lPlayerBankerMAX = 0l;
	m_bExchangeBanker = true;

	m_cbOpenResult = 0;
	m_cbEarlyOpen = FALSE;

	//时间控制
	m_cbFreeTime = TIME_FREE;
	m_cbBetTime = TIME_PLACE_JETTON;
	m_cbEndTime = TIME_GAME_END;

	//机器人控制
	m_nChipRobotCount = 0;
	m_nRobotListMaxCount =0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));


	m_cbJuControl = FALSE;
	m_cbJuControlTimes = 0;
	ZeroMemory(m_cbJuControlArea, sizeof(m_cbJuControlArea));

	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{

}

//释放对象
VOID CTableFrameSink::Release()
{

}

//接口查询
void * CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//初始化
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame == NULL) return false;

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//开始模式
	m_pITableFrame->SetStartMode(START_MODE_TIME_CONTROL);

	//读取配置
	memcpy(m_szGameRoomName, m_pGameServiceOption->szServerName, sizeof(m_szGameRoomName));

	//设置文件名
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_sntprintf(m_szConfigFileName, sizeof(m_szConfigFileName), TEXT("%s\\BaiRenConfig\\DTBattleConfig-%d-%d.ini"), szPath, m_pGameServiceOption->wServerID, m_pITableFrame->GetTableID());

	m_nChip[0] = GetPrivateProfileInt(TEXT("龙虎斗"), TEXT("nChip0"), 1, m_szConfigFileName);
	m_nChip[1] = GetPrivateProfileInt(TEXT("龙虎斗"), TEXT("nChip1"), 10, m_szConfigFileName);
	m_nChip[2] = GetPrivateProfileInt(TEXT("龙虎斗"), TEXT("nChip2"), 50, m_szConfigFileName);
	m_nChip[3] = GetPrivateProfileInt(TEXT("龙虎斗"), TEXT("nChip3"), 100, m_szConfigFileName);
	m_nChip[4] = GetPrivateProfileInt(TEXT("龙虎斗"), TEXT("nChip4"), 1000, m_szConfigFileName);

	m_lUserLimitScore = GetPrivateProfileInt(TEXT("龙虎斗"), TEXT("nLimitMax"), 20000, m_szConfigFileName);

	ReadConfigInformation();	

	return true;
}

//复位桌子
void CTableFrameSink::RepositionSink()
{
	m_wOfflineBanker = INVALID_CHAIR;

	ZeroMemory(m_lAllBet,sizeof(m_lAllBet));
	ZeroMemory(m_lPlayBet,sizeof(m_lPlayBet));
	

	//分数
	m_lBankerScore = 0l;
	ZeroMemory(m_lPlayScore,sizeof(m_lPlayScore));
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//机器人控制
	m_nChipRobotCount = 0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

	m_cbOpenResult = 0;
	m_cbEarlyOpen = FALSE;
	return;
}

//查询限额
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	if(pIServerUserItem->GetUserStatus() == US_PLAYING)
	{
		return 0L;
	}
	else
	{
		return __max(pIServerUserItem->GetUserScore()-m_pGameServiceOption->lMinTableScore, 0L);
	}
}

//游戏开始
bool CTableFrameSink::OnEventGameStart()
{
	m_cbOpenResult = 0;
	m_cbEarlyOpen = FALSE;

	//获取庄家
	IServerUserItem* pIBankerServerUserItem = NULL;
	if ( m_wCurrentBanker == INVALID_CHAIR )
	{
		m_lBankerScore = 1000000000;
	}
	else
	{
		IServerUserItem* pIBankerServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if ( pIBankerServerUserItem != NULL )
		{
			m_lBankerScore = pIBankerServerUserItem->GetUserScore();
		}
	}

	//机器人控制
	m_nChipRobotCount = 0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

	//变量定义
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart, sizeof(GameStart));

	//设置变量
	GameStart.cbTimeLeave = m_cbBetTime;

	FillMemory(GameStart.wSeatUser, sizeof(GameStart.wSeatUser), INVALID_CHAIR);

	OnGetUserListGameID(GameStart.wSeatUser);

    //旁观玩家
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GAME_START, &GameStart, sizeof(GameStart));

	return true;
}

//游戏结束
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束	
		{
			//计算分数
			SCORE lBankerWinScore = GameOver();

			//递增次数
			m_wBankerTime++;


			//结束消息
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//列表玩家信息
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);
				if (!pServerUser) continue;

				if (m_lUserWinScore[i] > 0)
				{
					m_bUserListWin[i][m_cbUserPlayCount[i]] = true;
				}

				SCORE lAddScore = 0;
				for (BYTE j = 0; j < AREA_MAX; j++)
				{
					lAddScore += m_lPlayBet[i][j];
				}
				m_lUserListScore[i][m_cbUserPlayCount[i]] = lAddScore;
				m_cbUserPlayCount[i] ++;

				if (m_cbUserPlayCount[i] == USER_LIST_COUNT)
					m_cbUserPlayCount[i] = 0;
			}

			//占位玩家成绩
			WORD wSeatUser[MAX_SEAT_COUNT];
			FillMemory(wSeatUser, sizeof(wSeatUser),INVALID_CHAIR);
			OnGetUserListGameID(wSeatUser);

			for (WORD i = 0; i < MAX_SEAT_COUNT; i++)
			{
				if (wSeatUser[i] != INVALID_CHAIR)
				{
					GameEnd.lSeatUserWinScore[i] = m_lUserWinScore[wSeatUser[i]];
				}
			}

			//庄家信息
			GameEnd.lBankerWinScore = lBankerWinScore;

			if (m_wCurrentBanker == INVALID_CHAIR)
				GameEnd.lBankerRestScore = 100000000;
			else
			{
				IServerUserItem* pBankerUser = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				if (pBankerUser)
					GameEnd.lBankerRestScore = pBankerUser->GetUserScore() + lBankerWinScore;
			}

			//扑克信息
			CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
			
			GameEnd.cbResult = m_cbOpenResult;
			GameEnd.cbEarlyOpenFlag = m_cbEarlyOpen;

			for ( WORD i = 0; i < GAME_PLAYER; ++i )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
				if ( pIServerUserItem == NULL ) continue;

				//设置成绩
				CopyMemory(GameEnd.lPlayAreaScore, m_lPlayScore[i], sizeof(GameEnd.lPlayAreaScore));
				GameEnd.lPlayerWinScore = m_lUserWinScore[i];
				GameEnd.lPlayerRestScore = pIServerUserItem->GetUserScore() + GameEnd.lPlayerWinScore;

				//发送消息					
				m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

				if (!pIServerUserItem->IsAndroidUser())
				{
					m_pGameServiceOption->pRoomStorageOption.lStorageCurrent -= GameEnd.lPlayerWinScore;
				}
			}

			tagDTPerGameRecord GameRecord;
			GameRecord.wTableID = m_pITableFrame->GetTableID() + 1;
			GameRecord.bAreaWin[0] = m_cbOpenResult == AREA_DRAGON ? TRUE:FALSE;
			GameRecord.bAreaWin[1] = m_cbOpenResult == AREA_TIGER ? TRUE:FALSE;
			GameRecord.bAreaWin[2] = m_cbOpenResult == AREA_PING ? TRUE:FALSE;
			m_pITableFrame->SendTableDataToLookOnUser(SUB_GF_BJL_GAME_RESULT, &GameRecord, sizeof(GameRecord));

			return true;
		}
	case GER_NETWORK_ERROR:		//网络中断
	case GER_USER_LEAVE:		//用户离开
		{
			return true;
			//闲家判断
			if (m_wCurrentBanker!=wChairID)
			{
				//变量定义
				SCORE lRevenue = 0;

				//写入积分
				if (m_pITableFrame->GetGameStatus() != GAME_SCENE_END)
				{
					for (WORD wAreaIndex = AREA_DRAGON; wAreaIndex < AREA_MAX; ++wAreaIndex)
					{
						if (m_lPlayBet[wChairID][wAreaIndex] != 0)
						{
							m_lAllBet[wAreaIndex] -= m_lPlayBet[wChairID][wAreaIndex];
							m_lPlayBet[wChairID][wAreaIndex] = 0;
						}
					}
				}
				else
				{

					//是否泄愤
					bool bWritePoints = false;
					for (WORD wAreaIndex = AREA_DRAGON; wAreaIndex < AREA_MAX; ++wAreaIndex)
					{
						if( m_lPlayBet[wChairID][wAreaIndex] != 0 )
						{
							bWritePoints = true;
							break;
						}
					}

					//不写分
					if( !bWritePoints )
					{
						return true;
					}

					//写入积分
					if(pIServerUserItem->GetUserScore() + m_lUserWinScore[wChairID] < 0L)
						m_lUserWinScore[wChairID] = -pIServerUserItem->GetUserScore();

					//库存金币
					if (!pIServerUserItem->IsAndroidUser())
						m_pGameServiceOption->pRoomStorageOption.lStorageCurrent -= m_lUserWinScore[wChairID];

					tagScoreInfo ScoreInfoArray;
					ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));
					ScoreInfoArray.lScore=m_lUserWinScore[wChairID];
					ScoreInfoArray.lRevenue = m_lUserRevenue[wChairID];

					if ( ScoreInfoArray.lScore > 0 )
						ScoreInfoArray.cbType=SCORE_TYPE_WIN;
					else if ( ScoreInfoArray.lScore < 0 )
						ScoreInfoArray.cbType=SCORE_TYPE_LOSE;
					else
						ScoreInfoArray.cbType=SCORE_TYPE_DRAW;

					m_pITableFrame->WriteUserScore(wChairID,ScoreInfoArray);
					m_lUserWinScore[wChairID] = 0;
					m_lUserRevenue[wChairID] = 0;
					ZeroMemory(m_lPlayScore[wChairID], sizeof(m_lPlayScore[wChairID]));


					//清除下注
					for (WORD wAreaIndex = AREA_DRAGON; wAreaIndex < AREA_MAX; ++wAreaIndex)
					{
						m_lPlayBet[wChairID][wAreaIndex] = 0;
					}
				}

				return true;
			}

			//状态判断
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_END)
			{
				//提示消息
				TCHAR szTipMsg[128];
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("由于庄家[ %s ]强退，游戏提前结束！"),pIServerUserItem->GetNickName());

				//发送消息
				SendGameMessage(INVALID_CHAIR,szTipMsg);	

				//设置状态
				m_pITableFrame->SetGameStatus(GAME_SCENE_END);

				//设置时间
				m_dwBetTime=(DWORD)time(NULL);
				m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);

				//计算分数
				SCORE lBankerWinScore = GameOver();

				//结束消息
				CMD_S_GameEnd GameEnd;
				ZeroMemory(&GameEnd,sizeof(GameEnd));

				//庄家信息
				GameEnd.lBankerWinScore = lBankerWinScore;

				//占位玩家成绩
				WORD wSeatUser[MAX_SEAT_COUNT] = { INVALID_CHAIR };
				FillMemory(wSeatUser, sizeof(wSeatUser), INVALID_CHAIR);
				OnGetUserListGameID(wSeatUser);

				for (WORD i = 0; i < MAX_SEAT_COUNT; i++)
				{
					if (wSeatUser[i] != INVALID_CHAIR)
					{
						GameEnd.lSeatUserWinScore[i] = m_lUserWinScore[wSeatUser[i]];
					}
				}

				if (m_wCurrentBanker == INVALID_CHAIR)
					GameEnd.lBankerRestScore = 100000000;
				else
				{
					IServerUserItem* pBankerUser = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
					if (pBankerUser)
						GameEnd.lBankerRestScore = pBankerUser->GetUserScore() + lBankerWinScore;
				}

				//扑克信息
				CopyMemory(GameEnd.cbTableCardArray, m_cbTableCardArray, sizeof(m_cbTableCardArray));

				GameEnd.cbResult = m_cbOpenResult;
				GameEnd.cbEarlyOpenFlag = m_cbEarlyOpen;
				
				for ( WORD i = 0; i < GAME_PLAYER; ++i )
				{
					IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
					if ( pIServerUserItem == NULL ) continue;

					CopyMemory(GameEnd.lPlayAreaScore, m_lPlayScore[i], sizeof(GameEnd.lPlayAreaScore));
					GameEnd.lPlayerWinScore = m_lUserWinScore[i];
					GameEnd.lPlayerRestScore = pIServerUserItem->GetUserScore() + GameEnd.lPlayerWinScore;

					//发送消息					
					m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					m_pITableFrame->SendLookonData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

					if (!pIServerUserItem->IsAndroidUser())
					{
						m_pGameServiceOption->pRoomStorageOption.lStorageCurrent -= GameEnd.lPlayerWinScore;
					}
				}
			}

			//扣除分数
			if(pIServerUserItem->GetUserScore() + m_lUserWinScore[m_wCurrentBanker] < 0L)
				m_lUserWinScore[m_wCurrentBanker] = -pIServerUserItem->GetUserScore();

			tagScoreInfo ScoreInfoArray;
			ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));
			ScoreInfoArray.lScore = m_lUserWinScore[m_wCurrentBanker];
			ScoreInfoArray.lRevenue = m_lUserRevenue[m_wCurrentBanker];

			if ( ScoreInfoArray.lScore > 0 )
				ScoreInfoArray.cbType = SCORE_TYPE_WIN;
			else if ( ScoreInfoArray.lScore < 0 )
				ScoreInfoArray.cbType = SCORE_TYPE_LOSE;
			else
				ScoreInfoArray.cbType = SCORE_TYPE_DRAW;

			//m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

			m_pITableFrame->WriteUserScore( m_wCurrentBanker, ScoreInfoArray );

			if (m_wCurrentBanker != INVALID_CHAIR)
			{
				IServerUserItem *pBankerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				if (pBankerUserItem && !pBankerUserItem->IsAndroidUser())
				{
					m_pGameServiceOption->pRoomStorageOption.lStorageCurrent -= m_lUserWinScore[m_wCurrentBanker];
				}
			}

			m_lUserWinScore[m_wCurrentBanker] = 0L;

			//切换庄家
			ChangeBanker(true);

			return true;
		}
	}

	return false;
}

//发送场景
bool CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:			//空闲状态
		{
			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));
			FillMemory(StatusFree.wApplyUser, sizeof(StatusFree.wApplyUser),INVALID_CHAIR);
			FillMemory(StatusFree.wSeatUser, sizeof(StatusFree.wSeatUser), INVALID_CHAIR);

			//全局信息
			DWORD dwPassTime = (DWORD)time(NULL)-m_dwBetTime;
			StatusFree.cbTimeLeave = (BYTE)(m_cbFreeTime - __min(dwPassTime, m_cbFreeTime));

			//获取庄家
			IServerUserItem* pIBankerServerUserItem = NULL;
			if ( m_wCurrentBanker == INVALID_CHAIR )
			{
				m_lBankerScore = 1000000000;
			}
			else
			{
				IServerUserItem* pIBankerServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				if ( pIBankerServerUserItem != NULL )
				{
					m_lBankerScore = pIBankerServerUserItem->GetUserScore();
					
				}
			}
			//庄家信息
			StatusFree.wBankerUser = m_wCurrentBanker;
			StatusFree.lBankerScore = m_lBankerScore;
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
			StatusFree.lAreaLimitScore = m_lAreaLimitScore;

			StatusFree.cbFreeTime = m_cbFreeTime;
			StatusFree.cbPlayTime = m_cbBetTime;
			StatusFree.cbEndTime = m_cbEndTime;

			StatusFree.lMinXianHong = m_nChip[0];
			StatusFree.lMaxXianHong = m_lUserLimitScore;

			CopyMemory(StatusFree.nChip, m_nChip, sizeof(StatusFree.nChip));
			
			for (INT_PTR nUserIdx = 0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
			{
				WORD wChairID = m_ApplyUserArray[nUserIdx];

				//获取玩家
				IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
				if (!pServerUserItem) continue;

				//庄家判断
				if (pServerUserItem->GetChairID() == m_wCurrentBanker) continue;

				StatusFree.wApplyUser[nUserIdx] = wChairID;
			}

			OnGetUserListGameID(StatusFree.wSeatUser);

			//发送场景
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
						
			//机器人配置
			if (pIServerUserItem->IsAndroidUser())
			{
				tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pITableFrame->GetCustomRule();
				ASSERT(pCustomConfig);

				m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_ANDROID_CONFIG, &pCustomConfig->CustomAndroid, sizeof(tagCustomAndroid));
			}

			//发送记录
			SendGameRecord(pIServerUserItem);

			return bSuccess;
		}
	case GAME_SCENE_BET:		//游戏状态
	case GAME_SCENE_END:		//结束状态
		{
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			FillMemory(StatusPlay.wApplyUser, sizeof(StatusPlay.wApplyUser), INVALID_CHAIR);
			FillMemory(StatusPlay.wSeatUser, sizeof(StatusPlay.wSeatUser), INVALID_CHAIR);

			//全局信息
			DWORD dwPassTime=(DWORD)time(NULL) - m_dwBetTime;
			int	nTotalTime = ( (cbGameStatus == GAME_SCENE_BET) ? m_cbBetTime : m_cbEndTime);
			StatusPlay.cbTimeLeave = (BYTE)(nTotalTime - __min(dwPassTime, (DWORD)nTotalTime));
			
			StatusPlay.lBankerWinScore = m_lBankerWinScore;	

			//获取庄家
			IServerUserItem* pIBankerServerUserItem = NULL;
			if ( m_wCurrentBanker == INVALID_CHAIR )
			{
				m_lBankerScore = 1000000000;
			}
			else
			{
				IServerUserItem* pIBankerServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				if ( pIBankerServerUserItem != NULL )
				{
					m_lBankerScore = pIBankerServerUserItem->GetUserScore();
					
				}
			}
			//庄家信息
			StatusPlay.wBankerUser = m_wCurrentBanker;
			StatusPlay.lBankerScore = m_lBankerScore;

			//结束判断
			if (cbGameStatus == GAME_SCENE_END)
			{
				//扑克信息
				CopyMemory(StatusPlay.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));

				StatusPlay.lPlayerWinScore = m_lUserWinScore[pIServerUserItem->GetChairID()];
				
				StatusPlay.cbResult = m_cbOpenResult;

				//占位玩家成绩
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

				if (m_wCurrentBanker != INVALID_CHAIR)
				{
					IServerUserItem* pIBankerServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
					if (pIBankerServerUserItem != NULL)
						StatusPlay.lBankerRestScore = pIBankerServerUserItem->GetUserScore() + m_lUserWinScore[m_wCurrentBanker];
				}
				else
					StatusPlay.lBankerRestScore = 1000000000;

				StatusPlay.lPlayerRestScore = pIServerUserItem->GetUserScore() + m_lUserWinScore[pIServerUserItem->GetChairID()];
			}
			else
			{
				if (m_wCurrentBanker != INVALID_CHAIR)
				{
					IServerUserItem* pIBankerServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
					if (pIBankerServerUserItem != NULL)
						StatusPlay.lBankerRestScore = pIBankerServerUserItem->GetUserScore();
				}
				else
					StatusPlay.lBankerRestScore = 1000000000;
			}

			StatusPlay.lBankerScore = m_lBankerScore;
			StatusPlay.lApplyBankerCondition = m_lApplyBankerCondition;
			StatusPlay.lAreaLimitScore = m_lAreaLimitScore;

			StatusPlay.cbFreeTime = m_cbFreeTime;
			StatusPlay.cbPlayTime = m_cbBetTime;
			StatusPlay.cbEndTime = m_cbEndTime;

			StatusPlay.lMinXianHong = m_nChip[0];
			StatusPlay.lMaxXianHong = m_lUserLimitScore;

			CopyMemory(StatusPlay.lPlayerJettonScore, m_lPlayBet[wChiarID], sizeof(StatusPlay.lPlayerJettonScore));
			CopyMemory(StatusPlay.lAllJettonScore, m_lAllBet, sizeof(StatusPlay.lAllJettonScore));

			CopyMemory(StatusPlay.nChip, m_nChip, sizeof(StatusPlay.nChip));

			for (INT_PTR nUserIdx = 0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
			{
				WORD wChairID = m_ApplyUserArray[nUserIdx];

				//获取玩家
				IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
				if (!pServerUserItem) continue;

				//庄家判断
				if (pServerUserItem->GetChairID() == m_wCurrentBanker) continue;

				StatusPlay.wApplyUser[nUserIdx] = wChairID;
			}

			OnGetUserListGameID(StatusPlay.wSeatUser);

			//发送场景
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

			//机器人配置
			if (pIServerUserItem->IsAndroidUser())
			{
				tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pITableFrame->GetCustomRule();
				ASSERT(pCustomConfig);

				m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_ANDROID_CONFIG, &pCustomConfig->CustomAndroid, sizeof(tagCustomAndroid));
			}

			//发送记录
			SendGameRecord(pIServerUserItem);
			
			return bSuccess;
		}
	}

	return false;
}

//定时器事件
bool  CTableFrameSink::OnTimerMessage(DWORD dwTimerID, WPARAM dwBindParameter)
{
	switch (dwTimerID)
	{
	case IDI_FREE:		//空闲时间
		{
			//开始游戏
			m_pITableFrame->StartGame();

			//设置时间
			m_dwBetTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,m_cbBetTime*1000,1,0L);

			//设置状态
			m_pITableFrame->SetGameStatus(GAME_SCENE_BET);

			//库存衰减
			m_pITableFrame->CalculateStorageDeduct();

			tagChangeStatus ChangeStatus;
			ChangeStatus.wTableID = m_pITableFrame->GetTableID() + 1;
			ChangeStatus.cbGameStatus = m_pITableFrame->GetGameStatus();
			m_pITableFrame->SendTableDataToLookOnUser(SUB_GF_BJL_CHANGE_STATUS, &ChangeStatus, sizeof(ChangeStatus));

			return true;
		}
	case IDI_PLACE_JETTON:		//下注时间
		{
			//状态判断(防止强退重复设置)
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_END)
			{
				//设置状态
				m_pITableFrame->SetGameStatus(GAME_SCENE_END);			

				//结束游戏
				OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);

				//设置时间
				m_dwBetTime=(DWORD)time(NULL);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);			

				tagChangeStatus ChangeStatus;
				ChangeStatus.wTableID = m_pITableFrame->GetTableID() + 1;
				ChangeStatus.cbGameStatus = m_pITableFrame->GetGameStatus();
				m_pITableFrame->SendTableDataToLookOnUser(SUB_GF_BJL_CHANGE_STATUS, &ChangeStatus, sizeof(ChangeStatus));
			}

			return true;
		}
	case IDI_GAME_END:			//结束游戏
		{
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));
			SCORE TempStartScore = 0;
			//写入积分
			for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);
				if ( pIServerUserItem == NULL ) continue;

				//判断是否写分
				bool bXie = false;
				for ( WORD iA = 0; iA < AREA_MAX; ++iA )
				{
					if ( m_lPlayBet[wUserChairID][iA] != 0 )
					{
						bXie = true;
						break;
					}
				}
				
				//当前庄家
				if ( wUserChairID == m_wCurrentBanker )
				{
					bXie = true;
				}

				//离线庄家
				if (wUserChairID == m_wOfflineBanker)
				{
					bXie = true;
				}

				if ( !bXie )
				{
					continue;
				}

				//防止负分
				if((pIServerUserItem->GetUserScore() + m_lUserWinScore[wUserChairID]) < 0L)
					m_lUserWinScore[wUserChairID] = -pIServerUserItem->GetUserScore();

				//写入积分				
				ScoreInfoArray[wUserChairID].lScore = m_lUserWinScore[wUserChairID];
				ScoreInfoArray[wUserChairID].lRevenue = m_lUserRevenue[wUserChairID];

				if ( ScoreInfoArray[wUserChairID].lScore > 0 )
				{
					ScoreInfoArray[wUserChairID].cbType = SCORE_TYPE_WIN;
				}
				else if ( ScoreInfoArray[wUserChairID].lScore < 0 )
					ScoreInfoArray[wUserChairID].cbType = SCORE_TYPE_LOSE;
				else
					ScoreInfoArray[wUserChairID].cbType = SCORE_TYPE_DRAW;

			}

			m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

			//设置时间
			m_dwBetTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,0L);

			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_FREE);
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_FREE);

			//大厅跑马灯消息
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
				if (pServerUser && m_lUserWinScore[i] > 1000)
				{
					CMD_GR_GameRadioMessage GameRadioMessage;
					ZeroMemory(&GameRadioMessage, sizeof(GameRadioMessage));

					GameRadioMessage.cbCardType = 0;
					GameRadioMessage.wKindID = m_pGameServiceOption->wKindID;
					GameRadioMessage.lScore = m_lUserWinScore[i];
					lstrcpyn(GameRadioMessage.szNickName, pServerUser->GetNickName(), CountArray(GameRadioMessage.szNickName));
					m_pITableFrame->SendHallRadioMessage(&GameRadioMessage, sizeof(GameRadioMessage));
				}
			}

			//结束游戏
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);

			//切换庄家
			ChangeBanker(false);

			tagChangeStatus ChangeStatus;
			ChangeStatus.wTableID = m_pITableFrame->GetTableID() + 1;
			ChangeStatus.cbGameStatus = m_pITableFrame->GetGameStatus();
			m_pITableFrame->SendTableDataToLookOnUser(SUB_GF_BJL_CHANGE_STATUS, &ChangeStatus, sizeof(ChangeStatus));

			return true;
		}
	}

	return false;
}

//获取百人游戏是否下注状态而且玩家下注了
bool CTableFrameSink::OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if (m_pITableFrame->GetGameStatus() == GAME_SCENE_FREE)
		return false;

	for (WORD wAreaIndex = AREA_DRAGON; wAreaIndex < AREA_MAX; ++wAreaIndex)
	{
		if (m_lPlayBet[wChairID][wAreaIndex] != 0)
		{
			return true;
		}
	}

	if (wChairID == m_wCurrentBanker)
		return true;

	return false;
}

//请求用户列表
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

	//找出一个获胜局数最多的放第一个
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

	//剩下的按下注量排列
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

	//然后开始发送了。。。
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

//获取前6个用户列表的椅子号
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

	//找出一个获胜局数最多的放第一个
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

	//剩下的按下注量排列
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

//百人游戏获取游戏记录
void CTableFrameSink::OnGetGameRecord(VOID *GameRecord)
{
	CMD_GF_DTRoomStatus *pRoomStatus = (CMD_GF_DTRoomStatus *)GameRecord;

	pRoomStatus->tagGameInfo.wTableID = m_pITableFrame->GetTableID() + 1;
	pRoomStatus->tagGameInfo.cbGameStatus = m_pITableFrame->GetGameStatus();

	pRoomStatus->tagTimeInfo.cbFreeTime = m_cbFreeTime;
	pRoomStatus->tagTimeInfo.cbBetTime = m_cbBetTime;
	pRoomStatus->tagTimeInfo.cbEndTime = m_cbEndTime;
	pRoomStatus->tagTimeInfo.cbPassTime = (DWORD)time(NULL) - m_dwBetTime;
	pRoomStatus->tagTimeInfo.lMinXianHong = m_nChip[0];
	pRoomStatus->tagTimeInfo.lMaxXianHong = m_lUserLimitScore;

	//客户端只显示48条
	int nIndex = m_nRecordFirst;

	pRoomStatus->cbRecordCount = m_nRecordLast - m_nRecordFirst;
	if (pRoomStatus->cbRecordCount > 48)
	{
		nIndex = m_nRecordLast - 48;
		pRoomStatus->cbRecordCount = 48;
	}

	int nArrayIndex = 0;

	int nEndIndex = (m_nRecordLast + 1) % MAX_SCORE_HISTORY;

	do
	{
		if (nArrayIndex >= 48)
			break;
		pRoomStatus->GameRecordArrary[nArrayIndex].bAreaWin[0] = m_GameRecordArrary[nIndex].bDragon;
		pRoomStatus->GameRecordArrary[nArrayIndex].bAreaWin[1] = m_GameRecordArrary[nIndex].bTiger;
		pRoomStatus->GameRecordArrary[nArrayIndex].bAreaWin[2] = m_GameRecordArrary[nIndex].bPing;

		nArrayIndex++;
		nIndex = (nIndex + 1) % MAX_SCORE_HISTORY;
	} while (nIndex != nEndIndex);
}

//游戏消息处理
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_PLACE_JETTON:		//用户加注
		{
			//效验数据
			ASSERT(wDataSize == sizeof(CMD_C_PlaceBet));
			if (wDataSize!=sizeof(CMD_C_PlaceBet)) return true;

			//消息处理
			CMD_C_PlaceBet * pPlaceBet = (CMD_C_PlaceBet *)pData;

			return OnUserPlayBet(pIServerUserItem->GetChairID(), pPlaceBet->cbBetArea, pPlaceBet->lBetScore);
		}
	case SUB_C_APPLY_BANKER:		//申请做庄
		{
			return OnUserApplyBanker(pIServerUserItem);
		}
	case SUB_C_CANCEL_BANKER:		//取消做庄
		{
			return OnUserCancelBanker(pIServerUserItem);
		}
	case SUB_C_ONLINE_PLAYER:		//获取用户列表
		{
			return OnUserRequestUserList(pIServerUserItem->GetChairID());
		}
	case SUB_C_REQUEST_CONTROL:		//控制信息请求
		{
			//权限判断
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
					CopyMemory(ControlInfo.lUserJettonScore[cbIndex], m_lPlayBet[i], sizeof(ControlInfo.lUserJettonScore[cbIndex]));
					lstrcpyn(ControlInfo.szNickName[cbIndex], pServerUser->GetNickName(), sizeof(ControlInfo.szNickName[cbIndex]));

					for (BYTE j = 0; j < AREA_MAX; j++)
					{
						lUserBetAllScore[cbIndex] += m_lPlayBet[i][j];
						ControlInfo.lAreaJettonScore[j] += m_lPlayBet[i][j];
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
	case SUB_C_CONTROL:				//本局控制输赢
		{
			//权限判断
			if (CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) == false)
				return true;

			CMD_C_ControlWinLost *pControlWinLost = (CMD_C_ControlWinLost *)pData;

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
	case SUB_C_CANCEL_CONTROL:		//取消本局控制
		{
			//权限判断
			if (CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) == false)
				return true;

			m_cbJuControl = FALSE;
			m_cbJuControlTimes = 0;
			ZeroMemory(m_cbJuControlArea, sizeof(m_cbJuControlArea));

			return true;
		}
	}

	return false;
}

//框架消息处理
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	
	return false;
}

//用户坐下
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//起始分数
	m_lUserStartScore[wChairID] = pIServerUserItem->GetUserScore();

	//设置时间
	if ((bLookonUser == false)&&(m_dwBetTime == 0L))
	{
		m_dwBetTime=(DWORD)time(NULL);
		m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,NULL);
		m_pITableFrame->SetGameStatus(GAME_SCENE_FREE);
	}



	return true;
}

//用户起来
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//起始分数
	m_lUserStartScore[wChairID] = 0;

	ZeroMemory(m_bUserListWin[wChairID], sizeof(m_bUserListWin[wChairID]));
	ZeroMemory(m_lUserListScore[wChairID], sizeof(m_lUserListScore[wChairID]));
	m_cbUserPlayCount[wChairID] = 0;

	//记录成绩
	if (bLookonUser == false)
	{
		//切换庄家
		if (wChairID == m_wCurrentBanker) ChangeBanker(true);

		//取消申请
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			if (wChairID!=m_ApplyUserArray[i]) continue;

			//删除玩家
			m_ApplyUserArray.RemoveAt(i);

			//构造变量
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//设置变量
			CancelBanker.wCancelUser = pIServerUserItem->GetChairID();

			//发送消息
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

			break;
		}

		return true;
	}

	return true;
}

//加注事件
bool CTableFrameSink::OnUserPlayBet(WORD wChairID, BYTE cbBetArea, SCORE lBetScore)
{
	//效验参数
	ASSERT((cbBetArea < AREA_MAX) && (lBetScore>0L));
	if ((cbBetArea >= AREA_MAX) || (lBetScore <= 0L))
	{
		CMD_S_PlaceBetFail PlaceBetFail;
		_sntprintf(PlaceBetFail.szBuffer, sizeof(PlaceBetFail.szBuffer), TEXT("下注信息错误！"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceBetFail, sizeof(PlaceBetFail));
		return true;
	}

	//效验状态
	if (m_pITableFrame->GetGameStatus() != GAME_SCENE_BET)
	{
		CMD_S_PlaceBetFail PlaceBetFail;
		_sntprintf(PlaceBetFail.szBuffer, sizeof(PlaceBetFail.szBuffer), TEXT("当前不是下注状态，不能下注！"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceBetFail, sizeof(PlaceBetFail));
		return true;
	}

	//庄家判断
	if (m_wCurrentBanker == wChairID)
	{
		CMD_S_PlaceBetFail PlaceBetFail;
		_sntprintf(PlaceBetFail.szBuffer, sizeof(PlaceBetFail.szBuffer), TEXT("庄家不能下注！"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceBetFail, sizeof(PlaceBetFail));
		return true;
	}
	if (m_bEnableSysBanker == false && m_wCurrentBanker == INVALID_CHAIR)
	{
		CMD_S_PlaceBetFail PlaceBetFail;
		_sntprintf(PlaceBetFail.szBuffer, sizeof(PlaceBetFail.szBuffer), TEXT("该房间系统坐庄不能下注！"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceBetFail, sizeof(PlaceBetFail));
		return true;
	}
	
	//庄家积分
	SCORE lBankerScore = 0;
	if(INVALID_CHAIR != m_wCurrentBanker)
	{
		IServerUserItem * pIServerUserItemBanker = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		lBankerScore = pIServerUserItemBanker->GetUserScore();
	}

	//变量定义
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	SCORE lUserScore = pIServerUserItem->GetUserScore();
	SCORE lBetCount = 0;
	for ( int i = 0; i < AREA_MAX; ++i )
	{
		lBetCount += m_lPlayBet[wChairID][i];
	}
	
	SCORE lAllBetCount = 0L;
	for (int nAreaIndex=0; nAreaIndex < AREA_MAX; ++nAreaIndex)
	{
		lAllBetCount += m_lAllBet[nAreaIndex];
	}

	//成功标识
	bool bPlaceBetSuccess=true;

	//合法校验
	if (lUserScore < lBetCount + lBetScore)
	{
		CMD_S_PlaceBetFail PlaceBetFail;
		_sntprintf(PlaceBetFail.szBuffer, sizeof(PlaceBetFail.szBuffer), TEXT("您的金币不足！"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceBetFail, sizeof(PlaceBetFail));
		return true;
	}
	else
	{
		//机器人验证
		if(pIServerUserItem->IsAndroidUser())
		{
			//区域限制
			if (m_lRobotAreaScore[cbBetArea] + lBetScore > m_lRobotAreaLimit)
				return true;

			//数目限制
			bool bHaveChip = false;
			bHaveChip = (lBetCount>0);

			//统计分数
			if (bPlaceBetSuccess)
				m_lRobotAreaScore[cbBetArea] += lBetScore;
		}
	}

	if (m_lUserLimitScore < lBetCount + lBetScore)
	{
		CMD_S_PlaceBetFail PlaceBetFail;
		_sntprintf(PlaceBetFail.szBuffer, sizeof(PlaceBetFail.szBuffer), TEXT("您的下注已到达限制！"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceBetFail, sizeof(PlaceBetFail));
		return true;
	}


	//合法验证
	if ( GetMaxPlayerScore(cbBetArea, wChairID) >= lBetScore )
	{
		m_lAllBet[cbBetArea] += lBetScore;
		m_lPlayBet[wChairID][cbBetArea]  += lBetScore;
	}
	else
	{
		bPlaceBetSuccess = false;
	}

	if (bPlaceBetSuccess)
	{
		//变量定义
		CMD_S_PlaceBet PlaceBet;
		ZeroMemory(&PlaceBet,sizeof(PlaceBet));

		//构造变量
		PlaceBet.wChairID=wChairID;
		PlaceBet.cbBetArea=cbBetArea;
		PlaceBet.lBetScore=lBetScore;

		SCORE lPlayerTableScore = m_lPlayBet[wChairID][0] + m_lPlayBet[wChairID][1] + m_lPlayBet[wChairID][2];

		PlaceBet.lPlayerRestScore = pIServerUserItem->GetUserScore() - lPlayerTableScore;

		WORD wSeatUser[MAX_SEAT_COUNT] = { 0 };
		FillMemory(wSeatUser, sizeof(wSeatUser), INVALID_CHAIR);
		OnGetUserListGameID(wSeatUser);

		for (WORD i = 0; i < GAME_PLAYER; i++)
		{
			bool bIsSeatUser = false;
			for (WORD j = 0; j < MAX_SEAT_COUNT; j++)
			{
				if (wSeatUser[j] == wChairID)
					bIsSeatUser = true;
			}
			//发送消息
			if (bIsSeatUser)
				m_pITableFrame->SendTableData(i, SUB_S_SEAT_JETTON, &PlaceBet, sizeof(PlaceBet));
			else if (i == wChairID)
				m_pITableFrame->SendTableData(i, SUB_S_PLACE_JETTON, &PlaceBet, sizeof(PlaceBet));
			else
				m_pITableFrame->SendTableData(i, SUB_S_OTHER_JETTON, &PlaceBet, sizeof(PlaceBet));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_PLACE_JETTON, &PlaceBet, sizeof(PlaceBet));

		//下注总额超过庄家能支付上限时提前结束游戏
		SCORE lBankerMaxScore = 100000000;
		if (INVALID_CHAIR != m_wCurrentBanker)
		{
			lBankerMaxScore = lBankerScore;
		}
		if (!bGetBankerCanPay(lBankerMaxScore))
		{
			if (m_pITableFrame->GetGameStatus() != GAME_SCENE_END)
			{
				//设置状态
				m_pITableFrame->SetGameStatus(GAME_SCENE_END);

				DispatchTableCard();

				m_cbEarlyOpen = TRUE;

				//结束游戏
				OnEventGameConclude(INVALID_CHAIR, NULL, GER_NORMAL);

				//设置时间
				m_dwBetTime = (DWORD)time(NULL);
				m_pITableFrame->SetGameTimer(IDI_GAME_END, m_cbEndTime * 1000, 1, 0L);
			}
		}
	}
	else
	{
		CMD_S_PlaceBetFail PlaceBetFail;
		_sntprintf(PlaceBetFail.szBuffer, sizeof(PlaceBetFail.szBuffer),TEXT("区域下注已到达限制！"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceBetFail, sizeof(PlaceBetFail));
	}

	return true;
}

//发送扑克
bool CTableFrameSink::DispatchTableCard()
{
	//随机扑克
	m_GameLogic.RandCardList(&m_cbTableCardArray[0],sizeof(m_cbTableCardArray)/sizeof(m_cbTableCardArray[0]));
	
	//首次发牌
	m_cbCardCount[INDEX_PLAYER] = 1;
	m_cbCardCount[INDEX_BANKER] = 1;
	

	return true;
}

//申请庄家
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	//合法判断
	SCORE lUserScore = pIApplyServerUserItem->GetUserScore();
	if ( lUserScore < m_lApplyBankerCondition )
	{
		CMD_S_ApplyFailed ApplyFailed;
		_sntprintf(ApplyFailed.szBuffer, sizeof(ApplyFailed.szBuffer),TEXT("你的金币不足以申请庄家，申请失败！"));
		m_pITableFrame->SendTableData(pIApplyServerUserItem->GetChairID(), SUB_S_APPLEY_BANKER_FAILURE, &ApplyFailed, sizeof(ApplyFailed));
		return true;
	}

	//存在判断
	WORD wApplyUserChairID = pIApplyServerUserItem->GetChairID();
	for (INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID = m_ApplyUserArray[nUserIdx];
		if (wChairID == wApplyUserChairID)
		{
			CMD_S_ApplyFailed ApplyFailed;
			_sntprintf(ApplyFailed.szBuffer, sizeof(ApplyFailed.szBuffer), TEXT("你已经申请了庄家，不需要再次申请！"));
			m_pITableFrame->SendTableData(pIApplyServerUserItem->GetChairID(), SUB_S_APPLEY_BANKER_FAILURE, &ApplyFailed, sizeof(ApplyFailed));
			return true;
		}
	}
	
	if (pIApplyServerUserItem->IsAndroidUser()&&(m_ApplyUserArray.GetCount())>m_nRobotListMaxCount)
	{
		return true;
	}

	if (m_ApplyUserArray.GetCount() >= 5)
	{
		CMD_S_ApplyFailed ApplyFailed;
		_sntprintf(ApplyFailed.szBuffer, sizeof(ApplyFailed.szBuffer), TEXT("上庄人数已满，不能申请庄家！"));
		m_pITableFrame->SendTableData(pIApplyServerUserItem->GetChairID(), SUB_S_APPLEY_BANKER_FAILURE, &ApplyFailed, sizeof(ApplyFailed));
		return true;
	}

	//保存信息 
	m_ApplyUserArray.Add(wApplyUserChairID);

	//构造变量
	CMD_S_ApplyBanker ApplyBanker;
	ZeroMemory(&ApplyBanker,sizeof(ApplyBanker));

	//设置变量
	ApplyBanker.wApplyUser = wApplyUserChairID;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

	//切换判断
	if (m_pITableFrame->GetGameStatus() == GAME_SCENE_FREE && m_ApplyUserArray.GetCount() == 1)
	{
		ChangeBanker(false);
	}

	return true;
}

//取消申请
bool CTableFrameSink::OnUserCancelBanker(IServerUserItem *pICancelServerUserItem)
{
	//当前庄家
	if (pICancelServerUserItem->GetChairID() == m_wCurrentBanker && m_pITableFrame->GetGameStatus()!=GAME_SCENE_FREE)
	{
		//发送消息
		m_pITableFrame->SendGameMessage(pICancelServerUserItem,TEXT("游戏已经开始，不可以取消当庄！"),SMT_CHAT|SMT_EJECT);
		return true;
	}
	
	bool bValidQuitBanker = true;
	//取消上庄列表上的（取消申请）
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		//获取玩家
		WORD wChairID=m_ApplyUserArray[i];
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);

		//条件过滤
		if (pIServerUserItem == NULL) continue;
		if (pIServerUserItem->GetUserID()!=pICancelServerUserItem->GetUserID()) continue;

		//删除玩家
		m_ApplyUserArray.RemoveAt(i);

		bValidQuitBanker = false;

		if (m_wCurrentBanker!=wChairID)
		{
			//构造变量
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//设置变量
			CancelBanker.wCancelUser = wChairID;

			//发送消息
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		}
		else if (m_wCurrentBanker == wChairID)
		{
			//切换庄家 
			m_wCurrentBanker=INVALID_CHAIR;
			ChangeBanker(true);
		}

		return true;
	}

	if (bValidQuitBanker == true)
	{
		m_wCurrentBanker=INVALID_CHAIR;
		ChangeBanker(true);
		return true;
	}

	return false;
}

//更换庄家
bool CTableFrameSink::ChangeBanker(bool bCancelCurrentBanker)
{
	//切换标识
	bool bChangeBanker=false;

	//取消当前
	if (bCancelCurrentBanker)
	{
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			//获取玩家
			WORD wChairID=m_ApplyUserArray[i];

			//条件过滤
			if (wChairID!=m_wCurrentBanker) continue;

			//删除玩家
			m_ApplyUserArray.RemoveAt(i);

			break;
		}

		//设置庄家
		m_wCurrentBanker=INVALID_CHAIR;

		//轮换判断
		TakeTurns();


		//设置变量
		bChangeBanker=true;
		m_bExchangeBanker = true;
	}
	//轮庄判断
	else if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//获取庄家
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if(pIServerUserItem)
		{
			SCORE lBankerScore = pIServerUserItem->GetUserScore();

			//次数判断
			if (m_lPlayerBankerMAX<=m_wBankerTime || lBankerScore<m_lApplyBankerCondition)
			{
				//庄家增加判断 同一个庄家情况下只判断一次
				if(m_lPlayerBankerMAX <= m_wBankerTime && m_bExchangeBanker && lBankerScore>=m_lApplyBankerCondition)
				{
					//加庄局数设置：当庄家坐满设定的局数之后(m_nBankerTimeLimit)，
					//所带金币值还超过下面申请庄家列表里面所有玩家金币时，
					//可以再加坐庄m_nBankerTimeAdd局，加庄局数可设置。

					//金币超过m_lExtraBankerScore之后，
					//就算是下面玩家的金币值大于他的金币值，他也可以再加庄m_nExtraBankerTime局。
					bool bScoreMAX = true;
					m_bExchangeBanker = false;

					for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
					{
						//获取玩家
						WORD wChairID = m_ApplyUserArray[i];
						IServerUserItem *pIUserItem = m_pITableFrame->GetTableUserItem(wChairID);
						SCORE lScore = pIServerUserItem->GetUserScore();

						if ( wChairID != m_wCurrentBanker && lBankerScore <= lScore )
						{
							bScoreMAX = false;
							break;
						}
					}

					if ( bScoreMAX || (lBankerScore > m_lExtraBankerScore && m_lExtraBankerScore != 0l) )
					{
						if ( bScoreMAX )
						{
							m_lPlayerBankerMAX += m_nBankerTimeAdd;
						}
						if ( lBankerScore > m_lExtraBankerScore && m_lExtraBankerScore != 0l )
						{
							m_lPlayerBankerMAX += m_nExtraBankerTime;
						}
						return true;
					}
				}

				//设置庄家
				m_wCurrentBanker=INVALID_CHAIR;

				//轮换判断
				TakeTurns();
					
				//撤销玩家
				for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
				{
					//获取玩家
					WORD wChairID=m_ApplyUserArray[i];

					//条件过滤
					if (wChairID!=m_wCurrentBanker) continue;

					//删除玩家
					m_ApplyUserArray.RemoveAt(i);

					break;
				}

				bChangeBanker=true;
				m_bExchangeBanker = true;

				//提示消息
				TCHAR szTipMsg[128];
				if (lBankerScore<m_lApplyBankerCondition)
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]分数少于(%I64d)，强行换庄!"),pIServerUserItem->GetNickName(),m_lApplyBankerCondition);
				else
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]做庄次数达到(%d)，强行换庄!"),pIServerUserItem->GetNickName(),m_lPlayerBankerMAX);

				//发送消息
				SendGameMessage(INVALID_CHAIR,szTipMsg);	
			}
		}
		
	}
	//系统做庄
	else if (m_wCurrentBanker == INVALID_CHAIR && m_ApplyUserArray.GetCount()!=0)
	{
		//轮换判断
		TakeTurns();
		
		//撤销玩家
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			//获取玩家
			WORD wChairID=m_ApplyUserArray[i];

			//条件过滤
			if (wChairID!=m_wCurrentBanker) continue;

			//删除玩家
			m_ApplyUserArray.RemoveAt(i);

			break;
		}

		bChangeBanker=true;
		m_bExchangeBanker = true;
	}

	//切换判断
	if (bChangeBanker)
	{
		//最大坐庄数
		m_lPlayerBankerMAX = m_nBankerTimeLimit;

		//设置变量
		m_wBankerTime = 0;
		m_lBankerWinScore=0;

		//发送消息
		CMD_S_ChangeBanker stChangeBanker;
		ZeroMemory(&stChangeBanker,sizeof(stChangeBanker));
		stChangeBanker.wBankerUser = m_wCurrentBanker;
		if (m_wCurrentBanker != INVALID_CHAIR)
		{
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
			stChangeBanker.lBankerScore = pIServerUserItem->GetUserScore();
		}
		else
		{
			stChangeBanker.lBankerScore = 100000000;
		}

		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&stChangeBanker,sizeof(CMD_S_ChangeBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&stChangeBanker,sizeof(CMD_S_ChangeBanker));
	}

	return bChangeBanker;
}

//轮换判断
void CTableFrameSink::TakeTurns()
{
	//变量定义
	int nInvalidApply = 0;

	for (int i = 0; i < m_ApplyUserArray.GetCount(); i++)
	{
		if (m_pITableFrame->GetGameStatus() == GAME_SCENE_FREE)
		{
			//获取分数
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(m_ApplyUserArray[i]);
			if (pIServerUserItem != NULL)
			{
				if (pIServerUserItem->GetUserScore() >= m_lApplyBankerCondition)
				{
					m_wCurrentBanker = m_ApplyUserArray[i];
					break;
				}
				else
				{
					nInvalidApply = i + 1;

					//发送消息
					CMD_S_CancelBanker CancelBanker = {};

					//设置变量
					CancelBanker.wCancelUser = pIServerUserItem->GetChairID();

					//发送消息
					m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
					m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
				}
			}
		}
	}

	//删除玩家
	if (nInvalidApply != 0)
		m_ApplyUserArray.RemoveAt(0, nInvalidApply);
}

//发送庄家
void CTableFrameSink::SendApplyUser( IServerUserItem *pRcvServerUserItem )
{
	for (INT_PTR nUserIdx = 0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID = m_ApplyUserArray[nUserIdx];

		//获取玩家
		IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
		if (!pServerUserItem) continue;

		//庄家判断
		if (pServerUserItem->GetChairID() == m_wCurrentBanker) continue;

		//构造变量
		CMD_S_ApplyBanker ApplyBanker;
		ApplyBanker.wApplyUser = pServerUserItem->GetChairID();

		//发送消息
		m_pITableFrame->SendUserItemData(pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	}
}

//用户断线
bool CTableFrameSink::OnActionUserOffLine( WORD wChairID, IServerUserItem * pIServerUserItem) 
{
	//离线庄家
	if (wChairID == m_wCurrentBanker && pIServerUserItem->IsAndroidUser() == false)
	{
		m_wOfflineBanker = wChairID;
	}

	//切换庄家
	if (wChairID == m_wCurrentBanker) ChangeBanker(true);

	//取消申请
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		if (wChairID!=m_ApplyUserArray[i]) continue;

		//删除玩家
		m_ApplyUserArray.RemoveAt(i);

		//构造变量
		CMD_S_CancelBanker CancelBanker;
		ZeroMemory(&CancelBanker,sizeof(CancelBanker));

		//设置变量
		CancelBanker.wCancelUser = wChairID;

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

		break;
	}

	return true;
}

//最大下注
SCORE CTableFrameSink::GetMaxPlayerScore(BYTE cbBetArea, WORD wChairID)
{
	//获取玩家
	IServerUserItem *pIMeServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
	if ( NULL == pIMeServerUserItem ) 
		return 0L;

	if ( cbBetArea >= AREA_MAX )
		return 0L;

	//已下注额
	SCORE lNowBet = 0;
	for (int nAreaIndex = 0; nAreaIndex < AREA_MAX; ++nAreaIndex ) 
		lNowBet += m_lPlayBet[wChairID][nAreaIndex];

	//最大下注
	SCORE lMaxBet = 0;

	//最大下注
	lMaxBet = min(pIMeServerUserItem->GetUserScore() - lNowBet, m_lUserLimitScore - m_lPlayBet[wChairID][cbBetArea]);
	lMaxBet = min( lMaxBet, m_lAreaLimitScore - m_lAllBet[cbBetArea]);

	//非零限制
	ASSERT(lMaxBet >= 0);
	lMaxBet = max(lMaxBet, 0);

	return lMaxBet;
}


//计算得分
SCORE CTableFrameSink::CalculateScore(OUT SCORE& lBankerWinScore, OUT tagServerGameRecord& GameRecord)
{
	//计算牌点
	BYTE cbPlayerCount = 1;
	BYTE cbBankerCount = 1;

	//系统输赢
	SCORE lSystemScore = 0l;

	//推断玩家
	BYTE cbWinArea[AREA_MAX] = {FALSE};
	DeduceWinner(cbWinArea);

	//游戏记录
	GameRecord.bDragon = cbWinArea[AREA_DRAGON];
	GameRecord.bPing = cbWinArea[AREA_PING];
	GameRecord.bTiger = cbWinArea[AREA_TIGER];

	if (cbWinArea[AREA_DRAGON] == TRUE)
		m_cbOpenResult = AREA_DRAGON;
	else if (cbWinArea[AREA_PING] == TRUE)
		m_cbOpenResult = AREA_PING;
	else
		m_cbOpenResult = AREA_TIGER;

	//玩家成绩
	SCORE lUserLostScore[GAME_PLAYER];
	ZeroMemory(m_lPlayScore, sizeof(m_lPlayScore));
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	//区域倍率
	BYTE cbMultiple[AREA_MAX] = { MULTIPLE_XIAN, MULTIPLE_ZHUANG, MULTIPLE_PING };

	BYTE pingPoint = 0;
	BYTE pingType = 0;

	if (cbPlayerCount == 7 || cbBankerCount == 7 )
	{
		pingPoint = 10;
		pingType = 7;
	}
	else if (cbPlayerCount == 6 || cbBankerCount == 6)
	{
		pingPoint = 7;
		pingType = 6;
	}
	else if (cbPlayerCount == 5 || cbBankerCount == 5)
	{
		pingPoint = 4;
		pingType = 5;
	}
	else if (cbPlayerCount == 4 || cbBankerCount == 4)
	{
		pingPoint = 3;
		pingType = 4;
	}
	else if (cbPlayerCount == 3 || cbBankerCount == 3)
	{
		pingPoint = 2;
		pingType = 3;
	}
	else if (cbPlayerCount == 2 || cbBankerCount == 2)
	{
		pingType = 2;
	}
	else
	{
		pingType = 1;
	}

	//庄家是不是机器人
	bool bIsBankerAndroidUser = false;
	if ( m_wCurrentBanker != INVALID_CHAIR )
	{
		IServerUserItem * pIBankerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if (pIBankerUserItem != NULL) 
		{
			bIsBankerAndroidUser = pIBankerUserItem->IsAndroidUser();
		}
	}

	//计算积分
	for (WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
	{
		//庄家判断
		if ( m_wCurrentBanker == wChairID ) continue;

		//获取用户
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem == NULL) continue;

		bool bIsAndroidUser = pIServerUserItem->IsAndroidUser();

		for ( WORD wAreaIndex = 0; wAreaIndex < AREA_MAX; ++wAreaIndex )
		{
			if( cbWinArea[wAreaIndex] == TRUE )
			{
				if (cbWinArea[AREA_DRAGON] == TRUE)
				{
					m_lUserWinScore[wChairID] += (m_lPlayBet[wChairID][AREA_DRAGON] * (2 - 1));
					m_lPlayScore[wChairID][AREA_DRAGON] += (m_lPlayBet[wChairID][AREA_DRAGON] * (2 - 1));
					lBankerWinScore -= (m_lPlayBet[wChairID][AREA_DRAGON] * (2 - 1));
				}
				else	if (cbWinArea[AREA_TIGER] == TRUE)
				{
					m_lUserWinScore[wChairID] += (m_lPlayBet[wChairID][AREA_TIGER] * (2 - 1));
					m_lPlayScore[wChairID][AREA_TIGER] += (m_lPlayBet[wChairID][AREA_TIGER] * (2 - 1));
					lBankerWinScore -= (m_lPlayBet[wChairID][AREA_TIGER] * (2 - 1));

				}

				if ( cbWinArea[AREA_PING] == TRUE)
				{
					m_lUserWinScore[wChairID] += (m_lPlayBet[wChairID][wAreaIndex] * (MULTIPLE_PING - 1));
					m_lPlayScore[wChairID][wAreaIndex] += (m_lPlayBet[wChairID][wAreaIndex] * (MULTIPLE_PING - 1));
					lBankerWinScore -= (m_lPlayBet[wChairID][wAreaIndex] * (MULTIPLE_PING - 1));
				}

				//系统或者机器人庄
				if (m_wCurrentBanker == INVALID_CHAIR || bIsBankerAndroidUser)
				{
					if (!bIsAndroidUser)
						lSystemScore -= m_lPlayBet[wChairID][wAreaIndex];
				}
				//玩家庄
				else
				{
					if (bIsAndroidUser)
						lSystemScore += m_lPlayBet[wChairID][wAreaIndex];
				}
					
			}
			else
			{
				SCORE lBetScore = m_lPlayBet[wChairID][wAreaIndex];

				if (wAreaIndex != AREA_PING && cbWinArea[AREA_PING] == TRUE)
					lBetScore = 0;

				lUserLostScore[wChairID] -= lBetScore;
				m_lPlayScore[wChairID][wAreaIndex] -= lBetScore;
				lBankerWinScore += lBetScore;

				//系统得分
				//系统或者机器人庄
				if (m_wCurrentBanker == INVALID_CHAIR || bIsBankerAndroidUser)
				{
					if (!bIsAndroidUser)
						lSystemScore += lBetScore;
				}
				//玩家庄
				else
				{
					if (bIsAndroidUser)
						lSystemScore -= lBetScore;
				}

				//庄家得分
				if ( m_wCurrentBanker != INVALID_CHAIR && m_wCurrentBanker != wChairID )
					m_lPlayScore[m_wCurrentBanker][wAreaIndex] += lBetScore;
			}
		}

		//总的分数
		m_lUserWinScore[wChairID] += lUserLostScore[wChairID];

		//计算税收
		if ( m_lUserWinScore[wChairID] > 0 )
		{
			float fRevenuePer = float(m_pGameServiceOption->wRevenueRatio/1000.0);
			m_lUserRevenue[wChairID] = SCORE(m_lUserWinScore[wChairID] * fRevenuePer);
			m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];
		}
	}

	//庄家成绩
	if (m_wCurrentBanker != INVALID_CHAIR)
	{
		m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;

		//计算税收
		if (0 < m_lUserWinScore[m_wCurrentBanker])
		{
			float fRevenuePer = float(m_pGameServiceOption->wRevenueRatio/1000.0);
			m_lUserRevenue[m_wCurrentBanker] = SCORE(m_lUserWinScore[m_wCurrentBanker] * fRevenuePer);
			m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
			lBankerWinScore = m_lUserWinScore[m_wCurrentBanker];
		}		
	}
	
	return lSystemScore;
}

//游戏结束计算
SCORE CTableFrameSink::GameOver()
{
	//定义变量
	SCORE lBankerWinScore = 0;
	bool bSuccess = false;

	//游戏记录
	tagServerGameRecord& GameRecord = m_GameRecordArrary[m_nRecordLast];

	tag_LongHu_Parameter LongHu_Parameter;
	ZeroMemory(&LongHu_Parameter, sizeof(LongHu_Parameter));

	LongHu_Parameter.wBankerUser = m_wCurrentBanker;
	LongHu_Parameter.cbJuControl = m_cbJuControl;
	CopyMemory(LongHu_Parameter.cbJuControlArea, m_cbJuControlArea, sizeof(LongHu_Parameter.cbJuControlArea));
	CopyMemory(LongHu_Parameter.lPlayBet, m_lPlayBet, sizeof(LongHu_Parameter.lPlayBet));

	tag_LongHu_Result pLongHu_Result;
	ZeroMemory(&pLongHu_Result, sizeof(pLongHu_Result));
	m_pITableFrame->GetControlResult(&LongHu_Parameter, sizeof(LongHu_Parameter), &pLongHu_Result);

	if (pLongHu_Result.cbTableCardArray[0] != 0 && pLongHu_Result.cbTableCardArray[1] != 0)
	{
		CopyMemory(m_cbTableCardArray, pLongHu_Result.cbTableCardArray, sizeof(m_cbTableCardArray));
		m_cbCardCount[0] = 1;
		m_cbCardCount[1] = 1;
	}
	else
		DispatchTableCard();

	if (m_cbJuControlTimes > 0)
		m_cbJuControlTimes--;
	if (m_cbJuControlTimes == 0)
	{
		m_cbJuControl = FALSE;
		ZeroMemory(m_cbJuControlArea, sizeof(m_cbJuControlArea));
	}

	CalculateScore(lBankerWinScore, GameRecord);

	//累计积分
	m_lBankerWinScore += lBankerWinScore;

	//当前积分
	m_lBankerCurGameScore = lBankerWinScore;

	//移动下标
	m_nRecordLast = (m_nRecordLast + 1) % MAX_SCORE_HISTORY;
	if (m_nRecordLast == m_nRecordFirst)
	{
		m_nRecordLast = m_nRecordFirst;
		ZeroMemory(m_GameRecordArrary, sizeof(m_GameRecordArrary));
	}

	return lBankerWinScore;
}

//推断赢家
void CTableFrameSink::DeduceWinner(BYTE* pWinArea)
{
	BYTE cbWin = m_GameLogic.CompareCard(m_cbTableCardArray[INDEX_PLAYER], m_cbTableCardArray[INDEX_BANKER], MAX_COUNT);

	if (cbWin == 1)
		pWinArea[AREA_DRAGON] = TRUE;
	else if (cbWin == 2)
		pWinArea[AREA_TIGER] = TRUE;
	else
		pWinArea[AREA_PING] = TRUE;

}

//判断庄家最大赔付额
bool CTableFrameSink::bGetBankerCanPay(SCORE lBankerScore)
{
	SCORE lBankerGrade = 0;


	for (BYTE wAreaIndex = 0; wAreaIndex < AREA_MAX; wAreaIndex++)
	{
		SCORE lBankerWinScore = 0;
		for (WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
		{
			//庄家判断
			if (m_wCurrentBanker == wChairID) continue;

			//获取用户
			IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
			if (pIServerUserItem == NULL) continue;

			if (wAreaIndex == AREA_DRAGON)
			{
				lBankerWinScore -= (m_lPlayBet[wChairID][AREA_DRAGON] * (2 - 1));

				lBankerWinScore += m_lPlayBet[wChairID][AREA_TIGER];
				lBankerWinScore += m_lPlayBet[wChairID][AREA_PING];
			}
			else if (wAreaIndex == AREA_TIGER)
			{
				lBankerWinScore -= (m_lPlayBet[wChairID][AREA_TIGER] * (2 - 1));

				lBankerWinScore += m_lPlayBet[wChairID][AREA_DRAGON];
				lBankerWinScore += m_lPlayBet[wChairID][AREA_PING];
			}
			else if (wAreaIndex == AREA_PING)
			{
				lBankerWinScore -= (m_lPlayBet[wChairID][wAreaIndex] * (MULTIPLE_PING - 1));
			}
		}

		if (lBankerGrade > lBankerWinScore)
			lBankerGrade = lBankerWinScore;
	}
	if ((lBankerGrade + lBankerScore) <= 0)
		return false;
	return true;
}

//发送记录
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

//发送消息
void CTableFrameSink::SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg)
{
	if (wChairID == INVALID_CHAIR)
	{
		//游戏玩家
		for (WORD i=0; i<GAME_PLAYER; ++i)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
			if (pIServerUserItem == NULL) continue;
			m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_CHAT);
		}

		//旁观玩家
		WORD wIndex=0;
		do {
			IServerUserItem *pILookonServerUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
			if (pILookonServerUserItem == NULL) break;

			m_pITableFrame->SendGameMessage(pILookonServerUserItem,pszTipMsg,SMT_CHAT);

		}while(true);
	}
	else
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem!=NULL) m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_CHAT|SMT_EJECT);
	}
}

//读取配置
void CTableFrameSink::ReadConfigInformation()
{
	//获取自定义配置
	tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pGameServiceOption->cbCustomRule;
	ASSERT(pCustomConfig);

	//上庄
	m_lApplyBankerCondition = pCustomConfig->CustomGeneral.lApplyBankerCondition;
	m_nBankerTimeLimit = pCustomConfig->CustomGeneral.lBankerTime;
	m_nBankerTimeAdd = pCustomConfig->CustomGeneral.lBankerTimeAdd;
	m_lExtraBankerScore = pCustomConfig->CustomGeneral.lBankerScoreMAX;
	m_nExtraBankerTime = pCustomConfig->CustomGeneral.lBankerTimeExtra;
	//m_bEnableSysBanker = (pCustomConfig->CustomGeneral.nEnableSysBanker == TRUE)?true:false;
	m_bEnableSysBanker = (pCustomConfig->CustomGeneral.nEnableSysBanker == TRUE)?true:true;

	//时间	
	m_cbFreeTime = pCustomConfig->CustomGeneral.lFreeTime;
	m_cbBetTime = pCustomConfig->CustomGeneral.lBetTime;
	m_cbEndTime = pCustomConfig->CustomGeneral.lEndTime;
	if(m_cbFreeTime < TIME_FREE	|| m_cbFreeTime > 99) m_cbFreeTime = TIME_FREE;
	if(m_cbBetTime < TIME_PLACE_JETTON || m_cbBetTime > 99) m_cbBetTime = TIME_PLACE_JETTON;
	if(m_cbEndTime < TIME_GAME_END || m_cbEndTime > 99) m_cbEndTime = TIME_GAME_END;

	//下注
	m_lAreaLimitScore = pCustomConfig->CustomGeneral.lAreaLimitScore;
	
	//机器人
	m_nRobotListMaxCount = pCustomConfig->CustomAndroid.lRobotListMaxCount;

	LONGLONG lRobotBetMinCount = pCustomConfig->CustomAndroid.lRobotBetMinCount;
	LONGLONG lRobotBetMaxCount = pCustomConfig->CustomAndroid.lRobotBetMaxCount;
	m_nMaxChipRobot = rand()%(lRobotBetMaxCount-lRobotBetMinCount+1) + lRobotBetMinCount;
	if (m_nMaxChipRobot < 0)	m_nMaxChipRobot = 8;
	m_lRobotAreaLimit = pCustomConfig->CustomAndroid.lRobotAreaLimit;
	
	return;
}

//查询是否扣服务费
bool CTableFrameSink::QueryBuckleServiceCharge( WORD wChairID )
{
	// 庄家判断
	if ( wChairID == m_wCurrentBanker )
	{
		for ( int i = 0; i < GAME_PLAYER; ++i )
		{
			for ( int j = 0; j < AREA_MAX; ++j )
			{
				if ( m_lPlayBet[i][j] != 0 )
					return true;
			}
		}
		return false;
	}

	// 一般玩家
	for ( int i = 0; i < AREA_MAX; ++i )
	{
		if ( m_lPlayBet[wChairID][i] != 0 )
			return true;
	}

	return false;
}

//积分事件
bool CTableFrameSink::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	//当庄家分数在空闲时间变动时(即庄家进行了存取款)校验庄家的上庄条件
	if(wChairID == m_wCurrentBanker && m_pITableFrame->GetGameStatus()==GAME_SCENE_FREE)
	{
		ChangeBanker(false);
	}

	return true;
}

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661//////////