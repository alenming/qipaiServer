#ifndef THIRTEEN_LOGIC_HEAD_FILE
#define THIRTEEN_LOGIC_HEAD_FILE

#pragma once
#include "afxtempl.h"
//////////////////////////////////////////////////////////////////////////

#define HAND_CARD_COUNT					13

//扑克类型
#define TH_INVALID						0								//错误类型
#define TH_SINGLE						1								//乌龙类型
#define TH_ONE_DOUBLE					2								//一对
#define TH_TWO_DOUBLE					3								//两对
#define TH_THREE						4								//三条
#define TH_FIVE_SINGLE_LINE				5								//顺子
#define TH_FIVE_FLUSH					6								//同花
#define TH_FIVE_THREE_DEOUBLE			7								//葫芦
#define TH_FIVE_FOUR_ONE				8								//铁支
#define TH_FIVE_TONGHUASHUN				9								//同花顺

//特殊类型
#define TH_EX_SANTONGHUA				10								//三同花
#define TH_EX_SANSHUNZI					11								//三顺子
#define TH_EX_LIUDUIBAN					12								//六对半
#define TH_EX_WUDUISANTIAO				13								//五对三条
#define TH_EX_SITAOSANTIAO				14								//四套三条
#define TH_EX_COUYISE					15								//凑一色
#define TH_EX_QUANXIAO					16								//全小
#define TH_EX_QUANDA					17								//全大
#define TH_EX_SANFENGTIANXIA			18								//三分天下
#define TH_EX_SANTONGHUASHUN			19								//三同花顺
#define TH_EX_SHIERHUANGZU				20								//十二皇族
#define TH_EX_YITIAOLONG				21								//一条龙
#define TH_EX_ZHIZUNQINGLONG			22								//至尊清龙

//数值掩码
#define	LOGIC_MASK_COLOR				0xF0							//花色掩码
#define	LOGIC_MASK_VALUE				0x0F							//数值掩码

//分析结构
struct tagTHAnalyseData
{
	BYTE							bOneCount;							//单张数目
	BYTE							bTwoCount;							//两张数目
	BYTE							bThreeCount;						//三张数目
	BYTE							bFourCount;							//四张数目
	BYTE							bOneFirst[13];						//单牌位置
	BYTE							bTwoFirst[13];						//对牌位置
	BYTE							bThreeFirst[13];					//三条位置
	BYTE							bFourFirst[13];						//四张位置
	bool							bSameColor;							//是否同花
};

//分析结构
struct tagTHAnalyseResult
{
	BYTE 							cbBlockCount[4];					//同牌数目
	BYTE							cbCardData[4][13];					//扑克列表
};

//分布信息
struct tagTHDistributing
{
	BYTE							cbCardCount;						//扑克数目
	BYTE							cbDistributing[13][5];				//分布信息
};

//搜索结果
struct tagTHSearchCardResult
{
	BYTE							cbSearchCount;						//结果数目
	BYTE							cbCardCount[13];					//扑克数目
	BYTE							cbResultCard[13][13];				//扑克列表
};

//转化数据
struct tagTHTransData
{
	BYTE							bKingCount;							//王牌数目
	BYTE							bHaveKing;							//是否有王	(0 无 1 小王 2 大王 3 大小王)
	CList<BYTE>						transList;							//转化成的数字链表
	tagTHTransData()
	{
		bKingCount = 0;
		bHaveKing = 0;
		transList.RemoveAll();
	}
};

enum COMRESULT
{
	enCRLess = -1,
	enCREqual,
	enCRGreater,
	enCRError
};

//排列类型
enum enSortCardType
{
	enDescend,															//降序类型
	enAscend,															//升序类型
	enColor																//花色类型
};

//控制输赢
enum enControlType
{
	enControlNone,														//不控制
	enControlAndroid,													//机器人输赢
	enControlUser														//控制玩家输赢
};

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////


//游戏逻辑类
class CThirteenLogic
{
	//变量定义
protected:
	static const BYTE				m_bCardListData[52];				//扑克数据

	//函数定义
public:
	//构造函数
	CThirteenLogic();
	//析构函数
	virtual ~CThirteenLogic();

	//类型函数
public:
	//获取类型
	BYTE GetCardType(BYTE bCardData[], BYTE bCardCount);
	//特殊牌型
	BYTE GetSpecialType(BYTE bHandCardData[], BYTE bCardCount);

	BYTE GetSpecialCardData(BYTE bHandCardData[HAND_CARD_COUNT], BYTE bHandCardCount, BYTE bSpecCardData[HAND_CARD_COUNT]);
	//获取数值
	BYTE GetCardValue(BYTE bCardData) { return bCardData&LOGIC_MASK_VALUE; } //十六进制前面四位表示牌的数值
	//获取花色
	BYTE GetCardColor(BYTE bCardData) { return (bCardData&LOGIC_MASK_COLOR)>>4; } //十六进制后面四位表示牌的花色

	//控制函数
public:
	//排列扑克
	void SortCardList(BYTE bCardData[], BYTE bCardCount,enSortCardType SortCardType=enDescend);
	//混乱扑克
	void RandCardList(BYTE bCardBuffer[], BYTE bBufferCount);
	//删除扑克
	bool RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE cbTemp[], BYTE bCardCount);


	//自动摆牌
	void AutoPutCard(BYTE bCardData[], BYTE bPutCardData[]);

	//自动摆6副牌
	int AutoSixPutCard(BYTE bCardData[], BYTE bPutCardData[][13]);

	//获取特殊牌型倍数
	int nGetSpecialTimes(BYTE cbFirstType, BYTE cbNextType);

	//逻辑函数
public:
	//逻辑数值
	BYTE GetCardLogicValue(BYTE bCardData);
	////对比扑克
	//bool CompareCard(BYTE bInFirstList[], BYTE bInNextList[], BYTE bFirstCount, BYTE bNextCount , bool bComperWithOther);
	//对比扑克
	int CompareCard(BYTE bInFirstList[], BYTE bInNextList[], BYTE bFirstCount, BYTE bNextCount , bool bComperWithOther);
	//检查龙牌
	bool IsStraightDragon(const BYTE cbCardData[], BYTE bCardCount);
	//是否顺子
	bool IsLinkCard(const BYTE cbCardData[], BYTE cbCardCount);
	//是否同花
	bool IsSameColorCard(const BYTE cbCardData[], BYTE cbCardCount);
	//获取对数
	BYTE GetDoubleCount(BYTE cbFrontCard[], BYTE cbMidCard[], BYTE cbBackCard[]);
	//设置顺子
	bool SetLinkCard(BYTE cbCardData[], BYTE cbCardCount, BYTE cbLineCardData[3][5]);

	//功能函数
public:
	//分析扑克
	void AnalyseCard(const BYTE bCardDataList[] , const BYTE bCardCount , tagTHAnalyseData& AnalyseData);
	//分析扑克
	void AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagTHAnalyseResult & AnalyseResult);
	//分析分布
	VOID AnalysebDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagTHDistributing & Distributing);
	//王牌转换
	BYTE AnalyseMaxTransform(const BYTE cbCardData[], const BYTE cbCardCount, BYTE bTransCardData[], tagTHTransData & TransData);
	//王牌转换
	void TransformCard(const BYTE cbNkCardData[], BYTE bCardCount, BYTE bKCount, BYTE bCardType, const tagTHAnalyseData & tad, BYTE bTransCardData[], tagTHTransData & TransData);
	//单张比较
	bool CompareOneCardEx(BYTE bFirstCard, BYTE bNextCard, tagTHTransData & ttdFst, tagTHTransData & ttdNxt);
	//抽取大牌
	BYTE GetMaxCardData(const BYTE cbCardData[], const BYTE cbCardCount, BYTE cbMaxCardData[], BYTE & bMaxCardCount, BYTE bNeedCCount = 5);
	//构造扑克
	BYTE MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex);

	//搜索同牌
	BYTE SearchSameCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbSameCardCount, tagTHSearchCardResult *pSearchCardResult);
	//搜索带牌(三带一，四带一等)
	BYTE SearchTakeCardType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbSameCount, BYTE cbTakeCardCount, tagTHSearchCardResult *pSearchCardResult);
	//搜索同花
	BYTE SearchSameColorType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbSameCount, tagTHSearchCardResult *pSearchCardResult);
	//搜索连牌
	BYTE SearchLineCardType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbLineCount, tagTHSearchCardResult *pSearchCardResult);
	//搜索同花顺
	BYTE SearchSameColorLineType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbLineCount, tagTHSearchCardResult *pSearchCardResult);

public:

	/*
	@以下函数为机器人控制做出的更改
	@修改日期2014-3-21
	*/
	//牌类型转换权值
	int CardTypeToWeights(BYTE bType, BYTE cbIndex);
	/*
	@以下函数为配合自动摆牌函数（AutoPutCard ）使用的更改
	@修改日期2014-3-21
	*/
	//优化组合牌型
	void OptimizationCombo(BYTE bInFirstList[], BYTE bInNextList[], BYTE bFirstCount, BYTE bNextCount);
};

//////////////////////////////////////////////////////////////////////////

#endif
