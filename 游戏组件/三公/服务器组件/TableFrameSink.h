#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "DlgCustomRule.h"

class ICGame;

//////////////////////////////////////////////////////////////////////////

//游戏桌子类
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//游戏变量
protected:
	WORD			    m_wPlayerNum;				//游戏最大人数
	WORD				m_wBankerUser;				//庄家用户
	WORD			    m_RealPlayers;			    //当前实际玩家
	WORD				m_wCurrentUser;				//当前用户

	//用户数据
public:
	BYTE				m_cbHandCardData[GAME_PLAYER][MAX_COUNT];	//用户扑克
	BYTE				m_cbPlayStatus[GAME_PLAYER];				//当前有效玩家
protected:
	BYTE				m_cbCallStatus[GAME_PLAYER];				//叫庄状态
	SCORE				m_lTableScore[GAME_PLAYER];					//下注数目
	BYTE				m_cbPlayerVlue[GAME_PLAYER];				//玩家点数
	SCORE				m_lCurrentScore[GAME_PLAYER];				//当前局得分

	BYTE				m_bFleeUser[GAME_PLAYER];				//逃跑玩家

	//状态变量
protected:
	DWORD				m_dwPassedTime;								//时间

	bool				m_bWriteRoomStorage;					//是否写库存

	//组件变量
protected:
	CGameLogic				m_GameLogic;              //游戏逻辑
	ITableFrame *			m_pITableFrame;           //框架接口
	tagGameServiceOption *	m_pGameServiceOption;     //配置参数
	tagGameServiceAttrib *	m_pGameServiceAttrib;     //游戏属性

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
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//管理接口
public:
	//初始化
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//复位桌子
	virtual void RepositionSink();

	//游戏事件
public:
	//游戏开始
	virtual bool OnEventGameStart();
	//游戏结束
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//发送场景
	virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);

	//百人游戏获取游戏记录
	virtual void OnGetGameRecord(VOID *GameRecord) {};

	//获取百人游戏是否下注状态而且玩家下注了 返回true就有断线重连
	virtual bool OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem);

	//事件接口
public:
	//定时器事件
	virtual bool OnTimerMessage(DWORD dwTimerID, WPARAM wBindParam);
	//游戏消息处理
	virtual bool OnGameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息处理
	virtual bool OnFrameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//数据事件
	virtual bool OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize) { return true; }

	//查询接口
public:
	//查询限额
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);
	//最少积分
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){ return 0; }
	//查询服务费
	virtual bool QueryBuckleServiceCharge(WORD wChairID);

	//数据事件
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
	{
	return false;
	}
	//积分事件
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason) { return false; }
	//设置基数
	virtual void SetGameBaseScore(SCORE lBaseScore)  { return; }
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
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }

//游戏事件
protected:
	//叫庄事件
	bool OnUserCallBanker(WORD wChairID, BYTE bBanker);
	//加注事件
	bool OnUserAddScore(WORD wChairID, SCORE lScore);
};

//////////////////////////////////////////////////////////////////////////

#endif
