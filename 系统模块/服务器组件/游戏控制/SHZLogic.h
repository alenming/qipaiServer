#ifndef SHZ_LOGIC_HEAD_FILE
#define SHZ_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"


//�߼�����
#define CT_FUTOU					0									//��ͷ
#define CT_YINGQIANG				1									//��ǹ
#define CT_DADAO					2									//��
#define CT_LU						3									//³
#define CT_LIN						4									//��
#define CT_SONG						5									//��
#define CT_TITIANXINGDAO			6									//�����е�
#define CT_ZHONGYITANG				7									//������
#define CT_SHUIHUZHUAN				8									//ˮ䰴�
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//��Ϸ�߼���
class CSHZLogic
{
	//��������
public:
	//���캯��
	CSHZLogic();
	//��������
	virtual ~CSHZLogic();

	//���ƺ���
public:
	//�����˿�
	VOID RandCardList(BYTE cbCardBuffer[][SHZ_X_COUNT]);

	//�߼�����
public:
	//ȡ���н�
	int GetZhongJiangTime(BYTE cbItemInfo[][SHZ_X_COUNT]);
	//ȫ�̽�
	int GetQuanPanJiangTime(BYTE cbItemInfo[][SHZ_X_COUNT]);
	//ȫ����
	int GetQuanRenJiangTime(BYTE cbItemInfo[][SHZ_X_COUNT]);
	//ȫ����
	int GetQuanBingQiJiangTime(BYTE cbItemInfo[][SHZ_X_COUNT]);
	//����С����
	BYTE GetEnterThree(BYTE cbItemInfo[][SHZ_X_COUNT]);
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

#endif