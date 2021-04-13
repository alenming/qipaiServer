#pragma once

//游戏控制基类
class IServerControl
{
public:
	IServerControl(void){};
	virtual ~IServerControl(void){};

public:
	//激活控制
	virtual bool ActiveControl(IUnknownEx * pIUnknownEx) = NULL;
	//返回控制区域
	virtual VOID * ControlResult(VOID * pData, WORD wDataSize, VOID * pResult) = NULL;
};
