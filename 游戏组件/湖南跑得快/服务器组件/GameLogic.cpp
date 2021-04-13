#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//��̬����
const BYTE cbIndexCount = 5;

//�˿�����
const BYTE	CGameLogic::m_bCardListData[48]=
{
	0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
		0x11,     0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
		0x21,     0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
		0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
};

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
}

//��������
CGameLogic::~CGameLogic()
{
}

//��ȡ����
BYTE CGameLogic::GetCardType(const BYTE bCardData[], BYTE bCardCount)
{
	//������
	switch (bCardCount)
	{
	case 0:	//����
	{
		return CT_ERROR;
	}
	case 1: //����
	{
		return CT_SINGLE;
	}
	case 2:	//���ƻ��
	{
		//�����ж�
		if (GetCardLogicValue(bCardData[0]) == GetCardLogicValue(bCardData[1])) return CT_DOUBLE_LINE;

		return CT_ERROR;
	}
	}

	BYTE cbTempCard[MAX_COUNT] = {0};
	CopyMemory(cbTempCard, bCardData, sizeof(BYTE) * bCardCount);

	SortCardList(cbTempCard, bCardCount);

	//�����˿�
	tagAnalyseResult AnalyseResult;
	AnalysebCardData(cbTempCard, bCardCount, AnalyseResult);

	//�����ж�
	if (AnalyseResult.cbBlockCount[3]>0)
	{
		//�����ж�
		if ((AnalyseResult.cbBlockCount[3] == 1) && (bCardCount == 4)) return CT_BOMB;

		return CT_ERROR;
	}

	//�����ж�
	if (AnalyseResult.cbBlockCount[2]>0)
	{
		//�����ж�
		if (AnalyseResult.cbBlockCount[2]>1)
		{
			//��������
			BYTE cbCardData = AnalyseResult.cbCardData[2][0];
			BYTE cbFirstLogicValue = GetCardLogicValue(cbCardData);

			//�������
			if (cbFirstLogicValue >= 15) return CT_ERROR;

			//�����ж�
			for (BYTE i = 1; i<AnalyseResult.cbBlockCount[2]; i++)
			{
				BYTE cbCardData = AnalyseResult.cbCardData[2][i * 3];
				if (cbFirstLogicValue != (GetCardLogicValue(cbCardData) + i)) return CT_ERROR;
			}
		}
		else if (bCardCount == 3) return CT_THREE_LINE;

		//�����ж�
		if (AnalyseResult.cbBlockCount[2] * 3 == bCardCount) return CT_THREE_LINE;
		if (AnalyseResult.cbBlockCount[2] * 4 == bCardCount) return CT_THREE_LINE_TAKE_SINGLE;
		if (AnalyseResult.cbBlockCount[2] * 5 == bCardCount) return CT_THREE_LINE_TAKE_DOUBLE;

		return CT_ERROR;
	}

	//��������
	if (AnalyseResult.cbBlockCount[1] >= 1)
	{
		//��������
		BYTE cbCardData = AnalyseResult.cbCardData[1][0];
		BYTE cbFirstLogicValue = GetCardLogicValue(cbCardData);

		//�������
		if (cbFirstLogicValue >= 15) return CT_ERROR;

		//�����ж�
		for (BYTE i = 1; i<AnalyseResult.cbBlockCount[1]; i++)
		{
			BYTE cbCardData = AnalyseResult.cbCardData[1][i * 2];
			if (cbFirstLogicValue != (GetCardLogicValue(cbCardData) + i)) return CT_ERROR;
		}

		//�����ж�
		if ((AnalyseResult.cbBlockCount[1] * 2) == bCardCount) return CT_DOUBLE_LINE;

		return CT_ERROR;
	}

	//�����ж�
	if ((AnalyseResult.cbBlockCount[0] >= 5) && (AnalyseResult.cbBlockCount[0] == bCardCount))
	{
		//��������
		BYTE cbCardData = AnalyseResult.cbCardData[0][0];
		BYTE cbFirstLogicValue = GetCardLogicValue(cbCardData);

		//�������
		if (cbFirstLogicValue >= 15) return CT_ERROR;

		//�����ж�
		for (BYTE i = 1; i<AnalyseResult.cbBlockCount[0]; i++)
		{
			BYTE cbCardData = AnalyseResult.cbCardData[0][i];
			if (cbFirstLogicValue != (GetCardLogicValue(cbCardData) + i)) return CT_ERROR;
		}

		return CT_SINGLE_LINE;
	}

	return CT_ERROR;
}

//�����˿�
void CGameLogic::SortCardList(BYTE bCardData[], BYTE bCardCount)
{
	//ת����ֵ
	BYTE bLogicVolue[MAX_COUNT];
	for (BYTE i=0;i<bCardCount;i++)	bLogicVolue[i]=GetCardLogicValue(bCardData[i]);	

	//�������
	bool bSorted=true;
	BYTE bTempData,bLast=bCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<bLast;i++)
		{
			if ((bLogicVolue[i]<bLogicVolue[i+1])||
				((bLogicVolue[i]==bLogicVolue[i+1])&&(bCardData[i]<bCardData[i+1])))
			{
				//����λ��
				bTempData=bCardData[i];
				bCardData[i]=bCardData[i+1];
				bCardData[i+1]=bTempData;
				bTempData=bLogicVolue[i];
				bLogicVolue[i]=bLogicVolue[i+1];
				bLogicVolue[i+1]=bTempData;
				bSorted=false;
			}	
		}
		bLast--;
	} while(bSorted==false);

	return;
}

//�����˿�
VOID CGameLogic::SortOutCardList(BYTE cbCardData[], BYTE cbCardCount)
{
	//��ȡ����
	BYTE cbCardType = GetCardType(cbCardData, cbCardCount);

	if (cbCardType == CT_THREE_LINE_TAKE_SINGLE || cbCardType == CT_THREE_LINE_TAKE_DOUBLE)
	{
		//������
		tagAnalyseResult AnalyseResult = {};
		AnalysebCardData(cbCardData, cbCardCount, AnalyseResult);

		cbCardCount = AnalyseResult.cbBlockCount[2] * 3;
		CopyMemory(cbCardData, AnalyseResult.cbCardData[2], sizeof(BYTE)*cbCardCount);
		for (int i = CountArray(AnalyseResult.cbBlockCount) - 1; i >= 0; i--)
		{
			if (i == 2) continue;

			if (AnalyseResult.cbBlockCount[i] > 0)
			{
				CopyMemory(&cbCardData[cbCardCount], AnalyseResult.cbCardData[i],
					sizeof(BYTE)*(i + 1)*AnalyseResult.cbBlockCount[i]);
				cbCardCount += (i + 1)*AnalyseResult.cbBlockCount[i];
			}
		}
	}

	return;
}

//�����˿�
void CGameLogic::RandCardList(BYTE bCardBuffer[], BYTE bBufferCount)
{
	//����׼��
	BYTE bCardData[sizeof(m_bCardListData)];
	CopyMemory(bCardData,m_bCardListData,sizeof(m_bCardListData));

	//�����˿�
	BYTE bRandCount=0,bPosition=0;
	do
	{
		bPosition=rand()%(bBufferCount-bRandCount);
		bCardBuffer[bRandCount++]=bCardData[bPosition];
		bCardData[bPosition]=bCardData[bBufferCount-bRandCount];
	} while (bRandCount<bBufferCount);

	return;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount)
{
	//��������
	ASSERT(bRemoveCount<=bCardCount);

	//�������
	BYTE bDeleteCount=0,bTempCardData[20];
	if (bCardCount>CountArray(bTempCardData)) return false;
	CopyMemory(bTempCardData,bCardData,bCardCount*sizeof(bCardData[0]));

	//�����˿�
	for (BYTE i=0;i<bRemoveCount;i++)
	{
		for (BYTE j=0;j<bCardCount;j++)
		{
			if (bRemoveCard[i]==bTempCardData[j])
			{
				bDeleteCount++;
				bTempCardData[j]=0;
				break;
			}
		}
	}
	if (bDeleteCount!=bRemoveCount) return false;

	//�����˿�
	BYTE bCardPos=0;
	for (BYTE i=0;i<bCardCount;i++)
	{
		if (bTempCardData[i]!=0) bCardData[bCardPos++]=bTempCardData[i];
	}

	return true;
}

//��Ч�ж�
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	//��������
	BYTE cbColor=GetCardColor(cbCardData);
	BYTE cbValue=GetCardValue(cbCardData);

	//��Ч�ж�
	switch (cbColor)
	{
	case 0x00:
		{
			return ((cbValue>=0x03)&&(cbValue<=0x0D));
		}
	case 0x10:
		{
			return ((cbValue==0x01)||((cbValue>=0x03)&&(cbValue<=0x0D)));
		}
	case 0x20:
		{
			return ((cbValue==0x01)||((cbValue>=0x03)&&(cbValue<=0x0D)));
		}
	case 0x30:
		{
			return ((cbValue>=0x01)&&(cbValue<=0x0D));
		}
	}

	return false;
}

//�߼���ֵ
BYTE CGameLogic::GetCardLogicValue(BYTE bCardData)
{
	//�˿�����
	BYTE bCardColor=GetCardColor(bCardData);
	BYTE bCardValue=GetCardValue(bCardData);

	//ת����ֵ
	return (bCardValue<=2)?(bCardValue+13):bCardValue;
}

//�Ա��˿�
bool CGameLogic::CompareCard(BYTE bFirstList[], BYTE bNextList[], BYTE bFirstCount, BYTE bNextCount)
{
	//��ȡ����
	BYTE cbNextType = GetCardType(bNextList, bNextCount);
	BYTE cbFirstType = GetCardType(bFirstList, bFirstCount);

	//�����ж�
	if (cbNextType == CT_ERROR) return false;

	//ը���ж�
	if ((cbFirstType != CT_BOMB) && (cbNextType == CT_BOMB)) return true;
	if ((cbFirstType == CT_BOMB) && (cbNextType != CT_BOMB)) return false;

	//�����ж�
	if ((cbFirstType != cbNextType) || (bFirstCount != bNextCount)) return false;

	//��ʼ�Ա�
	switch (cbNextType)
	{
	case CT_SINGLE:
	case CT_SINGLE_LINE:
	case CT_DOUBLE_LINE:
	case CT_THREE_LINE:
	case CT_BOMB:
	{
		//��ȡ��ֵ
		BYTE cbNextLogicValue = GetCardLogicValue(bNextList[0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(bFirstList[0]);

		//�Ա��˿�
		return cbNextLogicValue>cbFirstLogicValue;
	}
	case CT_THREE_LINE_TAKE_SINGLE:
	case CT_THREE_LINE_TAKE_DOUBLE:
	{
		//�����˿�
		tagAnalyseResult NextResult;
		tagAnalyseResult FirstResult;
		AnalysebCardData(bNextList, bNextCount, NextResult);
		AnalysebCardData(bFirstList, bFirstCount, FirstResult);

		//��ȡ��ֵ
		BYTE cbNextLogicValue = GetCardLogicValue(NextResult.cbCardData[2][0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(FirstResult.cbCardData[2][0]);

		//�Ա��˿�
		return cbNextLogicValue>cbFirstLogicValue;
	}
	}

	return false;
}

//�����˿�
void CGameLogic::AnalysebCardData(const BYTE bCardData[], BYTE bCardCount, tagAnalyseResult & AnalyseResult)
{
	//���ý��
	ZeroMemory(&AnalyseResult, sizeof(AnalyseResult));

	//�˿˷���
	for (BYTE i = 0; i<bCardCount; i++)
	{
		//��������
		BYTE cbSameCount = 1, cbCardValueTemp = 0;
		BYTE cbLogicValue = GetCardLogicValue(bCardData[i]);

		//����ͬ��
		for (BYTE j = i + 1; j<bCardCount; j++)
		{
			//��ȡ�˿�
			if (GetCardLogicValue(bCardData[j]) != cbLogicValue) break;

			//���ñ���
			cbSameCount++;
		}

		if (cbSameCount > 4)
		{
			ASSERT(FALSE);
			//���ý��
			ZeroMemory(&AnalyseResult, sizeof(AnalyseResult));
			return;
		}

		//���ý��
		BYTE cbIndex = AnalyseResult.cbBlockCount[cbSameCount - 1]++;
		for (BYTE j = 0; j<cbSameCount; j++) AnalyseResult.cbCardData[cbSameCount - 1][cbIndex*cbSameCount + j] = bCardData[i + j];

		//��������
		i += cbSameCount - 1;
	}

	return;
}

//�����ֲ�
VOID CGameLogic::AnalysebDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing)
{
	//���ñ���
	ZeroMemory(&Distributing, sizeof(Distributing));

	//���ñ���
	for (BYTE i = 0; i<cbCardCount; i++)
	{
		if (cbCardData[i] == 0) continue;

		//��ȡ����
		BYTE cbCardColor = GetCardColor(cbCardData[i]);
		BYTE cbCardValue = GetCardValue(cbCardData[i]);

		//�ֲ���Ϣ
		Distributing.cbCardCount++;
		Distributing.cbDistributing[cbCardValue - 1][cbIndexCount]++;
		Distributing.cbDistributing[cbCardValue - 1][cbCardColor >> 4]++;
	}

	return;
}


//��������
BYTE CGameLogic::SearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount,
	tagSearchCardResult *pSearchCardResult)
{
	//���ý��
	ASSERT(pSearchCardResult != NULL);
	if (pSearchCardResult == NULL) return 0;

	ZeroMemory(pSearchCardResult, sizeof(tagSearchCardResult));

	//��������
	BYTE cbResultCount = 0;
	tagSearchCardResult tmpSearchCardResult = {};

	//�����˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount = cbHandCardCount;
	CopyMemory(cbCardData, cbHandCardData, sizeof(BYTE)*cbHandCardCount);

	BYTE cbTempTurnCardData[MAX_COUNT];
	CopyMemory(cbTempTurnCardData, cbTurnCardData, sizeof(BYTE)*cbTurnCardCount);

	//�����˿�
	SortCardList(cbCardData, cbCardCount);
	SortCardList(cbTempTurnCardData, cbTurnCardCount);

	//��ȡ����
	BYTE cbTurnOutType = GetCardType(cbTempTurnCardData, cbTurnCardCount);

	//���Ʒ���
	switch (cbTurnOutType)
	{
	case CT_ERROR:					//��������
	{
		pSearchCardResult->cbCardCount[cbResultCount] = 1;
		pSearchCardResult->cbResultCard[cbResultCount][0] = cbCardData[0];
		cbResultCount++;

		return cbResultCount;
	}
	case CT_SINGLE:					//��������
	{
		//��������
		BYTE cbReferCard = cbTempTurnCardData[0];
		BYTE cbSameCount = 1;

		//������ͬ��
		cbResultCount = SearchSameCard(cbCardData, cbCardCount, cbReferCard, cbSameCount, pSearchCardResult);

		break;
	}
	case CT_SINGLE_LINE:		//��������
	case CT_DOUBLE_LINE:		//��������
	case CT_THREE_LINE:				//��������
	{
		//��������
		BYTE cbBlockCount = 1;
		if (cbTurnOutType == CT_DOUBLE_LINE) cbBlockCount = 2;
		else if (cbTurnOutType == CT_THREE_LINE) cbBlockCount = 3;

		BYTE cbLineCount = cbTurnCardCount / cbBlockCount;

		//��������
		cbResultCount = SearchLineCardType(cbCardData, cbCardCount, cbTempTurnCardData[0], cbBlockCount, cbLineCount, pSearchCardResult);

		break;
	}
	case CT_THREE_LINE_TAKE_SINGLE:	//����һ��
	case CT_THREE_LINE_TAKE_DOUBLE:	//����һ��
	{
		//Ч������
		if (cbCardCount < cbTurnCardCount) break;

		//���������һ��������
		if (cbTurnCardCount == 4 || cbTurnCardCount == 5)
		{
			BYTE cbTakeCardCount = cbTurnOutType == CT_THREE_LINE_TAKE_SINGLE ? 1 : 2;

			//������������
			cbResultCount = SearchTakeCardType(cbCardData, cbCardCount, cbTempTurnCardData[2], 3, cbTakeCardCount, pSearchCardResult);
		}
		else
		{
			//��������
			BYTE cbBlockCount = 3;
			BYTE cbLineCount = cbTurnCardCount / (cbTurnOutType == CT_THREE_LINE_TAKE_SINGLE ? 4 : 5);
			BYTE cbTakeCardCount = cbTurnOutType == CT_THREE_LINE_TAKE_SINGLE ? 1 : 2;

			//��������
			BYTE cbTmpTurnCard[MAX_COUNT] = {};
			CopyMemory(cbTmpTurnCard, cbTempTurnCardData, sizeof(BYTE)*cbTurnCardCount);
			SortOutCardList(cbTmpTurnCard, cbTurnCardCount);
			cbResultCount = SearchLineCardType(cbCardData, cbCardCount, cbTmpTurnCard[0], cbBlockCount, cbLineCount, pSearchCardResult);

			//��ȡ����
			bool bAllDistill = true;
			for (BYTE i = 0; i < cbResultCount; i++)
			{
				BYTE cbResultIndex = cbResultCount - i - 1;

				//��������
				BYTE cbTmpCardData[MAX_COUNT];
				BYTE cbTmpCardCount = cbCardCount;

				//ɾ������
				CopyMemory(cbTmpCardData, cbCardData, sizeof(BYTE)*cbCardCount);
				VERIFY(RemoveCard(pSearchCardResult->cbResultCard[cbResultIndex], pSearchCardResult->cbCardCount[cbResultIndex],
					cbTmpCardData, cbTmpCardCount));
				cbTmpCardCount -= pSearchCardResult->cbCardCount[cbResultIndex];

				//������
				tagAnalyseResult  TmpResult = {};
				AnalysebCardData(cbTmpCardData, cbTmpCardCount, TmpResult);

				//��ȡ��
				BYTE cbDistillCard[MAX_COUNT] = {};
				BYTE cbDistillCount = 0;
				for (BYTE j = 0; j < CountArray(TmpResult.cbBlockCount); j++)
				{
					if (TmpResult.cbBlockCount[j] > 0)
					{
						if (j + 1 == cbTakeCardCount && TmpResult.cbBlockCount[j] >= cbLineCount)
						{
							BYTE cbTmpBlockCount = TmpResult.cbBlockCount[j];
							CopyMemory(cbDistillCard, &TmpResult.cbCardData[j][(cbTmpBlockCount - cbLineCount)*(j + 1)],
								sizeof(BYTE)*(j + 1)*cbLineCount);
							cbDistillCount = (j + 1)*cbLineCount;
							break;
						}
						else
						{
							for (BYTE k = 0; k < TmpResult.cbBlockCount[j]; k++)
							{
								BYTE cbSubCount = cbTakeCardCount - cbDistillCount;

								if (cbSubCount > j + 1)
									cbSubCount = j + 1;

								BYTE cbTmpBlockCount = TmpResult.cbBlockCount[j];
								CopyMemory(&cbDistillCard[cbDistillCount], &TmpResult.cbCardData[j][(cbTmpBlockCount - k - 1)*(j + 1)],
									sizeof(BYTE)*cbSubCount);
								cbDistillCount += cbSubCount;

								//��ȡ���
								if (cbDistillCount == cbTakeCardCount*cbLineCount) break;
							}
						}
					}

					//��ȡ���
					if (cbDistillCount == cbTakeCardCount*cbLineCount) break;
				}

				//��ȡ���
				if (cbDistillCount == cbTakeCardCount*cbLineCount)
				{
					//���ƴ���
					BYTE cbCount = pSearchCardResult->cbCardCount[cbResultIndex];
					CopyMemory(&pSearchCardResult->cbResultCard[cbResultIndex][cbCount], cbDistillCard,
						sizeof(BYTE)*cbDistillCount);
					pSearchCardResult->cbCardCount[cbResultIndex] += cbDistillCount;
				}
				//����ɾ������
				else
				{
					bAllDistill = false;
					pSearchCardResult->cbCardCount[cbResultIndex] = 0;
				}
			}

			//�������
			if (!bAllDistill)
			{
				pSearchCardResult->cbSearchCount = cbResultCount;
				cbResultCount = 0;
				for (BYTE i = 0; i < pSearchCardResult->cbSearchCount; i++)
				{
					if (pSearchCardResult->cbCardCount[i] != 0)
					{
						tmpSearchCardResult.cbCardCount[cbResultCount] = pSearchCardResult->cbCardCount[i];
						CopyMemory(tmpSearchCardResult.cbResultCard[cbResultCount], pSearchCardResult->cbResultCard[i],
							sizeof(BYTE)*pSearchCardResult->cbCardCount[i]);
						cbResultCount++;
					}
				}
				tmpSearchCardResult.cbSearchCount = cbResultCount;
				CopyMemory(pSearchCardResult, &tmpSearchCardResult, sizeof(tagSearchCardResult));
			}
		}

		break;
	}
	}

	//����ը��
	if (cbCardCount >= 4)
	{
		//��������
		BYTE cbReferCard = 0;
		if (cbTurnOutType == CT_BOMB) cbReferCard = cbTempTurnCardData[0];

		//����ը��
		BYTE cbTmpResultCount = SearchSameCard(cbCardData, cbCardCount, cbReferCard, 4, &tmpSearchCardResult);
		for (BYTE i = 0; i < cbTmpResultCount; i++)
		{
			pSearchCardResult->cbCardCount[cbResultCount] = tmpSearchCardResult.cbCardCount[i];
			CopyMemory(pSearchCardResult->cbResultCard[cbResultCount], tmpSearchCardResult.cbResultCard[i],
				sizeof(BYTE)*tmpSearchCardResult.cbCardCount[i]);
			cbResultCount++;
		}
	}

	pSearchCardResult->cbSearchCount = cbResultCount;
	return cbResultCount;
}

//ͬ������
BYTE CGameLogic::SearchSameCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbReferCard, BYTE cbSameCardCount,
	tagSearchCardResult *pSearchCardResult)
{
	//���ý��
	if (pSearchCardResult)
		ZeroMemory(pSearchCardResult, sizeof(tagSearchCardResult));
	BYTE cbResultCount = 0;

	//�����˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount = cbHandCardCount;
	CopyMemory(cbCardData, cbHandCardData, sizeof(BYTE)*cbHandCardCount);

	//�����˿�
	SortCardList(cbCardData, cbCardCount);

	//�����˿�
	tagAnalyseResult AnalyseResult = {};
	AnalysebCardData(cbCardData, cbCardCount, AnalyseResult);

	BYTE cbReferLogicValue = cbReferCard == 0 ? 0 : GetCardLogicValue(cbReferCard);
	BYTE cbBlockIndex = cbSameCardCount - 1;
	do
	{
		for (BYTE i = 0; i < AnalyseResult.cbBlockCount[cbBlockIndex]; i++)
		{
			BYTE cbIndex = (AnalyseResult.cbBlockCount[cbBlockIndex] - i - 1)*(cbBlockIndex + 1);
			if (GetCardLogicValue(AnalyseResult.cbCardData[cbBlockIndex][cbIndex]) > cbReferLogicValue)
			{
				if (pSearchCardResult == NULL) return 1;

				ASSERT(cbResultCount < CountArray(pSearchCardResult->cbCardCount));

				//�����˿�
				CopyMemory(pSearchCardResult->cbResultCard[cbResultCount], &AnalyseResult.cbCardData[cbBlockIndex][cbIndex],
					cbSameCardCount*sizeof(BYTE));
				pSearchCardResult->cbCardCount[cbResultCount] = cbSameCardCount;

				cbResultCount++;
			}
		}

		cbBlockIndex++;
	} while (cbBlockIndex < CountArray(AnalyseResult.cbBlockCount));

	if (pSearchCardResult)
		pSearchCardResult->cbSearchCount = cbResultCount;
	return cbResultCount;
}

//������������(����һ���Ĵ�һ��)
BYTE CGameLogic::SearchTakeCardType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbReferCard, BYTE cbSameCount, BYTE cbTakeCardCount,
	tagSearchCardResult *pSearchCardResult)
{
	//���ý��
	if (pSearchCardResult)
		ZeroMemory(pSearchCardResult, sizeof(tagSearchCardResult));
	BYTE cbResultCount = 0;

	//Ч��
	ASSERT(cbSameCount == 3 || cbSameCount == 4);
	ASSERT(cbTakeCardCount == 1 || cbTakeCardCount == 2);
	if (cbSameCount != 3 && cbSameCount != 4)
		return cbResultCount;
	if (cbTakeCardCount != 1 && cbTakeCardCount != 2)
		return cbResultCount;

	//�����ж�
	if (cbSameCount == 4 && cbHandCardCount<cbSameCount + cbTakeCardCount * 2 ||
		cbHandCardCount < cbSameCount + cbTakeCardCount)
		return cbResultCount;

	//�����˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount = cbHandCardCount;
	CopyMemory(cbCardData, cbHandCardData, sizeof(BYTE)*cbHandCardCount);

	//�����˿�
	SortCardList(cbCardData, cbCardCount);

	//����ͬ��
	tagSearchCardResult SameCardResult = {};
	BYTE cbSameCardResultCount = SearchSameCard(cbCardData, cbCardCount, cbReferCard, cbSameCount, &SameCardResult);

	if (cbSameCardResultCount > 0)
	{
		//�����˿�
		tagAnalyseResult AnalyseResult;
		AnalysebCardData(cbCardData, cbCardCount, AnalyseResult);

		//��Ҫ����
		BYTE cbNeedCount = cbSameCount + cbTakeCardCount;
		if (cbSameCount == 4) cbNeedCount += cbTakeCardCount;

		//��ȡ����
		for (BYTE i = 0; i < cbSameCardResultCount; i++)
		{
			bool bMerge = false;

			BYTE cbHaveTakeCount = 0;

			for (BYTE j = 0; j < CountArray(AnalyseResult.cbBlockCount); j++)
			{
				for (BYTE k = 0; k < AnalyseResult.cbBlockCount[j]; k++)
				{
					//��С����
					BYTE cbIndex = (AnalyseResult.cbBlockCount[j] - k - 1)*(j + 1);

					//������ͬ��
					if (GetCardValue(SameCardResult.cbResultCard[i][0]) ==
						GetCardValue(AnalyseResult.cbCardData[j][cbIndex]))
						continue;

					BYTE cbSubCount = cbTakeCardCount - cbHaveTakeCount;

					if (cbSubCount > (j + 1))
					{
						cbSubCount = j + 1;
					}


					//���ƴ���
					BYTE cbCount = SameCardResult.cbCardCount[i];
					CopyMemory(&SameCardResult.cbResultCard[i][cbCount], &AnalyseResult.cbCardData[j][cbIndex],
						sizeof(BYTE)*cbSubCount);
					SameCardResult.cbCardCount[i] += cbSubCount;
					cbHaveTakeCount += cbSubCount;
					if (SameCardResult.cbCardCount[i] < cbNeedCount) continue;

					if (pSearchCardResult == NULL) return 1;

					bMerge = true;

					//��һ���
					break;
				}

				if (bMerge) break;
			}

			if (bMerge)
			{
				//���ƽ��
				CopyMemory(pSearchCardResult->cbResultCard[cbResultCount], SameCardResult.cbResultCard[i],
					sizeof(BYTE)*SameCardResult.cbCardCount[i]);
				pSearchCardResult->cbCardCount[cbResultCount] = SameCardResult.cbCardCount[i];
				cbResultCount++;
			}
		}
	}

	if (pSearchCardResult)
		pSearchCardResult->cbSearchCount = cbResultCount;
	return cbResultCount;
}

//��������
BYTE CGameLogic::SearchLineCardType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbReferCard, BYTE cbBlockCount, BYTE cbLineCount,
	tagSearchCardResult *pSearchCardResult)
{
	//���ý��
	if (pSearchCardResult)
		ZeroMemory(pSearchCardResult, sizeof(tagSearchCardResult));
	BYTE cbResultCount = 0;

	//�������
	BYTE cbLessLineCount = 0;
	if (cbLineCount == 0)
	{
		if (cbBlockCount == 1)
			cbLessLineCount = 5;
		else if (cbBlockCount == 2)
			cbLessLineCount = 3;
		else cbLessLineCount = 2;
	}
	else cbLessLineCount = cbLineCount;

	BYTE cbReferIndex = 2;
	if (cbReferCard != 0)
	{
		ASSERT(GetCardLogicValue(cbReferCard) - cbLessLineCount >= 2);
		cbReferIndex = GetCardLogicValue(cbReferCard) - cbLessLineCount + 1;
	}
	//����A
	if (cbReferIndex + cbLessLineCount > 14) return cbResultCount;

	//�����ж�
	if (cbHandCardCount < cbLessLineCount*cbBlockCount) return cbResultCount;

	//�����˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount = cbHandCardCount;
	CopyMemory(cbCardData, cbHandCardData, sizeof(BYTE)*cbHandCardCount);

	//�����˿�
	SortCardList(cbCardData, cbCardCount);

	//�����˿�
	tagDistributing Distributing = {};
	AnalysebDistributing(cbCardData, cbCardCount, Distributing);

	//����˳��
	BYTE cbTmpLinkCount = 0;
	BYTE cbValueIndex = cbReferIndex;
	for (; cbValueIndex<13; cbValueIndex++)
	{
		//�����ж�
		if (Distributing.cbDistributing[cbValueIndex][cbIndexCount]<cbBlockCount)
		{
			if (cbTmpLinkCount < cbLessLineCount)
			{
				cbTmpLinkCount = 0;
				continue;
			}
			else cbValueIndex--;
		}
		else
		{
			cbTmpLinkCount++;
			//Ѱ�����
			if (cbLineCount == 0) continue;
		}

		if (cbTmpLinkCount >= cbLessLineCount)
		{
			if (pSearchCardResult == NULL) return 1;

			ASSERT(cbResultCount < CountArray(pSearchCardResult->cbCardCount));

			//�����˿�
			BYTE cbCount = 0;
			for (BYTE cbIndex = cbValueIndex + 1 - cbTmpLinkCount; cbIndex <= cbValueIndex; cbIndex++)
			{
				BYTE cbTmpCount = 0;
				for (BYTE cbColorIndex = 0; cbColorIndex<4; cbColorIndex++)
				{
					for (BYTE cbColorCount = 0; cbColorCount < Distributing.cbDistributing[cbIndex][3 - cbColorIndex]; cbColorCount++)
					{
						pSearchCardResult->cbResultCard[cbResultCount][cbCount++] = MakeCardData(cbIndex, 3 - cbColorIndex);

						if (++cbTmpCount == cbBlockCount) break;
					}
					if (cbTmpCount == cbBlockCount) break;
				}
			}

			//���ñ���
			pSearchCardResult->cbCardCount[cbResultCount] = cbCount;
			cbResultCount++;

			if (cbLineCount != 0)
			{
				cbTmpLinkCount--;
			}
			else
			{
				cbTmpLinkCount = 0;
			}
		}
	}

	//����˳��
	if (cbTmpLinkCount >= cbLessLineCount - 1 && cbValueIndex == 13)
	{
		if (Distributing.cbDistributing[0][cbIndexCount] >= cbBlockCount ||
			cbTmpLinkCount >= cbLessLineCount)
		{
			if (pSearchCardResult == NULL) return 1;

			ASSERT(cbResultCount < CountArray(pSearchCardResult->cbCardCount));

			//�����˿�
			BYTE cbCount = 0;
			BYTE cbTmpCount = 0;
			for (BYTE cbIndex = cbValueIndex - cbTmpLinkCount; cbIndex < 13; cbIndex++)
			{
				cbTmpCount = 0;
				for (BYTE cbColorIndex = 0; cbColorIndex<4; cbColorIndex++)
				{
					for (BYTE cbColorCount = 0; cbColorCount < Distributing.cbDistributing[cbIndex][3 - cbColorIndex]; cbColorCount++)
					{
						pSearchCardResult->cbResultCard[cbResultCount][cbCount++] = MakeCardData(cbIndex, 3 - cbColorIndex);

						if (++cbTmpCount == cbBlockCount) break;
					}
					if (cbTmpCount == cbBlockCount) break;
				}
			}
			//����A
			if (Distributing.cbDistributing[0][cbIndexCount] >= cbBlockCount)
			{
				cbTmpCount = 0;
				for (BYTE cbColorIndex = 0; cbColorIndex<4; cbColorIndex++)
				{
					for (BYTE cbColorCount = 0; cbColorCount < Distributing.cbDistributing[0][3 - cbColorIndex]; cbColorCount++)
					{
						pSearchCardResult->cbResultCard[cbResultCount][cbCount++] = MakeCardData(0, 3 - cbColorIndex);

						if (++cbTmpCount == cbBlockCount) break;
					}
					if (cbTmpCount == cbBlockCount) break;
				}
			}

			//���ñ���
			pSearchCardResult->cbCardCount[cbResultCount] = cbCount;
			cbResultCount++;
		}
	}

	if (pSearchCardResult)
		pSearchCardResult->cbSearchCount = cbResultCount;
	return cbResultCount;
}

//�����˿�
BYTE CGameLogic::MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex)
{
	return (cbColorIndex << 4) | (cbValueIndex + 1);
}
//////////////////////////////////////////////////////////////////////////
