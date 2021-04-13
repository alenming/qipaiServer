#include "StdAfx.h"
#include "ModuleDBParameter.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//静态变量
CModuleDBParameter * CModuleDBParameter::m_pModuleDBParameter=NULL;		//对象指针

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//构造函数
CModuleDBParameter::CModuleDBParameter()
{
	//设置对象
	ASSERT(m_pModuleDBParameter==NULL);
	if (m_pModuleDBParameter==NULL) m_pModuleDBParameter=this;

	//设置变量
	ZeroMemory(&m_PlatformDBParameter,sizeof(m_PlatformDBParameter));

	return;
}

//析构函数
CModuleDBParameter::~CModuleDBParameter()
{
	//设置对象
	ASSERT(m_pModuleDBParameter==this);
	if (m_pModuleDBParameter==this) m_pModuleDBParameter=NULL;

	return;
}

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
