#pragma once

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#ifndef WINVER
#define WINVER 0x0500
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0400
#endif

#define _ATL_ATTRIBUTES
#define _AFX_ALL_WARNINGS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//MFC 文件
#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxCmn.h>
#include <AfxDisp.h>

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//编译环境
#include "..\..\..\SDKCondition.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
#define  SDK_CONDITION
#ifdef SDK_CONDITION

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//开发环境

//平台环境

#include "..\..\..\开发库\Include\GameServiceHead.h"

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../../开发库/Lib/Ansi/KernelEngine.lib")
#else
#pragma comment (lib,"../../../开发库/Lib/Unicode/KernelEngine.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../../开发库/Lib/Ansi/KernelEngineD.lib")
#else
#pragma comment (lib,"../../../开发库/Lib/Unicode/KernelEngineD.lib")
#endif
#endif

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

#else

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//系统环境

//平台环境

#include "..\..\..\..\系统模块\服务器组件\游戏服务\GameServiceHead.h"

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../../../系统模块/链接库/Ansi/KernelEngine.lib")
#else
#pragma comment (lib,"../../../../系统模块/链接库/Unicode/KernelEngine.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../../../系统模块/链接库/Ansi/KernelEngineD.lib")
#else
#pragma comment (lib,"../../../../系统模块/链接库/Unicode/KernelEngineD.lib")
#endif
#endif

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

#endif

//组件头文件
#include "..\消息定义\CMD_ZodiacBattle.h"


//////////////////////////////////////////////////////////////////////////