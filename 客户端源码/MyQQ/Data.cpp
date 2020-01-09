#include "pch.h"
#include "CMySocket.h"
#include "Data.h"

CMySocket g_Server;
char g_UserId[17] = { 0 };
vector<CString> g_vFriendList;
vector<CString> g_vGroupList;
vector<CString> g_vGroupMemberList;
map<CString, CDialogEx*> g_Dlg;
HANDLE g_hMutex;
vector<CString> g_vChatRecord;
