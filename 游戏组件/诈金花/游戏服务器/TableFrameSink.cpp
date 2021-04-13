#include "StdAfx.h"
#include "TableFrameSink.h"
#include "DlgCustomRule.h"
#include <conio.h>
#include <locale>

//////////////////////////////////////////////////////////////////////////


//静态变量
const BYTE			CTableFrameSink::m_GameStartMode=START_MODE_ALL_READY;	//开始模式


//定时器 0~30
#define IDI_GAME_COMPAREEND					1									//结束定时器
#define IDI_GAME_OPENEND					2									//结束定时器
#define IDI_ADD_SCORE						3									//结束定时器
#define IDI_AUTO_ADD						4									//跟到底定时器
#define IDI_AUTO_START						5									//开始定时器
#define IDI_WAIT_START						6									//等待开始定时器


#define TIME_GAME_COMPAREEND				2000								//结束定时器
#define TIME_GAME_OPENEND					1000								//结束定时器
#define TIME_ADD_SCORE						60000								//结束定时器
#define TIME_AUTO_ADD						1000								//跟到底时间
#define TIME_AUTO_START						5000								//开始时间
#define TIME_WAIT_START						4000								//等待开始时间
//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink()
{
	//游戏变量
	m_wPlayerCount = GAME_PLAYER;
	m_bOperaCount=0;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_cbShowHandStatus = false;

	m_cbGameTurns = 1;

	m_bWriteRoomStorage = true;

	//用户状态
	ZeroMemory(&m_StGameEnd,sizeof(m_StGameEnd));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_cbGiveUpUser,sizeof(m_cbGiveUpUser));
	for(int i=0;i<GAME_PLAYER;i++)
	{
		m_bMingZhu[i]=false;
	}
	ZeroMemory(m_cbRealPlayer, sizeof(m_cbRealPlayer));
	ZeroMemory(m_cbAndroidStatus, sizeof(m_cbAndroidStatus));

	ZeroMemory(m_bFleeUser, sizeof(m_bFleeUser));

	//扑克变量
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	FillMemory(m_wCompardUser, sizeof(m_wCompardUser),INVALID_CHAIR);
	ZeroMemory(m_cbCompareCount, sizeof(m_cbCompareCount));

	//下注信息
	m_lMaxCellScore=0L;
	m_lCurrentTimes=0L;		
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lUserMaxScore,sizeof(m_lUserMaxScore));
	ZeroMemory(m_lShowHandScore, sizeof(m_lShowHandScore));
	ZeroMemory(m_bFollowStatus, sizeof(m_bFollowStatus));

	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;	

	ZeroMemory(&m_pCustomAndroid, sizeof(m_pCustomAndroid));

	//随机种子
	srand(time(NULL));

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

//接口查询--检测相关信息版本
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

	//开始模式
	m_pITableFrame->SetStartMode(START_MODE_ALL_READY);

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//读取配置
	ReadConfigInformation();

	//下注变量
	m_lCellScore = m_pITableFrame->GetCellScore();

	TCHAR szConfigFileName[128] = TEXT("");
	//设置文件名
	TCHAR szPath[MAX_PATH] = TEXT("");
	GetCurrentDirectory(sizeof(szPath), szPath);
	_sntprintf(szConfigFileName, sizeof(szConfigFileName), TEXT("%s\\BaiRenConfig\\ZaJinHuaServer-%d.ini"), szPath, m_pGameServiceOption->wServerID);

	m_pCustomAndroid.nLookCard = GetPrivateProfileInt(TEXT("扎金花"), TEXT("nLookCard"), -1, szConfigFileName);
	m_pCustomAndroid.nSingleGiveup = GetPrivateProfileInt(TEXT("扎金花"), TEXT("nSingleGiveup"), -1, szConfigFileName);
	m_pCustomAndroid.nShunziGiveup = GetPrivateProfileInt(TEXT("扎金花"), TEXT("nShunziGiveup"), -1, szConfigFileName);
	m_pCustomAndroid.nLoserGiveup = GetPrivateProfileInt(TEXT("扎金花"), TEXT("nLoserGiveup"), -1, szConfigFileName);
	m_pCustomAndroid.nLoserCompare = GetPrivateProfileInt(TEXT("扎金花"), TEXT("nLoserCompare"), -1, szConfigFileName);
	m_pCustomAndroid.nWinnerCompare = GetPrivateProfileInt(TEXT("扎金花"), TEXT("nWinnerCompare"), -1, szConfigFileName);

	if (m_pCustomAndroid.nLookCard == -1 || m_pCustomAndroid.nSingleGiveup == -1 || m_pCustomAndroid.nShunziGiveup == -1 ||
		m_pCustomAndroid.nLoserGiveup == -1 || m_pCustomAndroid.nLoserCompare == -1 || m_pCustomAndroid.nWinnerCompare == -1)
	{
		CTraceService::TraceString(TEXT("读取配置文件失败！"),TraceLevel_Warning);
		return false;
	}

	return true;
}

//复位桌子
void  CTableFrameSink::RepositionSink()
{
	//游戏变量
	m_bOperaCount=0;
	m_wCurrentUser=INVALID_CHAIR;

	m_cbGameTurns = 1;
	m_cbShowHandStatus = false;

	m_bWriteRoomStorage = true;

	//用户状态
	ZeroMemory(&m_StGameEnd,sizeof(m_StGameEnd));
	ZeroMemory(m_cbGiveUpUser,sizeof(m_cbGiveUpUser));
	for(int i=0;i<m_wPlayerCount;i++)
	{
		m_bMingZhu[i]=false;	
	}
	ZeroMemory(m_cbRealPlayer, sizeof(m_cbRealPlayer));
	ZeroMemory(m_cbAndroidStatus, sizeof(m_cbAndroidStatus));

	//下注信息
	m_lMaxCellScore=0L;							
	m_lCurrentTimes=0L;		
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lUserMaxScore,sizeof(m_lUserMaxScore));
	ZeroMemory(m_lShowHandScore, sizeof(m_lShowHandScore));
	ZeroMemory(m_bFollowStatus, sizeof(m_bFollowStatus));

	return;
}

//积分事件
bool CTableFrameSink::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	

	return false;
}

//开始模式
BYTE  CTableFrameSink::GetGameStartMode()
{
	return m_GameStartMode;
}

//用户断线
bool CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{

	return true;
}

//游戏开始
bool  CTableFrameSink::OnEventGameStart()
{
	BYTE cbUserCount = 0;
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
		if (pServerUser)
		{
			cbUserCount++;
		}
	}

	FillMemory(m_wCompardUser, sizeof(m_wCompardUser), INVALID_CHAIR);
	ZeroMemory(m_cbCompareCount, sizeof(m_cbCompareCount));
	ZeroMemory(m_cbPlayStatus, sizeof(m_cbPlayStatus));

	ZeroMemory(m_bFleeUser, sizeof(m_bFleeUser));

	//扑克变量
	ZeroMemory(m_cbHandCardData, sizeof(m_cbHandCardData));
	m_cbGameTurns = 1;
	m_cbShowHandStatus = false;

	CMD_S_WaitStart WaitStart;
	ZeroMemory(&WaitStart, sizeof(WaitStart));

	WaitStart.cbUserCount = cbUserCount;

	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
		if (pServerUser)
		{
			WaitStart.lPlayerScore = pServerUser->GetUserScore();
			m_pITableFrame->SendTableData(i, SUB_S_WAIT_START, &WaitStart, sizeof(WaitStart));
			m_pITableFrame->SendLookonData(i, SUB_S_WAIT_START);
		}
	}

	m_pITableFrame->SetGameTimer(IDI_AUTO_START, TIME_AUTO_START, 1, 0);
	
	m_dwJettonTime = (DWORD)time(NULL);

	//设置状态
	m_pITableFrame->SetGameStatus(GAME_STATUS_WAIT);
	return true;
}

//游戏结束
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_COMPARECARD:	//比牌结束
	case GER_NO_PLAYER:		//没有玩家
		{
			m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);
			m_pITableFrame->KillGameTimer(IDI_AUTO_ADD);

			//定义变量
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
	
			//唯一玩家
			WORD wWinner,wUserCount=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{	
				if (m_cbPlayStatus[i] == PlayerPlaying || m_cbPlayStatus[i] == PlayerLookCard)
				{
					wUserCount++;
					wWinner=i;
					m_wBankerUser=i;
				}
			}

			//胜利者强退
			if (wUserCount==0)
			{
				wWinner=m_wBankerUser;
			}

			//延迟开始
			bool bDelayOverGame = false;

			//计算总注
			SCORE lWinnerScore = 0;
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{
				if(i==wWinner)continue;
				GameEnd.lGameScore[i]=-m_lTableScore[i];
				lWinnerScore+=m_lTableScore[i];
			}

			//处理税收
			SCORE lGameTax = m_pITableFrame->CalculateRevenue(wWinner, lWinnerScore);
			GameEnd.lGameScore[wWinner]=lWinnerScore-lGameTax;

			//胜利者强退
			if (wUserCount==1)
			{
				//修改积分
				tagScoreInfo ScoreInfo;
				
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));

					if (m_cbPlayStatus[i] != PlayerPlaying && m_cbPlayStatus[i] != PlayerLookCard)
						continue;
					ScoreInfo.cbType =(GameEnd.lGameScore[i]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
					ScoreInfo.lRevenue=lGameTax;                                  
					ScoreInfo.lScore=GameEnd.lGameScore[i];

					if (ScoreInfo.cbType == SCORE_TYPE_LOSE && bDelayOverGame == false)
					{
						IServerUserItem * pUserItem = m_pITableFrame->GetTableUserItem(i);
						if (pUserItem!=NULL && (pUserItem->GetUserScore()+GameEnd.lGameScore[i])<m_pGameServiceOption->lMinTableScore)
						{
							bDelayOverGame = true;
						}
					}

					m_pITableFrame->WriteUserScore(i, ScoreInfo);

					//库存值
					IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);
					if (pServerUser && pServerUser->IsAndroidUser() && m_bWriteRoomStorage)
						m_pGameServiceOption->pRoomStorageOption.lStorageCurrent += ScoreInfo.lScore;
				}
			}

			//大厅跑马灯消息
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
				if (pServerUser && GameEnd.lGameScore[i] > 500)
				{
					CMD_GR_GameRadioMessage GameRadioMessage;
					ZeroMemory(&GameRadioMessage, sizeof(GameRadioMessage));

					BYTE cbCardType = m_GameLogic.GetCardType(m_cbHandCardData[i], MAX_COUNT);

					if (cbCardType == CT_SPECIAL)
						cbCardType = CT_SINGLE;

					GameRadioMessage.cbCardType = cbCardType;
					GameRadioMessage.wKindID = m_pGameServiceOption->wKindID;
					GameRadioMessage.lScore = GameEnd.lGameScore[i];
					lstrcpyn(GameRadioMessage.szNickName, pServerUser->GetNickName(), CountArray(GameRadioMessage.szNickName));
					m_pITableFrame->SendHallRadioMessage(&GameRadioMessage, sizeof(GameRadioMessage));
				}
			}

			if(wChairID==GAME_PLAYER)
			{
				//结束数据
				CopyMemory(&m_StGameEnd,&GameEnd,sizeof(m_StGameEnd));
				m_pITableFrame->SetGameTimer(IDI_GAME_COMPAREEND,TIME_GAME_COMPAREEND,1,0);
			}
			else
			{
				//发送信息
				for (WORD i = 0; i < GAME_PLAYER; i++)
				{
					if (m_cbPlayStatus[i] == PlayerNoPlay)
						continue;

					IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);

					if (pServerUser == NULL)
						continue;

					CopyMemory(GameEnd.cbCardData[i], m_cbHandCardData[i], sizeof(GameEnd.cbCardData[i]));
					GameEnd.cbCardType[i] = m_GameLogic.GetCardType(GameEnd.cbCardData[i], MAX_COUNT);

					for (int j = 0; j < m_cbCompareCount[i]; j++)
					{
						if (m_wCompardUser[i][j] != INVALID_CHAIR)
						{
							WORD cbUser = m_wCompardUser[i][j];
							CopyMemory(GameEnd.cbCardData[cbUser], m_cbHandCardData[cbUser], sizeof(GameEnd.cbCardData[cbUser]));
							GameEnd.cbCardType[cbUser] = m_GameLogic.GetCardType(GameEnd.cbCardData[cbUser], MAX_COUNT);
						}
					}

					for (WORD j = 0; j < GAME_PLAYER; j++)
					{
						if (j == i)
							continue;
						if (m_cbPlayStatus[i] == PlayerPlaying || m_cbPlayStatus[i] == PlayerLookCard)
						{
							if (m_cbPlayStatus[j] == PlayerPlaying || m_cbPlayStatus[j] == PlayerLookCard)
							{
								CopyMemory(GameEnd.cbCardData[j], m_cbHandCardData[j], sizeof(GameEnd.cbCardData[j]));
								GameEnd.cbCardType[j] = m_GameLogic.GetCardType(GameEnd.cbCardData[j], MAX_COUNT);
							}
						}
					}

					m_pITableFrame->SendTableData(i, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));

					ZeroMemory(GameEnd.cbCardData, sizeof(GameEnd.cbCardData));
					ZeroMemory(GameEnd.cbCardType, sizeof(GameEnd.cbCardType));
				}
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

				m_pITableFrame->SetGameStatus(GAME_STATUS_WAIT);

				m_pITableFrame->SetGameTimer(IDI_WAIT_START, TIME_WAIT_START, 1, 0);
			}

			

			return true;
		}
	case GER_USER_LEAVE:		//用户强退
	case GER_NETWORK_ERROR:		//网络中断
		{
			if (m_cbPlayStatus[wChairID] == PlayerNoPlay || m_cbPlayStatus[wChairID] == PlayerGiveUp || m_cbPlayStatus[wChairID] == PlayerComFail)
				return true;

			if (m_pITableFrame->GetGameStatus() != GAME_STATUS_PLAY)
				return true;

			//设置数据
			m_cbGiveUpUser[wChairID] = TRUE;
			m_cbPlayStatus[wChairID] = PlayerGiveUp;

			bool bCurrentGiveUp = false;

			if (wChairID == m_wCurrentUser)
			{
				bCurrentGiveUp = true;

				//获取最后一个玩家的位置
				WORD wFinalUser = m_wBankerUser;
				for (BYTE i = 0; i < m_wPlayerCount; i++)
				{
					if (m_cbPlayStatus[wFinalUser] == PlayerNoPlay || m_cbPlayStatus[wFinalUser] == PlayerGiveUp || m_cbPlayStatus[wFinalUser] == PlayerComFail)
					{
						wFinalUser = (wFinalUser - 1 + m_wPlayerCount) % m_wPlayerCount;
					}
					else
						break;
				}

				//最后一个玩家放弃，轮数++
				if (wChairID == wFinalUser)
				{
					m_cbGameTurns++;
				}

				//用户切换
				WORD wNextPlayer = INVALID_CHAIR;
				for (WORD i = 1; i < m_wPlayerCount; i++)
				{
					//设置变量
					wNextPlayer = (m_wCurrentUser + i) % m_wPlayerCount;

					//继续判断
					if (m_cbPlayStatus[wNextPlayer] == PlayerPlaying || m_cbPlayStatus[wNextPlayer] == PlayerLookCard)
						break;
				}
				m_wCurrentUser = wNextPlayer;

			}

			if (m_cbGameTurns >= MAX_ROUND)
			{
				m_wCurrentUser = INVALID_CHAIR;
			}

			//发送消息
			CMD_S_GiveUp GiveUp;
			GiveUp.wGiveUpUser = wChairID;
			GiveUp.wCurrentUser = bCurrentGiveUp ? m_wCurrentUser : INVALID_CHAIR;
			GiveUp.cbGameTurns = m_cbGameTurns;
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GIVE_UP, &GiveUp, sizeof(GiveUp));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GIVE_UP, &GiveUp, sizeof(GiveUp));

			//修改积分
			tagScoreInfo ScoreInfo;
			ZeroMemory(&ScoreInfo, sizeof(ScoreInfo));
			ScoreInfo.cbType = SCORE_TYPE_FLEE;
			ScoreInfo.lRevenue = 0;
			ScoreInfo.lScore = -m_lTableScore[wChairID];

			m_pITableFrame->WriteUserScore(wChairID, ScoreInfo);

			//库存值
			IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(wChairID);
			if (pServerUser && pServerUser->IsAndroidUser() && m_bWriteRoomStorage)
				m_pGameServiceOption->pRoomStorageOption.lStorageCurrent += ScoreInfo.lScore;

			//人数统计
			WORD wPlayerCount = 0;
			for (WORD i = 0; i<m_wPlayerCount; i++)
			{
				if (m_cbPlayStatus[i] == PlayerPlaying || m_cbPlayStatus[i] == PlayerLookCard) wPlayerCount++;
			}

			//判断结束
			if (wPlayerCount <= 1)
				OnEventGameConclude(INVALID_CHAIR, NULL, GER_NO_PLAYER);

			//开牌判断
			if (m_cbGameTurns >= MAX_ROUND)
			{
				OnEventGameConclude(GAME_PLAYER, NULL, GER_OPENCARD);
				return true;
			}

			//强退处理
			return true;
		}
	case GER_OPENCARD:		//开牌结束   
		{
			m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);
			m_pITableFrame->KillGameTimer(IDI_AUTO_ADD);

			//清理数据
			m_wCurrentUser = INVALID_CHAIR;

			//保存扑克
			BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
			CopyMemory(cbUserCardData, m_cbHandCardData, sizeof(cbUserCardData));

			//比牌玩家
			WORD wWinner = 0;

			//查找最大玩家
			for (WORD i = 1; i<m_wPlayerCount; i++)
			{
				WORD w = (wWinner + i) % m_wPlayerCount;

				//用户过滤
				if (m_cbPlayStatus[w] == PlayerNoPlay || m_cbPlayStatus[w] == PlayerGiveUp || m_cbPlayStatus[w] == PlayerComFail)
					continue;

				if (m_cbPlayStatus[wWinner] == PlayerNoPlay || m_cbPlayStatus[wWinner] == PlayerGiveUp || m_cbPlayStatus[wWinner] == PlayerComFail)
				{
					wWinner = w;
					continue;
				}

				//对比扑克
				if (m_GameLogic.CompareCard(cbUserCardData[w], cbUserCardData[wWinner], MAX_COUNT) >= TRUE)
				{
					wWinner = w;
				}
			}
			
			//定义变量
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//如果全压状态 要把携带金币的差额补给输的玩家
			if (m_cbShowHandStatus)
			{
				SCORE lWinnerShowScore = m_lShowHandScore[wWinner];
				for (WORD i = 0; i < GAME_PLAYER; i++)
				{
					if (m_lShowHandScore[i] > 0 && i != wWinner)
					{
						if (m_lShowHandScore[i] > m_lShowHandScore[wWinner])
						{
							SCORE lSubShowScore = m_lShowHandScore[i] - m_lShowHandScore[wWinner];
							m_lTableScore[i] -= lSubShowScore;
						}
					}
				}
			}

			//计算分数
			SCORE lWinnerScore = 0;
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{
				if(i==wWinner)continue;
				lWinnerScore+=m_lTableScore[i];
				GameEnd.lGameScore[i]=-m_lTableScore[i];
			}

			//处理税收
			SCORE lGameTax = m_pITableFrame->CalculateRevenue(wWinner, lWinnerScore);
			GameEnd.lGameScore[wWinner]=lWinnerScore-lGameTax;

			//修改积分
			tagScoreInfo ScoreInfo;
			ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
			for (WORD i=0; i<m_wPlayerCount; i++) 
			{			
				ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
				if (i == wWinner || m_cbPlayStatus[i] == PlayerNoPlay || m_cbPlayStatus[i] == PlayerGiveUp || m_cbPlayStatus[i] == PlayerComFail)
					continue;
				ScoreInfo.lScore=GameEnd.lGameScore[i];
				ScoreInfo.cbType=SCORE_TYPE_LOSE;

				m_pITableFrame->WriteUserScore(i, ScoreInfo);

				//库存值
				IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);
				if (pServerUser && pServerUser->IsAndroidUser() && m_bWriteRoomStorage)
					m_pGameServiceOption->pRoomStorageOption.lStorageCurrent += ScoreInfo.lScore;
			}

			ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
			ScoreInfo.lScore=GameEnd.lGameScore[wWinner];
			ScoreInfo.cbType=SCORE_TYPE_WIN;
			ScoreInfo.lRevenue=lGameTax;
	
			m_pITableFrame->WriteUserScore(wWinner, ScoreInfo);

			//库存值
			IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(wWinner);
			if (pServerUser && pServerUser->IsAndroidUser() && m_bWriteRoomStorage)
				m_pGameServiceOption->pRoomStorageOption.lStorageCurrent += ScoreInfo.lScore;

			//大厅跑马灯消息
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
				if (pServerUser && GameEnd.lGameScore[i] > 500)
				{
					CMD_GR_GameRadioMessage GameRadioMessage;
					ZeroMemory(&GameRadioMessage, sizeof(GameRadioMessage));

					BYTE cbCardType = m_GameLogic.GetCardType(m_cbHandCardData[i], MAX_COUNT);

					if (cbCardType == CT_SPECIAL)
						cbCardType = CT_SINGLE;

					GameRadioMessage.cbCardType = cbCardType;
					GameRadioMessage.wKindID = m_pGameServiceOption->wKindID;
					GameRadioMessage.lScore = GameEnd.lGameScore[i];
					lstrcpyn(GameRadioMessage.szNickName, pServerUser->GetNickName(), CountArray(GameRadioMessage.szNickName));
					m_pITableFrame->SendHallRadioMessage(&GameRadioMessage, sizeof(GameRadioMessage));
				}
			}

			if(wChairID==GAME_PLAYER)
			{
				//结束数据
				CopyMemory(&m_StGameEnd,&GameEnd,sizeof(m_StGameEnd));
				m_pITableFrame->SetGameTimer(IDI_GAME_OPENEND,TIME_GAME_OPENEND,1,0);
			}
			else
			{
				//发送信息
				for (WORD i = 0; i < GAME_PLAYER; i++)
				{
					if (m_cbPlayStatus[i] == PlayerNoPlay)
						continue;

					IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);

					if (pServerUser == NULL)
						continue;

					CopyMemory(GameEnd.cbCardData[i], m_cbHandCardData[i], sizeof(GameEnd.cbCardData[i]));
					GameEnd.cbCardType[i] = m_GameLogic.GetCardType(GameEnd.cbCardData[i], MAX_COUNT);

					for (int j = 0; j < m_cbCompareCount[i]; j++)
					{
						if (m_wCompardUser[i][j] != INVALID_CHAIR)
						{
							WORD cbUser = m_wCompardUser[i][j];
							CopyMemory(GameEnd.cbCardData[cbUser], m_cbHandCardData[cbUser], sizeof(GameEnd.cbCardData[cbUser]));
							GameEnd.cbCardType[cbUser] = m_GameLogic.GetCardType(GameEnd.cbCardData[cbUser], MAX_COUNT);
						}
					}

					for (WORD j = 0; j < GAME_PLAYER; j++)
					{
						if (j == i)
							continue;

						if (m_cbPlayStatus[i] == PlayerPlaying || m_cbPlayStatus[i] == PlayerLookCard)
						{
							if (m_cbPlayStatus[j] == PlayerPlaying || m_cbPlayStatus[j] == PlayerLookCard)
							{
								CopyMemory(GameEnd.cbCardData[j], m_cbHandCardData[j], sizeof(GameEnd.cbCardData[j]));
								GameEnd.cbCardType[j] = m_GameLogic.GetCardType(GameEnd.cbCardData[j], MAX_COUNT);
							}
						}
					}

					m_pITableFrame->SendTableData(i, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));

					ZeroMemory(GameEnd.cbCardData, sizeof(GameEnd.cbCardData));
					ZeroMemory(GameEnd.cbCardType, sizeof(GameEnd.cbCardType));
				}
				m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));

				m_pITableFrame->SetGameStatus(GAME_STATUS_WAIT);

				m_pITableFrame->SetGameTimer(IDI_WAIT_START, TIME_WAIT_START, 1, 0);
			}
			
			
			return true;
		}
	case GER_DISMISS:		//游戏解散
		{
			m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);
			m_pITableFrame->KillGameTimer(IDI_AUTO_ADD);

			//变量定义
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			m_pITableFrame->SetGameStatus(GAME_STATUS_WAIT);

			m_pITableFrame->SetGameTimer(IDI_WAIT_START, TIME_WAIT_START, 1, 0);

			return true;
		}
	}

	return false;
}

//发送场景
bool  CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:		//空闲状态
	case GAME_STATUS_WAIT:		//等待开始状态
		{
			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			if (cbGameStatus == GAME_STATUS_WAIT)
			{
				DWORD dwRestTime = (DWORD)time(NULL) - m_dwJettonTime;
				StatusFree.nTimeLeave = (DWORD)(TIME_AUTO_START/1000 - __min(TIME_AUTO_START / 1000, dwRestTime));
			}

			//设置变量
			StatusFree.nTotalTime = TIME_ADD_SCORE / 1000;
			
			StatusFree.lCellScore = m_lCellScore;
			StatusFree.lPlayerScore = pIServerUserItem->GetUserScore();
			StatusFree.nMaxRound = MAX_ROUND;

			//获取自定义配置			
			if (pIServerUserItem->IsAndroidUser())
			{
				m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_ANDROID_CONFIG, &m_pCustomAndroid, sizeof(m_pCustomAndroid));
			}
			

			//发送场景
			bool bResult = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

			return bResult;
		}
	case GAME_STATUS_PLAY:	//游戏状态
		{
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay, sizeof(StatusPlay));
			
			DWORD dwRestTime = (DWORD)time(NULL) - m_dwJettonTime;
			StatusPlay.nTimeLeave = (DWORD)(TIME_ADD_SCORE/1000 - (__min(TIME_ADD_SCORE / 1000, dwRestTime)));
			StatusPlay.nTotalTime = TIME_ADD_SCORE / 1000;
			//加注信息
			StatusPlay.lPlayerScore = m_lUserMaxScore[wChairID];
			StatusPlay.lCellScore=m_lCellScore;

			StatusPlay.lMinScore = m_lCurrentTimes;
			StatusPlay.lMaxScore = __min(10 * m_lCellScore, m_lUserMaxScore[wChairID]);

			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				StatusPlay.lAllTableScore += m_lTableScore[i];
			}

			StatusPlay.nCurrentRound = m_cbGameTurns;
			StatusPlay.nMaxRound = MAX_ROUND;

			StatusPlay.cbFollowStatus = m_bFollowStatus[wChairID] ? TRUE : FALSE;

			//设置变量
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			CopyMemory(StatusPlay.cbPlayStatus,m_cbPlayStatus,sizeof(m_cbPlayStatus));

			CopyMemory(StatusPlay.lPlayerBetScore, m_lTableScore, sizeof(StatusPlay.lPlayerBetScore));

			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				if (m_lShowHandScore[i] > 0)
				{
					StatusPlay.cbPlayStatus[i] = PlayerShowHand;
				}
			}

			//获取自定义配置			
			if (pIServerUserItem->IsAndroidUser())
			{
				m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_ANDROID_CONFIG, &m_pCustomAndroid, sizeof(m_pCustomAndroid));
			}

			//设置扑克
			if(m_bMingZhu[wChairID])
				CopyMemory(&StatusPlay.cbHandCardData,&m_cbHandCardData[wChairID],MAX_COUNT);

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	//效验错误
	ASSERT(FALSE);

	return false;
}

//获取百人游戏是否下注状态而且玩家下注了 返回true就有断线重连
bool CTableFrameSink::OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	bool bPlay = m_cbPlayStatus[wChairID] == PlayerPlaying || m_cbPlayStatus[wChairID] == PlayerLookCard || m_cbPlayStatus[wChairID] == PlayerShowHand;
	if ((bPlay && m_pITableFrame->GetGameStatus() == GAME_STATUS_PLAY) || m_pITableFrame->GetGameStatus() == GAME_STATUS_WAIT)
		return true;
	else
		return false;
}

//定时器事件 
bool  CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	//结束定时
	if(wTimerID==IDI_GAME_COMPAREEND || IDI_GAME_OPENEND==wTimerID)
	{
		//删除时间
		if(wTimerID==IDI_GAME_COMPAREEND)m_pITableFrame->KillGameTimer(IDI_GAME_COMPAREEND);
		else m_pITableFrame->KillGameTimer(IDI_GAME_OPENEND);

		//发送信息
		for (WORD i = 0; i < GAME_PLAYER; i++)
		{
			if (m_cbPlayStatus[i] == PlayerNoPlay)
				continue;

			IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);

			if (pServerUser == NULL)
				continue;

			CopyMemory(m_StGameEnd.cbCardData[i], m_cbHandCardData[i], sizeof(m_StGameEnd.cbCardData[i]));
			m_StGameEnd.cbCardType[i] = m_GameLogic.GetCardType(m_StGameEnd.cbCardData[i], MAX_COUNT);

			for (int j = 0; j < m_cbCompareCount[i]; j++)
			{
				if (m_wCompardUser[i][j] != INVALID_CHAIR)
				{
					WORD cbUser = m_wCompardUser[i][j];
					CopyMemory(m_StGameEnd.cbCardData[cbUser], m_cbHandCardData[cbUser], sizeof(m_StGameEnd.cbCardData[cbUser]));
					m_StGameEnd.cbCardType[cbUser] = m_GameLogic.GetCardType(m_StGameEnd.cbCardData[cbUser], MAX_COUNT);
				}
			}

			for (WORD j = 0; j < GAME_PLAYER; j++)
			{
				if (j == i)
					continue;

				if (m_cbPlayStatus[i] == PlayerPlaying || m_cbPlayStatus[i] == PlayerLookCard)
				{
					if (m_cbPlayStatus[j] == PlayerPlaying || m_cbPlayStatus[j] == PlayerLookCard)
					{
						CopyMemory(m_StGameEnd.cbCardData[j], m_cbHandCardData[j], sizeof(m_StGameEnd.cbCardData[j]));
						m_StGameEnd.cbCardType[j] = m_GameLogic.GetCardType(m_StGameEnd.cbCardData[j], MAX_COUNT);
					}
				}
			}

			m_pITableFrame->SendTableData(i, SUB_S_GAME_END, &m_StGameEnd, sizeof(m_StGameEnd));

			ZeroMemory(m_StGameEnd.cbCardData, sizeof(m_StGameEnd.cbCardData));
			ZeroMemory(m_StGameEnd.cbCardType, sizeof(m_StGameEnd.cbCardType));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_END, &m_StGameEnd, sizeof(m_StGameEnd));

		m_pITableFrame->SetGameStatus(GAME_STATUS_WAIT);

		m_pITableFrame->SetGameTimer(IDI_WAIT_START, TIME_WAIT_START, 1, 0);
		return true;
	}
	if(wTimerID==IDI_ADD_SCORE)
	{
		m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);
		if (m_wCurrentUser!=INVALID_CHAIR)
		{
			m_bFleeUser[m_wCurrentUser] = true;
			OnUserGiveUp(m_wCurrentUser,false);
		}		
		return true;
	}
	if (wTimerID == IDI_AUTO_ADD)
	{
		m_pITableFrame->KillGameTimer(IDI_AUTO_ADD);
		if (m_wCurrentUser != INVALID_CHAIR && !m_cbShowHandStatus)
		{
			SCORE lScore = m_lCurrentTimes * m_lCellScore;
			if (m_bMingZhu[m_wCurrentUser])
				lScore *= 2;
			OnUserAddScore(m_wCurrentUser, lScore);
		}
		return true;
	}
	if (wTimerID == IDI_WAIT_START)
	{
		m_pITableFrame->KillGameTimer(IDI_WAIT_START);

		//结束游戏
		m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);

		for (WORD i = 0; i < GAME_PLAYER; i++)
		{
			if (m_bFleeUser[i])
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
	if (wTimerID == IDI_AUTO_START)
	{
		m_pITableFrame->KillGameTimer(IDI_AUTO_START);

		BYTE cbUserCount = 0;
		for (WORD i = 0; i < GAME_PLAYER; i++)
		{
			IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
			if (pServerUser)
			{
				cbUserCount++;
			}
		}

		if (cbUserCount >= 2)
			OnGameAutoStart();
		return true;
	}
	return false;
}

//游戏消息处理
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_GIVE_UP:			//用户放弃
		{
			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

			//状态判断
			if (m_cbPlayStatus[pUserData->wChairID] == PlayerNoPlay || m_cbPlayStatus[pUserData->wChairID] == PlayerGiveUp || m_cbPlayStatus[pUserData->wChairID] == PlayerComFail)
			{
				return true;
			}

			//消息处理
			return OnUserGiveUp(pUserData->wChairID);
		}
	case SUB_C_LOOK_CARD:		//用户看牌
		{
			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

			//状态判断
			if (m_cbPlayStatus[pUserData->wChairID] != PlayerPlaying)
				return true;

			//消息处理
			return OnUserLookCard(pUserData->wChairID);
		}
	case SUB_C_COMPARE_CARD:	//用户比牌
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_CompareCard));
			if (wDataSize!=sizeof(CMD_C_CompareCard)) return false;

			//变量定义
			CMD_C_CompareCard * pCompareCard=(CMD_C_CompareCard *)pData;

			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

			//参数效验
			ASSERT(pUserData->wChairID==m_wCurrentUser);
			if (pUserData->wChairID != m_wCurrentUser)
			{
				CMD_S_FailResult FailResult;
				_sntprintf(FailResult.szInfo, sizeof(FailResult.szInfo), TEXT("还没有轮到您说话，不能比牌！"));
				m_pITableFrame->SendTableData(pUserData->wChairID, SUB_S_PLAYER_COMMPARE_FALURE, &FailResult, sizeof(FailResult));
				return true;
			}

			//状态判断
			if ( (m_cbPlayStatus[pUserData->wChairID] != PlayerPlaying && m_cbPlayStatus[pUserData->wChairID] != PlayerLookCard)
				|| (m_cbPlayStatus[pCompareCard->wCompareUser] != PlayerPlaying && m_cbPlayStatus[pCompareCard->wCompareUser] != PlayerLookCard))
			{
				CMD_S_FailResult FailResult;
				_sntprintf(FailResult.szInfo, sizeof(FailResult.szInfo), TEXT("您或您要比牌的对象不在游戏状态，不能比牌！"));
				m_pITableFrame->SendTableData(pUserData->wChairID, SUB_S_PLAYER_COMMPARE_FALURE, &FailResult, sizeof(FailResult));
				return true;
			}

			//消息处理
			return OnUserCompareCard(pUserData->wChairID,pCompareCard->wCompareUser);
		}
	case SUB_C_ADD_SCORE:		//用户加注
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_AddScore));
			if (wDataSize!=sizeof(CMD_C_AddScore)) 
				return false;

			//变量定义
			CMD_C_AddScore * pAddScore=(CMD_C_AddScore *)pData;

			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

			//状态判断
			if (m_cbPlayStatus[pUserData->wChairID] == PlayerNoPlay || m_cbPlayStatus[pUserData->wChairID] == PlayerGiveUp || m_cbPlayStatus[pUserData->wChairID] == PlayerComFail)
			{
				CMD_S_FailResult FailResult;
				_sntprintf(FailResult.szInfo, sizeof(FailResult.szInfo),TEXT("您当前不在游戏状态，不能下注！"));
				m_pITableFrame->SendTableData(pUserData->wChairID, SUB_S_PLAYER_BET_FALURE, &FailResult, sizeof(FailResult));
				return true;
			}

			//消息处理
			return OnUserAddScore(pUserData->wChairID,pAddScore->lScore);
		}
	case SUB_C_OPEN_CARD:
		{
			//用户效验
			tagUserInfo * pUserData = pIServerUserItem->GetUserInfo();

			return OnUserOpenCard(pUserData->wChairID);
		}
	case SUB_C_FOLLOW_END:
		{
			//用户效验
			tagUserInfo * pUserData = pIServerUserItem->GetUserInfo();

			WORD wChairID = pUserData->wChairID;

			if (m_cbPlayStatus[wChairID] != PlayerPlaying && m_cbPlayStatus[wChairID] != PlayerLookCard)
				return true;

			m_bFollowStatus[wChairID] = m_bFollowStatus[wChairID] ? false : true;

			CMD_S_Follow pFollow;
			pFollow.cbFollowStatus = m_bFollowStatus[wChairID] ? TRUE : FALSE;

			m_pITableFrame->SendTableData(wChairID, SUB_S_PLAYER_FOLLOW_END, &pFollow, sizeof(pFollow));

			return true;
		}
	case SUB_C_PLAYER_ALLIN:
		{
			//用户效验
			tagUserInfo * pUserData = pIServerUserItem->GetUserInfo();

			WORD wChairID = pUserData->wChairID;

			if (m_cbPlayStatus[wChairID] != PlayerPlaying && m_cbPlayStatus[wChairID] != PlayerLookCard)
				return true;

			return OnUserShowHand(wChairID);
		}
	case SUB_C_REQUEST_CONTROL:		//控制信息请求
		{
			//权限判断
			if (CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) == false)
				return true;

			CMD_S_ControlInfo ControlInfo;
			ZeroMemory(&ControlInfo, sizeof(ControlInfo));

			CopyMemory(ControlInfo.cbHandCardData, m_cbHandCardData, sizeof(ControlInfo.cbHandCardData));

			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				IServerUserItem *pUser = m_pITableFrame->GetTableUserItem(i);
				if (pUser && pUser->IsAndroidUser())
					ControlInfo.bIsAndroid[i] = true;
			}

			BYTE cbCardListData[52] =
			{
				0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,	//方块 A - K
				0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,	//梅花 A - K
				0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,	//红桃 A - K
				0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D	//黑桃 A - K
			};

			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				if (m_cbPlayStatus[i] == FALSE && m_cbGiveUpUser[i] == FALSE)
					continue;

				for (BYTE j = 0; j < MAX_COUNT; j++)
				{
					for (BYTE k = 0; k < 52; k++)
					{
						if (cbCardListData[k] == m_cbHandCardData[i][j])
						{
							cbCardListData[k] = 0;
							break;
						}
					}
				}
			}

			BYTE cbCardPos = 0;
			for (BYTE i = 0; i < 52; i++)
			{
				if (cbCardListData[i] != 0)
					ControlInfo.cbCanChangeCard[cbCardPos++] = cbCardListData[i];
			}

			m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_RESPONSE_CONTROL, &ControlInfo, sizeof(ControlInfo));

			return true;
		}
	case SUB_C_CHANGE_CARD:				//改变手牌
		{
			//权限判断
			if (CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) == false)
				return true;

			CMD_C_ChangeCard *pChangeCard = (CMD_C_ChangeCard *)pData;

			WORD wChairID = pIServerUserItem->GetChairID();

			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				if (m_cbPlayStatus[i] == PlayerNoPlay)
					continue;
				if (i == wChairID)
					continue;

				for (BYTE j = 0; j < MAX_COUNT; j++)
				{
					for (BYTE k = 0; k < MAX_COUNT; k++)
					{
						if (m_cbHandCardData[i][j] == pChangeCard->cbCardData[k])
						{
							return true;
						}
					}
				}
			}

			CopyMemory(m_cbHandCardData[wChairID], pChangeCard->cbCardData, sizeof(m_cbHandCardData[wChairID]));

			CMD_S_ControlChangeCard ControlChangeCard;
			ZeroMemory(&ControlChangeCard, sizeof(ControlChangeCard));

			m_pITableFrame->SendTableData(wChairID, SUB_S_CHANGE_SUCCESS, &ControlChangeCard, sizeof(ControlChangeCard));

			return true;
		}
	}

	return false;
}

//框架消息处理
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}



//用户坐下
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	
	
	return true;
}

//用户起立
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{


	return true;
}

//用户同意
bool CTableFrameSink::OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{
	return true;
}

//游戏开始
bool CTableFrameSink::OnGameAutoStart()
{
	//删除定时器
	m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);
	m_pITableFrame->KillGameTimer(IDI_AUTO_ADD);
	m_pITableFrame->KillGameTimer(IDI_WAIT_START);

	//设置状态
	m_pITableFrame->SetGameStatus(GAME_STATUS_PLAY);

	//最大下注
	BYTE cbAiCount = 0;
	BYTE cbPlayCount = 0;

	for (WORD i = 0; i<m_wPlayerCount; i++)
	{
		//获取用户
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem == NULL) continue;

		const SCORE lUserScore = pIServerUserItem->GetUserScore();

		//设置变量
		m_cbPlayStatus[i] = PlayerPlaying;

		m_lUserMaxScore[i] = lUserScore;

		cbPlayCount++;
		if (pIServerUserItem->IsAndroidUser() == true)
		{
			cbAiCount++;
			m_cbAndroidStatus[i] = TRUE;
		}
		else
		{
			m_cbRealPlayer[i] = TRUE;
		}
	}

	if (cbAiCount == 0 || cbAiCount == cbPlayCount)
		m_bWriteRoomStorage = false;

	m_lCurrentTimes = 1;
	m_lMaxCellScore = m_lCellScore * 10;

	//库存衰减
	if (m_bWriteRoomStorage)
		m_pITableFrame->CalculateStorageDeduct();

	//分发扑克
	m_GameLogic.RandCardList(m_cbHandCardData[0], sizeof(m_cbHandCardData) / sizeof(m_cbHandCardData[0][0]));

	//游戏开始
	tag_ZhaJinHua_Result ZhaJinHua_Result;
	ZeroMemory(&ZhaJinHua_Result, sizeof(ZhaJinHua_Result));
	m_pITableFrame->GetControlResult(NULL, 0, &ZhaJinHua_Result);


	if (ZhaJinHua_Result.cbTableCardArray[0][0] != 0)
	{
		CopyMemory(m_cbHandCardData, ZhaJinHua_Result.cbTableCardArray, sizeof(m_cbHandCardData));
	}

	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		m_GameLogic.SortCardList(m_cbHandCardData[i], MAX_COUNT);
	}



	//机器人消息
	for (WORD i = 0; i < m_wPlayerCount; i++)
	{
		//机器人数据
		IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if (pServerUserItem == NULL) continue;
		if (pServerUserItem->IsAndroidUser() == true)
		{
			//机器人数据
			CMD_S_AndroidCard AndroidCard;
			ZeroMemory(&AndroidCard, sizeof(AndroidCard));

			//设置变量
			CopyMemory(AndroidCard.cbRealPlayer, m_cbRealPlayer, sizeof(m_cbRealPlayer));
			CopyMemory(AndroidCard.cbAndroidStatus, m_cbAndroidStatus, sizeof(m_cbAndroidStatus));
			CopyMemory(AndroidCard.cbAllHandCardData, m_cbHandCardData, sizeof(m_cbHandCardData));
			CopyMemory(AndroidCard.cbPlayStatus, m_cbPlayStatus, sizeof(AndroidCard.cbPlayStatus));
			m_pITableFrame->SendTableData(i, SUB_S_ANDROID_CARD, &AndroidCard, sizeof(AndroidCard));
		}
	}


	//设置庄家
	if (m_wBankerUser == INVALID_CHAIR) m_wBankerUser = rand() % m_wPlayerCount;

	//庄家离开
	if (m_wBankerUser<m_wPlayerCount && m_cbPlayStatus[m_wBankerUser] == PlayerNoPlay)
		m_wBankerUser = rand() % m_wPlayerCount;

	//确定庄家
	while (m_cbPlayStatus[m_wBankerUser] == PlayerNoPlay)
	{
		m_wBankerUser = (m_wBankerUser + 1) % m_wPlayerCount;
	}

	//用户设置
	for (WORD i = 0; i<m_wPlayerCount; i++)
	{
		if (m_cbPlayStatus[i] == PlayerPlaying)
		{
			m_lTableScore[i] = m_lCellScore;
		}
	}

	//当前用户
	m_wCurrentUser = (m_wBankerUser + 1) % m_wPlayerCount;
	while (m_cbPlayStatus[m_wCurrentUser] == PlayerNoPlay)
	{
		m_wCurrentUser = (m_wCurrentUser + 1) % m_wPlayerCount;
	}

	//构造数据
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart, sizeof(GameStart));
	GameStart.wCurrentUser = m_wCurrentUser;
	GameStart.wBankerUser = m_wBankerUser;
	GameStart.lMinScore = m_lCurrentTimes;

	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem * pServerUser = m_pITableFrame->GetTableUserItem(i);
		if (pServerUser)
		{
			GameStart.lPlayerRestScore[i] = pServerUser->GetUserScore();
		}
	}

	//发送数据
	for (WORD i = 0; i < m_wPlayerCount; i++)
	{
		if (m_cbPlayStatus[i] == PlayerPlaying)
		{
			GameStart.lUserMaxScore = m_lUserMaxScore[i];
			GameStart.lMaxScore = __min(10 * m_lCellScore, GameStart.lUserMaxScore);
			m_pITableFrame->SendTableData(i, SUB_S_GAME_START, &GameStart, sizeof(GameStart));
		}
		m_pITableFrame->SendLookonData(i, SUB_S_GAME_START, &GameStart, sizeof(GameStart));
	}

	//设置定时器 非房卡房间
	m_pITableFrame->SetGameTimer(IDI_ADD_SCORE, TIME_ADD_SCORE, 1, 0);
	m_dwJettonTime = (DWORD)time(NULL);
	return true;
}

//放弃事件
bool CTableFrameSink::OnUserGiveUp(WORD wChairID,bool bExit)
{
	//设置数据
	m_cbGiveUpUser[wChairID] = TRUE;
	m_cbPlayStatus[wChairID] = PlayerGiveUp;

	bool bExchangeCurrentUser = false;
	if (wChairID == m_wCurrentUser)
	{
		bExchangeCurrentUser = true;
		//获取最后一个玩家的位置
		WORD wFinalUser = m_wBankerUser;
		for (BYTE i = 0; i < m_wPlayerCount; i++)
		{
			if (m_cbPlayStatus[wFinalUser] == PlayerNoPlay || m_cbPlayStatus[wFinalUser] == PlayerGiveUp || m_cbPlayStatus[wFinalUser] == PlayerComFail)
			{
				wFinalUser = (wFinalUser - 1 + m_wPlayerCount) % m_wPlayerCount;
			}
			else
				break;
		}

		//最后一个玩家放弃，轮数++
		if (wChairID == wFinalUser)
		{
			m_cbGameTurns++;
		}

		//用户切换
		WORD wNextPlayer = INVALID_CHAIR;
		for (WORD i = 1; i < m_wPlayerCount; i++)
		{
			//设置变量
			wNextPlayer = (m_wCurrentUser + i) % m_wPlayerCount;

			//继续判断
			if (m_cbPlayStatus[wNextPlayer] == PlayerPlaying || m_cbPlayStatus[wNextPlayer] == PlayerLookCard)
				break;
		}
		m_wCurrentUser = wNextPlayer;
	}

	if (m_cbGameTurns >= MAX_ROUND)
	{
		m_wCurrentUser = INVALID_CHAIR;
	}

	//发送消息
	CMD_S_GiveUp GiveUp;
	GiveUp.wGiveUpUser=wChairID;
	GiveUp.wCurrentUser = bExchangeCurrentUser ? m_wCurrentUser : INVALID_CHAIR;
	GiveUp.cbGameTurns = m_cbGameTurns;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));

	//修改积分
	tagScoreInfo ScoreInfo;
	ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
	ScoreInfo.cbType =(bExit)?SCORE_TYPE_FLEE:SCORE_TYPE_LOSE;
	ScoreInfo.lRevenue=0;  
	ScoreInfo.lScore=-m_lTableScore[wChairID];

	m_pITableFrame->WriteUserScore(wChairID, ScoreInfo);

	//库存值
	IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(wChairID);
	if (pServerUser && pServerUser->IsAndroidUser() && m_bWriteRoomStorage)
		m_pGameServiceOption->pRoomStorageOption.lStorageCurrent += ScoreInfo.lScore;

	//人数统计
	WORD wPlayerCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i] == PlayerPlaying || m_cbPlayStatus[i] == PlayerLookCard) wPlayerCount++;
	}

	//判断结束
	if (wPlayerCount<=1)
		OnEventGameConclude(INVALID_CHAIR,NULL,GER_NO_PLAYER);

	//开牌判断
	if (m_cbGameTurns >= MAX_ROUND)
	{
		OnEventGameConclude(GAME_PLAYER, NULL, GER_OPENCARD);
		return true;
	}

	if (m_wCurrentUser != INVALID_CHAIR && bExchangeCurrentUser && m_bFollowStatus[m_wCurrentUser])
		m_pITableFrame->SetGameTimer(IDI_AUTO_ADD, TIME_AUTO_ADD, 1, 0);

	if (bExchangeCurrentUser)
	{
		//删除定时器
		m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);

		//设置定时器
		m_pITableFrame->SetGameTimer(IDI_ADD_SCORE, TIME_ADD_SCORE, 1, 0);

		m_dwJettonTime = (DWORD)time(NULL);
	}

	return true;
}

//看牌事件
bool CTableFrameSink::OnUserLookCard(WORD wChairID)
{
	//参数效验
	ASSERT(!m_bMingZhu[wChairID]);
	if (m_bMingZhu[wChairID]) return true;

	//设置参数
	m_bMingZhu[wChairID]=true;
	m_cbPlayStatus[wChairID] = PlayerLookCard;

	//构造数据
	CMD_S_LookCard LookCard;
	ZeroMemory(&LookCard,sizeof(LookCard));
	LookCard.wLookCardUser=wChairID;

	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_LOOK_CARD,&LookCard,sizeof(LookCard));

	for(WORD i = 0;i<m_wPlayerCount;i++)
	{
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if(pIServerUserItem == NULL ) continue;
		if(i == wChairID)
		{
			CopyMemory(LookCard.cbCardData,m_cbHandCardData[wChairID],sizeof(m_cbHandCardData[0]));
			LookCard.cbCardType = m_GameLogic.GetCardType(LookCard.cbCardData,MAX_COUNT);
		}
		else
		{
			ZeroMemory(LookCard.cbCardData,sizeof(LookCard.cbCardData));
		}
		m_pITableFrame->SendTableData(i,SUB_S_LOOK_CARD,&LookCard,sizeof(LookCard));
	}

	return true;
}

//比牌事件
bool CTableFrameSink::OnUserCompareCard(WORD wFirstChairID, WORD wNextChairID, bool bNeedScore)
{
	//if (m_cbShowHandStatus)
	//{
	//	CMD_S_FailResult FailResult;
	//	_sntprintf(FailResult.szInfo, sizeof(FailResult.szInfo), TEXT("当前处于全压状态，不能比牌！"));
	//	m_pITableFrame->SendTableData(wFirstChairID, SUB_S_PLAYER_COMMPARE_FALURE, &FailResult, sizeof(FailResult));
	//	return true;
	//}

	//当前倍数
	LONGLONG lTimes = m_bMingZhu[wFirstChairID] ? 4 : 2;
	
	SCORE lCompareScore = lTimes * m_lCellScore * m_lCurrentTimes;

	//全压也会来这里，不用扣钱
	if (!bNeedScore)
		lCompareScore = 0;

	if ((lCompareScore + m_lTableScore[wFirstChairID]) > m_lUserMaxScore[wFirstChairID])
	{
		CMD_S_FailResult FailResult;
		_sntprintf(FailResult.szInfo, sizeof(FailResult.szInfo), TEXT("金额超出您携带分数，不能比牌！"));
		m_pITableFrame->SendTableData(wFirstChairID, SUB_S_PLAYER_COMMPARE_FALURE, &FailResult, sizeof(FailResult));
		return true;
	}

	//用户注金
	m_lTableScore[wFirstChairID] += lCompareScore;

	//比较大小
	BYTE cbWin=m_GameLogic.CompareCard(m_cbHandCardData[wFirstChairID],m_cbHandCardData[wNextChairID],MAX_COUNT);

	//胜利用户
	WORD wLostUser,wWinUser;
	if(cbWin==TRUE) 
	{
		wWinUser=wFirstChairID;
		wLostUser=wNextChairID;
	}
	else
	{
		wWinUser=wNextChairID;
		wLostUser=wFirstChairID;
	}

	//设置数据
	m_cbPlayStatus[wLostUser] = PlayerComFail;
	m_wCompardUser[wLostUser][m_cbCompareCount[wLostUser]++] = wWinUser;
	m_wCompardUser[wWinUser][m_cbCompareCount[wWinUser]++] = wLostUser;

	//人数统计
	WORD wPlayerCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i] == PlayerPlaying || m_cbPlayStatus[i] == PlayerLookCard) wPlayerCount++;
	}

	//继续游戏
	if(wPlayerCount>=2)
	{
		//获取最后一个玩家的位置
		WORD wFinalUser = m_wBankerUser;
		for (BYTE i = 0; i < m_wPlayerCount; i++)
		{
			if (m_cbPlayStatus[wFinalUser] == PlayerNoPlay || m_cbPlayStatus[wFinalUser] == PlayerGiveUp || m_cbPlayStatus[wFinalUser] == PlayerComFail)
			{
				wFinalUser = (wFinalUser - 1 + m_wPlayerCount) % m_wPlayerCount;
			}
			else
				break;
		}

		//最后一个玩家加注，轮数++
		if (wFirstChairID == wFinalUser)
		{
			m_cbGameTurns++;
		}

		//用户切换
		WORD wNextPlayer=INVALID_CHAIR;
		for (WORD i=1;i<m_wPlayerCount;i++)
		{
			//设置变量
			wNextPlayer=(m_wCurrentUser+i)%m_wPlayerCount;

			//继续判断
			if (m_cbPlayStatus[wNextPlayer] == PlayerPlaying || m_cbPlayStatus[wNextPlayer] == PlayerLookCard)
				break;
		}
		//设置用户
		m_wCurrentUser=wNextPlayer;
	}
	else m_wCurrentUser=INVALID_CHAIR;

	if (m_cbGameTurns >= MAX_ROUND)
	{
		m_wCurrentUser = INVALID_CHAIR;
	}

	//构造数据
	CMD_S_CompareCard CompareCard;
	ZeroMemory(&CompareCard, sizeof(CompareCard));
	CompareCard.wCurrentUser = m_wCurrentUser;
	CompareCard.wCompareUser[0] = wFirstChairID;
	CompareCard.wCompareUser[1] = wNextChairID;
	CompareCard.lCompareScore = lCompareScore;
	CompareCard.lPlayerScore = m_lUserMaxScore[wFirstChairID] - m_lTableScore[wFirstChairID];
	CompareCard.lPlayerTableScore = m_lTableScore[wFirstChairID];

	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		CompareCard.lAllTableScore += m_lTableScore[i];
	}

	CompareCard.cbGameTurns = m_cbGameTurns;
	CompareCard.wWinUser = wWinUser;

	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_COMPARE_CARD, &CompareCard, sizeof(CompareCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_COMPARE_CARD, &CompareCard, sizeof(CompareCard));
	

	//修改积分
	tagScoreInfo ScoreInfo;
	ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
	ScoreInfo.cbType =SCORE_TYPE_LOSE;
	ScoreInfo.lRevenue=0;
	ScoreInfo.lScore=-m_lTableScore[wLostUser];

	m_pITableFrame->WriteUserScore(wLostUser, ScoreInfo);

	//库存值
	IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(wLostUser);
	if (pServerUser && pServerUser->IsAndroidUser() && m_bWriteRoomStorage)
		m_pGameServiceOption->pRoomStorageOption.lStorageCurrent += ScoreInfo.lScore;

	//结束游戏
	if (wPlayerCount<2)
	{
		m_wBankerUser=wWinUser;
		OnEventGameConclude(GAME_PLAYER,NULL,GER_COMPARECARD);
		return true;
	}
	else
	{
		if (m_cbGameTurns >= MAX_ROUND)
		{
			OnEventGameConclude(GAME_PLAYER, NULL, GER_OPENCARD);
			return true;
		}

		//删除定时器
		m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);

		if (m_wCurrentUser != INVALID_CHAIR && m_bFollowStatus[m_wCurrentUser])
			m_pITableFrame->SetGameTimer(IDI_AUTO_ADD, TIME_AUTO_ADD, 1, 0);

		//设置定时器
		m_pITableFrame->SetGameTimer(IDI_ADD_SCORE, TIME_ADD_SCORE, 1, 0);

		m_dwJettonTime = (DWORD)time(NULL);
	}
	return true;
}

//开牌事件
bool CTableFrameSink::OnUserOpenCard(WORD wUserID)
{
	//构造数据
	CMD_S_OpenCard OpenCard;
	OpenCard.wOpenUser = wUserID;
	CopyMemory(OpenCard.cbCardData, m_cbHandCardData[wUserID], sizeof(OpenCard.cbCardData));

	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_OPEN_CARD, &OpenCard, sizeof(OpenCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_OPEN_CARD, &OpenCard, sizeof(OpenCard));
	return true;
}


//全压事件
bool CTableFrameSink::OnUserShowHand(WORD wChairID)
{

	//状态效验
	ASSERT(m_wCurrentUser == wChairID);
	if (m_wCurrentUser != wChairID) return true;

	BYTE cbRestCount = 0;

	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		if (m_cbPlayStatus[i] == PlayerPlaying || m_cbPlayStatus[i] == PlayerLookCard)
		{
			cbRestCount++;
		}
	}

	//机器人防错误处理
	IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(wChairID);
	if (pServerUser && pServerUser->IsAndroidUser())
	{
		if (m_cbGameTurns < 3 || cbRestCount != 2)
		{
			SCORE lScore = m_lCurrentTimes * m_lCellScore;
			if (m_bMingZhu[wChairID])
				lScore *= 2;
			OnUserAddScore(wChairID, lScore);
			return true;
		}
	}

	if (m_cbGameTurns < 3 || cbRestCount != 2)
		return true;

	SCORE lMaxBetScore = 10 * 20 * m_lCellScore;

	if (m_bMingZhu[wChairID])
		lMaxBetScore *= 2;

	lMaxBetScore = __min(lMaxBetScore, m_lUserMaxScore[wChairID]);

	SCORE lScore = lMaxBetScore - m_lTableScore[wChairID];

	//用户注金
	m_lTableScore[wChairID] += lScore;
	m_lShowHandScore[wChairID] = lScore;

	//设置用户
	//用户切换
	WORD wNextPlayer = INVALID_CHAIR;
	for (WORD i = 1; i<m_wPlayerCount; i++)
	{
		//设置变量
		wNextPlayer = (m_wCurrentUser + i) % m_wPlayerCount;

		//继续判断
		if (m_cbPlayStatus[wNextPlayer] == PlayerPlaying || m_cbPlayStatus[wNextPlayer] == PlayerLookCard)
			break;
	}
	m_wCurrentUser = wNextPlayer;

	//获取剩余全压的玩家数量
	BYTE cbShowHandCount = 0;
	WORD wShowHandUser[GAME_PLAYER];
	FillMemory(wShowHandUser, sizeof(wShowHandUser),INVALID_CHAIR);
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		if (m_cbPlayStatus[i] == PlayerPlaying || m_cbPlayStatus[i] == PlayerLookCard)
		{
			if (m_lShowHandScore[i] > 0)
			{
				wShowHandUser[cbShowHandCount] = i;
				cbShowHandCount++;
			}
		}
	}

	if (cbShowHandCount >= 2)
		m_wCurrentUser = INVALID_CHAIR;

	//构造数据
	CMD_S_AddScore AddScore;
	ZeroMemory(&AddScore, sizeof(AddScore));
	AddScore.wCurrentUser = m_wCurrentUser;
	AddScore.wAddScoreUser = wChairID;
	AddScore.lAddScoreCount = lScore;
	AddScore.cbGameTurns = m_cbGameTurns;

	m_cbShowHandStatus = true;
	AddScore.cbShowHandStatus = m_cbShowHandStatus;
	
	AddScore.lPlayerScore = m_lUserMaxScore[wChairID] - m_lTableScore[wChairID];
	AddScore.lPlayerTableScore = m_lTableScore[wChairID];

	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		AddScore.lAllTableScore += m_lTableScore[i];
	}

	//发送数据
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);

		if (!pServerUser) continue;

		AddScore.lMinScore = m_lUserMaxScore[i] - m_lTableScore[i];
		AddScore.lMaxScore = m_lUserMaxScore[i] - m_lTableScore[i];

		m_pITableFrame->SendTableData(i, SUB_S_SHOW_HAND, &AddScore, sizeof(AddScore));
		m_pITableFrame->SendLookonData(i, SUB_S_SHOW_HAND, &AddScore, sizeof(AddScore));
	}

	if (cbShowHandCount >= 2)
	{
		if (cbShowHandCount == 2)
		{
			OnUserCompareCard(wShowHandUser[0], wShowHandUser[1],false);
			return true;
		}
		else
		{
			OnEventGameConclude(GAME_PLAYER, NULL, GER_OPENCARD);
			return true;
		}
	}
	else
	{
		//删除定时器
		m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);

		//设置定时器
		m_pITableFrame->SetGameTimer(IDI_ADD_SCORE, TIME_ADD_SCORE, 1, 0);

		m_dwJettonTime = (DWORD)time(NULL);
	}

	return true;
}


//加注事件
bool CTableFrameSink::OnUserAddScore(WORD wChairID, SCORE lScore)
{
	if (m_cbShowHandStatus)
	{
		CMD_S_FailResult FailResult;
		_sntprintf(FailResult.szInfo, sizeof(FailResult.szInfo), TEXT("当前处于全压状态，不能下注！"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLAYER_BET_FALURE, &FailResult, sizeof(FailResult));
		return true;
	}
	//状态效验
	ASSERT(m_wCurrentUser==wChairID);
	if (m_wCurrentUser != wChairID)
	{
		CMD_S_FailResult FailResult;
		_sntprintf(FailResult.szInfo, sizeof(FailResult.szInfo), TEXT("还没有轮到您说话，不能下注！"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLAYER_BET_FALURE, &FailResult, sizeof(FailResult));
		return true;
	}

	//金币效验
	ASSERT((lScore+m_lTableScore[wChairID])<=m_lUserMaxScore[wChairID]);
	DOUBLE dMor = lScore / m_lCellScore;
	int nMor = lScore / m_lCellScore;
	if (lScore < 0 || (dMor - nMor) > 0)
	{
		CMD_S_FailResult FailResult;
		_sntprintf(FailResult.szInfo, sizeof(FailResult.szInfo), TEXT("下注金额错误，不能下注！"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLAYER_BET_FALURE, &FailResult, sizeof(FailResult));
		return true;
	}
	if ((lScore + m_lTableScore[wChairID]) > m_lUserMaxScore[wChairID])
	{
		CMD_S_FailResult FailResult;
		_sntprintf(FailResult.szInfo, sizeof(FailResult.szInfo), TEXT("下注金额超出您携带分数，不能下注！"));
		m_pITableFrame->SendTableData(wChairID, SUB_S_PLAYER_BET_FALURE, &FailResult, sizeof(FailResult));
		return true;
	}

	//当前倍数
	LONGLONG lTimes=m_bMingZhu[wChairID]?2:1;
	LONGLONG lTemp=lScore/m_lCellScore/lTimes;

	if (!(m_lCurrentTimes <= lTemp && lTemp <= m_lMaxCellScore / m_lCellScore))
	{
		IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(wChairID);

		//如果机器人发送了一个下注倍数错误的消息，帮它改成跟注
		if (pServerUser && pServerUser->IsAndroidUser())
		{
			lScore = m_lCellScore * m_lCurrentTimes;
			lTemp = m_lCurrentTimes;
			if (m_bMingZhu[wChairID])
				lScore *= 2;
		}
		else
		{
			CMD_S_FailResult FailResult;
			_sntprintf(FailResult.szInfo, sizeof(FailResult.szInfo), TEXT("下注金额错误，不能下注！"));
			m_pITableFrame->SendTableData(wChairID, SUB_S_PLAYER_BET_FALURE, &FailResult, sizeof(FailResult));
			return true;
		}
	}

	//判断玩家是否跟注后钱不够跟剩下的玩家比牌了，如果是的话 帮他自动比牌
	//{
	//	BYTE cbUserCount = 0;
	//	for (WORD i = 0; i < GAME_PLAYER; i++)
	//	{
	//		if (m_cbPlayStatus[i] == PlayerPlaying || m_cbPlayStatus[i] == PlayerLookCard)
	//			cbUserCount++;
	//	}
	//	if (cbUserCount > 2)
	//	{
	//		LONGLONG lUserRestScore = m_lUserMaxScore[wChairID] - m_lTableScore[wChairID];
	//		LONGLONG lRestCompareScore = cbUserCount * 40 * m_lCellScore;
	//		if ((lUserRestScore - lScore) < lRestCompareScore)
	//		{
	//			WORD wCompareUser = INVALID_CHAIR;
	//			for (WORD i = 1; i < GAME_PLAYER; i++)
	//			{
	//				WORD wTempUser = (wChairID + i) % GAME_PLAYER;
	//				if (wTempUser == wChairID)
	//					continue;
	//				if (m_cbPlayStatus[wTempUser] == PlayerPlaying || m_cbPlayStatus[wTempUser] == PlayerLookCard)
	//				{
	//					wCompareUser = wTempUser;
	//				}
	//			}

	//			if (wCompareUser != INVALID_CHAIR)
	//			{
	//				OnUserCompareCard(wChairID,wCompareUser);
	//				return true;
	//			}
	//		}
	//	}
	//}

	if (lTemp <= 10)
		m_lCurrentTimes = lTemp;

	//用户注金
	m_lTableScore[wChairID]+=lScore;
	

	//获取最后一个玩家的位置
	WORD wFinalUser = m_wBankerUser;
	for (BYTE i = 0; i < m_wPlayerCount; i++)
	{
		if (m_cbPlayStatus[wFinalUser] == PlayerNoPlay || m_cbPlayStatus[wFinalUser] == PlayerGiveUp || m_cbPlayStatus[wFinalUser] == PlayerComFail)
		{
			wFinalUser = (wFinalUser - 1 + m_wPlayerCount) % m_wPlayerCount;
		}
		else
			break;
	}

	//最后一个玩家加注，轮数++
	if (wChairID == wFinalUser)
	{
		m_cbGameTurns++;
	}
	
	//用户切换
	WORD wNextPlayer=INVALID_CHAIR;
	for (WORD i=1;i<m_wPlayerCount;i++)
	{
		//设置变量
		wNextPlayer=(m_wCurrentUser+i)%m_wPlayerCount;

		//继续判断
		if (m_cbPlayStatus[wNextPlayer] == PlayerPlaying || m_cbPlayStatus[wNextPlayer] == PlayerLookCard)
			break;
	}
	m_wCurrentUser=wNextPlayer;
	
	if (m_cbGameTurns >= MAX_ROUND)
	{
		m_wCurrentUser = INVALID_CHAIR;
	}

	//构造数据
	CMD_S_AddScore AddScore;
	ZeroMemory(&AddScore, sizeof(AddScore));
	AddScore.wCurrentUser=m_wCurrentUser;
	AddScore.wAddScoreUser=wChairID;
	AddScore.lAddScoreCount=lScore;
	AddScore.cbGameTurns = m_cbGameTurns;

	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		AddScore.lAllTableScore += m_lTableScore[i];
	}

	AddScore.lMinScore = m_lCurrentTimes;
	AddScore.lPlayerTableScore = m_lTableScore[wChairID];
	AddScore.lPlayerScore = m_lUserMaxScore[wChairID] - m_lTableScore[wChairID];

	//发送数据
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);

		if (!pServerUser) continue;

		AddScore.lMaxScore = __min(10 * m_lCellScore, AddScore.lPlayerScore);

		m_pITableFrame->SendTableData(i, SUB_S_ADD_SCORE, &AddScore, sizeof(AddScore));
		m_pITableFrame->SendLookonData(i, SUB_S_ADD_SCORE, &AddScore, sizeof(AddScore));
	}

	if (m_cbGameTurns >= MAX_ROUND)
	{
		OnEventGameConclude(GAME_PLAYER, NULL, GER_OPENCARD);
		return true;
	}

	//删除定时器
	m_pITableFrame->KillGameTimer(IDI_ADD_SCORE);

	if (m_wCurrentUser != INVALID_CHAIR && m_bFollowStatus[m_wCurrentUser])
		m_pITableFrame->SetGameTimer(IDI_AUTO_ADD, TIME_AUTO_ADD, 1, 0);

	//设置定时器
	m_pITableFrame->SetGameTimer(IDI_ADD_SCORE,TIME_ADD_SCORE,1,0);

	m_dwJettonTime = (DWORD)time(NULL);
	
	
	return true;
}

//读取配置
void CTableFrameSink::ReadConfigInformation()
{	
	//获取自定义配置
	tagCustomRule *pCustomRule = (tagCustomRule *)m_pGameServiceOption->cbCustomRule;
	ASSERT(pCustomRule);

	m_pCustomAndroid.lRobotScoreMin = pCustomRule->lRobotScoreMin;
	m_pCustomAndroid.lRobotScoreMax = pCustomRule->lRobotScoreMax;
	m_pCustomAndroid.lRobotBankGet = pCustomRule->lRobotBankGet;
	m_pCustomAndroid.lRobotBankGetBanker = pCustomRule->lRobotBankGetBanker;
	m_pCustomAndroid.lRobotBankStoMul = pCustomRule->lRobotBankStoMul;
}
//////////////////////////////////////////////////////////////////////////
