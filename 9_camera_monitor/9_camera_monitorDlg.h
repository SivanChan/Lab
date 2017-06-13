
// 9_camera_monitorDlg.h : 头文件
//

#pragma once

#include <vlc/vlc.h>

// CMy9_camera_monitorDlg 对话框
class CMy9_camera_monitorDlg : public CDialogEx
{
// 构造
public:
	CMy9_camera_monitorDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY9_CAMERA_MONITOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	void InitCamera();

protected:
	libvlc_instance_t *     vlc_ins_;
	libvlc_media_player_t * vlc_player_;
	libvlc_media_t *        vlc_media_;
public:
	afx_msg void OnDestroy();
};
