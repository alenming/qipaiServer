#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

//1字节对其
#pragma pack(1)

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID						    104									//游戏 I D
#define GAME_NAME					TEXT("红黑大战")						//游戏名字

#define GAME_PLAYER					100									//游戏人数

//组件属性
#define VERSION_SERVER				PROCESS_VERSION(7,0,1)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//程序版本

//状态定义
#define GAME_SCENE_FREE				GAME_STATUS_FREE					//等待开始
#define GAME_SCENE_BET				GAME_STATUS_FREE					//下注状态
#define	GAME_SCENE_END				GAME_STATUS_PLAY					//结束状态

//玩家索引
#define AREA_XIAN					0									//黑
#define AREA_ZHUANG					1									//红
#define AREA_PING					2									//幸运一击
#define AREA_MAX					3									//最大区域

//区域倍数multiple
#define MULTIPLE_XIAN				2									//闲家倍数
#define MULTIPLE_PING				9									//平家倍数
#define MULTIPLE_ZHUANG				2									//庄家倍数

//占座
#define MAX_SEAT_COUNT		6									//最大占位个数
#define MAX_CHIP_COUNT				5									//最大筹码个数

#define USER_LIST_COUNT				20									//列表玩家相应数据条数

//赔率定义
#define RATE_TWO_PAIR				12									//对子赔率
#define SERVER_LEN					32									//房间长度



#ifndef _UNICODE
#define myprintf	_snprintf
#define mystrcpy	strcpy
#define mystrlen	strlen
#define myscanf		_snscanf
#define	myLPSTR		LPCSTR
#else
#define myprintf	swprintf
#define mystrcpy	wcscpy
#define mystrlen	wcslen
#define myscanf		_snwscanf
#define	myLPSTR		LPWSTR
#endif

struct tagServerGameRecord
{
	bool							bPlayer;						
	bool							bBanker;						
	BYTE							bPing;
	BYTE							cbCardType;  //1单张，2对子,3顺子,4金花,5顺金,6豹子
};
struct tagCustomAndroid
{
	int								nChip[8];
	//坐庄
	bool							nEnableRobotBanker;				//是否做庄
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
	
	//区域几率
	int								nAreaChance[AREA_MAX];		//区域几率
	//构造函数
	tagCustomAndroid()
	{
		DefaultCustomRule();
	}

	void DefaultCustomRule()
	{
		nEnableRobotBanker = true;
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

		int nTmpAreaChance[8] = {3, 1, 3, 1, 1, 1, 1, 2};
		memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));

	}
};

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_FREE				2002									//游戏空闲
#define SUB_S_GAME_START			2003									//游戏开始
#define SUB_S_PLACE_JETTON			2004									//用户下注
#define SUB_S_PLACE_JETTON_FAIL		2005									//下注失败
#define SUB_S_GAME_END				2006									//游戏结束
#define SUB_S_APPLY_BANKER			2007									//申请庄家
#define SUB_S_CHANGE_BANKER			2008									//切换庄家
#define SUB_S_CANCEL_BANKER			2009									//取消申请
#define SUB_S_APPLEY_BANKER_FAILURE	2011									//上庄失败
#define SUB_S_BET_INFO				2012									//用户下注信息
#define SUB_S_ONLINE_PLAYER			2014									//在线用户
#define SUB_S_ONLINE_PLAYER_ERROR	2015									//在线用户获取失败
#define SUB_S_CLEAR_DRAWLIST		2016									//清理路单

#define SUB_S_SEND_RECORD			2017									//游戏记录

#define SUB_S_OTHER_JETTON			2021									//其它玩家下注
#define SUB_S_SEAT_JETTON			2022									//占位玩家下注

#define SUB_S_ANDROID_CONFIG		2020									//机器人配置

#define SUB_S_RESPONSE_CONTROL		2030									//超端信息响应

//超端控制信息请求
struct CMD_S_ControlInfo
{
	BYTE							cbJuControl;							//是否进行局控
	BYTE							cbJuControlTimes;						//局控次数
	BYTE							cbJuControlArea[AREA_MAX];				//本局控制哪一门胜利 0不控制，1控制赢，2控制输

	SCORE							lAreaJettonScore[AREA_MAX];				//区域总注
	SCORE							lUserJettonScore[GAME_PLAYER][AREA_MAX];	//个人总注
	TCHAR							szNickName[GAME_PLAYER][32];			//玩家昵称
};

//申请庄家
struct CMD_S_ApplyBanker
{
	WORD							wApplyUser;							//申请玩家
};

//申请上庄失败
struct CMD_S_ApplyFailed
{
	TCHAR							szBuffer[64];						//描述信息
};

//取消申请
struct CMD_S_CancelBanker
{
	WORD							wCancelUser;						//取消玩家
};

//切换庄家
struct CMD_S_ChangeBanker
{
	WORD							wBankerUser;						//当庄玩家
	SCORE							lBankerScore;						//庄家分数
};

//游戏状态
struct CMD_S_StatusFree
{
	//全局信息
	BYTE							cbTimeLeave;						//剩余时间

	int								nChip[MAX_CHIP_COUNT];				//筹码配置

	WORD							wApplyUser[GAME_PLAYER];			//上庄列表玩家

	WORD							wSeatUser[MAX_SEAT_COUNT];	//6个椅子玩家的椅子号

	WORD							wBankerUser;						//当前庄家的GameID
	SCORE							lBankerScore;						//庄家分数
	SCORE							lAreaLimitScore;					//区域限制
	SCORE							lApplyBankerCondition;				//申请条件

	BYTE							cbFreeTime;							//空闲时间
	BYTE							cbPlayTime;							//游戏时间
	BYTE							cbEndTime;							//结束时间

	SCORE							lMinXianHong;
	SCORE							lMaxXianHong;
};

//游戏状态
struct CMD_S_StatusPlay
{
	//全局信息
	BYTE							cbTimeLeave;						//剩余时间

	int								nChip[MAX_CHIP_COUNT];				//筹码配置

	WORD							wApplyUser[GAME_PLAYER];			//上庄列表玩家的GameID

	WORD							wSeatUser[MAX_SEAT_COUNT];	//6个椅子玩家的椅子号

	SCORE							lSeatUserAreaScore[MAX_SEAT_COUNT][AREA_MAX];	//6个椅子玩家的区域下注信息

	WORD							wBankerUser;						//当前庄家的GameID
	SCORE							lBankerScore;						//庄家分数
	SCORE							lAreaLimitScore;					//区域限制
	SCORE							lApplyBankerCondition;				//申请条件

	BYTE							cbFreeTime;							//空闲时间
	BYTE							cbPlayTime;							//游戏时间
	BYTE							cbEndTime;							//结束时间

	SCORE							lMinXianHong;
	SCORE							lMaxXianHong;

	SCORE							lPlayerJettonScore[AREA_MAX];		//玩家下的注
	SCORE							lAllJettonScore[AREA_MAX];			//所有玩家下的注

	BYTE							cbTableCardArray[2][3];				//桌面扑克

	BYTE							cbResult[AREA_MAX+1];					//结果  0:龙 1虎 2和

	SCORE							lBankerWinScore;					//庄家赢分
	SCORE							lPlayerWinScore;					//玩家赢分
	SCORE							lSeatUserWinScore[MAX_SEAT_COUNT];	//坐下的玩家输赢
	SCORE							lPlayerRestScore;					//玩家还剩多少钱
	SCORE							lBankerRestScore;					//庄家还剩多少钱
};

//游戏开始
struct CMD_S_GameStart
{
	BYTE							cbTimeLeave;						//剩余时间

	WORD							wSeatUser[MAX_SEAT_COUNT];	//6个椅子玩家的椅子号
};

//用户下注
struct CMD_S_PlaceBet
{
	WORD							wChairID;							//用户位置
	BYTE							cbBetArea;							//筹码区域
	SCORE							lBetScore;							//加注数目

	SCORE							lPlayerRestScore;					//下注玩家剩余金币
};

//失败结构
struct CMD_S_PlaceBetFail
{
	TCHAR							szBuffer[64];						//描述信息
};

//游戏结束
struct CMD_S_GameEnd
{
	SCORE							lPlayAreaScore[AREA_MAX];			//玩家成绩
	SCORE							lPlayerWinScore;					//玩家赢的钱
	SCORE							lPlayerRestScore;					//玩家还剩多少钱
	SCORE							lBankerWinScore;					//庄家成绩
	SCORE							lBankerRestScore;					//庄家还剩多少钱

	SCORE							lSeatUserWinScore[MAX_SEAT_COUNT];	//坐下的玩家输赢
	SCORE							lPlayerJettonScore[AREA_MAX];		//玩家下的注
	SCORE							lAllJettonScore[AREA_MAX];			//所有玩家下的注

	BYTE							cbTableCardArray[2][3];				//桌面扑克
	BYTE							cbResult[AREA_MAX+1];				//结果
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
#define SUB_C_PLACE_JETTON			100									//用户下注
#define SUB_C_APPLY_BANKER			101									//申请庄家
#define SUB_C_CANCEL_BANKER			102									//取消申请
#define SUB_C_ONLINE_PLAYER			103									//在线用户

#define SUB_C_REQUEST_CONTROL		110									//控制信息请求
#define SUB_C_CONTROL				111									//本局控制输赢
#define SUB_C_CANCEL_CONTROL		112									//取消本局控制

//用户下注
struct CMD_C_PlaceBet
{
	
	BYTE							cbBetArea;							//筹码区域
	SCORE							lBetScore;							//加注数目
};

//超端局控
struct CMD_C_ControlWinLost
{
	BYTE							cbControlTimes;						//控制次数
	BYTE							cbJuControlArea[AREA_MAX];			//本局控制哪门胜利 0不控制，1控制赢，2控制输
};

//还原对其数
#pragma pack()
//////////////////////////////////////////////////////////////////////////

#endif
