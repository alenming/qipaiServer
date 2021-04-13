#ifndef WH_DATA_LOCKER_HEAD_FILE
#define WH_DATA_LOCKER_HEAD_FILE

#pragma once

#include "ServiceCoreHead.h"

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//��������
class SERVICE_CORE_CLASS CWHDataLocker
{
	//��������
private:
	INT								m_nLockCount;					//��������
	CCriticalSection &				m_CriticalSection;				//��������

	//��������
public:
	//���캯��
	CWHDataLocker(CCriticalSection & CriticalSection, bool bLockAtOnce=true);
	//��������
	virtual ~CWHDataLocker();

	//��������
public:
	//��������
	VOID Lock();
	//�������� 
	VOID UnLock();

	//״̬����
public:
	//��������
	inline INT GetLockCount() { return m_nLockCount; }
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

#endif