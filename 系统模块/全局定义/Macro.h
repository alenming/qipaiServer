#ifndef MACRO_HEAD_FILE
#define MACRO_HEAD_FILE

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//���ó���

//��Ч��ֵ
#define INVALID_BYTE				((BYTE)(0xFF))						//��Ч��ֵ
#define INVALID_WORD				((WORD)(0xFFFF))					//��Ч��ֵ
#define INVALID_DWORD				((DWORD)(0xFFFFFFFF))				//��Ч��ֵ

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//����ά��
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

//��Ч��ַ
#define INVALID_IP_ADDRESS(IPAddress) (((IPAddress==0L)||(IPAddress==INADDR_NONE)))

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//�洢����
#ifdef _UNICODE
	#define CountStringBuffer CountStringBufferW
#else
	#define CountStringBuffer CountStringBufferA
#endif

//�洢����
#define CountStringBufferA(String) ((UINT)((lstrlenA(String)+1)*sizeof(CHAR)))
#define CountStringBufferW(String) ((UINT)((lstrlenW(String)+1)*sizeof(WCHAR)))

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//�ӿ��ͷ�
#define SafeRelease(pObject) { if (pObject!=NULL) { pObject->Release(); pObject=NULL; } }

//ɾ��ָ��
#define SafeDelete(pData) { try { delete pData; } catch (...) { ASSERT(FALSE); } pData=NULL; } 

//�رվ��
#define SafeCloseHandle(hHandle) { if (hHandle!=NULL) { CloseHandle(hHandle); hHandle=NULL; } }

//ɾ������
#define SafeDeleteArray(pData) { try { delete [] pData; } catch (...) { ASSERT(FALSE); } pData=NULL; } 

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//ת���ַ�
inline VOID SwitchScoreFormat(LONGLONG lScore, UINT uSpace,LPCTSTR pszFormat,LPTSTR pszBuffer, WORD wBufferSize)
{
	//ת����ֵ
	TCHAR szSwitchScore[32]=TEXT("");
	_sntprintf(szSwitchScore,CountArray(szSwitchScore),pszFormat,lScore);

	if(uSpace>0)
	{
		//��������
		WORD wTargetIndex=0;
		WORD wSourceIndex=0;
		WORD wSourceStringLen=0;

		//���㳤��
		while (szSwitchScore[wSourceStringLen]!=0 && szSwitchScore[wSourceStringLen]!='.') 
		{
			++wSourceStringLen;
		}

		//ת���ַ�
		for (INT i=0;i<wSourceStringLen;i++)
		{
			//�����ַ�
			pszBuffer[wTargetIndex++]=szSwitchScore[wSourceIndex++];

			//���붺��
			if ((wSourceStringLen-wSourceIndex>0) && ((wSourceStringLen-wSourceIndex)%uSpace==0)) 
			{
				pszBuffer[wTargetIndex++]=TEXT(',');
			}			
		}

		//����С��	
		while (szSwitchScore[wSourceIndex]!=0)
		{
			pszBuffer[wTargetIndex++] = szSwitchScore[wSourceIndex++];
		}

		//�����ַ�
		pszBuffer[wTargetIndex++]=0;
	}
	else
	{
		CopyMemory(pszBuffer,szSwitchScore,wBufferSize);
	}

	return;
}


//ת���ַ�
inline VOID SwitchScoreFormat(double lScore, UINT uSpace,LPCTSTR pszFormat,LPTSTR pszBuffer, WORD wBufferSize)
{
	//ת����ֵ
	TCHAR szSwitchScore[32]=TEXT("");
	_sntprintf(szSwitchScore,CountArray(szSwitchScore),pszFormat,lScore);

	if(uSpace>0)
	{
		//��������
		WORD wTargetIndex=0;
		WORD wSourceIndex=0;
		WORD wSourceStringLen=0;

		//���㳤��
		while (szSwitchScore[wSourceStringLen]!=0 && szSwitchScore[wSourceStringLen]!='.') 
		{
			++wSourceStringLen;
		}

		//ת���ַ�
		for (INT i=0;i<wSourceStringLen;i++)
		{
			//�����ַ�
			pszBuffer[wTargetIndex++]=szSwitchScore[wSourceIndex++];

			//���붺��
			if ((wSourceStringLen-wSourceIndex>0) && ((wSourceStringLen-wSourceIndex)%uSpace==0)) 
			{
				pszBuffer[wTargetIndex++]=TEXT(',');
			}			
		}

		//����С��	
		while (szSwitchScore[wSourceIndex]!=0)
		{
			pszBuffer[wTargetIndex++] = szSwitchScore[wSourceIndex++];
		}

		//�����ַ�
		pszBuffer[wTargetIndex++]=0;
	}
	else
	{
		CopyMemory(pszBuffer,szSwitchScore,wBufferSize);
	}

	return;
}

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

#endif