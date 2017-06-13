
// 9_camera_monitorDlg.h : ͷ�ļ�
//

#pragma once

#include <vlc/vlc.h>

// CMy9_camera_monitorDlg �Ի���
class CMy9_camera_monitorDlg : public CDialogEx
{
// ����
public:
	CMy9_camera_monitorDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY9_CAMERA_MONITOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
