#ifndef DATABASE_ENGINE_SINK_HEAD_FILE
#define DATABASE_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "DataBasePacket.h"

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//���ݿ���
class CDataBaseEngineSink : public IDataBaseEngineSink
{
	//��Ԫ����
	friend class CServiceUnits;

	//�û����ݿ�
protected:
	CDataBaseAide					m_AccountsDBAide;					//�û����ݿ�
	CDataBaseHelper					m_AccountsDBModule;					//�û����ݿ�

	//��Ϸ�����ݿ�
protected:
	CDataBaseAide					m_TreasureDBAide;					//��Ϸ�����ݿ�
	CDataBaseHelper					m_TreasureDBModule;					//��Ϸ�����ݿ�

	//ƽ̨���ݿ�
protected:
	CDataBaseAide					m_PlatformDBAide;					//ƽ̨���ݿ�
	CDataBaseHelper					m_PlatformDBModule;					//ƽ̨���ݿ�

	//�������
protected:
	CInitParameter *				m_pInitParameter;					//���ò���
	IDataBaseEngineEvent *			m_pIDataBaseEngineEvent;			//�����¼�

	//��������
public:
	//���캯��
	CDataBaseEngineSink();
	//��������
	virtual ~CDataBaseEngineSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//ϵͳ�¼�
public:
	//�����¼�
	virtual bool OnDataBaseEngineStart(IUnknownEx * pIUnknownEx);
	//ֹͣ�¼�
	virtual bool OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx);

	//�ں��¼�
public:
	//ʱ���¼�
	virtual bool OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//�����¼�
	virtual bool OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize);
	//�����¼�
	virtual bool OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�ֻ���¼
protected:
	//I D ��¼
	bool OnMobileLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ʺŵ�¼
	bool OnMobileLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ʺ�ע��
	bool OnMobileRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������¼
	bool OnMobileLogonOtherPlatform(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ο͵�¼
	bool OnMobileLogonVisitor(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�ʺŷ���
protected:
	//�޸�����
	bool OnRequestModifyInsurePass(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸�ͷ��
	bool OnRequestModifySystemFace(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸�����
	bool OnRequestModifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//���з���
protected:
	//��ͨ����
	bool OnRequestUserEnableInsure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������Ϸ��
	bool OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�һ����
	bool OnRequestExchangeBankScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ��Ϸ��
	bool OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ת����Ϸ��
	bool OnRequestUserTransferScore(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//��������
protected:
	//��ȡ����
	bool OnRequestGetParameter(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ӳ���
	bool OnRequestAddParameter(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸Ĳ���
	bool OnRequestModifyParameter(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ɾ������
	bool OnRequestDeleteParameter(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//��Ϣ��ѯ
protected:
	//��ѯ����
	bool OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ�û�
	bool OnRequestQueryTransferUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�ͱ�����
protected:
	//���صͱ�
	bool OnRequestLoadBaseEnsure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ�ͱ�
	bool OnRequestTakeBaseEnsure(DWORD dwContextID, VOID * pData, WORD wDataSize);


	//����������������
	bool OnRequestServerDummyOnLine(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�齱����
protected:
	//��������
	bool OnRequestLotteryConfigReq(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�齱��ʼ
	bool OnRequestLotteryStart(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�ʼ��ͷ�
protected:
	//��ȡ�ʼ�
	bool OnRequestQueryEmail(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//ϵͳ����
protected:
	//�����б�
	bool OnRequestLoadGameList(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������Ϣ
	bool OnRequestOnLineCountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);	
	//ƽ̨����
	bool OnRequestPlatformParameter(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�������
protected:
	//��¼���
	VOID OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString,DWORD dwCheckUserRight,bool bMobileClient);
	//���н��
	VOID OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	//�������
	VOID OnOperateDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient,bool bCloseSocket=true);
	//�������
	VOID OnAndroidDisposeResult(DWORD dwContextID,DWORD dwErrorCode,WORD wSubCommdID,WORD wServerID);
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

#endif