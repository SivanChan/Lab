#pragma once


// DlgVideo dialog

#include <vlc/vlc.h>

class DlgVideo : public CDialogEx
{
	DECLARE_DYNAMIC(DlgVideo)

public:
	DlgVideo(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgVideo();

	virtual BOOL OnInitDialog();

	afx_msg LRESULT OnNcHitTest(CPoint point);

	class VideoPanel
	{
	public:
		VideoPanel(CRect const & rect, CWnd* parent);
		~VideoPanel();

		void PlayVideo(std::string const & url);
		HWND GetVideoHwnd();

	private:
		libvlc_media_player_t *  vlc_player_;
		libvlc_media_t *         vlc_media_;
		std::shared_ptr<CStatic> static_;
	};
	typedef std::shared_ptr<VideoPanel> VideoPanelPtr;

// Dialog Data
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_VIDEO };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

protected:
	std::vector<VideoPanelPtr> panels_;
public:
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};
