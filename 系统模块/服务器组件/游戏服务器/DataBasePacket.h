#ifndef DATA_BASE_PACKET_HEAD_FILE
#define DATA_BASE_PACKET_HEAD_FILE

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//�������ݰ�

//�û��¼�
#define	DBR_GR_LOGON_USERID			100									//I D ��¼
#define	DBR_GR_LOGON_MOBILE			101									//�ֻ���¼
#define DBR_GR_LOGON_MATCH			103									//������¼

//ϵͳ�¼�
#define DBR_GR_WRITE_GAME_SCORE		200									//��Ϸ����
#define DBR_GR_LEAVE_GAME_SERVER	201									//�뿪����
#define DBR_GR_GAME_SCORE_RECORD	202									//���ּ�¼
#define DBR_GR_MANAGE_USER_RIGHT	203									//Ȩ�޹���
#define DBR_GR_LOAD_SYSTEM_MESSAGE	204									//ϵͳ��Ϣ
#define DBR_GR_LOAD_SENSITIVE_WORDS	205									//���д�	
#define DBR_GR_UNLOCK_ANDROID_USER	206									//����������
#define DBR_GR_UNLOCK_USER			207									//�������

//�����¼�
#define DBR_GR_LOAD_PARAMETER		300									//���ز���
#define DBR_GR_LOAD_ANDROID_PARAMETER 302								//���ز���
#define DBR_GR_LOAD_ANDROID_USER	303									//���ػ���

#define DBR_GR_LOAD_BASEENSURE		308 								//���صͱ� 

//�����¼�
#define DBR_GR_USER_ENABLE_INSURE	400									//��ͨ����
#define DBR_GR_USER_SAVE_SCORE		401									//������Ϸ��
#define DBR_GR_USER_TAKE_SCORE		402									//��ȡ��Ϸ��
#define DBR_GR_USER_TRANSFER_SCORE	403									//ת����Ϸ��
#define DBR_GR_QUERY_INSURE_INFO	404									//��ѯ����
#define DBR_GR_QUERY_TRANSFER_USER_INFO	    405							//��ѯ�û�

//��Ϸ�¼�
#define DBR_GR_GAME_FRAME_REQUEST	502									//��Ϸ����

//�����¼�
#define DBR_GR_MATCH_SIGNUP_START	602									//��ʼ����
#define DBR_GR_MATCH_QUERY_QUALIFY	603									//��ѯ�ʸ�
#define DBR_GR_MATCH_BUY_SAFECARD	604									//�����տ�	
#define DBR_GR_MATCH_START			605									//������ʼ
#define DBR_GR_MATCH_OVER			606									//��������
#define DBR_GR_MATCH_CANCEL			607									//����ȡ��
#define DBR_GR_MATCH_REWARD			608									//��������
#define DBR_GR_MATCH_ELIMINATE      609									//������̭
#define DBR_GR_MATCH_QUERY_REVIVE	610									//��ѯ����
#define DBR_GR_MATCH_USER_REVIVE	611									//�û�����
#define DBR_GR_MATCH_RECORD_GRADES	612									//��¼�ɼ�

//�ͱ��¼�
#define DBR_GR_BASEENSURE_TAKE		1100								//��ȡ�ͱ�

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//�����Ϣ

//�߼��¼�
#define DBO_GR_LOGON_SUCCESS		100									//��¼�ɹ�
#define DBO_GR_LOGON_FAILURE		101									//��¼ʧ��

//�����¼�
#define DBO_GR_GAME_PARAMETER		200									//������Ϣ
#define DBO_GR_GAME_ANDROID_PARAMETER 202								//������Ϣ
#define DBO_GR_GAME_ANDROID_INFO	203									//������Ϣ
#define DBO_GR_GAME_UNLOCK_ANDROID	204									//��������

#define DBO_GR_BASEENSURE_PARAMETER	209									//�ͱ���Ϣ

//��������
#define DBO_GR_USER_INSURE_INFO		300									//��������
#define DBO_GR_USER_INSURE_SUCCESS	301									//���гɹ�
#define DBO_GR_USER_INSURE_FAILURE	302									//����ʧ��
#define DBO_GR_USER_INSURE_USER_INFO   303								//�û�����
#define DBO_GR_USER_INSURE_ENABLE_RESULT 304							//��ͨ���

//��Ϸ�¼�
#define DBO_GR_GAME_FRAME_RESULT	402									//��Ϸ���

//�����¼�
#define DBO_GR_MATCH_EVENT_START	500									//������ʶ
#define DBO_GR_MATCH_UNSIGNUP_RESULT 502								//�������
#define DBO_GR_MATCH_QUERY_RESULT	503									//��ѯ���
#define DBO_GR_MATCH_BUY_RESULT		504									//������
#define DBO_GR_MATCH_REVIVE_INFO	505									//������Ϣ
#define DBO_GR_MATCH_REVIVE_RESULT	506									//������		
#define DBO_GR_MATCH_RANK_LIST		507									//��������
#define DBO_GR_MATCH_REWARD_RESULT  508									//�������
#define DBO_GR_MATCH_CANCEL_RESULT	509									//����ȡ��
#define DBO_GR_MATCH_EVENT_END		599									//������ʶ

//ϵͳ�¼�
#define DBO_GR_SYSTEM_MESSAGE_RESULT	600								//��Ϣ���
#define DBO_GR_SYSTEM_MESSAGE_FINISH	601								//��Ϣ���	
#define DBO_GR_SENSITIVE_WORDS		    602								//���д�


//�ȼ�����

#define DBO_GR_UPDATE_CONTROLINFO	900									//���¿���



#define DBO_GR_BASEENSURE_RESULT	1100								//��ȡ���


/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//ID ��¼
struct DBR_GR_LogonUserID
{		
	//��¼��Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwMatchID;							//������ʶ
	LONGLONG						lMatchNo;							//�������
	BYTE							cbMatchType;						//��������
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//ID ��¼
struct DBR_GR_LogonMobile
{		
	//��¼��Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	BYTE                            cbDeviceType;                       //�豸����
	WORD                            wBehaviorFlags;                     //��Ϊ��ʶ
	WORD                            wPageTableCount;                    //��ҳ����

	//������Ϣ
	DWORD							dwMatchID;							//������ʶ
	LONGLONG						lMatchNo;							//�������
	BYTE							cbMatchType;						//��������
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//�ʺŵ�¼
struct DBR_GR_LogonAccounts
{
	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//��Ϸ����
struct DBR_GR_WriteGameScore
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ

	SCORE							lStorageCurrent;					//��浱ǰֵ

	//�û���Ϣ
	DWORD							dwDBQuestID;						//�����ʶ
	DWORD							dwInoutIndex;						//��������

	//�ɼ�����
	tagVariationInfo				VariationInfo;						//��ȡ��Ϣ

	//������Ϣ
	DWORD							dwMatchID;							//������ʶ
	LONGLONG						lMatchNo;							//��������
};

//�뿪����
struct DBR_GR_LeaveGameServer
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwInoutIndex;						//��¼����
	DWORD							dwLeaveReason;						//�뿪ԭ��
	DWORD							dwOnLineTimeCount;					//����ʱ��

	//�ɼ�����
	tagVariationInfo				RecordInfo;							//��¼��Ϣ
	tagVariationInfo				VariationInfo;						//��ȡ��Ϣ

	//ϵͳ��Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};
//���¿���
struct DBO_GR_UpdateControlInfo
{
	DWORD							dwUserID;							//�û���ʶ
	int								nWinRate;							//���з����ʤ����
	int								nRoomWinRate;						//�÷����ʤ����
	BYTE							cbNullity;							//�Ƿ񶳽�
	DWORD							dwTailGameID;						//����ID
	SCORE							lTotalScore;						//�������Ӯ
	SCORE							lTdTotalScore;						//��ҵ�������Ӯ
	bool							bRegister;							//�Ƿ��¼
};
//��Ϸ��¼
struct DBR_GR_GameScoreRecord
{
	//������Ϣ
	WORD							wTableID;							//���Ӻ���
	WORD							wUserCount;							//�û���Ŀ
	WORD							wAndroidCount;						//������Ŀ

	//���˰��
	SCORE							lWasteCount;						//�����Ŀ
	SCORE							lRevenueCount;						//˰����Ŀ

	//ͳ����Ϣ
	DWORD							dwUserMemal;						//������Ŀ
	DWORD							dwPlayTimeCount;					//��Ϸʱ��

	//ʱ����Ϣ
	SYSTEMTIME						SystemTimeStart;					//��ʼʱ��
	SYSTEMTIME						SystemTimeConclude;					//����ʱ��

	//���ּ�¼
	WORD							wRecordCount;						//��¼��Ŀ
	tagGameScoreRecord				GameScoreRecord[MAX_CHAIR];			//��Ϸ��¼
};

//��ͨ����
struct DBR_GR_UserEnableInsure
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwUserID;							//�û� I D	
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szLogonPass[LEN_PASSWORD];			//��¼����
	TCHAR							szInsurePass[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//������Ϸ��
struct DBR_GR_UserSaveScore
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwUserID;							//�û� I D
	SCORE							lSaveScore;							//������Ϸ��
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ȡ����Ϸ��
struct DBR_GR_UserTakeScore
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwUserID;							//�û� I D
	SCORE							lTakeScore;							//��ȡ��Ϸ��
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ת����Ϸ��
struct DBR_GR_UserTransferScore
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ	
	SCORE							lTransferScore;						//ת����Ϸ��
	TCHAR							szAccounts[LEN_NICKNAME];			//Ŀ���û�
	TCHAR							szPassword[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szTransRemark[LEN_TRANS_REMARK];	//ת�ʱ�ע
};

//���ػ���
struct DBR_GR_LoadAndroidUser
{
	DWORD							dwBatchID;							//���α�ʶ
	DWORD							dwAndroidCount;						//������Ŀ
	DWORD							AndroidCountMember0;				//��ͨ��Ա	
	DWORD							AndroidCountMember1;				//һ����Ա
	DWORD							AndroidCountMember2;				//������Ա
	DWORD							AndroidCountMember3;				//������Ա
	DWORD							AndroidCountMember4;				//�ļ���Ա
	DWORD							AndroidCountMember5;				//�弶��Ա
};

//��ѯ����
struct DBR_GR_QueryInsureInfo
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//��������
};

//��ѯ�û�
struct DBR_GR_QueryTransferUserInfo
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	BYTE                            cbByNickName;                       //�ǳ�����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//Ŀ���û�
	DWORD							dwUserID;							//�û� I D
};

//��ѯ�ʸ�
struct DBR_GR_QueryMatchQualify
{	
	DWORD							dwUserID;							//�û�ID
	DWORD							dwMatchID;							//����ID
	SCORE							lMatchNo;							//��������
};

//������
struct DBR_GR_MatchBuySafeCard
{
	DWORD							dwUserID;							//�û�ID
	DWORD							dwMatchID;							//����ID
	SCORE							lMatchNo;							//��������	
	SCORE							lSafeCardFee;						//���շ���		
};

//��������
struct DBR_GR_MatchSignup
{
	//��������
	DWORD							dwUserID;							//�û�ID	
	SCORE							lInitScore;							//��ʼ����
	BYTE							cbSignupMode;						//������ʽ
	WORD							wMaxSignupUser;						//�������

	//ϵͳ��Ϣ
	DWORD							dwInoutIndex;						//�����ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ
	DWORD							dwMatchID;							//����ID
	SCORE							lMatchNo;							//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ȡ������
struct DBR_GR_MatchUnSignup
{
	//��������
	DWORD							dwReason;							//ȡ��ԭ��
	DWORD							dwUserID;							//�û�ID

	//ϵͳ��Ϣ
	DWORD							dwInoutIndex;						//�����ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ
	DWORD							dwMatchID;							//����ID
	SCORE							lMatchNo;							//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//������ʼ
struct DBR_GR_MatchStart
{
	WORD							wServerID;							//�����ʶ
	BYTE							cbMatchType;						//��������
	DWORD							dwMatchID;							//������ʶ
	SCORE							lMatchNo;							//��������	
};

//������ʼ
struct DBR_GR_SignupStart
{	
	WORD							wServerID;							//�����ʶ
	BYTE							cbMatchType;						//��������
	DWORD							dwMatchID;							//������ʶ
	SCORE							lMatchNo;							//��������	
	BYTE							cbSignupMode;						//������ʽ
};

//��������
struct DBR_GR_MatchOver
{	
	WORD							wServerID;							//�����ʶ
	BYTE							cbMatchType;						//��������
	BOOL							bMatchFinish;						//�������
	WORD							wPlayCount;							//��Ϸ����
	DWORD							dwMatchID;							//������ʶ
	SCORE							lMatchNo;							//��������	
	SYSTEMTIME						MatchStartTime;						//��ʼʱ��
	SYSTEMTIME						MatchEndTime;						//����ʱ��
};

//����ȡ��
struct DBR_GR_MatchCancel
{
	BOOL							bMatchFinish;						//�������
	WORD							wServerID;							//�����ʶ
	DWORD							dwMatchID;							//������ʶ
	SCORE							lMatchNo;							//��������
	SCORE							lSafeCardFee;						//���շ���
};

//������̭
struct DBR_GR_MatchEliminate
{
	DWORD							dwUserID;							//�û���ʶ
	WORD							wServerID;							//�����ʶ
	BYTE							cbMatchType;						//��������
	DWORD							dwMatchID;							//������ʶ
	SCORE							lMatchNo;							//��������	
};

//��ѯ����
struct DBR_GR_MatchQueryRevive
{
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwMatchID;							//������ʶ
	SCORE							lMatchNo;							//��������		
};

//�û�����
struct DBR_GR_MatchUserRevive
{
	//������Ϣ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwMatchID;							//������ʶ
	SCORE							lMatchNo;							//��������

	//��������
	SCORE							lInitScore;							//��ʼ����
	SCORE							lCullScore;							//��̭����			

	//��������
	BYTE							cbReviveTimes;						//�������
	SCORE							lReviveFee;							//�������	
};

//��������
struct DBR_GR_MatchReward
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lRewardGold;						//�������
	SCORE							lRewardIngot;						//����Ԫ��
	DWORD							dwRewardExperience;					//��������
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//��¼�ɼ�
struct DBR_GR_MatchRecordGrades
{
	//������Ϣ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwMatchID;							//������ʶ
	WORD							wServerID;							//�����ʶ
	SCORE							lMatchNo;							//��������
	SCORE							lInitScore;							//��ʼ����
};

//��ȡ�ͱ�
struct DBR_GR_TakeBaseEnsure
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//��¼�ɹ�
struct DBO_GR_LogonSuccess
{
	//��������
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwGroupID;							//���� I D
	DWORD							dwCustomID;							//�Զ�ͷ��
	TCHAR							szNickName[LEN_NICKNAME];			//�ʺ��ǳ�
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������

	//�û�����
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	BYTE							cbMasterOrder;						//����ȼ�
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��

	//��������
	TCHAR							szQQ[LEN_QQ];						//�û�QQ
	TCHAR							szEMail[LEN_EMAIL];					//�û�Eamil
	TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ֻ�����	
	TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//��ϵ��ַ
	TCHAR							szPostalCode[LEN_POSTALCODE];		//��������

	//������Ϣ
	SCORE							lScore;								//�û�����
	SCORE							lIngot;								//�û�Ԫ��
	SCORE							lGrade;								//�û��ɼ�
	SCORE							lInsure;							//�û�����	
	DOUBLE							dBeans;								//�û���Ϸ��

	//��Ϸ��Ϣ
	DWORD							dwWinCount;							//ʤ������
	DWORD							dwLostCount;						//ʧ������
	DWORD							dwDrawCount;						//�;�����
	DWORD							dwFleeCount;						//��������
	DWORD							dwExperience;						//�û�����
	LONG							lLoveLiness;						//�û�����
	SCORE							lIntegralCount;						//��������(��ǰ����)

	//������Ϣ
	DWORD							dwAgentID;							//���� I D

	//�û�Ȩ��
	DWORD							dwUserRight;						//�û�Ȩ��
	DWORD							dwMasterRight;						//����Ȩ��

	//��������
	DWORD							dwInoutIndex;						//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ

	//�ֻ�����
	BYTE                            cbDeviceType;                       //�豸����
	WORD                            wBehaviorFlags;                     //��Ϊ��ʶ
	WORD                            wPageTableCount;                    //��ҳ����

	//������Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��¼ʧ��
struct DBO_GR_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ

	DWORD							dwLockKindID;						//��ס�������ϷKindID
	DWORD							dwLockServerID;						//��ס����ķ���ServerID
};

//������Ϣ
struct DBO_GR_GameParameter
{
	//������Ϣ
	DWORD							dwMedalRate;						//���ƻ���
	DWORD							dwRevenueRate;						//����˰��
	DWORD							dwExchangeRate;						//�һ�����
	DWORD							dwPresentExchangeRate;				//������Ϸ�Ҷһ���
	DWORD							dwRateGold;							//��Ϸ����Ϸ�Ҷһ���
	SCORE							lHallNoticeThreshold;				//����֪ͨ�����ֵ

	//���齱��
	DWORD							dwWinExperience;					//Ӯ�־���

	//�汾��Ϣ
	DWORD							dwClientVersion;					//�ͻ��汾
	DWORD							dwServerVersion;					//����汾

	SCORE							lEducateGrantScore;					//��ϰ����
};

//�б���Ϣ
struct DBO_GR_GameColumnInfo
{
	LONG							lResultCode;						//�������
	BYTE							cbColumnCount;						//�б���Ŀ
	tagColumnItem					ColumnItemInfo[MAX_COLUMN];			//�б���Ϣ
};

//������Ϣ
struct DBO_GR_GameAndroidParameter
{
	LONG							lResultCode;						//�������
	WORD							wParameterCount;					//������Ŀ
	tagAndroidParameter				AndroidParameter[MAX_BATCH];		//������Ϣ
};

//������Ϣ
struct DBO_GR_GameAndroidInfo
{
	LONG							lResultCode;						//�������
	DWORD							dwBatchID;							//���α�ʶ
	WORD							wAndroidCount;						//�û���Ŀ
	tagAndroidAccountsInfo			AndroidAccountsInfo[MAX_ANDROID];	//�����ʺ�
};
//////////////////////////////////////////////////////////////////////////

//�ͱ�����
struct DBO_GR_BaseEnsureParameter
{
	SCORE							lScoreCondition;					//��Ϸ������
	SCORE							lScoreAmount;						//��Ϸ������
	BYTE							cbTakeTimes;						//��ȡ����	
};

//�ͱ����
struct DBO_GR_BaseEnsureResult
{
	bool							bSuccessed;							//�ɹ���ʶ
	SCORE							lGameScore;							//��ǰ��Ϸ��
	TCHAR							szNotifyContent[128];				//��ʾ����
};

//��������
struct DBO_GR_UserInsureInfo
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	BYTE							cbEnjoinTransfer;					//ת�ʿ���
	WORD							wRevenueTake;						//˰�ձ���
	WORD							wRevenueTransfer;					//˰�ձ���
	WORD							wRevenueTransferMember;				//˰�ձ���
	WORD							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û���Ϸ��
	SCORE							lUserInsure;						//������Ϸ��
	SCORE							lTransferPrerequisite;				//ת������
};

//���гɹ�
struct DBO_GR_UserInsureSuccess
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwUserID;							//�û� I D
	SCORE							lSourceScore;						//ԭ����Ϸ��
	SCORE							lSourceInsure;						//ԭ����Ϸ��
	SCORE							lInsureRevenue;						//����˰��
	SCORE							lFrozenedScore;						//�������
	SCORE							lVariationScore;					//��Ϸ�ұ仯
	SCORE							lVariationInsure;					//���б仯
	TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct DBO_GR_UserInsureFailure
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	LONG							lResultCode;						//��������
	SCORE							lFrozenedScore;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�û�����
struct DBO_GR_UserTransferUserInfo
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwGameID;							//�û� I D
	TCHAR							szAccounts[LEN_ACCOUNTS];			//�ʺ��ǳ�
};

//��ͨ���
struct DBO_GR_UserInsureEnableResult
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	BYTE							cbInsureEnabled;					//ʹ�ܱ�ʶ
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�������
struct DBO_GR_MatchSingupResult
{
	bool							bResultCode;						//�������
	BYTE							cbSignupMode;						//������ʽ
	DWORD							dwReason;							//ȡ��ԭ��
	DWORD							dwUserID;							//�û���ʶ
	SCORE							lMatchNo;							//��������
	SCORE							lCurrGold;							//��ǰ���
	SCORE							lCurrIngot;							//��ǰԪ��
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��ѯ���
struct DBO_GR_MatchQueryResult
{
	bool							bHoldQualify;						//ӵ���ʸ�
};

//������
struct DBO_GR_MatchBuyResult
{
	bool							bSuccessed;							//�ɹ���ʶ
	SCORE							lCurrGold;							//��ǰ���
	TCHAR							szDescribeString[128];				//������Ϣ
};

//������Ϣ
struct DBO_GR_MatchReviveInfo
{	
	bool							bSuccessed;							//�ɹ���ʶ
	bool							bSafeCardRevive;					//���տ�����
	BYTE							cbReviveTimesed;					//�������
};

//������
struct DBO_GR_MatchReviveResult
{
	bool							bSuccessed;							//�ɹ���ʶ
	SCORE							lCurrGold;							//��ǰ���	
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��������
struct DBO_GR_MatchRankList
{
	BOOL							bMatchFinish;						//�������
	WORD							wUserCount;							//�û���Ŀ
	DWORD							dwMatchID;							//������ʶ
	SCORE							lMatchNo;							//��������
	tagMatchRankInfo				MatchRankInfo[128];					//������Ϣ
};

//�������
struct DBO_GR_MatchRewardResult
{
	bool							bResultCode;						//�������
	DWORD							dwUserID;							//�û���ʶ
	SCORE							lCurrGold;							//��ǰ���
};

//ȡ�����
struct DBO_GR_MatchCancelResult
{
	bool							bSuccessed;							//�ɹ���ʶ
	BOOL							bMatchFinish;						//�������
};

//Ȩ�޹���
struct DBR_GR_ManageUserRight
{
	DWORD							dwUserID;							//Ŀ���û�
	DWORD							dwAddRight;							//���Ȩ��
	DWORD							dwRemoveRight;						//ɾ��Ȩ��
	bool							bGameRight;							//��ϷȨ��
};

//Ȩ�޹���
struct DBR_GR_ManageMatchRight
{	
	DWORD							dwUserID;							//Ŀ���û�
	DWORD							dwAddRight;							//���Ȩ��
	DWORD							dwRemoveRight;						//ɾ��Ȩ��	
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������
};

//ϵͳ��Ϣ
struct DBR_GR_SystemMessage
{
	DWORD                           dwMessageID;                        //��ϢID
	BYTE							cbMessageType;						//��Ϣ����
	BYTE                            cbAllRoom;                          //ȫ�巿��
	DWORD							dwTimeRate;						    //ʱ��Ƶ��
	__time64_t						tStartTime;							//��ʼʱ��
	__time64_t                      tConcludeTime;                      //����ʱ��
	TCHAR							szSystemMessage[LEN_USER_CHAT];		//ϵͳ��Ϣ
};

//����������
struct DBR_GR_UnlockAndroidUser
{
	WORD							wServerID;							//����ID
	WORD							wBatchID;							//����ID
};
//������Ϣ
struct DBR_GR_Deblocking
{
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwServerID;							//���� I D
};
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

#endif