#ifndef DATA_BASE_PACKET_HEAD_FILE
#define DATA_BASE_PACKET_HEAD_FILE

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//请求数据包

//用户事件
#define	DBR_GR_LOGON_USERID			100									//I D 登录
#define	DBR_GR_LOGON_MOBILE			101									//手机登录
#define DBR_GR_LOGON_MATCH			103									//比赛登录

//系统事件
#define DBR_GR_WRITE_GAME_SCORE		200									//游戏积分
#define DBR_GR_LEAVE_GAME_SERVER	201									//离开房间
#define DBR_GR_GAME_SCORE_RECORD	202									//积分记录
#define DBR_GR_MANAGE_USER_RIGHT	203									//权限管理
#define DBR_GR_LOAD_SYSTEM_MESSAGE	204									//系统消息
#define DBR_GR_LOAD_SENSITIVE_WORDS	205									//敏感词	
#define DBR_GR_UNLOCK_ANDROID_USER	206									//解锁机器人
#define DBR_GR_UNLOCK_USER			207									//解锁玩家

//配置事件
#define DBR_GR_LOAD_PARAMETER		300									//加载参数
#define DBR_GR_LOAD_ANDROID_PARAMETER 302								//加载参数
#define DBR_GR_LOAD_ANDROID_USER	303									//加载机器

#define DBR_GR_LOAD_BASEENSURE		308 								//加载低保 

//银行事件
#define DBR_GR_USER_ENABLE_INSURE	400									//开通银行
#define DBR_GR_USER_SAVE_SCORE		401									//存入游戏币
#define DBR_GR_USER_TAKE_SCORE		402									//提取游戏币
#define DBR_GR_USER_TRANSFER_SCORE	403									//转帐游戏币
#define DBR_GR_QUERY_INSURE_INFO	404									//查询银行
#define DBR_GR_QUERY_TRANSFER_USER_INFO	    405							//查询用户

//游戏事件
#define DBR_GR_GAME_FRAME_REQUEST	502									//游戏请求

//比赛事件
#define DBR_GR_MATCH_SIGNUP_START	602									//开始报名
#define DBR_GR_MATCH_QUERY_QUALIFY	603									//查询资格
#define DBR_GR_MATCH_BUY_SAFECARD	604									//购买保险卡	
#define DBR_GR_MATCH_START			605									//比赛开始
#define DBR_GR_MATCH_OVER			606									//比赛结束
#define DBR_GR_MATCH_CANCEL			607									//比赛取消
#define DBR_GR_MATCH_REWARD			608									//比赛奖励
#define DBR_GR_MATCH_ELIMINATE      609									//比赛淘汰
#define DBR_GR_MATCH_QUERY_REVIVE	610									//查询复活
#define DBR_GR_MATCH_USER_REVIVE	611									//用户复活
#define DBR_GR_MATCH_RECORD_GRADES	612									//记录成绩

//低保事件
#define DBR_GR_BASEENSURE_TAKE		1100								//领取低保

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//输出信息

//逻辑事件
#define DBO_GR_LOGON_SUCCESS		100									//登录成功
#define DBO_GR_LOGON_FAILURE		101									//登录失败

//配置事件
#define DBO_GR_GAME_PARAMETER		200									//配置信息
#define DBO_GR_GAME_ANDROID_PARAMETER 202								//参数信息
#define DBO_GR_GAME_ANDROID_INFO	203									//机器信息
#define DBO_GR_GAME_UNLOCK_ANDROID	204									//解锁机器

#define DBO_GR_BASEENSURE_PARAMETER	209									//低保信息

//银行命令
#define DBO_GR_USER_INSURE_INFO		300									//银行资料
#define DBO_GR_USER_INSURE_SUCCESS	301									//银行成功
#define DBO_GR_USER_INSURE_FAILURE	302									//银行失败
#define DBO_GR_USER_INSURE_USER_INFO   303								//用户资料
#define DBO_GR_USER_INSURE_ENABLE_RESULT 304							//开通结果

//游戏事件
#define DBO_GR_GAME_FRAME_RESULT	402									//游戏结果

//比赛事件
#define DBO_GR_MATCH_EVENT_START	500									//比赛标识
#define DBO_GR_MATCH_UNSIGNUP_RESULT 502								//退赛结果
#define DBO_GR_MATCH_QUERY_RESULT	503									//查询结果
#define DBO_GR_MATCH_BUY_RESULT		504									//购买结果
#define DBO_GR_MATCH_REVIVE_INFO	505									//复活信息
#define DBO_GR_MATCH_REVIVE_RESULT	506									//复活结果		
#define DBO_GR_MATCH_RANK_LIST		507									//比赛排行
#define DBO_GR_MATCH_REWARD_RESULT  508									//奖励结果
#define DBO_GR_MATCH_CANCEL_RESULT	509									//比赛取消
#define DBO_GR_MATCH_EVENT_END		599									//比赛标识

//系统事件
#define DBO_GR_SYSTEM_MESSAGE_RESULT	600								//消息结果
#define DBO_GR_SYSTEM_MESSAGE_FINISH	601								//消息完成	
#define DBO_GR_SENSITIVE_WORDS		    602								//敏感词


//等级命令

#define DBO_GR_UPDATE_CONTROLINFO	900									//更新控制



#define DBO_GR_BASEENSURE_RESULT	1100								//领取结果


/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//ID 登录
struct DBR_GR_LogonUserID
{		
	//登录信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码

	//附加信息
	DWORD							dwMatchID;							//比赛标识
	LONGLONG						lMatchNo;							//比赛编号
	BYTE							cbMatchType;						//比赛类型
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//ID 登录
struct DBR_GR_LogonMobile
{		
	//登录信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	BYTE                            cbDeviceType;                       //设备类型
	WORD                            wBehaviorFlags;                     //行为标识
	WORD                            wPageTableCount;                    //分页桌数

	//附加信息
	DWORD							dwMatchID;							//比赛标识
	LONGLONG						lMatchNo;							//比赛编号
	BYTE							cbMatchType;						//比赛类型
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//帐号登录
struct DBR_GR_LogonAccounts
{
	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//游戏积分
struct DBR_GR_WriteGameScore
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwClientAddr;						//连接地址

	SCORE							lStorageCurrent;					//库存当前值

	//用户信息
	DWORD							dwDBQuestID;						//请求标识
	DWORD							dwInoutIndex;						//进出索引

	//成绩变量
	tagVariationInfo				VariationInfo;						//提取信息

	//比赛信息
	DWORD							dwMatchID;							//比赛标识
	LONGLONG						lMatchNo;							//比赛场次
};

//离开房间
struct DBR_GR_LeaveGameServer
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwInoutIndex;						//记录索引
	DWORD							dwLeaveReason;						//离开原因
	DWORD							dwOnLineTimeCount;					//在线时长

	//成绩变量
	tagVariationInfo				RecordInfo;							//记录信息
	tagVariationInfo				VariationInfo;						//提取信息

	//系统信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};
//更新控制
struct DBO_GR_UpdateControlInfo
{
	DWORD							dwUserID;							//用户标识
	int								nWinRate;							//所有房间获胜概率
	int								nRoomWinRate;						//该房间获胜概率
	BYTE							cbNullity;							//是否冻结
	DWORD							dwTailGameID;						//跟踪ID
	SCORE							lTotalScore;						//玩家总输赢
	SCORE							lTdTotalScore;						//玩家当天总输赢
	bool							bRegister;							//是否登录
};
//游戏记录
struct DBR_GR_GameScoreRecord
{
	//桌子信息
	WORD							wTableID;							//桌子号码
	WORD							wUserCount;							//用户数目
	WORD							wAndroidCount;						//机器数目

	//损耗税收
	SCORE							lWasteCount;						//损耗数目
	SCORE							lRevenueCount;						//税收数目

	//统计信息
	DWORD							dwUserMemal;						//奖牌数目
	DWORD							dwPlayTimeCount;					//游戏时间

	//时间信息
	SYSTEMTIME						SystemTimeStart;					//开始时间
	SYSTEMTIME						SystemTimeConclude;					//结束时间

	//积分记录
	WORD							wRecordCount;						//记录数目
	tagGameScoreRecord				GameScoreRecord[MAX_CHAIR];			//游戏记录
};

//开通银行
struct DBR_GR_UserEnableInsure
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户 I D	
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szLogonPass[LEN_PASSWORD];			//登录密码
	TCHAR							szInsurePass[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//存入游戏币
struct DBR_GR_UserSaveScore
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户 I D
	SCORE							lSaveScore;							//存入游戏币
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//取出游戏币
struct DBR_GR_UserTakeScore
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户 I D
	SCORE							lTakeScore;							//提取游戏币
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//转帐游戏币
struct DBR_GR_UserTransferScore
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址	
	SCORE							lTransferScore;						//转帐游戏币
	TCHAR							szAccounts[LEN_NICKNAME];			//目标用户
	TCHAR							szPassword[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	TCHAR							szTransRemark[LEN_TRANS_REMARK];	//转帐备注
};

//加载机器
struct DBR_GR_LoadAndroidUser
{
	DWORD							dwBatchID;							//批次标识
	DWORD							dwAndroidCount;						//机器数目
	DWORD							AndroidCountMember0;				//普通会员	
	DWORD							AndroidCountMember1;				//一级会员
	DWORD							AndroidCountMember2;				//二级会员
	DWORD							AndroidCountMember3;				//三级会员
	DWORD							AndroidCountMember4;				//四级会员
	DWORD							AndroidCountMember5;				//五级会员
};

//查询银行
struct DBR_GR_QueryInsureInfo
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//银行密码
};

//查询用户
struct DBR_GR_QueryTransferUserInfo
{
	BYTE                            cbActivityGame;                     //游戏动作
	BYTE                            cbByNickName;                       //昵称赠送
	TCHAR							szAccounts[LEN_ACCOUNTS];			//目标用户
	DWORD							dwUserID;							//用户 I D
};

//查询资格
struct DBR_GR_QueryMatchQualify
{	
	DWORD							dwUserID;							//用户ID
	DWORD							dwMatchID;							//比赛ID
	SCORE							lMatchNo;							//比赛场次
};

//购买保险
struct DBR_GR_MatchBuySafeCard
{
	DWORD							dwUserID;							//用户ID
	DWORD							dwMatchID;							//比赛ID
	SCORE							lMatchNo;							//比赛场次	
	SCORE							lSafeCardFee;						//保险费用		
};

//比赛报名
struct DBR_GR_MatchSignup
{
	//报名费用
	DWORD							dwUserID;							//用户ID	
	SCORE							lInitScore;							//初始分数
	BYTE							cbSignupMode;						//报名方式
	WORD							wMaxSignupUser;						//最大人数

	//系统信息
	DWORD							dwInoutIndex;						//进入标识
	DWORD							dwClientAddr;						//连接地址
	DWORD							dwMatchID;							//比赛ID
	SCORE							lMatchNo;							//比赛场次
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//取消报名
struct DBR_GR_MatchUnSignup
{
	//报名费用
	DWORD							dwReason;							//取消原因
	DWORD							dwUserID;							//用户ID

	//系统信息
	DWORD							dwInoutIndex;						//进入标识
	DWORD							dwClientAddr;						//连接地址
	DWORD							dwMatchID;							//比赛ID
	SCORE							lMatchNo;							//比赛场次
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//比赛开始
struct DBR_GR_MatchStart
{
	WORD							wServerID;							//房间标识
	BYTE							cbMatchType;						//比赛类型
	DWORD							dwMatchID;							//比赛标识
	SCORE							lMatchNo;							//比赛场次	
};

//报名开始
struct DBR_GR_SignupStart
{	
	WORD							wServerID;							//房间标识
	BYTE							cbMatchType;						//比赛类型
	DWORD							dwMatchID;							//比赛标识
	SCORE							lMatchNo;							//比赛场次	
	BYTE							cbSignupMode;						//报名方式
};

//比赛结束
struct DBR_GR_MatchOver
{	
	WORD							wServerID;							//房间标识
	BYTE							cbMatchType;						//比赛类型
	BOOL							bMatchFinish;						//比赛完成
	WORD							wPlayCount;							//游戏局数
	DWORD							dwMatchID;							//比赛标识
	SCORE							lMatchNo;							//比赛场次	
	SYSTEMTIME						MatchStartTime;						//开始时间
	SYSTEMTIME						MatchEndTime;						//结束时间
};

//比赛取消
struct DBR_GR_MatchCancel
{
	BOOL							bMatchFinish;						//比赛完成
	WORD							wServerID;							//房间标识
	DWORD							dwMatchID;							//比赛标识
	SCORE							lMatchNo;							//比赛场次
	SCORE							lSafeCardFee;						//保险费用
};

//比赛淘汰
struct DBR_GR_MatchEliminate
{
	DWORD							dwUserID;							//用户标识
	WORD							wServerID;							//房间标识
	BYTE							cbMatchType;						//比赛类型
	DWORD							dwMatchID;							//比赛标识
	SCORE							lMatchNo;							//比赛场次	
};

//查询复活
struct DBR_GR_MatchQueryRevive
{
	DWORD							dwUserID;							//用户标识
	DWORD							dwMatchID;							//比赛标识
	SCORE							lMatchNo;							//比赛场次		
};

//用户复活
struct DBR_GR_MatchUserRevive
{
	//比赛信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwMatchID;							//比赛标识
	SCORE							lMatchNo;							//比赛场次

	//分数配置
	SCORE							lInitScore;							//初始分数
	SCORE							lCullScore;							//淘汰分数			

	//复活配置
	BYTE							cbReviveTimes;						//复活次数
	SCORE							lReviveFee;							//复活费用	
};

//比赛奖励
struct DBR_GR_MatchReward
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lRewardGold;						//奖励金币
	SCORE							lRewardIngot;						//奖励元宝
	DWORD							dwRewardExperience;					//奖励经验
	DWORD							dwClientAddr;						//连接地址
};

//记录成绩
struct DBR_GR_MatchRecordGrades
{
	//比赛信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwMatchID;							//比赛标识
	WORD							wServerID;							//房间标识
	SCORE							lMatchNo;							//比赛场次
	SCORE							lInitScore;							//初始分数
};

//领取低保
struct DBR_GR_TakeBaseEnsure
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//登录成功
struct DBO_GR_LogonSuccess
{
	//属性资料
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwGroupID;							//社团 I D
	DWORD							dwCustomID;							//自定头像
	TCHAR							szNickName[LEN_NICKNAME];			//帐号昵称
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字

	//用户资料
	BYTE							cbGender;							//用户性别
	BYTE							cbMemberOrder;						//会员等级
	BYTE							cbMasterOrder;						//管理等级
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名

	//附属资料
	TCHAR							szQQ[LEN_QQ];						//用户QQ
	TCHAR							szEMail[LEN_EMAIL];					//用户Eamil
	TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//座机号码
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//手机号码	
	TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//联系地址
	TCHAR							szPostalCode[LEN_POSTALCODE];		//邮政编码

	//积分信息
	SCORE							lScore;								//用户分数
	SCORE							lIngot;								//用户元宝
	SCORE							lGrade;								//用户成绩
	SCORE							lInsure;							//用户银行	
	DOUBLE							dBeans;								//用户游戏豆

	//游戏信息
	DWORD							dwWinCount;							//胜利盘数
	DWORD							dwLostCount;						//失败盘数
	DWORD							dwDrawCount;						//和局盘数
	DWORD							dwFleeCount;						//逃跑盘数
	DWORD							dwExperience;						//用户经验
	LONG							lLoveLiness;						//用户魅力
	SCORE							lIntegralCount;						//积分总数(当前房间)

	//代理信息
	DWORD							dwAgentID;							//代理 I D

	//用户权限
	DWORD							dwUserRight;						//用户权限
	DWORD							dwMasterRight;						//管理权限

	//索引变量
	DWORD							dwInoutIndex;						//记录索引
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识

	//手机定义
	BYTE                            cbDeviceType;                       //设备类型
	WORD                            wBehaviorFlags;                     //行为标识
	WORD                            wPageTableCount;                    //分页桌数

	//辅助信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szDescribeString[128];				//错误消息
};

//登录失败
struct DBO_GR_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//错误消息

	DWORD							dwLockKindID;						//锁住房间的游戏KindID
	DWORD							dwLockServerID;						//锁住房间的房间ServerID
};

//配置信息
struct DBO_GR_GameParameter
{
	//汇率信息
	DWORD							dwMedalRate;						//奖牌汇率
	DWORD							dwRevenueRate;						//银行税收
	DWORD							dwExchangeRate;						//兑换比率
	DWORD							dwPresentExchangeRate;				//魅力游戏币兑换率
	DWORD							dwRateGold;							//游戏豆游戏币兑换率
	SCORE							lHallNoticeThreshold;				//大厅通知金币阈值

	//经验奖励
	DWORD							dwWinExperience;					//赢局经验

	//版本信息
	DWORD							dwClientVersion;					//客户版本
	DWORD							dwServerVersion;					//服务版本

	SCORE							lEducateGrantScore;					//练习赠送
};

//列表信息
struct DBO_GR_GameColumnInfo
{
	LONG							lResultCode;						//结果代码
	BYTE							cbColumnCount;						//列表数目
	tagColumnItem					ColumnItemInfo[MAX_COLUMN];			//列表信息
};

//机器信息
struct DBO_GR_GameAndroidParameter
{
	LONG							lResultCode;						//结果代码
	WORD							wParameterCount;					//参数数目
	tagAndroidParameter				AndroidParameter[MAX_BATCH];		//机器信息
};

//机器信息
struct DBO_GR_GameAndroidInfo
{
	LONG							lResultCode;						//结果代码
	DWORD							dwBatchID;							//批次标识
	WORD							wAndroidCount;						//用户数目
	tagAndroidAccountsInfo			AndroidAccountsInfo[MAX_ANDROID];	//机器帐号
};
//////////////////////////////////////////////////////////////////////////

//低保参数
struct DBO_GR_BaseEnsureParameter
{
	SCORE							lScoreCondition;					//游戏币条件
	SCORE							lScoreAmount;						//游戏币数量
	BYTE							cbTakeTimes;						//领取次数	
};

//低保结果
struct DBO_GR_BaseEnsureResult
{
	bool							bSuccessed;							//成功标识
	SCORE							lGameScore;							//当前游戏币
	TCHAR							szNotifyContent[128];				//提示内容
};

//银行资料
struct DBO_GR_UserInsureInfo
{
	BYTE                            cbActivityGame;                     //游戏动作
	BYTE							cbEnjoinTransfer;					//转帐开关
	WORD							wRevenueTake;						//税收比例
	WORD							wRevenueTransfer;					//税收比例
	WORD							wRevenueTransferMember;				//税收比例
	WORD							wServerID;							//房间标识
	SCORE							lUserScore;							//用户游戏币
	SCORE							lUserInsure;						//银行游戏币
	SCORE							lTransferPrerequisite;				//转帐条件
};

//银行成功
struct DBO_GR_UserInsureSuccess
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户 I D
	SCORE							lSourceScore;						//原来游戏币
	SCORE							lSourceInsure;						//原来游戏币
	SCORE							lInsureRevenue;						//银行税收
	SCORE							lFrozenedScore;						//冻结积分
	SCORE							lVariationScore;					//游戏币变化
	SCORE							lVariationInsure;					//银行变化
	TCHAR							szDescribeString[128];				//描述消息
};

//银行失败
struct DBO_GR_UserInsureFailure
{
	BYTE                            cbActivityGame;                     //游戏动作
	LONG							lResultCode;						//操作代码
	SCORE							lFrozenedScore;						//冻结积分
	TCHAR							szDescribeString[128];				//描述消息
};

//用户资料
struct DBO_GR_UserTransferUserInfo
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwGameID;							//用户 I D
	TCHAR							szAccounts[LEN_ACCOUNTS];			//帐号昵称
};

//开通结果
struct DBO_GR_UserInsureEnableResult
{
	BYTE                            cbActivityGame;                     //游戏动作
	BYTE							cbInsureEnabled;					//使能标识
	TCHAR							szDescribeString[128];				//描述消息
};

//报名结果
struct DBO_GR_MatchSingupResult
{
	bool							bResultCode;						//结果代码
	BYTE							cbSignupMode;						//报名方式
	DWORD							dwReason;							//取消原因
	DWORD							dwUserID;							//用户标识
	SCORE							lMatchNo;							//比赛场次
	SCORE							lCurrGold;							//当前金币
	SCORE							lCurrIngot;							//当前元宝
	TCHAR							szDescribeString[128];				//描述消息
};

//查询结果
struct DBO_GR_MatchQueryResult
{
	bool							bHoldQualify;						//拥有资格
};

//购买结果
struct DBO_GR_MatchBuyResult
{
	bool							bSuccessed;							//成功标识
	SCORE							lCurrGold;							//当前金币
	TCHAR							szDescribeString[128];				//描述消息
};

//复活信息
struct DBO_GR_MatchReviveInfo
{	
	bool							bSuccessed;							//成功标识
	bool							bSafeCardRevive;					//保险卡复活
	BYTE							cbReviveTimesed;					//复活次数
};

//复活结果
struct DBO_GR_MatchReviveResult
{
	bool							bSuccessed;							//成功标识
	SCORE							lCurrGold;							//当前金币	
	TCHAR							szDescribeString[128];				//描述消息
};

//比赛排行
struct DBO_GR_MatchRankList
{
	BOOL							bMatchFinish;						//比赛完成
	WORD							wUserCount;							//用户数目
	DWORD							dwMatchID;							//比赛标识
	SCORE							lMatchNo;							//比赛场次
	tagMatchRankInfo				MatchRankInfo[128];					//奖励信息
};

//奖励结果
struct DBO_GR_MatchRewardResult
{
	bool							bResultCode;						//结果代码
	DWORD							dwUserID;							//用户标识
	SCORE							lCurrGold;							//当前金币
};

//取消结果
struct DBO_GR_MatchCancelResult
{
	bool							bSuccessed;							//成功标识
	BOOL							bMatchFinish;						//比赛完成
};

//权限管理
struct DBR_GR_ManageUserRight
{
	DWORD							dwUserID;							//目标用户
	DWORD							dwAddRight;							//添加权限
	DWORD							dwRemoveRight;						//删除权限
	bool							bGameRight;							//游戏权限
};

//权限管理
struct DBR_GR_ManageMatchRight
{	
	DWORD							dwUserID;							//目标用户
	DWORD							dwAddRight;							//添加权限
	DWORD							dwRemoveRight;						//删除权限	
	DWORD							dwMatchID;							//比赛标识
	DWORD							dwMatchNO;							//比赛场次
};

//系统消息
struct DBR_GR_SystemMessage
{
	DWORD                           dwMessageID;                        //消息ID
	BYTE							cbMessageType;						//消息类型
	BYTE                            cbAllRoom;                          //全体房间
	DWORD							dwTimeRate;						    //时间频率
	__time64_t						tStartTime;							//开始时间
	__time64_t                      tConcludeTime;                      //结束时间
	TCHAR							szSystemMessage[LEN_USER_CHAT];		//系统消息
};

//解锁机器人
struct DBR_GR_UnlockAndroidUser
{
	WORD							wServerID;							//房间ID
	WORD							wBatchID;							//批次ID
};
//参与信息
struct DBR_GR_Deblocking
{
	DWORD							dwUserID;							//用户标识
	DWORD							dwServerID;							//桌子 I D
};
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

#endif