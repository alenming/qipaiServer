#include "StdAfx.h"
#include "ServiceUnits.h"
#include "DataBaseEngineSink.h"

//构造函数
CDataBaseEngineSink::CDataBaseEngineSink()
{
	//组件变量
	m_pInitParameter=NULL;
	m_pIDataBaseEngineEvent=NULL;

	return;
}

//析构函数
CDataBaseEngineSink::~CDataBaseEngineSink()
{
}

//接口查询
VOID * CDataBaseEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseEngineSink,Guid,dwQueryVer);
	return NULL;
}

//启动事件
bool CDataBaseEngineSink::OnDataBaseEngineStart(IUnknownEx * pIUnknownEx)
{
	//创建对象
	if ((m_AccountsDBModule.GetInterface()==NULL)&&(m_AccountsDBModule.CreateInstance()==false))
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

	try
	{
		//连接信息
		tagDataBaseParameter * pAccountsDBParameter=&m_pInitParameter->m_AccountsDBParameter;
		tagDataBaseParameter * pTreasureDBParameter=&m_pInitParameter->m_TreasureDBParameter;
		tagDataBaseParameter * pPlatformDBParameter=&m_pInitParameter->m_PlatformDBParameter;

		//设置连接
		m_AccountsDBModule->SetConnectionInfo(pAccountsDBParameter->szDataBaseAddr,pAccountsDBParameter->wDataBasePort,
			pAccountsDBParameter->szDataBaseName,pAccountsDBParameter->szDataBaseUser,pAccountsDBParameter->szDataBasePass);
		m_TreasureDBModule->SetConnectionInfo(pTreasureDBParameter->szDataBaseAddr,pTreasureDBParameter->wDataBasePort,
			pTreasureDBParameter->szDataBaseName,pTreasureDBParameter->szDataBaseUser,pTreasureDBParameter->szDataBasePass);
		m_PlatformDBModule->SetConnectionInfo(pPlatformDBParameter->szDataBaseAddr,pPlatformDBParameter->wDataBasePort,
			pPlatformDBParameter->szDataBaseName,pPlatformDBParameter->szDataBaseUser,pPlatformDBParameter->szDataBasePass);

		//发起连接
		m_AccountsDBModule->OpenConnection();
		m_AccountsDBAide.SetDataBase(m_AccountsDBModule.GetInterface());

		//发起连接
		m_TreasureDBModule->OpenConnection();
		m_TreasureDBAide.SetDataBase(m_TreasureDBModule.GetInterface());

		//发起连接
		m_PlatformDBModule->OpenConnection();
		m_PlatformDBAide.SetDataBase(m_PlatformDBModule.GetInterface());

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		return false;
	}

	return true;
}

//停止事件
bool CDataBaseEngineSink::OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx)
{
	//设置对象
	m_AccountsDBAide.SetDataBase(NULL);
	m_TreasureDBAide.SetDataBase(NULL);
	m_PlatformDBAide.SetDataBase(NULL);

	//关闭连接
	if (m_AccountsDBModule.GetInterface()!=NULL)
	{
		m_AccountsDBModule->CloseConnection();
		m_AccountsDBModule.CloseInstance();
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

	//组件变量
	m_pInitParameter=NULL;
	m_pIDataBaseEngineEvent=NULL;

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
	switch (wRequestID)
	{
	case DBR_GP_LOAD_PLATFORM_PARAMETER: //平台参数
		{
			return OnRequestPlatformParameter(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_INSURE_PASS:		//修改密码
		{
			return OnRequestModifyInsurePass(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_SYSTEM_FACE:		//修改头像
		{
			return OnRequestModifySystemFace(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_INDIVIDUAL:		//修改资料
		{
			return OnRequestModifyIndividual(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_ENABLE_INSURE:		//开通银行
		{
			return OnRequestUserEnableInsure(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_SAVE_SCORE:		//存入游戏币
		{
			return OnRequestUserSaveScore(dwContextID,pData,wDataSize);
		}
	case DBR_GP_EXCHANGE_BANK_SCORE:	//兑换金币
		{
			return OnRequestExchangeBankScore(dwContextID, pData, wDataSize);
		}
	case DBR_GP_USER_TAKE_SCORE:		//提取游戏币
		{
			return OnRequestUserTakeScore(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_TRANSFER_SCORE:	//转帐游戏币
		{
			return OnRequestUserTransferScore(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_INSURE_INFO:		//查询银行
		{
			return OnRequestQueryInsureInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_USER_INFO:		//查询用户
		{
			return OnRequestQueryTransferUserInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOAD_GAME_LIST:			//加载列表
		{
			return OnRequestLoadGameList(dwContextID,pData,wDataSize);
		}
	case DBR_GP_ONLINE_COUNT_INFO:		//在线信息
		{
			return OnRequestOnLineCountInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_SERVER_DUMMY_ONLINE:	//房间虚拟人数
		{
			return OnRequestServerDummyOnLine(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOAD_BASEENSURE:		//加载低保
		{
			return OnRequestLoadBaseEnsure(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_PARAMETER:			//获取参数
		{
			return OnRequestGetParameter(dwContextID,pData,wDataSize);
		}
	case DBR_GP_ADD_PARAMETER:			//添加参数
		{
			return OnRequestAddParameter(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_PARAMETER:		//修改参数
		{
			return OnRequestModifyParameter(dwContextID,pData,wDataSize);
		}
	case DBR_GP_DELETE_PARAMETER:		//删除参数
		{
			return OnRequestDeleteParameter(dwContextID,pData,wDataSize);
		}
	case DBR_GP_BASEENSURE_TAKE:		//领取低保
		{
			return OnRequestTakeBaseEnsure(dwContextID,pData,wDataSize);
		}
	case DBR_MB_LOGON_GAMEID:			//标识登录
		{
			return OnMobileLogonGameID(dwContextID,pData,wDataSize);
		}
	case DBR_MB_LOGON_ACCOUNTS:			//帐号登录
		{
			return OnMobileLogonAccounts(dwContextID,pData,wDataSize);
		}
	case DBR_MB_REGISTER_ACCOUNTS:		//注册帐号
		{
			return OnMobileRegisterAccounts(dwContextID,pData,wDataSize);
		}
	case DBR_MB_LOGON_OTHERPLATFORM:	//其他登录
		{
			return OnMobileLogonOtherPlatform(dwContextID,pData,wDataSize);
		}
	case DBR_MB_LOGON_VISITOR:			//游客登陆
		{
			return OnMobileLogonVisitor(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOTTERY_CONFIG_REQ:		//请求配置
		{
			return OnRequestLotteryConfigReq(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOTTERY_START:			//抽奖开始
		{
			return OnRequestLotteryStart(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_EMAIL:			//获取邮件
		{
			return OnRequestQueryEmail(dwContextID, pData, wDataSize);
		}
	}

	return false;
}

//I D 登录
bool CDataBaseEngineSink::OnMobileLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_MB_LogonGameID));
		if (wDataSize!=sizeof(DBR_MB_LogonGameID)) return false;

		//执行查询
		DBR_MB_LogonGameID * pLogonGameID=(DBR_MB_LogonGameID *)pData;

		//变量定义
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonGameID->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonGameID->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwGameID"),pLogonGameID->dwGameID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonGameID->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonGameID->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pLogonGameID->szMobilePhone);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_EfficacyGameID"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),0,true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"),0,true);

		return false;
	}

	return true;
}

//帐号登录
bool CDataBaseEngineSink::OnMobileLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_MB_LogonAccounts));
		if (wDataSize!=sizeof(DBR_MB_LogonAccounts)) return false;

		//请求处理
		DBR_MB_LogonAccounts * pLogonAccounts=(DBR_MB_LogonAccounts *)pData;

		//执行判断
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pLogonAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonAccounts->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pLogonAccounts->szMobilePhone);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_EfficacyAccounts"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),0,true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"),0,true);

		return false;
	}

	return true;
}

//其他登录
bool CDataBaseEngineSink::OnMobileLogonOtherPlatform(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_MB_LogonOtherPlatform));
		if (wDataSize!=sizeof(DBR_MB_LogonOtherPlatform)) return false;

		//请求处理
		DBR_MB_LogonOtherPlatform * pLogonOtherPlatform=(DBR_MB_LogonOtherPlatform *)pData;

		//执行判断
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonOtherPlatform->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonOtherPlatform->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@cbPlatformID"),pLogonOtherPlatform->cbPlatformID);
		m_AccountsDBAide.AddParameter(TEXT("@strUserUin"),pLogonOtherPlatform->szUserUin);
		m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pLogonOtherPlatform->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@strCompellation"),pLogonOtherPlatform->szCompellation);
		m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pLogonOtherPlatform->cbGender);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonOtherPlatform->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pLogonOtherPlatform->szMobilePhone);
		m_AccountsDBAide.AddParameter(TEXT("@strAgentID"),pLogonOtherPlatform->szAgentID);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_EfficacyOtherPlatform"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),0,true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"),0,true);

		return false;
	}

	return true;
}

//游客登录
bool CDataBaseEngineSink::OnMobileLogonVisitor(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_MB_LogonVisitor));
		if (wDataSize!=sizeof(DBR_MB_LogonVisitor)) return false;

		//请求处理
		DBR_MB_LogonVisitor * pVisitorLogon=(DBR_MB_LogonVisitor *)pData;

		//执行判断
		tagBindParameter * pBindParameter=(tagBindParameter *)pVisitorLogon->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pVisitorLogon->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@cbPlatformID"),pVisitorLogon->cbPlatformID);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pVisitorLogon->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pVisitorLogon->szMobilePhone);
		m_AccountsDBAide.AddParameter(TEXT("@strAgentID"),pVisitorLogon->szAgentID);
		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_EfficacyLogonVisitor"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),0,true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"),0,true);

		return false;
	}

	return true;
}

//帐号注册
bool CDataBaseEngineSink::OnMobileRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_MB_RegisterAccounts));
		if (wDataSize!=sizeof(DBR_MB_RegisterAccounts)) return false;

		//请求处理
		DBR_MB_RegisterAccounts * pRegisterAccounts=(DBR_MB_RegisterAccounts *)pData;

		//执行判断
		tagBindParameter * pBindParameter=(tagBindParameter *)pRegisterAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pRegisterAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pRegisterAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pRegisterAccounts->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@dwSpreaderGameID"),pRegisterAccounts->dwSpreaderGameID);
		m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"),pRegisterAccounts->szLogonPass);
		m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pRegisterAccounts->wFaceID);
		m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pRegisterAccounts->cbGender);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pRegisterAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pRegisterAccounts->szMobilePhone);
		m_AccountsDBAide.AddParameter(TEXT("@cbDeviceType"),pRegisterAccounts->cbDeviceType);
		m_AccountsDBAide.AddParameter(TEXT("@strAgentID"),pRegisterAccounts->szAgentID);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_RegisterAccounts"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),0,true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试或选择另一服务器登录！"),0,true);

		return false;
	}

	return true;
}

//修改头像
bool CDataBaseEngineSink::OnRequestModifySystemFace(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_ModifySystemFace));
		if (wDataSize!=sizeof(DBR_GP_ModifySystemFace)) return false;

		//请求处理
		DBR_GP_ModifySystemFace * pModifySystemFace=(DBR_GP_ModifySystemFace *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifySystemFace->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifySystemFace->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifySystemFace->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pModifySystemFace->wFaceID);

		//机器信息
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pModifySystemFace->szMachineID);

		//输出变量
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//结果处理
		if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_SystemFaceInsert"),true)==DB_SUCCESS)
		{
			//变量定义
			DBO_GP_UserFaceInfo UserFaceInfo;
			ZeroMemory(&UserFaceInfo,sizeof(UserFaceInfo));

			UserFaceInfo.bSuccess = true;
			//读取信息
			UserFaceInfo.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));

			//发送结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_FACE_INFO,dwContextID,&UserFaceInfo,sizeof(UserFaceInfo));
		}
		else
		{
			//获取参数
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//变量定义
			DBO_GP_UserFaceInfo UserFaceInfo;
			ZeroMemory(&UserFaceInfo, sizeof(UserFaceInfo));

			UserFaceInfo.bSuccess = false;
			//读取信息
			lstrcpyn(UserFaceInfo.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(UserFaceInfo.szDescribeString));

			//发送结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_FACE_INFO, dwContextID, &UserFaceInfo, sizeof(UserFaceInfo));
		}
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//结果处理
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试！"),false);

		return false;
	}

	return true;
}

//修改密码
bool CDataBaseEngineSink::OnRequestModifyInsurePass(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyInsurePass));
		if (wDataSize!=sizeof(DBR_GP_ModifyInsurePass)) return false;

		//请求处理
		DBR_GP_ModifyInsurePass * pModifyInsurePass=(DBR_GP_ModifyInsurePass *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyInsurePass->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyInsurePass->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strScrPassword"),pModifyInsurePass->szScrPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strDesPassword"),pModifyInsurePass->szDesPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//输出变量
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//结果处理
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyInsurePassword"),false);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//结果处理
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试！"),false);

		return false;
	}

	return true;
}

//修改资料
bool CDataBaseEngineSink::OnRequestModifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyIndividual));
		if (wDataSize!=sizeof(DBR_GP_ModifyIndividual)) return false;

		//请求处理
		DBR_GP_ModifyIndividual * pModifyIndividual=(DBR_GP_ModifyIndividual *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyIndividual->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyIndividual->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyIndividual->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pModifyIndividual->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//输出变量
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyUserNickName"),true);

		//结果处理
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		DBO_GP_IndividualResult IndividualResult;
		IndividualResult.bSuccessed = lResultCode == DB_SUCCESS;
		lstrcpyn(IndividualResult.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(IndividualResult.szDescribeString));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_INDIVIDUAL_RESULT, dwContextID, &IndividualResult, sizeof(IndividualResult));
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//结果处理
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试！"),false);

		return false;
	}

	return true;
}

//开通银行
bool CDataBaseEngineSink::OnRequestUserEnableInsure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_UserEnableInsure));
		if (wDataSize!=sizeof(DBR_GP_UserEnableInsure)) return false;

		//请求处理
		DBR_GP_UserEnableInsure * pUserEnableInsure=(DBR_GP_UserEnableInsure *)pData;

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
		DBO_GP_UserInsureEnableResult UserEnableInsureResult;
		ZeroMemory(&UserEnableInsureResult,sizeof(UserEnableInsureResult));

		//设置变量
		UserEnableInsureResult.cbInsureEnabled=(lResultCode==DB_SUCCESS)?TRUE:FALSE;
		lstrcpyn(UserEnableInsureResult.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(UserEnableInsureResult.szDescribeString));

		//发送结果
		WORD wDataSize=CountStringBuffer(UserEnableInsureResult.szDescribeString);
		WORD wHeadSize=sizeof(UserEnableInsureResult)-sizeof(UserEnableInsureResult.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_ENABLE_RESULT,dwContextID,&UserEnableInsureResult,wHeadSize+wDataSize);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//构造对象
		DBO_GP_UserInsureEnableResult UserEnableInsureResult;
		ZeroMemory(&UserEnableInsureResult,sizeof(UserEnableInsureResult));

		//设置变量
		UserEnableInsureResult.cbInsureEnabled=FALSE;
		lstrcpyn(UserEnableInsureResult.szDescribeString,TEXT("由于数据库操作异常，请您稍后重试！"),CountArray(UserEnableInsureResult.szDescribeString));

		//发送结果
		WORD wDataSize=CountStringBuffer(UserEnableInsureResult.szDescribeString);
		WORD wHeadSize=sizeof(UserEnableInsureResult)-sizeof(UserEnableInsureResult.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_ENABLE_RESULT,dwContextID,&UserEnableInsureResult,wHeadSize+wDataSize);

		return false;
	}
}

//存入游戏币
bool CDataBaseEngineSink::OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_UserSaveScore));
		if (wDataSize!=sizeof(DBR_GP_UserSaveScore)) return false;

		//请求处理
		DBR_GP_UserSaveScore * pUserSaveScore=(DBR_GP_UserSaveScore *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserSaveScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserSaveScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lSaveScore"),pUserSaveScore->lSaveScore);
		ShowLog(TEXT("%.2f---存入的金币"),pUserSaveScore->lSaveScore);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
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

		//变量定义
		DBO_GP_UserSaveResult UserSaveResult;
		ZeroMemory(&UserSaveResult, sizeof(UserSaveResult));

		//构造变量
		UserSaveResult.bSuccess = lResultCode == DB_SUCCESS;
		if (lResultCode == DB_SUCCESS)
		{
			UserSaveResult.lScore = m_TreasureDBAide.GetValue_DOUBLE(TEXT("Score"));
			UserSaveResult.lInsureScore = m_TreasureDBAide.GetValue_DOUBLE(TEXT("Insure"));
		}
		lstrcpyn(UserSaveResult.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(UserSaveResult.szDescribeString));

		//发送结果
		WORD wDataSize = CountStringBuffer(UserSaveResult.szDescribeString);
		WORD wHeadSize = sizeof(UserSaveResult) - sizeof(UserSaveResult.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_SAVE_RESULT, dwContextID, &UserSaveResult, wHeadSize + wDataSize);
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//变量定义
		DBO_GP_UserSaveResult UserSaveResult;
		ZeroMemory(&UserSaveResult, sizeof(UserSaveResult));

		//构造变量
		UserSaveResult.bSuccess = false;
		lstrcpyn(UserSaveResult.szDescribeString, TEXT("由于数据库操作异常，请您稍后重试！"), CountArray(UserSaveResult.szDescribeString));

		//发送结果
		WORD wDataSize = CountStringBuffer(UserSaveResult.szDescribeString);
		WORD wHeadSize = sizeof(UserSaveResult) - sizeof(UserSaveResult.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_SAVE_RESULT, dwContextID, &UserSaveResult, wHeadSize + wDataSize);
		return false;
	}

	return true;
}

//兑换金币
bool CDataBaseEngineSink::OnRequestExchangeBankScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize == sizeof(DBR_GP_ExchangeBankScore));
		if (wDataSize != sizeof(DBR_GP_ExchangeBankScore)) return false;

		//请求处理
		DBR_GP_ExchangeBankScore * pDBRData = (DBR_GP_ExchangeBankScore *)pData;

		//转化地址
		TCHAR szClientAddr[16] = TEXT("");
		BYTE * pClientAddr = (BYTE *)&pDBRData->dwClientAddr;
		_sntprintf(szClientAddr, CountArray(szClientAddr), TEXT("%d.%d.%d.%d"), pClientAddr[0], pClientAddr[1], pClientAddr[2], pClientAddr[3]);


		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), pDBRData->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@dwType"), pDBRData->dwType);
		m_AccountsDBAide.AddParameter(TEXT("@dwScore"), pDBRData->dwScore);
		m_AccountsDBAide.AddParameter(TEXT("@strSafePwd"), pDBRData->szBankPass);
		m_AccountsDBAide.AddParameter(TEXT("@strOrderID"), pDBRData->szOrderID);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);


		//输出参数
		TCHAR szDescribeString[128] = TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

		//执行查询
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("P_Acc_PlayerDraw"), true);

		//结果处理
		//获取参数
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);

		DBO_GP_ExchangeScoreResult ExchangeScoreResult;
		ZeroMemory(&ExchangeScoreResult, sizeof(ExchangeScoreResult));

		ExchangeScoreResult.bSuccess = lResultCode == DB_SUCCESS;
		if (lResultCode == DB_SUCCESS)
		{
			ExchangeScoreResult.lScore = m_AccountsDBAide.GetValue_DOUBLE(TEXT("Score"));
			ExchangeScoreResult.lInsureScore = m_AccountsDBAide.GetValue_DOUBLE(TEXT("InsureScore"));
		}
		lstrcpyn(ExchangeScoreResult.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(ExchangeScoreResult.szDescribeString));
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_EXCHANGESCORE_RESULT, dwContextID, &ExchangeScoreResult, sizeof(ExchangeScoreResult));
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

		return false;
	}

	return true;
}

//提取游戏币
bool CDataBaseEngineSink::OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_UserTakeScore));
		if (wDataSize!=sizeof(DBR_GP_UserTakeScore)) return false;

		//请求处理
		DBR_GP_UserTakeScore * pUserTakeScore=(DBR_GP_UserTakeScore *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserTakeScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserTakeScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lTakeScore"),pUserTakeScore->lTakeScore);
		m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pUserTakeScore->szPassword);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
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

		//变量定义
		DBO_GP_UserTakeResult UserTakeResult;
		ZeroMemory(&UserTakeResult, sizeof(UserTakeResult));

		//构造变量
		UserTakeResult.bSuccess = lResultCode == DB_SUCCESS;
		if (lResultCode == DB_SUCCESS)
		{
			UserTakeResult.lScore = m_TreasureDBAide.GetValue_DOUBLE(TEXT("Score"));
			UserTakeResult.lInsureScore = m_TreasureDBAide.GetValue_DOUBLE(TEXT("Insure"));
		}
		lstrcpyn(UserTakeResult.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(UserTakeResult.szDescribeString));

		//发送结果
		WORD wDataSize = CountStringBuffer(UserTakeResult.szDescribeString);
		WORD wHeadSize = sizeof(UserTakeResult) - sizeof(UserTakeResult.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_TAKE_RESULT, dwContextID, &UserTakeResult, wHeadSize + wDataSize);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//变量定义
		DBO_GP_UserTakeResult UserTakeResult;
		ZeroMemory(&UserTakeResult, sizeof(UserTakeResult));

		//构造变量
		UserTakeResult.bSuccess = false;
		lstrcpyn(UserTakeResult.szDescribeString, TEXT("由于数据库操作异常，请您稍后重试！"), CountArray(UserTakeResult.szDescribeString));

		//发送结果
		WORD wDataSize = CountStringBuffer(UserTakeResult.szDescribeString);
		WORD wHeadSize = sizeof(UserTakeResult) - sizeof(UserTakeResult.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_TAKE_RESULT, dwContextID, &UserTakeResult, wHeadSize + wDataSize);

		return false;
	}

	return true;
}

//转帐游戏币
bool CDataBaseEngineSink::OnRequestUserTransferScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_UserTransferScore));
		if (wDataSize!=sizeof(DBR_GP_UserTransferScore)) return false;

		//请求处理
		DBR_GP_UserTransferScore * pUserTransferScore=(DBR_GP_UserTransferScore *)pData;

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
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
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
		OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//错误处理
		OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试！"),false);

		return false;
	}

	return true;
}

//请求配置
bool CDataBaseEngineSink::OnRequestLotteryConfigReq(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_LotteryConfigReq));
		if (wDataSize!=sizeof(DBR_GP_LotteryConfigReq)) return false;

		//请求处理
		DBR_GP_LotteryConfigReq * pLotteryConfigReq=(DBR_GP_LotteryConfigReq *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLotteryConfigReq->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@wKindID"),pLotteryConfigReq->wKindID);


		//抽奖配置
		LONG lResultCode=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadLotteryConfig"),true);
		ASSERT(lResultCode==DB_SUCCESS);
		if (lResultCode==DB_SUCCESS)
		{
			//构造结果
			DBO_GP_LotteryConfig LotteryConfig;
			ZeroMemory(&LotteryConfig,sizeof(LotteryConfig));
			
			//变量定义
			WORD wItemCount=0;
			while (m_PlatformDBModule->IsRecordsetEnd()==false)
			{
				//读取数据
				LotteryConfig.LotteryItem[wItemCount].cbItemIndex = m_PlatformDBAide.GetValue_BYTE(TEXT("ItemIndex"));
				LotteryConfig.LotteryItem[wItemCount].cbItemType = m_PlatformDBAide.GetValue_BYTE(TEXT("ItemType"));
				LotteryConfig.LotteryItem[wItemCount].lItemQuota = m_PlatformDBAide.GetValue_WORD(TEXT("ItemQuota"));
				wItemCount++;

				//移动记录
				m_PlatformDBModule->MoveToNext();
			}

			LotteryConfig.wLotteryCount = wItemCount;

			//投递结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOTTERY_CONFIG,dwContextID,&LotteryConfig,sizeof(LotteryConfig));
		}
		else
		{
			CTraceService::TraceString(TEXT("抽奖配置加载失败！"),TraceLevel_Exception);
		}

		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@wKindID"),pLotteryConfigReq->wKindID);
		m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pLotteryConfigReq->dwUserID);
		m_PlatformDBAide.AddParameter(TEXT("@strLogonPass"),pLotteryConfigReq->szLogonPass);
		m_PlatformDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//抽奖信息
		lResultCode=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LotteryUserInfo"),true);
		if (lResultCode==DB_SUCCESS)
		{
			//构造结果
			DBO_GP_LotteryUserInfo LotteryUserInfo;
			ZeroMemory(&LotteryUserInfo,sizeof(LotteryUserInfo));

			LotteryUserInfo.wKindID = pLotteryConfigReq->wKindID;
			LotteryUserInfo.dwUserID = pLotteryConfigReq->dwUserID;
			LotteryUserInfo.cbFreeCount = m_PlatformDBAide.GetValue_BYTE(TEXT("FreeCount"));
			LotteryUserInfo.cbAlreadyCount = m_PlatformDBAide.GetValue_BYTE(TEXT("AlreadyCount"));
			LotteryUserInfo.lChargeFee = m_PlatformDBAide.GetValue_DOUBLE(TEXT("ChargeFee"));

			//投递结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOTTERY_USER_INFO,dwContextID,&LotteryUserInfo,sizeof(LotteryUserInfo));
		}
		else
		{
			//变量定义
			DBO_GP_OperateFailure OperateFailure;
			ZeroMemory(&OperateFailure,sizeof(OperateFailure));

			//获取信息
			CDBVarValue DBVarValue;
			m_PlatformDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//构造变量
			lstrcpyn(OperateFailure.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateFailure.szDescribeString));

			//发送结果
			WORD wStringSize=CountStringBuffer(OperateFailure.szDescribeString);
			WORD wSendSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString)+wStringSize;
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_FAILURE,dwContextID,&OperateFailure,wSendSize);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//结果处理
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试！"),false);

		return false;
	}

	return true;
}

//抽奖开始
bool CDataBaseEngineSink::OnRequestLotteryStart(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_LotteryStart));
		if (wDataSize!=sizeof(DBR_GP_LotteryStart)) return false;

		//请求处理
		DBR_GP_LotteryStart * pLotteryStart=(DBR_GP_LotteryStart *)pData;

		//转化地址
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLotteryStart->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//构造参数
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@wKindID"),pLotteryStart->wKindID);
		m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pLotteryStart->dwUserID);
		m_PlatformDBAide.AddParameter(TEXT("@strLogonPass"),pLotteryStart->szLogonPass);
		m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),pLotteryStart->szMachineID);
		m_PlatformDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LotteryStart"),true);

		if (lResultCode==DB_SUCCESS)
		{
			//构造结果
			DBO_GP_LotteryResult LotteryResult;
			ZeroMemory(&LotteryResult,sizeof(LotteryResult));

			LotteryResult.wKindID = pLotteryStart->wKindID;
			LotteryResult.dwUserID = pLotteryStart->dwUserID;
			LotteryResult.bWined = (m_PlatformDBAide.GetValue_BYTE(TEXT("Wined"))==TRUE);
			LotteryResult.lUserScore = m_PlatformDBAide.GetValue_DOUBLE(TEXT("UserScore"));
			LotteryResult.dUserBeans = m_PlatformDBAide.GetValue_DOUBLE(TEXT("UserBeans"));
			LotteryResult.LotteryItem.cbItemIndex = m_PlatformDBAide.GetValue_BYTE(TEXT("ItemIndex"));
			LotteryResult.LotteryItem.cbItemType = m_PlatformDBAide.GetValue_BYTE(TEXT("ItemType"));
			LotteryResult.LotteryItem.lItemQuota = m_PlatformDBAide.GetValue_DOUBLE(TEXT("ItemQuota"));

			//投递结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOTTERY_RESULT,dwContextID,&LotteryResult,sizeof(LotteryResult));
		}
		else
		{
			//变量定义
			DBO_GP_OperateFailure OperateFailure;
			ZeroMemory(&OperateFailure,sizeof(OperateFailure));

			//获取信息
			CDBVarValue DBVarValue;
			m_PlatformDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//构造变量
			lstrcpyn(OperateFailure.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateFailure.szDescribeString));

			//发送结果
			WORD wStringSize=CountStringBuffer(OperateFailure.szDescribeString);
			WORD wSendSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString)+wStringSize;
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_FAILURE,dwContextID,&OperateFailure,wSendSize);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//结果处理
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试！"),false);

		return false;
	}

	return true;
}

//获取邮件
bool CDataBaseEngineSink::OnRequestQueryEmail(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize == sizeof(DBR_GP_QueryEmail));
		if (wDataSize != sizeof(DBR_GP_QueryEmail)) return false;

		//请求处理
		DBR_GP_QueryEmail * pQueryEmail = (DBR_GP_QueryEmail *)pData;

		//构造参数
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@dwUserID"), pQueryEmail->dwUserID);
		m_PlatformDBAide.AddParameter(TEXT("@strLogonPass"), pQueryEmail->nIndex);

		////输出参数
		//TCHAR szDescribeString[128] = TEXT("");
		//m_PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribeString, sizeof(szDescribeString), adParamOutput);

		////执行查询
		//LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LotteryStart"), true);

		//if (lResultCode == DB_SUCCESS)
		//{
		//	//构造结果
		//	DBO_GP_LotteryResult LotteryResult;
		//	ZeroMemory(&LotteryResult, sizeof(LotteryResult));

		//	LotteryResult.wKindID = pLotteryStart->wKindID;
		//	LotteryResult.dwUserID = pLotteryStart->dwUserID;
		//	LotteryResult.bWined = (m_PlatformDBAide.GetValue_BYTE(TEXT("Wined")) == TRUE);
		//	LotteryResult.lUserScore = m_PlatformDBAide.GetValue_DOUBLE(TEXT("UserScore"));
		//	LotteryResult.dUserBeans = m_PlatformDBAide.GetValue_DOUBLE(TEXT("UserBeans"));
		//	LotteryResult.LotteryItem.cbItemIndex = m_PlatformDBAide.GetValue_BYTE(TEXT("ItemIndex"));
		//	LotteryResult.LotteryItem.cbItemType = m_PlatformDBAide.GetValue_BYTE(TEXT("ItemType"));
		//	LotteryResult.LotteryItem.lItemQuota = m_PlatformDBAide.GetValue_DOUBLE(TEXT("ItemQuota"));

		//	//投递结果
		//	m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOTTERY_RESULT, dwContextID, &LotteryResult, sizeof(LotteryResult));
		//}
		//else
		//{
		//	//变量定义
		//	DBO_GP_OperateFailure OperateFailure;
		//	ZeroMemory(&OperateFailure, sizeof(OperateFailure));

		//	//获取信息
		//	CDBVarValue DBVarValue;
		//	m_PlatformDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);

		//	//构造变量
		//	lstrcpyn(OperateFailure.szDescribeString, CW2CT(DBVarValue.bstrVal), CountArray(OperateFailure.szDescribeString));

		//	//发送结果
		//	WORD wStringSize = CountStringBuffer(OperateFailure.szDescribeString);
		//	WORD wSendSize = sizeof(OperateFailure) - sizeof(OperateFailure.szDescribeString) + wStringSize;
		//	m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_FAILURE, dwContextID, &OperateFailure, wSendSize);
		//}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

		//结果处理
		OnOperateDisposeResult(dwContextID, DB_ERROR, TEXT("由于数据库操作异常，请您稍后重试！"), false);

		return false;
	}

	return true;
}

__int64 TimeDiff(SYSTEMTIME left, SYSTEMTIME right)    
{  
	CTime tmLeft(left.wYear, left.wMonth, left.wDay, 0, 0, 0);  
	CTime tmRight(right.wYear, right.wMonth, right.wDay, 0, 0, 0);  

	CTimeSpan sp = tmLeft - tmRight;  
	long MillisecondsL = (left.wHour*3600 + left.wMinute*60 + left.wSecond)*1000 + left.wMilliseconds;    
	long MillisecondsR = (right.wHour*3600 + right.wMinute*60 + right.wSecond)*1000 + right.wMilliseconds;    

	return  (__int64)sp.GetDays()*86400000 + (MillisecondsL - MillisecondsR);//此处返回毫秒  
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

//获取参数
bool CDataBaseEngineSink::OnRequestGetParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_GetParameter));
		if (wDataSize!=sizeof(DBR_GP_GetParameter)) return false;

		//请求处理
		DBR_GP_GetParameter * pGetParameter=(DBR_GP_GetParameter *)pData;

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@wServerID"),pGetParameter->wServerID);

		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_AndroidGetParameter"),true);

		//执行结果
		OnAndroidDisposeResult(dwContextID,lResultCode,SUB_GP_GET_PARAMETER,pGetParameter->wServerID);		
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//操作结果
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("数据库异常,请稍后再试！"),false,false);

		return false;
	}

	return true;
}

//添加参数
bool CDataBaseEngineSink::OnRequestAddParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_AddParameter));
		if (wDataSize!=sizeof(DBR_GP_AddParameter)) return false;

		//请求处理
		DBR_GP_AddParameter * pAddParameter=(DBR_GP_AddParameter *)pData;

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@wServerID"),pAddParameter->wServerID);
		m_AccountsDBAide.AddParameter(TEXT("@dwServiceMode"),pAddParameter->AndroidParameter.dwServiceMode);
		m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCount"),pAddParameter->AndroidParameter.dwAndroidCount);
		m_AccountsDBAide.AddParameter(TEXT("@dwEnterTime"),pAddParameter->AndroidParameter.dwEnterTime);
		m_AccountsDBAide.AddParameter(TEXT("@dwLeaveTime"),pAddParameter->AndroidParameter.dwLeaveTime);
		m_AccountsDBAide.AddParameter(TEXT("@dwEnterMinInterval"),pAddParameter->AndroidParameter.dwEnterMinInterval);
		m_AccountsDBAide.AddParameter(TEXT("@dwEnterMaxInterval"),pAddParameter->AndroidParameter.dwEnterMaxInterval);
		m_AccountsDBAide.AddParameter(TEXT("@dwLeaveMinInterval"),pAddParameter->AndroidParameter.dwLeaveMinInterval);
		m_AccountsDBAide.AddParameter(TEXT("@dwLeaveMaxInterval"),pAddParameter->AndroidParameter.dwLeaveMaxInterval);
		m_AccountsDBAide.AddParameter(TEXT("@lTakeMinScore"),pAddParameter->AndroidParameter.lTakeMinScore);
		m_AccountsDBAide.AddParameter(TEXT("@lTakeMaxScore"),pAddParameter->AndroidParameter.lTakeMaxScore);
		m_AccountsDBAide.AddParameter(TEXT("@dwSwitchMinInnings"),pAddParameter->AndroidParameter.dwSwitchMinInnings);
		m_AccountsDBAide.AddParameter(TEXT("@dwSwitchMaxInnings"),pAddParameter->AndroidParameter.dwSwitchMaxInnings);
		m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCountMember0"),pAddParameter->AndroidParameter.AndroidCountMember0);
		m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCountMember1"),pAddParameter->AndroidParameter.AndroidCountMember1);
		m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCountMember2"),pAddParameter->AndroidParameter.AndroidCountMember2);
		m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCountMember3"),pAddParameter->AndroidParameter.AndroidCountMember3);
		m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCountMember4"),pAddParameter->AndroidParameter.AndroidCountMember4);
		m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCountMember5"),pAddParameter->AndroidParameter.AndroidCountMember5);
		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_AndroidAddParameter"),true);

		//执行结果
		OnAndroidDisposeResult(dwContextID,lResultCode,SUB_GP_ADD_PARAMETER,pAddParameter->wServerID);
		
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//操作结果
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("数据库异常,请稍后再试！"),false,false);

		return false;
	}

	return true;
}

//修改参数
bool CDataBaseEngineSink::OnRequestModifyParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyParameter));
		if (wDataSize!=sizeof(DBR_GP_ModifyParameter)) return false;

		//请求处理
		DBR_GP_ModifyParameter * pModifyParameter=(DBR_GP_ModifyParameter *)pData;

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwDatchID"),pModifyParameter->AndroidParameter.dwBatchID);
		m_AccountsDBAide.AddParameter(TEXT("@dwServiceMode"),pModifyParameter->AndroidParameter.dwServiceMode);
		m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCount"),pModifyParameter->AndroidParameter.dwAndroidCount);
		m_AccountsDBAide.AddParameter(TEXT("@dwEnterTime"),pModifyParameter->AndroidParameter.dwEnterTime);
		m_AccountsDBAide.AddParameter(TEXT("@dwLeaveTime"),pModifyParameter->AndroidParameter.dwLeaveTime);
		m_AccountsDBAide.AddParameter(TEXT("@dwEnterMinInterval"),pModifyParameter->AndroidParameter.dwEnterMinInterval);
		m_AccountsDBAide.AddParameter(TEXT("@dwEnterMaxInterval"),pModifyParameter->AndroidParameter.dwEnterMaxInterval);
		m_AccountsDBAide.AddParameter(TEXT("@dwLeaveMinInterval"),pModifyParameter->AndroidParameter.dwLeaveMinInterval);
		m_AccountsDBAide.AddParameter(TEXT("@dwLeaveMaxInterval"),pModifyParameter->AndroidParameter.dwLeaveMaxInterval);
		m_AccountsDBAide.AddParameter(TEXT("@lTakeMinScore"),pModifyParameter->AndroidParameter.lTakeMinScore);
		m_AccountsDBAide.AddParameter(TEXT("@lTakeMaxScore"),pModifyParameter->AndroidParameter.lTakeMaxScore);
		m_AccountsDBAide.AddParameter(TEXT("@dwSwitchMinInnings"),pModifyParameter->AndroidParameter.dwSwitchMinInnings);
		m_AccountsDBAide.AddParameter(TEXT("@dwSwitchMaxInnings"),pModifyParameter->AndroidParameter.dwSwitchMaxInnings);
		m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCountMember0"),pModifyParameter->AndroidParameter.AndroidCountMember0);
		m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCountMember1"),pModifyParameter->AndroidParameter.AndroidCountMember1);
		m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCountMember2"),pModifyParameter->AndroidParameter.AndroidCountMember2);
		m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCountMember3"),pModifyParameter->AndroidParameter.AndroidCountMember3);
		m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCountMember4"),pModifyParameter->AndroidParameter.AndroidCountMember4);
		m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCountMember5"),pModifyParameter->AndroidParameter.AndroidCountMember5);
		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_AndroidModifyParameter"),true);

		//执行结果
		OnAndroidDisposeResult(dwContextID,lResultCode,SUB_GP_MODIFY_PARAMETER,pModifyParameter->wServerID);
		
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//操作结果
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("数据库异常,请稍后再试！"),false,false);

		return false;
	}

	return true;
}

//删除参数
bool CDataBaseEngineSink::OnRequestDeleteParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_DeleteParameter));
		if (wDataSize!=sizeof(DBR_GP_DeleteParameter)) return false;

		//请求处理
		DBR_GP_DeleteParameter * pDeleteParameter=(DBR_GP_DeleteParameter *)pData;

		//构造参数
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwBatchID"),pDeleteParameter->dwBatchID);

		//执行查询
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_AndroidDeleteParameter"),true);

		//执行结果
		OnAndroidDisposeResult(dwContextID,lResultCode,SUB_GP_DELETE_PARAMETER,pDeleteParameter->wServerID);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//操作结果
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("数据库异常,请稍后再试！"),false,false);

		return false;
	}

	return true;
}

//查询银行
bool CDataBaseEngineSink::OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_QueryInsureInfo));
		if (wDataSize!=sizeof(DBR_GP_QueryInsureInfo)) return false;

		//请求处理
		DBR_GP_QueryInsureInfo * pQueryInsureInfo=(DBR_GP_QueryInsureInfo *)pData;

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
			DBO_GP_UserInsureInfo UserInsureInfo;
			ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

			//银行信息
			UserInsureInfo.cbEnjoinTransfer=m_TreasureDBAide.GetValue_BYTE(TEXT("EnjoinTransfer"));
			UserInsureInfo.wRevenueTake=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTake"));
			UserInsureInfo.wRevenueTransfer=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTransfer"));
			UserInsureInfo.wRevenueTransferMember=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTransferMember"));
			UserInsureInfo.wRevenueTransfer=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTransfer"));
			UserInsureInfo.wServerID=m_TreasureDBAide.GetValue_WORD(TEXT("ServerID"));
			UserInsureInfo.lUserScore=m_TreasureDBAide.GetValue_DOUBLE(TEXT("Score"));
			UserInsureInfo.lUserInsure=m_TreasureDBAide.GetValue_DOUBLE(TEXT("Insure"));
			ShowLog(TEXT("%.2f---用户金币"),UserInsureInfo.lUserScore);
			ShowLog(TEXT("%.2f---银行金币"),UserInsureInfo.lUserInsure);
			UserInsureInfo.lTransferPrerequisite=m_TreasureDBAide.GetValue_DOUBLE(TEXT("TransferPrerequisite"));

			//发送结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_INFO,dwContextID,&UserInsureInfo,sizeof(UserInsureInfo));
		}
		else
		{
			//获取参数
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//错误处理
			OnInsureDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//结果处理
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试！"),false);

		return false;
	}

	return true;
}

//查询用户
bool CDataBaseEngineSink::OnRequestQueryTransferUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(DBR_GP_QueryInsureUserInfo));
	if (wDataSize!=sizeof(DBR_GP_QueryInsureUserInfo)) return false;

	//请求处理
	DBR_GP_QueryInsureUserInfo * pQueryTransferUserInfo=(DBR_GP_QueryInsureUserInfo *)pData;

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
			DBO_GP_UserTransferUserInfo TransferUserInfo;
			ZeroMemory(&TransferUserInfo,sizeof(TransferUserInfo));

			//银行信息
			TransferUserInfo.dwGameID=m_TreasureDBAide.GetValue_DWORD(TEXT("GameID"));
			m_TreasureDBAide.GetValue_String(TEXT("Accounts"), TransferUserInfo.szAccounts, CountArray(TransferUserInfo.szAccounts));

			//发送结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_USER_INFO,dwContextID,&TransferUserInfo,sizeof(TransferUserInfo));
		}
		else
		{
			//获取参数
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//错误处理
			OnInsureDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//结果处理
		OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("由于数据库操作异常，请您稍后重试！"),false);

		return false;
	}
}

//低保参数
bool CDataBaseEngineSink::OnRequestLoadBaseEnsure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//构造参数
		m_PlatformDBAide.ResetParameter();

		//执行命令
		LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadBaseEnsure"),true);

		//执行成功
		if(lResultCode==DB_SUCCESS)
		{
			//构造结构
			DBO_GP_BaseEnsureParameter BaseEnsureParameter;
			ZeroMemory(&BaseEnsureParameter,sizeof(BaseEnsureParameter));

			//变量定义
			if(m_PlatformDBModule->IsRecordsetEnd()==false)
			{
				BaseEnsureParameter.cbTakeTimes= m_PlatformDBAide.GetValue_BYTE(TEXT("TakeTimes"));
				BaseEnsureParameter.lScoreAmount = m_PlatformDBAide.GetValue_WORD(TEXT("ScoreAmount"));
				BaseEnsureParameter.lScoreCondition = m_PlatformDBAide.GetValue_WORD(TEXT("ScoreCondition"));				

				//移动记录
				m_PlatformDBModule->MoveToNext();
			}

			//投递结果
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_BASEENSURE_PARAMETER,dwContextID,&BaseEnsureParameter,sizeof(BaseEnsureParameter));
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
bool CDataBaseEngineSink::OnRequestTakeBaseEnsure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//效验参数
		ASSERT(wDataSize==sizeof(DBR_GP_TakeBaseEnsure));
		if (wDataSize!=sizeof(DBR_GP_TakeBaseEnsure)) return false;

		//提取数据
		DBR_GP_TakeBaseEnsure * pTakeBaseEnsure=(DBR_GP_TakeBaseEnsure *)pData;

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
		DBO_GP_BaseEnsureResult BaseEnsureResult;
		ZeroMemory(&BaseEnsureResult,sizeof(BaseEnsureResult));

		//读取分数
		if(lResultCode==DB_SUCCESS && m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			BaseEnsureResult.lGameScore = m_PlatformDBAide.GetValue_DOUBLE(TEXT("Score"));
			BaseEnsureResult.nTakeTimes = m_PlatformDBAide.GetValue_LONGLONG(TEXT("TakeTimes"));
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
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_BASEENSURE_RESULT,dwContextID,&BaseEnsureResult,wSendSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//构造结构
		DBO_GP_BaseEnsureResult BaseEnsureResult;
		ZeroMemory(&BaseEnsureResult,sizeof(BaseEnsureResult));

		//设置变量
		BaseEnsureResult.bSuccessed=false;
		lstrcpyn(BaseEnsureResult.szNotifyContent,TEXT("由于数据库操作异常，请您稍后重试！"),CountArray(BaseEnsureResult.szNotifyContent));

		//发送结果
		WORD wSendSize=sizeof(BaseEnsureResult)-sizeof(BaseEnsureResult.szNotifyContent);
		wSendSize+=CountStringBuffer(BaseEnsureResult.szNotifyContent);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_BASEENSURE_RESULT,dwContextID,&BaseEnsureResult,wSendSize);

		return false;
	}

	return true;
}

//房间虚拟在线人数
bool CDataBaseEngineSink::OnRequestServerDummyOnLine(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//加载类型
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GR_LoadAddOfOnline"),true);

		//批量数据发送czz
		//计算单次最大发送数量
		WORD wTotalCount = SOCKET_TCP_PACKET / sizeof(tagServerDummyOnLine);

		BYTE cbBuffer[SOCKET_TCP_PACKET];

		ZeroMemory(cbBuffer, SOCKET_TCP_PACKET);

		tagServerDummyOnLine * pResultData = (tagServerDummyOnLine *)cbBuffer;

		WORD wCount = 0;
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SERVER_DUMMY_ONLINE_BEGIN,dwContextID, 0, 0);
		while(m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//读取信息
			pResultData->wServerID=m_PlatformDBAide.GetValue_WORD(TEXT("ServerID"));
			
			pResultData->dwDummyMaxOnLine = m_PlatformDBAide.GetValue_DWORD(TEXT("ChangeRate"));
			pResultData->dwDummyMinOnLine=m_PlatformDBAide.GetValue_DWORD(TEXT("AddCounts"));

			printf("wServerID:%d minDummy:%d, maxDummy:%d\n", pResultData->wServerID, pResultData->dwDummyMinOnLine, pResultData->dwDummyMaxOnLine);

			TCHAR szDataBuffer[128] = {0};
			m_PlatformDBAide.GetValue_String(TEXT("StartTime"), szDataBuffer, CountArray(szDataBuffer));

			CString str(szDataBuffer);

			int nHours = _ttoi(str.Left(2));
			int nMinute = _ttoi(str.Mid(3,4));

			//不知为何 用sscanf 会跪掉
			//_stscanf(szDataBuffer,_T("%[^:]"),nHours,nMinute);

			//printf("[2][][][][][]\n");

			SYSTEMTIME tempDate;
			GetSystemTime(&tempDate);

			//printf("年：%d 月：%d 周：%d 日：%d 小时：%d 分钟：%d 秒：%d\n", tempDate.wYear, tempDate.wMonth, tempDate.wDayOfWeek, tempDate.wDay, tempDate.wHour, tempDate.wMinute, tempDate.wSecond);

			//赋值的是本地时间
			tempDate.wYear = 1970;
			tempDate.wMonth = 1;
			tempDate.wDayOfWeek = 1;
			tempDate.wDay = 1;
			tempDate.wMinute = nMinute;
			tempDate.wHour = nHours;

			//减去8小时转化为标准时间
			pResultData->lTimeStart = SystemTimeToTimet(tempDate);

			//time_t nCurTime = 0;
			//nCurTime = time(NULL);

			//SYSTEMTIME tempData1;
			//GetSystemTime(&tempData1);
			//printf("SystemTimeToTimeT:%ld\n", SystemTimeToTimet(tempData1));

			//printf("000nMinuteStart：%d, nHoursStart：%d, CurTime：%ld, lTimeStart：%ld\n", nMinute, nHours, nCurTime, pServerDummyOnLine->lTimeStart);


			memset(szDataBuffer, 0, sizeof(szDataBuffer));
			m_PlatformDBAide.GetValue_String(TEXT("EndTime"), szDataBuffer, CountArray(szDataBuffer));

			str = szDataBuffer;
			nHours = _ttoi(str.Left(2));
			nMinute = _ttoi(str.Mid(3,4));

			tempDate.wMinute = nMinute;
			tempDate.wHour = nHours;

			pResultData->lTimeEnd = SystemTimeToTimet(tempDate);
			//printf("001nMinuteEnd：%d, nHoursEnd：%d, lTimeEnd：%ld\n", nMinute, nHours, pServerDummyOnLine->lTimeEnd);


			m_PlatformDBModule->MoveToNext();

			//指针后移
			++pResultData;

			if (++wCount>wTotalCount)
			{
				//发送满包数据
				if (m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SERVER_DUMMY_ONLINE,dwContextID, cbBuffer, sizeof(tagServerDummyOnLine) * wCount)== false)
				{
					CTraceService::TraceString(_T("DBO_GP_SERVER_DUMMY_ONLINE 消息 未能发送成功！"), TraceLevel_Warning);
					return false;
				}

				//重置状态
				wCount = 0;
				ZeroMemory(cbBuffer, SOCKET_TCP_PACKET * sizeof(BYTE));
				pResultData = (tagServerDummyOnLine *)cbBuffer;
			}
		}

		//发送非满包的剩余数据

		if (wCount != 0)
		{
			if (m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SERVER_DUMMY_ONLINE,dwContextID, cbBuffer, sizeof(tagServerDummyOnLine) * wCount)==false)
			{
				CTraceService::TraceString(_T("DBO_GP_SERVER_DUMMY_ONLINE 消息 未能发送成功！"), TraceLevel_Warning);
				return false;
			}
		}

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SERVER_DUMMY_ONLINE_END,dwContextID, 0, 0);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		return false;
	}
}

//加载列表
bool CDataBaseEngineSink::OnRequestLoadGameList(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//变量定义
		WORD wPacketSize=0;
		BYTE cbBuffer[MAX_ASYNCHRONISM_DATA];

		//加载类型
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameTypeItem"),true);

		//发送种类
		wPacketSize=0;
		DBO_GP_GameType * pGameType=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//发送信息
			if ((wPacketSize+sizeof(DBO_GP_GameType))>(sizeof(cbBuffer)-sizeof(NTY_DataBaseEvent)))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_TYPE_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//读取信息
			pGameType=(DBO_GP_GameType *)(cbBuffer+wPacketSize);
			pGameType->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGameType->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
			pGameType->wTypeID=m_PlatformDBAide.GetValue_WORD(TEXT("TypeID"));
			m_PlatformDBAide.GetValue_String(TEXT("TypeName"),pGameType->szTypeName,CountArray(pGameType->szTypeName));

			//设置位移
			wPacketSize+=sizeof(DBO_GP_GameType);

			//移动记录
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_TYPE_ITEM,dwContextID,cbBuffer,wPacketSize);

		//读取类型
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameKindItem"),true);

		//发送类型
		wPacketSize=0;
		DBO_GP_GameKind * pGameKind=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//发送信息
			if ((wPacketSize+sizeof(DBO_GP_GameKind))>(sizeof(cbBuffer)-sizeof(NTY_DataBaseEvent)))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_KIND_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//读取信息
			pGameKind=(DBO_GP_GameKind *)(cbBuffer+wPacketSize);
			pGameKind->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGameKind->wTypeID=m_PlatformDBAide.GetValue_WORD(TEXT("TypeID"));
			pGameKind->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
			pGameKind->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
			pGameKind->wGameID=m_PlatformDBAide.GetValue_WORD(TEXT("GameID"));
			m_PlatformDBAide.GetValue_String(TEXT("KindName"),pGameKind->szKindName,CountArray(pGameKind->szKindName));
			m_PlatformDBAide.GetValue_String(TEXT("ProcessName"),pGameKind->szProcessName,CountArray(pGameKind->szProcessName));
			pGameKind->wRecommend=m_PlatformDBAide.GetValue_WORD(TEXT("Recommend"));
			pGameKind->wGameFlag =m_PlatformDBAide.GetValue_WORD(TEXT("GameFlag"));
			pGameKind->dwSuportType=m_PlatformDBAide.GetValue_WORD(TEXT("SuportType"));

			//设置位移
			wPacketSize+=sizeof(DBO_GP_GameKind);

			//移动记录
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_KIND_ITEM,dwContextID,cbBuffer,wPacketSize);

		//读取节点
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameNodeItem"),true);

		//发送节点
		wPacketSize=0;
		DBO_GP_GameNode * pGameNode=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//发送信息
			if ((wPacketSize+sizeof(DBO_GP_GameNode))>(sizeof(cbBuffer)-sizeof(NTY_DataBaseEvent)))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_NODE_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//读取信息
			pGameNode=(DBO_GP_GameNode *)(cbBuffer+wPacketSize);
			pGameNode->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGameNode->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
			pGameNode->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
			pGameNode->wNodeID=m_PlatformDBAide.GetValue_WORD(TEXT("NodeID"));
			m_PlatformDBAide.GetValue_String(TEXT("NodeName"),pGameNode->szNodeName,CountArray(pGameNode->szNodeName));
			
			//设置位移
			wPacketSize+=sizeof(DBO_GP_GameNode);

			//移动记录
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_NODE_ITEM,dwContextID,cbBuffer,wPacketSize);

		//读取定制
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGamePageItem"),true);

		//发送定制
		wPacketSize=0;
		DBO_GP_GamePage * pGamePage=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//发送信息
			if ((wPacketSize+sizeof(DBO_GP_GamePage))>(sizeof(cbBuffer)-sizeof(NTY_DataBaseEvent)))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_PAGE_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//读取信息
			pGamePage=(DBO_GP_GamePage *)(cbBuffer+wPacketSize);
			pGamePage->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
			pGamePage->wNodeID=m_PlatformDBAide.GetValue_WORD(TEXT("NodeID"));
			pGamePage->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGamePage->wPageID=m_PlatformDBAide.GetValue_WORD(TEXT("PageID"));
			pGamePage->wOperateType=m_PlatformDBAide.GetValue_WORD(TEXT("OperateType"));
			m_PlatformDBAide.GetValue_String(TEXT("DisplayName"),pGamePage->szDisplayName,CountArray(pGamePage->szDisplayName));
			
			//设置位移
			wPacketSize+=sizeof(DBO_GP_GamePage);

			//移动记录
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_PAGE_ITEM,dwContextID,cbBuffer,wPacketSize);

		//变量定义
		DBO_GP_GameListResult GameListResult;
		ZeroMemory(&GameListResult,sizeof(GameListResult));

		//设置变量
		GameListResult.cbSuccess=TRUE;

		//发送消息
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_LIST_RESULT,dwContextID,&GameListResult,sizeof(GameListResult));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		//变量定义
		DBO_GP_GameListResult GameListResult;
		ZeroMemory(&GameListResult,sizeof(GameListResult));

		//设置变量
		GameListResult.cbSuccess=FALSE;

		//发送消息
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_LIST_RESULT,dwContextID,&GameListResult,sizeof(GameListResult));

		return false;
	}

	return true;
}

//在线信息
bool CDataBaseEngineSink::OnRequestOnLineCountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//变量定义
		DBR_GP_OnLineCountInfo * pOnLineCountInfo=(DBR_GP_OnLineCountInfo *)pData;
		WORD wHeadSize=(sizeof(DBR_GP_OnLineCountInfo)-sizeof(pOnLineCountInfo->OnLineCountKind));

		//效验数据
		ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pOnLineCountInfo->wKindCount*sizeof(tagOnLineInfoKindEx))));
		if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pOnLineCountInfo->wKindCount*sizeof(tagOnLineInfoKindEx)))) return false;

		//机器标识
		TCHAR szMachineID[LEN_MACHINE_ID];
		CWHService::GetMachineID(szMachineID);

		//构造信息
		TCHAR szOnLineCountKind[2048]=TEXT("");
		for (WORD i=0;i<pOnLineCountInfo->wKindCount;i++)
		{
			INT nLength=lstrlen(szOnLineCountKind);
			_sntprintf(&szOnLineCountKind[nLength],CountArray(szOnLineCountKind)-nLength,TEXT("%d:%ld;"),pOnLineCountInfo->OnLineCountKind[i].wKindID,pOnLineCountInfo->OnLineCountKind[i].dwOnLineCount);
		}

		//机器信息
		TCHAR szAndroidCountKind[2048]=TEXT("");
		for (WORD j=0;j<pOnLineCountInfo->wKindCount;j++)
		{
			INT nLength=lstrlen(szAndroidCountKind);
			_sntprintf(&szAndroidCountKind[nLength],CountArray(szAndroidCountKind)-nLength,TEXT("%d:%ld;"),pOnLineCountInfo->OnLineCountKind[j].wKindID,pOnLineCountInfo->OnLineCountKind[j].dwAndroidCount);
		}

		//构造参数
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),szMachineID);
		m_PlatformDBAide.AddParameter(TEXT("@strMachineServer"),m_pInitParameter->m_szServerName);
		m_PlatformDBAide.AddParameter(TEXT("@dwOnLineCountSum"),pOnLineCountInfo->dwOnLineCountSum);
		m_PlatformDBAide.AddParameter(TEXT("@dwAndroidCountSum"),pOnLineCountInfo->dwAndroidCountSum);
		m_PlatformDBAide.AddParameter(TEXT("@strOnLineCountKind"),szOnLineCountKind);
		m_PlatformDBAide.AddParameter(TEXT("@strAndroidCountKind"),szAndroidCountKind);

		//执行命令
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_OnLineCountInfo"),false);

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

//平台配置
bool CDataBaseEngineSink::OnRequestPlatformParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//构造参数
		m_PlatformDBAide.ResetParameter();

		//执行命令
		LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GR_LoadPlatformParameter"),true);

		//构造结构
		DBO_GP_PlatformParameter PlatformParameter;
		ZeroMemory(&PlatformParameter,sizeof(PlatformParameter));

		//执行成功
		if(lResultCode==DB_SUCCESS)
		{
			//记录判断
			if (m_PlatformDBModule->IsRecordsetEnd()==false)
			{
				//读取数据
				PlatformParameter.dwExchangeRate = m_PlatformDBAide.GetValue_DWORD(TEXT("ExchangeRate"));
				PlatformParameter.dwPresentExchangeRate = m_PlatformDBAide.GetValue_DWORD(TEXT("PresentExchangeRate"));
				PlatformParameter.dwRateGold = m_PlatformDBAide.GetValue_DWORD(TEXT("RateGold"));
			}						
		}

		//发送数据
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_PLATFORM_PARAMETER,dwContextID,&PlatformParameter,sizeof(PlatformParameter));

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

//登录成功
VOID CDataBaseEngineSink::OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString,DWORD dwCheckUserRight, bool bMobileClient)
{
	if (dwErrorCode==DB_SUCCESS)
	{
		//变量定义
		DBO_MB_LogonSuccess LogonSuccess;
		ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

		//属性资料
		LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
		LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
		LogonSuccess.dwCustomID=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomID"));
		LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
		LogonSuccess.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
		LogonSuccess.dwSpreaderID = m_AccountsDBAide.GetValue_DWORD(TEXT("SpreaderID"));
		LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));

		m_AccountsDBAide.GetValue_String(TEXT("Accounts"),LogonSuccess.szAccounts,CountArray(LogonSuccess.szAccounts));
		m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));
		m_AccountsDBAide.GetValue_String(TEXT("AliPayAcccount"), LogonSuccess.szAliPayAcccount, CountArray(LogonSuccess.szAliPayAcccount));
		m_AccountsDBAide.GetValue_String(TEXT("BinkID"), LogonSuccess.szBinkID, CountArray(LogonSuccess.szBinkID));
		m_AccountsDBAide.GetValue_String(TEXT("DynamicPass"),LogonSuccess.szDynamicPass,CountArray(LogonSuccess.szDynamicPass));

		//用户成绩
 		LogonSuccess.lUserScore=m_AccountsDBAide.GetValue_DOUBLE(TEXT("Score"));
		LogonSuccess.lUserInsure=m_AccountsDBAide.GetValue_DOUBLE(TEXT("Insure"));

		//扩展信息
		LogonSuccess.cbInsureEnabled=m_AccountsDBAide.GetValue_BYTE(TEXT("InsureEnabled"));
		LogonSuccess.cbIsAgent=m_AccountsDBAide.GetValue_BYTE(TEXT("IsAgent"));
		LogonSuccess.cbMoorMachine=m_AccountsDBAide.GetValue_BYTE(TEXT("MoorMachine"));
			
		//房卡信息，房卡锁表
		LogonSuccess.TodayAlmsCount = m_AccountsDBAide.GetValue_INT(TEXT("TodayAlmsCount"));
		LogonSuccess.dwLockServerID = m_AccountsDBAide.GetValue_DWORD(TEXT("LockServerID"));
		LogonSuccess.dwKindID = m_AccountsDBAide.GetValue_DWORD(TEXT("KindID"));

		
		//////////////////////////////////////////////////////////////////////////

		//发送结果
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_SUCCESS, dwContextID, &LogonSuccess, sizeof(LogonSuccess));
	}
	else
	{
		//变量定义
		DBO_MB_LogonFailure LogonFailure;
		ZeroMemory(&LogonFailure,sizeof(LogonFailure));

		//构造数据
		LogonFailure.lResultCode=dwErrorCode;
		lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

		//发送结果
		WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
		WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);
	}

	return;
}

//银行结果
VOID CDataBaseEngineSink::OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient)
{
	if (dwErrorCode==DB_SUCCESS)
	{
		//变量定义
		DBO_GP_UserInsureSuccess UserInsureSuccess;
		ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

		//构造变量
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
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_SUCCESS,dwContextID,&UserInsureSuccess,wHeadSize+wDataSize);
	}
	else
	{
		//变量定义
		DBO_GP_UserInsureFailure UserInsureFailure;
		ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

		//构造变量
		UserInsureFailure.lResultCode=dwErrorCode;
		lstrcpyn(UserInsureFailure.szDescribeString,pszErrorString,CountArray(UserInsureFailure.szDescribeString));

		//发送结果
		WORD wDataSize=CountStringBuffer(UserInsureFailure.szDescribeString);
		WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_FAILURE,dwContextID,&UserInsureFailure,wHeadSize+wDataSize);
	}

	return;
}

//操作结果
VOID CDataBaseEngineSink::OnOperateDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient,bool bCloseSocket)
{
	if (dwErrorCode==DB_SUCCESS)
	{
		//变量定义
		DBO_GP_OperateSuccess OperateSuccess;
		ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

		//构造变量
		lstrcpyn(OperateSuccess.szDescribeString,pszErrorString,CountArray(OperateSuccess.szDescribeString));

		//发送结果
		WORD wDataSize=CountStringBuffer(OperateSuccess.szDescribeString);
		WORD wHeadSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_SUCCESS,dwContextID,&OperateSuccess,wHeadSize+wDataSize);
	}
	else
	{
		//变量定义
		DBO_GP_OperateFailure OperateFailure;
		ZeroMemory(&OperateFailure,sizeof(OperateFailure));

		//构造变量
		lstrcpyn(OperateFailure.szDescribeString,pszErrorString,CountArray(OperateFailure.szDescribeString));

		//发送结果
		WORD wDataSize=CountStringBuffer(OperateFailure.szDescribeString);
		WORD wHeadSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_FAILURE,dwContextID,&OperateFailure,wHeadSize+wDataSize);
	}

	return;
}

//机器结果
VOID CDataBaseEngineSink::OnAndroidDisposeResult(DWORD dwContextID,DWORD dwErrorCode,WORD wSubCommdID,WORD wServerID)
{
	//构造结构
	DBO_GP_AndroidParameter AndroidParameter;
	ZeroMemory(&AndroidParameter,sizeof(AndroidParameter));

	//设置变量
	AndroidParameter.wSubCommdID=wSubCommdID;
	AndroidParameter.wServerID=wServerID;

	if(dwErrorCode==DB_SUCCESS)
	{
		//变量定义
		WORD wParameterCount=0;

		while(m_AccountsDBModule->IsRecordsetEnd()==false)		
		{
			wParameterCount = AndroidParameter.wParameterCount;
			AndroidParameter.AndroidParameter[wParameterCount].dwBatchID = m_AccountsDBAide.GetValue_DWORD(TEXT("BatchID"));
			AndroidParameter.AndroidParameter[wParameterCount].dwAndroidCount = m_AccountsDBAide.GetValue_DWORD(TEXT("AndroidCount"));
			AndroidParameter.AndroidParameter[wParameterCount].dwServiceMode = m_AccountsDBAide.GetValue_DWORD(TEXT("ServiceMode"));
			AndroidParameter.AndroidParameter[wParameterCount].dwEnterTime = m_AccountsDBAide.GetValue_DWORD(TEXT("EnterTime"));
			AndroidParameter.AndroidParameter[wParameterCount].dwLeaveTime = m_AccountsDBAide.GetValue_DWORD(TEXT("LeaveTime"));
			AndroidParameter.AndroidParameter[wParameterCount].dwEnterMinInterval = m_AccountsDBAide.GetValue_DWORD(TEXT("EnterMinInterval"));
			AndroidParameter.AndroidParameter[wParameterCount].dwEnterMaxInterval = m_AccountsDBAide.GetValue_DWORD(TEXT("EnterMaxInterval"));
			AndroidParameter.AndroidParameter[wParameterCount].dwLeaveMinInterval = m_AccountsDBAide.GetValue_DWORD(TEXT("LeaveMinInterval"));
			AndroidParameter.AndroidParameter[wParameterCount].dwLeaveMaxInterval = m_AccountsDBAide.GetValue_DWORD(TEXT("LeaveMaxInterval"));
			AndroidParameter.AndroidParameter[wParameterCount].dwSwitchMinInnings = m_AccountsDBAide.GetValue_DWORD(TEXT("SwitchMinInnings"));
			AndroidParameter.AndroidParameter[wParameterCount].dwSwitchMaxInnings = m_AccountsDBAide.GetValue_DWORD(TEXT("SwitchMaxInnings"));
			AndroidParameter.AndroidParameter[wParameterCount].lTakeMinScore = m_AccountsDBAide.GetValue_DOUBLE(TEXT("TakeMinScore"));
			AndroidParameter.AndroidParameter[wParameterCount].lTakeMaxScore = m_AccountsDBAide.GetValue_DOUBLE(TEXT("TakeMaxScore"));
			AndroidParameter.AndroidParameter[wParameterCount].AndroidCountMember0 = m_AccountsDBAide.GetValue_DWORD(TEXT("AndroidCountMember0"));
			AndroidParameter.AndroidParameter[wParameterCount].AndroidCountMember1 = m_AccountsDBAide.GetValue_DWORD(TEXT("AndroidCountMember1"));
			AndroidParameter.AndroidParameter[wParameterCount].AndroidCountMember2 = m_AccountsDBAide.GetValue_DWORD(TEXT("AndroidCountMember2"));
			AndroidParameter.AndroidParameter[wParameterCount].AndroidCountMember3 = m_AccountsDBAide.GetValue_DWORD(TEXT("AndroidCountMember3"));
			AndroidParameter.AndroidParameter[wParameterCount].AndroidCountMember4 = m_AccountsDBAide.GetValue_DWORD(TEXT("AndroidCountMember4"));
			AndroidParameter.AndroidParameter[wParameterCount].AndroidCountMember5 = m_AccountsDBAide.GetValue_DWORD(TEXT("AndroidCountMember5"));


			//设置变量
			AndroidParameter.wParameterCount++;

			//溢出判断
			if(AndroidParameter.wParameterCount>=CountArray(AndroidParameter.AndroidParameter))
			{
				//发送数据
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_ANDROID_PARAMETER,dwContextID,&AndroidParameter,sizeof(AndroidParameter));

				//设置变量
				ZeroMemory(&AndroidParameter,sizeof(AndroidParameter));
			}

			//移动游标
			m_AccountsDBModule->MoveToNext();
		}	
	}
	
	//计算大小
	WORD wSendDataSize = sizeof(AndroidParameter)-sizeof(AndroidParameter.AndroidParameter);
	wSendDataSize += AndroidParameter.wParameterCount*sizeof(tagAndroidParameter);

	//发送数据
	m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_ANDROID_PARAMETER,dwContextID,&AndroidParameter,wSendDataSize);
}

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
