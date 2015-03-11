// XRays.h : main header file for the XRays application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "XScene.h"


// CXRaysApp:
// See XRays.cpp for the implementation of this class
//

class CXRaysApp : public CWinApp
{
public:
	CXRaysApp();


// Overrides
public:
	XScene Scene;
	virtual BOOL InitInstance();
	

// Implementation

public:
	afx_msg void OnAppAbout();
	
	DECLARE_MESSAGE_MAP()
};

extern CXRaysApp XRaysApp;