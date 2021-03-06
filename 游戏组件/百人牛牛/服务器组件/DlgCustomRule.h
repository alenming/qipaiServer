#ifndef GAME_DLG_CUSTOM_RULE_HEAD_FILE
#define GAME_DLG_CUSTOM_RULE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "TableFrameSink.h"

//自定义配置
struct tagCustomGeneral
{
	//上庄信息
	SCORE						lApplyBankerCondition;			//申请条件
	LONGLONG						lBankerTime;					//坐庄次数
	LONGLONG						lBankerTimeAdd;					//庄家增加数
	SCORE							lBankerScoreMAX;				//三次坐庄金币数
	LONGLONG						lBankerTimeExtra;				//三次坐庄次数
	BOOL							nEnableSysBanker;				//系统做庄
	
	//时间
	LONGLONG						lFreeTime;						//空闲时间
	LONGLONG						lBetTime;						//下注时间
	LONGLONG						lEndTime;						//结束时间

	//下注限制
	SCORE							lAreaLimitScore;				//区域限制
	SCORE							lUserLimitScore;				//区域限制
	LONGLONG						lEndGameMul;					//提前开牌百分比
	
	//消息
	TCHAR							szMessageItem1[64];				//消息1
	TCHAR							szMessageItem2[64];				//消息2
	TCHAR							szMessageItem3[64];				//消息3

	//库存
	SCORE							StorageStart;					//起始库存
	LONGLONG						StorageDeduct;					//库存衰减
	SCORE							StorageMax1;					//库存封顶1
	LONGLONG						StorageMul1;					//库存封顶后玩家赢分概率
	SCORE							StorageMax2;					//库存封顶2
	LONGLONG						StorageMul2;					//库存封顶后玩家赢分概率

	//构造函数
	tagCustomGeneral()
	{
		DefaultCustomRule();
	}

	void DefaultCustomRule()
	{
		lApplyBankerCondition = 10000000;
		lBankerTime = 10;
		lBankerTimeAdd = 10;
		lBankerScoreMAX = 100000000;
		lBankerTimeExtra = 10;
		nEnableSysBanker = TRUE;

		lFreeTime = 5;
		lBetTime = 10;
		lEndTime = 20;

		lAreaLimitScore = 100000000;
		lUserLimitScore = 10000000;
		lEndGameMul = 80;

		CopyMemory(szMessageItem1,TEXT("让我们给他点颜色看看"),sizeof(TEXT("让我们给他点颜色看看")));
		CopyMemory(szMessageItem2,TEXT("掌声欢迎"),sizeof(TEXT("掌声欢迎")));
		CopyMemory(szMessageItem3,TEXT("大家尽情游戏吧"),sizeof(TEXT("大家尽情游戏吧")));

		StorageStart = 10000000;
		StorageDeduct = 0;
		StorageMax1 = 100000000;
		StorageMul1 = 50;
		StorageMax2 = 1000000000;
		StorageMul2 = 80;
	}
};


struct tagCustomConfig
{
	tagCustomGeneral				CustomGeneral;					//通用功能
	tagCustomAndroid				CustomAndroid;					//机器人

	//构造函数
	tagCustomConfig()
	{
		DefaultCustomRule();
	}

	void DefaultCustomRule()
	{
		CustomGeneral.DefaultCustomRule();
		CustomAndroid.DefaultCustomRule();
	}
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661//////////
class CDlgCustomGeneral : public CDialog
{
	//配置结构
public:
	tagCustomGeneral					m_CustomGeneral;						//自定配置
	int									m_iyoldpos;								//滚动条位置

	//函数定义
public:
	//构造函数
	CDlgCustomGeneral();
	//析构函数
	virtual ~CDlgCustomGeneral();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
	
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnClickSuperBankerVIPConfig();
	afx_msg void OnClickSuperBankerConsumeConfig();

	afx_msg void OnClickOccupySeatVIPConfig();
	afx_msg void OnClickOccupySeatConsumeConfig();
	afx_msg void OnClickOccupySeatFreeConfig();
};
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661//////////
class CDlgCustomAndroid : public CDialog
{
	//配置结构
public:
	tagCustomAndroid					m_CustomAndroid;						//自定配置

	//函数定义
public:
	//构造函数
	CDlgCustomAndroid();
	//析构函数
	virtual ~CDlgCustomAndroid();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
	afx_msg void OnBNClickEnableRobotBanker();
	
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661//////////
class CDlgCustomRule : public CDialog
{
	//控件变量
protected:
	CDlgCustomGeneral				m_DlgCustomGeneral;					//通用配置
	CDlgCustomAndroid				m_DlgCustomAndroid;					//机器配置

	//配置变量
protected:
	WORD							m_wCustomSize;						//配置大小
	LPBYTE							m_pcbCustomRule;					//配置缓冲

	//配置结构
protected:
	tagCustomConfig					m_CustomConfig;						//自定配置

	//函数定义
public:
	//构造函数
	CDlgCustomRule();
	//析构函数
	virtual ~CDlgCustomRule();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();
	//更新数据
	BOOL UpdateConfigData(BOOL bSaveAndValidate);

	//功能函数
public:
	//设置配置
	bool SetCustomRule(LPBYTE pcbCustomRule, WORD wCustomSize);
	//保存数据
	bool SaveCustomRule(LPBYTE pcbCustomRule, WORD wCustomSize);
	//默认数据
	bool DefaultCustomRule(LPBYTE pcbCustomRule, WORD wCustomSize);
	//隐藏机器人配置
	bool HideAndroidModule(bool bHide);

	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR,LRESULT *pResult);
	bool CheckDataMinMax(LONGLONG valueMax);

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661//////////
#endif