// DlgVideo.cpp : implementation file
//

#include "stdafx.h"
#include "8_monitor.h"
#include "DlgVideo.h"
#include "afxdialogex.h"
#include <AppFramework.h>
#include <StringUtil.h>


// DlgVideo dialog

IMPLEMENT_DYNAMIC(DlgVideo, CDialogEx)

DlgVideo::DlgVideo(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_VIDEO, pParent)
{

}

DlgVideo::~DlgVideo()
{
}

void DlgVideo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgVideo, CDialogEx)
	ON_WM_NCHITTEST()
	ON_WM_PARENTNOTIFY()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// DlgVideo message handlers


BOOL DlgVideo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	Forge::AppConfig const & config = Forge::AppFramework::Instance().GetAppConfig();

	if (!config.ips.empty())
	{
		CRect rect;
		int gap = 1, index = 0;
		int width = 0, height = 0;

		width = gap + (gap + config.video_width) * config.ips.size();
		height = gap * 2 + config.video_height;

		CWnd::SetWindowPos(NULL, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE);
		for (auto const & ip : config.ips)
		{
			++index;
			rect.left = gap + (gap + config.video_width) * (index - 1);
			rect.right = rect.left + config.video_width;
			rect.top = gap;
			rect.bottom = gap + config.video_height;

			std::string url = Forge::StringUtil::format("rtsp://%s:%d/%s", ip.c_str(), config.camera_port, config.camera_sub_add.c_str());
			VideoPanelPtr vp = std::make_shared<VideoPanel>(rect, this);
			vp->PlayVideo(url);
			panels_.push_back(vp);
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT DlgVideo::OnNcHitTest(CPoint point)
{
	return HTCAPTION;
}

DlgVideo::VideoPanel::VideoPanel(CRect const & rect, CWnd* parent)
	: vlc_player_(NULL),
	vlc_media_(NULL)
{
	static_ = std::make_shared<CStatic>();
	static_->Create(L"", WS_CHILD | WS_VISIBLE | SS_CENTER, rect, parent);
}

DlgVideo::VideoPanel::~VideoPanel()
{
	if (vlc_player_ != NULL &&
		vlc_media_ != NULL)
	{
		// ֹͣ
		libvlc_media_player_stop(vlc_player_);
		// �ͷ�
		libvlc_media_release(vlc_media_);
		libvlc_media_player_release(vlc_player_);
	}
}

void DlgVideo::VideoPanel::PlayVideo(std::string const & url)
{
	if (vlc_player_ == NULL &&
		vlc_media_ == NULL)
	{
		vlc_player_ = libvlc_media_player_new(Forge::AppFramework::Instance().GetVLCInstance());

		if (vlc_player_ != NULL)
		{
			//��һ��RTSP����ַ��ͨ����ַ����һ��ý��ʵ��
			vlc_media_ = libvlc_media_new_location(Forge::AppFramework::Instance().GetVLCInstance(), url.c_str());
			if (vlc_media_ != NULL)
			{
				// ����ý��ʵ��
				libvlc_media_parse(vlc_media_);
				// ��ȡý���ļ��Ĳ��ų���,  ���� ms
				libvlc_time_t duration = libvlc_media_get_duration(vlc_media_);

				// �˴��ǻ�ȡý������������Ƶ����Ƶ���Լ��������͵Ĺ����Ϣ
				libvlc_media_track_info_t *media_tracks = NULL;
				int trackCount = libvlc_media_get_tracks_info(vlc_media_, &media_tracks);
				// �������ͷ��ڴ棬���Ҳ��Ե�ʱ��������⣬��û��ϸ�о���Ϊ��
				// free(media_tracks);  // crash?

				// �Ѵ򿪵�ý���ļ����ø�������
				libvlc_media_player_set_media(vlc_player_, vlc_media_);

				// ��Ϊ��windowsϵͳ��������Ҫ����һ��HWND����������Ϊ����,�����ֱ��ʹ�����洰��,������ǲ���
				libvlc_media_player_set_hwnd(vlc_player_, static_->GetSafeHwnd());
				// ��ʼ������Ƶ
				libvlc_media_player_play(vlc_player_);
			}
		}
	}
}


void DlgVideo::OnParentNotify(UINT message, LPARAM lParam)
{
	CDialogEx::OnParentNotify(message, lParam);

	// TODO: Add your message handler code here
}


BOOL DlgVideo::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialogEx::PreTranslateMessage(pMsg);
}


void DlgVideo::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnLButtonDown(nFlags, point);
}