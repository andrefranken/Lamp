
// LampView.cpp : implementation of the CLampView class
//

#include "stdafx.h"
#include "comm.h"
#include "html.h"
#include "Lamp.h"
#include "SettingsDlg.h"
#include "LampDoc.h"
#include "LampView.h"
#include "MainFrm.h"
#include "ReplyDlg.h"
#include "DockTab.h"
#include "BookmarkDlg.h"
#include "AuthorDlg.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLampView

IMPLEMENT_DYNCREATE(CLampView, CView)

BEGIN_MESSAGE_MAP(CLampView, CView)
   ON_WM_CLOSE()
   ON_WM_KILLFOCUS()
   ON_WM_SETFOCUS()
   ON_WM_ERASEBKGND()
   ON_WM_MOUSEWHEEL()
   ON_WM_LBUTTONDOWN()
   ON_WM_LBUTTONUP()
   ON_WM_MOUSEMOVE()
   ON_WM_RBUTTONDOWN()
   ON_WM_RBUTTONUP()
   ON_WM_MBUTTONDOWN()
   ON_WM_MBUTTONUP()
   ON_WM_KEYDOWN()
   ON_WM_CHAR()
   ON_WM_SETCURSOR()
   ON_WM_LBUTTONDBLCLK()
   ON_WM_TIMER()
   ON_WM_MOUSELEAVE()
   ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
   ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
   ON_COMMAND(ID_EDIT_CUT, OnEditCut)
   ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
   ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
   ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
   ON_COMMAND(ID_EDIT_FIND, OnEditFindtext)
   ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateEditFindtext)
   ON_COMMAND(ID_EDIT_REFRESH, OnEditRefresh)
   ON_UPDATE_COMMAND_UI(ID_EDIT_REFRESH, OnUpdateEditRefresh)
   ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
   ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
   ON_COMMAND(ID_EDIT_PASTE_LAST_POST, OnEditPasteLastPost)
   ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_LAST_POST, OnUpdateEditPasteLastPost)
   ON_COMMAND(ID_EDIT_NEWTHREAD, &CLampView::OnEditNewthread)
   ON_UPDATE_COMMAND_UI(ID_EDIT_NEWTHREAD, &CLampView::OnUpdateEditNewthread)
   ON_COMMAND(ID_COPYLINK, &CLampView::OnCopyLink)
   ON_UPDATE_COMMAND_UI(ID_COPYLINK, &CLampView::OnUpdateCopyLink)
   ON_COMMAND(ID_LAUNCHLINK, &CLampView::OnLaunchLink)
   ON_UPDATE_COMMAND_UI(ID_LAUNCHLINK, &CLampView::OnUpdateLaunchLink)
   ON_COMMAND(ID_VIEW_INCREASEFONTSIZE, &CLampView::OnViewIncreasefontsize)
   ON_UPDATE_COMMAND_UI(ID_VIEW_INCREASEFONTSIZE, &CLampView::OnUpdateViewIncreasefontsize)
   ON_COMMAND(ID_VIEW_DECREASEFONTSIZE, &CLampView::OnViewDecreasefontsize)
   ON_UPDATE_COMMAND_UI(ID_VIEW_DECREASEFONTSIZE, &CLampView::OnUpdateViewDecreasefontsize)
   ON_COMMAND(ID_VIEW_RESETFONTSIZE, &CLampView::OnViewResetfontsize)
   ON_UPDATE_COMMAND_UI(ID_VIEW_RESETFONTSIZE, &CLampView::OnUpdateViewResetfontsize)
   ON_COMMAND(ID_EDIT_LOADALLIMAGES, &CLampView::OnLoadAllImages)
   ON_UPDATE_COMMAND_UI(ID_EDIT_LOADALLIMAGES, &CLampView::OnUpdateLoadAllImages)
   ON_COMMAND(ID_EDIT_CLOSEALLIMAGES, &CLampView::OnCloseAllImages)
   ON_UPDATE_COMMAND_UI(ID_EDIT_CLOSEALLIMAGES, &CLampView::OnUpdateCloseAllImages)
   ON_COMMAND(ID_BOOKMARK_ADD, &CLampView::OnBookmarkAdd)
   ON_UPDATE_COMMAND_UI(ID_BOOKMARK_ADD, &CLampView::OnUpdateBookmarkAdd)
   ON_COMMAND(ID_EDIT_BOOKMARK_THIS_POST, &CLampView::OnBookmarkThisPost)
   ON_UPDATE_COMMAND_UI(ID_EDIT_BOOKMARK_THIS_POST, &CLampView::OnUpdateBookmarkThisPost)
   ON_COMMAND(ID_PINNING_MENUITEM, &CLampView::OnPinning)
   ON_UPDATE_COMMAND_UI(ID_PINNING_MENUITEM, &CLampView::OnUpdatePinning)
   ON_COMMAND(ID_HIDE_COLLAPSED_POSTS, &CLampView::OnHideCollapsedThreads)
   ON_UPDATE_COMMAND_UI(ID_HIDE_COLLAPSED_POSTS, &CLampView::OnUpdateHideCollapsedThreads)
   ON_COMMAND(ID_INFINATE_PAGING, &CLampView::OnInfinatePaging)
   ON_UPDATE_COMMAND_UI(ID_INFINATE_PAGING, &CLampView::OnUpdateInfinatePaging)
   ON_COMMAND(ID_GOTONEWPOST, &CLampView::OnGotoNewPost)
   ON_UPDATE_COMMAND_UI(ID_GOTONEWPOST, &CLampView::OnUpdateGotoNewPost)
   ON_COMMAND(ID_CLEAR_ALL_PINNINGS_MENUITEM, &CLampView::OnClearAllPinnings)
   ON_UPDATE_COMMAND_UI(ID_CLEAR_ALL_PINNINGS_MENUITEM, &CLampView::OnUpdateClearAllPinnings)
   ON_COMMAND(ID_DOUBLE_PAGE_STORY_MENUITEM, &CLampView::OnDoublePageStory)
   ON_UPDATE_COMMAND_UI(ID_DOUBLE_PAGE_STORY_MENUITEM, &CLampView::OnUpdateDoublePageStory)
   ON_COMMAND(ID_OPENINTAB, &CLampView::OnOpenInTab)
   ON_UPDATE_COMMAND_UI(ID_OPENINTAB, &CLampView::OnUpdateOpenInTab)
   ON_COMMAND(ID_HELP_WIKI, &CLampView::OnHelpWiki)
   ON_UPDATE_COMMAND_UI(ID_HELP_WIKI, &CLampView::OnUpdateHelpWiki)
   ON_COMMAND(ID_SHOW_LOL_BUTTONS, &CLampView::OnShowLOLButtons)
   ON_UPDATE_COMMAND_UI(ID_SHOW_LOL_BUTTONS, &CLampView::OnUpdateShowLOLButtons)
   ON_COMMAND(ID_CHECK_SPELLING, &CLampView::OnCheckSpelling)
   ON_UPDATE_COMMAND_UI(ID_CHECK_SPELLING, &CLampView::OnUpdateCheckSpelling)
   ON_COMMAND(ID_FILTER_ENABLE_NWS, &CLampView::OnFilterNWS)
   ON_UPDATE_COMMAND_UI(ID_FILTER_ENABLE_NWS, &CLampView::OnUpdateFilterNWS)
   ON_COMMAND(ID_FILTER_ENABLE_INF, &CLampView::OnFilterINF)
   ON_UPDATE_COMMAND_UI(ID_FILTER_ENABLE_INF, &CLampView::OnUpdateFilterINF)
   ON_COMMAND(ID_FILTER_ENABLE_OFFTOPIC, &CLampView::OnFilterOFFTOPIC)
   ON_UPDATE_COMMAND_UI(ID_FILTER_ENABLE_OFFTOPIC, &CLampView::OnUpdateFilterOFFTOPIC)
   ON_COMMAND(ID_FILTER_ENABLE_STUPID, &CLampView::OnFilterSTUPID)
   ON_UPDATE_COMMAND_UI(ID_FILTER_ENABLE_STUPID, &CLampView::OnUpdateFilterSTUPID)
   ON_COMMAND(ID_FILTER_ENABLE_POLITICAL, &CLampView::OnFilterPOLITICAL)
   ON_UPDATE_COMMAND_UI(ID_FILTER_ENABLE_POLITICAL, &CLampView::OnUpdateFilterPOLITICAL)
   ON_COMMAND(ID_SKIN_ROUNDSHACK, &CLampView::OnSkinRoundShack)
   ON_UPDATE_COMMAND_UI(ID_SKIN_ROUNDSHACK, &CLampView::OnUpdateSkinRoundShack)
   ON_COMMAND(ID_SKIN_DEFAULT, &CLampView::OnSkinSquareShack)
   ON_UPDATE_COMMAND_UI(ID_SKIN_DEFAULT, &CLampView::OnUpdateSkinSquareShack)
   ON_COMMAND(ID_SKIN_WORKSAFE, &CLampView::OnSkinWorksafeShack)
   ON_UPDATE_COMMAND_UI(ID_SKIN_WORKSAFE, &CLampView::OnUpdateSkinWorksafeShack)
   ON_COMMAND(ID_SKIN_CUSTOM, &CLampView::OnSkinCustom)
   ON_UPDATE_COMMAND_UI(ID_SKIN_CUSTOM, &CLampView::OnUpdateSkinCustom)
   ON_COMMAND(ID_HIGHLIGHT_OP, &CLampView::OnHighlightOP)
   ON_UPDATE_COMMAND_UI(ID_HIGHLIGHT_OP, &CLampView::OnUpdateHighlightOP)
   ON_COMMAND(ID_SHOW_SETTINGS_DIALOG, &CLampView::OnShowSettings)
   ON_UPDATE_COMMAND_UI(ID_SHOW_SETTINGS_DIALOG, &CLampView::OnUpdateShowSettings)
   ON_COMMAND(ID_NORM_FONT, &CLampView::OnNormalFont)
   ON_UPDATE_COMMAND_UI(ID_NORM_FONT, &CLampView::OnUpdateNormalFont)
   ON_COMMAND(ID_QUOTE_FONT, &CLampView::OnQuotedFont)
   ON_UPDATE_COMMAND_UI(ID_QUOTE_FONT, &CLampView::OnUpdateQuotedFont)
   ON_COMMAND(ID_CODE_FONT, &CLampView::OnCodeFont)
   ON_UPDATE_COMMAND_UI(ID_CODE_FONT, &CLampView::OnUpdateCodeFont)
   ON_COMMAND(ID_SMALL_LOL, &CLampView::OnSmallLOL)
   ON_UPDATE_COMMAND_UI(ID_SMALL_LOL, &CLampView::OnUpdateSmallLOL)
   ON_COMMAND(ID_SELECT_DARK_BLUE, &CLampView::OnSelectDarkBlue)
   ON_UPDATE_COMMAND_UI(ID_SELECT_DARK_BLUE, &CLampView::OnUpdateSelectDarkBlue)
   ON_COMMAND(ID_SELECT_BLUE, &CLampView::OnSelectBlue)
   ON_UPDATE_COMMAND_UI(ID_SELECT_BLUE, &CLampView::OnUpdateSelectBlue)
   ON_COMMAND(ID_SELECT_YELLOW, &CLampView::OnSelectYellow)
   ON_UPDATE_COMMAND_UI(ID_SELECT_YELLOW, &CLampView::OnUpdateSelectYellow)
   ON_COMMAND(ID_SELECT_PALE_YELLOW, &CLampView::OnSelectPaleYellow)
   ON_UPDATE_COMMAND_UI(ID_SELECT_PALE_YELLOW, &CLampView::OnUpdateSelectPaleYellow)
   ON_COMMAND(ID_SELECT_CUSTOM, &CLampView::OnSelectCustom)
   ON_UPDATE_COMMAND_UI(ID_SELECT_CUSTOM, &CLampView::OnUpdateSelectCustom)
   ON_COMMAND(ID_FLAREDBRANCHES, &CLampView::OnFlaredBranches)
   ON_UPDATE_COMMAND_UI(ID_FLAREDBRANCHES, &CLampView::OnUpdateFlaredBranches)
   ON_COMMAND(ID_ALT_POSTKEYS, &CLampView::OnAltPostkeys)
   ON_UPDATE_COMMAND_UI(ID_ALT_POSTKEYS, &CLampView::OnUpdateAltPostkeys)
   ON_COMMAND(ID_LOADIMAGESINLAMP, &CLampView::OnLoadImagesInLamp)
   ON_UPDATE_COMMAND_UI(ID_LOADIMAGESINLAMP, &CLampView::OnUpdateLoadImagesInLamp)
   ON_COMMAND(ID_GOOGLE_SELECTED, &CLampView::OnGoogleSelected)
   ON_UPDATE_COMMAND_UI(ID_GOOGLE_SELECTED, &CLampView::OnUpdateGoogleSelected)
   ON_COMMAND(ID_GOOGLE_SELECTED_W_QUOTES, &CLampView::OnGoogleSelectedWQuotes)
   ON_UPDATE_COMMAND_UI(ID_GOOGLE_SELECTED_W_QUOTES, &CLampView::OnUpdateGoogleSelectedWQuotes)
   ON_COMMAND(ID_WIKIPEDIA_SELECTED, &CLampView::OnWikipediaSelected)
   ON_UPDATE_COMMAND_UI(ID_WIKIPEDIA_SELECTED, &CLampView::OnUpdateWikipediaSelected)
   ON_COMMAND(ID_AUTOCHECKINBOX, &CLampView::OnAutoCheckInbox)
   ON_UPDATE_COMMAND_UI(ID_AUTOCHECKINBOX, &CLampView::OnUpdateAutoCheckInbox)
   ON_COMMAND(ID_SHOW_ROOT_SELECTED, &CLampView::OnShowRootSelected)
   ON_UPDATE_COMMAND_UI(ID_SHOW_ROOT_SELECTED, &CLampView::OnUpdateShowRootSelected)
   ON_COMMAND(ID_SHOW_THOMW_LOLS, &CLampView::OnShowThomWLOLS)
   ON_UPDATE_COMMAND_UI(ID_SHOW_THOMW_LOLS, &CLampView::OnUpdateShowThomWLOLS)
   ON_COMMAND(ID_VERBOSE_LOL_PREVIEWS, &CLampView::OnVerboseLOLPreviews)
   ON_UPDATE_COMMAND_UI(ID_VERBOSE_LOL_PREVIEWS, &CLampView::OnUpdateVerboseLOLPreviews)
   ON_COMMAND(ID_INVERTED_LOL_PREVIEWS, &CLampView::OnInvertedLOLPreviews)
   ON_UPDATE_COMMAND_UI(ID_INVERTED_LOL_PREVIEWS, &CLampView::OnUpdateInvertedLOLPreviews)
   ON_COMMAND(ID_UNLOADALLIMAGES, &CLampView::OnUnloadAllImages)
   ON_UPDATE_COMMAND_UI(ID_UNLOADALLIMAGES, &CLampView::OnUpdateUnloadAllImages)
   ON_COMMAND(ID_EXPAND_PREVIEWS_DOWN, &CLampView::OnExpandPreviewsDown)
   ON_UPDATE_COMMAND_UI(ID_EXPAND_PREVIEWS_DOWN, &CLampView::OnUpdateExpandPreviewsDown)
   ON_COMMAND(ID_STROKE_PREVIEW_EDGES, &CLampView::OnStrokePreviewEdges)
   ON_UPDATE_COMMAND_UI(ID_STROKE_PREVIEW_EDGES, &CLampView::OnUpdateStrokePreviewEdges)
   ON_COMMAND(ID_EXPAND_PREVIEWS, &CLampView::OnExpandPreviews)
   ON_UPDATE_COMMAND_UI(ID_EXPAND_PREVIEWS, &CLampView::OnUpdateExpandPreviews)
   ON_COMMAND(ID_HELP_CHECK_UPDATE, &CLampView::OnCheckUpdate)
   ON_UPDATE_COMMAND_UI(ID_HELP_CHECK_UPDATE, &CLampView::OnUpdateCheckUpdate)
   ON_COMMAND(ID_BACK_ID, &CLampView::OnBackId)
   ON_UPDATE_COMMAND_UI(ID_BACK_ID, &CLampView::OnUpdateBackId)
   ON_COMMAND(ID_FORE_ID, &CLampView::OnForeId)
   ON_UPDATE_COMMAND_UI(ID_FORE_ID, &CLampView::OnUpdateForeId)
   ON_COMMAND(ID_VIEWPROFILE, &CLampView::OnViewProfile)
   ON_UPDATE_COMMAND_UI(ID_VIEWPROFILE, &CLampView::OnUpdateViewProfile)
   ON_COMMAND(ID_SENDMESSAGE, &CLampView::OnSendMessage)
   ON_UPDATE_COMMAND_UI(ID_SENDMESSAGE, &CLampView::OnUpdateSendMessage)
   ON_COMMAND(ID_VIEWCOMMENTS, &CLampView::OnViewComments)
   ON_UPDATE_COMMAND_UI(ID_VIEWCOMMENTS, &CLampView::OnUpdateViewComments)
   ON_COMMAND(ID_LINE_THICKNESS_1, &CLampView::OnLineThickness1)
   ON_UPDATE_COMMAND_UI(ID_LINE_THICKNESS_1, &CLampView::OnUpdateLineThickness1)
   ON_COMMAND(ID_LINE_THICKNESS_2, &CLampView::OnLineThickness2)
   ON_UPDATE_COMMAND_UI(ID_LINE_THICKNESS_2, &CLampView::OnUpdateLineThickness2)
   ON_COMMAND(ID_LINE_THICKNESS_3, &CLampView::OnLineThickness3)
   ON_UPDATE_COMMAND_UI(ID_LINE_THICKNESS_3, &CLampView::OnUpdateLineThickness3)
   ON_COMMAND(ID_USE_AUTHOR_COLOR_PREVIEW, &CLampView::OnUseAuthorColor)
   ON_UPDATE_COMMAND_UI(ID_USE_AUTHOR_COLOR_PREVIEW, &CLampView::OnUpdateUseAuthorColor)
   ON_COMMAND(ID_USERS_LOLS, &CLampView::OnUsersLOLs)
   ON_UPDATE_COMMAND_UI(ID_USERS_LOLS, &CLampView::OnUpdateUsersLOLs)

   END_MESSAGE_MAP()

// CLampView construction/destruction

CLampView::CLampView()
{
	m_pos = 0;
   m_gotopos = 0;
   m_lastdrawnpos = 0;
   m_hotspot = NULL;
   m_lasthotspot = NULL;
   m_bDrawHotsOnly = false;
   m_bPanOnly = false;
   m_bForceDrawAll = true;
   m_current_id = 0;
   m_hover_preview_id = 0;
   m_bDraggingTextSelection = false;
   m_textselectionpost = 0;
   m_selectionstart = 0;
   m_selectionend = 0;
   m_doubleclicktime = 0;
   m_bTrackingThumb = false;
   m_bLButtonDownOnScrollArrow = false;
   m_bPanning = false;
   m_panpos = 0;
   m_rbuttonmenufromid = 0;
   m_pReplyDlg = NULL;
   m_backbuffer = &m_backbuffer1;
   m_frame = 0;
   m_bAnimating = false;
   m_dlgup = false;
   m_bMButtonDown = false;
   m_bDrawMButtonDownIcon = false;
   m_bInertialPanning = false;
   m_inertia = 0.0f;
   m_bStartedTrackingMouse = false;
   m_brakes = false;
   m_bDoubleClickDragging = false;
   m_banneroffset = 0;
   m_bModToolIsUp = false;
   m_PREVIEW_TIMER_id = 0;
   m_mouseOverClientArea = false;
   m_hover_preview_percent = 1.0f;
   m_history_it = m_history_list.end();
   
   m_pFindDlg = NULL;

   theApp.AddView(this);
}

CLampView::~CLampView()
{
   if(m_pFindDlg != NULL)
   {
      delete(m_pFindDlg);
      m_pFindDlg = NULL;
   }

   ((CMainFrame*)theApp.GetMainWnd())->SetActiveLampView(NULL);
   if(m_pReplyDlg != NULL)
   {
      delete(m_pReplyDlg);
      m_pReplyDlg = NULL;
   }

   theApp.RemoveView(this);
}

BOOL CLampView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CLampView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

   CChildFrame *pFrame = (CChildFrame*)GetParentFrame();
   pFrame->SetView(this);

   SetFocus();

   GetDocument()->SetView(this);

   SetTimer(ANIM_TIMER,100,NULL);
}

void CLampView::OnClose()
{
   KillTimer(ANIM_TIMER);
}

void CLampView::SetCurrentId(unsigned int id)
{
   if(id != m_current_id)
   {
      // erase front of history up to the current id
      if(m_history_list.size() > 0 &&
         m_history_it != m_history_list.end())
      {
         std::list<unsigned int>::iterator it = m_history_list.begin();
         while(m_history_list.size() > 0 &&
               it != m_history_it)
         {
            it++;
            m_history_list.pop_front();
         }
      }

      if(m_history_list.size() == 0 &&
         m_current_id != 0)
      {
         // history is empty and current is valid.
         // add it
         m_history_list.push_front(m_current_id);
      }

      if(m_history_list.size() > 0 ||
         id != 0)
      {
         m_history_list.push_front(id);
      }

      if(m_history_list.size() > 0)
      {
         m_history_it = m_history_list.begin();
      }
      else
      {
         m_history_it = m_history_list.end();
      }

      while(m_history_list.size() > 2000)
      {
         m_history_list.pop_back();
      }

      m_current_id = id;
      
      if(m_current_id != 0)
      {
         GetDocument()->MakePostAvailable(id);
      }
   }
}

void CLampView::SetHoverPreviewId(unsigned int id)
{
   if(theApp.ExpandPreviews())
   {
      if(id != m_hover_preview_id)
      {
         if(id != 0)
         {
            ChattyPost *pPost = GetDocument()->FindPost(id);
            if(pPost != NULL)
            {            
               m_hover_preview_id = id;
               m_hover_preview_percent = 0.0f;
               InvalidateEverythingPan();
            }

            GetDocument()->MakePostAvailable(id);
         }
         else if(m_hover_preview_id != 0)
         {
            ChattyPost *pPost = GetDocument()->FindPost(m_hover_preview_id);
            if(pPost != NULL)
            {            
               m_hover_preview_id = 0;
               m_hover_preview_percent = 1.0f;
               InvalidateEverything();
            }
         }
      }
   }
   else
   {
      if(m_hover_preview_id != 0)
      {
         m_hover_preview_id = 0;
         m_hover_preview_percent = 1.0f;
         InvalidateEverything();
      }
   }
}

void CLampView::OnRButtonDown(UINT nFlags, CPoint point)
{
   m_hover_preview_percent = 1.0;
   m_hover_preview_id = 0;

   SetCapture();   
   CancelInertiaPanning();
   m_bStartedTrackingMouse = true;
   m_lastmousetime = ::GetTickCount();
   m_mousepoint = point;

   if(!m_brakes)
   {
      m_bDrawMButtonDownIcon = false;

      if(m_pReplyDlg == NULL ||
         !m_pReplyDlg->OnRButtonDown(nFlags, point))
      {
         m_rbuttondownpoint = point;
         m_rbuttondownpos = m_pos;

         m_panpoint = point;
         m_bPanning = true;
         m_panpos = m_pos;
      }
   }
   else
   {
      m_panpoint = point;
      m_bPanning = true;
      m_panpos = m_pos;
   }

   if(m_bModToolIsUp)
   {
      m_bModToolIsUp = false;
      InvalidateEverything();
   }
}

void CLampView::OnRButtonUp(UINT nFlags, CPoint point)
{
   TrackMouse(point);
   ReleaseCapture();
      
   if(!m_brakes)
   {
      if(m_bPanning)
      {
         m_bPanning = false;
         int ydiff = m_panpoint.y - point.y;
         m_gotopos = m_panpos + ydiff;
         MakePosLegal();
         m_pos = m_gotopos;// no smooth on pan
         BeginInertiaPanning();
      }

      // force a draw so that hotspots are updated
      DrawEverythingToBuffer();

      if(abs(point.x - m_rbuttondownpoint.x) < 3 &&
         abs(point.y - m_rbuttondownpoint.y) < 3)
      {
         // stop the attempt to start inertial panning
         CancelInertiaPanning();
         ClientToScreen(&point);
         OnContextMenu(this, point);
      }
   }
   else if(m_bPanning)
   {
      m_bPanning = false;
      int ydiff = m_panpoint.y - point.y;
      m_gotopos = m_panpos + ydiff;
      MakePosLegal();
      m_pos = m_gotopos;// no smooth on pan
      BeginInertiaPanning();
   }
   m_brakes = false;
}

void CLampView::TrackMouse(CPoint &point)
{
   if(m_bStartedTrackingMouse)
   {
      m_mousehistory.push_back(point.y - m_mousepoint.y);
      while(m_mousehistory.size() > 5) m_mousehistory.pop_front();
      DWORD thistime = ::GetTickCount();
      m_mousetimehistory.push_back(thistime - m_lastmousetime);
      while(m_mousetimehistory.size() > 5) m_mousetimehistory.pop_front();
      m_lastmousetime = thistime;
   }
   m_bStartedTrackingMouse = true;
   m_mousepoint = point;
}

void CLampView::CancelInertiaPanning()
{
   if(m_bInertialPanning)
   {
      m_brakes = true;
   }
   m_inertia = 0.0f;
   m_bInertialPanning = false;
   m_mousehistory.clear();
   m_mousetimehistory.clear();
}

void CLampView::BeginInertiaPanning()
{
   m_bInertialPanning = true;
   m_inertia = 0.0f;
   int numused = 0;
   if(m_mousehistory.size() == m_mousetimehistory.size())
   {
      int biggestdiff = 0;
      for(size_t i = 0; i < m_mousehistory.size(); i++)
      {
         int diff = m_mousehistory[i];
         int time = m_mousetimehistory[i];
         if(time < 25)
         {
            if(abs(diff) > abs(biggestdiff))
            {
               biggestdiff = diff;
            }
         }
         else
         {
            if(diff == 0)
            {
               biggestdiff = 0;
               break;
            }
         }

         /*
         int diff = m_mousehistory[i];
         int time = m_mousetimehistory[i];
         if(time < 25) time = 1;
         if(diff != 0 && time != 0)
         {
            m_inertia += (float)(diff * time);
         }
         numused += time;
         */
      }
      
      if(biggestdiff != 0)
      {
         m_inertia = (float)biggestdiff;
      }
      else
      {
         m_bInertialPanning = false;
         m_inertia = 0.0f;
      }
   }
   
   m_mousehistory.clear();
   m_mousetimehistory.clear();

   InvalidateEverythingPan();
}

void CLampView::OnContextMenu(CWnd* pWnd, CPoint point)
{
   CPoint worldpoint = point;
   ScreenToClient(&worldpoint);
   
   m_mousepoint = worldpoint;

   m_rbuttonmenufromid = 0;

   bool bDoLink = false;

   for(size_t i = 0; i < m_hotspots.size(); i++)
   {
      if(m_mousepoint.x >= m_hotspots[i].m_spot.left &&
         m_mousepoint.x < m_hotspots[i].m_spot.right &&
         m_mousepoint.y >= m_hotspots[i].m_spot.top &&
         m_mousepoint.y < m_hotspots[i].m_spot.bottom)
      {
         if(m_hotspots[i].m_type == HST_LINK ||
            m_hotspots[i].m_type == HST_IMAGE_LINK ||
            m_hotspots[i].m_type == HST_IMAGE ||
            m_hotspots[i].m_type == HST_THUMB ||
            m_hotspots[i].m_type == HST_OPENINTAB ||
            m_hotspots[i].m_type == HST_PIN ||
            m_hotspots[i].m_type == HST_REFRESH)
         {
            bDoLink = true;
         }
         else if(m_hotspots[i].m_type == HST_TEXT)
         {
            m_rbuttonmenufromid = m_hotspots[i].m_id;
         }
         break;
      }
   }

	if(bDoLink)
   {
      theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_COPYLINK_MENU, point.x, point.y, this, TRUE);
   }
   else
   {
      theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
   }
}


// CLampView diagnostics

#ifdef _DEBUG
void CLampView::AssertValid() const
{
	CView::AssertValid();
}

void CLampView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLampDoc* CLampView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLampDoc)));
	return (CLampDoc*)m_pDocument;
}
#endif //_DEBUG


// CLampView message handlers
void CLampView::OnDraw(CDC* pDC)
{
   HDC hDC = pDC->GetSafeHdc();
   CLampDoc *pDoc = GetDocument();

   if(hDC != NULL &&
      pDoc != NULL)
   {
      RECT DeviceRectangle;
      GetClientRect(&DeviceRectangle);
      RECT OriginalDeviceRectangle = DeviceRectangle;

      if(m_pReplyDlg != NULL &&
         m_pReplyDlg->IsMessage())
      {
         DeviceRectangle.bottom -= m_pReplyDlg->GetHeight();
      }

      int width = DeviceRectangle.right - DeviceRectangle.left;
      int height = DeviceRectangle.bottom - DeviceRectangle.top;

      if(width > 0 && height > 0)
      {
         MakePosLegal();

         bool bDrewNewMessagesTab = false;

         bool bDrewBanner = false;

         if(m_bDrawHotsOnly)
         {
            m_bDrawHotsOnly = false;
            bDrewNewMessagesTab = DrawCurrentHotSpots(hDC);
         }
         else
         {
            m_bForceDrawAll = false;

            if(width != m_backbuffer1.GetWidth() ||
               height != m_backbuffer1.GetHeight())
            {
               m_backbuffer1.Resize(width, height);
               m_backbuffer2.Resize(width, height);
               m_whitebuffer.Resize(width, height);
               m_whitebuffer.Fill(GetRValue(theApp.GetHoverColor()),GetGValue(theApp.GetHoverColor()),GetBValue(theApp.GetHoverColor()));

               int bannerheight = 71;
               CDCSurface *pNTImage = theApp.GetNewThreadImage(false);
               if(pNTImage != NULL)
               {
                  bannerheight = pNTImage->GetHeight();
               }
               m_bannerbuffer.Resize(width, bannerheight);
               m_BannerRectangle.left = m_BannerRectangle.top = 0;
               m_BannerRectangle.right = width;
               m_BannerRectangle.bottom = bannerheight;

               if(m_pReplyDlg != NULL &&
                  m_pReplyDlg->IsMessage())
               {
                  m_replybuffer.Resize(width, m_pReplyDlg->GetHeight());
                  m_whitereplybuffer.Resize(width, m_pReplyDlg->GetHeight());
                  m_whitereplybuffer.Fill(GetRValue(theApp.GetHoverColor()),GetGValue(theApp.GetHoverColor()),GetBValue(theApp.GetHoverColor()));
               }
               
               m_bPanOnly = false;
               theApp.UpdateTabSizes();
            }

            if(m_pos != m_gotopos)
            {
               m_hover_preview_percent = 1.0;
               m_hover_preview_id = 0;

               if(theApp.GetSmoothScroll())
               {
                  int posdiff = (m_gotopos - m_pos);
                  int newposdiff = (int)((float)posdiff * theApp.GetSmoothScrollScale());
                  if(newposdiff < 0)
                  {
                     newposdiff = __max(posdiff, newposdiff);
                  }
                  else if(newposdiff > 0)
                  {
                     newposdiff = __min(posdiff, newposdiff);
                  }
                  else
                  {
                     if(m_gotopos > m_pos)
                     {
                        newposdiff = 1;
                     }
                     else
                     {
                        newposdiff = -1;
                     }
                  }
                  m_pos += newposdiff;
               }
               else
               {
                  m_pos = m_gotopos;
               }
            }

            CDCSurface *pSurface = NULL;
            RECT DocRectangle;
            RECT ScrollRectangle;

            DocRectangle = DeviceRectangle;
            DocRectangle.right -= 16;

            ScrollRectangle = DeviceRectangle;
            ScrollRectangle.left = ScrollRectangle.right - 16;
            
            POINT brushorg;
            brushorg.x = DeviceRectangle.left;
            brushorg.y = DeviceRectangle.top - m_pos;
            
            if(m_pos == m_gotopos &&
               !m_bTrackingThumb &&
               !m_bPanning)
            {
               m_bPanOnly = false;
            }

            if(m_bPanOnly)
            {
               CDCSurface *oldsurf = &m_backbuffer1;
               CDCSurface *newsurf = &m_backbuffer2;

               if(m_backbuffer == &m_backbuffer2)
               {
                  oldsurf = &m_backbuffer2;
                  newsurf = &m_backbuffer1;
               }

               RECT fromrect = DeviceRectangle;
               fromrect.right -= 16;

               RECT torect = DeviceRectangle;
               torect.right -= 16;

               if(m_pos > m_lastdrawnpos)
               {
                  // panned down, shift image up
                  torect.bottom -= (m_pos - m_lastdrawnpos);
                  fromrect.top += (m_pos - m_lastdrawnpos);
                  DocRectangle.top = DeviceRectangle.bottom - (m_pos - m_lastdrawnpos);
               }
               else
               {
                  // panned up, shift image down
                  torect.top += (m_lastdrawnpos - m_pos);
                  fromrect.bottom -= (m_lastdrawnpos - m_pos);
                  DocRectangle.bottom = DeviceRectangle.top + (m_lastdrawnpos - m_pos);
               }

               oldsurf->BlitRect2Rect(newsurf->GetDC(), fromrect, torect);
               //::FillRect(newsurf->GetDC(), &torect,(HBRUSH)::GetStockObject(DKGRAY_BRUSH));

               m_backbuffer = newsurf;

               pSurface = newsurf;
            }
            else
            {
               pSurface = m_backbuffer;
            }

            ::IntersectClipRect(pSurface->GetDC(),DocRectangle.left,DocRectangle.top,DocRectangle.right,DocRectangle.bottom);

            ::SetStretchBltMode(pSurface->GetDC(),HALFTONE);
            ::SetBrushOrgEx(pSurface->GetDC(), brushorg.x, brushorg.y, NULL);

            m_hotspots.clear();

            if(theApp.HaveNewMessages() && theApp.GetAutoCheckInbox())
            {
               const UCChar *pChar;
               int *widths;
               size_t numchars;
               int textwidth;
               int center_x = (DeviceRectangle.left + DeviceRectangle.right) / 2;
               theApp.GetNewMessagesText(&pChar, &widths, numchars, textwidth);

               RECT newmessagesrect;
               newmessagesrect.top = DeviceRectangle.top;
               newmessagesrect.bottom = newmessagesrect.top + theApp.GetTextHeight();
               newmessagesrect.left = center_x - (textwidth / 2) - 5;
               newmessagesrect.right = newmessagesrect.left + textwidth + 10;

               CHotSpot hotspot;
               hotspot.m_bAnim = false;
               hotspot.m_type = HST_NEW_MESSAGES_NOTE;
               hotspot.m_spot = newmessagesrect;
               hotspot.m_id = 0;
               m_hotspots.push_back(hotspot);
            }

            ::SetTextAlign(m_bannerbuffer.GetDC(),TA_LEFT|TA_BOTTOM);
            ::SetBkMode(m_bannerbuffer.GetDC(),TRANSPARENT);

            switch(pDoc->GetDataType())
            {
            case DDT_STORY:
               pDoc->DrawBanner(m_bannerbuffer.GetDC(), m_BannerRectangle, 0, m_hotspots, true, false);
               bDrewBanner = true;
               break;
            case DDT_LOLS:
               pDoc->DrawBanner(m_bannerbuffer.GetDC(), m_BannerRectangle, 0, m_hotspots, false, false);
               bDrewBanner = true;
               break;
            case DDT_SEARCH:
               pDoc->DrawBanner(m_bannerbuffer.GetDC(), m_BannerRectangle, 0, m_hotspots, false, false);
               bDrewBanner = true;
               break;
            case DDT_SHACKMSG:
               pDoc->DrawBanner(m_bannerbuffer.GetDC(), m_BannerRectangle, 0, m_hotspots, false, true);
               bDrewBanner = true;
               break;
            }            

            m_banneroffset = 0;

            if(bDrewBanner)
            {
               m_banneroffset = m_bannerbuffer.GetHeight();
               ScrollRectangle.top += m_bannerbuffer.GetHeight();
            }

            if(m_pReplyDlg != NULL &&
               m_pReplyDlg->IsMessage())
            {
               ::SetBrushOrgEx(m_replybuffer.GetDC(), 0, 0, NULL);
               ::SetTextAlign(m_replybuffer.GetDC(),TA_LEFT|TA_BOTTOM);
               ::SetBkMode(m_replybuffer.GetDC(),TRANSPARENT);
               POINT oldorg;
               SetWindowOrgEx(m_replybuffer.GetDC(),0,OriginalDeviceRectangle.bottom - m_pReplyDlg->GetHeight(),&oldorg);

               m_pReplyDlg->Draw(m_replybuffer.GetDC(),OriginalDeviceRectangle, m_hotspots, m_mousepoint);

               SetWindowOrgEx(m_replybuffer.GetDC(),oldorg.x,oldorg.y,NULL);
            }
            
            DrawEverythingToBuffer(pSurface,&DocRectangle,&ScrollRectangle,false);

            if(m_pReplyDlg != NULL &&
               m_pReplyDlg->IsMessage())
            {
               m_replybuffer.Blit(hDC,m_pReplyDlg->GetDlgRect());
            }

            if(m_pos != m_gotopos)
            {
               InvalidateEverythingPan();
            }
                        
            if(!m_bPanOnly)
            {
               DrawTextSelection(pSurface->GetDC(), DocRectangle);
            }

            if(m_hover_preview_id != 0)
            {
               ChattyPost *pPost = GetDocument()->FindPost(m_hover_preview_id);
               if(pPost != NULL)
               {            
                  UCString rootauthor;
                  if(theApp.GetHighlightOP())
                  {
                     ChattyPost *pRoot = pPost->GetRoot();
                     if(pRoot != NULL)
                     {
                        rootauthor = pRoot->GetAuthor();
                     }
                  }

                  if(bDrewBanner)
                  {
                     DocRectangle.top += m_bannerbuffer.GetHeight();
                  }

                  if(m_hover_preview_percent < 1.0f)
                  {
                     m_hover_preview_percent += theApp.GetHoverPreviewPercentStepsize();
                     if(m_hover_preview_percent > 1.0f)
                     {
                        m_hover_preview_percent = 1.0f;
                     }
                     else
                     {                     
                        InvalidateEverythingPan();
                     }
                  }

                  int bottom;
                  int top = pPost->GetPos();
                  int height = pPost->GetReplyPreviewHeight(DocRectangle);
                  height = theApp.GetTextHeight() + (int)((float)(height - theApp.GetTextHeight()) * m_hover_preview_percent);

                  if(height < (DocRectangle.bottom - DocRectangle.top))
                  {
                     if(theApp.ExpandPreviewsDown())
                     {
                        bottom = top + height;
                     }
                     else
                     {
                        int center = top + (theApp.GetTextHeight() / 2);
                        top = center - (height >> 1);
                        bottom = top + height;
                     }

                     if(top < DocRectangle.top)
                     {
                        top = DocRectangle.top;
                        bottom = top + height;
                     }
                     else if(bottom > DocRectangle.bottom)
                     {
                        bottom = DocRectangle.bottom;;
                        top = bottom - height;
                     }
                  }
                  else
                  {
                     top = DocRectangle.top;
                     bottom = top + height;
                  }

                  pPost->DrawReplyPreview(pSurface->GetDC(), DocRectangle, top, bottom, rootauthor);
                  
                  if(bDrewBanner)
                  {
                     DocRectangle.top -= m_bannerbuffer.GetHeight();
                  }
               }
            }

            ::ExtSelectClipRgn(pSurface->GetDC(),NULL,RGN_COPY);

            if(bDrewBanner)
            {
               DeviceRectangle.top += m_bannerbuffer.GetHeight();
               pSurface->BlitRect2Rect(hDC,DeviceRectangle,DeviceRectangle);
               DeviceRectangle.top -= m_bannerbuffer.GetHeight();
            }
            else
            {
               pSurface->Blit(hDC, DeviceRectangle);
            }

            m_lastdrawnpos = m_pos;
            
            if(m_pos == m_gotopos &&
               m_bPanOnly &&
               !m_bPanning &&
               !m_bTrackingThumb)
            {
               m_hotspots.clear();
               m_hotspot = NULL;
               m_lasthotspot = NULL;
               InvalidateEverything();
            }

            UpdateHotspotPosition();
         }

         //

         if(bDrewBanner)
         {
            ::BitBlt(hDC, m_BannerRectangle.left, m_BannerRectangle.top, m_BannerRectangle.right - m_BannerRectangle.left, m_BannerRectangle.bottom - m_BannerRectangle.top, m_bannerbuffer.GetDC(), 0, 0, SRCCOPY);
         }

         if(!bDrewNewMessagesTab && theApp.HaveNewMessages() && theApp.GetAutoCheckInbox())
         {
            const UCChar *pChar;
            int *widths;
            size_t numchars;
            int textwidth;
            int center_x = (DeviceRectangle.left + DeviceRectangle.right) / 2;
            theApp.GetNewMessagesText(&pChar, &widths, numchars, textwidth);

            RECT newmessagesrect;
            newmessagesrect.top = DeviceRectangle.top;
            newmessagesrect.bottom = newmessagesrect.top + theApp.GetTextHeight();
            newmessagesrect.left = center_x - (textwidth / 2) - 5;
            newmessagesrect.right = newmessagesrect.left + textwidth + 10;

            pDoc->DrawNewMessagesTab(hDC, newmessagesrect, pChar, widths, numchars, false);
         }
         //

         // this is for repeating lbuttondown situations with the scrollbar
         if(m_bLButtonDownOnScrollArrow)
         {
            if(m_mousepoint.x >= m_uprect.left &&
               m_mousepoint.x < m_uprect.right &&
               m_mousepoint.y >= m_uprect.top &&
               m_mousepoint.y < m_uprect.bottom)
            {
               m_gotopos -= 20;
               MakePosLegal();
               InvalidateEverythingPan();
            }
            else if(m_mousepoint.x >= m_downrect.left &&
                    m_mousepoint.x < m_downrect.right &&
                    m_mousepoint.y >= m_downrect.top &&
                    m_mousepoint.y < m_downrect.bottom)
            {
               m_gotopos += 20;
               MakePosLegal();
               InvalidateEverythingPan();
            }
         }
         else if((m_bMButtonDown ||
                  m_bDrawMButtonDownIcon) &&
                 abs(m_MButtonDownPoint.y - m_mousepoint.y) > 13)
         {
            m_bDrawMButtonDownIcon = true;
            int ydiff = m_MButtonDownPoint.y - m_mousepoint.y;

            if(theApp.GetSmoothScroll())
            {
               ydiff = (int)((float)ydiff * theApp.GetMBPanScale());
            }
            else
            {
               ydiff = (int)((float)ydiff / 10.0f);
               ydiff = (int)((float)ydiff * theApp.GetMBPanScale());
            }

            m_gotopos = m_pos - ydiff;
            MakePosLegal();
            InvalidateEverythingPan();
         }
         else if(m_bInertialPanning)
         {
            m_inertia = m_inertia * (1.0f - theApp.GetInertiaFriction());
            m_gotopos = m_pos - (int)m_inertia;
            m_pos = m_gotopos;
            MakePosLegal();
            
            if(m_pos != m_gotopos || // testing whether or not MakePosLegal() hit a wall
               (m_inertia <= 1.0f &&
                m_inertia >= -1.0f))
            {
               m_pos = m_gotopos;
               CancelInertiaPanning();
               m_brakes = false;
               InvalidateEverything();
            }
            else
            {
               InvalidateEverythingPan();
            }
         }

         if(m_bDrawMButtonDownIcon)
         {
            RECT mppanicon;
            mppanicon.left = m_MButtonDownPoint.x - 13;
            mppanicon.top = m_MButtonDownPoint.y - 13;
            mppanicon.right = mppanicon.left + 26;
            mppanicon.bottom = mppanicon.top + 26;
            theApp.GetRMPanIcon()->Blit(hDC, mppanicon);
         }
      }
   }
}

void CLampView::DrawEverythingToBuffer(CDCSurface *pSurface/* = NULL*/, 
                                       RECT *pDeviceRectangle/* = NULL*/, 
                                       RECT *pScrollRectangle/* = NULL*/,
                                       bool bClearHotspots/* = true*/)
{
   if(pSurface == NULL)
   {
      pSurface = m_backbuffer;
      ::SetBrushOrgEx(pSurface->GetDC(), 0, -m_pos, NULL);
   }
   // setup some basic drawing assumptions
   ::SetTextAlign(pSurface->GetDC(),TA_LEFT|TA_BOTTOM);
   ::SetBkMode(pSurface->GetDC(),TRANSPARENT);
   
   RECT DeviceRectangle;

   if(pDeviceRectangle != NULL)
   {
      DeviceRectangle = *pDeviceRectangle;
   }
   else
   {
      DeviceRectangle.left = DeviceRectangle.top = 0;
      DeviceRectangle.right = pSurface->GetWidth() - 16;
      DeviceRectangle.bottom = pSurface->GetHeight();
   }

   if(bClearHotspots)
      m_hotspots.clear();

   m_hotspot = NULL;
   m_lasthotspot = NULL;

   if(DeviceRectangle.bottom > DeviceRectangle.top)
   {
      int device_height = pSurface->GetHeight();
      if(m_pReplyDlg != NULL &&
         m_pReplyDlg->IsMessage())
      {
         device_height += m_pReplyDlg->GetHeight();
      }

      GetDocument()->Draw(pSurface->GetDC(), device_height,DeviceRectangle, m_pos, m_hotspots, GetCurrentId(), m_bModToolIsUp, m_ModToolRect, m_ModToolPostID);
   
      if(m_pReplyDlg != NULL &&
         !m_pReplyDlg->IsMessage())
      {
         m_pReplyDlg->Draw(pSurface->GetDC(),DeviceRectangle, m_hotspots, m_mousepoint);
      }
   }

   if(pScrollRectangle != NULL)
   {
      m_ScrollRectangle = *pScrollRectangle;
   }
   else
   {
      m_ScrollRectangle = DeviceRectangle;
      m_ScrollRectangle.left = m_ScrollRectangle.right;
      m_ScrollRectangle.right += 16;
   }

   ::ExtSelectClipRgn(pSurface->GetDC(),NULL,RGN_COPY);

   DrawScrollbar(pSurface->GetDC(), m_ScrollRectangle, m_hotspots);

   ::IntersectClipRect(pSurface->GetDC(),DeviceRectangle.left,DeviceRectangle.top,DeviceRectangle.right,DeviceRectangle.bottom);

   if(m_pReplyDlg != NULL)
   {
      m_pReplyDlg->TweakHotspots(m_hotspots);
   }

   DrawHotSpots(pSurface->GetDC());

   if(m_bModToolIsUp)
   {
      m_hotspots.clear();
      if(!(m_ModToolRect.top == -1 && m_ModToolRect.bottom == -1))
      {
         GetDocument()->FillBackground(pSurface->GetDC(), m_ModToolRect);

         CHotSpot hotspot;
         hotspot.m_type = HST_MOD_TOOL_ITEM;
         hotspot.m_bAnim = false;
         hotspot.m_id = 0;
         hotspot.m_spot = m_ModToolRect;
         int itemheight = (m_ModToolRect.bottom - m_ModToolRect.top) / 7;
         hotspot.m_spot.bottom = hotspot.m_spot.top + itemheight;

         hotspot.m_cat_type = PCT_NORMAL;
         m_hotspots.push_back(hotspot);
         hotspot.m_spot.top += itemheight;
         hotspot.m_spot.bottom += itemheight;

         hotspot.m_cat_type = PCT_OFFTOPIC;
         m_hotspots.push_back(hotspot);
         hotspot.m_spot.top += itemheight;
         hotspot.m_spot.bottom += itemheight;

         hotspot.m_cat_type = PCT_INF;
         m_hotspots.push_back(hotspot);
         hotspot.m_spot.top += itemheight;
         hotspot.m_spot.bottom += itemheight;

         hotspot.m_cat_type = PCT_NWS;
         m_hotspots.push_back(hotspot);
         hotspot.m_spot.top += itemheight;
         hotspot.m_spot.bottom += itemheight;

         hotspot.m_cat_type = PCT_STUPID;
         m_hotspots.push_back(hotspot);
         hotspot.m_spot.top += itemheight;
         hotspot.m_spot.bottom += itemheight;

         hotspot.m_cat_type = PCT_POLITICAL;
         m_hotspots.push_back(hotspot);
         hotspot.m_spot.top += itemheight;
         hotspot.m_spot.bottom += itemheight;

         hotspot.m_cat_type = PCT_NUKED;
         m_hotspots.push_back(hotspot);

         DrawHotSpots(pSurface->GetDC());
      }
   }
}

void CLampView::DrawScrollbar(HDC hDC, const RECT &ScrollRectangle, std::vector<CHotSpot> &hotspots)
{
   scrollbitype maintype = SBT_INACTIVE;
   scrollbitype thistype;
   if(m_bTrackingThumb ||
      (m_mousepoint.x >= ScrollRectangle.left &&
       m_mousepoint.x < ScrollRectangle.right &&
       m_mousepoint.y >= ScrollRectangle.top &&
       m_mousepoint.y < ScrollRectangle.bottom))
   {
      maintype = SBT_ACTIVE;
   }

   m_uprect = ScrollRectangle;
   m_uprect.bottom = m_uprect.top + 16;
   thistype = maintype;
   if(thistype == SBT_ACTIVE &&
      m_mousepoint.x >= m_uprect.left &&
      m_mousepoint.x < m_uprect.right &&
      m_mousepoint.y >= m_uprect.top &&
      m_mousepoint.y < m_uprect.bottom)
   {
      thistype = SBT_HOVER;
   }
   theApp.GetUpImage(thistype)->Blit(hDC,m_uprect);

   CHotSpot hotspot;
   hotspot.m_bAnim = false;
   hotspot.m_type = HST_SCROLLBAR_UP;
   hotspot.m_spot = m_uprect;
   hotspot.m_id = 0;
   hotspots.push_back(hotspot);

   m_downrect = ScrollRectangle;
   m_downrect.top = m_downrect.bottom - 16;
   thistype = maintype;
   if(thistype == SBT_ACTIVE &&
      m_mousepoint.x >= m_downrect.left &&
      m_mousepoint.x < m_downrect.right &&
      m_mousepoint.y >= m_downrect.top &&
      m_mousepoint.y < m_downrect.bottom)
   {
      thistype = SBT_HOVER;
   }
   theApp.GetDownImage(thistype)->Blit(hDC,m_downrect);

   hotspot.m_type = HST_SCROLLBAR_DOWN;
   hotspot.m_spot = m_downrect;
   hotspot.m_id = 0;
   hotspots.push_back(hotspot);

   int scrollbarheight = ScrollRectangle.bottom - ScrollRectangle.top;
   int trackheight = scrollbarheight - 16 - 16;
   int docheight = GetDocument()->GetHeight();
   int thumbtop = m_pos;
   int screenheight = scrollbarheight;
   if(m_pReplyDlg != NULL &&
      m_pReplyDlg->IsMessage())
   {
      screenheight += m_pReplyDlg->GetHeight();
   }
   int thumbbottom = thumbtop + screenheight + ScrollRectangle.top;//scrollbarheight is also screenheight

   // now translate those to the range of the scrollbar's pixels
   m_scrollscale =  (float)trackheight / (float)docheight;
   thumbtop = ScrollRectangle.top + __max(16, 16 + (int)((float)thumbtop * m_scrollscale));
   thumbbottom = ScrollRectangle.top + __min(scrollbarheight - 16, 16 + (int)((float)thumbbottom * m_scrollscale));
   int thumbheight = thumbbottom - thumbtop;
   
   if(docheight > 0 && 
      thumbheight < (4 + 4))
   {
      // too small, bump it up
      int thumbcenter = (thumbtop + thumbbottom) >> 1;
      thumbtop = thumbcenter - 4;
      thumbbottom = thumbtop + 8;
      thumbheight = thumbbottom - thumbtop;
      
      if(thumbtop < 16)
      {
         int off = 16 - thumbtop;
         thumbtop+=off;
         thumbbottom+=off;
      }

      if(thumbbottom > scrollbarheight - 16)
      {
         int off = thumbbottom - (scrollbarheight - 16);
         thumbtop-=off;
         thumbbottom-=off;
      }
   }

   m_bdrawthumb = false;
   // draw thumb
   if(docheight > 0 && 
      thumbbottom - thumbtop < trackheight)
   {
      m_bdrawthumb = true;
      m_thumbrect = ScrollRectangle;
      m_thumbrect.top = thumbtop;
      m_thumbrect.bottom = thumbbottom;

      thistype = maintype;
      if(thistype == SBT_ACTIVE &&
         m_mousepoint.x >= m_thumbrect.left &&
         m_mousepoint.x < m_thumbrect.right &&
         m_mousepoint.y >= m_thumbrect.top &&
         m_mousepoint.y < m_thumbrect.bottom)
      {
         thistype = SBT_HOVER;
      }

      if(m_bTrackingThumb)
      {
         thistype = SBT_HOVER;
      }

      DrawThumb(hDC, m_thumbrect, thistype);

      hotspot.m_type = HST_SCROLLBAR_THUMB;
      hotspot.m_spot = m_thumbrect;
      hotspot.m_id = 0;
      hotspots.push_back(hotspot);

      // draw tracks
      if(m_thumbrect.top > m_uprect.bottom)
      {
         m_uptrackrect = m_thumbrect;
         m_uptrackrect.top = m_uprect.bottom;
         m_uptrackrect.bottom = m_thumbrect.top;
         theApp.GetTrackImage()->Fill(hDC,m_uptrackrect);
      }

      if(m_thumbrect.bottom < m_downrect.top)
      {
         m_downtrackrect = m_thumbrect;
         m_downtrackrect.top = m_thumbrect.bottom;
         m_downtrackrect.bottom = m_downrect.top;
         theApp.GetTrackImage()->Fill(hDC,m_downtrackrect);
      }
   }
   else
   {
      memset(&m_uptrackrect,0,sizeof(RECT));
      memset(&m_downtrackrect,0,sizeof(RECT));
      if(m_downrect.top > m_uprect.bottom)
      {
         RECT trackrect = ScrollRectangle;
         trackrect.top = m_uprect.bottom;
         trackrect.bottom = m_downrect.top;
         theApp.GetTrackImage()->Fill(hDC,trackrect);
      }
   }

   hotspot.m_type = HST_SCROLLBAR;
   hotspot.m_spot = ScrollRectangle;
   hotspot.m_id = 0;
   hotspots.push_back(hotspot);
}

void CLampView::DrawThumb(HDC hDC, const RECT &thumbrect, scrollbitype type)
{
   if(m_bdrawthumb)
   {
      RECT thumbtoprect = thumbrect;
      thumbtoprect.bottom = thumbrect.top + 4;
      theApp.GetThumbTopImage(type)->Blit(hDC,thumbtoprect);

      RECT thumbbottomrect = thumbrect;
      thumbbottomrect.top = thumbrect.bottom - 4;
      thumbbottomrect.bottom = thumbrect.bottom;
      theApp.GetThumbBottomImage(type)->Blit(hDC,thumbbottomrect);

      bool bShowGrip = false;
      if(thumbrect.bottom - thumbrect.top > (4 + 4 + 10))
      {
         bShowGrip = true;
      }

      if(bShowGrip)
      {
         int thumbcenter = (thumbrect.top + thumbrect.bottom) >> 1;
         RECT thumbgriprect = thumbrect;
         thumbgriprect.top = thumbcenter - 5;
         thumbgriprect.bottom = thumbgriprect.top + 10;
         theApp.GetThumbGripImage(type)->Blit(hDC,thumbgriprect);

         RECT thumbuppermrect = thumbrect;
         thumbuppermrect.top = thumbtoprect.bottom;
         thumbuppermrect.bottom = thumbgriprect.top;
         theApp.GetThumbImage(type)->Fill(hDC,thumbuppermrect);

         RECT thumblowermrect = thumbrect;
         thumblowermrect.top = thumbgriprect.bottom;
         thumblowermrect.bottom = thumbbottomrect.top;
         theApp.GetThumbImage(type)->Fill(hDC,thumblowermrect);
      }
      else
      {
         RECT thumbmidrect = thumbrect;
         thumbmidrect.top = thumbtoprect.bottom;
         thumbmidrect.bottom = thumbbottomrect.top;
         theApp.GetThumbImage(type)->Fill(hDC,thumbmidrect);
      }
   }
}

void CLampView::DrawTextSelection(HDC hDC, const RECT &DeviceRectangle)
{
   if(m_textselectionpost != 0 &&
      m_selectionstart != m_selectionend)
   {
      ChattyPost *pPost = GetDocument()->FindPost(m_textselectionpost);
      if(pPost != NULL)
      {
         int oldrop = ::GetROP2(hDC);
         ::SetROP2(hDC,R2_XORPEN);
         HBRUSH selbrush = ::CreateSolidBrush(theApp.GetTextSelectionColor());
         std::vector<RECT> selectionrects;
         pPost->GetTextSelectionRects(m_selectionstart,m_selectionend,selectionrects);

         for(size_t i = 0; i < selectionrects.size(); i++)
         {
            if(selectionrects[i].bottom < DeviceRectangle.top ||
               selectionrects[i].top > DeviceRectangle.bottom)
            {
               // skip
            }
            else
            {
               HRGN hRGN = ::CreateRectRgnIndirect(&(selectionrects[i]));
               ::FillRgn(hDC, hRGN, selbrush);
               ::DeleteObject(hRGN);
            }
         }         
         ::DeleteObject(selbrush);
         ::SetROP2(hDC,oldrop);
      }
   }
}

void CLampView::DrawHotSpots(HDC hDC)
{
   m_bAnimating = false;

   POINT oldorg;

   if(m_pReplyDlg != NULL &&
      m_pReplyDlg->IsMessage())
   {
      RECT OriginalDeviceRectangle;
      GetClientRect(&OriginalDeviceRectangle);
      SetWindowOrgEx(m_replybuffer.GetDC(),0,OriginalDeviceRectangle.bottom - m_pReplyDlg->GetHeight(),&oldorg);
   }

   for(size_t i = 0; i < m_hotspots.size(); i++)
   {
      if(m_hotspots[i].m_bAnim)
      {
         m_bAnimating = true;
      }

      bool hover = false;

      if(m_mousepoint.x >= m_hotspots[i].m_spot.left &&
         m_mousepoint.x < m_hotspots[i].m_spot.right &&
         m_mousepoint.y >= m_hotspots[i].m_spot.top &&
         m_mousepoint.y < m_hotspots[i].m_spot.bottom)
      {
         hover = true;
      }

      switch(m_hotspots[i].m_type)
      {
      case HST_REFRESH: 
         {
            theApp.GetRefreshBuffer(hover,m_hotspots[i].m_bAnim, m_frame)->Blit(hDC,m_hotspots[i].m_spot);
         }
         break;
      case HST_CLOSEREPLY: 
      case HST_COLLAPSEPOST:
         {
            theApp.GetCloseImage(hover)->Blit(hDC,m_hotspots[i].m_spot);
         }
         break;
      case HST_OPENINTAB:
         {
            theApp.GetTabBuffer(hover)->Blit(hDC,m_hotspots[i].m_spot);
         }
         break;
      case HST_PIN:
         {
            theApp.GetPinImage(m_hotspots[i].m_bOn,hover)->Blit(hDC,m_hotspots[i].m_spot);
         }
         break;
      case HST_MOD_TOOL:
         {
            switch(m_hotspots[i].m_cat_type)
            {
            case PCT_NORMAL:        theApp.GetOnTopicImage(hover)->Blit(hDC,m_hotspots[i].m_spot);break;
            case PCT_INF:           theApp.GetINFImage(hover)->Blit(hDC,m_hotspots[i].m_spot);break;
            case PCT_NWS:           theApp.GetNWSImage(hover)->Blit(hDC,m_hotspots[i].m_spot);break;
            case PCT_STUPID:        theApp.GetStupidImage(hover)->Blit(hDC,m_hotspots[i].m_spot);break;
            case PCT_OFFTOPIC:      theApp.GetOffTopicImage(hover)->Blit(hDC,m_hotspots[i].m_spot);break;
            case PCT_POLITICAL:     theApp.GetPoliticalImage(hover)->Blit(hDC,m_hotspots[i].m_spot);break;
            case PCT_NUKED:         theApp.GetNukedImage(hover)->Blit(hDC,m_hotspots[i].m_spot);break;
            }
         }
         break;
      case HST_MOD_TOOL_ITEM:
         {
            switch(m_hotspots[i].m_cat_type)
            {
            case PCT_NORMAL:        theApp.GetOnTopicImage(hover)->Blit(hDC,m_hotspots[i].m_spot);break;
            case PCT_INF:           theApp.GetINFImage(hover)->Blit(hDC,m_hotspots[i].m_spot);break;
            case PCT_NWS:           theApp.GetNWSImage(hover)->Blit(hDC,m_hotspots[i].m_spot);break;
            case PCT_STUPID:        theApp.GetStupidImage(hover)->Blit(hDC,m_hotspots[i].m_spot);break;
            case PCT_OFFTOPIC:      theApp.GetOffTopicImage(hover)->Blit(hDC,m_hotspots[i].m_spot);break;
            case PCT_POLITICAL:     theApp.GetPoliticalImage(hover)->Blit(hDC,m_hotspots[i].m_spot);break;
            case PCT_NUKED:         theApp.GetNukedImage(hover)->Blit(hDC,m_hotspots[i].m_spot);break;
            }
         }
         break;
      case HST_LIGHTNINGBOLT:
         {
            theApp.GetLightningBoltImage(hover)->Blit(hDC,m_hotspots[i].m_spot);
         }
         break;
      case HST_CREATEREPLY:
      case HST_REPLY_TO_MESSAGE:
         {
            theApp.GetReplyBuffer(hover)->Blit(hDC,m_hotspots[i].m_spot);
         }
         break;
      case HST_FORWARD_MESSAGE:
         {
            theApp.GetForwardImage(hover)->Blit(hDC,m_hotspots[i].m_spot);
         }
         break;
      case HST_DELETE_MESSAGE:
         {
            theApp.GetDeleteImage(hover)->Blit(hDC,m_hotspots[i].m_spot);
         }
         break;
      case HST_CLOSEREPLYDLG:
         {
            if(m_pReplyDlg != NULL &&
               m_pReplyDlg->IsMessage())
            {
               theApp.GetCloseImage(hover)->Blit(m_replybuffer.GetDC(),m_hotspots[i].m_spot);
            }
            else
            {
               theApp.GetCloseImage(hover)->Blit(hDC,m_hotspots[i].m_spot);
            }
         }
         break;
      case HST_POST:
         {
            if(m_pReplyDlg != NULL &&
               m_pReplyDlg->GetHasFocus() &&
               m_pReplyDlg->LastCharWasTab())
            {
               hover = true;
            }
            theApp.GetPostImage(hover)->Blit(hDC,m_hotspots[i].m_spot);
         }
         break;
      case HST_SEND:
         {
            if(m_pReplyDlg != NULL &&
               m_pReplyDlg->IsMessage())
            {
               if(m_pReplyDlg->GetHasFocus() &&
                  m_pReplyDlg->LastCharWasTab())
               {
                  hover = true;
               }
               theApp.GetSendImage(hover)->Blit(m_replybuffer.GetDC(),m_hotspots[i].m_spot);
            }
         }
         break;
      case HST_PREVIEW:
         {
            if(m_pReplyDlg != NULL &&
               m_pReplyDlg->IsMessage())
            {
               theApp.GetPreviewImage(hover)->Blit(m_replybuffer.GetDC(),m_hotspots[i].m_spot);
            }
            else
            {
               theApp.GetPreviewImage(hover)->Blit(hDC,m_hotspots[i].m_spot);
            }
         }
         break;
      case HST_UNPREVIEW:
         {
            if(m_pReplyDlg != NULL &&
               m_pReplyDlg->IsMessage())
            {
               theApp.GetPreviewImage(!hover)->Blit(m_replybuffer.GetDC(),m_hotspots[i].m_spot);
            }
            else
            {
               theApp.GetPreviewImage(!hover)->Blit(hDC,m_hotspots[i].m_spot);
            }
         }
         break;
      }
   }

   if(m_pReplyDlg != NULL &&
      m_pReplyDlg->IsMessage())
   {
      SetWindowOrgEx(m_replybuffer.GetDC(),oldorg.x,oldorg.y,NULL);
   }
}

bool CLampView::DrawCurrentHotSpots(HDC hDC)
{
   RECT DeviceRectangle;
   GetClientRect(&DeviceRectangle);

   ::IntersectClipRect(hDC,DeviceRectangle.left,DeviceRectangle.top + m_banneroffset,DeviceRectangle.right,DeviceRectangle.bottom);

   bool bDrewNewMessagesTab = false;
   if(m_lasthotspot != NULL)
   {
      // un-hover the last one
      for(size_t i = 0; i < m_hotspots.size(); i++)
      {
         if(m_lasthotspot->left == m_hotspots[i].m_spot.left &&
            m_lasthotspot->right == m_hotspots[i].m_spot.right &&
            m_lasthotspot->top == m_hotspots[i].m_spot.top &&
            m_lasthotspot->bottom == m_hotspots[i].m_spot.bottom)
         {
            CDCSurface *surf = NULL;
            switch(m_hotspots[i].m_type)
            {
            case HST_SCROLLBAR_UP:
            case HST_SCROLLBAR_DOWN:
            case HST_SCROLLBAR_THUMB:
            case HST_SCROLLBAR:
               {
                  DrawThumb(hDC,m_thumbrect,SBT_INACTIVE);
                  theApp.GetUpImage(SBT_INACTIVE)->Blit(hDC,m_uprect);
                  theApp.GetDownImage(SBT_INACTIVE)->Blit(hDC,m_downrect);
               }
               break;
            case HST_SCROLLBAR_UP_REPLYTEXT_DLG:
            case HST_SCROLLBAR_DOWN_REPLYTEXT_DLG:
            case HST_SCROLLBAR_THUMB_REPLYTEXT_DLG:
            case HST_SCROLLBAR_REPLYTEXT_DLG:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawThumb(hDC,m_pReplyDlg->GetThumbRect(),SBT_INACTIVE);
                     theApp.GetUpImage(SBT_INACTIVE)->Blit(hDC,m_pReplyDlg->GetUpRect());
                     theApp.GetDownImage(SBT_INACTIVE)->Blit(hDC,m_pReplyDlg->GetDownRect());
                  }
               }
               break;
            case HST_REFRESH:
               {
                  theApp.GetRefreshBuffer(false,m_hotspots[i].m_bAnim, m_frame)->Blit(hDC,m_hotspots[i].m_spot);
               }
               break;
            case HST_CLOSEREPLY: 
            case HST_COLLAPSEPOST:
               {
                  theApp.GetCloseImage(false)->Blit(hDC,m_hotspots[i].m_spot);
               }
               break;
            case HST_OPENINTAB:
               {
                  theApp.GetTabBuffer(false)->Blit(hDC,m_hotspots[i].m_spot);
               }
               break;
            case HST_PIN:
               {
                  theApp.GetPinImage(m_hotspots[i].m_bOn,false)->Blit(hDC,m_hotspots[i].m_spot);
               }
               break;
            case HST_MOD_TOOL:
               {
                  switch(m_hotspots[i].m_cat_type)
                  {
                  case PCT_NORMAL:        theApp.GetOnTopicImage(false)->Blit(hDC,m_hotspots[i].m_spot);break;
                  case PCT_INF:           theApp.GetINFImage(false)->Blit(hDC,m_hotspots[i].m_spot);break;
                  case PCT_NWS:           theApp.GetNWSImage(false)->Blit(hDC,m_hotspots[i].m_spot);break;
                  case PCT_STUPID:        theApp.GetStupidImage(false)->Blit(hDC,m_hotspots[i].m_spot);break;
                  case PCT_OFFTOPIC:      theApp.GetOffTopicImage(false)->Blit(hDC,m_hotspots[i].m_spot);break;
                  case PCT_POLITICAL:     theApp.GetPoliticalImage(false)->Blit(hDC,m_hotspots[i].m_spot);break;
                  case PCT_NUKED:         theApp.GetNukedImage(false)->Blit(hDC,m_hotspots[i].m_spot);break;
                  }
               }
               break;
            case HST_MOD_TOOL_ITEM:
               {
                  switch(m_hotspots[i].m_cat_type)
                  {
                  case PCT_NORMAL:        theApp.GetOnTopicImage(false)->Blit(hDC,m_hotspots[i].m_spot);break;
                  case PCT_INF:           theApp.GetINFImage(false)->Blit(hDC,m_hotspots[i].m_spot);break;
                  case PCT_NWS:           theApp.GetNWSImage(false)->Blit(hDC,m_hotspots[i].m_spot);break;
                  case PCT_STUPID:        theApp.GetStupidImage(false)->Blit(hDC,m_hotspots[i].m_spot);break;
                  case PCT_OFFTOPIC:      theApp.GetOffTopicImage(false)->Blit(hDC,m_hotspots[i].m_spot);break;
                  case PCT_POLITICAL:     theApp.GetPoliticalImage(false)->Blit(hDC,m_hotspots[i].m_spot);break;
                  case PCT_NUKED:         theApp.GetNukedImage(false)->Blit(hDC,m_hotspots[i].m_spot);break;
                  }
               }
               break;
            case HST_LIGHTNINGBOLT:
               {
                  theApp.GetLightningBoltImage(false)->Blit(hDC,m_hotspots[i].m_spot);
               }
               break;
            case HST_NEWTHREAD:
               {
                  ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);
                  theApp.GetNewThreadImage(false)->Blit(hDC,m_hotspots[i].m_spot,true,20,20);
                  ::IntersectClipRect(hDC,DeviceRectangle.left,DeviceRectangle.top + m_banneroffset,DeviceRectangle.right,DeviceRectangle.bottom);
               }
               break;
            case HST_COMPOSE_MESSAGE:
               {
                  ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);
                  theApp.GetComposeImage(false)->Blit(hDC,m_hotspots[i].m_spot,true,20,20);
                  ::IntersectClipRect(hDC,DeviceRectangle.left,DeviceRectangle.top + m_banneroffset,DeviceRectangle.right,DeviceRectangle.bottom);
               }
               break;
            case HST_REFRESHSTORY:
               {
                  ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);
                  theApp.GetRefreshStoryImage(false)->Blit(hDC,m_hotspots[i].m_spot,true,34,20);
                  ::IntersectClipRect(hDC,DeviceRectangle.left,DeviceRectangle.top + m_banneroffset,DeviceRectangle.right,DeviceRectangle.bottom);
               }
               break;
            case HST_REPLYPREVIEW:
               {
                  m_backbuffer->Blit(hDC, m_hotspots[i].m_spot, false);
               }
               break;
            case HST_REPLIESTOROOTPOSTHINT:
            case HST_POSTAS:
            case HST_AUTHOR:
            case HST_AUTHORPREVIEW:
            case HST_LINK:
            case HST_IMAGE_LINK:
            case HST_SPELL_SUGGESTION:
            case HST_CLOSE_MESSAGE:
            case HST_EXPAND:
               {
                  m_backbuffer->Blit(hDC, m_hotspots[i].m_spot, false);
               }
               break;
            case HST_MSG_INFO:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_replybuffer.Blit(hDC, m_hotspots[i].m_spot, false, 0, -(DeviceRectangle.bottom - m_pReplyDlg->GetHeight()));
                  }
               }
               break;
            case HST_PREV_PAGE:
            case HST_NEXT_PAGE:
            case HST_PAGE:
               {
                  ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);
                  m_bannerbuffer.Blit(hDC, m_hotspots[i].m_spot, false);
                  ::IntersectClipRect(hDC,DeviceRectangle.left,DeviceRectangle.top + m_banneroffset,DeviceRectangle.right,DeviceRectangle.bottom);
               }
               break;
            case HST_NEW_MESSAGES_NOTE:
               {
                  const UCChar *pChar;
                  int *widths;
                  size_t numchars;
                  int textwidth;
                  int center_x = (DeviceRectangle.left + DeviceRectangle.right) / 2;
                  theApp.GetNewMessagesText(&pChar, &widths, numchars, textwidth);

                  RECT newmessagesrect;
                  newmessagesrect.top = DeviceRectangle.top;
                  newmessagesrect.bottom = newmessagesrect.top + theApp.GetTextHeight();
                  newmessagesrect.left = center_x - (textwidth / 2) - 5;
                  newmessagesrect.right = newmessagesrect.left + textwidth + 10;

                  GetDocument()->DrawNewMessagesTab(hDC, newmessagesrect, pChar, widths, numchars, false);
                  bDrewNewMessagesTab = true;
               }
               break;
            case HST_CREATEREPLY:
            case HST_REPLY_TO_MESSAGE:
               {
                  theApp.GetReplyBuffer(false)->Blit(hDC,m_hotspots[i].m_spot);
               }
               break;
            case HST_FORWARD_MESSAGE:
               {
                  theApp.GetForwardImage(false)->Blit(hDC,m_hotspots[i].m_spot);
               }
               break;
            case HST_DELETE_MESSAGE:
               {
                  theApp.GetDeleteImage(false)->Blit(hDC,m_hotspots[i].m_spot);
               }
               break;
            case HST_CLOSEREPLYDLG:
               {
                  theApp.GetCloseImage(false)->Blit(hDC,m_hotspots[i].m_spot);
               }
               break;
            case HST_POST:
               {
                  theApp.GetPostImage(false)->Blit(hDC,m_hotspots[i].m_spot);
               }
               break;
            case HST_SEND:
               {
                  theApp.GetSendImage(false)->Blit(hDC,m_hotspots[i].m_spot);
               }
               break;
            case HST_PREVIEW:
               {
                  theApp.GetPreviewImage(false)->Blit(hDC,m_hotspots[i].m_spot);
               }
               break;
            case HST_UNPREVIEW:
               {
                  theApp.GetPreviewImage(true)->Blit(hDC,m_hotspots[i].m_spot);
               }
               break;
            case HST_TAG_RED:
               {
                  theApp.GetTagsImage(false)->Blit(hDC,m_hotspots[i].m_spot,true,0,40);
               }
               break;
            case HST_TAG_GREEN:
               {
                  theApp.GetTagsImage(false)->Blit(hDC,m_hotspots[i].m_spot,true,0,66);
               }
               break;
            case HST_TAG_BLUE:
               {
                  theApp.GetTagsImage(false)->Blit(hDC,m_hotspots[i].m_spot,true,0,92);
               }
               break;
            case HST_TAG_YELLOW:
               {
                  theApp.GetTagsImage(false)->Blit(hDC,m_hotspots[i].m_spot,true,0,118);
               }
               break;
            case HST_TAG_OLIVE:
               {
                  theApp.GetTagsImage(false)->Blit(hDC,m_hotspots[i].m_spot,true,0,144);
               }
               break;
            case HST_TAG_LIME:
               {
                  theApp.GetTagsImage(false)->Blit(hDC,m_hotspots[i].m_spot,true,0,170);
               }
               break;
            case HST_TAG_ORANGE:
               {
                  theApp.GetTagsImage(false)->Blit(hDC,m_hotspots[i].m_spot,true,0,196);
               }
               break;
            case HST_TAG_PINK:
               {
                  theApp.GetTagsImage(false)->Blit(hDC,m_hotspots[i].m_spot,true,0,222);
               }
               break;
            case HST_TAG_ITALICS:
               {
                  theApp.GetTagsImage(false)->Blit(hDC,m_hotspots[i].m_spot,true,67,40);
               }
               break;
            case HST_TAG_BOLD:
               {
                  theApp.GetTagsImage(false)->Blit(hDC,m_hotspots[i].m_spot,true,67,66);
               }
               break;
            case HST_TAG_QUOTE:
               {
                  theApp.GetTagsImage(false)->Blit(hDC,m_hotspots[i].m_spot,true,67,92);
               }
               break;
            case HST_TAG_SAMPLE:
               {
                  theApp.GetTagsImage(false)->Blit(hDC,m_hotspots[i].m_spot,true,67,118);
               }
               break;
            case HST_TAG_UNDERLINE:
               {
                  theApp.GetTagsImage(false)->Blit(hDC,m_hotspots[i].m_spot,true,67,144);
               }
               break;
            case HST_TAG_STRIKE:
               {
                  theApp.GetTagsImage(false)->Blit(hDC,m_hotspots[i].m_spot,true,67,170);
               }
               break;
            case HST_TAG_SPOILER:
               {
                  theApp.GetTagsImage(false)->Blit(hDC,m_hotspots[i].m_spot,true,67,196);
               }
               break;
            case HST_TAG_CODE:
               {
                  theApp.GetTagsImage(false)->Blit(hDC,m_hotspots[i].m_spot,true,67,222);
               }
               break;
            case HST_LOLTAG:
               {
                  GetDocument()->DrawLOLField(hDC, LTT_LOL, m_hotspots[i].m_spot, m_hotspots[i].m_loltext, false, m_hotspots[i].m_lolvoted, m_hotspots[i].m_lolroot, m_hotspots[i].m_haslols);
               }
               break;
            case HST_INFTAG:
               {
                  GetDocument()->DrawLOLField(hDC, LTT_INF, m_hotspots[i].m_spot, m_hotspots[i].m_loltext, false, m_hotspots[i].m_lolvoted, m_hotspots[i].m_lolroot, m_hotspots[i].m_haslols);
               }
               break;
            case HST_UNFTAG:
               {
                  GetDocument()->DrawLOLField(hDC, LTT_UNF, m_hotspots[i].m_spot, m_hotspots[i].m_loltext, false, m_hotspots[i].m_lolvoted, m_hotspots[i].m_lolroot, m_hotspots[i].m_haslols);
               }
               break;
            case HST_TAGTAG:
               {
                  GetDocument()->DrawLOLField(hDC, LTT_TAG, m_hotspots[i].m_spot, m_hotspots[i].m_loltext, false, m_hotspots[i].m_lolvoted, m_hotspots[i].m_lolroot, m_hotspots[i].m_haslols);
               }
               break;
            case HST_WTFTAG:
               {
                  GetDocument()->DrawLOLField(hDC, LTT_WTF, m_hotspots[i].m_spot, m_hotspots[i].m_loltext, false, m_hotspots[i].m_lolvoted, m_hotspots[i].m_lolroot, m_hotspots[i].m_haslols);
               }
               break;
            case HST_NULL_BACKGROUND:
               {
                  /* 
                  do nothing.
                  just eat the hotspot to keep others from processing further.
                  */
               }
               break;
            }
            break;
         }
      }
   }

   if(m_hotspot != NULL)
   {
      // hover the new one
      for(size_t i = 0; i < m_hotspots.size(); i++)
      {
         if(m_hotspot->left == m_hotspots[i].m_spot.left &&
            m_hotspot->right == m_hotspots[i].m_spot.right &&
            m_hotspot->top == m_hotspots[i].m_spot.top &&
            m_hotspot->bottom == m_hotspots[i].m_spot.bottom)
         {
            CDCSurface *surf = NULL;
            switch(m_hotspots[i].m_type)
            {
            case HST_SCROLLBAR_UP:
               {
                  theApp.GetUpImage(SBT_HOVER)->Blit(hDC,m_hotspots[i].m_spot);
                  DrawThumb(hDC,m_thumbrect,SBT_ACTIVE);
                  theApp.GetDownImage(SBT_ACTIVE)->Blit(hDC,m_downrect);
               }
               break;
            case HST_SCROLLBAR_DOWN:
               {
                  theApp.GetDownImage(SBT_HOVER)->Blit(hDC,m_hotspots[i].m_spot);
                  DrawThumb(hDC,m_thumbrect,SBT_ACTIVE);
                  theApp.GetUpImage(SBT_ACTIVE)->Blit(hDC,m_uprect);
               }
               break;
            case HST_SCROLLBAR_THUMB:
               {
                  DrawThumb(hDC,m_hotspots[i].m_spot,SBT_HOVER);
                  theApp.GetUpImage(SBT_ACTIVE)->Blit(hDC,m_uprect);
                  theApp.GetDownImage(SBT_ACTIVE)->Blit(hDC,m_downrect);
               }
               break;
            case HST_SCROLLBAR:
               {
                  DrawThumb(hDC,m_thumbrect,SBT_ACTIVE);
                  theApp.GetUpImage(SBT_ACTIVE)->Blit(hDC,m_uprect);
                  theApp.GetDownImage(SBT_ACTIVE)->Blit(hDC,m_downrect);
               }
               break;


            case HST_SCROLLBAR_UP_REPLYTEXT_DLG:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     theApp.GetUpImage(SBT_HOVER)->Blit(hDC,m_hotspots[i].m_spot);
                     m_pReplyDlg->DrawThumb(hDC,m_pReplyDlg->GetThumbRect(),SBT_ACTIVE);
                     theApp.GetDownImage(SBT_ACTIVE)->Blit(hDC,m_pReplyDlg->GetDownRect());
                  }
               }
               break;
            case HST_SCROLLBAR_DOWN_REPLYTEXT_DLG:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawThumb(hDC,m_pReplyDlg->GetThumbRect(),SBT_ACTIVE);
                     theApp.GetUpImage(SBT_ACTIVE)->Blit(hDC,m_pReplyDlg->GetUpRect());
                     theApp.GetDownImage(SBT_HOVER)->Blit(hDC,m_hotspots[i].m_spot);
                  }
               }
               break;
            case HST_SCROLLBAR_THUMB_REPLYTEXT_DLG:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawThumb(hDC,m_hotspots[i].m_spot,SBT_HOVER);
                     theApp.GetUpImage(SBT_ACTIVE)->Blit(hDC,m_pReplyDlg->GetUpRect());
                     theApp.GetDownImage(SBT_ACTIVE)->Blit(hDC,m_pReplyDlg->GetDownRect());
                  }
               }
               break;
            case HST_SCROLLBAR_REPLYTEXT_DLG:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawThumb(hDC,m_pReplyDlg->GetThumbRect(),SBT_ACTIVE);
                     theApp.GetUpImage(SBT_ACTIVE)->Blit(hDC,m_pReplyDlg->GetUpRect());
                     theApp.GetDownImage(SBT_ACTIVE)->Blit(hDC,m_pReplyDlg->GetDownRect());
                  }
               }
               break;
            case HST_REFRESH:
               {
                  theApp.GetRefreshBuffer(true,m_hotspots[i].m_bAnim, m_frame)->Blit(hDC,m_hotspots[i].m_spot);
               }
               break;
            case HST_CLOSEREPLY: 
            case HST_COLLAPSEPOST:
               {
                  theApp.GetCloseImage(true)->Blit(hDC,m_hotspots[i].m_spot);
               }
               break;
            case HST_OPENINTAB:
               {
                  theApp.GetTabBuffer(true)->Blit(hDC,m_hotspots[i].m_spot);
               }
               break;
            case HST_PIN:
               {
                  theApp.GetPinImage(m_hotspots[i].m_bOn,true)->Blit(hDC,m_hotspots[i].m_spot);
               }
               break;
            case HST_MOD_TOOL:
               {
                  switch(m_hotspots[i].m_cat_type)
                  {
                  case PCT_NORMAL:        theApp.GetOnTopicImage(true)->Blit(hDC,m_hotspots[i].m_spot);break;
                  case PCT_INF:           theApp.GetINFImage(true)->Blit(hDC,m_hotspots[i].m_spot);break;
                  case PCT_NWS:           theApp.GetNWSImage(true)->Blit(hDC,m_hotspots[i].m_spot);break;
                  case PCT_STUPID:        theApp.GetStupidImage(true)->Blit(hDC,m_hotspots[i].m_spot);break;
                  case PCT_OFFTOPIC:      theApp.GetOffTopicImage(true)->Blit(hDC,m_hotspots[i].m_spot);break;
                  case PCT_POLITICAL:     theApp.GetPoliticalImage(true)->Blit(hDC,m_hotspots[i].m_spot);break;
                  case PCT_NUKED:         theApp.GetNukedImage(true)->Blit(hDC,m_hotspots[i].m_spot);break;
                  }
               }
               break;
            case HST_MOD_TOOL_ITEM:
               {
                  switch(m_hotspots[i].m_cat_type)
                  {
                  case PCT_NORMAL:        theApp.GetOnTopicImage(true)->Blit(hDC,m_hotspots[i].m_spot);break;
                  case PCT_INF:           theApp.GetINFImage(true)->Blit(hDC,m_hotspots[i].m_spot);break;
                  case PCT_NWS:           theApp.GetNWSImage(true)->Blit(hDC,m_hotspots[i].m_spot);break;
                  case PCT_STUPID:        theApp.GetStupidImage(true)->Blit(hDC,m_hotspots[i].m_spot);break;
                  case PCT_OFFTOPIC:      theApp.GetOffTopicImage(true)->Blit(hDC,m_hotspots[i].m_spot);break;
                  case PCT_POLITICAL:     theApp.GetPoliticalImage(true)->Blit(hDC,m_hotspots[i].m_spot);break;
                  case PCT_NUKED:         theApp.GetNukedImage(true)->Blit(hDC,m_hotspots[i].m_spot);break;
                  }
               }
               break;
            case HST_LIGHTNINGBOLT:
               {
                  theApp.GetLightningBoltImage(true)->Blit(hDC,m_hotspots[i].m_spot);
               }
               break;
            case HST_NEWTHREAD:
               {
                  ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);
                  theApp.GetNewThreadImage(true)->Blit(hDC,m_hotspots[i].m_spot);
                  ::IntersectClipRect(hDC,DeviceRectangle.left,DeviceRectangle.top + m_banneroffset,DeviceRectangle.right,DeviceRectangle.bottom);
               }
               break;
            case HST_COMPOSE_MESSAGE:
               {
                  ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);
                  theApp.GetComposeImage(true)->Blit(hDC,m_hotspots[i].m_spot);
                  ::IntersectClipRect(hDC,DeviceRectangle.left,DeviceRectangle.top + m_banneroffset,DeviceRectangle.right,DeviceRectangle.bottom);
               }
               break;
            case HST_REFRESHSTORY:
               {
                  ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);
                  theApp.GetRefreshStoryImage(true)->Blit(hDC,m_hotspots[i].m_spot);
                  ::IntersectClipRect(hDC,DeviceRectangle.left,DeviceRectangle.top + m_banneroffset,DeviceRectangle.right,DeviceRectangle.bottom);
               }
               break;
            case HST_NEW_MESSAGES_NOTE:
               {
                  const UCChar *pChar;
                  int *widths;
                  size_t numchars;
                  int textwidth;
                  int center_x = (DeviceRectangle.left + DeviceRectangle.right) / 2;
                  theApp.GetNewMessagesText(&pChar, &widths, numchars, textwidth);

                  RECT newmessagesrect;
                  newmessagesrect.top = DeviceRectangle.top;
                  newmessagesrect.bottom = newmessagesrect.top + theApp.GetTextHeight();
                  newmessagesrect.left = center_x - (textwidth / 2) - 5;
                  newmessagesrect.right = newmessagesrect.left + textwidth + 10;

                  GetDocument()->DrawNewMessagesTab(hDC, newmessagesrect, pChar, widths, numchars, true);
                  bDrewNewMessagesTab = true;
               }
               break;
            case HST_REPLYPREVIEW:
               {                  
                  if(m_hotspots[i].m_id != m_hover_preview_id)
                  {
                     m_backbuffer->Blit(hDC, m_hotspots[i].m_spot, false);
                     m_whitebuffer.AlphaBlit(hDC, m_hotspots[i].m_spot, false, 64);
                  }

                  if(theApp.ExpandPreviews())
                  {                  
                     bool bSameHoverReply = false;

                     if(m_hotspots[i].m_id == m_hover_preview_id ||
                        m_hotspots[i].m_id == m_PREVIEW_TIMER_id)
                     {
                        // just hovering over hte same guy.
                        // do nothing
                        bSameHoverReply = true;
                     }
                     else if(m_PREVIEW_TIMER_id != m_hotspots[i].m_id)
                     {
                        // start hover timer on this guy
                        if(m_PREVIEW_TIMER_id != 0)
                        {
                           KillTimer(PREVIEW_TIMER);
                        }
                        m_PREVIEW_TIMER_id = m_hotspots[i].m_id;
                        SetTimer(PREVIEW_TIMER,theApp.GetMSecondsPreviewTimer(),NULL);
                     }
                     
                     if(!bSameHoverReply &&
                        m_hover_preview_id != 0 &&
                        m_hotspots.size() > 0)
                     {
                        // cancel the previous preview
                        SetHoverPreviewId(0);
                     }
                  }
               }
               break;
            case HST_AUTHORPREVIEW:
               {
                  m_backbuffer->Blit(hDC, m_hotspots[i].m_spot, false);
                  m_whitebuffer.AlphaBlit(hDC, m_hotspots[i].m_spot, false, 64);
               }
               break;
            case HST_REPLIESTOROOTPOSTHINT:
            case HST_POSTAS:
            case HST_AUTHOR:
            case HST_LINK:
            case HST_IMAGE_LINK:
            case HST_SPELL_SUGGESTION:
            case HST_CLOSE_MESSAGE:
            case HST_EXPAND:
               {
                  m_backbuffer->Blit(hDC, m_hotspots[i].m_spot, false);
                  m_whitebuffer.AlphaBlit(hDC, m_hotspots[i].m_spot, false, 32);
               }
               break;
            case HST_MSG_INFO:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_replybuffer.Blit(hDC, m_hotspots[i].m_spot, false, 0, -(DeviceRectangle.bottom - m_pReplyDlg->GetHeight()));
                     m_whitereplybuffer.AlphaBlit(hDC, m_hotspots[i].m_spot, false, 32, 0, -(DeviceRectangle.bottom - m_pReplyDlg->GetHeight()));
                  }
               }
               break;
            case HST_PREV_PAGE:
            case HST_NEXT_PAGE:
            case HST_PAGE:
               {
                  ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);
                  m_bannerbuffer.Blit(hDC, m_hotspots[i].m_spot, false);
                  m_whitebuffer.AlphaBlit(hDC, m_hotspots[i].m_spot, false, 32);
                  ::IntersectClipRect(hDC,DeviceRectangle.left,DeviceRectangle.top + m_banneroffset,DeviceRectangle.right,DeviceRectangle.bottom);
               }
               break;
            case HST_CREATEREPLY:
            case HST_REPLY_TO_MESSAGE:
               {
                  theApp.GetReplyBuffer(true)->Blit(hDC,m_hotspots[i].m_spot);
               }
               break;
            case HST_FORWARD_MESSAGE:
               {
                  theApp.GetForwardImage(true)->Blit(hDC,m_hotspots[i].m_spot);
               }
               break;
            case HST_DELETE_MESSAGE:
               {
                  theApp.GetDeleteImage(true)->Blit(hDC,m_hotspots[i].m_spot);
               }
               break;
            case HST_CLOSEREPLYDLG:
               {
                  theApp.GetCloseImage(true)->Blit(hDC,m_hotspots[i].m_spot);
               }
               break;
            case HST_POST:
               {
                  theApp.GetPostImage(true)->Blit(hDC,m_hotspots[i].m_spot);
               }
               break;
            case HST_SEND:
               {
                  theApp.GetSendImage(true)->Blit(hDC,m_hotspots[i].m_spot);
               }
               break;
            case HST_PREVIEW:
               {
                  theApp.GetPreviewImage(true)->Blit(hDC,m_hotspots[i].m_spot);
               }
               break;
            case HST_UNPREVIEW:
               {
                  theApp.GetPreviewImage(false)->Blit(hDC,m_hotspots[i].m_spot);
               }
               break;
            case HST_TAG_RED:
               {
                  theApp.GetTagsImage(true)->Blit(hDC,m_hotspots[i].m_spot,true,0,40);
               }
               break;
            case HST_TAG_GREEN:
               {
                  theApp.GetTagsImage(true)->Blit(hDC,m_hotspots[i].m_spot,true,0,66);
               }
               break;
            case HST_TAG_BLUE:
               {
                  theApp.GetTagsImage(true)->Blit(hDC,m_hotspots[i].m_spot,true,0,92);
               }
               break;
            case HST_TAG_YELLOW:
               {
                  theApp.GetTagsImage(true)->Blit(hDC,m_hotspots[i].m_spot,true,0,118);
               }
               break;
            case HST_TAG_OLIVE:
               {
                  theApp.GetTagsImage(true)->Blit(hDC,m_hotspots[i].m_spot,true,0,144);
               }
               break;
            case HST_TAG_LIME:
               {
                  theApp.GetTagsImage(true)->Blit(hDC,m_hotspots[i].m_spot,true,0,170);
               }
               break;
            case HST_TAG_ORANGE:
               {
                  theApp.GetTagsImage(true)->Blit(hDC,m_hotspots[i].m_spot,true,0,196);
               }
               break;
            case HST_TAG_PINK:
               {
                  theApp.GetTagsImage(true)->Blit(hDC,m_hotspots[i].m_spot,true,0,222);
               }
               break;
            case HST_TAG_ITALICS:
               {
                  theApp.GetTagsImage(true)->Blit(hDC,m_hotspots[i].m_spot,true,67,40);
               }
               break;
            case HST_TAG_BOLD:
               {
                  theApp.GetTagsImage(true)->Blit(hDC,m_hotspots[i].m_spot,true,67,66);
               }
               break;
            case HST_TAG_QUOTE:
               {
                  theApp.GetTagsImage(true)->Blit(hDC,m_hotspots[i].m_spot,true,67,92);
               }
               break;
            case HST_TAG_SAMPLE:
               {
                  theApp.GetTagsImage(true)->Blit(hDC,m_hotspots[i].m_spot,true,67,118);
               }
               break;
            case HST_TAG_UNDERLINE:
               {
                  theApp.GetTagsImage(true)->Blit(hDC,m_hotspots[i].m_spot,true,67,144);
               }
               break;
            case HST_TAG_STRIKE:
               {
                  theApp.GetTagsImage(true)->Blit(hDC,m_hotspots[i].m_spot,true,67,170);
               }
               break;
            case HST_TAG_SPOILER:
               {
                  theApp.GetTagsImage(true)->Blit(hDC,m_hotspots[i].m_spot,true,67,196);
               }
               break;
            case HST_TAG_CODE:
               {
                  theApp.GetTagsImage(true)->Blit(hDC,m_hotspots[i].m_spot,true,67,222);
               }
               break;
            case HST_LOLTAG:
               {
                  GetDocument()->DrawLOLField(hDC, LTT_LOL, m_hotspots[i].m_spot, m_hotspots[i].m_loltext, true, m_hotspots[i].m_lolvoted, m_hotspots[i].m_lolroot, m_hotspots[i].m_haslols);
               }
               break;
            case HST_INFTAG:
               {
                  GetDocument()->DrawLOLField(hDC, LTT_INF, m_hotspots[i].m_spot, m_hotspots[i].m_loltext, true, m_hotspots[i].m_lolvoted, m_hotspots[i].m_lolroot, m_hotspots[i].m_haslols);
               }
               break;
            case HST_UNFTAG:
               {
                  GetDocument()->DrawLOLField(hDC, LTT_UNF, m_hotspots[i].m_spot, m_hotspots[i].m_loltext, true, m_hotspots[i].m_lolvoted, m_hotspots[i].m_lolroot, m_hotspots[i].m_haslols);
               }
               break;
            case HST_TAGTAG:
               {
                  GetDocument()->DrawLOLField(hDC, LTT_TAG, m_hotspots[i].m_spot, m_hotspots[i].m_loltext, true, m_hotspots[i].m_lolvoted, m_hotspots[i].m_lolroot, m_hotspots[i].m_haslols);
               }
               break;
            case HST_WTFTAG:
               {
                  GetDocument()->DrawLOLField(hDC, LTT_WTF, m_hotspots[i].m_spot, m_hotspots[i].m_loltext, true, m_hotspots[i].m_lolvoted, m_hotspots[i].m_lolroot, m_hotspots[i].m_haslols);
               }
               break;
            case HST_NULL_BACKGROUND:
               {
                  /* 
                  do nothing.
                  just eat the hotspot to keep others from processing further.
                  */
               }
               break;
            }
            break;
         }
      }
   }

   m_bAnimating = false;
   for(size_t i = 0; i < m_hotspots.size(); i++)
   {
      if(m_hotspots[i].m_bAnim)
      {
         m_bAnimating = true;

         if(m_hotspots[i].m_type == HST_REFRESH)
         {
            theApp.GetRefreshBuffer(true,true, m_frame)->Blit(hDC,m_hotspots[i].m_spot);
         }
      }
   }

   ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);

   return bDrewNewMessagesTab;
}

void CLampView::MakeCurrentPostLegal(bool bTopOnly/* = false*/, bool bKeepInSameLocation/* = false*/, int y/* = 0*/, bool bInstant/* = false*/)
{
   if(GetCurrentId() != 0)
   {
      // force a draw so that positions are updated
      ChattyPost *pPost = GetDocument()->FindPost(GetCurrentId());
      if(pPost != NULL)
      {            
         CancelInertiaPanning();
         m_brakes = false;

         int old_top = pPost->GetPos();
         int old_bottom = old_top + pPost->GetHeight();

         DrawEverythingToBuffer();
         int top = pPost->GetPos();
         int bottom = top + pPost->GetHeight();

         RECT DeviceRectangle;
         GetClientRect(&DeviceRectangle);
         DeviceRectangle.top += (20 + m_banneroffset);
         DeviceRectangle.bottom -= 20;

         m_gotopos = m_pos;

         if(bKeepInSameLocation)
         {
            if(y < top || y > bottom)
            {
               int offset = ((top + bottom) / 2) - ((old_top + old_bottom) / 2);
               m_gotopos += offset;
               top -= offset;
               bottom -= offset;
            }
         }

         if(bTopOnly ||
            top < DeviceRectangle.top ||
            bottom - top > DeviceRectangle.bottom - DeviceRectangle.top)
         {
            m_gotopos += (top - DeviceRectangle.top);
         }
         else if(bottom > DeviceRectangle.bottom)
         {
            m_gotopos += (bottom - DeviceRectangle.bottom);
         }
         
         MakePosLegal();
         if(bInstant)
         {
            m_pos = m_gotopos;
         }
         InvalidateEverything();
      }
   }
}

void CLampView::MakePosLegal()
{
   RECT DeviceRectangle;
   GetClientRect(&DeviceRectangle);

   m_gotopos = __min(GetDocument()->GetHeight() - (DeviceRectangle.bottom - DeviceRectangle.top), m_gotopos);
   m_gotopos = __max(0, m_gotopos);

   if(GetDocument()->GetDataType() == DDT_STORY &&
      theApp.InfinatePaging() &&
      theApp.UseShack() &&
      !GetDocument()->IsFetchingNextPage())
   {
      if(m_gotopos > 0 && 
         GetDocument()->GetHeight() - m_gotopos < (3 * (DeviceRectangle.bottom - DeviceRectangle.top) ))
      {
         if(GetDocument()->FetchNextPage())
         {
            m_bTrackingThumb = false;
         }
      }
   }
}

void CLampView::InvalidateEverything()
{
   m_bDrawHotsOnly = false;
   m_bForceDrawAll = true;
   m_hotspot = NULL;
   m_lasthotspot = NULL;
   Invalidate();                  
}

void CLampView::InvalidateEverythingPan()
{
   InvalidateEverything();
   m_bPanOnly = true;
}

void CLampView::InvalidateHotspots()
{
   if(!m_bForceDrawAll)
   {
      m_bDrawHotsOnly = true;
      Invalidate();
   }
}

void CLampView::InvalidateSkin()
{
   GetDocument()->InvalidateSkin();

   if(m_pReplyDlg != NULL)
   {
      m_pReplyDlg->InvalidateSkin();
   }

   m_whitebuffer.Fill(GetRValue(theApp.GetHoverColor()),GetGValue(theApp.GetHoverColor()),GetBValue(theApp.GetHoverColor()));

   InvalidateEverything();
}

void CLampView::CloseFindDlg()
{
   if(m_pFindDlg != NULL)
   {
      m_pFindDlg->PostMessageW(WM_CLOSE);
   }
}

void CLampView::FindNext()
{
   bool bHadAResult = false;
   if(m_textselectionpost != 0)
   {
      bHadAResult = true;
   }

   bool bFound = GetDocument()->FindNext(theApp.GetFindText(), m_textselectionpost, m_selectionstart, m_selectionend);

   if(!bFound && bHadAResult)
   {
      // wrap around and try again
      m_textselectionpost = 0;
      bFound = GetDocument()->FindNext(theApp.GetFindText(), m_textselectionpost, m_selectionstart, m_selectionend);
   }

   if(bFound)
   {
      if(m_textselectionpost != 0 &&
         m_selectionstart != m_selectionend)
      {
         ChattyPost *pPost = GetDocument()->FindPost(m_textselectionpost);
         if(pPost != NULL)
         {
            // make sure the post is visible
            ChattyPost *pParent = pPost;
            while(pParent->GetParent() != NULL) pParent = pParent->GetParent();

            pParent->UnShowAsTruncated();
            if(pParent->IsCollapsed())
            {
               pParent->Expand();
               theApp.RemoveMyCollapse(pParent->GetId());
            }

            SetCurrentId(m_textselectionpost);
            // force a draw so that positions are updated
            DrawEverythingToBuffer();
            
            std::vector<RECT> selectionrects;
            pPost->GetTextSelectionRects(m_selectionstart,m_selectionend,selectionrects);

            if(selectionrects.size() > 0)
            {
               int min = selectionrects[0].top;
               int max = selectionrects[0].bottom;

               for(size_t i = 0; i < selectionrects.size(); i++)
               {
                  if(selectionrects[i].top < min)
                  {
                     min = selectionrects[i].top;
                  }

                  if(selectionrects[i].bottom > max)
                  {
                     max = selectionrects[i].bottom;
                  }
               }
               int viewpoint = (min + max) >> 1;
               RECT DeviceRectangle;
               GetClientRect(&DeviceRectangle);

               int trim = (int)((float)(DeviceRectangle.bottom - DeviceRectangle.top) * 0.1f);
               DeviceRectangle.top += trim;
               DeviceRectangle.bottom -= trim;

               if(viewpoint < DeviceRectangle.top ||
                  viewpoint > DeviceRectangle.bottom)
               {               
                  m_gotopos = m_pos - (((DeviceRectangle.bottom + DeviceRectangle.top) >> 1) - viewpoint);
                  MakePosLegal();
               }
            }
            else
            {
               m_gotopos = pPost->GetPos();
               MakePosLegal();
            }
            
            if(m_pos != m_gotopos)
            {
               InvalidateEverythingPan();
            }
            else
            {
               InvalidateEverything();
            }
         }
      }
   }
}

void CLampView::UpdateHotspotPosition()
{
   unsigned int id = 0;
   RECT *pNewHotspot = NULL;
   for(size_t i = 0; i < m_hotspots.size(); i++)
   {
      if(m_mousepoint.x >= m_hotspots[i].m_spot.left &&
         m_mousepoint.x < m_hotspots[i].m_spot.right &&
         m_mousepoint.y >= m_hotspots[i].m_spot.top &&
         m_mousepoint.y < m_hotspots[i].m_spot.bottom)
      {
         pNewHotspot = &m_hotspots[i].m_spot;
         m_hotspottype = m_hotspots[i].m_type;
         id = m_hotspots[i].m_id;
         break;
      }
   }

   if(pNewHotspot != m_hotspot)
   {
      m_lasthotspot = m_hotspot;
      m_hotspot = pNewHotspot;
      InvalidateHotspots();
      OnSetCursor(this, HTCLIENT, 0);
      
      if(m_hotspot != NULL)
      {
         switch(m_hotspottype)
         {
         case HST_REPLIESTOROOTPOSTHINT:
            {
               theApp.SetStatusBarText(L"Show All Replies",this);
            }
            break;
         case HST_REFRESH: 
            {
               theApp.SetStatusBarText(L"Refresh Replies",this);
            }
            break;
         case HST_CLOSEREPLY: 
            {
               theApp.SetStatusBarText(L"Close Reply",this);
            }
            break;
         case HST_CLOSE_MESSAGE:
            {
               theApp.SetStatusBarText(L"Close Message",this);
            }
            break;
         case HST_COLLAPSEPOST:
            {
               theApp.SetStatusBarText(L"Collapse Thread",this);
            }
            break;
         case HST_EXPAND:
            {
               theApp.SetStatusBarText(L"Expand Thread",this);
            }
            break;
         case HST_OPENINTAB:
            {
               theApp.SetStatusBarText(L"Open this thread in a Tab",this);
            }
            break;
         case HST_PIN:
            {
               theApp.SetStatusBarText(L"Pin This Thread",this);
            }
            break;
         case HST_MOD_TOOL:
            {
               theApp.SetStatusBarText(L"Set Category",this);
            }
            break;
         case HST_MOD_TOOL_ITEM:
            {
               theApp.SetStatusBarText(L"Set Category",this);
            }
            break;
         case HST_LIGHTNINGBOLT:
            {
               theApp.SetStatusBarText(L"This Person Is Cool!",this);
            }
            break;
         case HST_NEWTHREAD:
            {
               theApp.SetStatusBarText(L"Post a New Thread",this);
            }
            break;
         case HST_COMPOSE_MESSAGE:
            {
               theApp.SetStatusBarText(L"Compose New Message",this);
            }
            break;
         case HST_REFRESHSTORY:
            {
               theApp.SetStatusBarText(L"Refresh",this);
            }
            break;
         case HST_REPLYPREVIEW:
            {
               theApp.SetStatusBarText(L"Show Reply",this);
            }
            break;
         case HST_NEW_MESSAGES_NOTE:
            {
               theApp.SetStatusBarText(L"Go to Inbox",this);
            }
            break;
         case HST_AUTHOR:
         case HST_AUTHORPREVIEW:
            {
               theApp.SetStatusBarText(L"Author Info",this);
            }
            break;
         case HST_PREV_PAGE:
            {
               theApp.SetStatusBarText(L"Previous Page",this);
            }
            break;
         case HST_NEXT_PAGE:
            {
               theApp.SetStatusBarText(L"Next Page",this);
            }
            break;
         case HST_PAGE:
            {
               theApp.SetStatusBarText(L"Change Page",this);
            }
            break;
         case HST_POSTAS:
            {
               theApp.SetStatusBarText(L"Change Login",this);
            }
            break;
         case HST_MSG_INFO:
            {
               theApp.SetStatusBarText(L"Change Message Info",this);
            }
            break;
         case HST_PREVIEW:
            {
               theApp.SetStatusBarText(L"Preview Post with Shack Tags",this);
            }
            break;
         case HST_UNPREVIEW:
            {
               theApp.SetStatusBarText(L"Return to Editing Mode",this);
            }
            break;
         case HST_CREATEREPLY:
         case HST_REPLY_TO_MESSAGE:
            {
               theApp.SetStatusBarText(L"Reply",this);
            }
            break;
         case HST_FORWARD_MESSAGE:
            {
               theApp.SetStatusBarText(L"Forward",this);
            }
            break;
         case HST_DELETE_MESSAGE:
            {
               theApp.SetStatusBarText(L"Delete",this);
            }
            break;
         case HST_CLOSEREPLYDLG:
            {
               theApp.SetStatusBarText(L"Close Reply Dialog",this);
            }
            break;
         case HST_POST:
            {
               theApp.SetStatusBarText(L"Post",this);
            }
            break;
         case HST_SEND:
            {
               theApp.SetStatusBarText(L"Send",this);
            }
            break;
         case HST_SPOILER:
            {
               theApp.SetStatusBarText(L"Reveal Spoiler",this);
            }
            break;
         case HST_TEXT:
         case HST_REPLYTEXT:
            {
               theApp.SetStatusBarText(L"Select Text",this);
            }
            break;
         case HST_SPELL_SUGGESTION:
            {
               theApp.SetStatusBarText(L"Suggested Spelling",this);
            }
            break;
         case HST_LINK:
            {
               ChattyPost *pPost = GetDocument()->FindPost(id);
               if(pPost != NULL)
               {
                  UCString link;
                  pPost->GetLink(m_mousepoint.x, m_mousepoint.y, link);
                  theApp.SetStatusBarText(link,this);
               }
            }
            break;
         case HST_IMAGE_LINK:
            {
               ChattyPost *pPost = GetDocument()->FindPost(id);
               if(pPost != NULL)
               {
                  UCString link;
                  pPost->GetImageLink(m_mousepoint.x, m_mousepoint.y, link);
                  theApp.SetStatusBarText(link,this);
               }
            }
            break;
         case HST_IMAGE:
            {
               ChattyPost *pPost = GetDocument()->FindPost(id);
               if(pPost != NULL)
               {
                  UCString link;
                  pPost->GetLinkToImage(m_mousepoint.x, m_mousepoint.y, link);
                  theApp.SetStatusBarText(link,this);
               }
            }
            break;
         case HST_THUMB:
            {
               ChattyPost *pPost = GetDocument()->FindPost(id);
               if(pPost != NULL)
               {
                  UCString link;
                  pPost->GetLinkToThumb(m_mousepoint.x, m_mousepoint.y, link);
                  theApp.SetStatusBarText(link,this);
               }
            }
            break;
          case HST_NULL_BACKGROUND:
            {
               /* 
               do nothing.
               just eat the hotspot to keep others from processing further.
               */
            }
            break;
         }
      }
      else
      {
         theApp.SetStatusBarText(L"",this);
      }
   }
}

BOOL CLampView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
   if(m_pReplyDlg == NULL ||
      !m_pReplyDlg->OnMouseWheel(nFlags, zDelta, m_mousepoint))
   {
      m_gotopos -= (int)((float)zDelta * theApp.GetMouseWheelScale());
      MakePosLegal();
      InvalidateEverythingPan();
      MakeMousePointInvalid();
   }
   
   return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CLampView::CloseReplyDialog()
{
   if(m_pReplyDlg != NULL)
   {
      if(m_pReplyDlg->GetReplyId() != 0)
      {
         ChattyPost *post = GetDocument()->FindPost(m_pReplyDlg->GetReplyId());
         if(post != NULL)
         {
            post->SetReplyDlg(NULL);
         }
      }
      else
      {
         GetDocument()->SetReplyDlg(NULL);
      }

      delete m_pReplyDlg;
      m_pReplyDlg = NULL;
      
      DrawEverythingToBuffer();
      MakePosLegal();
      InvalidateEverything();
   }
}

void CLampView::OnLButtonDown(UINT nFlags, CPoint point) 
{
   SetCapture();   
   m_gotopos = m_pos;
   CancelInertiaPanning();
   m_bStartedTrackingMouse = true;
   m_lastmousetime = ::GetTickCount();
   m_mousepoint = point;
   m_bDrawMButtonDownIcon = false;
   m_bDoubleClickDragging = false;
   bool bJustPutUpModTool = false;

   if(!m_brakes)
   {
      bool bContinue = true;

      if(!GetDocument()->IsBusy())
      {
         if(m_pReplyDlg != NULL)
         {
            bool bCloseReplyDlg = false;
            bContinue = !m_pReplyDlg->OnLButtonDown(nFlags, point, bCloseReplyDlg);

            if(bCloseReplyDlg)
            {
               CloseReplyDialog();
               bContinue = false;
            }
         
            if(!bContinue)
            {
               m_textselectionpost = 0;
            }
         }
         
         if(bContinue)
         {   
            if(m_doubleclicktime != 0)
            {
               if(::GetTickCount() - m_doubleclicktime < 1000)
               {
                  // triple click mutha fucka!
                  for(size_t i = 0; i < m_hotspots.size(); i++)
                  {
                     if(m_mousepoint.x >= m_hotspots[i].m_spot.left &&
                        m_mousepoint.x < m_hotspots[i].m_spot.right &&
                        m_mousepoint.y >= m_hotspots[i].m_spot.top &&
                        m_mousepoint.y < m_hotspots[i].m_spot.bottom)
                     {
                        if(m_hotspots[i].m_type == HST_TEXT)
                        {
                           ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
                           if(pPost != NULL)
                           {
                              pPost->GetCharPosesForPara(m_mousepoint.x, m_mousepoint.y, m_selectionstart, m_selectionend);
                              m_textselectionpost = m_hotspots[i].m_id;
                              InvalidateEverything();
                              bContinue = false;
                           }
                        }
                        break;
                     }
                  }
               }
            }

            m_doubleclicktime = 0;
                
            if(bContinue)
            {
               bool bHandledByHotspot = false;

               m_textselectionpost = 0;

               if(m_hotspots.size() == 0 &&
                  m_hover_preview_id != 0)
               {
                  // see if it is the reply preview
                  ChattyPost *pPost = GetDocument()->FindPost(m_hover_preview_id);
                  if(pPost != NULL)
                  {            
                     int top = pPost->GetPos();
                     int bottom = top + theApp.GetTextHeight();

                     if(m_mousepoint.y >= top &&
                        m_mousepoint.y <= bottom)
                     {
                        ChattyPost *pParent = pPost->GetRoot();
                        pParent->UnShowAsTruncated();

                        SetCurrentId(m_hover_preview_id);
                        m_textselectionpost = 0;
                        m_selectionstart = 0;
                        m_selectionend = 0;
                        // force a draw so that positions are updated
                        MakeCurrentPostLegal(false,true,m_mousepoint.y,true);
                        bHandledByHotspot = true;
                     }

                     m_hover_preview_percent = 1.0f;
                     m_hover_preview_id = 0;
                  }
               }

               if(m_PREVIEW_TIMER_id != 0)
               {
                  KillTimer(PREVIEW_TIMER);
               }
               m_PREVIEW_TIMER_id = 0;

               for(size_t i = 0; i < m_hotspots.size() && !bHandledByHotspot; i++)
               {
                  if(m_mousepoint.x >= m_hotspots[i].m_spot.left &&
                     m_mousepoint.x < m_hotspots[i].m_spot.right &&
                     m_mousepoint.y >= m_hotspots[i].m_spot.top &&
                     m_mousepoint.y < m_hotspots[i].m_spot.bottom)
                  {
                     bHandledByHotspot = true;
                     switch(m_hotspots[i].m_type)
                     {
                     case HST_SCROLLBAR_UP:
                        {
                           m_bLButtonDownOnScrollArrow = true;
                           m_gotopos -= 20;
                           MakePosLegal();
                           InvalidateEverythingPan();
                        }
                        break;
                     case HST_SCROLLBAR_DOWN:
                        {
                           m_bLButtonDownOnScrollArrow = true;
                           m_gotopos += 20;
                           MakePosLegal();
                           InvalidateEverythingPan();
                        }
                        break;
                     case HST_SCROLLBAR_THUMB:
                        {
                           m_bTrackingThumb = true;
                           m_thumbdownpoint = m_mousepoint;
                           m_thumbdownpos = m_pos;
                        }
                        break;
                     case HST_SCROLLBAR:
                        {               
                           if(m_mousepoint.x >= m_uptrackrect.left &&
                              m_mousepoint.x < m_uptrackrect.right &&
                              m_mousepoint.y >= m_uptrackrect.top &&
                              m_mousepoint.y < m_uptrackrect.bottom)
                           {
                              if(m_mousepoint.y >= (m_uptrackrect.bottom - (m_thumbrect.bottom - m_thumbrect.top)))
                              {
                                 // prev page
                                 m_gotopos -= ((m_ScrollRectangle.bottom - m_ScrollRectangle.top) - 20);
                                 MakePosLegal();
                                 InvalidateEverythingPan();
                              }
                              else
                              {
                                 // goto spot
                                 m_gotopos = ((int)((float)(m_mousepoint.y - 16 - m_ScrollRectangle.top) * (1.0f / m_scrollscale))) - ((m_ScrollRectangle.bottom - m_ScrollRectangle.top) >> 1);
                                 MakePosLegal();
                                 InvalidateEverythingPan();
                              }
                           }
                           else if(m_mousepoint.x >= m_downtrackrect.left &&
                                   m_mousepoint.x < m_downtrackrect.right &&
                                   m_mousepoint.y >= m_downtrackrect.top &&
                                   m_mousepoint.y < m_downtrackrect.bottom)
                           {
                              RECT DeviceRectangle;
                              GetClientRect(&DeviceRectangle);

                              if(m_mousepoint.y <= (m_downtrackrect.top + (m_thumbrect.bottom - m_thumbrect.top)))
                              {
                                 // next page
                                 m_gotopos += ((m_ScrollRectangle.bottom - m_ScrollRectangle.top) - 20);
                                 MakePosLegal();
                                 InvalidateEverythingPan();
                              }
                              else
                              {
                                 // goto spot
                                 m_gotopos = ((int)((float)(m_mousepoint.y - 16 - m_ScrollRectangle.top) * (1.0f / m_scrollscale))) - ((m_ScrollRectangle.bottom - m_ScrollRectangle.top) >> 1);
                                 MakePosLegal();
                                 InvalidateEverythingPan();
                              }
                           }
                        }
                        break;
                     case HST_REPLIESTOROOTPOSTHINT:
                        {
                           ChattyPost *pPost = GetDocument()->GetRootPost(m_hotspots[i].m_id);
                           if(pPost != NULL)
                           {
                              UpdateCurrentIdAsRoot(m_hotspots[i].m_id);
                              pPost->UnShowAsTruncated();
                              InvalidateEverything();
                           }
                        }
                        break;
                     case HST_REFRESH: 
                        {
                           unsigned int id = m_hotspots[i].m_id;
                           GetDocument()->RefreshThread(GetDocument()->GetRootId(id), id);
                        }
                        break;
                     case HST_CLOSEREPLY: 
                        {
                           SetCurrentId(0);
                           m_textselectionpost = 0;
                           m_selectionstart = 0;
                           m_selectionend = 0;
                           InvalidateEverything();
                        }
                        break;
                     case HST_CLOSE_MESSAGE:
                        {
                           ChattyPost *post = GetDocument()->FindPost(m_hotspots[i].m_id);
                           if(post != NULL)
                           {
                              post->Collapse();
                              InvalidateEverything();
                           }
                        }
                        break;
                     case HST_COLLAPSEPOST:
                        {
                           ChattyPost *post = GetDocument()->FindPost(m_hotspots[i].m_id);
                           if(post != NULL)
                           {
                              if(GetCurrentId() == m_hotspots[i].m_id)
                              {
                                 SetCurrentId(0);
                                 m_textselectionpost = 0;
                                 m_selectionstart = 0;
                                 m_selectionend = 0;
                              }
                              post->Collapse();
                              theApp.AddMyCollapse(m_hotspots[i].m_id);
                              InvalidateEverything();
                           }
                        }
                        break;
                     case HST_EXPAND:
                        {
                           ChattyPost *post = GetDocument()->FindPost(m_hotspots[i].m_id);
                           if(post != NULL)
                           {
                              post->Expand();
                              theApp.RemoveMyCollapse(m_hotspots[i].m_id);
                              if(GetDocument()->GetDataType() == DDT_SHACKMSG &&
                                 post->GetHaveRead() == false)
                              {
                                 post->SetHaveRead(true);
                                 GetDocument()->UpdateUnreadShackMessagesCount();
                                 GetDocument()->MarkShackMessageRead(m_hotspots[i].m_id);
                              }
                              InvalidateEverything();
                           }
                        }
                        break;
                     case HST_OPENINTAB:
                        {
                           unsigned int id = GetDocument()->GetID(m_hotspots[i].m_id);
                           unsigned int rootid = GetDocument()->GetRootId(id);
                           UCString path;
                           if(rootid != 0)
                           {
                              path += rootid;
                           }
                           else
                           {
                              path += id;
                           }
                           path += L'_';
                           path += id;
                           ReleaseCapture();
                           theApp.OpenDocumentFile(path);
                        }
                        break;
                     case HST_PIN:
                        {
                           ChattyPost *post = GetDocument()->FindPost(m_hotspots[i].m_id);
                           if(post != NULL)
                           {
                              post->SetPinned(!post->IsPinned());
                              InvalidateEverything();
                           }
                        }
                        break;
                     case HST_MOD_TOOL:
                        {
                           CDCSurface *pImage = theApp.GetINFImage(true);
                           if(pImage != NULL)
                           {
                              m_ModToolPostID = m_hotspots[i].m_id;

                              m_ModToolRect.left = m_hotspots[i].m_spot.left;
                              m_ModToolRect.top = m_hotspots[i].m_spot.bottom;
                              m_ModToolRect.right = m_ModToolRect.left + pImage->GetWidth();
                              m_ModToolRect.bottom = m_ModToolRect.top + (pImage->GetHeight() * 7);

                              m_bModToolIsUp = true;
                              bJustPutUpModTool = true;
                              InvalidateEverything();
                           }
                        }
                        break;
                     case HST_MOD_TOOL_ITEM:
                        {
                           GetDocument()->SetCategory_Mod(m_ModToolPostID, m_hotspots[i].m_cat_type);
                           m_bModToolIsUp = false;
                           InvalidateEverything();
                        }
                        break;
                     case HST_LIGHTNINGBOLT:
                        {
                           UCString link(L"http://www.shacknews.com/mercury");
                           theApp.OpenShackLink(link);
                        }
                        break;
                     case HST_NEWTHREAD:
                        {
                           m_history_list.clear();
                           m_history_it = m_history_list.end();
                           m_current_id = 0;
                           m_textselectionpost = 0;
                           m_selectionstart = 0;
                           m_selectionend = 0;
                           OnEditNewthread();
                        }
                        break;
                     case HST_COMPOSE_MESSAGE:
                        {
                           SendMessageDlg(GetDocument(),UCString(),UCString(),UCString());
                        }
                        break;
                     case HST_REFRESHSTORY:
                        {
                           CloseReplyDialog();
                           OnEditRefresh();
                        }
                        break;
                     case HST_REPLYPREVIEW:
                        {
                           ChattyPost *post = GetDocument()->FindPost(m_hotspots[i].m_id);
                           if(post != NULL)
                           {
                              ChattyPost *pParent = post;
                              while(pParent->GetParent() != NULL) pParent = pParent->GetParent();
                              pParent->UnShowAsTruncated();

                              SetCurrentId(m_hotspots[i].m_id);
                              m_textselectionpost = 0;
                              m_selectionstart = 0;
                              m_selectionend = 0;
                              // force a draw so that positions are updated
                              MakeCurrentPostLegal(false,true,m_mousepoint.y,true);
                           }
                        }
                        break;
                     case HST_NEW_MESSAGES_NOTE:
                        {
                           ReleaseCapture();
                           theApp.ShowNewMessages();
                        }
                        break;
                     case HST_AUTHOR:
                     case HST_AUTHORPREVIEW:
                        {
                           ChattyPost *post = GetDocument()->FindPost(m_hotspots[i].m_id);
                           if(post != NULL)
                           {
                              CPoint worldpoint = m_mousepoint;
                              //ScreenToClient(&worldpoint);
                              ClientToScreen(&worldpoint);
                              m_authorname_clicked = post->GetAuthor();
                              theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_USERLINK_MENU, worldpoint.x, worldpoint.y, this, TRUE);
                           }
                           
                           /*
                           //GetDocument()->GetAuthorInfo(m_hotspots[i].m_id);                           
                           ChattyPost *post = GetDocument()->FindPost(m_hotspots[i].m_id);
                           if(post != NULL)
                           {
                              UCString author = post->GetAuthor();
                              UCString info = author;
                              info += L"\r\nProfile info will go here.\r\nOnce we have profiles again.";

                              AuthorDlg adlg(this);
                              adlg.m_info = info;
                              adlg.m_author = author;
                              if(m_pReplyDlg != NULL)
                              {
                                 adlg.m_bReplyIsUp = true;
                              }
                              else
                              {
                                 adlg.m_bReplyIsUp = false;
                              }
                              m_dlgup = true;
                              adlg.DoModal();
                              m_dlgup = false;
                              if(adlg.m_bSendMessage)
                              {
                                 SendMessageDlg(GetDocument(),author,UCString(),UCString());
                              }
                           }
                           */
                        }
                        break;
                     case HST_PREV_PAGE:
                        {
                           CloseReplyDialog();
                           GetDocument()->SetPage(GetDocument()->GetPage() - 1);
                           GetDocument()->Refresh();
                           m_gotopos = 0;
                           m_history_list.clear();
                           m_history_it = m_history_list.end();
                           m_current_id = 0;
                           m_textselectionpost = 0;
                           m_selectionstart = 0;
                           m_selectionend = 0;
                           InvalidateEverything();
                        }
                        break;
                     case HST_NEXT_PAGE:
                        {
                           CloseReplyDialog();
                           GetDocument()->SetPage(GetDocument()->GetPage() + 1);
                           GetDocument()->Refresh();
                           m_gotopos = 0;
                           m_history_list.clear();
                           m_history_it = m_history_list.end();
                           m_current_id = 0;
                           m_textselectionpost = 0;
                           m_selectionstart = 0;
                           m_selectionend = 0;
                           InvalidateEverything();
                        }
                        break;
                     case HST_PAGE:
                        {
                           int page = (int)m_hotspots[i].m_id;
                           CloseReplyDialog();
                           GetDocument()->SetPage(page);
                           GetDocument()->Refresh();
                           m_gotopos = 0;
                           m_history_list.clear();
                           m_history_it = m_history_list.end();
                           m_current_id = 0;
                           m_textselectionpost = 0;
                           m_selectionstart = 0;
                           m_selectionend = 0;
                           InvalidateEverything();
                        }
                        break;
                     case HST_CREATEREPLY:
                        {
                           if(m_pReplyDlg != NULL)
                           {
                              ChattyPost *post = GetDocument()->FindPost(m_pReplyDlg->GetReplyId());
                              if(post != NULL)
                              {
                                 post->SetReplyDlg(NULL);
                              }

                              SetCurrentId(m_hotspots[i].m_id);
                              m_textselectionpost = 0;
                              m_selectionstart = 0;
                              m_selectionend = 0;
                              post = GetDocument()->FindPost(m_hotspots[i].m_id);
                              if(post != NULL)
                              {
                                 m_pReplyDlg->SetReplyId(m_hotspots[i].m_id);
                                 post->SetReplyDlg(m_pReplyDlg);
                              }                                 
                              InvalidateEverything();
                           }
                           else if(theApp.HaveLogin())
                           {
                              ChattyPost *post = GetDocument()->FindPost(m_hotspots[i].m_id);
                              if(post != NULL)
                              {
                                 SetCurrentId(m_hotspots[i].m_id);
                                 m_textselectionpost = 0;
                                 m_selectionstart = 0;
                                 m_selectionend = 0;
                                 m_pReplyDlg = new CReplyDlg(this);
                                 m_pReplyDlg->SetDoc(GetDocument());
                                 m_pReplyDlg->SetReplyId(m_hotspots[i].m_id);
                                 post->SetReplyDlg(m_pReplyDlg);
                                 
                                 RECT DeviceRectangle;
                                 GetClientRect(&DeviceRectangle);

                                 int top = post->GetPos() + post->GetHeight();
                                 int bottom = top + m_pReplyDlg->GetHeight();
                                 
                                 if(bottom > DeviceRectangle.bottom)
                                 {
                                    m_gotopos += (bottom - DeviceRectangle.bottom) + 20;
                                    DrawEverythingToBuffer();
                                    MakePosLegal();
                                 }
                                 InvalidateEverything();
                              }
                           }
                        }
                        break;
                     case HST_REPLY_TO_MESSAGE:
                        {
                           if(GetDocument()->GetDataType() == DDT_SHACKMSG)
                           {
                              ChattyPost *post = GetDocument()->FindPost(m_hotspots[i].m_id);
                              if(post != NULL)
                              {
                                 UCString author = post->GetAuthor();

                                 UCString subject;
                                 const UCString &existingsubject = post->GetSubject();
                                 if(existingsubject.beginswith(L"Re: ") == NULL)
                                 {
                                    subject = L"Re: ";
                                 }
                                 subject += post->GetSubject();

                                 UCString shackmsg = L"\r\n/[On ";
                                 shackmsg += post->GetDateText();
                                 shackmsg += L" ";
                                 shackmsg += author;
                                 shackmsg += L" wrote:\r\n-----\r\n";

                                 UCString temp = post->GetBodyText();
                                 temp.Replace(L"\n",L"\r\n");
                                 shackmsg += temp;
                                 shackmsg += L"]/";

                                 SendMessageDlg(GetDocument(),author,subject,shackmsg);
                                 
                                 InvalidateEverything();
                              }
                           }
                        }
                        break;
                     case HST_FORWARD_MESSAGE:
                        {
                           if(GetDocument()->GetDataType() == DDT_SHACKMSG)
                           {
                              ChattyPost *post = GetDocument()->FindPost(m_hotspots[i].m_id);
                              if(post != NULL)
                              {
                                 UCString subject;
                                 const UCString &existingsubject = post->GetSubject();
                                 if(existingsubject.beginswith(L"Fwd: ") == NULL)
                                 {
                                    subject = L"Fwd: ";
                                 }
                                 subject += post->GetSubject();

                                 UCString shackmsg = L"\r\n/[On ";
                                 shackmsg += post->GetDateText();
                                 shackmsg += L" ";
                                 shackmsg += post->GetAuthor();
                                 shackmsg += L" wrote:\r\n-----\r\n";

                                 UCString temp = post->GetBodyText();
                                 temp.Replace(L"\n",L"\r\n");
                                 shackmsg += temp;
                                 shackmsg += L"]/";

                                 SendMessageDlg(GetDocument(),UCString(),subject,shackmsg);
                                 
                                 InvalidateEverything();

                                 if(m_pReplyDlg != NULL)
                                 {
                                    m_pReplyDlg->PromptForMessageInfo();
                                 }
                              }
                           }
                        }
                        break;
                     case HST_DELETE_MESSAGE:
                        {
                           if(GetDocument()->GetDataType() == DDT_SHACKMSG)
                           {
                              ChattyPost *post = GetDocument()->FindPost(m_hotspots[i].m_id);
                              if(post != NULL)
                              {
                                 UCString msg(L"Are you sure you wish to delete this message?");
                                 int ret = MessageBox(msg,L"Lamp",MB_YESNO);
                                 if(ret == IDYES)
                                 {
                                    GetDocument()->DeleteShackMessage(post->GetId());
                                 }
                              }
                           }
                        }
                        break;
                     case HST_SPOILER:
                        {
                           ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
                           if(pPost != NULL)
                           {
                              UpdateCurrentIdAsRoot(m_hotspots[i].m_id);
                              pPost->ClearSpoilerTags(m_mousepoint.x, m_mousepoint.y);
                              InvalidateEverything();
                           }
                        }
                        break;
                     case HST_TEXT:
                        {
                           ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
                           if(pPost != NULL)
                           {
                              unsigned int id = m_hotspots[i].m_id;
                              UpdateCurrentIdAsRoot(id);
                              bool off_end;
                              m_selectionstart = pPost->GetCharPos(m_mousepoint.x, m_mousepoint.y, off_end);
                              m_textselectionpost = id;
                              m_bDraggingTextSelection = true;
                              m_lastcharpos = m_selectionend_actual = m_selectionstart_actual = m_selectionend = m_selectionstart;
                              InvalidateEverything();
                           }
                        }
                        break;
                     case HST_LINK:
                        {
                           ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
                           if(pPost != NULL)
                           {
                              UpdateCurrentIdAsRoot(m_hotspots[i].m_id);
                              UCString link;
                              pPost->GetLink(m_mousepoint.x, m_mousepoint.y, link);

                              if(link.beginswith(L"sendmessage:"))
                              {
                                 m_authorname_clicked = link.Str() + 12;
                                 OnSendMessage();
                              }
                              else
                              {
                                 const UCChar *work = NULL;
                                                                     
                                 bool bIsLocal = false;
                                 if(link.beginswith(L"http://www.shacknews.com/chatty?id="))// http://www.shacknews.com/chatty?id=25857324#itemanchor_25857324
                                 {
                                    work = link.Str() + 35;
                                 }
                                 else if(link.beginswith(L"http://www.shacknews.com/laryn.x?id="))// http://www.shacknews.com/laryn.x?id=20919390#itemanchor_20919390
                                 {
                                    work = link.Str() + 36;
                                 }

                                 if(work != NULL)
                                 {
                                    UCString temp;
                                    while(*work != 0 && iswdigit(*work))
                                    {
                                       temp += *work;
                                       work++;
                                    }

                                    unsigned int id = temp;

                                    ChattyPost *post = GetDocument()->FindPost(id);
                                    if(post != NULL)
                                    {
                                       SetCurrentId(id);
                                       m_textselectionpost = 0;
                                       m_selectionstart = 0;
                                       m_selectionend = 0;
                                       bIsLocal = true;

                                       MakeCurrentPostLegal();
                                       
                                       CancelInertiaPanning();
                                       m_brakes = false;
                                       
                                       InvalidateEverything();
                                    }
                                 }
                                 
                                 if(!bIsLocal)
                                 {
                                    theApp.OpenShackLink(link);
                                 }
                              }
                           }
                        }
                        break;
                     case HST_IMAGE_LINK:
                        {
                           ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
                           if(pPost != NULL)
                           {
                              unsigned int id = m_hotspots[i].m_id;
                              UpdateCurrentIdAsRoot(id);
                              UCString link;
                              pPost->GetImageLink(m_mousepoint.x, m_mousepoint.y, link);

                              const UCChar *begin = link;
                              const UCChar *end = begin + link.Length();
                              const UCChar *ext = end;
                              while(ext > begin && *ext != L'.') ext--;

                              if(_wcsicmp(ext,L".png") == 0 ||
                                 _wcsicmp(ext,L".jpg") == 0 || 
                                 _wcsicmp(ext,L".jpeg") == 0)
                              {
                                 unsigned int index;
                                 CDCSurface *pImage = theApp.GetLinkedImage(link,index);
                                 
                                 pPost->MakeLinkIntoImage(m_mousepoint.x, m_mousepoint.y, index);
                                 InvalidateEverything();

                                 if(!theApp.IsImageLoaded(index))
                                 {
                                    theApp.LoadImage(index,id);
                                 }
                              }
                           }
                        }
                        break;
                     case HST_IMAGE:
                        {
                           ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
                           if(pPost != NULL)
                           {
                              UpdateCurrentIdAsRoot(m_hotspots[i].m_id);
                              if(theApp.ShowImageThumbs() &&
                                 (!theApp.DontAutoLoadNWSThumbs() ||
                                  !pPost->IsNWSPost()))
                              {
                                 pPost->MakeImageIntoThumb(m_mousepoint.x, m_mousepoint.y);
                              }
                              else
                              {
                                 pPost->MakeImageIntoLink(m_mousepoint.x, m_mousepoint.y);
                              }
                              
                              if(m_hotspots[i].m_spot.top < (20 + m_banneroffset))

                              m_gotopos = m_pos + (m_hotspots[i].m_spot.top - (20 + m_banneroffset));
                              MakePosLegal();
                              InvalidateEverything();
                           }
                        }
                        break;
                     case HST_THUMB:
                        {
                           ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
                           if(pPost != NULL)
                           {
                              unsigned int id = m_hotspots[i].m_id;
                              UpdateCurrentIdAsRoot(id);
                              UCString link;
                              pPost->GetThumbLink(m_mousepoint.x, m_mousepoint.y, link);

                              unsigned int index;
                              CDCSurface *pImage = theApp.GetLinkedImageThumb(link,index);
                              
                              pPost->MakeThumbIntoImage(m_mousepoint.x, m_mousepoint.y, index);
                              InvalidateEverything();

                              if(!theApp.IsImageLoaded(index))
                              {
                                 theApp.LoadImage(index,id);
                              }
                           }
                        }
                        break;
                     case HST_LOLTAG:
                        {
                           ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
                           if(pPost != NULL)
                           {
                              GetDocument()->LolTagPost(m_hotspots[i].m_id, LTT_LOL);
                              theApp.AddMyLol(m_hotspots[i].m_id,LTT_LOL);
                              pPost->AddLolTag(LTT_LOL);                           
                              InvalidateEverything();
                           }
                        }
                        break;
                     case HST_INFTAG:
                        {
                           ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
                           if(pPost != NULL)
                           {
                              GetDocument()->LolTagPost(m_hotspots[i].m_id, LTT_INF);
                              theApp.AddMyLol(m_hotspots[i].m_id,LTT_INF);
                              pPost->AddLolTag(LTT_INF);                           
                              InvalidateEverything();
                           }
                        }
                        break;
                     case HST_UNFTAG:
                        {
                           ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
                           if(pPost != NULL)
                           {
                              GetDocument()->LolTagPost(m_hotspots[i].m_id, LTT_UNF);
                              theApp.AddMyLol(m_hotspots[i].m_id,LTT_UNF);
                              pPost->AddLolTag(LTT_UNF);
                              InvalidateEverything();
                           }
                        }
                        break;
                     case HST_TAGTAG:
                        {
                           ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
                           if(pPost != NULL)
                           {
                              GetDocument()->LolTagPost(m_hotspots[i].m_id, LTT_TAG);
                              theApp.AddMyLol(m_hotspots[i].m_id,LTT_TAG);
                              pPost->AddLolTag(LTT_TAG);
                              InvalidateEverything();
                           }
                        }
                        break;
                     case HST_WTFTAG:
                        {
                           ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
                           if(pPost != NULL)
                           {
                              GetDocument()->LolTagPost(m_hotspots[i].m_id, LTT_WTF);
                              theApp.AddMyLol(m_hotspots[i].m_id,LTT_WTF);
                              pPost->AddLolTag(LTT_WTF);
                              InvalidateEverything();
                           }
                        }
                        break;
                     case HST_NULL_BACKGROUND:
                        {
                           /* 
                           do nothing.
                           just eat the hotspot to keep others from processing further.
                           */
                        }
                        break;
                     }
                     break;
                  }
               }

               if(!bHandledByHotspot)
               {
                  m_panpoint = point;
                  m_bPanning = true;
                  m_panpos = m_pos;
                  InvalidateEverything();
               }
            }
         }
      }
   }
   else
   {
      m_panpoint = point;
      m_bPanning = true;
      m_panpos = m_pos;
      InvalidateEverything();
   }

   if(m_bModToolIsUp && !bJustPutUpModTool)
   {
      m_bModToolIsUp = false;
      InvalidateEverything();
   }

   CView::OnLButtonDown(nFlags, point);
}

void CLampView::UpdateCurrentIdAsRoot(unsigned int id)
{
   if(GetCurrentId() != 0)
   {
      ChattyPost *post = GetDocument()->FindPost(GetCurrentId());
      if(post != NULL &&
         post->GetParent() != NULL)
      {
         ChattyPost *newpost = GetDocument()->FindPost(id);
         if(newpost != NULL &&
            newpost->GetParent() == NULL)
         {
            int pos = newpost->GetPos();
            SetCurrentId(id);
            DrawEverythingToBuffer();
            int newpos = newpost->GetPos();
            if(pos != newpos)
            {
               m_pos += (newpos - pos);
               m_gotopos += (newpos - pos);
               DrawEverythingToBuffer();
            }
         }
      }
   }
   SetCurrentId(id);
   m_textselectionpost = 0;
   m_selectionstart = 0;
   m_selectionend = 0;
}

void CLampView::OnLButtonUp(UINT nFlags, CPoint point) 
{
   ReleaseCapture();
   TrackMouse(point);

   if(!m_brakes)
   {
      if(!GetDocument()->IsBusy())
      {
         if(m_pReplyDlg == NULL ||
            !m_pReplyDlg->OnLButtonUp(nFlags, point))
         {
            if(m_bLButtonDownOnScrollArrow)
            {
               m_bLButtonDownOnScrollArrow = false;
               InvalidateEverything();
            }
            else if(m_bTrackingThumb)
            {
               m_bTrackingThumb = false;
               m_gotopos = m_thumbdownpos + (int)((float)(m_mousepoint.y - m_thumbdownpoint.y) * (1.0f / m_scrollscale));
               InvalidateEverything();
            }
            else if(m_bDraggingTextSelection)
            {
               m_bDraggingTextSelection = false;
               ChattyPost *pPost = GetDocument()->FindPost(m_textselectionpost);
               if(pPost != NULL)
               {
                  if(m_bDoubleClickDragging)
                  {
                     int selstart, selend;
                     pPost->GetCharPosesForWord(m_mousepoint.x, m_mousepoint.y, selstart, selend);
                     m_selectionstart = __min(m_doubleclickselectionstart, selstart);
                     m_selectionend = __max(m_doubleclickselectionend, selend);
                  }
                  else
                  {
                     bool off_end;
                     m_selectionend = pPost->GetCharPos(m_mousepoint.x, m_mousepoint.y, off_end);
                     if(m_selectionstart == m_selectionend)
                     {
                        m_textselectionpost = 0;
                     }
                  }
               }
               InvalidateEverything();
            }
            else if(m_bPanning)
            {
               m_bPanning = false;
               int ydiff = m_panpoint.y - point.y;
               m_gotopos = m_panpos + ydiff;
               MakePosLegal();
               m_pos = m_gotopos;// no smooth on pan
               BeginInertiaPanning();
            }
            else
            {
               for(size_t i = 0; i < m_hotspots.size(); i++)
               {
                  if(m_hotspots[i].m_type == HST_MOD_TOOL_ITEM &&
                     m_mousepoint.x >= m_hotspots[i].m_spot.left &&
                     m_mousepoint.x < m_hotspots[i].m_spot.right &&
                     m_mousepoint.y >= m_hotspots[i].m_spot.top &&
                     m_mousepoint.y < m_hotspots[i].m_spot.bottom)
                  {
                     GetDocument()->SetCategory_Mod(m_ModToolPostID, m_hotspots[i].m_cat_type);
                     m_bModToolIsUp = false;
                     InvalidateEverything();
                  }
               }
            }
         }
      }
   }
   else if(m_bPanning)
   {
      m_bPanning = false;
      int ydiff = m_panpoint.y - point.y;
      m_gotopos = m_panpos + ydiff;
      MakePosLegal();
      m_pos = m_gotopos;// no smooth on pan
      BeginInertiaPanning();
   }
   m_brakes = false;
   m_bDoubleClickDragging = false;
   
   CView::OnLButtonUp(nFlags, point);
}

void CLampView::OnMouseLeave()
{
   m_mouseOverClientArea = false;

   MakeMousePointInvalid();
   InvalidateEverything();

   if(m_PREVIEW_TIMER_id != 0)
   {
      KillTimer(PREVIEW_TIMER);
   }
   m_PREVIEW_TIMER_id = 0;
   SetHoverPreviewId(0);
}

void CLampView::OnMouseMove(UINT nFlags, CPoint point) 
{
   if(!m_mouseOverClientArea)
   {
      m_mouseOverClientArea = true;
      TRACKMOUSEEVENT tme;
      tme.cbSize = sizeof(tme);
      tme.dwFlags = TME_LEAVE;
      tme.dwHoverTime = HOVER_DEFAULT;
      tme.hwndTrack = m_hWnd;
      _TrackMouseEvent(&tme);
   }

   TrackMouse(point);

   if(!m_brakes)
   {
      if(!GetDocument()->IsBusy())
      {
         if(m_pReplyDlg == NULL ||
            !m_pReplyDlg->OnMouseMove(nFlags, point))
         {
            if((m_bMButtonDown ||
               m_bDrawMButtonDownIcon) &&
               abs(m_MButtonDownPoint.y - m_mousepoint.y) > 13)
            {
               m_bDrawMButtonDownIcon = true;
               InvalidateEverythingPan();
            }
            else if(m_bTrackingThumb)
            {
               m_gotopos = m_thumbdownpos + (int)((float)(m_mousepoint.y - m_thumbdownpoint.y) * (1.0f / m_scrollscale));
               InvalidateEverythingPan();
            }
            else if(m_bDraggingTextSelection)
            {
               ChattyPost *pPost = GetDocument()->FindPost(m_textselectionpost);
               if(pPost != NULL)
               {
                  if(m_bDoubleClickDragging)
                  {
                     int selstart, selend;
                     pPost->GetCharPosesForWord(m_mousepoint.x, m_mousepoint.y, selstart, selend);
                     m_selectionstart = __min(m_doubleclickselectionstart, selstart);
                     m_selectionend = __max(m_doubleclickselectionend, selend);
                  }
                  else
                  {
                     bool off_end;
                     m_selectionend_actual = pPost->GetCharPos(m_mousepoint.x, m_mousepoint.y, off_end);

                     //m_selectionend = m_selectionend_actual;
                     //= m_selectionstart_actual = 

                     //////////////////////////////
                     // at this point, the enhanced text selection logic happens
                     // try to move the carets away from eachother towards word boundries

                     if((size_t)m_selectionend_actual != m_lastcharpos)
                     {
                        bool bMovingAway = false;

                        if((m_selectionend_actual > m_selectionstart_actual &&
                            m_selectionend_actual > m_lastcharpos) ||
                           (m_selectionend_actual < m_selectionstart_actual &&
                            m_selectionend_actual < m_lastcharpos))
                        {
                           bMovingAway = true;
                        }

                        m_lastcharpos = m_selectionend_actual;
                        m_selectionstart = m_selectionstart_actual;
                        m_selectionend = m_selectionend_actual;
                     }
                        /*
                        IDLDocument *pDoc = pView->GetIDoc();
                        if(pDoc != NULL &&
                           m_ETSS != ETSS_RETURN_INSIDE_ORIGIN)
                        {
                           CDLTextSearchDocument *pTextSearchDocument = pDoc->GetTextSearchDocument();
                           if(pTextSearchDocument != NULL)
                           {
                              CDLTextIndex *pTextIndex = pTextSearchDocument->GetTextIndex();
                              if(pTextIndex != NULL)
                              {
                                 bool bForward = true;
                                 if(m_anchorCharPosition_used > m_currentCharPosition_used)
                                 {
                                    bForward = false;
                                 }

                                 if(bMovingAway)
                                 {
                                    if(charpos >= (int)m_originword_min &&
                                       charpos <= (int)m_originword_max)
                                    {
                                       // use raw values
                                       // make the precise word harmless
                                       m_preciseword_min = m_originword_min;
                                       m_preciseword_max = m_originword_max;

                                       if(m_ETSS == ETSS_OUTSIDE_ORIGIN)
                                       {
                                          m_ETSS = ETSS_RETURN_INSIDE_ORIGIN;
                                       }
                                    }
                                    else 
                                    {
                                       if(charpos >= (int)m_preciseword_min &&
                                          charpos <= (int)m_preciseword_max)
                                       {
                                          // use raw value for current
                                          // but snap anchor
                                          m_anchorCharPosition_used = pTextIndex->FindWordEdge((int)m_anchorCharPosition_used, page, !bForward);

                                          if(m_ETSS == ETSS_START_INSIDE_ORIGIN)
                                          {
                                             m_ETSS = ETSS_OUTSIDE_ORIGIN;
                                          }
                                       }
                                       else
                                       {
                                          // do both
                                          m_anchorCharPosition_used = pTextIndex->FindWordEdge((int)m_anchorCharPosition_used, page, !bForward);
                                          m_currentCharPosition_used = pTextIndex->FindWordEdge((int)m_currentCharPosition_used, page, bForward);

                                          // make the precise word harmless
                                          m_preciseword_min = m_originword_min;
                                          m_preciseword_max = m_originword_max;
                                       }

                                       if(m_ETSS == ETSS_START_INSIDE_ORIGIN)
                                       {
                                          m_ETSS = ETSS_OUTSIDE_ORIGIN;
                                       }
                                    }
                                 }
                                 else
                                 {
                                    if(charpos >= (int)m_originword_min &&
                                       charpos <= (int)m_originword_max)
                                    {
                                       // use raw values
                                       // make the precise word harmless
                                       m_preciseword_min = m_originword_min;
                                       m_preciseword_max = m_originword_max;

                                       if(m_ETSS == ETSS_OUTSIDE_ORIGIN)
                                       {
                                          m_ETSS = ETSS_RETURN_INSIDE_ORIGIN;
                                       }
                                    }
                                    else
                                    {
                                       // use raw value for current
                                       // but snap anchor
                                       m_anchorCharPosition_used = pTextIndex->FindWordEdge((int)m_anchorCharPosition_used, page, !bForward);

                                       // record the word we are in so that we can move forward in it precisly later.
                                       int begin, end;
                                       pTextIndex->FindWord(charpos, page, begin, end);
                                       m_preciseword_min = (size_t)begin;
                                       m_preciseword_max = (size_t)end;

                                       if(m_ETSS == ETSS_START_INSIDE_ORIGIN)
                                       {
                                          m_ETSS = ETSS_OUTSIDE_ORIGIN;
                                       }
                                    }
                                 }
                              }
                           }
                        }
                     }
                     */
                     //////////////////////////////
                  }

                  InvalidateEverything();
               }
            }
            else if(m_bPanning)
            {
               int ydiff = m_panpoint.y - point.y;
               m_gotopos = m_panpos + ydiff;
               MakePosLegal();
               m_pos = m_gotopos;// no smooth on pan
               InvalidateEverythingPan();
            }   
            else
            {
               if(theApp.ExpandPreviews())
               {
                  if(GetDocument()->GetDataType() != DDT_THREAD &&
                     m_mousepoint.x >= m_BannerRectangle.left &&
                     m_mousepoint.x < m_BannerRectangle.right &&
                     m_mousepoint.y >= m_BannerRectangle.top &&
                     m_mousepoint.y < m_BannerRectangle.bottom)
                  {
                     // mouse is over banner
                     if(m_PREVIEW_TIMER_id != 0)
                     {
                        KillTimer(PREVIEW_TIMER);
                     }
                     m_PREVIEW_TIMER_id = 0;
                     SetHoverPreviewId(0);
                  }
                  else
                  {
                     bool bSameHoverReply = false;
                     for(size_t i = 0; i < m_hotspots.size(); i++)
                     {
                        if(m_hotspots[i].m_type == HST_REPLYPREVIEW &&
                           m_hotspots[i].m_id != 0 &&
                           m_mousepoint.x >= m_hotspots[i].m_spot.left &&
                           m_mousepoint.x < m_hotspots[i].m_spot.right &&
                           m_mousepoint.y >= m_hotspots[i].m_spot.top &&
                           m_mousepoint.y < m_hotspots[i].m_spot.bottom)
                        {
                           if(m_hotspots[i].m_id == m_hover_preview_id ||
                              m_hotspots[i].m_id == m_PREVIEW_TIMER_id)
                           {
                              // just hovering over hte same guy.
                              // do nothing
                              bSameHoverReply = true;
                              break;
                           }
                           else
                           {
                              // start hover timer on this guy
                              if(m_PREVIEW_TIMER_id != 0)
                              {
                                 KillTimer(PREVIEW_TIMER);
                              }
                              m_PREVIEW_TIMER_id = m_hotspots[i].m_id;
                              SetTimer(PREVIEW_TIMER,theApp.GetMSecondsPreviewTimer(),NULL);
                              break;
                           }
                        }
                     }

                     if(!bSameHoverReply &&
                        m_hover_preview_id != 0 &&
                        m_hotspots.size() > 0)
                     {
                        // cancel the previous preview
                        SetHoverPreviewId(0);
                     }
                  }
               }
            
               UpdateHotspotPosition();
            }
         }
         else
         {
            UpdateHotspotPosition();
         }
      }
   }
   else if(m_bPanning)
   {
      int ydiff = m_panpoint.y - point.y;
      m_gotopos = m_panpos + ydiff;
      MakePosLegal();
      m_pos = m_gotopos;// no smooth on pan
      InvalidateEverythingPan();
   }

   CView::OnMouseMove(nFlags, point);
}

void CLampView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
   m_mousepoint = point;

   if(!GetDocument()->IsBusy())
   {
      if(m_pReplyDlg == NULL ||
         !m_pReplyDlg->OnLButtonDblClk(nFlags, point))
      {
         m_doubleclicktime = 0;
         for(size_t i = 0; i < m_hotspots.size(); i++)
         {
            if(m_mousepoint.x >= m_hotspots[i].m_spot.left &&
               m_mousepoint.x < m_hotspots[i].m_spot.right &&
               m_mousepoint.y >= m_hotspots[i].m_spot.top &&
               m_mousepoint.y < m_hotspots[i].m_spot.bottom)
            {
               if(m_hotspots[i].m_type == HST_TEXT)
               {
                  ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
                  if(pPost != NULL)
                  {
                     pPost->GetCharPosesForWord(m_mousepoint.x, m_mousepoint.y, m_selectionstart, m_selectionend);
                     m_doubleclickselectionstart = m_selectionstart;
                     m_doubleclickselectionend = m_selectionend;
                     m_textselectionpost = m_hotspots[i].m_id;
                     m_doubleclicktime = ::GetTickCount();
                     m_bDoubleClickDragging = true;
                     m_bDraggingTextSelection = true;
                     InvalidateEverything();
                  }
               }
               break;
            }
         }
      }
   }
}

void CLampView::OnMButtonDown(UINT nFlags, CPoint point) 
{
   SetCapture();   
   CancelInertiaPanning();
   m_bStartedTrackingMouse = true;
   m_lastmousetime = ::GetTickCount();
   m_mousepoint = point;

   if(!m_brakes)
   {   
      if(m_bDrawMButtonDownIcon == false)
      {
         m_MButtonDownPoint = point;
         m_mbuttondowntime = ::GetTickCount();
         m_bMButtonDown = true;
      }
      m_bDrawMButtonDownIcon = false;
   }

   if(m_bModToolIsUp)
   {
      m_bModToolIsUp = false;
      InvalidateEverything();
   }

   CView::OnMButtonDown(nFlags, point);
}

void CLampView::OnMButtonUp(UINT nFlags, CPoint point) 
{
   TrackMouse(point);
   m_bMButtonDown = false;
   m_bDrawMButtonDownIcon = false;
   ReleaseCapture();

   if(!m_brakes)
   {
      if(abs(m_MButtonDownPoint.x - point.x) < 5 &&
         abs(m_MButtonDownPoint.y - point.y) < 5 &&
         (::GetTickCount() - m_mbuttondowntime) < 1000 &&
         !GetDocument()->IsBusy())
      {
         m_textselectionpost = 0;
         bool bHandled = false;
         for(size_t i = 0; i < m_hotspots.size(); i++)
         {
            if(m_mousepoint.x >= m_hotspots[i].m_spot.left &&
               m_mousepoint.x < m_hotspots[i].m_spot.right &&
               m_mousepoint.y >= m_hotspots[i].m_spot.top &&
               m_mousepoint.y < m_hotspots[i].m_spot.bottom)
            {
               switch(m_hotspots[i].m_type)
               {
               case HST_PIN:
               case HST_OPENINTAB:
               case HST_REFRESH:
                  {
                     if(GetDocument()->GetDataType() == DDT_STORY)
                     {
                        unsigned int id = GetDocument()->GetID(m_hotspots[i].m_id);
                        unsigned int rootid = GetDocument()->GetRootId(id);
                        UCString path;
                        if(rootid != 0)
                        {
                           path += rootid;
                        }
                        else
                        {
                           path += id;
                        }
                        path += L'_';
                        path += id;
                        ReleaseCapture();
                        theApp.OpenDocumentFile(path);
                        bHandled = true;
                     }
                  }
                  break;
               case HST_SPOILER:
                  {
                     ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
                     if(pPost != NULL)
                     {
                        pPost->ClearSpoilerTags(m_mousepoint.x, m_mousepoint.y);
                     }
                     bHandled = true;
                  }
                  break;
               case HST_LINK:
                  {
                     ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
                     if(pPost != NULL)
                     {
                        UCString link;
                        pPost->GetLink(m_mousepoint.x, m_mousepoint.y, link);
                        theApp.OpenShackLink(link);
                     }
                     bHandled = true;
                  }
                  break;
               case HST_IMAGE_LINK:
                  {
                     ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
                     if(pPost != NULL)
                     {
                        UCString link;
                        pPost->GetImageLink(m_mousepoint.x, m_mousepoint.y, link);
                        theApp.OpenShackLink(link);
                     }
                     bHandled = true;
                  }
                  break;
               case HST_IMAGE:
                  {
                     ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
                     if(pPost != NULL)
                     {
                        UCString link;
                        pPost->GetLinkToImage(m_mousepoint.x, m_mousepoint.y, link);
                        theApp.OpenShackLink(link);
                     }
                     bHandled = true;
                  }
                  break;
               case HST_THUMB:
                  {
                     ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
                     if(pPost != NULL)
                     {
                        UCString link;
                        pPost->GetLinkToThumb(m_mousepoint.x, m_mousepoint.y, link);
                        theApp.OpenShackLink(link);
                     }
                     bHandled = true;
                  }
                  break;
               }
               break;
            }
         }
         
         if(!bHandled)
         {
            m_bDrawMButtonDownIcon = true;
         }
      }
   }
   m_brakes = false;

   InvalidateEverything();

   CView::OnMButtonUp(nFlags, point);
}

void CLampView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   if(m_bModToolIsUp)
   {
      m_bModToolIsUp = false;
      InvalidateEverything();
   }

   if(!GetDocument()->IsBusy())
   {
      if(nChar == VK_ESCAPE)
      {
         CloseReplyDialog();
      }
      else if(m_pReplyDlg != NULL &&
              m_pReplyDlg->GetHasFocus())
      {
         bool bCloseReplyDlg = false;
         
         m_pReplyDlg->OnKeyDown(nChar, nRepCnt, nFlags, bCloseReplyDlg);

         if(bCloseReplyDlg)
         {
            CloseReplyDialog();
         }

         InvalidateEverything();
      }
      else
      {
         bool bShiftPressed = false;
         if(GetAsyncKeyState(VK_SHIFT) & 0xFF00)
         {
            bShiftPressed = true;
         }

         bool bCtrlPressed = false;
         if(GetAsyncKeyState(VK_CONTROL) & 0xFF00)
         {
            bCtrlPressed = true;
         }

         if((nChar == 'w' ||
             nChar == 'W') &&
             bCtrlPressed)
         {
            GetParentFrame()->PostMessage(WM_CLOSE);
            //PostMessage(WM_CLOSE);
            return;
         }
         else if(nChar == VK_F3 &&
                 m_pReplyDlg == NULL)
         {
            if(!theApp.GetFindText().IsEmpty())
            {
               FindNext();
            }
         }
         else if(nChar == VK_DOWN)
         {
            CancelInertiaPanning();
            m_brakes = false;
            m_gotopos += 20;
            MakePosLegal();
            InvalidateEverythingPan();
            MakeMousePointInvalid();
         }
         else if(nChar == VK_UP)
         {
            CancelInertiaPanning();
            m_brakes = false;
            m_gotopos -= 20;
            MakePosLegal();
            InvalidateEverythingPan();
            MakeMousePointInvalid();
         }
         else if(nChar == VK_LEFT)
         {
            ChattyPost *pPost = GetDocument()->FindPost(GetCurrentId());
            if(pPost != NULL)
            {
               ChattyPost *pRoot = pPost->GetRoot();
               if(pRoot != NULL &&
                  pRoot->GetIndentOffset() < 0)
               {
                  pRoot->SetIndentOffset( pRoot->GetIndentOffset() + 1);
                  InvalidateEverything();
               }
            }
         }
         else if(nChar == VK_RIGHT)
         {
            ChattyPost *pPost = GetDocument()->FindPost(GetCurrentId());
            if(pPost != NULL)
            {
               ChattyPost *pRoot = pPost->GetRoot();
               if(pRoot != NULL)
               {
                  pRoot->SetIndentOffset( pRoot->GetIndentOffset() - 1);
                  InvalidateEverything();
               }
            }
         }
         else if(nChar == VK_NEXT || 
                (nChar == VK_SPACE &&
                 !bShiftPressed))
         {
            CancelInertiaPanning();
            m_brakes = false;
            RECT DeviceRectangle;
            GetClientRect(&DeviceRectangle);
            m_gotopos += ((DeviceRectangle.bottom - DeviceRectangle.top) - 20 - m_banneroffset);
            MakePosLegal();
            InvalidateEverythingPan();
            MakeMousePointInvalid();
         }
         else if(nChar == VK_PRIOR ||
                (nChar == VK_SPACE &&
                 bShiftPressed))
         {
            CancelInertiaPanning();
            m_brakes = false;
            RECT DeviceRectangle;
            GetClientRect(&DeviceRectangle);
            m_gotopos -= ((DeviceRectangle.bottom - DeviceRectangle.top) - 20 - m_banneroffset);
            MakePosLegal();
            InvalidateEverythingPan();
            MakeMousePointInvalid();
         }
         else if(nChar == VK_HOME)
         {
            CancelInertiaPanning();
            m_brakes = false;
            m_gotopos = 0;
            InvalidateEverythingPan();
            MakeMousePointInvalid();
         }
         else if(nChar == VK_END)
         {
            CancelInertiaPanning();
            m_brakes = false;
            RECT DeviceRectangle;
            GetClientRect(&DeviceRectangle);
            m_gotopos = GetDocument()->GetHeight() - (DeviceRectangle.bottom - DeviceRectangle.top);
            MakePosLegal();
            InvalidateEverythingPan();
            MakeMousePointInvalid();
         }
      }
   }

   CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CLampView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   if(m_bModToolIsUp)
   {
      m_bModToolIsUp = false;
      InvalidateEverything();
   }

   if(!GetDocument()->IsBusy())
   {
      if(m_pReplyDlg != NULL &&
         m_pReplyDlg->GetHasFocus())
      {
         m_pReplyDlg->OnChar(nChar, nRepCnt, nFlags);
         InvalidateEverything();
      }
      else
      {
         if(GetCurrentId() != 0)
         {
            ChattyPost *pPost = GetDocument()->FindPost(GetCurrentId());
            if(pPost != NULL)
            {            
               if(nChar == 'a' ||
                  nChar == 'A'||
                  nChar == 'z' ||
                  nChar == 'Z')
               {
                  if(nChar == 'a' ||
                     nChar == 'A')
                  {
                     // select prev reply
                     SetCurrentId(pPost->GetPrevReply());
                     m_textselectionpost = 0;
                     m_selectionstart = 0;
                     m_selectionend = 0;
                  }
                  else if(nChar == 'z' ||
                          nChar == 'Z')
                  {
                     // select next reply
                     SetCurrentId(pPost->GetNextReply());
                     m_textselectionpost = 0;
                     m_selectionstart = 0;
                     m_selectionend = 0;
                  }
                  // force a draw so that positions are updated
                  MakeCurrentPostLegal();
               }
            }
         }

         if(GetDocument()->GetDataType() != DDT_SHACKMSG &&
            (nChar == 'j' ||
             nChar == 'J' ||
             nChar == 'k' ||
             nChar == 'K' ||
             nChar == 's' ||
             nChar == 'S' ||
             nChar == 'x' ||
             nChar == 'X'))
         {
            if(nChar == 'j' ||
               nChar == 'J' ||
               nChar == 'x' ||
               nChar == 'X')
            {
               ChattyPost *pParent = NULL;
               ChattyPost *pPost = GetDocument()->FindPost(GetCurrentId());
               if(pPost != NULL)
               {            
                  pParent = pPost;
                  while(pParent->GetParent() != NULL) pParent = pParent->GetParent();
               }
               // select prev reply
               SetCurrentId(GetDocument()->GetNextRoot(pParent));
               m_textselectionpost = 0;
               m_selectionstart = 0;
               m_selectionend = 0;
            }
            else if(nChar == 'k' ||
                    nChar == 'K' ||
                    nChar == 's' ||
                    nChar == 'S')
            {
               ChattyPost *pParent = NULL;
               ChattyPost *pPost = GetDocument()->FindPost(GetCurrentId());
               if(pPost != NULL)
               {            
                  pParent = pPost;
                  while(pParent->GetParent() != NULL) pParent = pParent->GetParent();
               }
               // select next reply
               SetCurrentId(GetDocument()->GetPrevRoot(pParent));
               m_textselectionpost = 0;
               m_selectionstart = 0;
               m_selectionend = 0;
            }

            MakeCurrentPostLegal(true);
         }
         else if((nChar == 'r' ||
                  nChar == 'R') &&
                 GetCurrentId() != 0 &&
                 (GetDocument()->GetDataType() == DDT_STORY ||
                 GetDocument()->GetDataType() == DDT_THREAD) &&
                 theApp.HaveLogin())
         {
            if(m_pReplyDlg != NULL)
            {
               ChattyPost *post = GetDocument()->FindPost(m_pReplyDlg->GetReplyId());
               if(post != NULL)
               {
                  post->SetReplyDlg(NULL);
               }

               post = GetDocument()->FindPost(GetCurrentId());
               if(post != NULL)
               {
                  m_pReplyDlg->SetReplyId(GetCurrentId());
                  post->SetReplyDlg(m_pReplyDlg);
               }                                 
               InvalidateEverything();
            }
            else 
            {
               ChattyPost *post = GetDocument()->FindPost(GetCurrentId());
               if(post != NULL)
               {
                  m_textselectionpost = 0;
                  m_pReplyDlg = new CReplyDlg(this);
                  m_pReplyDlg->SetDoc(GetDocument());
                  m_pReplyDlg->SetReplyId(GetCurrentId());
                  post->SetReplyDlg(m_pReplyDlg);
                  
                  RECT DeviceRectangle;
                  GetClientRect(&DeviceRectangle);

                  int top = post->GetPos() + post->GetHeight();
                  int bottom = top + m_pReplyDlg->GetHeight();
                  
                  if(bottom > DeviceRectangle.bottom)
                  {
                     m_gotopos = m_pos + (bottom - DeviceRectangle.bottom) + 20;
                     DrawEverythingToBuffer();
                     MakePosLegal();
                  }
                  else if(top < DeviceRectangle.top)
                  {
                     m_gotopos = m_pos - (DeviceRectangle.top - top) - 20;
                     DrawEverythingToBuffer();
                     MakePosLegal();
                  }
                  InvalidateEverything();
               }
            }
         }
         else if((nChar == 'q' ||
                  nChar == 'Q') &&
                 GetCurrentId() != 0)
         {
            ChattyPost *post = GetDocument()->FindPost(GetCurrentId());
            if(post != NULL)
            {
               post->Despoil();
               InvalidateEverything();
            }
         }
         else if((nChar == 'f' ||
                  nChar == 'F') &&
                  (GetDocument()->GetDataType() == DDT_STORY ||
                 GetDocument()->GetDataType() == DDT_THREAD) &&
                 GetCurrentId() != 0)
         {
            ChattyPost *post = GetDocument()->FindPost(GetCurrentId());
            if(post != NULL)
            {
               GetDocument()->RefreshThread(GetDocument()->GetRootId(GetCurrentId()), GetCurrentId());
            }
         }
      }
   }

   CView::OnChar(nChar, nRepCnt, nFlags);
}

BOOL CLampView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{  
   if(GetDocument()->IsBusy())
   {
      SetCursor(::LoadCursor(NULL, IDC_WAIT));
   }
   else
   {
      if(nHitTest == HTCLIENT)
      {
         if(m_hotspot != NULL)
         {
            switch(m_hotspottype)
            {
            case HST_REFRESH: 
            case HST_REPLIESTOROOTPOSTHINT:
            case HST_OPENINTAB:
            case HST_PIN:
            case HST_REPLYPREVIEW:
            case HST_CREATEREPLY:
            case HST_REPLY_TO_MESSAGE:
            case HST_FORWARD_MESSAGE:
            case HST_DELETE_MESSAGE:
            case HST_COLLAPSEPOST:
            case HST_EXPAND:
            case HST_CLOSEREPLYDLG:
            case HST_CLOSE_MESSAGE:
            case HST_CLOSEREPLY: 
            case HST_POST:
            case HST_SEND:
            case HST_TAG_RED:
            case HST_TAG_GREEN:
            case HST_TAG_BLUE:
            case HST_TAG_YELLOW:
            case HST_TAG_OLIVE:
            case HST_TAG_LIME:
            case HST_TAG_ORANGE:
            case HST_TAG_PINK:
            case HST_TAG_ITALICS:
            case HST_TAG_BOLD:
            case HST_TAG_QUOTE:
            case HST_TAG_SAMPLE:
            case HST_TAG_UNDERLINE:
            case HST_TAG_STRIKE:
            case HST_TAG_SPOILER:
            case HST_TAG_CODE:
            case HST_POSTAS:
            case HST_MSG_INFO:
            case HST_NEWTHREAD:
            case HST_COMPOSE_MESSAGE:
            case HST_REFRESHSTORY:
            case HST_LINK:
            case HST_IMAGE_LINK:
            case HST_IMAGE:
            case HST_THUMB:
            case HST_PREV_PAGE:
            case HST_NEXT_PAGE:
            case HST_PAGE:
            case HST_AUTHOR:
            case HST_AUTHORPREVIEW:
            case HST_SPELL_SUGGESTION:
            case HST_PREVIEW:
            case HST_UNPREVIEW:
            case HST_LOLTAG:
            case HST_INFTAG:
            case HST_UNFTAG:
            case HST_TAGTAG:
            case HST_NEW_MESSAGES_NOTE:
            case HST_WTFTAG:
            case HST_LIGHTNINGBOLT:
            case HST_MOD_TOOL:
            case HST_MOD_TOOL_ITEM:
               {
                  SetCursor(::LoadCursor(NULL, IDC_HAND));
               }
               break;
            case HST_SPOILER:
               {
                  SetCursor(::LoadCursor(NULL, IDC_HELP));
               }
               break;
            case HST_TEXT:
            case HST_REPLYTEXT:
               {
                  SetCursor(::LoadCursor(NULL, IDC_IBEAM));
               }
               break;
            case HST_NULL_BACKGROUND:
            default:
               SetCursor(::LoadCursor(NULL, IDC_ARROW) );
               break;
            }
         }
         else
         {
            SetCursor(::LoadCursor(NULL, IDC_ARROW) );
         }
      }
      else
      {
         CView::OnSetCursor(pWnd, nHitTest, message);
      }
   }
   
   return TRUE;
}

void CLampView::OnEditUndo()
{
   if(m_pReplyDlg != NULL &&
      m_pReplyDlg->GetHasFocus() &&
      !m_pReplyDlg->AreSuggestionsUp())
   {
      if(m_pReplyDlg->CanUndo())
      {
         m_pReplyDlg->Undo();
         InvalidateEverything();
      }
   }
}

void CLampView::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
   if(m_pReplyDlg != NULL &&
      m_pReplyDlg->GetHasFocus() &&
      !m_pReplyDlg->AreSuggestionsUp())
   {
      if(m_pReplyDlg->CanUndo())
      {
         pCmdUI->Enable(TRUE);
      }
      else
      {
         pCmdUI->Enable(FALSE);
      }
   }
   else
   {
      pCmdUI->Enable(FALSE);
   }
}

void CLampView::OnEditCut()
{
   if(m_pReplyDlg != NULL &&
      m_pReplyDlg->GetHasFocus() &&
      !m_pReplyDlg->AreSuggestionsUp())
   {
      if(m_pReplyDlg->HasSelection())
      {
         UCString selectedtext;
         m_pReplyDlg->GetSelectedText(selectedtext);
         selectedtext.PushToClipboard();
         m_pReplyDlg->DeleteSelected();
         InvalidateEverything();
      }
   }
}

void CLampView::OnUpdateEditCut(CCmdUI *pCmdUI)
{
   if(m_pReplyDlg != NULL &&
      m_pReplyDlg->GetHasFocus() &&
      !m_pReplyDlg->AreSuggestionsUp())
   {
      if(m_pReplyDlg->HasSelection())
      {
         pCmdUI->Enable(TRUE);
      }
      else
      {
         pCmdUI->Enable(FALSE);
      }
   }
   else
   {
      pCmdUI->Enable(FALSE);
   }
}

void CLampView::OnEditCopy()
{
   if(m_pReplyDlg != NULL &&
      m_pReplyDlg->GetHasFocus() &&
      !m_pReplyDlg->AreSuggestionsUp())
   {
      if(m_pReplyDlg->HasSelection())
      {
         UCString selectedtext;
         m_pReplyDlg->GetSelectedText(selectedtext);
         selectedtext.PushToClipboard();
      }
   }
   else if(m_textselectionpost != 0 &&
           m_selectionstart != m_selectionend)
   {
      ChattyPost *pPost = GetDocument()->FindPost(m_textselectionpost);
      if(pPost != NULL)
      {
         UCString selectedtext;
         pPost->GetSelectedText(m_selectionstart, m_selectionend, selectedtext);
         selectedtext.PushToClipboard();
      }
   }
}

void CLampView::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
   if(m_pReplyDlg != NULL &&
      m_pReplyDlg->GetHasFocus() &&
      !m_pReplyDlg->AreSuggestionsUp())
   {
      if(m_pReplyDlg->HasSelection())
      {
         pCmdUI->Enable(TRUE);
      }
      else
      {
         pCmdUI->Enable(FALSE);
      }
   }
   else if(m_textselectionpost != 0 &&
          m_selectionstart != m_selectionend)
   {
      pCmdUI->Enable(TRUE);
   }
   else
   {
      pCmdUI->Enable(FALSE);
   }
}

void CLampView::OnEditPaste()
{
   if(m_pReplyDlg != NULL &&
      m_pReplyDlg->GetHasFocus() &&
      !m_pReplyDlg->AreSuggestionsUp())
   {
      UCString fromclip;
      fromclip.PullFromClipboard();
      fromclip.Replace(L"\r\n",L"\n");
      m_pReplyDlg->InsertString(fromclip);
      InvalidateEverything();
   }
}

void CLampView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
   if(m_pReplyDlg != NULL &&
      m_pReplyDlg->GetHasFocus() &&
      !m_pReplyDlg->AreSuggestionsUp())
   {
      pCmdUI->Enable(TRUE);
   }
   else
   {
      pCmdUI->Enable(FALSE);
   }
}

void CLampView::OnEditPasteLastPost()
{
   if(m_pReplyDlg != NULL &&
      m_pReplyDlg->GetHasFocus() &&
      !m_pReplyDlg->AreSuggestionsUp())
   {
      m_pReplyDlg->SelectAll();
      m_pReplyDlg->InsertString(theApp.GetLastPost());
      InvalidateEverything();
   }
}

void CLampView::OnUpdateEditPasteLastPost(CCmdUI *pCmdUI)
{
   if(m_pReplyDlg != NULL &&
      m_pReplyDlg->GetHasFocus() &&
      !m_pReplyDlg->AreSuggestionsUp() &&
      !theApp.GetLastPost().IsEmpty())
   {
      pCmdUI->Enable(TRUE);
   }
   else
   {
      pCmdUI->Enable(FALSE);
   }
}


void CLampView::OnEditFindtext()
{
   if(m_pReplyDlg == NULL)
   {
      ReleaseCapture();
      m_pFindDlg = new CFindTextDlg(this);

      m_pFindDlg->m_pView = this;
      m_pFindDlg->m_textselectionpost = m_textselectionpost;
      m_pFindDlg->m_selectionstart = m_selectionstart;
      m_pFindDlg->m_selectionend = m_selectionend;
      
      m_pFindDlg->Create(IDD_FINDTEXT_DIALOG,this);
      m_pFindDlg->ShowWindow(SW_SHOW);
   }
}

void CLampView::OnUpdateEditFindtext(CCmdUI *pCmdUI)
{
   if(m_pReplyDlg == NULL)
   {
      pCmdUI->Enable(TRUE);
   }
   else
   {
      pCmdUI->Enable(FALSE);
   }
}

void CLampView::OnEditRefresh()
{
   if(m_pReplyDlg == NULL &&
      !GetDocument()->IsBusy())
   {
      if(GetDocument()->Refresh())
      {
         if(GetDocument()->GetDataType() != DDT_THREAD)
         {
            m_history_list.clear();
            m_history_it = m_history_list.end();
            m_current_id = 0;
            m_gotopos = 0;
            m_textselectionpost = 0;
            m_selectionstart = 0;
            m_selectionend = 0;
         }
      }
      InvalidateEverything();
   }
}

void CLampView::OnUpdateEditRefresh(CCmdUI *pCmdUI)
{
   if(m_pReplyDlg == NULL &&
      !GetDocument()->IsBusy())
   {
      pCmdUI->Enable(TRUE);
   }
   else
   {
      pCmdUI->Enable(FALSE);
   }
}


void CLampView::OnEditNewthread()
{
   if(m_pReplyDlg == NULL && 
      GetDocument()->GetDataType() == DDT_STORY)
   {
      if(theApp.HaveLogin())
      {
         m_textselectionpost = 0;
         m_pReplyDlg = new CReplyDlg(this);
         m_pReplyDlg->SetDoc(GetDocument());
         m_pReplyDlg->SetReplyId(0);
         GetDocument()->SetReplyDlg(m_pReplyDlg);
         m_gotopos = m_pos = 0;
         InvalidateEverything();
      }
   }
}

void CLampView::OnUpdateEditNewthread(CCmdUI *pCmdUI)
{
   if(m_pReplyDlg == NULL && 
      GetDocument()->GetDataType() == DDT_STORY)
   {
      pCmdUI->Enable(TRUE);
   }
   else
   {
      pCmdUI->Enable(FALSE);
   }
}


void CLampView::OnCopyLink()
{
   for(size_t i = 0; i < m_hotspots.size(); i++)
   {
      if(m_rbuttondownpoint.x >= m_hotspots[i].m_spot.left &&
         m_rbuttondownpoint.x < m_hotspots[i].m_spot.right &&
         m_rbuttondownpoint.y >= m_hotspots[i].m_spot.top &&
         m_rbuttondownpoint.y < m_hotspots[i].m_spot.bottom &&
         (m_hotspots[i].m_type == HST_LINK ||
          m_hotspots[i].m_type == HST_IMAGE_LINK ||
          m_hotspots[i].m_type == HST_IMAGE ||
          m_hotspots[i].m_type == HST_THUMB ||
          m_hotspots[i].m_type == HST_OPENINTAB ||
          m_hotspots[i].m_type == HST_PIN ||
          m_hotspots[i].m_type == HST_REFRESH ||
          m_hotspots[i].m_type == HST_REPLIESTOROOTPOSTHINT ||
          (m_hotspots[i].m_type == HST_TEXT && GetDocument()->GetDataType() != DDT_SHACKMSG)))
      {
         ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
         if(pPost != NULL)
         {
            UCString link;
            if(m_hotspots[i].m_type == HST_LINK)
            {
               pPost->GetLink(m_rbuttondownpoint.x, m_rbuttondownpoint.y, link);
            }
            else if(m_hotspots[i].m_type == HST_IMAGE_LINK)
            {
               pPost->GetImageLink(m_rbuttondownpoint.x, m_rbuttondownpoint.y, link);
            }
            else if(m_hotspots[i].m_type == HST_IMAGE)
            {
               pPost->GetLinkToImage(m_rbuttondownpoint.x, m_rbuttondownpoint.y, link);
            }
            else if(m_hotspots[i].m_type == HST_THUMB)
            {
               pPost->GetLinkToThumb(m_rbuttondownpoint.x, m_rbuttondownpoint.y, link);
            }
            else
            {
               unsigned int id = GetDocument()->GetID(m_hotspots[i].m_id);

               link = L"http://www.shacknews.com/chatty?id=";
               link += id;
            }

            link.PushToClipboard();
         }
         break;
      }
   }
}

void CLampView::OnUpdateCopyLink(CCmdUI *pCmdUI)
{
   BOOL enable = FALSE;
   for(size_t i = 0; i < m_hotspots.size(); i++)
   {
      if(m_rbuttondownpoint.x >= m_hotspots[i].m_spot.left &&
         m_rbuttondownpoint.x < m_hotspots[i].m_spot.right &&
         m_rbuttondownpoint.y >= m_hotspots[i].m_spot.top &&
         m_rbuttondownpoint.y < m_hotspots[i].m_spot.bottom &&
         (m_hotspots[i].m_type == HST_LINK ||
          m_hotspots[i].m_type == HST_IMAGE_LINK ||
          m_hotspots[i].m_type == HST_IMAGE ||
          m_hotspots[i].m_type == HST_THUMB ||
          m_hotspots[i].m_type == HST_OPENINTAB ||
          m_hotspots[i].m_type == HST_PIN ||
          m_hotspots[i].m_type == HST_REFRESH ||
          m_hotspots[i].m_type == HST_REPLIESTOROOTPOSTHINT ||
          (m_hotspots[i].m_type == HST_TEXT && GetDocument()->GetDataType() != DDT_SHACKMSG)))
      {
         ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
         if(pPost != NULL)
         {
            enable = TRUE;
         }
         break;
      }
   }
   pCmdUI->Enable(enable);
}

void CLampView::OnLaunchLink()
{
   for(size_t i = 0; i < m_hotspots.size(); i++)
   {
      if(m_rbuttondownpoint.x >= m_hotspots[i].m_spot.left &&
         m_rbuttondownpoint.x < m_hotspots[i].m_spot.right &&
         m_rbuttondownpoint.y >= m_hotspots[i].m_spot.top &&
         m_rbuttondownpoint.y < m_hotspots[i].m_spot.bottom &&
         (m_hotspots[i].m_type == HST_LINK ||
          m_hotspots[i].m_type == HST_IMAGE_LINK ||
          m_hotspots[i].m_type == HST_IMAGE ||
          m_hotspots[i].m_type == HST_THUMB ||
          m_hotspots[i].m_type == HST_OPENINTAB ||
          m_hotspots[i].m_type == HST_PIN ||
          m_hotspots[i].m_type == HST_REFRESH))
      {
         ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
         if(pPost != NULL)
         {
            UCString link;
            if(m_hotspots[i].m_type == HST_LINK)
            {
               pPost->GetLink(m_rbuttondownpoint.x, m_rbuttondownpoint.y, link);
            }
            else if(m_hotspots[i].m_type == HST_IMAGE_LINK)
            {
               pPost->GetImageLink(m_rbuttondownpoint.x, m_rbuttondownpoint.y, link);
            }
            else if(m_hotspots[i].m_type == HST_IMAGE)
            {
               pPost->GetLinkToImage(m_rbuttondownpoint.x, m_rbuttondownpoint.y, link);
            }
            else if(m_hotspots[i].m_type == HST_THUMB)
            {
               pPost->GetLinkToThumb(m_rbuttondownpoint.x, m_rbuttondownpoint.y, link);
            }
            else
            {
               unsigned int id = GetDocument()->GetID(m_hotspots[i].m_id);

               link = L"http://www.shacknews.com/chatty?id=";
               link += id;
            }

            ShellExecuteW(NULL,
                    L"open",
                    link,
                    NULL,
                    NULL,
                    SW_SHOW);
         }
         break;
      }
   }
}

void CLampView::OnUpdateLaunchLink(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampView::OnViewIncreasefontsize()
{
   theApp.SetTextScaler(theApp.GetTextScaler() * 1.1f);
}

void CLampView::OnUpdateViewIncreasefontsize(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampView::OnViewDecreasefontsize()
{
   theApp.SetTextScaler(theApp.GetTextScaler() * 0.909090909090f);
}

void CLampView::OnUpdateViewDecreasefontsize(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampView::OnViewResetfontsize()
{
   theApp.SetTextScaler(1.0f);
}

void CLampView::OnUpdateViewResetfontsize(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampView::OnTimer(UINT nIDEvent) 
{
   if(nIDEvent == ANIM_TIMER)
   {
      if(m_bAnimating)
      {
         m_frame++;
         if(m_frame >= 8) 
            m_frame = 0;
         InvalidateHotspots();
      }
   }
   else if(nIDEvent == PREVIEW_TIMER)
   {
      if(m_hotspots.size() == 0)
      {
         SetHoverPreviewId(m_PREVIEW_TIMER_id);
      }
      else
      {
         for(size_t i = 0; i < m_hotspots.size(); i++)
         {
            if(m_hotspots[i].m_type == HST_REPLYPREVIEW &&
               m_hotspots[i].m_id != 0 &&
               m_mousepoint.x >= m_hotspots[i].m_spot.left &&
               m_mousepoint.x < m_hotspots[i].m_spot.right &&
               m_mousepoint.y >= m_hotspots[i].m_spot.top &&
               m_mousepoint.y < m_hotspots[i].m_spot.bottom)
            {
               SetHoverPreviewId(m_hotspots[i].m_id);
               break;
            }
         }
      }

      KillTimer(PREVIEW_TIMER);
      m_PREVIEW_TIMER_id = 0;
   }
   else
   {
      if(m_pReplyDlg != NULL)
      {
         m_pReplyDlg->OnTimer(nIDEvent);
      }
   }
}

   
void CLampView::OnKillFocus(CWnd* pNewWnd)
{
   if(theApp.m_bDockedMode)
   {
      if(!theApp.m_bCollapsed && 
         pNewWnd->GetTopLevelFrame() != theApp.GetMainWnd() &&
         theApp.GetMainWnd() != pNewWnd)
      {
         theApp.m_bCollapsed = true;

         if(theApp.m_pDockTab != NULL)
         {
            RECT framerect;
            ((CMainFrame*)theApp.GetMainWnd())->GetWindowRect(&framerect);
            RECT docktabrect;
            docktabrect.left = ((framerect.right + framerect.left) / 2) - (theApp.GetDockTabImage()->GetWidth() / 2);
            docktabrect.right = docktabrect.left + theApp.GetDockTabImage()->GetWidth();
            docktabrect.top = 0;
            docktabrect.bottom = theApp.GetDockTabImage()->GetHeight();
            theApp.m_pDockTab->MoveWindow(&docktabrect);

            theApp.m_pDockTab->ShowWindow(SW_SHOW);
         }
         
         theApp.GetMainWnd()->ShowWindow(SW_HIDE);
      }
      
      if(pNewWnd != NULL)
      {
         HWND hWnd = pNewWnd->GetSafeHwnd();

         AttachThreadInput(GetWindowThreadProcessId(::GetForegroundWindow(),NULL),GetCurrentThreadId(),TRUE);
               
         ::SetForegroundWindow(hWnd);	
         ::SetFocus(hWnd);
         
         AttachThreadInput(GetWindowThreadProcessId(::GetForegroundWindow(),NULL),GetCurrentThreadId(),FALSE);
      }
   }
}

void CLampView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
   ((CMainFrame*)theApp.GetMainWnd())->CloseFindDlg();
   if(bActivate)
   {
      ((CMainFrame*)theApp.GetMainWnd())->SetActiveLampView(this);
      InvalidateEverything();
   }
}


void CLampView::OnLoadAllImages()
{
   ChattyPost *pPost = GetDocument()->FindPost(m_rbuttonmenufromid);
   if(pPost != NULL)
   {
      CWaitCursor wait;
      pPost->LoadAllImageLinks();
      InvalidateEverything();
   }
}

void CLampView::OnUpdateLoadAllImages(CCmdUI *pCmdUI)
{
   if(m_rbuttonmenufromid != 0)
   {
      pCmdUI->Enable(TRUE);
   }
   else
   {
      pCmdUI->Enable(FALSE);
   }
}

void CLampView::OnCloseAllImages()
{
   ChattyPost *pPost = GetDocument()->FindPost(m_rbuttonmenufromid);
   if(pPost != NULL)
   {
      CWaitCursor wait;
      pPost->CloseAllImageLinks();
      InvalidateEverything();
   }
}

void CLampView::OnUpdateCloseAllImages(CCmdUI *pCmdUI)
{
   if(m_rbuttonmenufromid != 0)
   {
      pCmdUI->Enable(TRUE);
   }
   else
   {
      pCmdUI->Enable(FALSE);
   }
}


void CLampView::OnBookmarkAdd()
{
   if(GetDocument() != NULL &&
      GetDocument()->GetDataType() == DDT_THREAD)
   {
      ChattyPost *pPost = GetDocument()->GetThreadPost();
      if(pPost != NULL)
      {
         CShackBookmark bookmark;
         bookmark.m_root_id = pPost->GetId();
         bookmark.m_current_id = GetCurrentId();

         if(GetCurrentId() != 0)
         {
            pPost = GetDocument()->FindPost(GetCurrentId());
         }

         bookmark.m_category = pPost->GetCategory();
         bookmark.m_author = pPost->GetAuthor();
         pPost->GetTitle(bookmark.m_title);

         CBookmarkDlg bmdlg;
         bmdlg.m_bEditingExistingBM = false;
         bmdlg.m_Title = bookmark.m_title;

         m_dlgup = true;
         if(bmdlg.DoModal() == IDOK)
         {
            bookmark.m_title = bmdlg.m_Title;
            theApp.AddBookmark(bookmark);
            InvalidateEverything();
         }
         else
         {
            // dont' add it
         }
         m_dlgup = false;
      }
   }
}

void CLampView::OnUpdateBookmarkAdd(CCmdUI *pCmdUI)
{
   if(GetDocument() != NULL &&
      GetDocument()->GetDataType() == DDT_THREAD)
   {
      pCmdUI->Enable(TRUE);
   }
   else
   {
      pCmdUI->Enable(FALSE);
   }
}

void CLampView::OnBookmarkThisPost()
{
   if(m_rbuttonmenufromid != 0 &&
     (GetDocument()->GetDataType() == DDT_STORY ||
      GetDocument()->GetDataType() == DDT_THREAD))
   {
      ChattyPost *pPost = GetDocument()->FindPost(m_rbuttonmenufromid);
      if(pPost != NULL)
      {
         CShackBookmark bookmark;
         bookmark.m_root_id = GetDocument()->GetRootId(pPost->GetId());
         bookmark.m_current_id = pPost->GetId();
         bookmark.m_category = pPost->GetCategory();
         bookmark.m_author = pPost->GetAuthor();
         pPost->GetTitle(bookmark.m_title);

         CBookmarkDlg bmdlg;
         bmdlg.m_bEditingExistingBM = false;
         bmdlg.m_Title = bookmark.m_title;

         m_dlgup = true;
         if(bmdlg.DoModal() == IDOK)
         {
            bookmark.m_title = bmdlg.m_Title;
            theApp.AddBookmark(bookmark);
            InvalidateEverything();
         }
         else
         {
            // dont' add it
         }
         m_dlgup = false;
      }
   }
}

void CLampView::OnUpdateBookmarkThisPost(CCmdUI *pCmdUI)
{
   if(m_rbuttonmenufromid != 0 &&
     (GetDocument()->GetDataType() == DDT_STORY ||
      GetDocument()->GetDataType() == DDT_THREAD))
   {
      pCmdUI->Enable(TRUE);
   }
   else
   {
      pCmdUI->Enable(FALSE);
   }
}


void CLampView::MakeCurrentPosVisible()
{
   if(GetCurrentId() != 0)
   {
      ChattyPost *post = GetDocument()->FindPost(GetCurrentId());
      if(post != NULL)
      {
         RECT DeviceRectangle;
         GetClientRect(&DeviceRectangle);
         int width = DeviceRectangle.right - DeviceRectangle.left;
         int height = DeviceRectangle.bottom - DeviceRectangle.top;

         if(width != m_backbuffer1.GetWidth() ||
            height != m_backbuffer1.GetHeight())
         {
            m_backbuffer1.Resize(width, height);
            m_backbuffer2.Resize(width, height);
            m_whitebuffer.Resize(width, height);
            m_whitebuffer.Fill(GetRValue(theApp.GetHoverColor()),GetGValue(theApp.GetHoverColor()),GetBValue(theApp.GetHoverColor()));
            theApp.UpdateTabSizes();
         }
         // force a draw so that positions are updated
         m_gotopos = m_pos = 0;
         DrawEverythingToBuffer();
         int top = post->GetPos();
         int newpos;

         // if near the bottom, then back up a bit
         int threadheight = GetDocument()->GetHeight() - ((height / 8) * 3);
         if(threadheight - top < (height / 2))
         {
            newpos = threadheight - height;
         }
         else
         {
            newpos = top - (height / 2);
         }

         m_gotopos = newpos;
      }

      MakePosLegal();
   }
}


void CLampView::OnPinning()
{
   theApp.SetPinningInStories(!theApp.IsPinningInStories());
   InvalidateEverything();

   if(!theApp.IsPinningInStories())
   {
      theApp.ClearAllPinnedThreads();
   }
}

void CLampView::OnUpdatePinning(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.IsPinningInStories())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnHideCollapsedThreads()
{
   theApp.SetHideCollapsedPosts(!theApp.HideCollapsedPosts());
   InvalidateEverything();
}

void CLampView::OnUpdateHideCollapsedThreads(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.HideCollapsedPosts())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnInfinatePaging()
{
   theApp.SetInfinatePaging(!theApp.InfinatePaging());
   InvalidateEverything();
}

void CLampView::OnUpdateInfinatePaging(CCmdUI *pCmdUI)
{
   if(theApp.UseShack())
   {
      pCmdUI->Enable(TRUE);

      if(theApp.InfinatePaging())
      {
         pCmdUI->SetCheck(TRUE);
      }
      else
      {
         pCmdUI->SetCheck(FALSE);
      }
   }
   else
   {
      pCmdUI->Enable(FALSE);
   }
}

void CLampView::OnGotoNewPost()
{
   theApp.SetGotoNewPost(!theApp.GotoNewPost());
   InvalidateEverything();
}

void CLampView::OnUpdateGotoNewPost(CCmdUI *pCmdUI)
{
   if(theApp.UseShack())
   {
      pCmdUI->Enable(TRUE);

      if(theApp.GotoNewPost())
      {
         pCmdUI->SetCheck(TRUE);
      }
      else
      {
         pCmdUI->SetCheck(FALSE);
      }
   }
   else
   {
      pCmdUI->Enable(FALSE);
   }
}


void CLampView::OnClearAllPinnings()
{
   theApp.ClearAllPinnedThreads();
}

void CLampView::OnUpdateClearAllPinnings(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampView::OnDoublePageStory()
{
   theApp.SetDoublePageStory(!theApp.IsDoublePageStory());
}

void CLampView::OnUpdateDoublePageStory(CCmdUI *pCmdUI)
{
   if(theApp.UseShack())
   {
      pCmdUI->Enable(FALSE);
   }
   else
   {
      pCmdUI->Enable(TRUE);

      if(theApp.IsDoublePageStory())
      {
         pCmdUI->SetCheck(TRUE);
      }
      else
      {
         pCmdUI->SetCheck(FALSE);
      }
   }
}

void CLampView::OnOpenInTab()
{
   for(size_t i = 0; i < m_hotspots.size(); i++)
   {
      if(GetDocument()->GetDataType() != DDT_THREAD &&
         m_rbuttondownpoint.x >= m_hotspots[i].m_spot.left &&
         m_rbuttondownpoint.x < m_hotspots[i].m_spot.right &&
         m_rbuttondownpoint.y >= m_hotspots[i].m_spot.top &&
         m_rbuttondownpoint.y < m_hotspots[i].m_spot.bottom &&
         (m_hotspots[i].m_type == HST_OPENINTAB ||
          m_hotspots[i].m_type == HST_PIN ||
          m_hotspots[i].m_type == HST_REFRESH))
      {
         unsigned int id = GetDocument()->GetID(m_hotspots[i].m_id);
         unsigned int rootid = GetDocument()->GetRootId(id);
         UCString path;
         if(rootid != 0)
         {
            path += rootid;
         }
         else
         {
            path += id;
         }
         path += L'_';
         path += id;
         ReleaseCapture();
         theApp.OpenDocumentFile(path);
      }
   }
}

void CLampView::OnUpdateOpenInTab(CCmdUI *pCmdUI)
{
   BOOL enable = FALSE;
   for(size_t i = 0; i < m_hotspots.size(); i++)
   {
      if(GetDocument()->GetDataType() != DDT_THREAD &&
         m_rbuttondownpoint.x >= m_hotspots[i].m_spot.left &&
         m_rbuttondownpoint.x < m_hotspots[i].m_spot.right &&
         m_rbuttondownpoint.y >= m_hotspots[i].m_spot.top &&
         m_rbuttondownpoint.y < m_hotspots[i].m_spot.bottom &&
         (m_hotspots[i].m_type == HST_OPENINTAB ||
          m_hotspots[i].m_type == HST_PIN ||
          m_hotspots[i].m_type == HST_REFRESH))
      {
         enable = TRUE;
      }
   }
   pCmdUI->Enable(enable);
}

void CLampView::OnHelpWiki()
{
   UCString link(L"http://shackwiki.com/wiki/Lamp");
   theApp.OpenShackLink(link);
}

void CLampView::OnUpdateHelpWiki(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampView::OnShowLOLButtons()
{
   theApp.SetShowLOLButtons(!theApp.ShowLOLButtons());
   InvalidateEverything();
}

void CLampView::OnUpdateShowLOLButtons(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.ShowLOLButtons())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnCheckSpelling()
{
   theApp.SetIsSpellCheckerEnabled(!theApp.IsSpellCheckerEnabled());
   InvalidateEverything();
}

void CLampView::OnUpdateCheckSpelling(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.IsSpellCheckerEnabled())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnFilterNWS()
{
   theApp.SetEnableNWS(!theApp.EnableNWS());
}

void CLampView::OnUpdateFilterNWS(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.EnableNWS())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnFilterINF()
{
   theApp.SetEnableINF(!theApp.EnableINF());
}

void CLampView::OnUpdateFilterINF(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.EnableINF())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnFilterOFFTOPIC()
{
   theApp.SetEnableOffTopic(!theApp.EnableOffTopic());
}

void CLampView::OnUpdateFilterOFFTOPIC(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.EnableOffTopic())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnFilterSTUPID()
{
   theApp.SetEnableStupid(!theApp.EnableStupid());
}

void CLampView::OnUpdateFilterSTUPID(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.EnableStupid())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnFilterPOLITICAL()
{
   theApp.SetEnablePolitical(!theApp.EnablePolitical());
}

void CLampView::OnUpdateFilterPOLITICAL(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.EnablePolitical())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnSkinRoundShack()
{
   theApp.SetSkinFolder(L"roundshack");
}

void CLampView::OnUpdateSkinRoundShack(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.GetSkinFolder() == L"roundshack")
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnSkinSquareShack()
{
   theApp.SetSkinFolder(L"default");
}

void CLampView::OnUpdateSkinSquareShack(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.GetSkinFolder() == L"default")
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnSkinWorksafeShack()
{
   theApp.SetSkinFolder(L"worksafe");
}

void CLampView::OnUpdateSkinWorksafeShack(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.GetSkinFolder() == L"worksafe")
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnSkinCustom()
{
   ReleaseCapture();
   UCString skinfilename = L"skins\\";
   skinfilename += theApp.GetSkinFolder();
   skinfilename += L"\\skin.xml";

   UCString skinpath;
   skinpath.PathToMe(skinfilename);

   wchar_t szFilters[] = L"Skin.xml Files (*.xml)|*.xml|All Files (*.*)|*.*||";

   CFileDialog getskin(TRUE,//BOOL bOpenFileDialog,
                       L"xml",//LPCTSTR lpszDefExt = NULL,
                       skinpath,//LPCTSTR lpszFileName = NULL,
                       OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,//DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                       szFilters,//LPCTSTR lpszFilter = NULL,
                       this,//CWnd* pParentWnd = NULL,
                       0);//DWORD dwSize = 0

   if( getskin.DoModal() == IDOK )
   {
      CString pathName = getskin.GetPathName();

      const wchar_t *begin = pathName;
      const wchar_t *end = begin + pathName.GetLength();

      while(end > begin && *end != L'\\')end--;

      const wchar_t *first = end - 1;

      while(first > begin && *first != L'\\')first--;
   
      UCString foldername;
      foldername.AppendUnicodeString(first + 1, end - first - 1);

      theApp.SetSkinFolder(foldername);
   }
}

void CLampView::OnUpdateSkinCustom(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.GetSkinFolder() != L"roundshack" &&
      theApp.GetSkinFolder() != L"default" &&
      theApp.GetSkinFolder() != L"worksafe")
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnHighlightOP()
{
   theApp.SetHighlightOP(!theApp.GetHighlightOP());
   InvalidateEverything();
}

void CLampView::OnUpdateHighlightOP(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.GetHighlightOP())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnShowSettings()
{
   CSettingsDlg settingsdlg(this);
   settingsdlg.DoModal();
   InvalidateEverything();
}

void CLampView::OnUpdateShowSettings(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampView::OnNormalFont()
{
   ReleaseCapture();
   LOGFONTW lf;
   memset(&lf,0,sizeof(LOGFONTW));
   wcscpy_s(lf.lfFaceName,LF_FACESIZE,theApp.GetNormalFontName());
   lf.lfHeight = 16;

   CFontDialog fontdlg(&lf,0,NULL,this);
   if(fontdlg.DoModal() == IDOK)
   {
      CString temp = fontdlg.GetFaceName();
      theApp.SetNormalFontName(temp);
   }
}

void CLampView::OnUpdateNormalFont(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampView::OnQuotedFont()
{
   ReleaseCapture();
   LOGFONTW lf;
   memset(&lf,0,sizeof(LOGFONTW));
   wcscpy_s(lf.lfFaceName,LF_FACESIZE,theApp.GetQuotedFontName());
   lf.lfHeight = 16;

   CFontDialog fontdlg(&lf,0,NULL,this);
   if(fontdlg.DoModal() == IDOK)
   {
      CString temp = fontdlg.GetFaceName();
      theApp.SetQuotedFontName(temp);
   }
}

void CLampView::OnUpdateQuotedFont(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampView::OnCodeFont()
{
   ReleaseCapture();
   LOGFONTW lf;
   memset(&lf,0,sizeof(LOGFONTW));
   wcscpy_s(lf.lfFaceName,LF_FACESIZE,theApp.GetCodeFontName());
   lf.lfHeight = 16;

   CFontDialog fontdlg(&lf,0,NULL,this);
   if(fontdlg.DoModal() == IDOK)
   {
      CString temp = fontdlg.GetFaceName();
      theApp.SetCodeFontName(temp);
   }
}

void CLampView::OnUpdateCodeFont(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampView::OnSmallLOL()
{
   theApp.SetShowSmallLOL(!theApp.ShowSmallLOL());
   InvalidateEverything();
}

void CLampView::OnUpdateSmallLOL(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.ShowSmallLOL())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnSelectDarkBlue()
{
   theApp.SetTextSelectionColor(RGB(0,0,128));
   InvalidateEverything();
}

void CLampView::OnUpdateSelectDarkBlue(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
   if(theApp.GetTextSelectionColor() == RGB(0,0,128))
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnSelectBlue()
{
   theApp.SetTextSelectionColor(RGB(0,0,255));
   InvalidateEverything();
}

void CLampView::OnUpdateSelectBlue(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
   if(theApp.GetTextSelectionColor() == RGB(0,0,255))
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnSelectYellow()
{
   theApp.SetTextSelectionColor(RGB(255,255,0));
   InvalidateEverything();
}

void CLampView::OnUpdateSelectYellow(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
   if(theApp.GetTextSelectionColor() == RGB(255,255,0))
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnSelectPaleYellow()
{
   theApp.SetTextSelectionColor(RGB(255,255,128));
   InvalidateEverything();
}

void CLampView::OnUpdateSelectPaleYellow(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
   if(theApp.GetTextSelectionColor() == RGB(255,255,128))
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnSelectCustom()
{
   ReleaseCapture();
   CColorDialog clrdlg(theApp.GetTextSelectionColor(),CC_ANYCOLOR|CC_FULLOPEN,this);
   if(clrdlg.DoModal() == IDOK)
   {
      theApp.SetTextSelectionColor(clrdlg.GetColor());
      InvalidateEverything();
   }
}

void CLampView::OnUpdateSelectCustom(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
   if(theApp.GetTextSelectionColor() != RGB(0,0,128) &&
      theApp.GetTextSelectionColor() != RGB(0,0,255) &&
      theApp.GetTextSelectionColor() != RGB(255,255,0) &&
      theApp.GetTextSelectionColor() != RGB(255,255,128))
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnFlaredBranches()
{
   theApp.SetFlaredBranches(!theApp.FlaredBranches());
   InvalidateEverything();
}

void CLampView::OnUpdateFlaredBranches(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.FlaredBranches())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnAltPostkeys()
{
   theApp.SetAlternatePostKeys(!theApp.AlternatePostKeys());
}

void CLampView::OnUpdateAltPostkeys(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.AlternatePostKeys())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnLoadImagesInLamp()
{
   theApp.SetLoadImagesInLamp(!theApp.LoadImagesInLamp());
}

void CLampView::OnUpdateLoadImagesInLamp(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.LoadImagesInLamp())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnGoogleSelected()
{
   UCString selectedtext;

   if(m_pReplyDlg != NULL &&
      m_pReplyDlg->GetHasFocus() &&
      !m_pReplyDlg->AreSuggestionsUp())
   {
      if(m_pReplyDlg->HasSelection())
      {
         m_pReplyDlg->GetSelectedText(selectedtext);
      }
   }
   else if(m_textselectionpost != 0 &&
           m_selectionstart != m_selectionend)
   {
      ChattyPost *pPost = GetDocument()->FindPost(m_textselectionpost);
      if(pPost != NULL)
      {
         pPost->GetSelectedText(m_selectionstart, m_selectionend, selectedtext);
      }
   }

   if(!selectedtext.IsEmpty())
   {
      UCString link = L"http://www.google.com/search?q=";
      char *enc = url_encode(selectedtext.str8());
      link += enc;
      free(enc);
      theApp.OpenShackLink(link);
   }
}

void CLampView::OnUpdateGoogleSelected(CCmdUI *pCmdUI)
{
   if(m_pReplyDlg != NULL &&
      m_pReplyDlg->GetHasFocus() &&
      !m_pReplyDlg->AreSuggestionsUp())
   {
      if(m_pReplyDlg->HasSelection())
      {
         pCmdUI->Enable(TRUE);
      }
      else
      {
         pCmdUI->Enable(FALSE);
      }
   }
   else if(m_textselectionpost != 0 &&
           m_selectionstart != m_selectionend)
   {
      pCmdUI->Enable(TRUE);
   }
   else
   {
      pCmdUI->Enable(FALSE);
   }
}

void CLampView::OnGoogleSelectedWQuotes()
{
   UCString selectedtext;

   if(m_pReplyDlg != NULL &&
      m_pReplyDlg->GetHasFocus() &&
      !m_pReplyDlg->AreSuggestionsUp())
   {
      if(m_pReplyDlg->HasSelection())
      {
         m_pReplyDlg->GetSelectedText(selectedtext);
      }
   }
   else if(m_textselectionpost != 0 &&
           m_selectionstart != m_selectionend)
   {
      ChattyPost *pPost = GetDocument()->FindPost(m_textselectionpost);
      if(pPost != NULL)
      {
         pPost->GetSelectedText(m_selectionstart, m_selectionend, selectedtext);
      }
   }

   if(!selectedtext.IsEmpty())
   {
      /*
      UCString link = L"http://www.google.com/search?q=\\\"";
      char *enc = url_encode(selectedtext.str8());
      link += enc;
      link += L"\\\"";
      free(enc);
      */
      UCString link = L"http://www.google.com/search?q=%22";
      char *enc = url_encode(selectedtext.str8());
      link += enc;
      link += L"%22";
      free(enc);

      theApp.OpenShackLink(link);
   }
}

void CLampView::OnUpdateGoogleSelectedWQuotes(CCmdUI *pCmdUI)
{
   if(m_pReplyDlg != NULL &&
      m_pReplyDlg->GetHasFocus() &&
      !m_pReplyDlg->AreSuggestionsUp())
   {
      if(m_pReplyDlg->HasSelection())
      {
         pCmdUI->Enable(TRUE);
      }
      else
      {
         pCmdUI->Enable(FALSE);
      }
   }
   else if(m_textselectionpost != 0 &&
           m_selectionstart != m_selectionend)
   {
      pCmdUI->Enable(TRUE);
   }
   else
   {
      pCmdUI->Enable(FALSE);
   }
}

void CLampView::OnWikipediaSelected()
{
   UCString selectedtext;

   if(m_pReplyDlg != NULL &&
      m_pReplyDlg->GetHasFocus() &&
      !m_pReplyDlg->AreSuggestionsUp())
   {
      if(m_pReplyDlg->HasSelection())
      {
         m_pReplyDlg->GetSelectedText(selectedtext);
      }
   }
   else if(m_textselectionpost != 0 &&
           m_selectionstart != m_selectionend)
   {
      ChattyPost *pPost = GetDocument()->FindPost(m_textselectionpost);
      if(pPost != NULL)
      {
         pPost->GetSelectedText(m_selectionstart, m_selectionend, selectedtext);
      }
   }

   if(!selectedtext.IsEmpty())
   {
      UCString link = L"http://en.wikipedia.org/wiki/";
      char *enc = url_encode(selectedtext.str8());
      link += enc;
      free(enc);
      theApp.OpenShackLink(link);
   }
}

void CLampView::OnUpdateWikipediaSelected(CCmdUI *pCmdUI)
{
   if(m_pReplyDlg != NULL &&
      m_pReplyDlg->GetHasFocus() &&
      !m_pReplyDlg->AreSuggestionsUp())
   {
      if(m_pReplyDlg->HasSelection())
      {
         pCmdUI->Enable(TRUE);
      }
      else
      {
         pCmdUI->Enable(FALSE);
      }
   }
   else if(m_textselectionpost != 0 &&
           m_selectionstart != m_selectionend)
   {
      pCmdUI->Enable(TRUE);
   }
   else
   {
      pCmdUI->Enable(FALSE);
   }
}

void CLampView::OnAutoCheckInbox()
{
   theApp.SetAutoCheckInbox(!theApp.GetAutoCheckInbox());
   InvalidateEverything();
}

void CLampView::OnUpdateAutoCheckInbox(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.GetAutoCheckInbox())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnShowRootSelected()
{
   theApp.SetShowRootSelected(!theApp.ShowRootSelected());
   InvalidateEverything();
}

void CLampView::OnUpdateShowRootSelected(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.ShowRootSelected())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnShowThomWLOLS()
{
   theApp.SetShowThomWLOLS(!theApp.ShowThomWLOLS());
   InvalidateEverything();
}

void CLampView::OnUpdateShowThomWLOLS(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.ShowThomWLOLS())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnVerboseLOLPreviews()
{
   theApp.SetVerboseLOLPreviews(!theApp.VerboseLOLPreviews());
   InvalidateEverything();
}

void CLampView::OnUpdateVerboseLOLPreviews(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.VerboseLOLPreviews())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}


void CLampView::OnInvertedLOLPreviews()
{
   theApp.SetInvertedLOLPreviews(!theApp.InvertedLOLPreviews());
   InvalidateEverything();
}

void CLampView::OnUpdateInvertedLOLPreviews(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.InvertedLOLPreviews())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}


void CLampView::OnBackId()
{
   bool bFound = false;
   unsigned int back_id = 0;

   if(m_history_list.size() > 0 &&
      m_history_it != m_history_list.end())
   {
      std::list<unsigned int>::iterator it = m_history_it;
      it++;

      if(it != m_history_list.end())
      {
         back_id = (*it);
         bFound = true;
         m_history_it = it;
      }
   }

   if(bFound)
   {
      m_current_id = back_id;
      m_textselectionpost = 0;
      m_selectionstart = 0;
      m_selectionend = 0;

      if(back_id != 0)
      {
         ChattyPost *post = GetDocument()->FindPost(back_id);
         if(post != NULL)
         {         
            GetDocument()->MakePostAvailable(back_id);
            MakeCurrentPostLegal();
         }
      }

      InvalidateEverything();
   }
}

void CLampView::OnUpdateBackId(CCmdUI *pCmdUI)
{
   bool bFound = false;

   if(m_history_list.size() > 0 &&
      m_history_it != m_history_list.end())
   {
      std::list<unsigned int>::iterator it = m_history_it;
      it++;
      if(it != m_history_list.end())
      {
         bFound = true;
      }
   }

   if(bFound)
   {
      pCmdUI->Enable(TRUE);
   }
   else
   {
      pCmdUI->Enable(FALSE);
   }
}

void CLampView::OnForeId()
{
   bool bFound = false;
   unsigned int fore_id = 0;

   if(m_history_list.size() > 0 &&
      m_history_it != m_history_list.end() &&
      m_history_it != m_history_list.begin())
   {
      std::list<unsigned int>::iterator it = m_history_it;
      it--;
            
      fore_id = (*it);
      bFound = true;
      m_history_it = it;
   }

   if(bFound)
   {
      m_current_id = fore_id;
      m_textselectionpost = 0;
      m_selectionstart = 0;
      m_selectionend = 0;

      if(fore_id != 0)
      {
         ChattyPost *post = GetDocument()->FindPost(fore_id);
         if(post != NULL)
         {         
            GetDocument()->MakePostAvailable(fore_id);
            MakeCurrentPostLegal();
         }
      }

      InvalidateEverything();
   }
}

void CLampView::OnUpdateForeId(CCmdUI *pCmdUI)
{
   bool bFound = false;

   if(m_history_list.size() > 0 &&
      m_history_it != m_history_list.end() &&
      m_history_it != m_history_list.begin())
   {
      std::list<unsigned int>::iterator it = m_history_it;
      it--;
      bFound = true;
   }

   if(bFound)
   {
      pCmdUI->Enable(TRUE);
   }
   else
   {
      pCmdUI->Enable(FALSE);
   }
}


void CLampView::OnUnloadAllImages()
{
   theApp.UnloadAllImages();
}

void CLampView::OnUpdateUnloadAllImages(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampView::SendMessageDlg(CLampDoc *pDoc, const UCString &to, const UCString &subject, const UCString &shackmsg)
{
   if(m_pReplyDlg == NULL &&
      theApp.HaveLogin())
   {
      m_textselectionpost = 0;
      m_selectionstart = 0;
      m_selectionend = 0;
      m_pReplyDlg = new CReplyDlg(this);
      m_pReplyDlg->SetDoc(GetDocument());
      m_pReplyDlg->SetReplyId(0);
      m_pReplyDlg->SetIsMessage(true);
      m_pReplyDlg->SetMessageTo(to);
      m_pReplyDlg->SetMessageSubject(subject);
      m_pReplyDlg->SetReplyText(shackmsg);

      InvalidateEverything();
   }   
}

void CLampView::OnExpandPreviewsDown()
{
   theApp.SetExpandPreviewsDown(!theApp.ExpandPreviewsDown());
}

void CLampView::OnUpdateExpandPreviewsDown(CCmdUI *pCmdUI)
{
   if(theApp.ExpandPreviews())
   {
      pCmdUI->Enable(TRUE);
   }
   else
   {
      pCmdUI->Enable(FALSE);
   }

   if(theApp.ExpandPreviewsDown())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}
 
void CLampView::OnExpandPreviews()
{
   theApp.SetExpandPreviews(!theApp.ExpandPreviews());
}

void CLampView::OnUpdateExpandPreviews(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.ExpandPreviews())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnCheckUpdate()
{
   theApp.CheckForUpdates(true);
}

void CLampView::OnUpdateCheckUpdate(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampView::OnViewProfile()
{
   if(!m_authorname_clicked.IsEmpty())
   {
      theApp.SetLastSearchParms(m_authorname_clicked, UCString(), UCString());

      UCString path = L"PROFILE:";
      path += m_authorname_clicked;

      theApp.OpenDocumentFile(path);
   }
}

void CLampView::OnUpdateViewProfile(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampView::OnSendMessage()
{
   if(!m_authorname_clicked.IsEmpty())
   {
      SendMessageDlg(GetDocument(),m_authorname_clicked,UCString(),UCString());
   }
}

void CLampView::OnUpdateSendMessage(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampView::OnViewComments()
{
   if(!m_authorname_clicked.IsEmpty())
   {
      theApp.SetLastSearchParms(m_authorname_clicked, UCString(), UCString());

      UCString path = L"CUSTOMSEARCH:";
      char *enc = url_encode(m_authorname_clicked.str8());
      path += enc;
      free(enc);
      path += L"::";

      theApp.OpenDocumentFile(path);
   }
}

void CLampView::OnUpdateViewComments(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampView::OnStrokePreviewEdges()
{
   theApp.SetStrokePreviewEdges(!theApp.StrokePreviewEdges());
}

void CLampView::OnUpdateStrokePreviewEdges(CCmdUI *pCmdUI)
{
   if(theApp.ExpandPreviews())
   {
      pCmdUI->Enable(TRUE);
   }
   else
   {
      pCmdUI->Enable(FALSE);
   }

   if(theApp.StrokePreviewEdges())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}


void CLampView::OnLineThickness1()
{
   theApp.SetLineThickness(1);
}

void CLampView::OnUpdateLineThickness1(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.GetLineThickness() == 1)
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnLineThickness2()
{
   theApp.SetLineThickness(2);
}

void CLampView::OnUpdateLineThickness2(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.GetLineThickness() == 2)
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnLineThickness3()
{
   theApp.SetLineThickness(3);
}

void CLampView::OnUpdateLineThickness3(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.GetLineThickness() == 3)
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnUseAuthorColor()
{
   theApp.UseAuthorColorForPreview(!theApp.UseAuthorColorForPreview());
}

void CLampView::OnUpdateUseAuthorColor(CCmdUI *pCmdUI)
{
   if(theApp.ExpandPreviews() && theApp.StrokePreviewEdges())
   {
      pCmdUI->Enable(TRUE);
   }
   else
   {
      pCmdUI->Enable(FALSE);
   }

   if(theApp.UseAuthorColorForPreview())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnUsersLOLs()
{
   if(!m_authorname_clicked.IsEmpty())
   {
      UCString path = L"LOLTHEYWROTE";
      path += m_authorname_clicked;

      theApp.OpenDocumentFile(path);
   }
}

void CLampView::OnUpdateUsersLOLs(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}