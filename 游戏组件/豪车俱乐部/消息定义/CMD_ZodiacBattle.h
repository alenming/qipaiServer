#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//�����궨��
#define KIND_ID						111									//��Ϸ I D
#define GAME_PLAYER					100							//��Ϸ����
#define GAME_NAME					TEXT("�������ֲ�")					//��Ϸ����

//״̬����
#define	GS_PLACE_JETTON				GAME_STATUS_PLAY					//��ע״̬
#define	GS_GAME_END					GAME_STATUS_PLAY+1					//����״̬

#define VERSION_SERVER			    PROCESS_VERSION(7,0,1)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//����汾


//��������
#define ID_S_DZ						1									//С����	
#define ID_S_AD						2									//С�µ�		
#define ID_S_BM						3									//С����		
#define ID_S_BC						4									//С����		
#define ID_B_DZ						5									//�����
#define ID_B_AD						6									//��µ�
#define ID_B_BM						7									//����
#define ID_B_BC						8									//�󱼳�
		
#define AREA_S_DZ					0									//С����
#define AREA_S_AD					1									//С�µ�
#define AREA_S_BM					2									//С����
#define AREA_S_BC					3									//С����
#define AREA_B_DZ					4									//�����
#define AREA_B_AD					5									//��µ�
#define AREA_B_BM					6									//����
#define AREA_B_BC					7									//�󱼳�

#define AREA_COUNT					8									//������Ŀ

#define AREA_ALL					8									//��������

#define ANIMAL_COUNT                32                                  //�������
//���ʶ���
#define RATE_TWO_PAIR				12									//��������

#define SERVER_LEN					32									//���䳤��

//��ʷ��¼
#define MAX_SCORE_HISTORY			50									//��ʷ����

#define USER_LIST_COUNT				20									//����б�һ�η�������

//��������Ϣ
struct tagRobotInfo
{
	int nChip[5];														//���붨��
	int nAreaChance[AREA_COUNT];										//������
	TCHAR szCfgFileName[MAX_PATH];										//�����ļ�
	int	nMaxTime;														//�������

	tagRobotInfo()
	{
		int nTmpChip[5] = {1,10,100,500,1000};
		int nTmpAreaChance[AREA_COUNT] = {3, 3, 3, 3, 10, 10, 10, 10};
		TCHAR szTmpCfgFileName[MAX_PATH] = _T("LuxuryCarConfig.ini");

		nMaxTime = 1;
		memcpy(nChip, nTmpChip, sizeof(nChip));
		memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));
		memcpy(szCfgFileName, szTmpCfgFileName, sizeof(szCfgFileName));
	}
};

//��¼��Ϣ
struct tagServerGameRecord
{
	BYTE							cbAnimal;						   //���ж���
};

//��ע��Ϣ
//����������ṹ

#define SUB_S_GAME_FREE				299									//��Ϸ����
#define SUB_S_GAME_START			300									//��Ϸ��ʼ
#define SUB_S_PLACE_JETTON			301									//�û���ע
#define SUB_S_GAME_END				302									//��Ϸ����
#define SUB_S_APPLY_BANKER			303									//����ׯ��
#define SUB_S_CHANGE_BANKER			304									//�л�ׯ��
#define SUB_S_CHANGE_USER_SCORE		305									//���»���
#define SUB_S_SEND_RECORD			306									//��Ϸ��¼
#define SUB_S_PLACE_JETTON_FAIL		307									//��עʧ��
#define SUB_S_CANCEL_BANKER			308									//ȡ������
#define	SUB_S_CEAN_JETTON			309									//�����ע
#define SUB_S_CONTINU_JETTON		310									//������ע

#define SUB_S_ADMIN_COMMDN_EXT		318									//ϵͳ����
#define SUB_S_UPDATE_STORAGE_EXT    319									//���¿��
#define SUB_S_ONLINE_PLAYER         214    								//�����û�
#define SUB_S_ONLINE_PLAYER_ERROR	215									//�����û���ȡʧ��

#define SUB_S_ANDROID_CONFIG		216									//����������

#define SUB_S_OTHER_JETTON			2021								//���������ע

#define SUB_S_RESPONSE_CONTROL		2030								//������Ϣ��Ӧ

//���˿�����Ϣ����
struct CMD_S_ControlInfo
{
	BYTE							cbJuControl;						//�Ƿ���оֿ�
	BYTE							cbJuControlTimes;					//�ֿش���
	BYTE							cbJuControlArea;					//���ֿ�����һ��ʤ�� 0�����ƣ�1����Ӯ��2������

	SCORE							lAreaJettonScore[AREA_COUNT];			//������ע
	SCORE							lUserJettonScore[GAME_PLAYER][AREA_COUNT];	//������ע
	TCHAR							szNickName[GAME_PLAYER][32];		//����ǳ�
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

//����ׯ��
struct CMD_S_ApplyBanker
{
	BYTE							cbResult;							//���

	int								dwGameID;							//�������
	TCHAR							szNickName[32];						//��������ǳ�
	int								nApplyCount;						//��ׯ�б��С
};

//ȡ������
struct CMD_S_CancelBanker
{
	int								dwGameID;							//ȡ�����
};

//�л�ׯ��
struct CMD_S_ChangeBanker
{
	int								dwGameID;							//��ׯ���
	SCORE							lBankerScore;						//ׯ�ҽ��
	TCHAR							szNickName[32];						//��������ǳ�
	BYTE							cbBankerHeadInfo;					//ͷ��ID
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	SCORE							lChips[5];							//����
	int								nMultiple[AREA_COUNT];				//����

	SCORE							lApplyBankerCondition;				//��������
	int								wBankerUserGameID;					//��ǰׯ��
	SCORE							lBankerScore;						//ׯ�ҷ���
	TCHAR							szBankerNick[32];					//ׯ���ǳ�
	BYTE							cbBankerHeadInfo;					//ͷ��ID
	int								nApplyCounts;						//��ׯ�б�����

	BYTE							cbTimeLeave;						//ʣ��ʱ��

	int								nFreeTimes;							//����ʱ��
	int								nPlayTimes;							//��עʱ��
	int								nEndTimes;							//����ʱ��
	int								nAddTimes;							//����ʱ��

	SCORE							lMinXianHong;
	SCORE							lMaxXianHong;

	SCORE							lUserRestScore;						//�û�ʣ����

	BYTE							cbWinMen[20];		//��Ϸ��¼

	SCORE							lPlayerJettonScore[AREA_COUNT];		//����µ�ע
	SCORE							lAllJettonScore[AREA_COUNT];		//��������µ�ע
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	SCORE							lChips[5];							//����
	int								nMultiple[AREA_COUNT];				//����

	SCORE							lApplyBankerCondition;				//��������
	int								wBankerUserGameID;					//��ǰׯ��
	SCORE							lBankerScore;						//ׯ�ҷ���
	TCHAR							szBankerNick[32];					//ׯ���ǳ�
	BYTE							cbBankerHeadInfo;					//ͷ��ID
	int								nApplyCounts;						//��ׯ�б�����

	BYTE							cbTimeLeave;						//ʣ��ʱ��

	int								nFreeTimes;							//����ʱ��
	int								nPlayTimes;							//��עʱ��
	int								nEndTimes;							//����ʱ��
	int								nAddTimes;							//����ʱ��

	SCORE							lMinXianHong;
	SCORE							lMaxXianHong;

	SCORE							lUserRestScore;						//�û�ʣ����

	BYTE							cbWinMen[20];					//��Ϸ��¼

	SCORE							lPlayerJettonScore[AREA_COUNT];		//����µ�ע
	SCORE							lAllJettonScore[AREA_COUNT];		//��������µ�ע
};



//�û���ע
struct CMD_S_PlaceJetton
{
	BYTE							cbResult;							//���	1-6

	BYTE							cbJettonArea;						//��������	
	SCORE							lJettonScore;						//��ע��Ŀ
	int								wUserGameID;						//�û�λ��

	SCORE							lUserRestScore;						//���ʣ����

	SCORE							lPlayerJettonScore[AREA_COUNT];		//����µ�ע
};

//��Ϸ����
struct CMD_S_GameEnd
{
	SCORE							lAllAreaScore[AREA_COUNT];			//�ܵ�������ע
	SCORE							lUserWinScore;						//������Ӯ
	SCORE							lBankerWinScore;					//ׯ����Ӯ
	SCORE							lBankerRestScore;					//ׯ��ʣ����
	SCORE							lPlayerRestScore;					//���ʣ����					
	BYTE							cbTarget;							//�������
	BYTE							cbTurnTarget;						//Ŀ������
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
};

//����б�������
struct CMD_S_UserListInfo
{
	WORD							wWinNum;							//��ʤ����
	SCORE							lAllBet;							//��ע����
	SCORE							lUserScore;							//�û�����
	TCHAR							szNickName[32];						//�û��ǳ�
	BYTE							wFaceID;							//���ͷ��
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_PLACE_JETTON			201									//�û���ע
#define SUB_C_CLEAN_JETTON			205									//��������
#define SUB_C_CONTINUE_JETTON		206									//������ע
#define SUB_C_ONLINE_PLAYER			103									//��ȡ����б�
#define SUB_C_APPLY_BANKER			12									//����ׯ��
#define SUB_C_CANCEL_BANKER			13									//ȡ������

#define SUB_C_REQUEST_CONTROL		110									//������Ϣ����
#define SUB_C_CONTROL				111									//���ֿ�����Ӯ
#define SUB_C_CANCEL_CONTROL		112									//ȡ�����ֿ���

//�û���ע
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonArea;						//��������
	SCORE							lJettonScore;						//��ע��Ŀ
};

struct CMD_C_CleanMeJetto
{
	WORD							wChairID;							//�û�λ��
};

struct CMD_C_ContinueJetton
{
	WORD					wChairID;							//�û�λ��
	SCORE					lLastAllJettonPlace[AREA_COUNT + 1];	//ȫ������
	SCORE					lLastAllJettonArea[AREA_COUNT + 1];	//�������

	BYTE					cbJettonArea;						//��������
	SCORE					lJettonScore;						//��ע��Ŀ
	BYTE					cbAndroid;							//������
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
