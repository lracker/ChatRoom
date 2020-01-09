#pragma once


// CGroupChat 对话框

class CGroupChat : public CDialogEx
{
	DECLARE_DYNAMIC(CGroupChat)

public:
	CGroupChat(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGroupChat();
	CString m_GroupNum;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GroupChat };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_GoupMember;
protected:
	afx_msg LRESULT OnUmFlush(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
protected:
	afx_msg LRESULT OnUmFlushend(WPARAM wParam, LPARAM lParam);
public:
	CEdit m_GetMsg;
	CEdit m_SendMsg;
	void SearchData(WCHAR* buffer);
	afx_msg void OnBnClickedButton1();
protected:
	afx_msg LRESULT OnUmMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmUpdategroupmsg(WPARAM wParam, LPARAM lParam);
	virtual void OnCancel();
};
