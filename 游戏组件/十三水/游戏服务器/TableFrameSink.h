#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DlgCustomRule.h"
#include "..\��Ϣ����\CMD_Thirteenx.h"
#include "GameLogic.h"
#include<algorithm>
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//ģʽ����
protected:
	BYTE								m_cbPlayerCount;									//ָ����Ϸ������2-4

	//��Ϸ����
protected:
	SCORE								m_lCellScore;										//��������
	SCORE								m_lGameScore[GAME_PLAYER];							//��Ӯ�ܻ���
	SCORE								m_lScoreTimes[GAME_PLAYER][3];						//��Ӯע��
	SCORE								m_lAddScoreTimes[GAME_PLAYER][3];					//����ע��
	SCORE								m_lSpecialScoreTimes[GAME_PLAYER];					//�������ͷ�
	SCORE								m_lGunScoreTimes[GAME_PLAYER];						//��ǹ����
	SCORE								m_lHomeRunsScoreTimes[GAME_PLAYER];					//ȫ�ݴ�ע��
	WORD								m_wFireList[GAME_PLAYER][GAME_PLAYER-1];			//��ǹ�б�
	WORD								m_wFireCount[GAME_PLAYER];							//��ǹ����
	BYTE								m_bSpecialType[GAME_PLAYER];						//��������
	BYTE								m_bUserCardData[GAME_PLAYER][HAND_CARD_COUNT];		//����˿�
	BYTE								m_bSegmentCard[GAME_PLAYER][3][5];					//�ֶ��˿�

	BYTE								m_cbPlayerStatus[GAME_PLAYER];						//���״̬

	WORD								m_wShowCardCount;									//̯�Ƽ���
	DWORD								m_dwOperateTime;									//ʱ�����

	bool								m_bFinishSegment[GAME_PLAYER];						//��ɷֶ�

	bool								m_bAutoUser[GAME_PLAYER];							//�йܳ�����ң���Ϸ����Ҫ�߳�ȥ

	bool								m_bWriteRoomStorage;					//�Ƿ�д���

//�������
protected:
	CGameLogic							m_GameLogic;										//��Ϸ�߼�
	ITableFrame							*m_pITableFrame;									//��ܽӿ�
	tagGameServiceOption				*m_pGameServiceOption;								//���ò���
	tagCustomRule*						m_pGameCustomRule;									//�Զ�����

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

	//////////////////////////////////////////////////////////////////////////
	//�û��¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);
	//���û���
	virtual void SetGameBaseScore(SCORE lBaseScore){};

	//��Ϣ����
protected:
	//��ʼ����
	bool OnGameAutoStart();
	//���̯��
	bool OnUserShowCard(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);

	//ͨ�ú���
public:
	//��ȡ����
	void CompareCardGetTimes();
	//��ʼ����
	VOID StartCompareCard();
};

//////////////////////////////////////////////////////////////////////////

#endif
