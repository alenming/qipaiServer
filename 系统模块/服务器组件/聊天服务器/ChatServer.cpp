#include "Stdafx.h"
#include "ChatServer.h"
#include "ChatServerDlg.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//程序对象
CChatServerApp theApp;

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

BEGIN_MESSAGE_MAP(CChatServerApp, CWinApp)
END_MESSAGE_MAP()

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//构造函数
CChatServerApp::CChatServerApp()
{
}

//启动函数
BOOL CChatServerApp::InitInstance()
{
	__super::InitInstance();

	//设置组件
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//设置注册表
	SetRegistryKey(szProduct);

	//显示窗口
	CChatServerDlg ChatServerDlg;
	m_pMainWnd=&ChatServerDlg;
	ChatServerDlg.DoModal();

	return FALSE;
}

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
