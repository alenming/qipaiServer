#ifndef CMD_OXSIXX_HEAD_FILE
#define CMD_OXSIXX_HEAD_FILE

#pragma pack(push)  
#pragma pack(1)

#define		CARD_CONFIG

//////////////////////////////////////////////////////////////////////////
//�����궨��
#define KIND_ID							103									//��Ϸ I D
#define GAME_PLAYER						5									//��Ϸ����
#define GAME_NAME						TEXT("��ׯţţ")					//��Ϸ����
#define MAX_CARDCOUNT					5									//�����Ŀ
#define MAX_CONFIG						4									//������ø���


#define VERSION_SERVER					PROCESS_VERSION(7,0,1)				//����汾
#define VERSION_CLIENT					PROCESS_VERSION(7,0,1)				//����汾

//��Ϸ״̬
#define GS_TK_FREE						GAME_STATUS_FREE					//����״̬
#define GS_TK_WAIT						GAME_STATUS_PLAY					//�ȴ���ʼ
#define GS_TK_CALL						GAME_STATUS_PLAY+1					//��ׯ״̬
#define GS_TK_SCORE						GAME_STATUS_PLAY+2					//��ע״̬
#define GS_TK_END						GAME_STATUS_PLAY+3					//��Ϸ����

#define SEND_COUNT						3									//��ǰ��������

//////////////////////////////////////////////////////////////////////////
//����������ṹ
#define SUB_S_WAIT_START				2001								//�ȴ���ʼ
#define SUB_S_GAME_START				2002								//��Ϸ��ʼ
#define SUB_S_DISPATCH_CARD				2003								//�ɷ��˿�
#define SUB_S_CALL_BANKER				2004								//��ׯ
#define SUB_S_SET_BANKER				2005								//����ׯ��
#define SUB_S_PLAYER_BET				2006								//��ע
#define SUB_S_GAME_END					2007								//��Ϸ����

#define SUB_S_CUSTOM_ANDROID			2010								//����������
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661////

//Ԥ�������˴��ȡ��
struct tagCustomAndroid
{
	SCORE								lRobotScoreMin;	
	SCORE								lRobotScoreMax;
	SCORE	                            lRobotBankGet; 
	SCORE								lRobotBankGetBanker; 
	SCORE								lRobotBankStoMul; 
	LONGLONG							lAddChip[4];							//��ע����
};
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661////

//��Ϸ״̬
struct CMD_S_StatusFree
{
	SCORE								lCellScore;							//��������
	BYTE								cbPlayerStatus[GAME_PLAYER];		//���״̬

	BYTE								cbStartTime;						//��ʼʱ��
	BYTE								cbCallTime;							//��ׯʱ��
	BYTE								cbAddTime;							//��עʱ��
	BYTE								cbEndTime;							//����ʱ��

	BYTE								cbTimeLeave;						//ʣ��ʱ��
};

//��ׯ״̬
struct CMD_S_StatusCall
{
	SCORE								lCellScore;							//��������
	BYTE								cbPlayerStatus[GAME_PLAYER];		//���״̬

	BYTE								cbStartTime;						//��ʼʱ��
	BYTE								cbCallTime;							//��ׯʱ��
	BYTE								cbAddTime;							//��עʱ��
	BYTE								cbEndTime;							//����ʱ��

	BYTE								cbTimeLeave;						//ʣ��ʱ��

	BYTE								cbCallBankerTimes[GAME_PLAYER];		//��ׯ����
	BYTE								cbCardData[GAME_PLAYER][MAX_CARDCOUNT];	//�û��˿�
};

//��ע״̬
struct CMD_S_StatusScore
{
	SCORE								lCellScore;							//��������
	BYTE								cbPlayerStatus[GAME_PLAYER];		//���״̬

	BYTE								cbStartTime;						//��ʼʱ��
	BYTE								cbCallTime;							//��ׯʱ��
	BYTE								cbAddTime;							//��עʱ��
	BYTE								cbEndTime;							//����ʱ��

	BYTE								cbTimeLeave;						//ʣ��ʱ��

	WORD								wBankerUser;						//ׯ���û�
	BYTE								cbBankerTimes;						//ׯ�ұ���

	LONGLONG							lBetTimes[GAME_PLAYER];				//��ע����

	BYTE								cbCardData[GAME_PLAYER][MAX_CARDCOUNT];	//�û��˿�
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	BYTE								cbTime;								//ʱ��
};

//�����˿�
struct CMD_S_SendCard
{
	BYTE								cbPlayerStatus[GAME_PLAYER];		//���״̬
	//(����ģʽΪ�������壬�������ƣ� ����ȫΪ0)
	BYTE								cbCardData[GAME_PLAYER][MAX_CARDCOUNT];	//�û��˿�
};

//�û���ׯ��Ϣ
struct CMD_S_CallBankerInfo
{
	WORD								wCallUser;							//��ׯ�û�
	BYTE								cbCallBankerTimes;					//��ׯ����
};

//ȷ��ׯ��
struct CMD_S_SetBanker
{
	WORD								wBankerUser;						//ׯ���û�
	WORD								wCallList[GAME_PLAYER];				//����һ���������û��б�
};

//�û���ע
struct CMD_S_AddScore
{
	WORD								wBetUser;							//��ע�û�
	int									nBetTimes;							//��ע��Ŀ
};

//��Ϸ����
struct CMD_S_GameEnd
{
	BYTE								cbFleeEnd;							//��Ϊ�û���������Ľ���
	BYTE								cbHandCardData[GAME_PLAYER][MAX_CARDCOUNT];//�����˿�
	BYTE								cbCardType[GAME_PLAYER];			//�������

	SCORE								lPlayerScore[GAME_PLAYER];			//�û�������ʣ����Ǯ
	SCORE								lGameScore[GAME_PLAYER];			//��Ϸ�÷�
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ
#define SUB_C_CALL_BANKER				1									//�û���ׯ
#define SUB_C_ADD_SCORE					2									//�û���ע

//�û���ׯ
struct CMD_C_CallBanker
{
	BYTE								cbBankerTimes;						//��ׯ����(���û�����ׯ����ֵ0)
};

//�û���ע
struct CMD_C_AddScore
{
	int									nScore;								//��ע��Ŀ
};
#pragma pack(pop)

#endif
