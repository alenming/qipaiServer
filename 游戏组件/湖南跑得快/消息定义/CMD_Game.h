#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

#pragma pack(1)

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//服务定义

//游戏属性
#define KIND_ID						113									//游戏 I D
#define GAME_NAME					TEXT("湖南跑得快")					//游戏名字

//组件属性
#define GAME_PLAYER					3									//游戏人数
#define VERSION_SERVER				PROCESS_VERSION(7,0,1)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//程序版本

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//状态定义

#define GAME_SCENE_FREE				GAME_STATUS_FREE					//等待开始
#define GAME_SCENE_PLAY				200									//游戏进行


#define MAX_COUNT					16
//////////////////////////////////////////////////////////////////////////
//服务器命令结构
#define SUB_S_GAME_FREE				2001									//等待开始
#define SUB_S_GAME_START			2002									//游戏开始
#define SUB_S_OUT_CARD				2003									//用户出牌
#define SUB_S_PASS_CARD				2004									//放弃出牌
#define SUB_S_GAME_END				2006									//游戏结束
#define SUB_S_AUTOMATISM			2005									//托管消息

//游戏状态
struct CMD_S_StatusFree
{
	SCORE						lBaseScore;							//基础积分
};

//游戏状态
struct CMD_S_StatusPlay
{
	BYTE							bCardData[MAX_COUNT];				//自己的手牌
	bool							bAutoStatus[GAME_PLAYER];			//托管状态
	BYTE							bCardCount[GAME_PLAYER];			//扑克数目
	BYTE							bTurnCardData[MAX_COUNT];			//出牌列表
	WORD							wLastOutUser;						//出牌的人
	WORD				 			wCurrentUser;						//当前玩家

	WORD							wOutCardTime;						//出牌时间
	WORD							wEndTime;							//结束时间
};

//发送扑克
struct CMD_S_GameStart
{
	WORD							wBankerUser;						//庄家用户
	BYTE							cbCardData[MAX_COUNT];				//扑克列表

	WORD							wOutCardTime;						//出牌时间
	WORD							wEndTime;							//结束时间
};

//用户出牌
struct CMD_S_OutCard
{
	bool							bResult;							//出牌结果（1成功，其他失败）
	WORD							wOutCardUser;						//出牌玩家
	WORD				 			wCurrentUser;						//当前玩家
	BYTE							bCardCount;							//扑克数目
	BYTE							bCardData[MAX_COUNT];				//扑克列表
};

//放弃出牌
struct CMD_S_PassCard
{
	bool							bNewTurn;							//一轮开始
	WORD				 			wPassUser;							//放弃玩家
	WORD				 			wCurrentUser;						//当前玩家
};

//游戏结束
struct CMD_S_GameEnd
{
	WORD							wWinUser;							//赢家
	BYTE							bCardData[GAME_PLAYER][MAX_COUNT];	//扑克列表 
	
	TCHAR							szPlayerName[GAME_PLAYER][32];		//玩家名字
	BYTE							cbCardCount[GAME_PLAYER];			//扑克数目
	SCORE							lGameScore[GAME_PLAYER];			//输赢金币
	BYTE							cbBombCount[GAME_PLAYER];			//炸弹数量
	SCORE							lBombScore[GAME_PLAYER];			//炸弹得分
	BYTE							cbQuanGuan[GAME_PLAYER];			//是否全关
	BYTE							cbBaopei[GAME_PLAYER];				//是否包赔
};

//玩家托管事件
struct CMD_S_UserAutomatism
{
	WORD							wChairID;							//椅子号
	bool							bTrusee;							//托管
};
//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_OUT_CART				101									//用户出牌
#define SUB_C_PASS_CARD				102									//放弃出牌
#define SUB_C_AUTOMATISM			103									//托管消息

//用户托管
struct CMD_C_Automatism
{
	BYTE							bAutomatism;						//托管标志
};

//出牌数据包
struct CMD_C_OutCard
{
	BYTE							bCardCount;							//出牌数目
	BYTE							bCardData[MAX_COUNT];				//扑克列表
};

//////////////////////////////////////////////////////////////////////////
#pragma pack()

#endif