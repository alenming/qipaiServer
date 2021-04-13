#ifndef BACCARAT_LOGIC_HEAD_FILE
#define BACCARAT_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

#define AREA_ZHUANG					0									//ׯ������
#define AREA_XIAN					1									//�м�����
#define AREA_PING					2									//ƽ������
#define AREA_ZHUANG_DUI				3									//ׯ����
#define AREA_XIAN_DUI				4									//�ж���

#define AREA_MAX					5									//�������

//������multiple
#define MULTIPLE_XIAN				2									//�мұ���
#define MULTIPLE_PING				8									//ƽ�ұ���
#define MULTIPLE_ZHUANG				2									//ׯ�ұ���
#define MULTIPLE_XIAN_PING			11									//�ж��ӱ���
#define MULTIPLE_ZHUANG_PING		11									//ׯ���ӱ���
//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼�
class CBaccaratLogic
{
	//��������
private:
	static const BYTE				m_cbCardListData[52*8];				//�˿˶���

	//��������
public:
	//���캯��
	CBaccaratLogic();
	//��������
	virtual ~CBaccaratLogic();

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

	//�߼�����
public:
	//��ȡ�Ƶ�
	BYTE GetCardPip(BYTE cbCardData);
	//��ȡ�Ƶ�
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);

	//��ȡһ��û���ֹ�����
	BYTE cbGetControlCard(BYTE cbCardPlayer[], BYTE cbCardCountPlayer, BYTE cbCardBanker[], BYTE cbCardCountBanker, BYTE cbSameCard = 0);
};

//////////////////////////////////////////////////////////////////////////

#endif
