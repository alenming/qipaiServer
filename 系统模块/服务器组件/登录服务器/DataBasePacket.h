#ifndef DATA_BASE_PACKET_HEAD_FILE
#define DATA_BASE_PACKET_HEAD_FILE

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//�������ݰ�

//�ʺŷ���
#define DBR_GP_MODIFY_INSURE_PASS	12									//�޸�����
#define DBR_GP_MODIFY_INDIVIDUAL	14									//�޸�����

//ͷ������
#define DBR_GP_MODIFY_SYSTEM_FACE	20									//�޸�ͷ��

//��������
#define DBR_GP_USER_ENABLE_INSURE	30									//��ͨ����
#define DBR_GP_USER_SAVE_SCORE		31									//������Ϸ��
#define DBR_GP_USER_TAKE_SCORE		32									//��ȡ��Ϸ��
#define DBR_GP_USER_TRANSFER_SCORE	33									//ת����Ϸ��
#define DBR_GP_QUERY_BANK_INFO	34										//��ѯ�û���������Ϣ
#define DBR_GP_EXCHANGE_BANK_SCORE	36										//�һ����н��

//��ѯ����
#define DBR_GP_QUERY_INSURE_INFO	41									//��ѯ����
#define DBR_GP_QUERY_USER_INFO	    42									//��ѯ�û�

//ϵͳ����
#define DBR_GP_ONLINE_COUNT_INFO	60									//������Ϣ
#define DBR_GP_LOAD_GAME_LIST		61									//�����б�
#define DBR_GP_LOAD_BASEENSURE		64									//���صͱ�
#define DBR_GP_LOAD_PLATFORM_PARAMETER 67								//ƽ̨����


//��������
#define DBR_GP_GET_PARAMETER		70									//��ȡ����
#define DBR_GP_ADD_PARAMETER		71									//��Ӳ���
#define DBR_GP_MODIFY_PARAMETER		72									//�޸Ĳ���
#define DBR_GP_DELETE_PARAMETER		73									//ɾ������

//�ͱ�����
#define DBR_GP_BASEENSURE_TAKE		100									//��ȡ�ͱ�

//�齱����
#define DBR_GP_LOTTERY_CONFIG_REQ	140									//��������
#define DBR_GP_LOTTERY_START		141									//��ʼ�齱

//�ʼ��ͷ�
#define DBR_GP_QUERY_EMAIL			150									//��ȡ�ʼ�

//��֤����
#define DBR_GP_SERVER_DUMMY_ONLINE	168							//��ѯ������������


/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//ID ��¼
struct DBR_GP_LogonGameID
{
	//��¼��Ϣ
	DWORD							dwGameID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	BYTE							cbNeeValidateMBCard;				//�ܱ�У��

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺŵ�¼
struct DBR_GP_LogonAccounts
{
	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//���֤��
	BYTE							cbNeeValidateMBCard;				//�ܱ�У��

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���

	//��չ��Ϣ
	DWORD							dwCheckUserRight;					//���Ȩ��
};

//�ο͵�½
struct DBR_GP_LogonVisitor
{
	//��¼��Ϣ
	BYTE							cbPlatformID;						//ƽ̨���

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺŰ�
struct DBR_GP_AccountBind
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	TCHAR							szBindNewAccounts[LEN_ACCOUNTS];	//���ʺ�
	TCHAR							szBindNewPassword[LEN_PASSWORD];	//������
	TCHAR							szBindNewSpreader[LEN_ACCOUNTS];	//���Ƽ�
};

//ip������
struct DBR_GP_IpLocation
{
	DWORD							dwUserID;
	TCHAR							szLocation[LEN_IP];	//ip������
};

//�ʺŰ�
struct DBR_GP_AccountBind_Exists
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	TCHAR							szBindExistsAccounts[LEN_ACCOUNTS];	//���ʺ�
	TCHAR							szBindExistsPassword[LEN_PASSWORD];	//������
};

//�ʺ�ע��
struct DBR_GP_RegisterAccounts
{
	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����
	TCHAR							szAgentID[LEN_ACCOUNTS];			//�����ʶ
	DWORD							dwSpreaderGameID;					//�Ƽ���ʶ

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//��֤����
struct DBR_GP_VerifyIndividual
{
	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���

	WORD							wVerifyMask;						//У����֤
	TCHAR							szVerifyContent[LEN_ACCOUNTS];		//��֤����
};

//�޸Ļ���
struct DBR_GP_ModifyMachine
{
	BYTE							cbBind;								//�󶨱�־
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�����
struct DBR_GP_ModifyLogonPass
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�����
struct DBR_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�ǩ��
struct DBR_GP_ModifyUnderWrite
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��
};

//�޸�ͷ��
struct DBR_GP_ModifySystemFace
{
	//�û���Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�ͷ��
struct DBR_GP_ModifyCustomFace
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	DWORD							dwCustomFace[FACE_CX*FACE_CY];		//ͼƬ��Ϣ

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�����
struct DBR_GP_ModifyIndividual
{
	//��֤����
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����

	//�ʺ�����
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
};

//��ѯ����
struct DBR_GP_QueryIndividual
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//��������
};

//��ͨ����
struct DBR_GP_UserEnableInsure
{
	DWORD							dwUserID;							//�û� I D	
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szLogonPass[LEN_PASSWORD];			//��¼����
	TCHAR							szInsurePass[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//������Ϸ��
struct DBR_GP_UserSaveScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lSaveScore;							//������Ϸ��
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ѯ�û���������Ϣ
struct DBR_GP_QueryBankInfo
{
	DWORD							dwUserID;							//�û� I D
};

//������
struct DBR_GP_BindBankInfo
{											
	DWORD dwUserID;							//�û�id
	TCHAR szRealName[LEN_REAL_NAME];		//��ʵ����
	TCHAR szBankNo[LEN_BANK_NO];			//���п���
	TCHAR szBankName[LEN_BANK_NAME];		//��������
	TCHAR szBankAddress[LEN_BANK_ADDRESS];	//��������
	TCHAR szAlipayID[LEN_BANK_ADDRESS];		//֧����
};

//������
struct DBR_GP_ExchangeBankScore
{											
	DWORD dwUserID;							//�û�id
	SCORE dwScore;		//��ʵ����
	TCHAR szBankPass[LEN_PASSWORD];			//���п���
	TCHAR szOrderID[LEN_PASSWORD];		//��������
	DWORD dwClientAddr;						//���ӵ�ַ
	DWORD dwType;							//0 bank 1 alipay
};

//ȡ����Ϸ��
struct DBR_GP_UserTakeScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lTakeScore;							//��ȡ��Ϸ��
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ת����Ϸ��
struct DBR_GP_UserTransferScore
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	SCORE							lTransferScore;						//ת����Ϸ��
	TCHAR							szAccounts[LEN_ACCOUNTS];			//�û��ǳ�
	TCHAR							szPassword[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szTransRemark[LEN_TRANS_REMARK];	//ת�ʱ�ע
};

//��ѯ����
struct DBR_GP_QueryInsureInfo
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
};

//��ѯ�û�
struct DBR_GP_QueryInsureUserInfo
{
	BYTE                            cbByNickName;                       //�ǳ�����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//Ŀ���û�
};

//�û�����
struct DBO_GP_UserTransferUserInfo
{
	DWORD							dwGameID;							//�û� I D
	TCHAR							szAccounts[LEN_ACCOUNTS];			//�û��ʺ�
};

//��ѯ����
struct DBR_GP_QueryTransferRebate
{
	DWORD							dwUserID;							//�û�I D
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
};

//������Ϣ
struct DBR_GP_OnLineCountInfo
{
	WORD							wKindCount;							//������Ŀ
	DWORD							dwOnLineCountSum;					//��������
	DWORD							dwAndroidCountSum;					//��������
	tagOnLineInfoKindEx				OnLineCountKind[MAX_KIND];			//��������
};


//��������
struct DBR_GP_MatchSignup
{
	//������Ϣ
	WORD							wServerID;							//�����ʶ
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������

	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��������
struct DBR_GP_MatchUnSignup
{
	//������Ϣ
	WORD							wServerID;							//�����ʶ
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������

	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};


//��ȡ����
struct DBR_GP_GetParameter
{
	WORD							wServerID;							//�����ʶ
};

//��Ӳ���
struct DBR_GP_AddParameter
{
	WORD							wServerID;							//�����ʶ
	tagAndroidParameter				AndroidParameter;					//��������
};

//�޸Ĳ���
struct DBR_GP_ModifyParameter
{
	WORD							wServerID;							//�����ʶ
	tagAndroidParameter				AndroidParameter;					//��������
};

//ɾ������
struct DBR_GP_DeleteParameter
{
	WORD							wServerID;							//�����ʶ
	DWORD							dwBatchID;							//���α�ʶ
};

//��ȡ�ͱ�
struct DBR_GP_TakeBaseEnsure
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�ƹ���Ϣ
struct DBR_GP_QuerySpreadInfo
{
	DWORD							dwUserID;							//�û���ʶ
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//�齱����

//��ʼ�齱
struct DBR_GP_LotteryConfigReq
{
	WORD							wKindID;							//��Ϸ��ʶ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szLogonPass[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//��ʼ�齱
struct DBR_GP_LotteryStart
{
	WORD							wKindID;							//��Ϸ��ʶ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szLogonPass[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ȡ�ʼ�
struct DBR_GP_QueryEmail
{
	DWORD							dwUserID;							//�û���ʶ
	int								nIndex;								//��ʶ����
};
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//��Ϸ����
struct DBR_GP_QueryUserGameData
{
	WORD							wKindID;							//��Ϸ��ʶ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szDynamicPass[LEN_MD5];				//�û�����	
};



/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//�������
#define DBO_GP_USER_FACE_INFO		110									//�û�ͷ��

//��������
#define DBO_GP_USER_INSURE_INFO		120									//��������
#define DBO_GP_USER_INSURE_SUCCESS	121									//���гɹ�
#define DBO_GP_USER_INSURE_FAILURE	122									//����ʧ��
#define DBO_GP_USER_INSURE_USER_INFO 123								//�û�����
#define DBO_GP_USER_INSURE_ENABLE_RESULT 124							//��ͨ���

#define DBO_GP_USER_SAVE_RESULT		125									//�û���Ǯ���
#define DBO_GP_USER_TAKE_RESULT		126									//�û�ȡǮ���

//�б���
#define DBO_GP_GAME_TYPE_ITEM		130									//������Ϣ
#define DBO_GP_GAME_KIND_ITEM		131									//������Ϣ
#define DBO_GP_GAME_NODE_ITEM		132									//�ڵ���Ϣ
#define DBO_GP_GAME_PAGE_ITEM		133									//������Ϣ
#define DBO_GP_GAME_LIST_RESULT		134									//���ؽ��

//ϵͳ���
#define DBO_GP_PLATFORM_PARAMETER	140									//ƽ̨����
#define DBO_GP_SERVER_DUMMY_ONLINE	141									//������������
#define DBO_GP_SERVER_DUMMY_ONLINE_BEGIN	142							//������������
#define DBO_GP_SERVER_DUMMY_ONLINE_END	143								//������������
//��������
#define DBO_GP_ANDROID_PARAMETER	150									//��������	

//�ͱ�����
#define DBO_GP_BASEENSURE_PARAMETER	180									//�ͱ�����
#define DBO_GP_BASEENSURE_RESULT	181									//��ȡ���

//�齱���
#define DBO_GP_LOTTERY_CONFIG		230									//�齱����
#define DBO_GP_LOTTERY_USER_INFO	231									//�齱��Ϣ
#define DBO_GP_LOTTERY_RESULT		232									//�齱���

//���Ͻ��
#define DBO_GP_INDIVIDUAL_RESULT	260									//���Ͻ��	

//�����б�
#define DBO_GP_AGENT_GAME_KIND_ITEM		351								//������Ϣ

#define DBO_GP_EXCHANGESCORE_RESULT	401									//�һ���ҽ��

//������
#define DBO_GP_OPERATE_SUCCESS		800									//�����ɹ�
#define DBO_GP_OPERATE_FAILURE		801									//����ʧ��

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661////////

//��¼�ɹ�
struct DBO_GP_LogonSuccess
{
	//��������
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwGameID;							//��Ϸ��ʶ
	DWORD							dwGroupID;							//���ű�ʶ
	DWORD							dwCustomID;							//�Զ�����
	DWORD							dwExperience;						//������ֵ
	SCORE							lLoveLiness;						//�û�����
	TCHAR							szPassword[LEN_MD5];				//��¼����	
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szDynamicPass[LEN_PASSWORD];		//��̬����
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������

	//�û��ɼ�
	SCORE							lUserScore;							//�û���Ϸ��
	SCORE							lUserIngot;							//�û�Ԫ��
	SCORE							lUserInsure;						//�û�����	
	DOUBLE							dUserBeans;							//�û���Ϸ��

	//�û�����
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMoorMachine;						//��������
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��

	//��Ա����
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	SYSTEMTIME						MemberOverDate;						//����ʱ��

	//��չ��Ϣ
	BYTE							cbInsureEnabled;					//ʹ�ܱ�ʶ
	DWORD							dwCheckUserRight;					//���Ȩ��
	BYTE							cbIsAgent;							//�����ʶ	

	//������Ϣ
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��¼ʧ��
struct DBO_GP_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��¼ʧ��
struct DBO_GP_ValidateMBCard
{
	UINT							uMBCardID;						//��������
};

//��֤���
struct DBO_GP_VerifyIndividualResult
{
	bool							bVerifyPassage;						//��֤ͨ��
	WORD							wVerifyMask;						//��֤����
	TCHAR							szErrorMsg[128];					//������Ϣ
};

//ͷ����Ϣ
struct DBO_GP_UserFaceInfo
{
	bool							bSuccess;
	WORD							wFaceID;							//ͷ���ʶ
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��������
struct DBO_GP_UserIndividual
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szUserNote[LEN_USER_NOTE];			//�û�˵��
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������

	//�绰����
	TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//�̶��绰
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ƶ��绰

	//��ϵ����
	TCHAR							szQQ[LEN_QQ];						//Q Q ����
	TCHAR							szEMail[LEN_EMAIL];					//�����ʼ�
	TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//��ϵ��ַ

	//�ƹ���Ϣ
	TCHAR							szSpreader[LEN_ACCOUNTS];			//�ƹ���Ϣ
};

//��������
struct DBO_GP_UserInsureInfo
{
	BYTE							cbEnjoinTransfer;					//ת�ʿ���
	WORD							wRevenueTake;						//˰�ձ���
	WORD							wRevenueTransfer;					//˰�ձ���
	WORD							wRevenueTransferMember;				//˰�ձ���
	WORD							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û���Ϸ��
	SCORE							lUserInsure;						//������Ϸ��
	SCORE							lTransferPrerequisite;				//ת������
};

//��Ǯ���
struct DBO_GP_UserSaveResult
{
	bool							bSuccess;							//�Ƿ�ɹ�
	SCORE							lScore;								//���Ͻ��
	SCORE							lInsureScore;						//���н��
	TCHAR							szDescribeString[128];				//������Ϣ
};

//ȡǮ���
struct DBO_GP_UserTakeResult
{
	bool							bSuccess;							//�Ƿ�ɹ�
	SCORE							lScore;								//���Ͻ��
	SCORE							lInsureScore;						//���н��
	TCHAR							szDescribeString[128];				//������Ϣ
};

//���гɹ�
struct DBO_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lSourceScore;						//ԭ����Ϸ��
	SCORE							lSourceInsure;						//ԭ����Ϸ��
	SCORE							lInsureRevenue;						//����˰��
	SCORE							lVariationScore;					//��Ϸ�ұ仯
	SCORE							lVariationInsure;					//���б仯
	TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct  DBO_GP_UserInsureFailure
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��ͨ���
struct DBO_GP_UserInsureEnableResult
{
	BYTE							cbInsureEnabled;					//ʹ�ܱ�ʶ
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��ѯ�û������н��
struct DBO_GP_QueryBankInfoResult
{
	TCHAR							szRealName[LEN_REAL_NAME];			//��ʵ����
	TCHAR							szBankNo[LEN_BANK_NO];				//���п���
	SCORE							lDrawScore;							//�����ֽ��
	TCHAR							szAlipayID[LEN_BANK_ADDRESS];		//֧����
};

//��������
struct DBO_GP_PlatformParameter
{
	DWORD							dwExchangeRate;						//�һ�����
	DWORD							dwPresentExchangeRate;				//������Ϸ�Ҷһ���
	DWORD							dwRateGold;							//��Ϸ����Ϸ�Ҷһ���
};

//��������
struct DBO_GP_AndroidParameter
{
	WORD							wSubCommdID;						//��������
	WORD							wServerID;							//�����ʶ
	WORD							wParameterCount;					//������Ŀ
	tagAndroidParameter				AndroidParameter[MAX_BATCH];		//��������
};

//���Ͻ��
struct DBO_GP_IndividualResult
{
	bool							bSuccessed;							//�ɹ���ʶ
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��ѯ���
struct DBO_GP_QueryTransferRebateResult
{
	DWORD							dwUserID;							//�û�I D
	BYTE							cbRebateEnabled;					//ʹ�ܱ�ʶ	
	SCORE							lIngot;								//����Ԫ��
	SCORE							lLoveLiness;						//��������ֵ
};

//�ͱ�����
struct DBO_GP_BaseEnsureParameter
{
	SCORE							lScoreCondition;					//��Ϸ������
	SCORE							lScoreAmount;						//��Ϸ������
	BYTE							cbTakeTimes;						//��ȡ����	
};

//�ͱ����
struct DBO_GP_BaseEnsureResult
{
	bool							bSuccessed;							//�ɹ���ʶ
	SCORE							lGameScore;							//��ǰ��Ϸ��
	int								nTakeTimes;							//����ȡ����
	TCHAR							szNotifyContent[128];				//��ʾ����
};

//�ƹ���Ϣ
struct DBO_GP_UserSpreadInfo
{
	DWORD							dwSpreadCount;						//�ƹ�����
	SCORE							lSpreadReward;						//�ƹ㽱��
};

//ʵ����֤
struct DBO_GP_RealAuthParameter
{
	DWORD							dwAuthentDisable;					//��֤����
	SCORE							dwAuthRealAward;					//�������
};

//�û����ݽ��
struct DBO_GP_QueryUserDataResult
{
	DWORD							dwUserID;							//�û�ID
};

//�齱����
struct DBO_GP_LotteryConfig
{
	WORD							wLotteryCount;						//�������
	tagLotteryItem					LotteryItem[MAX_LOTTERY];			//��������
};

//�齱��Ϣ
struct DBO_GP_LotteryUserInfo
{
	BYTE							cbFreeCount;						//��Ѵ���
	BYTE							cbAlreadyCount;						//�������
	WORD							wKindID;							//��Ϸ��ʶ
	DWORD							dwUserID;							//�û���ʶ
	SCORE							lChargeFee;							//�齱����
};

//�齱���
struct DBO_GP_LotteryResult
{
	bool							bWined;								//�н���ʶ
	WORD							wKindID;							//��Ϸ��ʶ
	DWORD							dwUserID;							//�û���ʶ
	SCORE							lUserScore;							//�û�����
	DOUBLE							dUserBeans;							//�û���Ϸ��
	tagLotteryItem					LotteryItem;						//�н�����
};

//����ʧ��
struct DBO_GP_OperateFailure
{
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�����ɹ�
struct DBO_GP_OperateSuccess
{
	TCHAR							szDescribeString[128];				//�ɹ���Ϣ
};

//�һ���ҽ��
struct DBO_GP_ExchangeScoreResult
{
	bool							bSuccess;						//�������
	SCORE							lScore;								//���
	SCORE							lInsureScore;						//���н��
	TCHAR							szDescribeString[128];				//������Ϣ
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//��Ϸ����
struct DBO_GP_GameType
{
	WORD							wJoinID;							//�ҽ�����
	WORD							wSortID;							//��������
	WORD							wTypeID;							//��������
	TCHAR							szTypeName[LEN_TYPE];				//��������
};

//��Ϸ����
struct DBO_GP_GameKind
{
	WORD							wTypeID;							//��������
	WORD							wJoinID;							//�ҽ�����
	WORD							wSortID;							//��������
	WORD							wKindID;							//��������
	WORD							wGameID;							//ģ������
	WORD							wRecommend;							//�Ƽ���Ϸ
	WORD							wGameFlag;							//��Ϸ��־
	DWORD							dwSuportType;						//֧������
	TCHAR							szKindName[LEN_KIND];				//��Ϸ����
	TCHAR							szProcessName[LEN_PROCESS];			//��������
};

//��Ϸ�ڵ�
struct DBO_GP_GameNode
{
	WORD							wKindID;							//��������
	WORD							wJoinID;							//�ҽ�����
	WORD							wSortID;							//��������
	WORD							wNodeID;							//�ڵ�����
	TCHAR							szNodeName[LEN_NODE];				//�ڵ�����
};

//��������
struct DBO_GP_GamePage
{
	WORD							wKindID;							//��������
	WORD							wNodeID;							//�ڵ�����
	WORD							wSortID;							//��������
	WORD							wPageID;							//��������
	WORD							wOperateType;						//��������
	TCHAR							szDisplayName[LEN_PAGE];			//��ʾ����
};

//���ؽ��
struct DBO_GP_GameListResult
{
	BYTE							cbSuccess;							//�ɹ���־
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//�ֻ����ݰ�

//��¼����
#define	DBR_MB_LOGON_GAMEID			700									//I D ��¼
#define	DBR_MB_LOGON_ACCOUNTS		701									//�ʺŵ�¼
#define DBR_MB_REGISTER_ACCOUNTS	702									//ע���ʺ�
#define	DBR_MB_LOGON_OTHERPLATFORM	703									//������¼
#define	DBR_MB_LOGON_VISITOR		704									//�ο͵�¼
#define DBR_MB_QUERY_GAMELOCKINFO	711								//��Ϸ�����ѯ

//��¼���
#define DBO_MB_LOGON_SUCCESS		900									//��¼�ɹ�
#define DBO_MB_LOGON_FAILURE		901									//��¼ʧ��


/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//ID ��¼
struct DBR_MB_LogonGameID
{
	//��¼��Ϣ
	DWORD							dwGameID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺŵ�¼
struct DBR_MB_LogonAccounts
{
	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺŵ�¼
struct DBR_MB_LogonOtherPlatform
{
	//��¼��Ϣ
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbPlatformID;						//ƽ̨���
	TCHAR							szUserUin[LEN_USER_UIN];			//�û�Uin
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����
	TCHAR							szAgentID[LEN_ACCOUNTS];			//�����ʶ
	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ο͵�¼
struct DBR_MB_LogonVisitor
{
	//��¼��Ϣ
	BYTE							cbPlatformID;						//ƽ̨���

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
	TCHAR							szAgentID[LEN_ACCOUNTS];			//�����ʶ

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//Ͷ�ߴ���
struct DBR_MB_TouSuDaili
{
	//�û�ID
	DWORD dwUserID;								//�û�ID
	TCHAR szUserWx[LEN_AGENT_ID];				//�û�΢��
	TCHAR szAgentWx[LEN_AGENT_ID];				//����΢��
	TCHAR szReason[LEN_DESC];					//����΢��
	TCHAR szImageName[LEN_IMAGE_NAME];			//����΢��
};

//�������ǰ���
struct DBR_MB_ShenQingDailiCheck
{
	DWORD dwUserID;								//�û�ID
};

//�������
struct DBR_MB_ShenQingDaili
{
	DWORD							dwUserID;						//�û�ID
	TCHAR							szAgentAcc[LEN_ACCOUNTS];		//�����˺�
	TCHAR							szAgentName[LEN_AGENT_ID];		//��������
	TCHAR							szAlipay[LEN_ALIPAY];			//֧����
	TCHAR							szWeChat[LEN_WECHAT];			//΢�ź�
	TCHAR							szQQ[LEN_QQ];					//QQ��
	TCHAR							szNote[LEN_DESC];				//��ע
};

//�������
struct DBR_MB_QueryGameLockInfo
{
	DWORD							dwUserID;						//�û�ID
};


//�ʺ�ע��
struct DBR_MB_RegisterAccounts
{
	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
	BYTE                            cbDeviceType;                       //�豸����
	DWORD							dwSpreaderGameID;					//�Ƽ���ʶ
	TCHAR							szAgentID[LEN_ACCOUNTS];							//�����ʶ

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ֻ���ע��
struct DBR_MB_PhoneRegisterAccounts
{
	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
	BYTE                            cbDeviceType;                       //�豸����
	DWORD							dwSpreaderGameID;					//�Ƽ���ʶ
	TCHAR							szAgentID[LEN_ACCOUNTS];							//�����ʶ

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�޸�����
struct DBR_MB_ModifyPass
{
	//ע����Ϣ
	TCHAR							szAccounts[LEN_ACCOUNTS];			//�û��˺�
	TCHAR							szPassword[LEN_PASSWORD];			//������
	BYTE							cbMode;								//���� ��¼
	TCHAR							szVerifyCode[LEN_VERIFY_CODE];		//ע�������֤��
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ֻ���
	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//��֤���޸�������Ϣ
struct DBR_MB_ModifyBankInfo
{
	//ϵͳ��Ϣ
	DWORD							dwUserID;						//�û�ID
	TCHAR							szBankNo[LEN_BANK_NO];			//���п���
	TCHAR							szBankName[LEN_BANK_NAME];		//��������

	TCHAR							szBankAddress[LEN_BANK_ADDRESS];	//��������
	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//��ѯ�û���Ϣ
struct DBR_MB_QueryUserData
{
	//ϵͳ��Ϣ
	BYTE cbType;								//��������
	TCHAR szWhere[LEN_WHERE_PARM];				//��������
};

//��¼�ɹ�
struct DBO_MB_LogonSuccess
{
	//�û�����
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwCustomID;							//�Զ�ͷ��
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwSpreaderID;						//�ƹ�ID
	DWORD							dwExperience;						//������ֵ
	TCHAR							szAccounts[LEN_ACCOUNTS];			//�û��ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
	TCHAR							szAliPayAcccount[30];				//֧�����˻�
	TCHAR							szBinkID[20];						//���п��˻�
	TCHAR							szDynamicPass[LEN_PASSWORD];		//��̬����
	
	//�û��ɼ�
	SCORE							lUserScore;							//�û���Ϸ��
	SCORE							lUserInsure;						//�û�����	

	//��չ��Ϣ
	BYTE							cbInsureEnabled;					//ʹ�ܱ�ʶ
	BYTE							cbIsAgent;							//�����ʶ						
	BYTE							cbMoorMachine;						//��������
	int								TodayAlmsCount;						//ÿ�յͱ�����ȡ����
	DWORD							dwLockServerID;						//��������
	DWORD							dwKindID;							//��Ϸ����
};


//��¼ʧ��
struct DBO_MB_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};



/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

#endif