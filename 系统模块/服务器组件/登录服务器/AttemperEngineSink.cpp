#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include "AttemperEngineSink.h"
#include "MiniDumper.h"


/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//时间标识
#define IDI_LOAD_GAME_LIST			1									//加载列表
#define IDI_CONNECT_CORRESPOND		2									//重连标识
#define IDI_COLLECT_ONLINE_INFO		3									//统计在线

//时间定义
#define	TIME_VALID_VERIFY_CODE		1 * 60								//注册请求验证码时间间隔（单位 秒）

//统计在线人数时间间隔
#define	TIME_COLLECT_ONLINE_INFO	20									//请求间隔

#define IDI_HTTP_EVENT_LOOP		4									//HTTP事件分发
#define TIME_HTTP_EVENT_LOOP	1									//时间间隔



#define VERIFY_CODE_TYPE_REGISTER	1								//注册短信
#define VERIFY_CODE_TYPE_MODIFYPASS	2								//修改登录密码短信
#define VERIFY_CODE_TYPE_MODIFYBANKINFO 3							//修改银行信息短信

MiniDumper *g_pGFrame = NULL;

time_t SystemTimeToTimet(SYSTEMTIME st)
{

	FILETIME ft;

	SystemTimeToFileTime( &st, &ft );
	LONGLONG nLL;

	ULARGE_INTEGER ui;

	ui.LowPart = ft.dwLowDateTime;

	ui.HighPart = ft.dwHighDateTime;

	nLL = (ft.dwHighDateTime << 32) + ft.dwLowDateTime;

	time_t pt = (long)((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);

	pt -= 8 * 60 * 60;

	return pt;

}

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//构造函数
CAttemperEngineSink::CAttemperEngineSink()
{
	g_pGFrame = new MiniDumper(true);
	//状态变量
	m_bNeekCorrespond=true;
	m_bShowServerStatus=false;

	//状态变量
	m_pInitParameter=NULL;
	m_pBindParameter=NULL;

	//组件变量
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketService=NULL;
	m_pHttpClientService = NULL;

	//视频配置
	m_wAVServerPort=0;
	m_dwAVServerAddr=0;

	//缓存变量

	ZeroMemory(&m_PlatformParameter,sizeof(&m_PlatformParameter));
	ZeroMemory(&m_BaseEnsureParameter,sizeof(&m_BaseEnsureParameter));	
	ZeroMemory(m_szHttpRegisterValidCode, sizeof(m_szHttpRegisterValidCode));
	ZeroMemory(m_szHttpModifyPassValidCode, sizeof(m_szHttpModifyPassValidCode));
//清理数组
	m_VerifyCodeArray.RemoveAll();

	return;
}

//析构函数
CAttemperEngineSink::~CAttemperEngineSink()
{
	//清理资源
	for (INT i=0; i<m_VerifyCodeArray.GetCount();i++)
	{
		tagVerifyCode *pValidateCode=m_VerifyCodeArray[i];
		if (pValidateCode!=NULL)
		{
			SafeDelete(pValidateCode);
		}
		m_VerifyCodeArray.RemoveAt(i);
	}
}

//接口查询
VOID * CAttemperEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAttemperEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAttemperEngineSink,Guid,dwQueryVer);
	return NULL;
}

//启动事件
bool CAttemperEngineSink::OnAttemperEngineStart(IUnknownEx * pIUnknownEx)
{
	//绑定参数
	m_pBindParameter=new tagBindParameter[m_pInitParameter->m_wMaxConnect];
	ZeroMemory(m_pBindParameter,sizeof(tagBindParameter)*m_pInitParameter->m_wMaxConnect);

	//设置时间
	ASSERT(m_pITimerEngine!=NULL);
#ifndef _DEBUG
	m_pITimerEngine->SetTimer(IDI_COLLECT_ONLINE_INFO,TIME_COLLECT_ONLINE_INFO*1000L,TIMES_INFINITY,0);
#else
	m_pITimerEngine->SetTimer(IDI_COLLECT_ONLINE_INFO,TIME_COLLECT_ONLINE_INFO*1000L,TIMES_INFINITY,0);
#endif

	m_pHttpClientService = new CHttpKernel<CAttemperEngineSink>();

	if (m_pHttpClientService->InitClient() == false)
	{
		CTraceService::TraceString(_T("HTTP客户端启动失败"),TraceLevel_Warning);
	}
	else
	{
		m_pITimerEngine->SetTimer(IDI_HTTP_EVENT_LOOP,TIME_HTTP_EVENT_LOOP,TIMES_INFINITY,0);
	}

	//获取目录
	TCHAR szPath[MAX_PATH]=TEXT("");

	//获取目录
	TCHAR szServerAddr[LEN_SERVER]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);

	//读取配置
	TCHAR szVideoFileName[MAX_PATH]=TEXT("");
	_sntprintf(szVideoFileName,CountArray(szVideoFileName),TEXT("%s\\VideoOption.ini"),szPath);
	m_wAVServerPort=GetPrivateProfileInt(TEXT("VideoOption"),TEXT("ServerPort"), 0,szVideoFileName);
	DWORD dwAddrLen=GetPrivateProfileString(TEXT("VideoOption"),TEXT("ServerAddr"), TEXT(""), szServerAddr,LEN_SERVER,szVideoFileName);
	if(dwAddrLen>0)
	{
		CT2CA strServerDomain(szServerAddr);
		m_dwAVServerAddr=inet_addr(strServerDomain);
	}
	else
	{
		m_dwAVServerAddr=0;
	}

	//AllocConsole();
	//freopen("CONIN$", "r+t", stdin); // 重定向 STDIN
	//freopen("CONOUT$", "w+t", stdout); // 重定向STDOUT 

	return true;
}

//停止事件
bool CAttemperEngineSink::OnAttemperEngineConclude(IUnknownEx * pIUnknownEx)
{
	//状态变量
	m_bNeekCorrespond=true;

	//组件变量
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketService=NULL;

	if (m_pHttpClientService)
		SafeDelete(m_pHttpClientService);

	//删除数据
	SafeDeleteArray(m_pBindParameter);

	//列表组件
	m_ServerListManager.ResetServerList();

	//释放掉旧的数据
	for (int i = 0; i < m_ServerDummyOnLineArray.GetCount(); ++i)
	{
		tagServerDummyOnLine * pCursor = m_ServerDummyOnLineArray.GetAt(i);
		free(pCursor);
	}
	m_ServerDummyOnLineArray.RemoveAll();
	m_ServerDummyOnLineMap.RemoveAll();

	//FreeConsole();

	//删除用户
	m_ServerUserManager.DeleteUserItem();

	return true;
}

////POST请求回调
//void CAttemperEngineSink::OnClientPostCallBack(struct evhttp_request *req, void * pArg)
//{
//	if (req == NULL)
//	{
//		printf("some error occur!");
//		return;
//	}
//
//	struct evbuffer * pBuffer = evhttp_request_get_input_buffer(req);
//	size_t dataLength = evbuffer_get_length(pBuffer);
//	char* out = new char[dataLength];
//	do 
//	{
//		if (evbuffer_remove(pBuffer, out, dataLength) > 0)
//		{
//			//处理来自网站的数据
//		}
//		else
//		{
//			evhttp_send_reply(req, HTTP_BADREQUEST, "OK", NULL);
//			break;
//		}
//	} while (0);
//
//	delete[]out;
//}
void CAttemperEngineSink::OnClientGetVerifyCodeCallBack(struct evhttp_request *req, void * pArg)
{
	tagVerifyCode * pNewVerifyCode=(tagVerifyCode*)pArg;
	if (req == NULL)
	{
		delete pNewVerifyCode;
		printf("some error occur!");
		return;
	}

	bool bSuccessed = false;

	switch(req->response_code)
	{
	case HTTP_OK:
		{
			struct evbuffer * pBuffer = evhttp_request_get_input_buffer(req);
			size_t dataLength = evbuffer_get_length(pBuffer);
			char* out = new char[dataLength];
			do 
			{
				if (evbuffer_remove(pBuffer, out, dataLength) > 0)
				{
					//处理来自网站的post的数据
					Json::Reader reader;
					Json::Value root;

					if (reader.parse(out, root))  // json解析
					{ 
						if (!root.isObject())
						{
							evhttp_send_reply(req, HTTP_BADREQUEST, "OK", NULL);
							break;
						}

						int code =  root["code"].asInt();
						const Json::Value content = root["data"];
						Json::Value::Members mem = content.getMemberNames();

						if (code == 0)
						{
							//遍历json 处理数据
							for (Json::Value::Members::iterator iter = mem.begin(); iter != mem.end(); iter++)        
							{
								const char * pkey = (*iter).c_str();
								if(strcmp(pkey,"code")==0)
								{
									//更新时间
									pNewVerifyCode->dwUpdateTime=(DWORD)time(NULL);
									//更新验证码
									std::string validcode = content[pkey].asString();
									TCHAR szValidCode[MAX_PATH] = TEXT("");
									MultiByteToWideChar(CP_ACP, 0, validcode.c_str(), -1, szValidCode, CountArray(szValidCode));
									lstrcpyn(pNewVerifyCode->szValidateCode,szValidCode,CountArray(pNewVerifyCode->szValidateCode));
									//添加到验证码数组
									m_VerifyCodeArray.Add(pNewVerifyCode);

									//通知客户端验证码生成成功
									CMD_GP_VerifyCodeResult VerifyCodeResult;
									ZeroMemory(&VerifyCodeResult,sizeof(VerifyCodeResult));

									VerifyCodeResult.cbResultCode=0;
									lstrcpy(VerifyCodeResult.szDescString,TEXT("发送成功！"));

									//通知客户端消息 验证码超时
									m_pITCPNetworkEngine->SendData(pNewVerifyCode->dwSocketID,MDM_GP_LOGON,SUB_GP_VERIFY_CODE_RESULT,&VerifyCodeResult,sizeof(VerifyCodeResult));

									bSuccessed = true;
									break;
								}
							}
						}
						else
						{
							CMD_GP_VerifyCodeResult VerifyCodeResult;
							ZeroMemory(&VerifyCodeResult,sizeof(VerifyCodeResult));

							VerifyCodeResult.cbResultCode=2;
							lstrcpy(VerifyCodeResult.szDescString,TEXT("抱歉，验证码获取异常，请稍后重试C301！"));

							//通知客户端消息 验证码超时
							m_pITCPNetworkEngine->SendData(pNewVerifyCode->dwSocketID,MDM_GP_LOGON,SUB_GP_VERIFY_CODE_RESULT,&VerifyCodeResult,sizeof(VerifyCodeResult));
						}
					}
					else
					{
						CMD_GP_VerifyCodeResult VerifyCodeResult;
						ZeroMemory(&VerifyCodeResult,sizeof(VerifyCodeResult));

						VerifyCodeResult.cbResultCode=2;
						lstrcpy(VerifyCodeResult.szDescString,TEXT("抱歉，验证码获取异常，请稍后重试C302！"));

						//通知客户端消息 验证码超时
						m_pITCPNetworkEngine->SendData(pNewVerifyCode->dwSocketID,MDM_GP_LOGON,SUB_GP_VERIFY_CODE_RESULT,&VerifyCodeResult,sizeof(VerifyCodeResult));
					}
				}
				else
				{//获取验证码内部错误, 通知客户端
					CMD_GP_VerifyCodeResult VerifyCodeResult;
					ZeroMemory(&VerifyCodeResult,sizeof(VerifyCodeResult));

					VerifyCodeResult.cbResultCode=2;
					lstrcpy(VerifyCodeResult.szDescString,TEXT("抱歉，获取验证码内部错误，请稍后重试C303！"));

					//通知客户端消息 验证码超时
					m_pITCPNetworkEngine->SendData(pNewVerifyCode->dwSocketID,MDM_GP_LOGON,SUB_GP_VERIFY_CODE_RESULT,&VerifyCodeResult,sizeof(VerifyCodeResult));
					break;
				}
			} while (0);

			delete[]out;
		}
		break;
	case HTTP_MOVEPERM:
		break;
	case HTTP_MOVETEMP:
		{
			//HTTP发生重定向
			const char *new_location = evhttp_find_header(req->input_headers, "Location");   
			int i = 0;
			//struct evhttp_uri *new_uri = evhttp_uri_parse(new_location);    
			//evhttp_uri_free(http_req_post->uri);    
			//http_req_post->uri = new_uri;    
			//start_url_request((struct http_request_get *)http_req_post, REQUEST_POST_FLAG);    
		}
		break;
	}

	if (!bSuccessed)
	{
		CMD_GP_VerifyCodeResult VerifyCodeResult;
		ZeroMemory(&VerifyCodeResult,sizeof(VerifyCodeResult));

		VerifyCodeResult.cbResultCode=2;
		lstrcpy(VerifyCodeResult.szDescString,TEXT("抱歉，获取验证码内部错误，请稍后重试C304！"));

		//通知客户端消息 验证码超时
		m_pITCPNetworkEngine->SendData(pNewVerifyCode->dwSocketID,MDM_GP_LOGON,SUB_GP_VERIFY_CODE_RESULT,&VerifyCodeResult,sizeof(VerifyCodeResult));

		delete pNewVerifyCode;
	}
}


//GET请求回调
void CAttemperEngineSink::OnClientGetReBuildVerifyCodeCallBack(struct evhttp_request *req, void * pArg)
{
	if (req == NULL)
	{
		printf("some error occur!");
		return;
	}

	tagVerifyCode * pVerifyCode = (tagVerifyCode *)pArg;

	switch(req->response_code)
	{
	case HTTP_OK:
		{

			struct evbuffer * pBuffer = evhttp_request_get_input_buffer(req);
			size_t dataLength = evbuffer_get_length(pBuffer);
			char* out = new char[dataLength];
			do 
			{
				if (evbuffer_remove(pBuffer, out, dataLength) > 0)
				{
					//处理来自网站的post的数据
					Json::Reader reader;
					Json::Value root;

					if (reader.parse(out, root))  // json解析
					{ 
						if (!root.isObject())
						{
							evhttp_send_reply(req, HTTP_BADREQUEST, "OK", NULL);
							break;
						}

						int code =  root["code"].asInt();

						/*TCHAR szJson[MAX_PATH] = TEXT("");
						MultiByteToWideChar(CP_ACP, 0, out, -1, szJson, CountArray(szJson));
						ShowLog(TEXT("-------------------------------%s"),szJson);*/

						const Json::Value content = root["data"];
						Json::Value::Members mem = content.getMemberNames();

						if (code == 0)
						{
							//遍历json 处理数据
							for (Json::Value::Members::iterator iter = mem.begin(); iter != mem.end(); iter++)        
							{
								const char * pkey = (*iter).c_str();
								if(strcmp(pkey,"code")==0)
								{
									std::string validcode = content[pkey].asString();
									TCHAR szValidCode[MAX_PATH] = TEXT("");
									MultiByteToWideChar(CP_ACP, 0, validcode.c_str(), -1, szValidCode, CountArray(szValidCode));
									//处理来自网站的数据

									//更新验证码时间
									pVerifyCode->dwUpdateTime=(DWORD)time(NULL);
									//更新验证码
									lstrcpyn(pVerifyCode->szValidateCode,szValidCode,CountArray(pVerifyCode->szValidateCode));

									//通知客户端验证码生成成功
									CMD_GP_VerifyCodeResult VerifyCodeResult;
									ZeroMemory(&VerifyCodeResult,sizeof(VerifyCodeResult));

									VerifyCodeResult.cbResultCode=0;
									lstrcpy(VerifyCodeResult.szDescString,TEXT("发送成功！"));

									//通知客户端消息 验证码超时
									m_pITCPNetworkEngine->SendData(pVerifyCode->dwSocketID,MDM_GP_LOGON,SUB_GP_VERIFY_CODE_RESULT,&VerifyCodeResult,sizeof(VerifyCodeResult));
								}
								//else
								//{//验证码获取异常 通知客户端
								//	CMD_GP_VerifyCodeResult VerifyCodeResult;
								//	ZeroMemory(&VerifyCodeResult,sizeof(VerifyCodeResult));

								//	VerifyCodeResult.cbResultCode=2;
								//	lstrcpy(VerifyCodeResult.szDescString,TEXT("抱歉，验证码获取异常，请稍后重试！"));

								//	//通知客户端消息 验证码超时
								//	m_pITCPNetworkEngine->SendData(pVerifyCode->dwSocketID,MDM_GP_LOGON,SUB_GP_VERIFY_CODE_RESULT,&VerifyCodeResult,sizeof(VerifyCodeResult));
								//}
							}

						}
						else
						{
							CMD_GP_VerifyCodeResult VerifyCodeResult;
							ZeroMemory(&VerifyCodeResult,sizeof(VerifyCodeResult));

							VerifyCodeResult.cbResultCode=2;
							lstrcpy(VerifyCodeResult.szDescString,TEXT("抱歉，验证码获取异常，请稍后重试！"));

							//通知客户端消息 验证码超时
							m_pITCPNetworkEngine->SendData(pVerifyCode->dwSocketID,MDM_GP_LOGON,SUB_GP_VERIFY_CODE_RESULT,&VerifyCodeResult,sizeof(VerifyCodeResult));
						}
					}
					else
					{//获取验证码内部错误, 通知客户端
						CMD_GP_VerifyCodeResult VerifyCodeResult;
						ZeroMemory(&VerifyCodeResult,sizeof(VerifyCodeResult));

						VerifyCodeResult.cbResultCode=2;
						lstrcpy(VerifyCodeResult.szDescString,TEXT("抱歉，获取验证码内部错误，请稍后重试！"));

						//通知客户端消息 验证码超时
						m_pITCPNetworkEngine->SendData(pVerifyCode->dwSocketID,MDM_GP_LOGON,SUB_GP_VERIFY_CODE_RESULT,&VerifyCodeResult,sizeof(VerifyCodeResult));
						break;
					}
				}
				else
				{//获取验证码内部错误, 通知客户端
					CMD_GP_VerifyCodeResult VerifyCodeResult;
					ZeroMemory(&VerifyCodeResult,sizeof(VerifyCodeResult));

					VerifyCodeResult.cbResultCode=2;
					lstrcpy(VerifyCodeResult.szDescString,TEXT("抱歉，获取验证码内部错误，请稍后重试！"));

					//通知客户端消息 验证码超时
					m_pITCPNetworkEngine->SendData(pVerifyCode->dwSocketID,MDM_GP_LOGON,SUB_GP_VERIFY_CODE_RESULT,&VerifyCodeResult,sizeof(VerifyCodeResult));
					break;
				}
			} while (0);

			delete[]out;
		}
		break;
	case HTTP_MOVEPERM:
		break;
	case HTTP_MOVETEMP:
		{
			//HTTP发生重定向
			const char *new_location = evhttp_find_header(req->input_headers, "Location");   
			int i = 0;
			//struct evhttp_uri *new_uri = evhttp_uri_parse(new_location);    
			//evhttp_uri_free(http_req_post->uri);    
			//http_req_post->uri = new_uri;    
			//start_url_request((struct http_request_get *)http_req_post, REQUEST_POST_FLAG);    
		}
		break;
	}
}

//控制事件
bool CAttemperEngineSink::OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize)
{
	switch (wIdentifier)
	{
	case CT_LOAD_DB_GAME_LIST:		//加载列表
		{
			//加载列表
			m_ServerListManager.DisuseKernelItem();
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_GAME_LIST,0,NULL,0);

			//平台参数
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_PLATFORM_PARAMETER,0,NULL,0);			

			//加载低保
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_BASEENSURE,0,NULL,0);

			//查询虚拟在线人数
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_SERVER_DUMMY_ONLINE,0,NULL,0);

			return true;
		}
	case CT_CONNECT_CORRESPOND:		//连接协调
		{
			//发起连接
			tagAddressInfo * pCorrespondAddress=&m_pInitParameter->m_CorrespondAddress;
			m_pITCPSocketService->Connect(pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//构造提示
			TCHAR szString[512]=TEXT("");
			_sntprintf(szString,CountArray(szString),TEXT("正在连接协调服务器 [ %s:%d ]"),pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//提示消息
			CTraceService::TraceString(szString,TraceLevel_Normal);

			return true;
		}
	}

	return false;
}

//调度事件
bool CAttemperEngineSink::OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//时间事件
bool CAttemperEngineSink::OnEventTimer(DWORD dwTimerID, WPARAM wBindParam)
{
	switch (dwTimerID)
	{
	case IDI_LOAD_GAME_LIST:		//加载列表
		{
			//加载列表
			m_ServerListManager.DisuseKernelItem();
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_GAME_LIST,0,NULL,0);

			return true;
		}
	case IDI_CONNECT_CORRESPOND:	//连接协调
		{
			//发起连接
			tagAddressInfo * pCorrespondAddress=&m_pInitParameter->m_CorrespondAddress;
			m_pITCPSocketService->Connect(pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//构造提示
			TCHAR szString[512]=TEXT("");
			_sntprintf(szString,CountArray(szString),TEXT("正在连接协调服务器 [ %s:%d ]"),pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//提示消息
			CTraceService::TraceString(szString,TraceLevel_Normal);

			return true;
		}
	case IDI_COLLECT_ONLINE_INFO:	//统计在线
		{
			//变量定义
			DBR_GP_OnLineCountInfo OnLineCountInfo;
			ZeroMemory(&OnLineCountInfo,sizeof(OnLineCountInfo));

			//随机虚拟人数
			OnChangeDummyOnline();

			//获取总数
			OnLineCountInfo.dwOnLineCountSum=m_ServerListManager.CollectOnlineInfo(false);
			OnLineCountInfo.dwAndroidCountSum=m_ServerListManager.CollectOnlineInfo(true);

			//获取类型
			POSITION KindPosition=NULL;
			do
			{
				//获取类型
				CGameKindItem * pGameKindItem=m_ServerListManager.EmunGameKindItem(KindPosition);

				//设置变量
				if (pGameKindItem!=NULL)
				{
					WORD wKindIndex=OnLineCountInfo.wKindCount++;
					OnLineCountInfo.OnLineCountKind[wKindIndex].wKindID=pGameKindItem->m_GameKind.wKindID;

					//目录人数
					OnLineCountInfo.OnLineCountKind[wKindIndex].dwOnLineCount=pGameKindItem->m_GameKind.dwOnLineCount;
					OnLineCountInfo.OnLineCountKind[wKindIndex].dwAndroidCount=pGameKindItem->m_GameKind.dwAndroidCount;
				}

				//溢出判断
				if (OnLineCountInfo.wKindCount>=CountArray(OnLineCountInfo.OnLineCountKind))
				{
					ASSERT(FALSE);
					break;
				}

			} while (KindPosition!=NULL);

			//发送请求
			WORD wHeadSize=sizeof(OnLineCountInfo)-sizeof(OnLineCountInfo.OnLineCountKind);
			WORD wSendSize=wHeadSize+OnLineCountInfo.wKindCount*sizeof(OnLineCountInfo.OnLineCountKind[0]);
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_ONLINE_COUNT_INFO,0,&OnLineCountInfo,wSendSize);
			return true;
		}
	case IDI_HTTP_EVENT_LOOP:
		{
			if (m_pHttpClientService)
				m_pHttpClientService->EventDispatch();

			
			return true;
		}
	}

	return false;
}

//应答事件
bool CAttemperEngineSink::OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID)
{
	//获取索引
	ASSERT(LOWORD(dwSocketID)<m_pInitParameter->m_wMaxConnect);
	if (LOWORD(dwSocketID)>=m_pInitParameter->m_wMaxConnect) return false;

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//设置变量
	pBindParameter->dwSocketID=dwSocketID;
	pBindParameter->dwClientAddr=dwClientAddr;
	pBindParameter->dwActiveTime=(DWORD)time(NULL);

	return true;
}

//关闭事件
bool CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID)
{
	//清除信息
	WORD wBindIndex=LOWORD(dwSocketID);
	ZeroMemory((m_pBindParameter+wBindIndex),sizeof(tagBindParameter));

	return false;
}

//读取事件
bool CAttemperEngineSink::OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (Command.wMainCmdID)
	{
	case MDM_GP_SERVER_LIST:	//列表命令
		{
			return OnTCPNetworkMainPCServerList(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_USER_SERVICE:	//服务命令
		{
			return OnTCPNetworkMainPCUserService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_REMOTE_SERVICE:	//远程服务
		{
			return OnTCPNetworkMainPCRemoteService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_ANDROID_SERVICE: //机器服务
		{
			return OnTCPNetworkMainAndroidService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_MB_LOGON:			//登录命令
		{
			return OnTCPNetworkMainMBLogon(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_MB_SERVER_LIST:	//列表命令
		{
			return OnTCPNetworkMainMBServerList(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//数据库事件
bool CAttemperEngineSink::OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBO_GP_PLATFORM_PARAMETER:		//平台参数
		{
			return OnDBPCPlatformParameter(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_FACE_INFO:			//用户头像
		{
			return OnDBPCUserFaceInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_INFO:		//银行资料
		{
			return OnDBPCUserInsureInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_SAVE_RESULT:		//存钱结果
		{
			return OnDBPCUserSaveResult(dwContextID, pData, wDataSize);
		}
	case DBO_GP_USER_TAKE_RESULT:		//取钱结果
		{
			return OnDBPCUserTakeResult(dwContextID, pData, wDataSize);
		}
	case DBO_GP_USER_INSURE_SUCCESS:	//银行成功
		{
			return OnDBPCUserInsureSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_FAILURE:	//银行失败
		{
			return OnDBPCUserInsureFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_USER_INFO:  //用户信息
		{
			return OnDBPCUserInsureUserInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_ENABLE_RESULT: //开通结果
		{
			return OnDBPCUserInsureEnableResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_ANDROID_PARAMETER:		//机器参数
		{
			return OnDBAndroidParameter(dwContextID,pData,wDataSize);
		}
	case DBO_GP_INDIVIDUAL_RESULT:
		{
			return OnDBIndividualResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_EXCHANGESCORE_RESULT:
		{
		return OnDBMBExchangeScoreResult(dwContextID, pData, wDataSize);
		}
	case DBO_GP_OPERATE_SUCCESS:		//操作成功
		{
			return OnDBPCOperateSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GP_OPERATE_FAILURE:		//操作失败
		{
			return OnDBPCOperateFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_TYPE_ITEM:			//类型子项
		{
			return OnDBPCGameTypeItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_KIND_ITEM:			//类型子项
		{
			return OnDBPCGameKindItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_NODE_ITEM:			//节点子项
		{
			return OnDBPCGameNodeItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_PAGE_ITEM:			//定制子项
		{
			return OnDBPCGamePageItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_LIST_RESULT:		//加载结果
		{
			return OnDBPCGameListResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_SERVER_DUMMY_ONLINE_BEGIN:
		{
			return OnDBServerDummyOnLineBegin(dwContextID, pData, wDataSize);
		}
	case DBO_GP_SERVER_DUMMY_ONLINE:		//房间虚拟人数
		{
			return OnDBServerDummyOnLine(dwContextID, pData, wDataSize);
		}
	case DBO_GP_SERVER_DUMMY_ONLINE_END:
		{
			return OnDBServerDummyOnLineEnd(dwContextID, pData, wDataSize);
		}
	case DBO_GP_BASEENSURE_PARAMETER:	//低保参数
		{
			return OnDBPCBaseEnsureParameter(dwContextID,pData,wDataSize);
		}
	case DBO_GP_BASEENSURE_RESULT:		//低保结果
		{
			return OnDBPCBaseEnsureResult(dwContextID,pData,wDataSize);
		}
	case DBO_MB_LOGON_SUCCESS:			//登录成功
		{
			return OnDBMBLogonSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_MB_LOGON_FAILURE:			//登录失败
		{
			return OnDBMBLogonFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_LOTTERY_CONFIG:			//抽奖配置
		{
			return OnDBPCLotteryConfig(dwContextID,pData,wDataSize);
		}
	case DBO_GP_LOTTERY_USER_INFO:		//抽奖信息
		{
			return OnDBPCLotteryUserInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_LOTTERY_RESULT:			//抽奖结果
		{
			return OnDBPCLotteryResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_AGENT_GAME_KIND_ITEM:
		{
			return OnDBPCAgentGameList(dwContextID,pData,wDataSize);
		}
	}


	return false;
}

//关闭事件
bool CAttemperEngineSink::OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)
{
	//协调连接
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//重连判断
		if (m_bNeekCorrespond==true)
		{
			//构造提示
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("与协调服务器的连接关闭了，%ld 秒后将重新连接"),m_pInitParameter->m_wConnectTime);

			//提示消息
			CTraceService::TraceString(szDescribe,TraceLevel_Warning);

			//设置时间
			ASSERT(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);

			return true;
		}
	}

	return false;
}

//连接事件
bool CAttemperEngineSink::OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode)
{
	//协调连接
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//错误判断
		if (nErrorCode!=0)
		{
			//构造提示
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("协调服务器连接失败 [ %ld ]，%ld 秒后将重新连接"),
				nErrorCode,m_pInitParameter->m_wConnectTime);

			//提示消息
			CTraceService::TraceString(szDescribe,TraceLevel_Warning);

			//设置时间
			ASSERT(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);

			return false;
		}

		//提示消息
		CTraceService::TraceString(TEXT("正在注册游戏登录服务器..."),TraceLevel_Normal);

		//变量定义
		CMD_CS_C_RegisterPlaza RegisterPlaza;
		ZeroMemory(&RegisterPlaza,sizeof(RegisterPlaza));

		//设置变量
		lstrcpyn(RegisterPlaza.szServerName,m_pInitParameter->m_szServerName,CountArray(RegisterPlaza.szServerName));
		lstrcpyn(RegisterPlaza.szServerAddr,m_pInitParameter->m_ServiceAddress.szAddress,CountArray(RegisterPlaza.szServerAddr));

		//发送数据
		m_pITCPSocketService->SendData(MDM_CS_REGISTER,SUB_CS_C_REGISTER_PLAZA,&RegisterPlaza,sizeof(RegisterPlaza));

		return true;
	}

	return true;
}

//读取事件
bool CAttemperEngineSink::OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//协调连接
	if (wServiceID==NETWORK_CORRESPOND)
	{
		switch (Command.wMainCmdID)
		{
		case MDM_CS_REGISTER:		//注册服务
			{
				return OnTCPSocketMainRegister(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_SERVICE_INFO:	//服务信息
			{
				return OnTCPSocketMainServiceInfo(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_REMOTE_SERVICE:	//远程服务
			{
				return OnTCPSocketMainRemoteService(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_MANAGER_SERVICE: //管理服务
			{
				return OnTCPSocketMainManagerService(Command.wSubCmdID,pData,wDataSize);
			}
		}
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

//注册事件
bool CAttemperEngineSink::OnTCPSocketMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_REGISTER_FAILURE:		//注册失败
		{
			//变量定义
			CMD_CS_S_RegisterFailure * pRegisterFailure=(CMD_CS_S_RegisterFailure *)pData;

			//效验参数
			ASSERT(wDataSize>=(sizeof(CMD_CS_S_RegisterFailure)-sizeof(pRegisterFailure->szDescribeString)));
			if (wDataSize<(sizeof(CMD_CS_S_RegisterFailure)-sizeof(pRegisterFailure->szDescribeString))) return false;

			//关闭处理
			m_bNeekCorrespond=false;
			m_pITCPSocketService->CloseSocket();

			//显示消息
			LPCTSTR pszDescribeString=pRegisterFailure->szDescribeString;
			if (lstrlen(pszDescribeString)>0) CTraceService::TraceString(pszDescribeString,TraceLevel_Exception);

			//事件通知
			CP_ControlResult ControlResult;
			ControlResult.cbSuccess=ER_FAILURE;
			SendUIControlPacket(UI_CORRESPOND_RESULT,&ControlResult,sizeof(ControlResult));

			return true;
		}
	}

	return true;
}

//列表事件
bool CAttemperEngineSink::OnTCPSocketMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_SERVER_INFO:		//房间信息
		{
			//废弃列表
			m_ServerListManager.DisuseServerItem();

			return true;
		}
	case SUB_CS_S_SERVER_ONLINE:	//房间人数
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerOnLine));
			if (wDataSize!=sizeof(CMD_CS_S_ServerOnLine)) return false;

			//变量定义
			CMD_CS_S_ServerOnLine * pServerOnLine=(CMD_CS_S_ServerOnLine *)pData;

			//查找房间
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerOnLine->wServerID);
			if(pGameServerItem == NULL) return true;

			//获取对象
			tagGameServer * pGameServer=&pGameServerItem->m_GameServer; 

			//设置人数
			DWORD dwOldOnlineCount=0,dwOldAndroidCount=0,dwOldDummyCount=0;
			dwOldOnlineCount = pGameServer->dwOnLineCount;
			dwOldAndroidCount = pGameServer->dwAndroidCount;
			dwOldDummyCount = pGameServer->dwDummyCount;

			//查找房间虚拟在线人数
			tagServerDummyOnLine * pDummyOnLine= NULL;
			m_ServerDummyOnLineMap.Lookup(pServerOnLine->wServerID,pDummyOnLine);

			//房间人数
			pGameServer->dwAndroidCount=pServerOnLine->dwAndroidCount;
			pGameServer->dwOnLineCount=pServerOnLine->dwOnLineCount;

			if (pDummyOnLine != NULL)
				pGameServer->dwDummyCount = pDummyOnLine->dwCurDummyOnline;
			else
				pGameServer->dwDummyCount = 0;

			//目录人数
			CGameKindItem * pGameKindItem=m_ServerListManager.SearchGameKind(pGameServer->wKindID);
			if (pGameKindItem!=NULL)
			{
				//在线人数
				pGameKindItem->m_GameKind.dwOnLineCount -= dwOldOnlineCount;
				pGameKindItem->m_GameKind.dwOnLineCount += pGameServer->dwOnLineCount;

				//机器人数
				pGameKindItem->m_GameKind.dwAndroidCount -= dwOldAndroidCount;
				pGameKindItem->m_GameKind.dwAndroidCount += pGameServer->dwAndroidCount;

				//虚拟人数
				pGameKindItem->m_GameKind.dwDummyCount -= dwOldDummyCount;
				pGameKindItem->m_GameKind.dwDummyCount += pGameServer->dwDummyCount;
			}

			return true;
		}
	case SUB_CS_S_SERVER_INSERT:	//房间插入
		{
			//效验参数
			ASSERT(wDataSize%sizeof(tagGameServer)==0);
			if (wDataSize%sizeof(tagGameServer)!=0) return false;

			//变量定义
			WORD wItemCount=wDataSize/sizeof(tagGameServer);
			tagGameServer * pGameServer=(tagGameServer *)pData;

			//更新数据
			for (WORD i=0;i<wItemCount;i++)
			{
				m_ServerListManager.InsertGameServer(pGameServer++);
			}

			return true;
		}
	case SUB_CS_S_SERVER_MODIFY:	//房间修改
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerModify));
			if (wDataSize!=sizeof(CMD_CS_S_ServerModify)) return false;

			//变量定义
			CMD_CS_S_ServerModify * pServerModify=(CMD_CS_S_ServerModify *)pData;

			//查找房间
			ASSERT(m_ServerListManager.SearchGameServer(pServerModify->wServerID));
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerModify->wServerID);

			//设置房间
			if (pGameServerItem!=NULL)
			{
				//设置人数
				DWORD dwOldOnlineCount=0,dwOldAndroidCount=0,dwOldFullCount=0;
				dwOldAndroidCount = pGameServerItem->m_GameServer.dwAndroidCount;
				dwOldOnlineCount = pGameServerItem->m_GameServer.dwOnLineCount;				
				dwOldFullCount   = pGameServerItem->m_GameServer.dwFullCount;

				//修改房间信息
				pGameServerItem->m_GameServer.wKindID=pServerModify->wKindID;
				pGameServerItem->m_GameServer.wNodeID=pServerModify->wNodeID;
				pGameServerItem->m_GameServer.wSortID=pServerModify->wSortID;
				pGameServerItem->m_GameServer.wServerPort=pServerModify->wServerPort;
				pGameServerItem->m_GameServer.dwOnLineCount=pServerModify->dwOnLineCount;
				pGameServerItem->m_GameServer.dwFullCount=pServerModify->dwFullCount;
				pGameServerItem->m_GameServer.dwAndroidCount=pServerModify->dwAndroidCount;
				lstrcpyn(pGameServerItem->m_GameServer.szServerName,pServerModify->szServerName,CountArray(pGameServerItem->m_GameServer.szServerName));
				lstrcpyn(pGameServerItem->m_GameServer.szServerAddr,pServerModify->szServerAddr,CountArray(pGameServerItem->m_GameServer.szServerAddr));

				//目录人数
				CGameKindItem * pGameKindItem=m_ServerListManager.SearchGameKind(pGameServerItem->m_GameServer.wKindID);
				if (pGameKindItem!=NULL)
				{
					tagGameServer * pGameServer=&pGameServerItem->m_GameServer;
					pGameKindItem->m_GameKind.dwOnLineCount -= dwOldOnlineCount;
					pGameKindItem->m_GameKind.dwOnLineCount += pGameServer->dwOnLineCount;

					pGameKindItem->m_GameKind.dwFullCount -= dwOldFullCount;
					pGameKindItem->m_GameKind.dwFullCount += pGameServer->dwFullCount;

					pGameKindItem->m_GameKind.dwAndroidCount -= dwOldAndroidCount;
					pGameKindItem->m_GameKind.dwAndroidCount += pServerModify->dwAndroidCount;
				}
			}

			return true;
		}
	case SUB_CS_S_SERVER_REMOVE:	//房间删除
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerRemove));
			if (wDataSize!=sizeof(CMD_CS_S_ServerRemove)) return false;

			//变量定义
			CMD_CS_S_ServerRemove * pServerRemove=(CMD_CS_S_ServerRemove *)pData;

			//变量定义
			m_ServerListManager.DeleteGameServer(pServerRemove->wServerID);

			return true;
		}
	case SUB_CS_S_SERVER_FINISH:	//房间完成
		{
			//清理列表
			m_ServerListManager.CleanServerItem();

			//事件处理
			CP_ControlResult ControlResult;
			ControlResult.cbSuccess=ER_SUCCESS;
			SendUIControlPacket(UI_CORRESPOND_RESULT,&ControlResult,sizeof(ControlResult));

			return true;
		}
	case SUB_CS_S_MATCH_INSERT:		//比赛插入
		{
			//效验参数
			ASSERT(wDataSize%sizeof(tagGameMatch)==0);
			if (wDataSize%sizeof(tagGameMatch)!=0) return false;

			//变量定义
			WORD wItemCount=wDataSize/sizeof(tagGameMatch);
			tagGameMatch * pGameMatch=(tagGameMatch *)pData;

			//更新数据
			for (WORD i=0;i<wItemCount;i++)
			{
				CGameServerItem * pGameServerItem = m_ServerListManager.SearchGameServer(pGameMatch->wServerID);
				if(pGameServerItem!=NULL)
				{
					CopyMemory(&pGameServerItem->m_GameMatch,pGameMatch++,sizeof(pGameServerItem->m_GameMatch));
				}
			}

			return true;
		}
	case SUB_CS_S_PLAZA_CHANGE_DUMMY:
		{
			return m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_SERVER_DUMMY_ONLINE,0,NULL,0);
		}
	case SUB_CS_S_PLAZA_CUSTOMER:
		{
			//效验参数
			ASSERT(wDataSize == sizeof(CMD_CS_S_Customer));
			if (wDataSize != sizeof(CMD_CS_S_Customer)) return false;

				//变量定义
			CMD_CS_S_Customer * pCustomer = (CMD_CS_S_Customer *)pData;

			//遍历所有在线玩家
			WORD wEnumIndex = 0;
			CServerUserItem * pServerUserItem = NULL;
			do
			{
				pServerUserItem = m_ServerUserManager.EnumUserItem(wEnumIndex++);
				if (pServerUserItem != NULL)
				{
					CLocalUserItem * pLocalUerItem = (CLocalUserItem *)pServerUserItem;
					if (pLocalUerItem->GetMainStatus() != FRIEND_US_OFFLINE && pLocalUerItem->GetUserID() == pCustomer->dwUserID)
					{
						CMD_MB_Customer Customer;
						lstrcpyn(Customer.szContent, pCustomer->szContent, sizeof(Customer.szContent));
						m_pITCPNetworkEngine->SendData(pLocalUerItem->GetSocketID(), MDM_GP_USER_SERVICE, SUB_MB_CUSTOMER, &Customer, sizeof(Customer));
					}
				}
			} while (pServerUserItem != NULL);

			return true;
		}
	case SUB_CS_S_RECHARGE_SUCCESS:
		{
			//效验参数
			ASSERT(wDataSize == sizeof(CMD_CS_S_RechageSuccess));
			if (wDataSize != sizeof(CMD_CS_S_RechageSuccess)) return false;

			//变量定义
			CMD_CS_S_RechageSuccess * pRechageSuccess = (CMD_CS_S_RechageSuccess *)pData;

			//遍历所有在线玩家
			WORD wEnumIndex = 0;
			CServerUserItem * pServerUserItem = NULL;
			do
			{
				pServerUserItem = m_ServerUserManager.EnumUserItem(wEnumIndex++);
				if (pServerUserItem != NULL)
				{
					CLocalUserItem * pLocalUerItem = (CLocalUserItem *)pServerUserItem;
					if (pLocalUerItem->GetMainStatus() != FRIEND_US_OFFLINE && pLocalUerItem->GetUserID() == pRechageSuccess->dwUserID)
					{
						CMD_MB_RechargeSuccess RechargeSuccess;
						RechargeSuccess.lScore = pRechageSuccess->dRechargeScore;
						RechargeSuccess.nID = pRechageSuccess->nID;

						lstrcpyn(RechargeSuccess.szTitle, pRechageSuccess->szTitle, CountArray(RechargeSuccess.szTitle));
						lstrcpyn(RechargeSuccess.szSender, pRechageSuccess->szSender, CountArray(RechargeSuccess.szSender));
						lstrcpyn(RechargeSuccess.szContent, pRechageSuccess->szContent, CountArray(RechargeSuccess.szContent));
						lstrcpyn(RechargeSuccess.szSendTime, pRechageSuccess->szSendTime, CountArray(RechargeSuccess.szSendTime));

						RechargeSuccess.nStatus = pRechageSuccess->nStatus;

						m_pITCPNetworkEngine->SendData(pLocalUerItem->GetSocketID(), MDM_GP_USER_SERVICE, SUB_MB_RECHARGESUCCESS, &RechargeSuccess, sizeof(RechargeSuccess));
					}
				}
			} while (pServerUserItem != NULL);

			return true;
		}
	case SUB_CS_S_NOTIFYIDNEWMAIL:
		{
			//效验参数
			ASSERT(wDataSize == sizeof(CMD_CS_S_NotifyidNewMail));
			if (wDataSize != sizeof(CMD_CS_S_NotifyidNewMail)) return false;

			//变量定义
			CMD_CS_S_NotifyidNewMail * pNotifyidNewMail = (CMD_CS_S_NotifyidNewMail *)pData;

			//遍历所有在线玩家
			WORD wEnumIndex = 0;
			CServerUserItem * pServerUserItem = NULL;
			do
			{
				pServerUserItem = m_ServerUserManager.EnumUserItem(wEnumIndex++);
				if (pServerUserItem != NULL)
				{
					CLocalUserItem * pLocalUerItem = (CLocalUserItem *)pServerUserItem;
					if (pLocalUerItem->GetMainStatus() != FRIEND_US_OFFLINE && pLocalUerItem->GetUserID() == pNotifyidNewMail->dwUserID)
					{
						CMD_MB_NotifyidNewMail NotifyidNewMail;
						NotifyidNewMail.nID = pNotifyidNewMail->nID;

						lstrcpyn(NotifyidNewMail.szTitle, pNotifyidNewMail->szTitle, CountArray(NotifyidNewMail.szTitle));
						lstrcpyn(NotifyidNewMail.szSender, pNotifyidNewMail->szSender, CountArray(NotifyidNewMail.szSender));
						lstrcpyn(NotifyidNewMail.szContent, pNotifyidNewMail->szContent, CountArray(NotifyidNewMail.szContent));
						lstrcpyn(NotifyidNewMail.szSendTime, pNotifyidNewMail->szSendTime, CountArray(NotifyidNewMail.szSendTime));

						NotifyidNewMail.nStatus = pNotifyidNewMail->nStatus;

						m_pITCPNetworkEngine->SendData(pLocalUerItem->GetSocketID(), MDM_GP_USER_SERVICE, SUB_MB_NOTIFYIDNEWMAIL, &NotifyidNewMail, sizeof(NotifyidNewMail));
					}
				}
			} while (pServerUserItem != NULL);
			return true;
		}
	case SUB_SS_S_HALL_RADIO:
		{
			return OnEventTCPSocketSubRadioMessage(pData, wDataSize);
		}
	}

	return true;
}

//远程服务
bool CAttemperEngineSink::OnTCPSocketMainRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_SEARCH_CORRESPOND:	//协调查找
		{
			//变量定义
			CMD_CS_S_SearchCorrespond * pSearchCorrespond=(CMD_CS_S_SearchCorrespond *)pData;

			//效验参数
			ASSERT(wDataSize<=sizeof(CMD_CS_S_SearchCorrespond));
			ASSERT(wDataSize>=(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)));
			ASSERT(wDataSize==(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)+pSearchCorrespond->wUserCount*sizeof(pSearchCorrespond->UserRemoteInfo[0])));

			//效验参数
			if (wDataSize>sizeof(CMD_CS_S_SearchCorrespond)) return false;
			if (wDataSize<(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo))) return false;
			if (wDataSize!=(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)+pSearchCorrespond->wUserCount*sizeof(pSearchCorrespond->UserRemoteInfo[0]))) return false;

			//判断在线
			ASSERT(LOWORD(pSearchCorrespond->dwSocketID)<m_pInitParameter->m_wMaxConnect);
			if ((m_pBindParameter+LOWORD(pSearchCorrespond->dwSocketID))->dwSocketID!=pSearchCorrespond->dwSocketID) return true;

			//变量定义
			CMD_GP_S_SearchCorrespond SearchCorrespond;
			ZeroMemory(&SearchCorrespond,sizeof(SearchCorrespond));

			//设置变量
			for (WORD i=0;i<pSearchCorrespond->wUserCount;i++)
			{
				//数据效验
				ASSERT(SearchCorrespond.wUserCount<CountArray(SearchCorrespond.UserRemoteInfo));
				if (SearchCorrespond.wUserCount>=CountArray(SearchCorrespond.UserRemoteInfo)) break;

				//设置变量
				WORD wIndex=SearchCorrespond.wUserCount++;
				CopyMemory(&SearchCorrespond.UserRemoteInfo[wIndex],&pSearchCorrespond->UserRemoteInfo[i],sizeof(SearchCorrespond.UserRemoteInfo[wIndex]));
			}

			//发送数据
			WORD wHeadSize=sizeof(SearchCorrespond)-sizeof(SearchCorrespond.UserRemoteInfo);
			WORD wItemSize=sizeof(SearchCorrespond.UserRemoteInfo[0])*SearchCorrespond.wUserCount;
			m_pITCPNetworkEngine->SendData(pSearchCorrespond->dwSocketID,MDM_GP_REMOTE_SERVICE,SUB_GP_S_SEARCH_CORRESPOND,&SearchCorrespond,wHeadSize+wItemSize);

			return true;
		}
	case SUB_CS_S_SEARCH_ALLCORRESPOND:
		{
			//变量定义
			CMD_CS_S_SearchAllCorrespond * pSearchAllCorrespond=(CMD_CS_S_SearchAllCorrespond *)pData;


			BYTE cbDataBuffer[SOCKET_TCP_PACKET]={0};
			WORD cbDataSize = sizeof(CMD_GP_S_SearchAllCorrespond)-sizeof(tagUserRemoteInfo);
			CMD_GP_S_SearchAllCorrespond * pSearchCorrespond=(CMD_GP_S_SearchAllCorrespond *)cbDataBuffer;
			pSearchCorrespond->dwCount = pSearchAllCorrespond->dwCount;
			int nCount =(int)(pSearchAllCorrespond->dwCount);
			for(int i = 0; i < nCount; i++)
			{
				memcpy(&pSearchCorrespond->UserRemoteInfo[i], &pSearchAllCorrespond->UserRemoteInfo[i], sizeof(tagUserRemoteInfo));

				cbDataSize += sizeof(tagUserRemoteInfo);
			}

			if(pSearchAllCorrespond->dwCount == 0) cbDataSize = sizeof(CMD_GP_S_SearchAllCorrespond);
			m_pITCPNetworkEngine->SendData(pSearchAllCorrespond->dwSocketID,MDM_GP_REMOTE_SERVICE,SUB_GP_S_SEARCH_ALLCORRESPOND,pSearchCorrespond,cbDataSize);
			return true;
		}
	}

	return true;
}

//管理服务
bool CAttemperEngineSink::OnTCPSocketMainManagerService(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_PLATFORM_PARAMETER:	//平台参数
		{

			//平台参数
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_PLATFORM_PARAMETER,0,NULL,0);		

			//加载低保
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_BASEENSURE, 0, NULL, 0);

			return true;
		}
	}

	return true;
}

//列表处理
bool CAttemperEngineSink::OnTCPNetworkMainPCServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_GET_LIST:			//获取列表
		{
			//发送列表
			SendGameTypeInfo(dwSocketID);
			SendGameKindInfo(dwSocketID);

			//发送列表
			if (m_pInitParameter->m_cbDelayList==FALSE)
			{
				//发送列表
				SendGamePageInfo(dwSocketID,INVALID_WORD);
				SendGameNodeInfo(dwSocketID,INVALID_WORD);
				//SendGameServerInfo(dwSocketID,INVALID_WORD);
				SendGameServerInfo(dwSocketID,INVALID_WORD,DEVICE_TYPE_PC);
			}
			else
			{
				//发送页面
				SendGamePageInfo(dwSocketID,0);
			}

			//发送完成
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_FINISH);

			return true;
		}
	case SUB_GP_GET_SERVER:			//获取房间
		{
			//效验数据
			ASSERT(wDataSize%sizeof(WORD)==0);
			if (wDataSize%sizeof(WORD)!=0) return false;

			//发送列表
			UINT nKindCount=wDataSize/sizeof(WORD);
			for (UINT i=0;i<nKindCount;i++)
			{
				SendGameNodeInfo(dwSocketID,((WORD *)pData)[i]);
				SendGamePageInfo(dwSocketID,((WORD *)pData)[i]);
				//SendGameServerInfo(dwSocketID,((WORD *)pData)[i]);
				SendGameServerInfo(dwSocketID,((WORD *)pData)[i],DEVICE_TYPE_PC);
			}

			//发送完成
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_SERVER_FINISH,pData,wDataSize);

			return true;
		}
	case SUB_GP_GET_ONLINE:			//获取在线
		{
			//变量定义
			CMD_GP_GetOnline * pGetOnline=(CMD_GP_GetOnline *)pData;
			WORD wHeadSize=(sizeof(CMD_GP_GetOnline)-sizeof(pGetOnline->wOnLineServerID));

			//效验数据
			ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGetOnline->wServerCount*sizeof(WORD))));
			if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGetOnline->wServerCount*sizeof(WORD)))) return false;

			//变量定义
			CMD_GP_KindOnline KindOnline;
			CMD_GP_ServerOnline ServerOnline;
			ZeroMemory(&KindOnline,sizeof(KindOnline));
			ZeroMemory(&ServerOnline,sizeof(ServerOnline));

			//获取类型
			POSITION KindPosition=NULL;
			do
			{
				//获取类型
				CGameKindItem * pGameKindItem=m_ServerListManager.EmunGameKindItem(KindPosition);

				//设置变量
				if (pGameKindItem!=NULL)
				{
					WORD wKindIndex=KindOnline.wKindCount++;
					KindOnline.OnLineInfoKind[wKindIndex].wKindID=pGameKindItem->m_GameKind.wKindID;
					KindOnline.OnLineInfoKind[wKindIndex].dwOnLineCount=pGameKindItem->m_GameKind.dwOnLineCount+pGameKindItem->m_GameKind.dwAndroidCount;
				}

				//溢出判断
				if (KindOnline.wKindCount>=CountArray(KindOnline.OnLineInfoKind))
				{
					ASSERT(FALSE);
					break;
				}

			} while (KindPosition!=NULL);

			//获取房间
			for (WORD i=0;i<pGetOnline->wServerCount;i++)
			{
				//获取房间
				WORD wServerID=pGetOnline->wOnLineServerID[i];
				CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(wServerID);

				//设置变量
				if (pGameServerItem!=NULL)
				{
					WORD wServerIndex=ServerOnline.wServerCount++;
					ServerOnline.OnLineInfoServer[wServerIndex].wServerID=wServerID;
					ServerOnline.OnLineInfoServer[wServerIndex].dwOnLineCount=pGameServerItem->m_GameServer.dwOnLineCount+pGameServerItem->m_GameServer.dwAndroidCount;
				}

				//溢出判断
				if (ServerOnline.wServerCount>=CountArray(ServerOnline.OnLineInfoServer))
				{
					ASSERT(FALSE);
					break;
				}
			}

			//类型在线
			if (KindOnline.wKindCount>0)
			{
				WORD wHeadSize=sizeof(KindOnline)-sizeof(KindOnline.OnLineInfoKind);
				WORD wSendSize=wHeadSize+KindOnline.wKindCount*sizeof(KindOnline.OnLineInfoKind[0]);
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GR_KINE_ONLINE,&KindOnline,wSendSize);
			}

			//房间在线
			if (ServerOnline.wServerCount>0)
			{
				WORD wHeadSize=sizeof(ServerOnline)-sizeof(ServerOnline.OnLineInfoServer);
				WORD wSendSize=wHeadSize+ServerOnline.wServerCount*sizeof(ServerOnline.OnLineInfoServer[0]);
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GR_SERVER_ONLINE,&ServerOnline,wSendSize);
			}

			return true;
		}
	case SUB_GP_GET_COLLECTION:		//获取收藏
		{
			return true;
		}
	}

	return false;
}

//服务处理
bool CAttemperEngineSink::OnTCPNetworkMainPCUserService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_MODIFY_INSURE_PASS:		  //修改密码
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GP_ModifyInsurePass));
			if (wDataSize!=sizeof(CMD_GP_ModifyInsurePass)) return false;

			//处理消息
			CMD_GP_ModifyInsurePass * pModifyInsurePass=(CMD_GP_ModifyInsurePass *)pData;
			pModifyInsurePass->szDesPassword[CountArray(pModifyInsurePass->szDesPassword)-1]=0;
			pModifyInsurePass->szScrPassword[CountArray(pModifyInsurePass->szScrPassword)-1]=0;

			//变量定义
			DBR_GP_ModifyInsurePass ModifyInsurePass;
			ZeroMemory(&ModifyInsurePass,sizeof(ModifyInsurePass));

			//构造数据
			ModifyInsurePass.dwUserID=pModifyInsurePass->dwUserID;
			ModifyInsurePass.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyInsurePass.szDesPassword,pModifyInsurePass->szDesPassword,CountArray(ModifyInsurePass.szDesPassword));
			lstrcpyn(ModifyInsurePass.szScrPassword,pModifyInsurePass->szScrPassword,CountArray(ModifyInsurePass.szScrPassword));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_INSURE_PASS,dwSocketID,&ModifyInsurePass,sizeof(ModifyInsurePass));

			return true;
		}
	case SUB_GP_SYSTEM_FACE_INFO:		  //修改头像
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GP_SystemFaceInfo));
			if (wDataSize!=sizeof(CMD_GP_SystemFaceInfo)) return false;

			//处理消息
			CMD_GP_SystemFaceInfo * pSystemFaceInfo=(CMD_GP_SystemFaceInfo *)pData;

			//变量定义
			DBR_GP_ModifySystemFace ModifySystemFace;
			ZeroMemory(&ModifySystemFace,sizeof(ModifySystemFace));

			//构造数据
			ModifySystemFace.wFaceID=pSystemFaceInfo->wFaceID;
			ModifySystemFace.dwUserID=pSystemFaceInfo->dwUserID;
			ModifySystemFace.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifySystemFace.szPassword,pSystemFaceInfo->szPassword,CountArray(ModifySystemFace.szPassword));
			lstrcpyn(ModifySystemFace.szMachineID,pSystemFaceInfo->szMachineID,CountArray(ModifySystemFace.szMachineID));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_SYSTEM_FACE,dwSocketID,&ModifySystemFace,sizeof(ModifySystemFace));

			return true;
		}
	case SUB_GP_MODIFY_INDIVIDUAL:		  //修改资料
		{
			//效验参数
			ASSERT(wDataSize>=sizeof(CMD_GP_ModifyIndividual));
			if (wDataSize<sizeof(CMD_GP_ModifyIndividual)) return false;

			//处理消息
			CMD_GP_ModifyIndividual * pModifyIndividual=(CMD_GP_ModifyIndividual *)pData;
			pModifyIndividual->szPassword[CountArray(pModifyIndividual->szPassword)-1]=0;

			//变量定义
			DBR_GP_ModifyIndividual ModifyIndividual;
			ZeroMemory(&ModifyIndividual,sizeof(ModifyIndividual));

			//设置变量
			ModifyIndividual.dwUserID=pModifyIndividual->dwUserID;
			ModifyIndividual.dwClientAddr = (m_pBindParameter + LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyIndividual.szPassword,pModifyIndividual->szPassword,CountArray(ModifyIndividual.szPassword));
			lstrcpyn(ModifyIndividual.szNickName, pModifyIndividual->szNickName, CountArray(ModifyIndividual.szNickName));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_INDIVIDUAL,dwSocketID,&ModifyIndividual,sizeof(ModifyIndividual));

			return true;
		}
	case SUB_GP_USER_ENABLE_INSURE:		  //开通银行
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GP_UserEnableInsure));
			if (wDataSize!=sizeof(CMD_GP_UserEnableInsure)) return false;

			//处理消息
			CMD_GP_UserEnableInsure * pUserEnableInsure=(CMD_GP_UserEnableInsure *)pData;
			pUserEnableInsure->szLogonPass[CountArray(pUserEnableInsure->szLogonPass)-1]=0;
			pUserEnableInsure->szInsurePass[CountArray(pUserEnableInsure->szInsurePass)-1]=0;
			pUserEnableInsure->szMachineID[CountArray(pUserEnableInsure->szMachineID)-1]=0;

			//变量定义
			DBR_GP_UserEnableInsure UserEnableInsure;
			ZeroMemory(&UserEnableInsure,sizeof(UserEnableInsure));

			//构造数据
			UserEnableInsure.dwUserID=pUserEnableInsure->dwUserID;			
			UserEnableInsure.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserEnableInsure.szLogonPass,pUserEnableInsure->szLogonPass,CountArray(UserEnableInsure.szLogonPass));
			lstrcpyn(UserEnableInsure.szInsurePass,pUserEnableInsure->szInsurePass,CountArray(UserEnableInsure.szInsurePass));
			lstrcpyn(UserEnableInsure.szMachineID,pUserEnableInsure->szMachineID,CountArray(UserEnableInsure.szMachineID));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_ENABLE_INSURE,dwSocketID,&UserEnableInsure,sizeof(UserEnableInsure));

			return true;
		}
	case SUB_GP_USER_SAVE_SCORE:		  //存入游戏币
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GP_UserSaveScore));
			if (wDataSize!=sizeof(CMD_GP_UserSaveScore)) return false;

			//处理消息
			CMD_GP_UserSaveScore * pUserSaveScore=(CMD_GP_UserSaveScore *)pData;
			pUserSaveScore->szMachineID[CountArray(pUserSaveScore->szMachineID)-1]=0;

			//变量定义
			DBR_GP_UserSaveScore UserSaveScore;
			ZeroMemory(&UserSaveScore,sizeof(UserSaveScore));

			//构造数据
			UserSaveScore.dwUserID=pUserSaveScore->dwUserID;
			UserSaveScore.lSaveScore=pUserSaveScore->lSaveScore;
			UserSaveScore.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserSaveScore.szMachineID,pUserSaveScore->szMachineID,CountArray(UserSaveScore.szMachineID));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_SAVE_SCORE,dwSocketID,&UserSaveScore,sizeof(UserSaveScore));

			return true;
		}
	case SUB_GP_EXCHANGE_BANK_SCORE:
		{
			//效验参数
			ASSERT(wDataSize == sizeof(CMD_GP_ExchangeBankScore));
			int asd = sizeof(CMD_GP_ExchangeBankScore);
			if (wDataSize != sizeof(CMD_GP_ExchangeBankScore)) return false;

			//处理消息
			CMD_GP_ExchangeBankScore * pClientData = (CMD_GP_ExchangeBankScore *)pData;
			pClientData->szBankPass[CountArray(pClientData->szBankPass) - 1] = 0;
			pClientData->szOrderID[CountArray(pClientData->szOrderID) - 1] = 0;

			//变量定义
			DBR_GP_ExchangeBankScore DBRData;
			ZeroMemory(&DBRData, sizeof(DBRData));
			//构造数据
			DBRData.dwUserID = pClientData->dwUserID;
			DBRData.dwScore = pClientData->dwScore;
			lstrcpyn(DBRData.szBankPass, pClientData->szBankPass, CountArray(DBRData.szBankPass));
			lstrcpyn(DBRData.szOrderID, pClientData->szOrderID, CountArray(DBRData.szOrderID));

			DBRData.dwClientAddr = (m_pBindParameter + LOWORD(dwSocketID))->dwClientAddr;
			DBRData.dwType = pClientData->dwType;
			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_EXCHANGE_BANK_SCORE, dwSocketID, &DBRData, sizeof(DBRData));
			return true;
		}
	case SUB_GP_USER_TAKE_SCORE:		  //取出游戏币
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GP_UserTakeScore));
			if (wDataSize!=sizeof(CMD_GP_UserTakeScore)) return false;

			//处理消息
			CMD_GP_UserTakeScore * pUserTakeScore=(CMD_GP_UserTakeScore *)pData;
			pUserTakeScore->szPassword[CountArray(pUserTakeScore->szPassword)-1]=0;
			pUserTakeScore->szMachineID[CountArray(pUserTakeScore->szMachineID)-1]=0;

			//变量定义
			DBR_GP_UserTakeScore UserTakeScore;
			ZeroMemory(&UserTakeScore,sizeof(UserTakeScore));

			//构造数据
			UserTakeScore.dwUserID=pUserTakeScore->dwUserID;
			UserTakeScore.lTakeScore=pUserTakeScore->lTakeScore;
			UserTakeScore.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserTakeScore.szPassword,pUserTakeScore->szPassword,CountArray(UserTakeScore.szPassword));
			lstrcpyn(UserTakeScore.szMachineID,pUserTakeScore->szMachineID,CountArray(UserTakeScore.szMachineID));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_TAKE_SCORE,dwSocketID,&UserTakeScore,sizeof(UserTakeScore));

			return true;
		}
	case SUB_GP_USER_TRANSFER_SCORE:	  //转帐游戏币
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GP_UserTransferScore));
			if (wDataSize!=sizeof(CMD_GP_UserTransferScore)) return false;

			//处理消息
			CMD_GP_UserTransferScore * pUserTransferScore=(CMD_GP_UserTransferScore *)pData;
			pUserTransferScore->szAccounts[CountArray(pUserTransferScore->szAccounts)-1]=0;
			pUserTransferScore->szPassword[CountArray(pUserTransferScore->szPassword)-1]=0;
			pUserTransferScore->szMachineID[CountArray(pUserTransferScore->szMachineID)-1]=0;

			//变量定义
			DBR_GP_UserTransferScore UserTransferScore;
			ZeroMemory(&UserTransferScore,sizeof(UserTransferScore));

			//构造数据
			UserTransferScore.dwUserID=pUserTransferScore->dwUserID;
			UserTransferScore.lTransferScore=pUserTransferScore->lTransferScore;
			UserTransferScore.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserTransferScore.szAccounts,pUserTransferScore->szAccounts,CountArray(UserTransferScore.szAccounts));
			lstrcpyn(UserTransferScore.szPassword,pUserTransferScore->szPassword,CountArray(UserTransferScore.szPassword));
			lstrcpyn(UserTransferScore.szMachineID,pUserTransferScore->szMachineID,CountArray(UserTransferScore.szMachineID));
			lstrcpyn(UserTransferScore.szTransRemark,pUserTransferScore->szTransRemark,CountArray(UserTransferScore.szTransRemark));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_TRANSFER_SCORE,dwSocketID,&UserTransferScore,sizeof(UserTransferScore));

			return true;
		}
	case SUB_GP_QUERY_INSURE_INFO:		  //查询银行
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GP_QueryInsureInfo));
			if (wDataSize!=sizeof(CMD_GP_QueryInsureInfo)) return false;

			//处理消息
			CMD_GP_QueryInsureInfo * pQueryInsureInfo=(CMD_GP_QueryInsureInfo *)pData;

			//变量定义
			DBR_GP_QueryInsureInfo QueryInsureInfo;
			ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

			//构造数据
			QueryInsureInfo.dwUserID=pQueryInsureInfo->dwUserID;
			QueryInsureInfo.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(QueryInsureInfo.szPassword,pQueryInsureInfo->szPassword,CountArray(QueryInsureInfo.szPassword));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_INSURE_INFO,dwSocketID,&QueryInsureInfo,sizeof(QueryInsureInfo));

			return true;
		}
	case SUB_GP_QUERY_USER_INFO_REQUEST:  //查询用户
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GP_QueryUserInfoRequest));
			if (wDataSize!=sizeof(CMD_GP_QueryUserInfoRequest)) return false;

			//处理消息
			CMD_GP_QueryUserInfoRequest * pQueryUserInfoRequest=(CMD_GP_QueryUserInfoRequest *)pData;

			//ID判断
			if(pQueryUserInfoRequest->cbByNickName==FALSE)
			{
				//长度判断
				int nLen=lstrlen(pQueryUserInfoRequest->szAccounts);
				if(nLen>=8)
				{
					SendInsureFailure(dwSocketID,0,TEXT("请输入合法的玩家ID！"));
					return true;
				}

				//合法判断
				for(int i=0; i<nLen; i++)
				{
					if(pQueryUserInfoRequest->szAccounts[i] < TEXT('0') || pQueryUserInfoRequest->szAccounts[i] > TEXT('9'))
					{
						SendInsureFailure(dwSocketID,0,TEXT("请输入合法的玩家ID！"));
						return true;
					}
				}
			}

			//变量定义
			DBR_GP_QueryInsureUserInfo QueryInsureUserInfo;
			ZeroMemory(&QueryInsureUserInfo,sizeof(QueryInsureUserInfo));

			//构造数据
			QueryInsureUserInfo.cbByNickName=pQueryUserInfoRequest->cbByNickName;
			lstrcpyn(QueryInsureUserInfo.szAccounts,pQueryUserInfoRequest->szAccounts,CountArray(QueryInsureUserInfo.szAccounts));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_USER_INFO,dwSocketID,&QueryInsureUserInfo,sizeof(QueryInsureUserInfo));

			return true;
		}
	case SUB_GP_BASEENSURE_LOAD:		//加载低保
		{
			//构造结构
			CMD_GP_BaseEnsureParamter BaseEnsureParameter;
			BaseEnsureParameter.cbTakeTimes=m_BaseEnsureParameter.cbTakeTimes;
			BaseEnsureParameter.lScoreAmount=m_BaseEnsureParameter.lScoreAmount;
			BaseEnsureParameter.lScoreCondition=m_BaseEnsureParameter.lScoreCondition;

			//投递请求
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GP_BASEENSURE_PARAMETER,&BaseEnsureParameter,sizeof(BaseEnsureParameter));

			return true;
		}
	case SUB_GP_BASEENSURE_TAKE:		  //领取低保
		{
			//参数校验
			ASSERT(wDataSize==sizeof(CMD_GP_BaseEnsureTake));
			if(wDataSize!=sizeof(CMD_GP_BaseEnsureTake)) return false;

			//提取数据
			CMD_GP_BaseEnsureTake * pBaseEnsureTake = (CMD_GP_BaseEnsureTake *)pData;
			pBaseEnsureTake->szPassword[CountArray(pBaseEnsureTake->szPassword)-1]=0;
			pBaseEnsureTake->szMachineID[CountArray(pBaseEnsureTake->szMachineID)-1]=0;

			//构造结构
			DBR_GP_TakeBaseEnsure TakeBaseEnsure;
			TakeBaseEnsure.dwUserID = pBaseEnsureTake->dwUserID;
			TakeBaseEnsure.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(TakeBaseEnsure.szPassword,pBaseEnsureTake->szPassword,CountArray(TakeBaseEnsure.szPassword));
			lstrcpyn(TakeBaseEnsure.szMachineID,pBaseEnsureTake->szMachineID,CountArray(TakeBaseEnsure.szMachineID));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_BASEENSURE_TAKE,dwSocketID,&TakeBaseEnsure,sizeof(TakeBaseEnsure));

			return true;
		}
	case SUB_GP_QUERY_GAMEROOM:				//查询可加入游戏房间
		{
			//参数校验
			ASSERT(wDataSize == sizeof(CMD_GP_QueryGameRoom));
			if (wDataSize != sizeof(CMD_GP_QueryGameRoom)) return false;

			//提取数据
			CMD_GP_QueryGameRoom * pQueryGameRoom = (CMD_GP_QueryGameRoom *)pData;

			//枚举数据
			POSITION Position = NULL;
			CGameServerItem * pGameServerItem = NULL;

			//枚举数据
			for (DWORD i = 0; i<m_ServerListManager.GetGameServerCount(); i++)
			{
				//获取数据
				pGameServerItem = m_ServerListManager.EmunGameServerItem(Position);
				if (pGameServerItem == NULL) break;

				//拷贝数据
				if (pGameServerItem->m_GameServer.wKindID == pQueryGameRoom->wKindID && pGameServerItem->m_GameServer.wServerLevel == pQueryGameRoom->wServerLevel)
				{
					if (pGameServerItem->m_GameServer.dwOnLineCount < (pGameServerItem->m_GameServer.dwFullCount))
					{
						//构造结构
						CMD_GP_QueryGameRoomResult QueryGameRoomResult;
						QueryGameRoomResult.wServerPort = pGameServerItem->m_GameServer.wServerPort;
						lstrcpyn(QueryGameRoomResult.szServerAddr, pGameServerItem->m_GameServer.szServerAddr, CountArray(QueryGameRoomResult.szServerAddr));

						//发送数据
						m_pITCPNetworkEngine->SendData(dwSocketID, MDM_GP_USER_SERVICE, SUB_GP_QUERY_GAMEROOM_RESULT, &QueryGameRoomResult, sizeof(QueryGameRoomResult));
						break;
					}
				}
			}

			return true;
		}
	case SUB_GP_LOTTERY_CONFIG_REQ:			//请求配置
		{
			//参数校验
			ASSERT(wDataSize==sizeof(CMD_GP_LotteryConfigReq));
			if(wDataSize!=sizeof(CMD_GP_LotteryConfigReq)) return false;

			//构造结构
			CMD_GP_LotteryConfigReq * pLotteryConfigReq = (CMD_GP_LotteryConfigReq*)pData;
			pLotteryConfigReq->szLogonPass[CountArray(pLotteryConfigReq->szLogonPass)-1]=0;

			//构造结构
			DBR_GP_LotteryConfigReq LotteryConfigReq;
			ZeroMemory(&LotteryConfigReq,sizeof(LotteryConfigReq));

			//设置变量
			LotteryConfigReq.dwUserID=pLotteryConfigReq->dwUserID;
			LotteryConfigReq.wKindID=pLotteryConfigReq->wKindID;
			LotteryConfigReq.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(LotteryConfigReq.szLogonPass,pLotteryConfigReq->szLogonPass,CountArray(LotteryConfigReq.szLogonPass));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOTTERY_CONFIG_REQ,dwSocketID,&LotteryConfigReq,sizeof(LotteryConfigReq));

			return true;
		}
	case SUB_GP_LOTTERY_START:				//抽奖开始
		{
			//参数校验
			ASSERT(wDataSize==sizeof(CMD_GP_LotteryStart));
			if(wDataSize!=sizeof(CMD_GP_LotteryStart)) return false;

			//构造结构
			CMD_GP_LotteryStart * pLotteryStart = (CMD_GP_LotteryStart*)pData;
			pLotteryStart->szLogonPass[CountArray(pLotteryStart->szLogonPass)-1]=0;
			pLotteryStart->szMachineID[CountArray(pLotteryStart->szMachineID)-1]=0;

			//构造结构
			DBR_GP_LotteryStart LotteryStart;
			ZeroMemory(&LotteryStart,sizeof(LotteryStart));

			//设置变量
			LotteryStart.dwUserID=pLotteryStart->dwUserID;
			LotteryStart.wKindID=pLotteryStart->wKindID;
			LotteryStart.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(LotteryStart.szLogonPass,pLotteryStart->szLogonPass,CountArray(LotteryStart.szLogonPass));
			lstrcpyn(LotteryStart.szMachineID,pLotteryStart->szMachineID,CountArray(LotteryStart.szMachineID));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOTTERY_START,dwSocketID,&LotteryStart,sizeof(LotteryStart));

			return true;
		}
	case SUB_GP_QUERY_EMAIL:
		{
			//参数校验
			ASSERT(wDataSize == sizeof(CMD_GP_QueryEmail));
			if (wDataSize != sizeof(CMD_GP_QueryEmail)) return false;

				//构造结构
			CMD_GP_QueryEmail * pQueryEmail = (CMD_GP_QueryEmail*)pData;

			//构造结构
			DBR_GP_QueryEmail QueryEmail;
			ZeroMemory(&QueryEmail, sizeof(QueryEmail));

			//设置变量
			QueryEmail.dwUserID = pQueryEmail->dwUserID;
			QueryEmail.nIndex = pQueryEmail->nIndex;

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_EMAIL, dwSocketID, &QueryEmail, sizeof(QueryEmail));

			return true;
		}
	}

	return false;
}

//远程处理
bool CAttemperEngineSink::OnTCPNetworkMainPCRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_C_SEARCH_CORRESPOND:	//协调查找
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GP_C_SearchCorrespond));
			if (wDataSize!=sizeof(CMD_GP_C_SearchCorrespond)) return false;

			//处理消息
			CMD_GP_C_SearchCorrespond * pSearchCorrespond=(CMD_GP_C_SearchCorrespond *)pData;
			pSearchCorrespond->szNickName[CountArray(pSearchCorrespond->szNickName)-1]=0;

			//变量定义
			CMD_CS_C_SearchCorrespond SearchCorrespond;
			ZeroMemory(&SearchCorrespond,sizeof(SearchCorrespond));

			//连接变量
			SearchCorrespond.dwSocketID=dwSocketID;
			SearchCorrespond.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;

			//查找变量
			SearchCorrespond.dwGameID=pSearchCorrespond->dwGameID;
			lstrcpyn(SearchCorrespond.szNickName,pSearchCorrespond->szNickName,CountArray(SearchCorrespond.szNickName));

			//发送数据
			m_pITCPSocketService->SendData(MDM_CS_REMOTE_SERVICE,SUB_CS_C_SEARCH_CORRESPOND,&SearchCorrespond,sizeof(SearchCorrespond));

			return true;
		}
	case SUB_GP_C_SEARCH_ALLCORRESPOND:
		{
			//发送数据
			if(wDataSize >= sizeof(CMD_GP_C_SearchAllCorrespond)) 
			{
				CMD_GP_C_SearchAllCorrespond* pSearchAllCorrespond = (CMD_GP_C_SearchAllCorrespond*)pData;

				DWORD cBuffer[512]={0};
				CMD_CS_C_AllSearchCorrespond* pSearchCorrespond = (CMD_CS_C_AllSearchCorrespond*)cBuffer;

				//连接变量
				pSearchCorrespond->dwSocketID=dwSocketID;
				pSearchCorrespond->dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;

				pSearchCorrespond->dwCount = pSearchAllCorrespond->dwCount;
				memcpy(pSearchCorrespond->dwGameID, pSearchAllCorrespond->dwGameID, sizeof(DWORD)*pSearchAllCorrespond->dwCount);

				m_pITCPSocketService->SendData(MDM_CS_REMOTE_SERVICE,SUB_CS_C_SEARCH_ALLCORRESPOND,pSearchCorrespond, sizeof(CMD_CS_C_AllSearchCorrespond)+sizeof(DWORD)*(WORD)(pSearchAllCorrespond->dwCount-1));
			}
			return true;
		}
	}

	return false;
}


//机器服务
bool CAttemperEngineSink::OnTCPNetworkMainAndroidService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_GET_PARAMETER:		//获取参数
		{
			//参数校验
			ASSERT(wDataSize==sizeof(CMD_GP_GetParameter));
			if(wDataSize!=sizeof(CMD_GP_GetParameter)) return false;

			//提取数据
			CMD_GP_GetParameter * pGetParameter = (CMD_GP_GetParameter *)pData;
			ASSERT(pGetParameter!=NULL);

			//构造结构
			DBR_GP_GetParameter GetParameter;
			GetParameter.wServerID = pGetParameter->wServerID;

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_PARAMETER,dwSocketID,&GetParameter,sizeof(GetParameter));

			return true;
		}
	case SUB_GP_ADD_PARAMETER:		//添加参数
		{
			//参数校验
			ASSERT(wDataSize==sizeof(CMD_GP_AddParameter));
			if(wDataSize!=sizeof(CMD_GP_AddParameter)) return false;

			//提取数据
			CMD_GP_AddParameter * pAddParameter = (CMD_GP_AddParameter *)pData;
			ASSERT(pAddParameter!=NULL);

			//构造结构
			DBR_GP_AddParameter AddParameter;
			AddParameter.wServerID = pAddParameter->wServerID;
			CopyMemory(&AddParameter.AndroidParameter,&pAddParameter->AndroidParameter,sizeof(tagAndroidParameter));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_ADD_PARAMETER,dwSocketID,&AddParameter,sizeof(AddParameter));

			return true;
		}
	case SUB_GP_MODIFY_PARAMETER:	//修改参数
		{
			//参数校验
			ASSERT(wDataSize==sizeof(CMD_GP_ModifyParameter));
			if(wDataSize!=sizeof(CMD_GP_ModifyParameter)) return false;

			//提取数据
			CMD_GP_ModifyParameter * pModifyParameter = (CMD_GP_ModifyParameter *)pData;
			ASSERT(pModifyParameter!=NULL);

			//构造结构
			DBR_GP_ModifyParameter ModifyParameter;
			ModifyParameter.wServerID=pModifyParameter->wServerID;
			CopyMemory(&ModifyParameter.AndroidParameter,&pModifyParameter->AndroidParameter,sizeof(tagAndroidParameter));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_PARAMETER,dwSocketID,&ModifyParameter,sizeof(ModifyParameter));
			
			return true;
		}
	case SUB_GP_DELETE_PARAMETER:	//删除参数
		{
			//参数校验
			ASSERT(wDataSize==sizeof(CMD_GP_DeleteParameter));
			if(wDataSize!=sizeof(CMD_GP_DeleteParameter)) return false;

			//提取数据
			CMD_GP_DeleteParameter * pDeleteParameter = (CMD_GP_DeleteParameter *)pData;
			ASSERT(pDeleteParameter!=NULL);

			//构造结构
			DBR_GP_DeleteParameter DeleteParameter;
			DeleteParameter.wServerID = pDeleteParameter->wServerID;
			DeleteParameter.dwBatchID = pDeleteParameter->dwBatchID;

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_DELETE_PARAMETER,dwSocketID,&DeleteParameter,sizeof(DeleteParameter));
			
			return true;
		}
	}

	return false;
}

//登录处理
bool CAttemperEngineSink::OnTCPNetworkMainMBLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_MB_LOGON_GAMEID:		//I D 登录
		{
			return OnTCPNetworkSubMBLogonGameID(pData,wDataSize,dwSocketID);
		}
	case SUB_MB_LOGON_ACCOUNTS:		//帐号登录
		{
			return OnTCPNetworkSubMBLogonAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_MB_REGISTER_ACCOUNTS:	//帐号注册
		{
			return OnTCPNetworkSubMBRegisterAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_MB_LOGON_OTHERPLATFORM: //其他平台
		{
			return OnTCPNetworkSubMBLogonOtherPlatform(pData,wDataSize,dwSocketID);
		}
	case SUB_MB_LOGON_VISITOR:      //游客登录
		{
			return OnTCPNetworkSubMBLogonVisitor(pData,wDataSize,dwSocketID);
		}
	case SUB_MB_QUERY_GAMELOCKINFO:
		{
			return OnTCPNetworkSubMBQueryGameLockInfo(pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//列表处理
bool CAttemperEngineSink::OnTCPNetworkMainMBServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch(wSubCmdID)
	{
	case SUB_MB_GET_ONLINE:
		{
			return OnTCPNetworkSubMBGetOnline(pData,wDataSize,dwSocketID);
		}
	case SUB_QUEREY_SERVER:
		{
			return OnTCPNetworkSubMBQueryServer(pData,wDataSize,dwSocketID);
		}
	}
	return false;
}

//广播消息
bool CAttemperEngineSink::OnEventTCPSocketSubRadioMessage(VOID * pData, WORD wDataSize)
{
	//遍历所有在线玩家
	WORD wEnumIndex = 0;
	CServerUserItem * pServerUserItem = NULL;
	do
	{
		pServerUserItem = m_ServerUserManager.EnumUserItem(wEnumIndex++);
		if (pServerUserItem != NULL)
		{
			CLocalUserItem * pLocalUerItem = (CLocalUserItem *)pServerUserItem;
			if (pLocalUerItem->GetMainStatus() != FRIEND_US_OFFLINE)
			{
				m_pITCPNetworkEngine->SendData(pLocalUerItem->GetSocketID(), MDM_GP_USER_SERVICE, SUB_MB_RADIO_MESSAGE, pData, wDataSize);
			}
		}
	} while (pServerUserItem != NULL);

	return true;
}

//I D 登录
bool CAttemperEngineSink::OnTCPNetworkSubMBLogonGameID(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize>=sizeof(CMD_MB_LogonGameID));
	if (wDataSize<sizeof(CMD_MB_LogonGameID)) return false;

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//处理消息
	CMD_MB_LogonGameID * pLogonGameID=(CMD_MB_LogonGameID *)pData;
	pLogonGameID->szPassword[CountArray(pLogonGameID->szPassword)-1]=0;
	pLogonGameID->szMachineID[CountArray(pLogonGameID->szMachineID)-1]=0;
	pLogonGameID->szMobilePhone[CountArray(pLogonGameID->szMobilePhone)-1]=0;

	//设置连接
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pLogonGameID->wModuleID;
	pBindParameter->dwPlazaVersion=pLogonGameID->dwPlazaVersion;

	//效验版本
	if (CheckPlazaVersion(pLogonGameID->cbDeviceType,pLogonGameID->dwPlazaVersion,dwSocketID)==false) return true;

	//变量定义
	DBR_MB_LogonGameID LogonGameID;
	ZeroMemory(&LogonGameID,sizeof(LogonGameID));

	//附加信息
	LogonGameID.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//构造数据
	LogonGameID.dwGameID=pLogonGameID->dwGameID;
	LogonGameID.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonGameID.szPassword,pLogonGameID->szPassword,CountArray(LogonGameID.szPassword));
	lstrcpyn(LogonGameID.szMachineID,pLogonGameID->szMachineID,CountArray(LogonGameID.szMachineID));
	lstrcpyn(LogonGameID.szMobilePhone,pLogonGameID->szMobilePhone,CountArray(LogonGameID.szMobilePhone));

	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_LOGON_GAMEID,dwSocketID,&LogonGameID,sizeof(LogonGameID));

	return true;
}

//帐号登录
bool CAttemperEngineSink::OnTCPNetworkSubMBLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize>=sizeof(CMD_MB_LogonAccounts));
	if (wDataSize<sizeof(CMD_MB_LogonAccounts)) return false;

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//处理消息
	CMD_MB_LogonAccounts * pLogonAccounts=(CMD_MB_LogonAccounts *)pData;
	pLogonAccounts->szAccounts[CountArray(pLogonAccounts->szAccounts)-1]=0;
	pLogonAccounts->szPassword[CountArray(pLogonAccounts->szPassword)-1]=0;
	pLogonAccounts->szMachineID[CountArray(pLogonAccounts->szMachineID)-1]=0;
	pLogonAccounts->szMobilePhone[CountArray(pLogonAccounts->szMobilePhone)-1]=0;

	//设置连接
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pLogonAccounts->wModuleID;
	pBindParameter->dwPlazaVersion=pLogonAccounts->dwPlazaVersion;

	//版本判断
	if (CheckPlazaVersion(pLogonAccounts->cbDeviceType,pLogonAccounts->dwPlazaVersion,dwSocketID)==false) return true;

	//变量定义
	DBR_MB_LogonAccounts LogonAccounts;
	ZeroMemory(&LogonAccounts,sizeof(LogonAccounts));

	//附加信息
	LogonAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//构造数据
	LogonAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonAccounts.szAccounts,pLogonAccounts->szAccounts,CountArray(LogonAccounts.szAccounts));
	lstrcpyn(LogonAccounts.szPassword,pLogonAccounts->szPassword,CountArray(LogonAccounts.szPassword));
	lstrcpyn(LogonAccounts.szMachineID,pLogonAccounts->szMachineID,CountArray(LogonAccounts.szMachineID));
	lstrcpyn(LogonAccounts.szMobilePhone,pLogonAccounts->szMobilePhone,CountArray(LogonAccounts.szMobilePhone));

	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_LOGON_ACCOUNTS,dwSocketID,&LogonAccounts,sizeof(LogonAccounts));

	return true;
}

//帐号注册
bool CAttemperEngineSink::OnTCPNetworkSubMBRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize>=sizeof(CMD_MB_RegisterAccounts));
	if (wDataSize<sizeof(CMD_MB_RegisterAccounts)) return false;


	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//处理消息
	CMD_MB_RegisterAccounts * pRegisterAccounts=(CMD_MB_RegisterAccounts *)pData;
	pRegisterAccounts->szAccounts[CountArray(pRegisterAccounts->szAccounts)-1]=0;
	pRegisterAccounts->szNickName[CountArray(pRegisterAccounts->szNickName)-1]=0;
	pRegisterAccounts->szMachineID[CountArray(pRegisterAccounts->szMachineID)-1]=0;
	pRegisterAccounts->szLogonPass[CountArray(pRegisterAccounts->szLogonPass)-1]=0;
	pRegisterAccounts->szMobilePhone[CountArray(pRegisterAccounts->szMobilePhone)-1]=0;

	//设置连接
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pRegisterAccounts->wModuleID;
	pBindParameter->dwPlazaVersion=pRegisterAccounts->dwPlazaVersion;

	//效验版本
	if (CheckPlazaVersion(pRegisterAccounts->cbDeviceType,pRegisterAccounts->dwPlazaVersion,dwSocketID)==false) return true;

	//变量定义
	DBR_MB_RegisterAccounts RegisterAccounts;
	ZeroMemory(&RegisterAccounts,sizeof(RegisterAccounts));

	//附加信息
	RegisterAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//构造数据
	RegisterAccounts.cbDeviceType = pRegisterAccounts->cbDeviceType;
	RegisterAccounts.wFaceID=pRegisterAccounts->wFaceID;
	RegisterAccounts.cbGender=pRegisterAccounts->cbGender;
	RegisterAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	RegisterAccounts.dwSpreaderGameID=pRegisterAccounts->dwSpreaderGameID;
	lstrcpyn(RegisterAccounts.szAgentID,pRegisterAccounts->szAgentID,CountArray(RegisterAccounts.szAgentID));
	lstrcpyn(RegisterAccounts.szAccounts,pRegisterAccounts->szAccounts,CountArray(RegisterAccounts.szAccounts));
	lstrcpyn(RegisterAccounts.szNickName,pRegisterAccounts->szNickName,CountArray(RegisterAccounts.szNickName));
	lstrcpyn(RegisterAccounts.szMachineID,pRegisterAccounts->szMachineID,CountArray(RegisterAccounts.szMachineID));
	lstrcpyn(RegisterAccounts.szLogonPass,pRegisterAccounts->szLogonPass,CountArray(RegisterAccounts.szLogonPass));
	lstrcpyn(RegisterAccounts.szMobilePhone,pRegisterAccounts->szMobilePhone,CountArray(RegisterAccounts.szMobilePhone));

	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_REGISTER_ACCOUNTS,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));

	return true;
}

//其他登录
bool CAttemperEngineSink::OnTCPNetworkSubMBLogonOtherPlatform(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize>=sizeof(CMD_MB_LogonOtherPlatform));
	if (wDataSize<sizeof(CMD_MB_LogonOtherPlatform)) return false;

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//处理消息
	CMD_MB_LogonOtherPlatform * pLogonOtherPlatform=(CMD_MB_LogonOtherPlatform *)pData;
	pLogonOtherPlatform->szUserUin[CountArray(pLogonOtherPlatform->szUserUin)-1]=0;
	pLogonOtherPlatform->szNickName[CountArray(pLogonOtherPlatform->szNickName)-1]=0;
	pLogonOtherPlatform->szMachineID[CountArray(pLogonOtherPlatform->szMachineID)-1]=0;
	pLogonOtherPlatform->szMobilePhone[CountArray(pLogonOtherPlatform->szMobilePhone)-1]=0;
	pLogonOtherPlatform->szCompellation[CountArray(pLogonOtherPlatform->szCompellation)-1]=0;

	//平台判断
	ASSERT(pLogonOtherPlatform->cbPlatformID==ULMBySina || pLogonOtherPlatform->cbPlatformID==ULMByTencent || pLogonOtherPlatform->cbPlatformID==ULMByRenRen);
	if (pLogonOtherPlatform->cbPlatformID!=ULMBySina && pLogonOtherPlatform->cbPlatformID!=ULMByTencent && pLogonOtherPlatform->cbPlatformID!=ULMByRenRen) return false;

	//设置连接
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pLogonOtherPlatform->wModuleID;
	pBindParameter->dwPlazaVersion=pLogonOtherPlatform->dwPlazaVersion;

	//版本判断
	if (CheckPlazaVersion(pLogonOtherPlatform->cbDeviceType,pLogonOtherPlatform->dwPlazaVersion,dwSocketID)==false) return true;

	//变量定义
	DBR_MB_LogonOtherPlatform LogonOtherPlatform;
	ZeroMemory(&LogonOtherPlatform,sizeof(LogonOtherPlatform));

	//附加信息
	LogonOtherPlatform.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//构造数据
	LogonOtherPlatform.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	LogonOtherPlatform.cbGender=pLogonOtherPlatform->cbGender;
	LogonOtherPlatform.cbPlatformID=pLogonOtherPlatform->cbPlatformID;
	lstrcpyn(LogonOtherPlatform.szAgentID,pLogonOtherPlatform->szAgentID,CountArray(LogonOtherPlatform.szAgentID));
	lstrcpyn(LogonOtherPlatform.szUserUin,pLogonOtherPlatform->szUserUin,CountArray(LogonOtherPlatform.szUserUin));
	lstrcpyn(LogonOtherPlatform.szNickName,pLogonOtherPlatform->szNickName,CountArray(LogonOtherPlatform.szNickName));
	lstrcpyn(LogonOtherPlatform.szMachineID,pLogonOtherPlatform->szMachineID,CountArray(LogonOtherPlatform.szMachineID));
	lstrcpyn(LogonOtherPlatform.szMobilePhone,pLogonOtherPlatform->szMobilePhone,CountArray(LogonOtherPlatform.szMobilePhone));
	lstrcpyn(LogonOtherPlatform.szCompellation,pLogonOtherPlatform->szCompellation,CountArray(LogonOtherPlatform.szCompellation));

	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_LOGON_OTHERPLATFORM,dwSocketID,&LogonOtherPlatform,sizeof(LogonOtherPlatform));

	return true;
}

//游客登录
bool CAttemperEngineSink::OnTCPNetworkSubMBLogonVisitor(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize>=sizeof(CMD_MB_LogonVisitor));
	if (wDataSize<sizeof(CMD_MB_LogonVisitor))return false;

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//处理消息
	CMD_MB_LogonVisitor * pLogonVisitor=(CMD_MB_LogonVisitor *)pData;

	//设置连接
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pLogonVisitor->wModuleID;
	pBindParameter->dwPlazaVersion=pLogonVisitor->dwPlazaVersion;

	//版本判断
	if (CheckPlazaVersion(pLogonVisitor->cbDeviceType,pLogonVisitor->dwPlazaVersion,dwSocketID)==false)
	{
		return true;
	}

	//变量定义
	DBR_MB_LogonVisitor LogonVisitor;
	ZeroMemory(&LogonVisitor,sizeof(LogonVisitor));

	//附加信息
	LogonVisitor.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//构造数据
	LogonVisitor.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	LogonVisitor.cbPlatformID=ULMByVisitor;
	lstrcpyn(LogonVisitor.szAgentID,pLogonVisitor->szAgentID,CountArray(LogonVisitor.szAgentID));
	lstrcpyn(LogonVisitor.szMachineID,pLogonVisitor->szMachineID,CountArray(LogonVisitor.szMachineID));
	lstrcpyn(LogonVisitor.szMobilePhone,pLogonVisitor->szMobilePhone,CountArray(LogonVisitor.szMobilePhone));

	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_LOGON_VISITOR,dwSocketID,&LogonVisitor,sizeof(LogonVisitor));

	return true;
}

bool CAttemperEngineSink::OnTCPNetworkSubMBQueryGameLockInfo(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize>=sizeof(CMD_MB_QueryGameLockInfo));
	if (wDataSize<sizeof(CMD_MB_QueryGameLockInfo))return false;

	//处理消息
	CMD_MB_QueryGameLockInfo * pClientData=(CMD_MB_QueryGameLockInfo *)pData;

	//变量定义
	DBR_MB_QueryGameLockInfo DBRData;
	ZeroMemory(&DBRData,sizeof(DBRData));

	//构造数据
	DBRData.dwUserID = pClientData->dwUserID;

	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_QUERY_GAMELOCKINFO,dwSocketID,&DBRData,sizeof(DBRData));

	return true;
}

//获取在线人数
bool CAttemperEngineSink::OnTCPNetworkSubMBGetOnline(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//变量定义
	CMD_MB_GetOnline * pGetOnline=(CMD_MB_GetOnline *)pData;
	WORD wHeadSize=(sizeof(CMD_MB_GetOnline)-sizeof(pGetOnline->wOnLineServerID));

	//效验数据
	ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGetOnline->wServerCount*sizeof(WORD))));
	if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGetOnline->wServerCount*sizeof(WORD)))) return false;

	//变量定义
	CMD_MB_KindOnline KindOnline;
	CMD_MB_ServerOnline ServerOnline;
	ZeroMemory(&KindOnline,sizeof(KindOnline));
	ZeroMemory(&ServerOnline,sizeof(ServerOnline));

	//获取类型
	POSITION KindPosition=NULL;
	do
	{
		//获取类型
		CGameKindItem * pGameKindItem=m_ServerListManager.EmunGameKindItem(KindPosition);

		//设置变量
		if (pGameKindItem!=NULL)
		{
			WORD wKindIndex=KindOnline.wKindCount++;
			KindOnline.OnLineInfoKind[wKindIndex].wKindID=pGameKindItem->m_GameKind.wKindID;
			KindOnline.OnLineInfoKind[wKindIndex].dwOnLineCount=pGameKindItem->m_GameKind.dwOnLineCount+pGameKindItem->m_GameKind.dwAndroidCount;
		}

		//溢出判断
		if (KindOnline.wKindCount>=CountArray(KindOnline.OnLineInfoKind))
		{
			ASSERT(FALSE);
			break;
		}

	} while (KindPosition!=NULL);

	//获取房间
	for (WORD i=0;i<pGetOnline->wServerCount;i++)
	{
		//获取房间
		WORD wServerID=pGetOnline->wOnLineServerID[i];
		CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(wServerID);

		//设置变量
		if (pGameServerItem!=NULL)
		{
			WORD wServerIndex=ServerOnline.wServerCount++;
			ServerOnline.OnLineInfoServer[wServerIndex].wServerID=wServerID;
			ServerOnline.OnLineInfoServer[wServerIndex].dwOnLineCount=pGameServerItem->m_GameServer.dwOnLineCount+pGameServerItem->m_GameServer.dwAndroidCount;
		}

		//溢出判断
		if (ServerOnline.wServerCount>=CountArray(ServerOnline.OnLineInfoServer))
		{
			ASSERT(FALSE);
			break;
		}
	}

	//类型在线
	if (KindOnline.wKindCount>0)
	{
		WORD wHeadSize=sizeof(KindOnline)-sizeof(KindOnline.OnLineInfoKind);
		WORD wSendSize=wHeadSize+KindOnline.wKindCount*sizeof(KindOnline.OnLineInfoKind[0]);
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_KINE_ONLINE,&KindOnline,wSendSize);
	}

	//房间在线
	if (ServerOnline.wServerCount>0)
	{
		WORD wHeadSize=sizeof(ServerOnline)-sizeof(ServerOnline.OnLineInfoServer);
		WORD wSendSize=wHeadSize+ServerOnline.wServerCount*sizeof(ServerOnline.OnLineInfoServer[0]);
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_SERVER_ONLINE,&ServerOnline,wSendSize);
	}

	return true;
}

bool CAttemperEngineSink::OnTCPNetworkSubMBQueryServer(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_MB_QueryServer));
	if (wDataSize!=sizeof(CMD_MB_QueryServer))return false;
	CMD_MB_QueryServer * pClientData = (CMD_MB_QueryServer*)pData;
	m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_SERVER_START,0,0);
	SendMobileServerInfo(dwSocketID, pClientData->wKindID);
	m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_SERVER_END,0,0);
	return true;
}

//平台配置
bool CAttemperEngineSink::OnDBPCPlatformParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//变量定义
	DBO_GP_PlatformParameter * pPlatformParameter=(DBO_GP_PlatformParameter *)pData;

	//设置变量
	m_PlatformParameter.dwExchangeRate = pPlatformParameter->dwExchangeRate;
	m_PlatformParameter.dwPresentExchangeRate = pPlatformParameter->dwPresentExchangeRate;
	m_PlatformParameter.dwRateGold = pPlatformParameter->dwRateGold;

	return true;
}

//用户头像
bool CAttemperEngineSink::OnDBPCUserFaceInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	CMD_GP_UserFaceInfo UserFaceInfo;
	ZeroMemory(&UserFaceInfo,sizeof(UserFaceInfo));
	DBO_GP_UserFaceInfo * pUserFaceInfo=(DBO_GP_UserFaceInfo *)pData;

	//设置变量
	UserFaceInfo.bSuccess = pUserFaceInfo->bSuccess;
	UserFaceInfo.wFaceID=pUserFaceInfo->wFaceID;

	lstrcpyn(UserFaceInfo.szDescribeString, pUserFaceInfo->szDescribeString, CountArray(UserFaceInfo.szDescribeString));

	//发送消息
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_FACE_INFO,&UserFaceInfo,sizeof(UserFaceInfo));

	return true;
}

//银行信息
bool CAttemperEngineSink::OnDBPCUserInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	DBO_GP_UserInsureInfo * pUserInsureInfo=(DBO_GP_UserInsureInfo *)pData;

	//变量定义
	CMD_GP_UserInsureInfo UserInsureInfo;
	ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

	//构造数据
	UserInsureInfo.cbEnjoinTransfer=pUserInsureInfo->cbEnjoinTransfer;
	UserInsureInfo.wRevenueTake=pUserInsureInfo->wRevenueTake;
	UserInsureInfo.wRevenueTransfer=pUserInsureInfo->wRevenueTransfer;
	UserInsureInfo.wRevenueTransferMember=pUserInsureInfo->wRevenueTransferMember;
	UserInsureInfo.wServerID=pUserInsureInfo->wServerID;
	UserInsureInfo.lUserScore=pUserInsureInfo->lUserScore;
	UserInsureInfo.lUserInsure=pUserInsureInfo->lUserInsure;
	UserInsureInfo.lTransferPrerequisite=pUserInsureInfo->lTransferPrerequisite;

	//发送数据
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_INFO,&UserInsureInfo,sizeof(UserInsureInfo));

	//关闭连接
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//存钱结果
bool CAttemperEngineSink::OnDBPCUserSaveResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter + LOWORD(dwContextID))->dwSocketID != dwContextID) return true;

	//变量定义
	DBO_GP_UserSaveResult * pUserSaveResult = (DBO_GP_UserSaveResult *)pData;

	//变量定义
	CMD_GP_UserSaveResult UserSaveResult;
	ZeroMemory(&UserSaveResult, sizeof(UserSaveResult));

	//设置变量
	UserSaveResult.bSuccess = pUserSaveResult->bSuccess;
	UserSaveResult.lUserScore = pUserSaveResult->lScore;
	UserSaveResult.lUserInsure = pUserSaveResult->lInsureScore;
	lstrcpyn(UserSaveResult.szDescribeString, pUserSaveResult->szDescribeString, CountArray(UserSaveResult.szDescribeString));

	//发送消息
	WORD wDescribe = CountStringBuffer(UserSaveResult.szDescribeString);
	WORD wHeadSize = sizeof(UserSaveResult) - sizeof(UserSaveResult.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GP_USER_SAVE_RESULT, &UserSaveResult, wHeadSize + wDescribe);

	return true;
}

//取钱结果
bool CAttemperEngineSink::OnDBPCUserTakeResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter + LOWORD(dwContextID))->dwSocketID != dwContextID) return true;

	//变量定义
	DBO_GP_UserTakeResult * pUserTakeResult = (DBO_GP_UserTakeResult *)pData;

	//变量定义
	CMD_GP_UserTakeResult UserTakeResult;
	ZeroMemory(&UserTakeResult, sizeof(UserTakeResult));

	//设置变量
	UserTakeResult.bSuccess = pUserTakeResult->bSuccess;
	UserTakeResult.lUserScore = pUserTakeResult->lScore;
	UserTakeResult.lUserInsure = pUserTakeResult->lInsureScore;
	lstrcpyn(UserTakeResult.szDescribeString, pUserTakeResult->szDescribeString, CountArray(UserTakeResult.szDescribeString));

	//发送消息
	WORD wDescribe = CountStringBuffer(UserTakeResult.szDescribeString);
	WORD wHeadSize = sizeof(UserTakeResult) - sizeof(UserTakeResult.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GP_USER_TAKE_RESULT, &UserTakeResult, wHeadSize + wDescribe);

	return true;
}

//银行成功
bool CAttemperEngineSink::OnDBPCUserInsureSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	DBO_GP_UserInsureSuccess * pUserInsureSuccess=(DBO_GP_UserInsureSuccess *)pData;

	//变量定义
	CMD_GP_UserInsureSuccess UserInsureSuccess;
	ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

	//设置变量
	UserInsureSuccess.dwUserID=pUserInsureSuccess->dwUserID;
	UserInsureSuccess.lUserScore=pUserInsureSuccess->lSourceScore+pUserInsureSuccess->lVariationScore;
	UserInsureSuccess.lUserInsure=pUserInsureSuccess->lSourceInsure+pUserInsureSuccess->lVariationInsure;
	lstrcpyn(UserInsureSuccess.szDescribeString,pUserInsureSuccess->szDescribeString,CountArray(UserInsureSuccess.szDescribeString));

	//发送消息
	WORD wDescribe=CountStringBuffer(UserInsureSuccess.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureSuccess)-sizeof(UserInsureSuccess.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_SUCCESS,&UserInsureSuccess,wHeadSize+wDescribe);

	return true;
}

//操作失败
bool CAttemperEngineSink::OnDBPCUserInsureFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	CMD_GP_UserInsureFailure UserInsureFailure;
	ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

	//变量定义
	DBO_GP_UserInsureFailure * pUserInsureFailure=(DBO_GP_UserInsureFailure *)pData;

	//构造数据
	UserInsureFailure.lResultCode=pUserInsureFailure->lResultCode;
	lstrcpyn(UserInsureFailure.szDescribeString,pUserInsureFailure->szDescribeString,CountArray(UserInsureFailure.szDescribeString));

	//发送数据
	WORD wDescribe=CountStringBuffer(UserInsureFailure.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_FAILURE,&UserInsureFailure,wHeadSize+wDescribe);

	//关闭连接
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//用户信息
bool CAttemperEngineSink::OnDBPCUserInsureUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	DBO_GP_UserTransferUserInfo * pTransferUserInfo=(DBO_GP_UserTransferUserInfo *)pData;

	//变量定义
	CMD_GP_UserTransferUserInfo UserTransferUserInfo;
	ZeroMemory(&UserTransferUserInfo,sizeof(UserTransferUserInfo));

	//构造变量
	UserTransferUserInfo.dwTargetGameID=pTransferUserInfo->dwGameID;
	lstrcpyn(UserTransferUserInfo.szAccounts,pTransferUserInfo->szAccounts,CountArray(UserTransferUserInfo.szAccounts));

	//发送数据
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_USER_INFO_RESULT,&UserTransferUserInfo,sizeof(UserTransferUserInfo));

	return true;
}

//开通结果
bool CAttemperEngineSink::OnDBPCUserInsureEnableResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	DBO_GP_UserInsureEnableResult * pUserInsureEnableResult=(DBO_GP_UserInsureEnableResult *)pData;

	//变量定义
	CMD_GP_UserInsureEnableResult UserInsureEnableResult;
	ZeroMemory(&UserInsureEnableResult,sizeof(UserInsureEnableResult));

	//构造变量
	UserInsureEnableResult.cbInsureEnabled=pUserInsureEnableResult->cbInsureEnabled;
	lstrcpyn(UserInsureEnableResult.szDescribeString,pUserInsureEnableResult->szDescribeString,CountArray(UserInsureEnableResult.szDescribeString));

	//发送数据
	WORD wHeadSize=CountStringBuffer(UserInsureEnableResult.szDescribeString);
	wHeadSize +=sizeof(UserInsureEnableResult)-sizeof(UserInsureEnableResult.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_ENABLE_RESULT,&UserInsureEnableResult,wHeadSize);

	return true;
}

//低保参数
bool CAttemperEngineSink::OnDBPCBaseEnsureParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	DBO_GP_BaseEnsureParameter * pEnsureParameter=(DBO_GP_BaseEnsureParameter *)pData;

	//设置变量
	m_BaseEnsureParameter.cbTakeTimes = pEnsureParameter->cbTakeTimes;
	m_BaseEnsureParameter.lScoreAmount = pEnsureParameter->lScoreAmount;
	m_BaseEnsureParameter.lScoreCondition = pEnsureParameter->lScoreCondition;

	return true;
}

//低保结果
bool CAttemperEngineSink::OnDBPCBaseEnsureResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	DBO_GP_BaseEnsureResult * pBaseEnsureResult=(DBO_GP_BaseEnsureResult *)pData;

	//构造结构
	CMD_GP_BaseEnsureResult BaseEnsureResult;
	BaseEnsureResult.bSuccessed=pBaseEnsureResult->bSuccessed;
	BaseEnsureResult.lGameScore=pBaseEnsureResult->lGameScore;
	BaseEnsureResult.nTakeTimes = pBaseEnsureResult->nTakeTimes;
	lstrcpyn(BaseEnsureResult.szNotifyContent,pBaseEnsureResult->szNotifyContent,CountArray(BaseEnsureResult.szNotifyContent));

	//发送数据
	WORD wSendDataSize=sizeof(BaseEnsureResult)-sizeof(BaseEnsureResult.szNotifyContent);
	wSendDataSize+=CountStringBuffer(BaseEnsureResult.szNotifyContent);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_BASEENSURE_RESULT,&BaseEnsureResult,wSendDataSize);

	return true;
}

//机器操作
bool CAttemperEngineSink::OnDBAndroidParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;
	
	//变量定义
	DBO_GP_AndroidParameter * pAndroidParameter=(DBO_GP_AndroidParameter *)pData;

	//变量定义
	CMD_GP_AndroidParameter AndroidParameter;
	ZeroMemory(&AndroidParameter,sizeof(AndroidParameter));

	//构造变量
	AndroidParameter.wSubCommdID=pAndroidParameter->wSubCommdID;
	AndroidParameter.wParameterCount=pAndroidParameter->wParameterCount;
	CopyMemory(AndroidParameter.AndroidParameter,pAndroidParameter->AndroidParameter,sizeof(tagAndroidParameter)*AndroidParameter.wParameterCount);

	//计算大小
	WORD wSendDataSize = sizeof(AndroidParameter)-sizeof(AndroidParameter.AndroidParameter);
	wSendDataSize += sizeof(tagAndroidParameter)*AndroidParameter.wParameterCount;

	//发送数据
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_ANDROID_SERVICE,SUB_GP_ANDROID_PARAMETER,&AndroidParameter,wSendDataSize);

	//数量判断
	if(AndroidParameter.wParameterCount==1)
	{
		//协调房间
		if(AndroidParameter.wSubCommdID==SUB_GP_ADD_PARAMETER)
		{
			//构造结构
			CMD_CS_C_AddParameter AddParameter;
			AddParameter.wServerID=pAndroidParameter->wServerID;
			CopyMemory(&AddParameter.AndroidParameter,&AndroidParameter.AndroidParameter[0],sizeof(tagAndroidParameter));

			//发送数据
			m_pITCPSocketService->SendData(MDM_CS_ANDROID_SERVICE,SUB_CS_C_ADDPARAMETER,&AddParameter,sizeof(AddParameter));
		}

		//协调房间
		if(AndroidParameter.wSubCommdID==SUB_GP_MODIFY_PARAMETER)
		{
			//构造结构
			CMD_CS_C_ModifyParameter ModifyParameter;
			ModifyParameter.wServerID=pAndroidParameter->wServerID;
			CopyMemory(&ModifyParameter.AndroidParameter,&AndroidParameter.AndroidParameter[0],sizeof(tagAndroidParameter));

			//发送数据
			m_pITCPSocketService->SendData(MDM_CS_ANDROID_SERVICE,SUB_CS_C_MODIFYPARAMETER,&ModifyParameter,sizeof(ModifyParameter));
		}

		//协调房间
		if(AndroidParameter.wSubCommdID==SUB_GP_DELETE_PARAMETER)
		{
			//构造结构
			CMD_CS_C_DeleteParameter DeleteParameter;
			DeleteParameter.wServerID=pAndroidParameter->wServerID;
			DeleteParameter.dwBatchID=AndroidParameter.AndroidParameter[0].dwBatchID;

			//发送数据
			m_pITCPSocketService->SendData(MDM_CS_ANDROID_SERVICE,SUB_CS_C_DELETEPARAMETER,&DeleteParameter,sizeof(DeleteParameter));
		}
	}

	return true;
}

bool CAttemperEngineSink::OnDBIndividualResult( DWORD dwContextID, VOID * pData, WORD wDataSize )
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//提取数据
	DBO_GP_IndividualResult * pResult=(DBO_GP_IndividualResult *)pData;

	//构造结构
	CMD_GP_IndividuaResult currResult;
	ZeroMemory(&currResult,sizeof(currResult));

	//设置变量
	currResult.bSuccessed=pResult->bSuccessed;
	lstrcpyn(currResult.szNotifyContent,pResult->szDescribeString,CountArray(currResult.szNotifyContent));

	//发送数据

	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GP_INDIVIDUAL_RESULT, &currResult, sizeof(currResult));

	return true;
}

//兑换金币结果
bool CAttemperEngineSink::OnDBMBExchangeScoreResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter + LOWORD(dwContextID))->dwSocketID != dwContextID) return true;

	//变量定义
	CMD_GP_ExchangeScoreResult ExchangeScoreResult;
	ZeroMemory(&ExchangeScoreResult, sizeof(ExchangeScoreResult));

	//变量定义
	DBO_GP_ExchangeScoreResult * pExchangeScoreResult = (DBO_GP_ExchangeScoreResult *)pData;

	ExchangeScoreResult.bSuccess = pExchangeScoreResult->bSuccess;
	ExchangeScoreResult.lScore = pExchangeScoreResult->lScore;
	ExchangeScoreResult.lInsureScore = pExchangeScoreResult->lInsureScore;
	//构造数据
	lstrcpyn(ExchangeScoreResult.szDescribeString, pExchangeScoreResult->szDescribeString, CountArray(ExchangeScoreResult.szDescribeString));

	//发送数据
	WORD wDescribe = CountStringBuffer(ExchangeScoreResult.szDescribeString);
	WORD wHeadSize = sizeof(ExchangeScoreResult) - sizeof(ExchangeScoreResult.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GP_EXCHANGESCORE_RESULT, &ExchangeScoreResult, wHeadSize + wDescribe);

	return true;
}

//操作成功
bool CAttemperEngineSink::OnDBPCOperateSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	CMD_GP_OperateSuccess OperateSuccess;
	ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

	//变量定义
	DBO_GP_OperateSuccess * pOperateSuccess=(DBO_GP_OperateSuccess *)pData;

	//构造数据
	lstrcpyn(OperateSuccess.szDescribeString,pOperateSuccess->szDescribeString,CountArray(OperateSuccess.szDescribeString));

	//发送数据
	WORD wDescribe=CountStringBuffer(OperateSuccess.szDescribeString);
	WORD wHeadSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_OPERATE_SUCCESS,&OperateSuccess,wHeadSize+wDescribe);

	return true;
}

//操作失败
bool CAttemperEngineSink::OnDBPCOperateFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	CMD_GP_OperateFailure OperateFailure;
	ZeroMemory(&OperateFailure,sizeof(OperateFailure));

	//变量定义
	DBO_GP_OperateFailure * pOperateFailure=(DBO_GP_OperateFailure *)pData;

	//构造数据
	lstrcpyn(OperateFailure.szDescribeString,pOperateFailure->szDescribeString,CountArray(OperateFailure.szDescribeString));

	//发送数据
	WORD wDescribe=CountStringBuffer(OperateFailure.szDescribeString);
	WORD wHeadSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_OPERATE_FAILURE,&OperateFailure,wHeadSize+wDescribe);

	return true;
}

//登录成功
bool CAttemperEngineSink::OnDBMBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	DBO_MB_LogonSuccess * pDBOLogonSuccess=(DBO_MB_LogonSuccess *)pData;
	CMD_MB_LogonSuccess * pCMDLogonSuccess=(CMD_MB_LogonSuccess *)cbDataBuffer;

	//重复登录
	CServerUserItem * pServerUserItem = m_ServerUserManager.SearchUserItem(pDBOLogonSuccess->dwUserID);
	if (pServerUserItem != NULL)
	{
		m_ServerUserManager.DeleteUserItem(pDBOLogonSuccess->dwUserID);
	}

	//发送定义
	WORD wHeadSize=sizeof(CMD_MB_LogonSuccess);
	CSendPacketHelper SendPacket(cbDataBuffer+wHeadSize,sizeof(cbDataBuffer)-wHeadSize);

	//设置变量
	ZeroMemory(pCMDLogonSuccess,sizeof(CMD_MB_LogonSuccess));

	//构造数据
	pCMDLogonSuccess->wFaceID=pDBOLogonSuccess->wFaceID;
	pCMDLogonSuccess->cbGender=pDBOLogonSuccess->cbGender;
	pCMDLogonSuccess->dwCustomID=pDBOLogonSuccess->dwCustomID;
	pCMDLogonSuccess->dwGameID=pDBOLogonSuccess->dwGameID;
	pCMDLogonSuccess->dwSpreaderID = pDBOLogonSuccess->dwSpreaderID;
	pCMDLogonSuccess->dwUserID=pDBOLogonSuccess->dwUserID;
	pCMDLogonSuccess->dwExperience=pDBOLogonSuccess->dwExperience;
	lstrcpyn(pCMDLogonSuccess->szAccounts,pDBOLogonSuccess->szAccounts,CountArray(pCMDLogonSuccess->szAccounts));
	lstrcpyn(pCMDLogonSuccess->szNickName,pDBOLogonSuccess->szNickName,CountArray(pCMDLogonSuccess->szNickName));
	lstrcpyn(pCMDLogonSuccess->szAliPayAcccount, pDBOLogonSuccess->szAliPayAcccount, CountArray(pCMDLogonSuccess->szAliPayAcccount));
	lstrcpyn(pCMDLogonSuccess->szBinkID, pDBOLogonSuccess->szBinkID, CountArray(pCMDLogonSuccess->szBinkID));
	lstrcpyn(pCMDLogonSuccess->szDynamicPass,pDBOLogonSuccess->szDynamicPass,CountArray(pCMDLogonSuccess->szDynamicPass));


	//用户成绩
	pCMDLogonSuccess->lUserScore=pDBOLogonSuccess->lUserScore;
	pCMDLogonSuccess->lUserInsure=pDBOLogonSuccess->lUserInsure;

	//扩展信息
	pCMDLogonSuccess->cbInsureEnabled=pDBOLogonSuccess->cbInsureEnabled;
	pCMDLogonSuccess->cbIsAgent=pDBOLogonSuccess->cbIsAgent;
	pCMDLogonSuccess->cbMoorMachine=pDBOLogonSuccess->cbMoorMachine;
	pCMDLogonSuccess->TodayAlmsCount = pDBOLogonSuccess->TodayAlmsCount;
	pCMDLogonSuccess->dwLockServerID = pDBOLogonSuccess->dwLockServerID;
	pCMDLogonSuccess->dwKindID = pDBOLogonSuccess->dwKindID;

	//用户变量
	tagFriendUserInfo pFriendUserInfo;
	ZeroMemory(&pFriendUserInfo, sizeof(pFriendUserInfo));

	//基本资料
	pFriendUserInfo.dwUserID = pDBOLogonSuccess->dwUserID;
	pFriendUserInfo.dwGameID = pDBOLogonSuccess->dwGameID;
	lstrcpyn(pFriendUserInfo.szNickName, pDBOLogonSuccess->szNickName, CountArray(pFriendUserInfo.szNickName));
	pFriendUserInfo.cbGender = pDBOLogonSuccess->cbGender;
	pFriendUserInfo.dwFaceID = pDBOLogonSuccess->wFaceID;
	pFriendUserInfo.dwCustomID = pDBOLogonSuccess->dwCustomID;

	//用户状态
	pFriendUserInfo.cbMainStatus = FRIEND_US_ONLINE;
	pFriendUserInfo.cbGameStatus = US_NULL;
	pFriendUserInfo.wKindID = INVALID_KIND;
	pFriendUserInfo.wServerID = INVALID_SERVER;
	pFriendUserInfo.wTableID = INVALID_TABLE;
	pFriendUserInfo.wChairID = INVALID_CHAIR;
	ZeroMemory(pFriendUserInfo.szServerName, CountArray(pFriendUserInfo.szServerName));

	//变量定义
	tagInsertLocalUserInfo InsertLocalUserInfo;
	InsertLocalUserInfo.dwSocketID = dwContextID;
	InsertLocalUserInfo.dwLogonTime = GetTickCount();

	//激活用户
	m_ServerUserManager.InsertLocalUserItem(InsertLocalUserInfo, pFriendUserInfo, pDBOLogonSuccess->szDynamicPass);

	//登录成功
	WORD wSendSize=SendPacket.GetDataSize()+sizeof(CMD_MB_LogonSuccess);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_LOGON,SUB_MB_LOGON_SUCCESS,cbDataBuffer,wSendSize);


	//发送房间
	WORD wIndex=LOWORD(dwContextID);
	SendMobileKindInfo(dwContextID);
	SendMobileServerInfo(dwContextID, INVALID_KIND);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_SERVER_LIST,SUB_MB_LIST_FINISH);

	return true;
}

//登录失败
bool CAttemperEngineSink::OnDBMBLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	CMD_MB_LogonFailure LogonFailure;
	ZeroMemory(&LogonFailure,sizeof(LogonFailure));
	DBO_MB_LogonFailure * pLogonFailure=(DBO_MB_LogonFailure *)pData;

	//构造数据
	LogonFailure.lResultCode=pLogonFailure->lResultCode;
	lstrcpyn(LogonFailure.szDescribeString,pLogonFailure->szDescribeString,CountArray(LogonFailure.szDescribeString));

	//发送数据
	WORD wStringSize=CountStringBuffer(LogonFailure.szDescribeString);
	WORD wSendSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString)+wStringSize;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_LOGON,SUB_MB_LOGON_FAILURE,&LogonFailure,wSendSize);

	//关闭连接
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//抽奖配置
bool CAttemperEngineSink::OnDBPCLotteryConfig(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	DBO_GP_LotteryConfig * pLotteryConfig=(DBO_GP_LotteryConfig *)pData;

	//变量定义
	CMD_GP_LotteryConfig LotteryConfig;
	ZeroMemory(&LotteryConfig,sizeof(LotteryConfig));

	//构造变量
	LotteryConfig.wLotteryCount=pLotteryConfig->wLotteryCount;
	CopyMemory(LotteryConfig.LotteryItem,pLotteryConfig->LotteryItem,sizeof(LotteryConfig.LotteryItem));

	//发送数据
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_LOTTERY_CONFIG,&LotteryConfig,sizeof(LotteryConfig));

	return true;
}

//抽奖信息
bool CAttemperEngineSink::OnDBPCLotteryUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	DBO_GP_LotteryUserInfo * pLotteryConfig=(DBO_GP_LotteryUserInfo *)pData;

	//变量定义
	CMD_GP_LotteryUserInfo LotteryUserInfo;
	ZeroMemory(&LotteryUserInfo,sizeof(LotteryUserInfo));

	//构造变量
	LotteryUserInfo.cbFreeCount=pLotteryConfig->cbFreeCount;
	LotteryUserInfo.cbAlreadyCount=pLotteryConfig->cbAlreadyCount;
	LotteryUserInfo.wKindID=pLotteryConfig->wKindID;
	LotteryUserInfo.dwUserID=pLotteryConfig->dwUserID;
	LotteryUserInfo.lChargeFee=pLotteryConfig->lChargeFee;

	//发送数据
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_LOTTERY_USER_INFO,&LotteryUserInfo,sizeof(LotteryUserInfo));

	return true;
}

//抽奖结果
bool CAttemperEngineSink::OnDBPCLotteryResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	DBO_GP_LotteryResult * pLotteryResult=(DBO_GP_LotteryResult *)pData;

	//变量定义
	CMD_GP_LotteryResult LotteryResult;
	ZeroMemory(&LotteryResult,sizeof(LotteryResult));

	//构造变量
	LotteryResult.bWined=pLotteryResult->bWined;
	LotteryResult.wKindID=pLotteryResult->wKindID;
	LotteryResult.dwUserID=pLotteryResult->dwUserID;
	LotteryResult.lUserScore=pLotteryResult->lUserScore;
	LotteryResult.dUserBeans=pLotteryResult->dUserBeans;
	CopyMemory(&LotteryResult.LotteryItem,&pLotteryResult->LotteryItem,sizeof(LotteryResult.LotteryItem));

	//发送数据
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_LOTTERY_RESULT,&LotteryResult,sizeof(LotteryResult));

	return true;
}

//代理列表
bool CAttemperEngineSink::OnDBPCAgentGameList(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize%sizeof(tagAgentGameKind)==0);
	if (wDataSize%sizeof(tagAgentGameKind)!=0) return false;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_SERVER_LIST,SUB_GP_AGENT_KIND,pData,wDataSize);

	return true;
}

//游戏种类
bool CAttemperEngineSink::OnDBPCGameTypeItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize%sizeof(DBO_GP_GameType)==0);
	if (wDataSize%sizeof(DBO_GP_GameType)!=0) return false;

	//变量定义
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GameType);
	DBO_GP_GameType * pGameType=(DBO_GP_GameType *)pData;

	//更新数据
	for (WORD i=0;i<wItemCount;i++)
	{
		//变量定义
		tagGameType GameType;
		ZeroMemory(&GameType,sizeof(GameType));

		//构造数据
		GameType.wTypeID=(pGameType+i)->wTypeID;
		GameType.wJoinID=(pGameType+i)->wJoinID;
		GameType.wSortID=(pGameType+i)->wSortID;
		lstrcpyn(GameType.szTypeName,(pGameType+i)->szTypeName,CountArray(GameType.szTypeName));

		//插入列表
		m_ServerListManager.InsertGameType(&GameType);
	}

	return true;
}

//游戏类型
bool CAttemperEngineSink::OnDBPCGameKindItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize%sizeof(DBO_GP_GameKind)==0);
	if (wDataSize%sizeof(DBO_GP_GameKind)!=0) return false;

	//变量定义
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GameKind);
	DBO_GP_GameKind * pGameKind=(DBO_GP_GameKind *)pData;

	//更新数据
	for (WORD i=0;i<wItemCount;i++)
	{
		//变量定义
		tagGameKind GameKind;
		ZeroMemory(&GameKind,sizeof(GameKind));

		//构造数据
		GameKind.wTypeID=(pGameKind+i)->wTypeID;
		GameKind.wJoinID=(pGameKind+i)->wJoinID;
		GameKind.wSortID=(pGameKind+i)->wSortID;
		GameKind.wKindID=(pGameKind+i)->wKindID;
		GameKind.wGameID=(pGameKind+i)->wGameID;
		GameKind.wRecommend=(pGameKind+i)->wRecommend;
		GameKind.wGameFlag=(pGameKind+i)->wGameFlag;
		GameKind.dwSuportType = (pGameKind+i)->dwSuportType;
		GameKind.dwOnLineCount=m_ServerListManager.CollectOnlineInfo((pGameKind+i)->wKindID,false);
		GameKind.dwAndroidCount=m_ServerListManager.CollectOnlineInfo((pGameKind+i)->wKindID,true);
		lstrcpyn(GameKind.szKindName,(pGameKind+i)->szKindName,CountArray(GameKind.szKindName));
		lstrcpyn(GameKind.szProcessName,(pGameKind+i)->szProcessName,CountArray(GameKind.szProcessName));

		//插入列表
		m_ServerListManager.InsertGameKind(&GameKind);
	}

	return true;
}

//游戏节点
bool CAttemperEngineSink::OnDBPCGameNodeItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize%sizeof(DBO_GP_GameNode)==0);
	if (wDataSize%sizeof(DBO_GP_GameNode)!=0) return false;

	//变量定义
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GameNode);
	DBO_GP_GameNode * pGameNode=(DBO_GP_GameNode *)pData;

	//更新数据
	for (WORD i=0;i<wItemCount;i++)
	{
		//变量定义
		tagGameNode GameNode;
		ZeroMemory(&GameNode,sizeof(GameNode));

		//构造数据
		GameNode.wKindID=(pGameNode+i)->wKindID;
		GameNode.wJoinID=(pGameNode+i)->wJoinID;
		GameNode.wSortID=(pGameNode+i)->wSortID;
		GameNode.wNodeID=(pGameNode+i)->wNodeID;
		lstrcpyn(GameNode.szNodeName,(pGameNode+i)->szNodeName,CountArray(GameNode.szNodeName));

		//插入列表
		m_ServerListManager.InsertGameNode(&GameNode);
	}

	return true;
}

//游戏定制
bool CAttemperEngineSink::OnDBPCGamePageItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize%sizeof(DBO_GP_GamePage)==0);
	if (wDataSize%sizeof(DBO_GP_GamePage)!=0) return false;

	//变量定义
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GamePage);
	DBO_GP_GamePage * pGamePage=(DBO_GP_GamePage *)pData;

	//更新数据
	for (WORD i=0;i<wItemCount;i++)
	{
		//变量定义
		tagGamePage GamePage;
		ZeroMemory(&GamePage,sizeof(GamePage));

		//构造数据
		GamePage.wKindID=(pGamePage+i)->wKindID;
		GamePage.wNodeID=(pGamePage+i)->wNodeID;
		GamePage.wSortID=(pGamePage+i)->wSortID;
		GamePage.wPageID=(pGamePage+i)->wPageID;
		GamePage.wOperateType=(pGamePage+i)->wOperateType;
		lstrcpyn(GamePage.szDisplayName,(pGamePage+i)->szDisplayName,CountArray(GamePage.szDisplayName));

		//插入列表
		m_ServerListManager.InsertGamePage(&GamePage);
	}

	return true;
}

//游戏列表
bool CAttemperEngineSink::OnDBPCGameListResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(DBO_GP_GameListResult));
	if (wDataSize!=sizeof(DBO_GP_GameListResult)) return false;

	//变量定义
	DBO_GP_GameListResult * pGameListResult=(DBO_GP_GameListResult *)pData;

	//消息处理
	if (pGameListResult->cbSuccess==TRUE)
	{
		//清理列表
		m_ServerListManager.CleanKernelItem();

		//事件通知
		CP_ControlResult ControlResult;
		ControlResult.cbSuccess=ER_SUCCESS;
		SendUIControlPacket(UI_LOAD_DB_LIST_RESULT,&ControlResult,sizeof(ControlResult));

		//设置时间
		ASSERT(m_pITimerEngine!=NULL);
		m_pITimerEngine->SetTimer(IDI_LOAD_GAME_LIST,m_pInitParameter->m_wLoadListTime*1000L,1,0);
	}
	else
	{
		//构造提示
		TCHAR szDescribe[128]=TEXT("");
		_sntprintf(szDescribe,CountArray(szDescribe),TEXT("服务器列表加载失败，%ld 秒后将重新加载"),m_pInitParameter->m_wReLoadListTime);

		//提示消息
		CTraceService::TraceString(szDescribe,TraceLevel_Warning);

		//设置时间
		ASSERT(m_pITimerEngine!=NULL);
		m_pITimerEngine->SetTimer(IDI_LOAD_GAME_LIST,m_pInitParameter->m_wReLoadListTime*1000L,1,0);
	}

	return true;
}

//房间虚拟人数发送开始
bool CAttemperEngineSink::OnDBServerDummyOnLineBegin(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//释放掉旧的数据
	for (int i = 0; i < m_ServerDummyOnLineArray.GetCount(); ++i)
	{
		tagServerDummyOnLine * pCursor = m_ServerDummyOnLineArray.GetAt(i);
		free(pCursor);
	}
	m_ServerDummyOnLineArray.RemoveAll();
	m_ServerDummyOnLineMap.RemoveAll();
	return true;
}


//房间虚拟人数
bool CAttemperEngineSink::OnDBServerDummyOnLine(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize%sizeof(tagServerDummyOnLine)==0);
	if (wDataSize%sizeof(tagServerDummyOnLine)!=0) return false;

	//变量定义
	WORD wItemCount=wDataSize/sizeof(tagServerDummyOnLine);
	tagServerDummyOnLine * pServerDummyOnLine=(tagServerDummyOnLine *)pData;

	tagServerDummyOnLine * pItemCursor = NULL;


	for (WORD i=0;i<wItemCount;i++)
	{
		//变量定义
		pItemCursor=(pServerDummyOnLine+i);

		tagServerDummyOnLine *pNewDummyOnline = (tagServerDummyOnLine*)malloc(sizeof(tagServerDummyOnLine));
		CopyMemory(pNewDummyOnline, pItemCursor, sizeof(tagServerDummyOnLine));
		m_ServerDummyOnLineArray.Add(pNewDummyOnline);
		//插入数据
	}

	return true;
}

//房间虚拟人数发送结束
bool CAttemperEngineSink::OnDBServerDummyOnLineEnd(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	OnChangeDummyOnline();
	return true;
}

//版本检测
bool CAttemperEngineSink::CheckPlazaVersion(BYTE cbDeviceType, DWORD dwPlazaVersion, DWORD dwSocketID, bool bCheckLowVer)
{
	//变量定义
	bool bMustUpdate=false;
	bool bAdviceUpdate=false;
	DWORD dwVersion=VERSION_PLAZA;

	//手机版本
	if(cbDeviceType >= DEVICE_TYPE_IPAD) dwVersion=VERSION_MOBILE_IOS;
	else if(cbDeviceType >= DEVICE_TYPE_IPHONE) dwVersion=VERSION_MOBILE_IOS;
	else if(cbDeviceType >= DEVICE_TYPE_ITOUCH) dwVersion=VERSION_MOBILE_IOS;
	else if(cbDeviceType >= DEVICE_TYPE_ANDROID) dwVersion=VERSION_MOBILE_ANDROID;
	else if(cbDeviceType == DEVICE_TYPE_PC) dwVersion=VERSION_PLAZA;

	//版本判断
	if (bCheckLowVer && GetSubVer(dwPlazaVersion)<GetSubVer(dwVersion)) bAdviceUpdate=true;
	if (GetMainVer(dwPlazaVersion)!=GetMainVer(dwVersion)) bMustUpdate=true;
	if (GetProductVer(dwPlazaVersion)!=GetProductVer(dwVersion)) bMustUpdate=true;

	//升级判断
	if ((bMustUpdate==true)||(bAdviceUpdate==true))
	{
		//变量定义
		CMD_GP_UpdateNotify UpdateNotify;
		ZeroMemory(&UpdateNotify,sizeof(UpdateNotify));

		//变量定义
		UpdateNotify.cbMustUpdate=bMustUpdate;
		UpdateNotify.cbAdviceUpdate=bAdviceUpdate;
		UpdateNotify.dwCurrentVersion=dwVersion;

		//发送消息
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_LOGON,SUB_GP_UPDATE_NOTIFY,&UpdateNotify,sizeof(UpdateNotify));

		//中断判断
		if (bMustUpdate==true) 
		{
			m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);
			return false;
		}
	}

	return true;
}

//发送请求
bool CAttemperEngineSink::SendUIControlPacket(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	//发送数据
	CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
	pServiceUnits->PostControlRequest(wRequestID,pData,wDataSize);

	return true;
}

//发送类型
VOID CAttemperEngineSink::SendGameTypeInfo(DWORD dwSocketID)
{
	//网络数据
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//枚举数据
	POSITION Position=NULL;
	CGameTypeItem * pGameTypeItem=NULL;

	//枚举数据
	for (DWORD i=0;i<m_ServerListManager.GetGameTypeCount();i++)
	{
		//发送数据
		if ((wSendSize+sizeof(tagGameType))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_TYPE,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//获取数据
		pGameTypeItem=m_ServerListManager.EmunGameTypeItem(Position);
		if (pGameTypeItem==NULL) break;

		//拷贝数据
		CopyMemory(cbDataBuffer+wSendSize,&pGameTypeItem->m_GameType,sizeof(tagGameType));
		wSendSize+=sizeof(tagGameType);
	}

	//发送剩余
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_TYPE,cbDataBuffer,wSendSize);

	return;
}

//发送种类
VOID CAttemperEngineSink::SendGameKindInfo(DWORD dwSocketID)
{
	//网络数据
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//枚举数据
	POSITION Position=NULL;
	CGameKindItem * pGameKindItem=NULL;

	//枚举数据
	for (DWORD i=0;i<m_ServerListManager.GetGameKindCount();i++)
	{
		//发送数据
		if ((wSendSize+sizeof(tagGameKind))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_KIND,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//获取数据
		pGameKindItem=m_ServerListManager.EmunGameKindItem(Position);
		if (pGameKindItem==NULL) break;

		//拷贝数据
		CopyMemory(cbDataBuffer+wSendSize,&pGameKindItem->m_GameKind,sizeof(tagGameKind));
		wSendSize+=sizeof(tagGameKind);
	}

	//发送剩余
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_KIND,cbDataBuffer,wSendSize);

	return;
}

//发送节点
VOID CAttemperEngineSink::SendGameNodeInfo(DWORD dwSocketID, WORD wKindID)
{
	//网络数据
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//枚举数据
	POSITION Position=NULL;
	CGameNodeItem * pGameNodeItem=NULL;

	//枚举数据
	for (DWORD i=0;i<m_ServerListManager.GetGameNodeCount();i++)
	{
		//发送数据
		if ((wSendSize+sizeof(tagGameNode))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_NODE,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//获取数据
		pGameNodeItem=m_ServerListManager.EmunGameNodeItem(Position);
		if (pGameNodeItem==NULL) break;

		//拷贝数据
		if ((wKindID==INVALID_WORD)||(pGameNodeItem->m_GameNode.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameNodeItem->m_GameNode,sizeof(tagGameNode));
			wSendSize+=sizeof(tagGameNode);
		}
	}

	//发送剩余
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_NODE,cbDataBuffer,wSendSize);

	return;
}

//发送定制
VOID CAttemperEngineSink::SendGamePageInfo(DWORD dwSocketID, WORD wKindID)
{
	//网络数据
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//枚举数据
	POSITION Position=NULL;
	CGamePageItem * pGamePageItem=NULL;

	//枚举数据
	for (DWORD i=0;i<m_ServerListManager.GetGamePageCount();i++)
	{
		//发送数据
		if ((wSendSize+sizeof(tagGamePage))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_PAGE,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//获取数据
		pGamePageItem=m_ServerListManager.EmunGamePageItem(Position);
		if (pGamePageItem==NULL) break;

		//拷贝数据
		if ((wKindID==INVALID_WORD)||(pGamePageItem->m_GamePage.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGamePageItem->m_GamePage,sizeof(tagGamePage));
			wSendSize+=sizeof(tagGamePage);
		}
	}

	//发送剩余
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_PAGE,cbDataBuffer,wSendSize);

	return;
}

//发送房间
VOID CAttemperEngineSink::SendGameServerInfo(DWORD dwSocketID, WORD wKindID, BYTE cbDeviceType)
{
	//网络数据
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//枚举数据
	POSITION Position=NULL;
	CGameServerItem * pGameServerItem=NULL;

	//枚举数据
	for (DWORD i=0;i<m_ServerListManager.GetGameServerCount();i++)
	{
		//发送数据
		if ((wSendSize+sizeof(tagGameServer))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_SERVER,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//获取数据
		pGameServerItem=m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;
		//支持类型
		bool bSuportMobile = CServerRule::IsSuportMobile(pGameServerItem->m_GameServer.dwServerRule);
		bool bSurportPC = CServerRule::IsSuportPC(pGameServerItem->m_GameServer.dwServerRule);

		if(cbDeviceType == DEVICE_TYPE_PC)
		{
			if(bSurportPC == false) continue; 
		}
		else if(cbDeviceType != DEVICE_TYPE_PC)
		{
			if(bSuportMobile == false) continue;
		}

		//拷贝数据
		if ((wKindID==INVALID_WORD)||(pGameServerItem->m_GameServer.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameServerItem->m_GameServer,sizeof(tagGameServer));
			wSendSize+=sizeof(tagGameServer);
		}
	}

	//发送剩余
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_SERVER,cbDataBuffer,wSendSize);
	//设置变量
	wSendSize=0;
	ZeroMemory(cbDataBuffer,sizeof(cbDataBuffer));

	//枚举数据
	for (DWORD i=0;i<m_ServerListManager.GetGameServerCount();i++)
	{
		//发送数据
		if ((wSendSize+sizeof(tagGameMatch))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_MATCH,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//获取数据
		pGameServerItem=m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;
		if (pGameServerItem->IsMatchServer()==false) continue;

		//拷贝数据
		if ((wKindID==INVALID_WORD)||(pGameServerItem->m_GameServer.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameServerItem->m_GameMatch,sizeof(tagGameMatch));
			wSendSize+=sizeof(tagGameMatch);
		}
	}

	//发送剩余
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_MATCH,cbDataBuffer,wSendSize);

	if(m_wAVServerPort!=0 && m_dwAVServerAddr!=0)
	{
		//变量定义
		tagAVServerOption AVServerOption;
		AVServerOption.wAVServerPort=m_wAVServerPort;
		AVServerOption.dwAVServerAddr=m_dwAVServerAddr;

		//发送配置
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_VIDEO_OPTION,&AVServerOption,sizeof(AVServerOption));
	};

	return;
}

//发送类型
VOID CAttemperEngineSink::SendMobileKindInfo(DWORD dwSocketID)
{
	//网络数据
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//枚举数据
	POSITION Position=NULL;
	CGameKindItem * pGameKindItem=NULL;
	//枚举数据
	for (DWORD i=0;i<m_ServerListManager.GetGameKindCount();i++)
	{
		//发送数据
		if ((wSendSize+sizeof(tagGameKind))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_KIND,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//获取数据
		pGameKindItem=m_ServerListManager.EmunGameKindItem(Position);
		if (pGameKindItem==NULL) break;

		CopyMemory(cbDataBuffer+wSendSize,&pGameKindItem->m_GameKind,sizeof(tagGameKind));
		wSendSize+=sizeof(tagGameKind);
	}

	//发送剩余
	if (wSendSize>0) 
	{
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_KIND,cbDataBuffer,wSendSize);
	}

	return;
}

//发送房间
VOID CAttemperEngineSink::SendMobileServerInfo(DWORD dwSocketID, WORD wKindID)
{
	//网络数据
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//枚举数据
	POSITION Position=NULL;
	CGameServerItem * pGameServerItem=NULL;
	CGameKindItem * pGameKindItem=NULL;
	//枚举数据
	for (DWORD i=0;i<m_ServerListManager.GetGameServerCount();i++)
	{
		//发送数据
		if ((wSendSize+sizeof(tagGameServer))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_SERVER,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//获取数据
		pGameServerItem=m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;



		//支持类型
		bool bServerSuportMobile = CServerRule::IsSuportMobile(pGameServerItem->m_GameServer.dwServerRule);

		//拷贝数据
		if ((bServerSuportMobile==true)&&((wKindID==INVALID_WORD)||(pGameServerItem->m_GameServer.wKindID==wKindID)))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameServerItem->m_GameServer,sizeof(tagGameServer));
			wSendSize+=sizeof(tagGameServer);
		}
	}

	//发送剩余
	if (wSendSize>0) 
	{
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_SERVER,cbDataBuffer,wSendSize);
	}

	//设置变量
	wSendSize=0;
	ZeroMemory(cbDataBuffer,sizeof(cbDataBuffer));

	//枚举数据
	for (DWORD i=0;i<m_ServerListManager.GetGameServerCount();i++)
	{
		//发送数据
		if ((wSendSize+sizeof(tagGameMatch))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_MATCH,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//获取数据
		pGameServerItem=m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;
		if (pGameServerItem->IsMatchServer()==false) continue;

		//拷贝数据
		if ((wKindID==INVALID_WORD)||(pGameServerItem->m_GameServer.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameServerItem->m_GameMatch,sizeof(tagGameMatch));
			wSendSize+=sizeof(tagGameMatch);
		}
	}

	//发送剩余
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_MATCH,cbDataBuffer,wSendSize);

	return;
}

//银行失败
bool CAttemperEngineSink::SendInsureFailure(DWORD dwSocketID, LONG lResultCode, LPCTSTR pszDescribe)
{
	//变量定义
	CMD_GP_UserInsureFailure UserInsureFailure;
	ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

	//构造数据
	UserInsureFailure.lResultCode=lResultCode;
	lstrcpyn(UserInsureFailure.szDescribeString,pszDescribe,CountArray(UserInsureFailure.szDescribeString));

	//发送数据
	WORD wDescribe=CountStringBuffer(UserInsureFailure.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_FAILURE,&UserInsureFailure,wHeadSize+wDescribe);

	//关闭连接
	m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);

	return true;
}

//操作成功
VOID CAttemperEngineSink::SendOperateSuccess(DWORD dwContextID, LONG lResultCode, LPCTSTR pszDescribe)
{
	//效验参数
	ASSERT(pszDescribe != NULL);
	if(pszDescribe == NULL) return;

	//变量定义
	CMD_GP_OperateSuccess OperateSuccess;
	ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

	//构造数据
	lstrcpyn(OperateSuccess.szDescribeString,pszDescribe,CountArray(OperateSuccess.szDescribeString));

	//发送数据
	WORD wDescribe=CountStringBuffer(OperateSuccess.szDescribeString);
	WORD wHeadSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_OPERATE_SUCCESS,&OperateSuccess,wHeadSize+wDescribe);

	//关闭连接
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return;
}

//操作失败
VOID CAttemperEngineSink::SendOperateFailure(DWORD dwContextID, LONG lResultCode, LPCTSTR pszDescribe)
{
	//效验参数
	ASSERT(pszDescribe != NULL);
	if(pszDescribe == NULL) return;

	//变量定义
	CMD_GP_OperateFailure OperateFailure;
	ZeroMemory(&OperateFailure,sizeof(OperateFailure));

	//构造数据
	lstrcpyn(OperateFailure.szDescribeString,pszDescribe,CountArray(OperateFailure.szDescribeString));

	//发送数据
	WORD wDescribe=CountStringBuffer(OperateFailure.szDescribeString);
	WORD wHeadSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_OPERATE_FAILURE,&OperateFailure,wHeadSize+wDescribe);

	//关闭连接
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return;
}

VOID CAttemperEngineSink::OnChangeDummyOnline()
{
	////清除数据
	tagServerDummyOnLine * pItemCursor = NULL;


	SYSTEMTIME tempDate;
	GetSystemTime(&tempDate);

	//赋值的是本地时间
	tempDate.wYear = 1970;
	tempDate.wMonth = 1;
	tempDate.wDayOfWeek = 1;
	tempDate.wDay = 1;
	tempDate.wMinute;
	tempDate.wHour;

	time_t curTime = SystemTimeToTimet(tempDate);

	//更新数据
	srand(time(NULL));
	//iMaxAccCount 数值越大，波动幅度越小, iRetifyCount 越大，波动幅度越大 iMaxAccCount > iRetifyCount
	int iMaxAccCount = 5;
	int iRetifyCount = 3;
	//printf("----------------------start update dummy onLine\n");

	for (int i=0;i<m_ServerDummyOnLineArray.GetCount();i++)
	{
		//变量定义
		pItemCursor=m_ServerDummyOnLineArray.GetAt(i);

		//初始化值
		pItemCursor->dwCurDummyOnline = 0;

		//时间间隔
		int timeSpecStart = (curTime - pItemCursor->lTimeStart);
		int timeSpecEnd = (curTime - pItemCursor->lTimeEnd);

		if (timeSpecStart > 0 && timeSpecEnd < 0)
		{//在指定时间段使用变化值
			if (pItemCursor->dwDummyMinOnLine > pItemCursor->dwDummyMaxOnLine || (pItemCursor->dwDummyMaxOnLine - pItemCursor->dwDummyMinOnLine == 0))
			{
				if (pItemCursor->dwDummyMaxOnLine != 0)
				{
					pItemCursor->dwCurDummyOnline = pItemCursor->dwDummyMaxOnLine;
				}
				else
				{
					pItemCursor->dwCurDummyOnline = 0;
				}
			}
			else
			{
				//n+1阶的FIR滤波器 数据平滑处理
				tagServerDummyOnLine * pItemLast = m_ServerDummyOnLineMap[pItemCursor->wServerID];
				if (pItemLast == NULL)
				{
					m_ServerDummyOnLineMap[pItemCursor->wServerID] = pItemCursor;
					//printf("第一次获取，添加Map\n");
				}
				else
				{
					if (pItemLast != pItemCursor)
					{
						//获取上次的累积量，跟累计数
						pItemCursor->lAccumulationCount = pItemLast->lAccumulationCount;
						pItemCursor->lAccumulation = pItemLast->lAccumulation;
						m_ServerDummyOnLineMap[pItemCursor->wServerID] = pItemCursor;
					}
				}

				//printf(":ServerID：%d, 当前时间段人数范围:%d-%d\n", pItemCursor->wServerID, pItemCursor->dwDummyMinOnLine, pItemCursor->dwDummyMaxOnLine);
				int nRand = pItemCursor->dwDummyMinOnLine + rand() % (pItemCursor->dwDummyMaxOnLine - pItemCursor->dwDummyMinOnLine);
				//printf(":ServerID：%d, 当前时间段随机数:%d\n", pItemCursor->wServerID, nRand);

				//printf(":ServerID：%d, 当前时间段累积量前:%I64d，累计数数:%I64d\n", pItemCursor->wServerID, pItemCursor->lAccumulation, pItemCursor->lAccumulationCount);
				pItemCursor->lAccumulationCount = m_ServerDummyOnLineMap[pItemCursor->wServerID]->lAccumulationCount + 1;
				pItemCursor->lAccumulation = m_ServerDummyOnLineMap[pItemCursor->wServerID]->lAccumulation + nRand;
				//printf(":ServerID：%d, 当前时间段累积量后:%I64d，累计数数:%I64d\n", pItemCursor->wServerID, pItemCursor->lAccumulation, pItemCursor->lAccumulationCount);
				//平均波动值
				int iAve = pItemCursor->lAccumulation / pItemCursor->lAccumulationCount;
				pItemCursor->dwCurDummyOnline = iAve;
				//printf(":ServerID：%d, 当前时间段均值:%d\n", pItemCursor->wServerID, iAve);
			}

			if (pItemCursor->lAccumulationCount >= iMaxAccCount)
			{
				pItemCursor->lAccumulation -= pItemCursor->dwCurDummyOnline * iRetifyCount;
				pItemCursor->lAccumulationCount -= iRetifyCount;
				//printf(":ServerID：%d, 达到最大累计数，调整累计数:%I64d,累积量:%I64d\n", pItemCursor->wServerID, pItemCursor->lAccumulation, pItemCursor->lAccumulationCount);
			}
		}
		//插入数据
	}


	//往协调同步虚拟人数数据
	WORD wPacketSize=0L;
	POSITION Position=m_ServerDummyOnLineMap.GetStartPosition();
	BYTE cbBuffer[SOCKET_TCP_PACKET];

	//发送信息
	m_pITCPSocketService->SendData(MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_DUMMY_INFO_START);

	//收集数据
	while (Position!=NULL)
	{
		//发送数据
		if ((wPacketSize+sizeof(CMD_CS_C_DummyOnLine))>sizeof(cbBuffer))
		{
			m_pITCPSocketService->SendData(MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_DUMMY_INFO,cbBuffer,wPacketSize);
			wPacketSize=0;
		}

		//获取对象
		CMD_CS_C_DummyOnLine * pDummyOnLine=(CMD_CS_C_DummyOnLine *)(cbBuffer+wPacketSize);


		tagServerDummyOnLine * pDummyOnline = NULL;
		WORD wServerID;
		//获取对象
		m_ServerDummyOnLineMap.GetNextAssoc(Position,wServerID,pDummyOnline);

		//设置数据
		pDummyOnLine->wServerID = wServerID;
		pDummyOnLine->dwDummyCount = pDummyOnline->dwCurDummyOnline;

		//累计数据
		wPacketSize+=sizeof(CMD_CS_C_DummyOnLine);
	};

	//发送数据
	if (wPacketSize>0)
		m_pITCPSocketService->SendData(MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_DUMMY_INFO,cbBuffer,wPacketSize);

	m_pITCPSocketService->SendData(MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_DUMMY_INFO_END);
}
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
