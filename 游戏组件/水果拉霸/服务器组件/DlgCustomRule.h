#pragma once


/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661//////////

//���ýṹ
struct tagCustomRule
{
	BYTE									cbBetCount;					//��ע����
	SCORE									lBetScore[10];				//��ע��С
};

class CDlgCustomRule : public CDialog
{
	DECLARE_MESSAGE_MAP()
	//��������
public:
	//���캯��
	CDlgCustomRule();
	//��������
	virtual ~CDlgCustomRule();

	//���غ���
protected:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();

	//���ܺ���
public:
	//���¿ؼ�
	bool FillDataToControl();
	//��������
	bool FillControlToData();

	//������λС��
	SCORE GetFloorDouble(SCORE dNum);

	//���ú���
public:
	//��ȡ����
	bool GetCustomRule(tagCustomRule & CustomRule);
	//��������
	bool SetCustomRule(tagCustomRule & CustomRule);

	//���ñ���
protected:
	tagCustomRule					m_CustomRule;						//���ýṹ
};
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661//////////