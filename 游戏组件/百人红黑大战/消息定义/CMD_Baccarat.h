#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

//1�ֽڶ���
#pragma pack(1)

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						    104									//��Ϸ I D
#define GAME_NAME					TEXT("��ڴ�ս")						//��Ϸ����

#define GAME_PLAYER					100									//��Ϸ����

//�������
#define VERSION_SERVER				PROCESS_VERSION(7,0,1)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//����汾

//״̬����
#define GAME_SCENE_FREE				GAME_STATUS_FREE					//�ȴ���ʼ
#define GAME_SCENE_BET				GAME_STATUS_FREE					//��ע״̬
#define	GAME_SCENE_END				GAME_STATUS_PLAY					//����״̬

//�������
#define AREA_XIAN					0									//��
#define AREA_ZHUANG					1									//��
#define AREA_PING					2									//����һ��
#define AREA_MAX					3									//�������

//������multiple
#define MULTIPLE_XIAN				2									//�мұ���
#define MULTIPLE_PING				9									//ƽ�ұ���
#define MULTIPLE_ZHUANG				2									//ׯ�ұ���

//ռ��
#define MAX_SEAT_COUNT		6									//���ռλ����
#define MAX_CHIP_COUNT				5									//���������

#define USER_LIST_COUNT				20									//�б������Ӧ��������

//���ʶ���
#define RATE_TWO_PAIR				12									//��������
#define SERVER_LEN					32									//���䳤��



#ifndef _UNICODE
#define myprintf	_snprintf
#define mystrcpy	strcpy
#define mystrlen	strlen
#define myscanf		_snscanf
#define	myLPSTR		LPCSTR
#else
#define myprintf	swprintf
#define mystrcpy	wcscpy
#define mystrlen	wcslen
#define myscanf		_snwscanf
#define	myLPSTR		LPWSTR
#endif

struct tagServerGameRecord
{
	bool							bPlayer;						
	bool							bBanker;						
	BYTE							bPing;
	BYTE							cbCardType;  //1���ţ�2����,3˳��,4��,5˳��,6����
};
struct tagCustomAndroid
{
	int								nChip[8];
	//��ׯ
	bool							nEnableRobotBanker;				//�Ƿ���ׯ
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
	
	//������
	int								nAreaChance[AREA_MAX];		//������
	//���캯��
	tagCustomAndroid()
	{
		DefaultCustomRule();
	}

	void DefaultCustomRule()
	{
		nEnableRobotBanker = true;
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

		int nTmpAreaChance[8] = {3, 1, 3, 1, 1, 1, 1, 2};
		memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));

	}
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_FREE				2002									//��Ϸ����
#define SUB_S_GAME_START			2003									//��Ϸ��ʼ
#define SUB_S_PLACE_JETTON			2004									//�û���ע
#define SUB_S_PLACE_JETTON_FAIL		2005									//��עʧ��
#define SUB_S_GAME_END				2006									//��Ϸ����
#define SUB_S_APPLY_BANKER			2007									//����ׯ��
#define SUB_S_CHANGE_BANKER			2008									//�л�ׯ��
#define SUB_S_CANCEL_BANKER			2009									//ȡ������
#define SUB_S_APPLEY_BANKER_FAILURE	2011									//��ׯʧ��
#define SUB_S_BET_INFO				2012									//�û���ע��Ϣ
#define SUB_S_ONLINE_PLAYER			2014									//�����û�
#define SUB_S_ONLINE_PLAYER_ERROR	2015									//�����û���ȡʧ��
#define SUB_S_CLEAR_DRAWLIST		2016									//����·��

#define SUB_S_SEND_RECORD			2017									//��Ϸ��¼

#define SUB_S_OTHER_JETTON			2021									//���������ע
#define SUB_S_SEAT_JETTON			2022									//ռλ�����ע

#define SUB_S_ANDROID_CONFIG		2020									//����������

#define SUB_S_RESPONSE_CONTROL		2030									//������Ϣ��Ӧ

//���˿�����Ϣ����
struct CMD_S_ControlInfo
{
	BYTE							cbJuControl;							//�Ƿ���оֿ�
	BYTE							cbJuControlTimes;						//�ֿش���
	BYTE							cbJuControlArea[AREA_MAX];				//���ֿ�����һ��ʤ�� 0�����ƣ�1����Ӯ��2������

	SCORE							lAreaJettonScore[AREA_MAX];				//������ע
	SCORE							lUserJettonScore[GAME_PLAYER][AREA_MAX];	//������ע
	TCHAR							szNickName[GAME_PLAYER][32];			//����ǳ�
};

//����ׯ��
struct CMD_S_ApplyBanker
{
	WORD							wApplyUser;							//�������
};

//������ׯʧ��
struct CMD_S_ApplyFailed
{
	TCHAR							szBuffer[64];						//������Ϣ
};

//ȡ������
struct CMD_S_CancelBanker
{
	WORD							wCancelUser;						//ȡ�����
};

//�л�ׯ��
struct CMD_S_ChangeBanker
{
	WORD							wBankerUser;						//��ׯ���
	SCORE							lBankerScore;						//ׯ�ҷ���
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	int								nChip[MAX_CHIP_COUNT];				//��������

	WORD							wApplyUser[GAME_PLAYER];			//��ׯ�б����

	WORD							wSeatUser[MAX_SEAT_COUNT];	//6��������ҵ����Ӻ�

	WORD							wBankerUser;						//��ǰׯ�ҵ�GameID
	SCORE							lBankerScore;						//ׯ�ҷ���
	SCORE							lAreaLimitScore;					//��������
	SCORE							lApplyBankerCondition;				//��������

	BYTE							cbFreeTime;							//����ʱ��
	BYTE							cbPlayTime;							//��Ϸʱ��
	BYTE							cbEndTime;							//����ʱ��

	SCORE							lMinXianHong;
	SCORE							lMaxXianHong;
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	int								nChip[MAX_CHIP_COUNT];				//��������

	WORD							wApplyUser[GAME_PLAYER];			//��ׯ�б���ҵ�GameID

	WORD							wSeatUser[MAX_SEAT_COUNT];	//6��������ҵ����Ӻ�

	SCORE							lSeatUserAreaScore[MAX_SEAT_COUNT][AREA_MAX];	//6��������ҵ�������ע��Ϣ

	WORD							wBankerUser;						//��ǰׯ�ҵ�GameID
	SCORE							lBankerScore;						//ׯ�ҷ���
	SCORE							lAreaLimitScore;					//��������
	SCORE							lApplyBankerCondition;				//��������

	BYTE							cbFreeTime;							//����ʱ��
	BYTE							cbPlayTime;							//��Ϸʱ��
	BYTE							cbEndTime;							//����ʱ��

	SCORE							lMinXianHong;
	SCORE							lMaxXianHong;

	SCORE							lPlayerJettonScore[AREA_MAX];		//����µ�ע
	SCORE							lAllJettonScore[AREA_MAX];			//��������µ�ע

	BYTE							cbTableCardArray[2][3];				//�����˿�

	BYTE							cbResult[AREA_MAX+1];					//���  0:�� 1�� 2��

	SCORE							lBankerWinScore;					//ׯ��Ӯ��
	SCORE							lPlayerWinScore;					//���Ӯ��
	SCORE							lSeatUserWinScore[MAX_SEAT_COUNT];	//���µ������Ӯ
	SCORE							lPlayerRestScore;					//��һ�ʣ����Ǯ
	SCORE							lBankerRestScore;					//ׯ�һ�ʣ����Ǯ
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	WORD							wSeatUser[MAX_SEAT_COUNT];	//6��������ҵ����Ӻ�
};

//�û���ע
struct CMD_S_PlaceBet
{
	WORD							wChairID;							//�û�λ��
	BYTE							cbBetArea;							//��������
	SCORE							lBetScore;							//��ע��Ŀ

	SCORE							lPlayerRestScore;					//��ע���ʣ����
};

//ʧ�ܽṹ
struct CMD_S_PlaceBetFail
{
	TCHAR							szBuffer[64];						//������Ϣ
};

//��Ϸ����
struct CMD_S_GameEnd
{
	SCORE							lPlayAreaScore[AREA_MAX];			//��ҳɼ�
	SCORE							lPlayerWinScore;					//���Ӯ��Ǯ
	SCORE							lPlayerRestScore;					//��һ�ʣ����Ǯ
	SCORE							lBankerWinScore;					//ׯ�ҳɼ�
	SCORE							lBankerRestScore;					//ׯ�һ�ʣ����Ǯ

	SCORE							lSeatUserWinScore[MAX_SEAT_COUNT];	//���µ������Ӯ
	SCORE							lPlayerJettonScore[AREA_MAX];		//����µ�ע
	SCORE							lAllJettonScore[AREA_MAX];			//��������µ�ע

	BYTE							cbTableCardArray[2][3];				//�����˿�
	BYTE							cbResult[AREA_MAX+1];				//���
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
#define SUB_C_PLACE_JETTON			100									//�û���ע
#define SUB_C_APPLY_BANKER			101									//����ׯ��
#define SUB_C_CANCEL_BANKER			102									//ȡ������
#define SUB_C_ONLINE_PLAYER			103									//�����û�

#define SUB_C_REQUEST_CONTROL		110									//������Ϣ����
#define SUB_C_CONTROL				111									//���ֿ�����Ӯ
#define SUB_C_CANCEL_CONTROL		112									//ȡ�����ֿ���

//�û���ע
struct CMD_C_PlaceBet
{
	
	BYTE							cbBetArea;							//��������
	SCORE							lBetScore;							//��ע��Ŀ
};

//���˾ֿ�
struct CMD_C_ControlWinLost
{
	BYTE							cbControlTimes;						//���ƴ���
	BYTE							cbJuControlArea[AREA_MAX];			//���ֿ�������ʤ�� 0�����ƣ�1����Ӯ��2������
};

//��ԭ������
#pragma pack()
//////////////////////////////////////////////////////////////////////////

#endif
