#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

#pragma pack(1)

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//������

//��Ϸ����
#define KIND_ID						113									//��Ϸ I D
#define GAME_NAME					TEXT("�����ܵÿ�")					//��Ϸ����

//�������
#define GAME_PLAYER					3									//��Ϸ����
#define VERSION_SERVER				PROCESS_VERSION(7,0,1)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//����汾

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//״̬����

#define GAME_SCENE_FREE				GAME_STATUS_FREE					//�ȴ���ʼ
#define GAME_SCENE_PLAY				200									//��Ϸ����


#define MAX_COUNT					16
//////////////////////////////////////////////////////////////////////////
//����������ṹ
#define SUB_S_GAME_FREE				2001									//�ȴ���ʼ
#define SUB_S_GAME_START			2002									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				2003									//�û�����
#define SUB_S_PASS_CARD				2004									//��������
#define SUB_S_GAME_END				2006									//��Ϸ����
#define SUB_S_AUTOMATISM			2005									//�й���Ϣ

//��Ϸ״̬
struct CMD_S_StatusFree
{
	SCORE						lBaseScore;							//��������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	BYTE							bCardData[MAX_COUNT];				//�Լ�������
	bool							bAutoStatus[GAME_PLAYER];			//�й�״̬
	BYTE							bCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							bTurnCardData[MAX_COUNT];			//�����б�
	WORD							wLastOutUser;						//���Ƶ���
	WORD				 			wCurrentUser;						//��ǰ���

	WORD							wOutCardTime;						//����ʱ��
	WORD							wEndTime;							//����ʱ��
};

//�����˿�
struct CMD_S_GameStart
{
	WORD							wBankerUser;						//ׯ���û�
	BYTE							cbCardData[MAX_COUNT];				//�˿��б�

	WORD							wOutCardTime;						//����ʱ��
	WORD							wEndTime;							//����ʱ��
};

//�û�����
struct CMD_S_OutCard
{
	bool							bResult;							//���ƽ����1�ɹ�������ʧ�ܣ�
	WORD							wOutCardUser;						//�������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bCardCount;							//�˿���Ŀ
	BYTE							bCardData[MAX_COUNT];				//�˿��б�
};

//��������
struct CMD_S_PassCard
{
	bool							bNewTurn;							//һ�ֿ�ʼ
	WORD				 			wPassUser;							//�������
	WORD				 			wCurrentUser;						//��ǰ���
};

//��Ϸ����
struct CMD_S_GameEnd
{
	WORD							wWinUser;							//Ӯ��
	BYTE							bCardData[GAME_PLAYER][MAX_COUNT];	//�˿��б� 
	
	TCHAR							szPlayerName[GAME_PLAYER][32];		//�������
	BYTE							cbCardCount[GAME_PLAYER];			//�˿���Ŀ
	SCORE							lGameScore[GAME_PLAYER];			//��Ӯ���
	BYTE							cbBombCount[GAME_PLAYER];			//ը������
	SCORE							lBombScore[GAME_PLAYER];			//ը���÷�
	BYTE							cbQuanGuan[GAME_PLAYER];			//�Ƿ�ȫ��
	BYTE							cbBaopei[GAME_PLAYER];				//�Ƿ����
};

//����й��¼�
struct CMD_S_UserAutomatism
{
	WORD							wChairID;							//���Ӻ�
	bool							bTrusee;							//�й�
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CART				101									//�û�����
#define SUB_C_PASS_CARD				102									//��������
#define SUB_C_AUTOMATISM			103									//�й���Ϣ

//�û��й�
struct CMD_C_Automatism
{
	BYTE							bAutomatism;						//�йܱ�־
};

//�������ݰ�
struct CMD_C_OutCard
{
	BYTE							bCardCount;							//������Ŀ
	BYTE							bCardData[MAX_COUNT];				//�˿��б�
};

//////////////////////////////////////////////////////////////////////////
#pragma pack()

#endif