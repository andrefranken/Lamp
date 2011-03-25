// LampDoc.h : interface of the CLampDoc class
//
#pragma once

#include "xmlelement.h"
#include <list>
#include <map>
#include "ChattyPost.h"

#define WM_THREAD_DOWNLOAD               (WM_USER + 50)

typedef enum 
{
   DDT_EPICFAILD     = 0,
   DDT_STORY         = 1,
   DDT_THREAD        = 2,
   DDT_LOLS          = 3,
   DDT_SEARCH        = 4,
   DDT_SHACKMSG      = 5
} DocDataType;

typedef enum 
{
   DT_THREAD       = 0,
   DT_STORY        = 1,
   DT_SEARCH       = 2,
   DT_THREAD_START = 3,
   DT_LOL          = 4,
   DT_AUTHOR       = 5,
   DT_POST         = 6,
   DT_STORY_2      = 7,
   DT_SHACKMSG     = 8,
   DT_READMSG      = 9,
   DT_SENDMSG      = 10
} DownloadType;

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
   }

   void download(int numtries);
   void post(int numtries);

   UCString m_host;
   UCString m_path;
   UCString m_errmsg;
   UCString m_post_data;
   UCString m_username;
   UCString m_password;
   void *m_data;
   int m_datasize;
   CLampDoc *m_WhoWants;
   DownloadType m_dt;
   unsigned int m_id;
   unsigned int m_refreshid;
   unsigned int m_postrootid;
   unsigned int reply_to_id;
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

   DocDataType GetDataType(void){return m_datatype;}
   void Draw(HDC hDC, RECT &DeviceRectangle, int pos, std::vector<CHotSpot> &hotspots, unsigned int current_id);

   void GetCharWidths(UCString &text, int *widths, bool bold, bool sample, const UCChar *fontname);
   void DoBoldFont(HDC hDC){::SelectObject(hDC, m_boldfont);}
   void FillBackground(HDC hDC, RECT &rect){::FillRect(hDC, &rect, m_backgroundbrush);}
   void FillExpandedBackground(HDC hDC, RECT &rect, bool bAsRoot, postcategorytype posttype, bool bStrokeTopOnly);
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
                     const RECT *pClipRect = NULL);
   void DrawPreviewText(HDC hDC,
                        RECT &rect,
                        UCString &text,
                        int *charwidths,
                        std::vector<shacktagpos> &shacktags,
                        int shade,
                        bool &clipped);
   void CalcBodyText(RECT &rect,
                     const UCChar *text,
                     const int *widths,
                     std::vector<shacktagpos> &shacktags,
                     int numchars,
                     std::vector<const UCChar*> &lines_of_text,
                     std::vector<const int*> &charsizes,
                     std::vector<const int> &linesizes,
                     std::vector<std::vector<shacktagpos>> &linetags,
                     std::vector<linetype> &linetypes);
   void DrawRootAuthor(HDC hDC, RECT &rect,UCString &author, COLORREF AuthorColor, bool bFade = false, bool m_bIsInbox=true);
   void DrawDate(HDC hDC, RECT &rect, UCString &date, COLORREF ago_color, bool bGetExtents=false);
   void DrawRepliesHint(HDC hDC, RECT &rect, int m_reportedchildcount);
   void DrawCollapseNote(HDC hDC, RECT &rect);
   void DrawPreviewAuthor(HDC hDC, RECT &rect, UCString &text, bool clipped, int shade, COLORREF AuthorColor);
   void DrawBranch(HDC hDC, RECT &rect, indenttype type, int shade, newness Newness);
   int GetHeight(){return m_height;}
   bool FindNext(const UCChar *search, unsigned int &textselectionpost, int &selectionstart, int &selectionend);
   void RefreshAllRoots();
   bool PostReply(const UCString &replytext, unsigned int to_id);
   unsigned int GetRootId(unsigned int id);
   ChattyPost *GetRootPost(unsigned int id);
   unsigned int GetInitialPostId(void){return m_initialpostid;}
   void SetInitialPostId(unsigned int id){m_initialpostid = id;}
   void SetReplyDlg(CReplyDlg *pReplyDlg){m_pReplyDlg = pReplyDlg;}

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

   void StartDownload(const UCChar *host,
                      const UCChar *path,
                      DownloadType dt,
                      unsigned int id,
                      unsigned int refresh_id = 0,
                      unsigned int reply_to_id = 0,
                      const UCChar *post_data = NULL,
                      const UCChar *username = NULL,
                      const UCChar *password = NULL);

   void ProcessDownload(CDownloadData *pDD);

   void ClearAllPinnedThreads();

   void UpdateUnreadShackMessagesCount();

   void DeleteShackMessage(unsigned int id);

   ShackMsgType GetShackMessageType(){return m_shackmsgtype;}

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
   virtual BOOL OnOpenDocument( LPCTSTR lpszPathName );
   virtual void SetTitle(LPCTSTR lpszTitle);

   void MySetTitle(LPCTSTR lpszTitle);

   bool RefreshThread(unsigned int id, unsigned int refresh_id, bool bStarting = false, unsigned int reply_to_id = 0);
   bool Refresh();
   void ProcessLOLData(char *data, int datasize);

   ChattyPost *FindRootPost(unsigned int id);
   ChattyPost *FindPost(unsigned int id);
   ChattyPost *GetThreadPost();

   void InvalidateSkin();

   bool IsBusy(){return m_bBusy;}

   void MarkShackMessageRead(unsigned int id);

   void SendMessage(const UCString &to, const UCString &subject, const UCString &shackmsg);

// Implementation
public:
	virtual ~CLampDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
   bool GetXMLDataFromString(CXMLTree &xmldata, const char *data, int datasize);
   void SetDataType(DocDataType datatype){m_datatype = datatype;}
   bool ReadFromRoot(CXMLTree &xmldata, std::vector<unsigned int> &existing_threads);
   bool ReadSearchResultsFromRoot(CXMLTree &xmldata);
   bool ReadShackMessages(CXMLTree &xmldata);
   void ReadLatestChatty();
   void ReadLatestChattyPart2();
   void GetShackMessages();
   void ReadLOL();
   void PerformSearch();
   bool ReadExistingThreadFromRoot(CXMLTree &xmldata, unsigned int id, bool bDoingNewFlags);
   int DrawFromRoot(HDC hDC, RECT &DeviceRectangle, int pos, std::vector<CHotSpot> &hotspots, unsigned int current_id, bool bLinkOnly);
   int DrawMessages(HDC hDC, RECT &DeviceRectangle, int pos, std::vector<CHotSpot> &hotspots);

   int DrawBanner(HDC hDC, RECT &DeviceRectangle, int pos, std::vector<CHotSpot> &hotspots, bool bDrawNewThread, bool bDrawCompose);

   void CalcLineTags(std::vector<shacktagpos> &shacktags, std::vector<shacktagpos> &thislinetags, int beginpos, int endpos);
   void MyTextOut(HDC hdc, int x, int y, const UCChar *text, UINT count, const INT *widths, const RECT *pClipRect);

   void GenerateRedirectedIDs();
     

   DocDataType            m_datatype;
   unsigned int           m_storyid;
   unsigned int           m_initialpostid;

   unsigned int           m_page;
   unsigned int           m_lastpage;
   UCString               m_story_name;

   std::list<ChattyPost*> m_rootposts;

   HDC    m_hTempDC;
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

   UCString m_title;
   UCString m_actualtitle;
   int    m_height;

   CReplyDlg *m_pReplyDlg;

   UCString m_loltag;
   bool m_ThingsILOLD;
   bool m_ThingsIWrote;

   UCString m_search_author;
   UCString m_search_parent_author;
   UCString m_search_terms;

   bool m_bScramblePath;

   std::map<unsigned int,unsigned int> m_id_redirects;

   CLampView *m_pView;

   bool m_bBusy;

   ShackMsgType m_shackmsgtype;
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

