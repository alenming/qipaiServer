#pragma once

#include "Resource.h"		// 主符号

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//程序对象
class CLogonServerApp : public CWinApp
{
	//函数定义
public:
	//构造函数 
	CLogonServerApp();

	//重载函数
public:
	//启动函数
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//程序对象
extern CLogonServerApp theApp;

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
