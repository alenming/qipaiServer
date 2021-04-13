#ifndef OXBATTLE_LOGIC_HEAD_FILE
#define OXBATTLE_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//牌型

enum emCardType
{
	CT_POINT			=		0,								//点数类型
	CT_SPECIAL_NIU1		=		1,								//特殊类型
	CT_SPECIAL_NIU2		=		2,								//特殊类型
	CT_SPECIAL_NIU3		=		3,								//特殊类型
	CT_SPECIAL_NIU4		=		4,								//特殊类型
	CT_SPECIAL_NIU5		=		5,								//特殊类型
	CT_SPECIAL_NIU6		=		6,								//特殊类型
	CT_SPECIAL_NIU7		=		7,								//特殊类型
	CT_SPECIAL_NIU8		=		8,								//特殊类型
	CT_SPECIAL_NIU9		=	    9,								//特殊类型	
	CT_SPECIAL_NIUNIU	=		10,								//牛牛
	CT_SPECIAL_BOMEBOME	=		11,								//炸弹牛
	CT_SPECIAL_NIUWUHUA =		12,								//五花牛
	CT_SPECIAL_XIAO		=		13								//五小牛
};

//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

//排序类型
#define	ST_VALUE					1									//数值排序
#define	ST_NEW					    3									//数值排序
#define	ST_LOGIC					2									//逻辑排序

//扑克数目
#define CARD_COUNT					52									//扑克数目

#define OXBANKER_INDEX				0									//庄家索引
#define OXTIAN_MEN_INDEX			1									//天门索引
#define OXDI_MEN_INDEX				2									//地门索引
#define OXXUAN_MEN_INDEX			3									//玄门索引
#define OXHUANG_MEN_INDEX			4									//黄门索引

#define OXAREA_MAX					4									//区域数目
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//游戏逻辑
class COxBattleLogic
{
	//变量定义
public:
	static const BYTE				m_cbCardListData[CARD_COUNT];		//扑克定义

	//函数定义
public:
	//构造函数
	COxBattleLogic();
	//析构函数
	virtual ~COxBattleLogic();

	//类型函数
public:
	//获取数值
	BYTE GetCardValue(BYTE cbCardData) 
	{ 
		return cbCardData&LOGIC_MASK_VALUE; 
	}
	//获取花色
	BYTE GetCardColor(BYTE cbCardData)
	{
		return (cbCardData&LOGIC_MASK_COLOR)>>4;
	}

	//控制函数
public:
	//混乱扑克
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//混乱扑克
	void RandCardListEx(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//排列扑克
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);
	//删除扑克
	bool RemoveCardList(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount);
	int RetType(int itype);

	//逻辑函数
public:
	//获取牌点
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);
	//获取牌型
	BYTE GetCardType(const BYTE cbCardData[], BYTE cbCardCount,BYTE *bcOutCadData = NULL);
	//大小比较
	int CompareCard(const BYTE cbFirstCardData[], BYTE cbFirstCardCount,const BYTE cbNextCardData[], BYTE cbNextCardCount,int nServerLevel,BYTE &Multiple);
	//逻辑大小
	BYTE GetCardLogicValue(BYTE cbCardData);

	BYTE GetCardNewValue(BYTE cbCardData);
};

//////////////////////////////////////////////////////////////////////////

#endif
