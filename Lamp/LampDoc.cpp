
// LampDoc.cpp : implementation of the CLampDoc class
//

#include "stdafx.h"
#include "Lamp.h"
#include "LampDoc.h"
#include "comm.h"
#include "html.h"
#include "webclient.h"
#include "ReplyDlg.h"
#include "AuthorDlg.h"
#include "chatty.h"
#include "stack.h"
#include "thread.h"

#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;

#define NDEBUG
#include "JSONOptions.h"
#include "libjson.h"
#include "JSONNode.h"

extern CRITICAL_SECTION g_ThreadAccess;

DWORD g_LastPostTime = 0;

DWORD g_LastShackLoginTime = 0;

char g_ShackLoginCookie[1024];

DWORD g_LastUserShackLoginTime = 0;

char g_UserShackLoginCookie[1024];

extern bool g_bSingleThreadStyle;

UINT DownloadThreadProc( LPVOID pParam )
{
   CDownloadData* pDD = (CDownloadData*)pParam;

   if(pDD != NULL &&
      pDD->m_WhoWants != NULL)
   {
      if(g_bSingleThreadStyle)
      {
         ::EnterCriticalSection(&g_ThreadAccess);
      }

      if(pDD->m_dt == DT_SHACK_CHATTY ||
         pDD->m_dt == DT_SHACK_CHATTY_INFINATE_PAGE ||
         pDD->m_dt == DT_SHACK_THREAD_CONTENTS || 
         pDD->m_dt == DT_SHACK_THREAD ||
         pDD->m_dt == DT_SHACK_POST ||
         pDD->m_dt == DT_SHACK_SEARCH ||
         pDD->m_dt == DT_SHACK_SHACKMSG ||
         pDD->m_dt == DT_SHACK_READMSG ||
         pDD->m_dt == DT_SHACK_SENDMSG ||
         pDD->m_dt == DT_SHACK_DELETEMSG ||
         pDD->m_dt == DT_SHACK_MOD_CATEGORY_CHANGE ||
         pDD->m_dt == DT_LOL ||
         pDD->m_dt == DT_GET_PROFILE)
      {
         pDD->getchatty(3);
      }
      else if((pDD->m_dt == DT_POST ||
               pDD->m_dt == DT_SENDMSG) &&
              !pDD->m_post_data.IsEmpty())
      {
         pDD->post(1);
      }
      else
      {
         pDD->download(3);
      }

      theApp.PostThreadMessageW(WM_THREAD_DOWNLOAD,0,(LPARAM)pDD);

      if(g_bSingleThreadStyle)
      {
         ::LeaveCriticalSection(&g_ThreadAccess);
      }

      return 0;
   }

   return 1;// if pObject is not valid
}


void GetCharWidths(const UCChar *text, int *widths, size_t numchars, bool italic, bool bold, bool sample, const UCChar *fontname, bool *pComplex/* = NULL*/)
{
   bool complex = false;
   if(pComplex != NULL)
   {
      complex = *pComplex;
      if(*pComplex == false)
      {
         // test to see if the text is complex.
         const UCChar *work = text;
         const UCChar *end = text + numchars;

         while(work < end)
         {
            if((*work >= 0 && *work <= 255) ||
               (*work >= 0x2012 && *work <= 0x2026) ||
               (*work >= 0x2030 && *work <= 0x2044))
            {
               // ascii and some common unicode punctuation chars
               // continue
               work++;
            }
            else if((*work >= 0x0300 && *work <= 0x036F) ||
                    (*work >= 0x0370 && *work <= 0x3FF) ||
                    (*work >= 0x0400 && *work <= 0x04FF))
            {
               // zalgo
               // continue
               work++;
            }
            else
            {
               complex = true;
               *pComplex = true;
               break;
            }
         }
      }
   }

   memset(widths,0,sizeof(int) * numchars);
   GCP_RESULTS results;
   memset(&results,0,sizeof(GCP_RESULTS));
   results.lStructSize = sizeof(GCP_RESULTS);
   results.nGlyphs = numchars;
   results.lpDx = widths;
   HFONT hCreatedFont = NULL;
   HFONT oldfont = NULL;
      
   int fsize = theApp.GetFontHeight();
   if(sample)
   {
      fsize = theApp.GetSampleFontHeight();
   }
   int weight = FW_NORMAL;
   if(bold) weight = FW_EXTRABOLD;
   DWORD ditlc = 0;
   if(italic) ditlc = 1;
   hCreatedFont = ::CreateFontW(fsize,0,0,0,weight,ditlc,0,0,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_DONTCARE,fontname);
   HDC hTempDC = ::CreateCompatibleDC(NULL);

   /*
   if(complex)
   {
      Graphics graphics(hTempDC);

      graphics.SetPageUnit(UnitPixel);
      graphics.SetSmoothingMode(SmoothingModeHighQuality);
      graphics.SetCompositingMode(CompositingModeSourceOver);
      graphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);
      graphics.SetCompositingQuality(CompositingQualityHighQuality);

      RectF rectf(0, 0, 1000000000, -fsize);

      Gdiplus::Font font(hTempDC, hCreatedFont);

      StringFormat format;
      format.SetAlignment(StringAlignmentNear);

      Region *regions = new Region[numchars];
      CharacterRange *charRanges = new CharacterRange[numchars];
      for(size_t i = 0; i < numchars; i++)
      {
         charRanges[i].First = i;
         charRanges[i].Length = 1;
      }

      const UCChar *work = text;
      const UCChar *end = text + numchars;
      const UCChar *endwork = __min(end, work + 32);
      Region *workregions = regions;
      size_t width_index = 0;
      bool done = false;
      while(!done)
      {
         Status result = format.SetMeasurableCharacterRanges(endwork - work, charRanges);
         result = graphics.MeasureCharacterRanges(work, endwork - work, &font, rectf, &format, endwork - work, workregions);

         int totalsize = 0;
         for(size_t i = 0; i < endwork - work; i++)
         {
            Rect rect;
            result = workregions[i].GetBounds(&rect,&graphics);
            widths[width_index] = rect.GetRight() - totalsize;
            
            totalsize += widths[i];
            width_index++;
         }

         if(endwork < end)
         {
            workregions += 32;
            work += 32;
            endwork = __min(end, work + 32);
         }
         else
         {
            done = true;
         }
      }
   }
   else
   {
   */
      oldfont = (HFONT)::SelectObject(hTempDC,hCreatedFont);

      DWORD info = ::GetFontLanguageInfo(hTempDC);

      bool bGCP_DBCS         = (info & GCP_DBCS) ?true:false;
      bool bGCP_DIACRITIC    = (info & GCP_DIACRITIC) ?true:false;
      bool bFLI_GLYPHS       = (info & FLI_GLYPHS) ?true:false;
      bool bGCP_GLYPHSHAPE   = (info & GCP_GLYPHSHAPE) ?true:false;
      bool bGCP_KASHIDA      = (info & GCP_KASHIDA) ?true:false;
      bool bGCP_LIGATE       = (info & GCP_LIGATE) ?true:false;
      bool bGCP_USEKERNING   = (info & GCP_USEKERNING) ?true:false;
      bool bGCP_REORDER      = (info & GCP_REORDER) ?true:false;

      if(bFLI_GLYPHS)
      {
         for (size_t i = 0; i < numchars; i++)
         {
            SIZE temp_textSize;
            int temp_dx;
            int temp_nFit;
            ::GetTextExtentExPointW(hTempDC, text + i, 1, 5000000, &temp_nFit, &temp_dx, &temp_textSize);
            widths[i] = temp_dx;

            if(text[i] == 0xfe35)
               widths[i] = theApp.GetTextHeight();
         }
      }
      else
      {
         DWORD flags = 0;
         if(bGCP_USEKERNING) flags |= GCP_USEKERNING;   
         ::GetCharacterPlacementW(hTempDC,text,numchars,0,&results,flags);
      }

      ::SelectObject(hTempDC,oldfont);
   //}

   if(hTempDC != NULL)
   {
      ::DeleteDC(hTempDC);
   }

   if(hCreatedFont != NULL)
   {
      ::DeleteObject(hCreatedFont);
   }
}

void CDownloadData::setupshacklogin()
{
   std::string stdstring;

   error_t result = comm_download("www.shacknews.com","/login_laryn.x", &stdstring, "username=latestchatty&password=8675309&type=login","", NULL);

   // Set-Cookie: login=209981%2Clatestchatty%2Cf6356e3a49aedb4ba8a5442102b9810b; expires=Tue, 03-May-2011 18:27:44 GMT
   if(stdstring.length() > 0 && strncmp(stdstring.data()," OK",3) == 0)
   {
      const char *cook = strstr(stdstring.data(), "Set-Cookie: login=");
      if(cook != NULL)
      {
         cook += 12;
         const char *end = strstr(cook, "; expires=");
         if(end != NULL)
         {
            if(!g_bSingleThreadStyle)
            {
               ::EnterCriticalSection(&g_ThreadAccess);
            }
            strncpy_s(g_ShackLoginCookie,1024,cook,end-cook);
            g_LastShackLoginTime = ::GetTickCount();
            if(!g_bSingleThreadStyle)
            {
               ::LeaveCriticalSection(&g_ThreadAccess);
            }
         }
      }
   }
}

void CDownloadData::setupusershacklogin()
{
   std::string stdstring;
   UCString loginstr = L"username=";
   loginstr += m_username;
   loginstr += L"&password=" ;
   //loginstr += m_password;

   char *enc_password = NULL;

   if(!m_password.IsEmpty())
   {
      enc_password = url_encode(m_password.str8(false,CET_UTF8));
      loginstr += enc_password;
      free(enc_password);
   }

   loginstr += L"&type=login";

   error_t result = comm_download("www.shacknews.com","/login_laryn.x", &stdstring, loginstr.str8(false,CET_UTF8),"",NULL);

   // Set-Cookie: login=209981%2Clatestchatty%2Cf6356e3a49aedb4ba8a5442102b9810b; expires=Tue, 03-May-2011 18:27:44 GMT
   if(stdstring.length() > 0 && strncmp(stdstring.data()," OK",3) == 0)
   {
      const char *cook = strstr(stdstring.data(), "Set-Cookie: login=");
      if(cook != NULL)
      {
         cook += 12;
         const char *end = strstr(cook, "; expires=");
         if(end != NULL)
         {
            if(!g_bSingleThreadStyle)
            {
               ::EnterCriticalSection(&g_ThreadAccess);
            }
            strncpy_s(g_UserShackLoginCookie,1024,cook,end-cook);
            g_LastUserShackLoginTime = ::GetTickCount();
            if(!g_bSingleThreadStyle)
            {
               ::LeaveCriticalSection(&g_ThreadAccess);
            }
         }
      }
   }
}

void CDownloadData::getchatty(int numtries)
{
   m_start_time = ::GetTickCount();

   std::string usercookie;
   if(!m_username.IsEmpty())
   {
      if(g_LastUserShackLoginTime == 0 ||
         ::GetTickCount() - g_LastUserShackLoginTime > 86400000)
      {
         setupusershacklogin();
      }
      if(!g_bSingleThreadStyle)
      {
         ::EnterCriticalSection(&g_ThreadAccess);
      }
      usercookie = g_UserShackLoginCookie;
      if(!g_bSingleThreadStyle)
      {
         ::LeaveCriticalSection(&g_ThreadAccess);
      }
   }
   else
   {
      if(g_LastShackLoginTime == 0 ||
         ::GetTickCount() - g_LastShackLoginTime > 86400000)
      {
         setupshacklogin();
      }
      if(!g_bSingleThreadStyle)
      {
         ::EnterCriticalSection(&g_ThreadAccess);
      }
      usercookie = g_ShackLoginCookie;
      if(!g_bSingleThreadStyle)
      {
         ::LeaveCriticalSection(&g_ThreadAccess);
      }
   }

   const char *enc_host = m_host.str8(false,CET_UTF8);

   const char *path = m_path.str8(false,CET_UTF8);

   while(numtries > 0)
   {
      DWORD starttime = ::GetTickCount();
      const char *postdata = NULL;
      if(!m_post_data.IsEmpty())
         postdata = m_post_data.str8(false,CET_UTF8);

      error_t result = comm_download(enc_host,path,&m_stdstring,postdata,usercookie.data(),&m_recieve_time);
      DWORD endtime = ::GetTickCount();

      if(m_stdstring.length() > 0)
      {
         // todo : need clear way to know that the generic user login has expired, for other cases
         if(m_dt == DT_SHACK_CHATTY ||
            m_dt == DT_SHACK_CHATTY_INFINATE_PAGE)
         {
            const char *proof = strstr(m_stdstring.data(),"<li class=\"user light\">latestchatty</li>");
            if(proof == NULL)
            {
               // try logging in again
               if(!m_username.IsEmpty())
               {
                  setupusershacklogin();
                  if(!g_bSingleThreadStyle)
                  {
                     ::EnterCriticalSection(&g_ThreadAccess);
                  }
                  usercookie = g_UserShackLoginCookie;
                  if(!g_bSingleThreadStyle)
                  {
                     ::LeaveCriticalSection(&g_ThreadAccess);
                  }
               }
               else
               {
                  setupshacklogin();
                  if(!g_bSingleThreadStyle)
                  {
                     ::EnterCriticalSection(&g_ThreadAccess);
                  }
                  usercookie = g_ShackLoginCookie;
                  if(!g_bSingleThreadStyle)
                  {
                     ::LeaveCriticalSection(&g_ThreadAccess);
                  }
               }
            }
         }
      }

      if(m_stdstring.length() == 0 &&
         m_dt != DT_SHACK_SENDMSG &&
         (endtime - starttime) <= 2000)
      {
         // keep trying
         ::Sleep(1000);
         numtries--;
      }
      else
      {
         // found it
         numtries = 0;
      }      
   }

   m_end_time = ::GetTickCount();
}


void CDownloadData::download(int numtries)
{
   chattyerror err = ERR_NOT_IMPLEMENTED;

   const char *enc_host = m_host.str8(false,CET_UTF8);
   const char *enc_path = m_path.str8(false,CET_UTF8);
   /*
   char *enc_username = NULL;
   char *enc_password = NULL;

   if(!m_username.IsEmpty())
   {
      enc_username = url_encode(m_username.str8(false,CET_UTF8));
   }
   if(!m_password.IsEmpty())
   {
      enc_password = url_encode(m_password.str8(false,CET_UTF8));
   }
   */

   const char *enc_username = NULL;

   if(!m_username.IsEmpty())
   {
      enc_username = m_username.str8(false,CET_UTF8);
   }

   const char *enc_password = NULL;

   if(!m_password.IsEmpty())
   {
      enc_password = m_password.str8(false,CET_UTF8);
   }

   while(numtries > 0)
   {
      DWORD starttime = ::GetTickCount();
      err = webclient_download(enc_host, enc_path, enc_username, enc_password, (char**)&m_data, &m_datasize);
      DWORD endtime = ::GetTickCount();

      if(m_data == NULL && 
         (endtime - starttime) <= 2000)
      {
         // keep trying
         ::Sleep(1000);
         numtries--;
      }
      else
      {
         // found it
         numtries = 0;
      }      
   }
   /*
   if(enc_username != NULL)
   {
      free(enc_username);
   }
   if(enc_password != NULL)
   {
      free(enc_password);
   }
   */
}

void CDownloadData::post(int numtries)
{
   chattyerror err = ERR_NOT_IMPLEMENTED;

   const char *enc_host = m_host.str8(false,CET_UTF8);
   const char *enc_path = m_path.str8(false,CET_UTF8);
   const char *enc_post_data = m_post_data.str8(false,CET_UTF8);
   //char *enc_username = url_encode(m_username.str8(false,CET_UTF8));
   //char *enc_password = url_encode(m_password.str8(false,CET_UTF8));
   const char *enc_username = m_username.str8(false,CET_UTF8);
   const char *enc_password = m_password.str8(false,CET_UTF8);

   while(numtries > 0)
   {
      DWORD starttime = ::GetTickCount();
      err = webclient_post(enc_host, enc_path, enc_post_data, enc_username, enc_password, (char**)&m_data, &m_datasize);
      DWORD endtime = ::GetTickCount();

      if(m_data == NULL && 
         (endtime - starttime) <= 2000)
      {
         // keep trying
         ::Sleep(1000);
         numtries--;
      }
      else
      {
         // found it
         numtries = 0;
      }      
   }
   /*
   free(enc_username);
   free(enc_password);
   */
}

void CLampDoc::StartDownload(const UCChar *host,
                             const UCChar *path,
                             DownloadType dt,
                             unsigned int id,
                             unsigned int refresh_id/* = 0*/,
                             unsigned int reply_to_id/* = 0*/,
                             const UCChar *post_data/* = NULL*/,
                             const UCChar *username/* = NULL*/,
                             const UCChar *password/* = NULL*/,
                             bool bIgnoreTimeStamp /*= false*/)
{
   if(dt != DT_THREAD_START &&
      dt != DT_THREAD &&
      dt != DT_SHACK_THREAD &&
      dt != DT_SHACK_THREAD_CONTENTS &&
      dt != DT_READMSG &&
      dt != DT_SHACK_DELETEMSG &&
      dt != DT_SHACK_READMSG &&
      dt != DT_SUBMIT_LOLVOTE &&
      ((dt != DT_SHACKMSG &&
        dt != DT_SHACK_SHACKMSG) ||
       (dt == DT_SHACKMSG ||
        dt == DT_SHACK_SHACKMSG) &&
       ((CMainFrame*)theApp.GetMainWnd())->GetActiveLampView() == m_pView))
   {
      m_bBusy = true;
   }

   if(theApp.IsModMode() &&
      (dt == DT_SHACK_CHATTY ||
       dt == DT_SHACK_CHATTY_INFINATE_PAGE ||
       dt == DT_SHACK_THREAD_CONTENTS || 
       dt == DT_SHACK_THREAD ||
       dt == DT_SHACK_POST ||
       dt == DT_SHACK_SEARCH ||
       dt == DT_SHACK_SHACKMSG ||
       dt == DT_SHACK_READMSG ||
       dt == DT_SHACK_SENDMSG ||
       dt == DT_SHACK_DELETEMSG ||
       dt == DT_SHACK_MOD_CATEGORY_CHANGE ||
       dt == DT_LOL) &&
      (username == NULL ||
       password == NULL))
   {
      username = theApp.GetUsername();
      password = theApp.GetPassword();
   }

   CDownloadData *pDD = new CDownloadData();

   pDD->m_host = host;
   pDD->m_path = path;
   pDD->m_WhoWants = this;
   pDD->m_dt = dt;
   pDD->m_id = id;
   pDD->m_refreshid = refresh_id;
   pDD->reply_to_id = reply_to_id;
   pDD->m_username = username;
   pDD->m_password = password;
   pDD->m_post_data = post_data;

   if(dt == DT_POST ||
      dt == DT_SHACK_POST)
   {
      pDD->m_postrootid = GetRootId(id);
   }

   if(!bIgnoreTimeStamp)
   {
      g_LastPostTime = ::GetTickCount();
   }

   AfxBeginThread(DownloadThreadProc, pDD);

   if(m_pView != NULL)
   {
      m_pView->OnSetCursor(m_pView, HTCLIENT, 0);
      theApp.SetStatusBarText(UCString(), m_pView);
   }
}

void CLampDoc::ProcessDownload(CDownloadData *pDD)
{
   if(pDD != NULL)
   {
      CDownloadHistoryItem item;
      item.m_host         = pDD->m_host;
      item.m_path         = pDD->m_path;
      item.m_errmsg       = pDD->m_errmsg;
      item.m_post_data    = pDD->m_post_data;
      item.m_dt           = pDD->m_dt;
      item.m_start_time   = pDD->m_start_time;
      item.m_recieve_time = pDD->m_recieve_time;
      item.m_end_time     = pDD->m_end_time;
      theApp.AddDownloadHistoryItem(item);


      bool bDoingNewFlags = true;
      switch(pDD->m_dt)
      {
      case DT_GET_PROFILE:
         {
            if(pDD->m_stdstring.length() > 0 != NULL)
            {
               const char *text = (const char *)pDD->m_stdstring.data();
               text = strstr(text, "Content-Type: application/json");
               if(text != NULL)
               {
                  text = strstr(text, "{");
               }

               if(text != NULL)
               {
                  ReadProfile(text, pDD->m_stdstring.length() - (text - pDD->m_stdstring.data()));
               }
            }
         }
         break;
      case DT_THREAD_START:
         {
            ChattyPost *post = FindRootPost(pDD->m_id);
            if(post == NULL)
            {
               post = new ChattyPost();
               post->SetId(pDD->m_id);
               post->SetDoc(this);
               UCString text = L"Nuked?  Or timed out.";
               post->SetBodyText(text);
               post->InvalidateSkin();
               m_rootposts.push_back(post);
            }
            if(post != NULL &&
               !post->IsPinned())
            {
               bDoingNewFlags = false;
            }
         }
         // fall through
      case DT_THREAD:
         {
            if(pDD->m_data != NULL)
            {
               CXMLTree xmldata;
               if(GetXMLDataFromString(xmldata, (const char *)pDD->m_data, pDD->m_datasize))
               {
                  if(ReadExistingThreadFromRoot(xmldata,pDD->m_id,bDoingNewFlags))
                  {
                     if(m_datatype == DDT_THREAD &&
                        m_title == L"loading")
                     {
                        m_title = L"";
                        if(m_rootposts.size() == 1)
                        {
                           (*m_rootposts.begin())->GetTitle(m_title);
                        }
                        else
                        {
                           m_title = L"FAIL'D";
                        }

                        MySetTitle(m_title);
                     }
                     
                     if(m_pView != NULL)
                     {
                        if(m_datatype == DDT_THREAD &&
                           m_pView->GetCurrentId() == 0 &&
                           m_initialpostid != 0)
                        {
                           m_pView->SetCurrentId(m_initialpostid);
                           m_pView->MakeCurrentPosVisible();
                           m_initialpostid = 0;
                        }
                     }
                  }
               }
            }

            if(pDD->reply_to_id != 0)
            {
               bool bGotIt = false;
               // check to see if there is a new post by me on there.
               ChattyPost *pReplyToPost = FindPost(pDD->reply_to_id);
               if(pReplyToPost != NULL)
               {
                  std::list<ChattyPost*> *pChildren = pReplyToPost->GetChildren();
                  if(pChildren != NULL)
                  {
                     std::list<ChattyPost*>::iterator begin = pChildren->begin();
                     std::list<ChattyPost*>::iterator it = pChildren->end();
                     if(begin != it)
                     {
                        it--;
                        unsigned int new_id = 0;
                        bool bDone = false;
                        while(!bDone)
                        {
                           if(begin == it)
                           {
                              bDone = true;
                           }
                           
                           if((*it)->IsNew()  &&
                              (*it)->GetAuthor() == theApp.GetUsername())
                           {
                              new_id = (*it)->GetId();
                              bDone = true;
                              break;
                           }

                           if(!bDone)
                           {
                              it--;
                           }
                        }

                        if(new_id != 0 &&
                           m_pView != NULL)
                        {
                           m_pView->SetCurrentId(new_id);
                           bGotIt = true;
                        }
                     }
                  }
               }

               if(bGotIt == false)
               {
                  //assert(0);
               }
            }

            // tell the thread that he is done refreshing
            ChattyPost *pPost = FindPost(pDD->m_refreshid);
            if(pPost != NULL)
            {
               pPost->SetRefreshing(false);
               if(m_pView)
                  m_pView->InvalidateEverything();
            }
         }
         break;
      case DT_SHACK_THREAD:
         {
            unsigned int whohasreplydlg_id = 0;
            CReplyDlg *pReplyDlg = NULL;
            std::vector<unsigned int> existing_threads;

            ChattyPost *post = FindRootPost(pDD->m_id);
            if(post != NULL)
            {
               existing_threads.push_back(pDD->m_id);

               pReplyDlg = post->FindReplyDlgInPostRecurse(whohasreplydlg_id);
            }

            // scrape HTML
            ReadChattyPageFromHTML(pDD->m_stdstring, existing_threads, false);

            if(m_datatype == DDT_THREAD &&
               m_title == L"loading")
            {
               m_title = L"";
               if(m_rootposts.size() == 1)
               {
                  (*m_rootposts.begin())->GetTitle(m_title);
               }
               else
               {
                  m_title = L"FAIL'D";
               }

               MySetTitle(m_title);
            }

            if(whohasreplydlg_id != 0 &&
               pReplyDlg != NULL)
            {
               ChattyPost *post = FindPost(whohasreplydlg_id);
               if(post != NULL)
               {
                  post->SetReplyDlg(pReplyDlg);
               }
            }
            
            if(m_pView != NULL)
            {
               if(m_datatype == DDT_THREAD &&
                  m_pView->GetCurrentId() == 0 &&
                  m_initialpostid != 0)
               {
                  m_pView->SetCurrentId(m_initialpostid);
                  m_pView->MakeCurrentPosVisible();
                  m_initialpostid = 0;
               }
            }

            if(pDD->reply_to_id != 0 &&
               theApp.GotoNewPost())
            {
               bool bGotIt = false;
               // check to see if there is a new post by me on there.
               ChattyPost *pReplyToPost = FindPost(pDD->reply_to_id);
               if(pReplyToPost != NULL)
               {
                  std::list<ChattyPost*> *pChildren = pReplyToPost->GetChildren();
                  if(pChildren != NULL)
                  {
                     std::list<ChattyPost*>::iterator begin = pChildren->begin();
                     std::list<ChattyPost*>::iterator it = pChildren->end();
                     if(begin != it)
                     {
                        it--;
                        unsigned int new_id = 0;
                        bool bDone = false;
                        while(!bDone)
                        {
                           if(begin == it)
                           {
                              bDone = true;
                           }
                           
                           if((*it)->IsNew()  &&
                              (*it)->GetAuthor() == theApp.GetUsername())
                           {
                              new_id = (*it)->GetId();
                              bDone = true;
                              break;
                           }

                           if(!bDone)
                           {
                              it--;
                           }
                        }

                        if(new_id != 0 &&
                           m_pView != NULL)
                        {
                           m_pView->SetCurrentId(new_id);
                           m_pView->MakeCurrentPostLegal();
                           bGotIt = true;
                        }
                     }
                  }
               }

               if(bGotIt == false)
               {
                  //assert(0);
               }
            }

            post = FindPost(pDD->m_refreshid);
            if(post != NULL)
            {
               post->SetRefreshing(false);
               if(m_pView)
                  m_pView->InvalidateEverything();
            }
         }
         break;
      case DT_SHACK_CHATTY:
         {
            std::vector<unsigned int> existing_threads;
            std::list<ChattyPost*> newroots;
            std::list<ChattyPost*>::iterator it = m_rootposts.begin();
            std::list<ChattyPost*>::iterator end = m_rootposts.end();

            while(it != end)
            {
               if((*it) != NULL)
               {
                  if((*it)->IsPinned())
                  {
                     RefreshThread((*it)->GetId(),(*it)->GetId(),true);
                  }
               }
               it++;
            }

            it = m_rootposts.begin();

            while(it != end)
            {
               if((*it) != NULL)
               {
                  if((*it)->IsPinned())
                  {
                     existing_threads.push_back((*it)->GetId());
                     newroots.push_back((*it));
                  }
                  else
                  {
                     delete (*it);
                  }
                  (*it) = NULL;
               }
               it++;
            }
            m_rootposts.clear();
            m_rootposts = newroots;
            
            // scrape HTML
            ReadChattyPageFromHTML(pDD->m_stdstring, existing_threads, true);
            
            it = m_rootposts.begin();
            end = m_rootposts.end();
            while(it != end)
            {
               if((*it) != NULL)
               {
                  (*it)->ShowAsTruncated();
               }
               it++;
            }
         }
         break;
      case DT_SHACK_CHATTY_INFINATE_PAGE:
         {
            ChattyPost *pagepost = new ChattyPost();
            pagepost->SetAsPageBreak(m_page);
            pagepost->SetDoc(this);
            m_rootposts.push_back(pagepost);

            std::vector<unsigned int> existing_threads;
            std::list<ChattyPost*> newroots;
            std::list<ChattyPost*>::iterator it = m_rootposts.begin();
            std::list<ChattyPost*>::iterator end = m_rootposts.end();
            std::list<ChattyPost*>::iterator last = end;
            if(m_rootposts.size() > 0)
            {
               last--;
            }
            while(it != end)
            {
               if((*it) != NULL)
               {
                  existing_threads.push_back((*it)->GetId());
               }
               it++;
            }
            
            // scrape HTML
            ReadChattyPageFromHTML(pDD->m_stdstring, existing_threads, true, true);

            if(last != end)
            {
               it = last;
               it++;
            }
            else
            {
               it = m_rootposts.begin();
            }
            end = m_rootposts.end();
            while(it != end)
            {
               if((*it) != NULL)
               {
                  (*it)->ShowAsTruncated();
               }
               it++;
            }

            // update the page's height by forcing a draw
            if(m_pView)
            {
               m_pView->DrawEverythingToBuffer();
            }

            m_bFetchingNextPage = false;
         }
         break;
      case DT_SHACK_THREAD_CONTENTS:
         {
            htmlcxx::HTML::ParserDom parser;
            tree<htmlcxx::HTML::Node> dom;
            try
            {
               dom = parser.parseTree(pDD->m_stdstring);
            }
            catch(...)
            {
               // whatever. move on
            }
            
            RemoveContentsRequest(pDD->m_id);

            tree<htmlcxx::HTML::Node>::iterator it = dom.begin();
            tree<htmlcxx::HTML::Node>::iterator end = dom.end();
            while(it != end)
            {
               if(it->tagName() == "body")break;
               it++;
            }

            if(it != end)
            {
               int old_top = 0;
               int old_bottom = 0;
               int old_center = 0;

               if(m_pView)
               {
                  unsigned int hoverpreviewid = m_pView->GetHoverPreviewId();
                  if(hoverpreviewid != 0)
                  {
                     ChattyPost *hoverpost = FindPost(hoverpreviewid);
                     if(hoverpost != NULL)
                     {
                        old_top = hoverpost->GetPos();
                        old_bottom = old_top + hoverpost->GetHeight();
                        old_center = (old_top + old_bottom) >> 1;
                     }
                  }
               }

               tree<htmlcxx::HTML::Node>::sibling_iterator post_it = it.begin();
               tree<htmlcxx::HTML::Node>::sibling_iterator post_end = it.end();
               while(post_it != post_end)
               {
                  if(post_it->tagName() == "div")
                  {
                     unsigned int post_id = HTML_GetIDAttribute(post_it);

                     if(post_id != 0)
                     {
                        if(!theApp.IsPostKnown(post_id))
                        {
                           ChattyPost *post = new ChattyPost();
                           post->SetNewness(N_OLD);

                           post->ReadKnownPostChattyFromHTML(post_it, post_id);

                           theApp.KnowPost(post_id, post);
                        }
                     }
                  }
                  post_it++;
               }

               ChattyPost *root_post = FindRootPost(pDD->m_id);
               if(root_post != NULL)
               {
                  unsigned int root_id = root_post->GetId();
                  root_post->UpdatePreviewsToKnown();

                  if(root_post->IsFiltered())
                  {
                     m_rootposts.remove(root_post);

                     delete(root_post);
                  }

                  if(m_pView)
                  {
                     unsigned int currentid = m_pView->GetCurrentId();
                     if(currentid != 0 &&
                        FindPost(currentid) == NULL)
                     {
                        m_pView->SetCurrentId(root_id);
                     }
                  }
               }

               ChattyPost *post = FindPost(pDD->m_refreshid);
               if(post != NULL)
               {
                  post->SetRefreshing(false);
               }

               if(m_pView)
               {
                  unsigned int hoverpreviewid = m_pView->GetHoverPreviewId();
                  if(hoverpreviewid != 0 && old_center != 0)
                  {
                     ChattyPost *hoverpost = FindPost(hoverpreviewid);
                     if(hoverpost != NULL)
                     {
                        m_pView->DrawEverythingToBuffer();
                        int top = hoverpost->GetPos();
                        int bottom = top + hoverpost->GetHeight();
                        int center = (top + bottom) >> 1;

                        m_pView->SetPos(m_pView->GetPos() + (center - old_center));
                     }
                  }
               }

               if(m_pView)
                  m_pView->InvalidateEverything();
            }
         }
         break;
      case DT_STORY:
         {
            std::vector<unsigned int> existing_threads;
            std::list<ChattyPost*> newroots;
            std::list<ChattyPost*>::iterator it = m_rootposts.begin();
            std::list<ChattyPost*>::iterator end = m_rootposts.end();
            while(it != end)
            {
               if((*it) != NULL)
               {
                  if((*it)->IsPinned())
                  {
                     existing_threads.push_back((*it)->GetId());
                     newroots.push_back((*it));
                  }
                  else
                  {
                     delete (*it);
                  }
                  (*it) = NULL;
               }
               it++;
            }
            m_rootposts.clear();
            m_rootposts = newroots;

            if(pDD->m_data != NULL)
            {
               CXMLTree xmldata;
               if(GetXMLDataFromString(xmldata, (const char *)pDD->m_data, pDD->m_datasize))
               {
                  ReadFromRoot(xmldata, existing_threads);
               }
            }

            it = m_rootposts.begin();
            end = m_rootposts.end();
            while(it != end)
            {
               if((*it) != NULL)
               {
                  (*it)->ShowAsTruncated();
               }
               it++;
            }

            if(theApp.IsDoublePageStory())
            {
               ReadLatestChattyPart2();
            }
            else
            {
               RefreshAllRoots();
            }
         }
         break;
      case DT_STORY_2:
         {
            std::vector<unsigned int> existing_threads;
            std::list<ChattyPost*>::iterator it = m_rootposts.begin();
            std::list<ChattyPost*>::iterator end = m_rootposts.end();
            while(it != end)
            {
               if((*it) != NULL)
               {
                  existing_threads.push_back((*it)->GetId());
               }
               it++;
            }

            if(pDD->m_data != NULL)
            {
               CXMLTree xmldata;
               if(GetXMLDataFromString(xmldata, (const char *)pDD->m_data, pDD->m_datasize))
               {
                  int page = m_page;
                  ReadFromRoot(xmldata, existing_threads);
                  m_page = page;
               }
            }

            it = m_rootposts.begin();
            end = m_rootposts.end();
            while(it != end)
            {
               if((*it) != NULL)
               {
                  (*it)->ShowAsTruncated();
               }
               it++;
            }

            RefreshAllRoots();
         }
         break;
      case DT_SHACK_SEARCH:
         {
            htmlcxx::HTML::ParserDom parser;
            tree<htmlcxx::HTML::Node> dom;
            try
            {
               dom = parser.parseTree(pDD->m_stdstring);
            }
            catch(...)
            {
               // whatever. move on
            }

            tree<htmlcxx::HTML::Node>::sibling_iterator it;
            if(HTML_FindChild(dom,it, "body"))
            {
               if(HTML_FindChild_HasAttribute(it,it, "div", "id", "container"))
               {
                  if(HTML_FindChild_HasAttribute(it,it, "div", "id", "content"))
                  {
                     if(HTML_FindChild_HasAttribute(it,it, "div", "id", "main"))
                     {
                        tree<htmlcxx::HTML::Node>::sibling_iterator count_it;
                        if(HTML_FindChild_HasAttribute(it,count_it, "h2", "class", "search-num-found"))
                        {
                           std::string str;
                           HTML_GetValue(count_it,str);

                           ChattyPost *resultcountpost = new ChattyPost();
                           resultcountpost->SetDoc(this);
                           m_rootposts.push_back(resultcountpost);

                           resultcountpost->SetAsText(UCString(str.data()));
                        }

                        tree<htmlcxx::HTML::Node>::sibling_iterator results_it;
                        if(HTML_FindChild_HasAttribute(it,results_it, "ul", "class", "results"))
                        {
                           tree<htmlcxx::HTML::Node>::sibling_iterator sit = results_it.begin();
                           tree<htmlcxx::HTML::Node>::sibling_iterator send = results_it.end();

                           while(sit != send)
                           {
                              std::string attribute_value_remainder;

                              if(sit->tagName() == "li" &&
                                 //HTML_HasAttribute(sit, "class", "result chatty"))
                                 HTML_StartsWithAttribute(sit, "class", "result chatty", &attribute_value_remainder))
                              {
                                 bool bReal = true;
                                 tree<htmlcxx::HTML::Node>::sibling_iterator dummy_it;
                                 if(HTML_FindChild_HasAttribute(sit,dummy_it, "p", "class", "none"))
                                 {
                                    std::string str;
                                    HTML_GetValue(dummy_it,str);
                                    if(str == "No chat messages found")
                                    {
                                       bReal = false;
                                    }
                                 }
                                 //  <li class="result chatty"><p class="none">No chat messages found</p></li>    </ul>

                                 if(bReal)
                                 {
                                    ChattyPost *post = new ChattyPost();
                                    if(post != NULL)
                                    {
                                       m_rootposts.push_back(post);
                                       post->ReadSearchResultFromHTML(sit,this);
                                    }
                                 }
                              }
                              sit++;
                           }
                        }

                        tree<htmlcxx::HTML::Node>::sibling_iterator pagination_it;
                        if(HTML_FindChild_HasAttribute(it,pagination_it, "div", "class", "pagination"))
                        {
                           tree<htmlcxx::HTML::Node>::sibling_iterator sit = pagination_it.begin();
                           tree<htmlcxx::HTML::Node>::sibling_iterator send = pagination_it.end();

                           m_lastpage = 1;
                           
                           while(sit != send)
                           {
                              if(sit->tagName() == "a" &&
                                 HTML_StartsWithAttribute(sit, "href", "/search?"))
                              {
                                 std::string str;
                                 HTML_GetValue(sit,str);
                                 int num = atoi(str.data());
                                 if(num > (int)m_lastpage)
                                 {
                                    m_lastpage = num;
                                 }
                              }
                              sit++;
                           }                              
                        }
                     }
                  }
               }
            }
         }
         break;
      case DT_SEARCH:
         {
            if(pDD->m_data != NULL)
            {
               CXMLTree xmldata;
               if(GetXMLDataFromString(xmldata, (const char *)pDD->m_data, pDD->m_datasize))
               {
                  ReadSearchResultsFromRoot(xmldata);
               }
            }
         }
         break;
      case DT_LOL:
         {
            /*
            if(pDD->m_data != NULL)
            {
               ProcessLOLData((char *)pDD->m_data, pDD->m_datasize);
            }
            */

            if(pDD->m_stdstring.length() > 0)
            {
               ProcessLOLData((char *)pDD->m_stdstring.data(), pDD->m_stdstring.length());
            }
         }
         break;
      case DT_AUTHOR:
         {
            if(pDD->m_data != NULL)
            {
               CXMLTree xmldata;
               if(GetXMLDataFromString(xmldata, (const char *)pDD->m_data, pDD->m_datasize))
               {
                  ChattyPost *post = FindPost(pDD->m_id);
                  if(post != NULL)
                  {
                     UCString author = post->GetAuthor();
                     UCString info = author;

                     CXMLElement *temp = NULL;

                     temp = xmldata.FindChildElement(L"join-date");
                     if(temp != NULL)
                     {
                        info += L"\r\n\r\nRegistered : ";
                        info += temp->GetValue();
                     }

                     temp = xmldata.FindChildElement(L"age");
                     if(temp != NULL)
                     {
                        info += L"\r\nAge : ";
                        info += temp->GetValue();
                     }

                     temp = xmldata.FindChildElement(L"sex");
                     if(temp != NULL)
                     {
                        info += L"\r\nSex : ";
                        info += temp->GetValue();
                     }

                     temp = xmldata.FindChildElement(L"location");
                     if(temp != NULL)
                     {
                        info += L"\r\nLocation : ";
                        info += temp->GetValue();
                     }

                     temp = xmldata.FindChildElement(L"homepage");
                     if(temp != NULL)
                     {
                        info += L"\r\nHomepage : ";
                        info += temp->GetValue();
                     }

                     temp = xmldata.FindChildElement(L"steam");
                     if(temp != NULL && !temp->GetValue().IsEmpty())
                     {
                        info += L"\r\nSteam name : ";
                        info += temp->GetValue();
                     }

                     temp = xmldata.FindChildElement(L"xbox-live");
                     if(temp != NULL && !temp->GetValue().IsEmpty())
                     {
                        info += L"\r\nXbox-live name : ";
                        info += temp->GetValue();
                     }

                     temp = xmldata.FindChildElement(L"psn");
                     if(temp != NULL && !temp->GetValue().IsEmpty())
                     {
                        info += L"\r\nPSN name : ";
                        info += temp->GetValue();
                     }

                     temp = xmldata.FindChildElement(L"wii");
                     if(temp != NULL && !temp->GetValue().IsEmpty())
                     {
                        info += L"\r\nWii name : ";
                        info += temp->GetValue();
                     }

                     temp = xmldata.FindChildElement(L"xfire");
                     if(temp != NULL && !temp->GetValue().IsEmpty())
                     {
                        info += L"\r\nXFire name : ";
                        info += temp->GetValue();
                     }

                     AuthorDlg adlg(m_pView);
                     adlg.m_info = info;
                     adlg.m_author = author;
                     adlg.DoModal();
                  }
               }
            }
         }
         break;
      case DT_SHACK_POST:
         {
            if(!pDD->m_errmsg.IsEmpty())
            {
               if(m_pView != NULL)
               {
                  m_pView->MessageBox(pDD->m_errmsg);
               }
            }
            else
            {
               if(pDD->m_stdstring.length() > 0)
               {
                  bool bCloseAndRefresh = true;
                  const char *result = (const char *)pDD->m_stdstring.data();
                  if(result != NULL &&
                     *result != 0)
                  {
                     if(_strnicmp(result,"error_login_failed",18) == 0 ||
                        _strnicmp(result,"error_communication_authentication",34) == 0 ||
                        strstr(result,"You must be logged in to post") != NULL)
                     {
                        m_pView->MessageBox(L"Invalid username or password");
                        bCloseAndRefresh = false;
                     }

                     if(_strnicmp(result,"error_account_banned",20) == 0)
                     {
                        m_pView->MessageBox(L"You are naughty and banned");
                     }

                     if(_strnicmp(result,"error_post_rate_limiter",23) == 0 ||
                        strstr(result,"Please wait a few minutes before trying to post again.") != NULL)
                     {
                        m_pView->MessageBox(L"PRL'd (slow down)");
                        bCloseAndRefresh = false;
                     }
                  }

                  if(bCloseAndRefresh)
                  {
                     if(m_pView != NULL)
                     {
                        m_pView->CloseReplyDialog();
                     }

                     if(pDD->m_id)
                     {
                        RefreshThread(GetRootId(pDD->m_id),pDD->m_id,false,pDD->m_id);
                     }
                     else
                     {
                        Refresh();
                     }
                  }
               }
               else
               {
                  // new thread
                  if(m_pView != NULL)
                  {
                     m_pView->CloseReplyDialog();
                  }
                  Refresh();
               }
            }
         }
         break;
      case DT_POST:
         {
            if(!pDD->m_errmsg.IsEmpty())
            {
               if(m_pView != NULL)
               {
                  m_pView->MessageBox(pDD->m_errmsg);
               }
            }
            else
            {
               if(pDD->m_data != NULL)
               {
                  bool bCloseAndRefresh = true;
                  const char *result = (const char *)pDD->m_data;
                  if(result != NULL &&
                     *result != 0)
                  {
                     if(_strnicmp(result,"error_login_failed",18) == 0 ||
                        _strnicmp(result,"error_communication_authentication",34) == 0 ||
                        strstr(result,"You must be logged in to post") != NULL)
                     {
                        m_pView->MessageBox(L"Invalid username or password");
                        bCloseAndRefresh = false;
                     }

                     if(_strnicmp(result,"error_account_banned",20) == 0)
                     {
                        m_pView->MessageBox(L"You are naughty and banned");
                     }

                     if(_strnicmp(result,"error_post_rate_limiter",23) == 0 ||
                        strstr(result,"Please wait a few minutes before trying to post again.") != NULL)
                     {
                        m_pView->MessageBox(L"PRL'd (slow down)");
                        bCloseAndRefresh = false;
                     }
                  }

                  if(bCloseAndRefresh)
                  {
                     if(m_pView != NULL)
                     {
                        m_pView->CloseReplyDialog();
                     }

                     if(pDD->m_id)
                     {
                        RefreshThread(GetRootId(pDD->m_id),pDD->m_id,false,pDD->m_id);
                     }
                     else
                     {
                        Refresh();
                     }
                  }
               }
               else
               {
                  // new thread
                  if(m_pView != NULL)
                  {
                     m_pView->CloseReplyDialog();
                  }
                  Refresh();
               }
            }
         }
         break;
      case DT_SHACK_SHACKMSG:
         {
            ReadShackMessagesHTML(pDD->m_stdstring);
            UpdateUnreadShackMessagesCount();

            if(theApp.PeekUserID() == 0)
            {
               const char *work = strstr((const char*)pDD->m_stdstring.data(),"<input type=\"hidden\" name=\"uid\" value=\"");
               if(work != NULL)
               {
                  work += 39;
                  unsigned int userid = strtoul(work,NULL, 10);
                  theApp.SetUserID(userid);
               }
            }
         }
         break;
      case DT_SHACKMSG:
         {
            if(pDD->m_data != NULL)
            {
               CXMLTree xmldata;
               if(GetXMLDataFromString(xmldata, (const char *)pDD->m_data, pDD->m_datasize))
               {
                  ReadShackMessages(xmldata);
                  UpdateUnreadShackMessagesCount();
               }
            }
         }
         break;
      case DT_SHACK_SENDMSG:
      case DT_SHACK_READMSG:
      case DT_READMSG:
         {
            // do nothing
         }
         break;
      case DT_SHACK_DELETEMSG:
         {
            Refresh();
         }
         break;
      case DT_SENDMSG:
         {
            if(pDD->m_data != NULL)
            {
               const char *result = (const char *)pDD->m_data;
               if(result != NULL &&
                  *result != 0)
               {
                  if(_strnicmp(result,"error_communication_send",24) == 0)
                  {
                     m_pView->MessageBox(L"error_communication_send");
                  }
               }
            }
         }
         break;
      case DT_SHACK_MOD_CATEGORY_CHANGE:
         {
            RefreshThread(GetRootId(pDD->m_id), pDD->m_id);
         }
         break;
      }
      
      if(pDD->m_data != NULL)
      {
         free(pDD->m_data);
      }

      delete pDD;
   }

   m_bBusy = false;

   if(m_pView != NULL)
   {
      m_pView->OnSetCursor(m_pView, HTCLIENT, 0);
      m_pView->InvalidateEverything();
      theApp.SetStatusBarText(UCString(), m_pView);
   }
}

// CLampDoc

IMPLEMENT_DYNCREATE(CLampDoc, CDocument)

BEGIN_MESSAGE_MAP(CLampDoc, CDocument)
END_MESSAGE_MAP()


// CLampDoc construction/destruction

CLampDoc::CLampDoc()
{
   theApp.AddDocToList(this);

   m_pView = NULL;
   m_height = 0;
	SetDataType(DDT_EPICFAILD);
   m_initialpostid = 0;
   m_storyid = 0;
   m_pReplyDlg = NULL;
   m_page = 1;
   m_lastpage = 0;
   m_ThingsILOLD = false;
   m_ThingsIWrote = false;
   m_ThingsUserWrote = false;
   m_bScramblePath = false;
   m_bBusy = false;
   m_shackmsgtype = SMT_INBOX;
   m_bFetchingNextPage = false;

   m_lol_text = L"lol";
   m_inf_text = L"inf";
   m_unf_text = L"unf";
   m_tag_text = L"tag";
   m_wtf_text = L"wtf";

   m_widthofaverageprofilegroup = 0;

   m_link_color = RGB(174,174,155);
   m_image_link_color = RGB(154,154,195);

   m_spoilerbrush = ::CreateSolidBrush(theApp.GetSpoilerColor());

   if(theApp.GetBackground()->GetBitmap() != NULL)
   {
      m_backgroundbrush = ::CreatePatternBrush(theApp.GetBackground()->GetBitmap());
   }
   else
   {
      m_backgroundbrush = ::CreateSolidBrush(theApp.GetBackgroundColor());
   }

   if(theApp.GetRootPostBackground()->GetBitmap() != NULL)
   {
      m_rootbackgroundbrush = ::CreatePatternBrush(theApp.GetRootPostBackground()->GetBitmap());
   }
   else
   {
      m_rootbackgroundbrush = ::CreateSolidBrush(theApp.GetRootPostBackgroundColor());
   }

   if(theApp.GetPostBackground()->GetBitmap() != NULL)
   {
      m_replyexpandedbackgroundbrush = ::CreatePatternBrush(theApp.GetPostBackground()->GetBitmap());
   }
   else
   {
      m_replyexpandedbackgroundbrush = ::CreateSolidBrush(theApp.GetPostBackgroundColor());
   }

   m_roottoppen = ::CreatePen(PS_SOLID,0,theApp.GetPostEdgeColor());

   m_infpen = ::CreatePen(PS_SOLID,0,theApp.GetPostINFEdgeColor());
   m_nwspen = ::CreatePen(PS_SOLID,0,theApp.GetPostNWSEdgeColor());
   m_stupidpen = ::CreatePen(PS_SOLID,0,theApp.GetPostStupidEdgeColor());
   m_offtopicpen = ::CreatePen(PS_SOLID,0,theApp.GetPostOffTopicEdgeColor());
   m_politicalpen = ::CreatePen(PS_SOLID,0,theApp.GetPostPoliticalEdgeColor());
   m_nullpen = ::CreatePen(PS_NULL,0,0);

   m_branchpen = ::CreatePen(PS_SOLID,0,theApp.GetBranchColorShade(0,N_OLD));
   m_branchpenisnew = ::CreatePen(PS_SOLID,0,theApp.GetBranchColorShade(0,N_NEW));
   m_branchpenislast = ::CreatePen(PS_SOLID,0,theApp.GetBranchColorShade(0,N_LAST));
   m_normalfont = ::CreateFontW(theApp.GetFontHeight(),0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_DONTCARE,theApp.GetNormalFontName());
      
   m_miscfont = ::CreateFontW(theApp.GetMiscFontHeight(),0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_DONTCARE,theApp.GetNormalFontName());
   m_boldfont = ::CreateFontW(theApp.GetFontHeight(),0,0,0,FW_EXTRABOLD,0,0,0,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_DONTCARE,theApp.GetNormalFontName());
   m_pagefont = ::CreateFontW(-13,0,0,0,FW_EXTRABOLD,0,0,0,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_DONTCARE,theApp.GetNormalFontName());
   m_miscboldfont = ::CreateFontW(theApp.GetMiscFontHeight(),0,0,0,FW_EXTRABOLD,0,0,0,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_DONTCARE,theApp.GetNormalFontName());

   m_miscunderlinefont = ::CreateFontW(theApp.GetMiscFontHeight(),0,0,0,FW_NORMAL,0,1,0,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_DONTCARE,theApp.GetNormalFontName());
   m_normalunderlinefont = ::CreateFontW(theApp.GetFontHeight(),0,0,0,FW_NORMAL,0,1,0,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_DONTCARE,theApp.GetNormalFontName());
}

CLampDoc::~CLampDoc()
{
   theApp.RemoveDocFromList(this);

   if(m_backgroundbrush != NULL)
   {
      ::DeleteObject(m_backgroundbrush);
   }

   if(m_spoilerbrush != NULL)
   {
      ::DeleteObject(m_spoilerbrush);
   }

   if(m_rootbackgroundbrush != NULL)
   {
      ::DeleteObject(m_rootbackgroundbrush);
   }

   if(m_replyexpandedbackgroundbrush != NULL)
   {
      ::DeleteObject(m_replyexpandedbackgroundbrush);
   }

   if(m_infpen != NULL)
   {
      ::DeleteObject(m_infpen);
   }

   if(m_nwspen != NULL)
   {
      ::DeleteObject(m_nwspen);
   }
   
   if(m_stupidpen != NULL)
   {
      ::DeleteObject(m_stupidpen);
   }

   if(m_offtopicpen != NULL)
   {
      ::DeleteObject(m_offtopicpen);
   }

   if(m_politicalpen != NULL)
   {
      ::DeleteObject(m_politicalpen);
   }

   if(m_nullpen != NULL)
   {
      ::DeleteObject(m_nullpen);
   }

   if(m_roottoppen != NULL)
   {
      ::DeleteObject(m_roottoppen);
   }

   if(m_branchpen != NULL)
   {
      ::DeleteObject(m_branchpen);
   }

   if(m_branchpenisnew != NULL)
   {
      ::DeleteObject(m_branchpenisnew);
   }

   if(m_branchpenislast != NULL)
   {
      ::DeleteObject(m_branchpenislast);
   }

   if(m_normalfont != NULL)
   {
      ::DeleteObject(m_normalfont);
   }

   if(m_boldfont != NULL)
   {
      ::DeleteObject(m_boldfont);
   }

   if(m_pagefont != NULL)
   {
      ::DeleteObject(m_pagefont);
   }

   if(m_miscfont != NULL)
   {
      ::DeleteObject(m_miscfont);
   }

   if(m_miscunderlinefont != NULL)
   {
      ::DeleteObject(m_miscunderlinefont);
   }

   if(m_normalunderlinefont != NULL)
   {
      ::DeleteObject(m_normalunderlinefont);
   }

   if(m_miscboldfont != NULL)
   {
      ::DeleteObject(m_miscboldfont);
   }
     

   std::list<ChattyPost*>::iterator it = m_rootposts.begin();
   std::list<ChattyPost*>::iterator end = m_rootposts.end();
   while(it != end)
   {
      if((*it) != NULL)
      {
         delete (*it);
         (*it) = NULL;
      }
      it++;
   }
   m_rootposts.clear();
}

   

void CLampDoc::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU/* = TRUE*/)
{
   if(m_bScramblePath)
   {
      UCString nonsense = (unsigned int)this;

      m_strPathName = nonsense;
      m_bEmbedded = FALSE;
   }
   else
   {
      CDocument::SetPathName(lpszPathName, bAddToMRU);
   }
}

BOOL CLampDoc::OnNewDocument()
{
   BOOL result = TRUE;
	if(!CDocument::OnNewDocument())
   {
		result = FALSE;
   }
   
   if(result)
   {
      const UCChar *opendoc = theApp.GetOpenDocString(this);
      if(opendoc != NULL)
      {
         OnOpenDocumentImpl(opendoc);
      }
      else
      {
         /*
         SetDataType(DDT_STORY);
         m_title = L"latestchatty";
         MySetTitle(m_title);
         ReadLatestChatty();
         */
         OnOpenDocumentImpl(L"LATESTCHATTY");
      }
   }
   
   return result;
}

BOOL CLampDoc::OnOpenDocument( LPCTSTR lpszPathName )
{
   assert(0);
   return OnOpenDocumentImpl( lpszPathName );
}

BOOL CLampDoc::OnOpenDocumentImpl( LPCTSTR lpszPathName )
{
   BOOL result = TRUE;

   UCString rootid;

   bool isstory = false;
   bool islol = false;
   bool issearch = false;
   bool isshackmsg = false;
   bool isprofile = false;

   bool bAllowPreLoadingOfThread = true;

   const UCChar *start = lpszPathName;
   const UCChar *end = lpszPathName + wcslen(lpszPathName);
   const UCChar *newstart = NULL;

   const UCChar *http = wcsstr(start, L"http:\\www.shacknews.com\\");
   if(http != NULL)
   {
      start = http + 24;
   }
   else
   {
      http = wcsstr(start, L"http://www.shacknews.com/");
      if(http != NULL)
      {
         start = http + 25;
      }
   }

   if(end - start > 7 &&
      _wcsnicmp(start,L"search?",7) == 0)
   {
      m_search_author = L"";
      m_search_parent_author = L"";
      m_search_terms = L"";

      const UCChar *work = wcsstr(start, L"chatty_term=");
      if(work != NULL)
      {
         work += 12;
         while(work < end && *work != L'&'){m_search_terms += *work;work++;}
         m_search_terms.ReplaceAll(L'+',L' ');
      }
      else
      {
         work = wcsstr(start, L"q=");
         if(work != NULL)
         {
            work += 2;
            while(work < end && *work != L'&'){m_search_terms += *work;work++;}
            m_search_terms.ReplaceAll(L'+',L' ');
         }
      }

      work = wcsstr(start, L"chatty_user=");
      if(work != NULL)
      {
         work += 12;
         while(work < end && *work != L'&'){m_search_author += *work;work++;}
         m_search_author.ReplaceAll(L'+',L' ');
      }

      work = wcsstr(start, L"chatty_author=");
      if(work != NULL)
      {
         work += 14;
         while(work < end && *work != L'&'){m_search_parent_author += *work;work++;}
         m_search_parent_author.ReplaceAll(L'+',L' ');
      }

      if(!m_search_author.IsEmpty() &&
         m_search_parent_author.IsEmpty() &&
         m_search_terms.IsEmpty())
      {
         m_title = m_search_author;
         m_title += L"'s Comments";
      }
      else if(m_search_author.IsEmpty() &&
              !m_search_parent_author.IsEmpty() &&
              m_search_terms.IsEmpty())
      {
         m_title = L"Replies to ";
         m_title += m_search_parent_author;            
      }
      else if(m_search_author.IsEmpty() &&
              m_search_parent_author.IsEmpty() &&
              !m_search_terms.IsEmpty())
      {
         m_title = m_search_terms;
      }
      else
      {
         m_title = L"Custom Search";
      }
               
      MySetTitle(m_title);

      m_bScramblePath = true;

      issearch = true;
      
   }
   else if(end - start > 14 &&
      _wcsnicmp(start,L"story?id=",10) == 0)
   {
      // todo: alternate story ids
      const UCChar *work = start + 10;
      UCString storyid;

      while(work < end && 
            iswdigit(*work))
      {
         storyid += *work;
         work++;
      }

      m_storyid = storyid;
      m_page = 1;
      m_lastpage = 1;
      isstory = true;
   }
   else if(_wcsnicmp(start,L"PROFILE:",8) == 0)
   {
      m_profile_user = start + 8;
      m_profile_user.Replace(L"%20",L" ");
      m_storyid = 0;
      m_page = 1;
      m_lastpage = 1;
      isprofile = true;

      m_title = m_profile_user;
      m_title += L"'s Profile";
      MySetTitle(m_title);

      m_link_color = RGB(255,0,0);
      m_image_link_color = RGB(192,0,64);
   }
   else if(_wcsnicmp(start,L"http://chattyprofil.es/p/",25) == 0)
   {
      m_profile_user = start + 25;
      m_profile_user.Replace(L"%20",L" ");
      m_storyid = 0;
      m_page = 1;
      m_lastpage = 1;
      isprofile = true;

      m_title = m_profile_user;
      m_title += L"'s Profile";
      MySetTitle(m_title);

      m_link_color = RGB(255,0,0);
      m_image_link_color = RGB(192,0,64);
   }
   else if(_wcsnicmp(start,L"LATESTCHATTY",12) == 0)
   {
      m_storyid = 0;
      m_page = 1;
      m_lastpage = 1;
      isstory = true;
   }
   else if((newstart = wcsstr(start,L"?id=")) != NULL)
   {
      const UCChar *work = newstart + 4;

      while(work < end && 
            iswdigit(*work))
      {
         rootid += *work;
         work++;
      }

      m_initialpostid = rootid;

      bAllowPreLoadingOfThread = false;
   }
   else if(_wcsnicmp(start,L"chatty\\",7) == 0)
   {
      const UCChar *work = start + 7;

      while(work < end && 
            iswdigit(*work))
      {
         rootid += *work;
         work++;
      }

      m_initialpostid = rootid;

      bAllowPreLoadingOfThread = false;
   }
   else if(_wcsnicmp(start,L"LOL",3) == 0)
   {
      const UCChar *pCmd = start;
      m_page = m_lastpage = 1;
      islol = true;
      pCmd += 3;
      if(end - pCmd > 3 &&
        _wcsnicmp(pCmd,L"POP",3) == 0)
      {
         pCmd += 3;
         m_loltag = pCmd;
         m_loltag.MakeLower();
      }
      else if(end - pCmd >= 5 &&
        _wcsnicmp(pCmd,L"ILOLD",5) == 0)
      {
         pCmd += 5;
         m_loltag = L"";
         m_ThingsILOLD = true;
      }
      else if(end - pCmd >= 6 &&
        _wcsnicmp(pCmd,L"IWROTE",6) == 0)
      {
         pCmd += 6;
         m_loltag = L"";
         m_ThingsIWrote = true;
      }   
      else if(end - pCmd >= 6 &&
        _wcsnicmp(pCmd,L"THEYWROTE",9) == 0)
      {
         pCmd += 9;
         m_loltag = L"";
         m_ThingsUserWrote = true;
         m_profile_user = pCmd;
      }
   }
   else if(_wcsnicmp(start,L"MYCOMMENTS",10) == 0)
   {
      m_search_author = theApp.GetUsername();
      m_search_parent_author = L"";
      m_search_terms = L"";

      m_title = L"My Comments";
      MySetTitle(m_title);

      issearch = true;
   }
   else if(_wcsnicmp(start,L"REPLIESTOME",11) == 0)
   {
      m_search_author = L"";
      m_search_parent_author = theApp.GetUsername();
      m_search_terms = L"";

      m_title = L"Replies To Me";
      MySetTitle(m_title);

      issearch = true;
   }
   else if(_wcsnicmp(start,L"VANITYSEARCH",12) == 0)
   {
      m_search_author = L"";
      m_search_parent_author = L"";
      m_search_terms = theApp.GetUsername();

      m_title = L"Vanity Search";
      MySetTitle(m_title);

      issearch = true;
   }
   else if(_wcsnicmp(start,L"CUSTOMSEARCH:",13) == 0)
   {
      const UCChar *work = start + 13;
      m_search_author = L"";
      m_search_parent_author = L"";
      m_search_terms = L"";
      while(work < end && *work != L':'){m_search_author += *work;work++;}
      if(work < end && *work == L':')work++;
      while(work < end && *work != L':'){m_search_parent_author += *work;work++;}
      if(work < end && *work == L':')work++;
      while(work < end && *work != L':'){m_search_terms += *work;work++;}

      char *enc = url_decode(m_search_author.str8());
      m_search_author = enc;
      free(enc);

      enc = url_decode(m_search_parent_author.str8());
      m_search_parent_author = enc;
      free(enc);

      enc = url_decode(m_search_terms.str8());
      m_search_terms = enc;
      free(enc);

      //theApp.GetLastSearchParms(m_search_author, m_search_parent_author, m_search_terms);
      
      if(!m_search_author.IsEmpty() &&
         m_search_parent_author.IsEmpty() &&
         m_search_terms.IsEmpty())
      {
         m_title = m_search_author;
         m_title += L"'s Comments";
      }
      else if(m_search_author.IsEmpty() &&
              !m_search_parent_author.IsEmpty() &&
              m_search_terms.IsEmpty())
      {
         m_title = L"Replies to ";
         m_title += m_search_parent_author;            
      }
      else if(m_search_author.IsEmpty() &&
              m_search_parent_author.IsEmpty() &&
              !m_search_terms.IsEmpty())
      {
         m_title = m_search_terms;
      }
      else
      {
         m_title = L"Custom Search";
      }
               
      MySetTitle(m_title);

      m_bScramblePath = true;

      issearch = true;
   }
   else if(_wcsnicmp(start,L"SHACKMSG_",9) == 0)
   {
      const UCChar *work = start + 9;
      if(end - work >= 5 &&
        _wcsnicmp(work,L"INBOX",5) == 0)
      {
         work += 5;
         m_title = L"Inbox";
         m_shackmsgtype = SMT_INBOX;
      }
      else if(end - work >= 6 &&
        _wcsnicmp(work,L"OUTBOX",6) == 0)
      {
         work += 6;
         m_title = L"Sent Msgs";
         m_shackmsgtype = SMT_OUTBOX;
      }
      else if(end - work >= 7 &&
        _wcsnicmp(work,L"ARCHIVE",7) == 0)
      {
         work += 7;
         m_title = L"Archived Msgs";
         m_shackmsgtype = SMT_ARCHIVE;
      }
      m_page = 1;
      MySetTitle(m_title);
      isshackmsg = true;
   }
   else
   {
      const UCChar *thisid;
      const UCChar *work = end - 1;
      
      while(work >= start &&
            iswdigit(*work))
      {
         thisid = work;
         work--;
      }
      
      rootid = thisid;

      if(work >= start &&
         *work == L'_')
      {
         m_initialpostid = rootid;
         work--;
         int numchars = 0;
         // we have a view id
         // now find the root id
         while(work >= start &&
               iswdigit(*work))
         {
            thisid = work;
            work--;
            numchars++;
         }
         
         rootid = L"";
         rootid.AppendUnicodeString(thisid, numchars);
      }
   }

   if(islol)
   {
      SetDataType(DDT_LOLS);
      ReadLOL();
   }
   else if(isprofile)
   {
      SetDataType(DDT_PROFILE);
      GetProfile();
   }
   else if(issearch)
   {
      SetDataType(DDT_SEARCH);
      PerformSearch();
   }
   else if(isstory)
   {
      SetDataType(DDT_STORY);
      m_title = L"LatestChatty";
      MySetTitle(m_title);
      ReadLatestChatty();
   }
   else if(isshackmsg)
   {
      SetDataType(DDT_SHACKMSG);
      GetShackMessages();
   }
   else
   {
      SetDataType(DDT_THREAD);
      m_title = L"loading";
      MySetTitle(m_title);

      // see if we can steal the existing thread from someone first.
      if(bAllowPreLoadingOfThread)
      {
         ChattyPost *pExistingPost = theApp.FindFromAnywhere(rootid);
         if(pExistingPost != NULL)
         {
            rootid = pExistingPost->GetId();
            ChattyPost *post = new ChattyPost();
            if(post != NULL)
            {
               if(theApp.UseShack())
               {
                  post->SetNewness(N_OLD);
               }
               m_rootposts.push_back(post);
               post->ReadPost(pExistingPost,this);
               post->Expand();
               post->UnShowAsTruncated();
               
               ChattyPost *refresh_post = post->FindChild(m_initialpostid);
               if(refresh_post != NULL)
               {
                  refresh_post->SetRefreshing(true);
                  if(m_pView)
                     m_pView->InvalidateEverything();
               }
            }
         }
      }

      RefreshThread(rootid, m_initialpostid,true);
   }
   
   return result;
}

void CLampDoc::GetProfile()
{
   std::list<ChattyPost*>::iterator it = m_rootposts.begin();
   std::list<ChattyPost*>::iterator end = m_rootposts.end();
   while(it != end)
   {
      if((*it) != NULL)
      {
         delete (*it);
         (*it) = NULL;
      }
      it++;
   }
   m_rootposts.clear();

   // http://chattyprofil.es/api/profile/crasterimage

   UCString path = L"/api/profile/";
   path += m_profile_user;
   path.Replace(L" ",L"%20");

   StartDownload(theApp.GetProfileHost(),
                 path,
                 DT_GET_PROFILE,
                 0);
}

void CLampDoc::PerformSearch()
{
   std::list<ChattyPost*>::iterator it = m_rootposts.begin();
   std::list<ChattyPost*>::iterator end = m_rootposts.end();
   while(it != end)
   {
      if((*it) != NULL)
      {
         delete (*it);
         (*it) = NULL;
      }
      it++;
   }
   m_rootposts.clear();

   theApp.SetLastSearchParms(m_search_author, m_search_parent_author, m_search_terms);

   if(theApp.UseShack())
   {
      // http://www.shacknews.com/search?chatty=1&type=4&chatty_term=lamp&chatty_user=CRasterImage&chatty_author=electroly&chatty_filter=all

      UCString path = L"/search?chatty=1&type=4&chatty_term=";
      UCString temp = m_search_terms;
      temp.ReplaceAll(L' ',L'+');
      path += temp;
      path += L"&chatty_user=";
      temp = m_search_author;
      temp.ReplaceAll(L' ',L'+');
      temp.Replace(L"&",L"%26");
      path += temp;
      path += L"&chatty_author=";
      temp = m_search_parent_author;
      temp.ReplaceAll(L' ',L'+');
      temp.Replace(L"&",L"%26");
      path += temp;
      path += L"&chatty_filter=all&result_sort=postdate_desc&page=";
      path += m_page;

      StartDownload(L"www.shacknews.com",
                    path,
                    DT_SHACK_SEARCH,
                    0);
   }
   else
   {
      UCString path = L"/search.xml?author=";
      path += m_search_author;
      path += L"&parent_author=";
      path += m_search_parent_author;
      path += L"&terms=";
      path += m_search_terms;
      
      // these don't work
      //path += L"&SearchType=n";
      //path += L"&SearchType=i";

      StartDownload(L"shackapi.stonedonkey.com",
                    path,
                    DT_SEARCH,
                    0);
   }
}

void CLampDoc::GetShackMessages()
{
   if(theApp.UseShack())
   {
      std::list<ChattyPost*>::iterator it = m_rootposts.begin();
      std::list<ChattyPost*>::iterator end = m_rootposts.end();
      while(it != end)
      {
         if((*it) != NULL)
         {
            delete (*it);
            (*it) = NULL;
         }
         it++;
      }
      m_rootposts.clear();

      UCString path = L"/messages/";

      switch(m_shackmsgtype)
      {
      case SMT_INBOX:   path += L"inbox"; break;
      case SMT_OUTBOX:  path += L"sent"; break;
      case SMT_ARCHIVE: path += L"sent"; break;
      }
      path += L"?page=";
      path += m_page;

      StartDownload(L"www.shacknews.com",
                    path,
                    DT_SHACK_SHACKMSG,
                    0,
                    0,
                    0,
                    NULL,
                    theApp.GetUsername(),
                    theApp.GetPassword(), 
                    true);
   }
   else
   {
      UCString path = L"/Messages/?box=";
      
      switch(m_shackmsgtype)
      {
      case SMT_INBOX:   path += L"inbox"; break;
      case SMT_OUTBOX:  path += L"outbox"; break;
      case SMT_ARCHIVE: path += L"outbox"; break;
      }
      path += L"&page=";
      path += m_page;
      
   /*   
       * username - the users shack username.
       * password - the users shack password.
       * box - the mail box within Shack Messages [ Default: inbox ]
             o inbox = users current shack messages
             o outbox = users outgoing shack messages
             o archive = users archived shack messages
       * page - the page of results you wish to view [ Default: 1 ]
   */

      StartDownload(L"shackapi.stonedonkey.com",
                    path,
                    DT_SHACKMSG,
                    0,
                    0,
                    0,
                    NULL,
                    theApp.GetUsername(),
                    theApp.GetPassword(), 
                    true);
   }
}

void CLampDoc::MarkShackMessageRead(unsigned int id)
{
   if(theApp.UseShack())
   {
      /*
      $url = 'http://www.shacknews.com/messages/read';
      $postArgs = 
         'mid=' . urlencode($id);

      $this->userDownload($url, $username, $password, $postArgs);
      */
      UCString path = L"/messages/read";
      
      UCString postdata = L"mid=";
      postdata += id;

      StartDownload(L"www.shacknews.com",
                    path,
                    DT_SHACK_READMSG,
                    0,
                    0,
                    0,
                    postdata,
                    theApp.GetUsername(),
                    theApp.GetPassword());
   }
   else
   {
      // "http://shackapi.stonedonkey.com/messages/read/?username=" + _login +"&password=" + _password +"&messageid=" + 
      UCString path = L"/Messages/read/?messageid=";
      
      path += id;

      StartDownload(L"shackapi.stonedonkey.com",
                    path,
                    DT_READMSG,
                    0,
                    0,
                    0,
                    NULL,
                    theApp.GetUsername(),
                    theApp.GetPassword());
   }
}

void CLampDoc::DeleteShackMessage(unsigned int id)
{
   if(theApp.UseShack())
   {
      /*
      UCString path = L"/messages/delete";

      UCString postdata = L"mid=";
      postdata += id;

      if(m_shackmsgtype == SMT_INBOX)
      {
         postdata += L"&type=inbox";
      }
      else
      {
         postdata += L"&type=sent";
      }
      */

      // /messages/action?messages%5B%5D=1603104&page_type=inbox 

      UCString path = L"/messages/action?messages%5B%5D=";

      path += id;

      if(m_shackmsgtype == SMT_INBOX)
      {
         path += L"&page_type=inbox";
      }
      else
      {
         path += L"&page_type=sent";
      }

      StartDownload(L"www.shacknews.com",
                    path,
                    DT_SHACK_DELETEMSG,
                    0,
                    0,
                    0,
                    0,
                    theApp.GetUsername(),
                    theApp.GetPassword());
   }
   else
   {
   }
}

void CLampDoc::SendMessage(const UCString &to, const UCString &subject, const UCString &shackmsg)
{
   if(theApp.UseShack())
   {
      UCString path = L"/messages/send";   

      UCString post_data;

      post_data += L"uid=";
      post_data += theApp.GetUserID();

      post_data += L"&to=";
      char *enc_to = url_encode((char*)to.str8(false,CET_UTF8));
      post_data += enc_to;
      free(enc_to);

      post_data += L"&subject=";
      UCString temp = subject;
      temp.ReplaceAll(L'<',0x02C2);
      temp.ReplaceAll(L'>',0x02C3);
      char *enc_subject = url_encode((char*)temp.str8(false,CET_UTF8));
      post_data += enc_subject;
      free(enc_subject);

      post_data += L"&message=";
      temp = shackmsg;
      temp.ReplaceAll(L'<',0x02C2);
      temp.ReplaceAll(L'>',0x02C3);
      char *enc_shackmsg = url_encode((char*)temp.str8(false,CET_UTF8));
      post_data += enc_shackmsg;
      free(enc_shackmsg);
      
      StartDownload(L"www.shacknews.com",
                    path,
                    DT_SHACK_SENDMSG,
                    0,
                    0,
                    0,
                    post_data,
                    theApp.GetUsername(),
                    theApp.GetPassword());
   }
   else
   {
      UCString path = L"/messages/send/";   

      UCString post_data;

      UCString userlower = theApp.GetUsername();
      userlower.MakeLower();
      
      post_data += L"username=";
      char *enc_username = url_encode((char*)userlower.str8(false,CET_UTF8));
      post_data += enc_username;

      post_data += L"&password=";
      char *enc_password = url_encode((char*)theApp.GetPassword().str8(false,CET_UTF8));
      post_data += enc_password;

      post_data += L"&subject=";

      UCString temp = subject;
      temp.ReplaceAll(L'<',0x02C2);
      temp.ReplaceAll(L'>',0x02C3);

      char *enc_subject = url_encode((char*)temp.str8(false,CET_UTF8));
      post_data += enc_subject;

      post_data += L"&to=";
      char *enc_to = url_encode((char*)to.str8(false,CET_UTF8));
      post_data += enc_to;
      
      post_data += L"&body=";

      temp = shackmsg;
      temp.ReplaceAll(L'<',0x02C2);
      temp.ReplaceAll(L'>',0x02C3);

      char *enc_shackmsg = url_encode((char*)temp.str8(false,CET_UTF8));
      post_data += enc_shackmsg;
      
      StartDownload(L"shackapi.stonedonkey.com",
                    path,
                    DT_SENDMSG,
                    0,
                    0,
                    0,
                    post_data,
                    userlower,
                    theApp.GetPassword());

      free(enc_username);
      free(enc_password);
      free(enc_to);
      free(enc_subject);
      free(enc_shackmsg);
   }
}

void CLampDoc::SetCategory_Mod(unsigned int id, postcategorytype type)
{
   if(theApp.UseShack())
   {
      ChattyPost *post = FindPost(id);
      if(post != NULL)
      {
         ChattyPost *root = post->GetRoot();
         if(root != NULL)
         {
            // http://www.shacknews.com/mod_chatty.x?root=[root post id]&post_id=[post id]&mod_type_id=[x] 

            UCString path = L"/mod_chatty.x?root=";
            
            path += root->GetId();
            path += L"&post_id=";
            path += post->GetId();
            path += L"&mod_type_id=";

            UCString msg;
            switch(type)
            {
            case PCT_NORMAL:        path += "5";break;
            case PCT_INF:           path += "1";break;
            case PCT_NWS:           path += "2";break;
            case PCT_STUPID:        path += "3";break;
            case PCT_OFFTOPIC:      path += "4";break;
            case PCT_POLITICAL:     path += "9";break;
            case PCT_NUKED:         path += "8";break;
            }
            
            StartDownload(L"www.shacknews.com",
                          path,
                          DT_SHACK_MOD_CATEGORY_CHANGE,
                          id,
                          0,
                          0,
                          NULL,
                          theApp.GetUsername(),
                          theApp.GetPassword());
         }
      }
   }
}

void CLampDoc::ReadLOL()
{
   UCString LOLup = m_loltag;
   LOLup.MakeUpper();

   UCString path = L"/greasemonkey/shacklol/";

   if(m_ThingsILOLD)
   {
      m_title = L"Things I LOL'd";
      // http://lmnopc.com/greasemonkey/shacklol/user.php?loldby=[username]&sort_by=date&page=1
      path += L"user.php?loldby=";
      path += theApp.GetUsername();
      path += L"&sort_by=date&page=";
      path += m_page;
   }
   else if(m_ThingsIWrote)
   {
      m_title = L"Things I Wrote";
      // http://lmnopc.com/greasemonkey/shacklol/user.php?authoredby=[username]&sort_by=date&page=1
      path += L"user.php?authoredby=";
      path += theApp.GetUsername();
      path += L"&sort_by=date&page=";
      path += m_page;
   }
   else if(m_ThingsUserWrote)
   {
      m_title = L"Things ";
      m_title += m_profile_user;
      m_title += L" Wrote";
      // http://lmnopc.com/greasemonkey/shacklol/user.php?authoredby=[username]&sort_by=date&page=1
      path += L"user.php?authoredby=";
      path += m_profile_user;
      path += L"&sort_by=date&page=";
      path += m_page;
   }
   else
   {
      m_title = L"Popular ";
      m_title += LOLup;
      m_title += L"\'s";
      // http://lmnopc.com/greasemonkey/shacklol/?tag=lol&sort_by=lols
      path += L"?tag=";
      path += m_loltag;
      path += L"&sort_by=lols";
   }
   MySetTitle(m_title);

   StartDownload(theApp.GetLolHostName(),
                 path,
                 DT_LOL,
                 0);
}


void CLampDoc::ProcessLOLData(char *data, int datasize)
{
   const char *start = data;
   const char *end = start + datasize;
   const char *work = start;

   UCString LOLup;

   while(work < end)
   {
      UCString id;
      UCString lolcount;
      UCString author;
      UCString body;
      std::vector<UCString> whololed;
      UCString loltag;

      work = strstr(work,"<li id=\"thread_");

      if(work != NULL && work < end)
      {
         work += 15;
         while(isdigit(*work) && work < end)
         {
            id += *work;
            work++;
         }

         work = strstr(work,"class=\"permalink\" title=\"Permalink\">");

         if(work != NULL && work < end)
         {
            work += 36;
            while(isdigit(*work) && work < end)
            {
               lolcount += *work;
               work++;
            }

            work = strstr(work,"\' d *");

            if(work != NULL && work < end)
            {
               work -= 3;
               
               loltag.AppendEncodedString(work,3,CET_UTF8);

               //
                    if(loltag == L"lol") theApp.AddLOL_LOL((unsigned int)id, (unsigned int)lolcount);
               else if(loltag == L"inf") theApp.AddLOL_INF((unsigned int)id, (unsigned int)lolcount);
               else if(loltag == L"unf") theApp.AddLOL_UNF((unsigned int)id, (unsigned int)lolcount);
               else if(loltag == L"tag") theApp.AddLOL_TAG((unsigned int)id, (unsigned int)lolcount);
               else if(loltag == L"wtf") theApp.AddLOL_WTF((unsigned int)id, (unsigned int)lolcount);               
               //
               
               work = strstr(work,"<span class=\"post-author\">By <a href=\"");

               if(work != NULL && work < end)
               {
                  work += 38;
                  work = strstr(work,"\">");

                  if(work != NULL && work < end)
                  {
                     work += 2;

                     while(work < end && 
                           *work != '<')
                     {
                        author += *work;
                        work++;
                     }
                     work++;
                     
                     work = strstr(work,"<div class=\"postbody\">");

                     if(work != NULL && work < end)
                     {
                        work += 22;

                        const char *endsection = strstr(work,"</div>");

                        if(endsection != NULL && endsection < end)
                        {
                           /*
                           while(work < endsection)
                           {
                              body += *work;
                              work++;
                           }
                           */
                           body.AppendEncodedString(work,endsection - work);
                           work = endsection;

                           work = strstr(work,"<div class=\"loldby\">");

                           if(work != NULL && work < end)
                           {
                              work += 20;

                              endsection = strstr(work,"</div>");

                              if(endsection != NULL && endsection < end)
                              {
                                 while(work < endsection)
                                 {
                                    const char *loler = strstr(work,"<a href=\"user.php?loldby=");
                                    if(loler != NULL && loler < endsection)
                                    {
                                       loler = strstr(loler,"\">");
                                       if(loler != NULL && loler < endsection)
                                       {
                                          loler += 2;
                                          work = strstr(loler,"</a>");
                                          if(work != NULL && work < endsection)
                                          {
                                             UCString wholol;
                                             while(loler < work)
                                             {
                                                wholol += *loler;
                                                loler++;
                                             }

                                             wholol.Replace(L"<strong>",L"");
                                             wholol.Replace(L"</strong>",L"");

                                             whololed.push_back(wholol);
                                          }
                                          else work = endsection;
                                       }
                                       else work = endsection;
                                    }
                                    else work = endsection;
                                 }

                                 body += L"<br/><br/>";

                                      if(loltag == L"lol") body += L"<span class=\"jt_orange\">";
                                 else if(loltag == L"inf") body += L"<span class=\"jt_blue\">";
                                 else if(loltag == L"unf") body += L"<span class=\"jt_red\">";
                                 else if(loltag == L"tag") body += L"<span class=\"jt_green\">";
                                 else if(loltag == L"wtf") body += L"<span class=\"jt_purple\">";

                                 LOLup = loltag;
                                 LOLup.MakeUpper();

                                 body += L"<b>";
                                 body += lolcount;
                                 body += L" ";
                                 body += LOLup;
                                 body += L"\'d</b>";

                                 if(whololed.size() > 0)
                                 {
                                    body += L"<br/><span class=\"jt_sample\">By: ";
                                    for(size_t i = 0; i < whololed.size(); i++)
                                    {
                                       if(i > 0)
                                       {
                                          body += L", ";
                                       }

                                       if(whololed[i] == theApp.GetUsername())
                                       {
                                          if(loltag == L"inf") body += L"<span class=\"jt_purple\">";
                                          else body += L"<span class=\"jt_blue\">";
                                       
                                          body += whololed[i];
                                                                              
                                          body += L"</span>";
                                       }
                                       else
                                       {
                                          body += whololed[i];
                                       }
                                    }
                                    body += L"</span>";
                                 }
                                 body += L"</span>";

                                 // create record for lol and post
                                 ChattyPost *post = new ChattyPost();
                                 if(post != NULL)
                                 {
                                    m_rootposts.push_back(post);
                                    post->ReadLOL(this,id,lolcount,author,body);
                                    if(post->IsFiltered())
                                    {
                                       m_rootposts.pop_back();
                                       delete post;
                                    }
                                 }
                              }
                           }
                        }
                        else work = end;
                     }
                     else work = end;
                  }
                  else work = end;
               }
               else work = end;
            }
            else work = end;
         }
         else work = end;
      }
      else work = end;
   }

   if(m_ThingsILOLD ||
      m_ThingsIWrote ||
      m_ThingsUserWrote)
   {
      // find the last page
      work = strstr(start,"\">&raquo;</a> </p></div>");
      if(work != NULL && work < end)
      {
         const char *pagestr = work - 1;
         int count = 0;
         while(pagestr > start &&
               isdigit(*pagestr))
         {
            count++;
            pagestr--;
         }
         pagestr++;
         UCString lastpagestr;
         lastpagestr.AppendEncodedString(pagestr,count,CET_UTF8);
         m_lastpage = lastpagestr;
      }
   }

   GenerateRedirectedIDs();
}

void CLampDoc::ReadLatestChatty()
{
   if(theApp.UseShack())
   {
      UCString story = L"/chatty?page=";
      story += m_page;

      StartDownload(L"www.shacknews.com",
                    story,
                    DT_SHACK_CHATTY,
                    0);
   }
   else
   {
      UCString story = L"/";
         
      if(m_page > 1)
      {
         size_t actualpage = m_page;

         if(theApp.IsDoublePageStory())
         {
            actualpage = ((m_page - 1) * 2) + 1;
         }
         story += L"17.";
         story += actualpage;
         story += L".xml";
      }
      else
      {
         story += L"index.xml";
      }
   
      StartDownload(L"shackapi.stonedonkey.com",
                    story,
                    DT_STORY,
                    0);
   }
}

void CLampDoc::ReadLatestChattyPart2()
{
   UCString story = L"/";
   
   size_t actualpage = ((m_page - 1) * 2) + 2;

   story += L"17.";
   story += actualpage;
   story += L".xml";

   StartDownload(L"shackapi.stonedonkey.com",
                 story,
                 DT_STORY_2,
                 0);
}


bool CLampDoc::Refresh()
{
   bool bResetPos = false;

   switch(m_datatype)
   {
   case DDT_EPICFAILD:
      break;
   case DDT_STORY:
      {
         if(theApp.UseShack() && theApp.InfinatePaging())
         {
            m_page = 1;
         }
         ReadLatestChatty();
         bResetPos = true;
      }
      break;
   case DDT_SHACKMSG:
      {
         GetShackMessages();
      }
      break;
   case DDT_THREAD:
      if(m_rootposts.size() > 0)
      {
         RefreshThread((*m_rootposts.begin())->GetId(),m_pView->GetCurrentId());
      }
      break;
   case DDT_LOLS:
      {
         std::list<ChattyPost*>::iterator it = m_rootposts.begin();
         std::list<ChattyPost*>::iterator end = m_rootposts.end();
         while(it != end)
         {
            if((*it) != NULL)
            {
               delete (*it);
               (*it) = NULL;
            }
            it++;
         }
         m_rootposts.clear();
         ReadLOL();
         bResetPos = true;
      }
      break;
   case DDT_PROFILE:
      {
         std::list<ChattyPost*>::iterator it = m_rootposts.begin();
         std::list<ChattyPost*>::iterator end = m_rootposts.end();
         while(it != end)
         {
            if((*it) != NULL)
            {
               delete (*it);
               (*it) = NULL;
            }
            it++;
         }
         m_rootposts.clear();
         GetProfile();
         bResetPos = true;
      }
      break;
   case DDT_SEARCH:
      PerformSearch();
      bResetPos = true;
      break;
   }

   return bResetPos;
}

bool CLampDoc::RefreshThread(unsigned int id, unsigned int refresh_id, bool bStarting/* = false*/, unsigned int reply_to_id/* = 0*/)
{
   ChattyPost *pPost = FindPost(refresh_id);
   if(pPost != NULL)
   {
      pPost->SetRefreshing(true);
      if(m_pView)
         m_pView->InvalidateEverything();
   }

   if(theApp.UseShack())
   {
      UCString story = L"/chatty?id=";
      story += id;
      story += L"#itemanchor_";
      story += id;

      StartDownload(L"www.shacknews.com",
                    story,
                    DT_SHACK_THREAD,
                    id,
                    refresh_id,
                    reply_to_id);
   }
   else
   {
      UCString path = L"/";
      path += L"thread/";
      path += id;
      path += L".xml";

      DownloadType dt = DT_THREAD;
      if(bStarting)
      {
         dt = DT_THREAD_START;
      }

      StartDownload(L"shackapi.stonedonkey.com",
                    path,
                    dt,
                    id,
                    refresh_id,
                    reply_to_id);
   }

   return true;
}

void CLampDoc::RefreshAllRoots()
{
   std::list<ChattyPost*>::iterator it = m_rootposts.begin();
   std::list<ChattyPost*>::iterator end = m_rootposts.end();

   while(it != end)
   {
      if(!(*it)->IsCollapsed() &&
         !(*it)->IsJustText())
      {
         RefreshThread((*it)->GetId(),(*it)->GetId(),true);
      }
      it++;
   }
}

ChattyPost *CLampDoc::GetRootPost(unsigned int id)
{
   ChattyPost *result = NULL;
   std::list<ChattyPost*>::iterator it = m_rootposts.begin();
   std::list<ChattyPost*>::iterator end = m_rootposts.end();
   while(it != end)
   {
      if((*it)->GetId() == id)
      {
         result = (*it);
         break;
      }
      it++;
   }
   return result;
}

unsigned int CLampDoc::GetRootId(unsigned int id)
{
   std::list<ChattyPost*>::iterator it = m_rootposts.begin();
   std::list<ChattyPost*>::iterator end = m_rootposts.end();
   while(it != end)
   {
      if((*it)->GetId() == id)
      {
         return (*it)->GetId();
      }
      else
      {
         ChattyPost *pPost = (*it)->FindChild(id);
         if(pPost != NULL)
         {
            return (*it)->GetId();
         }
      }
      it++;
   }
   return NULL;
}

bool CLampDoc::PostReply(const UCString &replytext, unsigned int to_id)
{
   theApp.SetLastPost(replytext);

   if(theApp.UseShack())
   {

      UCString story = L"/post_chatty.x";

      UCString postdata = L"parent_id=";

      UCString content_type_id = L"17";

      if(to_id != 0)
      {
         postdata += to_id;
         ChattyPost *parent = FindPost(to_id);
         if(parent != NULL)
         {
            parent = parent->GetRoot();
            if(parent->GetAuthor() == L"Shacknews")
            {
               // this is a weird hack stonedonkey uses to 
               // get around problems posting replies to a 
               // thread started by the Shacknews bot.
               content_type_id = L"2";
            }
         }
      }
      
      postdata += L"&content_type_id=";
      postdata += content_type_id;
      postdata += L"&content_id=17&page=&parent_url=/chatty&body=";

      UCString temp = replytext;
      temp.ReplaceAll(L'<',0x02C2);
      temp.ReplaceAll(L'>',0x02C3);
      temp.Replace(L"\n",L"\r\n");

      char *enc_reply = url_encode((char*)temp.str8(false,CET_UTF8));
      postdata += enc_reply;
      free(enc_reply);
      
      StartDownload(L"www.shacknews.com",
                    story,
                    DT_SHACK_POST,
                    to_id,
                    0,
                    0,
                    postdata,
                    theApp.GetUsername(),
                    theApp.GetPassword());
   }
   else
   {
      ChattyPost *parent = NULL;

      if(to_id != 0)
      {
         parent = FindPost(to_id);
         if(parent != NULL)
         {
            parent = parent->GetRoot();
         }
      }

      UCString path = L"/post/";

      UCString post_data = L"parent_id=";
      post_data += to_id; // id of 0 means new post

      if(parent != NULL &&
         parent->GetAuthor() == L"Shacknews")
      {
         // this is a weird hack stonedonkey uses to 
         // get around problems posting replies to a 
         // thread started by the Shacknews bot.
         post_data += L"&content_type_id=2";
      }

      post_data += L"&body=";

      UCString temp = replytext;
      temp.ReplaceAll(L'<',0x02C2);
      temp.ReplaceAll(L'>',0x02C3);

      char *enc_reply = url_encode((char*)temp.str8(false,CET_UTF8));
      post_data += enc_reply;
      free(enc_reply);
      
      StartDownload(L"shackapi.stonedonkey.com",
                    path,
                    DT_POST,
                    to_id,
                    0,
                    0,
                    post_data,
                    theApp.GetUsername(),
                    theApp.GetPassword());
   }

   return true;
}

// CLampDoc diagnostics

#ifdef _DEBUG
void CLampDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLampDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CLampDoc commands
void CLampDoc::Draw(HDC hDC, int device_height, RECT &DeviceRectangle, int pos, std::vector<CHotSpot> &hotspots, unsigned int current_id, bool bModToolIsUp, RECT &ModToolRect, unsigned int ModToolPostID)
{
   pos = -pos;

   int startpos = pos;

   int bannerheight = 71;
   CDCSurface *pNTImage = theApp.GetNewThreadImage(false);
   if(pNTImage != NULL)
   {
      bannerheight = pNTImage->GetHeight();
   }

   switch(m_datatype)
   {
   case DDT_EPICFAILD:
      ::FillRect(hDC,&DeviceRectangle,m_backgroundbrush);
      break;
   case DDT_PROFILE:
      {
         FillBackground(hDC,DeviceRectangle);

         if(m_rootposts.size() > 0)
         {
            RECT backrect = DeviceRectangle;
            backrect.top = pos;
            backrect.bottom = pos + 20;
            FillBackground(hDC,backrect);
            pos += 20;

            int textheight = theApp.GetTextHeight();
            int devwidth = DeviceRectangle.right - DeviceRectangle.left - 20;

            if(m_widthofaverageprofilegroup == 0)
            {
               CalcWidthOfAverageProfileGroup();
            }
            // todo
            int numcolumns = 4;
            int itemwidth = devwidth / numcolumns;

            while(itemwidth < m_widthofaverageprofilegroup && numcolumns > 1)
            {
               numcolumns--;
               itemwidth = devwidth / numcolumns;
            }

            int rowdone = 0;
            int rowpos = pos;

            std::list<ChattyPost*>::iterator it = m_rootposts.begin();
            std::list<ChattyPost*>::iterator end = m_rootposts.end();
            while(it != end)
            {
               RECT thisrect;

               if((*it)->GetIsProfileGroup())
               {
                  if(rowdone == numcolumns)
                  {
                     rowdone = 0;
                     pos = rowpos;
                     pos += 20;
                  }
                  thisrect.left = 10 + itemwidth * rowdone;
                  thisrect.right = thisrect.left + itemwidth;

                  int thispos = (*it)->DrawProfile(hDC, thisrect, pos, hotspots);
                  if(thispos > rowpos)
                     rowpos = thispos;

                  rowdone++;
               }
               else
               {
                  if(rowdone == numcolumns)
                  {
                     rowdone = 0;
                     pos = rowpos;
                     pos += 20;
                     thisrect.left = DeviceRectangle.left + 10;
                  }
                  else
                  {
                     thisrect = DeviceRectangle;
                     thisrect.left = 10 + itemwidth * rowdone;
                  }
                  thisrect.right = DeviceRectangle.right - 10;

                  int thispos = (*it)->DrawProfile(hDC, thisrect, pos, hotspots);
                  if(rowdone > 0)
                  {
                     if(thispos > rowpos)
                        rowpos = thispos;
                     pos = rowpos;
                  }
                  else
                  {
                     pos = thispos;
                  }

                  rowdone = 0;
                  pos += 20;
               }
               
               it++;
            }

            backrect = DeviceRectangle;
            backrect.top = pos;
            backrect.bottom = pos + (device_height / 2);
            FillBackground(hDC,backrect);
            pos += (device_height / 2);
         }
      }
      break;
   case DDT_STORY:
      {
         if(m_rootposts.size() > 0)
         {
            //pos = DrawBanner(hDC, DeviceRectangle, pos, hotspots, true, false);
            pos += bannerheight;

            if(m_pReplyDlg != NULL && !m_pReplyDlg->IsMessage())
            {
               RECT replydlgrect;
               replydlgrect.left = DeviceRectangle.left + 20;
               replydlgrect.right = DeviceRectangle.right - 20;
               replydlgrect.top = pos;
               replydlgrect.bottom = pos + m_pReplyDlg->GetHeight();
               m_pReplyDlg->SetDlgRect(replydlgrect);

               replydlgrect.bottom += 20;
               pos = replydlgrect.bottom;

               if(replydlgrect.bottom < DeviceRectangle.top ||
                  replydlgrect.top > DeviceRectangle.bottom)
               {
                  // do nothing, we are not on the screen
               }
               else 
               {
                  RECT backrect = replydlgrect;
                  backrect.left = DeviceRectangle.left;
                  backrect.right = replydlgrect.left;
                  FillBackground(hDC,backrect);

                  backrect.left = replydlgrect.right;
                  backrect.right = DeviceRectangle.right;
                  FillBackground(hDC,backrect);

                  backrect = replydlgrect;
                  backrect.top = backrect.bottom - 20;
                  FillBackground(hDC,backrect);
               }
            }

            pos = DrawFromRoot(hDC, DeviceRectangle, pos, hotspots, current_id, false, theApp.IsModMode(), bModToolIsUp, ModToolRect, ModToolPostID);

            RECT backrect = DeviceRectangle;
            backrect.top = pos;
            backrect.bottom = pos + (device_height / 2);
            FillBackground(hDC,backrect);
            pos += (device_height / 2);

            //pos = DrawBanner(hDC, DeviceRectangle, pos, hotspots, true, false);
         }
      }
      break;
   case DDT_THREAD:
      {
         RECT backrect = DeviceRectangle;
         backrect.top = pos;
         backrect.bottom = pos + 20;
         FillBackground(hDC,backrect);
         pos += 20;
         
         pos = DrawFromRoot(hDC, DeviceRectangle, pos, hotspots, current_id, false, theApp.IsModMode(), bModToolIsUp, ModToolRect, ModToolPostID);

         backrect.top = pos;
         backrect.bottom = pos + (device_height / 2);
         FillBackground(hDC,backrect);
         pos += (device_height / 2);
      }
      break;
   case DDT_LOLS:
      {
         if(m_rootposts.size() > 0)
         {
            //pos = DrawBanner(hDC, DeviceRectangle, pos, hotspots, false, false);
            pos += bannerheight;
            
            pos = DrawFromRoot(hDC, DeviceRectangle, pos, hotspots, current_id, true, false, false, ModToolRect, 0);

            RECT backrect = DeviceRectangle;
            backrect.top = pos;
            backrect.bottom = pos + (device_height / 2);
            FillBackground(hDC,backrect);
            pos += (device_height / 2);
         }
      }
      break;
   case DDT_SEARCH:
      {
         //pos = DrawBanner(hDC, DeviceRectangle, pos, hotspots, false, false);
         pos += bannerheight;
         
         pos = DrawFromRoot(hDC, DeviceRectangle, pos, hotspots, current_id, true, false, false, ModToolRect, 0);

         RECT backrect = DeviceRectangle;
         backrect.top = pos;
         backrect.bottom = pos + (device_height / 2);
         FillBackground(hDC,backrect);
         pos += (device_height / 2);
      }
      break;
   case DDT_SHACKMSG:
      {
         if(m_rootposts.size() > 0)
         {
            //pos = DrawBanner(hDC, DeviceRectangle, pos, hotspots, false, true);
            pos += bannerheight;
            pos = DrawMessages(hDC, DeviceRectangle, pos, hotspots, current_id);

            RECT backrect = DeviceRectangle;
            backrect.top = pos;
            backrect.bottom = pos + (device_height / 2);
            FillBackground(hDC,backrect);
            pos += (device_height / 2);
         }
      }
      break;
   }

   m_height = pos - startpos;

   if(pos < DeviceRectangle.bottom)
   {
      RECT leftovers = DeviceRectangle;
      leftovers.top = pos;
      ::FillRect(hDC,&leftovers,m_backgroundbrush);
   }
}

int CLampDoc::DrawBanner(HDC hDC, RECT &DeviceRectangle, int pos, std::vector<CHotSpot> &hotspots, bool bDrawNewThread, bool bDrawCompose)
{
   RECT bannerrect = DeviceRectangle;
   CDCSurface *pNewThreadImage = theApp.GetNewThreadImage(false);
   CDCSurface *pComposeImage = theApp.GetComposeImage(false);
   CDCSurface *pRefreshStoryImage = theApp.GetRefreshStoryImage(false);
   if(pNewThreadImage != NULL &&
      pComposeImage != NULL &&
      pRefreshStoryImage != NULL)
   {
      bannerrect.top = pos;
      bannerrect.bottom = bannerrect.top + pNewThreadImage->GetHeight();

      if(bannerrect.bottom < DeviceRectangle.top ||
         bannerrect.top > DeviceRectangle.bottom)
      {
         // skip it.  Not visible
      }
      else
      {
         CHotSpot hotspot;
         hotspot.m_bAnim = false;
         RECT imagerect = bannerrect;
         RECT restrect = bannerrect;
         if(bDrawNewThread || bDrawCompose)
         {
            if(bDrawNewThread)
            {
               imagerect.right = imagerect.left + pNewThreadImage->GetWidth();
               pNewThreadImage->Blit(hDC,imagerect);
               hotspot.m_type = HST_NEWTHREAD;
            }
            else if(bDrawCompose)
            {
               imagerect.right = imagerect.left + pComposeImage->GetWidth();
               pComposeImage->Blit(hDC,imagerect);
               hotspot.m_type = HST_COMPOSE_MESSAGE;
            }
            hotspot.m_spot.left = imagerect.left + 20;
            hotspot.m_spot.top = imagerect.top + 20;
            hotspot.m_spot.right = hotspot.m_spot.left + 144;
            hotspot.m_spot.bottom = hotspot.m_spot.top + 28;
            hotspot.m_id = 0;
            hotspots.push_back(hotspot);

            restrect.left = imagerect.right;
         }

         // draw refresh story button

         imagerect = bannerrect;
         imagerect.left = imagerect.right - pRefreshStoryImage->GetWidth();
         pRefreshStoryImage->Blit(hDC,imagerect);
                     
         hotspot.m_type = HST_REFRESHSTORY;
         hotspot.m_spot.right = imagerect.right - 20;
         hotspot.m_spot.top = imagerect.top + 20;
         hotspot.m_spot.left = hotspot.m_spot.right - 144;
         hotspot.m_spot.bottom = hotspot.m_spot.top + 28;
         hotspot.m_id = 0;
         hotspots.push_back(hotspot);

         restrect.right = imagerect.left;

         // draw page bar

         FillBackground(hDC,restrect);

         RECT pagingrect = restrect;
         pagingrect.left += 20;
         pagingrect.right -= 20;

         if(GetDataType() == DDT_STORY && theApp.InfinatePaging() && theApp.UseShack())
         {
            ::SetTextAlign(hDC,TA_LEFT|TA_BOTTOM);
            MySelectFont(hDC,m_pagefont);
            ::SetTextColor(hDC,theApp.GetPostTextColor());

            UCString pagenote = L"Have ";
            pagenote += m_page;
            pagenote += L" page";
            if(m_page > 1)
            {
               pagenote += L"s";
            }
            pagenote += L" loaded.  Out of ";
            pagenote += m_lastpage;
            pagenote += L".";

            ::ExtTextOut(hDC, pagingrect.left + 15, pagingrect.top + ((pagingrect.bottom - pagingrect.top) / 2) + 6, 0, NULL, pagenote.Str(), pagenote.Length(), NULL);
         }
         else
         {
            int pagingwidth = pagingrect.right - pagingrect.left;

            // each square needs 20 + 10
            int howmanycanihave = m_lastpage + 2;

            while(howmanycanihave * (20+10) > pagingwidth)
            {
               howmanycanihave--;
            }

            // don't even bother if i cant' do at least 3
            if(howmanycanihave > 3)
            {
               howmanycanihave -= 2;  // reserve 2 for next and previous page

               int first = 1;
               int last = m_lastpage;

               if(howmanycanihave < (int)m_lastpage)
               {
                  if((int)m_page < howmanycanihave / 2)
                  {
                     // we can trim off the upper end
                     first = 1;
                     last = howmanycanihave;
                  }
                  else if((int)m_lastpage - (int)m_page < howmanycanihave / 2)
                  {
                     // we can trim off the lower end
                     first = ((int)m_lastpage - howmanycanihave) + 1;
                     last = m_lastpage;
                  }
                  else
                  {
                     // we trim both ends
                     first = __max(1,(int)m_page - (howmanycanihave / 2));
                     last = (first + howmanycanihave) - 1;
                  }
               }

               int x = ((pagingrect.right + pagingrect.left) / 2) - (int)(((float)(howmanycanihave + 2) /2.0f) * (20.0f+10.0f));
               int y = ((bannerrect.bottom + bannerrect.top) / 2) + 10;

               ::SetTextAlign(hDC,TA_CENTER|TA_BOTTOM);
               MySelectFont(hDC,m_pagefont);
               HBRUSH oldbrush = (HBRUSH)::SelectObject(hDC,m_rootbackgroundbrush);
               HPEN oldpen = (HPEN)::SelectObject(hDC,m_roottoppen);
               HPEN nullpen = ::CreatePen(PS_NULL,0,0);
               ::SetTextColor(hDC,theApp.GetPostTextColor());

               RECT pagebut;
               pagebut.left = x + 5;
               pagebut.top = y - 20;
               pagebut.right = x + 25;
               pagebut.bottom = y;

               // draw the previous
               if(m_page == 1)
               {
                  // draw greyed out
                  ::SelectObject(hDC,nullpen);
                  ::Rectangle(hDC, pagebut.left, pagebut.top, pagebut.right, pagebut.bottom);
                  ::ExtTextOut(hDC, x + 15, y - 2 , 0, NULL, L"<", 1, NULL);
                  ::SelectObject(hDC,m_roottoppen);
               }
               else
               {
                  ::Rectangle(hDC, pagebut.left, pagebut.top, pagebut.right, pagebut.bottom);
                  ::ExtTextOut(hDC, x + 15, y - 2 , 0, NULL, L"<", 1, NULL);
                  
                  hotspot.m_type = HST_PREV_PAGE;
                  hotspot.m_spot = pagebut;
                  hotspot.m_id = 0;
                  hotspots.push_back(hotspot);
               }
               pagebut.left += 30;
               pagebut.right += 30;
               x += 30;

               for(int i = first; i <= last; i++)
               {
                  UCString pagenum = i;
                  // draw the previous
                  if(i == m_page)
                  {
                     // draw greyed out
                     ::SelectObject(hDC,nullpen);
                     ::Rectangle(hDC, pagebut.left, pagebut.top, pagebut.right, pagebut.bottom);
                     ::ExtTextOut(hDC, x + 15, y - 2 , 0, NULL, pagenum, pagenum.Length(), NULL);
                     ::SelectObject(hDC,m_roottoppen);
                  }
                  else
                  {
                     ::Rectangle(hDC, pagebut.left, pagebut.top, pagebut.right, pagebut.bottom);
                     ::ExtTextOut(hDC, x + 15, y - 2 , 0, NULL, pagenum, pagenum.Length(), NULL);
                  }   

                  hotspot.m_type = HST_PAGE;
                  hotspot.m_spot = pagebut;
                  hotspot.m_id = i;
                  hotspots.push_back(hotspot);
                  
                  pagebut.left += 30;
                  pagebut.right += 30;
                  x += 30;
               }
               
               // draw the next
               if(m_page == m_lastpage)
               {
                  // draw greyed out
                  ::SelectObject(hDC,nullpen);
                  ::Rectangle(hDC, pagebut.left, pagebut.top, pagebut.right, pagebut.bottom);
                  ::ExtTextOut(hDC, x + 15, y - 2 , 0, NULL, L">", 1, NULL);
                  ::SelectObject(hDC,m_roottoppen);
               }
               else
               {
                  ::Rectangle(hDC, pagebut.left, pagebut.top, pagebut.right, pagebut.bottom);
                  ::ExtTextOut(hDC, x + 15, y - 2 , 0, NULL, L">", 1, NULL);
                  
                  hotspot.m_type = HST_NEXT_PAGE;
                  hotspot.m_spot = pagebut;
                  hotspot.m_id = 0;
                  hotspots.push_back(hotspot);
               }

               ::SelectObject(hDC,oldbrush);
               ::SelectObject(hDC,oldpen);
               ::DeleteObject(nullpen);
               ::SetTextAlign(hDC,TA_LEFT|TA_BOTTOM);
            }
         }

         hotspot.m_type = HST_NULL_BACKGROUND;
         hotspot.m_spot = DeviceRectangle;
         hotspot.m_id = 0;
         hotspots.push_back(hotspot);
      }
   }

   return bannerrect.bottom;
}

int CLampDoc::DrawFromRoot(HDC hDC, RECT &DeviceRectangle, int pos, std::vector<CHotSpot> &hotspots, unsigned int current_id, bool bLinkOnly, bool bAllowModTools, bool bModToolIsUp, RECT &ModToolRect, unsigned int ModToolPostID)
{
   std::list<ChattyPost*>::iterator begin = m_rootposts.begin();
   std::list<ChattyPost*>::iterator end = m_rootposts.end();
   std::list<ChattyPost*>::iterator it = begin;

   while(it != end)
   {
      pos = (*it)->DrawRoot(hDC,DeviceRectangle,pos,hotspots, current_id, bLinkOnly, bAllowModTools, bModToolIsUp, ModToolRect, ModToolPostID);
      it++;

      if(it != end)
      {
         RECT backrect = DeviceRectangle;
         backrect.top = pos;
         backrect.bottom = pos + 20;
         if(backrect.bottom < DeviceRectangle.top ||
            backrect.top > DeviceRectangle.bottom)
         {
            // do nothing, we are not on the screen
         }
         else 
         {
            FillBackground(hDC,backrect);
         }
         pos += 20;
      }
   }
   return pos;
}


int CLampDoc::DrawMessages(HDC hDC, RECT &DeviceRectangle, int pos, std::vector<CHotSpot> &hotspots, unsigned int current_id)
{
   std::list<ChattyPost*>::iterator begin = m_rootposts.begin();
   std::list<ChattyPost*>::iterator end = m_rootposts.end();
   std::list<ChattyPost*>::iterator it = begin;

   while(it != end)
   {
      pos = (*it)->DrawMessage(hDC,DeviceRectangle,pos,hotspots, current_id);
      it++;

      if(it != end)
      {
         RECT backrect = DeviceRectangle;
         backrect.top = pos;
         backrect.bottom = pos + 20;
         if(backrect.bottom < DeviceRectangle.top ||
            backrect.top > DeviceRectangle.bottom)
         {
            // do nothing, we are not on the screen
         }
         else 
         {
            FillBackground(hDC,backrect);
         }
         pos += 20;
      }
   }
   return pos;
}

bool GetXMLDataFromString(CXMLTree &xmldata, const char *data, int datasize)
{
   bool result = false;
   // find where the xml section begins
   // cant use strstr() becuase the data pointer is not null terminated
   bool bFoundit = false;
   const char *xml = data;
   const char *end = data + datasize - 6;
   while(xml < end)
   {   
      if(strncmp(xml, "<?xml",5) == 0)
      {
         bFoundit = true;
         break;
      }
      xml++;
   }
   
   if(bFoundit &&
      xml != NULL)
   {
      UCString xmlstring;
      xmlstring.AppendEncodedString(xml, datasize - (xml - data), CET_UTF8);
      result = xmldata.ReadFromBuffer(xmlstring);
   }

   bool bDebugOutTheXML = false;

   if(bDebugOutTheXML)
   {
      xmldata.Write(L"c:\\temp.xml");
   }

   return result;
}


void CLampDoc::ReadChattyPageFromHTML(std::string &stdstring, std::vector<unsigned int> &existing_threads, bool bCheckForPages, bool bSkipExistingThreads/*=false*/)
{
   if(bCheckForPages)
   {
      m_lastpage = 1;
   }
   htmlcxx::HTML::ParserDom parser;
   tree<htmlcxx::HTML::Node> dom;
   try
   {
      dom = parser.parseTree(stdstring);
   }
   catch(...)
   {
      // whatever. move on
   }

   tree<htmlcxx::HTML::Node>::sibling_iterator it;
   if(HTML_FindChild(dom,it, "body"))
   {
      if(HTML_FindChild_HasAttribute(it,it, "div", "id", "container"))
      {
         if(HTML_FindChild_HasAttribute(it,it, "div", "id", "content"))
         {
            if(HTML_FindChild_HasAttribute(it,it, "div", "id", "chatty_comments_wrap"))
            {
               if(HTML_FindChild_HasAttribute(it,it, "div", "class", "commentsblock"))
               {
                  tree<htmlcxx::HTML::Node>::sibling_iterator threads_it;
                  if(HTML_FindChild_HasAttribute(it,threads_it, "div", "class", "threads"))
                  {
                     tree<htmlcxx::HTML::Node>::sibling_iterator sit = threads_it.begin();
                     tree<htmlcxx::HTML::Node>::sibling_iterator send = threads_it.end();

                     while(sit != send)
                     {
                        if(sit->tagName() == "div" &&
                           HTML_StartsWithAttribute(sit, "class", "root"))
                        {
                           unsigned int root_id = HTML_GetIDAttribute(sit);

                           if(root_id != 0)
                           {
                              ChattyPost *post = NULL;
                              
                              bool bAlreadyHadIt = false;
                              bool bSkip = false;
                              std::map<unsigned int,newness> post_newness;
                              std::map<unsigned int,std::vector<shacktagpos>> post_tags;

                              for(size_t j = 0; j < existing_threads.size(); j++)
                              {
                                 if(existing_threads[j] == root_id)
                                 {
                                    if(bSkipExistingThreads)
                                    {
                                       bSkip = true;
                                    }
                                    else
                                    {
                                       post = FindRootPost(root_id);
                                       post->RecordNewness(post_newness);
                                       post->RecordTags(post_tags);
                                       post->ClearChildren();
                                    }
                                    bAlreadyHadIt = true;                                 
                                    break;
                                 }
                              }

                              if(!bSkip)
                              {
                                 if(post == NULL)
                                 {
                                    post = new ChattyPost();
                                    post->SetNewness(N_OLD);
                                    m_rootposts.push_back(post);
                                 }

                                 post->ReadRootChattyFromHTML(sit, this, root_id);
                                 post->EstablishNewness(post_newness);
                                 post->EstablishTags(post_tags);
                                 post->SetupPreviewShades(false);
                                 post->CountFamilySize();
                                 post->UpdateRootReplyList();
                                 post->SetParent(NULL);
                                 if(post->IsFiltered() && !bAlreadyHadIt)
                                 {
                                    m_rootposts.pop_back();
                                    delete post;
                                 }
                              }
                           }
                        }

                        sit++;
                     }
                  }

                  if(bCheckForPages)
                  {
                     tree<htmlcxx::HTML::Node>::sibling_iterator pagination_it;
                     if(HTML_FindChild_HasAttribute(it,pagination_it, "div", "class", "pagenavigation"))
                     {
                        tree<htmlcxx::HTML::Node>::sibling_iterator sit = pagination_it.begin();
                        tree<htmlcxx::HTML::Node>::sibling_iterator send = pagination_it.end();
                        
                        while(sit != send)
                        {
                           if(sit->tagName() == "a" &&
                              HTML_StartsWithAttribute(sit, "href", "/chatty?"))
                           {
                              std::string str;
                              HTML_GetValue(sit,str);
                              int num = atoi(str.data());
                              if(num > (int)m_lastpage)
                              {
                                 m_lastpage = num;
                              }
                           }
                           sit++;
                        }                              
                     }
                  }
               }
            }
         }
      }
   }
}





bool CLampDoc::ReadFromRoot(CXMLTree &xmldata, std::vector<unsigned int> &existing_threads)
{
   bool result = false;
   if(xmldata.GetTag() == L"comments")
   {
      UCString temp = xmldata.GetAttributeValue(L"story_id");
      m_storyid = temp;

      temp = xmldata.GetAttributeValue(L"story_name");
      m_story_name = temp;

      m_title = m_story_name;
      MySetTitle(m_title);

      temp = xmldata.GetAttributeValue(L"page");
      m_page = temp;

      if(theApp.IsDoublePageStory())
      {
         m_page = (m_page + 1) / 2;
      }

      temp = xmldata.GetAttributeValue(L"last_page");
      m_lastpage = temp;

      if(theApp.IsDoublePageStory())
      {
         m_lastpage = (m_lastpage + 1) / 2;
      }

      result = true;
      int count = xmldata.CountChildren();
      for(int i = 0; i < count; i++)
      {
         CXMLElement *pChild = xmldata.GetChildElement(i);
         if(pChild != NULL &&
            pChild->GetTag() == L"comment")
         {
            unsigned int root_id = pChild->GetAttributeValue(L"id");

            bool bAlreadyHere = false;

            for(size_t j = 0; j < existing_threads.size(); j++)
            {
               if(existing_threads[j] == root_id)
               {
                  bAlreadyHere = true;
                  break;
               }
            }

            if(!bAlreadyHere)
            {
               ChattyPost *post = new ChattyPost();
               if(post != NULL)
               {
                  m_rootposts.push_back(post);
                  post->Read(pChild, this, false);
                  post->SetupPreviewShades(false);
                  post->CountFamilySize();
                  post->UpdateRootReplyList();
                  post->SetParent(NULL);
                  if(post->IsFiltered())
                  {
                     m_rootposts.pop_back();
                     delete post;
                  }
               }
            }
         }
      }
   }
   
   return result;
}


void CLampDoc::ReadShackMessagesHTML(std::string &stdstring)
{
   htmlcxx::HTML::ParserDom parser;
   tree<htmlcxx::HTML::Node> dom;
   try
   {
      dom = parser.parseTree(stdstring);
   }
   catch(...)
   {
      // whatever. move on
   }
                  
   tree<htmlcxx::HTML::Node>::sibling_iterator it;
   if(HTML_FindChild(dom,it, "body"))
   {
      if(HTML_FindChild_HasAttribute(it,it, "div", "id", "container"))
      {
         if(HTML_FindChild_HasAttribute(it,it, "div", "id", "content"))
         {
            if(HTML_FindChild_HasAttribute(it,it, "div", "id", "messages_wrap"))
            {
               tree<htmlcxx::HTML::Node>::sibling_iterator pagination_it;
               if(HTML_FindChild_HasAttribute(it,pagination_it, "div", "class", "message-nav-bar"))
               {
                  if(HTML_FindChild_HasAttribute(pagination_it,pagination_it, "div", "class", "navigate-column pagination"))
                  {
                     tree<htmlcxx::HTML::Node>::sibling_iterator sit = pagination_it.begin();
                     tree<htmlcxx::HTML::Node>::sibling_iterator send = pagination_it.end();

                     m_lastpage = 1;

                     while(sit != send)
                     {
                        if(sit->tagName() == "a" &&
                           HTML_StartsWithAttribute(sit, "href", "/messages/"))
                        {
                           std::string str;
                           HTML_GetValue(sit,str);
                           int num = atoi(str.data());
                           if(num > (int)m_lastpage)
                           {
                              m_lastpage = num;
                           }
                        }
                        
                        sit++;
                     }
                  }
               }

               bool bIsInbox = false;
               if(m_shackmsgtype == SMT_INBOX)
               {
                  bIsInbox = true;
               }

               tree<htmlcxx::HTML::Node>::sibling_iterator messages_it;
               if(HTML_FindChild_HasAttribute(it,messages_it, "div", "id", "message_center"))
               {
                  if(HTML_FindChild(messages_it,messages_it, "form"))
                  {
                     if(HTML_FindChild_HasAttribute(messages_it,messages_it, "ul", "id", "messages"))
                     {
                        tree<htmlcxx::HTML::Node>::sibling_iterator sbegin = messages_it.begin();
                        tree<htmlcxx::HTML::Node>::sibling_iterator slast = sbegin;
                        tree<htmlcxx::HTML::Node>::sibling_iterator send = messages_it.end();
                        tree<htmlcxx::HTML::Node>::sibling_iterator sit = sbegin;
                        while(sit != send){slast = sit; sit++;}

                        sit = slast;
                        
                        bool bContinue = true;
                        while(bContinue)
                        {
                           if(sit->tagName() == "li")
                           {
                              std::string value;
                              if(HTML_StartsWithAttribute(sit, "class", "message",&value))
                              {
                                 std::string author;

                                 size_t id = 0;

                                 bool bHaveRead = false;
                                 if(value == " read")
                                    bHaveRead = true;

                                 tree<htmlcxx::HTML::Node>::sibling_iterator info_it;
                                 if(HTML_FindChild_HasAttribute(sit,info_it, "div", "class", "name-column"))
                                 {
                                    tree<htmlcxx::HTML::Node>::sibling_iterator id_it;
                                    if(HTML_FindChild(info_it,id_it, "input"))
                                    {
                                       id = HTML_GetIDAttribute(id_it, "value");
                                    }

                                    tree<htmlcxx::HTML::Node>::sibling_iterator name_it;
                                    if(HTML_FindChild_HasAttribute(info_it, name_it, "a", "class", "username"))
                                    {
                                       HTML_GetValue(name_it, author);
                                    }
                                 }

                                 bool bFoundIt = false;
                                 std::list<ChattyPost*>::iterator rit = m_rootposts.begin();
                                 std::list<ChattyPost*>::iterator rend = m_rootposts.end();
                                 while(rit != rend)
                                 {
                                    if((*rit) != NULL &&
                                       (*rit)->GetId() == id)
                                    {
                                       bFoundIt = true;
                                       (*rit)->SetHaveRead(bHaveRead);
                                       break;
                                    }
                                    rit++;
                                 }

                                 if(!bFoundIt)
                                 {
                                    ChattyPost *post = new ChattyPost();
                                    if(post != NULL)
                                    {
                                       m_rootposts.push_front(post);
                                       post->ReadMessageFromHTML(sit,this, bIsInbox, id, author, bHaveRead);
                                       post->Collapse();
                                    }
                                 }
                              }
                           }

                           if(sit == sbegin)
                           {
                              bContinue = false;
                           }
                           else
                           {
                              sit--;
                           }
                        }
                     }
                  }
               }
            }
         }
      }
   }
}

bool CLampDoc::ReadShackMessages(CXMLTree &xmldata)
{
   bool result = false;

   bool bIsInbox = false;
   if(m_shackmsgtype == SMT_INBOX)
   {
      bIsInbox = true;
   }

   if(xmldata.GetTag() == L"messages")
   {
      result = true;

      UCString temp = xmldata.GetAttributeValue(L"last_page");
      m_lastpage = temp;

      int count = xmldata.CountChildren();
      for(int i = count - 1; i >= 0; i--)
      {
         CXMLElement *pChild = xmldata.GetChildElement(i);
         if(pChild != NULL &&
            pChild->GetTag() == L"message")
         {
            UCString temp = pChild->GetAttributeValue(L"id");
            unsigned int id = temp;

            bool bFoundIt = false;
            std::list<ChattyPost*>::iterator it = m_rootposts.begin();
            std::list<ChattyPost*>::iterator end = m_rootposts.end();
            while(it != end)
            {
               if((*it) != NULL &&
                  (*it)->GetId() == id)
               {
                  bFoundIt = true;

                  bool bHaveRead = false;
                  temp = pChild->GetAttributeValue(L"status");
                  if(temp == L"read")
                  {
                     bHaveRead = true;
                  }
                  (*it)->SetHaveRead(bHaveRead);

                  break;
               }
               it++;
            }

            if(!bFoundIt)
            {
               ChattyPost *post = new ChattyPost();
               if(post != NULL)
               {
                  m_rootposts.push_front(post);
                  post->ReadShackMessage(pChild, this, bIsInbox);
                  post->Collapse();
               }
            }
         }
      }
   }

   return result;
}

bool CLampDoc::ReadSearchResultsFromRoot(CXMLTree &xmldata)
{
   bool result = false;
   if(xmldata.GetTag() == L"comments")
   {
      result = true;
      int count = xmldata.CountChildren();
      for(int i = 0; i < count; i++)
      {
         CXMLElement *pChild = xmldata.GetChildElement(i);
         if(pChild != NULL &&
            pChild->GetTag() == L"comment")
         {
            ChattyPost *post = new ChattyPost();
            if(post != NULL)
            {
               m_rootposts.push_back(post);
               post->ReadSearchResult(pChild, this);
               if(post->IsFiltered())
               {
                  m_rootposts.pop_back();
                  delete post;
               }
            }
         }
      }
   }

   return result;
}


bool CLampDoc::ReadExistingThreadFromRoot(CXMLTree &xmldata, unsigned int id, bool bDoingNewFlags)
{
   bool result = false;
   if(xmldata.GetTag() == L"comments")
   {
      UCString temp = xmldata.GetAttributeValue(L"story_id");
      m_storyid = temp;

      result = true;
      int count = xmldata.CountChildren();
      for(int i = 0; i < count; i++)
      {
         CXMLElement *pChild = xmldata.GetChildElement(i);
                  
         if(pChild != NULL &&
            pChild->GetTag() == L"comment")
         {
            unsigned int root_id = pChild->GetAttributeValue(L"id");
            CXMLElement *pReplies = pChild->FindChildElement(L"comments");
            if(pReplies != NULL)
            {
               CXMLElement *pCachedReplies = new CXMLElement(pReplies);
               theApp.AddCachedThreadReplies(root_id,pCachedReplies);

               ChattyPost *post = FindRootPost(id);
               if(post != NULL)
               {
                  post->Read(pChild, this, bDoingNewFlags);
                  post->SetupPreviewShades(bDoingNewFlags);
                  post->CountFamilySize();
                  post->UpdateRootReplyList();
                  post->SetParent(NULL);
                  if(post->IsFiltered())
                  {
                     m_rootposts.pop_back();
                     delete post;
                  }
               }
            }
            else
            {
               // try just looking for "comment" entities here
               CXMLElement *pCachedReplies = new CXMLElement(pChild);
               theApp.AddCachedThreadReplies(root_id,pCachedReplies);

               ChattyPost *post = FindRootPost(id);
               if(post != NULL)
               {
                  post->Read(pChild, this, bDoingNewFlags);
                  post->SetupPreviewShades(bDoingNewFlags);
                  post->CountFamilySize();
                  post->UpdateRootReplyList();
                  post->SetParent(NULL);
                  if(post->IsFiltered())
                  {
                     m_rootposts.pop_back();
                     delete post;
                  }
               }
            }
         }
      }
   }

   return result;
}


ChattyPost *CLampDoc::GetThreadPost()
{
   if(m_datatype == DDT_THREAD &&
      m_rootposts.size() > 0)
   {
      return (*(m_rootposts.begin()));
   }
   return NULL;
}

ChattyPost *CLampDoc::FindRootPost(unsigned int id)
{
   std::list<ChattyPost*>::iterator it = m_rootposts.begin();
   std::list<ChattyPost*>::iterator end = m_rootposts.end();
   while(it != end)
   {
      if((*it)->GetId() == id)
      {
         return *it;
      }
      it++;
   }
   return NULL;
}

ChattyPost *CLampDoc::FindPost(unsigned int id)
{
   std::list<ChattyPost*>::iterator it = m_rootposts.begin();
   std::list<ChattyPost*>::iterator end = m_rootposts.end();
   while(it != end)
   {
      if((*it)->GetId() == id)
      {
         return *it;
      }
      else
      {
         ChattyPost *pPost = (*it)->FindChild(id);
         if(pPost != NULL)
         {
            return pPost;
         }
      }
      it++;
   }
   return NULL;
}

void CLampDoc::CalcBodyText(RECT &rect, 
                            const UCChar *text, 
                            int *widths, 
                            std::vector<shacktagpos> &shacktags,
                            int numchars, 
                            std::vector<const UCChar*> &lines_of_text, 
                            std::vector<const int*> &charsizes, 
                            std::vector<const int> &linesizes,
                            std::vector<std::vector<shacktagpos>> &linetags,
                            std::vector<linetype> &linetypes)
{
   lines_of_text.clear();
   charsizes.clear();
   linesizes.clear();
   linetags.clear();
   linetypes.clear();

   int textwidth = rect.right - rect.left - 5 - 5;
   int rectheight = 4 + 4;

   const UCChar *thistext = text;
   const int *thiswidths = widths;
   int thisi = 0;
   int thislength = 0;

   int spacei = 0;

   int image_tag = -1;
   int image_endtag = -1;
   int image_begin = -1;
   int image_end = -1;

   bool bIsThumb = false;

   for(unsigned int tag = 0; tag < shacktags.size(); tag++)
   {
      if(image_begin == -1 && 
         shacktags[tag].m_tag == ST_IMAGE)
      {
         image_tag = (int)tag;
         image_begin = shacktags[tag].m_pos;
      }

      if(image_begin == -1 && 
         shacktags[tag].m_tag == ST_THUMB)
      {
         image_tag = (int)tag;
         image_begin = shacktags[tag].m_pos;
         bIsThumb = true;
      }

      if(image_begin != -1 &&
         ((!bIsThumb && shacktags[tag].m_tag == ST_IMAGE_END) || 
          (bIsThumb && shacktags[tag].m_tag == ST_THUMB_END)))
      {
         image_endtag = (int)tag;
         image_end = shacktags[tag].m_pos;
         break;
      }
   }


   bool breaknext = false;
   int i = 0;
   for(; i < numchars; i++)
   {
      while(i == image_begin &&
            image_tag != -1)
      {
         // finish any remaining 
         if(i > thisi)
         {
            // break first, then start with char
            lines_of_text.push_back(thistext);
            charsizes.push_back(thiswidths);
            linesizes.push_back(i - thisi);
            linetypes.push_back(linetype(true,0,theApp.GetTextHeight(),false));
            rectheight += theApp.GetTextHeight();
            // setup tags for this finished line
            std::vector<shacktagpos> thislinetags;
            CalcLineTags(shacktags,thislinetags, thistext - text, i);
            linetags.push_back(thislinetags);
         }   

         // start this image line
         thistext = text + i;
         thiswidths = widths + i;
         thisi = i;
         thislength = 0;
         spacei = 0;

         i = image_end;
         if(text[i] == L'\n')
         {
            i++;
         }

         lines_of_text.push_back(thistext);
         charsizes.push_back(thiswidths);
         linesizes.push_back(i - thisi);

         unsigned int height = theApp.GetTextHeight();

         CDCSurface *pImage = NULL;

         if(bIsThumb)
         {
            pImage = theApp.GetLinkedImageThumb(shacktags[image_tag].m_image_index);
         }
         else
         {
            pImage = theApp.GetLinkedImage(shacktags[image_tag].m_image_index);
         }

         if(pImage != NULL)
         {
            height = pImage->GetHeightForWidth(textwidth);
         }

         linetypes.push_back(linetype(false,shacktags[image_tag].m_image_index,height,bIsThumb));
         rectheight += height;

         // setup tags for this finished line
         std::vector<shacktagpos> thislinetags;
         linetags.push_back(thislinetags);

         
         thistext = text + i;
         thiswidths = widths + i;
         thisi = i;
         // reset for next image
         unsigned int tag = image_endtag;
         image_tag = -1;
         image_begin = -1;
         image_end = -1;
         bIsThumb = false;
         for(; tag < shacktags.size(); tag++)
         {
            if(image_begin == -1 && 
               shacktags[tag].m_tag == ST_IMAGE)
            {
               image_tag = (int)tag;
               image_begin = shacktags[tag].m_pos;
            }

            if(image_begin == -1 && 
               shacktags[tag].m_tag == ST_THUMB)
            {
               image_tag = (int)tag;
               image_begin = shacktags[tag].m_pos;
               bIsThumb = true;
            }

            if(image_begin != -1 &&
               ((!bIsThumb && shacktags[tag].m_tag == ST_IMAGE_END) || 
                (bIsThumb && shacktags[tag].m_tag == ST_THUMB_END)))
            {
               image_endtag = (int)tag;
               image_end = shacktags[tag].m_pos;
               break;
            }
         }

         if(i >= numchars)
         {
            break;
         }

         breaknext = false;
      }

      if(breaknext)
      {
         breaknext = false;
         // break first, then start with char
         lines_of_text.push_back(thistext);
         charsizes.push_back(thiswidths);
         linesizes.push_back(i - thisi);
         linetypes.push_back(linetype(true,0,theApp.GetTextHeight(),false));
         rectheight += theApp.GetTextHeight();
         // setup tags for this finished line
         std::vector<shacktagpos> thislinetags;
         CalcLineTags(shacktags,thislinetags, thistext - text, i);
         linetags.push_back(thislinetags);
         
         // start new line
         thistext = text + i;
         thiswidths = widths + i;
         thisi = i;
         thislength = 0;
         spacei = 0;
      }
      else if(thislength + widths[i] > textwidth)
      {
         // see if we can fall back on the last space
         if(spacei != 0)
         {
            i = spacei + 1; // point just beyond the space
         }

         // break first, then start with char
         lines_of_text.push_back(thistext);
         charsizes.push_back(thiswidths);
         linesizes.push_back(i - thisi);
         linetypes.push_back(linetype(true,0,theApp.GetTextHeight(),false));
         rectheight += theApp.GetTextHeight();
         // setup tags for this finished line
         std::vector<shacktagpos> thislinetags;
         CalcLineTags(shacktags,thislinetags, thistext - text, i);
         linetags.push_back(thislinetags);

         // start new line
         thistext = text + i;
         thiswidths = widths + i;
         thisi = i;
         thislength = 0;
         spacei = 0;
      }
      
      if(text[i] == L'\t')
      {
         if((textwidth / 2) - thislength > 0)
         {
            widths[i] = (textwidth / 2) - thislength;
         }
         spacei = i;
      }
      
      // append char
      thislength += widths[i];
      if(text[i] == L'\n')
      {
         // include char, then break
         breaknext = true;         
      }
      else if(text[i] == L' ')
      {
         spacei = i;
      }
   }


   if(thistext < text + numchars)
   {
      // insert the remainder
      lines_of_text.push_back(thistext);
      charsizes.push_back(thiswidths);
      linesizes.push_back(i - thisi);
      linetypes.push_back(linetype(true,0,theApp.GetTextHeight(),false));
      rectheight += theApp.GetTextHeight();
      // setup tags for this finished line
      std::vector<shacktagpos> thislinetags;
      CalcLineTags(shacktags,thislinetags, thistext - text, i);
      linetags.push_back(thislinetags);
   }
   
   rect.bottom = rectheight;
}

void CLampDoc::StrokeShapedRect(HDC hDC, RECT &rect, int thickness)
{
   HPEN hNewPen = ::CreatePen(PS_SOLID,thickness, theApp.GetBackgroundColor());
   HPEN oldpen = (HPEN)::SelectObject(hDC,hNewPen);
   HBRUSH oldbrush = (HBRUSH)::SelectObject(hDC,::GetStockObject(HOLLOW_BRUSH));

   if(theApp.RoundedPosts())
   {
      ::RoundRect(hDC,rect.left, rect.top, rect.right, rect.bottom, 20, 20);
   }
   else
   {
      ::Rectangle(hDC,rect.left, rect.top, rect.right, rect.bottom);
   }

   ::SelectObject(hDC,oldbrush);
   ::SelectObject(hDC,oldpen);
   ::DeleteObject(hNewPen);
}

void CLampDoc::FillExpandedBackground(HDC hDC, RECT &rect, bool bAsRoot, postcategorytype posttype, bool bStrokeTopOnly)
{
   RECT temprect = rect;
   temprect.bottom = temprect.top + __min(rect.bottom - rect.top, 20);
   temprect.right = temprect.left + __min(rect.right - rect.left, 20);
   ::FillRect(hDC,&temprect,m_backgroundbrush);

   temprect.right = rect.right;
   temprect.left = rect.right - __min(rect.right - rect.left, 20);
   ::FillRect(hDC,&temprect,m_backgroundbrush);

   temprect = rect;
   temprect.top = temprect.bottom - __min(rect.bottom - rect.top, 20);
   temprect.right = temprect.left + __min(rect.right - rect.left, 20);
   ::FillRect(hDC,&temprect,m_backgroundbrush);

   temprect.right = rect.right;
   temprect.left = rect.right - __min(rect.right - rect.left, 20);
   ::FillRect(hDC,&temprect,m_backgroundbrush);

   temprect = rect;

   if(theApp.RoundedPosts())
   {
      bStrokeTopOnly = false;
   }

   if(bStrokeTopOnly)
   {
      ::SelectObject(hDC,m_nullpen);
      temprect.right++;
      temprect.bottom++;
   }
   else
   {
      switch(posttype)
      {
      case PCT_NORMAL:     
         if(bAsRoot)
         {  
            ::SelectObject(hDC,m_nullpen); 
            temprect.right++;
            temprect.bottom++;
         }
         else
         {
            ::SelectObject(hDC,m_roottoppen); 
         }
         break;
      case PCT_INF:        ::SelectObject(hDC,m_infpen); break;
      case PCT_NWS:        ::SelectObject(hDC,m_nwspen); break;
      case PCT_STUPID:     ::SelectObject(hDC,m_stupidpen); break;
      case PCT_OFFTOPIC:   ::SelectObject(hDC,m_offtopicpen); break;
      case PCT_POLITICAL:  ::SelectObject(hDC,m_politicalpen); break;
      }
   }

   HBRUSH oldbrush;

   if(bAsRoot)
   {
      oldbrush = (HBRUSH)::SelectObject(hDC,m_rootbackgroundbrush);
   }
   else
   {
      oldbrush = (HBRUSH)::SelectObject(hDC,m_replyexpandedbackgroundbrush);
   }

   if(theApp.RoundedPosts())
   {
      ::RoundRect(hDC,temprect.left, temprect.top, temprect.right, temprect.bottom, 20, 20);
   }
   else
   {
      ::Rectangle(hDC,temprect.left, temprect.top, temprect.right, temprect.bottom);
      if(bStrokeTopOnly)
      {
         switch(posttype)
         {
         case PCT_NORMAL:     ::SelectObject(hDC,m_roottoppen); break;
         case PCT_INF:        ::SelectObject(hDC,m_infpen); break;
         case PCT_NWS:        ::SelectObject(hDC,m_nwspen); break;
         case PCT_STUPID:     ::SelectObject(hDC,m_stupidpen); break;
         case PCT_OFFTOPIC:   ::SelectObject(hDC,m_offtopicpen); break;
         case PCT_POLITICAL:  ::SelectObject(hDC,m_politicalpen); break;
         }
         ::MoveToEx(hDC,temprect.left, temprect.top,NULL);
         ::LineTo(hDC,temprect.right - 1, temprect.top);
      }
   }

   ::SelectObject(hDC,oldbrush);
}

void CLampDoc::DrawLOLField(HDC hDC, loltagtype type, RECT &rect, UCString &lols, bool bHover, bool bVoted, bool bRoot, bool bHasLols)
{
   if(theApp.ShowThomWLOLS())
   {
      HFONT oldfont = NULL;

      COLORREF edgecolor = theApp.GetPostTextColor();
      COLORREF backcolor = 0;

      if(bRoot)
      {
         backcolor = theApp.GetRootPostBackgroundColor();
      }
      else
      {
         backcolor = theApp.GetPostBackgroundColor();
      }

      if(bRoot)
      {
         ::FillRect(hDC,&rect,m_rootbackgroundbrush);
      }
      else
      {
         ::FillRect(hDC,&rect,m_replyexpandedbackgroundbrush);
      }   

      COLORREF bracketcolor = RGB((GetRValue(edgecolor) + (GetRValue(backcolor) * 2)) / 3,
                                  (GetGValue(edgecolor) + (GetGValue(backcolor) * 2)) / 3,
                                  (GetBValue(edgecolor) + (GetBValue(backcolor) * 2)) / 3);

      int x = rect.left + 2;
      int y = rect.bottom;
      
      // draw leading bracket
      if(theApp.ShowSmallLOL())
      {
         oldfont = MySelectFont(hDC,m_miscfont);
      }
      else
      {
         oldfont = MySelectFont(hDC,m_normalfont);
      }

      ::SetTextColor(hDC,bracketcolor);
      ::SetTextAlign(hDC,TA_LEFT|TA_BOTTOM|TA_UPDATECP);
      ::SetBkMode(hDC,TRANSPARENT);

      ::MoveToEx(hDC, x, y, NULL); 
      ::ExtTextOutW(hDC, 0, y, ETO_CLIPPED, &rect, L"[ ", 2, NULL);

      // draw main body
      if(bHover || bVoted)
      {
         // underline variations
         if(theApp.ShowSmallLOL())
         {
            MySelectFont(hDC,m_miscunderlinefont);
         }
         else
         {
            MySelectFont(hDC,m_normalunderlinefont);
         }
      }
      else
      {
         if(theApp.ShowSmallLOL())
         {
            MySelectFont(hDC,m_miscfont);
         }
         else
         {
            MySelectFont(hDC,m_normalfont);
         }
      }

      COLORREF color = 0;

      switch(type)
      {
         case LTT_LOL: color = theApp.GetOrange(); break;
         case LTT_INF: color = theApp.GetBlue(); break;
         case LTT_UNF: color = theApp.GetRed(); break;
         case LTT_TAG: color = theApp.GetGreen(); break;
         case LTT_WTF: color = theApp.GetPurple(); break;
      }

      if(!bHasLols && !bHover)
      {
         color = RGB((GetRValue(color) + GetRValue(backcolor)) / 2,
                     (GetGValue(color) + GetGValue(backcolor)) / 2,
                     (GetBValue(color) + GetBValue(backcolor)) / 2);
      }
      
      ::SetTextColor(hDC,color);

      ::ExtTextOutW(hDC, 0, y, ETO_CLIPPED, &rect, lols.Str(), lols.Length(), NULL);

      // draw trailing bracket

      ::SetTextColor(hDC,bracketcolor);
      ::ExtTextOutW(hDC, 0, y, ETO_CLIPPED, &rect, L" ]", 2, NULL);


      ::SetTextAlign(hDC,TA_LEFT|TA_BOTTOM);
      
      MySelectFont(hDC,oldfont);
   }
   else
   {
      HFONT oldfont = NULL;

      if(lols.Length() > 0 || bHover)
      {
         if(theApp.ShowSmallLOL())
         {
            oldfont = MySelectFont(hDC,m_miscboldfont);
         }
         else
         {
            oldfont = MySelectFont(hDC,m_boldfont);
         }
      }
      else
      {
         if(theApp.ShowSmallLOL())
         {
            oldfont = MySelectFont(hDC,m_miscfont);
         }
         else
         {
            oldfont = MySelectFont(hDC,m_normalfont);
         }
      }

      COLORREF color = 0;

      UCString *text = &lols;

      switch(type)
      {
         case LTT_LOL: color = theApp.GetOrange(); break;
         case LTT_INF: color = theApp.GetBlue(); break;
         case LTT_UNF: color = theApp.GetRed(); break;
         case LTT_TAG: color = theApp.GetGreen(); break;
         case LTT_WTF: color = theApp.GetPurple(); break;
      }

      COLORREF thiscolor = color;

      if(text->Length() == 0 && !bHover)
      {
         if(text->Length() == 0)
         {
            COLORREF backcolor = 0;

            if(bRoot)
            {
               backcolor = theApp.GetRootPostBackgroundColor();
            }
            else
            {
               backcolor = theApp.GetPostBackgroundColor();
            }

            color = RGB((GetRValue(color) + GetRValue(backcolor)) / 2,
                        (GetGValue(color) + GetGValue(backcolor)) / 2,
                        (GetBValue(color) + GetBValue(backcolor)) / 2);
         }
      }

      if(text->Length() == 0 || bHover)
      {
         switch(type)
         {
            case LTT_LOL: text = &m_lol_text; break;
            case LTT_INF: text = &m_inf_text; break;
            case LTT_UNF: text = &m_unf_text; break;
            case LTT_TAG: text = &m_tag_text; break;
            case LTT_WTF: text = &m_wtf_text; break;
         }
      }


      if(bVoted)
      {
         COLORREF backcolor = 0;

         if(bRoot)
         {
            backcolor = theApp.GetRootPostBackgroundColor();
         }
         else
         {
            backcolor = theApp.GetPostBackgroundColor();
         }

         thiscolor = RGB((GetRValue(thiscolor) + GetRValue(backcolor)) / 2,
                         (GetGValue(thiscolor) + GetGValue(backcolor)) / 2,
                         (GetBValue(thiscolor) + GetBValue(backcolor)) / 2);
      
         HBRUSH newbrush = ::CreateSolidBrush(thiscolor);
         HBRUSH oldbrush = (HBRUSH)::SelectObject(hDC,newbrush);
         HPEN oldpen = (HPEN)::SelectObject(hDC,m_nullpen);

         if(theApp.RoundedPosts())
         {
            ::RoundRect(hDC,rect.left,rect.top,rect.right,rect.bottom, (rect.bottom - rect.top) / 2, (rect.bottom - rect.top) / 2);
         }
         else
         {
            ::Rectangle(hDC,rect.left,rect.top,rect.right,rect.bottom);
         }
         ::SelectObject(hDC,oldpen);
         ::SelectObject(hDC,oldbrush);
         ::DeleteObject(newbrush);
      }
      else if(bRoot)
      {
         ::FillRect(hDC,&rect,m_rootbackgroundbrush);
      }
      else
      {
         ::FillRect(hDC,&rect,m_replyexpandedbackgroundbrush);
      }   
      
      ::SetTextColor(hDC,color);

      ::SetTextAlign(hDC,TA_CENTER|TA_BOTTOM);

      ::SetBkMode(hDC,TRANSPARENT);

      ::ExtTextOutW(hDC, (rect.right + rect.left) / 2, rect.bottom, ETO_CLIPPED, &rect, text->Str(), text->Length(), NULL);

      ::SetTextAlign(hDC,TA_LEFT|TA_BOTTOM);
      
      MySelectFont(hDC,oldfont);
   }
}

void CLampDoc::DrawPreviewAuthor(HDC hDC, RECT &rect, UCString &text, bool clipped, int shade, COLORREF AuthorColor, const UCString &rootauthor)
{
   HFONT oldfont = MySelectFont(hDC,m_normalfont);

   ::SetTextColor(hDC,theApp.GetPostTextColorShade(shade));

   int offset;

   if(clipped)
   {
      ::ExtTextOutW(hDC, rect.left, rect.bottom, 0, NULL, L"... :", 5, NULL);
      offset = abs(theApp.GetCellHeight()) + 5;
   }
   else
   {
      ::ExtTextOutW(hDC, rect.left, rect.bottom, 0, NULL, L" :", 2, NULL);
      offset = abs(theApp.GetCellHeight() / 3) + 5;
   }

   ::SetTextColor(hDC,AuthorColor);

   if(rootauthor == text)
   {
      HPEN oldpen = (HPEN)::SelectObject(hDC,m_nullpen);
      COLORREF backcolor = theApp.GetBackgroundColor();
      COLORREF color = RGB((GetRValue(AuthorColor) + (GetRValue(backcolor)*3)) / 4,
                           (GetGValue(AuthorColor) + (GetGValue(backcolor)*3)) / 4,
                           (GetBValue(AuthorColor) + (GetBValue(backcolor)*3)) / 4);
      HBRUSH newbrush = ::CreateSolidBrush(color);
      HBRUSH oldbrush = (HBRUSH)::SelectObject(hDC,newbrush);
      
      if(theApp.RoundedPosts())
      {
         ::RoundRect(hDC,rect.left + offset - 5, rect.top, rect.right, rect.bottom, (rect.bottom - rect.top)/2, (rect.bottom - rect.top)/2);
      }
      else
      {
         ::Rectangle(hDC,rect.left + offset - 5, rect.top, rect.right, rect.bottom);
      }

      ::SelectObject(hDC,oldpen);
      ::SelectObject(hDC,oldbrush);
      ::DeleteObject(newbrush);
   }

   ::ExtTextOutW(hDC, rect.left + offset, rect.bottom, 0, NULL, text, text.Length(), NULL);

   MySelectFont(hDC,oldfont);
}

void CLampDoc::MyTextOut(HDC hDC, int x, int y, bool bSampleText, const UCChar *text, UINT count, const INT *widths, const RECT *pClipRect, bool bComplexShapeText)
{
   const UCChar *start = text;
   const UCChar *work = text;
   const UCChar *end = text + count;
   const INT *w = widths;

   UINT fupotions = 0;
   if(pClipRect != NULL)
   {
      fupotions = ETO_CLIPPED;
   }
   
   while(work < end)
   {
      if(*work == L'\n')
      {
         //output what we have
         UINT thiscount = work - start;
         if(bComplexShapeText)
         {
            GDIPLUS_TextOut(hDC, x, y, bSampleText, fupotions, pClipRect, start, thiscount, w);
         }
         else
         {
            ::ExtTextOutW(hDC, x, y, fupotions, pClipRect, start, thiscount, w);
         }
         thiscount++;
         work++;
         start = work;
         for(UINT i = 0; i < thiscount; i++)
         {
            x += *w;
            w++;
         }
      }
      else
      {
         work++;
      }
   }

   if(start < end)
   {
      //output the remainder
      if(bComplexShapeText)
      {
         GDIPLUS_TextOut(hDC, x, y, bSampleText, fupotions, pClipRect, start, end - start, w);
      }
      else
      {
         ::ExtTextOutW(hDC, x, y, fupotions, pClipRect, start, end - start, w);
      }
   }
}

void CLampDoc::GDIPLUS_TextOut( HDC hdc, int x, int y, bool bSampleText, UINT options, CONST RECT * lprect, const UCChar *lpString, UINT c, const INT* lpDx)
{
   //::ExtTextOutW(hdc, x, y, options, lprect, lpString, c, lpDx);

   Graphics graphics(hdc);

   graphics.SetPageUnit(UnitPixel);
   graphics.SetSmoothingMode(SmoothingModeHighQuality);
   graphics.SetCompositingMode(CompositingModeSourceOver);
   graphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);
   graphics.SetCompositingQuality(CompositingQualityHighQuality);

   x = x - (theApp.GetTextHeight() / 7);
   y = y + (theApp.GetTextHeight() / 12);

   int height;
   
   if(bSampleText)
   {
      int sample_offset = abs(theApp.GetFontHeight() - theApp.GetSampleFontHeight());
      y += sample_offset;
      height = theApp.GetTextHeight() - sample_offset;
   }
   else
   {
      height = theApp.GetTextHeight();
   }

   RectF rectf(x, y + theApp.GetFontHeight(), 100000.0f, height);
   
   Gdiplus::Font font(hdc, m_currentfont);

   StringFormat format;
   format.SetAlignment(StringAlignmentNear);
      
   COLORREF crf = ::GetTextColor(hdc);
   SolidBrush textcolor(Color(GetRValue(crf), GetGValue(crf), GetBValue(crf)));
   //SolidBrush textcolor(Color(255, 0, 255));
   
   if(lpDx != NULL)
   {
      while(c > 0)
      {
         graphics.DrawString(lpString, 1, &font, rectf, &format, &textcolor);
         rectf.Offset(*lpDx,0);
         lpDx++;
         lpString++;
         c--;
      }
   }
   else
   {
      graphics.DrawString(lpString, c, &font, rectf, &format, &textcolor);
   }
   
   graphics.Flush(FlushIntentionSync);
}

void CLampDoc::DrawPreviewText(HDC hDC,
                               RECT &rect,
                               UCString &text,
                               int *charwidths,
                               std::vector<shacktagpos> &shacktags,
                               int shade,
                               bool &clipped, 
                               bool bComplexShapeText)
{
   clipped = false;
   std::vector<COLORREF> colorstack;
   bool quote = false;
   bool sample = false;
   bool strike = false;
   bool italic = false;
   bool bold = false;
   bool underline = false;
   bool spoiler = false;
   bool code = false;
   bool link = false;
   bool imagelink = false;
   bool invert = false;
   
   COLORREF normalcolor = theApp.GetPostTextColorShade(shade);
   COLORREF color = normalcolor;

   bool colorchange = false;
   bool stylechange = false;
   HFONT hCreatedFont = NULL;
   HFONT hPreviousFont = NULL;

   int rise = theApp.GetDescent();

   ::SetTextAlign(hDC,TA_LEFT|TA_BASELINE);

   // setup initial font stuff
   ::SetTextColor(hDC,normalcolor);
   hPreviousFont = MySelectFont(hDC, m_normalfont);

   int width = rect.right - rect.left;
   int numchars = 0;
   int length = 0;
   for(int i = 0; i < text.Length(); i++)
   {
      if(length + charwidths[i] <= width)
      {
         length += charwidths[i];
         numchars++;
      }
      else
      {
         clipped = true;
         break;
      }
   }

   rect.right = rect.left + length;

   int x = rect.left;

   if(shacktags.size() == 0)
   {
      // if there are no tags, 
      // output everything
      if(spoiler)
      {
         RECT spoilrect;
         spoilrect.left = rect.left;
         spoilrect.bottom = rect.bottom;
         spoilrect.top = rect.top;

         spoilrect.right = x;
         const int *work = charwidths;
         const int *end = work + numchars;
         while(work < end)
         {
            spoilrect.right += *work;
            work++;
         }
         ::FillRect(hDC,&spoilrect,m_spoilerbrush);
      }
      else
      {
         MyTextOut(hDC, rect.left, rect.bottom - rise, sample, text, numchars, charwidths, &rect, bComplexShapeText);
         //::ExtTextOutW(hDC, rect.left, rect.bottom, 0, NULL, text, numchars, charwidths);
      }
   }
   else 
   {
      std::vector<shacktagpos>::iterator it = shacktags.begin();
      std::vector<shacktagpos>::iterator end = shacktags.end();

      int charsdone = 0;
      // if the first tag doesn't begin at 0, or there are no tags, 
      // output that part first
      if((*it).m_pos != 0)
      {
         int charsthisrun = __min((*it).m_pos,numchars);

         if(spoiler)
         {
            RECT spoilrect;
            spoilrect.left = x;
            spoilrect.bottom = rect.bottom;
            spoilrect.top = rect.top;

            spoilrect.right = x;
            const int *work = charwidths;
            const int *end = work + charsthisrun;
            while(work < end)
            {
               spoilrect.right += *work;
               work++;
            }
            ::FillRect(hDC,&spoilrect,m_spoilerbrush);
         }
         else
         {
            MyTextOut(hDC, x, rect.bottom - rise, sample, text, charsthisrun, charwidths, &rect, bComplexShapeText);
            //::ExtTextOutW(hDC, x, rect.bottom, 0, NULL, text, charsthisrun, charwidths);
         }
         
         // update x
         for(int j = 0; j < charsthisrun; j++)
         {
            x += charwidths[j];
         }
         charsdone += charsthisrun;
         if(charsdone == numchars)
         {
            it = end;
         }
      }

      // loop for every shack tag range
      while(it != end)
      {
         // todo
         switch((*it).m_tag)
         {
         case ST_RED: color = theApp.GetRed(); colorstack.push_back(color); colorchange = true; break;
         case ST_GREEN: color = theApp.GetGreen(); colorstack.push_back(color); colorchange = true; break;
         case ST_BLUE: color = theApp.GetBlue(); colorstack.push_back(color); colorchange = true; break;
         case ST_YELLOW: color = theApp.GetYellow(); colorstack.push_back(color); colorchange = true; break;
         case ST_OLIVE: color = theApp.GetOlive(); colorstack.push_back(color); colorchange = true; break;
         case ST_LIME: color = theApp.GetLime(); colorstack.push_back(color); colorchange = true; break;
         case ST_ORANGE: color = theApp.GetOrange(); colorstack.push_back(color); colorchange = true; break;
         case ST_PURPLE: color = theApp.GetPurple(); colorstack.push_back(color); colorchange = true; break;
         case ST_PINK: color = theApp.GetPink(); colorstack.push_back(color); colorchange = true; break;

         case ST_RED_END:
         case ST_GREEN_END:
         case ST_BLUE_END:
         case ST_YELLOW_END:
         case ST_OLIVE_END:
         case ST_LIME_END:
         case ST_ORANGE_END:
         case ST_PURPLE_END:
         case ST_PINK_END: 
         {
            if(colorstack.size() > 0)
            {
               colorstack.pop_back();
               if(colorstack.size() > 0)
               {
                  color = colorstack[colorstack.size()-1];
               }
               else
               {
                  color = normalcolor;
               }
            }
            else
            {
               color = normalcolor;
            }
            colorchange = true;
            break;
         }

         case ST_INVERT: invert = true; break;
         case ST_INVERT_END: invert = false; colorchange = true; break;
                  
         case ST_QUOTE: quote = true;stylechange = true;break;
         case ST_SAMPLE: sample = true;stylechange = true;break;
         case ST_STRIKE: strike = true;stylechange = true;break;
         case ST_ITALIC: italic = true;stylechange = true;break;
         case ST_BOLD: bold = true;stylechange = true;break;
         case ST_UNDERLINE: underline = true;stylechange = true;break;
         case ST_SPOILER: spoiler = true;stylechange = true;break;
         case ST_CODE: code = true;stylechange = true;break;
         case ST_LINK: link = true;stylechange = true; color = normalcolor; colorstack.push_back(color); colorchange = true; break;
         case ST_IMAGE_LINK: imagelink = true;stylechange = true; color = normalcolor; colorstack.push_back(color); colorchange = true; break;

         case ST_QUOTE_END: quote = false;stylechange = true;break;
         case ST_SAMPLE_END: sample = false;stylechange = true;break;
         case ST_STRIKE_END: strike = false;stylechange = true;break;
         case ST_ITALIC_END: italic = false;stylechange = true;break;
         case ST_BOLD_END: bold = false;stylechange = true;break;
         case ST_UNDERLINE_END: underline = false;stylechange = true;break;
         case ST_SPOILER_END: spoiler = false;stylechange = true;break;
         case ST_CODE_END: code = false;stylechange = true;break;
         case ST_LINK_END: 
            {
               link = false;
               stylechange = true;
               
               if(colorstack.size() > 0)
               {
                  colorstack.pop_back();
                  if(colorstack.size() > 0)
                  {
                     color = colorstack[colorstack.size()-1];
                  }
                  else
                  {
                     color = normalcolor;
                  }
               }
               else
               {
                  color = normalcolor;
               }
               colorchange = true;
            }
            break;
         case ST_IMAGE_LINK_END: 
            {
               imagelink = false;
               stylechange = true;
               
               if(colorstack.size() > 0)
               {
                  colorstack.pop_back();
                  if(colorstack.size() > 0)
                  {
                     color = colorstack[colorstack.size()-1];
                  }
                  else
                  {
                     color = normalcolor;
                  }
               }
               else
               {
                  color = normalcolor;
               }
               colorchange = true;
            }
            break;
         }

         if(colorchange)
         {
            ::SetTextColor(hDC,color);
            colorchange = false;
         }

         if(stylechange)
         {
            if(hPreviousFont != NULL)
            {
               MySelectFont(hDC, hPreviousFont);
            }
            if(hCreatedFont != NULL)
            {
               ::DeleteObject(hCreatedFont);
            }
            
            int fsize = theApp.GetFontHeight();
            if(sample)
            {
               fsize = theApp.GetSampleFontHeight();
            }
            int weight = FW_NORMAL;
            if(bold) weight = FW_EXTRABOLD;
            const wchar_t *font = theApp.GetNormalFontName();
            if(code)
            {
               font = theApp.GetCodeFontName();
            }
            else if(quote)
            {
               font = theApp.GetQuotedFontName();
            }
            hCreatedFont = ::CreateFontW(fsize,0,0,0,weight,italic,underline,strike,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_DONTCARE,font);
            hPreviousFont = MySelectFont(hDC, hCreatedFont);
            stylechange = false;
         }

         int start = (*it).m_pos;
         it++;
         int finish = numchars;
         if(it != end)
         {
            finish = __min((*it).m_pos,numchars);
         }

         if(finish > start)
         {
            if(spoiler)
            {
               RECT spoilrect;
               spoilrect.left = x;
               spoilrect.bottom = rect.bottom;
               spoilrect.top = rect.top;

               spoilrect.right = x;
               const int *work = charwidths + start;
               const int *end = work + (finish - start);
               while(work < end)
               {
                  spoilrect.right += *work;
                  work++;
               }
               ::FillRect(hDC,&spoilrect,m_spoilerbrush);
            }
            else
            {
               if(invert)
               {
                  RECT invertrect;
                  invertrect.left = x;
                  invertrect.bottom = rect.bottom + 1;
                  if(sample)
                  {
                     invertrect.top = rect.top + abs(theApp.GetFontHeight() - theApp.GetSampleFontHeight());
                  }
                  else
                  {
                     invertrect.top = rect.top;
                  }

                  invertrect.right = x + 1;
                  const int *work = charwidths + start;
                  const int *end = work + (finish - start);
                  while(work < end)
                  {
                     invertrect.right += *work;
                     work++;
                  }

                  HBRUSH newbrush = ::CreateSolidBrush(color);
                  HBRUSH oldbrush = (HBRUSH)::SelectObject(hDC,newbrush);
                  HPEN oldpen = (HPEN)::SelectObject(hDC,m_nullpen);
                  
                  int roundamount = (invertrect.bottom - invertrect.top) / 3;
                  ::RoundRect(hDC,invertrect.left, invertrect.top, invertrect.right, invertrect.bottom, roundamount, roundamount);

                  ::SelectObject(hDC,oldpen);
                  ::SelectObject(hDC,oldbrush);
                  ::DeleteObject(newbrush);
                  ::SetTextColor(hDC,theApp.GetBackgroundColor());
               }
               MyTextOut(hDC, x, rect.bottom - rise, sample, text.Str() + start, finish - start, charwidths + start, &rect, bComplexShapeText);
               //::ExtTextOutW(hDC, x, rect.bottom, 0, NULL, text.Str() + start, finish - start, charwidths + start);
            }
         }
                  
         charsdone += (finish - start);
         // update x
         for(int j = start; j < finish; j++)
         {
            x += charwidths[j];
         }

         if(charsdone == numchars)
         {
            it = end;
         }
      }
   }

   if(hPreviousFont != NULL)
   {
      MySelectFont(hDC, hPreviousFont);
   }
   if(hCreatedFont != NULL)
   {
      ::DeleteObject(hCreatedFont);
   }

   ::SetTextAlign(hDC,TA_LEFT|TA_BOTTOM);
}

void CLampDoc::DrawBodyText(HDC hDC, 
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
                            const RECT *pClipRect/*=NULL*/)
{
   int y = rect.top + 4 + theApp.GetTextHeight();

   int rise = theApp.GetDescent();

   ::SetTextAlign(hDC,TA_LEFT|TA_BASELINE);

   std::vector<COLORREF> colorstack;
   bool quote = false;
   bool sample = false;
   bool strike = false;
   bool italic = false;
   bool bold = false;
   bool underline = false;
   bool spoiler = false;
   bool code = false;
   bool link = false;
   bool imagelink = false;
   bool fade = false;
   COLORREF color;
   COLORREF normalcolor = theApp.GetPostTextColor();
   COLORREF backcolor = theApp.GetBackgroundColor();
   COLORREF fadecolor = RGB((GetRValue(normalcolor) + GetRValue(backcolor)) / 2,
                            (GetGValue(normalcolor) + GetGValue(backcolor)) / 2,
                            (GetBValue(normalcolor) + GetBValue(backcolor)) / 2);

   bool colorchange = false;
   bool stylechange = false;
   HFONT hCreatedFont = NULL;
   HFONT hPreviousFont = NULL;

   // setup initial font stuff
   ::SetTextColor(hDC,normalcolor);
   hPreviousFont = MySelectFont(hDC, m_normalfont);

   // loop for every line
   for(size_t i=0; i < linesizes.size(); i++)
   {
      if(linetypes[i].m_bIsText)
      {
         int codeline = rect.left + 5 - 2;
         int x = rect.left + 5;
         const UCChar *pLineText = lines_of_text[i];
         const int* pLineWidths = charsizes[i];
         const int numchars = linesizes[i];

         if(code)
         {
            HPEN newpen = ::CreatePen(PS_SOLID,0,theApp.GetBranchColor());
            HPEN oldpen = (HPEN)::SelectObject(hDC,newpen);
            ::MoveToEx(hDC, codeline, y - theApp.GetTextHeight(),NULL);
            ::LineTo(hDC, codeline, y);
            ::SelectObject(hDC,oldpen);
            ::DeleteObject(newpen);
         }

         if(linetags[i].size() == 0)
         {
            // if there are no tags, 
            // output everything
            if(spoiler)
            {
               RECT spoilrect;
               spoilrect.left = x;
               spoilrect.bottom = y;
               spoilrect.top = y - (theApp.GetTextHeight() - 2);

               spoilrect.right = x;
               const int *work = pLineWidths;
               const int *end = work + numchars;
               while(work < end)
               {
                  spoilrect.right += *work;
                  work++;
               }
               ::FillRect(hDC,&spoilrect,m_spoilerbrush);
               spoilers.push_back(spoilrect);
            }
            else
            {
               if(link || imagelink)
               {
                  RECT linkrect;
                  linkrect.left = x;
                  linkrect.bottom = y;
                  linkrect.top = y - (theApp.GetTextHeight() - 2);

                  linkrect.right = x;
                  const int *work = pLineWidths;
                  const int *end = work + numchars;
                  while(work < end)
                  {
                     linkrect.right += *work;
                     work++;
                  }
                  if(link)links.push_back(linkrect);
                  else imagelinks.push_back(linkrect);
               }
               MyTextOut(hDC, x, y - rise, sample, pLineText, numchars, pLineWidths,pClipRect, bComplexShapeText);
               //::ExtTextOutW(hDC, x, y, 0, NULL, pLineText, numchars, pLineWidths);
            }
         }
         else 
         {
            std::vector<shacktagpos>::iterator it = linetags[i].begin();
            std::vector<shacktagpos>::iterator end = linetags[i].end();

            // if the first tag doesn't begin at 0, or there are no tags, 
            // output that part first
            if((*it).m_pos != 0)
            {
               if(spoiler)
               {
                  RECT spoilrect;
                  spoilrect.left = x;
                  spoilrect.bottom = y;
                  spoilrect.top = y - (theApp.GetTextHeight() - 2);

                  spoilrect.right = x;
                  const int *work = pLineWidths;
                  const int *end = work + (*it).m_pos;
                  while(work < end)
                  {
                     spoilrect.right += *work;
                     work++;
                  }
                  ::FillRect(hDC,&spoilrect,m_spoilerbrush);
                  spoilers.push_back(spoilrect);
               }
               else
               {
                  if(link || imagelink)
                  {
                     RECT linkrect;
                     linkrect.left = x;
                     linkrect.bottom = y;
                     linkrect.top = y - (theApp.GetTextHeight() - 2);

                     linkrect.right = x;
                     const int *work = pLineWidths;
                     const int *end = work + (*it).m_pos;
                     while(work < end)
                     {
                        linkrect.right += *work;
                        work++;
                     }
                     if(link)links.push_back(linkrect);
                     else imagelinks.push_back(linkrect);
                  }
                  MyTextOut(hDC, x, y - rise, sample, pLineText, (*it).m_pos, pLineWidths,pClipRect, bComplexShapeText);
                  //::ExtTextOutW(hDC, x, y, 0, NULL, pLineText, (*it).m_pos, pLineWidths);
               }
               
               // update x
               for(int j = 0; j < (*it).m_pos; j++)
               {
                  x += pLineWidths[j];
               }
            }

            // loop for every shack tag range
            while(it != end)
            {
               switch((*it).m_tag)
               {
               case ST_RED: color = theApp.GetRed(); colorstack.push_back(color); colorchange = true; break;
               case ST_GREEN: color = theApp.GetGreen(); colorstack.push_back(color); colorchange = true; break;
               case ST_BLUE: color = theApp.GetBlue(); colorstack.push_back(color); colorchange = true; break;
               case ST_YELLOW: color = theApp.GetYellow(); colorstack.push_back(color); colorchange = true; break;
               case ST_OLIVE: color = theApp.GetOlive(); colorstack.push_back(color); colorchange = true; break;
               case ST_LIME: color = theApp.GetLime(); colorstack.push_back(color); colorchange = true; break;
               case ST_ORANGE: color = theApp.GetOrange(); colorstack.push_back(color); colorchange = true; break;
               case ST_PURPLE: color = theApp.GetPurple(); colorstack.push_back(color); colorchange = true; break;
               case ST_PINK: color = theApp.GetPink(); colorstack.push_back(color); colorchange = true; break;
               case ST_FADE: color = fadecolor; colorstack.push_back(color); colorchange = true; break;

               case ST_RED_END:
               case ST_GREEN_END:
               case ST_BLUE_END:
               case ST_YELLOW_END:
               case ST_OLIVE_END:
               case ST_LIME_END:
               case ST_ORANGE_END:
               case ST_PURPLE_END:
               case ST_PINK_END: 
               case ST_FADE_END: 
               {
                  if(colorstack.size() > 0)
                  {
                     colorstack.pop_back();
                     if(colorstack.size() > 0)
                     {
                        color = colorstack[colorstack.size()-1];
                     }
                     else
                     {
                        color = normalcolor;
                     }
                  }
                  else
                  {
                     color = normalcolor;
                  }
                  colorchange = true;
                  break;
               }

               case ST_QUOTE: quote = true;stylechange = true;break;
               case ST_SAMPLE: sample = true;stylechange = true;break;
               case ST_STRIKE: strike = true;stylechange = true;break;
               case ST_ITALIC: italic = true;stylechange = true;break;
               case ST_BOLD: bold = true;stylechange = true;break;
               case ST_UNDERLINE: underline = true;stylechange = true;break;
               case ST_SPOILER: spoiler = true;stylechange = true;break;
               case ST_CODE: 
                  {
                     if(!code)
                     {
                        HPEN newpen = ::CreatePen(PS_SOLID,0,theApp.GetBranchColor());
                        HPEN oldpen = (HPEN)::SelectObject(hDC,newpen);
                        ::MoveToEx(hDC, codeline, y - theApp.GetTextHeight(),NULL);
                        ::LineTo(hDC, codeline, y);
                        ::SelectObject(hDC,oldpen);
                        ::DeleteObject(newpen);
                     }
                     code = true;
                     stylechange = true;
                  }
                  break;

               case ST_LINK: link = true;stylechange = true; color = m_link_color; colorstack.push_back(color); colorchange = true; break;
               case ST_IMAGE_LINK: imagelink = true;stylechange = true; color = m_image_link_color; colorstack.push_back(color); colorchange = true; break;

               case ST_QUOTE_END: quote = false;stylechange = true;break;
               case ST_SAMPLE_END: sample = false;stylechange = true;break;
               case ST_STRIKE_END: strike = false;stylechange = true;break;
               case ST_ITALIC_END: italic = false;stylechange = true;break;
               case ST_BOLD_END: bold = false;stylechange = true;break;
               case ST_UNDERLINE_END: underline = false;stylechange = true;break;
               case ST_SPOILER_END: spoiler = false;stylechange = true;break;
               case ST_CODE_END: code = false;stylechange = true;break;
               case ST_LINK_END: 
                  {
                     link = false;
                     stylechange = true;
                     
                     if(colorstack.size() > 0)
                     {
                        colorstack.pop_back();
                        if(colorstack.size() > 0)
                        {
                           color = colorstack[colorstack.size()-1];
                        }
                        else
                        {
                           color = normalcolor;
                        }
                     }
                     else
                     {
                        color = normalcolor;
                     }
                     colorchange = true;
                  }
                  break;
               
               case ST_IMAGE_LINK_END: 
                  {
                     imagelink = false;
                     stylechange = true;
                     
                     if(colorstack.size() > 0)
                     {
                        colorstack.pop_back();
                        if(colorstack.size() > 0)
                        {
                           color = colorstack[colorstack.size()-1];
                        }
                        else
                        {
                           color = normalcolor;
                        }
                     }
                     else
                     {
                        color = normalcolor;
                     }
                     colorchange = true;
                  }
                  break;
               }

               if(colorchange)
               {
                  ::SetTextColor(hDC,color);
                  colorchange = false;
               }

               if(stylechange)
               {
                  if(hPreviousFont != NULL)
                  {
                     MySelectFont(hDC, hPreviousFont);
                  }
                  if(hCreatedFont != NULL)
                  {
                     ::DeleteObject(hCreatedFont);
                  }
                  
                  int fsize = theApp.GetFontHeight();
                  if(sample)
                  {
                     fsize = theApp.GetSampleFontHeight();
                  }
                  int weight = FW_NORMAL;
                  if(bold) weight = FW_EXTRABOLD;
                  const wchar_t *font = theApp.GetNormalFontName();
                  if(code)
                  {
                     font = theApp.GetCodeFontName();
                  }
                  else if(quote)
                  {
                     font = theApp.GetQuotedFontName();
                  }
                  hCreatedFont = ::CreateFontW(fsize,0,0,0,weight,italic,underline|link|imagelink,strike,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_DONTCARE,font);
                  hPreviousFont = MySelectFont(hDC, hCreatedFont);
                  stylechange = false;
               }

               int start = (*it).m_pos;
               it++;
               int finish = numchars;
               if(it != end)
               {
                  finish = (*it).m_pos;
               }
               
               if(spoiler)
               {
                  RECT spoilrect;
                  spoilrect.left = x;
                  spoilrect.bottom = y;
                  spoilrect.top = y - (theApp.GetTextHeight() - 2);

                  spoilrect.right = x;
                  const int *work = pLineWidths + start;
                  const int *end = work + (finish - start);
                  while(work < end)
                  {
                     spoilrect.right += *work;
                     work++;
                  }
                  ::FillRect(hDC,&spoilrect,m_spoilerbrush);
                  spoilers.push_back(spoilrect);
               }
               else
               {
                  if(link || imagelink)
                  {
                     RECT linkrect;
                     linkrect.left = x;
                     linkrect.bottom = y;
                     linkrect.top = y - (theApp.GetTextHeight() - 2);

                     linkrect.right = x;
                     const int *work = pLineWidths + start;
                     const int *end = work + (finish - start);
                     while(work < end)
                     {
                        linkrect.right += *work;
                        work++;
                     }
                     if(link)links.push_back(linkrect);
                     else imagelinks.push_back(linkrect);
                  }
                  MyTextOut(hDC, x, y - rise, sample, pLineText + start, finish - start, pLineWidths + start,pClipRect, bComplexShapeText);
                  //::ExtTextOutW(hDC, x, y, 0, NULL, pLineText + start, finish - start, pLineWidths + start);
               }
               
               // update x
               for(int j = start; j < finish; j++)
               {
                  x += pLineWidths[j];
               }
            }
         }
      }
      else // ! linetypes[i].m_bIsText
      {
         // draw image

         if(linetypes[i].m_bIsThumb)
         {
            CDCSurface *pImage = theApp.GetLinkedImageThumb(linetypes[i].m_image_index);
            if(pImage != NULL)
            {
               RECT output;
               output.left = rect.left + 5;
               output.right = rect.right - 5;
               output.top = y - theApp.GetTextHeight();
               output.bottom = (y - theApp.GetTextHeight()) + linetypes[i].m_height;

               if(output.right - output.left > pImage->GetWidth())
               {
                  output.right = output.left + pImage->GetWidth();
               }

               pImage->StretchBlit(hDC,output);

               thumbs.push_back(output);
            }
         }
         else
         {
            CDCSurface *pImage = theApp.GetLinkedImage(linetypes[i].m_image_index);
            if(pImage != NULL)
            {
               RECT output;
               output.left = rect.left + 5;
               output.right = rect.right - 5;
               output.top = y - theApp.GetTextHeight();
               output.bottom = (y - theApp.GetTextHeight()) + linetypes[i].m_height;

               if(output.right - output.left > pImage->GetWidth())
               {
                  output.right = output.left + pImage->GetWidth();
               }

               pImage->StretchBlit(hDC,output);

               images.push_back(output);
            }
         }
      }
            
      y += linetypes[i].m_height;
   }

   if(hPreviousFont != NULL)
   {
      MySelectFont(hDC, hPreviousFont);
   }
   if(hCreatedFont != NULL)
   {
      ::DeleteObject(hCreatedFont);
   }

   y += 4;

   ::SetTextAlign(hDC,TA_LEFT|TA_BOTTOM);
}

void CLampDoc::DrawNewMessagesTab(HDC hDC, RECT &rect, const UCChar *pChar, int *widths, size_t numchars, bool bHover)
{
   HBRUSH oldbrush = NULL;

   if(bHover)
   {
      oldbrush = (HBRUSH)::SelectObject(hDC,m_spoilerbrush);
   }
   else
   {
      oldbrush = (HBRUSH)::SelectObject(hDC,m_rootbackgroundbrush);
   }

   HPEN oldpen = (HPEN)::SelectObject(hDC,m_roottoppen);

   POINT points[4];
   points[0].x = rect.left;
   points[0].y = rect.top;
   points[1].x = rect.right;
   points[1].y = rect.top;
   points[2].x = rect.right - 5;
   points[2].y = rect.bottom;
   points[3].x = rect.left + 5;
   points[3].y = rect.bottom;
   ::Polygon(hDC, points, 4);
   
   ::SelectObject(hDC,oldbrush);
   ::SelectObject(hDC,oldpen);

   HFONT oldfont = MySelectFont(hDC,m_normalfont);
   ::SetTextColor(hDC,theApp.GetPostTextColor());
   ::SetBkMode(hDC,TRANSPARENT);
   ::SetTextAlign(hDC,TA_LEFT|TA_BOTTOM);

   ::ExtTextOutW(hDC, rect.left + 5, rect.bottom, 0, NULL, pChar, numchars, widths);
   
   MySelectFont(hDC,m_boldfont);
}


void CLampDoc::DrawRootAuthor(HDC hDC, RECT &rect,UCString &author, COLORREF AuthorColor, bool bFade/*= false*/, bool m_bIsInbox/*=true*/)
{
   HFONT oldfont = MySelectFont(hDC,m_miscfont);
   ::SetTextColor(hDC,theApp.GetMiscPostTextColor());

   if(m_bIsInbox)
   {
      ::ExtTextOutW(hDC, rect.left + 5, rect.bottom, 0, NULL, L"By:", 3, NULL);
   }
   else
   {
      ::ExtTextOutW(hDC, rect.left + 5, rect.bottom, 0, NULL, L"To:", 3, NULL);
   }

   MySelectFont(hDC,m_boldfont);

   if(bFade)
   {
      COLORREF backcolor = theApp.GetPostBackgroundColor();

      ::SetTextColor(hDC,RGB((GetRValue(AuthorColor) + GetRValue(backcolor)) / 2,
                             (GetGValue(AuthorColor) + GetGValue(backcolor)) / 2,
                             (GetBValue(AuthorColor) + GetBValue(backcolor)) / 2));
   }
   else
   {
      ::SetTextColor(hDC,AuthorColor);
   }

   ::ExtTextOutW(hDC, rect.left + theApp.GetCellHeight() + 5, rect.bottom, 0, NULL, author, author.Length(), NULL);

   MySelectFont(hDC,oldfont);
}

void CLampDoc::DrawDate(HDC hDC, RECT &rect, UCString &date, COLORREF ago_color, bool bGetExtents/*=false*/)
{
   HFONT oldfont = MySelectFont(hDC,m_miscfont);
   ::SetTextColor(hDC,ago_color);
   ::SetTextAlign(hDC,TA_RIGHT|TA_BOTTOM);
   ::ExtTextOutW(hDC, rect.right - 5, rect.bottom - 5, 0, NULL, date, date.Length(), NULL);

   if(bGetExtents)
   {
      SIZE size;
      ::GetTextExtentPoint32W(hDC,date,date.Length(),&size);
      rect.left = rect.right - size.cx;
   }

   ::SetTextAlign(hDC,TA_LEFT|TA_BOTTOM);
   MySelectFont(hDC,oldfont);
}

void CLampDoc::DrawCollapseNote(HDC hDC, RECT &rect)
{
   HFONT oldfont = MySelectFont(hDC,m_miscfont);
   ::SetTextColor(hDC,theApp.GetMiscPostTextColor());

   ::ExtTextOutW(hDC, rect.left + 5, rect.bottom, 0, NULL, L"( Collapsed... )", 16, NULL);

   MySelectFont(hDC,oldfont);
}

void CLampDoc::DrawRepliesHint(HDC hDC, RECT &rect, int m_reportedchildcount)
{
   HFONT oldfont = MySelectFont(hDC,m_miscfont);
   ::SetTextColor(hDC,theApp.GetMiscPostTextColor());

   ::SetTextAlign(hDC,TA_LEFT|TA_BOTTOM|TA_UPDATECP);
   ::MoveToEx(hDC, rect.left + 5, rect.bottom - 5, NULL); 
   ::ExtTextOutW(hDC, rect.left + 5, rect.bottom - 5, 0, NULL, L"Click to see all ", 17, NULL);
   MySelectFont(hDC,m_miscboldfont);
   UCString text = m_reportedchildcount;
   ::SetTextColor(hDC,theApp.GetMyPostColor());
   ::ExtTextOutW(hDC, rect.left + 5 + (theApp.GetMiscFontHeight() * -7), rect.bottom - 5, 0, NULL, text, text.Length(), NULL);
   MySelectFont(hDC,m_miscfont);
   ::SetTextColor(hDC,theApp.GetMiscPostTextColor());
   ::ExtTextOutW(hDC, rect.left + 5 + (theApp.GetMiscFontHeight() * -7) + ((theApp.GetFontHeight() / -2) * text.Length()), rect.bottom - 5, 0, NULL, L" replies", 8, NULL);

   ::SetTextAlign(hDC,TA_LEFT|TA_BOTTOM);

   MySelectFont(hDC,oldfont);
}

void CLampDoc::DrawBranch(HDC hDC, RECT &rect, indenttype type, int shade, newness Newness)
{
   HPEN oldpen;
   
   switch(Newness)
   {
   case N_NEW:
      oldpen = (HPEN)::SelectObject(hDC,m_branchpenisnew);
      shade = 10;
      break;
   case N_LAST:
      oldpen = (HPEN)::SelectObject(hDC,m_branchpenislast);
      break;
   case N_OLD:
   default:
      oldpen = (HPEN)::SelectObject(hDC,m_branchpen);
      break;
   }
   

   // rect is 20, theApp.GetTextHeight()

   int stem = theApp.GetTextHeight() / 2;

   switch(type)
   {
   case IT_VERT:
      {
         ::MoveToEx(hDC,rect.left + 8, rect.top,NULL);
         ::LineTo(hDC,rect.left + 8, rect.bottom);
         ::SelectObject(hDC,oldpen);
      }
      break;
   case IT_TO_ME_FORK:
      {
         ::MoveToEx(hDC,rect.left + 8, rect.top,NULL);
         ::LineTo(hDC,rect.left + 8, rect.bottom);

         if(shade > 2 && theApp.FlaredBranches())
         {
            int shadecount = shade;
            while(shade > 0)
            {
               ::SelectObject(hDC,oldpen);
               HPEN newpen = ::CreatePen(PS_SOLID,0,theApp.GetBranchColorShade(shade, Newness));
               oldpen = (HPEN)::SelectObject(hDC,newpen);

               ::MoveToEx(hDC,rect.left + 8, rect.top + stem,NULL);
               ::LineTo(hDC,rect.right - 2, rect.top + stem - ((shadecount - shade) >> 1));
               ::MoveToEx(hDC,rect.left + 8, rect.top + stem,NULL);
               ::LineTo(hDC,rect.right - 2, rect.top + stem + ((shadecount - shade) >> 1));

               ::SelectObject(hDC,oldpen);
               ::DeleteObject(newpen);
               shade-=2;
            }
         }
         else
         {
            ::MoveToEx(hDC,rect.left + 8, rect.top + stem,NULL);
            ::LineTo(hDC,rect.right - 2, rect.top + stem);
         }
      }
      break;
   case IT_TO_ME_END:
      {
         ::MoveToEx(hDC,rect.left + 8, rect.top,NULL);
         ::LineTo(hDC,rect.left + 8, rect.top + stem);

         if(shade > 2 && theApp.FlaredBranches())
         {
            int shadecount = shade;
            while(shade > 0)
            {
               ::SelectObject(hDC,oldpen);
               HPEN newpen = ::CreatePen(PS_SOLID,0,theApp.GetBranchColorShade(shade, Newness));
               oldpen = (HPEN)::SelectObject(hDC,newpen);

               ::MoveToEx(hDC,rect.left + 8, rect.top + stem,NULL);
               ::LineTo(hDC,rect.right - 2, rect.top + stem - ((shadecount - shade) >> 1));
               ::MoveToEx(hDC,rect.left + 8, rect.top + stem,NULL);
               ::LineTo(hDC,rect.right - 2, rect.top + stem + ((shadecount - shade) >> 1));

               ::SelectObject(hDC,oldpen);
               ::DeleteObject(newpen);
               shade-=2;
            }
         }
         else
         {
            ::MoveToEx(hDC,rect.left + 8, rect.top + stem,NULL);
            ::LineTo(hDC,rect.right - 2, rect.top + stem);
         }
      }
      break;
   }

   ::SelectObject(hDC,oldpen);
}

bool CLampDoc::FindNext(const UCChar *search, unsigned int &textselectionpost, int &selectionstart, int &selectionend)
{
   bool bFound = false;
   std::list<ChattyPost*>::iterator it = m_rootposts.begin();
   std::list<ChattyPost*>::iterator end = m_rootposts.end();

   while(it != end)
   {
      bFound = (*it)->FindNext(search, textselectionpost, selectionstart, selectionend);
      if(bFound)
      {
         break;
      }
      it++;
   }

   return bFound;
}

void CLampDoc::CalcLineTags(std::vector<shacktagpos> &shacktags, std::vector<shacktagpos> &thislinetags, int beginpos, int endpos)
{
   thislinetags.clear();
   if(shacktags.size() > 0)
   {
      std::vector<shacktagpos>::iterator it = shacktags.begin();
      std::vector<shacktagpos>::iterator end = shacktags.end();
      
      while(it < end)
      {
         if((*it).m_pos >= beginpos &&
            (*it).m_pos < endpos)
         {
            // include it
            thislinetags.push_back(shacktagpos((*it).m_tag,(*it).m_pos - beginpos));
         }
         it++;
      }
   }
}


void CLampDoc::MySetTitle(LPCTSTR lpszTitle)
{
   m_actualtitle = lpszTitle; 
	SetTitle(m_actualtitle);
   theApp.UpdateTabSizes();
}

void CLampDoc::SetTitle(LPCTSTR lpszTitle)
{
	CDocument::SetTitle(m_actualtitle);
}

bool CLampDoc::LolTagPost(unsigned int post_id, loltagtype tag)
{
   bool result = false;

   if(tag != LTT_NONE)
   {
      //   http://thomwetzel.com/greasemonkey/shacklol/report.php?who=[username]&what=24079617&tag=lol&version=-1
      CWaitCursor wait;
      char*         data = NULL;

      UCString path = L"/greasemonkey/shacklol/report.php?who=";
      path += theApp.GetUsername();
      path += L"&what=";
      path += post_id;
      path += L"&tag=";

      switch(tag)
      {
      case LTT_LOL: path += L"lol";break;
      case LTT_INF: path += L"inf";break;
      case LTT_UNF: path += L"unf";break;
      case LTT_TAG: path += L"tag";break;
      case LTT_WTF: path += L"wtf";break;
      }

      path += L"&version=-1";

      StartDownload(theApp.GetLolHostName(),
                    path,
                    DT_SUBMIT_LOLVOTE,
                    0);

      result = true;
   }   

	return result;
}


bool CLampDoc::GetAuthorInfo(unsigned int post_id)
{
   bool result = true;

   ChattyPost *post = FindPost(post_id);
   if(post != NULL)
   {
      UCString author = post->GetAuthor();

      UCString path = L"/users/";
      path += author;
      path += L".xml";      

      StartDownload(theApp.GetUserHostName(),
                    path,
                    DT_AUTHOR,
                    post_id);
   }

   return true;
}



void CLampDoc::InvalidateSkin()
{
   if(m_backgroundbrush != NULL)
   {
      ::DeleteObject(m_backgroundbrush);
   }

   if(m_spoilerbrush != NULL)
   {
      ::DeleteObject(m_spoilerbrush);
   }

   if(m_rootbackgroundbrush != NULL)
   {
      ::DeleteObject(m_rootbackgroundbrush);
   }

   if(m_replyexpandedbackgroundbrush != NULL)
   {
      ::DeleteObject(m_replyexpandedbackgroundbrush);
   }

   if(m_infpen != NULL)
   {
      ::DeleteObject(m_infpen);
   }

   if(m_nwspen != NULL)
   {
      ::DeleteObject(m_nwspen);
   }

   if(m_stupidpen != NULL)
   {
      ::DeleteObject(m_stupidpen);
   }

   if(m_offtopicpen != NULL)
   {
      ::DeleteObject(m_offtopicpen);
   }

   if(m_politicalpen != NULL)
   {
      ::DeleteObject(m_politicalpen);
   }

   if(m_nullpen != NULL)
   {
      ::DeleteObject(m_nullpen);
   }

   if(m_roottoppen != NULL)
   {
      ::DeleteObject(m_roottoppen);
   }

   if(m_branchpen != NULL)
   {
      ::DeleteObject(m_branchpen);
   }

   if(m_branchpenisnew != NULL)
   {
      ::DeleteObject(m_branchpenisnew);
   }

   if(m_branchpenislast != NULL)
   {
      ::DeleteObject(m_branchpenislast);
   }

   if(m_normalfont != NULL)
   {
      ::DeleteObject(m_normalfont);
   }

   if(m_boldfont != NULL)
   {
      ::DeleteObject(m_boldfont);
   }

   if(m_pagefont != NULL)
   {
      ::DeleteObject(m_pagefont);
   }

   if(m_miscfont != NULL)
   {
      ::DeleteObject(m_miscfont);
   }

   if(m_miscunderlinefont != NULL)
   {
      ::DeleteObject(m_miscunderlinefont);
   }

   if(m_normalunderlinefont != NULL)
   {
      ::DeleteObject(m_normalunderlinefont);
   }

   if(m_miscboldfont != NULL)
   {
      ::DeleteObject(m_miscboldfont);
   }

   m_spoilerbrush = ::CreateSolidBrush(theApp.GetSpoilerColor());

   if(theApp.GetBackground()->GetBitmap() != NULL)
   {
      m_backgroundbrush = ::CreatePatternBrush(theApp.GetBackground()->GetBitmap());
   }
   else
   {
      m_backgroundbrush = ::CreateSolidBrush(theApp.GetBackgroundColor());
   }

   if(theApp.GetRootPostBackground()->GetBitmap() != NULL)
   {
      m_rootbackgroundbrush = ::CreatePatternBrush(theApp.GetRootPostBackground()->GetBitmap());
   }
   else
   {
      m_rootbackgroundbrush = ::CreateSolidBrush(theApp.GetRootPostBackgroundColor());
   }

   if(theApp.GetPostBackground()->GetBitmap() != NULL)
   {
      m_replyexpandedbackgroundbrush = ::CreatePatternBrush(theApp.GetPostBackground()->GetBitmap());
   }
   else
   {
      m_replyexpandedbackgroundbrush = ::CreateSolidBrush(theApp.GetPostBackgroundColor());
   }

   m_widthofaverageprofilegroup = 0;

   m_roottoppen = ::CreatePen(PS_SOLID,0,theApp.GetPostEdgeColor());

   m_infpen = ::CreatePen(PS_SOLID,0,theApp.GetPostINFEdgeColor());
   m_nwspen = ::CreatePen(PS_SOLID,0,theApp.GetPostNWSEdgeColor());
   m_stupidpen = ::CreatePen(PS_SOLID,0,theApp.GetPostStupidEdgeColor());
   m_offtopicpen = ::CreatePen(PS_SOLID,0,theApp.GetPostOffTopicEdgeColor());
   m_politicalpen = ::CreatePen(PS_SOLID,0,theApp.GetPostPoliticalEdgeColor());
   m_nullpen = ::CreatePen(PS_NULL,0,0);

   m_branchpen = ::CreatePen(PS_SOLID,0,theApp.GetBranchColorShade(0,N_OLD));
   m_branchpenisnew = ::CreatePen(PS_SOLID,0,theApp.GetBranchColorShade(0,N_NEW));
   m_branchpenislast = ::CreatePen(PS_SOLID,0,theApp.GetBranchColorShade(0,N_LAST));
   m_normalfont = ::CreateFontW(theApp.GetFontHeight(),0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_DONTCARE,theApp.GetNormalFontName());
      
   m_miscfont = ::CreateFontW(theApp.GetMiscFontHeight(),0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_DONTCARE,theApp.GetNormalFontName());
   m_boldfont = ::CreateFontW(theApp.GetFontHeight(),0,0,0,FW_EXTRABOLD,0,0,0,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_DONTCARE,theApp.GetNormalFontName());
   m_pagefont = ::CreateFontW(-13,0,0,0,FW_EXTRABOLD,0,0,0,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_DONTCARE,theApp.GetNormalFontName());
   m_miscboldfont = ::CreateFontW(theApp.GetMiscFontHeight(),0,0,0,FW_EXTRABOLD,0,0,0,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_DONTCARE,theApp.GetNormalFontName());

   m_miscunderlinefont = ::CreateFontW(theApp.GetMiscFontHeight(),0,0,0,FW_NORMAL,0,1,0,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_DONTCARE,theApp.GetNormalFontName());
   m_normalunderlinefont = ::CreateFontW(theApp.GetFontHeight(),0,0,0,FW_NORMAL,0,1,0,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_DONTCARE,theApp.GetNormalFontName());


   std::list<ChattyPost*>::iterator it = m_rootposts.begin();
   std::list<ChattyPost*>::iterator end = m_rootposts.end();
   while(it != end)
   {
      if((*it) != NULL)
      {
         (*it)->InvalidateSkin();
      }
      it++;
   }

}


void CLampDoc::GenerateRedirectedIDs()
{
   m_id_redirects.clear();
   std::list<ChattyPost*>::iterator it = m_rootposts.begin();
   std::list<ChattyPost*>::iterator end = m_rootposts.end();
   while(it != end)
   {
      if((*it) != NULL)
      {
         unsigned int oldid = (*it)->GetId();
         unsigned int newid = (unsigned int)(*it);
         (*it)->SetId(newid);
         m_id_redirects[newid] = oldid;
      }
      it++;
   }
}

unsigned int CLampDoc::GetID(unsigned int id)
{
   if(m_id_redirects.size() > 0)
   {
      return m_id_redirects[id];
   }

   return id;
}


void CLampDoc::ClearAllPinnedThreads()
{
   std::list<ChattyPost*>::iterator it = m_rootposts.begin();
   std::list<ChattyPost*>::iterator end = m_rootposts.end();
   while(it != end)
   {
      if((*it) != NULL)
      {
         (*it)->SetPinned(false);
      }
      it++;
   }
}

void CLampDoc::UpdateUnreadShackMessagesCount()
{
   if(m_datatype == DDT_SHACKMSG &&
      m_shackmsgtype == SMT_INBOX &&
      m_page == 1)
   {
      int count = 0;
      std::list<ChattyPost*>::iterator it = m_rootposts.begin();
      std::list<ChattyPost*>::iterator end = m_rootposts.end();
      while(it != end)
      {
         if((*it) != NULL &&
            !(*it)->GetHaveRead())
         {
            count++;
         }
         it++;
      }

      theApp.SetNewMessageCount((size_t)count);
   }
}

void CLampDoc::SetPage(int page)
{
   m_page = (unsigned int)__min((int)m_lastpage,__max(1,page));

   if(m_datatype == DDT_SHACKMSG)
   {
      std::list<ChattyPost*>::iterator it = m_rootposts.begin();
      std::list<ChattyPost*>::iterator end = m_rootposts.end();
      while(it != end)
      {
         if((*it) != NULL)
         {
            delete (*it);
            (*it) = NULL;
         }
         it++;
      }
      m_rootposts.clear();
   }
}

void CLampDoc::UpdateLOLsRecurse()
{
   std::list<ChattyPost*>::iterator it = m_rootposts.begin();
   std::list<ChattyPost*>::iterator end = m_rootposts.end();
   while(it != end)
   {
      if((*it) != NULL)
      {
         (*it)->UpdateLOLsRecurse();
      }
      it++;
   }
}

void CLampDoc::UnloadAllImages()
{
   std::list<ChattyPost*>::iterator it = m_rootposts.begin();
   std::list<ChattyPost*>::iterator end = m_rootposts.end();
   while(it != end)
   {
      if((*it) != NULL)
      {
         (*it)->UnloadAllImagesRecurse();
      }
      it++;
   }
}

void CLampDoc::UnloadImage(unsigned int image_index)
{
   std::list<ChattyPost*>::iterator it = m_rootposts.begin();
   std::list<ChattyPost*>::iterator end = m_rootposts.end();
   while(it != end)
   {
      if((*it) != NULL)
      {
         (*it)->UnloadImageRecurse(image_index);
      }
      it++;
   }
}

void CLampDoc::GetLaunchString(UCString &launch, unsigned int current_id)
{
   switch(m_datatype)
   {
   case DDT_EPICFAILD:
      // do nothing
      break;
   case DDT_PROFILE:
      launch = L"PROFILE:";
      launch += m_profile_user;
      break;
   case DDT_STORY:
      launch = L"LATESTCHATTY";
      break;
   case DDT_THREAD:
      launch = L"chatty?id=";
      launch += current_id;
      break;
   case DDT_LOLS:
      launch = L"LOL";
      if(m_ThingsILOLD)
      {
         launch += L"ILOLD";
      }
      else if(m_ThingsIWrote)
      {
         launch += L"IWROTE";
      }
      else if(m_ThingsUserWrote)
      {
         launch += L"THEYWROTE";
         launch += m_profile_user;
      }
      else
      {
         launch += L"POP";
         launch += m_loltag;
      }
      break;
   case DDT_SEARCH:

      if(m_search_author == theApp.GetUsername() &&
         m_search_parent_author.IsEmpty() &&
         m_search_terms.IsEmpty())
      {
         launch = L"MYCOMMENTS";
      }
      else if(m_search_author.IsEmpty() &&
              m_search_parent_author == theApp.GetUsername() &&
              m_search_terms.IsEmpty())
      {
         launch = L"REPLIESTOME";
      }
      else if(m_search_author.IsEmpty() &&
              m_search_parent_author.IsEmpty() &&
              m_search_terms == theApp.GetUsername())
      {
         launch = L"VANITYSEARCH";
      }
      else
      {
         launch = L"CUSTOMSEARCH:";
         char *enc = url_encode(m_search_author.str8());
         launch += enc;
         free(enc);
         launch += L":";
         enc = url_encode(m_search_parent_author.str8());
         launch += enc;
         free(enc);
         launch += L":";
         enc = url_encode(m_search_terms.str8());
         launch += enc;
         free(enc);
      }
      break;
   case DDT_SHACKMSG:
      launch = L"SHACKMSG_";
      switch(m_shackmsgtype)
      {
      case SMT_INBOX: launch += L"INBOX"; break;
      case SMT_OUTBOX: launch += L"OUTBOX"; break;
      case SMT_ARCHIVE: launch += L"ARCHIVE"; break;
      }
      break;
   }
}



unsigned int CLampDoc::GetNextRoot(ChattyPost *pRootPost)
{
   unsigned int id = 0;

   if(m_datatype != DDT_SHACKMSG)
   {
      std::list<ChattyPost*>::iterator it = m_rootposts.begin();
      std::list<ChattyPost*>::iterator end = m_rootposts.end();

      if(it != end)
      {
         if(pRootPost == NULL)
         {
            while(it != end)
            {
               if(!(*it)->IsCollapsed() &&
                  !(*it)->IsJustText())
               {
                  id = (*it)->GetId();
                  break;
               }
               it++;
            }
         }
         else
         {      
            while(it != end)
            {
               if((*it) == pRootPost)
               {         
                  id = (*it)->GetId();
                  it++;
                  while(it != end)
                  {
                     if(!(*it)->IsCollapsed() &&
                        !(*it)->IsJustText())
                     {
                        id = (*it)->GetId();
                        break;
                     }
                     it++;
                  }
                  break;         
               }

               it++;
            }
         }
      }
   }

   return id;
}

unsigned int CLampDoc::GetPrevRoot(ChattyPost *pRootPost)
{
   unsigned int id = 0;

   if(m_datatype != DDT_SHACKMSG)
   {  
      std::list<ChattyPost*>::iterator first = m_rootposts.begin();
      std::list<ChattyPost*>::iterator it = first;
      std::list<ChattyPost*>::iterator end = m_rootposts.end();

      if(it != end)
      {
         if(pRootPost == NULL)
         {
            bool done = false;
            it = end;
            it--;
            while(!done)
            {
               if(!(*it)->IsCollapsed() &&
                  !(*it)->IsJustText())
               {
                  id = (*it)->GetId();
                  done = true;
               }
               if(it == first)
               {
                  done = true;
               }
               else
               {
                  it--;
               }
            }
         }
         else
         {      
            while(it != end)
            {
               if((*it) == pRootPost)
               {         
                  id = (*it)->GetId();
                  while(it != first)
                  {
                     it--;
                     if(!(*it)->IsCollapsed() &&
                        !(*it)->IsJustText())
                     {
                        id = (*it)->GetId();
                        break;
                     }
                  }

                  break;
               }

               it++;
            }
         }
      }
   }

   return id;
}

void CLampDoc::MakePostAvailable(unsigned int id)
{
   if(theApp.UseShack())
   {
      ChattyPost *post = FindPost(id);

      if(post != NULL)
      {
         if(post->IsPreview() &&
            CheckForContentsRequest(id))
         {
            post->SetRefreshing(true);
            if(m_pView) 
               m_pView->InvalidateEverything();
            // http://www.shacknews.com/frame_chatty.x?root=25755052

            UCString story = L"/frame_chatty.x?root=";

            ChattyPost *rootpost = post->GetRoot();

            story += rootpost->GetId();

            StartDownload(L"www.shacknews.com",
                          story,
                          DT_SHACK_THREAD_CONTENTS,
                          rootpost->GetId(),
                          id);

            //OutputDebugStringA("get thread contents\r");
         }
      }
   }
}

bool CLampDoc::CheckForContentsRequest(unsigned int id)
{
   bool bPerformRequest = true;

   DWORD thistime = ::GetTickCount();

   // replace the id with the root post id

   ChattyPost *post = FindPost(id);
   if(post != NULL)
   {
      ChattyPost *root = post->GetRoot();
      if(root != NULL)
      {
         id = root->GetId();
      }
   }

   // see if he is in the map
   std::map<unsigned int, DWORD>::iterator it = m_id_contents_requests.find(id);
  
   if(it != m_id_contents_requests.end())
   {
      // found it.

      // see if it is older than 5 seconds ago
      if(thistime - it->second > 5000)
      {
         // is old.  allow new request 
      }
      else
      {
         // is recent.  update time and don't allow new request
         m_id_contents_requests[id] = thistime;
         bPerformRequest = false;
      }
   }
   else
   {
      // doesn't exist
      // allow new request and add to map
      m_id_contents_requests[id] = thistime;
   }

   return bPerformRequest;
}

void CLampDoc::RemoveContentsRequest(unsigned int id)
{
   // replace the id with the root post id
   ChattyPost *post = FindPost(id);
   if(post != NULL)
   {
      ChattyPost *root = post->GetRoot();
      if(root != NULL)
      {
         id = root->GetId();
      }
   }

   // see if he is in the map
   std::map<unsigned int, DWORD>::iterator it = m_id_contents_requests.find(id);
  
   if(it != m_id_contents_requests.end())
   {
      // found it.  remove it

      m_id_contents_requests.erase(it);
   }
}

void CLampDoc::FetchNextPage()
{
   if(theApp.UseShack() &&
      m_page + 1 <= m_lastpage)
   {
      m_bFetchingNextPage = true;
      m_page++;

      UCString story = L"/chatty?page=";
      story += m_page;

      StartDownload(L"www.shacknews.com",
                    story,
                    DT_SHACK_CHATTY_INFINATE_PAGE,
                    0);
   }
}

void FindValue(UCString &value, JSONNode::json_iterator &i, const UCChar *name)
{
   value = L"";
   JSONNode::json_iterator j = (*i).begin();
   while (j != (*i).end())
   {
      json_string jname = (*j).name();

      if(jname == name)
      {
         value = (*j).as_string().data();
         break;
      }

      j++;
   }
}

void CLampDoc::ReadProfile(const char *pText, int datasize)
{
   ChattyPost *titleblock_group = new ChattyPost();
   titleblock_group->SetDoc(this);
   titleblock_group->SetId(1);
   m_rootposts.push_back(titleblock_group);

   ChattyPost *profile_group = new ChattyPost();
   profile_group->SetDoc(this);
   profile_group->SetId(2);
   profile_group->SetIsProfileGroup(true);
   m_rootposts.push_back(profile_group);

   ChattyPost *biodata_group = new ChattyPost();
   biodata_group->SetDoc(this);
   biodata_group->SetId(3);
   biodata_group->SetIsProfileGroup(true);
   m_rootposts.push_back(biodata_group);

   ChattyPost *contactinfo_group = new ChattyPost();
   contactinfo_group->SetDoc(this);
   contactinfo_group->SetId(4);
   contactinfo_group->SetIsProfileGroup(true);
   m_rootposts.push_back(contactinfo_group);

   ChattyPost *gaminghandles_group = new ChattyPost();
   gaminghandles_group->SetDoc(this);
   gaminghandles_group->SetId(5);
   gaminghandles_group->SetIsProfileGroup(true);
   m_rootposts.push_back(gaminghandles_group);

   ChattyPost *userbio_group = new ChattyPost();
   userbio_group->SetDoc(this);
   userbio_group->SetId(6);
   m_rootposts.push_back(userbio_group);
   
   UCString temp;
   temp.AppendEncodedString(pText,datasize);

   json_string jsondata = temp;

   JSONNode n;

   try
   {
      n = libjson::parse(jsondata);
   }
   catch(std::invalid_argument)
   {
      // whatever. move on
   }
   
   JSONNode::json_iterator i = n.begin();
   if(i != n.end())
   {
      UCString text;
      UCString data;

      int reg_days = 1;
      UCString reg_date;
      FindValue(reg_date, i, L"join_date");
      if(!reg_date.IsEmpty())
      {
         COleDateTime foo;

         bool bTrimmed = false;
         UCChar *datetext = (UCChar*)reg_date.Str();
         UCChar *end = datetext + reg_date.Length() - 4;
         if(end > datetext &&
            end[0] == L' ' &&
            iswalpha(end[1]) &&
            iswalpha(end[2]) &&
            iswalpha(end[3]))
         {
            *end = 0;
            bTrimmed = false;
         }

         foo.ParseDateTime(datetext,0,MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT));

         if(bTrimmed) *end = L' ';
         
         tm regtime;
         regtime.tm_year = foo.GetYear() - 1900;
         regtime.tm_mon  = foo.GetMonth() - 1;
         regtime.tm_mday = foo.GetDay();
         regtime.tm_yday = foo.GetDayOfYear() - 1;
         regtime.tm_wday = foo.GetDayOfWeek() - 1;
         regtime.tm_hour = foo.GetHour();
         regtime.tm_min  = foo.GetMinute();
         regtime.tm_sec  = foo.GetSecond();

         time_t bar;
         time(&bar);

         tm tm_now;
         localtime_s(&tm_now,&bar);

         int now_seconds = tm_now.tm_sec;
         now_seconds += (tm_now.tm_min * 60);
         now_seconds += (tm_now.tm_hour * 60 * 60);
         now_seconds += (tm_now.tm_yday * 60 * 60 * 24);
         now_seconds += (tm_now.tm_year * 60 * 60 * 24 * 365);

         int reg_seconds = regtime.tm_sec;
         reg_seconds += (regtime.tm_min * 60);
         reg_seconds += (regtime.tm_hour * 60 * 60);
         reg_seconds += (regtime.tm_yday * 60 * 60 * 24);
         reg_seconds += (regtime.tm_year * 60 * 60 * 24 * 365);

         int ago_seconds = now_seconds - reg_seconds;
         double diff = (double)ago_seconds;

         data = L"";

         double day_diff = diff / (60.0 * 60.0 * 24.0);
         if(day_diff >= 1.0)
         {
            reg_days = (int)day_diff;
         }
      }

      int age = 0;
      FindValue(data, i, L"birthdate");
      if(!data.IsEmpty())
      {
         COleDateTime foo;

         bool bTrimmed = false;
         UCChar *datetext = (UCChar*)data.Str();
         UCChar *end = datetext + data.Length() - 4;
         if(end > datetext &&
            end[0] == L' ' &&
            iswalpha(end[1]) &&
            iswalpha(end[2]) &&
            iswalpha(end[3]))
         {
            *end = 0;
            bTrimmed = false;
         }

         foo.ParseDateTime(datetext,0,MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT));

         if(bTrimmed) *end = L' ';
         
         tm birthtime;
         birthtime.tm_year = foo.GetYear() - 1900;
         birthtime.tm_mon  = foo.GetMonth() - 1;
         birthtime.tm_mday = foo.GetDay();
         birthtime.tm_yday = foo.GetDayOfYear() - 1;
         birthtime.tm_wday = foo.GetDayOfWeek() - 1;
         birthtime.tm_hour = foo.GetHour();
         birthtime.tm_min  = foo.GetMinute();
         birthtime.tm_sec  = foo.GetSecond();

         time_t bar;
         time(&bar);

         tm tm_now;
         localtime_s(&tm_now,&bar);

         int now_seconds = tm_now.tm_sec;
         now_seconds += (tm_now.tm_min * 60);
         now_seconds += (tm_now.tm_hour * 60 * 60);
         now_seconds += (tm_now.tm_yday * 60 * 60 * 24);
         now_seconds += (tm_now.tm_year * 60 * 60 * 24 * 365);

         int birth_seconds = birthtime.tm_sec;
         birth_seconds += (birthtime.tm_min * 60);
         birth_seconds += (birthtime.tm_hour * 60 * 60);
         birth_seconds += (birthtime.tm_yday * 60 * 60 * 24);
         birth_seconds += (birthtime.tm_year * 60 * 60 * 24 * 365);

         int ago_seconds = now_seconds - birth_seconds;
         double diff = (double)ago_seconds;

         data = L"";

         double year_diff = diff / (60.0 * 60.0 * 24.0 * 365.0);
         if(year_diff > 1.0)
         {
            age = (int)year_diff;
         }
      }

      text = L"b[";
      text += m_profile_user;
      text += L"]b\t";

      text += L"l{ShackMessagel{sendmessage:";
      text += m_profile_user;
      text += L"}l}l - ";

      text += L"l{Commentsl{http://www.shacknews.com/search?chatty=1&type=4&chatty_term=&chatty_user=";
      text += m_profile_user;
      text += L"&chatty_author=&chatty_filter=all&result_sort=postdate_desc}l}l - ";

      text += L"l{LOL'dl{LOLTHEYWROTE";
      text += m_profile_user;
      text += L"}l}l ";

      if(m_profile_user == theApp.GetUsername())
      {
         text += L" - l{Edit Profilel{http://chattyprofil.es/myprofile}l}l ";
      }

      titleblock_group->DecodeShackTagsString(text, true, true);

      text = L"b[Chatty Profile]b s[(autogenerated)]s\n\n";

      text += L"f{Registered}f\t";
      text += reg_date;
      text += L"\n";

      text += L"f{First Post}f\t";
      FindValue(data, i, L"firstpost_date");
      text += data;
      text += L"\n";

      text += L"f{Total Posts}f\t";
      FindValue(data, i, L"postcount");
      int postcount = data;
      text += data;
      text += L"\n";

      text += L"f{Last Post}f\t";
      FindValue(data, i, L"mostrecentpost_date");
      text += data;

      if(age != 0)
      {
         text += L"\n";

         text += L"f{Posts per day}f\t";
         //FindValue(data, i, L"postsperday");
         data = ((float)postcount / (float)reg_days);
         text += data;
         /*      
         text += L" (";
         FindValue(data, i, L"class");
         text += data;
         text += L")\n";
         */
      }
      
      profile_group->DecodeShackTagsString(text, true, true);

      text = L"b[Bio Data]b\n\n";

      text += L"f{Age}f\t";
      if(age != 0)
         text += age;
      text += L"\n";

      text += L"f{Sex}f\t";
      FindValue(data, i, L"sex");
      if(data == L"M")
      {
         text += L"Male";
      }
      else if(data == L"F")
      {
         text += L"Female";
      }
      else if(data == L"Y")
      {
         text += L"Yes, Please";
      }
      else
      {
         text += data;
      }
      text += L"\n";

      text += L"f{Location}f\t";
      FindValue(data, i, L"location");
      text += data;
      text += L"\n";

      text += L"f{Homepage}f\t";
      FindValue(data, i, L"homepage");
      if(!data.IsEmpty())
      {
         if(data == L"none")
         {
            text += data;
         }
         else
         {
            text += L"l{";
            text += data;
            text += L"}l";
         }
      }

      biodata_group->DecodeShackTagsString(text, true, true);

      text = L"b[Contact Info]b\n\n";

      text += L"f{AIM}f\t";
      FindValue(data, i, L"aim");
      text += data;
      text += L"\n";

      text += L"f{Yahoo!}f\t";
      FindValue(data, i, L"yahoo");
      text += data;
      text += L"\n";

      text += L"f{MSN}f\t";
      FindValue(data, i, L"msn");
      text += data;
      text += L"\n";

      text += L"f{GTalk}f\t";
      FindValue(data, i, L"gtalk");
      text += data;

      contactinfo_group->DecodeShackTagsString(text, true, true);

      text = L"b[Gaming Handles]b\n\n";

      text += L"f{Steam}f\t";
      FindValue(data, i, L"steam");
      text += data;
      text += L"\n";

      text += L"f{XBox Live}f\t";
      FindValue(data, i, L"xboxlive");
      text += data;
      text += L"\n";

      text += L"f{psn}f\t";
      FindValue(data, i, L"playstation_network");
      text += data;
      text += L"\n";

      text += L"f{Wii}f\t";
      FindValue(data, i, L"wii");
      text += data;
      text += L"\n";

      text += L"f{XFire}f\t";
      FindValue(data, i, L"xfire");
      text += data;

      gaminghandles_group->DecodeShackTagsString(text, true, true);

      text = L"b[User Bio]b\n\n";

      FindValue(data, i, L"user_bio");

      FindLinksInStringAndTagThem(data);

      text += data;
      userbio_group->DecodeShackTagsString(text, true, true);

      userbio_group->InitImageLinks();
   }

}

void CLampDoc::CalcWidthOfAverageProfileGroup()
{
   UCChar sampletext[2];
   sampletext[0] = L'0';
   sampletext[1] = 0;

   int widths[2];

   GetCharWidths(sampletext, widths, 1, false, false, false, theApp.GetNormalFontName());

   m_widthofaverageprofilegroup = 40 + (widths[0] * 36);
}