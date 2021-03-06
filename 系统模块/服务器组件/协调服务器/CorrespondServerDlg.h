#ifndef CORRESPOND_SERVER_DLG_HEAD_FILE
#define CORRESPOND_SERVER_DLG_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ServiceUnits.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//主对话框
class CCorrespondServerDlg : public CDialog, public IServiceUnitsSink
{
	//组件变量
protected:
	CServiceUnits					m_ServiceUnits;						//服务单元
	CTraceServiceControl			m_TraceServiceControl;				//追踪窗口

	//函数定义
public:
	//构造函数
	CCorrespondServerDlg();
	//析构函数
	virtual ~CCorrespondServerDlg();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定消息
	virtual VOID OnOK();
	//取消函数
	virtual VOID OnCancel();
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//服务接口
public:
	//服务状态
	virtual VOID OnServiceUnitsStatus(enServiceStatus ServiceStatus);

	//按钮消息
protected:
	//启动服务
	VOID OnBnClickedStartService();
	//停止服务
	VOID OnBnClickedStopService();

	//消息映射
public:
	//关闭询问
	BOOL OnQueryEndSession();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

#endif