#include "Stdafx.h"
#include "AndroidUserItemSink.h"
#include "math.h"
//////////////////////////////////////////////////////////////////////////

//辅助时间
#define TIME_LESS           2                 //最少时间

//游戏时间
#define TIME_USER_CALL_BANKER     3                 //叫庄时间
#define TIME_USER_START_GAME      3                 //开始时间
#define TIME_USER_ADD_SCORE       3                 //下注时间
#define TIME_USER_OPEN_CARD       3                 //摊牌时间

#define TIME_CHECK_BANKER       30                  //摊牌时间

//游戏时间
#define IDI_START_GAME          (100)					//开始定时器
#define IDI_CALL_BANKER         (101)					//叫庄定时器
#define IDI_USER_ADD_SCORE      (102)					//下注定时器
//////////////////////////////////////////////////////////////////////////



//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
  ZeroMemory(m_HandCardData,sizeof(m_HandCardData));

  m_nRobotBankStorageMul=0;
  m_lRobotBankGetScore=0;
  m_lRobotBankGetScoreBanker=0;
  ZeroMemory(m_lRobotScoreRange,sizeof(m_lRobotScoreRange));

  //接口变量
  m_pIAndroidUserItem=NULL;
  srand((unsigned)time(NULL));

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
  if(m_pIAndroidUserItem==NULL)
  {
    return false;
  }
  return true;
}

//重置接口
bool CAndroidUserItemSink::RepositionSink()
{
  ZeroMemory(m_HandCardData,sizeof(m_HandCardData));


  return true;
}

//时间消息
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
  
	switch(nTimerID)
	{
		case IDI_START_GAME:    //开始定时器
		{
			//发送准备
			m_pIAndroidUserItem->SendUserReady(NULL,0);

			return true;
		}
		case IDI_CALL_BANKER:   //叫庄定时
		{
			//设置变量
			CMD_C_CallBanker CallBanker;

			BYTE cbValue = m_GameLogic.GetCardType(m_HandCardData, MAX_COUNT);

			if (cbValue >= 7)
				CallBanker.bBanker = ((rand() % 100) < 80) ? TRUE : FALSE;
			else
				CallBanker.bBanker = ((rand() % 100) < 20) ? TRUE : FALSE;

			//发送信息
			m_pIAndroidUserItem->SendSocketData(SUB_C_CALL_BANKER,&CallBanker,sizeof(CallBanker));

			return true;
		}
		case IDI_USER_ADD_SCORE:  //加注定时
		{
			//下注区域 60%概率选择中大注下
			BYTE cbAddScoreIndex = 0;
			BYTE cbRand1 = rand() % 100;
			BYTE cbRand2 = cbRand1 % 2 + 1;
			
			BYTE cbValue = m_GameLogic.GetCardType(m_HandCardData, MAX_COUNT);

			cbAddScoreIndex = (cbValue >= 7) ? (2 + cbRand2) : cbRand2;

			//发送消息
			CMD_C_AddScore AddScore;
			AddScore.lScore = cbAddScoreIndex;
			m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

			return true;
		}
	}

  

  return true;
}

//游戏消息
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
  switch(wSubCmdID)
  {
    case SUB_S_CALL_BANKER: //用户叫庄
    {
		//消息处理
		return OnSubCallBanker(pData,wDataSize);
    }
    case SUB_S_GAME_START:  //游戏开始
    {
		//消息处理
		return OnSubGameStart(pData,wDataSize);
    }
	case SUB_S_BANKER_END:	//叫庄结果
	{
		//消息处理
		return OnSubBankerEnd(pData, wDataSize);			
	}
    case SUB_S_ADD_SCORE: //用户下注
    {
		//消息处理
		return OnSubAddScore(pData,wDataSize);
    }
    case SUB_S_GAME_END:  //游戏结束
    {
		//消息处理
		return OnSubGameEnd(pData,wDataSize);
    }
	case SUB_S_ANDROID_CONFIG:
	{
		 //效验参数
		 ASSERT(wDataSize == sizeof(tagRobotConfig));
		 if (wDataSize != sizeof(tagRobotConfig)) return true;

		 //变量定义
		 tagRobotConfig * pAndroidConfig = (tagRobotConfig *)pData;

		 m_lRobotScoreRange[0] = pAndroidConfig->lRobotScoreMin;
		 m_lRobotScoreRange[1] = pAndroidConfig->lRobotScoreMax;

		 if (m_lRobotScoreRange[1] < m_lRobotScoreRange[0])
		 {
			 m_lRobotScoreRange[1] = m_lRobotScoreRange[0];
		 }

		 m_lRobotBankGetScore = pAndroidConfig->lRobotBankGet;
		 m_lRobotBankGetScoreBanker = pAndroidConfig->lRobotBankGetBanker;
		 m_nRobotBankStorageMul = pAndroidConfig->lRobotBankStoMul;

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
  switch(cbGameStatus)
  {
    case GAME_STATUS_FREE:    //空闲状态
    {
		//效验数据
		if(wDataSize!=sizeof(CMD_S_StatusFree))
		{
			return true;
		}

		//消息处理
		CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

		BankOperate(2);

		return true;
    }
    case GS_TK_CALL:  // 叫庄状态
    {
		//效验数据
		if(wDataSize!=sizeof(CMD_S_StatusCall))
		{
			return true;
		}
		CMD_S_StatusCall * pStatusCall=(CMD_S_StatusCall *)pData;

		BankOperate(2);

		//始叫用户
		if (pStatusCall->m_cbCallStatus[m_pIAndroidUserItem->GetChairID()] == INVALID_BANKER)
		{
			//叫庄时间
			UINT nElapse = TIME_LESS + (rand() % TIME_USER_CALL_BANKER);
			m_pIAndroidUserItem->SetGameTimer(IDI_CALL_BANKER,nElapse);
		}

		return true;
    }
    case GS_TK_SCORE: //下注状态
    {
		//效验数据
		if(wDataSize!=sizeof(CMD_S_StatusScore))
		{
			return true;
		}
		CMD_S_StatusScore * pStatusScore=(CMD_S_StatusScore *)pData;

		BankOperate(2);

		//设置变量
		WORD wMeChairId = m_pIAndroidUserItem->GetChairID();

		//设置筹码
		if(pStatusScore->lTableScore[wMeChairId]==0L)
		{
			//下注时间
			UINT nElapse = TIME_LESS + (rand() % TIME_USER_ADD_SCORE);
			m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE,nElapse);
		}

		return true;
    }
  }

  ASSERT(FALSE);

  return true;
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

//用户叫庄
bool CAndroidUserItemSink::OnSubCallBanker(const void * pBuffer, WORD wDataSize)
{

	//效验数据
	if (wDataSize != sizeof(CMD_S_Call_Banker))
	{
		return true;
	}
	CMD_S_Call_Banker * pCallBanker = (CMD_S_Call_Banker *)pBuffer;
	return true;
}

//游戏开始
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
  //效验数据
  if(wDataSize!=sizeof(CMD_S_GameStart))
  {
	  return true;
  }
  CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

  BYTE cbUserCount = 0;
  for (WORD i = 0; i < GAME_PLAYER; i++)
  {
	  if (pGameStart->cbPlayStatus[i] == TRUE)
		  cbUserCount++;
  }

  CopyMemory(m_HandCardData, pGameStart->cbCardData, sizeof(m_HandCardData));

  //抢庄时间
  UINT nUserPlayTime = (cbUserCount > 5) ? 2 : 1;
  UINT nElapse = TIME_LESS + (rand() % TIME_USER_CALL_BANKER) + nUserPlayTime;
  m_pIAndroidUserItem->SetGameTimer(IDI_CALL_BANKER, nElapse);
	  
  return true;
}

//叫庄结果
bool CAndroidUserItemSink::OnSubBankerEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize != sizeof(CMD_S_Banker_End))
	{
		return true;
	}
	CMD_S_Banker_End * pBankerEnd = (CMD_S_Banker_End *)pBuffer;

	//设置筹码
	if (pBankerEnd->cbBanker != m_pIAndroidUserItem->GetChairID())
	{
		//下注时间
		UINT nElapse = TIME_LESS + (rand() % TIME_USER_ADD_SCORE);
		m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE, nElapse);
	}

	return true;
}


//用户下注
bool CAndroidUserItemSink::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
  //效验数据
  if(wDataSize!=sizeof(CMD_S_AddScore))
  {
	  return true;
  }
  CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pBuffer;

  return true;
}


//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验参数
	if(wDataSize!=sizeof(CMD_S_GameEnd))
	{
	  return true;
	}
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//删除定时器
	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_BANKER);
	m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);
	m_pIAndroidUserItem->KillGameTimer(IDI_START_GAME);
	

	//清理变量

	ZeroMemory(m_HandCardData,sizeof(m_HandCardData));


  return true;
}

//银行操作
void CAndroidUserItemSink::BankOperate(BYTE cbType)
{
  if(cbType == 3)
  {
    if(rand() % 100 > 33)
    {
      return;
    }
  }
  IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();

  //变量定义
  LONGLONG lRobotScore = pUserItem->GetUserScore();

  {

    //判断存取
    if(lRobotScore > m_lRobotScoreRange[1])
    {
      LONGLONG lSaveScore=0L;

      lSaveScore = LONGLONG(lRobotScore*m_nRobotBankStorageMul/100);
      if(lSaveScore > lRobotScore)
      {
        lSaveScore = lRobotScore;
      }

      if(lSaveScore > 0)
      {
        m_pIAndroidUserItem->PerformSaveScore(lSaveScore);
      }

   
    }
    else if(lRobotScore < m_lRobotScoreRange[0])
    {

      SCORE lScore=rand()%m_lRobotBankGetScoreBanker+m_lRobotBankGetScore;
      if(lScore > 0)
      {
        m_pIAndroidUserItem->PerformTakeScore(lScore);
      }

    }
  }
}

//读取配置
void CAndroidUserItemSink::ReadConfigInformation()
{
}


//组件创建函数
DECLARE_CREATE_MODULE(AndroidUserItemSink);

//////////////////////////////////////////////////////////////////////////
