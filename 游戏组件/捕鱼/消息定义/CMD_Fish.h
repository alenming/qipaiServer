
#ifndef CMD_FISH_H_
#define CMD_FISH_H_

#pragma pack(1)

//------------------------------------------------------------------------------
// ������

#define KIND_ID               107
#define GAME_NAME             TEXT("������")
#define GAME_PLAYER           4
#define VERSION_SERVER        PROCESS_VERSION(1,0,0)
#define VERSION_CLIENT        PROCESS_VERSION(1,0,0)

//------------------------------------------------------------------------------

const DWORD m_kGameLoopElasped = 3000;	//�����ж�
const DWORD m_kSceneElasped = 30;	//�����ж�
const DWORD m_kAndroidElasped = 200;
const float m_kAndroidFireElasped = 0.2;

const DWORD m_kMaxFishInManager = 350;
const DWORD m_kMaxBulletsInManager = 400;
const float m_kTideActionDuration = 3.0f;

enum FishKind 
{
	FISH_DAYANYU = 0,         // ������
	FISH_XIAOLVYU,            // С����
	FISH_XIAOHONGYU,          // С����
	FISH_XIAOHUANGYU,         // С����
	FISH_XIAOLANYU,	          // С����
	FISH_XIAOKEDOU,           // С���
	FISH_LVHETUN,             // �̺���
	FISH_SHUIMU,              // ˮĸ
	FISH_XIAOHAIGUI,          // С����
	FISH_HUDIEYU,             // ������
	FISH_QUNDAIYU,            // ȹ����
	FISH_PANGHONGYU,          // �ֺ���
	FISH_DENGLONGYU,          // ������
	FISH_JIANYU,              // ������
	FISH_PANGXIE,             // �з
	FISH_MOGUIYU,             // ħ����
	FISH_LANBIANFU,           // ������
	FISH_LANCHUISHA,          // ����ͷ��
	FISH_LANSHA,              // ����
	FISH_50ZUHE,		      // 50�����
	FISH_60ZUHE,              // 60�����
	FISH_JINCHUISHA,          // ��ͷ��
	FISH_70ZUHE,		      // 70�����
	FISH_JINSHA,              // ����
	FISH_80ZUHE,              // 80�����
	FISH_JINBIANFU,           // ������
	FISH_JINWUGUI,            // ���ڹ�
	FISH_JINHAMA,             // ����
	FISH_MEIRENYU,            // ������
	FISH_ZHANGYU,             // ������
	FISH_JINLONG,             // ����
	FISH_YIWANG1,			  // һ����
	FISH_YIWANG2,			  // һ����
	FISH_YIWANG3,             // һ����
	FISH_YIWANG4,             // һ����
	FISH_YIWANG5,             // һ����

	FISH_KIND_COUNT
};

enum SceneStatus 
{
	SCENE_NONE = 0,
	SCENE_WAIT_TIDE,
	SCENE_RUNNING
};

//------------------------------------------------------------------------------
// ���������
#define SUB_S_FISH							2002        //������
#define SUB_S_USER_FIRE						2003        //�û������ӵ�
#define SUB_S_EXCHANGE_BULLET				2004		//����
#define SUB_S_SWITCH_SCENE					2005		//�л�����
#define SUB_S_CATCH_CHAIN					2006		//������
#define SUB_S_TIMER_SYNC					2007		//ʱ��ͬ��
#define SUB_S_LOCK_FISH						2008        //������
#define SUB_S_PLAYER_SYNC					2010		//ͬ�����
#define SUB_S_FISH_SYNC						2011		//ͬ����
#define SUB_S_BULLET_SYNC					2012		//ͬ���ӵ�

#define SUB_S_REPLACE_CATCH					2013		//����Ұ�����˷��Ͳ�������Ϣ

//������Զ��Ϣ
struct CMD_S_GameScene
{
	int				nSceneID;							//��ǰ����ID
	SCORE			lBaseScore;							//����׷�
	int				nStyleID;							//������ID
	WORD			wPlayerCount;						//�������
	WORD			wFishCount;							//������
	WORD			wBulletCount;						//�ӵ�����
	DWORD			dwServerTicket;						//������ʱ��
};

//�����Ϣ
struct CMD_S_PlayerInfo
{
	int				nPlayerID;							//���GAMEID
	WORD			wChairID;							//������Ӻ�
	SCORE			lPlayerScore;						//�û����
	DWORD			dwPlayerMultiple;					//�û�����
	DWORD			dwLockFishIndex;					//����������
};

//����Ϣ
struct CMD_S_FishInfo
{
	DWORD			dwFishIndex;						//������
	int				nFishKindID;						//������ID
	WORD			wPathID;							//·��ID
	WORD			wPath_Index;						//��������
	DWORD			dwCreteTicket;						//������ʱ��
};

//�ӵ���Ϣ
struct CMD_S_BulletInfo
{
	DWORD			dwBulletIndex;						//�ӵ�����
	int				nPlayerID;							//���GAMEID
	DOUBLE			dBulletAngle;						//�ӵ��Ƕ�
	DWORD			dwBulletTicket;						//�ӵ�����ʱ��
};

//�û�����
struct CMD_S_UserFire
{
	int				nPlayerID;							//���GAMEID
	DWORD			wBulletIndex;						//�ӵ�����
	int				nBulletMultiple;					//�ӵ�����
	DOUBLE			dBulletAngle;						//�ӵ��Ƕ�
	SCORE			lPlayerScore;						//�û����
	DWORD			dwCreateTicket;						//�ӵ�����ʱ��
};

//�û��л���
struct CMD_S_PlayerExchangeBullet
{
	int				nPlayerID;							//���GAMEID
	int				nPlayerMultiple;					//�û��ӵ�����
};

//������
struct CMD_S_PlayerCatchFish
{
	int				nPlayerID;							//���GAMEID
	DWORD			dwBulletIndex;						//�ӵ�����
	DWORD			dwFishIndex;						//������
	SCORE			lFishScore;							//���л�ý��
	SCORE			lPlayerScore;						//�û����
};

//�ӵ�ͬ��
struct CMD_S_BulletInfoSyn
{
	DWORD			dwBulletIndex;						//�ӵ�����
	DOUBLE			dCol;
	DOUBLE			dRow;
};

//�����л�
struct CMD_S_CHANGE_SCENE
{
	int				nSceneID;							//����ID
};

//������
struct CMD_S_LockFish
{
	int				nPlayerID;							//���GAMEID
	DWORD			dwLockFishIndex;					//����������
};

//ʱ��ͬ��
struct CMD_S_SynTime
{
	DWORD			dwServerTicket;						//������ʱ��
};

//����Ұ�����˷��Ͳ�������Ϣ
struct CMD_S_Replace_Catch
{
	DWORD			wMaxReplaceGameID[GAME_PLAYER-1];	//��æ���͵����Ӻ�
};

//------------------------------------------------------------------------------
// �ͻ�������
#define SUB_C_USER_FIRE						1			//�û������ӵ�
#define SUB_C_EXCHANGE_BULLET				2			//�û��л���̨
#define SUB_C_CATCH_FISH					3			//������
#define SUB_C_TIMER_SYNC					4			//ʱ��ͬ��
#define SUB_C_LOCK_FISH						5			//������

//�û�����
struct CMD_C_UserFire
{
	DWORD			dwBulletIndex;						//�ӵ�����
	DOUBLE			dBulletAngle;						//�Ƕ�
};

//�û��л���
struct CMD_C_PlayerExchangeBullet
{
	int				nPlayerBulletMultiple;				//�û��ӵ�����
};

//�û�������
struct CMD_C_PlayerCatchFish
{
	WORD			wChairID;							//������������Ӻ�
	DWORD			dwBulletIndex;						//�ӵ�����
	DOUBLE			dBulletRow;							//��������
	DOUBLE			dBulletCol;							//��������
	DWORD			dwFishIndex;						//������
	WORD			wPathIndex;						
};

//�û�������
struct CMD_C_LockFish
{
	DWORD			dwLockFishIndex;					//������
};

#pragma pack()

#endif