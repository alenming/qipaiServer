#ifndef DATA_BASE_PACKET_HEAD_FILE
#define DATA_BASE_PACKET_HEAD_FILE

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//请求数据包

//帐号服务
#define DBR_GP_MODIFY_INSURE_PASS	12									//修改密码
#define DBR_GP_MODIFY_INDIVIDUAL	14									//修改资料

//头像命令
#define DBR_GP_MODIFY_SYSTEM_FACE	20									//修改头像

//银行命令
#define DBR_GP_USER_ENABLE_INSURE	30									//开通银行
#define DBR_GP_USER_SAVE_SCORE		31									//存入游戏币
#define DBR_GP_USER_TAKE_SCORE		32									//提取游戏币
#define DBR_GP_USER_TRANSFER_SCORE	33									//转帐游戏币
#define DBR_GP_QUERY_BANK_INFO	34										//查询用户绑定银行信息
#define DBR_GP_EXCHANGE_BANK_SCORE	36										//兑换银行金币

//查询命令
#define DBR_GP_QUERY_INSURE_INFO	41									//查询银行
#define DBR_GP_QUERY_USER_INFO	    42									//查询用户

//系统命令
#define DBR_GP_ONLINE_COUNT_INFO	60									//在线信息
#define DBR_GP_LOAD_GAME_LIST		61									//加载列表
#define DBR_GP_LOAD_BASEENSURE		64									//加载低保
#define DBR_GP_LOAD_PLATFORM_PARAMETER 67								//平台参数


//机器命令
#define DBR_GP_GET_PARAMETER		70									//获取参数
#define DBR_GP_ADD_PARAMETER		71									//添加参数
#define DBR_GP_MODIFY_PARAMETER		72									//修改参数
#define DBR_GP_DELETE_PARAMETER		73									//删除参数

//低保命令
#define DBR_GP_BASEENSURE_TAKE		100									//领取低保

//抽奖服务
#define DBR_GP_LOTTERY_CONFIG_REQ	140									//请求配置
#define DBR_GP_LOTTERY_START		141									//开始抽奖

//邮件客服
#define DBR_GP_QUERY_EMAIL			150									//获取邮件

//认证命令
#define DBR_GP_SERVER_DUMMY_ONLINE	168							//查询在线虚拟人数


/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//ID 登录
struct DBR_GP_LogonGameID
{
	//登录信息
	DWORD							dwGameID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	BYTE							cbNeeValidateMBCard;				//密保校验

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//帐号登录
struct DBR_GP_LogonAccounts
{
	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//身份证号
	BYTE							cbNeeValidateMBCard;				//密保校验

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//连接信息
	LPVOID							pBindParameter;						//绑定参数

	//扩展信息
	DWORD							dwCheckUserRight;					//检查权限
};

//游客登陆
struct DBR_GP_LogonVisitor
{
	//登录信息
	BYTE							cbPlatformID;						//平台编号

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码

	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//帐号绑定
struct DBR_GP_AccountBind
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	TCHAR							szBindNewAccounts[LEN_ACCOUNTS];	//绑定帐号
	TCHAR							szBindNewPassword[LEN_PASSWORD];	//绑定密码
	TCHAR							szBindNewSpreader[LEN_ACCOUNTS];	//绑定推荐
};

//ip归属地
struct DBR_GP_IpLocation
{
	DWORD							dwUserID;
	TCHAR							szLocation[LEN_IP];	//ip归属地
};

//帐号绑定
struct DBR_GP_AccountBind_Exists
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	TCHAR							szBindExistsAccounts[LEN_ACCOUNTS];	//绑定帐号
	TCHAR							szBindExistsPassword[LEN_PASSWORD];	//绑定密码
};

//帐号注册
struct DBR_GP_RegisterAccounts
{
	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//证件号码
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字
	TCHAR							szAgentID[LEN_ACCOUNTS];			//代理标识
	DWORD							dwSpreaderGameID;					//推荐标识

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//验证资料
struct DBR_GP_VerifyIndividual
{
	//连接信息
	LPVOID							pBindParameter;						//绑定参数

	WORD							wVerifyMask;						//校验验证
	TCHAR							szVerifyContent[LEN_ACCOUNTS];		//验证内容
};

//修改机器
struct DBR_GP_ModifyMachine
{
	BYTE							cbBind;								//绑定标志
	DWORD							dwUserID;							//用户标识
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//修改密码
struct DBR_GP_ModifyLogonPass
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
};

//修改密码
struct DBR_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
};

//修改签名
struct DBR_GP_ModifyUnderWrite
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名
};

//修改头像
struct DBR_GP_ModifySystemFace
{
	//用户信息
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码

	//机器信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//修改头像
struct DBR_GP_ModifyCustomFace
{
	//用户信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	DWORD							dwCustomFace[FACE_CX*FACE_CY];		//图片信息

	//机器信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//修改资料
struct DBR_GP_ModifyIndividual
{
	//验证资料
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码

	//帐号资料
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
};

//查询资料
struct DBR_GP_QueryIndividual
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//银行密码
};

//开通银行
struct DBR_GP_UserEnableInsure
{
	DWORD							dwUserID;							//用户 I D	
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szLogonPass[LEN_PASSWORD];			//登录密码
	TCHAR							szInsurePass[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//存入游戏币
struct DBR_GP_UserSaveScore
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lSaveScore;							//存入游戏币
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//查询用户绑定银行信息
struct DBR_GP_QueryBankInfo
{
	DWORD							dwUserID;							//用户 I D
};

//绑定银行
struct DBR_GP_BindBankInfo
{											
	DWORD dwUserID;							//用户id
	TCHAR szRealName[LEN_REAL_NAME];		//真实姓名
	TCHAR szBankNo[LEN_BANK_NO];			//银行卡号
	TCHAR szBankName[LEN_BANK_NAME];		//银行名称
	TCHAR szBankAddress[LEN_BANK_ADDRESS];	//银行名称
	TCHAR szAlipayID[LEN_BANK_ADDRESS];		//支付宝
};

//绑定银行
struct DBR_GP_ExchangeBankScore
{											
	DWORD dwUserID;							//用户id
	SCORE dwScore;		//真实姓名
	TCHAR szBankPass[LEN_PASSWORD];			//银行卡号
	TCHAR szOrderID[LEN_PASSWORD];		//银行名称
	DWORD dwClientAddr;						//连接地址
	DWORD dwType;							//0 bank 1 alipay
};

//取出游戏币
struct DBR_GP_UserTakeScore
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lTakeScore;							//提取游戏币
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//转帐游戏币
struct DBR_GP_UserTransferScore
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	SCORE							lTransferScore;						//转帐游戏币
	TCHAR							szAccounts[LEN_ACCOUNTS];			//用户昵称
	TCHAR							szPassword[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	TCHAR							szTransRemark[LEN_TRANS_REMARK];	//转帐备注
};

//查询银行
struct DBR_GP_QueryInsureInfo
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
};

//查询用户
struct DBR_GP_QueryInsureUserInfo
{
	BYTE                            cbByNickName;                       //昵称赠送
	TCHAR							szAccounts[LEN_ACCOUNTS];			//目标用户
};

//用户资料
struct DBO_GP_UserTransferUserInfo
{
	DWORD							dwGameID;							//用户 I D
	TCHAR							szAccounts[LEN_ACCOUNTS];			//用户帐号
};

//查询返利
struct DBR_GP_QueryTransferRebate
{
	DWORD							dwUserID;							//用户I D
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码
};

//在线信息
struct DBR_GP_OnLineCountInfo
{
	WORD							wKindCount;							//类型数目
	DWORD							dwOnLineCountSum;					//总在线数
	DWORD							dwAndroidCountSum;					//总在线数
	tagOnLineInfoKindEx				OnLineCountKind[MAX_KIND];			//类型在线
};


//比赛报名
struct DBR_GP_MatchSignup
{
	//比赛信息
	WORD							wServerID;							//房间标识
	DWORD							dwMatchID;							//比赛标识
	DWORD							dwMatchNO;							//比赛场次

	//用户信息
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_MD5];				//登录密码

	//机器信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//比赛报名
struct DBR_GP_MatchUnSignup
{
	//比赛信息
	WORD							wServerID;							//房间标识
	DWORD							dwMatchID;							//比赛标识
	DWORD							dwMatchNO;							//比赛场次

	//用户信息
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_MD5];				//登录密码

	//机器信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};


//获取参数
struct DBR_GP_GetParameter
{
	WORD							wServerID;							//房间标识
};

//添加参数
struct DBR_GP_AddParameter
{
	WORD							wServerID;							//房间标识
	tagAndroidParameter				AndroidParameter;					//机器参数
};

//修改参数
struct DBR_GP_ModifyParameter
{
	WORD							wServerID;							//房间标识
	tagAndroidParameter				AndroidParameter;					//机器参数
};

//删除参数
struct DBR_GP_DeleteParameter
{
	WORD							wServerID;							//房间标识
	DWORD							dwBatchID;							//批次标识
};

//领取低保
struct DBR_GP_TakeBaseEnsure
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//推广信息
struct DBR_GP_QuerySpreadInfo
{
	DWORD							dwUserID;							//用户标识
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//抽奖服务

//开始抽奖
struct DBR_GP_LotteryConfigReq
{
	WORD							wKindID;							//游戏标识
	DWORD							dwUserID;							//用户标识
	TCHAR							szLogonPass[LEN_MD5];				//登录密码

	//附加信息
	DWORD							dwClientAddr;						//连接地址
};

//开始抽奖
struct DBR_GP_LotteryStart
{
	WORD							wKindID;							//游戏标识
	DWORD							dwUserID;							//用户标识
	TCHAR							szLogonPass[LEN_MD5];				//登录密码

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//获取邮件
struct DBR_GP_QueryEmail
{
	DWORD							dwUserID;							//用户标识
	int								nIndex;								//标识索引
};
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//游戏服务
struct DBR_GP_QueryUserGameData
{
	WORD							wKindID;							//游戏标识
	DWORD							dwUserID;							//用户标识
	TCHAR							szDynamicPass[LEN_MD5];				//用户密码	
};



/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//操作结果
#define DBO_GP_USER_FACE_INFO		110									//用户头像

//银行命令
#define DBO_GP_USER_INSURE_INFO		120									//银行资料
#define DBO_GP_USER_INSURE_SUCCESS	121									//银行成功
#define DBO_GP_USER_INSURE_FAILURE	122									//银行失败
#define DBO_GP_USER_INSURE_USER_INFO 123								//用户资料
#define DBO_GP_USER_INSURE_ENABLE_RESULT 124							//开通结果

#define DBO_GP_USER_SAVE_RESULT		125									//用户存钱结果
#define DBO_GP_USER_TAKE_RESULT		126									//用户取钱结果

//列表结果
#define DBO_GP_GAME_TYPE_ITEM		130									//种类信息
#define DBO_GP_GAME_KIND_ITEM		131									//类型信息
#define DBO_GP_GAME_NODE_ITEM		132									//节点信息
#define DBO_GP_GAME_PAGE_ITEM		133									//定制信息
#define DBO_GP_GAME_LIST_RESULT		134									//加载结果

//系统结果
#define DBO_GP_PLATFORM_PARAMETER	140									//平台参数
#define DBO_GP_SERVER_DUMMY_ONLINE	141									//房间虚拟人数
#define DBO_GP_SERVER_DUMMY_ONLINE_BEGIN	142							//房间虚拟人数
#define DBO_GP_SERVER_DUMMY_ONLINE_END	143								//房间虚拟人数
//机器命令
#define DBO_GP_ANDROID_PARAMETER	150									//机器参数	

//低保命令
#define DBO_GP_BASEENSURE_PARAMETER	180									//低保参数
#define DBO_GP_BASEENSURE_RESULT	181									//领取结果

//抽奖结果
#define DBO_GP_LOTTERY_CONFIG		230									//抽奖配置
#define DBO_GP_LOTTERY_USER_INFO	231									//抽奖信息
#define DBO_GP_LOTTERY_RESULT		232									//抽奖结果

//资料结果
#define DBO_GP_INDIVIDUAL_RESULT	260									//资料结果	

//代理列表
#define DBO_GP_AGENT_GAME_KIND_ITEM		351								//类型信息

#define DBO_GP_EXCHANGESCORE_RESULT	401									//兑换金币结果

//服务结果
#define DBO_GP_OPERATE_SUCCESS		800									//操作成功
#define DBO_GP_OPERATE_FAILURE		801									//操作失败

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661////////

//登录成功
struct DBO_GP_LogonSuccess
{
	//属性资料
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户标识
	DWORD							dwGameID;							//游戏标识
	DWORD							dwGroupID;							//社团标识
	DWORD							dwCustomID;							//自定索引
	DWORD							dwExperience;						//经验数值
	SCORE							lLoveLiness;						//用户魅力
	TCHAR							szPassword[LEN_MD5];				//登录密码	
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szDynamicPass[LEN_PASSWORD];		//动态密码
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字

	//用户成绩
	SCORE							lUserScore;							//用户游戏币
	SCORE							lUserIngot;							//用户元宝
	SCORE							lUserInsure;						//用户银行	
	DOUBLE							dUserBeans;							//用户游戏豆

	//用户资料
	BYTE							cbGender;							//用户性别
	BYTE							cbMoorMachine;						//锁定机器
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名

	//会员资料
	BYTE							cbMemberOrder;						//会员等级
	SYSTEMTIME						MemberOverDate;						//到期时间

	//扩展信息
	BYTE							cbInsureEnabled;					//使能标识
	DWORD							dwCheckUserRight;					//检查权限
	BYTE							cbIsAgent;							//代理标识	

	//描述信息
	TCHAR							szDescribeString[128];				//描述消息
};

//登录失败
struct DBO_GP_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//登录失败
struct DBO_GP_ValidateMBCard
{
	UINT							uMBCardID;						//机器序列
};

//验证结果
struct DBO_GP_VerifyIndividualResult
{
	bool							bVerifyPassage;						//验证通过
	WORD							wVerifyMask;						//验证掩码
	TCHAR							szErrorMsg[128];					//错误消息
};

//头像信息
struct DBO_GP_UserFaceInfo
{
	bool							bSuccess;
	WORD							wFaceID;							//头像标识
	TCHAR							szDescribeString[128];				//描述消息
};

//个人资料
struct DBO_GP_UserIndividual
{
	//用户信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szUserNote[LEN_USER_NOTE];			//用户说明
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//证件号码

	//电话号码
	TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//固定电话
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//移动电话

	//联系资料
	TCHAR							szQQ[LEN_QQ];						//Q Q 号码
	TCHAR							szEMail[LEN_EMAIL];					//电子邮件
	TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//联系地址

	//推广信息
	TCHAR							szSpreader[LEN_ACCOUNTS];			//推广信息
};

//银行资料
struct DBO_GP_UserInsureInfo
{
	BYTE							cbEnjoinTransfer;					//转帐开关
	WORD							wRevenueTake;						//税收比例
	WORD							wRevenueTransfer;					//税收比例
	WORD							wRevenueTransferMember;				//税收比例
	WORD							wServerID;							//房间标识
	SCORE							lUserScore;							//用户游戏币
	SCORE							lUserInsure;						//银行游戏币
	SCORE							lTransferPrerequisite;				//转帐条件
};

//存钱结果
struct DBO_GP_UserSaveResult
{
	bool							bSuccess;							//是否成功
	SCORE							lScore;								//身上金币
	SCORE							lInsureScore;						//银行金币
	TCHAR							szDescribeString[128];				//描述消息
};

//取钱结果
struct DBO_GP_UserTakeResult
{
	bool							bSuccess;							//是否成功
	SCORE							lScore;								//身上金币
	SCORE							lInsureScore;						//银行金币
	TCHAR							szDescribeString[128];				//描述消息
};

//银行成功
struct DBO_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lSourceScore;						//原来游戏币
	SCORE							lSourceInsure;						//原来游戏币
	SCORE							lInsureRevenue;						//银行税收
	SCORE							lVariationScore;					//游戏币变化
	SCORE							lVariationInsure;					//银行变化
	TCHAR							szDescribeString[128];				//描述消息
};

//银行失败
struct  DBO_GP_UserInsureFailure
{
	LONG							lResultCode;						//操作代码
	TCHAR							szDescribeString[128];				//描述消息
};

//开通结果
struct DBO_GP_UserInsureEnableResult
{
	BYTE							cbInsureEnabled;					//使能标识
	TCHAR							szDescribeString[128];				//描述消息
};

//查询用户绑定银行结果
struct DBO_GP_QueryBankInfoResult
{
	TCHAR							szRealName[LEN_REAL_NAME];			//真实姓名
	TCHAR							szBankNo[LEN_BANK_NO];				//银行卡号
	SCORE							lDrawScore;							//可提现金额
	TCHAR							szAlipayID[LEN_BANK_ADDRESS];		//支付寶
};

//机器参数
struct DBO_GP_PlatformParameter
{
	DWORD							dwExchangeRate;						//兑换比率
	DWORD							dwPresentExchangeRate;				//魅力游戏币兑换率
	DWORD							dwRateGold;							//游戏豆游戏币兑换率
};

//机器参数
struct DBO_GP_AndroidParameter
{
	WORD							wSubCommdID;						//子命令码
	WORD							wServerID;							//房间标识
	WORD							wParameterCount;					//参数数目
	tagAndroidParameter				AndroidParameter[MAX_BATCH];		//机器参数
};

//资料结果
struct DBO_GP_IndividualResult
{
	bool							bSuccessed;							//成功标识
	TCHAR							szDescribeString[128];				//描述消息
};

//查询结果
struct DBO_GP_QueryTransferRebateResult
{
	DWORD							dwUserID;							//用户I D
	BYTE							cbRebateEnabled;					//使能标识	
	SCORE							lIngot;								//返利元宝
	SCORE							lLoveLiness;						//返利魅力值
};

//低保参数
struct DBO_GP_BaseEnsureParameter
{
	SCORE							lScoreCondition;					//游戏币条件
	SCORE							lScoreAmount;						//游戏币数量
	BYTE							cbTakeTimes;						//领取次数	
};

//低保结果
struct DBO_GP_BaseEnsureResult
{
	bool							bSuccessed;							//成功标识
	SCORE							lGameScore;							//当前游戏币
	int								nTakeTimes;							//已领取次数
	TCHAR							szNotifyContent[128];				//提示内容
};

//推广信息
struct DBO_GP_UserSpreadInfo
{
	DWORD							dwSpreadCount;						//推广数量
	SCORE							lSpreadReward;						//推广奖励
};

//实名验证
struct DBO_GP_RealAuthParameter
{
	DWORD							dwAuthentDisable;					//验证开启
	SCORE							dwAuthRealAward;					//奖励金币
};

//用户数据结果
struct DBO_GP_QueryUserDataResult
{
	DWORD							dwUserID;							//用户ID
};

//抽奖配置
struct DBO_GP_LotteryConfig
{
	WORD							wLotteryCount;						//奖项个数
	tagLotteryItem					LotteryItem[MAX_LOTTERY];			//奖项内容
};

//抽奖信息
struct DBO_GP_LotteryUserInfo
{
	BYTE							cbFreeCount;						//免费次数
	BYTE							cbAlreadyCount;						//已领次数
	WORD							wKindID;							//游戏标识
	DWORD							dwUserID;							//用户标识
	SCORE							lChargeFee;							//抽奖费用
};

//抽奖结果
struct DBO_GP_LotteryResult
{
	bool							bWined;								//中奖标识
	WORD							wKindID;							//游戏标识
	DWORD							dwUserID;							//用户标识
	SCORE							lUserScore;							//用户分数
	DOUBLE							dUserBeans;							//用户游戏豆
	tagLotteryItem					LotteryItem;						//中奖内容
};

//操作失败
struct DBO_GP_OperateFailure
{
	TCHAR							szDescribeString[128];				//描述消息
};

//操作成功
struct DBO_GP_OperateSuccess
{
	TCHAR							szDescribeString[128];				//成功消息
};

//兑换金币结果
struct DBO_GP_ExchangeScoreResult
{
	bool							bSuccess;						//错误代码
	SCORE							lScore;								//金币
	SCORE							lInsureScore;						//银行金币
	TCHAR							szDescribeString[128];				//描述消息
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//游戏类型
struct DBO_GP_GameType
{
	WORD							wJoinID;							//挂接索引
	WORD							wSortID;							//排序索引
	WORD							wTypeID;							//类型索引
	TCHAR							szTypeName[LEN_TYPE];				//种类名字
};

//游戏种类
struct DBO_GP_GameKind
{
	WORD							wTypeID;							//类型索引
	WORD							wJoinID;							//挂接索引
	WORD							wSortID;							//排序索引
	WORD							wKindID;							//类型索引
	WORD							wGameID;							//模块索引
	WORD							wRecommend;							//推荐游戏
	WORD							wGameFlag;							//游戏标志
	DWORD							dwSuportType;						//支持类型
	TCHAR							szKindName[LEN_KIND];				//游戏名字
	TCHAR							szProcessName[LEN_PROCESS];			//进程名字
};

//游戏节点
struct DBO_GP_GameNode
{
	WORD							wKindID;							//名称索引
	WORD							wJoinID;							//挂接索引
	WORD							wSortID;							//排序索引
	WORD							wNodeID;							//节点索引
	TCHAR							szNodeName[LEN_NODE];				//节点名称
};

//定制类型
struct DBO_GP_GamePage
{
	WORD							wKindID;							//名称索引
	WORD							wNodeID;							//节点索引
	WORD							wSortID;							//排序索引
	WORD							wPageID;							//定制索引
	WORD							wOperateType;						//控制类型
	TCHAR							szDisplayName[LEN_PAGE];			//显示名称
};

//加载结果
struct DBO_GP_GameListResult
{
	BYTE							cbSuccess;							//成功标志
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//手机数据包

//登录命令
#define	DBR_MB_LOGON_GAMEID			700									//I D 登录
#define	DBR_MB_LOGON_ACCOUNTS		701									//帐号登录
#define DBR_MB_REGISTER_ACCOUNTS	702									//注册帐号
#define	DBR_MB_LOGON_OTHERPLATFORM	703									//其他登录
#define	DBR_MB_LOGON_VISITOR		704									//游客登录
#define DBR_MB_QUERY_GAMELOCKINFO	711								//游戏锁表查询

//登录结果
#define DBO_MB_LOGON_SUCCESS		900									//登录成功
#define DBO_MB_LOGON_FAILURE		901									//登录失败


/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//ID 登录
struct DBR_MB_LogonGameID
{
	//登录信息
	DWORD							dwGameID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码

	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//帐号登录
struct DBR_MB_LogonAccounts
{
	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码

	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//帐号登录
struct DBR_MB_LogonOtherPlatform
{
	//登录信息
	BYTE							cbGender;							//用户性别
	BYTE							cbPlatformID;						//平台编号
	TCHAR							szUserUin[LEN_USER_UIN];			//用户Uin
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字
	TCHAR							szAgentID[LEN_ACCOUNTS];			//代理标识
	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码

	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//游客登录
struct DBR_MB_LogonVisitor
{
	//登录信息
	BYTE							cbPlatformID;						//平台编号

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
	TCHAR							szAgentID[LEN_ACCOUNTS];			//代理标识

	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//投诉代理
struct DBR_MB_TouSuDaili
{
	//用户ID
	DWORD dwUserID;								//用户ID
	TCHAR szUserWx[LEN_AGENT_ID];				//用户微信
	TCHAR szAgentWx[LEN_AGENT_ID];				//代理微信
	TCHAR szReason[LEN_DESC];					//代理微信
	TCHAR szImageName[LEN_IMAGE_NAME];			//代理微信
};

//申请代理前检查
struct DBR_MB_ShenQingDailiCheck
{
	DWORD dwUserID;								//用户ID
};

//申请代理
struct DBR_MB_ShenQingDaili
{
	DWORD							dwUserID;						//用户ID
	TCHAR							szAgentAcc[LEN_ACCOUNTS];		//代理账号
	TCHAR							szAgentName[LEN_AGENT_ID];		//代理名称
	TCHAR							szAlipay[LEN_ALIPAY];			//支付宝
	TCHAR							szWeChat[LEN_WECHAT];			//微信号
	TCHAR							szQQ[LEN_QQ];					//QQ号
	TCHAR							szNote[LEN_DESC];				//备注
};

//申请代理
struct DBR_MB_QueryGameLockInfo
{
	DWORD							dwUserID;						//用户ID
};


//帐号注册
struct DBR_MB_RegisterAccounts
{
	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称
	BYTE                            cbDeviceType;                       //设备类型
	DWORD							dwSpreaderGameID;					//推荐标识
	TCHAR							szAgentID[LEN_ACCOUNTS];							//代理标识

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szInsurePass[LEN_MD5];				//银行密码

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码

	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//手机号注册
struct DBR_MB_PhoneRegisterAccounts
{
	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
	TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称
	BYTE                            cbDeviceType;                       //设备类型
	DWORD							dwSpreaderGameID;					//推荐标识
	TCHAR							szAgentID[LEN_ACCOUNTS];							//代理标识

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szInsurePass[LEN_MD5];				//银行密码

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//修改密码
struct DBR_MB_ModifyPass
{
	//注册信息
	TCHAR							szAccounts[LEN_ACCOUNTS];			//用户账号
	TCHAR							szPassword[LEN_PASSWORD];			//新密码
	BYTE							cbMode;								//银行 登录
	TCHAR							szVerifyCode[LEN_VERIFY_CODE];		//注册短信验证码
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//手机号
	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	//附加信息
	DWORD							dwClientAddr;						//连接地址
};

//验证码修改银行信息
struct DBR_MB_ModifyBankInfo
{
	//系统信息
	DWORD							dwUserID;						//用户ID
	TCHAR							szBankNo[LEN_BANK_NO];			//银行卡号
	TCHAR							szBankName[LEN_BANK_NAME];		//银行名称

	TCHAR							szBankAddress[LEN_BANK_ADDRESS];	//银行名称
	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	//附加信息
	DWORD							dwClientAddr;						//连接地址
};

//查询用户信息
struct DBR_MB_QueryUserData
{
	//系统信息
	BYTE cbType;								//操作类型
	TCHAR szWhere[LEN_WHERE_PARM];				//操作参数
};

//登录成功
struct DBO_MB_LogonSuccess
{
	//用户属性
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	DWORD							dwCustomID;							//自定头像
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwSpreaderID;						//推广ID
	DWORD							dwExperience;						//经验数值
	TCHAR							szAccounts[LEN_ACCOUNTS];			//用户帐号
	TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称
	TCHAR							szAliPayAcccount[30];				//支付宝账户
	TCHAR							szBinkID[20];						//银行卡账户
	TCHAR							szDynamicPass[LEN_PASSWORD];		//动态密码
	
	//用户成绩
	SCORE							lUserScore;							//用户游戏币
	SCORE							lUserInsure;						//用户银行	

	//扩展信息
	BYTE							cbInsureEnabled;					//使能标识
	BYTE							cbIsAgent;							//代理标识						
	BYTE							cbMoorMachine;						//锁定机器
	int								TodayAlmsCount;						//每日低保已领取次数
	DWORD							dwLockServerID;						//锁定房间
	DWORD							dwKindID;							//游戏类型
};


//登录失败
struct DBO_MB_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};



/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

#endif