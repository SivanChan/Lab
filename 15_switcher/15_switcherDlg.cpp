
// 15_switcherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "15_switcher.h"
#include "15_switcherDlg.h"
#include "afxdialogex.h"
#include "MessageController.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy15_switcherDlg dialog



CMy15_switcherDlg::CMy15_switcherDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MY15_SWITCHER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy15_switcherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMy15_switcherDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_P, &CMy15_switcherDlg::OnBnClickedBtnP)
END_MESSAGE_MAP()


// CMy15_switcherDlg message handlers

BOOL CMy15_switcherDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	message_controller_ = std::make_shared<MessageController>();
	message_controller_->Initialize("127.0.0.1", 1984);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMy15_switcherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMy15_switcherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMy15_switcherDlg::OnBnClickedBtnP()
{
	// TODO: Add your control notification handler code here

	std::string msg;
	msg = "{\"rendering\":1,\"service_id\":\"agent.terminal.show\",\"terminal_id\":\"{192.168.1.35.orz.00-ff-1f-f4-c5-6e-ec-88-8f-bd-9a-db-00-50-56-c0-00-01-00-50-56-c0-00-08}08c2f46c150533cfd7d0ea1926860b5e\"}";
	message_controller_->SendMessage(msg);
}
