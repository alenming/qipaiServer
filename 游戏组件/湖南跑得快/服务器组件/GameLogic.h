#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////

//�˿�����
#define CT_ERROR					0									//��������
#define CT_SINGLE					1									//��������
#define CT_SINGLE_LINE				2									//��������
#define CT_DOUBLE_LINE				3									//��������
#define CT_THREE_LINE				4									//��������
#define CT_THREE_LINE_TAKE_SINGLE	5									//����һ��
#define CT_THREE_LINE_TAKE_DOUBLE	6									//����һ��
#define CT_BOMB						7									//ը������

//�궨��
#define MAX_COUNT					16									//�����Ŀ

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

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

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
	//��������
protected:
	static const BYTE				m_bCardListData[48];				//�˿�����

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ͺ���
public:
	//��ȡ����
	BYTE GetCardType(const BYTE bCardData[], BYTE bCardCount);
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE bCardData) { return bCardData&LOGIC_MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE bCardData) { return bCardData&LOGIC_MASK_COLOR; }

	//���ƺ���
public:
	//�����˿�
	void SortCardList(BYTE bCardData[], BYTE bCardCount);
	//�����˿�
	VOID SortOutCardList(BYTE cbCardData[], BYTE cbCardCount);
	//�����˿�
	void RandCardList(BYTE bCardBuffer[], BYTE bBufferCount);
	//ɾ���˿�
	bool RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount);

	//�߼�����
public:
	//��Ч�ж�
	bool IsValidCard(BYTE cbCardData);
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE bCardData);

	//���ܺ���
public:
	//�Ա��˿�
	bool CompareCard(BYTE bFirstList[], BYTE bNextList[], BYTE bFirstCount, BYTE bNextCount);
	//�����˿�
	void AnalysebCardData(const BYTE bCardData[], BYTE bCardCount, tagAnalyseResult & AnalyseResult);
	//�����ֲ�
	VOID AnalysebDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing);
	//�����ж�
	BYTE SearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount,
		tagSearchCardResult *pSearchCardResult);
	//ͬ������
	BYTE SearchSameCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbReferCard, BYTE cbSameCardCount,
		tagSearchCardResult *pSearchCardResult);
	//������������(����һ���Ĵ�һ��)
	BYTE SearchTakeCardType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbReferCard, BYTE cbSameCount, BYTE cbTakeCardCount,
		tagSearchCardResult *pSearchCardResult);
	//��������
	BYTE SearchLineCardType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbReferCard, BYTE cbBlockCount, BYTE cbLineCount,
		tagSearchCardResult *pSearchCardResult);

	//�����˿�
	BYTE MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex);
};

//////////////////////////////////////////////////////////////////////////

#endif