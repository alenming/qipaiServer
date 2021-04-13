#ifndef IPC_GAME_FRAME_HEAD_FILE
#define IPC_GAME_FRAME_HEAD_FILE

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//网络消息

#define IPC_CMD_GF_SOCKET			1									//网络消息

#define IPC_SUB_GF_SOCKET_SEND		1									//网络发送
#define IPC_SUB_GF_SOCKET_RECV		2									//网络接收

//网络发送
struct IPC_GF_SocketSend
{
	TCP_Command						CommandInfo;						//命令信息
	BYTE							cbBuffer[SOCKET_TCP_PACKET];		//数据缓冲
};

//网络接收
struct IPC_GF_SocketRecv
{
	TCP_Command						CommandInfo;						//命令信息
	BYTE							cbBuffer[SOCKET_TCP_PACKET];		//数据缓冲
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//控制消息

#define IPC_CMD_GF_CONTROL			2									//控制消息

#define IPC_SUB_GF_CLIENT_READY		1									//准备就绪
#define IPC_SUB_GF_CLIENT_CLOSE		2									//进程关闭

#define IPC_SUB_GF_CLOSE_PROCESS	100									//关闭进程
#define IPC_SUB_GF_ACTIVE_PROCESS	101									//激活进程

#define IPC_SUB_GF_BOSS_COME		200									//老板来了
#define IPC_SUB_GF_BOSS_LEFT		201									//老板走了

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//配置消息

#define IPC_CMD_GF_CONFIG			3									//配置消息

#define IPC_SUB_GF_LEVEL_INFO		100									//等级信息
#define IPC_SUB_GF_COLUMN_INFO		101									//列表信息
#define IPC_SUB_GF_SERVER_INFO		102									//房间信息

#define IPC_SUB_GF_CONFIG_FINISH	104									//配置完成
#define IPC_SUB_GF_GROWLEVEL_CONFIG 105									//等级配置
#define IPC_SUB_GF_USER_RIGHT		107									//玩家权限
#define IPC_SUB_GF_MATCH_INFO		108									//比赛信息

//房间信息
struct IPC_GF_ServerInfo
{
	//用户信息
	WORD							wTableID;							//桌子号码
	WORD							wChairID;							//椅子号码
	DWORD							dwUserID;							//用户 I D

	//用户权限
	DWORD							dwUserRight;						//用户权限
	DWORD							dwMasterRight;						//管理权限

	//房间信息
	WORD							wKindID;							//类型标识
	WORD							wServerID;							//房间标识
	WORD							wServerType;						//房间类型
	DWORD							dwServerRule;						//房间规则
	TCHAR							szServerName[LEN_SERVER];			//房间名称

	//视频配置
	WORD							wAVServerPort;						//服务端口
	DWORD							dwAVServerAddr;						//服务地址
};

//比赛类型
struct IPC_GF_MatchInfo
{
	tagGameMatch					GameMatch;
};

//等级信息
struct IPC_GF_LevelInfo
{
	BYTE							cbItemCount;						//列表数目
	tagLevelItem					LevelItem[64];						//等级描述
};

//列表配置
struct IPC_GF_ColumnInfo
{
	BYTE							cbColumnCount;						//列表数目
	tagColumnItem					ColumnItem[MAX_COLUMN];				//列表描述
};

//等级配置
struct IPC_GF_GrowLevelConfig
{
	WORD							wLevelCount;						//等级数目
	tagGrowLevelConfig				GrowLevelItem[60];					//等级配置
};

//玩家权限
struct IPC_GF_UserRight
{
	DWORD							dwUserRight;						//玩家权限
};
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//用户消息

#define IPC_CMD_GF_USER_INFO		4									//用户消息

#define IPC_SUB_GF_USER_ENTER		100									//用户进入
#define IPC_SUB_GF_USER_SCORE		101									//用户分数
#define IPC_SUB_GF_USER_STATUS		102									//用户状态
#define IPC_SUB_GF_USER_ATTRIB		103									//用户属性
#define IPC_SUB_GF_CUSTOM_FACE		104									//自定头像
#define IPC_SUB_GF_KICK_USER        105                                 //用户踢出
#define IPC_SUB_GF_QUICK_TRANSPOS   106                                 //用户换位

//用户信息
struct IPC_GF_UserInfo
{
	BYTE							cbCompanion;						//用户关系
	tagUserInfoHead					UserInfoHead;						//用户信息
};

//用户积分
struct IPC_GF_UserScore
{
	DWORD							dwUserID;							//用户标识
	tagUserScore					UserScore;							//用户积分
};

//用户状态
struct IPC_GF_UserStatus
{
	DWORD							dwUserID;							//用户标识
	tagUserStatus					UserStatus;							//用户状态
};

//用户属性
struct IPC_GF_UserAttrib
{
	DWORD							dwUserID;							//用户标识
	tagUserAttrib					UserAttrib;							//用户属性
};

//用户头像
struct IPC_GF_CustomFace
{
	DWORD							dwUserID;							//用户标识
	DWORD							dwCustomID;							//自定标识
	tagCustomFaceInfo				CustomFaceInfo;						//自定头像
};

//用户踢出
struct IPC_GF_KickUser
{
	DWORD                          dwTargetUserID;                      //目标用户
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//比赛消息
#define IPC_CMD_GF_MATCH_INFO		6								//比赛消息

#define IPC_SUB_GF_EXIT_MATCH 		100								//离开比赛
#endif