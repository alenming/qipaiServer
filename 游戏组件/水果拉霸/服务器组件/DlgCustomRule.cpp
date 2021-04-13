// DlgCustomRule.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Resource.h"
#include "DlgCustomRule.h"
#include <math.h>


// CDlgCustomRule �Ի���

BEGIN_MESSAGE_MAP(CDlgCustomRule, CDialog)
END_MESSAGE_MAP()

CDlgCustomRule::CDlgCustomRule() : CDialog(IDD_CUSTOM_RULE)
{
	//���ñ���
	ZeroMemory(&m_CustomRule, sizeof(m_CustomRule));



	m_CustomRule.cbBetCount = 10;
	m_CustomRule.lBetScore[0] = 0.45;
	m_CustomRule.lBetScore[1] = 0.9;
	m_CustomRule.lBetScore[2] = 1.8;
	m_CustomRule.lBetScore[3] = 4.5;
	m_CustomRule.lBetScore[4] = 9;
	m_CustomRule.lBetScore[5] = 18;
	m_CustomRule.lBetScore[6] = 45;
	m_CustomRule.lBetScore[7] = 90;
	m_CustomRule.lBetScore[8] = 180;
	m_CustomRule.lBetScore[9] = 360;
}

CDlgCustomRule::~CDlgCustomRule()
{
}

//���ú���
BOOL CDlgCustomRule::OnInitDialog()
{
	__super::OnInitDialog();

	//���ÿؼ���Χ
	((CEdit *)GetDlgItem(IDC_EDIT_BET_SCORE_1))->LimitText(9);
	((CEdit *)GetDlgItem(IDC_EDIT_BET_SCORE_2))->LimitText(9);
	((CEdit *)GetDlgItem(IDC_EDIT_BET_SCORE_3))->LimitText(9);
	((CEdit *)GetDlgItem(IDC_EDIT_BET_SCORE_4))->LimitText(9);
	((CEdit *)GetDlgItem(IDC_EDIT_BET_SCORE_5))->LimitText(9);
	((CEdit *)GetDlgItem(IDC_EDIT_BET_SCORE_6))->LimitText(9);
	((CEdit *)GetDlgItem(IDC_EDIT_BET_SCORE_7))->LimitText(9);
	((CEdit *)GetDlgItem(IDC_EDIT_BET_SCORE_8))->LimitText(9);
	((CEdit *)GetDlgItem(IDC_EDIT_BET_SCORE_9))->LimitText(9);
	((CEdit *)GetDlgItem(IDC_EDIT_BET_SCORE_10))->LimitText(9);

	//���²���
	FillDataToControl();

	return FALSE;
}

//ȷ������
VOID CDlgCustomRule::OnOK() 
{ 
	//Ͷ����Ϣ
	GetParent()->PostMessage(WM_COMMAND, MAKELONG(IDOK, 0), 0);

	return;
}

//ȡ����Ϣ
VOID CDlgCustomRule::OnCancel() 
{ 
	//Ͷ����Ϣ
	GetParent()->PostMessage(WM_COMMAND, MAKELONG(IDCANCEL, 0), 0);

	return;
}

//���¿ؼ�
bool CDlgCustomRule::FillDataToControl()
{
	//��������
	TCHAR szBuffer[16] = TEXT("");
	for (int i = 0; i < 10; i++)
	{
		_sntprintf(szBuffer, sizeof(szBuffer), TEXT("%g"), m_CustomRule.lBetScore[i]);

		((CEdit*)(GetDlgItem(IDC_EDIT_BET_SCORE_1 + i)))->SetWindowText(szBuffer);
	}

	return true;
}

//��������
bool CDlgCustomRule::FillControlToData()
{
	TCHAR szBuffer[16] = TEXT("");
	
	//��������
	m_CustomRule.cbBetCount = 0;
	((CEdit*)(GetDlgItem(IDC_EDIT_BET_SCORE_1)))->GetWindowText(szBuffer, sizeof(szBuffer));
	m_CustomRule.lBetScore[m_CustomRule.cbBetCount] = _tstof(szBuffer);
	if(m_CustomRule.lBetScore[m_CustomRule.cbBetCount] > 0)
	{
		m_CustomRule.lBetScore[m_CustomRule.cbBetCount] = GetFloorDouble(m_CustomRule.lBetScore[m_CustomRule.cbBetCount]);
		m_CustomRule.cbBetCount++;
	}

	((CEdit*)(GetDlgItem(IDC_EDIT_BET_SCORE_2)))->GetWindowText(szBuffer, sizeof(szBuffer));
	m_CustomRule.lBetScore[m_CustomRule.cbBetCount] = _tstof(szBuffer);
	if(m_CustomRule.lBetScore[m_CustomRule.cbBetCount] > 0)
	{
		m_CustomRule.lBetScore[m_CustomRule.cbBetCount] = GetFloorDouble(m_CustomRule.lBetScore[m_CustomRule.cbBetCount]);
		m_CustomRule.cbBetCount++;
	}

	((CEdit*)(GetDlgItem(IDC_EDIT_BET_SCORE_3)))->GetWindowText(szBuffer, sizeof(szBuffer));
	m_CustomRule.lBetScore[m_CustomRule.cbBetCount] = _tstof(szBuffer);
	if(m_CustomRule.lBetScore[m_CustomRule.cbBetCount] > 0)
	{
		m_CustomRule.lBetScore[m_CustomRule.cbBetCount] = GetFloorDouble(m_CustomRule.lBetScore[m_CustomRule.cbBetCount]);
		m_CustomRule.cbBetCount++;
	}

	((CEdit*)(GetDlgItem(IDC_EDIT_BET_SCORE_4)))->GetWindowText(szBuffer, sizeof(szBuffer));
	m_CustomRule.lBetScore[m_CustomRule.cbBetCount] = _tstof(szBuffer);
	if(m_CustomRule.lBetScore[m_CustomRule.cbBetCount] > 0)
	{
		m_CustomRule.lBetScore[m_CustomRule.cbBetCount] = GetFloorDouble(m_CustomRule.lBetScore[m_CustomRule.cbBetCount]);
		m_CustomRule.cbBetCount++;
	}

	((CEdit*)(GetDlgItem(IDC_EDIT_BET_SCORE_5)))->GetWindowText(szBuffer, sizeof(szBuffer));
	m_CustomRule.lBetScore[m_CustomRule.cbBetCount] = _tstof(szBuffer);
	if(m_CustomRule.lBetScore[m_CustomRule.cbBetCount] > 0)
	{
		m_CustomRule.lBetScore[m_CustomRule.cbBetCount] = GetFloorDouble(m_CustomRule.lBetScore[m_CustomRule.cbBetCount]);
		m_CustomRule.cbBetCount++;
	}

	((CEdit*)(GetDlgItem(IDC_EDIT_BET_SCORE_6)))->GetWindowText(szBuffer, sizeof(szBuffer));
	m_CustomRule.lBetScore[m_CustomRule.cbBetCount] = _tstof(szBuffer);
	if(m_CustomRule.lBetScore[m_CustomRule.cbBetCount] > 0)
	{
		m_CustomRule.lBetScore[m_CustomRule.cbBetCount] = GetFloorDouble(m_CustomRule.lBetScore[m_CustomRule.cbBetCount]);
		m_CustomRule.cbBetCount++;
	}

	((CEdit*)(GetDlgItem(IDC_EDIT_BET_SCORE_7)))->GetWindowText(szBuffer, sizeof(szBuffer));
	m_CustomRule.lBetScore[m_CustomRule.cbBetCount] = _tstof(szBuffer);
	if(m_CustomRule.lBetScore[m_CustomRule.cbBetCount] > 0)
	{
		m_CustomRule.lBetScore[m_CustomRule.cbBetCount] = GetFloorDouble(m_CustomRule.lBetScore[m_CustomRule.cbBetCount]);
		m_CustomRule.cbBetCount++;
	}

	((CEdit*)(GetDlgItem(IDC_EDIT_BET_SCORE_8)))->GetWindowText(szBuffer, sizeof(szBuffer));
	m_CustomRule.lBetScore[m_CustomRule.cbBetCount] = _tstof(szBuffer);
	if(m_CustomRule.lBetScore[m_CustomRule.cbBetCount] > 0)
	{
		m_CustomRule.lBetScore[m_CustomRule.cbBetCount] = GetFloorDouble(m_CustomRule.lBetScore[m_CustomRule.cbBetCount]);
		m_CustomRule.cbBetCount++;
	}

	((CEdit*)(GetDlgItem(IDC_EDIT_BET_SCORE_9)))->GetWindowText(szBuffer, sizeof(szBuffer));
	m_CustomRule.lBetScore[m_CustomRule.cbBetCount] = _tstof(szBuffer);
	if (m_CustomRule.lBetScore[m_CustomRule.cbBetCount] > 0)
	{
		m_CustomRule.lBetScore[m_CustomRule.cbBetCount] = GetFloorDouble(m_CustomRule.lBetScore[m_CustomRule.cbBetCount]);
		m_CustomRule.cbBetCount++;
	}

	((CEdit*)(GetDlgItem(IDC_EDIT_BET_SCORE_10)))->GetWindowText(szBuffer, sizeof(szBuffer));
	m_CustomRule.lBetScore[m_CustomRule.cbBetCount] = _tstof(szBuffer);
	if (m_CustomRule.lBetScore[m_CustomRule.cbBetCount] > 0)
	{
		m_CustomRule.lBetScore[m_CustomRule.cbBetCount] = GetFloorDouble(m_CustomRule.lBetScore[m_CustomRule.cbBetCount]);
		m_CustomRule.cbBetCount++;
	}

	if (m_CustomRule.cbBetCount == 0)
	{
		AfxMessageBox(TEXT("��עֵ����ȫΪ�գ����������ã�"), MB_ICONSTOP);
		return false;
	}
	else
	{
		for(int i = 0; i < m_CustomRule.cbBetCount - 1; i++)
		{
			for(int j = i + 1; j < m_CustomRule.cbBetCount; j++)
			{
				if(m_CustomRule.lBetScore[i] == m_CustomRule.lBetScore[j] && m_CustomRule.lBetScore[i] != 0)
				{
					AfxMessageBox(TEXT("��עֵ���ظ������������ã�"), MB_ICONSTOP);
					return false;
				}
			}
		}
	}

	//��ע����
	LONGLONG lTempJteeon;
	for(int i = 0; i < m_CustomRule.cbBetCount - 1; i++)
	{
		for(int j = 0; j < m_CustomRule.cbBetCount - 1 - i; j++)
		{
			if(m_CustomRule.lBetScore[j] > m_CustomRule.lBetScore[j + 1])
			{
				lTempJteeon = m_CustomRule.lBetScore[j];
				m_CustomRule.lBetScore[j] = m_CustomRule.lBetScore[j + 1];
				m_CustomRule.lBetScore[j + 1] = lTempJteeon;
			}
		}
	}

	return true;
}

//��ȡ����
bool CDlgCustomRule::GetCustomRule(tagCustomRule & CustomRule)
{
	//��ȡ����
	if (FillControlToData() == true)
	{
		CustomRule = m_CustomRule;
		return true;
	}

	return false;
}

//��������
bool CDlgCustomRule::SetCustomRule(tagCustomRule & CustomRule)
{
	//���ñ���
	m_CustomRule = CustomRule;

	//���²���
	if (m_hWnd != NULL) 
	{
		FillDataToControl();
	}

	return true;
}

//������λС��
SCORE CDlgCustomRule::GetFloorDouble(SCORE dNum)
{
	DOUBLE dNewNum = dNum;

	//ȡС������
	dNewNum = dNewNum - (LONGLONG)dNum;

	dNewNum = dNewNum * 100;

	//ȡ��������
	dNewNum = (LONGLONG)dNewNum;

	DOUBLE lReturnNum = dNewNum / 100;

	lReturnNum += (LONGLONG)dNum;

	return lReturnNum;
}
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661/////////////////////
