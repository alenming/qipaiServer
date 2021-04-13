#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//游戏桌子类
class CTableFrameSink :public ITableFrameSink, public ITableUserAction
{
	//游戏变量
protected:
	BYTE							m_bOperaCount;							//操作次数
	WORD							m_wCurrentUser;							//当前用户
	WORD							m_wBankerUser;							//庄家用户

	BYTE							m_cbGameTurns;							//游戏轮数

	bool							m_cbShowHandStatus;						//梭哈状态
	//用户状态
protected:
	BYTE							m_cbPlayStatus[GAME_PLAYER];			//游戏状态
	BYTE							m_cbGiveUpUser[GAME_PLAYER];			//游戏状态
	CMD_S_GameEnd					m_StGameEnd;							//结束数据
	BYTE							m_cbRealPlayer[GAME_PLAYER];			//真人玩家
	BYTE							m_cbAndroidStatus[GAME_PLAYER];			//机器状态

	bool							m_bFleeUser[GAME_PLAYER];

	//扑克变量
protected:
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//桌面扑克

	WORD							m_wCompardUser[GAME_PLAYER][GAME_PLAYER-1];//比过牌的玩家
	BYTE							m_cbCompareCount[GAME_PLAYER];			//比过牌的次数

	DWORD							m_dwJettonTime;							//开始时间

	//下注信息
protected:
	SCORE							m_lTableScore[GAME_PLAYER];				//下注数目
	SCORE							m_lUserMaxScore[GAME_PLAYER];			//最大下注
	SCORE							m_lMaxCellScore;						//单元上限
	SCORE							m_lCellScore;							//单元下注
	LONGLONG						m_lCurrentTimes;						//当前倍数

	SCORE							m_lShowHandScore[GAME_PLAYER];			//全压金币

	bool							m_bMingZhu[GAME_PLAYER];				//看明下注
	bool							m_bFollowStatus[GAME_PLAYER];			//跟到底状态

	bool							m_bWriteRoomStorage;					//是否写库存

	//组件变量
protected:
	CGameLogic						m_GameLogic;							//游戏逻辑
	ITableFrame*					m_pITableFrame;							//框架接口
	tagGameServiceOption*			m_pGameServiceOption;					//配置参数

	tagCustomAndroid				m_pCustomAndroid;						//机器人配置

	//AI变量
protected:
	bool							m_bUpdataStorage;						//更新库存


	//属性变量
protected:
	WORD							m_wPlayerCount;							//游戏人数
	static const BYTE				m_GameStartMode;						//开始模式


	//函数定义
public:
	//构造函数
	CTableFrameSink();
	//析构函数
	virtual ~CTableFrameSink();

	//基础接口
public:
	//释放对象
	virtual VOID  Release();
	//是否有效
	virtual bool  IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//接口查询
	virtual void *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//管理接口
public:
	//初始化
	virtual bool  Initialization(IUnknownEx * pIUnknownEx);
	//复位桌子
	virtual void  RepositionSink();
	//查询接口
public:
	//查询限额
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem){return 0;}
		//最少积分
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//数据事件
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//积分事件
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//查询是否扣服务费
	virtual bool QueryBuckleServiceCharge(WORD wChairID){return true;}

	//比赛接口
public:
	//设置基数
	virtual void SetGameBaseScore(SCORE lBaseScore){};

	//信息接口
public:
	//开始模式
	virtual BYTE GetGameStartMode();

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


	//用户事件
public:
	//用户断线
	virtual bool OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem);
	//用户重入
	virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//用户坐下
	virtual bool OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起立
	virtual bool OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户同意
	virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);

	//游戏事件
protected:
	//游戏开始
	bool OnGameAutoStart();
	//放弃事件
	bool OnUserGiveUp(WORD wChairID,bool bExit=false);
	//看牌事件
	bool OnUserLookCard(WORD wChairID);
	//比牌事件
	bool OnUserCompareCard(WORD wFirstChairID,WORD wNextChairID,bool bNeedScore = true);
	//开牌事件
	bool OnUserOpenCard(WORD wUserID);
	//加注事件
	bool OnUserAddScore(WORD wChairID, SCORE lScore);
	//梭哈
	bool OnUserShowHand(WORD wUserID);
	//功能函数
protected:
	//读取配置
	void ReadConfigInformation();
};

//////////////////////////////////////////////////////////////////////////

#endif
