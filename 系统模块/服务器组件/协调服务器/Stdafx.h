#pragma once

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//MFC 文件

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

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

#define _AFX_ALL_WARNINGS

#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxDisp.h>
#include <AfxDtctl.h>

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <AfxCmn.h>
#endif

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//包含文件

//平台定义
#include "..\..\全局定义\Platform.h"
#include "..\..\消息定义\CMD_Correspond.h"
#include "..\..\消息定义\CMD_HttpServer.h"
#include "..\..\全局定义\Log.h"
//组件定义
#include "..\..\公共组件\服务核心\ServiceCoreHead.h"
#include "..\..\服务器组件\内核引擎\KernelEngineHead.h"

#include "..\..\公共组件\libevent\include\libevent.h"
#include "..\..\公共组件\json\json.h"
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//链接代码

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../链接库/Ansi/ServiceCore.lib")
#pragma comment (lib,"../../链接库/Ansi/KernelEngine.lib")
#pragma comment (lib,"../../链接库/Ansi/libevent.lib")
#pragma comment (lib,"../../链接库/Ansi/JsonCpp.lib")
#else
#pragma comment (lib,"../../链接库/Unicode/ServiceCore.lib")
#pragma comment (lib,"../../链接库/Unicode/KernelEngine.lib")
#pragma comment (lib,"../../链接库/Unicode/libevent.lib")
#pragma comment (lib,"../../链接库/Unicode/JsonCpp.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../链接库/Ansi/ServiceCoreD.lib")
#pragma comment (lib,"../../链接库/Ansi/KernelEngineD.lib")
#pragma comment (lib,"../../链接库/Ansi/libeventD.lib")
#pragma comment (lib,"../../链接库/Ansi/JsonCppD.lib")
#else
#pragma comment (lib,"../../链接库/Unicode/ServiceCoreD.lib")
#pragma comment (lib,"../../链接库/Unicode/KernelEngineD.lib")
#pragma comment (lib,"../../链接库/Unicode/libeventD.lib")
#pragma comment (lib,"../../链接库/Unicode/JsonCppD.lib")
#endif
#endif

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
