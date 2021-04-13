#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

#pragma pack(1)

#define UR_GAME_CONTROL					0x20000000L				//游戏特殊控制
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//服务定义

//游戏属性
#define KIND_ID						102									//游戏 I D
#define GAME_NAME					TEXT("百人牛牛")					//游戏名字

//组件属性
#define GAME_PLAYER					100									//游戏人数
#define VERSION_SERVER				PROCESS_VERSION(7,0,1)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//程序版本

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//状态定义

#define GAME_SCENE_FREE				GAME_STATUS_FREE					//等待开始
#define GAME_SCENE_BET				GAME_STATUS_PLAY					//下注状态
#define GAME_SCENE_END				GAME_STATUS_PLAY+1					//结束状态


//区域索引
#define ID_TIAN_MEN					1									//天
#define ID_DI_MEN					2									//地
#define ID_XUAN_MEN					3									//玄
#define ID_HUANG_MEN				4									//黄

//玩家索引
#define BANKER_INDEX				0									//庄家索引
#define SHUN_MEN_INDEX				1									//顺门索引
#define DUI_MEN_INDEX				2									//对门索引
#define DAO_MEN_INDEX				3									//倒门索引
#define HUAN_MEN_INDEX				4									//倒门索引
#define MAX_INDEX					3									//最大索引

#define AREA_MAX					4									//区域数目

#define MAX_SEAT_COUNT				6									//最大占位个数
#define MAX_CHIP_COUNT				5									//最大筹码个数


#define USER_LIST_COUNT				20									//列表玩家相应数据条数

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661//
//宏定义
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

/////////////////////////////////////////

struct tagCustomAndroid
{
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
	}
};

//机器人信息
struct tagRobotInfo
{
	int nChip[5];														//筹码定义
	int nAreaChance[AREA_MAX];											//区域几率
	TCHAR szCfgFileName[MAX_PATH];										//配置文件
	int	nMaxTime;														//最大赔率

	tagRobotInfo()
	{
		int nTmpChip[5] = {1, 10, 50, 100, 500};
		int nTmpAreaChance[AREA_MAX] = { 1, 1, 1, 1 };

		nMaxTime = 10;
		memcpy(nChip, nTmpChip, sizeof(nChip));
		memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));
	}
};

//记录信息
struct tagServerGameRecord
{
	BYTE							bWinShunMen;						//顺门胜利
	BYTE							bWinDuiMen;							//对门胜利
	BYTE							bWinDaoMen;							//倒门胜利
	BYTE							bWinHuang;							//黄门胜利
};

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_FREE				2002								//游戏空闲
#define SUB_S_GAME_START			2003								//游戏开始
#define SUB_S_PLACE_JETTON			2004								//用户下注
#define SUB_S_GAME_END				2005								//游戏结束
#define SUB_S_APPLY_BANKER			2006								//申请庄家
#define SUB_S_CHANGE_BANKER			2007								//切换庄家
#define SUB_S_SEND_RECORD			2008								//游戏记录
#define SUB_S_PLACE_JETTON_FAIL		2009								//下注失败
#define SUB_S_CANCEL_BANKER			2010								//取消申请
#define SUB_S_APPLEY_BANKER_FAILURE	2011								//上庄失败
#define SUB_S_CANCEL_FAILURE		2012								//取消失败
#define SUB_S_ONLINE_PLAYER			2014								//在线用户

#define SUB_S_OTHER_JETTON			2021								//其它玩家下注
#define SUB_S_SEAT_JETTON			2022								//占位玩家下注

#define SUB_S_ANDROID_CONFIG		2020								//机器人配置

#define SUB_S_RESPONSE_CONTROL		2030									//超端信息响应

//超端控制信息请求
struct CMD_S_ControlInfo
{
	BYTE							cbJuControl;							//是否进行局控
	BYTE							cbJuControlTimes;						//局控次数
	BYTE							cbJuControlArea[5];						//本局控制哪一门胜利 0不控制，1控制赢，2控制输

	SCORE							lAreaJettonScore[AREA_MAX+1];				//区域总注
	SCORE							lUserJettonScore[GAME_PLAYER][AREA_MAX+1];	//个人总注
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
	WORD							wCancelUser;					//取消玩家
};

//切换庄家
struct CMD_S_ChangeBanker
{
	WORD							wPreviousBanker;					//上一个当庄玩家
	WORD							wBankerUser;						//当庄玩家
	SCORE							lBankerScore;						//庄家金币
};

//游戏状态
struct CMD_S_StatusFree
{
	BYTE							cbHighTimes;						//是否是高倍场
	//全局信息
	BYTE							cbTimeLeave;						//剩余时间

	int								nChip[MAX_CHIP_COUNT];				//筹码配置

	WORD							wApplyUser[GAME_PLAYER];			//上庄列表玩家

	WORD							wSeatUser[MAX_SEAT_COUNT];			//6个椅子玩家的椅子号

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
	BYTE							cbHighTimes;							//是否是高倍场
	//全局信息
	BYTE							cbTimeLeave;						//剩余时间

	int								nChip[MAX_CHIP_COUNT];				//筹码配置

	WORD							wApplyUser[GAME_PLAYER];			//上庄列表玩家的GameID

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

	SCORE							lPlayerJettonScore[AREA_MAX+1];		//玩家下的注
	SCORE							lAllJettonScore[AREA_MAX+1];		//所有玩家下的注

	BYTE							cbTableCardArray[5][5];				//桌面扑克
	BYTE							cbCardType[AREA_MAX+1];
	BYTE							cbResult[AREA_MAX];					//结果  

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

	WORD							wSeatUser[MAX_SEAT_COUNT];			//6个椅子玩家的椅子号
};

//用户下注
struct CMD_S_PlaceJetton
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
	BYTE							cbEarlyOpenFlag;					//提前开牌标识


	SCORE							lPlayAreaScore[AREA_MAX];			//玩家成绩
	SCORE							lPlayerWinScore;					//玩家赢的钱
	SCORE							lPlayerRestScore;					//玩家还剩多少钱
	SCORE							lBankerWinScore;					//庄家成绩
	SCORE							lBankerRestScore;					//庄家还剩多少钱

	SCORE							lSeatUserWinScore[MAX_SEAT_COUNT];	//坐下的玩家输赢

	BYTE							cbTableCardArray[5][5];				//桌面扑克
	BYTE							cbCardType[AREA_MAX+1];
	BYTE							cbResult[AREA_MAX];					//结果 
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
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonArea;						//筹码区域
	SCORE							lJettonScore;						//加注数目
};

//超端局控
struct CMD_C_ControlWinLost
{
	BYTE							cbControlTimes;						//控制次数
	BYTE							cbJuControlArea[5];					//本局控制哪门胜利 0不控制，1控制赢，2控制输
};
//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif