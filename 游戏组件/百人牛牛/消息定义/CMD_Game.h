#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

#pragma pack(1)

#define UR_GAME_CONTROL					0x20000000L				//��Ϸ�������
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//������

//��Ϸ����
#define KIND_ID						102									//��Ϸ I D
#define GAME_NAME					TEXT("����ţţ")					//��Ϸ����

//�������
#define GAME_PLAYER					100									//��Ϸ����
#define VERSION_SERVER				PROCESS_VERSION(7,0,1)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//����汾

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//״̬����

#define GAME_SCENE_FREE				GAME_STATUS_FREE					//�ȴ���ʼ
#define GAME_SCENE_BET				GAME_STATUS_PLAY					//��ע״̬
#define GAME_SCENE_END				GAME_STATUS_PLAY+1					//����״̬


//��������
#define ID_TIAN_MEN					1									//��
#define ID_DI_MEN					2									//��
#define ID_XUAN_MEN					3									//��
#define ID_HUANG_MEN				4									//��

//�������
#define BANKER_INDEX				0									//ׯ������
#define SHUN_MEN_INDEX				1									//˳������
#define DUI_MEN_INDEX				2									//��������
#define DAO_MEN_INDEX				3									//��������
#define HUAN_MEN_INDEX				4									//��������
#define MAX_INDEX					3									//�������

#define AREA_MAX					4									//������Ŀ

#define MAX_SEAT_COUNT				6									//���ռλ����
#define MAX_CHIP_COUNT				5									//���������


#define USER_LIST_COUNT				20									//�б������Ӧ��������

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661//
//�궨��
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

/////////////////////////////////////////

struct tagCustomAndroid
{
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
	}
};

//��������Ϣ
struct tagRobotInfo
{
	int nChip[5];														//���붨��
	int nAreaChance[AREA_MAX];											//������
	TCHAR szCfgFileName[MAX_PATH];										//�����ļ�
	int	nMaxTime;														//�������

	tagRobotInfo()
	{
		int nTmpChip[5] = {1, 10, 50, 100, 500};
		int nTmpAreaChance[AREA_MAX] = { 1, 1, 1, 1 };

		nMaxTime = 10;
		memcpy(nChip, nTmpChip, sizeof(nChip));
		memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));
	}
};

//��¼��Ϣ
struct tagServerGameRecord
{
	BYTE							bWinShunMen;						//˳��ʤ��
	BYTE							bWinDuiMen;							//����ʤ��
	BYTE							bWinDaoMen;							//����ʤ��
	BYTE							bWinHuang;							//����ʤ��
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_FREE				2002								//��Ϸ����
#define SUB_S_GAME_START			2003								//��Ϸ��ʼ
#define SUB_S_PLACE_JETTON			2004								//�û���ע
#define SUB_S_GAME_END				2005								//��Ϸ����
#define SUB_S_APPLY_BANKER			2006								//����ׯ��
#define SUB_S_CHANGE_BANKER			2007								//�л�ׯ��
#define SUB_S_SEND_RECORD			2008								//��Ϸ��¼
#define SUB_S_PLACE_JETTON_FAIL		2009								//��עʧ��
#define SUB_S_CANCEL_BANKER			2010								//ȡ������
#define SUB_S_APPLEY_BANKER_FAILURE	2011								//��ׯʧ��
#define SUB_S_CANCEL_FAILURE		2012								//ȡ��ʧ��
#define SUB_S_ONLINE_PLAYER			2014								//�����û�

#define SUB_S_OTHER_JETTON			2021								//���������ע
#define SUB_S_SEAT_JETTON			2022								//ռλ�����ע

#define SUB_S_ANDROID_CONFIG		2020								//����������

#define SUB_S_RESPONSE_CONTROL		2030									//������Ϣ��Ӧ

//���˿�����Ϣ����
struct CMD_S_ControlInfo
{
	BYTE							cbJuControl;							//�Ƿ���оֿ�
	BYTE							cbJuControlTimes;						//�ֿش���
	BYTE							cbJuControlArea[5];						//���ֿ�����һ��ʤ�� 0�����ƣ�1����Ӯ��2������

	SCORE							lAreaJettonScore[AREA_MAX+1];				//������ע
	SCORE							lUserJettonScore[GAME_PLAYER][AREA_MAX+1];	//������ע
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
	WORD							wCancelUser;					//ȡ�����
};

//�л�ׯ��
struct CMD_S_ChangeBanker
{
	WORD							wPreviousBanker;					//��һ����ׯ���
	WORD							wBankerUser;						//��ׯ���
	SCORE							lBankerScore;						//ׯ�ҽ��
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	BYTE							cbHighTimes;						//�Ƿ��Ǹ߱���
	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	int								nChip[MAX_CHIP_COUNT];				//��������

	WORD							wApplyUser[GAME_PLAYER];			//��ׯ�б����

	WORD							wSeatUser[MAX_SEAT_COUNT];			//6��������ҵ����Ӻ�

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
	BYTE							cbHighTimes;							//�Ƿ��Ǹ߱���
	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	int								nChip[MAX_CHIP_COUNT];				//��������

	WORD							wApplyUser[GAME_PLAYER];			//��ׯ�б���ҵ�GameID

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

	SCORE							lPlayerJettonScore[AREA_MAX+1];		//����µ�ע
	SCORE							lAllJettonScore[AREA_MAX+1];		//��������µ�ע

	BYTE							cbTableCardArray[5][5];				//�����˿�
	BYTE							cbCardType[AREA_MAX+1];
	BYTE							cbResult[AREA_MAX];					//���  

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

	WORD							wSeatUser[MAX_SEAT_COUNT];			//6��������ҵ����Ӻ�
};

//�û���ע
struct CMD_S_PlaceJetton
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
	BYTE							cbEarlyOpenFlag;					//��ǰ���Ʊ�ʶ


	SCORE							lPlayAreaScore[AREA_MAX];			//��ҳɼ�
	SCORE							lPlayerWinScore;					//���Ӯ��Ǯ
	SCORE							lPlayerRestScore;					//��һ�ʣ����Ǯ
	SCORE							lBankerWinScore;					//ׯ�ҳɼ�
	SCORE							lBankerRestScore;					//ׯ�һ�ʣ����Ǯ

	SCORE							lSeatUserWinScore[MAX_SEAT_COUNT];	//���µ������Ӯ

	BYTE							cbTableCardArray[5][5];				//�����˿�
	BYTE							cbCardType[AREA_MAX+1];
	BYTE							cbResult[AREA_MAX];					//��� 
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
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonArea;						//��������
	SCORE							lJettonScore;						//��ע��Ŀ
};

//���˾ֿ�
struct CMD_C_ControlWinLost
{
	BYTE							cbControlTimes;						//���ƴ���
	BYTE							cbJuControlArea[5];					//���ֿ�������ʤ�� 0�����ƣ�1����Ӯ��2������
};
//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif