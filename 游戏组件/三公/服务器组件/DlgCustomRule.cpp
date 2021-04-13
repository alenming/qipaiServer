#include "Stdafx.h"
#include "Resource.h"
#include "DlgCustomRule.h"

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

BEGIN_MESSAGE_MAP(CDlgCustomRule, CDialog)
END_MESSAGE_MAP()

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//���캯��
CDlgCustomRule::CDlgCustomRule() : CDialog(IDD_CUSTOM_RULE)
{
  //���ñ���
  ZeroMemory(&m_CustomRule,sizeof(m_CustomRule));

  m_CustomRule.lRobotScoreMin = 100000;
  m_CustomRule.lRobotScoreMax = 1000000;
  m_CustomRule.lRobotBankGet = 1000000;
  m_CustomRule.lRobotBankGetBanker = 10000000;
  m_CustomRule.lRobotBankStoMul = 10;

  return;
}

//��������
CDlgCustomRule::~CDlgCustomRule()
{
}

//���ú���
BOOL CDlgCustomRule::OnInitDialog()
{
  __super::OnInitDialog();

  //���ÿؼ�
  ((CEdit*)GetDlgItem(IDC_EDIT_ROBOTSCOREMIN))->LimitText(10);
  ((CEdit*)GetDlgItem(IDC_EDIT_ROBOTSCOREMAX))->LimitText(10);
  ((CEdit*)GetDlgItem(IDC_EDIT_ROBOTBANKGET))->LimitText(10);
  ((CEdit*)GetDlgItem(IDC_EDIT_ROBOTBANKGETBANKER))->LimitText(10);
  ((CEdit*)GetDlgItem(IDC_EDIT_ROBOTBANKSTOMUL))->LimitText(2);
  
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
	//�����˴洢��
	SetDlgItemInt(IDC_EDIT_ROBOTSCOREMIN, m_CustomRule.lRobotScoreMin);
	SetDlgItemInt(IDC_EDIT_ROBOTSCOREMAX, m_CustomRule.lRobotScoreMax);
	SetDlgItemInt(IDC_EDIT_ROBOTBANKGET, m_CustomRule.lRobotBankGet);
	SetDlgItemInt(IDC_EDIT_ROBOTBANKGETBANKER, m_CustomRule.lRobotBankGetBanker);
	SetDlgItemInt(IDC_EDIT_ROBOTBANKSTOMUL, m_CustomRule.lRobotBankStoMul);

	return true;
}

//��������
bool CDlgCustomRule::FillControlToData()
{
	//�����˴洢��
	m_CustomRule.lRobotScoreMin = (LONGLONG)GetDlgItemInt(IDC_EDIT_ROBOTSCOREMIN);
	m_CustomRule.lRobotScoreMax = (LONGLONG)GetDlgItemInt(IDC_EDIT_ROBOTSCOREMAX);
	m_CustomRule.lRobotBankGet = (LONGLONG)GetDlgItemInt(IDC_EDIT_ROBOTBANKGET);
	m_CustomRule.lRobotBankGetBanker = (LONGLONG)GetDlgItemInt(IDC_EDIT_ROBOTBANKGETBANKER);
	m_CustomRule.lRobotBankStoMul = (LONGLONG)GetDlgItemInt(IDC_EDIT_ROBOTBANKSTOMUL);

	if (!(m_CustomRule.lRobotScoreMax > m_CustomRule.lRobotScoreMin))
	{
		AfxMessageBox(TEXT("������ֵӦ����ȡ����Сֵ"), MB_ICONSTOP);
		return false;
	}

	if (!(m_CustomRule.lRobotBankGetBanker > m_CustomRule.lRobotBankGet))
	{
		AfxMessageBox(TEXT("ȡ���������Ӧ����ȡ����С����"), MB_ICONSTOP);
		return false;
	}
	return true;
}

//��ȡ����
bool CDlgCustomRule::GetCustomRule(tagCustomRule & CustomRule)
{
  //��ȡ����
  if(FillControlToData() == true)
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
  if(m_hWnd != NULL)
  {
    FillDataToControl();
  }

  return true;
}

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
