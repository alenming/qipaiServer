#ifndef WH_ENCRYPT_HEAD_FILE
#define WH_ENCRYPT_HEAD_FILE

#pragma once

#include "ServiceCoreHead.h"

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//�궨��
#define XOR_TIMES					8									//���ܱ���
#define MAX_SOURCE_LEN				64									//��󳤶�
#define MAX_ENCRYPT_LEN				(MAX_SOURCE_LEN*XOR_TIMES)			//��󳤶�

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//�������
class SERVICE_CORE_CLASS CWHEncrypt
{
	//��������
public:
	//���캯��
	CWHEncrypt();
	//��������
	virtual ~CWHEncrypt();

	//���ܺ���
public:
	//��������
	static bool MD5Encrypt(LPCTSTR pszSourceData, TCHAR szMD5Result[LEN_MD5]);

	//���ܺ���
public:
	//��������
	static bool XorEncrypt(LPCTSTR pszSourceData, LPTSTR pszEncrypData, WORD wMaxCount);
	//�⿪����
	static bool XorCrevasse(LPCTSTR pszEncrypData, LPTSTR pszSourceData, WORD wMaxCount);

	//���ܺ���
public:
	//��������
	static bool MapEncrypt(LPCTSTR pszSourceData, LPTSTR pszEncrypData, WORD wMaxCount);
	//�⿪����
	static bool MapCrevasse(LPCTSTR pszEncrypData, LPTSTR pszSourceData, WORD wMaxCount);

	//des���ܺ���
public:
	//des����
	static bool DesEncrypt(LPCTSTR pszSourceData,LPTSTR pszEncrypData);
	//des����
	static bool DesCrevasse(LPCTSTR pszEncrypData,LPTSTR pszSourceData);
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

#endif