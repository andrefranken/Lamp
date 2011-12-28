
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Lamp.h"
#include "LampDoc.h"
#include "LampView.h"
#include "ChildFrm.h"
#include "MainFrm.h"
#include "DockTab.h"
#include "BookmarkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


extern DWORD g_LastPostTime;

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
   ON_WM_CLOSE()
   ON_WM_KILLFOCUS()
   ON_WM_TIMER()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)
   ON_MESSAGE(WM_EXPAND_TABS, &CMainFrame::OnExpandTabs)
   ON_MESSAGE(WM_WAKEUP, &CMainFrame::OnWakeUp)
   ON_COMMAND(ID_BOOKMARK_MENU, OnBookmarkMenu)
   ON_UPDATE_COMMAND_UI(ID_BOOKMARK_MENU, OnUpdateBookmarkMenu)
   ON_COMMAND_RANGE(ID_BOOKMARK_MENUITEM, ID_BOOKMARK_MENUITEM + 100, &CMainFrame::OnBookmark)
	ON_UPDATE_COMMAND_UI_RANGE(ID_BOOKMARK_MENUITEM, ID_BOOKMARK_MENUITEM + 100, &CMainFrame::OnUpdateBookmark)
                    
   ON_MESSAGE(WM_MENURBUTTONUP, &CMainFrame::OnMenuRBUMessage)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLACK);
   m_pLastActiveView = NULL;
}

CMainFrame::~CMainFrame()
{
}

// FindMenuItem() will find a menu item string from the specified
// popup menu and returns its position (0-based) in the specified 
// popup menu. It returns -1 if no such menu item string is found.
int FindMenuItem(CMenu* Menu, LPCTSTR MenuString)
{
   ASSERT(Menu);
   ASSERT(::IsMenu(Menu->GetSafeHmenu()));

   int count = Menu->GetMenuItemCount();
   for (int i = 0; i < count; i++)
   {
      CString str;
      if (Menu->GetMenuStringW(i, str, MF_BYPOSITION) &&
         (wcscmp(str, MenuString) == 0))
         return i;
   }

   return -1;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // other styles available...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // set to FALSE to place close button at right of tab area
	mdiTabParams.m_bTabIcons = FALSE;    // set to TRUE to enable document icons on MDI taba
	mdiTabParams.m_bAutoColor = TRUE;    // set to FALSE to disable auto-coloring of MDI tabs
	mdiTabParams.m_bDocumentMenu = FALSE; // TRUE; // enable the document menu at the right edge of the tab area
 
	mdiTabParams.m_bEnableTabSwap = TRUE;// Specifies whether the user can swap the positions of tabs by dragging.
	mdiTabParams.m_bFlatFrame = TRUE; //  Specifies whether tabs have a flat frame.
	mdiTabParams.m_bTabCloseButton  = TRUE;// Specifies whether each tab label displays a Close button.
	mdiTabParams.m_bTabCustomTooltips = FALSE;// Specifies whether custom tooltips are enabled.
	mdiTabParams.m_nTabBorderSize = 0; //  Specifies the border size of each tab window.
	mdiTabParams.m_tabLocation = CMFCTabCtrl::LOCATION_TOP;// Specifies whether the tabs labels are located at the top or the bottom of the page.

	EnableMDITabbedGroups(TRUE, mdiTabParams);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle((m_wndMenuBar.GetPaneStyle() ^ CBRS_GRIPPER) | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY );

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

   if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	
	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	//m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

   CMFCMenuBar::SetShowAllCommands();

   SetTimer(INBOX_TIMER,(UINT)60000 * (UINT)theApp.GetNumMinutesCheckInbox(),NULL);

   SetTimer(IMAGE_EXPIRE_TIMER,(UINT)60000 * 60,NULL);

   m_bFirstUpdate = true;
   SetTimer(UPDATE_TIMER,(UINT)60000 * 1,NULL); // check for updates starting in 1 minutes

   SetTimer(REFRESH_LOL_TIMER,(UINT)60000 * (UINT)theApp.GetNumMinutesCheckLOL(),NULL); // check for updates in 5 minutes
      
	return 0;
}

void CMainFrame::UpdateInboxTimer()
{
   KillTimer(INBOX_TIMER);
   SetTimer(INBOX_TIMER,(UINT)60000 * (UINT)theApp.GetNumMinutesCheckInbox(),NULL);
}


void CMainFrame::UpdateLOLTimer()
{
   KillTimer(REFRESH_LOL_TIMER);
   SetTimer(REFRESH_LOL_TIMER,(UINT)60000 * (UINT)theApp.GetNumMinutesCheckLOL(),NULL);
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
   static wchar_t classname[] =  L"Lamp - Shack Client";
   cs.lpszClass = classname;

	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnClose() 
{
   theApp.ClearSession();
   CHackedTabCtrl *tabctrl = (CHackedTabCtrl*)GetCA()->FindActiveTabWnd();
   if(tabctrl != NULL)
   {
      int numtabs = tabctrl->GetTabsNum();
      for(int i=0; i < numtabs; i++)
      {
         CChildFrame *pFrame = (CChildFrame*)tabctrl->GetTabWnd(i);
         if(pFrame != NULL)
         {
            CLampView *pView = pFrame->GetView();
            if(pView != NULL)
            {
               unsigned int current_id = pView->GetCurrentId();
               CLampDoc *pDoc = pView->GetDocument();
               if(pDoc != NULL)
               {
                  UCString launch;
                  pDoc->GetLaunchString(launch, current_id);
                  if(!launch.IsEmpty())
                  {
                     theApp.AddToSession(launch);
                  }
               }
            }
         }
      }
   }

	CMDIFrameWndEx::OnClose();
}

void CMainFrame::OnKillFocus(CWnd* pNewWnd)
{
   if(theApp.m_bDockedMode)
   {
      if(!theApp.m_bCollapsed && 
         pNewWnd->GetTopLevelFrame() != theApp.GetMainWnd())
      {
         theApp.m_bCollapsed = true;

         if(theApp.m_pDockTab != NULL)
         {
            RECT framerect;
            GetWindowRect(&framerect);
            RECT docktabrect;
            docktabrect.left = ((framerect.right + framerect.left) / 2) - (theApp.GetDockTabImage()->GetWidth() / 2);
            docktabrect.right = docktabrect.left + theApp.GetDockTabImage()->GetWidth();
            docktabrect.top = 0;
            docktabrect.bottom = theApp.GetDockTabImage()->GetHeight();
            theApp.m_pDockTab->MoveWindow(&docktabrect);

            theApp.m_pDockTab->ShowWindow(SW_SHOW);
         }
         
         ShowWindow(SW_HIDE);
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
   CMDIFrameWndEx::OnKillFocus(pNewWnd);
}

CLampView *CMainFrame::GetActiveLampView(void)
{
   CLampView *result = NULL;

   result = (CLampView *)GetActiveView();

   if(result == NULL ||
      result->IsKindOf( RUNTIME_CLASS( CLampView )) == FALSE)
   {
      result = m_pLastActiveView;         
   }
   
   CFrameWnd *pChild = (CFrameWnd *)GetActiveFrame();

   if(pChild != NULL && pChild->IsKindOf( RUNTIME_CLASS( CChildFrame ) ))
   {
      result = (CLampView*)pChild->GetActiveView();
   }
   return result;
}

LRESULT CMainFrame::OnExpandTabs(WPARAM,LPARAM)
{
   theApp.UpdateTabSizes();
   return 0;
}

LRESULT CMainFrame::OnWakeUp(WPARAM,LPARAM)
{
   if(theApp.m_bDockedMode &&
      theApp.m_bCollapsed)
   {
      DockTab *pDT = theApp.GetDockTab();
      if(pDT != NULL)
      {
         pDT->Trip();
      }
   }
   else
   {
      SetFocus();
   }
   return 0;
}

void CMainFrame::OnBookmarkMenu()
{
   CMenu menu;
   menu.LoadMenu(IDR_POPUP_BOOKMARK_MENU);
   CMenu *pMenu = menu.GetSubMenu(0);

   UINT count = pMenu->GetMenuItemCount();

   theApp.PopulateBookmarkMenu(pMenu);

   RECT rect;

   for(size_t i = 0; i < 10; i++)
   {
      if(m_wndMenuBar.GetItemID(i) == ID_BOOKMARK_MENU)
      {
         m_wndMenuBar.GetItemRect(i,&rect);
         break;
      }
   }

   ClientToScreen(&rect);

   pMenu->TrackPopupMenu(TPM_LEFTBUTTON, rect.left, rect.bottom, this, NULL);
}

void CMainFrame::OnUpdateBookmarkMenu(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}


void CMainFrame::OnBookmark(UINT id)
{
   size_t index = id - ID_BOOKMARK_MENUITEM;
   theApp.LaunchBookmarkMenu(index);
   //theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, 100, 100, this, TRUE);
}

void CMainFrame::OnUpdateBookmark(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(TRUE);
}


LRESULT CMainFrame::OnMenuRBUMessage(WPARAM wparam, LPARAM lparam)
{
   int menuitemindex = wparam;

   CShackBookmark *pBM = theApp.GetBookmark(menuitemindex - 2);
   if(pBM != NULL)
   {
      CBookmarkDlg bmdlg;
      bmdlg.m_bEditingExistingBM = true;
      bmdlg.m_Title = pBM->m_title;

      if(bmdlg.DoModal() == IDOK)
      {
         pBM->m_title = bmdlg.m_Title;
      }
      else
      {
         // delete it
         theApp.RemoveBookmark(*pBM);
      }
   }
   return 0;
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
   if(::GetTickCount() < g_LastPostTime + ((UINT)60000 * 15))
   {
      // ^^^ If the user hasn't done anything web-wise in the past 15 minutes,
      // assume they walked away.   Don't hammer the servers with automated
      // traffic, if the user isn't even there.
      if(nIDEvent == INBOX_TIMER)
      {
         theApp.UpdateInbox();
      }   
      else if(nIDEvent == REFRESH_LOL_TIMER)
      {
         theApp.RefreshLOLs();
      }
      else if(nIDEvent == UPDATE_TIMER)
      {
         if(m_bFirstUpdate)
         {
            m_bFirstUpdate = false;
            KillTimer(UPDATE_TIMER);
            SetTimer(UPDATE_TIMER,(UINT)60000 * 1440,NULL); // check for updates once a day
         }
         theApp.CheckForUpdates(false);
      }
      else if(nIDEvent == IMAGE_EXPIRE_TIMER)
      {
         theApp.CheckForExpiredImages();
      }
   }
   else
   {
      UCString breakpoint;
   }
}

void CMainFrame::MBClick(CPoint &point, HWND hwnd)
{
   CHackedTabCtrl *tabctrl = (CHackedTabCtrl*)GetCA()->FindActiveTabWnd();
   if(tabctrl != NULL &&
      tabctrl->GetSafeHwnd() == hwnd)
   {
      int tab = tabctrl->GetTabFromPoint(point);
      if(tab != -1)
      {
         CWnd *pView = tabctrl->GetTabWnd(tab);
         if(pView != NULL)
         {
            pView->PostMessageW(WM_CLOSE);
         }
      }
   }
}


void CMainFrame::CloseFindDlg()
{
   CLampView *pView = GetActiveLampView();

   if(pView != NULL)
   {
      pView->CloseFindDlg();
   }
}

/*
BEGIN_MESSAGE_MAP(CBMMenu, CMenu)
   ON_WM_LBUTTONDOWN()
   ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


void CBMMenu::OnLButtonDown(UINT nFlags, CPoint point)
{
   CMenu::OnLButtonDown(nFlags, point);
}

void CBMMenu::OnRButtonDown(UINT nFlags, CPoint point)
{
   CMenu::OnRButtonDown(nFlags, point);
}
*/
