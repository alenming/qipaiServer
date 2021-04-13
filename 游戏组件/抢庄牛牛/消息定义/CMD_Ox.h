#ifndef CMD_OXSIXX_HEAD_FILE
#define CMD_OXSIXX_HEAD_FILE

#pragma pack(push)  
#pragma pack(1)

#define		CARD_CONFIG

//////////////////////////////////////////////////////////////////////////
//公共宏定义
#define KIND_ID							103									//游戏 I D
#define GAME_PLAYER						5									//游戏人数
#define GAME_NAME						TEXT("抢庄牛牛")					//游戏名字
#define MAX_CARDCOUNT					5									//最大数目
#define MAX_CONFIG						4									//最大配置个数


#define VERSION_SERVER					PROCESS_VERSION(7,0,1)				//程序版本
#define VERSION_CLIENT					PROCESS_VERSION(7,0,1)				//程序版本

//游戏状态
#define GS_TK_FREE						GAME_STATUS_FREE					//空闲状态
#define GS_TK_WAIT						GAME_STATUS_PLAY					//等待开始
#define GS_TK_CALL						GAME_STATUS_PLAY+1					//叫庄状态
#define GS_TK_SCORE						GAME_STATUS_PLAY+2					//下注状态
#define GS_TK_END						GAME_STATUS_PLAY+3					//游戏结束

#define SEND_COUNT						3									//提前发牌数量

//////////////////////////////////////////////////////////////////////////
//服务器命令结构
#define SUB_S_WAIT_START				2001								//等待开始
#define SUB_S_GAME_START				2002								//游戏开始
#define SUB_S_DISPATCH_CARD				2003								//派发扑克
#define SUB_S_CALL_BANKER				2004								//叫庄
#define SUB_S_SET_BANKER				2005								//设置庄家
#define SUB_S_PLAYER_BET				2006								//下注
#define SUB_S_GAME_END					2007								//游戏结算

#define SUB_S_CUSTOM_ANDROID			2010								//机器人配置
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661////

//预留机器人存款取款
struct tagCustomAndroid
{
	SCORE								lRobotScoreMin;	
	SCORE								lRobotScoreMax;
	SCORE	                            lRobotBankGet; 
	SCORE								lRobotBankGetBanker; 
	SCORE								lRobotBankStoMul; 
	LONGLONG							lAddChip[4];							//下注倍数
};
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661////

//游戏状态
struct CMD_S_StatusFree
{
	SCORE								lCellScore;							//基础积分
	BYTE								cbPlayerStatus[GAME_PLAYER];		//玩家状态

	BYTE								cbStartTime;						//开始时间
	BYTE								cbCallTime;							//叫庄时间
	BYTE								cbAddTime;							//下注时间
	BYTE								cbEndTime;							//结束时间

	BYTE								cbTimeLeave;						//剩余时间
};

//叫庄状态
struct CMD_S_StatusCall
{
	SCORE								lCellScore;							//基础积分
	BYTE								cbPlayerStatus[GAME_PLAYER];		//玩家状态

	BYTE								cbStartTime;						//开始时间
	BYTE								cbCallTime;							//叫庄时间
	BYTE								cbAddTime;							//下注时间
	BYTE								cbEndTime;							//结束时间

	BYTE								cbTimeLeave;						//剩余时间

	BYTE								cbCallBankerTimes[GAME_PLAYER];		//叫庄倍数
	BYTE								cbCardData[GAME_PLAYER][MAX_CARDCOUNT];	//用户扑克
};

//下注状态
struct CMD_S_StatusScore
{
	SCORE								lCellScore;							//基础积分
	BYTE								cbPlayerStatus[GAME_PLAYER];		//玩家状态

	BYTE								cbStartTime;						//开始时间
	BYTE								cbCallTime;							//叫庄时间
	BYTE								cbAddTime;							//下注时间
	BYTE								cbEndTime;							//结束时间

	BYTE								cbTimeLeave;						//剩余时间

	WORD								wBankerUser;						//庄家用户
	BYTE								cbBankerTimes;						//庄家倍数

	LONGLONG							lBetTimes[GAME_PLAYER];				//下注倍数

	BYTE								cbCardData[GAME_PLAYER][MAX_CARDCOUNT];	//用户扑克
};

//游戏开始
struct CMD_S_GameStart
{
	BYTE								cbTime;								//时间
};

//发送扑克
struct CMD_S_SendCard
{
	BYTE								cbPlayerStatus[GAME_PLAYER];		//玩家状态
	//(发牌模式为发三等五，发三张牌， 其它全为0)
	BYTE								cbCardData[GAME_PLAYER][MAX_CARDCOUNT];	//用户扑克
};

//用户叫庄信息
struct CMD_S_CallBankerInfo
{
	WORD								wCallUser;							//叫庄用户
	BYTE								cbCallBankerTimes;					//叫庄倍数
};

//确定庄家
struct CMD_S_SetBanker
{
	WORD								wBankerUser;						//庄家用户
	WORD								wCallList[GAME_PLAYER];				//叫这一个倍数的用户列表
};

//用户下注
struct CMD_S_AddScore
{
	WORD								wBetUser;							//加注用户
	int									nBetTimes;							//加注数目
};

//游戏结束
struct CMD_S_GameEnd
{
	BYTE								cbFleeEnd;							//因为用户逃跑引起的结束
	BYTE								cbHandCardData[GAME_PLAYER][MAX_CARDCOUNT];//桌面扑克
	BYTE								cbCardType[GAME_PLAYER];			//玩家牌型

	SCORE								lPlayerScore[GAME_PLAYER];			//用户结算完剩多少钱
	SCORE								lGameScore[GAME_PLAYER];			//游戏得分
};
//////////////////////////////////////////////////////////////////////////
//客户端命令结构
#define SUB_C_CALL_BANKER				1									//用户叫庄
#define SUB_C_ADD_SCORE					2									//用户加注

//用户叫庄
struct CMD_C_CallBanker
{
	BYTE								cbBankerTimes;						//叫庄倍数(若用户不叫庄，赋值0)
};

//用户加注
struct CMD_C_AddScore
{
	int									nScore;								//加注数目
};
#pragma pack(pop)

#endif
