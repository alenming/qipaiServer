#include "Stdafx.h"
#include "GameServer.h"
#include "GameServerDlg.h"

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//�������
CGameServerApp theApp;

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

BEGIN_MESSAGE_MAP(CGameServerApp, CWinApp)
END_MESSAGE_MAP()

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//���캯��
CGameServerApp::CGameServerApp()
{
}

//��������
BOOL CGameServerApp::InitInstance()
{
	__super::InitInstance();

	//�������
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//����ע���
	SetRegistryKey(szProduct);

	//��ʾ����
	CGameServerDlg GameServerDlg;
	m_pMainWnd=&GameServerDlg;
	GameServerDlg.DoModal();

	return FALSE;
}

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
