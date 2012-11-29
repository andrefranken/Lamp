#pragma once
#include "DCSurface.h"
#include "Lamp.h"
#include "LampDoc.h"
#include "LampView.h"

class CCharRange
{
public:
   int begin;
   int end;
};

// CReplyDlg dialog

class CReplyDlg
{
public:
	CReplyDlg(CLampView *pView);
	virtual ~CReplyDlg();

   bool IsMessage(){return m_bIsMessage;}
   void SetIsMessage(bool value){m_bIsMessage = value;}

   UCString &GetMessageTo(){return message_to;}
   void SetMessageTo(const UCString &value){message_to = value; m_message_info_dirty = true;}

   UCString &GetMessageSubject(){return message_subject;}
   void SetMessageSubject(const UCString &value){message_subject = value; m_message_info_dirty = true;}

   UCString &GetReplyText(){return m_replytext;}
   void SetReplyText(const UCString &value){m_replytext = value;RecalcCharWidths();RecalcLines();}
   unsigned int GetReplyToId(){return m_replytoid;}

   void Draw(HDC hDC, RECT DeviceRectangle, std::vector<CHotSpot> &hotspots, CPoint &mousepoint);
   void DrawScrollbar(HDC hDC, const RECT &ScrollRectangle, std::vector<CHotSpot> &hotspots, CPoint &mousepoint);
   void DrawThumb(HDC hDC, const RECT &thumbrect, scrollbitype type);

   bool OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

   bool OnLButtonDown(UINT nFlags, CPoint point, bool &bCloseReplyDlg);
   bool OnLButtonUp(UINT nFlags, CPoint point);
   bool OnLButtonDblClk(UINT nFlags, CPoint point);
   bool OnRButtonDown(UINT nFlags, CPoint point);
   bool OnMouseMove(UINT nFlags, CPoint point);
   void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags, bool &bCloseReplyDlg);
   void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

   void SelectAll(){m_caretanchor = 0;m_caretpos = m_replytext.Length();}
   void SetDoc(CLampDoc *pDoc){m_pDoc = pDoc;}
   CLampDoc *GetDoc(){return m_pDoc;}
   void SetReplyId(unsigned int replytoid){m_replytoid = replytoid;}
   unsigned int GetReplyId(){return m_replytoid;}

   void SetDlgRect(RECT &dlgrect){m_replydlgrect = dlgrect;}
   RECT &GetDlgRect(){return m_replydlgrect;}

   int GetHeight(){return m_height;}

   void InsertChar(UCChar thechar);
   void InsertString(const UCString &text);
   void DeleteSelected(bool bRecordUndo = true);

   bool IsPointInTextDrawRect(int x, int y)
   {
      if(x >= m_textdrawrect.left &&
         x < m_textdrawrect.right &&
         y >= m_textdrawrect.top &&
         y < m_textdrawrect.bottom)
      {
         return true;
      }
      return false;
   }

   bool IsPointInScrollRect(int x, int y)
   {
      if(x >= m_ScrollRectangle.left &&
         x < m_ScrollRectangle.right &&
         y >= m_ScrollRectangle.top &&
         y < m_ScrollRectangle.bottom)
      {
         return true;
      }
      return false;
   }

   bool GetHasFocus(){return m_bHaveFocus;}
   void SetHasFocus(bool value){m_bHaveFocus = value;}

   int GetCharPos(int x, int y);
   void GetCharPosesForWord(int x, int y, int &selectionstart, int &selectionend);
   void GetCharPoint(int pos, int &x, int &y);

   void GetTextSelectionRects(int selectionstart, int selectionend, std::vector<RECT> &selectionrects);

   bool HasSelection(){if(m_caretpos != m_caretanchor)return true;return false;}

   void GetSelectedText(UCString &selectedtext)
   {
      int min = __min(m_caretpos, m_caretanchor);
      int max = __max(m_caretpos, m_caretanchor);
      const UCChar *work = m_replytext.Str() + min;
      const UCChar *end = m_replytext.Str() + max;
      while(work < end)
      {
         selectedtext += *work;
         work++;
      }
   }

   void Enclose(const UCChar *frontbit,const UCChar *backbit, bool bTighten = true);
   
   RECT GetThumbRect(){return m_thumbrect;}
   RECT GetUpRect(){return m_uprect;}
   RECT GetDownRect(){return m_downrect;}
   bool GetLButtonDownOnScrollArrow(){return m_bLButtonDownOnScrollArrow;}
   void SetLButtonDownOnScrollArrow(bool value){m_bLButtonDownOnScrollArrow = value;}
   void OnTimer(UINT nIDEvent);

   bool CanUndo()
   {
      if(wcscmp(m_replytext.Str(),m_undobuffer.Str()) == 0)
      {
         return false;
      }
      return true;
   }

   void Undo()
   {
      UCString tempbuffer = m_replytext;
      int temppos = m_caretpos;
      int tempanchor = m_caretanchor;

      m_replytext = m_undobuffer;
      m_caretpos = m_undocaretpos;
      m_caretanchor = m_undocaretanchor;

      m_undobuffer = tempbuffer;
      m_undocaretpos = temppos;
      m_undocaretanchor = tempanchor;

      m_bLastEventWasInsertChar = false;
      m_bLastEventWasDeletion = false;

      RecalcCharWidths();
      RecalcLines();
      MakeCaretVisible();
   }

   void InvalidateSkin();

   bool AreSuggestionsUp(){return m_bSuggestionsUp;}

   bool LastCharWasTab(){return m_bLastCharWasTab;}

   void TweakHotspots(std::vector<CHotSpot> &hotspots);

   void PromptForMessageInfo();

   void DrawShackTag(HDC hDC, shacktag which, RECT &rect, bool bHover);

   void SetupShackTags();

   RECT m_replydlgrect;

   RECT m_textrect;
   RECT m_textdrawrect;
   RECT m_textsizerect;
   RECT m_ScrollRectangle;

protected:
   void RecalcCharWidths();
   void RecalcLines();

   void PosToLinePos(int pos, int &line, int&linepos);

   void MakePosLegal();
   void MakeCaretVisible();
   void CannotCollectUndo()
   {
      m_bLastEventWasInsertChar = false;
      m_bLastEventWasDeletion = false;
   }

   void CheckSpellingAndTags();

   void SetupMessageInfo();
      
   CLampDoc *m_pDoc;
   unsigned int m_replytoid;
   
   int m_pos;
   int m_gotopos;

   int m_lasttextdrawwidth;

   HFONT  m_replyfont;

   int m_caretpos;
   int m_caretanchor;

   bool m_bHaveFocus;

   bool m_bLButtonDown;
   bool m_bSelectingText;
   DWORD m_doubleclicktime;

   UCString m_replytext;
   int *m_pCharWidths;
   std::vector<shacktagpos> m_shacktags;

   std::vector<const UCChar*> m_lines_of_text;
   std::vector<const int*> m_charsizes;
   std::vector<const int>  m_linesizes;
   std::vector<std::vector<shacktagpos>> m_linetags;
   std::vector<linetype>   m_linetypes;

   bool m_bdrawthumb;
   RECT m_thumbrect;
   RECT m_uprect;
   RECT m_downrect;
   RECT m_uptrackrect;
   RECT m_downtrackrect;

   HBRUSH m_backgroundbrush;
   HBRUSH m_textbackgroundbrush;

   bool m_bLButtonDownOnScrollArrow;

   bool m_bTrackingThumb;
   CPoint m_thumbdownpoint;
   int m_thumbdownpos;
   float m_scrollscale;

   bool m_bLastEventWasInsertChar;
   bool m_bLastEventWasDeletion;
   UCString m_undobuffer;
   int m_undocaretpos;
   int m_undocaretanchor;

   int m_height;

   bool m_bDoubleClickDragging;
   int m_doubleclickselectionstart;
   int m_doubleclickselectionend;

   std::vector<CCharRange> m_badwords;

   CLampView *m_pView;

   UINT m_timer;

   bool m_bSuggestionsUp;
   std::vector<CSuggestion> m_suggestions;
   CCharRange m_suggestion_who;
   int m_widest_suggestion;
   RECT m_suggestionsrect;

   bool m_bPreviewMode;

   bool m_bLastCharWasTab;

   ChattyPost m_previewhost;

   bool m_bIsMessage;

   UCString message_to;
   UCString message_subject;

   UCString                 m_message_info_text;
   int                     *m_pmessage_info_charwidths;
   std::vector<shacktagpos> m_message_info_shacktags;
   bool                     m_message_info_dirty;

   bool  m_bComplexShapeText;

   int m_tagheight;
   int m_tagwidth;
   /*
   UCString m_redtext;
   UCString m_greentext;
   UCString m_bluetext;
   UCString m_yellowtext;
   UCString m_olivetext;
   UCString m_limetext;
   UCString m_orangetext;
   UCString m_pinktext;
   UCString m_italicstext;
   UCString m_boldtext;
   UCString m_quotetext;
   UCString m_sampletext;
   UCString m_underlinetext;
   UCString m_striketext;
   UCString m_spoilertext;
   UCString m_codetext;
   */
   ChattyPost m_red_tag;
   ChattyPost m_green_tag;
   ChattyPost m_blue_tag;
   ChattyPost m_yellow_tag;
   ChattyPost m_olive_tag;
   ChattyPost m_lime_tag;
   ChattyPost m_orange_tag;
   ChattyPost m_pink_tag;
   ChattyPost m_italics_tag;
   ChattyPost m_bold_tag;
   ChattyPost m_quote_tag;
   ChattyPost m_sample_tag;
   ChattyPost m_underline_tag;
   ChattyPost m_strike_tag;
   ChattyPost m_spoiler_tag;
   ChattyPost m_code_tag;

   bool m_bHaveSetupTags;
};
