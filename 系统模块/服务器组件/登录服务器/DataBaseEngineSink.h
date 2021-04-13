#ifndef DATABASE_ENGINE_SINK_HEAD_FILE
#define DATABASE_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "DataBasePacket.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//数据库类
class CDataBaseEngineSink : public IDataBaseEngineSink
{
	//友元定义
	friend class CServiceUnits;

	//用户数据库
protected:
	CDataBaseAide					m_AccountsDBAide;					//用户数据库
	CDataBaseHelper					m_AccountsDBModule;					//用户数据库

	//游戏币数据库
protected:
	CDataBaseAide					m_TreasureDBAide;					//游戏币数据库
	CDataBaseHelper					m_TreasureDBModule;					//游戏币数据库

	//平台数据库
protected:
	CDataBaseAide					m_PlatformDBAide;					//平台数据库
	CDataBaseHelper					m_PlatformDBModule;					//平台数据库

	//组件变量
protected:
	CInitParameter *				m_pInitParameter;					//配置参数
	IDataBaseEngineEvent *			m_pIDataBaseEngineEvent;			//数据事件

	//函数定义
public:
	//构造函数
	CDataBaseEngineSink();
	//析构函数
	virtual ~CDataBaseEngineSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//系统事件
public:
	//启动事件
	virtual bool OnDataBaseEngineStart(IUnknownEx * pIUnknownEx);
	//停止事件
	virtual bool OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx);

	//内核事件
public:
	//时间事件
	virtual bool OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//控制事件
	virtual bool OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize);
	//请求事件
	virtual bool OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//手机登录
protected:
	//I D 登录
	bool OnMobileLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//帐号登录
	bool OnMobileLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//帐号注册
	bool OnMobileRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//其他登录
	bool OnMobileLogonOtherPlatform(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//游客登录
	bool OnMobileLogonVisitor(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//帐号服务
protected:
	//修改密码
	bool OnRequestModifyInsurePass(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改头像
	bool OnRequestModifySystemFace(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改资料
	bool OnRequestModifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//银行服务
protected:
	//开通银行
	bool OnRequestUserEnableInsure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//存入游戏币
	bool OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//兑换金币
	bool OnRequestExchangeBankScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//提取游戏币
	bool OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//转帐游戏币
	bool OnRequestUserTransferScore(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//机器服务
protected:
	//获取参数
	bool OnRequestGetParameter(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//添加参数
	bool OnRequestAddParameter(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改参数
	bool OnRequestModifyParameter(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//删除参数
	bool OnRequestDeleteParameter(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//信息查询
protected:
	//查询银行
	bool OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询用户
	bool OnRequestQueryTransferUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//低保服务
protected:
	//加载低保
	bool OnRequestLoadBaseEnsure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//领取低保
	bool OnRequestTakeBaseEnsure(DWORD dwContextID, VOID * pData, WORD wDataSize);


	//房间虚拟在线人数
	bool OnRequestServerDummyOnLine(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//抽奖服务
protected:
	//请求配置
	bool OnRequestLotteryConfigReq(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//抽奖开始
	bool OnRequestLotteryStart(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//邮件客服
protected:
	//获取邮件
	bool OnRequestQueryEmail(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//系统功能
protected:
	//加载列表
	bool OnRequestLoadGameList(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//在线信息
	bool OnRequestOnLineCountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);	
	//平台配置
	bool OnRequestPlatformParameter(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//结果处理
protected:
	//登录结果
	VOID OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString,DWORD dwCheckUserRight,bool bMobileClient);
	//银行结果
	VOID OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	//操作结果
	VOID OnOperateDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient,bool bCloseSocket=true);
	//机器结果
	VOID OnAndroidDisposeResult(DWORD dwContextID,DWORD dwErrorCode,WORD wSubCommdID,WORD wServerID);
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

#endif