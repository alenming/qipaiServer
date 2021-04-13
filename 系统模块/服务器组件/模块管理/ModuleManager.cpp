#include "Stdafx.h"
#include <Afxdllx.h>
#include "ModuleManagerHead.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//静态变量
static AFX_EXTENSION_MODULE	ModuleManagerDLL={NULL,NULL};

//全局函数
extern "C" INT APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(ModuleManagerDLL,hInstance)) return 0;
		new CDynLinkLibrary(ModuleManagerDLL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(ModuleManagerDLL);
	}

	return 1;
}

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
