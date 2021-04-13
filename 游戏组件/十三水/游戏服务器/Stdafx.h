#pragma once

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

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

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将为显式的

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 类
#include <afxodlgs.h>       // MFC OLE 对话框类
#include <afxdisp.h>        // MFC 自动化类
#endif // _AFX_NO_OLE_SUPPORT

#include <afxdtctl.h>		// MFC 对 Internet Explorer 4 公共控件的支持
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

//////////////////////////////////////////////////////////////////////////

//组件头文件
//#include "..\..\..\开发库\Include\GameServiceHead.h"

//////////////////////////////////////////////////////////////////////////
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//#ifdef SDK_CONDITION

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//开发环境

//平台环境
#include "..\..\..\系统模块\服务器组件\游戏服务\GameServiceHead.h"

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../../系统模块/链接库/Ansi/KernelEngine.lib")
#else
#pragma comment (lib,"../../../系统模块/链接库/Unicode/KernelEngine.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../../系统模块/链接库/Ansi/KernelEngineD.lib")
#else
#pragma comment (lib,"../../../系统模块/链接库/Unicode/KernelEngineD.lib")
#endif
#endif

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//游戏文件
#include "..\消息定义\CMD_Thirteenx.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
