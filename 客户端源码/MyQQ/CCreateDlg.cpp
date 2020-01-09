// CCreateDlg.cpp: 实现文件
//

#include "pch.h"
#include "MyQQ.h"
#include "CCreateDlg.h"
#include "afxdialogex.h"
#include "CMySocket.h"
#include "Data.h"


// CCreateDlg 对话框

IMPLEMENT_DYNAMIC(CCreateDlg, CDialogEx)

CCreateDlg::CCreateDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CreateGroup, pParent)
{

}

CCreateDlg::~CCreateDlg()
{
}

void CCreateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_GroupNum);
}


BEGIN_MESSAGE_MAP(CCreateDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CCreateDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CCreateDlg 消息处理程序


//*****************************************************************************************
// 函数名称: OnBnClickedButton1
// 函数说明: 创建群
// 作    者: lracker
// 时    间: 2019/09/18
// 返 回 值: void
//*****************************************************************************************
void CCreateDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	CString GroupNumber;
	m_GroupNum.GetWindowTextW(GroupNumber);
	USES_CONVERSION;
	char* GroupNum = W2A(GroupNumber);
	g_Server.Send(CREATEGROUP, GroupNum);
	EndDialog(0);
}


BOOL CCreateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_GroupNum.LimitText(16);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
