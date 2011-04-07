
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

#define LAMP_VERSION_MAJOR 1
#define LAMP_VERSION_MINOR 7

chattyerror download(const char* host, const char* path, char** out_response, int *psize=NULL);

typedef enum 
{
   SBT_INACTIVE,
   SBT_ACTIVE,
   SBT_HOVER
}scrollbitype;

class CLampView;
class DockTab;

class Hunspell;

class CMyLol
{
public:
   unsigned int m_post_id;
   byte m_mylols;
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

class CImageCacheItem
{
public:

   CImageCacheItem()
   {
   }

   CImageCacheItem(const CImageCacheItem &other)
   {
      m_host = other.m_host;
      m_path = other.m_path;
      m_ext = other.m_ext;
      m_tempfilename = other.m_tempfilename;
   }

   ~CImageCacheItem()
   {
   }

   CDCSurface *GetImage();

   UCString m_host;
   UCString m_path;
   UCString m_ext;
   UCString m_tempfilename;

   CDCSurface m_image;
};

class CLampApp : public CWinAppEx
{
public:
	CLampApp();

   virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);

   void OpenShackLink(const UCString &shackpath);

   void SetStatusBarText(const UCString &text, CLampView *pView);

   bool HaveLogin();
   bool Login();

   void AddDocToList(CLampDoc *pDoc){m_MyDocuments.push_back(pDoc);}
   void RemoveDocFromList(CLampDoc *pDoc){m_MyDocuments.remove(pDoc);}

   ChattyPost *FindFromAnywhere(unsigned int id);

   bool HasLinkedImage(const UCString &link, unsigned int &index);

   void UpdateTabSizes();

   CDCSurface *GetLinkedImage(const UCString &link, unsigned int &index);

   CDCSurface *GetLinkedImage(unsigned int index);

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

   CDCSurface *GetINFImage(bool root)
   {
      if(root)
      {
         return &m_infroot;
      }
      else
      {
         return &m_infreply;
      }
   }

   CDCSurface *GetINFStar(){return &m_infstar;}

   CDCSurface *GetNWSImage(bool root)
   {
      if(root)
      {
         return &m_nwsroot;
      }
      else
      {
         return &m_nwsreply;
      }
   }

   CDCSurface *GetNWSStar(){return &m_nwsstar;}

   CDCSurface *GetStupidImage(bool root)
   {
      if(root)
      {
         return &m_stupidroot;
      }
      else
      {
         return &m_stupidreply;
      }
   }

   CDCSurface *GetStupidStar(){return &m_stupidstar;}

   CDCSurface *GetOffTopicImage(bool root)
   {
      if(root)
      {
         return &m_offtopicroot;
      }
      else
      {
         return &m_offtopicreply;
      }
   }

   CDCSurface *GetOffTopicStar(){return &m_offtopicstar;}

   CDCSurface *GetPoliticalImage(bool root)
   {
      if(root)
      {
         return &m_politicalroot;
      }
      else
      {
         return &m_politicalreply;
      }
   }

   CDCSurface *GetPoliticalStar(){return &m_politicalstar;}

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

   CDCSurface *GetTagsImage(bool bHover)
   {
      if(bHover)
      {
         return &m_tags_hover;
      }
      else
      {
         return &m_tags;
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

   UCString &GetHostName(){return m_hostname;}
   UCString &GetUserHostName(){return m_userhostname;}
   UCString &GetLolHostName(){return m_lolhostname;}
   
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

   COLORREF GetUserColor(UCString &name);

   const UCChar *GetNormalFontName(){return m_normal_fontname;}
   const UCChar *GetQuotedFontName(){return m_quoted_fontname;}
   const UCChar *GetCodeFontName(){return m_code_fontname;}

   void SetNormalFontName(const UCChar *value){m_normal_fontname = value;InvalidateSkinAllViews();}
   void SetQuotedFontName(const UCChar *value){m_quoted_fontname = value;InvalidateSkinAllViews();}
   void SetCodeFontName(const UCChar *value){m_code_fontname = value;InvalidateSkinAllViews();}

   bool RoundedPosts(){return m_rounded_posts;}

   bool StrokeRootEdges(){return m_stroke_root_edges;}

   int GetTabTitleWordLimit(){return m_tab_title_word_limit;}
   int GetTabTitleCharLimit(){return m_tab_title_char_limit;}

   bool GetSmoothScroll(){return m_smooth_scroll;}
   float GetSmoothScrollScale(){return m_smoothscrollscale;}
   float GetMouseWheelScale(){return m_mouse_wheel_scale;}
   int GetNumMinutesCheckInbox(){return m_num_minutes_check_inbox;}
   void SetNumMinutesCheckInbox(int value);

   void SetMouseWheelScale(float value){m_mouse_wheel_scale = value;}

   bool IsSpellCheckerEnabled(){return m_enable_spell_checker;}
   void SetIsSpellCheckerEnabled(bool value){m_enable_spell_checker = value;}

   bool GetHighlightOP(){return m_highlight_OP;}
   void SetHighlightOP(bool value){m_highlight_OP = value;}

   void AddView(CLampView *pView){m_views.push_back(pView);}
   void RemoveView(CLampView *pView){m_views.remove(pView);}

   void InvalidateSkinAllViews();

   float GetTextScaler(){return m_textscaler;}
   void SetTextScaler(float value)
   {
      m_textscaler = value;
      m_textscaler = __min(5.0f,m_textscaler);
      m_textscaler = __max(0.5f,m_textscaler);
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

   void GetSpellSuggestions(const UCChar *wordtotest, size_t count, std::vector<UCString> &suggestions);

   void AddMyLol(unsigned int post_id, loltagtype);
   byte GetMyLol(unsigned int post_id);

   void AddMyCollapse(unsigned int post_id);
   void RemoveMyCollapse(unsigned int post_id);
   bool GetMyCollapse(unsigned int post_id);

   void GetLastSearchParms(UCString &author, UCString &parent_author, UCString &terms)
   {
      author = m_last_search_author;
      parent_author = m_last_search_parent_author;
      terms = m_last_search_terms;
   }

   void SetLastSearchParms(const UCString &author, const UCString &parent_author, const UCString &terms)
   {
      m_last_search_author = author;
      m_last_search_parent_author = parent_author;
      m_last_search_terms = terms;
   }

   bool GetAutoShowLoadedImages(){return m_AutoShowLoadedImages;}

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

   bool AlwaysOnTopWhenNotDocked(){return m_bAlwaysOnTopWhenNotDocked;}

   void SetAlwaysOnTopWhenNotDocked(bool value){m_bAlwaysOnTopWhenNotDocked = value;}

   void ClearAllPinnedThreads();

   UCString &GetLastPost(){return m_lastpost;}
   void SetLastPost(const UCString &post){m_lastpost = post;}

   void SendMessageDlg(CLampDoc *pDoc, const UCString &to, const UCString &subject, const UCString &shackmsg);

   void UpdateInbox();

   void CheckForUpdates();

   void RefreshLOLs();

   UCString &GetFindText(){return m_findtext;}
   void SetFindText(UCString &txt){m_findtext = txt;}

   void AddLOL_LOL(unsigned int post_id, unsigned int count);
   void AddLOL_INF(unsigned int post_id, unsigned int count);
   void AddLOL_UNF(unsigned int post_id, unsigned int count);
   void AddLOL_TAG(unsigned int post_id, unsigned int count);
   void AddLOL_WTF(unsigned int post_id, unsigned int count);
   CLOLFlags &GetKnownLOLFlags(unsigned int post_id);

   int GetLOLFieldWidth(){return m_LOLFieldWidth;}

   bool ShowSmallLOL(){return m_bShowSmallLOL;}
   void SetShowSmallLOL(bool value);

   bool FlaredBranches(){return m_bFlaredBranches;}
   void SetFlaredBranches(bool value){m_bFlaredBranches = value;}

   void ClearSession(){m_session.clear();}

   void AddToSession(UCString &launch){m_session.push_back(launch);}

   std::vector<UCString> GetCheatSheet(){return m_cheatsheet;}
   size_t GetCheatSheetSize(){return m_cheatsheet.size();}
      
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
   CDCSurface m_infreply;
   CDCSurface m_infroot;
   CDCSurface m_infstar;
   CDCSurface m_nwsreply;
   CDCSurface m_nwsroot;
   CDCSurface m_nwsstar;
   CDCSurface m_stupidroot;
   CDCSurface m_stupidreply;
   CDCSurface m_stupidstar;
   CDCSurface m_offtopicroot;
   CDCSurface m_offtopicreply;
   CDCSurface m_offtopicstar;
   CDCSurface m_politicalroot;
   CDCSurface m_politicalreply;
   CDCSurface m_politicalstar;
   CDCSurface m_close;
   CDCSurface m_close_hover;
   CDCSurface m_post;
   CDCSurface m_post_hover;
   CDCSurface m_tags;
   CDCSurface m_tags_hover;
   CDCSurface m_newthread;
   CDCSurface m_newthread_hover;
   CDCSurface m_preview;
   CDCSurface m_preview_hover;
   CDCSurface m_docktabimage;

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
   CDCSurface m_forward;
   CDCSurface m_forward_hover;

   CDCSurface m_mb_pan;

   CDCSurface m_refresh_anim[8];

   UCString m_hostname;
   UCString m_userhostname;
   UCString m_lolhostname;

   UCString m_username;
   UCString m_password;

   bool m_rounded_posts;

   bool m_stroke_root_edges;

   UCString m_lang;

   int m_tab_title_word_limit;
   int m_tab_title_char_limit;

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

   bool m_AutoShowLoadedImages;

   bool m_smooth_scroll;
   float m_smoothscrollscale;
   float m_mouse_wheel_scale;
   UCString m_skinname;

   int m_fontheight;
   int m_miscfontheight;
   int m_samplefontheight;
   int m_textheight;
   int m_cellheight;

   float m_textscaler;

   int m_numshow_truncated;

   bool m_bPinningInStories;
   bool m_bDoublePageStory;
   bool m_bStartInDockedMode;
   bool m_bShowLOLButtons;
   bool m_bAlwaysOnTopWhenNotDocked;

   std::map<UCString,COLORREF> m_namelist;

   std::list<CLampView*> m_views;

   Hunspell *m_pHunspell;

   std::list<CMyLol> m_mylols;
   std::list<unsigned int> m_mycollapses;

   UCString m_last_search_author;
   UCString m_last_search_parent_author;
   UCString m_last_search_terms;

   std::vector<CImageCacheItem> m_imagecache;

   std::list<CLampDoc*> m_MyDocuments;

   std::list<CShackBookmark> m_Bookmarks;

   std::map<unsigned int,CXMLElement *> m_cachedthreadreplies;
   int m_numcachedthreadreplyinserts;

   std::map<unsigned int,CLOLFlags> m_cachedLOLposts;

   UCString m_userpath;
   UCString m_appdatapath;

   CXMLElement m_Mods;
   CXMLElement m_ShackEmployees;
   CXMLElement m_GameDevs;

   UCString m_normal_fontname;
   UCString m_quoted_fontname;
   UCString m_code_fontname;

   UCString m_lastpost;

   int m_num_minutes_check_inbox;

   float m_mb_pan_scale;

   bool m_enable_spell_checker;

   bool m_highlight_OP;

   UCString m_findtext;

   int m_LOLFieldWidth;

   bool m_bShowSmallLOL;

   bool m_bFlaredBranches;

   float m_inertia_friction;

   std::vector<UCString> m_cheatsheet;

   std::vector<UCString> m_session;

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
   afx_msg void OnViewAutoshowloadedimages();
   afx_msg void OnUpdateViewAutoshowloadedimages(CCmdUI *pCmdUI);
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

extern CLampApp theApp;
