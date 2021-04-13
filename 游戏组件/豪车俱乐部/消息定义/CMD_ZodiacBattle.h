#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//公共宏定义
#define KIND_ID						111									//游戏 I D
#define GAME_PLAYER					100							//游戏人数
#define GAME_NAME					TEXT("豪车俱乐部")					//游戏名字

//状态定义
#define	GS_PLACE_JETTON				GAME_STATUS_PLAY					//下注状态
#define	GS_GAME_END					GAME_STATUS_PLAY+1					//结束状态

#define VERSION_SERVER			    PROCESS_VERSION(7,0,1)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//程序版本


//区域索引
#define ID_S_DZ						1									//小大众	
#define ID_S_AD						2									//小奥迪		
#define ID_S_BM						3									//小宝马		
#define ID_S_BC						4									//小奔驰		
#define ID_B_DZ						5									//大大众
#define ID_B_AD						6									//大奥迪
#define ID_B_BM						7									//大宝马
#define ID_B_BC						8									//大奔驰
		
#define AREA_S_DZ					0									//小大众
#define AREA_S_AD					1									//小奥迪
#define AREA_S_BM					2									//小宝马
#define AREA_S_BC					3									//小奔驰
#define AREA_B_DZ					4									//大大众
#define AREA_B_AD					5									//大奥迪
#define AREA_B_BM					6									//大宝马
#define AREA_B_BC					7									//大奔驰

#define AREA_COUNT					8									//区域数目

#define AREA_ALL					8									//动物种类

#define ANIMAL_COUNT                32                                  //动物个数
//赔率定义
#define RATE_TWO_PAIR				12									//对子赔率

#define SERVER_LEN					32									//房间长度

//历史记录
#define MAX_SCORE_HISTORY			50									//历史个数

#define USER_LIST_COUNT				20									//玩家列表一次发送数量

//机器人信息
struct tagRobotInfo
{
	int nChip[5];														//筹码定义
	int nAreaChance[AREA_COUNT];										//区域几率
	TCHAR szCfgFileName[MAX_PATH];										//配置文件
	int	nMaxTime;														//最大赔率

	tagRobotInfo()
	{
		int nTmpChip[5] = {1,10,100,500,1000};
		int nTmpAreaChance[AREA_COUNT] = {3, 3, 3, 3, 10, 10, 10, 10};
		TCHAR szTmpCfgFileName[MAX_PATH] = _T("LuxuryCarConfig.ini");

		nMaxTime = 1;
		memcpy(nChip, nTmpChip, sizeof(nChip));
		memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));
		memcpy(szCfgFileName, szTmpCfgFileName, sizeof(szCfgFileName));
	}
};

//记录信息
struct tagServerGameRecord
{
	BYTE							cbAnimal;						   //开中动物
};

//下注信息
//服务器命令结构

#define SUB_S_GAME_FREE				299									//游戏空闲
#define SUB_S_GAME_START			300									//游戏开始
#define SUB_S_PLACE_JETTON			301									//用户下注
#define SUB_S_GAME_END				302									//游戏结束
#define SUB_S_APPLY_BANKER			303									//申请庄家
#define SUB_S_CHANGE_BANKER			304									//切换庄家
#define SUB_S_CHANGE_USER_SCORE		305									//更新积分
#define SUB_S_SEND_RECORD			306									//游戏记录
#define SUB_S_PLACE_JETTON_FAIL		307									//下注失败
#define SUB_S_CANCEL_BANKER			308									//取消申请
#define	SUB_S_CEAN_JETTON			309									//清楚下注
#define SUB_S_CONTINU_JETTON		310									//继续下注

#define SUB_S_ADMIN_COMMDN_EXT		318									//系统控制
#define SUB_S_UPDATE_STORAGE_EXT    319									//更新库存
#define SUB_S_ONLINE_PLAYER         214    								//在线用户
#define SUB_S_ONLINE_PLAYER_ERROR	215									//在线用户获取失败

#define SUB_S_ANDROID_CONFIG		216									//机器人配置

#define SUB_S_OTHER_JETTON			2021								//其它玩家下注

#define SUB_S_RESPONSE_CONTROL		2030								//超端信息响应

//超端控制信息请求
struct CMD_S_ControlInfo
{
	BYTE							cbJuControl;						//是否进行局控
	BYTE							cbJuControlTimes;					//局控次数
	BYTE							cbJuControlArea;					//本局控制哪一门胜利 0不控制，1控制赢，2控制输

	SCORE							lAreaJettonScore[AREA_COUNT];			//区域总注
	SCORE							lUserJettonScore[GAME_PLAYER][AREA_COUNT];	//个人总注
	TCHAR							szNickName[GAME_PLAYER][32];		//玩家昵称
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

//申请庄家
struct CMD_S_ApplyBanker
{
	BYTE							cbResult;							//结果

	int								dwGameID;							//申请玩家
	TCHAR							szNickName[32];						//申请玩家昵称
	int								nApplyCount;						//上庄列表大小
};

//取消申请
struct CMD_S_CancelBanker
{
	int								dwGameID;							//取消玩家
};

//切换庄家
struct CMD_S_ChangeBanker
{
	int								dwGameID;							//当庄玩家
	SCORE							lBankerScore;						//庄家金币
	TCHAR							szNickName[32];						//申请玩家昵称
	BYTE							cbBankerHeadInfo;					//头像ID
};

//游戏状态
struct CMD_S_StatusFree
{
	SCORE							lChips[5];							//筹码
	int								nMultiple[AREA_COUNT];				//倍数

	SCORE							lApplyBankerCondition;				//申请条件
	int								wBankerUserGameID;					//当前庄家
	SCORE							lBankerScore;						//庄家分数
	TCHAR							szBankerNick[32];					//庄家昵称
	BYTE							cbBankerHeadInfo;					//头像ID
	int								nApplyCounts;						//上庄列表人数

	BYTE							cbTimeLeave;						//剩下时间

	int								nFreeTimes;							//空闲时间
	int								nPlayTimes;							//下注时间
	int								nEndTimes;							//结束时间
	int								nAddTimes;							//附加时间

	SCORE							lMinXianHong;
	SCORE							lMaxXianHong;

	SCORE							lUserRestScore;						//用户剩余金币

	BYTE							cbWinMen[20];		//游戏记录

	SCORE							lPlayerJettonScore[AREA_COUNT];		//玩家下的注
	SCORE							lAllJettonScore[AREA_COUNT];		//所有玩家下的注
};

//游戏状态
struct CMD_S_StatusPlay
{
	SCORE							lChips[5];							//筹码
	int								nMultiple[AREA_COUNT];				//倍数

	SCORE							lApplyBankerCondition;				//申请条件
	int								wBankerUserGameID;					//当前庄家
	SCORE							lBankerScore;						//庄家分数
	TCHAR							szBankerNick[32];					//庄家昵称
	BYTE							cbBankerHeadInfo;					//头像ID
	int								nApplyCounts;						//上庄列表人数

	BYTE							cbTimeLeave;						//剩下时间

	int								nFreeTimes;							//空闲时间
	int								nPlayTimes;							//下注时间
	int								nEndTimes;							//结束时间
	int								nAddTimes;							//附加时间

	SCORE							lMinXianHong;
	SCORE							lMaxXianHong;

	SCORE							lUserRestScore;						//用户剩余金币

	BYTE							cbWinMen[20];					//游戏记录

	SCORE							lPlayerJettonScore[AREA_COUNT];		//玩家下的注
	SCORE							lAllJettonScore[AREA_COUNT];		//所有玩家下的注
};



//用户下注
struct CMD_S_PlaceJetton
{
	BYTE							cbResult;							//结果	1-6

	BYTE							cbJettonArea;						//筹码区域	
	SCORE							lJettonScore;						//加注数目
	int								wUserGameID;						//用户位置

	SCORE							lUserRestScore;						//玩家剩余金币

	SCORE							lPlayerJettonScore[AREA_COUNT];		//玩家下的注
};

//游戏结束
struct CMD_S_GameEnd
{
	SCORE							lAllAreaScore[AREA_COUNT];			//总的区域下注
	SCORE							lUserWinScore;						//个人输赢
	SCORE							lBankerWinScore;					//庄家输赢
	SCORE							lBankerRestScore;					//庄家剩余金币
	SCORE							lPlayerRestScore;					//玩家剩余金币					
	BYTE							cbTarget;							//结果动物
	BYTE							cbTurnTarget;						//目标区域
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
};

//玩家列表单个数据
struct CMD_S_UserListInfo
{
	WORD							wWinNum;							//获胜次数
	SCORE							lAllBet;							//下注分数
	SCORE							lUserScore;							//用户积分
	TCHAR							szNickName[32];						//用户昵称
	BYTE							wFaceID;							//玩家头像
};
//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_PLACE_JETTON			201									//用户下注
#define SUB_C_CLEAN_JETTON			205									//清零命令
#define SUB_C_CONTINUE_JETTON		206									//继续下注
#define SUB_C_ONLINE_PLAYER			103									//获取玩家列表
#define SUB_C_APPLY_BANKER			12									//申请庄家
#define SUB_C_CANCEL_BANKER			13									//取消申请

#define SUB_C_REQUEST_CONTROL		110									//控制信息请求
#define SUB_C_CONTROL				111									//本局控制输赢
#define SUB_C_CANCEL_CONTROL		112									//取消本局控制

//用户下注
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonArea;						//筹码区域
	SCORE							lJettonScore;						//加注数目
};

struct CMD_C_CleanMeJetto
{
	WORD							wChairID;							//用户位置
};

struct CMD_C_ContinueJetton
{
	WORD					wChairID;							//用户位置
	SCORE					lLastAllJettonPlace[AREA_COUNT + 1];	//全部筹码
	SCORE					lLastAllJettonArea[AREA_COUNT + 1];	//区域筹码

	BYTE					cbJettonArea;						//筹码区域
	SCORE					lJettonScore;						//加注数目
	BYTE					cbAndroid;							//机器人
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
