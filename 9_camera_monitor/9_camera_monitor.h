
// 9_camera_monitor.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMy9_camera_monitorApp: 
// �йش����ʵ�֣������ 9_camera_monitor.cpp
//

class CMy9_camera_monitorApp : public CWinApp
{
public:
	CMy9_camera_monitorApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMy9_camera_monitorApp theApp;