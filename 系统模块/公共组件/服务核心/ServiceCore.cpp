#include "Stdafx.h"
#include "AfxDllx.h"
#include "ServiceCoreHead.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//静态变量
static AFX_EXTENSION_MODULE ServiceCoreDLL={NULL,NULL};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//导出函数
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(ServiceCoreDLL, hInstance)) return 0;
		new CDynLinkLibrary(ServiceCoreDLL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(ServiceCoreDLL);
	}

	return 1;
}

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
