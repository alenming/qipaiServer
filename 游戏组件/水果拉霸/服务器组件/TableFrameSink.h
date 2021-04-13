#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"
#include "GameLogic.h"
#include "DlgCustomRule.h"


/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//游戏桌子
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	int								m_lBet;								//押线下注
	SCORE							m_lBetScore;						//押线下注
	SCORE							m_lOneGameScore;					//押线得分
	SCORE							m_lThreeGameScore;					//玛丽得分
	SCORE							m_lJackPotScore;					//奖池积分
	BYTE							m_cbItemInfo[ITEM_Y_COUNT][ITEM_X_COUNT];	//开奖信息

	int								m_lFreeBet;							//记录免费次数获取时的下注
	BYTE							m_cbAllFreeTimes;					//总的免费次数
	BYTE							m_cbUsedFreeTimes;					//使用的免费次数

	//组件变量
protected:
	CGameLogic						m_GameLogic;						//游戏逻辑

	//组件接口
protected:
	ITableFrame	*					m_pITableFrame;						//框架接口
	tagCustomRule *					m_pGameCustomRule;					//自定规则
	tagGameServiceOption *			m_pGameServiceOption;				//游戏配置
	tagGameServiceAttrib *			m_pGameServiceAttrib;				//游戏属性

	//函数定义
public:
	//构造函数
	CTableFrameSink();
	//析构函数
	virtual ~CTableFrameSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//管理接口
public:
	//复位桌子
	virtual VOID RepositionSink();
	//配置桌子
	virtual bool Initialization(IUnknownEx * pIUnknownEx);

	//查询接口
public:
	//查询限额
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);
	//最少积分
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem);
	//查询是否扣服务费
	virtual bool QueryBuckleServiceCharge(WORD wChairID){return true;}

	//游戏事件
public:
	//游戏开始
	virtual bool OnEventGameStart();
	//游戏结束
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//发送场景
	virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//百人游戏获取游戏记录
	virtual void OnGetGameRecord(VOID *GameRecord) {};

	//获取百人游戏是否下注状态而且玩家下注了 返回true就有断线重连
	virtual bool OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem) { return false; };

	//事件接口
public:
	//时间事件
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//数据事件
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize);
	//积分事件
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);

	//网络接口
public:
	//游戏消息
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//比赛接口
public:
	//设置基数
	virtual void SetGameBaseScore(SCORE lBaseScore);

	//用户事件
public:
	//用户断线
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem);
	//用户重入
	virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem);
	//用户坐下
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起立
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户同意
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);

	//游戏事件
protected:
	//押线开始
	bool OnUserBetStart(IServerUserItem * pIServerUserItem, int lBet);
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

#endif