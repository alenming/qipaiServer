#pragma once

//��Ϸ��Ƶ����
class IGameVideo
{
public:
	IGameVideo(void){};
	virtual ~IGameVideo(void){};

public:
	//��ʼ¼��
	virtual bool __cdecl	StartVideo(ITableFrame	*pTableFrame, BYTE cbPlayerCount) = NULL;
	//ֹͣ�ͱ���
	virtual bool __cdecl	StopAndSaveVideo(WORD wServerID,WORD wTableID)	= NULL;
	//����¼������
	virtual bool __cdecl    AddVideoData(WORD wMsgKind, Video_GameStart *pVideoGameStart, bool bFirst) = NULL;
	virtual bool __cdecl    AddVideoData(WORD wMsgKind, CMD_S_CallBanker *pVideoCallBanker) = NULL;
	virtual bool __cdecl    AddVideoData(WORD wMsgKind, CMD_S_SendFourCard *pVideoSendFourCard) = NULL;
	virtual bool __cdecl    AddVideoData(WORD wMsgKind,CMD_S_CallBankerInfo *pVideoCallBankerInfo) = NULL;
	virtual bool __cdecl    AddVideoData(WORD wMsgKind,CMD_S_AddScore *pVideoAddScore) = NULL;
	virtual bool __cdecl    AddVideoData(WORD wMsgKind,CMD_S_SendCard *pVideoSendCard) = NULL;
	virtual bool __cdecl    AddVideoData(WORD wMsgKind,CMD_S_Open_Card *pVideoOpen_Card) = NULL;
	virtual bool __cdecl    AddVideoData(WORD wMsgKind,CMD_S_GameEnd *pVideoGameEnd) = NULL;	
};
