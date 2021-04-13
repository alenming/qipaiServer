#include "StdAfx.h"
#include "CaiShenLogic.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//静态变量


/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//构造函数
CCaiShenLogic::CCaiShenLogic()
{
}

//析构函数
CCaiShenLogic::~CCaiShenLogic()
{
}

//混乱扑克
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

//取得中奖
DOUBLE CCaiShenLogic::GetZhongJiangTime(BYTE cbItemInfo[][CAISHEN_X_COUNT])
{
	DOUBLE nTime = 0;

	BYTE cbZhongJiangCount = 0;
	for (int i = 0; i < CAISHEN_Y_COUNT; i++)
	{
		bool bLeftLink = true;
		int nLeftBaseLinkCount = 1;

		//免费游戏这里不处理
		if (cbItemInfo[i][0] == CT_JINLUO || cbItemInfo[i][0] == CT_CAISHENDAO)
			continue;

		//中奖线
		for (int j = 1; j < CAISHEN_X_COUNT; j++)
		{
			//左到右基本奖
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

		if (nLeftBaseLinkCount == 5)		//全盘奖 | 五连	
		{
			//中奖类型
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
				//中奖类型
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

//进入小玛丽
BYTE CCaiShenLogic::GetEnterFree(BYTE cbItemInfo[][CAISHEN_X_COUNT])
{
	bool bLeftLink = true;
	int nLeftBaseLinkCount = 0;

	//中奖线
	for (int i = 0; i < CAISHEN_X_COUNT; i++)
	{
		//左到右基本奖
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