#ifndef CONTROLHEAD_HEAD_FILE
#define CONTROLHEAD_HEAD_FILE
#include <Afxtempl.h>
#pragma pack(1)

//百家乐
#define BACCARAT_AREA_MAX			5										//百家乐区域数量
#define BACCARAT_PLAYER				100										//百家乐人数
//百家乐控制参数
struct tag_Baccarat_Parameter
{
	WORD							wBankerUser;									//庄家用户
	BYTE							cbJuControl;;									//是否进行局控
	BYTE							cbJuControlArea[BACCARAT_AREA_MAX];				//本局控制哪门胜利 0不控制，1控制赢，2控制输

	SCORE							lPlayBet[BACCARAT_PLAYER][BACCARAT_AREA_MAX];	//下注情况
};

//百家乐控制结果
struct tag_Baccarat_Result
{
	BYTE							cbCardCount[2];							//扑克数目
	BYTE							cbTableCardArray[2][3];					//桌面扑克
};

//红黑
#define REDBLACK_AREA_MAX			3										//红黑区域数量
#define REDBLACK_PLAYER				100										//红黑人数
//红黑控制参数
struct tag_Redblack_Parameter
{
	WORD							wBankerUser;									//庄家用户
	BYTE							cbJuControl;;									//是否进行局控
	BYTE							cbJuControlArea[REDBLACK_AREA_MAX];				//本局控制哪门胜利 0不控制，1控制赢，2控制输

	SCORE							lPlayBet[REDBLACK_PLAYER][REDBLACK_AREA_MAX];	//下注情况
};

//红黑控制结果
struct tag_Redblack_Result
{
	BYTE							cbCardCount[2];							//扑克数目
	BYTE							cbTableCardArray[2][3];					//桌面扑克
};

//龙虎
#define LONGHU_AREA_MAX				3										//龙虎区域数量
#define LONGHU_PLAYER				100										//龙虎人数

//龙虎控制参数
struct tag_LongHu_Parameter
{
	WORD							wBankerUser;									//庄家用户
	BYTE							cbJuControl;;									//是否进行局控
	BYTE							cbJuControlArea[LONGHU_AREA_MAX];				//本局控制哪门胜利 0不控制，1控制赢，2控制输

	SCORE							lPlayBet[LONGHU_PLAYER][LONGHU_AREA_MAX];	//下注情况
};

//龙虎控制结果
struct tag_LongHu_Result
{
	BYTE							cbTableCardArray[2];					//桌面扑克
};

//百人牛牛 
#define OXBATTLE_AREA_MAX			4												//百人牛牛区域数量
#define OXBATTLE_PLAYER				100												//百人牛牛人数
//百人牛牛控制参数
struct tag_OxBattle_Parameter
{
	WORD							wBankerUser;									//庄家用户
	BYTE							cbJuControl;;									//是否进行局控
	BYTE							cbJuControlArea[OXBATTLE_AREA_MAX + 1];			//本局控制哪门胜利 0不控制，1控制赢，2控制输

	BYTE							cbTableCardArray[5][5];							//已经发过了牌
	SCORE							lPlayBet[OXBATTLE_PLAYER][OXBATTLE_AREA_MAX + 1];	//下注情况
};

//百人牛牛控制结果
struct tag_OxBattle_Result
{
	BYTE							cbTableCardArray[5][5];							//桌面扑克
};

//斗地主
#define LAND_NORMAL_COUNT			17												//常规数量
#define LAND_FULL_COUNT				54												//扑克数量
#define LAND_GAME_PLAYER			3												//斗地主人数

//斗地主控制结果
struct tag_Land_Result
{
	BYTE							cbTableCardArray[LAND_FULL_COUNT];				//扑克
};

//扎金花
#define ZHAJINHUA_GAME_PLAYER		5												//扎金花人数

//扎金花控制结果
struct tag_ZhaJinHua_Result
{
	BYTE							cbTableCardArray[ZHAJINHUA_GAME_PLAYER][3];		//扑克
};

//抢庄牛牛
#define OXSIX_GAME_PLAYER			5												//抢庄牛牛最大人数
#define OXSIX_MAX_CARDCOUNT			5												//抢庄牛牛最大扑克

//抢庄牛牛控制参数
struct tag_OxSix_Parameter
{
	WORD							wBankerUser;									//庄家用户

	BYTE							cbPlayerStatus[OXSIX_GAME_PLAYER];				//玩家状态

	SCORE							lExitScore;										//强退分数
	SCORE							lTableScore[OXSIX_GAME_PLAYER];					//桌面分数

	BYTE							cbCallBankerTimes[OXSIX_GAME_PLAYER];			//叫庄倍数

	BYTE							cbTableCardArray[5][5];							//已经发过了牌
};
//抢庄牛牛控制结果
struct tag_OxSix_Result
{
	BYTE							cbTableCardArray[OXSIX_GAME_PLAYER][OXSIX_MAX_CARDCOUNT];		//扑克
};

//深海捕鱼
//深海捕鱼控制参数
struct tag_Shby_Parameter
{
	WORD							wshutUser;									//开炮用户
	double							probability;								//概率
};

//深海捕鱼控制结果
struct tag_Shby_Result
{
	double							probability;								//概率
};

//金蟾捕鱼
//金蟾捕鱼控制参数
struct tag_Jcby_Parameter
{
	WORD							wshutUser;									//开炮用户
	double							probability;								//概率
};
//金蟾捕鱼控制结果
struct tag_Jcby_Result
{
	double							probability;								//概率
};

//大闹天宫捕鱼
//大闹天宫捕鱼控制参数
struct tag_Dntg_Parameter
{
	WORD							wshutUser;									//开炮用户
	double							probability;								//概率
};
//大闹天宫捕鱼控制结果
struct tag_Dntg_Result
{
	double							probability;								//概率
};

//财神到
#define CAISHEN_X_COUNT					5									//图标数量
#define CAISHEN_Y_COUNT					3									//图标数量
//财神到控制参数
struct tag_CaiShen_Parameter
{
	SCORE								lBetScore;							//押分
};
//财神到控制结果
struct tag_CaiShen_Result
{
	BYTE								cbItemInfo[CAISHEN_Y_COUNT][CAISHEN_X_COUNT];	//开奖信息
};

//绝地求生
#define JDQS_X_COUNT					5									//图标数量
#define JDQS_Y_COUNT					3									//图标数量
//绝地求生控制参数
struct tag_JDQS_Parameter
{
	SCORE								lBetScore;							//押分
};
//绝地求生控制结果
struct tag_JDQS_Result
{
	BYTE								cbItemInfo[JDQS_Y_COUNT][JDQS_X_COUNT];	//开奖信息
};

//飞禽走兽
#define FQZS_AREA_COUNT					12									//区域数量
#define FQZS_ANIMAL_COUNT				28									//动物数量
#define FQZS_GAME_PLAYER				100									//飞禽走兽人数

//飞禽走兽控制参数
struct tag_FQZS_Parameter
{
	WORD								wCurrentBanker;						//当前庄家
	BYTE								cbJuControl;;						//是否进行局控
	BYTE								cbJuControlArea;					//本局控制哪门胜利 0不控制，1控制赢，2控制输
	SCORE								lUserJettonScore[FQZS_GAME_PLAYER][FQZS_AREA_COUNT];	//用户下注
	INT									nMultiple[FQZS_AREA_COUNT];			//区域倍数
};
//飞禽走兽控制结果
struct tag_FQZS_Result
{
	BYTE								cbTableCardArray[2];				//桌面扑克
};

//奔驰宝马
#define BCBM_AREA_COUNT					8									//区域数量
#define BCBM_ANIMAL_COUNT				32									//动物数量
#define BCBM_GAME_PLAYER				100									//飞禽走兽人数

//奔驰宝马控制参数
struct tag_BCBM_Parameter
{
	WORD								wCurrentBanker;						//当前庄家
	BYTE								cbJuControl;;						//是否进行局控
	BYTE								cbJuControlArea;					//本局控制哪门胜利 0不控制，1控制赢，2控制输
	SCORE								lUserJettonScore[BCBM_GAME_PLAYER][BCBM_AREA_COUNT];	//用户下注
	INT									nMultiple[BCBM_AREA_COUNT];			//区域倍数
};
//奔驰宝马控制结果
struct tag_BCBM_Result
{
	BYTE								cbTableCardArray;					//桌面扑克
};

//水浒传
#define SHZ_X_COUNT					5										//图标数量
#define SHZ_Y_COUNT					3										//图标数量

//水浒传控制参数
struct tag_SHZ_Parameter
{
	BYTE								cbOneMode;							//压线还是比倍
	SCORE								lBetScore;							//押分
	BYTE								cbOpenArea;							//比倍区域
};

//水浒传控制结果
struct tag_SHZ_Result
{
	BYTE								cbItemInfo[SHZ_Y_COUNT][SHZ_X_COUNT];	//开奖信息
	BYTE								cbOpenSize[2];						//开奖结果
};

//轮盘
#define LP_AREA_COUNT					49									//区域数量
#define LP_RESULT_COUNT					37									//结果数目
#define LP_GAME_PLAYER					100									//人数

//轮盘控制参数
struct tag_LP_Parameter
{
	SCORE								lUserJettonScore[LP_GAME_PLAYER][LP_AREA_COUNT];	//用户下注
};
//轮盘控制结果
struct tag_LP_Result
{
	BYTE								cbTableCardArray;					//桌面扑克
};

//骰宝
#define SICBO_AREA_COUNT				35									//区域数量
#define SICBO_RESULT_COUNT				6									//结果数目
#define SICBO_GAME_PLAYER				100									//人数

//骰宝控制参数
struct tag_SICBO_Parameter
{
	SCORE								lUserJettonScore[SICBO_GAME_PLAYER][SICBO_AREA_COUNT];	//用户下注
};
//骰宝控制结果
struct tag_SICBO_Result
{
	BYTE								cbTableCardArray[3];				//桌面扑克
};

//十三水
#define THIRTEEN_CARD_COUNT			13												//常规数量
#define THIRTEEN_GAME_PLAYER		4												//十三水人数

//十三水控制结果
struct tag_Thirteen_Result
{
	BYTE							cbTableCardArray[THIRTEEN_GAME_PLAYER][THIRTEEN_CARD_COUNT];				//扑克
};

//百人二八杠 
#define TONGZI_AREA_MAX				4												//百人二八杠区域数量
#define TONGZI_PLAYER				100												//百人二八杠人数
//百人二八杠控制参数
struct tag_TongZi_Parameter
{
	WORD							wBankerUser;									//庄家用户
	BYTE							cbJuControl;;									//是否进行局控
	BYTE							cbJuControlArea[TONGZI_AREA_MAX];				//本局控制哪门胜利 0不控制，1控制赢，2控制输

	BYTE							cbLeftCardCount;								//剩余牌数目
	BYTE							cbTableCard[40];								//剩余牌

	SCORE							lPlayBet[TONGZI_PLAYER][TONGZI_AREA_MAX];		//下注情况
};

//百人二八杠控制结果
struct tag_TongZi_Result
{
	BYTE							cbTableCardArray[4][2];							//桌面扑克
};

//三公
#define SG_GAME_PLAYER				9												//三公最大人数
#define SG_MAX_CARDCOUNT			3												//三公最大扑克

//三公控制参数
struct tag_SG_Parameter
{
	WORD							wBankerUser;									//庄家用户
	BYTE							cbPlayerStatus[SG_GAME_PLAYER];					//玩家状态
	SCORE							lTableScore[SG_GAME_PLAYER];					//桌面分数
	BYTE							cbCallBankerTimes[SG_GAME_PLAYER];				//叫庄倍数
	BYTE							cbTableCardArray[SG_GAME_PLAYER][SG_MAX_CARDCOUNT];	//已经发过了牌
};
//三公控制结果
struct tag_SG_Result
{
	BYTE							cbTableCardArray[SG_GAME_PLAYER][SG_MAX_CARDCOUNT];	//扑克
};

//水果拉霸
#define SGLB_X_COUNT					5										//图标数量
#define SGLB_Y_COUNT					3										//图标数量

//水果拉霸控制参数
struct tag_SGLB_Parameter
{
	BYTE								cbOneMode;							//压线还是开箱子
	SCORE								lBetScore;							//押分
};

//水果拉霸控制结果
struct tag_SGLB_Result
{
	BYTE								cbItemInfo[SGLB_Y_COUNT][SGLB_X_COUNT];	//开奖信息
	SCORE								lBonusScore;							//箱子分数
};

#pragma pack()

#endif