#pragma once


// CAddGroup 对话框

class CAddGroup : public CDialogEx
{
	DECLARE_DYNAMIC(CAddGroup)

public:
	CAddGroup(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAddGroup();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AddGroup };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	CEdit m_GroupNum;
};
