#include "StdAfx.h"
#include "GlobalInfoManager.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//构造函数
CGlobalUserItem::CGlobalUserItem()
{
	//用户属性
	m_dwUserID=0L;
	m_dwGameID=0L;
	m_szNickName[0]=0;

	//等级信息
	m_cbMemberOrder=0;
	m_cbMasterOrder=0;

	//链表属性
	m_pNextUserItemPtr=NULL;

	return;
}

//析构函数
CGlobalUserItem::~CGlobalUserItem()
{
}

void CGlobalUserItem::UpdateStatus(const WORD wTableID, const WORD wChairID, const BYTE cbUserStatus)
{
	m_UserInfo.wTableID = wTableID;
	m_UserInfo.wChairID = wChairID;
	m_UserInfo.cbUserStatus = cbUserStatus;
}

//枚举房间
CGlobalServerItem * CGlobalUserItem::EnumServerItem(WORD wIndex)
{
	if (wIndex>=m_GlobalServerItemArray.GetCount()) return NULL;
	return m_GlobalServerItemArray[wIndex];
}

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//构造函数
CGlobalDummyItem::CGlobalDummyItem()
{
	m_pNextItemPtr = NULL;
	wServerID = 0;
	ZeroMemory(&m_DummyInfo, 0);
}

//析构函数
CGlobalDummyItem::~CGlobalDummyItem()
{


}

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//构造函数
CGlobalPlazaItem::CGlobalPlazaItem()
{
	//设置变量
	m_wIndex=0;
	ZeroMemory(&m_GamePlaza,sizeof(m_GamePlaza));

	//链表属性
	m_pNextPlazaItemPtr=NULL;

	return;
}

//析构函数
CGlobalPlazaItem::~CGlobalPlazaItem()
{
}

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//构造函数
CGlobalChatServerItem::CGlobalChatServerItem()
{
	//设置变量
	m_wIndex=0;
	ZeroMemory(&m_ChatServer,sizeof(m_ChatServer));

	//链表属性
	m_pNextChatServerPtr=NULL;

	return;
}

//析构函数
CGlobalChatServerItem::~CGlobalChatServerItem()
{
}

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//构造函数
CGlobalServerItem::CGlobalServerItem()
{
	//设置变量
	m_wIndex=0;
	ZeroMemory(&m_GameServer,sizeof(m_GameServer));

	//链表属性
	m_pNextServerItemPtr=NULL;

	//设置质数
	m_MapUserID.InitHashTable(PRIME_SERVER_USER);

	return;
}

//析构函数
CGlobalServerItem::~CGlobalServerItem()
{
}

//寻找用户
CGlobalUserItem * CGlobalServerItem::SearchUserItem(DWORD dwUserID)
{
	//搜索用户
	CGlobalUserItem * pGlobalUserItem=NULL;
	m_MapUserID.Lookup(dwUserID,pGlobalUserItem);

	return pGlobalUserItem;
}

//机器人数目
DWORD CGlobalServerItem::GetAndroidUserItemCount()
{
	DWORD dwAndroidUserCount = 0;
	DWORD dwUserID=0L;
	CGlobalUserItem * pGlobalUserItem=NULL;
	POSITION Position=m_MapUserID.GetStartPosition();
	while (Position!=NULL)
	{
		m_MapUserID.GetNextAssoc(Position,dwUserID,pGlobalUserItem);
		if (pGlobalUserItem->IsAndroid())
			++dwAndroidUserCount;
	}

	return dwAndroidUserCount;
}
//真实人数目
DWORD CGlobalServerItem::GetRealUserItemCount()
{
	DWORD dwRealUserCount = 0;
	DWORD dwUserID=0L;
	CGlobalUserItem * pGlobalUserItem=NULL;
	POSITION Position=m_MapUserID.GetStartPosition();
	while (Position!=NULL)
	{
		m_MapUserID.GetNextAssoc(Position,dwUserID,pGlobalUserItem);
		if (!pGlobalUserItem->IsAndroid())
			++dwRealUserCount;
	}

	return dwRealUserCount;
}

//寻找用户
CGlobalChatUserItem * CGlobalChatServerItem::SearchChatUserItem(DWORD dwUserID)
{
	//搜索用户
	CGlobalChatUserItem * pGlobalUserItem=NULL;
	m_MapUserID.Lookup(dwUserID,pGlobalUserItem);

	return pGlobalUserItem;
}

//构造函数
CGlobalChatUserItem::CGlobalChatUserItem()
{
	//用户属性
	m_dwUserID=0L;
	m_szNickName[0]=0;

	//链表属性
	m_pNextUserItemPtr=NULL;

	return;
}

//析构函数
CGlobalChatUserItem::~CGlobalChatUserItem()
{
}

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//构造函数
CGlobalInfoManager::CGlobalInfoManager()
{
	//存储变量
	m_pGlobalUserItem=NULL;
	m_pGlobalChatUserItem=NULL;
	m_pGlobalPlazaItem=NULL;
	m_pGlobalServerItem=NULL;
	m_pGlobalChatServerItem = NULL;
	m_pGlobalDummyItem = NULL;

	//设置索引
	m_MapPlazaID.InitHashTable(PRIME_SERVER);
	m_MapServerID.InitHashTable(PRIME_SERVER);
	m_MapUserID.InitHashTable(PRIME_PLATFORM_USER);
	m_MapGameID.InitHashTable(PRIME_PLATFORM_USER);
	m_MapNickName.InitHashTable(PRIME_PLATFORM_USER);
	m_MapChatID.InitHashTable(PRIME_SERVER);
	m_MapChatUserID.InitHashTable(PRIME_PLATFORM_USER);
	m_MapDummyInfo.InitHashTable(PRIME_SERVER);

	m_ServerTableCountArray.RemoveAll();
	return;
}

//析构函数
CGlobalInfoManager::~CGlobalInfoManager()
{
	//删除用户
	DWORD dwUserID=0L;
	CGlobalUserItem * pGlobalUserItem=NULL;
	POSITION Position=m_MapUserID.GetStartPosition();
	while (Position!=NULL)
	{
		m_MapUserID.GetNextAssoc(Position,dwUserID,pGlobalUserItem);
		SafeDelete(pGlobalUserItem);
	}
	m_MapUserID.RemoveAll();

	//删除聊天用户
	CGlobalChatUserItem * pGlobalChatUserItem=NULL;
	Position=m_MapChatUserID.GetStartPosition();

	//删除聊天用户
	while (Position!=NULL)
	{
		m_MapChatUserID.GetNextAssoc(Position,dwUserID,pGlobalChatUserItem);
		SafeDelete(pGlobalChatUserItem);
	}
	m_MapChatUserID.RemoveAll();

	//删除虚拟用户
	WORD wServerID = 0;
	CGlobalDummyItem * pGlobalDummyItem=NULL;
	Position=m_MapDummyInfo.GetStartPosition();

	//删除虚拟用户
	while (Position!=NULL)
	{
		m_MapDummyInfo.GetNextAssoc(Position,wServerID,pGlobalDummyItem);
		SafeDelete(pGlobalDummyItem);
	}
	m_MapDummyInfo.RemoveAll();

	//删除广场
	WORD wPlazaID=0;
	CGlobalPlazaItem * pGlobalPlazaItem=NULL;
	Position=m_MapPlazaID.GetStartPosition();
	while (Position!=NULL)
	{
		m_MapPlazaID.GetNextAssoc(Position,wPlazaID,pGlobalPlazaItem);
		SafeDelete(pGlobalPlazaItem);
	}
	m_MapPlazaID.RemoveAll();

	//删除房间
	//WORD wServerID=0;
	CGlobalServerItem * pGlobalServerItem=NULL;
	Position=m_MapServerID.GetStartPosition();
	while (Position!=NULL)
	{
		m_MapServerID.GetNextAssoc(Position,wServerID,pGlobalServerItem);
		SafeDelete(pGlobalServerItem);
	}
	m_MapServerID.RemoveAll();

	//删除聊天
	WORD wChatID=0;
	CGlobalChatServerItem * pGlobalChatItem=NULL;
	Position=m_MapChatID.GetStartPosition();
	while (Position!=NULL)
	{
		m_MapChatID.GetNextAssoc(Position,wChatID,pGlobalChatItem);
		SafeDelete(pGlobalChatItem);
	}
	m_MapChatID.RemoveAll();

	//删除用户
	while (m_pGlobalUserItem!=NULL)
	{
		pGlobalUserItem=m_pGlobalUserItem;
		m_pGlobalUserItem=m_pGlobalUserItem->m_pNextUserItemPtr;
		SafeDelete(pGlobalUserItem);
	}

	//删除聊天用户
	while (m_pGlobalChatUserItem!=NULL)
	{
		pGlobalChatUserItem=m_pGlobalChatUserItem;
		m_pGlobalChatUserItem=m_pGlobalChatUserItem->m_pNextUserItemPtr;
		SafeDelete(pGlobalChatUserItem);
	}

	//删除聊天用户
	while (m_pGlobalDummyItem!=NULL)
	{
		pGlobalDummyItem=m_pGlobalDummyItem;
		m_pGlobalDummyItem=m_pGlobalDummyItem->m_pNextItemPtr;
		SafeDelete(pGlobalDummyItem);
	}

	//删除广场
	while (m_pGlobalPlazaItem!=NULL)
	{
		pGlobalPlazaItem=m_pGlobalPlazaItem;
		m_pGlobalPlazaItem=m_pGlobalPlazaItem->m_pNextPlazaItemPtr;
		SafeDelete(pGlobalPlazaItem);
	}

	//删除房间
	while (m_pGlobalServerItem!=NULL)
	{
		pGlobalServerItem=m_pGlobalServerItem;
		m_pGlobalServerItem=m_pGlobalServerItem->m_pNextServerItemPtr;
		SafeDelete(pGlobalServerItem);
	}

	//删除聊天
	while (m_pGlobalChatServerItem!=NULL)
	{
		pGlobalChatItem=m_pGlobalChatServerItem;
		m_pGlobalChatServerItem=m_pGlobalChatServerItem->m_pNextChatServerPtr;
		SafeDelete(pGlobalChatItem);
	}

	return;
}

//重置数据
VOID CGlobalInfoManager::ResetData()
{
	//删除用户
	DWORD dwUserID=0L;
	CGlobalUserItem * pGlobalUserItem=NULL;
	POSITION Position=m_MapUserID.GetStartPosition();

	//删除用户
	while (Position!=NULL)
	{
		m_MapUserID.GetNextAssoc(Position,dwUserID,pGlobalUserItem);
		if (pGlobalUserItem!=NULL) FreeGlobalUserItem(pGlobalUserItem);
	}

	//删除聊天用户
	CGlobalChatUserItem * pGlobalChatUserItem=NULL;
	Position=m_MapChatUserID.GetStartPosition();

	//删除聊天用户
	while (Position!=NULL)
	{
		m_MapChatUserID.GetNextAssoc(Position,dwUserID,pGlobalChatUserItem);
		if (pGlobalChatUserItem!=NULL) FreeGlobalChatUserItem(pGlobalChatUserItem);
	}

	//删除虚拟用户
	WORD wServerID = 0;
	CGlobalDummyItem * pGlobalDummyItem=NULL;
	Position=m_MapDummyInfo.GetStartPosition();

	//删除虚拟用户
	while (Position!=NULL)
	{
		m_MapDummyInfo.GetNextAssoc(Position,wServerID,pGlobalDummyItem);
		if (pGlobalDummyItem!=NULL) FreeGlobalDummyItem(pGlobalDummyItem);
	}

	//删除广场
	WORD wPlazaID=0;
	CGlobalPlazaItem * pGlobalPlazaItem=NULL;
	Position=m_MapPlazaID.GetStartPosition();

	//删除广场
	while (Position!=NULL)
	{
		m_MapPlazaID.GetNextAssoc(Position,wPlazaID,pGlobalPlazaItem);
		if (pGlobalPlazaItem!=NULL) FreeGlobalPlazaItem(pGlobalPlazaItem);
	}

	//删除房间
	//WORD wServerID=0;
	CGlobalServerItem * pGlobalServerItem=NULL;
	Position=m_MapServerID.GetStartPosition();

	//删除房间
	while (Position!=NULL)
	{
		m_MapServerID.GetNextAssoc(Position,wServerID,pGlobalServerItem);
		if (pGlobalServerItem!=NULL) FreeGlobalServerItem(pGlobalServerItem);
	}

	//删除聊天
	WORD wChatID=0;
	CGlobalChatServerItem * pGlobalChatItem=NULL;
	Position=m_MapChatID.GetStartPosition();

	//删除广场
	while (Position!=NULL)
	{
		m_MapChatID.GetNextAssoc(Position,wPlazaID,pGlobalChatItem);
		if (pGlobalChatItem!=NULL) FreeGlobalChatItem(pGlobalChatItem);
	}

	//删除索引
	m_MapUserID.RemoveAll();
	m_MapChatUserID.RemoveAll();
	m_MapDummyInfo.RemoveAll();
	m_MapGameID.RemoveAll();
	m_MapPlazaID.RemoveAll();
	m_MapServerID.RemoveAll();
	m_MapNickName.RemoveAll();
	m_MapChatID.RemoveAll();

	return;
}

//删除用户
bool CGlobalInfoManager::DeleteUserItem(DWORD dwUserID, WORD wServerID)
{
	//寻找用户
	CGlobalUserItem * pGlobalUserItem=NULL;
	if (m_MapUserID.Lookup(dwUserID,pGlobalUserItem)==FALSE) 
	{
		ASSERT(FALSE);
		return false;
	}

	//变量定义
	CGlobalServerItem * pGlobalServerItem=NULL;
	INT_PTR nServerCount=pGlobalUserItem->m_GlobalServerItemArray.GetCount();

	//退出房间
	for (INT_PTR i=0;i<nServerCount;i++)
	{
		//获取房间
		pGlobalServerItem=pGlobalUserItem->m_GlobalServerItemArray[i];

		//房间判断
		if (pGlobalServerItem->GetServerID()==wServerID)
		{
			//删除关联
			pGlobalServerItem->m_MapUserID.RemoveKey(dwUserID);
			pGlobalUserItem->m_GlobalServerItemArray.RemoveAt(i);

			//释放用户
			if (pGlobalUserItem->m_GlobalServerItemArray.GetCount()==0L) 
			{
				//释放索引
				m_MapUserID.RemoveKey(dwUserID);
				m_MapGameID.RemoveKey(pGlobalUserItem->GetGameID());
				m_MapNickName.RemoveKey(pGlobalUserItem->GetNickName());

				//释放对象
				FreeGlobalUserItem(pGlobalUserItem);
			}

			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//激活用户
bool CGlobalInfoManager::ActiveUserItem(tagGlobalUserInfo & GlobalUserInfo, WORD wServerID)
{
	//寻找房间
	CGlobalServerItem * pGlobalServerItem=NULL;
	if (m_MapServerID.Lookup(wServerID,pGlobalServerItem)==FALSE)
	{
		ASSERT(FALSE);
		return false;
	}

	//寻找用户
	CGlobalUserItem * pGlobalUserItem=NULL;
	if (m_MapUserID.Lookup(GlobalUserInfo.dwUserID,pGlobalUserItem)==FALSE)
	{
		//创建用户
		pGlobalUserItem=CreateGlobalUserItem();
		if (pGlobalUserItem==NULL) 
		{
			ASSERT(FALSE);
			return false;
		}

		//构造昵称
		TCHAR szNickName[LEN_NICKNAME]=TEXT("");
		lstrcpyn(szNickName,GlobalUserInfo.szNickName,CountArray(szNickName));

		//设置用户
		pGlobalUserItem->m_dwUserID=GlobalUserInfo.dwUserID;
		pGlobalUserItem->m_dwGameID=GlobalUserInfo.dwGameID;
		pGlobalUserItem->m_cbAndroidUser=GlobalUserInfo.cbAndroidUser;
		lstrcpyn(pGlobalUserItem->m_szNickName,GlobalUserInfo.szNickName,CountArray(pGlobalUserItem->m_szNickName));

		//辅助信息
		pGlobalUserItem->m_cbGender=GlobalUserInfo.cbGender;
		pGlobalUserItem->m_cbMemberOrder=GlobalUserInfo.cbMemberOrder;
		pGlobalUserItem->m_cbMasterOrder=GlobalUserInfo.cbMasterOrder;
		memcpy(&pGlobalUserItem->m_UserInfo, &GlobalUserInfo.userInfo, sizeof(tagUserInfo));

		//昵称索引
		m_MapNickName[szNickName]=pGlobalUserItem;

		//设置索引
		m_MapUserID[GlobalUserInfo.dwUserID]=pGlobalUserItem;
		m_MapGameID[GlobalUserInfo.dwGameID]=pGlobalUserItem;
	}
	else
	{
		//重复判断
		for (INT_PTR i=0;i<pGlobalUserItem->m_GlobalServerItemArray.GetCount();i++)
		{
			if (pGlobalUserItem->m_GlobalServerItemArray[i]->GetServerID()==wServerID)
			{
				ASSERT(FALSE);
				return false;
			}
		}
	}

	//设置关联
	pGlobalUserItem->m_GlobalServerItemArray.Add(pGlobalServerItem);
	pGlobalServerItem->m_MapUserID[GlobalUserInfo.dwUserID]=pGlobalUserItem;

	return true;
}

//删除用户
bool CGlobalInfoManager::DeleteChatUserItem(DWORD dwUserID, WORD wServerID)
{
	//寻找用户
	CGlobalChatUserItem * pGlobalUserItem=NULL;
	if (m_MapChatUserID.Lookup(dwUserID,pGlobalUserItem)==FALSE) 
	{
		ASSERT(FALSE);
		return false;
	}

	//变量定义
	CGlobalChatServerItem * pGlobalServerItem=NULL;
	INT_PTR nServerCount=pGlobalUserItem->m_GlobalServerItemArray.GetCount();

	//退出房间
	for (INT_PTR i=0;i<nServerCount;i++)
	{
		//获取房间
		pGlobalServerItem=pGlobalUserItem->m_GlobalServerItemArray[i];

		//房间判断
		if (wServerID == 0)
		{
			//删除关联
			pGlobalServerItem->m_MapUserID.RemoveKey(dwUserID);
			pGlobalUserItem->m_GlobalServerItemArray.RemoveAt(i);

			//释放用户
			if (pGlobalUserItem->m_GlobalServerItemArray.GetCount()==0L) 
			{
				//释放索引
				m_MapChatUserID.RemoveKey(dwUserID);

				//释放对象
				FreeGlobalChatUserItem(pGlobalUserItem);
			}
		}
		else
		{
			if (pGlobalServerItem->GetIndex()==wServerID)
			{
				//删除关联
				pGlobalServerItem->m_MapUserID.RemoveKey(dwUserID);
				pGlobalUserItem->m_GlobalServerItemArray.RemoveAt(i);

				//释放用户
				if (pGlobalUserItem->m_GlobalServerItemArray.GetCount()==0L) 
				{
					//释放索引
					m_MapChatUserID.RemoveKey(dwUserID);

					//释放对象
					FreeGlobalChatUserItem(pGlobalUserItem);
				}

				return true;
			}
		}
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//激活用户
bool CGlobalInfoManager::ActiveChatUserItem(tagGlobalChatUserInfo & GlobalUserInfo, WORD wServerID)
{
	//寻找聊天服务器
	CGlobalChatServerItem * pGlobalChatItem=NULL;
	if (m_MapChatID.Lookup(wServerID,pGlobalChatItem)==FALSE)
	{
		ASSERT(FALSE);
		return false;
	}

	//寻找聊天列表用户
	CGlobalChatUserItem * pGlobalUserItem=NULL;
	if (m_MapChatUserID.Lookup(GlobalUserInfo.dwUserID,pGlobalUserItem)==FALSE)
	{
		//创建用户
		pGlobalUserItem=CreateGlobalChatUserItem();
		if (pGlobalUserItem==NULL) 
		{
			ASSERT(FALSE);
			return false;
		}

		//设置用户
		pGlobalUserItem->m_dwUserID=GlobalUserInfo.dwUserID;
		//构造昵称
		lstrcpyn(pGlobalUserItem->m_szNickName,GlobalUserInfo.szNickName,CountArray(pGlobalUserItem->m_szNickName));

		//设置索引
		m_MapChatUserID[GlobalUserInfo.dwUserID]=pGlobalUserItem;
	}
	else
	{
		//重复判断
		for (INT_PTR i=0;i<pGlobalUserItem->m_GlobalServerItemArray.GetCount();i++)
		{
			if (pGlobalUserItem->m_GlobalServerItemArray[i]->GetIndex()==wServerID)
			{
				ASSERT(FALSE);
				return false;
			}
		}
	}

	//设置关联
	pGlobalUserItem->m_GlobalServerItemArray.Add(pGlobalChatItem);
	pGlobalChatItem->m_MapUserID[GlobalUserInfo.dwUserID]=pGlobalUserItem;

	return true;
}


//删除虚拟用户
bool CGlobalInfoManager::DeleteDummyItem(WORD wServerID)
{
	//寻找虚拟用户
	CGlobalDummyItem * pGlobalDummyItem=NULL;
	if (m_MapDummyInfo.Lookup(wServerID,pGlobalDummyItem)==FALSE)
	{
		ASSERT(FALSE);
		return false;
	}

	//释放广场
	m_MapDummyInfo.RemoveKey(wServerID);
	FreeGlobalDummyItem(pGlobalDummyItem);

	return true;
}

bool CGlobalInfoManager::ClearDummyItem()
{
	WORD wServerID;
	CGlobalDummyItem* curItem;
	POSITION Position=m_MapDummyInfo.GetStartPosition();
	while (Position!=NULL)
	{
		m_MapDummyInfo.GetNextAssoc(Position,wServerID,curItem);
		FreeGlobalDummyItem(curItem);
	}

	m_MapDummyInfo.RemoveAll();
	return true;
}

//激活虚拟用户
bool CGlobalInfoManager::ActiveDummyItem(tagDummyInfo & DummyInfo, WORD wServerID)
{
	//寻找聊天列表用户
	CGlobalDummyItem * pGlobalDummyItem=NULL;
	if (m_MapDummyInfo.Lookup(wServerID,pGlobalDummyItem)==FALSE)
	{//创建虚拟用户
		pGlobalDummyItem=CreateGlobalDummyItem();
		if (pGlobalDummyItem==NULL) 
		{
			ASSERT(FALSE);
			return false;
		}

		//设置数据
		pGlobalDummyItem->wServerID=wServerID;
		CopyMemory(&pGlobalDummyItem->m_DummyInfo, &DummyInfo, sizeof(tagDummyInfo));

		//设置索引
		m_MapDummyInfo[wServerID]=pGlobalDummyItem;
	}
	else
	{//更新数据
		//设置数据
		pGlobalDummyItem->wServerID=wServerID;
		CopyMemory(&pGlobalDummyItem->m_DummyInfo, &DummyInfo, sizeof(tagDummyInfo));
	}
	return true;
}

//删除广场
bool CGlobalInfoManager::DeletePlazaItem(WORD wPlazaID)
{
	//寻找广场
	CGlobalPlazaItem * pGlobalPlazaItem=NULL;
	if (m_MapPlazaID.Lookup(wPlazaID,pGlobalPlazaItem)==FALSE)
	{
		ASSERT(FALSE);
		return false;
	}

	//释放广场
	m_MapPlazaID.RemoveKey(wPlazaID);
	FreeGlobalPlazaItem(pGlobalPlazaItem);

	return true;
}

//激活广场
bool CGlobalInfoManager::ActivePlazaItem(WORD wBindIndex, tagGamePlaza & GamePlaza)
{
	//寻找广场
	CGlobalPlazaItem * pGlobalPlazaItem=NULL;
	if (m_MapPlazaID.Lookup(GamePlaza.wPlazaID,pGlobalPlazaItem)==TRUE)
	{
		ASSERT(FALSE);
		return false;
	}

	//创建广场
	pGlobalPlazaItem=CreateGlobalPlazaItem();
	if (pGlobalPlazaItem==NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	//设置广场
	pGlobalPlazaItem->m_wIndex=wBindIndex;
	pGlobalPlazaItem->m_GamePlaza=GamePlaza;

	//设置索引
	m_MapPlazaID[GamePlaza.wPlazaID]=pGlobalPlazaItem;

	return true;
}

//删除房间
bool CGlobalInfoManager::DeleteServerItem(WORD wServerID)
{
	//寻找房间
	CGlobalServerItem * pGlobalServerItem=NULL;
	if (m_MapServerID.Lookup(wServerID,pGlobalServerItem)==FALSE)
	{
		ASSERT(FALSE);
		return false;
	}

	//变量定义
	DWORD dwUserKey=0L;
	CGlobalUserItem * pGlobalUserItem=NULL;
	POSITION Position=pGlobalServerItem->m_MapUserID.GetStartPosition();

	//删除用户
	while (Position!=NULL)
	{
		//获取数据
		pGlobalServerItem->m_MapUserID.GetNextAssoc(Position,dwUserKey,pGlobalUserItem);

		//房间关联
		for (INT_PTR i=0;i<pGlobalUserItem->m_GlobalServerItemArray.GetCount();i++)
		{
			//获取房间
			CGlobalServerItem * pTempServerItem=pGlobalUserItem->m_GlobalServerItemArray[i];

			//房间判断
			if (pTempServerItem->GetServerID()==wServerID)
			{
				pGlobalUserItem->m_GlobalServerItemArray.RemoveAt(i);
				break;
			}
		}

		//释放用户
		if (pGlobalUserItem->m_GlobalServerItemArray.GetCount()==0)
		{
			m_MapUserID.RemoveKey(dwUserKey);
			FreeGlobalUserItem(pGlobalUserItem);
		}
	}

	//释放房间
	m_MapServerID.RemoveKey(wServerID);
	FreeGlobalServerItem(pGlobalServerItem);

	//查找房间
	int nSize = m_ServerTableCountArray.GetCount();
	for(int i = 0; i < nSize; ++i)
	{
		tagServerTableCount* pServerTableCount = m_ServerTableCountArray.GetAt(i);
		if(pServerTableCount->dwServerID == wServerID)
		{
			SafeDelete(pServerTableCount);
			m_ServerTableCountArray.RemoveAt(i);
			break;
		}
	}

	return true;
}

//激活房间
bool CGlobalInfoManager::ActiveServerItem(WORD wBindIndex, tagGameServer & GameServer)
{
	//寻找房间
	CGlobalServerItem * pGlobalServerItem=NULL;
	if (m_MapServerID.Lookup(GameServer.wServerID,pGlobalServerItem)==TRUE)
	{
		ASSERT(FALSE);
		return false;
	}

	//创建房间
	pGlobalServerItem=CreateGlobalServerItem();
	if (pGlobalServerItem==NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	//设置房间
	pGlobalServerItem->m_wIndex=wBindIndex;
	pGlobalServerItem->m_GameServer=GameServer;

	//设置索引
	m_MapServerID[GameServer.wServerID]=pGlobalServerItem;

	//插入房间
	DWORD dwServerID = pGlobalServerItem->GetServerID();
	DWORD dwTableCount = pGlobalServerItem->GetTabelCount();

	//查找房间
	int nSize = m_ServerTableCountArray.GetCount();
	bool bExit = false;
	for(int i = 0; i < nSize; ++i)
	{
		tagServerTableCount* pServerTableCount = m_ServerTableCountArray.GetAt(i);
		if(pServerTableCount != NULL && pServerTableCount->dwServerID == dwServerID)
		{
			bExit = true;
			break;
		}
	}

	return true;
}

//删除聊天
bool CGlobalInfoManager::DeleteChatItem(WORD wChatID)
{
	//寻找聊天项
	CGlobalChatServerItem * pGlobalChatItem=NULL;
	if (m_MapChatID.Lookup(wChatID,pGlobalChatItem)==FALSE)
	{
		ASSERT(FALSE);
		return false;
	}

	//释放聊天项
	m_MapChatID.RemoveKey(wChatID);
	FreeGlobalChatItem(pGlobalChatItem);

	return true;
}

//激活聊天
bool CGlobalInfoManager::ActiveChatItem(WORD wBindIndex, tagChatServer & ChatServer)
{
	//寻找房间
	CGlobalChatServerItem * pGlobalChatItem=NULL;
	if (m_MapChatID.Lookup(ChatServer.wChatID,pGlobalChatItem)==TRUE)
	{
		ASSERT(FALSE);
		return false;
	}

	//创建房间
	pGlobalChatItem=CreateGlobalChatItem();
	if (pGlobalChatItem==NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	//设置房间
	pGlobalChatItem->m_wIndex=wBindIndex;
	pGlobalChatItem->m_ChatServer=ChatServer;

	//设置索引
	m_MapChatID[ChatServer.wChatID]=pGlobalChatItem;

	return true;
}

//寻找广场
CGlobalPlazaItem * CGlobalInfoManager::SearchPlazaItem(WORD wPlazaID)
{
	//寻找房间
	CGlobalPlazaItem * pGlobalPlazaItem=NULL;
	m_MapPlazaID.Lookup(wPlazaID,pGlobalPlazaItem);

	return pGlobalPlazaItem;
}

//寻找房间
CGlobalServerItem * CGlobalInfoManager::SearchServerItem(WORD wServerID)
{
	//寻找房间
	CGlobalServerItem * pGlobalServerItem=NULL;
	m_MapServerID.Lookup(wServerID,pGlobalServerItem);

	return pGlobalServerItem;
}

//寻找聊天
CGlobalChatServerItem * CGlobalInfoManager::SearchChatServerItem(WORD wChatID)
{
	//寻找房间
	CGlobalChatServerItem * pGlobalChatItem=NULL;
	m_MapChatID.Lookup(wChatID,pGlobalChatItem);

	return pGlobalChatItem;
}

//寻找虚拟人数
CGlobalDummyItem * CGlobalInfoManager::SearchDummyItem(WORD wServerID)
{
	//寻找房间
	CGlobalDummyItem * pGlobalItem=NULL;
	m_MapDummyInfo.Lookup(wServerID,pGlobalItem);

	return pGlobalItem;
}

//寻找用户
CGlobalUserItem * CGlobalInfoManager::SearchUserItemByUserID(DWORD dwUserID)
{
	//寻找用户
	CGlobalUserItem * pGlobalUserItem=NULL;
	m_MapUserID.Lookup(dwUserID,pGlobalUserItem);

	return pGlobalUserItem;
}

//寻找用户
CGlobalChatUserItem * CGlobalInfoManager::SearchChatUserItemByUserID(DWORD dwUserID)
{
	//寻找用户
	CGlobalChatUserItem * pGlobalUserItem=NULL;
	m_MapChatUserID.Lookup(dwUserID,pGlobalUserItem);

	return pGlobalUserItem;
}

//寻找用户
CGlobalUserItem * CGlobalInfoManager::SearchUserItemByGameID(DWORD dwGameID)
{
	//寻找用户
	CGlobalUserItem * pGlobalUserItem=NULL;
	m_MapGameID.Lookup(dwGameID,pGlobalUserItem);

	return pGlobalUserItem;
}

//寻找用户
CGlobalUserItem * CGlobalInfoManager::SearchUserItemByNickName(LPCTSTR pszNickName)
{
	//寻找用户
	VOID * pGlobalUserItem=NULL;
	m_MapNickName.Lookup(pszNickName,pGlobalUserItem);

	return (CGlobalUserItem *)pGlobalUserItem;
}

//枚举用户
CGlobalUserItem * CGlobalInfoManager::EnumUserItem(POSITION & Position)
{
	//变量定义
	DWORD dwUserID=0L;
	CGlobalUserItem * pGlobalUserItem=NULL;

	//获取对象
	if (Position==NULL) Position=m_MapUserID.GetStartPosition();
	if (Position!=NULL)	m_MapUserID.GetNextAssoc(Position,dwUserID,pGlobalUserItem);

	return pGlobalUserItem;
}

//枚举用户
CGlobalChatUserItem * CGlobalInfoManager::EnumChatUserItem(POSITION & Position)
{
	//变量定义
	DWORD dwUserID=0L;
	CGlobalChatUserItem * pGlobalUserItem=NULL;

	//获取对象
	if (Position==NULL) Position=m_MapChatUserID.GetStartPosition();
	if (Position!=NULL)	m_MapChatUserID.GetNextAssoc(Position,dwUserID,pGlobalUserItem);

	return pGlobalUserItem;
}

//枚举广场
CGlobalPlazaItem * CGlobalInfoManager::EnumPlazaItem(POSITION & Position)
{
	//变量定义
	WORD wPlazaID=0L;
	CGlobalPlazaItem * pGlobalPlazaItem=NULL;

	//获取对象
	if (Position==NULL) Position=m_MapPlazaID.GetStartPosition();
	if (Position!=NULL)	m_MapPlazaID.GetNextAssoc(Position,wPlazaID,pGlobalPlazaItem);

	return pGlobalPlazaItem;
}

//枚举房间
CGlobalServerItem * CGlobalInfoManager::EnumServerItem(POSITION & Position)
{
	//变量定义
	WORD wServerID=0L;
	CGlobalServerItem * pGlobalServerItem=NULL;

	//获取对象
	if (Position==NULL) Position=m_MapServerID.GetStartPosition();
	if (Position!=NULL)	m_MapServerID.GetNextAssoc(Position,wServerID,pGlobalServerItem);

	return pGlobalServerItem;
}

//枚举聊天
CGlobalChatServerItem * CGlobalInfoManager::EnumChatItem(POSITION & Position)
{
	//变量定义
	WORD wChatID=0L;
	CGlobalChatServerItem * pGlobalChatItem=NULL;

	//获取对象
	if (Position==NULL) Position=m_MapChatID.GetStartPosition();
	if (Position!=NULL)	m_MapChatID.GetNextAssoc(Position,wChatID,pGlobalChatItem);

	return pGlobalChatItem;
}

//创建用户
CGlobalUserItem * CGlobalInfoManager::CreateGlobalUserItem()
{
	//使用存储
	if (m_pGlobalUserItem!=NULL)
	{
		CGlobalUserItem * pGlobalUserItem=m_pGlobalUserItem;
		m_pGlobalUserItem=m_pGlobalUserItem->m_pNextUserItemPtr;
		pGlobalUserItem->m_pNextUserItemPtr=NULL;
		return pGlobalUserItem;
	}

	//创建对象
	try 
	{ 
		CGlobalUserItem * pGlobalUserItem=new CGlobalUserItem;
		return pGlobalUserItem;
	}
	catch (...) {}

	return NULL;
}

//创建聊天用户
CGlobalChatUserItem * CGlobalInfoManager::CreateGlobalChatUserItem()
{
	//使用存储
	if (m_pGlobalChatUserItem!=NULL)
	{
		CGlobalChatUserItem * pGlobalUserItem=m_pGlobalChatUserItem;
		m_pGlobalChatUserItem=m_pGlobalChatUserItem->m_pNextUserItemPtr;
		pGlobalUserItem->m_pNextUserItemPtr=NULL;
		return pGlobalUserItem;
	}

	//创建对象
	try 
	{ 
		CGlobalChatUserItem * pGlobalUserItem=new CGlobalChatUserItem;
		return pGlobalUserItem;
	}
	catch (...) {}

	return NULL;
}

//创建广场
CGlobalPlazaItem * CGlobalInfoManager::CreateGlobalPlazaItem()
{
	//使用存储
	if (m_pGlobalPlazaItem!=NULL)
	{
		CGlobalPlazaItem * pGlobalPlazaItem=m_pGlobalPlazaItem;
		m_pGlobalPlazaItem=m_pGlobalPlazaItem->m_pNextPlazaItemPtr;
		pGlobalPlazaItem->m_pNextPlazaItemPtr=NULL;
		return pGlobalPlazaItem;
	}

	//创建对象
	try 
	{ 
		CGlobalPlazaItem * pGlobalPlazaItem=new CGlobalPlazaItem;
		return pGlobalPlazaItem;
	}
	catch (...) {}

	return NULL;
}

//创建房间
CGlobalServerItem * CGlobalInfoManager::CreateGlobalServerItem()
{
	//使用存储
	if (m_pGlobalServerItem!=NULL)
	{
		CGlobalServerItem * pGlobalServerItem=m_pGlobalServerItem;
		m_pGlobalServerItem=m_pGlobalServerItem->m_pNextServerItemPtr;
		pGlobalServerItem->m_pNextServerItemPtr=NULL;
		return pGlobalServerItem;
	}

	//创建对象
	try 
	{ 
		CGlobalServerItem * pGlobalServerItem=new CGlobalServerItem;
		return pGlobalServerItem;
	}
	catch (...) {}

	return NULL;
}

//创建聊天
CGlobalChatServerItem * CGlobalInfoManager::CreateGlobalChatItem()
{
	//使用存储
	if (m_pGlobalChatServerItem!=NULL)
	{
		CGlobalChatServerItem * pGlobalChatItem=m_pGlobalChatServerItem;
		m_pGlobalChatServerItem=m_pGlobalChatServerItem->m_pNextChatServerPtr;
		pGlobalChatItem->m_pNextChatServerPtr=NULL;
		return pGlobalChatItem;
	}

	//创建对象
	try 
	{ 
		CGlobalChatServerItem * pGlobalChatItem=new CGlobalChatServerItem;
		return pGlobalChatItem;
	}
	catch (...) {}

	return NULL;
}

//创建虚拟用户
CGlobalDummyItem * CGlobalInfoManager::CreateGlobalDummyItem()
{
	//使用存储
	if (m_pGlobalDummyItem!=NULL)
	{
		CGlobalDummyItem * pGlobalDummyItem=m_pGlobalDummyItem;
		m_pGlobalDummyItem=m_pGlobalDummyItem->m_pNextItemPtr;
		pGlobalDummyItem->m_pNextItemPtr=NULL;
		return pGlobalDummyItem;
	}

	//创建对象
	try 
	{ 
		CGlobalDummyItem * pGlobalDummyItem=new CGlobalDummyItem;
		return pGlobalDummyItem;
	}
	catch (...)
	{
		return NULL;
	}
}

//释放用户
bool CGlobalInfoManager::FreeGlobalUserItem(CGlobalUserItem * pGlobalUserItem)
{
	//效验参数
	ASSERT(pGlobalUserItem!=NULL);
	if (pGlobalUserItem==NULL) return false;

	//设置变量
	pGlobalUserItem->m_dwUserID=0L;
	pGlobalUserItem->m_dwGameID=0L;
	pGlobalUserItem->m_szNickName[0]=0;
	ZeroMemory(&pGlobalUserItem->m_UserInfo, sizeof(tagUserInfo));
	pGlobalUserItem->m_GlobalServerItemArray.RemoveAll();

	//加入存储
	pGlobalUserItem->m_pNextUserItemPtr=m_pGlobalUserItem;
	m_pGlobalUserItem=pGlobalUserItem;

	return true;
}

//释放用户
bool CGlobalInfoManager::FreeGlobalChatUserItem(CGlobalChatUserItem * pGlobalUserItem)
{
	//效验参数
	ASSERT(pGlobalUserItem!=NULL);
	if (pGlobalUserItem==NULL) return false;

	//设置变量
	pGlobalUserItem->m_dwUserID=0L;
	pGlobalUserItem->m_szNickName[0]=0;
	pGlobalUserItem->m_GlobalServerItemArray.RemoveAll();

	//加入存储
	pGlobalUserItem->m_pNextUserItemPtr=m_pGlobalChatUserItem;
	m_pGlobalChatUserItem=pGlobalUserItem;

	return true;
}

//释放广场
bool CGlobalInfoManager::FreeGlobalPlazaItem(CGlobalPlazaItem * pGlobalPlazaItem)
{
	//效验参数
	ASSERT(pGlobalPlazaItem!=NULL);
	if (pGlobalPlazaItem==NULL) return false;

	//设置变量
	pGlobalPlazaItem->m_wIndex=0;
	ZeroMemory(&pGlobalPlazaItem->m_GamePlaza,sizeof(pGlobalPlazaItem->m_GamePlaza));

	//加入存储
	pGlobalPlazaItem->m_pNextPlazaItemPtr=m_pGlobalPlazaItem;
	m_pGlobalPlazaItem=pGlobalPlazaItem;

	return true;
}

//释放房间
bool CGlobalInfoManager::FreeGlobalServerItem(CGlobalServerItem * pGlobalServerItem)
{
	//效验参数
	ASSERT(pGlobalServerItem!=NULL);
	if (pGlobalServerItem==NULL) return false;

	//设置索引
	pGlobalServerItem->m_MapUserID.RemoveAll();

	//设置变量
	pGlobalServerItem->m_wIndex=0;
	ZeroMemory(&pGlobalServerItem->m_GameServer,sizeof(pGlobalServerItem->m_GameServer));

	//加入存储
	pGlobalServerItem->m_pNextServerItemPtr=m_pGlobalServerItem;
	m_pGlobalServerItem=pGlobalServerItem;

	return true;
}

//释放聊天
bool CGlobalInfoManager::FreeGlobalChatItem(CGlobalChatServerItem * pGlobalChatItem)
{
	//效验参数
	ASSERT(pGlobalChatItem!=NULL);
	if (pGlobalChatItem==NULL) return false;

	//设置变量
	pGlobalChatItem->m_wIndex=0;
	ZeroMemory(&pGlobalChatItem->m_ChatServer,sizeof(pGlobalChatItem->m_ChatServer));

	//加入存储
	pGlobalChatItem->m_pNextChatServerPtr=m_pGlobalChatServerItem;
	m_pGlobalChatServerItem=pGlobalChatItem;

	return true;
}

//释放虚拟用户
bool CGlobalInfoManager::FreeGlobalDummyItem(CGlobalDummyItem * pDummyItem)
{
	//效验参数
	ASSERT(pDummyItem!=NULL);
	if (pDummyItem==NULL) return false;

	//设置变量
	pDummyItem->wServerID=0;
	ZeroMemory(&pDummyItem->m_DummyInfo,sizeof(pDummyItem->m_DummyInfo));

	//加入存储
	pDummyItem->m_pNextItemPtr=m_pGlobalDummyItem;
	m_pGlobalDummyItem=pDummyItem;

	return true;
}

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
