#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

#pragma pack(1)

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//������

//��Ϸ����
#define KIND_ID						100									//��Ϸ I D
#define GAME_NAME					TEXT("������")						//��Ϸ����

//�������
#define GAME_PLAYER					3									//��Ϸ����
#define VERSION_SERVER				PROCESS_VERSION(7,0,1)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//����汾

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//��Ŀ����
#define MAX_COUNT					20									//�����Ŀ
#define FULL_COUNT					54									//ȫ����Ŀ

//�߼���Ŀ
#define NORMAL_COUNT				17									//������Ŀ
#define DISPATCH_COUNT				51									//�ɷ���Ŀ

//��ֵ����
#define	MASK_COLOR					0xF0								//��ɫ����
#define	MASK_VALUE					0x0F								//��ֵ����

//�߼�����
#define CT_ERROR					0									//��������
#define CT_SINGLE					1									//��������
#define CT_DOUBLE					2									//��������
#define CT_THREE					3									//��������
#define CT_SINGLE_LINE				4									//��������
#define CT_DOUBLE_LINE				5									//��������
#define CT_THREE_LINE				6									//��������
#define CT_THREE_TAKE_ONE			7									//����һ��
#define CT_THREE_TAKE_TWO			8									//����һ��
#define CT_FOUR_TAKE_ONE			9									//�Ĵ�����
#define CT_FOUR_TAKE_TWO			10									//�Ĵ�����
#define CT_BOMB_CARD				11									//ը������
#define CT_MISSILE_CARD				12									//�������

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//״̬����

#define GAME_SCENE_FREE				0									//�ȴ���ʼ
#define GAME_SCENE_CALL				200									//�з�״̬
#define GAME_SCENE_MULTIPLE			201									//�ӱ�״̬
#define GAME_SCENE_PLAY				202									//��Ϸ����
#define GAME_SCENE_END				300									//��Ϸ����

//����״̬
struct CMD_S_StatusFree
{
	int								cbCallTimes;						//��ׯʱ��
	int								cbOutTimes;							//����ʱ��
	int								cbDoubleTimes;						//�ӱ�ʱ��
	int								cbEndTimes;							//����ʱ��

	BYTE							nTrusteeStatus[GAME_PLAYER];		//�й�״̬  0����  1�й�
};

//�з�״̬
struct CMD_S_StatusCall
{
	int								cbCallTimes;						//��ׯʱ��
	int								cbOutTimes;							//����ʱ��
	int								cbDoubleTimes;						//�ӱ�ʱ��
	int								cbEndTimes;							//����ʱ��

	BYTE							nTrusteeStatus[GAME_PLAYER];		//�й�״̬  0����  1�й�

	BYTE							cbTimeLeave;						//ʣ��ʱ��
	BYTE							cbScoreInfo[GAME_PLAYER];			//�з���Ϣ
	WORD							wCurrentUser;						//��ǰ���
	BYTE							cbHandCardData[NORMAL_COUNT];		//�����˿�
};

//�ӱ�״̬
struct CMD_S_StatusDouble
{
	int								cbCallTimes;						//��ׯʱ��
	int								cbOutTimes;							//����ʱ��
	int								cbDoubleTimes;						//�ӱ�ʱ��
	int								cbEndTimes;							//����ʱ��

	BYTE							nTrusteeStatus[GAME_PLAYER];		//�й�״̬  0����  1�й�

	BYTE							cbTimeLeave;						//ʣ��ʱ��

	BYTE							cbBankerScore;						//ׯ�ҽз�
	BYTE							cbPlayerMultiple[GAME_PLAYER];		//��ұ���
	WORD							wBankerUser;						//ׯ���û�
	BYTE							cbBankerCard[3];					//��Ϸ����
	BYTE							cbHandCardData[MAX_COUNT];			//�����˿�
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	int								cbCallTimes;						//��ׯʱ��
	int								cbOutTimes;							//����ʱ��
	int								cbDoubleTimes;						//�ӱ�ʱ��
	int								cbEndTimes;							//����ʱ��

	BYTE							nTrusteeStatus[GAME_PLAYER];		//�й�״̬  0����  1�й�

	BYTE							cbTimeLeave;						//ʣ��ʱ��

	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ���
	BYTE							cbBankerScore;						//ׯ�ҽз�

	BYTE							cbBankerCard[3];					//��Ϸ����
	BYTE							cbHandCardData[MAX_COUNT];			//�����˿�
	BYTE							cbHandCardCount[GAME_PLAYER];		//�˿���Ŀ

	BYTE							cbBombCount;						//ը������
	BYTE							cbPlayerMultiple[GAME_PLAYER];		//��ұ���

	WORD							wTurnWiner;							//ʤ�����
	BYTE							cbTurnCardCount;					//������Ŀ
	BYTE							cbTurnCardData[MAX_COUNT];			//��������
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//�����

#define SUB_S_GAME_START			2002									//��Ϸ��ʼ
#define SUB_S_CALL_SCORE			2003									//�û��з�
#define SUB_S_BANKER_INFO			2004									//ׯ����Ϣ
#define SUB_S_GAME_CALL_DOUBLE		2005									//�û��ӱ�
#define SUB_S_BANKER_OUT			2006									//������ʼ����
#define SUB_S_OUT_CARD				2007									//�û�����
#define SUB_S_PASS_CARD				2008									//�û�����
#define SUB_S_TRUSTEE				2009									//�й�
#define SUB_S_GAME_CONCLUDE			2010									//��Ϸ����



//�����˿�
struct CMD_S_GameStart
{
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbCardData[NORMAL_COUNT];			//�˿��б�
};

//�������˿�
struct CMD_S_AndroidCard
{
	BYTE							cbHandCard[GAME_PLAYER][NORMAL_COUNT];//�����˿�
	BYTE							cbBankerCard[3];
	WORD							wCurrentUser ;						//��ǰ���
};

//�����˿�
struct CMD_S_CheatCard
{
	WORD							wCardUser[GAME_PLAYER ];				//�������
	BYTE							cbUserCount;							//��������
	BYTE							cbCardData[GAME_PLAYER ][MAX_COUNT];	//�˿��б�
	BYTE							cbCardCount[GAME_PLAYER ];				//�˿�����

};
//�û��з�
struct CMD_S_CallScore
{
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wCallScoreUser;						//�з����
	BYTE							cbCurrentScore;						//��ǰ�з�
	BYTE							cbUserCallScore;					//�ϴνз�
};

//ׯ����Ϣ
struct CMD_S_BankerInfo
{
	WORD				 			wBankerUser;						//ׯ�����
	BYTE							cbBankerScore;						//ׯ�ҽз�
	BYTE							cbBankerCard[3];					//ׯ���˿�
};

//�û��ӱ�
struct CMD_S_Double
{
	WORD							wDoubleUser;						//�ӱ����
	BYTE							cbPlayerMultiple;					//�ӱ�����
};

//������ʼ����
struct CMD_S_BankerOut
{
	WORD							wBankerUser;						//ׯ�����
};

//�û�����
struct CMD_S_OutCard
{
	BYTE							cbCardCount;						//������Ŀ
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wOutCardUser;						//�������
	BYTE							cbCardData[MAX_COUNT];				//�˿��б�
};

//��������
struct CMD_S_PassCard
{
	WORD				 			wCurrentUser;						//��ǰ���
	WORD				 			wPassCardUser;						//�������
};

//��Ϸ����
struct CMD_S_GameConclude
{
	//���ֱ���
	SCORE							lGameScore[GAME_PLAYER];			//��Ϸ����

	//�����־
	BYTE							bChunTian;							//�����־
	BYTE							bFanChunTian;						//�����־

	//ը����Ϣ
	BYTE							cbBombCount;						//ը������

	//��Ϸ��Ϣ
	BYTE							cbCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							cbHandCardData[GAME_PLAYER][MAX_COUNT];			//�˿��б�
};

//�й�
struct CMD_S_TRUSTEE
{
	WORD							wTrusteeUser;						//�й����
	BYTE							bTrustee;							//�йܱ�־
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//�����

#define SUB_C_CALL_SCORE			100									//�û��з�
#define SUB_C_DOUBLE				101									//�û��ӱ�
#define SUB_C_OUT_CARD				102									//�û�����
#define SUB_C_PASS_CARD				103									//�û�����
#define SUB_C_TRUSTEE				104									//�û��й�


//�û��з�
struct CMD_C_CallScore
{
	BYTE							cbCallScore;						//�з���Ŀ
};

//�û�����
struct CMD_C_OutCard
{
	BYTE							cbCardCount;						//������Ŀ
	BYTE							cbCardData[MAX_COUNT];				//�˿�����
};

//�û��ӱ�
struct CMD_C_CallDouble
{
	BYTE							cbPlayerMultiple;					//�ӱ�����
};

//�й�
struct CMD_C_TRUSTEE
{
	BYTE							bTrustee;							//�йܱ�־
};
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

#pragma pack()

#endif