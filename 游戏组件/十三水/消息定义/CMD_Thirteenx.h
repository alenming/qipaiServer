#ifndef CMD_THIRTEENX_HEAD_FILE
#define CMD_THIRTEENX_HEAD_FILE
#pragma pack ( 1 )
//////////////////////////////////////////////////////////////////////////
//�����궨��
#define KIND_ID							126									//��Ϸ I D
#define GAME_PLAYER						4									//��Ϸ����
#define GAME_NAME						TEXT("ʮ��ˮ")						//��Ϸ����
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//��Ϸ����

#define VERSION_SERVER					PROCESS_VERSION(7,0,1)				//����汾
#define VERSION_CLIENT					PROCESS_VERSION(7,0,1)				//����汾

//��������
#define HAND_CARD_COUNT					13									//�˿���Ŀ
#define DISPATCH_COUNT					52									//�ɷ���Ŀ

#define MAX_CARD_LIST					6									//�Զ�ѡ����Ŀ
#define MAX_TYPE_COUNT					4									//ÿ����������Ŀ

//��Ϸ״̬
#define GS_WK_FREE						GAME_STATUS_FREE					//�ȴ���ʼ
#define GS_WK_PLAYING					GAME_STATUS_PLAY					//��Ϸ����
#define GS_WK_END						GAME_STATUS_PLAY+1					//����״̬

enum PlayerStatus
{
	NoPlayer,			//û�����
	LookOn,				//��ս��
	Range_Card,			//������
	Range_End,			//�������
	Show_Card			//��ʾ����
};

#ifndef _UNICODE
#define myprintf	_snprintf
#define mystrcpy	strcpy
#define mystrlen	strlen
#define myscanf		_snscanf
#define myLPSTR		LPCSTR
#define myatoi		atoi
#define myatoi64	_atoi64
#else
#define myprintf	swprintf
#define mystrcpy	wcscpy
#define mystrlen	wcslen
#define myscanf		_snwscanf
#define myLPSTR		LPWSTR
#define myatoi		_wtoi
#define myatoi64	_wtoi64
#endif

//////////////////////////////////////////////////////////////////////////
//����������ṹ
#define SUB_S_GAME_START				2002								//��Ϸ��ʼ
#define SUB_S_GAME_SENDCARD				2003								//��ʼ����
#define SUB_S_GAME_OUTCARD				2004								//�ͻ��˳��ƽ��
#define SUB_S_GAME_END					2006								//����
#define SUB_S_GAME_SETTLEEND			2007								//�������
////////////////////////////////////////////////////////////////////////////
//����״̬
struct CMD_S_StatusFree
{
	SCORE								lCellScore;							//�׷�
	BYTE								cbDelayTime;						//������֮�󵯳����ƽ���ļ��ʱ��
	BYTE								cbEndTime;							//����ʱ��
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	SCORE								lCellScore;							//�׷�
	BYTE								cbDelayTime;						//������֮�󵯳����ƽ���ļ��ʱ��
	BYTE								cbEndTime;							//����ʱ��
	BYTE								cbPlayerStatus[GAME_PLAYER];		//���״̬
	BYTE								cbLeaveTime;						//ʣ��ʱ��

	BYTE								cbCardData[HAND_CARD_COUNT];		//�˿�
	BYTE								cbSpecialType;						//����������
	BYTE								cbSpecialCard[HAND_CARD_COUNT];		//������
	BYTE								cbBestCardList[MAX_CARD_LIST][HAND_CARD_COUNT];	//������
};

//����״̬
struct CMD_S_StatusEnd
{
	SCORE								lCellScore;							//�׷�
	BYTE								cbDelayTime;						//������֮�󵯳����ƽ���ļ��ʱ��
	BYTE								cbEndTime;							//����ʱ��
	BYTE								cbPlayerStatus[GAME_PLAYER];		//���״̬
	BYTE								cbLeaveTime;						//ʣ��ʱ��

	BYTE								cbEndCardList[GAME_PLAYER][HAND_CARD_COUNT];	//��Ϸ���������б�
	BYTE								cbEndCardType[GAME_PLAYER][MAX_TYPE_COUNT];		//����ʱ���ͣ���һ�����������ͣ�����������������
	SCORE								lGameScore[GAME_PLAYER];			//����Ӯ��

	SCORE								lPerScore[GAME_PLAYER][9];			//ÿһ�յķ�����0���������ͷ֣�1~3�����յ÷�,4~6�����ո��ӷ�,7�Ǵ�ǹ�ӷ�,8��ȫ�ݴ�ӷ�
};

//��ʼ����
struct CMD_S_SendCard
{
	BYTE								cbLeaveTime;						//ʣ��ʱ��
	BYTE								cbCard[HAND_CARD_COUNT];			//�û��˿�
	BYTE								cbSpecialType;						//����������
	BYTE								cbSpecialCard[HAND_CARD_COUNT];		//������
	BYTE								cbBestCardList[MAX_CARD_LIST][HAND_CARD_COUNT];	//������

	BYTE								cbPlayerStatus[GAME_PLAYER];		//���״̬
};

//�ͻ��˳��ƽ��
struct CMD_S_OutCard
{
	WORD								wChairID;							//�����û�
	BYTE								cbSpecialType;						//����0Ϊ�����������ͣ�1Ϊ����������
};

//��Ϸ����
struct CMD_S_GameEnd
{
	WORD								wFireList[GAME_PLAYER][GAME_PLAYER-1];	//��ǹ
	BYTE								cbEndCardList[GAME_PLAYER][HAND_CARD_COUNT];	//��Ϸ���������б�
	BYTE								cbEndCardType[GAME_PLAYER][MAX_TYPE_COUNT];		//����ʱ���ͣ���һ�����������ͣ�����������������
	SCORE								lGameScore[GAME_PLAYER];			//����Ӯ��

	SCORE								lPerScore[GAME_PLAYER][9];			//ÿһ�յķ�����0���������ͷ֣�1~3�����յ÷�,4~6�����ո��ӷ�,7�Ǵ�ǹ�ӷ�,8��ȫ�ݴ�ӷ�
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ
#define SUB_C_PLAYER_SHOWCARD			100									//�������

//�������
struct CMD_C_ShowCard
{
	BYTE								cbCardData[HAND_CARD_COUNT];		//�˿�
	BYTE								cbCardType[MAX_TYPE_COUNT];			//����
};
//////////////////////////////////////////////////////////////////////////
#endif
