#include "StdAfx.h"
#include "InitParameter.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//服务时间
#define TIME_CONNECT				30									//重连时间
#define TIME_COLLECT				300									//统计时间
#define TIME_LOAD_LIST				120									//加载列表
#define TIME_RELOAD_LIST			600									//加载列表

//客户时间
#define TIME_INTERMIT				0									//中断时间
#define TIME_ONLINE_COUNT			600									//人数时间

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//构造函数
CInitParameter::CInitParameter()
{ 
	InitParameter();
}

//析构函数
CInitParameter::~CInitParameter()
{
}

//初始化
VOID CInitParameter::InitParameter()
{
	//时间定义
	m_wConnectTime=TIME_CONNECT;
	m_wCollectTime=TIME_COLLECT;
	m_wLoadListTime=TIME_LOAD_LIST;
	m_wReLoadListTime=TIME_RELOAD_LIST;

	//客户时间
	m_wIntermitTime=TIME_INTERMIT;
	m_wOnLineCountTime=TIME_ONLINE_COUNT;

	//系统配置
	m_cbDelayList=TRUE;
	m_wMaxConnect=MAX_CONTENT;
	m_wServicePort=PORT_LOGON;
	ZeroMemory(m_szServerName,sizeof(m_szServerName));

	//协调信息
	m_wCorrespondPort=PORT_CENTER;
	ZeroMemory(&m_CorrespondAddress,sizeof(m_CorrespondAddress));

	//连接信息
	ZeroMemory(&m_ServiceAddress,sizeof(m_ServiceAddress));
	ZeroMemory(&m_AccountsDBParameter,sizeof(m_AccountsDBParameter));
	ZeroMemory(&m_TreasureDBParameter,sizeof(m_TreasureDBParameter));
	ZeroMemory(&m_PlatformDBParameter,sizeof(m_PlatformDBParameter));

	return;
}

//加载配置
VOID CInitParameter::LoadInitParameter()
{
	//重置参数
	InitParameter();

	//获取路径
	TCHAR szWorkDir[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDir,CountArray(szWorkDir));

	//构造路径
	TCHAR szIniFile[MAX_PATH]=TEXT("");
	_sntprintf(szIniFile,CountArray(szIniFile),TEXT("%s\\ServerParameter.ini"),szWorkDir);

	//读取配置
	CWHIniData IniData;
	IniData.SetIniFilePath(szIniFile);

	//读取配置
	m_cbDelayList=IniData.ReadInt(TEXT("LogonServer"),TEXT("DelayList"),m_cbDelayList);
	m_wMaxConnect=IniData.ReadInt(TEXT("LogonServer"),TEXT("ConnectMax"),m_wMaxConnect);
	m_wServicePort=IniData.ReadInt(TEXT("LogonServer"),TEXT("ServicePort"),m_wServicePort);
	IniData.ReadEncryptString(TEXT("ServerInfo"),TEXT("ServiceName"),NULL,m_szServerName,CountArray(m_szServerName));
	IniData.ReadEncryptString(TEXT("ServerInfo"),TEXT("ServiceAddr"),NULL,m_ServiceAddress.szAddress,CountArray(m_ServiceAddress.szAddress));

	//协调信息
	m_wCorrespondPort=IniData.ReadInt(TEXT("Correspond"),TEXT("ServicePort"),m_wCorrespondPort);
	IniData.ReadEncryptString(TEXT("ServerInfo"),TEXT("CorrespondAddr"),NULL,m_CorrespondAddress.szAddress,CountArray(m_CorrespondAddress.szAddress));

	//连接信息
	m_AccountsDBParameter.wDataBasePort=(WORD)IniData.ReadInt(TEXT("AccountsDB"),TEXT("DBPort"),1433);
	IniData.ReadEncryptString(TEXT("AccountsDB"),TEXT("DBAddr"),NULL,m_AccountsDBParameter.szDataBaseAddr,CountArray(m_AccountsDBParameter.szDataBaseAddr));
	IniData.ReadEncryptString(TEXT("AccountsDB"),TEXT("DBUser"),NULL,m_AccountsDBParameter.szDataBaseUser,CountArray(m_AccountsDBParameter.szDataBaseUser));
	IniData.ReadEncryptString(TEXT("AccountsDB"),TEXT("DBPass"),NULL,m_AccountsDBParameter.szDataBasePass,CountArray(m_AccountsDBParameter.szDataBasePass));
	IniData.ReadEncryptString(TEXT("AccountsDB"),TEXT("DBName"),szAccountsDB,m_AccountsDBParameter.szDataBaseName,CountArray(m_AccountsDBParameter.szDataBaseName));

	//连接信息
	m_TreasureDBParameter.wDataBasePort=(WORD)IniData.ReadInt(TEXT("TreasureDB"),TEXT("DBPort"),1433);
	IniData.ReadEncryptString(TEXT("TreasureDB"),TEXT("DBAddr"),NULL,m_TreasureDBParameter.szDataBaseAddr,CountArray(m_TreasureDBParameter.szDataBaseAddr));
	IniData.ReadEncryptString(TEXT("TreasureDB"),TEXT("DBUser"),NULL,m_TreasureDBParameter.szDataBaseUser,CountArray(m_TreasureDBParameter.szDataBaseUser));
	IniData.ReadEncryptString(TEXT("TreasureDB"),TEXT("DBPass"),NULL,m_TreasureDBParameter.szDataBasePass,CountArray(m_TreasureDBParameter.szDataBasePass));
	IniData.ReadEncryptString(TEXT("TreasureDB"),TEXT("DBName"),szTreasureDB,m_TreasureDBParameter.szDataBaseName,CountArray(m_TreasureDBParameter.szDataBaseName));

	//连接信息
	TCHAR szPlatformDBItem[32]=TEXT("PlatformDB");
	TCHAR szPlatformDBName[32]=TEXT("QPPlatformDB");
	IniData.ReadString(TEXT("LogonServer"),TEXT("PlatformDBItem"),TEXT("PlatformDB"),szPlatformDBItem,CountArray(szPlatformDBItem));
	IniData.ReadString(TEXT("LogonServer"),TEXT("PlatformDBName"),szPlatformDB,szPlatformDBName,CountArray(szPlatformDBName));
	m_PlatformDBParameter.wDataBasePort=(WORD)IniData.ReadInt(TEXT("PlatformDB"),TEXT("DBPort"),1433);
	IniData.ReadEncryptString(szPlatformDBItem,TEXT("DBAddr"),NULL,m_PlatformDBParameter.szDataBaseAddr,CountArray(m_PlatformDBParameter.szDataBaseAddr));
	IniData.ReadEncryptString(szPlatformDBItem,TEXT("DBUser"),NULL,m_PlatformDBParameter.szDataBaseUser,CountArray(m_PlatformDBParameter.szDataBaseUser));
	IniData.ReadEncryptString(szPlatformDBItem,TEXT("DBPass"),NULL,m_PlatformDBParameter.szDataBasePass,CountArray(m_PlatformDBParameter.szDataBasePass));
	IniData.ReadEncryptString(szPlatformDBItem,TEXT("DBName"),szPlatformDBName,m_PlatformDBParameter.szDataBaseName,CountArray(m_PlatformDBParameter.szDataBaseName));

	return;
}

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
