// DlgMsgBox.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "8_monitor.h"
#include "DlgMsgBox.h"
#include "afxdialogex.h"
#include <StringUtil.h>


// DlgMsgBox �Ի���

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
		title.Format(L"��Ϣ - %d���ر�", last_time_);
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


// DlgMsgBox ��Ϣ�������


BOOL DlgMsgBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetTimer(1,1000,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void DlgMsgBox::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (GetSafeHwnd() && IsWindowVisible())
	{
		if (nIDEvent == 1)
		{
			--last_time_;
			
			if (last_time_ <= 0)
				ShowWindow(SW_HIDE);

			CString title;
			title.Format(L"��Ϣ - %d���ر�", last_time_);
			SetWindowText(title);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}
