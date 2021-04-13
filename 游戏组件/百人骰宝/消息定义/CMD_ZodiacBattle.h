#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//公共宏定义
#define KIND_ID						127									//游戏 I D
#define GAME_PLAYER					100									//游戏人数
#define GAME_NAME					TEXT("百人骰宝")					//游戏名字

//状态定义
#define GS_FREE						GAME_STATUS_FREE					//空闲状态
#define	GS_PLACE_JETTON				GAME_STATUS_PLAY					//下注状态
#define	GS_GAME_END					GAME_STATUS_PLAY+1					//结束状态

#define VERSION_SERVER			    PROCESS_VERSION(7,0,1)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//程序版本


#define BET_AREA_XIAO				0									//押小1 : 1 （4 - 10）
#define BET_AREA_TWO1				1									//双骰1 : 10 两个1点
#define BET_AREA_TWO2				2									//双骰1 : 10 两个2点
#define BET_AREA_TWO3				3									//双骰1 : 10 两个3点
#define BET_AREA_WEI1				4									//围骰1 : 200 三个1点
#define BET_AREA_WEI2				5									//围骰1 : 200 三个2点
#define BET_AREA_WEI3				6									//围骰1 : 200 三个3点
#define BET_AREA_QUAN				7									//全围1 : 30 （1, 3, 5）点 （2, 4, 6）点
#define BET_AREA_WEI4				8									//围骰1 : 200 三个4点
#define BET_AREA_WEI5				9									//围骰1 : 200 三个5点
#define BET_AREA_WEI6				10									//围骰1 : 200 三个6点
#define BET_AREA_TWO4				11									//双骰1 : 10 两个4点
#define BET_AREA_TWO5				12									//双骰1 : 10 两个5点
#define BET_AREA_TWO6				13									//双骰1 : 10 两个6点
#define BET_AREA_DA                 14									//大1 : 1 （11 - 17）
#define BET_AREA_4					15									//4点 1 : 60
#define BET_AREA_5					16									//5点 1 : 30
#define BET_AREA_6					17									//6点 1 : 20
#define BET_AREA_7					18									//7点 1 : 12
#define BET_AREA_8					19									//8点 1 : 8
#define BET_AREA_9					20									//9点 1 : 6
#define BET_AREA_10                 21									//10点 1 : 6
#define BET_AREA_11					22									//11点 1 : 6
#define BET_AREA_12					23									//12点 1 : 6
#define BET_AREA_13					24									//13点 1 : 8
#define BET_AREA_14					25									//14点 1 : 12
#define BET_AREA_15					26									//15点 1 : 20
#define BET_AREA_16					27									//16点 1 : 30
#define BET_AREA_17					28									//17点 1 : 60
#define BET_AREA_DAN1				29									//单点数 1 中1个1倍 2个2倍 3个3倍
#define BET_AREA_DAN2				30									//单点数 2
#define BET_AREA_DAN3				31									//单点数 3
#define BET_AREA_DAN4				32									//单点数 4
#define BET_AREA_DAN5				33									//单点数 5
#define BET_AREA_DAN6				34									//单点数 6

#define AREA_COUNT					35									//区域数目


//历史记录
#define MAX_SCORE_HISTORY			40									//历史个数

#define USER_LIST_COUNT				20									//玩家列表一次发送数量

#define MAX_CHIP_COUNT				5									//筹码个数

#define MAX_TYPE_COUNT				11									//类型个数

//机器人信息
struct tagRobotInfo
{
	int nChip[5];														//筹码定义
	TCHAR szCfgFileName[MAX_PATH];										//配置文件
	int	nMaxTime;														//最大赔率

	tagRobotInfo()
	{
		int nTmpChip[5] = {1,10,50,100,500};
		TCHAR szTmpCfgFileName[MAX_PATH] = _T("LuxuryCarConfig.ini");

		nMaxTime = 1;
		memcpy(nChip, nTmpChip, sizeof(nChip));
		memcpy(szCfgFileName, szTmpCfgFileName, sizeof(szCfgFileName));
	}
};

//记录信息
struct tagServerGameRecord
{
	BYTE							cbResult[3];						//骰子点数
	BYTE							cbIsBig;							//是否为大  0小 1 大  2豹子
};

//下注信息
//服务器命令结构
#define SUB_S_GAME_READY			2002								//准备开始
#define SUB_S_GAME_START			2003								//游戏开始
#define SUB_S_GAME_END				2004								//游戏结束


#define SUB_S_PLACE_JETTON			2005								//用户下注
#define SUB_S_ONLINE_PLAYER         2008    							//在线用户
#define SUB_S_USER_CONTINUE_BETS	2009								//用户续投


#define SUB_S_ANDROID_CONFIG		216									//机器人配置

#define SUB_S_RESPONSE_CONTROL		2030								//超端信息响应

//超端控制信息请求
struct CMD_S_ControlInfo
{
	BYTE							cbJuControl;						//是否进行局控
	BYTE							cbJuControlTimes;					//局控次数
	BYTE							cbJuControlArea[3];					//本局控制哪一门胜利 0不控制，1控制赢，2控制输

	SCORE							lAreaJettonScore[AREA_COUNT];			//区域总注
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
	SCORE							lRobotScoreMin;					//金币下限
	SCORE							lRobotScoreMax;					//金币上限
	SCORE							lRobotBankGetMin;				//取款最小值(非庄)
	SCORE							lRobotBankGetMax;				//取款最大值(非庄)
	SCORE							lRobotBankGetBankerMin;			//取款最小值(坐庄)
	SCORE							lRobotBankGetBankerMax;			//取款最大值(坐庄)
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
struct CMD_S_StatusFree
{
	SCORE							lChips[MAX_CHIP_COUNT];				//筹码
	SCORE							nTypeLimit[MAX_TYPE_COUNT];			//类型限制

	BYTE							cbTimeLeave;						//剩下时间

	int								nFreeTimes;							//空闲时间
	int								nPlayTimes;							//下注时间
	int								nEndTimes;							//结束时间

	tagServerGameRecord				pGameRecord[MAX_SCORE_HISTORY];		//游戏记录
};

//游戏状态
struct CMD_S_StatusPlay
{
	SCORE							lChips[MAX_CHIP_COUNT];				//筹码
	SCORE							nTypeLimit[MAX_TYPE_COUNT];			//类型限制

	BYTE							cbTimeLeave;						//剩下时间

	int								nFreeTimes;							//空闲时间
	int								nPlayTimes;							//下注时间
	int								nEndTimes;							//结束时间

	SCORE							lPlayerJettonScore[AREA_COUNT];		//玩家下的注
	SCORE							lAllJettonScore[AREA_COUNT];		//所有玩家下的注

	BYTE							cbResult[3];						//开奖结果
	BYTE							cbResultType;						//结果类型
	SCORE							lPlayerWinScore;					//玩家赢分
	SCORE							lPlayerRestScore;					//玩家还剩多少钱

	tagServerGameRecord				pGameRecord[MAX_SCORE_HISTORY];		//游戏记录
};

//用户下注
struct CMD_S_PlaceJetton
{
	BYTE							cbResultCode;						//结果	1-6
	TCHAR							cbFailReson[64];					//失败原因
	BYTE							cbJettonType;						//下注类型	
	SCORE							lJettonScore;						//加注数目
	WORD							wAddUser;							//用户位置

	SCORE							lUserRestScore;						//玩家剩余金币
};

//游戏结束
struct CMD_S_GameEnd
{
	BYTE							cbWinNum;							//骰子总点数
	BYTE							cbSiceNum[3];						//骰子点数
	BYTE							cbWinArea[AREA_COUNT];
	SCORE							lAllAreaScore[AREA_COUNT];			//总的区域下注
	SCORE							lPlayerAreaScore[AREA_COUNT];		//玩家区域下注
	SCORE							lUserWinScore;						//个人输赢
	SCORE							lPlayerRestScore;					//玩家剩余金币					
};

//用户准备
struct CMD_S_GameReady
{
	SCORE							lPlayerAllScore;					//用户上一局总注
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
#define SUB_C_CLEAN_JETTON			102									//清零命令
#define SUB_C_CONTINUE_JETTON		103									//继续下注
#define SUB_C_ONLINE_PLAYER			104									//获取玩家列表

#define SUB_C_REQUEST_CONTROL		110									//控制信息请求
#define SUB_C_CONTROL				111									//本局控制输赢
#define SUB_C_CANCEL_CONTROL		112									//取消本局控制

//用户下注
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonType;						//筹码区域
	SCORE							lJettonScore;						//加注数目
};

//超端局控
struct CMD_C_ControlWinLost
{
	BYTE							cbControlTimes;						//控制次数
	BYTE							cbJuControlArea[3];					//本局控制哪门胜利 0不控制，1控制赢，2控制输
};
//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif
