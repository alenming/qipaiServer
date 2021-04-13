#include "Stdafx.h"
#include "Resource.h"
#include "DlgCustomRule.h"

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

BEGIN_MESSAGE_MAP(CDlgCustomRule, CDialog)
END_MESSAGE_MAP()

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//构造函数
CDlgCustomRule::CDlgCustomRule() : CDialog(IDD_CUSTOM_RULE)
{
  //设置变量
  ZeroMemory(&m_CustomRule,sizeof(m_CustomRule));

  m_CustomRule.lRobotScoreMin = 100000;
  m_CustomRule.lRobotScoreMax = 1000000;
  m_CustomRule.lRobotBankGet = 1000000;
  m_CustomRule.lRobotBankGetBanker = 10000000;
  m_CustomRule.lRobotBankStoMul = 10;

  return;
}

//析构函数
CDlgCustomRule::~CDlgCustomRule()
{
}

//配置函数
BOOL CDlgCustomRule::OnInitDialog()
{
  __super::OnInitDialog();

  //设置控件
  ((CEdit*)GetDlgItem(IDC_EDIT_ROBOTSCOREMIN))->LimitText(10);
  ((CEdit*)GetDlgItem(IDC_EDIT_ROBOTSCOREMAX))->LimitText(10);
  ((CEdit*)GetDlgItem(IDC_EDIT_ROBOTBANKGET))->LimitText(10);
  ((CEdit*)GetDlgItem(IDC_EDIT_ROBOTBANKGETBANKER))->LimitText(10);
  ((CEdit*)GetDlgItem(IDC_EDIT_ROBOTBANKSTOMUL))->LimitText(2);
  
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
	//机器人存储款
	SetDlgItemInt(IDC_EDIT_ROBOTSCOREMIN, m_CustomRule.lRobotScoreMin);
	SetDlgItemInt(IDC_EDIT_ROBOTSCOREMAX, m_CustomRule.lRobotScoreMax);
	SetDlgItemInt(IDC_EDIT_ROBOTBANKGET, m_CustomRule.lRobotBankGet);
	SetDlgItemInt(IDC_EDIT_ROBOTBANKGETBANKER, m_CustomRule.lRobotBankGetBanker);
	SetDlgItemInt(IDC_EDIT_ROBOTBANKSTOMUL, m_CustomRule.lRobotBankStoMul);

	return true;
}

//更新数据
bool CDlgCustomRule::FillControlToData()
{
	//机器人存储款
	m_CustomRule.lRobotScoreMin = (LONGLONG)GetDlgItemInt(IDC_EDIT_ROBOTSCOREMIN);
	m_CustomRule.lRobotScoreMax = (LONGLONG)GetDlgItemInt(IDC_EDIT_ROBOTSCOREMAX);
	m_CustomRule.lRobotBankGet = (LONGLONG)GetDlgItemInt(IDC_EDIT_ROBOTBANKGET);
	m_CustomRule.lRobotBankGetBanker = (LONGLONG)GetDlgItemInt(IDC_EDIT_ROBOTBANKGETBANKER);
	m_CustomRule.lRobotBankStoMul = (LONGLONG)GetDlgItemInt(IDC_EDIT_ROBOTBANKSTOMUL);

	if (!(m_CustomRule.lRobotScoreMax > m_CustomRule.lRobotScoreMin))
	{
		AfxMessageBox(TEXT("存款最大值应大于取款最小值"), MB_ICONSTOP);
		return false;
	}

	if (!(m_CustomRule.lRobotBankGetBanker > m_CustomRule.lRobotBankGet))
	{
		AfxMessageBox(TEXT("取款最大数额应大于取款最小数额"), MB_ICONSTOP);
		return false;
	}
	return true;
}

//读取配置
bool CDlgCustomRule::GetCustomRule(tagCustomRule & CustomRule)
{
  //读取参数
  if(FillControlToData() == true)
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
  if(m_hWnd != NULL)
  {
    FillDataToControl();
  }

  return true;
}

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
