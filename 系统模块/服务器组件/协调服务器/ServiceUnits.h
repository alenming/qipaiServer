#ifndef SERVICE_UNITS_HEAD_FILE
#define SERVICE_UNITS_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "AttemperEngineSink.h"

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//ö�ٶ���

//����״̬
enum enServiceStatus
{
	ServiceStatus_Stop,				//ֹͣ״̬
	ServiceStatus_Config,			//����״̬
	ServiceStatus_Service,			//����״̬
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//״̬�ӿ�
interface IServiceUnitsSink
{
	//�ӿڶ���
public:
	//����״̬
	virtual VOID OnServiceUnitsStatus(enServiceStatus ServiceStatus)=NULL;
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//����Ԫ
class CServiceUnits
{
	//״̬����
protected:
	enServiceStatus					m_ServiceStatus;					//����״̬

	//�������
private:
	CWHDataQueue					m_DataQueue;						//���ݶ���
	CCriticalSection				m_CriticalSection;					//ͬ������

	//�������
protected:
	CInitParameter					m_InitParameter;					//���ò���
	CAttemperEngineSink				m_AttemperEngineSink;				//���ȹ���

	//�ں����
protected:
	CTimerEngineHelper				m_TimerEngine;						//ʱ������
	CAttemperEngineHelper			m_AttemperEngine;					//��������
	CTCPNetworkEngineHelper			m_TCPNetworkEngine;					//��������

	//�ӿڱ���
protected:
	IServiceUnitsSink *				m_pIServiceUnitsSink;				//״̬�ӿ�

	//��������
public:
	//���캯��
	CServiceUnits();
	//��������
	virtual ~CServiceUnits();

	//��Ϣ����
public:
	//��ȡ״̬
	enServiceStatus GetServiceStatus() { return m_ServiceStatus; }

	//�������
public:
	//��������
	bool StartService();
	//ֹͣ����
	bool ConcludeService();
	//���ýӿ�
	bool SetServiceUnitsSink(IServiceUnitsSink * pIServiceUnitsSink);

	//��������
protected:
	//�������
	bool InitializeService();
	//�����ں�
	bool StartKernelService();

	//�ڲ�����
private:
	//����״̬
	bool SetServiceStatus(enServiceStatus ServiceStatus);
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

#endif