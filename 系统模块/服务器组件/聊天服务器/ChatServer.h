#pragma once

#include "Resource.h"		// ������

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//�������
class CChatServerApp : public CWinApp
{
	//��������
public:
	//���캯�� 
	CChatServerApp();

	//���غ���
public:
	//��������
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//�������
extern CChatServerApp theApp;

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
