
#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
}

//��������
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//�ӿڲ�ѯ
void * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink, Guid, dwQueryVer);
	return NULL;
}

//��ʼ�ӿ�
bool CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pIAndroidUserItem = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IAndroidUserItem);
	if (m_pIAndroidUserItem == NULL) return false;

	return true;
}

//���ýӿ�
bool CAndroidUserItemSink::RepositionSink()
{
	return true;
}

//ʱ����Ϣ
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	return true;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventGameMessage(WORD sub_cmdid, void * data, WORD data_size)
{
	return true;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * data, WORD data_size)
{
	return true;
}
