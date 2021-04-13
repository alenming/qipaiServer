#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"


#define LINE_COUNT						9					//线数量
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//游戏逻辑类
class CGameLogic
{
private:
	BYTE								m_cbLineInfo[LINE_COUNT][ITEM_X_COUNT];		//9条线

public:
	int									m_nIndex;

	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	//控制函数
public:
	//混乱扑克
	VOID RandCardList(BYTE cbCardBuffer[][ITEM_X_COUNT]);

	//逻辑函数
public:
	//取得中奖
	int GetZhongJiangTime(BYTE cbItemInfo[][ITEM_X_COUNT]);
	//进入小玛丽
	BYTE GetEnterThree(BYTE cbItemInfo[][ITEM_X_COUNT]);	
	//获取免费次数
	BYTE GetEnterFree(BYTE cbItemInfo[][ITEM_X_COUNT]);
	//获取JackPot分数
	SCORE GetJackPotScore(BYTE cbItemInfo[][ITEM_X_COUNT],SCORE lJackPotScore);
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

#endif