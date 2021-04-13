#ifndef CMD_LONGON_HEAD_FILE
#define CMD_LONGON_HEAD_FILE

#pragma pack(1)

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//��¼����

#define MDM_GP_LOGON				1									//�㳡��¼

//��¼���
#define SUB_GP_LOGON_SUCCESS		100									//��¼�ɹ�
#define SUB_GP_LOGON_FAILURE		101									//��¼ʧ��
#define SUB_GP_LOGON_FINISH			102									//��¼���
#define SUB_GP_VALIDATE_MBCARD      103                                 //��¼ʧ��
#define SUB_GP_VALIDATE_PASSPORT	104									//��¼ʧ��	
#define SUB_GP_MATCH_SIGNUPINFO		106									//������Ϣ
#define SUB_GP_VERIFY_CODE_RESULT	108									//��֤���

//������ʾ
#define SUB_GP_UPDATE_NOTIFY		200									//������ʾ

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//
#define MB_VALIDATE_FLAGS           0x01                                //Ч���ܱ�
#define LOW_VER_VALIDATE_FLAGS      0x02                                //Ч��Ͱ汾

//У������
#define VERIFY_ACCOUNTS				0x01								//У���˺�
#define VERIFY_NICKNAME				0x02								//У���ǳ�

//I D ��¼
struct CMD_GP_LogonGameID
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//��¼��Ϣ
	DWORD							dwGameID;							//��Ϸ I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	BYTE							cbValidateFlags;			        //У���ʶ
};

//�ʺŵ�¼
struct CMD_GP_LogonAccounts
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//��¼��Ϣ
	BYTE							cbValidateFlags;			        //У���ʶ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//���֤��
};

//ע���ʺ�
struct CMD_GP_RegisterAccounts
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����

	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����
	BYTE							cbValidateFlags;			        //У���ʶ
	TCHAR							szAgentID[LEN_ACCOUNTS];			//�����ʶ
	DWORD							dwSpreaderGameID;					//�Ƽ���ʶ
};

//��֤����
struct CMD_GP_VerifyIndividual
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾

	//��֤��Ϣ
	WORD							wVerifyMask;						//��֤����
};

//�ο͵�¼
struct CMD_GP_LogonVisitor
{
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE							cbValidateFlags;			        //У���ʶ

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//��¼�ɹ�
struct CMD_GP_LogonSuccess
{
	//��������
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwGroupID;							//���ű�ʶ
	DWORD							dwCustomID;							//�Զ���ʶ	
	DWORD							dwExperience;						//������ֵ
	SCORE							lLoveLiness;						//�û�����

	//�û��ɼ�
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//�û�����
	SCORE							lUserIngot;							//�û�Ԫ��
	DOUBLE							dUserBeans;							//�û���Ϸ��

	//�û���Ϣ
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMoorMachine;						//��������
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
	TCHAR							szDynamicPass[LEN_PASSWORD];		//��̬����
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������

	//������Ϣ
	BYTE							cbInsureEnabled;					//����ʹ�ܱ�ʶ
	BYTE                            cbShowServerStatus;                 //��ʾ������״̬
	BYTE							cbIsAgent;							//�����ʶ
};

//��¼ʧ��
struct CMD_GP_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��½���
struct CMD_GP_LogonFinish
{
	WORD							wIntermitTime;						//�ж�ʱ��
	WORD							wOnLineCountTime;					//����ʱ��
};

//��¼ʧ��
struct CMD_GP_ValidateMBCard
{
	UINT								uMBCardID;						//��������
};

//��֤���
struct CMD_GP_VerifyIndividualResult
{
	bool							bVerifyPassage;						//��֤ͨ��
	WORD							wVerifyMask;						//��֤����
	TCHAR							szErrorMsg[128];					//������Ϣ
};

//������ʾ
struct CMD_GP_UpdateNotify
{
	BYTE							cbMustUpdate;						//ǿ������
	BYTE							cbAdviceUpdate;						//��������
	DWORD							dwCurrentVersion;					//��ǰ�汾
};

//��֤���
struct CMD_GP_VerifyCodeResult
{
	BYTE								cbResultCode;					//�����ʶ��1�ɹ���0ʧ�ܣ�2�ظ����룩
	TCHAR								szDescString[64];				//����ַ���
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//Я����Ϣ CMD_GP_LogonSuccess

#define DTP_GP_GROUP_INFO			1									//������Ϣ
#define DTP_GP_MEMBER_INFO			2									//��Ա��Ϣ
#define	DTP_GP_UNDER_WRITE			3									//����ǩ��
#define DTP_GP_STATION_URL			4									//��ҳ��Ϣ

//������Ϣ
struct DTP_GP_GroupInfo
{
	DWORD							dwGroupID;							//��������
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������
};

//��Ա��Ϣ
struct DTP_GP_MemberInfo
{
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	SYSTEMTIME						MemberOverDate;						//����ʱ��
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//�б�����

#define MDM_GP_SERVER_LIST			2									//�б���Ϣ

//��ȡ����
#define SUB_GP_GET_LIST				1									//��ȡ�б�
#define SUB_GP_GET_SERVER			2									//��ȡ����
#define SUB_GP_GET_MATCH			3									//��ȡ����
#define SUB_GP_GET_ONLINE			4									//��ȡ����
#define SUB_GP_GET_COLLECTION		5									//��ȡ�ղ�

//�б���Ϣ
#define SUB_GP_LIST_TYPE			100									//�����б�
#define SUB_GP_LIST_KIND			101									//�����б�
#define SUB_GP_LIST_NODE			102									//�ڵ��б�
#define SUB_GP_LIST_PAGE			103									//�����б�
#define SUB_GP_LIST_SERVER			104									//�����б�
#define SUB_GP_LIST_MATCH			105									//�����б�
#define SUB_GP_VIDEO_OPTION			106									//��Ƶ����
#define SUB_GP_AGENT_KIND			107									//�����б�

//�����Ϣ
#define SUB_GP_LIST_FINISH			200									//�������
#define SUB_GP_SERVER_FINISH		201									//�������
#define SUB_GP_MATCH_FINISH			202									//�������

//������Ϣ
#define SUB_GR_KINE_ONLINE			300									//��������
#define SUB_GR_SERVER_ONLINE		301									//��������

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//��ȡ����
struct CMD_GP_GetOnline
{
	WORD							wServerCount;						//������Ŀ
	WORD							wOnLineServerID[MAX_SERVER];		//�����ʶ
};

//��������
struct CMD_GP_KindOnline
{
	WORD							wKindCount;							//������Ŀ
	tagOnLineInfoKind				OnLineInfoKind[MAX_KIND];			//��������
};

//��������
struct CMD_GP_ServerOnline
{
	WORD							wServerCount;						//������Ŀ
	tagOnLineInfoServer				OnLineInfoServer[MAX_SERVER];		//��������
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//��������

#define MDM_GP_USER_SERVICE				3								//�û�����

//�˺ŷ���
#define SUB_GP_MODIFY_INSURE_PASS		102								//�޸�����

//�޸�ͷ��
#define SUB_GP_USER_FACE_INFO			120								//ͷ����Ϣ
#define SUB_GP_SYSTEM_FACE_INFO			122								//ϵͳͷ��

//��������
#define SUB_GP_USER_INDIVIDUAL			140								//��������
#define SUB_GP_MODIFY_INDIVIDUAL		152								//�޸�����
#define SUB_GP_INDIVIDUAL_RESULT		153								//��������

//���з���
#define SUB_GP_USER_ENABLE_INSURE		160								//��ͨ����
#define SUB_GP_USER_SAVE_SCORE			161								//������
#define SUB_GP_USER_TAKE_SCORE			162								//ȡ�����
#define SUB_GP_USER_TRANSFER_SCORE		163								//ת�˲���
#define SUB_GP_USER_INSURE_INFO			164								//��������
#define SUB_GP_QUERY_INSURE_INFO		165								//��ѯ����
#define SUB_GP_USER_INSURE_SUCCESS		166								//���гɹ�
#define SUB_GP_USER_INSURE_FAILURE		167								//����ʧ��
#define SUB_GP_QUERY_USER_INFO_REQUEST	168								//��ѯ�û�
#define SUB_GP_QUERY_USER_INFO_RESULT	169								//�û���Ϣ
#define SUB_GP_USER_INSURE_ENABLE_RESULT 170							//��ͨ���	
#define SUB_GP_EXCHANGE_BANK_SCORE		177								//�һ����н��
#define SUB_GP_QUERY_GAMELOCKINFO_RESULT		178						//��ѯ������

#define SUB_GP_USER_SAVE_RESULT			179								//��Ǯ���
#define SUB_GP_USER_TAKE_RESULT			180								//ȡǮ���

//�ͱ�����
#define SUB_GP_BASEENSURE_LOAD			260								//���صͱ�
#define SUB_GP_BASEENSURE_TAKE			261								//��ȡ�ͱ�
#define SUB_GP_BASEENSURE_PARAMETER		262								//�ͱ�����
#define SUB_GP_BASEENSURE_RESULT		263								//�ͱ����

//��ѯ�ɼ�����Ϸ����
#define SUB_GP_QUERY_GAMEROOM			301								//��ѯ�ɼ�����Ϸ����
#define SUB_GP_QUERY_GAMEROOM_RESULT	302								//��ѯ�ɼ�����Ϸ������

//�齱����
#define SUB_GP_LOTTERY_CONFIG_REQ		360								//��������
#define SUB_GP_LOTTERY_CONFIG			361								//�齱����
#define SUB_GP_LOTTERY_USER_INFO		362								//�齱��Ϣ
#define SUB_GP_LOTTERY_START			363								//��ʼ�齱
#define SUB_GP_LOTTERY_RESULT			364								//�齱���

//�ʼ��ͷ�
#define SUB_GP_QUERY_EMAIL				370								//��ȡ�ʼ�
#define SUB_GP_QUERY_CUSTOMER			371								//��ȡ�ͷ���Ϣ
#define SUB_GP_SUBMIT_CUSTOMER			372								//�ύ�ͷ���Ϣ

#define SUB_GP_EXCHANGESCORE_RESULT		401								//�һ���ҽ��

//�������
#define SUB_GP_OPERATE_SUCCESS			500								//�����ɹ�
#define SUB_GP_OPERATE_FAILURE			501								//����ʧ��

#define SUB_MB_RADIO_MESSAGE			550								//�㲥��Ϣ

#define SUB_MB_CUSTOMER					551								//���Ϳͷ�����
#define SUB_MB_RECHARGESUCCESS			552								//��ֵ�ɹ�����
#define SUB_MB_NOTIFYIDNEWMAIL			553								//���ʼ�����
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//���Ϳͷ�����
struct CMD_MB_Customer
{
	TCHAR							szContent[256];						//����
};

//��ֵ�ɹ�����
struct CMD_MB_RechargeSuccess
{
	SCORE							lScore;
	int								nID;								//�ʼ�ID
	TCHAR							szTitle[20];						//�ʼ�����
	TCHAR							szSender[20];						//������
	TCHAR							szContent[256];						//����
	TCHAR							szSendTime[20];						//����ʱ��
	int								nStatus;							//״̬
};

//���ʼ�����
struct CMD_MB_NotifyidNewMail
{
	int								nID;								//�ʼ�ID
	TCHAR							szTitle[20];						//�ʼ�����
	TCHAR							szSender[20];						//������
	TCHAR							szContent[256];						//����
	TCHAR							szSendTime[20];						//����ʱ��
	int								nStatus;							//״̬
};

//�޸�����
struct CMD_GP_ModifyLogonPass
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�����
struct CMD_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�ǩ��
struct CMD_GP_ModifyUnderWrite
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//�û�ͷ��
struct CMD_GP_UserFaceInfo
{
	bool							bSuccess;
	WORD							wFaceID;							//ͷ���ʶ
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�޸�ͷ��
struct CMD_GP_SystemFaceInfo
{
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�ͷ��
struct CMD_GP_CustomFaceInfo
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	DWORD							dwCustomFace[FACE_CX*FACE_CY];		//ͼƬ��Ϣ
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//�󶨻���
struct CMD_GP_ModifyMachine
{
	BYTE							cbBind;								//�󶨱�־
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//��������
struct CMD_GP_UserIndividual
{
	DWORD							dwUserID;							//�û� I D
};

//��ѯ��Ϣ
struct CMD_GP_QueryIndividual
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
};

//�޸�����
struct CMD_GP_ModifyIndividual
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
};

//��ѯ����
struct CMD_GP_QuerySendPresent
{
	DWORD							dwUserID;							//�û� I D
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//Я����Ϣ CMD_GP_UserIndividual
#define DTP_GP_UI_ACCOUNTS			1									//�û��˺�	
#define DTP_GP_UI_NICKNAME			2									//�û��ǳ�
#define DTP_GP_UI_USER_NOTE			3									//�û�˵��
#define DTP_GP_UI_UNDER_WRITE		4									//����ǩ��
#define DTP_GP_UI_QQ				5									//Q Q ����
#define DTP_GP_UI_EMAIL				6									//�����ʼ�
#define DTP_GP_UI_SEAT_PHONE		7									//�̶��绰
#define DTP_GP_UI_MOBILE_PHONE		8									//�ƶ��绰
#define DTP_GP_UI_COMPELLATION		9									//��ʵ����
#define DTP_GP_UI_DWELLING_PLACE	10									//��ϵ��ַ
#define DTP_GP_UI_PASSPORTID    	11									//��ݱ�ʶ
#define DTP_GP_UI_SPREADER			12									//�ƹ��ʶ
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//��������
struct CMD_GP_UserInsureInfo
{
	BYTE							cbEnjoinTransfer;					//ת�˿���
	WORD							wRevenueTake;						//˰�ձ���
	WORD							wRevenueTransfer;					//˰�ձ���
	WORD							wRevenueTransferMember;				//˰�ձ���
	WORD							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
	SCORE							lTransferPrerequisite;				//ת������
};

//��ͨ����
struct CMD_GP_UserEnableInsure
{
	DWORD							dwUserID;							//�û�I D
	TCHAR							szLogonPass[LEN_PASSWORD];			//��¼����
	TCHAR							szInsurePass[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//������
struct CMD_GP_UserSaveScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lSaveScore;							//������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ȡ���
struct CMD_GP_UserTakeScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lTakeScore;							//��ȡ���
	TCHAR							szPassword[LEN_MD5];				//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ת�˽��
struct CMD_GP_UserTransferScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lTransferScore;						//ת�˽��
	TCHAR							szPassword[LEN_MD5];				//��������
	TCHAR							szAccounts[LEN_NICKNAME];			//Ŀ���û�
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szTransRemark[LEN_TRANS_REMARK];	//ת�˱�ע
};

//��Ǯ���
struct CMD_GP_UserSaveResult
{
	bool							bSuccess;							//�Ƿ�ɹ�
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
	TCHAR							szDescribeString[128];				//������Ϣ
};

//ȡǮ���
struct CMD_GP_UserTakeResult
{
	bool							bSuccess;							//�Ƿ�ɹ�
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
	TCHAR							szDescribeString[128];				//������Ϣ
};

//���гɹ�
struct CMD_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
	TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct CMD_GP_UserInsureFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��ѯ����
struct CMD_GP_QueryInsureInfo
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��������
};

//��ѯ�û������п���Ϣ
struct CMD_GP_QueryBankInfo
{
	DWORD							dwUserID;							//�û� I D
};

//��ѯ�û�
struct CMD_GP_QueryUserInfoRequest
{
	BYTE                            cbByNickName;                       //�ǳ�����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//Ŀ���û�
};

//�û���Ϣ
struct CMD_GP_UserTransferUserInfo
{
	DWORD							dwTargetGameID;						//Ŀ���û�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//Ŀ���û�
};

//��ͨ���
struct CMD_GP_UserInsureEnableResult
{
	BYTE							cbInsureEnabled;					//ʹ�ܱ�ʶ
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��ѯ����
struct CMD_GP_QueryTransferRebate
{
	DWORD							dwUserID;							//�û�I D
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
};

//��ѯ���
struct CMD_GP_QueryTransferRebateResult
{
	DWORD							dwUserID;							//�û�I D
	BYTE							cbRebateEnabled;					//ʹ�ܱ�ʶ	
	SCORE							lIngot;								//����Ԫ��
	SCORE							lLoveLiness;						//��������ֵ
};

//�û����ݽ��
struct CMD_GP_QueryUserDataResult
{
	DWORD							dwUserID;							//�û�ID
};

//��ѯ�ɼ�����Ϸ����
struct CMD_GP_QueryGameRoom
{
	WORD							wKindID;							//��Ϸ��ʶ
	WORD							wServerLevel;						//��Ϸ�ȼ���ʶ
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//��ȡ�ͱ�
struct CMD_GP_BaseEnsureTake
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�ͱ�����
struct CMD_GP_BaseEnsureParamter
{
	SCORE							lScoreCondition;					//��Ϸ������
	SCORE							lScoreAmount;						//��Ϸ������
	BYTE							cbTakeTimes;						//��ȡ����	
};

//�ͱ����
struct CMD_GP_BaseEnsureResult
{
	bool							bSuccessed;							//�ɹ���ʶ
	SCORE							lGameScore;							//��ǰ��Ϸ��
	int								nTakeTimes;							//����ȡ����
	TCHAR							szNotifyContent[128];				//��ʾ����
};

//��ѯ�ɼ�����Ϸ������
struct CMD_GP_QueryGameRoomResult
{
	WORD							wServerPort;						//����˿�
	TCHAR							szServerAddr[32];					//��������
};
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//�ƹ����

//�ƹ��ѯ
struct CMD_GP_UserSpreadQuery
{
	DWORD							dwUserID;							//�û���ʶ
};

//�ƹ����
struct CMD_GP_UserSpreadInfo
{
	DWORD							dwSpreadCount;						//�ƹ�����
	SCORE							lSpreadReward;						//�ƹ㽱��
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//�齱����

//��������
struct CMD_GP_LotteryConfigReq
{
	WORD							wKindID;							//��Ϸ��ʶ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
};

//�齱����
struct CMD_GP_LotteryConfig
{
	WORD							wLotteryCount;						//�������
	tagLotteryItem					LotteryItem[MAX_LOTTERY];			//��������
};

//�齱��Ϣ
struct CMD_GP_LotteryUserInfo
{
	BYTE							cbFreeCount;						//��Ѵ���
	BYTE							cbAlreadyCount;						//�������
	WORD							wKindID;							//��Ϸ��ʶ
	DWORD							dwUserID;							//�û���ʶ
	SCORE							lChargeFee;							//�齱����
};

//��ʼ�齱
struct CMD_GP_LotteryStart
{
	WORD							wKindID;							//��Ϸ��ʶ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�齱���
struct CMD_GP_LotteryResult
{
	bool							bWined;								//�н���ʶ
	WORD							wKindID;							//��Ϸ��ʶ
	DWORD							dwUserID;							//�û���ʶ
	SCORE							lUserScore;							//�û�����
	DOUBLE							dUserBeans;							//�û���Ϸ��
	tagLotteryItem					LotteryItem;						//�н�����
};

//��ȡ�ʼ�
struct CMD_GP_QueryEmail
{
	DWORD							dwUserID;							//�û���ʶ
	int								nIndex;								//��ʶ����
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//��Ϸ����
struct CMD_GP_QueryUserGameData
{
	WORD							wKindID;							//��Ϸ��ʶ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szDynamicPass[LEN_MD5];				//�û�����	
};

//������Ϣ
#define DTP_GP_UI_USER_GAME_DATA	1									//��Ϸ����	

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//���Ͻ��
struct CMD_GP_IndividuaResult
{
	bool							bSuccessed;							//�ɹ���ʶ
	TCHAR							szNotifyContent[128];				//��ʾ����
};


/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//��������
struct CMD_GP_MatchSignup
{
	//������Ϣ
	WORD							wServerID;							//�����ʶ
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������

	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ȡ������
struct CMD_GP_MatchUnSignup
{
	//������Ϣ
	WORD							wServerID;							//�����ʶ
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������

	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�������
struct CMD_GP_MatchSignupResult
{
	bool							bSignup;							//������ʶ
	bool							bSuccessed;							//�ɹ���ʶ
	WORD							wServerID;							//�����ʶ
	SCORE							lCurrScore;							//��ǰ���
	TCHAR							szDescribeString[128];				//������Ϣ
};
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//�˻���
struct CMD_GP_AccountBind
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	TCHAR							szBindNewAccounts[LEN_ACCOUNTS];	//���ʺ�
	TCHAR							szBindNewPassword[LEN_PASSWORD];	//������
	TCHAR							szBindNewSpreader[LEN_ACCOUNTS];	//���Ƽ�
};

//�˻���
struct CMD_GP_AccountBind_Exists
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	TCHAR							szBindExistsAccounts[LEN_ACCOUNTS];	//���ʺ�
	TCHAR							szBindExistsPassword[LEN_PASSWORD];	//������
};

//ip������
struct CMD_GP_IpLocation
{
	DWORD							dwUserID;
	TCHAR							szLocation[LEN_IP];	//ip������
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//�һ���ҽ��
struct CMD_GP_ExchangeScoreResult
{
	bool							bSuccess;						//�������
	SCORE							lScore;								//���
	SCORE							lInsureScore;						//���н��
	TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct CMD_GP_OperateFailure
{
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�����ɹ�
struct CMD_GP_OperateSuccess
{
	TCHAR							szDescribeString[128];				//�ɹ���Ϣ
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//Զ�̷���

#define MDM_GP_REMOTE_SERVICE		4									//Զ�̷���

//���ҷ���
#define SUB_GP_C_SEARCH_DATABASE	100									//���ݲ���
#define SUB_GP_C_SEARCH_CORRESPOND	101									//Э������
#define SUB_GP_C_SEARCH_ALLCORRESPOND	102								//Э������

//���ҷ���
#define SUB_GP_S_SEARCH_DATABASE	200									//���ݲ���
#define SUB_GP_S_SEARCH_CORRESPOND	201									//Э������
#define SUB_GP_S_SEARCH_ALLCORRESPOND	202								//Э������

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//Э������
struct CMD_GP_C_SearchCorrespond
{
	DWORD							dwGameID;							//��Ϸ��ʶ
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
};

struct CMD_GP_C_SearchAllCorrespond
{
	DWORD							dwCount;
	DWORD							dwGameID[1];							//��Ϸ��ʶ
};

//Э������
struct CMD_GP_S_SearchCorrespond
{
	WORD							wUserCount;							//�û���Ŀ
	tagUserRemoteInfo				UserRemoteInfo[16];					//�û���Ϣ
};

struct CMD_GP_S_SearchAllCorrespond
{
	DWORD							dwCount;
	tagUserRemoteInfo				UserRemoteInfo[1];					//�û���Ϣ				
};


/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//��������

#define MDM_GP_ANDROID_SERVICE		5									//��������

//��������
#define SUB_GP_GET_PARAMETER		100									//��ȡ����
#define SUB_GP_ADD_PARAMETER		101									//��Ӳ���
#define SUB_GP_MODIFY_PARAMETER		102									//�޸Ĳ���
#define SUB_GP_DELETE_PARAMETER		103									//ɾ������

//������Ϣ
#define SUB_GP_ANDROID_PARAMETER	200									//��������		

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//��ȡ����
struct CMD_GP_GetParameter
{
	WORD							wServerID;							//�����ʶ
};

//��Ӳ���
struct CMD_GP_AddParameter
{
	WORD							wServerID;							//�����ʶ
	tagAndroidParameter				AndroidParameter;					//��������
};

//�޸Ĳ���
struct CMD_GP_ModifyParameter
{
	WORD							wServerID;							//�����ʶ
	tagAndroidParameter				AndroidParameter;					//��������
};

//ɾ������
struct CMD_GP_DeleteParameter
{
	WORD							wServerID;							//�����ʶ
	DWORD							dwBatchID;							//���α�ʶ
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//��������
struct CMD_GP_AndroidParameter
{
	WORD							wSubCommdID;						//��������
	WORD							wParameterCount;					//������Ŀ
	tagAndroidParameter				AndroidParameter[MAX_BATCH];		//��������
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//��¼����
#define MDM_MB_LOGON				100									//�㳡��¼

//��¼ģʽ
#define SUB_MB_LOGON_GAMEID			1									//I D ��¼
#define SUB_MB_LOGON_ACCOUNTS		2									//�ʺŵ�¼
#define SUB_MB_REGISTER_ACCOUNTS	3									//ע���ʺ�
#define SUB_MB_LOGON_OTHERPLATFORM	4									//������¼
#define SUB_MB_LOGON_VISITOR		5									//�ο͵�¼
#define SUB_MB_QUERY_GAMELOCKINFO 				 14							//��ѯ��Ϸ������Ϣ

//��¼���
#define SUB_MB_LOGON_SUCCESS		100									//��¼�ɹ�
#define SUB_MB_LOGON_FAILURE		101									//��¼ʧ��
#define SUB_MB_MATCH_SIGNUPINFO		102									//������Ϣ

//������ʾ
#define SUB_MB_UPDATE_NOTIFY		200									//������ʾ

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//I D ��¼
struct CMD_MB_LogonGameID
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE                            cbDeviceType;                       //�豸����

	//��¼��Ϣ
	DWORD							dwGameID;							//��Ϸ I D
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//�ʺŵ�¼
struct CMD_MB_LogonAccounts
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE                            cbDeviceType;                       //�豸����

	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//�ʺŵ�¼
struct CMD_MB_LogonOtherPlatform
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE                            cbDeviceType;                       //�豸����

	//��¼��Ϣ
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbPlatformID;						//ƽ̨���
	TCHAR							szUserUin[LEN_USER_UIN];			//�û�Uin
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����
	TCHAR							szAgentID[LEN_ACCOUNTS];			//�����ʶ

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//ע���ʺ�
struct CMD_MB_RegisterAccounts
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE                            cbDeviceType;                       //�豸����

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����

	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	DWORD							dwSpreaderGameID;					//�Ƽ���ʶ	
	TCHAR							szAgentID[LEN_ACCOUNTS];			//�����ʶ	

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
	TCHAR							szRegisterMobile[LEN_MOBILE_PHONE];	//ע���ֻ�����-���˺���
};

//ע���ֻ��ʺ�
struct CMD_MB_PhoneRegisterAccounts
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE                            cbDeviceType;                       //�豸����

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����

	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	DWORD							dwSpreaderGameID;					//�Ƽ���ʶ	
	TCHAR							szAgentID[LEN_ACCOUNTS];			//�����ʶ	
	TCHAR							szVerifyCode[LEN_VERIFY_CODE];		//ע�������֤��

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};


//��ȡ�ֻ�ע����֤��
struct CMD_MB_GetRegisterVerifyCode
{
	//ϵͳ��Ϣ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ֻ���
	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//��ȡ�û���Ϣ
struct CMD_MB_QueryUserData
{
	//ϵͳ��Ϣ
	BYTE cbType;								//��������
	TCHAR szWhere[LEN_WHERE_PARM];				//��������
};

//Ͷ�ߴ���
struct CMD_MB_TouSuDaili
{
	//�û�ID
	DWORD dwUserID;								//�û�ID
	TCHAR szUserWx[LEN_AGENT_ID];				//�û�΢��
	TCHAR szAgentWx[LEN_AGENT_ID];				//����΢��
	TCHAR szReason[LEN_DESC];					//����΢��
	TCHAR szImageName[LEN_IMAGE_NAME];			//����΢��
};

//�������ǰ���
struct CMD_MB_ShenQingDailiCheck
{
	DWORD dwUserID;								//�û�ID
};

//�������
struct CMD_MB_ShenQingDaili
{
	DWORD							dwUserID;						//�û�ID
	TCHAR							szAgentAcc[LEN_ACCOUNTS];		//�����˺�
	TCHAR							szAgentName[LEN_AGENT_ID];		//��������
	TCHAR							szAlipay[LEN_ALIPAY];			//֧����
	TCHAR							szWeChat[LEN_WECHAT];			//΢�ź�
	TCHAR							szQQ[LEN_QQ];					//QQ��
	TCHAR							szNote[LEN_DESC];				//��ע
};

//��ѯ��Ϸ������Ϣ
struct CMD_MB_QueryGameLockInfo
{
	DWORD							dwUserID;						//�û�ID
};

//��ȡ�޸�������֤��
struct CMD_MB_GetModifyPassVerifyCode
{
	//ϵͳ��Ϣ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ֻ���
	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//��ȡ�޸�������Ϣ��֤��
struct CMD_MB_GetModifyBankInfoVerifyCode
{
	//ϵͳ��Ϣ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ֻ���
	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//��֤���޸�����
struct CMD_MB_ModifyPassVerifyCode
{
	//ϵͳ��Ϣ
	TCHAR							szAccounts[LEN_ACCOUNTS];			//�û��˺�
	TCHAR							szPassword[LEN_PASSWORD];			//������
	BYTE							cbMode;								//���� ��¼
	TCHAR							szVerifyCode[LEN_VERIFY_CODE];		//ע�������֤��
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ֻ���
	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//��֤���޸�������Ϣ
struct CMD_MB_ModifyBankInfoVerifyCode
{
	//ϵͳ��Ϣ
	DWORD							dwUserID;						//�û�ID
	TCHAR							szBankNo[LEN_BANK_NO];			//���п���
	TCHAR							szBankName[LEN_BANK_NAME];		//��������

	TCHAR							szVerifyCode[LEN_VERIFY_CODE];		//ע�������֤��
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ֻ���
	TCHAR							szBankAddress[LEN_BANK_ADDRESS];	//���е�ַ
	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//�ο͵�¼
struct CMD_MB_LogonVisitor
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szAgentID[LEN_ACCOUNTS];			//�����ʶ
	BYTE                            cbDeviceType;                       //�豸����

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//��¼�ɹ�
struct CMD_MB_LogonSuccess
{
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwCustomID;							//�Զ�ͷ��
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwSpreaderID;						//�ƹ�ID
	DWORD							dwExperience;						//������ֵ
	TCHAR							szAccounts[LEN_ACCOUNTS];			//�û��ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szAliPayAcccount[30];				//֧�����˻�
	TCHAR							szBinkID[20];						//���п��˻�
	TCHAR							szDynamicPass[LEN_PASSWORD];		//��̬����

	//�Ƹ���Ϣ
	SCORE							lUserScore;							//�û���Ϸ��
	SCORE							lUserInsure;						//�û�����	

	//��չ��Ϣ
	BYTE							cbInsureEnabled;					//ʹ�ܱ�ʶ
	BYTE							cbIsAgent;							//�����ʶ
	BYTE							cbMoorMachine;						//��������

	//Լս�����
	int								TodayAlmsCount;						//ÿ�յͱ�����ȡ����
	DWORD							dwLockServerID;						//��������
	DWORD							dwKindID;							//��Ϸ����
};

//��¼ʧ��
struct CMD_MB_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//������ʾ
struct CMD_MB_UpdateNotify
{
	BYTE							cbMustUpdate;						//ǿ������
	BYTE							cbAdviceUpdate;						//��������
	DWORD							dwCurrentVersion;					//��ǰ�汾
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//�б�����������
#define MDM_MB_SERVER_LIST			101									//�б���Ϣ

//��ѯ��Ϣ
#define SUB_QUEREY_SERVER			1									//��ѯ�����б�

struct CMD_MB_QueryServer
{
	WORD							wKindID;							
};

//�����Ϣ
#define SUB_MB_LIST_SERVER_START	1									//��ʼ�����б�֪ͨ
#define SUB_MB_LIST_SERVER_END		2									//���������б�֪ͨ

#define SUB_MB_LIST_KIND			100									//�����б�
#define SUB_MB_LIST_SERVER			101									//�����б�
#define SUB_MB_LIST_MATCH			102									//�����б�	
#define SUB_MB_LIST_FINISH			200									//�б����

#define SUB_MB_GET_ONLINE			300									//��ȡ����
#define SUB_MB_KINE_ONLINE			301									//��������
#define SUB_MB_SERVER_ONLINE		302									//��������
#define SUB_MB_AGENT_KIND			400									//�����б�
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//��ȡ����
struct CMD_MB_GetOnline
{
	WORD							wServerCount;						//������Ŀ
	WORD							wOnLineServerID[MAX_SERVER];		//�����ʶ
};

//��������
struct CMD_MB_KindOnline
{
	WORD							wKindCount;							//������Ŀ
	tagOnLineInfoKind				OnLineInfoKind[MAX_KIND];			//��������
};

//��������
struct CMD_MB_ServerOnline
{
	WORD							wServerCount;						//������Ŀ
	tagOnLineInfoServer				OnLineInfoServer[MAX_SERVER];		//��������
};

//�һ����
struct CMD_GP_ExchangeBankScore
{
	DWORD dwUserID;							//�û�id
	SCORE dwScore;		//��ʵ����
	TCHAR szBankPass[LEN_PASSWORD];			//���п���
	TCHAR szOrderID[LEN_PASSWORD];		//��������
	DWORD dwType;							//0 bank 1 alipay
};
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

#pragma pack()

#endif