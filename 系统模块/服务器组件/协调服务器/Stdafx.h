#pragma once

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//MFC �ļ�

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

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//�����ļ�

//ƽ̨����
#include "..\..\ȫ�ֶ���\Platform.h"
#include "..\..\��Ϣ����\CMD_Correspond.h"
#include "..\..\��Ϣ����\CMD_HttpServer.h"
#include "..\..\ȫ�ֶ���\Log.h"
//�������
#include "..\..\�������\�������\ServiceCoreHead.h"
#include "..\..\���������\�ں�����\KernelEngineHead.h"

#include "..\..\�������\libevent\include\libevent.h"
#include "..\..\�������\json\json.h"
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//���Ӵ���

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../���ӿ�/Ansi/ServiceCore.lib")
#pragma comment (lib,"../../���ӿ�/Ansi/KernelEngine.lib")
#pragma comment (lib,"../../���ӿ�/Ansi/libevent.lib")
#pragma comment (lib,"../../���ӿ�/Ansi/JsonCpp.lib")
#else
#pragma comment (lib,"../../���ӿ�/Unicode/ServiceCore.lib")
#pragma comment (lib,"../../���ӿ�/Unicode/KernelEngine.lib")
#pragma comment (lib,"../../���ӿ�/Unicode/libevent.lib")
#pragma comment (lib,"../../���ӿ�/Unicode/JsonCpp.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../���ӿ�/Ansi/ServiceCoreD.lib")
#pragma comment (lib,"../../���ӿ�/Ansi/KernelEngineD.lib")
#pragma comment (lib,"../../���ӿ�/Ansi/libeventD.lib")
#pragma comment (lib,"../../���ӿ�/Ansi/JsonCppD.lib")
#else
#pragma comment (lib,"../../���ӿ�/Unicode/ServiceCoreD.lib")
#pragma comment (lib,"../../���ӿ�/Unicode/KernelEngineD.lib")
#pragma comment (lib,"../../���ӿ�/Unicode/libeventD.lib")
#pragma comment (lib,"../../���ӿ�/Unicode/JsonCppD.lib")
#endif
#endif

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
