#include "Stdafx.h"
#include "LogonServer.h"
#include "LogonServerDlg.h"

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//�������
CLogonServerApp theApp;

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

BEGIN_MESSAGE_MAP(CLogonServerApp, CWinApp)
END_MESSAGE_MAP()

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//���캯��
CLogonServerApp::CLogonServerApp()
{
}

//��������
BOOL CLogonServerApp::InitInstance()
{
	__super::InitInstance();

	//�������
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//����ע���
	SetRegistryKey(szProduct);

	//��ʾ����
	CLogonServerDlg LogonServerDlg;
	m_pMainWnd=&LogonServerDlg;
	LogonServerDlg.DoModal();

	return FALSE;
}

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
