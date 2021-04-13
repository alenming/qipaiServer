#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

#pragma pack(1)

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//������

//��Ϸ����
#define KIND_ID						119									//��Ϸ I D
#define GAME_NAME					TEXT("ˮ������")					//��Ϸ����

//�������
#define GAME_PLAYER					1									//��Ϸ����
#define VERSION_SERVER				PROCESS_VERSION(7,0,1)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//����汾

//��Ŀ����
#define ITEM_COUNT					9									//ͼ������
#define ITEM_X_COUNT				5									//ͼ������
#define ITEM_Y_COUNT				3									//ͼ������

//�߼�����
#define CT_APPLE					1									//ƻ��
#define CT_MANGO					2									//â��
#define CT_ORANGE					3									//����
#define CT_GRAPE					4									//����
#define CT_WATERMELON				5									//����
#define CT_CHERRY					6									//ӣ��
#define CT_BELL						7									//����
#define CT_DOUBLESEVEN				8									//˫��
#define CT_BAR						9									//Bar
#define CT_WILD						10									//WILD
#define CT_BONUS					11									//BONUS
#define CT_SCATTER					12									//SCATTER(���ת)
#define CT_JACKPOT					13									//JACKPOT

#define ENUM_PRIZE_LINE				1									//����
#define ENUM_PRIZE_BOX				2									//������
#define ENUM_PRIZE_LUCKY			4									//�����˲�
#define ENUM_PRIZE_JACKPOT			8									//�н���
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//״̬����
#define GAME_SCENE_FREE				GAME_STATUS_FREE					//�ȴ���ʼ

//����״̬
struct CMD_S_StatusFree
{
	//��עֵ
	BYTE							cbBetCount;							//��ע����
	SCORE							lBetScore[10];						//��ע��С

	SCORE							lPlayerScore;						//���Ǯ��
	SCORE							lJackPot;							//�����
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//�����

#define SUB_S_BETRESULT				2002								//��ע����
#define SUB_S_BETFAIL				2003								//��עʧ��
#define SUB_S_JACKPOT_RESET			2005								//��������

//��Ϸ����
struct CMD_S_GameConclude
{
	//���ֱ���
	SCORE							lBetScore;									//��ע����
	SCORE							lBonusScore[9];								//�н����ӽ����
	SCORE							lGameScore;									//��Ϸ����
	BYTE							cbItemInfo[ITEM_Y_COUNT][ITEM_X_COUNT];		//������Ϣ
	int								nAllFreeTimes;								//�ܵ���Ѵ���
	int								nUsedFreeTimes;								//�Ѿ�ʹ�õ���Ѵ���
	SCORE							lPlayerRestScore;							//��ҽ��
	int								nLineMask;									//�н���
	SCORE							lJackPot;									//��ǰ����
	BYTE							cbSettleMask;								//0Ϊû�н�
	SCORE							lWinPot;									//����ӯ��
};

//��������
struct CMD_S_JackPot_Reset
{
	SCORE							lJackPot;									//����
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//�����
#define SUB_C_BETREQ				101									//��ע

//�û��з�
struct CMD_C_OneStart
{
	int								lBet;								//Ѻ�߷���
};
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

#pragma pack()

#endif