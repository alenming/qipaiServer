#pragma once

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#ifndef WINVER
#define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0501
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0501
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯����Ϊ��ʽ��

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE ��
#include <afxodlgs.h>       // MFC OLE �Ի�����
#include <afxdisp.h>        // MFC �Զ�����
#endif // _AFX_NO_OLE_SUPPORT

#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �����ؼ���֧��
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

//////////////////////////////////////////////////////////////////////////

//���ͷ�ļ�
//#include "..\..\..\������\Include\GameServiceHead.h"

//////////////////////////////////////////////////////////////////////////
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//#ifdef SDK_CONDITION

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//��������

//ƽ̨����
#include "..\..\..\ϵͳģ��\���������\��Ϸ����\GameServiceHead.h"

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../../ϵͳģ��/���ӿ�/Ansi/KernelEngine.lib")
#else
#pragma comment (lib,"../../../ϵͳģ��/���ӿ�/Unicode/KernelEngine.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../../ϵͳģ��/���ӿ�/Ansi/KernelEngineD.lib")
#else
#pragma comment (lib,"../../../ϵͳģ��/���ӿ�/Unicode/KernelEngineD.lib")
#endif
#endif

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//��Ϸ�ļ�
#include "..\��Ϣ����\CMD_Thirteenx.h"

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
