#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//宏定义

enum AndroidBetAreas
{
	ASTRAIGHT_0,
	ASTRAIGHT_1,
	ASTRAIGHT_2,
	ASTRAIGHT_3,
	ASTRAIGHT_4,
	ASTRAIGHT_5,
	ASTRAIGHT_6,
	ASTRAIGHT_7,
	ASTRAIGHT_8,
	ASTRAIGHT_9,
	ASTRAIGHT_10,
	ASTRAIGHT_11,
	ASTRAIGHT_12,
	ASTRAIGHT_13,
	ASTRAIGHT_14,
	ASTRAIGHT_15,
	ASTRAIGHT_16,
	ASTRAIGHT_17,
	ASTRAIGHT_18,
	ASTRAIGHT_19,
	ASTRAIGHT_20,
	ASTRAIGHT_21,
	ASTRAIGHT_22,
	ASTRAIGHT_23,
	ASTRAIGHT_24,
	ASTRAIGHT_25,
	ASTRAIGHT_26,
	ASTRAIGHT_27,
	ASTRAIGHT_28,
	ASTRAIGHT_29,
	ASTRAIGHT_30,
	ASTRAIGHT_31,
	ASTRAIGHT_32,
	ASTRAIGHT_33,
	ASTRAIGHT_34,
	ASTRAIGHT_35,
	ASTRAIGHT_36,
	ASPLIT_1,
	ASPLIT_2,
	ASPLIT_3,
	ASPLIT_4,
	ASPLIT_5,
	ASPLIT_6,
	ASPLIT_7,
	ASPLIT_8,
	ASPLIT_9,
	ASPLIT_10,
	ASPLIT_11,
	ASPLIT_12,
	ASPLIT_13,
	ASPLIT_14,
	ASPLIT_15,
	ASPLIT_16,
	ASPLIT_17,
	ASPLIT_18,
	ASPLIT_19,
	ASPLIT_20,
	ASPLIT_21,
	ASPLIT_22,
	ASPLIT_23,
	ASPLIT_24,
	ASPLIT_25,
	ASPLIT_26,
	ASPLIT_27,
	ASPLIT_28,
	ASPLIT_29,
	ASPLIT_30,
	ASPLIT_31,
	ASPLIT_32,
	ASPLIT_33,
	ASPLIT_34,
	ASPLIT_35,
	ASPLIT_36,
	ASPLIT_37,
	ASPLIT_38,
	ASPLIT_39,
	ASPLIT_40,
	ASPLIT_41,
	ASPLIT_42,
	ASPLIT_43,
	ASPLIT_44,
	ASPLIT_45,
	ASPLIT_46,
	ASPLIT_47,
	ASPLIT_48,
	ASPLIT_49,
	ASPLIT_50,
	ASPLIT_51,
	ASPLIT_52,
	ASPLIT_53,
	ASPLIT_54,
	ASPLIT_55,
	ASPLIT_56,
	ASPLIT_57,
	ASTREET_1,
	ASTREET_2,
	ASTREET_3,
	ASTREET_4,
	ASTREET_5,
	ASTREET_6,
	ASTREET_7,
	ASTREET_8,
	ASTREET_9,
	ASTREET_10,
	ASTREET_11,
	ASTREET_12,
	ATHREE_1,
	ATHREE_2,
	ACORNER_1,
	ACORNER_2,
	ACORNER_3,
	ACORNER_4,
	ACORNER_5,
	ACORNER_6,
	ACORNER_7,
	ACORNER_8,
	ACORNER_9,
	ACORNER_10,
	ACORNER_11,
	ACORNER_12,
	ACORNER_13,
	ACORNER_14,
	ACORNER_15,
	ACORNER_16,
	ACORNER_17,
	ACORNER_18,
	ACORNER_19,
	ACORNER_20,
	ACORNER_21,
	ACORNER_22,
	AFOUR_1,
	ALINE_1,
	ALINE_2,
	ALINE_3,
	ALINE_4,
	ALINE_5,
	ALINE_6,
	ALINE_7,
	ALINE_8,
	ALINE_9,
	ALINE_10,
	ALINE_11,
	ACOLUMN_1,
	ACOLUMN_2,
	ACOLUMN_3,
	ADOZEN_1,
	ADOZEN_2,
	ADOZEN_3,
	ABLACK_1,
	ARED_1,
	AODD_1,
	AEVEN_1,
	ALOW_1,
	AHI_1,
	ABET_AREA_COUNT
};

//最大下注次数
#define MAX_CHIP_TIME								50
//////////////////////////////////////////////////////////////////////////

//机器人类
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	//游戏变量
protected:
	SCORE							m_lMaxChipBanker;					//最大下注 (庄家)
	SCORE							m_lMaxChipUser;						//最大下注 (个人)
	SCORE							m_lAreaChip[AREA_COUNT];			//区域下注 
	WORD							m_wCurrentBanker;					//庄家位置
	BYTE							m_cbTimeLeave;						//剩余时间

	int								m_nChipLimit[2];					//下注范围 (0-AREA_COUNT)
	int								m_nChipTime;						//下注次数 (本局)
	int								m_nChipTimeCount;					//已下次数 (本局)
	
	//上庄变量
protected:
	bool							m_bMeApplyBanker;					//申请标识
	int								m_nBankerCount;						//本机器人的坐庄次数
	int								m_nWaitBanker;						//空几盘
	static int						m_stlApplyBanker;					//申请数

	//配置变量  (全局配置)
protected:
	tagRobotInfo					m_RobotInfo;						//全局配置
	TCHAR							m_szRoomName[32];					//配置房间

	//配置变量	(游戏配置)
protected:
	bool							m_bRefreshCfg;						//每盘刷新
	SCORE							m_lAreaLimitScore;					//区域限制
	SCORE							m_lUserLimitScore;					//下注限制
	SCORE							m_lBankerCondition;					//上庄条件		

	//配置变量  (机器人配置)
protected:
	SCORE							m_lRobotJettonLimit[2];				//筹码限制	
	int								m_nRobotBetTimeLimit[2];			//次数限制	
	bool							m_bRobotBanker;						//是否坐庄
	int								m_nRobotBankerCount;				//坐庄次数
	int								m_nRobotWaitBanker;					//空盘重申
	int								m_nRobotApplyBanker;				//上庄个数
	bool							m_bReduceJettonLimit;				//降低限制

	//机器人存取款
	LONGLONG						m_lRobotScoreRange[2];				//最大范围
	LONGLONG						m_lRobotBankGetScore;				//提款数额
	LONGLONG						m_lRobotBankGetScoreBanker;			//提款数额 (庄家)
	int								m_nRobotBankStorageMul;				//存款倍数

	int								m_nAreaNum[154][18];				//下注数字

	//控件变量
protected:
	CGameLogic						m_GameLogic;						//游戏逻辑
	IAndroidUserItem *				m_pIAndroidUserItem;				//用户接口

	//函数定义
public:
	//构造函数
	CAndroidUserItemSink();
	//析构函数
	virtual ~CAndroidUserItemSink();

	//基础接口
public:
	//释放对象
	virtual VOID  Release() {delete this; }
	
	//接口查询
	virtual VOID *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//控制接口
public:
	//初始接口
	virtual bool  Initialization(IUnknownEx * pIUnknownEx);
	//重置接口
	virtual bool  RepositionSink();

	//游戏事件
public:
	//时间消息
	virtual bool  OnEventTimer(UINT nTimerID);
	//游戏消息
	virtual bool  OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//游戏消息
	virtual bool  OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//场景消息
	virtual bool  OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize);

	//用户事件
public:
	//用户进入
	virtual void  OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户离开
	virtual void  OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户积分
	virtual void  OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户状态
	virtual void  OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	
	//消息处理
public:
	//游戏空闲
	bool OnSubGameFree(const void * pBuffer, WORD wDataSize);
	//游戏开始
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//用户加注
	bool OnSubPlaceJetton(const void * pBuffer, WORD wDataSize);
	//下注失败
	bool OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//申请做庄
	bool OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize);
	//取消做庄
	bool OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize);
	//切换庄家
	bool OnSubChangeBanker(const void * pBuffer, WORD wDataSize);

	//功能函数
public:
  //读取配置
  void ReadConfigInformation(tagCustomAndroid *pCustomAndroid);
  //计算范围
  bool CalcJettonRange(SCORE lMaxScore, SCORE lChipLmt[], int & nChipTime, int lJetLmt[]);
  //读取长整
  LONGLONG GetPrivateProfileInt64(LPCTSTR lpAppName, LPCTSTR lpKeyName, LONGLONG lDefault, LPCTSTR lpFileName);
};

//////////////////////////////////////////////////////////////////////////

#endif
