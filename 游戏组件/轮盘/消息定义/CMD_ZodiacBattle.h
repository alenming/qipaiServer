#ifndef CMD_ROULETTE_HEAD_FILE
#define CMD_ROULETTE_HEAD_FILE

#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//公共宏定义
#define KIND_ID						123									//游戏 I D
#define GAME_PLAYER					100									//游戏人数
#define GAME_NAME					TEXT("轮盘")						//游戏名字

//状态定义
#define	GS_PLACE_JETTON				GAME_STATUS_FREE					//下注状态
#define	GS_GAME_END					GAME_STATUS_PLAY					//结束状态

#define VERSION_SERVER			    PROCESS_VERSION(7,0,1)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//程序版本


//区域索引
#define BET_TYPE_STRAIGHT			1									//直接注(0-36个号码)   37
#define BET_TYPE_SPLIT				2									//分注(非0的相邻2个数) 57
#define BET_TYPE_STREET				3									//街注(1列3个数)       12
#define BET_TYPE_THREE				4									//三数({ 0, 1, 2 } {0, 2, 3})   2
#define BET_TYPE_CORNER				5									//角注(4个数位方框)  22
#define BET_TYPE_FOUR				6									//四个号码(0 - 3)    1
#define BET_TYPE_LINE				7									//线注(2列6个数)     11
#define BET_TYPE_COLUMN				8									//直行注(第一行:3. + 3.36 第二行 : 2. + 3.35 第三行 : 1. + 3.34)  3
#define BET_TYPE_DOZEN				9									//打注(第一打:1 - 12 第二打 : 13 - 24 第三打 : 25 - 36)   3
#define BET_TYPE_BLACK				10									//黑注(0通杀)   1
#define BET_TYPE_RED				11									//红注(0通杀)   1
#define BET_TYPE_ODD				12									//奇数注(0通杀) 1
#define BET_TYPE_EVEN				13									//偶数注(0通杀) 1
#define BET_TYPE_LOW				14									//低注(1 - 18 0通杀) 1
#define BET_TYPE_HI					15									//高注(19 - 36 0通杀) 1     154

#define AREA_COUNT					49									//区域个数

//历史记录
#define MAX_SCORE_HISTORY			72									//历史个数

#define USER_LIST_COUNT				20									//玩家列表一次发送数量

#define MAX_CHIP_COUNT				4									//筹码个数
#define MAX_SEAT_COUNT				3									//坐下玩家数量

enum BetAreas
{
	STRAIGHT_0,
	STRAIGHT_1,
	STRAIGHT_2,
	STRAIGHT_3,
	STRAIGHT_4,
	STRAIGHT_5,
	STRAIGHT_6,
	STRAIGHT_7,
	STRAIGHT_8,
	STRAIGHT_9,
	STRAIGHT_10,
	STRAIGHT_11,
	STRAIGHT_12,
	STRAIGHT_13,
	STRAIGHT_14,
	STRAIGHT_15,
	STRAIGHT_16,
	STRAIGHT_17,
	STRAIGHT_18,
	STRAIGHT_19,
	STRAIGHT_20,
	STRAIGHT_21,
	STRAIGHT_22,
	STRAIGHT_23,
	STRAIGHT_24,
	STRAIGHT_25,
	STRAIGHT_26,
	STRAIGHT_27,
	STRAIGHT_28,
	STRAIGHT_29,
	STRAIGHT_30,
	STRAIGHT_31,
	STRAIGHT_32,
	STRAIGHT_33,
	STRAIGHT_34,
	STRAIGHT_35,
	STRAIGHT_36,
	COLUMN_1,
	COLUMN_2,
	COLUMN_3,
	DOZEN_1,
	DOZEN_2,
	DOZEN_3,
	LOW_1,
	EVEN_1,
	RED_1,
	BLACK_1,
	ODD_1,
	HI_1,
	BET_AREA_COUNT
};

//机器人信息
struct tagRobotInfo
{
	int nChip[MAX_CHIP_COUNT];											//筹码定义
	int nAreaChance[AREA_COUNT];										//区域几率

	tagRobotInfo()
	{
		int nTmpChip[MAX_CHIP_COUNT] = { 1, 10, 100, 500};
		int nTmpAreaChance[AREA_COUNT] = 
		{3, 5, 5, 5, 5, 5, 5, 5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5};

		memcpy(nChip, nTmpChip, sizeof(nChip));
		memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));
	}
};

//记录信息
struct tagServerGameRecord
{
	BYTE							cbResult;						   //开中动物
};

//下注信息
//服务器命令结构
#define SUB_S_GAME_START			2002								//游戏开始
#define SUB_S_GAME_END				2004								//游戏结束
#define SUB_S_PLACE_JETTON			2005								//用户下注
#define SUB_S_PLACE_JETTON_FAIL		2006								//下注失败
#define SUB_S_CHEHUI				2007								//下注撤回
#define SUB_S_SEND_RECORD			2117								//游戏记录

#define SUB_S_ONLINE_PLAYER         2008    							//在线用户
#define SUB_S_ONLINE_PLAYER_ERROR	2015								//在线用户获取失败

#define SUB_S_ANDROID_CONFIG		216									//机器人配置

#define SUB_S_RESPONSE_CONTROL		2030								//超端信息响应

//超端控制信息请求
struct CMD_S_ControlInfo
{
	BYTE							cbJuControl;						//是否进行局控
	BYTE							cbJuControlTimes;					//局控次数
	BYTE							cbJuControlArea;					//本局控制哪一门胜利 0不控制，1控制赢，2控制输

	SCORE							lAreaJettonScore[AREA_COUNT];		//区域总注
};

//下注信息数组
struct tagCustomAndroid
{
	//坐庄
	BOOL							nEnableRobotBanker;				//是否做庄
	LONGLONG						lRobotBankerCountMin;			//坐庄次数
	LONGLONG						lRobotBankerCountMax;			//坐庄次数
	LONGLONG						lRobotListMinCount;				//列表人数
	LONGLONG						lRobotListMaxCount;				//列表人数
	LONGLONG						lRobotApplyBanker;				//最多申请个数
	LONGLONG						lRobotWaitBanker;				//空盘重申

	//下注
	LONGLONG						lRobotMinBetTime;				//下注筹码个数
	LONGLONG						lRobotMaxBetTime;				//下注筹码个数
	SCORE							lRobotMinJetton;				//下注筹码金额
	SCORE							lRobotMaxJetton;				//下注筹码金额
	LONGLONG						lRobotBetMinCount;				//下注机器人数
	LONGLONG						lRobotBetMaxCount;				//下注机器人数
	SCORE							lRobotAreaLimit;				//区域限制

	//存取款
	LONGLONG						lRobotScoreMin;					//金币下限
	LONGLONG						lRobotScoreMax;					//金币上限
	LONGLONG						lRobotBankGetMin;				//取款最小值(非庄)
	LONGLONG						lRobotBankGetMax;				//取款最大值(非庄)
	LONGLONG						lRobotBankGetBankerMin;			//取款最小值(坐庄)
	LONGLONG						lRobotBankGetBankerMax;			//取款最大值(坐庄)
	LONGLONG						lRobotBankStoMul;				//存款百分比

	//构造函数
	tagCustomAndroid()
	{
		DefaultCustomRule();
	}

	void DefaultCustomRule()
	{
		nEnableRobotBanker = TRUE;
		lRobotBankerCountMin = 5;
		lRobotBankerCountMax = 10;
		lRobotListMinCount = 2;
		lRobotListMaxCount = 5;
		lRobotApplyBanker = 5;
		lRobotWaitBanker = 3;

		lRobotMinBetTime = 6;
		lRobotMaxBetTime = 8;
		lRobotMinJetton = 100;
		lRobotMaxJetton = 5000000;
		lRobotBetMinCount = 4;
		lRobotBetMaxCount = 8;
		lRobotAreaLimit = 10000000;

		lRobotScoreMin = 1000000;
		lRobotScoreMax = 100000000;
		lRobotBankGetMin = 100;
		lRobotBankGetMax = 30000000;
		lRobotBankGetBankerMin = 10000000;
		lRobotBankGetBankerMax = 50000000;
		lRobotBankStoMul = 50;
	}
};

//游戏状态
struct CMD_S_StatusPlay
{
	SCORE							lChips[MAX_CHIP_COUNT];				//筹码

	SCORE							lTypeLimit[7];						//类型限制下注

	WORD							wSeatUser[MAX_SEAT_COUNT];			//3个椅子玩家的椅子号
	SCORE							lSeatUserAreaScore[MAX_SEAT_COUNT][AREA_COUNT];	//3个椅子玩家的区域下注信息

	BYTE							cbTimeLeave;						//剩下时间
	int								nPlayTimes;							//下注时间
	int								nEndTimes;							//结束时间

	BYTE							cbWinMen[40];						//游戏记录

	SCORE							lPlayerJettonScore[AREA_COUNT];		//玩家下的注
	SCORE							lAllJettonScore[AREA_COUNT];		//所有玩家下的注

	BYTE							cbResult;							//开奖结果
	SCORE							lPlayerWinScore;					//玩家赢分
	SCORE							lSeatUserWinScore[MAX_SEAT_COUNT];	//坐下的玩家输赢
	SCORE							lPlayerRestScore;					//玩家还剩多少钱
};

//游戏开始
struct CMD_S_GameStart
{
	BYTE							cbTimeLeave;						//剩余时间

	WORD							wSeatUser[MAX_SEAT_COUNT];			//3个椅子玩家的椅子号
};

//用户下注
struct CMD_S_PlaceJetton
{
	BYTE							cbJettonType;						//下注类型
	BYTE							cbJettonArea[18];					//筹码区域	
	SCORE							lJettonScore;						//加注数目
	WORD							wChairID;							//用户位置

	SCORE							lUserRestScore;						//玩家剩余金币

	int								nKeys;
};

//下注失败
struct CMD_S_PlaceJetton_Fail
{
	BYTE							cbJettonArea[18];					//失败区域
	TCHAR							szFailReason[64];					//失败原因
};

//用户撤回
struct CMD_S_CheHui
{
	WORD							wChairID;							//撤回的用户
	SCORE							lUserRestScore;						//用户剩余金币
};

//游戏结束
struct CMD_S_GameEnd
{

	SCORE							lPlayerJettonScore[AREA_COUNT];		//玩家下的注
	SCORE							lAllJettonScore[AREA_COUNT];		//所有玩家下的注
	SCORE							lSeatUserWinScore[MAX_SEAT_COUNT];	//坐下的玩家输赢
	SCORE							lPlayAreaScore[AREA_COUNT];			//玩家成绩
	SCORE							lPlayerWinScore;					//玩家赢的钱
	SCORE							lPlayerRestScore;					//玩家还剩多少钱				
	BYTE							cbResult;							//开奖结果
};

//玩家列表
struct CMD_S_UserList
{
	WORD							wCount;								//数组数量
	bool							bEnd;								//是否结束
	BYTE							cbIndex[USER_LIST_COUNT];			//排名
	TCHAR							szUserNick[USER_LIST_COUNT][32];	//昵称
	SCORE							lBetScore[USER_LIST_COUNT];			//近20局下注金额
	BYTE							cbWinTimes[USER_LIST_COUNT];		//近20局赢了多少局
	SCORE							lUserScore[USER_LIST_COUNT];		//玩家金币
	BYTE							wFaceID[USER_LIST_COUNT];			//玩家头像
	WORD							wChairID[USER_LIST_COUNT];
};

//玩家列表单个数据
struct CMD_S_UserListInfo
{
	WORD							wWinNum;							//获胜次数
	SCORE							lAllBet;							//下注分数
	SCORE							lUserScore;							//用户积分
	TCHAR							szNickName[32];						//用户昵称
	BYTE							wFaceID;							//玩家头像
	WORD							wChairID;
};
//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_PLACE_JETTON			101									//用户下注
#define SUB_C_XUTOU					102									//用户续投
#define SUB_C_CHEHUI				103									//撤回下注
#define SUB_C_ONLINE_UESER			104									//获取用户列表

#define SUB_C_REQUEST_CONTROL		110									//控制信息请求
#define SUB_C_CONTROL				111									//本局控制输赢
#define SUB_C_CANCEL_CONTROL		112									//取消本局控制

//用户下注
struct CMD_C_PlaceJetton
{
	BYTE							cbBetArea[18];						//筹码区域
	BYTE							cbBetType;
	SCORE							lBetScore;							//加注数目
	int								nBetKeys;							//发回给客户端的
};

//超端局控
struct CMD_C_ControlWinLost
{
	BYTE							cbControlTimes;				//控制次数
	BYTE							cbJuControlArea;			//本局控制哪门胜利 0不控制，1控制赢，2控制输
};
//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif
