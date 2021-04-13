#ifndef WH_ENCRYPT_HEAD_FILE
#define WH_ENCRYPT_HEAD_FILE

#pragma once

#include "ServiceCoreHead.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//宏定义
#define XOR_TIMES					8									//加密倍数
#define MAX_SOURCE_LEN				64									//最大长度
#define MAX_ENCRYPT_LEN				(MAX_SOURCE_LEN*XOR_TIMES)			//最大长度

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//加密组件
class SERVICE_CORE_CLASS CWHEncrypt
{
	//函数定义
public:
	//构造函数
	CWHEncrypt();
	//析构函数
	virtual ~CWHEncrypt();

	//加密函数
public:
	//生成密文
	static bool MD5Encrypt(LPCTSTR pszSourceData, TCHAR szMD5Result[LEN_MD5]);

	//加密函数
public:
	//生成密文
	static bool XorEncrypt(LPCTSTR pszSourceData, LPTSTR pszEncrypData, WORD wMaxCount);
	//解开密文
	static bool XorCrevasse(LPCTSTR pszEncrypData, LPTSTR pszSourceData, WORD wMaxCount);

	//加密函数
public:
	//生成密文
	static bool MapEncrypt(LPCTSTR pszSourceData, LPTSTR pszEncrypData, WORD wMaxCount);
	//解开密文
	static bool MapCrevasse(LPCTSTR pszEncrypData, LPTSTR pszSourceData, WORD wMaxCount);

	//des加密函数
public:
	//des加密
	static bool DesEncrypt(LPCTSTR pszSourceData,LPTSTR pszEncrypData);
	//des解密
	static bool DesCrevasse(LPCTSTR pszEncrypData,LPTSTR pszSourceData);
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

#endif