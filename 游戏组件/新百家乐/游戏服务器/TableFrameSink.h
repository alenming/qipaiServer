#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "ServerControl.h"

//////////////////////////////////////////////////////////////////////////
//历史记录
#define MAX_SCORE_HISTORY			72									//历史个数
//////////////////////////////////////////////////////////////////////////

//游戏桌子类
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//用户信息
protected:
	SCORE							m_lUserStartScore[GAME_PLAYER];		//起始分数

	bool							m_bUserListWin[GAME_PLAYER][USER_LIST_COUNT];		//列表玩家获胜次数
	SCORE							m_lUserListScore[GAME_PLAYER][USER_LIST_COUNT];		//列表玩家下注数目
	BYTE							m_cbUserPlayCount[GAME_PLAYER];						//列表玩家游戏局数

	//下注数
protected:
	SCORE							m_lAllBet[AREA_MAX];					//总下注
	SCORE							m_lPlayBet[GAME_PLAYER][AREA_MAX];		//玩家下注
	
	int								m_nChip[MAX_CHIP_COUNT];								//筹码配置

	BYTE							m_cbMianYong[GAME_PLAYER];				//是否免佣金

	//分数
protected:
	SCORE							m_lBankerScore;							//庄家积分
	SCORE							m_lPlayScore[GAME_PLAYER][AREA_MAX];	//玩家输赢
	SCORE							m_lUserWinScore[GAME_PLAYER];			//玩家成绩
	SCORE							m_lUserRevenue[GAME_PLAYER];			//玩家税收

	//控制变量
protected:
	SCORE							m_lAreaLimitScore;						//区域限制
	SCORE							m_lUserLimitScore;						//区域限制

	SCORE							m_lApplyBankerCondition;				//申请条件
	TCHAR							m_szConfigFileName[MAX_PATH];			//配置文件

	//局控
	BYTE							m_cbJuControlTimes;						//局控次数
	BYTE							m_cbJuControl;							//是否进行局控
	BYTE							m_cbJuControlArea[AREA_MAX];			//本局控制哪门胜利 0不控制，1控制赢，2控制输


	//扑克信息
protected:
	BYTE							m_cbCardCount[2];						//扑克数目
    BYTE							m_cbTableCardArray[2][3];				//桌面扑克
	BYTE							m_cbOpenResult[AREA_MAX];				//开奖结果

	//状态变量
protected:
	DWORD							m_dwBetTime;							//开始时间

	//庄家信息
protected:
	CWHArray<WORD>					m_ApplyUserArray;						//申请玩家
	WORD							m_wCurrentBanker;						//当前庄家
	WORD							m_wOfflineBanker;						//离线庄家
	WORD							m_wBankerTime;							//做庄次数
	SCORE							m_lBankerWinScore;						//累计成绩
	SCORE							m_lBankerCurGameScore;					//当前成绩
	bool							m_bEnableSysBanker;						//系统做庄

	//记录变量
protected:
	tagServerGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];	//游戏记录
	int								m_nRecordFirst;							//开始记录
	int								m_nRecordLast;							//最后记录
	DWORD							m_dwRecordCount;						//记录数目

	//组件变量
protected:
	CGameLogic						m_GameLogic;							//游戏逻辑
	ITableFrame	*					m_pITableFrame;							//框架接口
	tagGameServiceOption		* m_pGameServiceOption;					//配置参数


	//控制变量
protected:
	bool							m_bRefreshCfg;							//每盘刷新
	TCHAR							m_szGameRoomName[SERVER_LEN];			//房间名称

	//庄家设置
protected:
	//加庄局数设置：当庄家坐满设定的局数之后(m_nBankerTimeLimit)，
	//所带金币值还超过下面申请庄家列表里面所有玩家金币时，
	//可以再加坐庄m_nBankerTimeAdd局，加庄局数可设置。
	LONGLONG						m_nBankerTimeLimit;							//最大庄家数
	LONGLONG						m_nBankerTimeAdd;							//庄家增加数

	//金币超过m_lExtraBankerScore之后，就算是下面玩家的金币值大于他的金币值，他也可以再加庄m_nExtraBankerTime局。
	SCORE							m_lExtraBankerScore;						//庄家钱
	LONGLONG						m_nExtraBankerTime;						//庄家钱大时,坐庄增加数

	//最大庄家数
	LONGLONG						m_lPlayerBankerMAX;						//玩家最大庄家数

	//换庄
	bool							m_bExchangeBanker;						//交换庄家

	//时间设置
protected:
	BYTE							m_cbFreeTime;							//空闲时间
	BYTE							m_cbBetTime;							//下注时间
	BYTE							m_cbEndTime;							//结束时间

	//机器人控制
	int								m_nMaxChipRobot;						//最大数目 (下注机器人)
	int								m_nChipRobotCount;						//人数统计 (下注机器人)
	SCORE							m_lRobotAreaLimit;						//区域统计 (机器人)
	SCORE							m_lRobotAreaScore[AREA_MAX];		//区域统计 (机器人)
	int								m_nRobotListMaxCount;					//最多人数

	//函数定义
public:
	//构造函数
	CTableFrameSink();
	//析构函数
	virtual ~CTableFrameSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release();
	//是否有效
	virtual bool IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//接口查询
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//管理接口
public:
	//初始化
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//复位桌子
	virtual void RepositionSink();

	//查询接口
public:
	//查询限额
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);
	//最少积分
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){ return 0; }
	//查询是否扣服务费
	virtual bool QueryBuckleServiceCharge(WORD wChairID);

	//比赛接口
public:
	//设置基数
	virtual void SetGameBaseScore(SCORE lBaseScore){};

public:
	//数据事件
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//积分事件
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);

	//游戏事件
public:
	//游戏开始
	virtual bool OnEventGameStart();
	//游戏结束
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//发送场景
	virtual bool OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//百人游戏获取游戏记录
	virtual void OnGetGameRecord(VOID *GameRecord);

	//获取百人游戏是否下注状态而且玩家下注了
	virtual bool OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem);

	//事件接口
public:
	//定时器事件
	virtual bool OnTimerMessage(DWORD dwTimerID, WPARAM dwBindParameter);
	//游戏消息处理
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息处理
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//动作事件
public:
	//用户断线
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem);
	//用户重入
	virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//用户坐下
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起来
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户同意
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize){ return true; }
	
	//游戏事件
protected:
	//加注事件
	bool OnUserPlayBet(WORD wChairID, BYTE cbBetArea, SCORE lBetScore);
	//申请庄家
	bool OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem);
	//取消申请
	bool OnUserCancelBanker(IServerUserItem *pICancelServerUserItem);
	//请求用户列表
	bool OnUserRequestUserList(WORD wChairID);
	//获取前6个用户列表的GameID
	bool OnGetUserListGameID(WORD wSeatUser[MAX_SEAT_COUNT]);

	//辅助函数
private:
	//发送扑克
	bool DispatchTableCard();
	//发送庄家
	void SendApplyUser( IServerUserItem *pServerUserItem );
	//更换庄家
	bool ChangeBanker(bool bCancelCurrentBanker);
	//轮换判断
	void TakeTurns();
	//发送记录
	void SendGameRecord(IServerUserItem *pIServerUserItem);
	//读取配置
	void ReadConfigInformation();


	//下注计算
private:
	//最大下注
	SCORE GetMaxPlayerScore(BYTE cbBetArea, WORD wChairID);

	//游戏统计
private:
	//游戏结束计算
	SCORE GameOver();
	//计算得分
	SCORE CalculateScore(OUT SCORE& lBankerWinScore, OUT tagServerGameRecord& GameRecord);
	//推断赢家
	void DeduceWinner(BYTE* pWinArea);
};

//////////////////////////////////////////////////////////////////////////

#endif
