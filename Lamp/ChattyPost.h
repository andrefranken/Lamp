// LampDoc.h : interface of the CLampDoc class
//
#pragma once

#include "xmlelement.h"
#include "Chatty.h"
#include "thread.h"
#include "Lamp.h"
#include <list>
#include "html/ParserDom.h"

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
   HST_IMAGE_LINK,
   HST_IMAGE,
   HST_THUMB,
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
   HST_DELETE_MESSAGE,
   HST_COMPOSE_MESSAGE,
   HST_NEW_MESSAGES_NOTE,
   HST_BANNER_BACKGROUND,
   HST_LIGHTNINGBOLT,
   HST_MOD_TOOL,
   HST_MOD_TOOL_ITEM

}hotspottype;

typedef enum 
{
   N_OLD,
   N_LAST,
   N_NEW
}newness;

class CLOLFlags
{
public:
   CLOLFlags()
   {
      m_LOLd = 0;
      m_INFd = 0;
      m_UNFd = 0;
      m_TAGd = 0;
      m_WTFd = 0;
   }

   unsigned int m_LOLd;
   unsigned int m_INFd;
   unsigned int m_UNFd;
   unsigned int m_TAGd;
   unsigned int m_WTFd;
};


typedef enum 
{
   ST_RED = 0,
   ST_GREEN,
   ST_BLUE,
   ST_YELLOW,
   ST_OLIVE,
   ST_LIME,
   ST_ORANGE,
   ST_PURPLE,
   ST_PINK,
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
   ST_INVERT,
   ST_IMAGE,
   ST_IMAGE_LINK,
   ST_THUMB,

   ST_RED_END = 30,
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
   ST_INVERT_END,
   ST_IMAGE_END,
   ST_IMAGE_LINK_END,
   ST_THUMB_END,

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
   linetype(bool bIsText, unsigned int image_index, unsigned int height, bool bIsThumb)
   {
      m_bIsText = bIsText;
      m_image_index = image_index;
      m_height = height;
      m_bIsThumb = bIsThumb;
   }
   bool m_bIsText;
   bool m_bIsThumb;
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
   PCT_POLITICAL,
   PCT_NUKED
}postcategorytype;

class CHotSpot
{
public:
   hotspottype    m_type;
   RECT           m_spot;
   unsigned int   m_id;
   bool           m_bAnim;
   bool           m_bOn;

   UCString       m_loltext;
   bool           m_lolvoted;
   bool           m_lolroot;
   bool           m_haslols;
   postcategorytype m_cat_type;
};


typedef enum 
{
   LTT_NONE = 0,
   LTT_LOL = 1,
   LTT_INF = 2,
   LTT_UNF = 4,
   LTT_TAG = 8,
   LTT_WTF = 16
}loltagtype;

size_t HTML_GetIDAttribute(tree<htmlcxx::HTML::Node>::sibling_iterator &it, const char *attr_name = NULL);

bool HTML_HasAttribute(tree<htmlcxx::HTML::Node>::sibling_iterator &it, 
                       const char *attribute_name, 
                       const char *attribute_value);

bool HTML_StartsWithAttribute(tree<htmlcxx::HTML::Node>::sibling_iterator &it, 
                              const char *attribute_name, 
                              const char *attribute_value, 
                              std::string *attribute_value_remainder = 0);

void HTML_GetValue(tree<htmlcxx::HTML::Node>::sibling_iterator &from_it, std::string &value);

bool HTML_FindChild(tree<htmlcxx::HTML::Node>::sibling_iterator &from_it, 
                    tree<htmlcxx::HTML::Node>::sibling_iterator &result_it, 
                    const char *tag_name);

bool HTML_FindChild_HasAttribute(tree<htmlcxx::HTML::Node>::sibling_iterator &from_it,
                                 tree<htmlcxx::HTML::Node>::sibling_iterator &result_it, 
                                 const char *tag_name, 
                                 const char *attribute_name, 
                                 const char *attribute_value);

bool HTML_FindChild_StartsWithAttribute(tree<htmlcxx::HTML::Node>::sibling_iterator &from_it,
                                        tree<htmlcxx::HTML::Node>::sibling_iterator &result_it, 
                                        const char *tag_name, 
                                        const char *attribute_name, 
                                        const char *attribute_value, 
                                        std::string *attribute_value_remainder = 0);

bool HTML_FindChild(tree<htmlcxx::HTML::Node> &from_dom, 
                    tree<htmlcxx::HTML::Node>::sibling_iterator &result_it, 
                    const char *tag_name);

bool HTML_FindChild_HasAttribute(tree<htmlcxx::HTML::Node> &from_dom,
                                 tree<htmlcxx::HTML::Node>::sibling_iterator &result_it, 
                                 const char *tag_name, 
                                 const char *attribute_name, 
                                 const char *attribute_value);

bool HTML_FindChild_StartsWithAttribute(tree<htmlcxx::HTML::Node> &from_dom,
                                        tree<htmlcxx::HTML::Node>::sibling_iterator &result_it, 
                                        const char *tag_name, 
                                        const char *attribute_name, 
                                        const char *attribute_value, 
                                        std::string *attribute_value_remainder = 0);

bool PopTag(std::vector<shacktag> &shacktags, shacktag who);
void FindBadShackTagsString(UCString &from, std::vector<shacktagpos> &shacktags);
void MergeTags(std::vector<shacktagpos> &ina, std::vector<shacktagpos> &inb, std::vector<shacktagpos> &out);

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
      //m_Newness = N_OLD;
      m_Newness = N_NEW;
      m_bShowTruncated = false;
      m_familysize = 0;
      m_bRefreshing = false;
      m_bPinned = false;
      m_ago_color = 0;
      m_bHaveRead = false;
      m_bIsInbox = false;
      m_plol_preview_charwidths = NULL;
      m_lol_preview_size = 0;
      m_bHaveLOLPreview = false;
      m_bIsMe = false;
      m_bIsPreview = false;
      m_bPageBreak = false;
      m_subjectwidth = 0;
      memset(&m_tm_posttime,0,sizeof(tm));
      m_lightningbolt = false;
   }
   virtual ~ChattyPost();

   void ClearChildren();

   unsigned int GetId(){return m_id;}
   void SetId(unsigned int newid){m_id = newid;}

   bool IsPreview(){return m_bIsPreview;}

   bool IsPageBreak(){return m_bPageBreak;}

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

   void ReadRootChattyFromHTML(tree<htmlcxx::HTML::Node>::sibling_iterator &root_it, 
                               CLampDoc *pDoc,
                               unsigned int id);

   void ReadPostPreviewChattyFromHTML(tree<htmlcxx::HTML::Node>::sibling_iterator &post_it, 
                                      CLampDoc *pDoc,
                                      unsigned int id);

   void ReadKnownPostChattyFromHTML(tree<htmlcxx::HTML::Node>::sibling_iterator &root_it, unsigned int id);

   void ReadSearchResultFromHTML(tree<htmlcxx::HTML::Node>::sibling_iterator &result_it, CLampDoc *pDoc);

   void Read(CXMLElement *pElement, CLampDoc *pDoc, bool bDoingNewFlags);
   void ReadSearchResult(CXMLElement *pElement, CLampDoc *pDoc);
   void ReadShackMessage(CXMLElement *pElement, CLampDoc *pDoc, bool bIsInbox);

   void ReadMessageFromHTML(tree<htmlcxx::HTML::Node>::sibling_iterator &it, CLampDoc *pDoc, bool bIsInbox, size_t id, std::string &author, bool bHaveRead);

   void ReadPost(ChattyPost *pOther, CLampDoc *pDoc);

   void ReadLOL(CLampDoc *pDoc,
                UCString &id,
                UCString &lolcount,
                UCString &author,
                UCString &body);
   int DrawRoot(HDC hDC, RECT &DeviceRectangle, int pos, std::vector<CHotSpot> &hotspots, unsigned int current_id, bool bLinkOnly, bool bAllowModTools, bool bModToolIsUp, RECT &ModToolRect, unsigned int ModToolPostID);
   int DrawReply(HDC hDC, RECT &DeviceRectangle, int pos, std::vector<CHotSpot> &hotspots, int indent, unsigned int current_id, int &trunkatingposts, const UCString &rootauthor, bool bAllowModTools, bool bModToolIsUp, RECT &ModToolRect, unsigned int ModToolPostID);
   int DrawMessage(HDC hDC, RECT &DeviceRectangle, int pos, std::vector<CHotSpot> &hotspots, unsigned int current_id);
   void DrawTextOnly(HDC hDC, RECT &DeviceRectangle, int pos);
   void SetupPreviewShades(bool bDoingNewFlags);

   ChattyPost *FindChild(unsigned int id);

   void UpdatePreviewsToKnown();

   void ReadFromKnown(CLampDoc *pDoc);

   void GetTitle(UCString &title);

   postcategorytype GetCategory(){return m_category;}
   void SetCategory(postcategorytype category){m_category = category;}

   void Despoil();
   void ClearSpoilerTags(int x, int y);
   void GetLink(int x, int y, UCString &link);
   void GetImageLink(int x, int y, UCString &link);
   void GetThumbLink(int x, int y, UCString &link);
   void GetLinkToImage(int x, int y, UCString &link);
   void GetLinkToThumb(int x, int y, UCString &link);
   void MakeLinkIntoImage(int x, int y, unsigned int &index);
   void MakeThumbIntoImage(int x, int y, unsigned int &index);
   void MakeImageIntoLink(int x, int y);
   void MakeImageIntoThumb(int x, int y);
   

   int GetCharPos(int x, int y, bool &off_end);
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

   void AddLolTag(loltagtype tag){m_mylols |= tag; UpdateLOLs();}

   void UpdateLOLs();
   void UpdateLOLsRecurse();

   void LoadAllImageLinks();
   void CloseAllImageLinks();
   void UnloadAllImagesRecurse();
   void UnloadImageRecurse(unsigned int image_index);

   bool IsFiltered();

   bool HasMissingChildren()
   {
      if(m_familysize > 0 &&
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

   void SetBodyText(const UCString &text){m_bodytext = text;}

   ChattyPost *GetRoot();

   void RecordNewness(std::map<unsigned int,newness> &post_newness);
   void EstablishNewness(std::map<unsigned int,newness> &post_newness);

   void RecordTags(std::map<unsigned int,std::vector<shacktagpos>> &post_tags);
   void EstablishTags(std::map<unsigned int,std::vector<shacktagpos>> &post_tags);

   void SetAsPageBreak(size_t page);

   void InvalidateContentLayout(){m_lasttextrectwidth = 0;}

   bool IsNWSPost();

protected:
   void SetupCharWidths();
   void SetupBodyText(RECT &textrect);
   void InitImageLinks();
   void DecodeString(UCString &from, UCString &to, std::vector<shacktagpos> &shacktags);
   void GatherIds(std::list<unsigned int> &ids);
   void RemoveSomeTags(UCString &str);
   void AddToFamilySize(size_t &familysize);
   void UpdateDate();

   unsigned int GetLOLWidth(const UCChar *text);

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
   bool                    m_bIsMe;

   bool                    m_bRefreshing;

   bool                    m_bPinned;

   COLORREF                m_ago_color;

   std::vector<unsigned int> m_root_reply_list;

   bool                    m_bHaveRead;
   bool                    m_bIsInbox;

   CLOLFlags               m_lolflags;

   UCString                m_lol_text;
   UCString                m_inf_text;
   UCString                m_unf_text;
   UCString                m_tag_text;
   UCString                m_wtf_text;

   unsigned int            m_lol_width;
   unsigned int            m_inf_width;
   unsigned int            m_unf_width;
   unsigned int            m_tag_width;
   unsigned int            m_wtf_width;

   UCString                m_lol_preview_text;
   int                    *m_plol_preview_charwidths;
   std::vector<shacktagpos> m_lol_preview_shacktags;
   int                     m_lol_preview_size;
   bool                    m_bHaveLOLPreview;

   bool                    m_bIsPreview;

   tm                      m_tm_posttime;

   bool                    m_bPageBreak;

   int                     m_subjectwidth;

   bool                    m_lightningbolt;
};
