#ifndef CMD_LONGON_HEAD_FILE
#define CMD_LONGON_HEAD_FILE

#pragma pack(1)

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//登录命令

#define MDM_GP_LOGON				1									//广场登录

//登录结果
#define SUB_GP_LOGON_SUCCESS		100									//登录成功
#define SUB_GP_LOGON_FAILURE		101									//登录失败
#define SUB_GP_LOGON_FINISH			102									//登录完成
#define SUB_GP_VALIDATE_MBCARD      103                                 //登录失败
#define SUB_GP_VALIDATE_PASSPORT	104									//登录失败	
#define SUB_GP_MATCH_SIGNUPINFO		106									//报名信息
#define SUB_GP_VERIFY_CODE_RESULT	108									//验证结果

//升级提示
#define SUB_GP_UPDATE_NOTIFY		200									//升级提示

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//
#define MB_VALIDATE_FLAGS           0x01                                //效验密保
#define LOW_VER_VALIDATE_FLAGS      0x02                                //效验低版本

//校验掩码
#define VERIFY_ACCOUNTS				0x01								//校验账号
#define VERIFY_NICKNAME				0x02								//校验昵称

//I D 登录
struct CMD_GP_LogonGameID
{
	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//登录信息
	DWORD							dwGameID;							//游戏 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	BYTE							cbValidateFlags;			        //校验标识
};

//帐号登录
struct CMD_GP_LogonAccounts
{
	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//登录信息
	BYTE							cbValidateFlags;			        //校验标识
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//身份证号
};

//注册帐号
struct CMD_GP_RegisterAccounts
{
	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码

	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//证件号码
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字
	BYTE							cbValidateFlags;			        //校验标识
	TCHAR							szAgentID[LEN_ACCOUNTS];			//代理标识
	DWORD							dwSpreaderGameID;					//推荐标识
};

//验证资料
struct CMD_GP_VerifyIndividual
{
	//系统信息
	DWORD							dwPlazaVersion;						//广场版本

	//验证信息
	WORD							wVerifyMask;						//验证掩码
};

//游客登录
struct CMD_GP_LogonVisitor
{
	DWORD							dwPlazaVersion;						//广场版本
	BYTE							cbValidateFlags;			        //校验标识

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//登录成功
struct CMD_GP_LogonSuccess
{
	//属性资料
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwGroupID;							//社团标识
	DWORD							dwCustomID;							//自定标识	
	DWORD							dwExperience;						//经验数值
	SCORE							lLoveLiness;						//用户魅力

	//用户成绩
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//用户银行
	SCORE							lUserIngot;							//用户元宝
	DOUBLE							dUserBeans;							//用户游戏豆

	//用户信息
	BYTE							cbGender;							//用户性别
	BYTE							cbMoorMachine;						//锁定机器
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称
	TCHAR							szDynamicPass[LEN_PASSWORD];		//动态密码
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字

	//配置信息
	BYTE							cbInsureEnabled;					//银行使能标识
	BYTE                            cbShowServerStatus;                 //显示服务器状态
	BYTE							cbIsAgent;							//代理标识
};

//登录失败
struct CMD_GP_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//登陆完成
struct CMD_GP_LogonFinish
{
	WORD							wIntermitTime;						//中断时间
	WORD							wOnLineCountTime;					//更新时间
};

//登录失败
struct CMD_GP_ValidateMBCard
{
	UINT								uMBCardID;						//机器序列
};

//验证结果
struct CMD_GP_VerifyIndividualResult
{
	bool							bVerifyPassage;						//验证通过
	WORD							wVerifyMask;						//验证掩码
	TCHAR							szErrorMsg[128];					//错误信息
};

//升级提示
struct CMD_GP_UpdateNotify
{
	BYTE							cbMustUpdate;						//强行升级
	BYTE							cbAdviceUpdate;						//建议升级
	DWORD							dwCurrentVersion;					//当前版本
};

//验证结果
struct CMD_GP_VerifyCodeResult
{
	BYTE								cbResultCode;					//结果标识（1成功，0失败，2重复申请）
	TCHAR								szDescString[64];				//结果字符串
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//携带信息 CMD_GP_LogonSuccess

#define DTP_GP_GROUP_INFO			1									//社团信息
#define DTP_GP_MEMBER_INFO			2									//会员信息
#define	DTP_GP_UNDER_WRITE			3									//个性签名
#define DTP_GP_STATION_URL			4									//主页信息

//社团信息
struct DTP_GP_GroupInfo
{
	DWORD							dwGroupID;							//社团索引
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字
};

//会员信息
struct DTP_GP_MemberInfo
{
	BYTE							cbMemberOrder;						//会员等级
	SYSTEMTIME						MemberOverDate;						//到期时间
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//列表命令

#define MDM_GP_SERVER_LIST			2									//列表信息

//获取命令
#define SUB_GP_GET_LIST				1									//获取列表
#define SUB_GP_GET_SERVER			2									//获取房间
#define SUB_GP_GET_MATCH			3									//获取比赛
#define SUB_GP_GET_ONLINE			4									//获取在线
#define SUB_GP_GET_COLLECTION		5									//获取收藏

//列表信息
#define SUB_GP_LIST_TYPE			100									//类型列表
#define SUB_GP_LIST_KIND			101									//种类列表
#define SUB_GP_LIST_NODE			102									//节点列表
#define SUB_GP_LIST_PAGE			103									//定制列表
#define SUB_GP_LIST_SERVER			104									//房间列表
#define SUB_GP_LIST_MATCH			105									//比赛列表
#define SUB_GP_VIDEO_OPTION			106									//视频配置
#define SUB_GP_AGENT_KIND			107									//代理列表

//完成信息
#define SUB_GP_LIST_FINISH			200									//发送完成
#define SUB_GP_SERVER_FINISH		201									//房间完成
#define SUB_GP_MATCH_FINISH			202									//比赛完成

//在线信息
#define SUB_GR_KINE_ONLINE			300									//类型在线
#define SUB_GR_SERVER_ONLINE		301									//房间在线

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//获取在线
struct CMD_GP_GetOnline
{
	WORD							wServerCount;						//房间数目
	WORD							wOnLineServerID[MAX_SERVER];		//房间标识
};

//类型在线
struct CMD_GP_KindOnline
{
	WORD							wKindCount;							//类型数目
	tagOnLineInfoKind				OnLineInfoKind[MAX_KIND];			//类型在线
};

//房间在线
struct CMD_GP_ServerOnline
{
	WORD							wServerCount;						//房间数目
	tagOnLineInfoServer				OnLineInfoServer[MAX_SERVER];		//房间在线
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//服务命令

#define MDM_GP_USER_SERVICE				3								//用户服务

//账号服务
#define SUB_GP_MODIFY_INSURE_PASS		102								//修改密码

//修改头像
#define SUB_GP_USER_FACE_INFO			120								//头像信息
#define SUB_GP_SYSTEM_FACE_INFO			122								//系统头像

//个人资料
#define SUB_GP_USER_INDIVIDUAL			140								//个人资料
#define SUB_GP_MODIFY_INDIVIDUAL		152								//修改资料
#define SUB_GP_INDIVIDUAL_RESULT		153								//完善资料

//银行服务
#define SUB_GP_USER_ENABLE_INSURE		160								//开通银行
#define SUB_GP_USER_SAVE_SCORE			161								//存款操作
#define SUB_GP_USER_TAKE_SCORE			162								//取款操作
#define SUB_GP_USER_TRANSFER_SCORE		163								//转账操作
#define SUB_GP_USER_INSURE_INFO			164								//银行资料
#define SUB_GP_QUERY_INSURE_INFO		165								//查询银行
#define SUB_GP_USER_INSURE_SUCCESS		166								//银行成功
#define SUB_GP_USER_INSURE_FAILURE		167								//银行失败
#define SUB_GP_QUERY_USER_INFO_REQUEST	168								//查询用户
#define SUB_GP_QUERY_USER_INFO_RESULT	169								//用户信息
#define SUB_GP_USER_INSURE_ENABLE_RESULT 170							//开通结果	
#define SUB_GP_EXCHANGE_BANK_SCORE		177								//兑换银行金币
#define SUB_GP_QUERY_GAMELOCKINFO_RESULT		178						//查询锁表结果

#define SUB_GP_USER_SAVE_RESULT			179								//存钱结果
#define SUB_GP_USER_TAKE_RESULT			180								//取钱结果

//低保服务
#define SUB_GP_BASEENSURE_LOAD			260								//加载低保
#define SUB_GP_BASEENSURE_TAKE			261								//领取低保
#define SUB_GP_BASEENSURE_PARAMETER		262								//低保参数
#define SUB_GP_BASEENSURE_RESULT		263								//低保结果

//查询可加入游戏房间
#define SUB_GP_QUERY_GAMEROOM			301								//查询可加入游戏房间
#define SUB_GP_QUERY_GAMEROOM_RESULT	302								//查询可加入游戏房间结果

//抽奖服务
#define SUB_GP_LOTTERY_CONFIG_REQ		360								//请求配置
#define SUB_GP_LOTTERY_CONFIG			361								//抽奖配置
#define SUB_GP_LOTTERY_USER_INFO		362								//抽奖信息
#define SUB_GP_LOTTERY_START			363								//开始抽奖
#define SUB_GP_LOTTERY_RESULT			364								//抽奖结果

//邮件客服
#define SUB_GP_QUERY_EMAIL				370								//获取邮件
#define SUB_GP_QUERY_CUSTOMER			371								//获取客服信息
#define SUB_GP_SUBMIT_CUSTOMER			372								//提交客服信息

#define SUB_GP_EXCHANGESCORE_RESULT		401								//兑换金币结果

//操作结果
#define SUB_GP_OPERATE_SUCCESS			500								//操作成功
#define SUB_GP_OPERATE_FAILURE			501								//操作失败

#define SUB_MB_RADIO_MESSAGE			550								//广播消息

#define SUB_MB_CUSTOMER					551								//推送客服反馈
#define SUB_MB_RECHARGESUCCESS			552								//充值成功提醒
#define SUB_MB_NOTIFYIDNEWMAIL			553								//新邮件提醒
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//推送客服反馈
struct CMD_MB_Customer
{
	TCHAR							szContent[256];						//内容
};

//充值成功提醒
struct CMD_MB_RechargeSuccess
{
	SCORE							lScore;
	int								nID;								//邮件ID
	TCHAR							szTitle[20];						//邮件标题
	TCHAR							szSender[20];						//发送人
	TCHAR							szContent[256];						//内容
	TCHAR							szSendTime[20];						//发送时间
	int								nStatus;							//状态
};

//新邮件提醒
struct CMD_MB_NotifyidNewMail
{
	int								nID;								//邮件ID
	TCHAR							szTitle[20];						//邮件标题
	TCHAR							szSender[20];						//发送人
	TCHAR							szContent[256];						//内容
	TCHAR							szSendTime[20];						//发送时间
	int								nStatus;							//状态
};

//修改密码
struct CMD_GP_ModifyLogonPass
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
};

//修改密码
struct CMD_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
};

//修改签名
struct CMD_GP_ModifyUnderWrite
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//用户头像
struct CMD_GP_UserFaceInfo
{
	bool							bSuccess;
	WORD							wFaceID;							//头像标识
	TCHAR							szDescribeString[128];				//描述消息
};

//修改头像
struct CMD_GP_SystemFaceInfo
{
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//修改头像
struct CMD_GP_CustomFaceInfo
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	DWORD							dwCustomFace[FACE_CX*FACE_CY];		//图片信息
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//绑定机器
struct CMD_GP_ModifyMachine
{
	BYTE							cbBind;								//绑定标志
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//个人资料
struct CMD_GP_UserIndividual
{
	DWORD							dwUserID;							//用户 I D
};

//查询信息
struct CMD_GP_QueryIndividual
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
};

//修改资料
struct CMD_GP_ModifyIndividual
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
};

//查询赠送
struct CMD_GP_QuerySendPresent
{
	DWORD							dwUserID;							//用户 I D
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//携带信息 CMD_GP_UserIndividual
#define DTP_GP_UI_ACCOUNTS			1									//用户账号	
#define DTP_GP_UI_NICKNAME			2									//用户昵称
#define DTP_GP_UI_USER_NOTE			3									//用户说明
#define DTP_GP_UI_UNDER_WRITE		4									//个性签名
#define DTP_GP_UI_QQ				5									//Q Q 号码
#define DTP_GP_UI_EMAIL				6									//电子邮件
#define DTP_GP_UI_SEAT_PHONE		7									//固定电话
#define DTP_GP_UI_MOBILE_PHONE		8									//移动电话
#define DTP_GP_UI_COMPELLATION		9									//真实名字
#define DTP_GP_UI_DWELLING_PLACE	10									//联系地址
#define DTP_GP_UI_PASSPORTID    	11									//身份标识
#define DTP_GP_UI_SPREADER			12									//推广标识
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//银行资料
struct CMD_GP_UserInsureInfo
{
	BYTE							cbEnjoinTransfer;					//转账开关
	WORD							wRevenueTake;						//税收比例
	WORD							wRevenueTransfer;					//税收比例
	WORD							wRevenueTransferMember;				//税收比例
	WORD							wServerID;							//房间标识
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
	SCORE							lTransferPrerequisite;				//转账条件
};

//开通银行
struct CMD_GP_UserEnableInsure
{
	DWORD							dwUserID;							//用户I D
	TCHAR							szLogonPass[LEN_PASSWORD];			//登录密码
	TCHAR							szInsurePass[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//存入金币
struct CMD_GP_UserSaveScore
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lSaveScore;							//存入金币
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//提取金币
struct CMD_GP_UserTakeScore
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lTakeScore;							//提取金币
	TCHAR							szPassword[LEN_MD5];				//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//转账金币
struct CMD_GP_UserTransferScore
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lTransferScore;						//转账金币
	TCHAR							szPassword[LEN_MD5];				//银行密码
	TCHAR							szAccounts[LEN_NICKNAME];			//目标用户
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	TCHAR							szTransRemark[LEN_TRANS_REMARK];	//转账备注
};

//存钱结果
struct CMD_GP_UserSaveResult
{
	bool							bSuccess;							//是否成功
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
	TCHAR							szDescribeString[128];				//描述消息
};

//取钱结果
struct CMD_GP_UserTakeResult
{
	bool							bSuccess;							//是否成功
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
	TCHAR							szDescribeString[128];				//描述消息
};

//银行成功
struct CMD_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
	TCHAR							szDescribeString[128];				//描述消息
};

//银行失败
struct CMD_GP_UserInsureFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//查询银行
struct CMD_GP_QueryInsureInfo
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//银行密码
};

//查询用户绑定银行卡信息
struct CMD_GP_QueryBankInfo
{
	DWORD							dwUserID;							//用户 I D
};

//查询用户
struct CMD_GP_QueryUserInfoRequest
{
	BYTE                            cbByNickName;                       //昵称赠送
	TCHAR							szAccounts[LEN_ACCOUNTS];			//目标用户
};

//用户信息
struct CMD_GP_UserTransferUserInfo
{
	DWORD							dwTargetGameID;						//目标用户
	TCHAR							szAccounts[LEN_ACCOUNTS];			//目标用户
};

//开通结果
struct CMD_GP_UserInsureEnableResult
{
	BYTE							cbInsureEnabled;					//使能标识
	TCHAR							szDescribeString[128];				//描述消息
};

//查询返利
struct CMD_GP_QueryTransferRebate
{
	DWORD							dwUserID;							//用户I D
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码
};

//查询结果
struct CMD_GP_QueryTransferRebateResult
{
	DWORD							dwUserID;							//用户I D
	BYTE							cbRebateEnabled;					//使能标识	
	SCORE							lIngot;								//返利元宝
	SCORE							lLoveLiness;						//返利魅力值
};

//用户数据结果
struct CMD_GP_QueryUserDataResult
{
	DWORD							dwUserID;							//用户ID
};

//查询可加入游戏房间
struct CMD_GP_QueryGameRoom
{
	WORD							wKindID;							//游戏标识
	WORD							wServerLevel;						//游戏等级标识
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//领取低保
struct CMD_GP_BaseEnsureTake
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//低保参数
struct CMD_GP_BaseEnsureParamter
{
	SCORE							lScoreCondition;					//游戏币条件
	SCORE							lScoreAmount;						//游戏币数量
	BYTE							cbTakeTimes;						//领取次数	
};

//低保结果
struct CMD_GP_BaseEnsureResult
{
	bool							bSuccessed;							//成功标识
	SCORE							lGameScore;							//当前游戏币
	int								nTakeTimes;							//已领取次数
	TCHAR							szNotifyContent[128];				//提示内容
};

//查询可加入游戏房间结果
struct CMD_GP_QueryGameRoomResult
{
	WORD							wServerPort;						//房间端口
	TCHAR							szServerAddr[32];					//房间名称
};
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//推广服务

//推广查询
struct CMD_GP_UserSpreadQuery
{
	DWORD							dwUserID;							//用户标识
};

//推广参数
struct CMD_GP_UserSpreadInfo
{
	DWORD							dwSpreadCount;						//推广人数
	SCORE							lSpreadReward;						//推广奖励
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//抽奖服务

//请求配置
struct CMD_GP_LotteryConfigReq
{
	WORD							wKindID;							//游戏标识
	DWORD							dwUserID;							//用户标识
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
};

//抽奖配置
struct CMD_GP_LotteryConfig
{
	WORD							wLotteryCount;						//奖项个数
	tagLotteryItem					LotteryItem[MAX_LOTTERY];			//奖项内容
};

//抽奖信息
struct CMD_GP_LotteryUserInfo
{
	BYTE							cbFreeCount;						//免费次数
	BYTE							cbAlreadyCount;						//已领次数
	WORD							wKindID;							//游戏标识
	DWORD							dwUserID;							//用户标识
	SCORE							lChargeFee;							//抽奖费用
};

//开始抽奖
struct CMD_GP_LotteryStart
{
	WORD							wKindID;							//游戏标识
	DWORD							dwUserID;							//用户标识
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//抽奖结果
struct CMD_GP_LotteryResult
{
	bool							bWined;								//中奖标识
	WORD							wKindID;							//游戏标识
	DWORD							dwUserID;							//用户标识
	SCORE							lUserScore;							//用户分数
	DOUBLE							dUserBeans;							//用户游戏豆
	tagLotteryItem					LotteryItem;						//中奖内容
};

//获取邮件
struct CMD_GP_QueryEmail
{
	DWORD							dwUserID;							//用户标识
	int								nIndex;								//标识索引
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//游戏服务
struct CMD_GP_QueryUserGameData
{
	WORD							wKindID;							//游戏标识
	DWORD							dwUserID;							//用户标识
	TCHAR							szDynamicPass[LEN_MD5];				//用户密码	
};

//附加信息
#define DTP_GP_UI_USER_GAME_DATA	1									//游戏数据	

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//资料结果
struct CMD_GP_IndividuaResult
{
	bool							bSuccessed;							//成功标识
	TCHAR							szNotifyContent[128];				//提示内容
};


/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//比赛报名
struct CMD_GP_MatchSignup
{
	//比赛信息
	WORD							wServerID;							//房间标识
	DWORD							dwMatchID;							//比赛标识
	DWORD							dwMatchNO;							//比赛场次

	//用户信息
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_MD5];				//登录密码

	//机器信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//取消报名
struct CMD_GP_MatchUnSignup
{
	//比赛信息
	WORD							wServerID;							//房间标识
	DWORD							dwMatchID;							//比赛标识
	DWORD							dwMatchNO;							//比赛场次

	//用户信息
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_MD5];				//登录密码

	//机器信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//报名结果
struct CMD_GP_MatchSignupResult
{
	bool							bSignup;							//报名标识
	bool							bSuccessed;							//成功标识
	WORD							wServerID;							//房间标识
	SCORE							lCurrScore;							//当前金币
	TCHAR							szDescribeString[128];				//描述信息
};
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//账户绑定
struct CMD_GP_AccountBind
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	TCHAR							szBindNewAccounts[LEN_ACCOUNTS];	//绑定帐号
	TCHAR							szBindNewPassword[LEN_PASSWORD];	//绑定密码
	TCHAR							szBindNewSpreader[LEN_ACCOUNTS];	//绑定推荐
};

//账户绑定
struct CMD_GP_AccountBind_Exists
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	TCHAR							szBindExistsAccounts[LEN_ACCOUNTS];	//绑定帐号
	TCHAR							szBindExistsPassword[LEN_PASSWORD];	//绑定密码
};

//ip归属地
struct CMD_GP_IpLocation
{
	DWORD							dwUserID;
	TCHAR							szLocation[LEN_IP];	//ip归属地
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//兑换金币结果
struct CMD_GP_ExchangeScoreResult
{
	bool							bSuccess;						//错误代码
	SCORE							lScore;								//金币
	SCORE							lInsureScore;						//银行金币
	TCHAR							szDescribeString[128];				//描述消息
};

//操作失败
struct CMD_GP_OperateFailure
{
	TCHAR							szDescribeString[128];				//描述消息
};

//操作成功
struct CMD_GP_OperateSuccess
{
	TCHAR							szDescribeString[128];				//成功消息
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//远程服务

#define MDM_GP_REMOTE_SERVICE		4									//远程服务

//查找服务
#define SUB_GP_C_SEARCH_DATABASE	100									//数据查找
#define SUB_GP_C_SEARCH_CORRESPOND	101									//协调查找
#define SUB_GP_C_SEARCH_ALLCORRESPOND	102								//协调查找

//查找服务
#define SUB_GP_S_SEARCH_DATABASE	200									//数据查找
#define SUB_GP_S_SEARCH_CORRESPOND	201									//协调查找
#define SUB_GP_S_SEARCH_ALLCORRESPOND	202								//协调查找

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//协调查找
struct CMD_GP_C_SearchCorrespond
{
	DWORD							dwGameID;							//游戏标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
};

struct CMD_GP_C_SearchAllCorrespond
{
	DWORD							dwCount;
	DWORD							dwGameID[1];							//游戏标识
};

//协调查找
struct CMD_GP_S_SearchCorrespond
{
	WORD							wUserCount;							//用户数目
	tagUserRemoteInfo				UserRemoteInfo[16];					//用户信息
};

struct CMD_GP_S_SearchAllCorrespond
{
	DWORD							dwCount;
	tagUserRemoteInfo				UserRemoteInfo[1];					//用户信息				
};


/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//机器服务

#define MDM_GP_ANDROID_SERVICE		5									//机器服务

//参数操作
#define SUB_GP_GET_PARAMETER		100									//获取参数
#define SUB_GP_ADD_PARAMETER		101									//添加参数
#define SUB_GP_MODIFY_PARAMETER		102									//修改参数
#define SUB_GP_DELETE_PARAMETER		103									//删除参数

//参数信息
#define SUB_GP_ANDROID_PARAMETER	200									//机器参数		

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//获取参数
struct CMD_GP_GetParameter
{
	WORD							wServerID;							//房间标识
};

//添加参数
struct CMD_GP_AddParameter
{
	WORD							wServerID;							//房间标识
	tagAndroidParameter				AndroidParameter;					//机器参数
};

//修改参数
struct CMD_GP_ModifyParameter
{
	WORD							wServerID;							//房间标识
	tagAndroidParameter				AndroidParameter;					//机器参数
};

//删除参数
struct CMD_GP_DeleteParameter
{
	WORD							wServerID;							//房间标识
	DWORD							dwBatchID;							//批次标识
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//机器参数
struct CMD_GP_AndroidParameter
{
	WORD							wSubCommdID;						//子命令码
	WORD							wParameterCount;					//参数数目
	tagAndroidParameter				AndroidParameter[MAX_BATCH];		//机器参数
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//登录命令
#define MDM_MB_LOGON				100									//广场登录

//登录模式
#define SUB_MB_LOGON_GAMEID			1									//I D 登录
#define SUB_MB_LOGON_ACCOUNTS		2									//帐号登录
#define SUB_MB_REGISTER_ACCOUNTS	3									//注册帐号
#define SUB_MB_LOGON_OTHERPLATFORM	4									//其他登录
#define SUB_MB_LOGON_VISITOR		5									//游客登录
#define SUB_MB_QUERY_GAMELOCKINFO 				 14							//查询游戏锁表信息

//登录结果
#define SUB_MB_LOGON_SUCCESS		100									//登录成功
#define SUB_MB_LOGON_FAILURE		101									//登录失败
#define SUB_MB_MATCH_SIGNUPINFO		102									//报名信息

//升级提示
#define SUB_MB_UPDATE_NOTIFY		200									//升级提示

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//I D 登录
struct CMD_MB_LogonGameID
{
	//系统信息
	WORD							wModuleID;							//模块标识
	DWORD							dwPlazaVersion;						//广场版本
	BYTE                            cbDeviceType;                       //设备类型

	//登录信息
	DWORD							dwGameID;							//游戏 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
};

//帐号登录
struct CMD_MB_LogonAccounts
{
	//系统信息
	WORD							wModuleID;							//模块标识
	DWORD							dwPlazaVersion;						//广场版本
	BYTE                            cbDeviceType;                       //设备类型

	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
};

//帐号登录
struct CMD_MB_LogonOtherPlatform
{
	//系统信息
	WORD							wModuleID;							//模块标识
	DWORD							dwPlazaVersion;						//广场版本
	BYTE                            cbDeviceType;                       //设备类型

	//登录信息
	BYTE							cbGender;							//用户性别
	BYTE							cbPlatformID;						//平台编号
	TCHAR							szUserUin[LEN_USER_UIN];			//用户Uin
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字
	TCHAR							szAgentID[LEN_ACCOUNTS];			//代理标识

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
};

//注册帐号
struct CMD_MB_RegisterAccounts
{
	//系统信息
	WORD							wModuleID;							//模块标识
	DWORD							dwPlazaVersion;						//广场版本
	BYTE                            cbDeviceType;                       //设备类型

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码

	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	DWORD							dwSpreaderGameID;					//推荐标识	
	TCHAR							szAgentID[LEN_ACCOUNTS];			//代理标识	

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
	TCHAR							szRegisterMobile[LEN_MOBILE_PHONE];	//注册手机号码-当账号用
};

//注册手机帐号
struct CMD_MB_PhoneRegisterAccounts
{
	//系统信息
	WORD							wModuleID;							//模块标识
	DWORD							dwPlazaVersion;						//广场版本
	BYTE                            cbDeviceType;                       //设备类型

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码

	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//登录帐号
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	DWORD							dwSpreaderGameID;					//推荐标识	
	TCHAR							szAgentID[LEN_ACCOUNTS];			//代理标识	
	TCHAR							szVerifyCode[LEN_VERIFY_CODE];		//注册短信验证码

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};


//获取手机注册验证码
struct CMD_MB_GetRegisterVerifyCode
{
	//系统信息
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//手机号
	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//获取用户信息
struct CMD_MB_QueryUserData
{
	//系统信息
	BYTE cbType;								//操作类型
	TCHAR szWhere[LEN_WHERE_PARM];				//操作参数
};

//投诉代理
struct CMD_MB_TouSuDaili
{
	//用户ID
	DWORD dwUserID;								//用户ID
	TCHAR szUserWx[LEN_AGENT_ID];				//用户微信
	TCHAR szAgentWx[LEN_AGENT_ID];				//代理微信
	TCHAR szReason[LEN_DESC];					//代理微信
	TCHAR szImageName[LEN_IMAGE_NAME];			//代理微信
};

//申请代理前检查
struct CMD_MB_ShenQingDailiCheck
{
	DWORD dwUserID;								//用户ID
};

//申请代理
struct CMD_MB_ShenQingDaili
{
	DWORD							dwUserID;						//用户ID
	TCHAR							szAgentAcc[LEN_ACCOUNTS];		//代理账号
	TCHAR							szAgentName[LEN_AGENT_ID];		//代理名称
	TCHAR							szAlipay[LEN_ALIPAY];			//支付宝
	TCHAR							szWeChat[LEN_WECHAT];			//微信号
	TCHAR							szQQ[LEN_QQ];					//QQ号
	TCHAR							szNote[LEN_DESC];				//备注
};

//查询游戏锁表信息
struct CMD_MB_QueryGameLockInfo
{
	DWORD							dwUserID;						//用户ID
};

//获取修改密码验证码
struct CMD_MB_GetModifyPassVerifyCode
{
	//系统信息
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//手机号
	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//获取修改银行信息验证码
struct CMD_MB_GetModifyBankInfoVerifyCode
{
	//系统信息
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//手机号
	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//验证码修改密码
struct CMD_MB_ModifyPassVerifyCode
{
	//系统信息
	TCHAR							szAccounts[LEN_ACCOUNTS];			//用户账号
	TCHAR							szPassword[LEN_PASSWORD];			//新密码
	BYTE							cbMode;								//银行 登录
	TCHAR							szVerifyCode[LEN_VERIFY_CODE];		//注册短信验证码
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//手机号
	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//验证码修改银行信息
struct CMD_MB_ModifyBankInfoVerifyCode
{
	//系统信息
	DWORD							dwUserID;						//用户ID
	TCHAR							szBankNo[LEN_BANK_NO];			//银行卡号
	TCHAR							szBankName[LEN_BANK_NAME];		//银行名称

	TCHAR							szVerifyCode[LEN_VERIFY_CODE];		//注册短信验证码
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//手机号
	TCHAR							szBankAddress[LEN_BANK_ADDRESS];	//银行地址
	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//游客登录
struct CMD_MB_LogonVisitor
{
	//系统信息
	WORD							wModuleID;							//模块标识
	DWORD							dwPlazaVersion;						//广场版本
	TCHAR							szAgentID[LEN_ACCOUNTS];			//代理标识
	BYTE                            cbDeviceType;                       //设备类型

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
};

//登录成功
struct CMD_MB_LogonSuccess
{
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	DWORD							dwCustomID;							//自定头像
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwSpreaderID;						//推广ID
	DWORD							dwExperience;						//经验数值
	TCHAR							szAccounts[LEN_ACCOUNTS];			//用户帐号
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szAliPayAcccount[30];				//支付宝账户
	TCHAR							szBinkID[20];						//银行卡账户
	TCHAR							szDynamicPass[LEN_PASSWORD];		//动态密码

	//财富信息
	SCORE							lUserScore;							//用户游戏币
	SCORE							lUserInsure;						//用户银行	

	//扩展信息
	BYTE							cbInsureEnabled;					//使能标识
	BYTE							cbIsAgent;							//代理标识
	BYTE							cbMoorMachine;						//锁定机器

	//约战房相关
	int								TodayAlmsCount;						//每日低保已领取次数
	DWORD							dwLockServerID;						//锁定房间
	DWORD							dwKindID;							//游戏类型
};

//登录失败
struct CMD_MB_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//升级提示
struct CMD_MB_UpdateNotify
{
	BYTE							cbMustUpdate;						//强行升级
	BYTE							cbAdviceUpdate;						//建议升级
	DWORD							dwCurrentVersion;					//当前版本
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//列表主命令掩码
#define MDM_MB_SERVER_LIST			101									//列表信息

//查询消息
#define SUB_QUEREY_SERVER			1									//查询房间列表

struct CMD_MB_QueryServer
{
	WORD							wKindID;							
};

//结果消息
#define SUB_MB_LIST_SERVER_START	1									//开始发送列表通知
#define SUB_MB_LIST_SERVER_END		2									//结束发送列表通知

#define SUB_MB_LIST_KIND			100									//种类列表
#define SUB_MB_LIST_SERVER			101									//房间列表
#define SUB_MB_LIST_MATCH			102									//比赛列表	
#define SUB_MB_LIST_FINISH			200									//列表完成

#define SUB_MB_GET_ONLINE			300									//获取在线
#define SUB_MB_KINE_ONLINE			301									//类型在线
#define SUB_MB_SERVER_ONLINE		302									//房间在线
#define SUB_MB_AGENT_KIND			400									//代理列表
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//获取在线
struct CMD_MB_GetOnline
{
	WORD							wServerCount;						//房间数目
	WORD							wOnLineServerID[MAX_SERVER];		//房间标识
};

//类型在线
struct CMD_MB_KindOnline
{
	WORD							wKindCount;							//类型数目
	tagOnLineInfoKind				OnLineInfoKind[MAX_KIND];			//类型在线
};

//房间在线
struct CMD_MB_ServerOnline
{
	WORD							wServerCount;						//房间数目
	tagOnLineInfoServer				OnLineInfoServer[MAX_SERVER];		//房间在线
};

//兑换金币
struct CMD_GP_ExchangeBankScore
{
	DWORD dwUserID;							//用户id
	SCORE dwScore;		//真实姓名
	TCHAR szBankPass[LEN_PASSWORD];			//银行卡号
	TCHAR szOrderID[LEN_PASSWORD];		//银行名称
	DWORD dwType;							//0 bank 1 alipay
};
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

#pragma pack()

#endif