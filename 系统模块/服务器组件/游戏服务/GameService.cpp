#include "Stdafx.h"
#include "AfxDllx.h"
#include "GameServiceHead.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//静态变量
static AFX_EXTENSION_MODULE GameServiceDLL={NULL,NULL};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//导出函数
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(GameServiceDLL, hInstance)) return 0;
		new CDynLinkLibrary(GameServiceDLL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(GameServiceDLL);
	}

	return 1;
}

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
