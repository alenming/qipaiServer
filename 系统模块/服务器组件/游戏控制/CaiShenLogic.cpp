#include "StdAfx.h"
#include "CaiShenLogic.h"

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//��̬����


/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//���캯��
CCaiShenLogic::CCaiShenLogic()
{
}

//��������
CCaiShenLogic::~CCaiShenLogic()
{
}

//�����˿�
VOID CCaiShenLogic::RandCardList(BYTE cbCardBuffer[][CAISHEN_X_COUNT])
{
	for (int i = 0; i < CAISHEN_Y_COUNT; i++)
	{
		for (int j = 0; j < CAISHEN_X_COUNT; j++)
		{
			cbCardBuffer[i][j] = rand() % (CT_JINLUO + 1);
		}
	}
	return;
}

//ȡ���н�
DOUBLE CCaiShenLogic::GetZhongJiangTime(BYTE cbItemInfo[][CAISHEN_X_COUNT])
{
	DOUBLE nTime = 0;

	BYTE cbZhongJiangCount = 0;
	for (int i = 0; i < CAISHEN_Y_COUNT; i++)
	{
		bool bLeftLink = true;
		int nLeftBaseLinkCount = 1;

		//�����Ϸ���ﲻ����
		if (cbItemInfo[i][0] == CT_JINLUO || cbItemInfo[i][0] == CT_CAISHENDAO)
			continue;

		//�н���
		for (int j = 1; j < CAISHEN_X_COUNT; j++)
		{
			//���һ�����
			bool bFirstRowSame = cbItemInfo[0][j] == cbItemInfo[i][0] || cbItemInfo[0][j] == CT_CAISHENDAO;
			bool bSecondRowSame = cbItemInfo[1][j] == cbItemInfo[i][0] || cbItemInfo[1][j] == CT_CAISHENDAO;
			bool bThirdRowSame = cbItemInfo[2][j] == cbItemInfo[i][0] || cbItemInfo[2][j] == CT_CAISHENDAO;
			if ((bFirstRowSame || bSecondRowSame || bThirdRowSame) && bLeftLink)
			{
				nLeftBaseLinkCount++;
			}
			else
			{
				bLeftLink = false;
			}
		}

		if (nLeftBaseLinkCount == 5)		//ȫ�̽� | ����	
		{
			//�н�����
			switch (cbItemInfo[i][0])
			{
			case CT_GUANYUAN:
			{
				nTime += 12;
				break;
			}
			case CT_HESHANG:
			{
				nTime += 10;
				break;
			}
			case CT_LAOTOU:
			{
				nTime += 10;
				break;
			}
			case CT_JINYU:
			{
				nTime += 8;
				break;
			}
			case CT_FUDAI:
			{
				nTime += 4;
				break;
			}
			case CT_CUIYU:
			{
				nTime += 4;
				break;
			}
			case CT_TAOZI:
			{
				nTime += 4;
				break;
			}
			case CT_BAOZI:
			{
				nTime += 3;
				break;
			}
			case CT_HULU:
			{
				nTime += 3;
				break;
			}
			}
		}
		else
		{
			if (nLeftBaseLinkCount == 3 || nLeftBaseLinkCount == 4)
			{
				//�н�����
				if (cbItemInfo[i][0] == CT_GUANYUAN)
				{
					nTime += nLeftBaseLinkCount == 3 ? 1 : 6;
				}
				else if (cbItemInfo[i][0] == CT_HESHANG)
				{
					nTime += nLeftBaseLinkCount == 3 ? 0.6 : 4;
				}
				else if (cbItemInfo[i][0] == CT_LAOTOU)
				{
					nTime += nLeftBaseLinkCount == 3 ? 0.6 : 4;
				}
				else if (cbItemInfo[i][0] == CT_JINYU)
				{
					nTime += nLeftBaseLinkCount == 3 ? 0.6 : 2;
				}
				else if (cbItemInfo[i][0] == CT_FUDAI)
				{
					nTime += nLeftBaseLinkCount == 3 ? 0.2 : 1.2;
				}
				else if (cbItemInfo[i][0] == CT_CUIYU)
				{
					nTime += nLeftBaseLinkCount == 3 ? 0.2 : 1.2;
				}
				else if (cbItemInfo[i][0] == CT_TAOZI)
				{
					nTime += nLeftBaseLinkCount == 3 ? 0.2 : 0.8;
				}
				else if (cbItemInfo[i][0] == CT_BAOZI)
				{
					nTime += nLeftBaseLinkCount == 3 ? 0.2 : 0.6;
				}
				else if (cbItemInfo[i][0] == CT_HULU)
				{
					nTime += nLeftBaseLinkCount == 3 ? 0.2 : 0.6;
				}
			}
		}
	}

	return nTime;
}

//����С����
BYTE CCaiShenLogic::GetEnterFree(BYTE cbItemInfo[][CAISHEN_X_COUNT])
{
	bool bLeftLink = true;
	int nLeftBaseLinkCount = 0;

	//�н���
	for (int i = 0; i < CAISHEN_X_COUNT; i++)
	{
		//���һ�����
		if ((cbItemInfo[0][i] == CT_JINLUO || cbItemInfo[1][i] == CT_JINLUO || cbItemInfo[2][i] == CT_JINLUO) && bLeftLink)
		{
			nLeftBaseLinkCount++;
		}
		else
		{
			bLeftLink = false;
			break;
		}
	}

	if (nLeftBaseLinkCount >= 3)
	{
		return TRUE;
	}

	return FALSE;
}