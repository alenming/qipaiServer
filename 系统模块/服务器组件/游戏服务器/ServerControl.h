#pragma once

//��Ϸ���ƻ���
class IServerControl
{
public:
	IServerControl(void){};
	virtual ~IServerControl(void){};

public:
	//�������
	virtual bool ActiveControl(IUnknownEx * pIUnknownEx) = NULL;
	//���ؿ�������
	virtual VOID * ControlResult(VOID * pData, WORD wDataSize, VOID * pResult) = NULL;
};
