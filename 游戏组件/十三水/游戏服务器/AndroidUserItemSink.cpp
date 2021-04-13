#include "Stdafx.h"
#include "AndroidUserItemSink.h"
#include "math.h"
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//时间定义
	m_cbTimeRangeCard = 0;

	//接口变量
	m_pIAndroidUserItem=NULL;

	return;
}

//析构函数
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//接口查询
VOID * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//初始接口
bool CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}

//重置接口
bool CAndroidUserItemSink::RepositionSink()
{
	return true;
}

//时间消息
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_RANGE_CARD:  //自动出牌
		{
			AutomaticOutCard();
			return true;
		}
	}

	return false;
}


//游戏消息
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_SENDCARD:			//发送扑克
		{
			return OnSubSendCardEx(pData,wDataSize);
		}
	case SUB_S_GAME_END:				//游戏结束
		{
			return OnSubGameEnd(pData,wDataSize);
		}
	}

	return true;

}

//游戏消息
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return true;
}

//场景消息
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_WK_FREE:	//空闲状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//变量定义
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			m_lCellScore = pStatusFree->lCellScore;
			//时间定义
			m_cbTimeRangeCard = pStatusFree->cbDelayTime;

			//银行操作
			BankOperate(2);

			return true;
		}
	case GS_WK_PLAYING: //游戏状态
		{
			//验证参数
			ASSERT( sizeof( CMD_S_StatusPlay ) == wDataSize );
			if ( sizeof( CMD_S_StatusPlay ) != wDataSize ) return false;

			CMD_S_StatusPlay *pStatusPlay = (CMD_S_StatusPlay *)pData;

			m_lCellScore = pStatusPlay->lCellScore;
			//时间定义
			m_cbTimeRangeCard = pStatusPlay->cbDelayTime;

			//银行操作
			BankOperate(2);

			if (pStatusPlay->cbPlayerStatus[m_pIAndroidUserItem->GetChairID()] != Range_End)
			{
				//洗牌时间
				UINT nElapse = rand() % m_cbTimeRangeCard + TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_RANGE_CARD, nElapse);
			}

			return true;
		}
	case GS_WK_END: //游戏状态
		{
			//验证参数
			ASSERT(sizeof(CMD_S_StatusEnd) == wDataSize);
			if (sizeof(CMD_S_StatusEnd) != wDataSize) return false;

			CMD_S_StatusEnd *pStatusPlay = (CMD_S_StatusEnd *)pData;

			m_lCellScore = pStatusPlay->lCellScore;
			//时间定义
			m_cbTimeRangeCard = pStatusPlay->cbDelayTime;

			//银行操作
			BankOperate(2);
			return true;
		}
	}
	return true;
}

//用户进入
VOID CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户离开
VOID CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户积分
VOID CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户状态
VOID CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户段位
VOID CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}


//发送扑克
bool CAndroidUserItemSink::OnSubSendCardEx( const void * pBuffer, WORD wDataSize )
{
	if (m_pIAndroidUserItem->GetMeUserItem()->GetUserStatus() != US_PLAYING )
	{
		return true;
	}

	ASSERT(wDataSize == sizeof(CMD_S_SendCard));
	if (wDataSize != sizeof(CMD_S_SendCard)) return true;

	//变量定义
	CMD_S_SendCard * pSendCard = (CMD_S_SendCard *)pBuffer;
	IServerUserItem * pIServerUserItem = m_pIAndroidUserItem->GetMeUserItem();


	//设置扑克
	if (m_pIAndroidUserItem->GetMeUserItem()->GetUserStatus() == US_PLAYING)
	{
		CopyMemory(m_bHandCardData, pSendCard->cbCard, sizeof(BYTE)*HAND_CARD_COUNT);
		m_GameLogic.SortCardList(m_bHandCardData, HAND_CARD_COUNT);

		//洗牌时间
		UINT nElapse = rand() % m_cbTimeRangeCard + TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_RANGE_CARD,nElapse);
	}

	return true;
}

//自动代打
void CAndroidUserItemSink::AutomaticOutCard()
{
	IServerUserItem* pIServerUserItem = m_pIAndroidUserItem->GetMeUserItem();

	BYTE cbHandCard[MAX_COUNT];
	BYTE cbHandLen = 0;

	CopyMemory(cbHandCard, m_bHandCardData, HAND_CARD_COUNT);
	cbHandLen = HAND_CARD_COUNT;

	m_GameLogic.SortCardList(cbHandCard, cbHandLen, enDescend);

	//自动摆牌
	BYTE bAutoPutCard[13], bIndex[13];
	ZeroMemory(bAutoPutCard, sizeof(bAutoPutCard));
	ZeroMemory(bIndex, sizeof(bIndex));

	BYTE cbType = m_GameLogic.GetSpecialType(cbHandCard, 13);
	if (cbType != CT_INVALID)
	{
		BYTE cbSegCardData[13] = {0};
		if (0 != m_GameLogic.GetSpecialCardData(cbHandCard, cbHandLen, cbSegCardData))
		{
			CMD_C_ShowCard AllSegmentCard;
			ZeroMemory(&AllSegmentCard, sizeof(AllSegmentCard));

			CopyMemory(AllSegmentCard.cbCardData, cbSegCardData, sizeof(BYTE) * 13);
			m_GameLogic.SortCardList(AllSegmentCard.cbCardData, 13);

			AllSegmentCard.cbCardType[0] = cbType;

			m_pIAndroidUserItem->SendSocketData(SUB_C_PLAYER_SHOWCARD, &AllSegmentCard, sizeof(AllSegmentCard));

			return;
		}
	}

	m_GameLogic.AutoPutCard(cbHandCard, bAutoPutCard);

	//对应序列
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			if (bAutoPutCard[i] == cbHandCard[j])
			{
				bIndex[i] = j;
				break;
			}
		}
	}

	BYTE tmpCard[3][5];
	ZeroMemory(tmpCard, sizeof(tmpCard));

	CopyMemory(tmpCard[0], &bAutoPutCard[0],sizeof(BYTE) * 3);
	m_GameLogic.SortCardList(tmpCard[0], 3, enDescend);

	CopyMemory(tmpCard[1], &bAutoPutCard[3], sizeof(BYTE) * 5);
	m_GameLogic.SortCardList(tmpCard[1], 5, enDescend);

	CopyMemory(tmpCard[2], &bAutoPutCard[8], sizeof(BYTE) * 5);
	m_GameLogic.SortCardList(tmpCard[2], 5, enDescend);

	CMD_C_ShowCard AllSegmentCard;
	ZeroMemory(&AllSegmentCard, sizeof(AllSegmentCard));

	CopyMemory(&AllSegmentCard.cbCardData[0], tmpCard[0], sizeof(BYTE) * 3);
	CopyMemory(&AllSegmentCard.cbCardData[3], tmpCard[1], sizeof(BYTE) * 5);
	CopyMemory(&AllSegmentCard.cbCardData[8], tmpCard[2], sizeof(BYTE) * 5);

	AllSegmentCard.cbCardType[1] = m_GameLogic.GetCardType(tmpCard[0],3);
	AllSegmentCard.cbCardType[2] = m_GameLogic.GetCardType(tmpCard[1], 5);
	AllSegmentCard.cbCardType[3] = m_GameLogic.GetCardType(tmpCard[2], 5);

	m_pIAndroidUserItem->SendSocketData(SUB_C_PLAYER_SHOWCARD, &AllSegmentCard, sizeof(AllSegmentCard));

}

bool CAndroidUserItemSink::OnSubGameEnd( const void * pBuffer, WORD wDataSize )

{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;


	//删除时间
	m_pIAndroidUserItem->KillGameTimer(IDI_RANGE_CARD);

	//扑克变量
	ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));

	return true;
}

//银行操作
void CAndroidUserItemSink::BankOperate(BYTE cbType)
{
	IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
	if(pUserItem->GetUserStatus()>=US_SIT)
	{
		if(cbType==1)
		{
			return;
		}
	}

	LONGLONG lRobotScoreRange[2] = {0};
	lRobotScoreRange[0] = m_lCellScore * 100;
	lRobotScoreRange[1] = m_lCellScore * 500;

	LONGLONG lRobotBankGetScoreBanker = m_lCellScore * 150;
	LONGLONG RobotBankGetScore = m_lCellScore * 450;

	int nRobotBankStorageMul = 30;

	//变量定义
	LONGLONG lRobotScore = pUserItem->GetUserScore();

	//判断存取
	if (lRobotScore > lRobotScoreRange[1])
	{
		LONGLONG lSaveScore=0L;
		lSaveScore = lRobotScore - lRobotScoreRange[1];
		SCORE lScore = rand() % (RobotBankGetScore - lRobotBankGetScoreBanker*2);
		lSaveScore += lScore;
		if (lSaveScore > lRobotScore) lSaveScore = lRobotScore;
		if (lSaveScore > 0) 
			m_pIAndroidUserItem->PerformSaveScore(lSaveScore);
	}
	else if (lRobotScore < lRobotScoreRange[0])
	{
		SCORE lScore = rand() % (RobotBankGetScore - lRobotBankGetScoreBanker) + lRobotBankGetScoreBanker;
		if (lScore > 0)
			m_pIAndroidUserItem->PerformTakeScore(lScore);
	}
	return;
}

//////////////////////////////////////////////////////////////////////////
DECLARE_CREATE_MODULE(AndroidUserItemSink);