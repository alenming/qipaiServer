#ifndef CMD_SG_GAME_H
#define CMD_SG_GAME_H

#pragma pack(push)
#pragma pack(1)


#define KIND_ID             352                          //游戏 I D
#define GAME_NAME			TEXT("三公")	             //游戏名字
#define GAME_PLAYER			9				             //游戏人数

#define VERSION_SERVER          PROCESS_VERSION(7,0,1)   //程序版本
#define VERSION_CLIENT          PROCESS_VERSION(7,0,1)   //程序版本



#define FULL_COUNT          52					//整副牌的张数
#define MAX_COUNT           3                   //玩家牌数
#define NO_BANKER			0xFFFF				//没有庄家
#define INVALID_BANKER      0xFF                //无效庄家

//抢庄状态
#define OPERATE_NO_BANKERER           0          //不抢庄
#define OPERATE_SCORE_ONE             1          //下一注

//游戏状态
#define GS_TK_FREE            GAME_STATUS_FREE          //等待开始
#define GS_TK_CALL            GAME_STATUS_PLAY          //叫庄状态
#define GS_TK_SCORE           GAME_STATUS_PLAY+1        //下注状态

//////////////////////////////////////////////////////////////////////////
//服务器命令
#define SUB_S_WAIT_START		100					//等待开始
#define SUB_S_GAME_START        101                 //游戏开始
#define SUB_S_CALL_BANKER       102                 //用户叫庄
#define SUB_S_BANKER_END        103                 //叫庄结果
#define SUB_S_ADD_SCORE         104                 //加注结果
#define SUB_S_GAME_END          105                 //游戏结束
#define SUB_S_ANDROID_CONFIG	106					//发送机器人配置

//游戏状态
/////////////////////////////////////////////////////////////////////////////////
//空闲状态
struct CMD_S_StatusFree
{
	SCORE				lCellScore;							//游戏底分

};

//叫庄状态
struct CMD_S_StatusCall
{
	SCORE				lCellScore;							//游戏底分
	BYTE				cbTimeLeave;						//剩余时间
	BYTE                cbPlayStatus[GAME_PLAYER];		    //用户状态
	BYTE				m_cbCallStatus[GAME_PLAYER];		//叫庄状态
	BYTE                cbCardData[MAX_COUNT];				//用户扑克
};

//下注状态
struct CMD_S_StatusScore
{
	SCORE				lCellScore;							//游戏底分
	BYTE				cbTimeLeave;						//剩余时间
	WORD                wBankerUser;                        //庄家用户
	BYTE                cbPlayStatus[GAME_PLAYER];          //用户状态
	BYTE				cbCallStatus[GAME_PLAYER];			//叫庄状态
	SCORE				lTableScore[GAME_PLAYER];           //下注数目
	BYTE                cbCardData[MAX_COUNT];              //用户扑克
};


//服务器命令结构
/////////////////////////////////////////////////////////////////////
//游戏开始
struct CMD_S_GameStart
{
	BYTE				cbPlayStatus[GAME_PLAYER];			//玩家状态
	BYTE				cbCardData[MAX_COUNT];				//用户扑克
};

//机器人配置
struct tagRobotConfig
{
	LONGLONG            lRobotScoreMin;
	LONGLONG            lRobotScoreMax;
	LONGLONG            lRobotBankGet;
	LONGLONG            lRobotBankGetBanker;
	LONGLONG            lRobotBankStoMul;
};

//用户叫庄
struct CMD_S_Call_Banker
{
	WORD				cbBanker;						//叫庄用户
	BYTE				cbStatus;						//叫庄状态
};

//抢庄结果
struct CMD_S_Banker_End
{
	WORD				cbBanker;						//庄家用户
	BYTE				cbCallStatus[GAME_PLAYER];		//抢庄用户
};

//用户下注
struct CMD_S_AddScore
{
	WORD                wAddScoreUser;					//加注用户
	SCORE				lAddScoreCount;					//加注数目
};


//游戏结束
struct CMD_S_GameEnd
{
	BYTE				cbPlayerStatus[GAME_PLAYER];		//玩家状态
	BYTE				cbPlayerValue[GAME_PLAYER];			//玩家点数
	SCORE				lGameScore[GAME_PLAYER];			//游戏得分
	BYTE				cbCardData[GAME_PLAYER][MAX_COUNT]; //用户扑克
};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构
#define SUB_C_CALL_BANKER       1					//用户叫庄
#define SUB_C_ADD_SCORE         2					//用户加注

//用户叫庄
struct CMD_C_CallBanker
{
	BYTE						bBanker;            //做庄标志
};


//用户加注
struct CMD_C_AddScore
{
	SCORE						lScore;				//加注数目
};

//////////////////////////////////////////////////////////////////////////

#pragma pack(pop)

#endif
