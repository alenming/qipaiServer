#ifndef CMD_HK_FIVE_CARD_HEAD_FILE
#define CMD_HK_FIVE_CARD_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID							101									//游戏 I D
#define GAME_PLAYER						5									//游戏人数
#define GAME_NAME						TEXT("炸金花")						//游戏名字
#define MAX_COUNT						3									//扑克数目
#define VERSION_SERVER			    	PROCESS_VERSION(7,0,1)				//程序版本
#define VERSION_CLIENT				    PROCESS_VERSION(7,0,1)				//程序版本

//游戏概率
#define GIVE_UP							2									//放弃概率
#define LOOK_CARD						8									//看牌概率
#define COMPARE_CARD					5									//比牌概率

#define MAX_ROUND						20									//最大轮数

//结束原因
#define GER_NO_PLAYER					0x10								//没有玩家
#define GER_COMPARECARD					0x20								//比牌结束
#define GER_OPENCARD					0x30								//开牌结束

//游戏状态
#define GS_T_FREE						0									//等待开始
#define GS_T_PLAYING					100									//游戏进行

enum PlayerStatus
{
	PlayerNoPlay = 0,
	PlayerPlaying,
	PlayerLookCard,
	PlayerGiveUp,
	PlayerComFail,
	PlayerShowHand
};

//////////////////////////////////////////////////////////////////////////
//服务器命令结构
#define SUB_S_WAIT_START				2002									//等待开始
#define SUB_S_GAME_START				2003									//游戏开始
#define SUB_S_ADD_SCORE					2004									//加注结果
#define SUB_S_LOOK_CARD					2005									//看牌结果
#define SUB_S_COMPARE_CARD				2006									//比牌结果
#define SUB_S_GIVE_UP					2007									//放弃结果
#define SUB_S_GAME_END					2008									//游戏结束
#define SUB_S_SHOW_HAND					2009									//全压
#define SUB_S_OPEN_CARD					2010									//开牌消息
#define SUB_S_PLAYER_BET_FALURE			2011									//下注失败
#define SUB_S_PLAYER_COMMPARE_FALURE	2012									//比牌失败
#define SUB_S_PLAYER_ALL_BET_FALURE		2013									//全压失败
#define SUB_S_PLAYER_FOLLOW_END			2016									//跟到底
#define	SUB_S_ANDROID_CARD				2017									//机器人扑克
#define SUB_S_ANDROID_CONFIG			2019									//机器人配置

#define SUB_S_RESPONSE_CONTROL			2020									//超端信息响应
#define	SUB_S_CHANGE_SUCCESS			2021									//变牌成功

//超端控制信息请求
struct CMD_S_ControlInfo
{
	BYTE								cbHandCardData[GAME_PLAYER][MAX_COUNT];	//手牌扑克
	bool								bIsAndroid[GAME_PLAYER];				//是否机器人玩家

	BYTE								cbCanChangeCard[52];					//能够改变的牌
};

//超端控制改变手牌
struct CMD_S_ControlChangeCard
{
	BYTE								cbCardData[MAX_COUNT];				//用户扑克
};

//机器人存款取款
struct tagCustomAndroid
{
	LONGLONG							lRobotScoreMin;	
	LONGLONG							lRobotScoreMax;
	LONGLONG	                        lRobotBankGet; 
	LONGLONG							lRobotBankGetBanker; 
	LONGLONG							lRobotBankStoMul; 

	int									nLookCard;
	int									nSingleGiveup;
	int									nShunziGiveup;
	int									nLoserGiveup;
	int									nLoserCompare;
	int									nWinnerCompare;
};

//游戏状态
struct CMD_S_StatusFree
{
	DWORD								nTimeLeave;							//剩余时间
	DWORD								nTotalTime;							//操作时间
	SCORE								lCellScore;							//基础积分
	SCORE								lPlayerScore;						//玩家金币
	int									nMaxRound;							//最大轮数
};

//游戏状态
struct CMD_S_StatusPlay
{
	DWORD								nTimeLeave;							//剩余时间
	DWORD								nTotalTime;							//操作时间

	//加注信息
	SCORE								lPlayerScore;						//玩家金币
	SCORE								lCellScore;							//单元下注

	SCORE								lMinScore;							//最小跟注金币
	SCORE								lMaxScore;							//最大加注金币

	SCORE								lAllTableScore;						//桌面分数

	int									nCurrentRound;						//当前轮数
	int									nMaxRound;							//最大轮数

	BYTE								cbFollowStatus;						//跟到底状态

	//状态信息
	WORD								wBankerUser;						//庄家用户
	WORD				 				wCurrentUser;						//当前玩家
	BYTE								cbPlayStatus[GAME_PLAYER];			//游戏状态0没有玩家或者旁观游戏,1，游戏中，2 看牌，3 放弃，4 比牌失败
	SCORE								lPlayerBetScore[GAME_PLAYER];		//玩家下注数目

	//扑克信息
	BYTE								cbHandCardData[MAX_COUNT];			//扑克数据
};

//机器人扑克
struct CMD_S_AndroidCard
{
	BYTE								cbRealPlayer[GAME_PLAYER];				//真人玩家
	BYTE								cbAndroidStatus[GAME_PLAYER];			//机器数目
	BYTE								cbAllHandCardData[GAME_PLAYER][MAX_COUNT];//手上扑克
	BYTE								cbPlayStatus[GAME_PLAYER];			//游戏状态0没有玩家或者旁观游戏,1，游戏中，2 看牌，3 放弃，4 比牌失败					
};

//等待开始
struct CMD_S_WaitStart
{
	SCORE								lPlayerScore;						//用户金币
	BYTE								cbUserCount;						//用户人数
};

//游戏开始
struct CMD_S_GameStart
{
	//下注信息
	SCORE								lUserMaxScore;						//分数上限

	SCORE								lMinScore;							//最小跟注金币
	SCORE								lMaxScore;							//最大加注金币

	//用户信息
	WORD								wBankerUser;						//庄家用户
	WORD				 				wCurrentUser;						//当前玩家

	SCORE								lPlayerRestScore[GAME_PLAYER];		//玩家剩余分数
};

//用户下注
struct CMD_S_AddScore
{
	WORD								wCurrentUser;						//当前用户
	WORD								wAddScoreUser;						//加注用户
	SCORE								lAddScoreCount;						//加注数目

	SCORE								lMinScore;							//最小跟注金币
	SCORE								lMaxScore;							//最大加注金币

	BYTE								cbGameTurns;						//游戏轮数
	bool								cbShowHandStatus;					//全压状态

	SCORE								lPlayerScore;						//下注用户金币数
	SCORE								lPlayerTableScore;					//下注用户桌面分数
	SCORE								lAllTableScore;						//桌面分数
};

//失败结构
struct CMD_S_FailResult
{
	TCHAR								szInfo[64];							//文字信息
};

//用户放弃
struct CMD_S_GiveUp
{
	WORD								wGiveUpUser;						//放弃用户
	WORD								wCurrentUser;						//当前用户
	BYTE								cbGameTurns;						//游戏轮数

};

//比牌数据包
struct CMD_S_CompareCard
{
	WORD								wCurrentUser;						//当前用户

	WORD								wCompareUser[2];					//比牌用户
	SCORE								lCompareScore;						//比牌金币
	SCORE								lPlayerScore;						//玩家剩余金币
	SCORE								lPlayerTableScore;					//比牌那个人总下注
	SCORE								lAllTableScore;						//桌面分数
	BYTE								cbGameTurns;						//游戏轮数
	WORD								wWinUser;							//赢牌用户
};

//看牌数据包
struct CMD_S_LookCard
{
	WORD								wLookCardUser;						//看牌用户
	BYTE								cbCardData[MAX_COUNT];				//用户扑克
	BYTE								cbCardType;
};

//开牌数据包
struct CMD_S_OpenCard
{
	WORD								wOpenUser;							//开牌用户
	BYTE								cbCardData[MAX_COUNT];				//用户扑克
};

//游戏结束
struct CMD_S_GameEnd
{
	SCORE								lGameScore[GAME_PLAYER];			//游戏得分
	BYTE								cbCardData[GAME_PLAYER][3];			//用户扑克
	BYTE								cbCardType[GAME_PLAYER];			//扑克类型					
	//WORD								wCompareUser[GAME_PLAYER][4];		//比牌用户
	SCORE								lPlayerScore[GAME_PLAYER];			//用户金币
};

//跟到底
struct CMD_S_Follow
{
	BYTE								cbFollowStatus;						//跟到底状态
};

//////////////////////////////////////////////////////////////////////////

//客户端命令结构
#define SUB_C_ADD_SCORE					100									//用户加注
#define SUB_C_GIVE_UP					101									//放弃消息
#define SUB_C_LOOK_CARD					102									//看牌消息
#define SUB_C_COMPARE_CARD				103									//比牌消息
#define SUB_C_PLAYER_ALLIN				104									//玩家全压
#define SUB_C_OPEN_CARD					105									//开牌消息
#define SUB_C_FOLLOW_END				106									//跟到底

#define SUB_C_REQUEST_CONTROL			20									//控制信息请求
#define SUB_C_CHANGE_CARD				21									//改变手牌

//用户加注
struct CMD_C_AddScore
{
	SCORE								lScore;								//加注数目
};

//比牌数据包
struct CMD_C_CompareCard
{	
	WORD								wCompareUser;						//比牌用户
};

//超端局控
struct CMD_C_ChangeCard
{
	BYTE									cbCardData[MAX_COUNT];				//想要变成的手牌
};
//////////////////////////////////////////////////////////////////////////


#pragma pack()

#endif
