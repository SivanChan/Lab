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

// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "8_monitor.h"

#include "MainFrm.h"
#include <ListboxOutputter.h>
#include <AppFramework.h>
#include <StringUtil.h>
#include "8_monitorDoc.h"
#include "8_monitorView.h"

#include "DlgMsgBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(COutlookBar, CMFCOutlookBar)
	ON_COMMAND(ID_SERVERTREE_MAIN_VIDEO, OnMainVideo)
	ON_COMMAND(ID_SERVERTREE_SUB_VIDEO, OnSubVideo)
	ON_COMMAND(ID_SERVERTREE_SNAPSHOT, OnSnapShot)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
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
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // ����������ʽ...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // ����Ϊ FALSE �Ὣ�رհ�ť������ѡ�������Ҳ�
	mdiTabParams.m_bTabIcons = FALSE;    // ����Ϊ TRUE ���� MDI ѡ��������ĵ�ͼ��
	mdiTabParams.m_bAutoColor = TRUE;    // ����Ϊ FALSE ������ MDI ѡ����Զ���ɫ
	mdiTabParams.m_bDocumentMenu = TRUE; // ��ѡ�������ұ�Ե�����ĵ��˵�
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// �������񽫴�������࣬��˽���ʱ��������ͣ��: 
	EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT);

	// ���������á�Outlook��������: 
	if (!CreateOutlookBar(m_wndNavigationBar, ID_VIEW_NAVIGATION, m_wndTree, m_wndCalendar, 250))
	{
		TRACE0("δ�ܴ�����������\n");
		m_wndNavigationBar.SetServerTree(NULL);
		return -1;      // δ�ܴ���
	}
	m_wndNavigationBar.SetServerTree(&m_wndTree);
	Forge::AppFramework::Instance().SetServerTree(std::make_shared<Forge::ServerTree>(m_wndTree));


	// �Ѵ��� Outlook ����Ӧ���������ͣ����
	EnableDocking(CBRS_ALIGN_LEFT);
	EnableAutoHidePanes(CBRS_ALIGN_RIGHT);

	// ����ͣ������
	if (!CreateDockingWindows())
	{
		TRACE0("δ�ܴ���ͣ������\n");
		return -1;
	}

	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);

	// ���ڳ־�ֵ�����Ӿ�����������ʽ
	OnApplicationLook(theApp.m_nAppLook);

	// ������ǿ�Ĵ��ڹ���Ի���
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// ���ĵ�����Ӧ�ó��������ڴ��ڱ������ϵ�˳����н�������
	// ���Ľ��������Ŀ����ԣ���Ϊ��ʾ���ĵ�����������ͼ��
	ModifyStyle(0, FWS_PREFIXTITLE);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}

void CMainFrame::ShowMsgBox(std::string const & msg)
{
	if (!msg_box_)
		msg_box_ = std::make_shared<DlgMsgBox>();
	if (msg_box_->GetSafeHwnd() == NULL)
		msg_box_->Create(DlgMsgBox::IDD, this);
	msg_box_->SetMsg(msg);
	msg_box_->ShowWindow(SW_SHOW);

	CRect rect,rc;
	msg_box_->GetWindowRect(&rect);
	this->GetWindowRect(&rc);

	int x = rc.right - rect.Width();
	int y = rc.bottom - rect.Height();
	rect.MoveToXY(x, y);
	msg_box_->MoveWindow(&rect);
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;
	// �����������
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ����������\n");
		return FALSE; // δ�ܴ���
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
		return FALSE; // δ�ܴ���
	}

	CMFCOutlookBarTabCtrl* pOutlookBar = (CMFCOutlookBarTabCtrl*)bar.GetUnderlyingWindow();

	if (pOutlookBar == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	pOutlookBar->EnableInPlaceEdit(TRUE);

	static UINT uiPageID = 1;

	// �ɸ��������Զ����أ��ɵ�����С�������ܹر�
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

// CMainFrame ���

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


// CMainFrame ��Ϣ�������

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
		}
	}

	CMFCOutlookBar::OnContextMenu(pWnd,point);
}

void COutlookBar::OnMainVideo()
{
	HTREEITEM ht = tree_->GetSelectedItem();
	if (ht)
	{
		std::wstring wstr = tree_->GetItemText(ht).GetString();
		std::string str, rtsp_str;

		Forge::StringUtil::StringConvert(wstr,str);
		rtsp_str = Forge::StringUtil::format("rtsp://%s:6554/live1",str.c_str());
		Forge::StringUtil::StringConvert(rtsp_str,wstr);
		
		OpenVideo(wstr);
	}
}

void COutlookBar::OnSubVideo()
{
	HTREEITEM ht = tree_->GetSelectedItem();
	if (ht)
	{
		std::wstring wstr = tree_->GetItemText(ht).GetString();
		std::string str, rtsp_str;

		Forge::StringUtil::StringConvert(wstr, str);
		rtsp_str = Forge::StringUtil::format("rtsp://%s:6554/live2", str.c_str());
		Forge::StringUtil::StringConvert(rtsp_str, wstr);
		
		OpenVideo(wstr);
	}
}

void COutlookBar::OnSnapShot()
{
	HTREEITEM ht = tree_->GetSelectedItem();
	if (ht)
	{
		std::wstring wstr = tree_->GetItemText(ht).GetString();
		std::string str, rtsp_str;

		Forge::StringUtil::StringConvert(wstr, str);
		rtsp_str = Forge::StringUtil::format("rtsp://%s:6554/live1", str.c_str());
		Forge::StringUtil::StringConvert(rtsp_str, wstr);

		SnapShot(wstr);
	}
}

void COutlookBar::SetServerTree(CTreeCtrl* tree)
{
	tree_ = tree;
}

void COutlookBar::OpenVideo(std::wstring const & wstr)
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

		((CMy8_monitorView*)((CMainFrame*)AfxGetMainWnd())->MDIGetActive()->GetActiveView())->OpenVideo();
	}
}

void COutlookBar::SnapShot(std::wstring const & wstr)
{
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
					POSITION posview = pDoc->GetFirstViewPosition();
					if (posview != NULL)
					{
						CView* pV = pDoc->GetNextView(posview);
						pV->GetParentFrame()->ActivateFrame();

						CMy8_monitorView* pView = (CMy8_monitorView*)pV;
						pView->CaptureVideo();
					}
					continue;
				}
			}
		}
	}
}
