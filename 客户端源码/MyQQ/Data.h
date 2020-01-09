#pragma once
#include "CMySocket.h"
#include <vector>
#include <map>
using std::vector;
using std::map;

extern CMySocket g_Server;  // 在构造函数里包含了连接服务器的函数
extern char g_UserId[17];   // 多一位是给结束符的
extern vector<CString> g_vFriendList;
extern vector<CString> g_vGroupList;
extern vector<CString> g_vGroupMemberList;
extern map<CString, CDialogEx*> g_Dlg;
extern HANDLE g_hMutex;
extern vector<CString> g_vChatRecord;
