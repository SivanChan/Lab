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

// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "8_monitor.h"

#include "MainFrm.h"
#include <ListboxOutputter.h>
#include <AppFramework.h>
#include <StringUtil.h>
#include "8_monitorDoc.h"
#include "8_monitorView.h"
#include <StringUtil.h>
#include <iomanip>
#include <Windows.h>
#include <sstream>
#include <iostream>
#include <Util.h>

#include "DlgMsgBox.h"
#include "DlgVideo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(COutlookBar, CMFCOutlookBar)
	ON_COMMAND(ID_SERVERTREE_SUB_VIDEO, OnSubVideo)
	ON_COMMAND(ID_SERVERTREE_SNAPSHOT, OnSnapshot)
	ON_COMMAND(ID_CAMERA_MONITOR_MODE, OnMonitorMode)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


// CMainFrame
#define WM_CLICK_TRAY (WM_USER+001)
static const UINT THREAD_MSG = ::RegisterWindowMessage(_T("THREAD_MSG"));

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE(THREAD_MSG, &CMainFrame::OnThreadMessage)
	ON_MESSAGE(WM_CLICK_TRAY, &CMainFrame::OnClickTray)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

// CMainFrame 构造/析构

CMainFrame::CMainFrame() : monitor_mode_(false)
{
	// TODO: 在此添加成员初始化代码
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
}

LRESULT CMainFrame::OnThreadMessage(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 1)
	{
		Forge::AppFramework::AlertInfo* info = (Forge::AppFramework::AlertInfo*)lParam;
		std::string rtsp_str = Forge::StringUtil::format("rtsp://%s:%d/%s", info->camera_ip.c_str(), Forge::AppFramework::Instance().GetAppConfig().camera_port, Forge::AppFramework::Instance().GetAppConfig().camera_sub_add.c_str());
		std::wstring wstr;

		Forge::StringUtil::StringConvert(rtsp_str, wstr);
		std::string path = m_wndNavigationBar.Snapshot(wstr);

		std::stringstream out_stream;
		out_stream << "类型 - 周界报警\r\n"
			<< "相机 - " << info->camera_ip.c_str() << "\r\n\r\n"
			<< "内容 - " << info->alert_info.c_str() << "\r\n"
			<< "截图 - " << path.c_str();

		ShowMsgBox(out_stream.str());
	} 
	else
	{

	}

	return 1;
}

void CMainFrame::ShowMsgBox(std::string const & msg)
{
	if (!msg_box_)
		msg_box_ = std::make_shared<DlgMsgBox>();
	if (msg_box_->GetSafeHwnd() == NULL)
		msg_box_->Create(DlgMsgBox::IDD, this);
	msg_box_->SetMsg(msg);
	msg_box_->ShowWindow(SW_SHOW);

	CRect rect, rc;
	msg_box_->GetWindowRect(&rect);
	GetDesktopWindow()->GetWindowRect(&rc);

	int x = rc.right - rect.Width();
	int y = rc.bottom - rect.Height();
	y -= 30;
	rect.MoveToXY(x, y);
	msg_box_->MoveWindow(&rect);
}

void CMainFrame::DoMonitorMode(bool monitor_mode)
{
	if (monitor_mode_ != monitor_mode)
	{
		monitor_mode_ = monitor_mode;

		if (monitor_mode_)
		{
			if (!video_dlg_)
				video_dlg_ = std::make_shared<DlgVideo>();
			if (video_dlg_->GetSafeHwnd() == NULL)
				video_dlg_->Create(DlgVideo::IDD, this);
			video_dlg_->ShowWindow(SW_SHOW);

			CRect rect, rc;
			video_dlg_->GetWindowRect(&rect);
			GetDesktopWindow()->GetWindowRect(&rc);

			int x = rc.left;
			int y = rc.bottom - rect.Height();
			y -= 30;
			rect.MoveToXY(x, y);
			video_dlg_->MoveWindow(&rect);
		}
		else
		{
			video_dlg_.reset();
		}
	}
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // 其他可用样式...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // 设置为 FALSE 会将关闭按钮放置在选项卡区域的右侧
	mdiTabParams.m_bTabIcons = FALSE;    // 设置为 TRUE 将在 MDI 选项卡上启用文档图标
	mdiTabParams.m_bAutoColor = TRUE;    // 设置为 FALSE 将禁用 MDI 选项卡的自动着色
	mdiTabParams.m_bDocumentMenu = TRUE; // 在选项卡区域的右边缘启用文档菜单
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 导航窗格将创建在左侧，因此将暂时禁用左侧的停靠: 
	EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT);

	// 创建并设置“Outlook”导航栏: 
	if (!CreateOutlookBar(m_wndNavigationBar, ID_VIEW_NAVIGATION, m_wndTree, m_wndCalendar, 250))
	{
		TRACE0("未能创建导航窗格\n");
		m_wndNavigationBar.SetServerTree(NULL);
		return -1;      // 未能创建
	}
	m_wndNavigationBar.SetServerTree(&m_wndTree);
	Forge::AppFramework::Instance().SetServerTree(std::make_shared<Forge::ServerTree>(m_wndTree));


	// 已创建 Outlook 栏，应允许在左侧停靠。
	EnableDocking(CBRS_ALIGN_LEFT);
	EnableAutoHidePanes(CBRS_ALIGN_RIGHT);

	// 创建停靠窗口
	if (!CreateDockingWindows())
	{
		TRACE0("未能创建停靠窗口\n");
		return -1;
	}

	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);

	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);

	// 启用增强的窗口管理对话框
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// 将文档名和应用程序名称在窗口标题栏上的顺序进行交换。这
	// 将改进任务栏的可用性，因为显示的文档名带有缩略图。
	ModifyStyle(0, FWS_PREFIXTITLE);

	Forge::AppFramework::Instance().SetMessageID(THREAD_MSG);
	Forge::AppFramework::Instance().SetWnd(GetSafeHwnd());

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;
	// 创建输出窗口
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建输出窗口\n");
		return FALSE; // 未能创建
	}
	Forge::Log::Instance().Add( std::make_shared<Forge::ListboxOutputter>(m_wndOutput.GetOutputList()) );

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	UpdateMDITabbedBarsIcons();
}

BOOL CMainFrame::CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, CTreeCtrl& tree, CCalendarBar& calendar, int nInitialWidth)
{
	bar.SetMode2003();

	BOOL bNameValid;
	CString strTemp;
	bNameValid = strTemp.LoadString(IDS_SHORTCUTS);
	ASSERT(bNameValid);
	if (!bar.Create(strTemp, this, CRect(0, 0, nInitialWidth, 32000), uiID, WS_CHILD | WS_VISIBLE | CBRS_LEFT))
	{
		return FALSE; // 未能创建
	}

	CMFCOutlookBarTabCtrl* pOutlookBar = (CMFCOutlookBarTabCtrl*)bar.GetUnderlyingWindow();

	if (pOutlookBar == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	pOutlookBar->EnableInPlaceEdit(TRUE);

	static UINT uiPageID = 1;

	// 可浮动，可自动隐藏，可调整大小，但不能关闭
	DWORD dwStyle = AFX_CBRS_FLOAT | AFX_CBRS_AUTOHIDE | AFX_CBRS_RESIZE;

	CRect rectDummy(0, 0, 0, 0);
	const DWORD dwTreeStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	tree.Create(dwTreeStyle, rectDummy, &bar, 1200);
	bNameValid = strTemp.LoadString(IDS_FOLDERS);
	ASSERT(bNameValid);
	pOutlookBar->AddControl(&tree, strTemp, 2, TRUE, dwStyle);

	calendar.Create(rectDummy, &bar, 1201);
	bNameValid = strTemp.LoadString(IDS_CALENDAR);
	ASSERT(bNameValid);
	pOutlookBar->AddControl(&calendar, strTemp, 3, TRUE, dwStyle);

	bar.SetPaneStyle(bar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	pOutlookBar->SetImageList(theApp.m_bHiColorIcons ? IDB_PAGES_HC : IDB_PAGES, 24);
	pOutlookBar->SetToolbarImageList(theApp.m_bHiColorIcons ? IDB_PAGES_SMALL_HC : IDB_PAGES_SMALL, 16);
	pOutlookBar->RecalcLayout();

	BOOL bAnimation = theApp.GetInt(_T("OutlookAnimation"), TRUE);
	CMFCOutlookBarTabCtrl::EnableAnimation(bAnimation);

	bar.SetButtonsFont(&afxGlobalData.fontBold);

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	m_wndOutput.UpdateFonts();
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}

LRESULT CMainFrame::OnClickTray(WPARAM wParam, LPARAM IParam)
{
	if ((IParam == WM_LBUTTONDOWN) || (IParam == WM_RBUTTONDOWN))
	{
		ModifyStyleEx(0, WS_EX_TOPMOST);
		ShowWindow(SW_SHOW);
		DoMonitorMode(false);
	}
	return 0;
}

void COutlookBar::OnContextMenu(CWnd* pWnd, CPoint point)
{
	if (pWnd == tree_ && pWnd != NULL)
	{

		CPoint pt;
		GetCursorPos(&pt);
		tree_->ScreenToClient(&pt);

		UINT uFlags;
		HTREEITEM ht = tree_->HitTest(pt,&uFlags);
		if (ht != NULL && (TVHT_ONITEM & uFlags))
		{
			tree_->SelectItem(ht);

			if (tree_->GetItemData(ht) != NULL)
			{
				CMenu menu;
				menu.LoadMenu(IDR_MENU_TREE);

				CMenu* pSumMenu = menu.GetSubMenu(0);

				if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
				{
					CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

					if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
						return;

					((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
					UpdateDialogControls(this, FALSE);
				}
			}
			else
			{
				CString text = tree_->GetItemText(ht);
				if (text.Compare(L"Camera") == 0)
				{
					CMenu menu;
					menu.LoadMenu(IDR_MENU_CAMERA);

					CMenu* pSumMenu = menu.GetSubMenu(0);

					if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
					{
						CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

						if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
							return;

						((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
						UpdateDialogControls(this, FALSE);
					}
				}
			}
		}
	}

	CMFCOutlookBar::OnContextMenu(pWnd,point);
}

void COutlookBar::OnSubVideo()
{
	HTREEITEM ht = tree_->GetSelectedItem();
	if (ht)
	{
		std::wstring wstr = tree_->GetItemText(ht).GetString();
		std::string str, rtsp_str;

		Forge::StringUtil::StringConvert(wstr, str);
		rtsp_str = Forge::StringUtil::format("rtsp://%s:%d/%s", str.c_str(), Forge::AppFramework::Instance().GetAppConfig().camera_port, Forge::AppFramework::Instance().GetAppConfig().camera_sub_add.c_str());
		Forge::StringUtil::StringConvert(rtsp_str, wstr);
		
		OpenVideo(wstr);
	}
}

void COutlookBar::OnSnapshot()
{
	HTREEITEM ht = tree_->GetSelectedItem();
	if (ht)
	{
		std::wstring wstr = tree_->GetItemText(ht).GetString();
		std::string str, rtsp_str;

		Forge::StringUtil::StringConvert(wstr, str);
		rtsp_str = Forge::StringUtil::format("rtsp://%s:%d/%s", str.c_str(), Forge::AppFramework::Instance().GetAppConfig().camera_port, Forge::AppFramework::Instance().GetAppConfig().camera_sub_add.c_str());
		Forge::StringUtil::StringConvert(rtsp_str, wstr);
		Snapshot(wstr);
	}
}

void COutlookBar::OnMonitorMode()
{
	((CMainFrame*)AfxGetMainWnd())->DoMonitorMode(true);
	::SendMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

void COutlookBar::SetServerTree(CTreeCtrl* tree)
{
	tree_ = tree;
}

CView* COutlookBar::OpenVideo(std::wstring const & wstr)
{
	bool find = false;

	POSITION pos = AfxGetApp()->GetFirstDocTemplatePosition();
	if (pos != NULL)
	{
		CDocTemplate *p = AfxGetApp()->GetNextDocTemplate(pos);
		POSITION posdoc = p->GetFirstDocPosition();
		while (posdoc != NULL)
		{
			CMy8_monitorDoc* pDoc = (CMy8_monitorDoc*)p->GetNextDoc(posdoc);
			if (pDoc != NULL)
			{
				if (wstr.compare(pDoc->GetTitle().GetString()) == 0)
				{
					find = true;

					POSITION posview = pDoc->GetFirstViewPosition();
					if (posview != NULL)
					{
						CView* pV = pDoc->GetNextView(posview);
						pV->GetParentFrame()->ActivateFrame();
						return pV;
					}
					continue;
				}
			}
		}
	}

	if (!find)
	{
		AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_FILE_NEW, 0), 0);
		((CMainFrame*)AfxGetMainWnd())->MDIGetActive()->GetActiveView()->GetDocument()->SetTitle(wstr.c_str());
		((CMainFrame*)AfxGetMainWnd())->MDIGetActive()->SetWindowText(wstr.c_str());
		CMy8_monitorView* pView = (CMy8_monitorView*)((CMainFrame*)AfxGetMainWnd())->MDIGetActive()->GetActiveView();
		pView->OpenVideo();
		return pView;
	}
	return NULL;
}

std::string COutlookBar::Snapshot(std::wstring const & wstr)
{
	std::string rtsp_str;
	Forge::StringUtil::StringConvert(wstr, rtsp_str);

	Forge::StringUtil::trim_left(rtsp_str, "rtsp://");
	Forge::StringUtil::replace2(rtsp_str, ":", "_");
	Forge::StringUtil::replace2(rtsp_str, "/", "_");
	Forge::StringUtil::replace2(rtsp_str, ".", "_");

	std::stringstream out_stream;
	struct tm cur_tm;
	time_t cur_time;

	time(&cur_time);
	localtime_s(&cur_tm, &cur_time);
	out_stream.clear();
	out_stream.str() = "";
	out_stream << Forge::GetExeDirectory().c_str() << "\\snapshot\\"
		<< cur_tm.tm_year + 1900
		<< std::setw(2) << std::setfill('0') << cur_tm.tm_mon + 1
		<< std::setw(2) << std::setfill('0') << cur_tm.tm_mday
		<< std::setw(2) << std::setfill('0') << cur_tm.tm_hour
		<< std::setw(2) << std::setfill('0') << cur_tm.tm_min
		<< std::setw(2) << std::setfill('0') << cur_tm.tm_sec
		<< "_" << rtsp_str.c_str() << ".png";

	CMy8_monitorView* pView = (CMy8_monitorView*)OpenVideo(wstr);
	if (pView)
		pView->Snapshot(out_stream.str());
	return out_stream.str();
}


void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default
	if (nID == SC_MINIMIZE)
	{
		notify_icon_.cbSize = sizeof(NOTIFYICONDATA);
		notify_icon_.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
		notify_icon_.hWnd = m_hWnd;
		lstrcpy(notify_icon_.szTip, L"monitor");
		notify_icon_.uCallbackMessage = WM_CLICK_TRAY;
		notify_icon_.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		Shell_NotifyIcon(NIM_ADD, &notify_icon_);
		ShowWindow(SW_HIDE);
	}
	else
		CMDIFrameWndEx::OnSysCommand(nID, lParam);
}
