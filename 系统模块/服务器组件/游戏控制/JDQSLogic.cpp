#include "StdAfx.h"
#include "JDQSLogic.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//构造函数
CJDQSLogic::CJDQSLogic()
{
	m_nCount = 0;
}

//析构函数
CJDQSLogic::~CJDQSLogic()
{
}


//混乱扑克
VOID CJDQSLogic::RandCardList(BYTE cbCardBuffer[][JDQS_X_COUNT])
{
	//随机一组牌
	for (int i = 0; i < JDQS_Y_COUNT; i++)
	{
		for (int j = 0; j < JDQS_X_COUNT; j++)
		{
			cbCardBuffer[i][j] = rand() % (CT_FREE + 1);
			if (cbCardBuffer[i][j] == 9)
			{
				while (true)
				{
					cbCardBuffer[i][j] = rand() % (CT_FREE + 1);
					if (cbCardBuffer[i][j] != 9)
						break;
				}
			}
		}
	}

	//枪的特殊判断  如果第二行是枪，那么第三行也要是它
	for (int i = 0; i < JDQS_X_COUNT; i++)
	{
		if (cbCardBuffer[1][i] == CT_SCAR || cbCardBuffer[1][i] == CT_98K)
		{
			cbCardBuffer[2][i] = cbCardBuffer[1][i];
		}
	}

	if (m_nCount == 0)
	{
		//cbCardBuffer[0][0] = CT_FREE;
		//cbCardBuffer[0][1] = CT_FREE;
		//cbCardBuffer[0][2] = CT_FREE;
	}
	return;
}

//取得中奖
DOUBLE CJDQSLogic::GetZhongJiangTime(BYTE cbItemInfo[][JDQS_X_COUNT])
{
	DOUBLE nTime = 0;

	BYTE cbZhongJiangCount = 0;
	for (int i = 0; i < JDQS_Y_COUNT; i++)
	{
		bool bLeftLink = true;
		int nLeftBaseLinkCount = 1;

		//免费游戏这里不处理
		if (cbItemInfo[i][0] == CT_FREE || cbItemInfo[i][0] == CT_JUEDI)
			continue;

		//中奖线
		for (int j = 1; j < JDQS_X_COUNT; j++)
		{
			//左到右基本奖
			bool bFirstRowSame = cbItemInfo[0][j] == cbItemInfo[i][0] || cbItemInfo[0][j] == CT_JUEDI;
			bool bSecondRowSame = cbItemInfo[1][j] == cbItemInfo[i][0] || cbItemInfo[1][j] == CT_JUEDI;
			bool bThirdRowSame = cbItemInfo[2][j] == cbItemInfo[i][0] || cbItemInfo[2][j] == CT_JUEDI;
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
				case CT_98K:
				{
					nTime += 10;
					break;
				}
				case CT_SCAR:
				{
					nTime += 8;
					break;
				}
				case CT_XIANG:
				{
					nTime += 6;
					break;
				}
				case CT_GUO:
				{
					nTime += 5;
					break;
				}
				case CT_A:
				{
					nTime += 2.5;
					break;
				}
				case CT_K:
				{
					nTime += 2.5;
					break;
				}
				case CT_Q:
				{
					nTime += 1.5;
					break;
				}
				case CT_J:
				{
					nTime += 1.5;
					break;
				}
			}
		}
		else
		{
			if (nLeftBaseLinkCount == 3 || nLeftBaseLinkCount == 4)
			{
				//中奖类型
				if (cbItemInfo[i][0] == CT_98K)
				{
					nTime += nLeftBaseLinkCount == 3 ? 1 : 2.5;
				}
				else if (cbItemInfo[i][0] == CT_SCAR)
				{
					nTime += nLeftBaseLinkCount == 3 ? 1 : 2.5;
				}
				else if (cbItemInfo[i][0] == CT_XIANG)
				{
					nTime += nLeftBaseLinkCount == 3 ? 0.5 : 1.5;
				}
				else if (cbItemInfo[i][0] == CT_GUO)
				{
					nTime += nLeftBaseLinkCount == 3 ? 0.5 : 1.5;
				}
				else if (cbItemInfo[i][0] == CT_A)
				{
					nTime += nLeftBaseLinkCount == 3 ? 0.3 : 1;
				}
				else if (cbItemInfo[i][0] == CT_K)
				{
					nTime += nLeftBaseLinkCount == 3 ? 0.3 : 1;
				}
				else if (cbItemInfo[i][0] == CT_Q)
				{
					nTime += nLeftBaseLinkCount == 3 ? 0.2 : 0.5;
				}
				else if (cbItemInfo[i][0] == CT_J)
				{
					nTime += nLeftBaseLinkCount == 3 ? 0.2 : 0.5;
				}
			}
		}
	}

	return nTime;
}


//进入小玛丽
BYTE CJDQSLogic::GetEnterFree(BYTE cbItemInfo[][JDQS_X_COUNT])
{	
	bool bLeftLink = true;
	int nLeftBaseLinkCount = 0;

	//中奖线
	for (int i = 0; i < JDQS_X_COUNT; i++)
	{
		//左到右基本奖
		if ((cbItemInfo[0][i] == CT_FREE || cbItemInfo[1][i] == CT_FREE || cbItemInfo[2][i] == CT_FREE) && bLeftLink)
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

