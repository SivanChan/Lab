
// 15_switcherDlg.h : header file
//

#pragma once

#include <memory>

class MessageController;
// CMy15_switcherDlg dialog
class CMy15_switcherDlg : public CDialogEx
{
// Construction
public:
	CMy15_switcherDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY15_SWITCHER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnP();


protected:
	std::shared_ptr<MessageController> message_controller_;
};
