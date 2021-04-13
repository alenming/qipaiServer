#include "Stdafx.h"
#include "Resource.h"
#include "DlgCustomRule.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgCustomRule, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CUSTOM, OnTcnSelchangeTab)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

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
	
	((CTabCtrl *)GetDlgItem(IDC_TAB_CUSTOM))->InsertItem(0,TEXT("ͨ�ù���"));
	((CTabCtrl *)GetDlgItem(IDC_TAB_CUSTOM))->InsertItem(1,TEXT("������"));
	
	m_DlgCustomGeneral.Create(IDD_CUSTOM_GENERAL,GetDlgItem(IDC_TAB_CUSTOM)); 
	m_DlgCustomAndroid.Create(IDD_CUSTOM_ANDROID,GetDlgItem(IDC_TAB_CUSTOM)); 
	
	CRect rcTabClient;
	GetDlgItem(IDC_TAB_CUSTOM)->GetClientRect(rcTabClient);
	rcTabClient.top+=20;
	rcTabClient.bottom-=4; 
	rcTabClient.left+=4; 
	rcTabClient.right-=4; 
	m_DlgCustomGeneral.MoveWindow(rcTabClient);
	m_DlgCustomAndroid.MoveWindow(rcTabClient);
	
	m_DlgCustomGeneral.ShowWindow(TRUE);
	m_DlgCustomAndroid.ShowWindow(FALSE);
	((CTabCtrl *)GetDlgItem(IDC_TAB_CUSTOM))->SetCurSel(0);
	
	return TRUE;
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
BOOL CDlgCustomRule::UpdateConfigData(BOOL bSaveAndValidate)
{
	if(bSaveAndValidate)	//�ӿؼ���ȡ���ݵ�����
	{
		m_DlgCustomGeneral.UpdateData(TRUE);
		m_DlgCustomAndroid.UpdateData(TRUE);
		
		CopyMemory(&m_CustomConfig.CustomGeneral, &m_DlgCustomGeneral.m_CustomGeneral, sizeof(tagCustomGeneral));
		CopyMemory(&m_CustomConfig.CustomAndroid, &m_DlgCustomAndroid.m_CustomAndroid, sizeof(tagCustomAndroid));

	}
	else					//��������ֵ���ؼ���ʾ
	{
		CopyMemory(&m_DlgCustomGeneral.m_CustomGeneral, &m_CustomConfig.CustomGeneral, sizeof(tagCustomGeneral));
		CopyMemory(&m_DlgCustomAndroid.m_CustomAndroid, &m_CustomConfig.CustomAndroid, sizeof(tagCustomAndroid));

		m_DlgCustomGeneral.UpdateData(FALSE);
		m_DlgCustomAndroid.UpdateData(FALSE);
	}

	return TRUE;
}

//��������
bool CDlgCustomRule::SetCustomRule(LPBYTE pcbCustomRule, WORD wCustomSize)
{
	//���ñ���
	m_wCustomSize=wCustomSize;
	m_pcbCustomRule=pcbCustomRule;

	//���ñ���
	ASSERT(m_pcbCustomRule);
	if( !m_pcbCustomRule ) return false;
	tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pcbCustomRule;
	//if( pCustomConfig->lApplyBankerCondition != 0 )
		memcpy(&m_CustomConfig, pCustomConfig, sizeof(tagCustomConfig));

	//���½���
	if( m_hWnd )
		UpdateConfigData(FALSE);  //��������ֵ���ؼ���ʾ

	if (m_CustomConfig.CustomAndroid.nEnableRobotBanker == true)
	{
		((CButton*)(m_DlgCustomAndroid.GetDlgItem(IDC_CHECK_ANDROID_1)))->SetCheck(1);
	}
	else if (m_CustomConfig.CustomAndroid.nEnableRobotBanker == false)
	{
		((CButton*)(m_DlgCustomAndroid.GetDlgItem(IDC_CHECK_ANDROID_1)))->SetCheck(0);
	}

	return true;
}

//��������
bool CDlgCustomRule::SaveCustomRule(LPBYTE pcbCustomRule, WORD wCustomSize)
{
	//���½���
	if( m_hWnd )
	{
		//�������ֵ�ǿ�
		bool bDataOK = true;
		INT nID = IDC_EDIT_GENERAL_1;
		while(nID <= IDC_EDIT_GENERAL_20)
		{
			TCHAR szData[256] = {0};
			m_DlgCustomGeneral.GetDlgItemText(nID, szData, sizeof(szData));

			if(0 == szData[0])
			{
				if( IDOK  == AfxMessageBox(TEXT("������һ���Ϸ����ݣ�")))
				{
					return false;
				}
			}

			nID ++;
		}
		nID = IDC_EDIT_ANDROID_1;
		while(nID <= IDC_EDIT_ANDROID_20)
		{
			TCHAR szData[256] = {0};
			m_DlgCustomAndroid.GetDlgItemText(nID, szData, sizeof(szData));

			if(0 == szData[0])
			{
				if( IDOK  == AfxMessageBox(TEXT("������һ���Ϸ����ݣ�")))
				{
					return false;
				}
			}

			nID ++;
		}	

		UpdateConfigData(TRUE); //�ӿؼ���ȡ���ݵ�����
	}
	
	if (m_DlgCustomGeneral.m_CustomGeneral.lFreeTime<1||m_DlgCustomGeneral.m_CustomGeneral.lFreeTime>99)
	{
           AfxMessageBox(L"[����ʱ��]���뷶ΧΪ1-99");
			    return false;
	}
	if (m_DlgCustomGeneral.m_CustomGeneral.lBetTime<5||
		m_DlgCustomGeneral.m_CustomGeneral.lBetTime>99)
	{
       AfxMessageBox(L"[��עʱ��]���뷶ΧΪ5-99");
		    return false;
	}
	if (m_DlgCustomGeneral.m_CustomGeneral.lEndTime<8||m_DlgCustomGeneral.m_CustomGeneral.lEndTime>99)
	{
		AfxMessageBox(L"[����ʱ��]���뷶ΧΪ8-99");
	 return false;
	}
	if (m_DlgCustomGeneral.m_CustomGeneral.StorageDeduct<0||m_DlgCustomGeneral.m_CustomGeneral.StorageDeduct>1000)
	{
		AfxMessageBox(L"[���˥��ֵ]���뷶ΧΪ0-1000");
		return false;
	}
	if (m_DlgCustomGeneral.m_CustomGeneral.StorageMul1<0||m_DlgCustomGeneral.m_CustomGeneral.StorageMul1>100)
	{
		AfxMessageBox(L"[���Ӯ�ָ���1]���뷶ΧΪ0-100");
		return false;
	}
	if (m_DlgCustomGeneral.m_CustomGeneral.StorageMul2<0||m_DlgCustomGeneral.m_CustomGeneral.StorageMul2>100)
	{
		AfxMessageBox(L"[���Ӯ�ָ���2]���뷶ΧΪ0-100");
		return false;
	}
	if (m_DlgCustomGeneral.m_CustomGeneral.lEndGameMul<1||m_DlgCustomGeneral.m_CustomGeneral.lEndGameMul>100)
	{
		AfxMessageBox(L"[��ǰ���ưٷֱ�]���뷶ΧΪ1-100");
		return false;
	}

	if (m_DlgCustomAndroid.m_CustomAndroid.lRobotBankStoMul<0||m_DlgCustomAndroid.m_CustomAndroid.lRobotBankStoMul>100)
	{
		AfxMessageBox(L"[���ٷֱ�]���뷶ΧΪ0-100");
		return false;
	}
	
	if(!CheckDataMinMax(m_DlgCustomGeneral.m_CustomGeneral.lApplyBankerCondition)) return false;
	if(!CheckDataMinMax(m_DlgCustomGeneral.m_CustomGeneral.lBankerTime)) return false;
	if(!CheckDataMinMax(m_DlgCustomGeneral.m_CustomGeneral.lBankerTimeAdd)) return false;
	if(!CheckDataMinMax(m_DlgCustomGeneral.m_CustomGeneral.lBankerScoreMAX)) return false;
	if(!CheckDataMinMax(m_DlgCustomGeneral.m_CustomGeneral.lBankerTimeExtra)) return false;
	if(!CheckDataMinMax(m_DlgCustomGeneral.m_CustomGeneral.StorageStart)) return false;
	if(!CheckDataMinMax(m_DlgCustomGeneral.m_CustomGeneral.StorageMax1)) return false;
	if(!CheckDataMinMax(m_DlgCustomGeneral.m_CustomGeneral.StorageMax2)) return false;
	if(!CheckDataMinMax(m_DlgCustomGeneral.m_CustomGeneral.lAreaLimitScore)) return false;
	if(!CheckDataMinMax(m_DlgCustomGeneral.m_CustomGeneral.lUserLimitScore)) return false;
	if(!CheckDataMinMax(m_DlgCustomAndroid.m_CustomAndroid.lRobotBankerCountMin)) return false;
	if(!CheckDataMinMax(m_DlgCustomAndroid.m_CustomAndroid.lRobotBankerCountMax)) return false;
	if(!CheckDataMinMax(m_DlgCustomAndroid.m_CustomAndroid.lRobotListMinCount)) return false;
	if(!CheckDataMinMax(m_DlgCustomAndroid.m_CustomAndroid.lRobotListMaxCount)) return false;
	if(!CheckDataMinMax(m_DlgCustomAndroid.m_CustomAndroid.lRobotApplyBanker)) return false;
	if(!CheckDataMinMax(m_DlgCustomAndroid.m_CustomAndroid.lRobotWaitBanker)) return false;
	if(!CheckDataMinMax(m_DlgCustomAndroid.m_CustomAndroid.lRobotMinBetTime)) return false;
	if(!CheckDataMinMax(m_DlgCustomAndroid.m_CustomAndroid.lRobotMaxBetTime)) return false;
	if(!CheckDataMinMax(m_DlgCustomAndroid.m_CustomAndroid.lRobotMinJetton)) return false;
	if(!CheckDataMinMax(m_DlgCustomAndroid.m_CustomAndroid.lRobotMaxJetton)) return false;
	if(!CheckDataMinMax(m_DlgCustomAndroid.m_CustomAndroid.lRobotBetMinCount)) return false;
	if(!CheckDataMinMax(m_DlgCustomAndroid.m_CustomAndroid.lRobotBetMaxCount)) return false;
	if(!CheckDataMinMax(m_DlgCustomAndroid.m_CustomAndroid.lRobotAreaLimit)) return false;
	if(!CheckDataMinMax(m_DlgCustomAndroid.m_CustomAndroid.lRobotScoreMin)) return false;
	if(!CheckDataMinMax(m_DlgCustomAndroid.m_CustomAndroid.lRobotScoreMax)) return false;
	if(!CheckDataMinMax(m_DlgCustomAndroid.m_CustomAndroid.lRobotBankGetMin)) return false;
	if(!CheckDataMinMax(m_DlgCustomAndroid.m_CustomAndroid.lRobotBankGetMax)) return false;
	if(!CheckDataMinMax(m_DlgCustomAndroid.m_CustomAndroid.lRobotBankGetBankerMin)) return false;
	if(!CheckDataMinMax(m_DlgCustomAndroid.m_CustomAndroid.lRobotBankGetBankerMax)) return false;

	if (((CButton*)(m_DlgCustomAndroid.GetDlgItem(IDC_CHECK_ANDROID_1)))->GetCheck() == 1)
	{
		m_CustomConfig.CustomAndroid.nEnableRobotBanker = true;
	}
	else if (((CButton*)(m_DlgCustomAndroid.GetDlgItem(IDC_CHECK_ANDROID_1)))->GetCheck() == 0)
	{
		m_CustomConfig.CustomAndroid.nEnableRobotBanker = false;
	}

	//���ñ���
	m_wCustomSize=wCustomSize;
	m_pcbCustomRule=pcbCustomRule;

	//���ñ���
	ASSERT(m_pcbCustomRule);
	if( !m_pcbCustomRule ) return true;
	tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pcbCustomRule;
	memcpy(pCustomConfig, &m_CustomConfig, sizeof(tagCustomConfig));

	return true;
}

//��������
bool CDlgCustomRule::DefaultCustomRule(LPBYTE pcbCustomRule, WORD wCustomSize)
{
	//���ñ���
	m_wCustomSize=wCustomSize;
	m_pcbCustomRule=pcbCustomRule;

	//���ñ���
	ASSERT(m_pcbCustomRule);
	if( !m_pcbCustomRule ) return true;
	tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pcbCustomRule;

	m_CustomConfig.DefaultCustomRule();
	memcpy(pCustomConfig, &m_CustomConfig, sizeof(tagCustomConfig));

	//���½���
	if( m_hWnd )
		UpdateConfigData(FALSE); // ��������ֵ���ؼ���ʾ

	return true;
}

//���ػ���������
bool CDlgCustomRule::HideAndroidModule(bool bHide)
{
	if(bHide)
	{
		((CTabCtrl *)GetDlgItem(IDC_TAB_CUSTOM))->DeleteItem(1);
		m_DlgCustomAndroid.ShowWindow(false);
		((CTabCtrl *)GetDlgItem(IDC_TAB_CUSTOM))->SetCurSel(0);
	}

	return true;
}

void CDlgCustomRule::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{   
	INT CurSel =((CTabCtrl *)GetDlgItem(IDC_TAB_CUSTOM))->GetCurSel();

    switch(CurSel)
    {
    case 0:
        m_DlgCustomGeneral.ShowWindow(true);
        m_DlgCustomAndroid.ShowWindow(false);
        break;
    case 1:
        m_DlgCustomGeneral.ShowWindow(false);
        m_DlgCustomAndroid.ShowWindow(true);
        break;
    }
    *pResult = 0;

	return;
}

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

BEGIN_MESSAGE_MAP(CDlgCustomGeneral, CDialog)
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RADIO_SUPERBANKER_VIP, OnClickSuperBankerVIPConfig)
	ON_BN_CLICKED(IDC_RADIO_SUPERBANKER_CONSUME, OnClickSuperBankerConsumeConfig)

	ON_BN_CLICKED(IDC_RADIO_OCCUPYSEAT_VIP, OnClickOccupySeatVIPConfig)
	ON_BN_CLICKED(IDC_RADIO_OCCUPYSEAT_CONSUME, OnClickOccupySeatConsumeConfig)
	ON_BN_CLICKED(IDC_RADIO_OCCUPYSEAT_FREE, OnClickOccupySeatFreeConfig)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CDlgCustomGeneral::CDlgCustomGeneral() : CDialog(IDD_CUSTOM_GENERAL)
{
}

//��������
CDlgCustomGeneral::~CDlgCustomGeneral()
{
}

//�ؼ���
VOID CDlgCustomGeneral::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
		
	DDX_Text(pDX,IDC_EDIT_GENERAL_1,m_CustomGeneral.lApplyBankerCondition);
	DDX_Text(pDX,IDC_EDIT_GENERAL_2,m_CustomGeneral.lBankerTime);
	DDX_Text(pDX,IDC_EDIT_GENERAL_3,m_CustomGeneral.lBankerTimeAdd);
	DDX_Text(pDX,IDC_EDIT_GENERAL_4,m_CustomGeneral.lBankerScoreMAX);
	DDX_Text(pDX,IDC_EDIT_GENERAL_5,m_CustomGeneral.lBankerTimeExtra);
	DDX_Text(pDX,IDC_EDIT_GENERAL_6,m_CustomGeneral.lFreeTime);
	DDX_Text(pDX,IDC_EDIT_GENERAL_7,m_CustomGeneral.lBetTime);
	DDX_Text(pDX,IDC_EDIT_GENERAL_8,m_CustomGeneral.lEndTime);
	DDX_Text(pDX,IDC_EDIT_GENERAL_9,m_CustomGeneral.lAreaLimitScore);
	DDX_Text(pDX,IDC_EDIT_GENERAL_10,m_CustomGeneral.lUserLimitScore);

	DDX_Text(pDX,IDC_EDIT_GENERAL_11,m_CustomGeneral.szMessageItem1,64);
	DDX_Text(pDX,IDC_EDIT_GENERAL_12,m_CustomGeneral.szMessageItem2,64);
	DDX_Text(pDX,IDC_EDIT_GENERAL_13,m_CustomGeneral.szMessageItem3,64);

	DDX_Text(pDX,IDC_EDIT_GENERAL_14,m_CustomGeneral.StorageStart);
	DDX_Text(pDX,IDC_EDIT_GENERAL_15,m_CustomGeneral.StorageDeduct);
	DDX_Text(pDX,IDC_EDIT_GENERAL_16,m_CustomGeneral.StorageMax1);
	DDX_Text(pDX,IDC_EDIT_GENERAL_17,m_CustomGeneral.StorageMul1);
	DDX_Text(pDX,IDC_EDIT_GENERAL_18,m_CustomGeneral.StorageMax2);
	DDX_Text(pDX,IDC_EDIT_GENERAL_19,m_CustomGeneral.StorageMul2);
	
	DDX_Text(pDX,IDC_EDIT_GENERAL_20,m_CustomGeneral.lEndGameMul);

	DDX_Check(pDX,IDC_CHECK_GENERAL_1,m_CustomGeneral.nEnableSysBanker);
}

//��ʼ������
BOOL CDlgCustomGeneral::OnInitDialog()
{
	__super::OnInitDialog();

	((CButton*)(GetDlgItem(IDC_RADIO_SUPERBANKER_VIP)))->SetCheck(1);
	((CEdit*)(GetDlgItem(IDC_EDIT_SUPERBANKERCONSUME)))->LimitText(10);
	((CEdit*)(GetDlgItem(IDC_EDIT_SUPERBANKERCONSUME)))->EnableWindow(false);
	((CComboBox*)GetDlgItem(IDC_COMBO_SUPERBANKER_VIP))->AddString(TEXT("VIP1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_SUPERBANKER_VIP))->AddString(TEXT("VIP2"));
	((CComboBox*)GetDlgItem(IDC_COMBO_SUPERBANKER_VIP))->AddString(TEXT("VIP3"));
	((CComboBox*)GetDlgItem(IDC_COMBO_SUPERBANKER_VIP))->AddString(TEXT("VIP4"));
	((CComboBox*)GetDlgItem(IDC_COMBO_SUPERBANKER_VIP))->AddString(TEXT("VIP5"));
	((CComboBox*)GetDlgItem(IDC_COMBO_SUPERBANKER_VIP))->SetCurSel(0);

	((CButton*)(GetDlgItem(IDC_RADIO_OCCUPYSEAT_VIP)))->SetCheck(1);
	((CEdit*)(GetDlgItem(IDC_EDIT_OCCUPYSEATCONSUME)))->LimitText(10);
	((CEdit*)(GetDlgItem(IDC_EDIT_OCCUPYSEATCONSUME)))->EnableWindow(false);
	((CComboBox*)GetDlgItem(IDC_COMBO_OCCUPYSEAT_VIP))->AddString(TEXT("VIP1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_OCCUPYSEAT_VIP))->AddString(TEXT("VIP2"));
	((CComboBox*)GetDlgItem(IDC_COMBO_OCCUPYSEAT_VIP))->AddString(TEXT("VIP3"));
	((CComboBox*)GetDlgItem(IDC_COMBO_OCCUPYSEAT_VIP))->AddString(TEXT("VIP4"));
	((CComboBox*)GetDlgItem(IDC_COMBO_OCCUPYSEAT_VIP))->AddString(TEXT("VIP5"));
	((CComboBox*)GetDlgItem(IDC_COMBO_OCCUPYSEAT_VIP))->SetCurSel(0);

	((CEdit*)(GetDlgItem(IDC_EDIT_OCCUPYSEATFREE)))->LimitText(10);
	((CEdit*)(GetDlgItem(IDC_EDIT_OCCUPYSEATFREE)))->EnableWindow(false);

	((CEdit*)(GetDlgItem(IDC_EDIT_FORCESTANDUP)))->LimitText(10);
	
	return TRUE;
}

void CDlgCustomGeneral::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	static int oldpos = 0;  
	int minpos = 0;  
	int maxpos = 0;  
	GetScrollRange(SB_VERT, &minpos, &maxpos);   
	maxpos = GetScrollLimit(SB_VERT); 
	int curpos = GetScrollPos(SB_VERT);  
	switch (nSBCode)  
	{  
	case SB_LEFT:      
		curpos = minpos;  
		break;  

	case SB_RIGHT:     
		curpos = maxpos;  
		break;  

	case SB_ENDSCROLL:   
		break;  

	case SB_LINELEFT:       
		if (curpos > minpos)  
			curpos--;  
		break;  

	case SB_LINERIGHT:    
		if (curpos < maxpos)  
			curpos++;  
		break;  

	case SB_PAGELEFT:      
		{  

			SCROLLINFO   info;  
			GetScrollInfo(SB_VERT, &info, SIF_ALL);  

			if (curpos > minpos)  
				curpos = max(minpos, curpos - (int) info.nPage);  
		}  
		break;  

	case SB_PAGERIGHT:        
		{    
			SCROLLINFO   info;  
			GetScrollInfo(SB_VERT, &info, SIF_ALL);  

			if (curpos < maxpos)  
				curpos = min(maxpos, curpos + (int) info.nPage);  
		}  
		break;  

	case SB_THUMBPOSITION:  
		curpos = nPos;     
		break;  

	case SB_THUMBTRACK:   
		curpos = nPos;     
		break;  
	}  

	SetScrollPos(SB_VERT, curpos);  
	ScrollWindow(0,oldpos-curpos);  

	oldpos = curpos;  
	UpdateWindow(); 
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CDlgCustomGeneral::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	//���ù�������Χ  
	SCROLLINFO si;  
	si.cbSize = sizeof(si);  
	si.fMask = SIF_RANGE | SIF_PAGE;  
	si.nMin = 0;  
	si.nMax = 596;  
	si.nPage = cy;  
	SetScrollInfo(SB_VERT,&si,TRUE);  

	int icurypos = GetScrollPos(SB_VERT);  

	if (icurypos < m_iyoldpos)  
	{  
		ScrollWindow(0,m_iyoldpos-icurypos);  
	}     
	m_iyoldpos = icurypos;  

	Invalidate(TRUE); 
}

void CDlgCustomGeneral::OnClickSuperBankerVIPConfig()
{
	((CEdit*)(GetDlgItem(IDC_EDIT_SUPERBANKERCONSUME)))->EnableWindow(false);
	((CComboBox*)(GetDlgItem(IDC_COMBO_SUPERBANKER_VIP)))->EnableWindow(true);

	((CEdit*)(GetDlgItem(IDC_EDIT_SUPERBANKERCONSUME)))->SetWindowText(TEXT(""));
}

void CDlgCustomGeneral::OnClickSuperBankerConsumeConfig()
{
	((CEdit*)(GetDlgItem(IDC_EDIT_SUPERBANKERCONSUME)))->EnableWindow(true);
	((CComboBox*)(GetDlgItem(IDC_COMBO_SUPERBANKER_VIP)))->EnableWindow(false);

	((CComboBox*)(GetDlgItem(IDC_COMBO_SUPERBANKER_VIP)))->SetWindowText(TEXT(""));
}

void CDlgCustomGeneral::OnClickOccupySeatVIPConfig()
{
	((CEdit*)(GetDlgItem(IDC_EDIT_OCCUPYSEATCONSUME)))->EnableWindow(false);
	((CComboBox*)(GetDlgItem(IDC_COMBO_OCCUPYSEAT_VIP)))->EnableWindow(true);

	((CEdit*)(GetDlgItem(IDC_EDIT_OCCUPYSEATCONSUME)))->SetWindowText(TEXT(""));

	((CEdit*)(GetDlgItem(IDC_EDIT_OCCUPYSEATFREE)))->EnableWindow(false);
	((CEdit*)(GetDlgItem(IDC_EDIT_OCCUPYSEATFREE)))->SetWindowText(TEXT(""));
}

void CDlgCustomGeneral::OnClickOccupySeatConsumeConfig()
{
	((CEdit*)(GetDlgItem(IDC_EDIT_OCCUPYSEATCONSUME)))->EnableWindow(true);
	((CComboBox*)(GetDlgItem(IDC_COMBO_OCCUPYSEAT_VIP)))->EnableWindow(false);

	((CComboBox*)(GetDlgItem(IDC_COMBO_OCCUPYSEAT_VIP)))->SetWindowText(TEXT(""));

	((CEdit*)(GetDlgItem(IDC_EDIT_OCCUPYSEATFREE)))->EnableWindow(false);
	((CEdit*)(GetDlgItem(IDC_EDIT_OCCUPYSEATFREE)))->SetWindowText(TEXT(""));
}

void CDlgCustomGeneral::OnClickOccupySeatFreeConfig()
{
	((CComboBox*)(GetDlgItem(IDC_COMBO_OCCUPYSEAT_VIP)))->EnableWindow(false);
	((CComboBox*)(GetDlgItem(IDC_COMBO_OCCUPYSEAT_VIP)))->SetWindowText(TEXT(""));
	((CEdit*)(GetDlgItem(IDC_EDIT_OCCUPYSEATCONSUME)))->EnableWindow(false);
	((CEdit*)(GetDlgItem(IDC_EDIT_OCCUPYSEATCONSUME)))->SetWindowText(TEXT(""));
	((CEdit*)(GetDlgItem(IDC_EDIT_OCCUPYSEATFREE)))->EnableWindow(true);
}

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

BEGIN_MESSAGE_MAP(CDlgCustomAndroid, CDialog)
	ON_BN_CLICKED(IDC_CHECK_ANDROID_1, OnBNClickEnableRobotBanker)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CDlgCustomAndroid::CDlgCustomAndroid() : CDialog(IDD_CUSTOM_ANDROID)
{
}

//��������
CDlgCustomAndroid::~CDlgCustomAndroid()
{
}

//�ؼ���
VOID CDlgCustomAndroid::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	
	DDX_Text(pDX,IDC_EDIT_ANDROID_1,m_CustomAndroid.lRobotBankerCountMin);
	DDX_Text(pDX,IDC_EDIT_ANDROID_2,m_CustomAndroid.lRobotBankerCountMax);
	DDX_Text(pDX,IDC_EDIT_ANDROID_3,m_CustomAndroid.lRobotListMinCount);
	DDX_Text(pDX,IDC_EDIT_ANDROID_4,m_CustomAndroid.lRobotListMaxCount);
	DDX_Text(pDX,IDC_EDIT_ANDROID_5,m_CustomAndroid.lRobotApplyBanker);
	DDX_Text(pDX,IDC_EDIT_ANDROID_6,m_CustomAndroid.lRobotWaitBanker);

	DDX_Text(pDX,IDC_EDIT_ANDROID_7,m_CustomAndroid.lRobotMinBetTime);
	DDX_Text(pDX,IDC_EDIT_ANDROID_8,m_CustomAndroid.lRobotMaxBetTime);
	DDX_Text(pDX,IDC_EDIT_ANDROID_9,m_CustomAndroid.lRobotMinJetton);
	DDX_Text(pDX,IDC_EDIT_ANDROID_10,m_CustomAndroid.lRobotMaxJetton);
	DDX_Text(pDX,IDC_EDIT_ANDROID_11,m_CustomAndroid.lRobotBetMinCount);
	DDX_Text(pDX,IDC_EDIT_ANDROID_12,m_CustomAndroid.lRobotBetMaxCount);
	DDX_Text(pDX,IDC_EDIT_ANDROID_13,m_CustomAndroid.lRobotAreaLimit);

	DDX_Text(pDX,IDC_EDIT_ANDROID_14,m_CustomAndroid.lRobotScoreMin);
	DDX_Text(pDX,IDC_EDIT_ANDROID_15,m_CustomAndroid.lRobotScoreMax);
	DDX_Text(pDX,IDC_EDIT_ANDROID_16,m_CustomAndroid.lRobotBankGetMin);
	DDX_Text(pDX,IDC_EDIT_ANDROID_17,m_CustomAndroid.lRobotBankGetMax);
	DDX_Text(pDX,IDC_EDIT_ANDROID_18,m_CustomAndroid.lRobotBankGetBankerMin);
	DDX_Text(pDX,IDC_EDIT_ANDROID_19,m_CustomAndroid.lRobotBankGetBankerMax);
	DDX_Text(pDX,IDC_EDIT_ANDROID_20,m_CustomAndroid.lRobotBankStoMul);
	//DDX_Check(pDX,IDC_CHECK_ANDROID_1,m_CustomAndroid.nEnableRobotBanker);

}

//��ʼ������
BOOL CDlgCustomAndroid::OnInitDialog()
{
	__super::OnInitDialog();
	
	return TRUE;
}

bool CDlgCustomRule::CheckDataMinMax(LONGLONG valueMax)
{	
	if (valueMax<0||valueMax>LLONG_MAX)
	{  
			
		CString str;
		str.Format(L"���뷶ΧΪ0-%I64d",LLONG_MAX);
		AfxMessageBox(str);

		return false;
	}
  return true;
}

void CDlgCustomAndroid::OnBNClickEnableRobotBanker()
{
	if (((CButton*)GetDlgItem(IDC_CHECK_ANDROID_1))->GetCheck() == 1)
	{
		m_CustomAndroid.nEnableRobotBanker = true;
	}
	else
	{
		m_CustomAndroid.nEnableRobotBanker = false;
	}
}
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661/////////////////////