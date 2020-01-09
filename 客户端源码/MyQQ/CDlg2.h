#pragma once


// CDlg2 对话框

class CDlg2 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg2)

public:
	CDlg2(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlg2();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GROUP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_GroupList;
	virtual BOOL OnInitDialog();
	void GetGroup();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
protected:
	afx_msg LRESULT OnUmMessage(WPARAM wParam, LPARAM lParam);
};
