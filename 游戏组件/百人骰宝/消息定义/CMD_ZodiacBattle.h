#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//�����궨��
#define KIND_ID						127									//��Ϸ I D
#define GAME_PLAYER					100									//��Ϸ����
#define GAME_NAME					TEXT("��������")					//��Ϸ����

//״̬����
#define GS_FREE						GAME_STATUS_FREE					//����״̬
#define	GS_PLACE_JETTON				GAME_STATUS_PLAY					//��ע״̬
#define	GS_GAME_END					GAME_STATUS_PLAY+1					//����״̬

#define VERSION_SERVER			    PROCESS_VERSION(7,0,1)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//����汾


#define BET_AREA_XIAO				0									//ѺС1 : 1 ��4 - 10��
#define BET_AREA_TWO1				1									//˫��1 : 10 ����1��
#define BET_AREA_TWO2				2									//˫��1 : 10 ����2��
#define BET_AREA_TWO3				3									//˫��1 : 10 ����3��
#define BET_AREA_WEI1				4									//Χ��1 : 200 ����1��
#define BET_AREA_WEI2				5									//Χ��1 : 200 ����2��
#define BET_AREA_WEI3				6									//Χ��1 : 200 ����3��
#define BET_AREA_QUAN				7									//ȫΧ1 : 30 ��1, 3, 5���� ��2, 4, 6����
#define BET_AREA_WEI4				8									//Χ��1 : 200 ����4��
#define BET_AREA_WEI5				9									//Χ��1 : 200 ����5��
#define BET_AREA_WEI6				10									//Χ��1 : 200 ����6��
#define BET_AREA_TWO4				11									//˫��1 : 10 ����4��
#define BET_AREA_TWO5				12									//˫��1 : 10 ����5��
#define BET_AREA_TWO6				13									//˫��1 : 10 ����6��
#define BET_AREA_DA                 14									//��1 : 1 ��11 - 17��
#define BET_AREA_4					15									//4�� 1 : 60
#define BET_AREA_5					16									//5�� 1 : 30
#define BET_AREA_6					17									//6�� 1 : 20
#define BET_AREA_7					18									//7�� 1 : 12
#define BET_AREA_8					19									//8�� 1 : 8
#define BET_AREA_9					20									//9�� 1 : 6
#define BET_AREA_10                 21									//10�� 1 : 6
#define BET_AREA_11					22									//11�� 1 : 6
#define BET_AREA_12					23									//12�� 1 : 6
#define BET_AREA_13					24									//13�� 1 : 8
#define BET_AREA_14					25									//14�� 1 : 12
#define BET_AREA_15					26									//15�� 1 : 20
#define BET_AREA_16					27									//16�� 1 : 30
#define BET_AREA_17					28									//17�� 1 : 60
#define BET_AREA_DAN1				29									//������ 1 ��1��1�� 2��2�� 3��3��
#define BET_AREA_DAN2				30									//������ 2
#define BET_AREA_DAN3				31									//������ 3
#define BET_AREA_DAN4				32									//������ 4
#define BET_AREA_DAN5				33									//������ 5
#define BET_AREA_DAN6				34									//������ 6

#define AREA_COUNT					35									//������Ŀ


//��ʷ��¼
#define MAX_SCORE_HISTORY			40									//��ʷ����

#define USER_LIST_COUNT				20									//����б�һ�η�������

#define MAX_CHIP_COUNT				5									//�������

#define MAX_TYPE_COUNT				11									//���͸���

//��������Ϣ
struct tagRobotInfo
{
	int nChip[5];														//���붨��
	TCHAR szCfgFileName[MAX_PATH];										//�����ļ�
	int	nMaxTime;														//�������

	tagRobotInfo()
	{
		int nTmpChip[5] = {1,10,50,100,500};
		TCHAR szTmpCfgFileName[MAX_PATH] = _T("LuxuryCarConfig.ini");

		nMaxTime = 1;
		memcpy(nChip, nTmpChip, sizeof(nChip));
		memcpy(szCfgFileName, szTmpCfgFileName, sizeof(szCfgFileName));
	}
};

//��¼��Ϣ
struct tagServerGameRecord
{
	BYTE							cbResult[3];						//���ӵ���
	BYTE							cbIsBig;							//�Ƿ�Ϊ��  0С 1 ��  2����
};

//��ע��Ϣ
//����������ṹ
#define SUB_S_GAME_READY			2002								//׼����ʼ
#define SUB_S_GAME_START			2003								//��Ϸ��ʼ
#define SUB_S_GAME_END				2004								//��Ϸ����


#define SUB_S_PLACE_JETTON			2005								//�û���ע
#define SUB_S_ONLINE_PLAYER         2008    							//�����û�
#define SUB_S_USER_CONTINUE_BETS	2009								//�û���Ͷ


#define SUB_S_ANDROID_CONFIG		216									//����������

#define SUB_S_RESPONSE_CONTROL		2030								//������Ϣ��Ӧ

//���˿�����Ϣ����
struct CMD_S_ControlInfo
{
	BYTE							cbJuControl;						//�Ƿ���оֿ�
	BYTE							cbJuControlTimes;					//�ֿش���
	BYTE							cbJuControlArea[3];					//���ֿ�����һ��ʤ�� 0�����ƣ�1����Ӯ��2������

	SCORE							lAreaJettonScore[AREA_COUNT];			//������ע
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
	SCORE							lRobotScoreMin;					//�������
	SCORE							lRobotScoreMax;					//�������
	SCORE							lRobotBankGetMin;				//ȡ����Сֵ(��ׯ)
	SCORE							lRobotBankGetMax;				//ȡ�����ֵ(��ׯ)
	SCORE							lRobotBankGetBankerMin;			//ȡ����Сֵ(��ׯ)
	SCORE							lRobotBankGetBankerMax;			//ȡ�����ֵ(��ׯ)
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
struct CMD_S_StatusFree
{
	SCORE							lChips[MAX_CHIP_COUNT];				//����
	SCORE							nTypeLimit[MAX_TYPE_COUNT];			//��������

	BYTE							cbTimeLeave;						//ʣ��ʱ��

	int								nFreeTimes;							//����ʱ��
	int								nPlayTimes;							//��עʱ��
	int								nEndTimes;							//����ʱ��

	tagServerGameRecord				pGameRecord[MAX_SCORE_HISTORY];		//��Ϸ��¼
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	SCORE							lChips[MAX_CHIP_COUNT];				//����
	SCORE							nTypeLimit[MAX_TYPE_COUNT];			//��������

	BYTE							cbTimeLeave;						//ʣ��ʱ��

	int								nFreeTimes;							//����ʱ��
	int								nPlayTimes;							//��עʱ��
	int								nEndTimes;							//����ʱ��

	SCORE							lPlayerJettonScore[AREA_COUNT];		//����µ�ע
	SCORE							lAllJettonScore[AREA_COUNT];		//��������µ�ע

	BYTE							cbResult[3];						//�������
	BYTE							cbResultType;						//�������
	SCORE							lPlayerWinScore;					//���Ӯ��
	SCORE							lPlayerRestScore;					//��һ�ʣ����Ǯ

	tagServerGameRecord				pGameRecord[MAX_SCORE_HISTORY];		//��Ϸ��¼
};

//�û���ע
struct CMD_S_PlaceJetton
{
	BYTE							cbResultCode;						//���	1-6
	TCHAR							cbFailReson[64];					//ʧ��ԭ��
	BYTE							cbJettonType;						//��ע����	
	SCORE							lJettonScore;						//��ע��Ŀ
	WORD							wAddUser;							//�û�λ��

	SCORE							lUserRestScore;						//���ʣ����
};

//��Ϸ����
struct CMD_S_GameEnd
{
	BYTE							cbWinNum;							//�����ܵ���
	BYTE							cbSiceNum[3];						//���ӵ���
	BYTE							cbWinArea[AREA_COUNT];
	SCORE							lAllAreaScore[AREA_COUNT];			//�ܵ�������ע
	SCORE							lPlayerAreaScore[AREA_COUNT];		//���������ע
	SCORE							lUserWinScore;						//������Ӯ
	SCORE							lPlayerRestScore;					//���ʣ����					
};

//�û�׼��
struct CMD_S_GameReady
{
	SCORE							lPlayerAllScore;					//�û���һ����ע
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
#define SUB_C_CLEAN_JETTON			102									//��������
#define SUB_C_CONTINUE_JETTON		103									//������ע
#define SUB_C_ONLINE_PLAYER			104									//��ȡ����б�

#define SUB_C_REQUEST_CONTROL		110									//������Ϣ����
#define SUB_C_CONTROL				111									//���ֿ�����Ӯ
#define SUB_C_CANCEL_CONTROL		112									//ȡ�����ֿ���

//�û���ע
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonType;						//��������
	SCORE							lJettonScore;						//��ע��Ŀ
};

//���˾ֿ�
struct CMD_C_ControlWinLost
{
	BYTE							cbControlTimes;						//���ƴ���
	BYTE							cbJuControlArea[3];					//���ֿ�������ʤ�� 0�����ƣ�1����Ӯ��2������
};
//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif
