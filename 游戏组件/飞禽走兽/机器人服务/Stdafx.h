#pragma once

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

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

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//MFC �ļ�
#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxCmn.h>
#include <AfxDisp.h>

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//���뻷��
#include "..\..\..\SDKCondition.h"

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
#define  SDK_CONDITION
#ifdef SDK_CONDITION

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//��������

//ƽ̨����

#include "..\..\..\������\Include\GameServiceHead.h"

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../../������/Lib/Ansi/KernelEngine.lib")
#else
#pragma comment (lib,"../../../������/Lib/Unicode/KernelEngine.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../../������/Lib/Ansi/KernelEngineD.lib")
#else
#pragma comment (lib,"../../../������/Lib/Unicode/KernelEngineD.lib")
#endif
#endif

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

#else

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//ϵͳ����

//ƽ̨����

#include "..\..\..\..\ϵͳģ��\���������\��Ϸ����\GameServiceHead.h"

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../../../ϵͳģ��/���ӿ�/Ansi/KernelEngine.lib")
#else
#pragma comment (lib,"../../../../ϵͳģ��/���ӿ�/Unicode/KernelEngine.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../../../ϵͳģ��/���ӿ�/Ansi/KernelEngineD.lib")
#else
#pragma comment (lib,"../../../../ϵͳģ��/���ӿ�/Unicode/KernelEngineD.lib")
#endif
#endif

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

#endif

//���ͷ�ļ�
#include "..\��Ϣ����\CMD_ZodiacBattle.h"


//////////////////////////////////////////////////////////////////////////