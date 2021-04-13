#ifndef TONGZI_LOGIC_HEAD_FILE
#define TONGZI_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//��������
#define	ST_VALUE					1									//��ֵ����
#define	ST_LOGIC					2									//�߼�����

//�˿���Ŀ
#define TZ_CARD_COUNT					40								//�˿���Ŀ

#define TZ_MAX_COUNT					2								//�����Ŀ
//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼�
void RandCount(BYTE cbCardBuffer[], BYTE cbBufferCount);
class CTongZiLogic
{
	//��������
private:
	static const BYTE				m_cbCardListData[TZ_CARD_COUNT];		//�˿˶���

	//��������
public:
	//���캯��
	CTongZiLogic();
	//��������
	virtual ~CTongZiLogic();

	//���ͺ���
public:
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }

	//���ƺ���
public:
	//�����˿�
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//�����˿�
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);

	//�߼�����
public:
	//��ȡ����
	DWORD GetCardType(const BYTE cbCardData[], BYTE cbCardCount);
	//��С�Ƚ�
	int CompareCard(const BYTE cbFirstCardData[], BYTE cbFirstCardCount,const BYTE cbNextCardData[], BYTE cbNextCardCount);
	//�߼���С
	BYTE GetCardLogicValue(BYTE cbCardData);

	void GetCardPoint(BYTE bcCardData ,CPoint &Point,int CardX,int CardY);
};

//////////////////////////////////////////////////////////////////////////

#endif
