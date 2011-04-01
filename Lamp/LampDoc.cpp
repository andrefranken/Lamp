
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
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD g_LastPostTime = 0;

UINT DownloadThreadProc( LPVOID pParam )
{
   CDownloadData* pDD = (CDownloadData*)pParam;

   if(pDD != NULL &&
      pDD->m_WhoWants != NULL)
   {
      if((pDD->m_dt == DT_POST ||
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

      return 0;
   }

   return 1;// if pObject is not valid
}


void GetCharWidths(const UCChar *text, int *widths, size_t numchars, bool italic, bool bold, bool sample, const UCChar *fontname)
{
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
   oldfont = (HFONT)::SelectObject(hTempDC,hCreatedFont);
   
   ::GetCharacterPlacementW(hTempDC,text,numchars,0,&results,GCP_USEKERNING);

   ::SelectObject(hTempDC,oldfont);

   if(hTempDC != NULL)
   {
      ::DeleteDC(hTempDC);
   }

   if(hCreatedFont != NULL)
   {
      ::DeleteObject(hCreatedFont);
   }
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

   const char *enc_username = m_username.str8(false,CET_UTF8);
   const char *enc_password = m_password.str8(false,CET_UTF8);

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
      dt != DT_READMSG &&
      (dt != DT_SHACKMSG ||
       dt == DT_SHACKMSG &&
       ((CMainFrame*)theApp.GetMainWnd())->GetActiveLampView() == m_pView))
   {
      m_bBusy = true;
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

   if(dt == DT_POST)
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
      if(pDD->m_data != NULL)
      {
         bool bDoingNewFlags = true;
         switch(pDD->m_dt)
         {
         case DT_THREAD_START:
            {
               ChattyPost *post = FindRootPost(pDD->m_id);
               if(post == NULL)
               {
                  post = new ChattyPost();
                  post->SetId(pDD->m_id);
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

               CXMLTree xmldata;
               if(GetXMLDataFromString(xmldata, (const char *)pDD->m_data, pDD->m_datasize))
               {
                  ReadFromRoot(xmldata, existing_threads);
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

               CXMLTree xmldata;
               if(GetXMLDataFromString(xmldata, (const char *)pDD->m_data, pDD->m_datasize))
               {
                  int page = m_page;
                  ReadFromRoot(xmldata, existing_threads);
                  m_page = page;
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
         case DT_SEARCH:
            {
               CXMLTree xmldata;
               if(GetXMLDataFromString(xmldata, (const char *)pDD->m_data, pDD->m_datasize))
               {
                  ReadSearchResultsFromRoot(xmldata);
               }
            }
            break;
         case DT_LOL:
            {
               ProcessLOLData((char *)pDD->m_data, pDD->m_datasize);
            }
            break;
         case DT_AUTHOR:
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
                  bool bCloseAndRefresh = true;
                  const char *result = (const char *)pDD->m_data;
                  if(result != NULL &&
                     *result != 0)
                  {
                     if(_strnicmp(result,"error_login_failed",18) == 0)
                     {
                        m_pView->MessageBox(L"Invalid username or password");
                        bCloseAndRefresh = false;
                     }

                     if(_strnicmp(result,"error_communication_authentication",34) == 0)
                     {
                        m_pView->MessageBox(L"Invalid username or password");
                        bCloseAndRefresh = false;
                     }

                     if(_strnicmp(result,"error_account_banned",20) == 0)
                     {
                        m_pView->MessageBox(L"You are naughty and banned");
                     }

                     if(_strnicmp(result,"error_post_rate_limiter",23) == 0)
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
            }
            break;
         case DT_SHACKMSG:
            {
               CXMLTree xmldata;
               if(GetXMLDataFromString(xmldata, (const char *)pDD->m_data, pDD->m_datasize))
               {
                  ReadShackMessages(xmldata);
                  UpdateUnreadShackMessagesCount();
               }
            }
            break;
         case DT_READMSG:
            {
               // nothing to do
            }
            break;
         case DT_SENDMSG:
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
            break;
         }
         free(pDD->m_data);
      }
      else if(pDD->m_dt == DT_POST)
      {
         // new thread
         if(m_pView != NULL)
         {
            m_pView->CloseReplyDialog();
         }
         Refresh();
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
   m_page = 0;
   m_lastpage = 0;
   m_ThingsILOLD = false;
   m_ThingsIWrote = false;
   m_bScramblePath = false;
   m_bBusy = false;
   m_shackmsgtype = SMT_INBOX;

   m_lol_text = L"lol";
   m_inf_text = L"inf";
   m_unf_text = L"unf";
   m_tag_text = L"tag";
   m_wtf_text = L"wtf";

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
      SetDataType(DDT_STORY);
      m_title = L"latestchatty";
      MySetTitle(m_title);
      ReadLatestChatty();
      result = TRUE;
   }
   
   return result;
}

BOOL CLampDoc::OnOpenDocument( LPCTSTR lpszPathName )
{
   BOOL result = TRUE;

   UCString rootid;

   bool isstory = false;
   bool islol = false;
   bool issearch = false;
   bool isshackmsg = false;

   bool bAllowPreLoadingOfThread = true;

   const UCChar *start = lpszPathName;
   const UCChar *end = lpszPathName + wcslen(lpszPathName);

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

   if(end - start > 14 &&
      _wcsnicmp(start,L"search.x?type=",14) == 0)
   {
      // todo
      // comments&terms=ffxii&cs_user=Modica+Sol+Solis&cs_parentauthor=&s_type=all
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
   else if(end - start > 11 &&
           _wcsnicmp(start,L"chatty?id=",10) == 0)
   {
      const UCChar *work = start + 10;

      while(work < end && 
            iswdigit(*work))
      {
         rootid += *work;
         work++;
      }

      m_initialpostid = rootid;

      bAllowPreLoadingOfThread = false;
   }
   else if(end - start > 12 &&
           _wcsnicmp(start,L"laryn.x?id=",11) == 0)
   {
      //laryn.x?id=24937945#itemanchor_24937945
      const UCChar *work = start + 11;

      while(work < end && 
            iswdigit(*work))
      {
         rootid += *work;
         work++;
      }

      m_initialpostid = rootid;

      bAllowPreLoadingOfThread = false;
   }
   else
   {
      const UCChar *pCmd = NULL;
      if((pCmd = wcsstr(start,L"\\LOL")) != NULL)
      {
         m_page = m_lastpage = 1;
         islol = true;
         pCmd += 4;
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
      }
      else if(wcsstr(start,L"MYCOMMENTS") != NULL)
      {
         m_search_author = theApp.GetUsername();
         m_search_parent_author = L"";
         m_search_terms = L"";

         m_title = L"My Comments";
         MySetTitle(m_title);

         issearch = true;
      }
      else if(wcsstr(start,L"REPLIESTOME") != NULL)
      {
         m_search_author = L"";
         m_search_parent_author = theApp.GetUsername();
         m_search_terms = L"";

         m_title = L"Replies To Me";
         MySetTitle(m_title);

         issearch = true;
      }
      else if(wcsstr(start,L"VANITYSEARCH") != NULL)
      {
         m_search_author = L"";
         m_search_parent_author = L"";
         m_search_terms = theApp.GetUsername();

         m_title = L"Vanity Search";
         MySetTitle(m_title);

         issearch = true;
      }
      else if(wcsstr(start,L"CUSTOMSEARCH") != NULL)
      {
         theApp.GetLastSearchParms(m_search_author, m_search_parent_author, m_search_terms);
         
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
      else if((pCmd = wcsstr(start,L"SHACKMSG_")) != NULL)
      {
         const UCChar *work = pCmd + 9;
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
   }

   if(islol)
   {
      SetDataType(DDT_LOLS);
      ReadLOL();
   }
   else if(issearch)
   {
      SetDataType(DDT_SEARCH);
      PerformSearch();
   }
   else if(isstory)
   {
      SetDataType(DDT_STORY);
      m_title = L"story";
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
               m_rootposts.push_back(post);
               post->ReadPost(pExistingPost,this);
               post->Expand();
               post->UnShowAsTruncated();
               
               ChattyPost *refresh_post = post->FindChild(m_initialpostid);
               if(refresh_post != NULL)
               {
                  refresh_post->SetRefreshing(true);
               }
            }
         }
      }

      RefreshThread(rootid, m_initialpostid,true);
   }
   
   return result;
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
   
   UCString path = L"/search.xml?author=";
   path += m_search_author;
   path += L"&parent_author=";
   path += m_search_parent_author;
   path += L"&terms=";
   path += m_search_terms;

   path += L"&terms=";
   path += m_search_terms;
   
   // these don't work
   //path += L"&SearchType=n";
   //path += L"&SearchType=i";

   StartDownload(theApp.GetHostName(),
                 path,
                 DT_SEARCH,
                 0);

   
}

void CLampDoc::GetShackMessages()
{
   UCString path = L"/Messages/?box=";
   
   switch(m_shackmsgtype)
   {
   case SMT_INBOX:   path += L"inbox"; break;
   case SMT_OUTBOX:  path += L"outbox"; break;
   case SMT_ARCHIVE: path += L"archive"; break;
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

   StartDownload(theApp.GetHostName(),
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

void CLampDoc::MarkShackMessageRead(unsigned int id)
{
   // "http://shackapi.stonedonkey.com/messages/read/?username=" + _login +"&password=" + _password +"&messageid=" + 
   UCString path = L"/Messages/read/?messageid=";
   
   path += id;

   StartDownload(theApp.GetHostName(),
                 path,
                 DT_READMSG,
                 0,
                 0,
                 0,
                 NULL,
                 theApp.GetUsername(),
                 theApp.GetPassword());
}

void CLampDoc::DeleteShackMessage(unsigned int id)
{
   // "http://shackapi.stonedonkey.com/messages/read/?username=" + _login +"&password=" + _password +"&messageid=" + 
   UCString path = L"/Messages/read/?messageid=";
   
   path += id;

   StartDownload(theApp.GetHostName(),
                 path,
                 DT_READMSG,
                 0,
                 0,
                 0,
                 NULL,
                 theApp.GetUsername(),
                 theApp.GetPassword());
}

void CLampDoc::SendMessage(const UCString &to, const UCString &subject, const UCString &shackmsg)
{
   // "http://shackapi.stonedonkey.com/messages/Send/"

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
   char *enc_subject = url_encode((char*)subject.str8(false,CET_UTF8));
   post_data += enc_subject;

   post_data += L"&to=";
   char *enc_to = url_encode((char*)to.str8(false,CET_UTF8));
   post_data += enc_to;
   
   post_data += L"&body=";
   char *enc_shackmsg = url_encode((char*)shackmsg.str8(false,CET_UTF8));
   post_data += enc_shackmsg;
   
   StartDownload(theApp.GetHostName(),
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
                           while(work < endsection)
                           {
                              body += *work;
                              work++;
                           }

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
      m_ThingsIWrote)
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

   StartDownload(theApp.GetHostName(),
                 story,
                 DT_STORY,
                 0);
}

void CLampDoc::ReadLatestChattyPart2()
{
   UCString story = L"/";
   
   size_t actualpage = ((m_page - 1) * 2) + 2;

   story += L"17.";
   story += actualpage;
   story += L".xml";

   StartDownload(theApp.GetHostName(),
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

   UCString path = L"/";
   path += L"thread/";
   path += id;
   path += L".xml";

   DownloadType dt = DT_THREAD;
   if(bStarting)
   {
      dt = DT_THREAD_START;
   }

   StartDownload(theApp.GetHostName(),
                 path,
                 dt,
                 id,
                 refresh_id,
                 reply_to_id);

   return true;
}

void CLampDoc::RefreshAllRoots()
{
   std::list<ChattyPost*>::iterator it = m_rootposts.begin();
   std::list<ChattyPost*>::iterator end = m_rootposts.end();

   while(it != end)
   {
      if(!(*it)->IsCollapsed())
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
   ChattyPost *parent = FindPost(to_id);
   while(parent != NULL && parent->GetParent() != NULL){parent = parent->GetParent();}

   theApp.SetLastPost(replytext);
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
   char *enc_reply = url_encode((char*)replytext.str8(false,CET_UTF8));
   post_data += enc_reply;
   
   StartDownload(theApp.GetHostName(),
                 path,
                 DT_POST,
                 to_id,
                 0,
                 0,
                 post_data,
                 theApp.GetUsername(),
                 theApp.GetPassword());

   free(enc_reply);
      
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
void CLampDoc::Draw(HDC hDC, RECT &DeviceRectangle, int pos, std::vector<CHotSpot> &hotspots, unsigned int current_id)
{
   pos = -pos;

   int startpos = pos;

   switch(m_datatype)
   {
   case DDT_EPICFAILD:
      ::FillRect(hDC,&DeviceRectangle,m_backgroundbrush);
      break;
   case DDT_STORY:
      {
         if(m_rootposts.size() > 0)
         {
            pos = DrawBanner(hDC, DeviceRectangle, pos, hotspots, true, false);

            if(m_pReplyDlg != NULL)
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

            pos = DrawFromRoot(hDC, DeviceRectangle, pos, hotspots, current_id, false);

            pos = DrawBanner(hDC, DeviceRectangle, pos, hotspots, true, false);
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
         
         pos = DrawFromRoot(hDC, DeviceRectangle, pos, hotspots, current_id, false);

         backrect.top = pos;
         backrect.bottom = pos + 20;
         FillBackground(hDC,backrect);
         pos += 20;
      }
      break;
   case DDT_LOLS:
      {
         if(m_rootposts.size() > 0)
         {
            if(m_lastpage > 1)
            {
               pos = DrawBanner(hDC, DeviceRectangle, pos, hotspots, false, false);
            }
            else
            {
               RECT backrect = DeviceRectangle;
               backrect.top = pos;
               backrect.bottom = pos + 20;
               FillBackground(hDC,backrect);
               pos += 20;
            }
            
            pos = DrawFromRoot(hDC, DeviceRectangle, pos, hotspots, current_id, true);

            if(m_lastpage > 1)
            {
               pos = DrawBanner(hDC, DeviceRectangle, pos, hotspots, false, false);
            }
            else
            {
               RECT backrect = DeviceRectangle;
               backrect.top = pos;
               backrect.bottom = pos + 20;
               FillBackground(hDC,backrect);
               pos += 20;
            }
         }
      }
      break;
   case DDT_SEARCH:
      {
         RECT backrect = DeviceRectangle;
         backrect.top = pos;
         backrect.bottom = pos + 20;
         FillBackground(hDC,backrect);
         pos += 20;
         
         pos = DrawFromRoot(hDC, DeviceRectangle, pos, hotspots, current_id, true);

         backrect.top = pos;
         backrect.bottom = pos + 20;
         FillBackground(hDC,backrect);
         pos += 20;
      }
      break;
   case DDT_SHACKMSG:
      {
         if(m_rootposts.size() > 0)
         {
            pos = DrawBanner(hDC, DeviceRectangle, pos, hotspots, false, true);
            pos = DrawMessages(hDC, DeviceRectangle, pos, hotspots);
            pos = DrawBanner(hDC, DeviceRectangle, pos, hotspots, false, true);
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
            ::SelectObject(hDC,m_pagefont);
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
                  
                  hotspot.m_type = HST_PAGE;
                  hotspot.m_spot = pagebut;
                  hotspot.m_id = i;
                  hotspots.push_back(hotspot);
               }
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
   }

   return bannerrect.bottom;
}

int CLampDoc::DrawFromRoot(HDC hDC, RECT &DeviceRectangle, int pos, std::vector<CHotSpot> &hotspots, unsigned int current_id, bool bLinkOnly)
{
   std::list<ChattyPost*>::iterator begin = m_rootposts.begin();
   std::list<ChattyPost*>::iterator end = m_rootposts.end();
   std::list<ChattyPost*>::iterator it = begin;

   while(it != end)
   {
      pos = (*it)->DrawRoot(hDC,DeviceRectangle,pos,hotspots, current_id, bLinkOnly);
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


int CLampDoc::DrawMessages(HDC hDC, RECT &DeviceRectangle, int pos, std::vector<CHotSpot> &hotspots)
{
   std::list<ChattyPost*>::iterator begin = m_rootposts.begin();
   std::list<ChattyPost*>::iterator end = m_rootposts.end();
   std::list<ChattyPost*>::iterator it = begin;

   while(it != end)
   {
      pos = (*it)->DrawMessage(hDC,DeviceRectangle,pos,hotspots);
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
                  existing_threads.push_back(root_id);
                  m_rootposts.push_back(post);
                  post->Read(pChild, this, false);
                  post->SetupPreviewShades(false);
                  post->CountFamilySize();
                  post->UpdateRootReplyList();
                  post->SetParent(NULL);
                  if(post->IsFiltered())
                  {
                     m_rootposts.pop_back();
                     existing_threads.pop_back();
                     delete post;
                  }
               }
            }
         }
      }
   }
   
   return result;
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
                            const int *widths, 
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
   for(unsigned int tag = 0; tag < shacktags.size(); tag++)
   {
      if(image_begin == -1 && 
         shacktags[tag].m_tag == ST_IMAGE)
      {
         image_tag = (int)tag;
         image_begin = shacktags[tag].m_pos;
      }

      if(image_begin != -1 &&
         shacktags[tag].m_tag == ST_IMAGE_END)
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
            linetypes.push_back(linetype(true,0,theApp.GetTextHeight()));
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

         CDCSurface *pImage = theApp.GetLinkedImage(shacktags[image_tag].m_image_index);
         if(pImage != NULL)
         {
            height = pImage->GetHeightForWidth(textwidth);
         }

         linetypes.push_back(linetype(false,shacktags[image_tag].m_image_index,height));
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
         for(; tag < shacktags.size(); tag++)
         {
            if(image_begin == -1 && 
               shacktags[tag].m_tag == ST_IMAGE)
            {
               image_tag = (int)tag;
               image_begin = shacktags[tag].m_pos;
            }

            if(image_begin != -1 &&
               shacktags[tag].m_tag == ST_IMAGE_END)
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
         linetypes.push_back(linetype(true,0,theApp.GetTextHeight()));
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
         linetypes.push_back(linetype(true,0,theApp.GetTextHeight()));
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

   // insert the remainder
   lines_of_text.push_back(thistext);
   charsizes.push_back(thiswidths);
   linesizes.push_back(i - thisi);
   linetypes.push_back(linetype(true,0,theApp.GetTextHeight()));
   rectheight += theApp.GetTextHeight();
   // setup tags for this finished line
   std::vector<shacktagpos> thislinetags;
   CalcLineTags(shacktags,thislinetags, thistext - text, i);
   linetags.push_back(thislinetags);
   
   rect.bottom = rectheight;
}

void CLampDoc::FillExpandedBackground(HDC hDC, RECT &rect, bool bAsRoot, postcategorytype posttype, bool bStrokeTopOnly)
{
   RECT temprect = rect;
   temprect.bottom = temprect.top + 20;
   temprect.right = temprect.left + 20;
   ::FillRect(hDC,&temprect,m_backgroundbrush);

   temprect.right = rect.right;
   temprect.left = rect.right - 20;
   ::FillRect(hDC,&temprect,m_backgroundbrush);

   temprect = rect;
   temprect.top = temprect.bottom - 20;
   temprect.right = temprect.left + 20;
   ::FillRect(hDC,&temprect,m_backgroundbrush);

   temprect.right = rect.right;
   temprect.left = rect.right - 20;
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
      case PCT_POLITCIAL:  ::SelectObject(hDC,m_politicalpen); break;
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
         case PCT_POLITCIAL:  ::SelectObject(hDC,m_politicalpen); break;
         }
         ::MoveToEx(hDC,temprect.left, temprect.top,NULL);
         ::LineTo(hDC,temprect.right - 1, temprect.top);
      }
   }

   ::SelectObject(hDC,oldbrush);
}

void CLampDoc::DrawLOLField(HDC hDC, loltagtype type, RECT &rect, UCString &lols, bool bHover, bool bVoted, bool bRoot)
{
   HFONT oldfont = NULL;

   if(lols.Length() > 0 || bHover)
   {
      if(theApp.ShowSmallLOL())
      {
         oldfont = (HFONT)::SelectObject(hDC,m_miscboldfont);
      }
      else
      {
         oldfont = (HFONT)::SelectObject(hDC,m_boldfont);
      }
   }
   else
   {
      if(theApp.ShowSmallLOL())
      {
         oldfont = (HFONT)::SelectObject(hDC,m_miscfont);
      }
      else
      {
         oldfont = (HFONT)::SelectObject(hDC,m_normalfont);
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
   
   ::SelectObject(hDC,oldfont);
}

void CLampDoc::DrawPreviewAuthor(HDC hDC, RECT &rect, UCString &text, bool clipped, int shade, COLORREF AuthorColor, const UCString &rootauthor)
{
   HFONT oldfont = (HFONT)::SelectObject(hDC,m_normalfont);

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

   ::SelectObject(hDC,oldfont);
}

void CLampDoc::MyTextOut(HDC hDC, int x, int y, const UCChar *text, UINT count, const INT *widths, const RECT *pClipRect)
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
         ::ExtTextOutW(hDC, x, y, fupotions, pClipRect, start, thiscount, w);
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
      ::ExtTextOutW(hDC, x, y, fupotions, pClipRect, start, end - start, w);
   }
}

void CLampDoc::DrawPreviewText(HDC hDC,
                               RECT &rect,
                               UCString &text,
                               int *charwidths,
                               std::vector<shacktagpos> &shacktags,
                               int shade,
                               bool &clipped)
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
   COLORREF color;
   COLORREF normalcolor = theApp.GetPostTextColorShade(shade);

   bool colorchange = false;
   bool stylechange = false;
   HFONT hCreatedFont = NULL;
   HFONT hPreviousFont = NULL;

   // setup initial font stuff
   ::SetTextColor(hDC,normalcolor);
   hPreviousFont = (HFONT)::SelectObject(hDC, m_normalfont);

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
         MyTextOut(hDC, rect.left, rect.bottom, text, numchars, charwidths, NULL);
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
            MyTextOut(hDC, x, rect.bottom, text, charsthisrun, charwidths, NULL);
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

         case ST_QUOTE: quote = true;stylechange = true;break;
         case ST_SAMPLE: sample = true;stylechange = true;break;
         case ST_STRIKE: strike = true;stylechange = true;break;
         case ST_ITALIC: italic = true;stylechange = true;break;
         case ST_BOLD: bold = true;stylechange = true;break;
         case ST_UNDERLINE: underline = true;stylechange = true;break;
         case ST_SPOILER: spoiler = true;stylechange = true;break;
         case ST_CODE: code = true;stylechange = true;break;
         case ST_LINK: link = true;stylechange = true; color = normalcolor; colorstack.push_back(color); colorchange = true; break;

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
               ::SelectObject(hDC, hPreviousFont);
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
            hPreviousFont = (HFONT)::SelectObject(hDC, hCreatedFont);
            stylechange = false;
         }

         int start = (*it).m_pos;
         it++;
         int finish = numchars;
         if(it != end)
         {
            finish = __min((*it).m_pos,numchars);
         }

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
            MyTextOut(hDC, x, rect.bottom, text.Str() + start, finish - start, charwidths + start, NULL);
            //::ExtTextOutW(hDC, x, rect.bottom, 0, NULL, text.Str() + start, finish - start, charwidths + start);
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
      ::SelectObject(hDC, hPreviousFont);
   }
   if(hCreatedFont != NULL)
   {
      ::DeleteObject(hCreatedFont);
   }
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
                            const RECT *pClipRect/*=NULL*/)
{
   int y = rect.top + 4 + theApp.GetTextHeight();

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
   COLORREF color;
   COLORREF normalcolor = theApp.GetPostTextColor();
   bool colorchange = false;
   bool stylechange = false;
   HFONT hCreatedFont = NULL;
   HFONT hPreviousFont = NULL;

   // setup initial font stuff
   ::SetTextColor(hDC,normalcolor);
   hPreviousFont = (HFONT)::SelectObject(hDC, m_normalfont);

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
               if(link)
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
                  links.push_back(linkrect);
               }
               MyTextOut(hDC, x, y, pLineText, numchars, pLineWidths,pClipRect);
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
                  if(link)
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
                     links.push_back(linkrect);
                  }
                  MyTextOut(hDC, x, y, pLineText, (*it).m_pos, pLineWidths,pClipRect);
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
               case ST_LINK: link = true;stylechange = true; color = RGB(174,174,155); colorstack.push_back(color); colorchange = true; break;

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
                     ::SelectObject(hDC, hPreviousFont);
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
                  hCreatedFont = ::CreateFontW(fsize,0,0,0,weight,italic,underline|link,strike,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_DONTCARE,font);
                  hPreviousFont = (HFONT)::SelectObject(hDC, hCreatedFont);
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
                  if(link)
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
                     links.push_back(linkrect);
                  }
                  MyTextOut(hDC, x, y, pLineText + start, finish - start, pLineWidths + start,pClipRect);
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

            imagelinks.push_back(output);
         }
      }
            
      y += linetypes[i].m_height;
   }

   if(hPreviousFont != NULL)
   {
      ::SelectObject(hDC, hPreviousFont);
   }
   if(hCreatedFont != NULL)
   {
      ::DeleteObject(hCreatedFont);
   }

   y += 4;
}

void CLampDoc::DrawRootAuthor(HDC hDC, RECT &rect,UCString &author, COLORREF AuthorColor, bool bFade/*= false*/, bool m_bIsInbox/*=true*/)
{
   HFONT oldfont = (HFONT)::SelectObject(hDC,m_miscfont);
   ::SetTextColor(hDC,theApp.GetMiscPostTextColor());

   if(m_bIsInbox)
   {
      ::ExtTextOutW(hDC, rect.left + 5, rect.bottom, 0, NULL, L"By:", 3, NULL);
   }
   else
   {
      ::ExtTextOutW(hDC, rect.left + 5, rect.bottom, 0, NULL, L"To:", 3, NULL);
   }

   ::SelectObject(hDC,m_boldfont);

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

   ::SelectObject(hDC,oldfont);
}

void CLampDoc::DrawDate(HDC hDC, RECT &rect, UCString &date, COLORREF ago_color, bool bGetExtents/*=false*/)
{
   HFONT oldfont = (HFONT)::SelectObject(hDC,m_miscfont);
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
   ::SelectObject(hDC,oldfont);
}

void CLampDoc::DrawCollapseNote(HDC hDC, RECT &rect)
{
   HFONT oldfont = (HFONT)::SelectObject(hDC,m_miscfont);
   ::SetTextColor(hDC,theApp.GetMiscPostTextColor());

   ::ExtTextOutW(hDC, rect.left + 5, rect.bottom, 0, NULL, L"( Collapsed... )", 16, NULL);

   ::SelectObject(hDC,oldfont);
}

void CLampDoc::DrawRepliesHint(HDC hDC, RECT &rect, int m_reportedchildcount)
{
   HFONT oldfont = (HFONT)::SelectObject(hDC,m_miscfont);
   ::SetTextColor(hDC,theApp.GetMiscPostTextColor());

   ::SetTextAlign(hDC,TA_LEFT|TA_BOTTOM|TA_UPDATECP);
   ::MoveToEx(hDC, rect.left + 5, rect.bottom - 5, NULL); 
   ::ExtTextOutW(hDC, rect.left + 5, rect.bottom - 5, 0, NULL, L"Click to see all ", 17, NULL);
   UCString text = m_reportedchildcount;
   ::SetTextColor(hDC,theApp.GetMyPostColor());
   ::ExtTextOutW(hDC, rect.left + 5 + (theApp.GetMiscFontHeight() * -7), rect.bottom - 5, 0, NULL, text, text.Length(), NULL);
   ::SetTextColor(hDC,theApp.GetMiscPostTextColor());
   ::ExtTextOutW(hDC, rect.left + 5 + (theApp.GetMiscFontHeight() * -7) + ((theApp.GetFontHeight() / -2) * text.Length()), rect.bottom - 5, 0, NULL, L" replies", 8, NULL);

   ::SetTextAlign(hDC,TA_LEFT|TA_BOTTOM);

   ::SelectObject(hDC,oldfont);
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

         if(shade > 2)
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

         if(shade > 2)
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

      chattyerror err = webclient_download(theApp.GetLolHostName().str8(), path.str8(false,CET_UTF8), NULL, NULL, &data);

      if(err == ERR_OK && data != NULL)
      {
         if(strstr(data, "ok") != NULL)
         {
            result = true;
         }
      }
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
      m_shackmsgtype == SMT_INBOX)
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

      if(count > 0)
      {
         m_title = L"Inbox (";
         m_title += count;
         m_title += L")";
      }
      else
      {
         m_title = L"Inbox";
      }
      MySetTitle(m_title);
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