#include "StdAfx.h"
#include "TableFrameSink.h"
#include <locale>
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661


/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
#define IDI_CHECK_TABLE				1					//�������
#define IDI_USER_OUT_TIME			6					//��ҳ�ʱ

//���캯��
CTableFrameSink::CTableFrameSink()
{
	m_lBet = 0;
	m_lOneGameScore = 0;
	m_lBetScore = 0;

	//�������
	m_pITableFrame=NULL;
	m_pGameCustomRule=NULL;
	m_pGameServiceOption=NULL;
	m_pGameServiceAttrib=NULL;

	m_lThreeGameScore = 0;

	m_cbAllFreeTimes = 0;
	m_cbUsedFreeTimes = 0;

	m_lJackPotScore = 0;
	return;
}

//��������
CTableFrameSink::~CTableFrameSink()
{

}

//�ӿڲ�ѯ
VOID * CTableFrameSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//��λ����
VOID CTableFrameSink::RepositionSink()
{
	m_lOneGameScore = 0;
	m_lBetScore = 0;
	m_lThreeGameScore = 0;
	m_lJackPotScore = 0;
	return;
}

//��������
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);

	//�����ж�
	if (m_pITableFrame==NULL)
	{
		
		return true;
	}

	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);

	//��Ϸ����
	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();

	//�Զ�����
	ASSERT(m_pITableFrame->GetCustomRule()!=NULL);
	m_pGameCustomRule=(tagCustomRule *)m_pITableFrame->GetCustomRule();

	ASSERT(m_pGameCustomRule != NULL);

	WORD wTableID = m_pITableFrame->GetTableID();



	return true;
}

//��������
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	return 0L;
}

//���ٻ���
SCORE CTableFrameSink::QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return 0;
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{
	//���˥��
	m_pITableFrame->CalculateStorageDeduct();
	return true;
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{				
			//���ֱ���
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));

			if(pIServerUserItem != NULL)
			{			
				//�������
				ScoreInfoArray[0].lScore = m_lBetScore + m_lOneGameScore + m_lThreeGameScore + m_lJackPotScore;
				ScoreInfoArray[0].cbType=(ScoreInfoArray[0].lScore>=0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;

				//����˰��
				ScoreInfoArray[0].lRevenue=m_pITableFrame->CalculateRevenue(0,ScoreInfoArray[0].lScore);
				if (ScoreInfoArray[0].lRevenue>0L) ScoreInfoArray[0].lScore-=ScoreInfoArray[0].lRevenue;

				//�����˲�д���
				if(!pIServerUserItem->IsAndroidUser())
				{
					//���
					WORD wTableID = m_pITableFrame->GetTableID();

					m_pGameServiceOption->pRoomStorageOption.lStorageCurrent -= ScoreInfoArray[0].lScore;

					if(ScoreInfoArray[0].lScore+pIServerUserItem->GetUserScore() < 0)
					{

						ScoreInfoArray[0].lScore  = -pIServerUserItem->GetUserScore();
					}

				}

				//д�����
				m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

				if (m_cbAllFreeTimes == m_cbUsedFreeTimes)
				{
					m_lFreeBet = 0;
					m_cbUsedFreeTimes = 0;
					m_cbAllFreeTimes = 0;
				}
			}

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);			

			m_pITableFrame->StartGame();
			return true;
		}
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);
			return true;
		}
	case GER_USER_LEAVE:	//�û�ǿ��
	case GER_NETWORK_ERROR:	//�����ж�
		{
			//���ֱ���
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));

			if(pIServerUserItem != NULL)
			{
				//�������
				ScoreInfoArray[0].lScore = m_lBetScore + m_lOneGameScore + m_lThreeGameScore + m_lJackPotScore;
				ScoreInfoArray[0].cbType=(ScoreInfoArray[0].lScore>=0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;

				//����˰��
				ScoreInfoArray[0].lRevenue=m_pITableFrame->CalculateRevenue(0,ScoreInfoArray[0].lScore);
				if (ScoreInfoArray[0].lRevenue>0L) ScoreInfoArray[0].lScore-=ScoreInfoArray[0].lRevenue;

				//�����˲�д���
				if(!pIServerUserItem->IsAndroidUser())
				{
					//���
					WORD wTableID = m_pITableFrame->GetTableID();
						
					m_pGameServiceOption->pRoomStorageOption.lStorageCurrent -= ScoreInfoArray[0].lScore;

					if(ScoreInfoArray[0].lScore+pIServerUserItem->GetUserScore() < 0)
					{
						ScoreInfoArray[0].lScore  = -pIServerUserItem->GetUserScore();
					}
				}
				//д�����
				m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

				if (m_cbAllFreeTimes == m_cbUsedFreeTimes)
				{
					m_lFreeBet = 0;
					m_cbUsedFreeTimes = 0;
					m_cbAllFreeTimes = 0;
				}
			}
			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);
			return true;
		}
	}
	return false;
}

//���ͳ���
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:	//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//��עֵ
			StatusFree.cbBetCount = m_pGameCustomRule->cbBetCount;
			CopyMemory(StatusFree.lBetScore, m_pGameCustomRule->lBetScore, sizeof(StatusFree.lBetScore));

			StatusFree.lPlayerScore = pIServerUserItem->GetUserScore();

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//ʱ���¼�
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	return true;
}

//�����¼�
bool CTableFrameSink::OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return true;
}

//�����¼�
bool CTableFrameSink::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	return true;
}

//��Ϸ��Ϣ
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_BETREQ:					//Ѻ�߿�ʼ
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_OneStart));
			if (wDataSize != sizeof(CMD_C_OneStart))
			{
				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_BETFAIL);
				return true;
			}


			//��������
			CMD_C_OneStart * pOneStart=(CMD_C_OneStart *)pData;

			return OnUserBetStart(pIServerUserItem, pOneStart->lBet);
		}
	}

	return true;
}

//�����Ϣ
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�����
bool CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return true; 
}

//�û�����
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	m_lFreeBet = 0;
	m_cbUsedFreeTimes = 0;
	m_cbAllFreeTimes = 0;
	return true;
}

//�û�ͬ��
bool CTableFrameSink::OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{
	return true;
}

//Ѻ�߿�ʼ
bool CTableFrameSink::OnUserBetStart(IServerUserItem * pIServerUserItem, int lBet)
{
	if (lBet < 0 || lBet >= m_pGameCustomRule->cbBetCount)
	{
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_BETFAIL);
		return true;
	}

	DOUBLE lBetScore = m_pGameCustomRule->lBetScore[lBet];

	if (pIServerUserItem->GetUserScore() < lBetScore)
	{
		DOUBLE lUserScore = pIServerUserItem->GetUserScore();
		if ((lBetScore - lUserScore) < 0.01)
		{
			lBetScore = lUserScore;
		}
		else
		{
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_BETFAIL);
			return true;
		}
	}

	m_lBet = lBet;

	m_lBetScore = -lBetScore;

	//ʹ����Ѵ�����������
	if (m_cbAllFreeTimes > 0 && m_cbAllFreeTimes > m_cbUsedFreeTimes)
	{
		m_lBet = m_lFreeBet;
		m_lBetScore = 0;
		m_cbUsedFreeTimes++;
	}

	int nLineMask = 0;

	//������
	tag_SGLB_Parameter SGLB_Parameter;
	ZeroMemory(&SGLB_Parameter, sizeof(SGLB_Parameter));

	SGLB_Parameter.cbOneMode = 1;
	SGLB_Parameter.lBetScore = lBetScore;

	tag_SGLB_Result pSHZ_Result;
	ZeroMemory(&pSHZ_Result, sizeof(pSHZ_Result));
	m_pITableFrame->GetControlResult(&SGLB_Parameter, sizeof(SGLB_Parameter), &pSHZ_Result);

	CopyMemory(m_cbItemInfo, pSHZ_Result.cbItemInfo, sizeof(m_cbItemInfo));

	m_lOneGameScore = m_GameLogic.GetZhongJiangTime(m_cbItemInfo) * (lBetScore / 9);

	if (m_lOneGameScore > 0)
		nLineMask += ENUM_PRIZE_LINE;

	//�����Ӵ���
	int nMaryTimes = m_GameLogic.GetEnterThree(m_cbItemInfo);

	//��Ѵ���
	BYTE cbFreeTimes = m_GameLogic.GetEnterFree(m_cbItemInfo);
	if (cbFreeTimes > 0)
	{
		m_lFreeBet = m_lBet;
		m_cbAllFreeTimes += cbFreeTimes;

		nLineMask += ENUM_PRIZE_LUCKY;
	}

	//JackPot
	m_lJackPotScore = m_GameLogic.GetJackPotScore(m_cbItemInfo,0);

	if (m_lJackPotScore > 0)
		nLineMask += ENUM_PRIZE_JACKPOT;

	CMD_S_GameConclude GameEnd;
	ZeroMemory(&GameEnd, sizeof(GameEnd));

	GameEnd.lBetScore = lBetScore;

	if (nMaryTimes > 0 && nMaryTimes <= 9)
	{
		nLineMask += ENUM_PRIZE_BOX;
		for (int i = 0; i < nMaryTimes; i++)
		{
			//������
			tag_SGLB_Parameter SGLB_Parameter;
			ZeroMemory(&SGLB_Parameter, sizeof(SGLB_Parameter));

			SGLB_Parameter.cbOneMode = 2;
			SGLB_Parameter.lBetScore = lBetScore;

			tag_SGLB_Result pSGLB_Result;
			ZeroMemory(&pSGLB_Result, sizeof(pSGLB_Result));

			m_pITableFrame->GetControlResult(&SGLB_Parameter, sizeof(SGLB_Parameter), &pSGLB_Result);

			if (pSGLB_Result.lBonusScore > 0)
			{
				GameEnd.lBonusScore[i] = pSGLB_Result.lBonusScore;
				m_lThreeGameScore += pSGLB_Result.lBonusScore;
			}
		}
	}

	GameEnd.lGameScore = m_lBetScore + m_lOneGameScore + m_lThreeGameScore + m_lJackPotScore;
	CopyMemory(GameEnd.cbItemInfo, m_cbItemInfo, sizeof(GameEnd.cbItemInfo));
	GameEnd.nAllFreeTimes = m_cbAllFreeTimes;
	GameEnd.nUsedFreeTimes = m_cbUsedFreeTimes;
	GameEnd.lPlayerRestScore = pIServerUserItem->GetUserScore() + GameEnd.lGameScore;
	GameEnd.nLineMask = nLineMask;

	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_BETRESULT, &GameEnd, sizeof(GameEnd));

	OnEventGameConclude(0, pIServerUserItem, GER_NORMAL);

	return true;
}

void CTableFrameSink::SetGameBaseScore(SCORE lBaseScore)
{
}

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
