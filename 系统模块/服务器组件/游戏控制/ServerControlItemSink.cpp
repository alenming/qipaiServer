#include "StdAfx.h"
#include "servercontrolitemsink.h"

//////////////////////////////////////////////////////////////////////////

//
CServerControlItemSink::CServerControlItemSink(void)
{
	enum BetAreas
	{
		STRAIGHT_0,
		STRAIGHT_1,
		STRAIGHT_2,
		STRAIGHT_3,
		STRAIGHT_4,
		STRAIGHT_5,
		STRAIGHT_6,
		STRAIGHT_7,
		STRAIGHT_8,
		STRAIGHT_9,
		STRAIGHT_10,
		STRAIGHT_11,
		STRAIGHT_12,
		STRAIGHT_13,
		STRAIGHT_14,
		STRAIGHT_15,
		STRAIGHT_16,
		STRAIGHT_17,
		STRAIGHT_18,
		STRAIGHT_19,
		STRAIGHT_20,
		STRAIGHT_21,
		STRAIGHT_22,
		STRAIGHT_23,
		STRAIGHT_24,
		STRAIGHT_25,
		STRAIGHT_26,
		STRAIGHT_27,
		STRAIGHT_28,
		STRAIGHT_29,
		STRAIGHT_30,
		STRAIGHT_31,
		STRAIGHT_32,
		STRAIGHT_33,
		STRAIGHT_34,
		STRAIGHT_35,
		STRAIGHT_36,
		COLUMN_1,
		COLUMN_2,
		COLUMN_3,
		DOZEN_1,
		DOZEN_2,
		DOZEN_3,
		LOW_1,
		EVEN_1,
		RED_1,
		BLACK_1,
		ODD_1,
		HI_1,
		BET_AREA_COUNT
	};

	for (int i = STRAIGHT_0; i <= STRAIGHT_36; i++)
	{
		m_nLPAreaNums[i][0] = i;
		for (int j = 1; j < 18; j++)
		{
			m_nLPAreaNums[i][j] = 255;
		}
	}

	for (int i = COLUMN_1; i <= COLUMN_3; i++)
	{
		m_nLPAreaNums[i][0] = (i - COLUMN_1 + 1);
		m_nLPAreaNums[i][1] = (i - COLUMN_1 + 1) + 3;
		m_nLPAreaNums[i][2] = (i - COLUMN_1 + 1) + 6;
		m_nLPAreaNums[i][3] = (i - COLUMN_1 + 1) + 9;
		m_nLPAreaNums[i][4] = (i - COLUMN_1 + 1) + 12;
		m_nLPAreaNums[i][5] = (i - COLUMN_1 + 1) + 15;
		m_nLPAreaNums[i][6] = (i - COLUMN_1 + 1) + 18;
		m_nLPAreaNums[i][7] = (i - COLUMN_1 + 1) + 21;
		m_nLPAreaNums[i][8] = (i - COLUMN_1 + 1) + 24;
		m_nLPAreaNums[i][9] = (i - COLUMN_1 + 1) + 27;
		m_nLPAreaNums[i][10] = (i - COLUMN_1 + 1) + 30;
		m_nLPAreaNums[i][11] = (i - COLUMN_1 + 1) + 33;
		for (int j = 12; j < 18; j++)
		{
			m_nLPAreaNums[i][j] = 255;
		}
	}

	for (int i = DOZEN_1; i <= DOZEN_3; i++)
	{
		m_nLPAreaNums[i][0] = (i - DOZEN_1) * 12 + 1;
		m_nLPAreaNums[i][1] = (i - DOZEN_1) * 12 + 2;
		m_nLPAreaNums[i][2] = (i - DOZEN_1) * 12 + 3;
		m_nLPAreaNums[i][3] = (i - DOZEN_1) * 12 + 4;
		m_nLPAreaNums[i][4] = (i - DOZEN_1) * 12 + 5;
		m_nLPAreaNums[i][5] = (i - DOZEN_1) * 12 + 6;
		m_nLPAreaNums[i][6] = (i - DOZEN_1) * 12 + 7;
		m_nLPAreaNums[i][7] = (i - DOZEN_1) * 12 + 8;
		m_nLPAreaNums[i][8] = (i - DOZEN_1) * 12 + 9;
		m_nLPAreaNums[i][9] = (i - DOZEN_1) * 12 + 10;
		m_nLPAreaNums[i][10] = (i - DOZEN_1) * 12 + 11;
		m_nLPAreaNums[i][11] = (i - DOZEN_1) * 12 + 12;
		for (int j = 12; j < 18; j++)
		{
			m_nLPAreaNums[i][j] = 255;
		}
	}

	m_nLPAreaNums[BLACK_1][0] = 2;
	m_nLPAreaNums[BLACK_1][1] = 4;
	m_nLPAreaNums[BLACK_1][2] = 6;
	m_nLPAreaNums[BLACK_1][3] = 8;
	m_nLPAreaNums[BLACK_1][4] = 10;
	m_nLPAreaNums[BLACK_1][5] = 11;
	m_nLPAreaNums[BLACK_1][6] = 13;
	m_nLPAreaNums[BLACK_1][7] = 15;
	m_nLPAreaNums[BLACK_1][8] = 17;
	m_nLPAreaNums[BLACK_1][9] = 20;
	m_nLPAreaNums[BLACK_1][10] = 22;
	m_nLPAreaNums[BLACK_1][11] = 24;
	m_nLPAreaNums[BLACK_1][12] = 26;
	m_nLPAreaNums[BLACK_1][13] = 28;
	m_nLPAreaNums[BLACK_1][14] = 29;
	m_nLPAreaNums[BLACK_1][15] = 31;
	m_nLPAreaNums[BLACK_1][16] = 33;
	m_nLPAreaNums[BLACK_1][17] = 35;

	m_nLPAreaNums[RED_1][0] = 1;
	m_nLPAreaNums[RED_1][1] = 3;
	m_nLPAreaNums[RED_1][2] = 5;
	m_nLPAreaNums[RED_1][3] = 7;
	m_nLPAreaNums[RED_1][4] = 9;
	m_nLPAreaNums[RED_1][5] = 12;
	m_nLPAreaNums[RED_1][6] = 14;
	m_nLPAreaNums[RED_1][7] = 16;
	m_nLPAreaNums[RED_1][8] = 18;
	m_nLPAreaNums[RED_1][9] = 19;
	m_nLPAreaNums[RED_1][10] = 21;
	m_nLPAreaNums[RED_1][11] = 23;
	m_nLPAreaNums[RED_1][12] = 25;
	m_nLPAreaNums[RED_1][13] = 27;
	m_nLPAreaNums[RED_1][14] = 30;
	m_nLPAreaNums[RED_1][15] = 32;
	m_nLPAreaNums[RED_1][16] = 34;
	m_nLPAreaNums[RED_1][17] = 36;

	m_nLPAreaNums[ODD_1][0] = 1;
	m_nLPAreaNums[ODD_1][1] = 3;
	m_nLPAreaNums[ODD_1][2] = 5;
	m_nLPAreaNums[ODD_1][3] = 7;
	m_nLPAreaNums[ODD_1][4] = 9;
	m_nLPAreaNums[ODD_1][5] = 11;
	m_nLPAreaNums[ODD_1][6] = 13;
	m_nLPAreaNums[ODD_1][7] = 15;
	m_nLPAreaNums[ODD_1][8] = 17;
	m_nLPAreaNums[ODD_1][9] = 19;
	m_nLPAreaNums[ODD_1][10] = 21;
	m_nLPAreaNums[ODD_1][11] = 23;
	m_nLPAreaNums[ODD_1][12] = 25;
	m_nLPAreaNums[ODD_1][13] = 27;
	m_nLPAreaNums[ODD_1][14] = 29;
	m_nLPAreaNums[ODD_1][15] = 31;
	m_nLPAreaNums[ODD_1][16] = 33;
	m_nLPAreaNums[ODD_1][17] = 35;

	m_nLPAreaNums[EVEN_1][0] = 2;
	m_nLPAreaNums[EVEN_1][1] = 4;
	m_nLPAreaNums[EVEN_1][2] = 6;
	m_nLPAreaNums[EVEN_1][3] = 8;
	m_nLPAreaNums[EVEN_1][4] = 10;
	m_nLPAreaNums[EVEN_1][5] = 12;
	m_nLPAreaNums[EVEN_1][6] = 14;
	m_nLPAreaNums[EVEN_1][7] = 16;
	m_nLPAreaNums[EVEN_1][8] = 18;
	m_nLPAreaNums[EVEN_1][9] = 20;
	m_nLPAreaNums[EVEN_1][10] = 22;
	m_nLPAreaNums[EVEN_1][11] = 24;
	m_nLPAreaNums[EVEN_1][12] = 26;
	m_nLPAreaNums[EVEN_1][13] = 28;
	m_nLPAreaNums[EVEN_1][14] = 30;
	m_nLPAreaNums[EVEN_1][15] = 32;
	m_nLPAreaNums[EVEN_1][16] = 34;
	m_nLPAreaNums[EVEN_1][17] = 36;

	m_nLPAreaNums[LOW_1][0] = 1;
	m_nLPAreaNums[LOW_1][1] = 2;
	m_nLPAreaNums[LOW_1][2] = 3;
	m_nLPAreaNums[LOW_1][3] = 4;
	m_nLPAreaNums[LOW_1][4] = 5;
	m_nLPAreaNums[LOW_1][5] = 6;
	m_nLPAreaNums[LOW_1][6] = 7;
	m_nLPAreaNums[LOW_1][7] = 8;
	m_nLPAreaNums[LOW_1][8] = 9;
	m_nLPAreaNums[LOW_1][9] = 10;
	m_nLPAreaNums[LOW_1][10] = 11;
	m_nLPAreaNums[LOW_1][11] = 12;
	m_nLPAreaNums[LOW_1][12] = 13;
	m_nLPAreaNums[LOW_1][13] = 14;
	m_nLPAreaNums[LOW_1][14] = 15;
	m_nLPAreaNums[LOW_1][15] = 16;
	m_nLPAreaNums[LOW_1][16] = 17;
	m_nLPAreaNums[LOW_1][17] = 18;

	m_nLPAreaNums[HI_1][0] = 19;
	m_nLPAreaNums[HI_1][1] = 20;
	m_nLPAreaNums[HI_1][2] = 21;
	m_nLPAreaNums[HI_1][3] = 22;
	m_nLPAreaNums[HI_1][4] = 23;
	m_nLPAreaNums[HI_1][5] = 24;
	m_nLPAreaNums[HI_1][6] = 25;
	m_nLPAreaNums[HI_1][7] = 26;
	m_nLPAreaNums[HI_1][8] = 27;
	m_nLPAreaNums[HI_1][9] = 28;
	m_nLPAreaNums[HI_1][10] = 29;
	m_nLPAreaNums[HI_1][11] = 30;
	m_nLPAreaNums[HI_1][12] = 31;
	m_nLPAreaNums[HI_1][13] = 32;
	m_nLPAreaNums[HI_1][14] = 33;
	m_nLPAreaNums[HI_1][15] = 34;
	m_nLPAreaNums[HI_1][16] = 35;
	m_nLPAreaNums[HI_1][17] = 36;
}

CServerControlItemSink::~CServerControlItemSink( void )
{

}

//激活控制
bool CServerControlItemSink::ActiveControl(IUnknownEx * pIUnknownEx)
{
	m_pITableFrame = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, ITableFrame);
	if (m_pITableFrame == NULL) 
		return false;

	m_pGameServiceOption = m_pITableFrame->GetGameServiceOption();

	if (m_pGameServiceOption == NULL)
		return false;

	return true;
}

//返回控制区域
VOID * CServerControlItemSink::ControlResult(VOID * pData, WORD wDataSize, VOID * pResult)
{
	if (m_pITableFrame == NULL || m_pGameServiceOption == NULL)
		return NULL;

	switch (m_pGameServiceOption->wKindID)
	{
		//百家乐
		case 106:
		{
			return OnBaccaratControlResult(pData, wDataSize, pResult);
		}
		//红黑
		case 104:
		{
			return OnRedBlackControlResult(pData, wDataSize, pResult);
		}
		//龙虎
		case 105:
		{
			return OnLongHuDouControlResult(pData, wDataSize, pResult);
		}
		//百人牛牛
		case 102:
		{
			return OnOxBattleControlResult(pData, wDataSize, pResult);
		}
		//斗地主
		case 100:
		{
			return OnLandControlResult(pData, wDataSize, pResult);
		}
		//扎金花
		case 101:
		{
			return OnZhaJinHuaControlResult(pData, wDataSize, pResult);
		}
		//抢庄牛牛
		case 103:
		{
			return OnOxSixControlResult(pData, wDataSize, pResult);
		}
		//深海捕鱼
		case 407:
		{
			return OnShbyControlResult(pData, wDataSize, pResult);
		}
		//金蟾捕鱼
		case 107:
		{
			return OnJcbyControlResult(pData, wDataSize, pResult);
		}
		//大闹天宫捕鱼
		case 411:
		{
			return OnDntgControlResult(pData, wDataSize, pResult);
		}
		//财神到
		case 516:
		{
			return OnCaiShenControlResult(pData, wDataSize, pResult);
		}
		//绝地求生
		case 517:
		{
			return OnJDQSControlResult(pData, wDataSize, pResult);
		}
		//飞禽走兽
		case 110:
		{
			return OnFQZSControlResult(pData, wDataSize, pResult);
		}
		//奔驰宝马
		case 111:
		{
			return OnBCBMControlResult(pData, wDataSize, pResult);
		}
		//水浒传
		case 618:
		{
			return OnSHZControlResult(pData, wDataSize, pResult);
		}
		//轮盘
		case 123:
		{
			return OnLPControlResult(pData, wDataSize, pResult);
		}
		//骰宝
		case 127:
		{
			return OnSICBOControlResult(pData, wDataSize, pResult);
		}
		//十三水
		case 126:
		{
			return OnThirteenControlResult(pData, wDataSize, pResult);
		}
		//百人二八杠
		case 320:
		{
			return OnTongziControlResult(pData, wDataSize, pResult);
		}
		//三公
		case 352:
		{
			return OnSanGongControlResult(pData, wDataSize, pResult);
		}
		//水果拉霸
		case 119:
		{
			return OnSGLBControlResult(pData, wDataSize, pResult);
		}
	}

	return NULL;
}

//百家乐控制结果
VOID * CServerControlItemSink::OnBaccaratControlResult(VOID * pData, WORD wDataSize, VOID * pResult)
{
	if (wDataSize != sizeof(tag_Baccarat_Parameter))
		return NULL;

	tag_Baccarat_Parameter *pBaccarat_Parameter = (tag_Baccarat_Parameter *)pData;
	bool bSuccess = true;

	tag_Baccarat_Result * Baccarat_Result = (tag_Baccarat_Result *)pResult;

	//局控
	if (pBaccarat_Parameter->cbJuControl == TRUE)
	{
		//计算分数
		BYTE cbDispatchCount = 0;

		do
		{
			bSuccess = true;
			//派发扑克
			DispatchBaccaratCard(Baccarat_Result);

			//用做牌的方式去控制对子区域输赢，更省资源
			//先判断庄对子和闲对子
			if (pBaccarat_Parameter->cbJuControlArea[AREA_XIAN_DUI] == 1)
			{
				if (m_BaccaratLogic.GetCardValue(Baccarat_Result->cbTableCardArray[INDEX_PLAYER][0]) != m_BaccaratLogic.GetCardValue(Baccarat_Result->cbTableCardArray[INDEX_PLAYER][1]))
				{
					Baccarat_Result->cbTableCardArray[INDEX_PLAYER][1] = m_BaccaratLogic.cbGetControlCard(Baccarat_Result->cbTableCardArray[INDEX_PLAYER], Baccarat_Result->cbCardCount[INDEX_PLAYER], Baccarat_Result->cbTableCardArray[INDEX_BANKER], Baccarat_Result->cbCardCount[INDEX_BANKER], Baccarat_Result->cbTableCardArray[INDEX_PLAYER][0]);
				}
			}
			else if (pBaccarat_Parameter->cbJuControlArea[AREA_XIAN_DUI] == 2)
			{
				if (m_BaccaratLogic.GetCardValue(Baccarat_Result->cbTableCardArray[INDEX_PLAYER][0]) == m_BaccaratLogic.GetCardValue(Baccarat_Result->cbTableCardArray[INDEX_PLAYER][1]))
				{
					Baccarat_Result->cbTableCardArray[INDEX_PLAYER][1] = m_BaccaratLogic.cbGetControlCard(Baccarat_Result->cbTableCardArray[INDEX_PLAYER], Baccarat_Result->cbCardCount[INDEX_PLAYER], Baccarat_Result->cbTableCardArray[INDEX_BANKER], Baccarat_Result->cbCardCount[INDEX_BANKER]);
				}
			}
			//判断完闲对子，赋值一下正确的扑克数量，方便判断后面的区域
			CalculateCardCount(Baccarat_Result);

			if (pBaccarat_Parameter->cbJuControlArea[AREA_ZHUANG_DUI] == 1)
			{
				if (m_BaccaratLogic.GetCardValue(Baccarat_Result->cbTableCardArray[INDEX_BANKER][0]) != m_BaccaratLogic.GetCardValue(Baccarat_Result->cbTableCardArray[INDEX_BANKER][1]))
				{
					Baccarat_Result->cbTableCardArray[INDEX_BANKER][1] = m_BaccaratLogic.cbGetControlCard(Baccarat_Result->cbTableCardArray[INDEX_PLAYER], Baccarat_Result->cbCardCount[INDEX_PLAYER], Baccarat_Result->cbTableCardArray[INDEX_BANKER], Baccarat_Result->cbCardCount[INDEX_BANKER], Baccarat_Result->cbTableCardArray[INDEX_BANKER][0]);
				}
			}
			else if (pBaccarat_Parameter->cbJuControlArea[AREA_ZHUANG_DUI] == 2)
			{
				if (m_BaccaratLogic.GetCardValue(Baccarat_Result->cbTableCardArray[INDEX_BANKER][0]) == m_BaccaratLogic.GetCardValue(Baccarat_Result->cbTableCardArray[INDEX_BANKER][1]))
				{
					Baccarat_Result->cbTableCardArray[INDEX_BANKER][1] = m_BaccaratLogic.cbGetControlCard(Baccarat_Result->cbTableCardArray[INDEX_PLAYER], Baccarat_Result->cbCardCount[INDEX_PLAYER], Baccarat_Result->cbTableCardArray[INDEX_BANKER], Baccarat_Result->cbCardCount[INDEX_BANKER]);
				}
			}
			//判断完庄对子，赋值一下正确的扑克数量，方便判断后面的区域
			CalculateCardCount(Baccarat_Result);

			BYTE cbWinArea[AREA_MAX] = { FALSE };
			
			//计算牌点
			BYTE cbPlayerCount = m_BaccaratLogic.GetCardListPip(Baccarat_Result->cbTableCardArray[INDEX_PLAYER], Baccarat_Result->cbCardCount[INDEX_PLAYER]);
			BYTE cbBankerCount = m_BaccaratLogic.GetCardListPip(Baccarat_Result->cbTableCardArray[INDEX_BANKER], Baccarat_Result->cbCardCount[INDEX_BANKER]);

			//胜利区域--------------------------
			//平
			if (cbPlayerCount == cbBankerCount)
			{
				cbWinArea[AREA_PING] = TRUE;
			}
			// 庄
			else if (cbPlayerCount < cbBankerCount)
			{
				cbWinArea[AREA_ZHUANG] = TRUE;
			}
			// 闲
			else
			{
				cbWinArea[AREA_XIAN] = TRUE;
			}


			//对子判断
			if (m_BaccaratLogic.GetCardValue(Baccarat_Result->cbTableCardArray[INDEX_PLAYER][0]) == m_BaccaratLogic.GetCardValue(Baccarat_Result->cbTableCardArray[INDEX_PLAYER][1]))
			{
				cbWinArea[AREA_XIAN_DUI] = TRUE;
			}
			if (m_BaccaratLogic.GetCardValue(Baccarat_Result->cbTableCardArray[INDEX_BANKER][0]) == m_BaccaratLogic.GetCardValue(Baccarat_Result->cbTableCardArray[INDEX_BANKER][1]))
			{
				cbWinArea[AREA_ZHUANG_DUI] = TRUE;
			}

			//判断庄闲平
			if (pBaccarat_Parameter->cbJuControlArea[AREA_XIAN] == 1 || pBaccarat_Parameter->cbJuControlArea[AREA_XIAN] == 2)
			{
				if ((pBaccarat_Parameter->cbJuControlArea[AREA_XIAN] == 1 && cbWinArea[AREA_XIAN] == FALSE) || (pBaccarat_Parameter->cbJuControlArea[AREA_XIAN] == 2 && cbWinArea[AREA_XIAN] == TRUE))
				{
					bSuccess = false;
				}
			}
			else if (pBaccarat_Parameter->cbJuControlArea[AREA_PING] == 1 || pBaccarat_Parameter->cbJuControlArea[AREA_PING] == 2)
			{
				if ((pBaccarat_Parameter->cbJuControlArea[AREA_PING] == 1 && cbWinArea[AREA_PING] == FALSE) || (pBaccarat_Parameter->cbJuControlArea[AREA_PING] == 2 && cbWinArea[AREA_PING] == TRUE))
				{
					bSuccess = false;
				}
			}
			else if (pBaccarat_Parameter->cbJuControlArea[AREA_ZHUANG] == 1 || pBaccarat_Parameter->cbJuControlArea[AREA_ZHUANG] == 2)
			{
				if ((pBaccarat_Parameter->cbJuControlArea[AREA_ZHUANG] == 1 && cbWinArea[AREA_ZHUANG] == FALSE) || (pBaccarat_Parameter->cbJuControlArea[AREA_ZHUANG] == 2 && cbWinArea[AREA_ZHUANG] == TRUE))
				{
					bSuccess = false;
				}
			}

			cbDispatchCount++;

			if (cbDispatchCount > 200)
				break;

		} while (!bSuccess);
	}
	//库存控制判断
	else
	{
		BYTE bControlType = 0;	//0不控制，1控制系统赢:概率区间101~200，2控制系统输概率区间1~100
		SCORE lCurrentStorage = m_pGameServiceOption->pRoomStorageOption.lStorageCurrent;
		SCORE lStorageMax = m_pGameServiceOption->pRoomStorageOption.lStorageMax;
		int lStorageMul = m_pGameServiceOption->pRoomStorageOption.lStorageMul;
		SCORE lMinStorage[STORAGE_COUNT] = { 0 };
		SCORE lMaxStorage[STORAGE_COUNT] = { 0 };
		int nMulStorage[STORAGE_COUNT] = { 0 };

		TCHAR outBuff[64] = { 0 };
		for (int i = 0; i < STORAGE_COUNT; i++)
		{
			memset(outBuff, 0, sizeof(outBuff));
			_sntprintf_s(outBuff, sizeof(outBuff), _T("%s"), m_pGameServiceOption->pRoomStorageOption.szStorageControl[i]);

			SCORE llValue[3] = { 0 };
			this->GetValueFromCombStr(llValue, CountArray(llValue), outBuff, _tcslen(outBuff));
			lMinStorage[i] = llValue[0];
			lMaxStorage[i] = llValue[1];
			nMulStorage[i] = llValue[2];
		}

		int nControlIndex = 0;
		for (; nControlIndex < STORAGE_COUNT; nControlIndex++)
		{
			if (lCurrentStorage > lMinStorage[nControlIndex] && lCurrentStorage <= lMaxStorage[nControlIndex])
				break;
		}

		//进入库存控制
		if ((nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] > 100) || lCurrentStorage < 0)
		{
			if ((rand() % 100) < (nMulStorage[nControlIndex] - 100) || lCurrentStorage < 0)
			{
				bControlType = 1;
			}
		}

		// 送分判断
		bool bStorageSend = nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] <= 100 && nMulStorage[nControlIndex] > 0 && (rand() % 100) < nMulStorage[nControlIndex];
		if ((lCurrentStorage > 0 && lCurrentStorage > lStorageMax && rand() % 100 < lStorageMul) || bStorageSend)
		{
			bControlType = 2;
		}

		BYTE cbRealCount = 0;
		BYTE cbAndroidCount = 0;
		for (WORD i = 0; i < BACCARAT_PLAYER; i++)
		{
			IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
			if (pServerUser)
			{
				if (!pServerUser->IsAndroidUser())
					cbRealCount++;
				else
					cbAndroidCount++;
			}
		}

		//如果没有真人就不控制
		if (cbRealCount == 0)
			bControlType = 0;

		//计算分数
		BYTE cbDispatchCount = 0;
		do
		{
			//派发扑克
			DispatchBaccaratCard(Baccarat_Result);

			tag_Baccart_ScoreParameter ScoreParameter;
			ScoreParameter.wBankerUser = pBaccarat_Parameter->wBankerUser;
			CopyMemory(ScoreParameter.lPlayBet, pBaccarat_Parameter->lPlayBet, sizeof(ScoreParameter.lPlayBet));
			CopyMemory(ScoreParameter.cbTableCard, Baccarat_Result->cbTableCardArray, sizeof(ScoreParameter.cbTableCard));
			CopyMemory(ScoreParameter.cbCardCount, Baccarat_Result->cbCardCount, sizeof(ScoreParameter.cbCardCount));

			//试探性判断
			SCORE lSystemScore = CalculateBaccaratScore(ScoreParameter);
			cbDispatchCount++;

			bSuccess = true;

			if (bControlType == 1 && lSystemScore <= 0)
				bSuccess = false;
			if (bControlType == 2 && lSystemScore >= 0)
				bSuccess = false;

			if ((lSystemScore + lCurrentStorage) < 0 && lSystemScore < 0)
				bSuccess = false;

			if (cbDispatchCount > 100)
				break;
		} while (!bSuccess);
	}

	return Baccarat_Result;
}

//百家乐洗牌
void CServerControlItemSink::DispatchBaccaratCard(tag_Baccarat_Result *Baccarat_Result)
{
	//随机扑克
	m_BaccaratLogic.RandCardList(Baccarat_Result->cbTableCardArray[0], sizeof(Baccarat_Result->cbTableCardArray) / sizeof(Baccarat_Result->cbTableCardArray[0][0]));

	//首次发牌
	Baccarat_Result->cbCardCount[INDEX_PLAYER] = 2;
	Baccarat_Result->cbCardCount[INDEX_BANKER] = 2;

	//计算点数
	BYTE cbBankerCount = m_BaccaratLogic.GetCardListPip(Baccarat_Result->cbTableCardArray[INDEX_BANKER], Baccarat_Result->cbCardCount[INDEX_BANKER]);
	BYTE cbPlayerTwoCardCount = m_BaccaratLogic.GetCardListPip(Baccarat_Result->cbTableCardArray[INDEX_PLAYER], Baccarat_Result->cbCardCount[INDEX_PLAYER]);

	//闲家补牌
	BYTE cbPlayerThirdCardValue = 0;	//第三张牌点数
	if (cbPlayerTwoCardCount <= 5 && cbBankerCount<8)
	{
		//计算点数
		Baccarat_Result->cbCardCount[INDEX_PLAYER]++;
		cbPlayerThirdCardValue = m_BaccaratLogic.GetCardPip(Baccarat_Result->cbTableCardArray[INDEX_PLAYER][2]);
	}

	//庄家补牌
	if (cbPlayerTwoCardCount<8 && cbBankerCount<8)
	{
		switch (cbBankerCount)
		{
		case 0:
		case 1:
		case 2:
			Baccarat_Result->cbCardCount[INDEX_BANKER]++;
			break;

		case 3:
			if ((Baccarat_Result->cbCardCount[INDEX_PLAYER] == 3 && cbPlayerThirdCardValue != 8) || Baccarat_Result->cbCardCount[INDEX_PLAYER] == 2) Baccarat_Result->cbCardCount[INDEX_BANKER]++;
			break;

		case 4:
			if ((Baccarat_Result->cbCardCount[INDEX_PLAYER] == 3 && cbPlayerThirdCardValue != 1 && cbPlayerThirdCardValue != 8 && cbPlayerThirdCardValue != 9 && cbPlayerThirdCardValue != 0) || Baccarat_Result->cbCardCount[INDEX_PLAYER] == 2) Baccarat_Result->cbCardCount[INDEX_BANKER]++;
			break;

		case 5:
			if ((Baccarat_Result->cbCardCount[INDEX_PLAYER] == 3 && cbPlayerThirdCardValue != 1 && cbPlayerThirdCardValue != 2 && cbPlayerThirdCardValue != 3 && cbPlayerThirdCardValue != 8 && cbPlayerThirdCardValue != 9 && cbPlayerThirdCardValue != 0) || Baccarat_Result->cbCardCount[INDEX_PLAYER] == 2) Baccarat_Result->cbCardCount[INDEX_BANKER]++;
			break;

		case 6:
			if (Baccarat_Result->cbCardCount[INDEX_PLAYER] == 3 && (cbPlayerThirdCardValue == 6 || cbPlayerThirdCardValue == 7)) Baccarat_Result->cbCardCount[INDEX_BANKER]++;
			break;

			//不须补牌
		case 7:
		case 8:
		case 9:
			break;
		default:
			break;
		}
	}

	return ;
}

//根据扑克数组赋值扑克数量
void CServerControlItemSink::CalculateCardCount(tag_Baccarat_Result *Baccarat_Result)
{
	//首次发牌
	Baccarat_Result->cbCardCount[INDEX_PLAYER] = 2;
	Baccarat_Result->cbCardCount[INDEX_BANKER] = 2;

	//计算点数
	BYTE cbBankerCount = m_BaccaratLogic.GetCardListPip(Baccarat_Result->cbTableCardArray[INDEX_BANKER], Baccarat_Result->cbCardCount[INDEX_BANKER]);
	BYTE cbPlayerTwoCardCount = m_BaccaratLogic.GetCardListPip(Baccarat_Result->cbTableCardArray[INDEX_PLAYER], Baccarat_Result->cbCardCount[INDEX_PLAYER]);

	//闲家补牌
	BYTE cbPlayerThirdCardValue = 0;	//第三张牌点数
	if (cbPlayerTwoCardCount <= 5 && cbBankerCount<8)
	{
		//计算点数
		Baccarat_Result->cbCardCount[INDEX_PLAYER]++;
		cbPlayerThirdCardValue = m_BaccaratLogic.GetCardPip(Baccarat_Result->cbTableCardArray[INDEX_PLAYER][2]);
	}

	//庄家补牌
	if (cbPlayerTwoCardCount<8 && cbBankerCount<8)
	{
		switch (cbBankerCount)
		{
		case 0:
		case 1:
		case 2:
			Baccarat_Result->cbCardCount[INDEX_BANKER]++;
			break;

		case 3:
			if ((Baccarat_Result->cbCardCount[INDEX_PLAYER] == 3 && cbPlayerThirdCardValue != 8) || Baccarat_Result->cbCardCount[INDEX_PLAYER] == 2) Baccarat_Result->cbCardCount[INDEX_BANKER]++;
			break;

		case 4:
			if ((Baccarat_Result->cbCardCount[INDEX_PLAYER] == 3 && cbPlayerThirdCardValue != 1 && cbPlayerThirdCardValue != 8 && cbPlayerThirdCardValue != 9 && cbPlayerThirdCardValue != 0) || Baccarat_Result->cbCardCount[INDEX_PLAYER] == 2) Baccarat_Result->cbCardCount[INDEX_BANKER]++;
			break;

		case 5:
			if ((Baccarat_Result->cbCardCount[INDEX_PLAYER] == 3 && cbPlayerThirdCardValue != 1 && cbPlayerThirdCardValue != 2 && cbPlayerThirdCardValue != 3 && cbPlayerThirdCardValue != 8 && cbPlayerThirdCardValue != 9 && cbPlayerThirdCardValue != 0) || Baccarat_Result->cbCardCount[INDEX_PLAYER] == 2) Baccarat_Result->cbCardCount[INDEX_BANKER]++;
			break;

		case 6:
			if (Baccarat_Result->cbCardCount[INDEX_PLAYER] == 3 && (cbPlayerThirdCardValue == 6 || cbPlayerThirdCardValue == 7)) Baccarat_Result->cbCardCount[INDEX_BANKER]++;
			break;

			//不须补牌
		case 7:
		case 8:
		case 9:
			break;
		default:
			break;
		}
	}
}

//百家乐计算系统分
SCORE CServerControlItemSink::CalculateBaccaratScore(tag_Baccart_ScoreParameter ScoreParameter)
{
	SCORE lSystemScore = 0;

	//计算牌点
	BYTE cbPlayerCount = m_BaccaratLogic.GetCardListPip(ScoreParameter.cbTableCard[INDEX_PLAYER], ScoreParameter.cbCardCount[INDEX_PLAYER]);
	BYTE cbBankerCount = m_BaccaratLogic.GetCardListPip(ScoreParameter.cbTableCard[INDEX_BANKER], ScoreParameter.cbCardCount[INDEX_BANKER]);

	//推断玩家
	BYTE cbWinArea[AREA_MAX] = { FALSE };
	
	//胜利区域--------------------------
	//平
	if (cbPlayerCount == cbBankerCount)
	{
		cbWinArea[AREA_PING] = TRUE;
	}
	// 庄
	else if (cbPlayerCount < cbBankerCount)
	{
		cbWinArea[AREA_ZHUANG] = TRUE;
	}
	// 闲
	else
	{
		cbWinArea[AREA_XIAN] = TRUE;
	}


	//对子判断
	if (m_BaccaratLogic.GetCardValue(ScoreParameter.cbTableCard[INDEX_PLAYER][0]) == m_BaccaratLogic.GetCardValue(ScoreParameter.cbTableCard[INDEX_PLAYER][1]))
	{
		cbWinArea[AREA_XIAN_DUI] = TRUE;
	}
	if (m_BaccaratLogic.GetCardValue(ScoreParameter.cbTableCard[INDEX_BANKER][0]) == m_BaccaratLogic.GetCardValue(ScoreParameter.cbTableCard[INDEX_BANKER][1]))
	{
		cbWinArea[AREA_ZHUANG_DUI] = TRUE;
	}

	//区域倍率
	BYTE cbMultiple[AREA_MAX] = { MULTIPLE_ZHUANG, MULTIPLE_XIAN, MULTIPLE_PING, MULTIPLE_ZHUANG_PING, MULTIPLE_XIAN_PING };

	//庄家是不是机器人
	bool bIsBankerAndroidUser = false;
	if (ScoreParameter.wBankerUser != INVALID_CHAIR)
	{
		IServerUserItem * pIBankerUserItem = m_pITableFrame->GetTableUserItem(ScoreParameter.wBankerUser);
		if (pIBankerUserItem != NULL)
		{
			bIsBankerAndroidUser = pIBankerUserItem->IsAndroidUser();
		}
	}

	//计算积分
	for (WORD wChairID = 0; wChairID < BACCARAT_PLAYER; wChairID++)
	{
		//庄家判断
		if (ScoreParameter.wBankerUser == wChairID) continue;

		//获取用户
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem == NULL) continue;

		bool bIsAndroidUser = pIServerUserItem->IsAndroidUser();

		for (WORD wAreaIndex = 0; wAreaIndex < AREA_MAX; ++wAreaIndex)
		{
			if (cbWinArea[wAreaIndex] == TRUE)
			{
				//系统得分
				if (bIsAndroidUser)
					lSystemScore += (ScoreParameter.lPlayBet[wChairID][wAreaIndex] * (cbMultiple[wAreaIndex] - 1));
				if (ScoreParameter.wBankerUser == INVALID_CHAIR || bIsBankerAndroidUser)
					lSystemScore -= (ScoreParameter.lPlayBet[wChairID][wAreaIndex] * (cbMultiple[wAreaIndex] - 1));
			}
			else if (cbWinArea[AREA_PING] == TRUE && (wAreaIndex == AREA_XIAN || wAreaIndex == AREA_ZHUANG))
			{

			}
			else
			{
				//系统得分
				if (bIsAndroidUser)
					lSystemScore -= ScoreParameter.lPlayBet[wChairID][wAreaIndex];
				if (ScoreParameter.wBankerUser == INVALID_CHAIR || bIsBankerAndroidUser)
					lSystemScore += ScoreParameter.lPlayBet[wChairID][wAreaIndex];
			}

		}
	}

	return lSystemScore;
}

//红黑控制结果
VOID * CServerControlItemSink::OnRedBlackControlResult(VOID * pData, WORD wDataSize, VOID * pResult)
{
	if (wDataSize != sizeof(tag_Redblack_Parameter))
		return NULL;

	tag_Redblack_Parameter *pRedblack_Parameter = (tag_Redblack_Parameter *)pData;
	bool bSuccess = true;

	tag_Redblack_Result * Redblack_Result = (tag_Redblack_Result *)pResult;

	//局控
	if (pRedblack_Parameter->cbJuControl == TRUE)
	{
		//计算分数
		BYTE cbDispatchCount = 0;

		do
		{
			bSuccess = true;
			//派发扑克
			DispatchRedBlackCard(Redblack_Result);

			//如果是豹子和顺金需要做一下牌
			if (pRedblack_Parameter->cbJuControlArea[RBAREA_PING] == CT_RB_BAO_ZI)
			{
				if (pRedblack_Parameter->cbJuControlArea[RBAREA_XIAN] == 1)
				{
					for (int i = 1; i < 3; i++)
					{
						BYTE cbColor = Redblack_Result->cbTableCardArray[RBAREA_XIAN][0] & MASK_COLOR;
						BYTE cbValue = Redblack_Result->cbTableCardArray[RBAREA_XIAN][0] & MASK_VALUE;
						cbColor = (cbColor + 0x10 * i) % 0x40;
						Redblack_Result->cbTableCardArray[RBAREA_XIAN][i] = cbColor | cbValue;
					}

					int nIndex = 0;

					while (true)
					{
						m_RedBlackLogic.RandCardList(Redblack_Result->cbTableCardArray[RBAREA_ZHUANG], sizeof(BYTE) * 3);
						bool bRepeat = false;
						for (int i = 0; i < 3; i++)
						{
							for (int j = 0; j < 3; j++)
							{
								if (Redblack_Result->cbTableCardArray[RBAREA_ZHUANG][i] == Redblack_Result->cbTableCardArray[RBAREA_XIAN][j])
								{
									bRepeat = true;
									break;
								}
							}
							if (bRepeat)
								break;
						}

						if (!bRepeat)
							break;

						nIndex++;

						if (nIndex > 100)
							break;
					}
				}
				else if (pRedblack_Parameter->cbJuControlArea[RBAREA_ZHUANG] == 1)
				{
					for (int i = 1; i < 3; i++)
					{
						BYTE cbColor = Redblack_Result->cbTableCardArray[RBAREA_ZHUANG][0] & MASK_COLOR;
						BYTE cbValue = Redblack_Result->cbTableCardArray[RBAREA_ZHUANG][0] & MASK_VALUE;
						cbColor = (cbColor + 0x10 * i) % 0x40;
						Redblack_Result->cbTableCardArray[RBAREA_ZHUANG][i] = cbColor | cbValue;
					}

					int nIndex = 0;

					while (true)
					{
						m_RedBlackLogic.RandCardList(Redblack_Result->cbTableCardArray[RBAREA_XIAN], sizeof(BYTE) * 3);
						bool bRepeat = false;
						for (int i = 0; i < 3; i++)
						{
							for (int j = 0; j < 3; j++)
							{
								if (Redblack_Result->cbTableCardArray[RBAREA_XIAN][i] == Redblack_Result->cbTableCardArray[RBAREA_ZHUANG][j])
								{
									bRepeat = true;
									break;
								}
							}
							if (bRepeat)
								break;
						}

						if (!bRepeat)
							break;

						nIndex++;

						if (nIndex > 100)
							break;
					}
				}
			}
			else if (pRedblack_Parameter->cbJuControlArea[RBAREA_PING] == CT_RB_SHUN_JIN)
			{
				if (pRedblack_Parameter->cbJuControlArea[RBAREA_XIAN] == 1)
				{
					for (int i = 1; i < 3; i++)
					{
						BYTE cbColor = Redblack_Result->cbTableCardArray[RBAREA_XIAN][0] & MASK_COLOR;
						BYTE cbValue = Redblack_Result->cbTableCardArray[RBAREA_XIAN][0] & MASK_VALUE;

						if (cbValue <= 0x0C)
						{
							cbValue = (cbValue + 0x01 * i) % 0x0D;
							if (cbValue == 0x00)
								cbValue = 0x01;
							Redblack_Result->cbTableCardArray[RBAREA_XIAN][i] = cbColor | cbValue;
						}
						else if (cbValue >= 0x03)
						{
							cbValue = cbValue - 0x01 * i;
							Redblack_Result->cbTableCardArray[RBAREA_XIAN][i] = cbColor | cbValue;
						}
					}

					int nIndex = 0;

					while (true)
					{
						m_RedBlackLogic.RandCardList(Redblack_Result->cbTableCardArray[RBAREA_ZHUANG], sizeof(BYTE) * 3);
						bool bRepeat = false;
						for (int i = 0; i < 3; i++)
						{
							for (int j = 0; j < 3; j++)
							{
								if (Redblack_Result->cbTableCardArray[RBAREA_ZHUANG][i] == Redblack_Result->cbTableCardArray[RBAREA_XIAN][j])
								{
									bRepeat = true;
									break;
								}
							}
							if (bRepeat)
								break;
						}

						if (!bRepeat)
							break;

						nIndex++;

						if (nIndex > 100)
							break;
					}
				}
				else if (pRedblack_Parameter->cbJuControlArea[RBAREA_ZHUANG] == 1)
				{
					for (int i = 1; i < 3; i++)
					{
						BYTE cbColor = Redblack_Result->cbTableCardArray[RBAREA_ZHUANG][0] & MASK_COLOR;
						BYTE cbValue = Redblack_Result->cbTableCardArray[RBAREA_ZHUANG][0] & MASK_VALUE;

						if (cbValue <= 0x0C)
						{
							cbValue = (cbValue + 0x01 * i) % 0x0D;
							if (cbValue == 0x00)
								cbValue = 0x01;
							Redblack_Result->cbTableCardArray[RBAREA_ZHUANG][i] = cbColor | cbValue;
						}
						else if (cbValue >= 0x03)
						{
							cbValue = cbValue - 0x01 * i;
							Redblack_Result->cbTableCardArray[RBAREA_ZHUANG][i] = cbColor | cbValue;
						}
					}

					int nIndex = 0;

					while (true)
					{
						m_RedBlackLogic.RandCardList(Redblack_Result->cbTableCardArray[RBAREA_XIAN], sizeof(BYTE) * 3);
						bool bRepeat = false;
						for (int i = 0; i < 3; i++)
						{
							for (int j = 0; j < 3; j++)
							{
								if (Redblack_Result->cbTableCardArray[RBAREA_XIAN][i] == Redblack_Result->cbTableCardArray[RBAREA_ZHUANG][j])
								{
									bRepeat = true;
									break;
								}
							}
							if (bRepeat)
								break;
						}

						if (!bRepeat)
							break;

						nIndex++;

						if (nIndex > 100)
							break;
					}
				}
			}

			BYTE cbWinArea[RBAREA_MAX] = { FALSE };
			
			//计算牌点
			BYTE cbPlayerCount = m_RedBlackLogic.GetCardType(Redblack_Result->cbTableCardArray[INDEX_PLAYER], Redblack_Result->cbCardCount[INDEX_PLAYER]);
			BYTE cbBankerCount = m_RedBlackLogic.GetCardType(Redblack_Result->cbTableCardArray[INDEX_BANKER], Redblack_Result->cbCardCount[INDEX_BANKER]);

			BYTE cbWin = m_RedBlackLogic.CompareCard(Redblack_Result->cbTableCardArray[INDEX_PLAYER], Redblack_Result->cbTableCardArray[INDEX_BANKER], 3);


			////////////////////////////////2018-3-26
			if (cbWin)
				cbWinArea[RBAREA_XIAN] = TRUE;
			else
				cbWinArea[RBAREA_ZHUANG] = TRUE;

			BYTE cbPointRed = m_RedBlackLogic.GetCardType(Redblack_Result->cbTableCardArray[INDEX_PLAYER], 3);
			BYTE cbPointBlack = m_RedBlackLogic.GetCardType(Redblack_Result->cbTableCardArray[INDEX_BANKER], 3);

			if (cbPointRed > 3 || cbPointBlack > 3)
				cbWinArea[RBAREA_PING] = TRUE;

			//判断庄闲平
			for (BYTE i = 0; i < RBAREA_MAX; i++)
			{
				if (i != RBAREA_PING)
				{
					if ((pRedblack_Parameter->cbJuControlArea[i] == 1 && cbWinArea[i] == FALSE) || (pRedblack_Parameter->cbJuControlArea[i] == 2 && cbWinArea[i] == TRUE))
					{
						bSuccess = false;
						break;
					}
				}
				else
				{
					if (pRedblack_Parameter->cbJuControlArea[i] >= CT_RB_DOUBLE_SHINE && pRedblack_Parameter->cbJuControlArea[i] <= CT_RB_BAO_ZI)
					{
						if (!(pRedblack_Parameter->cbJuControlArea[i] == cbPointRed || cbPointBlack == pRedblack_Parameter->cbJuControlArea[i]))
						{
							bSuccess = false;
							break;
						}
					}
				}
			}

			cbDispatchCount++;

			if (cbDispatchCount > 200)
				break;

		} while (!bSuccess);
	}
	//库存控制判断
	else
	{
		BYTE bControlType = 0;	//0不控制，1控制系统赢:概率区间101~200，2控制系统输概率区间1~100
		SCORE lCurrentStorage = m_pGameServiceOption->pRoomStorageOption.lStorageCurrent;
		SCORE lStorageMax = m_pGameServiceOption->pRoomStorageOption.lStorageMax;
		int lStorageMul = m_pGameServiceOption->pRoomStorageOption.lStorageMul;
		SCORE lMinStorage[STORAGE_COUNT] = { 0 };
		SCORE lMaxStorage[STORAGE_COUNT] = { 0 };
		int nMulStorage[STORAGE_COUNT] = { 0 };

		TCHAR outBuff[64] = { 0 };
		for (int i = 0; i < STORAGE_COUNT; i++)
		{
			memset(outBuff, 0, sizeof(outBuff));
			_sntprintf_s(outBuff, sizeof(outBuff), _T("%s"), m_pGameServiceOption->pRoomStorageOption.szStorageControl[i]);

			SCORE llValue[3] = { 0 };
			this->GetValueFromCombStr(llValue, CountArray(llValue), outBuff, _tcslen(outBuff));
			lMinStorage[i] = llValue[0];
			lMaxStorage[i] = llValue[1];
			nMulStorage[i] = llValue[2];
		}

		int nControlIndex = 0;
		for (; nControlIndex < STORAGE_COUNT; nControlIndex++)
		{
			if (lCurrentStorage > lMinStorage[nControlIndex] && lCurrentStorage <= lMaxStorage[nControlIndex])
				break;
		}

		//进入库存控制
		if ((nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] > 100) || lCurrentStorage < 0)
		{
			if ((rand() % 100) < (nMulStorage[nControlIndex] - 100) || lCurrentStorage < 0)
			{
				bControlType = 1;
			}
		}

		// 送分判断
		bool bStorageSend = nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] <= 100 && nMulStorage[nControlIndex] > 0 && (rand() % 100) < nMulStorage[nControlIndex];
		if ((lCurrentStorage > 0 && lCurrentStorage > lStorageMax && rand() % 100 < lStorageMul) || bStorageSend)
		{
			bControlType = 2;
		}

		BYTE cbRealCount = 0;
		BYTE cbAndroidCount = 0;
		for (WORD i = 0; i < REDBLACK_PLAYER; i++)
		{
			IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
			if (pServerUser)
			{
				if (!pServerUser->IsAndroidUser())
					cbRealCount++;
				else
					cbAndroidCount++;
			}
		}

		//如果没有真人就不控制
		if (cbRealCount == 0)
			bControlType = 0;

		//计算分数
		BYTE cbDispatchCount = 0;
		do
		{
			//派发扑克
			DispatchRedBlackCard(Redblack_Result);

			tag_RedBlack_ScoreParameter ScoreParameter;
			ScoreParameter.wBankerUser = pRedblack_Parameter->wBankerUser;
			CopyMemory(ScoreParameter.lPlayBet, pRedblack_Parameter->lPlayBet, sizeof(ScoreParameter.lPlayBet));
			CopyMemory(ScoreParameter.cbTableCard, Redblack_Result->cbTableCardArray, sizeof(ScoreParameter.cbTableCard));
			CopyMemory(ScoreParameter.cbCardCount, Redblack_Result->cbCardCount, sizeof(ScoreParameter.cbCardCount));

			//试探性判断
			SCORE lSystemScore = CalculateRedBlackScore(ScoreParameter);
			cbDispatchCount++;


			bSuccess = true;

			if (bControlType == 1 && lSystemScore <= 0)
				bSuccess = false;
			if (bControlType == 2 && lSystemScore >= 0)
				bSuccess = false;

			if ((lSystemScore + lCurrentStorage) < 0 && lSystemScore < 0)
				bSuccess = false;

			if (cbDispatchCount > 100)
				break;
		} while (!bSuccess);
	}

	return Redblack_Result;
}

//红黑洗牌
void CServerControlItemSink::DispatchRedBlackCard(tag_Redblack_Result *Redblack_Result)
{
	//随机扑克
	m_RedBlackLogic.RandCardList(Redblack_Result->cbTableCardArray[0], sizeof(Redblack_Result->cbTableCardArray) / sizeof(Redblack_Result->cbTableCardArray[0][0]));

	//首次发牌
	Redblack_Result->cbCardCount[INDEX_PLAYER] = MAX_COUNT;
	Redblack_Result->cbCardCount[INDEX_BANKER] = MAX_COUNT;
}

//红黑计算系统分
SCORE CServerControlItemSink::CalculateRedBlackScore(tag_RedBlack_ScoreParameter ScoreParameter)
{
	//计算牌点
	BYTE cbPlayerCount = m_RedBlackLogic.GetCardType(ScoreParameter.cbTableCard[INDEX_PLAYER], ScoreParameter.cbCardCount[INDEX_PLAYER]);
	BYTE cbBankerCount = m_RedBlackLogic.GetCardType(ScoreParameter.cbTableCard[INDEX_BANKER], ScoreParameter.cbCardCount[INDEX_BANKER]);

	//系统输赢
	SCORE lSystemScore = 0;

	//推断玩家
	BYTE cbWinArea[RBAREA_MAX] = { FALSE };
	
	BYTE cbWin = m_RedBlackLogic.CompareCard(ScoreParameter.cbTableCard[INDEX_PLAYER], ScoreParameter.cbTableCard[INDEX_BANKER], 3);

	////////////////////////////////2018-3-26
	if (cbWin)
		cbWinArea[RBAREA_XIAN] = TRUE;
	else
		cbWinArea[RBAREA_ZHUANG] = TRUE;

	BYTE cbPointRed = m_RedBlackLogic.GetCardType(ScoreParameter.cbTableCard[INDEX_PLAYER], 3);
	BYTE cbPointBlack = m_RedBlackLogic.GetCardType(ScoreParameter.cbTableCard[INDEX_BANKER], 3);

	if (cbPointRed > 2 || cbPointBlack > 2)
		cbWinArea[RBAREA_PING] = TRUE;


	//区域倍率
	BYTE cbMultiple[RBAREA_MAX] = { 2, 2, 9 };

	int pingPoint = 0;
	int pingType = 0;

	if (cbPlayerCount == 7 || cbBankerCount == 7)
	{
		pingPoint = 11;
		pingType = 7;
		cbMultiple[RBAREA_PING] = 11;
	}
	else if (cbPlayerCount == 6 || cbBankerCount == 6)
	{
		pingPoint = 6;
		pingType = 6;
		cbMultiple[RBAREA_PING] = 6;
	}
	else if (cbPlayerCount == 5 || cbBankerCount == 5)
	{
		pingPoint = 4;
		pingType = 5;
		cbMultiple[RBAREA_PING] = 4;
	}
	else if (cbPlayerCount == 4 || cbBankerCount == 4)
	{
		pingPoint = 3;
		pingType = 4;
		cbMultiple[RBAREA_PING] = 3;
	}
	else if (cbPlayerCount == 3 || cbBankerCount == 3)
	{
		pingPoint = 2;
		pingType = 3;
		cbMultiple[RBAREA_PING] = 2;
	}
	else if (cbPlayerCount == 2 || cbBankerCount == 2)
	{
		pingType = 2;
		cbMultiple[RBAREA_PING] = 2;
	}
	else
	{
		pingType = 1;
	}


	//庄家是不是机器人
	bool bIsBankerAndroidUser = false;
	if (ScoreParameter.wBankerUser != INVALID_CHAIR)
	{
		IServerUserItem * pIBankerUserItem = m_pITableFrame->GetTableUserItem(ScoreParameter.wBankerUser);
		if (pIBankerUserItem != NULL)
		{
			bIsBankerAndroidUser = pIBankerUserItem->IsAndroidUser();
		}
	}

	//180820计算积分
	for (WORD wChairID = 0; wChairID < REDBLACK_PLAYER; wChairID++)
	{
		//庄家判断
		if (ScoreParameter.wBankerUser == wChairID) continue;

		//获取用户
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem == NULL) continue;

		bool bIsAndroidUser = pIServerUserItem->IsAndroidUser();

		for (WORD wAreaIndex = 0; wAreaIndex < RBAREA_MAX; ++wAreaIndex)
		{

			if (cbWinArea[wAreaIndex] == TRUE)
			{
				//系统得分
				if (bIsAndroidUser)
					lSystemScore += (ScoreParameter.lPlayBet[wChairID][wAreaIndex] * (cbMultiple[wAreaIndex] - 1));
				if (ScoreParameter.wBankerUser == INVALID_CHAIR || bIsBankerAndroidUser)
					lSystemScore -= (ScoreParameter.lPlayBet[wChairID][wAreaIndex] * (cbMultiple[wAreaIndex] - 1));
			}
			else
			{
				//系统得分
				if (bIsAndroidUser)
					lSystemScore -= ScoreParameter.lPlayBet[wChairID][wAreaIndex];
				if (ScoreParameter.wBankerUser == INVALID_CHAIR || bIsBankerAndroidUser)
					lSystemScore += ScoreParameter.lPlayBet[wChairID][wAreaIndex];
			}
		}
	}

	return lSystemScore;
}

//龙虎控制结果
VOID * CServerControlItemSink::OnLongHuDouControlResult(VOID * pData, WORD wDataSize, VOID * pResult)
{
	if (wDataSize != sizeof(tag_LongHu_Parameter))
		return NULL;

	tag_LongHu_Parameter *pLongHu_Parameter = (tag_LongHu_Parameter *)pData;
	bool bSuccess = true;

	tag_LongHu_Result * LongHu_Result = (tag_LongHu_Result *)pResult;

	//局控
	if (pLongHu_Parameter->cbJuControl == TRUE)
	{
		//计算分数
		BYTE cbDispatchCount = 0;

		do
		{
			bSuccess = true;

			//派发扑克
			DispatchLongHuCard(LongHu_Result);

			BYTE cbWinArea[LHAREA_MAX] = { FALSE };
			
			BYTE cbWin = m_LongHuLogic.CompareCard(LongHu_Result->cbTableCardArray[INDEX_PLAYER], LongHu_Result->cbTableCardArray[INDEX_BANKER], MAX_COUNT);

			if (cbWin == 1)
				cbWinArea[LHAREA_DRAGON] = TRUE;
			else if (cbWin == 2)
				cbWinArea[LHAREA_TIGER] = TRUE;
			else
				cbWinArea[LHAREA_PING] = TRUE;

			//判断庄闲平
			for (BYTE i = 0; i < LHAREA_MAX; i++)
			{
				if ((pLongHu_Parameter->cbJuControlArea[i] == 1 && cbWinArea[i] == FALSE) || (pLongHu_Parameter->cbJuControlArea[i] == 2 && cbWinArea[i] == TRUE))
				{
					bSuccess = false;
					break;
				}
			}

			cbDispatchCount++;

			if (cbDispatchCount > 200)
				break;

		} while (!bSuccess);
	}
	//库存控制判断
	else
	{
		BYTE bControlType = 0;	//0不控制，1控制系统赢:概率区间101~200，2控制系统输概率区间1~100
		SCORE lCurrentStorage = m_pGameServiceOption->pRoomStorageOption.lStorageCurrent;
		SCORE lStorageMax = m_pGameServiceOption->pRoomStorageOption.lStorageMax;
		int lStorageMul = m_pGameServiceOption->pRoomStorageOption.lStorageMul;
		SCORE lMinStorage[STORAGE_COUNT] = { 0 };
		SCORE lMaxStorage[STORAGE_COUNT] = { 0 };
		int nMulStorage[STORAGE_COUNT] = { 0 };

		TCHAR outBuff[64] = { 0 };
		for (int i = 0; i < STORAGE_COUNT; i++)
		{
			memset(outBuff, 0, sizeof(outBuff));
			_sntprintf_s(outBuff, sizeof(outBuff), _T("%s"), m_pGameServiceOption->pRoomStorageOption.szStorageControl[i]);

			SCORE llValue[3] = { 0 };
			this->GetValueFromCombStr(llValue, CountArray(llValue), outBuff, _tcslen(outBuff));
			lMinStorage[i] = llValue[0];
			lMaxStorage[i] = llValue[1];
			nMulStorage[i] = llValue[2];
		}

		int nControlIndex = 0;
		for (; nControlIndex < STORAGE_COUNT; nControlIndex++)
		{
			if (lCurrentStorage > lMinStorage[nControlIndex] && lCurrentStorage <= lMaxStorage[nControlIndex])
				break;
		}

		//进入库存控制
		if ((nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] > 100) || lCurrentStorage < 0)
		{
			if ((rand() % 100) < (nMulStorage[nControlIndex] - 100) || lCurrentStorage < 0)
			{
				bControlType = 1;
			}
		}

		// 送分判断
		bool bStorageSend = nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] <= 100 && nMulStorage[nControlIndex] > 0 && (rand() % 100) < nMulStorage[nControlIndex];
		if ((lCurrentStorage > 0 && lCurrentStorage > lStorageMax && rand() % 100 < lStorageMul) || bStorageSend)
		{
			bControlType = 2;
		}

		BYTE cbRealCount = 0;
		BYTE cbAndroidCount = 0;
		for (WORD i = 0; i < LONGHU_PLAYER; i++)
		{
			IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
			if (pServerUser)
			{
				if (!pServerUser->IsAndroidUser())
					cbRealCount++;
				else
					cbAndroidCount++;
			}
		}

		//如果没有真人就不控制
		if (cbRealCount == 0)
			bControlType = 0;

		//计算分数
		BYTE cbDispatchCount = 0;
		do
		{

			//派发扑克
			DispatchLongHuCard(LongHu_Result);

			tag_LongHu_ScoreParameter ScoreParameter;
			ScoreParameter.wBankerUser = pLongHu_Parameter->wBankerUser;
			CopyMemory(ScoreParameter.lPlayBet, pLongHu_Parameter->lPlayBet, sizeof(ScoreParameter.lPlayBet));
			CopyMemory(ScoreParameter.cbTableCard, LongHu_Result->cbTableCardArray, sizeof(ScoreParameter.cbTableCard));

			//试探性判断
			SCORE lSystemScore = CalculateLongHuScore(ScoreParameter);
			cbDispatchCount++;

			bSuccess = true;

			if (bControlType == 1 && lSystemScore <= 0)
				bSuccess = false;
			if (bControlType == 2 && lSystemScore >= 0)
				bSuccess = false;

			if ((lSystemScore + lCurrentStorage) < 0 && lSystemScore < 0)
				bSuccess = false;

			if (cbDispatchCount > 100)
				break;
		} while (!bSuccess);
	}

	return LongHu_Result;
}

//龙虎洗牌
void CServerControlItemSink::DispatchLongHuCard(tag_LongHu_Result *LongHu_Result)
{
	//随机扑克
	m_LongHuLogic.RandCardList(&LongHu_Result->cbTableCardArray[0], sizeof(LongHu_Result->cbTableCardArray) / sizeof(LongHu_Result->cbTableCardArray[0]));
}
//龙虎计算系统分
SCORE CServerControlItemSink::CalculateLongHuScore(tag_LongHu_ScoreParameter ScoreParameter)
{
	//计算牌点
	BYTE cbPlayerCount = 1;
	BYTE cbBankerCount = 1;

	//系统输赢
	SCORE lSystemScore = 0;

	//推断玩家
	BYTE cbWinArea[LHAREA_MAX] = { FALSE };
	
	BYTE cbWin = m_LongHuLogic.CompareCard(ScoreParameter.cbTableCard[INDEX_PLAYER], ScoreParameter.cbTableCard[INDEX_BANKER], 1);

	if (cbWin == 1)
		cbWinArea[LHAREA_DRAGON] = TRUE;
	else if (cbWin == 2)
		cbWinArea[LHAREA_TIGER] = TRUE;
	else
		cbWinArea[LHAREA_PING] = TRUE;

	//区域倍率
	BYTE cbMultiple[LHAREA_MAX] = { 2, 2, 17 };

	//庄家是不是机器人
	bool bIsBankerAndroidUser = false;
	if (ScoreParameter.wBankerUser != INVALID_CHAIR)
	{
		IServerUserItem * pIBankerUserItem = m_pITableFrame->GetTableUserItem(ScoreParameter.wBankerUser);
		if (pIBankerUserItem != NULL)
		{
			bIsBankerAndroidUser = pIBankerUserItem->IsAndroidUser();
		}
	}

	//计算积分
	for (WORD wChairID = 0; wChairID < LONGHU_PLAYER; wChairID++)
	{
		//庄家判断
		if (ScoreParameter.wBankerUser == wChairID) continue;

		//获取用户
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem == NULL) continue;

		bool bIsAndroidUser = pIServerUserItem->IsAndroidUser();

		for (WORD wAreaIndex = 0; wAreaIndex < LHAREA_MAX; ++wAreaIndex)
		{
			if (cbWinArea[wAreaIndex] == TRUE)
			{
				//系统或者机器人庄
				if (ScoreParameter.wBankerUser == INVALID_CHAIR || bIsBankerAndroidUser)
				{
					if (!bIsAndroidUser)
						lSystemScore -= ScoreParameter.lPlayBet[wChairID][wAreaIndex];
				}
				//玩家庄
				else
				{
					if (bIsAndroidUser)
						lSystemScore += ScoreParameter.lPlayBet[wChairID][wAreaIndex];
				}

			}
			else
			{
				SCORE lBetScore = ScoreParameter.lPlayBet[wChairID][wAreaIndex];

				if (wAreaIndex != LHAREA_PING && cbWinArea[LHAREA_PING] == TRUE)
					lBetScore = 0;

				//系统得分
				//系统或者机器人庄
				if (ScoreParameter.wBankerUser == INVALID_CHAIR || bIsBankerAndroidUser)
				{
					if (!bIsAndroidUser)
						lSystemScore += lBetScore;
				}
				//玩家庄
				else
				{
					if (bIsAndroidUser)
						lSystemScore -= lBetScore;
				}
			}
		}

	}


	return lSystemScore;
}

//百人牛牛控制结果
VOID * CServerControlItemSink::OnOxBattleControlResult(VOID * pData, WORD wDataSize, VOID * pResult)
{
	if (wDataSize != sizeof(tag_OxBattle_Parameter))
		return NULL;

	tag_OxBattle_Parameter *pOxBattle_Parameter = (tag_OxBattle_Parameter *)pData;
	bool bSuccess = true;

	tag_OxBattle_Result * OxBattle_Result = (tag_OxBattle_Result *)pResult;

	//游戏作弊
	if (pOxBattle_Parameter->cbJuControl == TRUE)
	{
		DispatchOxBattleCard(OxBattle_Result);
		//循环计数
		LONGLONG lCirculateCount = 0L;
		while (true)
		{
			if (lCirculateCount > 100)
			{
				break;
			}

			bool bOK = true;

			bool bXianWin[4] = { false, false, false, false };							//比较输赢
			BYTE bMultiple[4] = { 1, 1, 1, 1 };											//比较倍数
			for (int i = 0; i < 4; i++)
			{
				bXianWin[i] = (m_OxBattleLogic.CompareCard(OxBattle_Result->cbTableCardArray[0], 5, OxBattle_Result->cbTableCardArray[i + 1], 5, m_pITableFrame->GetGameServiceOption()->wServerLevel, bMultiple[i]) == 1);
			}

			BYTE cbAreaControl[4] = { pOxBattle_Parameter->cbJuControlArea[1], pOxBattle_Parameter->cbJuControlArea[2], pOxBattle_Parameter->cbJuControlArea[3], pOxBattle_Parameter->cbJuControlArea[4] };

			for (BYTE i = 0; i < 4; i++)
			{
				if (cbAreaControl[i] == 0)
					continue;

				if ((cbAreaControl[i] == 1 && bXianWin[i]) || (cbAreaControl[i] == 2 && !bXianWin[i]))
					continue;

				bOK = false;
				break;
			}

			//庄家赢
			if (pOxBattle_Parameter->cbJuControlArea[0] == 1)
			{
				tag_OxBattle_ScoreParameter ScoreParameter;
				ScoreParameter.wBankerUser = pOxBattle_Parameter->wBankerUser;
				CopyMemory(ScoreParameter.lPlayBet, pOxBattle_Parameter->lPlayBet, sizeof(ScoreParameter.lPlayBet));
				CopyMemory(ScoreParameter.cbTableCard, OxBattle_Result->cbTableCardArray, sizeof(ScoreParameter.cbTableCard));

				DOUBLE lBankerScore = CalculateOxBattleBankerScore(ScoreParameter);

				if (lBankerScore < 0)
				{
					bOK = false;
				}

			}
			//庄家输
			else if (pOxBattle_Parameter->cbJuControlArea[0] == 2)
			{
				tag_OxBattle_ScoreParameter ScoreParameter;
				ScoreParameter.wBankerUser = pOxBattle_Parameter->wBankerUser;
				CopyMemory(ScoreParameter.lPlayBet, pOxBattle_Parameter->lPlayBet, sizeof(ScoreParameter.lPlayBet));
				CopyMemory(ScoreParameter.cbTableCard, OxBattle_Result->cbTableCardArray, sizeof(ScoreParameter.cbTableCard));

				DOUBLE lBankerScore = CalculateOxBattleBankerScore(ScoreParameter);

				if (lBankerScore > 0)
				{
					bOK = false;
				}
			}

			if (!bOK)
			{
				DispatchOxBattleCard(OxBattle_Result);
				lCirculateCount++;
				continue;
			}
			else
				break;
		}

	}
	//库存控制判断
	else
	{
		BYTE bControlType = 0;	//0不控制，1控制系统赢:概率区间101~200，2控制系统输概率区间1~100
		SCORE lCurrentStorage = m_pGameServiceOption->pRoomStorageOption.lStorageCurrent;
		SCORE lStorageMax = m_pGameServiceOption->pRoomStorageOption.lStorageMax;
		int lStorageMul = m_pGameServiceOption->pRoomStorageOption.lStorageMul;
		SCORE lMinStorage[STORAGE_COUNT] = { 0 };
		SCORE lMaxStorage[STORAGE_COUNT] = { 0 };
		int nMulStorage[STORAGE_COUNT] = { 0 };

		TCHAR outBuff[64] = { 0 };
		for (int i = 0; i < STORAGE_COUNT; i++)
		{
			memset(outBuff, 0, sizeof(outBuff));
			_sntprintf_s(outBuff, sizeof(outBuff), _T("%s"), m_pGameServiceOption->pRoomStorageOption.szStorageControl[i]);

			SCORE llValue[3] = { 0 };
			this->GetValueFromCombStr(llValue, CountArray(llValue), outBuff, _tcslen(outBuff));
			lMinStorage[i] = llValue[0];
			lMaxStorage[i] = llValue[1];
			nMulStorage[i] = llValue[2];
		}

		int nControlIndex = 0;
		for (; nControlIndex < STORAGE_COUNT; nControlIndex++)
		{
			if (lCurrentStorage > lMinStorage[nControlIndex] && lCurrentStorage <= lMaxStorage[nControlIndex])
				break;
		}

		//进入库存控制
		if ((nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] > 100) || lCurrentStorage < 0)
		{
			if ((rand() % 100) < (nMulStorage[nControlIndex] - 100) || lCurrentStorage < 0)
			{
				bControlType = 1;
			}
		}

		// 送分判断
		bool bStorageSend = nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] <= 100 && nMulStorage[nControlIndex] > 0 && (rand() % 100) < nMulStorage[nControlIndex];
		if ((lCurrentStorage > 0 && lCurrentStorage > lStorageMax && rand() % 100 < lStorageMul) || bStorageSend)
		{
			bControlType = 2;
		}

		BYTE cbRealCount = 0;
		BYTE cbAndroidCount = 0;
		for (WORD i = 0; i < OXBATTLE_PLAYER; i++)
		{
			IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
			if (pServerUser)
			{
				if (!pServerUser->IsAndroidUser())
					cbRealCount++;
				else
					cbAndroidCount++;
			}
		}

		//如果没有真人就不控制
		if (cbRealCount == 0)
			bControlType = 0;

		//循环计数
		LONGLONG lCirculateCount = 0L;
		while (true)
		{
			DispatchOxBattleCard(OxBattle_Result);

			tag_OxBattle_ScoreParameter ScoreParameter;
			ScoreParameter.wBankerUser = pOxBattle_Parameter->wBankerUser;
			CopyMemory(ScoreParameter.lPlayBet, pOxBattle_Parameter->lPlayBet, sizeof(ScoreParameter.lPlayBet));
			CopyMemory(ScoreParameter.cbTableCard, OxBattle_Result->cbTableCardArray, sizeof(ScoreParameter.cbTableCard));

			//真人玩家得分
			SCORE lSystemScore = CalculateOxBattleScore(ScoreParameter);
			lCirculateCount++;

			bool bSuccess = true;

			if (bControlType == 1 && lSystemScore <= 0)
				bSuccess = false;
			if (bControlType == 2 && lSystemScore >= 0)
				bSuccess = false;

			if ((lSystemScore + lCurrentStorage) < 0 && lSystemScore < 0)
				bSuccess = false;

			if (lCirculateCount > 100 || bSuccess)
			{
				break;
			}
		}
	}


	return OxBattle_Result;
}

//百人牛牛洗牌
void CServerControlItemSink::DispatchOxBattleCard(tag_OxBattle_Result *OxBattle_Result)
{

	BYTE cbCardData[CountArray(m_OxBattleLogic.m_cbCardListData)];
	CopyMemory(cbCardData, m_OxBattleLogic.m_cbCardListData, sizeof(m_OxBattleLogic.m_cbCardListData));
	
	BYTE cbRestCardCount = 25;//五个方位每个地方随机三张牌

	BYTE cbRandCount = 0, cbPosition = 0;

	for (BYTE i = 0; i < cbRestCardCount; i++)
	{
		cbPosition = rand() % (52 - cbRandCount);
		cbRandCount++;
		WORD wChair = i / 5;
		BYTE cbCardIndex = i % 5;
		OxBattle_Result->cbTableCardArray[wChair][cbCardIndex] = cbCardData[cbPosition];
		cbCardData[cbPosition] = cbCardData[52 - cbRandCount];
	}
}

//百人牛牛计算系统分
SCORE CServerControlItemSink::CalculateOxBattleScore(tag_OxBattle_ScoreParameter ScoreParameter)
{
	SCORE lSystemScore = 0;

	//比较倍数
	bool bXianWin[4] = { false, false, false, false };							//比较输赢
	BYTE bMultiple[4] = { 1, 1, 1, 1 };											//比较倍数
	for (int i = 0; i < 4; i++)
	{
		bXianWin[i] = (m_OxBattleLogic.CompareCard(ScoreParameter.cbTableCard[0], 5, ScoreParameter.cbTableCard[i + 1], 5, m_pITableFrame->GetGameServiceOption()->wServerLevel, bMultiple[i]) == 1);
	}

	//系统当庄或者机器人当庄
	if ((ScoreParameter.wBankerUser != INVALID_CHAIR && m_pITableFrame->GetTableUserItem(ScoreParameter.wBankerUser)->IsAndroidUser() == true) || (ScoreParameter.wBankerUser == INVALID_CHAIR))
	{
		// 计算系统输赢
		for (WORD wChairID = 0; wChairID < OXBATTLE_PLAYER; wChairID++)
		{
			// 获取玩家
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);

			// 过滤庄家
			if (wChairID == ScoreParameter.wBankerUser || pIServerUserItem == NULL || pIServerUserItem->IsAndroidUser() == true)
			{
				continue;
			}

			// 计算玩家下注
			for (int nAarea = 0; nAarea < 4; nAarea++)
			{
				if (ScoreParameter.lPlayBet[wChairID][nAarea + 1] != 0)
				{
					if (bXianWin[nAarea])
					{
						lSystemScore -= ScoreParameter.lPlayBet[wChairID][nAarea + 1] * bMultiple[nAarea];
					}
					else
					{
						lSystemScore += ScoreParameter.lPlayBet[wChairID][nAarea + 1] * bMultiple[nAarea];
					}
				}
			}
		}
	}
	//玩家当庄
	else
	{
		// 计算系统输赢
		for (WORD wChairID = 0; wChairID < OXBATTLE_PLAYER; wChairID++)
		{
			// 获取玩家
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);

			// 过滤庄家
			if (wChairID == ScoreParameter.wBankerUser || pIServerUserItem == NULL || pIServerUserItem->IsAndroidUser() == false)
			{
				continue;
			}

			// 计算玩家下注
			for (int nAarea = 0; nAarea < 4; nAarea++)
			{
				if (ScoreParameter.lPlayBet[wChairID][nAarea + 1] != 0)
				{
					if (bXianWin[nAarea])
					{
						lSystemScore += ScoreParameter.lPlayBet[wChairID][nAarea + 1] * bMultiple[nAarea];
					}
					else
					{
						lSystemScore -= ScoreParameter.lPlayBet[wChairID][nAarea + 1] * bMultiple[nAarea];
					}
				}
			}
		}
	}

	return lSystemScore;
}

//百人牛牛计算庄家输赢分
SCORE CServerControlItemSink::CalculateOxBattleBankerScore(tag_OxBattle_ScoreParameter ScoreParameter)
{
	SCORE lBankerScore = 0;

	//比较倍数
	bool bXianWin[4] = { false, false, false, false };							//比较输赢
	BYTE bMultiple[4] = { 1, 1, 1, 1 };											//比较倍数
	for (int i = 0; i < 4; i++)
	{
		bXianWin[i] = (m_OxBattleLogic.CompareCard(ScoreParameter.cbTableCard[0], 5, ScoreParameter.cbTableCard[i + 1], 5, m_pITableFrame->GetGameServiceOption()->wServerLevel, bMultiple[i]) == 1);
	}

	// 计算庄家输赢
	for (WORD wChairID = 0; wChairID < OXBATTLE_PLAYER; wChairID++)
	{
		// 获取玩家
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);

		// 过滤庄家
		if (wChairID == ScoreParameter.wBankerUser || pIServerUserItem == NULL)
		{
			continue;
		}

		// 计算玩家下注
		for (int nAarea = 0; nAarea < 4; nAarea++)
		{
			if (ScoreParameter.lPlayBet[wChairID][nAarea + 1] != 0)
			{
				if (bXianWin[nAarea])
				{
					lBankerScore -= ScoreParameter.lPlayBet[wChairID][nAarea + 1] * bMultiple[nAarea];
				}
				else
				{
					lBankerScore += ScoreParameter.lPlayBet[wChairID][nAarea + 1] * bMultiple[nAarea];
				}
			}
		}
	}
	

	return lBankerScore;
}

//斗地主控制结果
VOID * CServerControlItemSink::OnLandControlResult(VOID * pData, WORD wDataSize, VOID * pResult)
{
	tag_Land_Result * Land_Result = (tag_Land_Result *)pResult;

	//机器人数
	WORD wAiCount = 0;
	WORD wPlayerCount = 0;
	for (WORD i = 0; i<LAND_GAME_PLAYER; i++)
	{
		//获取用户
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem != NULL)
		{
			if (pIServerUserItem->IsAndroidUser())
			{
				wAiCount++;
			}
			wPlayerCount++;
		}
	}

	//全部机器
	if (wPlayerCount == wAiCount || wAiCount == 0) return NULL;

	BYTE bControlType = 0;	//0不控制，1控制系统赢:概率区间101~200，2控制系统输概率区间1~100
	SCORE lCurrentStorage = m_pGameServiceOption->pRoomStorageOption.lStorageCurrent;
	SCORE lStorageMax = m_pGameServiceOption->pRoomStorageOption.lStorageMax;
	int lStorageMul = m_pGameServiceOption->pRoomStorageOption.lStorageMul;
	SCORE lMinStorage[STORAGE_COUNT] = { 0 };
	SCORE lMaxStorage[STORAGE_COUNT] = { 0 };
	int nMulStorage[STORAGE_COUNT] = { 0 };

	TCHAR outBuff[64] = { 0 };
	for (int i = 0; i < STORAGE_COUNT; i++)
	{
		memset(outBuff, 0, sizeof(outBuff));
		_sntprintf_s(outBuff, sizeof(outBuff), _T("%s"), m_pGameServiceOption->pRoomStorageOption.szStorageControl[i]);

		SCORE llValue[3] = { 0 };
		this->GetValueFromCombStr(llValue, CountArray(llValue), outBuff, _tcslen(outBuff));
		lMinStorage[i] = llValue[0];
		lMaxStorage[i] = llValue[1];
		nMulStorage[i] = llValue[2];
	}

	int nControlIndex = 0;
	for (; nControlIndex < STORAGE_COUNT; nControlIndex++)
	{
		if (lCurrentStorage > lMinStorage[nControlIndex] && lCurrentStorage <= lMaxStorage[nControlIndex])
			break;
	}

	WORD wGoodCardUser = INVALID_CHAIR;

	//进入库存控制
	if ((nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] > 100) || lCurrentStorage < 0)
	{
		//系统赢判断
		if ((rand() % 100) < (nMulStorage[nControlIndex] - 100) || lCurrentStorage < 0)
		{
			for (WORD i = 0; i < LAND_GAME_PLAYER; i++)
			{
				IServerUserItem *pUser = m_pITableFrame->GetTableUserItem(i);
				if (pUser && pUser->IsAndroidUser())
				{
					wGoodCardUser = i;
					break;
				}
			}
		}
	}

	// 送分判断
	bool bStorageSend = nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] <= 100 && nMulStorage[nControlIndex] > 0 && (rand() % 100) < nMulStorage[nControlIndex];
	if ((lCurrentStorage > 0 && lCurrentStorage > lStorageMax && rand() % 100 < lStorageMul) || bStorageSend)
	{
		for (WORD i = 0; i < LAND_GAME_PLAYER; i++)
		{
			IServerUserItem *pUser = m_pITableFrame->GetTableUserItem(i);
			if (pUser && !pUser->IsAndroidUser())
			{
				wGoodCardUser = i;
				break;
			}
		}
	}

	if (wGoodCardUser != INVALID_CHAIR)
	{
		BYTE cbGoodCardData[LAND_NORMAL_COUNT] = { 0 };
		BYTE cbAllRandCardData[LAND_FULL_COUNT] = { 0 };
		BYTE cbRandedCard[LAND_FULL_COUNT] = { 0 };
		m_LandLogic.RandCardList(cbAllRandCardData, CountArray(cbAllRandCardData));
		m_LandLogic.RandGoodCardList(cbGoodCardData, LAND_NORMAL_COUNT);

		m_LandLogic.RemoveCard(cbGoodCardData, LAND_NORMAL_COUNT, cbAllRandCardData, LAND_FULL_COUNT);

		for (BYTE i = 0; i < LAND_FULL_COUNT; i++)
		{
			if (i >= wGoodCardUser*LAND_NORMAL_COUNT && i < (wGoodCardUser + 1)*LAND_NORMAL_COUNT)
				cbRandedCard[i] = cbGoodCardData[i - wGoodCardUser*LAND_NORMAL_COUNT];
			else if (i < wGoodCardUser*LAND_NORMAL_COUNT)
				cbRandedCard[i] = cbAllRandCardData[i];
			else if (i >= (wGoodCardUser + 1)*LAND_NORMAL_COUNT)
				cbRandedCard[i] = cbAllRandCardData[i - LAND_NORMAL_COUNT];
		}

		CopyMemory(Land_Result->cbTableCardArray, cbRandedCard, sizeof(cbRandedCard));
	}
	else
	{
		m_LandLogic.RandCardList(Land_Result->cbTableCardArray, CountArray(Land_Result->cbTableCardArray));
	}


	return Land_Result;
}

//扎金花控制结果
VOID * CServerControlItemSink::OnZhaJinHuaControlResult(VOID * pData, WORD wDataSize, VOID * pResult)
{
	tag_ZhaJinHua_Result * ZhaJinHua_Result = (tag_ZhaJinHua_Result *)pResult;

	m_ZhaJinHuaLogic.RandCardList(ZhaJinHua_Result->cbTableCardArray[0], sizeof(ZhaJinHua_Result->cbTableCardArray) / sizeof(ZhaJinHua_Result->cbTableCardArray[0][0]));

	//机器人数
	WORD wAiCount = 0;
	WORD wPlayerCount = 0;
	BYTE cbPlayStatus[ZHAJINHUA_GAME_PLAYER] = {FALSE};
	for (WORD i = 0; i<ZHAJINHUA_GAME_PLAYER; i++)
	{
		//获取用户
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem != NULL)
		{
			cbPlayStatus[i] = TRUE;
			if (pIServerUserItem->IsAndroidUser())
			{
				wAiCount++;
			}
			wPlayerCount++;
		}
	}

	//全部机器
	if (wPlayerCount == wAiCount || wAiCount == 0) return NULL;

	//扑克变量
	BYTE cbUserCardData[ZHAJINHUA_GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData, ZhaJinHua_Result->cbTableCardArray, sizeof(ZhaJinHua_Result->cbTableCardArray));

	//排列扑克
	for (WORD i = 0; i<ZHAJINHUA_GAME_PLAYER; i++)
	{
		m_ZhaJinHuaLogic.SortCardList(cbUserCardData[i], MAX_COUNT);
	}

	//变量定义
	WORD wMaxChairID = INVALID_CHAIR;

	//查找数据
	for (WORD i = 0; i < ZHAJINHUA_GAME_PLAYER; i++)
	{
		//用户过滤
		if (cbPlayStatus[i] == FALSE) continue;

		//设置用户
		if (wMaxChairID == INVALID_CHAIR)
		{
			wMaxChairID = i;
			continue;
		}

		//对比扑克
		if (m_ZhaJinHuaLogic.CompareCard(cbUserCardData[i], cbUserCardData[wMaxChairID], MAX_COUNT) >= TRUE)
		{
			wMaxChairID = i;
		}
	}

	BYTE bControlType = 0;	//0不控制，1控制系统赢:概率区间101~200，2控制系统输概率区间1~100
	SCORE lCurrentStorage = m_pGameServiceOption->pRoomStorageOption.lStorageCurrent;
	SCORE lStorageMax = m_pGameServiceOption->pRoomStorageOption.lStorageMax;
	int lStorageMul = m_pGameServiceOption->pRoomStorageOption.lStorageMul;
	SCORE lMinStorage[STORAGE_COUNT] = { 0 };
	SCORE lMaxStorage[STORAGE_COUNT] = { 0 };
	int nMulStorage[STORAGE_COUNT] = { 0 };

	TCHAR outBuff[64] = { 0 };
	for (int i = 0; i < STORAGE_COUNT; i++)
	{
		memset(outBuff, 0, sizeof(outBuff));
		_sntprintf_s(outBuff, sizeof(outBuff), _T("%s"), m_pGameServiceOption->pRoomStorageOption.szStorageControl[i]);

		SCORE llValue[3] = { 0 };
		this->GetValueFromCombStr(llValue, CountArray(llValue), outBuff, _tcslen(outBuff));
		lMinStorage[i] = llValue[0];
		lMaxStorage[i] = llValue[1];
		nMulStorage[i] = llValue[2];
	}

	int nControlIndex = 0;
	for (; nControlIndex < STORAGE_COUNT; nControlIndex++)
	{
		if (lCurrentStorage > lMinStorage[nControlIndex] && lCurrentStorage <= lMaxStorage[nControlIndex])
			break;
	}

	//进入库存控制
	if ((nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] > 100) || lCurrentStorage < 0)
	{
		//系统赢判断
		if ((rand() % 100) < (nMulStorage[nControlIndex] - 100) || lCurrentStorage < 0)
		{
			IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(wMaxChairID);
			if (pServerUser && !pServerUser->IsAndroidUser())
			{
				WORD wNextAndroid = INVALID_CHAIR;
				for (WORD i = 1; i < ZHAJINHUA_GAME_PLAYER; i++)
				{
					wNextAndroid = (wMaxChairID + i) % ZHAJINHUA_GAME_PLAYER;
					if (cbPlayStatus[wNextAndroid] == FALSE) continue;
					IServerUserItem *pNextUser = m_pITableFrame->GetTableUserItem(wNextAndroid);
					if (pNextUser && pNextUser->IsAndroidUser())
						break;
				}

				if (cbPlayStatus[wNextAndroid] == TRUE)
				{
					BYTE cbTempCard[MAX_COUNT];
					ZeroMemory(&cbTempCard, sizeof(cbTempCard));

					CopyMemory(cbTempCard, ZhaJinHua_Result->cbTableCardArray[wNextAndroid], sizeof(cbTempCard));
					CopyMemory(ZhaJinHua_Result->cbTableCardArray[wNextAndroid], ZhaJinHua_Result->cbTableCardArray[wMaxChairID], sizeof(ZhaJinHua_Result->cbTableCardArray[wNextAndroid]));
					CopyMemory(ZhaJinHua_Result->cbTableCardArray[wMaxChairID], cbTempCard, sizeof(ZhaJinHua_Result->cbTableCardArray[wMaxChairID]));
				}
			}
		}
	}

	// 送分判断
	bool bStorageSend = nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] <= 100 && nMulStorage[nControlIndex] > 0 && (rand() % 100) < nMulStorage[nControlIndex];
	if ((lCurrentStorage > 0 && lCurrentStorage > lStorageMax && rand() % 100 < lStorageMul) || bStorageSend)
	{
		IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(wMaxChairID);
		if (pServerUser && pServerUser->IsAndroidUser())
		{
			WORD wNextChair = INVALID_CHAIR;
			for (WORD i = 1; i < ZHAJINHUA_GAME_PLAYER; i++)
			{
				wNextChair = (wMaxChairID + i) % ZHAJINHUA_GAME_PLAYER;
				if (cbPlayStatus[wNextChair] == FALSE) continue;
				IServerUserItem *pNextUser = m_pITableFrame->GetTableUserItem(wNextChair);
				if (pNextUser && !pNextUser->IsAndroidUser())
					break;
			}

			if (cbPlayStatus[wNextChair] == TRUE)
			{
				BYTE cbTempCard[MAX_COUNT];
				ZeroMemory(&cbTempCard, sizeof(cbTempCard));

				CopyMemory(cbTempCard, ZhaJinHua_Result->cbTableCardArray[wNextChair], sizeof(cbTempCard));
				CopyMemory(ZhaJinHua_Result->cbTableCardArray[wNextChair], ZhaJinHua_Result->cbTableCardArray[wMaxChairID], sizeof(ZhaJinHua_Result->cbTableCardArray[wNextChair]));
				CopyMemory(ZhaJinHua_Result->cbTableCardArray[wMaxChairID], cbTempCard, sizeof(ZhaJinHua_Result->cbTableCardArray[wMaxChairID]));
			}
		}
	}

	return ZhaJinHua_Result;
}

//抢庄牛牛控制结果
VOID * CServerControlItemSink::OnOxSixControlResult(VOID * pData, WORD wDataSize, VOID * pResult)
{
	if (wDataSize != sizeof(tag_OxSix_Parameter))
		return NULL;

	tag_OxSix_Parameter *pOxSix_Parameter = (tag_OxSix_Parameter *)pData;

	tag_OxSix_Result * OxSix_Result = (tag_OxSix_Result *)pResult;

	CopyMemory(OxSix_Result->cbTableCardArray, pOxSix_Parameter->cbTableCardArray, sizeof(OxSix_Result->cbTableCardArray));

	//机器人数
	bool bIsAiBanker = false;
	WORD wAiCount = 0;
	WORD wPlayerCount = 0;
	for (WORD i = 0; i<OXSIX_GAME_PLAYER; i++)
	{
		//获取用户
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem != NULL)
		{
			if (pOxSix_Parameter->cbPlayerStatus[i] == FALSE)continue;
			if (pIServerUserItem->IsAndroidUser())
			{
				wAiCount++;
				if (!bIsAiBanker && i == pOxSix_Parameter->wBankerUser)bIsAiBanker = true;
			}
			wPlayerCount++;
		}
	}

	//全部机器
	if (wPlayerCount == wAiCount || wAiCount == 0)
	{
		return NULL;
	}

	BYTE bControlType = 0;	//0不控制，1控制系统赢:概率区间101~200，2控制系统输概率区间1~100
	SCORE lCurrentStorage = m_pGameServiceOption->pRoomStorageOption.lStorageCurrent;
	SCORE lStorageMax = m_pGameServiceOption->pRoomStorageOption.lStorageMax;
	int lStorageMul = m_pGameServiceOption->pRoomStorageOption.lStorageMul;
	SCORE lMinStorage[STORAGE_COUNT] = { 0 };
	SCORE lMaxStorage[STORAGE_COUNT] = { 0 };
	int nMulStorage[STORAGE_COUNT] = { 0 };

	TCHAR outBuff[64] = { 0 };
	for (int i = 0; i < STORAGE_COUNT; i++)
	{
		memset(outBuff, 0, sizeof(outBuff));
		_sntprintf_s(outBuff, sizeof(outBuff), _T("%s"), m_pGameServiceOption->pRoomStorageOption.szStorageControl[i]);

		SCORE llValue[3] = { 0 };
		this->GetValueFromCombStr(llValue, CountArray(llValue), outBuff, _tcslen(outBuff));
		lMinStorage[i] = llValue[0];
		lMaxStorage[i] = llValue[1];
		nMulStorage[i] = llValue[2];
	}

	int nControlIndex = 0;
	for (; nControlIndex < STORAGE_COUNT; nControlIndex++)
	{
		if (lCurrentStorage > lMinStorage[nControlIndex] && lCurrentStorage <= lMaxStorage[nControlIndex])
			break;
	}

	//进入库存控制
	if ((nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] > 100) || lCurrentStorage < 0)
	{
		if ((rand() % 100) < (nMulStorage[nControlIndex] - 100) || lCurrentStorage < 0)
		{
			bControlType = 1;
		}
	}

	// 送分判断
	bool bStorageSend = nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] <= 100 && nMulStorage[nControlIndex] > 0 && (rand() % 100) < nMulStorage[nControlIndex];
	if ((lCurrentStorage > 0 && lCurrentStorage > lStorageMax && rand() % 100 < lStorageMul) || bStorageSend)
	{
		bControlType = 2;
	}

	

	BYTE cbTempAllCard[52] = {
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,
		0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,
		0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,
		0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D
	};

	BYTE cbCardAllCount = 52;

	for (WORD i = 0; i<OXSIX_GAME_PLAYER; i++)
	{
		if (pOxSix_Parameter->cbPlayerStatus[i] == FALSE)
			continue;

		m_OxSixLogic.RemoveCard(pOxSix_Parameter->cbTableCardArray[i], OXSIX_MAX_CARDCOUNT-2, cbTempAllCard, cbCardAllCount);
		cbCardAllCount -= OXSIX_MAX_CARDCOUNT-2;
	}

	//计算分数
	BYTE cbDispatchCount = 0;
	bool bSuccess = false;
	do
	{
		//派发最后俩张扑克
		BYTE cbPosition = 0;
		for (WORD i = 0; i < OXSIX_GAME_PLAYER; i++)
		{
			if (pOxSix_Parameter->cbPlayerStatus[i] == FALSE)
				continue;

			for (int j = 0; j < 2; j++)
			{
				BYTE cbRandIndex = rand() % (cbCardAllCount - cbPosition);
				OxSix_Result->cbTableCardArray[i][OXSIX_MAX_CARDCOUNT - 1-j] = cbTempAllCard[cbRandIndex];
				cbPosition++;
				cbTempAllCard[cbRandIndex] = cbTempAllCard[cbCardAllCount - cbPosition];
			}
		}

		tag_OxSix_ScoreParameter pOxSix_ScoreParameter;
		ZeroMemory(&pOxSix_ScoreParameter, sizeof(pOxSix_ScoreParameter));

		pOxSix_ScoreParameter.wBankerUser = pOxSix_Parameter->wBankerUser;
		pOxSix_ScoreParameter.lExitScore = pOxSix_Parameter->lExitScore;
		CopyMemory(pOxSix_ScoreParameter.lTableScore, pOxSix_Parameter->lTableScore, sizeof(pOxSix_ScoreParameter.lTableScore));
		CopyMemory(pOxSix_ScoreParameter.cbCallBankerTimes, pOxSix_Parameter->cbCallBankerTimes, sizeof(pOxSix_ScoreParameter.cbCallBankerTimes));
		CopyMemory(pOxSix_ScoreParameter.cbPlayerStatus, pOxSix_Parameter->cbPlayerStatus, sizeof(pOxSix_ScoreParameter.cbPlayerStatus));
		CopyMemory(pOxSix_ScoreParameter.cbTableCard, pOxSix_Parameter->cbTableCardArray, sizeof(pOxSix_ScoreParameter.cbTableCard));

		SCORE lSystemScore = CalculateOxSixScore(pOxSix_ScoreParameter);
		cbDispatchCount++;

		bSuccess = true;

		if (bControlType == 1 && lSystemScore <= 0)
			bSuccess = false;
		if (bControlType == 2 && lSystemScore >= 0)
			bSuccess = false;

		if ((lSystemScore + lCurrentStorage) < 0 && lSystemScore < 0)
			bSuccess = false;

		if (cbDispatchCount > 100)
			break;
	} while (!bSuccess);
	

	return OxSix_Result;
}

//抢庄牛牛计算系统分
SCORE CServerControlItemSink::CalculateOxSixScore(tag_OxSix_ScoreParameter ScoreParameter)
{
	SCORE lSystemScore = 0;

	WORD wWinTimes[OXSIX_GAME_PLAYER];
	WORD wWinCount[OXSIX_GAME_PLAYER];
	ZeroMemory(wWinCount, sizeof(wWinCount));
	ZeroMemory(wWinTimes, sizeof(wWinTimes));
	//保存扑克
	BYTE cbUserCardData[OXSIX_GAME_PLAYER][MAX_CARDCOUNT];
	CopyMemory(cbUserCardData, ScoreParameter.cbTableCard, sizeof(cbUserCardData));

	
	//倍数抢庄 结算需要乘以cbMaxCallBankerTimes
	BYTE cbMaxCallBankerTimes = 1;

	for (WORD i = 0; i < OXSIX_GAME_PLAYER; i++)
	{
		if (ScoreParameter.cbPlayerStatus[i] == TRUE && ScoreParameter.cbCallBankerTimes[i] > cbMaxCallBankerTimes)
		{
			cbMaxCallBankerTimes = ScoreParameter.cbCallBankerTimes[i];
		}
	}

	//庄家倍数
	wWinTimes[ScoreParameter.wBankerUser] = m_OxSixLogic.GetTimes(cbUserCardData[ScoreParameter.wBankerUser], MAX_CARDCOUNT);

	IServerUserItem *pBankerUserItem = m_pITableFrame->GetTableUserItem(ScoreParameter.wBankerUser);

	//对比玩家
	for (WORD i = 0; i < OXSIX_GAME_PLAYER; i++)
	{
		if (i == ScoreParameter.wBankerUser || ScoreParameter.cbPlayerStatus[i] == FALSE)continue;

		//对比扑克
		if (m_OxSixLogic.CompareCard(cbUserCardData[i], cbUserCardData[ScoreParameter.wBankerUser], MAX_CARDCOUNT))
		{
			wWinCount[i]++;
			//获取倍数
			wWinTimes[i] = m_OxSixLogic.GetTimes(cbUserCardData[i], MAX_CARDCOUNT);
		}
		else
		{
			wWinCount[ScoreParameter.wBankerUser]++;
		}
	}

	//统计得分
	for (WORD i = 0; i < OXSIX_GAME_PLAYER; i++)
	{
		if (i == ScoreParameter.wBankerUser || ScoreParameter.cbPlayerStatus[i] == FALSE)continue;

		if (wWinCount[i] > 0)	//闲家胜利
		{
			SCORE lGameScore = ScoreParameter.lTableScore[i] * wWinTimes[i] * m_pITableFrame->GetCellScore() * cbMaxCallBankerTimes;

			IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);

			//庄家是机器人，系统得分为减去真实玩家的分数
			if (pBankerUserItem && pBankerUserItem->IsAndroidUser())
			{
				if (pServerUser && !pServerUser->IsAndroidUser())
				{
					lSystemScore -= lGameScore;
				}
			}
			//庄家是玩家，系统得分为加上机器人的分数
			else if (pBankerUserItem && !pBankerUserItem->IsAndroidUser())
			{
				if (pServerUser && pServerUser->IsAndroidUser())
				{
					lSystemScore += lGameScore;
				}
			}
		}
		else					//庄家胜利
		{
			SCORE lGameScore = (-1)*ScoreParameter.lTableScore[i] * wWinTimes[ScoreParameter.wBankerUser] * m_pITableFrame->GetCellScore() * cbMaxCallBankerTimes;

			IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);
			if (pServerUser && (pServerUser->GetUserScore() + lGameScore) < 0)
			{
				lGameScore = (-1) * pServerUser->GetUserScore();
			}

			//庄家是机器人，系统得分为减去真实玩家的分数
			if (pBankerUserItem && pBankerUserItem->IsAndroidUser())
			{
				if (pServerUser && !pServerUser->IsAndroidUser())
				{
					lSystemScore -= lGameScore;
				}
			}
			//庄家是玩家，系统得分为加上机器人的分数
			else if (pBankerUserItem && !pBankerUserItem->IsAndroidUser())
			{
				if (pServerUser && pServerUser->IsAndroidUser())
				{
					lSystemScore += lGameScore;
				}
			}
		}
	}

	//闲家强退分数	
	if (pBankerUserItem && pBankerUserItem->IsAndroidUser())
	{
		lSystemScore += ScoreParameter.lExitScore;
	}
	

	return lSystemScore;
}

//深海捕鱼控制结果
VOID * CServerControlItemSink::OnShbyControlResult(VOID * pData, WORD wDataSize, VOID * pResult)
{
	if (wDataSize != sizeof(tag_Shby_Parameter))
		return NULL;

	tag_Shby_Parameter *pShby_Parameter = (tag_Shby_Parameter *)pData;
	tag_Shby_Result * Shby_Result = (tag_Shby_Result *)pResult;

	Shby_Result->probability = pShby_Parameter->probability;

	IServerUserItem * server_user_item = m_pITableFrame->GetTableUserItem(pShby_Parameter->wshutUser);
	if (server_user_item && !server_user_item->IsAndroidUser())
	{
		BYTE bControlType = 0;	//0不控制，1控制系统赢:概率区间101~200，2控制系统输概率区间1~100
		SCORE lCurrentStorage = m_pGameServiceOption->pRoomStorageOption.lStorageCurrent;
		SCORE lStorageMax = m_pGameServiceOption->pRoomStorageOption.lStorageMax;
		int lStorageMul = m_pGameServiceOption->pRoomStorageOption.lStorageMul;
		SCORE lMinStorage[STORAGE_COUNT] = { 0 };
		SCORE lMaxStorage[STORAGE_COUNT] = { 0 };
		int nMulStorage[STORAGE_COUNT] = { 0 };

		TCHAR outBuff[64] = { 0 };
		for (int i = 0; i < STORAGE_COUNT; i++)
		{
			memset(outBuff, 0, sizeof(outBuff));
			_sntprintf_s(outBuff, sizeof(outBuff), _T("%s"), m_pGameServiceOption->pRoomStorageOption.szStorageControl[i]);

			SCORE llValue[3] = { 0 };
			this->GetValueFromCombStr(llValue, CountArray(llValue), outBuff, _tcslen(outBuff));
			lMinStorage[i] = llValue[0];
			lMaxStorage[i] = llValue[1];
			nMulStorage[i] = llValue[2];
		}

		int nControlIndex = 0;
		for (; nControlIndex < STORAGE_COUNT; nControlIndex++)
		{
			if (lCurrentStorage > lMinStorage[nControlIndex] && lCurrentStorage <= lMaxStorage[nControlIndex])
				break;
		}

		WORD wGoodCardUser = INVALID_CHAIR;

		//进入库存控制
		if ((nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] > 100) || lCurrentStorage < 0)
		{
			//系统赢判断
			if ((rand() % 100) < (nMulStorage[nControlIndex] - 100) || lCurrentStorage < 0)
			{
				Shby_Result->probability = 10;
			}
		}

		// 送分判断
		bool bStorageSend = nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] <= 100 && nMulStorage[nControlIndex] > 0 && (rand() % 100) < nMulStorage[nControlIndex];
		if ((lCurrentStorage > 0 && lCurrentStorage > lStorageMax && rand() % 100 < lStorageMul) || bStorageSend)
		{
			Shby_Result->probability = 0.00001;
		}
	}

	return Shby_Result;
}

//金蟾捕鱼控制结果
VOID * CServerControlItemSink::OnJcbyControlResult(VOID * pData, WORD wDataSize, VOID * pResult)
{
	if (wDataSize != sizeof(tag_Jcby_Parameter))
		return NULL;

	tag_Jcby_Parameter *pJcby_Parameter = (tag_Jcby_Parameter *)pData;
	tag_Jcby_Result * Jcby_Result = (tag_Jcby_Result *)pResult;

	Jcby_Result->probability = pJcby_Parameter->probability;

	IServerUserItem * server_user_item = m_pITableFrame->GetTableUserItem(pJcby_Parameter->wshutUser);
	if (server_user_item && !server_user_item->IsAndroidUser())
	{
		//玩家单控
		if (server_user_item->GetRoomGameRate() < 101)
		{
			int nRand = rand() % 100;
			//控制玩家赢
			if (nRand < server_user_item->GetRoomGameRate())
			{
				Jcby_Result->probability /= 10;
			}
			//控制玩家输
			else
			{
				Jcby_Result->probability *= 10;
			}
		}
		//库存控制
		else
		{
			BYTE bControlType = 0;	//0不控制，1控制系统赢:概率区间101~200，2控制系统输概率区间1~100
			SCORE lCurrentStorage = m_pGameServiceOption->pRoomStorageOption.lStorageCurrent;
			SCORE lStorageMax = m_pGameServiceOption->pRoomStorageOption.lStorageMax;
			int lStorageMul = m_pGameServiceOption->pRoomStorageOption.lStorageMul;
			SCORE lMinStorage[STORAGE_COUNT] = { 0 };
			SCORE lMaxStorage[STORAGE_COUNT] = { 0 };
			int nMulStorage[STORAGE_COUNT] = { 0 };

			TCHAR outBuff[64] = { 0 };
			for (int i = 0; i < STORAGE_COUNT; i++)
			{
				memset(outBuff, 0, sizeof(outBuff));
				_sntprintf_s(outBuff, sizeof(outBuff), _T("%s"), m_pGameServiceOption->pRoomStorageOption.szStorageControl[i]);

				SCORE llValue[3] = { 0 };
				this->GetValueFromCombStr(llValue, CountArray(llValue), outBuff, _tcslen(outBuff));
				lMinStorage[i] = llValue[0];
				lMaxStorage[i] = llValue[1];
				nMulStorage[i] = llValue[2];
			}

			int nControlIndex = 0;
			for (; nControlIndex < STORAGE_COUNT; nControlIndex++)
			{
				if (lCurrentStorage > lMinStorage[nControlIndex] && lCurrentStorage <= lMaxStorage[nControlIndex])
					break;
			}

			//进入库存控制
			if ((nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] > 100) || lCurrentStorage < 0)
			{
				//系统赢判断
				if ((rand() % 100) < (nMulStorage[nControlIndex] - 100) || lCurrentStorage < 0)
				{
					Jcby_Result->probability *= 10;
				}
			}
			else
			{
				// 送分判断
				bool bStorageSend = nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] <= 100 && nMulStorage[nControlIndex] > 0 && (rand() % 100) < nMulStorage[nControlIndex];
				if ((lCurrentStorage > 0 && lCurrentStorage > lStorageMax && rand() % 100 < lStorageMul) || bStorageSend)
				{
					Jcby_Result->probability /= 10;
				}
			}
		}
	}

	return Jcby_Result;
}

//大闹天宫捕鱼控制结果
VOID * CServerControlItemSink::OnDntgControlResult(VOID * pData, WORD wDataSize, VOID * pResult)
{
	if (wDataSize != sizeof(tag_Dntg_Parameter))
		return NULL;

	tag_Dntg_Parameter *pDntg_Parameter = (tag_Dntg_Parameter *)pData;
	tag_Dntg_Result * Dntg_Result = (tag_Dntg_Result *)pResult;

	Dntg_Result->probability = pDntg_Parameter->probability;

	IServerUserItem * server_user_item = m_pITableFrame->GetTableUserItem(pDntg_Parameter->wshutUser);
	if (server_user_item && !server_user_item->IsAndroidUser())
	{
		BYTE bControlType = 0;	//0不控制，1控制系统赢:概率区间101~200，2控制系统输概率区间1~100
		SCORE lCurrentStorage = m_pGameServiceOption->pRoomStorageOption.lStorageCurrent;
		SCORE lStorageMax = m_pGameServiceOption->pRoomStorageOption.lStorageMax;
		int lStorageMul = m_pGameServiceOption->pRoomStorageOption.lStorageMul;
		SCORE lMinStorage[STORAGE_COUNT] = { 0 };
		SCORE lMaxStorage[STORAGE_COUNT] = { 0 };
		int nMulStorage[STORAGE_COUNT] = { 0 };

		TCHAR outBuff[64] = { 0 };
		for (int i = 0; i < STORAGE_COUNT; i++)
		{
			memset(outBuff, 0, sizeof(outBuff));
			_sntprintf_s(outBuff, sizeof(outBuff), _T("%s"), m_pGameServiceOption->pRoomStorageOption.szStorageControl[i]);

			SCORE llValue[3] = { 0 };
			this->GetValueFromCombStr(llValue, CountArray(llValue), outBuff, _tcslen(outBuff));
			lMinStorage[i] = llValue[0];
			lMaxStorage[i] = llValue[1];
			nMulStorage[i] = llValue[2];
		}

		int nControlIndex = 0;
		for (; nControlIndex < STORAGE_COUNT; nControlIndex++)
		{
			if (lCurrentStorage > lMinStorage[nControlIndex] && lCurrentStorage <= lMaxStorage[nControlIndex])
				break;
		}

		//进入库存控制
		if ((nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] > 100) || lCurrentStorage < 0)
		{
			//系统赢判断
			if ((rand() % 100) < (nMulStorage[nControlIndex] - 100) || lCurrentStorage < 0)
			{
				Dntg_Result->probability = 10;
			}
		}
		else
		{
			// 送分判断
			bool bStorageSend = nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] <= 100 && nMulStorage[nControlIndex] > 0 && (rand() % 100) < nMulStorage[nControlIndex];
			if ((lCurrentStorage > 0 && lCurrentStorage > lStorageMax && rand() % 100 < lStorageMul) || bStorageSend)
			{
				Dntg_Result->probability = 0.00001;
			}
		}
	}

	return Dntg_Result;
}

//财神到控制结果
VOID * CServerControlItemSink::OnCaiShenControlResult(VOID * pData, WORD wDataSize, VOID * pResult)
{
	if (wDataSize != sizeof(tag_CaiShen_Parameter))
		return NULL;

	tag_CaiShen_Parameter *pCaiShen_Parameter = (tag_CaiShen_Parameter *)pData;
	tag_CaiShen_Result * CaiShen_Result = (tag_CaiShen_Result *)pResult;

	BYTE bControlType = 0;	//0不控制，1控制系统赢:概率区间101~200，2控制系统输概率区间1~100
	SCORE lCurrentStorage = m_pGameServiceOption->pRoomStorageOption.lStorageCurrent;
	SCORE lStorageMax = m_pGameServiceOption->pRoomStorageOption.lStorageMax;
	int lStorageMul = m_pGameServiceOption->pRoomStorageOption.lStorageMul;
	SCORE lMinStorage[STORAGE_COUNT] = { 0 };
	SCORE lMaxStorage[STORAGE_COUNT] = { 0 };
	int nMulStorage[STORAGE_COUNT] = { 0 };

	TCHAR outBuff[64] = { 0 };
	for (int i = 0; i < STORAGE_COUNT; i++)
	{
		memset(outBuff, 0, sizeof(outBuff));
		_sntprintf_s(outBuff, sizeof(outBuff), _T("%s"), m_pGameServiceOption->pRoomStorageOption.szStorageControl[i]);

		SCORE llValue[3] = { 0 };
		this->GetValueFromCombStr(llValue, CountArray(llValue), outBuff, _tcslen(outBuff));
		lMinStorage[i] = llValue[0];
		lMaxStorage[i] = llValue[1];
		nMulStorage[i] = llValue[2];
	}

	int nControlIndex = 0;
	for (; nControlIndex < STORAGE_COUNT; nControlIndex++)
	{
		if (lCurrentStorage > lMinStorage[nControlIndex] && lCurrentStorage <= lMaxStorage[nControlIndex])
			break;
	}

	//进入库存控制
	if ((nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] > 100) || lCurrentStorage < 0)
	{
		if ((rand() % 100) < (nMulStorage[nControlIndex] - 100) || lCurrentStorage < 0)
		{
			bControlType = 1;
		}
	}

	// 送分判断
	bool bStorageSend = nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] <= 100 && nMulStorage[nControlIndex] > 0 && (rand() % 100) < nMulStorage[nControlIndex];
	if ((lCurrentStorage > 0 && lCurrentStorage > lStorageMax && rand() % 100 < lStorageMul) || bStorageSend)
	{
		bControlType = 2;
	}

	SCORE lOneGameScore = 0;

	//计算分数
	BYTE cbDispatchCount = 0;
	bool bSuccess = false;
	do
	{
		//派发扑克
		m_CaiShenLogic.RandCardList(CaiShen_Result->cbItemInfo);

		
		lOneGameScore = m_CaiShenLogic.GetZhongJiangTime(CaiShen_Result->cbItemInfo) * pCaiShen_Parameter->lBetScore;

		cbDispatchCount++;

		bSuccess = true;

		if (bControlType == 1 && lOneGameScore >= 0)
			bSuccess = false;
		if (bControlType == 2 && lOneGameScore <= 0)
			bSuccess = false;

		if (cbDispatchCount > 100)
			break;
	} while (!bSuccess);
	

	return CaiShen_Result;
}

//绝地求生控制结果
VOID * CServerControlItemSink::OnJDQSControlResult(VOID * pData, WORD wDataSize, VOID * pResult)
{
	if (wDataSize != sizeof(tag_JDQS_Parameter))
		return NULL;

	tag_JDQS_Parameter *pJDQS_Parameter = (tag_JDQS_Parameter *)pData;
	tag_JDQS_Result * JDQS_Result = (tag_JDQS_Result *)pResult;

	BYTE bControlType = 0;	//0不控制，1控制系统赢:概率区间101~200，2控制系统输概率区间1~100
	SCORE lCurrentStorage = m_pGameServiceOption->pRoomStorageOption.lStorageCurrent;
	SCORE lStorageMax = m_pGameServiceOption->pRoomStorageOption.lStorageMax;
	int lStorageMul = m_pGameServiceOption->pRoomStorageOption.lStorageMul;
	SCORE lMinStorage[STORAGE_COUNT] = { 0 };
	SCORE lMaxStorage[STORAGE_COUNT] = { 0 };
	int nMulStorage[STORAGE_COUNT] = { 0 };

	TCHAR outBuff[64] = { 0 };
	for (int i = 0; i < STORAGE_COUNT; i++)
	{
		memset(outBuff, 0, sizeof(outBuff));
		_sntprintf_s(outBuff, sizeof(outBuff), _T("%s"), m_pGameServiceOption->pRoomStorageOption.szStorageControl[i]);

		SCORE llValue[3] = { 0 };
		this->GetValueFromCombStr(llValue, CountArray(llValue), outBuff, _tcslen(outBuff));
		lMinStorage[i] = llValue[0];
		lMaxStorage[i] = llValue[1];
		nMulStorage[i] = llValue[2];
	}

	int nControlIndex = 0;
	for (; nControlIndex < STORAGE_COUNT; nControlIndex++)
	{
		if (lCurrentStorage > lMinStorage[nControlIndex] && lCurrentStorage <= lMaxStorage[nControlIndex])
			break;
	}

	//进入库存控制
	if ((nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] > 100) || lCurrentStorage < 0)
	{
		if ((rand() % 100) < (nMulStorage[nControlIndex] - 100) || lCurrentStorage < 0)
		{
			bControlType = 1;
		}
	}

	// 送分判断
	bool bStorageSend = nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] <= 100 && nMulStorage[nControlIndex] > 0 && (rand() % 100) < nMulStorage[nControlIndex];
	if ((lCurrentStorage > 0 && lCurrentStorage > lStorageMax && rand() % 100 < lStorageMul) || bStorageSend)
	{
		bControlType = 2;
	}

	SCORE lOneGameScore = 0;

	//计算分数
	BYTE cbDispatchCount = 0;
	bool bSuccess = false;
	do
	{
		//派发扑克
		m_JDQSLogic.RandCardList(JDQS_Result->cbItemInfo);


		lOneGameScore = m_JDQSLogic.GetZhongJiangTime(JDQS_Result->cbItemInfo) * pJDQS_Parameter->lBetScore;

		cbDispatchCount++;

		bSuccess = true;

		if (bControlType == 1 && lOneGameScore >= 0)
			bSuccess = false;
		if (bControlType == 2 && lOneGameScore <= 0)
			bSuccess = false;

		if (cbDispatchCount > 100)
			break;
	} while (!bSuccess);


	return JDQS_Result;
}

//飞禽走兽控制结果
VOID * CServerControlItemSink::OnFQZSControlResult(VOID * pData, WORD wDataSize, VOID * pResult)
{
	if (wDataSize != sizeof(tag_FQZS_Parameter))
		return NULL;

	tag_FQZS_Parameter *pFQZS_Parameter = (tag_FQZS_Parameter *)pData;
	tag_FQZS_Result * FQZS_Result = (tag_FQZS_Result *)pResult;

	if (pFQZS_Parameter->cbJuControl == TRUE)
	{
		//清空扑克
		ZeroMemory(FQZS_Result->cbTableCardArray, sizeof(FQZS_Result->cbTableCardArray));

		//获取扑克
		FQZS_Result->cbTableCardArray[0] = rand() % FQZS_ANIMAL_COUNT;

		//鲨鱼 随机奖励
		if (FQZS_Result->cbTableCardArray[0] == 18)
		{
			FQZS_Result->cbTableCardArray[1] = rand() % FQZS_ANIMAL_COUNT;
			while (FQZS_Result->cbTableCardArray[1] == 18 || FQZS_Result->cbTableCardArray[1] == 11 || FQZS_Result->cbTableCardArray[1] == 25)
			{
				FQZS_Result->cbTableCardArray[1] = rand() % FQZS_ANIMAL_COUNT;
			}
		}


		if (pFQZS_Parameter->cbJuControlArea >= 0 && pFQZS_Parameter->cbJuControlArea < FQZS_AREA_COUNT)
		{
			//计算分数
			BYTE cbDispatchCount = 0;
			bool bSuccess;

			do
			{
				bSuccess = true;

				//清空扑克
				ZeroMemory(FQZS_Result->cbTableCardArray, sizeof(FQZS_Result->cbTableCardArray));

				//获取扑克
				FQZS_Result->cbTableCardArray[0] = rand() % FQZS_ANIMAL_COUNT;

				//鲨鱼 随机奖励
				if (FQZS_Result->cbTableCardArray[0] == 18)
				{
					FQZS_Result->cbTableCardArray[1] = rand() % FQZS_ANIMAL_COUNT;
					while (FQZS_Result->cbTableCardArray[1] == 18 || FQZS_Result->cbTableCardArray[1] == 11 || FQZS_Result->cbTableCardArray[1] == 25)
					{
						FQZS_Result->cbTableCardArray[1] = rand() % FQZS_ANIMAL_COUNT;
					}
				}

				if (cbFQZSGetAreaAnimal(FQZS_Result->cbTableCardArray[0]) != pFQZS_Parameter->cbJuControlArea)
				{
					bSuccess = false;
				}

				cbDispatchCount++;

				if (cbDispatchCount > 200)
					break;

			} while (!bSuccess);
		}
	}
	else
	{
		BYTE bControlType = 0;	//0不控制，1控制系统赢:概率区间101~200，2控制系统输概率区间1~100
		SCORE lCurrentStorage = m_pGameServiceOption->pRoomStorageOption.lStorageCurrent;
		SCORE lStorageMax = m_pGameServiceOption->pRoomStorageOption.lStorageMax;
		int lStorageMul = m_pGameServiceOption->pRoomStorageOption.lStorageMul;
		SCORE lMinStorage[STORAGE_COUNT] = { 0 };
		SCORE lMaxStorage[STORAGE_COUNT] = { 0 };
		int nMulStorage[STORAGE_COUNT] = { 0 };

		TCHAR outBuff[64] = { 0 };
		for (int i = 0; i < STORAGE_COUNT; i++)
		{
			memset(outBuff, 0, sizeof(outBuff));
			_sntprintf_s(outBuff, sizeof(outBuff), _T("%s"), m_pGameServiceOption->pRoomStorageOption.szStorageControl[i]);

			SCORE llValue[3] = { 0 };
			this->GetValueFromCombStr(llValue, CountArray(llValue), outBuff, _tcslen(outBuff));
			lMinStorage[i] = llValue[0];
			lMaxStorage[i] = llValue[1];
			nMulStorage[i] = llValue[2];
		}

		int nControlIndex = 0;
		for (; nControlIndex < STORAGE_COUNT; nControlIndex++)
		{
			if (lCurrentStorage > lMinStorage[nControlIndex] && lCurrentStorage <= lMaxStorage[nControlIndex])
				break;
		}

		//进入库存控制
		if ((nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] > 100) || lCurrentStorage < 0)
		{
			if ((rand() % 100) < (nMulStorage[nControlIndex] - 100) || lCurrentStorage < 0)
			{
				bControlType = 1;
			}
		}

		// 送分判断
		bool bStorageSend = nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] <= 100 && nMulStorage[nControlIndex] > 0 && (rand() % 100) < nMulStorage[nControlIndex];
		if ((lCurrentStorage > 0 && lCurrentStorage > lStorageMax && rand() % 100 < lStorageMul) || bStorageSend)
		{
			bControlType = 2;
		}

		BYTE cbRealCount = 0;
		BYTE cbAndroidCount = 0;
		for (WORD i = 0; i < FQZS_GAME_PLAYER; i++)
		{
			IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
			if (pServerUser)
			{
				if (!pServerUser->IsAndroidUser())
					cbRealCount++;
				else
					cbAndroidCount++;
			}
		}

		//如果没有真人就不控制
		if (cbRealCount == 0)
			bControlType = 0;

		//计算分数
		BYTE cbDispatchCount = 0;
		bool bSuccess = false;
		do
		{
			//派发扑克
			//清空扑克
			ZeroMemory(FQZS_Result->cbTableCardArray, sizeof(FQZS_Result->cbTableCardArray));

			//获取扑克
			FQZS_Result->cbTableCardArray[0] = rand() % FQZS_ANIMAL_COUNT;

			//鲨鱼 随机奖励
			if (FQZS_Result->cbTableCardArray[0] == 18)
			{
				FQZS_Result->cbTableCardArray[1] = rand() % FQZS_ANIMAL_COUNT;
				while (FQZS_Result->cbTableCardArray[1] == 18 || FQZS_Result->cbTableCardArray[1] == 11 || FQZS_Result->cbTableCardArray[1] == 25)
				{
					FQZS_Result->cbTableCardArray[1] = rand() % FQZS_ANIMAL_COUNT;
				}
			}

			cbDispatchCount++;

			bSuccess = true;

			tag_FQZS_ScoreParameter ScoreParameter;
			ZeroMemory(&ScoreParameter, sizeof(ScoreParameter));

			ScoreParameter.wBankerUser = pFQZS_Parameter->wCurrentBanker;
			CopyMemory(ScoreParameter.lUserJettonScore, pFQZS_Parameter->lUserJettonScore, sizeof(ScoreParameter.lUserJettonScore));
			CopyMemory(ScoreParameter.nMultiple, pFQZS_Parameter->nMultiple, sizeof(ScoreParameter.nMultiple));
			CopyMemory(ScoreParameter.cbTableCardArray, FQZS_Result->cbTableCardArray, sizeof(ScoreParameter.cbTableCardArray));

			SCORE lSystemScore = CalculateFQZSScore(ScoreParameter);

			if (bControlType == 1 && lSystemScore <= 0)
				bSuccess = false;
			if (bControlType == 2 && lSystemScore >= 0)
				bSuccess = false;

			if ((lSystemScore + lCurrentStorage) < 0 && lSystemScore < 0)
				bSuccess = false;

			if (cbDispatchCount > 100)
				break;
		} while (!bSuccess);
	}
	
	return FQZS_Result;
}

//飞禽走兽获取动物获胜
BYTE CServerControlItemSink::cbFQZSGetAreaAnimal(BYTE cbCardData)
{
	enum AnimalIndex
	{
		AREA_YAN_ZI,
		AREA_GE_ZI,
		AREA_KONG_QUE,
		AREA_LAO_YING,
		AREA_TU_ZI,
		AREA_XIONG_MAO,
		AREA_HOU_ZI,
		AREA_SHI_ZI,
		AREA_SHA_YU,
		AREA_FEI_QIN,
		AREA_ZOU_SHOU,
		AREA_JIN_SHA
	};

	if (cbCardData == 0 || cbCardData == 26 || cbCardData == 27)
		return AREA_GE_ZI;
	else if (cbCardData == 1 || cbCardData == 2 || cbCardData == 3)
		return AREA_YAN_ZI;
	else if (cbCardData == 4)
		return AREA_JIN_SHA;
	else if (cbCardData == 5 || cbCardData == 6 || cbCardData == 7)
		return AREA_TU_ZI;
	else if (cbCardData == 8 || cbCardData == 9 || cbCardData == 10)
		return AREA_XIONG_MAO;
	else if (cbCardData == 11)
		return AREA_FEI_QIN;
	else if (cbCardData == 12 || cbCardData == 13 || cbCardData == 14)
		return AREA_HOU_ZI;
	else if (cbCardData == 15 || cbCardData == 16 || cbCardData == 17)
		return AREA_SHI_ZI;
	else if (cbCardData == 18)
		return AREA_SHA_YU;
	else if (cbCardData == 19 || cbCardData == 20 || cbCardData == 21)
		return AREA_LAO_YING;
	else if (cbCardData == 22 || cbCardData == 23 || cbCardData == 24)
		return AREA_KONG_QUE;
	else if (cbCardData == 25)
		return AREA_ZOU_SHOU;
}

//飞禽走兽计算系统分
SCORE CServerControlItemSink::CalculateFQZSScore(tag_FQZS_ScoreParameter ScoreParameter)
{
	enum AnimalIndex
	{
		AREA_YAN_ZI,           
		AREA_GE_ZI,            
		AREA_KONG_QUE,         
		AREA_LAO_YING,
		AREA_TU_ZI,
		AREA_XIONG_MAO,
		AREA_HOU_ZI,
		AREA_SHI_ZI,
		AREA_SHA_YU,
		AREA_FEI_QIN,
		AREA_ZOU_SHOU,
		AREA_JIN_SHA
	};

	BYTE cbWinArea[FQZS_AREA_COUNT] = { FALSE };
	//银鲨，有俩个区域亮起
	if (ScoreParameter.cbTableCardArray[0] == 18)
	{
		cbWinArea[AREA_SHA_YU] = TRUE;
		if (ScoreParameter.cbTableCardArray[1] != 18 && ScoreParameter.cbTableCardArray[1] != 11 && ScoreParameter.cbTableCardArray[1] != 25)
		{
			cbWinArea[cbFQZSGetAreaAnimal(ScoreParameter.cbTableCardArray[1])] = TRUE;

			BYTE cbHotAnimal = cbFQZSGetAreaAnimal(ScoreParameter.cbTableCardArray[1]);
			if (cbHotAnimal >= AREA_YAN_ZI && cbHotAnimal <= AREA_LAO_YING)
				cbWinArea[AREA_FEI_QIN] = TRUE;
			else if (cbHotAnimal >= AREA_TU_ZI && cbHotAnimal <= AREA_SHI_ZI)
				cbWinArea[AREA_ZOU_SHOU] = TRUE;
		}
	}
	//通杀
	else if (ScoreParameter.cbTableCardArray[0] == 11)
	{
		ZeroMemory(cbWinArea, sizeof(cbWinArea));
	}
	//通赔
	else if (ScoreParameter.cbTableCardArray[0] == 25)
	{
		FillMemory(cbWinArea, sizeof(cbWinArea), TRUE);
	}
	else
	{
		cbWinArea[cbFQZSGetAreaAnimal(ScoreParameter.cbTableCardArray[0])] = TRUE;
		BYTE cbHotAnimal = cbFQZSGetAreaAnimal(ScoreParameter.cbTableCardArray[0]);
		if (cbHotAnimal >= AREA_YAN_ZI && cbHotAnimal <= AREA_LAO_YING)
			cbWinArea[AREA_FEI_QIN] = TRUE;
		else if (cbHotAnimal >= AREA_TU_ZI && cbHotAnimal <= AREA_SHI_ZI)
			cbWinArea[AREA_ZOU_SHOU] = TRUE;

		if (cbHotAnimal == AREA_JIN_SHA)
			cbWinArea[AREA_SHA_YU] = TRUE;
	}

	//庄家总量
	SCORE lSystemScore = 0;

	bool bIsBankerSystem = true;
	if (ScoreParameter.wBankerUser != INVALID_CHAIR)
	{
		IServerUserItem *pBankerUser = m_pITableFrame->GetTableUserItem(ScoreParameter.wBankerUser);
		if (!pBankerUser->IsAndroidUser())
			bIsBankerSystem = false;
	}

	//计算积分
	for (WORD wChairID = 0; wChairID<FQZS_GAME_PLAYER; wChairID++)
	{
		//庄家判断
		if (ScoreParameter.wBankerUser == wChairID)
		{
			continue;
		}

		//获取用户
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem == NULL)
		{
			continue;
		}

		for (WORD wAreaIndex = 0; wAreaIndex < FQZS_AREA_COUNT; ++wAreaIndex)
		{
			if (ScoreParameter.lUserJettonScore[wChairID][wAreaIndex] > 0)
			{
				if (cbWinArea[wAreaIndex] == TRUE)
				{
					SCORE Score = (ScoreParameter.nMultiple[wAreaIndex] - 1) * ScoreParameter.lUserJettonScore[wChairID][wAreaIndex];

					if (wAreaIndex == AREA_SHA_YU && cbWinArea[AREA_JIN_SHA] == TRUE)
						Score = (ScoreParameter.nMultiple[AREA_JIN_SHA] - 1) * ScoreParameter.lUserJettonScore[wChairID][wAreaIndex];

					if (bIsBankerSystem && !pIServerUserItem->IsAndroidUser())
						lSystemScore -= Score;
					else if (!bIsBankerSystem && pIServerUserItem->IsAndroidUser())
						lSystemScore += Score;
				}
				else
				{
					if (bIsBankerSystem && !pIServerUserItem->IsAndroidUser())
						lSystemScore += ScoreParameter.lUserJettonScore[wChairID][wAreaIndex];
					else if (!bIsBankerSystem && pIServerUserItem->IsAndroidUser())
						lSystemScore -= ScoreParameter.lUserJettonScore[wChairID][wAreaIndex];
				}
			}
		}

	}

	return lSystemScore;
}

//奔驰宝马控制结果
VOID * CServerControlItemSink::OnBCBMControlResult(VOID * pData, WORD wDataSize, VOID * pResult)
{
	if (wDataSize != sizeof(tag_BCBM_Parameter))
		return NULL;

	tag_BCBM_Parameter *pBCBM_Parameter = (tag_BCBM_Parameter *)pData;
	tag_BCBM_Result * BCBM_Result = (tag_BCBM_Result *)pResult;

	if (pBCBM_Parameter->cbJuControl == TRUE)
	{
		//获取扑克  先随机一次，怕下面的不会走
		BCBM_Result->cbTableCardArray = rand() % BCBM_ANIMAL_COUNT;

		
		if (pBCBM_Parameter->cbJuControlArea >= 0 && pBCBM_Parameter->cbJuControlArea < BCBM_AREA_COUNT)
		{
			//计算分数
			BYTE cbDispatchCount = 0;
			bool bSuccess;

			do
			{
				bSuccess = true;

				//获取扑克
				BCBM_Result->cbTableCardArray = rand() % BCBM_ANIMAL_COUNT;

				if (cbBCBMGetAreaAnimal(BCBM_Result->cbTableCardArray) != pBCBM_Parameter->cbJuControlArea)
				{
					bSuccess = false;
				}

				cbDispatchCount++;

				if (cbDispatchCount > 200)
					break;

			} while (!bSuccess);
		}
	}
	else
	{
		BYTE bControlType = 0;	//0不控制，1控制系统赢:概率区间101~200，2控制系统输概率区间1~100
		SCORE lCurrentStorage = m_pGameServiceOption->pRoomStorageOption.lStorageCurrent;
		SCORE lStorageMax = m_pGameServiceOption->pRoomStorageOption.lStorageMax;
		int lStorageMul = m_pGameServiceOption->pRoomStorageOption.lStorageMul;
		SCORE lMinStorage[STORAGE_COUNT] = { 0 };
		SCORE lMaxStorage[STORAGE_COUNT] = { 0 };
		int nMulStorage[STORAGE_COUNT] = { 0 };

		TCHAR outBuff[64] = { 0 };
		for (int i = 0; i < STORAGE_COUNT; i++)
		{
			memset(outBuff, 0, sizeof(outBuff));
			_sntprintf_s(outBuff, sizeof(outBuff), _T("%s"), m_pGameServiceOption->pRoomStorageOption.szStorageControl[i]);

			SCORE llValue[3] = { 0 };
			this->GetValueFromCombStr(llValue, CountArray(llValue), outBuff, _tcslen(outBuff));
			lMinStorage[i] = llValue[0];
			lMaxStorage[i] = llValue[1];
			nMulStorage[i] = llValue[2];
		}

		int nControlIndex = 0;
		for (; nControlIndex < STORAGE_COUNT; nControlIndex++)
		{
			if (lCurrentStorage > lMinStorage[nControlIndex] && lCurrentStorage <= lMaxStorage[nControlIndex])
				break;
		}

		//进入库存控制
		if ((nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] > 100) || lCurrentStorage < 0)
		{
			if ((rand() % 100) < (nMulStorage[nControlIndex] - 100) || lCurrentStorage < 0)
			{
				bControlType = 1;
			}
		}

		// 送分判断
		bool bStorageSend = nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] <= 100 && nMulStorage[nControlIndex] > 0 && (rand() % 100) < nMulStorage[nControlIndex];
		if ((lCurrentStorage > 0 && lCurrentStorage > lStorageMax && rand() % 100 < lStorageMul) || bStorageSend)
		{
			bControlType = 2;
		}

		BYTE cbRealCount = 0;
		BYTE cbAndroidCount = 0;
		for (WORD i = 0; i < BCBM_GAME_PLAYER; i++)
		{
			IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
			if (pServerUser)
			{
				if (!pServerUser->IsAndroidUser())
					cbRealCount++;
				else
					cbAndroidCount++;
			}
		}

		//如果没有真人就不控制
		if (cbRealCount == 0)
			bControlType = 0;

		//计算分数
		BYTE cbDispatchCount = 0;
		bool bSuccess = false;
		do
		{
			//派发扑克
			//清空扑克
			BCBM_Result->cbTableCardArray = 0;

			//获取扑克
			BCBM_Result->cbTableCardArray = rand() % BCBM_ANIMAL_COUNT;
			cbDispatchCount++;

			bSuccess = true;

			tag_BCBM_ScoreParameter ScoreParameter;
			ZeroMemory(&ScoreParameter, sizeof(ScoreParameter));

			ScoreParameter.wBankerUser = pBCBM_Parameter->wCurrentBanker;
			CopyMemory(ScoreParameter.lUserJettonScore, pBCBM_Parameter->lUserJettonScore, sizeof(ScoreParameter.lUserJettonScore));
			CopyMemory(ScoreParameter.nMultiple, pBCBM_Parameter->nMultiple, sizeof(ScoreParameter.nMultiple));
			ScoreParameter.cbTableCardArray = BCBM_Result->cbTableCardArray;

			SCORE lSystemScore = CalculateBCBMScore(ScoreParameter);

			if (bControlType == 1 && lSystemScore <= 0)
				bSuccess = false;
			if (bControlType == 2 && lSystemScore >= 0)
				bSuccess = false;

			if ((lSystemScore + lCurrentStorage) < 0 && lSystemScore < 0)
				bSuccess = false;

			if (cbDispatchCount > 100)
				break;
		} while (!bSuccess);
	}

	return BCBM_Result;
}

//奔驰宝马获取动物获胜
BYTE CServerControlItemSink::cbBCBMGetAreaAnimal(BYTE cbCardData)
{
	enum AnimalIndex
	{
		AREA_S_DZ,
		AREA_S_AD,
		AREA_S_BM,
		AREA_S_BC,
		AREA_B_DZ,
		AREA_B_AD,
		AREA_B_BM,
		AREA_B_BC
	};

	if (cbCardData == 0 || cbCardData == 8 || cbCardData == 16 || cbCardData == 24)
		return AREA_B_BM;
	else if (cbCardData == 1 || cbCardData == 9 || cbCardData == 17 || cbCardData == 25)
		return AREA_S_BM;
	else if (cbCardData == 2 || cbCardData == 10 || cbCardData == 18 || cbCardData == 26)
		return AREA_B_AD;
	else if (cbCardData == 3 || cbCardData == 11 || cbCardData == 19 || cbCardData == 27)
		return AREA_S_AD;
	else if (cbCardData == 4 || cbCardData == 12 || cbCardData == 20 || cbCardData == 28)
		return AREA_B_DZ;
	else if (cbCardData == 5 || cbCardData == 13 || cbCardData == 21 || cbCardData == 29)
		return AREA_S_DZ;
	else if (cbCardData == 6 || cbCardData == 14 || cbCardData == 22 || cbCardData == 30)
		return AREA_B_BC;
	else if (cbCardData == 7 || cbCardData == 15 || cbCardData == 23 || cbCardData == 31)
		return AREA_S_BC;
}

//奔驰宝马计算系统分
SCORE CServerControlItemSink::CalculateBCBMScore(tag_BCBM_ScoreParameter ScoreParameter)
{
	enum AnimalIndex
	{
		AREA_S_DZ,
		AREA_S_AD,
		AREA_S_BM,
		AREA_S_BC,
		AREA_B_DZ,
		AREA_B_AD,
		AREA_B_BM,
		AREA_B_BC
	};

	BYTE cbWinArea[BCBM_AREA_COUNT] = { FALSE };
	cbWinArea[cbBCBMGetAreaAnimal(ScoreParameter.cbTableCardArray)] = TRUE;

	//庄家总量
	SCORE lSystemScore = 0;

	bool bIsBankerSystem = true;
	if (ScoreParameter.wBankerUser != INVALID_CHAIR)
	{
		IServerUserItem *pBankerUser = m_pITableFrame->GetTableUserItem(ScoreParameter.wBankerUser);
		if (!pBankerUser->IsAndroidUser())
			bIsBankerSystem = false;
	}

	//计算积分
	for (WORD wChairID = 0; wChairID<BCBM_GAME_PLAYER; wChairID++)
	{
		//庄家判断
		if (ScoreParameter.wBankerUser == wChairID)
		{
			continue;
		}

		//获取用户
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem == NULL)
		{
			continue;
		}

		for (WORD wAreaIndex = 0; wAreaIndex < BCBM_AREA_COUNT; ++wAreaIndex)
		{
			if (ScoreParameter.lUserJettonScore[wChairID][wAreaIndex] > 0)
			{
				if (cbWinArea[wAreaIndex] == TRUE)
				{
					SCORE Score = ScoreParameter.nMultiple[wAreaIndex] * ScoreParameter.lUserJettonScore[wChairID][wAreaIndex];

					if (bIsBankerSystem && !pIServerUserItem->IsAndroidUser())
						lSystemScore -= Score;
					else if (!bIsBankerSystem && pIServerUserItem->IsAndroidUser())
						lSystemScore += Score;
				}
				else
				{
					if (bIsBankerSystem && !pIServerUserItem->IsAndroidUser())
						lSystemScore += ScoreParameter.lUserJettonScore[wChairID][wAreaIndex];
					else if (!bIsBankerSystem && pIServerUserItem->IsAndroidUser())
						lSystemScore -= ScoreParameter.lUserJettonScore[wChairID][wAreaIndex];
				}
			}
		}

	}

	return lSystemScore;
}

//水浒传控制结果
VOID * CServerControlItemSink::OnSHZControlResult(VOID * pData, WORD wDataSize, VOID * pResult)
{
	if (wDataSize != sizeof(tag_SHZ_Parameter))
		return NULL;

	tag_SHZ_Parameter *pSHZ_Parameter = (tag_SHZ_Parameter *)pData;
	tag_SHZ_Result * SHZ_Result = (tag_SHZ_Result *)pResult;

	BYTE bControlType = 0;	//0不控制，1控制系统赢:概率区间101~200，2控制系统输概率区间1~100
	SCORE lCurrentStorage = m_pGameServiceOption->pRoomStorageOption.lStorageCurrent;
	SCORE lStorageMax = m_pGameServiceOption->pRoomStorageOption.lStorageMax;
	int lStorageMul = m_pGameServiceOption->pRoomStorageOption.lStorageMul;
	SCORE lMinStorage[STORAGE_COUNT] = { 0 };
	SCORE lMaxStorage[STORAGE_COUNT] = { 0 };
	int nMulStorage[STORAGE_COUNT] = { 0 };

	TCHAR outBuff[64] = { 0 };
	for (int i = 0; i < STORAGE_COUNT; i++)
	{
		memset(outBuff, 0, sizeof(outBuff));
		_sntprintf_s(outBuff, sizeof(outBuff), _T("%s"), m_pGameServiceOption->pRoomStorageOption.szStorageControl[i]);

		SCORE llValue[3] = { 0 };
		this->GetValueFromCombStr(llValue, CountArray(llValue), outBuff, _tcslen(outBuff));
		lMinStorage[i] = llValue[0];
		lMaxStorage[i] = llValue[1];
		nMulStorage[i] = llValue[2];
	}

	int nControlIndex = 0;
	for (; nControlIndex < STORAGE_COUNT; nControlIndex++)
	{
		if (lCurrentStorage > lMinStorage[nControlIndex] && lCurrentStorage <= lMaxStorage[nControlIndex])
			break;
	}

	//进入库存控制
	if ((nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] > 100) || lCurrentStorage < 0)
	{
		if ((rand() % 100) < (nMulStorage[nControlIndex] - 100) || lCurrentStorage < 0)
		{
			bControlType = 1;
		}
	}

	// 送分判断
	bool bStorageSend = nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] <= 100 && nMulStorage[nControlIndex] > 0 && (rand() % 100) < nMulStorage[nControlIndex];
	if ((lCurrentStorage > 0 && lCurrentStorage > lStorageMax && rand() % 100 < lStorageMul) || bStorageSend)
	{
		bControlType = 2;
	}

	//正常开奖模式
	if (pSHZ_Parameter->cbOneMode == 1)
	{
		SCORE lOneGameScore = 0;
		//计算分数
		BYTE cbDispatchCount = 0;
		bool bSuccess = false;
		do
		{
			//派发扑克
			m_SHZLogic.RandCardList(SHZ_Result->cbItemInfo);

			lOneGameScore = m_SHZLogic.GetZhongJiangTime(SHZ_Result->cbItemInfo) * pSHZ_Parameter->lBetScore;
			cbDispatchCount++;

			bSuccess = true;

			if (bControlType == 1 && lOneGameScore >= 0)
				bSuccess = false;
			if (bControlType == 2 && lOneGameScore <= 0)
				bSuccess = false;

			if (cbDispatchCount > 100)
				break;
		} while (!bSuccess);
	}
	//比倍模式
	else if (pSHZ_Parameter->cbOneMode == 2)
	{

		SCORE lTwoScore = 0;
		BYTE cbDispatchCount = 0;

		do
		{
			SHZ_Result->cbOpenSize[0] = rand() % 6 + 1;
			SHZ_Result->cbOpenSize[1] = rand() % 6 + 1;

			lTwoScore = 0;

			if (pSHZ_Parameter->cbOpenArea == 0 && (SHZ_Result->cbOpenSize[0] + SHZ_Result->cbOpenSize[1]) <= 6)		//小
			{
				lTwoScore = pSHZ_Parameter->lBetScore * 2;
				if (SHZ_Result->cbOpenSize[0] == SHZ_Result->cbOpenSize[1])		//对子
				{
					lTwoScore = pSHZ_Parameter->lBetScore * 4;
				}
			}
			else if (pSHZ_Parameter->cbOpenArea == 2 && (SHZ_Result->cbOpenSize[0] + SHZ_Result->cbOpenSize[1]) > 7)	//大
			{
				lTwoScore = pSHZ_Parameter->lBetScore * 2;
				if (SHZ_Result->cbOpenSize[0] == SHZ_Result->cbOpenSize[1])		//对子
				{
					lTwoScore = pSHZ_Parameter->lBetScore * 4;
				}
			}
			else if (pSHZ_Parameter->cbOpenArea == 1 && (SHZ_Result->cbOpenSize[0] + SHZ_Result->cbOpenSize[1]) == 7)		//和
			{
				lTwoScore = pSHZ_Parameter->lBetScore * 6;
			}

			cbDispatchCount++;
			if (cbDispatchCount > 100)
				break;

			if (bControlType == 2)		//控制输
			{
				if (lTwoScore > 0)
				{
					break;
				}
			}
			else if (bControlType == 1) //控制赢
			{
				if (lTwoScore <= 0)
				{
					break;
				}
			}
			else
				break;

		} while (bControlType != 0);
	}

	return SHZ_Result;
}

//轮盘控制结果
VOID * CServerControlItemSink::OnLPControlResult(VOID * pData, WORD wDataSize, VOID * pResult)
{
	if (wDataSize != sizeof(tag_LP_Parameter))
		return NULL;

	tag_LP_Parameter *pLP_Parameter = (tag_LP_Parameter *)pData;
	tag_LP_Result * LP_Result = (tag_LP_Result *)pResult;

	BYTE bControlType = 0;	//0不控制，1控制系统赢:概率区间101~200，2控制系统输概率区间1~100
	SCORE lCurrentStorage = m_pGameServiceOption->pRoomStorageOption.lStorageCurrent;
	SCORE lStorageMax = m_pGameServiceOption->pRoomStorageOption.lStorageMax;
	int lStorageMul = m_pGameServiceOption->pRoomStorageOption.lStorageMul;
	SCORE lMinStorage[STORAGE_COUNT] = { 0 };
	SCORE lMaxStorage[STORAGE_COUNT] = { 0 };
	int nMulStorage[STORAGE_COUNT] = { 0 };

	TCHAR outBuff[64] = { 0 };
	for (int i = 0; i < STORAGE_COUNT; i++)
	{
		memset(outBuff, 0, sizeof(outBuff));
		_sntprintf_s(outBuff, sizeof(outBuff), _T("%s"), m_pGameServiceOption->pRoomStorageOption.szStorageControl[i]);

		SCORE llValue[3] = { 0 };
		this->GetValueFromCombStr(llValue, CountArray(llValue), outBuff, _tcslen(outBuff));
		lMinStorage[i] = llValue[0];
		lMaxStorage[i] = llValue[1];
		nMulStorage[i] = llValue[2];
	}

	int nControlIndex = 0;
	for (; nControlIndex < STORAGE_COUNT; nControlIndex++)
	{
		if (lCurrentStorage > lMinStorage[nControlIndex] && lCurrentStorage <= lMaxStorage[nControlIndex])
			break;
	}

	//进入库存控制
	if ((nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] > 100) || lCurrentStorage < 0)
	{
		if ((rand() % 100) < (nMulStorage[nControlIndex] - 100) || lCurrentStorage < 0)
		{
			bControlType = 1;
		}
	}

	// 送分判断
	bool bStorageSend = nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] <= 100 && nMulStorage[nControlIndex] > 0 && (rand() % 100) < nMulStorage[nControlIndex];
	if ((lCurrentStorage > 0 && lCurrentStorage > lStorageMax && rand() % 100 < lStorageMul) || bStorageSend)
	{
		bControlType = 2;
	}

	BYTE cbRealCount = 0;
	BYTE cbAndroidCount = 0;
	for (WORD i = 0; i < LP_GAME_PLAYER; i++)
	{
		IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
		if (pServerUser)
		{
			if (!pServerUser->IsAndroidUser())
				cbRealCount++;
			else
				cbAndroidCount++;
		}
	}

	//如果没有真人就不控制
	if (cbRealCount == 0)
		bControlType = 0;

	//计算分数
	BYTE cbDispatchCount = 0;
	bool bSuccess = false;
	do
	{
		//派发扑克
		LP_Result->cbTableCardArray = rand() % LP_RESULT_COUNT;
		cbDispatchCount++;

		bSuccess = true;

		tag_LP_ScoreParameter ScoreParameter;
		ZeroMemory(&ScoreParameter, sizeof(ScoreParameter));

		
		CopyMemory(ScoreParameter.lUserJettonScore, pLP_Parameter->lUserJettonScore, sizeof(ScoreParameter.lUserJettonScore));
		ScoreParameter.cbTableCardArray = LP_Result->cbTableCardArray;

		SCORE lSystemScore = CalculateLPScore(ScoreParameter);

		if (bControlType == 1 && lSystemScore <= 0)
			bSuccess = false;
		if (bControlType == 2 && lSystemScore >= 0)
			bSuccess = false;

		if ((lSystemScore + lCurrentStorage) < 0 && lSystemScore < 0)
			bSuccess = false;

		if (cbDispatchCount > 100)
			break;
	} while (!bSuccess);

	return LP_Result;
}

//轮盘获取胜利区域
BYTE CServerControlItemSink::cbLPGetWinArea(BYTE cbTableCard, BYTE cbWinArea[])
{
	for (int i = 0; i < LP_AREA_COUNT; i++)
	{
		for (int j = 0; j < 18; j++)
		{
			if (m_nLPAreaNums[i][j] == cbTableCard)
			{
				cbWinArea[i] = TRUE;
				break;
			}
			else if (m_nLPAreaNums[i][j] == 255)
				break;
		}
	}

	return 0;
}

//轮盘获取区域倍数
BYTE CServerControlItemSink::cbLPGetAreaTimes(BYTE cbArea)
{
	enum BetAreas
	{
		STRAIGHT_0,
		STRAIGHT_1,
		STRAIGHT_2,
		STRAIGHT_3,
		STRAIGHT_4,
		STRAIGHT_5,
		STRAIGHT_6,
		STRAIGHT_7,
		STRAIGHT_8,
		STRAIGHT_9,
		STRAIGHT_10,
		STRAIGHT_11,
		STRAIGHT_12,
		STRAIGHT_13,
		STRAIGHT_14,
		STRAIGHT_15,
		STRAIGHT_16,
		STRAIGHT_17,
		STRAIGHT_18,
		STRAIGHT_19,
		STRAIGHT_20,
		STRAIGHT_21,
		STRAIGHT_22,
		STRAIGHT_23,
		STRAIGHT_24,
		STRAIGHT_25,
		STRAIGHT_26,
		STRAIGHT_27,
		STRAIGHT_28,
		STRAIGHT_29,
		STRAIGHT_30,
		STRAIGHT_31,
		STRAIGHT_32,
		STRAIGHT_33,
		STRAIGHT_34,
		STRAIGHT_35,
		STRAIGHT_36,
		COLUMN_1,
		COLUMN_2,
		COLUMN_3,
		DOZEN_1,
		DOZEN_2,
		DOZEN_3,
		LOW_1,
		EVEN_1,
		RED_1,
		BLACK_1,
		ODD_1,
		HI_1,
		BET_AREA_COUNT
	};
	if (cbArea >= STRAIGHT_0 && cbArea <= STRAIGHT_36)
		return 35;
	else if (cbArea >= COLUMN_1 && cbArea <= DOZEN_3)
		return 2;
	else
		return 1;
}

//轮盘计算系统分
SCORE CServerControlItemSink::CalculateLPScore(tag_LP_ScoreParameter ScoreParameter)
{
	BYTE cbWinArea[LP_AREA_COUNT] = { FALSE };
	cbLPGetWinArea(ScoreParameter.cbTableCardArray,cbWinArea);

	//庄家总量
	SCORE lSystemScore = 0;

	//计算积分
	for (WORD wChairID = 0; wChairID<LP_GAME_PLAYER; wChairID++)
	{
		//获取用户
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem == NULL)
		{
			continue;
		}

		for (WORD wAreaIndex = 0; wAreaIndex < LP_AREA_COUNT; ++wAreaIndex)
		{
			if (ScoreParameter.lUserJettonScore[wChairID][wAreaIndex] > 0)
			{
				if (cbWinArea[wAreaIndex] == TRUE)
				{
					SCORE Score = cbLPGetAreaTimes(wAreaIndex) * ScoreParameter.lUserJettonScore[wChairID][wAreaIndex];

					if (!pIServerUserItem->IsAndroidUser())
						lSystemScore -= Score;
				}
				else
				{
					if (!pIServerUserItem->IsAndroidUser())
						lSystemScore += ScoreParameter.lUserJettonScore[wChairID][wAreaIndex];
				}
			}
		}

	}

	return lSystemScore;
}

//骰宝控制结果
VOID * CServerControlItemSink::OnSICBOControlResult(VOID * pData, WORD wDataSize, VOID * pResult)
{
	if (wDataSize != sizeof(tag_SICBO_Parameter))
		return NULL;

	tag_SICBO_Parameter *pSICBO_Parameter = (tag_SICBO_Parameter *)pData;
	tag_SICBO_Result * SICBO_Result = (tag_SICBO_Result *)pResult;

	BYTE bControlType = 0;	//0不控制，1控制系统赢:概率区间101~200，2控制系统输概率区间1~100
	SCORE lCurrentStorage = m_pGameServiceOption->pRoomStorageOption.lStorageCurrent;
	SCORE lStorageMax = m_pGameServiceOption->pRoomStorageOption.lStorageMax;
	int lStorageMul = m_pGameServiceOption->pRoomStorageOption.lStorageMul;
	SCORE lMinStorage[STORAGE_COUNT] = { 0 };
	SCORE lMaxStorage[STORAGE_COUNT] = { 0 };
	int nMulStorage[STORAGE_COUNT] = { 0 };

	TCHAR outBuff[64] = { 0 };
	for (int i = 0; i < STORAGE_COUNT; i++)
	{
		memset(outBuff, 0, sizeof(outBuff));
		_sntprintf_s(outBuff, sizeof(outBuff), _T("%s"), m_pGameServiceOption->pRoomStorageOption.szStorageControl[i]);

		SCORE llValue[3] = { 0 };
		this->GetValueFromCombStr(llValue, CountArray(llValue), outBuff, _tcslen(outBuff));
		lMinStorage[i] = llValue[0];
		lMaxStorage[i] = llValue[1];
		nMulStorage[i] = llValue[2];
	}

	int nControlIndex = 0;
	for (; nControlIndex < STORAGE_COUNT; nControlIndex++)
	{
		if (lCurrentStorage > lMinStorage[nControlIndex] && lCurrentStorage <= lMaxStorage[nControlIndex])
			break;
	}

	//进入库存控制
	if ((nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] > 100) || lCurrentStorage < 0)
	{
		if ((rand() % 100) < (nMulStorage[nControlIndex] - 100) || lCurrentStorage < 0)
		{
			bControlType = 1;
		}
	}

	// 送分判断
	bool bStorageSend = nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] <= 100 && nMulStorage[nControlIndex] > 0 && (rand() % 100) < nMulStorage[nControlIndex];
	if ((lCurrentStorage > 0 && lCurrentStorage > lStorageMax && rand() % 100 < lStorageMul) || bStorageSend)
	{
		bControlType = 2;
	}

	BYTE cbRealCount = 0;
	BYTE cbAndroidCount = 0;
	for (WORD i = 0; i < SICBO_GAME_PLAYER; i++)
	{
		IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
		if (pServerUser)
		{
			if (!pServerUser->IsAndroidUser())
				cbRealCount++;
			else
				cbAndroidCount++;
		}
	}

	//如果没有真人就不控制
	if (cbRealCount == 0)
		bControlType = 0;


	//计算分数
	BYTE cbDispatchCount = 0;
	bool bSuccess = false;
	do
	{
		//派发扑克
		SICBO_Result->cbTableCardArray[0] = rand() % SICBO_RESULT_COUNT + 1;
		SICBO_Result->cbTableCardArray[1] = rand() % SICBO_RESULT_COUNT + 1;
		SICBO_Result->cbTableCardArray[2] = rand() % SICBO_RESULT_COUNT + 1;
		cbDispatchCount++;

		bSuccess = true;

		tag_SICBO_ScoreParameter ScoreParameter;
		ZeroMemory(&ScoreParameter, sizeof(ScoreParameter));


		CopyMemory(ScoreParameter.lUserJettonScore, pSICBO_Parameter->lUserJettonScore, sizeof(ScoreParameter.lUserJettonScore));
		CopyMemory(ScoreParameter.cbTableCardArray, SICBO_Result->cbTableCardArray, sizeof(ScoreParameter.cbTableCardArray));

		SCORE lSystemScore = CalculateSICBOScore(ScoreParameter);

		if (bControlType == 1 && lSystemScore <= 0)
			bSuccess = false;
		if (bControlType == 2 && lSystemScore >= 0)
			bSuccess = false;

		if ((lSystemScore + lCurrentStorage) < 0 && lSystemScore < 0)
			bSuccess = false;

		if (cbDispatchCount > 100)
			break;
	} while (!bSuccess);

	return SICBO_Result;
}

//骰宝计算系统分
SCORE CServerControlItemSink::CalculateSICBOScore(tag_SICBO_ScoreParameter ScoreParameter)
{
	BYTE cbWinArea[SICBO_AREA_COUNT] = { FALSE };
	int cbAreaTimes[SICBO_AREA_COUNT] = {
		1,10,10,10,200,200,200,30,200,200,200,10,10,10,1,60,30,20,12,8,6,6,6,6,8,12,20,30,60,1,1,1,1,1,1
	};
	BYTE cbNumSum = ScoreParameter.cbTableCardArray[0] + ScoreParameter.cbTableCardArray[1] + ScoreParameter.cbTableCardArray[2];
	
	BYTE cbNumCount[6] = {0};
	cbNumCount[ScoreParameter.cbTableCardArray[0] - 1] ++;
	cbNumCount[ScoreParameter.cbTableCardArray[1] - 1] ++;
	cbNumCount[ScoreParameter.cbTableCardArray[2] - 1] ++;

	bool bDouble = false;
	BYTE cbDoubleNum = 0;
	if (ScoreParameter.cbTableCardArray[0] == ScoreParameter.cbTableCardArray[1] ||
		ScoreParameter.cbTableCardArray[0] == ScoreParameter.cbTableCardArray[2] ||
		ScoreParameter.cbTableCardArray[1] == ScoreParameter.cbTableCardArray[2])
	{
		bDouble = true;
		if (ScoreParameter.cbTableCardArray[0] == ScoreParameter.cbTableCardArray[1] ||
			ScoreParameter.cbTableCardArray[0] == ScoreParameter.cbTableCardArray[2])
			cbDoubleNum = ScoreParameter.cbTableCardArray[0];
		else if (ScoreParameter.cbTableCardArray[1] == ScoreParameter.cbTableCardArray[2])
			cbDoubleNum = ScoreParameter.cbTableCardArray[1];
	}

	bool bThree = false;
	BYTE cbThreeNum = 0;
	if (ScoreParameter.cbTableCardArray[0] == ScoreParameter.cbTableCardArray[1] &&
		ScoreParameter.cbTableCardArray[1] == ScoreParameter.cbTableCardArray[2])
	{
		bThree = true;
		cbThreeNum = ScoreParameter.cbTableCardArray[0];
	}

	if (cbNumSum >= 4 && cbNumSum <= 10 && !bThree)
		cbWinArea[0] = TRUE;
	if (bDouble && cbDoubleNum == 1)
		cbWinArea[1] = TRUE;
	if (bDouble && cbDoubleNum == 2)
		cbWinArea[2] = TRUE;
	if (bDouble && cbDoubleNum == 3)
		cbWinArea[3] = TRUE;
	if (bThree && cbThreeNum == 1)
		cbWinArea[4] = TRUE;
	if (bThree && cbThreeNum == 2)
		cbWinArea[5] = TRUE;
	if (bThree && cbThreeNum == 3)
		cbWinArea[6] = TRUE;
	if (bThree)
		cbWinArea[7] = TRUE;
	if (bThree && cbThreeNum == 4)
		cbWinArea[8] = TRUE;
	if (bThree && cbThreeNum == 5)
		cbWinArea[9] = TRUE;
	if (bThree && cbThreeNum == 6)
		cbWinArea[10] = TRUE;
	if (bDouble && cbDoubleNum == 4)
		cbWinArea[11] = TRUE;
	if (bDouble && cbDoubleNum == 5)
		cbWinArea[12] = TRUE;
	if (bDouble && cbDoubleNum == 6)
		cbWinArea[13] = TRUE;
	if (cbNumSum >= 11 && cbNumSum <= 17 && !bThree)
		cbWinArea[14] = TRUE;
	if (cbNumSum == 4)
		cbWinArea[15] = TRUE;
	if (cbNumSum == 5)
		cbWinArea[16] = TRUE;
	if (cbNumSum == 6)
		cbWinArea[17] = TRUE;
	if (cbNumSum == 7)
		cbWinArea[18] = TRUE;
	if (cbNumSum == 8)
		cbWinArea[19] = TRUE;
	if (cbNumSum == 9)
		cbWinArea[20] = TRUE;
	if (cbNumSum == 10)
		cbWinArea[21] = TRUE;
	if (cbNumSum == 11)
		cbWinArea[22] = TRUE;
	if (cbNumSum == 12)
		cbWinArea[23] = TRUE;
	if (cbNumSum == 13)
		cbWinArea[24] = TRUE;
	if (cbNumSum == 14)
		cbWinArea[25] = TRUE;
	if (cbNumSum == 15)
		cbWinArea[26] = TRUE;
	if (cbNumSum == 16)
		cbWinArea[27] = TRUE;
	if (cbNumSum == 17)
		cbWinArea[28] = TRUE;
	if (cbNumCount[0] > 0)
		cbWinArea[29] = TRUE;
	if (cbNumCount[1] > 0)
		cbWinArea[30] = TRUE;
	if (cbNumCount[2] > 0)
		cbWinArea[31] = TRUE;
	if (cbNumCount[3] > 0)
		cbWinArea[32] = TRUE;
	if (cbNumCount[4] > 0)
		cbWinArea[33] = TRUE;
	if (cbNumCount[5] > 0)
		cbWinArea[34] = TRUE;

	for (int i = 0; i < 6; i++)
	{
		if (cbNumCount[i] == 3)
			cbAreaTimes[29 + i] = 3;
		else if (cbNumCount[i] == 2)
			cbAreaTimes[29 + i] = 2;
	}

	//庄家总量
	SCORE lSystemScore = 0;

	//计算积分
	for (WORD wChairID = 0; wChairID<SICBO_GAME_PLAYER; wChairID++)
	{
		//获取用户
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem == NULL)
		{
			continue;
		}

		for (WORD wAreaIndex = 0; wAreaIndex < SICBO_AREA_COUNT; ++wAreaIndex)
		{
			if (ScoreParameter.lUserJettonScore[wChairID][wAreaIndex] > 0)
			{
				if (cbWinArea[wAreaIndex] == TRUE)
				{
					SCORE Score = cbAreaTimes[wAreaIndex] * ScoreParameter.lUserJettonScore[wChairID][wAreaIndex];

					if (!pIServerUserItem->IsAndroidUser())
						lSystemScore -= Score;
				}
				else
				{
					if (!pIServerUserItem->IsAndroidUser())
						lSystemScore += ScoreParameter.lUserJettonScore[wChairID][wAreaIndex];
				}
			}
		}

	}

	return lSystemScore;
}

//十三水控制结果
VOID * CServerControlItemSink::OnThirteenControlResult(VOID * pData, WORD wDataSize, VOID * pResult)
{
	tag_Thirteen_Result * Thirteen_Result = (tag_Thirteen_Result *)pResult;

	//洗牌发牌
	BYTE bCardData[52];
	m_ThLogic.RandCardList(bCardData, 52);
	CopyMemory(Thirteen_Result->cbTableCardArray, bCardData, sizeof(BYTE) * 52);

	//机器人数
	WORD wAiCount = 0;
	WORD wPlayerCount = 0;
	for (WORD i = 0; i<LAND_GAME_PLAYER; i++)
	{
		//获取用户
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem != NULL)
		{
			if (pIServerUserItem->IsAndroidUser())
			{
				wAiCount++;
			}
			wPlayerCount++;
		}
	}

	//全部机器
	if (wPlayerCount == wAiCount || wAiCount == 0) return NULL;

	BYTE bControlType = 0;	//0不控制，1控制系统赢:概率区间101~200，2控制系统输概率区间1~100
	SCORE lCurrentStorage = m_pGameServiceOption->pRoomStorageOption.lStorageCurrent;
	SCORE lStorageMax = m_pGameServiceOption->pRoomStorageOption.lStorageMax;
	int lStorageMul = m_pGameServiceOption->pRoomStorageOption.lStorageMul;
	SCORE lMinStorage[STORAGE_COUNT] = { 0 };
	SCORE lMaxStorage[STORAGE_COUNT] = { 0 };
	int nMulStorage[STORAGE_COUNT] = { 0 };

	TCHAR outBuff[64] = { 0 };
	for (int i = 0; i < STORAGE_COUNT; i++)
	{
		memset(outBuff, 0, sizeof(outBuff));
		_sntprintf_s(outBuff, sizeof(outBuff), _T("%s"), m_pGameServiceOption->pRoomStorageOption.szStorageControl[i]);

		SCORE llValue[3] = { 0 };
		this->GetValueFromCombStr(llValue, CountArray(llValue), outBuff, _tcslen(outBuff));
		lMinStorage[i] = llValue[0];
		lMaxStorage[i] = llValue[1];
		nMulStorage[i] = llValue[2];
	}

	int nControlIndex = 0;
	for (; nControlIndex < STORAGE_COUNT; nControlIndex++)
	{
		if (lCurrentStorage > lMinStorage[nControlIndex] && lCurrentStorage <= lMaxStorage[nControlIndex])
			break;
	}

	//进入库存控制
	if ((nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] > 100) || lCurrentStorage < 0)
	{
		//系统赢判断
		if ((rand() % 100) < (nMulStorage[nControlIndex] - 100) || lCurrentStorage < 0)
		{
			bControlType = 1;
		}
	}

	// 送分判断
	bool bStorageSend = nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] <= 100 && nMulStorage[nControlIndex] > 0 && (rand() % 100) < nMulStorage[nControlIndex];
	if ((lCurrentStorage > 0 && lCurrentStorage > lStorageMax && rand() % 100 < lStorageMul) || bStorageSend)
	{
		bControlType = 2;
	}

	//计算分数
	BYTE cbDispatchCount = 0;
	bool bSuccess = false;
	do
	{
		//派发扑克
		//洗牌发牌
		BYTE bCardData[52];
		m_ThLogic.RandCardList(bCardData, 52);
		CopyMemory(Thirteen_Result->cbTableCardArray, bCardData, sizeof(BYTE) * 52);
		cbDispatchCount++;

		bSuccess = true;

		tag_Thirteen_ScoreParameter ScoreParameter;
		ZeroMemory(&ScoreParameter, sizeof(ScoreParameter));

		CopyMemory(ScoreParameter.cbTableCardArray, Thirteen_Result->cbTableCardArray, sizeof(ScoreParameter.cbTableCardArray));

		SCORE lSystemScore = CalculateThirteenScore(ScoreParameter);

		if (bControlType == 1 && lSystemScore <= 0)
			bSuccess = false;
		if (bControlType == 2 && lSystemScore >= 0)
			bSuccess = false;

		if ((lSystemScore + lCurrentStorage) < 0 && lSystemScore < 0)
			bSuccess = false;

		if (cbDispatchCount > 100)
			break;
	} while (!bSuccess);



	return Thirteen_Result;
}

//十三水计算系统分
SCORE CServerControlItemSink::CalculateThirteenScore(tag_Thirteen_ScoreParameter ScoreParameter)
{
	BYTE cbTempHandCard[THIRTEEN_GAME_PLAYER][THIRTEEN_CARD_COUNT] = {0};
	BYTE cbSpecialType[THIRTEEN_GAME_PLAYER] = {0};
	BYTE bSegmentCard[THIRTEEN_GAME_PLAYER][3][5] = {0};
	BYTE lAddScoreTimes[THIRTEEN_GAME_PLAYER][3] = {0};
	SCORE lScoreTimes[THIRTEEN_GAME_PLAYER][3] = {0};
	SCORE lGunScoreTimes[THIRTEEN_GAME_PLAYER] = {0};
	SCORE lHomeRunsScoreTimes[THIRTEEN_GAME_PLAYER] = {0};
	SCORE lCellScore = m_pITableFrame->GetCellScore();

	CopyMemory(cbTempHandCard, ScoreParameter.cbTableCardArray, sizeof(cbTempHandCard));

	for (WORD i = 0; i < THIRTEEN_GAME_PLAYER; i++)
	{
		cbSpecialType[i] = m_ThLogic.GetSpecialType(cbTempHandCard[i], THIRTEEN_CARD_COUNT);
		BYTE cbPutCardData[THIRTEEN_CARD_COUNT] = {0};
		m_ThLogic.AutoPutCard(cbTempHandCard[i], cbPutCardData);

		CopyMemory(bSegmentCard[i][0], &cbPutCardData[0],sizeof(BYTE) * 3);
		CopyMemory(bSegmentCard[i][1], &cbPutCardData[3], sizeof(BYTE) * 5);
		CopyMemory(bSegmentCard[i][2], &cbPutCardData[8], sizeof(BYTE) * 5);
	}

	//计算输赢
	for (WORD i = 0; i < THIRTEEN_GAME_PLAYER; i++)
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem == NULL) continue;
		//打枪次数
		WORD wWinCount = 0;

		if (cbSpecialType[i] > 0)
			continue;

		for (int j = 0; j < THIRTEEN_GAME_PLAYER; j++)
		{

			if (j != i && cbSpecialType[j] == TH_INVALID)
			{
				IServerUserItem *pOtherIServerUserItem = m_pITableFrame->GetTableUserItem(j);
				if (pOtherIServerUserItem == NULL) continue;
				if (cbSpecialType[j] != 0 || cbSpecialType[i] != 0)
				{
					continue;
				}

				int bLoseWinCount = 0;
				//变量定义
				BYTE bWindInfo[3] = { 0 };

				//检查胜负
				bWindInfo[0] = m_ThLogic.CompareCard(bSegmentCard[i][0], bSegmentCard[j][0], 3, 3, true);
				bWindInfo[1] = m_ThLogic.CompareCard(bSegmentCard[i][1], bSegmentCard[j][1], 5, 5, true);
				bWindInfo[2] = m_ThLogic.CompareCard(bSegmentCard[i][2], bSegmentCard[j][2], 5, 5, true);

				//判断输赢
				for (BYTE bIdx = 0; bIdx < 3; bIdx++)
				{
					if (bWindInfo[bIdx] == enCREqual) continue;
					WORD wWinUser = (bWindInfo[bIdx] == enCRGreater) ? j : i;
					INT nMultiple = (bWindInfo[bIdx] == enCRGreater) ? 1 : -1;
					if (bWindInfo[bIdx] == enCRGreater) bLoseWinCount++;
					else bLoseWinCount--;

					//牌型翻倍
					BYTE bFrontCT = m_ThLogic.GetCardType(bSegmentCard[wWinUser][0], 3);
					BYTE bMidCT = m_ThLogic.GetCardType(bSegmentCard[wWinUser][1], 5);
					BYTE bBackCT = m_ThLogic.GetCardType(bSegmentCard[wWinUser][2], 5);

					//第一注
					if (bIdx == 0)
					{
						//第一注三条
						if (bFrontCT == CT_THREE)
						{
							lAddScoreTimes[i][0] -= nMultiple * 3;
						}
					}

					//第二注
					if (bIdx == 1)
					{
						//同花顺
						if (bMidCT >= TH_FIVE_TONGHUASHUN)
						{
							lAddScoreTimes[i][1] -= nMultiple * 10;
						}

						//铁支
						if (bMidCT == TH_FIVE_FOUR_ONE)
						{
							lAddScoreTimes[i][1] -= nMultiple * 8;
						}

						//葫芦
						if (bMidCT == TH_FIVE_THREE_DEOUBLE)
						{
							lAddScoreTimes[i][1] -= nMultiple * 2;
						}
					}

					//第三注
					if (bIdx == 2)
					{
						//同花顺
						if (bBackCT >= TH_FIVE_TONGHUASHUN)
						{
							lAddScoreTimes[i][2] -= nMultiple * 5;
						}

						//铁支
						if (bBackCT == TH_FIVE_FOUR_ONE)
						{
							lAddScoreTimes[i][2] -= nMultiple * 4;
						}
					}
					lScoreTimes[i][bIdx] -= nMultiple;
				}

				//判断是否打枪
				if (bLoseWinCount == 3)
				{
					lGunScoreTimes[i] -= 3;
				}
				else if (bLoseWinCount == -3)
				{
					wWinCount++;
					lGunScoreTimes[i] += 3;
				}
			}
		}

		//全垒打,分数翻倍。
		if (wWinCount == (THIRTEEN_GAME_PLAYER - 1))
		{
			//计算自家翻倍分
			lHomeRunsScoreTimes[i] = 18 * 3;

			//计算其他三家翻倍的分
			for (BYTE j = 0; j < THIRTEEN_GAME_PLAYER; j++)
			{
				if (j != i)
					lHomeRunsScoreTimes[j] = -18;
			}
		}
	}

	SCORE lSysTemScore = 0;

	for (WORD i = 0; i<THIRTEEN_GAME_PLAYER; ++i)
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);

		if (pIServerUserItem)
		{
			for (WORD j = 0; j < THIRTEEN_GAME_PLAYER; j++)
			{
				if (i != j)
				{
					IServerUserItem *pOtherIServerUserItem = m_pITableFrame->GetTableUserItem(j);

					//只计算机器人的得分当做系统得分
					if (pOtherIServerUserItem && pIServerUserItem->IsAndroidUser())
					{
						//特殊牌型
						if (cbSpecialType[j] != TH_INVALID || cbSpecialType[i] != TH_INVALID)
						{
							int nMulOnSD = m_ThLogic.nGetSpecialTimes(cbSpecialType[j], cbSpecialType[i]);
							lSysTemScore -= nMulOnSD * lCellScore;
						}
						//普通牌型
						else
						{
							//最终结算
							INT nMultiple = (lScoreTimes[j][0] + lScoreTimes[j][1] + lScoreTimes[j][2] + lGunScoreTimes[j] + lHomeRunsScoreTimes[j]);
							nMultiple += (lAddScoreTimes[j][0] + lAddScoreTimes[j][1] + lAddScoreTimes[j][2]);

							lSysTemScore -= nMultiple * lCellScore;
						}
					}
				}
			}
		}
	}

	return lSysTemScore;
}

//百人二八杠控制结果
VOID * CServerControlItemSink::OnTongziControlResult(VOID * pData, WORD wDataSize, VOID * pResult)
{
	if (wDataSize != sizeof(tag_TongZi_Parameter))
		return NULL;

	tag_TongZi_Parameter *pTongZi_Parameter = (tag_TongZi_Parameter *)pData;
	bool bSuccess = true;

	tag_TongZi_Result * TongZi_Result = (tag_TongZi_Result *)pResult;

	//游戏作弊
	if (pTongZi_Parameter->cbJuControl == TRUE)
	{
		DispatchTongziCard(TongZi_Result, pTongZi_Parameter->cbLeftCardCount, pTongZi_Parameter->cbTableCard);
		//循环计数
		LONGLONG lCirculateCount = 0L;
		while (true)
		{
			if (lCirculateCount > 100)
			{
				break;
			}

			bool bOK = true;

			bool bXianWin[3] = { false, false, false };							//比较输赢
			for (int i = 0; i < 3; i++)
			{
				bXianWin[i] = (m_TzLogic.CompareCard(TongZi_Result->cbTableCardArray[0], 2, TongZi_Result->cbTableCardArray[i + 1], 2) == 1);
			}

			BYTE cbAreaControl[3] = { pTongZi_Parameter->cbJuControlArea[1], pTongZi_Parameter->cbJuControlArea[2], pTongZi_Parameter->cbJuControlArea[3]};

			for (BYTE i = 0; i < 3; i++)
			{
				if (cbAreaControl[i] == 0)
					continue;

				if ((cbAreaControl[i] == 1 && bXianWin[i]) || (cbAreaControl[i] == 2 && !bXianWin[i]))
					continue;

				bOK = false;
				break;
			}

			//庄家赢
			if (pTongZi_Parameter->cbJuControlArea[0] == 1)
			{
				tag_TongZi_ScoreParameter ScoreParameter;
				ScoreParameter.wBankerUser = pTongZi_Parameter->wBankerUser;
				CopyMemory(ScoreParameter.lPlayBet, pTongZi_Parameter->lPlayBet, sizeof(ScoreParameter.lPlayBet));
				CopyMemory(ScoreParameter.cbTableCard, TongZi_Result->cbTableCardArray, sizeof(ScoreParameter.cbTableCard));

				DOUBLE lBankerScore = CalculateTongziBankerScore(ScoreParameter);

				if (lBankerScore < 0)
				{
					bOK = false;
				}

			}
			//庄家输
			else if (pTongZi_Parameter->cbJuControlArea[0] == 2)
			{
				tag_TongZi_ScoreParameter ScoreParameter;
				ScoreParameter.wBankerUser = pTongZi_Parameter->wBankerUser;
				CopyMemory(ScoreParameter.lPlayBet, pTongZi_Parameter->lPlayBet, sizeof(ScoreParameter.lPlayBet));
				CopyMemory(ScoreParameter.cbTableCard, TongZi_Result->cbTableCardArray, sizeof(ScoreParameter.cbTableCard));

				DOUBLE lBankerScore = CalculateTongziBankerScore(ScoreParameter);

				if (lBankerScore > 0)
				{
					bOK = false;
				}
			}

			if (!bOK)
			{
				DispatchTongziCard(TongZi_Result, pTongZi_Parameter->cbLeftCardCount, pTongZi_Parameter->cbTableCard);
				lCirculateCount++;
				continue;
			}
			else
				break;
		}

	}
	//库存控制判断
	else
	{
		BYTE bControlType = 0;	//0不控制，1控制系统赢:概率区间101~200，2控制系统输概率区间1~100
		SCORE lCurrentStorage = m_pGameServiceOption->pRoomStorageOption.lStorageCurrent;
		SCORE lStorageMax = m_pGameServiceOption->pRoomStorageOption.lStorageMax;
		int lStorageMul = m_pGameServiceOption->pRoomStorageOption.lStorageMul;
		SCORE lMinStorage[STORAGE_COUNT] = { 0 };
		SCORE lMaxStorage[STORAGE_COUNT] = { 0 };
		int nMulStorage[STORAGE_COUNT] = { 0 };

		TCHAR outBuff[64] = { 0 };
		for (int i = 0; i < STORAGE_COUNT; i++)
		{
			memset(outBuff, 0, sizeof(outBuff));
			_sntprintf_s(outBuff, sizeof(outBuff), _T("%s"), m_pGameServiceOption->pRoomStorageOption.szStorageControl[i]);

			SCORE llValue[3] = { 0 };
			this->GetValueFromCombStr(llValue, CountArray(llValue), outBuff, _tcslen(outBuff));
			lMinStorage[i] = llValue[0];
			lMaxStorage[i] = llValue[1];
			nMulStorage[i] = llValue[2];
		}

		int nControlIndex = 0;
		for (; nControlIndex < STORAGE_COUNT; nControlIndex++)
		{
			if (lCurrentStorage > lMinStorage[nControlIndex] && lCurrentStorage <= lMaxStorage[nControlIndex])
				break;
		}

		//进入库存控制
		if ((nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] > 100) || lCurrentStorage < 0)
		{
			if ((rand() % 100) < (nMulStorage[nControlIndex] - 100) || lCurrentStorage < 0)
			{
				bControlType = 1;
			}
		}

		// 送分判断
		bool bStorageSend = nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] <= 100 && nMulStorage[nControlIndex] > 0 && (rand() % 100) < nMulStorage[nControlIndex];
		if ((lCurrentStorage > 0 && lCurrentStorage > lStorageMax && rand() % 100 < lStorageMul) || bStorageSend)
		{
			bControlType = 2;
		}

		BYTE cbRealCount = 0;
		BYTE cbAndroidCount = 0;
		for (WORD i = 0; i < OXBATTLE_PLAYER; i++)
		{
			IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
			if (pServerUser)
			{
				if (!pServerUser->IsAndroidUser())
					cbRealCount++;
				else
					cbAndroidCount++;
			}
		}

		//如果没有真人就不控制
		if (cbRealCount == 0)
			bControlType = 0;

		//循环计数
		LONGLONG lCirculateCount = 0L;
		while (true)
		{
			DispatchTongziCard(TongZi_Result, pTongZi_Parameter->cbLeftCardCount, pTongZi_Parameter->cbTableCard);

			tag_TongZi_ScoreParameter ScoreParameter;
			ScoreParameter.wBankerUser = pTongZi_Parameter->wBankerUser;
			CopyMemory(ScoreParameter.lPlayBet, pTongZi_Parameter->lPlayBet, sizeof(ScoreParameter.lPlayBet));
			CopyMemory(ScoreParameter.cbTableCard, TongZi_Result->cbTableCardArray, sizeof(ScoreParameter.cbTableCard));

			//真人玩家得分
			SCORE lSystemScore = CalculateTongziScore(ScoreParameter);
			lCirculateCount++;

			bool bSuccess = true;

			if (bControlType == 1 && lSystemScore <= 0)
				bSuccess = false;
			if (bControlType == 2 && lSystemScore >= 0)
				bSuccess = false;

			if ((lSystemScore + lCurrentStorage) < 0 && lSystemScore < 0)
				bSuccess = false;

			if (lCirculateCount > 100 || bSuccess)
			{
				break;
			}
		}
	}

	return TongZi_Result;
}

//百人二八杠洗牌
void CServerControlItemSink::DispatchTongziCard(tag_TongZi_Result *OxBattle_Result, BYTE cbLeftCardCount, BYTE cbTableCard[])
{
	//混乱准备
	BYTE cbCardData[TZ_CARD_COUNT] = {0};
	CopyMemory(cbCardData, cbTableCard, sizeof(BYTE) * cbLeftCardCount);

	BYTE cbCardBuffer[8] = {0};
	BYTE cbBufferCount = 8;

	//混乱扑克
	BYTE cbRandCount = 0, cbPosition = 0;
	do
	{
		cbPosition = rand() % (cbLeftCardCount - cbRandCount);
		cbCardBuffer[cbRandCount++] = cbCardData[cbPosition];
		cbCardData[cbPosition] = cbCardData[cbLeftCardCount - cbRandCount];
	} while (cbRandCount<cbBufferCount);

	CopyMemory(OxBattle_Result->cbTableCardArray, cbCardBuffer,sizeof(BYTE) * 8);
}

//百人二八杠计算系统分
SCORE CServerControlItemSink::CalculateTongziScore(tag_TongZi_ScoreParameter ScoreParameter)
{
	SCORE lSystemScore = 0;

	//比较倍数
	bool bXianWin[3] = { false, false, false };							//比较输赢
	for (int i = 0; i < 3; i++)
	{
		bXianWin[i] = (m_TzLogic.CompareCard(ScoreParameter.cbTableCard[0], 2, ScoreParameter.cbTableCard[i + 1], 2) == 1);
	}

	//系统当庄或者机器人当庄
	if ((ScoreParameter.wBankerUser != INVALID_CHAIR && m_pITableFrame->GetTableUserItem(ScoreParameter.wBankerUser)->IsAndroidUser() == true) || (ScoreParameter.wBankerUser == INVALID_CHAIR))
	{
		// 计算系统输赢
		for (WORD wChairID = 0; wChairID < TONGZI_PLAYER; wChairID++)
		{
			// 获取玩家
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);

			// 过滤庄家
			if (wChairID == ScoreParameter.wBankerUser || pIServerUserItem == NULL || pIServerUserItem->IsAndroidUser() == true)
			{
				continue;
			}

			// 计算玩家下注
			for (int nAarea = 0; nAarea < 3; nAarea++)
			{
				if (ScoreParameter.lPlayBet[wChairID][nAarea + 1] != 0)
				{
					if (bXianWin[nAarea])
					{
						lSystemScore -= ScoreParameter.lPlayBet[wChairID][nAarea + 1];
					}
					else
					{
						lSystemScore += ScoreParameter.lPlayBet[wChairID][nAarea + 1];
					}
				}
			}
		}
	}
	//玩家当庄
	else
	{
		// 计算系统输赢
		for (WORD wChairID = 0; wChairID < TONGZI_PLAYER; wChairID++)
		{
			// 获取玩家
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);

			// 过滤庄家
			if (wChairID == ScoreParameter.wBankerUser || pIServerUserItem == NULL || pIServerUserItem->IsAndroidUser() == false)
			{
				continue;
			}

			// 计算玩家下注
			for (int nAarea = 0; nAarea < 3; nAarea++)
			{
				if (ScoreParameter.lPlayBet[wChairID][nAarea + 1] != 0)
				{
					if (bXianWin[nAarea])
					{
						lSystemScore += ScoreParameter.lPlayBet[wChairID][nAarea + 1];
					}
					else
					{
						lSystemScore -= ScoreParameter.lPlayBet[wChairID][nAarea + 1];
					}
				}
			}
		}
	}

	return lSystemScore;
}

//百人二八杠计算庄家输赢分
SCORE CServerControlItemSink::CalculateTongziBankerScore(tag_TongZi_ScoreParameter ScoreParameter)
{
	SCORE lBankerScore = 0;

	//比较倍数
	bool bXianWin[3] = { false, false, false };							//比较输赢

	for (int i = 0; i < 3; i++)
	{
		bXianWin[i] = (m_TzLogic.CompareCard(ScoreParameter.cbTableCard[0], 2, ScoreParameter.cbTableCard[i + 1], 2) == 1);
	}

	// 计算庄家输赢
	for (WORD wChairID = 0; wChairID < TONGZI_PLAYER; wChairID++)
	{
		// 获取玩家
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);

		// 过滤庄家
		if (wChairID == ScoreParameter.wBankerUser || pIServerUserItem == NULL)
		{
			continue;
		}

		// 计算玩家下注
		for (int nAarea = 0; nAarea < 3; nAarea++)
		{
			if (ScoreParameter.lPlayBet[wChairID][nAarea + 1] != 0)
			{
				if (bXianWin[nAarea])
				{
					lBankerScore -= ScoreParameter.lPlayBet[wChairID][nAarea + 1];
				}
				else
				{
					lBankerScore += ScoreParameter.lPlayBet[wChairID][nAarea + 1];
				}
			}
		}
	}

	return lBankerScore;
}

//三公控制结果
VOID * CServerControlItemSink::OnSanGongControlResult(VOID * pData, WORD wDataSize, VOID * pResult)
{
	if (wDataSize != sizeof(tag_SG_Parameter))
		return NULL;

	tag_SG_Parameter *pSG_Parameter = (tag_SG_Parameter *)pData;

	tag_SG_Result * SG_Result = (tag_SG_Result *)pResult;

	CopyMemory(SG_Result->cbTableCardArray, pSG_Parameter->cbTableCardArray, sizeof(SG_Result->cbTableCardArray));

	//机器人数
	bool bIsAiBanker = false;
	WORD wAiCount = 0;
	WORD wPlayerCount = 0;
	for (WORD i = 0; i<OXSIX_GAME_PLAYER; i++)
	{
		//获取用户
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem != NULL)
		{
			if (pSG_Parameter->cbPlayerStatus[i] == FALSE)continue;
			if (pIServerUserItem->IsAndroidUser())
			{
				wAiCount++;
				if (!bIsAiBanker && i == pSG_Parameter->wBankerUser)bIsAiBanker = true;
			}
			wPlayerCount++;
		}
	}

	//全部机器
	if (wPlayerCount == wAiCount || wAiCount == 0)
	{
		return NULL;
	}

	BYTE bControlType = 0;	//0不控制，1控制系统赢:概率区间101~200，2控制系统输概率区间1~100
	SCORE lCurrentStorage = m_pGameServiceOption->pRoomStorageOption.lStorageCurrent;
	SCORE lStorageMax = m_pGameServiceOption->pRoomStorageOption.lStorageMax;
	int lStorageMul = m_pGameServiceOption->pRoomStorageOption.lStorageMul;
	SCORE lMinStorage[STORAGE_COUNT] = { 0 };
	SCORE lMaxStorage[STORAGE_COUNT] = { 0 };
	int nMulStorage[STORAGE_COUNT] = { 0 };

	TCHAR outBuff[64] = { 0 };
	for (int i = 0; i < STORAGE_COUNT; i++)
	{
		memset(outBuff, 0, sizeof(outBuff));
		_sntprintf_s(outBuff, sizeof(outBuff), _T("%s"), m_pGameServiceOption->pRoomStorageOption.szStorageControl[i]);

		SCORE llValue[3] = { 0 };
		this->GetValueFromCombStr(llValue, CountArray(llValue), outBuff, _tcslen(outBuff));
		lMinStorage[i] = llValue[0];
		lMaxStorage[i] = llValue[1];
		nMulStorage[i] = llValue[2];
	}

	int nControlIndex = 0;
	for (; nControlIndex < STORAGE_COUNT; nControlIndex++)
	{
		if (lCurrentStorage > lMinStorage[nControlIndex] && lCurrentStorage <= lMaxStorage[nControlIndex])
			break;
	}

	//进入库存控制
	if ((nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] > 100) || lCurrentStorage < 0)
	{
		if ((rand() % 100) < (nMulStorage[nControlIndex] - 100) || lCurrentStorage < 0)
		{
			bControlType = 1;
		}
	}

	// 送分判断
	bool bStorageSend = nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] <= 100 && nMulStorage[nControlIndex] > 0 && (rand() % 100) < nMulStorage[nControlIndex];
	if ((lCurrentStorage > 0 && lCurrentStorage > lStorageMax && rand() % 100 < lStorageMul) || bStorageSend)
	{
		bControlType = 2;
	}



	BYTE cbTempAllCard[52] = {
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,
		0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,
		0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,
		0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D
	};

	BYTE cbCardAllCount = 52;

	for (WORD i = 0; i<OXSIX_GAME_PLAYER; i++)
	{
		if (pSG_Parameter->cbPlayerStatus[i] == FALSE)
			continue;

		m_SGLogic.RemoveCard(pSG_Parameter->cbTableCardArray[i], 2, cbTempAllCard, cbCardAllCount);
		cbCardAllCount -= 2;
	}

	//计算分数
	BYTE cbDispatchCount = 0;
	bool bSuccess = false;
	do
	{
		//派发最后一张扑克
		BYTE cbPosition = 0;
		for (WORD i = 0; i < SG_GAME_PLAYER; i++)
		{
			if (pSG_Parameter->cbPlayerStatus[i] == FALSE)
				continue;


			BYTE cbRandIndex = rand() % (cbCardAllCount - cbPosition);
			SG_Result->cbTableCardArray[i][SG_MAX_CARDCOUNT - 1] = cbTempAllCard[cbRandIndex];
			cbPosition++;
			cbTempAllCard[cbRandIndex] = cbTempAllCard[cbCardAllCount - cbPosition];
		}

		tag_SG_ScoreParameter pScoreParameter;
		ZeroMemory(&pScoreParameter, sizeof(pScoreParameter));

		pScoreParameter.wBankerUser = pSG_Parameter->wBankerUser;
		CopyMemory(pScoreParameter.lTableScore, pSG_Parameter->lTableScore, sizeof(pScoreParameter.lTableScore));
		CopyMemory(pScoreParameter.cbCallBankerTimes, pSG_Parameter->cbCallBankerTimes, sizeof(pScoreParameter.cbCallBankerTimes));
		CopyMemory(pScoreParameter.cbPlayerStatus, pSG_Parameter->cbPlayerStatus, sizeof(pScoreParameter.cbPlayerStatus));
		CopyMemory(pScoreParameter.cbTableCard, pSG_Parameter->cbTableCardArray, sizeof(pScoreParameter.cbTableCard));

		SCORE lSystemScore = CalculateSGScore(pScoreParameter);
		cbDispatchCount++;

		bSuccess = true;

		if (bControlType == 1 && lSystemScore <= 0)
			bSuccess = false;
		if (bControlType == 2 && lSystemScore >= 0)
			bSuccess = false;

		if ((lSystemScore + lCurrentStorage) < 0 && lSystemScore < 0)
			bSuccess = false;

		if (cbDispatchCount > 100)
			break;
	} while (!bSuccess);


	return SG_Result;
}

//三公计算系统分
SCORE CServerControlItemSink::CalculateSGScore(tag_SG_ScoreParameter ScoreParameter)
{
	SCORE lSystemScore = 0;

	WORD wWinTimes[SG_GAME_PLAYER];
	WORD wWinCount[SG_GAME_PLAYER];
	ZeroMemory(wWinCount, sizeof(wWinCount));
	ZeroMemory(wWinTimes, sizeof(wWinTimes));
	//保存扑克
	BYTE cbUserCardData[SG_GAME_PLAYER][SG_MAX_CARDCOUNT];
	CopyMemory(cbUserCardData, ScoreParameter.cbTableCard, sizeof(cbUserCardData));


	//倍数抢庄 结算需要乘以cbMaxCallBankerTimes
	BYTE cbMaxCallBankerTimes = 1;

	for (WORD i = 0; i < SG_GAME_PLAYER; i++)
	{
		if (ScoreParameter.cbPlayerStatus[i] == TRUE && ScoreParameter.cbCallBankerTimes[i] > cbMaxCallBankerTimes)
		{
			cbMaxCallBankerTimes = ScoreParameter.cbCallBankerTimes[i];
		}
	}

	//庄家倍数
	wWinTimes[ScoreParameter.wBankerUser] = m_SGLogic.GetTimes(cbUserCardData[ScoreParameter.wBankerUser], SG_MAX_CARDCOUNT);

	IServerUserItem *pBankerUserItem = m_pITableFrame->GetTableUserItem(ScoreParameter.wBankerUser);

	//对比玩家
	for (WORD i = 0; i < SG_GAME_PLAYER; i++)
	{
		if (i == ScoreParameter.wBankerUser || ScoreParameter.cbPlayerStatus[i] == FALSE)continue;

		//对比扑克
		if (m_SGLogic.CompareCard(cbUserCardData[i], cbUserCardData[ScoreParameter.wBankerUser], SG_MAX_CARDCOUNT))
		{
			wWinCount[i]++;
			//获取倍数
			wWinTimes[i] = m_SGLogic.GetTimes(cbUserCardData[i], SG_MAX_CARDCOUNT);
		}
		else
		{
			wWinCount[ScoreParameter.wBankerUser]++;
		}
	}

	//统计得分
	for (WORD i = 0; i < SG_GAME_PLAYER; i++)
	{
		if (i == ScoreParameter.wBankerUser || ScoreParameter.cbPlayerStatus[i] == FALSE)continue;

		if (wWinCount[i] > 0)	//闲家胜利
		{
			SCORE lGameScore = ScoreParameter.lTableScore[i] * wWinTimes[i] * m_pITableFrame->GetCellScore() * cbMaxCallBankerTimes;

			IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);

			//庄家是机器人，系统得分为减去真实玩家的分数
			if (pBankerUserItem && pBankerUserItem->IsAndroidUser())
			{
				if (pServerUser && !pServerUser->IsAndroidUser())
				{
					lSystemScore -= lGameScore;
				}
			}
			//庄家是玩家，系统得分为加上机器人的分数
			else if (pBankerUserItem && !pBankerUserItem->IsAndroidUser())
			{
				if (pServerUser && pServerUser->IsAndroidUser())
				{
					lSystemScore += lGameScore;
				}
			}
		}
		else					//庄家胜利
		{
			SCORE lGameScore = (-1)*ScoreParameter.lTableScore[i] * wWinTimes[ScoreParameter.wBankerUser] * m_pITableFrame->GetCellScore() * cbMaxCallBankerTimes;

			IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);
			if (pServerUser && (pServerUser->GetUserScore() + lGameScore) < 0)
			{
				lGameScore = (-1) * pServerUser->GetUserScore();
			}

			//庄家是机器人，系统得分为减去真实玩家的分数
			if (pBankerUserItem && pBankerUserItem->IsAndroidUser())
			{
				if (pServerUser && !pServerUser->IsAndroidUser())
				{
					lSystemScore -= lGameScore;
				}
			}
			//庄家是玩家，系统得分为加上机器人的分数
			else if (pBankerUserItem && !pBankerUserItem->IsAndroidUser())
			{
				if (pServerUser && pServerUser->IsAndroidUser())
				{
					lSystemScore += lGameScore;
				}
			}
		}
	}

	return lSystemScore;
}

//水果拉霸控制结果
VOID * CServerControlItemSink::OnSGLBControlResult(VOID * pData, WORD wDataSize, VOID * pResult)
{
	if (wDataSize != sizeof(tag_SGLB_Parameter))
		return NULL;

	tag_SGLB_Parameter *pSGLB_Parameter = (tag_SGLB_Parameter *)pData;
	tag_SGLB_Result * SGLB_Result = (tag_SGLB_Result *)pResult;

	BYTE bControlType = 0;	//0不控制，1控制系统赢:概率区间101~200，2控制系统输概率区间1~100
	LONGLONG lCurrentStorage = m_pGameServiceOption->pRoomStorageOption.lStorageCurrent;
	LONGLONG lStorageMax = m_pGameServiceOption->pRoomStorageOption.lStorageMax;
	int lStorageMul = m_pGameServiceOption->pRoomStorageOption.lStorageMul;
	LONGLONG lMinStorage[STORAGE_COUNT] = { 0 };
	LONGLONG lMaxStorage[STORAGE_COUNT] = { 0 };
	int nMulStorage[STORAGE_COUNT] = { 0 };

	TCHAR outBuff[64] = { 0 };
	for (int i = 0; i < STORAGE_COUNT; i++)
	{
		memset(outBuff, 0, sizeof(outBuff));
		_sntprintf_s(outBuff, sizeof(outBuff), _T("%s"), m_pGameServiceOption->pRoomStorageOption.szStorageControl[i]);

		DOUBLE llValue[3] = { 0 };
		this->GetValueFromCombStr(llValue, CountArray(llValue), outBuff, _tcslen(outBuff));
		lMinStorage[i] = llValue[0];
		lMaxStorage[i] = llValue[1];
		nMulStorage[i] = llValue[2];
	}

	int nControlIndex = 0;
	for (; nControlIndex < STORAGE_COUNT; nControlIndex++)
	{
		if (lCurrentStorage > lMinStorage[nControlIndex] && lCurrentStorage <= lMaxStorage[nControlIndex])
			break;
	}

	//进入库存控制
	if ((nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] > 100) || lCurrentStorage < 0)
	{
		if ((rand() % 100) < (nMulStorage[nControlIndex] - 100) || lCurrentStorage < 0)
		{
			bControlType = 1;
		}
	}

	// 送分判断
	bool bStorageSend = nControlIndex < STORAGE_COUNT && nMulStorage[nControlIndex] <= 100 && nMulStorage[nControlIndex] > 0 && (rand() % 100) < nMulStorage[nControlIndex];
	if ((lCurrentStorage > 0 && lCurrentStorage > lStorageMax && rand() % 100 < lStorageMul) || bStorageSend)
	{
		bControlType = 2;
	}

	//正常开奖模式
	if (pSGLB_Parameter->cbOneMode == 1)
	{
		int nControlTimes = 0;
		LONGLONG lOneGameScore = 0;
		//计算分数
		BYTE cbDispatchCount = 0;
		bool bSuccess = false;
		do
		{
			//派发扑克
			m_SGLBLogic.RandCardList(SGLB_Result->cbItemInfo);

			lOneGameScore = m_SGLBLogic.GetZhongJiangTime(SGLB_Result->cbItemInfo) * pSGLB_Parameter->lBetScore;
			cbDispatchCount++;

			bSuccess = true;

			if (bControlType == 1 && lOneGameScore >= 0)
				bSuccess = false;
			if (bControlType == 2 && lOneGameScore <= 0)
				bSuccess = false;

			//限制特殊图标概率
			if (bSuccess)
			{
				DOUBLE lJackPotScore = m_SGLBLogic.GetJackPotScore(SGLB_Result->cbItemInfo, 10000);
				BYTE cbFreeTimes = m_SGLBLogic.GetEnterFree(SGLB_Result->cbItemInfo);
				int nMaryTimes = m_SGLBLogic.GetEnterThree(SGLB_Result->cbItemInfo);

				int nRandIndex = rand() % 100000;
				if (nRandIndex >= 10 && lJackPotScore > 0)
				{
					bSuccess = false;
				}
				if (nRandIndex >= 5000 && nMaryTimes > 0)
				{
					bSuccess = false;
				}
				if (nRandIndex >= 10000 && cbFreeTimes > 0)
				{
					bSuccess = false;
				}
			}

			if (cbDispatchCount > 100)
				break;
		} while (!bSuccess);
	}
	//箱子开奖
	else if (pSGLB_Parameter->cbOneMode == 2)
	{
		LONGLONG lMinScore = 20 * pSGLB_Parameter->lBetScore;
		LONGLONG lMaxScore = 50 * pSGLB_Parameter->lBetScore;
		SGLB_Result->lBonusScore = (rand() % (lMaxScore - lMinScore)) + lMinScore;
	}

	return SGLB_Result;
}

//水浒传随机4个小玛丽区域
void CServerControlItemSink::GetSGLBMaryAreaItem(BYTE cbThreeItem[], BYTE cbControlType)
{
	BYTE cbDispatchCount = 0;

	do
	{
		for (int i = 0; i < 4; i++)
		{
			BYTE cbRand = rand() % 100;
			if (cbRand > 97)
			{
				cbThreeItem[i] = CT_ZHONGYITANG;
			}
			else if (cbRand > 93)
			{
				cbThreeItem[i] = CT_TITIANXINGDAO;
			}
			else if (cbRand > 83)
			{
				cbThreeItem[i] = CT_SONG;
			}
			else if (cbRand > 72)
			{
				cbThreeItem[i] = CT_LIN;
			}
			else if (cbRand > 60)
			{
				cbThreeItem[i] = CT_LU;
			}
			else if (cbRand > 45)
			{
				cbThreeItem[i] = CT_DADAO;
			}
			else if (cbRand > 25)
			{
				cbThreeItem[i] = CT_YINGQIANG;
			}
			else
			{
				cbThreeItem[i] = CT_FUTOU;
			}
		}

		BYTE cbLeftLinkCount = 1;
		for (int i = 1; i < 4; i++)
		{
			if (cbThreeItem[i] == cbThreeItem[0])
				cbLeftLinkCount++;
			else
				break;
		}

		BYTE cbRightLinkCount = 1;
		for (int i = 2; i >= 0; i--)
		{
			if (cbThreeItem[i] == cbThreeItem[3])
				cbRightLinkCount++;
			else
				break;
		}

		BYTE cbLinkCount = cbLeftLinkCount;
		if (cbRightLinkCount > cbLinkCount)
			cbLinkCount = cbRightLinkCount;

		cbDispatchCount++;
		if (cbDispatchCount > 100)
			break;

		if (cbControlType == 2)		//控制系统输
		{
			if (cbLinkCount >= 3)
			{
				break;
			}
		}
		else if (cbControlType == 1) //控制系统赢
		{
			if (cbLinkCount < 3)
			{
				break;
			}
		}
		else
			break;

	} while (cbControlType != 0);
}

//获取小玛丽开奖倍数
int CServerControlItemSink::GetSGLBMaryPrizeTimes(BYTE cbThreeIndex, BYTE cbThreeItem[])
{
	BYTE cbAllArea[24] = {
		CT_LU, CT_YINGQIANG, CT_ZHONGYITANG, CT_FUTOU, CT_DADAO, 9,
		CT_SONG, CT_LU, CT_YINGQIANG, CT_FUTOU, CT_TITIANXINGDAO, 9,
		CT_LIN, CT_DADAO, CT_FUTOU, CT_LU, CT_SONG, 9,
		CT_LIN, CT_YINGQIANG, CT_FUTOU, CT_DADAO, CT_TITIANXINGDAO, 9 };

	int nTimes = 0;

	for (int i = 0; i < 4; i++)
	{
		if (cbThreeItem[i] == cbAllArea[cbThreeIndex])
			nTimes++;
	}

	if (nTimes > 0)
	{
		switch (cbAllArea[cbThreeIndex])
		{
		case CT_FUTOU:
		{
			return 2;
		}
		case CT_YINGQIANG:
		{
			return 5;
		}
		case CT_DADAO:
		{
			return 10;
		}
		case CT_LU:
		{
			return 20;
		}
		case CT_LIN:
		{
			return 50;
		}
		case CT_SONG:
		{
			return 70;
		}
		case CT_TITIANXINGDAO:
		{
			return 100;
		}
		case CT_ZHONGYITANG:
		{
			return 200;
		}
		}
		return 0;
	}
	else
		return 0;
}

void CServerControlItemSink::GetValueFromCombStr(SCORE llData[], int nDataLen, LPCTSTR sTr, int nStrLen)
{
	int nIndex = 0;							// 索引
	TCHAR szStr[500] = TEXT("");

	int nCount = 0;							// 获取到的数据
	for (int i = 0; i<nStrLen; i++)
	{
		if (sTr[i] != '|')
		{
			szStr[nIndex] = sTr[i];
			nIndex++;
			if (i == (int)(_tcslen(sTr) - 1))
			{
				llData[nCount] = _ttof(szStr);
				nCount++;
				nIndex = 0;
				memset(szStr, NULL, sizeof(szStr));
			}
			continue;
		}

		if (nCount >= nDataLen)
			return;

		llData[nCount] = _ttof(szStr);
		nCount++;
		nIndex = 0;
		memset(szStr, NULL, sizeof(szStr));
	}
}