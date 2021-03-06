#ifndef SERVICE_CORE_HEAD_FILE
#define SERVICE_CORE_HEAD_FILE

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//包含文件

//MFC 文件
#include <Nb30.h>
#include <Afxmt.h>
#include <AtlBase.h>

//系统文件
#include "..\..\全局定义\Platform.h"
//#include "..\..\公共组件\libevent\include\libevent.h"
//#include "..\..\公共组件\json\json.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//公共定义

//导出定义
#ifndef SERVICE_CORE_CLASS
	#ifdef  SERVICE_CORE_DLL
		#define SERVICE_CORE_CLASS _declspec(dllexport)
	#else
		#define SERVICE_CORE_CLASS _declspec(dllimport)
	#endif
#endif

//模块定义
#ifndef _DEBUG
	#define SERVICE_CORE_DLL_NAME	TEXT("ServiceCore.dll")			//组件名字
#else
	#define SERVICE_CORE_DLL_NAME	TEXT("ServiceCoreD.dll")		//组件名字
#endif

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//导出文件

#ifndef SERVICE_CORE_DLL
	#include "WHThread.h"
	#include "WHRegKey.h"
	#include "WHEncrypt.h"
	#include "WHService.h"
	#include "WHIniData.h"
	#include "WHDataQueue.h"
	#include "WHDataLocker.h"
	#include "WHCommandLine.h"
	#include "WHMD5CheckSum.h"
#endif

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

#endif