
// MainFrm.h : interface of the CMainFrame class
//
#pragma once

#include "LampView.h"

#define ANIM_TIMER 15
#define INBOX_TIMER 16
#define UPDATE_TIMER 17
#define REFRESH_LOL_TIMER 18
#define IMAGE_EXPIRE_TIMER 19
#define PREVIEW_TIMER 20

class CBMMenu : public CMenu
{
public:

protected:
   /*
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
   */
};


class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

   CMFCStatusBar *GetStatusBar(){return &m_wndStatusBar;}

   CMDIClientAreaWnd *GetCA(){return &m_wndClientArea;}

   void MBClick(CPoint &point, HWND hwnd);

   void CloseFindDlg();

   void UpdateInboxTimer();
   void UpdateLOLTimer();

// Attributes
public:
   void SetActiveLampView(CLampView *pView){m_pLastActiveView = pView;}
   CLampView *GetActiveLampView(void);

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar m_wndMenuBar;
   CMFCStatusBar     m_wndStatusBar;

   CLampView *m_pLastActiveView;

   bool m_bFirstUpdate;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnClose();
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
   afx_msg void OnKillFocus(CWnd* pNewWnd);
   afx_msg LRESULT OnExpandTabs(WPARAM,LPARAM);
   afx_msg LRESULT OnWakeUp(WPARAM,LPARAM);
   afx_msg void OnBookmarkMenu();
   afx_msg void OnUpdateBookmarkMenu(CCmdUI *pCmdUI);
   afx_msg void OnBookmark(UINT id);
	afx_msg void OnUpdateBookmark(CCmdUI* pCmdUI);
   afx_msg void OnTimer(UINT_PTR nIDEvent);

   afx_msg LRESULT OnMenuRBUMessage(WPARAM wparam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()

};


