#ifndef CAISHEN_LOGIC_HEAD_FILE
#define CAISHEN_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"


//�߼�����
#define CT_HULU						0									//��«
#define CT_BAOZI					1									//����
#define CT_TAOZI					2									//����
#define CT_CUIYU					3									//����
#define CT_FUDAI					4									//����
#define CT_JINYU					5									//����
#define CT_LAOTOU					6									//��ͷ
#define CT_HESHANG					7									//����
#define CT_CAISHENDAO				8									//����
#define CT_GUANYUAN					9									//��Ա
#define CT_JINLUO					10									//����
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//��Ϸ�߼���
class CCaiShenLogic
{

	//��������
public:
	//���캯��
	CCaiShenLogic();
	//��������
	virtual ~CCaiShenLogic();

	//���ƺ���
public:
	//�����˿�
	VOID RandCardList(BYTE cbCardBuffer[][CAISHEN_X_COUNT]);

	//�߼�����
public:
	//ȡ���н�
	DOUBLE GetZhongJiangTime(BYTE cbItemInfo[][CAISHEN_X_COUNT]);
	//����С����
	BYTE GetEnterFree(BYTE cbItemInfo[][CAISHEN_X_COUNT]);
	
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

#endif