#pragma once


// CChatRecod 对话框

class CChatRecod : public CDialogEx
{
	DECLARE_DYNAMIC(CChatRecod)

public:
	CChatRecod(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CChatRecod();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ChatRecord };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Recod;
protected:
	afx_msg LRESULT OnUmUpdatemsg(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnUmFlush(WPARAM wParam, LPARAM lParam);
};
