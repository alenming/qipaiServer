#ifndef MODULE_DB_PARAMETER_HEAD_FILE
#define MODULE_DB_PARAMETER_HEAD_FILE

#pragma once

#include "ModuleManagerHead.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//模块参数
class MODULE_MANAGER_CLASS CModuleDBParameter
{
	//服务参数
protected:
	tagDataBaseParameter			m_PlatformDBParameter;				//连接信息
	tagDataBaseParameter			m_GameMatchDBParameter;				//连接信息

	//静态变量
protected:
	static CModuleDBParameter *		m_pModuleDBParameter;				//对象指针

	//函数定义
public:
	//构造函数
	CModuleDBParameter();
	//析构函数
	virtual ~CModuleDBParameter();

	//连接信息
public:
	//连接信息
	tagDataBaseParameter * GetPlatformDBParameter() { return &m_PlatformDBParameter; }
	//连接信息
	VOID SetPlatformDBParameter(tagDataBaseParameter & PlatformDBParameter) { m_PlatformDBParameter=PlatformDBParameter; }

	//连接信息
public:
	//连接信息
	tagDataBaseParameter * GetGameMatchDBParameter() { return &m_GameMatchDBParameter; }
	//连接信息
	VOID SetGameMatchDBParameter(tagDataBaseParameter & GameMatchDBParameter) { m_GameMatchDBParameter=GameMatchDBParameter; }

	//静态函数
public:
	//获取对象
	static CModuleDBParameter * GetModuleDBParameter() { return m_pModuleDBParameter; }
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

#endif