#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"
#include "GameLogic.h"
#include "HistoryScore.h"
#include "DlgCustomRule.h"
#include "ServerControl.h"

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//��Ϸ����
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��Ϸ����
protected:
	WORD							m_wFirstUser;						//�׽��û�
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ���
	BYTE							m_cbOutCardCount[GAME_PLAYER];		//���ƴ���
	BYTE							m_cbWaitTime;						//�ȴ�ʱ��
	WORD							m_wTimerControl;					//ʱ�����	
	BYTE							m_cbUserTrustee[GAME_PLAYER];		//�йܱ�־

	BYTE							m_cbLeftCardNum[15];				//ʣ������
	BYTE							m_cbPlayerMultiple[GAME_PLAYER];	//�ӱ�����

	//�й���Ϣ
	bool							m_bOffLineTrustee;					//�����й�

	DWORD							m_dwOperateTime;					//����ʱ��

	//ը����Ϣ
protected:
	BYTE							m_cbBombCount;						//ը������
	BYTE							m_cbEachBombCount[GAME_PLAYER];		//ը������

	BYTE							m_cbHaveOutMissile;					//�Ƿ�������

	//�з���Ϣ
protected:
	BYTE							m_cbCallScoreCount;					//�зִ���
	BYTE							m_cbBankerScore;					//ׯ�ҽз�
	BYTE							m_cbScoreInfo[GAME_PLAYER];			//�з���Ϣ

	//������Ϣ
protected:
	WORD							m_wTurnWiner;						//ʤ�����
	BYTE							m_cbTurnCardCount;					//������Ŀ
	BYTE							m_cbTurnCardData[MAX_COUNT];		//��������

	//�˿���Ϣ
protected:
	BYTE							m_cbBankerCard[3];					//��Ϸ����
	BYTE							m_cbHandCardCount[GAME_PLAYER];		//�˿���Ŀ
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];	//�����˿�

	bool							m_bWriteRoomStorage;					//�Ƿ�д���

	//�������
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�


	//����ӿ�
protected:
	ITableFrame	*					m_pITableFrame;						//��ܽӿ�
	tagCustomRule *					m_pGameCustomRule;					//�Զ�����
	tagGameServiceOption *			m_pGameServiceOption;				//��Ϸ����
	tagGameServiceAttrib *			m_pGameServiceAttrib;				//��Ϸ����

	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��λ����
	virtual VOID RepositionSink();
	//��������
	virtual bool Initialization(IUnknownEx * pIUnknownEx);

	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem);
	//��ѯ�Ƿ�۷����
	virtual bool QueryBuckleServiceCharge(WORD wChairID){return true;}

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool OnEventGameStart();
	//��Ϸ����
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//������Ϸ��ȡ��Ϸ��¼
	virtual void OnGetGameRecord(VOID *GameRecord) {};

	//��ȡ������Ϸ�Ƿ���ע״̬���������ע��
	virtual bool OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem);

	//�¼��ӿ�
public:
	//ʱ���¼�
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize);
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);

	//����ӿ�
public:
	//��Ϸ��Ϣ
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//�����ӿ�
public:
	//���û���
	virtual void SetGameBaseScore(SCORE lBaseScore);

	//�û��¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem);
	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem);
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }


	//��Ϸ�¼�
protected:
	//�û�����
	bool OnUserPassCard(WORD wChairID);
	//�û��з�
	bool OnUserCallScore(WORD wChairID, BYTE cbCallScore);
	//�û��ӱ�
	bool OnUserDouble(WORD wChairID, BYTE cbDoubleTimes);
	//�û�����
	bool OnUserOutCard(WORD wChairID, BYTE cbCardData[], BYTE cbCardCount);
	//�����й�
	bool IsOfflineTrustee();
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

#endif