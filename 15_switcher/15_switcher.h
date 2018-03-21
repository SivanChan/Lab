
// 15_switcher.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMy15_switcherApp:
// See 15_switcher.cpp for the implementation of this class
//

class CMy15_switcherApp : public CWinApp
{
public:
	CMy15_switcherApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMy15_switcherApp theApp;