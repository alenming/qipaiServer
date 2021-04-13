#ifndef SG_LOGIC_HEAD_FILE
#define SG_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//宏定义

//数值掩码
#define LOGIC_MASK_COLOR      0xF0                //花色掩码
#define LOGIC_MASK_VALUE      0x0F                //数值掩码

//扑克类型
#define SG_EIGHT                8                 //八点
#define SG_NINE                 9                 //九点
#define SG_THREE_KING			10		          //三公：三个都是JQK
#define SG_THREEE_SAME			11			      //三条：三个牌一样


//////////////////////////////////////////////////////////////////////////


//分析结构
struct tagSGAnalyseResult
{
  BYTE              cbFourCount;				  //四张数目
  BYTE              cbThreeCount;				  //三张数目
  BYTE              cbDoubleCount;				  //两张数目
  BYTE              cbSignedCount;				  //单张数目
  BYTE              cbFourLogicVolue[1];          //四张列表
  BYTE              cbThreeLogicVolue[1];		  //三张列表
  BYTE              cbDoubleLogicVolue[2];        //两张列表
  BYTE              cbSignedLogicVolue[5];        //单张列表
  BYTE              cbFourCardData[SG_MAX_CARDCOUNT];    //四张列表
  BYTE              cbThreeCardData[SG_MAX_CARDCOUNT];   //三张列表
  BYTE              cbDoubleCardData[SG_MAX_CARDCOUNT];  //两张列表
  BYTE              cbSignedCardData[SG_MAX_CARDCOUNT];  //单张数目
};


//////////////////////////////////////////////////////////////////////////

//游戏逻辑类
class CSGLogic
{
  //变量定义
public:
  static BYTE           m_cbCardListData[52];       //扑克定义

  //函数定义
public:
  //构造函数
	CSGLogic();
  //析构函数
	virtual ~CSGLogic();

  //类型函数
public:
  //获取类型
  BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount);
  //获取数值
  BYTE GetCardValue(BYTE cbCardData)
  {
    return cbCardData & LOGIC_MASK_VALUE;
  }
  //删除扑克
  bool RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount);

  //获取花色
  BYTE GetCardColor(BYTE cbCardData)
  {
    return cbCardData&LOGIC_MASK_COLOR;
  }
  //获取倍数
  BYTE GetTimes(BYTE cbCardData[], BYTE cbCardCount);

  //获取整数
  bool IsIntValue(BYTE cbCardData[], BYTE cbCardCount);

  //控制函数
public:
  void GetDebugCards(BYTE cbCardData[], BYTE cbCardCount);
  //排列扑克
  void SortCardList(BYTE cbCardData[], BYTE cbCardCount);
  //混乱扑克
  void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
  //获取扑克
  int GetCards(BYTE outCards[], BYTE outLen, BYTE repoCards[], BYTE repoLen, BYTE cCards[] = NULL, BYTE cLen = 0);

  //功能函数
public:
  //逻辑数值
  BYTE GetCardLogicValue(BYTE cbCardData, BOOL & isKing);
  //对比扑克
  bool CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount);
  //排序点数
  VOID SortPlayerValue(BYTE cbPlayerValue[], BYTE cbCount);
  //获取玩家大小序列
  VOID GetPlayerOrder(CONST BYTE cbHandCard[], CONST BYTE cbPlayerValue[], BYTE cbPlayerOrder[], BYTE cbCount);

};

//////////////////////////////////////////////////////////////////////////

#endif
