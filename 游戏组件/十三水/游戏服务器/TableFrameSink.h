#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DlgCustomRule.h"
#include "..\消息定义\CMD_Thirteenx.h"
#include "GameLogic.h"
#include<algorithm>
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

//游戏桌子类
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//模式变量
protected:
	BYTE								m_cbPlayerCount;									//指定游戏人数，2-4

	//游戏变量
protected:
	SCORE								m_lCellScore;										//基础积分
	SCORE								m_lGameScore[GAME_PLAYER];							//输赢总积分
	SCORE								m_lScoreTimes[GAME_PLAYER][3];						//输赢注数
	SCORE								m_lAddScoreTimes[GAME_PLAYER][3];					//附加注数
	SCORE								m_lSpecialScoreTimes[GAME_PLAYER];					//特殊牌型分
	SCORE								m_lGunScoreTimes[GAME_PLAYER];						//打枪柱数
	SCORE								m_lHomeRunsScoreTimes[GAME_PLAYER];					//全垒打注数
	WORD								m_wFireList[GAME_PLAYER][GAME_PLAYER-1];			//开枪列表
	WORD								m_wFireCount[GAME_PLAYER];							//开枪次数
	BYTE								m_bSpecialType[GAME_PLAYER];						//特殊牌型
	BYTE								m_bUserCardData[GAME_PLAYER][HAND_CARD_COUNT];		//玩家扑克
	BYTE								m_bSegmentCard[GAME_PLAYER][3][5];					//分段扑克

	BYTE								m_cbPlayerStatus[GAME_PLAYER];						//玩家状态

	WORD								m_wShowCardCount;									//摊牌计数
	DWORD								m_dwOperateTime;									//时间计数

	bool								m_bFinishSegment[GAME_PLAYER];						//完成分段

	bool								m_bAutoUser[GAME_PLAYER];							//托管出牌玩家，游戏结束要踢出去

	bool								m_bWriteRoomStorage;					//是否写库存

//组件变量
protected:
	CGameLogic							m_GameLogic;										//游戏逻辑
	ITableFrame							*m_pITableFrame;									//框架接口
	tagGameServiceOption				*m_pGameServiceOption;								//配置参数
	tagCustomRule*						m_pGameCustomRule;									//自定规则

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

	//获取百人游戏是否下注状态而且玩家下注了
	virtual bool OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem);

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

	//////////////////////////////////////////////////////////////////////////
	//用户事件
public:
	//用户断线
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//用户重入
	virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//用户坐下
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起立
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户同意
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);
	//设置基数
	virtual void SetGameBaseScore(SCORE lBaseScore){};

	//消息处理
protected:
	//开始发牌
	bool OnGameAutoStart();
	//玩家摊牌
	bool OnUserShowCard(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);

	//通用函数
public:
	//获取倍数
	void CompareCardGetTimes();
	//开始比牌
	VOID StartCompareCard();
};

//////////////////////////////////////////////////////////////////////////

#endif
