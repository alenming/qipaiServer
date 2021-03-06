#ifndef ATTEMPER_ENGINE_SINK_HEAD_FILE
#define ATTEMPER_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "GlobalInfoManager.h"
#include "CHttpKernel.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//数据定义

//服务类型
enum enServiceKind
{
	ServiceKind_None,				//无效服务
	ServiceKind_Game,				//游戏服务
	ServiceKind_Plaza,				//广场服务
	ServiceKind_Chat,				//好友服务
};

//绑定参数
struct tagBindParameter
{
	//网络数据
	DWORD							dwSocketID;							//网络标识
	DWORD							dwClientAddr;						//连接地址
	DWORD							dwActiveTime;						//激活时间

	//附加数据
	WORD							wServiceID;							//服务标识
	enServiceKind					ServiceKind;						//服务类型
};

//数组定义
typedef CWHArray<WORD> CWordArrayTemplate;

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//调度钩子
class CAttemperEngineSink : public IAttemperEngineSink
{
	//友元定义
	friend class CServiceUnits;

	//状态变量
protected:
	WORD							m_wCollectItem;						//汇总连接
	CWordArrayTemplate				m_WaitCollectItemArray;				//汇总等待

	//变量定义
protected:
	CInitParameter *				m_pInitParameter;					//配置参数
	tagBindParameter *				m_pBindParameter;					//辅助数组

	//组件变量
protected:
	CGlobalInfoManager				m_GlobalInfoManager;				//全局管理

	//组件接口
protected:
	ITimerEngine *					m_pITimerEngine;					//时间引擎
	ITCPNetworkEngine *				m_pITCPNetworkEngine;				//网络引擎
	CHttpKernel<CAttemperEngineSink> *					m_pHttpServerService;				//HTTP服务

	//函数定义
public:
	//构造函数
	CAttemperEngineSink();
	//析构函数
	virtual ~CAttemperEngineSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//异步接口
public:
	//启动事件
	virtual bool OnAttemperEngineStart(IUnknownEx * pIUnknownEx);
	//停止事件
	virtual bool OnAttemperEngineConclude(IUnknownEx * pIUnknownEx);

	//HTTP事件接口
protected:
	//POST请求回调
	void OnServerPostCallBack(struct evhttp_request *req, void * pArg);
	//GET请求回调
	void OnServerGetCallBack(struct evhttp_request *req, void * pArg);

	//事件接口
public:
	//控制事件
	virtual bool OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize);
	//自定事件
	virtual bool OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize);

	//内核事件
public:
	//时间事件
	virtual bool OnEventTimer(DWORD dwTimerID, WPARAM wBindParam);
	//数据库事件
	virtual bool OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//连接事件
public:
	//连接事件
	virtual bool OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode);
	//关闭事件
	virtual bool OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason);
	//读取事件
	virtual bool OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize);

	//网络事件
public:
	//应答事件
	virtual bool OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID);
	//关闭事件
	virtual bool OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID);
	//读取事件
	virtual bool OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//HTTP读取事件
	virtual bool OnEventServerHttpNetworkRead(DWORD dwMsgID, VOID * pData, WORD wDataSize,struct evhttp_request *req);

	//网络事件
protected:
	//注册服务
	bool OnTCPNetworkMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//服务状态
	bool OnTCPNetworkMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//用户汇总
	bool OnTCPNetworkMainUserCollect(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//远程服务
	bool OnTCPNetworkMainRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//管理服务
	bool OnTCPNetworkMainManagerService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//机器服务
	bool OnTCPNetworkMainAndroidService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//HTTP网络事件
protected:
	//查询在线信息
	bool OnHttpQueryChatServerUserOnline(DWORD dwMsgID,VOID * pData,WORD wDataSize, struct evhttp_request *req);
	//解散游戏桌子
	bool OnHttpClearUser(DWORD dwMsgID,VOID * pData,WORD wDataSize, struct evhttp_request *req);
	//踢掉大厅在线用户（强制用户下线）
	bool OnHttpForceOffline(DWORD dwMsgID,VOID * pData,WORD wDataSize, struct evhttp_request *req);
	//查询虚拟人数
	bool OnHttpQueryDummyOnlineUser(DWORD dwMsgID,VOID * pData,WORD wDataSize, struct evhttp_request *req);
	//虚拟人数时间段有变更
	bool OnHttpChangeDummyOnlineUser(DWORD dwMsgID,VOID * pData,WORD wDataSize, struct evhttp_request *req);
	//推送客服反馈
	bool OnHttpCustomer(DWORD dwMsgID, VOID * pData, WORD wDataSize, struct evhttp_request *req);
	//修改房间库存
	bool OnHttpModifyStorage(DWORD dwMsgID, VOID * pData, WORD wDataSize, struct evhttp_request *req);
	//充值成功提醒
	bool OnHttpRechargeSuccess(DWORD dwMsgID, VOID * pData, WORD wDataSize, struct evhttp_request *req);
	//新邮件提醒
	bool OnHttpNotifyidNewMail(DWORD dwMsgID, VOID * pData, WORD wDataSize, struct evhttp_request *req);

	//辅助函数
protected:
	//发送列表
	bool SendServerListItem(DWORD dwSocketID);
	//发送列表
	bool SendMatchListItem(DWORD dwSocketID);

	//数据发送
protected:
	//房间发送
	bool SendDataToGame(WORD wServerID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//用户发送
	bool SendDataToUser(WORD wServerID, DWORD dwUserID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

#endif