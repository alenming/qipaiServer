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
	SCORE							m_lAllJettonScore[AREA_COUNT];						//ȫ����ע

//������ע
protected:
	SCORE							m_lUserJettonScore[GAME_PLAYER][AREA_COUNT];		//������ע

	SCORE							m_lUserTypeScore[GAME_PLAYER][7];					//������ע

//�Ͼ���Ϣ
public:
	bool							m_bUserListWin[GAME_PLAYER][USER_LIST_COUNT];		//�б���һ�ʤ����
	SCORE							m_lUserListScore[GAME_PLAYER][USER_LIST_COUNT];		//�б������ע��Ŀ
	BYTE							m_cbUserPlayCount[GAME_PLAYER];						//�б������Ϸ����
	//���Ʊ���
protected:
	SCORE							m_lAreaLimitScore;						//��������
	SCORE							m_lUserLimitScore;						//��������
	SCORE							m_lTypeLimitScore[7];					//��������

	//��ҳɼ�
protected:
	SCORE							m_lUserWinScore[GAME_PLAYER];			//��ҳɼ�
	SCORE							m_lUserReturnScore[GAME_PLAYER];		//������ע
	SCORE							m_lUserRevenue[GAME_PLAYER];			//���˰��

	SCORE							m_lPlayScore[GAME_PLAYER][AREA_COUNT];	//�����Ӯ

	SCORE							m_lPreviousAreaScore[GAME_PLAYER][AREA_COUNT];	//�����һ����ע

	//���Ʋ���
protected:
	BYTE							m_cbJuControlTimes;						//�ֿش���
	BYTE							m_cbJuControl;							//�Ƿ���оֿ�
	BYTE							m_cbJuControlArea;						//���ֿ�������ʤ�� 0�����ƣ�1����Ӯ��2������

	//�˿���Ϣ
protected:
	
    BYTE							m_cbTableCard;							//�����˿�	
	SCORE							m_nChips[MAX_CHIP_COUNT];				//����

	//״̬����
protected:
	DWORD							m_dwJettonTime;							//��ʼʱ��

	//��¼����
protected:
	tagServerGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];	//��Ϸ��¼
	int								m_nRecordFirst;							//��ʼ��¼
	int								m_nRecordLast;							//����¼
	
	//���Ʊ���
protected:
	TCHAR							m_szConfigFileName[MAX_PATH];			//�����ļ�

	//ʱ������
protected:						
	BYTE							m_cbBetTime;							//��עʱ��
	BYTE							m_cbEndTime;							//����ʱ��

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

	BYTE							m_nAreaNums[AREA_COUNT][18];
	
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
	bool OnUserPlaceJetton(WORD wChairID, BYTE cbJettonType, BYTE cbJettonArea[], SCORE lJettonScore, int nKeys);
	//�����û��б�
	bool OnUserRequestUserList(WORD wChairID);
	//��ȡǰ6���û��б��GameID
	bool OnGetUserListGameID(WORD wSeatUser[MAX_SEAT_COUNT]);

	//��������
private:
	//�����˿�
	void DispatchTableCard();
	//���ͼ�¼
	void SendGameRecord(IServerUserItem *pIServerUserItem);
	//��ע����
private:
	void   GetAllWinArea(BYTE bcWinArea[],BYTE bcAreaCount,BYTE InArea);
	//�����ע
	SCORE GetUserMaxJetton(WORD wChairID, BYTE Area[], BYTE cbType);
	
	//��Ϸͳ��
private:
	//����÷�
	SCORE CalculateScore();
	//��ȡ����
	void ReadConfigInformation();

	//��ȡʤ������
	BYTE cbGetWinArea(BYTE cbTableCard,BYTE cbWinArea[]);
	//��ȡ������
	BYTE cbGetAreaTimes(BYTE cbArea);

	//��ʼ����������
	void InitAreaNums();
};

//////////////////////////////////////////////////////////////////////////

#endif
