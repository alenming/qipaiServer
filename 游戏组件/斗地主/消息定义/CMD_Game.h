#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

#pragma pack(1)

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//服务定义

//游戏属性
#define KIND_ID						100									//游戏 I D
#define GAME_NAME					TEXT("斗地主")						//游戏名字

//组件属性
#define GAME_PLAYER					3									//游戏人数
#define VERSION_SERVER				PROCESS_VERSION(7,0,1)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//程序版本

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//数目定义
#define MAX_COUNT					20									//最大数目
#define FULL_COUNT					54									//全牌数目

//逻辑数目
#define NORMAL_COUNT				17									//常规数目
#define DISPATCH_COUNT				51									//派发数目

//数值掩码
#define	MASK_COLOR					0xF0								//花色掩码
#define	MASK_VALUE					0x0F								//数值掩码

//逻辑类型
#define CT_ERROR					0									//错误类型
#define CT_SINGLE					1									//单牌类型
#define CT_DOUBLE					2									//对牌类型
#define CT_THREE					3									//三条类型
#define CT_SINGLE_LINE				4									//单连类型
#define CT_DOUBLE_LINE				5									//对连类型
#define CT_THREE_LINE				6									//三连类型
#define CT_THREE_TAKE_ONE			7									//三带一单
#define CT_THREE_TAKE_TWO			8									//三带一对
#define CT_FOUR_TAKE_ONE			9									//四带两单
#define CT_FOUR_TAKE_TWO			10									//四带两对
#define CT_BOMB_CARD				11									//炸弹类型
#define CT_MISSILE_CARD				12									//火箭类型

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//状态定义

#define GAME_SCENE_FREE				0									//等待开始
#define GAME_SCENE_CALL				200									//叫分状态
#define GAME_SCENE_MULTIPLE			201									//加倍状态
#define GAME_SCENE_PLAY				202									//游戏进行
#define GAME_SCENE_END				300									//游戏结束

//空闲状态
struct CMD_S_StatusFree
{
	int								cbCallTimes;						//叫庄时间
	int								cbOutTimes;							//出牌时间
	int								cbDoubleTimes;						//加倍时间
	int								cbEndTimes;							//结束时间

	BYTE							nTrusteeStatus[GAME_PLAYER];		//托管状态  0正常  1托管
};

//叫分状态
struct CMD_S_StatusCall
{
	int								cbCallTimes;						//叫庄时间
	int								cbOutTimes;							//出牌时间
	int								cbDoubleTimes;						//加倍时间
	int								cbEndTimes;							//结束时间

	BYTE							nTrusteeStatus[GAME_PLAYER];		//托管状态  0正常  1托管

	BYTE							cbTimeLeave;						//剩余时间
	BYTE							cbScoreInfo[GAME_PLAYER];			//叫分信息
	WORD							wCurrentUser;						//当前玩家
	BYTE							cbHandCardData[NORMAL_COUNT];		//手上扑克
};

//加倍状态
struct CMD_S_StatusDouble
{
	int								cbCallTimes;						//叫庄时间
	int								cbOutTimes;							//出牌时间
	int								cbDoubleTimes;						//加倍时间
	int								cbEndTimes;							//结束时间

	BYTE							nTrusteeStatus[GAME_PLAYER];		//托管状态  0正常  1托管

	BYTE							cbTimeLeave;						//剩余时间

	BYTE							cbBankerScore;						//庄家叫分
	BYTE							cbPlayerMultiple[GAME_PLAYER];		//玩家倍数
	WORD							wBankerUser;						//庄家用户
	BYTE							cbBankerCard[3];					//游戏底牌
	BYTE							cbHandCardData[MAX_COUNT];			//手上扑克
};

//游戏状态
struct CMD_S_StatusPlay
{
	int								cbCallTimes;						//叫庄时间
	int								cbOutTimes;							//出牌时间
	int								cbDoubleTimes;						//加倍时间
	int								cbEndTimes;							//结束时间

	BYTE							nTrusteeStatus[GAME_PLAYER];		//托管状态  0正常  1托管

	BYTE							cbTimeLeave;						//剩余时间

	WORD							wBankerUser;						//庄家用户
	WORD							wCurrentUser;						//当前玩家
	BYTE							cbBankerScore;						//庄家叫分

	BYTE							cbBankerCard[3];					//游戏底牌
	BYTE							cbHandCardData[MAX_COUNT];			//手上扑克
	BYTE							cbHandCardCount[GAME_PLAYER];		//扑克数目

	BYTE							cbBombCount;						//炸弹次数
	BYTE							cbPlayerMultiple[GAME_PLAYER];		//玩家倍数

	WORD							wTurnWiner;							//胜利玩家
	BYTE							cbTurnCardCount;					//出牌数目
	BYTE							cbTurnCardData[MAX_COUNT];			//出牌数据
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//命令定义

#define SUB_S_GAME_START			2002									//游戏开始
#define SUB_S_CALL_SCORE			2003									//用户叫分
#define SUB_S_BANKER_INFO			2004									//庄家信息
#define SUB_S_GAME_CALL_DOUBLE		2005									//用户加倍
#define SUB_S_BANKER_OUT			2006									//地主开始出牌
#define SUB_S_OUT_CARD				2007									//用户出牌
#define SUB_S_PASS_CARD				2008									//用户放弃
#define SUB_S_TRUSTEE				2009									//托管
#define SUB_S_GAME_CONCLUDE			2010									//游戏结束



//发送扑克
struct CMD_S_GameStart
{
	WORD				 			wCurrentUser;						//当前玩家
	BYTE							cbCardData[NORMAL_COUNT];			//扑克列表
};

//机器人扑克
struct CMD_S_AndroidCard
{
	BYTE							cbHandCard[GAME_PLAYER][NORMAL_COUNT];//手上扑克
	BYTE							cbBankerCard[3];
	WORD							wCurrentUser ;						//当前玩家
};

//作弊扑克
struct CMD_S_CheatCard
{
	WORD							wCardUser[GAME_PLAYER ];				//作弊玩家
	BYTE							cbUserCount;							//作弊数量
	BYTE							cbCardData[GAME_PLAYER ][MAX_COUNT];	//扑克列表
	BYTE							cbCardCount[GAME_PLAYER ];				//扑克数量

};
//用户叫分
struct CMD_S_CallScore
{
	WORD				 			wCurrentUser;						//当前玩家
	WORD							wCallScoreUser;						//叫分玩家
	BYTE							cbCurrentScore;						//当前叫分
	BYTE							cbUserCallScore;					//上次叫分
};

//庄家信息
struct CMD_S_BankerInfo
{
	WORD				 			wBankerUser;						//庄家玩家
	BYTE							cbBankerScore;						//庄家叫分
	BYTE							cbBankerCard[3];					//庄家扑克
};

//用户加倍
struct CMD_S_Double
{
	WORD							wDoubleUser;						//加倍玩家
	BYTE							cbPlayerMultiple;					//加倍倍数
};

//地主开始出牌
struct CMD_S_BankerOut
{
	WORD							wBankerUser;						//庄家玩家
};

//用户出牌
struct CMD_S_OutCard
{
	BYTE							cbCardCount;						//出牌数目
	WORD				 			wCurrentUser;						//当前玩家
	WORD							wOutCardUser;						//出牌玩家
	BYTE							cbCardData[MAX_COUNT];				//扑克列表
};

//放弃出牌
struct CMD_S_PassCard
{
	WORD				 			wCurrentUser;						//当前玩家
	WORD				 			wPassCardUser;						//放弃玩家
};

//游戏结束
struct CMD_S_GameConclude
{
	//积分变量
	SCORE							lGameScore[GAME_PLAYER];			//游戏积分

	//春天标志
	BYTE							bChunTian;							//春天标志
	BYTE							bFanChunTian;						//春天标志

	//炸弹信息
	BYTE							cbBombCount;						//炸弹个数

	//游戏信息
	BYTE							cbCardCount[GAME_PLAYER];			//扑克数目
	BYTE							cbHandCardData[GAME_PLAYER][MAX_COUNT];			//扑克列表
};

//托管
struct CMD_S_TRUSTEE
{
	WORD							wTrusteeUser;						//托管玩家
	BYTE							bTrustee;							//托管标志
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//命令定义

#define SUB_C_CALL_SCORE			100									//用户叫分
#define SUB_C_DOUBLE				101									//用户加倍
#define SUB_C_OUT_CARD				102									//用户出牌
#define SUB_C_PASS_CARD				103									//用户放弃
#define SUB_C_TRUSTEE				104									//用户托管


//用户叫分
struct CMD_C_CallScore
{
	BYTE							cbCallScore;						//叫分数目
};

//用户出牌
struct CMD_C_OutCard
{
	BYTE							cbCardCount;						//出牌数目
	BYTE							cbCardData[MAX_COUNT];				//扑克数据
};

//用户加倍
struct CMD_C_CallDouble
{
	BYTE							cbPlayerMultiple;					//加倍倍数
};

//托管
struct CMD_C_TRUSTEE
{
	BYTE							bTrustee;							//托管标志
};
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

#pragma pack()

#endif