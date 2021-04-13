#ifndef SG_LOGIC_HEAD_FILE
#define SG_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��ֵ����
#define LOGIC_MASK_COLOR      0xF0                //��ɫ����
#define LOGIC_MASK_VALUE      0x0F                //��ֵ����

//�˿�����
#define SG_EIGHT                8                 //�˵�
#define SG_NINE                 9                 //�ŵ�
#define SG_THREE_KING			10		          //��������������JQK
#define SG_THREEE_SAME			11			      //������������һ��


//////////////////////////////////////////////////////////////////////////


//�����ṹ
struct tagSGAnalyseResult
{
  BYTE              cbFourCount;				  //������Ŀ
  BYTE              cbThreeCount;				  //������Ŀ
  BYTE              cbDoubleCount;				  //������Ŀ
  BYTE              cbSignedCount;				  //������Ŀ
  BYTE              cbFourLogicVolue[1];          //�����б�
  BYTE              cbThreeLogicVolue[1];		  //�����б�
  BYTE              cbDoubleLogicVolue[2];        //�����б�
  BYTE              cbSignedLogicVolue[5];        //�����б�
  BYTE              cbFourCardData[SG_MAX_CARDCOUNT];    //�����б�
  BYTE              cbThreeCardData[SG_MAX_CARDCOUNT];   //�����б�
  BYTE              cbDoubleCardData[SG_MAX_CARDCOUNT];  //�����б�
  BYTE              cbSignedCardData[SG_MAX_CARDCOUNT];  //������Ŀ
};


//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CSGLogic
{
  //��������
public:
  static BYTE           m_cbCardListData[52];       //�˿˶���

  //��������
public:
  //���캯��
	CSGLogic();
  //��������
	virtual ~CSGLogic();

  //���ͺ���
public:
  //��ȡ����
  BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount);
  //��ȡ��ֵ
  BYTE GetCardValue(BYTE cbCardData)
  {
    return cbCardData & LOGIC_MASK_VALUE;
  }
  //ɾ���˿�
  bool RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount);

  //��ȡ��ɫ
  BYTE GetCardColor(BYTE cbCardData)
  {
    return cbCardData&LOGIC_MASK_COLOR;
  }
  //��ȡ����
  BYTE GetTimes(BYTE cbCardData[], BYTE cbCardCount);

  //��ȡ����
  bool IsIntValue(BYTE cbCardData[], BYTE cbCardCount);

  //���ƺ���
public:
  void GetDebugCards(BYTE cbCardData[], BYTE cbCardCount);
  //�����˿�
  void SortCardList(BYTE cbCardData[], BYTE cbCardCount);
  //�����˿�
  void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
  //��ȡ�˿�
  int GetCards(BYTE outCards[], BYTE outLen, BYTE repoCards[], BYTE repoLen, BYTE cCards[] = NULL, BYTE cLen = 0);

  //���ܺ���
public:
  //�߼���ֵ
  BYTE GetCardLogicValue(BYTE cbCardData, BOOL & isKing);
  //�Ա��˿�
  bool CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount);
  //�������
  VOID SortPlayerValue(BYTE cbPlayerValue[], BYTE cbCount);
  //��ȡ��Ҵ�С����
  VOID GetPlayerOrder(CONST BYTE cbHandCard[], CONST BYTE cbPlayerValue[], BYTE cbPlayerOrder[], BYTE cbCount);

};

//////////////////////////////////////////////////////////////////////////

#endif
