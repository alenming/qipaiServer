#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��ֵ����
#define LOGIC_MASK_COLOR      0xF0                //��ɫ����
#define LOGIC_MASK_VALUE      0x0F                //��ֵ����

//�˿�����
#define SG_EIGHT                8                 //�˵�
#define SG_NINE                 9                 //�ŵ�
#define SG_THREE_KING			10		          //��������������JQK
#define SG_THREEE_SAME			11			      //������������һ��


//////////////////////////////////////////////////////////////////////////


//�����ṹ
struct tagAnalyseResult
{
  BYTE              cbFourCount;				  //������Ŀ
  BYTE              cbThreeCount;				  //������Ŀ
  BYTE              cbDoubleCount;				  //������Ŀ
  BYTE              cbSignedCount;				  //������Ŀ
  BYTE              cbFourLogicVolue[1];          //�����б�
  BYTE              cbThreeLogicVolue[1];		  //�����б�
  BYTE              cbDoubleLogicVolue[2];        //�����б�
  BYTE              cbSignedLogicVolue[5];        //�����б�
  BYTE              cbFourCardData[MAX_COUNT];    //�����б�
  BYTE              cbThreeCardData[MAX_COUNT];   //�����б�
  BYTE              cbDoubleCardData[MAX_COUNT];  //�����б�
  BYTE              cbSignedCardData[MAX_COUNT];  //������Ŀ
};

//��־���
/////////////////////////////////////////////////////////////////////////////////
static CString GetFileDialogPath()
{
  CString strFileDlgPath;
  TCHAR szModuleDirectory[MAX_PATH];  //ģ��Ŀ¼
  GetModuleFileName(AfxGetInstanceHandle(),szModuleDirectory,sizeof(szModuleDirectory));
  int nModuleLen=lstrlen(szModuleDirectory);
  int nProcessLen=lstrlen(AfxGetApp()->m_pszExeName)+lstrlen(TEXT(".EXE")) + 1;
  if(nModuleLen<=nProcessLen)
  {
    return TEXT("");
  }
  szModuleDirectory[nModuleLen-nProcessLen]=0;
  strFileDlgPath = szModuleDirectory;
  return strFileDlgPath;
}

static void NcaTextOut(CString strInfo)
{
  CString strName = GetFileDialogPath()+  "\\SG.log";

  CTime time = CTime::GetCurrentTime() ;
  CString strTime ;
  strTime.Format(TEXT("%d-%d-%d %d:%d:%d") ,
                 time.GetYear() ,
                 time.GetMonth() ,
                 time.GetDay() ,
                 time.GetHour() ,
                 time.GetMinute() ,
                 time.GetSecond()
                );
  CString strMsg;
  strMsg.Format(TEXT("%s,%s\r\n"),strTime,strInfo);

  CFile fLog;
  if(fLog.Open(strName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite))
  {

    fLog.SeekToEnd();
    int strLength=strMsg.GetLength();
#ifdef _UNICODE
    BYTE bom[2] = {0xff, 0xfe};
    fLog.Write(bom,sizeof(BYTE)*2);
    strLength*=2;
#endif
    fLog.Write((LPCTSTR)strMsg,strLength);
    fLog.Close();
  }

}

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
  //��������
public:
  static BYTE           m_cbCardListData[52];       //�˿˶���

  //��������
public:
  //���캯��
  CGameLogic();
  //��������
  virtual ~CGameLogic();

  //���ͺ���
public:
  //��ȡ����
  BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount);
  //��ȡ��ֵ
  BYTE GetCardValue(BYTE cbCardData)
  {
    return cbCardData & LOGIC_MASK_VALUE;
  }
  //��ȡ��ɫ
  BYTE GetCardColor(BYTE cbCardData)
  {
    return cbCardData&LOGIC_MASK_COLOR;
  }
  //��ȡ����
  BYTE GetTimes(BYTE cbCardData[], BYTE cbCardCount);

  //��ȡ����
  bool IsIntValue(BYTE cbCardData[], BYTE cbCardCount);

  //���ƺ���
public:
  void GetDebugCards(BYTE cbCardData[], BYTE cbCardCount);
  //�����˿�
  void SortCardList(BYTE cbCardData[], BYTE cbCardCount);
  //�����˿�
  void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
  //��ȡ�˿�
  int GetCards(BYTE outCards[], BYTE outLen, BYTE repoCards[], BYTE repoLen, BYTE cCards[] = NULL, BYTE cLen = 0);

  //���ܺ���
public:
  //�߼���ֵ
  BYTE GetCardLogicValue(BYTE cbCardData, BOOL & isKing);
  //�Ա��˿�
  bool CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount);
  //�������
  VOID SortPlayerValue(BYTE cbPlayerValue[], BYTE cbCount);
  //��ȡ��Ҵ�С����
  VOID GetPlayerOrder(CONST BYTE cbHandCard[], CONST BYTE cbPlayerValue[], BYTE cbPlayerOrder[], BYTE cbCount);

};

//////////////////////////////////////////////////////////////////////////

#endif
