#ifndef BACCARAT_LOGIC_HEAD_FILE
#define BACCARAT_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

//索引定义
#define INDEX_PLAYER				0									//闲家索引
#define INDEX_BANKER				1									//庄家索引

#define AREA_ZHUANG					0									//庄家索引
#define AREA_XIAN					1									//闲家索引
#define AREA_PING					2									//平家索引
#define AREA_ZHUANG_DUI				3									//庄对子
#define AREA_XIAN_DUI				4									//闲对子

#define AREA_MAX					5									//最大区域

//区域倍数multiple
#define MULTIPLE_XIAN				2									//闲家倍数
#define MULTIPLE_PING				8									//平家倍数
#define MULTIPLE_ZHUANG				2									//庄家倍数
#define MULTIPLE_XIAN_PING			11									//闲对子倍数
#define MULTIPLE_ZHUANG_PING		11									//庄对子倍数
//////////////////////////////////////////////////////////////////////////

//游戏逻辑
class CBaccaratLogic
{
	//变量定义
private:
	static const BYTE				m_cbCardListData[52*8];				//扑克定义

	//函数定义
public:
	//构造函数
	CBaccaratLogic();
	//析构函数
	virtual ~CBaccaratLogic();

	//类型函数
public:
	//获取数值
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//获取花色
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }

	//控制函数
public:
	//混乱扑克
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);

	//逻辑函数
public:
	//获取牌点
	BYTE GetCardPip(BYTE cbCardData);
	//获取牌点
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);

	//获取一张没出现过的牌
	BYTE cbGetControlCard(BYTE cbCardPlayer[], BYTE cbCardCountPlayer, BYTE cbCardBanker[], BYTE cbCardCountBanker, BYTE cbSameCard = 0);
};

//////////////////////////////////////////////////////////////////////////

#endif
