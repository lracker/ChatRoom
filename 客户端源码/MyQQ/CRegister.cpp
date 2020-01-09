// CRegister.cpp: 实现文件
//

#include "pch.h"
#include "MyQQ.h"
#include "CRegister.h"
#include "afxdialogex.h"
#include "Data.h"


// CRegister 对话框

IMPLEMENT_DYNAMIC(CRegister, CDialogEx)

CRegister::CRegister(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Register, pParent)
{

}

CRegister::~CRegister()
{
}

void CRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_User);
	DDX_Control(pDX, IDC_EDIT2, m_Pswd1);
	DDX_Control(pDX, IDC_EDIT3, m_Pswd2);
}


BEGIN_MESSAGE_MAP(CRegister, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CRegister::OnBnClickedButton1)
END_MESSAGE_MAP()


// CRegister 消息处理程序


//*****************************************************************************************
// 函数名称: OnBnClickedButton1
// 函数说明: 注册函数，发送账号和密码
// 作    者: lracker
// 时    间: 2019/09/16
// 返 回 值: void
//*****************************************************************************************
void CRegister::OnBnClickedButton1()
{
	// 获取控件的内容
	UpdateData(TRUE);
	CString pswd1, pswd2, user;
	m_Pswd1.GetWindowTextW(pswd1);
	m_Pswd2.GetWindowTextW(pswd2);
	m_User.GetWindowTextW(user);
	if (pswd1 != pswd2)
	{
		MessageBox(L"两次密码不相同", L"Error");
		return;  //判断，如果两次密码不相等,则返回
	}
	// 将密码和用户拼接起来，中间用\n间隔开来并且发送给服务器
	CString data = user + '\n' + pswd1;
	// 转换为char类型
	USES_CONVERSION;
	char* buffer = W2A(data);
	// 否则发送给服务器进行注册
	g_Server.Send(REGISTE, buffer);
	RECVDATASTRUCT* pRecv = NULL;
	pRecv = g_Server.Recv();  // 接收服务器发过来的信息
	CString RecvData = A2W(pRecv->data);
	MessageBox(RecvData);  // 弹出返回来的信息
	if (pRecv->status == 1)  // 如果返回来的信息是成功的话，那么发送销毁函数
	{
		EndDialog(0);
	}
}
