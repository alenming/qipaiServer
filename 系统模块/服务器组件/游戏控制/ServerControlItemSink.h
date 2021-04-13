#pragma once
#include "../��Ϸ������/ServerControl.h"
#include "BaccaratLogic.h"
#include "RedBlackLogic.h"
#include "LongHuLogic.h"
#include "OxBattleLogic.h"
#include "LandLogic.h"
#include "ZhaJinHuaLogic.h"
#include "OxSixLogic.h"
#include "CaiShenLogic.h"
#include "JDQSLogic.h"
#include "SHZLogic.h"
#include "ThirteenLogic.h"
#include "TongZiLogic.h"
#include "SGLogic.h"
#include "SGLBLogic.h"

struct tag_Baccart_ScoreParameter
{
	WORD							wBankerUser;
	SCORE							lPlayBet[BACCARAT_PLAYER][BACCARAT_AREA_MAX];
	BYTE							cbTableCard[2][3];
	BYTE							cbCardCount[2];
};

struct tag_RedBlack_ScoreParameter
{
	WORD							wBankerUser;
	SCORE							lPlayBet[REDBLACK_PLAYER][REDBLACK_AREA_MAX];
	BYTE							cbTableCard[2][3];
	BYTE							cbCardCount[2];
};

struct tag_LongHu_ScoreParameter
{
	WORD							wBankerUser;
	SCORE							lPlayBet[LONGHU_PLAYER][LONGHU_AREA_MAX];
	BYTE							cbTableCard[2];
};

struct tag_OxBattle_ScoreParameter
{
	WORD							wBankerUser;
	SCORE							lPlayBet[OXBATTLE_PLAYER][OXBATTLE_AREA_MAX + 1];
	BYTE							cbTableCard[5][5];
};

struct tag_OxSix_ScoreParameter
{
	WORD							wBankerUser;									//ׯ���û�

	SCORE							lExitScore;										//ǿ�˷���
	SCORE							lTableScore[OXSIX_GAME_PLAYER];					//�������

	BYTE							cbCallBankerTimes[OXSIX_GAME_PLAYER];			//��ׯ����

	BYTE							cbPlayerStatus[OXSIX_GAME_PLAYER];				//���״̬
	BYTE							cbTableCard[5][5];
};

struct tag_FQZS_ScoreParameter
{
	WORD							wBankerUser;									//ׯ���û�
	SCORE							lUserJettonScore[FQZS_GAME_PLAYER][FQZS_AREA_COUNT];	//�û���ע
	INT								nMultiple[FQZS_AREA_COUNT];						//������
	BYTE							cbTableCardArray[2];							//�����˿�
};

struct tag_BCBM_ScoreParameter
{
	WORD							wBankerUser;									//ׯ���û�
	SCORE							lUserJettonScore[BCBM_GAME_PLAYER][BCBM_AREA_COUNT];	//�û���ע
	INT								nMultiple[BCBM_AREA_COUNT];						//������
	BYTE							cbTableCardArray;								//�����˿�
};

struct tag_LP_ScoreParameter
{
	SCORE							lUserJettonScore[LP_GAME_PLAYER][LP_AREA_COUNT];	//�û���ע
	BYTE							cbTableCardArray;								//�����˿�
};

struct tag_SICBO_ScoreParameter
{
	SCORE							lUserJettonScore[SICBO_GAME_PLAYER][SICBO_AREA_COUNT];	//�û���ע
	BYTE							cbTableCardArray[3];								//�����˿�
};

struct tag_Thirteen_ScoreParameter
{
	BYTE							cbTableCardArray[THIRTEEN_GAME_PLAYER][THIRTEEN_CARD_COUNT];//�����˿�
};

struct tag_TongZi_ScoreParameter
{
	WORD							wBankerUser;
	SCORE							lPlayBet[TONGZI_PLAYER][TONGZI_AREA_MAX];
	BYTE							cbTableCard[4][2];						//�����˿�
};

struct tag_SG_ScoreParameter
{
	WORD							wBankerUser;									//ׯ���û�

	SCORE							lTableScore[SG_GAME_PLAYER];					//�������

	BYTE							cbCallBankerTimes[SG_GAME_PLAYER];				//��ׯ����

	BYTE							cbPlayerStatus[SG_GAME_PLAYER];					//���״̬
	BYTE							cbTableCard[SG_GAME_PLAYER][SG_MAX_CARDCOUNT];
};

class CServerControlItemSink : public IServerControl
{
private:
	static const BYTE				m_cbBarracatCardList[52*8];				//�ټ����˿˶���

	int								m_nLPAreaNums[LP_AREA_COUNT][18];		//������������

protected:
	ITableFrame	*					m_pITableFrame;							//��ܽӿ�
	tagGameServiceOption*			m_pGameServiceOption;					//���ò���

	CBaccaratLogic					m_BaccaratLogic;						//��Ϸ�߼� �ټ���
	CRedBlackLogic					m_RedBlackLogic;						//��Ϸ�߼� ���
	CLongHuLogic					m_LongHuLogic;							//��Ϸ�߼� ������
	COxBattleLogic					m_OxBattleLogic;						//��Ϸ�߼� ����ţţ
	CLandLogic						m_LandLogic;							//��Ϸ�߼� ������
	CZhaJinHuaLogic					m_ZhaJinHuaLogic;						//��Ϸ�߼� ����
	COxSixLogic						m_OxSixLogic;							//��Ϸ�߼� ��ׯţţ
	CCaiShenLogic					m_CaiShenLogic;							//��Ϸ�߼� ����
	CJDQSLogic						m_JDQSLogic;							//��Ϸ�߼� ��������
	CSHZLogic						m_SHZLogic;								//��Ϸ�߼� ˮ䰴�
	CThirteenLogic					m_ThLogic;								//��Ϸ�߼� ʮ��ˮ
	CTongZiLogic					m_TzLogic;								//��Ϸ�߼� ���˸�
	CSGLogic						m_SGLogic;								//��Ϸ�߼� ����
	CSGLBLogic						m_SGLBLogic;							//��Ϸ�߼� ˮ������

public:
	CServerControlItemSink(void);
	virtual ~CServerControlItemSink(void);

public:
	//�������
	virtual bool ActiveControl(IUnknownEx * pIUnknownEx);
	//���ؿ�������
	virtual VOID * ControlResult(VOID * pData, WORD wDataSize, VOID * pResult);

protected:
	//�ټ��ֿ��ƽ��
	VOID * OnBaccaratControlResult(VOID * pData, WORD wDataSize, VOID * pResult);
	//�ټ���ϴ��
	void DispatchBaccaratCard(tag_Baccarat_Result *Baccarat_Result);
	//�����˿����鸳ֵ�˿�����
	void CalculateCardCount(tag_Baccarat_Result *Baccarat_Result);
	//�ټ��ּ���ϵͳ��
	SCORE CalculateBaccaratScore(tag_Baccart_ScoreParameter ScoreParameter);

	//��ڿ��ƽ��
	VOID * OnRedBlackControlResult(VOID * pData, WORD wDataSize, VOID * pResult);
	//���ϴ��
	void DispatchRedBlackCard(tag_Redblack_Result *Redblack_Result);
	//��ڼ���ϵͳ��
	SCORE CalculateRedBlackScore(tag_RedBlack_ScoreParameter ScoreParameter);

	//�������ƽ��
	VOID * OnLongHuDouControlResult(VOID * pData, WORD wDataSize, VOID * pResult);
	//����ϴ��
	void DispatchLongHuCard(tag_LongHu_Result *LongHu_Result);
	//��������ϵͳ��
	SCORE CalculateLongHuScore(tag_LongHu_ScoreParameter ScoreParameter);

	//����ţţ���ƽ��
	VOID * OnOxBattleControlResult(VOID * pData, WORD wDataSize, VOID * pResult);
	//����ţţϴ��
	void DispatchOxBattleCard(tag_OxBattle_Result *OxBattle_Result);
	//����ţţ����ϵͳ��
	SCORE CalculateOxBattleScore(tag_OxBattle_ScoreParameter ScoreParameter);
	//����ţţ����ׯ����Ӯ��
	SCORE CalculateOxBattleBankerScore(tag_OxBattle_ScoreParameter ScoreParameter);

	//���������ƽ��
	VOID * OnLandControlResult(VOID * pData, WORD wDataSize, VOID * pResult);

	//���𻨿��ƽ��
	VOID * OnZhaJinHuaControlResult(VOID * pData, WORD wDataSize, VOID * pResult);

	//��ׯţţ���ƽ��
	VOID * OnOxSixControlResult(VOID * pData, WORD wDataSize, VOID * pResult);
	//��ׯţţ����ϵͳ��
	SCORE CalculateOxSixScore(tag_OxSix_ScoreParameter ScoreParameter);

	//�������ƽ��
	VOID * OnShbyControlResult(VOID * pData, WORD wDataSize, VOID * pResult);

	//��󸲶����ƽ��
	VOID * OnJcbyControlResult(VOID * pData, WORD wDataSize, VOID * pResult);

	//�����칬������ƽ��
	VOID * OnDntgControlResult(VOID * pData, WORD wDataSize, VOID * pResult);

	//���񵽿��ƽ��
	VOID * OnCaiShenControlResult(VOID * pData, WORD wDataSize, VOID * pResult);

	//�����������ƽ��
	VOID * OnJDQSControlResult(VOID * pData, WORD wDataSize, VOID * pResult);

	//�������޿��ƽ��
	VOID * OnFQZSControlResult(VOID * pData, WORD wDataSize, VOID * pResult);
	//�������޻�ȡ�����ʤ
	BYTE cbFQZSGetAreaAnimal(BYTE cbCardData);
	//�������޼���ϵͳ��
	SCORE CalculateFQZSScore(tag_FQZS_ScoreParameter ScoreParameter);

	//���۱�����ƽ��
	VOID * OnBCBMControlResult(VOID * pData, WORD wDataSize, VOID * pResult);
	//���۱����ȡ�����ʤ
	BYTE cbBCBMGetAreaAnimal(BYTE cbCardData);
	//���۱������ϵͳ��
	SCORE CalculateBCBMScore(tag_BCBM_ScoreParameter ScoreParameter);

	//ˮ䰴����ƽ��
	VOID * OnSHZControlResult(VOID * pData, WORD wDataSize, VOID * pResult);

	//���̿��ƽ��
	VOID * OnLPControlResult(VOID * pData, WORD wDataSize, VOID * pResult);
	//���̻�ȡʤ������
	BYTE cbLPGetWinArea(BYTE cbTableCard, BYTE cbWinArea[]);
	//���̻�ȡ������
	BYTE cbLPGetAreaTimes(BYTE cbArea);
	//���̼���ϵͳ��
	SCORE CalculateLPScore(tag_LP_ScoreParameter ScoreParameter);

	//�������ƽ��
	VOID * OnSICBOControlResult(VOID * pData, WORD wDataSize, VOID * pResult);
	//��������ϵͳ��
	SCORE CalculateSICBOScore(tag_SICBO_ScoreParameter ScoreParameter);

	//ʮ��ˮ���ƽ��
	VOID * OnThirteenControlResult(VOID * pData, WORD wDataSize, VOID * pResult);
	//ʮ��ˮ����ϵͳ��
	SCORE CalculateThirteenScore(tag_Thirteen_ScoreParameter ScoreParameter);

	//���˶��˸ܿ��ƽ��
	VOID * OnTongziControlResult(VOID * pData, WORD wDataSize, VOID * pResult);
	//���˶��˸�ϴ��
	void DispatchTongziCard(tag_TongZi_Result *OxBattle_Result,BYTE cbLeftCardCount,BYTE cbTableCard[]);
	//���˶��˸ܼ���ϵͳ��
	SCORE CalculateTongziScore(tag_TongZi_ScoreParameter ScoreParameter);
	//���˶��˸ܼ���ׯ����Ӯ��
	SCORE CalculateTongziBankerScore(tag_TongZi_ScoreParameter ScoreParameter);

	//�������ƽ��
	VOID * OnSanGongControlResult(VOID * pData, WORD wDataSize, VOID * pResult);
	//��������ϵͳ��
	SCORE CalculateSGScore(tag_SG_ScoreParameter ScoreParameter);

	//ˮ�����Կ��ƽ��
	VOID * OnSGLBControlResult(VOID * pData, WORD wDataSize, VOID * pResult);
	//ˮ���������4��С��������
	void GetSGLBMaryAreaItem(BYTE cbThreeItem[], BYTE cbControlType);
	//ˮ�����Ի�ȡС������������
	int GetSGLBMaryPrizeTimes(BYTE cbThreeIndex, BYTE cbThreeItem[]);

	void GetValueFromCombStr(SCORE llData[], int nDataLen, LPCTSTR sTr, int nStrLen);
};
