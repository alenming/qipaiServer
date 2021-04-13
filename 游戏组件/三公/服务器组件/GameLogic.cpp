#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//�˿�����
BYTE CGameLogic::m_cbCardListData[52]=
{
  0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D, //���� A - K
  0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D, //÷�� A - K
  0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D, //���� A - K
  0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D  //���� A - K
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
  ASSERT(cbCardCount == MAX_COUNT);
  if (cbCardData[0] == 0)
	  return 0;

  BYTE KingCount      = 0;
  BYTE bTemp[MAX_COUNT] = { 0 };
  BYTE bSum = 0;
  for (BYTE i = 0; i<cbCardCount; i++)
  {
	  BOOL isHuaCard = FALSE;
	  bTemp[i] = GetCardLogicValue(cbCardData[i], isHuaCard);
	  bSum += bTemp[i];

	  if (isHuaCard)
		  KingCount++;
  }

  BYTE cbValue = bSum % 10;

  if (cbValue > 0)
  {
	  if ((GetCardValue(cbCardData[0]) == GetCardValue(cbCardData[1])) && (GetCardValue(cbCardData[0]) == GetCardValue(cbCardData[2])) )
		  return SG_THREEE_SAME;

	  return cbValue;
  }

  if (MAX_COUNT == KingCount && (GetCardValue(cbCardData[0]) == GetCardValue(cbCardData[1])) && (GetCardValue(cbCardData[0]) == GetCardValue(cbCardData[2])))
	  return SG_THREEE_SAME;

  if (MAX_COUNT == KingCount)
	  return SG_THREE_KING;

	return 0;
}

//�����˿�
void CGameLogic::GetDebugCards(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	ZeroMemory(cbCardBuffer, cbBufferCount);

	BYTE testbuffer[] = { 0x05, 0x15, 0x25};

	CopyMemory(cbCardBuffer, testbuffer, 3);
}

//��ȡ����
BYTE CGameLogic::GetTimes(BYTE cbCardData[], BYTE cbCardCount)
{
	if(cbCardCount!=MAX_COUNT)return 0;

	BYTE bTimes = GetCardType(cbCardData,MAX_COUNT);
	if( bTimes <=7 )		return 1;
	else if (bTimes == 8)	return 2;
	else if (bTimes == 9)	return 3;
	else if (bTimes == 10)  return 4;
	else if (bTimes == 11)  return 5;

  return 0;
}



//��ȡ����
bool CGameLogic::IsIntValue(BYTE cbCardData[], BYTE cbCardCount)
{
	return true;
}

//�����˿�
void CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount)
{
  //ת����ֵ
  BYTE cbLogicValue[MAX_COUNT] = { 0 };
  for(BYTE i=0; i<cbCardCount; i++)
  {
    cbLogicValue[i] = GetCardValue(cbCardData[i]);
  }

  //�������
  bool bSorted=true;
  BYTE cbTempData,bLast=cbCardCount-1;
  do
  {
    bSorted=true;
    for(BYTE i=0; i<bLast; i++)
    {
      if( (cbLogicValue[i]<cbLogicValue[i+1])||
         ((cbLogicValue[i]==cbLogicValue[i+1]) && (cbCardData[i]<cbCardData[i+1])) )
      {
        //����λ��
        cbTempData=cbCardData[i];
        cbCardData[i]=cbCardData[i+1];
        cbCardData[i+1]=cbTempData;

        cbTempData=cbLogicValue[i];
        cbLogicValue[i]=cbLogicValue[i+1];
        cbLogicValue[i+1]=cbTempData;
        bSorted=false;
      }
    }

    bLast--;
  }
  while(bSorted==false);

  return;
}

//�����˿�
void CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{

  //����׼��
  BYTE cbCardData[CountArray(m_cbCardListData)];
  CopyMemory(cbCardData,m_cbCardListData,sizeof(m_cbCardListData));

  //�����˿�
  BYTE bRandCount=0,bPosition=0;
  do
  {
    bPosition=rand()%(CountArray(m_cbCardListData)-bRandCount);
    cbCardBuffer[bRandCount++]=cbCardData[bPosition];
    cbCardData[bPosition]=cbCardData[CountArray(m_cbCardListData)-bRandCount];
  }
  while(bRandCount<cbBufferCount);

  return;
}

/*
	�ӿ����repoCards�������Ʒŵ�outCards
	������ƴӿ���Ƴ�
*/
int CGameLogic::GetCards(BYTE outCards[], BYTE outLen, BYTE repoCardsPrime[], BYTE repoLen, BYTE cCards[], BYTE cLen)
{
	BYTE repoCards[CountArray(m_cbCardListData)];
	CopyMemory(repoCards, repoCardsPrime, repoLen);
	if (cLen > outLen || outLen > repoLen)
	{
		return 0;
	}

	//repoCards��ҵ����е�cCards,�ƶ�����ʼλ��
	for (int i = 0; i < cLen; i++)
	{
		int j = i;
		for (; j < repoLen; j++)
		{
			if (repoCards[j] == cCards[i])
			{
				//���ҵ����ƽ����ŵ��ʼ�ĵط�
				BYTE tmp = repoCards[j];
				repoCards[j] = repoCards[i];
				repoCards[i] = tmp;
				break;
			}
		}
		//��ָ������û�ҵ�
		if (j == repoLen)
		{
			return 0;
		}
	}


	//��cLen<outLen, �������������
	BYTE bRandCount = cLen, bPosition = 0;
	while (bRandCount<outLen)
	{
		bPosition = rand() % (repoLen - bRandCount);
		BYTE tmp = repoCards[bRandCount];
		repoCards[bRandCount] = repoCards[bRandCount + bPosition];
		repoCards[bRandCount + bPosition] = tmp;
		bRandCount++;
	}
	//���Ƶ�outCards
	CopyMemory(outCards, repoCards, outLen);

	//ԭ������Ƴ�������
	if (repoLen > outLen)
	{
		CopyMemory(repoCardsPrime, &repoCards[outLen], (repoLen - outLen));
	}

	ZeroMemory(&repoCardsPrime[repoLen - outLen], outLen);

	return outLen;
}

//�߼���ֵ
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData, BOOL & isKing)
{
  //�˿�����
  BYTE bCardColor=GetCardColor(cbCardData);
  BYTE bCardValue=GetCardValue(cbCardData);

  if (bCardValue > 10)
  {
	  bCardValue = 10;
	  isKing     = TRUE;
  }

  return  bCardValue;
}


//�Ա��˿�
bool CGameLogic::CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount)
{

	//�Ƚ�ţ��С
	//��ȡ����
	BYTE cbNextType  = GetCardType(cbNextData, cbCardCount);
	BYTE cbFirstType = GetCardType(cbFirstData, cbCardCount);

	//�����ж�
	if (cbFirstType != cbNextType)
	{
		return (cbFirstType > cbNextType);
	}

	//�����С
	BYTE bFirstTemp[MAX_COUNT], bNextTemp[MAX_COUNT];
	CopyMemory(bFirstTemp, cbFirstData, cbCardCount);
	CopyMemory(bNextTemp, cbNextData, cbCardCount);
	SortCardList(bFirstTemp, cbCardCount);
	SortCardList(bNextTemp, cbCardCount);

	//�Ƚ���ֵ
	BYTE cbNextMaxValue  = GetCardValue(bNextTemp[0]);
	BYTE cbFirstMaxValue = GetCardValue(bFirstTemp[0]);
	if (cbNextMaxValue != cbFirstMaxValue)
	{
		return cbFirstMaxValue > cbNextMaxValue;
	}

	//�Ƚ���ɫ
	return GetCardColor(bFirstTemp[0]) > GetCardColor(bNextTemp[0]);

}

VOID CGameLogic::SortPlayerValue(BYTE cbPlayerValue[], BYTE cbCount)
{
	//�������
	bool bSorted = true;
	BYTE cbTempData, bLast = cbCount - 1;
	do
	{
		bSorted = true;
		for (BYTE i = 0; i<bLast; i++)
		{
			if (cbPlayerValue[i] <= cbPlayerValue[i + 1])
			{
				//����λ��
				cbTempData = cbPlayerValue[i];
				cbPlayerValue[i] = cbPlayerValue[i + 1];
				cbPlayerValue[i + 1] = cbTempData;

				bSorted = false;
			}
		}

		bLast--;
	} while (bSorted == false);

	return;
}

//��ȡ��Ҵ�С����
VOID CGameLogic::GetPlayerOrder(CONST BYTE cbHandCard[], CONST BYTE cbPlayerValue[], BYTE cbPlayerOrder[], BYTE cbCount)
{
	BYTE cbCard[GAME_PLAYER][MAX_COUNT] = { 0 };
	CopyMemory(cbCard, cbHandCard, GAME_PLAYER * MAX_COUNT);

	//ÿ���û��ƽ�������
	for (WORD i = 0; i < cbCount; i++)
	{
		if (cbCard[i][0] == 0) continue;

		SortCardList(cbCard[i], MAX_COUNT);
	}

	//�������
	BYTE cbValue[GAME_PLAYER] = { 0 };
	CopyMemory(cbValue, cbPlayerValue, cbCount);
	//SortPlayerValue(cbValue, GAME_PLAYER);

	for (WORD i = 0; i < cbCount; i++)
	{
		for (WORD j = i + 1; j < cbCount; j++)
		{
			if (cbValue[j] > cbValue[i])
			{
				BYTE cbTempValue = cbValue[i];
				cbValue[i] = cbValue[j];
				cbValue[j] = cbTempValue;

				WORD wTempUser = cbPlayerOrder[i];
				cbPlayerOrder[i] = cbPlayerOrder[j];
				cbPlayerOrder[j] = wTempUser;
			}
		}
	}

	//WORD wCount      = 0;
	//CHAR cbTempValue = -1;

	//for (WORD i = 0; i < cbCount; i++)
	//{
	//	BYTE cbCardSort[GAME_PLAYER] = { 0 };
	//	WORD wSameCount = 0;

	//	if (cbTempValue == cbValue[i])
	//		continue;

	//	for (WORD j = 0; j < cbCount; j++)
	//	{
	//		if (cbCard[j][0] == 0) continue;

	//		BYTE cbUserValue = GetCardType(cbCard[j], MAX_COUNT);
	//		if (cbValue[i] == cbUserValue)
	//		{
	//			cbCardSort[wSameCount++] = cbCard[j][0];
	//		}
	//			
	//	}

	//	if (wSameCount > 1)
	//	{
	//		SortCardList(cbCardSort, wSameCount);
	//		for (WORD m = 0; m < wSameCount; m++)
	//		{
	//			for (WORD j = 0; j < cbCount; j++)
	//			{
	//				if (cbCardSort[m] == cbCard[j][0])
	//					cbPlayerOrder[wCount++] = j;
	//			}
	//		}
	//	}
	//	else if (wSameCount == 1)
	//	{
	//		for (WORD j = 0; j < cbCount; j++)
	//		{
	//			if (cbCardSort[0] == cbCard[j][0])
	//				cbPlayerOrder[wCount++] = j;
	//		}
	//	}

	//	cbTempValue = cbValue[i];

	//}

}

//////////////////////////////////////////////////////////////////////////
