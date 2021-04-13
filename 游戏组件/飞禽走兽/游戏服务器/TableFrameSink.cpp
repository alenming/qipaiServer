#include "StdAfx.h"
#include "TableFrameSink.h"
#include "DlgCustomRule.h"
#include <locale>
//////////////////////////////////////////////////////////////////////////

//��������
#define SEND_COUNT          300                 //���ʹ���

//��������
#define INDEX_PLAYER        0                 //�м�����
#define INDEX_BANKER        1                 //ׯ������

//��עʱ��
#define IDI_FREE          1                 //����ʱ��
#define TIME_FREE         10                  //����ʱ��

//��עʱ��
#define IDI_PLACE_JETTON      2                 //��עʱ��
#define TIME_PLACE_JETTON     10                  //��עʱ��

//����ʱ��
#define IDI_GAME_END        3                 //����ʱ��
#define TIME_GAME_END       15                  //����ʱ��
#define TIME_GAME_ADD               10                                  //����ʱ��
//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD      CTableFrameSink::m_wPlayerCount=GAME_PLAYER;        //��Ϸ����

//////////////////////////////////////////////////////////////////////////
int  Findstr(char *buf,char findchar,int len)
{
  int i = 0;
  while(i<len)
  {
    if(buf[i++]==findchar)
    {
      return (i-1);
    }
  }
  return -1;
}
char* strGetbuffer(char *buf,int ipos,char *OutBuf,int outLen)
{
  if(ipos<outLen)
  {
    memcpy(OutBuf,buf,ipos);
  }
  else
  {
    memcpy(OutBuf,buf,outLen);
  }
  return  OutBuf;

}

//���캯��
CTableFrameSink::CTableFrameSink()
{
  //����ע��
  ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

  //������ע
  ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

  //�Ͼ���Ϣ
  ZeroMemory(m_lLastAllJettonPlace,sizeof(m_lLastAllJettonPlace));
  ZeroMemory(m_lLastAllJettonArea,sizeof(m_lLastAllJettonArea));

  ZeroMemory(m_bUserListWin, sizeof(m_bUserListWin));
  ZeroMemory(m_lUserListScore, sizeof(m_lUserListScore));
  ZeroMemory(m_cbUserPlayCount, sizeof(m_cbUserPlayCount));

  //��ʼ����
  ZeroMemory(m_lUserStartScore,sizeof(m_lUserStartScore));

  m_lBonus = 0l;

  //��ҳɼ�
  ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
  ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

  //�˿���Ϣ
  ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

  //״̬����
  m_dwJettonTime=0L;
  m_cbShaYuAddMulti = 0;
  //ׯ����Ϣ
  m_ApplyUserArray.RemoveAll();
  m_wCurrentBanker=INVALID_CHAIR;
  m_wBankerTime=0;
  m_lBankerWinScore=0L;
  m_lBankerCurGameScore=0L;
  m_bEnableSysBanker=true;
  m_nRobotListMaxCount =0;
  //��¼����
  ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
  m_nRecordFirst=0;
  m_nRecordLast=0;

  //���Ʊ���
  m_StorageControl =0l;
  m_lStorageCurrent = 0l;
  m_StorageDeduct = 0l;
  m_lCompensateRadio = 0l;

  m_lStorageMax1 = 0;
  m_lStorageMul1 = 0;
  m_lStorageMax2 = 0;
  m_lStorageMul2 = 0;
  //�����˿���
  m_lRobotAreaLimit = 0l;
  m_lRobotBetCount = 0l;

  //ׯ������
  m_lBankerMAX = 0l;
  m_lBankerAdd = 0l;
  m_lBankerScoreMAX = 0l;
  m_lBankerScoreAdd = 0l;
  m_lPlayerBankerMAX = 0l;
  m_bExchangeBanker = true;

  //ʱ�����
  m_cbFreeTime = TIME_FREE;
  m_cbBetTime = TIME_PLACE_JETTON;
  m_cbEndTime = TIME_GAME_END;
  m_cbAddTime = TIME_GAME_ADD;
  //�����˿���
  m_nChipRobotCount = 0;
  ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

  //�������
  srand(time(NULL));

  m_nChips[0] = 1000;
  m_nChips[1] = 10000;
  m_nChips[2] = 100000;
  m_nChips[3] = 500000;
  m_nChips[4] = 1000000;

  m_cbJuControl = FALSE;
  m_cbJuControlTimes = 0;
  m_cbJuControlArea = AREA_COUNT;

  return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
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

//��ʼ��
bool  CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
  //��ѯ�ӿ�
  m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
  if(m_pITableFrame==NULL)
  {
    return false;
  }

  //��ѯ����
  m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
  m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();

  //��ʼģʽ
  m_pITableFrame->SetStartMode(START_MODE_ALL_READY);


  //�����ļ���
  TCHAR szPath[MAX_PATH]=TEXT("");
  GetCurrentDirectory(sizeof(szPath),szPath);
  myprintf(m_szConfigFileName, sizeof(m_szConfigFileName), TEXT("%s\\BaiRenConfig\\AnimalBattleConfig-%d-%d.ini"), szPath, m_pGameServiceOption->wServerID, m_pITableFrame->GetTableID());

  m_nChips[0] = GetPrivateProfileInt(TEXT("��������"), TEXT("nChip0"), 1, m_szConfigFileName);
  m_nChips[1] = GetPrivateProfileInt(TEXT("��������"), TEXT("nChip1"), 10, m_szConfigFileName);
  m_nChips[2] = GetPrivateProfileInt(TEXT("��������"), TEXT("nChip2"), 100, m_szConfigFileName);
  m_nChips[3] = GetPrivateProfileInt(TEXT("��������"), TEXT("nChip3"), 500, m_szConfigFileName);
  m_nChips[4] = GetPrivateProfileInt(TEXT("��������"), TEXT("nChip4"), 1000, m_szConfigFileName);

  m_lUserLimitScore = GetPrivateProfileInt(TEXT("��������"), TEXT("nLimitMax"), 100000, m_szConfigFileName);

  memcpy(m_szGameRoomName, m_pGameServiceOption->szServerName, sizeof(m_szGameRoomName));

  ReadConfigInformation();

  return true;
}

//��λ����
VOID  CTableFrameSink::RepositionSink()
{
  //����ע��
  ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

  //������ע
  ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

  //�������һע
  ZeroMemory(m_lLastAllJettonArea,sizeof(m_lLastAllJettonArea));

  //��ҳɼ�
  ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
  ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

  //�����˿���
  m_nChipRobotCount = 0;
  ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

  ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

  m_cbShaYuAddMulti=0;

  return;
}

//��ѯ�޶�
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{

  if(pIServerUserItem->GetUserStatus() == US_PLAYING)
  {
    return 0L;
  }
  else
  {
    return __max(pIServerUserItem->GetUserScore()-m_pGameServiceOption->lMinTableScore, 0L);
  }
}

//��ѯ�����
bool CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{

  for(int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex)
  {
	  if (m_lUserJettonScore[wChairID][nAreaIndex]>0)
    {
      return true;
    }
  }
  return false;
}

//��Ϸ��ʼ
bool  CTableFrameSink::OnEventGameStart()
{
	//���˥��
	m_pITableFrame->CalculateStorageDeduct();

  //�����˿���
  m_nChipRobotCount = 0;

  //�Թ����
  m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START);

  //��Ϸ���
  for(WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
  {
    IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
    if(pIServerUserItem==NULL)
    {
      continue;
    }
    m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_START);
  }

  return true;
}

//��Ϸ����
bool  CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
  switch(cbReason)
  {
    case GER_NORMAL:    //�������
    {
		tag_FQZS_Parameter FQZS_Parameter;
		ZeroMemory(&FQZS_Parameter, sizeof(FQZS_Parameter));

		FQZS_Parameter.wCurrentBanker = m_wCurrentBanker;
		FQZS_Parameter.cbJuControl = m_cbJuControl;
		FQZS_Parameter.cbJuControlArea = m_cbJuControlArea;
		CopyMemory(FQZS_Parameter.lUserJettonScore, m_lUserJettonScore, sizeof(FQZS_Parameter.lUserJettonScore));
		CopyMemory(FQZS_Parameter.nMultiple, m_nMultiple, sizeof(FQZS_Parameter.nMultiple));

		tag_FQZS_Result pFQZS_Result;
		ZeroMemory(&pFQZS_Result, sizeof(pFQZS_Result));
		m_pITableFrame->GetControlResult(&FQZS_Parameter, sizeof(FQZS_Parameter), &pFQZS_Result);

		CopyMemory(m_cbTableCardArray, pFQZS_Result.cbTableCardArray, sizeof(m_cbTableCardArray));

		if (m_cbJuControlTimes > 0)
			m_cbJuControlTimes--;
		if (m_cbJuControlTimes == 0)
		{
			m_cbJuControl = FALSE;
			m_cbJuControlArea = AREA_COUNT;
		}

		  //�����Ͼ���ע
		  CopyMemory(m_lLastAllJettonPlace,m_lUserJettonScore,sizeof(m_lLastAllJettonPlace));
		  //��ձ������һע
		  ZeroMemory(m_lLastAllJettonArea,sizeof(m_lLastAllJettonArea));

		  //�������
		  SCORE lBankerWinScore = CalculateScore();

		  //��������
		  m_wBankerTime++;

		  //�б������Ϣ
		  for (WORD i = 0; i < GAME_PLAYER; i++)
		  {
			  IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);
			  if (!pServerUser) continue;

			  if (m_lUserWinScore[i] > 0)
			  {
				  m_bUserListWin[i][m_cbUserPlayCount[i]] = true;
			  }

			  SCORE lAddScore = 0;
			  for (BYTE j = 0; j < AREA_COUNT; j++)
			  {
				  lAddScore += m_lUserJettonScore[i][j];
			  }
			  m_lUserListScore[i][m_cbUserPlayCount[i]] = lAddScore;
			  m_cbUserPlayCount[i] ++;

			  if (m_cbUserPlayCount[i] == USER_LIST_COUNT)
				  m_cbUserPlayCount[i] = 0;
		  }

		  //������Ϣ
		  CMD_S_GameEnd GameEnd;
		  ZeroMemory(&GameEnd,sizeof(GameEnd));

		  CopyMemory(GameEnd.lAllAreaScore, m_lAllJettonScore, sizeof(GameEnd.lAllAreaScore));

		  //ׯ����Ϣ
		  GameEnd.lBankerWinScore = m_lBankerCurGameScore;
		  if (m_wCurrentBanker != INVALID_CHAIR)
		  {
			  IServerUserItem *pBankerUser = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
			  GameEnd.lBankerRestScore = pBankerUser->GetUserScore() + GameEnd.lBankerWinScore;
		  }
		  else
			  GameEnd.lBankerRestScore = 100000000;

	  
		  GameEnd.cbTarget[0] = cbGetTargetAnimal(m_cbTableCardArray[0]);
		  GameEnd.cbTarget[1] = cbGetTargetAnimal(m_cbTableCardArray[1]);

		  CopyMemory(GameEnd.cbTurnTarget, m_cbTableCardArray, sizeof(GameEnd.cbTurnTarget));
      
		  for(WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex)
		  {
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
			if(pIServerUserItem == NULL)
			{
			  continue;
			}

			GameEnd.lUserWinScore = m_lUserWinScore[wUserIndex];
			GameEnd.lPlayerRestScore = pIServerUserItem->GetUserScore() + GameEnd.lUserWinScore;

			//������Ϣ
			m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
		  }

		  tagAnPerGameRecord GameRecord;
		  GameRecord.wTableID = m_pITableFrame->GetTableID() + 1;
		  GameRecord.cbAnimal = cbGetTargetAnimal(m_cbTableCardArray[0]);
		  m_pITableFrame->SendTableDataToLookOnUser(SUB_GF_BJL_GAME_RESULT, &GameRecord, sizeof(GameRecord));

		  return true;
    }
    case GER_USER_LEAVE:    //�û��뿪
    case GER_NETWORK_ERROR:
    {
		return true;
      //�м��ж�
      if(m_wCurrentBanker!=wChairID)
      {
        //��������
		SCORE lScore = 0;
		SCORE lRevenue = 0;

        //ͳ�Ƴɼ�
        for(int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex)
        {
			lScore -= m_lUserJettonScore[wChairID][nAreaIndex];
        }

        //д�����
        if(m_pITableFrame->GetGameStatus() != GS_GAME_END)
        {
          for(int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex)
          {
			  if (m_lUserJettonScore[wChairID][nAreaIndex] != 0)
            {
				  m_lAllJettonScore[nAreaIndex] -= m_lUserJettonScore[wChairID][nAreaIndex];
				  m_lUserJettonScore[wChairID][nAreaIndex] = 0;
            }
          }
        }
        else
        {

          for(int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex)
          {
			  if (m_lUserJettonScore[wChairID][nAreaIndex] != 0)
            {
				  m_lUserJettonScore[wChairID][nAreaIndex] = 0;
            }
          }

          //д�����
          if(m_lUserWinScore[wChairID]!=0L)
          {

            tagScoreInfo ScoreInfo[GAME_PLAYER];
            ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
            ScoreInfo[wChairID].cbType=(m_lUserWinScore[wChairID]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
            ScoreInfo[wChairID].lRevenue=m_lUserRevenue[wChairID];
            ScoreInfo[wChairID].lScore=m_lUserWinScore[wChairID];
            m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

            m_lUserWinScore[wChairID] = 0;

			IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
			if (pServerUserItem && !pServerUserItem->IsAndroidUser())
			{
				m_pGameServiceOption->pRoomStorageOption.lStorageCurrent -= ScoreInfo[wChairID].lScore;
			}
          }
        }

      }

      return true;
    }
  }

  return false;
}

//���ͳ���
bool  CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
  switch(cbGameStatus)
  {
    case GAME_STATUS_FREE:      //����״̬
    {
      //��������
      CMD_S_StatusFree StatusFree;
      ZeroMemory(&StatusFree,sizeof(StatusFree));

	  CopyMemory(StatusFree.lChips, m_nChips, sizeof(StatusFree.lChips));
	  CopyMemory(StatusFree.nMultiple, m_nMultiple, sizeof(StatusFree.nMultiple));

      //�ر���ͷ
      m_bJettonState =false;
      //������Ϣ
      StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;

	  if (m_wCurrentBanker != INVALID_CHAIR)
	  {
		  IServerUserItem *pBankerServerItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		  StatusFree.wBankerUserGameID = pBankerServerItem->GetGameID();
		  StatusFree.lBankerScore = pBankerServerItem->GetUserScore();
		  lstrcpyn(StatusFree.szBankerNick, pBankerServerItem->GetNickName(), CountArray(StatusFree.szBankerNick));
		  StatusFree.cbBankerHeadInfo = pBankerServerItem->GetUserInfo()->wFaceID;
	  }
	  //ϵͳ��ׯ
	  else
	  {
		  StatusFree.wBankerUserGameID = -1;
		  StatusFree.lBankerScore = 100000000;
		  _sntprintf(StatusFree.szBankerNick, sizeof(StatusFree.szBankerNick),TEXT("ϵͳ��ׯ"));
		  StatusFree.cbBankerHeadInfo = 0;
	  }

	  StatusFree.nApplyCounts = m_ApplyUserArray.GetCount();

	  StatusFree.lUserRestScore = pIServerUserItem->GetUserScore();

      //ȫ����Ϣ
      DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
      StatusFree.cbTimeLeave=(BYTE)(m_cbFreeTime-__min(dwPassTime,m_cbFreeTime));

	  StatusFree.nFreeTimes = m_cbFreeTime;
	  StatusFree.nPlayTimes = m_cbBetTime;
	  StatusFree.nEndTimes = m_cbEndTime;
	  StatusFree.nAddTimes = m_cbAddTime;

	  StatusFree.lMinXianHong = m_nChips[0];
	  StatusFree.lMaxXianHong = m_lUserLimitScore;

	  FillMemory(StatusFree.cbWinMen, sizeof(StatusFree.cbWinMen), 255);
	  int nIndex = m_nRecordFirst;
	  BYTE cbRecordCount = m_nRecordLast - m_nRecordFirst;

	  if (cbRecordCount > 20)
	  {
		  nIndex = m_nRecordLast - 20;
	  }

	  if (m_nRecordLast < m_nRecordFirst)
	  {
		  nIndex = (m_nRecordLast + MAX_SCORE_HISTORY - 20) % MAX_SCORE_HISTORY;
	  }
	  int nAddNum = 0;
	  while (nIndex != m_nRecordLast)
	  {
		  tagServerGameRecord ServerGameRecord = m_GameRecordArrary[nIndex];
		  StatusFree.cbWinMen[nAddNum++] = ServerGameRecord.bWinMen;
		 
		  nIndex = (nIndex + 1) % MAX_SCORE_HISTORY;
	  }

      //���ͳ���
      bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

	  //����������
	  if (pIServerUserItem->IsAndroidUser())
	  {
		  tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pITableFrame->GetCustomRule();
		  ASSERT(pCustomConfig);

		  m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_ANDROID_CONFIG, &pCustomConfig->CustomAndroid, sizeof(tagCustomAndroid));
	  }

      //����������
      SendApplyUser(pIServerUserItem);
      return bSuccess;
    }
    case GS_PLACE_JETTON:   //��Ϸ״̬
    case GS_GAME_END:     //����״̬
    {
      //��������
      CMD_S_StatusPlay StatusPlay;
	  ZeroMemory(&StatusPlay, sizeof(StatusPlay));

	  CopyMemory(StatusPlay.lChips, m_nChips, sizeof(StatusPlay.lChips));
	  CopyMemory(StatusPlay.nMultiple, m_nMultiple, sizeof(StatusPlay.nMultiple));

      //�ر���ͷ
      m_bJettonState = false;


	  //������Ϣ
	  StatusPlay.lApplyBankerCondition = m_lApplyBankerCondition;

	  if (m_wCurrentBanker != INVALID_CHAIR)
	  {
		  IServerUserItem *pBankerServerItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		  StatusPlay.wBankerUserGameID = pBankerServerItem->GetGameID();
		  StatusPlay.lBankerScore = pBankerServerItem->GetUserScore();
		  lstrcpyn(StatusPlay.szBankerNick, pBankerServerItem->GetNickName(), CountArray(StatusPlay.szBankerNick));
		  StatusPlay.cbBankerHeadInfo = pBankerServerItem->GetUserInfo()->wFaceID;
	  }
	  //ϵͳ��ׯ
	  else
	  {
		  StatusPlay.wBankerUserGameID = -1;
		  StatusPlay.lBankerScore = 100000000;
		  _sntprintf(StatusPlay.szBankerNick, sizeof(StatusPlay.szBankerNick), TEXT("ϵͳ��ׯ"));
		  StatusPlay.cbBankerHeadInfo = 0;
	  }

	  StatusPlay.nApplyCounts = m_ApplyUserArray.GetCount();

	  StatusPlay.lUserRestScore = pIServerUserItem->GetUserScore();

      //ȫ����Ϣ
      DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
      StatusPlay.cbTimeLeave=(BYTE)(m_cbBetTime-__min(dwPassTime,m_cbBetTime));

	  StatusPlay.nFreeTimes = m_cbFreeTime;
	  StatusPlay.nPlayTimes = m_cbBetTime;
	  StatusPlay.nEndTimes = m_cbEndTime;
	  StatusPlay.nAddTimes = m_cbAddTime;

	  StatusPlay.lMinXianHong = m_nChips[0];
	  StatusPlay.lMaxXianHong = m_lUserLimitScore;

      //�����ж�
      if(cbGameStatus==GS_GAME_END)
      {
			BYTE cbEndTime=m_cbEndTime;
			
			if (m_cbTableCardArray[0] == 18)
				cbEndTime += m_cbAddTime;
			StatusPlay.cbTimeLeave=(BYTE)(cbEndTime-__min(dwPassTime,cbEndTime));
      }

	  FillMemory(StatusPlay.cbWinMen, sizeof(StatusPlay.cbWinMen),255);
	  int nIndex = m_nRecordFirst;
	  BYTE cbRecordCount = m_nRecordLast - m_nRecordFirst;

	  if (cbRecordCount > 20)
	  {
		  nIndex = m_nRecordLast - 20;
	  }

	  if (m_nRecordLast < m_nRecordFirst)
	  {
		  nIndex = (m_nRecordLast + MAX_SCORE_HISTORY - 20) % MAX_SCORE_HISTORY;
	  }
	  int nAddNum = 0;
	  while (nIndex != m_nRecordLast)
	  {
		  tagServerGameRecord ServerGameRecord = m_GameRecordArrary[nIndex];
		  StatusPlay.cbWinMen[nAddNum++] = ServerGameRecord.bWinMen;

		  nIndex = (nIndex + 1) % MAX_SCORE_HISTORY;
	  }

      //���ͳ���
      bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

	  //����������
	  if (pIServerUserItem->IsAndroidUser())
	  {
		  tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pITableFrame->GetCustomRule();
		  ASSERT(pCustomConfig);

		  m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_ANDROID_CONFIG, &pCustomConfig->CustomAndroid, sizeof(tagCustomAndroid));
	  }

      //����������
      SendApplyUser(pIServerUserItem);

      return bSuccess;
    }
  }

  return false;
}

//��ʱ���¼�
bool  CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
  switch(wTimerID)
  {
    case IDI_FREE:    //����ʱ��
    {
      //��ʼ��Ϸ
      m_pITableFrame->StartGame();

      //����ʱ��
      m_dwJettonTime=(DWORD)time(NULL);
      m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,m_cbBetTime*1000,1,0L);

      //����״̬
      m_pITableFrame->SetGameStatus(GS_PLACE_JETTON);

	  tagChangeStatus ChangeStatus;
	  ChangeStatus.wTableID = m_pITableFrame->GetTableID() + 1;
	  ChangeStatus.cbGameStatus = m_pITableFrame->GetGameStatus();
	  m_pITableFrame->SendTableDataToLookOnUser(SUB_GF_BJL_CHANGE_STATUS, &ChangeStatus, sizeof(ChangeStatus));
      return true;
    }
    case IDI_PLACE_JETTON:    //��עʱ��
    {
      //״̬�ж�(��ֹǿ���ظ�����)
      if(m_pITableFrame->GetGameStatus()!=GS_GAME_END)
      {
        //����״̬
        m_pITableFrame->SetGameStatus(GS_GAME_END);

        //������Ϸ
        OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);

        //����ʱ��
        m_dwJettonTime=(DWORD)time(NULL);
        
		if (m_cbTableCardArray[0] == 18)
			m_pITableFrame->SetGameTimer(IDI_GAME_END, (m_cbEndTime + m_cbAddTime) * 1000, 1, 0L);
		else
	         m_pITableFrame->SetGameTimer(IDI_GAME_END,m_cbEndTime*1000,1,0L);

		tagChangeStatus ChangeStatus;
		ChangeStatus.wTableID = m_pITableFrame->GetTableID() + 1;
		ChangeStatus.cbGameStatus = m_pITableFrame->GetGameStatus();
		m_pITableFrame->SendTableDataToLookOnUser(SUB_GF_BJL_CHANGE_STATUS, &ChangeStatus, sizeof(ChangeStatus));
      }

      return true;
    }
    case IDI_GAME_END:      //������Ϸ
    {
      tagScoreInfo ScoreInfo[GAME_PLAYER];
      ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
      //д�����
      for(WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID)
      {
        IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);
        if(pIServerUserItem == NULL)
        {
          continue;
        }

        if(m_lUserWinScore[wUserChairID]!=0L)
        {
          ScoreInfo[wUserChairID].cbType=(m_lUserWinScore[wUserChairID]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
          ScoreInfo[wUserChairID].lRevenue=m_lUserRevenue[wUserChairID];
          ScoreInfo[wUserChairID].lScore=m_lUserWinScore[wUserChairID];
        }

		//�����˲�д���
		if (!pIServerUserItem->IsAndroidUser())
		{
			m_pGameServiceOption->pRoomStorageOption.lStorageCurrent -= ScoreInfo[wUserChairID].lScore;
		}
      }

      m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

	  //�����������Ϣ
	  for (WORD i = 0; i < GAME_PLAYER; i++)
	  {
		  IServerUserItem* pServerUser = m_pITableFrame->GetTableUserItem(i);
		  if (pServerUser && m_lUserWinScore[i] > 1000)
		  {
			  CMD_GR_GameRadioMessage GameRadioMessage;
			  ZeroMemory(&GameRadioMessage, sizeof(GameRadioMessage));

			  GameRadioMessage.cbCardType = 0;
			  GameRadioMessage.wKindID = m_pGameServiceOption->wKindID;
			  GameRadioMessage.lScore = m_lUserWinScore[i];
			  lstrcpyn(GameRadioMessage.szNickName, pServerUser->GetNickName(), CountArray(GameRadioMessage.szNickName));
			  m_pITableFrame->SendHallRadioMessage(&GameRadioMessage, sizeof(GameRadioMessage));
		  }
	  }

      //������Ϸ
      m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);

      //ReadConfigInformation();
      //�л�ׯ��
      ChangeBanker(false);

      //����ʱ��
      m_dwJettonTime=(DWORD)time(NULL);
      m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,0L);

      m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_FREE);
      m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_FREE);

	  tagChangeStatus ChangeStatus;
	  ChangeStatus.wTableID = m_pITableFrame->GetTableID() + 1;
	  ChangeStatus.cbGameStatus = m_pITableFrame->GetGameStatus();
	  m_pITableFrame->SendTableDataToLookOnUser(SUB_GF_BJL_CHANGE_STATUS, &ChangeStatus, sizeof(ChangeStatus));

      return true;
    }
  }

  return false;
}

//��ȡ������Ϸ�Ƿ���ע״̬���������ע��
bool CTableFrameSink::OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if (m_pITableFrame->GetGameStatus() == GAME_STATUS_FREE)
		return false;

	//������ע
	for (int nAreaIndex = 0; nAreaIndex < AREA_COUNT; ++nAreaIndex)
	{
		if (m_lUserJettonScore[wChairID][nAreaIndex] != 0)
		{
			return true;
		}
	}

	if (wChairID == m_wCurrentBanker)
		return true;

	return false;
}

//������Ϸ��ȡ��Ϸ��¼
void CTableFrameSink::OnGetGameRecord(VOID *GameRecord)
{
	CMD_GF_AnRoomStatus *pRoomStatus = (CMD_GF_AnRoomStatus *)GameRecord;

	pRoomStatus->tagGameInfo.wTableID = m_pITableFrame->GetTableID() + 1;
	pRoomStatus->tagGameInfo.cbGameStatus = m_pITableFrame->GetGameStatus();

	pRoomStatus->tagTimeInfo.cbFreeTime = m_cbFreeTime;
	pRoomStatus->tagTimeInfo.cbBetTime = m_cbBetTime;
	pRoomStatus->tagTimeInfo.cbEndTime = m_cbEndTime;
	pRoomStatus->tagTimeInfo.cbPassTime = (DWORD)time(NULL) - m_dwJettonTime;
	pRoomStatus->tagTimeInfo.lMinXianHong = m_nChips[0];
	pRoomStatus->tagTimeInfo.lMaxXianHong = m_lUserLimitScore;

	//�ͻ���ֻ��ʾ40��
	int nIndex = m_nRecordFirst;

	pRoomStatus->cbRecordCount = m_nRecordLast - m_nRecordFirst;
	if (pRoomStatus->cbRecordCount > 40)
	{
		pRoomStatus->cbRecordCount = 40;
		nIndex = m_nRecordLast - 40;
	}

	if (m_nRecordLast < m_nRecordFirst)
	{
		nIndex = (m_nRecordLast + MAX_SCORE_HISTORY - 40) % MAX_SCORE_HISTORY;
		pRoomStatus->cbRecordCount = 40;
	}

	int nArrayIndex = 0;

	while (nIndex != m_nRecordLast)
	{
		if (nArrayIndex >= 40)
			break;
		pRoomStatus->GameRecordArrary[nArrayIndex].cbAnimal = m_GameRecordArrary[nIndex].bWinMen;

		nArrayIndex++;
		nIndex = (nIndex + 1) % MAX_SCORE_HISTORY;
	}
}

//��Ϸ��Ϣ����
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
  switch(wSubCmdID)
  {
    case SUB_C_PLACE_JETTON:    //�û���ע
    {
      //Ч������
      ASSERT(wDataSize==sizeof(CMD_C_PlaceJetton));
      if(wDataSize!=sizeof(CMD_C_PlaceJetton))
      {
        return true;
      }

      //�û�Ч��

      tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

      if(pUserData->cbUserStatus!=US_PLAYING)
      {
        return true;
      }

      //��Ϣ����
      CMD_C_PlaceJetton * pPlaceJetton=(CMD_C_PlaceJetton *)pDataBuffer;
      return OnUserPlaceJetton(pUserData->wChairID,pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);
    }
    case SUB_C_APPLY_BANKER:    //������ׯ
    {
      //�û�Ч��
      tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
      if(pUserData->cbUserStatus==US_LOOKON)
      {
        return true;
      }

      return OnUserApplyBanker(pIServerUserItem);
    }
    case SUB_C_CANCEL_BANKER:   //ȡ����ׯ
    {
      //�û�Ч��
      tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
      if(pUserData->cbUserStatus==US_LOOKON)
      {
        return true;
      }

      return OnUserCancelBanker(pIServerUserItem);
    }

  
    case  SUB_C_CLEAN_JETTON:
    {
      //Ч������
      ASSERT(wDataSize==sizeof(CMD_C_CleanMeJetto));
      if(wDataSize!=sizeof(CMD_C_CleanMeJetto))
      {
        return false;
      }
      CMD_C_CleanMeJetto * pCleanMeJetton=(CMD_C_CleanMeJetto *)pDataBuffer;

    

      //ͳ�������ע
      for(int nAreaIndex=0; nAreaIndex<AREA_COUNT; nAreaIndex++)
      {
		  m_lAllJettonScore[nAreaIndex] -= m_lUserJettonScore[pCleanMeJetton->wChairID][nAreaIndex];
		  m_lUserJettonScore[pCleanMeJetton->wChairID][nAreaIndex] = 0;
      }


     
      //��������
      CMD_S_CeanJetton CleanJetton;
      ZeroMemory(&CleanJetton,sizeof(CleanJetton));
      CopyMemory(&CleanJetton.lAllCPlaceScore,m_lAllJettonScore,sizeof(CleanJetton.lAllCPlaceScore));


      //������Ϣ
      m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CEAN_JETTON,&CleanJetton,sizeof(CleanJetton));
      m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CEAN_JETTON,&CleanJetton,sizeof(CleanJetton));

      return true;
    }
	case SUB_C_CONTINUE_JETTON:
	{
		//Ч������
		ASSERT(wDataSize == sizeof(CMD_C_ContinueJetton));
		if (wDataSize != sizeof(CMD_C_ContinueJetton))
		{
			return false;
		}
		CMD_C_ContinueJetton * pLastJetton = (CMD_C_ContinueJetton *)pDataBuffer;

		bool bCheckPlaceJettonIsOk = false;
		if (m_bJettonState == false)
		{
			return true;
		}

		//��һ���
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(pLastJetton->wChairID);
		SCORE lUserScore = pIServerUserItem->GetUserScore();

		SCORE lLastAllJettonArea = 0L;
		SCORE lLastAllJettonPlace = 0L;
		SCORE lUserJettonScore = 0L;
		//ͳ�Ʊ��������ע
		for (int nAreaIndex = 0; nAreaIndex <AREA_COUNT; ++nAreaIndex)
		{
			lLastAllJettonArea += m_lLastAllJettonArea[pLastJetton->wChairID][nAreaIndex];
		}

		//ͳ���Ͼ�����ע
		for (int nAreaIndex = 0; nAreaIndex <AREA_COUNT; ++nAreaIndex)
		{
			lLastAllJettonPlace += m_lLastAllJettonPlace[pLastJetton->wChairID][nAreaIndex];
		}

		//ͳ�Ʊ�������ע
		for (int nAreaIndex = 0; nAreaIndex <AREA_COUNT; ++nAreaIndex)
		{
			lUserJettonScore += m_lUserJettonScore[pLastJetton->wChairID][nAreaIndex];
		}

		bool bIsLast = false;
		//���һע
		for (int nAreaIndex = 0; nAreaIndex <AREA_COUNT; ++nAreaIndex)
		{
			//�����������һע
			if (m_lLastAllJettonArea[pLastJetton->wChairID][nAreaIndex] > 0)
			{
				bIsLast = false;
				break;
			}
			//���������һע
			if (m_lLastAllJettonArea[pLastJetton->wChairID][nAreaIndex] == 0)
			{
				bIsLast = true;
			}
		}

		bCheckPlaceJettonIsOk = true;
		//�ж��û�����Ƿ��㹻
		if (bIsLast) //�����һע
		{
			if (lUserScore - lUserJettonScore < lLastAllJettonPlace)
			{
				bCheckPlaceJettonIsOk = false;
			}
		}
		else
		{
			if (lUserScore - lUserJettonScore < lLastAllJettonArea)
			{
				bCheckPlaceJettonIsOk = false;
			}
		}
		if (!bCheckPlaceJettonIsOk)
		{
			//AfxMessageBox(L"�û���Ҳ�����");
			return true;
		}

		//�û�����
		if (bIsLast) //�����һע
		{
			if (lUserJettonScore + lLastAllJettonPlace > m_lUserLimitScore)
			{
				bCheckPlaceJettonIsOk = false;
			}
		}
		else
		{
			if (lUserJettonScore + lLastAllJettonArea > m_lUserLimitScore)
			{
				bCheckPlaceJettonIsOk = false;
			}
		}
		if (!bCheckPlaceJettonIsOk)
		{
			//AfxMessageBox(L"�����û����ƣ�");
			return true;
		}

		//��������
		if (bIsLast) //�����һע
		{
			for (int nAreaIndex = 0; nAreaIndex <AREA_COUNT; nAreaIndex++)
			{
				if (m_lLastAllJettonPlace[pLastJetton->wChairID][nAreaIndex] + m_lUserJettonScore[pLastJetton->wChairID][nAreaIndex] > m_lAreaLimitScore)
				{
					bCheckPlaceJettonIsOk = false;
					break;
				}
			}
		}
		else  //�����һע
		{
			for (int nAreaIndex = 0; nAreaIndex <AREA_COUNT; nAreaIndex++)
			{
				if (m_lLastAllJettonArea[pLastJetton->wChairID][nAreaIndex] + m_lUserJettonScore[pLastJetton->wChairID][nAreaIndex] > m_lAreaLimitScore)
				{
					bCheckPlaceJettonIsOk = false;
					break;
				}
			}

		}
		if (!bCheckPlaceJettonIsOk)
		{
			//AfxMessageBox(L"�����������ƣ�");
			return true;
		}



		//��������
		SCORE  TemplAllJettonScore[AREA_COUNT];           //ȫ����ע
		SCORE  TemplUserJettonScore[GAME_PLAYER][AREA_COUNT];   //������ע
		SCORE  TemplLastAllJettonPlace[GAME_PLAYER][AREA_COUNT]; //�Ͼ�����ע
		SCORE  TempLastAllJettonArea[GAME_PLAYER][AREA_COUNT];   //���������ע

		CopyMemory(TemplAllJettonScore, m_lAllJettonScore, sizeof(TemplAllJettonScore));
		CopyMemory(TemplUserJettonScore, m_lUserJettonScore, sizeof(m_lUserJettonScore));
		CopyMemory(TemplLastAllJettonPlace, m_lLastAllJettonPlace, sizeof(m_lLastAllJettonPlace));
		CopyMemory(TempLastAllJettonArea, m_lLastAllJettonArea, sizeof(TempLastAllJettonArea));
		/////////////////////////////////////////////////////////////////////

		//�����һע �ظ���һ������ע
		if (bIsLast == true)
		{
			if (bCheckPlaceJettonIsOk == true)
			{
				//��������
				for (int nAreaIndex = 0; nAreaIndex <AREA_COUNT; nAreaIndex++)
				{
					//�Ͼ�����ע ����

					//�������һ�β��� ����   0 (��Ϸ����ʱ���)

					//ȫ����ע
					TemplAllJettonScore[nAreaIndex] += TemplLastAllJettonPlace[pLastJetton->wChairID][nAreaIndex];

					//������ע
					TemplUserJettonScore[pLastJetton->wChairID][nAreaIndex] += TemplLastAllJettonPlace[pLastJetton->wChairID][nAreaIndex];

				}
			}
		}

		//�����һע �ظ��������һע
		else
		{

			//�����һע ֻ��һע
			if (bCheckPlaceJettonIsOk == true)
			{
				//��������
				for (int nAreaIndex = 0; nAreaIndex <AREA_COUNT; nAreaIndex++)
				{
					//�Ͼ�����ע ����     0 (�ڱ����û���һעʱ���)

					//�������һ�β��� ����

					//ȫ����ע
					TemplAllJettonScore[nAreaIndex] += TempLastAllJettonArea[pLastJetton->wChairID][nAreaIndex];

					//������ע
					TemplUserJettonScore[pLastJetton->wChairID][nAreaIndex] += TempLastAllJettonArea[pLastJetton->wChairID][nAreaIndex];

				}

			}
		}


		if (bCheckPlaceJettonIsOk)
		{
			//��������
			CMD_S_ContiueJetton PlaceLastJetton;
			ZeroMemory(&PlaceLastJetton, sizeof(PlaceLastJetton));

			//�������
			PlaceLastJetton.wChairID = pLastJetton->wChairID;
			//��ȡ�û�
			IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(pLastJetton->wChairID);
			if (pIServerUserItem != NULL)
			{
				PlaceLastJetton.cbAndroid = pIServerUserItem->IsAndroidUser() ? TRUE : FALSE;
			}

			//���ñ���
			CopyMemory(m_lAllJettonScore, TemplAllJettonScore, sizeof(TemplAllJettonScore));
			CopyMemory(m_lUserJettonScore, TemplUserJettonScore, sizeof(TemplUserJettonScore));
			CopyMemory(m_lLastAllJettonPlace, TemplLastAllJettonPlace, sizeof(TemplLastAllJettonPlace));
			CopyMemory(m_lLastAllJettonArea, TempLastAllJettonArea, sizeof(TempLastAllJettonArea));

			CopyMemory(PlaceLastJetton.lAllJettonScore, m_lAllJettonScore, sizeof(PlaceLastJetton.lAllJettonScore));

			for (int nAreaIndex = 0; nAreaIndex <AREA_COUNT; nAreaIndex++)
			{
				PlaceLastJetton.lUserJettonScore[nAreaIndex] += m_lUserJettonScore[pLastJetton->wChairID][nAreaIndex];
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CONTINU_JETTON, &PlaceLastJetton, sizeof(PlaceLastJetton));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CONTINU_JETTON, &PlaceLastJetton, sizeof(PlaceLastJetton));
		}
	}
	  //��ȡ����б�
	  case SUB_C_ONLINE_PLAYER:
	  {
		  return OnUserRequestUserList(pIServerUserItem->GetChairID());
	  }
	  case SUB_C_REQUEST_CONTROL:		//������Ϣ����
	  {
		  //Ȩ���ж�
		  if (CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) == false)
			  return true;

		  CMD_S_ControlInfo ControlInfo;
		  ZeroMemory(&ControlInfo, sizeof(ControlInfo));

		  ControlInfo.cbJuControl = m_cbJuControl;
		  ControlInfo.cbJuControlTimes = m_cbJuControlTimes;
		  ControlInfo.cbJuControlArea = m_cbJuControlArea;

		  LONGLONG lUserBetAllScore[GAME_PLAYER] = { 0 };
		  BYTE cbIndex = 0;
		  for (WORD i = 0; i < GAME_PLAYER; i++)
		  {
			  IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);
			  if (pServerUser && !pServerUser->IsAndroidUser())
			  {
				  CopyMemory(ControlInfo.lUserJettonScore[cbIndex], m_lUserJettonScore[i], sizeof(ControlInfo.lUserJettonScore[cbIndex]));
				  lstrcpyn(ControlInfo.szNickName[cbIndex], pServerUser->GetNickName(), sizeof(ControlInfo.szNickName[cbIndex]));

				  for (BYTE j = 0; j < AREA_COUNT; j++)
				  {
					  lUserBetAllScore[cbIndex] += m_lUserJettonScore[i][j];
					  ControlInfo.lAreaJettonScore[j] += m_lUserJettonScore[i][j];
				  }

				  cbIndex++;
			  }
		  }

		  for (WORD i = 0; i < GAME_PLAYER; i++)
		  {
			  for (WORD j = i + 1; j < GAME_PLAYER; j++)
			  {
				  if (lUserBetAllScore[i] < lUserBetAllScore[j])
				  {
					  LONGLONG lTempScore = lUserBetAllScore[i];
					  lUserBetAllScore[i] = lUserBetAllScore[j];
					  lUserBetAllScore[j] = lTempScore;

					  LONGLONG lAreaTempScore[AREA_COUNT] = { 0 };
					  CopyMemory(lAreaTempScore, ControlInfo.lUserJettonScore[i], sizeof(lAreaTempScore));
					  CopyMemory(ControlInfo.lUserJettonScore[i], ControlInfo.lUserJettonScore[j], sizeof(lAreaTempScore));
					  CopyMemory(ControlInfo.lUserJettonScore[j], lAreaTempScore, sizeof(lAreaTempScore));

					  TCHAR szTempNick[32] = { 0 };
					  CopyMemory(szTempNick, ControlInfo.szNickName[i], sizeof(szTempNick));
					  CopyMemory(ControlInfo.szNickName[i], ControlInfo.szNickName[j], sizeof(szTempNick));
					  CopyMemory(ControlInfo.szNickName[j], szTempNick, sizeof(szTempNick));
				  }
			  }
		  }

		  m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_RESPONSE_CONTROL, &ControlInfo, sizeof(ControlInfo));

		  return true;
	  }
	  case SUB_C_CONTROL:				//���ֿ�����Ӯ
	  {
		  //Ȩ���ж�
		  if (CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) == false)
			  return true;

		  CMD_C_ControlWinLost *pControlWinLost = (CMD_C_ControlWinLost *)pDataBuffer;

		  m_cbJuControlArea = pControlWinLost->cbJuControlArea;

		  m_cbJuControl = TRUE;
		  m_cbJuControlTimes = pControlWinLost->cbControlTimes;

		  if (m_cbJuControlTimes < 0 || m_cbJuControlTimes > 5 || m_cbJuControlArea >= AREA_COUNT)
		  {
			  m_cbJuControl = FALSE;
			  m_cbJuControlTimes = 0;
			  m_cbJuControlArea = AREA_COUNT;
		  }

		  return true;
	  }
	  case SUB_C_CANCEL_CONTROL:		//ȡ�����ֿ���
	  {
		  //Ȩ���ж�
		  if (CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) == false)
			  return true;

		  m_cbJuControl = FALSE;
		  m_cbJuControlTimes = 0;
		  m_cbJuControlArea = AREA_COUNT;

		  return true;
	  }
    return true;
  }

  return false;
}


//�����Ϣ����
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
  return false;
}

//�û�����
bool  CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
  //��ʼ����
  m_lUserStartScore[wChairID] = pIServerUserItem->GetUserScore();
  //����ʱ��
  if((bLookonUser==false)&&(m_dwJettonTime==0L))
  {
    m_dwJettonTime=(DWORD)time(NULL);
    m_pITableFrame->SetGameTimer(IDI_FREE,m_cbFreeTime*1000,1,NULL);
    m_pITableFrame->SetGameStatus(GAME_STATUS_FREE);
  }
  return true;
}

//�û�����
bool  CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
  //��ʼ����
  m_lUserStartScore[wChairID] = 0;

  ZeroMemory(m_bUserListWin[wChairID], sizeof(m_bUserListWin[wChairID]));
  ZeroMemory(m_lUserListScore[wChairID], sizeof(m_lUserListScore[wChairID]));
  m_cbUserPlayCount[wChairID] = 0;

  //��¼�ɼ�
  if(bLookonUser==false)
  {
    //�л�ׯ��
    if(wChairID==m_wCurrentBanker)
    {
      ChangeBanker(true);
    }

    //ȡ������
    for(WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
    {
      if(wChairID!=m_ApplyUserArray[i])
      {
        continue;
      }

      //ɾ�����
      m_ApplyUserArray.RemoveAt(i);

      //�������
      CMD_S_CancelBanker CancelBanker;
      ZeroMemory(&CancelBanker,sizeof(CancelBanker));

      tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
      //���ñ���
	  CancelBanker.dwGameID = pIServerUserItem->GetGameID();

      //������Ϣ
      m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
      m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

      break;
    }

    return true;
  }

  return true;
}

//��ע�¼�
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, SCORE lJettonScore)
{
  //Ч�����
	if (lJettonScore != m_nChips[0] && lJettonScore != m_nChips[1] && lJettonScore != m_nChips[2] && lJettonScore != m_nChips[3] && lJettonScore != m_nChips[4])
  {
	  CMD_S_PlaceJetton PlaceJetton;
	  ZeroMemory(&PlaceJetton, sizeof(PlaceJetton));
	  PlaceJetton.cbResult = 2;

	  //������Ϣ
	  m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
    return true;
  }

  if ((cbJettonArea > AREA_COUNT) || cbJettonArea < 0)
  {
	  CMD_S_PlaceJetton PlaceJetton;
	  ZeroMemory(&PlaceJetton, sizeof(PlaceJetton));
	  PlaceJetton.cbResult = 3;

	  //������Ϣ
	  m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
	  return true;
  }

  if(m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON)
  {
	  CMD_S_PlaceJetton PlaceJetton;
	  ZeroMemory(&PlaceJetton, sizeof(PlaceJetton));
	  PlaceJetton.cbResult = 1;

	  //������Ϣ
	  m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
    return true;
  }

  //ׯ���ж�
  if(m_wCurrentBanker==wChairID)
  {
	  CMD_S_PlaceJetton PlaceJetton;
	  ZeroMemory(&PlaceJetton, sizeof(PlaceJetton));
	  PlaceJetton.cbResult = 5;

	  //������Ϣ
	  m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
    return true;
  }
  if(m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR)
  {
    return true;
  }

  //��������
  IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
  SCORE lJettonCount = 0;
  SCORE lJettonPlace = 0;
  for(int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex)
  {
	  lJettonCount += m_lUserJettonScore[wChairID][nAreaIndex];
  }

  //��һ���
  SCORE lUserScore = pIServerUserItem->GetUserScore();

  //�Ϸ�У��
  if(lUserScore < lJettonCount + lJettonScore)
  {
	  CMD_S_PlaceJetton PlaceJetton;
	  ZeroMemory(&PlaceJetton, sizeof(PlaceJetton));
	  PlaceJetton.cbResult = 4;

	  //������Ϣ
	  m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
    return true;
  }
  if(m_lUserLimitScore < lJettonCount + lJettonScore)
  {
	  CMD_S_PlaceJetton PlaceJetton;
	  ZeroMemory(&PlaceJetton, sizeof(PlaceJetton));
	  PlaceJetton.cbResult = 6;

	  //������Ϣ
	  m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
    return true;
  }

  //�ɹ���ʶ
  bool bPlaceJettonSuccess=true;
  //����Ͷ
  m_bJettonState = true;

  //�Ϸ���֤
  if(GetUserMaxJetton(wChairID,cbJettonArea) >= lJettonScore)
  {
    //��������֤
    if(pIServerUserItem->IsAndroidUser())
    {
      //��������
      if(m_lRobotAreaScore[cbJettonArea] + lJettonScore > m_lRobotAreaLimit)
      {
        return true;
      }

      //��Ŀ����
      bool bHaveChip = false;
      for(int i = 0; i < AREA_COUNT; i++)
      {
		  if (m_lUserJettonScore[wChairID][i + 1] != 0)
        {
          bHaveChip = true;
        }
      }

      //ͳ�Ʒ���
      if(bPlaceJettonSuccess)
      {
        m_lRobotAreaScore[cbJettonArea] += lJettonScore;
      }
    }


    if(bPlaceJettonSuccess)
    {

      for(int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex)
      {
        //����Ͼ���ע
		  m_lLastAllJettonPlace[wChairID][nAreaIndex] = 0;
        //��ձ������һע
		  m_lLastAllJettonArea[wChairID][nAreaIndex] = 0;
      }
      //���汾�����һע
	  m_lLastAllJettonArea[wChairID][cbJettonArea] += lJettonScore;

      //ȫ����ע
      m_lAllJettonScore[cbJettonArea] += lJettonScore;

      //������ע
	  m_lUserJettonScore[wChairID][cbJettonArea] += lJettonScore;
    }
  }
  else
  {
    bPlaceJettonSuccess=false;
  }

  if(bPlaceJettonSuccess)
  {
    //��������
    CMD_S_PlaceJetton PlaceJetton;
    ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

	PlaceJetton.cbResult = 0;
    PlaceJetton.cbJettonArea=cbJettonArea;
    PlaceJetton.lJettonScore=lJettonScore;

	IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(wChairID);
	if (pServerUser)
		PlaceJetton.wUserGameID = pServerUser->GetGameID();

    for(int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex)
    {
		PlaceJetton.lUserJettonScore[nAreaIndex] += m_lUserJettonScore[wChairID][nAreaIndex];
    }

	CopyMemory(PlaceJetton.lPlayerJettonScore, m_lAllJettonScore, sizeof(PlaceJetton.lPlayerJettonScore));


	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pUser = m_pITableFrame->GetTableUserItem(i);
		if (pUser == NULL)
			continue;

		if (i == wChairID)
		{
			SCORE lAllBet = 0;
			for (BYTE j = 0; j < AREA_COUNT; j++)
			{
				lAllBet += m_lUserJettonScore[i][j];
			}

			PlaceJetton.lUserRestScore = pUser->GetUserScore() - lAllBet;
		}

		//������Ϣ
		if (i == wChairID)
			m_pITableFrame->SendTableData(i, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
		else
			m_pITableFrame->SendTableData(i, SUB_S_OTHER_JETTON, &PlaceJetton, sizeof(PlaceJetton));


		m_pITableFrame->SendLookonData(i, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
	}
  }
  else
  {
	  CMD_S_PlaceJetton PlaceJetton;
	  ZeroMemory(&PlaceJetton, sizeof(PlaceJetton));
	  PlaceJetton.cbResult = 6;

    //������Ϣ
	  m_pITableFrame->SendTableData(wChairID, SUB_S_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
  }

  return true;
}

//�����˿�
void CTableFrameSink::DispatchTableCard()
{
  //����˿�
  ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

  //��ȡ�˿�
  m_cbTableCardArray[0] = rand() % ANIMAL_COUNT;

  //���� �������
  if(m_cbTableCardArray[0] == 18)
  {
    m_cbTableCardArray[1]=rand() % ANIMAL_COUNT;
	while (m_cbTableCardArray[1] == 18 || m_cbTableCardArray[1] == 11 || m_cbTableCardArray[1] == 25)
	{
		m_cbTableCardArray[1] = rand() % ANIMAL_COUNT;
	}
  }
}

//����ׯ��
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
  //�Ϸ��ж�
	WORD wApplyUserChairID = pIApplyServerUserItem->GetChairID();
	SCORE lUserScore = pIApplyServerUserItem->GetUserScore();
  if(lUserScore<m_lApplyBankerCondition)
  {
	  CMD_S_ApplyBanker ApplyBanker;
	  ZeroMemory(&ApplyBanker, sizeof(ApplyBanker));

	  //���ñ���
	  ApplyBanker.cbResult = 1;

	  //������Ϣ
	  m_pITableFrame->SendTableData(wApplyUserChairID, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
    return true;
  }

  if(pIApplyServerUserItem->IsAndroidUser()&&(m_ApplyUserArray.GetCount())>m_nRobotListMaxCount)
  {
    return true;
  }

  if (m_ApplyUserArray.GetCount() >= 5)
  {
	  CMD_S_ApplyBanker ApplyBanker;
	  ZeroMemory(&ApplyBanker, sizeof(ApplyBanker));

	  //���ñ���
	  ApplyBanker.cbResult = 2;

	  //������Ϣ
	  m_pITableFrame->SendTableData(wApplyUserChairID, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	  return true;
  }

  //�����ж�
  for(INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
  {
    WORD wChairID=m_ApplyUserArray[nUserIdx];
    if(wChairID==wApplyUserChairID)
    {
		CMD_S_ApplyBanker ApplyBanker;
		ZeroMemory(&ApplyBanker, sizeof(ApplyBanker));

		//���ñ���
		ApplyBanker.cbResult = 4;

		//������Ϣ
		m_pITableFrame->SendTableData(wApplyUserChairID, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
      return true;
    }
  }

  if (wApplyUserChairID == m_wCurrentBanker)
  {
	  CMD_S_ApplyBanker ApplyBanker;
	  ZeroMemory(&ApplyBanker, sizeof(ApplyBanker));

	  //���ñ���
	  ApplyBanker.cbResult = 3;

	  //������Ϣ
	  m_pITableFrame->SendTableData(wApplyUserChairID, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	  return true;
  }

  //������Ϣ
  m_ApplyUserArray.Add(wApplyUserChairID);

  //�������
  CMD_S_ApplyBanker ApplyBanker;
  ZeroMemory(&ApplyBanker,sizeof(ApplyBanker));

  //���ñ���
  ApplyBanker.cbResult = 0;
  ApplyBanker.dwGameID = pIApplyServerUserItem->GetGameID();
  lstrcpyn(ApplyBanker.szNickName, pIApplyServerUserItem->GetNickName(), CountArray(ApplyBanker.szNickName));
  ApplyBanker.nApplyCount = m_ApplyUserArray.GetCount();

  //������Ϣ
  m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
  m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

  //�л��ж�
  if(m_pITableFrame->GetGameStatus()==GAME_STATUS_FREE && m_ApplyUserArray.GetCount()==1)
  {
    ChangeBanker(false);
  }

  return true;
}

//ȡ������
bool CTableFrameSink::OnUserCancelBanker(IServerUserItem *pICancelServerUserItem)
{
  //��ǰׯ��
  if(pICancelServerUserItem->GetChairID()==m_wCurrentBanker && m_pITableFrame->GetGameStatus()!=GAME_STATUS_FREE)
  {
    //������Ϣ
    m_pITableFrame->SendGameMessage(pICancelServerUserItem,TEXT("��Ϸ�Ѿ���ʼ��������ȡ����ׯ��"),SMT_CHAT|SMT_EJECT);
    return true;
  }

  //�����ж�
  for(WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
  {
    //��ȡ���
    WORD wChairID=m_ApplyUserArray[i];
    IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);

    //��������
    if(pIServerUserItem==NULL)
    {
      continue;
    }
    if(pIServerUserItem->GetUserID()!=pICancelServerUserItem->GetUserID())
    {
      continue;
    }

    //ɾ�����
    m_ApplyUserArray.RemoveAt(i);

    if(m_wCurrentBanker!=wChairID)
    {
      //�������
      CMD_S_CancelBanker CancelBanker;
      ZeroMemory(&CancelBanker,sizeof(CancelBanker));

      tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
      //���ñ���
	  CancelBanker.dwGameID = pIServerUserItem->GetGameID();

      //������Ϣ
      m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
      m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
    }
    else if(m_wCurrentBanker==wChairID)
    {
      //�л�ׯ��
      m_wCurrentBanker=INVALID_CHAIR;
      ChangeBanker(true);
    }

    return true;
  }

  return true;
}

//����ׯ��
bool CTableFrameSink::ChangeBanker(bool bCancelCurrentBanker)
{
  //�л���ʶ
  bool bChangeBanker=false;

  //ȡ����ǰ
  if(bCancelCurrentBanker)
  {
    for(WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
    {
      //��ȡ���
      WORD wChairID=m_ApplyUserArray[i];

      //��������
      if(wChairID!=m_wCurrentBanker)
      {
        continue;
      }

      //ɾ�����
      m_ApplyUserArray.RemoveAt(i);

      break;
    }

    //����ׯ��
    m_wCurrentBanker=INVALID_CHAIR;

    //�ֻ��ж�
    if(m_pITableFrame->GetGameStatus()==GAME_STATUS_FREE && FindSuitBanker())
    {
      m_wCurrentBanker=m_ApplyUserArray[0];
    }

    //���ñ���
    bChangeBanker=true;
    m_bExchangeBanker = true;
  }
  //��ׯ�ж�
  else if(m_wCurrentBanker!=INVALID_CHAIR)
  {
    //��ȡׯ��
    IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

    if(pIServerUserItem!= NULL)
    {
		SCORE lBankerScore = pIServerUserItem->GetUserScore();

      //�����ж�
      if(m_lPlayerBankerMAX<=m_wBankerTime || lBankerScore<m_lApplyBankerCondition)
      {
        //ׯ�������ж� ͬһ��ׯ�������ֻ�ж�һ��
        if(m_lPlayerBankerMAX <= m_wBankerTime && m_bExchangeBanker)
        {
          bool bScoreMAX = true;
          m_bExchangeBanker = false;

          for(WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
          {
            //��ȡ���
            WORD wChairID = m_ApplyUserArray[i];
            IServerUserItem *pIUserItem = m_pITableFrame->GetTableUserItem(wChairID);
			SCORE lScore = pIUserItem->GetUserScore();

            if(wChairID != m_wCurrentBanker && lBankerScore <= lScore)
            {
              bScoreMAX = false;
              break;
            }
          }

          LONGLONG wAddTimes=0;
          if(bScoreMAX || (lBankerScore > m_lBankerScoreMAX && m_lBankerScoreMAX != 0l))
          {
            if(bScoreMAX)
            {
              wAddTimes=m_lBankerAdd;
            }
            if(lBankerScore > m_lBankerScoreMAX && m_lBankerScoreMAX != 0l)
            {
              wAddTimes+=m_lBankerScoreAdd;
            }

            m_lPlayerBankerMAX+=wAddTimes;
            tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
            //��ʾ��Ϣ
            TCHAR szTipMsg[128] = {};
            if(wAddTimes==m_lBankerAdd)
            {
              myprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]�ķ�����������������ׯ��ң����%d�ζ�����ׯ����!"),pUserInfo->szNickName,wAddTimes);
            }
            else if(wAddTimes==m_lBankerScoreAdd)
            {
              myprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]�ķ�������[%I64d]�����%d�ζ�����ׯ����!"),pUserInfo->szNickName,m_lBankerScoreMAX,wAddTimes);
            }
            else if(wAddTimes==(m_lBankerScoreAdd+m_lBankerAdd))
            {
              myprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]�ķ�������[%I64d]�ҳ�������������ׯ��ң����%d�ζ�����ׯ����!"),pUserInfo->szNickName,m_lBankerScoreMAX,wAddTimes);
            }

            if(wAddTimes != 0)
            {
              //������Ϣ
              SendGameMessage(INVALID_CHAIR,szTipMsg);
              return true;
            }
            return true;
          }
        }

        //�������
        for(WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
        {
          //��ȡ���
          WORD wChairID=m_ApplyUserArray[i];

          //��������
          if(wChairID!=m_wCurrentBanker)
          {
            continue;
          }

          //ɾ�����
          m_ApplyUserArray.RemoveAt(i);

          break;
        }

        //����ׯ��
        m_wCurrentBanker=INVALID_CHAIR;

        //�ֻ��ж�
        if(FindSuitBanker())
        {
          m_wCurrentBanker=m_ApplyUserArray[0];
        }

        bChangeBanker=true;
        m_bExchangeBanker = true;
        tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
        //��ʾ��Ϣ
        TCHAR szTipMsg[128];
        if(lBankerScore<m_lApplyBankerCondition)
        {
          myprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]��������(%I64d)��ǿ�л�ׯ!"),pUserInfo->szNickName,m_lApplyBankerCondition);
        }
        else
        {
          myprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]��ׯ�����ﵽ(%d)��ǿ�л�ׯ!"),pUserInfo->szNickName,m_lPlayerBankerMAX);
        }

        //������Ϣ
        SendGameMessage(INVALID_CHAIR,szTipMsg);
      }
    }
    else
    {
      for(WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
      {
        //��ȡ���
        WORD wChairID=m_ApplyUserArray[i];

        //��������
        if(wChairID!=m_wCurrentBanker)
        {
          continue;
        }

        //ɾ�����
        m_ApplyUserArray.RemoveAt(i);

        break;
      }
      //����ׯ��
      m_wCurrentBanker=INVALID_CHAIR;
    }

  }
  //ϵͳ��ׯ
  else if(m_wCurrentBanker==INVALID_CHAIR &&FindSuitBanker())
  {
    m_wCurrentBanker=m_ApplyUserArray[0];
    bChangeBanker=true;
    m_bExchangeBanker = true;
  }

  //�л��ж�
  if(bChangeBanker)
  {
    //�����ׯ��
    m_lPlayerBankerMAX = m_lBankerMAX;

    //���ñ���
    m_wBankerTime = 0;
    m_lBankerWinScore=0;

    //������Ϣ
    CMD_S_ChangeBanker sChangeBanker;
    ZeroMemory(&sChangeBanker,sizeof(sChangeBanker));
    
	if (m_wCurrentBanker == INVALID_CHAIR)
	{
		sChangeBanker.dwGameID = -1;
		sChangeBanker.lBankerScore = 100000000;
		_sntprintf(sChangeBanker.szNickName, sizeof(sChangeBanker),TEXT("ϵͳ��ׯ"));
		sChangeBanker.cbBankerHeadInfo = 0;
	}
	else
	{
		IServerUserItem *pBankerUser = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		sChangeBanker.dwGameID = pBankerUser->GetGameID();
		sChangeBanker.lBankerScore = pBankerUser->GetUserScore();
		lstrcpyn(sChangeBanker.szNickName, pBankerUser->GetNickName(), CountArray(sChangeBanker.szNickName));
		sChangeBanker.cbBankerHeadInfo = pBankerUser->GetUserInfo()->wFaceID;
	}

    m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&sChangeBanker,sizeof(sChangeBanker));
    m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&sChangeBanker,sizeof(sChangeBanker));
  }

  return bChangeBanker;
}

//�����û��б�
bool CTableFrameSink::OnUserRequestUserList(WORD wChairID)
{
	CMD_S_UserListInfo UserListInfo[GAME_PLAYER];
	ZeroMemory(UserListInfo, sizeof(UserListInfo));

	BYTE cbListUserCount = 0;

	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);
		if (!pServerUser) continue;

		SCORE lAddScore = 0;
		BYTE cbWinCount = 0;

		for (BYTE j = 0; j < USER_LIST_COUNT; j++)
		{
			if (m_bUserListWin[i][j])
				cbWinCount++;
			lAddScore += m_lUserListScore[i][j];
		}

		UserListInfo[cbListUserCount].wWinNum = cbWinCount;
		UserListInfo[cbListUserCount].lAllBet = lAddScore;
		UserListInfo[cbListUserCount].lUserScore = pServerUser->GetUserScore();
		lstrcpyn(UserListInfo[cbListUserCount].szNickName, pServerUser->GetNickName(), CountArray(UserListInfo[cbListUserCount].szNickName));
		UserListInfo[cbListUserCount].wFaceID = pServerUser->GetUserInfo()->wFaceID;
		cbListUserCount++;
	}

	//�ҳ�һ����ʤ�������ķŵ�һ��
	for (WORD i = 1; i < cbListUserCount; i++)
	{
		bool bExchange = false;
		if (UserListInfo[i].wWinNum > UserListInfo[0].wWinNum)
			bExchange = true;
		else if (UserListInfo[i].wWinNum == UserListInfo[0].wWinNum)
		{
			if (UserListInfo[i].lAllBet > UserListInfo[0].lAllBet)
				bExchange = true;
		}

		if (bExchange)
		{
			CMD_S_UserListInfo UserTempInfo;
			ZeroMemory(&UserTempInfo, sizeof(UserTempInfo));

			CopyMemory(&UserTempInfo, &UserListInfo[i], sizeof(UserTempInfo));
			CopyMemory(&UserListInfo[i], &UserListInfo[0], sizeof(UserTempInfo));
			CopyMemory(&UserListInfo[0], &UserTempInfo, sizeof(UserTempInfo));
		}
	}

	//ʣ�µİ���ע������
	for (WORD i = 1; i < cbListUserCount; i++)
	{
		for (WORD j = i + 1; j < cbListUserCount; j++)
		{
			if (UserListInfo[j].lAllBet > UserListInfo[i].lAllBet)
			{
				CMD_S_UserListInfo UserTempInfo;
				ZeroMemory(&UserTempInfo, sizeof(UserTempInfo));

				CopyMemory(&UserTempInfo, &UserListInfo[i], sizeof(UserTempInfo));
				CopyMemory(&UserListInfo[i], &UserListInfo[j], sizeof(UserTempInfo));
				CopyMemory(&UserListInfo[j], &UserTempInfo, sizeof(UserTempInfo));
			}
		}
	}

	//Ȼ��ʼ�����ˡ�����

	BYTE cbMaxTimes = cbListUserCount / 20;
	if (cbListUserCount % 20 != 0)
		cbMaxTimes += 1;

	for (int i = 0; i < cbMaxTimes; i++)
	{
		BYTE cbRestCount = 20;
		if (i == (cbMaxTimes - 1))
			cbRestCount = cbListUserCount % 20;

		CMD_S_UserList UpdateUserList;
		ZeroMemory(&UpdateUserList, sizeof(UpdateUserList));
		UpdateUserList.wCount = cbRestCount;
		if (i != (cbMaxTimes - 1))
			UpdateUserList.bEnd = false;
		else
			UpdateUserList.bEnd = true;

		for (int j = 0; j < cbRestCount; j++)
		{
			UpdateUserList.cbIndex[j] = i * 20 + j;
		}

		for (int j = 0; j < cbRestCount; j++)
			UpdateUserList.cbWinTimes[j] = UserListInfo[i * 20 + j].wWinNum;
		for (int j = 0; j < cbRestCount; j++)
			UpdateUserList.lBetScore[j] = UserListInfo[i * 20 + j].lAllBet;
		for (int j = 0; j < cbRestCount; j++)
			UpdateUserList.lUserScore[j] = UserListInfo[i * 20 + j].lUserScore;


		for (int j = 0; j < cbRestCount; j++)
			lstrcpyn(UpdateUserList.szUserNick[j], UserListInfo[i * 20 + j].szNickName, CountArray(UpdateUserList.szUserNick[j]));

		for (int j = 0; j < cbRestCount; j++)
			UpdateUserList.wFaceID[j] = UserListInfo[i * 20 + j].wFaceID;

		m_pITableFrame->SendTableData(wChairID, SUB_S_ONLINE_PLAYER, &UpdateUserList, sizeof(UpdateUserList));
		m_pITableFrame->SendLookonData(wChairID, SUB_S_ONLINE_PLAYER, &UpdateUserList, sizeof(UpdateUserList));
	}
	


	return true;
}

//����ׯ��
void CTableFrameSink::SendApplyUser(IServerUserItem *pRcvServerUserItem)
{
  for(INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
  {
    WORD wChairID=m_ApplyUserArray[nUserIdx];

    //��ȡ���
    IServerUserItem *pServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
    if(!pServerUserItem)
    {
      continue;
    }

    //ׯ���ж�
    if(pServerUserItem->GetChairID()==m_wCurrentBanker)
    {
      continue;
    }

    //�������
    CMD_S_ApplyBanker ApplyBanker;
	ApplyBanker.cbResult = 0;
	ApplyBanker.dwGameID = pServerUserItem->GetGameID();
	lstrcpyn(ApplyBanker.szNickName, pServerUserItem->GetNickName(), CountArray(ApplyBanker.szNickName));
	ApplyBanker.nApplyCount = m_ApplyUserArray.GetCount();

    //������Ϣ
    m_pITableFrame->SendUserItemData(pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
  }
}

//�û�����
bool  CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{
  //�л�ׯ��
  if(wChairID==m_wCurrentBanker)
  {
    ChangeBanker(true);
  }

  //ȡ������
  for(WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
  {
    if(wChairID!=m_ApplyUserArray[i])
    {
      continue;
    }

    //ɾ�����
    m_ApplyUserArray.RemoveAt(i);

    //�������
    CMD_S_CancelBanker CancelBanker;
    ZeroMemory(&CancelBanker,sizeof(CancelBanker));
    tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
    //���ñ���
	CancelBanker.dwGameID = pIServerUserItem->GetGameID();

    //������Ϣ
    m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
    m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

    break;
  }

  return true;
}

//�����ע
SCORE CTableFrameSink::GetUserMaxJetton(WORD wChairID, BYTE Area)
{
  IServerUserItem *pIMeServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
  if(NULL==pIMeServerUserItem)
  {
    return 0L;
  }

  //����ע��
  SCORE lNowJetton = 0;
  ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
  for(int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex)
  {
	  lNowJetton += m_lUserJettonScore[wChairID][nAreaIndex];
  }

  //ׯ�ҽ��
  SCORE lBankerScore = 100000000;
  if(m_wCurrentBanker!=INVALID_CHAIR)
  {
    IServerUserItem *pIUserItemBanker=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
    if(NULL!=pIUserItemBanker)
    {
      lBankerScore=pIUserItemBanker->GetUserScore();
    }
  }

  BYTE bcWinArea[AREA_COUNT] = {TRUE};
  SCORE LosScore = 0;

  //����ׯ��ͨ������������
  for(int nAreaIndex=0; nAreaIndex<AREA_COUNT; ++nAreaIndex)
  {
	  if (nAreaIndex == AREA_FEI_QIN || nAreaIndex == AREA_ZOU_SHOU)
		  continue;

		LosScore+=m_lAllJettonScore[nAreaIndex]*(m_nMultiple[nAreaIndex]);

		//����
		if (nAreaIndex >= AREA_YAN_ZI && nAreaIndex <= AREA_LAO_YING)
		{
			LosScore += m_lAllJettonScore[nAreaIndex] * (m_nMultiple[AREA_FEI_QIN]);
		}		
		else if (nAreaIndex >= AREA_TU_ZI && nAreaIndex <= AREA_SHI_ZI)
		{
			LosScore += m_lAllJettonScore[nAreaIndex] * (m_nMultiple[AREA_ZOU_SHOU]);
		}
  }
  lBankerScore = lBankerScore - LosScore;

  if(lBankerScore < 0)
  {
	  return 0;
  }

  //��������
  SCORE lMeMaxScore = min((pIMeServerUserItem->GetUserScore() - lNowJetton), m_lUserLimitScore);

  //��������
  lMeMaxScore=min(lMeMaxScore,m_lAreaLimitScore);

  //ׯ������
  lMeMaxScore=min(lMeMaxScore,lBankerScore/(m_nMultiple[Area]));

  //��������
  lMeMaxScore = max(lMeMaxScore, 0);

  return (SCORE)(lMeMaxScore);
}
//����÷�
SCORE CTableFrameSink::CalculateScore()
{
	BYTE cbWinArea[AREA_COUNT] = {FALSE};
	//���裬��������������
	if (m_cbTableCardArray[0] == 18)
	{
		cbWinArea[AREA_SHA_YU] = TRUE;
		if (m_cbTableCardArray[1] != 18 && m_cbTableCardArray[1] != 11 && m_cbTableCardArray[1] != 25)
		{
			cbWinArea[cbGetAreaAnimal(m_cbTableCardArray[1])] = TRUE;

			BYTE cbHotAnimal = cbGetAreaAnimal(m_cbTableCardArray[1]);
			if (cbHotAnimal >= AREA_YAN_ZI && cbHotAnimal <= AREA_LAO_YING)
				cbWinArea[AREA_FEI_QIN] = TRUE;
			else if (cbHotAnimal >= AREA_TU_ZI && cbHotAnimal <= AREA_SHI_ZI)
				cbWinArea[AREA_ZOU_SHOU] = TRUE;
		}
	}
	//ͨɱ
	else if (m_cbTableCardArray[0] == 11)
	{
		ZeroMemory(cbWinArea, sizeof(cbWinArea));
	}
	//ͨ��
	else if (m_cbTableCardArray[0] == 25)
	{
		FillMemory(cbWinArea, sizeof(cbWinArea), TRUE);
	}
	else
	{
		cbWinArea[cbGetAreaAnimal(m_cbTableCardArray[0])] = TRUE;
		BYTE cbHotAnimal = cbGetAreaAnimal(m_cbTableCardArray[0]);
		if (cbHotAnimal >= AREA_YAN_ZI && cbHotAnimal <= AREA_LAO_YING)
			cbWinArea[AREA_FEI_QIN] = TRUE;
		else if (cbHotAnimal >= AREA_TU_ZI && cbHotAnimal <= AREA_SHI_ZI)
			cbWinArea[AREA_ZOU_SHOU] = TRUE;

		if (cbHotAnimal == AREA_JIN_SHA)
			cbWinArea[AREA_SHA_YU] = TRUE;
	}

    //��Ϸ��¼
    tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];

	GameRecord.bWinMen = cbGetTargetAnimal(m_cbTableCardArray[0]);

    //�ƶ��±�
    m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
    if(m_nRecordLast == m_nRecordFirst)
    {
      m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
    }

    //ׯ������
	SCORE lBankerWinScore = 0;

    //��ҳɼ�
    ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
    ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	SCORE lUserLostScore[GAME_PLAYER];
    ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	SCORE lUserWinScore = 0;

    //�������
    for(WORD wChairID=0; wChairID<GAME_PLAYER; wChairID++)
    {
      //ׯ���ж�
      if(m_wCurrentBanker==wChairID)
      {
        continue;
      }

      //��ȡ�û�
      IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
      if(pIServerUserItem==NULL)
      {
        continue;
      }

	  for (WORD wAreaIndex = 0; wAreaIndex < AREA_COUNT; ++wAreaIndex)
	  {
		  if (m_lUserJettonScore[wChairID][wAreaIndex] > 0)
		  {
			  if (cbWinArea[wAreaIndex] == TRUE)
			  {
				  SCORE Score = (m_nMultiple[wAreaIndex] - 1) * m_lUserJettonScore[wChairID][wAreaIndex];

				  if (wAreaIndex == AREA_SHA_YU && cbWinArea[AREA_JIN_SHA] == TRUE)
					  Score = (m_nMultiple[AREA_JIN_SHA] - 1) * m_lUserJettonScore[wChairID][wAreaIndex];

				  m_lUserWinScore[wChairID] += Score;
				  lBankerWinScore -= Score;
			  }
			  else
			  {
				  m_lUserWinScore[wChairID] -= m_lUserJettonScore[wChairID][wAreaIndex];
				  lBankerWinScore += m_lUserJettonScore[wChairID][wAreaIndex];
			  }
		  }
	  }
     
    }

    if(m_wCurrentBanker!=INVALID_CHAIR)
    {
      //ׯ�ҳɼ�
      m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;
    }

    //����˰��
    for(WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
    {
      IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
      if(pIServerUserItem==NULL)
      {
        continue;
      }
      if(wChairID != m_wCurrentBanker && m_lUserWinScore[wChairID] > 0)
      {
        SCORE Revenue = m_pITableFrame->CalculateRevenue(wChairID,m_lUserWinScore[wChairID]);
        m_lUserRevenue[wChairID] = Revenue;
        m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];

      }
      else if(m_wCurrentBanker!=INVALID_CHAIR && wChairID == m_wCurrentBanker && lBankerWinScore > 0)
      {
        SCORE Revenue = m_pITableFrame->CalculateRevenue(wChairID,m_lUserWinScore[m_wCurrentBanker]);
        m_lUserRevenue[m_wCurrentBanker] = Revenue;
        m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
      }
    }

    //�ۼƻ���
    m_lBankerWinScore += lBankerWinScore;

    //��ǰ����
    m_lBankerCurGameScore=lBankerWinScore;

    return lBankerWinScore;
}

//���ͼ�¼
void CTableFrameSink::SendGameRecord(IServerUserItem *pIServerUserItem)
{
  WORD wBufferSize=0;
  BYTE cbBuffer[SOCKET_TCP_BUFFER];
  int nIndex = m_nRecordFirst;
  BYTE cbRecordCount = m_nRecordLast - m_nRecordFirst;

  if (cbRecordCount > 40)
  {
	  nIndex = m_nRecordLast - 40;
  }

  if (m_nRecordLast < m_nRecordFirst)
  {
	  nIndex = (m_nRecordLast + MAX_SCORE_HISTORY - 40) % MAX_SCORE_HISTORY;
  }

  while(nIndex != m_nRecordLast)
  {
    if((wBufferSize+sizeof(tagServerGameRecord))>sizeof(cbBuffer))
    {
      m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
      wBufferSize=0;
    }
    CopyMemory(cbBuffer+wBufferSize,&m_GameRecordArrary[nIndex],sizeof(tagServerGameRecord));
    wBufferSize+=sizeof(tagServerGameRecord);

    nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
  }
  if(wBufferSize>0)
  {
    m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
  }
}

//������Ϣ
void CTableFrameSink::SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg)
{
  if(wChairID==INVALID_CHAIR)
  {
    //��Ϸ���
    for(WORD i=0; i<GAME_PLAYER; ++i)
    {
      IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
      if(pIServerUserItem==NULL)
      {
        continue;
      }
      m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_CHAT);
    }

    //�Թ����
    WORD wIndex=0;
    do
    {
      IServerUserItem *pILookonServerUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
      if(pILookonServerUserItem==NULL)
      {
        break;
      }

      m_pITableFrame->SendGameMessage(pILookonServerUserItem,pszTipMsg,SMT_CHAT);

    }
    while(true);
  }
  else
  {
    IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
    if(pIServerUserItem!=NULL)
    {
      m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_CHAT|SMT_EJECT);
    }
  }
}


//////////////////////////////////////////////////////////////////////////
//����ׯ��
bool CTableFrameSink::FindSuitBanker()
{

  bool bReturn=false;
  while(m_ApplyUserArray.GetCount()>0)
  {
    IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(m_ApplyUserArray[0]);
    if(pIServerUserItem == NULL)
    {
      m_ApplyUserArray.RemoveAt(0);
    }
    else
    {
      //��ׯ�ж�
		SCORE lUserScore = pIServerUserItem->GetUserScore();
      if(lUserScore<m_lApplyBankerCondition)
      {
        //������Ϣ
        CMD_S_CancelBanker CancelBanker;
        ZeroMemory(&CancelBanker,sizeof(CancelBanker));
        tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
        //���ñ���
		CancelBanker.dwGameID = pIServerUserItem->GetGameID();

        //������Ϣ
        m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
        m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

        //��ʾ��Ϣ
        TCHAR szTipMsg[128];
        myprintf(szTipMsg,CountArray(szTipMsg),TEXT("������Ľ����������ׯ����������%I64d��,���Զ���ׯ��"),m_lApplyBankerCondition);
        SendGameMessage(m_ApplyUserArray[0],szTipMsg);

        //ɾ�����
        m_ApplyUserArray.RemoveAt(0);
      }
      else
      {
        bReturn=true;
        break;
      }
    }
  }

  return bReturn;
}

//��ȡ����
void CTableFrameSink::ReadConfigInformation()
{
  //��ȡ�Զ�������
  tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pGameServiceOption->cbCustomRule;
  ASSERT(pCustomConfig);

  //��ׯ
  m_lApplyBankerCondition = 5000;
  m_lBankerMAX = pCustomConfig->CustomGeneral.lBankerTime;
  m_lBankerAdd = pCustomConfig->CustomGeneral.lBankerTimeAdd;
  m_lBankerScoreMAX = pCustomConfig->CustomGeneral.lBankerScoreMAX;
  m_lBankerScoreAdd = pCustomConfig->CustomGeneral.lBankerTimeExtra;
  m_bEnableSysBanker = (pCustomConfig->CustomGeneral.nEnableSysBanker == TRUE)?true:false;

  //ʱ��
  m_cbFreeTime = pCustomConfig->CustomGeneral.cbFreeTime;
  m_cbBetTime = pCustomConfig->CustomGeneral.cbBetTime;
  m_cbEndTime = pCustomConfig->CustomGeneral.cbEndTime;
  if(m_cbFreeTime < 1 || m_cbFreeTime > 99)
  {
    m_cbFreeTime = TIME_FREE;
  }
  if(m_cbBetTime < TIME_PLACE_JETTON || m_cbBetTime > 99)
  {
    m_cbBetTime = TIME_PLACE_JETTON;
  }
  if(m_cbEndTime < TIME_GAME_END || m_cbEndTime > 99)
  {
    m_cbEndTime = TIME_GAME_END;
  }

  //��ע
  m_lAreaLimitScore = pCustomConfig->CustomGeneral.lAreaLimitScore;

  //���
  m_lStorageStart = pCustomConfig->CustomGeneral.StorageStart;
  m_lBonus = pCustomConfig->CustomGeneral.lBonus;
  m_lStorageCurrent = m_lStorageStart;
  m_lCompensateRadio = pCustomConfig->CustomGeneral.lCompensateRadio;
  m_StorageDeduct = pCustomConfig->CustomGeneral.StorageDeduct;
  m_lStorageMax1 = pCustomConfig->CustomGeneral.StorageMax1;
  m_lStorageMul1 = pCustomConfig->CustomGeneral.StorageMul1;
  m_lStorageMax2 = pCustomConfig->CustomGeneral.StorageMax2;
  m_lStorageMul2 = pCustomConfig->CustomGeneral.StorageMul2;
  if(m_lStorageMul1 < 0 || m_lStorageMul1 > 100)
  {
    m_lStorageMul1 = 50;
  }
  if(m_lStorageMul2 < 0 || m_lStorageMul2 > 100)
  {
    m_lStorageMul2 = 80;
  }

  //������
  m_nRobotListMaxCount = 4;

  LONGLONG lRobotBetMinCount = pCustomConfig->CustomAndroid.lRobotBetMinCount;
  LONGLONG lRobotBetMaxCount = pCustomConfig->CustomAndroid.lRobotBetMaxCount;
  m_nMaxChipRobot = rand()%(lRobotBetMaxCount-lRobotBetMinCount+1) + lRobotBetMinCount;
  if(m_nMaxChipRobot < 0)
  {
    m_nMaxChipRobot = 8;
  }
  m_lRobotAreaLimit = pCustomConfig->CustomAndroid.lRobotAreaLimit;


  //������
  m_nMultiple[0] = pCustomConfig->CustomGeneral.Area1Radio;
  m_nMultiple[1] = pCustomConfig->CustomGeneral.Area2Radio;
  m_nMultiple[2] = pCustomConfig->CustomGeneral.Area3Radio;
  m_nMultiple[3] = pCustomConfig->CustomGeneral.Area4Radio;
  m_nMultiple[4] = pCustomConfig->CustomGeneral.Area5Radio;
  m_nMultiple[5] = pCustomConfig->CustomGeneral.Area6Radio;
  m_nMultiple[6] = pCustomConfig->CustomGeneral.Area7Radio;
  m_nMultiple[7] = pCustomConfig->CustomGeneral.Area8Radio;
  m_nMultiple[8] = pCustomConfig->CustomGeneral.Area9Radio;
  m_nMultiple[9] = pCustomConfig->CustomGeneral.Area10Radio;
  m_nMultiple[10] = pCustomConfig->CustomGeneral.Area11Radio;
  m_nMultiple[11] = pCustomConfig->CustomGeneral.Area12Radio;

}

//��ȡ������
BYTE CTableFrameSink::cbGetAreaAnimal(BYTE cbCardData)
{
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

//���ͻ��˷��ͽ�����ʾ����
BYTE CTableFrameSink::cbGetTargetAnimal(BYTE cbCardData)
{
	//1.����, 2.����, 3.��ȸ, 4.��ӥ, 5.����, 6.��è, 7.����, 8.ʨ��, 9.ͨ��, 10.ͨ��, 11.����, 12.����
	if (cbCardData == 0 || cbCardData == 26 || cbCardData == 27)
		return 2;
	else if (cbCardData == 1 || cbCardData == 2 || cbCardData == 3)
		return 1;
	else if (cbCardData == 4)
		return 12;
	else if (cbCardData == 5 || cbCardData == 6 || cbCardData == 7)
		return 5;
	else if (cbCardData == 8 || cbCardData == 9 || cbCardData == 10)
		return 6;
	else if (cbCardData == 11)
		return 9;
	else if (cbCardData == 12 || cbCardData == 13 || cbCardData == 14)
		return 7;
	else if (cbCardData == 15 || cbCardData == 16 || cbCardData == 17)
		return 8;
	else if (cbCardData == 18)
		return 11;
	else if (cbCardData == 19 || cbCardData == 20 || cbCardData == 21)
		return 4;
	else if (cbCardData == 22 || cbCardData == 23 || cbCardData == 24)
		return 3;
	else if (cbCardData == 25)
		return 10;

	return 0;
}