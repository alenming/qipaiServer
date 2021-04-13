#ifndef ATTEMPER_ENGINE_SINK_HEAD_FILE
#define ATTEMPER_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "ServerListManager.h"
#include "CHttpKernel.h"
#include "ServerUserManager.h"

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

time_t SystemTimeToTimet(SYSTEMTIME st);

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//���ݶ���

//��������
#define CLIENT_KIND_FALSH			1									//��ҳ����
#define CLIENT_KIND_MOBILE			2									//�ֻ�����
#define CLIENT_KIND_COMPUTER		3									//��������

//�󶨲���
struct tagBindParameter
{
	//�������
	DWORD							dwSocketID;							//�����ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ
	DWORD							dwActiveTime;						//����ʱ��

	//������Ϣ
	WORD							wModuleID;							//ģ���ʶ
	BYTE							cbClientKind;						//��������
	DWORD							dwPlazaVersion;						//�����汾
};

//ƽ̨����
struct tagPlatformParameter
{
	DWORD							dwExchangeRate;						//�һ�����
	DWORD							dwPresentExchangeRate;				//������Ϸ�Ҷһ���
	DWORD							dwRateGold;							//��Ϸ����Ϸ�Ҷһ���
};


//��֤�����
struct tagVerifyCode
{
	DWORD							dwSocketID;							//������
	DWORD							dwClientAddr;						//�ͻ���IP
	DWORD							dwUpdateTime;						//����ʱ��
	BYTE							cbVerifyCodeType;					//��֤������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//��֤��󶨵��ֻ���
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������
	TCHAR							szValidateCode[LEN_VERIFY_CODE];	//��֤��
};
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

typedef CWHArray<tagVerifyCode *>	CVerifyCodeArray;					//��֤���б�

typedef CMap<WORD,WORD,tagServerDummyOnLine *,tagServerDummyOnLine *> ServerDummyMap;		//��ǰ��Ч��������
typedef CWHArray<tagServerDummyOnLine *>	ServerDummyArray;			//������������
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//���ȹ���
class CAttemperEngineSink : public IAttemperEngineSink
{
	//��Ԫ����
	friend class CServiceUnits;

	//״̬����
protected:
	bool							m_bNeekCorrespond;					//Э����־
	bool                            m_bShowServerStatus;                //��ʾ������״̬
	char							m_szHttpRegisterValidCode[MAX_PATH];//ע��������֤���ַ
	char							m_szHttpModifyPassValidCode[MAX_PATH];//�޸�����������֤���ַ
	char							m_szHttpModifyBankInfoValidCode[MAX_PATH];//�޸�������Ϣ������֤���ַ

	//���ñ���
protected:
	tagPlatformParameter			m_PlatformParameter;				//ƽ̨����
	tagBaseEnsureParameter			m_BaseEnsureParameter;				//�ͱ�����	

	//��������
protected:
	CInitParameter *				m_pInitParameter;					//���ò���
	tagBindParameter *				m_pBindParameter;					//��������
	ServerDummyMap					m_ServerDummyOnLineMap;				//��ǰʱ�����������
	ServerDummyArray				m_ServerDummyOnLineArray;			//����ʱ�����������
	//�������
protected:
	CServerListManager				m_ServerListManager;				//�б����
	CVerifyCodeArray				m_VerifyCodeArray;					//��֤����

	CServerUserManager				m_ServerUserManager;				//�û�����
	
	//����ӿ�
protected:
	ITimerEngine *					m_pITimerEngine;					//ʱ������
	IDataBaseEngine *				m_pIDataBaseEngine;					//��������
	ITCPNetworkEngine *				m_pITCPNetworkEngine;				//��������
	ITCPSocketService *				m_pITCPSocketService;				//Э������
	CHttpKernel<CAttemperEngineSink> *	m_pHttpClientService;			//HTTP����

	//��Ƶ����
protected:
	WORD							m_wAVServerPort;					//��Ƶ�˿�
	DWORD							m_dwAVServerAddr;					//��Ƶ��ַ

	//��������
public:
	//���캯��
	CAttemperEngineSink();
	//��������
	virtual ~CAttemperEngineSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//�첽�ӿ�
public:
	//�����¼�
	virtual bool OnAttemperEngineStart(IUnknownEx * pIUnknownEx);
	//ֹͣ�¼�
	virtual bool OnAttemperEngineConclude(IUnknownEx * pIUnknownEx);

	//HTTP�¼��ӿ�
protected:
	//POST����ص�
	//void OnClientPostCallBack(struct evhttp_request *req, void * pArg);
	//����ע����֤��
	void OnClientGetVerifyCodeCallBack(struct evhttp_request *req, void * pArg);
	//��������ע����֤��
	void OnClientGetReBuildVerifyCodeCallBack(struct evhttp_request *req, void * pArg);

	//�¼��ӿ�
public:
	//�����¼�
	virtual bool OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize);
	//�Զ��¼�
	virtual bool OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize);

	//�ں��¼�
public:
	//ʱ���¼�
	virtual bool OnEventTimer(DWORD dwTimerID, WPARAM wBindParam);
	//���ݿ��¼�
	virtual bool OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�����¼�
public:
	//�����¼�
	virtual bool OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize);

	//�����¼�
public:
	//Ӧ���¼�
	virtual bool OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID);
	//�ر��¼�
	virtual bool OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID);
	//��ȡ�¼�
	virtual bool OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//���Ӵ���
protected:
	//ע���¼�
	bool OnTCPSocketMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������Ϣ
	bool OnTCPSocketMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//Զ�̷���
	bool OnTCPSocketMainRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//�������
	bool OnTCPSocketMainManagerService(WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//�����¼�
protected:
	//�б���
	bool OnTCPNetworkMainPCServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//������
	bool OnTCPNetworkMainPCUserService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//Զ�̴���
	bool OnTCPNetworkMainPCRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//��������
	bool OnTCPNetworkMainAndroidService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�㲥��Ϣ
	bool OnEventTCPSocketSubRadioMessage(VOID * pData, WORD wDataSize);

	//�ֻ��¼�
protected:
	//��¼����
	bool OnTCPNetworkMainMBLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);	
	//�б���
	bool OnTCPNetworkMainMBServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//�ֻ��¼�
protected:
	//I D ��¼
	bool OnTCPNetworkSubMBLogonGameID(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ʺŵ�¼
	bool OnTCPNetworkSubMBLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ʺ�ע��
	bool OnTCPNetworkSubMBRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//������¼
	bool OnTCPNetworkSubMBLogonOtherPlatform(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ο͵�¼
	bool OnTCPNetworkSubMBLogonVisitor(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//��ѯ��Ϸ������Ϣ
	bool OnTCPNetworkSubMBQueryGameLockInfo(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//��ȡ��������
	bool OnTCPNetworkSubMBGetOnline(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//��ѯ������Ϣ
	bool OnTCPNetworkSubMBQueryServer(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//��������
	bool OnTCPNetworkSubMBCreateTable(VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//��Ϸ�¼�
protected:
    //ƽ̨����
	bool OnDBPCPlatformParameter(DWORD dwContextID, VOID * pData, WORD wDataSize);	  
	//�û�ͷ��
	bool OnDBPCUserFaceInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������Ϣ
	bool OnDBPCUserInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ǯ���
	bool OnDBPCUserSaveResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ȡǮ���
	bool OnDBPCUserTakeResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//���гɹ�
	bool OnDBPCUserInsureSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����ʧ��
	bool OnDBPCUserInsureFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û���Ϣ
	bool OnDBPCUserInsureUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ͨ���
	bool OnDBPCUserInsureEnableResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ͱ�����
	bool OnDBPCBaseEnsureParameter(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ͱ����
	bool OnDBPCBaseEnsureResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��������
	bool OnDBAndroidParameter(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//���Ͻ��
	bool OnDBIndividualResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�һ���ҽ��
	bool OnDBMBExchangeScoreResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����ɹ�
	bool OnDBPCOperateSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����ʧ��
	bool OnDBPCOperateFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�齱����
	bool OnDBPCLotteryConfig(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�齱��Ϣ
	bool OnDBPCLotteryUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�齱���
	bool OnDBPCLotteryResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����б�
	bool OnDBPCAgentGameList(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�ֻ��¼�
protected:
	//��¼�ɹ�
	bool OnDBMBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��¼ʧ��
	bool OnDBMBLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�б��¼�
protected:
	//��Ϸ����
	bool OnDBPCGameTypeItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ����
	bool OnDBPCGameKindItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ�ڵ�
	bool OnDBPCGameNodeItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ����
	bool OnDBPCGamePageItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ�б�
	bool OnDBPCGameListResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������������
	bool OnDBServerDummyOnLine(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//���������������Ϳ�ʼ
	bool OnDBServerDummyOnLineBegin(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//���������������ͽ���
	bool OnDBServerDummyOnLineEnd(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��������
protected:
	//�汾���
	bool CheckPlazaVersion(BYTE cbDeviceType, DWORD dwPlazaVersion, DWORD dwSocketID, bool bCheckLowVer=true);
	//��������
	bool SendUIControlPacket(WORD wRequestID, VOID * pData, WORD wDataSize);

	//�����б�
protected:
	//��������
	VOID SendGameTypeInfo(DWORD dwSocketID);
	//��������
	VOID SendGameKindInfo(DWORD dwSocketID);
	//���ͽڵ�
	VOID SendGameNodeInfo(DWORD dwSocketID, WORD wKindID);
	//���Ͷ���
	VOID SendGamePageInfo(DWORD dwSocketID, WORD wKindID);
	////���ͷ���
	//VOID SendGameServerInfo(DWORD dwSocketID, WORD wKindID);
	//���ͷ���
	VOID SendGameServerInfo(DWORD dwSocketID, WORD wKindID, BYTE cbDeviceType);

	//�ֻ��б�
protected:
	//��������
	VOID SendMobileKindInfo(DWORD dwSocketID);
	//���ͷ���
	VOID SendMobileServerInfo(DWORD dwSocketID, WORD wKindID);

	//��������
protected:
	//����ʧ��
	bool SendInsureFailure(DWORD dwSocketID, LONG lResultCode, LPCTSTR pszDescribe);
	//�����ɹ�
	VOID SendOperateSuccess(DWORD dwContextID,LONG lResultCode, LPCTSTR pszDescribe);
	//����ʧ��
	VOID SendOperateFailure(DWORD dwContextID,LONG lResultCode, LPCTSTR pszDescribe);

	//���ܸ�����
protected:
	VOID OnChangeDummyOnline();
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

#endif