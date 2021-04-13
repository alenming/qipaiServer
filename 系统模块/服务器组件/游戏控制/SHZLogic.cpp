#include "StdAfx.h"
#include "SHZLogic.h"

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//��̬����

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//���캯��
CSHZLogic::CSHZLogic()
{
}

//��������
CSHZLogic::~CSHZLogic()
{
}

//�����˿�
VOID CSHZLogic::RandCardList(BYTE cbCardBuffer[][SHZ_X_COUNT])
{
	for (int i = 0; i < SHZ_Y_COUNT; i++)
	{
		for (int j = 0; j < SHZ_X_COUNT; j++)
		{
			cbCardBuffer[i][j] = rand() % (CT_SHUIHUZHUAN + 1);
		}
	}

	while (GetEnterThree(cbCardBuffer) > 0)
	{
		for (int i = 0; i < SHZ_Y_COUNT; i++)
		{
			for (int j = 0; j < SHZ_X_COUNT; j++)
			{
				cbCardBuffer[i][j] = rand() % (CT_SHUIHUZHUAN + 1);
			}
		}
	}

	return;
}

//ȡ���н�
int CSHZLogic::GetZhongJiangTime(BYTE cbItemInfo[][SHZ_X_COUNT])
{
	DOUBLE nTime = 0;

	BYTE cbZhongJiangCount = 0;
	for (int i = 0; i < SHZ_Y_COUNT; i++)
	{
		bool bLeftLink = true;
		int nLeftBaseLinkCount = 1;

		//�����Ϸ���ﲻ����
		if (cbItemInfo[i][0] == CT_SHUIHUZHUAN)
			continue;

		//�н���
		for (int j = 1; j < SHZ_X_COUNT; j++)
		{
			//���һ�����
			bool bFirstRowSame = cbItemInfo[0][j] == cbItemInfo[i][0];
			bool bSecondRowSame = cbItemInfo[1][j] == cbItemInfo[i][0];
			bool bThirdRowSame = cbItemInfo[2][j] == cbItemInfo[i][0];
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
				case CT_FUTOU:
				{
					nTime += 20;
					break;
				}
				case CT_YINGQIANG:
				{
					nTime += 40;
					break;
				}
				case CT_DADAO:
				{
					nTime += 60;
					break;
				}
				case CT_LU:
				{
					nTime += 100;
					break;
				}
				case CT_LIN:
				{
					nTime += 160;
					break;
				}
				case CT_SONG:
				{
					nTime += 200;
					break;
				}
				case CT_TITIANXINGDAO:
				{
					nTime += 400;
					break;
				}
				case CT_ZHONGYITANG:
				{
					nTime += 1000;
					break;
				}
			}
		}
		else
		{
			if (nLeftBaseLinkCount == 3 || nLeftBaseLinkCount == 4)
			{
				//�н�����
				if (cbItemInfo[i][0] == CT_FUTOU)
				{
					nTime += nLeftBaseLinkCount == 3 ? 2 : 5;
				}
				else if (cbItemInfo[i][0] == CT_YINGQIANG)
				{
					nTime += nLeftBaseLinkCount == 3 ? 3 : 10;
				}
				else if (cbItemInfo[i][0] == CT_DADAO)
				{
					nTime += nLeftBaseLinkCount == 3 ? 5 : 15;
				}
				else if (cbItemInfo[i][0] == CT_LU)
				{
					nTime += nLeftBaseLinkCount == 3 ? 7 : 20;
				}
				else if (cbItemInfo[i][0] == CT_LIN)
				{
					nTime += nLeftBaseLinkCount == 3 ? 10 : 30;
				}
				else if (cbItemInfo[i][0] == CT_SONG)
				{
					nTime += nLeftBaseLinkCount == 3 ? 15 : 40;
				}
				else if (cbItemInfo[i][0] == CT_TITIANXINGDAO)
				{
					nTime += nLeftBaseLinkCount == 3 ? 20 : 80;
				}
				else if (cbItemInfo[i][0] == CT_ZHONGYITANG)
				{
					nTime += nLeftBaseLinkCount == 3 ? 50 : 200;
				}
			}
		}
	}

	//ȫ����
	nTime += GetQuanPanJiangTime(cbItemInfo);

	//ȫ�����ȫ����
	nTime += GetQuanRenJiangTime(cbItemInfo);
	nTime += GetQuanBingQiJiangTime(cbItemInfo);

	return nTime;
}

//ȫ�̽�
int CSHZLogic::GetQuanPanJiangTime(BYTE cbItemInfo[][SHZ_X_COUNT])
{
	int nTime = 0;

	for (int i = 0; i < SHZ_Y_COUNT; i++)
	{
		for (int j = 0; j < SHZ_X_COUNT; j++)
		{
			if (cbItemInfo[i][0] != cbItemInfo[i][j])
			{
				return 0;
			}
		}
	}

	switch (cbItemInfo[0][0])
	{
		case CT_FUTOU:
			{
				nTime = 50;
				break;
			}
		case CT_YINGQIANG:
			{
				nTime = 100;
				break;
			}
		case CT_DADAO:
			{
				nTime = 150;
				break;
			}
		case CT_LU:
			{
				nTime = 250;
				break;
			}
		case CT_LIN:
			{
				nTime = 400;
				break;
			}
		case CT_SONG:
			{
				nTime = 500;
				break;
			}
		case CT_TITIANXINGDAO:
			{
				nTime = 1000;
				break;
			}
		case CT_ZHONGYITANG:
			{
				nTime = 2500;
				break;
			}
		default:
			{
				return 0;
			}
	}

	return nTime;
}

//ȫ����
int CSHZLogic::GetQuanRenJiangTime(BYTE cbItemInfo[][SHZ_X_COUNT])
{
	for (int i = 0; i < SHZ_Y_COUNT; i++)
	{
		for (int j = 0; j < SHZ_X_COUNT; j++)
		{
			if (cbItemInfo[i][j] != CT_SONG && cbItemInfo[i][j] != CT_LIN && cbItemInfo[i][j] != CT_LU)
			{
				return 0;
			}
		}
	}

	return 50;
}

//ȫ����
int CSHZLogic::GetQuanBingQiJiangTime(BYTE cbItemInfo[][SHZ_X_COUNT])
{
	for (int i = 0; i < SHZ_Y_COUNT; i++)
	{
		for (int j = 0; j < SHZ_X_COUNT; j++)
		{
			if (cbItemInfo[i][j] != CT_DADAO && cbItemInfo[i][j] != CT_YINGQIANG && cbItemInfo[i][j] != CT_FUTOU)
			{
				return 0;
			}
		}
	}

	return 15;
}

//����С����
BYTE CSHZLogic::GetEnterThree(BYTE cbItemInfo[][SHZ_X_COUNT])
{
	bool bLeftLink = true;
	int nLeftBaseLinkCount = 0;

	//�н���
	for (int i = 0; i < SHZ_X_COUNT; i++)
	{
		//���һ�����
		if ((cbItemInfo[0][i] == CT_SHUIHUZHUAN || cbItemInfo[1][i] == CT_SHUIHUZHUAN || cbItemInfo[2][i] == CT_SHUIHUZHUAN) && bLeftLink)
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