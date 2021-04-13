#ifndef CMD_CORRESPOND_HEAD_FILE
#define CMD_CORRESPOND_HEAD_FILE

#pragma pack(1)

//平台定义
#include "..\全局定义\Platform.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//注册命令

#define MDM_CS_REGISTER				1									//服务注册

//服务注册
#define SUB_CS_C_REGISTER_PLAZA		100									//注册广场
#define SUB_CS_C_REGISTER_SERVER	101									//注册房间
#define SUB_CS_C_REGISTER_MATCH		102									//注册比赛
#define SUB_CS_C_REGISTER_CHAT		103									//注册聊天	
//注册结果
#define SUB_CS_S_REGISTER_FAILURE	200									//注册失败

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//注册广场
struct CMD_CS_C_RegisterPlaza
{
	TCHAR							szServerAddr[32];					//服务地址
	TCHAR							szServerName[LEN_SERVER];			//服务器名
};

//注册游戏
struct CMD_CS_C_RegisterServer
{
	WORD							wKindID;							//名称索引
	WORD							wNodeID;							//节点索引
	WORD							wSortID;							//排序索引
	WORD							wServerID;							//房间索引
	WORD                            wServerKind;                        //房间类别
	WORD                            wServerType;                        //房间类型		
	WORD							wServerLevel;						//房间等级
	WORD							wServerPort;						//房间端口
	SCORE							lCellScore;							//单元积分
	SCORE							lEnterScore;						//进入积分
	SCORE							lTableScore;						//坐下游戏积分
	BYTE							cbEnterMember;						//进入会员
	DWORD							dwOnLineCount;						//在线人数
	DWORD							dwFullCount;						//满员人数
	WORD							wTableCount;						//桌子数目
	DWORD							dwServerRule;						//房间规则
	TCHAR							szServerAddr[32];					//服务地址
	TCHAR							szServerName[LEN_SERVER];			//房间名称
};

//注册失败
struct CMD_CS_S_RegisterFailure
{
	LONG							lErrorCode;							//错误代码
	TCHAR							szDescribeString[128];				//错误消息
};

//注册比赛
struct CMD_CS_C_RegisterMatch
{
	//基本信息
	WORD							wServerID;							//房间标识
	//基本信息
	DWORD							dwMatchID;							//比赛标识	
	LONGLONG						lMatchNo;							//比赛场次
	BYTE							cbMatchStatus;						//比赛状态
	BYTE							cbMatchType;						//比赛类型
	WCHAR							szMatchName[32];					//比赛名称

	//报名信息
	BYTE							cbFeeType;							//费用类型
	BYTE							cbDeductArea;						//缴费区域
	LONGLONG						lSignupFee;							//报名费用	
	BYTE							cbSignupMode;						//报名方式
	BYTE							cbJoinCondition;					//参赛条件
	BYTE							cbMemberOrder;						//会员等级
	DWORD							dwExperience;						//玩家经验
	DWORD							dwFromMatchID;						//比赛标识		
	BYTE							cbFilterType;						//筛选方式
	WORD							wMaxRankID;							//最大名次
	SYSTEMTIME						MatchEndDate;						//结束日期
	SYSTEMTIME						MatchStartDate;						//起始日期	

	//排名方式
	BYTE							cbRankingMode;						//排名方式	
	WORD							wCountInnings;						//统计局数
	BYTE							cbFilterGradesMode;					//筛选方式

	//分组设置
	BYTE							cbDistributeRule;					//分组规则
	WORD							wMinDistributeUser;					//最少人数
	WORD							wDistributeTimeSpace;				//分组间隔	
	WORD							wMinPartakeGameUser;				//最少人数
	WORD							wMaxPartakeGameUser;				//最多人数

	//比赛规则
	BYTE							cbMatchRule[512];					//比赛规则

	//奖励信息
	WORD							wRewardCount;						//奖励人数
	tagMatchRewardInfo				MatchRewardInfo[3];					//比赛奖励
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//服务信息

#define MDM_CS_SERVICE_INFO			2									//服务信息

//房间信息
#define SUB_CS_C_SERVER_ONLINE		1									//房间人数
#define SUB_CS_C_SERVER_MODIFY		2									//房间修改
#define SUB_CS_S_SERVER_DUMMY_INFO	3									//统计虚拟人数数据
#define SUB_CS_S_SERVER_DUMMY_INFO_START	4							//统计虚拟人数数据开始
#define SUB_CS_S_SERVER_DUMMY_INFO_END	5								//统计虚拟人数数据接受

//广场命令
#define SUB_CS_S_PLAZA_INFO			100									//广场信息
#define SUB_CS_S_PLAZA_INSERT		101									//广场列表
#define SUB_CS_S_PLAZA_REMOVE		103									//广场删除
#define SUB_CS_S_PLAZA_FINISH		104									//广场完成
#define SUB_CS_S_PLAZA_CHANGE_DUMMY	106									//广场变更虚拟人数
#define SUB_CS_S_PLAZA_CUSTOMER		107									//推送客服反馈
#define SUB_CS_S_RECHARGE_SUCCESS	108									//充值成功提醒
#define SUB_CS_S_NOTIFYIDNEWMAIL	109									//新邮件提醒

//房间命令
#define SUB_CS_S_SERVER_INFO		110									//房间信息
#define SUB_CS_S_SERVER_ONLINE		111									//房间人数
#define SUB_CS_S_SERVER_INSERT		112									//房间列表
#define SUB_CS_S_SERVER_MODIFY		113									//房间修改
#define SUB_CS_S_SERVER_REMOVE		114									//房间删除
#define SUB_CS_S_SERVER_FINISH		115									//房间完成
#define SUB_CS_S_CLEARUER			116									//房间完成
#define SUB_CS_S_MODIFY_DUMMY		117									//修改虚拟人数
#define SUB_CS_S_MODIFY_ROOMSTORAGE	118									//修改房间库存参数

//比赛命令
#define SUB_CS_S_MATCH_INSERT		120									//比赛列表

//聊天命令
#define SUB_CS_S_CHAT_INSERT		130									//聊天服务
#define SUB_SS_S_ONLINE				140									//用户上线
#define SUB_SS_S_OFFLINE			141									//用户离线
#define SUB_SS_S_STATUS				142									//用户状态
#define SUB_SS_S_GAMESTATUS			143									//游戏状态
#define SUB_SS_S_SYNCHRFRIEND		144									//同步好友	
#define SUB_SS_S_SYNCHARFINISH		145									//同步完成
#define SUB_SS_S_MODIFYFRIEND		146									//修改好友
#define SUB_SS_S_REMOVEGROUP		147									//移除分组
#define SUB_SS_S_HALL_RADIO			148									//大厅广播消息
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//解散桌子
struct CMD_CS_S_ClearUser
{
	DWORD							dwServerID;							//房间ID
	DWORD							dwUserID;							//UserID
};

//强制大厅下线
struct CMD_CS_S_ForceOffline
{
	DWORD							dwUserID;							//UserID
};

//修改虚拟人数
struct CMD_CS_S_ModifyDummyCount
{
	DWORD							dwServerID;							//房间ID
	DWORD							dwAddcounts;						//修改人数
};


//房间数据
struct CMD_CS_C_ServerModify
{
	WORD							wSortID;							//排序索引
	WORD							wKindID;							//类型索引
	WORD							wNodeID;							//节点索引
	WORD							wServerPort;						//房间端口
	DWORD							dwOnLineCount;						//在线人数
	DWORD							dwAndroidCount;						//机器人数
	DWORD							dwFullCount;						//满员人数
	TCHAR							szServerAddr[32];					//服务地址
	TCHAR							szServerName[LEN_SERVER];			//房间名称
};

//房间人数
struct CMD_CS_C_ServerOnLine
{
	DWORD							dwOnLineCount;						//在线人数
	DWORD							dwAndroidCount;						//机器人数
};

//房间人数
struct CMD_CS_C_DummyOnLine
{
	WORD							wServerID;							//服务器ID
	DWORD							dwDummyCount;						//虚拟在线人数
};

//推送客服反馈
struct CMD_CS_S_Customer
{
	DWORD							dwUserID;							//UserID
	TCHAR							szContent[256];						//内容
};

//修改房间库存
struct CMD_CS_S_ModifyRoomStorage
{
	WORD							wServerID;							//服务器ID
	int								lStorageStart;
	SCORE							lStorageCurrent;
	int								lStorageDeduct;
	int								lStorageMul;
	SCORE							lStorageMax;
	TCHAR							szStorageControl[STORAGE_COUNT][32];
};

//充值成功提醒
struct CMD_CS_S_RechageSuccess
{
	DWORD							dwUserID;							//UserID
	SCORE							dRechargeScore;						//充值金额
	int								nID;								//邮件ID
	TCHAR							szTitle[20];						//邮件标题
	TCHAR							szSender[20];						//发送人
	TCHAR							szContent[256];						//内容
	TCHAR							szSendTime[20];						//发送时间
	int								nStatus;							//状态
};

//新邮件提醒
struct CMD_CS_S_NotifyidNewMail
{
	DWORD							dwUserID;							//UserID
	int								nID;								//邮件ID
	TCHAR							szTitle[20];						//邮件标题
	TCHAR							szSender[20];						//发送人
	TCHAR							szContent[256];						//内容
	TCHAR							szSendTime[20];						//发送时间
	int								nStatus;							//状态
};
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//广场删除
struct CMD_CS_S_PlazaRemove
{
	DWORD							dwPlazaID;							//广场标识
};

//房间人数
struct CMD_CS_S_ServerOnLine
{
	WORD							wServerID;							//房间标识
	DWORD							dwOnLineCount;						//在线人数
	DWORD							dwAndroidCount;						//机器人数
};

//房间修改
struct CMD_CS_S_ServerModify
{
	WORD							wSortID;							//排序索引
	WORD							wKindID;							//类型索引
	WORD							wNodeID;							//节点索引
	WORD							wServerID;							//房间标识
	WORD							wServerPort;						//房间端口
	DWORD							dwOnLineCount;						//在线人数
	DWORD							dwAndroidCount;						//机器人数
	DWORD							dwFullCount;						//满员人数
	TCHAR							szServerAddr[32];					//服务地址
	TCHAR							szServerName[LEN_SERVER];			//房间名称
};

//房间删除
struct CMD_CS_S_ServerRemove
{
	WORD							wServerID;							//房间标识
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//用户汇总

#define MDM_CS_USER_COLLECT			3									//用户汇总

//用户状态
#define SUB_CS_C_USER_ENTER			1									//用户进入
#define SUB_CS_C_USER_LEAVE			2									//用户离开
#define SUB_CS_C_USER_FINISH		3									//用户完成
#define SUB_CS_C_USER_STATUS		4									//用户状态

#define SUB_CS_C_CHAT_USER_ENTER	5									//聊天用户进入
#define SUB_CS_C_CHAT_USER_LEAVE	6									//聊天用户离开

#define SUB_CS_C_RADIO_MESSAGE		7									//广播消息

//用户状态
#define SUB_CS_S_COLLECT_REQUEST	100									//汇总请求

#define SUB_CS_S_USER_GAMESTATE		101									//用户状态

//聊点用户强制离线
#define SUB_CS_S_CHAT_FORCE_OFFLINE	102									//聊天用户强制离线

#define SUB_CS_S_RADIO_MESSAGE		103									//广播消息
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//用户进入
struct CMD_CS_C_UserEnter
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwGameID;							//游戏标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称

	//辅助信息
	BYTE							cbGender;							//用户性别
	BYTE							cbMemberOrder;						//会员等级
	BYTE							cbMasterOrder;						//管理等级

	BYTE							cbAndroidUser;						//机器人玩家

	//详细信息
	tagUserInfo						userInfo;							//用户信息
};

//用户离开
struct CMD_CS_C_UserLeave
{
	DWORD							dwUserID;							//用户标识
};

//用户进入
struct CMD_CS_C_ChatUserEnter
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
};

//用户离开
struct CMD_CS_C_ChatUserLeave
{
	DWORD							dwUserID;							//用户标识
};


//用户状态
struct CMD_CS_C_UserStatus
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	BYTE							cbUserStatus;						//用户状态
	WORD							wKindID;							//游戏标识
	WORD							wServerID;							//房间标识
	WORD							wTableID;							//桌子索引
	WORD							wChairID;							//椅子位置
};

//用户状态
struct CMD_CS_S_UserGameStatus
{
	DWORD							dwUserID;							//用户标识
	BYTE							cbGameStatus;						//用户状态
	WORD							wKindID;							//游戏标识
	WORD							wServerID;							//房间标识
	WORD							wTableID;							//桌子标识
	WORD							wChairID;							//椅子位置
	TCHAR							szServerName[LEN_SERVER];			//房间名称
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//远程服务

#define MDM_CS_REMOTE_SERVICE		4									//远程服务

//查找服务
#define SUB_CS_C_SEARCH_DATABASE	1									//数据查找
#define SUB_CS_C_SEARCH_CORRESPOND	2									//协调查找
#define SUB_CS_C_SEARCH_ALLCORRESPOND	3								//协调查找

//查找结果
#define SUB_CS_S_SEARCH_DATABASE	100									//数据查找
#define SUB_CS_S_SEARCH_CORRESPOND	101									//协调查找
#define SUB_CS_S_SEARCH_ALLCORRESPOND	102								//协调查找

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//协调查找
struct CMD_CS_C_SearchCorrespond
{
	//连接参数
	DWORD							dwSocketID;							//网络标识
	DWORD							dwClientAddr;						//连接地址

	//查找数据
	DWORD							dwGameID;							//游戏标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
};

//协调查找
struct CMD_CS_S_SearchCorrespond
{
	//连接信息
	DWORD							dwSocketID;							//网络标识
	DWORD							dwClientAddr;						//连接地址

	//数据信息
	WORD							wUserCount;							//用户数目
	tagUserRemoteInfo				UserRemoteInfo[16];					//用户信息
};


//协调查找
struct CMD_CS_C_AllSearchCorrespond
{
	//连接参数
	DWORD							dwSocketID;							//网络标识
	DWORD							dwClientAddr;						//连接地址

	DWORD							dwCount;
	DWORD							dwGameID[1];						//游戏标识
};

//协调查找
struct CMD_CS_S_SearchAllCorrespond
{
	//连接信息
	DWORD							dwSocketID;							//网络标识
	DWORD							dwClientAddr;						//连接地址

	//数据信息
	DWORD							dwCount;
	tagUserRemoteInfo				UserRemoteInfo[1];
};


/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//管理服务

#define MDM_CS_MANAGER_SERVICE		5									//管理服务

#define SUB_CS_C_SYSTEM_MESSAGE 	1									//系统消息

#define SUB_CS_S_SYSTEM_MESSAGE 	100									//系统消息

#define SUB_CS_S_PLATFORM_PARAMETER 300									//平台参数	



/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//机器服务

#define MDM_CS_ANDROID_SERVICE	    6									//机器服务

#define SUB_CS_C_ADDPARAMETER		100									//添加参数
#define SUB_CS_C_MODIFYPARAMETER    101									//修改参数
#define SUB_CS_C_DELETEPARAMETER	102									//删除参数	

#define SUB_CS_S_ADDPARAMETER		200									//添加参数
#define SUB_CS_S_MODIFYPARAMETER    201									//修改参数
#define SUB_CS_S_DELETEPARAMETER	202									//删除参数	

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//添加参数
struct CMD_CS_C_AddParameter
{
	WORD							wServerID;							//房间标识
	tagAndroidParameter				AndroidParameter;					//机器参数
};

//修改参数
struct CMD_CS_C_ModifyParameter
{
	WORD							wServerID;							//房间标识
	tagAndroidParameter				AndroidParameter;					//机器参数
};

//删除参数
struct CMD_CS_C_DeleteParameter
{
	WORD							wServerID;							//房间标识
	DWORD							dwBatchID;							//批次标识
};


//添加参数
struct CMD_CS_S_AddParameter
{	
	tagAndroidParameter				AndroidParameter;					//机器参数
};

//修改参数
struct CMD_CS_S_ModifyParameter
{
	tagAndroidParameter				AndroidParameter;					//机器参数
};

//删除参数
struct CMD_CS_S_DeleteParameter
{
	DWORD							dwBatchID;							//批次标识
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

#pragma pack()

#endif