// CAddGroup.cpp: 实现文件
//

#include "pch.h"
#include "MyQQ.h"
#include "CAddGroup.h"
#include "afxdialogex.h"
#include "Data.h"


// CAddGroup 对话框

IMPLEMENT_DYNAMIC(CAddGroup, CDialogEx)

CAddGroup::CAddGroup(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AddGroup, pParent)
{

}

CAddGroup::~CAddGroup()
{
}

void CAddGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_GroupNum);
}


BEGIN_MESSAGE_MAP(CAddGroup, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAddGroup::OnBnClickedButton1)
END_MESSAGE_MAP()


// CAddGroup 消息处理程序


//*****************************************************************************************
// 函数名称: OnBnClickedButton1
// 函数说明: 加入
// 作    者: lracker
// 时    间: 2019/09/18
// 返 回 值: void
//*****************************************************************************************
void CAddGroup::OnBnClickedButton1()
{
	UpdateData(TRUE);
	CString GroupNumber;
	m_GroupNum.GetWindowTextW(GroupNumber);
	USES_CONVERSION;
	char* GroupNum = W2A(GroupNumber);
	g_Server.Send(ADDGROUP, GroupNum);
	EndDialog(0);
}


BOOL CAddGroup::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_GroupNum.LimitText(16);
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
