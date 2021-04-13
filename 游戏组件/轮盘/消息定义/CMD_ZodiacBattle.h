#ifndef CMD_ROULETTE_HEAD_FILE
#define CMD_ROULETTE_HEAD_FILE

#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//�����궨��
#define KIND_ID						123									//��Ϸ I D
#define GAME_PLAYER					100									//��Ϸ����
#define GAME_NAME					TEXT("����")						//��Ϸ����

//״̬����
#define	GS_PLACE_JETTON				GAME_STATUS_FREE					//��ע״̬
#define	GS_GAME_END					GAME_STATUS_PLAY					//����״̬

#define VERSION_SERVER			    PROCESS_VERSION(7,0,1)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//����汾


//��������
#define BET_TYPE_STRAIGHT			1									//ֱ��ע(0-36������)   37
#define BET_TYPE_SPLIT				2									//��ע(��0������2����) 57
#define BET_TYPE_STREET				3									//��ע(1��3����)       12
#define BET_TYPE_THREE				4									//����({ 0, 1, 2 } {0, 2, 3})   2
#define BET_TYPE_CORNER				5									//��ע(4����λ����)  22
#define BET_TYPE_FOUR				6									//�ĸ�����(0 - 3)    1
#define BET_TYPE_LINE				7									//��ע(2��6����)     11
#define BET_TYPE_COLUMN				8									//ֱ��ע(��һ��:3. + 3.36 �ڶ��� : 2. + 3.35 ������ : 1. + 3.34)  3
#define BET_TYPE_DOZEN				9									//��ע(��һ��:1 - 12 �ڶ��� : 13 - 24 ������ : 25 - 36)   3
#define BET_TYPE_BLACK				10									//��ע(0ͨɱ)   1
#define BET_TYPE_RED				11									//��ע(0ͨɱ)   1
#define BET_TYPE_ODD				12									//����ע(0ͨɱ) 1
#define BET_TYPE_EVEN				13									//ż��ע(0ͨɱ) 1
#define BET_TYPE_LOW				14									//��ע(1 - 18 0ͨɱ) 1
#define BET_TYPE_HI					15									//��ע(19 - 36 0ͨɱ) 1     154

#define AREA_COUNT					49									//�������

//��ʷ��¼
#define MAX_SCORE_HISTORY			72									//��ʷ����

#define USER_LIST_COUNT				20									//����б�һ�η�������

#define MAX_CHIP_COUNT				4									//�������
#define MAX_SEAT_COUNT				3									//�����������

enum BetAreas
{
	STRAIGHT_0,
	STRAIGHT_1,
	STRAIGHT_2,
	STRAIGHT_3,
	STRAIGHT_4,
	STRAIGHT_5,
	STRAIGHT_6,
	STRAIGHT_7,
	STRAIGHT_8,
	STRAIGHT_9,
	STRAIGHT_10,
	STRAIGHT_11,
	STRAIGHT_12,
	STRAIGHT_13,
	STRAIGHT_14,
	STRAIGHT_15,
	STRAIGHT_16,
	STRAIGHT_17,
	STRAIGHT_18,
	STRAIGHT_19,
	STRAIGHT_20,
	STRAIGHT_21,
	STRAIGHT_22,
	STRAIGHT_23,
	STRAIGHT_24,
	STRAIGHT_25,
	STRAIGHT_26,
	STRAIGHT_27,
	STRAIGHT_28,
	STRAIGHT_29,
	STRAIGHT_30,
	STRAIGHT_31,
	STRAIGHT_32,
	STRAIGHT_33,
	STRAIGHT_34,
	STRAIGHT_35,
	STRAIGHT_36,
	COLUMN_1,
	COLUMN_2,
	COLUMN_3,
	DOZEN_1,
	DOZEN_2,
	DOZEN_3,
	LOW_1,
	EVEN_1,
	RED_1,
	BLACK_1,
	ODD_1,
	HI_1,
	BET_AREA_COUNT
};

//��������Ϣ
struct tagRobotInfo
{
	int nChip[MAX_CHIP_COUNT];											//���붨��
	int nAreaChance[AREA_COUNT];										//������

	tagRobotInfo()
	{
		int nTmpChip[MAX_CHIP_COUNT] = { 1, 10, 100, 500};
		int nTmpAreaChance[AREA_COUNT] = 
		{3, 5, 5, 5, 5, 5, 5, 5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5};

		memcpy(nChip, nTmpChip, sizeof(nChip));
		memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));
	}
};

//��¼��Ϣ
struct tagServerGameRecord
{
	BYTE							cbResult;						   //���ж���
};

//��ע��Ϣ
//����������ṹ
#define SUB_S_GAME_START			2002								//��Ϸ��ʼ
#define SUB_S_GAME_END				2004								//��Ϸ����
#define SUB_S_PLACE_JETTON			2005								//�û���ע
#define SUB_S_PLACE_JETTON_FAIL		2006								//��עʧ��
#define SUB_S_CHEHUI				2007								//��ע����
#define SUB_S_SEND_RECORD			2117								//��Ϸ��¼

#define SUB_S_ONLINE_PLAYER         2008    							//�����û�
#define SUB_S_ONLINE_PLAYER_ERROR	2015								//�����û���ȡʧ��

#define SUB_S_ANDROID_CONFIG		216									//����������

#define SUB_S_RESPONSE_CONTROL		2030								//������Ϣ��Ӧ

//���˿�����Ϣ����
struct CMD_S_ControlInfo
{
	BYTE							cbJuControl;						//�Ƿ���оֿ�
	BYTE							cbJuControlTimes;					//�ֿش���
	BYTE							cbJuControlArea;					//���ֿ�����һ��ʤ�� 0�����ƣ�1����Ӯ��2������

	SCORE							lAreaJettonScore[AREA_COUNT];		//������ע
};

//��ע��Ϣ����
struct tagCustomAndroid
{
	//��ׯ
	BOOL							nEnableRobotBanker;				//�Ƿ���ׯ
	LONGLONG						lRobotBankerCountMin;			//��ׯ����
	LONGLONG						lRobotBankerCountMax;			//��ׯ����
	LONGLONG						lRobotListMinCount;				//�б�����
	LONGLONG						lRobotListMaxCount;				//�б�����
	LONGLONG						lRobotApplyBanker;				//����������
	LONGLONG						lRobotWaitBanker;				//��������

	//��ע
	LONGLONG						lRobotMinBetTime;				//��ע�������
	LONGLONG						lRobotMaxBetTime;				//��ע�������
	SCORE							lRobotMinJetton;				//��ע������
	SCORE							lRobotMaxJetton;				//��ע������
	LONGLONG						lRobotBetMinCount;				//��ע��������
	LONGLONG						lRobotBetMaxCount;				//��ע��������
	SCORE							lRobotAreaLimit;				//��������

	//��ȡ��
	LONGLONG						lRobotScoreMin;					//�������
	LONGLONG						lRobotScoreMax;					//�������
	LONGLONG						lRobotBankGetMin;				//ȡ����Сֵ(��ׯ)
	LONGLONG						lRobotBankGetMax;				//ȡ�����ֵ(��ׯ)
	LONGLONG						lRobotBankGetBankerMin;			//ȡ����Сֵ(��ׯ)
	LONGLONG						lRobotBankGetBankerMax;			//ȡ�����ֵ(��ׯ)
	LONGLONG						lRobotBankStoMul;				//���ٷֱ�

	//���캯��
	tagCustomAndroid()
	{
		DefaultCustomRule();
	}

	void DefaultCustomRule()
	{
		nEnableRobotBanker = TRUE;
		lRobotBankerCountMin = 5;
		lRobotBankerCountMax = 10;
		lRobotListMinCount = 2;
		lRobotListMaxCount = 5;
		lRobotApplyBanker = 5;
		lRobotWaitBanker = 3;

		lRobotMinBetTime = 6;
		lRobotMaxBetTime = 8;
		lRobotMinJetton = 100;
		lRobotMaxJetton = 5000000;
		lRobotBetMinCount = 4;
		lRobotBetMaxCount = 8;
		lRobotAreaLimit = 10000000;

		lRobotScoreMin = 1000000;
		lRobotScoreMax = 100000000;
		lRobotBankGetMin = 100;
		lRobotBankGetMax = 30000000;
		lRobotBankGetBankerMin = 10000000;
		lRobotBankGetBankerMax = 50000000;
		lRobotBankStoMul = 50;
	}
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	SCORE							lChips[MAX_CHIP_COUNT];				//����

	SCORE							lTypeLimit[7];						//����������ע

	WORD							wSeatUser[MAX_SEAT_COUNT];			//3��������ҵ����Ӻ�
	SCORE							lSeatUserAreaScore[MAX_SEAT_COUNT][AREA_COUNT];	//3��������ҵ�������ע��Ϣ

	BYTE							cbTimeLeave;						//ʣ��ʱ��
	int								nPlayTimes;							//��עʱ��
	int								nEndTimes;							//����ʱ��

	BYTE							cbWinMen[40];						//��Ϸ��¼

	SCORE							lPlayerJettonScore[AREA_COUNT];		//����µ�ע
	SCORE							lAllJettonScore[AREA_COUNT];		//��������µ�ע

	BYTE							cbResult;							//�������
	SCORE							lPlayerWinScore;					//���Ӯ��
	SCORE							lSeatUserWinScore[MAX_SEAT_COUNT];	//���µ������Ӯ
	SCORE							lPlayerRestScore;					//��һ�ʣ����Ǯ
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	WORD							wSeatUser[MAX_SEAT_COUNT];			//3��������ҵ����Ӻ�
};

//�û���ע
struct CMD_S_PlaceJetton
{
	BYTE							cbJettonType;						//��ע����
	BYTE							cbJettonArea[18];					//��������	
	SCORE							lJettonScore;						//��ע��Ŀ
	WORD							wChairID;							//�û�λ��

	SCORE							lUserRestScore;						//���ʣ����

	int								nKeys;
};

//��עʧ��
struct CMD_S_PlaceJetton_Fail
{
	BYTE							cbJettonArea[18];					//ʧ������
	TCHAR							szFailReason[64];					//ʧ��ԭ��
};

//�û�����
struct CMD_S_CheHui
{
	WORD							wChairID;							//���ص��û�
	SCORE							lUserRestScore;						//�û�ʣ����
};

//��Ϸ����
struct CMD_S_GameEnd
{

	SCORE							lPlayerJettonScore[AREA_COUNT];		//����µ�ע
	SCORE							lAllJettonScore[AREA_COUNT];		//��������µ�ע
	SCORE							lSeatUserWinScore[MAX_SEAT_COUNT];	//���µ������Ӯ
	SCORE							lPlayAreaScore[AREA_COUNT];			//��ҳɼ�
	SCORE							lPlayerWinScore;					//���Ӯ��Ǯ
	SCORE							lPlayerRestScore;					//��һ�ʣ����Ǯ				
	BYTE							cbResult;							//�������
};

//����б�
struct CMD_S_UserList
{
	WORD							wCount;								//��������
	bool							bEnd;								//�Ƿ����
	BYTE							cbIndex[USER_LIST_COUNT];			//����
	TCHAR							szUserNick[USER_LIST_COUNT][32];	//�ǳ�
	SCORE							lBetScore[USER_LIST_COUNT];			//��20����ע���
	BYTE							cbWinTimes[USER_LIST_COUNT];		//��20��Ӯ�˶��پ�
	SCORE							lUserScore[USER_LIST_COUNT];		//��ҽ��
	BYTE							wFaceID[USER_LIST_COUNT];			//���ͷ��
	WORD							wChairID[USER_LIST_COUNT];
};

//����б�������
struct CMD_S_UserListInfo
{
	WORD							wWinNum;							//��ʤ����
	SCORE							lAllBet;							//��ע����
	SCORE							lUserScore;							//�û�����
	TCHAR							szNickName[32];						//�û��ǳ�
	BYTE							wFaceID;							//���ͷ��
	WORD							wChairID;
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_PLACE_JETTON			101									//�û���ע
#define SUB_C_XUTOU					102									//�û���Ͷ
#define SUB_C_CHEHUI				103									//������ע
#define SUB_C_ONLINE_UESER			104									//��ȡ�û��б�

#define SUB_C_REQUEST_CONTROL		110									//������Ϣ����
#define SUB_C_CONTROL				111									//���ֿ�����Ӯ
#define SUB_C_CANCEL_CONTROL		112									//ȡ�����ֿ���

//�û���ע
struct CMD_C_PlaceJetton
{
	BYTE							cbBetArea[18];						//��������
	BYTE							cbBetType;
	SCORE							lBetScore;							//��ע��Ŀ
	int								nBetKeys;							//���ظ��ͻ��˵�
};

//���˾ֿ�
struct CMD_C_ControlWinLost
{
	BYTE							cbControlTimes;				//���ƴ���
	BYTE							cbJuControlArea;			//���ֿ�������ʤ�� 0�����ƣ�1����Ӯ��2������
};
//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif
