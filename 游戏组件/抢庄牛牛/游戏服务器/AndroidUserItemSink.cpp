#include "Stdafx.h"
#include "AndroidUserItemSink.h"
#include "math.h"
#include <locale>

//////////////////////////////////////////////////////////////////////////

//辅助时间
#define TIME_LESS						2									//最少时间

//游戏时间
#define TIME_USER_CALL_BANKER			3									//叫庄时间
#define TIME_USER_START_GAME			3									//开始时间
#define TIME_USER_ADD_SCORE				3									//下注时间
#define TIME_USER_OPEN_CARD				3									//摊牌时间

//游戏时间
#define IDI_START_GAME					(100)									//开始定时器
#define IDI_CALL_BANKER					(101)									//叫庄定时器
#define IDI_USER_ADD_SCORE				(102)									//下注定时器
#define IDI_OPEN_CARD					(103)									//开牌定时器
#define IDI_DELAY_TIME					(105)									//延时定时器

//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	m_nRobotBankStorageMul=0;
	m_lRobotBankGetScore=0;
	m_lRobotBankGetScoreBanker=0;
	ZeroMemory(m_lRobotScoreRange,sizeof(m_lRobotScoreRange));
	
	//接口变量
	m_pIAndroidUserItem=NULL;
	srand((unsigned)time(NULL));   
	
	m_bForbidOperate = true;
	return;
}

//析构函数
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//接口查询
void * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
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
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	m_bForbidOperate = true;
	return true;
}

//时间消息
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	try
	{
		switch (nTimerID)
		{
		case IDI_DELAY_TIME:
			{
				//开始时间
				UINT nElapse= rand() % TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

				return true;
			}

		case IDI_START_GAME:		//开始定时器
			{			
				//发送准备
				m_pIAndroidUserItem->SendUserReady(NULL,0);

				return true;
			}
		case IDI_CALL_BANKER:		//叫庄定时
			{
				//删除定时器
				m_pIAndroidUserItem->KillGameTimer(IDI_CALL_BANKER);
				//设置变量
				CMD_C_CallBanker CallBanker;
				ZeroMemory(&CallBanker, sizeof(CallBanker));

				bool bBanker = rand () % 100 > 50;
				if (bBanker == false)
				{
					CallBanker.cbBankerTimes = 0;
				}
				else
				{
					BYTE cbTimes[3] = {1,2,4};
					CallBanker.cbBankerTimes = cbTimes[rand() % 3];
				}

				//发送信息
				m_pIAndroidUserItem->SendSocketData(SUB_C_CALL_BANKER,&CallBanker, sizeof(CallBanker));

				return true;
			}
		case IDI_USER_ADD_SCORE:	//加注定时
			{
				//删除定时器
				m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);
				CMD_C_AddScore AddScore;
				ZeroMemory(&AddScore, sizeof(AddScore));

				int lAddScore = 0LL;
				
				lAddScore = m_lAddChip[rand() % MAX_CONFIG];

				if (lAddScore <= 0 || lAddScore > 20)
				{
					lAddScore = 5;
				}
				
				AddScore.nScore = lAddScore;
		
				//发送消息
				m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));

				return true;	
			}
		}
	}
	catch (...)
	{

	}	
	
	return false;
}

//游戏消息
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:	//游戏开始
		{
			//消息处理
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_PLAYER_BET:	//用户下注
		{
			//消息处理
			return OnSubAddScore(pData,wDataSize);
		}
	case SUB_S_DISPATCH_CARD:	//发牌消息
		{
			//消息处理
			return OnSubSendCard(pData,wDataSize);
		}
	case SUB_S_GAME_END:	//游戏结束
		{
			//消息处理
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_CALL_BANKER:	//用户叫庄
		{
			//消息处理
			return OnSubCallBanker(pData,wDataSize);
		}
	case SUB_S_SET_BANKER:	//用户叫庄信息
		{
			//消息处理
			return OnSubCallBankerInfo(pData,wDataSize);
		}
	case SUB_S_CUSTOM_ANDROID:
		{
			//消息处理
			if (wDataSize != sizeof(tagCustomAndroid)) return false;
			tagCustomAndroid * pCustomAndroid = (tagCustomAndroid *)pData;
			//读取配置
			ReadConfigInformation(pCustomAndroid);
			return true;
		}
	}

	return true;
}

//游戏消息
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	return true;
}

//场景消息
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_TK_FREE:		//空闲状态
	case GS_TK_WAIT:
	case GS_TK_END:
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//消息处理
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			BankOperate(2);

			return true;
		}
	case GS_TK_CALL:	// 叫庄状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusCall)) return false;
			CMD_S_StatusCall * pStatusCall=(CMD_S_StatusCall *)pData;

			BankOperate(2);
			
			if (pStatusCall->cbPlayerStatus[m_pIAndroidUserItem->GetChairID()] == FALSE)
				m_bForbidOperate = true;

			if (pStatusCall->cbCallBankerTimes[m_pIAndroidUserItem->GetChairID()] == 0xFF && !m_bForbidOperate)
			{
				//叫庄时间
				UINT nElapse = TIME_LESS + (rand() % TIME_USER_CALL_BANKER);
				m_pIAndroidUserItem->SetGameTimer(IDI_CALL_BANKER, nElapse);
			}

			return true;
		}
	case GS_TK_SCORE:	//下注状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusScore)) return false;
			CMD_S_StatusScore * pStatusScore=(CMD_S_StatusScore *)pData;

			BankOperate(2);
			
			//设置变量
			WORD wMeChairId = m_pIAndroidUserItem->GetChairID();

			if (pStatusScore->cbPlayerStatus[wMeChairId] == FALSE)
				m_bForbidOperate = true;

			//设置筹码
			if (pStatusScore->lBetTimes[wMeChairId] == 0L && wMeChairId != pStatusScore->wBankerUser && !m_bForbidOperate)
			{
				//下注时间
				UINT nElapse = TIME_LESS + (rand() % TIME_USER_ADD_SCORE + 1);
				m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE, nElapse);
			}

			return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//用户进入
void CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户离开
void CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户积分
void CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户状态
void CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户段位
void CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//游戏开始
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	

	return true;
}

//用户下注
bool CAndroidUserItemSink::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
	//机器人不处理用户下注
	return true;
}

//用户强退
bool CAndroidUserItemSink::OnSubPlayerExit(const void * pBuffer, WORD wDataSize)
{
	//机器人不处理用户强退
	return true;
}

//发牌消息
bool CAndroidUserItemSink::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//删除定时器
	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_BANKER);
	m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);

	//设置数据
	WORD wMeChiarID=m_pIAndroidUserItem->GetChairID();
	CopyMemory(m_cbHandCardData, pSendCard->cbCardData[wMeChiarID], sizeof(m_cbHandCardData));

	if (pSendCard->cbPlayerStatus[wMeChiarID] == TRUE)
		m_bForbidOperate = false;

	if (!m_bForbidOperate)
	{
		//开牌时间
		UINT nElapse = TIME_LESS + 2 + (rand() % TIME_USER_CALL_BANKER);
		m_pIAndroidUserItem->SetGameTimer(IDI_CALL_BANKER, nElapse);
	}

	return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验参数
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//删除定时器
	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_BANKER);
	m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);

	//清理变量
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	return true;
}

//用户叫庄
bool CAndroidUserItemSink::OnSubCallBanker(const void * pBuffer, WORD wDataSize)
{
	return true;
}

//用户叫庄信息
bool CAndroidUserItemSink::OnSubCallBankerInfo(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize != sizeof(CMD_S_SetBanker)) return false;
	CMD_S_SetBanker * pSetBanker = (CMD_S_SetBanker *)pBuffer;

	//删除定时器
	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_BANKER);
	m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);

	//设置数据
	WORD wMeChiarID = m_pIAndroidUserItem->GetChairID();
	
	if (wMeChiarID != pSetBanker->wBankerUser && !m_bForbidOperate)
	{
		//开牌时间
		UINT nElapse = TIME_LESS + 2 + (rand() % TIME_USER_ADD_SCORE);
		m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE, nElapse);
	}

	return true;
}

//银行操作
void CAndroidUserItemSink::BankOperate(BYTE cbType)
{
	//变量定义
	SCORE lRobotScore = m_pIAndroidUserItem->GetMeUserItem()->GetUserScore();

	//判断存取
	if (lRobotScore > m_lRobotScoreRange[1])
	{			
		SCORE lSaveScore = 0;

		lSaveScore = SCORE(lRobotScore*m_nRobotBankStorageMul / 100);
		if (lSaveScore > lRobotScore)  lSaveScore = lRobotScore;

		if (lSaveScore > 0)
			m_pIAndroidUserItem->PerformSaveScore(lSaveScore);

		SCORE lRobotNewScore = m_pIAndroidUserItem->GetMeUserItem()->GetUserScore();
	}
	else if (lRobotScore < m_lRobotScoreRange[0])
	{
		CString strInfo;
		
		SCORE lScore=rand()%m_lRobotBankGetScoreBanker+m_lRobotBankGetScore;
		if (lScore > 0)
			m_pIAndroidUserItem->PerformTakeScore(lScore);

		SCORE lRobotNewScore = m_pIAndroidUserItem->GetMeUserItem()->GetUserScore();
		
	}
}

//读取配置
void CAndroidUserItemSink::ReadConfigInformation(tagCustomAndroid *pCustomAndroid)
{
	m_lRobotScoreRange[0] = pCustomAndroid->lRobotScoreMin;
	m_lRobotScoreRange[1] = pCustomAndroid->lRobotScoreMax;

	if (m_lRobotScoreRange[1] < m_lRobotScoreRange[0])	
		m_lRobotScoreRange[1] = m_lRobotScoreRange[0];

	m_lRobotBankGetScore = pCustomAndroid->lRobotBankGet;
	m_lRobotBankGetScoreBanker = pCustomAndroid->lRobotBankGetBanker;
	m_nRobotBankStorageMul = pCustomAndroid->lRobotBankStoMul;
	
	if (m_nRobotBankStorageMul<0||m_nRobotBankStorageMul>100) 
		m_nRobotBankStorageMul =20;

	CopyMemory(m_lAddChip, pCustomAndroid->lAddChip, sizeof(m_lAddChip));
}


//组件创建函数
DECLARE_CREATE_MODULE(AndroidUserItemSink);

//////////////////////////////////////////////////////////////////////////
