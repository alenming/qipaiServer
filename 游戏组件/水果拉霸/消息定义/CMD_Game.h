#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

#pragma pack(1)

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//服务定义

//游戏属性
#define KIND_ID						119									//游戏 I D
#define GAME_NAME					TEXT("水果拉霸")					//游戏名字

//组件属性
#define GAME_PLAYER					1									//游戏人数
#define VERSION_SERVER				PROCESS_VERSION(7,0,1)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//程序版本

//数目定义
#define ITEM_COUNT					9									//图标数量
#define ITEM_X_COUNT				5									//图标数量
#define ITEM_Y_COUNT				3									//图标数量

//逻辑类型
#define CT_APPLE					1									//苹果
#define CT_MANGO					2									//芒果
#define CT_ORANGE					3									//橘子
#define CT_GRAPE					4									//葡萄
#define CT_WATERMELON				5									//西瓜
#define CT_CHERRY					6									//樱桃
#define CT_BELL						7									//铃铛
#define CT_DOUBLESEVEN				8									//双七
#define CT_BAR						9									//Bar
#define CT_WILD						10									//WILD
#define CT_BONUS					11									//BONUS
#define CT_SCATTER					12									//SCATTER(免费转)
#define CT_JACKPOT					13									//JACKPOT

#define ENUM_PRIZE_LINE				1									//中线
#define ENUM_PRIZE_BOX				2									//中箱子
#define ENUM_PRIZE_LUCKY			4									//中幸运草
#define ENUM_PRIZE_JACKPOT			8									//中奖池
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//状态定义
#define GAME_SCENE_FREE				GAME_STATUS_FREE					//等待开始

//空闲状态
struct CMD_S_StatusFree
{
	//下注值
	BYTE							cbBetCount;							//下注数量
	SCORE							lBetScore[10];						//下注大小

	SCORE							lPlayerScore;						//玩家钱数
	SCORE							lJackPot;							//奖金池
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//命令定义

#define SUB_S_BETRESULT				2002								//下注结算
#define SUB_S_BETFAIL				2003								//下注失败
#define SUB_S_JACKPOT_RESET			2005								//奖池重置

//游戏结束
struct CMD_S_GameConclude
{
	//积分变量
	SCORE							lBetScore;									//下注分数
	SCORE							lBonusScore[9];								//中奖箱子金币数
	SCORE							lGameScore;									//游戏积分
	BYTE							cbItemInfo[ITEM_Y_COUNT][ITEM_X_COUNT];		//开奖信息
	int								nAllFreeTimes;								//总的免费次数
	int								nUsedFreeTimes;								//已经使用的免费次数
	SCORE							lPlayerRestScore;							//玩家金币
	int								nLineMask;									//中奖线
	SCORE							lJackPot;									//当前奖池
	BYTE							cbSettleMask;								//0为没中奖
	SCORE							lWinPot;									//奖池盈利
};

//奖池重置
struct CMD_S_JackPot_Reset
{
	SCORE							lJackPot;									//奖池
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//命令定义
#define SUB_C_BETREQ				101									//下注

//用户叫分
struct CMD_C_OneStart
{
	int								lBet;								//押线分数
};
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

#pragma pack()

#endif