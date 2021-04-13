#ifndef REDBLACK_LOGIC_HEAD_FILE
#define REDBLACK_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

#define MAX_COUNT					3									//�����Ŀ
#define	DRAW						2									//�;�����

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//�˿�����
#define CT_RB_SINGLE					1									//��������
#define CT_RB_DOUBLE					2									//��������
#define CT_RB_DOUBLE_SHINE				3									//���������
#define	CT_RB_SHUN_ZI					4									//˳������
#define CT_RB_JIN_HUA					5									//������
#define	CT_RB_SHUN_JIN					6									//˳������
#define	CT_RB_BAO_ZI					7									//��������
#define CT_RB_SPECIAL					8									//��������

#define RBAREA_XIAN					0									//��
#define RBAREA_ZHUANG				1									//��
#define RBAREA_PING					2									//����һ��
#define RBAREA_MAX					3									//�������
//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CRedBlackLogic
{
	//��������
private:
	static BYTE						m_cbCardListData[52];				//�˿˶���

	//��������
public:
	//���캯��
	CRedBlackLogic();
	//��������
	virtual ~CRedBlackLogic();

	//���ͺ���
public:
	//��ȡ����
	BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount);
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }

	//���ƺ���
public:
	//�����˿�
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount);
	//�����˿�
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);

	//���ܺ���
public:
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE cbCardData);
	//�Ա��˿�
	BYTE CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount);
};

//////////////////////////////////////////////////////////////////////////

#endif