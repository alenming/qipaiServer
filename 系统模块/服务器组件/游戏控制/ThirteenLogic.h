#ifndef THIRTEEN_LOGIC_HEAD_FILE
#define THIRTEEN_LOGIC_HEAD_FILE

#pragma once
#include "afxtempl.h"
//////////////////////////////////////////////////////////////////////////

#define HAND_CARD_COUNT					13

//�˿�����
#define TH_INVALID						0								//��������
#define TH_SINGLE						1								//��������
#define TH_ONE_DOUBLE					2								//һ��
#define TH_TWO_DOUBLE					3								//����
#define TH_THREE						4								//����
#define TH_FIVE_SINGLE_LINE				5								//˳��
#define TH_FIVE_FLUSH					6								//ͬ��
#define TH_FIVE_THREE_DEOUBLE			7								//��«
#define TH_FIVE_FOUR_ONE				8								//��֧
#define TH_FIVE_TONGHUASHUN				9								//ͬ��˳

//��������
#define TH_EX_SANTONGHUA				10								//��ͬ��
#define TH_EX_SANSHUNZI					11								//��˳��
#define TH_EX_LIUDUIBAN					12								//���԰�
#define TH_EX_WUDUISANTIAO				13								//�������
#define TH_EX_SITAOSANTIAO				14								//��������
#define TH_EX_COUYISE					15								//��һɫ
#define TH_EX_QUANXIAO					16								//ȫС
#define TH_EX_QUANDA					17								//ȫ��
#define TH_EX_SANFENGTIANXIA			18								//��������
#define TH_EX_SANTONGHUASHUN			19								//��ͬ��˳
#define TH_EX_SHIERHUANGZU				20								//ʮ������
#define TH_EX_YITIAOLONG				21								//һ����
#define TH_EX_ZHIZUNQINGLONG			22								//��������

//��ֵ����
#define	LOGIC_MASK_COLOR				0xF0							//��ɫ����
#define	LOGIC_MASK_VALUE				0x0F							//��ֵ����

//�����ṹ
struct tagTHAnalyseData
{
	BYTE							bOneCount;							//������Ŀ
	BYTE							bTwoCount;							//������Ŀ
	BYTE							bThreeCount;						//������Ŀ
	BYTE							bFourCount;							//������Ŀ
	BYTE							bOneFirst[13];						//����λ��
	BYTE							bTwoFirst[13];						//����λ��
	BYTE							bThreeFirst[13];					//����λ��
	BYTE							bFourFirst[13];						//����λ��
	bool							bSameColor;							//�Ƿ�ͬ��
};

//�����ṹ
struct tagTHAnalyseResult
{
	BYTE 							cbBlockCount[4];					//ͬ����Ŀ
	BYTE							cbCardData[4][13];					//�˿��б�
};

//�ֲ���Ϣ
struct tagTHDistributing
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbDistributing[13][5];				//�ֲ���Ϣ
};

//�������
struct tagTHSearchCardResult
{
	BYTE							cbSearchCount;						//�����Ŀ
	BYTE							cbCardCount[13];					//�˿���Ŀ
	BYTE							cbResultCard[13][13];				//�˿��б�
};

//ת������
struct tagTHTransData
{
	BYTE							bKingCount;							//������Ŀ
	BYTE							bHaveKing;							//�Ƿ�����	(0 �� 1 С�� 2 ���� 3 ��С��)
	CList<BYTE>						transList;							//ת���ɵ���������
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

//��������
enum enSortCardType
{
	enDescend,															//��������
	enAscend,															//��������
	enColor																//��ɫ����
};

//������Ӯ
enum enControlType
{
	enControlNone,														//������
	enControlAndroid,													//��������Ӯ
	enControlUser														//���������Ӯ
};

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////


//��Ϸ�߼���
class CThirteenLogic
{
	//��������
protected:
	static const BYTE				m_bCardListData[52];				//�˿�����

	//��������
public:
	//���캯��
	CThirteenLogic();
	//��������
	virtual ~CThirteenLogic();

	//���ͺ���
public:
	//��ȡ����
	BYTE GetCardType(BYTE bCardData[], BYTE bCardCount);
	//��������
	BYTE GetSpecialType(BYTE bHandCardData[], BYTE bCardCount);

	BYTE GetSpecialCardData(BYTE bHandCardData[HAND_CARD_COUNT], BYTE bHandCardCount, BYTE bSpecCardData[HAND_CARD_COUNT]);
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE bCardData) { return bCardData&LOGIC_MASK_VALUE; } //ʮ������ǰ����λ��ʾ�Ƶ���ֵ
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE bCardData) { return (bCardData&LOGIC_MASK_COLOR)>>4; } //ʮ�����ƺ�����λ��ʾ�ƵĻ�ɫ

	//���ƺ���
public:
	//�����˿�
	void SortCardList(BYTE bCardData[], BYTE bCardCount,enSortCardType SortCardType=enDescend);
	//�����˿�
	void RandCardList(BYTE bCardBuffer[], BYTE bBufferCount);
	//ɾ���˿�
	bool RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE cbTemp[], BYTE bCardCount);


	//�Զ�����
	void AutoPutCard(BYTE bCardData[], BYTE bPutCardData[]);

	//�Զ���6����
	int AutoSixPutCard(BYTE bCardData[], BYTE bPutCardData[][13]);

	//��ȡ�������ͱ���
	int nGetSpecialTimes(BYTE cbFirstType, BYTE cbNextType);

	//�߼�����
public:
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE bCardData);
	////�Ա��˿�
	//bool CompareCard(BYTE bInFirstList[], BYTE bInNextList[], BYTE bFirstCount, BYTE bNextCount , bool bComperWithOther);
	//�Ա��˿�
	int CompareCard(BYTE bInFirstList[], BYTE bInNextList[], BYTE bFirstCount, BYTE bNextCount , bool bComperWithOther);
	//�������
	bool IsStraightDragon(const BYTE cbCardData[], BYTE bCardCount);
	//�Ƿ�˳��
	bool IsLinkCard(const BYTE cbCardData[], BYTE cbCardCount);
	//�Ƿ�ͬ��
	bool IsSameColorCard(const BYTE cbCardData[], BYTE cbCardCount);
	//��ȡ����
	BYTE GetDoubleCount(BYTE cbFrontCard[], BYTE cbMidCard[], BYTE cbBackCard[]);
	//����˳��
	bool SetLinkCard(BYTE cbCardData[], BYTE cbCardCount, BYTE cbLineCardData[3][5]);

	//���ܺ���
public:
	//�����˿�
	void AnalyseCard(const BYTE bCardDataList[] , const BYTE bCardCount , tagTHAnalyseData& AnalyseData);
	//�����˿�
	void AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagTHAnalyseResult & AnalyseResult);
	//�����ֲ�
	VOID AnalysebDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagTHDistributing & Distributing);
	//����ת��
	BYTE AnalyseMaxTransform(const BYTE cbCardData[], const BYTE cbCardCount, BYTE bTransCardData[], tagTHTransData & TransData);
	//����ת��
	void TransformCard(const BYTE cbNkCardData[], BYTE bCardCount, BYTE bKCount, BYTE bCardType, const tagTHAnalyseData & tad, BYTE bTransCardData[], tagTHTransData & TransData);
	//���űȽ�
	bool CompareOneCardEx(BYTE bFirstCard, BYTE bNextCard, tagTHTransData & ttdFst, tagTHTransData & ttdNxt);
	//��ȡ����
	BYTE GetMaxCardData(const BYTE cbCardData[], const BYTE cbCardCount, BYTE cbMaxCardData[], BYTE & bMaxCardCount, BYTE bNeedCCount = 5);
	//�����˿�
	BYTE MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex);

	//����ͬ��
	BYTE SearchSameCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbSameCardCount, tagTHSearchCardResult *pSearchCardResult);
	//��������(����һ���Ĵ�һ��)
	BYTE SearchTakeCardType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbSameCount, BYTE cbTakeCardCount, tagTHSearchCardResult *pSearchCardResult);
	//����ͬ��
	BYTE SearchSameColorType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbSameCount, tagTHSearchCardResult *pSearchCardResult);
	//��������
	BYTE SearchLineCardType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbLineCount, tagTHSearchCardResult *pSearchCardResult);
	//����ͬ��˳
	BYTE SearchSameColorLineType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbLineCount, tagTHSearchCardResult *pSearchCardResult);

public:

	/*
	@���º���Ϊ�����˿��������ĸ���
	@�޸�����2014-3-21
	*/
	//������ת��Ȩֵ
	int CardTypeToWeights(BYTE bType, BYTE cbIndex);
	/*
	@���º���Ϊ����Զ����ƺ�����AutoPutCard ��ʹ�õĸ���
	@�޸�����2014-3-21
	*/
	//�Ż��������
	void OptimizationCombo(BYTE bInFirstList[], BYTE bInNextList[], BYTE bFirstCount, BYTE bNextCount);
};

//////////////////////////////////////////////////////////////////////////

#endif
