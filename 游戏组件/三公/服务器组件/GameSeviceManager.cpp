#include "StdAfx.h"
#include "Resource.h"
#include "Tableframesink.h"
#include "GameServiceManager.h"
#include "AndroidUserItemSink.h"
#include "MiniDumper.h"

MiniDumper *g_pGFrame = NULL;
//////////////////////////////////////////////////////////////////////////
//机器定义
#ifndef _DEBUG
#define ANDROID_SERVICE_DLL_NAME  TEXT("SGAndroid.dll")  //组件名字
#else
#define ANDROID_SERVICE_DLL_NAME  TEXT("SGAndroid.dll")  //组件名字
#endif

//////////////////////////////////////////////////////////////////////////



//构造函数
CGameServiceManager::CGameServiceManager(void)
{
	g_pGFrame = new MiniDumper(true);

  m_pDlgCustomRule = NULL;

  //设置属性
  m_GameServiceAttrib.wKindID=KIND_ID;
  m_GameServiceAttrib.wChairCount=GAME_PLAYER;
  m_GameServiceAttrib.wSupporType=(GAME_GENRE_GOLD|GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_EDUCATE|GAME_GENRE_PERSONAL);

  //功能标志
  m_GameServiceAttrib.cbDynamicJoin=TRUE;
  m_GameServiceAttrib.cbAndroidUser=TRUE;
  m_GameServiceAttrib.cbOffLineTrustee=TRUE;

  //服务属性
  m_GameServiceAttrib.dwServerVersion=VERSION_SERVER;
  m_GameServiceAttrib.dwClientVersion=VERSION_CLIENT;
  lstrcpyn(m_GameServiceAttrib.szGameName,GAME_NAME,CountArray(m_GameServiceAttrib.szGameName));
  lstrcpyn(m_GameServiceAttrib.szDataBaseName,szTreasureDB,CountArray(m_GameServiceAttrib.szDataBaseName));
  lstrcpyn(m_GameServiceAttrib.szClientEXEName,TEXT("SG.exe"),CountArray(m_GameServiceAttrib.szClientEXEName));
  lstrcpyn(m_GameServiceAttrib.szServerDLLName,TEXT("SGServer.dll"),CountArray(m_GameServiceAttrib.szServerDLLName));

  return;
}

//析构函数
CGameServiceManager::~CGameServiceManager(void)
{
  //删除对象
  SafeDelete(m_pDlgCustomRule);
  //释放 DLL
  if(m_hDllInstance!=NULL)
  {
    AfxFreeLibrary(m_hDllInstance);
    m_hDllInstance=NULL;
  }
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
    if(pTableFrameSink==NULL)
    {
      throw TEXT("创建失败");
    }
    void * pObject=pTableFrameSink->QueryInterface(Guid,dwQueryVer);
    if(pObject==NULL)
    {
      throw TEXT("接口查询失败");
    }
    return pObject;
  }
  catch(...) {}

  //清理对象
  SafeDelete(pTableFrameSink);

  return NULL;
}

//获取属性
bool CGameServiceManager::GetServiceAttrib(tagGameServiceAttrib & GameServiceAttrib)
{
  GameServiceAttrib=m_GameServiceAttrib;
  return true;
}

//参数修改
bool CGameServiceManager::RectifyParameter(tagGameServiceOption & GameServiceOption)
{
  //效验参数
  ASSERT(&GameServiceOption!=NULL);
  if(&GameServiceOption==NULL)
  {
    return false;
  }

  //积分下限
  //if (GameServiceOption.wServerType != GAME_GENRE_SCORE)//ztodo
  if (GameServiceOption.wServerType&(GAME_GENRE_GOLD | SCORE_GENRE_POSITIVE))
  {
    GameServiceOption.lMinTableScore=__max(GameServiceOption.lCellScore*50L,GameServiceOption.lMinTableScore);
  }
  return true;
}

//获取配置
bool CGameServiceManager::SaveCustomRule(LPBYTE pcbCustomRule, WORD wCustonSize)
{
  //效验状态
  ASSERT(m_pDlgCustomRule!=NULL);
  if(m_pDlgCustomRule==NULL)
  {
    return false;
  }

  //变量定义
  ASSERT(wCustonSize>=sizeof(tagCustomRule));
  tagCustomRule * pCustomRule=(tagCustomRule *)pcbCustomRule;

  //获取配置
  if(m_pDlgCustomRule->GetCustomRule(*pCustomRule)==false)
  {
    return false;
  }

  return true;
}

//默认配置
bool CGameServiceManager::DefaultCustomRule(LPBYTE pcbCustomRule, WORD wCustonSize)
{
  //变量定义
  ASSERT(wCustonSize>=sizeof(tagCustomRule));
  tagCustomRule * pCustomRule=(tagCustomRule *)pcbCustomRule;

  //机器人存取款
  pCustomRule->lRobotScoreMin = 100000;
  pCustomRule->lRobotScoreMax = 1000000;
  pCustomRule->lRobotBankGet = 1000000;
  pCustomRule->lRobotBankGetBanker = 10000000;
  pCustomRule->lRobotBankStoMul = 10;

  return true;
}

//创建窗口
HWND CGameServiceManager::CreateCustomRule(CWnd * pParentWnd, CRect rcCreate, LPBYTE pcbCustomRule, WORD wCustonSize)
{
  //创建窗口
  if(m_pDlgCustomRule==NULL)
  {
    m_pDlgCustomRule=new CDlgCustomRule;
  }

  //创建窗口
  if(m_pDlgCustomRule->m_hWnd==NULL)
  {
    //设置资源
    AfxSetResourceHandle(GetModuleHandle(m_GameServiceAttrib.szServerDLLName));

    //创建窗口
    m_pDlgCustomRule->Create(IDD_CUSTOM_RULE,pParentWnd);

    //还原资源
    AfxSetResourceHandle(GetModuleHandle(NULL));
  }

  //设置变量
  ASSERT(wCustonSize>=sizeof(tagCustomRule));
  m_pDlgCustomRule->SetCustomRule(*((tagCustomRule *)pcbCustomRule));

  //显示窗口
  m_pDlgCustomRule->SetWindowPos(NULL,rcCreate.left,rcCreate.top,rcCreate.Width(),rcCreate.Height(),SWP_NOZORDER|SWP_SHOWWINDOW);

  return m_pDlgCustomRule->GetSafeHwnd();
}


//创建机器
VOID * CGameServiceManager::CreateAndroidUserItemSink(REFGUID Guid, DWORD dwQueryVer)
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

//创建数据
VOID * CGameServiceManager::CreateGameDataBaseEngineSink(REFGUID Guid, DWORD dwQueryVer)
{
  return NULL;
}

//////////////////////////////////////////////////////////////////////////

//建立对象函数
extern "C" __declspec(dllexport) VOID * CreateGameServiceManager(REFGUID Guid, DWORD dwInterfaceVer)
{
  static CGameServiceManager GameServiceManager;
  return GameServiceManager.QueryInterface(Guid,dwInterfaceVer);
}
