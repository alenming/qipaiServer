#ifndef SGLB_LOGIC_HEAD_FILE
#define SGLB_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"


#define LINE_COUNT						9					//������
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//��Ϸ�߼���
class CSGLBLogic
{
private:
	BYTE								m_cbLineInfo[LINE_COUNT][SGLB_X_COUNT];		//9����

public:
	int									m_nIndex;

	//��������
public:
	//���캯��
	CSGLBLogic();
	//��������
	virtual ~CSGLBLogic();

	//���ƺ���
public:
	//�����˿�
	VOID RandCardList(BYTE cbCardBuffer[][SGLB_X_COUNT]);

	//�߼�����
public:
	//ȡ���н�
	int GetZhongJiangTime(BYTE cbItemInfo[][SGLB_X_COUNT]);
	//����С����
	BYTE GetEnterThree(BYTE cbItemInfo[][SGLB_X_COUNT]);
	//��ȡ��Ѵ���
	BYTE GetEnterFree(BYTE cbItemInfo[][SGLB_X_COUNT]);
	//��ȡJackPot����
	SCORE GetJackPotScore(BYTE cbItemInfo[][SGLB_X_COUNT], SCORE lJackPotScore);
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

#endif