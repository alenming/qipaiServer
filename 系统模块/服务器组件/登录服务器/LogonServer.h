#pragma once

#include "Resource.h"		// ������

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//�������
class CLogonServerApp : public CWinApp
{
	//��������
public:
	//���캯�� 
	CLogonServerApp();

	//���غ���
public:
	//��������
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//�������
extern CLogonServerApp theApp;

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
