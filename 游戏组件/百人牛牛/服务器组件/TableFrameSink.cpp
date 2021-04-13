#include "StdAfx.h"
#include "DlgCustomRule.h"
#include "TableFrameSink.h"
#include <locale>

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//常量定义
#define SEND_COUNT					100									//发送次数

//索引定义
#define INDEX_PLAYER				0									//闲家索引
#define INDEX_BANKER				1									//庄家索引

//时间定义
#define IDI_FREE					1									//空闲时间
#define IDI_PLACE_JETTON			2									//下注时间
#define IDI_GAME_END				3									//结束时间

#define TIME_FREE					1									//空闲时间
#define TIME_PLACE_JETTON			5									//下注时间
#define TIME_GAME_END				8									//结束时间
//////////////////////////////////////////////////////////////////////////

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//游戏人数

//////////////////////////////////////////////////////////////////////////
//构造函数
CTableFrameSink::CTableFrameSink()
{
	//起始分数
	ZeroMemory(m_lUserStartScore,sizeof(m_lUserStartScore));

	//总下注数
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//个人下注
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	ZeroMemory(m_bUserListWin, sizeof(m_bUserListWin));
	ZeroMemory(m_lUserListScore, sizeof(m_lUserListScore));
	ZeroMemory(m_cbUserPlayCount, sizeof(m_cbUserPlayCount));

	//控制变量
	m_cbWinSideControl=0;
	m_nSendCardCount=0;

	m_cbJuControl = FALSE;
	m_cbJuControlTimes = 0;
	ZeroMemory(m_cbJuControlArea, sizeof(m_cbJuControlArea));

	//玩家成绩	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//扑克信息
	ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

	//状态变量
	m_dwJettonTime=0L;
	m_bExchangeBanker=true;
	m_wAddTime=0;

	//庄家信息
	m_ApplyUserArray.RemoveAll();
	m_wCurrentBanker=INVALID_CHAIR;
	m_wOfflineBanker = INVALID_CHAIR;
	m_wBankerTime=0;
	m_lPlayerBankerMAX = 0l;
	m_lBankerWinScore=0L;		
	m_lBankerCurGameScore=0L;
	m_bEnableSysBanker=true;
	m_cbLeftCardCount=0;
	m_bContiueCard=false;
	m_lBankerScore = 0l;

	//记录变量
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;
	m_nRecordLast=0;
	m_dwRecordCount=0;

	//机器人控制
	m_nChipRobotCount = 0;
	m_nRobotListMaxCount =0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));
	
	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink()
{
}

VOID CTableFrameSink::Release()
{
}
//接口查询
VOID * CTableFrameSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//配置桌子
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);

	//错误判断
	if (m_pITableFrame==NULL)
	{
		CTraceService::TraceString(TEXT("游戏桌子 CTableFrameSink 查询 ITableFrame 接口失败"),TraceLevel_Exception);
		return false;
	}

	//开始模式
	m_pITableFrame->SetStartMode(START_MODE_TIME_CONTROL);

	m_pGameServiceOption = m_pITableFrame->GetGameServiceOption();
	m_pGameServiceAttrib = m_pITableFrame->GetGameServiceAttrib();

	ASSERT(m_pGameServiceOption!=NULL);
	ASSERT(m_pGameServiceAttrib!=NULL);
	//设置文件名
	TCHAR szPath[MAX_PATH] = TEXT("");
	GetCurrentDirectory(sizeof(szPath), szPath);
	_sntprintf(m_szConfigFileName, sizeof(m_szConfigFileName), TEXT("%s\\BaiRenConfig\\OxBattleConfig-%d-%d.ini"), szPath, m_pGameServiceOption->wServerID, m_pITableFrame->GetTableID());

	m_nChip[0] = GetPrivateProfileInt(TEXT("百人牛牛"), TEXT("nChip0"), 1, m_szConfigFileName);
	m_nChip[1] = GetPrivateProfileInt(TEXT("百人牛牛"), TEXT("nChip1"), 10, m_szConfigFileName);
	m_nChip[2] = GetPrivateProfileInt(TEXT("百人牛牛"), TEXT("nChip2"), 50, m_szConfigFileName);
	m_nChip[3] = GetPrivateProfileInt(TEXT("百人牛牛"), TEXT("nChip3"), 100, m_szConfigFileName);
	m_nChip[4] = GetPrivateProfileInt(TEXT("百人牛牛"), TEXT("nChip4"), 1000, m_szConfigFileName);

	m_lUserLimitScore = GetPrivateProfileInt(TEXT("百人牛牛"), TEXT("nLimitMax"), 20000, m_szConfigFileName);
	m_nMaxTimes = GetPrivateProfileInt(TEXT("百人牛牛"), TEXT("nMaxTimes"), 5, m_szConfigFileName);

	//房间名称
	memcpy(m_szRoomName, m_pGameServiceOption->szServerName, sizeof(m_pGameServiceOption->szServerName));

	ReadConfigInformation();
	return true;
}

//复位桌子
VOID CTableFrameSink::RepositionSink()
{
	m_wOfflineBanker = INVALID_CHAIR;

	//总下注数
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//个人下注
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//玩家成绩	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//机器人控制
	m_nChipRobotCount = 0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));
	
	m_lBankerScore = 0l;

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

//游戏开始
bool CTableFrameSink::OnEventGameStart()
{
	ZeroMemory(m_cbTableCardArray, sizeof(m_cbTableCardArray));

	ZeroMemory(m_cbOpenResult, sizeof(m_cbOpenResult));
	m_cbEarlyOpen = FALSE;

	DispatchTableCard();

	//获取庄家
	IServerUserItem* pIBankerServerUserItem = NULL;
	if ( m_wCurrentBanker == INVALID_CHAIR )
	{
		m_lBankerScore = 10000000;
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
	GameStart.cbTimeLeave=m_nPlaceJettonTime;

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
			tag_OxBattle_Parameter OxBattle_Parameter;
			ZeroMemory(&OxBattle_Parameter, sizeof(OxBattle_Parameter));

			OxBattle_Parameter.wBankerUser = m_wCurrentBanker;
			OxBattle_Parameter.cbJuControl = m_cbJuControl;
			CopyMemory(OxBattle_Parameter.cbJuControlArea, m_cbJuControlArea, sizeof(OxBattle_Parameter.cbJuControlArea));
			CopyMemory(OxBattle_Parameter.lPlayBet, m_lUserJettonScore, sizeof(OxBattle_Parameter.lPlayBet));
			CopyMemory(OxBattle_Parameter.cbTableCardArray, m_cbTableCardArray, sizeof(OxBattle_Parameter.cbTableCardArray));

			tag_OxBattle_Result pOxBattle_Result;
			ZeroMemory(&pOxBattle_Result, sizeof(pOxBattle_Result));
			m_pITableFrame->GetControlResult(&OxBattle_Parameter, sizeof(OxBattle_Parameter), &pOxBattle_Result);

			if (pOxBattle_Result.cbTableCardArray[0][2] != 0 && pOxBattle_Result.cbTableCardArray[1][2] != 0)
			{
				CopyMemory(m_cbTableCardArray, pOxBattle_Result.cbTableCardArray, sizeof(m_cbTableCardArray));
				m_cbCardCount[0] = 5;
				m_cbCardCount[1] = 5;
				m_cbCardCount[2] = 5;
				m_cbCardCount[3] = 5;
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

			//计算分数
			SCORE lBankerWinScore = CalculateScore();

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
				for (BYTE j = 1; j < AREA_MAX + 1; j++)
				{
					lAddScore += m_lUserJettonScore[i][j];
				}
				m_lUserListScore[i][m_cbUserPlayCount[i]] = lAddScore;
				m_cbUserPlayCount[i] ++;

				if (m_cbUserPlayCount[i] == USER_LIST_COUNT)
					m_cbUserPlayCount[i] = 0;
			}

			//占位玩家成绩
			WORD wSeatUser[MAX_SEAT_COUNT];
			FillMemory(wSeatUser, sizeof(wSeatUser), INVALID_CHAIR);
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
				GameEnd.lBankerRestScore = 10000000;
			else
			{
				IServerUserItem* pBankerUser = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				if (pBankerUser)
					GameEnd.lBankerRestScore = pBankerUser->GetUserScore() + lBankerWinScore;
			}

			//扑克信息
			CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
			for (int i = 0; i < AREA_MAX + 1; i++)
			{
				GameEnd.cbCardType[i] = m_GameLogic.GetCardType(GameEnd.cbTableCardArray[i], MAX_CARD);
			}
			CopyMemory(GameEnd.cbResult, m_cbOpenResult, sizeof(GameEnd.cbResult));
			GameEnd.cbEarlyOpenFlag = m_cbEarlyOpen;

			for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
				if ( pIServerUserItem == NULL ) continue;

				//设置成绩
				CopyMemory(GameEnd.lPlayAreaScore, &m_lUserJettonScore[wUserIndex][1], sizeof(GameEnd.lPlayAreaScore));
				GameEnd.lPlayerWinScore = m_lUserWinScore[wUserIndex];
				GameEnd.lPlayerRestScore = pIServerUserItem->GetUserScore() + GameEnd.lPlayerWinScore;

				//发送消息					
				m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				//库存金币
				if (!pIServerUserItem->IsAndroidUser())
					m_pGameServiceOption->pRoomStorageOption.lStorageCurrent -= m_lUserWinScore[wUserIndex];
			}
			
			tagOxPerGameRecord GameRecord;
			GameRecord.wTableID = m_pITableFrame->GetTableID() + 1;
			CopyMemory(GameRecord.bAreaWin, m_cbOpenResult, sizeof(GameRecord.bAreaWin));
			m_pITableFrame->SendTableDataToLookOnUser(SUB_GF_BJL_GAME_RESULT, &GameRecord, sizeof(GameRecord));

			return true;
		}
	case GER_USER_LEAVE:		//用户离开
	case GER_NETWORK_ERROR:
		{
			return true;
			//闲家判断
			if (m_wCurrentBanker!=wChairID)
			{
				//变量定义
				SCORE lRevenue = 0;

				//是否开牌
				if (m_pITableFrame->GetGameStatus() == GAME_SCENE_BET)
				{
					//返回下注
					for (int nAreaIndex = 1; nAreaIndex <= AREA_MAX; ++nAreaIndex)
					{
						if (m_lUserJettonScore[wChairID][nAreaIndex] != 0)
						{
							m_lAllJettonScore[nAreaIndex] -= m_lUserJettonScore[wChairID][nAreaIndex];
							m_lUserJettonScore[wChairID][nAreaIndex] = 0;
						}
					}
				}
				else
				{
					//写入积分
					if (m_lUserWinScore[wChairID]!=0L) 
					{
						tagScoreInfo ScoreInfo[GAME_PLAYER];
						ZeroMemory(ScoreInfo,sizeof(ScoreInfo));

						//库存金币
						if (!pIServerUserItem->IsAndroidUser())
							m_pGameServiceOption->pRoomStorageOption.lStorageCurrent -= m_lUserWinScore[wChairID];
					
						ScoreInfo[wChairID].lScore = m_lUserWinScore[wChairID];
						ScoreInfo[wChairID].cbType=m_lUserWinScore[wChairID]>0?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
						ScoreInfo[wChairID].lRevenue = m_lUserRevenue[wChairID];
						m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));
						m_lUserWinScore[wChairID] = 0;
					}
					//清除下注
					for (int nAreaIndex = 1; nAreaIndex <= AREA_MAX; nAreaIndex++)
					{
						m_lUserJettonScore[wChairID][nAreaIndex] = 0;
					}
				}

				return true;
			}

			//状态判断
			if (m_pITableFrame->GetGameStatus() != GAME_SCENE_END)
			{
				//设置状态
				m_pITableFrame->SetGameStatus(GAME_SCENE_END);

				//设置时间
				m_dwJettonTime=(DWORD)time(NULL);
				m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_nGameEndTime*1000,1,0L);

				//计算分数
				SCORE lBankerWinScore = CalculateScore();

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
				CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
				for (int i = 0; i < AREA_MAX + 1; i++)
				{
					GameEnd.cbCardType[i] = m_GameLogic.GetCardType(GameEnd.cbTableCardArray[i], MAX_CARD);
				}
				CopyMemory(GameEnd.cbResult, m_cbOpenResult, sizeof(GameEnd.cbResult));
				GameEnd.cbEarlyOpenFlag = m_cbEarlyOpen;

				for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
				{
					IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
					if ( pIServerUserItem == NULL ) continue;

					CopyMemory(GameEnd.lPlayAreaScore, m_lUserJettonScore[wUserIndex], sizeof(GameEnd.lPlayAreaScore));
					GameEnd.lPlayerWinScore = m_lUserWinScore[wUserIndex];
					GameEnd.lPlayerRestScore = pIServerUserItem->GetUserScore() + GameEnd.lPlayerWinScore;

					//发送消息					
					m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

					//库存金币
					if (!pIServerUserItem->IsAndroidUser())
						m_pGameServiceOption->pRoomStorageOption.lStorageCurrent -= m_lUserWinScore[wUserIndex];
				}
			}

			//扣除分数
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
			ScoreInfo[m_wCurrentBanker].lScore =m_lUserWinScore[m_wCurrentBanker];
			ScoreInfo[m_wCurrentBanker].lRevenue = m_lUserRevenue[m_wCurrentBanker];
			ScoreInfo[m_wCurrentBanker].cbType = (m_lUserWinScore[m_wCurrentBanker]>0?SCORE_TYPE_WIN:SCORE_TYPE_LOSE);
			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

			if (m_wCurrentBanker != INVALID_CHAIR)
			{
				IServerUserItem *pBankerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				if (pBankerUserItem && !pBankerUserItem->IsAndroidUser())
				{
					m_pGameServiceOption->pRoomStorageOption.lStorageCurrent -= m_lUserWinScore[m_wCurrentBanker];
				}
			}

			m_lUserWinScore[m_wCurrentBanker] = 0;

			//切换庄家
			ChangeBanker(true);

			return true;
		}
	}
	ASSERT(FALSE);
	return false;
}

//发送场景
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:			//空闲状态
		{
			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));			
			FillMemory(StatusFree.wApplyUser, sizeof(StatusFree.wApplyUser), INVALID_CHAIR);
			FillMemory(StatusFree.wSeatUser, sizeof(StatusFree.wSeatUser), INVALID_CHAIR);

			StatusFree.cbHighTimes = m_nMaxTimes;

			//全局信息
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusFree.cbTimeLeave=(BYTE)(m_nFreeTime-__min(dwPassTime,(DWORD)m_nFreeTime));
			
			//获取庄家
			IServerUserItem* pIBankerServerUserItem = NULL;
			if (m_wCurrentBanker == INVALID_CHAIR)
			{
				m_lBankerScore = 10000000;
			}
			else
			{
				IServerUserItem* pIBankerServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				if (pIBankerServerUserItem != NULL)
				{
					m_lBankerScore = pIBankerServerUserItem->GetUserScore();

				}
			}
			//庄家信息
			StatusFree.wBankerUser = m_wCurrentBanker;
			StatusFree.lBankerScore = m_lBankerScore;
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
			StatusFree.lAreaLimitScore = m_lAreaLimitScore;

			StatusFree.cbFreeTime = m_nFreeTime;
			StatusFree.cbPlayTime = m_nPlaceJettonTime;
			StatusFree.cbEndTime = m_nGameEndTime;

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
	case GAME_SCENE_BET:				//游戏状态
	case GAME_SCENE_END:				//结束状态
		{
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay, sizeof(StatusPlay));

			FillMemory(StatusPlay.wApplyUser, sizeof(StatusPlay.wApplyUser), INVALID_CHAIR);
			FillMemory(StatusPlay.wSeatUser, sizeof(StatusPlay.wSeatUser), INVALID_CHAIR);

			StatusPlay.cbHighTimes = m_nMaxTimes;

			//全局信息
			DWORD dwPassTime = (DWORD)time(NULL) - m_dwJettonTime;
			int	nTotalTime = (cbGameStatus == GAME_SCENE_BET ? m_nPlaceJettonTime : m_nGameEndTime);
			StatusPlay.cbTimeLeave = (BYTE)(nTotalTime - __min(dwPassTime, (DWORD)nTotalTime));

			StatusPlay.lBankerWinScore = m_lBankerWinScore;

			//获取庄家
			IServerUserItem* pIBankerServerUserItem = NULL;
			if (m_wCurrentBanker == INVALID_CHAIR)
			{
				m_lBankerScore = 10000000;
			}
			else
			{
				IServerUserItem* pIBankerServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				if (pIBankerServerUserItem != NULL)
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
				CopyMemory(StatusPlay.cbTableCardArray, m_cbTableCardArray, sizeof(m_cbTableCardArray));

				StatusPlay.lPlayerWinScore = m_lUserWinScore[pIServerUserItem->GetChairID()];
				for (int i = 0; i < AREA_MAX + 1; i++)
				{
					StatusPlay.cbCardType[i] = m_GameLogic.GetCardType(StatusPlay.cbTableCardArray[i],MAX_CARD);
				}
				CopyMemory(StatusPlay.cbResult, m_cbOpenResult, sizeof(StatusPlay.cbResult));

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

			StatusPlay.cbFreeTime = m_nFreeTime;
			StatusPlay.cbPlayTime = m_nPlaceJettonTime;
			StatusPlay.cbEndTime = m_nGameEndTime;

			StatusPlay.lMinXianHong = m_nChip[0];
			StatusPlay.lMaxXianHong = m_lUserLimitScore;

			CopyMemory(StatusPlay.lPlayerJettonScore, m_lUserJettonScore[wChairID], sizeof(StatusPlay.lPlayerJettonScore));
			CopyMemory(StatusPlay.lAllJettonScore, m_lAllJettonScore, sizeof(StatusPlay.lAllJettonScore));

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
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_FREE:		//空闲时间
		{
			//开始游戏
			m_pITableFrame->StartGame();

			//设置时间
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,m_nPlaceJettonTime*1000,1,0L);

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
			if (m_pITableFrame->GetGameStatus() != GAME_SCENE_END)
			{
				//设置状态
				m_pITableFrame->SetGameStatus(GAME_SCENE_END);

				DispatchTableCard();

				//结束游戏
				OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);

				//设置时间
				m_dwJettonTime=(DWORD)time(NULL);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,m_nGameEndTime*1000,1,0L);	

				tagChangeStatus ChangeStatus;
				ChangeStatus.wTableID = m_pITableFrame->GetTableID() + 1;
				ChangeStatus.cbGameStatus = m_pITableFrame->GetGameStatus();
				m_pITableFrame->SendTableDataToLookOnUser(SUB_GF_BJL_CHANGE_STATUS, &ChangeStatus, sizeof(ChangeStatus));
			}

			return true;
		}
	case IDI_GAME_END:			//结束游戏
		{
			//写入积分
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
			for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);				
				if ( pIServerUserItem == NULL||(QueryBuckleServiceCharge(wUserChairID)==false)) continue;				
				
				//胜利类型
				BYTE ScoreKind=(m_lUserWinScore[wUserChairID]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
				ScoreInfo[wUserChairID].lScore = m_lUserWinScore[wUserChairID];
				ScoreInfo[wUserChairID].lRevenue = m_lUserRevenue[wUserChairID];
				ScoreInfo[wUserChairID].cbType = ScoreKind;

			}

			//写入积分
			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

			//设置时间
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE,m_nFreeTime*1000,1,0L);

			//发送消息
			
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

//百人游戏获取游戏记录
void CTableFrameSink::OnGetGameRecord(VOID *GameRecord)
{
	CMD_GF_OxRoomStatus *pRoomStatus = (CMD_GF_OxRoomStatus *)GameRecord;

	pRoomStatus->tagGameInfo.wTableID = m_pITableFrame->GetTableID() + 1;
	pRoomStatus->tagGameInfo.cbGameStatus = m_pITableFrame->GetGameStatus();

	pRoomStatus->tagTimeInfo.cbFreeTime = m_nFreeTime;
	pRoomStatus->tagTimeInfo.cbBetTime = m_nPlaceJettonTime;
	pRoomStatus->tagTimeInfo.cbEndTime = m_nGameEndTime;
	pRoomStatus->tagTimeInfo.cbPassTime = (DWORD)time(NULL) - m_dwJettonTime;
	pRoomStatus->tagTimeInfo.lMinXianHong = m_nChip[0];
	pRoomStatus->tagTimeInfo.lMaxXianHong = m_lUserLimitScore;

	//客户端只显示20条
	int nIndex = m_nRecordFirst;

	pRoomStatus->cbRecordCount = m_nRecordLast - m_nRecordFirst;
	if (pRoomStatus->cbRecordCount > 20)
	{
		pRoomStatus->cbRecordCount = 20;
		nIndex = m_nRecordLast - 20;
	}

	if (m_nRecordLast < m_nRecordFirst)
	{
		nIndex = (m_nRecordLast + MAX_SCORE_HISTORY - 20) % MAX_SCORE_HISTORY;
		pRoomStatus->cbRecordCount = 20;
	}

	pRoomStatus->cbMultiple = m_nMaxTimes;

	int nArrayIndex = 0;

	while (nIndex != m_nRecordLast)
	{
		if (nArrayIndex >= 20)
			break;
		pRoomStatus->GameRecordArrary[nArrayIndex].bAreaWin[0] = m_GameRecordArrary[nIndex].bWinShunMen;
		pRoomStatus->GameRecordArrary[nArrayIndex].bAreaWin[1] = m_GameRecordArrary[nIndex].bWinDuiMen;
		pRoomStatus->GameRecordArrary[nArrayIndex].bAreaWin[2] = m_GameRecordArrary[nIndex].bWinDaoMen;
		pRoomStatus->GameRecordArrary[nArrayIndex].bAreaWin[3] = m_GameRecordArrary[nIndex].bWinHuang;

		nArrayIndex++;
		nIndex = (nIndex + 1) % MAX_SCORE_HISTORY;
	}

}

//获取百人游戏是否下注状态而且玩家下注了
bool CTableFrameSink::OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if (m_pITableFrame->GetGameStatus() == GAME_SCENE_FREE)
		return false;

	for (WORD wAreaIndex = 1; wAreaIndex < AREA_MAX+1; ++wAreaIndex)
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

//游戏消息
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_PLACE_JETTON:		//用户加注
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_PlaceJetton));
			if (wDataSize!=sizeof(CMD_C_PlaceJetton)) return false;

			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//消息处理
			CMD_C_PlaceJetton * pPlaceJetton=(CMD_C_PlaceJetton *)pData;
			return OnUserPlaceJetton(pUserData->wChairID,pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);
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
					CopyMemory(ControlInfo.lUserJettonScore[cbIndex], m_lUserJettonScore[i], sizeof(ControlInfo.lUserJettonScore[cbIndex]));
					lstrcpyn(ControlInfo.szNickName[cbIndex], pServerUser->GetNickName(), sizeof(ControlInfo.szNickName[cbIndex]));

					for (BYTE j = 0; j < AREA_MAX+1; j++)
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

//框架消息
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

bool CTableFrameSink::OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem)
{
	//离线庄家
	if (wChairID == m_wCurrentBanker && pIServerUserItem->IsAndroidUser() == false)
	{
		m_wOfflineBanker = wChairID;
	}

	if(pIServerUserItem == NULL) return false;
	//切换庄家
	if (wChairID==m_wCurrentBanker) ChangeBanker(true);

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
		CancelBanker.wCancelUser=wChairID;

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

		break;
	}

	return true;

}
//用户坐下
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//起始分数
	m_lUserStartScore[wChairID] = pIServerUserItem->GetUserScore();

	//设置时间
	if ((bLookonUser==false)&&(m_dwJettonTime==0L))
	{
		m_dwJettonTime=(DWORD)time(NULL);
		m_pITableFrame->SetGameTimer(IDI_FREE,m_nFreeTime*1000,1,NULL);
		m_pITableFrame->SetGameStatus(GAME_SCENE_FREE);
	}
	

	return true;
}

//用户起立
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	ASSERT(pIServerUserItem !=NULL);

	//起始分数
	m_lUserStartScore[wChairID] = 0;
	
	ZeroMemory(m_bUserListWin[wChairID], sizeof(m_bUserListWin[wChairID]));
	ZeroMemory(m_lUserListScore[wChairID], sizeof(m_lUserListScore[wChairID]));
	m_cbUserPlayCount[wChairID] = 0;

	//记录成绩
	if (bLookonUser==false)
	{
		//切换庄家
		if (wChairID==m_wCurrentBanker)
		{
			ChangeBanker(true);
			m_bContiueCard=false;
		}

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
			CancelBanker.wCancelUser=wChairID;

			//发送消息
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

			break;
		}
	}

	return true;
}


//加注事件
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, SCORE lJettonScore)
{
	BYTE cbServerBetArea = cbJettonArea + 1;

	//效验参数
	ASSERT((cbServerBetArea<=ID_HUANG_MEN && cbServerBetArea>=ID_TIAN_MEN)&&(lJettonScore>0L));
	if ((cbServerBetArea>ID_HUANG_MEN)||(lJettonScore<=0L) || cbServerBetArea<ID_TIAN_MEN)
	{
		CMD_S_PlaceBetFail PlaceBetFail;
		_sntprintf(PlaceBetFail.szBuffer, sizeof(PlaceBetFail.szBuffer), TEXT("下注信息错误！"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceBetFail, sizeof(PlaceBetFail));
		return true;
	}

	if (m_pITableFrame->GetGameStatus() != GAME_SCENE_BET)
	{
		CMD_S_PlaceBetFail PlaceBetFail;
		_sntprintf(PlaceBetFail.szBuffer, sizeof(PlaceBetFail.szBuffer), TEXT("当前不是下注状态，不能下注！"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceBetFail, sizeof(PlaceBetFail));
		return true;
	}
	//庄家判断
	if (m_wCurrentBanker==wChairID)
	{
		CMD_S_PlaceBetFail PlaceBetFail;
		_sntprintf(PlaceBetFail.szBuffer, sizeof(PlaceBetFail.szBuffer), TEXT("庄家不能下注！"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceBetFail, sizeof(PlaceBetFail));
		return true;
	}
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR) 
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
	SCORE lJettonCount = 0L;
	for (int nAreaIndex = 1; nAreaIndex <= AREA_MAX; ++nAreaIndex) lJettonCount += m_lUserJettonScore[wChairID][nAreaIndex];
	SCORE lAllJettonCount = 0L;
	for (int nAreaIndex = 1; nAreaIndex <= AREA_MAX; ++nAreaIndex) lAllJettonCount += m_lAllJettonScore[nAreaIndex];

	//玩家积分
	SCORE lUserScore = pIServerUserItem->GetUserScore();

	//合法校验
	if (lUserScore < lJettonCount + lJettonScore)
	{
		CMD_S_PlaceBetFail PlaceBetFail;
		_sntprintf(PlaceBetFail.szBuffer, sizeof(PlaceBetFail.szBuffer), TEXT("您的金币不足！"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceBetFail, sizeof(PlaceBetFail));
		return true;
	}
	if (m_lUserLimitScore < lJettonCount + lJettonScore)
	{
		CMD_S_PlaceBetFail PlaceBetFail;
		_sntprintf(PlaceBetFail.szBuffer, sizeof(PlaceBetFail.szBuffer), TEXT("您的下注已到达限制！"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceBetFail, sizeof(PlaceBetFail));
		return true;
	}

	//成功标识
	bool bPlaceJettonSuccess=true;

	//合法验证
	if (GetUserMaxJetton(wChairID, cbServerBetArea) >= lJettonScore)
	{
		//机器人验证
		if(pIServerUserItem->IsAndroidUser())
		{
			//区域限制
			if (m_lRobotAreaScore[cbServerBetArea] + lJettonScore > m_lRobotAreaLimit)
				return true;

			//数目限制
			bool bHaveChip = false;
			for (int i = 0; i <= AREA_MAX; i++)
			{
				if (m_lUserJettonScore[wChairID][i + 1] != 0)
					bHaveChip = true;
			}

			//统计分数
			if (bPlaceJettonSuccess)
				m_lRobotAreaScore[cbServerBetArea] += lJettonScore;
		}

		if (bPlaceJettonSuccess)
		{
			//保存下注
			m_lAllJettonScore[cbServerBetArea] += lJettonScore;
			m_lUserJettonScore[wChairID][cbServerBetArea] += lJettonScore;
		}
	}
	else
	{
		bPlaceJettonSuccess=false;
	}

	if (bPlaceJettonSuccess)
	{
		//变量定义
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

		//构造变量
		PlaceJetton.wChairID=wChairID;
		PlaceJetton.cbBetArea = cbJettonArea;
		PlaceJetton.lBetScore = lJettonScore;

		PlaceJetton.lPlayerRestScore = pIServerUserItem->GetUserScore() - (lJettonCount + lJettonScore);


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
				m_pITableFrame->SendTableData(i, SUB_S_SEAT_JETTON, &PlaceJetton, sizeof(PlaceJetton));
			else if (i == wChairID)
				m_pITableFrame->SendTableData(i, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
			else
				m_pITableFrame->SendTableData(i, SUB_S_OTHER_JETTON, &PlaceJetton, sizeof(PlaceJetton));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
		
		//下注总额超过庄家能支付上限时提前结束游戏
		SCORE lBankerMaxScore = 100000000;
		if (INVALID_CHAIR != m_wCurrentBanker)
		{
			lBankerMaxScore = lBankerScore;
		}

		if (lBankerMaxScore <= (lAllJettonCount + lJettonScore + 100) * m_nMaxTimes)
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
				m_dwJettonTime = (DWORD)time(NULL);
				m_pITableFrame->SetGameTimer(IDI_GAME_END, m_nGameEndTime * 1000, 1, 0L);
			}
		}
	}
	else
	{
		CMD_S_PlaceBetFail PlaceBetFail;
		_sntprintf(PlaceBetFail.szBuffer, sizeof(PlaceBetFail.szBuffer), TEXT("区域下注已到达限制！"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON_FAIL, &PlaceBetFail, sizeof(PlaceBetFail));
	}

	return true;
}

//发送扑克
bool CTableFrameSink::DispatchTableCard()
{	
	//重新洗牌
	ZeroMemory(m_cbTableCard, sizeof(m_cbTableCard));

	BYTE cbCardData[CountArray(m_GameLogic.m_cbCardListData)];
	CopyMemory(cbCardData, m_GameLogic.m_cbCardListData, sizeof(m_GameLogic.m_cbCardListData));

	//混乱扑克
	BYTE cbRandCount = 0, cbPosition = 0;
	do
	{
		cbPosition = rand() % (CountArray(cbCardData) - cbRandCount);
		m_cbTableCard[cbRandCount++] = cbCardData[cbPosition];
		cbCardData[cbPosition] = cbCardData[CountArray(cbCardData) - cbRandCount];
	} while (cbRandCount < CountArray(m_cbTableCard));
	
	for (WORD i = 0; i < 5; i++)
		CopyMemory(m_cbTableCardArray[i], &m_cbTableCard[i * 5], sizeof(BYTE) * 5);

	//设置数目
	for (int i=0; i<CountArray(m_cbCardCount); ++i) m_cbCardCount[i]=5;

	return true;
}

//申请庄家
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	//合法判断
	SCORE lUserScore = pIApplyServerUserItem->GetUserScore();
	if (lUserScore<m_lApplyBankerCondition)
	{
		CMD_S_ApplyFailed ApplyFailed;
		_sntprintf(ApplyFailed.szBuffer, sizeof(ApplyFailed.szBuffer), TEXT("你的金币不足以申请庄家，申请失败！"));
		m_pITableFrame->SendTableData(pIApplyServerUserItem->GetChairID(), SUB_S_APPLEY_BANKER_FAILURE, &ApplyFailed, sizeof(ApplyFailed));
		return true;
	}

	//存在判断
	WORD wApplyUserChairID=pIApplyServerUserItem->GetChairID();
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];
		if (wChairID==wApplyUserChairID)
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
	ApplyBanker.wApplyUser=wApplyUserChairID;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

	//切换判断
	if (m_pITableFrame->GetGameStatus()==GAME_SCENE_FREE && m_ApplyUserArray.GetCount()==1)
	{
		ChangeBanker(false);
	}

	return true;
}

//取消申请
bool CTableFrameSink::OnUserCancelBanker(IServerUserItem *pICancelServerUserItem)
{
	//当前庄家
	if (pICancelServerUserItem->GetChairID()==m_wCurrentBanker && m_pITableFrame->GetGameStatus()!=GAME_SCENE_FREE)
	{
		m_pITableFrame->SendTableData(pICancelServerUserItem->GetChairID(), SUB_S_CANCEL_FAILURE);
		return true;
	}
	
	bool bValidQuitBanker = true;
	//存在判断
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		//获取玩家
		WORD wChairID=m_ApplyUserArray[i];
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);

		//条件过滤
		if (pIServerUserItem==NULL) continue;
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
			CancelBanker.wCancelUser = pIServerUserItem->GetChairID();

			//发送消息
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		}
		else if (m_wCurrentBanker==wChairID)
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

	return true;
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

//更换庄家
bool CTableFrameSink::ChangeBanker(bool bCancelCurrentBanker)
{
	//切换标识
	bool bChangeBanker=false;

	WORD wPreviousBanker = m_wCurrentBanker;

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
		stChangeBanker.wPreviousBanker = wPreviousBanker;
		stChangeBanker.wBankerUser = m_wCurrentBanker;
		if (m_wCurrentBanker != INVALID_CHAIR)
		{
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
			stChangeBanker.lBankerScore = pIServerUserItem->GetUserScore();
		}
		else
		{
			stChangeBanker.lBankerScore = 10000000;
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
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_ApplyUserArray[i]);
			if (pIServerUserItem != NULL)
			{
				if (pIServerUserItem->GetUserScore() >= m_lApplyBankerCondition)
				{
					m_wCurrentBanker=m_ApplyUserArray[i];
					break;
				}
				else
				{
					nInvalidApply = i + 1;

					//发送消息
					CMD_S_CancelBanker CancelBanker = {};

					//设置变量
					CancelBanker.wCancelUser=pIServerUserItem->GetChairID();					

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
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];

		//获取玩家
		IServerUserItem *pServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (!pServerUserItem) continue;

		//庄家判断
		if (pServerUserItem->GetChairID()==m_wCurrentBanker) continue;

		//构造变量
		CMD_S_ApplyBanker ApplyBanker;
		ApplyBanker.wApplyUser=wChairID;

		//发送消息
		m_pITableFrame->SendUserItemData(pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	}
}


//最大下注
SCORE CTableFrameSink::GetUserMaxJetton(WORD wChairID, BYTE cbJettonArea)
{
	IServerUserItem *pIMeServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	if (NULL==pIMeServerUserItem) return 0L;

	int iTimer = m_nMaxTimes;

	//已下注额
	SCORE lNowJetton = 0;

	for (int nAreaIndex = 1; nAreaIndex <= AREA_MAX; ++nAreaIndex) lNowJetton += m_lUserJettonScore[wChairID][nAreaIndex];

	//庄家金币
	SCORE lBankerScore = m_lBankerScore*m_nEndGameMul / 100;;
	for (int nAreaIndex = 1; nAreaIndex <= AREA_MAX; ++nAreaIndex) lBankerScore -= m_lAllJettonScore[nAreaIndex] * iTimer;

	//区域金币
	SCORE lAreaLimitScore = m_lAreaLimitScore - m_lAllJettonScore[cbJettonArea];

	//个人限制
	SCORE lMeMaxScore = min((pIMeServerUserItem->GetUserScore() - lNowJetton*iTimer) / iTimer, m_lUserLimitScore);

//	const tagUserScore *Info = pIMeServerUserItem->GetUserScore();

//	const tagUserInfo *Data = pIMeServerUserItem->GetUserInfo();

	//区域限制
	lMeMaxScore=min(lMeMaxScore,lAreaLimitScore);

	//庄家限制
	lMeMaxScore=min(lMeMaxScore,lBankerScore/iTimer);

	//非零限制
	ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = max(lMeMaxScore, 0);

	return (lMeMaxScore);

}
//计算得分
SCORE CTableFrameSink::CalculateScore()
{
	//	return 1;
	//变量定义
	SCORE static wRevenue = m_pGameServiceOption->wRevenueRatio;

	//推断玩家
	bool static bWinTianMen, bWinDiMen, bWinXuanMen,bWinHuang;
	BYTE TianMultiple,diMultiple,TianXuanltiple,HuangMultiple;
	TianMultiple  = 1;
	diMultiple = 1 ;
	TianXuanltiple = 1;
	HuangMultiple = 1;
	DeduceWinner(bWinTianMen, bWinDiMen, bWinXuanMen,bWinHuang,TianMultiple,diMultiple,TianXuanltiple,HuangMultiple );

	//游戏记录
	tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.bWinShunMen = bWinTianMen ? TRUE : FALSE;
	GameRecord.bWinDuiMen = bWinDiMen ? TRUE : FALSE;
	GameRecord.bWinDaoMen = bWinXuanMen ? TRUE : FALSE;
	GameRecord.bWinHuang = bWinHuang ? TRUE : FALSE;

	m_cbOpenResult[0] = bWinTianMen ? TRUE : FALSE;
	m_cbOpenResult[1] = bWinDiMen ? TRUE : FALSE;
	m_cbOpenResult[2] = bWinXuanMen ? TRUE : FALSE;
	m_cbOpenResult[3] = bWinHuang ? TRUE : FALSE;

	BYTE  cbMultiple[] = {0, 1, 1, 1, 1};

	cbMultiple[1] = TianMultiple;
	cbMultiple[2] = diMultiple;
	cbMultiple[3] = TianXuanltiple;
	cbMultiple[4] = HuangMultiple;

	//移动下标
	m_nRecordLast = (m_nRecordLast + 1) % MAX_SCORE_HISTORY;
	if (m_nRecordLast == m_nRecordFirst)
	{
		m_nRecordFirst = (m_nRecordFirst + 1) % MAX_SCORE_HISTORY;
	}

	//庄家总量
	SCORE lBankerWinScore = 0;

	//玩家成绩
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	SCORE lUserLostScore[GAME_PLAYER];
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	//区域倍率
	//BYTE static const cbMultiple[] = {0, 1, 1, 1, 1, 1, 1};

	//胜利标识
	bool static bWinFlag[AREA_MAX + 1];
	bWinFlag[0]=false;
	bWinFlag[ID_TIAN_MEN]=bWinTianMen;
	bWinFlag[ID_DI_MEN]=bWinDiMen;
	bWinFlag[ID_XUAN_MEN]=bWinXuanMen;
	bWinFlag[ID_HUANG_MEN]=bWinHuang;

	//FILE *pf = fopen("C://ServLong.txt","ab+");

	//计算积分
	for (WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//庄家判断
		if (m_wCurrentBanker==wChairID) continue;

		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;

		for (WORD wAreaIndex = ID_TIAN_MEN; wAreaIndex <= ID_HUANG_MEN; ++wAreaIndex)
		{

			if (true==bWinFlag[wAreaIndex]) 
			{
				m_lUserWinScore[wChairID] += (m_lUserJettonScore[wChairID][wAreaIndex] * cbMultiple[wAreaIndex]);
				m_lUserReturnScore[wChairID] += m_lUserJettonScore[wChairID][wAreaIndex];
				lBankerWinScore -= (m_lUserJettonScore[wChairID][wAreaIndex] * cbMultiple[wAreaIndex]);
			}
			else
			{
				lUserLostScore[wChairID] -= m_lUserJettonScore[wChairID][wAreaIndex] * cbMultiple[wAreaIndex];
				lBankerWinScore += m_lUserJettonScore[wChairID][wAreaIndex] * cbMultiple[wAreaIndex];
			}
		}
		
		//总的分数
		m_lUserWinScore[wChairID] += lUserLostScore[wChairID];

		//计算税收
		if (0 < m_lUserWinScore[wChairID])
		{
			double fRevenuePer=double(wRevenue/1000.0);
			m_lUserRevenue[wChairID] = SCORE(m_lUserWinScore[wChairID] * fRevenuePer);
			m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];
		}
	}

	//庄家成绩
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;

		//计算税收
		if (0 < m_lUserWinScore[m_wCurrentBanker])
		{
			double fRevenuePer=double(wRevenue/1000.0);
			m_lUserRevenue[m_wCurrentBanker] = SCORE(m_lUserWinScore[m_wCurrentBanker] * fRevenuePer);
			m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
			lBankerWinScore = m_lUserWinScore[m_wCurrentBanker];
		}	
	}

	//离线庄家成绩
	if (m_wOfflineBanker!=INVALID_CHAIR)
	{
		m_lUserWinScore[m_wOfflineBanker] = lBankerWinScore;

		//计算税收
		if (0 < m_lUserWinScore[m_wOfflineBanker])
		{
			double fRevenuePer=double(wRevenue/1000.0);
			m_lUserRevenue[m_wOfflineBanker] = SCORE(m_lUserWinScore[m_wOfflineBanker] * fRevenuePer);
			m_lUserWinScore[m_wOfflineBanker] -= m_lUserRevenue[m_wOfflineBanker];
			lBankerWinScore = m_lUserWinScore[m_wOfflineBanker];
		}	
	}

	//累计积分
	m_lBankerWinScore += lBankerWinScore;

	//当前积分
	m_lBankerCurGameScore=lBankerWinScore;

	return lBankerWinScore;
}


void CTableFrameSink::DeduceWinner(bool &bWinTian, bool &bWinDi, bool &bWinXuan,bool &bWinHuan,BYTE &TianMultiple,BYTE &diMultiple,BYTE &TianXuanltiple,BYTE &HuangMultiple)
{
	//大小比较
	bWinTian = m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX], 5, m_cbTableCardArray[SHUN_MEN_INDEX], 5, m_nMaxTimes, TianMultiple) == 1 ? true : false;
	bWinDi = m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX], 5, m_cbTableCardArray[DUI_MEN_INDEX], 5, m_nMaxTimes, diMultiple) == 1 ? true : false;
	bWinXuan = m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX], 5, m_cbTableCardArray[DAO_MEN_INDEX], 5, m_nMaxTimes, TianXuanltiple) == 1 ? true : false;
	bWinHuan = m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX], 5, m_cbTableCardArray[HUAN_MEN_INDEX], 5, m_nMaxTimes, HuangMultiple) == 1 ? true : false;
}

//发送记录
void CTableFrameSink::SendGameRecord(IServerUserItem *pIServerUserItem)
{
	WORD wBufferSize=0;
	BYTE cbBuffer[SOCKET_TCP_BUFFER];
	int nIndex = m_nRecordFirst;
	BYTE cbRecordCount = m_nRecordLast - m_nRecordFirst;

	if (cbRecordCount > 20)
	{
		nIndex = m_nRecordLast - 20;
	}

	if (m_nRecordLast < m_nRecordFirst)
	{
		nIndex = (m_nRecordLast + MAX_SCORE_HISTORY - 20) % MAX_SCORE_HISTORY;
	}

	while (nIndex != m_nRecordLast)
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
	m_bEnableSysBanker = (pCustomConfig->CustomGeneral.nEnableSysBanker == TRUE)?true:false;
	
	//时间
	m_nFreeTime = pCustomConfig->CustomGeneral.lFreeTime;
	m_nPlaceJettonTime = pCustomConfig->CustomGeneral.lBetTime;
	m_nGameEndTime = pCustomConfig->CustomGeneral.lEndTime;
	if(m_nFreeTime < TIME_FREE	|| m_nFreeTime > 99) m_nFreeTime = TIME_FREE;
	if(m_nPlaceJettonTime < TIME_PLACE_JETTON || m_nPlaceJettonTime > 99) m_nPlaceJettonTime = TIME_PLACE_JETTON;
	if(m_nGameEndTime < TIME_GAME_END || m_nGameEndTime > 99) m_nGameEndTime = TIME_GAME_END;

	//下注
	m_lAreaLimitScore = pCustomConfig->CustomGeneral.lAreaLimitScore;
	m_nEndGameMul = pCustomConfig->CustomGeneral.lEndGameMul;
	if(m_nEndGameMul < 1 || m_nEndGameMul > 100 ) m_nEndGameMul = 80;
	
	//机器人
	m_nRobotListMaxCount = pCustomConfig->CustomAndroid.lRobotListMaxCount;

	LONGLONG lRobotBetMinCount = pCustomConfig->CustomAndroid.lRobotBetMinCount;
	LONGLONG lRobotBetMaxCount = pCustomConfig->CustomAndroid.lRobotBetMaxCount;
	m_nMaxChipRobot = rand()%(lRobotBetMaxCount-lRobotBetMinCount+1) + lRobotBetMinCount;
	if (m_nMaxChipRobot < 0)	m_nMaxChipRobot = 8;
	m_lRobotAreaLimit = pCustomConfig->CustomAndroid.lRobotAreaLimit;
	
	return;
}

//计算系统及机器人坐庄时真人玩家得分
SCORE CTableFrameSink::CalSysOrAndroidBankerScore()
{
	ASSERT ((m_wCurrentBanker == INVALID_CHAIR && m_bEnableSysBanker == true)
		|| (m_wCurrentBanker != INVALID_CHAIR && m_pITableFrame->GetTableUserItem(m_wCurrentBanker)->IsAndroidUser() == true));

	//比较倍数
	bool bXianWin[4] = { false, false, false, false };							//比较输赢
	BYTE bMultiple[4] = { 1, 1, 1, 1 };											//比较倍数
	for (int i = 0; i < 4; i++)
	{
		bXianWin[i] = (m_GameLogic.CompareCard(m_cbTableCardArray[0], 5, m_cbTableCardArray[i + 1], 5, m_nMaxTimes, bMultiple[i]) == 1);
	}

	//系统及机器人坐庄时真人玩家得分
	SCORE lRealPlayerWinScore = 0;

	// 计算系统输赢
	for (WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
	{
		// 获取玩家
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);

		// 过滤庄家
		if (wChairID == m_wCurrentBanker || pIServerUserItem == NULL)
		{
			continue;
		}

		//过滤机器人
		if (pIServerUserItem->IsAndroidUser() == true)
		{
			continue;
		}

		// 计算玩家下注
		for (int nAarea = 0; nAarea < 4; nAarea++)
		{
			if (m_lUserJettonScore[wChairID][nAarea + 1] != 0)
			{
				if( bXianWin[nAarea] )
				{				
					lRealPlayerWinScore += m_lUserJettonScore[wChairID][nAarea + 1] * bMultiple[nAarea];
				}
				else
				{
					lRealPlayerWinScore -= m_lUserJettonScore[wChairID][nAarea + 1] * bMultiple[nAarea];
				}
			}
		}
	}

	return lRealPlayerWinScore;
}

//计算真人玩家坐庄时真人的得分
SCORE CTableFrameSink::CalRealPlayerScore()
{
	SCORE lPlayerUserScore = 0;

	//比较倍数
	bool bXianWin[4] = { false, false, false, false };							//比较输赢
	BYTE bMultiple[4] = { 1, 1, 1, 1 };											//比较倍数
	for (int i = 0; i < 4; i++)
	{
		bXianWin[i] = (m_GameLogic.CompareCard(m_cbTableCardArray[0], 5, m_cbTableCardArray[i + 1], 5, m_nMaxTimes, bMultiple[i]) == 1);
	}

	//系统当庄或者机器人当庄
	if ((m_wCurrentBanker != INVALID_CHAIR && m_pITableFrame->GetTableUserItem(m_wCurrentBanker)->IsAndroidUser() == true) || (m_wCurrentBanker == INVALID_CHAIR))
	{
		// 计算系统输赢
		for (WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
		{
			// 获取玩家
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);

			// 过滤庄家
			if (wChairID == m_wCurrentBanker || pIServerUserItem == NULL || pIServerUserItem->IsAndroidUser() == true)
			{
				continue;
			}

			// 计算玩家下注
			for (int nAarea = 0; nAarea < 4; nAarea++)
			{
				if (m_lUserJettonScore[wChairID][nAarea + 1] != 0)
				{
					if (bXianWin[nAarea])
					{
						lPlayerUserScore += m_lUserJettonScore[wChairID][nAarea + 1] * bMultiple[nAarea];
					}
					else
					{
						lPlayerUserScore -= m_lUserJettonScore[wChairID][nAarea + 1] * bMultiple[nAarea];
					}
				}
			}
		}
	}
	//玩家当庄
	else
	{
		// 计算系统输赢
		for (WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
		{
			// 获取玩家
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);

			// 过滤庄家
			if (wChairID == m_wCurrentBanker || pIServerUserItem == NULL || pIServerUserItem->IsAndroidUser() == false)
			{
				continue;
			}

			// 计算玩家下注
			for (int nAarea = 0; nAarea < 4; nAarea++)
			{
				if (m_lUserJettonScore[wChairID][nAarea + 1] != 0)
				{
					if (bXianWin[nAarea])
					{
						lPlayerUserScore -= m_lUserJettonScore[wChairID][nAarea + 1] * bMultiple[nAarea];
					}
					else
					{
						lPlayerUserScore += m_lUserJettonScore[wChairID][nAarea + 1] * bMultiple[nAarea];
					}
				}
			}
		}
	}
	
	return lPlayerUserScore;
}

//查询是否扣服务费
bool CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem *pUserItem=m_pITableFrame->GetTableUserItem(i);
		if(pUserItem==NULL) continue;
		if(wChairID==i)
		{
			//返回下注
			for (int nAreaIndex = 0; nAreaIndex <= AREA_MAX; ++nAreaIndex)
			{

				if (m_lUserJettonScore[wChairID][nAreaIndex] != 0)
				{
					return true;
				}
			}
			break;
		}
	}
	if (wChairID==m_wCurrentBanker)
	{
		return true;
	}

	//离线庄家
	if (wChairID == m_wOfflineBanker)
	{
		return true;
	}

	return false;
}

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
