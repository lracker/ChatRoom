// CDlg1.cpp: 实现文件
//

#include "pch.h"
#include "MyQQ.h"
#include "CDlg1.h"
#include "afxdialogex.h"
#include "Data.h"
#include "Engine.h"
#include "CChatDlg.h"
#include "resource.h"
#define UM_Message (WM_USER+1000)


// CDlg1 对话框

IMPLEMENT_DYNAMIC(CDlg1, CDialogEx)

CDlg1::CDlg1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FRIEND, pParent)
{

}

CDlg1::~CDlg1()
{
}

void CDlg1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_FriendList);
}


BEGIN_MESSAGE_MAP(CDlg1, CDialogEx)

	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDlg1::OnDblclkList1)
	ON_MESSAGE(UM_Message, &CDlg1::OnUmMessage)
END_MESSAGE_MAP()


// CDlg1 消息处理程序


//*****************************************************************************************
// 函数名称: OnInitDialog
// 函数说明: 初始化列表
// 作    者: lracker
// 时    间: 2019/09/17
// 返 回 值: BOOL
//*****************************************************************************************
BOOL CDlg1::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_FriendList.InsertColumn(0, L"UserId", 0, 155);
	m_FriendList.InsertColumn(1, L"是否有新消息", 0, 155);
	m_FriendList.SetExtendedStyle(LVS_EX_FULLROWSELECT |  //选中整行
								  LVS_EX_GRIDLINES);      //网格线
	// 刷新好友列表
	// 发送正在使用的userId
	g_Server.Send(FRIENDLIST, g_UserId);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


//*****************************************************************************************
// 函数名称: GetFriend
// 函数说明: 刷新好友的信息
// 作    者: lracker
// 时    间: 2019/09/17
// 参    数: WPARAM wParam
// 参    数: LPARAM lParam
// 返 回 值: LRESULT
//*****************************************************************************************
void CDlg1::GetFriend()
{
	m_FriendList.DeleteAllItems();
	int j = 0;
	for (auto& i : g_vFriendList)
	{
		m_FriendList.InsertItem(j++, i);
	}
}



//*****************************************************************************************
// 函数名称: OnDblclkList1
// 函数说明: 创建聊天的对话框
// 作    者: lracker
// 时    间: 2019/09/17
// 参    数: NMHDR * pNMHDR
// 参    数: LRESULT * pResult
// 返 回 值: void
//*****************************************************************************************
void CDlg1::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CString FriendNum;
	FriendNum = m_FriendList.GetItemText(pNMItemActivate->iItem, 0);  //获取选中项的ID
	if (FriendNum.IsEmpty())  // 如果是空的话，直接返回了
		return;
	CChatDlg* obj = new CChatDlg();  //创建一个非模态对话框
	obj->m_FriendNum = FriendNum;
	obj->Create(IDD_CHAT);
	obj->ShowWindow(SW_SHOW);
	g_Dlg[FriendNum.GetBuffer()] = obj;  // 加入到map中
	/*将有消息标识符清空*/
	int i = pNMItemActivate->iItem;  //行
	m_FriendList.SetItemText(i, 1, L"     ");
	/*发送消息给chat dlg*/
	::SendMessage(obj->m_hWnd, UM_Message, (WPARAM)FriendNum.GetBuffer(), 0);
	*pResult = 0;
}


//*****************************************************************************************
// 函数名称: OnUmMessage
// 函数说明: 获取是否有信息，如果有窗口了已经，那直接刷新窗口的内容
// 作    者: lracker
// 时    间: 2019/09/18
// 参    数: WPARAM wParam
// 参    数: LPARAM lParam
// 返 回 值: LRESULT
//*****************************************************************************************
afx_msg LRESULT CDlg1::OnUmMessage(WPARAM wParam, LPARAM lParam)
{
	char* buffer = (char*)wParam;
	CString buffer2(buffer);
	if (g_Dlg[buffer2])  // 如果有这个窗口的话
		/*发送消息给chat dlg*/
		::SendMessage(g_Dlg[buffer2]->m_hWnd, UM_Message, (WPARAM)buffer2.GetBuffer(), 0);
	else
	{
		LVFINDINFO info;
		info.flags = LVFI_STRING;
		info.psz = buffer2;
		int index = m_FriendList.FindItem(&info);
		m_FriendList.SetItemText(index, 1, L"有消息");
	}
 	return 0;
}
