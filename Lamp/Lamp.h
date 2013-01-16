
// Lamp.h : main header file for the Lamp application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "DCSurface.h"
#include "ChattyPost.h"
#include <list>
#include <map>
#include <vector>
#include <deque>
// CLampApp:
// See Lamp.cpp for the implementation of this class
//

#define WM_EXPAND_TABS (WM_USER + 101)
#define WM_WAKEUP (WM_USER + 102)

#define LAMP_VERSION_MAJOR 3
#define LAMP_VERSION_MINOR 9

#define QUOTE_FONT_SCALE   1.125f

chattyerror download(const char* host, const char* path, char** out_response, int *psize=NULL);

typedef enum 
{
   SBT_INACTIVE,
   SBT_ACTIVE,
   SBT_HOVER
}scrollbitype;

typedef enum 
{
   BT_SYSTEM,
   BT_CHROME,
   BT_CHROME_INCOGNITO,
   BT_FIREFOX,
   BT_IE,
   BT_IE_PRIVATE,
   BT_SAFARI
}browsertype;


typedef enum 
{
   DT_THREAD       = 0,
   DT_STORY        = 1,
   DT_SEARCH       = 2,
   DT_THREAD_START = 3,
   DT_LOL          = 4,
   DT_LOL_SOFT     = 5,
   DT_AUTHOR       = 6,
   DT_POST         = 7,
   DT_STORY_2      = 8,
   DT_SHACKMSG     = 9,
   DT_READMSG      = 10,
   DT_SENDMSG      = 11,
   DT_CHECK_UPDATE = 12,
   DT_REFRESH_LOLS = 13,

   DT_SHACK_CHATTY = 14,
   DT_SHACK_THREAD = 15,
   DT_SHACK_THREAD_SOFT = 16,
   DT_SHACK_THREAD_CONTENTS = 17,
   DT_SHACK_POST   = 18,
   DT_SHACK_SEARCH = 19,
   DT_SHACK_SEARCH_SOFT = 20,
   DT_SHACK_SHACKMSG = 21,
   DT_SHACK_READMSG = 22,
   DT_SHACK_SENDMSG = 23,
   DT_SHACK_DELETEMSG = 24,
   DT_SHACK_CHATTY_INFINATE_PAGE = 25,
   DT_SHACK_MOD_CATEGORY_CHANGE = 26,
   
   DT_SUBMIT_LOLVOTE = 27,
   DT_GET_IMAGE = 28,
   DT_GET_THUMB = 29,
   DT_GET_PROFILE = 30
} DownloadType;

class CLampView;
class DockTab;

class Hunspell;

class CMyLol
{
public:
   unsigned int m_post_id;
   byte m_mylols;
};

class CSessionDoc
{
public:
   CSessionDoc(int pane, const UCString &launch)
   {
      m_pane = pane;
      m_launch = launch;
   }
   int m_pane;
   UCString m_launch;
};

class CSuggestion
{
public:
   UCString display;
   UCString value;
};

class CDownloadHistoryItem
{
public:
   UCString m_host;
   UCString m_path;
   UCString m_errmsg;
   UCString m_post_data;
   DownloadType m_dt;
   DWORD m_start_time;
   DWORD m_recieve_time;
   DWORD m_end_time;
};

class CollapsedThread
{
public:
   CollapsedThread(unsigned int thread_id, time_t date)
   {
      m_thread_id = thread_id;
      m_date = date;
   }

   unsigned int m_thread_id;
   time_t m_date;
};

class CShackBookmark
{
public:

   bool operator == (const CShackBookmark &other) const
   {
      if(m_root_id == other.m_root_id &&
         m_current_id == other.m_current_id &&
         m_category == other.m_category &&
         m_title == other.m_title &&
         m_author == other.m_author)
      {
         return true;
      }
      return false;
   }

   unsigned int m_root_id;
   unsigned int m_current_id;
   postcategorytype m_category;
   UCString m_title;
   UCString m_author;
};

class CFlagImage
{
public:
   CFlagImage()
   {
      m_image_preview = NULL;
      m_image_reply = NULL;
      m_image_root = NULL;
   }

   ~CFlagImage()
   {
      if(m_image_preview != NULL)
         delete m_image_preview;

      if(m_image_reply != NULL)
         delete m_image_reply;

      if(m_image_root != NULL)
         delete m_image_root;
   }

   RECT m_active_rect;
   CDCSurface *m_image_preview;
   CDCSurface *m_image_reply;
   CDCSurface *m_image_root;
   UCString m_image_path;
};

class CFlaggedUser
{
public:

   CFlagImage *m_flag_image;
   UCString m_flag;
   UCString m_note;
};

class CImageCacheItem
{
public:

   CImageCacheItem()
   {
      m_secure = false;
   }

   CImageCacheItem(const CImageCacheItem &other)
   {
      m_host = other.m_host;
      m_path = other.m_path;
      m_ext = other.m_ext;
      m_secure = other.m_secure;
   }

   ~CImageCacheItem()
   {
   }

   CDCSurface *GetImage();
   CDCSurface *GetImageThumb();

   void MakeThumb();

   void AddNotify(unsigned int id);

   UCString m_host;
   UCString m_path;
   UCString m_ext;
   bool m_secure;

   CDCSurface m_image;
   CDCSurface m_imagethumb;

   std::vector<unsigned int> m_notifylist;

   DWORD m_time;
};

class CLampApp : public CWinAppEx
{
public:
	CLampApp();

   virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);

   void OpenActiveDocumentFile(LPCTSTR lpszFileName, bool dofocusstuff);

   void ActivateActiveThread(bool dofocusstuff);

   void OpenShackLink(const UCString &shackpath, bool NWS = false);

   void SetStatusBarText(const UCString &text, CLampView *pView);

   bool HaveLogin();
   bool Login();

   void AddDocToList(CLampDoc *pDoc){m_MyDocuments.push_back(pDoc);}
   void RemoveDocFromList(CLampDoc *pDoc){m_MyDocuments.remove(pDoc);}

   ChattyPost *FindFromLatestChatty(unsigned int id, CLampDoc **pDoc);

   CLampDoc *GetLatestChatty();

   void InvalidateContentLayout(unsigned int id);

   bool HasLinkedImage(const UCString &link, unsigned int &index);

   bool IsImageLoaded(unsigned int index);

   bool IsThumbLoaded(unsigned int index);

   void LoadImage(unsigned int index, unsigned int postid);

   void LoadImageThumb(unsigned int index, unsigned int postid);

   void UnloadAllImages();

   void UpdateTabSizes();

   bool allow_gdiplus(){return m_allow_gdiplus;}
   void allow_gdiplus(bool value){m_allow_gdiplus = value;}

   CDCSurface *GetLinkedImage(const UCString &link, unsigned int &index);
   CDCSurface *GetLinkedImage(unsigned int index);

   CDCSurface *GetLinkedImageThumb(const UCString &link, unsigned int &index);
   CDCSurface *GetLinkedImageThumb(unsigned int index);

   void MakeThumb(unsigned int index);

   UCString &GetUsername(){return m_username;}
   UCString &GetPassword(){return m_password;}

   CDCSurface *GetRefreshBuffer(bool bHover, bool bAnimated, size_t frame)
   {
      if(bAnimated)
      {
         return (m_refresh_anim + frame);
      }
      else
      {
         if(bHover)
         {
            return &m_refresh_hover_buffer;
         }
         else
         {
            return &m_refresh_buffer;
         }
      }
   }

   CDCSurface *GetTabBuffer(bool bHover)
   {
      if(bHover)
      {
         return &m_tab_hover_buffer;
      }
      else
      {
         return &m_tab_buffer;
      }
   }

   CDCSurface *GetReplyBuffer(bool bHover)
   {
      if(bHover)
      {
         return &m_reply_hover_buffer;
      }
      else
      {
         return &m_reply_buffer;
      }
   }

   CDCSurface *GetUpImage(scrollbitype type)
   {
      switch(type)
      {
      case SBT_INACTIVE: return &m_up_inactive;
      case SBT_ACTIVE:   return &m_up_active;
      case SBT_HOVER:    return &m_up_hover;
      }
      return &m_up_inactive;
   }
   
   CDCSurface *GetDownImage(scrollbitype type)
   {
      switch(type)
      {
      case SBT_INACTIVE: return &m_down_inactive;
      case SBT_ACTIVE:   return &m_down_active;
      case SBT_HOVER:    return &m_down_hover;
      }
      return &m_down_inactive;
   }

   CDCSurface *GetThumbImage(scrollbitype type)
   {
      switch(type)
      {
      case SBT_INACTIVE: return &m_thumb_inactive;
      case SBT_ACTIVE:   return &m_thumb_active;
      case SBT_HOVER:    return &m_thumb_hover;
      }
      return &m_thumb_inactive;
   }

   CDCSurface *GetThumbTopImage(scrollbitype type)
   {
      switch(type)
      {
      case SBT_INACTIVE: return &m_thumb_top_inactive;
      case SBT_ACTIVE:   return &m_thumb_top_active;
      case SBT_HOVER:    return &m_thumb_top_hover;
      }
      return &m_thumb_top_inactive;
   }

   CDCSurface *GetThumbBottomImage(scrollbitype type)
   {
      switch(type)
      {
      case SBT_INACTIVE: return &m_thumb_bottom_inactive;
      case SBT_ACTIVE:   return &m_thumb_bottom_active;
      case SBT_HOVER:    return &m_thumb_bottom_hover;
      }
      return &m_thumb_bottom_inactive;
   }

   CDCSurface *GetThumbGripImage(scrollbitype type)
   {
      switch(type)
      {
      case SBT_INACTIVE: return &m_thumb_grip_inactive;
      case SBT_ACTIVE:   return &m_thumb_grip_active;
      case SBT_HOVER:    return &m_thumb_grip_hover;
      }
      return &m_thumb_grip_inactive;
   }
   
   CDCSurface *GetTrackImage()
   {
      return &m_track;
   }

   CDCSurface *GetOnTopicImage(bool hover)
   {
      if(!hover)
      {
         return &m_ontopic;
      }
      else
      {
         return &m_ontopic_hover;
      }
   }

   CDCSurface *GetINFImage(bool hover)
   {
      if(!hover)
      {
         return &m_inf;
      }
      else
      {
         return &m_inf_hover;
      }
   }

   CDCSurface *GetINFStar(){return &m_infstar;}

   CDCSurface *GetNWSImage(bool hover)
   {
      if(!hover)
      {
         return &m_nws;
      }
      else
      {
         return &m_nws_hover;
      }
   }

   CDCSurface *GetNWSStar(){return &m_nwsstar;}

   CDCSurface *GetStupidImage(bool hover)
   {
      if(!hover)
      {
         return &m_stupid;
      }
      else
      {
         return &m_stupid_hover;
      }
   }

   CDCSurface *GetStupidStar(){return &m_stupidstar;}

   CDCSurface *GetOffTopicImage(bool hover)
   {
      if(!hover)
      {
         return &m_offtopic;
      }
      else
      {
         return &m_offtopic_hover;
      }
   }

   CDCSurface *GetOffTopicStar(){return &m_offtopicstar;}

   CDCSurface *GetPoliticalImage(bool hover)
   {
      if(!hover)
      {
         return &m_political;
      }
      else
      {
         return &m_political_hover;
      }
   }

   CDCSurface *GetPoliticalStar(){return &m_politicalstar;}

   CDCSurface *GetNukedImage(bool hover)
   {
      if(!hover)
      {
         return &m_nuked;
      }
      else
      {
         return &m_nuked_hover;
      }
   }

   CDCSurface *GetCloseImage(bool bHover)
   {
      if(bHover)
      {
         return &m_close_hover;
      }
      else
      {
         return &m_close;
      }
   }

   CDCSurface *GetPostImage(bool bHover)
   {
      if(bHover)
      {
         return &m_post_hover;
      }
      else
      {
         return &m_post;
      }
   }

   CDCSurface *GetSendImage(bool bHover)
   {
      if(bHover)
      {
         return &m_send_hover;
      }
      else
      {
         return &m_send;
      }
   }

   CDCSurface *GetNewThreadImage(bool bHover)
   {
      if(bHover)
      {
         return &m_newthread_hover;
      }
      else
      {
         return &m_newthread;
      }
   }

   CDCSurface *GetPreviewImage(bool bHover)
   {
      if(bHover)
      {
         return &m_preview_hover;
      }
      else
      {
         return &m_preview;
      }
   }   

   CDCSurface *GetNavImage(bool bThread, bool bNew, bool bNext, bool bHover, bool bInactive)
   {
      if(bThread)
      {
         if(bNext)
         {
            if(bInactive)
            {
               return &m_thread_next_inactive;
            }
            else if(bHover)
            {
               return &m_thread_next_hover;
            }
            else
            {
               return &m_thread_next_active;
            }
         }
         else
         {
            if(bInactive)
            {
               return &m_thread_prev_inactive;
            }
            else if(bHover)
            {
               return &m_thread_prev_hover;
            }
            else
            {
               return &m_thread_prev_active;
            }
         }
      }
      else if(bNew)
      {
         if(bNext)
         {
            if(bInactive)
            {
               return &m_post_new_next_inactive;
            }
            else if(bHover)
            {
               return &m_post_new_next_hover;
            }
            else
            {
               return &m_post_new_next_active;
            }
         }
         else
         {
            if(bInactive)
            {
               return &m_post_new_prev_inactive;
            }
            else if(bHover)
            {
               return &m_post_new_prev_hover;
            }
            else
            {
               return &m_post_new_prev_active;
            }
         }
      }
      else
      {
         if(bNext)
         {
            if(bInactive)
            {
               return &m_post_next_inactive;
            }
            else if(bHover)
            {
               return &m_post_next_hover;
            }
            else
            {
               return &m_post_next_active;
            }
         }
         else
         {
            if(bInactive)
            {
               return &m_post_prev_inactive;
            }
            else if(bHover)
            {
               return &m_post_prev_hover;
            }
            else
            {
               return &m_post_prev_active;
            }
         }
      }
   }   

   CDCSurface *GetDemoteImage(bool enabled, bool bHover)
   {
      if(bHover)
      {
         return &m_demote_hover;
      }
      else
      {
         if(enabled)
         {
            return &m_demote_active;
         }
         else
         {
            return &m_demote_inactive;
         }
      }
   }

   CDCSurface *GetPinImage(bool on, bool bHover)
   {
      if(bHover)
      {
         return &m_pin_hover;
      }
      else
      {
         if(on)
         {
            return &m_pin_on;
         }
         else
         {
            return &m_pin_off;
         }
      }
   }

   CDCSurface *GetRefreshStoryImage(bool bHover)
   {
      if(bHover)
      {
         return &m_refresh_story_hover;
      }
      else
      {
         return &m_refresh_story;
      }
   }

   CDCSurface *GetComposeImage(bool bHover)
   {
      if(bHover)
      {
         return &m_compose_hover;
      }
      else
      {
         return &m_compose;
      }
   }

   CDCSurface *GetSearchImage(bool bHover)
   {
      if(bHover)
      {
         return &m_search_hover;
      }
      else
      {
         return &m_search;
      }
   }

   CDCSurface *GetForwardImage(bool bHover)
   {
      if(bHover)
      {
         return &m_forward_hover;
      }
      else
      {
         return &m_forward;
      }
   }

   CDCSurface *GetDeleteImage(bool bHover)
   {
      if(bHover)
      {
         return &m_delete_hover;
      }
      else
      {
         return &m_delete;
      }
   }
   
   CDCSurface *GetDockTabImage(){return &m_docktabimage;}

   CDCSurface *GetBackground(){return &m_background;}
   CDCSurface *GetPostBackground(){return &m_post_background;}
   CDCSurface *GetRootPostBackground(){return &m_rootpost_background;}
   CDCSurface *GetReplyBackground(){return &m_reply_background;}

   CDCSurface *GetRMPanIcon(){return &m_mb_pan;}
      
   int GetFontHeight(){return m_fontheight;}
   int GetMiscFontHeight(){return m_miscfontheight;}
   int GetSampleFontHeight(){return m_samplefontheight;}
   int GetTextHeight(){return m_textheight;}
   int GetCellHeight(){return m_cellheight;}

   UCString &GetUserHostName(){return m_userhostname;}
   UCString &GetLolHostName(){return m_lolhostname;}
   UCString &GetProfileHost(){return m_profilehostname;}

   bool BackgroundIsDark(){return m_bBackgroundIsDark;}
   
   COLORREF GetBackgroundColor(){return m_background_color;}
   COLORREF GetRootPostBackgroundColor(){return m_rootpost_background_color;}
   COLORREF GetEditBackgroundColor(){return m_edit_background_color;}
   COLORREF GetPostBackgroundColor(){return m_post_background_color;}
   COLORREF GetPostEdgeColor(){return m_post_edge_color;}
   COLORREF GetPostINFEdgeColor(){return m_post_INF_edge_color;}
   COLORREF GetPostNWSEdgeColor(){return m_post_NWS_edge_color;}
   COLORREF GetPostStupidEdgeColor(){return m_post_stupid_edge_color;}
   COLORREF GetPostOffTopicEdgeColor(){return m_post_offtopic_edge_color;}
   COLORREF GetPostPoliticalEdgeColor(){return m_post_political_edge_color;}
   COLORREF GetBranchColor(){return m_branch_color;}
   COLORREF GetMiscPostTextColor(){return m_misc_post_text_color;}
   COLORREF GetPostTextColor(){return m_post_text_color;}
   COLORREF GetFadedPostTextColor(){return m_faded_post_text_color;}
   COLORREF GetPostTextColorShade(int shade)
   {
      shade = __min(10,shade);
      shade = __max(0,shade);
      return m_post_text_color_shade[shade];
   }
   COLORREF GetBranchColorShade(int shade, newness Newness)
   {
      switch(Newness)
      {
      case N_NEW:
         shade = __min(10,shade);
         shade = __max(0,shade);
         return m_branchisnew_color_shade[shade];
         break;
      case N_LAST:
         shade = __min(10,shade);
         shade = __max(0,shade);
         return m_branchislast_color_shade[shade];
         break;
      case N_OLD:
      default:
         shade = __min(10,shade);
         shade = __max(0,shade);
         return m_branch_color_shade[shade];
         break;
      }
   }
   COLORREF GetBranchIsNew(){return m_branchisnew_color;}
   COLORREF GetBranchIsLast(){return m_branchislast_color;}
   COLORREF GetSpoilerColor(){return m_spoiler_color;}
   COLORREF GetTextSelectionColor(){return m_text_selection_color;}
   void SetTextSelectionColor(COLORREF color){m_text_selection_color = color;}
   COLORREF GetMyPostColor(){return m_mypost_color;}
   COLORREF GetHoverColor(){return m_hover_color;}


   COLORREF GetRed(){return m_RED;}
   COLORREF GetGreen(){return m_GREEN;}
   COLORREF GetDGreen(){return m_DGREEN;}
   COLORREF GetBlue(){return m_BLUE;}
   COLORREF GetYellow(){return m_YELLOW;}
   COLORREF GetOlive(){return m_OLIVE;}
   COLORREF GetLime(){return m_LIME;}
   COLORREF GetOrange(){return m_ORANGE;}
   COLORREF GetPurple(){return m_PURPLE;}
   COLORREF GetPink(){return m_PINK;}

   COLORREF GetExpiredTextColor(){return m_expired_color;}
   COLORREF GetExpiringTextColor(){return m_expiring_color;}

   int GetNumToShowWhenTruncated(){return m_numshow_truncated;}
   void SetNumToShowWhenTruncated(int value){m_numshow_truncated = value;}

   int GetLineThickness(){return m_line_thickness;}
   void SetLineThickness(int value){m_line_thickness = value;InvalidateSkinAllViews();}

   COLORREF GetUserColor(UCString &name);

   const UCChar *GetNormalFontName(){return m_normal_fontname;}
   const UCChar *GetQuotedFontName(){return m_quoted_fontname;}
   const UCChar *GetCodeFontName(){return m_code_fontname;}

   void SetNormalFontName(const UCChar *value){m_normal_fontname = value;InvalidateSkinAllViews();}
   void SetQuotedFontName(const UCChar *value){m_quoted_fontname = value;InvalidateSkinAllViews();}
   void SetCodeFontName(const UCChar *value){m_code_fontname = value;InvalidateSkinAllViews();}

   bool RoundedPosts(){return m_rounded_posts;}
   
   bool UseAuthorColorForPreview(){return m_bUseAuthorColorForPreview;}
   void UseAuthorColorForPreview(bool value){m_bUseAuthorColorForPreview = value;}

   bool LeftMousePan(){return m_left_mouse_pan;}
   void LeftMousePan(bool value){m_left_mouse_pan = value;}

   bool MoveRefreshToTop(){return m_move_refresh_to_top;}
   void MoveRefreshToTop(bool value){m_move_refresh_to_top = value;}

   bool AutoRefresh(){return m_auto_refresh;}
   void AutoRefresh(bool value);

   bool ShowNavButtons(){return m_show_nav_buttons;}
   void ShowNavButtons(bool value){m_show_nav_buttons = value;}

   bool LatestChattySummaryMode(){return m_latestchatty_summary_mode;}
   void LatestChattySummaryMode(bool value);

   bool TightFitSummary(){return m_tight_fit_summary;}
   void TightFitSummary(bool value){m_tight_fit_summary = value;}

   int MaxSummaryLines(){return m_max_summary_lines;}
   void MaxSummaryLines(int value);

   bool StrokeRootEdges(){return m_stroke_root_edges;}
   bool StrokePreviewEdges(){return m_stroke_preview_edges;}
   void SetStrokePreviewEdges(bool value){m_stroke_preview_edges = value;}

   bool SingleThreadStyle();
   void SetSingleThreadStyle(bool value);

   int GetTabTitleWordLimit(){return m_tab_title_word_limit;}
   int GetTabTitleCharLimit(){return m_tab_title_char_limit;}

   bool GetSmoothScroll(){return m_smooth_scroll;}
   float GetSmoothScrollScale(){return m_smoothscrollscale;}
   float GetMouseWheelScale(){return m_mouse_wheel_scale;}
   int GetNumMinutesUpdateTab(){return m_num_minutes_update_tab;}
   void SetNumMinutesUpdateTab(int value);
   
   int GetNumMinutesCheckLOL(){return m_num_minutes_check_lol;}
   void SetNumMinutesCheckLOL(int value){m_num_minutes_check_lol = value;}

   void SetMouseWheelScale(float value){m_mouse_wheel_scale = value;}

   bool IsSpellCheckerEnabled(){return m_enable_spell_checker;}
   void SetIsSpellCheckerEnabled(bool value){m_enable_spell_checker = value;}

   bool GetHighlightOP(){return m_highlight_OP;}
   void SetHighlightOP(bool value){m_highlight_OP = value;}

   bool GetAuthorGlow(){return m_authorglow;}
   void SetAuthorGlow(bool value){m_authorglow = value;}

   void AddView(CLampView *pView){m_views.push_back(pView);}
   void RemoveView(CLampView *pView){m_views.remove(pView);}

   void InvalidateSkinAllViews();

   void InvalidateFlagsAllViews();

   float GetTextScaler(){return m_textscaler;}
   void SetTextScaler(float value)
   {
      m_textscaler = value;
      m_textscaler = __min(5.0f,m_textscaler);
      m_textscaler = __max(0.5f,m_textscaler);
      InvalidateSkinAllViews();
   }

   float GetMBPanScale(){return m_mb_pan_scale;}
   void SetMBPanScale(float value){m_mb_pan_scale = value;}

   float GetInertiaFriction(){return m_inertia_friction;}
   void SetInertiaFriction(float value)
   {
      m_inertia_friction = value;
      if(m_inertia_friction < 0.000000001f)
         m_inertia_friction = 0.000000001f;
      if(m_inertia_friction > 1.0f)
         m_inertia_friction = 1.0f;
   }

   bool IsSpelledCorrectly(const UCChar *wordtotest, size_t count);

   void GetSpellSuggestions(const UCChar *wordtotest, size_t count, std::vector<CSuggestion> &suggestions);

   void AddMyLol(unsigned int post_id, loltagtype);
   void RemoveMyLol(unsigned int post_id, loltagtype);
   byte GetMyLol(unsigned int post_id);

   void AddMyCollapse(unsigned int post_id);
   void RemoveMyCollapse(unsigned int post_id);
   bool GetMyCollapse(unsigned int post_id);

   void SetLastSearchParms(const UCString &author, const UCString &parent_author, const UCString &terms, const UCString &search_filter, const UCString &search_sort)
   {
      m_last_search_author = author;
      m_last_search_parent_author = parent_author;
      m_last_search_terms = terms;
      m_last_search_filter = search_filter;
      m_last_search_sort = search_sort;
   }

   bool ShowImageThumbs(){return m_bShowImageThumbs;}

   bool AutoLoadChattypicsThumbs(){return m_bAutoLoadChattypicsThumbs;}
   bool DontAutoLoadNWSThumbs(){return m_bDontAutoLoadNWSThumbs;}

   bool EnableNWS(){return m_enable_nws;}
   bool EnableINF(){return m_enable_inf;}
   bool EnableOffTopic(){return m_enable_offtopic;}
   bool EnableStupid(){return m_enable_stupid;}
   bool EnablePolitical(){return m_enable_political;}

   void SetEnableNWS(bool value){m_enable_nws = value;}
   void SetEnableINF(bool value){m_enable_inf = value;}
   void SetEnableOffTopic(bool value){m_enable_offtopic = value;}
   void SetEnableStupid(bool value){m_enable_stupid = value;}
   void SetEnablePolitical(bool value){m_enable_political = value;}

   UCString &GetSkinFolder(){return m_skinname;}
   void SetSkinFolder(const UCChar *skinname);

   void AddBookmark(const CShackBookmark &bookmark)
   {
      m_Bookmarks.push_back(bookmark);
      WriteBookmarks();
   }

   void RemoveBookmark(const CShackBookmark &bookmark)
   {
      m_Bookmarks.remove(bookmark);
   }

   void PopulateBookmarkMenu(CMenu *pMenu);

   void LaunchBookmarkMenu(size_t index);

   CShackBookmark *GetBookmark(size_t index);

   void AddCachedThreadReplies(unsigned int id,CXMLElement *pReplies);

   CXMLElement *GetCachedThreadReplies(unsigned int id);

   bool IsPinningInStories(){return m_bPinningInStories;}

   void SetPinningInStories(bool value){m_bPinningInStories = value;}

   bool IsDoublePageStory(){return m_bDoublePageStory;}

   void SetDoublePageStory(bool value){m_bDoublePageStory = value;}

   bool ShowLOLButtons(){return m_bShowLOLButtons;}

   void SetShowLOLButtons(bool value){m_bShowLOLButtons = value;}

   bool DoUGH(){return m_bDoUGH;}
   void SetDoUGH(bool value){m_bDoUGH = value;}

   int  GetUGHThreshold(){return m_UGHThreshold;}
   void SetUGHThreshold(int value){m_UGHThreshold = value;}

   bool HideCollapsedPosts(){return m_bHideCollapsedPosts;}
   void SetHideCollapsedPosts(bool value){m_bHideCollapsedPosts = value;}

   bool InfinatePaging(){return m_bInfinatePaging;}
   void SetInfinatePaging(bool value){m_bInfinatePaging = value;}

   bool GotoNewPost(){return m_bGotoNewPost;}
   void SetGotoNewPost(bool value){m_bGotoNewPost = value;}

   bool AlwaysOnTopWhenNotDocked(){return m_bAlwaysOnTopWhenNotDocked;}

   void SetAlwaysOnTopWhenNotDocked(bool value){m_bAlwaysOnTopWhenNotDocked = value;}

   void ClearAllPinnedThreads();

   UCString &GetLastPost(){return m_lastpost;}
   void SetLastPost(const UCString &post){m_lastpost = post;}

   void UpdateInbox();
   void RefreshATab();
   void UpdateTabNames();
   DWORD GetRecentUserActivity();

   void CheckForUpdates(bool bManual);

   void RefreshLOLs();

   UCString &GetFindText(){return m_findtext;}
   void SetFindText(UCString &txt){m_findtext = txt;}

   void SetLOL_LOL(unsigned int post_id, unsigned int count);
   void SetLOL_INF(unsigned int post_id, unsigned int count);
   void SetLOL_UNF(unsigned int post_id, unsigned int count);
   void SetLOL_TAG(unsigned int post_id, unsigned int count);
   void SetLOL_WTF(unsigned int post_id, unsigned int count);
   void SetLOL_UGH(unsigned int post_id, unsigned int count);

   CLOLFlags &GetKnownLOLFlags(unsigned int post_id);

   int GetLOLFieldWidth(){return m_LOLFieldWidth;}

   int GetHintFieldWidth(){return m_HintFieldWidth;}

   bool ShowSmallLOL(){return m_bShowSmallLOL;}
   void SetShowSmallLOL(bool value);

   bool FlaredBranches(){return m_bFlaredBranches;}
   void SetFlaredBranches(bool value){m_bFlaredBranches = value;}

   bool AlternatePostKeys(){return m_bAlternatePostKeys;}
   void SetAlternatePostKeys(bool value){m_bAlternatePostKeys = value;}

   bool LoadImagesInLamp(){return m_bLoadImagesInLamp;}
   void SetLoadImagesInLamp(bool value){m_bLoadImagesInLamp = value;}

   bool GetAutoCheckInbox(){return m_auto_check_inbox;}
   void SetAutoCheckInbox(bool value){m_auto_check_inbox = value;}

   bool ShowRootSelected(){return m_show_root_selected;}
   void SetShowRootSelected(bool value){m_show_root_selected = value;}

   bool ExpandPreviewsDown(){return m_expand_previews_down;}
   void SetExpandPreviewsDown(bool value){m_expand_previews_down = value;}

   bool ExpandPreviews(){return m_expand_previews;}
   void SetExpandPreviews(bool value){m_expand_previews = value;}

   bool ShowThomWLOLS(){return m_show_thomw_lols;}
   void SetShowThomWLOLS(bool value);

   bool VerboseLOLPreviews(){return m_verbose_lol_previews;}
   void SetVerboseLOLPreviews(bool value);

   bool InvertedLOLPreviews(){return m_inverted_lol_previews;}
   void SetInvertedLOLPreviews(bool value);

   bool ShowRawDate(){return m_show_raw_date;}
   void SetShowRawDate(bool value){m_show_raw_date = value;}
   
   void ClearSession(){m_session.clear();m_session_panesizes.clear();}

   void AddToSession(int pane, UCString &launch){m_session.push_back(CSessionDoc(pane,launch));}
   void AddPaneSizeToSession(int panesize){m_session_panesizes.push_back(panesize);}

   std::vector<CSuggestion> GetCheatSheet(){return m_cheatsheet;}
   size_t GetCheatSheetSize(){return m_cheatsheet.size();}


   bool HaveNewMessages(){return (m_unreadmessagecount > 0)?true:false;}

   void GetNewMessagesText(const UCChar **pChar, int **widths, size_t &numchars, int &textwidth)
   {
      *pChar = m_pNewMessages;
      *widths = m_new_messages_widths;
      numchars = m_new_messages_charcount;
      textwidth = m_new_messages_textwidth;
   }

   void SetNewMessageCount(size_t value){m_unreadmessagecount = value; UpdateNewMessages();}

   void UpdateNewMessages();

   void ShowNewMessages();

   int GetHoursExpire(){return m_hours_expire;}
   void SetHoursExpire(int value){m_hours_expire = value;}

   int GetMSecondsPreviewTimer(){return m_mseconds_preview_timer;}
   void SetMSecondsPreviewTimer(int value){m_mseconds_preview_timer = __max(1,value);}

   float GetHoverPreviewPercentStepsize(){return m_hover_preview_percent_stepsize;}
   void SetHoverPreviewPercentStepsize(float value){m_hover_preview_percent_stepsize = __max(0.001f,value);}
   
   DockTab *GetDockTab(){return m_pDockTab;}

   int GetDescent(){return m_descent;}

   bool UseShack(){return m_use_shack;}

   void SetUseShack(bool value){m_use_shack = value;}

   void SetDefaultBrowserType(browsertype browser){m_defaultbrowser = browser;}

   browsertype GetDefaultBrowserType(){return m_defaultbrowser;}

   void SetDefaultBrowserType_NWS(browsertype browser){m_defaultbrowser_nws = browser;}

   browsertype GetDefaultBrowserType_NWS(){return m_defaultbrowser_nws;}

   bool IsPostKnown(unsigned int id);

   void KnowPost(unsigned int id, ChattyPost *post);

   ChattyPost *GetKnownPost(unsigned int id);

   void ClearKnownPosts();

   unsigned int GetUserID();

   unsigned int PeekUserID(){return m_userid;}

   void SetUserID(unsigned int value){m_userid = value;}

   const UCChar *GetOpenDocString(CDocument *pWho)
   {
      if(!m_open_doc.IsEmpty())
      {
         m_pDocWho = pWho; 
         return m_open_doc.Str();
      }
      return NULL;
   }

   std::set<UCString> &GetFilteredUsernameList(){return m_filterusernamelist;}

   std::set<UCString> &GetFilteredPhraseList(){return m_filterphraselist;}

   CDCSurface *GetTempImage(){return &m_tempimage;}

   bool IsValidImageIndex(unsigned int index)
   {
      std::map<unsigned int,CImageCacheItem>::iterator it = m_imagecache.find(index);
      if(it != m_imagecache.end())
      {
         return true;
      }
      return false;
   }

   void CheckForExpiredImages(void);

   CDCSurface *GetLightningBoltImage(bool bHover)
   {
      if(bHover)
      {
         return &m_lightningbolt_hover;
      }
      else
      {
         return &m_lightningbolt;
      }
   }

   bool IsModMode(){return m_modmode;}

   void SetModMode(bool value){m_modmode = value;}

   void AddDownloadHistoryItem(CDownloadHistoryItem &item)
   {
      m_downloadhistory.push_back(item);
      while(m_downloadhistory.size() > 200)
      {
         m_downloadhistory.pop_front();
      }
   }

   void DisplayDownload();

   void LaunchLinkInDefaultBrowser(const UCChar *link, bool NWS = false);

   CFlaggedUser *GetFlaggedUser(const UCString &username);
   void AddFlaggedUser(const UCString &username, const UCString &flag, const UCString &note);
   void UpdateFlaggedUser(const UCString &username, const UCString &flag, const UCString &note);
   void DeleteFlaggedUser(const UCString &username);

   std::map<UCString,CFlagImage> &GetFlagImages(){return m_flagimages;}

   bool BigSkin(){return m_bigskin;}

   int PaneCount();
   int WhichPaneHasDoc(CLampDoc *pDoc);
   int WhichPaneHasLatestChatty();
   int GetActivePane();
   void ChangeActivePane(int to);
   void SetPaneSize(int pane, int panesize);
   int GetPaneSize(int pane);
   void RecordLatestChattyPaneSize();

   HMENU GetContextMenu(UINT uiMenuResId);

   void SetActiveTabDoc(CLampDoc *pDoc, bool setinitialpost = false, bool forcefocus = false);
   void SetLatestChattyActive();
   CLampDoc *GetInboxDoc();
   CLampDoc *GetActiveThread();

      
// Overrides
public:
	virtual BOOL InitInstance();
   virtual int ExitInstance();
   virtual BOOL PreTranslateMessage(MSG* pMsg);

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;
   bool m_bDockedMode;
   bool m_bWasInDockedMode;
   bool m_bCollapsed;
   RECT m_restoredrect;
   RECT m_dockedrect;
   DockTab *m_pDockTab;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
   afx_msg void OnFileOpenthread();
   afx_msg void OnUpdateFileOpenthread(CCmdUI *pCmdUI);
   afx_msg void OnFileOpen();
   afx_msg void OnFileNew();

	DECLARE_MESSAGE_MAP()

protected:
   void ReadSettingsFile();
   void WriteSettingsFile();

   void ReadSkinFiles();

   void ReadBookmarks();
   void WriteBookmarks();

   bool PreventMultipleInstances();

   void CalcDescent();

   void GenerateLightningBolt();

   void GenerateFlagActives();

   void CheckForModMode();

   void CollectFlagImages();

   HANDLE m_hMutex;
   
   CDCSurface m_refresh_buffer;
   CDCSurface m_refresh_hover_buffer;
   CDCSurface m_tab_buffer;
   CDCSurface m_tab_hover_buffer;
   CDCSurface m_reply_buffer;
   CDCSurface m_reply_hover_buffer;

   CDCSurface m_up_inactive;
   CDCSurface m_up_active;
   CDCSurface m_up_hover;
   CDCSurface m_down_inactive;
   CDCSurface m_down_active;
   CDCSurface m_down_hover;
   CDCSurface m_thumb_inactive;
   CDCSurface m_thumb_active;
   CDCSurface m_thumb_hover;
   CDCSurface m_thumb_top_inactive;
   CDCSurface m_thumb_top_active;
   CDCSurface m_thumb_top_hover;
   CDCSurface m_thumb_bottom_inactive;
   CDCSurface m_thumb_bottom_active;
   CDCSurface m_thumb_bottom_hover;
   CDCSurface m_thumb_grip_inactive;
   CDCSurface m_thumb_grip_active;
   CDCSurface m_thumb_grip_hover;
   CDCSurface m_track;
   CDCSurface m_ontopic;
   CDCSurface m_ontopic_hover;
   CDCSurface m_inf_hover;
   CDCSurface m_inf;
   CDCSurface m_infstar;
   CDCSurface m_nws_hover;
   CDCSurface m_nws;
   CDCSurface m_nwsstar;
   CDCSurface m_stupid;
   CDCSurface m_stupid_hover;
   CDCSurface m_stupidstar;
   CDCSurface m_offtopic;
   CDCSurface m_offtopic_hover;
   CDCSurface m_offtopicstar;
   CDCSurface m_political;
   CDCSurface m_political_hover;
   CDCSurface m_politicalstar;
   CDCSurface m_nuked;
   CDCSurface m_nuked_hover;
   CDCSurface m_close;
   CDCSurface m_close_hover;
   CDCSurface m_post;
   CDCSurface m_post_hover;
   CDCSurface m_send;
   CDCSurface m_send_hover;
   CDCSurface m_newthread;
   CDCSurface m_newthread_hover;
   CDCSurface m_preview;
   CDCSurface m_preview_hover;
   CDCSurface m_docktabimage;

   CDCSurface m_thread_next_inactive;
   CDCSurface m_thread_next_active;
   CDCSurface m_thread_next_hover;
   CDCSurface m_thread_prev_inactive;
   CDCSurface m_thread_prev_active;
   CDCSurface m_thread_prev_hover;
   CDCSurface m_post_next_inactive;
   CDCSurface m_post_next_active;
   CDCSurface m_post_next_hover;
   CDCSurface m_post_prev_inactive;
   CDCSurface m_post_prev_active;
   CDCSurface m_post_prev_hover;

   CDCSurface m_demote_inactive;
   CDCSurface m_demote_active;
   CDCSurface m_demote_hover;

   CDCSurface m_post_new_next_inactive;
   CDCSurface m_post_new_next_active;
   CDCSurface m_post_new_next_hover;
   CDCSurface m_post_new_prev_inactive;
   CDCSurface m_post_new_prev_active;
   CDCSurface m_post_new_prev_hover;

   CDCSurface m_pin_hover;
   CDCSurface m_pin_on;
   CDCSurface m_pin_off;

   CDCSurface m_refresh_story;
   CDCSurface m_refresh_story_hover;

   CDCSurface m_background;
   CDCSurface m_post_background;
   CDCSurface m_rootpost_background;
   CDCSurface m_reply_background;

   CDCSurface m_compose;
   CDCSurface m_compose_hover;
   CDCSurface m_search;
   CDCSurface m_search_hover;
   CDCSurface m_forward;
   CDCSurface m_forward_hover;
   CDCSurface m_delete;
   CDCSurface m_delete_hover;

   CDCSurface m_mb_pan;

   CDCSurface m_lightningbolt;
   CDCSurface m_lightningbolt_hover;

   CDCSurface m_refresh_anim[8];

   UCString m_userhostname;
   UCString m_lolhostname;
   UCString m_profilehostname;

   UCString m_username;
   UCString m_password;

   bool m_rounded_posts;

   bool m_stroke_root_edges;
   bool m_stroke_preview_edges;

   UCString m_lang;

   int m_tab_title_word_limit;
   int m_tab_title_char_limit;

   bool m_bBackgroundIsDark;

   COLORREF m_background_color;
   COLORREF m_rootpost_background_color;
   COLORREF m_edit_background_color;
   COLORREF m_post_background_color;
   COLORREF m_post_edge_color;
   COLORREF m_post_INF_edge_color;
   COLORREF m_post_NWS_edge_color;
   COLORREF m_post_stupid_edge_color;
   COLORREF m_post_offtopic_edge_color;
   COLORREF m_post_political_edge_color;

   COLORREF m_branch_color;
   COLORREF m_branchisnew_color;
   COLORREF m_branchislast_color;
   COLORREF m_author_color;
   COLORREF m_misc_post_text_color;
   COLORREF m_post_text_color;
   COLORREF m_faded_post_text_color;
   COLORREF m_spoiler_color;
   COLORREF m_text_selection_color;
   COLORREF m_faded_branch_color;
   COLORREF m_faded_branchisnew_color;
   COLORREF m_faded_branchislast_color;
   COLORREF m_mypost_color;
   COLORREF m_hover_color;
   
   COLORREF m_post_text_color_shade[11];
   COLORREF m_branch_color_shade[11];
   COLORREF m_branchisnew_color_shade[11];
   COLORREF m_branchislast_color_shade[11];

   COLORREF m_RED;
   COLORREF m_GREEN;
   COLORREF m_DGREEN;
   COLORREF m_BLUE;
   COLORREF m_YELLOW;
   COLORREF m_OLIVE;
   COLORREF m_LIME;
   COLORREF m_ORANGE;
   COLORREF m_PURPLE;
   COLORREF m_PINK;

   COLORREF m_expired_color;
   COLORREF m_expiring_color;

   bool m_enable_nws;
   bool m_enable_inf;
   bool m_enable_offtopic;
   bool m_enable_stupid;
   bool m_enable_political;

   bool m_bShowImageThumbs;
   bool m_bAutoLoadChattypicsThumbs;
   bool m_bDontAutoLoadNWSThumbs;

   bool m_smooth_scroll;
   float m_smoothscrollscale;
   float m_mouse_wheel_scale;
   UCString m_skinname;

   int m_fontheight;
   int m_miscfontheight;
   int m_samplefontheight;
   int m_textheight;
   int m_cellheight;
   int m_descent;

   float m_textscaler;

   int m_numshow_truncated;

   int m_line_thickness;

   bool m_bPinningInStories;
   bool m_bDoublePageStory;
   bool m_bStartInDockedMode;
   bool m_bShowLOLButtons;
   bool m_bDoUGH;
   bool m_bAlwaysOnTopWhenNotDocked;
   bool m_bHideCollapsedPosts;
   bool m_bInfinatePaging;
   bool m_bGotoNewPost;

   int m_UGHThreshold;

   std::map<UCString,CFlagImage> m_flagimages;
   std::map<UCString,CFlaggedUser> m_flagged_users;

   std::map<UCString,COLORREF> m_namelist;

   std::set<UCString> m_filterusernamelist;
   std::set<UCString> m_filterphraselist;
   std::set<UCString> m_mod_list;

   std::list<CLampView*> m_views;

   Hunspell *m_pHunspell;

   std::list<CMyLol> m_mylols;
   std::list<CollapsedThread> m_mycollapses;

   UCString m_last_search_author;
   UCString m_last_search_parent_author;
   UCString m_last_search_terms;
   UCString m_last_search_filter;
   UCString m_last_search_sort;

   std::map<unsigned int,CImageCacheItem> m_imagecache;
   unsigned int m_nextimagecacheindex;

   std::list<CLampDoc*> m_MyDocuments;

   std::list<CShackBookmark> m_Bookmarks;

   std::map<unsigned int,CXMLElement *> m_cachedthreadreplies;
   int m_numcachedthreadreplyinserts;

   std::map<unsigned int,CLOLFlags> m_cachedLOLposts;

   std::map<unsigned int,ChattyPost*> m_KnownPosts;

   UCString m_userpath;
   UCString m_appdatapath;

   CXMLElement m_Mods;
   CXMLElement m_ShackEmployees;
   CXMLElement m_GameDevs;

   UCString m_normal_fontname;
   UCString m_quoted_fontname;
   UCString m_code_fontname;

   UCString m_lastpost;

   int m_num_minutes_update_tab;

   int m_num_minutes_check_lol;

   float m_mb_pan_scale;

   bool m_enable_spell_checker;

   bool m_highlight_OP;

   bool m_authorglow;

   UCString m_findtext;

   int m_LOLFieldWidth;

   int m_HintFieldWidth;

   bool m_bShowSmallLOL;

   bool m_bFlaredBranches;

   bool m_bAlternatePostKeys;

   bool m_bLoadImagesInLamp;

   float m_inertia_friction;

   bool m_modmode;

   std::vector<CSuggestion> m_cheatsheet;

   std::vector<CSessionDoc> m_session;
   std::vector<int> m_session_panesizes;

   size_t m_unreadmessagecount;

   UCChar m_new_messages_text[17];
   int m_new_messages_widths[16];
   const UCChar *m_pNewMessages;
   size_t m_new_messages_charcount;
   int m_new_messages_textwidth;

   bool m_auto_check_inbox;

   int m_hours_expire;

   int m_mseconds_preview_timer;

   bool m_show_root_selected;

   bool m_expand_previews;

   bool m_expand_previews_down;

   bool m_show_thomw_lols;

   bool m_verbose_lol_previews;

   bool m_inverted_lol_previews;

   bool m_show_raw_date;

   bool m_use_shack;

   bool m_allow_gdiplus;

   browsertype m_defaultbrowser;

   browsertype m_defaultbrowser_nws;

   unsigned int m_userid;

   UCString m_open_doc;

   CDocument *m_pDocWho;

   CDCSurface m_tempimage;

   float m_hover_preview_percent_stepsize;

   std::list<CDownloadHistoryItem> m_downloadhistory;

   bool m_bUseAuthorColorForPreview;   

   bool m_bigskin;

   bool m_left_mouse_pan;

   bool m_move_refresh_to_top;

   bool m_auto_refresh;

   bool m_show_nav_buttons;

   bool m_latestchatty_summary_mode;

   bool m_tight_fit_summary;

   int m_max_summary_lines;

   bool m_initing;

   int m_lcpanesize;

   

public:
   afx_msg void OnFileSetuplogininfo();
   afx_msg void OnUpdateFileSetuplogininfo(CCmdUI *pCmdUI);
   afx_msg void OnViewSmoothscroll();
   afx_msg void OnUpdateViewSmoothscroll(CCmdUI *pCmdUI);
   afx_msg void OnViewDocktop();
   afx_msg void OnUpdateViewDocktop(CCmdUI *pCmdUI);
   afx_msg void OnAlwaysOnTopWhenNotDocked();
   afx_msg void OnUpdateAlwaysOnTopWhenNotDocked(CCmdUI *pCmdUI);
   afx_msg void OnLOLLOL();
   afx_msg void OnUpdateLOLLOL(CCmdUI *pCmdUI);
   afx_msg void OnLOLINF();
   afx_msg void OnUpdateLOLINF(CCmdUI *pCmdUI);
   afx_msg void OnLOLUNF();
   afx_msg void OnUpdateLOLUNF(CCmdUI *pCmdUI);
   afx_msg void OnLOLTAG();
   afx_msg void OnUpdateLOLTAG(CCmdUI *pCmdUI);
   afx_msg void OnLOLWTF();
   afx_msg void OnUpdateLOLWTF(CCmdUI *pCmdUI);
   afx_msg void OnThingsILold();
   afx_msg void OnUpdateThingsILold(CCmdUI *pCmdUI);
   afx_msg void OnThingsITagd();
   afx_msg void OnUpdateThingsITagd(CCmdUI *pCmdUI);
   afx_msg void OnThingsIWrote();
   afx_msg void OnUpdateThingsIWrote(CCmdUI *pCmdUI);
   afx_msg void OnGotoLol();
   afx_msg void OnUpdateGotoLol(CCmdUI *pCmdUI);
   afx_msg void OnMyComments();
   afx_msg void OnUpdateMyComments(CCmdUI *pCmdUI);
   afx_msg void OnVanitySearch();
   afx_msg void OnUpdateVanitySearch(CCmdUI *pCmdUI);
   afx_msg void OnShackSearch();
   afx_msg void OnUpdateShackSearch(CCmdUI *pCmdUI);
   afx_msg void OnRepliesToMe();
   afx_msg void OnUpdateRepliesToMe(CCmdUI *pCmdUI);
   afx_msg void OnViewShowThumbs();
   afx_msg void OnUpdateViewShowThumbs(CCmdUI *pCmdUI);
   afx_msg void OnAutoLoadChattypicsThumbs();
   afx_msg void OnUpdateAutoLoadChattypicsThumbs(CCmdUI *pCmdUI);
   afx_msg void OnDontAutoLoadNWSThumbs();
   afx_msg void OnUpdateDontAutoLoadNWSThumbs(CCmdUI *pCmdUI);
   afx_msg void OnShackMsg_Inbox();
   afx_msg void OnUpdateShackMsg_Inbox(CCmdUI *pCmdUI);
   afx_msg void OnShackMsg_Outbox();
   afx_msg void OnUpdateShackMsg_Outbox(CCmdUI *pCmdUI);
   afx_msg void OnShackMsg_Archive();
   afx_msg void OnUpdateShackMsg_Archive(CCmdUI *pCmdUI);
};

class CHackedTabCtrl : public CMFCTabCtrl
{ friend class CLampApp;
public:
};

class CHackedContextMenuManager : public CContextMenuManager
{ friend class CLampApp;
public:
};

extern CLampApp theApp;
