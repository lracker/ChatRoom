#pragma once


// CAddFriend 对话框

class CAddFriend : public CDialogEx
{
	DECLARE_DYNAMIC(CAddFriend)

public:
	CAddFriend(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAddFriend();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AddFriend };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Num;
	afx_msg void OnBnClickedButton1();
};
