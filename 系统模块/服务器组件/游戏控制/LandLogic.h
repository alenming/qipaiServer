#ifndef LAND_LOGIC_HEAD_FILE
#define LAND_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//排序类型
#define ST_ORDER					1									//大小排序
#define ST_COUNT					2									//数目排序
#define ST_CUSTOM					3									//自定排序

#define MAX_COUNT					20
#define FULL_COUNT					54

#define	MASK_COLOR					0xF0								//花色掩码
#define	MASK_VALUE					0x0F								//数值掩码

#define CT_ERROR					0									//错误类型
#define CT_SINGLE					1									//单牌类型
#define CT_DOUBLE					2									//对牌类型
#define CT_THREE					3									//三条类型
#define CT_SINGLE_LINE				4									//单连类型
#define CT_DOUBLE_LINE				5									//对连类型
#define CT_THREE_LINE				6									//三连类型
#define CT_THREE_TAKE_ONE			7									//三带一单
#define CT_THREE_TAKE_TWO			8									//三带一对
#define CT_FOUR_TAKE_ONE			9									//四带两单
#define CT_FOUR_TAKE_TWO			10									//四带两对
#define CT_BOMB_CARD				11									//炸弹类型
#define CT_MISSILE_CARD				12									//火箭类型
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//分析结构
struct tagAnalyseResult
{
	BYTE 							cbBlockCount[4];					//扑克数目
	BYTE							cbCardData[4][MAX_COUNT];			//扑克数据
};

//出牌结果
struct tagOutCardResult
{
	BYTE							cbCardCount;						//扑克数目
	BYTE							cbResultCard[MAX_COUNT];			//结果扑克
};

//分布信息
struct tagDistributing
{
	BYTE							cbCardCount;						//扑克数目
	BYTE							cbDistributing[15][6];				//分布信息
};

//搜索结果
struct tagSearchCardResult
{
	BYTE							cbSearchCount;						//结果数目
	BYTE							cbCardCount[MAX_COUNT];				//扑克数目
	BYTE							cbResultCard[MAX_COUNT][MAX_COUNT];	//结果扑克
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//游戏逻辑类
class CLandLogic
{
	//变量定义
protected:
	static const BYTE				m_cbCardData[FULL_COUNT];			//扑克数据
	static const BYTE				m_cbGoodCardData[34];					//扑克数据

	//函数定义
public:
	//构造函数
	CLandLogic();
	//析构函数
	virtual ~CLandLogic();

	//类型函数
public:
	//获取类型
	BYTE GetCardType(const BYTE cbCardData[], BYTE cbCardCount);
	//获取数值
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&MASK_VALUE; }
	//获取花色
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&MASK_COLOR; }

	//控制函数
public:
	//混乱扑克
	VOID RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//混乱好牌
	VOID RandGoodCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//排列扑克
	VOID SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);
	//排列扑克
	VOID SortOutCardList(BYTE cbCardData[], BYTE cbCardCount);
	//删除扑克
	bool RemoveCardList(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount);
	//删除扑克
	bool RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount);

	//逻辑函数
public:
	//逻辑数值
	BYTE GetCardLogicValue(BYTE cbCardData);
	//对比扑克
	bool CompareCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount);
	//出牌搜索
	BYTE SearchOutCard( const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, 
		tagSearchCardResult *pSearchCardResult );
	//同牌搜索
	BYTE SearchSameCard( const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbReferCard, BYTE cbSameCardCount,
		tagSearchCardResult *pSearchCardResult );
	//连牌搜索
	BYTE SearchLineCardType( const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbReferCard, BYTE cbBlockCount, BYTE cbLineCount,
		tagSearchCardResult *pSearchCardResult );
	//带牌类型搜索(三带一，四带一等)
	BYTE SearchTakeCardType( const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbReferCard, BYTE cbSameCount, BYTE cbTakeCardCount, 
		tagSearchCardResult *pSearchCardResult );
	//搜索飞机
	BYTE SearchThreeTwoLine( const BYTE cbHandCardData[], BYTE cbHandCardCount, tagSearchCardResult *pSearchCardResult );

	//内部函数
public:
	//构造扑克
	BYTE MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex);
	//分析扑克
	VOID AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult);
	//分析分布
	VOID AnalysebDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing);
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

#endif