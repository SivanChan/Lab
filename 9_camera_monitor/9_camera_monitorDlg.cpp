
// 9_camera_monitorDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMy9_camera_monitorDlg �Ի���



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


// CMy9_camera_monitorDlg ��Ϣ�������

BOOL CMy9_camera_monitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	InitCamera();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMy9_camera_monitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMy9_camera_monitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMy9_camera_monitorDlg::InitCamera()
{
	//����һ��vlcʵ��

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
		//��ʵ���ϴ���һ��VLC����������Ƶ����
		vlc_player_ = libvlc_media_player_new(vlc_ins_);

		if (vlc_player_ != NULL)
		{
			//��һ��RTSP����ַ��ͨ����ַ����һ��ý��ʵ��
			//vlc_media_ = libvlc_media_new_location(vlc_ins_, "rtsp://192.168.1.223:6554/live2");
			vlc_media_ = libvlc_media_new_location(vlc_ins_, "rtsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov");
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



void CMy9_camera_monitorDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	// ֹͣ
	libvlc_media_player_stop(vlc_player_);
	// �ͷ�
	libvlc_media_release(vlc_media_);
	libvlc_media_player_release(vlc_player_);
	libvlc_release(vlc_ins_);
}
