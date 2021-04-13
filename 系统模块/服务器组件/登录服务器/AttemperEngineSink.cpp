#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include "AttemperEngineSink.h"
#include "MiniDumper.h"


/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//ʱ���ʶ
#define IDI_LOAD_GAME_LIST			1									//�����б�
#define IDI_CONNECT_CORRESPOND		2									//������ʶ
#define IDI_COLLECT_ONLINE_INFO		3									//ͳ������

//ʱ�䶨��
#define	TIME_VALID_VERIFY_CODE		1 * 60								//ע��������֤��ʱ��������λ �룩

//ͳ����������ʱ����
#define	TIME_COLLECT_ONLINE_INFO	20									//������

#define IDI_HTTP_EVENT_LOOP		4									//HTTP�¼��ַ�
#define TIME_HTTP_EVENT_LOOP	1									//ʱ����



#define VERIFY_CODE_TYPE_REGISTER	1								//ע�����
#define VERIFY_CODE_TYPE_MODIFYPASS	2								//�޸ĵ�¼�������
#define VERIFY_CODE_TYPE_MODIFYBANKINFO 3							//�޸�������Ϣ����

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

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//���캯��
CAttemperEngineSink::CAttemperEngineSink()
{
	g_pGFrame = new MiniDumper(true);
	//״̬����
	m_bNeekCorrespond=true;
	m_bShowServerStatus=false;

	//״̬����
	m_pInitParameter=NULL;
	m_pBindParameter=NULL;

	//�������
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketService=NULL;
	m_pHttpClientService = NULL;

	//��Ƶ����
	m_wAVServerPort=0;
	m_dwAVServerAddr=0;

	//�������

	ZeroMemory(&m_PlatformParameter,sizeof(&m_PlatformParameter));
	ZeroMemory(&m_BaseEnsureParameter,sizeof(&m_BaseEnsureParameter));	
	ZeroMemory(m_szHttpRegisterValidCode, sizeof(m_szHttpRegisterValidCode));
	ZeroMemory(m_szHttpModifyPassValidCode, sizeof(m_szHttpModifyPassValidCode));
//��������
	m_VerifyCodeArray.RemoveAll();

	return;
}

//��������
CAttemperEngineSink::~CAttemperEngineSink()
{
	//������Դ
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

//�ӿڲ�ѯ
VOID * CAttemperEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAttemperEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAttemperEngineSink,Guid,dwQueryVer);
	return NULL;
}

//�����¼�
bool CAttemperEngineSink::OnAttemperEngineStart(IUnknownEx * pIUnknownEx)
{
	//�󶨲���
	m_pBindParameter=new tagBindParameter[m_pInitParameter->m_wMaxConnect];
	ZeroMemory(m_pBindParameter,sizeof(tagBindParameter)*m_pInitParameter->m_wMaxConnect);

	//����ʱ��
	ASSERT(m_pITimerEngine!=NULL);
#ifndef _DEBUG
	m_pITimerEngine->SetTimer(IDI_COLLECT_ONLINE_INFO,TIME_COLLECT_ONLINE_INFO*1000L,TIMES_INFINITY,0);
#else
	m_pITimerEngine->SetTimer(IDI_COLLECT_ONLINE_INFO,TIME_COLLECT_ONLINE_INFO*1000L,TIMES_INFINITY,0);
#endif

	m_pHttpClientService = new CHttpKernel<CAttemperEngineSink>();

	if (m_pHttpClientService->InitClient() == false)
	{
		CTraceService::TraceString(_T("HTTP�ͻ�������ʧ��"),TraceLevel_Warning);
	}
	else
	{
		m_pITimerEngine->SetTimer(IDI_HTTP_EVENT_LOOP,TIME_HTTP_EVENT_LOOP,TIMES_INFINITY,0);
	}

	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");

	//��ȡĿ¼
	TCHAR szServerAddr[LEN_SERVER]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);

	//��ȡ����
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
	//freopen("CONIN$", "r+t", stdin); // �ض��� STDIN
	//freopen("CONOUT$", "w+t", stdout); // �ض���STDOUT 

	return true;
}

//ֹͣ�¼�
bool CAttemperEngineSink::OnAttemperEngineConclude(IUnknownEx * pIUnknownEx)
{
	//״̬����
	m_bNeekCorrespond=true;

	//�������
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketService=NULL;

	if (m_pHttpClientService)
		SafeDelete(m_pHttpClientService);

	//ɾ������
	SafeDeleteArray(m_pBindParameter);

	//�б����
	m_ServerListManager.ResetServerList();

	//�ͷŵ��ɵ�����
	for (int i = 0; i < m_ServerDummyOnLineArray.GetCount(); ++i)
	{
		tagServerDummyOnLine * pCursor = m_ServerDummyOnLineArray.GetAt(i);
		free(pCursor);
	}
	m_ServerDummyOnLineArray.RemoveAll();
	m_ServerDummyOnLineMap.RemoveAll();

	//FreeConsole();

	//ɾ���û�
	m_ServerUserManager.DeleteUserItem();

	return true;
}

////POST����ص�
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
//			//����������վ������
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
					//����������վ��post������
					Json::Reader reader;
					Json::Value root;

					if (reader.parse(out, root))  // json����
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
							//����json ��������
							for (Json::Value::Members::iterator iter = mem.begin(); iter != mem.end(); iter++)        
							{
								const char * pkey = (*iter).c_str();
								if(strcmp(pkey,"code")==0)
								{
									//����ʱ��
									pNewVerifyCode->dwUpdateTime=(DWORD)time(NULL);
									//������֤��
									std::string validcode = content[pkey].asString();
									TCHAR szValidCode[MAX_PATH] = TEXT("");
									MultiByteToWideChar(CP_ACP, 0, validcode.c_str(), -1, szValidCode, CountArray(szValidCode));
									lstrcpyn(pNewVerifyCode->szValidateCode,szValidCode,CountArray(pNewVerifyCode->szValidateCode));
									//��ӵ���֤������
									m_VerifyCodeArray.Add(pNewVerifyCode);

									//֪ͨ�ͻ�����֤�����ɳɹ�
									CMD_GP_VerifyCodeResult VerifyCodeResult;
									ZeroMemory(&VerifyCodeResult,sizeof(VerifyCodeResult));

									VerifyCodeResult.cbResultCode=0;
									lstrcpy(VerifyCodeResult.szDescString,TEXT("���ͳɹ���"));

									//֪ͨ�ͻ�����Ϣ ��֤�볬ʱ
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
							lstrcpy(VerifyCodeResult.szDescString,TEXT("��Ǹ����֤���ȡ�쳣�����Ժ�����C301��"));

							//֪ͨ�ͻ�����Ϣ ��֤�볬ʱ
							m_pITCPNetworkEngine->SendData(pNewVerifyCode->dwSocketID,MDM_GP_LOGON,SUB_GP_VERIFY_CODE_RESULT,&VerifyCodeResult,sizeof(VerifyCodeResult));
						}
					}
					else
					{
						CMD_GP_VerifyCodeResult VerifyCodeResult;
						ZeroMemory(&VerifyCodeResult,sizeof(VerifyCodeResult));

						VerifyCodeResult.cbResultCode=2;
						lstrcpy(VerifyCodeResult.szDescString,TEXT("��Ǹ����֤���ȡ�쳣�����Ժ�����C302��"));

						//֪ͨ�ͻ�����Ϣ ��֤�볬ʱ
						m_pITCPNetworkEngine->SendData(pNewVerifyCode->dwSocketID,MDM_GP_LOGON,SUB_GP_VERIFY_CODE_RESULT,&VerifyCodeResult,sizeof(VerifyCodeResult));
					}
				}
				else
				{//��ȡ��֤���ڲ�����, ֪ͨ�ͻ���
					CMD_GP_VerifyCodeResult VerifyCodeResult;
					ZeroMemory(&VerifyCodeResult,sizeof(VerifyCodeResult));

					VerifyCodeResult.cbResultCode=2;
					lstrcpy(VerifyCodeResult.szDescString,TEXT("��Ǹ����ȡ��֤���ڲ��������Ժ�����C303��"));

					//֪ͨ�ͻ�����Ϣ ��֤�볬ʱ
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
			//HTTP�����ض���
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
		lstrcpy(VerifyCodeResult.szDescString,TEXT("��Ǹ����ȡ��֤���ڲ��������Ժ�����C304��"));

		//֪ͨ�ͻ�����Ϣ ��֤�볬ʱ
		m_pITCPNetworkEngine->SendData(pNewVerifyCode->dwSocketID,MDM_GP_LOGON,SUB_GP_VERIFY_CODE_RESULT,&VerifyCodeResult,sizeof(VerifyCodeResult));

		delete pNewVerifyCode;
	}
}


//GET����ص�
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
					//����������վ��post������
					Json::Reader reader;
					Json::Value root;

					if (reader.parse(out, root))  // json����
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
							//����json ��������
							for (Json::Value::Members::iterator iter = mem.begin(); iter != mem.end(); iter++)        
							{
								const char * pkey = (*iter).c_str();
								if(strcmp(pkey,"code")==0)
								{
									std::string validcode = content[pkey].asString();
									TCHAR szValidCode[MAX_PATH] = TEXT("");
									MultiByteToWideChar(CP_ACP, 0, validcode.c_str(), -1, szValidCode, CountArray(szValidCode));
									//����������վ������

									//������֤��ʱ��
									pVerifyCode->dwUpdateTime=(DWORD)time(NULL);
									//������֤��
									lstrcpyn(pVerifyCode->szValidateCode,szValidCode,CountArray(pVerifyCode->szValidateCode));

									//֪ͨ�ͻ�����֤�����ɳɹ�
									CMD_GP_VerifyCodeResult VerifyCodeResult;
									ZeroMemory(&VerifyCodeResult,sizeof(VerifyCodeResult));

									VerifyCodeResult.cbResultCode=0;
									lstrcpy(VerifyCodeResult.szDescString,TEXT("���ͳɹ���"));

									//֪ͨ�ͻ�����Ϣ ��֤�볬ʱ
									m_pITCPNetworkEngine->SendData(pVerifyCode->dwSocketID,MDM_GP_LOGON,SUB_GP_VERIFY_CODE_RESULT,&VerifyCodeResult,sizeof(VerifyCodeResult));
								}
								//else
								//{//��֤���ȡ�쳣 ֪ͨ�ͻ���
								//	CMD_GP_VerifyCodeResult VerifyCodeResult;
								//	ZeroMemory(&VerifyCodeResult,sizeof(VerifyCodeResult));

								//	VerifyCodeResult.cbResultCode=2;
								//	lstrcpy(VerifyCodeResult.szDescString,TEXT("��Ǹ����֤���ȡ�쳣�����Ժ����ԣ�"));

								//	//֪ͨ�ͻ�����Ϣ ��֤�볬ʱ
								//	m_pITCPNetworkEngine->SendData(pVerifyCode->dwSocketID,MDM_GP_LOGON,SUB_GP_VERIFY_CODE_RESULT,&VerifyCodeResult,sizeof(VerifyCodeResult));
								//}
							}

						}
						else
						{
							CMD_GP_VerifyCodeResult VerifyCodeResult;
							ZeroMemory(&VerifyCodeResult,sizeof(VerifyCodeResult));

							VerifyCodeResult.cbResultCode=2;
							lstrcpy(VerifyCodeResult.szDescString,TEXT("��Ǹ����֤���ȡ�쳣�����Ժ����ԣ�"));

							//֪ͨ�ͻ�����Ϣ ��֤�볬ʱ
							m_pITCPNetworkEngine->SendData(pVerifyCode->dwSocketID,MDM_GP_LOGON,SUB_GP_VERIFY_CODE_RESULT,&VerifyCodeResult,sizeof(VerifyCodeResult));
						}
					}
					else
					{//��ȡ��֤���ڲ�����, ֪ͨ�ͻ���
						CMD_GP_VerifyCodeResult VerifyCodeResult;
						ZeroMemory(&VerifyCodeResult,sizeof(VerifyCodeResult));

						VerifyCodeResult.cbResultCode=2;
						lstrcpy(VerifyCodeResult.szDescString,TEXT("��Ǹ����ȡ��֤���ڲ��������Ժ����ԣ�"));

						//֪ͨ�ͻ�����Ϣ ��֤�볬ʱ
						m_pITCPNetworkEngine->SendData(pVerifyCode->dwSocketID,MDM_GP_LOGON,SUB_GP_VERIFY_CODE_RESULT,&VerifyCodeResult,sizeof(VerifyCodeResult));
						break;
					}
				}
				else
				{//��ȡ��֤���ڲ�����, ֪ͨ�ͻ���
					CMD_GP_VerifyCodeResult VerifyCodeResult;
					ZeroMemory(&VerifyCodeResult,sizeof(VerifyCodeResult));

					VerifyCodeResult.cbResultCode=2;
					lstrcpy(VerifyCodeResult.szDescString,TEXT("��Ǹ����ȡ��֤���ڲ��������Ժ����ԣ�"));

					//֪ͨ�ͻ�����Ϣ ��֤�볬ʱ
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
			//HTTP�����ض���
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

//�����¼�
bool CAttemperEngineSink::OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize)
{
	switch (wIdentifier)
	{
	case CT_LOAD_DB_GAME_LIST:		//�����б�
		{
			//�����б�
			m_ServerListManager.DisuseKernelItem();
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_GAME_LIST,0,NULL,0);

			//ƽ̨����
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_PLATFORM_PARAMETER,0,NULL,0);			

			//���صͱ�
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_BASEENSURE,0,NULL,0);

			//��ѯ������������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_SERVER_DUMMY_ONLINE,0,NULL,0);

			return true;
		}
	case CT_CONNECT_CORRESPOND:		//����Э��
		{
			//��������
			tagAddressInfo * pCorrespondAddress=&m_pInitParameter->m_CorrespondAddress;
			m_pITCPSocketService->Connect(pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//������ʾ
			TCHAR szString[512]=TEXT("");
			_sntprintf(szString,CountArray(szString),TEXT("��������Э�������� [ %s:%d ]"),pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//��ʾ��Ϣ
			CTraceService::TraceString(szString,TraceLevel_Normal);

			return true;
		}
	}

	return false;
}

//�����¼�
bool CAttemperEngineSink::OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//ʱ���¼�
bool CAttemperEngineSink::OnEventTimer(DWORD dwTimerID, WPARAM wBindParam)
{
	switch (dwTimerID)
	{
	case IDI_LOAD_GAME_LIST:		//�����б�
		{
			//�����б�
			m_ServerListManager.DisuseKernelItem();
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_GAME_LIST,0,NULL,0);

			return true;
		}
	case IDI_CONNECT_CORRESPOND:	//����Э��
		{
			//��������
			tagAddressInfo * pCorrespondAddress=&m_pInitParameter->m_CorrespondAddress;
			m_pITCPSocketService->Connect(pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//������ʾ
			TCHAR szString[512]=TEXT("");
			_sntprintf(szString,CountArray(szString),TEXT("��������Э�������� [ %s:%d ]"),pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//��ʾ��Ϣ
			CTraceService::TraceString(szString,TraceLevel_Normal);

			return true;
		}
	case IDI_COLLECT_ONLINE_INFO:	//ͳ������
		{
			//��������
			DBR_GP_OnLineCountInfo OnLineCountInfo;
			ZeroMemory(&OnLineCountInfo,sizeof(OnLineCountInfo));

			//�����������
			OnChangeDummyOnline();

			//��ȡ����
			OnLineCountInfo.dwOnLineCountSum=m_ServerListManager.CollectOnlineInfo(false);
			OnLineCountInfo.dwAndroidCountSum=m_ServerListManager.CollectOnlineInfo(true);

			//��ȡ����
			POSITION KindPosition=NULL;
			do
			{
				//��ȡ����
				CGameKindItem * pGameKindItem=m_ServerListManager.EmunGameKindItem(KindPosition);

				//���ñ���
				if (pGameKindItem!=NULL)
				{
					WORD wKindIndex=OnLineCountInfo.wKindCount++;
					OnLineCountInfo.OnLineCountKind[wKindIndex].wKindID=pGameKindItem->m_GameKind.wKindID;

					//Ŀ¼����
					OnLineCountInfo.OnLineCountKind[wKindIndex].dwOnLineCount=pGameKindItem->m_GameKind.dwOnLineCount;
					OnLineCountInfo.OnLineCountKind[wKindIndex].dwAndroidCount=pGameKindItem->m_GameKind.dwAndroidCount;
				}

				//����ж�
				if (OnLineCountInfo.wKindCount>=CountArray(OnLineCountInfo.OnLineCountKind))
				{
					ASSERT(FALSE);
					break;
				}

			} while (KindPosition!=NULL);

			//��������
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

//Ӧ���¼�
bool CAttemperEngineSink::OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID)
{
	//��ȡ����
	ASSERT(LOWORD(dwSocketID)<m_pInitParameter->m_wMaxConnect);
	if (LOWORD(dwSocketID)>=m_pInitParameter->m_wMaxConnect) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//���ñ���
	pBindParameter->dwSocketID=dwSocketID;
	pBindParameter->dwClientAddr=dwClientAddr;
	pBindParameter->dwActiveTime=(DWORD)time(NULL);

	return true;
}

//�ر��¼�
bool CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID)
{
	//�����Ϣ
	WORD wBindIndex=LOWORD(dwSocketID);
	ZeroMemory((m_pBindParameter+wBindIndex),sizeof(tagBindParameter));

	return false;
}

//��ȡ�¼�
bool CAttemperEngineSink::OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (Command.wMainCmdID)
	{
	case MDM_GP_SERVER_LIST:	//�б�����
		{
			return OnTCPNetworkMainPCServerList(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_USER_SERVICE:	//��������
		{
			return OnTCPNetworkMainPCUserService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_REMOTE_SERVICE:	//Զ�̷���
		{
			return OnTCPNetworkMainPCRemoteService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_ANDROID_SERVICE: //��������
		{
			return OnTCPNetworkMainAndroidService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_MB_LOGON:			//��¼����
		{
			return OnTCPNetworkMainMBLogon(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_MB_SERVER_LIST:	//�б�����
		{
			return OnTCPNetworkMainMBServerList(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//���ݿ��¼�
bool CAttemperEngineSink::OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBO_GP_PLATFORM_PARAMETER:		//ƽ̨����
		{
			return OnDBPCPlatformParameter(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_FACE_INFO:			//�û�ͷ��
		{
			return OnDBPCUserFaceInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_INFO:		//��������
		{
			return OnDBPCUserInsureInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_SAVE_RESULT:		//��Ǯ���
		{
			return OnDBPCUserSaveResult(dwContextID, pData, wDataSize);
		}
	case DBO_GP_USER_TAKE_RESULT:		//ȡǮ���
		{
			return OnDBPCUserTakeResult(dwContextID, pData, wDataSize);
		}
	case DBO_GP_USER_INSURE_SUCCESS:	//���гɹ�
		{
			return OnDBPCUserInsureSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_FAILURE:	//����ʧ��
		{
			return OnDBPCUserInsureFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_USER_INFO:  //�û���Ϣ
		{
			return OnDBPCUserInsureUserInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_ENABLE_RESULT: //��ͨ���
		{
			return OnDBPCUserInsureEnableResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_ANDROID_PARAMETER:		//��������
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
	case DBO_GP_OPERATE_SUCCESS:		//�����ɹ�
		{
			return OnDBPCOperateSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GP_OPERATE_FAILURE:		//����ʧ��
		{
			return OnDBPCOperateFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_TYPE_ITEM:			//��������
		{
			return OnDBPCGameTypeItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_KIND_ITEM:			//��������
		{
			return OnDBPCGameKindItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_NODE_ITEM:			//�ڵ�����
		{
			return OnDBPCGameNodeItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_PAGE_ITEM:			//��������
		{
			return OnDBPCGamePageItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_LIST_RESULT:		//���ؽ��
		{
			return OnDBPCGameListResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_SERVER_DUMMY_ONLINE_BEGIN:
		{
			return OnDBServerDummyOnLineBegin(dwContextID, pData, wDataSize);
		}
	case DBO_GP_SERVER_DUMMY_ONLINE:		//������������
		{
			return OnDBServerDummyOnLine(dwContextID, pData, wDataSize);
		}
	case DBO_GP_SERVER_DUMMY_ONLINE_END:
		{
			return OnDBServerDummyOnLineEnd(dwContextID, pData, wDataSize);
		}
	case DBO_GP_BASEENSURE_PARAMETER:	//�ͱ�����
		{
			return OnDBPCBaseEnsureParameter(dwContextID,pData,wDataSize);
		}
	case DBO_GP_BASEENSURE_RESULT:		//�ͱ����
		{
			return OnDBPCBaseEnsureResult(dwContextID,pData,wDataSize);
		}
	case DBO_MB_LOGON_SUCCESS:			//��¼�ɹ�
		{
			return OnDBMBLogonSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_MB_LOGON_FAILURE:			//��¼ʧ��
		{
			return OnDBMBLogonFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_LOTTERY_CONFIG:			//�齱����
		{
			return OnDBPCLotteryConfig(dwContextID,pData,wDataSize);
		}
	case DBO_GP_LOTTERY_USER_INFO:		//�齱��Ϣ
		{
			return OnDBPCLotteryUserInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_LOTTERY_RESULT:			//�齱���
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

//�ر��¼�
bool CAttemperEngineSink::OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)
{
	//Э������
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//�����ж�
		if (m_bNeekCorrespond==true)
		{
			//������ʾ
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("��Э�������������ӹر��ˣ�%ld �����������"),m_pInitParameter->m_wConnectTime);

			//��ʾ��Ϣ
			CTraceService::TraceString(szDescribe,TraceLevel_Warning);

			//����ʱ��
			ASSERT(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);

			return true;
		}
	}

	return false;
}

//�����¼�
bool CAttemperEngineSink::OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode)
{
	//Э������
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//�����ж�
		if (nErrorCode!=0)
		{
			//������ʾ
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("Э������������ʧ�� [ %ld ]��%ld �����������"),
				nErrorCode,m_pInitParameter->m_wConnectTime);

			//��ʾ��Ϣ
			CTraceService::TraceString(szDescribe,TraceLevel_Warning);

			//����ʱ��
			ASSERT(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);

			return false;
		}

		//��ʾ��Ϣ
		CTraceService::TraceString(TEXT("����ע����Ϸ��¼������..."),TraceLevel_Normal);

		//��������
		CMD_CS_C_RegisterPlaza RegisterPlaza;
		ZeroMemory(&RegisterPlaza,sizeof(RegisterPlaza));

		//���ñ���
		lstrcpyn(RegisterPlaza.szServerName,m_pInitParameter->m_szServerName,CountArray(RegisterPlaza.szServerName));
		lstrcpyn(RegisterPlaza.szServerAddr,m_pInitParameter->m_ServiceAddress.szAddress,CountArray(RegisterPlaza.szServerAddr));

		//��������
		m_pITCPSocketService->SendData(MDM_CS_REGISTER,SUB_CS_C_REGISTER_PLAZA,&RegisterPlaza,sizeof(RegisterPlaza));

		return true;
	}

	return true;
}

//��ȡ�¼�
bool CAttemperEngineSink::OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//Э������
	if (wServiceID==NETWORK_CORRESPOND)
	{
		switch (Command.wMainCmdID)
		{
		case MDM_CS_REGISTER:		//ע�����
			{
				return OnTCPSocketMainRegister(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_SERVICE_INFO:	//������Ϣ
			{
				return OnTCPSocketMainServiceInfo(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_REMOTE_SERVICE:	//Զ�̷���
			{
				return OnTCPSocketMainRemoteService(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_MANAGER_SERVICE: //�������
			{
				return OnTCPSocketMainManagerService(Command.wSubCmdID,pData,wDataSize);
			}
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//ע���¼�
bool CAttemperEngineSink::OnTCPSocketMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_REGISTER_FAILURE:		//ע��ʧ��
		{
			//��������
			CMD_CS_S_RegisterFailure * pRegisterFailure=(CMD_CS_S_RegisterFailure *)pData;

			//Ч�����
			ASSERT(wDataSize>=(sizeof(CMD_CS_S_RegisterFailure)-sizeof(pRegisterFailure->szDescribeString)));
			if (wDataSize<(sizeof(CMD_CS_S_RegisterFailure)-sizeof(pRegisterFailure->szDescribeString))) return false;

			//�رմ���
			m_bNeekCorrespond=false;
			m_pITCPSocketService->CloseSocket();

			//��ʾ��Ϣ
			LPCTSTR pszDescribeString=pRegisterFailure->szDescribeString;
			if (lstrlen(pszDescribeString)>0) CTraceService::TraceString(pszDescribeString,TraceLevel_Exception);

			//�¼�֪ͨ
			CP_ControlResult ControlResult;
			ControlResult.cbSuccess=ER_FAILURE;
			SendUIControlPacket(UI_CORRESPOND_RESULT,&ControlResult,sizeof(ControlResult));

			return true;
		}
	}

	return true;
}

//�б��¼�
bool CAttemperEngineSink::OnTCPSocketMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_SERVER_INFO:		//������Ϣ
		{
			//�����б�
			m_ServerListManager.DisuseServerItem();

			return true;
		}
	case SUB_CS_S_SERVER_ONLINE:	//��������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerOnLine));
			if (wDataSize!=sizeof(CMD_CS_S_ServerOnLine)) return false;

			//��������
			CMD_CS_S_ServerOnLine * pServerOnLine=(CMD_CS_S_ServerOnLine *)pData;

			//���ҷ���
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerOnLine->wServerID);
			if(pGameServerItem == NULL) return true;

			//��ȡ����
			tagGameServer * pGameServer=&pGameServerItem->m_GameServer; 

			//��������
			DWORD dwOldOnlineCount=0,dwOldAndroidCount=0,dwOldDummyCount=0;
			dwOldOnlineCount = pGameServer->dwOnLineCount;
			dwOldAndroidCount = pGameServer->dwAndroidCount;
			dwOldDummyCount = pGameServer->dwDummyCount;

			//���ҷ���������������
			tagServerDummyOnLine * pDummyOnLine= NULL;
			m_ServerDummyOnLineMap.Lookup(pServerOnLine->wServerID,pDummyOnLine);

			//��������
			pGameServer->dwAndroidCount=pServerOnLine->dwAndroidCount;
			pGameServer->dwOnLineCount=pServerOnLine->dwOnLineCount;

			if (pDummyOnLine != NULL)
				pGameServer->dwDummyCount = pDummyOnLine->dwCurDummyOnline;
			else
				pGameServer->dwDummyCount = 0;

			//Ŀ¼����
			CGameKindItem * pGameKindItem=m_ServerListManager.SearchGameKind(pGameServer->wKindID);
			if (pGameKindItem!=NULL)
			{
				//��������
				pGameKindItem->m_GameKind.dwOnLineCount -= dwOldOnlineCount;
				pGameKindItem->m_GameKind.dwOnLineCount += pGameServer->dwOnLineCount;

				//��������
				pGameKindItem->m_GameKind.dwAndroidCount -= dwOldAndroidCount;
				pGameKindItem->m_GameKind.dwAndroidCount += pGameServer->dwAndroidCount;

				//��������
				pGameKindItem->m_GameKind.dwDummyCount -= dwOldDummyCount;
				pGameKindItem->m_GameKind.dwDummyCount += pGameServer->dwDummyCount;
			}

			return true;
		}
	case SUB_CS_S_SERVER_INSERT:	//�������
		{
			//Ч�����
			ASSERT(wDataSize%sizeof(tagGameServer)==0);
			if (wDataSize%sizeof(tagGameServer)!=0) return false;

			//��������
			WORD wItemCount=wDataSize/sizeof(tagGameServer);
			tagGameServer * pGameServer=(tagGameServer *)pData;

			//��������
			for (WORD i=0;i<wItemCount;i++)
			{
				m_ServerListManager.InsertGameServer(pGameServer++);
			}

			return true;
		}
	case SUB_CS_S_SERVER_MODIFY:	//�����޸�
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerModify));
			if (wDataSize!=sizeof(CMD_CS_S_ServerModify)) return false;

			//��������
			CMD_CS_S_ServerModify * pServerModify=(CMD_CS_S_ServerModify *)pData;

			//���ҷ���
			ASSERT(m_ServerListManager.SearchGameServer(pServerModify->wServerID));
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerModify->wServerID);

			//���÷���
			if (pGameServerItem!=NULL)
			{
				//��������
				DWORD dwOldOnlineCount=0,dwOldAndroidCount=0,dwOldFullCount=0;
				dwOldAndroidCount = pGameServerItem->m_GameServer.dwAndroidCount;
				dwOldOnlineCount = pGameServerItem->m_GameServer.dwOnLineCount;				
				dwOldFullCount   = pGameServerItem->m_GameServer.dwFullCount;

				//�޸ķ�����Ϣ
				pGameServerItem->m_GameServer.wKindID=pServerModify->wKindID;
				pGameServerItem->m_GameServer.wNodeID=pServerModify->wNodeID;
				pGameServerItem->m_GameServer.wSortID=pServerModify->wSortID;
				pGameServerItem->m_GameServer.wServerPort=pServerModify->wServerPort;
				pGameServerItem->m_GameServer.dwOnLineCount=pServerModify->dwOnLineCount;
				pGameServerItem->m_GameServer.dwFullCount=pServerModify->dwFullCount;
				pGameServerItem->m_GameServer.dwAndroidCount=pServerModify->dwAndroidCount;
				lstrcpyn(pGameServerItem->m_GameServer.szServerName,pServerModify->szServerName,CountArray(pGameServerItem->m_GameServer.szServerName));
				lstrcpyn(pGameServerItem->m_GameServer.szServerAddr,pServerModify->szServerAddr,CountArray(pGameServerItem->m_GameServer.szServerAddr));

				//Ŀ¼����
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
	case SUB_CS_S_SERVER_REMOVE:	//����ɾ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerRemove));
			if (wDataSize!=sizeof(CMD_CS_S_ServerRemove)) return false;

			//��������
			CMD_CS_S_ServerRemove * pServerRemove=(CMD_CS_S_ServerRemove *)pData;

			//��������
			m_ServerListManager.DeleteGameServer(pServerRemove->wServerID);

			return true;
		}
	case SUB_CS_S_SERVER_FINISH:	//�������
		{
			//�����б�
			m_ServerListManager.CleanServerItem();

			//�¼�����
			CP_ControlResult ControlResult;
			ControlResult.cbSuccess=ER_SUCCESS;
			SendUIControlPacket(UI_CORRESPOND_RESULT,&ControlResult,sizeof(ControlResult));

			return true;
		}
	case SUB_CS_S_MATCH_INSERT:		//��������
		{
			//Ч�����
			ASSERT(wDataSize%sizeof(tagGameMatch)==0);
			if (wDataSize%sizeof(tagGameMatch)!=0) return false;

			//��������
			WORD wItemCount=wDataSize/sizeof(tagGameMatch);
			tagGameMatch * pGameMatch=(tagGameMatch *)pData;

			//��������
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
			//Ч�����
			ASSERT(wDataSize == sizeof(CMD_CS_S_Customer));
			if (wDataSize != sizeof(CMD_CS_S_Customer)) return false;

				//��������
			CMD_CS_S_Customer * pCustomer = (CMD_CS_S_Customer *)pData;

			//���������������
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
			//Ч�����
			ASSERT(wDataSize == sizeof(CMD_CS_S_RechageSuccess));
			if (wDataSize != sizeof(CMD_CS_S_RechageSuccess)) return false;

			//��������
			CMD_CS_S_RechageSuccess * pRechageSuccess = (CMD_CS_S_RechageSuccess *)pData;

			//���������������
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
			//Ч�����
			ASSERT(wDataSize == sizeof(CMD_CS_S_NotifyidNewMail));
			if (wDataSize != sizeof(CMD_CS_S_NotifyidNewMail)) return false;

			//��������
			CMD_CS_S_NotifyidNewMail * pNotifyidNewMail = (CMD_CS_S_NotifyidNewMail *)pData;

			//���������������
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

//Զ�̷���
bool CAttemperEngineSink::OnTCPSocketMainRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_SEARCH_CORRESPOND:	//Э������
		{
			//��������
			CMD_CS_S_SearchCorrespond * pSearchCorrespond=(CMD_CS_S_SearchCorrespond *)pData;

			//Ч�����
			ASSERT(wDataSize<=sizeof(CMD_CS_S_SearchCorrespond));
			ASSERT(wDataSize>=(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)));
			ASSERT(wDataSize==(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)+pSearchCorrespond->wUserCount*sizeof(pSearchCorrespond->UserRemoteInfo[0])));

			//Ч�����
			if (wDataSize>sizeof(CMD_CS_S_SearchCorrespond)) return false;
			if (wDataSize<(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo))) return false;
			if (wDataSize!=(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)+pSearchCorrespond->wUserCount*sizeof(pSearchCorrespond->UserRemoteInfo[0]))) return false;

			//�ж�����
			ASSERT(LOWORD(pSearchCorrespond->dwSocketID)<m_pInitParameter->m_wMaxConnect);
			if ((m_pBindParameter+LOWORD(pSearchCorrespond->dwSocketID))->dwSocketID!=pSearchCorrespond->dwSocketID) return true;

			//��������
			CMD_GP_S_SearchCorrespond SearchCorrespond;
			ZeroMemory(&SearchCorrespond,sizeof(SearchCorrespond));

			//���ñ���
			for (WORD i=0;i<pSearchCorrespond->wUserCount;i++)
			{
				//����Ч��
				ASSERT(SearchCorrespond.wUserCount<CountArray(SearchCorrespond.UserRemoteInfo));
				if (SearchCorrespond.wUserCount>=CountArray(SearchCorrespond.UserRemoteInfo)) break;

				//���ñ���
				WORD wIndex=SearchCorrespond.wUserCount++;
				CopyMemory(&SearchCorrespond.UserRemoteInfo[wIndex],&pSearchCorrespond->UserRemoteInfo[i],sizeof(SearchCorrespond.UserRemoteInfo[wIndex]));
			}

			//��������
			WORD wHeadSize=sizeof(SearchCorrespond)-sizeof(SearchCorrespond.UserRemoteInfo);
			WORD wItemSize=sizeof(SearchCorrespond.UserRemoteInfo[0])*SearchCorrespond.wUserCount;
			m_pITCPNetworkEngine->SendData(pSearchCorrespond->dwSocketID,MDM_GP_REMOTE_SERVICE,SUB_GP_S_SEARCH_CORRESPOND,&SearchCorrespond,wHeadSize+wItemSize);

			return true;
		}
	case SUB_CS_S_SEARCH_ALLCORRESPOND:
		{
			//��������
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

//�������
bool CAttemperEngineSink::OnTCPSocketMainManagerService(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_PLATFORM_PARAMETER:	//ƽ̨����
		{

			//ƽ̨����
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_PLATFORM_PARAMETER,0,NULL,0);		

			//���صͱ�
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_BASEENSURE, 0, NULL, 0);

			return true;
		}
	}

	return true;
}

//�б���
bool CAttemperEngineSink::OnTCPNetworkMainPCServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_GET_LIST:			//��ȡ�б�
		{
			//�����б�
			SendGameTypeInfo(dwSocketID);
			SendGameKindInfo(dwSocketID);

			//�����б�
			if (m_pInitParameter->m_cbDelayList==FALSE)
			{
				//�����б�
				SendGamePageInfo(dwSocketID,INVALID_WORD);
				SendGameNodeInfo(dwSocketID,INVALID_WORD);
				//SendGameServerInfo(dwSocketID,INVALID_WORD);
				SendGameServerInfo(dwSocketID,INVALID_WORD,DEVICE_TYPE_PC);
			}
			else
			{
				//����ҳ��
				SendGamePageInfo(dwSocketID,0);
			}

			//�������
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_FINISH);

			return true;
		}
	case SUB_GP_GET_SERVER:			//��ȡ����
		{
			//Ч������
			ASSERT(wDataSize%sizeof(WORD)==0);
			if (wDataSize%sizeof(WORD)!=0) return false;

			//�����б�
			UINT nKindCount=wDataSize/sizeof(WORD);
			for (UINT i=0;i<nKindCount;i++)
			{
				SendGameNodeInfo(dwSocketID,((WORD *)pData)[i]);
				SendGamePageInfo(dwSocketID,((WORD *)pData)[i]);
				//SendGameServerInfo(dwSocketID,((WORD *)pData)[i]);
				SendGameServerInfo(dwSocketID,((WORD *)pData)[i],DEVICE_TYPE_PC);
			}

			//�������
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_SERVER_FINISH,pData,wDataSize);

			return true;
		}
	case SUB_GP_GET_ONLINE:			//��ȡ����
		{
			//��������
			CMD_GP_GetOnline * pGetOnline=(CMD_GP_GetOnline *)pData;
			WORD wHeadSize=(sizeof(CMD_GP_GetOnline)-sizeof(pGetOnline->wOnLineServerID));

			//Ч������
			ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGetOnline->wServerCount*sizeof(WORD))));
			if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGetOnline->wServerCount*sizeof(WORD)))) return false;

			//��������
			CMD_GP_KindOnline KindOnline;
			CMD_GP_ServerOnline ServerOnline;
			ZeroMemory(&KindOnline,sizeof(KindOnline));
			ZeroMemory(&ServerOnline,sizeof(ServerOnline));

			//��ȡ����
			POSITION KindPosition=NULL;
			do
			{
				//��ȡ����
				CGameKindItem * pGameKindItem=m_ServerListManager.EmunGameKindItem(KindPosition);

				//���ñ���
				if (pGameKindItem!=NULL)
				{
					WORD wKindIndex=KindOnline.wKindCount++;
					KindOnline.OnLineInfoKind[wKindIndex].wKindID=pGameKindItem->m_GameKind.wKindID;
					KindOnline.OnLineInfoKind[wKindIndex].dwOnLineCount=pGameKindItem->m_GameKind.dwOnLineCount+pGameKindItem->m_GameKind.dwAndroidCount;
				}

				//����ж�
				if (KindOnline.wKindCount>=CountArray(KindOnline.OnLineInfoKind))
				{
					ASSERT(FALSE);
					break;
				}

			} while (KindPosition!=NULL);

			//��ȡ����
			for (WORD i=0;i<pGetOnline->wServerCount;i++)
			{
				//��ȡ����
				WORD wServerID=pGetOnline->wOnLineServerID[i];
				CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(wServerID);

				//���ñ���
				if (pGameServerItem!=NULL)
				{
					WORD wServerIndex=ServerOnline.wServerCount++;
					ServerOnline.OnLineInfoServer[wServerIndex].wServerID=wServerID;
					ServerOnline.OnLineInfoServer[wServerIndex].dwOnLineCount=pGameServerItem->m_GameServer.dwOnLineCount+pGameServerItem->m_GameServer.dwAndroidCount;
				}

				//����ж�
				if (ServerOnline.wServerCount>=CountArray(ServerOnline.OnLineInfoServer))
				{
					ASSERT(FALSE);
					break;
				}
			}

			//��������
			if (KindOnline.wKindCount>0)
			{
				WORD wHeadSize=sizeof(KindOnline)-sizeof(KindOnline.OnLineInfoKind);
				WORD wSendSize=wHeadSize+KindOnline.wKindCount*sizeof(KindOnline.OnLineInfoKind[0]);
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GR_KINE_ONLINE,&KindOnline,wSendSize);
			}

			//��������
			if (ServerOnline.wServerCount>0)
			{
				WORD wHeadSize=sizeof(ServerOnline)-sizeof(ServerOnline.OnLineInfoServer);
				WORD wSendSize=wHeadSize+ServerOnline.wServerCount*sizeof(ServerOnline.OnLineInfoServer[0]);
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GR_SERVER_ONLINE,&ServerOnline,wSendSize);
			}

			return true;
		}
	case SUB_GP_GET_COLLECTION:		//��ȡ�ղ�
		{
			return true;
		}
	}

	return false;
}

//������
bool CAttemperEngineSink::OnTCPNetworkMainPCUserService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_MODIFY_INSURE_PASS:		  //�޸�����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_ModifyInsurePass));
			if (wDataSize!=sizeof(CMD_GP_ModifyInsurePass)) return false;

			//������Ϣ
			CMD_GP_ModifyInsurePass * pModifyInsurePass=(CMD_GP_ModifyInsurePass *)pData;
			pModifyInsurePass->szDesPassword[CountArray(pModifyInsurePass->szDesPassword)-1]=0;
			pModifyInsurePass->szScrPassword[CountArray(pModifyInsurePass->szScrPassword)-1]=0;

			//��������
			DBR_GP_ModifyInsurePass ModifyInsurePass;
			ZeroMemory(&ModifyInsurePass,sizeof(ModifyInsurePass));

			//��������
			ModifyInsurePass.dwUserID=pModifyInsurePass->dwUserID;
			ModifyInsurePass.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyInsurePass.szDesPassword,pModifyInsurePass->szDesPassword,CountArray(ModifyInsurePass.szDesPassword));
			lstrcpyn(ModifyInsurePass.szScrPassword,pModifyInsurePass->szScrPassword,CountArray(ModifyInsurePass.szScrPassword));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_INSURE_PASS,dwSocketID,&ModifyInsurePass,sizeof(ModifyInsurePass));

			return true;
		}
	case SUB_GP_SYSTEM_FACE_INFO:		  //�޸�ͷ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_SystemFaceInfo));
			if (wDataSize!=sizeof(CMD_GP_SystemFaceInfo)) return false;

			//������Ϣ
			CMD_GP_SystemFaceInfo * pSystemFaceInfo=(CMD_GP_SystemFaceInfo *)pData;

			//��������
			DBR_GP_ModifySystemFace ModifySystemFace;
			ZeroMemory(&ModifySystemFace,sizeof(ModifySystemFace));

			//��������
			ModifySystemFace.wFaceID=pSystemFaceInfo->wFaceID;
			ModifySystemFace.dwUserID=pSystemFaceInfo->dwUserID;
			ModifySystemFace.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifySystemFace.szPassword,pSystemFaceInfo->szPassword,CountArray(ModifySystemFace.szPassword));
			lstrcpyn(ModifySystemFace.szMachineID,pSystemFaceInfo->szMachineID,CountArray(ModifySystemFace.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_SYSTEM_FACE,dwSocketID,&ModifySystemFace,sizeof(ModifySystemFace));

			return true;
		}
	case SUB_GP_MODIFY_INDIVIDUAL:		  //�޸�����
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(CMD_GP_ModifyIndividual));
			if (wDataSize<sizeof(CMD_GP_ModifyIndividual)) return false;

			//������Ϣ
			CMD_GP_ModifyIndividual * pModifyIndividual=(CMD_GP_ModifyIndividual *)pData;
			pModifyIndividual->szPassword[CountArray(pModifyIndividual->szPassword)-1]=0;

			//��������
			DBR_GP_ModifyIndividual ModifyIndividual;
			ZeroMemory(&ModifyIndividual,sizeof(ModifyIndividual));

			//���ñ���
			ModifyIndividual.dwUserID=pModifyIndividual->dwUserID;
			ModifyIndividual.dwClientAddr = (m_pBindParameter + LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyIndividual.szPassword,pModifyIndividual->szPassword,CountArray(ModifyIndividual.szPassword));
			lstrcpyn(ModifyIndividual.szNickName, pModifyIndividual->szNickName, CountArray(ModifyIndividual.szNickName));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_INDIVIDUAL,dwSocketID,&ModifyIndividual,sizeof(ModifyIndividual));

			return true;
		}
	case SUB_GP_USER_ENABLE_INSURE:		  //��ͨ����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_UserEnableInsure));
			if (wDataSize!=sizeof(CMD_GP_UserEnableInsure)) return false;

			//������Ϣ
			CMD_GP_UserEnableInsure * pUserEnableInsure=(CMD_GP_UserEnableInsure *)pData;
			pUserEnableInsure->szLogonPass[CountArray(pUserEnableInsure->szLogonPass)-1]=0;
			pUserEnableInsure->szInsurePass[CountArray(pUserEnableInsure->szInsurePass)-1]=0;
			pUserEnableInsure->szMachineID[CountArray(pUserEnableInsure->szMachineID)-1]=0;

			//��������
			DBR_GP_UserEnableInsure UserEnableInsure;
			ZeroMemory(&UserEnableInsure,sizeof(UserEnableInsure));

			//��������
			UserEnableInsure.dwUserID=pUserEnableInsure->dwUserID;			
			UserEnableInsure.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserEnableInsure.szLogonPass,pUserEnableInsure->szLogonPass,CountArray(UserEnableInsure.szLogonPass));
			lstrcpyn(UserEnableInsure.szInsurePass,pUserEnableInsure->szInsurePass,CountArray(UserEnableInsure.szInsurePass));
			lstrcpyn(UserEnableInsure.szMachineID,pUserEnableInsure->szMachineID,CountArray(UserEnableInsure.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_ENABLE_INSURE,dwSocketID,&UserEnableInsure,sizeof(UserEnableInsure));

			return true;
		}
	case SUB_GP_USER_SAVE_SCORE:		  //������Ϸ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_UserSaveScore));
			if (wDataSize!=sizeof(CMD_GP_UserSaveScore)) return false;

			//������Ϣ
			CMD_GP_UserSaveScore * pUserSaveScore=(CMD_GP_UserSaveScore *)pData;
			pUserSaveScore->szMachineID[CountArray(pUserSaveScore->szMachineID)-1]=0;

			//��������
			DBR_GP_UserSaveScore UserSaveScore;
			ZeroMemory(&UserSaveScore,sizeof(UserSaveScore));

			//��������
			UserSaveScore.dwUserID=pUserSaveScore->dwUserID;
			UserSaveScore.lSaveScore=pUserSaveScore->lSaveScore;
			UserSaveScore.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserSaveScore.szMachineID,pUserSaveScore->szMachineID,CountArray(UserSaveScore.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_SAVE_SCORE,dwSocketID,&UserSaveScore,sizeof(UserSaveScore));

			return true;
		}
	case SUB_GP_EXCHANGE_BANK_SCORE:
		{
			//Ч�����
			ASSERT(wDataSize == sizeof(CMD_GP_ExchangeBankScore));
			int asd = sizeof(CMD_GP_ExchangeBankScore);
			if (wDataSize != sizeof(CMD_GP_ExchangeBankScore)) return false;

			//������Ϣ
			CMD_GP_ExchangeBankScore * pClientData = (CMD_GP_ExchangeBankScore *)pData;
			pClientData->szBankPass[CountArray(pClientData->szBankPass) - 1] = 0;
			pClientData->szOrderID[CountArray(pClientData->szOrderID) - 1] = 0;

			//��������
			DBR_GP_ExchangeBankScore DBRData;
			ZeroMemory(&DBRData, sizeof(DBRData));
			//��������
			DBRData.dwUserID = pClientData->dwUserID;
			DBRData.dwScore = pClientData->dwScore;
			lstrcpyn(DBRData.szBankPass, pClientData->szBankPass, CountArray(DBRData.szBankPass));
			lstrcpyn(DBRData.szOrderID, pClientData->szOrderID, CountArray(DBRData.szOrderID));

			DBRData.dwClientAddr = (m_pBindParameter + LOWORD(dwSocketID))->dwClientAddr;
			DBRData.dwType = pClientData->dwType;
			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_EXCHANGE_BANK_SCORE, dwSocketID, &DBRData, sizeof(DBRData));
			return true;
		}
	case SUB_GP_USER_TAKE_SCORE:		  //ȡ����Ϸ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_UserTakeScore));
			if (wDataSize!=sizeof(CMD_GP_UserTakeScore)) return false;

			//������Ϣ
			CMD_GP_UserTakeScore * pUserTakeScore=(CMD_GP_UserTakeScore *)pData;
			pUserTakeScore->szPassword[CountArray(pUserTakeScore->szPassword)-1]=0;
			pUserTakeScore->szMachineID[CountArray(pUserTakeScore->szMachineID)-1]=0;

			//��������
			DBR_GP_UserTakeScore UserTakeScore;
			ZeroMemory(&UserTakeScore,sizeof(UserTakeScore));

			//��������
			UserTakeScore.dwUserID=pUserTakeScore->dwUserID;
			UserTakeScore.lTakeScore=pUserTakeScore->lTakeScore;
			UserTakeScore.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserTakeScore.szPassword,pUserTakeScore->szPassword,CountArray(UserTakeScore.szPassword));
			lstrcpyn(UserTakeScore.szMachineID,pUserTakeScore->szMachineID,CountArray(UserTakeScore.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_TAKE_SCORE,dwSocketID,&UserTakeScore,sizeof(UserTakeScore));

			return true;
		}
	case SUB_GP_USER_TRANSFER_SCORE:	  //ת����Ϸ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_UserTransferScore));
			if (wDataSize!=sizeof(CMD_GP_UserTransferScore)) return false;

			//������Ϣ
			CMD_GP_UserTransferScore * pUserTransferScore=(CMD_GP_UserTransferScore *)pData;
			pUserTransferScore->szAccounts[CountArray(pUserTransferScore->szAccounts)-1]=0;
			pUserTransferScore->szPassword[CountArray(pUserTransferScore->szPassword)-1]=0;
			pUserTransferScore->szMachineID[CountArray(pUserTransferScore->szMachineID)-1]=0;

			//��������
			DBR_GP_UserTransferScore UserTransferScore;
			ZeroMemory(&UserTransferScore,sizeof(UserTransferScore));

			//��������
			UserTransferScore.dwUserID=pUserTransferScore->dwUserID;
			UserTransferScore.lTransferScore=pUserTransferScore->lTransferScore;
			UserTransferScore.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserTransferScore.szAccounts,pUserTransferScore->szAccounts,CountArray(UserTransferScore.szAccounts));
			lstrcpyn(UserTransferScore.szPassword,pUserTransferScore->szPassword,CountArray(UserTransferScore.szPassword));
			lstrcpyn(UserTransferScore.szMachineID,pUserTransferScore->szMachineID,CountArray(UserTransferScore.szMachineID));
			lstrcpyn(UserTransferScore.szTransRemark,pUserTransferScore->szTransRemark,CountArray(UserTransferScore.szTransRemark));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_TRANSFER_SCORE,dwSocketID,&UserTransferScore,sizeof(UserTransferScore));

			return true;
		}
	case SUB_GP_QUERY_INSURE_INFO:		  //��ѯ����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_QueryInsureInfo));
			if (wDataSize!=sizeof(CMD_GP_QueryInsureInfo)) return false;

			//������Ϣ
			CMD_GP_QueryInsureInfo * pQueryInsureInfo=(CMD_GP_QueryInsureInfo *)pData;

			//��������
			DBR_GP_QueryInsureInfo QueryInsureInfo;
			ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

			//��������
			QueryInsureInfo.dwUserID=pQueryInsureInfo->dwUserID;
			QueryInsureInfo.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(QueryInsureInfo.szPassword,pQueryInsureInfo->szPassword,CountArray(QueryInsureInfo.szPassword));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_INSURE_INFO,dwSocketID,&QueryInsureInfo,sizeof(QueryInsureInfo));

			return true;
		}
	case SUB_GP_QUERY_USER_INFO_REQUEST:  //��ѯ�û�
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_QueryUserInfoRequest));
			if (wDataSize!=sizeof(CMD_GP_QueryUserInfoRequest)) return false;

			//������Ϣ
			CMD_GP_QueryUserInfoRequest * pQueryUserInfoRequest=(CMD_GP_QueryUserInfoRequest *)pData;

			//ID�ж�
			if(pQueryUserInfoRequest->cbByNickName==FALSE)
			{
				//�����ж�
				int nLen=lstrlen(pQueryUserInfoRequest->szAccounts);
				if(nLen>=8)
				{
					SendInsureFailure(dwSocketID,0,TEXT("������Ϸ������ID��"));
					return true;
				}

				//�Ϸ��ж�
				for(int i=0; i<nLen; i++)
				{
					if(pQueryUserInfoRequest->szAccounts[i] < TEXT('0') || pQueryUserInfoRequest->szAccounts[i] > TEXT('9'))
					{
						SendInsureFailure(dwSocketID,0,TEXT("������Ϸ������ID��"));
						return true;
					}
				}
			}

			//��������
			DBR_GP_QueryInsureUserInfo QueryInsureUserInfo;
			ZeroMemory(&QueryInsureUserInfo,sizeof(QueryInsureUserInfo));

			//��������
			QueryInsureUserInfo.cbByNickName=pQueryUserInfoRequest->cbByNickName;
			lstrcpyn(QueryInsureUserInfo.szAccounts,pQueryUserInfoRequest->szAccounts,CountArray(QueryInsureUserInfo.szAccounts));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_USER_INFO,dwSocketID,&QueryInsureUserInfo,sizeof(QueryInsureUserInfo));

			return true;
		}
	case SUB_GP_BASEENSURE_LOAD:		//���صͱ�
		{
			//����ṹ
			CMD_GP_BaseEnsureParamter BaseEnsureParameter;
			BaseEnsureParameter.cbTakeTimes=m_BaseEnsureParameter.cbTakeTimes;
			BaseEnsureParameter.lScoreAmount=m_BaseEnsureParameter.lScoreAmount;
			BaseEnsureParameter.lScoreCondition=m_BaseEnsureParameter.lScoreCondition;

			//Ͷ������
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GP_BASEENSURE_PARAMETER,&BaseEnsureParameter,sizeof(BaseEnsureParameter));

			return true;
		}
	case SUB_GP_BASEENSURE_TAKE:		  //��ȡ�ͱ�
		{
			//����У��
			ASSERT(wDataSize==sizeof(CMD_GP_BaseEnsureTake));
			if(wDataSize!=sizeof(CMD_GP_BaseEnsureTake)) return false;

			//��ȡ����
			CMD_GP_BaseEnsureTake * pBaseEnsureTake = (CMD_GP_BaseEnsureTake *)pData;
			pBaseEnsureTake->szPassword[CountArray(pBaseEnsureTake->szPassword)-1]=0;
			pBaseEnsureTake->szMachineID[CountArray(pBaseEnsureTake->szMachineID)-1]=0;

			//����ṹ
			DBR_GP_TakeBaseEnsure TakeBaseEnsure;
			TakeBaseEnsure.dwUserID = pBaseEnsureTake->dwUserID;
			TakeBaseEnsure.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(TakeBaseEnsure.szPassword,pBaseEnsureTake->szPassword,CountArray(TakeBaseEnsure.szPassword));
			lstrcpyn(TakeBaseEnsure.szMachineID,pBaseEnsureTake->szMachineID,CountArray(TakeBaseEnsure.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_BASEENSURE_TAKE,dwSocketID,&TakeBaseEnsure,sizeof(TakeBaseEnsure));

			return true;
		}
	case SUB_GP_QUERY_GAMEROOM:				//��ѯ�ɼ�����Ϸ����
		{
			//����У��
			ASSERT(wDataSize == sizeof(CMD_GP_QueryGameRoom));
			if (wDataSize != sizeof(CMD_GP_QueryGameRoom)) return false;

			//��ȡ����
			CMD_GP_QueryGameRoom * pQueryGameRoom = (CMD_GP_QueryGameRoom *)pData;

			//ö������
			POSITION Position = NULL;
			CGameServerItem * pGameServerItem = NULL;

			//ö������
			for (DWORD i = 0; i<m_ServerListManager.GetGameServerCount(); i++)
			{
				//��ȡ����
				pGameServerItem = m_ServerListManager.EmunGameServerItem(Position);
				if (pGameServerItem == NULL) break;

				//��������
				if (pGameServerItem->m_GameServer.wKindID == pQueryGameRoom->wKindID && pGameServerItem->m_GameServer.wServerLevel == pQueryGameRoom->wServerLevel)
				{
					if (pGameServerItem->m_GameServer.dwOnLineCount < (pGameServerItem->m_GameServer.dwFullCount))
					{
						//����ṹ
						CMD_GP_QueryGameRoomResult QueryGameRoomResult;
						QueryGameRoomResult.wServerPort = pGameServerItem->m_GameServer.wServerPort;
						lstrcpyn(QueryGameRoomResult.szServerAddr, pGameServerItem->m_GameServer.szServerAddr, CountArray(QueryGameRoomResult.szServerAddr));

						//��������
						m_pITCPNetworkEngine->SendData(dwSocketID, MDM_GP_USER_SERVICE, SUB_GP_QUERY_GAMEROOM_RESULT, &QueryGameRoomResult, sizeof(QueryGameRoomResult));
						break;
					}
				}
			}

			return true;
		}
	case SUB_GP_LOTTERY_CONFIG_REQ:			//��������
		{
			//����У��
			ASSERT(wDataSize==sizeof(CMD_GP_LotteryConfigReq));
			if(wDataSize!=sizeof(CMD_GP_LotteryConfigReq)) return false;

			//����ṹ
			CMD_GP_LotteryConfigReq * pLotteryConfigReq = (CMD_GP_LotteryConfigReq*)pData;
			pLotteryConfigReq->szLogonPass[CountArray(pLotteryConfigReq->szLogonPass)-1]=0;

			//����ṹ
			DBR_GP_LotteryConfigReq LotteryConfigReq;
			ZeroMemory(&LotteryConfigReq,sizeof(LotteryConfigReq));

			//���ñ���
			LotteryConfigReq.dwUserID=pLotteryConfigReq->dwUserID;
			LotteryConfigReq.wKindID=pLotteryConfigReq->wKindID;
			LotteryConfigReq.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(LotteryConfigReq.szLogonPass,pLotteryConfigReq->szLogonPass,CountArray(LotteryConfigReq.szLogonPass));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOTTERY_CONFIG_REQ,dwSocketID,&LotteryConfigReq,sizeof(LotteryConfigReq));

			return true;
		}
	case SUB_GP_LOTTERY_START:				//�齱��ʼ
		{
			//����У��
			ASSERT(wDataSize==sizeof(CMD_GP_LotteryStart));
			if(wDataSize!=sizeof(CMD_GP_LotteryStart)) return false;

			//����ṹ
			CMD_GP_LotteryStart * pLotteryStart = (CMD_GP_LotteryStart*)pData;
			pLotteryStart->szLogonPass[CountArray(pLotteryStart->szLogonPass)-1]=0;
			pLotteryStart->szMachineID[CountArray(pLotteryStart->szMachineID)-1]=0;

			//����ṹ
			DBR_GP_LotteryStart LotteryStart;
			ZeroMemory(&LotteryStart,sizeof(LotteryStart));

			//���ñ���
			LotteryStart.dwUserID=pLotteryStart->dwUserID;
			LotteryStart.wKindID=pLotteryStart->wKindID;
			LotteryStart.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(LotteryStart.szLogonPass,pLotteryStart->szLogonPass,CountArray(LotteryStart.szLogonPass));
			lstrcpyn(LotteryStart.szMachineID,pLotteryStart->szMachineID,CountArray(LotteryStart.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOTTERY_START,dwSocketID,&LotteryStart,sizeof(LotteryStart));

			return true;
		}
	case SUB_GP_QUERY_EMAIL:
		{
			//����У��
			ASSERT(wDataSize == sizeof(CMD_GP_QueryEmail));
			if (wDataSize != sizeof(CMD_GP_QueryEmail)) return false;

				//����ṹ
			CMD_GP_QueryEmail * pQueryEmail = (CMD_GP_QueryEmail*)pData;

			//����ṹ
			DBR_GP_QueryEmail QueryEmail;
			ZeroMemory(&QueryEmail, sizeof(QueryEmail));

			//���ñ���
			QueryEmail.dwUserID = pQueryEmail->dwUserID;
			QueryEmail.nIndex = pQueryEmail->nIndex;

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_EMAIL, dwSocketID, &QueryEmail, sizeof(QueryEmail));

			return true;
		}
	}

	return false;
}

//Զ�̴���
bool CAttemperEngineSink::OnTCPNetworkMainPCRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_C_SEARCH_CORRESPOND:	//Э������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_C_SearchCorrespond));
			if (wDataSize!=sizeof(CMD_GP_C_SearchCorrespond)) return false;

			//������Ϣ
			CMD_GP_C_SearchCorrespond * pSearchCorrespond=(CMD_GP_C_SearchCorrespond *)pData;
			pSearchCorrespond->szNickName[CountArray(pSearchCorrespond->szNickName)-1]=0;

			//��������
			CMD_CS_C_SearchCorrespond SearchCorrespond;
			ZeroMemory(&SearchCorrespond,sizeof(SearchCorrespond));

			//���ӱ���
			SearchCorrespond.dwSocketID=dwSocketID;
			SearchCorrespond.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;

			//���ұ���
			SearchCorrespond.dwGameID=pSearchCorrespond->dwGameID;
			lstrcpyn(SearchCorrespond.szNickName,pSearchCorrespond->szNickName,CountArray(SearchCorrespond.szNickName));

			//��������
			m_pITCPSocketService->SendData(MDM_CS_REMOTE_SERVICE,SUB_CS_C_SEARCH_CORRESPOND,&SearchCorrespond,sizeof(SearchCorrespond));

			return true;
		}
	case SUB_GP_C_SEARCH_ALLCORRESPOND:
		{
			//��������
			if(wDataSize >= sizeof(CMD_GP_C_SearchAllCorrespond)) 
			{
				CMD_GP_C_SearchAllCorrespond* pSearchAllCorrespond = (CMD_GP_C_SearchAllCorrespond*)pData;

				DWORD cBuffer[512]={0};
				CMD_CS_C_AllSearchCorrespond* pSearchCorrespond = (CMD_CS_C_AllSearchCorrespond*)cBuffer;

				//���ӱ���
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


//��������
bool CAttemperEngineSink::OnTCPNetworkMainAndroidService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_GET_PARAMETER:		//��ȡ����
		{
			//����У��
			ASSERT(wDataSize==sizeof(CMD_GP_GetParameter));
			if(wDataSize!=sizeof(CMD_GP_GetParameter)) return false;

			//��ȡ����
			CMD_GP_GetParameter * pGetParameter = (CMD_GP_GetParameter *)pData;
			ASSERT(pGetParameter!=NULL);

			//����ṹ
			DBR_GP_GetParameter GetParameter;
			GetParameter.wServerID = pGetParameter->wServerID;

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_PARAMETER,dwSocketID,&GetParameter,sizeof(GetParameter));

			return true;
		}
	case SUB_GP_ADD_PARAMETER:		//��Ӳ���
		{
			//����У��
			ASSERT(wDataSize==sizeof(CMD_GP_AddParameter));
			if(wDataSize!=sizeof(CMD_GP_AddParameter)) return false;

			//��ȡ����
			CMD_GP_AddParameter * pAddParameter = (CMD_GP_AddParameter *)pData;
			ASSERT(pAddParameter!=NULL);

			//����ṹ
			DBR_GP_AddParameter AddParameter;
			AddParameter.wServerID = pAddParameter->wServerID;
			CopyMemory(&AddParameter.AndroidParameter,&pAddParameter->AndroidParameter,sizeof(tagAndroidParameter));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_ADD_PARAMETER,dwSocketID,&AddParameter,sizeof(AddParameter));

			return true;
		}
	case SUB_GP_MODIFY_PARAMETER:	//�޸Ĳ���
		{
			//����У��
			ASSERT(wDataSize==sizeof(CMD_GP_ModifyParameter));
			if(wDataSize!=sizeof(CMD_GP_ModifyParameter)) return false;

			//��ȡ����
			CMD_GP_ModifyParameter * pModifyParameter = (CMD_GP_ModifyParameter *)pData;
			ASSERT(pModifyParameter!=NULL);

			//����ṹ
			DBR_GP_ModifyParameter ModifyParameter;
			ModifyParameter.wServerID=pModifyParameter->wServerID;
			CopyMemory(&ModifyParameter.AndroidParameter,&pModifyParameter->AndroidParameter,sizeof(tagAndroidParameter));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_PARAMETER,dwSocketID,&ModifyParameter,sizeof(ModifyParameter));
			
			return true;
		}
	case SUB_GP_DELETE_PARAMETER:	//ɾ������
		{
			//����У��
			ASSERT(wDataSize==sizeof(CMD_GP_DeleteParameter));
			if(wDataSize!=sizeof(CMD_GP_DeleteParameter)) return false;

			//��ȡ����
			CMD_GP_DeleteParameter * pDeleteParameter = (CMD_GP_DeleteParameter *)pData;
			ASSERT(pDeleteParameter!=NULL);

			//����ṹ
			DBR_GP_DeleteParameter DeleteParameter;
			DeleteParameter.wServerID = pDeleteParameter->wServerID;
			DeleteParameter.dwBatchID = pDeleteParameter->dwBatchID;

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_DELETE_PARAMETER,dwSocketID,&DeleteParameter,sizeof(DeleteParameter));
			
			return true;
		}
	}

	return false;
}

//��¼����
bool CAttemperEngineSink::OnTCPNetworkMainMBLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_MB_LOGON_GAMEID:		//I D ��¼
		{
			return OnTCPNetworkSubMBLogonGameID(pData,wDataSize,dwSocketID);
		}
	case SUB_MB_LOGON_ACCOUNTS:		//�ʺŵ�¼
		{
			return OnTCPNetworkSubMBLogonAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_MB_REGISTER_ACCOUNTS:	//�ʺ�ע��
		{
			return OnTCPNetworkSubMBRegisterAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_MB_LOGON_OTHERPLATFORM: //����ƽ̨
		{
			return OnTCPNetworkSubMBLogonOtherPlatform(pData,wDataSize,dwSocketID);
		}
	case SUB_MB_LOGON_VISITOR:      //�ο͵�¼
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

//�б���
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

//�㲥��Ϣ
bool CAttemperEngineSink::OnEventTCPSocketSubRadioMessage(VOID * pData, WORD wDataSize)
{
	//���������������
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

//I D ��¼
bool CAttemperEngineSink::OnTCPNetworkSubMBLogonGameID(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_MB_LogonGameID));
	if (wDataSize<sizeof(CMD_MB_LogonGameID)) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_MB_LogonGameID * pLogonGameID=(CMD_MB_LogonGameID *)pData;
	pLogonGameID->szPassword[CountArray(pLogonGameID->szPassword)-1]=0;
	pLogonGameID->szMachineID[CountArray(pLogonGameID->szMachineID)-1]=0;
	pLogonGameID->szMobilePhone[CountArray(pLogonGameID->szMobilePhone)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pLogonGameID->wModuleID;
	pBindParameter->dwPlazaVersion=pLogonGameID->dwPlazaVersion;

	//Ч��汾
	if (CheckPlazaVersion(pLogonGameID->cbDeviceType,pLogonGameID->dwPlazaVersion,dwSocketID)==false) return true;

	//��������
	DBR_MB_LogonGameID LogonGameID;
	ZeroMemory(&LogonGameID,sizeof(LogonGameID));

	//������Ϣ
	LogonGameID.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	LogonGameID.dwGameID=pLogonGameID->dwGameID;
	LogonGameID.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonGameID.szPassword,pLogonGameID->szPassword,CountArray(LogonGameID.szPassword));
	lstrcpyn(LogonGameID.szMachineID,pLogonGameID->szMachineID,CountArray(LogonGameID.szMachineID));
	lstrcpyn(LogonGameID.szMobilePhone,pLogonGameID->szMobilePhone,CountArray(LogonGameID.szMobilePhone));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_LOGON_GAMEID,dwSocketID,&LogonGameID,sizeof(LogonGameID));

	return true;
}

//�ʺŵ�¼
bool CAttemperEngineSink::OnTCPNetworkSubMBLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_MB_LogonAccounts));
	if (wDataSize<sizeof(CMD_MB_LogonAccounts)) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_MB_LogonAccounts * pLogonAccounts=(CMD_MB_LogonAccounts *)pData;
	pLogonAccounts->szAccounts[CountArray(pLogonAccounts->szAccounts)-1]=0;
	pLogonAccounts->szPassword[CountArray(pLogonAccounts->szPassword)-1]=0;
	pLogonAccounts->szMachineID[CountArray(pLogonAccounts->szMachineID)-1]=0;
	pLogonAccounts->szMobilePhone[CountArray(pLogonAccounts->szMobilePhone)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pLogonAccounts->wModuleID;
	pBindParameter->dwPlazaVersion=pLogonAccounts->dwPlazaVersion;

	//�汾�ж�
	if (CheckPlazaVersion(pLogonAccounts->cbDeviceType,pLogonAccounts->dwPlazaVersion,dwSocketID)==false) return true;

	//��������
	DBR_MB_LogonAccounts LogonAccounts;
	ZeroMemory(&LogonAccounts,sizeof(LogonAccounts));

	//������Ϣ
	LogonAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	LogonAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonAccounts.szAccounts,pLogonAccounts->szAccounts,CountArray(LogonAccounts.szAccounts));
	lstrcpyn(LogonAccounts.szPassword,pLogonAccounts->szPassword,CountArray(LogonAccounts.szPassword));
	lstrcpyn(LogonAccounts.szMachineID,pLogonAccounts->szMachineID,CountArray(LogonAccounts.szMachineID));
	lstrcpyn(LogonAccounts.szMobilePhone,pLogonAccounts->szMobilePhone,CountArray(LogonAccounts.szMobilePhone));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_LOGON_ACCOUNTS,dwSocketID,&LogonAccounts,sizeof(LogonAccounts));

	return true;
}

//�ʺ�ע��
bool CAttemperEngineSink::OnTCPNetworkSubMBRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_MB_RegisterAccounts));
	if (wDataSize<sizeof(CMD_MB_RegisterAccounts)) return false;


	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_MB_RegisterAccounts * pRegisterAccounts=(CMD_MB_RegisterAccounts *)pData;
	pRegisterAccounts->szAccounts[CountArray(pRegisterAccounts->szAccounts)-1]=0;
	pRegisterAccounts->szNickName[CountArray(pRegisterAccounts->szNickName)-1]=0;
	pRegisterAccounts->szMachineID[CountArray(pRegisterAccounts->szMachineID)-1]=0;
	pRegisterAccounts->szLogonPass[CountArray(pRegisterAccounts->szLogonPass)-1]=0;
	pRegisterAccounts->szMobilePhone[CountArray(pRegisterAccounts->szMobilePhone)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pRegisterAccounts->wModuleID;
	pBindParameter->dwPlazaVersion=pRegisterAccounts->dwPlazaVersion;

	//Ч��汾
	if (CheckPlazaVersion(pRegisterAccounts->cbDeviceType,pRegisterAccounts->dwPlazaVersion,dwSocketID)==false) return true;

	//��������
	DBR_MB_RegisterAccounts RegisterAccounts;
	ZeroMemory(&RegisterAccounts,sizeof(RegisterAccounts));

	//������Ϣ
	RegisterAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
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

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_REGISTER_ACCOUNTS,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));

	return true;
}

//������¼
bool CAttemperEngineSink::OnTCPNetworkSubMBLogonOtherPlatform(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_MB_LogonOtherPlatform));
	if (wDataSize<sizeof(CMD_MB_LogonOtherPlatform)) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_MB_LogonOtherPlatform * pLogonOtherPlatform=(CMD_MB_LogonOtherPlatform *)pData;
	pLogonOtherPlatform->szUserUin[CountArray(pLogonOtherPlatform->szUserUin)-1]=0;
	pLogonOtherPlatform->szNickName[CountArray(pLogonOtherPlatform->szNickName)-1]=0;
	pLogonOtherPlatform->szMachineID[CountArray(pLogonOtherPlatform->szMachineID)-1]=0;
	pLogonOtherPlatform->szMobilePhone[CountArray(pLogonOtherPlatform->szMobilePhone)-1]=0;
	pLogonOtherPlatform->szCompellation[CountArray(pLogonOtherPlatform->szCompellation)-1]=0;

	//ƽ̨�ж�
	ASSERT(pLogonOtherPlatform->cbPlatformID==ULMBySina || pLogonOtherPlatform->cbPlatformID==ULMByTencent || pLogonOtherPlatform->cbPlatformID==ULMByRenRen);
	if (pLogonOtherPlatform->cbPlatformID!=ULMBySina && pLogonOtherPlatform->cbPlatformID!=ULMByTencent && pLogonOtherPlatform->cbPlatformID!=ULMByRenRen) return false;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pLogonOtherPlatform->wModuleID;
	pBindParameter->dwPlazaVersion=pLogonOtherPlatform->dwPlazaVersion;

	//�汾�ж�
	if (CheckPlazaVersion(pLogonOtherPlatform->cbDeviceType,pLogonOtherPlatform->dwPlazaVersion,dwSocketID)==false) return true;

	//��������
	DBR_MB_LogonOtherPlatform LogonOtherPlatform;
	ZeroMemory(&LogonOtherPlatform,sizeof(LogonOtherPlatform));

	//������Ϣ
	LogonOtherPlatform.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	LogonOtherPlatform.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	LogonOtherPlatform.cbGender=pLogonOtherPlatform->cbGender;
	LogonOtherPlatform.cbPlatformID=pLogonOtherPlatform->cbPlatformID;
	lstrcpyn(LogonOtherPlatform.szAgentID,pLogonOtherPlatform->szAgentID,CountArray(LogonOtherPlatform.szAgentID));
	lstrcpyn(LogonOtherPlatform.szUserUin,pLogonOtherPlatform->szUserUin,CountArray(LogonOtherPlatform.szUserUin));
	lstrcpyn(LogonOtherPlatform.szNickName,pLogonOtherPlatform->szNickName,CountArray(LogonOtherPlatform.szNickName));
	lstrcpyn(LogonOtherPlatform.szMachineID,pLogonOtherPlatform->szMachineID,CountArray(LogonOtherPlatform.szMachineID));
	lstrcpyn(LogonOtherPlatform.szMobilePhone,pLogonOtherPlatform->szMobilePhone,CountArray(LogonOtherPlatform.szMobilePhone));
	lstrcpyn(LogonOtherPlatform.szCompellation,pLogonOtherPlatform->szCompellation,CountArray(LogonOtherPlatform.szCompellation));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_LOGON_OTHERPLATFORM,dwSocketID,&LogonOtherPlatform,sizeof(LogonOtherPlatform));

	return true;
}

//�ο͵�¼
bool CAttemperEngineSink::OnTCPNetworkSubMBLogonVisitor(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_MB_LogonVisitor));
	if (wDataSize<sizeof(CMD_MB_LogonVisitor))return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_MB_LogonVisitor * pLogonVisitor=(CMD_MB_LogonVisitor *)pData;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pLogonVisitor->wModuleID;
	pBindParameter->dwPlazaVersion=pLogonVisitor->dwPlazaVersion;

	//�汾�ж�
	if (CheckPlazaVersion(pLogonVisitor->cbDeviceType,pLogonVisitor->dwPlazaVersion,dwSocketID)==false)
	{
		return true;
	}

	//��������
	DBR_MB_LogonVisitor LogonVisitor;
	ZeroMemory(&LogonVisitor,sizeof(LogonVisitor));

	//������Ϣ
	LogonVisitor.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	LogonVisitor.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	LogonVisitor.cbPlatformID=ULMByVisitor;
	lstrcpyn(LogonVisitor.szAgentID,pLogonVisitor->szAgentID,CountArray(LogonVisitor.szAgentID));
	lstrcpyn(LogonVisitor.szMachineID,pLogonVisitor->szMachineID,CountArray(LogonVisitor.szMachineID));
	lstrcpyn(LogonVisitor.szMobilePhone,pLogonVisitor->szMobilePhone,CountArray(LogonVisitor.szMobilePhone));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_LOGON_VISITOR,dwSocketID,&LogonVisitor,sizeof(LogonVisitor));

	return true;
}

bool CAttemperEngineSink::OnTCPNetworkSubMBQueryGameLockInfo(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_MB_QueryGameLockInfo));
	if (wDataSize<sizeof(CMD_MB_QueryGameLockInfo))return false;

	//������Ϣ
	CMD_MB_QueryGameLockInfo * pClientData=(CMD_MB_QueryGameLockInfo *)pData;

	//��������
	DBR_MB_QueryGameLockInfo DBRData;
	ZeroMemory(&DBRData,sizeof(DBRData));

	//��������
	DBRData.dwUserID = pClientData->dwUserID;

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_QUERY_GAMELOCKINFO,dwSocketID,&DBRData,sizeof(DBRData));

	return true;
}

//��ȡ��������
bool CAttemperEngineSink::OnTCPNetworkSubMBGetOnline(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��������
	CMD_MB_GetOnline * pGetOnline=(CMD_MB_GetOnline *)pData;
	WORD wHeadSize=(sizeof(CMD_MB_GetOnline)-sizeof(pGetOnline->wOnLineServerID));

	//Ч������
	ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGetOnline->wServerCount*sizeof(WORD))));
	if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGetOnline->wServerCount*sizeof(WORD)))) return false;

	//��������
	CMD_MB_KindOnline KindOnline;
	CMD_MB_ServerOnline ServerOnline;
	ZeroMemory(&KindOnline,sizeof(KindOnline));
	ZeroMemory(&ServerOnline,sizeof(ServerOnline));

	//��ȡ����
	POSITION KindPosition=NULL;
	do
	{
		//��ȡ����
		CGameKindItem * pGameKindItem=m_ServerListManager.EmunGameKindItem(KindPosition);

		//���ñ���
		if (pGameKindItem!=NULL)
		{
			WORD wKindIndex=KindOnline.wKindCount++;
			KindOnline.OnLineInfoKind[wKindIndex].wKindID=pGameKindItem->m_GameKind.wKindID;
			KindOnline.OnLineInfoKind[wKindIndex].dwOnLineCount=pGameKindItem->m_GameKind.dwOnLineCount+pGameKindItem->m_GameKind.dwAndroidCount;
		}

		//����ж�
		if (KindOnline.wKindCount>=CountArray(KindOnline.OnLineInfoKind))
		{
			ASSERT(FALSE);
			break;
		}

	} while (KindPosition!=NULL);

	//��ȡ����
	for (WORD i=0;i<pGetOnline->wServerCount;i++)
	{
		//��ȡ����
		WORD wServerID=pGetOnline->wOnLineServerID[i];
		CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(wServerID);

		//���ñ���
		if (pGameServerItem!=NULL)
		{
			WORD wServerIndex=ServerOnline.wServerCount++;
			ServerOnline.OnLineInfoServer[wServerIndex].wServerID=wServerID;
			ServerOnline.OnLineInfoServer[wServerIndex].dwOnLineCount=pGameServerItem->m_GameServer.dwOnLineCount+pGameServerItem->m_GameServer.dwAndroidCount;
		}

		//����ж�
		if (ServerOnline.wServerCount>=CountArray(ServerOnline.OnLineInfoServer))
		{
			ASSERT(FALSE);
			break;
		}
	}

	//��������
	if (KindOnline.wKindCount>0)
	{
		WORD wHeadSize=sizeof(KindOnline)-sizeof(KindOnline.OnLineInfoKind);
		WORD wSendSize=wHeadSize+KindOnline.wKindCount*sizeof(KindOnline.OnLineInfoKind[0]);
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_KINE_ONLINE,&KindOnline,wSendSize);
	}

	//��������
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
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_MB_QueryServer));
	if (wDataSize!=sizeof(CMD_MB_QueryServer))return false;
	CMD_MB_QueryServer * pClientData = (CMD_MB_QueryServer*)pData;
	m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_SERVER_START,0,0);
	SendMobileServerInfo(dwSocketID, pClientData->wKindID);
	m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_SERVER_END,0,0);
	return true;
}

//ƽ̨����
bool CAttemperEngineSink::OnDBPCPlatformParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//��������
	DBO_GP_PlatformParameter * pPlatformParameter=(DBO_GP_PlatformParameter *)pData;

	//���ñ���
	m_PlatformParameter.dwExchangeRate = pPlatformParameter->dwExchangeRate;
	m_PlatformParameter.dwPresentExchangeRate = pPlatformParameter->dwPresentExchangeRate;
	m_PlatformParameter.dwRateGold = pPlatformParameter->dwRateGold;

	return true;
}

//�û�ͷ��
bool CAttemperEngineSink::OnDBPCUserFaceInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_UserFaceInfo UserFaceInfo;
	ZeroMemory(&UserFaceInfo,sizeof(UserFaceInfo));
	DBO_GP_UserFaceInfo * pUserFaceInfo=(DBO_GP_UserFaceInfo *)pData;

	//���ñ���
	UserFaceInfo.bSuccess = pUserFaceInfo->bSuccess;
	UserFaceInfo.wFaceID=pUserFaceInfo->wFaceID;

	lstrcpyn(UserFaceInfo.szDescribeString, pUserFaceInfo->szDescribeString, CountArray(UserFaceInfo.szDescribeString));

	//������Ϣ
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_FACE_INFO,&UserFaceInfo,sizeof(UserFaceInfo));

	return true;
}

//������Ϣ
bool CAttemperEngineSink::OnDBPCUserInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_UserInsureInfo * pUserInsureInfo=(DBO_GP_UserInsureInfo *)pData;

	//��������
	CMD_GP_UserInsureInfo UserInsureInfo;
	ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

	//��������
	UserInsureInfo.cbEnjoinTransfer=pUserInsureInfo->cbEnjoinTransfer;
	UserInsureInfo.wRevenueTake=pUserInsureInfo->wRevenueTake;
	UserInsureInfo.wRevenueTransfer=pUserInsureInfo->wRevenueTransfer;
	UserInsureInfo.wRevenueTransferMember=pUserInsureInfo->wRevenueTransferMember;
	UserInsureInfo.wServerID=pUserInsureInfo->wServerID;
	UserInsureInfo.lUserScore=pUserInsureInfo->lUserScore;
	UserInsureInfo.lUserInsure=pUserInsureInfo->lUserInsure;
	UserInsureInfo.lTransferPrerequisite=pUserInsureInfo->lTransferPrerequisite;

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_INFO,&UserInsureInfo,sizeof(UserInsureInfo));

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//��Ǯ���
bool CAttemperEngineSink::OnDBPCUserSaveResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter + LOWORD(dwContextID))->dwSocketID != dwContextID) return true;

	//��������
	DBO_GP_UserSaveResult * pUserSaveResult = (DBO_GP_UserSaveResult *)pData;

	//��������
	CMD_GP_UserSaveResult UserSaveResult;
	ZeroMemory(&UserSaveResult, sizeof(UserSaveResult));

	//���ñ���
	UserSaveResult.bSuccess = pUserSaveResult->bSuccess;
	UserSaveResult.lUserScore = pUserSaveResult->lScore;
	UserSaveResult.lUserInsure = pUserSaveResult->lInsureScore;
	lstrcpyn(UserSaveResult.szDescribeString, pUserSaveResult->szDescribeString, CountArray(UserSaveResult.szDescribeString));

	//������Ϣ
	WORD wDescribe = CountStringBuffer(UserSaveResult.szDescribeString);
	WORD wHeadSize = sizeof(UserSaveResult) - sizeof(UserSaveResult.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GP_USER_SAVE_RESULT, &UserSaveResult, wHeadSize + wDescribe);

	return true;
}

//ȡǮ���
bool CAttemperEngineSink::OnDBPCUserTakeResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter + LOWORD(dwContextID))->dwSocketID != dwContextID) return true;

	//��������
	DBO_GP_UserTakeResult * pUserTakeResult = (DBO_GP_UserTakeResult *)pData;

	//��������
	CMD_GP_UserTakeResult UserTakeResult;
	ZeroMemory(&UserTakeResult, sizeof(UserTakeResult));

	//���ñ���
	UserTakeResult.bSuccess = pUserTakeResult->bSuccess;
	UserTakeResult.lUserScore = pUserTakeResult->lScore;
	UserTakeResult.lUserInsure = pUserTakeResult->lInsureScore;
	lstrcpyn(UserTakeResult.szDescribeString, pUserTakeResult->szDescribeString, CountArray(UserTakeResult.szDescribeString));

	//������Ϣ
	WORD wDescribe = CountStringBuffer(UserTakeResult.szDescribeString);
	WORD wHeadSize = sizeof(UserTakeResult) - sizeof(UserTakeResult.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GP_USER_TAKE_RESULT, &UserTakeResult, wHeadSize + wDescribe);

	return true;
}

//���гɹ�
bool CAttemperEngineSink::OnDBPCUserInsureSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_UserInsureSuccess * pUserInsureSuccess=(DBO_GP_UserInsureSuccess *)pData;

	//��������
	CMD_GP_UserInsureSuccess UserInsureSuccess;
	ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

	//���ñ���
	UserInsureSuccess.dwUserID=pUserInsureSuccess->dwUserID;
	UserInsureSuccess.lUserScore=pUserInsureSuccess->lSourceScore+pUserInsureSuccess->lVariationScore;
	UserInsureSuccess.lUserInsure=pUserInsureSuccess->lSourceInsure+pUserInsureSuccess->lVariationInsure;
	lstrcpyn(UserInsureSuccess.szDescribeString,pUserInsureSuccess->szDescribeString,CountArray(UserInsureSuccess.szDescribeString));

	//������Ϣ
	WORD wDescribe=CountStringBuffer(UserInsureSuccess.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureSuccess)-sizeof(UserInsureSuccess.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_SUCCESS,&UserInsureSuccess,wHeadSize+wDescribe);

	return true;
}

//����ʧ��
bool CAttemperEngineSink::OnDBPCUserInsureFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_UserInsureFailure UserInsureFailure;
	ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

	//��������
	DBO_GP_UserInsureFailure * pUserInsureFailure=(DBO_GP_UserInsureFailure *)pData;

	//��������
	UserInsureFailure.lResultCode=pUserInsureFailure->lResultCode;
	lstrcpyn(UserInsureFailure.szDescribeString,pUserInsureFailure->szDescribeString,CountArray(UserInsureFailure.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(UserInsureFailure.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_FAILURE,&UserInsureFailure,wHeadSize+wDescribe);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//�û���Ϣ
bool CAttemperEngineSink::OnDBPCUserInsureUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_UserTransferUserInfo * pTransferUserInfo=(DBO_GP_UserTransferUserInfo *)pData;

	//��������
	CMD_GP_UserTransferUserInfo UserTransferUserInfo;
	ZeroMemory(&UserTransferUserInfo,sizeof(UserTransferUserInfo));

	//�������
	UserTransferUserInfo.dwTargetGameID=pTransferUserInfo->dwGameID;
	lstrcpyn(UserTransferUserInfo.szAccounts,pTransferUserInfo->szAccounts,CountArray(UserTransferUserInfo.szAccounts));

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_USER_INFO_RESULT,&UserTransferUserInfo,sizeof(UserTransferUserInfo));

	return true;
}

//��ͨ���
bool CAttemperEngineSink::OnDBPCUserInsureEnableResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_UserInsureEnableResult * pUserInsureEnableResult=(DBO_GP_UserInsureEnableResult *)pData;

	//��������
	CMD_GP_UserInsureEnableResult UserInsureEnableResult;
	ZeroMemory(&UserInsureEnableResult,sizeof(UserInsureEnableResult));

	//�������
	UserInsureEnableResult.cbInsureEnabled=pUserInsureEnableResult->cbInsureEnabled;
	lstrcpyn(UserInsureEnableResult.szDescribeString,pUserInsureEnableResult->szDescribeString,CountArray(UserInsureEnableResult.szDescribeString));

	//��������
	WORD wHeadSize=CountStringBuffer(UserInsureEnableResult.szDescribeString);
	wHeadSize +=sizeof(UserInsureEnableResult)-sizeof(UserInsureEnableResult.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_ENABLE_RESULT,&UserInsureEnableResult,wHeadSize);

	return true;
}

//�ͱ�����
bool CAttemperEngineSink::OnDBPCBaseEnsureParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_BaseEnsureParameter * pEnsureParameter=(DBO_GP_BaseEnsureParameter *)pData;

	//���ñ���
	m_BaseEnsureParameter.cbTakeTimes = pEnsureParameter->cbTakeTimes;
	m_BaseEnsureParameter.lScoreAmount = pEnsureParameter->lScoreAmount;
	m_BaseEnsureParameter.lScoreCondition = pEnsureParameter->lScoreCondition;

	return true;
}

//�ͱ����
bool CAttemperEngineSink::OnDBPCBaseEnsureResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_BaseEnsureResult * pBaseEnsureResult=(DBO_GP_BaseEnsureResult *)pData;

	//����ṹ
	CMD_GP_BaseEnsureResult BaseEnsureResult;
	BaseEnsureResult.bSuccessed=pBaseEnsureResult->bSuccessed;
	BaseEnsureResult.lGameScore=pBaseEnsureResult->lGameScore;
	BaseEnsureResult.nTakeTimes = pBaseEnsureResult->nTakeTimes;
	lstrcpyn(BaseEnsureResult.szNotifyContent,pBaseEnsureResult->szNotifyContent,CountArray(BaseEnsureResult.szNotifyContent));

	//��������
	WORD wSendDataSize=sizeof(BaseEnsureResult)-sizeof(BaseEnsureResult.szNotifyContent);
	wSendDataSize+=CountStringBuffer(BaseEnsureResult.szNotifyContent);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_BASEENSURE_RESULT,&BaseEnsureResult,wSendDataSize);

	return true;
}

//��������
bool CAttemperEngineSink::OnDBAndroidParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;
	
	//��������
	DBO_GP_AndroidParameter * pAndroidParameter=(DBO_GP_AndroidParameter *)pData;

	//��������
	CMD_GP_AndroidParameter AndroidParameter;
	ZeroMemory(&AndroidParameter,sizeof(AndroidParameter));

	//�������
	AndroidParameter.wSubCommdID=pAndroidParameter->wSubCommdID;
	AndroidParameter.wParameterCount=pAndroidParameter->wParameterCount;
	CopyMemory(AndroidParameter.AndroidParameter,pAndroidParameter->AndroidParameter,sizeof(tagAndroidParameter)*AndroidParameter.wParameterCount);

	//�����С
	WORD wSendDataSize = sizeof(AndroidParameter)-sizeof(AndroidParameter.AndroidParameter);
	wSendDataSize += sizeof(tagAndroidParameter)*AndroidParameter.wParameterCount;

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_ANDROID_SERVICE,SUB_GP_ANDROID_PARAMETER,&AndroidParameter,wSendDataSize);

	//�����ж�
	if(AndroidParameter.wParameterCount==1)
	{
		//Э������
		if(AndroidParameter.wSubCommdID==SUB_GP_ADD_PARAMETER)
		{
			//����ṹ
			CMD_CS_C_AddParameter AddParameter;
			AddParameter.wServerID=pAndroidParameter->wServerID;
			CopyMemory(&AddParameter.AndroidParameter,&AndroidParameter.AndroidParameter[0],sizeof(tagAndroidParameter));

			//��������
			m_pITCPSocketService->SendData(MDM_CS_ANDROID_SERVICE,SUB_CS_C_ADDPARAMETER,&AddParameter,sizeof(AddParameter));
		}

		//Э������
		if(AndroidParameter.wSubCommdID==SUB_GP_MODIFY_PARAMETER)
		{
			//����ṹ
			CMD_CS_C_ModifyParameter ModifyParameter;
			ModifyParameter.wServerID=pAndroidParameter->wServerID;
			CopyMemory(&ModifyParameter.AndroidParameter,&AndroidParameter.AndroidParameter[0],sizeof(tagAndroidParameter));

			//��������
			m_pITCPSocketService->SendData(MDM_CS_ANDROID_SERVICE,SUB_CS_C_MODIFYPARAMETER,&ModifyParameter,sizeof(ModifyParameter));
		}

		//Э������
		if(AndroidParameter.wSubCommdID==SUB_GP_DELETE_PARAMETER)
		{
			//����ṹ
			CMD_CS_C_DeleteParameter DeleteParameter;
			DeleteParameter.wServerID=pAndroidParameter->wServerID;
			DeleteParameter.dwBatchID=AndroidParameter.AndroidParameter[0].dwBatchID;

			//��������
			m_pITCPSocketService->SendData(MDM_CS_ANDROID_SERVICE,SUB_CS_C_DELETEPARAMETER,&DeleteParameter,sizeof(DeleteParameter));
		}
	}

	return true;
}

bool CAttemperEngineSink::OnDBIndividualResult( DWORD dwContextID, VOID * pData, WORD wDataSize )
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��ȡ����
	DBO_GP_IndividualResult * pResult=(DBO_GP_IndividualResult *)pData;

	//����ṹ
	CMD_GP_IndividuaResult currResult;
	ZeroMemory(&currResult,sizeof(currResult));

	//���ñ���
	currResult.bSuccessed=pResult->bSuccessed;
	lstrcpyn(currResult.szNotifyContent,pResult->szDescribeString,CountArray(currResult.szNotifyContent));

	//��������

	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GP_INDIVIDUAL_RESULT, &currResult, sizeof(currResult));

	return true;
}

//�һ���ҽ��
bool CAttemperEngineSink::OnDBMBExchangeScoreResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter + LOWORD(dwContextID))->dwSocketID != dwContextID) return true;

	//��������
	CMD_GP_ExchangeScoreResult ExchangeScoreResult;
	ZeroMemory(&ExchangeScoreResult, sizeof(ExchangeScoreResult));

	//��������
	DBO_GP_ExchangeScoreResult * pExchangeScoreResult = (DBO_GP_ExchangeScoreResult *)pData;

	ExchangeScoreResult.bSuccess = pExchangeScoreResult->bSuccess;
	ExchangeScoreResult.lScore = pExchangeScoreResult->lScore;
	ExchangeScoreResult.lInsureScore = pExchangeScoreResult->lInsureScore;
	//��������
	lstrcpyn(ExchangeScoreResult.szDescribeString, pExchangeScoreResult->szDescribeString, CountArray(ExchangeScoreResult.szDescribeString));

	//��������
	WORD wDescribe = CountStringBuffer(ExchangeScoreResult.szDescribeString);
	WORD wHeadSize = sizeof(ExchangeScoreResult) - sizeof(ExchangeScoreResult.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GP_EXCHANGESCORE_RESULT, &ExchangeScoreResult, wHeadSize + wDescribe);

	return true;
}

//�����ɹ�
bool CAttemperEngineSink::OnDBPCOperateSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_OperateSuccess OperateSuccess;
	ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

	//��������
	DBO_GP_OperateSuccess * pOperateSuccess=(DBO_GP_OperateSuccess *)pData;

	//��������
	lstrcpyn(OperateSuccess.szDescribeString,pOperateSuccess->szDescribeString,CountArray(OperateSuccess.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(OperateSuccess.szDescribeString);
	WORD wHeadSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_OPERATE_SUCCESS,&OperateSuccess,wHeadSize+wDescribe);

	return true;
}

//����ʧ��
bool CAttemperEngineSink::OnDBPCOperateFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_OperateFailure OperateFailure;
	ZeroMemory(&OperateFailure,sizeof(OperateFailure));

	//��������
	DBO_GP_OperateFailure * pOperateFailure=(DBO_GP_OperateFailure *)pData;

	//��������
	lstrcpyn(OperateFailure.szDescribeString,pOperateFailure->szDescribeString,CountArray(OperateFailure.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(OperateFailure.szDescribeString);
	WORD wHeadSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_OPERATE_FAILURE,&OperateFailure,wHeadSize+wDescribe);

	return true;
}

//��¼�ɹ�
bool CAttemperEngineSink::OnDBMBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	DBO_MB_LogonSuccess * pDBOLogonSuccess=(DBO_MB_LogonSuccess *)pData;
	CMD_MB_LogonSuccess * pCMDLogonSuccess=(CMD_MB_LogonSuccess *)cbDataBuffer;

	//�ظ���¼
	CServerUserItem * pServerUserItem = m_ServerUserManager.SearchUserItem(pDBOLogonSuccess->dwUserID);
	if (pServerUserItem != NULL)
	{
		m_ServerUserManager.DeleteUserItem(pDBOLogonSuccess->dwUserID);
	}

	//���Ͷ���
	WORD wHeadSize=sizeof(CMD_MB_LogonSuccess);
	CSendPacketHelper SendPacket(cbDataBuffer+wHeadSize,sizeof(cbDataBuffer)-wHeadSize);

	//���ñ���
	ZeroMemory(pCMDLogonSuccess,sizeof(CMD_MB_LogonSuccess));

	//��������
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


	//�û��ɼ�
	pCMDLogonSuccess->lUserScore=pDBOLogonSuccess->lUserScore;
	pCMDLogonSuccess->lUserInsure=pDBOLogonSuccess->lUserInsure;

	//��չ��Ϣ
	pCMDLogonSuccess->cbInsureEnabled=pDBOLogonSuccess->cbInsureEnabled;
	pCMDLogonSuccess->cbIsAgent=pDBOLogonSuccess->cbIsAgent;
	pCMDLogonSuccess->cbMoorMachine=pDBOLogonSuccess->cbMoorMachine;
	pCMDLogonSuccess->TodayAlmsCount = pDBOLogonSuccess->TodayAlmsCount;
	pCMDLogonSuccess->dwLockServerID = pDBOLogonSuccess->dwLockServerID;
	pCMDLogonSuccess->dwKindID = pDBOLogonSuccess->dwKindID;

	//�û�����
	tagFriendUserInfo pFriendUserInfo;
	ZeroMemory(&pFriendUserInfo, sizeof(pFriendUserInfo));

	//��������
	pFriendUserInfo.dwUserID = pDBOLogonSuccess->dwUserID;
	pFriendUserInfo.dwGameID = pDBOLogonSuccess->dwGameID;
	lstrcpyn(pFriendUserInfo.szNickName, pDBOLogonSuccess->szNickName, CountArray(pFriendUserInfo.szNickName));
	pFriendUserInfo.cbGender = pDBOLogonSuccess->cbGender;
	pFriendUserInfo.dwFaceID = pDBOLogonSuccess->wFaceID;
	pFriendUserInfo.dwCustomID = pDBOLogonSuccess->dwCustomID;

	//�û�״̬
	pFriendUserInfo.cbMainStatus = FRIEND_US_ONLINE;
	pFriendUserInfo.cbGameStatus = US_NULL;
	pFriendUserInfo.wKindID = INVALID_KIND;
	pFriendUserInfo.wServerID = INVALID_SERVER;
	pFriendUserInfo.wTableID = INVALID_TABLE;
	pFriendUserInfo.wChairID = INVALID_CHAIR;
	ZeroMemory(pFriendUserInfo.szServerName, CountArray(pFriendUserInfo.szServerName));

	//��������
	tagInsertLocalUserInfo InsertLocalUserInfo;
	InsertLocalUserInfo.dwSocketID = dwContextID;
	InsertLocalUserInfo.dwLogonTime = GetTickCount();

	//�����û�
	m_ServerUserManager.InsertLocalUserItem(InsertLocalUserInfo, pFriendUserInfo, pDBOLogonSuccess->szDynamicPass);

	//��¼�ɹ�
	WORD wSendSize=SendPacket.GetDataSize()+sizeof(CMD_MB_LogonSuccess);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_LOGON,SUB_MB_LOGON_SUCCESS,cbDataBuffer,wSendSize);


	//���ͷ���
	WORD wIndex=LOWORD(dwContextID);
	SendMobileKindInfo(dwContextID);
	SendMobileServerInfo(dwContextID, INVALID_KIND);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_SERVER_LIST,SUB_MB_LIST_FINISH);

	return true;
}

//��¼ʧ��
bool CAttemperEngineSink::OnDBMBLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_MB_LogonFailure LogonFailure;
	ZeroMemory(&LogonFailure,sizeof(LogonFailure));
	DBO_MB_LogonFailure * pLogonFailure=(DBO_MB_LogonFailure *)pData;

	//��������
	LogonFailure.lResultCode=pLogonFailure->lResultCode;
	lstrcpyn(LogonFailure.szDescribeString,pLogonFailure->szDescribeString,CountArray(LogonFailure.szDescribeString));

	//��������
	WORD wStringSize=CountStringBuffer(LogonFailure.szDescribeString);
	WORD wSendSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString)+wStringSize;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_LOGON,SUB_MB_LOGON_FAILURE,&LogonFailure,wSendSize);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//�齱����
bool CAttemperEngineSink::OnDBPCLotteryConfig(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_LotteryConfig * pLotteryConfig=(DBO_GP_LotteryConfig *)pData;

	//��������
	CMD_GP_LotteryConfig LotteryConfig;
	ZeroMemory(&LotteryConfig,sizeof(LotteryConfig));

	//�������
	LotteryConfig.wLotteryCount=pLotteryConfig->wLotteryCount;
	CopyMemory(LotteryConfig.LotteryItem,pLotteryConfig->LotteryItem,sizeof(LotteryConfig.LotteryItem));

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_LOTTERY_CONFIG,&LotteryConfig,sizeof(LotteryConfig));

	return true;
}

//�齱��Ϣ
bool CAttemperEngineSink::OnDBPCLotteryUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_LotteryUserInfo * pLotteryConfig=(DBO_GP_LotteryUserInfo *)pData;

	//��������
	CMD_GP_LotteryUserInfo LotteryUserInfo;
	ZeroMemory(&LotteryUserInfo,sizeof(LotteryUserInfo));

	//�������
	LotteryUserInfo.cbFreeCount=pLotteryConfig->cbFreeCount;
	LotteryUserInfo.cbAlreadyCount=pLotteryConfig->cbAlreadyCount;
	LotteryUserInfo.wKindID=pLotteryConfig->wKindID;
	LotteryUserInfo.dwUserID=pLotteryConfig->dwUserID;
	LotteryUserInfo.lChargeFee=pLotteryConfig->lChargeFee;

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_LOTTERY_USER_INFO,&LotteryUserInfo,sizeof(LotteryUserInfo));

	return true;
}

//�齱���
bool CAttemperEngineSink::OnDBPCLotteryResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_LotteryResult * pLotteryResult=(DBO_GP_LotteryResult *)pData;

	//��������
	CMD_GP_LotteryResult LotteryResult;
	ZeroMemory(&LotteryResult,sizeof(LotteryResult));

	//�������
	LotteryResult.bWined=pLotteryResult->bWined;
	LotteryResult.wKindID=pLotteryResult->wKindID;
	LotteryResult.dwUserID=pLotteryResult->dwUserID;
	LotteryResult.lUserScore=pLotteryResult->lUserScore;
	LotteryResult.dUserBeans=pLotteryResult->dUserBeans;
	CopyMemory(&LotteryResult.LotteryItem,&pLotteryResult->LotteryItem,sizeof(LotteryResult.LotteryItem));

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_LOTTERY_RESULT,&LotteryResult,sizeof(LotteryResult));

	return true;
}

//�����б�
bool CAttemperEngineSink::OnDBPCAgentGameList(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(tagAgentGameKind)==0);
	if (wDataSize%sizeof(tagAgentGameKind)!=0) return false;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_SERVER_LIST,SUB_GP_AGENT_KIND,pData,wDataSize);

	return true;
}

//��Ϸ����
bool CAttemperEngineSink::OnDBPCGameTypeItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GP_GameType)==0);
	if (wDataSize%sizeof(DBO_GP_GameType)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GameType);
	DBO_GP_GameType * pGameType=(DBO_GP_GameType *)pData;

	//��������
	for (WORD i=0;i<wItemCount;i++)
	{
		//��������
		tagGameType GameType;
		ZeroMemory(&GameType,sizeof(GameType));

		//��������
		GameType.wTypeID=(pGameType+i)->wTypeID;
		GameType.wJoinID=(pGameType+i)->wJoinID;
		GameType.wSortID=(pGameType+i)->wSortID;
		lstrcpyn(GameType.szTypeName,(pGameType+i)->szTypeName,CountArray(GameType.szTypeName));

		//�����б�
		m_ServerListManager.InsertGameType(&GameType);
	}

	return true;
}

//��Ϸ����
bool CAttemperEngineSink::OnDBPCGameKindItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GP_GameKind)==0);
	if (wDataSize%sizeof(DBO_GP_GameKind)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GameKind);
	DBO_GP_GameKind * pGameKind=(DBO_GP_GameKind *)pData;

	//��������
	for (WORD i=0;i<wItemCount;i++)
	{
		//��������
		tagGameKind GameKind;
		ZeroMemory(&GameKind,sizeof(GameKind));

		//��������
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

		//�����б�
		m_ServerListManager.InsertGameKind(&GameKind);
	}

	return true;
}

//��Ϸ�ڵ�
bool CAttemperEngineSink::OnDBPCGameNodeItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GP_GameNode)==0);
	if (wDataSize%sizeof(DBO_GP_GameNode)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GameNode);
	DBO_GP_GameNode * pGameNode=(DBO_GP_GameNode *)pData;

	//��������
	for (WORD i=0;i<wItemCount;i++)
	{
		//��������
		tagGameNode GameNode;
		ZeroMemory(&GameNode,sizeof(GameNode));

		//��������
		GameNode.wKindID=(pGameNode+i)->wKindID;
		GameNode.wJoinID=(pGameNode+i)->wJoinID;
		GameNode.wSortID=(pGameNode+i)->wSortID;
		GameNode.wNodeID=(pGameNode+i)->wNodeID;
		lstrcpyn(GameNode.szNodeName,(pGameNode+i)->szNodeName,CountArray(GameNode.szNodeName));

		//�����б�
		m_ServerListManager.InsertGameNode(&GameNode);
	}

	return true;
}

//��Ϸ����
bool CAttemperEngineSink::OnDBPCGamePageItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GP_GamePage)==0);
	if (wDataSize%sizeof(DBO_GP_GamePage)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GamePage);
	DBO_GP_GamePage * pGamePage=(DBO_GP_GamePage *)pData;

	//��������
	for (WORD i=0;i<wItemCount;i++)
	{
		//��������
		tagGamePage GamePage;
		ZeroMemory(&GamePage,sizeof(GamePage));

		//��������
		GamePage.wKindID=(pGamePage+i)->wKindID;
		GamePage.wNodeID=(pGamePage+i)->wNodeID;
		GamePage.wSortID=(pGamePage+i)->wSortID;
		GamePage.wPageID=(pGamePage+i)->wPageID;
		GamePage.wOperateType=(pGamePage+i)->wOperateType;
		lstrcpyn(GamePage.szDisplayName,(pGamePage+i)->szDisplayName,CountArray(GamePage.szDisplayName));

		//�����б�
		m_ServerListManager.InsertGamePage(&GamePage);
	}

	return true;
}

//��Ϸ�б�
bool CAttemperEngineSink::OnDBPCGameListResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBO_GP_GameListResult));
	if (wDataSize!=sizeof(DBO_GP_GameListResult)) return false;

	//��������
	DBO_GP_GameListResult * pGameListResult=(DBO_GP_GameListResult *)pData;

	//��Ϣ����
	if (pGameListResult->cbSuccess==TRUE)
	{
		//�����б�
		m_ServerListManager.CleanKernelItem();

		//�¼�֪ͨ
		CP_ControlResult ControlResult;
		ControlResult.cbSuccess=ER_SUCCESS;
		SendUIControlPacket(UI_LOAD_DB_LIST_RESULT,&ControlResult,sizeof(ControlResult));

		//����ʱ��
		ASSERT(m_pITimerEngine!=NULL);
		m_pITimerEngine->SetTimer(IDI_LOAD_GAME_LIST,m_pInitParameter->m_wLoadListTime*1000L,1,0);
	}
	else
	{
		//������ʾ
		TCHAR szDescribe[128]=TEXT("");
		_sntprintf(szDescribe,CountArray(szDescribe),TEXT("�������б����ʧ�ܣ�%ld ������¼���"),m_pInitParameter->m_wReLoadListTime);

		//��ʾ��Ϣ
		CTraceService::TraceString(szDescribe,TraceLevel_Warning);

		//����ʱ��
		ASSERT(m_pITimerEngine!=NULL);
		m_pITimerEngine->SetTimer(IDI_LOAD_GAME_LIST,m_pInitParameter->m_wReLoadListTime*1000L,1,0);
	}

	return true;
}

//���������������Ϳ�ʼ
bool CAttemperEngineSink::OnDBServerDummyOnLineBegin(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ͷŵ��ɵ�����
	for (int i = 0; i < m_ServerDummyOnLineArray.GetCount(); ++i)
	{
		tagServerDummyOnLine * pCursor = m_ServerDummyOnLineArray.GetAt(i);
		free(pCursor);
	}
	m_ServerDummyOnLineArray.RemoveAll();
	m_ServerDummyOnLineMap.RemoveAll();
	return true;
}


//������������
bool CAttemperEngineSink::OnDBServerDummyOnLine(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(tagServerDummyOnLine)==0);
	if (wDataSize%sizeof(tagServerDummyOnLine)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(tagServerDummyOnLine);
	tagServerDummyOnLine * pServerDummyOnLine=(tagServerDummyOnLine *)pData;

	tagServerDummyOnLine * pItemCursor = NULL;


	for (WORD i=0;i<wItemCount;i++)
	{
		//��������
		pItemCursor=(pServerDummyOnLine+i);

		tagServerDummyOnLine *pNewDummyOnline = (tagServerDummyOnLine*)malloc(sizeof(tagServerDummyOnLine));
		CopyMemory(pNewDummyOnline, pItemCursor, sizeof(tagServerDummyOnLine));
		m_ServerDummyOnLineArray.Add(pNewDummyOnline);
		//��������
	}

	return true;
}

//���������������ͽ���
bool CAttemperEngineSink::OnDBServerDummyOnLineEnd(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	OnChangeDummyOnline();
	return true;
}

//�汾���
bool CAttemperEngineSink::CheckPlazaVersion(BYTE cbDeviceType, DWORD dwPlazaVersion, DWORD dwSocketID, bool bCheckLowVer)
{
	//��������
	bool bMustUpdate=false;
	bool bAdviceUpdate=false;
	DWORD dwVersion=VERSION_PLAZA;

	//�ֻ��汾
	if(cbDeviceType >= DEVICE_TYPE_IPAD) dwVersion=VERSION_MOBILE_IOS;
	else if(cbDeviceType >= DEVICE_TYPE_IPHONE) dwVersion=VERSION_MOBILE_IOS;
	else if(cbDeviceType >= DEVICE_TYPE_ITOUCH) dwVersion=VERSION_MOBILE_IOS;
	else if(cbDeviceType >= DEVICE_TYPE_ANDROID) dwVersion=VERSION_MOBILE_ANDROID;
	else if(cbDeviceType == DEVICE_TYPE_PC) dwVersion=VERSION_PLAZA;

	//�汾�ж�
	if (bCheckLowVer && GetSubVer(dwPlazaVersion)<GetSubVer(dwVersion)) bAdviceUpdate=true;
	if (GetMainVer(dwPlazaVersion)!=GetMainVer(dwVersion)) bMustUpdate=true;
	if (GetProductVer(dwPlazaVersion)!=GetProductVer(dwVersion)) bMustUpdate=true;

	//�����ж�
	if ((bMustUpdate==true)||(bAdviceUpdate==true))
	{
		//��������
		CMD_GP_UpdateNotify UpdateNotify;
		ZeroMemory(&UpdateNotify,sizeof(UpdateNotify));

		//��������
		UpdateNotify.cbMustUpdate=bMustUpdate;
		UpdateNotify.cbAdviceUpdate=bAdviceUpdate;
		UpdateNotify.dwCurrentVersion=dwVersion;

		//������Ϣ
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_LOGON,SUB_GP_UPDATE_NOTIFY,&UpdateNotify,sizeof(UpdateNotify));

		//�ж��ж�
		if (bMustUpdate==true) 
		{
			m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);
			return false;
		}
	}

	return true;
}

//��������
bool CAttemperEngineSink::SendUIControlPacket(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	//��������
	CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
	pServiceUnits->PostControlRequest(wRequestID,pData,wDataSize);

	return true;
}

//��������
VOID CAttemperEngineSink::SendGameTypeInfo(DWORD dwSocketID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameTypeItem * pGameTypeItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameTypeCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameType))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_TYPE,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameTypeItem=m_ServerListManager.EmunGameTypeItem(Position);
		if (pGameTypeItem==NULL) break;

		//��������
		CopyMemory(cbDataBuffer+wSendSize,&pGameTypeItem->m_GameType,sizeof(tagGameType));
		wSendSize+=sizeof(tagGameType);
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_TYPE,cbDataBuffer,wSendSize);

	return;
}

//��������
VOID CAttemperEngineSink::SendGameKindInfo(DWORD dwSocketID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameKindItem * pGameKindItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameKindCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameKind))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_KIND,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameKindItem=m_ServerListManager.EmunGameKindItem(Position);
		if (pGameKindItem==NULL) break;

		//��������
		CopyMemory(cbDataBuffer+wSendSize,&pGameKindItem->m_GameKind,sizeof(tagGameKind));
		wSendSize+=sizeof(tagGameKind);
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_KIND,cbDataBuffer,wSendSize);

	return;
}

//���ͽڵ�
VOID CAttemperEngineSink::SendGameNodeInfo(DWORD dwSocketID, WORD wKindID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameNodeItem * pGameNodeItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameNodeCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameNode))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_NODE,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameNodeItem=m_ServerListManager.EmunGameNodeItem(Position);
		if (pGameNodeItem==NULL) break;

		//��������
		if ((wKindID==INVALID_WORD)||(pGameNodeItem->m_GameNode.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameNodeItem->m_GameNode,sizeof(tagGameNode));
			wSendSize+=sizeof(tagGameNode);
		}
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_NODE,cbDataBuffer,wSendSize);

	return;
}

//���Ͷ���
VOID CAttemperEngineSink::SendGamePageInfo(DWORD dwSocketID, WORD wKindID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGamePageItem * pGamePageItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGamePageCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGamePage))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_PAGE,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGamePageItem=m_ServerListManager.EmunGamePageItem(Position);
		if (pGamePageItem==NULL) break;

		//��������
		if ((wKindID==INVALID_WORD)||(pGamePageItem->m_GamePage.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGamePageItem->m_GamePage,sizeof(tagGamePage));
			wSendSize+=sizeof(tagGamePage);
		}
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_PAGE,cbDataBuffer,wSendSize);

	return;
}

//���ͷ���
VOID CAttemperEngineSink::SendGameServerInfo(DWORD dwSocketID, WORD wKindID, BYTE cbDeviceType)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameServerItem * pGameServerItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameServerCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameServer))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_SERVER,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameServerItem=m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;
		//֧������
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

		//��������
		if ((wKindID==INVALID_WORD)||(pGameServerItem->m_GameServer.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameServerItem->m_GameServer,sizeof(tagGameServer));
			wSendSize+=sizeof(tagGameServer);
		}
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_SERVER,cbDataBuffer,wSendSize);
	//���ñ���
	wSendSize=0;
	ZeroMemory(cbDataBuffer,sizeof(cbDataBuffer));

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameServerCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameMatch))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_MATCH,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameServerItem=m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;
		if (pGameServerItem->IsMatchServer()==false) continue;

		//��������
		if ((wKindID==INVALID_WORD)||(pGameServerItem->m_GameServer.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameServerItem->m_GameMatch,sizeof(tagGameMatch));
			wSendSize+=sizeof(tagGameMatch);
		}
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_MATCH,cbDataBuffer,wSendSize);

	if(m_wAVServerPort!=0 && m_dwAVServerAddr!=0)
	{
		//��������
		tagAVServerOption AVServerOption;
		AVServerOption.wAVServerPort=m_wAVServerPort;
		AVServerOption.dwAVServerAddr=m_dwAVServerAddr;

		//��������
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_VIDEO_OPTION,&AVServerOption,sizeof(AVServerOption));
	};

	return;
}

//��������
VOID CAttemperEngineSink::SendMobileKindInfo(DWORD dwSocketID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameKindItem * pGameKindItem=NULL;
	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameKindCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameKind))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_KIND,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameKindItem=m_ServerListManager.EmunGameKindItem(Position);
		if (pGameKindItem==NULL) break;

		CopyMemory(cbDataBuffer+wSendSize,&pGameKindItem->m_GameKind,sizeof(tagGameKind));
		wSendSize+=sizeof(tagGameKind);
	}

	//����ʣ��
	if (wSendSize>0) 
	{
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_KIND,cbDataBuffer,wSendSize);
	}

	return;
}

//���ͷ���
VOID CAttemperEngineSink::SendMobileServerInfo(DWORD dwSocketID, WORD wKindID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameServerItem * pGameServerItem=NULL;
	CGameKindItem * pGameKindItem=NULL;
	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameServerCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameServer))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_SERVER,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameServerItem=m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;



		//֧������
		bool bServerSuportMobile = CServerRule::IsSuportMobile(pGameServerItem->m_GameServer.dwServerRule);

		//��������
		if ((bServerSuportMobile==true)&&((wKindID==INVALID_WORD)||(pGameServerItem->m_GameServer.wKindID==wKindID)))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameServerItem->m_GameServer,sizeof(tagGameServer));
			wSendSize+=sizeof(tagGameServer);
		}
	}

	//����ʣ��
	if (wSendSize>0) 
	{
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_SERVER,cbDataBuffer,wSendSize);
	}

	//���ñ���
	wSendSize=0;
	ZeroMemory(cbDataBuffer,sizeof(cbDataBuffer));

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameServerCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameMatch))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_MATCH,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameServerItem=m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;
		if (pGameServerItem->IsMatchServer()==false) continue;

		//��������
		if ((wKindID==INVALID_WORD)||(pGameServerItem->m_GameServer.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameServerItem->m_GameMatch,sizeof(tagGameMatch));
			wSendSize+=sizeof(tagGameMatch);
		}
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_MATCH,cbDataBuffer,wSendSize);

	return;
}

//����ʧ��
bool CAttemperEngineSink::SendInsureFailure(DWORD dwSocketID, LONG lResultCode, LPCTSTR pszDescribe)
{
	//��������
	CMD_GP_UserInsureFailure UserInsureFailure;
	ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

	//��������
	UserInsureFailure.lResultCode=lResultCode;
	lstrcpyn(UserInsureFailure.szDescribeString,pszDescribe,CountArray(UserInsureFailure.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(UserInsureFailure.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_FAILURE,&UserInsureFailure,wHeadSize+wDescribe);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);

	return true;
}

//�����ɹ�
VOID CAttemperEngineSink::SendOperateSuccess(DWORD dwContextID, LONG lResultCode, LPCTSTR pszDescribe)
{
	//Ч�����
	ASSERT(pszDescribe != NULL);
	if(pszDescribe == NULL) return;

	//��������
	CMD_GP_OperateSuccess OperateSuccess;
	ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

	//��������
	lstrcpyn(OperateSuccess.szDescribeString,pszDescribe,CountArray(OperateSuccess.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(OperateSuccess.szDescribeString);
	WORD wHeadSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_OPERATE_SUCCESS,&OperateSuccess,wHeadSize+wDescribe);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return;
}

//����ʧ��
VOID CAttemperEngineSink::SendOperateFailure(DWORD dwContextID, LONG lResultCode, LPCTSTR pszDescribe)
{
	//Ч�����
	ASSERT(pszDescribe != NULL);
	if(pszDescribe == NULL) return;

	//��������
	CMD_GP_OperateFailure OperateFailure;
	ZeroMemory(&OperateFailure,sizeof(OperateFailure));

	//��������
	lstrcpyn(OperateFailure.szDescribeString,pszDescribe,CountArray(OperateFailure.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(OperateFailure.szDescribeString);
	WORD wHeadSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_OPERATE_FAILURE,&OperateFailure,wHeadSize+wDescribe);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return;
}

VOID CAttemperEngineSink::OnChangeDummyOnline()
{
	////�������
	tagServerDummyOnLine * pItemCursor = NULL;


	SYSTEMTIME tempDate;
	GetSystemTime(&tempDate);

	//��ֵ���Ǳ���ʱ��
	tempDate.wYear = 1970;
	tempDate.wMonth = 1;
	tempDate.wDayOfWeek = 1;
	tempDate.wDay = 1;
	tempDate.wMinute;
	tempDate.wHour;

	time_t curTime = SystemTimeToTimet(tempDate);

	//��������
	srand(time(NULL));
	//iMaxAccCount ��ֵԽ�󣬲�������ԽС, iRetifyCount Խ�󣬲�������Խ�� iMaxAccCount > iRetifyCount
	int iMaxAccCount = 5;
	int iRetifyCount = 3;
	//printf("----------------------start update dummy onLine\n");

	for (int i=0;i<m_ServerDummyOnLineArray.GetCount();i++)
	{
		//��������
		pItemCursor=m_ServerDummyOnLineArray.GetAt(i);

		//��ʼ��ֵ
		pItemCursor->dwCurDummyOnline = 0;

		//ʱ����
		int timeSpecStart = (curTime - pItemCursor->lTimeStart);
		int timeSpecEnd = (curTime - pItemCursor->lTimeEnd);

		if (timeSpecStart > 0 && timeSpecEnd < 0)
		{//��ָ��ʱ���ʹ�ñ仯ֵ
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
				//n+1�׵�FIR�˲��� ����ƽ������
				tagServerDummyOnLine * pItemLast = m_ServerDummyOnLineMap[pItemCursor->wServerID];
				if (pItemLast == NULL)
				{
					m_ServerDummyOnLineMap[pItemCursor->wServerID] = pItemCursor;
					//printf("��һ�λ�ȡ�����Map\n");
				}
				else
				{
					if (pItemLast != pItemCursor)
					{
						//��ȡ�ϴε��ۻ��������ۼ���
						pItemCursor->lAccumulationCount = pItemLast->lAccumulationCount;
						pItemCursor->lAccumulation = pItemLast->lAccumulation;
						m_ServerDummyOnLineMap[pItemCursor->wServerID] = pItemCursor;
					}
				}

				//printf(":ServerID��%d, ��ǰʱ���������Χ:%d-%d\n", pItemCursor->wServerID, pItemCursor->dwDummyMinOnLine, pItemCursor->dwDummyMaxOnLine);
				int nRand = pItemCursor->dwDummyMinOnLine + rand() % (pItemCursor->dwDummyMaxOnLine - pItemCursor->dwDummyMinOnLine);
				//printf(":ServerID��%d, ��ǰʱ��������:%d\n", pItemCursor->wServerID, nRand);

				//printf(":ServerID��%d, ��ǰʱ����ۻ���ǰ:%I64d���ۼ�����:%I64d\n", pItemCursor->wServerID, pItemCursor->lAccumulation, pItemCursor->lAccumulationCount);
				pItemCursor->lAccumulationCount = m_ServerDummyOnLineMap[pItemCursor->wServerID]->lAccumulationCount + 1;
				pItemCursor->lAccumulation = m_ServerDummyOnLineMap[pItemCursor->wServerID]->lAccumulation + nRand;
				//printf(":ServerID��%d, ��ǰʱ����ۻ�����:%I64d���ۼ�����:%I64d\n", pItemCursor->wServerID, pItemCursor->lAccumulation, pItemCursor->lAccumulationCount);
				//ƽ������ֵ
				int iAve = pItemCursor->lAccumulation / pItemCursor->lAccumulationCount;
				pItemCursor->dwCurDummyOnline = iAve;
				//printf(":ServerID��%d, ��ǰʱ��ξ�ֵ:%d\n", pItemCursor->wServerID, iAve);
			}

			if (pItemCursor->lAccumulationCount >= iMaxAccCount)
			{
				pItemCursor->lAccumulation -= pItemCursor->dwCurDummyOnline * iRetifyCount;
				pItemCursor->lAccumulationCount -= iRetifyCount;
				//printf(":ServerID��%d, �ﵽ����ۼ����������ۼ���:%I64d,�ۻ���:%I64d\n", pItemCursor->wServerID, pItemCursor->lAccumulation, pItemCursor->lAccumulationCount);
			}
		}
		//��������
	}


	//��Э��ͬ��������������
	WORD wPacketSize=0L;
	POSITION Position=m_ServerDummyOnLineMap.GetStartPosition();
	BYTE cbBuffer[SOCKET_TCP_PACKET];

	//������Ϣ
	m_pITCPSocketService->SendData(MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_DUMMY_INFO_START);

	//�ռ�����
	while (Position!=NULL)
	{
		//��������
		if ((wPacketSize+sizeof(CMD_CS_C_DummyOnLine))>sizeof(cbBuffer))
		{
			m_pITCPSocketService->SendData(MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_DUMMY_INFO,cbBuffer,wPacketSize);
			wPacketSize=0;
		}

		//��ȡ����
		CMD_CS_C_DummyOnLine * pDummyOnLine=(CMD_CS_C_DummyOnLine *)(cbBuffer+wPacketSize);


		tagServerDummyOnLine * pDummyOnline = NULL;
		WORD wServerID;
		//��ȡ����
		m_ServerDummyOnLineMap.GetNextAssoc(Position,wServerID,pDummyOnline);

		//��������
		pDummyOnLine->wServerID = wServerID;
		pDummyOnLine->dwDummyCount = pDummyOnline->dwCurDummyOnline;

		//�ۼ�����
		wPacketSize+=sizeof(CMD_CS_C_DummyOnLine);
	};

	//��������
	if (wPacketSize>0)
		m_pITCPSocketService->SendData(MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_DUMMY_INFO,cbBuffer,wPacketSize);

	m_pITCPSocketService->SendData(MDM_CS_SERVICE_INFO,SUB_CS_S_SERVER_DUMMY_INFO_END);
}
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
