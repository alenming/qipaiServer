#ifndef HISTORY_SCORE_HEAD_FILE
#define HISTORY_SCORE_HEAD_FILE

#pragma once

#include "Stdafx.h"

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//�ṹ����

//������Ϣ
struct tagHistoryScore
{
	SCORE							lTurnScore;								//������Ϣ
	SCORE							lCollectScore;							//������Ϣ
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//��ʷ����
class CHistoryScore
{
	//��������
protected:
	tagHistoryScore					m_HistoryScore[GAME_PLAYER];			//������Ϣ

	//��������
public:
	//���캯��
	CHistoryScore();
	//��������
	virtual ~CHistoryScore();

	//���ܺ���
public:
	//��������
	VOID ResetData();
	//��ȡ����
	tagHistoryScore * GetHistoryScore(WORD wChairID);

	//�¼�����
public:
	//�û�����
	VOID OnEventUserEnter(WORD wChairID);
	//�û��뿪
	VOID OnEventUserLeave(WORD wChairID);
	//�û�����
	VOID OnEventUserScore(WORD wChairID, SCORE lGameScore);
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

#endif