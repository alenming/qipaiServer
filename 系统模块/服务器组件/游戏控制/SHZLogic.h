#ifndef SHZ_LOGIC_HEAD_FILE
#define SHZ_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"


//逻辑类型
#define CT_FUTOU					0									//斧头
#define CT_YINGQIANG				1									//银枪
#define CT_DADAO					2									//大刀
#define CT_LU						3									//鲁
#define CT_LIN						4									//林
#define CT_SONG						5									//宋
#define CT_TITIANXINGDAO			6									//替天行道
#define CT_ZHONGYITANG				7									//忠义堂
#define CT_SHUIHUZHUAN				8									//水浒传
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//游戏逻辑类
class CSHZLogic
{
	//函数定义
public:
	//构造函数
	CSHZLogic();
	//析构函数
	virtual ~CSHZLogic();

	//控制函数
public:
	//混乱扑克
	VOID RandCardList(BYTE cbCardBuffer[][SHZ_X_COUNT]);

	//逻辑函数
public:
	//取得中奖
	int GetZhongJiangTime(BYTE cbItemInfo[][SHZ_X_COUNT]);
	//全盘奖
	int GetQuanPanJiangTime(BYTE cbItemInfo[][SHZ_X_COUNT]);
	//全人物
	int GetQuanRenJiangTime(BYTE cbItemInfo[][SHZ_X_COUNT]);
	//全兵器
	int GetQuanBingQiJiangTime(BYTE cbItemInfo[][SHZ_X_COUNT]);
	//进入小玛丽
	BYTE GetEnterThree(BYTE cbItemInfo[][SHZ_X_COUNT]);
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

#endif