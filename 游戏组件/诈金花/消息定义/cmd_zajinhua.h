#ifndef CMD_HK_FIVE_CARD_HEAD_FILE
#define CMD_HK_FIVE_CARD_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID							101									//��Ϸ I D
#define GAME_PLAYER						5									//��Ϸ����
#define GAME_NAME						TEXT("ը��")						//��Ϸ����
#define MAX_COUNT						3									//�˿���Ŀ
#define VERSION_SERVER			    	PROCESS_VERSION(7,0,1)				//����汾
#define VERSION_CLIENT				    PROCESS_VERSION(7,0,1)				//����汾

//��Ϸ����
#define GIVE_UP							2									//��������
#define LOOK_CARD						8									//���Ƹ���
#define COMPARE_CARD					5									//���Ƹ���

#define MAX_ROUND						20									//�������

//����ԭ��
#define GER_NO_PLAYER					0x10								//û�����
#define GER_COMPARECARD					0x20								//���ƽ���
#define GER_OPENCARD					0x30								//���ƽ���

//��Ϸ״̬
#define GS_T_FREE						0									//�ȴ���ʼ
#define GS_T_PLAYING					100									//��Ϸ����

enum PlayerStatus
{
	PlayerNoPlay = 0,
	PlayerPlaying,
	PlayerLookCard,
	PlayerGiveUp,
	PlayerComFail,
	PlayerShowHand
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ
#define SUB_S_WAIT_START				2002									//�ȴ���ʼ
#define SUB_S_GAME_START				2003									//��Ϸ��ʼ
#define SUB_S_ADD_SCORE					2004									//��ע���
#define SUB_S_LOOK_CARD					2005									//���ƽ��
#define SUB_S_COMPARE_CARD				2006									//���ƽ��
#define SUB_S_GIVE_UP					2007									//�������
#define SUB_S_GAME_END					2008									//��Ϸ����
#define SUB_S_SHOW_HAND					2009									//ȫѹ
#define SUB_S_OPEN_CARD					2010									//������Ϣ
#define SUB_S_PLAYER_BET_FALURE			2011									//��עʧ��
#define SUB_S_PLAYER_COMMPARE_FALURE	2012									//����ʧ��
#define SUB_S_PLAYER_ALL_BET_FALURE		2013									//ȫѹʧ��
#define SUB_S_PLAYER_FOLLOW_END			2016									//������
#define	SUB_S_ANDROID_CARD				2017									//�������˿�
#define SUB_S_ANDROID_CONFIG			2019									//����������

#define SUB_S_RESPONSE_CONTROL			2020									//������Ϣ��Ӧ
#define	SUB_S_CHANGE_SUCCESS			2021									//���Ƴɹ�

//���˿�����Ϣ����
struct CMD_S_ControlInfo
{
	BYTE								cbHandCardData[GAME_PLAYER][MAX_COUNT];	//�����˿�
	bool								bIsAndroid[GAME_PLAYER];				//�Ƿ���������

	BYTE								cbCanChangeCard[52];					//�ܹ��ı����
};

//���˿��Ƹı�����
struct CMD_S_ControlChangeCard
{
	BYTE								cbCardData[MAX_COUNT];				//�û��˿�
};

//�����˴��ȡ��
struct tagCustomAndroid
{
	LONGLONG							lRobotScoreMin;	
	LONGLONG							lRobotScoreMax;
	LONGLONG	                        lRobotBankGet; 
	LONGLONG							lRobotBankGetBanker; 
	LONGLONG							lRobotBankStoMul; 

	int									nLookCard;
	int									nSingleGiveup;
	int									nShunziGiveup;
	int									nLoserGiveup;
	int									nLoserCompare;
	int									nWinnerCompare;
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	DWORD								nTimeLeave;							//ʣ��ʱ��
	DWORD								nTotalTime;							//����ʱ��
	SCORE								lCellScore;							//��������
	SCORE								lPlayerScore;						//��ҽ��
	int									nMaxRound;							//�������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	DWORD								nTimeLeave;							//ʣ��ʱ��
	DWORD								nTotalTime;							//����ʱ��

	//��ע��Ϣ
	SCORE								lPlayerScore;						//��ҽ��
	SCORE								lCellScore;							//��Ԫ��ע

	SCORE								lMinScore;							//��С��ע���
	SCORE								lMaxScore;							//����ע���

	SCORE								lAllTableScore;						//�������

	int									nCurrentRound;						//��ǰ����
	int									nMaxRound;							//�������

	BYTE								cbFollowStatus;						//������״̬

	//״̬��Ϣ
	WORD								wBankerUser;						//ׯ���û�
	WORD				 				wCurrentUser;						//��ǰ���
	BYTE								cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬0û����һ����Թ���Ϸ,1����Ϸ�У�2 ���ƣ�3 ������4 ����ʧ��
	SCORE								lPlayerBetScore[GAME_PLAYER];		//�����ע��Ŀ

	//�˿���Ϣ
	BYTE								cbHandCardData[MAX_COUNT];			//�˿�����
};

//�������˿�
struct CMD_S_AndroidCard
{
	BYTE								cbRealPlayer[GAME_PLAYER];				//�������
	BYTE								cbAndroidStatus[GAME_PLAYER];			//������Ŀ
	BYTE								cbAllHandCardData[GAME_PLAYER][MAX_COUNT];//�����˿�
	BYTE								cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬0û����һ����Թ���Ϸ,1����Ϸ�У�2 ���ƣ�3 ������4 ����ʧ��					
};

//�ȴ���ʼ
struct CMD_S_WaitStart
{
	SCORE								lPlayerScore;						//�û����
	BYTE								cbUserCount;						//�û�����
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	//��ע��Ϣ
	SCORE								lUserMaxScore;						//��������

	SCORE								lMinScore;							//��С��ע���
	SCORE								lMaxScore;							//����ע���

	//�û���Ϣ
	WORD								wBankerUser;						//ׯ���û�
	WORD				 				wCurrentUser;						//��ǰ���

	SCORE								lPlayerRestScore[GAME_PLAYER];		//���ʣ�����
};

//�û���ע
struct CMD_S_AddScore
{
	WORD								wCurrentUser;						//��ǰ�û�
	WORD								wAddScoreUser;						//��ע�û�
	SCORE								lAddScoreCount;						//��ע��Ŀ

	SCORE								lMinScore;							//��С��ע���
	SCORE								lMaxScore;							//����ע���

	BYTE								cbGameTurns;						//��Ϸ����
	bool								cbShowHandStatus;					//ȫѹ״̬

	SCORE								lPlayerScore;						//��ע�û������
	SCORE								lPlayerTableScore;					//��ע�û��������
	SCORE								lAllTableScore;						//�������
};

//ʧ�ܽṹ
struct CMD_S_FailResult
{
	TCHAR								szInfo[64];							//������Ϣ
};

//�û�����
struct CMD_S_GiveUp
{
	WORD								wGiveUpUser;						//�����û�
	WORD								wCurrentUser;						//��ǰ�û�
	BYTE								cbGameTurns;						//��Ϸ����

};

//�������ݰ�
struct CMD_S_CompareCard
{
	WORD								wCurrentUser;						//��ǰ�û�

	WORD								wCompareUser[2];					//�����û�
	SCORE								lCompareScore;						//���ƽ��
	SCORE								lPlayerScore;						//���ʣ����
	SCORE								lPlayerTableScore;					//�����Ǹ�������ע
	SCORE								lAllTableScore;						//�������
	BYTE								cbGameTurns;						//��Ϸ����
	WORD								wWinUser;							//Ӯ���û�
};

//�������ݰ�
struct CMD_S_LookCard
{
	WORD								wLookCardUser;						//�����û�
	BYTE								cbCardData[MAX_COUNT];				//�û��˿�
	BYTE								cbCardType;
};

//�������ݰ�
struct CMD_S_OpenCard
{
	WORD								wOpenUser;							//�����û�
	BYTE								cbCardData[MAX_COUNT];				//�û��˿�
};

//��Ϸ����
struct CMD_S_GameEnd
{
	SCORE								lGameScore[GAME_PLAYER];			//��Ϸ�÷�
	BYTE								cbCardData[GAME_PLAYER][3];			//�û��˿�
	BYTE								cbCardType[GAME_PLAYER];			//�˿�����					
	//WORD								wCompareUser[GAME_PLAYER][4];		//�����û�
	SCORE								lPlayerScore[GAME_PLAYER];			//�û����
};

//������
struct CMD_S_Follow
{
	BYTE								cbFollowStatus;						//������״̬
};

//////////////////////////////////////////////////////////////////////////

//�ͻ�������ṹ
#define SUB_C_ADD_SCORE					100									//�û���ע
#define SUB_C_GIVE_UP					101									//������Ϣ
#define SUB_C_LOOK_CARD					102									//������Ϣ
#define SUB_C_COMPARE_CARD				103									//������Ϣ
#define SUB_C_PLAYER_ALLIN				104									//���ȫѹ
#define SUB_C_OPEN_CARD					105									//������Ϣ
#define SUB_C_FOLLOW_END				106									//������

#define SUB_C_REQUEST_CONTROL			20									//������Ϣ����
#define SUB_C_CHANGE_CARD				21									//�ı�����

//�û���ע
struct CMD_C_AddScore
{
	SCORE								lScore;								//��ע��Ŀ
};

//�������ݰ�
struct CMD_C_CompareCard
{	
	WORD								wCompareUser;						//�����û�
};

//���˾ֿ�
struct CMD_C_ChangeCard
{
	BYTE									cbCardData[MAX_COUNT];				//��Ҫ��ɵ�����
};
//////////////////////////////////////////////////////////////////////////


#pragma pack()

#endif
