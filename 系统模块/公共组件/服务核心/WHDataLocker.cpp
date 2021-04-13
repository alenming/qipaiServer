#include "StdAfx.h"
#include "WHDataLocker.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//构造函数
CWHDataLocker::CWHDataLocker(CCriticalSection & CriticalSection, bool bLockAtOnce) 
	: m_CriticalSection(CriticalSection)
{
	//设置变量
	m_nLockCount=0;

	//锁定对象
	if (bLockAtOnce==true)
	{
		Lock();
	}

	return;
}

//析构函数
CWHDataLocker::~CWHDataLocker()
{
	//解除锁定
	while (m_nLockCount>0)
	{
		UnLock();
	}

	return;
}

//锁定函数
VOID CWHDataLocker::Lock()
{
	//锁定对象
	m_CriticalSection.Lock();

	//设置变量
	m_nLockCount++;

	return;
}

//解锁函数
VOID CWHDataLocker::UnLock()
{
	//效验状态
	ASSERT(m_nLockCount>0);
	if (m_nLockCount==0) return;

	//设置变量
	m_nLockCount--;

	//解除锁定
	m_CriticalSection.Unlock();

	return;
}

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
