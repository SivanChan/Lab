// DlgMsgBox.cpp : 实现文件
//

#include "stdafx.h"
#include "8_monitor.h"
#include "DlgMsgBox.h"
#include "afxdialogex.h"
#include <StringUtil.h>


// DlgMsgBox 对话框

IMPLEMENT_DYNAMIC(DlgMsgBox, CDialogEx)

DlgMsgBox::DlgMsgBox(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_MSG, pParent),
	count_down_(8),
	last_time_(0)
{

}

DlgMsgBox::~DlgMsgBox()
{
}

void DlgMsgBox::SetMsg(std::string const & msg)
{
	if (GetSafeHwnd())
	{
		std::wstring wstr;
		Forge::StringUtil::StringConvert(msg,wstr);
		GetDlgItem(IDC_EDIT_MSG)->SetWindowText(wstr.c_str());

		last_time_ = count_down_;
		CString title;
		title.Format(L"消息 - %d秒后关闭", last_time_);
		SetWindowText(title);
	}
}

void DlgMsgBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgMsgBox, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// DlgMsgBox 消息处理程序


BOOL DlgMsgBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetTimer(1,1000,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void DlgMsgBox::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (GetSafeHwnd() && IsWindowVisible())
	{
		if (nIDEvent == 1)
		{
			--last_time_;
			
			if (last_time_ <= 0)
				ShowWindow(SW_HIDE);

			CString title;
			title.Format(L"消息 - %d秒后关闭", last_time_);
			SetWindowText(title);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}
