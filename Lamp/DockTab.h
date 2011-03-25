#pragma once

#include "MainFrm.h"
// DockTab dialog

class DockTab : public CDialog
{
	DECLARE_DYNAMIC(DockTab)

public:
	DockTab(CWnd* pParent = NULL);   // standard constructor
	virtual ~DockTab();

   CMainFrame *m_pFrameWnd;

// Dialog Data
	enum { IDD = IDD_DOCKTAB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   afx_msg void OnMouseMove(UINT nFlags, CPoint point);
   afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};
