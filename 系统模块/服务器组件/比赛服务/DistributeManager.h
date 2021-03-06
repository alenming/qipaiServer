#ifndef DISTRIBUTE_MANAGER_HEAD_FILE
#define DISTRIBUTE_MANAGER_HEAD_FILE

#pragma once

#include "MatchServiceHead.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//同桌信息
struct tagSameTableInfo
{
	WORD							wPlayerCount;					    //玩家数目
	DWORD							wPlayerIDSet[16];					//玩家集合
};

//分组数据
struct tagDistributeInfo
{
	WORD							wDistribute;						//分配等级	
	DWORD							dwClientAddr;						//玩家地址
	DWORD							dwInsertStamp;						//插入时间
	IServerUserItem *				pIServerUserItem;					//用户接口
	VOID *                          pPertainNode;                       //所属结点
};

//分组结点
struct tagDistributeNode
{
	tagDistributeNode *             pPrepDistributeNode;               //前一结点
	tagDistributeNode *			    pNextDistributeNode;               //下一结点
	tagDistributeInfo               DistributeInfo;                    //分组信息

	tagDistributeNode()
	{
		pPrepDistributeNode=NULL;
		pNextDistributeNode=NULL;
	}
	~tagDistributeNode()
	{
		pPrepDistributeNode=NULL;
		pNextDistributeNode=NULL;
	}
};  

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//类型定义
typedef CWHArray<tagSameTableInfo *> CSameTableArray;						//同坐数组
typedef CWHArray<tagDistributeInfo> CDistributeInfoArray;					//分组数组
typedef CMap<DWORD,DWORD,tagSameTableInfo *, tagSameTableInfo*>	CSameTableMap;	//同桌信息

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//结点池
class  CDistributeNodePool
{
	//管理变量
public:
	static const int BLOCK_SIZE;
	static tagDistributeNode * m_pHeadOfFreeList;

	//函数定义
public:
	//构造函数
	CDistributeNodePool();
	//析构函数
	~CDistributeNodePool();

	//功能函数
public:
	//分配结点
	tagDistributeNode * AllocNode();
	//释放结点
	VOID FreeNode(void * pNode);
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//分组管理
class CDistributeManager
{
	//数目变量
protected:
	WORD							m_wNodeCount;				      //结点数目
	WORD                            m_wRealCount;	                  //真人数目
	WORD                            m_wAndroidCount;		          //机器数目 

	//存储变量
protected:
	CSameTableArray					m_SameTableBuffer;				  //同桌缓冲
	CSameTableArray					m_SameTableActive;				  //同桌数组
	CSameTableMap					m_SameTableMap;					  //同桌字典

	//管理变量
protected:
	CDistributeNodePool             m_DistributeNodePool;             //结点池 
	tagDistributeNode *				m_pHeadNode;					  //头部结点

	//分组变量
protected:
	BYTE							m_cbDistributeRule;				  //分组规则

	//函数定义
public:
	//构造函数
	CDistributeManager();
	//析构函数
	virtual ~CDistributeManager();

	//内联函数
public:
	//结点数目
	inline WORD  GetCount() { return m_wNodeCount; }
	//真人数目
	inline WORD  GetRealCount() { return m_wRealCount; };
	//机器数目
	inline WORD  GetAndroidCount() { return m_wAndroidCount; };
	//设置规则
	inline VOID  SetDistributeRule(BYTE cbDistributeRule) { m_cbDistributeRule=cbDistributeRule; }

	//功能函数
public:	
	//移除结点
	VOID RemoveAll();
	//插入结点
	BOOL InsertDistributeNode(const tagDistributeInfo & DistributeInfo);
	//移除结点
	VOID RemoveDistributeNode(tagDistributeNode * pDistributeNode);
	//移除结点
	VOID RemoveDistributeNode(IServerUserItem  *  pIServerUserItem);	
	//执行分组
	WORD PerformDistribute(CDistributeInfoArray & DistributeInfoArray,WORD wNeedCount);

	//功能函数
public:
	//查找结点
	tagDistributeNode* SearchNode(IServerUserItem  * pIServerUserItem);
	//获取信息
	tagSameTableInfo * GetUserSameTableInfo(DWORD dwUserID);
	//移除信息
	VOID RemoveUserSameTableInfo(DWORD dwUserID);

	//内部函数
protected:
	//激活对象
	tagSameTableInfo * ActiveSameTableInfo();
	//移除对象
	VOID RemoveSameTableInfo(tagSameTableInfo * pSameTableInfo);	

	//规则函数
protected:
	//IP同址
	inline BOOL FilterRuleExitsIPAddr(const CDistributeInfoArray & DistributeInfoArray,DWORD dwClientAddr);	
	//机器过滤
	inline BOOL FilterRuleIsAllAndroid(const CDistributeInfoArray & DistributeInfoArray, IServerUserItem  * const pIServerUserItem);
	//上局同桌
	inline BOOL FilterRuleIsLastSameTable(const CDistributeInfoArray & DistributeInfoArray,DWORD dwUserID);
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

#endif