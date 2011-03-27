// LampDoc.h : interface of the CLampDoc class
//
#pragma once

#include "xmlelement.h"
#include "Chatty.h"
#include "thread.h"
#include <list>

typedef enum 
{
   HST_SCROLLBAR,
   HST_SCROLLBAR_UP,
   HST_SCROLLBAR_DOWN,
   HST_SCROLLBAR_THUMB,
   HST_REFRESH,
   HST_REPLIESTOROOTPOSTHINT,
   HST_OPENINTAB,
   HST_REPLYPREVIEW,
   HST_CREATEREPLY,
   HST_SPOILER,
   HST_TEXT,
   HST_LINK,
   HST_IMAGELINK,
   HST_CLOSEREPLYDLG,
   HST_POST,
   HST_TAG_RED,
   HST_TAG_GREEN,
   HST_TAG_BLUE,
   HST_TAG_YELLOW,
   HST_TAG_OLIVE,
   HST_TAG_LIME,
   HST_TAG_ORANGE,
   HST_TAG_PINK,
   HST_TAG_ITALICS,
   HST_TAG_BOLD,
   HST_TAG_QUOTE,
   HST_TAG_SAMPLE,
   HST_TAG_UNDERLINE,
   HST_TAG_STRIKE,
   HST_TAG_SPOILER,
   HST_TAG_CODE,
   HST_POSTAS,
   HST_REPLYTEXT,
   HST_SCROLLBAR_REPLYTEXT_DLG,
   HST_SCROLLBAR_UP_REPLYTEXT_DLG,
   HST_SCROLLBAR_DOWN_REPLYTEXT_DLG,
   HST_SCROLLBAR_THUMB_REPLYTEXT_DLG,
   HST_NEWTHREAD,
   HST_PREV_PAGE,
   HST_NEXT_PAGE,
   HST_PAGE,
   HST_AUTHOR,
   HST_AUTHORPREVIEW,
   HST_SPELL_SUGGESTION,
   HST_PREVIEW,
   HST_UNPREVIEW,
   HST_LOLTAG,
   HST_INFTAG,
   HST_UNFTAG,
   HST_TAGTAG,
   HST_WTFTAG,
   HST_CLOSEREPLY,
   HST_COLLAPSEPOST,
   HST_EXPAND,
   HST_PIN,
   HST_REFRESHSTORY,
   HST_CLOSE_MESSAGE,
   HST_REPLY_TO_MESSAGE,
   HST_FORWARD_MESSAGE,
   HST_COMPOSE_MESSAGE
}hotspottype;

typedef enum 
{
   N_OLD,
   N_LAST,
   N_NEW
}newness;

class CHotSpot
{
public:
   hotspottype    m_type;
   RECT           m_spot;
   unsigned int   m_id;
   bool           m_bAnim;
   bool           m_bOn;
};


typedef enum 
{
   ST_RED,
   ST_GREEN,
   ST_BLUE,
   ST_YELLOW,
   ST_OLIVE,
   ST_LIME,
   ST_ORANGE,
   ST_PURPLE,
   ST_PINK,
   ST_FUCHSIA,
   ST_QUOTE,
   ST_SAMPLE,
   ST_STRIKE,
   ST_ITALIC,
   ST_BOLD,
   ST_UNDERLINE,
   ST_SPOILER,
   ST_CODE,
   ST_LINK,
   ST_UNSPOILER,

   ST_RED_END,
   ST_GREEN_END,
   ST_BLUE_END,
   ST_YELLOW_END,
   ST_OLIVE_END,
   ST_LIME_END,
   ST_ORANGE_END,
   ST_PURPLE_END,
   ST_PINK_END,
   ST_QUOTE_END,
   ST_SAMPLE_END,
   ST_STRIKE_END,
   ST_ITALIC_END,
   ST_BOLD_END,
   ST_UNDERLINE_END,
   ST_SPOILER_END,
   ST_CODE_END,
   ST_LINK_END,
   ST_UNSPOILER_END,

   ST_IMAGE,
   ST_IMAGE_END,
}shacktag;

class shacktagpos
{
public:
   shacktagpos(shacktag tag, int pos, const UCChar *href = NULL)
   {
      m_tag = tag;
      m_pos = pos;
      m_image_index = 0;
      m_href = href;
   }
   shacktag m_tag;
   int m_pos;
   unsigned int m_image_index;
   UCString m_href;
};

class linetype
{
public:
   linetype(bool bIsText, unsigned int image_index, unsigned int height)
   {
      m_bIsText = bIsText;
      m_image_index = image_index;
      m_height = height;
   }
   bool m_bIsText;
   unsigned int m_image_index;
   unsigned int m_height;
};


/*
   <comment 
      preview="They've been in latest chatty too, we just catch them here quicker" 
      date="Wed Aug 25 08:16:00 -0700 2010" 
      reply_count="1" 
      category="ontopic" 
      id="23812643" 
      author="geedeck">

      <body>They've been in latest chatty too, we just catch them here quicker</body> 

      <comments>
         <comment/> 
      </comments>

  </comment>
*/

class CLampDoc;
class CReplyDlg;
typedef enum 
{
   IT_VERT,
   IT_TO_ME_FORK,
   IT_TO_ME_END
}indenttype;

typedef enum 
{
   PCT_NORMAL,
   PCT_INF,
   PCT_NWS,
   PCT_STUPID,
   PCT_OFFTOPIC,
   PCT_POLITCIAL
}postcategorytype;

typedef enum 
{
   LTT_NONE = 0,
   LTT_LOL = 1,
   LTT_INF = 2,
   LTT_UNF = 4,
   LTT_TAG = 8,
   LTT_WTF = 16
}loltagtype;

class ChattyPost
{
public:
   ChattyPost()
   {
      m_category = PCT_NORMAL;
      m_pParent = NULL;
      m_pNextSibling = NULL;
      m_pPrevSibling = NULL;
      m_pDoc = NULL;
      m_id = 0;
      m_pCharWidths = NULL;
      m_pAuthorCharWidths = NULL;
      m_pSubjectCharWidths = NULL;
      m_lasttextrectwidth = 0;
      m_textrectheight = 0;
      m_reportedchildcount = 0;
      m_previewshade = 0;
      m_authorsize = 0;
      m_authorpreviewsize = 0;
      m_drewtextpos = 0;
      m_drewtextedge = 0;
      m_bDrewTextBody = false;
      m_pos = 0;
      m_pReplyDlg = NULL;
      m_mylols = LTT_NONE;
      m_bCollapsed = false;
      m_Newness = N_NEW;
      m_bShowTruncated = false;
      m_familysize = 0;
      m_bRefreshing = false;
      m_bPinned = false;
      m_ago_color = 0;
      m_bHaveRead = false;
      m_bIsInbox = false;
   }
   virtual ~ChattyPost();

   unsigned int GetId(){return m_id;}
   void SetId(unsigned int newid){m_id = newid;}

   void SetNewness(newness value){m_Newness = value;}
   void BumpNewnessDown(){if(m_Newness == N_NEW)m_Newness = N_LAST;else if(m_Newness == N_LAST)m_Newness = N_OLD;}
   bool IsNew(){return (m_Newness == N_NEW);}
   newness GetNewness(){return m_Newness;}

   void Collapse(){m_bCollapsed = true;}
   void Expand(){m_bCollapsed = false;}
   bool IsCollapsed(){return m_bCollapsed;}

   void SetDoc(CLampDoc *pDoc){m_pDoc = pDoc;}
   void SetPreviewShade(int value){m_previewshade = value;}

   void SetParent(ChattyPost *pParent){m_pParent = pParent;}
   ChattyPost *GetParent(void){return m_pParent;}
   
   void SetNextSibling(ChattyPost *pNextSibling){m_pNextSibling = pNextSibling;}
   ChattyPost *GetNextSibling(void){return m_pNextSibling;}

   void SetPrevSibling(ChattyPost *pPrevSibling){m_pPrevSibling = pPrevSibling;}
   ChattyPost *GetPrevSibling(void){return m_pPrevSibling;}

   void Read(CXMLElement *pElement, CLampDoc *pDoc, bool bDoingNewFlags);
   void ReadSearchResult(CXMLElement *pElement, CLampDoc *pDoc);
   void ReadShackMessage(CXMLElement *pElement, CLampDoc *pDoc, bool bIsInbox);

   void ReadPost(ChattyPost *pOther, CLampDoc *pDoc);

   void ReadLOL(CLampDoc *pDoc,
                UCString &id,
                UCString &lolcount,
                UCString &author,
                UCString &body);
   int DrawRoot(HDC hDC, RECT &DeviceRectangle, int pos, std::vector<CHotSpot> &hotspots, unsigned int current_id, bool bLinkOnly);
   int DrawReply(HDC hDC, RECT &DeviceRectangle, int pos, std::vector<CHotSpot> &hotspots, int indent, unsigned int current_id, int &trunkatingposts, const UCString &rootauthor);
   int DrawMessage(HDC hDC, RECT &DeviceRectangle, int pos, std::vector<CHotSpot> &hotspots);
   void DrawTextOnly(HDC hDC, RECT &DeviceRectangle, int pos);
   void SetupPreviewShades(bool bDoingNewFlags);

   ChattyPost *FindChild(unsigned int id);

   void GetTitle(UCString &title);

   postcategorytype GetCategory(){return m_category;}

   void ClearSpoilerTags(int x, int y);
   void GetLink(int x, int y, UCString &link);
   void GetImageLink(int x, int y, UCString &link);
   void MakeLinkIntoImage(int x, int y, unsigned int &index);
   void MakeImageIntoLink(int x, int y);

   int GetCharPos(int x, int y);
   void GetCharPosesForWord(int x, int y, int &selectionstart, int &selectionend);
   void GetCharPosesForPara(int x, int y, int &selectionstart, int &selectionend);

   void GetTextSelectionRects(int selectionstart, int selectionend, std::vector<RECT> &selectionrects);
   void GetSelectedText(int selectionstart, int selectionend, UCString &selectionrects);

   bool FindNext(const UCChar *search, unsigned int &textselectionpost, int &selectionstart, int &selectionend);

   int GetPos(){return m_pos;}
   int GetHeight(){return m_height;}

   void SetReplyDlg(CReplyDlg *pReplyDlg){m_pReplyDlg = pReplyDlg;}

   UCString &GetAuthor(){return m_author;}

   void InvalidateSkin();

   void DecodeShackTagsString(UCString &from);

   void AddLolTag(loltagtype tag){m_mylols |= tag;}

   void LoadAllImageLinks();
   void CloseAllImageLinks();

   bool IsFiltered();

   bool HasMissingChildren()
   {
      if(m_reportedchildcount > 0 &&
         m_children.size() == 0)
      {
         return true;
      }
      return false;
   }

   void ShowAsTruncated(){m_bShowTruncated = true;}
   void UnShowAsTruncated(){m_bShowTruncated = false;}

   void CountFamilySize();

   void UpdateAuthorColor();

   std::list<ChattyPost*> *GetChildren(){return &m_children;}

   void SetRefreshing(bool bValue){m_bRefreshing = bValue;}

   void SetPinned(bool bValue){m_bPinned = bValue;}
   bool IsPinned(){return m_bPinned;}

   unsigned int GetPrevReply(bool bSkipSelf = false);
   unsigned int GetNextReply(bool bSkipSelf = false);

   void UpdateRootReplyList(std::vector<unsigned int> *list = NULL);

   bool GetHaveRead(){return m_bHaveRead;}
   void SetHaveRead(bool value){m_bHaveRead = value;}

   const UCString &GetSubject(){return m_subject;}
   const UCString &GetBodyText(){return m_bodytext;}
   const UCString &GetDateText(){return m_datetext;}

protected:
   void SetupCharWidths();
   void SetupBodyText(RECT &textrect);
   void InitImageLinks();
   void PopTag(std::vector<shacktag> &shacktags, shacktag who);
   void DecodeString(UCString &from, UCString &to, std::vector<shacktagpos> &shacktags);
   void GatherIds(std::list<unsigned int> &ids);
   void RemoveSomeTags(UCString &str);
   void AddToFamilySize(size_t &familysize);
   void UpdateDate();

   bool                    m_bCollapsed;
   newness                 m_Newness;
   int                     m_pos;
   int                     m_height;
   int                     m_drewtextpos;
   int                     m_drewtextedge;
   bool                    m_bDrewTextBody;
   UCString                m_bodytext;
   UCString                m_datetext;
   postcategorytype        m_category;
   UCString                m_author;
   UCString                m_subject;
   unsigned int            m_id;
   int                    *m_pCharWidths;
   int                    *m_pAuthorCharWidths;
   int                    *m_pSubjectCharWidths;
   std::vector<shacktagpos> m_shacktags;

   std::vector<const UCChar*> m_lines_of_text;
   std::vector<const int*> m_charsizes;
   std::vector<const int>  m_linesizes;
   std::vector<std::vector<shacktagpos>> m_linetags;
   std::vector<linetype>   m_linetypes;

   int                     m_lasttextrectwidth;
   int                     m_textrectheight;
   int                     m_previewshade;
   int                     m_authorsize;
   int                     m_authorpreviewsize;
   
   ChattyPost             *m_pParent;
   ChattyPost             *m_pPrevSibling;
   ChattyPost             *m_pNextSibling;
   CLampDoc               *m_pDoc;
   int                     m_reportedchildcount;
   std::list<ChattyPost*>  m_children;

   CReplyDlg              *m_pReplyDlg;
   byte                    m_mylols;

   bool                    m_bShowTruncated;
   size_t                  m_familysize;

   COLORREF                m_AuthorColor;

   bool                    m_bRefreshing;

   bool                    m_bPinned;

   COLORREF                m_ago_color;

   std::vector<unsigned int> m_root_reply_list;

   bool                    m_bHaveRead;
   bool                    m_bIsInbox;
};
