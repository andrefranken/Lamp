
// LampUpdate.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include "LampUpdateDlg.h"		
#include "UCString.h"		


// CLampUpdateApp:
// See LampUpdate.cpp for the implementation of this class
//

class CLampUpdateApp : public CWinAppEx
{
public:
	CLampUpdateApp();

// Overrides
	public:
	virtual BOOL InitInstance();

   CLampUpdateDlg *m_pDialog;

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CLampUpdateApp theApp;

UINT ThreadProc( LPVOID pParam );

bool GetLatestVersionNumber(int &major, int &minor);
bool GetCurrentVersionNumber(int &major, int &minor);
bool GetNewVersionTempFile(UCString &zipped_temp_file);
bool CloseLamp();
bool UnzipNewVersionTempFile(const UCString &zipped_temp_file);