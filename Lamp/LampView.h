
// LampView.h : interface of the CLampView class
//


#pragma once

#include "DCSurface.h"
#include "MainFrm.h"
#include "chattypost.h"
#include "FindTextDlg.h"

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
   void SetCurrentId(unsigned int id);

   unsigned int GetHoverPreviewId(){return m_hover_preview_id;}
   void SetHoverPreviewId(unsigned int id);

   float GetHoverPreviewPercent(){return m_hover_preview_percent;}
   void SetHoverPreviewPercent(float id);

   void InvalidateSkin();

   int GetPos(){return m_pos;}

   void MakeCurrentPosVisible();

   void CloseReplyDialog();

   void SetDLGUp(bool value){m_dlgup = value;}
   bool GetDLGUp(){return m_dlgup;}

   void ResetSelection(){
   m_textselectionpost = 0;
   m_selectionstart = 0;
   m_selectionend = 0;
   }

   void SetSelection(unsigned int textselectionpost, int selectionstart, int selectionend){
   m_textselectionpost = textselectionpost;
   m_selectionstart = selectionstart;
   m_selectionend = selectionend;
   }

   void GetSelection(unsigned int &textselectionpost, int &selectionstart, int &selectionend){
   textselectionpost = m_textselectionpost;
   selectionstart = m_selectionstart;
   selectionend = m_selectionend;
   }

   void FindNext();

   void FindDlgClosing(){m_pFindDlg = NULL;}

   void CloseFindDlg();

   void DrawEverythingToBuffer(CDCSurface *pSurface = NULL, 
                               RECT *pDeviceRectangle = NULL, 
                               RECT *pScrollRectangle = NULL,
                               bool bClearHotspots = true);

   void SendMessageDlg(CLampDoc *pDoc, const UCString &to, const UCString &subject, const UCString &shackmsg);
      
   void MakeCurrentPostLegal(bool bTopOnly = false, bool bKeepInSameLocation = false, int y = 0, bool bInstant = false);

   void SetPos(int pos){m_gotopos = m_pos = pos;}

   void MakeMousePointInvalid(){m_mousepoint.x = -1000; m_mousepoint.y = -1000;}

protected:
   void DrawHotSpots(HDC hDC);
   bool DrawCurrentHotSpots(HDC hDC);
   void DrawTextSelection(HDC hDC, const RECT &DeviceRectangle);
   void DrawScrollbar(HDC hDC, const RECT &ScrollRectangle, std::vector<CHotSpot> &hotspots);
   void DrawThumb(HDC hDC, const RECT &thumbrect, scrollbitype type);
   void UpdateHotspotPosition();

   void BeginInertiaPanning();
   void CancelInertiaPanning();
   void TrackMouse(CPoint &point);
   void UpdateCurrentIdAsRoot(unsigned int id);
      
   void MakePosLegal();
   CDCSurface *m_backbuffer;
   CDCSurface m_backbuffer1;
   CDCSurface m_backbuffer2;
   CDCSurface m_whitebuffer;
   CDCSurface m_bannerbuffer;
   CDCSurface m_replybuffer;
   CDCSurface m_whitereplybuffer;
   RECT m_BannerRectangle;
   int m_pos;
   int m_gotopos;
   int m_lastdrawnpos;
   CPoint m_mousepoint;
   std::vector<CHotSpot> m_hotspots;
   std::vector<CHotSpot> m_message_hotspots;
   RECT *m_hotspot;
   RECT *m_lasthotspot;
   hotspottype m_hotspottype;

   bool m_bDrawHotsOnly;
   bool m_bPanOnly;
   bool m_bForceDrawAll;
   bool m_bLButtonDownOnScrollArrow;
   unsigned int m_current_id;
   std::list<unsigned int> m_history_list;
   std::list<unsigned int>::iterator m_history_it;
   unsigned int m_hover_preview_id;

   bool m_bDraggingTextSelection;
   unsigned int m_textselectionpost;
   int m_selectionstart;
   int m_selectionend;

   int m_doubleclickselectionstart;
   int m_doubleclickselectionend;

   int m_selectionstart_actual;
   int m_selectionend_actual;
   int m_lastcharpos;

   DWORD m_doubleclicktime;
   bool m_bDoubleClickDragging;

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

   bool m_bInertialPanning;
   bool m_bStartedTrackingMouse;
   float m_inertia;
   std::deque<int> m_mousehistory;
   std::deque<int> m_mousetimehistory;
   DWORD m_lastmousetime;
   bool m_brakes;

   CFindTextDlg *m_pFindDlg;

   RECT m_ScrollRectangle;

   int m_banneroffset;

   bool m_bModToolIsUp;
   RECT m_ModToolRect;
   unsigned int m_ModToolPostID;

   unsigned int m_PREVIEW_TIMER_id;

   bool m_mouseOverClientArea;

   float m_hover_preview_percent;

   UCString m_authorname_clicked;

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
   afx_msg void OnHideCollapsedThreads();
   afx_msg void OnUpdateHideCollapsedThreads(CCmdUI *pCmdUI);
   afx_msg void OnInfinatePaging();
   afx_msg void OnUpdateInfinatePaging(CCmdUI *pCmdUI);
   afx_msg void OnGotoNewPost();
   afx_msg void OnUpdateGotoNewPost(CCmdUI *pCmdUI);
   afx_msg void OnClearAllPinnings();
   afx_msg void OnUpdateClearAllPinnings(CCmdUI *pCmdUI);
   afx_msg void OnDoublePageStory();
   afx_msg void OnUpdateDoublePageStory(CCmdUI *pCmdUI);
   afx_msg void OnOpenInTab();
   afx_msg void OnUpdateOpenInTab(CCmdUI *pCmdUI);
   afx_msg void OnHelpWiki();
   afx_msg void OnUpdateHelpWiki(CCmdUI *pCmdUI);
   afx_msg void OnShowLOLButtons();
   afx_msg void OnUpdateShowLOLButtons(CCmdUI *pCmdUI);
   afx_msg void OnCheckSpelling();
   afx_msg void OnUpdateCheckSpelling(CCmdUI *pCmdUI);
   afx_msg void OnFilterNWS();
   afx_msg void OnUpdateFilterNWS(CCmdUI *pCmdUI);
   afx_msg void OnFilterINF();
   afx_msg void OnUpdateFilterINF(CCmdUI *pCmdUI);
   afx_msg void OnFilterOFFTOPIC();
   afx_msg void OnUpdateFilterOFFTOPIC(CCmdUI *pCmdUI);
   afx_msg void OnFilterSTUPID();
   afx_msg void OnUpdateFilterSTUPID(CCmdUI *pCmdUI);
   afx_msg void OnFilterPOLITICAL();
   afx_msg void OnUpdateFilterPOLITICAL(CCmdUI *pCmdUI);
   afx_msg void OnSkinRoundShack();
   afx_msg void OnUpdateSkinRoundShack(CCmdUI *pCmdUI);
   afx_msg void OnSkinSquareShack();
   afx_msg void OnUpdateSkinSquareShack(CCmdUI *pCmdUI);
   afx_msg void OnSkinWorksafeShack();
   afx_msg void OnUpdateSkinWorksafeShack(CCmdUI *pCmdUI);
   afx_msg void OnSkinCustom();
   afx_msg void OnUpdateSkinCustom(CCmdUI *pCmdUI);
   afx_msg void OnHighlightOP();
   afx_msg void OnUpdateHighlightOP(CCmdUI *pCmdUI);
   afx_msg void OnShowSettings();
   afx_msg void OnUpdateShowSettings(CCmdUI *pCmdUI);
   afx_msg void OnNormalFont();
   afx_msg void OnUpdateNormalFont(CCmdUI *pCmdUI);
   afx_msg void OnQuotedFont();
   afx_msg void OnUpdateQuotedFont(CCmdUI *pCmdUI);
   afx_msg void OnCodeFont();
   afx_msg void OnUpdateCodeFont(CCmdUI *pCmdUI);
   afx_msg void OnSmallLOL();
   afx_msg void OnUpdateSmallLOL(CCmdUI *pCmdUI);
   afx_msg void OnSelectDarkBlue();
   afx_msg void OnUpdateSelectDarkBlue(CCmdUI *pCmdUI);
   afx_msg void OnSelectBlue();
   afx_msg void OnUpdateSelectBlue(CCmdUI *pCmdUI);
   afx_msg void OnSelectYellow();
   afx_msg void OnUpdateSelectYellow(CCmdUI *pCmdUI);
   afx_msg void OnSelectPaleYellow();
   afx_msg void OnUpdateSelectPaleYellow(CCmdUI *pCmdUI);
   afx_msg void OnSelectCustom();
   afx_msg void OnUpdateSelectCustom(CCmdUI *pCmdUI);
   afx_msg void OnFlaredBranches();
   afx_msg void OnUpdateFlaredBranches(CCmdUI *pCmdUI);
   afx_msg void OnAltPostkeys();
   afx_msg void OnUpdateAltPostkeys(CCmdUI *pCmdUI);
   afx_msg void OnLoadImagesInLamp();
   afx_msg void OnUpdateLoadImagesInLamp(CCmdUI *pCmdUI);
   afx_msg void OnGoogleSelected();
   afx_msg void OnUpdateGoogleSelected(CCmdUI *pCmdUI);
   afx_msg void OnGoogleSelectedWQuotes();
   afx_msg void OnUpdateGoogleSelectedWQuotes(CCmdUI *pCmdUI);
   afx_msg void OnWikipediaSelected();
   afx_msg void OnUpdateWikipediaSelected(CCmdUI *pCmdUI);
   afx_msg void OnAutoCheckInbox();
   afx_msg void OnUpdateAutoCheckInbox(CCmdUI *pCmdUI);
   afx_msg void OnShowRootSelected();
   afx_msg void OnUpdateShowRootSelected(CCmdUI *pCmdUI);
   afx_msg void OnShowThomWLOLS();
   afx_msg void OnUpdateShowThomWLOLS(CCmdUI *pCmdUI);
   afx_msg void OnVerboseLOLPreviews();
   afx_msg void OnUpdateVerboseLOLPreviews(CCmdUI *pCmdUI);
   afx_msg void OnInvertedLOLPreviews();
   afx_msg void OnUpdateInvertedLOLPreviews(CCmdUI *pCmdUI);
   afx_msg void OnBackId();
   afx_msg void OnUpdateBackId(CCmdUI *pCmdUI);
   afx_msg void OnForeId();
   afx_msg void OnUpdateForeId(CCmdUI *pCmdUI);
   afx_msg void OnUnloadAllImages();
   afx_msg void OnUpdateUnloadAllImages(CCmdUI *pCmdUI);
   afx_msg void OnMouseLeave();
   afx_msg void OnExpandPreviewsDown();
   afx_msg void OnUpdateExpandPreviewsDown(CCmdUI *pCmdUI);
   afx_msg void OnExpandPreviews();
   afx_msg void OnUpdateExpandPreviews(CCmdUI *pCmdUI);
   afx_msg void OnCheckUpdate();
   afx_msg void OnUpdateCheckUpdate(CCmdUI *pCmdUI);
   afx_msg void OnViewProfile();
   afx_msg void OnUpdateViewProfile(CCmdUI *pCmdUI);
   afx_msg void OnSendMessage();
   afx_msg void OnUpdateSendMessage(CCmdUI *pCmdUI);
   afx_msg void OnViewComments();
   afx_msg void OnUpdateViewComments(CCmdUI *pCmdUI);
   afx_msg void OnStrokePreviewEdges();
   afx_msg void OnUpdateStrokePreviewEdges(CCmdUI *pCmdUI);
   afx_msg void OnLineThickness1();
   afx_msg void OnUpdateLineThickness1(CCmdUI *pCmdUI);
   afx_msg void OnLineThickness2();
   afx_msg void OnUpdateLineThickness2(CCmdUI *pCmdUI);
   afx_msg void OnLineThickness3();
   afx_msg void OnUpdateLineThickness3(CCmdUI *pCmdUI);
   afx_msg void OnUseAuthorColor();
   afx_msg void OnUpdateUseAuthorColor(CCmdUI *pCmdUI);
   afx_msg void OnUsersLOLs();
   afx_msg void OnUpdateUsersLOLs(CCmdUI *pCmdUI);

};

#ifndef _DEBUG  // debug version in LampView.cpp
inline CLampDoc* CLampView::GetDocument() const
   { return reinterpret_cast<CLampDoc*>(m_pDocument); }
#endif

