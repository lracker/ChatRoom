#pragma once
#include "CMySocket.h"
#include <vector>
#include <map>
using std::vector;
using std::map;

extern CMySocket g_Server;  // �ڹ��캯������������ӷ������ĺ���
extern char g_UserId[17];   // ��һλ�Ǹ���������
extern vector<CString> g_vFriendList;
extern vector<CString> g_vGroupList;
extern vector<CString> g_vGroupMemberList;
extern map<CString, CDialogEx*> g_Dlg;
extern HANDLE g_hMutex;
extern vector<CString> g_vChatRecord;
