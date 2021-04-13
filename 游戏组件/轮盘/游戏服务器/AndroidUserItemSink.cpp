#include "Stdafx.h"
#include "AndroidUserItemSink.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_BANK_OPERATE			101									//���ж�ʱ
#define IDI_CHECK_BANKER			102									//�����ׯ
#define IDI_REQUEST_BANKER			103									//���붨ʱ
#define IDI_GIVEUP_BANKER			104									//��ׯ��ʱ
#define IDI_PLACE_JETTON			110									//��ע���� (Ԥ��110-160)

//////////////////////////////////////////////////////////////////////////

int CAndroidUserItemSink::m_stlApplyBanker = 0L;

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//��Ϸ����
	m_lMaxChipBanker = 0;
	m_lMaxChipUser = 0;
	m_wCurrentBanker = 0;
	m_nChipTime = 0;
	m_nChipTimeCount = 0;
	m_cbTimeLeave = 0;
	ZeroMemory(m_lAreaChip, sizeof(m_lAreaChip));
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));
	m_nRobotBetTimeLimit[0]=3;
	m_nRobotBetTimeLimit[1]=8;
	m_lRobotJettonLimit[0]=10;
	m_lRobotJettonLimit[1]=1000000;

  m_lAreaLimitScore = 1000000;
	//��ׯ����
	m_bMeApplyBanker=false;
	m_nWaitBanker=0;
	m_nBankerCount=0;

	for (int i = ASTRAIGHT_0; i <= ASTRAIGHT_36; i++)
	{
		m_nAreaNum[i][0] = i;
		for (int j = 1; j < 18; j++)
		{
			m_nAreaNum[i][j] = 255;
		}
	}

	for (int i = ASPLIT_1; i <= ASPLIT_57; i++)
	{
		for (int j = 2; j < 18; j++)
		{
			m_nAreaNum[i][j] = 255;
		}
	}
	m_nAreaNum[ASPLIT_1][0] = 1;
	m_nAreaNum[ASPLIT_1][1] = 2;

	m_nAreaNum[ASPLIT_2][0] = 1;
	m_nAreaNum[ASPLIT_2][1] = 4;

	m_nAreaNum[ASPLIT_3][0] = 2;
	m_nAreaNum[ASPLIT_3][1] = 3;

	m_nAreaNum[ASPLIT_4][0] = 2;
	m_nAreaNum[ASPLIT_4][1] = 5;

	m_nAreaNum[ASPLIT_5][0] = 3;
	m_nAreaNum[ASPLIT_5][1] = 6;

	m_nAreaNum[ASPLIT_6][0] = 4;
	m_nAreaNum[ASPLIT_6][1] = 5;

	m_nAreaNum[ASPLIT_7][0] = 4;
	m_nAreaNum[ASPLIT_7][1] = 7;

	m_nAreaNum[ASPLIT_8][0] = 5;
	m_nAreaNum[ASPLIT_8][1] = 6;

	m_nAreaNum[ASPLIT_9][0] = 5;
	m_nAreaNum[ASPLIT_9][1] = 8;

	m_nAreaNum[ASPLIT_10][0] = 6;
	m_nAreaNum[ASPLIT_10][1] = 9;

	m_nAreaNum[ASPLIT_11][0] = 7;
	m_nAreaNum[ASPLIT_11][1] = 8;

	m_nAreaNum[ASPLIT_12][0] = 7;
	m_nAreaNum[ASPLIT_12][1] = 10;

	m_nAreaNum[ASPLIT_13][0] = 8;
	m_nAreaNum[ASPLIT_13][1] = 9;

	m_nAreaNum[ASPLIT_14][0] = 8;
	m_nAreaNum[ASPLIT_14][1] = 11;

	m_nAreaNum[ASPLIT_15][0] = 9;
	m_nAreaNum[ASPLIT_15][1] = 12;

	m_nAreaNum[ASPLIT_16][0] = 10;
	m_nAreaNum[ASPLIT_16][1] = 11;

	m_nAreaNum[ASPLIT_17][0] = 10;
	m_nAreaNum[ASPLIT_17][1] = 13;

	m_nAreaNum[ASPLIT_18][0] = 11;
	m_nAreaNum[ASPLIT_18][1] = 12;

	m_nAreaNum[ASPLIT_19][0] = 11;
	m_nAreaNum[ASPLIT_19][1] = 14;

	m_nAreaNum[ASPLIT_20][0] = 12;
	m_nAreaNum[ASPLIT_20][1] = 15;

	m_nAreaNum[ASPLIT_21][0] = 13;
	m_nAreaNum[ASPLIT_21][1] = 14;

	m_nAreaNum[ASPLIT_22][0] = 13;
	m_nAreaNum[ASPLIT_22][1] = 16;

	m_nAreaNum[ASPLIT_23][0] = 14;
	m_nAreaNum[ASPLIT_23][1] = 15;

	m_nAreaNum[ASPLIT_24][0] = 14;
	m_nAreaNum[ASPLIT_24][1] = 17;

	m_nAreaNum[ASPLIT_25][0] = 15;
	m_nAreaNum[ASPLIT_25][1] = 18;

	m_nAreaNum[ASPLIT_26][0] = 16;
	m_nAreaNum[ASPLIT_26][1] = 17;

	m_nAreaNum[ASPLIT_27][0] = 16;
	m_nAreaNum[ASPLIT_27][1] = 19;

	m_nAreaNum[ASPLIT_28][0] = 17;
	m_nAreaNum[ASPLIT_28][1] = 18;

	m_nAreaNum[ASPLIT_29][0] = 17;
	m_nAreaNum[ASPLIT_29][1] = 20;

	m_nAreaNum[ASPLIT_30][0] = 18;
	m_nAreaNum[ASPLIT_30][1] = 21;

	m_nAreaNum[ASPLIT_31][0] = 19;
	m_nAreaNum[ASPLIT_31][1] = 20;

	m_nAreaNum[ASPLIT_32][0] = 19;
	m_nAreaNum[ASPLIT_32][1] = 22;

	m_nAreaNum[ASPLIT_33][0] = 20;
	m_nAreaNum[ASPLIT_33][1] = 21;

	m_nAreaNum[ASPLIT_34][0] = 20;
	m_nAreaNum[ASPLIT_34][1] = 23;

	m_nAreaNum[ASPLIT_35][0] = 21;
	m_nAreaNum[ASPLIT_35][1] = 24;

	m_nAreaNum[ASPLIT_36][0] = 22;
	m_nAreaNum[ASPLIT_36][1] = 23;

	m_nAreaNum[ASPLIT_37][0] = 22;
	m_nAreaNum[ASPLIT_37][1] = 25;

	m_nAreaNum[ASPLIT_38][0] = 23;
	m_nAreaNum[ASPLIT_38][1] = 24;

	m_nAreaNum[ASPLIT_39][0] = 23;
	m_nAreaNum[ASPLIT_39][1] = 26;

	m_nAreaNum[ASPLIT_40][0] = 24;
	m_nAreaNum[ASPLIT_40][1] = 27;

	m_nAreaNum[ASPLIT_41][0] = 25;
	m_nAreaNum[ASPLIT_41][1] = 26;

	m_nAreaNum[ASPLIT_42][0] = 25;
	m_nAreaNum[ASPLIT_42][1] = 28;

	m_nAreaNum[ASPLIT_43][0] = 26;
	m_nAreaNum[ASPLIT_43][1] = 27;

	m_nAreaNum[ASPLIT_44][0] = 26;
	m_nAreaNum[ASPLIT_44][1] = 29;

	m_nAreaNum[ASPLIT_45][0] = 27;
	m_nAreaNum[ASPLIT_45][1] = 30;

	m_nAreaNum[ASPLIT_46][0] = 28;
	m_nAreaNum[ASPLIT_46][1] = 29;

	m_nAreaNum[ASPLIT_47][0] = 28;
	m_nAreaNum[ASPLIT_47][1] = 31;

	m_nAreaNum[ASPLIT_48][0] = 29;
	m_nAreaNum[ASPLIT_48][1] = 30;

	m_nAreaNum[ASPLIT_49][0] = 29;
	m_nAreaNum[ASPLIT_49][1] = 32;

	m_nAreaNum[ASPLIT_50][0] = 30;
	m_nAreaNum[ASPLIT_50][1] = 33;

	m_nAreaNum[ASPLIT_51][0] = 31;
	m_nAreaNum[ASPLIT_51][1] = 32;

	m_nAreaNum[ASPLIT_52][0] = 31;
	m_nAreaNum[ASPLIT_52][1] = 34;

	m_nAreaNum[ASPLIT_53][0] = 32;
	m_nAreaNum[ASPLIT_53][1] = 33;

	m_nAreaNum[ASPLIT_54][0] = 32;
	m_nAreaNum[ASPLIT_54][1] = 35;

	m_nAreaNum[ASPLIT_55][0] = 33;
	m_nAreaNum[ASPLIT_55][1] = 36;

	m_nAreaNum[ASPLIT_56][0] = 34;
	m_nAreaNum[ASPLIT_56][1] = 35;

	m_nAreaNum[ASPLIT_57][0] = 35;
	m_nAreaNum[ASPLIT_57][1] = 36;

	for (int i = ASTREET_1; i <= ASTREET_12; i++)
	{
		m_nAreaNum[i][0] = (i - ASTREET_1) * 3 + 1;
		m_nAreaNum[i][1] = (i - ASTREET_1) * 3 + 2;
		m_nAreaNum[i][2] = (i - ASTREET_1) * 3 + 3;
		for (int j = 3; j < 18; j++)
		{
			m_nAreaNum[i][j] = 255;
		}
	}

	m_nAreaNum[ATHREE_1][0] = 0;
	m_nAreaNum[ATHREE_1][1] = 1;
	m_nAreaNum[ATHREE_1][2] = 2;

	m_nAreaNum[ATHREE_2][0] = 0;
	m_nAreaNum[ATHREE_2][1] = 2;
	m_nAreaNum[ATHREE_2][2] = 3;

	for (int i = ATHREE_1; i <= ATHREE_2; i++)
	{
		for (int j = 3; j < 18; j++)
		{
			m_nAreaNum[i][j] = 255;
		}
	}

	for (int i = ACORNER_1; i <= ACORNER_22; i++)
	{
		for (int j = 4; j < 18; j++)
		{
			m_nAreaNum[i][j] = 255;
		}
	}

	m_nAreaNum[ACORNER_1][0] = 1;
	m_nAreaNum[ACORNER_1][1] = 2;
	m_nAreaNum[ACORNER_1][2] = 4;
	m_nAreaNum[ACORNER_1][3] = 5;

	m_nAreaNum[ACORNER_2][0] = 2;
	m_nAreaNum[ACORNER_2][1] = 3;
	m_nAreaNum[ACORNER_2][2] = 5;
	m_nAreaNum[ACORNER_2][3] = 6;

	m_nAreaNum[ACORNER_3][0] = 4;
	m_nAreaNum[ACORNER_3][1] = 5;
	m_nAreaNum[ACORNER_3][2] = 7;
	m_nAreaNum[ACORNER_3][3] = 8;

	m_nAreaNum[ACORNER_4][0] = 5;
	m_nAreaNum[ACORNER_4][1] = 6;
	m_nAreaNum[ACORNER_4][2] = 8;
	m_nAreaNum[ACORNER_4][3] = 9;

	m_nAreaNum[ACORNER_5][0] = 7;
	m_nAreaNum[ACORNER_5][1] = 8;
	m_nAreaNum[ACORNER_5][2] = 10;
	m_nAreaNum[ACORNER_5][3] = 11;

	m_nAreaNum[ACORNER_6][0] = 8;
	m_nAreaNum[ACORNER_6][1] = 9;
	m_nAreaNum[ACORNER_6][2] = 11;
	m_nAreaNum[ACORNER_6][3] = 12;

	m_nAreaNum[ACORNER_7][0] = 10;
	m_nAreaNum[ACORNER_7][1] = 11;
	m_nAreaNum[ACORNER_7][2] = 13;
	m_nAreaNum[ACORNER_7][3] = 14;

	m_nAreaNum[ACORNER_8][0] = 11;
	m_nAreaNum[ACORNER_8][1] = 12;
	m_nAreaNum[ACORNER_8][2] = 14;
	m_nAreaNum[ACORNER_8][3] = 15;

	m_nAreaNum[ACORNER_9][0] = 13;
	m_nAreaNum[ACORNER_9][1] = 14;
	m_nAreaNum[ACORNER_9][2] = 16;
	m_nAreaNum[ACORNER_9][3] = 17;

	m_nAreaNum[ACORNER_10][0] = 14;
	m_nAreaNum[ACORNER_10][1] = 15;
	m_nAreaNum[ACORNER_10][2] = 17;
	m_nAreaNum[ACORNER_10][3] = 18;

	m_nAreaNum[ACORNER_11][0] = 16;
	m_nAreaNum[ACORNER_11][1] = 17;
	m_nAreaNum[ACORNER_11][2] = 19;
	m_nAreaNum[ACORNER_11][3] = 20;

	m_nAreaNum[ACORNER_12][0] = 17;
	m_nAreaNum[ACORNER_12][1] = 18;
	m_nAreaNum[ACORNER_12][2] = 20;
	m_nAreaNum[ACORNER_12][3] = 21;

	m_nAreaNum[ACORNER_13][0] = 19;
	m_nAreaNum[ACORNER_13][1] = 20;
	m_nAreaNum[ACORNER_13][2] = 22;
	m_nAreaNum[ACORNER_13][3] = 23;

	m_nAreaNum[ACORNER_14][0] = 20;
	m_nAreaNum[ACORNER_14][1] = 21;
	m_nAreaNum[ACORNER_14][2] = 23;
	m_nAreaNum[ACORNER_14][3] = 24;

	m_nAreaNum[ACORNER_15][0] = 22;
	m_nAreaNum[ACORNER_15][1] = 23;
	m_nAreaNum[ACORNER_15][2] = 25;
	m_nAreaNum[ACORNER_15][3] = 26;

	m_nAreaNum[ACORNER_16][0] = 23;
	m_nAreaNum[ACORNER_16][1] = 24;
	m_nAreaNum[ACORNER_16][2] = 26;
	m_nAreaNum[ACORNER_16][3] = 27;

	m_nAreaNum[ACORNER_17][0] = 25;
	m_nAreaNum[ACORNER_17][1] = 26;
	m_nAreaNum[ACORNER_17][2] = 28;
	m_nAreaNum[ACORNER_17][3] = 29;

	m_nAreaNum[ACORNER_18][0] = 26;
	m_nAreaNum[ACORNER_18][1] = 27;
	m_nAreaNum[ACORNER_18][2] = 29;
	m_nAreaNum[ACORNER_18][3] = 30;

	m_nAreaNum[ACORNER_19][0] = 28;
	m_nAreaNum[ACORNER_19][1] = 29;
	m_nAreaNum[ACORNER_19][2] = 31;
	m_nAreaNum[ACORNER_19][3] = 32;

	m_nAreaNum[ACORNER_20][0] = 29;
	m_nAreaNum[ACORNER_20][1] = 30;
	m_nAreaNum[ACORNER_20][2] = 32;
	m_nAreaNum[ACORNER_20][3] = 33;

	m_nAreaNum[ACORNER_21][0] = 31;
	m_nAreaNum[ACORNER_21][1] = 32;
	m_nAreaNum[ACORNER_21][2] = 34;
	m_nAreaNum[ACORNER_21][3] = 35;

	m_nAreaNum[ACORNER_22][0] = 32;
	m_nAreaNum[ACORNER_22][1] = 33;
	m_nAreaNum[ACORNER_22][2] = 35;
	m_nAreaNum[ACORNER_22][3] = 36;

	m_nAreaNum[AFOUR_1][0] = 0;
	m_nAreaNum[AFOUR_1][1] = 1;
	m_nAreaNum[AFOUR_1][2] = 2;
	m_nAreaNum[AFOUR_1][3] = 3;

	for (int i = ALINE_1; i <= ALINE_11; i++)
	{
		m_nAreaNum[i][0] = (i - ALINE_1) * 3 + 1;
		m_nAreaNum[i][1] = (i - ALINE_1) * 3 + 2;
		m_nAreaNum[i][2] = (i - ALINE_1) * 3 + 3;
		m_nAreaNum[i][3] = (i - ALINE_1) * 3 + 4;
		m_nAreaNum[i][4] = (i - ALINE_1) * 3 + 5;
		m_nAreaNum[i][5] = (i - ALINE_1) * 3 + 6;
		for (int j = 6; j < 18; j++)
		{
			m_nAreaNum[i][j] = 255;
		}
	}

	for (int i = ACOLUMN_1; i <= ACOLUMN_3; i++)
	{
		m_nAreaNum[i][0] = (i - ACOLUMN_1 + 1);
		m_nAreaNum[i][1] = (i - ACOLUMN_1 + 1) + 3;
		m_nAreaNum[i][2] = (i - ACOLUMN_1 + 1) + 6;
		m_nAreaNum[i][3] = (i - ACOLUMN_1 + 1) + 9;
		m_nAreaNum[i][4] = (i - ACOLUMN_1 + 1) + 12;
		m_nAreaNum[i][5] = (i - ACOLUMN_1 + 1) + 15;
		m_nAreaNum[i][6] = (i - ACOLUMN_1 + 1) + 18;
		m_nAreaNum[i][7] = (i - ACOLUMN_1 + 1) + 21;
		m_nAreaNum[i][8] = (i - ACOLUMN_1 + 1) + 24;
		m_nAreaNum[i][9] = (i - ACOLUMN_1 + 1) + 27;
		m_nAreaNum[i][10] = (i - ACOLUMN_1 + 1) + 30;
		m_nAreaNum[i][11] = (i - ACOLUMN_1 + 1) + 33;
		for (int j = 12; j < 18; j++)
		{
			m_nAreaNum[i][j] = 255;
		}
	}

	for (int i = ADOZEN_1; i <= ADOZEN_3; i++)
	{
		m_nAreaNum[i][0] = (i - ADOZEN_1) * 12 + 1;
		m_nAreaNum[i][1] = (i - ADOZEN_1) * 12 + 2;
		m_nAreaNum[i][2] = (i - ADOZEN_1) * 12 + 3;
		m_nAreaNum[i][3] = (i - ADOZEN_1) * 12 + 4;
		m_nAreaNum[i][4] = (i - ADOZEN_1) * 12 + 5;
		m_nAreaNum[i][5] = (i - ADOZEN_1) * 12 + 6;
		m_nAreaNum[i][6] = (i - ADOZEN_1) * 12 + 7;
		m_nAreaNum[i][7] = (i - ADOZEN_1) * 12 + 8;
		m_nAreaNum[i][8] = (i - ADOZEN_1) * 12 + 9;
		m_nAreaNum[i][9] = (i - ADOZEN_1) * 12 + 10;
		m_nAreaNum[i][10] = (i - ADOZEN_1) * 12 + 11;
		m_nAreaNum[i][11] = (i - ADOZEN_1) * 12 + 12;
		for (int j = 12; j < 18; j++)
		{
			m_nAreaNum[i][j] = 255;
		}
	}

	m_nAreaNum[ABLACK_1][0] = 2;
	m_nAreaNum[ABLACK_1][1] = 4;
	m_nAreaNum[ABLACK_1][2] = 6;
	m_nAreaNum[ABLACK_1][3] = 8;
	m_nAreaNum[ABLACK_1][4] = 10;
	m_nAreaNum[ABLACK_1][5] = 11;
	m_nAreaNum[ABLACK_1][6] = 13;
	m_nAreaNum[ABLACK_1][7] = 15;
	m_nAreaNum[ABLACK_1][8] = 17;
	m_nAreaNum[ABLACK_1][9] = 20;
	m_nAreaNum[ABLACK_1][10] = 22;
	m_nAreaNum[ABLACK_1][11] = 24;
	m_nAreaNum[ABLACK_1][12] = 26;
	m_nAreaNum[ABLACK_1][13] = 28;
	m_nAreaNum[ABLACK_1][14] = 29;
	m_nAreaNum[ABLACK_1][15] = 31;
	m_nAreaNum[ABLACK_1][16] = 33;
	m_nAreaNum[ABLACK_1][17] = 35;

	m_nAreaNum[ARED_1][0] = 1;
	m_nAreaNum[ARED_1][1] = 3;
	m_nAreaNum[ARED_1][2] = 5;
	m_nAreaNum[ARED_1][3] = 7;
	m_nAreaNum[ARED_1][4] = 9;
	m_nAreaNum[ARED_1][5] = 12;
	m_nAreaNum[ARED_1][6] = 14;
	m_nAreaNum[ARED_1][7] = 16;
	m_nAreaNum[ARED_1][8] = 18;
	m_nAreaNum[ARED_1][9] = 19;
	m_nAreaNum[ARED_1][10] = 21;
	m_nAreaNum[ARED_1][11] = 23;
	m_nAreaNum[ARED_1][12] = 25;
	m_nAreaNum[ARED_1][13] = 27;
	m_nAreaNum[ARED_1][14] = 30;
	m_nAreaNum[ARED_1][15] = 32;
	m_nAreaNum[ARED_1][16] = 34;
	m_nAreaNum[ARED_1][17] = 36;

	m_nAreaNum[AODD_1][0] = 1;
	m_nAreaNum[AODD_1][1] = 3;
	m_nAreaNum[AODD_1][2] = 5;
	m_nAreaNum[AODD_1][3] = 7;
	m_nAreaNum[AODD_1][4] = 9;
	m_nAreaNum[AODD_1][5] = 11;
	m_nAreaNum[AODD_1][6] = 13;
	m_nAreaNum[AODD_1][7] = 15;
	m_nAreaNum[AODD_1][8] = 17;
	m_nAreaNum[AODD_1][9] = 19;
	m_nAreaNum[AODD_1][10] = 21;
	m_nAreaNum[AODD_1][11] = 23;
	m_nAreaNum[AODD_1][12] = 25;
	m_nAreaNum[AODD_1][13] = 27;
	m_nAreaNum[AODD_1][14] = 29;
	m_nAreaNum[AODD_1][15] = 31;
	m_nAreaNum[AODD_1][16] = 33;
	m_nAreaNum[AODD_1][17] = 35;

	m_nAreaNum[AEVEN_1][0] = 2;
	m_nAreaNum[AEVEN_1][1] = 4;
	m_nAreaNum[AEVEN_1][2] = 6;
	m_nAreaNum[AEVEN_1][3] = 8;
	m_nAreaNum[AEVEN_1][4] = 10;
	m_nAreaNum[AEVEN_1][5] = 12;
	m_nAreaNum[AEVEN_1][6] = 14;
	m_nAreaNum[AEVEN_1][7] = 16;
	m_nAreaNum[AEVEN_1][8] = 18;
	m_nAreaNum[AEVEN_1][9] = 20;
	m_nAreaNum[AEVEN_1][10] = 22;
	m_nAreaNum[AEVEN_1][11] = 24;
	m_nAreaNum[AEVEN_1][12] = 26;
	m_nAreaNum[AEVEN_1][13] = 28;
	m_nAreaNum[AEVEN_1][14] = 30;
	m_nAreaNum[AEVEN_1][15] = 32;
	m_nAreaNum[AEVEN_1][16] = 34;
	m_nAreaNum[AEVEN_1][17] = 36;

	m_nAreaNum[ALOW_1][0] = 1;
	m_nAreaNum[ALOW_1][1] = 2;
	m_nAreaNum[ALOW_1][2] = 3;
	m_nAreaNum[ALOW_1][3] = 4;
	m_nAreaNum[ALOW_1][4] = 5;
	m_nAreaNum[ALOW_1][5] = 6;
	m_nAreaNum[ALOW_1][6] = 7;
	m_nAreaNum[ALOW_1][7] = 8;
	m_nAreaNum[ALOW_1][8] = 9;
	m_nAreaNum[ALOW_1][9] = 10;
	m_nAreaNum[ALOW_1][10] = 11;
	m_nAreaNum[ALOW_1][11] = 12;
	m_nAreaNum[ALOW_1][12] = 13;
	m_nAreaNum[ALOW_1][13] = 14;
	m_nAreaNum[ALOW_1][14] = 15;
	m_nAreaNum[ALOW_1][15] = 16;
	m_nAreaNum[ALOW_1][16] = 17;
	m_nAreaNum[ALOW_1][17] = 18;

	m_nAreaNum[AHI_1][0] = 19;
	m_nAreaNum[AHI_1][1] = 20;
	m_nAreaNum[AHI_1][2] = 21;
	m_nAreaNum[AHI_1][3] = 22;
	m_nAreaNum[AHI_1][4] = 23;
	m_nAreaNum[AHI_1][5] = 24;
	m_nAreaNum[AHI_1][6] = 25;
	m_nAreaNum[AHI_1][7] = 26;
	m_nAreaNum[AHI_1][8] = 27;
	m_nAreaNum[AHI_1][9] = 28;
	m_nAreaNum[AHI_1][10] = 29;
	m_nAreaNum[AHI_1][11] = 30;
	m_nAreaNum[AHI_1][12] = 31;
	m_nAreaNum[AHI_1][13] = 32;
	m_nAreaNum[AHI_1][14] = 33;
	m_nAreaNum[AHI_1][15] = 34;
	m_nAreaNum[AHI_1][16] = 35;
	m_nAreaNum[AHI_1][17] = 36;

	return;
}

//��������
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//�ӿڲ�ѯ
VOID *  CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ�ӿ�
bool  CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem)!=NULL);
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);

	//�����ж�
	if (m_pIAndroidUserItem==NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//���ýӿ�
bool  CAndroidUserItemSink::RepositionSink()
{
	//��Ϸ����
	m_lMaxChipBanker = 0;
	m_lMaxChipUser = 0;
	m_wCurrentBanker = 0;
	m_nChipTime = 0;
	m_nChipTimeCount = 0;
	m_cbTimeLeave = 0;
	ZeroMemory(m_lAreaChip, sizeof(m_lAreaChip));
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));

	//��ׯ����
	m_bMeApplyBanker=false;
	m_nWaitBanker=0;
	m_nBankerCount=0;

	return true;
}

//ʱ����Ϣ
bool  CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_BANK_OPERATE:		//���в���
		{
			m_pIAndroidUserItem->KillGameTimer(nTimerID);

			//��������
			IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
			SCORE lRobotScore = pUserItem->GetUserScore();
			{

				//�жϴ�ȡ
				if (lRobotScore > m_lRobotScoreRange[1])
				{
					SCORE lSaveScore = 0;

					lSaveScore = SCORE(lRobotScore*m_nRobotBankStorageMul / 100);
					if (lSaveScore > lRobotScore)  lSaveScore = lRobotScore;

					if (lSaveScore > 0 && m_wCurrentBanker != m_pIAndroidUserItem->GetChairID())
						m_pIAndroidUserItem->PerformSaveScore(lSaveScore);

				}
				else if (lRobotScore < m_lRobotScoreRange[0])
				{
		            SCORE lScore=rand()%m_lRobotBankGetScoreBanker+m_lRobotBankGetScore;
		            if(lScore > 0)
		            {
		              m_pIAndroidUserItem->PerformTakeScore(lScore);
		            }
          		}
        	}
        	return false;
      }

	default:
		{
			if (nTimerID >= IDI_PLACE_JETTON && nTimerID <= IDI_PLACE_JETTON+MAX_CHIP_TIME)
			{
				BYTE cbBetType = rand() % 15 + 1;
				//��������
				CMD_C_PlaceJetton PlaceJetton;
				ZeroMemory(&PlaceJetton, sizeof(PlaceJetton));
				FillMemory(PlaceJetton.cbBetArea, sizeof(PlaceJetton.cbBetArea),255);
				if (cbBetType == BET_TYPE_STRAIGHT)
				{
					PlaceJetton.cbBetArea[0] = rand() % 37;
				}
				else if (cbBetType == BET_TYPE_SPLIT)
				{
					int nRandNum = rand() % 57;
					int nNumID = nRandNum + ASPLIT_1;
					CopyMemory(PlaceJetton.cbBetArea, m_nAreaNum[nNumID], sizeof(PlaceJetton.cbBetArea));
				}
				else if (cbBetType == BET_TYPE_STREET)
				{
					int nRandNum = rand() % 12;
					int nNumID = nRandNum + ASTREET_1;
					CopyMemory(PlaceJetton.cbBetArea, m_nAreaNum[nNumID], sizeof(PlaceJetton.cbBetArea));
				}
				else if (cbBetType == BET_TYPE_THREE)
				{
					int nRandNum = rand() % 2;
					int nNumID = nRandNum + ATHREE_1;
					CopyMemory(PlaceJetton.cbBetArea, m_nAreaNum[nNumID], sizeof(PlaceJetton.cbBetArea));
				}
				else if (cbBetType == BET_TYPE_CORNER)
				{
					int nRandNum = rand() % 22;
					int nNumID = nRandNum + ACORNER_1;
					CopyMemory(PlaceJetton.cbBetArea, m_nAreaNum[nNumID], sizeof(PlaceJetton.cbBetArea));
				}
				else if (cbBetType == BET_TYPE_FOUR)
				{
					CopyMemory(PlaceJetton.cbBetArea, m_nAreaNum[AFOUR_1], sizeof(PlaceJetton.cbBetArea));
				}
				else if (cbBetType == BET_TYPE_LINE)
				{
					int nRandNum = rand() % 11;
					int nNumID = nRandNum + ALINE_1;
					CopyMemory(PlaceJetton.cbBetArea, m_nAreaNum[nNumID], sizeof(PlaceJetton.cbBetArea));
				}
				else if (cbBetType == BET_TYPE_COLUMN)
				{
					int nRandNum = rand() % 3;
					int nNumID = nRandNum + ACOLUMN_1;
					CopyMemory(PlaceJetton.cbBetArea, m_nAreaNum[nNumID], sizeof(PlaceJetton.cbBetArea));
				}
				else if (cbBetType == BET_TYPE_DOZEN)
				{
					int nRandNum = rand() % 3;
					int nNumID = nRandNum + ADOZEN_1;
					CopyMemory(PlaceJetton.cbBetArea, m_nAreaNum[nNumID], sizeof(PlaceJetton.cbBetArea));
				}


		        //�������
				PlaceJetton.cbBetType = cbBetType;
				int nCurChip = rand() % MAX_CHIP_COUNT;
				if (m_RobotInfo.nChip[nCurChip] > m_nChipLimit[1])
					nCurChip = 0;
		        PlaceJetton.lBetScore = m_RobotInfo.nChip[nCurChip];

				int nNum1 = PlaceJetton.cbBetArea[0] == 255 ? 0 : PlaceJetton.cbBetArea[0];
				int nNum2 = PlaceJetton.cbBetArea[1] == 255 ? 0 : PlaceJetton.cbBetArea[1];
				PlaceJetton.nBetKeys = nNum1 * 100 + nNum2;

				//������Ϣ
				m_pIAndroidUserItem->SendSocketData(SUB_C_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));
			}

			m_pIAndroidUserItem->KillGameTimer(nTimerID);
			return false;
		}
	}
	return false;
}

//��Ϸ��Ϣ
bool  CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, VOID * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:			//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer, wDataSize);
		}
	case SUB_S_PLACE_JETTON:		//�û���ע
		{
			return OnSubPlaceJetton(pBuffer, wDataSize);
		}
	case SUB_S_GAME_END:			//��Ϸ���� 
		{
			return OnSubGameEnd(pBuffer, wDataSize);
		}
	case SUB_S_SEND_RECORD:			//��Ϸ��¼ (����)
		{
			return true;
		}
	case SUB_S_PLACE_JETTON_FAIL:	//��עʧ�� (����)
		{
			return true;
		}
	case SUB_S_ANDROID_CONFIG:
	{
		if (sizeof(tagCustomAndroid) == wDataSize)
		{
			tagCustomAndroid * pCustomAndroid = (tagCustomAndroid *)pBuffer;
			ReadConfigInformation(pCustomAndroid);
		}
		return true;
	}
  }

	return true;
}

//��Ϸ��Ϣ
bool  CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return true;
}

//������Ϣ
bool  CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_PLACE_JETTON:	//��Ϸ״̬
	case GS_GAME_END:		//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//��Ϣ����
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

     

			for (int i = 0; i < MAX_CHIP_COUNT; i++)
				m_RobotInfo.nChip[i] = pStatusPlay->lChips[i];

			return true;
		}
	}

	return true;
}

//�û�����
void  CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û��뿪
void  CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�����
void  CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�״̬
void  CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
//  //��ȡ����
//  if (m_bRefreshCfg)
//    ReadConfigInformation(m_RobotInfo.szCfgFileName, m_szRoomName, false);

	//��Ϣ����
	//CMD_S_GameFree* pGameFree=(CMD_S_GameFree *)pBuffer;

	//m_cbTimeLeave = pGameFree->cbTimeLeave;

	////���в���
	//if (pGameFree->cbTimeLeave > 1)
		m_pIAndroidUserItem->SetGameTimer(IDI_BANK_OPERATE, (rand() % 2 + 1));

  //bool bMeGiveUp = false;
  //if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID())
  //{
  //  m_nBankerCount++;
  //  if ( m_nBankerCount >= m_nRobotBankerCount )
  //  {
  //    //��������ׯ
  //    m_nBankerCount = 0;
  //    m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP_BANKER, rand()%2 + 1);

  //    bMeGiveUp = true;
  //  }
  //}

  ////�����ׯ
  //if (m_wCurrentBanker != m_pIAndroidUserItem->GetChairID() || bMeGiveUp)
  //{
  //  m_cbTimeLeave = pGameFree->cbTimeLeave - 3;
  //  m_pIAndroidUserItem->SetGameTimer(IDI_CHECK_BANKER, 3);
  //}

	return true;
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{	
	srand(GetTickCount());

	//���ñ���
	m_nChipTimeCount = 0;
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));
	for (int i = 0; i < AREA_COUNT; i++)
		m_lAreaChip[i] = m_lAreaLimitScore;

	//ϵͳ��ׯ
	m_lMaxChipUser = __min( m_lMaxChipUser, m_lMaxChipBanker );

	//������ע����
	int nElapse = 0;												
	WORD wMyID = m_pIAndroidUserItem->GetChairID();

	if (m_nRobotBetTimeLimit[0] == m_nRobotBetTimeLimit[1])
		m_nChipTime = m_nRobotBetTimeLimit[0];
	else
		m_nChipTime = (rand()+wMyID)%(m_nRobotBetTimeLimit[1]-m_nRobotBetTimeLimit[0]+1) + m_nRobotBetTimeLimit[0];
	ASSERT(m_nChipTime>=0);		
	if (m_nChipTime <= 0)	return false;								//��ȷ,2����������
	if (m_nChipTime > MAX_CHIP_TIME)	m_nChipTime = MAX_CHIP_TIME;	//�޶�MAX_CHIP����ע

	//���㷶Χ
  if (!CalcJettonRange(m_pIAndroidUserItem->GetMeUserItem()->GetUserScore(), m_lRobotJettonLimit, m_nChipTime, m_nChipLimit))
		return true;

	//����ʱ��
	int nTimeGrid = int(15-2)*800/m_nChipTime;		//ʱ���,ǰ2�벻��ע,����-2,800��ʾ��������עʱ�䷶Χǧ�ֱ�
	for (int i = 0; i < m_nChipTime; i++)
	{
		int nRandRage = int( nTimeGrid * i / (1500*sqrt((double)m_nChipTime)) ) + 1;		//������Χ
		nElapse = 2 + (nTimeGrid*i)/1000 + ( (rand()+wMyID)%(nRandRage*2) - (nRandRage-1) );
		ASSERT(nElapse>=2&&nElapse<=15);
		if (nElapse < 2 || nElapse > 15)	continue;
		
		m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON+i+1, nElapse);
	}

	//ASSERT( MyDebug(_T("������ %d ��ע���� %d ��Χ [%d %d] ������ %d ���� [%I64d %I64d] ��ׯ [%d %d]"), wMyID, m_nChipTime, m_nChipLimit[0], m_nChipLimit[1], 
	//	pGameStart->nChipRobotCount, m_lMaxChipBanker, m_lMaxChipUser, m_stlApplyBanker, m_nRobotApplyBanker) );

	return true;
}

//�û���ע
bool CAndroidUserItemSink::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) return false;

	//��Ϣ����
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;

	//���ñ���
	/*m_lMaxChipBanker -= pPlaceJetton->lJettonScore;
	m_lAreaChip[pPlaceJetton->cbJettonArea-1] -= pPlaceJetton->lJettonScore;
	if (pPlaceJetton->wChairID == m_pIAndroidUserItem->GetChairID())
		m_lMaxChipUser -= pPlaceJetton->lJettonScore;*/

	return true;
}

//��עʧ��
bool CAndroidUserItemSink::OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize)
{
	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	return true;
}

//������ׯ
bool CAndroidUserItemSink::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	

	return true;
}

//ȡ����ׯ
bool CAndroidUserItemSink::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{


	return true;
}

//�л�ׯ��
bool CAndroidUserItemSink::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{


	return true;
}

//��ȡ����
void CAndroidUserItemSink::ReadConfigInformation(tagCustomAndroid *pCustomAndroid)
{
  //��������
  m_nRobotBetTimeLimit[0] = pCustomAndroid->lRobotMinBetTime;
  m_nRobotBetTimeLimit[1] = pCustomAndroid->lRobotMaxBetTime;
  if(m_nRobotBetTimeLimit[0] < 0)
  {
    m_nRobotBetTimeLimit[0] = 0;
  }
  if(m_nRobotBetTimeLimit[1] < m_nRobotBetTimeLimit[0])
  {
    m_nRobotBetTimeLimit[1] = m_nRobotBetTimeLimit[0];
  }
  if(m_nRobotBetTimeLimit[0] == m_nRobotBetTimeLimit[1] && m_nRobotBetTimeLimit[0] == 0)
  {
    m_nRobotBetTimeLimit[0]=4;
    m_nRobotBetTimeLimit[1]=8;
  }
  //��������
  m_lRobotJettonLimit[0] = pCustomAndroid->lRobotMinJetton;
  m_lRobotJettonLimit[1] = pCustomAndroid->lRobotMaxJetton;
  if(m_lRobotJettonLimit[1] > 5000000)
  {
    m_lRobotJettonLimit[1] = 5000000;
  }
  if(m_lRobotJettonLimit[0] < 100)
  {
    m_lRobotJettonLimit[0] = 100;
  }
  if(m_lRobotJettonLimit[1] < m_lRobotJettonLimit[0])
  {
    m_lRobotJettonLimit[1] = m_lRobotJettonLimit[0];
  }

  //��������
  m_lRobotScoreRange[0] = pCustomAndroid->lRobotScoreMin;
  m_lRobotScoreRange[1] = pCustomAndroid->lRobotScoreMax;
  if(m_lRobotScoreRange[1] < m_lRobotScoreRange[0])
  {
    m_lRobotScoreRange[1] = m_lRobotScoreRange[0];
  }

  //�������
  LONGLONG lRobotBankGetScoreMin = pCustomAndroid->lRobotBankGetMin;
  LONGLONG lRobotBankGetScoreMax = pCustomAndroid->lRobotBankGetMax;
  if(lRobotBankGetScoreMax<lRobotBankGetScoreMin)
  {
    lRobotBankGetScoreMax=lRobotBankGetScoreMin;
  }

  m_lRobotBankGetScore=lRobotBankGetScoreMin+(rand()%(lRobotBankGetScoreMax-lRobotBankGetScoreMin+1))/10*10;
  //������� (ׯ��)
  LONGLONG lRobotBankGetScoreBankerMin = pCustomAndroid->lRobotBankGetBankerMin;
  LONGLONG lRobotBankGetScoreBankerMax = pCustomAndroid->lRobotBankGetBankerMax;
  if(lRobotBankGetScoreBankerMax<lRobotBankGetScoreMin)
  {
    lRobotBankGetScoreBankerMax=lRobotBankGetScoreMin;
  }
  m_lRobotBankGetScoreBanker = lRobotBankGetScoreBankerMin+(rand()%(lRobotBankGetScoreBankerMax-lRobotBankGetScoreBankerMin+1))/10*10;
  m_lRobotBankGetScore = m_lRobotBankGetScoreBanker;
  //����
  m_nRobotBankStorageMul = pCustomAndroid->lRobotBankStoMul;
  if(m_nRobotBankStorageMul<0 || m_nRobotBankStorageMul>100)
  {
    m_nRobotBankStorageMul = 20;
  }

  //�Ƿ���ׯ
  m_bRobotBanker = pCustomAndroid->nEnableRobotBanker;

  //��ׯ����
  LONGLONG lRobotBankerCountMin = pCustomAndroid->lRobotBankerCountMin;
  LONGLONG lRobotBankerCountMax = pCustomAndroid->lRobotBankerCountMax;
  if(lRobotBankerCountMax<lRobotBankerCountMin)
  {
    lRobotBankerCountMax = lRobotBankerCountMin;
  }
  m_nRobotBankerCount= rand()*(lRobotBankerCountMax-lRobotBankerCountMin+1)/RAND_MAX+lRobotBankerCountMin;
  //��������
  m_nRobotWaitBanker = pCustomAndroid->lRobotWaitBanker;
  //������
  m_nRobotApplyBanker=pCustomAndroid->lRobotApplyBanker;
  //��������
  m_bReduceJettonLimit = 0;
  //MyDebug(_T("������ ��ȡ���� [%I64d %I64d %d %d] %d [%d %d %d] ��ע %d �½� %d"), m_lRobotJettonLimit[0], m_lRobotJettonLimit[1],
  //  m_nRobotBetTimeLimit, m_nRobotBetTimeLimit, m_bRobotBanker, m_nRobotBankerCount, m_nRobotWaitBanker, m_nRobotApplyBanker, m_bReduceJettonLimit);
}

//���㷶Χ	(����ֵ��ʾ�Ƿ����ͨ���½����޴ﵽ��ע)
bool CAndroidUserItemSink::CalcJettonRange(SCORE lMaxScore, SCORE lChipLmt[], int & nChipTime, int lJetLmt[])
{
	//�������
	bool bHaveSetMinChip = false;

	//����һע
	if (lMaxScore < m_RobotInfo.nChip[0])	return false;

	//���÷�Χ
	for (int i = 0; i < CountArray(m_RobotInfo.nChip); i++)
	{
		if (!bHaveSetMinChip && m_RobotInfo.nChip[i] >= lChipLmt[0])
		{ 
			lJetLmt[0] = i;
			bHaveSetMinChip = true;
		}
		if (m_RobotInfo.nChip[i] <= lChipLmt[1])
			lJetLmt[1] = i;
	}
	if (lJetLmt[0] > lJetLmt[1])	lJetLmt[0] = lJetLmt[1];

	//�Ƿ񽵵�����
	if (m_bReduceJettonLimit)
	{
		if (nChipTime * m_RobotInfo.nChip[lJetLmt[0]] > lMaxScore)
		{
			//�Ƿ񽵵���ע����
			if (nChipTime * m_RobotInfo.nChip[0] > lMaxScore)
			{
				nChipTime = int(lMaxScore/m_RobotInfo.nChip[0]);
				lJetLmt[0] = 0;
				lJetLmt[1] = 0;
			}
			else
			{
				//���͵���������
				while (nChipTime * m_RobotInfo.nChip[lJetLmt[0]] > lMaxScore)
				{
					lJetLmt[0]--;
					ASSERT(lJetLmt[0]>=0);
				}
			}
		}
	}

	return true;
}

//��ȡ����
LONGLONG CAndroidUserItemSink::GetPrivateProfileInt64(LPCTSTR lpAppName, LPCTSTR lpKeyName, LONGLONG lDefault, LPCTSTR lpFileName)
{
	//��������
	TCHAR OutBuf[255] = _T("");
	TCHAR DefaultBuf[255] = {};
	LONGLONG lNumber = 0;

	GetPrivateProfileString(lpAppName, lpKeyName, DefaultBuf, OutBuf, 255, lpFileName);

	if (OutBuf[0] != 0)
		_sntscanf(OutBuf, lstrlen(OutBuf), _T("%I64d"), &lNumber);
	else
		lNumber = lDefault;

	return lNumber;
}

//////////////////////////////////////////////////////////////////////////
