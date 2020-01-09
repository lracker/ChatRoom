// CChatDlg.cpp: 实现文件
//

#include "pch.h"
#include "MyQQ.h"
#include "CChatDlg.h"
#include "afxdialogex.h"
#include "Data.h"
#include "CChatRecod.h"
#define UM_Message (WM_USER+1000)
#define UM_UPDATEMSG (WM_USER+1001)
// CChatDlg 对话框

IMPLEMENT_DYNAMIC(CChatDlg, CDialogEx)

CChatDlg::CChatDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHAT, pParent)
{

}

CChatDlg::~CChatDlg()
{

}

void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_SendMessage);
	DDX_Control(pDX, IDC_EDIT2, m_GetMessage);
}


BEGIN_MESSAGE_MAP(CChatDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CChatDlg::OnBnClickedButton1)
	ON_MESSAGE(UM_Message, &CChatDlg::OnUmMessage)
	ON_MESSAGE(UM_UPDATEMSG, &CChatDlg::OnUmUpdatemsg)
	ON_BN_CLICKED(IDC_BUTTON2, &CChatDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CChatDlg 消息处理程序


//*****************************************************************************************
// 函数名称: OnBnClickedButton1
// 函数说明: 获取编辑框内容并且发送信息
// 作    者: lracker
// 时    间: 2019/09/17
// 返 回 值: void
//*****************************************************************************************
void CChatDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	CString Message;
	m_SendMessage.GetWindowTextW(Message);
	if (Message.IsEmpty())
	{
		MessageBox(L"消息不能为空");
		return;
	}
	m_SendMessage.Clear();
	UpdateData(FALSE);
	CString Data = m_FriendNum + '\n' + Message;    // 好友的id+'\n'+Message
	// 转换为char类型
	USES_CONVERSION;
	char* buffer = W2A(Data);
	g_Server.Send(SENDMESSAGE, buffer);
}


BOOL CChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



//*****************************************************************************************
// 函数名称: SearchData
// 函数说明: 查询数据库
// 作    者: lracker
// 时    间: 2019/09/19
// 参    数: WCHAR * buffer
// 返 回 值: void
//*****************************************************************************************
void CChatDlg::SearchData(WCHAR* buffer)
{
	CString Data(buffer);
	Data += '\n';
	Data += "chat";
	USES_CONVERSION;
	char* data = W2A(Data);
	g_Server.Send(SEARCHDATA, data);  // 将好友账号都发过去给服务器
}

//*****************************************************************************************
// 函数名称: OnUmMessage
// 函数说明: 消息框获取消息
// 作    者: lracker
// 时    间: 2019/09/18
// 参    数: WPARAM wParam
// 参    数: LPARAM lParam
// 返 回 值: LRESULT
//*****************************************************************************************
afx_msg LRESULT CChatDlg::OnUmMessage(WPARAM wParam, LPARAM lParam)
{
	WCHAR* buffer = (WCHAR*)wParam;
	SearchData(buffer);
	return 0;
}



//*****************************************************************************************
// 函数名称: OnCancel
// 函数说明: 关闭窗口的时候
// 作    者: lracker
// 时    间: 2019/09/19
// 返 回 值: void
//*****************************************************************************************
void CChatDlg::OnCancel()
{
	//从字典中删掉自己的窗口指针
	g_Dlg.erase(m_FriendNum);
	CDialogEx::OnCancel();
}


//*****************************************************************************************
// 函数名称: OnUmUpdatemsg
// 函数说明: 打印对话框，响应UM_UPDATEMSG消息
// 作    者: lracker
// 时    间: 2019/09/18
// 参    数: WPARAM wParam
// 参    数: LPARAM lParam
// 返 回 值: LRESULT
//*****************************************************************************************
afx_msg LRESULT CChatDlg::OnUmUpdatemsg(WPARAM wParam, LPARAM lParam)
{
	WaitForSingleObject(g_hMutex, INFINITE);
	UpdateData(TRUE);
	m_GetMessage.Clear();  // 清空
	CString str;
	for (auto& i : g_vChatRecord)
	{
		str += i;
		str += "\r\n";
		m_GetMessage.SetWindowText(str);
	}
	UpdateData(FALSE);
	ReleaseMutex(g_hMutex);
	int nline = m_GetMessage.GetLineCount();
	m_GetMessage.LineScroll(nline - 1);
	return 0;
}


//*****************************************************************************************
// 函数名称: OnBnClickedButton2
// 函数说明: 查看聊天记录
// 作    者: lracker
// 时    间: 2019/09/18
// 返 回 值: void
//*****************************************************************************************
void CChatDlg::OnBnClickedButton2()
{
	CChatRecod* obj = new CChatRecod();
	obj->Create(IDD_ChatRecord);
	obj->ShowWindow(SW_SHOW);
	CString temp = m_FriendNum + "record";
	g_Dlg[temp] = obj;
	// 发送查看聊天记录的请求
	CString user2(m_FriendNum);
	CString Data = user2 + '\n' + "record";
	USES_CONVERSION;
	char* data = W2A(Data);
	g_Server.Send(SEARCHDATA, data);
}
