#pragma once

#if _MSC_VER>=1600
#define _CRT_SECURE_NO_WARNINGS
#else  
#endif 
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#if _MSC_VER>=1600

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
#define _WIN32_IE 0x0601
#endif
#else  

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

#endif 


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

#include <AfxWin.h>
#include <AfxExt.h>

#ifndef _AFX_NO_OLE_SUPPORT
#include <AfxOLE.h>
#include <AfxDisp.h>
#include <AfxoDlgs.h>
#endif

#include <AfxDtctl.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <AfxCmn.h>
#endif

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

