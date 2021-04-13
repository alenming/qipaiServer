#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//游戏桌子类
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
//总下注数
protected:
	SCORE							m_lAllJettonScore[AREA_COUNT];						//全体总注

//个人下注
protected:
	SCORE							m_lUserJettonScore[GAME_PLAYER][AREA_COUNT];		//个人总注

	SCORE							m_lUserTypeScore[GAME_PLAYER][7];					//类型下注

//上局信息
public:
	bool							m_bUserListWin[GAME_PLAYER][USER_LIST_COUNT];		//列表玩家获胜次数
	SCORE							m_lUserListScore[GAME_PLAYER][USER_LIST_COUNT];		//列表玩家下注数目
	BYTE							m_cbUserPlayCount[GAME_PLAYER];						//列表玩家游戏局数
	//控制变量
protected:
	SCORE							m_lAreaLimitScore;						//区域限制
	SCORE							m_lUserLimitScore;						//区域限制
	SCORE							m_lTypeLimitScore[7];					//类型限制

	//玩家成绩
protected:
	SCORE							m_lUserWinScore[GAME_PLAYER];			//玩家成绩
	SCORE							m_lUserReturnScore[GAME_PLAYER];		//返回下注
	SCORE							m_lUserRevenue[GAME_PLAYER];			//玩家税收

	SCORE							m_lPlayScore[GAME_PLAYER][AREA_COUNT];	//玩家输赢

	SCORE							m_lPreviousAreaScore[GAME_PLAYER][AREA_COUNT];	//玩家上一轮下注

	//控制操作
protected:
	BYTE							m_cbJuControlTimes;						//局控次数
	BYTE							m_cbJuControl;							//是否进行局控
	BYTE							m_cbJuControlArea;						//本局控制哪门胜利 0不控制，1控制赢，2控制输

	//扑克信息
protected:
	
    BYTE							m_cbTableCard;							//桌面扑克	
	SCORE							m_nChips[MAX_CHIP_COUNT];				//筹码

	//状态变量
protected:
	DWORD							m_dwJettonTime;							//开始时间

	//记录变量
protected:
	tagServerGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];	//游戏记录
	int								m_nRecordFirst;							//开始记录
	int								m_nRecordLast;							//最后记录
	
	//控制变量
protected:
	TCHAR							m_szConfigFileName[MAX_PATH];			//配置文件

	//时间设置
protected:						
	BYTE							m_cbBetTime;							//下注时间
	BYTE							m_cbEndTime;							//结束时间

	//组件变量
protected:

	CGameLogic						m_GameLogic;							//游戏逻辑

	//组件接口
protected:
	ITableFrame	*					m_pITableFrame;							//框架接口
	tagGameServiceOption *			m_pGameServiceOption;					//服务配置
	tagGameServiceAttrib *			m_pGameServiceAttrib;					//游戏属性

	//属性变量
protected:
	static const WORD				m_wPlayerCount;							//游戏人数

	BYTE							m_nAreaNums[AREA_COUNT][18];
	
	//函数定义
public:
	//构造函数
	CTableFrameSink();
	//析构函数
	virtual ~CTableFrameSink();

	//基础接口
public:
	//释放对象
	virtual VOID  Release() {delete this;}

	//接口查询
	virtual void *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//管理接口
public:
	//初始化
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//复位桌子
	virtual VOID RepositionSink();

	//查询接口
public:
	//查询限额
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);
	//最少积分
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//数据事件
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//积分事件
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
	{
		return false;
	}

	//游戏事件
public:
	//游戏开始
	virtual bool OnEventGameStart();
	//游戏结束
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//发送场景
	virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//百人游戏获取游戏记录
	virtual void OnGetGameRecord(VOID *GameRecord);

	//获取百人游戏是否下注状态而且玩家下注了
	virtual bool OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem);

	//事件接口
public:
	//定时器事件
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//游戏消息处理
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息处理
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//数据事件
	virtual bool OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize);
	//扣除服务费
	virtual bool QueryBuckleServiceCharge(WORD wChairID);
	//比赛接口
public:
	//设置基数
	virtual void SetGameBaseScore(SCORE lBaseScore){};

	//动作事件
public:
	//用户断线
	virtual bool  OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) ;

	//用户重入
	virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }

	//用户坐下
	virtual bool  OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起来
	virtual bool  OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户同意
	virtual bool  OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize){ return true; }

	//游戏事件
protected:
	//加注事件
	bool OnUserPlaceJetton(WORD wChairID, BYTE cbJettonType, BYTE cbJettonArea[], SCORE lJettonScore, int nKeys);
	//请求用户列表
	bool OnUserRequestUserList(WORD wChairID);
	//获取前6个用户列表的GameID
	bool OnGetUserListGameID(WORD wSeatUser[MAX_SEAT_COUNT]);

	//辅助函数
private:
	//发送扑克
	void DispatchTableCard();
	//发送记录
	void SendGameRecord(IServerUserItem *pIServerUserItem);
	//下注计算
private:
	void   GetAllWinArea(BYTE bcWinArea[],BYTE bcAreaCount,BYTE InArea);
	//最大下注
	SCORE GetUserMaxJetton(WORD wChairID, BYTE Area[], BYTE cbType);
	
	//游戏统计
private:
	//计算得分
	SCORE CalculateScore();
	//读取配置
	void ReadConfigInformation();

	//获取胜利区域
	BYTE cbGetWinArea(BYTE cbTableCard,BYTE cbWinArea[]);
	//获取区域倍数
	BYTE cbGetAreaTimes(BYTE cbArea);

	//初始化区域数字
	void InitAreaNums();
};

//////////////////////////////////////////////////////////////////////////

#endif
