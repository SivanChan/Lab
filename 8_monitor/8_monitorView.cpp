// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ������� 
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// 8_monitorView.cpp : CMy8_monitorView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "8_monitor.h"
#endif

#include "8_monitorDoc.h"
#include "8_monitorView.h"
#include <AppFramework.h>
#include <StringUtil.h>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <Util.h>
#include <boost/bind.hpp>
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy8_monitorView

IMPLEMENT_DYNCREATE(CMy8_monitorView, CView)

BEGIN_MESSAGE_MAP(CMy8_monitorView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMy8_monitorView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMy8_monitorView ����/����

CMy8_monitorView::CMy8_monitorView()
	: vlc_player_(NULL),
	vlc_media_(NULL)
{
	// TODO: �ڴ˴���ӹ������

}

CMy8_monitorView::~CMy8_monitorView()
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

BOOL CMy8_monitorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMy8_monitorView ����

void CMy8_monitorView::OnDraw(CDC* /*pDC*/)
{
	CMy8_monitorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CMy8_monitorView ��ӡ


void CMy8_monitorView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMy8_monitorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMy8_monitorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMy8_monitorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CMy8_monitorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
// 	ClientToScreen(&point);
// 	OnContextMenu(this, point);
}

void CMy8_monitorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMy8_monitorView ���

#ifdef _DEBUG
void CMy8_monitorView::AssertValid() const
{
	CView::AssertValid();
}

void CMy8_monitorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy8_monitorDoc* CMy8_monitorView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy8_monitorDoc)));
	return (CMy8_monitorDoc*)m_pDocument;
}

void CMy8_monitorView::OpenVideo()
{
	std::wstring wstr = GetDocument()->GetTitle().GetString();
	std::string str;

	Forge::StringUtil::StringConvert(wstr, str);

	ip_port_ = str;
	Forge::StringUtil::trim_left(ip_port_, "rtsp://");
	Forge::StringUtil::replace2(ip_port_, ":", "_");
	Forge::StringUtil::replace2(ip_port_, "/", "_");

	if (vlc_player_ == NULL &&
		vlc_media_ == NULL)
	{
		vlc_player_ = libvlc_media_player_new(Forge::AppFramework::Instance().GetVLCInstance());

		if (vlc_player_ != NULL)
		{
			//��һ��RTSP����ַ��ͨ����ַ����һ��ý��ʵ��
			vlc_media_ = libvlc_media_new_location(Forge::AppFramework::Instance().GetVLCInstance(), str.c_str());
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
				libvlc_media_player_set_hwnd(vlc_player_, GetSafeHwnd());
				// ��ʼ������Ƶ
				libvlc_media_player_play(vlc_player_);
			}
		}
	}
}

void CMy8_monitorView::CaptureVideo()
{
	if (vlc_player_ != NULL && vlc_media_ != NULL)
	{
		std::stringstream out_stream;
		struct tm cur_tm;
		time_t cur_time;

		time(&cur_time);
		localtime_s(&cur_tm, &cur_time);
		out_stream.clear();
		out_stream.str() = "";
		out_stream 
			<< cur_tm.tm_year + 1900
			<< std::setw(2) << std::setfill('0') << cur_tm.tm_mon + 1
			<< std::setw(2) << std::setfill('0') << cur_tm.tm_mday
			<< std::setw(2) << std::setfill('0') << cur_tm.tm_hour
			<< std::setw(2) << std::setfill('0') << cur_tm.tm_min
			<< std::setw(2) << std::setfill('0') << cur_tm.tm_sec
			<< "_" << ip_port_.c_str();

		path_ = Forge::StringUtil::format("%s\\snapshot\\%s.png", Forge::GetExeDirectory().c_str(), out_stream.str().c_str());
		std::shared_ptr<std::thread> thd = std::make_shared<std::thread>([this](){ libvlc_video_take_snapshot(vlc_player_, 0, path_.c_str(), 0, 0); });
		thd->join();

		std::string msg = Forge::StringUtil::format("Snapshot:%s", path_.c_str());
		Forge::Log::Instance().LogMessage(msg);

		((CMainFrame*)AfxGetMainWnd())->ShowMsgBox(msg);
	}
}

#endif //_DEBUG


// CMy8_monitorView ��Ϣ�������
