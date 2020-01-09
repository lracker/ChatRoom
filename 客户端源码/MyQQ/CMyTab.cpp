// CMyTab.cpp: 实现文件
//

#include "pch.h"
#include "MyQQ.h"
#include "CMyTab.h"


// CMyTab

IMPLEMENT_DYNAMIC(CMyTab, CTabCtrl)

CMyTab::CMyTab()
{

}

CMyTab::~CMyTab()
{
}


BEGIN_MESSAGE_MAP(CMyTab, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CMyTab::OnTcnSelchange)
END_MESSAGE_MAP()



// CMyTab 消息处理程序




//*****************************************************************************************
// 函数名称: OnTcnSelchange
// 函数说明: 获取选项卡选项
// 作    者: lracker
// 时    间: 2019/09/16
// 参    数: NMHDR * pNMHDR
// 参    数: LRESULT * pResult
// 返 回 值: void
//*****************************************************************************************
void CMyTab::OnTcnSelchange(NMHDR* pNMHDR, LRESULT* pResult)
{
	int num = GetCurSel();
	switch (num)
	{
	case 0:
	{
		m_Dlg[0]->ShowWindow(SW_SHOW);
		m_Dlg[1]->ShowWindow(SW_HIDE);
		break;
	}
	case 1:
	{
		m_Dlg[0]->ShowWindow(SW_HIDE);
		m_Dlg[1]->ShowWindow(SW_SHOW);
		break;
	}
	break;
	}
	*pResult = 0;
}
