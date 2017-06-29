#pragma once


// DlgMsgBox 对话框

class DlgMsgBox : public CDialogEx
{
	DECLARE_DYNAMIC(DlgMsgBox)

public:
	DlgMsgBox(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgMsgBox();

	void SetMsg(std::string const & msg);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
// 对话框数据
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MSG };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	const Forge::uint32_t count_down_;
	int      last_time_;
};
