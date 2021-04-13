#include "Stdafx.h"
#include "Resource.h"
#include "DlgCustomRule.h"

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//���캯��
CDlgCustomRule::CDlgCustomRule() : CDialog(IDD_CUSTOM_RULE)
{
	//���ñ���
	ZeroMemory(&m_CustomRule,sizeof(m_CustomRule));

	m_CustomRule.lRoomStorageStart = 100000;
	m_CustomRule.lRoomStorageDeduct = 0;
	m_CustomRule.lRoomStorageMax1 = 1000000;
	m_CustomRule.lRoomStorageMul1 = 50;
	m_CustomRule.lRoomStorageMax2 = 5000000;
	m_CustomRule.lRoomStorageMul2 = 80;

	//�����˴��ȡ��
	m_CustomRule.lRobotScoreMin = 100000;
	m_CustomRule.lRobotScoreMax = 1000000;
	m_CustomRule.lRobotBankGet = 1000000;
	m_CustomRule.lRobotBankGetBanker = 10000000;
	m_CustomRule.lRobotBankStoMul = 10;


	m_CustomRule.lFreeConfig[0] = 5;
	m_CustomRule.lFreeConfig[1] = 10;
	m_CustomRule.lFreeConfig[2] = 15;
	m_CustomRule.lFreeConfig[3] = 20;

    
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
	((CEdit *)GetDlgItem(IDC_EDIT_ROOMSTORAGE_START))->LimitText(9);
	((CEdit *)GetDlgItem(IDC_EDIT_ROOMSTORAGE_DEDUCT))->LimitText(3);
	((CEdit *)GetDlgItem(IDC_EDIT_ROOMSTORAGE_MAX1))->LimitText(9);
	((CEdit *)GetDlgItem(IDC_EDIT_ROOMSTORAGE_MUL1))->LimitText(2);
	((CEdit *)GetDlgItem(IDC_EDIT_ROOMSTORAGE_MAX2))->LimitText(9);
	((CEdit *)GetDlgItem(IDC_EDIT_ROOMSTORAGE_MUL2))->LimitText(2);

	((CEdit *)GetDlgItem(IDC_ROBOT_SCOREMIN))->LimitText(9);
	((CEdit *)GetDlgItem(IDC_ROBOT_BANKERGETBANKER))->LimitText(9);
	((CEdit *)GetDlgItem(IDC_ROBOT_SCOREMAX))->LimitText(9);
	((CEdit *)GetDlgItem(IDC_ROBOT_BANKGET))->LimitText(9);
	((CEdit *)GetDlgItem(IDC_ROBOT_STOMUL))->LimitText(2);
	
	for (WORD i=0; i<MAX_CONFIG; i++)
	{
		((CEdit *)GetDlgItem(IDC_EDIT_BT_FREE_0 + i))->LimitText(9);
	}

	//���²���
	FillDataToControl();

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

//���¿ؼ�
bool CDlgCustomRule::FillDataToControl()
{
	//��������
	SetDlgItemInt(IDC_EDIT_ROOMSTORAGE_START, m_CustomRule.lRoomStorageStart);
	SetDlgItemInt(IDC_EDIT_ROOMSTORAGE_DEDUCT, m_CustomRule.lRoomStorageDeduct);
	SetDlgItemInt(IDC_EDIT_ROOMSTORAGE_MAX1, m_CustomRule.lRoomStorageMax1);
	SetDlgItemInt(IDC_EDIT_ROOMSTORAGE_MUL1, m_CustomRule.lRoomStorageMul1);
	SetDlgItemInt(IDC_EDIT_ROOMSTORAGE_MAX2, m_CustomRule.lRoomStorageMax2);
	SetDlgItemInt(IDC_EDIT_ROOMSTORAGE_MUL2, m_CustomRule.lRoomStorageMul2);

	//�����˿���
	SetDlgItemInt(IDC_ROBOT_SCOREMIN, m_CustomRule.lRobotScoreMin);
	SetDlgItemInt(IDC_ROBOT_SCOREMAX, m_CustomRule.lRobotScoreMax);
	SetDlgItemInt(IDC_ROBOT_BANKERGETBANKER, m_CustomRule.lRobotBankGetBanker);
	SetDlgItemInt(IDC_ROBOT_BANKGET, m_CustomRule.lRobotBankGet);
	SetDlgItemInt(IDC_ROBOT_STOMUL, m_CustomRule.lRobotBankStoMul);
		
	for (WORD i=0; i<MAX_CONFIG; i++)
	{
		SetDlgItemInt(IDC_EDIT_BT_FREE_0 + i, m_CustomRule.lFreeConfig[i]);
		((CEdit *)GetDlgItem(IDC_EDIT_BT_FREE_0 + i))->EnableWindow(TRUE);
	}


	return true;
}

//��������
bool CDlgCustomRule::FillControlToData()
{
	//��������
	m_CustomRule.lRoomStorageStart = (SCORE)GetDlgItemInt(IDC_EDIT_ROOMSTORAGE_START);
	m_CustomRule.lRoomStorageDeduct = (SCORE)GetDlgItemInt(IDC_EDIT_ROOMSTORAGE_DEDUCT);
	m_CustomRule.lRoomStorageMax1 = (SCORE)GetDlgItemInt(IDC_EDIT_ROOMSTORAGE_MAX1);
	m_CustomRule.lRoomStorageMul1 = (SCORE)GetDlgItemInt(IDC_EDIT_ROOMSTORAGE_MUL1);
	m_CustomRule.lRoomStorageMax2 = (SCORE)GetDlgItemInt(IDC_EDIT_ROOMSTORAGE_MAX2);
	m_CustomRule.lRoomStorageMul2 = (SCORE)GetDlgItemInt(IDC_EDIT_ROOMSTORAGE_MUL2);

	//�����˿���
	m_CustomRule.lRobotScoreMin = (SCORE)GetDlgItemInt(IDC_ROBOT_SCOREMIN);
	m_CustomRule.lRobotScoreMax = (SCORE)GetDlgItemInt(IDC_ROBOT_SCOREMAX);
	m_CustomRule.lRobotBankGetBanker = (SCORE)GetDlgItemInt(IDC_ROBOT_BANKERGETBANKER);
	m_CustomRule.lRobotBankGet = (SCORE)GetDlgItemInt(IDC_ROBOT_BANKGET);
	m_CustomRule.lRobotBankStoMul = (SCORE)GetDlgItemInt(IDC_ROBOT_STOMUL);
	
	//��ע����
	for (WORD i=0; i<MAX_CONFIG; i++)
	{
		m_CustomRule.lFreeConfig[i] = (LONG)GetDlgItemInt(IDC_EDIT_BT_FREE_0 + i);
	}
		
	//У��ǰ����
	if (m_CustomRule.lFreeConfig[0] == 0 || m_CustomRule.lFreeConfig[1] == 0)
	{
		AfxMessageBox(TEXT("�������ö��ǰ������ӦΪ0�����������ã�"), MB_ICONSTOP);
		return false;
	}
		
	//У���Ƿ�������ظ�
	for (WORD i=0; i<MAX_CONFIG-1; i++)
	{
		if (m_CustomRule.lFreeConfig[i + 1] <=   m_CustomRule.lFreeConfig[i])
		{
			//�Ƿ���涼��0
			bool bContinueZero = true;

			for (WORD j = i + 1; j<MAX_CONFIG; j++)
			{
				if (m_CustomRule.lFreeConfig[j] != 0)
				{
					bContinueZero = false;
					break;
				}
			}

			if (!bContinueZero)
			{
				AfxMessageBox(TEXT("�������ö��Ӧ�õ�����Ψһ�����������ã�"), MB_ICONSTOP);
				return false;
			}

		}
	}
	

	//����У��
	if ((m_CustomRule.lRoomStorageMax1 > m_CustomRule.lRoomStorageMax2))
	{
		AfxMessageBox(TEXT("���ⶥֵ1ӦС�ڿ��ⶥֵ2�����������ã�"),MB_ICONSTOP);
		return false;
	}

	if ((m_CustomRule.lRoomStorageMul1 > m_CustomRule.lRoomStorageMul2))
	{
		AfxMessageBox(TEXT("Ӯ�ָ���1ӦС��Ӯ�ָ���2�����������ã�"),MB_ICONSTOP);
		return false;
	}

	if ((m_CustomRule.lRobotScoreMin > m_CustomRule.lRobotScoreMax))
	{
		AfxMessageBox(TEXT("�����˷�����СֵӦС�����ֵ�����������ã�"),MB_ICONSTOP);
		return false;
	}

	return true;
}

//��ȡ����
bool CDlgCustomRule::GetCustomRule(tagCustomRule & CustomRule)
{
	//��ȡ����
	if (FillControlToData()==true)
	{
		CustomRule=m_CustomRule;
		return true;
	}

	return false;
}

//��������
bool CDlgCustomRule::SetCustomRule(tagCustomRule & CustomRule)
{
	//���ñ���
	m_CustomRule=CustomRule;

	//���²���
	if (m_hWnd!=NULL) FillDataToControl();

	return true;
}


/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
