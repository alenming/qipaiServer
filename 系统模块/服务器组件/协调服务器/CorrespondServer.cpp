#include "Stdafx.h"
#include "CorrespondServer.h"
#include "CorrespondServerDlg.h"

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//�������
CCorrespondServerApp theApp;

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

BEGIN_MESSAGE_MAP(CCorrespondServerApp, CWinApp)
END_MESSAGE_MAP()

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//���캯��
CCorrespondServerApp::CCorrespondServerApp()
{
}

//��������
BOOL CCorrespondServerApp::InitInstance()
{
	__super::InitInstance();

	//�������
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//����ע���
	SetRegistryKey(szProduct);

	//��ʾ����
	CCorrespondServerDlg CorrespondServerDlg;
	m_pMainWnd=&CorrespondServerDlg;
	CorrespondServerDlg.DoModal();

	return FALSE;
}

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
