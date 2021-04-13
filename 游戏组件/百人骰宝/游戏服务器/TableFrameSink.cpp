#include "StdAfx.h"
#include "TableFrameSink.h"
#include <locale>
#include "DlgCustomRule.h"
//////////////////////////////////////////////////////////////////////////

//常量定义
#define SEND_COUNT					300									//发送次数

//索引定义
#define INDEX_PLAYER				0									//闲家索引
#define INDEX_BANKER				1									//庄家索引

//下注时间
#define IDI_FREE					1									//空闲时间
#define TIME_FREE					5									//空闲时间

//下注时间
#define IDI_PLACE_JETTON			2									//下注时间

#define TIME_PLACE_JETTON			15									//下注时间

//结束时间
#define IDI_GAME_END				3									//结束时间
#define TIME_GAME_END				10									//结束时间
#define TIME_GAME_ADD               10                                  //附加时间
//////////////////////////////////////////////////////////////////////////

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//游戏人数
//ConsoleWindow	    CTableFrameSink::m_DebugWindow;

//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink()
{
	//总下注数
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//个人下注
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	ZeroMemory(m_bUserListWin, sizeof(m_bUserListWin));
	ZeroMemory(m_lUserListScore, sizeof(m_lUserListScore));
	ZeroMemory(m_cbUserPlayCount, sizeof(m_cbUserPlayCount));

	//玩家成绩	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));
	
	//状态变量
	m_dwJettonTime=0L;
	
	ZeroMemory(m_cbTableCard, sizeof(m_cbTableCard));
		
	//记录变量
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;
	m_nRecordLast=0;

	//时间控制
	m_cbFreeTime = TIME_FREE;
	m_cbBetTime = TIME_PLACE_JETTON;
	m_cbEndTime = TIME_GAME_END;

	srand(GetTickCount());

	m_cbJuControl = FALSE;
	m_cbJuControlTimes = 0;
	ZeroMemory(m_cbJuControlArea, sizeof(m_cbJuControlArea));

	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{
}

//接口查询
void *  CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{

	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);	
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;

}

//初始化
bool  CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{

	//查询接口
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//查询配置
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();

	//开始模式
	//m_pITableFrame->SetStartMode(START_MODE_ALL_READY);

	m_pITableFrame->SetStartMode(START_MODE_TIME_CONTROL);
    ReadConfigInformation();

	//设置文件名
	TCHAR szPath[MAX_PATH] = TEXT("");
	GetCurrentDirectory(sizeof(szPath), szPath);
	_sntprintf(m_szConfigFileName, sizeof(m_szConfigFileName), TEXT("%s\\BaiRenConfig\\SicBoBattleConfig-%d-%d.ini"), szPath, m_pGameServiceOption->wServerID, m_pITableFrame->GetTableID());

	m_nChips[0] = GetPrivateProfileInt(TEXT("百人骰宝"), TEXT("nChip0"), 1, m_szConfigFileName);
	m_nChips[1] = GetPrivateProfileInt(TEXT("百人骰宝"), TEXT("nChip1"), 10, m_szConfigFileName);
	m_nChips[2] = GetPrivateProfileInt(TEXT("百人骰宝"), TEXT("nChip2"), 50, m_szConfigFileName);
	m_nChips[3] = GetPrivateProfileInt(TEXT("百人骰宝"), TEXT("nChip3"), 100, m_szConfigFileName);
	m_nChips[4] = GetPrivateProfileInt(TEXT("百人骰宝"), TEXT("nChip4"), 500, m_szConfigFileName);

	m_lTypeLimitScore[0] = GetPrivateProfileInt(TEXT("百人骰宝"), TEXT("nBigSmall"), -1, m_szConfigFileName);
	m_lTypeLimitScore[1] = GetPrivateProfileInt(TEXT("百人骰宝"), TEXT("nDanDian"), -1, m_szConfigFileName);
	m_lTypeLimitScore[2] = GetPrivateProfileInt(TEXT("百人骰宝"), TEXT("nShuangShai"), -1, m_szConfigFileName);
	m_lTypeLimitScore[3] = GetPrivateProfileInt(TEXT("百人骰宝"), TEXT("nQuanWei"), -1, m_szConfigFileName);
	m_lTypeLimitScore[4] = GetPrivateProfileInt(TEXT("百人骰宝"), TEXT("nWeiShai"), -1, m_szConfigFileName);
	m_lTypeLimitScore[5] = GetPrivateProfileInt(TEXT("百人骰宝"), TEXT("nHeJiu"), -1, m_szConfigFileName);
	m_lTypeLimitScore[6] = GetPrivateProfileInt(TEXT("百人骰宝"), TEXT("nHeBa"), -1, m_szConfigFileName);
	m_lTypeLimitScore[7] = GetPrivateProfileInt(TEXT("百人骰宝"), TEXT("nHeQi"), -1, m_szConfigFileName);
	m_lTypeLimitScore[8] = GetPrivateProfileInt(TEXT("百人骰宝"), TEXT("nHeLiu"), -1, m_szConfigFileName);
	m_lTypeLimitScore[9] = GetPrivateProfileInt(TEXT("百人骰宝"), TEXT("nHeWu"), -1, m_szConfigFileName);
	m_lTypeLimitScore[10] = GetPrivateProfileInt(TEXT("百人骰宝"), TEXT("nHeSi"), -1, m_szConfigFileName);

	for (int i = 0; i < MAX_TYPE_COUNT; i++)
	{
		if (m_lTypeLimitScore[i] == -1)
		{
			CTraceService::TraceString(TEXT("百人骰宝类型限制配置错误！"), TraceLevel_Warning);
			return false;
		}
	}

	return true;
}

//复位桌子
VOID  CTableFrameSink::RepositionSink()
{
	//总下注数
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));
	
	//个人下注
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//玩家成绩	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

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

//查询服务费
bool CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
	{
		if (m_lUserJettonScore[wChairID][nAreaIndex]>0)
			return true;
	}
	return false;
}

//游戏开始
bool  CTableFrameSink::OnEventGameStart()
{
	//库存衰减
	m_pITableFrame->CalculateStorageDeduct();

    //旁观玩家
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START);	
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GAME_START);

	return true;
}

//游戏结束
bool  CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束	
	{
		if (m_cbJuControl == TRUE && m_cbJuControlTimes > 0)
		{
			CopyMemory(m_cbTableCard, m_cbJuControlArea, sizeof(m_cbTableCard));
		}
		else
		{
			tag_SICBO_Parameter SICBO_Parameter;
			ZeroMemory(&SICBO_Parameter, sizeof(SICBO_Parameter));

			CopyMemory(SICBO_Parameter.lUserJettonScore, m_lUserJettonScore, sizeof(SICBO_Parameter.lUserJettonScore));

			tag_SICBO_Result pSICBO_Result;
			ZeroMemory(&pSICBO_Result, sizeof(pSICBO_Result));
			m_pITableFrame->GetControlResult(&SICBO_Parameter, sizeof(SICBO_Parameter), &pSICBO_Result);

			CopyMemory(m_cbTableCard, pSICBO_Result.cbTableCardArray, sizeof(m_cbTableCard));
		}

		if (m_cbJuControlTimes > 0)
			m_cbJuControlTimes--;
		if (m_cbJuControlTimes == 0)
		{
			m_cbJuControl = FALSE;
			ZeroMemory(m_cbJuControlArea, sizeof(m_cbJuControlArea));
		}

		//计算分数
		CalculateScore();

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
			for (BYTE j = 0; j < AREA_COUNT; j++)
			{
				lAddScore += m_lUserJettonScore[i][j];
			}
			m_lUserListScore[i][m_cbUserPlayCount[i]] = lAddScore;
			m_cbUserPlayCount[i] ++;

			if (m_cbUserPlayCount[i] == USER_LIST_COUNT)
				m_cbUserPlayCount[i] = 0;
		}

		//结束消息
		CMD_S_GameEnd GameEnd;
		ZeroMemory(&GameEnd,sizeof(GameEnd));

		GameEnd.cbWinNum = m_cbTableCard[0] + m_cbTableCard[1] + m_cbTableCard[2];
		CopyMemory(GameEnd.lAllAreaScore, m_lAllJettonScore, sizeof(GameEnd.lAllAreaScore));

		CopyMemory(GameEnd.cbWinArea, m_cbWinArea, sizeof(GameEnd.cbWinArea));
		CopyMemory(GameEnd.cbSiceNum, m_cbTableCard, sizeof(GameEnd.cbSiceNum));

		for(WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex)
		{
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
			if(pIServerUserItem == NULL)
			{
				continue;
			}

			CopyMemory(GameEnd.lPlayerAreaScore, m_lUserJettonScore[wUserIndex], sizeof(GameEnd.lPlayerAreaScore));
			GameEnd.lUserWinScore = m_lUserWinScore[wUserIndex];
			GameEnd.lPlayerRestScore = pIServerUserItem->GetUserScore() + GameEnd.lUserWinScore;

			//发送消息
			m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			if (!pIServerUserItem->IsAndroidUser())
			{
				m_pGameServiceOption->pRoomStorageOption.lStorageCurrent -= GameEnd.lUserWinScore;
			}
		}

		tagSicBoPerGameRecord GameRecord;
		GameRecord.wTableID = m_pITableFrame->GetTableID() + 1;
		CopyMemory(GameRecord.cbResult, m_cbTableCard, sizeof(GameRecord.cbResult));
		GameRecord.cbIsBig = 0;
		if (m_cbWinArea[BET_AREA_QUAN] == TRUE)
			GameRecord.cbIsBig = 2;
		else if (m_cbWinArea[BET_AREA_DA] == TRUE)
			GameRecord.cbIsBig = 1;
		m_pITableFrame->SendTableDataToLookOnUser(SUB_GF_BJL_GAME_RESULT, &GameRecord, sizeof(GameRecord));

		return true;
	}
	case GER_USER_LEAVE:		//用户离开
	case GER_NETWORK_ERROR:
		{
			return true;
		}
	}

	return false;
}

//发送场景
bool  CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:			//空闲状态
		{

			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));			

			CopyMemory(StatusFree.lChips, m_nChips, sizeof(StatusFree.lChips));
			CopyMemory(StatusFree.nTypeLimit, m_lTypeLimitScore, sizeof(StatusFree.nTypeLimit));

			//全局信息
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusFree.cbTimeLeave=(BYTE)(m_cbFreeTime-__min(dwPassTime,m_cbFreeTime));

			StatusFree.nFreeTimes = m_cbFreeTime;
			StatusFree.nPlayTimes = m_cbBetTime;
			StatusFree.nEndTimes = m_cbEndTime;

			
			int nIndex = m_nRecordFirst;
			int nAddNum = 0;
			while (nIndex != m_nRecordLast)
			{
				tagServerGameRecord ServerGameRecord = m_GameRecordArrary[nIndex];

				CopyMemory(StatusFree.pGameRecord[nAddNum].cbResult, ServerGameRecord.cbResult, sizeof(StatusFree.pGameRecord[nAddNum].cbResult));
				StatusFree.pGameRecord[nAddNum++].cbIsBig = ServerGameRecord.cbIsBig;
		 
				nIndex = (nIndex + 1) % MAX_SCORE_HISTORY;
			}

			//发送场景
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

			//机器人配置
			if (pIServerUserItem->IsAndroidUser())
			{
				tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pITableFrame->GetCustomRule();
				ASSERT(pCustomConfig);

				m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_ANDROID_CONFIG, &pCustomConfig->CustomAndroid, sizeof(tagCustomAndroid));
			}
			
			return bSuccess;
		}
	case GS_PLACE_JETTON:		//游戏状态
	case GS_GAME_END:			//结束状态
		{

			//构造数据
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay, sizeof(StatusPlay));

			CopyMemory(StatusPlay.lChips, m_nChips, sizeof(StatusPlay.lChips));
			CopyMemory(StatusPlay.nTypeLimit, m_lTypeLimitScore, sizeof(StatusPlay.nTypeLimit));

			//全局信息
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusPlay.cbTimeLeave=(BYTE)(m_cbBetTime-__min(dwPassTime,m_cbBetTime));

			StatusPlay.nFreeTimes = m_cbFreeTime;
			StatusPlay.nPlayTimes = m_cbBetTime;
			StatusPlay.nEndTimes = m_cbEndTime;

			SCORE lAllBet = 0;
			for (int i = 0; i < AREA_COUNT; i++)
				lAllBet += m_lUserJettonScore[wChiarID][i];

			StatusPlay.lPlayerRestScore = pIServerUserItem->GetUserScore() - lAllBet;

			//结束判断
			if(cbGameStatus==GS_GAME_END)
			{
				BYTE cbEndTime=m_cbEndTime;
			
				StatusPlay.cbTimeLeave=(BYTE)(cbEndTime-__min(dwPassTime,cbEndTime));

				CopyMemory(StatusPlay.cbResult, m_cbTableCard, sizeof(StatusPlay.cbResult));
				StatusPlay.cbResultType = m_cbResultType;

				StatusPlay.lPlayerWinScore = m_lUserWinScore[wChiarID];

				StatusPlay.lPlayerRestScore = pIServerUserItem->GetUserScore() + m_lUserWinScore[pIServerUserItem->GetChairID()];
			}

			int nIndex = m_nRecordFirst;
			int nAddNum = 0;
			while (nIndex != m_nRecordLast)
			{
				tagServerGameRecord ServerGameRecord = m_GameRecordArrary[nIndex];
				
				CopyMemory(StatusPlay.pGameRecord[nAddNum].cbResult, ServerGameRecord.cbResult, sizeof(StatusPlay.pGameRecord[nAddNum].cbResult));
				StatusPlay.pGameRecord[nAddNum++].cbIsBig = ServerGameRecord.cbIsBig;

				nIndex = (nIndex + 1) % MAX_SCORE_HISTORY;
			}

			for (BYTE i = 0; i < AREA_COUNT; i++)
			{
				StatusPlay.lPlayerJettonScore[i] = m_lUserJettonScore[wChiarID][i];
			}
			CopyMemory(StatusPlay.lAllJettonScore, m_lAllJettonScore, sizeof(StatusPlay.lAllJettonScore));

			//发送场景
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

			//机器人配置
			if (pIServerUserItem->IsAndroidUser())
			{
				tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pITableFrame->GetCustomRule();
				ASSERT(pCustomConfig);

				m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_ANDROID_CONFIG, &pCustomConfig->CustomAndroid, sizeof(tagCustomAndroid));
			}

			return bSuccess;
		}
	}

	return false;
}

//定时器事件
bool  CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_FREE:		//空闲时间
		{			
			//开始游戏
			m_pITableFrame->StartGame();

			//设置时间
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,m_cbBetTime*1000,1,0L);

			//设置状态
			m_pITableFrame->SetGameStatus(GS_PLACE_JETTON);

			tagChangeStatus ChangeStatus;
			ChangeStatus.wTableID = m_pITableFrame->GetTableID() + 1;
			ChangeStatus.cbGameStatus = m_pITableFrame->GetGameStatus();
			m_pITableFrame->SendTableDataToLookOnUser(SUB_GF_BJL_CHANGE_STATUS, &ChangeStatus, sizeof(ChangeStatus));

			return true;
		}
	case IDI_PLACE_JETTON:		//下注时间
		{
			//状态判断(防止强退重复设置)
			if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
			{
				//设置状态
				m_pITableFrame->SetGameStatus(GS_GAME_END);			

				//结束游戏
				OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);

				//设置时间
				m_dwJettonTime=(DWORD)time(NULL);
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
			tagScoreInfo ScoreInfo;

			//写入积分
			for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);
				if ( pIServerUserItem == NULL ) continue;
				ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));

				if(m_lUserWinScore[wUserChairID] != 0)
				{
					ScoreInfo.cbType=(m_lUserWinScore[wUserChairID]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
					ScoreInfo.lRevenue=m_lUserRevenue[wUserChairID];
					ScoreInfo.lScore=m_lUserWinScore[wUserChairID];
					
					m_pITableFrame->WriteUserScore(wUserChairID,ScoreInfo);
				}
			}

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
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);

			//设置时间
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,0L);

			CMD_S_GameReady GameReady;
			ZeroMemory(&GameReady, sizeof(GameReady));

			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
				if (pServerUser)
				{
					GameReady.lPlayerAllScore = 0;
					for (int j = 0; j < AREA_COUNT; j++)
					{
						GameReady.lPlayerAllScore += m_lUserJettonScore[i][j];
					}

					m_pITableFrame->SendTableData(i, SUB_S_GAME_READY, &GameReady, sizeof(GameReady));
					m_pITableFrame->SendLookonData(i, SUB_S_GAME_READY, &GameReady, sizeof(GameReady));
				}
			}

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
	if (m_pITableFrame->GetGameStatus() == GS_FREE)
		return false;
	//返回下注
	for (int nAreaIndex = 0; nAreaIndex < AREA_COUNT; ++nAreaIndex)
	{
		if (m_lUserJettonScore[wChairID][nAreaIndex] != 0)
		{
			return true;
		}
	}

	return false;
}

//百人游戏获取游戏记录
void CTableFrameSink::OnGetGameRecord(VOID *GameRecord)
{
	CMD_GF_SicBoRoomStatus *pRoomStatus = (CMD_GF_SicBoRoomStatus *)GameRecord;

	pRoomStatus->tagGameInfo.wTableID = m_pITableFrame->GetTableID() + 1;
	pRoomStatus->tagGameInfo.cbGameStatus = m_pITableFrame->GetGameStatus();

	pRoomStatus->tagTimeInfo.cbFreeTime = m_cbFreeTime;
	pRoomStatus->tagTimeInfo.cbBetTime = m_cbBetTime;
	pRoomStatus->tagTimeInfo.cbEndTime = m_cbEndTime;
	pRoomStatus->tagTimeInfo.cbPassTime = (DWORD)time(NULL) - m_dwJettonTime;

	int nIndex = m_nRecordFirst;
	int nArrayIndex = 0;

	pRoomStatus->cbRecordCount = m_nRecordLast - m_nRecordFirst;

	if (m_nRecordLast < m_nRecordFirst)
		pRoomStatus->cbRecordCount = m_nRecordLast + MAX_SCORE_HISTORY - m_nRecordFirst;

	while (nIndex != m_nRecordLast)
	{
		if (nArrayIndex >= 40)
			break;
		CopyMemory(pRoomStatus->GameRecordArrary[nArrayIndex].cbResult, m_GameRecordArrary[nIndex].cbResult, sizeof(pRoomStatus->GameRecordArrary[nArrayIndex].cbResult));
		pRoomStatus->GameRecordArrary[nArrayIndex].cbIsBig = m_GameRecordArrary[nIndex].cbIsBig;

		nArrayIndex++;
		nIndex = (nIndex + 1) % MAX_SCORE_HISTORY;
	}
}

//游戏消息处理
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	
	switch (wSubCmdID)
	{
	case SUB_C_PLACE_JETTON:		//用户加注
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_PlaceJetton));
			if (wDataSize!=sizeof(CMD_C_PlaceJetton)) return true;

			//用户效验
			
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//消息处理
			CMD_C_PlaceJetton * pPlaceJetton=(CMD_C_PlaceJetton *)pDataBuffer;
			//玩家下注的实际值要除以兑换倍数
			return OnUserPlaceJetton(pUserData->wChairID,pPlaceJetton->cbJettonType,pPlaceJetton->lJettonScore);
		}
		//获取玩家列表
	  case SUB_C_ONLINE_PLAYER:
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

		  BYTE cbIndex = 0;
		  for (WORD i = 0; i < GAME_PLAYER; i++)
		  {
			  IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);
			  if (pServerUser && !pServerUser->IsAndroidUser())
			  {
				  for (BYTE j = 0; j < AREA_COUNT; j++)
				  {
					  ControlInfo.lAreaJettonScore[j] += m_lUserJettonScore[i][j];
				  }

				  cbIndex++;
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

		  CMD_C_ControlWinLost *pControlWinLost = (CMD_C_ControlWinLost *)pDataBuffer;

		  CopyMemory(m_cbJuControlArea, pControlWinLost->cbJuControlArea, sizeof(m_cbJuControlArea));

		  m_cbJuControl = TRUE;
		  m_cbJuControlTimes = pControlWinLost->cbControlTimes;

		  bool bSicOk = m_cbJuControlArea[0] >= 1 && m_cbJuControlArea[0] <= 6 && m_cbJuControlArea[1] >= 1 && m_cbJuControlArea[1] <= 6 && m_cbJuControlArea[2] >= 1 && m_cbJuControlArea[2] <= 6;

		  if (m_cbJuControlTimes < 0 || m_cbJuControlTimes > 5 || !bSicOk)
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
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//数据事件
bool CTableFrameSink::OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//用户坐下
bool  CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//设置时间
	if ((bLookonUser==false)&&(m_dwJettonTime==0L))
	{
		m_dwJettonTime=(DWORD)time(NULL);
		m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,NULL);
		m_pITableFrame->SetGameStatus(GAME_STATUS_FREE);
	}
	return true;
}

//用户起来
bool  CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	ZeroMemory(m_bUserListWin[wChairID], sizeof(m_bUserListWin[wChairID]));
	ZeroMemory(m_lUserListScore[wChairID], sizeof(m_lUserListScore[wChairID]));
	m_cbUserPlayCount[wChairID] = 0;
	return true;
}

//加注事件
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, SCORE lJettonScore)
{
	BYTE cbServerArea = cbJettonArea - 1;
	//效验参数
	if (lJettonScore != m_nChips[0] && lJettonScore != m_nChips[1] && lJettonScore != m_nChips[2] && lJettonScore != m_nChips[3] && lJettonScore != m_nChips[4])
	{
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton, sizeof(PlaceJetton));
		PlaceJetton.cbResultCode = 2;
		_sntprintf(PlaceJetton.cbFailReson, sizeof(PlaceJetton.cbFailReson),TEXT("下注信息错误"));

		//发送消息
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
		return true;
	}

	if (cbServerArea < BET_AREA_XIAO || cbServerArea > BET_AREA_DAN6)
	{
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton, sizeof(PlaceJetton));
		PlaceJetton.cbResultCode = 2;
		_sntprintf(PlaceJetton.cbFailReson, sizeof(PlaceJetton.cbFailReson), TEXT("下注信息错误"));

		//发送消息
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
		return true;
	}

	if(m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON)
	{
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton, sizeof(PlaceJetton));
		PlaceJetton.cbResultCode = 3;
		_sntprintf(PlaceJetton.cbFailReson, sizeof(PlaceJetton.cbFailReson), TEXT("非下注时间，不能下注！"));

		//发送消息
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
		return true;
	}

	//变量定义
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	SCORE lJettonCount = 0;
	SCORE lJettonPlace = 0;
	for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
	{
		lJettonCount += m_lUserJettonScore[wChairID][nAreaIndex];
	}

	//玩家积分
	SCORE lUserScore = pIServerUserItem->GetUserScore();

	//合法校验
	if(lUserScore < lJettonCount + lJettonScore)
	{
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton, sizeof(PlaceJetton));
		PlaceJetton.cbResultCode = 4;
		_sntprintf(PlaceJetton.cbFailReson, sizeof(PlaceJetton.cbFailReson), TEXT("金币不足，不能下注！"));

		//发送消息
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
		return true;
	}

	//成功标识
	bool bPlaceJettonSuccess=true;

	//合法验证
	if (GetUserMaxJetton(wChairID, cbServerArea) >= lJettonScore)
	{
		//全体总注
		m_lAllJettonScore[cbServerArea] += lJettonScore;

		//个人总注
		m_lUserJettonScore[wChairID][cbServerArea] += lJettonScore;
	}
	else
	{
		bPlaceJettonSuccess=false;
	}

	if(bPlaceJettonSuccess)
	{
		//变量定义
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

		PlaceJetton.cbResultCode = 1;
		PlaceJetton.cbJettonType = cbJettonArea;
		PlaceJetton.lJettonScore=lJettonScore;

		PlaceJetton.wAddUser = wChairID;

		for (WORD i = 0; i < GAME_PLAYER; i++)
		{
			IServerUserItem *pUser = m_pITableFrame->GetTableUserItem(i);
			if (pUser == NULL)
				continue;

			if (i == wChairID)
			{
				SCORE lAllBet = 0;
				for (BYTE j = 0; j < AREA_COUNT; j++)
				{
					lAllBet += m_lUserJettonScore[i][j];
				}

				PlaceJetton.lUserRestScore = pUser->GetUserScore() - lAllBet;
			}

			//发送消息
			m_pITableFrame->SendTableData(i, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
			m_pITableFrame->SendLookonData(i, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));


			PlaceJetton.lUserRestScore = 0;
		}
	}
	else
	{
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton, sizeof(PlaceJetton));
		PlaceJetton.cbResultCode = 5;
		_sntprintf(PlaceJetton.cbFailReson, sizeof(PlaceJetton.cbFailReson), TEXT("下注已达到限制！"));

		//发送消息
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
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

//用户断线
bool  CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) 
{
	
	return true;
}

//最大下注
SCORE CTableFrameSink::GetUserMaxJetton(WORD wChairID, BYTE Area)
{
	IServerUserItem *pIMeServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	if(NULL==pIMeServerUserItem)
	{
		return 0L;
	}

	//已下注额
	SCORE lNowJetton = 0;
	for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
	{
		lNowJetton += m_lUserJettonScore[wChairID][nAreaIndex];
	}

	//个人限制
	SCORE lMeMaxScore = min((pIMeServerUserItem->GetUserScore() - lNowJetton), m_lUserLimitScore);

	//区域限制
	lMeMaxScore=min(lMeMaxScore,m_lAreaLimitScore);

	SCORE lDaXiaoBetScore = m_lUserJettonScore[wChairID][BET_AREA_XIAO] + m_lUserJettonScore[wChairID][BET_AREA_DA];
	SCORE lDanDianBetScore = 0;
	for (BYTE cbArea = BET_AREA_DAN1; cbArea <= BET_AREA_DAN6; cbArea ++)
		lDanDianBetScore += m_lUserJettonScore[wChairID][cbArea];

	SCORE lTwoBetScore = m_lUserJettonScore[wChairID][BET_AREA_TWO1] + m_lUserJettonScore[wChairID][BET_AREA_TWO2] + m_lUserJettonScore[wChairID][BET_AREA_TWO3] + m_lUserJettonScore[wChairID][BET_AREA_TWO4] + m_lUserJettonScore[wChairID][BET_AREA_TWO5] + m_lUserJettonScore[wChairID][BET_AREA_TWO6];

	SCORE lWeiBetScore = m_lUserJettonScore[wChairID][BET_AREA_WEI1] + m_lUserJettonScore[wChairID][BET_AREA_WEI2] + m_lUserJettonScore[wChairID][BET_AREA_WEI3] + m_lUserJettonScore[wChairID][BET_AREA_WEI4] + m_lUserJettonScore[wChairID][BET_AREA_WEI5] + m_lUserJettonScore[wChairID][BET_AREA_WEI6];
	SCORE lHeJiuBetScore = m_lUserJettonScore[wChairID][BET_AREA_9] + m_lUserJettonScore[wChairID][BET_AREA_10] + m_lUserJettonScore[wChairID][BET_AREA_11] + m_lUserJettonScore[wChairID][BET_AREA_12];
	SCORE lHeBaBetScore = m_lUserJettonScore[wChairID][BET_AREA_8] + m_lUserJettonScore[wChairID][BET_AREA_13];
	SCORE lHeQiBetScore = m_lUserJettonScore[wChairID][BET_AREA_7] + m_lUserJettonScore[wChairID][BET_AREA_14];
	SCORE lHeLiuBetScore = m_lUserJettonScore[wChairID][BET_AREA_6] + m_lUserJettonScore[wChairID][BET_AREA_15];
	SCORE lHeWuBetScore = m_lUserJettonScore[wChairID][BET_AREA_5] + m_lUserJettonScore[wChairID][BET_AREA_16];
	SCORE lHeSiBetScore = m_lUserJettonScore[wChairID][BET_AREA_4] + m_lUserJettonScore[wChairID][BET_AREA_17];

	//类型限制
	if (Area == BET_AREA_XIAO || Area == BET_AREA_DA)
		lMeMaxScore = min(lMeMaxScore, m_lTypeLimitScore[0] - lDaXiaoBetScore);
	else if (Area >= BET_AREA_DAN1 && Area <= BET_AREA_DAN6)
		lMeMaxScore = min(lMeMaxScore, m_lTypeLimitScore[1] - lDanDianBetScore);
	else if (Area == BET_AREA_TWO1 || Area == BET_AREA_TWO2 || Area == BET_AREA_TWO3 || Area == BET_AREA_TWO4 || Area == BET_AREA_TWO5 || Area == BET_AREA_TWO6)
		lMeMaxScore = min(lMeMaxScore, m_lTypeLimitScore[2] - lTwoBetScore);
	else if (Area == BET_AREA_QUAN)
		lMeMaxScore = min(lMeMaxScore, m_lTypeLimitScore[3] - m_lUserJettonScore[wChairID][BET_AREA_QUAN]);
	else if (Area == BET_AREA_WEI1 || Area == BET_AREA_WEI2 || Area == BET_AREA_WEI3 || Area == BET_AREA_WEI4 || Area == BET_AREA_WEI5 || Area == BET_AREA_WEI6)
		lMeMaxScore = min(lMeMaxScore, m_lTypeLimitScore[4] - lWeiBetScore);
	else if (Area == BET_AREA_9 || Area == BET_AREA_10 || Area == BET_AREA_11 || Area == BET_AREA_12)
		lMeMaxScore = min(lMeMaxScore, m_lTypeLimitScore[5] - lHeJiuBetScore);
	else if (Area == BET_AREA_8 || Area == BET_AREA_13)
		lMeMaxScore = min(lMeMaxScore, m_lTypeLimitScore[6] - lHeBaBetScore);
	else if (Area == BET_AREA_7 || Area == BET_AREA_14)
		lMeMaxScore = min(lMeMaxScore, m_lTypeLimitScore[7] - lHeQiBetScore);
	else if (Area == BET_AREA_6 || Area == BET_AREA_15)
		lMeMaxScore = min(lMeMaxScore, m_lTypeLimitScore[8] - lHeLiuBetScore);
	else if (Area == BET_AREA_5 || Area == BET_AREA_16)
		lMeMaxScore = min(lMeMaxScore, m_lTypeLimitScore[9] - lHeWuBetScore);
	else if (Area == BET_AREA_4 || Area == BET_AREA_17)
		lMeMaxScore = min(lMeMaxScore, m_lTypeLimitScore[10] - lHeSiBetScore);

	//非零限制
	lMeMaxScore = max(lMeMaxScore, 0);

	return (SCORE)(lMeMaxScore);
}
//计算得分
SCORE CTableFrameSink::CalculateScore()
{
	ZeroMemory(m_cbWinArea, sizeof(m_cbWinArea));
	int cbAreaTimes[SICBO_AREA_COUNT] = {
		1, 10, 10, 10, 200, 200, 200, 30, 200, 200, 200, 10, 10, 10, 1, 60, 30, 20, 12, 8, 6, 6, 6, 6, 8, 12, 20, 30, 60, 1, 1, 1, 1, 1, 1
	};
	BYTE cbNumSum = m_cbTableCard[0] + m_cbTableCard[1] + m_cbTableCard[2];

	BYTE cbNumCount[6] = { 0 };
	cbNumCount[m_cbTableCard[0] - 1] ++;
	cbNumCount[m_cbTableCard[1] - 1] ++;
	cbNumCount[m_cbTableCard[2] - 1] ++;

	bool bDouble = false;
	BYTE cbDoubleNum = 0;
	if (m_cbTableCard[0] == m_cbTableCard[1] || m_cbTableCard[0] == m_cbTableCard[2] || m_cbTableCard[1] == m_cbTableCard[2])
	{
		bDouble = true;
		if (m_cbTableCard[0] == m_cbTableCard[1] || m_cbTableCard[0] == m_cbTableCard[2])
			cbDoubleNum = m_cbTableCard[0];
		else if (m_cbTableCard[1] == m_cbTableCard[2])
			cbDoubleNum = m_cbTableCard[1];
	}

	bool bThree = false;
	BYTE cbThreeNum = 0;
	if (m_cbTableCard[0] == m_cbTableCard[1] && m_cbTableCard[1] == m_cbTableCard[2])
	{
		bThree = true;
		cbThreeNum = m_cbTableCard[0];
	}

	if (cbNumSum >= 4 && cbNumSum <= 10 && !bThree)
		m_cbWinArea[0] = TRUE;
	if (bDouble && cbDoubleNum == 1)
		m_cbWinArea[1] = TRUE;
	if (bDouble && cbDoubleNum == 2)
		m_cbWinArea[2] = TRUE;
	if (bDouble && cbDoubleNum == 3)
		m_cbWinArea[3] = TRUE;
	if (bThree && cbThreeNum == 1)
		m_cbWinArea[4] = TRUE;
	if (bThree && cbThreeNum == 2)
		m_cbWinArea[5] = TRUE;
	if (bThree && cbThreeNum == 3)
		m_cbWinArea[6] = TRUE;
	if (bThree)
		m_cbWinArea[7] = TRUE;
	if (bThree && cbThreeNum == 4)
		m_cbWinArea[8] = TRUE;
	if (bThree && cbThreeNum == 5)
		m_cbWinArea[9] = TRUE;
	if (bThree && cbThreeNum == 6)
		m_cbWinArea[10] = TRUE;
	if (bDouble && cbDoubleNum == 4)
		m_cbWinArea[11] = TRUE;
	if (bDouble && cbDoubleNum == 5)
		m_cbWinArea[12] = TRUE;
	if (bDouble && cbDoubleNum == 6)
		m_cbWinArea[13] = TRUE;
	if (cbNumSum >= 11 && cbNumSum <= 17 && !bThree)
		m_cbWinArea[14] = TRUE;
	if (cbNumSum == 4)
		m_cbWinArea[15] = TRUE;
	if (cbNumSum == 5)
		m_cbWinArea[16] = TRUE;
	if (cbNumSum == 6)
		m_cbWinArea[17] = TRUE;
	if (cbNumSum == 7)
		m_cbWinArea[18] = TRUE;
	if (cbNumSum == 8)
		m_cbWinArea[19] = TRUE;
	if (cbNumSum == 9)
		m_cbWinArea[20] = TRUE;
	if (cbNumSum == 10)
		m_cbWinArea[21] = TRUE;
	if (cbNumSum == 11)
		m_cbWinArea[22] = TRUE;
	if (cbNumSum == 12)
		m_cbWinArea[23] = TRUE;
	if (cbNumSum == 13)
		m_cbWinArea[24] = TRUE;
	if (cbNumSum == 14)
		m_cbWinArea[25] = TRUE;
	if (cbNumSum == 15)
		m_cbWinArea[26] = TRUE;
	if (cbNumSum == 16)
		m_cbWinArea[27] = TRUE;
	if (cbNumSum == 17)
		m_cbWinArea[28] = TRUE;
	if (cbNumCount[0] > 0)
		m_cbWinArea[29] = TRUE;
	if (cbNumCount[1] > 0)
		m_cbWinArea[30] = TRUE;
	if (cbNumCount[2] > 0)
		m_cbWinArea[31] = TRUE;
	if (cbNumCount[3] > 0)
		m_cbWinArea[32] = TRUE;
	if (cbNumCount[4] > 0)
		m_cbWinArea[33] = TRUE;
	if (cbNumCount[5] > 0)
		m_cbWinArea[34] = TRUE;

	for (int i = 0; i < 6; i++)
	{
		if (cbNumCount[i] == 3)
			cbAreaTimes[29 + i] = 3;
		else if (cbNumCount[i] == 2)
			cbAreaTimes[29 + i] = 2;
	}

    //游戏记录
    tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.cbIsBig = 0;
	if (m_cbWinArea[BET_AREA_DA] == TRUE)
		GameRecord.cbIsBig = 1;
	else if (m_cbWinArea[BET_AREA_QUAN] == TRUE)
		GameRecord.cbIsBig = 2;
	CopyMemory(GameRecord.cbResult, m_cbTableCard, sizeof(GameRecord.cbResult));

    //移动下标
    m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
    if(m_nRecordLast == m_nRecordFirst)
    {
      m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
    }

    //玩家成绩
    ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
    ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	SCORE lUserLostScore[GAME_PLAYER];
    ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	SCORE lUserWinScore = 0;

    //计算积分
    for(WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
    {
		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if(pIServerUserItem==NULL)
		{
			continue;
		}

		for (WORD wAreaIndex = 0; wAreaIndex < AREA_COUNT; ++wAreaIndex)
		{
			if (m_lUserJettonScore[wChairID][wAreaIndex] > 0)
			{
				if (m_cbWinArea[wAreaIndex] == TRUE)
				{
					SCORE Score = cbAreaTimes[wAreaIndex] * m_lUserJettonScore[wChairID][wAreaIndex];

					m_lUserWinScore[wChairID] += Score;
				}
				else
				{
					m_lUserWinScore[wChairID] -= m_lUserJettonScore[wChairID][wAreaIndex];
				}
			}
		}
    }

    //计算税收
    for(WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
    {
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if(pIServerUserItem==NULL)
		{
			continue;
		}
		if(m_lUserWinScore[wChairID] > 0)
		{
			SCORE Revenue = m_pITableFrame->CalculateRevenue(wChairID,m_lUserWinScore[wChairID]);
			m_lUserRevenue[wChairID] = Revenue;
			m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];
		}
    }

	return 0;
}

//读取配置
void CTableFrameSink::ReadConfigInformation()
{
	//获取自定义配置
	tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pGameServiceOption->cbCustomRule;
	ASSERT(pCustomConfig);

	//时间
	m_cbFreeTime = pCustomConfig->CustomGeneral.cbFreeTime;
	m_cbBetTime = pCustomConfig->CustomGeneral.cbBetTime;
	m_cbEndTime = pCustomConfig->CustomGeneral.cbEndTime;
	if(m_cbFreeTime < 1 || m_cbFreeTime > 99)
	{
		m_cbFreeTime = TIME_FREE;
	}
	if(m_cbBetTime < TIME_PLACE_JETTON || m_cbBetTime > 99)
	{
		m_cbBetTime = TIME_PLACE_JETTON;
	}
	if(m_cbEndTime < TIME_GAME_END || m_cbEndTime > 99)
	{
		m_cbEndTime = TIME_GAME_END;
	}

	//下注
	m_lAreaLimitScore = pCustomConfig->CustomGeneral.lAreaLimitScore;
	m_lUserLimitScore = pCustomConfig->CustomGeneral.lUserLimitScore;
}
