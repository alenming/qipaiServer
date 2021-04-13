#ifndef CONTROLHEAD_HEAD_FILE
#define CONTROLHEAD_HEAD_FILE
#include <Afxtempl.h>
#pragma pack(1)

//�ټ���
#define BACCARAT_AREA_MAX			5										//�ټ�����������
#define BACCARAT_PLAYER				100										//�ټ�������
//�ټ��ֿ��Ʋ���
struct tag_Baccarat_Parameter
{
	WORD							wBankerUser;									//ׯ���û�
	BYTE							cbJuControl;;									//�Ƿ���оֿ�
	BYTE							cbJuControlArea[BACCARAT_AREA_MAX];				//���ֿ�������ʤ�� 0�����ƣ�1����Ӯ��2������

	SCORE							lPlayBet[BACCARAT_PLAYER][BACCARAT_AREA_MAX];	//��ע���
};

//�ټ��ֿ��ƽ��
struct tag_Baccarat_Result
{
	BYTE							cbCardCount[2];							//�˿���Ŀ
	BYTE							cbTableCardArray[2][3];					//�����˿�
};

//���
#define REDBLACK_AREA_MAX			3										//�����������
#define REDBLACK_PLAYER				100										//�������
//��ڿ��Ʋ���
struct tag_Redblack_Parameter
{
	WORD							wBankerUser;									//ׯ���û�
	BYTE							cbJuControl;;									//�Ƿ���оֿ�
	BYTE							cbJuControlArea[REDBLACK_AREA_MAX];				//���ֿ�������ʤ�� 0�����ƣ�1����Ӯ��2������

	SCORE							lPlayBet[REDBLACK_PLAYER][REDBLACK_AREA_MAX];	//��ע���
};

//��ڿ��ƽ��
struct tag_Redblack_Result
{
	BYTE							cbCardCount[2];							//�˿���Ŀ
	BYTE							cbTableCardArray[2][3];					//�����˿�
};

//����
#define LONGHU_AREA_MAX				3										//������������
#define LONGHU_PLAYER				100										//��������

//�������Ʋ���
struct tag_LongHu_Parameter
{
	WORD							wBankerUser;									//ׯ���û�
	BYTE							cbJuControl;;									//�Ƿ���оֿ�
	BYTE							cbJuControlArea[LONGHU_AREA_MAX];				//���ֿ�������ʤ�� 0�����ƣ�1����Ӯ��2������

	SCORE							lPlayBet[LONGHU_PLAYER][LONGHU_AREA_MAX];	//��ע���
};

//�������ƽ��
struct tag_LongHu_Result
{
	BYTE							cbTableCardArray[2];					//�����˿�
};

//����ţţ 
#define OXBATTLE_AREA_MAX			4												//����ţţ��������
#define OXBATTLE_PLAYER				100												//����ţţ����
//����ţţ���Ʋ���
struct tag_OxBattle_Parameter
{
	WORD							wBankerUser;									//ׯ���û�
	BYTE							cbJuControl;;									//�Ƿ���оֿ�
	BYTE							cbJuControlArea[OXBATTLE_AREA_MAX + 1];			//���ֿ�������ʤ�� 0�����ƣ�1����Ӯ��2������

	BYTE							cbTableCardArray[5][5];							//�Ѿ���������
	SCORE							lPlayBet[OXBATTLE_PLAYER][OXBATTLE_AREA_MAX + 1];	//��ע���
};

//����ţţ���ƽ��
struct tag_OxBattle_Result
{
	BYTE							cbTableCardArray[5][5];							//�����˿�
};

//������
#define LAND_NORMAL_COUNT			17												//��������
#define LAND_FULL_COUNT				54												//�˿�����
#define LAND_GAME_PLAYER			3												//����������

//���������ƽ��
struct tag_Land_Result
{
	BYTE							cbTableCardArray[LAND_FULL_COUNT];				//�˿�
};

//����
#define ZHAJINHUA_GAME_PLAYER		5												//��������

//���𻨿��ƽ��
struct tag_ZhaJinHua_Result
{
	BYTE							cbTableCardArray[ZHAJINHUA_GAME_PLAYER][3];		//�˿�
};

//��ׯţţ
#define OXSIX_GAME_PLAYER			5												//��ׯţţ�������
#define OXSIX_MAX_CARDCOUNT			5												//��ׯţţ����˿�

//��ׯţţ���Ʋ���
struct tag_OxSix_Parameter
{
	WORD							wBankerUser;									//ׯ���û�

	BYTE							cbPlayerStatus[OXSIX_GAME_PLAYER];				//���״̬

	SCORE							lExitScore;										//ǿ�˷���
	SCORE							lTableScore[OXSIX_GAME_PLAYER];					//�������

	BYTE							cbCallBankerTimes[OXSIX_GAME_PLAYER];			//��ׯ����

	BYTE							cbTableCardArray[5][5];							//�Ѿ���������
};
//��ׯţţ���ƽ��
struct tag_OxSix_Result
{
	BYTE							cbTableCardArray[OXSIX_GAME_PLAYER][OXSIX_MAX_CARDCOUNT];		//�˿�
};

//�����
//�������Ʋ���
struct tag_Shby_Parameter
{
	WORD							wshutUser;									//�����û�
	double							probability;								//����
};

//�������ƽ��
struct tag_Shby_Result
{
	double							probability;								//����
};

//��󸲶��
//��󸲶����Ʋ���
struct tag_Jcby_Parameter
{
	WORD							wshutUser;									//�����û�
	double							probability;								//����
};
//��󸲶����ƽ��
struct tag_Jcby_Result
{
	double							probability;								//����
};

//�����칬����
//�����칬������Ʋ���
struct tag_Dntg_Parameter
{
	WORD							wshutUser;									//�����û�
	double							probability;								//����
};
//�����칬������ƽ��
struct tag_Dntg_Result
{
	double							probability;								//����
};

//����
#define CAISHEN_X_COUNT					5									//ͼ������
#define CAISHEN_Y_COUNT					3									//ͼ������
//���񵽿��Ʋ���
struct tag_CaiShen_Parameter
{
	SCORE								lBetScore;							//Ѻ��
};
//���񵽿��ƽ��
struct tag_CaiShen_Result
{
	BYTE								cbItemInfo[CAISHEN_Y_COUNT][CAISHEN_X_COUNT];	//������Ϣ
};

//��������
#define JDQS_X_COUNT					5									//ͼ������
#define JDQS_Y_COUNT					3									//ͼ������
//�����������Ʋ���
struct tag_JDQS_Parameter
{
	SCORE								lBetScore;							//Ѻ��
};
//�����������ƽ��
struct tag_JDQS_Result
{
	BYTE								cbItemInfo[JDQS_Y_COUNT][JDQS_X_COUNT];	//������Ϣ
};

//��������
#define FQZS_AREA_COUNT					12									//��������
#define FQZS_ANIMAL_COUNT				28									//��������
#define FQZS_GAME_PLAYER				100									//������������

//�������޿��Ʋ���
struct tag_FQZS_Parameter
{
	WORD								wCurrentBanker;						//��ǰׯ��
	BYTE								cbJuControl;;						//�Ƿ���оֿ�
	BYTE								cbJuControlArea;					//���ֿ�������ʤ�� 0�����ƣ�1����Ӯ��2������
	SCORE								lUserJettonScore[FQZS_GAME_PLAYER][FQZS_AREA_COUNT];	//�û���ע
	INT									nMultiple[FQZS_AREA_COUNT];			//������
};
//�������޿��ƽ��
struct tag_FQZS_Result
{
	BYTE								cbTableCardArray[2];				//�����˿�
};

//���۱���
#define BCBM_AREA_COUNT					8									//��������
#define BCBM_ANIMAL_COUNT				32									//��������
#define BCBM_GAME_PLAYER				100									//������������

//���۱�����Ʋ���
struct tag_BCBM_Parameter
{
	WORD								wCurrentBanker;						//��ǰׯ��
	BYTE								cbJuControl;;						//�Ƿ���оֿ�
	BYTE								cbJuControlArea;					//���ֿ�������ʤ�� 0�����ƣ�1����Ӯ��2������
	SCORE								lUserJettonScore[BCBM_GAME_PLAYER][BCBM_AREA_COUNT];	//�û���ע
	INT									nMultiple[BCBM_AREA_COUNT];			//������
};
//���۱�����ƽ��
struct tag_BCBM_Result
{
	BYTE								cbTableCardArray;					//�����˿�
};

//ˮ䰴�
#define SHZ_X_COUNT					5										//ͼ������
#define SHZ_Y_COUNT					3										//ͼ������

//ˮ䰴����Ʋ���
struct tag_SHZ_Parameter
{
	BYTE								cbOneMode;							//ѹ�߻��Ǳȱ�
	SCORE								lBetScore;							//Ѻ��
	BYTE								cbOpenArea;							//�ȱ�����
};

//ˮ䰴����ƽ��
struct tag_SHZ_Result
{
	BYTE								cbItemInfo[SHZ_Y_COUNT][SHZ_X_COUNT];	//������Ϣ
	BYTE								cbOpenSize[2];						//�������
};

//����
#define LP_AREA_COUNT					49									//��������
#define LP_RESULT_COUNT					37									//�����Ŀ
#define LP_GAME_PLAYER					100									//����

//���̿��Ʋ���
struct tag_LP_Parameter
{
	SCORE								lUserJettonScore[LP_GAME_PLAYER][LP_AREA_COUNT];	//�û���ע
};
//���̿��ƽ��
struct tag_LP_Result
{
	BYTE								cbTableCardArray;					//�����˿�
};

//����
#define SICBO_AREA_COUNT				35									//��������
#define SICBO_RESULT_COUNT				6									//�����Ŀ
#define SICBO_GAME_PLAYER				100									//����

//�������Ʋ���
struct tag_SICBO_Parameter
{
	SCORE								lUserJettonScore[SICBO_GAME_PLAYER][SICBO_AREA_COUNT];	//�û���ע
};
//�������ƽ��
struct tag_SICBO_Result
{
	BYTE								cbTableCardArray[3];				//�����˿�
};

//ʮ��ˮ
#define THIRTEEN_CARD_COUNT			13												//��������
#define THIRTEEN_GAME_PLAYER		4												//ʮ��ˮ����

//ʮ��ˮ���ƽ��
struct tag_Thirteen_Result
{
	BYTE							cbTableCardArray[THIRTEEN_GAME_PLAYER][THIRTEEN_CARD_COUNT];				//�˿�
};

//���˶��˸� 
#define TONGZI_AREA_MAX				4												//���˶��˸���������
#define TONGZI_PLAYER				100												//���˶��˸�����
//���˶��˸ܿ��Ʋ���
struct tag_TongZi_Parameter
{
	WORD							wBankerUser;									//ׯ���û�
	BYTE							cbJuControl;;									//�Ƿ���оֿ�
	BYTE							cbJuControlArea[TONGZI_AREA_MAX];				//���ֿ�������ʤ�� 0�����ƣ�1����Ӯ��2������

	BYTE							cbLeftCardCount;								//ʣ������Ŀ
	BYTE							cbTableCard[40];								//ʣ����

	SCORE							lPlayBet[TONGZI_PLAYER][TONGZI_AREA_MAX];		//��ע���
};

//���˶��˸ܿ��ƽ��
struct tag_TongZi_Result
{
	BYTE							cbTableCardArray[4][2];							//�����˿�
};

//����
#define SG_GAME_PLAYER				9												//�����������
#define SG_MAX_CARDCOUNT			3												//��������˿�

//�������Ʋ���
struct tag_SG_Parameter
{
	WORD							wBankerUser;									//ׯ���û�
	BYTE							cbPlayerStatus[SG_GAME_PLAYER];					//���״̬
	SCORE							lTableScore[SG_GAME_PLAYER];					//�������
	BYTE							cbCallBankerTimes[SG_GAME_PLAYER];				//��ׯ����
	BYTE							cbTableCardArray[SG_GAME_PLAYER][SG_MAX_CARDCOUNT];	//�Ѿ���������
};
//�������ƽ��
struct tag_SG_Result
{
	BYTE							cbTableCardArray[SG_GAME_PLAYER][SG_MAX_CARDCOUNT];	//�˿�
};

//ˮ������
#define SGLB_X_COUNT					5										//ͼ������
#define SGLB_Y_COUNT					3										//ͼ������

//ˮ�����Կ��Ʋ���
struct tag_SGLB_Parameter
{
	BYTE								cbOneMode;							//ѹ�߻��ǿ�����
	SCORE								lBetScore;							//Ѻ��
};

//ˮ�����Կ��ƽ��
struct tag_SGLB_Result
{
	BYTE								cbItemInfo[SGLB_Y_COUNT][SGLB_X_COUNT];	//������Ϣ
	SCORE								lBonusScore;							//���ӷ���
};

#pragma pack()

#endif