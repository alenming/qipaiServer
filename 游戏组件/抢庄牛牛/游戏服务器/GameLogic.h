#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��ֵ����
#define	LOGIC_MASK_COLOR				0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE				0x0F								//��ֵ����

//����ģʽ
#define CT_OX_VALUE0			0									//��ţ
#define CT_OX_VALUE1			1									//ţһ
#define CT_OX_VALUE2			2									//ţ��
#define CT_OX_VALUE3			3									//ţ��
#define CT_OX_VALUE4			4									//ţ��
#define CT_OX_VALUE5			5									//ţ��
#define CT_OX_VALUE6			6									//ţ��
#define CT_OX_VALUE7			7									//ţ��
#define CT_OX_VALUE8			8									//ţ��
#define CT_OX_VALUE9			9									//ţ��
#define CT_OX_VALUENIUNIU		10									//ţţ
#define CT_OX_VALUE_BOMB		11									//ը��
#define CT_OX_VALUE_FIVEFLOWER	12									//�廨ţ
#define CT_OX_VALUE_FIVESNIUNIU	13									//��Сţ

//////////////////////////////////////////////////////////////////////////

//�����ṹ
struct tagAnalyseResult
{
	BYTE 							cbBlockCount[4];					//�˿���Ŀ
	BYTE							cbCardData[4][MAX_CARDCOUNT];			//�˿�����
	BYTE							cbKingCount;
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
	//��������
public:
	static BYTE						m_cbCardListDataNoKing[52];					//�˿˶���
	static BYTE						m_cbCardListDataHaveKing[54];				//�˿˶���
	
	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ͺ���
public:
	//��ȡ����
	BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount);
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }
	//��ȡ����
	BYTE GetTimes(BYTE cbCardData[], BYTE cbCardCount);
	//���ƺ���
public:
	//�����˿�(�¼�����ʹ��)
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount, bool bDescend = true);
	//�����˿�
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//ɾ���˿�
	bool RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount);

	//���ܺ���
public:
	//�߼���ֵ(�¼�����ʹ��)
	BYTE GetCardLogicValue(BYTE cbCardData);
	//�Ա��˿�
	bool CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount);
	//��������
public:
	//�����˿�
	bool AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult);
	int RetType(int itype);
};

//////////////////////////////////////////////////////////////////////////

#endif
