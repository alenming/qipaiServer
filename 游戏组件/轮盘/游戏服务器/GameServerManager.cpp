#include "StdAfx.h"
#include "Resource.h"
#include "Tableframesink.h"
#include "GameServerManager.h"
#include "AndroidUserItemSink.h"
//////////////////////////////////////////////////////////////////////////

//构造函数
CGameServiceManager::CGameServiceManager(void)
{
	//设置属性
	m_GameServiceAttrib.wKindID=KIND_ID;
	m_GameServiceAttrib.wChairCount=GAME_PLAYER;
	m_GameServiceAttrib.wSupporType=(GAME_GENRE_GOLD|GAME_GENRE_SCORE|GAME_GENRE_EDUCATE);

	//功能标志
	m_GameServiceAttrib.cbDynamicJoin=TRUE;
	m_GameServiceAttrib.cbAndroidUser=TRUE;
	m_GameServiceAttrib.cbOffLineTrustee=TRUE;

	//服务属性
	m_GameServiceAttrib.dwServerVersion=VERSION_SERVER;
	m_GameServiceAttrib.dwClientVersion=VERSION_CLIENT;
	lstrcpyn(m_GameServiceAttrib.szGameName,GAME_NAME,CountArray(m_GameServiceAttrib.szGameName));
	lstrcpyn(m_GameServiceAttrib.szDataBaseName,szTreasureDB,CountArray(m_GameServiceAttrib.szDataBaseName));

	lstrcpyn(m_GameServiceAttrib.szClientEXEName,TEXT("Roulette.EXE"),CountArray(m_GameServiceAttrib.szClientEXEName));
	lstrcpyn(m_GameServiceAttrib.szServerDLLName,TEXT("RouletteServer.DLL"),CountArray(m_GameServiceAttrib.szServerDLLName));

	return;
}

//析构函数
CGameServiceManager::~CGameServiceManager(void)
{
}

//接口查询
VOID * CGameServiceManager::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameServiceManager,Guid,dwQueryVer);
	QUERYINTERFACE(IGameServiceCustomRule,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameServiceManager,Guid,dwQueryVer);
	return NULL;
}

//创建游戏桌
VOID * CGameServiceManager::CreateTableFrameSink(REFGUID Guid, DWORD dwQueryVer)
{
	//建立对象
	CTableFrameSink * pTableFrameSink=NULL;
	try
	{
		pTableFrameSink=new CTableFrameSink();
		if (pTableFrameSink==NULL) throw TEXT("创建失败");
		void * pObject=pTableFrameSink->QueryInterface(Guid,dwQueryVer);
		if (pObject==NULL) throw TEXT("接口查询失败");
		return pObject;
	}
	catch (...) {}

	//清理对象
	SafeDelete(pTableFrameSink);

	return NULL;
}

//创建数据
VOID * CGameServiceManager::CreateGameDataBaseEngineSink(REFGUID Guid, DWORD dwQueryVer)
{
	return NULL;
}

bool CGameServiceManager::GetServiceAttrib(tagGameServiceAttrib & GameServiceAttrib)
{
	//设置变量
	GameServiceAttrib=m_GameServiceAttrib;

	return true;
}


//参数修改
bool CGameServiceManager::RectifyParameter(tagGameServiceOption & GameServiceOption)
{
	//效验参数
	ASSERT(&GameServiceOption!=NULL);
	if (&GameServiceOption==NULL) return false;

	//积分上限
	if (GameServiceOption.lRestrictScore!=0L)
	{
		GameServiceOption.lRestrictScore=__max(GameServiceOption.lRestrictScore,GameServiceOption.lMinTableScore);
	}

	return true;
}

//获取配置
bool CGameServiceManager::SaveCustomRule(LPBYTE pcbCustomRule, WORD wCustonSize)
{
	return m_DlgCustomRule.SaveCustomRule(pcbCustomRule,wCustonSize);

	return true;
}

//默认配置
bool CGameServiceManager::DefaultCustomRule(LPBYTE pcbCustomRule, WORD wCustonSize)
{
	return m_DlgCustomRule.DefaultCustomRule(pcbCustomRule,wCustonSize);
	return true;
}

//创建窗口
HWND CGameServiceManager::CreateCustomRule(CWnd * pParentWnd, CRect rcCreate, LPBYTE pcbCustomRule, WORD wCustonSize)
{

	//创建窗口
	if (m_DlgCustomRule.m_hWnd == NULL)
	{
		//设置资源
		AfxSetResourceHandle(GetModuleHandle(m_GameServiceAttrib.szServerDLLName));

		//创建窗口
		m_DlgCustomRule.Create(IDD_CUSTOM_RULE,pParentWnd);

		//还原资源
		AfxSetResourceHandle(GetModuleHandle(NULL));
	}

	//设置变量
	m_DlgCustomRule.SetCustomRule(pcbCustomRule,wCustonSize);

	if(m_DlgCustomRule.CheckData()==false) m_DlgCustomRule.DefaultCustomRule(pcbCustomRule,wCustonSize);
	//显示窗口
	m_DlgCustomRule.SetWindowPos(NULL,rcCreate.left,rcCreate.top,rcCreate.Width(),rcCreate.Height(),SWP_NOZORDER|SWP_SHOWWINDOW);

	return m_DlgCustomRule.GetSafeHwnd();
	return NULL;
}

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//建立对象函数
extern "C" __declspec(dllexport) VOID * CreateGameServiceManager(const GUID & Guid, DWORD dwInterfaceVer)
{
	static CGameServiceManager GameServiceManager;
	return GameServiceManager.QueryInterface(Guid,dwInterfaceVer);
}

//创建机器
VOID *  CGameServiceManager::CreateAndroidUserItemSink(REFGUID Guid, DWORD dwQueryVer)
{
	CAndroidUserItemSink * pAndroidUserItemSink = NULL;

	try
	{
		//建立对象
		pAndroidUserItemSink = new CAndroidUserItemSink();
		if (pAndroidUserItemSink == NULL) throw TEXT("创建失败");

		//查询接口
		void * pObject = pAndroidUserItemSink->QueryInterface(Guid, dwQueryVer);
		if (pObject == NULL) throw TEXT("接口查询失败");

		return pObject;
	}
	catch (...) {}

	//删除对象
	SafeDelete(pAndroidUserItemSink);
}

//////////////////////////////////////////////////////////////////////////


