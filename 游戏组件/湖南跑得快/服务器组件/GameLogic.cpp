#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//静态变量
const BYTE cbIndexCount = 5;

//扑克数据
const BYTE	CGameLogic::m_bCardListData[48]=
{
	0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
		0x11,     0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
		0x21,     0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
		0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K
};

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameLogic::CGameLogic()
{
}

//析构函数
CGameLogic::~CGameLogic()
{
}

//获取类型
BYTE CGameLogic::GetCardType(const BYTE bCardData[], BYTE bCardCount)
{
	//简单牌型
	switch (bCardCount)
	{
	case 0:	//空牌
	{
		return CT_ERROR;
	}
	case 1: //单牌
	{
		return CT_SINGLE;
	}
	case 2:	//对牌火箭
	{
		//牌型判断
		if (GetCardLogicValue(bCardData[0]) == GetCardLogicValue(bCardData[1])) return CT_DOUBLE_LINE;

		return CT_ERROR;
	}
	}

	BYTE cbTempCard[MAX_COUNT] = {0};
	CopyMemory(cbTempCard, bCardData, sizeof(BYTE) * bCardCount);

	SortCardList(cbTempCard, bCardCount);

	//分析扑克
	tagAnalyseResult AnalyseResult;
	AnalysebCardData(cbTempCard, bCardCount, AnalyseResult);

	//四牌判断
	if (AnalyseResult.cbBlockCount[3]>0)
	{
		//牌型判断
		if ((AnalyseResult.cbBlockCount[3] == 1) && (bCardCount == 4)) return CT_BOMB;

		return CT_ERROR;
	}

	//三牌判断
	if (AnalyseResult.cbBlockCount[2]>0)
	{
		//连牌判断
		if (AnalyseResult.cbBlockCount[2]>1)
		{
			//变量定义
			BYTE cbCardData = AnalyseResult.cbCardData[2][0];
			BYTE cbFirstLogicValue = GetCardLogicValue(cbCardData);

			//错误过虑
			if (cbFirstLogicValue >= 15) return CT_ERROR;

			//连牌判断
			for (BYTE i = 1; i<AnalyseResult.cbBlockCount[2]; i++)
			{
				BYTE cbCardData = AnalyseResult.cbCardData[2][i * 3];
				if (cbFirstLogicValue != (GetCardLogicValue(cbCardData) + i)) return CT_ERROR;
			}
		}
		else if (bCardCount == 3) return CT_THREE_LINE;

		//牌形判断
		if (AnalyseResult.cbBlockCount[2] * 3 == bCardCount) return CT_THREE_LINE;
		if (AnalyseResult.cbBlockCount[2] * 4 == bCardCount) return CT_THREE_LINE_TAKE_SINGLE;
		if (AnalyseResult.cbBlockCount[2] * 5 == bCardCount) return CT_THREE_LINE_TAKE_DOUBLE;

		return CT_ERROR;
	}

	//两张类型
	if (AnalyseResult.cbBlockCount[1] >= 1)
	{
		//变量定义
		BYTE cbCardData = AnalyseResult.cbCardData[1][0];
		BYTE cbFirstLogicValue = GetCardLogicValue(cbCardData);

		//错误过虑
		if (cbFirstLogicValue >= 15) return CT_ERROR;

		//连牌判断
		for (BYTE i = 1; i<AnalyseResult.cbBlockCount[1]; i++)
		{
			BYTE cbCardData = AnalyseResult.cbCardData[1][i * 2];
			if (cbFirstLogicValue != (GetCardLogicValue(cbCardData) + i)) return CT_ERROR;
		}

		//二连判断
		if ((AnalyseResult.cbBlockCount[1] * 2) == bCardCount) return CT_DOUBLE_LINE;

		return CT_ERROR;
	}

	//单张判断
	if ((AnalyseResult.cbBlockCount[0] >= 5) && (AnalyseResult.cbBlockCount[0] == bCardCount))
	{
		//变量定义
		BYTE cbCardData = AnalyseResult.cbCardData[0][0];
		BYTE cbFirstLogicValue = GetCardLogicValue(cbCardData);

		//错误过虑
		if (cbFirstLogicValue >= 15) return CT_ERROR;

		//连牌判断
		for (BYTE i = 1; i<AnalyseResult.cbBlockCount[0]; i++)
		{
			BYTE cbCardData = AnalyseResult.cbCardData[0][i];
			if (cbFirstLogicValue != (GetCardLogicValue(cbCardData) + i)) return CT_ERROR;
		}

		return CT_SINGLE_LINE;
	}

	return CT_ERROR;
}

//排列扑克
void CGameLogic::SortCardList(BYTE bCardData[], BYTE bCardCount)
{
	//转换数值
	BYTE bLogicVolue[MAX_COUNT];
	for (BYTE i=0;i<bCardCount;i++)	bLogicVolue[i]=GetCardLogicValue(bCardData[i]);	

	//排序操作
	bool bSorted=true;
	BYTE bTempData,bLast=bCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<bLast;i++)
		{
			if ((bLogicVolue[i]<bLogicVolue[i+1])||
				((bLogicVolue[i]==bLogicVolue[i+1])&&(bCardData[i]<bCardData[i+1])))
			{
				//交换位置
				bTempData=bCardData[i];
				bCardData[i]=bCardData[i+1];
				bCardData[i+1]=bTempData;
				bTempData=bLogicVolue[i];
				bLogicVolue[i]=bLogicVolue[i+1];
				bLogicVolue[i+1]=bTempData;
				bSorted=false;
			}	
		}
		bLast--;
	} while(bSorted==false);

	return;
}

//排列扑克
VOID CGameLogic::SortOutCardList(BYTE cbCardData[], BYTE cbCardCount)
{
	//获取牌型
	BYTE cbCardType = GetCardType(cbCardData, cbCardCount);

	if (cbCardType == CT_THREE_LINE_TAKE_SINGLE || cbCardType == CT_THREE_LINE_TAKE_DOUBLE)
	{
		//分析牌
		tagAnalyseResult AnalyseResult = {};
		AnalysebCardData(cbCardData, cbCardCount, AnalyseResult);

		cbCardCount = AnalyseResult.cbBlockCount[2] * 3;
		CopyMemory(cbCardData, AnalyseResult.cbCardData[2], sizeof(BYTE)*cbCardCount);
		for (int i = CountArray(AnalyseResult.cbBlockCount) - 1; i >= 0; i--)
		{
			if (i == 2) continue;

			if (AnalyseResult.cbBlockCount[i] > 0)
			{
				CopyMemory(&cbCardData[cbCardCount], AnalyseResult.cbCardData[i],
					sizeof(BYTE)*(i + 1)*AnalyseResult.cbBlockCount[i]);
				cbCardCount += (i + 1)*AnalyseResult.cbBlockCount[i];
			}
		}
	}

	return;
}

//混乱扑克
void CGameLogic::RandCardList(BYTE bCardBuffer[], BYTE bBufferCount)
{
	//混乱准备
	BYTE bCardData[sizeof(m_bCardListData)];
	CopyMemory(bCardData,m_bCardListData,sizeof(m_bCardListData));

	//混乱扑克
	BYTE bRandCount=0,bPosition=0;
	do
	{
		bPosition=rand()%(bBufferCount-bRandCount);
		bCardBuffer[bRandCount++]=bCardData[bPosition];
		bCardData[bPosition]=bCardData[bBufferCount-bRandCount];
	} while (bRandCount<bBufferCount);

	return;
}

//删除扑克
bool CGameLogic::RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount)
{
	//检验数据
	ASSERT(bRemoveCount<=bCardCount);

	//定义变量
	BYTE bDeleteCount=0,bTempCardData[20];
	if (bCardCount>CountArray(bTempCardData)) return false;
	CopyMemory(bTempCardData,bCardData,bCardCount*sizeof(bCardData[0]));

	//置零扑克
	for (BYTE i=0;i<bRemoveCount;i++)
	{
		for (BYTE j=0;j<bCardCount;j++)
		{
			if (bRemoveCard[i]==bTempCardData[j])
			{
				bDeleteCount++;
				bTempCardData[j]=0;
				break;
			}
		}
	}
	if (bDeleteCount!=bRemoveCount) return false;

	//清理扑克
	BYTE bCardPos=0;
	for (BYTE i=0;i<bCardCount;i++)
	{
		if (bTempCardData[i]!=0) bCardData[bCardPos++]=bTempCardData[i];
	}

	return true;
}

//有效判断
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	//变量定义
	BYTE cbColor=GetCardColor(cbCardData);
	BYTE cbValue=GetCardValue(cbCardData);

	//有效判断
	switch (cbColor)
	{
	case 0x00:
		{
			return ((cbValue>=0x03)&&(cbValue<=0x0D));
		}
	case 0x10:
		{
			return ((cbValue==0x01)||((cbValue>=0x03)&&(cbValue<=0x0D)));
		}
	case 0x20:
		{
			return ((cbValue==0x01)||((cbValue>=0x03)&&(cbValue<=0x0D)));
		}
	case 0x30:
		{
			return ((cbValue>=0x01)&&(cbValue<=0x0D));
		}
	}

	return false;
}

//逻辑数值
BYTE CGameLogic::GetCardLogicValue(BYTE bCardData)
{
	//扑克属性
	BYTE bCardColor=GetCardColor(bCardData);
	BYTE bCardValue=GetCardValue(bCardData);

	//转换数值
	return (bCardValue<=2)?(bCardValue+13):bCardValue;
}

//对比扑克
bool CGameLogic::CompareCard(BYTE bFirstList[], BYTE bNextList[], BYTE bFirstCount, BYTE bNextCount)
{
	//获取类型
	BYTE cbNextType = GetCardType(bNextList, bNextCount);
	BYTE cbFirstType = GetCardType(bFirstList, bFirstCount);

	//类型判断
	if (cbNextType == CT_ERROR) return false;

	//炸弹判断
	if ((cbFirstType != CT_BOMB) && (cbNextType == CT_BOMB)) return true;
	if ((cbFirstType == CT_BOMB) && (cbNextType != CT_BOMB)) return false;

	//规则判断
	if ((cbFirstType != cbNextType) || (bFirstCount != bNextCount)) return false;

	//开始对比
	switch (cbNextType)
	{
	case CT_SINGLE:
	case CT_SINGLE_LINE:
	case CT_DOUBLE_LINE:
	case CT_THREE_LINE:
	case CT_BOMB:
	{
		//获取数值
		BYTE cbNextLogicValue = GetCardLogicValue(bNextList[0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(bFirstList[0]);

		//对比扑克
		return cbNextLogicValue>cbFirstLogicValue;
	}
	case CT_THREE_LINE_TAKE_SINGLE:
	case CT_THREE_LINE_TAKE_DOUBLE:
	{
		//分析扑克
		tagAnalyseResult NextResult;
		tagAnalyseResult FirstResult;
		AnalysebCardData(bNextList, bNextCount, NextResult);
		AnalysebCardData(bFirstList, bFirstCount, FirstResult);

		//获取数值
		BYTE cbNextLogicValue = GetCardLogicValue(NextResult.cbCardData[2][0]);
		BYTE cbFirstLogicValue = GetCardLogicValue(FirstResult.cbCardData[2][0]);

		//对比扑克
		return cbNextLogicValue>cbFirstLogicValue;
	}
	}

	return false;
}

//分析扑克
void CGameLogic::AnalysebCardData(const BYTE bCardData[], BYTE bCardCount, tagAnalyseResult & AnalyseResult)
{
	//设置结果
	ZeroMemory(&AnalyseResult, sizeof(AnalyseResult));

	//扑克分析
	for (BYTE i = 0; i<bCardCount; i++)
	{
		//变量定义
		BYTE cbSameCount = 1, cbCardValueTemp = 0;
		BYTE cbLogicValue = GetCardLogicValue(bCardData[i]);

		//搜索同牌
		for (BYTE j = i + 1; j<bCardCount; j++)
		{
			//获取扑克
			if (GetCardLogicValue(bCardData[j]) != cbLogicValue) break;

			//设置变量
			cbSameCount++;
		}

		if (cbSameCount > 4)
		{
			ASSERT(FALSE);
			//设置结果
			ZeroMemory(&AnalyseResult, sizeof(AnalyseResult));
			return;
		}

		//设置结果
		BYTE cbIndex = AnalyseResult.cbBlockCount[cbSameCount - 1]++;
		for (BYTE j = 0; j<cbSameCount; j++) AnalyseResult.cbCardData[cbSameCount - 1][cbIndex*cbSameCount + j] = bCardData[i + j];

		//设置索引
		i += cbSameCount - 1;
	}

	return;
}

//分析分布
VOID CGameLogic::AnalysebDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing)
{
	//设置变量
	ZeroMemory(&Distributing, sizeof(Distributing));

	//设置变量
	for (BYTE i = 0; i<cbCardCount; i++)
	{
		if (cbCardData[i] == 0) continue;

		//获取属性
		BYTE cbCardColor = GetCardColor(cbCardData[i]);
		BYTE cbCardValue = GetCardValue(cbCardData[i]);

		//分布信息
		Distributing.cbCardCount++;
		Distributing.cbDistributing[cbCardValue - 1][cbIndexCount]++;
		Distributing.cbDistributing[cbCardValue - 1][cbCardColor >> 4]++;
	}

	return;
}


//出牌搜索
BYTE CGameLogic::SearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount,
	tagSearchCardResult *pSearchCardResult)
{
	//设置结果
	ASSERT(pSearchCardResult != NULL);
	if (pSearchCardResult == NULL) return 0;

	ZeroMemory(pSearchCardResult, sizeof(tagSearchCardResult));

	//变量定义
	BYTE cbResultCount = 0;
	tagSearchCardResult tmpSearchCardResult = {};

	//构造扑克
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount = cbHandCardCount;
	CopyMemory(cbCardData, cbHandCardData, sizeof(BYTE)*cbHandCardCount);

	BYTE cbTempTurnCardData[MAX_COUNT];
	CopyMemory(cbTempTurnCardData, cbTurnCardData, sizeof(BYTE)*cbTurnCardCount);

	//排列扑克
	SortCardList(cbCardData, cbCardCount);
	SortCardList(cbTempTurnCardData, cbTurnCardCount);

	//获取类型
	BYTE cbTurnOutType = GetCardType(cbTempTurnCardData, cbTurnCardCount);

	//出牌分析
	switch (cbTurnOutType)
	{
	case CT_ERROR:					//错误类型
	{
		pSearchCardResult->cbCardCount[cbResultCount] = 1;
		pSearchCardResult->cbResultCard[cbResultCount][0] = cbCardData[0];
		cbResultCount++;

		return cbResultCount;
	}
	case CT_SINGLE:					//单牌类型
	{
		//变量定义
		BYTE cbReferCard = cbTempTurnCardData[0];
		BYTE cbSameCount = 1;

		//搜索相同牌
		cbResultCount = SearchSameCard(cbCardData, cbCardCount, cbReferCard, cbSameCount, pSearchCardResult);

		break;
	}
	case CT_SINGLE_LINE:		//单连类型
	case CT_DOUBLE_LINE:		//对连类型
	case CT_THREE_LINE:				//三连类型
	{
		//变量定义
		BYTE cbBlockCount = 1;
		if (cbTurnOutType == CT_DOUBLE_LINE) cbBlockCount = 2;
		else if (cbTurnOutType == CT_THREE_LINE) cbBlockCount = 3;

		BYTE cbLineCount = cbTurnCardCount / cbBlockCount;

		//搜索边牌
		cbResultCount = SearchLineCardType(cbCardData, cbCardCount, cbTempTurnCardData[0], cbBlockCount, cbLineCount, pSearchCardResult);

		break;
	}
	case CT_THREE_LINE_TAKE_SINGLE:	//三带一单
	case CT_THREE_LINE_TAKE_DOUBLE:	//三带一对
	{
		//效验牌数
		if (cbCardCount < cbTurnCardCount) break;

		//如果是三带一或三带二
		if (cbTurnCardCount == 4 || cbTurnCardCount == 5)
		{
			BYTE cbTakeCardCount = cbTurnOutType == CT_THREE_LINE_TAKE_SINGLE ? 1 : 2;

			//搜索三带牌型
			cbResultCount = SearchTakeCardType(cbCardData, cbCardCount, cbTempTurnCardData[2], 3, cbTakeCardCount, pSearchCardResult);
		}
		else
		{
			//变量定义
			BYTE cbBlockCount = 3;
			BYTE cbLineCount = cbTurnCardCount / (cbTurnOutType == CT_THREE_LINE_TAKE_SINGLE ? 4 : 5);
			BYTE cbTakeCardCount = cbTurnOutType == CT_THREE_LINE_TAKE_SINGLE ? 1 : 2;

			//搜索连牌
			BYTE cbTmpTurnCard[MAX_COUNT] = {};
			CopyMemory(cbTmpTurnCard, cbTempTurnCardData, sizeof(BYTE)*cbTurnCardCount);
			SortOutCardList(cbTmpTurnCard, cbTurnCardCount);
			cbResultCount = SearchLineCardType(cbCardData, cbCardCount, cbTmpTurnCard[0], cbBlockCount, cbLineCount, pSearchCardResult);

			//提取带牌
			bool bAllDistill = true;
			for (BYTE i = 0; i < cbResultCount; i++)
			{
				BYTE cbResultIndex = cbResultCount - i - 1;

				//变量定义
				BYTE cbTmpCardData[MAX_COUNT];
				BYTE cbTmpCardCount = cbCardCount;

				//删除连牌
				CopyMemory(cbTmpCardData, cbCardData, sizeof(BYTE)*cbCardCount);
				VERIFY(RemoveCard(pSearchCardResult->cbResultCard[cbResultIndex], pSearchCardResult->cbCardCount[cbResultIndex],
					cbTmpCardData, cbTmpCardCount));
				cbTmpCardCount -= pSearchCardResult->cbCardCount[cbResultIndex];

				//分析牌
				tagAnalyseResult  TmpResult = {};
				AnalysebCardData(cbTmpCardData, cbTmpCardCount, TmpResult);

				//提取牌
				BYTE cbDistillCard[MAX_COUNT] = {};
				BYTE cbDistillCount = 0;
				for (BYTE j = 0; j < CountArray(TmpResult.cbBlockCount); j++)
				{
					if (TmpResult.cbBlockCount[j] > 0)
					{
						if (j + 1 == cbTakeCardCount && TmpResult.cbBlockCount[j] >= cbLineCount)
						{
							BYTE cbTmpBlockCount = TmpResult.cbBlockCount[j];
							CopyMemory(cbDistillCard, &TmpResult.cbCardData[j][(cbTmpBlockCount - cbLineCount)*(j + 1)],
								sizeof(BYTE)*(j + 1)*cbLineCount);
							cbDistillCount = (j + 1)*cbLineCount;
							break;
						}
						else
						{
							for (BYTE k = 0; k < TmpResult.cbBlockCount[j]; k++)
							{
								BYTE cbSubCount = cbTakeCardCount - cbDistillCount;

								if (cbSubCount > j + 1)
									cbSubCount = j + 1;

								BYTE cbTmpBlockCount = TmpResult.cbBlockCount[j];
								CopyMemory(&cbDistillCard[cbDistillCount], &TmpResult.cbCardData[j][(cbTmpBlockCount - k - 1)*(j + 1)],
									sizeof(BYTE)*cbSubCount);
								cbDistillCount += cbSubCount;

								//提取完成
								if (cbDistillCount == cbTakeCardCount*cbLineCount) break;
							}
						}
					}

					//提取完成
					if (cbDistillCount == cbTakeCardCount*cbLineCount) break;
				}

				//提取完成
				if (cbDistillCount == cbTakeCardCount*cbLineCount)
				{
					//复制带牌
					BYTE cbCount = pSearchCardResult->cbCardCount[cbResultIndex];
					CopyMemory(&pSearchCardResult->cbResultCard[cbResultIndex][cbCount], cbDistillCard,
						sizeof(BYTE)*cbDistillCount);
					pSearchCardResult->cbCardCount[cbResultIndex] += cbDistillCount;
				}
				//否则删除连牌
				else
				{
					bAllDistill = false;
					pSearchCardResult->cbCardCount[cbResultIndex] = 0;
				}
			}

			//整理组合
			if (!bAllDistill)
			{
				pSearchCardResult->cbSearchCount = cbResultCount;
				cbResultCount = 0;
				for (BYTE i = 0; i < pSearchCardResult->cbSearchCount; i++)
				{
					if (pSearchCardResult->cbCardCount[i] != 0)
					{
						tmpSearchCardResult.cbCardCount[cbResultCount] = pSearchCardResult->cbCardCount[i];
						CopyMemory(tmpSearchCardResult.cbResultCard[cbResultCount], pSearchCardResult->cbResultCard[i],
							sizeof(BYTE)*pSearchCardResult->cbCardCount[i]);
						cbResultCount++;
					}
				}
				tmpSearchCardResult.cbSearchCount = cbResultCount;
				CopyMemory(pSearchCardResult, &tmpSearchCardResult, sizeof(tagSearchCardResult));
			}
		}

		break;
	}
	}

	//搜索炸弹
	if (cbCardCount >= 4)
	{
		//变量定义
		BYTE cbReferCard = 0;
		if (cbTurnOutType == CT_BOMB) cbReferCard = cbTempTurnCardData[0];

		//搜索炸弹
		BYTE cbTmpResultCount = SearchSameCard(cbCardData, cbCardCount, cbReferCard, 4, &tmpSearchCardResult);
		for (BYTE i = 0; i < cbTmpResultCount; i++)
		{
			pSearchCardResult->cbCardCount[cbResultCount] = tmpSearchCardResult.cbCardCount[i];
			CopyMemory(pSearchCardResult->cbResultCard[cbResultCount], tmpSearchCardResult.cbResultCard[i],
				sizeof(BYTE)*tmpSearchCardResult.cbCardCount[i]);
			cbResultCount++;
		}
	}

	pSearchCardResult->cbSearchCount = cbResultCount;
	return cbResultCount;
}

//同牌搜索
BYTE CGameLogic::SearchSameCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbReferCard, BYTE cbSameCardCount,
	tagSearchCardResult *pSearchCardResult)
{
	//设置结果
	if (pSearchCardResult)
		ZeroMemory(pSearchCardResult, sizeof(tagSearchCardResult));
	BYTE cbResultCount = 0;

	//构造扑克
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount = cbHandCardCount;
	CopyMemory(cbCardData, cbHandCardData, sizeof(BYTE)*cbHandCardCount);

	//排列扑克
	SortCardList(cbCardData, cbCardCount);

	//分析扑克
	tagAnalyseResult AnalyseResult = {};
	AnalysebCardData(cbCardData, cbCardCount, AnalyseResult);

	BYTE cbReferLogicValue = cbReferCard == 0 ? 0 : GetCardLogicValue(cbReferCard);
	BYTE cbBlockIndex = cbSameCardCount - 1;
	do
	{
		for (BYTE i = 0; i < AnalyseResult.cbBlockCount[cbBlockIndex]; i++)
		{
			BYTE cbIndex = (AnalyseResult.cbBlockCount[cbBlockIndex] - i - 1)*(cbBlockIndex + 1);
			if (GetCardLogicValue(AnalyseResult.cbCardData[cbBlockIndex][cbIndex]) > cbReferLogicValue)
			{
				if (pSearchCardResult == NULL) return 1;

				ASSERT(cbResultCount < CountArray(pSearchCardResult->cbCardCount));

				//复制扑克
				CopyMemory(pSearchCardResult->cbResultCard[cbResultCount], &AnalyseResult.cbCardData[cbBlockIndex][cbIndex],
					cbSameCardCount*sizeof(BYTE));
				pSearchCardResult->cbCardCount[cbResultCount] = cbSameCardCount;

				cbResultCount++;
			}
		}

		cbBlockIndex++;
	} while (cbBlockIndex < CountArray(AnalyseResult.cbBlockCount));

	if (pSearchCardResult)
		pSearchCardResult->cbSearchCount = cbResultCount;
	return cbResultCount;
}

//带牌类型搜索(三带一，四带一等)
BYTE CGameLogic::SearchTakeCardType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbReferCard, BYTE cbSameCount, BYTE cbTakeCardCount,
	tagSearchCardResult *pSearchCardResult)
{
	//设置结果
	if (pSearchCardResult)
		ZeroMemory(pSearchCardResult, sizeof(tagSearchCardResult));
	BYTE cbResultCount = 0;

	//效验
	ASSERT(cbSameCount == 3 || cbSameCount == 4);
	ASSERT(cbTakeCardCount == 1 || cbTakeCardCount == 2);
	if (cbSameCount != 3 && cbSameCount != 4)
		return cbResultCount;
	if (cbTakeCardCount != 1 && cbTakeCardCount != 2)
		return cbResultCount;

	//长度判断
	if (cbSameCount == 4 && cbHandCardCount<cbSameCount + cbTakeCardCount * 2 ||
		cbHandCardCount < cbSameCount + cbTakeCardCount)
		return cbResultCount;

	//构造扑克
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount = cbHandCardCount;
	CopyMemory(cbCardData, cbHandCardData, sizeof(BYTE)*cbHandCardCount);

	//排列扑克
	SortCardList(cbCardData, cbCardCount);

	//搜索同张
	tagSearchCardResult SameCardResult = {};
	BYTE cbSameCardResultCount = SearchSameCard(cbCardData, cbCardCount, cbReferCard, cbSameCount, &SameCardResult);

	if (cbSameCardResultCount > 0)
	{
		//分析扑克
		tagAnalyseResult AnalyseResult;
		AnalysebCardData(cbCardData, cbCardCount, AnalyseResult);

		//需要牌数
		BYTE cbNeedCount = cbSameCount + cbTakeCardCount;
		if (cbSameCount == 4) cbNeedCount += cbTakeCardCount;

		//提取带牌
		for (BYTE i = 0; i < cbSameCardResultCount; i++)
		{
			bool bMerge = false;

			BYTE cbHaveTakeCount = 0;

			for (BYTE j = 0; j < CountArray(AnalyseResult.cbBlockCount); j++)
			{
				for (BYTE k = 0; k < AnalyseResult.cbBlockCount[j]; k++)
				{
					//从小到大
					BYTE cbIndex = (AnalyseResult.cbBlockCount[j] - k - 1)*(j + 1);

					//过滤相同牌
					if (GetCardValue(SameCardResult.cbResultCard[i][0]) ==
						GetCardValue(AnalyseResult.cbCardData[j][cbIndex]))
						continue;

					BYTE cbSubCount = cbTakeCardCount - cbHaveTakeCount;

					if (cbSubCount > (j + 1))
					{
						cbSubCount = j + 1;
					}


					//复制带牌
					BYTE cbCount = SameCardResult.cbCardCount[i];
					CopyMemory(&SameCardResult.cbResultCard[i][cbCount], &AnalyseResult.cbCardData[j][cbIndex],
						sizeof(BYTE)*cbSubCount);
					SameCardResult.cbCardCount[i] += cbSubCount;
					cbHaveTakeCount += cbSubCount;
					if (SameCardResult.cbCardCount[i] < cbNeedCount) continue;

					if (pSearchCardResult == NULL) return 1;

					bMerge = true;

					//下一组合
					break;
				}

				if (bMerge) break;
			}

			if (bMerge)
			{
				//复制结果
				CopyMemory(pSearchCardResult->cbResultCard[cbResultCount], SameCardResult.cbResultCard[i],
					sizeof(BYTE)*SameCardResult.cbCardCount[i]);
				pSearchCardResult->cbCardCount[cbResultCount] = SameCardResult.cbCardCount[i];
				cbResultCount++;
			}
		}
	}

	if (pSearchCardResult)
		pSearchCardResult->cbSearchCount = cbResultCount;
	return cbResultCount;
}

//连牌搜索
BYTE CGameLogic::SearchLineCardType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbReferCard, BYTE cbBlockCount, BYTE cbLineCount,
	tagSearchCardResult *pSearchCardResult)
{
	//设置结果
	if (pSearchCardResult)
		ZeroMemory(pSearchCardResult, sizeof(tagSearchCardResult));
	BYTE cbResultCount = 0;

	//定义变量
	BYTE cbLessLineCount = 0;
	if (cbLineCount == 0)
	{
		if (cbBlockCount == 1)
			cbLessLineCount = 5;
		else if (cbBlockCount == 2)
			cbLessLineCount = 3;
		else cbLessLineCount = 2;
	}
	else cbLessLineCount = cbLineCount;

	BYTE cbReferIndex = 2;
	if (cbReferCard != 0)
	{
		ASSERT(GetCardLogicValue(cbReferCard) - cbLessLineCount >= 2);
		cbReferIndex = GetCardLogicValue(cbReferCard) - cbLessLineCount + 1;
	}
	//超过A
	if (cbReferIndex + cbLessLineCount > 14) return cbResultCount;

	//长度判断
	if (cbHandCardCount < cbLessLineCount*cbBlockCount) return cbResultCount;

	//构造扑克
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount = cbHandCardCount;
	CopyMemory(cbCardData, cbHandCardData, sizeof(BYTE)*cbHandCardCount);

	//排列扑克
	SortCardList(cbCardData, cbCardCount);

	//分析扑克
	tagDistributing Distributing = {};
	AnalysebDistributing(cbCardData, cbCardCount, Distributing);

	//搜索顺子
	BYTE cbTmpLinkCount = 0;
	BYTE cbValueIndex = cbReferIndex;
	for (; cbValueIndex<13; cbValueIndex++)
	{
		//继续判断
		if (Distributing.cbDistributing[cbValueIndex][cbIndexCount]<cbBlockCount)
		{
			if (cbTmpLinkCount < cbLessLineCount)
			{
				cbTmpLinkCount = 0;
				continue;
			}
			else cbValueIndex--;
		}
		else
		{
			cbTmpLinkCount++;
			//寻找最长连
			if (cbLineCount == 0) continue;
		}

		if (cbTmpLinkCount >= cbLessLineCount)
		{
			if (pSearchCardResult == NULL) return 1;

			ASSERT(cbResultCount < CountArray(pSearchCardResult->cbCardCount));

			//复制扑克
			BYTE cbCount = 0;
			for (BYTE cbIndex = cbValueIndex + 1 - cbTmpLinkCount; cbIndex <= cbValueIndex; cbIndex++)
			{
				BYTE cbTmpCount = 0;
				for (BYTE cbColorIndex = 0; cbColorIndex<4; cbColorIndex++)
				{
					for (BYTE cbColorCount = 0; cbColorCount < Distributing.cbDistributing[cbIndex][3 - cbColorIndex]; cbColorCount++)
					{
						pSearchCardResult->cbResultCard[cbResultCount][cbCount++] = MakeCardData(cbIndex, 3 - cbColorIndex);

						if (++cbTmpCount == cbBlockCount) break;
					}
					if (cbTmpCount == cbBlockCount) break;
				}
			}

			//设置变量
			pSearchCardResult->cbCardCount[cbResultCount] = cbCount;
			cbResultCount++;

			if (cbLineCount != 0)
			{
				cbTmpLinkCount--;
			}
			else
			{
				cbTmpLinkCount = 0;
			}
		}
	}

	//特殊顺子
	if (cbTmpLinkCount >= cbLessLineCount - 1 && cbValueIndex == 13)
	{
		if (Distributing.cbDistributing[0][cbIndexCount] >= cbBlockCount ||
			cbTmpLinkCount >= cbLessLineCount)
		{
			if (pSearchCardResult == NULL) return 1;

			ASSERT(cbResultCount < CountArray(pSearchCardResult->cbCardCount));

			//复制扑克
			BYTE cbCount = 0;
			BYTE cbTmpCount = 0;
			for (BYTE cbIndex = cbValueIndex - cbTmpLinkCount; cbIndex < 13; cbIndex++)
			{
				cbTmpCount = 0;
				for (BYTE cbColorIndex = 0; cbColorIndex<4; cbColorIndex++)
				{
					for (BYTE cbColorCount = 0; cbColorCount < Distributing.cbDistributing[cbIndex][3 - cbColorIndex]; cbColorCount++)
					{
						pSearchCardResult->cbResultCard[cbResultCount][cbCount++] = MakeCardData(cbIndex, 3 - cbColorIndex);

						if (++cbTmpCount == cbBlockCount) break;
					}
					if (cbTmpCount == cbBlockCount) break;
				}
			}
			//复制A
			if (Distributing.cbDistributing[0][cbIndexCount] >= cbBlockCount)
			{
				cbTmpCount = 0;
				for (BYTE cbColorIndex = 0; cbColorIndex<4; cbColorIndex++)
				{
					for (BYTE cbColorCount = 0; cbColorCount < Distributing.cbDistributing[0][3 - cbColorIndex]; cbColorCount++)
					{
						pSearchCardResult->cbResultCard[cbResultCount][cbCount++] = MakeCardData(0, 3 - cbColorIndex);

						if (++cbTmpCount == cbBlockCount) break;
					}
					if (cbTmpCount == cbBlockCount) break;
				}
			}

			//设置变量
			pSearchCardResult->cbCardCount[cbResultCount] = cbCount;
			cbResultCount++;
		}
	}

	if (pSearchCardResult)
		pSearchCardResult->cbSearchCount = cbResultCount;
	return cbResultCount;
}

//构造扑克
BYTE CGameLogic::MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex)
{
	return (cbColorIndex << 4) | (cbValueIndex + 1);
}
//////////////////////////////////////////////////////////////////////////
