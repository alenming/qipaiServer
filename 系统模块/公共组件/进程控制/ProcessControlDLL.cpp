#include "Stdafx.h"
#include "AfxDllx.h"
#include "ProcessControlHead.h"

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//��̬����
static AFX_EXTENSION_MODULE ProcessControlDLL={NULL,NULL};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//��������
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(ProcessControlDLL, hInstance)) return 0;
		new CDynLinkLibrary(ProcessControlDLL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(ProcessControlDLL);
	}

	return 1;
}

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
