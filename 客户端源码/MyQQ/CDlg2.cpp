// CDlg2.cpp: 实现文件
//

#include "pch.h"
#include "MyQQ.h"
#include "CDlg2.h"
#include "afxdialogex.h"
#include "Data.h"
#include "CGroupChat.h"
#define UM_Message WM_USER+1000

// CDlg2 对话框

IMPLEMENT_DYNAMIC(CDlg2, CDialogEx)

CDlg2::CDlg2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GROUP, pParent)
{

}

CDlg2::~CDlg2()
{
}

void CDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_GroupList);
}


BEGIN_MESSAGE_MAP(CDlg2, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDlg2::OnDblclkList1)
	ON_MESSAGE(UM_Message, &CDlg2::OnUmMessage)
END_MESSAGE_MAP()


// CDlg2 消息处理程序


BOOL CDlg2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_GroupList.InsertColumn(0, L"群名", 0, 155);
	m_GroupList.InsertColumn(1, L"是否有新消息", 0, 155);
	m_GroupList.SetExtendedStyle(LVS_EX_FULLROWSELECT |  //选中整行
		LVS_EX_GRIDLINES);      //网格线
	g_Server.Send(GROUPLIST, g_UserId);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CDlg2::GetGroup()
{
	m_GroupList.DeleteAllItems();
	int j = 0;
	for (auto& i : g_vGroupList)
	{
		m_GroupList.InsertItem(j++, i);
	}
}


//*****************************************************************************************
// 函数名称: OnDblclkList1
// 函数说明: 相应双击，弹出对话框
// 作    者: lracker
// 时    间: 2019/09/18
// 参    数: NMHDR * pNMHDR
// 参    数: LRESULT * pResult
// 返 回 值: void
//*****************************************************************************************
void CDlg2::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CString GroupNum;
	GroupNum = m_GroupList.GetItemText(pNMItemActivate->iItem, 0);  //获取选中项的ID
	if (GroupNum.IsEmpty())  // 如果是空的话，直接返回了
		return;
	CGroupChat* obj = new CGroupChat();  //创建一个非模态对话框
	obj->m_GroupNum = GroupNum;
	obj->Create(IDD_GroupChat);
	obj->ShowWindow(SW_SHOW);
	g_Dlg[GroupNum.GetBuffer()] = obj;  // 加入到map中
	USES_CONVERSION;
	char* buffer = W2A(GroupNum);
	g_Server.Send(GROUPMEMBERFLUSH, buffer);
	/*将有消息标识符清空*/
	int i = pNMItemActivate->iItem;  //行
	m_GroupList.SetItemText(i, 1, L"     ");
	/*发送消息给groupchat dlg*/
	::SendMessage(obj->m_hWnd, UM_Message, (WPARAM)GroupNum.GetBuffer(), 0);
	*pResult = 0;
}


afx_msg LRESULT CDlg2::OnUmMessage(WPARAM wParam, LPARAM lParam)
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
		int index = m_GroupList.FindItem(&info);
		m_GroupList.SetItemText(index, 1, L"有消息");
	}
	return 0;
}
