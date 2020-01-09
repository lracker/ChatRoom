#pragma once
#include <vector>
using std::vector;

// CChatDlg 对话框

class CChatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChatDlg)

public:
	CChatDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CChatDlg();
	CString m_FriendNum;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHAT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_SendMessage;
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	CEdit m_GetMessage;
	void SearchData(WCHAR* buffer);
protected:
	afx_msg LRESULT OnUmMessage(WPARAM wParam, LPARAM lParam);
public:
	virtual void OnCancel();
protected:
	afx_msg LRESULT OnUmUpdatemsg(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButton2();
};
