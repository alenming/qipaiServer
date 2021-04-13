#ifndef CAISHEN_LOGIC_HEAD_FILE
#define CAISHEN_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"


//逻辑类型
#define CT_HULU						0									//葫芦
#define CT_BAOZI					1									//包子
#define CT_TAOZI					2									//桃子
#define CT_CUIYU					3									//翠玉
#define CT_FUDAI					4									//福袋
#define CT_JINYU					5									//金鱼
#define CT_LAOTOU					6									//老头
#define CT_HESHANG					7									//和尚
#define CT_CAISHENDAO				8									//财神
#define CT_GUANYUAN					9									//官员
#define CT_JINLUO					10									//金锣
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//游戏逻辑类
class CCaiShenLogic
{

	//函数定义
public:
	//构造函数
	CCaiShenLogic();
	//析构函数
	virtual ~CCaiShenLogic();

	//控制函数
public:
	//混乱扑克
	VOID RandCardList(BYTE cbCardBuffer[][CAISHEN_X_COUNT]);

	//逻辑函数
public:
	//取得中奖
	DOUBLE GetZhongJiangTime(BYTE cbItemInfo[][CAISHEN_X_COUNT]);
	//进入小玛丽
	BYTE GetEnterFree(BYTE cbItemInfo[][CAISHEN_X_COUNT]);
	
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

#endif