
// 9_camera_monitorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "9_camera_monitor.h"
#include "9_camera_monitorDlg.h"
#include "afxdialogex.h"

#pragma comment(lib, "libvlc.lib")
#pragma comment(lib, "libvlccore.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMy9_camera_monitorDlg 对话框



CMy9_camera_monitorDlg::CMy9_camera_monitorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MY9_CAMERA_MONITOR_DIALOG, pParent),
	vlc_ins_(NULL),
	vlc_player_(NULL),
	vlc_media_(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy9_camera_monitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMy9_camera_monitorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMy9_camera_monitorDlg 消息处理程序

BOOL CMy9_camera_monitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InitCamera();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMy9_camera_monitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMy9_camera_monitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMy9_camera_monitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMy9_camera_monitorDlg::InitCamera()
{
	//创建一个vlc实例

	const char * vlc_args[] =
	{
		"-I",
		"dummy",
		"--ignore-config",
		"--verbose=2",
	};

	vlc_ins_ = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]),vlc_args);

	if (vlc_ins_ != NULL)
	{
		//从实例上创建一个VLC播放器供视频播放
		vlc_player_ = libvlc_media_player_new(vlc_ins_);

		if (vlc_player_ != NULL)
		{
			//打开一个RTSP流地址，通过地址创建一个媒体实例
			vlc_media_ = libvlc_media_new_location(vlc_ins_, "rtsp://192.168.1.223:6554/live2");

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



void CMy9_camera_monitorDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	// 停止
	libvlc_media_player_stop(vlc_player_);
	// 释放
	libvlc_media_release(vlc_media_);
	libvlc_media_player_release(vlc_player_);
	libvlc_release(vlc_ins_);
}
