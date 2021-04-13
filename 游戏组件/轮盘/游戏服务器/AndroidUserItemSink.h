#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

enum AndroidBetAreas
{
	ASTRAIGHT_0,
	ASTRAIGHT_1,
	ASTRAIGHT_2,
	ASTRAIGHT_3,
	ASTRAIGHT_4,
	ASTRAIGHT_5,
	ASTRAIGHT_6,
	ASTRAIGHT_7,
	ASTRAIGHT_8,
	ASTRAIGHT_9,
	ASTRAIGHT_10,
	ASTRAIGHT_11,
	ASTRAIGHT_12,
	ASTRAIGHT_13,
	ASTRAIGHT_14,
	ASTRAIGHT_15,
	ASTRAIGHT_16,
	ASTRAIGHT_17,
	ASTRAIGHT_18,
	ASTRAIGHT_19,
	ASTRAIGHT_20,
	ASTRAIGHT_21,
	ASTRAIGHT_22,
	ASTRAIGHT_23,
	ASTRAIGHT_24,
	ASTRAIGHT_25,
	ASTRAIGHT_26,
	ASTRAIGHT_27,
	ASTRAIGHT_28,
	ASTRAIGHT_29,
	ASTRAIGHT_30,
	ASTRAIGHT_31,
	ASTRAIGHT_32,
	ASTRAIGHT_33,
	ASTRAIGHT_34,
	ASTRAIGHT_35,
	ASTRAIGHT_36,
	ASPLIT_1,
	ASPLIT_2,
	ASPLIT_3,
	ASPLIT_4,
	ASPLIT_5,
	ASPLIT_6,
	ASPLIT_7,
	ASPLIT_8,
	ASPLIT_9,
	ASPLIT_10,
	ASPLIT_11,
	ASPLIT_12,
	ASPLIT_13,
	ASPLIT_14,
	ASPLIT_15,
	ASPLIT_16,
	ASPLIT_17,
	ASPLIT_18,
	ASPLIT_19,
	ASPLIT_20,
	ASPLIT_21,
	ASPLIT_22,
	ASPLIT_23,
	ASPLIT_24,
	ASPLIT_25,
	ASPLIT_26,
	ASPLIT_27,
	ASPLIT_28,
	ASPLIT_29,
	ASPLIT_30,
	ASPLIT_31,
	ASPLIT_32,
	ASPLIT_33,
	ASPLIT_34,
	ASPLIT_35,
	ASPLIT_36,
	ASPLIT_37,
	ASPLIT_38,
	ASPLIT_39,
	ASPLIT_40,
	ASPLIT_41,
	ASPLIT_42,
	ASPLIT_43,
	ASPLIT_44,
	ASPLIT_45,
	ASPLIT_46,
	ASPLIT_47,
	ASPLIT_48,
	ASPLIT_49,
	ASPLIT_50,
	ASPLIT_51,
	ASPLIT_52,
	ASPLIT_53,
	ASPLIT_54,
	ASPLIT_55,
	ASPLIT_56,
	ASPLIT_57,
	ASTREET_1,
	ASTREET_2,
	ASTREET_3,
	ASTREET_4,
	ASTREET_5,
	ASTREET_6,
	ASTREET_7,
	ASTREET_8,
	ASTREET_9,
	ASTREET_10,
	ASTREET_11,
	ASTREET_12,
	ATHREE_1,
	ATHREE_2,
	ACORNER_1,
	ACORNER_2,
	ACORNER_3,
	ACORNER_4,
	ACORNER_5,
	ACORNER_6,
	ACORNER_7,
	ACORNER_8,
	ACORNER_9,
	ACORNER_10,
	ACORNER_11,
	ACORNER_12,
	ACORNER_13,
	ACORNER_14,
	ACORNER_15,
	ACORNER_16,
	ACORNER_17,
	ACORNER_18,
	ACORNER_19,
	ACORNER_20,
	ACORNER_21,
	ACORNER_22,
	AFOUR_1,
	ALINE_1,
	ALINE_2,
	ALINE_3,
	ALINE_4,
	ALINE_5,
	ALINE_6,
	ALINE_7,
	ALINE_8,
	ALINE_9,
	ALINE_10,
	ALINE_11,
	ACOLUMN_1,
	ACOLUMN_2,
	ACOLUMN_3,
	ADOZEN_1,
	ADOZEN_2,
	ADOZEN_3,
	ABLACK_1,
	ARED_1,
	AODD_1,
	AEVEN_1,
	ALOW_1,
	AHI_1,
	ABET_AREA_COUNT
};

//�����ע����
#define MAX_CHIP_TIME								50
//////////////////////////////////////////////////////////////////////////

//��������
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	//��Ϸ����
protected:
	SCORE							m_lMaxChipBanker;					//�����ע (ׯ��)
	SCORE							m_lMaxChipUser;						//�����ע (����)
	SCORE							m_lAreaChip[AREA_COUNT];			//������ע 
	WORD							m_wCurrentBanker;					//ׯ��λ��
	BYTE							m_cbTimeLeave;						//ʣ��ʱ��

	int								m_nChipLimit[2];					//��ע��Χ (0-AREA_COUNT)
	int								m_nChipTime;						//��ע���� (����)
	int								m_nChipTimeCount;					//���´��� (����)
	
	//��ׯ����
protected:
	bool							m_bMeApplyBanker;					//�����ʶ
	int								m_nBankerCount;						//�������˵���ׯ����
	int								m_nWaitBanker;						//�ռ���
	static int						m_stlApplyBanker;					//������

	//���ñ���  (ȫ������)
protected:
	tagRobotInfo					m_RobotInfo;						//ȫ������
	TCHAR							m_szRoomName[32];					//���÷���

	//���ñ���	(��Ϸ����)
protected:
	bool							m_bRefreshCfg;						//ÿ��ˢ��
	SCORE							m_lAreaLimitScore;					//��������
	SCORE							m_lUserLimitScore;					//��ע����
	SCORE							m_lBankerCondition;					//��ׯ����		

	//���ñ���  (����������)
protected:
	SCORE							m_lRobotJettonLimit[2];				//��������	
	int								m_nRobotBetTimeLimit[2];			//��������	
	bool							m_bRobotBanker;						//�Ƿ���ׯ
	int								m_nRobotBankerCount;				//��ׯ����
	int								m_nRobotWaitBanker;					//��������
	int								m_nRobotApplyBanker;				//��ׯ����
	bool							m_bReduceJettonLimit;				//��������

	//�����˴�ȡ��
	LONGLONG						m_lRobotScoreRange[2];				//���Χ
	LONGLONG						m_lRobotBankGetScore;				//�������
	LONGLONG						m_lRobotBankGetScoreBanker;			//������� (ׯ��)
	int								m_nRobotBankStorageMul;				//����

	int								m_nAreaNum[154][18];				//��ע����

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	IAndroidUserItem *				m_pIAndroidUserItem;				//�û��ӿ�

	//��������
public:
	//���캯��
	CAndroidUserItemSink();
	//��������
	virtual ~CAndroidUserItemSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID  Release() {delete this; }
	
	//�ӿڲ�ѯ
	virtual VOID *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���ƽӿ�
public:
	//��ʼ�ӿ�
	virtual bool  Initialization(IUnknownEx * pIUnknownEx);
	//���ýӿ�
	virtual bool  RepositionSink();

	//��Ϸ�¼�
public:
	//ʱ����Ϣ
	virtual bool  OnEventTimer(UINT nTimerID);
	//��Ϸ��Ϣ
	virtual bool  OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��Ϸ��Ϣ
	virtual bool  OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������Ϣ
	virtual bool  OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize);

	//�û��¼�
public:
	//�û�����
	virtual void  OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û��뿪
	virtual void  OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û�����
	virtual void  OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û�״̬
	virtual void  OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	
	//��Ϣ����
public:
	//��Ϸ����
	bool OnSubGameFree(const void * pBuffer, WORD wDataSize);
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubPlaceJetton(const void * pBuffer, WORD wDataSize);
	//��עʧ��
	bool OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//������ׯ
	bool OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize);
	//ȡ����ׯ
	bool OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize);
	//�л�ׯ��
	bool OnSubChangeBanker(const void * pBuffer, WORD wDataSize);

	//���ܺ���
public:
  //��ȡ����
  void ReadConfigInformation(tagCustomAndroid *pCustomAndroid);
  //���㷶Χ
  bool CalcJettonRange(SCORE lMaxScore, SCORE lChipLmt[], int & nChipTime, int lJetLmt[]);
  //��ȡ����
  LONGLONG GetPrivateProfileInt64(LPCTSTR lpAppName, LPCTSTR lpKeyName, LONGLONG lDefault, LPCTSTR lpFileName);
};

//////////////////////////////////////////////////////////////////////////

#endif
