
// MyQQDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MyQQ.h"
#include "MyQQDlg.h"
#include "afxdialogex.h"
#include <ws2tcpip.h>
#include "CRegister.h"
#include "MainDlg.h"
#include "Data.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyQQDlg 对话框



CMyQQDlg::CMyQQDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYQQ_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyQQDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_User);
	DDX_Control(pDX, IDC_EDIT2, m_PassWord);
}

BEGIN_MESSAGE_MAP(CMyQQDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMyQQDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyQQDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CMyQQDlg 消息处理程序
BOOL CMyQQDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// 设置用户名和密码的最大长度
	m_User.SetLimitText(16);
	m_PassWord.SetLimitText(16);

	// 设置位图大小
	CWnd* pWnd = GetDlgItem(IDC_STATIC1);
	CRect rcPic;
	pWnd->GetClientRect(rcPic);
	rcPic.right += 230;	 // 加宽
	rcPic.bottom += 30;  // 加高
	pWnd->MoveWindow(rcPic);

	// 连接服务器
	g_Server.connect();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyQQDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMyQQDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




//*****************************************************************************************
// 函数名称: OnBnClickedButton1
// 函数说明: 响应登录按钮的函数
// 作    者: lracker
// 时    间: 2019/09/16
// 返 回 值: void
//*****************************************************************************************
void CMyQQDlg::OnBnClickedButton1()
{
	// 获取控件信息
	UpdateData(TRUE);  
	CString Username;
	m_User.GetWindowTextW(Username);
	CString Password;
	m_PassWord.GetWindowTextW(Password);
	// 将密码和用户拼接起来，中间用\n间隔开来并且发送给服务器
	CString data = Username + '\n' + Password;
	// 转换为char类型
	USES_CONVERSION;
	char* buffer = W2A(data);
	// 发送登录的信息
	g_Server.Send(LOGIN, buffer);
	RECVDATASTRUCT* pRecv = NULL;
	pRecv = g_Server.Recv();  // 接收服务器发过来的信息
	if (pRecv->status == 1)  // 如果返回来的信息是成功的话，那么调用销毁函数
	{
		strcpy_s(g_UserId, 16, W2A(Username));
		EndDialog(0);
		// 创建新的主对话框
		MainDlg obj;
		obj.m_Server = g_Server;
		obj.DoModal();
	}
	if (pRecv->status == 0)  //如果是失败信息，则弹窗
	{
		CString RecvData(pRecv->data);
		MessageBox(RecvData, L"ERROR");
	}
}


//*****************************************************************************************
// 函数名称: OnBnClickedButton2
// 函数说明: 注册账号函数
// 作    者: lracker
// 时    间: 2019/09/16
// 返 回 值: void
//*****************************************************************************************
void CMyQQDlg::OnBnClickedButton2()
{
	CRegister registobj;
	registobj.DoModal();
}
