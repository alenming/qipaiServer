#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink :public ITableFrameSink, public ITableUserAction
{
	//��Ϸ����
protected:
	BYTE							m_bOperaCount;							//��������
	WORD							m_wCurrentUser;							//��ǰ�û�
	WORD							m_wBankerUser;							//ׯ���û�

	BYTE							m_cbGameTurns;							//��Ϸ����

	bool							m_cbShowHandStatus;						//���״̬
	//�û�״̬
protected:
	BYTE							m_cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	BYTE							m_cbGiveUpUser[GAME_PLAYER];			//��Ϸ״̬
	CMD_S_GameEnd					m_StGameEnd;							//��������
	BYTE							m_cbRealPlayer[GAME_PLAYER];			//�������
	BYTE							m_cbAndroidStatus[GAME_PLAYER];			//����״̬

	bool							m_bFleeUser[GAME_PLAYER];

	//�˿˱���
protected:
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//�����˿�

	WORD							m_wCompardUser[GAME_PLAYER][GAME_PLAYER-1];//�ȹ��Ƶ����
	BYTE							m_cbCompareCount[GAME_PLAYER];			//�ȹ��ƵĴ���

	DWORD							m_dwJettonTime;							//��ʼʱ��

	//��ע��Ϣ
protected:
	SCORE							m_lTableScore[GAME_PLAYER];				//��ע��Ŀ
	SCORE							m_lUserMaxScore[GAME_PLAYER];			//�����ע
	SCORE							m_lMaxCellScore;						//��Ԫ����
	SCORE							m_lCellScore;							//��Ԫ��ע
	LONGLONG						m_lCurrentTimes;						//��ǰ����

	SCORE							m_lShowHandScore[GAME_PLAYER];			//ȫѹ���

	bool							m_bMingZhu[GAME_PLAYER];				//������ע
	bool							m_bFollowStatus[GAME_PLAYER];			//������״̬

	bool							m_bWriteRoomStorage;					//�Ƿ�д���

	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame*					m_pITableFrame;							//��ܽӿ�
	tagGameServiceOption*			m_pGameServiceOption;					//���ò���

	tagCustomAndroid				m_pCustomAndroid;						//����������

	//AI����
protected:
	bool							m_bUpdataStorage;						//���¿��


	//���Ա���
protected:
	WORD							m_wPlayerCount;							//��Ϸ����
	static const BYTE				m_GameStartMode;						//��ʼģʽ


	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID  Release();
	//�Ƿ���Ч
	virtual bool  IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//�ӿڲ�ѯ
	virtual void *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��ʼ��
	virtual bool  Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual void  RepositionSink();
	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem){return 0;}
		//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//��ѯ�Ƿ�۷����
	virtual bool QueryBuckleServiceCharge(WORD wChairID){return true;}

	//�����ӿ�
public:
	//���û���
	virtual void SetGameBaseScore(SCORE lBaseScore){};

	//��Ϣ�ӿ�
public:
	//��ʼģʽ
	virtual BYTE GetGameStartMode();

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool OnEventGameStart();
	//��Ϸ����
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);

	//������Ϸ��ȡ��Ϸ��¼
	virtual void OnGetGameRecord(VOID *GameRecord) {};

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


	//�û��¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem);
	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);

	//��Ϸ�¼�
protected:
	//��Ϸ��ʼ
	bool OnGameAutoStart();
	//�����¼�
	bool OnUserGiveUp(WORD wChairID,bool bExit=false);
	//�����¼�
	bool OnUserLookCard(WORD wChairID);
	//�����¼�
	bool OnUserCompareCard(WORD wFirstChairID,WORD wNextChairID,bool bNeedScore = true);
	//�����¼�
	bool OnUserOpenCard(WORD wUserID);
	//��ע�¼�
	bool OnUserAddScore(WORD wChairID, SCORE lScore);
	//���
	bool OnUserShowHand(WORD wUserID);
	//���ܺ���
protected:
	//��ȡ����
	void ReadConfigInformation();
};

//////////////////////////////////////////////////////////////////////////

#endif
