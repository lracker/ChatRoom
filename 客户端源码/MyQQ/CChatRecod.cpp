// CChatRecod.cpp: 实现文件
//

#include "pch.h"
#include "MyQQ.h"
#include "CChatRecod.h"
#include "afxdialogex.h"
#include "Data.h"
#define UM_UPDATEMSG (WM_USER+1001)
// CChatRecod 对话框

IMPLEMENT_DYNAMIC(CChatRecod, CDialogEx)

CChatRecod::CChatRecod(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ChatRecord, pParent)
{

}

CChatRecod::~CChatRecod()
{
}

void CChatRecod::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Recod);
}


BEGIN_MESSAGE_MAP(CChatRecod, CDialogEx)
	ON_MESSAGE(UM_UPDATEMSG, &CChatRecod::OnUmUpdatemsg)
END_MESSAGE_MAP()


// CChatRecod 消息处理程序


afx_msg LRESULT CChatRecod::OnUmUpdatemsg(WPARAM wParam, LPARAM lParam)
{
	WaitForSingleObject(g_hMutex, INFINITE);
	UpdateData(TRUE);
	m_Recod.Clear();  // 清空
	CString str;
	for (auto& i : g_vChatRecord)
	{
		str += i;
		str += "\r\n";
		m_Recod.SetWindowText(str);
	}
	UpdateData(FALSE);
	ReleaseMutex(g_hMutex);
	return 0;
}


