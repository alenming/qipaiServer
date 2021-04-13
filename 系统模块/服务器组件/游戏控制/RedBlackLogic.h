#ifndef REDBLACK_LOGIC_HEAD_FILE
#define REDBLACK_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//宏定义

#define MAX_COUNT					3									//最大数目
#define	DRAW						2									//和局类型

//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

//扑克类型
#define CT_RB_SINGLE					1									//单牌类型
#define CT_RB_DOUBLE					2									//对子类型
#define CT_RB_DOUBLE_SHINE				3									//大对子类型
#define	CT_RB_SHUN_ZI					4									//顺子类型
#define CT_RB_JIN_HUA					5									//金花类型
#define	CT_RB_SHUN_JIN					6									//顺金类型
#define	CT_RB_BAO_ZI					7									//豹子类型
#define CT_RB_SPECIAL					8									//特殊类型

#define RBAREA_XIAN					0									//黑
#define RBAREA_ZHUANG				1									//红
#define RBAREA_PING					2									//幸运一击
#define RBAREA_MAX					3									//最大区域
//////////////////////////////////////////////////////////////////////////

//游戏逻辑类
class CRedBlackLogic
{
	//变量定义
private:
	static BYTE						m_cbCardListData[52];				//扑克定义

	//函数定义
public:
	//构造函数
	CRedBlackLogic();
	//析构函数
	virtual ~CRedBlackLogic();

	//类型函数
public:
	//获取类型
	BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount);
	//获取数值
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//获取花色
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }

	//控制函数
public:
	//排列扑克
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount);
	//混乱扑克
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);

	//功能函数
public:
	//逻辑数值
	BYTE GetCardLogicValue(BYTE cbCardData);
	//对比扑克
	BYTE CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount);
};

//////////////////////////////////////////////////////////////////////////

#endif