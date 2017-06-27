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
	: CDialogEx(IDD_DLG_MSG, pParent)
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
	}
}

void DlgMsgBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgMsgBox, CDialogEx)
END_MESSAGE_MAP()


// DlgMsgBox 消息处理程序
