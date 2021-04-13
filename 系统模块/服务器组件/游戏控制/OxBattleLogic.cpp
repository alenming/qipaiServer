#include "StdAfx.h"
#include "OxBattleLogic.h"

//////////////////////////////////////////////////////////////////////////

//�˿�����
const BYTE COxBattleLogic::m_cbCardListData[CARD_COUNT] =
{
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
		0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
		//0x41,0x42,
};

//////////////////////////////////////////////////////////////////////////

//���캯��
COxBattleLogic::COxBattleLogic()
{
}

//��������
COxBattleLogic::~COxBattleLogic()
{
}

//�����˿�
void COxBattleLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//����׼��
	BYTE cbCardData[CountArray(m_cbCardListData)];
	CopyMemory(cbCardData,m_cbCardListData,sizeof(m_cbCardListData));

	//�����˿�
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(CountArray(cbCardData)-cbRandCount);
		cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
		cbCardData[cbPosition]=cbCardData[CountArray(cbCardData)-cbRandCount];
	} while (cbRandCount<cbBufferCount);

	return;
}

//ɾ���˿�
bool COxBattleLogic::RemoveCardList(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount)
{
	//��������
	ASSERT(cbRemoveCount <= cbCardCount);

	//�������
	BYTE cbDeleteCount = 0, cbTempCardData[52];
	if (cbCardCount>CountArray(cbTempCardData)) return false;
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

	ZeroMemory(cbCardData, cbCardCount*sizeof(cbCardData[0]));
	//�����˿�
	BYTE cbCardPos = 0;
	for (BYTE i = 0; i<cbCardCount; i++)
	{
		if (cbTempCardData[i] != 0) cbCardData[cbCardPos++] = cbTempCardData[i];
	}

	return true;
}

void COxBattleLogic::RandCardListEx(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	BYTE *cbCardData=new BYTE[cbBufferCount];
	CopyMemory(cbCardData,cbCardBuffer,sizeof(BYTE)*cbBufferCount);

	//�����˿�
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(cbBufferCount-cbRandCount);
		cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
		cbCardData[cbPosition]=cbCardData[cbBufferCount-cbRandCount];
	} while (cbRandCount<cbBufferCount);
	delete[] cbCardData;
}

int COxBattleLogic::RetType(int itype)
{
	itype = itype%10;
	switch(itype)
	{
	case 0:
		return CT_SPECIAL_NIUNIU;
	case 1:
		return CT_SPECIAL_NIU1;
		break;
	case 2:
		return CT_SPECIAL_NIU2;
		break;
	case 3:
		return CT_SPECIAL_NIU3;
		break;
	case 4:
		return CT_SPECIAL_NIU4;
		break;
	case 5:
		return CT_SPECIAL_NIU5;
		break;
	case 6:
		return CT_SPECIAL_NIU6;
		break;
	case 7:
		return CT_SPECIAL_NIU7;
		break;
	case 8:
		return CT_SPECIAL_NIU8;
		break;
	case 9:
		return CT_SPECIAL_NIU9;
		break;
	default :
		return CT_POINT;
		break;
	}

}
//��ȡ����
BYTE COxBattleLogic::GetCardType(const BYTE cbCardData[], BYTE cbCardCount, BYTE *bcOutCadData)
{
	//�Ϸ��ж�
	ASSERT(5==cbCardCount);
	if (5!=cbCardCount) return CT_POINT;

	//�����˿�
	BYTE cbCardDataSort[CARD_COUNT];
	CopyMemory(cbCardDataSort,cbCardData,sizeof(BYTE)*cbCardCount);
	SortCardList(cbCardDataSort,cbCardCount,ST_NEW);

	if(bcOutCadData != NULL)
	{
		CopyMemory(bcOutCadData,cbCardDataSort,cbCardCount);
	}

	//��Сţ�ж�
	BYTE cbAllValue = 0;
	for (int i = 0; i < cbCardCount; i++)
	{
		cbAllValue += GetCardLogicValue(cbCardDataSort[i]);
	}

	if (cbAllValue <= 10)
		return CT_SPECIAL_XIAO;

	//�廨ţ
	bool bWuHua = true;

	for (int i = 0; i < cbCardCount; i++)
	{
		if (GetCardNewValue(cbCardDataSort[i]) <= 10)
		{
			bWuHua = false;
			break;
		}
	}
	if (bWuHua)
	{
		if (bcOutCadData != NULL)
		{
			CopyMemory(bcOutCadData, cbCardDataSort, cbCardCount);
		}

		return CT_SPECIAL_NIUWUHUA;
	}

	//ը��ţ�ж�
	if(GetCardNewValue(cbCardDataSort[0])==GetCardNewValue(cbCardDataSort[cbCardCount-2]))
	{
		if(bcOutCadData != NULL)
		{
			CopyMemory(bcOutCadData,cbCardDataSort,cbCardCount);
		}
		return CT_SPECIAL_BOMEBOME;
	}
	else
	{
		if(GetCardNewValue(cbCardDataSort[1])==GetCardNewValue(cbCardDataSort[cbCardCount-1]))
		{
			if(bcOutCadData != NULL)
			{
				CopyMemory(bcOutCadData,cbCardDataSort,cbCardCount);
			}
			return CT_SPECIAL_BOMEBOME;
		}

	}

	//���������ͨţ���ж�
	int n = 0;

	BYTE bcMakeMax[5];
	memset(bcMakeMax,0,5);
	int iBigValue = 0;
	BYTE iSingleA[2];
	int iIndex = 0;
	bcMakeMax[0]= cbCardDataSort[n];

	int iGetTenCount = 0;

	for (int iten = 0;iten<cbCardCount;iten++)
	{
		if(GetCardLogicValue(cbCardDataSort[iten])==10)
		{
			iGetTenCount++;

		}
	}
	if( iGetTenCount>=3)
	{
		return RetType(GetCardLogicValue(cbCardDataSort[3])+GetCardLogicValue(cbCardDataSort[4]));
	}

	if(iGetTenCount==2)
	{
		if((GetCardLogicValue(cbCardDataSort[2])+GetCardLogicValue(cbCardDataSort[3])+GetCardLogicValue(cbCardDataSort[4]))%10==0)
		{
			if(bcOutCadData != NULL)
			{
				bcOutCadData[0] = cbCardDataSort[2];
				bcOutCadData[1] = cbCardDataSort[3];
				bcOutCadData[2] = cbCardDataSort[4];
				bcOutCadData[3] = cbCardDataSort[0];
				bcOutCadData[4] = cbCardDataSort[1];
			}
			return CT_SPECIAL_NIUNIU;
		}else
		{
			for ( n= 2;n<cbCardCount;n++)
			{
				for (int j = 2;j<cbCardCount;j++)
				{
					if(j != n)
					{
						if((GetCardLogicValue(cbCardDataSort[n])+GetCardLogicValue(cbCardDataSort[j]))%10==0)
						{
							int add = 0;
							for (int y = 2;y<cbCardCount;y++)
							{
								if(y != n&&y!=j)
								{
									iSingleA[add] =cbCardDataSort[y]; 
									add++;

								}
							}
							if(iBigValue<=iSingleA[0]%10)
							{
								iBigValue = GetCardLogicValue(iSingleA[0])%10;
								if(bcOutCadData != NULL)
								{
									bcOutCadData[0]= cbCardDataSort[0];
									bcOutCadData[1]= cbCardDataSort[n]; 
									bcOutCadData[2]= cbCardDataSort[j]; 
									bcOutCadData[3]= cbCardDataSort[1];
									bcOutCadData[4]= iSingleA[0]; 

								}

								if(iBigValue==0)
								{

									return CT_SPECIAL_NIUNIU;
								}
							}

						}
					}
				}
			}
			if(iBigValue != 0)
			{
				return RetType(iBigValue);
			}
		}

		iGetTenCount = 1;

	}
	//4�����
	if(iGetTenCount==1)
	{
		
		//ȡ4�������������Ϊ10 Ȼ�����������ŵ���Ͽ��Ƿ�����������
		for ( n= 1;n<cbCardCount;n++)
		{
			for (int j = 1;j<cbCardCount;j++)
			{
				if(j != n)
				{
					if((GetCardLogicValue(cbCardDataSort[n])+GetCardLogicValue(cbCardDataSort[j]))%10==0)
					{
						int add = 0;
						for (int y = 1;y<cbCardCount;y++)
						{
							if(y != n&&y!=j)
							{
								iSingleA[add] =cbCardDataSort[y]; 
								add++;

							}

						}
						if(iBigValue<=(GetCardLogicValue(iSingleA[0])+GetCardLogicValue(iSingleA[1]))%10)
						{
							iBigValue = GetCardLogicValue(iSingleA[0])+GetCardLogicValue(iSingleA[1])%10;
							bcMakeMax[0]= cbCardDataSort[0];
							bcMakeMax[1]= cbCardDataSort[j];
							bcMakeMax[2]= cbCardDataSort[n]; 
							bcMakeMax[3]= iSingleA[0]; 
							bcMakeMax[4]= iSingleA[1]; 

							if(bcOutCadData != NULL)
							{
								CopyMemory(bcOutCadData,bcMakeMax,cbCardCount);
							}
							if(iBigValue==0)
							{

								return CT_SPECIAL_NIUNIU;
							}
						}

					}
				}
			}
		}
		if(iBigValue!= 0)
		{
			return RetType(iBigValue);
		}else
		{
			//�����ϲ��ɹ�
			iGetTenCount = 0;
		}

	}
	if(iGetTenCount==0)
	{
		//5�����
		for ( n= 0;n<cbCardCount;n++)
		{
			for (int j = 0;j<cbCardCount;j++)
			{
				if(j != n)
				{
					for (int w = 0;w<cbCardCount;w++)
					{
						if(w != n&&w!=j)
						{
							int valueAdd = GetCardLogicValue(cbCardDataSort[n]);
							valueAdd += GetCardLogicValue(cbCardDataSort[j]);
							valueAdd += GetCardLogicValue(cbCardDataSort[w]);

							if(valueAdd%10==0)
							{
								int add = 0;
								for (int y = 0;y<cbCardCount;y++)
								{
									if(y != n&&y!=j&&y!=w)
									{
										iSingleA[add] =cbCardDataSort[y]; 
										add++;

									}

								}
								if(iBigValue<=(GetCardLogicValue(iSingleA[0])+GetCardLogicValue(iSingleA[1]))%10)
								{
									iBigValue = GetCardLogicValue(iSingleA[0])+GetCardLogicValue(iSingleA[1])%10;
									bcMakeMax[0]= cbCardDataSort[n];
									bcMakeMax[1]= cbCardDataSort[j];
									bcMakeMax[2]= cbCardDataSort[w]; 
									bcMakeMax[3]= iSingleA[0]; 
									bcMakeMax[4]= iSingleA[1]; 

									if(bcOutCadData != NULL)
									{
										CopyMemory(bcOutCadData,bcMakeMax,cbCardCount);
									}
									if(iBigValue==0)
									{

										return CT_SPECIAL_NIUNIU;
									}
								}

							}

						}
					}
				}
			}		
		}
		if(iBigValue!=0)
		{
			return RetType(iBigValue);
		}	
		else
		{
			return CT_POINT;
		}

	}

	return CT_POINT;
}

//��С�Ƚ�
/*
cbNextCardData>cbFirstCardData  ����1
cbNextCardData<cbFirstCardData  ����-1
cbNextCardData==cbFirstCardData ����0
*/
//Multiple �Ƚϳ����ı���
int COxBattleLogic::CompareCard(const BYTE cbFirstCardData[], BYTE cbFirstCardCount, const BYTE cbNextCardData[], BYTE cbNextCardCount, int nServerLevel, BYTE &Multiple)
{
	//�Ϸ��ж�
	ASSERT(5==cbFirstCardCount && 5==cbNextCardCount);
	if (!(5==cbFirstCardCount && 5==cbNextCardCount)) return 0;

	//��ȡ����
	BYTE cbFirstCardType=GetCardType(cbFirstCardData, cbFirstCardCount);
	BYTE cbNextCardType=GetCardType(cbNextCardData, cbNextCardCount);

	//���ͱȽ�
	if (cbFirstCardType != cbNextCardType) 
	{
		if (cbNextCardType > cbFirstCardType)
		{
			if (nServerLevel == 2)
			{
				if (cbNextCardType >= CT_SPECIAL_BOMEBOME)
					Multiple = 10;
				else if (cbNextCardType >= CT_SPECIAL_NIU1)
					Multiple = cbNextCardType;
				else
					Multiple = 1;
			}
			else
			{
				if (cbNextCardType >= CT_SPECIAL_NIUWUHUA)
					Multiple = 5;
				else if (cbNextCardType >= CT_SPECIAL_BOMEBOME)
					Multiple = 4;
				else if (cbNextCardType >= CT_SPECIAL_NIUNIU)
					Multiple = 3;
				else if (cbNextCardType >= CT_SPECIAL_NIU7)
					Multiple = 2;
				else
					Multiple = 1;
			}

			return 1;

		}
		else
		{
			if (nServerLevel == 2)
			{
				if (cbFirstCardType >= CT_SPECIAL_BOMEBOME)
					Multiple = 10;
				else if (cbFirstCardType >= CT_SPECIAL_NIU1)
					Multiple = cbFirstCardType;
				else
					Multiple = 1;
			}
			else
			{
				if (cbFirstCardType >= CT_SPECIAL_NIUWUHUA)
					Multiple = 5;
				else if (cbFirstCardType >= CT_SPECIAL_BOMEBOME)
					Multiple = 4;
				else if (cbFirstCardType >= CT_SPECIAL_NIUNIU)
					Multiple = 3;
				else if (cbFirstCardType >= CT_SPECIAL_NIU7)
					Multiple = 2;
				else
					Multiple = 1;
			}

			return -1;
		}
	}

	//���������ж�
	if (CT_POINT!=cbFirstCardType && cbFirstCardType==cbNextCardType)
	{
		//�����˿�
		BYTE cbFirstCardDataTmp[CARD_COUNT], cbNextCardDataTmp[CARD_COUNT];

		CopyMemory(cbFirstCardDataTmp,cbFirstCardData,sizeof(BYTE)*cbFirstCardCount);
		CopyMemory(cbNextCardDataTmp,cbNextCardData,sizeof(BYTE)*cbNextCardCount);
		SortCardList(cbFirstCardDataTmp,cbFirstCardCount,ST_NEW);
		SortCardList(cbNextCardDataTmp,cbNextCardCount,ST_NEW);

		if (nServerLevel == 2)
		{
			if (cbFirstCardType >= CT_SPECIAL_BOMEBOME)
				Multiple = 10;
			else if (cbFirstCardType >= CT_SPECIAL_NIU1)
				Multiple = cbFirstCardType;
			else
				Multiple = 1;
		}
		else
		{
			if (cbFirstCardType >= CT_SPECIAL_NIUWUHUA)
				Multiple = 5;
			else if (cbFirstCardType >= CT_SPECIAL_BOMEBOME)
				Multiple = 4;
			else if (cbFirstCardType >= CT_SPECIAL_NIUNIU)
				Multiple = 3;
			else if (cbFirstCardType >= CT_SPECIAL_NIU7)
				Multiple = 2;
			else
				Multiple = 1;
		}

		BYTE firstValue = GetCardNewValue(cbFirstCardDataTmp[0]);
		BYTE NextValue = GetCardNewValue(cbNextCardDataTmp[0]);

		BYTE firstColor = GetCardColor(cbFirstCardDataTmp[0]);

		BYTE NextColor = GetCardColor(cbNextCardDataTmp[0]);


		if(firstValue<NextValue)
		{
			return 1;
		}else
		{
			if(firstValue==NextValue)
			{
				if(firstColor<NextColor)
				{
					return 1;

				}else
				{
					return -1;
				}
			}
			return -1;
		}

	}

	//�����˿�
	BYTE cbFirstCardDataTmp[CARD_COUNT], cbNextCardDataTmp[CARD_COUNT];
	CopyMemory(cbFirstCardDataTmp,cbFirstCardData,sizeof(BYTE)*cbFirstCardCount);
	CopyMemory(cbNextCardDataTmp,cbNextCardData,sizeof(BYTE)*cbNextCardCount);
	SortCardList(cbFirstCardDataTmp,cbFirstCardCount,ST_NEW);
	SortCardList(cbNextCardDataTmp,cbNextCardCount,ST_NEW);

	BYTE firstValue = GetCardNewValue(cbFirstCardDataTmp[0]);
	BYTE NextValue = GetCardNewValue(cbNextCardDataTmp[0]);
	BYTE firstColor = GetCardColor(cbFirstCardDataTmp[0]);
	BYTE NextColor = GetCardColor(cbNextCardDataTmp[0]);

	if(firstValue<NextValue)
	{
		return 1;
	}else
	{
		if(firstValue==NextValue)
		{
			if(firstColor<NextColor)
			{
				return 1;

			}else
			{
				return -1;
			}
		}
		return -1;
	}

}

//��ȡ�Ƶ�
BYTE COxBattleLogic::GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount)
{
	//��������
	BYTE cbPipCount=0;

	//��ȡ�Ƶ�
	BYTE cbCardValue=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		cbCardValue=GetCardValue(cbCardData[i]);
		if(cbCardValue>10)
		{
			cbCardValue = 10;

		}
		cbPipCount+=cbCardValue;
	}
	return (cbPipCount%10);
}
BYTE COxBattleLogic::GetCardNewValue(BYTE cbCardData)
{
	//�˿�����
	BYTE cbCardColor=GetCardColor(cbCardData);
	BYTE cbCardValue=GetCardValue(cbCardData);

	//ת����ֵ
	return cbCardValue;

}
//�߼���С
BYTE COxBattleLogic::GetCardLogicValue(BYTE cbCardData)
{
	BYTE cbValue=GetCardValue(cbCardData);

	//��ȡ��ɫ
	BYTE cbColor=GetCardColor(cbCardData);

	if(cbValue>10)
	{
		cbValue = 10;

	}

	return cbValue;
}

//�����˿�
void COxBattleLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType)
{
	//��Ŀ����
	if (cbCardCount==0) return;

	//ת����ֵ
	BYTE cbSortValue[CARD_COUNT];
	if (ST_VALUE==cbSortType)
	{
		for (BYTE i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardValue(cbCardData[i]);	
	}
	else 
	{
		if(cbSortType==ST_NEW)
		{
			for (BYTE i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardNewValue(cbCardData[i]);	

		}else
		{
			for (BYTE i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardLogicValue(cbCardData[i]);	

		}

	}


	//�������
	bool bSorted=true;
	BYTE cbThreeCount,cbLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<cbLast;i++)
		{
			if ((cbSortValue[i]<cbSortValue[i+1])||
				((cbSortValue[i]==cbSortValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
			{
				//����λ��
				cbThreeCount=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbThreeCount;
				cbThreeCount=cbSortValue[i];
				cbSortValue[i]=cbSortValue[i+1];
				cbSortValue[i+1]=cbThreeCount;
				bSorted=false;
			}	
		}
		cbLast--;
	} while(bSorted==false);

	return;
}

//////////////////////////////////////////////////////////////////////////

