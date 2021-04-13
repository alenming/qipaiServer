#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once
#include "afxtempl.h"
//////////////////////////////////////////////////////////////////////////

//�˿�����
#define CT_INVALID						0								//��������
#define CT_SINGLE						1								//��������
#define CT_ONE_DOUBLE					2								//һ��
#define CT_TWO_DOUBLE					3								//����
#define CT_THREE						4								//����
#define CT_FIVE_SINGLE_LINE				5								//˳��
#define CT_FIVE_FLUSH					6								//ͬ��
#define CT_FIVE_THREE_DEOUBLE			7								//��«
#define CT_FIVE_FOUR_ONE				8								//��֧
#define CT_FIVE_TONGHUASHUN				9								//ͬ��˳

//��������
#define CT_EX_SANTONGHUA				10								//��ͬ��
#define CT_EX_SANSHUNZI					11								//��˳��
#define CT_EX_LIUDUIBAN					12								//���԰�
#define CT_EX_WUDUISANTIAO				13								//�������
#define CT_EX_SITAOSANTIAO				14								//��������
#define CT_EX_COUYISE					15								//��һɫ
#define CT_EX_QUANXIAO					16								//ȫС
#define CT_EX_QUANDA					17								//ȫ��
#define CT_EX_SANFENGTIANXIA			18								//��������
#define CT_EX_SANTONGHUASHUN			19								//��ͬ��˳
#define CT_EX_SHIERHUANGZU				20								//ʮ������
#define CT_EX_YITIAOLONG				21								//һ����
#define CT_EX_ZHIZUNQINGLONG			22								//��������

//��ֵ����
#define	LOGIC_MASK_COLOR				0xF0							//��ɫ����
#define	LOGIC_MASK_VALUE				0x0F							//��ֵ����

//�����ṹ
struct tagAnalyseData
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
struct tagAnalyseResult
{
	BYTE 							cbBlockCount[4];					//ͬ����Ŀ
	BYTE							cbCardData[4][13];					//�˿��б�
};

//�ֲ���Ϣ
struct tagDistributing
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbDistributing[13][5];				//�ֲ���Ϣ
};

//�������
struct tagSearchCardResult
{
	BYTE							cbSearchCount;						//�����Ŀ
	BYTE							cbCardCount[13];					//�˿���Ŀ
	BYTE							cbResultCard[13][13];				//�˿��б�
};

//ת������
struct tagTransData
{
	BYTE							bKingCount;							//������Ŀ
	BYTE							bHaveKing;							//�Ƿ�����	(0 �� 1 С�� 2 ���� 3 ��С��)
	CList<BYTE>						transList;							//ת���ɵ���������
	tagTransData()
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
class CGameLogic
{
	//��������
protected:
	static const BYTE				m_bCardListData[52];				//�˿�����

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

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
	void AnalyseCard(const BYTE bCardDataList[] , const BYTE bCardCount , tagAnalyseData& AnalyseData);
	//�����˿�
	void AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult);
	//�����ֲ�
	VOID AnalysebDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing);
	//����ת��
	BYTE AnalyseMaxTransform(const BYTE cbCardData[], const BYTE cbCardCount, BYTE bTransCardData[], tagTransData & TransData);
	//����ת��
	void TransformCard(const BYTE cbNkCardData[], BYTE bCardCount, BYTE bKCount, BYTE bCardType, const tagAnalyseData & tad, BYTE bTransCardData[], tagTransData & TransData);
	//���űȽ�
	bool CompareOneCardEx(BYTE bFirstCard, BYTE bNextCard, tagTransData & ttdFst, tagTransData & ttdNxt);
	//��ȡ����
	BYTE GetMaxCardData(const BYTE cbCardData[], const BYTE cbCardCount, BYTE cbMaxCardData[], BYTE & bMaxCardCount, BYTE bNeedCCount = 5);
	//�����˿�
	BYTE MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex);

	//����ͬ��
	BYTE SearchSameCard( const BYTE cbHandCardData[], BYTE cbHandCardCount,BYTE cbSameCardCount, tagSearchCardResult *pSearchCardResult );
	//��������(����һ���Ĵ�һ��)
	BYTE SearchTakeCardType( const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbSameCount, BYTE cbTakeCardCount, tagSearchCardResult *pSearchCardResult );
	//����ͬ��
	BYTE SearchSameColorType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbSameCount, tagSearchCardResult *pSearchCardResult);
	//��������
	BYTE SearchLineCardType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbLineCount, tagSearchCardResult *pSearchCardResult);
	//����ͬ��˳
	BYTE SearchSameColorLineType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbLineCount, tagSearchCardResult *pSearchCardResult);

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
