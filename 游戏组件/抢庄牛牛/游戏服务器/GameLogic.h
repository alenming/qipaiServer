#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//宏定义

//数值掩码
#define	LOGIC_MASK_COLOR				0xF0								//花色掩码
#define	LOGIC_MASK_VALUE				0x0F								//数值掩码

//经典模式
#define CT_OX_VALUE0			0									//无牛
#define CT_OX_VALUE1			1									//牛一
#define CT_OX_VALUE2			2									//牛二
#define CT_OX_VALUE3			3									//牛三
#define CT_OX_VALUE4			4									//牛四
#define CT_OX_VALUE5			5									//牛五
#define CT_OX_VALUE6			6									//牛六
#define CT_OX_VALUE7			7									//牛七
#define CT_OX_VALUE8			8									//牛八
#define CT_OX_VALUE9			9									//牛九
#define CT_OX_VALUENIUNIU		10									//牛牛
#define CT_OX_VALUE_BOMB		11									//炸弹
#define CT_OX_VALUE_FIVEFLOWER	12									//五花牛
#define CT_OX_VALUE_FIVESNIUNIU	13									//五小牛

//////////////////////////////////////////////////////////////////////////

//分析结构
struct tagAnalyseResult
{
	BYTE 							cbBlockCount[4];					//扑克数目
	BYTE							cbCardData[4][MAX_CARDCOUNT];			//扑克数据
	BYTE							cbKingCount;
};

//////////////////////////////////////////////////////////////////////////

//游戏逻辑类
class CGameLogic
{
	//变量定义
public:
	static BYTE						m_cbCardListDataNoKing[52];					//扑克定义
	static BYTE						m_cbCardListDataHaveKing[54];				//扑克定义
	
	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	//类型函数
public:
	//获取类型
	BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount);
	//获取数值
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//获取花色
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }
	//获取倍数
	BYTE GetTimes(BYTE cbCardData[], BYTE cbCardCount);
	//控制函数
public:
	//排列扑克(新加牌型使用)
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount, bool bDescend = true);
	//混乱扑克
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//删除扑克
	bool RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount);

	//功能函数
public:
	//逻辑数值(新加牌型使用)
	BYTE GetCardLogicValue(BYTE cbCardData);
	//对比扑克
	bool CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount);
	//辅助函数
public:
	//分析扑克
	bool AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult);
	int RetType(int itype);
};

//////////////////////////////////////////////////////////////////////////

#endif
