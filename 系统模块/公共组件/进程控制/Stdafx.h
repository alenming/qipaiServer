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
//链接代码

#ifndef _DEBUG
#ifndef _UNICODE
	#pragma comment (lib,"../../链接库/Ansi/ServiceCore.lib")
#else
	#pragma comment (lib,"../../链接库/Unicode/ServiceCore.lib")
#endif
#else
#ifndef _UNICODE
	#pragma comment (lib,"../../链接库/Ansi/ServiceCoreD.lib")
#else
	#pragma comment (lib,"../../链接库/Unicode/ServiceCoreD.lib")
#endif
#endif

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
