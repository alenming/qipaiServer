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

#define _ATL_ATTRIBUTES
#define _AFX_ALL_WARNINGS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//MFC �ļ�
#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxCmn.h>
#include <AfxDisp.h>


//���뻷��
//#include "..\..\..\SDKCondition.h"

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

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

//////////////////////////////////////////////////////////////////////////

#include "..\��Ϣ����\CMD_Game.h"

///////////////////////////////////////////////////////////////////////////