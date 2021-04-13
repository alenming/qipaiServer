#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"
#include "GameLogic.h"

#define MAX_CARD					5

//////////////////////////////////////////////////////////////////////////
//历史记录
#define MAX_SCORE_HISTORY			40									//历史个数
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//游戏桌子
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//用户信息
protected:
	SCORE							m_lUserStartScore[GAME_PLAYER];		//起始分数

	bool							m_bUserListWin[GAME_PLAYER][USER_LIST_COUNT];		//列表玩家获胜次数
	SCORE							m_lUserListScore[GAME_PLAYER][USER_LIST_COUNT];		//列表玩家下注数目
	BYTE							m_cbUserPlayCount[GAME_PLAYER];						//列表玩家游戏局数

	//总下注数
protected:
	SCORE							m_lAllJettonScore[AREA_MAX + 1];		//全体总注
	
	//个人下注
protected:
	SCORE							m_lUserJettonScore[GAME_PLAYER][AREA_MAX + 1];	//个人总注

	int								m_nChip[MAX_CHIP_COUNT];				//筹码配置
	int								m_nMaxTimes;							//最大倍数

	//控制变量
protected:
	bool							m_bRefreshCfg;							//每盘刷新
	TCHAR							m_szRoomName[32];						//配置房间
	TCHAR							m_szConfigFileName[MAX_PATH];			//配置文件
	SCORE							m_lAreaLimitScore;						//区域限制
	SCORE							m_lUserLimitScore;						//区域限制
	INT								m_nEndGameMul;							//提前开牌百分比

	//坐庄控制
	SCORE							m_lApplyBankerCondition;				//申请条件
	int								m_nBankerTimeLimit;						//次数限制
	LONGLONG						m_lPlayerBankerMAX;						//玩家最大庄家数
	int								m_nBankerTimeAdd;						//增加次数 (金币大于其他玩家时)
	SCORE							m_lExtraBankerScore;					//额外条件 (大于此值时可以无视条件额外坐庄)
	int								m_nExtraBankerTime;						//额外次数

	//时间控制
	int								m_nFreeTime;							//空闲时间
	int								m_nPlaceJettonTime;						//下注时间
	int								m_nGameEndTime;							//结束时间

	//机器人控制
	int								m_nMaxChipRobot;						//最大数目 (下注机器人)
	int								m_nChipRobotCount;						//人数统计 (下注机器人)
	SCORE							m_lRobotAreaLimit;						//区域统计 (机器人)
	SCORE							m_lRobotAreaScore[AREA_MAX + 1];		//区域统计 (机器人)
	int								m_nRobotListMaxCount;					//最多人数

	//玩家成绩
protected:
	SCORE							m_lUserWinScore[GAME_PLAYER];			//玩家成绩
	SCORE							m_lUserReturnScore[GAME_PLAYER];		//返回下注
	SCORE							m_lUserRevenue[GAME_PLAYER];			//玩家税收
	BYTE							m_cbLeftCardCount;						//扑克数目
	bool							m_bContiueCard;							//继续发牌
	BYTE							m_bcFirstPostCard;						//最新發牌點數

	//扑克信息
protected:
	BYTE							m_cbCardCount[4];						//扑克数目
    BYTE							m_cbTableCardArray[5][5];				//桌面扑克
	BYTE							m_cbTableCard[CARD_COUNT];				//桌面扑克

	BYTE							m_cbOpenResult[AREA_MAX];				//开奖结果

	BYTE							m_cbEarlyOpen;							//提前开奖标识

	//状态变量
protected:
	DWORD							m_dwJettonTime;							//开始时间
	bool							m_bExchangeBanker;						//变换庄家
	WORD							m_wAddTime;								//额外坐庄 

	//庄家信息
protected:
	CWHArray<WORD>					m_ApplyUserArray;						//申请玩家
	WORD							m_wCurrentBanker;						//当前庄家
	WORD							m_wOfflineBanker;						//离线庄家
	WORD							m_wBankerTime;							//做庄次数
	SCORE							m_lBankerScore;							//
	SCORE							m_lBankerWinScore;						//累计成绩
	SCORE							m_lBankerCurGameScore;					//当前成绩
	bool							m_bEnableSysBanker;						//系统做庄



	//控制变量
protected:
	BYTE							m_cbWinSideControl;						//控制输赢
	int								m_nSendCardCount;						//发送次数

	//局控
	BYTE							m_cbJuControlTimes;						//局控次数
	BYTE							m_cbJuControl;							//是否进行局控
	BYTE							m_cbJuControlArea[AREA_MAX + 1];		//本局控制区域胜利 0不控制，1控制赢，2控制输


	//记录变量
protected:
	tagServerGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];	//游戏记录
	int								m_nRecordFirst;							//开始记录
	int								m_nRecordLast;							//最后记录
	CFile							m_FileRecord;							//记录结果
	DWORD							m_dwRecordCount;						//记录数目

	//组件变量
protected:
	CGameLogic						m_GameLogic;							//游戏逻辑
	ITableFrame						* m_pITableFrame;						//框架接口
	tagGameServiceOption			* m_pGameServiceOption;					//配置参数
	tagGameServiceAttrib *			m_pGameServiceAttrib;					//游戏属性

	//属性变量
protected:
	static const WORD				m_wPlayerCount;							//游戏人数

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
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//管理接口
public:
	//复位接口
	virtual VOID RepositionSink();
	//配置接口
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//比赛接口
public:
	//设置基数
	virtual void SetGameBaseScore(SCORE lBaseScore){};
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
	//时间事件
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//游戏消息
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息
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
	virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }
	//查询接口
public:
	//查询限额
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);
	//最少积分
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//数据事件
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//积分事件
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//查询是否扣服务费
	virtual bool QueryBuckleServiceCharge(WORD wChairID);	

	//游戏事件
protected:
	//加注事件
	bool OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, SCORE lJettonScore);
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

	//输赢处理
protected:
	//计算系统及机器人坐庄时真人玩家得分
	SCORE CalSysOrAndroidBankerScore();
	//计算真人玩家坐庄时真人的得分
	SCORE CalRealPlayerScore();

	//下注计算
private:
	//最大下注
	SCORE GetUserMaxJetton(WORD wChairID, BYTE cbJettonArea);

	//游戏统计
private:
	//计算得分
	SCORE CalculateScore();
	//推断赢家
	void DeduceWinner(bool &bWinTian, bool &bWinDi, bool &bWinXuan,bool &bWinHuan,BYTE &TianMultiple,BYTE &diMultiple,BYTE &TianXuanltiple,BYTE &HuangMultiple );

};


/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

#endif