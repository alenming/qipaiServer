#ifndef STRUCT_HEAD_FILE
#define STRUCT_HEAD_FILE


#pragma pack(1)

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//��Ϸ�б�

//��Ϸ����
struct tagGameType
{
	WORD							wJoinID;							//�ҽ�����
	WORD							wSortID;							//��������
	WORD							wTypeID;							//��������
	TCHAR							szTypeName[LEN_TYPE];				//��������
};

//��Ϸ����
struct tagGameKind
{
	WORD							wTypeID;							//��������
	WORD							wJoinID;							//�ҽ�����
	WORD							wSortID;							//��������
	WORD							wKindID;							//��������
	WORD							wGameID;							//ģ������
	WORD							wRecommend;							//�Ƽ���Ϸ
	WORD							wGameFlag;							//��Ϸ��־
	DWORD							dwOnLineCount;						//��������
	DWORD							dwAndroidCount;						//��������
	DWORD							dwDummyCount;						//��������
	DWORD							dwFullCount;						//��Ա����
	DWORD							dwSuportType;						//֧������
	TCHAR							szKindName[LEN_KIND];				//��Ϸ����
	TCHAR							szProcessName[LEN_PROCESS];			//��������
};

//��Ϸ�ڵ�
struct tagGameNode
{
	WORD							wKindID;							//��������
	WORD							wJoinID;							//�ҽ�����
	WORD							wSortID;							//��������
	WORD							wNodeID;							//�ڵ�����
	TCHAR							szNodeName[LEN_NODE];				//�ڵ�����
};

//��������
struct tagGamePage
{
	WORD							wPageID;							//ҳ������
	WORD							wKindID;							//��������
	WORD							wNodeID;							//�ڵ�����
	WORD							wSortID;							//��������
	WORD							wOperateType;						//��������
	TCHAR							szDisplayName[LEN_PAGE];			//��ʾ����
};

//��Ϸ����
struct tagGameServer
{
	WORD							wKindID;							//��������
	WORD							wNodeID;							//�ڵ�����
	WORD							wSortID;							//��������
	WORD							wServerID;							//��������
	WORD                            wServerKind;                        //��������
	WORD							wServerType;						//��������
	WORD							wServerLevel;						//����ȼ�
	WORD							wServerPort;						//����˿�
	SCORE							lCellScore;							//��Ԫ����
	BYTE							cbEnterMember;						//�����Ա
	SCORE							lEnterScore;						//�������
	SCORE							lTableScore;						//������Ϸ����
	DWORD							dwServerRule;						//�������
	DWORD							dwOnLineCount;						//��������
	DWORD							dwAndroidCount;						//��������
	DWORD							dwFullCount;						//��Ա����
	TCHAR							szServerAddr[32];					//��������
	TCHAR							szServerName[LEN_SERVER];			//��������
	//˽�˷����
	DWORD							dwSurportType;						//֧������
	WORD							wTableCount;						//������Ŀ
	DWORD							dwDummyCount;						//��������
};

//�����б�
struct tagAgentGameKind
{
	WORD							wKindID;							//��������
	WORD							wSortID;							//��������
};

//��ʱ��
struct tagLockTimeMatch
{
	//ʱ������
	DWORD							wEndDelayTime;						//�ӳ�ʱ��
	SYSTEMTIME						MatchStartTime;						//��ʼ����
	SYSTEMTIME						MatchEndTime;						//��������

	//��������
	BYTE							cbMatchMode;						//����ģʽ
	WORD							wMaxSignupUser;						//��������
	WORD							wStartUserCount;					//��������	
	SYSTEMTIME						SignupStartTime;					//������ʼ
	SYSTEMTIME						SignupEndTime;						//������ֹ	

	//��������
	LONGLONG						lMatchInitScore;					//��ʼ����
	LONGLONG						lMatchCullScore;					//��̭����

	//��������
	WORD							wMinPlayCount;						//���پ���
	WORD							wMaxPlayCount;						//������

	//��������
	BYTE							cbReviveEnabled;					//���ñ�ʶ
	BYTE							cbSafeCardEnabled;					//���ñ�ʶ
	BYTE							cbReviveTimes;						//�������
	LONGLONG						lReviveFee;							//�������
	LONGLONG						lSafeCardFee;						//���շ���
};

//��ʱ��
struct tagImmediateMatch
{
	//������Ϣ
	WORD							wStartUserCount;					//��������	
	WORD							wAndroidUserCount;					//��������
	WORD							wAndroidDelaySignupTime;			//�ӳٱ���

	//������Ϣ
	LONGLONG						lInitalBase;						//��ʼ����
	LONGLONG						lInitalScore;						//��ʼ����

	//������Ϣ
	WORD							wPlayCount;							//��Ϸ����
	BYTE							cbSwitchTableCount;					//��������(0Ϊ������)
	WORD							wPrecedeTimer;						//��������
};	

//������Ϣ
struct tagMatchRewardInfo
{
	WORD							wRankID;							//��������
	SCORE							lRewardGold;						//�������
	SCORE							lRewardIngot;						//����Ԫ��	
	DWORD							dwRewardExperience;					//��������
};

//������Ϣ
struct tagGameMatch
{
	//������Ϣ
	WORD							wServerID;							//�����ʶ
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
	LONG							lExperience;						//��Ҿ���

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

//������Ϣ
struct tagChatServer
{
	WORD							wChatID;							//�㳡��ʶ
	DWORD							dwSocketID;							//�����ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//��Ƶ����
struct tagAVServerOption
{
	WORD							wAVServerPort;						//��Ƶ�˿�
	DWORD							dwAVServerAddr;						//��Ƶ��ַ
};

//������Ϣ
struct tagOnLineInfoKind
{
	WORD							wKindID;							//���ͱ�ʶ
	DWORD							dwOnLineCount;						//��������
};

//����ͳ��
struct tagOnLineInfoKindEx
{
	WORD							wKindID;							//���ͱ�ʶ
	DWORD							dwOnLineCount;						//��������
	DWORD							dwAndroidCount;						//��������
};

//������Ϣ
struct tagOnLineInfoServer
{
	WORD							wServerID;							//�����ʶ
	DWORD							dwOnLineCount;						//��������
};

//��������
struct tagLotteryItem
{
	BYTE							cbItemIndex;						//��������
	BYTE							cbItemType;							//��������
	SCORE							lItemQuota;							//�������
};
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//��������
struct tagAndroidParameter
{
	//���Ա���
	DWORD							dwBatchID;							//���α�ʶ
	DWORD							dwServiceMode;						//����ģʽ
	DWORD							dwAndroidCount;						//������Ŀ
	DWORD							dwEnterTime;						//����ʱ��
	DWORD							dwLeaveTime;						//�뿪ʱ��
	SCORE							lTakeMinScore;						//Я������
	SCORE							lTakeMaxScore;						//Я������
	DWORD							dwEnterMinInterval;					//������
	DWORD							dwEnterMaxInterval;					//������
	DWORD							dwLeaveMinInterval;					//�뿪���
	DWORD							dwLeaveMaxInterval;					//�뿪���	
	DWORD							dwSwitchMinInnings;					//��������
	DWORD							dwSwitchMaxInnings;					//��������
	DWORD							AndroidCountMember0;				//��ͨ��Ա	
	DWORD							AndroidCountMember1;				//һ����Ա
	DWORD							AndroidCountMember2;				//������Ա
	DWORD							AndroidCountMember3;				//������Ա
	DWORD							AndroidCountMember4;				//�ļ���Ա
	DWORD							AndroidCountMember5;				//�弶��Ա	
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//�û���Ϣ

//����״̬
struct tagTableStatus
{
	BYTE							cbTableLock;						//������־
	BYTE							cbPlayStatus;						//��Ϸ��־
	LONG							lCellScore;							//��Ԫ����
};

//�û�״̬
struct tagUserStatus
{
	WORD							wTableID;							//��������
	WORD							wChairID;							//����λ��
	BYTE							cbUserStatus;						//�û�״̬
};

//�û�����
struct tagUserAttrib
{
	BYTE							cbCompanion;						//�û���ϵ
};

//�û�����
struct tagUserScore
{
	//������Ϣ
	SCORE							lScore;								//�û�����
	SCORE							lGrade;								//�û��ɼ�
	SCORE							lInsure;							//�û�����

	//��Ӯ��Ϣ
	DWORD							dwWinCount;							//ʤ������
	DWORD							dwLostCount;						//ʧ������
	DWORD							dwDrawCount;						//�;�����
	DWORD							dwFleeCount;						//��������
};

//�û�����
struct tagMobileUserScore
{
	//������Ϣ
	SCORE							lScore;								//�û�����

	//��Ӯ��Ϣ
	DWORD							dwWinCount;							//ʤ������
	DWORD							dwLostCount;						//ʧ������
	DWORD							dwDrawCount;						//�;�����
	DWORD							dwFleeCount;						//��������
};

//ʱ����Ϣ
struct tagTimeInfo
{
	DWORD						dwEnterTableTimer;						//��������ʱ��
	DWORD						dwLeaveTableTimer;						//�뿪����ʱ��
	DWORD						dwStartGameTimer;						//��ʼ��Ϸʱ��
	DWORD						dwEndGameTimer;							//�뿪��Ϸʱ��
};

//////////////////////////////////////////////////////////////////////////
//������Ϣ

struct tagNearUserInfo
{
	//�û�����
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
	DWORD							dwFaceID;							//ͷ�� I D
	DWORD							dwCustomID;							//ͷ�� I D
	BYTE							cbGender;							//�û��Ա�
	WORD							wMemberOrder;						//��Ա�ȼ�
	WORD							wGrowLevel;							//�û��ȼ�
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��

	//����γ��
	DOUBLE							dLongitude;							//���꾭��
	DOUBLE							dLatitude;							//����γ��
	DWORD							dwDistance;							//Ŀ�����	

	//�û���ַ
	DWORD							dwClientAddr;						//�û���ַ
};

//�û���Ϣ
struct tagFriendUserInfo
{
	//�û�����
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	TCHAR							szAccounts[LEN_ACCOUNTS];			//�û��˺�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	DWORD							dwFaceID;							//ͷ�� I D
	DWORD							dwCustomID;							//ͷ�� I D
	BYTE							cbGender;							//�û��Ա�
	WORD							wMemberOrder;						//��Ա�ȼ�
	WORD							wGrowLevel;							//�û��ȼ�
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����

	//��������
	TCHAR							szQQ[LEN_QQ];						//�û�QQ
	TCHAR							szEMail[LEN_EMAIL];					//�û�Eamil
	TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ֻ�����
	TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//��ϵ��ַ
	TCHAR							szPostalCode[LEN_POSTALCODE];		//��������

	//�û�״̬
	BYTE							cbMainStatus;						//�û�״̬
	BYTE							cbGameStatus;						//��Ϸ״̬
	WORD							wKindID;							//��Ϸ��ʶ
	WORD							wServerID;							//�����ʶ
	WORD							wTableID;							//���ӱ�ʶ
	WORD							wChairID;							//����λ��
	TCHAR							szServerName[LEN_SERVER];			//��������
	TCHAR							szPhoneMode[LEN_PHONE_MODE];		//�ֻ��ͺ�
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������
	DWORD							dwClientAddr;						//�û���ַ

	//����γ��
	BYTE							cbCoordinate;						//������ʶ
	DOUBLE							dLongitude;							//���꾭��
	DOUBLE							dLatitude;							//����γ��
};

//�û�����
struct tagClientGroupInfo
{
	BYTE							cbGroupIndex;						//��������
	TCHAR							szGroupName[GROUP_LEN_NAME];		//��������		
};

//������Ϣ
struct tagServerFriendInfo
{
	DWORD							dwUserID;							//�û���ʶ
	BYTE							cbMainStatus;						//�û�״̬
	BYTE							cbGameStatus;						//�û�״̬
	BYTE							cbGroupID;							//����ʶ	
};

//������Ϣ
struct tagClientFriendInfo
{
	//�û�����
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
	DWORD							dwFaceID;							//ͷ�� I D
	DWORD							dwCustomID;							//ͷ�� I D
	BYTE							cbGender;							//�û��Ա�
	WORD							wMemberOrder;						//�û��ȼ�
	WORD							wGrowLevel;							//�û��ȼ�
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����
	TCHAR							szPhoneMode[LEN_PHONE_MODE];		//�ֻ��ͺ�
	BYTE							cbGroupID;							//����ʶ	

	//������Ϣ
	BYTE							cbMainStatus;						//�û�״̬
	BYTE							cbGameStatus;						//��Ϸ״̬
	WORD							wKindID;							//��Ϸ��ʶ
	WORD							wServerID;							//�����ʶ
	WORD							wTableID;							//���ӱ�ʶ
	WORD							wChairID;							//����λ��
	TCHAR							szServerName[LEN_SERVER];			//��������
};

//�û�����
struct tagUserIndividual
{
	DWORD							dwUserID;							//�û���ʶ
	//��������
	TCHAR							szQQ[LEN_QQ];						//�û�QQ
	TCHAR							szEMail[LEN_EMAIL];					//�û�Eamil
	TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ֻ�����	
	TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//��ϵ��ַ
	TCHAR							szPostalCode[LEN_POSTALCODE];		//��������
};


//��ע��Ϣ
struct tagUserRemarksInfo
{
	DWORD							dwFriendUserID;						//���ѱ�ʶ
	TCHAR							szRemarksInfo[LEN_REMARKS];			//��ע��Ϣ
};

//�Ƹ���Ϣ
struct tagUserWealthInfo
{
	DWORD							dwUserID;							//�û���ʶ
	LONGLONG						lUserGold;							//�û����	
};




//////////////////////////////////////////////////////////////////////////

//�û���Ϣ
struct tagUserInfo
{
	//��������
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwGroupID;							//���� I D
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��
	TCHAR							szQQ[LEN_QQ];						//QQ����
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ֻ�����

	//ͷ����Ϣ
	WORD							wFaceID;							//ͷ������
	DWORD							dwCustomID;							//�Զ���ʶ

	//�û�����
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	BYTE							cbMasterOrder;						//����ȼ�

	//�û�״̬
	WORD							wTableID;							//��������
	WORD							wLastTableID;					    //��Ϸ����
	WORD							wChairID;							//��������
	BYTE							cbUserStatus;						//�û�״̬

	//������Ϣ
	SCORE							lScore;								//�û�����
	SCORE							lGrade;								//�û��ɼ�
	SCORE							lInsure;							//�û�����
	SCORE							lIngot;								//�û�Ԫ��
	DOUBLE							dBeans;								//��Ϸ��
	//˽�˷����
	SCORE							lRoomCard;							//��������

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

	TCHAR							szChangeLogonIP[LEN_IP];			//�޸�IP������

	//ʱ����Ϣ
	tagTimeInfo						TimerInfo;							//ʱ����Ϣ	
};

//�û���Ϣ
struct tagUserInfoHead
{
	//�û�����
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGroupID;							//���� I D

	//ͷ����Ϣ
	WORD							wFaceID;							//ͷ������
	DWORD							dwCustomID;							//�Զ���ʶ

	TCHAR							szNickName[LEN_NICKNAME];			//����ǳ�

	//�û�����
	bool							bIsAndroid;							//������ʶ
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	BYTE							cbMasterOrder;						//����ȼ�

	//�û�״̬
	WORD							wTableID;							//��������
	WORD							wChairID;							//��������
	BYTE							cbUserStatus;						//�û�״̬

	//������Ϣ
	SCORE							lScore;								//�û�����
	SCORE							lGrade;								//�û��ɼ�
	SCORE							lInsure;							//�û�����
	SCORE							lIngot;								//�û�Ԫ��
	DOUBLE							dBeans;								//�û���Ϸ��
	bool							bAndroid;							//�Ƿ�Ϊ������
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
};

//ͷ����Ϣ
struct tagCustomFaceInfo
{
	DWORD							dwDataSize;							//���ݴ�С
	DWORD							dwCustomFace[FACE_CX*FACE_CY];		//ͼƬ��Ϣ
};
//ͷ����Ϣ96*96
struct tagCustomFaceInfo96
{
	DWORD							dwDataSize;							//���ݴ�С
	DWORD							dwCustomFace[BIGFACE_CX*BIGFACE_CY];//ͼƬ��Ϣ
};

//�û���Ϣ
struct tagUserRemoteInfo
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwGameID;							//��Ϸ��ʶ
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	WORD							wFaceID;							//ͷ������

	//�ȼ���Ϣ
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	BYTE							cbMasterOrder;						//����ȼ�

	//λ����Ϣ
	WORD							wKindID;							//���ͱ�ʶ
	WORD							wServerID;							//�����ʶ
	TCHAR							szGameServer[LEN_SERVER];			//����λ��

	//�û�״̬
	WORD							wTableID;							//��������
	WORD							wLastTableID;					    //��Ϸ����
	WORD							wChairID;							//��������
	BYTE							cbUserStatus;						//�û�״̬
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//�ͱ�����
struct tagBaseEnsureParameter
{
	SCORE							lScoreCondition;					//��Ϸ������
	SCORE							lScoreAmount;						//��Ϸ������
	BYTE							cbTakeTimes;						//��ȡ����	
};

//�ƹ���Ϣ
struct tagUserSpreadInfo
{
	DWORD							dwSpreadCount;						//�ƹ�����
	SCORE							lSpreadReward;						//�ƹ㽱��
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//�㳡����
struct tagGamePlaza
{
	WORD							wPlazaID;							//�㳡��ʶ
	TCHAR							szServerAddr[32];					//�����ַ
	TCHAR							szServerName[32];					//��������
};

//��������
struct tagLevelItem
{
	LONG							lLevelScore;						//�������
	TCHAR							szLevelName[16];					//��������
};

//��Ա����
struct tagMemberItem
{
	BYTE							cbMemberOrder;						//�ȼ���ʶ
	TCHAR							szMemberName[16];					//�ȼ�����
};

//��������
struct tagMasterItem
{
	BYTE							cbMasterOrder;						//�ȼ���ʶ
	TCHAR							szMasterName[16];					//�ȼ�����
};

//�б�����
struct tagColumnItem
{
	BYTE							cbColumnWidth;						//�б���
	BYTE							cbDataDescribe;						//�ֶ�����
	TCHAR							szColumnName[16];					//�б�����
};

//��ַ��Ϣ
struct tagAddressInfo
{
	TCHAR							szAddress[32];						//�����ַ
};

//������Ϣ
struct tagDataBaseParameter
{
	WORD							wDataBasePort;						//���ݿ�˿�
	TCHAR							szDataBaseAddr[32];					//���ݿ��ַ
	TCHAR							szDataBaseUser[32];					//���ݿ��û�
	TCHAR							szDataBasePass[32];					//���ݿ�����
	TCHAR							szDataBaseName[32];					//���ݿ�����
};

//��������
struct tagServerOptionInfo
{
	//�ҽ�����
	WORD							wKindID;							//�ҽ�����
	WORD							wNodeID;							//�ҽӽڵ�
	WORD							wSortID;							//���б�ʶ

	//˰������
	WORD							wRevenueRatio;						//˰�ձ���
	SCORE							lServiceScore;						//�������

	//��������
	SCORE							lRestrictScore;						//���ƻ���
	SCORE							lMinTableScore;						//��ͻ���
	SCORE							lMinEnterScore;						//��ͻ���
	SCORE							lMaxEnterScore;						//��߻���

	//��Ա����
	BYTE							cbMinEnterMember;					//��ͻ�Ա
	BYTE							cbMaxEnterMember;					//��߻�Ա

	//��������
	DWORD							dwServerRule;						//�������
	TCHAR							szServerName[LEN_SERVER];			//��������
};

//�û���Ϣ
struct tagMobileUserInfoHead
{
	//�û�����
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwUserID;							//�û� I D

	//ͷ����Ϣ
	WORD							wFaceID;							//ͷ������
	DWORD							dwCustomID;							//�Զ���ʶ

	TCHAR							szNickName[LEN_NICKNAME];			//����ǳ�

	//�û�����
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMemberOrder;						//��Ա�ȼ�

	//�û�״̬
	WORD							wTableID;							//��������
	WORD							wChairID;							//��������
	BYTE							cbUserStatus;						//�û�״̬

	//������Ϣ
	SCORE							lScore;								//�û�����

	//��Ϸ��Ϣ
	DWORD							dwWinCount;							//ʤ������
	DWORD							dwLostCount;						//ʧ������
	DWORD							dwDrawCount;						//�;�����
	DWORD							dwFleeCount;						//��������
};
/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//������������
struct tagServerDummyOnLine
{
	WORD							wServerID;							//�����ʾ
	DWORD							dwDummyMaxOnLine;					//���������������
	DWORD							dwDummyMinOnLine;					//����������С����
	time_t							lTimeStart;							//�仯��ʼʱ��
	time_t							lTimeEnd;							//�仯����ʱ��
	long long						lAccumulation;						//�ۼӱ仯��
	long long						lAccumulationCount;					//�ۼƱ仯����

	DWORD							dwCurDummyOnline;					//��ǰ��¼�仯����
};

//������Ϣ
struct tagUpdateInfo
{
	WORD							wStructSize;						//�ṹ��С
	WORD							wUpdateCount;						//������Ŀ
	char							szDownLoadAddress[MAX_PATH];		//���ص�ַ
};

//��������
struct tagUpdateItem
{
	WORD							wStructSize;						//�ṹ��С
	char							szFileCheckSum[33];					//�ļ�У���
	char							szFileName[128];					//�ļ�����
};

//������ѡ��
struct tagRoomStorageOption
{
	SCORE lStorageStart;
	SCORE lStorageCurrent;
	SCORE lStorageMax;
	int lStorageDeduct;
	int lStorageMul;
	TCHAR szStorageControl[5][32];
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

enum enUserLogonMode
{
	//��½��ʽ
	ULMByAccount,
	ULMByGameID,
	ULMByVisitor,
	ULMBySina,
	ULMByTencent,
	ULMByRenRen,
};

#pragma pack()

#endif