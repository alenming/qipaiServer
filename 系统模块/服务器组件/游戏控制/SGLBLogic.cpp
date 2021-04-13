#include "StdAfx.h"
#include "SGLBLogic.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//静态变量
#define CT_APPLE					1									//苹果
#define CT_MANGO					2									//芒果
#define CT_ORANGE					3									//橘子
#define CT_GRAPE					4									//葡萄
#define CT_WATERMELON				5									//西瓜
#define CT_CHERRY					6									//樱桃
#define CT_BELL						7									//铃铛
#define CT_DOUBLESEVEN				8									//双七
#define CT_BAR						9									//Bar
#define CT_WILD						10									//WILD
#define CT_BONUS					11									//BONUS
#define CT_SCATTER					12									//SCATTER(免费转)
#define CT_JACKPOT					13									//JACKPOT
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//构造函数
CSGLBLogic::CSGLBLogic()
{
	m_cbLineInfo[0][0] = 1;
	m_cbLineInfo[0][1] = 1;
	m_cbLineInfo[0][2] = 1;
	m_cbLineInfo[0][3] = 1;
	m_cbLineInfo[0][4] = 1;

	m_cbLineInfo[1][0] = 0;
	m_cbLineInfo[1][1] = 0;
	m_cbLineInfo[1][2] = 0;
	m_cbLineInfo[1][3] = 0;
	m_cbLineInfo[1][4] = 0;

	m_cbLineInfo[2][0] = 2;
	m_cbLineInfo[2][1] = 2;
	m_cbLineInfo[2][2] = 2;
	m_cbLineInfo[2][3] = 2;
	m_cbLineInfo[2][4] = 2;

	m_cbLineInfo[3][0] = 0;
	m_cbLineInfo[3][1] = 1;
	m_cbLineInfo[3][2] = 2;
	m_cbLineInfo[3][3] = 1;
	m_cbLineInfo[3][4] = 0;

	m_cbLineInfo[4][0] = 2;
	m_cbLineInfo[4][1] = 1;
	m_cbLineInfo[4][2] = 0;
	m_cbLineInfo[4][3] = 1;
	m_cbLineInfo[4][4] = 2;

	m_cbLineInfo[5][0] = 1;
	m_cbLineInfo[5][1] = 0;
	m_cbLineInfo[5][2] = 0;
	m_cbLineInfo[5][3] = 0;
	m_cbLineInfo[5][4] = 1;

	m_cbLineInfo[6][0] = 1;
	m_cbLineInfo[6][1] = 2;
	m_cbLineInfo[6][2] = 2;
	m_cbLineInfo[6][3] = 2;
	m_cbLineInfo[6][4] = 1;

	m_cbLineInfo[7][0] = 0;
	m_cbLineInfo[7][1] = 0;
	m_cbLineInfo[7][2] = 1;
	m_cbLineInfo[7][3] = 2;
	m_cbLineInfo[7][4] = 2;

	m_cbLineInfo[8][0] = 2;
	m_cbLineInfo[8][1] = 2;
	m_cbLineInfo[8][2] = 1;
	m_cbLineInfo[8][3] = 0;
	m_cbLineInfo[8][4] = 0;

	m_nIndex = 0;
}

//析构函数
CSGLBLogic::~CSGLBLogic()
{
}

//混乱扑克
VOID CSGLBLogic::RandCardList(BYTE cbCardBuffer[][SGLB_X_COUNT])
{
	for (int i = 0; i < SGLB_Y_COUNT; i++)
	{
		for (int j = 0; j < SGLB_X_COUNT; j++)
		{
			cbCardBuffer[i][j] = rand() % (CT_JACKPOT) + 1;
		}
	}
	return;
}

//取得中奖
int CSGLBLogic::GetZhongJiangTime(BYTE cbItemInfo[][SGLB_X_COUNT])
{
	DOUBLE nTime = 0;

	BYTE cbZhongJiangCount = 0;

	for (int i = 0; i < LINE_COUNT; i++)
	{
		BYTE cbLeftLinkCount = 1;

		//先判断前面的Wild元素，可以与除了三个特殊图标的任意元素搭配
		BYTE cbWildLinkCount = 0;
		for (int j = 0; j < SGLB_X_COUNT; j++)
		{
			BYTE cbIndex = m_cbLineInfo[i][j];

			if (cbItemInfo[cbIndex][j] == CT_WILD)
				cbWildLinkCount++;
			else
				break;
		}

		cbLeftLinkCount += cbWildLinkCount;

		if (cbWildLinkCount == SGLB_X_COUNT)
			cbLeftLinkCount == SGLB_X_COUNT;

		if ((cbWildLinkCount + 1) < SGLB_X_COUNT)
		{
			for (int j = cbWildLinkCount + 1; j < SGLB_X_COUNT; j++)
			{
				BYTE cbIndex = m_cbLineInfo[i][j];
				BYTE cbFirstIndex = m_cbLineInfo[i][cbWildLinkCount];

				if (cbItemInfo[cbIndex][j] == cbItemInfo[cbFirstIndex][cbWildLinkCount])
				{
					cbLeftLinkCount++;

				}
				else
					break;
			}
		}

		BYTE cbItemIndex = cbItemInfo[m_cbLineInfo[i][0]][0];

		if (cbWildLinkCount > 0 && cbWildLinkCount < SGLB_X_COUNT)
		{
			cbItemIndex = cbItemInfo[m_cbLineInfo[i][cbWildLinkCount]][cbWildLinkCount];
		}

		BYTE cbLinkCount = cbLeftLinkCount;

		if (cbWildLinkCount == 2)
			nTime += 10;
		else if (cbWildLinkCount == 3)
			nTime += 100;
		else if (cbWildLinkCount == 4)
			nTime += 500;
		else if (cbWildLinkCount == 5)
			nTime += 1000;

		if (cbLinkCount == 5)
		{
			//中奖类型
			switch (cbItemIndex)
			{
				case CT_APPLE:
				{
					nTime += 20;
					break;
				}
				case CT_MANGO:
				{
					nTime += 40;
					break;
				}
				case CT_ORANGE:
				{
					nTime += 60;
					break;
				}
				case CT_GRAPE:
				{
					nTime += 80;
					break;
				}
				case CT_WATERMELON:
				{
					nTime += 100;
					break;
				}
				case CT_CHERRY:
				{
					nTime += 150;
					break;
				}
				case CT_BELL:
				{
					nTime += 200;
					break;
				}
				case CT_DOUBLESEVEN:
				{
					nTime += 300;
					break;
				}
				case CT_BAR:
				{
					nTime += 600;
					break;
				}
			}
		}
		else
		{
			if (cbLinkCount == 2)
			{
				if (cbItemIndex == CT_CHERRY)
				{
					nTime += 3;
				}
				else if (cbItemIndex == CT_BELL)
				{
					nTime += 3;
				}
				else if (cbItemIndex == CT_DOUBLESEVEN)
				{
					nTime += 4;
				}
				else if (cbItemIndex == CT_BAR)
				{
					nTime += 5;
				}
			}
			else if (cbLinkCount == 3 || cbLinkCount == 4)
			{
				//中奖类型
				if (cbItemIndex == CT_APPLE)
				{
					nTime += cbLinkCount == 3 ? 5 : 10;
				}
				else if (cbItemIndex == CT_MANGO)
				{
					nTime += cbLinkCount == 3 ? 10 : 20;
				}
				else if (cbItemIndex == CT_ORANGE)
				{
					nTime += cbLinkCount == 3 ? 15 : 30;
				}
				else if (cbItemIndex == CT_GRAPE)
				{
					nTime += cbLinkCount == 3 ? 20 : 40;
				}
				else if (cbItemIndex == CT_WATERMELON)
				{
					nTime += cbLinkCount == 3 ? 25 : 50;
				}
				else if (cbItemIndex == CT_CHERRY)
				{
					nTime += cbLinkCount == 3 ? 30 : 60;
				}
				else if (cbItemIndex == CT_BELL)
				{
					nTime += cbLinkCount == 3 ? 40 : 80;
				}
				else if (cbItemIndex == CT_DOUBLESEVEN)
				{
					nTime += cbLinkCount == 3 ? 50 : 100;
				}
				else if (cbItemIndex == CT_BAR)
				{
					nTime += cbLinkCount == 3 ? 60 : 150;
				}
			}
		}
	}

	return nTime;
}

//进入小玛丽
BYTE CSGLBLogic::GetEnterThree(BYTE cbItemInfo[][SGLB_X_COUNT])
{
	BYTE nTime = 0;

	for (int i = 0; i < LINE_COUNT; i++)
	{
		BYTE cbLeftLinkCount = 1;
		for (int j = 1; j < SGLB_X_COUNT; j++)
		{
			BYTE cbIndex = m_cbLineInfo[i][j];
			BYTE cbFirstIndex = m_cbLineInfo[i][0];

			if (cbItemInfo[cbFirstIndex][0] != CT_BONUS)
				break;

			if (cbItemInfo[cbIndex][j] == cbItemInfo[cbFirstIndex][0])
				cbLeftLinkCount++;
			else
				break;
		}

		if (cbLeftLinkCount >= 1)
		{
			nTime = 1;
		}
	}

	return nTime;
}

//获取免费次数
BYTE CSGLBLogic::GetEnterFree(BYTE cbItemInfo[][SGLB_X_COUNT])
{
	BYTE nTime = 0;

	for (int i = 0; i < LINE_COUNT; i++)
	{
		BYTE cbLeftLinkCount = 1;
		for (int j = 1; j < SGLB_X_COUNT; j++)
		{
			BYTE cbIndex = m_cbLineInfo[i][j];
			BYTE cbFirstIndex = m_cbLineInfo[i][0];

			if (cbItemInfo[cbFirstIndex][0] != CT_SCATTER)
				break;

			if (cbItemInfo[cbIndex][j] == cbItemInfo[cbFirstIndex][0])
				cbLeftLinkCount++;
			else
				break;
		}

		if (cbLeftLinkCount >= 3)
		{
			nTime = 5;
		}
	}

	return nTime;
}

//获取JackPot分数
SCORE CSGLBLogic::GetJackPotScore(BYTE cbItemInfo[][SGLB_X_COUNT], SCORE lJackPotScore)
{
	SCORE nTime = 0;

	for (int i = 0; i < LINE_COUNT; i++)
	{
		BYTE cbLeftLinkCount = 1;
		for (int j = 1; j < SGLB_X_COUNT; j++)
		{
			BYTE cbIndex = m_cbLineInfo[i][j];
			BYTE cbFirstIndex = m_cbLineInfo[i][0];

			if (cbItemInfo[cbFirstIndex][0] != CT_JACKPOT)
				break;

			if (cbItemInfo[cbIndex][j] == cbItemInfo[cbFirstIndex][0])
				cbLeftLinkCount++;
			else
				break;
		}

		if (cbLeftLinkCount == 3)
		{
			nTime = 0.1;
		}
		else if (cbLeftLinkCount == 4)
		{
			nTime = 0.3;
		}
		else if (cbLeftLinkCount == 5)
		{
			nTime = 0.5;
		}
	}

	return nTime * lJackPotScore;
}