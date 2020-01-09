// MainDlg.cpp: 实现文件
//

#include "pch.h"
#include "MyQQ.h"
#include "MainDlg.h"
#include "afxdialogex.h"
#include "CDlg1.h"
#include "CDlg2.h"
#include "CAddFriend.h"
#include "Data.h"
#include "CChatDlg.h"
#include "CCreateDlg.h"
#include "CAddGroup.h"
#include "CGroupChat.h"
#define UM_Message (WM_USER+1000)
#define UM_UPDATEMSG (WM_USER+1001)
#define UM_FLUSHEND WM_USER+1002
#define UM_UPDATEGROUPMSG WM_USER + 1003
// MainDlg 对话框

IMPLEMENT_DYNAMIC(MainDlg, CDialogEx)

MainDlg::MainDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MainDlg, pParent)
{

}

MainDlg::~MainDlg()
{
}

void MainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_MyTab);
}


BEGIN_MESSAGE_MAP(MainDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &MainDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &MainDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &MainDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


//*****************************************************************************************
// 函数名称: FlushFriend
// 函数说明: 刷新好友列表的线程
// 作    者: lracker
// 时    间: 2019/09/17
// 参    数: LPVOID args
// 返 回 值: DWORD CALLBACK
//*****************************************************************************************
DWORD CALLBACK FlushFriend(LPVOID args)
{
	g_vFriendList.clear();  // 先清空
	char* buffer = (char*)args;  // 将数据转换为char型
	char* splitData;
	CString size(strtok_s(buffer, "\n", &splitData));
	int nSize = _ttoi(size);
	for(int i=0; i < nSize; i++)
	{
		CString temp(strtok_s(NULL, "\n", &splitData));
		g_vFriendList.push_back(temp);
	}
	// 刷新好友列表
	CDlg1* temp = (CDlg1*)g_Dlg[L"Friend"];
	temp->GetFriend();
	return TRUE;
}

DWORD CALLBACK AddFriend(LPVOID args)
{
	char* buffer = (char*)args;  // 将数据转换为char型
	if(*buffer == 1)
	{
		char* buffer2 = buffer + 8;
		USES_CONVERSION;
		WCHAR* wBuffer = A2W(buffer2);
		int nRet = MessageBox(NULL, wBuffer, L"好友申请", MB_YESNO);
		char* buffer3 = 0;
		strtok_s(buffer2, "想", &buffer3);
		CString FriNum(buffer2);
		CString MyNum(g_UserId);
		if (nRet == IDYES)
		{
		
			CString Data = FriNum + '\n' + MyNum + '\n' + "ADDFRIENDCOMFIRM";
			char* cData = W2A(Data);
			g_Server.Send(ADDFRIENDCOMFIRM, cData); // 将本id与好友id一起发过去
		}
		else if (nRet == IDNO)
		{
			CString Data = FriNum + '\n' + MyNum + '\n' + "ERR";
			char* cData = W2A(Data);
			g_Server.Send(ADDFRIENDCOMFIRM, cData);
		}
	}
	if (*buffer == 0)
	{
		char* buffer2 = buffer + 8;
		USES_CONVERSION;
		WCHAR* wBuffer = A2W(buffer2);
		MessageBox(NULL, wBuffer, L"好友申请回复", MB_OK);
	}
	// 刷新好友列表
	// 发送正在使用的userId
	g_Server.Send(FRIENDLIST, g_UserId);
	return TRUE;
}

DWORD CALLBACK SMessage(LPVOID args)
{
	char* Stauts = (char*)args;
	char* splitData = (char*)args;
	int s = *Stauts;
	//char* UserId = (char*)args;
	WaitForSingleObject(g_hMutex, INFINITE);
	splitData += 8;
	if (s == 0)  // 如果是发送给好友窗口的话
	{
		CString userid(strtok_s(NULL, "\n", &splitData));  //切割出userId
		USES_CONVERSION;
		char* UserId = W2A(userid);
		CChatDlg* temp = (CChatDlg*)g_Dlg[L"Friend"];
		SendMessage(temp->m_hWnd, UM_Message, (WPARAM)UserId, 0);   // 发送给好友列表，让他在id后面显示有消息
	}
	else if (s == 1) // 如果是发送给群聊框的
	{
		CString roomid(strtok_s(NULL, "\n", &splitData));  //切割出userId
		USES_CONVERSION;
		char* RoomId = W2A(roomid);
		CGroupChat* temp = (CGroupChat*)g_Dlg[L"Group"];
		SendMessage(temp->m_hWnd, UM_Message, (WPARAM)RoomId, 0);   // 发送给好友列表，让他在id后面显示有消息
	}

	ReleaseMutex(g_hMutex);
	return TRUE;
}

DWORD CALLBACK SData(LPVOID args)
{
	char* choice = (char*)args;  // 将数据转换为char型,获取是发给聊天窗口还是查看记录窗口
	int nChoice = *choice;
	char* friendid = (char*)args+8;  // 将数据转换为char型
	char* splitData;
	strtok_s(friendid, "\n", &splitData);  // 获取到friendid
	CString FriendId(friendid);
	CString num(strtok_s(NULL, "\n", &splitData));
	int nNum = _ttoi(num);
	g_vChatRecord.clear();  //清空掉容器
	for (int i = 0; i < nNum; i++)
	{
		CString temp(strtok_s(NULL, "\n", &splitData));
		g_vChatRecord.push_back(temp);  // 将信息存放到容器里
	}
	if(nChoice == 1)
	{
		CChatDlg* temp = (CChatDlg*)g_Dlg[FriendId];
		SendMessage(temp->m_hWnd, UM_UPDATEMSG, 0, 0);  // 发送给该窗口
	}
	else if (nChoice == 0)
	{
		CString temps = FriendId + "record";
		CChatDlg* temp = (CChatDlg*)g_Dlg[temps];
		SendMessage(temp->m_hWnd, UM_UPDATEMSG, 0, 0);  // 发送给该查看记录的窗口
	}
	return TRUE;
}

DWORD CALLBACK FlushGroup(LPVOID args)
{
	g_vGroupList.clear();  // 先清空
	char* buffer = (char*)args;  // 将数据转换为char型
	char* splitData;
	CString size(strtok_s(buffer, "\n", &splitData));
	int nSize = _ttoi(size);
	for (int i = 0; i < nSize; i++)
	{
		CString temp(strtok_s(NULL, "\n", &splitData));
		g_vGroupList.push_back(temp);
	}
	// 刷新群的列表
	CDlg2* temp = (CDlg2*)g_Dlg[L"Group"];
	temp->GetGroup();
	return TRUE;
}

DWORD CALLBACK FlushGroupMember(LPVOID args)
{
	g_vGroupMemberList.clear();  // 先清空
	char* buffer = (char*)args;  // 将数据转换为char型
	char* splitData;
	CString RoomNum(strtok_s(buffer, "\n", &splitData));
	CString size(strtok_s(NULL, "\n", &splitData));
	int nSize = _ttoi(size);
	for (int i = 0; i < nSize; i++)
	{
		CString temp(strtok_s(NULL, "\n", &splitData));
		g_vGroupMemberList.push_back(temp);
	}
	// 刷新群的列表
	CGroupChat* temp = (CGroupChat*)g_Dlg[RoomNum];
	SendMessage(temp->m_hWnd, UM_FLUSHEND, 0, 0);
	return TRUE;
}

DWORD CALLBACK SGData(LPVOID args)
{
	char* choice = (char*)args;  // 将数据转换为char型,获取是发给聊天窗口还是查看记录窗口
	int nChoice = *choice;
	char* roomid = (char*)args + 8;  // 将数据转换为char型
	char* splitData;
	strtok_s(roomid, "\n", &splitData);  // 获取到friendid
	CString RoomId(roomid);
	CString num(strtok_s(NULL, "\n", &splitData));
	int nNum = _ttoi(num);
	g_vChatRecord.clear();  //清空掉容器
	for (int i = 0; i < nNum; i++)
	{
		CString temp(strtok_s(NULL, "\n", &splitData));
		g_vChatRecord.push_back(temp);  // 将信息存放到容器里
	}
	if (nChoice == 1)
	{
		CGroupChat* temp = (CGroupChat*)g_Dlg[RoomId];
		SendMessage(temp->m_hWnd, UM_UPDATEGROUPMSG, 0, 0);  // 发送给该窗口
	}
	else if (nChoice == 0)
	{
		CString temps = RoomId + "record";
		CGroupChat* temp = (CGroupChat*)g_Dlg[temps];
		SendMessage(temp->m_hWnd, UM_UPDATEGROUPMSG, 0, 0);  // 发送给该查看记录的窗口
	}
	return TRUE;
}
// MainDlg 消息处理程序

DWORD CALLBACK RecvProc()
{
	while (TRUE)
	{
		RECVDATASTRUCT* pRecv = g_Server.Recv();  // 接收服务器发过来的信息
		switch (pRecv->Type)
		{
		case FRIENDLIST:
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)FlushFriend, (LPVOID)pRecv->data, NULL, NULL);
			break;
		case ADDFRIEND:
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)AddFriend, (LPVOID)&pRecv->status, NULL, NULL);
			break;
		case SENDMESSAGE:
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SMessage, (LPVOID)&pRecv->status, NULL, NULL);
			break;
		case SEARCHDATA:
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SData, (LPVOID)&pRecv->status, NULL, NULL);
			break;
		case GROUPLIST:
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)FlushGroup, (LPVOID)pRecv->data, NULL, NULL);
			break;
		case GROUPMEMBERFLUSH:
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)FlushGroupMember, (LPVOID)pRecv->data, NULL, NULL);
			break;
		case SEARCHGROUPDATA:
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SGData, (LPVOID)& pRecv->status, NULL, NULL);
			break;
		case SYS:
			CString Msg(pRecv->data);
			MessageBox(NULL, Msg, L"系统消息", MB_OK);
			// 刷新群的列表
			g_Server.Send(GROUPLIST, g_UserId);
			break;
		}

	}
	return true;
}

//*****************************************************************************************
// 函数名称: OnInitDialog
// 函数说明: 初始化主对话框
// 作    者: lracker
// 时    间: 2019/09/17
// 返 回 值: BOOL
//*****************************************************************************************
BOOL MainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 插入菜单选项
	m_MyTab.InsertItem(0, L"好友");
	m_MyTab.InsertItem(1, L"群聊");
	// 给子窗口指针赋值
	m_MyTab.m_Dlg[0] = new CDlg1();
	g_Dlg[L"Friend"] = m_MyTab.m_Dlg[0];
	m_MyTab.m_Dlg[1] = new CDlg2();
	g_Dlg[L"Group"] = m_MyTab.m_Dlg[1];
	/*创建子窗口*/
	m_MyTab.m_Dlg[0]->Create(IDD_FRIEND, &m_MyTab);
	m_MyTab.m_Dlg[1]->Create(IDD_GROUP, &m_MyTab);
	/*获取坐标*/
	CRect pos = {};
	m_MyTab.GetClientRect(pos);  //转换为客户端的坐标
	pos.DeflateRect(2, 30, 3, 2);    //移动坐标，免得盖住菜单
	/*移动窗口*/
	m_MyTab.m_Dlg[0]->MoveWindow(pos);
	m_MyTab.m_Dlg[1]->MoveWindow(pos);
	/*显示窗口*/
	m_MyTab.m_Dlg[0]->ShowWindow(SW_SHOW);
	m_MyTab.m_Dlg[1]->ShowWindow(SW_HIDE);
	// 创建一个互斥体
	g_hMutex = CreateMutex(NULL, NULL, L"聊天室");
	// 创建一个消息主线程来接收所有来自服务端的消息
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)RecvProc, NULL, NULL, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


//*****************************************************************************************
// 函数名称: OnBnClickedButton1
// 函数说明: 添加好友
// 作    者: lracker
// 时    间: 2019/09/18
// 返 回 值: void
//*****************************************************************************************
void MainDlg::OnBnClickedButton1()
{
	CAddFriend Addobj;
	Addobj.DoModal();
}


//*****************************************************************************************
// 函数名称: OnBnClickedButton2
// 函数说明: 创建群聊
// 作    者: lracker
// 时    间: 2019/09/18
// 返 回 值: void
//*****************************************************************************************
void MainDlg::OnBnClickedButton2()
{
	CCreateDlg obj;
	obj.DoModal();
}


//*****************************************************************************************
// 函数名称: OnBnClickedButton3
// 函数说明: 加入群聊
// 作    者: lracker
// 时    间: 2019/09/18
// 返 回 值: void
//*****************************************************************************************
void MainDlg::OnBnClickedButton3()
{
	CAddGroup obj;
	obj.DoModal();
}
