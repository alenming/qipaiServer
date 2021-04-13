#ifndef CMD_GAME_SERVER_HEAD_FILE
#define CMD_GAME_SERVER_HEAD_FILE

#pragma pack(1)

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//��¼����

#define MDM_GR_LOGON				1									//��¼��Ϣ

//��¼ģʽ
#define SUB_GR_LOGON_USERID			1									//I D ��¼
#define SUB_GR_LOGON_MOBILE			2									//�ֻ���¼

//��¼���
#define SUB_GR_LOGON_SUCCESS		100									//��¼�ɹ�
#define SUB_GR_LOGON_FAILURE		101									//��¼ʧ��
#define SUB_GR_LOGON_FINISH			102									//��¼���

//������ʾ
#define SUB_GR_UPDATE_NOTIFY		200									//������ʾ

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//I D ��¼
struct CMD_GR_LogonUserID
{
	//�汾��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	DWORD							dwFrameVersion;						//��ܰ汾
	DWORD							dwProcessVersion;					//���̰汾

	//��¼��Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR                           szServerPasswd[LEN_PASSWORD];       //��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	WORD							wKindID;							//��������
};

//�ֻ���¼
struct CMD_GR_LogonMobile
{
	//�汾��Ϣ
	WORD							wGameID;							//��Ϸ��ʶ
	DWORD							dwProcessVersion;					//���̰汾

	//��������
	BYTE                            cbDeviceType;                       //�豸����
	WORD                            wBehaviorFlags;                     //��Ϊ��ʶ
	WORD                            wPageTableCount;                    //��ҳ����

	//��¼��Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR                           szServerPasswd[LEN_PASSWORD];       //��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//�ʺŵ�¼
struct CMD_GR_LogonAccounts
{
	//�汾��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	DWORD							dwFrameVersion;						//��ܰ汾
	DWORD							dwProcessVersion;					//���̰汾

	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��¼�ɹ�
struct CMD_GR_LogonSuccess
{
	DWORD							dwUserRight;						//�û�Ȩ��
	DWORD							dwMasterRight;						//����Ȩ��
};

//��¼ʧ��
struct CMD_GR_LogonFailure
{
	LONG							lErrorCode;							//�������

	DWORD							dwLockKindID;						//��ס�������ϷKindID
	DWORD							dwLockServerID;						//��ס����ķ���ServerID

	TCHAR							szDescribeString[128];				//������Ϣ
};

//������ʾ
struct CMD_GR_UpdateNotify
{
	//������־
	BYTE							cbMustUpdatePlaza;					//ǿ������
	BYTE							cbMustUpdateClient;					//ǿ������
	BYTE							cbAdviceUpdateClient;				//��������

	//��ǰ�汾
	DWORD							dwCurrentPlazaVersion;				//��ǰ�汾
	DWORD							dwCurrentFrameVersion;				//��ǰ�汾
	DWORD							dwCurrentClientVersion;				//��ǰ�汾
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//��������

#define MDM_GR_CONFIG				2									//������Ϣ

#define SUB_GR_CONFIG_SERVER		101									//��������
#define SUB_GR_CONFIG_FINISH		103									//�������
#define SUB_GR_CONFIG_USER_RIGHT	104									//���Ȩ��

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//�б�����
struct CMD_GR_ConfigColumn
{
	BYTE							cbColumnCount;						//�б���Ŀ
	tagColumnItem					ColumnItem[MAX_COLUMN];				//�б�����
};

//��������
struct CMD_GR_ConfigServer
{
	//��������
	WORD							wTableCount;						//������Ŀ
	WORD							wChairCount;						//������Ŀ

	//��������
	WORD							wServerType;						//��������
	DWORD							dwServerRule;						//�������
};


//���Ȩ��
struct CMD_GR_ConfigUserRight
{
	DWORD							dwUserRight;						//���Ȩ��
};
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//�û�����

#define MDM_GR_USER					3									//�û���Ϣ

//�û�����
#define SUB_GR_USER_RULE			1									//�û�����
#define SUB_GR_USER_LOOKON			2									//�Թ�����
#define SUB_GR_USER_SITDOWN			3									//��������
#define SUB_GR_USER_STANDUP			4									//��������
#define SUB_GR_USER_INVITE			5									//�û�����
#define SUB_GR_USER_INVITE_REQ		6									//��������
#define SUB_GR_USER_REPULSE_SIT  	7									//�ܾ��������
#define SUB_GR_USER_KICK_USER       8                                   //�߳��û�
#define SUB_GR_USER_INFO_REQ        9                                   //�����û���Ϣ
#define SUB_GR_USER_CHAIR_REQ       10                                  //�������λ��
#define SUB_GR_USER_CHAIR_INFO_REQ  11                                  //���������û���Ϣ
#define SUB_GR_USER_WAIT_DISTRIBUTE 12									//�ȴ�����

//�û�״̬
#define SUB_GR_USER_ENTER			100									//�û�����
#define SUB_GR_USER_SCORE			101									//�û�����
#define SUB_GR_USER_STATUS			102									//�û�״̬
#define SUB_GR_REQUEST_FAILURE		103									//����ʧ��
#define SUB_GR_USER_GAME_DATA		104									//�û���Ϸ����
#define SUB_GR_USER_HALL_SCORE		105									//���˳���Ϸ���û�ˢ�½��

//��������
#define SUB_GR_USER_CHAT			201									//������Ϣ
#define SUB_GR_USER_EXPRESSION		202									//������Ϣ
#define SUB_GR_WISPER_CHAT			203									//˽����Ϣ
#define SUB_GR_WISPER_EXPRESSION	204									//˽�ı���
#define SUB_GR_COLLOQUY_CHAT		205									//�Ự��Ϣ
#define SUB_GR_COLLOQUY_EXPRESSION	206									//�Ự����

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//�Թ�����
struct CMD_GR_UserLookon
{
	WORD							wTableID;							//����λ��
	WORD							wChairID;							//����λ��
};

//��������
struct CMD_GR_UserSitDown
{
	WORD							wTableID;							//����λ��
	WORD							wChairID;							//����λ��
	TCHAR							szPassword[LEN_PASSWORD];			//��������
};

//��������
struct CMD_GR_UserStandUp
{
	WORD							wTableID;							//����λ��
	WORD							wChairID;							//����λ��
	BYTE							cbForceLeave;						//ǿ���뿪
};

//�����û� 
struct CMD_GR_UserInvite
{
	WORD							wTableID;							//���Ӻ���
	DWORD							dwUserID;							//�û� I D
};

//�����û����� 
struct CMD_GR_UserInviteReq
{
	WORD							wTableID;							//���Ӻ���
	DWORD							dwUserID;							//�û� I D
};

//�û�����
struct CMD_GR_UserScore
{
	DWORD							dwUserID;							//�û���ʶ
	tagUserScore					UserScore;							//������Ϣ
};

//�û�����
struct CMD_GR_MobileUserScore
{
	DWORD							dwUserID;							//�û���ʶ
	tagMobileUserScore				UserScore;							//������Ϣ
};

//�˳���Ϸ�û�����
struct CMD_GR_UserHallScore
{
	DWORD							dwUserID;							//�û���ʶ
	SCORE							lUserScore;							//������Ϣ
};

//�û�״̬
struct CMD_GR_UserStatus
{
	DWORD							dwUserID;							//�û���ʶ
	tagUserStatus					UserStatus;							//�û�״̬
};

//����ʧ��
struct CMD_GR_RequestFailure
{
	LONG							lErrorCode;							//�������
	TCHAR							szDescribeString[256];				//������Ϣ
};


//�û�����
struct CMD_GR_C_UserChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�û�����
struct CMD_GR_S_UserChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�û�����
struct CMD_GR_C_UserExpression
{
	WORD							wItemIndex;							//��������
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�û�����
struct CMD_GR_S_UserExpression
{
	WORD							wItemIndex;							//��������
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�û�˽��
struct CMD_GR_C_WisperChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�û�˽��
struct CMD_GR_S_WisperChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//˽�ı���
struct CMD_GR_C_WisperExpression
{
	WORD							wItemIndex;							//��������
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//˽�ı���
struct CMD_GR_S_WisperExpression
{
	WORD							wItemIndex;							//��������
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�û��Ự
struct CMD_GR_ColloquyChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwConversationID;					//�Ự��ʶ
	DWORD							dwTargetUserID[16];					//Ŀ���û�
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�����û�
struct CMD_GR_C_InviteUser
{
	WORD							wTableID;							//���Ӻ���
	DWORD							dwSendUserID;						//�����û�
};

//�����û�
struct CMD_GR_S_InviteUser
{
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�û��ܾ�����������
struct CMD_GR_UserRepulseSit
{
	WORD							wTableID;							//���Ӻ���
	WORD							wChairID;							//����λ��
	DWORD							dwUserID;							//�û� I D
	DWORD							dwRepulseUserID;					//�û� I D
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//�����־
#define UR_LIMIT_SAME_IP			0x01								//���Ƶ�ַ
#define UR_LIMIT_WIN_RATE			0x02								//����ʤ��
#define UR_LIMIT_FLEE_RATE			0x04								//��������
#define UR_LIMIT_GAME_SCORE			0x08								//���ƻ���

//�û�����
struct CMD_GR_UserRule
{
	BYTE							cbRuleMask;							//��������
	WORD							wMinWinRate;						//���ʤ��
	WORD							wMaxFleeRate;						//�������
	LONG							lMaxGameScore;						//��߷��� 
	LONG							lMinGameScore;						//��ͷ���
};

//�����û���Ϣ
struct CMD_GR_UserInfoReq
{
	DWORD                           dwUserIDReq;                        //�����û�
	WORD							wTablePos;							//����λ��
};

//�����û���Ϣ
struct CMD_GR_ChairUserInfoReq
{
	WORD							wTableID;							//���Ӻ���
	WORD							wChairID;							//����λ��
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//״̬����

#define MDM_GR_STATUS				4									//״̬��Ϣ

#define SUB_GR_TABLE_INFO			100									//������Ϣ
#define SUB_GR_TABLE_STATUS			101									//����״̬

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//������Ϣ
struct CMD_GR_TableInfo
{
	WORD							wTableCount;						//������Ŀ
	tagTableStatus					TableStatusArray[512];				//����״̬
};

//����״̬
struct CMD_GR_TableStatus
{
	WORD							wTableID;							//���Ӻ���
	tagTableStatus					TableStatus;						//����״̬
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//��������

#define MDM_GR_INSURE				5									//�û���Ϣ

//��������
#define SUB_GR_ENABLE_INSURE_REQUEST 1									//��ͨ����
#define SUB_GR_QUERY_INSURE_INFO	2									//��ѯ����
#define SUB_GR_SAVE_SCORE_REQUEST	3									//������
#define SUB_GR_TAKE_SCORE_REQUEST	4									//ȡ�����
#define SUB_GR_TRANSFER_SCORE_REQUEST	5								//ȡ�����
#define SUB_GR_QUERY_USER_INFO_REQUEST	6								//��ѯ�û�

#define SUB_GR_USER_INSURE_INFO		100									//��������
#define SUB_GR_USER_INSURE_SUCCESS	101									//���гɹ�
#define SUB_GR_USER_INSURE_FAILURE	102									//����ʧ��
#define SUB_GR_USER_TRANSFER_USER_INFO	103								//�û�����
#define SUB_GR_USER_INSURE_ENABLE_RESULT 104							//��ͨ���

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//��ͨ����
struct CMD_GR_C_EnableInsureRequest
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwUserID;							//�û�I D
	TCHAR							szLogonPass[LEN_PASSWORD];			//��¼����
	TCHAR							szInsurePass[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ѯ����
struct CMD_GR_C_QueryInsureInfoRequest
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	TCHAR							szInsurePass[LEN_PASSWORD];			//��������
};

//�������
struct CMD_GR_C_SaveScoreRequest
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	SCORE							lSaveScore;							//�����Ŀ
};

//ȡ������
struct CMD_GR_C_TakeScoreRequest
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	SCORE							lTakeScore;							//ȡ����Ŀ
	TCHAR							szInsurePass[LEN_PASSWORD];			//��������
};

//ת�˽��
struct CMD_GP_C_TransferScoreRequest
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	SCORE							lTransferScore;						//ת�˽��
	TCHAR							szAccounts[LEN_ACCOUNTS];			//Ŀ���û�
	TCHAR							szInsurePass[LEN_PASSWORD];			//��������
	TCHAR							szTransRemark[LEN_TRANS_REMARK];	//ת�˱�ע
};

//��ѯ�û�
struct CMD_GR_C_QueryUserInfoRequest
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	BYTE                            cbByNickName;                       //�ǳ�����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//Ŀ���û�
};

//��������
struct CMD_GR_S_UserInsureInfo
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	BYTE							cbEnjoinTransfer;					//ת�˿���
	WORD							wRevenueTake;						//˰�ձ���
	WORD							wRevenueTransfer;					//˰�ձ���
	WORD							wRevenueTransferMember;				//˰�ձ���	
	WORD							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
	SCORE							lTransferPrerequisite;				//ת������
};

//���гɹ�
struct CMD_GR_S_UserInsureSuccess
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	SCORE							lUserScore;							//���Ͻ��
	SCORE							lUserInsure;						//���н��
	TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct CMD_GR_S_UserInsureFailure
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	LONG							lErrorCode;							//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�û���Ϣ
struct CMD_GR_S_UserTransferUserInfo
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwTargetGameID;						//Ŀ���û�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//Ŀ���û�
};

//��ͨ���
struct CMD_GR_S_UserInsureEnableResult
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	BYTE							cbInsureEnabled;					//ʹ�ܱ�ʶ
	TCHAR							szDescribeString[128];				//������Ϣ
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//��������

#define MDM_GR_MANAGE				9									//��������

#define SUB_GR_SEND_WARNING			1									//���;���
#define SUB_GR_SEND_MESSAGE			2									//������Ϣ
#define SUB_GR_LOOK_USER_IP			3									//�鿴��ַ
#define SUB_GR_KILL_USER			4									//�߳��û�
#define SUB_GR_LIMIT_ACCOUNS		5									//�����ʻ�
#define SUB_GR_SET_USER_RIGHT		6									//Ȩ������

//��������
#define SUB_GR_QUERY_OPTION			7									//��ѯ����
#define SUB_GR_OPTION_SERVER		8									//��������
#define SUB_GR_OPTION_CURRENT		9									//��ǰ����
#define SUB_GR_LIMIT_USER_CHAT		10									//��������
#define SUB_GR_KICK_ALL_USER		11									//�߳��û�
#define SUB_GR_DISMISSGAME		    12									//��ɢ��Ϸ

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//���;���
struct CMD_GR_SendWarning
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szWarningMessage[LEN_USER_CHAT];	//������Ϣ
};

//ϵͳ��Ϣ
struct CMD_GR_SendMessage
{
	BYTE							cbGame;								//�Ƿ���Ϣ�㲥����Ϸ
	BYTE							cbRoom;								//�Ƿ���Ϣ�㲥�����䣨���ӽ��棩
	BYTE							cbAllRoom;							//�Ƿ���Ϣ�㲥��������Ϸ
	BYTE                            cbLoop;                             //ѭ����־
	DWORD                           dwTimeRate;                         //ѭ�����
	__time64_t                      tConcludeTime;                      //����ʱ��
	WORD							wChatLength;						//��Ϣ����
	TCHAR							szSystemMessage[LEN_USER_CHAT];		//ϵͳ��Ϣ
};

//�鿴��ַ
struct CMD_GR_LookUserIP
{
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�߳��û�
struct CMD_GR_KickUser
{
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�����ʻ�
struct CMD_GR_LimitAccounts
{
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//Ȩ������
struct CMD_GR_SetUserRight
{
	//Ŀ���û�
	DWORD							dwTargetUserID;						//Ŀ���û�

	//�󶨱���
	BYTE							cbGameRight;						//�ʺ�Ȩ��
	BYTE							cbAccountsRight;					//�ʺ�Ȩ��

	//Ȩ�ޱ仯
	BYTE							cbLimitRoomChat;					//��������
	BYTE							cbLimitGameChat;					//��Ϸ����
	BYTE							cbLimitPlayGame;					//��ϷȨ��
	BYTE							cbLimitSendWisper;					//������Ϣ
	BYTE							cbLimitLookonGame;					//�Թ�Ȩ��
};

//��������
struct CMD_GR_OptionCurrent
{
	DWORD							dwRuleMask;							//��������
	tagServerOptionInfo				ServerOptionInfo;					//��������
};

//��������
struct CMD_GR_ServerOption
{
	tagServerOptionInfo				ServerOptionInfo;					//��������
};

//�߳������û�
struct CMD_GR_KickAllUser
{
	TCHAR							szKickMessage[LEN_USER_CHAT];		//�߳���ʾ
};

//��ɢ��Ϸ
struct CMD_GR_DismissGame
{
	WORD							wDismissTableNum;		            //��ɢ����
};
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//���ñ�־
#define OSF_ROOM_CHAT				1									//��������
#define OSF_GAME_CHAT				2									//��Ϸ����
#define OSF_ROOM_WISPER				3									//����˽��
#define OSF_ENTER_TABLE				4									//������Ϸ
#define OSF_ENTER_SERVER			5									//���뷿��

//��������
struct CMD_GR_OptionServer
{
	BYTE							cbOptionFlags;						//���ñ�־
	BYTE							cbOptionValue;						//���ñ�־
};

//��������
struct CMD_GR_LimitUserChat
{
	DWORD							dwTargetUserID;						//Ŀ���û�
	BYTE							cbLimitFlags;						//���Ʊ�־
	BYTE							cbLimitValue;						//�������
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//��������

#define MDM_GR_MATCH				10									//��������

#define SUB_GR_MATCH_INFO			1									//������Ϣ	
#define SUB_GR_MATCH_RULE			2									//��������
#define SUB_GR_MATCH_FEE			3									//��������
#define SUB_GR_MATCH_NUM			4									//�ȴ�����
#define SUB_GR_LEAVE_MATCH			5									//�˳�����
#define SUB_GR_MATCH_SAFECARD		6									//��������
#define SUB_GR_MATCH_BUY_SAFECARD	7									//������
#define SUB_GR_MATCH_WAIT_TIP		8									//�ȴ���ʾ
#define SUB_GR_MATCH_RESULT			9									//�������
#define SUB_GR_MATCH_STATUS			10									//����״̬
#define SUB_GR_MATCH_USTATUS		11									//����״̬	
#define SUB_GR_MATCH_DESC			12									//��������
#define SUB_GR_MATCH_GOLDUPDATE		13									//��Ҹ���
#define SUB_GR_MATCH_ELIMINATE		14									//������̭
#define SUB_GR_MATCH_REVIVE			15									//��������
#define SUB_GR_MATCH_USER_REVIVE	16									//�û�����


/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661//////

//��������
struct CMD_GR_Match_Rule
{
	//������Ϣ
	DWORD							dwMatchID;							//������ʶ	
	BYTE							cbMatchType;						//��������
	TCHAR							szMatchName[32];					//��������

	//������Ϣ
	BYTE							cbFeeType;							//��������
	BYTE							cbDeductArea;						//�ɷ�����
	LONGLONG						lSignupFee;							//��������	
	BYTE							cbSignupMode;						//������ʽ
	BYTE							cbJoinCondition;					//��������
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	DWORD							dwExperience;						//��Ҿ���

	//������ʽ
	BYTE							cbRankingMode;						//������ʽ	
	WORD							wCountInnings;						//ͳ�ƾ���
	BYTE							cbFilterGradesMode;					//ɸѡ��ʽ
	
	//������Ϣ
	WORD							wRewardCount;						//��������
	tagMatchRewardInfo				MatchRewardInfo[3];					//��������
	//��������
	BYTE							cbMatchRule[512];					//��������
};

//��������
struct CMD_GR_Match_Revive
{
	//������Ϣ
	DWORD							dwMatchID;							//������ʶ
	LONGLONG						lMatchNO;							//��������

	//������Ϣ
	LONGLONG						lReviveFee;							//�������
	BYTE							cbReviveTimes;						//�������
	BYTE							cbReviveTimesed;					//�������
	TCHAR							szNotifyContent[128];				//��ʾ����
};

//��������
struct CMD_GR_Match_SafeCard
{
	//������Ϣ
	DWORD							dwMatchID;							//������ʶ
	LONGLONG						lMatchNO;							//��������

	//���շ���
	LONGLONG						lSafeCardFee;						//���շ���
	TCHAR							szNotifyContent[128];				//��ʾ����
};

//������
struct CMD_GR_Match_BuySafeCard
{
	bool							bBuySafeCard;						//�����ʶ
};

//��������
struct CMD_GR_Match_Fee
{
	//������Ϣ
	DWORD							dwMatchID;							//������ʶ
	LONGLONG						lMatchNO;							//��������

	//������Ϣ
	BYTE							cbSignupMode;						//������ʽ
	BYTE							cbFeeType;							//��������
	BYTE							cbDeductArea;						//�۷�����
	LONGLONG						lSignupFee;							//��������
	TCHAR							szNotifyContent[128];				//��ʾ����
};

//��������
struct CMD_GR_Match_Num
{
	DWORD							dwWaitting;							//�ȴ�����
	DWORD							dwTotal;							//��������
};

//������Ϣ
struct CMD_GR_Match_Info
{
	TCHAR							szTitle[4][64];						//��Ϣ����
    WORD							wGameCount;							//��Ϸ����
};

//��ʾ��Ϣ
struct CMD_GR_Match_Wait_Tip
{
	LONGLONG						lScore;								//��ǰ����
	WORD							wRank;								//��ǰ����
	WORD							wCurTableRank;						//��������
	WORD							wUserCount;							//��ǰ����
	WORD							wCurGameCount;						//��ǰ����
	WORD							wGameCount;							//�ܹ�����
	WORD							wPlayingTable;						//��Ϸ����
	TCHAR							szMatchName[LEN_SERVER];			//��������
};

//�������
struct CMD_GR_MatchResult
{	
	//������Ϣ
	SCORE							lGold;								//��ҽ���
	SCORE							lIngot;								//Ԫ������
	DWORD							dwExperience;						//���齱��
	TCHAR							szNickName[LEN_ACCOUNTS];			//����ǳ�

	//������Ϣ
	WORD							wMatchRank;							//��������
	TCHAR							szMatchName[64];					//��������	
	SYSTEMTIME						MatchEndTime;						//����ʱ��	
};

#define MAX_MATCH_DESC				4									//�������

//��������
struct CMD_GR_MatchDesc
{
	TCHAR							szTitle[MAX_MATCH_DESC][16];		//��Ϣ����
	TCHAR							szDescribe[MAX_MATCH_DESC][64];		//��������
	COLORREF						crTitleColor;						//������ɫ
	COLORREF						crDescribeColor;					//������ɫ
};

//��Ҹ���
struct CMD_GR_MatchGoldUpdate
{
	LONGLONG						lCurrGold;							//��ǰ���
};

//�û�����
struct CMD_GR_Match_UserRevive
{
	bool							bUseSafeCard;						//ʹ�ñ��տ�
};

//////////////////////////////////////////////////////////////////////////

//�ͱ�����
#define MDM_GR_BASEENSURE				11

#define SUB_GR_BASEENSURE_LOAD			260								//���صͱ�
#define SUB_GR_BASEENSURE_TAKE			261								//��ȡ�ͱ�
#define SUB_GR_BASEENSURE_PARAMETER		262								//�ͱ�����
#define SUB_GR_BASEENSURE_RESULT		263								//�ͱ����

//��ȡ�ͱ�
struct CMD_GR_BaseEnsureTake
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�ͱ�����
struct CMD_GR_BaseEnsureParamter
{
	SCORE							lScoreCondition;					//��Ϸ������
	SCORE							lScoreAmount;						//��Ϸ������
	BYTE							cbTakeTimes;						//��ȡ����	
};

//�ͱ����
struct CMD_GR_BaseEnsureResult
{
	bool							bSuccessed;							//�ɹ���ʶ
	SCORE							lGameScore;							//��ǰ��Ϸ��
	TCHAR							szNotifyContent[128];				//��ʾ����
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//�������

#define MDM_GF_FRAME				100									//�������

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//�������

//�û�����
#define SUB_GF_GAME_OPTION			1									//��Ϸ����
#define SUB_GF_USER_READY			2									//�û�׼��
#define SUB_GF_LOOKON_CONFIG		3									//�Թ�����
#define SUB_GF_ANDEOID_QUIT			4									//�������˳�
//��������
#define SUB_GF_USER_CHAT			10									//�û�����
#define SUB_GF_USER_EXPRESSION		11									//�û�����
#define SUB_GF_USER_VOICE			12									//�û�����

//��Ϸ��Ϣ
#define SUB_GF_GAME_STATUS			100									//��Ϸ״̬
#define SUB_GF_GAME_SCENE			101									//��Ϸ����
#define SUB_GF_LOOKON_STATUS		102									//�Թ�״̬
#define SUB_GF_USER_DATA			103									//�������

#define SUB_GF_BJL_GAME_STATUS		2001								//�ټ��ִ����������з�����Ϸ״̬������ʷ��¼��
#define SUB_GF_BJL_GAME_RESULT		2002								//�ټ���֪ͨ������ҵ�����Ϸ���
#define SUB_GF_BJL_CHANGE_STATUS	2003								//�ټ���֪ͨ���������Ϸ״̬�ı�
#define SUB_GF_BAIREN_STATUS_END	2004								//�����ȡ�б����

//ϵͳ��Ϣ
#define SUB_GF_SYSTEM_MESSAGE		200									//ϵͳ��Ϣ
#define SUB_GF_ACTION_MESSAGE		201									//������Ϣ


/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//��Ϸ����
struct CMD_GF_GameOption
{
	BYTE							cbAllowLookon;						//�Թ۱�־
	DWORD							dwFrameVersion;						//��ܰ汾
	DWORD							dwClientVersion;					//��Ϸ�汾
};

//�Թ�����
struct CMD_GF_LookonConfig
{
	DWORD							dwUserID;							//�û���ʶ
	BYTE							cbAllowLookon;						//�����Թ�
};

//��Ϸ״̬�л�
struct tagChangeStatus
{
	WORD							wTableID;							//����ID
	BYTE							cbGameStatus;						//��Ϸ״̬
};

//�ټ��ּ�¼��Ϣ
struct tagBjlGameRecord
{
	BYTE							bAreaWin[5];						//����ʤ����ʶ
};

//�ټ��ֵ��μ�¼
struct tagBjlPerGameRecord
{
	WORD							wTableID;							//����ID
	BYTE							bAreaWin[5];						//����ʤ����ʶ
};

//����ţţʱ����Ϣ
struct tagBjlTimeInfo
{
	BYTE							cbFreeTime;							//����ʱ��
	BYTE							cbBetTime;							//��עʱ��
	BYTE							cbEndTime;							//����ʱ��
	BYTE							cbPassTime;							//�Ѿ����˶���ʱ��

	SCORE							lMinXianHong;						//��С�޺�
	SCORE							lMaxXianHong;						//����޺�
};

//�ټ��ַ���״̬
struct CMD_GF_BjlRoomStatus
{
	tagChangeStatus					tagGameInfo;						//������Ϣ
	tagBjlTimeInfo					tagTimeInfo;						//ʱ����Ϣ
	tagBjlGameRecord				GameRecordArrary[48];				//·����¼
	BYTE							cbRecordCount;						//��¼����
};

//����ţţ��¼��Ϣ
struct tagOxGameRecord
{
	BYTE							bAreaWin[4];						//����ʤ����ʶ
};

//����ţţ���ּ�¼��Ϣ
struct tagOxPerGameRecord
{
	WORD							wTableID;							//����ID
	BYTE							bAreaWin[4];						//����ʤ����ʶ
};

//����ţţʱ����Ϣ
struct tagOxTimeInfo
{
	BYTE							cbFreeTime;							//����ʱ��
	BYTE							cbBetTime;							//��עʱ��
	BYTE							cbEndTime;							//����ʱ��
	BYTE							cbPassTime;							//�Ѿ����˶���ʱ��

	SCORE							lMinXianHong;						//��С�޺�
	SCORE							lMaxXianHong;						//����޺�
};

//����ţţ����״̬
struct CMD_GF_OxRoomStatus
{
	tagChangeStatus					tagGameInfo;						//������Ϣ
	tagOxTimeInfo					tagTimeInfo;						//ʱ����Ϣ
	tagOxGameRecord					GameRecordArrary[20];				//·����¼
	BYTE							cbRecordCount;						//��¼����
	BYTE							cbMultiple;							//�����
};

//��ڼ�¼��Ϣ
struct tagRBGameRecord
{
	BYTE							cbAreaWin[4];							//����ʤ����ʶ
};

//��ڵ��ּ�¼��Ϣ
struct tagRBPerGameRecord
{
	WORD							wTableID;
	BYTE							cbAreaWin[4];							//����ʤ����ʶ
};

//���ʱ����Ϣ
struct tagRBTimeInfo
{
	BYTE							cbBetTime;							//��עʱ��
	BYTE							cbEndTime;							//����ʱ��
	BYTE							cbPassTime;							//�Ѿ����˶���ʱ��

	SCORE							lMinXianHong;						//��С�޺�
	SCORE							lMaxXianHong;						//����޺�
};

//��ڷ���״̬
struct CMD_GF_RBRoomStatus
{
	tagChangeStatus					tagGameInfo;						//������Ϣ
	tagRBTimeInfo					tagTimeInfo;						//ʱ����Ϣ
	tagRBGameRecord					GameRecordArrary[48];				//·����¼
	BYTE							cbRecordCount;						//��¼����
};

//������¼��Ϣ
struct tagDTGameRecord
{
	BYTE							bAreaWin[3];
};

//������¼��Ϣ
struct tagDTPerGameRecord
{
	WORD							wTableID;
	BYTE							bAreaWin[3];
};

//����ʱ����Ϣ
struct tagDTTimeInfo
{
	BYTE							cbFreeTime;							//����ʱ��
	BYTE							cbBetTime;							//��עʱ��
	BYTE							cbEndTime;							//����ʱ��
	BYTE							cbPassTime;							//�Ѿ����˶���ʱ��

	SCORE							lMinXianHong;						//��С�޺�
	SCORE							lMaxXianHong;						//����޺�
};

//��������״̬
struct CMD_GF_DTRoomStatus
{
	tagChangeStatus					tagGameInfo;						//������Ϣ
	tagDTTimeInfo					tagTimeInfo;						//ʱ����Ϣ
	tagDTGameRecord					GameRecordArrary[48];				//·����¼
	BYTE							cbRecordCount;						//��¼����
};

//�������޼�¼��Ϣ
struct tagAnGameRecord
{
	BYTE							cbAnimal;							//����ʤ��
};

//�������޼�¼��Ϣ
struct tagAnPerGameRecord
{
	WORD							wTableID;
	BYTE							cbAnimal;							//����ʤ��
};

//��������ʱ����Ϣ
struct tagAnTimeInfo
{
	BYTE							cbFreeTime;							//����ʱ��
	BYTE							cbBetTime;							//��עʱ��
	BYTE							cbEndTime;							//����ʱ��
	BYTE							cbPassTime;							//�Ѿ����˶���ʱ��

	SCORE							lMinXianHong;						//��С�޺�
	SCORE							lMaxXianHong;						//����޺�
};

//�������޷���״̬
struct CMD_GF_AnRoomStatus
{
	tagChangeStatus					tagGameInfo;						//������Ϣ
	tagAnTimeInfo					tagTimeInfo;						//ʱ����Ϣ
	tagAnGameRecord					GameRecordArrary[40];				//·����¼
	BYTE							cbRecordCount;						//��¼����
};

//���۱����¼��Ϣ
struct tagZoGameRecord
{
	BYTE							cbAnimal;							//����ʤ��
};

//���۱����¼��Ϣ
struct tagZoPerGameRecord
{
	WORD							wTableID;
	BYTE							cbAnimal;							//����ʤ��
};

//���۱���ʱ����Ϣ
struct tagZoTimeInfo
{
	BYTE							cbFreeTime;							//����ʱ��
	BYTE							cbBetTime;							//��עʱ��
	BYTE							cbEndTime;							//����ʱ��
	BYTE							cbPassTime;							//�Ѿ����˶���ʱ��

	SCORE							lMinXianHong;						//��С�޺�
	SCORE							lMaxXianHong;						//����޺�
};

//���۱�����״̬
struct CMD_GF_ZoRoomStatus
{
	tagChangeStatus					tagGameInfo;						//������Ϣ
	tagZoTimeInfo					tagTimeInfo;						//ʱ����Ϣ
	tagZoGameRecord					GameRecordArrary[40];				//·����¼
	BYTE							cbRecordCount;						//��¼����
};

//���̼�¼��Ϣ
struct tagRoletteGameRecord
{
	BYTE							cbResult;							//�������
};

//���̼�¼��Ϣ
struct tagRolettePerGameRecord
{
	WORD							wTableID;
	BYTE							cbResult;							//�������
};

//����ʱ����Ϣ
struct tagRoletteTimeInfo
{
	BYTE							cbBetTime;							//��עʱ��
	BYTE							cbEndTime;							//����ʱ��
	BYTE							cbPassTime;							//�Ѿ����˶���ʱ��
};

//���̷���״̬
struct CMD_GF_RoletteRoomStatus
{
	tagChangeStatus					tagGameInfo;						//������Ϣ
	tagRoletteTimeInfo				tagTimeInfo;						//ʱ����Ϣ
	tagRoletteGameRecord			GameRecordArrary[72];				//·����¼
	BYTE							cbRecordCount;						//��¼����
};

//������¼��Ϣ
struct tagSicBoGameRecord
{
	BYTE							cbResult[3];						//�������
	BYTE							cbIsBig;							//�Ƿ�Ϊ��  0С 1 ��  2����
};

//������¼��Ϣ
struct tagSicBoPerGameRecord
{
	WORD							wTableID;
	BYTE							cbResult[3];						//�������
	BYTE							cbIsBig;							//�Ƿ�Ϊ��  0С 1 ��  2����
};

//����ʱ����Ϣ
struct tagSicBoTimeInfo
{
	BYTE							cbFreeTime;							//����ʱ��
	BYTE							cbBetTime;							//��עʱ��
	BYTE							cbEndTime;							//����ʱ��
	BYTE							cbPassTime;							//�Ѿ����˶���ʱ��
};

//��������״̬
struct CMD_GF_SicBoRoomStatus
{
	tagChangeStatus					tagGameInfo;						//������Ϣ
	tagSicBoTimeInfo				tagTimeInfo;						//ʱ����Ϣ
	tagSicBoGameRecord				GameRecordArrary[40];				//·����¼
	BYTE							cbRecordCount;						//��¼����
};

//������Ͳ�Ӽ�¼��Ϣ
struct tagTongGameRecord
{
	BYTE							cbAreaWin[3];						//�������
};

//������Ͳ�Ӽ�¼��Ϣ
struct tagTongPerGameRecord
{
	WORD							wTableID;
	BYTE							cbAreaWin[3];						//�������
};

//������Ͳ��ʱ����Ϣ
struct tagTongTimeInfo
{
	BYTE							cbFreeTime;							//����ʱ��
	BYTE							cbBetTime;							//��עʱ��
	BYTE							cbEndTime;							//����ʱ��
	BYTE							cbPassTime;							//�Ѿ����˶���ʱ��
};

//������Ͳ�ӷ���״̬
struct CMD_GF_TongRoomStatus
{
	tagChangeStatus					tagGameInfo;						//������Ϣ
	tagTongTimeInfo					tagTimeInfo;						//ʱ����Ϣ
	tagTongGameRecord				GameRecordArrary[20];				//·����¼
	BYTE							cbRecordCount;						//��¼����
};

//�Թ�״̬
struct CMD_GF_LookonStatus
{
	BYTE							cbAllowLookon;						//�����Թ�
};

//��Ϸ����
struct CMD_GF_GameStatus
{
	BYTE							cbGameStatus;						//��Ϸ״̬
	BYTE							cbAllowLookon;						//�Թ۱�־
};
//��Ϸ����
struct CMD_GF_GameUserData
{
	DWORD							cbUserCharID;						//��Ϸ״̬
};
//�û�����
struct CMD_GF_C_UserChat
{
	WORD							wItemIndex;							//��������
};

//�û�����
struct CMD_GF_S_UserChat
{
	DWORD							dwSendUserID;						//�����û�
	WORD							wItemIndex;							//��������
};

//�û�����
struct CMD_GF_C_UserExpression
{
	WORD							wItemIndex;							//��������
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�û�����
struct CMD_GF_S_UserExpression
{
	WORD							wItemIndex;							//��������
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�û�����
struct CMD_GF_C_UserVoice
{
	DWORD							dwTargetUserID;						//Ŀ���û�
	DWORD							dwVoiceLength;						//��������
	BYTE							byVoiceData[MAXT_VOICE_LENGTH];		//��������
};

//�û�����
struct CMD_GF_S_UserVoice
{
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
	DWORD							dwVoiceLength;						//��������
	BYTE							byVoiceData[MAXT_VOICE_LENGTH];		//��������
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//��Ϸ����

#define MDM_GF_GAME					200									//��Ϸ����

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//Я����Ϣ

//������Ϣ
#define DTP_GR_TABLE_PASSWORD		1									//��������

//�û�����
#define DTP_GR_NICK_NAME			10									//�û��ǳ�
#define DTP_GR_GROUP_NAME			11									//��������
#define DTP_GR_UNDER_WRITE			12									//����ǩ��
#define DTP_GR_QQ					13									//QQ����
#define DTP_GR_MOBILE_PHONE			14									//�ֻ�����

//������Ϣ
#define DTP_GR_USER_NOTE			20									//�û���ע
#define DTP_GR_CUSTOM_FACE			21									//�Զ�ͷ��

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//�������
#define REQUEST_FAILURE_NORMAL		0									//����ԭ��
#define REQUEST_FAILURE_NOGOLD		1									//��Ҳ���
#define REQUEST_FAILURE_NOSCORE		2									//���ֲ���
#define REQUEST_FAILURE_PASSWORD	3									//�������

//��Ϸ�������Ϣ
struct CMD_GR_GameRadioMessage
{
	WORD					wKindID;							//��Ϸ
	BYTE					cbCardType;							//����
	SCORE					lScore;								//���
	TCHAR					szNickName[32];						//����ǳ�
};
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

#pragma pack()

#endif