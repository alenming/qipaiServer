#ifndef CMD_THIRTEENX_HEAD_FILE
#define CMD_THIRTEENX_HEAD_FILE
#pragma pack ( 1 )
//////////////////////////////////////////////////////////////////////////
//公共宏定义
#define KIND_ID							126									//游戏 I D
#define GAME_PLAYER						4									//游戏人数
#define GAME_NAME						TEXT("十三水")						//游戏名字
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//游戏类型

#define VERSION_SERVER					PROCESS_VERSION(7,0,1)				//程序版本
#define VERSION_CLIENT					PROCESS_VERSION(7,0,1)				//程序版本

//常量数据
#define HAND_CARD_COUNT					13									//扑克数目
#define DISPATCH_COUNT					52									//派发数目

#define MAX_CARD_LIST					6									//自动选牌数目
#define MAX_TYPE_COUNT					4									//每个人牌型数目

//游戏状态
#define GS_WK_FREE						GAME_STATUS_FREE					//等待开始
#define GS_WK_PLAYING					GAME_STATUS_PLAY					//游戏进行
#define GS_WK_END						GAME_STATUS_PLAY+1					//结束状态

enum PlayerStatus
{
	NoPlayer,			//没有玩家
	LookOn,				//观战中
	Range_Card,			//理牌中
	Range_End,			//理牌完成
	Show_Card			//显示所有
};

#ifndef _UNICODE
#define myprintf	_snprintf
#define mystrcpy	strcpy
#define mystrlen	strlen
#define myscanf		_snscanf
#define myLPSTR		LPCSTR
#define myatoi		atoi
#define myatoi64	_atoi64
#else
#define myprintf	swprintf
#define mystrcpy	wcscpy
#define mystrlen	wcslen
#define myscanf		_snwscanf
#define myLPSTR		LPWSTR
#define myatoi		_wtoi
#define myatoi64	_wtoi64
#endif

//////////////////////////////////////////////////////////////////////////
//服务器命令结构
#define SUB_S_GAME_START				2002								//游戏开始
#define SUB_S_GAME_SENDCARD				2003								//开始发牌
#define SUB_S_GAME_OUTCARD				2004								//客户端出牌结果
#define SUB_S_GAME_END					2006								//结算
#define SUB_S_GAME_SETTLEEND			2007								//结算结束
////////////////////////////////////////////////////////////////////////////
//空闲状态
struct CMD_S_StatusFree
{
	SCORE								lCellScore;							//底分
	BYTE								cbDelayTime;						//发完牌之后弹出理牌界面的间隔时间
	BYTE								cbEndTime;							//结算时间
};

//游戏状态
struct CMD_S_StatusPlay
{
	SCORE								lCellScore;							//底分
	BYTE								cbDelayTime;						//发完牌之后弹出理牌界面的间隔时间
	BYTE								cbEndTime;							//结算时间
	BYTE								cbPlayerStatus[GAME_PLAYER];		//玩家状态
	BYTE								cbLeaveTime;						//剩余时间

	BYTE								cbCardData[HAND_CARD_COUNT];		//扑克
	BYTE								cbSpecialType;						//特殊牌类型
	BYTE								cbSpecialCard[HAND_CARD_COUNT];		//特殊牌
	BYTE								cbBestCardList[MAX_CARD_LIST][HAND_CARD_COUNT];	//最佳组合
};

//结束状态
struct CMD_S_StatusEnd
{
	SCORE								lCellScore;							//底分
	BYTE								cbDelayTime;						//发完牌之后弹出理牌界面的间隔时间
	BYTE								cbEndTime;							//结算时间
	BYTE								cbPlayerStatus[GAME_PLAYER];		//玩家状态
	BYTE								cbLeaveTime;						//剩余时间

	BYTE								cbEndCardList[GAME_PLAYER][HAND_CARD_COUNT];	//游戏结束理牌列表
	BYTE								cbEndCardType[GAME_PLAYER][MAX_TYPE_COUNT];		//结束时牌型，第一个放特殊牌型，后三个是三墩牌型
	SCORE								lGameScore[GAME_PLAYER];			//总输赢分

	SCORE								lPerScore[GAME_PLAYER][9];			//每一墩的分数，0放特殊牌型分，1~3是三墩得分,4~6是三墩附加分,7是打枪加分,8是全垒打加分
};

//开始发牌
struct CMD_S_SendCard
{
	BYTE								cbLeaveTime;						//剩余时间
	BYTE								cbCard[HAND_CARD_COUNT];			//用户扑克
	BYTE								cbSpecialType;						//特殊牌类型
	BYTE								cbSpecialCard[HAND_CARD_COUNT];		//特殊牌
	BYTE								cbBestCardList[MAX_CARD_LIST][HAND_CARD_COUNT];	//最佳组合

	BYTE								cbPlayerStatus[GAME_PLAYER];		//玩家状态
};

//客户端出牌结果
struct CMD_S_OutCard
{
	WORD								wChairID;							//出牌用户
	BYTE								cbSpecialType;						//这里0为不是特殊牌型，1为是特殊牌型
};

//游戏结束
struct CMD_S_GameEnd
{
	WORD								wFireList[GAME_PLAYER][GAME_PLAYER-1];	//打枪
	BYTE								cbEndCardList[GAME_PLAYER][HAND_CARD_COUNT];	//游戏结束理牌列表
	BYTE								cbEndCardType[GAME_PLAYER][MAX_TYPE_COUNT];		//结束时牌型，第一个放特殊牌型，后三个是三墩牌型
	SCORE								lGameScore[GAME_PLAYER];			//总输赢分

	SCORE								lPerScore[GAME_PLAYER][9];			//每一墩的分数，0放特殊牌型分，1~3是三墩得分,4~6是三墩附加分,7是打枪加分,8是全垒打加分
};
//////////////////////////////////////////////////////////////////////////
//客户端命令结构
#define SUB_C_PLAYER_SHOWCARD			100									//玩家亮牌

//玩家亮牌
struct CMD_C_ShowCard
{
	BYTE								cbCardData[HAND_CARD_COUNT];		//扑克
	BYTE								cbCardType[MAX_TYPE_COUNT];			//牌型
};
//////////////////////////////////////////////////////////////////////////
#endif
