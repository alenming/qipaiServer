#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "DlgCustomRule.h"

class ICGame;

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��Ϸ����
protected:
	WORD			    m_wPlayerNum;				//��Ϸ�������
	WORD				m_wBankerUser;				//ׯ���û�
	WORD			    m_RealPlayers;			    //��ǰʵ�����
	WORD				m_wCurrentUser;				//��ǰ�û�

	//�û�����
public:
	BYTE				m_cbHandCardData[GAME_PLAYER][MAX_COUNT];	//�û��˿�
	BYTE				m_cbPlayStatus[GAME_PLAYER];				//��ǰ��Ч���
protected:
	BYTE				m_cbCallStatus[GAME_PLAYER];				//��ׯ״̬
	SCORE				m_lTableScore[GAME_PLAYER];					//��ע��Ŀ
	BYTE				m_cbPlayerVlue[GAME_PLAYER];				//��ҵ���
	SCORE				m_lCurrentScore[GAME_PLAYER];				//��ǰ�ֵ÷�

	BYTE				m_bFleeUser[GAME_PLAYER];				//�������

	//״̬����
protected:
	DWORD				m_dwPassedTime;								//ʱ��

	bool				m_bWriteRoomStorage;					//�Ƿ�д���

	//�������
protected:
	CGameLogic				m_GameLogic;              //��Ϸ�߼�
	ITableFrame *			m_pITableFrame;           //��ܽӿ�
	tagGameServiceOption *	m_pGameServiceOption;     //���ò���
	tagGameServiceAttrib *	m_pGameServiceAttrib;     //��Ϸ����

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

	//������Ϸ��ȡ��Ϸ��¼
	virtual void OnGetGameRecord(VOID *GameRecord) {};

	//��ȡ������Ϸ�Ƿ���ע״̬���������ע�� ����true���ж�������
	virtual bool OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem);

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool OnTimerMessage(DWORD dwTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ����
	virtual bool OnGameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool OnFrameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����¼�
	virtual bool OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize) { return true; }

	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){ return 0; }
	//��ѯ�����
	virtual bool QueryBuckleServiceCharge(WORD wChairID);

	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
	{
	return false;
	}
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason) { return false; }
	//���û���
	virtual void SetGameBaseScore(SCORE lBaseScore)  { return; }
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
	//��ׯ�¼�
	bool OnUserCallBanker(WORD wChairID, BYTE bBanker);
	//��ע�¼�
	bool OnUserAddScore(WORD wChairID, SCORE lScore);
};

//////////////////////////////////////////////////////////////////////////

#endif
