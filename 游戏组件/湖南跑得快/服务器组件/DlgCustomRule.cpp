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
}

//��������
CDlgCustomRule::~CDlgCustomRule()
{
}

//�ؼ���
VOID CDlgCustomRule::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
}

//��ʼ������
BOOL CDlgCustomRule::OnInitDialog()
{
	__super::OnInitDialog();

	return FALSE;
}

//ȷ������
VOID CDlgCustomRule::OnOK() 
{ 
	//Ͷ����Ϣ
	GetParent()->PostMessage(WM_COMMAND,MAKELONG(IDOK,0),0);

	return;
}

//ȡ����Ϣ
VOID CDlgCustomRule::OnCancel() 
{ 
	//Ͷ����Ϣ
	GetParent()->PostMessage(WM_COMMAND,MAKELONG(IDCANCEL,0),0);

	return;
}

//��������
bool CDlgCustomRule::SetCustomRule(LPBYTE pcbCustomRule, WORD wCustomSize)
{
	//���ñ���
	m_wCustomSize=wCustomSize;
	m_pcbCustomRule=pcbCustomRule;

	return true;
}

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
