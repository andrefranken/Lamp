
// LampView.h : interface of the CLampView class
//


#pragma once

#include "DCSurface.h"
#include "MainFrm.h"
#include "chattypost.h"

#define ANIM_TIMER 15

class CLampDoc;

class CHotSpot;

class CLampView : public CView
{
protected: // create from serialization only
	CLampView();
	DECLARE_DYNCREATE(CLampView)

// Attributes
public:
	CLampDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

protected:
	virtual void OnInitialUpdate(); // called first time after construct
   virtual void OnDraw(CDC* pDC);
   
// Implementation
public:
	virtual ~CLampView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

   void InvalidateEverything();
   void InvalidateEverythingPan();
   void InvalidateHotspots();

   std::vector<CHotSpot> &GetHotspots(){return m_hotspots;}

   unsigned int GetCurrentId(){return m_current_id;}
   void SetCurrentId(unsigned int id){m_current_id = id;}

   void InvalidateSkin();

   int GetPos(){return m_pos;}

   void MakeCurrentPosVisible();

   void CloseReplyDialog();

   void SetDLGUp(bool value){m_dlgup = value;}
   bool GetDLGUp(){return m_dlgup;}

protected:
   void DrawHotSpots(HDC hDC);
   void DrawCurrentHotSpots(HDC hDC);
   void DrawTextSelection(HDC hDC, const RECT &DeviceRectangle);
   void DrawScrollbar(HDC hDC, const RECT &ScrollRectangle, std::vector<CHotSpot> &hotspots);
   void DrawThumb(HDC hDC, const RECT &thumbrect, scrollbitype type);
   void UpdateHotspotPosition();
   
   void DrawEverythingToBuffer(CDCSurface *pSurface = NULL, 
                               RECT *pDeviceRectangle = NULL, 
                               RECT *pScrollRectangle = NULL);
   void FindNext();
   void MakePosLegal();
   void GetDocHeight();
   CDCSurface *m_backbuffer;
   CDCSurface m_backbuffer1;
   CDCSurface m_backbuffer2;
   CDCSurface m_whitebuffer;
   int m_pos;
   int m_gotopos;
   int m_lastdrawnpos;
   CPoint m_mousepoint;
   std::vector<CHotSpot> m_hotspots;
   RECT *m_hotspot;
   RECT *m_lasthotspot;
   hotspottype m_hotspottype;

   bool m_bDrawHotsOnly;
   bool m_bPanOnly;
   bool m_bForceDrawAll;
   bool m_bLButtonDownOnScrollArrow;
   unsigned int m_current_id;

   bool m_bDraggingTextSelection;
   unsigned int m_textselectionpost;
   int m_selectionstart;
   int m_selectionend;

   int m_selectionstart_actual;
   int m_selectionend_actual;
   int m_lastcharpos;

   DWORD m_doubleclicktime;

   bool m_bdrawthumb;
   RECT m_thumbrect;
   RECT m_uprect;
   RECT m_downrect;
   RECT m_uptrackrect;
   RECT m_downtrackrect;

   bool m_bTrackingThumb;
   CPoint m_thumbdownpoint;
   int m_thumbdownpos;
   float m_scrollscale;

   CPoint m_rbuttondownpoint;
   bool m_rbuttondown;
   int m_rbuttondownpos;

   CPoint m_panpoint;
   bool m_bPanning;
   int m_panpos;

   CReplyDlg *m_pReplyDlg;

   unsigned int m_rbuttonmenufromid;

   size_t m_frame;

   bool m_bAnimating;

   bool m_dlgup;

   CPoint m_MButtonDownPoint;
   DWORD m_mbuttondowntime;
   bool m_bMButtonDown;
   bool m_bDrawMButtonDownIcon;
// Generated message map functions
protected:
   
	DECLARE_MESSAGE_MAP()
public:
   afx_msg BOOL OnEraseBkgnd(CDC* pDC){return FALSE;}
   afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
   afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
   afx_msg void OnMouseMove(UINT nFlags, CPoint point);
   afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
   afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
   afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
   afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
   afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
   afx_msg void OnEditCopy();
   afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
   afx_msg void OnEditCut();
   afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
   afx_msg void OnEditUndo();
   afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
   afx_msg void OnCopyLink();
   afx_msg void OnUpdateCopyLink(CCmdUI *pCmdUI);
   afx_msg void OnLaunchLink();
   afx_msg void OnUpdateLaunchLink(CCmdUI *pCmdUI);
   afx_msg void OnTimer(UINT_PTR nIDEvent);
   afx_msg void OnKillFocus(CWnd* pNewWnd);
   afx_msg void OnClose();
   afx_msg void OnEditFindtext();
   afx_msg void OnUpdateEditFindtext(CCmdUI *pCmdUI);
   afx_msg void OnEditExpandallthreads();
   afx_msg void OnUpdateEditExpandallthreads(CCmdUI *pCmdUI);
   afx_msg void OnEditRefresh();
   afx_msg void OnUpdateEditRefresh(CCmdUI *pCmdUI);
   afx_msg void OnEditPaste();
   afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
   afx_msg void OnEditPasteLastPost();
   afx_msg void OnUpdateEditPasteLastPost(CCmdUI *pCmdUI);
   afx_msg void OnEditNewthread();
   afx_msg void OnUpdateEditNewthread(CCmdUI *pCmdUI);
   afx_msg void OnViewIncreasefontsize();
   afx_msg void OnUpdateViewIncreasefontsize(CCmdUI *pCmdUI);
   afx_msg void OnViewDecreasefontsize();
   afx_msg void OnUpdateViewDecreasefontsize(CCmdUI *pCmdUI);
   afx_msg void OnViewResetfontsize();
   afx_msg void OnUpdateViewResetfontsize(CCmdUI *pCmdUI);
   afx_msg void OnLoadAllImages();
   afx_msg void OnUpdateLoadAllImages(CCmdUI *pCmdUI);
   afx_msg void OnCloseAllImages();
   afx_msg void OnUpdateCloseAllImages(CCmdUI *pCmdUI);
   afx_msg void OnBookmarkAdd();
   afx_msg void OnUpdateBookmarkAdd(CCmdUI *pCmdUI);
   afx_msg void OnBookmarkThisPost();
   afx_msg void OnUpdateBookmarkThisPost(CCmdUI *pCmdUI);
   afx_msg void OnPinning();
   afx_msg void OnUpdatePinning(CCmdUI *pCmdUI);
   afx_msg void OnClearAllPinnings();
   afx_msg void OnUpdateClearAllPinnings(CCmdUI *pCmdUI);
   afx_msg void OnDoublePageStory();
   afx_msg void OnUpdateDoublePageStory(CCmdUI *pCmdUI);
   afx_msg void OnOpenInTab();
   afx_msg void OnUpdateOpenInTab(CCmdUI *pCmdUI);
   afx_msg void OnHelpWiki();
   afx_msg void OnUpdateHelpWiki(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in LampView.cpp
inline CLampDoc* CLampView::GetDocument() const
   { return reinterpret_cast<CLampDoc*>(m_pDocument); }
#endif

