#ifndef CONTROL_PACKET_HEAD_FILE
#define CONTROL_PACKET_HEAD_FILE

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//�¼����
#define ER_FAILURE					0									//ʧ�ܱ�־
#define ER_SUCCESS					1									//�ɹ���־

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//�������ݰ�

#define UI_CORRESPOND_RESULT		1									//Э�����
#define UI_SERVICE_CONFIG_RESULT	2									//���ý��

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661
//�������ݰ�

#define CT_CONNECT_CORRESPOND		100									//����Э��
#define CT_LOAD_SERVICE_CONFIG		101									//��������

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

//���ƽ��
struct CP_ControlResult
{
	BYTE							cbSuccess;							//�ɹ���־
};

/////////////////////////////////��������è����Ϸ�����������ֵ18��QQ:361283661

#endif