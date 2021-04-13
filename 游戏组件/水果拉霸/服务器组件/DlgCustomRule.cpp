// DlgCustomRule.cpp : 实现文件
//

#include "stdafx.h"
#include "Resource.h"
#include "DlgCustomRule.h"
#include <math.h>


// CDlgCustomRule 对话框

BEGIN_MESSAGE_MAP(CDlgCustomRule, CDialog)
END_MESSAGE_MAP()

CDlgCustomRule::CDlgCustomRule() : CDialog(IDD_CUSTOM_RULE)
{
	//设置变量
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

//配置函数
BOOL CDlgCustomRule::OnInitDialog()
{
	__super::OnInitDialog();

	//设置控件范围
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

	//更新参数
	FillDataToControl();

	return FALSE;
}

//确定函数
VOID CDlgCustomRule::OnOK() 
{ 
	//投递消息
	GetParent()->PostMessage(WM_COMMAND, MAKELONG(IDOK, 0), 0);

	return;
}

//取消消息
VOID CDlgCustomRule::OnCancel() 
{ 
	//投递消息
	GetParent()->PostMessage(WM_COMMAND, MAKELONG(IDCANCEL, 0), 0);

	return;
}

//更新控件
bool CDlgCustomRule::FillDataToControl()
{
	//设置数据
	TCHAR szBuffer[16] = TEXT("");
	for (int i = 0; i < 10; i++)
	{
		_sntprintf(szBuffer, sizeof(szBuffer), TEXT("%g"), m_CustomRule.lBetScore[i]);

		((CEdit*)(GetDlgItem(IDC_EDIT_BET_SCORE_1 + i)))->SetWindowText(szBuffer);
	}

	return true;
}

//更新数据
bool CDlgCustomRule::FillControlToData()
{
	TCHAR szBuffer[16] = TEXT("");
	
	//设置数据
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
		AfxMessageBox(TEXT("下注值不能全为空，请重新设置！"), MB_ICONSTOP);
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
					AfxMessageBox(TEXT("下注值有重复，请重新设置！"), MB_ICONSTOP);
					return false;
				}
			}
		}
	}

	//下注排序
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

//读取配置
bool CDlgCustomRule::GetCustomRule(tagCustomRule & CustomRule)
{
	//读取参数
	if (FillControlToData() == true)
	{
		CustomRule = m_CustomRule;
		return true;
	}

	return false;
}

//设置配置
bool CDlgCustomRule::SetCustomRule(tagCustomRule & CustomRule)
{
	//设置变量
	m_CustomRule = CustomRule;

	//更新参数
	if (m_hWnd != NULL) 
	{
		FillDataToControl();
	}

	return true;
}

//保留俩位小数
SCORE CDlgCustomRule::GetFloorDouble(SCORE dNum)
{
	DOUBLE dNewNum = dNum;

	//取小数部分
	dNewNum = dNewNum - (LONGLONG)dNum;

	dNewNum = dNewNum * 100;

	//取整数部分
	dNewNum = (LONGLONG)dNewNum;

	DOUBLE lReturnNum = dNewNum / 100;

	lReturnNum += (LONGLONG)dNum;

	return lReturnNum;
}
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661/////////////////////
