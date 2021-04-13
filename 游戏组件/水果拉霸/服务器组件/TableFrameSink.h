#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"
#include "GameLogic.h"
#include "DlgCustomRule.h"


/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//��Ϸ����
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	int								m_lBet;								//Ѻ����ע
	SCORE							m_lBetScore;						//Ѻ����ע
	SCORE							m_lOneGameScore;					//Ѻ�ߵ÷�
	SCORE							m_lThreeGameScore;					//�����÷�
	SCORE							m_lJackPotScore;					//���ػ���
	BYTE							m_cbItemInfo[ITEM_Y_COUNT][ITEM_X_COUNT];	//������Ϣ

	int								m_lFreeBet;							//��¼��Ѵ�����ȡʱ����ע
	BYTE							m_cbAllFreeTimes;					//�ܵ���Ѵ���
	BYTE							m_cbUsedFreeTimes;					//ʹ�õ���Ѵ���

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

	//��ȡ������Ϸ�Ƿ���ע״̬���������ע�� ����true���ж�������
	virtual bool OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem) { return false; };

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
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);

	//��Ϸ�¼�
protected:
	//Ѻ�߿�ʼ
	bool OnUserBetStart(IServerUserItem * pIServerUserItem, int lBet);
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

#endif