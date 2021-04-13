#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//��ʷ��¼
#define MAX_SCORE_HISTORY			20									//��ʷ����
//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//����ע��
protected:
	SCORE							m_lAllJettonScore[AREA_MAX];	//ȫ����ע
	
	//������ע
protected:
	SCORE							m_lUserJettonScore[GAME_PLAYER][AREA_MAX];			//������ע

	bool							m_bUserListWin[GAME_PLAYER][USER_LIST_COUNT];		//�б���һ�ʤ����
	SCORE							m_lUserListScore[GAME_PLAYER][USER_LIST_COUNT];		//�б������ע��Ŀ
	BYTE							m_cbUserPlayCount[GAME_PLAYER];						//�б������Ϸ����

	//���Ʊ���
protected:
	SCORE							m_lAreaLimitScore;						//��������
	SCORE							m_lUserLimitScore;						//��������
	SCORE							m_lApplyBankerCondition;				//��������

	TCHAR							m_szConfigFileName[MAX_PATH];			//�����ļ�

	//�����˿���
	SCORE							m_lRobotAreaLimit;						//����ͳ�� (������)
	SCORE							m_lRobotAreaScore[AREA_MAX];			//����ͳ�� (������)
	int								m_nBankerTimeLimit;						//��������

	//ʱ�����
	int								m_nFreeTime;							//����ʱ��
	int								m_nPlaceJettonTime;						//��עʱ��
	int								m_nGameEndTime;							//����ʱ��

	//��ҳɼ�
protected:
	SCORE							m_lUserWinScore[GAME_PLAYER];			//��ҳɼ�
	SCORE							m_lUserReturnScore[GAME_PLAYER];		//������ע
	SCORE							m_lUserRevenue[GAME_PLAYER];			//���˰��
	
	//�˿���Ϣ
protected:
	BYTE							m_cbCardCount[4];						//�˿���Ŀ
    BYTE							m_cbTableCardArray[4][2];				//�����˿�
	BYTE							m_cbTableHavaSendCardArray[4][10];		//�����˿�
	BYTE							m_cbTableHavaSendCount[4];				//�����˿�
	BYTE							m_cbTableCard[CARD_COUNT];				//�����˿�
	BYTE							m_cbLeftCardCount;						//ʣ���˿���Ŀ

	//״̬����
protected:
	DWORD							m_dwJettonTime;							//��ʼʱ��

	//�ֿ�
	BYTE							m_cbJuControlTimes;						//�ֿش���
	BYTE							m_cbJuControl;							//�Ƿ���оֿ�
	BYTE							m_cbJuControlArea[AREA_MAX];			//���ֿ�������ʤ�� 0�����ƣ�1����Ӯ��2������

	//ׯ����Ϣ
protected:
	CWHArray<WORD>					m_ApplyUserArray;						//�������
	WORD							m_wCurrentBanker;						//��ǰׯ��
	WORD							m_wBankerTime;							//��ׯ����
	SCORE							m_lBankerScore;							//ׯ�ҷ���
	SCORE							m_lBankerWinScore;						//�ۼƳɼ�
	SCORE							m_lBankerCurGameScore;					//��ǰ�ɼ�
	SCORE							m_lSysBankerScore;						//ϵͳׯ��Ǯ��

	//��¼����
protected:
	tagServerGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];	//��Ϸ��¼
	int								m_nRecordFirst;							//��ʼ��¼
	int								m_nRecordLast;							//����¼

	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	tagGameServiceOption		    *m_pGameServiceOption;					//���ò���
	tagGameServiceAttrib			*m_pGameServiceAttrib;					//��Ϸ����
	//���Ա���
protected:
	static const WORD				m_wPlayerCount;							//��Ϸ����

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
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��ʼ��
	virtual bool  Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual void  RepositionSink();


	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool  OnEventGameStart();

	//��Ϸ����
	virtual bool  OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool  OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//������Ϸ��ȡ��Ϸ��¼
	virtual void OnGetGameRecord(VOID *GameRecord);

	//��ȡ������Ϸ�Ƿ���ע״̬���������ע��
	virtual bool OnGetUserBetInfo(WORD wChairID, IServerUserItem * pIServerUserItem);

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool  OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ����
	virtual bool  OnGameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool  OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//�����¼�
public:
	//�û�����
	virtual bool  OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) ;
	//�û�����
	virtual bool  OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem){ return true; }
	//�û�����
	virtual bool  OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool  OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool  OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize){ return true; }


	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);//{return 0;}
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//��ѯ�����
	virtual bool QueryBuckleServiceCharge(WORD wChairID);
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason){return false;}
	//���û���
	virtual void SetGameBaseScore(SCORE lBaseScore){ return; }

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
	//��ȡǰ6���û��б��GameID
	bool OnGetUserListGameID(WORD wSeatUser[MAX_SEAT_COUNT]);

	//��������
private:
	//����ׯ��
	void SendApplyUser( IServerUserItem *pServerUserItem );
	//����ׯ��
	bool ChangeBanker(bool bCancelCurrentBanker);
	//���ͼ�¼
	void SendGameRecord(IServerUserItem *pIServerUserItem);

	//��ע����
private:
	//�����ע
	SCORE GetUserMaxJetton(WORD wChairID);
	
	
	void ReadConfigInformation(bool bReadFresh,bool bReadBaseConfig=true);

	//ѡ��ׯ��
	void TrySelectBanker();

	//��Ϸͳ��
private:
	//����÷�
	SCORE CalculateScore();
	//�ƶ�Ӯ��
	void DeduceWinner(bool &bWinShangMen, bool &bWinTianMen, bool &bWinXiaMen);

	//����ϵͳ��Ӯ��
	SCORE CalculateSystemScore();
	//�Ƿ����
	BYTE AnalyseControl();

	void GetValueFromCombStr(LONGLONG llData[], int nDataLen, LPCTSTR sTr, int nStrLen);
};

//////////////////////////////////////////////////////////////////////////

#endif
