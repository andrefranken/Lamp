
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
#include "CustomSearchDlg.h"

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
   ON_COMMAND(ID_COPY_NAME, &CLampView::OnCopyName)
   ON_UPDATE_COMMAND_UI(ID_COPY_NAME, &CLampView::OnUpdateCopyName)
   ON_COMMAND(ID_COPYPOST, &CLampView::OnCopyPost)
   ON_UPDATE_COMMAND_UI(ID_COPYPOST, &CLampView::OnUpdateCopyPost)
   ON_COMMAND(ID_LAUNCHLINK, &CLampView::OnLaunchLink)
   ON_UPDATE_COMMAND_UI(ID_LAUNCHLINK, &CLampView::OnUpdateLaunchLink)
   ON_COMMAND(ID_CHROME_OLI, &CLampView::OnLaunchLink_Chrome)
   ON_COMMAND(ID_CHROME_INCOGNITO_OLI, &CLampView::OnLaunchLink_Chrome_Incognito)
   ON_COMMAND(ID_FIREFOX_OLI, &CLampView::OnLaunchLink_Firefox)
   ON_COMMAND(ID_IE_OLI, &CLampView::OnLaunchLink_IE)
   ON_COMMAND(ID_IE_PRIVATE_OLI, &CLampView::OnLaunchLink_IE_Private)
   ON_COMMAND(ID_SAFARI_OLI, &CLampView::OnLaunchLink_Safari)
   ON_UPDATE_COMMAND_UI(ID_CHROME_OLI, &CLampView::OnUpdateLaunchLink)
   ON_UPDATE_COMMAND_UI(ID_CHROME_INCOGNITO_OLI, &CLampView::OnUpdateLaunchLink)
   ON_UPDATE_COMMAND_UI(ID_FIREFOX_OLI, &CLampView::OnUpdateLaunchLink)
   ON_UPDATE_COMMAND_UI(ID_IE_OLI, &CLampView::OnUpdateLaunchLink)
   ON_UPDATE_COMMAND_UI(ID_IE_PRIVATE_OLI, &CLampView::OnUpdateLaunchLink)
   ON_UPDATE_COMMAND_UI(ID_SAFARI_OLI, &CLampView::OnUpdateLaunchLink)
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
   ON_COMMAND(ID_OPENALLLINKS, &CLampView::OnOpenAllLinks)
   ON_UPDATE_COMMAND_UI(ID_OPENALLLINKS, &CLampView::OnUpdateOpenAllLinks)
   ON_COMMAND(ID_OPENALLGIFS, &CLampView::OnOpenAllGifs)
   ON_UPDATE_COMMAND_UI(ID_OPENALLGIFS, &CLampView::OnUpdateOpenAllGifs)
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
   ON_COMMAND(ID_DO_UGH, &CLampView::OnDoUGH)
   ON_UPDATE_COMMAND_UI(ID_DO_UGH, &CLampView::OnUpdateDoUGH)
   ON_COMMAND(ID_CHECK_SPELLING, &CLampView::OnCheckSpelling)
   ON_UPDATE_COMMAND_UI(ID_CHECK_SPELLING, &CLampView::OnUpdateCheckSpelling)
   ON_COMMAND(ID_LEFTMOUSEPAN, &CLampView::OnLeftMousePan)
   ON_UPDATE_COMMAND_UI(ID_LEFTMOUSEPAN, &CLampView::OnUpdateLeftMousePan)
   ON_COMMAND(ID_MOVEREFRESHTOTOP, &CLampView::OnMoveRefreshToTop)
   ON_UPDATE_COMMAND_UI(ID_MOVEREFRESHTOTOP, &CLampView::OnUpdateMoveRefreshToTop)
   ON_COMMAND(ID_SHOW_NAV_BUTTONS, &CLampView::OnShowNavButtons)
   ON_UPDATE_COMMAND_UI(ID_SHOW_NAV_BUTTONS, &CLampView::OnUpdateShowNavButtons)
   ON_COMMAND(ID_LATESTCHATTY_SUMMARY_MODE, &CLampView::OnLatestChattySummaryMode)
   ON_UPDATE_COMMAND_UI(ID_LATESTCHATTY_SUMMARY_MODE, &CLampView::OnUpdateLatestChattySummaryMode)
   ON_COMMAND(ID_TIGHT_FIT_SUMMARY, &CLampView::OnTightFitSummary)
   ON_UPDATE_COMMAND_UI(ID_TIGHT_FIT_SUMMARY, &CLampView::OnUpdateTightFitSummary)
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
   ON_COMMAND(ID_SKIN_METRO, &CLampView::OnSkinMetroShack)
   ON_UPDATE_COMMAND_UI(ID_SKIN_METRO, &CLampView::OnUpdateSkinMetroShack)
   ON_COMMAND(ID_SKIN_ROUNDSHACK_2, &CLampView::OnSkinRoundShack2)
   ON_UPDATE_COMMAND_UI(ID_SKIN_ROUNDSHACK_2, &CLampView::OnUpdateSkinRoundShack2)
   ON_COMMAND(ID_SKIN_DEFAULT_2, &CLampView::OnSkinSquareShack2)
   ON_UPDATE_COMMAND_UI(ID_SKIN_DEFAULT_2, &CLampView::OnUpdateSkinSquareShack2)
   ON_COMMAND(ID_SKIN_WORKSAFE_2, &CLampView::OnSkinWorksafeShack2)
   ON_UPDATE_COMMAND_UI(ID_SKIN_WORKSAFE_2, &CLampView::OnUpdateSkinWorksafeShack2)
   ON_COMMAND(ID_SKIN_METRO_2, &CLampView::OnSkinMetroShack2)
   ON_UPDATE_COMMAND_UI(ID_SKIN_METRO_2, &CLampView::OnUpdateSkinMetroShack2)
   ON_COMMAND(ID_SKIN_CUSTOM, &CLampView::OnSkinCustom)
   ON_UPDATE_COMMAND_UI(ID_SKIN_CUSTOM, &CLampView::OnUpdateSkinCustom)
   ON_COMMAND(ID_HIGHLIGHT_OP, &CLampView::OnHighlightOP)
   ON_UPDATE_COMMAND_UI(ID_HIGHLIGHT_OP, &CLampView::OnUpdateHighlightOP)
   ON_COMMAND(ID_AUTHOR_GLOW, &CLampView::OnAuthorGlow)
   ON_UPDATE_COMMAND_UI(ID_AUTHOR_GLOW, &CLampView::OnUpdateAuthorGlow)
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
   ON_COMMAND(ID_URBANDIC_SELECTED, &CLampView::OnUrbanDicSelected)
   ON_UPDATE_COMMAND_UI(ID_URBANDIC_SELECTED, &CLampView::OnUpdateUrbanDicSelected)
   ON_COMMAND(ID_DIC_SELECTED, &CLampView::OnDicSelected)
   ON_UPDATE_COMMAND_UI(ID_DIC_SELECTED, &CLampView::OnUpdateDicSelected)
   ON_COMMAND(ID_WOLFRAM_SELECTED, &CLampView::OnWolframSelected)
   ON_UPDATE_COMMAND_UI(ID_WOLFRAM_SELECTED, &CLampView::OnUpdateWolframSelected)
   ON_COMMAND(ID_IMDB_SELECTED, &CLampView::OnIMDBSelected)
   ON_UPDATE_COMMAND_UI(ID_IMDB_SELECTED, &CLampView::OnUpdateIMDBSelected)
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
   ON_COMMAND(ID_SHOW_RAW_DATE, &CLampView::OnShowRawDate)
   ON_UPDATE_COMMAND_UI(ID_SHOW_RAW_DATE, &CLampView::OnUpdateShowRawDate)
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
   ON_COMMAND(ID_FLAG_USER, &CLampView::OnFlagUser)
   ON_UPDATE_COMMAND_UI(ID_FLAG_USER, &CLampView::OnUpdateFlagUser)
   ON_COMMAND(ID_SYSTEM_DEF, &CLampView::OnSystemDef)
   ON_COMMAND(ID_CHROME_DEF, &CLampView::OnChromeDef)
   ON_COMMAND(ID_CHROME_INCOGNITO_DEF, &CLampView::OnChromeIncognitoDef)
   ON_COMMAND(ID_FIREFOX_DEF, &CLampView::OnFirefoxDef)
   ON_COMMAND(ID_IE_DEF, &CLampView::OnIEDef)
   ON_COMMAND(ID_IE_PRIVATE_DEF, &CLampView::OnIEPrivateDef)
   ON_COMMAND(ID_SAFARI_DEF, &CLampView::OnSafariDef)
   ON_UPDATE_COMMAND_UI(ID_SYSTEM_DEF, &CLampView::OnUpdateSystemDef)
   ON_UPDATE_COMMAND_UI(ID_CHROME_DEF, &CLampView::OnUpdateChromeDef)
   ON_UPDATE_COMMAND_UI(ID_CHROME_INCOGNITO_DEF, &CLampView::OnUpdateChromeIncognitoDef)
   ON_UPDATE_COMMAND_UI(ID_FIREFOX_DEF, &CLampView::OnUpdateFirefoxDef)
   ON_UPDATE_COMMAND_UI(ID_IE_DEF, &CLampView::OnUpdateIEDef)
   ON_UPDATE_COMMAND_UI(ID_IE_PRIVATE_DEF, &CLampView::OnUpdateIEPrivateDef)
   ON_UPDATE_COMMAND_UI(ID_SAFARI_DEF, &CLampView::OnUpdateSafariDef)
   ON_COMMAND(ID_SYSTEM_DEF_NWS, &CLampView::OnSystemDef_nws)
   ON_COMMAND(ID_CHROME_DEF_NWS, &CLampView::OnChromeDef_nws)
   ON_COMMAND(ID_CHROME_INCOGNITO_DEF_NWS, &CLampView::OnChromeIncognitoDef_nws)
   ON_COMMAND(ID_FIREFOX_DEF_NWS, &CLampView::OnFirefoxDef_nws)
   ON_COMMAND(ID_IE_DEF_NWS, &CLampView::OnIEDef_nws)
   ON_COMMAND(ID_IE_PRIVATE_DEF_NWS, &CLampView::OnIEPrivateDef_nws)
   ON_COMMAND(ID_SAFARI_DEF_NWS, &CLampView::OnSafariDef_nws)
   ON_UPDATE_COMMAND_UI(ID_SYSTEM_DEF_NWS, &CLampView::OnUpdateSystemDef_nws)
   ON_UPDATE_COMMAND_UI(ID_CHROME_DEF_NWS, &CLampView::OnUpdateChromeDef_nws)
   ON_UPDATE_COMMAND_UI(ID_CHROME_INCOGNITO_DEF_NWS, &CLampView::OnUpdateChromeIncognitoDef_nws)
   ON_UPDATE_COMMAND_UI(ID_FIREFOX_DEF_NWS, &CLampView::OnUpdateFirefoxDef_nws)
   ON_UPDATE_COMMAND_UI(ID_IE_DEF_NWS, &CLampView::OnUpdateIEDef_nws)
   ON_UPDATE_COMMAND_UI(ID_IE_PRIVATE_DEF_NWS, &CLampView::OnUpdateIEPrivateDef_nws)
   ON_UPDATE_COMMAND_UI(ID_SAFARI_DEF_NWS, &CLampView::OnUpdateSafariDef_nws)
   ON_COMMAND(ID_ALLOW_GDIPLUS, &CLampView::OnAllowGDIPlus)
   ON_UPDATE_COMMAND_UI(ID_ALLOW_GDIPLUS, &CLampView::OnUpdateAllowGDIPlus)
   ON_COMMAND(ID_AUTO_REFRESH, &CLampView::OnAutoRefresh)
   ON_UPDATE_COMMAND_UI(ID_AUTO_REFRESH, &CLampView::OnUpdateAutoRefresh)

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
   m_bLButtonDown = false;
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
   m_leftmouse_timer_active = false;
   m_mouseOverClientArea = false;
   m_indent_panning = false;
   m_hover_preview_percent = 1.0f;
   m_history_it = m_history_list.end();
   m_panning_reply = false;
   m_lmb_in_reply = false;
   m_bLBDownOnDblClkable = false;
   m_mousepoint.SetPoint(9999,9999);
   m_refreshing_latestchatty = false;

   m_last_user_activity = ::GetTickCount();
   
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

void CLampView::CloseReplyOnlyDlg()
{
   if(m_pReplyDlg != NULL &&
      m_pReplyDlg->GetReplyId() != 0)
   {
      CloseReplyDialog();
   }
}

void CLampView::CloseNonReplyOnlyDlg()
{
   if(m_pReplyDlg != NULL &&
      m_pReplyDlg->GetReplyId() == 0)
   {
      CloseReplyDialog();
   }
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

void CLampView::SetCurrentId(unsigned int id, bool dofocusstuff /*= true*/)
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
         if(GetDocument()->GetDataType() == DDT_STORY &&
            theApp.LatestChattySummaryMode())
         {
            // find and tell the active view to display this thread
            UCString path;
            path += L"ACTIVE:";
            path += id;
            path += L'_';
            path += id;
            ReleaseCapture();
            theApp.OpenActiveDocumentFile(path, dofocusstuff);

            MakeCurrentPostLegal();
         }
         else
         {
            GetDocument()->MakePostAvailable(id);
         }
      }
   }
   else
   {
      if(GetDocument()->GetDataType() == DDT_STORY &&
         theApp.LatestChattySummaryMode())
      {
         // find and tell the active view to display this thread
         theApp.ActivateActiveThread(dofocusstuff);
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
   m_lastmousetime = m_last_user_activity = ::GetTickCount();
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
   m_last_user_activity = ::GetTickCount();
   if(m_bStartedTrackingMouse)
   {
      m_mousehistory.push_back(point.y - m_mousepoint.y);
      while(m_mousehistory.size() > 5) m_mousehistory.pop_front();
      DWORD thistime = m_last_user_activity;
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
   bool bDoAuthor = false;
   bool bDoReplyEdit = false;
   bool bDoPostBody = false;
   bool bHasSel = false;

   if(m_pReplyDlg != NULL &&
      m_mousepoint.x >= m_pReplyDlg->m_replydlgrect.left &&
      m_mousepoint.x < m_pReplyDlg->m_replydlgrect.right &&
      m_mousepoint.y >= m_pReplyDlg->m_replydlgrect.top &&
      m_mousepoint.y < m_pReplyDlg->m_replydlgrect.bottom)
   {
      if(m_mousepoint.x >= m_pReplyDlg->m_textdrawrect.left &&
         m_mousepoint.x < m_pReplyDlg->m_textdrawrect.right &&
         m_mousepoint.y >= m_pReplyDlg->m_textdrawrect.top &&
         m_mousepoint.y < m_pReplyDlg->m_textdrawrect.bottom)
      {
         bDoReplyEdit = true;

         if(!m_pReplyDlg->AreSuggestionsUp())
         {
            if(m_pReplyDlg->HasSelection())
            {
               bHasSel = true;
            }
         }
      }
   }
   else
   {
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

            else if(m_hotspots[i].m_type == HST_AUTHOR ||
                    m_hotspots[i].m_type == HST_AUTHORPREVIEW)
            {
               ChattyPost *post = GetDocument()->FindPost(m_hotspots[i].m_id);
               if(post != NULL)
               {
                  m_authorname_clicked = post->GetAuthor();
                  bDoAuthor = true;
               }
            }
            else if(m_hotspots[i].m_type == HST_REPLYTEXT)
            {
               bDoReplyEdit = true;

               if(m_pReplyDlg != NULL &&
                  !m_pReplyDlg->AreSuggestionsUp())
               {
                  if(m_pReplyDlg->HasSelection())
                  {
                     bHasSel = true;
                  }
               }
            }
            else if(m_hotspots[i].m_type == HST_TEXT ||
                    m_hotspots[i].m_type == HST_POST_AREA)
            {
               m_rbuttonmenufromid = m_hotspots[i].m_id;
                           
               bDoPostBody = true;

               if(m_textselectionpost != 0 &&
                  m_selectionstart != m_selectionend)
               {
                  bHasSel = true;
               }
            }
            break;
         }
      }
   }

	if(bDoLink)
   {
      theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_COPYLINK_MENU, point.x, point.y, this, TRUE);
   }
   else if(bDoAuthor)
   {
      theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_USERLINK_MENU, point.x, point.y, this, TRUE);
   }
   else if(bDoPostBody)
   {
      if(bHasSel)
      {
         theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_SEL, point.x, point.y, this, TRUE);
      }
      else
      {
         theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_NOSEL, point.x, point.y, this, TRUE);
      }
   }
   else if(bDoReplyEdit)
   {
      if(bHasSel)
      {
         theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT_SEL, point.x, point.y, this, TRUE);
      }
      else
      {
         theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT_NOSEL, point.x, point.y, this, TRUE);
      }
   }
   else
   {
      theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_DEFAULT, point.x, point.y, this, TRUE);
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

            int scrollwidth = 16;
            if(theApp.BigSkin()) scrollwidth = 32;

            DocRectangle = DeviceRectangle;
            DocRectangle.right -= scrollwidth;

            ScrollRectangle = DeviceRectangle;
            ScrollRectangle.left = ScrollRectangle.right - scrollwidth;
            
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
               fromrect.right -= scrollwidth;

               RECT torect = DeviceRectangle;
               torect.right -= scrollwidth;

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

            if(theApp.HaveNewMessages() && 
               theApp.GetAutoCheckInbox() &&
              (pDoc->GetDataType() != DDT_STORY || 
              (pDoc->GetDataType() == DDT_STORY &&
               !theApp.LatestChattySummaryMode())))
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
               if(theApp.LatestChattySummaryMode())
               {
                  pDoc->DrawBanner(m_bannerbuffer.GetDC(), m_BannerRectangle, 0, m_hotspots, false, false, false, this);
               }
               else
               {
                  pDoc->DrawBanner(m_bannerbuffer.GetDC(), m_BannerRectangle, 0, m_hotspots, true, false, false, this);
               }
               bDrewBanner = true;
               break;
            case DDT_LOLS:
               pDoc->DrawBanner(m_bannerbuffer.GetDC(), m_BannerRectangle, 0, m_hotspots, false, false, false, this);
               bDrewBanner = true;
               break;
            case DDT_SEARCH:
               pDoc->DrawBanner(m_bannerbuffer.GetDC(), m_BannerRectangle, 0, m_hotspots, false, false, true, this);
               bDrewBanner = true;
               break;
            case DDT_SHACKMSG:
               pDoc->DrawBanner(m_bannerbuffer.GetDC(), m_BannerRectangle, 0, m_hotspots, false, true, false, this);
               bDrewBanner = true;
               break;
            case DDT_THREAD:
               pDoc->DrawBanner(m_bannerbuffer.GetDC(), m_BannerRectangle, 0, m_hotspots, false, false, false, this);
               bDrewBanner = true;
               break;
            case DDT_ACTIVE_THREAD:
               pDoc->DrawBanner(m_bannerbuffer.GetDC(), m_BannerRectangle, 0, m_hotspots, true, false, false, this);
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

            bool bDrawNewIndicators = false;
            if((pDoc->GetDataType() == DDT_THREAD || 
                pDoc->GetDataType() == DDT_ACTIVE_THREAD) && 
               !m_bPanOnly)
            {
               bDrawNewIndicators = true;
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

            // draw new post indicators
            if(bDrawNewIndicators)
            {
               if(m_newpostabove)
               {
                  TRIVERTEX tripoints[3];
                  memset(tripoints,0,sizeof(TRIVERTEX) * 3);
                                    
                  tripoints[0].x = 0;
                  tripoints[0].y = m_banneroffset;
                  tripoints[1].x = 20;
                  tripoints[1].y = m_banneroffset;
                  tripoints[2].x = 0;
                  tripoints[2].y = m_banneroffset + 20;

                  tripoints[0].Red = GetRValue(theApp.GetBranchColorShade(10, N_NEW)) << 8;
                  tripoints[0].Green = GetGValue(theApp.GetBranchColorShade(10, N_NEW)) << 8;
                  tripoints[0].Blue = GetBValue(theApp.GetBranchColorShade(10, N_NEW)) << 8;
                  tripoints[0].Alpha = 0xFFFF;

                  tripoints[1].Red = GetRValue(theApp.GetBackgroundColor()) << 8;
                  tripoints[1].Green = GetGValue(theApp.GetBackgroundColor()) << 8;
                  tripoints[1].Blue = GetBValue(theApp.GetBackgroundColor()) << 8;
                  tripoints[1].Alpha = 0xFFFF;

                  tripoints[2].Red = GetRValue(theApp.GetBackgroundColor()) << 8;
                  tripoints[2].Green = GetGValue(theApp.GetBackgroundColor()) << 8;
                  tripoints[2].Blue = GetBValue(theApp.GetBackgroundColor()) << 8;
                  tripoints[2].Alpha = 0xFFFF;
                  
                  GRADIENT_TRIANGLE triangles[1];
                  triangles[0].Vertex1 = 0;
                  triangles[0].Vertex2 = 1;
                  triangles[0].Vertex3 = 2;

                  GradientFill(hDC,
                               tripoints,
                               3,
                               triangles,
                               1,
                               GRADIENT_FILL_TRIANGLE);

                  tripoints[0].x = DocRectangle.right;
                  tripoints[1].x = DocRectangle.right - 20;
                  tripoints[2].x = DocRectangle.right;

                  GradientFill(hDC,
                               tripoints,
                               3,
                               triangles,
                               1,
                               GRADIENT_FILL_TRIANGLE);
               }

               if(m_newpostbelow)
               {
                  TRIVERTEX tripoints[3];
                  memset(tripoints,0,sizeof(TRIVERTEX) * 3);
                                    
                  tripoints[0].x = 0;
                  tripoints[0].y = DeviceRectangle.bottom;
                  tripoints[1].x = 20;
                  tripoints[1].y = DeviceRectangle.bottom;
                  tripoints[2].x = 0;
                  tripoints[2].y = DeviceRectangle.bottom - 20;

                  tripoints[0].Red = GetRValue(theApp.GetBranchColorShade(10, N_NEW)) << 8;
                  tripoints[0].Green = GetGValue(theApp.GetBranchColorShade(10, N_NEW)) << 8;
                  tripoints[0].Blue = GetBValue(theApp.GetBranchColorShade(10, N_NEW)) << 8;
                  tripoints[0].Alpha = 0xFFFF;

                  tripoints[1].Red = GetRValue(theApp.GetBackgroundColor()) << 8;
                  tripoints[1].Green = GetGValue(theApp.GetBackgroundColor()) << 8;
                  tripoints[1].Blue = GetBValue(theApp.GetBackgroundColor()) << 8;
                  tripoints[1].Alpha = 0xFFFF;

                  tripoints[2].Red = GetRValue(theApp.GetBackgroundColor()) << 8;
                  tripoints[2].Green = GetGValue(theApp.GetBackgroundColor()) << 8;
                  tripoints[2].Blue = GetBValue(theApp.GetBackgroundColor()) << 8;
                  tripoints[2].Alpha = 0xFFFF;
                  
                  GRADIENT_TRIANGLE triangles[1];
                  triangles[0].Vertex1 = 0;
                  triangles[0].Vertex2 = 1;
                  triangles[0].Vertex3 = 2;

                  GradientFill(hDC,
                               tripoints,
                               3,
                               triangles,
                               1,
                               GRADIENT_FILL_TRIANGLE);

                  tripoints[0].x = DocRectangle.right;
                  tripoints[1].x = DocRectangle.right - 20;
                  tripoints[2].x = DocRectangle.right;

                  GradientFill(hDC,
                               tripoints,
                               3,
                               triangles,
                               1,
                               GRADIENT_FILL_TRIANGLE);
               }
            }
            
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

         if(!bDrewNewMessagesTab && 
            theApp.HaveNewMessages() && 
            theApp.GetAutoCheckInbox() &&
           (pDoc->GetDataType() != DDT_STORY || 
           (pDoc->GetDataType() == DDT_STORY &&
            !theApp.LatestChattySummaryMode())))
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

   int scrollwidth = 16;
   if(theApp.BigSkin()) scrollwidth = 32;
   
   RECT DeviceRectangle;

   if(pDeviceRectangle != NULL)
   {
      DeviceRectangle = *pDeviceRectangle;
   }
   else
   {
      DeviceRectangle.left = DeviceRectangle.top = 0;
      DeviceRectangle.right = pSurface->GetWidth() - scrollwidth;
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

      m_newpostabove = false;
      m_newpostbelow = false;

      int topclip = 0;
      if(GetDocument()->GetDataType() != DDT_PROFILE)
      {
         topclip = m_bannerbuffer.GetHeight();
      }

      GetDocument()->Draw(pSurface->GetDC(), 
                          device_height,
                          DeviceRectangle, 
                          m_pos, 
                          m_hotspots, 
                          GetCurrentId(), 
                          m_bModToolIsUp, 
                          m_ModToolRect, 
                          m_ModToolPostID,
                          m_newpostabove,
                          m_newpostbelow,
                          topclip);
   
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
      m_ScrollRectangle.right += scrollwidth;
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

   if(m_refreshing_latestchatty)
   {
      COLORREF bkclr = theApp.GetBackgroundColor();
      if((GetRValue(bkclr) + GetGValue(bkclr) + GetBValue(bkclr)) > (127 * 3))
      {
         pSurface->Brighten(DeviceRectangle.top, DeviceRectangle.bottom);
      }
      else
      {
         pSurface->Dim(DeviceRectangle.top, DeviceRectangle.bottom);
      }
   }
}

void CLampView::DrawScrollbar(HDC hDC, const RECT &ScrollRectangle, std::vector<CHotSpot> &hotspots)
{
   int thumbcapsize = 4;
   int scrollwidth = 16;
   if(theApp.BigSkin())
   {
      thumbcapsize = 8;
      scrollwidth = 32;
   }

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
   m_uprect.bottom = m_uprect.top + scrollwidth;
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
   m_downrect.top = m_downrect.bottom - scrollwidth;
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
   int trackheight = scrollbarheight - scrollwidth - scrollwidth;
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
   thumbtop = ScrollRectangle.top + __max(scrollwidth, scrollwidth + (int)((float)thumbtop * m_scrollscale));
   thumbbottom = ScrollRectangle.top + __min(scrollbarheight - scrollwidth, scrollwidth + (int)((float)thumbbottom * m_scrollscale));
   int thumbheight = thumbbottom - thumbtop;

   if(docheight > 0 && 
      thumbheight < (thumbcapsize + thumbcapsize))
   {
      // too small, bump it up
      int thumbcenter = (thumbtop + thumbbottom) >> 1;
      thumbtop = thumbcenter - thumbcapsize;
      thumbbottom = thumbtop + (thumbcapsize + thumbcapsize);
      thumbheight = thumbbottom - thumbtop;
      
      if(thumbtop < scrollwidth)
      {
         int off = scrollwidth - thumbtop;
         thumbtop+=off;
         thumbbottom+=off;
      }

      if(thumbbottom > ScrollRectangle.bottom - scrollwidth)
      {
         int off = thumbbottom - (ScrollRectangle.bottom - scrollwidth);
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
      int thumbcapsize = 4;
      int thumbgripsize = 10;
      int scrollwidth = 16;
      if(theApp.BigSkin())
      {
         thumbcapsize = 8;
         thumbgripsize = 20;
         scrollwidth = 32;
      }

      RECT thumbtoprect = thumbrect;
      thumbtoprect.bottom = thumbrect.top + thumbcapsize;
      theApp.GetThumbTopImage(type)->Blit(hDC,thumbtoprect);

      RECT thumbbottomrect = thumbrect;
      thumbbottomrect.top = thumbrect.bottom - thumbcapsize;
      thumbbottomrect.bottom = thumbrect.bottom;
      theApp.GetThumbBottomImage(type)->Blit(hDC,thumbbottomrect);

      bool bShowGrip = false;
      if(thumbrect.bottom - thumbrect.top > (thumbcapsize + thumbcapsize + thumbgripsize))
      {
         bShowGrip = true;
      }

      if(bShowGrip)
      {
         int thumbcenter = (thumbrect.top + thumbrect.bottom) >> 1;
         RECT thumbgriprect = thumbrect;
         thumbgriprect.top = thumbcenter - (thumbgripsize >> 1);
         thumbgriprect.bottom = thumbgriprect.top + thumbgripsize;
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
         //int oldrop = ::GetROP2(hDC);
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
         ::SetROP2(hDC,R2_COPYPEN);
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
            case HST_NAV_PREV_THREAD:
               {
                  ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);
                  theApp.GetNavImage(true, false, false, false, false)->Blit(hDC,m_hotspots[i].m_spot);
                  ::IntersectClipRect(hDC,DeviceRectangle.left,DeviceRectangle.top + m_banneroffset,DeviceRectangle.right,DeviceRectangle.bottom);
               }
               break;
            case HST_NAV_PREV_POST:
               {
                  ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);
                  theApp.GetNavImage(false, false, false, false, false)->Blit(hDC,m_hotspots[i].m_spot);
                  ::IntersectClipRect(hDC,DeviceRectangle.left,DeviceRectangle.top + m_banneroffset,DeviceRectangle.right,DeviceRectangle.bottom);
               }
               break;
            case HST_NAV_NEXT_POST:
               {
                  ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);
                  theApp.GetNavImage(false, false, true, false, false)->Blit(hDC,m_hotspots[i].m_spot);
                  ::IntersectClipRect(hDC,DeviceRectangle.left,DeviceRectangle.top + m_banneroffset,DeviceRectangle.right,DeviceRectangle.bottom);
               }
               break;
            case HST_NAV_NEXT_THREAD:
               {
                  ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);
                  theApp.GetNavImage(true, false, true, false, false)->Blit(hDC,m_hotspots[i].m_spot);
                  ::IntersectClipRect(hDC,DeviceRectangle.left,DeviceRectangle.top + m_banneroffset,DeviceRectangle.right,DeviceRectangle.bottom);
               }
               break;
            case HST_NAV_PREV_NEW_POST:
               {
                  ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);
                  theApp.GetNavImage(false, true, false, false, false)->Blit(hDC,m_hotspots[i].m_spot);
                  ::IntersectClipRect(hDC,DeviceRectangle.left,DeviceRectangle.top + m_banneroffset,DeviceRectangle.right,DeviceRectangle.bottom);
               }
               break;
            case HST_NAV_NEXT_NEW_POST:
               {
                  ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);
                  theApp.GetNavImage(false, true, true, false, false)->Blit(hDC,m_hotspots[i].m_spot);
                  ::IntersectClipRect(hDC,DeviceRectangle.left,DeviceRectangle.top + m_banneroffset,DeviceRectangle.right,DeviceRectangle.bottom);
               }
               break;
            case HST_DEMOTE:
               {
                  ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);
                  theApp.GetDemoteImage(true, false)->Blit(hDC,m_hotspots[i].m_spot);
                  ::IntersectClipRect(hDC,DeviceRectangle.left,DeviceRectangle.top + m_banneroffset,DeviceRectangle.right,DeviceRectangle.bottom);
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
                  theApp.GetNewThreadImage(false)->Blit(hDC,m_hotspots[i].m_spot,true,0,0);
                  ::IntersectClipRect(hDC,DeviceRectangle.left,DeviceRectangle.top + m_banneroffset,DeviceRectangle.right,DeviceRectangle.bottom);
               }
               break;
            case HST_COMPOSE_MESSAGE:
               {
                  ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);
                  theApp.GetComposeImage(false)->Blit(hDC,m_hotspots[i].m_spot,true,0,0);
                  ::IntersectClipRect(hDC,DeviceRectangle.left,DeviceRectangle.top + m_banneroffset,DeviceRectangle.right,DeviceRectangle.bottom);
               }
               break;
            case HST_SEARCH_DLG:
               {
                  ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);
                  theApp.GetSearchImage(false)->Blit(hDC,m_hotspots[i].m_spot,true,0,0);
                  ::IntersectClipRect(hDC,DeviceRectangle.left,DeviceRectangle.top + m_banneroffset,DeviceRectangle.right,DeviceRectangle.bottom);
               }
               break;
            case HST_REFRESHSTORY:
               {
                  ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);
                  theApp.GetRefreshStoryImage(false)->Blit(hDC,m_hotspots[i].m_spot,true,0,0);
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
            case HST_FLAG:
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
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_RED,m_hotspots[i].m_spot,false);
                  }
               }
               break;
            case HST_TAG_GREEN:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_GREEN,m_hotspots[i].m_spot,false);
                  }
               }
               break;
            case HST_TAG_BLUE:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_BLUE,m_hotspots[i].m_spot,false);
                  }
               }
               break;
            case HST_TAG_YELLOW:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_YELLOW,m_hotspots[i].m_spot,false);
                  }
               }
               break;
            case HST_TAG_OLIVE:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_OLIVE,m_hotspots[i].m_spot,false);
                  }
               }
               break;
            case HST_TAG_LIME:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_LIME,m_hotspots[i].m_spot,false);
                  }
               }
               break;
            case HST_TAG_ORANGE:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_ORANGE,m_hotspots[i].m_spot,false);
                  }
               }
               break;
            case HST_TAG_PINK:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_PINK,m_hotspots[i].m_spot,false);
                  }
               }
               break;
            case HST_TAG_ITALICS:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_ITALIC,m_hotspots[i].m_spot,false);
                  }
               }
               break;
            case HST_TAG_BOLD:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_BOLD,m_hotspots[i].m_spot,false);
                  }
               }
               break;
            case HST_TAG_QUOTE:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_QUOTE,m_hotspots[i].m_spot,false);
                  }
               }
               break;
            case HST_TAG_SAMPLE:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_SAMPLE,m_hotspots[i].m_spot,false);
                  }
               }
               break;
            case HST_TAG_UNDERLINE:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_UNDERLINE,m_hotspots[i].m_spot,false);
                  }
               }
               break;
            case HST_TAG_STRIKE:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_STRIKE,m_hotspots[i].m_spot,false);
                  }
               }
               break;
            case HST_TAG_SPOILER:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_SPOILER,m_hotspots[i].m_spot,false);
                  }
               }
               break;
            case HST_TAG_CODE:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_CODE,m_hotspots[i].m_spot,false);
                  }
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
            case HST_UGHTAG:
               {
                  GetDocument()->DrawLOLField(hDC, LTT_UGH, m_hotspots[i].m_spot, m_hotspots[i].m_loltext, false, m_hotspots[i].m_lolvoted, m_hotspots[i].m_lolroot, m_hotspots[i].m_haslols);
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
            case HST_NAV_PREV_THREAD:
               {
                  ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);
                  theApp.GetNavImage(true, false, false, true, false)->Blit(hDC,m_hotspots[i].m_spot);
                  ::IntersectClipRect(hDC,DeviceRectangle.left,DeviceRectangle.top + m_banneroffset,DeviceRectangle.right,DeviceRectangle.bottom);
               }
               break;
            case HST_NAV_PREV_POST:
               {
                  ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);
                  theApp.GetNavImage(false, false, false, true, false)->Blit(hDC,m_hotspots[i].m_spot);
                  ::IntersectClipRect(hDC,DeviceRectangle.left,DeviceRectangle.top + m_banneroffset,DeviceRectangle.right,DeviceRectangle.bottom);
               }
               break;
            case HST_NAV_NEXT_POST:
               {
                  ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);
                  theApp.GetNavImage(false, false, true, true, false)->Blit(hDC,m_hotspots[i].m_spot);
                  ::IntersectClipRect(hDC,DeviceRectangle.left,DeviceRectangle.top + m_banneroffset,DeviceRectangle.right,DeviceRectangle.bottom);
               }
               break;
            case HST_NAV_NEXT_THREAD:
               {
                  ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);
                  theApp.GetNavImage(true, false, true, true, false)->Blit(hDC,m_hotspots[i].m_spot);
                  ::IntersectClipRect(hDC,DeviceRectangle.left,DeviceRectangle.top + m_banneroffset,DeviceRectangle.right,DeviceRectangle.bottom);
               }
               break;
            case HST_NAV_PREV_NEW_POST:
               {
                  ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);
                  theApp.GetNavImage(false, true, false, true, false)->Blit(hDC,m_hotspots[i].m_spot);
                  ::IntersectClipRect(hDC,DeviceRectangle.left,DeviceRectangle.top + m_banneroffset,DeviceRectangle.right,DeviceRectangle.bottom);
               }
               break;
            case HST_NAV_NEXT_NEW_POST:
               {
                  ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);
                  theApp.GetNavImage(false, true, true, true, false)->Blit(hDC,m_hotspots[i].m_spot);
                  ::IntersectClipRect(hDC,DeviceRectangle.left,DeviceRectangle.top + m_banneroffset,DeviceRectangle.right,DeviceRectangle.bottom);
               }
               break;
            case HST_DEMOTE:
               {
                  ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);
                  theApp.GetDemoteImage(true, true)->Blit(hDC,m_hotspots[i].m_spot);
                  ::IntersectClipRect(hDC,DeviceRectangle.left,DeviceRectangle.top + m_banneroffset,DeviceRectangle.right,DeviceRectangle.bottom);
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
            case HST_SEARCH_DLG:
               {
                  ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);
                  theApp.GetSearchImage(true)->Blit(hDC,m_hotspots[i].m_spot);
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
            case HST_FLAG:
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
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_RED,m_hotspots[i].m_spot,true);
                  }
               }
               break;
            case HST_TAG_GREEN:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_GREEN,m_hotspots[i].m_spot,true);
                  }
               }
               break;
            case HST_TAG_BLUE:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_BLUE,m_hotspots[i].m_spot,true);
                  }
               }
               break;
            case HST_TAG_YELLOW:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_YELLOW,m_hotspots[i].m_spot,true);
                  }
               }
               break;
            case HST_TAG_OLIVE:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_OLIVE,m_hotspots[i].m_spot,true);
                  }
               }
               break;
            case HST_TAG_LIME:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_LIME,m_hotspots[i].m_spot,true);
                  }
               }
               break;
            case HST_TAG_ORANGE:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_ORANGE,m_hotspots[i].m_spot,true);
                  }
               }
               break;
            case HST_TAG_PINK:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_PINK,m_hotspots[i].m_spot,true);
                  }
               }
               break;
            case HST_TAG_ITALICS:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_ITALIC,m_hotspots[i].m_spot,true);
                  }
               }
               break;
            case HST_TAG_BOLD:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_BOLD,m_hotspots[i].m_spot,true);
                  }
               }
               break;
            case HST_TAG_QUOTE:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_QUOTE,m_hotspots[i].m_spot,true);
                  }
               }
               break;
            case HST_TAG_SAMPLE:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_SAMPLE,m_hotspots[i].m_spot,true);
                  }
               }
               break;
            case HST_TAG_UNDERLINE:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_UNDERLINE,m_hotspots[i].m_spot,true);
                  }
               }
               break;
            case HST_TAG_STRIKE:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_STRIKE,m_hotspots[i].m_spot,true);
                  }
               }
               break;
            case HST_TAG_SPOILER:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_SPOILER,m_hotspots[i].m_spot,true);
                  }
               }
               break;
            case HST_TAG_CODE:
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->DrawShackTag(hDC,ST_CODE,m_hotspots[i].m_spot,true);
                  }
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
            case HST_UGHTAG:
               {
                  GetDocument()->DrawLOLField(hDC, LTT_UGH, m_hotspots[i].m_spot, m_hotspots[i].m_loltext, true, m_hotspots[i].m_lolvoted, m_hotspots[i].m_lolroot, m_hotspots[i].m_haslols);
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

int CLampView::GetViewPosition()
{
   int result = INT32_MAX;

   if(GetCurrentId() != 0)
   {
      // force a draw so that positions are updated
      ChattyPost *pPost = GetDocument()->FindPost(GetCurrentId());
      if(pPost != NULL)
      {            
         CancelInertiaPanning();
         m_brakes = false;

         result = pPost->GetPos();
      }
   }

   return result;
}

void CLampView::UpdateViewPosition(int pos)
{
   if(GetCurrentId() != 0 && pos != INT32_MAX)
   {
      // force a draw so that positions are updated
      ChattyPost *pPost = GetDocument()->FindPost(GetCurrentId());
      if(pPost != NULL)
      {            
         DrawEverythingToBuffer();
         int newpos = pPost->GetPos();         
         m_gotopos = m_pos;
         int offset = newpos - pos;
         m_gotopos += offset;
         MakePosLegal();
         m_pos = m_gotopos;
         InvalidateEverything();
      }
   }
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

         int bordersize = 20;
         if(theApp.TightFitSummary())
         {
            bordersize = 10;
         }
                  
         DeviceRectangle.top += (bordersize + m_banneroffset);
         DeviceRectangle.bottom -= bordersize;

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

void CLampView::InvalidateFlags()
{
   GetDocument()->InvalidateFlags();
}

void CLampView::CloseFindDlg()
{
   if(m_pFindDlg != NULL)
   {
      m_pFindDlg->PostMessageW(WM_CLOSE);
   }
}

void CLampView::FindText(bool fromstart /*= false*/)
{
   if(fromstart)
   {
      m_textselectionpost = 0;
      m_selectionstart = 0;
      m_selectionend = 0;
   }
   else if(GetDocument()->GetDataType() == DDT_STORY &&
           theApp.LatestChattySummaryMode())
   {
      CLampDoc *pAT = theApp.GetActiveThread();

      if(pAT != NULL &&
         pAT->GetView() != NULL)
      {
         pAT->GetView()->GetSelection(m_textselectionpost, m_selectionstart, m_selectionend);
      }
   }

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

            if(GetDocument()->GetDataType() == DDT_STORY &&
               theApp.LatestChattySummaryMode())
            {
               if(pParent != NULL)
               {
                  // show which thread it is in
                  SetCurrentId(pParent->GetId(), false);

                  // now make the active thread tab show the result
                  CLampDoc *pAT = theApp.GetActiveThread();

                  if(pAT != NULL &&
                     pAT->GetView() != NULL)
                  {
                     pAT->GetView()->ShowTextSelection(m_textselectionpost, m_selectionstart, m_selectionend);
                  }

                  // but I don't show result
                  m_textselectionpost = 0;
                  m_selectionstart = 0;
                  m_selectionend = 0;
               }
            }
            else
            {
               pParent->UnShowAsTruncated();
               if(pParent->IsCollapsed())
               {
                  pParent->Expand();
                  theApp.RemoveMyCollapse(pParent->GetId());
               }

               SetCurrentId(m_textselectionpost, false);
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
}


void CLampView::ShowTextSelection(unsigned int textselectionpost, int selectionstart, int selectionend)
{
   SetSelection(textselectionpost, selectionstart, selectionend);

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

         SetCurrentId(m_textselectionpost, false);
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
         case HST_FLAG: 
            {
               theApp.SetStatusBarText(L"View Note About User",this);
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
         case HST_NAV_PREV_THREAD:
            {
               theApp.SetStatusBarText(L"Previous Thread",this);
            }
            break;
         case HST_NAV_PREV_POST:
            {
               theApp.SetStatusBarText(L"Previous Post",this);
            }
            break;
         case HST_NAV_NEXT_POST:
            {
               theApp.SetStatusBarText(L"Next Post",this);
            }
            break;
         case HST_NAV_NEXT_THREAD:
            {
               theApp.SetStatusBarText(L"Next Thread",this);
            }
            break;
         case HST_NAV_PREV_NEW_POST:
            {
               theApp.SetStatusBarText(L"Previous New Post",this);
            }
            break;
         case HST_NAV_NEXT_NEW_POST:
            {
               theApp.SetStatusBarText(L"Next New Post",this);
            }
            break;
         case HST_DEMOTE:
            {
               theApp.SetStatusBarText(L"Demote New-ness of Thread",this);
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
         case HST_SEARCH_DLG:
            {
               theApp.SetStatusBarText(L"Change Search Conditions",this);
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
   CLampView *targetview = ((CMainFrame*)theApp.GetMainWnd())->GetTargetView();

   if(targetview != NULL)
   {
      targetview->OnMouseWheelImpl(nFlags, zDelta, pt); 
   }
   else
   {
      OnMouseWheelImpl(nFlags, zDelta, pt); 
   }

   return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CLampView::OnMouseWheelImpl(UINT nFlags, short zDelta, CPoint pt) 
{
   m_bDrawMButtonDownIcon = false;
   m_bMButtonDown = false;
   //m_bStartedTrackingMouse = false;

   if(m_pReplyDlg == NULL ||
      !m_pReplyDlg->OnMouseWheel(nFlags, zDelta, m_mousepoint))
   {
      m_gotopos -= (int)((float)zDelta * theApp.GetMouseWheelScale());
      MakePosLegal();
      InvalidateEverythingPan();
      MakeMousePointInvalid();
   }
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


void CLampView::OnClick(CPoint point) 
{
   bool bJustPutUpModTool = false;

   GetDocument()->Viewed();

   if(!m_brakes)
   {
      bool bContinue = true;

      if(!GetDocument()->IsBusy())
      {
         if(m_pReplyDlg != NULL)
         {
            bool bCloseReplyDlg = false;
            bContinue = !m_pReplyDlg->OnLButtonDown(0, point, bCloseReplyDlg);

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
                           int scrollwidth = 16;
                           if(theApp.BigSkin()) scrollwidth = 32;

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
                                 m_gotopos = ((int)((float)(m_mousepoint.y - scrollwidth - m_ScrollRectangle.top) * (1.0f / m_scrollscale))) - ((m_ScrollRectangle.bottom - m_ScrollRectangle.top) >> 1);
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
                                 m_gotopos = ((int)((float)(m_mousepoint.y - scrollwidth - m_ScrollRectangle.top) * (1.0f / m_scrollscale))) - ((m_ScrollRectangle.bottom - m_ScrollRectangle.top) >> 1);
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
                     case HST_FLAG:
                        {
                           ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
                           if(pPost != NULL)
                           {
                              CFlagUserDialog dlg(this);
                              dlg.m_name = pPost->GetAuthor();
                              dlg.DoModal();
                           }
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
                     case HST_NAV_PREV_THREAD:
                        {
                           PrevThread();
                        }
                        break;
                     case HST_NAV_PREV_POST:
                        {
                           PrevPost();
                        }
                        break;
                     case HST_NAV_NEXT_POST:
                        {
                           NextPost();
                        }
                        break;
                     case HST_NAV_NEXT_THREAD:
                        {
                           NextThread();
                        }
                        break;
                     case HST_NAV_PREV_NEW_POST:
                        {
                           PrevNewPost();
                        }
                        break;
                     case HST_NAV_NEXT_NEW_POST:
                        {
                           NextNewPost();
                        }
                        break;
                     case HST_DEMOTE:
                        {
                           DemoteNewness();
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
                     case HST_SEARCH_DLG:
                        {
                           if(GetDocument()->GetDataType() == DDT_SEARCH)
                           {
                              CCustomSearchDlg csdlg(this);

                              UCString search_author, search_parent_author, search_terms, search_filter, search_sort;

                              GetDocument()->GetSearchConditions(search_author, search_parent_author, search_terms, search_filter, search_sort);

                              csdlg.m_user = search_author;
                              csdlg.m_parent = search_parent_author;
                              csdlg.m_terms = search_terms;
                              csdlg.m_filter = search_filter;
                              csdlg.m_sort = search_sort;

                              if(csdlg.DoModal() == IDOK)
                              {
                                 search_author = csdlg.m_user;
                                 search_parent_author = csdlg.m_parent;
                                 search_terms = csdlg.m_terms;
                                 search_filter = csdlg.m_filter;
                                 search_sort = csdlg.m_sort;

                                 GetDocument()->SetSearchConditions(search_author, search_parent_author, search_terms, search_filter, search_sort);
                                 GetDocument()->SetPage(0);
                                 GetDocument()->Refresh();
                              }
                           }
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
                              m_hover_preview_percent = 1.0;
                              m_hover_preview_id = 0;
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
                           CloseNonReplyOnlyDlg();

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
                              m_bLBDownOnDblClkable = true;
                           }
                        }
                        break;
                     case HST_POST_AREA:
                        {
                           ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
                           if(pPost != NULL)
                           {
                              unsigned int id = m_hotspots[i].m_id;
                              UpdateCurrentIdAsRoot(id);
                              m_textselectionpost = id;
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
                                    bool NWS = false;
                                    if(pPost->IsNWSPost() || 
                                       GetAsyncKeyState(VK_CONTROL) & 0xFF00)
                                    {
                                       NWS = true;
                                    }
                                    theApp.OpenShackLink(link,NWS);
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

                              //RecalcImgurLink(link);

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

                              //RecalcImgurLink(link);

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
                              InvalidateEverything();
                           }
                        }
                        break;
                     case HST_UGHTAG:
                        {
                           ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
                           if(pPost != NULL)
                           {
                              UCString msg;
                              if(m_hotspots[i].m_lolvoted)
                              {
                                 msg = L"Are you sure you wish to Un-UGH this post?";
                              }
                              else
                              {
                                 msg = L"Are you sure you wish to UGH this post?";
                              }
                              int ret = MessageBox(msg,L"Lamp",MB_YESNO);
                              if(ret == IDYES)
                              {
                                 GetDocument()->LolTagPost(m_hotspots[i].m_id, LTT_UGH);
                                 InvalidateEverything();
                              }
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
}


void CLampView::OnLButtonDown(UINT nFlags, CPoint point) 
{
   m_bLButtonDown = true;
   SetFocus();
   SetCapture();   

   CHotSpot *hotspot = GetHotspot(point);

   bool takeit = false;

   if(hotspot != NULL &&
     (hotspot->m_type == HST_NAV_PREV_THREAD ||
      hotspot->m_type == HST_NAV_PREV_POST ||
      hotspot->m_type == HST_NAV_NEXT_POST ||
      hotspot->m_type == HST_NAV_NEXT_THREAD ||
      hotspot->m_type == HST_NAV_PREV_NEW_POST ||
      hotspot->m_type == HST_NAV_NEXT_NEW_POST ||
      hotspot->m_type == HST_DEMOTE ||
      hotspot->m_type == HST_NULL_BACKGROUND))
   {
      takeit = true;
   }
   else
   {
      m_gotopos = m_pos;
      CancelInertiaPanning();
   }
   m_bStartedTrackingMouse = true;
   m_lastmousetime = m_last_user_activity = ::GetTickCount();
   m_mousepoint = point;
   m_bDrawMButtonDownIcon = false;
   m_bDoubleClickDragging = false;
   m_bLBDownOnDblClkable = false;
     
   
   if(!takeit &&
      theApp.LeftMousePan() &&
      (hotspot == NULL ||
      (hotspot->m_type != HST_SCROLLBAR &&
       hotspot->m_type != HST_SCROLLBAR_UP &&
       hotspot->m_type != HST_SCROLLBAR_DOWN &&
       hotspot->m_type != HST_SCROLLBAR_THUMB &&
       hotspot->m_type != HST_SCROLLBAR_REPLYTEXT_DLG &&
       hotspot->m_type != HST_SCROLLBAR_UP_REPLYTEXT_DLG &&
       hotspot->m_type != HST_SCROLLBAR_DOWN_REPLYTEXT_DLG &&
       hotspot->m_type != HST_SCROLLBAR_THUMB_REPLYTEXT_DLG &&
       
       hotspot->m_type != HST_NAV_PREV_THREAD &&
       hotspot->m_type != HST_NAV_PREV_POST &&
       hotspot->m_type != HST_NAV_NEXT_POST &&
       hotspot->m_type != HST_NAV_NEXT_THREAD &&
       hotspot->m_type != HST_NAV_PREV_NEW_POST &&
       hotspot->m_type != HST_NAV_NEXT_NEW_POST &&
       hotspot->m_type != HST_DEMOTE )))
   {
      // start left mouse timer
      if(m_leftmouse_timer_active)
      {
         KillTimer(LEFTMOUSE_TIMER);
         m_leftmouse_timer_active = false;
      }
      SetTimer(LEFTMOUSE_TIMER,500,NULL);
      m_leftmouse_timer_active = true;

      m_panpoint = point;

      if(hotspot != NULL &&
         hotspot->m_type == HST_REPLYTEXT &&
         m_pReplyDlg != NULL)
      {
         m_reply_start_pan_pos = m_pReplyDlg->m_gotopos;
         m_lmb_in_reply = true;
      }
      else
      {
         m_panpos = m_pos;
      }
   }
   else
   {
      OnClick(point);
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
   m_bLButtonDown = false;
   ReleaseCapture();
   TrackMouse(point);
   m_indent_panning = false;
   m_lmb_in_reply = false;

   if(m_leftmouse_timer_active)
   {
      KillTimer(LEFTMOUSE_TIMER);
      m_leftmouse_timer_active = false;

      if(!m_bPanning && !m_panning_reply)
      {
         CPoint temppoint = m_mousepoint;
         m_mousepoint = m_panpoint;
         OnClick(m_panpoint);
         OnMouseMove(0,point);
         m_mousepoint = temppoint;
      }
   }

   if(!m_brakes)
   {
      if(!GetDocument()->IsBusy())
      {
         if(m_pReplyDlg == NULL ||
            !m_pReplyDlg->OnLButtonUp(nFlags, point))
         {
            if(m_panning_reply)
            {
               if(m_pReplyDlg != NULL)
               {
                  m_pReplyDlg->m_gotopos = m_reply_start_pan_pos - (point.y - m_panpoint.y);
                  m_pReplyDlg->MakePosLegal();
                  m_pReplyDlg->m_pos = m_pReplyDlg->m_gotopos;
                  InvalidateEverything();
               }
               m_panning_reply = false;
            }
            else if(m_bLButtonDownOnScrollArrow)
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
      else if(m_bPanning)
      {
         m_bPanning = false;
         int ydiff = m_panpoint.y - point.y;
         m_gotopos = m_panpos + ydiff;
         MakePosLegal();
         m_pos = m_gotopos;// no smooth on pan
         BeginInertiaPanning();
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
   ((CMainFrame*)theApp.GetMainWnd())->SetTargetView(this);

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

   if(m_leftmouse_timer_active)
   {
      if(abs(m_panpoint.x - point.x) > 20 ||
         abs(m_panpoint.y - point.y) > 10)
      {
         // panning
         KillTimer(LEFTMOUSE_TIMER);
         m_leftmouse_timer_active = false;
         m_hover_preview_percent = 1.0;
         m_hover_preview_id = 0;

         if(abs(m_panpoint.y - point.y) > 10)
         {
            if(m_lmb_in_reply)
            {
               if(m_pReplyDlg != NULL)
               {
                  m_panning_reply = true;
                  m_pReplyDlg->m_gotopos = m_reply_start_pan_pos - (point.y - m_panpoint.y);
                  m_pReplyDlg->MakePosLegal();
                  m_pReplyDlg->m_pos = m_pReplyDlg->m_gotopos;
                  InvalidateEverything();
               }
            }
            else
            {
               m_bPanning = true;
               int ydiff = m_panpoint.y - point.y;
               m_gotopos = m_panpos + ydiff;
               MakePosLegal();
               m_pos = m_gotopos;// no smooth on pan
               InvalidateEverythingPan();
            }
         }
         else
         {
            bool bCtrlPressed = false;
            if(GetAsyncKeyState(VK_CONTROL) & 0xFF00)
            {
               bCtrlPressed = true;
            }

            if(bCtrlPressed)
            {
               ChattyPost *pPost = GetDocument()->FindPost(GetCurrentId());
               if(pPost != NULL)
               {
                  ChattyPost *pRoot = pPost->GetRoot();
                  if(pRoot != NULL)
                  {
                     m_indent_down = pRoot->GetIndentOffset();
                     pRoot->SetIndentOffset(m_indent_down + (point.x - m_panpoint.x));
                     InvalidateEverything();
                     m_indent_panning = true;
                  }
               }
            }
            else
            {
               CPoint temppoint = m_mousepoint;
               m_mousepoint = m_panpoint;
               OnClick(m_panpoint);
               OnMouseMove(0,point);
               m_mousepoint = temppoint;
            }
         }
      }
      else
      {
         // clicking
      }
   }
   else
   {
      if(!m_brakes)
      {
         if(!GetDocument()->IsBusy())
         {
            if(m_pReplyDlg == NULL ||
               !m_pReplyDlg->OnMouseMove(nFlags, point))
            {
               if(m_indent_panning)
               {
                  ChattyPost *pPost = GetDocument()->FindPost(GetCurrentId());
                  if(pPost != NULL)
                  {
                     ChattyPost *pRoot = pPost->GetRoot();
                     if(pRoot != NULL)
                     {
                        pRoot->SetIndentOffset( m_indent_down + (point.x - m_panpoint.x));
                        InvalidateEverything();
                     }
                  }
               }
               else if(m_panning_reply)
               {
                  if(m_pReplyDlg != NULL)
                  {
                     m_pReplyDlg->m_gotopos = m_reply_start_pan_pos - (point.y - m_panpoint.y);
                     m_pReplyDlg->MakePosLegal();
                     m_pReplyDlg->m_pos = m_pReplyDlg->m_gotopos;
                     InvalidateEverything();
                  }
               }
               else if((m_bMButtonDown ||
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
                     if(m_mousepoint.x >= m_BannerRectangle.left &&
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
   }

   CView::OnMouseMove(nFlags, point);
}

void CLampView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
   if(m_bLBDownOnDblClkable)
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
   else
   {
      OnLButtonDown(0, point);
   }
}

void CLampView::OnMButtonDown(UINT nFlags, CPoint point) 
{
   if(m_gotopos == m_pos)
   {
      SetCapture();   
      CancelInertiaPanning();
      m_bStartedTrackingMouse = true;
      m_lastmousetime = m_last_user_activity = ::GetTickCount();
      m_mousepoint = point;

      if(!m_brakes)
      {   
         if(m_bDrawMButtonDownIcon == false)
         {
            m_MButtonDownPoint = point;
            m_mbuttondowntime = m_last_user_activity;
            m_bMButtonDown = true;
         }
         m_bDrawMButtonDownIcon = false;
      }

      if(m_bModToolIsUp)
      {
         m_bModToolIsUp = false;
         InvalidateEverything();
      }
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
                     if(GetDocument()->GetDataType() == DDT_STORY ||
                        GetDocument()->GetDataType() == DDT_ACTIVE_THREAD)
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
                        bool NWS = false;
                        if(pPost->IsNWSPost() || 
                           GetAsyncKeyState(VK_CONTROL) & 0xFF00)
                        {
                           NWS = true;
                        }
                        UCString link;
                        pPost->GetLink(m_mousepoint.x, m_mousepoint.y, link);
                        theApp.OpenShackLink(link,NWS);
                     }
                     bHandled = true;
                  }
                  break;
               case HST_IMAGE_LINK:
                  {
                     ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
                     if(pPost != NULL)
                     {
                        bool NWS = false;
                        if(pPost->IsNWSPost() || 
                           GetAsyncKeyState(VK_CONTROL) & 0xFF00)
                        {
                           NWS = true;
                        }
                        UCString link;
                        pPost->GetImageLink(m_mousepoint.x, m_mousepoint.y, link);
                        theApp.OpenShackLink(link,NWS);
                     }
                     bHandled = true;
                  }
                  break;
               case HST_IMAGE:
                  {
                     ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
                     if(pPost != NULL)
                     {
                        bool NWS = false;
                        if(pPost->IsNWSPost() || 
                           GetAsyncKeyState(VK_CONTROL) & 0xFF00)
                        {
                           NWS = true;
                        }
                        UCString link;
                        pPost->GetLinkToImage(m_mousepoint.x, m_mousepoint.y, link);
                        theApp.OpenShackLink(link,NWS);
                     }
                     bHandled = true;
                  }
                  break;
               case HST_THUMB:
                  {
                     ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
                     if(pPost != NULL)
                     {
                        bool NWS = false;
                        if(pPost->IsNWSPost() || 
                           GetAsyncKeyState(VK_CONTROL) & 0xFF00)
                        {
                           NWS = true;
                        }
                        UCString link;
                        pPost->GetLinkToThumb(m_mousepoint.x, m_mousepoint.y, link);
                        theApp.OpenShackLink(link,NWS);
                     }
                     bHandled = true;
                  }
                  break;
               case HST_AUTHOR:
               case HST_AUTHORPREVIEW:
                  {
                     ChattyPost *post = GetDocument()->FindPost(m_hotspots[i].m_id);
                     if(post != NULL)
                     {
                        UCString authorname = post->GetAuthor();
                        if(!authorname.IsEmpty())
                        {
                           theApp.SetLastSearchParms(authorname, UCString(), UCString(), UCString(), UCString());

                           UCString path = L"PROFILE:";
                           path += authorname;

                           theApp.OpenDocumentFile(path);
                           bHandled = true;
                        }
                     }
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
   m_last_user_activity = ::GetTickCount();

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
      else if(nChar == VK_F8)
      {
         theApp.UpdateTabNames();
      }
      else if(nChar == VK_F9)
      {
         theApp.RefreshATab();
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
               FindText();
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
                  pRoot->SetIndentOffset( pRoot->GetIndentOffset() + 20);
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
                  pRoot->SetIndentOffset( pRoot->GetIndentOffset() - 20);
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

void CLampView::NextPost()
{
   if(GetDocument()->GetDataType() == DDT_STORY &&
      theApp.LatestChattySummaryMode())
   {
      CLampDoc *pAT = theApp.GetActiveThread();
      if(pAT != NULL)
      {
         theApp.SetActiveTabDoc(pAT);
         if(pAT->GetView() != NULL)
         {
            pAT->GetView()->NextPost();
         }
      }
   }
   else
   {
      ChattyPost *pPost = GetDocument()->FindPost(GetCurrentId());
      if(pPost != NULL)
      {
         // select next reply
         SetCurrentId(pPost->GetNextReply());
         m_textselectionpost = 0;
         m_selectionstart = 0;
         m_selectionend = 0;
         // force a draw so that positions are updated
         MakeCurrentPostLegal();
      }
   }
}

void CLampView::PrevPost()
{
   if(GetDocument()->GetDataType() == DDT_STORY &&
      theApp.LatestChattySummaryMode())
   {
      CLampDoc *pAT = theApp.GetActiveThread();
      if(pAT != NULL)
      {
         theApp.SetActiveTabDoc(pAT);
         if(pAT->GetView() != NULL)
         {
            pAT->GetView()->PrevPost();
         }
      }
   }
   else
   {
      ChattyPost *pPost = GetDocument()->FindPost(GetCurrentId());
      if(pPost != NULL)
      {
         // select prev reply
         SetCurrentId(pPost->GetPrevReply());
         m_textselectionpost = 0;
         m_selectionstart = 0;
         m_selectionend = 0;
         // force a draw so that positions are updated
         MakeCurrentPostLegal();
      }
   }
}

void CLampView::NextThread()
{
   if(GetDocument()->GetDataType() == DDT_ACTIVE_THREAD)
   {
      CLampDoc *pAT = theApp.GetLatestChatty();
      if(pAT != NULL)
      {
         theApp.SetLatestChattyActive();
         if(pAT->GetView() != NULL)
         {
            pAT->GetView()->NextThread();
         }
      }
   }
   else
   {
      if(GetDocument()->GetDataType() != DDT_THREAD &&
         GetDocument()->GetDataType() != DDT_ACTIVE_THREAD &&
         GetDocument()->GetDataType() != DDT_SHACKMSG)
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
         MakeCurrentPostLegal(true);
      }
   }
}

void CLampView::PrevThread()
{
   if(GetDocument()->GetDataType() == DDT_ACTIVE_THREAD)
   {
      CLampDoc *pAT = theApp.GetLatestChatty();
      if(pAT != NULL)
      {
         theApp.SetLatestChattyActive();
         if(pAT->GetView() != NULL)
         {
            pAT->GetView()->PrevThread();
         }
      }
   }
   else
   {
      if(GetCurrentId() != 0 &&
         GetDocument()->GetDataType() != DDT_THREAD &&
         GetDocument()->GetDataType() != DDT_ACTIVE_THREAD &&
         GetDocument()->GetDataType() != DDT_SHACKMSG)
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
         MakeCurrentPostLegal(true);
      }
   }
}

void CLampView::NextNewPost()
{
   if(GetDocument()->GetDataType() == DDT_STORY &&
      theApp.LatestChattySummaryMode())
   {
      CLampDoc *pAT = theApp.GetActiveThread();
      if(pAT != NULL)
      {
         theApp.SetActiveTabDoc(pAT);
         if(pAT->GetView() != NULL)
         {
            pAT->GetView()->NextNewPost();
         }
      }
   }
   else
   {
      ChattyPost *pPost = GetDocument()->FindPost(GetCurrentId());
      if(pPost != NULL)
      {
         // select next reply
         SetCurrentId(pPost->GetNextNewReply());
         m_textselectionpost = 0;
         m_selectionstart = 0;
         m_selectionend = 0;
         // force a draw so that positions are updated
         MakeCurrentPostLegal();
      }
   }
}

void CLampView::PrevNewPost()
{
   if(GetDocument()->GetDataType() == DDT_STORY &&
      theApp.LatestChattySummaryMode())
   {
      CLampDoc *pAT = theApp.GetActiveThread();
      if(pAT != NULL)
      {
         theApp.SetActiveTabDoc(pAT);
         if(pAT->GetView() != NULL)
         {
            pAT->GetView()->PrevNewPost();
         }
      }
   }
   else
   {
      ChattyPost *pPost = GetDocument()->FindPost(GetCurrentId());
      if(pPost != NULL)
      {
         // select prev reply
         SetCurrentId(pPost->GetPrevNewReply());
         m_textselectionpost = 0;
         m_selectionstart = 0;
         m_selectionend = 0;
         // force a draw so that positions are updated
         MakeCurrentPostLegal();
      }
   }
}

void CLampView::DemoteNewness()
{
   if(GetDocument()->GetDataType() == DDT_STORY &&
      theApp.LatestChattySummaryMode())
   {
      CLampDoc *pAT = theApp.GetActiveThread();
      if(pAT != NULL)
      {
         theApp.SetActiveTabDoc(pAT);
         if(pAT->GetView() != NULL)
         {
            pAT->GetView()->DemoteNewness();
         }
      }
   }
   else
   {
      ChattyPost *pPost = GetDocument()->FindPost(GetCurrentId());
      if(pPost != NULL)
      {
         if(pPost->CanDemote())
         {
            GetDocument()->DemoteNewness(pPost->GetId());
            GetDocument()->Viewed();
            InvalidateEverything();


            if(GetDocument()->GetDataType() == DDT_ACTIVE_THREAD &&
               GetDocument()->GetInitialPostId() != 0 &&
               GetDocument()->GetRootCount() > 0)
            {
               // update latest chatty know know about any changes from active thread
               CLampDoc *pDoc = NULL;
               ChattyPost *pExistingPost = theApp.FindFromLatestChatty(GetDocument()->GetInitialPostId(), &pDoc);
               if(pExistingPost != NULL)
               {
                  ChattyPost *post = GetDocument()->GetRootPost(GetDocument()->GetInitialPostId());
                  if(post != NULL)
                  {
                     pExistingPost->ClearChildren();
                     pExistingPost->ReadPost(post,pDoc);
                     pExistingPost->UpdateLOLs(true); // since the latest chatty is in summary mode, collect all lols
                     pExistingPost->CountNews();
                     if(pDoc != NULL && 
                        pDoc->GetView() != NULL)
                     {
                        pDoc->GetView()->InvalidateEverything();
                     }
                  }
               }
            }
         }
      }
   }
}

void CLampView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   m_last_user_activity = ::GetTickCount();

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
            if(nChar == 'a' ||
               nChar == 'A' ||
               nChar == 'z' ||
               nChar == 'Z' ||
               nChar == 'd' ||
               nChar == 'D' ||
               nChar == 'c' ||
               nChar == 'C' ||
               nChar == 'e' ||
               nChar == 'E' )
            {
               if(nChar == 'a' ||
                  nChar == 'A')
               {
                  PrevPost();
               }
               else if(nChar == 'z' ||
                       nChar == 'Z')
               {
                  NextPost();
               }
               else if(nChar == 'd' ||
                       nChar == 'D')
               {
                  PrevNewPost();
               }
               else if(nChar == 'c' ||
                       nChar == 'C')
               {
                  NextNewPost();
               }
               else if(nChar == 'e' ||
                       nChar == 'E')
               {
                  DemoteNewness();
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
               NextThread();
            }
            else if(nChar == 'k' ||
                    nChar == 'K' ||
                    nChar == 's' ||
                    nChar == 'S')
            {
               PrevThread();
            }
         }
         else if((nChar == 'r' ||
                  nChar == 'R') &&
                  GetCurrentId() != 0 &&
                 (GetDocument()->GetDataType() == DDT_STORY ||
                  GetDocument()->GetDataType() == DDT_THREAD ||
                  GetDocument()->GetDataType() == DDT_ACTIVE_THREAD) &&
                  theApp.HaveLogin())
         {
            if(GetDocument()->GetDataType() == DDT_STORY &&
               theApp.LatestChattySummaryMode())
            {
               CLampDoc *pAT = theApp.GetActiveThread();
               if(pAT != NULL)
               {
                  theApp.SetActiveTabDoc(pAT,false,true);
                  if(pAT->GetView() != NULL)
                  {
                     pAT->GetView()->OnChar(nChar, nRepCnt, nFlags);
                  }
               }
            }
            else
            {
               CloseNonReplyOnlyDlg();

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
         }
         else if((nChar == 'q' ||
                  nChar == 'Q') &&
                 GetCurrentId() != 0)
         {
            if(GetDocument()->GetDataType() == DDT_STORY &&
               theApp.LatestChattySummaryMode())
            {
               CLampDoc *pAT = theApp.GetActiveThread();
               if(pAT != NULL)
               {
                  theApp.SetActiveTabDoc(pAT);
                  if(pAT->GetView() != NULL)
                  {
                     pAT->GetView()->OnChar(nChar, nRepCnt, nFlags);
                  }
               }
            }
            else
            {
               ChattyPost *post = GetDocument()->FindPost(GetCurrentId());
               if(post != NULL)
               {
                  post->Despoil();
                  InvalidateEverything();
               }
            }
         }
         else if((nChar == 'f' ||
                  nChar == 'F') &&
                  (GetDocument()->GetDataType() == DDT_STORY ||
                   GetDocument()->GetDataType() == DDT_THREAD ||
                   GetDocument()->GetDataType() == DDT_ACTIVE_THREAD) &&
                 GetCurrentId() != 0)
         {
            if(GetDocument()->GetDataType() == DDT_STORY &&
               theApp.LatestChattySummaryMode())
            {
               CLampDoc *pAT = theApp.GetActiveThread();
               if(pAT != NULL)
               {
                  theApp.SetActiveTabDoc(pAT);
                  if(pAT->GetView() != NULL)
                  {
                     pAT->GetView()->OnChar(nChar, nRepCnt, nFlags);
                  }
               }
            }
            else
            {
               ChattyPost *post = GetDocument()->FindPost(GetCurrentId());
               if(post != NULL)
               {
                  GetDocument()->RefreshThread(GetDocument()->GetRootId(GetCurrentId()), GetCurrentId());
               }
            }
         }
         else if((nChar == 'w' ||
                  nChar == 'W') &&
                  (GetDocument()->GetDataType() == DDT_STORY ||
                   GetDocument()->GetDataType() == DDT_THREAD ||
                   GetDocument()->GetDataType() == DDT_ACTIVE_THREAD) &&
                   GetCurrentId() != 0)
         {
            if(GetDocument()->GetDataType() == DDT_STORY &&
               theApp.LatestChattySummaryMode())
            {
               CLampDoc *pAT = theApp.GetActiveThread();
               if(pAT != NULL)
               {
                  theApp.SetActiveTabDoc(pAT);
                  if(pAT->GetView() != NULL)
                  {
                     pAT->GetView()->OnChar(nChar, nRepCnt, nFlags);
                  }
               }
            }
            else
            {
               ChattyPost *post = GetDocument()->FindPost(GetCurrentId());
               if(post != NULL)
               {
                  if(post->HasOpenImageLinks())
                  {
                     post->CloseAllImageLinks();
                  }
                  else
                  {
                     post->LoadAllImageLinks();
                  }
                  InvalidateEverything();
               }
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
            case HST_FLAG: 
            case HST_REPLIESTOROOTPOSTHINT:
            case HST_OPENINTAB:
            case HST_PIN:
            case HST_NAV_PREV_THREAD:
            case HST_NAV_PREV_POST:
            case HST_NAV_NEXT_POST:
            case HST_NAV_NEXT_THREAD:
            case HST_NAV_PREV_NEW_POST:
            case HST_NAV_NEXT_NEW_POST:
            case HST_DEMOTE:
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
            case HST_SEARCH_DLG:
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
            case HST_WTFTAG:
            case HST_UGHTAG:
            case HST_NEW_MESSAGES_NOTE:
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
                  /*
                  if(m_bDraggingTextSelection ||
                     (m_pReplyDlg != NULL &&
                      m_pReplyDlg->m_bSelectingText))
                  {
                     SetCursor(::LoadCursor(NULL, IDC_CROSS));
                  }
                  else
                  {*/
                     SetCursor(::LoadCursor(NULL, IDC_IBEAM));
                  //}
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

      if(GetDocument() != NULL &&
         GetDocument()->GetDataType() == DDT_STORY &&
         theApp.LatestChattySummaryMode())
      {
         CLampDoc *active = theApp.GetActiveThread();
         if(active != NULL)
         {
            // now set focus to it
            theApp.SetActiveTabDoc(active);
            // now set focus back to the latest chatty tab
            theApp.SetLatestChattyActive();
         }
      }

      m_pFindDlg = new CFindTextDlg(this);

      m_pFindDlg->m_pView = this;
      
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
         if(GetDocument()->GetDataType() != DDT_THREAD &&
            GetDocument()->GetDataType() != DDT_ACTIVE_THREAD)
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
   CloseReplyOnlyDlg();

   if(m_pReplyDlg == NULL && 
     (GetDocument()->GetDataType() == DDT_STORY ||
      GetDocument()->GetDataType() == DDT_ACTIVE_THREAD))
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
     ((GetDocument()->GetDataType() == DDT_STORY && 
      !theApp.LatestChattySummaryMode()) ||
      GetDocument()->GetDataType() == DDT_ACTIVE_THREAD))
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
          (m_hotspots[i].m_type == HST_POST_AREA && GetDocument()->GetDataType() != DDT_SHACKMSG) ||
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
          (m_hotspots[i].m_type == HST_POST_AREA && GetDocument()->GetDataType() != DDT_SHACKMSG) ||
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

void CLampView::OnCopyPost()
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
          (m_hotspots[i].m_type == HST_POST_AREA && GetDocument()->GetDataType() != DDT_SHACKMSG) ||
          (m_hotspots[i].m_type == HST_TEXT && GetDocument()->GetDataType() != DDT_SHACKMSG)))
      {
         ChattyPost *pPost = GetDocument()->FindPost(m_hotspots[i].m_id);
         if(pPost != NULL)
         {
            UCString text = pPost->GetAuthor();
            text += L" :\r\n";
            text += pPost->GetBodyText();
            
            text.PushToClipboard();
         }
         break;
      }
   }
}

void CLampView::OnUpdateCopyPost(CCmdUI *pCmdUI)
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
          (m_hotspots[i].m_type == HST_POST_AREA && GetDocument()->GetDataType() != DDT_SHACKMSG) ||
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

void CLampView::OnCopyName()
{
   if(!m_authorname_clicked.IsEmpty())
   {
      m_authorname_clicked.PushToClipboard();
   }
}

void CLampView::OnUpdateCopyName(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

ChattyPost *CLampView::GetRMBLink(UCString &link)
{
   ChattyPost *result = NULL;

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

            result = pPost;
         }
         break;
      }
   }

   return result;
}

void CLampView::OnLaunchLink()
{
   UCString link;
   ChattyPost *post = GetRMBLink(link);
   
   if(!link.IsEmpty())
   {
      bool NWS = false;

      if((post != NULL &&
         post->IsNWSPost()) || 
         GetAsyncKeyState(VK_CONTROL) & 0xFF00)
      {
         NWS = true;
      }

      theApp.LaunchLinkInDefaultBrowser(link, NWS);
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
   else if(nIDEvent == LEFTMOUSE_TIMER)
   {
      if(m_leftmouse_timer_active)
      {
         KillTimer(LEFTMOUSE_TIMER);
         m_leftmouse_timer_active = false;

         ReleaseCapture();

         CPoint temppoint = m_mousepoint;
         m_rbuttondownpoint = m_mousepoint;
         ClientToScreen(&temppoint);
         OnContextMenu(this, temppoint);
         
         /*
            m_mousepoint = m_panpoint;
            OnClick(m_panpoint);
            m_mousepoint = temppoint;
            OnMouseMove(0,m_mousepoint);
         */
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

      theApp.UpdateTabNames();
   }
}

void CLampView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
   ((CMainFrame*)theApp.GetMainWnd())->CloseFindDlg();

   if(bActivate)
   {
      ((CMainFrame*)theApp.GetMainWnd())->SetActiveLampView(this);
      InvalidateEverything();

      if(GetDocument() != NULL)
      {
         GetDocument()->Viewed();
      }
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

void CLampView::OnOpenAllLinks()
{
   ChattyPost *pPost = GetDocument()->FindPost(m_rbuttonmenufromid);
   if(pPost != NULL)
   {
      CWaitCursor wait;
      pPost->OpenAllLinks();
   }
}

void CLampView::OnUpdateOpenAllLinks(CCmdUI *pCmdUI)
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

void CLampView::OnOpenAllGifs()
{
   ChattyPost *pPost = GetDocument()->FindPost(m_rbuttonmenufromid);
   if(pPost != NULL)
   {
      CWaitCursor wait;
      pPost->OpenAllGifs();
   }
}

void CLampView::OnUpdateOpenAllGifs(CCmdUI *pCmdUI)
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
      (GetDocument()->GetDataType() == DDT_THREAD ||
       GetDocument()->GetDataType() == DDT_ACTIVE_THREAD))
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
      (GetDocument()->GetDataType() == DDT_THREAD ||
       GetDocument()->GetDataType() == DDT_ACTIVE_THREAD))
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
      GetDocument()->GetDataType() == DDT_THREAD ||
      GetDocument()->GetDataType() == DDT_ACTIVE_THREAD))
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
      GetDocument()->GetDataType() == DDT_THREAD ||
      GetDocument()->GetDataType() == DDT_ACTIVE_THREAD))
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

void CLampView::OnDoUGH()
{
   theApp.SetDoUGH(!theApp.DoUGH());
   InvalidateEverything();
}

void CLampView::OnUpdateDoUGH(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.DoUGH())
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

void CLampView::OnLeftMousePan()
{
   theApp.LeftMousePan(!theApp.LeftMousePan());
   InvalidateEverything();
}

void CLampView::OnUpdateLeftMousePan(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.LeftMousePan())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnMoveRefreshToTop()
{
   theApp.MoveRefreshToTop(!theApp.MoveRefreshToTop());
   InvalidateEverything();
}

void CLampView::OnUpdateMoveRefreshToTop(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.MoveRefreshToTop())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnShowNavButtons()
{
   theApp.ShowNavButtons(!theApp.ShowNavButtons());
   InvalidateEverything();
}

void CLampView::OnUpdateShowNavButtons(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.ShowNavButtons())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnLatestChattySummaryMode()
{
   theApp.LatestChattySummaryMode(!theApp.LatestChattySummaryMode());
   InvalidateEverything();
}

void CLampView::OnUpdateLatestChattySummaryMode(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.LatestChattySummaryMode())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnTightFitSummary()
{
   theApp.TightFitSummary(!theApp.TightFitSummary());
   InvalidateEverything();
}

void CLampView::OnUpdateTightFitSummary(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.TightFitSummary())
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

void CLampView::OnSkinMetroShack()
{
   theApp.SetSkinFolder(L"metro");
}

void CLampView::OnUpdateSkinMetroShack(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.GetSkinFolder() == L"metro")
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}


void CLampView::OnSkinRoundShack2()
{
   theApp.SetSkinFolder(L"roundshack_2");
}

void CLampView::OnUpdateSkinRoundShack2(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.GetSkinFolder() == L"roundshack_2")
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnSkinSquareShack2()
{
   theApp.SetSkinFolder(L"default_2");
}

void CLampView::OnUpdateSkinSquareShack2(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.GetSkinFolder() == L"default_2")
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnSkinWorksafeShack2()
{
   theApp.SetSkinFolder(L"worksafe_2");
}

void CLampView::OnUpdateSkinWorksafeShack2(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.GetSkinFolder() == L"worksafe_2")
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnSkinMetroShack2()
{
   theApp.SetSkinFolder(L"metro_2");
}

void CLampView::OnUpdateSkinMetroShack2(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.GetSkinFolder() == L"metro_2")
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
      theApp.GetSkinFolder() != L"worksafe" &&
      theApp.GetSkinFolder() != L"metro" &&
      theApp.GetSkinFolder() != L"roundshack_2" &&
      theApp.GetSkinFolder() != L"default_2" &&
      theApp.GetSkinFolder() != L"worksafe_2" &&
      theApp.GetSkinFolder() != L"metro_2" )
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

void CLampView::OnAuthorGlow()
{
   theApp.SetAuthorGlow(!theApp.GetAuthorGlow());
   InvalidateEverything();
}

void CLampView::OnUpdateAuthorGlow(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.GetAuthorGlow())
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

   selectedtext.TrimWhitespace();

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

   selectedtext.TrimWhitespace();

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

   selectedtext.TrimWhitespace();

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

void CLampView::OnUrbanDicSelected()
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

   selectedtext.TrimWhitespace();

   if(!selectedtext.IsEmpty())
   {
      UCString link = L"http://www.urbandictionary.com/define.php?term=";
      char *enc = url_encode(selectedtext.str8());
      link += enc;
      free(enc);
      theApp.OpenShackLink(link);
   }
}

void CLampView::OnUpdateUrbanDicSelected(CCmdUI *pCmdUI)
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

void CLampView::OnDicSelected()
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

   selectedtext.TrimWhitespace();

   if(!selectedtext.IsEmpty())
   {
      UCString link = L"http://dictionary.reference.com/browse/";
      char *enc = url_encode(selectedtext.str8());
      link += enc;
      free(enc);
      theApp.OpenShackLink(link);
   }
}

void CLampView::OnUpdateDicSelected(CCmdUI *pCmdUI)
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

void CLampView::OnWolframSelected()
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

   selectedtext.TrimWhitespace();

   if(!selectedtext.IsEmpty())
   {
      UCString link = L"http://www.wolframalpha.com/input/?i=";
      char *enc = url_encode(selectedtext.str8());
      link += enc;
      free(enc);
      theApp.OpenShackLink(link);
   }
}

void CLampView::OnUpdateWolframSelected(CCmdUI *pCmdUI)
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


void CLampView::OnIMDBSelected()
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

   selectedtext.TrimWhitespace();

   selectedtext.ReplaceAll(L' ',L'+');

   if(!selectedtext.IsEmpty())
   {
      // http://www.imdb.com/find?q=Candice+Cameron&s=all
      UCString link = L"http://www.imdb.com/find?q=";
      char *enc = url_encode(selectedtext.str8());
      link += enc;
      link += L"&s=all";
      free(enc);
      theApp.OpenShackLink(link);
   }
}

void CLampView::OnUpdateIMDBSelected(CCmdUI *pCmdUI)
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

void CLampView::OnShowRawDate()
{
   theApp.SetShowRawDate(!theApp.ShowRawDate());
   InvalidateEverything();
   theApp.ClearKnownPosts();
}

void CLampView::OnUpdateShowRawDate(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(theApp.ShowRawDate())
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
   CloseReplyOnlyDlg();

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
      theApp.SetLastSearchParms(m_authorname_clicked, UCString(), UCString(), UCString(), UCString());

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
      theApp.SetLastSearchParms(m_authorname_clicked, UCString(), UCString(), UCString(), UCString());

      UCString path = L"CUSTOMSEARCH:";
      char *enc = url_encode(m_authorname_clicked.str8());
      path += enc;
      free(enc);
      path += L":::all:postdate_desc";

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

void CLampView::OnFlagUser()
{
   if(!m_authorname_clicked.IsEmpty())
   {
      CFlagUserDialog dlg(this);

      dlg.m_name = m_authorname_clicked;

      dlg.DoModal();
   }
}

void CLampView::OnUpdateFlagUser(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampView::OnLaunchLink_Chrome()
{
   UCString link;
   GetRMBLink(link);
   
   if(!link.IsEmpty())
   {
      const UCChar *work = link;
      if(_wcsnicmp(work,L"forceout:",9) == 0)
      {
         work += 9;
      }

      HINSTANCE hInst = ShellExecuteW(NULL,
                                      L"open",
                                      L"chrome.exe",
                                      work,
                                      NULL,
                                      SW_SHOW);

      if((int)hInst == ERROR_FILE_NOT_FOUND ||
         (int)hInst == ERROR_PATH_NOT_FOUND)
      {
         UCChar buffer[MAX_PATH];
         buffer[0] = 0;
         ::GetEnvironmentVariable(L"USERPROFILE",buffer,MAX_PATH);

         if(buffer[0])
         {
            UCString file = buffer;

            if(file.endswith(L"\\") == NULL)
            {
               file += L"\\";
            }

            file += L"AppData\\Local\\Google\\Chrome\\Application\\chrome.exe";
                     
            ShellExecuteW(NULL,
                          L"open",
                          file,
                          work,
                          NULL,
                          SW_SHOW);
         }
      }
   }
}

void CLampView::OnLaunchLink_Chrome_Incognito()
{
   UCString link;
   GetRMBLink(link);
   
   if(!link.IsEmpty())
   {
      const UCChar *work = link;
      if(_wcsnicmp(work,L"forceout:",9) == 0)
      {
         work += 9;
      }

      UCString cmdline = L"--incognito ";
      cmdline += work;
            
      HINSTANCE hInst = ShellExecuteW(NULL,
                                      L"open",
                                      L"chrome.exe",
                                      cmdline,
                                      NULL,
                                      SW_SHOW);

      if((int)hInst == ERROR_FILE_NOT_FOUND ||
         (int)hInst == ERROR_PATH_NOT_FOUND)
      {
         UCChar buffer[MAX_PATH];
         buffer[0] = 0;
         ::GetEnvironmentVariable(L"USERPROFILE",buffer,MAX_PATH);

         if(buffer[0])
         {
            UCString file = buffer;

            if(file.endswith(L"\\") == NULL)
            {
               file += L"\\";
            }

            file += L"AppData\\Local\\Google\\Chrome\\Application\\chrome.exe";
                     
            ShellExecuteW(NULL,
                          L"open",
                          file,
                          cmdline,
                          NULL,
                          SW_SHOW);
         }
      }
   }
}

void CLampView::OnLaunchLink_Firefox()
{
   UCString link;
   GetRMBLink(link);
   
   if(!link.IsEmpty())
   {
      const UCChar *work = link;
      if(_wcsnicmp(work,L"forceout:",9) == 0)
      {
         work += 9;
      }

      ShellExecuteW(NULL,
                    L"open",
                    L"firefox.exe",
                    work,
                    NULL,
                    SW_SHOW);
   }
}

void CLampView::OnLaunchLink_IE()
{
   UCString link;
   GetRMBLink(link);
   
   if(!link.IsEmpty())
   {
      const UCChar *work = link;
      if(_wcsnicmp(work,L"forceout:",9) == 0)
      {
         work += 9;
      }

      ShellExecuteW(NULL,
                    L"open",
                    L"iexplore.exe",
                    work,
                    NULL,
                    SW_SHOW);
   }
}

void CLampView::OnLaunchLink_IE_Private()
{
   
   UCString link;
   GetRMBLink(link);
   
   if(!link.IsEmpty())
   {
      const UCChar *work = link;
      if(_wcsnicmp(work,L"forceout:",9) == 0)
      {
         work += 9;
      }

      UCString cmdline = L"-private ";
      cmdline += work;

      ShellExecuteW(NULL,
                    L"open",
                    L"iexplore.exe",
                    cmdline,
                    NULL,
                    SW_SHOW);
   }
}

void CLampView::OnLaunchLink_Safari()
{
   UCString link;
   GetRMBLink(link);
   
   if(!link.IsEmpty())
   {
      const UCChar *work = link;
      if(_wcsnicmp(work,L"forceout:",9) == 0)
      {
         work += 9;
      }

      ShellExecuteW(NULL,
                    L"open",
                    L"safari.exe",
                    work,
                    NULL,
                    SW_SHOW);
   }
}



void CLampView::OnSystemDef()
{
   theApp.SetDefaultBrowserType(BT_SYSTEM);
}

void CLampView::OnChromeDef()
{
   theApp.SetDefaultBrowserType(BT_CHROME);
}

void CLampView::OnChromeIncognitoDef()
{
   theApp.SetDefaultBrowserType(BT_CHROME_INCOGNITO);
}

void CLampView::OnFirefoxDef()
{
   theApp.SetDefaultBrowserType(BT_FIREFOX);
}

void CLampView::OnIEDef()
{
   theApp.SetDefaultBrowserType(BT_IE);
}

void CLampView::OnIEPrivateDef()
{
   theApp.SetDefaultBrowserType(BT_IE_PRIVATE);
}

void CLampView::OnSafariDef()
{
   theApp.SetDefaultBrowserType(BT_SAFARI);
}

void CLampView::OnUpdateSystemDef(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
   
   if(theApp.GetDefaultBrowserType() == BT_SYSTEM)
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnUpdateChromeDef(CCmdUI *pCmdUI)
{
   if(theApp.GetDefaultBrowserType() == BT_CHROME)
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnUpdateChromeIncognitoDef(CCmdUI *pCmdUI)
{
   if(theApp.GetDefaultBrowserType() == BT_CHROME_INCOGNITO)
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnUpdateFirefoxDef(CCmdUI *pCmdUI)
{
   if(theApp.GetDefaultBrowserType() == BT_FIREFOX)
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnUpdateIEDef(CCmdUI *pCmdUI)
{
   if(theApp.GetDefaultBrowserType() == BT_IE)
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnUpdateIEPrivateDef(CCmdUI *pCmdUI)
{
   if(theApp.GetDefaultBrowserType() == BT_IE_PRIVATE)
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnUpdateSafariDef(CCmdUI *pCmdUI)
{
   if(theApp.GetDefaultBrowserType() == BT_SAFARI)
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

///////////////////

void CLampView::OnSystemDef_nws()
{
   theApp.SetDefaultBrowserType_NWS(BT_SYSTEM);
}

void CLampView::OnChromeDef_nws()
{
   theApp.SetDefaultBrowserType_NWS(BT_CHROME);
}

void CLampView::OnChromeIncognitoDef_nws()
{
   theApp.SetDefaultBrowserType_NWS(BT_CHROME_INCOGNITO);
}

void CLampView::OnFirefoxDef_nws()
{
   theApp.SetDefaultBrowserType_NWS(BT_FIREFOX);
}

void CLampView::OnIEDef_nws()
{
   theApp.SetDefaultBrowserType_NWS(BT_IE);
}

void CLampView::OnIEPrivateDef_nws()
{
   theApp.SetDefaultBrowserType_NWS(BT_IE_PRIVATE);
}

void CLampView::OnSafariDef_nws()
{
   theApp.SetDefaultBrowserType_NWS(BT_SAFARI);
}

void CLampView::OnUpdateSystemDef_nws(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
   
   if(theApp.GetDefaultBrowserType_NWS() == BT_SYSTEM)
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnUpdateChromeDef_nws(CCmdUI *pCmdUI)
{
   if(theApp.GetDefaultBrowserType_NWS() == BT_CHROME)
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnUpdateChromeIncognitoDef_nws(CCmdUI *pCmdUI)
{
   if(theApp.GetDefaultBrowserType_NWS() == BT_CHROME_INCOGNITO)
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnUpdateFirefoxDef_nws(CCmdUI *pCmdUI)
{
   if(theApp.GetDefaultBrowserType_NWS() == BT_FIREFOX)
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnUpdateIEDef_nws(CCmdUI *pCmdUI)
{
   if(theApp.GetDefaultBrowserType_NWS() == BT_IE)
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnUpdateIEPrivateDef_nws(CCmdUI *pCmdUI)
{
   if(theApp.GetDefaultBrowserType_NWS() == BT_IE_PRIVATE)
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnUpdateSafariDef_nws(CCmdUI *pCmdUI)
{
   if(theApp.GetDefaultBrowserType_NWS() == BT_SAFARI)
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}


void CLampView::OnAllowGDIPlus()
{
   theApp.allow_gdiplus(!theApp.allow_gdiplus());
}

void CLampView::OnUpdateAllowGDIPlus(CCmdUI *pCmdUI)
{
   if(theApp.allow_gdiplus())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampView::OnAutoRefresh()
{
   theApp.AutoRefresh(!theApp.AutoRefresh());
}

void CLampView::OnUpdateAutoRefresh(CCmdUI *pCmdUI)
{
   if(theApp.AutoRefresh())
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

CHotSpot *CLampView::GetHotspot(CPoint &point)
{
   CHotSpot *result = NULL;

   for(size_t i = 0; i < m_hotspots.size(); i++)
   {
      if(point.x >= m_hotspots[i].m_spot.left &&
         point.x < m_hotspots[i].m_spot.right &&
         point.y >= m_hotspots[i].m_spot.top &&
         point.y < m_hotspots[i].m_spot.bottom)
      {
         result = &(m_hotspots[i]);
         break;
      }
   }

   return result;
}

bool CLampView::HaveNextPost()
{
   if(GetCurrentId() != 0)
   {
      ChattyPost *pPost = GetDocument()->FindPost(GetCurrentId());
      if(pPost != NULL)
      {            
         unsigned int id = pPost->GetNextReply(false,false);
         if(id != GetCurrentId())
         {
            return true;
         }
      }
   }

   return false;
}

bool CLampView::HavePrevPost()
{
   if(GetCurrentId() != 0)
   {
      ChattyPost *pPost = GetDocument()->FindPost(GetCurrentId());
      if(pPost != NULL)
      {            
         unsigned int id = pPost->GetPrevReply(false,false);
         if(id != GetCurrentId())
         {
            return true;
         }
      }
   }

   return false;
}

bool CLampView::HaveNextThread()
{
   if(GetDocument()->GetRootCount() > 0)
   {
      ChattyPost *pParent = NULL;

      if(GetCurrentId() != 0)
      {
         ChattyPost *pPost = GetDocument()->FindPost(GetCurrentId());
         if(pPost != NULL)
         {            
            pParent = pPost;
            while(pParent->GetParent() != NULL) pParent = pParent->GetParent();
         }
      }
      // select next root thread
      unsigned int id = GetDocument()->GetNextRoot(pParent);

      if((pParent == NULL ||
         id != pParent->GetId()) &&
         id != 0)
      {
         return true;
      }
   }

   return false;
}

bool CLampView::HavePrevThread()
{
   if(GetDocument()->GetRootCount() > 0 &&
      GetCurrentId() != 0)
   {
      ChattyPost *pParent = NULL;

      ChattyPost *pPost = GetDocument()->FindPost(GetCurrentId());
      if(pPost != NULL)
      {            
         pParent = pPost;
         while(pParent->GetParent() != NULL) pParent = pParent->GetParent();
      }

      // select next root thread
      unsigned int id = GetDocument()->GetPrevRoot(pParent);

      if(pParent != NULL && 
         id != pParent->GetId() &&
         id != 0)
      {
         return true;
      }
   }

   return false;
}

bool CLampView::HaveNextNewPost()
{
   if(GetCurrentId() != 0)
   {
      ChattyPost *pPost = GetDocument()->FindPost(GetCurrentId());
      if(pPost != NULL)
      {            
         unsigned int id = pPost->GetNextNewReply(false,false);
         if(id != GetCurrentId())
         {
            return true;
         }
      }
   }

   return false;
}

bool CLampView::HavePrevNewPost()
{
   if(GetCurrentId() != 0)
   {
      ChattyPost *pPost = GetDocument()->FindPost(GetCurrentId());
      if(pPost != NULL)
      {            
         unsigned int id = pPost->GetPrevNewReply(false,false);
         if(id != GetCurrentId())
         {
            return true;
         }
      }
   }

   return false;
}

bool CLampView::CanDemote()
{
   if(GetCurrentId() != 0)
   {
      ChattyPost *pPost = GetDocument()->FindPost(GetCurrentId());
      if(pPost != NULL)
      {            
         return pPost->CanDemote();
      }
   }

   return false;
}

void CLampView::SoftRefresh()
{
   if(GetDocument() != NULL)
   {
      GetDocument()->Refresh(true);
   }
}
