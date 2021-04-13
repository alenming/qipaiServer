#include "StdAfx.h"
#include "TableFrameSink.h"
#include <conio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////

#define IDI_SO_OPERATE          1				//代打定时器
#define TIME_SO_OVERTIME        30*1000			//超时时间

//动作标识
#define IDI_DELAY_ENDGAME		2				//动作标识
#define TIME_DELAY_END			3000			//延时时间

#define IDI_AUTO_START			3				//自动开始
#define TIME_START				5000			//自动开始时间
//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink()
{
	m_RealPlayers = 0;
	m_wPlayerNum = GAME_PLAYER;

	//游戏变量
	m_wBankerUser = INVALID_CHAIR;
	m_wCurrentUser = INVALID_CHAIR;

	//用户状态
	ZeroMemory(m_lCurrentScore, sizeof(m_lCurrentScore));
	ZeroMemory(m_lTableScore, sizeof(m_lTableScore));
	ZeroMemory(m_cbPlayStatus, sizeof(m_cbPlayStatus));
	ZeroMemory(m_cbPlayerVlue, sizeof(m_cbPlayerVlue));
	FillMemory(m_cbCallStatus, sizeof(m_cbCallStatus), 0xFF);

	m_dwPassedTime = 0;

	m_bWriteRoomStorage = true;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{
}

//接口查询--检测相关信息版本
void * CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink, Guid, dwQueryVer);
	QUERYINTERFACE(ITableUserAction, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink, Guid, dwQueryVer);
	return NULL;
}

//初始化
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	ASSERT(pIUnknownEx != NULL);
	m_pITableFrame = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, ITableFrame);
	if (m_pITableFrame == NULL)
	{
		return false;
	}

	m_pITableFrame->SetStartMode(START_MODE_ALL_READY);

	//游戏配置
	m_pGameServiceAttrib = m_pITableFrame->GetGameServiceAttrib();
	m_pGameServiceOption = m_pITableFrame->GetGameServiceOption();

	//自定义配置
	tagCustomRule *pGameCustomRule = (tagCustomRule *)m_pITableFrame->GetCustomRule();
	ASSERT(pGameCustomRule != NULL);

	return true;
}

//复位桌子
void CTableFrameSink::RepositionSink()
{
	//游戏变量
	m_wBankerUser = INVALID_BANKER;
	m_wCurrentUser = INVALID_CHAIR;
	m_RealPlayers = 0;
	ZeroMemory(m_lTableScore, sizeof(m_lTableScore));
	ZeroMemory(m_lCurrentScore, sizeof(m_lCurrentScore));
	ZeroMemory(m_cbHandCardData, sizeof(m_cbHandCardData));
	FillMemory(m_cbCallStatus, sizeof(m_cbCallStatus), 0xFF);

	m_bWriteRoomStorage = true;

	return;
}

//用户断线
bool CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return true;
}

//用户重入
bool CTableFrameSink::OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return true;
}

//用户坐下
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	return true;
}

//用户起立
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	return true;
}

//游戏开始
bool CTableFrameSink::OnEventGameStart()
{
	ZeroMemory(m_bFleeUser, sizeof(m_bFleeUser));
	ZeroMemory(m_cbPlayStatus, sizeof(m_cbPlayStatus));
	//设置状态
	m_pITableFrame->SetGameStatus(GS_TK_CALL);

	BYTE cbAiCount = 0;
	BYTE cbPlayCount = 0;

	//用户状态
	for (WORD i = 0; i < m_wPlayerNum; i++)
	{
		//获取用户
		IServerUserItem *pIServerUser = m_pITableFrame->GetTableUserItem(i);
		if (pIServerUser == NULL)
		{
			m_cbPlayStatus[i] = FALSE;
		}
		else
		{
			m_cbPlayStatus[i] = TRUE;
			m_RealPlayers++;

			cbPlayCount++;
			if (pIServerUser->IsAndroidUser())
			{
				cbAiCount++;
			}
		}
	}

	if (cbAiCount == 0 || cbAiCount == cbPlayCount)
		m_bWriteRoomStorage = false;

	//库存衰减
	if (m_bWriteRoomStorage)
		m_pITableFrame->CalculateStorageDeduct();

	//玩家扑克
	BYTE cbRandCard[FULL_COUNT];
	m_GameLogic.RandCardList(cbRandCard, FULL_COUNT);

	for (WORD i = 0; i < m_wPlayerNum; i++)
	{
		if (m_cbPlayStatus[i])
		{
			CopyMemory(m_cbHandCardData[i], &cbRandCard[i * MAX_COUNT], MAX_COUNT);
		}
	}	

	//设置变量
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart, sizeof(GameStart));
	CopyMemory(GameStart.cbPlayStatus, m_cbPlayStatus, sizeof(GameStart.cbPlayStatus));

	//发送数据
	for (WORD i = 0; i < m_wPlayerNum; i++)
	{
		if (m_cbPlayStatus[i] != TRUE)
			continue;
		ZeroMemory(GameStart.cbCardData, sizeof(GameStart.cbCardData));
		IServerUserItem *pIServerUser = m_pITableFrame->GetTableUserItem(i);
		if (pIServerUser && pIServerUser->IsAndroidUser())
			CopyMemory(GameStart.cbCardData, m_cbHandCardData[i], MAX_COUNT);
		else
			CopyMemory(GameStart.cbCardData, m_cbHandCardData[i], MAX_COUNT-1);

		m_pITableFrame->SendTableData(i, SUB_S_GAME_START, &GameStart, sizeof(GameStart));
	}

	m_pITableFrame->KillGameTimer(IDI_SO_OPERATE);
	m_pITableFrame->SetGameTimer(IDI_SO_OPERATE, TIME_SO_OVERTIME, 1, 0);

	m_dwPassedTime = (DWORD)time(NULL);

	return true;
}


//游戏结束
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_DISMISS:   //游戏解散
	{
		//构造数据
		CMD_S_GameEnd GameEnd;
		ZeroMemory(&GameEnd, sizeof(GameEnd));

		CopyMemory(GameEnd.cbCardData, m_cbHandCardData, sizeof(GameEnd.cbCardData));
		CopyMemory(GameEnd.cbPlayerValue, m_cbPlayerVlue, sizeof(GameEnd.cbPlayerValue));
		CopyMemory(GameEnd.lGameScore, m_lCurrentScore, sizeof(GameEnd.lGameScore));

		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));

		m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);


		return true;
	}
	case GER_NORMAL:    //常规结束
	{
		//删除时间
		m_pITableFrame->KillGameTimer(IDI_SO_OPERATE);

		//库存控制
		tag_SG_Parameter SG_Parameter;
		ZeroMemory(&SG_Parameter, sizeof(SG_Parameter));

		SG_Parameter.wBankerUser = m_wBankerUser;
		CopyMemory(SG_Parameter.cbPlayerStatus, m_cbPlayStatus, sizeof(SG_Parameter.cbPlayerStatus));
		CopyMemory(SG_Parameter.lTableScore, m_lTableScore, sizeof(SG_Parameter.lTableScore));
		CopyMemory(SG_Parameter.cbCallBankerTimes, m_cbCallStatus, sizeof(SG_Parameter.cbCallBankerTimes));
		CopyMemory(SG_Parameter.cbTableCardArray, m_cbHandCardData, sizeof(SG_Parameter.cbTableCardArray));

		tag_SG_Result pSG_Result;
		ZeroMemory(&pSG_Result, sizeof(pSG_Result));
		m_pITableFrame->GetControlResult(&SG_Parameter, sizeof(SG_Parameter), &pSG_Result);
		CopyMemory(m_cbHandCardData, pSG_Result.cbTableCardArray, sizeof(m_cbHandCardData));

		//定义变量
		CMD_S_GameEnd GameEnd;
		ZeroMemory(&GameEnd, sizeof(GameEnd));

		CopyMemory(GameEnd.cbPlayerStatus,m_cbPlayStatus,sizeof(GameEnd.cbPlayerStatus));

		//保存扑克
		BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
		CopyMemory(cbUserCardData, m_cbHandCardData, sizeof(cbUserCardData));
		CopyMemory(GameEnd.cbCardData, cbUserCardData, sizeof(GameEnd.cbCardData));

		//玩家点数
		ZeroMemory(m_cbPlayerVlue, sizeof(m_cbPlayerVlue));
		for (WORD i = 0; i < m_wPlayerNum; i++)
		{
			if (!m_cbPlayStatus[i])
				continue;

			m_cbPlayerVlue[i] = m_GameLogic.GetCardType(cbUserCardData[i], MAX_COUNT);
		}

		//玩家算分
		SCORE lBankerScore = 0;
		SCORE lPlayerScore = 0;

		SCORE lCellScore = m_pITableFrame->GetCellScore();

		for (WORD i = 0; i < m_wPlayerNum; i++)
		{
			if (i == m_wBankerUser || m_cbPlayStatus[i] == FALSE)
				continue;

			lBankerScore = m_lTableScore[i] * m_GameLogic.GetTimes(cbUserCardData[m_wBankerUser], MAX_COUNT) * lCellScore * m_cbCallStatus[m_wBankerUser];
			lPlayerScore = m_lTableScore[i] * m_GameLogic.GetTimes(cbUserCardData[i], MAX_COUNT) * lCellScore * m_cbCallStatus[m_wBankerUser];
			
			if (m_GameLogic.CompareCard(cbUserCardData[m_wBankerUser], cbUserCardData[i], MAX_COUNT))
			{
				m_lCurrentScore[m_wBankerUser] += lBankerScore;
				m_lCurrentScore[i] -= lBankerScore;
			}
			else
			{
				m_lCurrentScore[m_wBankerUser] -= lPlayerScore;
				m_lCurrentScore[i] += lPlayerScore;
			}

		}

		//积分变量
		tagScoreInfo ScoreInfoArray[GAME_PLAYER];
		ZeroMemory(&ScoreInfoArray, sizeof(ScoreInfoArray));

		//历史积分
		for (WORD i = 0; i < m_wPlayerNum; i++)
		{
			//收税
			SCORE lRevenue = 0;
			if (m_lCurrentScore[i]>0)
			{
				lRevenue = m_pITableFrame->CalculateRevenue(i, m_lCurrentScore[i]);
				m_lCurrentScore[i] -= lRevenue;
				ScoreInfoArray[i].lRevenue = lRevenue;
			}
			//计算积分
			ScoreInfoArray[i].lScore = m_lCurrentScore[i];
			ScoreInfoArray[i].cbType = (ScoreInfoArray[i].lScore >= 0L) ? SCORE_TYPE_WIN : SCORE_TYPE_LOSE;
		}

		//写入积分
		m_pITableFrame->WriteTableScore(ScoreInfoArray, CountArray(ScoreInfoArray));

		CopyMemory(GameEnd.cbPlayerValue, m_cbPlayerVlue, sizeof(GameEnd.cbPlayerValue));
		CopyMemory(GameEnd.lGameScore, m_lCurrentScore, sizeof(GameEnd.lGameScore));

		//库存统计
		for (WORD i = 0; i < m_wPlayerNum; i++)
		{
			//获取用户
			IServerUserItem * pIServerUserIte = m_pITableFrame->GetTableUserItem(i);
			if (pIServerUserIte == NULL)
			{
				continue;
			}

			//库存累计
			if (pIServerUserIte->IsAndroidUser() && m_bWriteRoomStorage)
			{
				m_pGameServiceOption->pRoomStorageOption.lStorageCurrent += GameEnd.lGameScore[i];
			}

		}

		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));

		m_pITableFrame->SetGameTimer(IDI_DELAY_ENDGAME, TIME_DELAY_END, 1, 0);


		return true;
	}
	case GER_USER_LEAVE:    //用户强退
	case GER_NETWORK_ERROR:
	{
		return true;


	}

	}

	return false;
}

//发送场景
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:    //空闲状态
	{
		//构造数据
		CMD_S_StatusFree StatusFree;
		ZeroMemory(&StatusFree, sizeof(StatusFree));

		StatusFree.lCellScore = m_pITableFrame->GetCellScore();

		if (pIServerUserItem->IsAndroidUser())
		{
			tagCustomRule *pGameCustomRule = (tagCustomRule *)m_pITableFrame->GetCustomRule();

			tagRobotConfig RobotConfig;
			RobotConfig.lRobotScoreMin = pGameCustomRule->lRobotScoreMin;
			RobotConfig.lRobotScoreMax = pGameCustomRule->lRobotScoreMax;
			RobotConfig.lRobotBankGet = pGameCustomRule->lRobotBankGet;
			RobotConfig.lRobotBankGetBanker = pGameCustomRule->lRobotBankGetBanker;
			RobotConfig.lRobotBankStoMul = pGameCustomRule->lRobotBankStoMul;

			m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_ANDROID_CONFIG, &RobotConfig, sizeof(RobotConfig));
		}

		//发送场景
		return m_pITableFrame->SendGameScene(pIServerUserItem, &StatusFree, sizeof(StatusFree));
	}
	case GS_TK_CALL:  //叫庄状态
	{
		//构造数据
		CMD_S_StatusCall StatusCall;
		ZeroMemory(&StatusCall, sizeof(StatusCall));

		StatusCall.cbTimeLeave = (DWORD)time(NULL) - m_dwPassedTime;

		StatusCall.lCellScore = m_pITableFrame->GetCellScore();
		CopyMemory(StatusCall.cbCardData, m_cbHandCardData[wChairID], MAX_COUNT - 1);
		CopyMemory(StatusCall.cbPlayStatus, m_cbPlayStatus, sizeof(StatusCall.cbPlayStatus));
		CopyMemory(StatusCall.m_cbCallStatus, m_cbCallStatus, sizeof(StatusCall.m_cbCallStatus));

		if (pIServerUserItem->IsAndroidUser())
		{
			tagCustomRule *pGameCustomRule = (tagCustomRule *)m_pITableFrame->GetCustomRule();

			tagRobotConfig RobotConfig;
			RobotConfig.lRobotScoreMin = pGameCustomRule->lRobotScoreMin;
			RobotConfig.lRobotScoreMax = pGameCustomRule->lRobotScoreMax;
			RobotConfig.lRobotBankGet = pGameCustomRule->lRobotBankGet;
			RobotConfig.lRobotBankGetBanker = pGameCustomRule->lRobotBankGetBanker;
			RobotConfig.lRobotBankStoMul = pGameCustomRule->lRobotBankStoMul;

			m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_ANDROID_CONFIG, &RobotConfig, sizeof(RobotConfig));
		}

		//发送场景
		return m_pITableFrame->SendGameScene(pIServerUserItem, &StatusCall, sizeof(StatusCall));
	}
	case GS_TK_SCORE: //下注状态
	{
		//构造数据
		CMD_S_StatusScore StatusScore;
		ZeroMemory(&StatusScore, sizeof(StatusScore));

		StatusScore.cbTimeLeave = (DWORD)time(NULL) - m_dwPassedTime;

		StatusScore.lCellScore = m_pITableFrame->GetCellScore();
		StatusScore.wBankerUser = m_wBankerUser;
		CopyMemory(StatusScore.cbCallStatus, m_cbCallStatus, sizeof(StatusScore.cbCallStatus));
		CopyMemory(StatusScore.cbCardData, m_cbHandCardData[wChairID], MAX_COUNT - 1);
		CopyMemory(StatusScore.cbPlayStatus, m_cbPlayStatus, sizeof(StatusScore.cbPlayStatus));
		CopyMemory(StatusScore.lTableScore, m_lTableScore, sizeof(StatusScore.lTableScore));

		if (pIServerUserItem->IsAndroidUser())
		{
			tagCustomRule *pGameCustomRule = (tagCustomRule *)m_pITableFrame->GetCustomRule();

			tagRobotConfig RobotConfig;
			RobotConfig.lRobotScoreMin = pGameCustomRule->lRobotScoreMin;
			RobotConfig.lRobotScoreMax = pGameCustomRule->lRobotScoreMax;
			RobotConfig.lRobotBankGet = pGameCustomRule->lRobotBankGet;
			RobotConfig.lRobotBankGetBanker = pGameCustomRule->lRobotBankGetBanker;
			RobotConfig.lRobotBankStoMul = pGameCustomRule->lRobotBankStoMul;

			m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_ANDROID_CONFIG, &RobotConfig, sizeof(RobotConfig));
		}

		//发送场景
		return m_pITableFrame->SendGameScene(pIServerUserItem, &StatusScore, sizeof(StatusScore));
	}

	}
	//效验错误
	ASSERT(FALSE);

	return false;
}

//获取百人游戏是否下注状态而且玩家下注了 返回true就有断线重连
bool CTableFrameSink::OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if (m_cbPlayStatus[wChairID] == TRUE && (m_pITableFrame->GetGameStatus() == GS_TK_CALL || m_pITableFrame->GetGameStatus() == GS_TK_SCORE))
		return true;
	else
		return false;
}

//定时器事件
bool CTableFrameSink::OnTimerMessage(DWORD dwTimerID, WPARAM wBindParam)
{
	switch (dwTimerID)
	{
	case IDI_DELAY_ENDGAME:
	{
		m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);
		m_pITableFrame->KillGameTimer(IDI_DELAY_ENDGAME);

		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_WAIT_START);

		m_pITableFrame->SetGameTimer(IDI_AUTO_START,TIME_START,1,0);
		return true;
	}
	case IDI_AUTO_START:
	{
		m_pITableFrame->KillGameTimer(IDI_AUTO_START);

		for (WORD i = 0; i < GAME_PLAYER; i++)
		{
			if (m_bFleeUser[i] == 2)
			{
				IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
				if (pServerUser)
					m_pITableFrame->PerformStandUpAction(pServerUser);
			}
		}

		BYTE cbUserCount = 0;
		for (WORD i = 0; i < GAME_PLAYER; i++)
		{
			IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
			if (pServerUser)
			{
				pServerUser->SetUserStatus(US_READY, pServerUser->GetTableID(), pServerUser->GetChairID());
				cbUserCount++;
			}
		}

		if (cbUserCount >= 2)
			m_pITableFrame->StartGame();

		return true;
	}
	case IDI_SO_OPERATE:
	{
		//删除时间
		m_pITableFrame->KillGameTimer(IDI_SO_OPERATE);

		//游戏状态
		switch (m_pITableFrame->GetGameStatus())
		{
		case GS_TK_CALL:  //用户叫庄
		{
			for (WORD i = 0; i < m_wPlayerNum; i++)
			{
				if (m_cbPlayStatus[i] == TRUE && m_cbCallStatus[i] == INVALID_BANKER)
				{
					OnUserCallBanker(i, OPERATE_NO_BANKERER);
					m_bFleeUser[i] ++;
				}
			}
			break;
		}
		case GS_TK_SCORE:  //下注操作
		{
			for (WORD i = 0; i < m_wPlayerNum; i++)
			{
				if (m_cbPlayStatus[i] == TRUE && m_lTableScore[i] == 0 && m_wBankerUser != i)
				{
					OnUserAddScore(i, OPERATE_SCORE_ONE);
					m_bFleeUser[i] ++;
				}
			}
			break;
		}

		default:
		{
			break;
		}

		}

		return true;
	}
	}

	return true;
}

//游戏消息处理
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_CALL_BANKER:     //用户叫庄
		{
			//效验数据
			ASSERT(wDataSize == sizeof(CMD_C_CallBanker));
			if (wDataSize != sizeof(CMD_C_CallBanker))
			{
				return true;
			}

			//变量定义
			CMD_C_CallBanker * pCallBanker = (CMD_C_CallBanker *)pDataBuffer;

			//消息处理
			return OnUserCallBanker(pIServerUserItem->GetChairID(), pCallBanker->bBanker);
		}
	case SUB_C_ADD_SCORE:     //用户加注
		{
			//效验数据
			ASSERT(wDataSize == sizeof(CMD_C_AddScore));
			if (wDataSize != sizeof(CMD_C_AddScore))
			{
				return true;
			}

			//变量定义
			CMD_C_AddScore * pAddScore = (CMD_C_AddScore *)pDataBuffer;

			//消息处理
			return OnUserAddScore(pIServerUserItem->GetChairID(), pAddScore->lScore);
		}

	}

	return true;
}

//框架消息处理
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

bool CTableFrameSink::OnUserCallBanker(WORD wChairID, BYTE bBanker)
{
	if (m_cbCallStatus[wChairID] != INVALID_BANKER)
		return true;

	//设置抢庄倍数，0 表示不抢
	m_cbCallStatus[wChairID] = bBanker;

	//发送抢庄数据
	CMD_S_Call_Banker CallBanker;
	ZeroMemory(&CallBanker, sizeof(CallBanker));
	CallBanker.cbBanker = wChairID;
	CallBanker.cbStatus = bBanker;
	for (WORD i = 0; i < m_wPlayerNum; i++)
	{
		if (!m_cbPlayStatus[i])
			continue;

		m_pITableFrame->SendTableData(i, SUB_S_CALL_BANKER, &CallBanker, sizeof(CallBanker));
	}

	BYTE cbCallCount = 0;
	BYTE cbRealCount = 0;
	for (WORD i = 0; i < m_wPlayerNum; i++)
	{
		if (m_cbPlayStatus[i] == TRUE)
		{
			cbRealCount++;

			if (m_cbCallStatus[i] != INVALID_BANKER)
				cbCallCount++;
		}
	}

	//分配庄家
	if (cbCallCount == cbRealCount)
	{
		WORD wCallBanker = 0;
		BYTE vbBanker[GAME_PLAYER];

		BYTE cbCall[GAME_PLAYER];
		CopyMemory(cbCall, m_cbCallStatus, sizeof(cbCall));

		//获取最大叫庄倍数
		BYTE cbMaxTimes = 0;
		for (WORD i = 0; i < m_wPlayerNum; i++)
		{
			if (cbCall[i] > cbMaxTimes && cbCall[i] != INVALID_BANKER)
				cbMaxTimes = cbCall[i];
		}

		//获取叫庄数组
		for (WORD i = 0; i < m_wPlayerNum; i++)
		{
			if (cbCall[i] == cbMaxTimes && cbCall[i] != INVALID_BANKER)
				vbBanker[wCallBanker++] = i;
		}

		if (wCallBanker > 0)
		{
			m_wBankerUser = vbBanker[rand() % wCallBanker];
			if (m_cbCallStatus[m_wBankerUser] == 0)
				m_cbCallStatus[m_wBankerUser] = 1;
		}
		else
		{
			m_wBankerUser = INVALID_BANKER;
		}

		if (cbMaxTimes == 0)
			cbMaxTimes = 1;

		//抢庄结果
		CMD_S_Banker_End BankerEnd;
		ZeroMemory(&BankerEnd, sizeof(BankerEnd));
		BankerEnd.cbBanker = m_wBankerUser;

		CopyMemory(BankerEnd.cbCallStatus,m_cbCallStatus,sizeof(BankerEnd.cbCallStatus));


		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_BANKER_END, &BankerEnd, sizeof(BankerEnd));
		

		//设置下注状态
		m_pITableFrame->SetGameStatus(GS_TK_SCORE);

		m_pITableFrame->KillGameTimer(IDI_SO_OPERATE);
		m_pITableFrame->SetGameTimer(IDI_SO_OPERATE, TIME_SO_OVERTIME, 1, 0);

		m_dwPassedTime = (DWORD)time(NULL);

		return true;
	}
	return true;

}

//加注事件
bool CTableFrameSink::OnUserAddScore(WORD wChairID, SCORE lScore)
{
	//状态效验
	if (m_pITableFrame->GetGameStatus() != GS_TK_SCORE && m_lTableScore[wChairID] != 0)
	{
		return true;
	}

	m_lTableScore[wChairID] = lScore;

	CMD_S_AddScore addScore;
	ZeroMemory(&addScore, sizeof(addScore));
	addScore.lAddScoreCount = lScore;
	addScore.wAddScoreUser = wChairID;

	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_ADD_SCORE, &addScore, sizeof(addScore));
	

	//结束游戏
	BYTE cbScoreAddCount = 0;
	BYTE cbRealCount = 0;
	for (WORD i = 0; i < m_wPlayerNum; i++)
	{
		if (m_cbPlayStatus[i] == TRUE)
		{
			cbRealCount++;
			if (m_lTableScore[i] > 0)
				cbScoreAddCount++;
		}
	}
	
	if (cbScoreAddCount == (cbRealCount - 1))
	{
		return OnEventGameConclude(INVALID_CHAIR, NULL, GER_NORMAL);
	}
	return true;
}

//消费能力
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	return 0L;
}

//查询是否扣服务费
bool CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{
	
	if (m_lTableScore[wChairID] > 0 || m_cbCallStatus[wChairID] != INVALID_BANKER)
	{
		return true;
	}

	return false;
}
