#pragma once
#include <vector>
using std::vector;

// CDlg1 对话框

class CDlg1 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg1)

public:
	CDlg1(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlg1();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FRIEND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_FriendList;
	virtual BOOL OnInitDialog();
	void GetFriend();

public:

	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
protected:
	afx_msg LRESULT OnUmMessage(WPARAM wParam, LPARAM lParam);
};
