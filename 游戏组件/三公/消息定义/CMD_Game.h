#ifndef CMD_SG_GAME_H
#define CMD_SG_GAME_H

#pragma pack(push)
#pragma pack(1)


#define KIND_ID             352                          //��Ϸ I D
#define GAME_NAME			TEXT("����")	             //��Ϸ����
#define GAME_PLAYER			9				             //��Ϸ����

#define VERSION_SERVER          PROCESS_VERSION(7,0,1)   //����汾
#define VERSION_CLIENT          PROCESS_VERSION(7,0,1)   //����汾



#define FULL_COUNT          52					//�����Ƶ�����
#define MAX_COUNT           3                   //�������
#define NO_BANKER			0xFFFF				//û��ׯ��
#define INVALID_BANKER      0xFF                //��Чׯ��

//��ׯ״̬
#define OPERATE_NO_BANKERER           0          //����ׯ
#define OPERATE_SCORE_ONE             1          //��һע

//��Ϸ״̬
#define GS_TK_FREE            GAME_STATUS_FREE          //�ȴ���ʼ
#define GS_TK_CALL            GAME_STATUS_PLAY          //��ׯ״̬
#define GS_TK_SCORE           GAME_STATUS_PLAY+1        //��ע״̬

//////////////////////////////////////////////////////////////////////////
//����������
#define SUB_S_WAIT_START		100					//�ȴ���ʼ
#define SUB_S_GAME_START        101                 //��Ϸ��ʼ
#define SUB_S_CALL_BANKER       102                 //�û���ׯ
#define SUB_S_BANKER_END        103                 //��ׯ���
#define SUB_S_ADD_SCORE         104                 //��ע���
#define SUB_S_GAME_END          105                 //��Ϸ����
#define SUB_S_ANDROID_CONFIG	106					//���ͻ���������

//��Ϸ״̬
/////////////////////////////////////////////////////////////////////////////////
//����״̬
struct CMD_S_StatusFree
{
	SCORE				lCellScore;							//��Ϸ�׷�

};

//��ׯ״̬
struct CMD_S_StatusCall
{
	SCORE				lCellScore;							//��Ϸ�׷�
	BYTE				cbTimeLeave;						//ʣ��ʱ��
	BYTE                cbPlayStatus[GAME_PLAYER];		    //�û�״̬
	BYTE				m_cbCallStatus[GAME_PLAYER];		//��ׯ״̬
	BYTE                cbCardData[MAX_COUNT];				//�û��˿�
};

//��ע״̬
struct CMD_S_StatusScore
{
	SCORE				lCellScore;							//��Ϸ�׷�
	BYTE				cbTimeLeave;						//ʣ��ʱ��
	WORD                wBankerUser;                        //ׯ���û�
	BYTE                cbPlayStatus[GAME_PLAYER];          //�û�״̬
	BYTE				cbCallStatus[GAME_PLAYER];			//��ׯ״̬
	SCORE				lTableScore[GAME_PLAYER];           //��ע��Ŀ
	BYTE                cbCardData[MAX_COUNT];              //�û��˿�
};


//����������ṹ
/////////////////////////////////////////////////////////////////////
//��Ϸ��ʼ
struct CMD_S_GameStart
{
	BYTE				cbPlayStatus[GAME_PLAYER];			//���״̬
	BYTE				cbCardData[MAX_COUNT];				//�û��˿�
};

//����������
struct tagRobotConfig
{
	LONGLONG            lRobotScoreMin;
	LONGLONG            lRobotScoreMax;
	LONGLONG            lRobotBankGet;
	LONGLONG            lRobotBankGetBanker;
	LONGLONG            lRobotBankStoMul;
};

//�û���ׯ
struct CMD_S_Call_Banker
{
	WORD				cbBanker;						//��ׯ�û�
	BYTE				cbStatus;						//��ׯ״̬
};

//��ׯ���
struct CMD_S_Banker_End
{
	WORD				cbBanker;						//ׯ���û�
	BYTE				cbCallStatus[GAME_PLAYER];		//��ׯ�û�
};

//�û���ע
struct CMD_S_AddScore
{
	WORD                wAddScoreUser;					//��ע�û�
	SCORE				lAddScoreCount;					//��ע��Ŀ
};


//��Ϸ����
struct CMD_S_GameEnd
{
	BYTE				cbPlayerStatus[GAME_PLAYER];		//���״̬
	BYTE				cbPlayerValue[GAME_PLAYER];			//��ҵ���
	SCORE				lGameScore[GAME_PLAYER];			//��Ϸ�÷�
	BYTE				cbCardData[GAME_PLAYER][MAX_COUNT]; //�û��˿�
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ
#define SUB_C_CALL_BANKER       1					//�û���ׯ
#define SUB_C_ADD_SCORE         2					//�û���ע

//�û���ׯ
struct CMD_C_CallBanker
{
	BYTE						bBanker;            //��ׯ��־
};


//�û���ע
struct CMD_C_AddScore
{
	SCORE						lScore;				//��ע��Ŀ
};

//////////////////////////////////////////////////////////////////////////

#pragma pack(pop)

#endif
