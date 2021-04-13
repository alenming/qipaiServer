#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"
#include "GameLogic.h"

#define MAX_CARD					5

//////////////////////////////////////////////////////////////////////////
//��ʷ��¼
#define MAX_SCORE_HISTORY			40									//��ʷ����
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//��Ϸ����
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//�û���Ϣ
protected:
	SCORE							m_lUserStartScore[GAME_PLAYER];		//��ʼ����

	bool							m_bUserListWin[GAME_PLAYER][USER_LIST_COUNT];		//�б���һ�ʤ����
	SCORE							m_lUserListScore[GAME_PLAYER][USER_LIST_COUNT];		//�б������ע��Ŀ
	BYTE							m_cbUserPlayCount[GAME_PLAYER];						//�б������Ϸ����

	//����ע��
protected:
	SCORE							m_lAllJettonScore[AREA_MAX + 1];		//ȫ����ע
	
	//������ע
protected:
	SCORE							m_lUserJettonScore[GAME_PLAYER][AREA_MAX + 1];	//������ע

	int								m_nChip[MAX_CHIP_COUNT];				//��������
	int								m_nMaxTimes;							//�����

	//���Ʊ���
protected:
	bool							m_bRefreshCfg;							//ÿ��ˢ��
	TCHAR							m_szRoomName[32];						//���÷���
	TCHAR							m_szConfigFileName[MAX_PATH];			//�����ļ�
	SCORE							m_lAreaLimitScore;						//��������
	SCORE							m_lUserLimitScore;						//��������
	INT								m_nEndGameMul;							//��ǰ���ưٷֱ�

	//��ׯ����
	SCORE							m_lApplyBankerCondition;				//��������
	int								m_nBankerTimeLimit;						//��������
	LONGLONG						m_lPlayerBankerMAX;						//������ׯ����
	int								m_nBankerTimeAdd;						//���Ӵ��� (��Ҵ����������ʱ)
	SCORE							m_lExtraBankerScore;					//�������� (���ڴ�ֵʱ������������������ׯ)
	int								m_nExtraBankerTime;						//�������

	//ʱ�����
	int								m_nFreeTime;							//����ʱ��
	int								m_nPlaceJettonTime;						//��עʱ��
	int								m_nGameEndTime;							//����ʱ��

	//�����˿���
	int								m_nMaxChipRobot;						//�����Ŀ (��ע������)
	int								m_nChipRobotCount;						//����ͳ�� (��ע������)
	SCORE							m_lRobotAreaLimit;						//����ͳ�� (������)
	SCORE							m_lRobotAreaScore[AREA_MAX + 1];		//����ͳ�� (������)
	int								m_nRobotListMaxCount;					//�������

	//��ҳɼ�
protected:
	SCORE							m_lUserWinScore[GAME_PLAYER];			//��ҳɼ�
	SCORE							m_lUserReturnScore[GAME_PLAYER];		//������ע
	SCORE							m_lUserRevenue[GAME_PLAYER];			//���˰��
	BYTE							m_cbLeftCardCount;						//�˿���Ŀ
	bool							m_bContiueCard;							//��������
	BYTE							m_bcFirstPostCard;						//���°l���c��

	//�˿���Ϣ
protected:
	BYTE							m_cbCardCount[4];						//�˿���Ŀ
    BYTE							m_cbTableCardArray[5][5];				//�����˿�
	BYTE							m_cbTableCard[CARD_COUNT];				//�����˿�

	BYTE							m_cbOpenResult[AREA_MAX];				//�������

	BYTE							m_cbEarlyOpen;							//��ǰ������ʶ

	//״̬����
protected:
	DWORD							m_dwJettonTime;							//��ʼʱ��
	bool							m_bExchangeBanker;						//�任ׯ��
	WORD							m_wAddTime;								//������ׯ 

	//ׯ����Ϣ
protected:
	CWHArray<WORD>					m_ApplyUserArray;						//�������
	WORD							m_wCurrentBanker;						//��ǰׯ��
	WORD							m_wOfflineBanker;						//����ׯ��
	WORD							m_wBankerTime;							//��ׯ����
	SCORE							m_lBankerScore;							//
	SCORE							m_lBankerWinScore;						//�ۼƳɼ�
	SCORE							m_lBankerCurGameScore;					//��ǰ�ɼ�
	bool							m_bEnableSysBanker;						//ϵͳ��ׯ



	//���Ʊ���
protected:
	BYTE							m_cbWinSideControl;						//������Ӯ
	int								m_nSendCardCount;						//���ʹ���

	//�ֿ�
	BYTE							m_cbJuControlTimes;						//�ֿش���
	BYTE							m_cbJuControl;							//�Ƿ���оֿ�
	BYTE							m_cbJuControlArea[AREA_MAX + 1];		//���ֿ�������ʤ�� 0�����ƣ�1����Ӯ��2������


	//��¼����
protected:
	tagServerGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];	//��Ϸ��¼
	int								m_nRecordFirst;							//��ʼ��¼
	int								m_nRecordLast;							//����¼
	CFile							m_FileRecord;							//��¼���
	DWORD							m_dwRecordCount;						//��¼��Ŀ

	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	tagGameServiceOption			* m_pGameServiceOption;					//���ò���
	tagGameServiceAttrib *			m_pGameServiceAttrib;					//��Ϸ����

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
	virtual VOID Release();
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��λ�ӿ�
	virtual VOID RepositionSink();
	//���ýӿ�
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//�����ӿ�
public:
	//���û���
	virtual void SetGameBaseScore(SCORE lBaseScore){};
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
	//ʱ���¼�
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ
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
	virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }
	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//��ѯ�Ƿ�۷����
	virtual bool QueryBuckleServiceCharge(WORD wChairID);	

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
	//�����˿�
	bool DispatchTableCard();
	//����ׯ��
	void SendApplyUser( IServerUserItem *pServerUserItem );
	//����ׯ��
	bool ChangeBanker(bool bCancelCurrentBanker);
	//�ֻ��ж�
	void TakeTurns();
	//���ͼ�¼
	void SendGameRecord(IServerUserItem *pIServerUserItem);
	//��ȡ����
	void ReadConfigInformation();

	//��Ӯ����
protected:
	//����ϵͳ����������ׯʱ������ҵ÷�
	SCORE CalSysOrAndroidBankerScore();
	//�������������ׯʱ���˵ĵ÷�
	SCORE CalRealPlayerScore();

	//��ע����
private:
	//�����ע
	SCORE GetUserMaxJetton(WORD wChairID, BYTE cbJettonArea);

	//��Ϸͳ��
private:
	//����÷�
	SCORE CalculateScore();
	//�ƶ�Ӯ��
	void DeduceWinner(bool &bWinTian, bool &bWinDi, bool &bWinXuan,bool &bWinHuan,BYTE &TianMultiple,BYTE &diMultiple,BYTE &TianXuanltiple,BYTE &HuangMultiple );

};


/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

#endif