#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//����ע��
protected:
	SCORE            m_lAllJettonScore[AREA_COUNT];    //ȫ����ע

  //������ע
protected:
	SCORE            m_lUserJettonScore[GAME_PLAYER][AREA_COUNT];  //������ע

  //�Ͼ���Ϣ
public:
  bool				m_bUserListWin[GAME_PLAYER][USER_LIST_COUNT];		//�б���һ�ʤ����
  SCORE				m_lUserListScore[GAME_PLAYER][USER_LIST_COUNT];		//�б������ע��Ŀ
  BYTE				m_cbUserPlayCount[GAME_PLAYER];						//�б������Ϸ����
	//���Ʊ���
protected:
	SCORE							m_lAreaLimitScore;						//��������
	SCORE							m_lUserLimitScore;						//��������
	SCORE							m_lApplyBankerCondition;				//��������

	//��ҳɼ�
protected:
	SCORE							m_lUserWinScore[GAME_PLAYER];			//��ҳɼ�
	SCORE							m_lUserReturnScore[GAME_PLAYER];		//������ע
	SCORE							m_lUserRevenue[GAME_PLAYER];			//���˰��
		

	//���Ʋ���
protected:
	BYTE							m_cbJuControlTimes;						//�ֿش���
	BYTE							m_cbJuControl;							//�Ƿ���оֿ�
	BYTE							m_cbJuControlArea;						//���ֿ�������ʤ�� 0�����ƣ�1����Ӯ��2������

	//�˿���Ϣ
protected:
	
    BYTE							m_cbTableCard;							//�����˿�	
	INT								m_nMultiple[AREA_ALL];					//������
	INT								m_nAnimalTimes[AREA_ALL];				//���ִ���
	SCORE							m_nChips[5];							//����

	//״̬����
protected:
	DWORD							m_dwJettonTime;							//��ʼʱ��

	//ׯ����Ϣ
protected:
	CWHArray<WORD>					m_ApplyUserArray;						//�������
	WORD							m_wCurrentBanker;						//��ǰׯ��
	WORD							m_wBankerTime;							//��ׯ����
	SCORE							m_lBankerWinScore;						//�ۼƳɼ�
	SCORE							m_lBankerCurGameScore;					//��ǰ�ɼ�
	bool							m_bEnableSysBanker;						//ϵͳ��ׯ

	//��¼����
protected:
	tagServerGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];	//��Ϸ��¼
	int								m_nRecordFirst;							//��ʼ��¼
	int								m_nRecordLast;							//����¼
	
	
	//���Ʊ���
protected:
	LONGLONG						m_lStorageCurrent;							//�������
		LONGLONG                    m_StorageStart;
	LONGLONG						m_StorageDeduct;						//�۳�����
	TCHAR							m_szConfigFileName[MAX_PATH];			//�����ļ�
	TCHAR							m_szGameRoomName[SERVER_LEN];			//��������

	LONGLONG						m_lStorageMax1;							//���ⶥ1
	LONGLONG						m_lStorageMul1;							//ϵͳ��Ǯ����
	LONGLONG						m_lStorageMax2;							//���ⶥ2
	LONGLONG						m_lStorageMul2;							//ϵͳ��Ǯ����
	int								m_nJettonMultiple;						//����һ�����
	//�����˿���
protected:
	int								m_nMaxChipRobot;						//�����Ŀ 
	int								m_nChipRobotCount;						//����ͳ�� 
	SCORE							m_lRobotAreaLimit;						//��������
	LONGLONG						m_lRobotBetCount;						//��ע����
	SCORE							m_lRobotAreaScore[AREA_COUNT + 1];		//������ע
    int								m_nRobotListMaxCount;					//�������
	//ׯ������
protected:
	
	LONGLONG						m_lBankerMAX;							//���ׯ��
	LONGLONG						m_lBankerAdd;							//ׯ������

	LONGLONG						m_lBankerScoreMAX;						//ׯ��Ǯ
	LONGLONG						m_lBankerScoreAdd;						//��ׯ����
	//���ׯ����
	LONGLONG						m_lPlayerBankerMAX;						//���ׯ��

	//��ׯ
	bool							m_bExchangeBanker;						//����ׯ��
	SCORE							m_lUserStartScore[GAME_PLAYER];		//��ʼ����
	//ʱ������
protected:						
	BYTE							m_cbFreeTime;							//����ʱ��
	BYTE							m_cbBetTime;							//��עʱ��
	BYTE							m_cbEndTime;							//����ʱ��
	BYTE                            m_cbAddTime;							//����ʱ��

	//�������
protected:

	CGameLogic						m_GameLogic;							//��Ϸ�߼�

	//����ӿ�
protected:
	ITableFrame	*					m_pITableFrame;							//��ܽӿ�
	tagGameServiceOption *			m_pGameServiceOption;					//��������
	tagGameServiceAttrib *			m_pGameServiceAttrib;					//��Ϸ����

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;							//��Ϸ����
	//static ConsoleWindow			m_DebugWindow;							//���Դ���
	
	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID  Release() {delete this;}

	//�ӿڲ�ѯ
	virtual void *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��ʼ��
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual VOID RepositionSink();

	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
	{
		return false;
	}

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool OnEventGameStart();
	//��Ϸ����
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//������Ϸ��ȡ��Ϸ��¼
	virtual void OnGetGameRecord(VOID *GameRecord);

	//��ȡ������Ϸ�Ƿ���ע״̬���������ע��
	virtual bool OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem);

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ����
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����¼�
	virtual bool OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize);
	//�۳������
	virtual bool QueryBuckleServiceCharge(WORD wChairID);
	//�����ӿ�
public:
	//���û���
	virtual void SetGameBaseScore(SCORE lBaseScore){};

	//�����¼�
public:
	//�û�����
	virtual bool  OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) ;

	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }

	//�û�����
	virtual bool  OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool  OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool  OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize){ return true; }



	//��Ϸ�¼�
protected:
	//��ע�¼�
	bool OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, SCORE lJettonScore);
	//����ׯ��
	bool OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem);
	//ȡ������
	bool OnUserCancelBanker(IServerUserItem *pICancelServerUserItem);
  //�����û��б�
  bool OnUserRequestUserList(WORD wChairID);

	//��������
private:
	bool FindSuitBanker();
	//�����˿�
  void DispatchTableCard();
	//����ׯ��
	void SendApplyUser( IServerUserItem *pServerUserItem );
	//����ׯ��
	bool ChangeBanker(bool bCancelCurrentBanker);
	//���ͼ�¼
	void SendGameRecord(IServerUserItem *pIServerUserItem);
	//������Ϣ
	void SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg);
	//��ע����
private:
	void   GetAllWinArea(BYTE bcWinArea[],BYTE bcAreaCount,BYTE InArea);
	//�����ע
	SCORE GetUserMaxJetton(WORD wChairID, BYTE Area);
	
	//��Ϸͳ��
private:
	//����÷�
	SCORE CalculateScore();
	//��ȡ����
	void ReadConfigInformation();
  //��ȡ������
  BYTE cbGetAreaAnimal(BYTE cbCardData);
};

//////////////////////////////////////////////////////////////////////////

#endif
