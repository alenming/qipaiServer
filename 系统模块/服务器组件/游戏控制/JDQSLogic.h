#ifndef JDQS_LOGIC_HEAD_FILE
#define JDQS_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

#define CT_J						0									//J
#define CT_Q						1									//Q
#define CT_K						2									//K
#define CT_A						3									//A
#define CT_GUO						4									//平底锅
#define CT_XIANG					5									//补给箱
#define CT_SCAR						6									//Scar
#define CT_98K						7									//98K
#define CT_JUEDI					8									//绝地求生
#define CT_FREE						10									//免费游戏
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//游戏逻辑类
class CJDQSLogic
{
public:
	int			m_nCount;

	//函数定义
public:
	//构造函数
	CJDQSLogic();
	//析构函数
	virtual ~CJDQSLogic();

	//控制函数
public:
	//混乱扑克
	VOID RandCardList(BYTE cbCardBuffer[][JDQS_X_COUNT]);

	//逻辑函数
public:
	//取得中奖
	DOUBLE GetZhongJiangTime(BYTE cbItemInfo[][JDQS_X_COUNT]);
	//进入小玛丽
	BYTE GetEnterFree(BYTE cbItemInfo[][JDQS_X_COUNT]);


};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

#endif