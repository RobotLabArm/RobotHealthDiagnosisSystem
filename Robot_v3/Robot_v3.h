
// Robot_v3.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CRobot_v3App:
// See Robot_v3.cpp for the implementation of this class
//

class CRobot_v3App : public CWinApp
{
public:
	CRobot_v3App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CRobot_v3App theApp;