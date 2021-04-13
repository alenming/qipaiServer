#ifndef CMD_GAME_SERVER_HEAD_FILE
#define CMD_GAME_SERVER_HEAD_FILE

#pragma pack(1)

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//登录命令

#define MDM_GR_LOGON				1									//登录信息

//登录模式
#define SUB_GR_LOGON_USERID			1									//I D 登录
#define SUB_GR_LOGON_MOBILE			2									//手机登录

//登录结果
#define SUB_GR_LOGON_SUCCESS		100									//登录成功
#define SUB_GR_LOGON_FAILURE		101									//登录失败
#define SUB_GR_LOGON_FINISH			102									//登录完成

//升级提示
#define SUB_GR_UPDATE_NOTIFY		200									//升级提示

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//I D 登录
struct CMD_GR_LogonUserID
{
	//版本信息
	DWORD							dwPlazaVersion;						//广场版本
	DWORD							dwFrameVersion;						//框架版本
	DWORD							dwProcessVersion;					//进程版本

	//登录信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR                           szServerPasswd[LEN_PASSWORD];       //房间密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	WORD							wKindID;							//类型索引
};

//手机登录
struct CMD_GR_LogonMobile
{
	//版本信息
	WORD							wGameID;							//游戏标识
	DWORD							dwProcessVersion;					//进程版本

	//桌子区域
	BYTE                            cbDeviceType;                       //设备类型
	WORD                            wBehaviorFlags;                     //行为标识
	WORD                            wPageTableCount;                    //分页桌数

	//登录信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR                           szServerPasswd[LEN_PASSWORD];       //房间密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//帐号登录
struct CMD_GR_LogonAccounts
{
	//版本信息
	DWORD							dwPlazaVersion;						//广场版本
	DWORD							dwFrameVersion;						//框架版本
	DWORD							dwProcessVersion;					//进程版本

	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//登录成功
struct CMD_GR_LogonSuccess
{
	DWORD							dwUserRight;						//用户权限
	DWORD							dwMasterRight;						//管理权限
};

//登录失败
struct CMD_GR_LogonFailure
{
	LONG							lErrorCode;							//错误代码

	DWORD							dwLockKindID;						//锁住房间的游戏KindID
	DWORD							dwLockServerID;						//锁住房间的房间ServerID

	TCHAR							szDescribeString[128];				//描述消息
};

//升级提示
struct CMD_GR_UpdateNotify
{
	//升级标志
	BYTE							cbMustUpdatePlaza;					//强行升级
	BYTE							cbMustUpdateClient;					//强行升级
	BYTE							cbAdviceUpdateClient;				//建议升级

	//当前版本
	DWORD							dwCurrentPlazaVersion;				//当前版本
	DWORD							dwCurrentFrameVersion;				//当前版本
	DWORD							dwCurrentClientVersion;				//当前版本
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//配置命令

#define MDM_GR_CONFIG				2									//配置信息

#define SUB_GR_CONFIG_SERVER		101									//房间配置
#define SUB_GR_CONFIG_FINISH		103									//配置完成
#define SUB_GR_CONFIG_USER_RIGHT	104									//玩家权限

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//列表配置
struct CMD_GR_ConfigColumn
{
	BYTE							cbColumnCount;						//列表数目
	tagColumnItem					ColumnItem[MAX_COLUMN];				//列表描述
};

//房间配置
struct CMD_GR_ConfigServer
{
	//房间属性
	WORD							wTableCount;						//桌子数目
	WORD							wChairCount;						//椅子数目

	//房间配置
	WORD							wServerType;						//房间类型
	DWORD							dwServerRule;						//房间规则
};


//玩家权限
struct CMD_GR_ConfigUserRight
{
	DWORD							dwUserRight;						//玩家权限
};
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//用户命令

#define MDM_GR_USER					3									//用户信息

//用户动作
#define SUB_GR_USER_RULE			1									//用户规则
#define SUB_GR_USER_LOOKON			2									//旁观请求
#define SUB_GR_USER_SITDOWN			3									//坐下请求
#define SUB_GR_USER_STANDUP			4									//起立请求
#define SUB_GR_USER_INVITE			5									//用户邀请
#define SUB_GR_USER_INVITE_REQ		6									//邀请请求
#define SUB_GR_USER_REPULSE_SIT  	7									//拒绝玩家坐下
#define SUB_GR_USER_KICK_USER       8                                   //踢出用户
#define SUB_GR_USER_INFO_REQ        9                                   //请求用户信息
#define SUB_GR_USER_CHAIR_REQ       10                                  //请求更换位置
#define SUB_GR_USER_CHAIR_INFO_REQ  11                                  //请求椅子用户信息
#define SUB_GR_USER_WAIT_DISTRIBUTE 12									//等待分配

//用户状态
#define SUB_GR_USER_ENTER			100									//用户进入
#define SUB_GR_USER_SCORE			101									//用户分数
#define SUB_GR_USER_STATUS			102									//用户状态
#define SUB_GR_REQUEST_FAILURE		103									//请求失败
#define SUB_GR_USER_GAME_DATA		104									//用户游戏数据
#define SUB_GR_USER_HALL_SCORE		105									//给退出游戏的用户刷新金币

//聊天命令
#define SUB_GR_USER_CHAT			201									//聊天消息
#define SUB_GR_USER_EXPRESSION		202									//表情消息
#define SUB_GR_WISPER_CHAT			203									//私聊消息
#define SUB_GR_WISPER_EXPRESSION	204									//私聊表情
#define SUB_GR_COLLOQUY_CHAT		205									//会话消息
#define SUB_GR_COLLOQUY_EXPRESSION	206									//会话表情

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//旁观请求
struct CMD_GR_UserLookon
{
	WORD							wTableID;							//桌子位置
	WORD							wChairID;							//椅子位置
};

//坐下请求
struct CMD_GR_UserSitDown
{
	WORD							wTableID;							//桌子位置
	WORD							wChairID;							//椅子位置
	TCHAR							szPassword[LEN_PASSWORD];			//桌子密码
};

//起立请求
struct CMD_GR_UserStandUp
{
	WORD							wTableID;							//桌子位置
	WORD							wChairID;							//椅子位置
	BYTE							cbForceLeave;						//强行离开
};

//邀请用户 
struct CMD_GR_UserInvite
{
	WORD							wTableID;							//桌子号码
	DWORD							dwUserID;							//用户 I D
};

//邀请用户请求 
struct CMD_GR_UserInviteReq
{
	WORD							wTableID;							//桌子号码
	DWORD							dwUserID;							//用户 I D
};

//用户分数
struct CMD_GR_UserScore
{
	DWORD							dwUserID;							//用户标识
	tagUserScore					UserScore;							//积分信息
};

//用户分数
struct CMD_GR_MobileUserScore
{
	DWORD							dwUserID;							//用户标识
	tagMobileUserScore				UserScore;							//积分信息
};

//退出游戏用户分数
struct CMD_GR_UserHallScore
{
	DWORD							dwUserID;							//用户标识
	SCORE							lUserScore;							//积分信息
};

//用户状态
struct CMD_GR_UserStatus
{
	DWORD							dwUserID;							//用户标识
	tagUserStatus					UserStatus;							//用户状态
};

//请求失败
struct CMD_GR_RequestFailure
{
	LONG							lErrorCode;							//错误代码
	TCHAR							szDescribeString[256];				//描述信息
};


//用户聊天
struct CMD_GR_C_UserChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
};

//用户聊天
struct CMD_GR_S_UserChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
};

//用户表情
struct CMD_GR_C_UserExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
};

//用户表情
struct CMD_GR_S_UserExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
};

//用户私聊
struct CMD_GR_C_WisperChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
};

//用户私聊
struct CMD_GR_S_WisperChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
};

//私聊表情
struct CMD_GR_C_WisperExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
};

//私聊表情
struct CMD_GR_S_WisperExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
};

//用户会话
struct CMD_GR_ColloquyChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwConversationID;					//会话标识
	DWORD							dwTargetUserID[16];					//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
};

//邀请用户
struct CMD_GR_C_InviteUser
{
	WORD							wTableID;							//桌子号码
	DWORD							dwSendUserID;						//发送用户
};

//邀请用户
struct CMD_GR_S_InviteUser
{
	DWORD							dwTargetUserID;						//目标用户
};

//用户拒绝黑名单坐下
struct CMD_GR_UserRepulseSit
{
	WORD							wTableID;							//桌子号码
	WORD							wChairID;							//椅子位置
	DWORD							dwUserID;							//用户 I D
	DWORD							dwRepulseUserID;					//用户 I D
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//规则标志
#define UR_LIMIT_SAME_IP			0x01								//限制地址
#define UR_LIMIT_WIN_RATE			0x02								//限制胜率
#define UR_LIMIT_FLEE_RATE			0x04								//限制逃率
#define UR_LIMIT_GAME_SCORE			0x08								//限制积分

//用户规则
struct CMD_GR_UserRule
{
	BYTE							cbRuleMask;							//规则掩码
	WORD							wMinWinRate;						//最低胜率
	WORD							wMaxFleeRate;						//最高逃率
	LONG							lMaxGameScore;						//最高分数 
	LONG							lMinGameScore;						//最低分数
};

//请求用户信息
struct CMD_GR_UserInfoReq
{
	DWORD                           dwUserIDReq;                        //请求用户
	WORD							wTablePos;							//桌子位置
};

//请求用户信息
struct CMD_GR_ChairUserInfoReq
{
	WORD							wTableID;							//桌子号码
	WORD							wChairID;							//椅子位置
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//状态命令

#define MDM_GR_STATUS				4									//状态信息

#define SUB_GR_TABLE_INFO			100									//桌子信息
#define SUB_GR_TABLE_STATUS			101									//桌子状态

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//桌子信息
struct CMD_GR_TableInfo
{
	WORD							wTableCount;						//桌子数目
	tagTableStatus					TableStatusArray[512];				//桌子状态
};

//桌子状态
struct CMD_GR_TableStatus
{
	WORD							wTableID;							//桌子号码
	tagTableStatus					TableStatus;						//桌子状态
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//银行命令

#define MDM_GR_INSURE				5									//用户信息

//银行命令
#define SUB_GR_ENABLE_INSURE_REQUEST 1									//开通银行
#define SUB_GR_QUERY_INSURE_INFO	2									//查询银行
#define SUB_GR_SAVE_SCORE_REQUEST	3									//存款操作
#define SUB_GR_TAKE_SCORE_REQUEST	4									//取款操作
#define SUB_GR_TRANSFER_SCORE_REQUEST	5								//取款操作
#define SUB_GR_QUERY_USER_INFO_REQUEST	6								//查询用户

#define SUB_GR_USER_INSURE_INFO		100									//银行资料
#define SUB_GR_USER_INSURE_SUCCESS	101									//银行成功
#define SUB_GR_USER_INSURE_FAILURE	102									//银行失败
#define SUB_GR_USER_TRANSFER_USER_INFO	103								//用户资料
#define SUB_GR_USER_INSURE_ENABLE_RESULT 104							//开通结果

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//开通银行
struct CMD_GR_C_EnableInsureRequest
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户I D
	TCHAR							szLogonPass[LEN_PASSWORD];			//登录密码
	TCHAR							szInsurePass[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//查询银行
struct CMD_GR_C_QueryInsureInfoRequest
{
	BYTE                            cbActivityGame;                     //游戏动作
	TCHAR							szInsurePass[LEN_PASSWORD];			//银行密码
};

//存款请求
struct CMD_GR_C_SaveScoreRequest
{
	BYTE                            cbActivityGame;                     //游戏动作
	SCORE							lSaveScore;							//存款数目
};

//取款请求
struct CMD_GR_C_TakeScoreRequest
{
	BYTE                            cbActivityGame;                     //游戏动作
	SCORE							lTakeScore;							//取款数目
	TCHAR							szInsurePass[LEN_PASSWORD];			//银行密码
};

//转账金币
struct CMD_GP_C_TransferScoreRequest
{
	BYTE                            cbActivityGame;                     //游戏动作
	SCORE							lTransferScore;						//转账金币
	TCHAR							szAccounts[LEN_ACCOUNTS];			//目标用户
	TCHAR							szInsurePass[LEN_PASSWORD];			//银行密码
	TCHAR							szTransRemark[LEN_TRANS_REMARK];	//转账备注
};

//查询用户
struct CMD_GR_C_QueryUserInfoRequest
{
	BYTE                            cbActivityGame;                     //游戏动作
	BYTE                            cbByNickName;                       //昵称赠送
	TCHAR							szAccounts[LEN_ACCOUNTS];			//目标用户
};

//银行资料
struct CMD_GR_S_UserInsureInfo
{
	BYTE                            cbActivityGame;                     //游戏动作
	BYTE							cbEnjoinTransfer;					//转账开关
	WORD							wRevenueTake;						//税收比例
	WORD							wRevenueTransfer;					//税收比例
	WORD							wRevenueTransferMember;				//税收比例	
	WORD							wServerID;							//房间标识
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
	SCORE							lTransferPrerequisite;				//转账条件
};

//银行成功
struct CMD_GR_S_UserInsureSuccess
{
	BYTE                            cbActivityGame;                     //游戏动作
	SCORE							lUserScore;							//身上金币
	SCORE							lUserInsure;						//银行金币
	TCHAR							szDescribeString[128];				//描述消息
};

//银行失败
struct CMD_GR_S_UserInsureFailure
{
	BYTE                            cbActivityGame;                     //游戏动作
	LONG							lErrorCode;							//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//用户信息
struct CMD_GR_S_UserTransferUserInfo
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwTargetGameID;						//目标用户
	TCHAR							szAccounts[LEN_ACCOUNTS];			//目标用户
};

//开通结果
struct CMD_GR_S_UserInsureEnableResult
{
	BYTE                            cbActivityGame;                     //游戏动作
	BYTE							cbInsureEnabled;					//使能标识
	TCHAR							szDescribeString[128];				//描述消息
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//管理命令

#define MDM_GR_MANAGE				9									//管理命令

#define SUB_GR_SEND_WARNING			1									//发送警告
#define SUB_GR_SEND_MESSAGE			2									//发送消息
#define SUB_GR_LOOK_USER_IP			3									//查看地址
#define SUB_GR_KILL_USER			4									//踢出用户
#define SUB_GR_LIMIT_ACCOUNS		5									//禁用帐户
#define SUB_GR_SET_USER_RIGHT		6									//权限设置

//房间设置
#define SUB_GR_QUERY_OPTION			7									//查询设置
#define SUB_GR_OPTION_SERVER		8									//房间设置
#define SUB_GR_OPTION_CURRENT		9									//当前设置
#define SUB_GR_LIMIT_USER_CHAT		10									//限制聊天
#define SUB_GR_KICK_ALL_USER		11									//踢出用户
#define SUB_GR_DISMISSGAME		    12									//解散游戏

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//发送警告
struct CMD_GR_SendWarning
{
	WORD							wChatLength;						//信息长度
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szWarningMessage[LEN_USER_CHAT];	//警告消息
};

//系统消息
struct CMD_GR_SendMessage
{
	BYTE							cbGame;								//是否将消息广播至游戏
	BYTE							cbRoom;								//是否将消息广播至房间（桌子界面）
	BYTE							cbAllRoom;							//是否将消息广播至所有游戏
	BYTE                            cbLoop;                             //循环标志
	DWORD                           dwTimeRate;                         //循环间隔
	__time64_t                      tConcludeTime;                      //结束时间
	WORD							wChatLength;						//信息长度
	TCHAR							szSystemMessage[LEN_USER_CHAT];		//系统消息
};

//查看地址
struct CMD_GR_LookUserIP
{
	DWORD							dwTargetUserID;						//目标用户
};

//踢出用户
struct CMD_GR_KickUser
{
	DWORD							dwTargetUserID;						//目标用户
};

//禁用帐户
struct CMD_GR_LimitAccounts
{
	DWORD							dwTargetUserID;						//目标用户
};

//权限设置
struct CMD_GR_SetUserRight
{
	//目标用户
	DWORD							dwTargetUserID;						//目标用户

	//绑定变量
	BYTE							cbGameRight;						//帐号权限
	BYTE							cbAccountsRight;					//帐号权限

	//权限变化
	BYTE							cbLimitRoomChat;					//大厅聊天
	BYTE							cbLimitGameChat;					//游戏聊天
	BYTE							cbLimitPlayGame;					//游戏权限
	BYTE							cbLimitSendWisper;					//发送消息
	BYTE							cbLimitLookonGame;					//旁观权限
};

//房间设置
struct CMD_GR_OptionCurrent
{
	DWORD							dwRuleMask;							//规则掩码
	tagServerOptionInfo				ServerOptionInfo;					//房间配置
};

//房间设置
struct CMD_GR_ServerOption
{
	tagServerOptionInfo				ServerOptionInfo;					//房间配置
};

//踢出所有用户
struct CMD_GR_KickAllUser
{
	TCHAR							szKickMessage[LEN_USER_CHAT];		//踢出提示
};

//解散游戏
struct CMD_GR_DismissGame
{
	WORD							wDismissTableNum;		            //解散桌号
};
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//设置标志
#define OSF_ROOM_CHAT				1									//大厅聊天
#define OSF_GAME_CHAT				2									//游戏聊天
#define OSF_ROOM_WISPER				3									//大厅私聊
#define OSF_ENTER_TABLE				4									//进入游戏
#define OSF_ENTER_SERVER			5									//进入房间

//房间设置
struct CMD_GR_OptionServer
{
	BYTE							cbOptionFlags;						//设置标志
	BYTE							cbOptionValue;						//设置标志
};

//限制聊天
struct CMD_GR_LimitUserChat
{
	DWORD							dwTargetUserID;						//目标用户
	BYTE							cbLimitFlags;						//限制标志
	BYTE							cbLimitValue;						//限制与否
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//比赛命令

#define MDM_GR_MATCH				10									//比赛命令

#define SUB_GR_MATCH_INFO			1									//比赛信息	
#define SUB_GR_MATCH_RULE			2									//比赛规则
#define SUB_GR_MATCH_FEE			3									//报名费用
#define SUB_GR_MATCH_NUM			4									//等待人数
#define SUB_GR_LEAVE_MATCH			5									//退出比赛
#define SUB_GR_MATCH_SAFECARD		6									//保险提醒
#define SUB_GR_MATCH_BUY_SAFECARD	7									//购买保险
#define SUB_GR_MATCH_WAIT_TIP		8									//等待提示
#define SUB_GR_MATCH_RESULT			9									//比赛结果
#define SUB_GR_MATCH_STATUS			10									//比赛状态
#define SUB_GR_MATCH_USTATUS		11									//比赛状态	
#define SUB_GR_MATCH_DESC			12									//比赛描述
#define SUB_GR_MATCH_GOLDUPDATE		13									//金币更新
#define SUB_GR_MATCH_ELIMINATE		14									//比赛淘汰
#define SUB_GR_MATCH_REVIVE			15									//复活提醒
#define SUB_GR_MATCH_USER_REVIVE	16									//用户复活


/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661//////

//比赛规则
struct CMD_GR_Match_Rule
{
	//基本信息
	DWORD							dwMatchID;							//比赛标识	
	BYTE							cbMatchType;						//比赛类型
	TCHAR							szMatchName[32];					//比赛名称

	//报名信息
	BYTE							cbFeeType;							//费用类型
	BYTE							cbDeductArea;						//缴费区域
	LONGLONG						lSignupFee;							//报名费用	
	BYTE							cbSignupMode;						//报名方式
	BYTE							cbJoinCondition;					//参赛条件
	BYTE							cbMemberOrder;						//会员等级
	DWORD							dwExperience;						//玩家经验

	//排名方式
	BYTE							cbRankingMode;						//排名方式	
	WORD							wCountInnings;						//统计局数
	BYTE							cbFilterGradesMode;					//筛选方式
	
	//奖励信息
	WORD							wRewardCount;						//奖励人数
	tagMatchRewardInfo				MatchRewardInfo[3];					//比赛奖励
	//比赛规则
	BYTE							cbMatchRule[512];					//比赛规则
};

//复活提醒
struct CMD_GR_Match_Revive
{
	//比赛信息
	DWORD							dwMatchID;							//比赛标识
	LONGLONG						lMatchNO;							//比赛场次

	//复活信息
	LONGLONG						lReviveFee;							//复活费用
	BYTE							cbReviveTimes;						//复活次数
	BYTE							cbReviveTimesed;					//复活次数
	TCHAR							szNotifyContent[128];				//提示内容
};

//保险提醒
struct CMD_GR_Match_SafeCard
{
	//比赛信息
	DWORD							dwMatchID;							//比赛标识
	LONGLONG						lMatchNO;							//比赛场次

	//保险费用
	LONGLONG						lSafeCardFee;						//保险费用
	TCHAR							szNotifyContent[128];				//提示内容
};

//购买保险
struct CMD_GR_Match_BuySafeCard
{
	bool							bBuySafeCard;						//购买标识
};

//费用提醒
struct CMD_GR_Match_Fee
{
	//比赛信息
	DWORD							dwMatchID;							//比赛标识
	LONGLONG						lMatchNO;							//比赛场次

	//报名信息
	BYTE							cbSignupMode;						//报名方式
	BYTE							cbFeeType;							//费用类型
	BYTE							cbDeductArea;						//扣费区域
	LONGLONG						lSignupFee;							//报名费用
	TCHAR							szNotifyContent[128];				//提示内容
};

//比赛人数
struct CMD_GR_Match_Num
{
	DWORD							dwWaitting;							//等待人数
	DWORD							dwTotal;							//开赛人数
};

//赛事信息
struct CMD_GR_Match_Info
{
	TCHAR							szTitle[4][64];						//信息标题
    WORD							wGameCount;							//游戏局数
};

//提示信息
struct CMD_GR_Match_Wait_Tip
{
	LONGLONG						lScore;								//当前积分
	WORD							wRank;								//当前名次
	WORD							wCurTableRank;						//本桌名次
	WORD							wUserCount;							//当前人数
	WORD							wCurGameCount;						//当前局数
	WORD							wGameCount;							//总共局数
	WORD							wPlayingTable;						//游戏桌数
	TCHAR							szMatchName[LEN_SERVER];			//比赛名称
};

//比赛结果
struct CMD_GR_MatchResult
{	
	//奖励信息
	SCORE							lGold;								//金币奖励
	SCORE							lIngot;								//元宝奖励
	DWORD							dwExperience;						//经验奖励
	TCHAR							szNickName[LEN_ACCOUNTS];			//玩家昵称

	//比赛信息
	WORD							wMatchRank;							//比赛名次
	TCHAR							szMatchName[64];					//比赛名称	
	SYSTEMTIME						MatchEndTime;						//结束时间	
};

#define MAX_MATCH_DESC				4									//最多描述

//比赛描述
struct CMD_GR_MatchDesc
{
	TCHAR							szTitle[MAX_MATCH_DESC][16];		//信息标题
	TCHAR							szDescribe[MAX_MATCH_DESC][64];		//描述内容
	COLORREF						crTitleColor;						//标题颜色
	COLORREF						crDescribeColor;					//描述颜色
};

//金币更新
struct CMD_GR_MatchGoldUpdate
{
	LONGLONG						lCurrGold;							//当前金币
};

//用户复活
struct CMD_GR_Match_UserRevive
{
	bool							bUseSafeCard;						//使用保险卡
};

//////////////////////////////////////////////////////////////////////////

//低保命令
#define MDM_GR_BASEENSURE				11

#define SUB_GR_BASEENSURE_LOAD			260								//加载低保
#define SUB_GR_BASEENSURE_TAKE			261								//领取低保
#define SUB_GR_BASEENSURE_PARAMETER		262								//低保参数
#define SUB_GR_BASEENSURE_RESULT		263								//低保结果

//领取低保
struct CMD_GR_BaseEnsureTake
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//低保参数
struct CMD_GR_BaseEnsureParamter
{
	SCORE							lScoreCondition;					//游戏币条件
	SCORE							lScoreAmount;						//游戏币数量
	BYTE							cbTakeTimes;						//领取次数	
};

//低保结果
struct CMD_GR_BaseEnsureResult
{
	bool							bSuccessed;							//成功标识
	SCORE							lGameScore;							//当前游戏币
	TCHAR							szNotifyContent[128];				//提示内容
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//框架命令

#define MDM_GF_FRAME				100									//框架命令

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//框架命令

//用户命令
#define SUB_GF_GAME_OPTION			1									//游戏配置
#define SUB_GF_USER_READY			2									//用户准备
#define SUB_GF_LOOKON_CONFIG		3									//旁观配置
#define SUB_GF_ANDEOID_QUIT			4									//机器人退出
//聊天命令
#define SUB_GF_USER_CHAT			10									//用户聊天
#define SUB_GF_USER_EXPRESSION		11									//用户表情
#define SUB_GF_USER_VOICE			12									//用户语音

//游戏信息
#define SUB_GF_GAME_STATUS			100									//游戏状态
#define SUB_GF_GAME_SCENE			101									//游戏场景
#define SUB_GF_LOOKON_STATUS		102									//旁观状态
#define SUB_GF_USER_DATA			103									//玩家数据

#define SUB_GF_BJL_GAME_STATUS		2001								//百家乐大厅请求所有房间游戏状态（含历史记录）
#define SUB_GF_BJL_GAME_RESULT		2002								//百家乐通知房间玩家单局游戏结果
#define SUB_GF_BJL_CHANGE_STATUS	2003								//百家乐通知房间玩家游戏状态改变
#define SUB_GF_BAIREN_STATUS_END	2004								//请求获取列表完成

//系统消息
#define SUB_GF_SYSTEM_MESSAGE		200									//系统消息
#define SUB_GF_ACTION_MESSAGE		201									//动作消息


/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//游戏配置
struct CMD_GF_GameOption
{
	BYTE							cbAllowLookon;						//旁观标志
	DWORD							dwFrameVersion;						//框架版本
	DWORD							dwClientVersion;					//游戏版本
};

//旁观配置
struct CMD_GF_LookonConfig
{
	DWORD							dwUserID;							//用户标识
	BYTE							cbAllowLookon;						//允许旁观
};

//游戏状态切换
struct tagChangeStatus
{
	WORD							wTableID;							//桌子ID
	BYTE							cbGameStatus;						//游戏状态
};

//百家乐记录信息
struct tagBjlGameRecord
{
	BYTE							bAreaWin[5];						//区域胜利标识
};

//百家乐单次记录
struct tagBjlPerGameRecord
{
	WORD							wTableID;							//桌子ID
	BYTE							bAreaWin[5];						//区域胜利标识
};

//百人牛牛时间信息
struct tagBjlTimeInfo
{
	BYTE							cbFreeTime;							//空闲时间
	BYTE							cbBetTime;							//下注时间
	BYTE							cbEndTime;							//结束时间
	BYTE							cbPassTime;							//已经过了多少时间

	SCORE							lMinXianHong;						//最小限红
	SCORE							lMaxXianHong;						//最大限红
};

//百家乐房间状态
struct CMD_GF_BjlRoomStatus
{
	tagChangeStatus					tagGameInfo;						//房间信息
	tagBjlTimeInfo					tagTimeInfo;						//时间信息
	tagBjlGameRecord				GameRecordArrary[48];				//路单记录
	BYTE							cbRecordCount;						//记录条数
};

//百人牛牛记录信息
struct tagOxGameRecord
{
	BYTE							bAreaWin[4];						//区域胜利标识
};

//百人牛牛单局记录信息
struct tagOxPerGameRecord
{
	WORD							wTableID;							//桌子ID
	BYTE							bAreaWin[4];						//区域胜利标识
};

//百人牛牛时间信息
struct tagOxTimeInfo
{
	BYTE							cbFreeTime;							//空闲时间
	BYTE							cbBetTime;							//下注时间
	BYTE							cbEndTime;							//结束时间
	BYTE							cbPassTime;							//已经过了多少时间

	SCORE							lMinXianHong;						//最小限红
	SCORE							lMaxXianHong;						//最大限红
};

//百人牛牛房间状态
struct CMD_GF_OxRoomStatus
{
	tagChangeStatus					tagGameInfo;						//房间信息
	tagOxTimeInfo					tagTimeInfo;						//时间信息
	tagOxGameRecord					GameRecordArrary[20];				//路单记录
	BYTE							cbRecordCount;						//记录条数
	BYTE							cbMultiple;							//最大倍数
};

//红黑记录信息
struct tagRBGameRecord
{
	BYTE							cbAreaWin[4];							//区域胜利标识
};

//红黑单局记录信息
struct tagRBPerGameRecord
{
	WORD							wTableID;
	BYTE							cbAreaWin[4];							//区域胜利标识
};

//红黑时间信息
struct tagRBTimeInfo
{
	BYTE							cbBetTime;							//下注时间
	BYTE							cbEndTime;							//结束时间
	BYTE							cbPassTime;							//已经过了多少时间

	SCORE							lMinXianHong;						//最小限红
	SCORE							lMaxXianHong;						//最大限红
};

//红黑房间状态
struct CMD_GF_RBRoomStatus
{
	tagChangeStatus					tagGameInfo;						//房间信息
	tagRBTimeInfo					tagTimeInfo;						//时间信息
	tagRBGameRecord					GameRecordArrary[48];				//路单记录
	BYTE							cbRecordCount;						//记录条数
};

//龙虎记录信息
struct tagDTGameRecord
{
	BYTE							bAreaWin[3];
};

//龙虎记录信息
struct tagDTPerGameRecord
{
	WORD							wTableID;
	BYTE							bAreaWin[3];
};

//龙虎时间信息
struct tagDTTimeInfo
{
	BYTE							cbFreeTime;							//空闲时间
	BYTE							cbBetTime;							//下注时间
	BYTE							cbEndTime;							//结束时间
	BYTE							cbPassTime;							//已经过了多少时间

	SCORE							lMinXianHong;						//最小限红
	SCORE							lMaxXianHong;						//最大限红
};

//龙虎房间状态
struct CMD_GF_DTRoomStatus
{
	tagChangeStatus					tagGameInfo;						//房间信息
	tagDTTimeInfo					tagTimeInfo;						//时间信息
	tagDTGameRecord					GameRecordArrary[48];				//路单记录
	BYTE							cbRecordCount;						//记录条数
};

//飞禽走兽记录信息
struct tagAnGameRecord
{
	BYTE							cbAnimal;							//天门胜利
};

//飞禽走兽记录信息
struct tagAnPerGameRecord
{
	WORD							wTableID;
	BYTE							cbAnimal;							//天门胜利
};

//飞禽走兽时间信息
struct tagAnTimeInfo
{
	BYTE							cbFreeTime;							//空闲时间
	BYTE							cbBetTime;							//下注时间
	BYTE							cbEndTime;							//结束时间
	BYTE							cbPassTime;							//已经过了多少时间

	SCORE							lMinXianHong;						//最小限红
	SCORE							lMaxXianHong;						//最大限红
};

//飞禽走兽房间状态
struct CMD_GF_AnRoomStatus
{
	tagChangeStatus					tagGameInfo;						//房间信息
	tagAnTimeInfo					tagTimeInfo;						//时间信息
	tagAnGameRecord					GameRecordArrary[40];				//路单记录
	BYTE							cbRecordCount;						//记录条数
};

//奔驰宝马记录信息
struct tagZoGameRecord
{
	BYTE							cbAnimal;							//天门胜利
};

//奔驰宝马记录信息
struct tagZoPerGameRecord
{
	WORD							wTableID;
	BYTE							cbAnimal;							//天门胜利
};

//奔驰宝马时间信息
struct tagZoTimeInfo
{
	BYTE							cbFreeTime;							//空闲时间
	BYTE							cbBetTime;							//下注时间
	BYTE							cbEndTime;							//结束时间
	BYTE							cbPassTime;							//已经过了多少时间

	SCORE							lMinXianHong;						//最小限红
	SCORE							lMaxXianHong;						//最大限红
};

//奔驰宝马房间状态
struct CMD_GF_ZoRoomStatus
{
	tagChangeStatus					tagGameInfo;						//房间信息
	tagZoTimeInfo					tagTimeInfo;						//时间信息
	tagZoGameRecord					GameRecordArrary[40];				//路单记录
	BYTE							cbRecordCount;						//记录条数
};

//轮盘记录信息
struct tagRoletteGameRecord
{
	BYTE							cbResult;							//开奖结果
};

//轮盘记录信息
struct tagRolettePerGameRecord
{
	WORD							wTableID;
	BYTE							cbResult;							//开奖结果
};

//轮盘时间信息
struct tagRoletteTimeInfo
{
	BYTE							cbBetTime;							//下注时间
	BYTE							cbEndTime;							//结束时间
	BYTE							cbPassTime;							//已经过了多少时间
};

//轮盘房间状态
struct CMD_GF_RoletteRoomStatus
{
	tagChangeStatus					tagGameInfo;						//房间信息
	tagRoletteTimeInfo				tagTimeInfo;						//时间信息
	tagRoletteGameRecord			GameRecordArrary[72];				//路单记录
	BYTE							cbRecordCount;						//记录条数
};

//骰宝记录信息
struct tagSicBoGameRecord
{
	BYTE							cbResult[3];						//开奖结果
	BYTE							cbIsBig;							//是否为大  0小 1 大  2豹子
};

//骰宝记录信息
struct tagSicBoPerGameRecord
{
	WORD							wTableID;
	BYTE							cbResult[3];						//开奖结果
	BYTE							cbIsBig;							//是否为大  0小 1 大  2豹子
};

//骰宝时间信息
struct tagSicBoTimeInfo
{
	BYTE							cbFreeTime;							//空闲时间
	BYTE							cbBetTime;							//下注时间
	BYTE							cbEndTime;							//结束时间
	BYTE							cbPassTime;							//已经过了多少时间
};

//骰宝房间状态
struct CMD_GF_SicBoRoomStatus
{
	tagChangeStatus					tagGameInfo;						//房间信息
	tagSicBoTimeInfo				tagTimeInfo;						//时间信息
	tagSicBoGameRecord				GameRecordArrary[40];				//路单记录
	BYTE							cbRecordCount;						//记录条数
};

//百人推筒子记录信息
struct tagTongGameRecord
{
	BYTE							cbAreaWin[3];						//开奖结果
};

//百人推筒子记录信息
struct tagTongPerGameRecord
{
	WORD							wTableID;
	BYTE							cbAreaWin[3];						//开奖结果
};

//百人推筒子时间信息
struct tagTongTimeInfo
{
	BYTE							cbFreeTime;							//空闲时间
	BYTE							cbBetTime;							//下注时间
	BYTE							cbEndTime;							//结束时间
	BYTE							cbPassTime;							//已经过了多少时间
};

//百人推筒子房间状态
struct CMD_GF_TongRoomStatus
{
	tagChangeStatus					tagGameInfo;						//房间信息
	tagTongTimeInfo					tagTimeInfo;						//时间信息
	tagTongGameRecord				GameRecordArrary[20];				//路单记录
	BYTE							cbRecordCount;						//记录条数
};

//旁观状态
struct CMD_GF_LookonStatus
{
	BYTE							cbAllowLookon;						//允许旁观
};

//游戏环境
struct CMD_GF_GameStatus
{
	BYTE							cbGameStatus;						//游戏状态
	BYTE							cbAllowLookon;						//旁观标志
};
//游戏环境
struct CMD_GF_GameUserData
{
	DWORD							cbUserCharID;						//游戏状态
};
//用户聊天
struct CMD_GF_C_UserChat
{
	WORD							wItemIndex;							//文字索引
};

//用户聊天
struct CMD_GF_S_UserChat
{
	DWORD							dwSendUserID;						//发送用户
	WORD							wItemIndex;							//文字索引
};

//用户表情
struct CMD_GF_C_UserExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwTargetUserID;						//目标用户
};

//用户表情
struct CMD_GF_S_UserExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
};

//用户语音
struct CMD_GF_C_UserVoice
{
	DWORD							dwTargetUserID;						//目标用户
	DWORD							dwVoiceLength;						//语音长度
	BYTE							byVoiceData[MAXT_VOICE_LENGTH];		//语音数据
};

//用户语音
struct CMD_GF_S_UserVoice
{
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
	DWORD							dwVoiceLength;						//语音长度
	BYTE							byVoiceData[MAXT_VOICE_LENGTH];		//语音数据
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//游戏命令

#define MDM_GF_GAME					200									//游戏命令

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//携带信息

//其他信息
#define DTP_GR_TABLE_PASSWORD		1									//桌子密码

//用户属性
#define DTP_GR_NICK_NAME			10									//用户昵称
#define DTP_GR_GROUP_NAME			11									//社团名字
#define DTP_GR_UNDER_WRITE			12									//个性签名
#define DTP_GR_QQ					13									//QQ号码
#define DTP_GR_MOBILE_PHONE			14									//手机号码

//附加信息
#define DTP_GR_USER_NOTE			20									//用户备注
#define DTP_GR_CUSTOM_FACE			21									//自定头像

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//请求错误
#define REQUEST_FAILURE_NORMAL		0									//常规原因
#define REQUEST_FAILURE_NOGOLD		1									//金币不足
#define REQUEST_FAILURE_NOSCORE		2									//积分不足
#define REQUEST_FAILURE_PASSWORD	3									//密码错误

//游戏跑马灯消息
struct CMD_GR_GameRadioMessage
{
	WORD					wKindID;							//游戏
	BYTE					cbCardType;							//牌型
	SCORE					lScore;								//金币
	TCHAR					szNickName[32];						//玩家昵称
};
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

#pragma pack()

#endif