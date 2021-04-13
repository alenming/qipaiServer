#include "Stdafx.h"
#include <Afxdllx.h>

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

static AFX_EXTENSION_MODULE	GameServerDLL={NULL,NULL};

//DLL 导出主函数
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(GameServerDLL,hInstance)) return 0;
		new CDynLinkLibrary(GameServerDLL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(GameServerDLL);
	}

	return 1;
}

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
