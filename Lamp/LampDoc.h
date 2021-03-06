// LampDoc.h : interface of the CLampDoc class
//
#pragma once

#include "xmlelement.h"
#include <list>
#include <map>
#include "ChattyPost.h"
#include "html/ParserDom.h"

#define WM_THREAD_DOWNLOAD               (WM_USER + 50)

UINT DownloadThreadProc( LPVOID pParam );

bool GetXMLDataFromString(CXMLTree &xmldata, const char *data, int datasize);

void GetCharWidths(const UCChar *text, int *widths, size_t numchars, bool italic, bool bold, bool sample, bool quote, const UCChar *fontname, bool *pComplex = NULL);

typedef enum 
{
   DDT_EPICFAILD     = 0,
   DDT_STORY         = 1,
   DDT_THREAD        = 2,
   DDT_LOLS          = 3,
   DDT_SEARCH        = 4,
   DDT_SHACKMSG      = 5,
   DDT_PROFILE       = 6,
   DDT_ACTIVE_THREAD = 7
} DocDataType;

typedef enum 
{
   SMT_INBOX      = 0,
   SMT_OUTBOX     = 1,
   SMT_ARCHIVE    = 2
} ShackMsgType;

class CLampDoc;

class CDownloadData
{
public:
   CDownloadData()
   {
      m_data = NULL;
      m_datasize = 0;
      m_WhoWants = NULL;
      m_dt = DT_THREAD;
      m_id = 0;
      m_postrootid = 0;
      reply_to_id = 0;
      m_refreshid = 0;
      m_start_time = 0;
      m_recieve_time = 0;
      m_end_time = 0;
      m_secure = false;
   }

   void download(int numtries);
   void post(int numtries);
   void getchatty(int numtries);

   void setupshacklogin();
   void setupusershacklogin();

   UCString m_host;
   UCString m_path;
   UCString m_contenttrigger;
   UCString m_errmsg;
   UCString m_post_data;
   UCString m_username;
   UCString m_password;
   UCString m_originalurl;
   bool m_secure;
   void *m_data;
   int m_datasize;
   std::string m_stdstring;
   void *m_WhoWants;
   DownloadType m_dt;
   unsigned int m_id;
   unsigned int m_refreshid;
   unsigned int m_postrootid;
   unsigned int reply_to_id;

   DWORD m_start_time;
   DWORD m_recieve_time;
   DWORD m_end_time;
};

class CLampDoc : public CDocument
{
protected: // create from serialization only
	CLampDoc();
	DECLARE_DYNCREATE(CLampDoc)

// Attributes
public:
   virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);

   void SetView(CLampView *pView){m_pView = pView;}

   CLampView *GetView(){return m_pView;}

   DocDataType GetDataType(void){return m_datatype;}

   void Draw(HDC hDC, 
             int device_height, 
             RECT &DeviceRectangle, 
             int pos, 
             std::vector<CHotSpot> &hotspots, 
             unsigned int current_id, 
             bool bModToolIsUp, 
             RECT &ModToolRect, 
             unsigned int ModToolPostID,
             bool &newpostabove, 
             bool &newpostbelow,
             int topclip);

   void DoBoldFont(HDC hDC){::SelectObject(hDC, m_boldfont);}
   void FillBackground(HDC hDC, RECT &rect){::FillRect(hDC, &rect, m_backgroundbrush);}
   void FillBackgroundAuthorGlow(HDC hDC, RECT &rect, bool rightedge);
   void FillExpandedBackground(HDC hDC, RECT &rect, bool bAsRoot, postcategorytype posttype, bool bStrokeTopOnly, bool bUseCustomStrokeColor = false, COLORREF customcolor = 0);
   void StrokeShapedRect(HDC hDC, RECT &rect, int thickness);
   void DrawBodyText(HDC hDC,
                     RECT &rect,
                     std::vector<const UCChar*> &lines_of_text,
                     std::vector<const int*> &charsizes,
                     std::vector<const int> &linesizes,
                     std::vector<std::vector<shacktagpos>> &linetags,
                     std::vector<linetype> &linetypes,
                     std::vector<RECT> &spoilers,
                     std::vector<RECT> &links,
                     std::vector<RECT> &imagelinks,
                     std::vector<RECT> &images,
                     std::vector<RECT> &thumbs, 
                     bool bComplexShapeText,
                     const RECT *pClipRect = NULL,
                     bool bCenterSingleLine = false,
                     int numlinestodraw = -1);
   void DrawPreviewText(HDC hDC,
                        RECT &rect,
                        UCString &text,
                        int *charwidths,
                        std::vector<shacktagpos> &shacktags,
                        int shade,
                        bool &clipped, 
                        bool bComplexShapeText);
   void CalcBodyText(RECT &rect,
                     const UCChar *text,
                     int *widths,
                     std::vector<shacktagpos> &shacktags,
                     int numchars,
                     std::vector<const UCChar*> &lines_of_text,
                     std::vector<const int*> &charsizes,
                     std::vector<const int> &linesizes,
                     std::vector<std::vector<shacktagpos>> &linetags,
                     std::vector<linetype> &linetypes,
                     bool tight = false);
   void DrawNewMessagesTab(HDC hDC, RECT &rect, const UCChar *pChar, int *widths, size_t numchars, bool bHover);
   void DrawRootAuthor(HDC hDC, RECT &rect,UCString &author, COLORREF AuthorColor, CDCSurface *Flag, RECT &flagrect, bool bFade = false, bool bIsInbox = true, bool bPrefix = true, RECT *cliprect = NULL);
   void DrawDate(HDC hDC, RECT &rect, UCString &date, COLORREF ago_color, bool bGetExtents = false);
   void DrawRepliesHint(HDC hDC, RECT &rect, int m_reportedchildcount, bool numberonly = false, bool smallfont = false);
   void DrawCollapseNote(HDC hDC, RECT &rect);
   void DrawPreviewAuthor(HDC hDC, RECT &rect, UCString &text, bool clipped, int shade, COLORREF AuthorColor, const UCString &rootauthor, CDCSurface *Flag, RECT &flagrect);
   void DrawBranch(HDC hDC, RECT &rect, indenttype type, int shade, newness Newness);
   void DrawLOLField(HDC hDC, loltagtype type, RECT &rect, UCString &lols, bool bHover, bool bVoted, bool bRoot, bool bHasLols);
   int GetHeight(){return m_height;}
   bool FindNext(const UCChar *search, unsigned int &textselectionpost, int &selectionstart, int &selectionend);
   void RefreshAllRoots();
   bool PostReply(const UCString &replytext, unsigned int to_id);
   unsigned int GetRootId(unsigned int id);
   ChattyPost *GetRootPost(unsigned int id);
   unsigned int GetInitialPostId(void){return m_initialpostid;}
   void SetInitialPostId(unsigned int id){m_initialpostid = id;}
   void SetReplyDlg(CReplyDlg *pReplyDlg){m_pReplyDlg = pReplyDlg;}
   void UpdateLOLsRecurse();
   void UnloadAllImages();
   void UnloadImage(unsigned int image_index);
   void DetermineIfUserHasPostedInThreads();
   
   int GetPage(){return m_page;}
   void SetPage(int page);

   bool GetAuthorInfo(unsigned int post_id);
   bool LolTagPost(unsigned int post_id, loltagtype tag);

   HPEN GetRootTopPen(){return m_roottoppen;}

   unsigned int GetID(unsigned int id);

   int GetDesiredTitleSize(){return m_title.Length();}
   int GetAtualTitleSize(){return m_actualtitle.Length();}

   void TrimAtualTitle(int numchars)
   {
      m_actualtitle.TrimEnd(numchars);
      SetTitle(m_actualtitle);
   }

   void ExpandAtualTitle(int numchars)
   {
      const UCChar *pRead = m_title.Str() + m_actualtitle.Length();
      m_actualtitle.AppendUnicodeString(pRead, numchars);
      SetTitle(m_actualtitle);
   }

   void Viewed();
   void NewContent();
   void UpdateTabName();

   void StartDownload(const UCChar *host,
                      const UCChar *path,
                      const UCChar *contenttrigger,
                      DownloadType dt,
                      unsigned int id,
                      unsigned int refresh_id = 0,
                      unsigned int reply_to_id = 0,
                      const UCChar *post_data = NULL,
                      const UCChar *username = NULL,
                      const UCChar *password = NULL,
                      bool bIgnoreTimeStamp = false);

   void ProcessDownload(CDownloadData *pDD);

   void ClearAllPinnedThreads();

   void ClearChildren();

   void UpdateUnreadShackMessagesCount();

   void DeleteShackMessage(unsigned int id);

   ShackMsgType GetShackMessageType(){return m_shackmsgtype;}

   void SetCategory_Mod(unsigned int id, postcategorytype type);

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
   virtual BOOL OnOpenDocument( LPCTSTR lpszPathName );
   BOOL OnOpenDocumentImpl( LPCTSTR lpszPathName );
   virtual void SetTitle(LPCTSTR lpszTitle);

   void MySetTitle(LPCTSTR lpszTitle);

   bool RefreshThread(unsigned int id, 
                      unsigned int refresh_id, 
                      bool bStarting = false, 
                      unsigned int reply_to_id = 0, 
                      bool soft = false);
   bool Refresh(bool soft = false);
   void ProcessLOLData(char *data, int datasize);

   ChattyPost *FindRootPost(unsigned int id);
   ChattyPost *FindPost(unsigned int id);
   ChattyPost *GetThreadPost();

   void InvalidateSkin();
   void InvalidateFlags();

   bool IsBusy(){return m_bBusy;}

   void MarkShackMessageRead(unsigned int id);

   void SendMessage(const UCString &to, const UCString &subject, const UCString &shackmsg);

   void GetShackMessages();

   void GetLaunchString(UCString &launch, unsigned int current_id);

   unsigned int GetNextRoot(ChattyPost *pRootPost);
   unsigned int GetPrevRoot(ChattyPost *pRootPost);

   bool ReadShackMessages(CXMLTree &xmldata);

   void ReadShackMessagesHTML(std::string &stdstring);

   void MakePostAvailable(unsigned int id);

   bool IsFetchingNextPage(){return m_bFetchingNextPage;}

   bool FetchNextPage();

   int DrawBanner(HDC hDC, RECT &DeviceRectangle, int pos, std::vector<CHotSpot> &hotspots, bool bDrawNewThread, bool bDrawCompose, bool bDrawSearch, CLampView *pView);

   HFONT MySelectFont(HDC hdc, HFONT hfont)
   {
      HFONT result = (HFONT)::SelectObject(hdc,hfont);
      m_currentfont = hfont;
      return result;
   }

   void GDIPLUS_TextOut( HDC hdc, int x, int y, bool bSampleText, UINT options, CONST RECT * lprect, const UCChar *lpString, UINT c, const INT* lpDx);

   void SetLinkColor(COLORREF value){m_link_color = value;}
   void SetImageLinkColor(COLORREF value){m_image_link_color = value;}

   void GetSearchConditions(UCString &search_author,
                            UCString &search_parent_author,
                            UCString &search_terms,
                            UCString &search_filter,
                            UCString &search_sort)
   {
      search_author        = m_search_author;
      search_parent_author = m_search_parent_author;
      search_terms         = m_search_terms;
      search_filter        = m_search_filter;
      search_sort          = m_search_sort;
   }


   void SetSearchConditions(const UCString &search_author,
                            const UCString &search_parent_author,
                            const UCString &search_terms,
                            const UCString &search_filter,
                            const UCString &search_sort)
   {
      m_search_author        = search_author;
      m_search_parent_author = search_parent_author;
      m_search_terms         = search_terms;
      m_search_filter        = search_filter;
      m_search_sort          = search_sort;
   }

   unsigned int GetRootCount(){return m_rootposts.size();}

   DWORD GetLastRefreshTime(){return m_last_refresh_time;}

   bool IsExpired(){if(m_rootposts.size() == 1)return (*m_rootposts.begin())->IsExpired();return false;}

   void DemoteNewness(unsigned int id);

   void InvalidateContentLayout();

// Implementation
public:
	virtual ~CLampDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
   void SetDataType(DocDataType datatype){m_datatype = datatype;}
   bool ReadFromRoot(CXMLTree &xmldata, std::vector<unsigned int> &existing_threads);
   bool ReadSearchResultsFromRoot(CXMLTree &xmldata);
   void ReadChattyPageFromHTML(std::string &stdstring, std::vector<unsigned int> &existing_threads, bool bCheckForPages, bool bSkipExistingThreads = false, bool preservenewness = false);
   void ReadLatestChatty();
   void ReadLatestChattyPart2();   
   void ReadLOL(bool soft = false);
   void ReadProfile(const char *pText, int datasize);
   void PerformSearch(bool soft = false);
   void GetProfile();
   bool ReadExistingThreadFromRoot(CXMLTree &xmldata, unsigned int id, bool bDoingNewFlags);
   int DrawFromRoot(HDC hDC, 
                    RECT &DeviceRectangle, 
                    int pos, 
                    std::vector<CHotSpot> &hotspots, 
                    unsigned int current_id, 
                    bool bLinkOnly, 
                    bool bAllowModTools, 
                    bool bModToolIsUp, 
                    RECT &ModToolRect, 
                    unsigned int ModToolPostID, 
                    bool &newpostabove, 
                    bool &newpostbelow,
                    int topclip);
   int DrawMessages(HDC hDC, RECT &DeviceRectangle, int pos, std::vector<CHotSpot> &hotspots, unsigned int current_id);
   
   void CalcLineTags(std::vector<shacktagpos> &shacktags, std::vector<shacktagpos> &thislinetags, int beginpos, int endpos);
   void MyTextOut(HDC hdc, int x, int y, bool bSampleText, const UCChar *text, UINT count, const INT *widths, const RECT *pClipRect, bool bComplexShapeText);

   void GenerateRedirectedIDs();
   
   bool CheckForContentsRequest(unsigned int id);
   void RemoveContentsRequest(unsigned int id);

   void CalcWidthOfAverageProfileGroup();

   void RecordNewness();

   void MakePureNewThreadsOld();

   DocDataType            m_datatype;
   unsigned int           m_storyid;
   unsigned int           m_initialpostid;

   unsigned int           m_page;
   unsigned int           m_lastpage;
   UCString               m_story_name;

   std::list<ChattyPost*> m_rootposts;

   HBRUSH m_backgroundbrush;
   HBRUSH m_rootbackgroundbrush;
   HBRUSH m_replyexpandedbackgroundbrush;
   HBRUSH m_spoilerbrush;
   HPEN   m_roottoppen;
   HPEN   m_infpen;
   HPEN   m_nwspen;
   HPEN   m_stupidpen;
   HPEN   m_offtopicpen;
   HPEN   m_politicalpen;

   HPEN   m_nullpen;

   HPEN   m_branchpen;
   HPEN   m_branchpenisnew;
   HPEN   m_branchpenislast;
   HFONT  m_normalfont;
   HFONT  m_boldfont;
   HFONT  m_miscfont;
   HFONT  m_pagefont;
   HFONT  m_pagecountfont;
   HFONT  m_miscboldfont;
   HFONT  m_normalunderlinefont;
   HFONT  m_miscunderlinefont;

   HFONT  m_currentfont;

   UCString m_title;
   UCString m_actualtitle;
   int    m_height;

   CReplyDlg *m_pReplyDlg;

   UCString m_loltag;
   bool m_ThingsITAGD;
   bool m_ThingsILOLD;
   bool m_ThingsIWrote;
   bool m_ThingsUserWrote;

   UCString m_search_author;
   UCString m_search_parent_author;
   UCString m_search_terms;
   UCString m_search_filter;
   UCString m_search_sort;

   UCString m_profile_user;

   bool m_bScramblePath;

   bool m_bFetchingNextPage;

   std::map<unsigned int,unsigned int> m_id_redirects;

   CLampView *m_pView;

   bool m_bBusy;

   ShackMsgType m_shackmsgtype;

   UCString                m_lol_text;
   UCString                m_inf_text;
   UCString                m_unf_text;
   UCString                m_tag_text;
   UCString                m_wtf_text;
   UCString                m_ugh_text;

   std::map<unsigned int,DWORD> m_id_contents_requests;

   int         m_widthofaverageprofilegroup;

   COLORREF m_link_color;
   COLORREF m_image_link_color;

   DWORD m_last_refresh_time;

   std::map<unsigned int,newness> m_post_newness_for_refresh;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


