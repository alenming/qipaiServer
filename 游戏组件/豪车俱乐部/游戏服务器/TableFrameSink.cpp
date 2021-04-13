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
#define TIME_GAME_END				20									//结束时间
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

	m_lStorageMax1 = 0;
	m_lStorageMul1 = 0;
	m_lStorageMax2 = 0;
	m_lStorageMul2 = 0;
	//玩家成绩	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));
	//起始分数
	ZeroMemory(m_lUserStartScore,sizeof(m_lUserStartScore));
	//扑克信息
	m_cbTableCard = 0;
	//状态变量
	m_dwJettonTime=0L;
      m_nRobotListMaxCount =0;
	//庄家信息
	m_ApplyUserArray.RemoveAll();
	m_wCurrentBanker=INVALID_CHAIR;
	m_wBankerTime=0;
	m_lBankerWinScore=0L;		
	m_lBankerCurGameScore=0L;
	m_bEnableSysBanker = false;
		
	//记录变量
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;
	m_nRecordLast=0;

	//控制变量
	m_lStorageCurrent = 0l;
	m_StorageDeduct = 0l;

	//机器人控制					
	m_lRobotAreaLimit = 0l;
	m_lRobotBetCount = 0l;										

	//庄家设置
	m_lBankerMAX = 0l;
	m_lBankerAdd = 0l;							
	m_lBankerScoreMAX = 0l;
	m_lBankerScoreAdd = 0l;
	m_lPlayerBankerMAX = 0l;
	m_bExchangeBanker = true;

	//时间控制
	m_cbFreeTime = TIME_FREE;
	m_cbBetTime = TIME_PLACE_JETTON;
	m_cbEndTime = TIME_GAME_END;

	m_cbAddTime = TIME_GAME_ADD;
	//机器人控制
	m_nChipRobotCount = 0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));
	ZeroMemory(m_nAnimalTimes, sizeof(m_nAnimalTimes));

	srand(GetTickCount());

	m_cbJuControl = FALSE;
	m_cbJuControlTimes = 0;
	m_cbJuControlArea = AREA_COUNT;

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
	_sntprintf(m_szConfigFileName, sizeof(m_szConfigFileName), TEXT("%s\\BaiRenConfig\\LuxuryCarConfig-%d-%d.ini"), szPath, m_pGameServiceOption->wServerID, m_pITableFrame->GetTableID());

	m_nChips[0] = GetPrivateProfileInt(TEXT("奔驰宝马"), TEXT("nChip0"), 1, m_szConfigFileName);
	m_nChips[1] = GetPrivateProfileInt(TEXT("奔驰宝马"), TEXT("nChip1"), 10, m_szConfigFileName);
	m_nChips[2] = GetPrivateProfileInt(TEXT("奔驰宝马"), TEXT("nChip2"), 100, m_szConfigFileName);
	m_nChips[3] = GetPrivateProfileInt(TEXT("奔驰宝马"), TEXT("nChip3"), 500, m_szConfigFileName);
	m_nChips[4] = GetPrivateProfileInt(TEXT("奔驰宝马"), TEXT("nChip4"), 1000, m_szConfigFileName);

	m_lUserLimitScore = GetPrivateProfileInt(TEXT("奔驰宝马"), TEXT("nLimitMax"), 100000, m_szConfigFileName);

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

	//机器人控制
	m_nChipRobotCount = 0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));
	//m_cbTableCard = 0;
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
	//库存
	m_pITableFrame->CalculateStorageDeduct();

  //机器人控制
  m_nChipRobotCount = 0;
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
		tag_BCBM_Parameter BCBM_Parameter;
		ZeroMemory(&BCBM_Parameter, sizeof(BCBM_Parameter));

		BCBM_Parameter.wCurrentBanker = m_wCurrentBanker;
		BCBM_Parameter.cbJuControl = m_cbJuControl;
		BCBM_Parameter.cbJuControlArea = m_cbJuControlArea;
		CopyMemory(BCBM_Parameter.lUserJettonScore, m_lUserJettonScore, sizeof(BCBM_Parameter.lUserJettonScore));
		CopyMemory(BCBM_Parameter.nMultiple, m_nMultiple, sizeof(BCBM_Parameter.nMultiple));

		tag_BCBM_Result pBCBM_Result;
		ZeroMemory(&pBCBM_Result, sizeof(pBCBM_Result));
		m_pITableFrame->GetControlResult(&BCBM_Parameter, sizeof(BCBM_Parameter), &pBCBM_Result);

		m_cbTableCard = pBCBM_Result.cbTableCardArray;

		if (m_cbJuControlTimes > 0)
			m_cbJuControlTimes--;
		if (m_cbJuControlTimes == 0)
		{
			m_cbJuControl = FALSE;
			m_cbJuControlArea = AREA_COUNT;
		}

		//计算分数
		SCORE lBankerWinScore = CalculateScore();

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
					
		//递增次数
		m_wBankerTime++;

		//结束消息
		CMD_S_GameEnd GameEnd;
		ZeroMemory(&GameEnd,sizeof(GameEnd));

		CopyMemory(GameEnd.lAllAreaScore, m_lAllJettonScore, sizeof(GameEnd.lAllAreaScore));

		//庄家信息
		GameEnd.lBankerWinScore = m_lBankerCurGameScore;
		if (m_wCurrentBanker != INVALID_CHAIR)
		{
			IServerUserItem *pBankerUser = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
			GameEnd.lBankerRestScore = pBankerUser->GetUserScore() + GameEnd.lBankerWinScore;
		}
		else
			GameEnd.lBankerRestScore = 100000000;

	  
		GameEnd.cbTarget = cbGetAreaAnimal(m_cbTableCard);

		GameEnd.cbTurnTarget = m_cbTableCard;
      
		for(WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex)
		{
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
			if(pIServerUserItem == NULL)
			{
				continue;
			}

			GameEnd.lUserWinScore = m_lUserWinScore[wUserIndex];
			GameEnd.lPlayerRestScore = pIServerUserItem->GetUserScore() + GameEnd.lUserWinScore;

			//发送消息
			m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
		}

		tagZoPerGameRecord GameRecord;
		GameRecord.wTableID = m_pITableFrame->GetTableID() + 1;
		GameRecord.cbAnimal = cbGetAreaAnimal(m_cbTableCard);
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
				SCORE lScore = 0;
				SCORE lRevenue = 0;
				
				//统计成绩
				for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
				{
					lScore -= m_lUserJettonScore[wChairID][nAreaIndex];
				}

				//写入积分
				if (m_pITableFrame->GetGameStatus() != GS_GAME_END)
				{
					for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
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

					for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
					{
						if (m_lUserJettonScore[wChairID][nAreaIndex] != 0)
						{
							m_lUserJettonScore[wChairID][nAreaIndex] = 0;
						}
					}

					
					//写入积分
					if (m_lUserWinScore[wChairID]!=0L) 
					{

						tagScoreInfo ScoreInfo[GAME_PLAYER];
						ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
						ScoreInfo[wChairID].cbType=(m_lUserWinScore[wChairID]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
						ScoreInfo[wChairID].lRevenue=m_lUserRevenue[wChairID];
						ScoreInfo[wChairID].lScore=m_lUserWinScore[wChairID];
						m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

						m_lUserWinScore[wChairID] = 0;

						IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
						if (pServerUserItem && !pServerUserItem->IsAndroidUser())
						{
							m_pGameServiceOption->pRoomStorageOption.lStorageCurrent -= ScoreInfo[wChairID].lScore;
						}
					}
				}

			}

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
			//发送记录
			SendGameRecord(pIServerUserItem);

			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));			

			CopyMemory(StatusFree.lChips, m_nChips, sizeof(StatusFree.lChips));
			CopyMemory(StatusFree.nMultiple, m_nMultiple, sizeof(StatusFree.nMultiple));


			//控制信息
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;

			if (m_wCurrentBanker != INVALID_CHAIR)
			{
				IServerUserItem *pBankerServerItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				StatusFree.wBankerUserGameID = pBankerServerItem->GetGameID();
				StatusFree.lBankerScore = pBankerServerItem->GetUserScore();
				lstrcpyn(StatusFree.szBankerNick, pBankerServerItem->GetNickName(), CountArray(StatusFree.szBankerNick));
				StatusFree.cbBankerHeadInfo = pBankerServerItem->GetUserInfo()->wFaceID;
			}
			//系统坐庄
			else
			{
				StatusFree.wBankerUserGameID = -1;
				StatusFree.lBankerScore = 100000000;
				_sntprintf(StatusFree.szBankerNick, sizeof(StatusFree.szBankerNick),TEXT("系统坐庄"));
				StatusFree.cbBankerHeadInfo = 0;
			}

			StatusFree.nApplyCounts = m_ApplyUserArray.GetCount();

			StatusFree.lUserRestScore = pIServerUserItem->GetUserScore();

			//全局信息
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusFree.cbTimeLeave=(BYTE)(m_cbFreeTime-__min(dwPassTime,m_cbFreeTime));

			StatusFree.nFreeTimes = m_cbFreeTime;
			StatusFree.nPlayTimes = m_cbBetTime;
			StatusFree.nEndTimes = m_cbEndTime;
			StatusFree.nAddTimes = m_cbAddTime;

			StatusFree.lMinXianHong = m_nChips[0];
			StatusFree.lMaxXianHong = m_lUserLimitScore;

			FillMemory(StatusFree.cbWinMen, sizeof(StatusFree.cbWinMen), 255);
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
			int nAddNum = 0;
			while (nIndex != m_nRecordLast)
			{
				tagServerGameRecord ServerGameRecord = m_GameRecordArrary[nIndex];
				StatusFree.cbWinMen[nAddNum++] = ServerGameRecord.cbAnimal;
		 
				nIndex = (nIndex + 1) % MAX_SCORE_HISTORY;
			}

			for (BYTE i = 0; i < AREA_COUNT; i++)
			{
				StatusFree.lPlayerJettonScore[i] = m_lUserJettonScore[wChiarID][i];
			}
			CopyMemory(StatusFree.lAllJettonScore, m_lAllJettonScore, sizeof(StatusFree.lAllJettonScore));

			//发送场景
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

			//机器人配置
			if (pIServerUserItem->IsAndroidUser())
			{
				tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pITableFrame->GetCustomRule();
				ASSERT(pCustomConfig);

				m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_ANDROID_CONFIG, &pCustomConfig->CustomAndroid, sizeof(tagCustomAndroid));
			}

			//发送申请者
			SendApplyUser(pIServerUserItem);
			
			return bSuccess;
		}
	case GS_PLACE_JETTON:		//游戏状态
	case GS_GAME_END:			//结束状态
		{

			//构造数据
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay, sizeof(StatusPlay));

			CopyMemory(StatusPlay.lChips, m_nChips, sizeof(StatusPlay.lChips));
			CopyMemory(StatusPlay.nMultiple, m_nMultiple, sizeof(StatusPlay.nMultiple));



			//控制信息
			StatusPlay.lApplyBankerCondition = m_lApplyBankerCondition;

			if (m_wCurrentBanker != INVALID_CHAIR)
			{
				IServerUserItem *pBankerServerItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				StatusPlay.wBankerUserGameID = pBankerServerItem->GetGameID();
				StatusPlay.lBankerScore = pBankerServerItem->GetUserScore();
				lstrcpyn(StatusPlay.szBankerNick, pBankerServerItem->GetNickName(), CountArray(StatusPlay.szBankerNick));
				StatusPlay.cbBankerHeadInfo = pBankerServerItem->GetUserInfo()->wFaceID;
			}
			//系统坐庄
			else
			{
				StatusPlay.wBankerUserGameID = -1;
				StatusPlay.lBankerScore = 100000000;
				_sntprintf(StatusPlay.szBankerNick, sizeof(StatusPlay.szBankerNick), TEXT("系统坐庄"));
				StatusPlay.cbBankerHeadInfo = 0;
			}

			StatusPlay.nApplyCounts = m_ApplyUserArray.GetCount();

			StatusPlay.lUserRestScore = pIServerUserItem->GetUserScore();

			//全局信息
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusPlay.cbTimeLeave=(BYTE)(m_cbBetTime-__min(dwPassTime,m_cbBetTime));

			StatusPlay.nFreeTimes = m_cbFreeTime;
			StatusPlay.nPlayTimes = m_cbBetTime;
			StatusPlay.nEndTimes = m_cbEndTime;
			StatusPlay.nAddTimes = m_cbAddTime;

			StatusPlay.lMinXianHong = m_nChips[0];
			StatusPlay.lMaxXianHong = m_lUserLimitScore;

			//结束判断
			if(cbGameStatus==GS_GAME_END)
			{
				BYTE cbEndTime=m_cbEndTime;
			
				StatusPlay.cbTimeLeave=(BYTE)(cbEndTime-__min(dwPassTime,cbEndTime));
			}

			FillMemory(StatusPlay.cbWinMen, sizeof(StatusPlay.cbWinMen),255);
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
			int nAddNum = 0;
			while (nIndex != m_nRecordLast)
			{
				tagServerGameRecord ServerGameRecord = m_GameRecordArrary[nIndex];
				StatusPlay.cbWinMen[nAddNum++] = ServerGameRecord.cbAnimal;

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

			//发送申请者
			SendApplyUser(pIServerUserItem);

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
				//机器人不写库存
				if (!pIServerUserItem->IsAndroidUser())
				{
					m_pGameServiceOption->pRoomStorageOption.lStorageCurrent -= ScoreInfo.lScore;
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

			//切换庄家
			ChangeBanker(false);

			//设置时间
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,0L);

			
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_FREE);
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_FREE);

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
	if (m_pITableFrame->GetGameStatus() == GAME_STATUS_FREE)
		return false;

	//返回下注
	for (int nAreaIndex = 0; nAreaIndex < AREA_COUNT; ++nAreaIndex)
	{
		if (m_lUserJettonScore[wChairID][nAreaIndex] != 0)
		{
			return true;
		}
	}

	if (wChairID == m_wCurrentBanker)
		return true;

	return false;
}

//百人游戏获取游戏记录
void CTableFrameSink::OnGetGameRecord(VOID *GameRecord)
{
	CMD_GF_ZoRoomStatus *pRoomStatus = (CMD_GF_ZoRoomStatus *)GameRecord;

	pRoomStatus->tagGameInfo.wTableID = m_pITableFrame->GetTableID() + 1;
	pRoomStatus->tagGameInfo.cbGameStatus = m_pITableFrame->GetGameStatus();

	pRoomStatus->tagTimeInfo.cbFreeTime = m_cbFreeTime;
	pRoomStatus->tagTimeInfo.cbBetTime = m_cbBetTime;
	pRoomStatus->tagTimeInfo.cbEndTime = m_cbEndTime;
	pRoomStatus->tagTimeInfo.cbPassTime = (DWORD)time(NULL) - m_dwJettonTime;
	pRoomStatus->tagTimeInfo.lMinXianHong = m_nChips[0];
	pRoomStatus->tagTimeInfo.lMaxXianHong = m_lUserLimitScore;

	//客户端只显示40条
	int nIndex = m_nRecordFirst;

	pRoomStatus->cbRecordCount = m_nRecordLast - m_nRecordFirst;
	if (pRoomStatus->cbRecordCount > 40)
	{
		pRoomStatus->cbRecordCount = 40;
		nIndex = m_nRecordLast - 40;
	}

	if (m_nRecordLast < m_nRecordFirst)
	{
		nIndex = (m_nRecordLast + MAX_SCORE_HISTORY - 40) % MAX_SCORE_HISTORY;
		pRoomStatus->cbRecordCount = 40;
	}

	int nArrayIndex = 0;

	while (nIndex != m_nRecordLast)
	{
		if (nArrayIndex >= 40)
			break;
		pRoomStatus->GameRecordArrary[nArrayIndex].cbAnimal = m_GameRecordArrary[nIndex].cbAnimal;

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
			return OnUserPlaceJetton(pUserData->wChairID,pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);
		}
	case SUB_C_APPLY_BANKER:		//申请做庄
		{
			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus==US_LOOKON) return true;

			return OnUserApplyBanker(pIServerUserItem);	
		}
	case SUB_C_CANCEL_BANKER:		//取消做庄
		{
			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus==US_LOOKON) return true;

			return OnUserCancelBanker(pIServerUserItem);	
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
		  ControlInfo.cbJuControlArea = m_cbJuControlArea;

		  LONGLONG lUserBetAllScore[GAME_PLAYER] = { 0 };
		  BYTE cbIndex = 0;
		  for (WORD i = 0; i < GAME_PLAYER; i++)
		  {
			  IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);
			  if (pServerUser && !pServerUser->IsAndroidUser())
			  {
				  CopyMemory(ControlInfo.lUserJettonScore[cbIndex], m_lUserJettonScore[i], sizeof(ControlInfo.lUserJettonScore[cbIndex]));
				  lstrcpyn(ControlInfo.szNickName[cbIndex], pServerUser->GetNickName(), sizeof(ControlInfo.szNickName[cbIndex]));

				  for (BYTE j = 0; j < AREA_COUNT; j++)
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

					  LONGLONG lAreaTempScore[AREA_COUNT] = { 0 };
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

		  CMD_C_ControlWinLost *pControlWinLost = (CMD_C_ControlWinLost *)pDataBuffer;

		  m_cbJuControlArea = pControlWinLost->cbJuControlArea;

		  m_cbJuControl = TRUE;
		  m_cbJuControlTimes = pControlWinLost->cbControlTimes;

		  if (m_cbJuControlTimes < 0 || m_cbJuControlTimes > 5 || m_cbJuControlArea >= AREA_COUNT)
		  {
			  m_cbJuControl = FALSE;
			  m_cbJuControlTimes = 0;
			  m_cbJuControlArea = AREA_COUNT;
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
		  m_cbJuControlArea = AREA_COUNT;

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
	//起始分数
	m_lUserStartScore[wChairID] = pIServerUserItem->GetUserScore();
	//设置时间
	if ((bLookonUser==false)&&(m_dwJettonTime==0L))
	{
		m_dwJettonTime=(DWORD)time(NULL);
		m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,NULL);
		m_pITableFrame->SetGameStatus(GAME_STATUS_FREE);
	}

	//限制提示
	/*TCHAR szTipMsg[128];
	myprintf(szTipMsg,CountArray(szTipMsg),TEXT("本房间上庄条件为：%I64d,区域限制为：%I64d,玩家限制为：%I64d"),m_lApplyBankerCondition,
		m_lAreaLimitScore,m_lUserLimitScore);
	m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);*/

	return true;
}

//用户起来
bool  CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
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
	
      tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
      //设置变量
	  CancelBanker.dwGameID = pIServerUserItem->GetGameID();

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
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, SCORE lJettonScore)
{
	//效验参数
	if (lJettonScore != m_nChips[0] && lJettonScore != m_nChips[1] && lJettonScore != m_nChips[2] && lJettonScore != m_nChips[3] && lJettonScore != m_nChips[4])
	{
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton, sizeof(PlaceJetton));
		PlaceJetton.cbResult = 2;

		//发送消息
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
		return true;
	}

	if ((cbJettonArea > AREA_COUNT) || cbJettonArea < 0)
	{
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton, sizeof(PlaceJetton));
		PlaceJetton.cbResult = 3;

		//发送消息
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
		return true;
	}

	if(m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON)
	{
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton, sizeof(PlaceJetton));
		PlaceJetton.cbResult = 1;

		//发送消息
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
		return true;
	}

	//庄家判断
	if(m_wCurrentBanker==wChairID)
	{
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton, sizeof(PlaceJetton));
		PlaceJetton.cbResult = 5;

		//发送消息
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
		return true;
	}
	if(m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR)
	{
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
		PlaceJetton.cbResult = 4;

		//发送消息
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
		return true;
	}
	if(m_lUserLimitScore < lJettonCount + lJettonScore)
	{
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton, sizeof(PlaceJetton));
		PlaceJetton.cbResult = 6;

		//发送消息
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
		return true;
	}

	//成功标识
	bool bPlaceJettonSuccess=true;


	//合法验证
	if(GetUserMaxJetton(wChairID,cbJettonArea) >= lJettonScore)
	{
		//机器人验证
		if(pIServerUserItem->IsAndroidUser())
		{
			//区域限制
			if(m_lRobotAreaScore[cbJettonArea] + lJettonScore > m_lRobotAreaLimit)
			{
				return true;
			}

			//统计分数
			if(bPlaceJettonSuccess)
			{
				m_lRobotAreaScore[cbJettonArea] += lJettonScore;
			}
		}


		if(bPlaceJettonSuccess)
		{
			//全体总注
			m_lAllJettonScore[cbJettonArea] += lJettonScore;

			//个人总注
			m_lUserJettonScore[wChairID][cbJettonArea] += lJettonScore;
		}
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

		PlaceJetton.cbResult = 0;
		PlaceJetton.cbJettonArea=cbJettonArea;
		PlaceJetton.lJettonScore=lJettonScore;

		IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(wChairID);
		if (pServerUser)
			PlaceJetton.wUserGameID = pServerUser->GetGameID();

		CopyMemory(PlaceJetton.lPlayerJettonScore, m_lAllJettonScore, sizeof(PlaceJetton.lPlayerJettonScore));


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
			if (i == wChairID)
				m_pITableFrame->SendTableData(i, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
			else
				m_pITableFrame->SendTableData(i, SUB_S_OTHER_JETTON, &PlaceJetton, sizeof(PlaceJetton));


			m_pITableFrame->SendLookonData(i, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
		}
	}
	else
	{
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton, sizeof(PlaceJetton));
		PlaceJetton.cbResult = 6;

		//发送消息
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
	}

	return true;
}


//发送扑克
void CTableFrameSink::DispatchTableCard()
{
  //清空扑克
  m_cbTableCard = 0;

  //获取扑克
  m_cbTableCard = rand() % ANIMAL_COUNT;


}

//申请庄家
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	//合法判断
	WORD wApplyUserChairID = pIApplyServerUserItem->GetChairID();
	SCORE lUserScore = pIApplyServerUserItem->GetUserScore();
	if(lUserScore<m_lApplyBankerCondition)
	{
		CMD_S_ApplyBanker ApplyBanker;
		ZeroMemory(&ApplyBanker, sizeof(ApplyBanker));

		//设置变量
		ApplyBanker.cbResult = 1;

		//发送消息
		m_pITableFrame->SendTableData(wApplyUserChairID, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
		return true;
	}

	if(pIApplyServerUserItem->IsAndroidUser()&&(m_ApplyUserArray.GetCount())>m_nRobotListMaxCount)
	{
		return true;
	}

	if (m_ApplyUserArray.GetCount() >= 5)
	{
		CMD_S_ApplyBanker ApplyBanker;
		ZeroMemory(&ApplyBanker, sizeof(ApplyBanker));

		//设置变量
		ApplyBanker.cbResult = 2;

		//发送消息
		m_pITableFrame->SendTableData(wApplyUserChairID, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
		return true;
	}

	//存在判断
	for(INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];
		if(wChairID==wApplyUserChairID)
		{
			CMD_S_ApplyBanker ApplyBanker;
			ZeroMemory(&ApplyBanker, sizeof(ApplyBanker));

			//设置变量
			ApplyBanker.cbResult = 4;

			//发送消息
			m_pITableFrame->SendTableData(wApplyUserChairID, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
			return true;
		}
	}

	if (wApplyUserChairID == m_wCurrentBanker)
	{
		CMD_S_ApplyBanker ApplyBanker;
		ZeroMemory(&ApplyBanker, sizeof(ApplyBanker));

		//设置变量
		ApplyBanker.cbResult = 3;

		//发送消息
		m_pITableFrame->SendTableData(wApplyUserChairID, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
		return true;
	}

	//保存信息
	m_ApplyUserArray.Add(wApplyUserChairID);

	//构造变量
	CMD_S_ApplyBanker ApplyBanker;
	ZeroMemory(&ApplyBanker,sizeof(ApplyBanker));

	//设置变量
	ApplyBanker.cbResult = 0;
	ApplyBanker.dwGameID = pIApplyServerUserItem->GetGameID();
	lstrcpyn(ApplyBanker.szNickName, pIApplyServerUserItem->GetNickName(), CountArray(ApplyBanker.szNickName));
	ApplyBanker.nApplyCount = m_ApplyUserArray.GetCount();

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

	//切换判断
	if(m_pITableFrame->GetGameStatus()==GAME_STATUS_FREE && m_ApplyUserArray.GetCount()==1)
	{
		ChangeBanker(false);
	}

	return true;
}

//取消申请
bool CTableFrameSink::OnUserCancelBanker(IServerUserItem *pICancelServerUserItem)
{
	//当前庄家
	if (pICancelServerUserItem->GetChairID()==m_wCurrentBanker && m_pITableFrame->GetGameStatus()!=GAME_STATUS_FREE)
	{
		//发送消息
		m_pITableFrame->SendGameMessage(pICancelServerUserItem,TEXT("游戏已经开始，不可以取消当庄！"),SMT_CHAT|SMT_EJECT);
		return true;
	}

	//存在判断
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		//获取玩家
		WORD wChairID=m_ApplyUserArray[i];
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);

		//条件过滤
		if(pIServerUserItem==NULL)
		{
			continue;
		}
		if(pIServerUserItem->GetUserID()!=pICancelServerUserItem->GetUserID())
		{
			continue;
		}

		//删除玩家
		m_ApplyUserArray.RemoveAt(i);

		if (m_wCurrentBanker!=wChairID)
		{
			//构造变量
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
			//设置变量
			CancelBanker.dwGameID = pIServerUserItem->GetGameID();

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

	return true;
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
			if(wChairID!=m_wCurrentBanker)
			{
			continue;
			}

			//删除玩家
			m_ApplyUserArray.RemoveAt(i);

			break;
		}

		//设置庄家
		m_wCurrentBanker=INVALID_CHAIR;

		//轮换判断
		if (m_pITableFrame->GetGameStatus()==GAME_STATUS_FREE && FindSuitBanker())
		{
			m_wCurrentBanker=m_ApplyUserArray[0];
		}

		//设置变量
		bChangeBanker=true;
		m_bExchangeBanker = true;
	}
	//轮庄判断
	else if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//获取庄家
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

		if(pIServerUserItem!= NULL)
		{
			SCORE lBankerScore = pIServerUserItem->GetUserScore();

			//次数判断
			if (m_lPlayerBankerMAX<=m_wBankerTime || lBankerScore<m_lApplyBankerCondition)
			{
				//庄家增加判断 同一个庄家情况下只判断一次
				if(m_lPlayerBankerMAX <= m_wBankerTime && m_bExchangeBanker)
				{
					bool bScoreMAX = true;
					m_bExchangeBanker = false;
			
					for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
					{
						//获取玩家
						WORD wChairID = m_ApplyUserArray[i];
						IServerUserItem *pIUserItem = m_pITableFrame->GetTableUserItem(wChairID);
						SCORE lScore = pIUserItem->GetUserScore();

						if ( wChairID != m_wCurrentBanker && lBankerScore <= lScore )
						{
							bScoreMAX = false;
							break;
						}
					}

					LONGLONG wAddTimes=0;
					if ( bScoreMAX || (lBankerScore > m_lBankerScoreMAX && m_lBankerScoreMAX != 0l) )
					{
						if ( bScoreMAX )
						{
							wAddTimes=m_lBankerAdd;
						}
						if(lBankerScore > m_lBankerScoreMAX && m_lBankerScoreMAX != 0l)
						{
							wAddTimes+=m_lBankerScoreAdd;
						}

						m_lPlayerBankerMAX+=wAddTimes;
					
						return true;
					}
				}

				//撤销玩家
				for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
				{
					//获取玩家
					WORD wChairID=m_ApplyUserArray[i];

					//条件过滤
					if(wChairID!=m_wCurrentBanker)
					{
						continue;
					}

					//删除玩家
					m_ApplyUserArray.RemoveAt(i);

					break;
				}

				//设置庄家
				m_wCurrentBanker=INVALID_CHAIR;

				//轮换判断
				if (FindSuitBanker())
				{
					m_wCurrentBanker=m_ApplyUserArray[0];
				}

				bChangeBanker=true;
				m_bExchangeBanker = true;
			
			}
		}
		else
		{
			for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
			{
				//获取玩家
				WORD wChairID=m_ApplyUserArray[i];

				//条件过滤
				if(wChairID!=m_wCurrentBanker)
				{
					continue;
				}

				//删除玩家
				m_ApplyUserArray.RemoveAt(i);

				break;
			}
			//设置庄家
			m_wCurrentBanker=INVALID_CHAIR;
		}
	}
	//系统做庄
	else if (m_wCurrentBanker==INVALID_CHAIR &&FindSuitBanker())
	{
		m_wCurrentBanker=m_ApplyUserArray[0];
		bChangeBanker=true;
		m_bExchangeBanker = true;
	}

	//切换判断
	if (bChangeBanker)
	{

		//最大坐庄数
		m_lPlayerBankerMAX = m_lBankerMAX;

		//设置变量
		m_wBankerTime = 0;
		m_lBankerWinScore=0;

		//发送消息
		CMD_S_ChangeBanker sChangeBanker;
		ZeroMemory(&sChangeBanker,sizeof(sChangeBanker));
    
		if (m_wCurrentBanker == INVALID_CHAIR)
		{
			sChangeBanker.dwGameID = -1;
			sChangeBanker.lBankerScore = 100000000;
			_sntprintf(sChangeBanker.szNickName, sizeof(sChangeBanker),TEXT("系统坐庄"));
			sChangeBanker.cbBankerHeadInfo = 0;
		}
		else
		{
			IServerUserItem *pBankerUser = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
			sChangeBanker.dwGameID = pBankerUser->GetGameID();
			sChangeBanker.lBankerScore = pBankerUser->GetUserScore();
			lstrcpyn(sChangeBanker.szNickName, pBankerUser->GetNickName(), CountArray(sChangeBanker.szNickName));
			sChangeBanker.cbBankerHeadInfo = pBankerUser->GetUserInfo()->wFaceID;
		}

		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&sChangeBanker,sizeof(sChangeBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&sChangeBanker,sizeof(sChangeBanker));
	}

	return bChangeBanker;
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

//发送庄家
void CTableFrameSink::SendApplyUser( IServerUserItem *pRcvServerUserItem )
{
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];

		//获取玩家
		IServerUserItem *pServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if(!pServerUserItem)
		{
		  continue;
		}

		//庄家判断
		if(pServerUserItem->GetChairID()==m_wCurrentBanker)
		{
		  continue;
		}

		//构造变量
		CMD_S_ApplyBanker ApplyBanker;
		ApplyBanker.cbResult = 0;
		ApplyBanker.dwGameID = pServerUserItem->GetGameID();
		lstrcpyn(ApplyBanker.szNickName, pServerUserItem->GetNickName(), CountArray(ApplyBanker.szNickName));
		ApplyBanker.nApplyCount = m_ApplyUserArray.GetCount();

		//发送消息
		m_pITableFrame->SendUserItemData(pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	}
}

//用户断线
bool CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) 
{
	//切换庄家
	if(wChairID==m_wCurrentBanker)
	{
		ChangeBanker(true);
	}

	//取消申请
	for(WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		if(wChairID!=m_ApplyUserArray[i])
		{
		  continue;
		}

		//删除玩家
		m_ApplyUserArray.RemoveAt(i);

		//构造变量
		CMD_S_CancelBanker CancelBanker;
		ZeroMemory(&CancelBanker,sizeof(CancelBanker));
		tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
		//设置变量
		CancelBanker.dwGameID = pIServerUserItem->GetGameID();

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

		break;
	}

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
	ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
	for(int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
	{
		lNowJetton += m_lUserJettonScore[wChairID][nAreaIndex];
	}

	//庄家金币
	SCORE lBankerScore = 100000000;
	if(m_wCurrentBanker!=INVALID_CHAIR)
	{
	IServerUserItem *pIUserItemBanker=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
	if(NULL!=pIUserItemBanker)
	{
		lBankerScore=pIUserItemBanker->GetUserScore();
	}
	}

	BYTE bcWinArea[AREA_COUNT] = {TRUE};
	SCORE LosScore = m_lAllJettonScore[Area] * (m_nMultiple[Area]);

	
	lBankerScore = lBankerScore - LosScore;

	if(lBankerScore < 0)
	{
		return 0;
	}

	//个人限制
	SCORE lMeMaxScore = min((pIMeServerUserItem->GetUserScore() - lNowJetton), m_lUserLimitScore);

	//区域限制
	lMeMaxScore=min(lMeMaxScore,m_lAreaLimitScore);

	//庄家限制
	lMeMaxScore=min(lMeMaxScore,lBankerScore/(m_nMultiple[Area]));

	//非零限制
	lMeMaxScore = max(lMeMaxScore, 0);

	return (SCORE)(lMeMaxScore);
}
//计算得分
SCORE CTableFrameSink::CalculateScore()
{
	BYTE cbWinArea[AREA_COUNT] = {FALSE};
	
	cbWinArea[cbGetAreaAnimal(m_cbTableCard)] = TRUE;
		

    //游戏记录
    tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];

	GameRecord.cbAnimal = cbGetAreaAnimal(m_cbTableCard);

    //移动下标
    m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
    if(m_nRecordLast == m_nRecordFirst)
    {
      m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
    }

    //庄家总量
	SCORE lBankerWinScore = 0;

    //玩家成绩
    ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
    ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	SCORE lUserLostScore[GAME_PLAYER];
    ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	SCORE lUserWinScore = 0;

    //计算积分
    for(WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
    {
      //庄家判断
      if(m_wCurrentBanker==wChairID)
      {
        continue;
      }

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
			  if (cbWinArea[wAreaIndex] == TRUE)
			  {
				  SCORE Score = m_nMultiple[wAreaIndex] * m_lUserJettonScore[wChairID][wAreaIndex];

				  m_lUserWinScore[wChairID] += Score;
				  lBankerWinScore -= Score;
			  }
			  else
			  {
				  m_lUserWinScore[wChairID] -= m_lUserJettonScore[wChairID][wAreaIndex];
				  lBankerWinScore += m_lUserJettonScore[wChairID][wAreaIndex];
			  }
		  }
	  }
     
    }

    if(m_wCurrentBanker!=INVALID_CHAIR)
    {
      //庄家成绩
      m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;
    }

    //计算税收
    for(WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
    {
      IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
      if(pIServerUserItem==NULL)
      {
        continue;
      }
      if(wChairID != m_wCurrentBanker && m_lUserWinScore[wChairID] > 0)
      {
        SCORE Revenue = m_pITableFrame->CalculateRevenue(wChairID,m_lUserWinScore[wChairID]);
        m_lUserRevenue[wChairID] = Revenue;
        m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];

      }
      else if(m_wCurrentBanker!=INVALID_CHAIR && wChairID == m_wCurrentBanker && lBankerWinScore > 0)
      {
        SCORE Revenue = m_pITableFrame->CalculateRevenue(wChairID,m_lUserWinScore[m_wCurrentBanker]);
        m_lUserRevenue[m_wCurrentBanker] = Revenue;
        m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
      }
    }


	//累计积分
	m_lBankerWinScore += lBankerWinScore;

	//当前积分
	m_lBankerCurGameScore=lBankerWinScore;


	return lBankerWinScore;
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
	if(wBufferSize>0)
	{
		m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
	}
}

//发送消息
void CTableFrameSink::SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg)
{
	if (wChairID==INVALID_CHAIR)
	{
		//游戏玩家
		for (WORD i=0; i<GAME_PLAYER; ++i)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
			if(pIServerUserItem==NULL)
			{
				continue;
			}
			m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_CHAT);
		}

		//旁观玩家
		WORD wIndex=0;
		do
		{
			IServerUserItem *pILookonServerUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
			if(pILookonServerUserItem==NULL)
			{
			break;
			}

			m_pITableFrame->SendGameMessage(pILookonServerUserItem,pszTipMsg,SMT_CHAT);

		}
		while(true);
	}
	else
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if(pIServerUserItem!=NULL)
		{
		  m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_CHAT|SMT_EJECT);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//查找庄家
bool CTableFrameSink::FindSuitBanker()
{

	bool bReturn=false;
	while(m_ApplyUserArray.GetCount()>0)
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(m_ApplyUserArray[0]);
		if ( pIServerUserItem == NULL )
		{
			m_ApplyUserArray.RemoveAt(0);
		}
		else
		{
			//坐庄判断
			SCORE lUserScore = pIServerUserItem->GetUserScore();
			if(lUserScore<m_lApplyBankerCondition)
			{
				//发送消息
				CMD_S_CancelBanker CancelBanker;
				ZeroMemory(&CancelBanker,sizeof(CancelBanker));
				tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
				//设置变量
				CancelBanker.dwGameID = pIServerUserItem->GetGameID();

				//发送消息
				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
				m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));


				//删除玩家
				m_ApplyUserArray.RemoveAt(0);
			}
			else
			{
				bReturn=true;
				break;
			}
		}
	}

	return bReturn;
}

//读取配置
void CTableFrameSink::ReadConfigInformation()
{
	//获取自定义配置
	tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pGameServiceOption->cbCustomRule;
	ASSERT(pCustomConfig);

	//上庄
	m_lApplyBankerCondition = pCustomConfig->CustomGeneral.lApplyBankerCondition;
	m_lBankerMAX = pCustomConfig->CustomGeneral.lBankerTime;
	m_lBankerAdd = pCustomConfig->CustomGeneral.lBankerTimeAdd;
	m_lBankerScoreMAX = pCustomConfig->CustomGeneral.lBankerScoreMAX;
	m_lBankerScoreAdd = pCustomConfig->CustomGeneral.lBankerTimeExtra;
	m_bEnableSysBanker = (pCustomConfig->CustomGeneral.nEnableSysBanker == TRUE)?true:false;

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


	//库存
	m_StorageDeduct = pCustomConfig->CustomGeneral.StorageDeduct;
	m_lStorageMax1 = pCustomConfig->CustomGeneral.StorageMax1;
	m_lStorageMul1 = pCustomConfig->CustomGeneral.StorageMul1;
	m_lStorageMax2 = pCustomConfig->CustomGeneral.StorageMax2;
	m_lStorageMul2 = pCustomConfig->CustomGeneral.StorageMul2;
	if(m_lStorageMul1 < 0 || m_lStorageMul1 > 100)
	{
		m_lStorageMul1 = 50;
	}
	if(m_lStorageMul2 < 0 || m_lStorageMul2 > 100)
	{
		m_lStorageMul2 = 80;
	}

	//机器人
	m_nRobotListMaxCount = pCustomConfig->CustomAndroid.lRobotListMaxCount;

	LONGLONG lRobotBetMinCount = pCustomConfig->CustomAndroid.lRobotBetMinCount;
	LONGLONG lRobotBetMaxCount = pCustomConfig->CustomAndroid.lRobotBetMaxCount;
	m_nMaxChipRobot = rand()%(lRobotBetMaxCount-lRobotBetMinCount+1) + lRobotBetMinCount;
	if(m_nMaxChipRobot < 0)
	{
		m_nMaxChipRobot = 8;
	}
	m_lRobotAreaLimit = pCustomConfig->CustomAndroid.lRobotAreaLimit;


	if ( m_lBankerScoreMAX <= m_lApplyBankerCondition)
		m_lBankerScoreMAX = 0l;

	m_lPlayerBankerMAX = m_lBankerMAX;

	//区域倍数
	m_nMultiple[AREA_S_DZ] = 5;
	m_nMultiple[AREA_S_AD] = 5;
	m_nMultiple[AREA_S_BM] = 5;
	m_nMultiple[AREA_S_BC] = 5;
	m_nMultiple[AREA_B_DZ] = 10;
	m_nMultiple[AREA_B_AD] = 20;
	m_nMultiple[AREA_B_BM] = 30;
	m_nMultiple[AREA_B_BC] = 40;

}

//获取区域动物
BYTE CTableFrameSink::cbGetAreaAnimal(BYTE cbCardData)
{
	if (cbCardData == 0 || cbCardData == 8 || cbCardData == 16 || cbCardData == 24)
		return AREA_B_BM;
	else if (cbCardData == 1 || cbCardData == 9 || cbCardData == 17 || cbCardData == 25)
		return AREA_S_BM;
	else if (cbCardData == 2 || cbCardData == 10 || cbCardData == 18 || cbCardData == 26)
		return AREA_B_AD;
	else if (cbCardData == 3 || cbCardData == 11 || cbCardData == 19 || cbCardData == 27)
		return AREA_S_AD;
	else if (cbCardData == 4 || cbCardData == 12 || cbCardData == 20 || cbCardData == 28)
		return AREA_B_DZ;
	else if (cbCardData == 5 || cbCardData == 13 || cbCardData == 21 || cbCardData == 29)
		return AREA_S_DZ;
	else if (cbCardData == 6 || cbCardData == 14 || cbCardData == 22 || cbCardData == 30)
		return AREA_B_BC;
	else if (cbCardData == 7 || cbCardData == 15 || cbCardData == 23 || cbCardData == 31)
		return AREA_S_BC;
}