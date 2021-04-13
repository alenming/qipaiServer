#pragma once


/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661//////////

//配置结构
struct tagCustomRule
{
	BYTE									cbBetCount;					//下注数量
	SCORE									lBetScore[10];				//下注大小
};

class CDlgCustomRule : public CDialog
{
	DECLARE_MESSAGE_MAP()
	//函数定义
public:
	//构造函数
	CDlgCustomRule();
	//析构函数
	virtual ~CDlgCustomRule();

	//重载函数
protected:
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();

	//功能函数
public:
	//更新控件
	bool FillDataToControl();
	//更新数据
	bool FillControlToData();

	//保留俩位小数
	SCORE GetFloorDouble(SCORE dNum);

	//配置函数
public:
	//读取配置
	bool GetCustomRule(tagCustomRule & CustomRule);
	//设置配置
	bool SetCustomRule(tagCustomRule & CustomRule);

	//配置变量
protected:
	tagCustomRule					m_CustomRule;						//配置结构
};
/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661//////////