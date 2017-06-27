// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。  
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。  
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问 
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// 8_monitorView.cpp : CMy8_monitorView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "8_monitor.h"
#endif

#include "8_monitorDoc.h"
#include "8_monitorView.h"
#include <AppFramework.h>
#include <StringUtil.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy8_monitorView

IMPLEMENT_DYNCREATE(CMy8_monitorView, CView)

BEGIN_MESSAGE_MAP(CMy8_monitorView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMy8_monitorView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMy8_monitorView 构造/析构

CMy8_monitorView::CMy8_monitorView()
	: vlc_player_(NULL),
	vlc_media_(NULL)
{
	// TODO: 在此处添加构造代码

}

CMy8_monitorView::~CMy8_monitorView()
{
	if (vlc_player_ != NULL &&
		vlc_media_ != NULL)
	{
		// 停止
		libvlc_media_player_stop(vlc_player_);
		// 释放
		libvlc_media_release(vlc_media_);
		libvlc_media_player_release(vlc_player_);
	}
}

BOOL CMy8_monitorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMy8_monitorView 绘制

void CMy8_monitorView::OnDraw(CDC* /*pDC*/)
{
	CMy8_monitorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CMy8_monitorView 打印


void CMy8_monitorView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMy8_monitorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMy8_monitorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMy8_monitorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// CMy8_monitorView 诊断

#ifdef _DEBUG
void CMy8_monitorView::AssertValid() const
{
	CView::AssertValid();
}

void CMy8_monitorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy8_monitorDoc* CMy8_monitorView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy8_monitorDoc)));
	return (CMy8_monitorDoc*)m_pDocument;
}

void CMy8_monitorView::OpenVideo()
{
	std::wstring wstr = GetDocument()->GetTitle().GetString();
	std::string str;

	Forge::StringUtil::StringConvert(wstr, str);
	if (vlc_player_ == NULL &&
		vlc_media_ == NULL)
	{
		vlc_player_ = libvlc_media_player_new(Forge::AppFramework::Instance().GetVLCInstance());

		if (vlc_player_ != NULL)
		{
			//打开一个RTSP流地址，通过地址创建一个媒体实例
			vlc_media_ = libvlc_media_new_location(Forge::AppFramework::Instance().GetVLCInstance(), str.c_str());
			if (vlc_media_ != NULL)
			{
				// 解析媒体实例
				libvlc_media_parse(vlc_media_);
				// 获取媒体文件的播放长度,  返回 ms
				libvlc_time_t duration = libvlc_media_get_duration(vlc_media_);

				// 此处是获取媒体包含多个的视频和音频轨以及其他类型的轨道信息
				libvlc_media_track_info_t *media_tracks = NULL;
				int trackCount = libvlc_media_get_tracks_info(vlc_media_, &media_tracks);
				// 这里是释放内存，但我测试的时候会有问题，还没仔细研究是为何
				// free(media_tracks);  // crash?

				// 把打开的媒体文件设置给播放器
				libvlc_media_player_set_media(vlc_player_, vlc_media_);

				// 因为是windows系统，所以需要设置一个HWND给播放器作为窗口,这里就直接使用桌面窗口,这里仅是测试
				libvlc_media_player_set_hwnd(vlc_player_, GetSafeHwnd());
				// 开始播放视频
				libvlc_media_player_play(vlc_player_);
			}
		}
	}
}

bool CMy8_monitorView::Snapshot(std::string const & file_path)
{
	if (vlc_player_ != NULL && vlc_media_ != NULL)
	{
		std::shared_ptr<std::thread> thd = std::make_shared<std::thread>([&]() { libvlc_video_take_snapshot(vlc_player_, 0, file_path.c_str(), 0, 0); });
		thd->join();
		return true;
	}
	return false;
}

#endif //_DEBUG


// CMy8_monitorView 消息处理程序
