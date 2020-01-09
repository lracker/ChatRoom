// CGroupChat.cpp: 实现文件
//

#include "pch.h"
#include "MyQQ.h"
#include "CGroupChat.h"
#include "afxdialogex.h"
#include "Data.h"
#include <vector>
#include <algorithm>
using std::vector;
#define UM_Message WM_USER+1000
#define UM_FLUSH WM_USER+1001
#define UM_FLUSHEND WM_USER+1002
#define UM_UPDATEGROUPMSG WM_USER+1003
// CGroupChat 对话框

IMPLEMENT_DYNAMIC(CGroupChat, CDialogEx)

CGroupChat::CGroupChat(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GroupChat, pParent)
{

}

CGroupChat::~CGroupChat()
{
}

void CGroupChat::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_GoupMember);
	DDX_Control(pDX, IDC_EDIT1, m_GetMsg);
	DDX_Control(pDX, IDC_EDIT2, m_SendMsg);
}


BEGIN_MESSAGE_MAP(CGroupChat, CDialogEx)
	ON_MESSAGE(UM_FLUSH, &CGroupChat::OnUmFlush)
	ON_MESSAGE(UM_FLUSHEND, &CGroupChat::OnUmFlushend)
	ON_BN_CLICKED(IDC_BUTTON1, &CGroupChat::OnBnClickedButton1)
	ON_MESSAGE(UM_Message, &CGroupChat::OnUmMessage)
	ON_MESSAGE(UM_UPDATEGROUPMSG, &CGroupChat::OnUmUpdategroupmsg)
END_MESSAGE_MAP()


// CGroupChat 消息处理程序


//*****************************************************************************************
// 函数名称: OnUmFlush
// 函数说明: 向服务器请求刷新
// 作    者: lracker
// 时    间: 2019/09/18
// 参    数: WPARAM wParam
// 参    数: LPARAM lParam
// 返 回 值: LRESULT
//*****************************************************************************************
afx_msg LRESULT CGroupChat::OnUmFlush(WPARAM wParam, LPARAM lParam)
{
	USES_CONVERSION;
	char* buffer = W2A(m_GroupNum);
	g_Server.Send(GROUPMEMBERFLUSH, buffer);  // 发送房间号码过去
	return 0;
}


BOOL CGroupChat::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//初始化列表
	m_GoupMember.InsertColumn(0, L"群友id", 0, 190);
	m_GoupMember.SetExtendedStyle(LVS_EX_FULLROWSELECT |  //选中整行
		LVS_EX_GRIDLINES);      //网格线
	int nline = m_GetMsg.GetLineCount();
	m_GetMsg.LineScroll(nline - 1);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


//*****************************************************************************************
// 函数名称: OnUmFlushend
// 函数说明: 更新群友列表
// 作    者: lracker
// 时    间: 2019/09/19
// 参    数: WPARAM wParam
// 参    数: LPARAM lParam
// 返 回 值: LRESULT
//*****************************************************************************************
afx_msg LRESULT CGroupChat::OnUmFlushend(WPARAM wParam, LPARAM lParam)
{
	m_GoupMember.DeleteAllItems();

	int j = 0;
	for (auto& i : g_vGroupMemberList)
	{
		m_GoupMember.InsertItem(j++, i);
	}
	return 0;
}

//*****************************************************************************************
// 函数名称: SearchData 
// 函数说明: 查询数据库
// 作    者: lracker
// 时    间: 2019/09/19
// 参    数: WCHAR * buffer
// 返 回 值: void
//*****************************************************************************************
void CGroupChat::SearchData(WCHAR* buffer)
{
	CString Data(buffer);
	Data += '\n';
	Data += "chat";
	USES_CONVERSION;
	char* data = W2A(Data);		// 房间号+'\n'+'chat'
	g_Server.Send(SEARCHGROUPDATA, data);  // 将房间账号都发过去给服务器
}

//*****************************************************************************************
// 函数名称: OnBnClickedButton1
// 函数说明: 发送群聊信息
// 作    者: lracker
// 时    间: 2019/09/19
// 返 回 值: void
//*****************************************************************************************
void CGroupChat::OnBnClickedButton1()
{
	UpdateData(TRUE);
	CString Message;
	m_SendMsg.GetWindowTextW(Message);
	if (Message.IsEmpty())
	{
		MessageBox(L"消息不能为空");
		return;
	}
	m_SendMsg.Clear();
	UpdateData(FALSE);
	CString Data = m_GroupNum + '\n' + Message;   // 群的id+'\n'+Message
	// 转换为char类型
	USES_CONVERSION;
	char* buffer = W2A(Data);
	g_Server.Send(SENDGROUPMESSAGE, buffer);
}


//*****************************************************************************************
// 函数名称: OnUmMessage
// 函数说明: 有消息通知
// 作    者: lracker
// 时    间: 2019/09/19
// 参    数: WPARAM wParam
// 参    数: LPARAM lParam
// 返 回 值: LRESULT
//*****************************************************************************************
afx_msg LRESULT CGroupChat::OnUmMessage(WPARAM wParam, LPARAM lParam)
{
	WCHAR* buffer = (WCHAR*)wParam;
	SearchData(buffer);
	return 0;
}


//*****************************************************************************************
// 函数名称: OnUmUpdategroupmsg
// 函数说明: 更新聊天窗口
// 作    者: lracker
// 时    间: 2019/09/19
// 参    数: WPARAM wParam
// 参    数: LPARAM lParam
// 返 回 值: LRESULT
//*****************************************************************************************
afx_msg LRESULT CGroupChat::OnUmUpdategroupmsg(WPARAM wParam, LPARAM lParam)
{
	WaitForSingleObject(g_hMutex, INFINITE);
	UpdateData(TRUE);
	m_GetMsg.Clear();  // 清空
	CString str;
	for (auto& i : g_vChatRecord)
	{
		str += i;
		str += "\r\n";
		m_GetMsg.SetWindowText(str);
	}
	UpdateData(FALSE);
	ReleaseMutex(g_hMutex);
	int nline = m_GetMsg.GetLineCount();
	m_GetMsg.LineScroll(nline - 1);
	return 0;
}


void CGroupChat::OnCancel()
{
	//从字典中删掉自己的窗口指针
	g_Dlg.erase(m_GroupNum);
	CDialogEx::OnCancel();
}
