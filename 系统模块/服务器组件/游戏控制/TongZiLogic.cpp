#include "StdAfx.h"
#include "TongZiLogic.h"

//////////////////////////////////////////////////////////////////////////

const BYTE CTongZiLogic::m_cbCardListData[TZ_CARD_COUNT] =
{
	    0x10,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,		//0x37�װ�55��1-9Ͳ
		0x10,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,		//0x37�װ�55��1-9Ͳ
		0x10,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,		//0x37�װ�55��1-9Ͳ
		0x10,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,		//0x37�װ�55��1-9Ͳ
		
};
//////////////////////////////////////////////////////////////////////////

//�����˿�
void RandCount(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//����׼��
	BYTE *cbCardData = new BYTE[cbBufferCount];
	CopyMemory(cbCardData,cbCardBuffer,cbBufferCount);

	//�����˿�
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(cbBufferCount-cbRandCount);
		cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
		cbCardData[cbPosition]=cbCardData[cbBufferCount-cbRandCount];
	} while (cbRandCount<cbBufferCount);


	
	delete []cbCardData;
	cbCardData = NULL;

	return;
}


//���캯��
CTongZiLogic::CTongZiLogic()
{
}

//��������
CTongZiLogic::~CTongZiLogic()
{
}

//�����˿�
void CTongZiLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
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


//#ifdef _DEBUG
//	cbCardBuffer[0]=0x01;
//	cbCardBuffer[1]=0x02;
//
//	cbCardBuffer[2]=0x02;
//	cbCardBuffer[3]=0x03;
//
//	cbCardBuffer[4]=0x04;
//	cbCardBuffer[5]=0x04;
//
//	cbCardBuffer[6]=0x05;
//	cbCardBuffer[7]=0x05;
//
//#endif

	return;
}
void CTongZiLogic::GetCardPoint(BYTE bcCardData, CPoint &Point, int CardX, int CardY)
{
	BYTE pbcCardData[]={0x10,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09};
	for (int i = 0;i<10;i++)
	{
		if(bcCardData==pbcCardData[i])
		{
			if(i==0)
			{
				Point.x = CardX*(i-11);
				Point.y =CardY;

			}else
			{
				Point.x =CardX*i;
				Point.y = 0;

			}

			return ;

		}
	}
	Point.x =CardX*10;
	Point.y = 1;

	return ;

}
//��ȡ����
DWORD CTongZiLogic::GetCardType(const BYTE cbCardData[], BYTE cbCardCount)
{

	//Ч�����
	ASSERT(cbCardCount==TZ_MAX_COUNT);
	if(cbCardCount!=TZ_MAX_COUNT)return 0;

	DWORD dwCardType=0;

	
	//����
	if(GetCardValue(cbCardData[0])==GetCardValue(cbCardData[1]))
	{	
		dwCardType=0x30;
		dwCardType |= GetCardValue(cbCardData[0]);
		if (cbCardData[0] == 0x10 || cbCardData[1] == 0x10)
		{
			dwCardType |= 0x0A;
		}
	}
	//��ֵ
	else
	{
		bool bHasBaiBan=false;
		if (cbCardData[0]==0x10||cbCardData[1]==0x10)
		{
			bHasBaiBan = true;
			dwCardType = 0x20;
		}
		else
		{
			dwCardType = 0x10;
		}
		BYTE maxValue=0;
		maxValue=GetCardValue(cbCardData[0])+GetCardValue(cbCardData[1]);
		if (maxValue>=10)
		{
			maxValue -=10; 
		}

		dwCardType |= maxValue;
		
	}

	return dwCardType;
}

//��С�Ƚ�
/*
cbNextCardData>cbFirstCardData  ����1
cbNextCardData<cbFirstCardData  ����-1
cbNextCardData==cbFirstCardData ����0
*/
int CTongZiLogic::CompareCard(const BYTE cbFirstCardData[], BYTE cbFirstCardCount, const BYTE cbNextCardData[], BYTE cbNextCardCount)
{
	//�Ϸ��ж�
	ASSERT(2==cbFirstCardCount && 2==cbNextCardCount);
	if (!(2==cbFirstCardCount && 2==cbNextCardCount)) return 0;

	DWORD dwCardType[2]={GetCardType(cbFirstCardData,2),GetCardType(cbNextCardData,2)};
	
	int nResult=0;

	BYTE cbColorFirst = dwCardType[0] & 0xF0;
	BYTE cbColorNext = dwCardType[1] & 0xF0;

	if ((cbColorFirst == 0x10 && cbColorNext == 0x20) || (cbColorFirst == 0x20 && cbColorNext == 0x10))
	{
		BYTE cbValueFirst = dwCardType[0] & 0x0F;
		BYTE cbValueNext = dwCardType[1] & 0x0F;
		if (cbValueFirst == cbValueNext)
		{
			if (cbColorFirst < cbColorNext)
				nResult = 1;
			else if (cbColorFirst > cbColorNext)
				nResult = -1;
		}
		else
		{
			if (cbValueFirst < cbValueNext)
				nResult = 1;
			else if (cbValueFirst > cbValueNext)
				nResult = -1;
		}
	}
	else
	{
		if (dwCardType[1] > dwCardType[0])
		{
			nResult = 1;
		}
		else if (dwCardType[1] < dwCardType[0])
		{
			nResult = -1;
		}
	}
	
	return nResult;
}

//�߼���С
BYTE CTongZiLogic::GetCardLogicValue(BYTE cbCardData)
{
	//�˿�����
	BYTE bCardValue=GetCardValue(cbCardData);

	//ת����ֵ
	return (bCardValue==0)?(0x10):bCardValue;
}

//�����˿�
void CTongZiLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType)
{
	//��Ŀ����
	if (cbCardCount==0) return;

	//ת����ֵ
	BYTE cbSortValue[TZ_CARD_COUNT];
	if (ST_VALUE==cbSortType)
	{
		for (BYTE i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardValue(cbCardData[i]);	
	}
	else 
	{
		for (BYTE i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardLogicValue(cbCardData[i]);	
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

