#pragma once

//��Ϸ���ƻ���
class IServerControl
{
public:
	IServerControl(void){};
	virtual ~IServerControl(void){};

public:
	//���ؿ�������
	virtual bool __cdecl ControlResult(BYTE cbControlCardData[GAME_PLAYER][MAX_CARDCOUNT], ROOMUSERCONTROL Keyroomusercontrol, SENDCARDTYPE_CONFIG stConfig, CARDTYPE_CONFIG ctConfig, KING_CONFIG gtConfig) = NULL;
};
