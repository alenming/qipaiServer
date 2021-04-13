#include "Stdafx.h"
#include "CorrespondServer.h"
#include "CorrespondServerDlg.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//程序对象
CCorrespondServerApp theApp;

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

BEGIN_MESSAGE_MAP(CCorrespondServerApp, CWinApp)
END_MESSAGE_MAP()

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//构造函数
CCorrespondServerApp::CCorrespondServerApp()
{
}

//启动函数
BOOL CCorrespondServerApp::InitInstance()
{
	__super::InitInstance();

	//设置组件
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//设置注册表
	SetRegistryKey(szProduct);

	//显示窗口
	CCorrespondServerDlg CorrespondServerDlg;
	m_pMainWnd=&CorrespondServerDlg;
	CorrespondServerDlg.DoModal();

	return FALSE;
}

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
