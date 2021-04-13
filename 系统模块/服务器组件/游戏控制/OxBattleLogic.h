#ifndef OXBATTLE_LOGIC_HEAD_FILE
#define OXBATTLE_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//����

enum emCardType
{
	CT_POINT			=		0,								//��������
	CT_SPECIAL_NIU1		=		1,								//��������
	CT_SPECIAL_NIU2		=		2,								//��������
	CT_SPECIAL_NIU3		=		3,								//��������
	CT_SPECIAL_NIU4		=		4,								//��������
	CT_SPECIAL_NIU5		=		5,								//��������
	CT_SPECIAL_NIU6		=		6,								//��������
	CT_SPECIAL_NIU7		=		7,								//��������
	CT_SPECIAL_NIU8		=		8,								//��������
	CT_SPECIAL_NIU9		=	    9,								//��������	
	CT_SPECIAL_NIUNIU	=		10,								//ţţ
	CT_SPECIAL_BOMEBOME	=		11,								//ը��ţ
	CT_SPECIAL_NIUWUHUA =		12,								//�廨ţ
	CT_SPECIAL_XIAO		=		13								//��Сţ
};

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//��������
#define	ST_VALUE					1									//��ֵ����
#define	ST_NEW					    3									//��ֵ����
#define	ST_LOGIC					2									//�߼�����

//�˿���Ŀ
#define CARD_COUNT					52									//�˿���Ŀ

#define OXBANKER_INDEX				0									//ׯ������
#define OXTIAN_MEN_INDEX			1									//��������
#define OXDI_MEN_INDEX				2									//��������
#define OXXUAN_MEN_INDEX			3									//��������
#define OXHUANG_MEN_INDEX			4									//��������

#define OXAREA_MAX					4									//������Ŀ
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼�
class COxBattleLogic
{
	//��������
public:
	static const BYTE				m_cbCardListData[CARD_COUNT];		//�˿˶���

	//��������
public:
	//���캯��
	COxBattleLogic();
	//��������
	virtual ~COxBattleLogic();

	//���ͺ���
public:
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) 
	{ 
		return cbCardData&LOGIC_MASK_VALUE; 
	}
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData)
	{
		return (cbCardData&LOGIC_MASK_COLOR)>>4;
	}

	//���ƺ���
public:
	//�����˿�
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//�����˿�
	void RandCardListEx(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//�����˿�
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);
	//ɾ���˿�
	bool RemoveCardList(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount);
	int RetType(int itype);

	//�߼�����
public:
	//��ȡ�Ƶ�
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);
	//��ȡ����
	BYTE GetCardType(const BYTE cbCardData[], BYTE cbCardCount,BYTE *bcOutCadData = NULL);
	//��С�Ƚ�
	int CompareCard(const BYTE cbFirstCardData[], BYTE cbFirstCardCount,const BYTE cbNextCardData[], BYTE cbNextCardCount,int nServerLevel,BYTE &Multiple);
	//�߼���С
	BYTE GetCardLogicValue(BYTE cbCardData);

	BYTE GetCardNewValue(BYTE cbCardData);
};

//////////////////////////////////////////////////////////////////////////

#endif
