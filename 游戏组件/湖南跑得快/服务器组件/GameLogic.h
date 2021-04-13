#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////

//扑克类型
#define CT_ERROR					0									//错误类型
#define CT_SINGLE					1									//单牌类型
#define CT_SINGLE_LINE				2									//单连类型
#define CT_DOUBLE_LINE				3									//对连类型
#define CT_THREE_LINE				4									//三连类型
#define CT_THREE_LINE_TAKE_SINGLE	5									//三带一单
#define CT_THREE_LINE_TAKE_DOUBLE	6									//三带一对
#define CT_BOMB						7									//炸弹类型

//宏定义
#define MAX_COUNT					16									//最大数目

//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

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

//////////////////////////////////////////////////////////////////////////

//游戏逻辑类
class CGameLogic
{
	//变量定义
protected:
	static const BYTE				m_bCardListData[48];				//扑克数据

	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	//类型函数
public:
	//获取类型
	BYTE GetCardType(const BYTE bCardData[], BYTE bCardCount);
	//获取数值
	BYTE GetCardValue(BYTE bCardData) { return bCardData&LOGIC_MASK_VALUE; }
	//获取花色
	BYTE GetCardColor(BYTE bCardData) { return bCardData&LOGIC_MASK_COLOR; }

	//控制函数
public:
	//排列扑克
	void SortCardList(BYTE bCardData[], BYTE bCardCount);
	//排列扑克
	VOID SortOutCardList(BYTE cbCardData[], BYTE cbCardCount);
	//混乱扑克
	void RandCardList(BYTE bCardBuffer[], BYTE bBufferCount);
	//删除扑克
	bool RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount);

	//逻辑函数
public:
	//有效判断
	bool IsValidCard(BYTE cbCardData);
	//逻辑数值
	BYTE GetCardLogicValue(BYTE bCardData);

	//功能函数
public:
	//对比扑克
	bool CompareCard(BYTE bFirstList[], BYTE bNextList[], BYTE bFirstCount, BYTE bNextCount);
	//分析扑克
	void AnalysebCardData(const BYTE bCardData[], BYTE bCardCount, tagAnalyseResult & AnalyseResult);
	//分析分布
	VOID AnalysebDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing);
	//出牌判断
	BYTE SearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount,
		tagSearchCardResult *pSearchCardResult);
	//同牌搜索
	BYTE SearchSameCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbReferCard, BYTE cbSameCardCount,
		tagSearchCardResult *pSearchCardResult);
	//带牌类型搜索(三带一，四带一等)
	BYTE SearchTakeCardType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbReferCard, BYTE cbSameCount, BYTE cbTakeCardCount,
		tagSearchCardResult *pSearchCardResult);
	//连牌搜索
	BYTE SearchLineCardType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbReferCard, BYTE cbBlockCount, BYTE cbLineCount,
		tagSearchCardResult *pSearchCardResult);

	//构造扑克
	BYTE MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex);
};

//////////////////////////////////////////////////////////////////////////

#endif