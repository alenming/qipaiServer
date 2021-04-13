#ifndef LAND_LOGIC_HEAD_FILE
#define LAND_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//��������
#define ST_ORDER					1									//��С����
#define ST_COUNT					2									//��Ŀ����
#define ST_CUSTOM					3									//�Զ�����

#define MAX_COUNT					20
#define FULL_COUNT					54

#define	MASK_COLOR					0xF0								//��ɫ����
#define	MASK_VALUE					0x0F								//��ֵ����

#define CT_ERROR					0									//��������
#define CT_SINGLE					1									//��������
#define CT_DOUBLE					2									//��������
#define CT_THREE					3									//��������
#define CT_SINGLE_LINE				4									//��������
#define CT_DOUBLE_LINE				5									//��������
#define CT_THREE_LINE				6									//��������
#define CT_THREE_TAKE_ONE			7									//����һ��
#define CT_THREE_TAKE_TWO			8									//����һ��
#define CT_FOUR_TAKE_ONE			9									//�Ĵ�����
#define CT_FOUR_TAKE_TWO			10									//�Ĵ�����
#define CT_BOMB_CARD				11									//ը������
#define CT_MISSILE_CARD				12									//�������
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//�����ṹ
struct tagAnalyseResult
{
	BYTE 							cbBlockCount[4];					//�˿���Ŀ
	BYTE							cbCardData[4][MAX_COUNT];			//�˿�����
};

//���ƽ��
struct tagOutCardResult
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbResultCard[MAX_COUNT];			//����˿�
};

//�ֲ���Ϣ
struct tagDistributing
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbDistributing[15][6];				//�ֲ���Ϣ
};

//�������
struct tagSearchCardResult
{
	BYTE							cbSearchCount;						//�����Ŀ
	BYTE							cbCardCount[MAX_COUNT];				//�˿���Ŀ
	BYTE							cbResultCard[MAX_COUNT][MAX_COUNT];	//����˿�
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//��Ϸ�߼���
class CLandLogic
{
	//��������
protected:
	static const BYTE				m_cbCardData[FULL_COUNT];			//�˿�����
	static const BYTE				m_cbGoodCardData[34];					//�˿�����

	//��������
public:
	//���캯��
	CLandLogic();
	//��������
	virtual ~CLandLogic();

	//���ͺ���
public:
	//��ȡ����
	BYTE GetCardType(const BYTE cbCardData[], BYTE cbCardCount);
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&MASK_COLOR; }

	//���ƺ���
public:
	//�����˿�
	VOID RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//���Һ���
	VOID RandGoodCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//�����˿�
	VOID SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);
	//�����˿�
	VOID SortOutCardList(BYTE cbCardData[], BYTE cbCardCount);
	//ɾ���˿�
	bool RemoveCardList(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount);
	//ɾ���˿�
	bool RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount);

	//�߼�����
public:
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE cbCardData);
	//�Ա��˿�
	bool CompareCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount);
	//��������
	BYTE SearchOutCard( const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, 
		tagSearchCardResult *pSearchCardResult );
	//ͬ������
	BYTE SearchSameCard( const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbReferCard, BYTE cbSameCardCount,
		tagSearchCardResult *pSearchCardResult );
	//��������
	BYTE SearchLineCardType( const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbReferCard, BYTE cbBlockCount, BYTE cbLineCount,
		tagSearchCardResult *pSearchCardResult );
	//������������(����һ���Ĵ�һ��)
	BYTE SearchTakeCardType( const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbReferCard, BYTE cbSameCount, BYTE cbTakeCardCount, 
		tagSearchCardResult *pSearchCardResult );
	//�����ɻ�
	BYTE SearchThreeTwoLine( const BYTE cbHandCardData[], BYTE cbHandCardCount, tagSearchCardResult *pSearchCardResult );

	//�ڲ�����
public:
	//�����˿�
	BYTE MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex);
	//�����˿�
	VOID AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult);
	//�����ֲ�
	VOID AnalysebDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing);
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

#endif