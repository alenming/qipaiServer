#include "StdAfx.h"
#include "Resource.h"
#include "Tableframesink.h"
#include "GameServerManager.h"
#include "AndroidUserItemSink.h"

#include "MiniDumper.h"

MiniDumper *g_pGFrame = NULL;
//////////////////////////////////////////////////////////////////////////

//ȫ�ֱ���
static CGameServiceManager			g_GameServiceManager;				//�������

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameServiceManager::CGameServiceManager(void)
{
	g_pGFrame = new MiniDumper(true);

	//��������
	m_GameServiceAttrib.wKindID=KIND_ID;
	m_GameServiceAttrib.wChairCount=GAME_PLAYER;
	m_GameServiceAttrib.wSupporType=(GAME_GENRE_GOLD|GAME_GENRE_SCORE|GAME_GENRE_EDUCATE);

	//���ܱ�־
	m_GameServiceAttrib.cbDynamicJoin=TRUE;
	m_GameServiceAttrib.cbAndroidUser=TRUE;
	m_GameServiceAttrib.cbOffLineTrustee=TRUE;

	//��������
	m_GameServiceAttrib.dwServerVersion=VERSION_SERVER;
	m_GameServiceAttrib.dwClientVersion=VERSION_CLIENT;
	lstrcpyn(m_GameServiceAttrib.szGameName,GAME_NAME,CountArray(m_GameServiceAttrib.szGameName));
	lstrcpyn(m_GameServiceAttrib.szDataBaseName,szTreasureDB,CountArray(m_GameServiceAttrib.szDataBaseName));
	lstrcpyn(m_GameServiceAttrib.szClientEXEName,TEXT("LongHuDouBattle.EXE"),CountArray(m_GameServiceAttrib.szClientEXEName));
	lstrcpyn(m_GameServiceAttrib.szServerDLLName,TEXT("LongHuDouBattleServer.DLL"),CountArray(m_GameServiceAttrib.szServerDLLName));

	return;
}

//��������
CGameServiceManager::~CGameServiceManager(void)
{
}

//�ӿڲ�ѯ
VOID * CGameServiceManager::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameServiceManager,Guid,dwQueryVer);
	QUERYINTERFACE(IGameServiceCustomRule,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameServiceManager,Guid,dwQueryVer);
	return NULL;
}

//������Ϸ��
void * CGameServiceManager::CreateTableFrameSink(const IID & Guid, DWORD dwQueryVer)
{
	//��������
	CTableFrameSink * pTableFrameSink=NULL;
	try
	{
		pTableFrameSink=new CTableFrameSink();
		if (pTableFrameSink==NULL) throw TEXT("����ʧ��");
		void * pObject=pTableFrameSink->QueryInterface(Guid,dwQueryVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	catch (...) {}

	//�������
	SafeDelete(pTableFrameSink);

	return NULL;
}

//��ȡ����
bool CGameServiceManager::GetServiceAttrib(tagGameServiceAttrib & GameServiceAttrib)
{
	GameServiceAttrib=m_GameServiceAttrib;
	return true;
}

//�����޸�
bool CGameServiceManager::RectifyParameter(tagGameServiceOption & GameServiceOption)
{

	//Ч�����
	ASSERT(&GameServiceOption!=NULL);
	if (&GameServiceOption==NULL) return false;

	//�������(ԭ���Ļ�������)
	if (GameServiceOption.lRestrictScore!=0L)
	{
		GameServiceOption.lRestrictScore=__max(GameServiceOption.lRestrictScore,GameServiceOption.lMinTableScore);
	}



	return true;
}

//��������
VOID * CGameServiceManager::CreateAndroidUserItemSink(REFGUID Guid, DWORD dwQueryVer)
{
	CAndroidUserItemSink * pAndroidUserItemSink = NULL;

	try
	{
		//��������
		pAndroidUserItemSink = new CAndroidUserItemSink();
		if (pAndroidUserItemSink == NULL) throw TEXT("����ʧ��");

		//��ѯ�ӿ�
		void * pObject = pAndroidUserItemSink->QueryInterface(Guid, dwQueryVer);
		if (pObject == NULL) throw TEXT("�ӿڲ�ѯʧ��");

		return pObject;
	}
	catch (...) {}

	//ɾ������
	SafeDelete(pAndroidUserItemSink);
}

//��������
VOID * CGameServiceManager::CreateGameDataBaseEngineSink( REFGUID Guid, DWORD dwQueryVer )
{
	return NULL;
}


//��ȡ����
bool CGameServiceManager::SaveCustomRule( LPBYTE pcbCustomRule, WORD wCustonSize )
{
	return m_DlgCustomRule.SaveCustomRule(pcbCustomRule,wCustonSize);
}

//��������
HWND CGameServiceManager::CreateCustomRule( CWnd * pParentWnd, CRect rcCreate, LPBYTE pcbCustomRule, WORD wCustonSize )
{
	//��������
	if (m_DlgCustomRule.m_hWnd == NULL)
	{
		//������Դ
		AfxSetResourceHandle(GetModuleHandle(m_GameServiceAttrib.szServerDLLName));

		//��������
		m_DlgCustomRule.Create(IDD_CUSTOM_RULE,pParentWnd);

		//��ԭ��Դ
		AfxSetResourceHandle(GetModuleHandle(NULL));
	}
	
	//��������ģ��
	//HINSTANCE hDllInstance = NULL;
	//hDllInstance = AfxLoadLibrary(TEXT("LongHuDouBattleAndroid.dll"));
	//if (hDllInstance==NULL)
	//{
	//	m_DlgCustomRule.HideAndroidModule(true);
	//}

	//���ñ���
	m_DlgCustomRule.SetCustomRule(pcbCustomRule,wCustonSize);
	//m_DlgCustomRule.DefaultCustomRule(pcbCustomRule,wCustonSize);
	//��ʾ����
	m_DlgCustomRule.SetWindowPos(NULL,rcCreate.left,rcCreate.top,rcCreate.Width(),rcCreate.Height(),SWP_NOZORDER|SWP_SHOWWINDOW);

	return m_DlgCustomRule.GetSafeHwnd();
}

//Ĭ������
bool CGameServiceManager::DefaultCustomRule( LPBYTE pcbCustomRule, WORD wCustonSize )
{
	return m_DlgCustomRule.DefaultCustomRule(pcbCustomRule,wCustonSize);
}


//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateGameServiceManager(const GUID & Guid, DWORD dwInterfaceVer)
{
	return g_GameServiceManager.QueryInterface(Guid,dwInterfaceVer);
}

//////////////////////////////////////////////////////////////////////////
