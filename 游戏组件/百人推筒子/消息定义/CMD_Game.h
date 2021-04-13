#ifndef CMD_TUITONGZI_HEAD_FILE
#define CMD_TUITONGZI_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��
#pragma pack(push)
#pragma pack(1)

#define KIND_ID						320									//��Ϸ I D
#define GAME_PLAYER					100									//��Ϸ����
#define GAME_NAME					TEXT("���˶��˸�")					//��Ϸ����

//�汾��Ϣ
#define VERSION_SERVER			    PROCESS_VERSION(7,0,1)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//����汾
//״̬����
#define GAME_SCENE_FREE				GAME_STATUS_FREE					//�ȴ���ʼ
#define	GS_PLACE_JETTON				GAME_STATUS_PLAY					//��ע״̬
#define	GS_GAME_END					GAME_STATUS_PLAY+1					//����״̬
#define	GS_MOVECARD_END				GAME_STATUS_PLAY+2					//����״̬

//��������
#define ID_SHUN_MEN					1									//˳��
#define ID_TIAN_MEN					2									//��
#define ID_DI_MEN					3									//����

#define AREA_MAX                    4                                   //�������

//�������
#define BANKER_INDEX				0									//ׯ������
#define SHANG_MEN_INDEX				1									//��������
#define TIAN_MEN_INDEX				2									//��������
#define XIA_MEN_INDEX				3									//��������

#define AREA_COUNT					3									//������Ŀ
//���ʶ���
#define RATE_TWO_PAIR				12									//��������

#define SERVER_LEN					32									//���䳤��

#define SEAT_COUNT					6									//ռλ����

#define STORAGE_COUNT				5									//����������

#define MAX_SEAT_COUNT				6									//���ռλ����

#define USER_LIST_COUNT				20									//�б������Ӧ��������

//��������Ϣ
struct tagRobotInfo
{
	int nChip[6];														//���붨��
	int nAreaChance[AREA_COUNT];										//������
	TCHAR szCfgFileName[MAX_PATH];										//�����ļ�
	int	nMaxTime;														//�������

	tagRobotInfo()
	{
		int nTmpChip[6] = {1, 5, 10, 50, 100, 500};
		int nTmpAreaChance[AREA_COUNT] = {1, 1, 1};
		TCHAR szTmpCfgFileName[MAX_PATH] = _T("28GangBattleConfig.ini");

		nMaxTime = 1;
		memcpy(nChip, nTmpChip, sizeof(nChip));
		memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));
		memcpy(szCfgFileName, szTmpCfgFileName, sizeof(szCfgFileName));
	}
};

//��¼��Ϣ
struct tagServerGameRecord
{
	BYTE							bWinShangMen;						//˳��ʤ��
	BYTE							bWinTianMen;						//����ʤ��
	BYTE							bWinXiaMen;							//����ʤ��
};


#ifndef _UNICODE
#define myprintf	myprintf
#define mystrcpy	strcpy
#define mystrlen	strlen
#define myscanf		_snscanf
#define	myLPSTR		LPCSTR
#define myatoi      atoi
#define myatoi64    _atoi64
#else
#define myprintf	swprintf
#define mystrcpy	wcscpy
#define mystrlen	wcslen
#define myscanf		_snwscanf
#define	myLPSTR		LPWSTR
#define myatoi      _wtoi
#define myatoi64	_wtoi64
#endif


//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_FREE				99									//��Ϸ����
#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_PLACE_JETTON			101									//�û���ע
#define SUB_S_GAME_END				102									//��Ϸ����
#define SUB_S_APPLY_BANKER			103									//����ׯ��
#define SUB_S_CHANGE_BANKER			104									//�л�ׯ��
#define SUB_S_SEND_RECORD			106									//��Ϸ��¼
#define SUB_S_PLACE_JETTON_FAIL		107									//��עʧ��
#define SUB_S_CANCEL_BANKER			108									//ȡ������

#define SUB_ANDROID_INIT			115									//��������Ϣ

#define SUB_S_ONLINE_PLAYER			2014								//�����û�

#define SUB_S_OTHER_JETTON			2021								//���������ע
#define SUB_S_SEAT_JETTON			2022								//ռλ�����ע

#define SUB_S_RESPONSE_CONTROL		2030								//������Ϣ��Ӧ

//���˿�����Ϣ����
struct CMD_S_ControlInfo
{
	BYTE							cbJuControl;							//�Ƿ���оֿ�
	BYTE							cbJuControlTimes;						//�ֿش���
	BYTE							cbJuControlArea[AREA_MAX];				//���ֿ�����һ��ʤ�� 0�����ƣ�1����Ӯ��2������

	SCORE							lAreaJettonScore[AREA_MAX];				//������ע
	SCORE							lUserJettonScore[GAME_PLAYER][AREA_MAX];//������ע
	TCHAR							szNickName[GAME_PLAYER][32];			//����ǳ�
};

//��������Ϣ
struct CMD_S_AndroidInit
{
	//ȫ����Ϣ
	TCHAR							szRoomName[SERVER_LEN];				//���÷���
	TCHAR							szConfigName[MAX_PATH];				//���÷���
};

//ʧ�ܽṹ
struct CMD_S_PlaceJettonFail
{
	WORD							wPlaceUser;							//��ע���
	BYTE							lJettonArea;						//��ע����
	SCORE							lPlaceScore;						//��ǰ��ע
};

//����ׯ��
struct CMD_S_ApplyBanker
{
	WORD							wApplyUser;							//�������
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
	SCORE							lBankerScore;						//ׯ�ҽ��
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//�����Ϣ
	SCORE							lUserMaxScore;						//��ҽ��

	WORD							wSeatUser[MAX_SEAT_COUNT];	//6��������ҵ����Ӻ�

	//ׯ����Ϣ
	WORD							wBankerUser;						//��ǰׯ��
	WORD							cbBankerTime;						//ׯ�Ҿ���
	SCORE							lBankerWinScore;					//ׯ�ҳɼ�
	SCORE							lBankerScore;						//ׯ�ҷ���

	BYTE							cbTableHavaSendCardArray[4][10];	//�����˿�
	BYTE							cbTableHavaSendCount[4];			//�����˿�
	BYTE							cbLeftCardCount;					//�˿���Ŀ

	//������Ϣ
	SCORE							lApplyBankerCondition;				//��������
	SCORE							lAreaLimitScore;					//��������

	//������Ϣ
	TCHAR							szGameRoomName[SERVER_LEN];			//��������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//ȫ����ע
	SCORE							lAllJettonScore[AREA_MAX];			//ȫ����ע

	//�����ע
	SCORE							lUserJettonScore[AREA_MAX];			//������ע

	WORD							wSeatUser[MAX_SEAT_COUNT];	//6��������ҵ����Ӻ�
	SCORE							lSeatUserWinScore[MAX_SEAT_COUNT];	//���µ������Ӯ
	SCORE							lSeatUserAreaScore[MAX_SEAT_COUNT][AREA_MAX];	//6��������ҵ�������ע��Ϣ

	BYTE							cbTableHavaSendCardArray[4][10];	//�����˿�
	BYTE							cbTableHavaSendCount[4];			//�����˿�
	BYTE							cbLeftCardCount;					//�˿���Ŀ

	//��һ���
	SCORE							lUserMaxScore;						//�����ע							

	//������Ϣ
	SCORE							lApplyBankerCondition;				//��������
	SCORE							lAreaLimitScore;					//��������

	//�˿���Ϣ
	BYTE							cbTableCardArray[4][2];				//�����˿�

	BYTE							cbCardType[4];						//�˿�����

	BYTE							cbBankerTong;						//ׯ��ͨ��ͨӮ

	BYTE							cbWinArea[4];						//������Ӯ

	//ׯ����Ϣ
	WORD							wBankerUser;						//��ǰׯ��
	WORD							cbBankerTime;						//ׯ�Ҿ���
	SCORE							lBankerWinScore;					//ׯ��Ӯ��
	SCORE							lBankerScore;						//ׯ�ҷ���

	//������Ϣ
	SCORE							lEndBankerScore;					//ׯ�ҳɼ�
	SCORE							lEndUserScore;						//��ҳɼ�
	SCORE							lEndUserReturnScore;				//���ػ���
	SCORE							lEndRevenue;						//��Ϸ˰��

	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��
	BYTE							cbGameStatus;						//��Ϸ״̬
	//������Ϣ
	TCHAR							szGameRoomName[SERVER_LEN];			//��������
};

//��Ϸ����
struct CMD_S_GameFree
{
	BYTE							cbTimeLeave;						//ʣ��ʱ��
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD							wBankerUser;						//ׯ��λ��
	SCORE							lBankerScore;						//ׯ�ҽ��
	SCORE							lUserMaxScore;						//�ҵĽ��
	BYTE							cbTableHavaSendCardArray[4][10];	//�����˿�
	BYTE							cbTableHavaSendCount[4];			//�����˿�
	BYTE							cbLeftCardCount;					//�˿���Ŀ

	BYTE							cbTimeLeave;						//ʣ��ʱ��	
	bool							bContiueCard;						//��������

	WORD							wSeatUser[MAX_SEAT_COUNT];	//6��������ҵ����Ӻ�
};

//�û���ע
struct CMD_S_PlaceJetton
{
	WORD							wChairID;							//�û�λ��
	BYTE							cbJettonArea;						//��������
	SCORE							lJettonScore;						//��ע��Ŀ
	bool							bIsAndroid;							//�Ƿ������

	SCORE							lUserRestScore;						//�û�ʣ����
};

//��Ϸ����
struct CMD_S_GameEnd
{
	//�¾���Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	BYTE							cbBankerTong;						//ׯ��ͨ��ͨӮ

	//�˿���Ϣ
	BYTE							cbTableCardArray[4][2];				//�����˿�
	BYTE							cbTableHavaSendCardArray[4][10];	//�����˿�
	BYTE							cbTableHavaSendCount[4];			//�����˿�
	BYTE							cbLeftCardCount;					//�˿���Ŀ

	BYTE							cbCardType[4];						//�˿�����

	BYTE							cbWinArea[4];						//������Ӯ
 
	//ׯ����Ϣ
	SCORE							lBankerScore;						//ׯ�ҳɼ�
	SCORE							lBankerTotallScore;					//ׯ�ҳɼ�
	SCORE							lBankerHaveScore;					//ׯ��ӵ�н��
	INT								nBankerTime;						//��ׯ����

	//��ҳɼ�
	SCORE							lUserScore;							//��ҳɼ�
	SCORE							lUserReturnScore;					//���ػ���

	//ȫ����Ϣ
	SCORE							lRevenue;							//��Ϸ˰��

	SCORE							lSeatScore[SEAT_COUNT];				//ռλ��ҳɼ�
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
	BYTE							cbJuControlArea[AREA_MAX];			//���ֿ�������ʤ�� 0�����ƣ�1����Ӯ��2������
};
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
