#pragma once


// CCreateDlg 对话框

class CCreateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCreateDlg)

public:
	CCreateDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCreateDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CreateGroup };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_GroupNum;
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
};
