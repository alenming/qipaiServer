#ifndef JDQS_LOGIC_HEAD_FILE
#define JDQS_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

#define CT_J						0									//J
#define CT_Q						1									//Q
#define CT_K						2									//K
#define CT_A						3									//A
#define CT_GUO						4									//ƽ�׹�
#define CT_XIANG					5									//������
#define CT_SCAR						6									//Scar
#define CT_98K						7									//98K
#define CT_JUEDI					8									//��������
#define CT_FREE						10									//�����Ϸ
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//��Ϸ�߼���
class CJDQSLogic
{
public:
	int			m_nCount;

	//��������
public:
	//���캯��
	CJDQSLogic();
	//��������
	virtual ~CJDQSLogic();

	//���ƺ���
public:
	//�����˿�
	VOID RandCardList(BYTE cbCardBuffer[][JDQS_X_COUNT]);

	//�߼�����
public:
	//ȡ���н�
	DOUBLE GetZhongJiangTime(BYTE cbItemInfo[][JDQS_X_COUNT]);
	//����С����
	BYTE GetEnterFree(BYTE cbItemInfo[][JDQS_X_COUNT]);


};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

#endif