
#ifndef ANDROID_USER_ITEM_SINK_H_
#define ANDROID_USER_ITEM_SINK_H_

// ��������
class CAndroidUserItemSink : public IAndroidUserItemSink
{
protected:
	IAndroidUserItem *				m_pIAndroidUserItem;				//�û��ӿ�

	//��������
public:
	//���캯��
	CAndroidUserItemSink();
	//��������
	virtual ~CAndroidUserItemSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { }
	//�Ƿ���Ч
	virtual bool IsValid() { return AfxIsValidAddress(this, sizeof(CAndroidUserItemSink)) ? true : false; }
	//�ӿڲ�ѯ
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���ƽӿ�
public:
	//��ʼ�ӿ�
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//���ýӿ�
	virtual bool RepositionSink();

	//��Ϸ�¼�
public:
	//ʱ����Ϣ
	virtual bool OnEventTimer(UINT nTimerID);
	//��Ϸ��Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
	//��Ϸ��Ϣ
	virtual bool OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize);

	//�û��¼�
public:
	//�û�����
	virtual void OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser) {}
	//�û��뿪
	virtual void OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser) {}
	//�û�����
	virtual void OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser) {}
	//�û�״̬
	virtual void OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser) {}
	//�û���λ
	virtual void OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser) {}
};

#endif  // ANDROID_USER_ITEM_SINK_H_
