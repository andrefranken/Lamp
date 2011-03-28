// ReplyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ReplyDlg.h"


// CReplyDlg dialog

CReplyDlg::CReplyDlg(CLampView *pView)
{
   m_bLastCharWasTab = false;
   m_bHaveFocus = true;
   m_bLButtonDown = false;
   m_bTrackingThumb = false;
   m_bSelectingText = false;
   m_bdrawthumb = false;
   m_bLButtonDownOnScrollArrow = false;
   m_pos = 0;
   m_gotopos = 0;
   m_caretpos = 0;
   m_caretanchor = 0;
   m_pCharWidths = NULL;
   m_lasttextdrawwidth = 0;
   m_bLButtonDownOnScrollArrow = false;
   m_replyfont = ::CreateFontW(theApp.GetFontHeight(),0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_DONTCARE,theApp.GetNormalFontName());
   m_pDoc = NULL;
   m_replytoid = 0;
   m_textsizerect.top = 0;
   m_textsizerect.bottom = 0;
   m_thumbdownpos = 0;
   m_scrollscale = 1.0f;
   m_bLastEventWasInsertChar = false;
   m_bLastEventWasDeletion = false;
   m_undocaretpos = 0;
   m_undocaretanchor = 0;
   m_height = 279 + theApp.GetCellHeight();
   m_pView = pView;
   m_timer = 0;
   m_bSuggestionsUp = false;
   m_widest_suggestion = 0;
   m_bPreviewMode = false;

   if(theApp.GetPostBackground()->GetBitmap() != NULL)
   {
      m_backgroundbrush = ::CreatePatternBrush(theApp.GetPostBackground()->GetBitmap());
   }
   else
   {
      m_backgroundbrush = ::CreateSolidBrush(theApp.GetPostBackgroundColor());
   }

   if(theApp.GetReplyBackground()->GetBitmap() != NULL)
   {
      m_textbackgroundbrush = ::CreatePatternBrush(theApp.GetReplyBackground()->GetBitmap());
   }
   else
   {
      m_textbackgroundbrush = ::CreateSolidBrush(theApp.GetEditBackgroundColor());
   }
}

CReplyDlg::~CReplyDlg()
{
   if(m_backgroundbrush != NULL)
   {
      ::DeleteObject(m_backgroundbrush);
   }

   if(m_textbackgroundbrush != NULL)
   {
      ::DeleteObject(m_textbackgroundbrush);
   }

   if(m_pCharWidths != NULL)
   {
      free(m_pCharWidths);
      m_pCharWidths = NULL;
   }

   if(m_replyfont != NULL)
   {
      ::DeleteObject(m_replyfont);
   }
}

void CReplyDlg::Draw(HDC hDC, RECT DeviceRectangle, std::vector<CHotSpot> &hotspots, CPoint &mousepoint)
{
   // remove all hotspots that cannot be executed while the reply dialog is up
   std::vector<CHotSpot> newhotspots;
   for(size_t i = 0; i < hotspots.size(); i++)
   {
      switch(hotspots[i].m_type)
      {
      case HST_SCROLLBAR:
      case HST_SCROLLBAR_UP:
      case HST_SCROLLBAR_DOWN:
      case HST_SCROLLBAR_THUMB:
      case HST_REPLYPREVIEW:
      case HST_SPOILER:
      case HST_TEXT:
      case HST_LINK:
      case HST_IMAGELINK:
         newhotspots.push_back(hotspots[i]);
         break;
      }
   }
   hotspots = newhotspots;

   if(m_pos != m_gotopos)
   {
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

   if(m_replydlgrect.bottom < DeviceRectangle.top ||
      m_replydlgrect.top > DeviceRectangle.bottom)
   {
      // do nothing, we are not on the screen
      m_textdrawrect.left = m_textdrawrect.top = m_textdrawrect.right = m_textdrawrect.bottom = -1;
      m_replydlgrect.left = m_replydlgrect.top = m_replydlgrect.right = m_replydlgrect.bottom = -1;
   }
   else
   {
      HBRUSH oldbrush = (HBRUSH)::SelectObject(hDC,m_backgroundbrush);
      HPEN newpen = ::CreatePen(PS_SOLID,0,theApp.GetPostEdgeColor());
      HPEN oldpen = (HPEN)::SelectObject(hDC,newpen);

      if(theApp.RoundedPosts())
      {
         RECT temprect = m_replydlgrect;
         temprect.bottom = temprect.top + 20;
         temprect.right = temprect.left + 20;
         m_pDoc->FillBackground(hDC,temprect);

         temprect.right = m_replydlgrect.right;
         temprect.left = m_replydlgrect.right - 20;
         m_pDoc->FillBackground(hDC,temprect);

         temprect = m_replydlgrect;
         temprect.top = temprect.bottom - 20;
         temprect.right = temprect.left + 20;
         m_pDoc->FillBackground(hDC,temprect);

         temprect.right = m_replydlgrect.right;
         temprect.left = m_replydlgrect.right - 20;
         m_pDoc->FillBackground(hDC,temprect);

         ::RoundRect(hDC,m_replydlgrect.left, m_replydlgrect.top-1, m_replydlgrect.right, m_replydlgrect.bottom, 20, 20);
         if(m_replytoid != 0)
         {
            HPEN newpen = ::CreatePen(PS_SOLID,0,theApp.GetPostNWSEdgeColor());
            HPEN oldpen = (HPEN)::SelectObject(hDC,newpen);
            // draw with a y-1 so that we overdraw teh post's bottom line with this one
            ::MoveToEx(hDC,m_replydlgrect.left + 10,m_replydlgrect.top-1,NULL);
            ::LineTo(hDC,m_replydlgrect.right - 10,m_replydlgrect.top-1);
            ::SelectObject(hDC,oldpen);
            ::DeleteObject(newpen);
         }
      }
      else
      {
         ::Rectangle(hDC,m_replydlgrect.left, m_replydlgrect.top-1, m_replydlgrect.right, m_replydlgrect.bottom);
         if(m_replytoid != 0)
         {
            HPEN newpen = ::CreatePen(PS_SOLID,0,theApp.GetPostNWSEdgeColor());
            HPEN oldpen = (HPEN)::SelectObject(hDC,newpen);
            // draw with a y-1 so that we overdraw teh post's bottom line with this one
            ::MoveToEx(hDC,m_replydlgrect.left,m_replydlgrect.top-1,NULL);
            ::LineTo(hDC,m_replydlgrect.right,m_replydlgrect.top-1);
            ::SelectObject(hDC,oldpen);
            ::DeleteObject(newpen);
         }
      }
      ::SelectObject(hDC,oldbrush);

      CHotSpot hotspot;
      hotspot.m_bAnim = false;
      hotspot.m_type = HST_CLOSEREPLYDLG;
      hotspot.m_spot = m_replydlgrect;
      hotspot.m_spot.left = hotspot.m_spot.right - 20;
      hotspot.m_spot.bottom = hotspot.m_spot.top + 20;
      hotspot.m_id = m_replytoid;
      hotspots.push_back(hotspot);

      // from reply top to text top = 40

      // text height = 209

      // from text bottom to reply bottom = 50

      RECT tagsrect;
      tagsrect.left = m_replydlgrect.right - (135 + 20);
      tagsrect.right = m_replydlgrect.right - 20;
      tagsrect.top = (m_replydlgrect.top - 20) + theApp.GetCellHeight();
      tagsrect.bottom = tagsrect.top + 249;

      RECT restrect;
      restrect = m_replydlgrect;
      restrect.right = tagsrect.left;

      bool bDrawTags = true;

      if(restrect.right - restrect.left < 200)
      {
         bDrawTags = false;
         restrect.right = m_replydlgrect.right;
      }

      if(bDrawTags)
      {
         theApp.GetTagsImage(false)->Blit(hDC,tagsrect);

         if(!m_bPreviewMode)
         {
            RECT tagrect;
            tagrect.left = tagsrect.left;
            tagrect.right = tagrect.left + 68;
            tagrect.top = tagsrect.top + 40;
            tagrect.bottom = tagrect.top + 27;
            
            hotspot.m_id = 0;

            hotspot.m_type = HST_TAG_RED;
            hotspot.m_spot = tagrect;
            hotspots.push_back(hotspot);

            tagrect.top += 26;
            tagrect.bottom += 26;
            hotspot.m_type = HST_TAG_GREEN;
            hotspot.m_spot = tagrect;
            hotspots.push_back(hotspot);
                     
            tagrect.top += 26;
            tagrect.bottom += 26;
            hotspot.m_type = HST_TAG_BLUE;
            hotspot.m_spot = tagrect;
            hotspots.push_back(hotspot);

            tagrect.top += 26;
            tagrect.bottom += 26;
            hotspot.m_type = HST_TAG_YELLOW;
            hotspot.m_spot = tagrect;
            hotspots.push_back(hotspot);

            tagrect.top += 26;
            tagrect.bottom += 26;
            hotspot.m_type = HST_TAG_OLIVE;
            hotspot.m_spot = tagrect;
            hotspots.push_back(hotspot);

            tagrect.top += 26;
            tagrect.bottom += 26;
            hotspot.m_type = HST_TAG_LIME;
            hotspot.m_spot = tagrect;
            hotspots.push_back(hotspot);

            tagrect.top += 26;
            tagrect.bottom += 26;
            hotspot.m_type = HST_TAG_ORANGE;
            hotspot.m_spot = tagrect;
            hotspots.push_back(hotspot);

            tagrect.top += 26;
            tagrect.bottom += 26;
            hotspot.m_type = HST_TAG_PINK;
            hotspot.m_spot = tagrect;
            hotspots.push_back(hotspot);

            tagrect.left = tagrect.right;
            tagrect.right = tagrect.left + 68;
            tagrect.top = tagsrect.top + 40;
            tagrect.bottom = tagrect.top + 27;
            hotspot.m_type = HST_TAG_ITALICS;
            hotspot.m_spot = tagrect;
            hotspots.push_back(hotspot);

            tagrect.top += 26;
            tagrect.bottom += 26;
            hotspot.m_type = HST_TAG_BOLD;
            hotspot.m_spot = tagrect;
            hotspots.push_back(hotspot);

            tagrect.top += 26;
            tagrect.bottom += 26;
            hotspot.m_type = HST_TAG_QUOTE;
            hotspot.m_spot = tagrect;
            hotspots.push_back(hotspot);

            tagrect.top += 26;
            tagrect.bottom += 26;
            hotspot.m_type = HST_TAG_SAMPLE;
            hotspot.m_spot = tagrect;
            hotspots.push_back(hotspot);

            tagrect.top += 26;
            tagrect.bottom += 26;
            hotspot.m_type = HST_TAG_UNDERLINE;
            hotspot.m_spot = tagrect;
            hotspots.push_back(hotspot);

            tagrect.top += 26;
            tagrect.bottom += 26;
            hotspot.m_type = HST_TAG_STRIKE;
            hotspot.m_spot = tagrect;
            hotspots.push_back(hotspot);

            tagrect.top += 26;
            tagrect.bottom += 26;
            hotspot.m_type = HST_TAG_SPOILER;
            hotspot.m_spot = tagrect;
            hotspots.push_back(hotspot);

            tagrect.top += 26;
            tagrect.bottom += 26;
            hotspot.m_type = HST_TAG_CODE;
            hotspot.m_spot = tagrect;
            hotspots.push_back(hotspot);
         }
         
         RECT previewrect;
         previewrect.left = ((tagsrect.right + tagsrect.left) / 2) - (theApp.GetPreviewImage(false)->GetWidth() / 2);
         previewrect.right = previewrect.left + theApp.GetPreviewImage(false)->GetWidth();
         previewrect.top = tagsrect.bottom + 15;
         previewrect.bottom = previewrect.top + theApp.GetPreviewImage(false)->GetHeight();
         if(m_bPreviewMode)
         {
            hotspot.m_type = HST_UNPREVIEW;
         }
         else
         {
            hotspot.m_type = HST_PREVIEW;
         }
         hotspot.m_spot = previewrect;
         hotspots.push_back(hotspot);
      }

      hotspot.m_type = HST_POST;
      hotspot.m_spot.left = ((restrect.right + restrect.left) / 2) - 75;
      hotspot.m_spot.right = hotspot.m_spot.left + 150;
      hotspot.m_spot.top = restrect.bottom - 40 - 1;
      hotspot.m_spot.bottom = hotspot.m_spot.top + 30;
      hotspot.m_id = m_replytoid;
      hotspots.push_back(hotspot);

      hotspot.m_type = HST_POSTAS;
      hotspot.m_spot.left = restrect.left + 20;
      hotspot.m_spot.top = restrect.top + 12;
      hotspot.m_spot.bottom = hotspot.m_spot.top + theApp.GetTextHeight();
      hotspot.m_id = 0;

      HFONT newfont = ::CreateFontW(theApp.GetMiscFontHeight(),0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_DONTCARE,theApp.GetNormalFontName());
      HFONT oldfont = (HFONT)::SelectObject(hDC,newfont);
      ::SetTextColor(hDC,theApp.GetMiscPostTextColor());
      ::ExtTextOutW(hDC, restrect.left + 20, hotspot.m_spot.bottom, 0, NULL, L"As:", 3, NULL);
      ::SelectObject(hDC,oldfont);
      ::DeleteObject(newfont);

      newfont = ::CreateFontW(theApp.GetFontHeight(),0,0,0,FW_EXTRABOLD,0,0,0,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_DONTCARE,theApp.GetNormalFontName());
      oldfont = (HFONT)::SelectObject(hDC,newfont);
      ::SetTextColor(hDC,theApp.GetMyPostColor());
      ::ExtTextOutW(hDC, restrect.left + 20 + theApp.GetCellHeight(), hotspot.m_spot.bottom, 0, NULL, theApp.GetUsername(), theApp.GetUsername().Length(), NULL);
      SIZE textsize;
      ::GetTextExtentPoint32W(hDC,theApp.GetUsername(),theApp.GetUsername().Length(),&textsize);
      ::SelectObject(hDC,oldfont);
      ::DeleteObject(newfont);
      
      hotspot.m_spot.right = hotspot.m_spot.left + textsize.cx + theApp.GetCellHeight();
      hotspots.push_back(hotspot);

      // finally, the text rect

      m_textrect.left = restrect.left + 20;
      m_textrect.right = (restrect.right - 20) + 1;
      m_textrect.top = restrect.top + 12 + theApp.GetCellHeight() + 8;
      m_textrect.bottom = (restrect.bottom - 52) + 1;

      POINT pnt;
      ::GetBrushOrgEx(hDC, &pnt);
      ::SetBrushOrgEx(hDC, pnt.x, pnt.y - m_pos, NULL);
      ::FillRect(hDC,&m_textrect,m_textbackgroundbrush);
      ::SetBrushOrgEx(hDC, pnt.x, pnt.y, NULL);

      ::MoveToEx(hDC,m_textrect.left,m_textrect.top,NULL);
      ::LineTo(hDC,m_textrect.left,m_textrect.bottom);
      ::LineTo(hDC,m_textrect.right,m_textrect.bottom);
      ::LineTo(hDC,m_textrect.right,m_textrect.top);
      ::LineTo(hDC,m_textrect.left,m_textrect.top);
      ::SelectObject(hDC,oldpen);
      ::DeleteObject(newpen);

      m_textdrawrect = m_textrect;
      m_textdrawrect.right -= 16;
            
      if(m_textdrawrect.right - m_textdrawrect.left != m_lasttextdrawwidth)
      {
         m_lasttextdrawwidth = m_textdrawrect.right - m_textdrawrect.left;
         RecalcLines();
      }

      if(!m_bPreviewMode)
      {
         hotspot.m_type = HST_REPLYTEXT;
         hotspot.m_spot = m_textdrawrect;
         hotspot.m_id = 0;
         hotspots.push_back(hotspot);
      }

      RECT textsizerect = m_textsizerect;
      textsizerect.top = m_textdrawrect.top - m_pos;
      textsizerect.bottom = (m_textdrawrect.top - m_pos) + (m_textsizerect.bottom - m_textsizerect.top);

      ::IntersectClipRect(hDC,m_textdrawrect.left,m_textdrawrect.top,m_textdrawrect.right,m_textdrawrect.bottom);

      if(m_bPreviewMode)
      {
         m_previewhost.DrawTextOnly(hDC, m_textdrawrect, textsizerect.top);
      }
      else
      {
         std::vector<RECT> filler;
         m_pDoc->DrawBodyText(hDC, 
                              textsizerect, 
                              m_lines_of_text, 
                              m_charsizes, 
                              m_linesizes,
                              m_linetags,
                              m_linetypes,
                              filler,
                              filler,
                              filler,
                              &m_textdrawrect);
      }

      // draw caret and selection

      if(m_bHaveFocus)
      {
         if(m_caretpos != m_caretanchor)
         {
            std::vector<RECT> selectionrects;
            GetTextSelectionRects(m_caretpos, m_caretanchor, selectionrects);

            int oldrop = ::GetROP2(hDC);
            ::SetROP2(hDC,R2_XORPEN);
            HBRUSH selbrush = ::CreateSolidBrush(theApp.GetTextSelectionColor());

            for(size_t i = 0; i < selectionrects.size(); i++)
            {
               RECT thisrect = selectionrects[i];
               thisrect.top -= m_pos;
               thisrect.bottom -= m_pos;
               HRGN hRGN = ::CreateRectRgnIndirect(&thisrect);
               ::FillRgn(hDC, hRGN, selbrush);
               ::DeleteObject(hRGN);
            }         
            ::DeleteObject(selbrush);
            ::SetROP2(hDC,oldrop);
         }

         int x, y;
         GetCharPoint(m_caretpos, x, y);
         y += (theApp.GetTextHeight() / 2);
         y -= m_pos;

         HPEN newpen = ::CreatePen(PS_SOLID,0,theApp.GetMyPostColor());
         HPEN oldpen = (HPEN)::SelectObject(hDC,newpen);
         ::MoveToEx(hDC, x, y,NULL);
         ::LineTo(hDC, x, y - theApp.GetTextHeight());

         ::SelectObject(hDC,oldpen);
         ::DeleteObject(newpen);
      }

      ::ExtSelectClipRgn(hDC,NULL,RGN_COPY);

      m_ScrollRectangle = m_textrect;
      m_ScrollRectangle.left = m_ScrollRectangle.right - 16;
      DrawScrollbar(hDC, m_ScrollRectangle, hotspots, mousepoint);

      if(m_bSuggestionsUp)
      {
         hotspots.clear();
         
         int x,y;
         GetCharPoint(m_suggestion_who.begin,x,y);
         y = y + (theApp.GetTextHeight() / 2);
         y -= m_pos;
                  
         m_suggestionsrect.left = x;
         m_suggestionsrect.top = y;
         m_suggestionsrect.right = m_suggestionsrect.left + m_widest_suggestion + 5 + 5;
         m_suggestionsrect.bottom = m_suggestionsrect.top + (m_suggestions.size() * theApp.GetTextHeight());

         m_pView->GetDocument()->FillBackground(hDC,m_suggestionsrect);
         HPEN newpen = m_pView->GetDocument()->GetRootTopPen();
         HPEN oldpen = (HPEN)::SelectObject(hDC,newpen);
         ::MoveToEx(hDC,m_suggestionsrect.left,m_suggestionsrect.top,NULL);
         ::LineTo(hDC,m_suggestionsrect.left,m_suggestionsrect.bottom);
         ::LineTo(hDC,m_suggestionsrect.right,m_suggestionsrect.bottom);
         ::LineTo(hDC,m_suggestionsrect.right,m_suggestionsrect.top);
         ::LineTo(hDC,m_suggestionsrect.left,m_suggestionsrect.top);
         ::SelectObject(hDC,oldpen);

         x += 5;
         y += theApp.GetTextHeight();

         HFONT newfont = ::CreateFontW(theApp.GetFontHeight(),0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_DONTCARE,theApp.GetNormalFontName());
         HFONT oldfont = (HFONT)::SelectObject(hDC,newfont);
         ::SetTextColor(hDC,theApp.GetPostTextColor());

         RECT thisrect = m_suggestionsrect;
         thisrect.bottom = y;

         for(size_t i = 0; i < m_suggestions.size(); i++)
         {
            ::ExtTextOutW(hDC, x, y, 0, NULL, m_suggestions[i].Str(), m_suggestions[i].Length(), NULL);
            
            hotspot.m_type = HST_SPELL_SUGGESTION;
            hotspot.m_spot = thisrect;
            hotspot.m_id = i;
            hotspots.push_back(hotspot);

            y += theApp.GetTextHeight();
            thisrect.top += theApp.GetTextHeight();
            thisrect.bottom += theApp.GetTextHeight();
         }

         ::SelectObject(hDC,oldfont);
         ::DeleteObject(newfont);
      }
      else if(m_bLButtonDownOnScrollArrow)
      {
         if(mousepoint.x >= m_uprect.left &&
            mousepoint.x < m_uprect.right &&
            mousepoint.y >= m_uprect.top &&
            mousepoint.y < m_uprect.bottom)
         {
            m_gotopos -= 20;
            MakePosLegal();
            m_pView->InvalidateEverything();
         }
         else if(mousepoint.x >= m_downrect.left &&
                 mousepoint.x < m_downrect.right &&
                 mousepoint.y >= m_downrect.top &&
                 mousepoint.y < m_downrect.bottom)
         {
            m_gotopos += 20;
            MakePosLegal();
            m_pView->InvalidateEverything();
         }
      }
   }

   if(m_pos != m_gotopos)
   {
      m_pView->InvalidateEverything();
   }
}

void CReplyDlg::RecalcCharWidths()
{
   if(m_pCharWidths != NULL)
   {
      free(m_pCharWidths);
      m_pCharWidths = NULL;
   }

   if(!m_replytext.IsEmpty())
   {
      int numchars = m_replytext.Length();
      m_pCharWidths = (int*)malloc(sizeof(int) * numchars);
      m_pDoc->GetCharWidths(m_replytext, m_pCharWidths, m_replytext.Length(), false, false, false, theApp.GetNormalFontName());

      for(int i = 0; i < m_replytext.Length(); i++)
      {
         if(m_replytext[i] == L'\n')
         {
            m_pCharWidths[i] = 4;
         }
      }
   }

   // start spelling timer
   if(m_pView != NULL)
   {
      m_badwords.clear();
      m_shacktags.clear();
      if(m_timer != 0)
      {
         m_pView->KillTimer(m_timer);
      }

      if(theApp.IsSpellCheckerEnabled())
      {
         m_timer = m_pView->SetTimer(5,500,NULL); // half a second delay
      }
   }
}

void CReplyDlg::RecalcLines()
{
   m_lines_of_text.clear();
   m_charsizes.clear();
   m_linesizes.clear();
   m_linetags.clear();
   m_linetypes.clear();

   if(!m_replytext.IsEmpty())
   {
      m_textsizerect = m_textdrawrect;
      m_textsizerect.top = 0;
      m_textsizerect.bottom = 0;
      m_pDoc->CalcBodyText(m_textsizerect,m_replytext,m_pCharWidths,m_shacktags,m_replytext.Length(),m_lines_of_text,m_charsizes,m_linesizes,m_linetags,m_linetypes);

      if(m_replytext[m_replytext.Length()-1] == L'\n')
      {
         // add a blank line to the line list
         m_lines_of_text.push_back(m_replytext.Str() + m_replytext.Length());
         m_charsizes.push_back(m_pCharWidths + m_replytext.Length());
         m_linesizes.push_back(0);
         std::vector<shacktagpos> blank;
         m_linetags.push_back(blank);
         m_linetypes.push_back(linetype(true,0,theApp.GetTextHeight()));
         m_textsizerect.bottom += theApp.GetTextHeight();
      }
   }
}

void CReplyDlg::GetTextSelectionRects(int selectionstart, int selectionend, std::vector<RECT> &selectionrects)
{
   int min = (size_t)__max(0,__min(selectionstart, selectionend));
   int max = (size_t)__min(m_replytext.Length(),__max(selectionstart, selectionend));

   RECT rect;
   rect.top = m_textdrawrect.top + 4;
   rect.bottom = m_textdrawrect.top + theApp.GetTextHeight() + 4;
   
   int charswalked = 0;
   for(size_t line = 0; line < m_charsizes.size(); line++)
   {
      if(m_linetypes[line].m_bIsText)
      {
         const int* charsizes = m_charsizes[line];
         int numcharsinline = m_linesizes[line];

         bool bHaveleft = false;

         if(min - charswalked < 0)
         {
            rect.left = m_textdrawrect.left + 5;
            bHaveleft = true;
         }
         else if(min - charswalked < numcharsinline)
         {
            rect.left = m_textdrawrect.left + 5;
            for(int i=0; i < min - charswalked; i++)
            {
               rect.left += charsizes[i];
            }
            bHaveleft = true;
         }
         else
         {
            // skip.  Not there yet
         }

         if(bHaveleft)
         {
            if(max - charswalked >= 0)
            {
               rect.right = m_textdrawrect.left + 5;
               for(int i=0; i < __min(numcharsinline,max - charswalked); i++)
               {
                  rect.right += charsizes[i];
               }
               
               if(rect.right > rect.left)
               {
                  selectionrects.push_back(rect);
               }
            }
            else
            {
               // skip.  It ended earlier.
            }
         }
      }

      charswalked += m_linesizes[line];
      rect.top += m_linetypes[line].m_height;
      rect.bottom += m_linetypes[line].m_height;
   }
}


void CReplyDlg::InsertChar(UCChar thechar)
{
   if(m_bLastEventWasInsertChar)
   {
      // leave the undo buffer alone
   }
   else
   {
      m_undobuffer = m_replytext;
      m_undocaretpos = m_caretpos;
      m_undocaretanchor = m_caretanchor;
   }
   
   if(m_caretanchor != m_caretpos)
   {
      DeleteSelected(false);
   }
   m_replytext.InsertChar(thechar,m_caretpos);
   m_caretpos++;
   m_caretanchor = m_caretpos;
   RecalcCharWidths();
   RecalcLines();
   MakeCaretVisible();

   m_bLastEventWasInsertChar = true;
   m_bLastEventWasDeletion = false;
}

void CReplyDlg::InsertString(const UCString &text)
{
   m_undobuffer = m_replytext;
   m_undocaretpos = m_caretpos;
   m_undocaretanchor = m_caretanchor;
   
   if(m_caretanchor != m_caretpos)
   {
      DeleteSelected(false);
   }
   const UCChar *work = text.Str();
   const UCChar *end = text.Str() + text.Length();
   while(work < end)
   {
      m_replytext.InsertChar(*work,m_caretpos);
      m_caretpos++;
      work++;
   }
   m_caretanchor = m_caretpos;
   RecalcCharWidths();
   RecalcLines();
   MakeCaretVisible();

   m_bLastEventWasInsertChar = false;
   m_bLastEventWasDeletion = false;
}

void CReplyDlg::DeleteSelected(bool bRecordUndo/* = true*/)
{
   if(bRecordUndo)
   {
      if(m_bLastEventWasDeletion)
      {
         // leave the undo buffer alone
      }
      else
      {
         m_undobuffer = m_replytext;
         m_undocaretpos = m_caretpos;
         m_undocaretanchor = m_caretanchor;
      }
      m_bLastEventWasInsertChar = false;
      m_bLastEventWasDeletion = true;
   }

   int mincaretpos = __min(m_caretpos,m_caretanchor);
   int maxcaretpos = __max(m_caretpos,m_caretanchor);

   for(int position = mincaretpos; position < maxcaretpos; position++)
   {
      m_replytext.DeleteChar(mincaretpos);
   }

   m_caretpos = m_caretanchor = mincaretpos;
   if(bRecordUndo)
   {
      RecalcCharWidths();
      RecalcLines();
      MakeCaretVisible();
   }
}

void CReplyDlg::PosToLinePos(int pos, int &line, int&linepos)
{
   if(m_linesizes.size() > 0)
   {
      int charswalked = 0;
      // loop for every line
      for(size_t i=0; i < m_linesizes.size(); i++)
      {
         if(pos >= charswalked &&
           ((i != m_linesizes.size() - 1 &&
            pos < (charswalked + m_linesizes[i])) ||
            (i == m_linesizes.size() - 1 &&
            (pos <= (charswalked + m_linesizes[i])))))
         {
            line = i;
            linepos = pos - charswalked;
            break;
         }
         charswalked += m_linesizes[i];
      }
   }
}

int CReplyDlg::GetCharPos(int x, int y)
{
   int charpos = 0;

   // find the charpos
   x -= m_textdrawrect.left;
   y -= m_textdrawrect.top;
   x -= 5;
   y -= 4;
   size_t line =0;
   while(line < m_linetypes.size() &&
         y > (int)m_linetypes[line].m_height)
   {
      y -= m_linetypes[line].m_height;

      if(line < m_linesizes.size())
      {
         charpos += m_linesizes[line];
      }

      line++;
   }

   int startcharpos = charpos;

   if(line < m_charsizes.size())
   {
      const UCChar* lineoftext = m_lines_of_text[line];
      const int* charsizes = m_charsizes[line];
      const int numcharsinline = m_linesizes[line];
      int length = 0;
      int i = 0;
      for(; i < numcharsinline; i++)
      {
         if(length + charsizes[i] > x)
         {
            if(((length + charsizes[i]) - x) < (x - length))
            {
               charpos++;
            }
            break;
         }   
         else
         {
            length += charsizes[i];
            charpos++;
         }
      }

      if(numcharsinline > 0 && 
         charpos - startcharpos == numcharsinline)
      {
         if(lineoftext[numcharsinline-1] == L' ' ||
            lineoftext[numcharsinline-1] == L'\n')
         {
            charpos--;
         }
      }
   }

   return charpos;
}


void CReplyDlg::GetCharPoint(int pos, int &x, int &y)
{
   x = m_textdrawrect.left + 5;
   y = m_textdrawrect.top + 4 + (theApp.GetTextHeight() / 2);

   int charswalked = 0;
   // loop for every line
   for(size_t i=0; i < m_linesizes.size(); i++)
   {
      if(pos >= charswalked &&
        ((i != m_linesizes.size() - 1 &&
         pos < (charswalked + m_linesizes[i])) ||
         (i == m_linesizes.size() - 1 &&
         (pos <= (charswalked + m_linesizes[i])))))
      {
         const int* pLineWidths = m_charsizes[i];
         const int* pEnd = pLineWidths + (pos - charswalked);
         while(pLineWidths < pEnd)
         {
            x += *pLineWidths;
            pLineWidths++;
         }
         break;
      }
      charswalked += m_linesizes[i];
      y += m_linetypes[i].m_height;
   }
}

bool CReplyDlg::OnLButtonDown(UINT nFlags, CPoint point, bool &bCloseReplyDlg)
{
   bool bITookIt = false;

   m_bHaveFocus = false;

   if(m_bSuggestionsUp)
   {
      bITookIt = true;
      if(point.x >= m_suggestionsrect.left &&
         point.x < m_suggestionsrect.right &&
         point.y >= m_suggestionsrect.top &&
         point.y < m_suggestionsrect.bottom)
      {
         // clicked on a suggestion

         int bottom = m_suggestionsrect.top + theApp.GetTextHeight();

         for(size_t i = 0; i < m_suggestions.size(); i++)
         {
            if(point.y < bottom)
            {
               // this is it
               m_caretanchor = m_suggestion_who.begin;
               m_caretpos = m_suggestion_who.end;

               InsertString(m_suggestions[i]);
               m_bHaveFocus = true;
               break;
            }

            bottom += theApp.GetTextHeight();
         }
      }
      // clear suggestion
      m_bSuggestionsUp = false;
      m_pView->InvalidateEverything();
   }

   if(bITookIt == false &&
      point.x >= m_replydlgrect.left &&
      point.x < m_replydlgrect.right &&
      point.y >= m_replydlgrect.top &&
      point.y < m_replydlgrect.bottom)
   {
      bITookIt = true;
      m_bLButtonDown = true;

      if(!m_bPreviewMode &&
         IsPointInTextDrawRect(point.x,point.y))
      {
         m_bHaveFocus = true;
         m_bSelectingText = true;
         m_pView->SetCapture();
         
         bool bContinue = true;
         if(m_doubleclicktime != 0)
         {
            if(::GetTickCount() - m_doubleclicktime < 1000)
            {
               // triple click mutha fucka!
               int charpos = GetCharPos(point.x, point.y + m_pos);
               charpos = __max(0,__min(m_replytext.Length(),charpos));

               const UCChar *begin = m_replytext;
               const UCChar *here = begin + charpos;
               const UCChar *end = begin + m_replytext.Length();
               const UCChar *work = here;

               // look left
               while(work >= begin && 
                     *work != L'\n')
               {
                  m_caretpos = work - begin;
                  work--;
               }
               
               work = here;

               // look right
               while(work < end && 
                     *work != L'\n')
               {
                  m_caretanchor = work - begin + 1;
                  work++;
               }

               if(work < end && 
                  *work == L'\n')
               {
                  m_caretanchor = work - begin + 1;
               }

               bContinue = false;
               m_bLButtonDown = false;
               m_bSelectingText = false;
               m_pView->InvalidateEverything();
            }
         }

         if(bContinue)
         {
            m_caretpos = GetCharPos(point.x, point.y + m_pos);

            if(!(GetAsyncKeyState(VK_SHIFT) & 0xFF00))
            {
               m_caretanchor = m_caretpos;
            } 

            m_pView->InvalidateEverything();
         }
      }
      else
      {
         std::vector<CHotSpot> &hotspots = m_pView->GetHotspots();

         for(size_t i = 0; i < hotspots.size(); i++)
         {
            if(point.x >= hotspots[i].m_spot.left &&
               point.x < hotspots[i].m_spot.right &&
               point.y >= hotspots[i].m_spot.top &&
               point.y < hotspots[i].m_spot.bottom)
            {
               switch(hotspots[i].m_type)
               {
               case HST_SCROLLBAR_UP_REPLYTEXT_DLG:
                  {
                     m_bLButtonDownOnScrollArrow = true;
                     m_pView->SetCapture();
                     m_gotopos -= 20;
                     MakePosLegal();
                     m_pView->InvalidateEverything();
                  }
                  break;
               case HST_SCROLLBAR_DOWN_REPLYTEXT_DLG:
                  {
                     m_bLButtonDownOnScrollArrow = true;
                     m_pView->SetCapture();
                     m_gotopos += 20;
                     MakePosLegal();
                     m_pView->InvalidateEverything();
                  }
                  break;
               case HST_SCROLLBAR_THUMB_REPLYTEXT_DLG:
                  {
                     m_bTrackingThumb = true;
                     m_thumbdownpoint = point;
                     m_thumbdownpos = m_pos;
                     m_pView->SetCapture();
                  }
                  break;
               case HST_SCROLLBAR_REPLYTEXT_DLG:
                  {               
                     if(point.x >= m_uptrackrect.left &&
                        point.x < m_uptrackrect.right &&
                        point.y >= m_uptrackrect.top &&
                        point.y < m_uptrackrect.bottom)
                     {
                        if(point.y >= (m_uptrackrect.bottom - (m_thumbrect.bottom - m_thumbrect.top)))
                        {
                           // prev page
                           m_gotopos -= ((m_ScrollRectangle.bottom - m_ScrollRectangle.top) - 20);
                           MakePosLegal();
                           m_pView->InvalidateEverything();
                        }
                        else
                        {
                           // goto spot
                           m_gotopos = ((int)((float)((point.y - m_ScrollRectangle.top) - 16) * (1.0f / m_scrollscale))) - ((m_ScrollRectangle.bottom - m_ScrollRectangle.top) >> 1);
                           MakePosLegal();
                           m_pView->InvalidateEverything();
                        }
                     }
                     else if(point.x >= m_downtrackrect.left &&
                             point.x < m_downtrackrect.right &&
                             point.y >= m_downtrackrect.top &&
                             point.y < m_downtrackrect.bottom)
                     {
                        if(point.y <= (m_downtrackrect.top + (m_thumbrect.bottom - m_thumbrect.top)))
                        {
                           // next page
                           m_gotopos += ((m_ScrollRectangle.bottom - m_ScrollRectangle.top) - 20);
                           MakePosLegal();
                           m_pView->InvalidateEverything();
                        }
                        else
                        {
                           // goto spot
                           m_gotopos = ((int)((float)((point.y - m_ScrollRectangle.top) - 16) * (1.0f / m_scrollscale))) - ((m_ScrollRectangle.bottom - m_ScrollRectangle.top) >> 1);
                           MakePosLegal();
                           m_pView->InvalidateEverything();
                        }
                     }
                  }
                  break;
               case HST_CLOSEREPLYDLG:
                  {
                     bCloseReplyDlg = true;
                  }
                  break;
               case HST_POST:
                  {
                     if(m_replytext.Length() < 5)
                     {
                        m_pView->MessageBox(L"Please post something with more than 5 characters.");
                     }
                     else
                     {
                        m_pView->GetDocument()->PostReply(m_replytext, m_replytoid);
                     }
                  }
                  break;
               case HST_PREVIEW:
                  {
                     m_bPreviewMode = true;
                     m_bHaveFocus = false;
                     m_previewhost.SetDoc(m_pView->GetDocument());
                     m_previewhost.DecodeShackTagsString(m_replytext);
                     m_pView->InvalidateEverything();
                  }
                  break;
               case HST_UNPREVIEW:
                  {
                     m_bPreviewMode = false;
                     RecalcCharWidths();
                     RecalcLines();
                     MakeCaretVisible();
                     m_pView->InvalidateEverything();
                  }
                  break;                           
               case HST_POSTAS:
                  {
                     theApp.Login();
                     m_pView->InvalidateEverything();
                  }
                  break;
               case HST_TAG_RED:
                  {
                     Enclose(L"r{",L"}r");
                     SetHasFocus(true);
                     m_pView->InvalidateEverything();
                  }
                  break;
               case HST_TAG_GREEN:
                  {
                     Enclose(L"g{",L"}g");
                     SetHasFocus(true);
                     m_pView->InvalidateEverything();
                  }
                  break;
               case HST_TAG_BLUE:
                  {
                     Enclose(L"b{",L"}b");
                     SetHasFocus(true);
                     m_pView->InvalidateEverything();
                  }
                  break;
               case HST_TAG_YELLOW:
                  {
                     Enclose(L"y{",L"}y");
                     SetHasFocus(true);
                     m_pView->InvalidateEverything();
                  }
                  break;
               case HST_TAG_OLIVE:
                  {
                     Enclose(L"e[",L"]e");
                     SetHasFocus(true);
                     m_pView->InvalidateEverything();
                  }
                  break;
               case HST_TAG_LIME:
                  {
                     Enclose(L"l[",L"]l");
                     SetHasFocus(true);
                     m_pView->InvalidateEverything();
                  }
                  break;
               case HST_TAG_ORANGE:
                  {
                     Enclose(L"n[",L"]n");
                     SetHasFocus(true);
                     m_pView->InvalidateEverything();
                  }
                  break;
               case HST_TAG_PINK:
                  {
                     Enclose(L"p[",L"]p");
                     SetHasFocus(true);
                     m_pView->InvalidateEverything();
                  }
                  break;
               case HST_TAG_ITALICS:
                  {
                     Enclose(L"/[",L"]/");
                     SetHasFocus(true);
                     m_pView->InvalidateEverything();
                  }
                  break;
               case HST_TAG_BOLD:
                  {
                     Enclose(L"b[",L"]b");
                     SetHasFocus(true);
                     m_pView->InvalidateEverything();
                  }
                  break;
               case HST_TAG_QUOTE:
                  {
                     Enclose(L"q[",L"]q");
                     SetHasFocus(true);
                     m_pView->InvalidateEverything();
                  }
                  break;
               case HST_TAG_SAMPLE:
                  {
                     Enclose(L"s[",L"]s");
                     SetHasFocus(true);
                     m_pView->InvalidateEverything();
                  }
                  break;
               case HST_TAG_UNDERLINE:
                  {
                     Enclose(L"_[",L"]_");
                     SetHasFocus(true);
                     m_pView->InvalidateEverything();
                  }
                  break;
               case HST_TAG_STRIKE:
                  {
                     Enclose(L"-[",L"]-");
                     SetHasFocus(true);
                     m_pView->InvalidateEverything();
                  }
                  break;
               case HST_TAG_SPOILER:
                  {
                     Enclose(L"o[",L"]o");
                     SetHasFocus(true);
                     m_pView->InvalidateEverything();
                  }
                  break;
               case HST_TAG_CODE:
                  {
                     Enclose(L"/{{",L"}}/");
                     SetHasFocus(true);
                     m_pView->InvalidateEverything();
                  }
                  break;
               }
            }
         }
      }
   }
   return bITookIt;
}

bool CReplyDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
   bool bITookIt = false;

   if(m_bLButtonDownOnScrollArrow)
   {
      bITookIt = true;
      m_bLButtonDownOnScrollArrow = false;
      ReleaseCapture();
   }
   else if(m_bTrackingThumb)
   {
      bITookIt = true;
      m_bTrackingThumb = false;
      m_gotopos = m_thumbdownpos + (int)((float)(point.y - m_thumbdownpoint.y) * (1.0f / m_scrollscale));
      MakePosLegal();
      m_pView->InvalidateEverything();
      ReleaseCapture();
   }
   else if(m_bSelectingText)
   {
      bITookIt = true;
      m_bSelectingText = false;
      ReleaseCapture();
      m_caretpos = GetCharPos(point.x, point.y + m_pos);
      MakeCaretVisible();
      CannotCollectUndo();
      m_pView->InvalidateEverything();
   } 

   m_bLButtonDown = false;

   return bITookIt;
}

bool CReplyDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
   bool bITookIt = false;

   if(m_bTrackingThumb)
   {
      m_gotopos = m_thumbdownpos + (int)((float)(point.y - m_thumbdownpoint.y) * (1.0f / m_scrollscale));
      MakePosLegal();
      m_pView->InvalidateEverything();
   }
   else if(m_bSelectingText)
   {
      bITookIt = true;
      m_caretpos = GetCharPos(point.x, point.y + m_pos);
      MakeCaretVisible();
      m_pView->InvalidateEverything();
   }

   return bITookIt;
}

bool CReplyDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
   bool bITookIt = false;

   if(!m_bPreviewMode &&
      IsPointInTextDrawRect(point.x,point.y))
   {
      bITookIt = true;

      m_doubleclicktime = 0;

      int charpos = GetCharPos(point.x, point.y + m_pos);
      charpos = __max(0,__min(m_replytext.Length(),charpos));

      const UCChar *begin = m_replytext;
      const UCChar *here = begin + charpos;
      const UCChar *end = begin + m_replytext.Length();
      const UCChar *work = here;

      // look left
      while(work >= begin && 
            iswspace(*work))
      {
         m_caretpos = work - begin;
         work--;
      }
      while(work >= begin && 
            iswalnum(*work))
      {
         m_caretpos = work - begin;
         work--;
      }
      
      work = here;

      // look right
      while(work < end && 
            iswalnum(*work))
      {
         m_caretanchor = work - begin + 1;
         work++;
      }

      if(work < end && 
         *work == L' ')
      {
         m_caretanchor = work - begin + 1;
      }

      m_pView->InvalidateEverything();

      m_doubleclicktime = ::GetTickCount();
   }
   return bITookIt;
}

void CReplyDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
   if(!m_bPreviewMode)
   {
      switch(nChar)
      {
      case 0x41:// 'a'
         {
            if(GetAsyncKeyState(VK_CONTROL) & 0xFF00)
            {
               m_caretanchor = 0;
               m_caretpos = m_replytext.Length();
               MakeCaretVisible();
               CannotCollectUndo();
            }
         }
         break;
      case 0x09:// TAB
         {
            m_bLastCharWasTab = !m_bLastCharWasTab;
         }
         break;
      case VK_DELETE: 
         {
            if(m_caretanchor != m_caretpos)
            {
               DeleteSelected();
            }
            else if(m_caretpos < m_replytext.Length())
            {
               if(GetAsyncKeyState(VK_CONTROL) & 0xFF00)
               {            
                  // Move caret 1 WORD right.
                  if(iswspace(m_replytext[m_caretpos]))
                  {
                     // move up to the first character
                     while(m_caretpos < m_replytext.Length() && iswspace(m_replytext[m_caretpos]))
                     {
                        m_caretpos++;
                     }
                  }
                  else
                  {
                     // move past all the non-whitespace
                     while(m_caretpos < m_replytext.Length() && !iswspace(m_replytext[m_caretpos]))
                     {
                        m_caretpos++;
                     }
                     // move up to the first character
                     while(m_caretpos < m_replytext.Length() && iswspace(m_replytext[m_caretpos]))
                     {
                        m_caretpos++;
                     }
                  }
            
                  //m_caretanchor = m_caretpos + 1;
               }
               else
               {
                  m_caretanchor = m_caretpos + 1;
               }
               DeleteSelected();
            }
         }
         break;
      case VK_RETURN: 
      case VK_SPACE:
         {
            if(m_bLastCharWasTab)
            {
               m_bLastCharWasTab = false;

               if(m_replytext.Length() < 5)
               {
                  m_pView->MessageBox(L"Please post something with more than 5 characters.");
               }
               else
               {
                  m_pView->GetDocument()->PostReply(m_replytext, m_replytoid);
               }
            }
            else
            {
               if(nChar == VK_RETURN)
               {
                  InsertChar(L'\n'); 
               }
            }
         }
         break;
      case VK_ESCAPE: break;
      case VK_BACK:
         {
            if(m_caretanchor != m_caretpos)
            {
               DeleteSelected();
            }
            else if(m_caretpos > 0)
            {
               if(GetAsyncKeyState(VK_CONTROL) & 0xFF00)
               {
                  // Move caret 1 WORD left.
                  // move past any whitespace
                  while(m_caretpos > 0 && iswspace(m_replytext[m_caretpos-1]))
                  {
                     m_caretpos--;
                  }
                  
                  // move past all the non-whitespace
                  while(m_caretpos > 0 && !iswspace(m_replytext[m_caretpos-1]))
                  {
                     m_caretpos--;
                  }
               }
               else
               {
                  m_caretanchor = m_caretpos - 1;
               }
               DeleteSelected();
            }
         }
         break;
      case VK_LEFT: 
         {
            if(GetAsyncKeyState(VK_CONTROL) & 0xFF00)
            {
               // Move caret 1 WORD left.
               // move past any whitespace
               while(m_caretpos > 0 && iswspace(m_replytext[m_caretpos-1]))
               {
                  m_caretpos--;
               }
               
               // move past all the non-whitespace
               while(m_caretpos > 0 && !iswspace(m_replytext[m_caretpos-1]))
               {
                  m_caretpos--;
               }
            }
            else
            {
               m_caretpos--;
               if(m_caretpos < 0)m_caretpos = 0;
            }
            if(!(GetAsyncKeyState(VK_SHIFT) & 0xFF00))
            {
               m_caretanchor = m_caretpos;
            } 
            MakeCaretVisible();
            CannotCollectUndo();
         }
         break;
      case VK_RIGHT:
         {
            if(GetAsyncKeyState(VK_CONTROL))
            {
               // Move caret 1 WORD right.
               if(iswspace(m_replytext[m_caretpos]))
               {
                  // move up to the first character
                  while(m_caretpos < m_replytext.Length() && iswspace(m_replytext[m_caretpos]))
                  {
                     m_caretpos++;
                  }
               }
               else
               {
                  // move past all the non-whitespace
                  while(m_caretpos < m_replytext.Length() && !iswspace(m_replytext[m_caretpos]))
                  {
                     m_caretpos++;
                  }
                  // move up to the first character
                  while(m_caretpos < m_replytext.Length() && iswspace(m_replytext[m_caretpos]))
                  {
                     m_caretpos++;
                  }
               }
            }
            else
            {
               m_caretpos++;
               if(m_caretpos > m_replytext.Length())m_caretpos = m_replytext.Length();
            }
            if(!(GetAsyncKeyState(VK_SHIFT) & 0xFF00))
            {
               m_caretanchor = m_caretpos;
            } 
            MakeCaretVisible();
            CannotCollectUndo();
         }
         break;
      case VK_PRIOR:
      case VK_UP:
         {
            int line, linepos;
            PosToLinePos(m_caretpos, line, linepos);
            if(line > 0)
            {
               int x, y;
               GetCharPoint(m_caretpos, x, y);
               if(nChar == VK_UP)
               {
                  y -= theApp.GetTextHeight();
               }
               else
               {
                  y -= (m_textdrawrect.bottom - m_textdrawrect.top);
               }
               m_caretpos = GetCharPos(x, y);
               if(m_caretpos > m_replytext.Length())m_caretpos = m_replytext.Length();
               if(m_caretpos < 0)m_caretpos = 0;
            
               if(!(GetAsyncKeyState(VK_SHIFT) & 0xFF00))
               {
                  m_caretanchor = m_caretpos;
               } 
            }
            MakeCaretVisible();
            CannotCollectUndo();
         }
         break;
      case VK_DOWN:
      case VK_NEXT:
         {
            int line, linepos;
            PosToLinePos(m_caretpos, line, linepos);
            if(line < (int)m_linesizes.size() - 1)
            {
               int x, y;
               GetCharPoint(m_caretpos, x, y);
               if(nChar == VK_DOWN)
               {
                  y += theApp.GetTextHeight();
               }
               else
               {
                  y += (m_textdrawrect.bottom - m_textdrawrect.top);
               }
               m_caretpos = GetCharPos(x, y);
               if(m_caretpos > m_replytext.Length())m_caretpos = m_replytext.Length();
               if(m_caretpos < 0)m_caretpos = 0;
            
               if(!(GetAsyncKeyState(VK_SHIFT) & 0xFF00))
               {
                  m_caretanchor = m_caretpos;
               } 
            }
            MakeCaretVisible();
            CannotCollectUndo();
         }
         break;
      case VK_HOME:
         {
            if(GetAsyncKeyState(VK_CONTROL) & 0xFF00)
            {
               // Move caret to begining
               m_caretpos = 0;
            }
            else
            {
               if(m_caretpos <= m_replytext.Length())
               {
                  int line, linepos;
                  PosToLinePos(m_caretpos, line, linepos);
                  if(line > 0 &&
                     line < (int)m_linesizes.size())
                  {
                     m_caretpos -= linepos;
                  }
                  else
                  {
                     m_caretpos = 0;
                  }
               }
            }
            if(!(GetAsyncKeyState(VK_SHIFT) & 0xFF00))
            {
               m_caretanchor = m_caretpos;
            } 
            MakeCaretVisible();
            CannotCollectUndo();
         }
         break;
      case VK_END:
         {
            if(GetAsyncKeyState(VK_CONTROL) & 0xFF00)
            {
               // Move caret to end
               m_caretpos = m_replytext.Length();
            }
            else
            {
               if(m_caretpos < m_replytext.Length())
               {
                  int line, linepos;
                  PosToLinePos(m_caretpos, line, linepos);
                  if(line >= 0 &&
                     line < (int)m_linesizes.size())
                  {
                     m_caretpos += (m_linesizes[line] - linepos);
                     if(line < (int)m_linesizes.size() - 1)
                     {
                        m_caretpos--;
                     }
                  }
               }
            }
            if(!(GetAsyncKeyState(VK_SHIFT) & 0xFF00))
            {
               m_caretanchor = m_caretpos;
            } 
            MakeCaretVisible();
            CannotCollectUndo();
         }
         break;
      case VK_ADD: break;
      case VK_SUBTRACT: break;
      }

      if(nChar != 0x09)// TAB
      {
         m_bLastCharWasTab = false;
      }
   }
}

void CReplyDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   if(!m_bPreviewMode &&
      iswprint(nChar) &&
      nChar != 0x09)// don't insert TABs
   {
      InsertChar(nChar);
      m_bLastCharWasTab = false;
   }
}

void CReplyDlg::Enclose(const UCChar *frontbit,const UCChar *backbit)
{
   m_undobuffer = m_replytext;
   m_undocaretpos = m_caretpos;
   m_undocaretanchor = m_caretanchor;
   m_bLastEventWasInsertChar = false;
   m_bLastEventWasDeletion = false;

   int mincaretpos = __min(m_caretpos,m_caretanchor);
   int maxcaretpos = __max(m_caretpos,m_caretanchor);

   // skip any leading whitespace
   while(iswspace(m_replytext[mincaretpos]))
   {
      mincaretpos++;
   }

   // skip any trailing whitespace
   while(maxcaretpos > mincaretpos &&
         iswspace(m_replytext[maxcaretpos - 1]))
   {
      maxcaretpos--;
   }

   const UCChar *work = frontbit;
   const UCChar *end = work + wcslen(frontbit);
   while(work < end)
   {
      m_replytext.InsertChar(*work,mincaretpos);
      mincaretpos++;
      maxcaretpos++;
      work++;
   }

   work = backbit;
   end = work + wcslen(backbit);
   int thispos = maxcaretpos;
   while(work < end)
   {
      m_replytext.InsertChar(*work,thispos);
      thispos++;
      work++;
   }

   m_caretpos = mincaretpos;
   m_caretanchor = maxcaretpos;
   
   RecalcCharWidths();
   RecalcLines();
   MakeCaretVisible();
}

void CReplyDlg::DrawScrollbar(HDC hDC, const RECT &ScrollRectangle, std::vector<CHotSpot> &hotspots, CPoint &mousepoint)
{
   scrollbitype maintype = SBT_INACTIVE;
   scrollbitype thistype;
   if(m_bTrackingThumb ||
      (mousepoint.x >= ScrollRectangle.left &&
       mousepoint.x < ScrollRectangle.right &&
       mousepoint.y >= ScrollRectangle.top &&
       mousepoint.y < ScrollRectangle.bottom))
   {
      maintype = SBT_ACTIVE;
   }

   m_uprect = ScrollRectangle;
   m_uprect.bottom = m_uprect.top + 16;
   thistype = maintype;
   if(thistype == SBT_ACTIVE &&
      mousepoint.x >= m_uprect.left &&
      mousepoint.x < m_uprect.right &&
      mousepoint.y >= m_uprect.top &&
      mousepoint.y < m_uprect.bottom)
   {
      thistype = SBT_HOVER;
   }
   theApp.GetUpImage(thistype)->Blit(hDC,m_uprect);

   CHotSpot hotspot;
   hotspot.m_bAnim = false;
   hotspot.m_type = HST_SCROLLBAR_UP_REPLYTEXT_DLG;
   hotspot.m_spot = m_uprect;
   hotspot.m_id = 0;
   hotspots.push_back(hotspot);

   m_downrect = ScrollRectangle;
   m_downrect.top = m_downrect.bottom - 16;
   thistype = maintype;
   if(thistype == SBT_ACTIVE &&
      mousepoint.x >= m_downrect.left &&
      mousepoint.x < m_downrect.right &&
      mousepoint.y >= m_downrect.top &&
      mousepoint.y < m_downrect.bottom)
   {
      thistype = SBT_HOVER;
   }
   theApp.GetDownImage(thistype)->Blit(hDC,m_downrect);

   hotspot.m_type = HST_SCROLLBAR_DOWN_REPLYTEXT_DLG;
   hotspot.m_spot = m_downrect;
   hotspot.m_id = 0;
   hotspots.push_back(hotspot);

   int scrollbarheight = ScrollRectangle.bottom - ScrollRectangle.top;
   int trackheight = scrollbarheight - 16 - 16;
   int docheight = __max(16,m_textsizerect.bottom - m_textsizerect.top);
   int thumbtop = m_pos;
   int thumbbottom = thumbtop + scrollbarheight;//scrollbarheight is also screenheight

   // now translate those to the range of the scrollbar's pixels
   m_scrollscale =  (float)trackheight / (float)docheight;
   thumbtop = __max(16, 16 + (int)((float)thumbtop * m_scrollscale));
   thumbbottom = __min(scrollbarheight - 16, 16 + (int)((float)thumbbottom * m_scrollscale));
   int thumbheight = thumbbottom - thumbtop;
   
   if(thumbheight < (4 + 4))
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
   if(thumbbottom - thumbtop < trackheight)
   {
      m_bdrawthumb = true;
      m_thumbrect = ScrollRectangle;
      m_thumbrect.top = ScrollRectangle.top + thumbtop;
      m_thumbrect.bottom = ScrollRectangle.top + thumbbottom;

      thistype = maintype;
      if(thistype == SBT_ACTIVE &&
         mousepoint.x >= m_thumbrect.left &&
         mousepoint.x < m_thumbrect.right &&
         mousepoint.y >= m_thumbrect.top &&
         mousepoint.y < m_thumbrect.bottom)
      {
         thistype = SBT_HOVER;
      }

      if(m_bTrackingThumb)
      {
         thistype = SBT_HOVER;
      }

      DrawThumb(hDC, m_thumbrect, thistype);

      hotspot.m_type = HST_SCROLLBAR_THUMB_REPLYTEXT_DLG;
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

   hotspot.m_type = HST_SCROLLBAR_REPLYTEXT_DLG;
   hotspot.m_spot = ScrollRectangle;
   hotspot.m_id = 0;
   hotspots.push_back(hotspot);
}

void CReplyDlg::DrawThumb(HDC hDC, const RECT &thumbrect, scrollbitype type)
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

bool CReplyDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
   bool bITookIt = false;

   if(m_bSuggestionsUp == false &&
      pt.x >= m_textdrawrect.left &&
      pt.x < m_textdrawrect.right &&
      pt.y >= m_textdrawrect.top &&
      pt.y < m_textdrawrect.bottom)
   {
      m_gotopos -= (int)((float)zDelta * theApp.GetMouseWheelScale());
      MakePosLegal();
      if(m_pos != m_gotopos)
      {
         m_pView->InvalidateEverything();
         bITookIt = true;
      }
   }

   return bITookIt;
}

void CReplyDlg::MakePosLegal()
{
   m_gotopos = __min((m_textsizerect.bottom - m_textsizerect.top) - (m_textdrawrect.bottom - m_textdrawrect.top), m_gotopos);
   m_gotopos = __max(0, m_gotopos);
}

void CReplyDlg::MakeCaretVisible()
{
   int x, y;
   GetCharPoint(m_caretpos, x, y);
   int min = ((y - (theApp.GetTextHeight()/2)) - 4) - m_pos;
   int max = ((y + (theApp.GetTextHeight()/2)) + 4) - m_pos;

   if(min < m_textdrawrect.top)
   {
      m_gotopos -= (m_textdrawrect.top - min);
   }
   else if(max > m_textdrawrect.bottom)
   {
      m_gotopos += (max - m_textdrawrect.bottom);
   }
   MakePosLegal();
}

void CReplyDlg::InvalidateSkin()
{
   m_height = 279 + theApp.GetCellHeight();

   if(m_backgroundbrush != NULL)
   {
      ::DeleteObject(m_backgroundbrush);
   }

   if(m_textbackgroundbrush != NULL)
   {
      ::DeleteObject(m_textbackgroundbrush);
   }

   if(theApp.GetPostBackground()->GetBitmap() != NULL)
   {
      m_backgroundbrush = ::CreatePatternBrush(theApp.GetPostBackground()->GetBitmap());
   }
   else
   {
      m_backgroundbrush = ::CreateSolidBrush(theApp.GetPostBackgroundColor());
   }

   if(theApp.GetReplyBackground()->GetBitmap() != NULL)
   {
      m_textbackgroundbrush = ::CreatePatternBrush(theApp.GetReplyBackground()->GetBitmap());
   }
   else
   {
      m_textbackgroundbrush = ::CreateSolidBrush(theApp.GetEditBackgroundColor());
   }

   if(m_replyfont != NULL)
   {
      ::DeleteObject(m_replyfont);
   }

   m_replyfont = ::CreateFontW(theApp.GetFontHeight(),0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_DONTCARE,theApp.GetNormalFontName());

   RecalcCharWidths();
   RecalcLines();
   MakeCaretVisible();

   if(m_bPreviewMode)
   {
      m_previewhost.DecodeShackTagsString(m_replytext);
   }
}

void CReplyDlg::CheckSpelling()
{
   m_badwords.clear();
   m_shacktags.clear();
   const UCChar *begin = m_replytext;
   const UCChar *end = begin + m_replytext.Length();
   const UCChar *work = begin;
   const UCChar *wordstart = NULL;
   int num_alphanums = 0;

   while(work < end)
   {
      if(iswalnum(*work))
      {
         if(wordstart == NULL)
         {
            wordstart = work;
         }
         num_alphanums++;
      }
      else if(*work == L'\'' ||
              *work == L'_' ||
              *work == L'-')
      {
         if(wordstart == NULL)
         {
            wordstart = work;
         }
      }
      else
      {
         if(wordstart != NULL)
         {            
            if(num_alphanums > 0 &&
               !theApp.IsSpelledCorrectly(wordstart, work - wordstart))
            {
               // flag this word as bad
               CCharRange badword;
               badword.begin = wordstart - begin;
               badword.end = work - begin;
               m_badwords.push_back(badword);

               shacktagpos tag(ST_RED,wordstart - begin);
               m_shacktags.push_back(tag);

               tag.m_tag = ST_RED_END;
               tag.m_pos = work - begin;
               m_shacktags.push_back(tag);
            }
            wordstart = NULL;
            num_alphanums = 0;
         }
      }

      work++;
   }

   if(wordstart != NULL)
   {
      if(num_alphanums > 0 &&
         !theApp.IsSpelledCorrectly(wordstart, end - wordstart))
      {
         // flag this word as bad
         CCharRange badword;
         badword.begin = wordstart - begin;
         badword.end = end - begin;
         m_badwords.push_back(badword);

         shacktagpos tag(ST_RED,wordstart - begin);
         m_shacktags.push_back(tag);

         tag.m_tag = ST_RED_END;
         tag.m_pos = end - begin;
         m_shacktags.push_back(tag);
      }
      wordstart = NULL;
   }

   RecalcLines();
}


void CReplyDlg::OnTimer(UINT nIDEvent)
{
   if(m_timer != 0 &&
      m_timer == nIDEvent)
   {
      m_pView->KillTimer(m_timer);
   }
   CheckSpelling();
   m_pView->InvalidateEverything();
}

bool CReplyDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
   bool bITookIt = false;

   if(m_bSuggestionsUp)
   {
      // kill all rbutton activity
      bITookIt = true;
   }
   else if(!m_bPreviewMode &&
      point.x >= m_replydlgrect.left &&
      point.x < m_replydlgrect.right &&
      point.y >= m_replydlgrect.top &&
      point.y < m_replydlgrect.bottom)
   {
      m_bHaveFocus = true;
      int charpos = GetCharPos(point.x, point.y + m_pos);

      for(size_t i = 0; i < m_badwords.size(); i++)
      {
         if(charpos >= m_badwords[i].begin &&
            charpos <= m_badwords[i].end)
         {
            theApp.GetSpellSuggestions(m_replytext.Str() + m_badwords[i].begin, m_badwords[i].end - m_badwords[i].begin, m_suggestions);
            if(m_suggestions.size() > 0)
            {
               bITookIt = true;
               m_bSuggestionsUp = true;
               m_suggestion_who = m_badwords[i];
               m_widest_suggestion = 0;

               for(size_t j = 0; j < m_suggestions.size(); j++)
               {
                  int numchars = m_suggestions[j].Length();
                  int *pCharWidths = (int*)malloc(sizeof(int) * numchars);
                  m_pView->GetDocument()->GetCharWidths(m_suggestions[j], pCharWidths, m_suggestions[j].Length(), false, false, false, theApp.GetNormalFontName());
                  int thiswidth = 0;
                  for(int k = 0; k < numchars; k++)
                  {
                     thiswidth += pCharWidths[k];
                  }

                  if(thiswidth > m_widest_suggestion)
                  {
                     m_widest_suggestion = thiswidth;
                  }
               }

               m_pView->InvalidateEverything();
            }
            break;
         }
      }
   }
   return bITookIt;
}