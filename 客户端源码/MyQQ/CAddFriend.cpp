// CAddFriend.cpp: 实现文件
//

#include "pch.h"
#include "MyQQ.h"
#include "CAddFriend.h"
#include "afxdialogex.h"
#include "Data.h"
#include <atlconv.h>


// CAddFriend 对话框

IMPLEMENT_DYNAMIC(CAddFriend, CDialogEx)

CAddFriend::CAddFriend(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AddFriend, pParent)
{

}

CAddFriend::~CAddFriend()
{
}

void CAddFriend::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Num);
}


BEGIN_MESSAGE_MAP(CAddFriend, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAddFriend::OnBnClickedButton1)

END_MESSAGE_MAP()


// CAddFriend 消息处理程序


//*****************************************************************************************
// 函数名称: OnBnClickedButton1
// 函数说明: 响应添加好友函数
// 作    者: lracker
// 时    间: 2019/09/17
// 返 回 值: void
//*****************************************************************************************
void CAddFriend::OnBnClickedButton1()
{
	UpdateData(TRUE);
	CString FriendNum;
	m_Num.GetWindowTextW(FriendNum);
	if (!FriendNum.IsEmpty())	// 如果输入不为空
	{
		CString MyNum(g_UserId);
		if (MyNum == FriendNum)
		{
			MessageBox(L"不能添加自己");
			return;
		}
		CString Data = MyNum + '\n' + FriendNum;
		USES_CONVERSION;
		char* rawData = T2A(Data.GetBuffer(0));
		g_Server.Send(ADDFRIEND, rawData);
		// 发送完就关闭窗口
		EndDialog(0);
	}
}
