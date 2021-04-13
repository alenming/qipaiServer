#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "DlgCustomRule.h"

//////////////////////////////////////////////////////////////////////////


//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��Ϸ����
protected:
	WORD							m_wBankerUser;							//ׯ���û�
	SCORE							m_lExitScore;							//ǿ�˷���

	LONGLONG						m_lAddChip[4];							//��ע����

	BYTE							m_cbStartTime;							//��ʼʱ��
	BYTE							m_cbCallTime;							//��ׯʱ��
	BYTE							m_cbAddTime;							//��עʱ��
	BYTE							m_cbEndTime;							//����ʱ��

	DWORD							m_dwOperateTime;						//����ʱ��

	//�û�����
protected:
	BYTE							m_cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	BYTE							m_cbCallBankerTimes[GAME_PLAYER];		//��ׯ����

	BYTE							m_cbMaxBankerTimes;						//����ׯ����

	LONGLONG						m_lTableScore[GAME_PLAYER];				//��ע��Ŀ

	BYTE							m_bFleeUser[GAME_PLAYER];				//�������

	//�˿˱���
protected:
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_CARDCOUNT];//�����˿�

	bool							m_bWriteRoomStorage;					//�Ƿ�д���

	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	tagGameServiceOption		    *m_pGameServiceOption;					//���ò���
	tagGameServiceAttrib			*m_pGameServiceAttrib;					//��Ϸ����

	//���Ա���
protected:
	WORD							m_wPlayerCount;							//��Ϸ����
	LONG							m_lMaxCardTimes;						//���������

	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() {}
	//�ӿڲ�ѯ
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��ʼ��
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual void RepositionSink();
	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool OnEventGameStart();
	//��Ϸ����
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);


	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool OnTimerMessage(DWORD dwTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ����
	virtual bool OnGameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool OnFrameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����¼�
	virtual bool OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize){return true;}

	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//��ѯ�����
	virtual bool QueryBuckleServiceCharge(WORD wChairID) { return true; }
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason){return false;}
	//���û���
	virtual void SetGameBaseScore(SCORE lBaseScore){ return; }

	//�û��¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem);
	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; };
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);

	//��Ϸ�¼�
protected:
	//�Զ���ʼ
	bool OnGameAutoStart();
	//��ׯ�¼�
	bool OnUserCallBanker(WORD wChairID, BYTE cbBankerTimes);
	//��ע�¼�
	bool OnUserAddScore(WORD wChairID, LONGLONG lScore);

	//д�ֺ���
	bool TryWriteTableScore(tagScoreInfo ScoreInfoArray[]);

	//������Ϸ��ȡ��Ϸ��¼
	virtual void OnGetGameRecord(VOID *GameRecord) {};

	//��ȡ������Ϸ�Ƿ���ע״̬���������ע�� ����true���ж�������
	virtual bool OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem);

	//���ܺ���
protected:
	//��ȡ����
	void ReadConfigInformation();
};

//////////////////////////////////////////////////////////////////////////

#endif
