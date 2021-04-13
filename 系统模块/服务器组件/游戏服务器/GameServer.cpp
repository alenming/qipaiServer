#include "Stdafx.h"
#include "GameServer.h"
#include "GameServerDlg.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//程序对象
CGameServerApp theApp;

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

BEGIN_MESSAGE_MAP(CGameServerApp, CWinApp)
END_MESSAGE_MAP()

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//构造函数
CGameServerApp::CGameServerApp()
{
}

//启动函数
BOOL CGameServerApp::InitInstance()
{
	__super::InitInstance();

	//设置组件
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//设置注册表
	SetRegistryKey(szProduct);

	//显示窗口
	CGameServerDlg GameServerDlg;
	m_pMainWnd=&GameServerDlg;
	GameServerDlg.DoModal();

	return FALSE;
}

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
