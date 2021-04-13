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
	SCORE	            m_lAllJettonScore[AREA_COUNT];    //全体总注

  //个人下注
protected:
	SCORE	            m_lUserJettonScore[GAME_PLAYER][AREA_COUNT];  //个人总注

  //上局信息
public:
	SCORE	            m_lLastAllJettonPlace[GAME_PLAYER][AREA_COUNT]; //上局总下注
	SCORE            m_lLastAllJettonArea[GAME_PLAYER][AREA_COUNT];  //本局最后一次操作

  bool				m_bUserListWin[GAME_PLAYER][USER_LIST_COUNT];		//列表玩家获胜次数
  SCORE				m_lUserListScore[GAME_PLAYER][USER_LIST_COUNT];		//列表玩家下注数目
  BYTE				m_cbUserPlayCount[GAME_PLAYER];						//列表玩家游戏局数


  //控制变量
protected:
	SCORE            m_lAreaLimitScore;            //区域限制
	SCORE            m_lUserLimitScore;            //区域限制
	SCORE            m_lApplyBankerCondition;        //申请条件
  //玩家成绩
protected:
	SCORE            m_lUserWinScore[GAME_PLAYER];     //玩家成绩
	SCORE            m_lUserRevenue[GAME_PLAYER];      //玩家税收

	//控制操作
protected:
	BYTE			m_cbJuControlTimes;						//局控次数
	BYTE			m_cbJuControl;							//是否进行局控
	BYTE			m_cbJuControlArea;						//本局控制哪门胜利 0不控制，1控制赢，2控制输

  //扑克信息
protected:

  BYTE              m_cbTableCardArray[2];          //桌面扑克
  BYTE              m_cbShaYuAddMulti;                      //附加赔率


  INT               m_nMultiple[AREA_COUNT];          //区域倍数

  SCORE				m_nChips[5];						//筹码


  //状态变量
protected:
  DWORD             m_dwJettonTime;             //开始时间
  bool              m_bJettonState;             //下注状态

  //庄家信息
protected:
  CWHArray<WORD>    m_ApplyUserArray;           //申请玩家
  WORD              m_wCurrentBanker;           //当前庄家
  WORD              m_wBankerTime;              //做庄次数
  SCORE				m_lBankerWinScore;            //累计成绩
  SCORE				m_lBankerCurGameScore;          //当前成绩
  bool              m_bEnableSysBanker;           //系统做庄

  //记录变量
protected:
  tagServerGameRecord       m_GameRecordArrary[MAX_SCORE_HISTORY];  //游戏记录
  int               m_nRecordFirst;             //开始记录
  int               m_nRecordLast;              //最后记录


  //控制变量
protected:
  LONGLONG            m_StorageControl;           //库存控制
  LONGLONG            m_lStorageCurrent;            //库存数值
  LONGLONG            m_lStorageStart;              //启动库存
  LONGLONG            m_lBonus;               //启动彩金池
  LONGLONG            m_StorageDeduct;            //扣除比例
  LONGLONG            m_lCompensateRadio;           //最大库存赔付比率
  TCHAR             m_szConfigFileName[MAX_PATH];     //配置文件
  TCHAR             m_szGameRoomName[SERVER_LEN];     //房间名称

  LONGLONG            m_lStorageMax1;             //库存封顶1
  LONGLONG            m_lStorageMul1;             //系统输钱概率
  LONGLONG            m_lStorageMax2;             //库存封顶2
  LONGLONG            m_lStorageMul2;             //系统输钱概率
  //机器人控制
protected:
  int               m_nMaxChipRobot;            //最大数目
  int               m_nChipRobotCount;            //人数统计
  SCORE	            m_lRobotAreaLimit;            //区域限制
  LONGLONG            m_lRobotBetCount;           //下注个数
  SCORE	            m_lRobotAreaScore[AREA_COUNT];    //区域下注
  int               m_nRobotListMaxCount;         //最多人数
  //庄家设置
protected:

  LONGLONG            m_lBankerMAX;             //最大庄数
  LONGLONG            m_lBankerAdd;             //庄家增数

  SCORE	            m_lBankerScoreMAX;            //庄家钱
  LONGLONG            m_lBankerScoreAdd;            //坐庄增数

  //最大庄家数
  LONGLONG            m_lPlayerBankerMAX;           //玩家庄数
  int               m_nBankerTimeLimit;           //次数限制
  int               m_nBankerTimeAdd;           //增加次数 (金币大于其他玩家时)
  SCORE	            m_lExtraBankerScore;          //额外条件 (大于此值时可以无视条件额外坐庄)
  int               m_nExtraBankerTime;           //额外次数
  //换庄
  bool              m_bExchangeBanker;            //交换庄家

  //清楚
  bool              m_bIsCleanCurJetton;

  //时间设置
protected:
  BYTE              m_cbFreeTime;             //空闲时间
  BYTE              m_cbBetTime;              //下注时间
  BYTE              m_cbEndTime;              //结束时间
  BYTE                            m_cbAddTime;              //附加时间
  SCORE	            m_lUserStartScore[GAME_PLAYER];   //起始分数
  //组件变量
protected:

  CGameLogic            m_GameLogic;              //游戏逻辑

  //组件接口
protected:
  ITableFrame *         m_pITableFrame;             //框架接口
  tagGameServiceOption *      m_pGameServiceOption;         //服务配置
  tagGameServiceAttrib *      m_pGameServiceAttrib;         //游戏属性

  //属性变量
protected:
  static const WORD       m_wPlayerCount;             //游戏人数

  //函数定义
public:
  //构造函数
  CTableFrameSink();
  //析构函数
  virtual ~CTableFrameSink();

  //基础接口
public:
  //释放对象
  virtual VOID  Release()
  {
    delete this;
  }

  //接口查询
  virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

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
  virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem)
  {
    return 0;
  }
  //数据事件
  virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
  {
    return false;
  }
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
  //扣除服务费
  virtual bool QueryBuckleServiceCharge(WORD wChairID);
  //比赛接口
public:
  //设置基数
	virtual void SetGameBaseScore(SCORE lBaseScore) {};

  //动作事件
public:
  //用户断线
  virtual bool  OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) ;

  //用户重入
  virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem)
  {
    return true;
  }

  //用户坐下
  virtual bool  OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
  //用户起来
  virtual bool  OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
  //用户同意
  virtual bool  OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
  {
    return true;
  }

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

  //辅助函数
private:
  bool FindSuitBanker();
  //发送扑克
  void DispatchTableCard();
  //发送庄家
  void SendApplyUser(IServerUserItem *pServerUserItem);
  //更换庄家
  bool ChangeBanker(bool bCancelCurrentBanker);
  //发送记录
  void SendGameRecord(IServerUserItem *pIServerUserItem);
  //发送消息
  void SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg);
  //下注计算
private:
  //最大下注
	SCORE GetUserMaxJetton(WORD wChairID, BYTE Area);


  //游戏统计
private:
  //计算得分
	SCORE CalculateScore();

  //读取配置
  void ReadConfigInformation();
  //获取区域动物
  BYTE cbGetAreaAnimal(BYTE cbCardData);
  //给客户端发送结束显示动物
  BYTE cbGetTargetAnimal(BYTE cbCardData);
};

//////////////////////////////////////////////////////////////////////////

#endif
