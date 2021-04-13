#pragma once
#include "../游戏服务器/ServerControl.h"
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
	WORD							wBankerUser;									//庄家用户

	SCORE							lExitScore;										//强退分数
	SCORE							lTableScore[OXSIX_GAME_PLAYER];					//桌面分数

	BYTE							cbCallBankerTimes[OXSIX_GAME_PLAYER];			//叫庄倍数

	BYTE							cbPlayerStatus[OXSIX_GAME_PLAYER];				//玩家状态
	BYTE							cbTableCard[5][5];
};

struct tag_FQZS_ScoreParameter
{
	WORD							wBankerUser;									//庄家用户
	SCORE							lUserJettonScore[FQZS_GAME_PLAYER][FQZS_AREA_COUNT];	//用户下注
	INT								nMultiple[FQZS_AREA_COUNT];						//区域倍数
	BYTE							cbTableCardArray[2];							//桌面扑克
};

struct tag_BCBM_ScoreParameter
{
	WORD							wBankerUser;									//庄家用户
	SCORE							lUserJettonScore[BCBM_GAME_PLAYER][BCBM_AREA_COUNT];	//用户下注
	INT								nMultiple[BCBM_AREA_COUNT];						//区域倍数
	BYTE							cbTableCardArray;								//桌面扑克
};

struct tag_LP_ScoreParameter
{
	SCORE							lUserJettonScore[LP_GAME_PLAYER][LP_AREA_COUNT];	//用户下注
	BYTE							cbTableCardArray;								//桌面扑克
};

struct tag_SICBO_ScoreParameter
{
	SCORE							lUserJettonScore[SICBO_GAME_PLAYER][SICBO_AREA_COUNT];	//用户下注
	BYTE							cbTableCardArray[3];								//桌面扑克
};

struct tag_Thirteen_ScoreParameter
{
	BYTE							cbTableCardArray[THIRTEEN_GAME_PLAYER][THIRTEEN_CARD_COUNT];//桌面扑克
};

struct tag_TongZi_ScoreParameter
{
	WORD							wBankerUser;
	SCORE							lPlayBet[TONGZI_PLAYER][TONGZI_AREA_MAX];
	BYTE							cbTableCard[4][2];						//桌面扑克
};

struct tag_SG_ScoreParameter
{
	WORD							wBankerUser;									//庄家用户

	SCORE							lTableScore[SG_GAME_PLAYER];					//桌面分数

	BYTE							cbCallBankerTimes[SG_GAME_PLAYER];				//叫庄倍数

	BYTE							cbPlayerStatus[SG_GAME_PLAYER];					//玩家状态
	BYTE							cbTableCard[SG_GAME_PLAYER][SG_MAX_CARDCOUNT];
};

class CServerControlItemSink : public IServerControl
{
private:
	static const BYTE				m_cbBarracatCardList[52*8];				//百家乐扑克定义

	int								m_nLPAreaNums[LP_AREA_COUNT][18];		//轮盘区域数字

protected:
	ITableFrame	*					m_pITableFrame;							//框架接口
	tagGameServiceOption*			m_pGameServiceOption;					//配置参数

	CBaccaratLogic					m_BaccaratLogic;						//游戏逻辑 百家乐
	CRedBlackLogic					m_RedBlackLogic;						//游戏逻辑 红黑
	CLongHuLogic					m_LongHuLogic;							//游戏逻辑 龙虎斗
	COxBattleLogic					m_OxBattleLogic;						//游戏逻辑 百人牛牛
	CLandLogic						m_LandLogic;							//游戏逻辑 斗地主
	CZhaJinHuaLogic					m_ZhaJinHuaLogic;						//游戏逻辑 扎金花
	COxSixLogic						m_OxSixLogic;							//游戏逻辑 抢庄牛牛
	CCaiShenLogic					m_CaiShenLogic;							//游戏逻辑 财神到
	CJDQSLogic						m_JDQSLogic;							//游戏逻辑 绝地求生
	CSHZLogic						m_SHZLogic;								//游戏逻辑 水浒传
	CThirteenLogic					m_ThLogic;								//游戏逻辑 十三水
	CTongZiLogic					m_TzLogic;								//游戏逻辑 二八杠
	CSGLogic						m_SGLogic;								//游戏逻辑 三公
	CSGLBLogic						m_SGLBLogic;							//游戏逻辑 水果拉霸

public:
	CServerControlItemSink(void);
	virtual ~CServerControlItemSink(void);

public:
	//激活控制
	virtual bool ActiveControl(IUnknownEx * pIUnknownEx);
	//返回控制区域
	virtual VOID * ControlResult(VOID * pData, WORD wDataSize, VOID * pResult);

protected:
	//百家乐控制结果
	VOID * OnBaccaratControlResult(VOID * pData, WORD wDataSize, VOID * pResult);
	//百家乐洗牌
	void DispatchBaccaratCard(tag_Baccarat_Result *Baccarat_Result);
	//根据扑克数组赋值扑克数量
	void CalculateCardCount(tag_Baccarat_Result *Baccarat_Result);
	//百家乐计算系统分
	SCORE CalculateBaccaratScore(tag_Baccart_ScoreParameter ScoreParameter);

	//红黑控制结果
	VOID * OnRedBlackControlResult(VOID * pData, WORD wDataSize, VOID * pResult);
	//红黑洗牌
	void DispatchRedBlackCard(tag_Redblack_Result *Redblack_Result);
	//红黑计算系统分
	SCORE CalculateRedBlackScore(tag_RedBlack_ScoreParameter ScoreParameter);

	//龙虎控制结果
	VOID * OnLongHuDouControlResult(VOID * pData, WORD wDataSize, VOID * pResult);
	//龙虎洗牌
	void DispatchLongHuCard(tag_LongHu_Result *LongHu_Result);
	//龙虎计算系统分
	SCORE CalculateLongHuScore(tag_LongHu_ScoreParameter ScoreParameter);

	//百人牛牛控制结果
	VOID * OnOxBattleControlResult(VOID * pData, WORD wDataSize, VOID * pResult);
	//百人牛牛洗牌
	void DispatchOxBattleCard(tag_OxBattle_Result *OxBattle_Result);
	//百人牛牛计算系统分
	SCORE CalculateOxBattleScore(tag_OxBattle_ScoreParameter ScoreParameter);
	//百人牛牛计算庄家输赢分
	SCORE CalculateOxBattleBankerScore(tag_OxBattle_ScoreParameter ScoreParameter);

	//斗地主控制结果
	VOID * OnLandControlResult(VOID * pData, WORD wDataSize, VOID * pResult);

	//扎金花控制结果
	VOID * OnZhaJinHuaControlResult(VOID * pData, WORD wDataSize, VOID * pResult);

	//抢庄牛牛控制结果
	VOID * OnOxSixControlResult(VOID * pData, WORD wDataSize, VOID * pResult);
	//抢庄牛牛计算系统分
	SCORE CalculateOxSixScore(tag_OxSix_ScoreParameter ScoreParameter);

	//深海捕鱼控制结果
	VOID * OnShbyControlResult(VOID * pData, WORD wDataSize, VOID * pResult);

	//金蟾捕鱼控制结果
	VOID * OnJcbyControlResult(VOID * pData, WORD wDataSize, VOID * pResult);

	//大闹天宫捕鱼控制结果
	VOID * OnDntgControlResult(VOID * pData, WORD wDataSize, VOID * pResult);

	//财神到控制结果
	VOID * OnCaiShenControlResult(VOID * pData, WORD wDataSize, VOID * pResult);

	//绝地求生控制结果
	VOID * OnJDQSControlResult(VOID * pData, WORD wDataSize, VOID * pResult);

	//飞禽走兽控制结果
	VOID * OnFQZSControlResult(VOID * pData, WORD wDataSize, VOID * pResult);
	//飞禽走兽获取动物获胜
	BYTE cbFQZSGetAreaAnimal(BYTE cbCardData);
	//飞禽走兽计算系统分
	SCORE CalculateFQZSScore(tag_FQZS_ScoreParameter ScoreParameter);

	//奔驰宝马控制结果
	VOID * OnBCBMControlResult(VOID * pData, WORD wDataSize, VOID * pResult);
	//奔驰宝马获取动物获胜
	BYTE cbBCBMGetAreaAnimal(BYTE cbCardData);
	//奔驰宝马计算系统分
	SCORE CalculateBCBMScore(tag_BCBM_ScoreParameter ScoreParameter);

	//水浒传控制结果
	VOID * OnSHZControlResult(VOID * pData, WORD wDataSize, VOID * pResult);

	//轮盘控制结果
	VOID * OnLPControlResult(VOID * pData, WORD wDataSize, VOID * pResult);
	//轮盘获取胜利区域
	BYTE cbLPGetWinArea(BYTE cbTableCard, BYTE cbWinArea[]);
	//轮盘获取区域倍数
	BYTE cbLPGetAreaTimes(BYTE cbArea);
	//轮盘计算系统分
	SCORE CalculateLPScore(tag_LP_ScoreParameter ScoreParameter);

	//骰宝控制结果
	VOID * OnSICBOControlResult(VOID * pData, WORD wDataSize, VOID * pResult);
	//骰宝计算系统分
	SCORE CalculateSICBOScore(tag_SICBO_ScoreParameter ScoreParameter);

	//十三水控制结果
	VOID * OnThirteenControlResult(VOID * pData, WORD wDataSize, VOID * pResult);
	//十三水计算系统分
	SCORE CalculateThirteenScore(tag_Thirteen_ScoreParameter ScoreParameter);

	//百人二八杠控制结果
	VOID * OnTongziControlResult(VOID * pData, WORD wDataSize, VOID * pResult);
	//百人二八杠洗牌
	void DispatchTongziCard(tag_TongZi_Result *OxBattle_Result,BYTE cbLeftCardCount,BYTE cbTableCard[]);
	//百人二八杠计算系统分
	SCORE CalculateTongziScore(tag_TongZi_ScoreParameter ScoreParameter);
	//百人二八杠计算庄家输赢分
	SCORE CalculateTongziBankerScore(tag_TongZi_ScoreParameter ScoreParameter);

	//三公控制结果
	VOID * OnSanGongControlResult(VOID * pData, WORD wDataSize, VOID * pResult);
	//三公计算系统分
	SCORE CalculateSGScore(tag_SG_ScoreParameter ScoreParameter);

	//水果拉霸控制结果
	VOID * OnSGLBControlResult(VOID * pData, WORD wDataSize, VOID * pResult);
	//水果拉霸随机4个小玛丽区域
	void GetSGLBMaryAreaItem(BYTE cbThreeItem[], BYTE cbControlType);
	//水果拉霸获取小玛丽开奖倍数
	int GetSGLBMaryPrizeTimes(BYTE cbThreeIndex, BYTE cbThreeItem[]);

	void GetValueFromCombStr(SCORE llData[], int nDataLen, LPCTSTR sTr, int nStrLen);
};
