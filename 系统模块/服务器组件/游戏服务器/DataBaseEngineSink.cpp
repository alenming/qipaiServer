#include "StdAfx.h"
#include "ServiceUnits.h"
#include "DataBaseEngineSink.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//构造函数
CDataBaseEngineSink::CDataBaseEngineSink()
{
	//配置变量
	m_pGameParameter=NULL;
	m_pInitParameter=NULL;
	m_pDataBaseParameter=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//组件变量
	m_pIDataBaseEngine=NULL;
	m_pIGameServiceManager=NULL;
	m_pIDataBaseEngineEvent=NULL;
	m_pIGameDataBaseEngineSink=NULL;
	m_pIDBCorrespondManager=NULL;

	//辅助变量
	ZeroMemory(&m_LogonFailure,sizeof(m_LogonFailure));
	ZeroMemory(&m_LogonSuccess,sizeof(m_LogonSuccess));

	return;
}

//析构函数
CDataBaseEngineSink::~CDataBaseEngineSink()
{
	//释放对象
	SafeRelease(m_pIGameDataBaseEngineSink);

	return;
}

//接口查询
VOID * CDataBaseEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE(IGameDataBaseEngine,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseEngineSink,Guid,dwQueryVer);
	return NULL;
}

//获取对象
VOID * CDataBaseEngineSink::GetDataBase(REFGUID Guid, DWORD dwQueryVer)
{
	//效验状态
	ASSERT(m_GameDBModule.GetInterface()!=NULL);
	if (m_GameDBModule.GetInterface()==NULL) return NULL;

	//查询对象
	IDataBase * pIDataBase=m_GameDBModule.GetInterface();
	VOID * pIQueryObject=pIDataBase->QueryInterface(Guid,dwQueryVer);

	return pIQueryObject;
}

//获取对象
VOID * CDataBaseEngineSink::GetDataBaseEngine(REFGUID Guid, DWORD dwQueryVer)
{
	//效验状态
	ASSERT(m_pIDataBaseEngine!=NULL);
	if (m_pIDataBaseEngine==NULL) return NULL;

	//查询对象
	VOID * pIQueryObject=m_pIDataBaseEngine->QueryInterface(Guid,dwQueryVer);

	return pIQueryObject;
}

//投递结果
bool CDataBaseEngineSink::PostGameDataBaseResult(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return true;
}

//启动事件
bool CDataBaseEngineSink::OnDataBaseEngineStart(IUnknownEx * pIUnknownEx)
{
	//查询对象
	ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IDataBaseEngine)!=NULL);
	m_pIDataBaseEngine=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IDataBaseEngine);

	//创建对象
	if ((m_GameDBModule.GetInterface()==NULL)&&(m_GameDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//创建对象
	if ((m_TreasureDBModule.GetInterface()==NULL)&&(m_TreasureDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//创建对象
	if ((m_PlatformDBModule.GetInterface()==NULL)&&(m_PlatformDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//连接游戏
	try
	{
		//连接信息
		tagDataBaseParameter * pTreasureDBParameter=&m_pInitParameter->m_TreasureDBParameter;
		tagDataBaseParameter * pPlatformDBParameter=&m_pInitParameter->m_PlatformDBParameter;

		//设置连接
		m_GameDBModule->SetConnectionInfo(m_pDataBaseParameter->szDataBaseAddr,m_pDataBaseParameter->wDataBasePort,
			m_pDataBaseParameter->szDataBaseName,m_pDataBaseParameter->szDataBaseUser,m_pDataBaseParameter->szDataBasePass);

		//设置连接
		m_TreasureDBModule->SetConnectionInfo(pTreasureDBParameter->szDataBaseAddr,pTreasureDBParameter->wDataBasePort,
			pTreasureDBParameter->szDataBaseName,pTreasureDBParameter->szDataBaseUser,pTreasureDBParameter->szDataBasePass);

		//设置连接
		m_PlatformDBModule->SetConnectionInfo(pPlatformDBParameter->szDataBaseAddr,pPlatformDBParameter->wDataBasePort,
			pPlatformDBParameter->szDataBaseName,pPlatformDBParameter->szDataBaseUser,pPlatformDBParameter->szDataBasePass);

		//发起连接
		m_GameDBModule->OpenConnection();
		m_GameDBAide.SetDataBase(m_GameDBModule.GetInterface());

		//发起连接
		m_TreasureDBModule->OpenConnection();
		m_TreasureDBAide.SetDataBase(m_TreasureDBModule.GetInterface());

		//发起连接
		m_PlatformDBModule->OpenConnection();
		m_PlatformDBAide.SetDataBase(m_PlatformDBModule.GetInterface());

		//数据钩子
		ASSERT(m_pIGameServiceManager!=NULL);
		m_pIGameDataBaseEngineSink=(IGameDataBaseEngineSink *)m_pIGameServiceManager->CreateGameDataBaseEngineSink(IID_IGameDataBaseEngineSink,VER_IGameDataBaseEngineSink);

		//配置对象
		if ((m_pIGameDataBaseEngineSink!=NULL)&&(m_pIGameDataBaseEngineSink->InitializeSink(QUERY_ME_INTERFACE(IUnknownEx))==false))
		{
			//错误断言
			ASSERT(FALSE);

			//输出信息
			CTraceService::TraceString(TEXT("游戏数据库扩展钩子引擎对象配置失败"),TraceLevel_Exception);

			return false;
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//停止事件
bool CDataBaseEngineSink::OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx)
{
	//解锁机器
	DWORD dwUserID=0;
	DBR_GR_UnlockAndroidUser UnlockAndroidUser;
	UnlockAndroidUser.wServerID=m_pGameServiceOption->wServerID;
	UnlockAndroidUser.wBatchID=0;
	OnRequestUnlockAndroidUser(0,&UnlockAndroidUser,sizeof(UnlockAndroidUser),dwUserID);

	//配置变量
	m_pInitParameter=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//组件变量
	m_pIGameServiceManager=NULL;
	m_pIDataBaseEngineEvent=NULL;

	//设置对象
	m_GameDBAide.SetDataBase(NULL);

	//释放对象
	SafeRelease(m_pIGameDataBaseEngineSink);

	//关闭连接
	if (m_GameDBModule.GetInterface()!=NULL)
	{
		m_GameDBModule->CloseConnection();
		m_GameDBModule.CloseInstance();
	}

	//关闭连接
	if (m_TreasureDBModule.GetInterface()!=NULL)
	{
		m_TreasureDBModule->CloseConnection();
		m_TreasureDBModule.CloseInstance();
	}

	//关闭连接
	if (m_PlatformDBModule.GetInterface()!=NULL)
	{
		m_PlatformDBModule->CloseConnection();
		m_PlatformDBModule.CloseInstance();
	}

	return true;
}

//时间事件
bool CDataBaseEngineSink::OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter)
{
	return false;
}

//控制事件
bool CDataBaseEngineSink::OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize)
{
	return false;
}

//请求事件
bool CDataBaseEngineSink::OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//变量定义
	bool bSucceed = false;
	DWORD dwUserID = 0L;

	//请求处理
	switch (wRequestID)
	{
	case DBR_GR_LOGON_USERID:			//I D 登录
		{
			bSucceed = OnRequestLogonUserID(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_LOGON_MOBILE:			//手机登录
		{
			bSucceed = OnRequestLogonMobile(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_WRITE_GAME_SCORE:		//游戏写分
		{
			bSucceed = OnRequestWriteGameScore(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_LEAVE_GAME_SERVER:		//离开房间
		{
			bSucceed = OnRequestLeaveGameServer(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_GAME_SCORE_RECORD:		//游戏记录
		{
			bSucceed = OnRequestGameScoreRecord(dwContextID, pData, wDataSize, dwUserID);
		}
		break;
	case DBR_GR_LOAD_PARAMETER:			//加载参数
		{
			bSucceed = OnRequestLoadParameter(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_LOAD_ANDROID_PARAMETER:	//加载配置
		{
			bSucceed = OnRequestLoadAndroidParameter(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_LOAD_ANDROID_USER:		//加载机器
		{
			bSucceed = OnRequestLoadAndroidUser(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_USER_ENABLE_INSURE:		//开通银行
		{
			bSucceed = OnRequestUserEnableInsure(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_USER_SAVE_SCORE:		//存入游戏币
		{
			bSucceed = OnRequestUserSaveScore(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_USER_TAKE_SCORE:		//提取游戏币
		{
			bSucceed = OnRequestUserTakeScore(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_USER_TRANSFER_SCORE:	//转帐游戏币
		{
			bSucceed = OnRequestUserTransferScore(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_QUERY_INSURE_INFO:		//查询银行
		{
			bSucceed = OnRequestQueryInsureInfo(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_QUERY_TRANSFER_USER_INFO:		//查询用户
		{
			bSucceed = OnRequestQueryTransferUserInfo(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_MANAGE_USER_RIGHT:		//用户权限
		{
			bSucceed = OnRequestManageUserRight(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_LOAD_SYSTEM_MESSAGE:   //系统消息
		{
			bSucceed = OnRequestLoadSystemMessage(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_LOAD_SENSITIVE_WORDS://加载敏感词
		{
			bSucceed = OnRequestLoadSensitiveWords(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_UNLOCK_ANDROID_USER://解锁机器人
		{
			bSucceed = OnRequestUnlockAndroidUser(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_UNLOCK_USER://解锁玩家
		{
			bSucceed = OnRequestDeblocking(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_MATCH_SIGNUP_START:		//报名开始
		{
			//请求处理
			bSucceed = OnRequestMatchSignupStart(dwContextID,pData,wDataSize,dwUserID);			
		}
		break;
	case DBR_GR_MATCH_QUERY_QUALIFY:	//查询资格
		{
			bSucceed = OnRequestMatchQueryQualify(dwContextID,pData,wDataSize,dwUserID);			
		}
		break;
	case DBR_GR_MATCH_BUY_SAFECARD:		//购买保险
		{
			bSucceed = OnRequestMatchBuySafeCard(dwContextID,pData,wDataSize,dwUserID);			
		}
		break;
	case DBR_GR_MATCH_START:			//比赛开始
		{
			//请求处理
			bSucceed = OnRequestMatchStart(dwContextID,pData,wDataSize,dwUserID);			
		}
		break;
	case DBR_GR_MATCH_ELIMINATE:		//比赛淘汰
		{
			//请求处理
			bSucceed = OnRequestMatchEliminate(dwContextID,pData,wDataSize,dwUserID);			
		}
		break;
	case DBR_GR_MATCH_QUERY_REVIVE:		//查询信息
		{
			bSucceed = OnRequestMatchQueryReviveInfo(dwContextID,pData,wDataSize,dwUserID);			
		}
		break;
	case DBR_GR_MATCH_USER_REVIVE:		//玩家复活
		{
			bSucceed = OnRequestMatchUserRevive(dwContextID,pData,wDataSize,dwUserID);			
		}
		break;
	case DBR_GR_MATCH_RECORD_GRADES:	//记录成绩
		{
			bSucceed = OnRequestMatchRecordGrades(dwContextID,pData,wDataSize,dwUserID);						
		}
		break;
	case DBR_GR_MATCH_OVER:				//比赛结束
		{
			bSucceed = OnRequestMatchOver(dwContextID,pData,wDataSize,dwUserID);			
		}
		break;
	case DBR_GR_MATCH_CANCEL:			//比赛取消
		{
			bSucceed = OnRequestMatchCancel(dwContextID,pData,wDataSize,dwUserID);			
		}
		break;				
	case DBR_GR_MATCH_REWARD:			//比赛奖励
		{
			bSucceed = OnRequestMatchReward(dwContextID,pData,wDataSize,dwUserID);			
		}
		break;	
	case DBR_GR_LOAD_BASEENSURE:		//加载低保
		{
			bSucceed =OnRequestLoadBaseEnsure(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_BASEENSURE_TAKE:		//领取低保
		{
			bSucceed =OnRequestTakeBaseEnsure(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	}

	//协调通知
	if(m_pIDBCorrespondManager) m_pIDBCorrespondManager->OnPostRequestComplete(dwUserID, bSucceed);

	return bSucceed;
}

//I D 登录
bool CDataBaseEngineSink::OnRequestLogonUserID(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID,BOOL bMatch)
{
	//执行查询
	DBR_GR_LogonUserID * pLogonUserID=(DBR_GR_LogonUserID *)pData;
	dwUserID = pLogonUserID->dwUserID;

	//效验参数
	ASSERT(wDataSize==sizeof(DBR_GR_LogonUserID));
	if (wDataSize!=sizeof(DBR_GR_LogonUserID)) return false;

	try
	{
		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonUserID->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),pLogonUserID->dwUserID);
		m_GameDBAide.AddParameter(TEXT("@strPassword"),pLogonUserID->szPassword);
		m_GameDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_GameDBAide.AddParameter(TEXT("@strMachineID"),pLogonUserID->szMachineID);
		m_GameDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);

		//比赛参数
		if ((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)!=0)
		{
			m_GameDBAide.AddParameter(TEXT("@dwMatchID"),pLogonUserID->dwMatchID);
			m_GameDBAide.AddParameter(TEXT("@lMatchNo"),pLogonUserID->lMatchNo);
			m_GameDBAide.AddParameter(TEXT("@cbMatchType"),pLogonUserID->cbMatchType);
		}

		//输出参数
		m_GameDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),m_LogonFailure.szDescribeString,sizeof(m_LogonFailure.szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_EfficacyUserID"),true);

		//用户信息
		lstrcpyn(m_LogonSuccess.szPassword,pLogonUserID->szPassword,CountArray(m_LogonSuccess.szPassword));
		lstrcpyn(m_LogonSuccess.szMachineID,pLogonUserID->szMachineID,CountArray(m_LogonSuccess.szMachineID));
	
		//结果处理
		CDBVarValue DBVarValue;
		m_GameDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
		//检测玩家是否为单控
		if(lResultCode==DB_SUCCESS)
		{
			//构造结构
			DBO_GR_UpdateControlInfo UpdateControlInfo;
			ZeroMemory(&UpdateControlInfo,sizeof(UpdateControlInfo));
			UpdateControlInfo.dwUserID = pLogonUserID->dwUserID;
			UpdateControlInfo.nWinRate = m_GameDBAide.GetValue_INT(TEXT("GameRate"));
			UpdateControlInfo.nRoomWinRate = m_GameDBAide.GetValue_INT(TEXT("GameRate2"));
			UpdateControlInfo.dwTailGameID = m_GameDBAide.GetValue_INT(TEXT("FollowGameID"));
			UpdateControlInfo.lTotalScore = m_GameDBAide.GetValue_INT(TEXT("TotalScore"));
			UpdateControlInfo.lTdTotalScore = m_GameDBAide.GetValue_INT(TEXT("TdTotalScore"));
			UpdateControlInfo.bRegister = true;
			//投递数据
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_UPDATE_CONTROLINFO,dwContextID,&UpdateControlInfo,sizeof(UpdateControlInfo));
		}
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"),false);

		return false;
	}

	return true;
}

//I D 登录
bool CDataBaseEngineSink::OnRequestLogonMobile(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//执行查询
	DBR_GR_LogonMobile * pLogonMobile=(DBR_GR_LogonMobile *)pData;
	dwUserID = pLogonMobile->dwUserID;

	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GR_LogonMobile));
		if (wDataSize!=sizeof(DBR_GR_LogonMobile)) return false;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonMobile->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),pLogonMobile->dwUserID);
		m_GameDBAide.AddParameter(TEXT("@strPassword"),pLogonMobile->szPassword);
		m_GameDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_GameDBAide.AddParameter(TEXT("@strMachineID"),pLogonMobile->szMachineID);
		m_GameDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);

		//输出参数
		m_GameDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),m_LogonFailure.szDescribeString,sizeof(m_LogonFailure.szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_EfficacyMobile"),true);

		//用户信息
		lstrcpyn(m_LogonSuccess.szPassword,pLogonMobile->szPassword,CountArray(m_LogonSuccess.szPassword));
		lstrcpyn(m_LogonSuccess.szMachineID,pLogonMobile->szMachineID,CountArray(m_LogonSuccess.szMachineID));
		m_LogonSuccess.cbDeviceType=pLogonMobile->cbDeviceType;
		m_LogonSuccess.wBehaviorFlags=pLogonMobile->wBehaviorFlags;
		m_LogonSuccess.wPageTableCount=pLogonMobile->wPageTableCount;
	
		//结果处理
		CDBVarValue DBVarValue;
		m_GameDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),true,pLogonMobile->cbDeviceType,pLogonMobile->wBehaviorFlags,pLogonMobile->wPageTableCount);
		if(lResultCode==DB_SUCCESS)
		{
			//构造结构
			DBO_GR_UpdateControlInfo UpdateControlInfo;
			ZeroMemory(&UpdateControlInfo,sizeof(UpdateControlInfo));
			UpdateControlInfo.dwUserID = pLogonMobile->dwUserID;
			UpdateControlInfo.nWinRate = m_GameDBAide.GetValue_INT(TEXT("GameRate"));
			UpdateControlInfo.nRoomWinRate = m_GameDBAide.GetValue_INT(TEXT("GameRate2"));
			UpdateControlInfo.dwTailGameID = m_GameDBAide.GetValue_INT(TEXT("FollowGameID"));
			UpdateControlInfo.lTotalScore = m_GameDBAide.GetValue_INT(TEXT("TotalScore"));
			UpdateControlInfo.lTdTotalScore = m_GameDBAide.GetValue_INT(TEXT("TdTotalScore"));
			UpdateControlInfo.bRegister = true;
			//投递数据
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_UPDATE_CONTROLINFO,dwContextID,&UpdateControlInfo,sizeof(UpdateControlInfo));
		}
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"),true);

		return false;
	}

	return true;
}

//游戏写分
bool CDataBaseEngineSink::OnRequestWriteGameScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//请求处理
	DBR_GR_WriteGameScore * pWriteGameScore=(DBR_GR_WriteGameScore *)pData;
	dwUserID=pWriteGameScore->dwUserID;

	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GR_WriteGameScore));
		if (wDataSize!=sizeof(DBR_GR_WriteGameScore)) return false;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pWriteGameScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();

		//用户信息
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"), pWriteGameScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@dwDBQuestID"), pWriteGameScore->dwDBQuestID);
		m_TreasureDBAide.AddParameter(TEXT("@dwInoutIndex"), pWriteGameScore->dwInoutIndex);
		m_TreasureDBAide.AddParameter(TEXT("@lStorageCurrent"), pWriteGameScore->lStorageCurrent);

		//变更成绩
		m_TreasureDBAide.AddParameter(TEXT("@lScore"),pWriteGameScore->VariationInfo.lScore);
		m_TreasureDBAide.AddParameter(TEXT("@lGrade"),pWriteGameScore->VariationInfo.lGrade);
		m_TreasureDBAide.AddParameter(TEXT("@lInsure"),pWriteGameScore->VariationInfo.lInsure);
		m_TreasureDBAide.AddParameter(TEXT("@lRevenue"),pWriteGameScore->VariationInfo.lRevenue);
		m_TreasureDBAide.AddParameter(TEXT("@lWinCount"),pWriteGameScore->VariationInfo.dwWinCount);
		m_TreasureDBAide.AddParameter(TEXT("@lLostCount"),pWriteGameScore->VariationInfo.dwLostCount);
		m_TreasureDBAide.AddParameter(TEXT("@lDrawCount"),pWriteGameScore->VariationInfo.dwDrawCount);
		m_TreasureDBAide.AddParameter(TEXT("@lFleeCount"),pWriteGameScore->VariationInfo.dwFleeCount);
		m_TreasureDBAide.AddParameter(TEXT("@lIntegralCount"),pWriteGameScore->VariationInfo.lIntegralCount);
		m_TreasureDBAide.AddParameter(TEXT("@lUserMedal"),pWriteGameScore->VariationInfo.lIngot);
		m_TreasureDBAide.AddParameter(TEXT("@lExperience"),pWriteGameScore->VariationInfo.dwExperience);
		m_TreasureDBAide.AddParameter(TEXT("@lLoveLiness"),pWriteGameScore->VariationInfo.lLoveLiness);
		m_TreasureDBAide.AddParameter(TEXT("@dwPlayTimeCount"),pWriteGameScore->VariationInfo.dwPlayTimeCount);
		m_TreasureDBAide.AddParameter(TEXT("@cbTaskForward"), 0);

		//属性信息
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"), m_pGameServiceOption->wKindID);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"), m_pGameServiceOption->wServerID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);

		//执行查询
		LONG lResultCode = m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_WriteGameScore"), true);

		if(lResultCode==DB_SUCCESS)
		{
			//构造结构
			DBO_GR_UpdateControlInfo UpdateControlInfo;
			ZeroMemory(&UpdateControlInfo,sizeof(UpdateControlInfo));
			UpdateControlInfo.dwUserID = pWriteGameScore->dwUserID;
			UpdateControlInfo.nWinRate = m_TreasureDBAide.GetValue_INT(TEXT("GameRate"));
			UpdateControlInfo.nRoomWinRate = m_TreasureDBAide.GetValue_INT(TEXT("GameRate2"));
			UpdateControlInfo.cbNullity = m_TreasureDBAide.GetValue_BYTE(TEXT("Nullity"));
			UpdateControlInfo.dwTailGameID = m_TreasureDBAide.GetValue_INT(TEXT("FollowGameID"));
			UpdateControlInfo.bRegister = false;
			//投递数据
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_UPDATE_CONTROLINFO,dwContextID,&UpdateControlInfo,sizeof(UpdateControlInfo));
		}
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//离开房间
bool CDataBaseEngineSink::OnRequestLeaveGameServer(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//请求处理
	DBR_GR_LeaveGameServer * pLeaveGameServer=(DBR_GR_LeaveGameServer *)pData;
	dwUserID=pLeaveGameServer->dwUserID;

	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GR_LeaveGameServer));
		if (wDataSize!=sizeof(DBR_GR_LeaveGameServer)) return false;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLeaveGameServer->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_GameDBAide.ResetParameter();

		//用户信息
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),pLeaveGameServer->dwUserID);

		m_GameDBAide.AddParameter(TEXT("@dwOnLineTimeCount"),pLeaveGameServer->dwOnLineTimeCount);

		//系统信息
		m_GameDBAide.AddParameter(TEXT("@dwInoutIndex"),pLeaveGameServer->dwInoutIndex);
		m_GameDBAide.AddParameter(TEXT("@dwLeaveReason"),pLeaveGameServer->dwLeaveReason);

		//记录成绩
		m_GameDBAide.AddParameter(TEXT("@lRecordScore"),pLeaveGameServer->RecordInfo.lScore);
		m_GameDBAide.AddParameter(TEXT("@lRecordGrade"),pLeaveGameServer->RecordInfo.lGrade);
		m_GameDBAide.AddParameter(TEXT("@lRecordInsure"),pLeaveGameServer->RecordInfo.lInsure);
		m_GameDBAide.AddParameter(TEXT("@lRecordRevenue"),pLeaveGameServer->RecordInfo.lRevenue);
		m_GameDBAide.AddParameter(TEXT("@lRecordWinCount"),pLeaveGameServer->RecordInfo.dwWinCount);
		m_GameDBAide.AddParameter(TEXT("@lRecordLostCount"),pLeaveGameServer->RecordInfo.dwLostCount);
		m_GameDBAide.AddParameter(TEXT("@lRecordDrawCount"),pLeaveGameServer->RecordInfo.dwDrawCount);
		m_GameDBAide.AddParameter(TEXT("@lRecordFleeCount"),pLeaveGameServer->RecordInfo.dwFleeCount);
		m_GameDBAide.AddParameter(TEXT("@lRecordUserMedal"),pLeaveGameServer->RecordInfo.lIngot);
		m_GameDBAide.AddParameter(TEXT("@lRecordExperience"),pLeaveGameServer->RecordInfo.dwExperience);
		m_GameDBAide.AddParameter(TEXT("@lRecordLoveLiness"),pLeaveGameServer->RecordInfo.lLoveLiness);
		m_GameDBAide.AddParameter(TEXT("@dwRecordPlayTimeCount"),pLeaveGameServer->RecordInfo.dwPlayTimeCount);

		//变更成绩
		m_GameDBAide.AddParameter(TEXT("@lVariationScore"),pLeaveGameServer->VariationInfo.lScore);
		m_GameDBAide.AddParameter(TEXT("@lVariationGrade"),pLeaveGameServer->VariationInfo.lGrade);
		m_GameDBAide.AddParameter(TEXT("@lVariationInsure"),pLeaveGameServer->VariationInfo.lInsure);
		m_GameDBAide.AddParameter(TEXT("@lVariationRevenue"),pLeaveGameServer->VariationInfo.lRevenue);
		m_GameDBAide.AddParameter(TEXT("@lVariationWinCount"),pLeaveGameServer->VariationInfo.dwWinCount);
		m_GameDBAide.AddParameter(TEXT("@lVariationLostCount"),pLeaveGameServer->VariationInfo.dwLostCount);
		m_GameDBAide.AddParameter(TEXT("@lVariationDrawCount"),pLeaveGameServer->VariationInfo.dwDrawCount);
		m_GameDBAide.AddParameter(TEXT("@lVariationFleeCount"),pLeaveGameServer->VariationInfo.dwFleeCount);
		m_GameDBAide.AddParameter(TEXT("@lVariationIntegralCount"),pLeaveGameServer->VariationInfo.lIntegralCount);
		m_GameDBAide.AddParameter(TEXT("@lVariationUserMedal"),pLeaveGameServer->VariationInfo.lIngot);
		m_GameDBAide.AddParameter(TEXT("@lVariationExperience"),pLeaveGameServer->VariationInfo.dwExperience);
		m_GameDBAide.AddParameter(TEXT("@lVariationLoveLiness"),pLeaveGameServer->VariationInfo.lLoveLiness);
		m_GameDBAide.AddParameter(TEXT("@dwVariationPlayTimeCount"),pLeaveGameServer->VariationInfo.dwPlayTimeCount);

		//其他参数
		m_GameDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_GameDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_GameDBAide.AddParameter(TEXT("@strMachineID"),pLeaveGameServer->szMachineID);

		//执行查询
		LONG lResultCode=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_LeaveGameServer"),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//游戏记录
bool CDataBaseEngineSink::OnRequestGameScoreRecord(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		//变量定义
		DBR_GR_GameScoreRecord * pGameScoreRecord = (DBR_GR_GameScoreRecord *)pData;
		dwUserID = INVALID_DWORD;

		//效验参数
		ASSERT(wDataSize <= sizeof(DBR_GR_GameScoreRecord));
		ASSERT(wDataSize >= (sizeof(DBR_GR_GameScoreRecord) - sizeof(pGameScoreRecord->GameScoreRecord)));
		ASSERT(wDataSize == (sizeof(DBR_GR_GameScoreRecord) - sizeof(pGameScoreRecord->GameScoreRecord) + pGameScoreRecord->wRecordCount*sizeof(pGameScoreRecord->GameScoreRecord[0])));

		//效验参数
		if (wDataSize>sizeof(DBR_GR_GameScoreRecord)) return false;
		if (wDataSize<(sizeof(DBR_GR_GameScoreRecord) - sizeof(pGameScoreRecord->GameScoreRecord))) return false;
		if (wDataSize != (sizeof(DBR_GR_GameScoreRecord) - sizeof(pGameScoreRecord->GameScoreRecord) + pGameScoreRecord->wRecordCount*sizeof(pGameScoreRecord->GameScoreRecord[0]))) return false;

		//房间信息
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@wKindID"), m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"), m_pGameServiceOption->wServerID);

		//桌子信息
		m_GameDBAide.AddParameter(TEXT("@wTableID"), pGameScoreRecord->wTableID);
		m_GameDBAide.AddParameter(TEXT("@wUserCount"), pGameScoreRecord->wUserCount);
		m_GameDBAide.AddParameter(TEXT("@wAndroidCount"), pGameScoreRecord->wAndroidCount);

		//税收损耗
		m_GameDBAide.AddParameter(TEXT("@lWasteCount"), pGameScoreRecord->lWasteCount);
		m_GameDBAide.AddParameter(TEXT("@lRevenueCount"), pGameScoreRecord->lRevenueCount);

		//统计信息
		m_GameDBAide.AddParameter(TEXT("@dwUserMemal"), pGameScoreRecord->dwUserMemal);
		m_GameDBAide.AddParameter(TEXT("@dwPlayTimeCount"), pGameScoreRecord->dwPlayTimeCount);

		//时间信息
		m_GameDBAide.AddParameter(TEXT("@SystemTimeStart"), pGameScoreRecord->SystemTimeStart);
		m_GameDBAide.AddParameter(TEXT("@SystemTimeConclude"), pGameScoreRecord->SystemTimeConclude);

		//执行查询
		LONG lResultCode = m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_RecordDrawInfo"), true);

		//写入记录
		if (lResultCode == DB_SUCCESS)
		{
			//获取标识
			DWORD dwDrawID = m_GameDBAide.GetValue_DWORD(TEXT("DrawID"));

			//写入记录
			for (WORD i = 0; i<pGameScoreRecord->wRecordCount; i++)
			{
				//重置参数
				m_GameDBAide.ResetParameter();

				//房间信息
				m_GameDBAide.AddParameter(TEXT("@dwDrawID"), dwDrawID);
				m_GameDBAide.AddParameter(TEXT("@dwUserID"), pGameScoreRecord->GameScoreRecord[i].dwUserID);
				m_GameDBAide.AddParameter(TEXT("@wChairID"), pGameScoreRecord->GameScoreRecord[i].wChairID);

				//用户信息
				m_GameDBAide.AddParameter(TEXT("@dwDBQuestID"), pGameScoreRecord->GameScoreRecord[i].dwDBQuestID);
				m_GameDBAide.AddParameter(TEXT("@dwInoutIndex"), pGameScoreRecord->GameScoreRecord[i].dwInoutIndex);

				//成绩信息
				m_GameDBAide.AddParameter(TEXT("@lScore"), pGameScoreRecord->GameScoreRecord[i].lScore);
				m_GameDBAide.AddParameter(TEXT("@lGrade"), pGameScoreRecord->GameScoreRecord[i].lGrade);
				m_GameDBAide.AddParameter(TEXT("@lRevenue"), pGameScoreRecord->GameScoreRecord[i].lRevenue);
				m_GameDBAide.AddParameter(TEXT("@dwUserMedal"), pGameScoreRecord->GameScoreRecord[i].dwUserMemal);
				m_GameDBAide.AddParameter(TEXT("@dwPlayTimeCount"), pGameScoreRecord->GameScoreRecord[i].dwPlayTimeCount);

				//执行查询
				m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_RecordDrawScore"), false);
			}
		}
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		TCHAR szInfo[260] = { 0 };
		wsprintf(szInfo, TEXT("%s   %s"), AnsiToUnicode(__FUNCTION__), pIException->GetExceptionDescribe());
		CTraceService::TraceString(szInfo, TraceLevel_Exception);

		return false;
	}

	return true;
}

//加载参数
bool CDataBaseEngineSink::OnRequestLoadParameter(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		//变量定义
		DBO_GR_GameParameter GameParameter;
		ZeroMemory(&GameParameter,sizeof(GameParameter));

		//执行查询
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);

		//执行查询
		LONG lResultCode=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_LoadParameter"),true);

		//读取信息
		if (lResultCode==DB_SUCCESS)
		{
			//汇率信息
			GameParameter.dwMedalRate=m_GameDBAide.GetValue_DWORD(TEXT("MedalRate"));
			GameParameter.dwRevenueRate=m_GameDBAide.GetValue_DWORD(TEXT("RevenueRate"));
			GameParameter.dwExchangeRate=m_GameDBAide.GetValue_DWORD(TEXT("ExchangeRate"));
			GameParameter.dwPresentExchangeRate=m_GameDBAide.GetValue_DWORD(TEXT("PresentExchangeRate"));
			GameParameter.dwRateGold=m_GameDBAide.GetValue_DWORD(TEXT("RateGold"));
			GameParameter.lHallNoticeThreshold=m_GameDBAide.GetValue_DOUBLE(TEXT("HighScoreAnnounce"));

			if (m_pGameServiceOption->wServerType==GAME_GENRE_EDUCATE)
			{
				GameParameter.lEducateGrantScore=m_GameDBAide.GetValue_DOUBLE(TEXT("EducateGrantScore"));
			}

			//经验奖励
			GameParameter.dwWinExperience=m_GameDBAide.GetValue_DWORD(TEXT("WinExperience"));

			//版本信息
			GameParameter.dwClientVersion=m_GameDBAide.GetValue_DWORD(TEXT("ClientVersion"));
			GameParameter.dwServerVersion=m_GameDBAide.GetValue_DWORD(TEXT("ServerVersion"));
		
			//发送信息
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_GAME_PARAMETER,dwContextID,&GameParameter,sizeof(GameParameter));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//加载机器
bool CDataBaseEngineSink::OnRequestLoadAndroidUser(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//参数校验
	ASSERT(sizeof(DBR_GR_LoadAndroidUser)==wDataSize);
	if(sizeof(DBR_GR_LoadAndroidUser)!=wDataSize) return false;

	try
	{		
		//提取变量
		DBR_GR_LoadAndroidUser * pLoadAndroidUser = (DBR_GR_LoadAndroidUser *)pData;
        if(pLoadAndroidUser==NULL) return false;

		//变量定义
		DBO_GR_GameAndroidInfo GameAndroidInfo;
		ZeroMemory(&GameAndroidInfo,sizeof(GameAndroidInfo));

		//设置批次
		GameAndroidInfo.dwBatchID=pLoadAndroidUser->dwBatchID;

		//用户帐户
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_TreasureDBAide.AddParameter(TEXT("@dwBatchID"),pLoadAndroidUser->dwBatchID);
		m_TreasureDBAide.AddParameter(TEXT("@dwAndroidCount"),pLoadAndroidUser->dwAndroidCount);
		m_TreasureDBAide.AddParameter(TEXT("@dwAndroidCountMember0"),pLoadAndroidUser->AndroidCountMember0);
		m_TreasureDBAide.AddParameter(TEXT("@dwAndroidCountMember1"),pLoadAndroidUser->AndroidCountMember1);
		m_TreasureDBAide.AddParameter(TEXT("@dwAndroidCountMember2"),pLoadAndroidUser->AndroidCountMember2);
		m_TreasureDBAide.AddParameter(TEXT("@dwAndroidCountMember3"),pLoadAndroidUser->AndroidCountMember3);
		m_TreasureDBAide.AddParameter(TEXT("@dwAndroidCountMember4"),pLoadAndroidUser->AndroidCountMember4);
		m_TreasureDBAide.AddParameter(TEXT("@dwAndroidCountMember5"),pLoadAndroidUser->AndroidCountMember5);
		//执行查询
		GameAndroidInfo.lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_LoadAndroidUser"),true);

		//读取信息
		for (WORD i=0;i<CountArray(GameAndroidInfo.AndroidAccountsInfo);i++)
		{
			//结束判断
			if (m_TreasureDBModule->IsRecordsetEnd()==true) break;

			//溢出效验
			ASSERT(GameAndroidInfo.wAndroidCount<CountArray(GameAndroidInfo.AndroidAccountsInfo));
			if (GameAndroidInfo.wAndroidCount>=CountArray(GameAndroidInfo.AndroidAccountsInfo)) break;

			//读取数据
			GameAndroidInfo.wAndroidCount++;
			GameAndroidInfo.AndroidAccountsInfo[i].dwUserID=m_TreasureDBAide.GetValue_DWORD(TEXT("UserID"));
			m_TreasureDBAide.GetValue_String(TEXT("LogonPass"),GameAndroidInfo.AndroidAccountsInfo[i].szPassword,CountArray(GameAndroidInfo.AndroidAccountsInfo[i].szPassword));

			//移动记录
			m_TreasureDBModule->MoveToNext();
		}

		//发送信息
		WORD wHeadSize=sizeof(GameAndroidInfo)-sizeof(GameAndroidInfo.AndroidAccountsInfo);
		WORD wDataSize=GameAndroidInfo.wAndroidCount*sizeof(GameAndroidInfo.AndroidAccountsInfo[0]);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_GAME_ANDROID_INFO,dwContextID,&GameAndroidInfo,wHeadSize+wDataSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//变量定义
		DBO_GR_GameAndroidInfo GameAndroidInfo;
		ZeroMemory(&GameAndroidInfo,sizeof(GameAndroidInfo));

		//构造变量
		GameAndroidInfo.wAndroidCount=0L;
		GameAndroidInfo.lResultCode=DB_ERROR;

		//发送信息
		WORD wHeadSize=sizeof(GameAndroidInfo)-sizeof(GameAndroidInfo.AndroidAccountsInfo);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_GAME_ANDROID_INFO,dwContextID,&GameAndroidInfo,wHeadSize);
	}

	return false;
}

//加载机器
bool CDataBaseEngineSink::OnRequestLoadAndroidParameter(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		//变量定义
		DBO_GR_GameAndroidParameter GameAndroidParameter;
		ZeroMemory(&GameAndroidParameter,sizeof(GameAndroidParameter));

		//用户帐户
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceAttrib->wKindID);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);

		//执行查询
		GameAndroidParameter.lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_LoadAndroidConfigure"),true);

		//读取信息
		for (WORD i=0;i<CountArray(GameAndroidParameter.AndroidParameter);i++)
		{
			//结束判断
			if (m_TreasureDBModule->IsRecordsetEnd()==true) break;

			//溢出效验
			ASSERT(GameAndroidParameter.wParameterCount<CountArray(GameAndroidParameter.AndroidParameter));
			if (GameAndroidParameter.wParameterCount>=CountArray(GameAndroidParameter.AndroidParameter)) break;

			//读取数据
			GameAndroidParameter.wParameterCount++;
			GameAndroidParameter.AndroidParameter[i].dwBatchID=m_TreasureDBAide.GetValue_DWORD(TEXT("BatchID"));
			GameAndroidParameter.AndroidParameter[i].dwServiceMode=m_TreasureDBAide.GetValue_DWORD(TEXT("ServiceMode"));
			GameAndroidParameter.AndroidParameter[i].dwAndroidCount=m_TreasureDBAide.GetValue_DWORD(TEXT("AndroidCount"));
			GameAndroidParameter.AndroidParameter[i].dwEnterTime=m_TreasureDBAide.GetValue_DWORD(TEXT("EnterTime"));
			GameAndroidParameter.AndroidParameter[i].dwLeaveTime=m_TreasureDBAide.GetValue_DWORD(TEXT("LeaveTime"));
			GameAndroidParameter.AndroidParameter[i].lTakeMinScore=m_TreasureDBAide.GetValue_DOUBLE(TEXT("TakeMinScore"));
			GameAndroidParameter.AndroidParameter[i].lTakeMaxScore=m_TreasureDBAide.GetValue_DOUBLE(TEXT("TakeMaxScore"));
			GameAndroidParameter.AndroidParameter[i].dwEnterMinInterval=m_TreasureDBAide.GetValue_DWORD(TEXT("EnterMinInterval"));
			GameAndroidParameter.AndroidParameter[i].dwEnterMaxInterval=m_TreasureDBAide.GetValue_DWORD(TEXT("EnterMaxInterval"));
			GameAndroidParameter.AndroidParameter[i].dwLeaveMinInterval=m_TreasureDBAide.GetValue_DWORD(TEXT("LeaveMinInterval"));
			GameAndroidParameter.AndroidParameter[i].dwLeaveMaxInterval=m_TreasureDBAide.GetValue_DWORD(TEXT("LeaveMaxInterval"));
			GameAndroidParameter.AndroidParameter[i].dwSwitchMinInnings=m_TreasureDBAide.GetValue_DWORD(TEXT("SwitchMinInnings"));
			GameAndroidParameter.AndroidParameter[i].dwSwitchMaxInnings=m_TreasureDBAide.GetValue_DWORD(TEXT("SwitchMaxInnings"));
			GameAndroidParameter.AndroidParameter[i].AndroidCountMember0 =m_TreasureDBAide.GetValue_DWORD(TEXT("AndroidCountMember0"));
			GameAndroidParameter.AndroidParameter[i].AndroidCountMember1 =m_TreasureDBAide.GetValue_DWORD(TEXT("AndroidCountMember1"));
			GameAndroidParameter.AndroidParameter[i].AndroidCountMember2 =m_TreasureDBAide.GetValue_DWORD(TEXT("AndroidCountMember2"));
			GameAndroidParameter.AndroidParameter[i].AndroidCountMember3 =m_TreasureDBAide.GetValue_DWORD(TEXT("AndroidCountMember3"));
			GameAndroidParameter.AndroidParameter[i].AndroidCountMember4 =m_TreasureDBAide.GetValue_DWORD(TEXT("AndroidCountMember4"));
			GameAndroidParameter.AndroidParameter[i].AndroidCountMember5 =m_TreasureDBAide.GetValue_DWORD(TEXT("AndroidCountMember5"));
			m_TreasureDBModule->MoveToNext();//移动记录
		}

		//发送信息
		WORD wHeadSize=sizeof(GameAndroidParameter)-sizeof(GameAndroidParameter.AndroidParameter);
		WORD wDataSize=GameAndroidParameter.wParameterCount*sizeof(GameAndroidParameter.AndroidParameter[0]);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_GAME_ANDROID_PARAMETER,dwContextID,&GameAndroidParameter,wHeadSize+wDataSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//变量定义
		DBO_GR_GameAndroidParameter GameAndroidParameter;
		ZeroMemory(&GameAndroidParameter,sizeof(GameAndroidParameter));

		//构造变量
		GameAndroidParameter.wParameterCount=0L;
		GameAndroidParameter.lResultCode=DB_ERROR;

		//发送信息
		WORD wHeadSize=sizeof(GameAndroidParameter)-sizeof(GameAndroidParameter.AndroidParameter);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_GAME_ANDROID_PARAMETER,dwContextID,&GameAndroidParameter,wHeadSize);
	}

	return false;
}




//开通银行
bool CDataBaseEngineSink::OnRequestUserEnableInsure(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GR_UserEnableInsure));
		if (wDataSize!=sizeof(DBR_GR_UserEnableInsure)) return false;

		//请求处理
		DBR_GR_UserEnableInsure * pUserEnableInsure=(DBR_GR_UserEnableInsure *)pData;

		//设置标识
		dwUserID=pUserEnableInsure->dwUserID;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserEnableInsure->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserEnableInsure->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@strLogonPass"),pUserEnableInsure->szLogonPass);
		m_TreasureDBAide.AddParameter(TEXT("@strInsurePass"),pUserEnableInsure->szInsurePass);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserEnableInsure->szMachineID);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserEnableInsure"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		//构造对象
		DBO_GR_UserInsureEnableResult UserEnableInsureResult;
		ZeroMemory(&UserEnableInsureResult,sizeof(UserEnableInsureResult));

		//设置变量
		UserEnableInsureResult.cbActivityGame=pUserEnableInsure->cbActivityGame;
		UserEnableInsureResult.cbInsureEnabled=(lResultCode==DB_SUCCESS)?TRUE:FALSE;
		lstrcpyn(UserEnableInsureResult.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(UserEnableInsureResult.szDescribeString));

		//发送结果
		WORD wDataSize=CountStringBuffer(UserEnableInsureResult.szDescribeString);
		WORD wHeadSize=sizeof(UserEnableInsureResult)-sizeof(UserEnableInsureResult.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_USER_INSURE_ENABLE_RESULT,dwContextID,&UserEnableInsureResult,wHeadSize+wDataSize);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//构造对象
		DBO_GR_UserInsureEnableResult UserEnableInsureResult;
		ZeroMemory(&UserEnableInsureResult,sizeof(UserEnableInsureResult));

		//请求处理
		DBR_GR_UserEnableInsure * pUserEnableInsure=(DBR_GR_UserEnableInsure *)pData;

		//设置变量
		UserEnableInsureResult.cbInsureEnabled=FALSE;
		UserEnableInsureResult.cbActivityGame=pUserEnableInsure->cbActivityGame;
		lstrcpyn(UserEnableInsureResult.szDescribeString,TEXT("由于数据库操作异常，请您稍后重试！"),CountArray(UserEnableInsureResult.szDescribeString));

		//发送结果
		WORD wDataSize=CountStringBuffer(UserEnableInsureResult.szDescribeString);
		WORD wHeadSize=sizeof(UserEnableInsureResult)-sizeof(UserEnableInsureResult.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_USER_INSURE_ENABLE_RESULT,dwContextID,&UserEnableInsureResult,wHeadSize+wDataSize);

		return false;
	}
}

//存入游戏币
bool CDataBaseEngineSink::OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(DBR_GR_UserSaveScore));
	if (wDataSize!=sizeof(DBR_GR_UserSaveScore)) return false;

	//变量定义
	DBR_GR_UserSaveScore * pUserSaveScore=(DBR_GR_UserSaveScore *)pData;
	dwUserID=pUserSaveScore->dwUserID;

	//请求处理
	try
	{
		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserSaveScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserSaveScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lSaveScore"),pUserSaveScore->lSaveScore);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserSaveScore->szMachineID);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserSaveScore"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnInsureDisposeResult(dwContextID,lResultCode,pUserSaveScore->lSaveScore,CW2CT(DBVarValue.bstrVal),false,pUserSaveScore->cbActivityGame);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnInsureDisposeResult(dwContextID,DB_ERROR,pUserSaveScore->lSaveScore,TEXT("由于数据库操作异常，请您稍后重试！"),false,pUserSaveScore->cbActivityGame);

		return false;
	}

	return true;
}

//提取游戏币
bool CDataBaseEngineSink::OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(DBR_GR_UserTakeScore));
	if (wDataSize!=sizeof(DBR_GR_UserTakeScore)) return false;

	//变量定义
	DBR_GR_UserTakeScore * pUserTakeScore=(DBR_GR_UserTakeScore *)pData;
	dwUserID=pUserTakeScore->dwUserID;

	//请求处理
	try
	{
		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserTakeScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserTakeScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lTakeScore"),pUserTakeScore->lTakeScore);
		m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pUserTakeScore->szPassword);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserTakeScore->szMachineID);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserTakeScore"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnInsureDisposeResult(dwContextID,lResultCode,0L,CW2CT(DBVarValue.bstrVal),false,pUserTakeScore->cbActivityGame);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("由于数据库操作异常，请您稍后重试！"),false,pUserTakeScore->cbActivityGame);

		return false;
	}

	return true;
}

//转帐游戏币
bool CDataBaseEngineSink::OnRequestUserTransferScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(DBR_GR_UserTransferScore));
	if (wDataSize!=sizeof(DBR_GR_UserTransferScore)) return false;

	//变量定义
	DBR_GR_UserTransferScore * pUserTransferScore=(DBR_GR_UserTransferScore *)pData;
	dwUserID=pUserTransferScore->dwUserID;

	//请求处理
	try
	{
		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserTransferScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserTransferScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lTransferScore"),pUserTransferScore->lTransferScore);
		m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pUserTransferScore->szPassword);
		m_TreasureDBAide.AddParameter(TEXT("@strNickName"),pUserTransferScore->szAccounts);
		m_TreasureDBAide.AddParameter(TEXT("@strTransRemark"),pUserTransferScore->szTransRemark);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserTransferScore->szMachineID);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserTransferScore"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnInsureDisposeResult(dwContextID,lResultCode,0L,CW2CT(DBVarValue.bstrVal),false,pUserTransferScore->cbActivityGame);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("由于数据库操作异常，请您稍后重试！"),false,pUserTransferScore->cbActivityGame);

		return false;
	}

	return true;
}

//查询银行
bool CDataBaseEngineSink::OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(DBR_GR_QueryInsureInfo));
	if (wDataSize!=sizeof(DBR_GR_QueryInsureInfo)) return false;

	//请求处理
	DBR_GR_QueryInsureInfo * pQueryInsureInfo=(DBR_GR_QueryInsureInfo *)pData;
	dwUserID=pQueryInsureInfo->dwUserID;

	try
	{
		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pQueryInsureInfo->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pQueryInsureInfo->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pQueryInsureInfo->szPassword);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//结果处理
		if (m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_QueryUserInsureInfo"),true)==DB_SUCCESS)
		{
			//变量定义
			DBO_GR_UserInsureInfo UserInsureInfo;
			ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

			//银行信息
			UserInsureInfo.cbActivityGame=pQueryInsureInfo->cbActivityGame;
			UserInsureInfo.cbEnjoinTransfer=m_TreasureDBAide.GetValue_BYTE(TEXT("EnjoinTransfer"));
			UserInsureInfo.wRevenueTake=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTake"));
			UserInsureInfo.wRevenueTransfer=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTransfer"));
			UserInsureInfo.wRevenueTransferMember=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTransferMember"));
			UserInsureInfo.wServerID=m_TreasureDBAide.GetValue_WORD(TEXT("ServerID"));
			UserInsureInfo.lUserScore=m_TreasureDBAide.GetValue_DOUBLE(TEXT("Score"));
			UserInsureInfo.lUserInsure=m_TreasureDBAide.GetValue_DOUBLE(TEXT("Insure"));
			UserInsureInfo.lTransferPrerequisite=m_TreasureDBAide.GetValue_DOUBLE(TEXT("TransferPrerequisite"));

			//发送结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_USER_INSURE_INFO,dwContextID,&UserInsureInfo,sizeof(UserInsureInfo));
		}
		else
		{
			//获取参数
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//错误处理
			OnInsureDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),0L,CW2CT(DBVarValue.bstrVal),false,pQueryInsureInfo->cbActivityGame);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//结果处理
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("由于数据库操作异常，请您稍后重试！"),false,pQueryInsureInfo->cbActivityGame);

		return false;
	}

	return true;
}

//查询用户
bool CDataBaseEngineSink::OnRequestQueryTransferUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(DBR_GR_QueryTransferUserInfo));
	if (wDataSize!=sizeof(DBR_GR_QueryTransferUserInfo)) return false;

	//请求处理
	DBR_GR_QueryTransferUserInfo * pQueryTransferUserInfo=(DBR_GR_QueryTransferUserInfo *)pData;
	dwUserID=pQueryTransferUserInfo->dwUserID;

	try
	{
		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@cbByNickName"),pQueryTransferUserInfo->cbByNickName);
		m_TreasureDBAide.AddParameter(TEXT("@strAccounts"),pQueryTransferUserInfo->szAccounts);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//结果处理
		if (m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_QueryTransferUserInfo"),true)==DB_SUCCESS)
		{
			//变量定义
			DBO_GR_UserTransferUserInfo TransferUserInfo;
			ZeroMemory(&TransferUserInfo,sizeof(TransferUserInfo));

			//银行信息
			TransferUserInfo.cbActivityGame=pQueryTransferUserInfo->cbActivityGame;
			TransferUserInfo.dwGameID=m_TreasureDBAide.GetValue_DWORD(TEXT("GameID"));
			m_TreasureDBAide.GetValue_String(TEXT("Accounts"), TransferUserInfo.szAccounts, CountArray(TransferUserInfo.szAccounts));

			//发送结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_USER_INSURE_USER_INFO,dwContextID,&TransferUserInfo,sizeof(TransferUserInfo));
		}
		else
		{
			//获取参数
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//错误处理
			OnInsureDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),0L,CW2CT(DBVarValue.bstrVal),false,pQueryTransferUserInfo->cbActivityGame);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//结果处理
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("由于数据库操作异常，请您稍后重试！"),false,pQueryTransferUserInfo->cbActivityGame);

		return false;
	}

	return true;
}

//两个字符转换成一个字符，长度为原来的1/2
void Hex2Char(char *szHex, unsigned char & rch)
{
	rch = 0;
	int i;
	for(i=0; i<2; i++)
	{
		if (i == 0)
		{
			if(*(szHex + i) >='0' && *(szHex + i) <= '9')
				rch += (*(szHex + i)  - '0') * 16;
			else if(*(szHex + i) >='a' && *(szHex + i) <= 'f' )
				rch +=  (*(szHex + i)  - 'a' + 10) * 16;
			else if(*(szHex + i) >='A' && *(szHex + i) <= 'F')
				rch +=  (*(szHex + i)  - 'A' + 10) * 16;
		}
		else
		{
			if(*(szHex + i) >='0' && *(szHex + i) <= '9')
				rch += (*(szHex + i)  - '0') ;
			else if(*(szHex + i) >='a' && *(szHex + i) <= 'f' )
				rch +=  *(szHex + i)  - 'a' + 10;
			else if(*(szHex + i) >='A' && *(szHex + i) <= 'F')
				rch +=  *(szHex + i)  - 'A'  + 10;
		}

	}
}


///十六进制char* 转 Binary char*函数
void HexStr2CharStr( char *pszHexStr, int iSize,  byte *pucCharStr)
{
	int i;
	unsigned char ch;
	if (iSize%2 != 0) return;
	for(i=0; i<iSize/2; i++)
	{
		Hex2Char(pszHexStr+2*i, ch);
		pucCharStr[i] = ch;
	}
}

//加载低保
bool CDataBaseEngineSink::OnRequestLoadBaseEnsure( DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID )
{
	try
	{
		
		dwUserID = 0;

		//构造参数
		m_PlatformDBAide.ResetParameter();

		//执行命令
		LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadBaseEnsure"),true);

		//执行成功
		if (lResultCode==DB_SUCCESS)
		{
			//构造结构
			DBO_GR_BaseEnsureParameter BaseEnsureParameter;
			ZeroMemory(&BaseEnsureParameter,sizeof(BaseEnsureParameter));

			//变量定义
			if (m_PlatformDBModule->IsRecordsetEnd()==false)
			{
				BaseEnsureParameter.cbTakeTimes= m_PlatformDBAide.GetValue_BYTE(TEXT("TakeTimes"));
				BaseEnsureParameter.lScoreAmount = m_PlatformDBAide.GetValue_DOUBLE(TEXT("ScoreAmount"));
				BaseEnsureParameter.lScoreCondition = m_PlatformDBAide.GetValue_DOUBLE(TEXT("ScoreCondition"));				

				//移动记录
				m_PlatformDBModule->MoveToNext();
			}

			//投递结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_BASEENSURE_PARAMETER,dwContextID,&BaseEnsureParameter,sizeof(BaseEnsureParameter));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}
}

//领取低保
bool CDataBaseEngineSink::OnRequestTakeBaseEnsure( DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID )
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GR_TakeBaseEnsure));
		if (wDataSize!=sizeof(DBR_GR_TakeBaseEnsure)) return false;

		//提取数据
		DBR_GR_TakeBaseEnsure * pTakeBaseEnsure=(DBR_GR_TakeBaseEnsure *)pData;

		dwUserID = pTakeBaseEnsure->dwUserID;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pTakeBaseEnsure->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pTakeBaseEnsure->dwUserID);
		m_PlatformDBAide.AddParameter(TEXT("@strPassword"),pTakeBaseEnsure->szPassword);
		m_PlatformDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),pTakeBaseEnsure->szMachineID);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_PlatformDBAide.AddParameterOutput(TEXT("@strNotifyContent"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行脚本
		LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_TakeBaseEnsure"),true);

		//变量定义
		DBO_GR_BaseEnsureResult BaseEnsureResult;
		ZeroMemory(&BaseEnsureResult,sizeof(BaseEnsureResult));

		//读取分数
		if(lResultCode==DB_SUCCESS && m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			BaseEnsureResult.lGameScore = m_PlatformDBAide.GetValue_DOUBLE(TEXT("Score"));
		}

		//获取参数
		CDBVarValue DBVarValue;
		m_PlatformDBModule->GetParameter(TEXT("@strNotifyContent"),DBVarValue);

		//银行信息
		BaseEnsureResult.bSuccessed=lResultCode==DB_SUCCESS;
		lstrcpyn(BaseEnsureResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(BaseEnsureResult.szNotifyContent));

		//发送结果
		WORD wSendSize=sizeof(BaseEnsureResult)-sizeof(BaseEnsureResult.szNotifyContent);
		wSendSize+=CountStringBuffer(BaseEnsureResult.szNotifyContent);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_BASEENSURE_RESULT,dwContextID,&BaseEnsureResult,wSendSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//构造结构
		DBO_GR_BaseEnsureResult BaseEnsureResult;
		ZeroMemory(&BaseEnsureResult,sizeof(BaseEnsureResult));

		//设置变量
		BaseEnsureResult.bSuccessed=false;
		lstrcpyn(BaseEnsureResult.szNotifyContent,TEXT("由于数据库操作异常，请您稍后重试！"),CountArray(BaseEnsureResult.szNotifyContent));

		//发送结果
		WORD wSendSize=sizeof(BaseEnsureResult)-sizeof(BaseEnsureResult.szNotifyContent);
		wSendSize+=CountStringBuffer(BaseEnsureResult.szNotifyContent);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_BASEENSURE_RESULT,dwContextID,&BaseEnsureResult,wSendSize);

		return false;
	}

	return true;
}

//报名开始
bool CDataBaseEngineSink::OnRequestMatchSignupStart(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(DBR_GR_SignupStart));
	if (wDataSize!=sizeof(DBR_GR_SignupStart)) return false;

	//变量定义
	DBR_GR_SignupStart * pSignupStart=(DBR_GR_SignupStart *)pData;	
	dwUserID=pSignupStart->dwMatchID;

	//请求处理
	try
	{
		//构造参数
		m_GameDBAide.ResetParameter();		
		m_GameDBAide.AddParameter(TEXT("@wServerID"),pSignupStart->dwMatchID);
		m_GameDBAide.AddParameter(TEXT("@dwMatchID"),pSignupStart->dwMatchID);
		m_GameDBAide.AddParameter(TEXT("@lMatchNo"),pSignupStart->lMatchNo);
		m_GameDBAide.AddParameter(TEXT("@cbMatchType"),pSignupStart->cbMatchType);
		m_GameDBAide.AddParameter(TEXT("@cbSignupMode"),pSignupStart->cbSignupMode);

		//结果处理
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_MatchSignupStart"),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//查询资格
bool CDataBaseEngineSink::OnRequestMatchQueryQualify(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(DBR_GR_QueryMatchQualify));
	if (wDataSize!=sizeof(DBR_GR_QueryMatchQualify)) return false;

	//变量定义
	DBR_GR_QueryMatchQualify * pQueryMatchQualify=(DBR_GR_QueryMatchQualify *)pData;
	dwUserID=pQueryMatchQualify->dwUserID;

	//请求处理
	try
	{
		//构造参数
		m_GameDBAide.ResetParameter();		
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),pQueryMatchQualify->dwUserID);
		m_GameDBAide.AddParameter(TEXT("@dwMatchID"),pQueryMatchQualify->dwMatchID);
		m_GameDBAide.AddParameter(TEXT("@lMatchNo"),pQueryMatchQualify->lMatchNo);

		//结果处理
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_MatchQueryQualify"),false);

		//构造结果
		DBO_GR_MatchQueryResult MatchQueryResult;
		MatchQueryResult.bHoldQualify=lReturnValue==DB_SUCCESS;

		//发送数据			
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_MATCH_QUERY_RESULT,dwContextID,&MatchQueryResult,sizeof(MatchQueryResult));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//购买保险
bool CDataBaseEngineSink::OnRequestMatchBuySafeCard(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(DBR_GR_MatchBuySafeCard));
	if (wDataSize!=sizeof(DBR_GR_MatchBuySafeCard)) return false;

	//变量定义
	DBR_GR_MatchBuySafeCard * pMatchBuySafeCard=(DBR_GR_MatchBuySafeCard *)pData;
	dwUserID=pMatchBuySafeCard->dwUserID;

	//请求处理
	try
	{
		//构造参数
		m_GameDBAide.ResetParameter();		
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),pMatchBuySafeCard->dwUserID);
		m_GameDBAide.AddParameter(TEXT("@dwMatchID"),pMatchBuySafeCard->dwMatchID);
		m_GameDBAide.AddParameter(TEXT("@lMatchNo"),pMatchBuySafeCard->lMatchNo);
		m_GameDBAide.AddParameter(TEXT("@lSafeCardFee"),pMatchBuySafeCard->lSafeCardFee);		

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_GameDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//结果处理
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_MatchBuySafeCard"),true);

		//构造结构
		DBO_GR_MatchBuyResult MatchBuyResult;
		ZeroMemory(&MatchBuyResult,sizeof(MatchBuyResult));

		//设置变量
		MatchBuyResult.bSuccessed=lReturnValue==DB_SUCCESS;

		//读取财富
		if(MatchBuyResult.bSuccessed==true)
		{
			MatchBuyResult.lCurrGold=m_GameDBAide.GetValue_DOUBLE(TEXT("Score"));
		}
		else
		{
			//错误描述
			CDBVarValue DBVarValue;
			m_GameDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
			lstrcpyn(MatchBuyResult.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(MatchBuyResult.szDescribeString));
		}

		//发送数据		
		WORD wSendDataSize=sizeof(MatchBuyResult)-sizeof(MatchBuyResult.szDescribeString);
		wSendDataSize+=CountStringBuffer(MatchBuyResult.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_MATCH_BUY_RESULT,dwContextID,&MatchBuyResult,wSendDataSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//构造结构
		DBO_GR_MatchBuyResult MatchBuyResult;
		ZeroMemory(&MatchBuyResult,sizeof(MatchBuyResult));

		//设置变量
		MatchBuyResult.bSuccessed=false;
		lstrcpyn(MatchBuyResult.szDescribeString,TEXT("数据库异常，保险卡购买失败！"),CountArray(MatchBuyResult.szDescribeString));

		//发送数据			
		WORD wSendDataSize=sizeof(MatchBuyResult)-sizeof(MatchBuyResult.szDescribeString);
		wSendDataSize+=CountStringBuffer(MatchBuyResult.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_MATCH_BUY_RESULT,dwContextID,&MatchBuyResult,wSendDataSize);

		return false;
	}

	return true;
}

//比赛开始
bool CDataBaseEngineSink::OnRequestMatchStart(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(DBR_GR_MatchStart));
	if (wDataSize!=sizeof(DBR_GR_MatchStart)) return false;

	//变量定义
	DBR_GR_MatchStart * pMatchStart=(DBR_GR_MatchStart *)pData;
	dwUserID=pMatchStart->dwMatchID;

	//请求处理
	try
	{
		//构造参数
		m_GameDBAide.ResetParameter();		
		m_GameDBAide.AddParameter(TEXT("@wServerID"),pMatchStart->dwMatchID);
		m_GameDBAide.AddParameter(TEXT("@dwMatchID"),pMatchStart->dwMatchID);
		m_GameDBAide.AddParameter(TEXT("@lMatchNo"),pMatchStart->lMatchNo);
		m_GameDBAide.AddParameter(TEXT("@cbMatchType"),pMatchStart->cbMatchType);

		//结果处理
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_MatchStart"),true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//比赛结束
bool CDataBaseEngineSink::OnRequestMatchOver(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(DBR_GR_MatchOver));
	if (wDataSize!=sizeof(DBR_GR_MatchOver)) return false;

	//变量定义
	DBR_GR_MatchOver * pMatchOver=(DBR_GR_MatchOver *)pData;
	dwUserID=pMatchOver->dwMatchID;

	//请求处理
	try
	{
		//构造参数
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@wServerID"),pMatchOver->wServerID);	
		m_GameDBAide.AddParameter(TEXT("@dwMatchID"),pMatchOver->dwMatchID);
		m_GameDBAide.AddParameter(TEXT("@lMatchNo"),pMatchOver->lMatchNo);
		m_GameDBAide.AddParameter(TEXT("@cbMatchType"),pMatchOver->cbMatchType);					
		m_GameDBAide.AddParameter(TEXT("@cbPlayCount"),pMatchOver->wPlayCount);	
		m_GameDBAide.AddParameter(TEXT("@bMatchFinish"),pMatchOver->bMatchFinish);	
		m_GameDBAide.AddParameter(TEXT("@MatchStartTime"),pMatchOver->MatchStartTime);	
		m_GameDBAide.AddParameter(TEXT("@MatchEndTime"),pMatchOver->MatchEndTime);	

		//结果处理
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_MatchOver"),true);

		//执行成功
		if(lReturnValue==DB_SUCCESS)
		{
			//构造结构
			DBO_GR_MatchRankList MatchRankList;
			ZeroMemory(&MatchRankList,sizeof(MatchRankList));

			//变量定义
			tagMatchRankInfo * pMatchRankInfo=NULL;

			//设置变量
			MatchRankList.dwMatchID=pMatchOver->dwMatchID;
			MatchRankList.lMatchNo=pMatchOver->lMatchNo;
			MatchRankList.bMatchFinish=pMatchOver->bMatchFinish;

			//读取记录
			while(m_GameDBModule->IsRecordsetEnd()==false)
			{
				pMatchRankInfo = &MatchRankList.MatchRankInfo[MatchRankList.wUserCount++];
				pMatchRankInfo->wRankID=m_GameDBAide.GetValue_WORD(TEXT("RankID"));
				pMatchRankInfo->dwUserID=m_GameDBAide.GetValue_DWORD(TEXT("UserID"));
				pMatchRankInfo->lMatchScore=m_GameDBAide.GetValue_DOUBLE(TEXT("Score"));
				pMatchRankInfo->lRewardGold=m_GameDBAide.GetValue_DOUBLE(TEXT("RewardGold"));
				pMatchRankInfo->lRewardIngot=m_GameDBAide.GetValue_DOUBLE(TEXT("RewardIngot"));
				pMatchRankInfo->dwRewardExperience=m_GameDBAide.GetValue_DWORD(TEXT("RewardExperience"));

				//移动游标 
				m_GameDBModule->MoveToNext();
			}

			//发送数据
			WORD wSendDataSize = sizeof(MatchRankList)-sizeof(MatchRankList.MatchRankInfo);
			wSendDataSize += MatchRankList.wUserCount*sizeof(MatchRankList.MatchRankInfo[0]);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_MATCH_RANK_LIST,dwContextID,&MatchRankList,wSendDataSize);
		}
		
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//比赛取消
bool CDataBaseEngineSink::OnRequestMatchCancel(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(DBR_GR_MatchCancel));
	if (wDataSize!=sizeof(DBR_GR_MatchCancel)) return false;

	//变量定义
	DBR_GR_MatchCancel * pMatchCancel=(DBR_GR_MatchCancel *)pData;
	dwUserID=pMatchCancel->dwMatchID;

	//请求处理
	try
	{
		//构造参数
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@wServerID"),pMatchCancel->wServerID);	
		m_GameDBAide.AddParameter(TEXT("@dwMatchID"),pMatchCancel->dwMatchID);
		m_GameDBAide.AddParameter(TEXT("@lMatchNo"),pMatchCancel->lMatchNo);
		m_GameDBAide.AddParameter(TEXT("@lSafeCardFee"),pMatchCancel->lSafeCardFee);
		m_GameDBAide.AddParameter(TEXT("@bMatchFinish"),pMatchCancel->bMatchFinish);	

		//结果处理
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_MatchCancel"),false);

		//构造结构
		DBO_GR_MatchCancelResult MatchCancelResult;		
		MatchCancelResult.bSuccessed=lReturnValue==DB_SUCCESS;
		MatchCancelResult.bMatchFinish=pMatchCancel->bMatchFinish;

		//发送结果
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_MATCH_CANCEL_RESULT,dwContextID,&MatchCancelResult,sizeof(MatchCancelResult));
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//比赛奖励
bool CDataBaseEngineSink::OnRequestMatchReward(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(DBR_GR_MatchReward));
	if (wDataSize!=sizeof(DBR_GR_MatchReward)) return false;

	//变量定义
	DBR_GR_MatchReward * pMatchReward=(DBR_GR_MatchReward *)pData;
	dwUserID=pMatchReward->dwUserID;

	//请求处理
	try
	{
		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pMatchReward->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),pMatchReward->dwUserID);		
		m_GameDBAide.AddParameter(TEXT("@lRewardGold"),pMatchReward->lRewardGold);
		m_GameDBAide.AddParameter(TEXT("@lRewardIngot"),pMatchReward->lRewardIngot);		
		m_GameDBAide.AddParameter(TEXT("@dwRewardExperience"),pMatchReward->dwRewardExperience);
		m_GameDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_GameDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//结果处理
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_MatchReward"),true);

		//构造结构
		DBO_GR_MatchRewardResult MatchRewardResult;
		ZeroMemory(&MatchRewardResult,sizeof(MatchRewardResult));

		//设置变量
		MatchRewardResult.dwUserID=pMatchReward->dwUserID;
		MatchRewardResult.bResultCode=lReturnValue==DB_SUCCESS;

		//读取财富
		if(MatchRewardResult.bResultCode==true)
		{
			MatchRewardResult.lCurrGold=m_GameDBAide.GetValue_DOUBLE(TEXT("Score"));
		}

		//发送结果
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_MATCH_REWARD_RESULT,dwContextID,&MatchRewardResult,sizeof(MatchRewardResult));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//构造结构
		//DBO_GR_MatchRewardResult MatchRewardResult;
		//ZeroMemory(&MatchRewardResult,sizeof(MatchRewardResult));

		////设置变量
		//MatchRewardResult.bResultCode=false;

		////发送结果
		//m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_MATCH_REWARD_RESULT,dwContextID,&MatchRewardResult,sizeof(MatchRewardResult));

		return false;
	}

	return true;
}

//比赛淘汰
bool CDataBaseEngineSink::OnRequestMatchEliminate(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(DBR_GR_MatchEliminate));
	if (wDataSize!=sizeof(DBR_GR_MatchEliminate)) return false;

	//变量定义
	DBR_GR_MatchEliminate * pMatchEliminate=(DBR_GR_MatchEliminate *)pData;
	dwUserID=pMatchEliminate->dwUserID;

	//请求处理
	try
	{
		//构造参数
		m_GameDBAide.ResetParameter();		
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),pMatchEliminate->dwUserID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),pMatchEliminate->wServerID);
		m_GameDBAide.AddParameter(TEXT("@dwMatchID"),pMatchEliminate->dwMatchID);
		m_GameDBAide.AddParameter(TEXT("@lMatchNo"),pMatchEliminate->lMatchNo);	
		m_GameDBAide.AddParameter(TEXT("@cbMatchType"),pMatchEliminate->cbMatchType);

		//结果处理
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_MatchEliminate"),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}


//记录成绩
bool CDataBaseEngineSink::OnRequestMatchRecordGrades(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(DBR_GR_MatchRecordGrades));
	if (wDataSize!=sizeof(DBR_GR_MatchRecordGrades)) return false;

	//变量定义
	DBR_GR_MatchRecordGrades * pMatchRecordGrades=(DBR_GR_MatchRecordGrades *)pData;
	dwUserID=pMatchRecordGrades->dwUserID;

	//请求处理
	try
	{
		//构造参数
		m_GameDBAide.ResetParameter();		
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),pMatchRecordGrades->dwUserID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),pMatchRecordGrades->wServerID);		
		m_GameDBAide.AddParameter(TEXT("@dwMatchID"),pMatchRecordGrades->dwMatchID);
		m_GameDBAide.AddParameter(TEXT("@lMatchNo"),pMatchRecordGrades->lMatchNo);
		m_GameDBAide.AddParameter(TEXT("@lInitScore"),pMatchRecordGrades->lInitScore);

		//结果处理
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_MatchRecordGrades"),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//玩家复活
bool CDataBaseEngineSink::OnRequestMatchUserRevive(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(DBR_GR_MatchUserRevive));
	if (wDataSize!=sizeof(DBR_GR_MatchUserRevive)) return false;

	//变量定义
	DBR_GR_MatchUserRevive * pMatchUserRevive=(DBR_GR_MatchUserRevive *)pData;
	dwUserID=pMatchUserRevive->dwUserID;

	//请求处理
	try
	{
		//构造参数
		m_GameDBAide.ResetParameter();		
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),pMatchUserRevive->dwUserID);
		m_GameDBAide.AddParameter(TEXT("@dwMatchID"),pMatchUserRevive->dwMatchID);
		m_GameDBAide.AddParameter(TEXT("@lMatchNo"),pMatchUserRevive->lMatchNo);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);		
		m_GameDBAide.AddParameter(TEXT("@InitScore"),pMatchUserRevive->lInitScore);
		m_GameDBAide.AddParameter(TEXT("@CullScore"),pMatchUserRevive->lCullScore);
		m_GameDBAide.AddParameter(TEXT("@lReviveFee"),pMatchUserRevive->lReviveFee);		
		m_GameDBAide.AddParameter(TEXT("@cbReviveTimes"),pMatchUserRevive->cbReviveTimes);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_GameDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//结果处理
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_MatchUserRevive"),true);

		//构造结构
		DBO_GR_MatchReviveResult MatchReviveResult;
		ZeroMemory(&MatchReviveResult,sizeof(MatchReviveResult));

		//设置变量
		MatchReviveResult.bSuccessed=lReturnValue==DB_SUCCESS;

		//读取财富
		if(MatchReviveResult.bSuccessed==true)
		{
			MatchReviveResult.lCurrGold=m_GameDBAide.GetValue_DOUBLE(TEXT("Score"));
		}
		else
		{
			//错误描述
			CDBVarValue DBVarValue;
			m_GameDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
			lstrcpyn(MatchReviveResult.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(MatchReviveResult.szDescribeString));
		}

		//发送数据	
		WORD wSendDataSize=sizeof(MatchReviveResult)-sizeof(MatchReviveResult.szDescribeString);
		wSendDataSize+=CountStringBuffer(MatchReviveResult.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_MATCH_REVIVE_RESULT,dwContextID,&MatchReviveResult,wSendDataSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//构造结构
		DBO_GR_MatchReviveResult MatchReviveResult;
		ZeroMemory(&MatchReviveResult,sizeof(MatchReviveResult));

		//设置变量
		MatchReviveResult.bSuccessed=false;
		lstrcpyn(MatchReviveResult.szDescribeString,TEXT("数据库异常，复活失败！"),CountArray(MatchReviveResult.szDescribeString));

		//发送数据			
		WORD wSendDataSize=sizeof(MatchReviveResult)-sizeof(MatchReviveResult.szDescribeString);
		wSendDataSize+=CountStringBuffer(MatchReviveResult.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_MATCH_REVIVE_RESULT,dwContextID,&MatchReviveResult,wSendDataSize);

		return true;
	}

	return true;
}

//查询复活
bool CDataBaseEngineSink::OnRequestMatchQueryReviveInfo(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(DBR_GR_MatchQueryRevive));
	if (wDataSize!=sizeof(DBR_GR_MatchQueryRevive)) return false;

	//变量定义
	DBR_GR_MatchQueryRevive * pMatchQueryRevive=(DBR_GR_MatchQueryRevive *)pData;
	dwUserID=pMatchQueryRevive->dwUserID;

	//请求处理
	try
	{
		//构造参数
		m_GameDBAide.ResetParameter();		
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),pMatchQueryRevive->dwUserID);
		m_GameDBAide.AddParameter(TEXT("@dwMatchID"),pMatchQueryRevive->dwMatchID);
		m_GameDBAide.AddParameter(TEXT("@lMatchNo"),pMatchQueryRevive->lMatchNo);

		//结果处理
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_MatchQueryReviveInfo"),true);

		//构造结果
		DBO_GR_MatchReviveInfo MatchReviveInfo;
		ZeroMemory(&MatchReviveInfo,sizeof(MatchReviveInfo));

		//读取次数
		if(lReturnValue==DB_SUCCESS)
		{
			MatchReviveInfo.bSuccessed=true;
			MatchReviveInfo.bSafeCardRevive=(m_GameDBAide.GetValue_BYTE(TEXT("SafeCardRevive"))==TRUE);
			MatchReviveInfo.cbReviveTimesed=m_GameDBAide.GetValue_BYTE(TEXT("ReviveTimes"));
		}

		//发送数据			
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_MATCH_REVIVE_INFO,dwContextID,&MatchReviveInfo,sizeof(MatchReviveInfo));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//构造结果
		DBO_GR_MatchReviveInfo MatchReviveInfo;
		ZeroMemory(&MatchReviveInfo,sizeof(MatchReviveInfo));

		//发送数据			
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_MATCH_REVIVE_INFO,dwContextID,&MatchReviveInfo,sizeof(MatchReviveInfo));

		return true;
	}

	return true;
}

//用户权限
bool CDataBaseEngineSink::OnRequestManageUserRight(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GR_ManageUserRight));
		if (wDataSize!=sizeof(DBR_GR_ManageUserRight)) return false;

		//请求处理
		DBR_GR_ManageUserRight * pManageUserRight=(DBR_GR_ManageUserRight *)pData;
		dwUserID=pManageUserRight->dwUserID;

		//构造参数
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),pManageUserRight->dwUserID);
		m_GameDBAide.AddParameter(TEXT("@dwAddRight"),pManageUserRight->dwAddRight);
		m_GameDBAide.AddParameter(TEXT("@dwRemoveRight"),pManageUserRight->dwRemoveRight);

		//执行查询
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_ManageUserRight"),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//比赛权限
bool CDataBaseEngineSink::OnRequestManageMatchRight(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GR_ManageUserRight));
		if (wDataSize!=sizeof(DBR_GR_ManageUserRight)) return false;

		//请求处理
		DBR_GR_ManageMatchRight * pManageMatchRight=(DBR_GR_ManageMatchRight *)pData;
		dwUserID=pManageMatchRight->dwUserID;

		//构造参数
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),pManageMatchRight->dwUserID);
		m_GameDBAide.AddParameter(TEXT("@dwAddRight"),pManageMatchRight->dwAddRight);
		m_GameDBAide.AddParameter(TEXT("@dwRemoveRight"),pManageMatchRight->dwRemoveRight);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_GameDBAide.AddParameter(TEXT("@dwMatchID"),pManageMatchRight->dwMatchID);
		m_GameDBAide.AddParameter(TEXT("@dwMatchNo"),pManageMatchRight->dwMatchNO);

		//执行查询
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_ManageMatchRight"),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("由于数据库操作异常，请您稍后重试！"),false);

		return false;
	}

	return true;
}

//系统消息
bool CDataBaseEngineSink::OnRequestLoadSystemMessage(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		//构造参数
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);

		//执行查询
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_LoadSystemMessage"),true);

		//结果处理
		if(lReturnValue==0)
		{
			TCHAR szServerID[32]={0};
			_sntprintf(szServerID, CountArray(szServerID), TEXT("%d"), m_pGameServiceOption->wServerID);

			while(true)
			{
				//结束判断
				if (m_GameDBModule->IsRecordsetEnd()==true) break;

				//定义变量
				TCHAR szServerRange[1024]={0};
				CString strServerRange;
				bool bSendMessage=false;
				bool bAllRoom=false;

				//读取范围
				m_GameDBAide.GetValue_String(TEXT("ServerRange"), szServerRange, CountArray(szServerRange));
				szServerRange[1023]=0;
				strServerRange.Format(TEXT("%s"), szServerRange);

				//范围判断
				while(true)
				{
					int nfind=strServerRange.Find(TEXT(','));
					if(nfind!=-1 && nfind>0)
					{
						CString strID=strServerRange.Left(nfind);
						WORD wServerID=StrToInt(strID);
						bSendMessage=(wServerID==0 || wServerID==m_pGameServiceOption->wServerID);
						if(wServerID==0)bAllRoom=true;

						if(bSendMessage) break;

						strServerRange=strServerRange.Right(strServerRange.GetLength()-nfind-1);
					}
					else
					{
						WORD wServerID=StrToInt(szServerRange);
						bSendMessage=(wServerID==0 || wServerID==m_pGameServiceOption->wServerID);
						if(wServerID==0)bAllRoom=true;

						break;
					}
				}

				//发送消息
				if(bSendMessage)
				{
					//定义变量
					DBR_GR_SystemMessage SystemMessage;
					ZeroMemory(&SystemMessage, sizeof(SystemMessage));

					//读取消息
					SystemMessage.dwMessageID=m_GameDBAide.GetValue_DWORD(TEXT("ID"));
					SystemMessage.cbMessageType=m_GameDBAide.GetValue_BYTE(TEXT("MessageType"));
					SystemMessage.dwTimeRate=m_GameDBAide.GetValue_DWORD(TEXT("TimeRate"));
					SystemMessage.cbAllRoom=bAllRoom?TRUE:FALSE;
					m_GameDBAide.GetValue_String(TEXT("MessageString"),SystemMessage.szSystemMessage,CountArray(SystemMessage.szSystemMessage));

					//读取时间
					SYSTEMTIME systime;
					ZeroMemory(&systime, sizeof(systime));

					//开始时间
					m_GameDBAide.GetValue_SystemTime(TEXT("StartTime"),systime);
					CTime StarTime(systime);
					SystemMessage.tStartTime=StarTime.GetTime();

					//结束时间
					m_GameDBAide.GetValue_SystemTime(TEXT("ConcludeTime"),systime);
					CTime ConcludeTime(systime);
					SystemMessage.tConcludeTime=ConcludeTime.GetTime();

					//发送结果
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_SYSTEM_MESSAGE_RESULT,dwContextID,&SystemMessage,sizeof(SystemMessage));
				}

				//下一条
				m_GameDBModule->MoveToNext();
			}

			//加载完成
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_SYSTEM_MESSAGE_FINISH,dwContextID,NULL,0);			
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("由于数据库操作异常，请您稍后重试！"),false);

		return false;
	}

	return true;
}


//加载敏感词
bool CDataBaseEngineSink::OnRequestLoadSensitiveWords(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		//构造参数
		m_PlatformDBAide.ResetParameter();

		//执行查询
		LONG lReturnValue=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GR_LoadSensitiveWords"),true);

		//读取信息
		if (lReturnValue==DB_SUCCESS)
		{
			//起始消息
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_SENSITIVE_WORDS,0xfffe,NULL,0);

			//读取消息
			while (m_PlatformDBModule->IsRecordsetEnd()==false)
			{
				//变量定义
				TCHAR szSensitiveWords[32]=TEXT("");

				//读取消息
				m_PlatformDBAide.GetValue_String(TEXT("SensitiveWords"),szSensitiveWords,CountArray(szSensitiveWords));				

				//发送消息
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_SENSITIVE_WORDS,0,szSensitiveWords,sizeof(szSensitiveWords));

				//移动记录
				m_PlatformDBModule->MoveToNext();
			};

			//结束消息
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_SENSITIVE_WORDS,0xffff,NULL,0);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//解锁机器人
bool CDataBaseEngineSink::OnRequestUnlockAndroidUser(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GR_UnlockAndroidUser));
		if (wDataSize!=sizeof(DBR_GR_UnlockAndroidUser)) return false;

		//请求处理
		DBR_GR_UnlockAndroidUser * pUnlockAndroidUser=(DBR_GR_UnlockAndroidUser *)pData;

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),pUnlockAndroidUser->wServerID);
		m_TreasureDBAide.AddParameter(TEXT("@wBatchID"),pUnlockAndroidUser->wBatchID);

		//执行查询
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UnlockAndroidUser"),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}

	return true;
}
//解锁玩家
bool CDataBaseEngineSink::OnRequestDeblocking(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		DBR_GR_Deblocking * pDeblocking=(DBR_GR_Deblocking *)pData;
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GR_Deblocking));
		if (wDataSize!=sizeof(DBR_GR_Deblocking)) return false;


		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pDeblocking->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),pDeblocking->dwServerID);


		//执行查询
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("P_UnLockUserInGame"),false);
	}
	catch(...)
	{
		TCHAR szInfo[260] = {0};
		wsprintf(szInfo, TEXT("OnRequestWriteJoinInfo 函数中有错误") );
		CTraceService::TraceString(szInfo,TraceLevel_Exception);
	}


	return true;
}
//登录结果
VOID CDataBaseEngineSink::OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient,BYTE cbDeviceType,WORD wBehaviorFlags,WORD wPageTableCount)
{
	if (dwErrorCode==DB_SUCCESS)
	{
		//属性资料
		m_LogonSuccess.wFaceID=m_GameDBAide.GetValue_WORD(TEXT("FaceID"));
		m_LogonSuccess.dwUserID=m_GameDBAide.GetValue_DWORD(TEXT("UserID"));
		m_LogonSuccess.dwGameID=m_GameDBAide.GetValue_DWORD(TEXT("GameID"));
		m_LogonSuccess.dwGroupID=m_GameDBAide.GetValue_DWORD(TEXT("GroupID"));
		m_LogonSuccess.dwCustomID=m_GameDBAide.GetValue_DWORD(TEXT("CustomID"));
		m_GameDBAide.GetValue_String(TEXT("NickName"),m_LogonSuccess.szNickName,CountArray(m_LogonSuccess.szNickName));
		m_GameDBAide.GetValue_String(TEXT("GroupName"),m_LogonSuccess.szGroupName,CountArray(m_LogonSuccess.szGroupName));

		//用户资料
		m_LogonSuccess.cbGender=m_GameDBAide.GetValue_BYTE(TEXT("Gender"));
		m_LogonSuccess.cbMemberOrder=m_GameDBAide.GetValue_BYTE(TEXT("MemberOrder"));
		m_LogonSuccess.cbMasterOrder=m_GameDBAide.GetValue_BYTE(TEXT("MasterOrder"));
		m_GameDBAide.GetValue_String(TEXT("UnderWrite"),m_LogonSuccess.szUnderWrite,CountArray(m_LogonSuccess.szUnderWrite));

		//用户资料
		m_GameDBAide.GetValue_String(TEXT("QQ"),m_LogonSuccess.szQQ,CountArray(m_LogonSuccess.szQQ));
		m_GameDBAide.GetValue_String(TEXT("EMail"),m_LogonSuccess.szEMail,CountArray(m_LogonSuccess.szEMail));
		m_GameDBAide.GetValue_String(TEXT("SeatPhone"),m_LogonSuccess.szSeatPhone,CountArray(m_LogonSuccess.szSeatPhone));
		m_GameDBAide.GetValue_String(TEXT("MobilePhone"),m_LogonSuccess.szMobilePhone,CountArray(m_LogonSuccess.szMobilePhone));
		m_GameDBAide.GetValue_String(TEXT("DwellingPlace"),m_LogonSuccess.szDwellingPlace,CountArray(m_LogonSuccess.szDwellingPlace));
		m_GameDBAide.GetValue_String(TEXT("PostalCode"),m_LogonSuccess.szPostalCode,CountArray(m_LogonSuccess.szPostalCode));

		//积分信息
		m_LogonSuccess.lScore=m_GameDBAide.GetValue_DOUBLE(TEXT("Score"));
		m_LogonSuccess.lIngot=m_GameDBAide.GetValue_DOUBLE(TEXT("Ingot"));
		m_LogonSuccess.lGrade=m_GameDBAide.GetValue_DOUBLE(TEXT("Grade"));
		m_LogonSuccess.lInsure=m_GameDBAide.GetValue_DOUBLE(TEXT("Insure"));
		m_LogonSuccess.dBeans=m_GameDBAide.GetValue_DOUBLE(TEXT("Beans"));

		//局数信息
		m_LogonSuccess.dwWinCount=m_GameDBAide.GetValue_LONG(TEXT("WinCount"));
		m_LogonSuccess.dwLostCount=m_GameDBAide.GetValue_LONG(TEXT("LostCount"));
		m_LogonSuccess.dwDrawCount=m_GameDBAide.GetValue_LONG(TEXT("DrawCount"));
		m_LogonSuccess.dwFleeCount=m_GameDBAide.GetValue_LONG(TEXT("FleeCount"));		
		m_LogonSuccess.dwExperience=m_GameDBAide.GetValue_LONG(TEXT("Experience"));
		m_LogonSuccess.lLoveLiness=m_GameDBAide.GetValue_LONG(TEXT("LoveLiness"));
		m_LogonSuccess.lIntegralCount=m_GameDBAide.GetValue_DOUBLE(TEXT("IntegralCount"));

		//代理信息
		m_LogonSuccess.dwAgentID=m_GameDBAide.GetValue_LONG(TEXT("AgentID"));

		//附加信息		
		m_LogonSuccess.dwUserRight=m_GameDBAide.GetValue_DWORD(TEXT("UserRight"));
		m_LogonSuccess.dwMasterRight=m_GameDBAide.GetValue_DWORD(TEXT("MasterRight"));
		m_LogonSuccess.cbDeviceType=cbDeviceType;
		m_LogonSuccess.wBehaviorFlags=wBehaviorFlags;
		m_LogonSuccess.wPageTableCount=wPageTableCount;

		//索引变量
		m_LogonSuccess.dwInoutIndex=m_GameDBAide.GetValue_DWORD(TEXT("InoutIndex"));

		//获取信息
		if(pszErrorString!=NULL) lstrcpyn(m_LogonSuccess.szDescribeString,pszErrorString,CountArray(m_LogonSuccess.szDescribeString));

		//发送结果
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_LOGON_SUCCESS, dwContextID, &m_LogonSuccess, sizeof(m_LogonSuccess));
	}
	else
	{
		//变量定义
		DBO_GR_LogonFailure LogonFailure;
		ZeroMemory(&LogonFailure,sizeof(LogonFailure));

		//构造数据
		LogonFailure.lResultCode=dwErrorCode;
		lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

		//因为锁房间登陆失败 所以要把锁住的游戏KindID和房间ServerID发回去
		if (dwErrorCode == 100)
		{
			LogonFailure.dwLockKindID = m_GameDBAide.GetValue_DWORD(TEXT("LockKindID"));
			LogonFailure.dwLockServerID = m_GameDBAide.GetValue_DWORD(TEXT("LockServerID"));
		}

		//发送结果
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_LOGON_FAILURE, dwContextID, &LogonFailure, sizeof(LogonFailure));
	}

	return;
}

//银行结果
VOID CDataBaseEngineSink::OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, SCORE lFrozenedScore, LPCTSTR pszErrorString, bool bMobileClient,BYTE cbActivityGame)
{
	if (dwErrorCode==DB_SUCCESS)
	{
		//变量定义
		DBO_GR_UserInsureSuccess UserInsureSuccess;
		ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

		//构造变量
		UserInsureSuccess.cbActivityGame=cbActivityGame;
		UserInsureSuccess.lFrozenedScore=lFrozenedScore;
		UserInsureSuccess.dwUserID=m_TreasureDBAide.GetValue_DWORD(TEXT("UserID"));
		UserInsureSuccess.lSourceScore=m_TreasureDBAide.GetValue_DOUBLE(TEXT("SourceScore"));
		UserInsureSuccess.lSourceInsure=m_TreasureDBAide.GetValue_DOUBLE(TEXT("SourceInsure"));
		UserInsureSuccess.lInsureRevenue=m_TreasureDBAide.GetValue_DOUBLE(TEXT("InsureRevenue"));
		UserInsureSuccess.lVariationScore=m_TreasureDBAide.GetValue_DOUBLE(TEXT("VariationScore"));
		UserInsureSuccess.lVariationInsure=m_TreasureDBAide.GetValue_DOUBLE(TEXT("VariationInsure"));
		lstrcpyn(UserInsureSuccess.szDescribeString,pszErrorString,CountArray(UserInsureSuccess.szDescribeString));

		//发送结果
		WORD wDataSize=CountStringBuffer(UserInsureSuccess.szDescribeString);
		WORD wHeadSize=sizeof(UserInsureSuccess)-sizeof(UserInsureSuccess.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_USER_INSURE_SUCCESS,dwContextID,&UserInsureSuccess,wHeadSize+wDataSize);
	}
	else
	{
		//变量定义
		DBO_GR_UserInsureFailure UserInsureFailure;
		ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

		//构造变量
		UserInsureFailure.cbActivityGame=cbActivityGame;
		UserInsureFailure.lResultCode=dwErrorCode;
		UserInsureFailure.lFrozenedScore=lFrozenedScore;
		lstrcpyn(UserInsureFailure.szDescribeString,pszErrorString,CountArray(UserInsureFailure.szDescribeString));

		//发送结果
		WORD wDataSize=CountStringBuffer(UserInsureFailure.szDescribeString);
		WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_USER_INSURE_FAILURE,dwContextID,&UserInsureFailure,wHeadSize+wDataSize);
	}

	return;
}

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
