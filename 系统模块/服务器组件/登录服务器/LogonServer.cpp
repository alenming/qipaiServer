#include "Stdafx.h"
#include "LogonServer.h"
#include "LogonServerDlg.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//程序对象
CLogonServerApp theApp;

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

BEGIN_MESSAGE_MAP(CLogonServerApp, CWinApp)
END_MESSAGE_MAP()

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//构造函数
CLogonServerApp::CLogonServerApp()
{
}

//启动函数
BOOL CLogonServerApp::InitInstance()
{
	__super::InitInstance();

	//设置组件
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//设置注册表
	SetRegistryKey(szProduct);

	//显示窗口
	CLogonServerDlg LogonServerDlg;
	m_pMainWnd=&LogonServerDlg;
	LogonServerDlg.DoModal();

	return FALSE;
}

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
