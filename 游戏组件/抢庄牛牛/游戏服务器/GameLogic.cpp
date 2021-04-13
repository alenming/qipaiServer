#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//�˿�����
BYTE CGameLogic::m_cbCardListDataNoKing[52]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,		//���� A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,		//÷�� A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,		//���� A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D		//���� A - K
};

//�˿�����
BYTE CGameLogic::m_cbCardListDataHaveKing[54]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,		//���� A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,		//÷�� A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,		//���� A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,		//���� A - K
	0x4E,0x4F
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
BYTE CGameLogic::GetCardType(BYTE cbCardData[], BYTE cbCardCount)
{
	//����Ч��
	ASSERT (cbCardCount == MAX_CARDCOUNT);

	//�����˿�
	tagAnalyseResult AnalyseResult;
	ZeroMemory(&AnalyseResult, sizeof(AnalyseResult));
	AnalysebCardData(cbCardData, cbCardCount, AnalyseResult);
	
	//�����˿�
	BYTE cbCardDataSort[5];
	CopyMemory(cbCardDataSort, cbCardData, sizeof(BYTE)*cbCardCount);
	SortCardList(cbCardDataSort, cbCardCount,true);


	//��Сţ�ж�
	BYTE cbAllValue = 0;
	for (int i = 0; i < cbCardCount; i++)
	{
		cbAllValue += GetCardLogicValue(cbCardDataSort[i]);
	}

	if (cbAllValue <= 10)
		return CT_OX_VALUE_FIVESNIUNIU;

	//�廨ţ
	bool bWuHua = true;

	for (int i = 0; i < cbCardCount; i++)
	{
		if (GetCardValue(cbCardDataSort[i]) <= 10)
		{
			bWuHua = false;
			break;
		}
	}
	if (bWuHua)
	{
		return CT_OX_VALUE_FIVEFLOWER;
	}

	//ը��ţ�ж�
	if (GetCardValue(cbCardDataSort[0]) == GetCardValue(cbCardDataSort[cbCardCount - 2]))
	{
		return CT_OX_VALUE_BOMB;
	}
	else
	{
		if (GetCardValue(cbCardDataSort[1]) == GetCardValue(cbCardDataSort[cbCardCount - 1]))
		{
			return CT_OX_VALUE_BOMB;
		}

	}

	//���������ͨţ���ж�
	int n = 0;

	int iBigValue = 0;
	BYTE iSingleA[2];
	int iIndex = 0;

	int iGetTenCount = 0;

	for (int iten = 0; iten<cbCardCount; iten++)
	{
		if (GetCardLogicValue(cbCardDataSort[iten]) == 10)
		{
			iGetTenCount++;

		}
	}
	if (iGetTenCount >= 3)
	{
		return RetType(GetCardLogicValue(cbCardDataSort[3]) + GetCardLogicValue(cbCardDataSort[4]));
	}

	if (iGetTenCount == 2)
	{
		if ((GetCardLogicValue(cbCardDataSort[2]) + GetCardLogicValue(cbCardDataSort[3]) + GetCardLogicValue(cbCardDataSort[4])) % 10 == 0)
		{
			return CT_OX_VALUENIUNIU;
		}
		else
		{
			for (n = 2; n<cbCardCount; n++)
			{
				for (int j = 2; j<cbCardCount; j++)
				{
					if (j != n)
					{
						if ((GetCardLogicValue(cbCardDataSort[n]) + GetCardLogicValue(cbCardDataSort[j])) % 10 == 0)
						{
							int add = 0;
							for (int y = 2; y<cbCardCount; y++)
							{
								if (y != n&&y != j)
								{
									iSingleA[add] = cbCardDataSort[y];
									add++;

								}
							}
							if (iBigValue <= iSingleA[0] % 10)
							{
								iBigValue = GetCardLogicValue(iSingleA[0]) % 10;

								if (iBigValue == 0)
								{

									return CT_OX_VALUENIUNIU;
								}
							}

						}
					}
				}
			}
			if (iBigValue != 0)
			{
				return RetType(iBigValue);
			}
		}

		iGetTenCount = 1;

	}
	//4�����
	if (iGetTenCount == 1)
	{

		//ȡ4�������������Ϊ10 Ȼ�����������ŵ���Ͽ��Ƿ�����������
		for (n = 1; n<cbCardCount; n++)
		{
			for (int j = 1; j<cbCardCount; j++)
			{
				if (j != n)
				{
					if ((GetCardLogicValue(cbCardDataSort[n]) + GetCardLogicValue(cbCardDataSort[j])) % 10 == 0)
					{
						int add = 0;
						for (int y = 1; y<cbCardCount; y++)
						{
							if (y != n&&y != j)
							{
								iSingleA[add] = cbCardDataSort[y];
								add++;

							}

						}
						if (iBigValue <= (GetCardLogicValue(iSingleA[0]) + GetCardLogicValue(iSingleA[1])) % 10)
						{
							iBigValue = GetCardLogicValue(iSingleA[0]) + GetCardLogicValue(iSingleA[1]) % 10;

							if (iBigValue == 0)
							{

								return CT_OX_VALUENIUNIU;
							}
						}

					}
				}
			}
		}
		if (iBigValue != 0)
		{
			return RetType(iBigValue);
		}
		else
		{
			//�����ϲ��ɹ�
			iGetTenCount = 0;
		}

	}
	if (iGetTenCount == 0)
	{
		//5�����
		for (n = 0; n<cbCardCount; n++)
		{
			for (int j = 0; j<cbCardCount; j++)
			{
				if (j != n)
				{
					for (int w = 0; w<cbCardCount; w++)
					{
						if (w != n&&w != j)
						{
							int valueAdd = GetCardLogicValue(cbCardDataSort[n]);
							valueAdd += GetCardLogicValue(cbCardDataSort[j]);
							valueAdd += GetCardLogicValue(cbCardDataSort[w]);

							if (valueAdd % 10 == 0)
							{
								int add = 0;
								for (int y = 0; y<cbCardCount; y++)
								{
									if (y != n&&y != j&&y != w)
									{
										iSingleA[add] = cbCardDataSort[y];
										add++;

									}

								}
								if (iBigValue <= (GetCardLogicValue(iSingleA[0]) + GetCardLogicValue(iSingleA[1])) % 10)
								{
									iBigValue = GetCardLogicValue(iSingleA[0]) + GetCardLogicValue(iSingleA[1]) % 10;

									if (iBigValue == 0)
									{

										return CT_OX_VALUENIUNIU;
									}
								}

							}

						}
					}
				}
			}
		}
		if (iBigValue != 0)
		{
			return RetType(iBigValue);
		}
		else
		{
			return CT_OX_VALUE0;
		}

	}

	return CT_OX_VALUE0;
}

//��ȡ����
BYTE CGameLogic::GetTimes(BYTE cbCardData[], BYTE cbCardCount)
{
	if (cbCardCount != MAX_CARDCOUNT)
	{
		return 0;
	}
	
	//��ȡ����
	BYTE cbCardType =  GetCardType(cbCardData, MAX_CARDCOUNT);

	switch(cbCardType)
	{
		case CT_OX_VALUE_FIVESNIUNIU:
		case CT_OX_VALUE_FIVEFLOWER:
			{
				return 5;
			}
		case CT_OX_VALUE_BOMB:
			{
				return 4;
			}
		case CT_OX_VALUENIUNIU:
			{
				return 3;
			}
		case CT_OX_VALUE9:
		case CT_OX_VALUE8:
		case CT_OX_VALUE7:
			{
				return 2;
			}
		case CT_OX_VALUE6:
		case CT_OX_VALUE5:
		case CT_OX_VALUE4:
		case CT_OX_VALUE3:
		case CT_OX_VALUE2:
		case CT_OX_VALUE1:
		case CT_OX_VALUE0:
			{
				return 1;
			}
	}
	
	return INVALID_BYTE;
}

void CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount, bool bDescend)
{
	//ת����ֵ
	BYTE cbSortValue[MAX_CARDCOUNT];
	for (BYTE i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardValue(cbCardData[i]);	

	//�������
	bool bSorted=true;
	BYTE cbSwitchData=0,cbLast=cbCardCount-1;
	
	//����
	if (bDescend)
	{
		do
		{
			bSorted=true;
			for (BYTE i=0;i<cbLast;i++)
			{
				if ((cbSortValue[i]<cbSortValue[i+1])||
					((cbSortValue[i]==cbSortValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
				{
					//���ñ�־
					bSorted=false;

					//�˿�����
					cbSwitchData=cbCardData[i];
					cbCardData[i]=cbCardData[i+1];
					cbCardData[i+1]=cbSwitchData;

					//����Ȩλ
					cbSwitchData=cbSortValue[i];
					cbSortValue[i]=cbSortValue[i+1];
					cbSortValue[i+1]=cbSwitchData;
				}	
			}
			cbLast--;
		} while(bSorted==false);
	}
	else
	{
		do
		{
			bSorted=true;
			for (BYTE i=0;i<cbLast;i++)
			{
				if ((cbSortValue[i]>cbSortValue[i+1])||
					((cbSortValue[i]==cbSortValue[i+1])&&(cbCardData[i]>cbCardData[i+1])))
				{
					//���ñ�־
					bSorted=false;

					//�˿�����
					cbSwitchData=cbCardData[i];
					cbCardData[i]=cbCardData[i+1];
					cbCardData[i+1]=cbSwitchData;

					//����Ȩλ
					cbSwitchData=cbSortValue[i];
					cbSortValue[i]=cbSortValue[i+1];
					cbSortValue[i+1]=cbSwitchData;
				}	
			}
			cbLast--;
		} while(bSorted==false);
	}
}

//�����˿�
void CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//����׼��
	BYTE cbCardData[CountArray(m_cbCardListDataNoKing)];
	CopyMemory(cbCardData,m_cbCardListDataNoKing,sizeof(m_cbCardListDataNoKing));

	//�����˿�
	BYTE bRandCount=0,bPosition=0;
	do
	{
		bPosition=rand()%(CountArray(m_cbCardListDataNoKing)-bRandCount);
		cbCardBuffer[bRandCount++]=cbCardData[bPosition];
		cbCardData[bPosition]=cbCardData[CountArray(m_cbCardListDataNoKing)-bRandCount];
	} while (bRandCount<cbBufferCount);

	return;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount)
{
	//��������
	ASSERT(cbRemoveCount <= cbCardCount);

	//�������
	BYTE cbDeleteCount = 0, cbTempCardData[52];
	ZeroMemory(cbTempCardData, sizeof(cbTempCardData));
	CopyMemory(cbTempCardData, cbCardData, cbCardCount*sizeof(cbCardData[0]));

	//�����˿�
	for (BYTE i = 0; i<cbRemoveCount; i++)
	{
		for (BYTE j = 0; j<cbCardCount; j++)
		{
			if (cbRemoveCard[i] == cbTempCardData[j])
			{
				cbDeleteCount++;
				cbTempCardData[j] = 0;
				break;
			}
		}
	}
	if (cbDeleteCount != cbRemoveCount) return false;

	//�����˿�
	BYTE cbCardPos = 0;
	for (BYTE i = 0; i<cbCardCount; i++)
	{
		if (cbTempCardData[i] != 0) cbCardData[cbCardPos++] = cbTempCardData[i];
	}

	return true;
}

//�߼���ֵ(�¼�����ʹ��)
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
	//�˿�����
	BYTE cbCardColor=GetCardColor(cbCardData);
	BYTE cbCardValue=GetCardValue(cbCardData);

	//ת����ֵ
	if (cbCardValue >= 0x0A)
		cbCardValue = 0x0A;
	return cbCardValue;
}

//�Ա��˿�
bool CGameLogic::CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount)
{
	//�Ƚ�ţ��С(FirstOX ֻ���ж���ţһ������)
	//��ȡ����
	BYTE cbNextType = GetCardType(cbNextData, cbCardCount);
	BYTE cbFirstType = GetCardType(cbFirstData, cbCardCount);

	//�����ж�
	if (cbFirstType != cbNextType) 
	{
		return (cbFirstType > cbNextType);
	}
	
	//�����˿�
	tagAnalyseResult FirstAnalyseResult;
	tagAnalyseResult NextAnalyseResult;
	ZeroMemory(&FirstAnalyseResult, sizeof(FirstAnalyseResult));
	ZeroMemory(&NextAnalyseResult, sizeof(NextAnalyseResult));
	AnalysebCardData(cbFirstData, cbCardCount, FirstAnalyseResult);
	AnalysebCardData(cbNextData, cbCardCount, NextAnalyseResult);
	
	
	//�Ƚ�ͬ���͵�(����ģʽ�ͼӱ�ģʽ�ĺ궨��ֵһ��,����ֻ�о���ģʽ�������ڼӱ�ģʽ�µı���)
	switch(cbNextType)
	{
		case CT_OX_VALUE0:
		case CT_OX_VALUE1:
		case CT_OX_VALUE2:
		case CT_OX_VALUE3:
		case CT_OX_VALUE4:
		case CT_OX_VALUE5:
		case CT_OX_VALUE6:
		case CT_OX_VALUE7:
		case CT_OX_VALUE8:
		case CT_OX_VALUE9:
		case CT_OX_VALUENIUNIU:
		case CT_OX_VALUE_FIVEFLOWER:
		{
			//�����С
			BYTE bFirstTemp[MAX_CARDCOUNT];
			BYTE bNextTemp[MAX_CARDCOUNT];
			CopyMemory(bFirstTemp,cbFirstData,cbCardCount);
			CopyMemory(bNextTemp,cbNextData,cbCardCount);
			SortCardList(bFirstTemp,cbCardCount,true);
			SortCardList(bNextTemp, cbCardCount, true);

			//�Ƚ���ֵ
			BYTE cbNextMaxValue=GetCardValue(bNextTemp[0]);
			BYTE cbFirstMaxValue=GetCardValue(bFirstTemp[0]);
			if (cbNextMaxValue!=cbFirstMaxValue)
			{
				return cbFirstMaxValue>cbNextMaxValue;
			}

			//�Ƚ���ɫ
			return GetCardColor(bFirstTemp[0]) > GetCardColor(bNextTemp[0]);
		}
		//��Сţ
		case CT_OX_VALUE_FIVESNIUNIU:
		{
			BYTE cbAllFirstValue = 0;
			BYTE cbAllNextValue = 0;
			for (int i = 0; i < cbCardCount; i++)
			{
				cbAllFirstValue += GetCardLogicValue(cbFirstData[i]);
				cbAllNextValue += GetCardLogicValue(cbNextData[i]);
			}


			if (cbAllFirstValue == cbAllNextValue)
			{
				//�����С
				BYTE bFirstTemp[MAX_CARDCOUNT];
				BYTE bNextTemp[MAX_CARDCOUNT];
				CopyMemory(bFirstTemp, cbFirstData, cbCardCount);
				CopyMemory(bNextTemp, cbNextData, cbCardCount);
				SortCardList(bFirstTemp, cbCardCount, false);
				SortCardList(bNextTemp, cbCardCount, false);

				//�Ƚ���ֵ
				BYTE cbNextMinValue = GetCardValue(bNextTemp[0]);
				BYTE cbFirstMinValue = GetCardValue(bFirstTemp[0]);
				if (cbNextMinValue != cbFirstMinValue)
				{
					return cbFirstMinValue<cbNextMinValue;
				}

				//�Ƚ���ɫ
				return GetCardColor(bFirstTemp[0]) < GetCardColor(bNextTemp[0]);
			}
			else
			{
				return cbAllFirstValue < cbAllNextValue;
			}

			return false;
		}
		//ը��
	case CT_OX_VALUE_BOMB:
		{

			//�Ƚ���ֵ
			BYTE cbNextMaxValue = GetCardLogicValue(NextAnalyseResult.cbCardData[3][0]);
			BYTE cbFirstMaxValue = GetCardLogicValue(FirstAnalyseResult.cbCardData[3][0]);
			if (cbNextMaxValue != cbFirstMaxValue)
			{
				return cbFirstMaxValue > cbNextMaxValue;
			}


			return false;
		}
	}
	
	return false;
}

//�����˿�
bool CGameLogic::AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult)
{
	//���ý��
	ZeroMemory(&AnalyseResult,sizeof(AnalyseResult));

	//�˿˷���
	for (BYTE i=0;i<cbCardCount;i++)
	{
		//��������
		BYTE cbSameCount=1,cbCardValueTemp=0;
		BYTE cbLogicValue=GetCardLogicValue(cbCardData[i]);

		//��С������
		if (GetCardColor(cbCardData[i]) == GetCardColor(0x4E))
		{
			AnalyseResult.cbKingCount++;
		}

		//����ͬ��
		for (BYTE j=i+1;j<cbCardCount;j++)
		{
			//��ȡ�˿�
			if (GetCardLogicValue(cbCardData[j])!=cbLogicValue) break;

			//���ñ���
			cbSameCount++;
		}
		if(cbSameCount > 4)
		{
			return false;
		}
		//���ý��
		BYTE cbIndex = AnalyseResult.cbBlockCount[cbSameCount-1]++;
		for (BYTE j=0;j<cbSameCount;j++)
		{
			AnalyseResult.cbCardData[cbSameCount-1][cbIndex*cbSameCount+j]=cbCardData[i+j];
		}

		//��������
		i+=cbSameCount-1;
	}

	return true;
}

int CGameLogic::RetType(int itype)
{
	itype = itype % 10;
	switch (itype)
	{
	case 0:
		return CT_OX_VALUENIUNIU;
	case 1:
		return CT_OX_VALUE1;
		break;
	case 2:
		return CT_OX_VALUE2;
		break;
	case 3:
		return CT_OX_VALUE3;
		break;
	case 4:
		return CT_OX_VALUE4;
		break;
	case 5:
		return CT_OX_VALUE5;
		break;
	case 6:
		return CT_OX_VALUE6;
		break;
	case 7:
		return CT_OX_VALUE7;
		break;
	case 8:
		return CT_OX_VALUE8;
		break;
	case 9:
		return CT_OX_VALUE9;
		break;
	default:
		return CT_OX_VALUE0;
		break;
	}

}
//////////////////////////////////////////////////////////////////////////
