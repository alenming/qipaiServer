#include "Stdafx.h"
#include "ChatServer.h"
#include "ChatServerDlg.h"

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//�������
CChatServerApp theApp;

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

BEGIN_MESSAGE_MAP(CChatServerApp, CWinApp)
END_MESSAGE_MAP()

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//���캯��
CChatServerApp::CChatServerApp()
{
}

//��������
BOOL CChatServerApp::InitInstance()
{
	__super::InitInstance();

	//�������
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//����ע���
	SetRegistryKey(szProduct);

	//��ʾ����
	CChatServerDlg ChatServerDlg;
	m_pMainWnd=&ChatServerDlg;
	ChatServerDlg.DoModal();

	return FALSE;
}

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
