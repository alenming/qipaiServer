#include "Stdafx.h"
#include "AfxDllx.h"
#include "ServiceCoreHead.h"

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//��̬����
static AFX_EXTENSION_MODULE ServiceCoreDLL={NULL,NULL};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//��������
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

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
