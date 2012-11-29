
// Lamp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "Lamp.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "LampDoc.h"
#include "LampView.h"
#include "OpenThreadDlg.h"
#include "LoginDlg.h"
#include "webclient.h"
#include "DockTab.h"
#include "html.h"
#include "hunspell.hxx"
#include "CustomSearchDlg.h"
#include "comm.h"

#include <io.h>

#define NDEBUG

#include "JSONOptions.h"
#include "libjson.h"
#include "JSONNode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UCString g_PathToMe;

bool g_bIsXP = false;

bool g_bSingleThreadStyle = false;

#include <gdiplus.h>
using namespace Gdiplus;
GdiplusStartupInput g_gdiplusStartupInput;
ULONG_PTR g_gdiplusToken;

chattyerror download(const char* host, const char* path, char** out_response, int *psize/*=NULL*/)
{
   chattyerror err = ERR_NOT_IMPLEMENTED;

   int numtries = 3;

   while(numtries > 0)
   {
      DWORD starttime = ::GetTickCount();
      err = webclient_download(host, path, NULL, NULL, out_response,psize);
      DWORD endtime = ::GetTickCount();

      if(*out_response == NULL && 
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

   return err;
}

// CLampApp

BEGIN_MESSAGE_MAP(CLampApp, CWinAppEx)
   ON_COMMAND(ID_FILE_NEW, OnFileNew) 
   ON_COMMAND(ID_FILE_OPEN, OnFileOpen) 
	ON_COMMAND(ID_APP_ABOUT, &CLampApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
   ON_COMMAND(ID_FILE_OPENTHREAD, OnFileOpenthread)
   ON_UPDATE_COMMAND_UI(ID_FILE_OPENTHREAD, &OnUpdateFileOpenthread)
   ON_COMMAND(ID_FILE_SETUPLOGININFO, &CLampApp::OnFileSetuplogininfo)
   ON_UPDATE_COMMAND_UI(ID_FILE_SETUPLOGININFO, &CLampApp::OnUpdateFileSetuplogininfo)
   ON_COMMAND(ID_VIEW_SMOOTHSCROLL, &CLampApp::OnViewSmoothscroll)
   ON_UPDATE_COMMAND_UI(ID_VIEW_SMOOTHSCROLL, &CLampApp::OnUpdateViewSmoothscroll)
   ON_COMMAND(ID_VIEW_DOCKTOP, &CLampApp::OnViewDocktop)
   ON_UPDATE_COMMAND_UI(ID_VIEW_DOCKTOP, &CLampApp::OnUpdateViewDocktop)
   ON_COMMAND(ID_ALWAYSONTOP_WHENNOTDOCKED, &CLampApp::OnAlwaysOnTopWhenNotDocked)
   ON_UPDATE_COMMAND_UI(ID_ALWAYSONTOP_WHENNOTDOCKED, &CLampApp::OnUpdateAlwaysOnTopWhenNotDocked)
   ON_COMMAND(ID_LOL_LOL, &CLampApp::OnLOLLOL)
   ON_UPDATE_COMMAND_UI(ID_LOL_LOL, &CLampApp::OnUpdateLOLLOL)
   ON_COMMAND(ID_LOL_INF, &CLampApp::OnLOLINF)
   ON_UPDATE_COMMAND_UI(ID_LOL_INF, &CLampApp::OnUpdateLOLINF)
   ON_COMMAND(ID_LOL_UNF, &CLampApp::OnLOLUNF)
   ON_UPDATE_COMMAND_UI(ID_LOL_UNF, &CLampApp::OnUpdateLOLUNF)
   ON_COMMAND(ID_LOL_TAG, &CLampApp::OnLOLTAG)
   ON_UPDATE_COMMAND_UI(ID_LOL_TAG, &CLampApp::OnUpdateLOLTAG)
   ON_COMMAND(ID_LOL_WTF, &CLampApp::OnLOLWTF)
   ON_UPDATE_COMMAND_UI(ID_LOL_WTF, &CLampApp::OnUpdateLOLWTF)
   ON_COMMAND(ID_LOL_THINGS_I_LOLD, &CLampApp::OnThingsILold)
   ON_UPDATE_COMMAND_UI(ID_LOL_THINGS_I_LOLD, &CLampApp::OnUpdateThingsILold)
   ON_COMMAND(ID_LOL_THINGS_I_TAGD, &CLampApp::OnThingsITagd)
   ON_UPDATE_COMMAND_UI(ID_LOL_THINGS_I_TAGD, &CLampApp::OnUpdateThingsITagd)
   ON_COMMAND(ID_LOL_THINGS_I_WROTE, &CLampApp::OnThingsIWrote)
   ON_UPDATE_COMMAND_UI(ID_LOL_THINGS_I_WROTE, &CLampApp::OnUpdateThingsIWrote)
   ON_COMMAND(ID_LOL_GOTO_LOL, &CLampApp::OnGotoLol)
   ON_UPDATE_COMMAND_UI(ID_LOL_GOTO_LOL, &CLampApp::OnUpdateGotoLol)
   ON_COMMAND(ID_MY_COMMENTS, &CLampApp::OnMyComments)
   ON_UPDATE_COMMAND_UI(ID_MY_COMMENTS, &CLampApp::OnUpdateMyComments)
   ON_COMMAND(ID_VANITY_SEARCH, &CLampApp::OnVanitySearch)
   ON_UPDATE_COMMAND_UI(ID_VANITY_SEARCH, &CLampApp::OnUpdateVanitySearch)
   ON_COMMAND(ID_SHACK_SEARCH, &CLampApp::OnShackSearch)
   ON_UPDATE_COMMAND_UI(ID_SHACK_SEARCH, &CLampApp::OnUpdateShackSearch)
   ON_COMMAND(ID_REPLIES_TO_ME, &CLampApp::OnRepliesToMe)
   ON_UPDATE_COMMAND_UI(ID_REPLIES_TO_ME, &CLampApp::OnUpdateRepliesToMe)
   ON_COMMAND(ID_VIEW_SHOWIMAGETHUMBS, &CLampApp::OnViewShowThumbs)
   ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWIMAGETHUMBS, &CLampApp::OnUpdateViewShowThumbs)
   ON_COMMAND(ID_SHKMSGS_INBOX, &CLampApp::OnShackMsg_Inbox)
   ON_UPDATE_COMMAND_UI(ID_SHKMSGS_INBOX, &CLampApp::OnUpdateShackMsg_Inbox)
   ON_COMMAND(ID_SHKMSGS_OUTBOX, &CLampApp::OnShackMsg_Outbox)
   ON_UPDATE_COMMAND_UI(ID_SHKMSGS_OUTBOX, &CLampApp::OnUpdateShackMsg_Outbox)
   ON_COMMAND(ID_SHKMSGS_ARCHIVE, &CLampApp::OnShackMsg_Archive)
   ON_UPDATE_COMMAND_UI(ID_SHKMSGS_ARCHIVE, &CLampApp::OnUpdateShackMsg_Archive)
   ON_COMMAND(ID_AUTOLOADCHATTYPICSTHUMBS, &CLampApp::OnAutoLoadChattypicsThumbs)
   ON_UPDATE_COMMAND_UI(ID_AUTOLOADCHATTYPICSTHUMBS, &CLampApp::OnUpdateAutoLoadChattypicsThumbs)
   ON_COMMAND(ID_DONTAUTOLOADNWSTHUMBS, &CLampApp::OnDontAutoLoadNWSThumbs)
   ON_UPDATE_COMMAND_UI(ID_DONTAUTOLOADNWSTHUMBS, &CLampApp::OnUpdateDontAutoLoadNWSThumbs)
   //ON_MESSAGE(WM_THREAD_DOWNLOAD, &CLampApp::OnThreadDownload)

END_MESSAGE_MAP()

BOOL CLampApp::PreTranslateMessage(MSG* pMsg)
{
   /*
   if(m_password.IsEmpty() == false &&
      m_password != L"password")
   {
      int g = 5;
   }
   */

   // Is it the Message you want?
   // You can use a switch statement but because this is
   // only looking for one message, you can use the if/else
   if (pMsg->message == WM_THREAD_DOWNLOAD)
   {
      // handle it
      CDownloadData *pDD = (CDownloadData*)pMsg->lParam;
      if(pDD != NULL &&
         pDD->m_WhoWants != NULL)
      {
         if(pDD->m_WhoWants == this)
         {
            if(pDD->m_dt == DT_CHECK_UPDATE)
            {
               if(pDD->m_data != NULL &&
                  pDD->m_datasize > 0)
               {
                  char *pText = (char *)pDD->m_data;

                  CXMLTree xmldata;
                  if(GetXMLDataFromString(xmldata, (const char *)pDD->m_data, pDD->m_datasize))
                  {
                     int majorversion = xmldata.GetElementValue(L"MajorVersion");

                     int minorversion = xmldata.GetElementValue(L"MinorVersion");

                     if(majorversion > LAMP_VERSION_MAJOR ||
                       (majorversion == LAMP_VERSION_MAJOR &&
                        minorversion > LAMP_VERSION_MINOR))
                     {
                        UCString msg(L"There is a newer version (");
                        msg += majorversion;
                        msg += L'.';
                        msg += minorversion;
                        msg += L") available.\r\nWould you like to update?";
                        int ret = GetMainWnd()->MessageBox(msg,L"Lamp",MB_YESNO);
                        if(ret == IDYES)
                        {
                           //UCString link(L"http://shackwiki.com/wiki/Lamp#-_Download");
                           //OpenShackLink(link);
                           UCString update_filename;
                           update_filename.PathToMe(L"LampUpdate.exe");

                           ShellExecuteW(NULL,L"open",update_filename, NULL, NULL, SW_SHOW);
                        }
                     }                     
                     else if(pDD->m_postrootid == 1)
                     {
                        GetMainWnd()->MessageBox(L"Your version is up-to-date.",L"Lamp",MB_OK);
                     }

                     int minutes = xmldata.GetElementValue(L"CheckLOLMinutes");

                     if(minutes != 0 &&
                        theApp.GetNumMinutesCheckLOL() != minutes)
                     {
                        theApp.SetNumMinutesCheckLOL(minutes);
                        ((CMainFrame*)GetMainWnd())->UpdateLOLTimer();
                     }

                     UCString lolserver = xmldata.GetElementValue(L"ForceLOLServer");

                     if(!lolserver.IsEmpty())
                     {
                        m_lolhostname = lolserver;
                     }

                     CXMLElement *pMods = xmldata.FindChildElement(L"Mods");
                     CXMLElement *pEmps = xmldata.FindChildElement(L"ShackEmployees");
                     CXMLElement *pDevs = xmldata.FindChildElement(L"GameDevs");
                     if(pMods != NULL &&
                        pEmps != NULL &&
                        pDevs != NULL)
                     {
                        m_namelist.clear();
                        m_mod_list.clear();
                     
                        // the update doesn't have colors, so preserve them
                        UCString existingcolor = m_GameDevs.GetAttributeValue(L"color");
                        UCString existingenable = m_GameDevs.GetAttributeValue(L"enable");
                        m_GameDevs = *pDevs;
                        m_GameDevs.AddAttribute(L"color",existingcolor);
                        m_GameDevs.AddAttribute(L"enable",existingenable);

                        COLORREF color = m_GameDevs.GetAttributeValue(L"color");
                        bool enabled = m_GameDevs.GetAttributeValue(L"enable");
                        if(enabled)
                        {
                           int count = pDevs->CountChildren();
                           for(int i = 0; i < count; i++)
                           {
                              CXMLElement *name = pDevs->GetChildElement(i);
                              if(name != NULL && name->GetTag() == L"name")
                              {
                                 UCString temp = name->GetValue();
                                 temp.MakeLower();
                                 m_namelist[temp] = color;
                              }
                           }
                        }

                        // the update doesn't have colors, so preserve them
                        existingcolor = m_Mods.GetAttributeValue(L"color");
                        existingenable = m_Mods.GetAttributeValue(L"enable");
                        m_Mods = *pMods;
                        m_Mods.AddAttribute(L"color",existingcolor);
                        m_Mods.AddAttribute(L"enable",existingenable);

                        color = m_Mods.GetAttributeValue(L"color");
                        enabled = m_Mods.GetAttributeValue(L"enable");
                        if(enabled)
                        {
                           int count = pMods->CountChildren();
                           for(int i = 0; i < count; i++)
                           {
                              CXMLElement *name = pMods->GetChildElement(i);
                              if(name != NULL && name->GetTag() == L"name")
                              {
                                 UCString temp = name->GetValue();
                                 temp.MakeLower();
                                 m_namelist[temp] = color;

                                 temp = name->GetValue();
                                 temp.TrimWhitespace();
                                 temp.MakeLower();
                                 m_mod_list.insert(temp);
                              }
                           }
                        }

                        // the update doesn't have colors, so preserve them
                        existingcolor = m_ShackEmployees.GetAttributeValue(L"color");
                        existingenable = m_ShackEmployees.GetAttributeValue(L"enable");
                        m_ShackEmployees = *pEmps;
                        m_ShackEmployees.AddAttribute(L"color",existingcolor);
                        m_ShackEmployees.AddAttribute(L"enable",existingenable);

                        color = m_ShackEmployees.GetAttributeValue(L"color");
                        enabled = m_ShackEmployees.GetAttributeValue(L"enable");
                        if(enabled)
                        {
                           int count = pEmps->CountChildren();
                           for(int i = 0; i < count; i++)
                           {
                              CXMLElement *name = pEmps->GetChildElement(i);
                              if(name != NULL && name->GetTag() == L"name")
                              {
                                 UCString temp = name->GetValue();
                                 temp.MakeLower();
                                 m_namelist[temp] = color;
                              }
                           }
                        }
                     }    
                     CheckForModMode();
                  }
               }
            }
            else if(pDD->m_dt == DT_REFRESH_LOLS)
            {
               if(pDD->m_data != NULL &&
                  pDD->m_datasize > 0)
               {
                  char *pText = (char *)pDD->m_data;

                  UCString temp;
                  temp.AppendEncodedString(pText,pDD->m_datasize);

                  json_string jsondata = temp;

                  /*
                  // to test parsing of non-json data, enable this
                  jsondata = L"<b>Warning</b>:  mysql_connect() [<a href='function.mysql-connect'>function.mysql-connect</a>]: User thomw1_scf already has more than 'max_user_connections' active connections in <b>/home/thomw1/public_html/greasemonkey/shacklol/db.mysql.php</b> on line <b>72</b><br />";
                  */
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
                  while (i != n.end())
                  {
                     json_string root_name = (*i).name();

                     JSONNode::json_iterator j = (*i).begin();
                     while (j != (*i).end())
                     {
                        json_string str_id = (*j).name();

                        JSONNode::json_iterator k = (*j).begin();
                        while (k != (*j).end())
                        {
                           json_string str_category = (*k).name();

                           json_string str_count = (*k).as_string();

                           UCString thread_id = (const wchar_t*)str_id.data();
                           UCString category = (const wchar_t*)str_category.data();
                           UCString uccount = (const wchar_t*)str_count.data();

                           unsigned int id = thread_id;
                           unsigned int count = uccount;

                           if(category == L"lol")      SetLOL_LOL(id, count);
                           else if(category == L"inf") SetLOL_INF(id, count);
                           else if(category == L"unf") SetLOL_UNF(id, count);
                           else if(category == L"tag") SetLOL_TAG(id, count);
                           else if(category == L"wtf") SetLOL_WTF(id, count);
                           else if(category == L"ugh") SetLOL_UGH(id, count);

                           k++;
                        }

                        j++;
                     }

                     i++;
                  }

                                    
                  // have all the tabs update their lol count info
                  std::list<CLampDoc*>::iterator it = m_MyDocuments.begin();
                  std::list<CLampDoc*>::iterator end = m_MyDocuments.end();

                  while(it != end)
                  {
                     (*it)->UpdateLOLsRecurse();
                     it++;
                  }
               }
            }
            else if(pDD->m_dt == DT_SHACKMSG)
            {
               if(pDD->m_data != NULL)
               {
                  CXMLTree xmldata;
                  if(GetXMLDataFromString(xmldata, (const char *)pDD->m_data, pDD->m_datasize))
                  {
                     // look for unread messages on page 1
                     m_unreadmessagecount = 0;

                     if(xmldata.GetTag() == L"messages")
                     {
                        int count = xmldata.CountChildren();
                        for(int i = 0; i < count; i++)
                        {
                           CXMLElement *pChild = xmldata.GetChildElement(i);
                           if(pChild != NULL &&
                              pChild->GetTag() == L"message")
                           {
                              UCString temp = pChild->GetAttributeValue(L"status");
                              if(temp == L"unread")
                              {
                                 m_unreadmessagecount++;
                              }
                           }
                        }
                     }

                     UpdateNewMessages();
               
                     // if the inbox is open, and it is on page 1, update it
                     std::list<CLampView*>::iterator it = m_views.begin();
                     std::list<CLampView*>::iterator end = m_views.end();

                     while(it != end)
                     {
                        if((*it)->GetDocument()->GetDataType() == DDT_SHACKMSG &&
                           (*it)->GetDocument()->GetShackMessageType() == SMT_INBOX &&
                           (*it)->GetDocument()->GetPage() == 1 &&
                           !(*it)->GetDLGUp())
                        {
                           (*it)->GetDocument()->ReadShackMessages(xmldata);
                        }
                        it++;
                     }

                     CLampView *pView = ((CMainFrame*)GetMainWnd())->GetActiveLampView();
                     if(pView != NULL)
                     {
                        pView->InvalidateEverything();
                     }
                  }
               }
            }
            else if(pDD->m_dt == DT_SHACK_SHACKMSG)
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

               m_unreadmessagecount = 0;

               tree<htmlcxx::HTML::Node>::sibling_iterator it;
               if(HTML_FindChild(dom,it, "body"))
               {
                  if(HTML_FindChild_HasAttribute(it,it, "div", "id", "container"))
                  {
                     if(HTML_FindChild_HasAttribute(it,it, "div", "id", "content"))
                     {
                        if(HTML_FindChild_HasAttribute(it,it, "div", "id", "messages_wrap"))
                        {
                           tree<htmlcxx::HTML::Node>::sibling_iterator messages_it;
                           if(HTML_FindChild_HasAttribute(it,messages_it, "div", "id", "message_center"))
                           {
                              if(HTML_FindChild(messages_it,messages_it, "form"))
                              {
                                 if(HTML_FindChild_HasAttribute(messages_it,messages_it, "ul", "id", "messages"))
                                 {
                                    tree<htmlcxx::HTML::Node>::sibling_iterator sit = messages_it.begin();
                                    tree<htmlcxx::HTML::Node>::sibling_iterator send = messages_it.end();
                                    
                                    while(sit != send)
                                    {
                                       if(sit->tagName() == "li")
                                       {
                                          std::string value;
                                          if(HTML_StartsWithAttribute(sit, "class", "message",&value))
                                          {
                                             if(value != " read")
                                             {
                                                m_unreadmessagecount++;
                                             }
                                          }
                                       }
                                       sit++;
                                    }
                                 }
                              }
                           }
                        }
                     }

                     if(m_userid == 0)
                     {
                        const char *work = strstr((const char*)pDD->m_stdstring.data(),"<input type=\"hidden\" name=\"uid\" value=\"");
                        if(work != NULL)
                        {
                           work += 39;
                           m_userid = strtoul(work,NULL, 10);
                        }
                     }
                  
                     UpdateNewMessages();
               
                     // if the inbox is open, and it is on page 1, update it
                     std::list<CLampView*>::iterator vit = m_views.begin();
                     std::list<CLampView*>::iterator vend = m_views.end();

                     while(vit != vend)
                     {
                        if((*vit)->GetDocument()->GetDataType() == DDT_SHACKMSG &&
                           (*vit)->GetDocument()->GetShackMessageType() == SMT_INBOX &&
                           (*vit)->GetDocument()->GetPage() == 1 &&
                           !(*vit)->GetDLGUp())
                        {
                           (*vit)->GetDocument()->ReadShackMessagesHTML(pDD->m_stdstring);
                        }
                        vit++;
                     }

                     CLampView *pView = ((CMainFrame*)GetMainWnd())->GetActiveLampView();
                     if(pView != NULL)
                     {
                        pView->InvalidateEverything();
                     }
                  }
               }
            }
            else if(pDD->m_dt == DT_GET_IMAGE)
            {
               if(IsValidImageIndex(pDD->m_id))
               {
                  CImageCacheItem &ici = m_imagecache[pDD->m_id];

                  bool bGotImage = false;

                  if(pDD->m_data != NULL)
                  {
                     char *data = (char*)pDD->m_data;
                     int size = pDD->m_datasize;

                     if((data[0] == (char)0xff && data[1] == (char)0xe0) ||
                        (data[0] == (char)0xff && data[1] == (char)0xe1) ||
                        (data[0] == (char)0xff && data[1] == (char)0xd8 && data[2] == (char)0xff && data[3] == (char)0xe0) ||
                        (data[0] == (char)0xff && data[1] == (char)0xd8 && data[2] == (char)0xff && data[3] == (char)0xe2) ||
                        (data[0] == (char)0xff && data[1] == (char)0xd8 && data[2] == (char)0xff && data[3] == (char)0xdb) ||
                        (data[0] == (char)0xff && data[1] == (char)0xd8 && data[2] == (char)0xff && data[3] == (char)0xe1) ||
                        (data[0] == (char)0x89 && data[1] == (char)0x50 && data[2] == (char)0x4e && data[3] == (char)0x47))
                     {
                        wchar_t path[MAX_PATH+1]={0};

                        DWORD processId = GetCurrentProcessId();
                        DWORD threadId = GetCurrentThreadId();
                        time_t t = time(&t);
                        struct tm lt;
                        localtime_s(&lt,&t);
                        wchar_t asciitime[8] = {0};
                        swprintf_s(asciitime, 8,
                                   L"%02d%02d%02d",
                                   lt.tm_year-100,
                                   lt.tm_mon+1,
                                   lt.tm_mday);

                        if(GetTempPath(MAX_PATH, path) != 0)
                        {
                           if(path[wcslen(path)-1] != L'\\')
                           {
                              wcscat_s(path,MAX_PATH,L"\\");
                           }

                           bool gotit = false;
                           while(!gotit)
                           {
                              UCString suspect = path;
                              suspect += L"Lamp_";
                              suspect += asciitime;
                              suspect += L"_";         
                              suspect += (int)processId;
                              suspect += L"_";         
                              suspect += (int)threadId;
                              suspect += L"_";
                              suspect += (int)GetTickCount();
                              suspect += ici.m_ext;

                              if(_waccess(suspect,00) == -1)
                              {
                                 FILE *pFile = NULL;
                                 fopen_s(&pFile,suspect.str8(),"wb");
                                 if(pFile != NULL)
                                 {
                                    if(_wcsicmp(ici.m_ext,L".jpg") == 0 ||
                                       _wcsicmp(ici.m_ext,L".jpeg") == 0)
                                    {
                                       if(data[0] == (char)0xff &&
                                          data[1] == (char)0xe0)
                                       {
                                          char t = (char)0xff;
                                          fwrite(&t,1,1,pFile);
                                          t = (char)0xd8;
                                          fwrite(&t,1,1,pFile);
                                       }
                                    }

                                    fwrite(data,1,size,pFile);
                                    fclose(pFile);
                                    gotit = true;
                                                                     

                                    if(data[0] == (char)0x89 && data[1] == (char)0x50 && data[2] == (char)0x4e && data[3] == (char)0x47)
                                    {
                                       ici.m_image.ReadPNG(suspect,true);
                                    }      
                                    else // if(_wcsicmp(ici.m_ext,L".jpg") == 0 || _wcsicmp(ici.m_ext,L".jpeg") == 0)
                                    {
                                       ici.m_image.ReadJpeg(suspect);
                                    }

                                    ici.m_image.EnableCachedStretchImage(true);

                                    // now that we have the real image, invalidate the thumbnail image.
                                    // so that it can be recreated from the real thing.
                                    ici.m_imagethumb.Resize(0,0);

                                    _wunlink(suspect);

                                    bGotImage = true;
                                 }
                              }
                           }
                        }
                     }
                  }
                  
                  if(!bGotImage)
                  {
                     ici.m_image.Resize(152,130);
                     ici.m_image.Fill(0,0,0);

                     RECT rect;
                     rect.top = rect.left = 0;
                     rect.right = 152;
                     rect.bottom = 130;
                     ::SetTextColor(ici.m_image.GetDC(),RGB(255,0,0));
                     ::SetBkMode(ici.m_image.GetDC(),TRANSPARENT);
                     ::DrawText(ici.m_image.GetDC(),L":-(",3,&rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
                  }

                  for(size_t i = 0; i < ici.m_notifylist.size(); i++)
                  {
                     InvalidateContentLayout(ici.m_notifylist[i]);
                  }
                  ici.m_notifylist.clear();
               }
            }
            else if(pDD->m_dt == DT_GET_THUMB)
            {
               if(IsValidImageIndex(pDD->m_id))
               {
                  CImageCacheItem &ici = m_imagecache[pDD->m_id];

                  bool bGotImage = false;

                  if(pDD->m_data != NULL)
                  {
                     char *data = (char*)pDD->m_data;
                     int size = pDD->m_datasize;

                     if((data[0] == (char)0xff && data[1] == (char)0xe0) ||
                        (data[0] == (char)0xff && data[1] == (char)0xe1) ||
                        (data[0] == (char)0xff && data[1] == (char)0xd8 && data[2] == (char)0xff && data[3] == (char)0xe0) ||
                        (data[0] == (char)0xff && data[1] == (char)0xd8 && data[2] == (char)0xff && data[3] == (char)0xe2) ||
                        (data[0] == (char)0xff && data[1] == (char)0xd8 && data[2] == (char)0xff && data[3] == (char)0xdb) ||
                        (data[0] == (char)0xff && data[1] == (char)0xd8 && data[2] == (char)0xff && data[3] == (char)0xe1) ||
                        (data[0] == (char)0x89 && data[1] == (char)0x50 && data[2] == (char)0x4e && data[3] == (char)0x47))
                     {
                        wchar_t path[MAX_PATH+1]={0};

                        DWORD processId = GetCurrentProcessId();
                        DWORD threadId = GetCurrentThreadId();
                        time_t t = time(&t);
                        struct tm lt;
                        localtime_s(&lt,&t);
                        wchar_t asciitime[8] = {0};
                        swprintf_s(asciitime, 8,
                                   L"%02d%02d%02d",
                                   lt.tm_year-100,
                                   lt.tm_mon+1,
                                   lt.tm_mday);

                        if(GetTempPath(MAX_PATH, path) != 0)
                        {
                           if(path[wcslen(path)-1] != L'\\')
                           {
                              wcscat_s(path,MAX_PATH,L"\\");
                           }

                           bool gotit = false;
                           while(!gotit)
                           {
                              UCString suspect = path;
                              suspect += L"Lamp_";
                              suspect += asciitime;
                              suspect += L"_";         
                              suspect += (int)processId;
                              suspect += L"_";         
                              suspect += (int)threadId;
                              suspect += L"_";
                              suspect += (int)GetTickCount();
                              suspect += ici.m_ext;

                              if(_waccess(suspect,00) == -1)
                              {
                                 FILE *pFile = NULL;
                                 fopen_s(&pFile,suspect.str8(),"wb");
                                 if(pFile != NULL)
                                 {
                                    if(_wcsicmp(ici.m_ext,L".jpg") == 0 ||
                                       _wcsicmp(ici.m_ext,L".jpeg") == 0 ||
                                       _wcsicmp(ici.m_ext,L".png") == 0)
                                    {
                                       if(data[0] == (char)0xff &&
                                          data[1] == (char)0xe0)
                                       {
                                          char t = (char)0xff;
                                          fwrite(&t,1,1,pFile);
                                          t = (char)0xd8;
                                          fwrite(&t,1,1,pFile);
                                       }
                                    }

                                    fwrite(data,1,size,pFile);
                                    fclose(pFile);
                                    gotit = true;
                                    
                                    if(data[0] == (char)0x89 && data[1] == (char)0x50 && data[2] == (char)0x4e && data[3] == (char)0x47)
                                    {
                                       ici.m_imagethumb.ReadPNG(suspect,true);
                                    }
                                    else
                                    {
                                       ici.m_imagethumb.ReadJpeg(suspect);
                                    }
                                    
                                    ici.m_image.EnableCachedStretchImage(true);

                                    _wunlink(suspect);

                                    bGotImage = true;
                                 }
                              }
                           }
                        }
                     }
                  }
                  
                  if(!bGotImage)
                  {
                     ici.m_imagethumb.Resize(152,130);
                     ici.m_imagethumb.Fill(0,0,0);

                     RECT rect;
                     rect.top = rect.left = 0;
                     rect.right = 152;
                     rect.bottom = 130;
                     ::SetTextColor(ici.m_imagethumb.GetDC(),RGB(255,0,0));
                     ::SetBkMode(ici.m_imagethumb.GetDC(),TRANSPARENT);
                     ::DrawText(ici.m_imagethumb.GetDC(),L":-(",3,&rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
                  }

                  for(size_t i = 0; i < ici.m_notifylist.size(); i++)
                  {
                     InvalidateContentLayout(ici.m_notifylist[i]);
                  }
                  ici.m_notifylist.clear();
               }
            }
            

            if(pDD->m_data != NULL)
            {
               free(pDD->m_data);
            }
            delete pDD;
         }
         else
         {
            std::list<CLampDoc*>::iterator it = m_MyDocuments.begin();
            std::list<CLampDoc*>::iterator end = m_MyDocuments.end();

            bool bDocHandledIt = false;

            while(it != end)
            {
               if((*it) == pDD->m_WhoWants)
               {
                  ((CLampDoc*)pDD->m_WhoWants)->ProcessDownload(pDD);
                  bDocHandledIt = true;
                  break;
               }
               it++;
            }

            if(!bDocHandledIt)
            {
               if(pDD->m_data != NULL)
               {
                  free(pDD->m_data);
               }
               delete pDD;
            }
         }
      }

      // Tell MFC no more processing is needed
      return TRUE;
   }
   else
   {
      if(pMsg->message == WM_MBUTTONDOWN)
      {
         CMainFrame *pMainFrame = (CMainFrame*)GetMainWnd();
         if(pMainFrame != NULL)
         {
            CPoint point(pMsg->lParam);
            pMainFrame->MBClick(point,pMsg->hwnd);
         }
      }
   
      // Call MFC to have it continue processing the message
      return CWinThread::PreTranslateMessage(pMsg);
   }
}



// CLampApp construction

CLampApp::CLampApp()
{
   m_smooth_scroll = true;
   m_bShowImageThumbs = true;
   m_bAutoLoadChattypicsThumbs = true;
   m_bDontAutoLoadNWSThumbs = false;
	m_bHiColorIcons = TRUE;
   m_smoothscrollscale = 0.1f;

   m_fontheight = -13;
   m_miscfontheight = -10;
   m_samplefontheight = -9;
   m_textheight = 16;
   m_cellheight = 20;
   m_descent = m_textheight / 4;

   m_textscaler = 1.0f;

   m_pHunspell = NULL;

   m_bDockedMode = false;
   m_bCollapsed = false;
   m_bWasInDockedMode = false;

   m_pDockTab = NULL;

   m_numcachedthreadreplyinserts = 0;

   m_numshow_truncated = 10;

   m_line_thickness = 1;

   m_bPinningInStories = true;
   m_bDoublePageStory = false;
   m_bStartInDockedMode = true;
   m_bShowLOLButtons = true;
   m_bDoUGH = false;
   m_bHideCollapsedPosts = false;
   m_bAlwaysOnTopWhenNotDocked = false;
   m_bInfinatePaging = false;
   m_bGotoNewPost = true;

   m_UGHThreshold = 3;

   m_normal_fontname = L"Arial";
   m_quoted_fontname = L"Times New Roman";
   m_code_fontname = L"Courier New";

   m_num_minutes_check_inbox = 3;

   m_num_minutes_check_lol = 5;

   m_mb_pan_scale = 1.0f;
   m_inertia_friction = 0.01f;

   m_enable_spell_checker = true;
   m_highlight_OP = true;
   m_authorglow = true;

   m_unreadmessagecount = 0;

   wcscpy_s(m_new_messages_text,17,L"000 new messages");

   m_use_shack = true;
   m_allow_gdiplus = true;

   m_defaultbrowser = BT_SYSTEM;
   m_defaultbrowser_nws = BT_SYSTEM;

   m_userid = 0;

   m_pDocWho = NULL;

   g_bSingleThreadStyle = false;

   m_tempimage.Resize(152,130);
   m_tempimage.Fill(0,0,0);

   m_nextimagecacheindex = 0;

   m_modmode = false;

   m_bigskin = false;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CLampApp object

CLampApp theApp;

CRITICAL_SECTION g_ThreadAccess;

// CLampApp initialization

bool CLampApp::PreventMultipleInstances()
{
#ifndef _DEBUG

	// Create mutex, because there cannot be 2 instances of the same application
	m_hMutex = CreateMutex(NULL, FALSE, L"Lamp - Shack Client"); 

	// Check if mutex is created succesfully
	switch (GetLastError())
	{
	case ERROR_SUCCESS:
		// Mutex created successfully. There is no instance running
		break;
		
	case ERROR_ALREADY_EXISTS:
		// Mutex already exists so there is a running instance of our app.
		return false;
		
	default:
		// Failed to create mutex by unknown reason
		return false;
	}
#endif

	// Succeeded
	return true;
}


BOOL CLampApp::InitInstance()
{

   if(PreventMultipleInstances() == false)
   {
      HWND hLamp = ::FindWindow(L"Lamp - Shack Client",NULL);
      if(hLamp != NULL)
      {
         ::PostMessage(hLamp,WM_WAKEUP,0,0);
      }
      return FALSE;
   }

   if(!g_bSingleThreadStyle)
   {
      ::InitializeCriticalSection(&g_ThreadAccess);
   }

   // Parse command line for standard shell commands, DDE, file open

   if(m_lpCmdLine != NULL)
   {
      g_PathToMe = m_lpCmdLine;
   }

   UCString useragent = L"Lamp v";
   useragent += (int)LAMP_VERSION_MAJOR;
   useragent += L".";
   useragent += (int)LAMP_VERSION_MINOR;

   SetUserAgent(useragent.str8());
	//CCommandLineInfo cmdInfo;
	//ParseCommandLine(cmdInfo);
   // g_PathToMe

   // make sure the user path exists
   wchar_t path[MAX_PATH+1]={0,0,0};

   if(GetEnvironmentVariable(L"USERPROFILE",path,MAX_PATH))
   {
      if(path[0])
      {
         m_userpath = path;

         if(m_userpath.endswith(L"\\") == NULL)
         {
            m_userpath += L"\\";
         }

         m_userpath += L"Lamp";
         
         m_appdatapath = path;

         if(m_appdatapath.endswith(L"\\") == NULL)
         {
            m_appdatapath += L"\\";
         }

         m_appdatapath += L"AppData\\Roaming\\Lamp";
      }
   }

   _putenv("TZ=PST8PDT");

   OSVERSIONINFO versionInfo;
   versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

   g_bIsXP = false;

   if(GetVersionEx(&versionInfo))
   {
      if(versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT &&
         versionInfo.dwMajorVersion == 5)
      {
         g_bIsXP = true;
      }
   }

   //if(!g_bIsXP)
   {
      GdiplusStartup(&g_gdiplusToken, &g_gdiplusStartupInput, NULL);
   }

   ReadBookmarks();
   ReadSettingsFile();
   ReadSkinFiles();
   CollectFlagImages();
   
   UCString langfile;
   langfile = L"hun_dic\\";
   langfile += m_lang;
   langfile += L".aff";

   UCString en_US_aff;
   en_US_aff.PathToMe(langfile);

   langfile = L"hun_dic\\";
   langfile += m_lang;
   langfile += L".dic";

   UCString en_US_dic;
   en_US_dic.PathToMe(langfile);

   m_pHunspell = new Hunspell(en_US_aff.str8(true),en_US_dic.str8(true));

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Lamp"));
	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)
   WriteProfileStringW(L"Workspace\\Keyboard-0",L"Accelerators",NULL);
   WriteProfileStringW(L"login",L"username",NULL);
   WriteProfileStringW(L"login",L"password",NULL);
   WriteProfileStringW(L"view",L"smooth_scroll",NULL);
   WriteProfileStringW(L"view",L"AutoShowLoadedImages",NULL);
   WriteProfileStringW(L"view",L"text_scale",NULL);
   WriteProfileStringW(L"view",L"DockedMode",NULL);
   WriteProfileStringW(L"view",L"PinningInStories",NULL);
   WriteProfileStringW(L"view",L"DoublePageStory",NULL);

   m_fontheight = (int)(-13.0f * m_textscaler);
   m_miscfontheight = (int)(-10.0f * m_textscaler);
   m_samplefontheight = (int)(-9.0f * m_textscaler);
   m_textheight = (int)(16.0f * m_textscaler);
   m_cellheight = (int)(20.0f * m_textscaler);
   CalcDescent();

   int widths[4];
   GetCharWidths(L"www", widths, 3, false, false, ShowSmallLOL(), GetNormalFontName());
   m_LOLFieldWidth = widths[0] + widths[1] + widths[2];

   UpdateNewMessages();

   GenerateLightningBolt();
   GenerateFlagActives();

   ///
   WNDCLASSW wndcls;

   memset(&wndcls, 0, sizeof(WNDCLASSW));   // start with NULL
                                            // defaults

   wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;

   //you can specify your own window procedure
   wndcls.lpfnWndProc = ::DefWindowProc; 
   wndcls.hInstance = AfxGetInstanceHandle();
   wndcls.hIcon = LoadIcon(IDR_MAINFRAME); // or load a different icon
   wndcls.hCursor = LoadCursor( IDC_ARROW );
   wndcls.hbrBackground = 0;
   wndcls.lpszMenuName = NULL;
   // Specify your own class name for using FindWindow later
   wndcls.lpszClassName = _T("Lamp - Shack Client");

   // Register the new class and exit if it fails
   if(!AfxRegisterClass(&wndcls))
   {
      TRACE("Class Registration Failed\n");
      return FALSE;
   }
   ///
   
	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_LampTYPE,
		RUNTIME_CLASS(CLampDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CLampView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

   if(m_bStartInDockedMode)
   {
      OnViewDocktop();
   }
   else if(m_bAlwaysOnTopWhenNotDocked)
   {
      m_bAlwaysOnTopWhenNotDocked = false;// because the next call toggles it
      OnAlwaysOnTopWhenNotDocked();
   }
   
   if(m_session.size() > 0)
   {
      // load the tabs that were closed in the last session
      for(size_t i = 0; i < m_session.size(); i++)
      {
         OpenDocumentFile(m_session[i]);
      }

      m_session.clear();
   }
   else
   {
      // launch a latestchatty tab
      OnFileNew();
   }

   // get lols
   RefreshLOLs();

   return TRUE;
}

int CLampApp::ExitInstance() 
{
   ClearKnownPosts();

   std::map<unsigned int,CXMLElement *>::iterator it = m_cachedthreadreplies.begin();
   std::map<unsigned int,CXMLElement *>::iterator end = m_cachedthreadreplies.end();
   while(it != end)
   {
      delete it->second;
      it++;
   }
   m_cachedthreadreplies.clear();

   if(m_pDockTab != NULL)
   {
      delete m_pDockTab;
      m_pDockTab = NULL;
   }

   if(m_pHunspell != NULL)
   {
      delete m_pHunspell;
      m_pHunspell = false;
   }
   
   WriteBookmarks();
   WriteSettingsFile();

   if(!g_bSingleThreadStyle)
   {
      ::DeleteCriticalSection(&g_ThreadAccess);
   }

   //if(!g_bIsXP)
   {
      GdiplusShutdown(g_gdiplusToken);
   }

   return CWinApp::ExitInstance();
}

CDocument* CLampApp::OpenDocumentFile(LPCTSTR lpszFileName)
{
   //CDocument* pNewDoc = CWinAppEx::OpenDocumentFile(lpszFileName);

   m_open_doc = lpszFileName;
   CWinApp::OnFileNew();
   CDocument* pNewDoc = m_pDocWho;
   m_open_doc = "";
   m_pDocWho = NULL;

   UpdateTabSizes();

   return pNewDoc;
}

void CLampApp::OnFileNew()
{
	CWinApp::OnFileNew();

   UpdateTabSizes();
}


void CLampApp::UpdateTabSizes()
{
   CMainFrame *pMainFrame = (CMainFrame*)GetMainWnd();
   if(pMainFrame != NULL &&
      m_MyDocuments.size() > 0)
   {
      CHackedTabCtrl *tabctrl = (CHackedTabCtrl*)pMainFrame->GetCA()->FindActiveTabWnd();
      if(tabctrl != NULL)
      {      
         bool bMakeFirstVisible = false;

         CRect areaavail;
         tabctrl->GetTabsRect(areaavail);
         int widthavail = areaavail.Width();
         int widthneeded = tabctrl->m_nTabsTotalWidth;

         if(widthneeded < (widthavail - 20 ))
         {
            while(widthneeded < (widthavail - 20 ))
            {
               std::list<CLampDoc*>::iterator it = m_MyDocuments.end();
               it--;
               std::list<CLampDoc*>::iterator begin = m_MyDocuments.begin();

               CLampDoc *pShortestName = NULL;
               int shortestname = 9999;

               while(1)
               {
                  if((*it)->GetAtualTitleSize() < shortestname &&
                     (*it)->GetAtualTitleSize() < (*it)->GetDesiredTitleSize())
                  {
                     pShortestName = (*it);
                     shortestname = pShortestName->GetAtualTitleSize();
                  }

                  if(it == begin)
                  {
                     break;
                  }
                  else
                  {
                     it--;
                  }
               }

               if(pShortestName != NULL)
               {
                  pShortestName->ExpandAtualTitle(1);
                  widthneeded = tabctrl->m_nTabsTotalWidth;
               }
               else
               {
                  break;
               }
            }
         }
         else if(widthavail < widthneeded)
         {
            while(widthavail < widthneeded)
            {
               std::list<CLampDoc*>::iterator it = m_MyDocuments.end();
               it--;
               std::list<CLampDoc*>::iterator begin = m_MyDocuments.begin();

               CLampDoc *pLongestName = (*it);
               int longestname = pLongestName->GetAtualTitleSize();

               while(1)
               {
                  if((*it)->GetAtualTitleSize() > longestname)
                  {
                     pLongestName = (*it);
                     longestname = pLongestName->GetAtualTitleSize();
                  }

                  if(it == begin)
                  {
                     break;
                  }
                  else
                  {
                     it--;
                  }
               }

               if(pLongestName->GetAtualTitleSize() > 1)
               {
                  pLongestName->TrimAtualTitle(1);
                  widthneeded = tabctrl->m_nTabsTotalWidth;
                  bMakeFirstVisible = true;
               }
               else
               {
                  bMakeFirstVisible = false;
                  break;
               }
            }
         }

         pMainFrame->GetCA()->UpdateMDITabbedGroups(TRUE);
         if(bMakeFirstVisible)
         {
            tabctrl->EnsureVisible(0);
         }
      }
   }
}


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

   DECLARE_MESSAGE_MAP()
// Implementation
protected:
public:
   afx_msg void OnBnClickedTraffic();
};

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
   ON_BN_CLICKED(ID_NETWORK_TRAFFIC_ID, &CAboutDlg::OnBnClickedTraffic)
END_MESSAGE_MAP()

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CAboutDlg::OnBnClickedTraffic()
{
   theApp.DisplayDownload();
   OnOK();
}

// App command to run the dialog
void CLampApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CLampApp customization load/save methods

void CLampApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
   /*
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
   */
   bNameValid = strName.LoadString(IDS_COPYLINK_MENU);
	ASSERT(bNameValid);
   GetContextMenuManager()->AddMenu(strName, IDR_POPUP_COPYLINK_MENU);

   bNameValid = strName.LoadString(IDS_USERLINK_MENU);
	ASSERT(bNameValid);
   GetContextMenuManager()->AddMenu(strName, IDR_POPUP_USERLINK_MENU);

   strName = L"EditSel";
   GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT_SEL);

   strName = L"EditNoSel";
   GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT_NOSEL);

   strName = L"Sel";
   GetContextMenuManager()->AddMenu(strName, IDR_POPUP_SEL);
   
   strName = L"NoSel";
   GetContextMenuManager()->AddMenu(strName, IDR_POPUP_NOSEL);
}

void CLampApp::LoadCustomState()
{
}

void CLampApp::SaveCustomState()
{
}

void CLampApp::ReadSettingsFile()
{
   CXMLTree hostxml;
   
   UCString settingspath_self;
   settingspath_self.PathToMe(L"settings.xml");
   
   UCString settingspath_appdata = m_appdatapath;
   settingspath_appdata+= L"\\settings.xml";
   
   if(g_bIsXP ||
      (_waccess(settingspath_self,00) == 0 &&
       _waccess(settingspath_appdata,00) != 0))
   {
      // if the appdata version doesn't exist, then read the install folder
      hostxml.Read(settingspath_self);
   }
   else if(_waccess(settingspath_self,00) == 0 &&
           _waccess(settingspath_self,06) == 0 &&
           settingspath_self.Find(L"Program Files") == NULL)
   {
      // I will be able to write to the install folder later, so read it from there
      hostxml.Read(settingspath_self);
   }
   else
   {
      // I can't write to the lamp folder, so read it from app folder
      hostxml.Read(settingspath_appdata);
   }

   CXMLElement *setting;

   setting = hostxml.FindChildElement(L"use_shack");
   if(setting!=NULL) m_use_shack = setting->GetValue();
   else m_use_shack = true;

   setting = hostxml.FindChildElement(L"allow_gdiplus");
   if(setting!=NULL) m_allow_gdiplus = setting->GetValue();
   else m_allow_gdiplus = true;

   setting = hostxml.FindChildElement(L"default_browser");
   if(setting!=NULL)
   {
      UCString temp = setting->GetValue();
           if(temp == L"system")             m_defaultbrowser = BT_SYSTEM;
      else if(temp == L"chrome")             m_defaultbrowser = BT_CHROME;
      else if(temp == L"chrome_incognito")   m_defaultbrowser = BT_CHROME_INCOGNITO;
      else if(temp == L"firefox")            m_defaultbrowser = BT_FIREFOX;
      else if(temp == L"ie")                 m_defaultbrowser = BT_IE;
      else if(temp == L"ie_private")         m_defaultbrowser = BT_IE_PRIVATE;
      else if(temp == L"safari")             m_defaultbrowser = BT_SAFARI;
      else m_defaultbrowser = BT_SYSTEM;
   }
   else m_defaultbrowser = BT_SYSTEM;

   setting = hostxml.FindChildElement(L"default_browser_nws");
   if(setting!=NULL)
   {
      UCString temp = setting->GetValue();
           if(temp == L"system")             m_defaultbrowser_nws = BT_SYSTEM;
      else if(temp == L"chrome")             m_defaultbrowser_nws = BT_CHROME;
      else if(temp == L"chrome_incognito")   m_defaultbrowser_nws = BT_CHROME_INCOGNITO;
      else if(temp == L"firefox")            m_defaultbrowser_nws = BT_FIREFOX;
      else if(temp == L"ie")                 m_defaultbrowser_nws = BT_IE;
      else if(temp == L"ie_private")         m_defaultbrowser_nws = BT_IE_PRIVATE;
      else if(temp == L"safari")             m_defaultbrowser_nws = BT_SAFARI;
      else m_defaultbrowser_nws = BT_SYSTEM;
   }
   else m_defaultbrowser_nws = BT_SYSTEM;

   setting = hostxml.FindChildElement(L"userhost");
   if(setting!=NULL) m_userhostname = setting->GetValue();
   else m_userhostname = L"shackapi.stonedonkey.com";

   setting = hostxml.FindChildElement(L"lolhost");
   if(setting!=NULL) m_lolhostname = setting->GetValue();
   else m_lolhostname = L"lmnopc.com";

   setting = hostxml.FindChildElement(L"profilehost");
   if(setting!=NULL) m_profilehostname = setting->GetValue();
   else m_profilehostname = L"chattyprofil.es";

   setting = hostxml.FindChildElement(L"username");
   if(setting!=NULL) m_username = setting->GetValue();

   setting = hostxml.FindChildElement(L"password");
   if(setting!=NULL)
   {
      UCString temp = setting->GetValue();
      if(!temp.IsEmpty())
      {
         byte *buffer = (byte *)malloc(temp.Length());
         if(buffer != NULL)
         {
            size_t numbytes = temp.DecodeBase64TextToBinary(buffer, temp.Length());
            if(numbytes > 0)
            {
               byte *work = buffer;
               byte *end = buffer + numbytes;
               while(work < end)
               {
                  *work  = *work ^ 0xFF;
                  work++;
               }               
               *end = 0;

               m_password = (const char*)buffer;
            }

            free(buffer);
         }
      }
   }

   CheckForModMode();

   setting = hostxml.FindChildElement(L"tab_title_word_limit");
   if(setting!=NULL) m_tab_title_word_limit = setting->GetValue();
   else m_tab_title_word_limit = 7;

   setting = hostxml.FindChildElement(L"tab_title_char_limit");
   if(setting!=NULL) m_tab_title_char_limit = setting->GetValue();
   else m_tab_title_char_limit = 50;

   setting = hostxml.FindChildElement(L"smooth_scroll_scale");
   if(setting!=NULL) m_smoothscrollscale = setting->GetValue();
   else m_smoothscrollscale = 0.1f;
   if(m_smoothscrollscale == 0.0f)
      m_smoothscrollscale = 0.1f;

   setting = hostxml.FindChildElement(L"hours_expire");
   if(setting!=NULL) m_hours_expire = setting->GetValue();
   else m_hours_expire = 18;

   setting = hostxml.FindChildElement(L"mseconds_preview_timer");
   if(setting!=NULL) m_mseconds_preview_timer = setting->GetValue();
   else m_mseconds_preview_timer = 500;
   if(m_mseconds_preview_timer == 0)
      m_mseconds_preview_timer = 1;

   setting = hostxml.FindChildElement(L"hover_preview_percent_stepsize");
   if(setting!=NULL) m_hover_preview_percent_stepsize = setting->GetValue();
   else m_hover_preview_percent_stepsize = 0.1f;
   if(m_hover_preview_percent_stepsize == 0.0f)
      m_hover_preview_percent_stepsize = 0.001f;

   setting = hostxml.FindChildElement(L"skin_folder");
   if(setting!=NULL) m_skinname = setting->GetValue();
   else m_skinname = L"roundshack";

   setting = hostxml.FindChildElement(L"mouse_wheel_scale");
   if(setting!=NULL) m_mouse_wheel_scale = setting->GetValue();
   else m_mouse_wheel_scale = 1.0f;
   if(m_mouse_wheel_scale == 0.0f)
      m_mouse_wheel_scale = 1.0f;

   setting = hostxml.FindChildElement(L"mb_pan_scale");
   if(setting!=NULL) m_mb_pan_scale = setting->GetValue();
   else m_mb_pan_scale = 1.0f;
   if(m_mb_pan_scale == 0.0f)
      m_mb_pan_scale = 1.0f;

   setting = hostxml.FindChildElement(L"inertia_friction");
   if(setting!=NULL) m_inertia_friction = setting->GetValue();
   else m_inertia_friction = 0.025f;
   if(m_inertia_friction < 0.000000001f)
      m_inertia_friction = 0.000000001f;
   if(m_inertia_friction > 1.0f)
      m_inertia_friction = 1.0f;

   setting = hostxml.FindChildElement(L"lang");
   if(setting!=NULL) m_lang = setting->GetValue();
   else m_lang = L"en_US";

   setting = hostxml.FindChildElement(L"numshow_truncated");
   if(setting!=NULL) m_numshow_truncated = setting->GetValue();
   else m_numshow_truncated = 10;

   setting = hostxml.FindChildElement(L"line_thickness");
   if(setting!=NULL) m_line_thickness = setting->GetValue();
   else m_line_thickness = 1;

   setting = hostxml.FindChildElement(L"smooth_scroll");
   if(setting!=NULL) m_smooth_scroll = setting->GetValue();
   else m_smooth_scroll = true;

   setting = hostxml.FindChildElement(L"stroke_preview_edges");
   if(setting!=NULL) m_stroke_preview_edges = setting->GetValue();
   else m_stroke_preview_edges = false;

   setting = hostxml.FindChildElement(L"ShowImageThumbs");
   if(setting!=NULL) m_bShowImageThumbs = setting->GetValue();
   else m_bShowImageThumbs = true;

   setting = hostxml.FindChildElement(L"AutoLoadChattypicsThumbs");
   if(setting!=NULL) m_bAutoLoadChattypicsThumbs = setting->GetValue();
   else m_bAutoLoadChattypicsThumbs = true;

   setting = hostxml.FindChildElement(L"DontAutoLoadNWSThumbs");
   if(setting!=NULL) m_bDontAutoLoadNWSThumbs = setting->GetValue();
   else m_bDontAutoLoadNWSThumbs = false;

   setting = hostxml.FindChildElement(L"single_thread_style");
   if(setting!=NULL) g_bSingleThreadStyle = setting->GetValue();
   else g_bSingleThreadStyle = false;

   setting = hostxml.FindChildElement(L"text_scale");
   if(setting!=NULL) m_textscaler = setting->GetValue();
   else m_textscaler = 1.0;
   m_textscaler = __min(5.0f,m_textscaler);
   m_textscaler = __max(0.5f,m_textscaler);

   setting = hostxml.FindChildElement(L"DockedMode");
   if(setting!=NULL) m_bStartInDockedMode = setting->GetValue();
   else m_bStartInDockedMode = false;

#ifdef _DEBUG
   // don't allow dock top mode in debug
   // it messes up the debugger when focus changes
   m_bStartInDockedMode = false;
#endif


   setting = hostxml.FindChildElement(L"PinningInStories");
   if(setting!=NULL) m_bPinningInStories = setting->GetValue();
   else m_bPinningInStories = true;

   setting = hostxml.FindChildElement(L"DoublePageStory");
   if(setting!=NULL) m_bDoublePageStory = setting->GetValue();
   else m_bDoublePageStory = false;

   setting = hostxml.FindChildElement(L"ShowLOLButtons");
   if(setting!=NULL) m_bShowLOLButtons = setting->GetValue();
   else m_bShowLOLButtons = true;   

   setting = hostxml.FindChildElement(L"DoUGH");
   if(setting!=NULL) m_bDoUGH = setting->GetValue();
   else m_bDoUGH = false;   

   setting = hostxml.FindChildElement(L"UGHThreshold");
   if(setting!=NULL) m_UGHThreshold = setting->GetValue();
   else m_UGHThreshold = 3;

   setting = hostxml.FindChildElement(L"SmallLOLButtons");
   if(setting!=NULL) m_bShowSmallLOL = setting->GetValue();
   else m_bShowSmallLOL = true;   

   setting = hostxml.FindChildElement(L"HideCollapsedPosts");
   if(setting!=NULL) m_bHideCollapsedPosts = setting->GetValue();
   else m_bHideCollapsedPosts = false;
   
   setting = hostxml.FindChildElement(L"InfinatePaging");
   if(setting!=NULL) m_bInfinatePaging = setting->GetValue();
   else m_bInfinatePaging = false;

   setting = hostxml.FindChildElement(L"GotoNewPost");
   if(setting!=NULL) m_bGotoNewPost = setting->GetValue();
   else m_bGotoNewPost = true;

   setting = hostxml.FindChildElement(L"FlaredBranches");
   if(setting!=NULL) m_bFlaredBranches = setting->GetValue();
   else m_bFlaredBranches = true;

   setting = hostxml.FindChildElement(L"AlternatePostKeys");
   if(setting!=NULL) m_bAlternatePostKeys = setting->GetValue();
   else m_bAlternatePostKeys = false;

   setting = hostxml.FindChildElement(L"LoadImagesInLamp");
   if(setting!=NULL) m_bLoadImagesInLamp = setting->GetValue();
   else m_bLoadImagesInLamp = true;

   setting = hostxml.FindChildElement(L"auto_check_inbox");
   if(setting!=NULL) m_auto_check_inbox = setting->GetValue();
   else m_auto_check_inbox = true;

   setting = hostxml.FindChildElement(L"show_root_selected");
   if(setting!=NULL) m_show_root_selected = setting->GetValue();
   else m_show_root_selected = true;

   setting = hostxml.FindChildElement(L"expand_previews_down");
   if(setting!=NULL) m_expand_previews_down = setting->GetValue();
   else m_expand_previews_down = true;

   setting = hostxml.FindChildElement(L"expand_previews");
   if(setting!=NULL) m_expand_previews = setting->GetValue();
   else m_expand_previews = false;

   setting = hostxml.FindChildElement(L"show_thomw_lols");
   if(setting!=NULL) m_show_thomw_lols = setting->GetValue();
   else m_show_thomw_lols = false;

   setting = hostxml.FindChildElement(L"verbose_lol_previews");
   if(setting!=NULL) m_verbose_lol_previews = setting->GetValue();
   else m_verbose_lol_previews = false;

   setting = hostxml.FindChildElement(L"inverted_lol_previews");
   if(setting!=NULL) m_inverted_lol_previews = setting->GetValue();
   else m_inverted_lol_previews = true;

   setting = hostxml.FindChildElement(L"show_raw_date");
   if(setting!=NULL) m_show_raw_date = setting->GetValue();
   else m_show_raw_date = false;

   setting = hostxml.FindChildElement(L"UseAuthorColorForPreview");
   if(setting!=NULL) m_bUseAuthorColorForPreview = setting->GetValue();
   else m_bUseAuthorColorForPreview = false;
   
   setting = hostxml.FindChildElement(L"AlwaysOnTopWhenNotDocked");
   if(setting!=NULL) m_bAlwaysOnTopWhenNotDocked = setting->GetValue();
   else m_bAlwaysOnTopWhenNotDocked = false;

   setting = hostxml.FindChildElement(L"num_minutes_check_inbox");
   if(setting!=NULL) m_num_minutes_check_inbox = setting->GetValue();
   else m_num_minutes_check_inbox = 3;

   setting = hostxml.FindChildElement(L"enable_spell_checker");
   if(setting!=NULL) m_enable_spell_checker = setting->GetValue();
   else m_enable_spell_checker = true;   

   setting = hostxml.FindChildElement(L"highlight_OP");
   if(setting!=NULL) m_highlight_OP = setting->GetValue();
   else m_highlight_OP = true;

   setting = hostxml.FindChildElement(L"authorglow");
   if(setting!=NULL) m_authorglow = setting->GetValue();
   else m_authorglow = true;

   setting = hostxml.FindChildElement(L"normal_fontname");
   if(setting!=NULL) m_normal_fontname = setting->GetValue();
   else m_normal_fontname = L"Arial";

   setting = hostxml.FindChildElement(L"quoted_fontname");
   if(setting!=NULL) m_quoted_fontname = setting->GetValue();
   else m_quoted_fontname = L"Times New Roman";

   setting = hostxml.FindChildElement(L"code_fontname");
   if(setting!=NULL) m_code_fontname = setting->GetValue();
   else m_code_fontname = L"Courier New";

   setting = hostxml.FindChildElement(L"text_selection_color");
   if(setting!=NULL) m_text_selection_color = setting->GetValue();
   else m_text_selection_color = RGB(0,0,255);

   setting = hostxml.FindChildElement(L"enable_nws");
   if(setting!=NULL) m_enable_nws = setting->GetValue();
   else m_enable_nws = true;

   setting = hostxml.FindChildElement(L"enable_inf");
   if(setting!=NULL) m_enable_inf = setting->GetValue();
   else m_enable_inf = true;

   setting = hostxml.FindChildElement(L"enable_offtopic");
   if(setting!=NULL) m_enable_offtopic = setting->GetValue();
   else m_enable_offtopic = true;

   setting = hostxml.FindChildElement(L"enable_stupid");
   if(setting!=NULL) m_enable_stupid = setting->GetValue();
   else m_enable_stupid = true;

   setting = hostxml.FindChildElement(L"enable_political");
   if(setting!=NULL) m_enable_political = setting->GetValue();
   else m_enable_political = true;


   setting = hostxml.FindChildElement(L"FilterUsernames");
   if(setting != NULL)
   {
      int count = setting->CountChildren();
      for(int i = 0; i < count; i++)
      {
         CXMLElement *name = setting->GetChildElement(i);
         if(name != NULL && name->GetTag() == L"username")
         {
            UCString temp = name->GetValue();
            temp.TrimWhitespace();
            m_filterusernamelist.insert(temp);
         }
      }
   }
   
   setting = hostxml.FindChildElement(L"FilterPhrases");
   if(setting != NULL)
   {
      int count = setting->CountChildren();
      for(int i = 0; i < count; i++)
      {
         CXMLElement *name = setting->GetChildElement(i);
         if(name != NULL && name->GetTag() == L"phrase")
         {
            UCString temp = name->GetValue();
            temp.TrimWhitespace();
            m_filterphraselist.insert(temp);
         }
      }
   }

   m_namelist.clear();
   m_mod_list.clear();

   setting = hostxml.FindChildElement(L"GameDevs");
   if(setting != NULL)
   {
      m_GameDevs = *setting;
      COLORREF color;
      bool enabled;
      UCString attr = setting->GetAttributeValue(L"color");

      if(attr.IsEmpty())
      {
         color = RGB(255,0,255);
         enabled = true;
         m_GameDevs.SetAttributeValue(L"color",L"255,0,255");
         m_GameDevs.SetAttributeValue(L"enable",L"true");
      }
      else
      {
         color = attr;
         enabled = setting->GetAttributeValue(L"enable");
      }
      if(enabled)
      {
         int count = setting->CountChildren();
         for(int i = 0; i < count; i++)
         {
            CXMLElement *name = setting->GetChildElement(i);
            if(name != NULL && name->GetTag() == L"name")
            {
               UCString temp = name->GetValue();
               temp.MakeLower();
               m_namelist[temp] = color;
            }
         }
      }
   }
   else
   {
      m_GameDevs.SetTag(L"GameDevs");
      m_GameDevs.SetAttributeValue(L"color",L"255,0,255");
      m_GameDevs.SetAttributeValue(L"enable",L"true");
   }

   setting = hostxml.FindChildElement(L"Mods");
   if(setting != NULL)
   {
      m_Mods = *setting;
      COLORREF color;
      bool enabled;
      UCString attr = setting->GetAttributeValue(L"color");

      if(attr.IsEmpty())
      {
         color = RGB(255,0,0);
         enabled = true;
         m_Mods.SetAttributeValue(L"color",L"255,0,0");
         m_Mods.SetAttributeValue(L"enable",L"true");
      }
      else
      {
         color = attr;
         enabled = setting->GetAttributeValue(L"enable");
      }
      if(enabled)
      {
         int count = setting->CountChildren();
         for(int i = 0; i < count; i++)
         {
            CXMLElement *name = setting->GetChildElement(i);
            if(name != NULL && name->GetTag() == L"name")
            {
               UCString temp = name->GetValue();
               temp.MakeLower();
               m_namelist[temp] = color;

               temp = name->GetValue();
               temp.TrimWhitespace();
               temp.MakeLower();
               m_mod_list.insert(temp);
            }
         }
      }
   }
   else
   {
      m_Mods.SetTag(L"Mods");
      m_Mods.SetAttributeValue(L"color",L"255,0,0");
      m_Mods.SetAttributeValue(L"enable",L"true");
   }

   CheckForModMode();

   setting = hostxml.FindChildElement(L"ShackEmployees");
   if(setting != NULL)
   {
      m_ShackEmployees = *setting;
      COLORREF color;
      bool enabled;
      UCString attr = setting->GetAttributeValue(L"color");

      if(attr.IsEmpty())
      {
         color = RGB(0,255,0);
         enabled = true;
         m_ShackEmployees.SetAttributeValue(L"color",L"0,255,0");
         m_ShackEmployees.SetAttributeValue(L"enable",L"true");
      }
      else
      {
         color = attr;
         enabled = setting->GetAttributeValue(L"enable");
      }
      if(enabled)
      {
         int count = setting->CountChildren();
         for(int i = 0; i < count; i++)
         {
            CXMLElement *name = setting->GetChildElement(i);
            if(name != NULL && name->GetTag() == L"name")
            {
               UCString temp = name->GetValue();
               temp.MakeLower();
               m_namelist[temp] = color;
            }
         }
      }
   }
   else
   {
      m_ShackEmployees.SetTag(L"ShackEmployees");
      m_ShackEmployees.SetAttributeValue(L"color",L"0,255,0");
      m_ShackEmployees.SetAttributeValue(L"enable",L"true");
   }
   
   // cheat sheet
   setting = hostxml.FindChildElement(L"CheatSheet");
   if(setting != NULL)
   {
      int num = setting->CountChildren();
      for(int i = 0; i < num; i++)
      {
         CXMLElement *cheat = setting->GetChildElement(i);
         if(cheat != NULL && cheat->GetTag() == L"cheat")
         {
            CSuggestion temp;
            temp.value = cheat->GetValue();
            temp.display = cheat->GetValue();
            temp.display.ReplaceAll(L'\n',L' ');
            if(temp.display.Length() > 20)
            {
               temp.display.TrimEnd(temp.display.Length() - 20);
               temp.display += L"...";
            }
            m_cheatsheet.push_back(temp);
         }
      }
   }
   /*
   else
   {
      UCChar foo[2];
      foo[1] = 0;
      foo[0] = 0x2665; //heart
      m_cheatsheet.push_back(UCString(foo));
      foo[0] = 0x266B; //musical note
      m_cheatsheet.push_back(UCString(foo));
      foo[0] = 0x221E; //infinity
      m_cheatsheet.push_back(UCString(foo));
      foo[0] = 0x2122; //TM
      m_cheatsheet.push_back(UCString(foo));
      foo[0] = 0x20AC; //Euro
      m_cheatsheet.push_back(UCString(foo));
      foo[0] = 0x00A5; //Yen
      m_cheatsheet.push_back(UCString(foo));
      foo[0] = 0x00A3; //Pound
      m_cheatsheet.push_back(UCString(foo));
      foo[0] = 0x00A9; //Copyright
      m_cheatsheet.push_back(UCString(foo));
      foo[0] = 0x00AE; //Registered
      m_cheatsheet.push_back(UCString(foo));
      foo[0] = 0x00B0; //Degree
      m_cheatsheet.push_back(UCString(foo));            
   }
   */

   // restore session
   setting = hostxml.FindChildElement(L"Session");
   if(setting != NULL)
   {
      int num = setting->CountChildren();
      for(int i = 0; i < num; i++)
      {
         CXMLElement *tab = setting->GetChildElement(i);
         if(tab != NULL && tab->GetTag() == L"tab")
         {
            m_session.push_back(tab->GetValue());
         }
      }
   }

   // collapsed threads
   setting = hostxml.FindChildElement(L"collapsed_threads");
   if(setting != NULL)
   {
      int num = setting->CountChildren();
      for(int i = 0; i < num; i++)
      {
         CXMLElement *thread = setting->GetChildElement(i);
         if(thread != NULL)
         {
            size_t days2 = 1000 * 60 * 60 * 24 * 2;
            size_t now = ::GetTickCount();
            size_t then = (size_t)((unsigned int)thread->GetAttributeValue(L"time"));
            if(now - then < days2)
            {
               unsigned int id = (unsigned int)thread->GetTag();
               m_mycollapses.push_back(CollapsedThread(id,then));
            }
         }
      }
   }

   // flagged users
   setting = hostxml.FindChildElement(L"flagged_users");
   if(setting != NULL)
   {
      int num = setting->CountChildren();
      for(int i = 0; i < num; i++)
      {
         CXMLElement *entry = setting->GetChildElement(i);
         if(entry != NULL)
         {
            UCString username = entry->GetAttributeValue(L"name");

            CFlaggedUser flagged_user;
            flagged_user.m_flag = entry->GetAttributeValue(L"flag");
            flagged_user.m_note = entry->GetValue();
            
            m_flagged_users[username] = flagged_user;
         }
      }
   }
   
}

void CLampApp::WriteSettingsFile()
{
   CXMLTree settingsxml;

   settingsxml.SetTag(L"Settings");

   settingsxml.AddChildComment(L"settings");

   settingsxml.AddChildComment(L"Should we use the Shack, as opposed to stonedonkey");
   settingsxml.AddChildElement(L"use_shack",UCString(m_use_shack));
   settingsxml.AddChildElement(L"allow_gdiplus",UCString(m_allow_gdiplus));

   UCString temp;
   switch(m_defaultbrowser)
   {
   case BT_SYSTEM:           temp = L"system"; break;
   case BT_CHROME:           temp = L"chrome"; break;
   case BT_CHROME_INCOGNITO: temp = L"chrome_incognito"; break;
   case BT_FIREFOX:          temp = L"firefox"; break;
   case BT_IE:               temp = L"ie"; break;
   case BT_IE_PRIVATE:       temp = L"ie_private"; break;
   case BT_SAFARI:           temp = L"safari"; break;
   default:                  temp = L"system"; break;
   }
   settingsxml.AddChildElement(L"default_browser",temp);

   switch(m_defaultbrowser_nws)
   {
   case BT_SYSTEM:           temp = L"system"; break;
   case BT_CHROME:           temp = L"chrome"; break;
   case BT_CHROME_INCOGNITO: temp = L"chrome_incognito"; break;
   case BT_FIREFOX:          temp = L"firefox"; break;
   case BT_IE:               temp = L"ie"; break;
   case BT_IE_PRIVATE:       temp = L"ie_private"; break;
   case BT_SAFARI:           temp = L"safari"; break;
   default:                  temp = L"system"; break;
   }
   settingsxml.AddChildElement(L"default_browser_nws",temp);

   settingsxml.AddChildComment(L"host for getting user info");
   settingsxml.AddChildElement(L"userhost",m_userhostname);

   settingsxml.AddChildComment(L"host for sending lol votes");
   settingsxml.AddChildElement(L"lolhost",m_lolhostname);

   settingsxml.AddChildComment(L"host for getting profiles");
   settingsxml.AddChildElement(L"profilehost",m_profilehostname);

   settingsxml.AddChildComment(L"login stuff");
   settingsxml.AddChildElement(L"username",m_username);


   if(!m_password.IsEmpty())
   {
      byte *buffer = (byte *)m_password.str8(false,CET_UTF8);
      if(buffer != NULL)
      {
         size_t numbytes = m_password.Length();
         if(numbytes > 0)
         {
            byte *work = buffer;
            byte *end = buffer + numbytes;
            while(work < end)
            {
               *work  = *work ^ 0xFF;
               work++;
            }

            UCString temp;
            temp.EncodeBinaryToBase64Text(buffer,numbytes);

            settingsxml.AddChildElement(L"password",temp);

            // put back
            work = buffer;
            while(work < end)
            {
               *work  = *work ^ 0xFF;
               work++;
            }
         }
      }
   }
   

   settingsxml.AddChildComment(L"general stuff");
   settingsxml.AddChildElement(L"tab_title_word_limit",UCString(m_tab_title_word_limit));
   settingsxml.AddChildElement(L"tab_title_char_limit",UCString(m_tab_title_char_limit));
   settingsxml.AddChildElement(L"smooth_scroll_scale",UCString(m_smoothscrollscale));
   settingsxml.AddChildElement(L"stroke_preview_edges",UCString(m_stroke_preview_edges));
   settingsxml.AddChildElement(L"hours_expire",UCString(m_hours_expire));
   settingsxml.AddChildElement(L"mseconds_preview_timer",UCString(m_mseconds_preview_timer));
   settingsxml.AddChildElement(L"hover_preview_percent_stepsize",UCString(m_hover_preview_percent_stepsize));
   settingsxml.AddChildElement(L"skin_folder",m_skinname);
   settingsxml.AddChildElement(L"mouse_wheel_scale",UCString(m_mouse_wheel_scale));
   settingsxml.AddChildElement(L"mb_pan_scale",UCString(m_mb_pan_scale));
   settingsxml.AddChildElement(L"inertia_friction",UCString(m_inertia_friction));
   settingsxml.AddChildElement(L"lang",m_lang);
   settingsxml.AddChildElement(L"numshow_truncated",UCString(m_numshow_truncated));
   settingsxml.AddChildElement(L"line_thickness",UCString(m_line_thickness));
   settingsxml.AddChildElement(L"smooth_scroll",UCString(m_smooth_scroll));
   settingsxml.AddChildElement(L"ShowImageThumbs",UCString(m_bShowImageThumbs));
   settingsxml.AddChildElement(L"AutoLoadChattypicsThumbs",UCString(m_bAutoLoadChattypicsThumbs));
   settingsxml.AddChildElement(L"DontAutoLoadNWSThumbs",UCString(m_bDontAutoLoadNWSThumbs));
   settingsxml.AddChildElement(L"single_thread_style",UCString(g_bSingleThreadStyle));
   settingsxml.AddChildElement(L"text_scale",UCString(m_textscaler));
   settingsxml.AddChildElement(L"DockedMode",UCString(m_bWasInDockedMode));
   settingsxml.AddChildElement(L"PinningInStories",UCString(m_bPinningInStories));
   settingsxml.AddChildElement(L"DoublePageStory",UCString(m_bDoublePageStory));
   settingsxml.AddChildElement(L"ShowLOLButtons",UCString(m_bShowLOLButtons));
   settingsxml.AddChildElement(L"DoUGH",UCString(m_bDoUGH));
   settingsxml.AddChildElement(L"UGHThreshold",UCString(m_UGHThreshold));
   settingsxml.AddChildElement(L"SmallLOLButtons",UCString(m_bShowSmallLOL));
   settingsxml.AddChildElement(L"HideCollapsedPosts",UCString(m_bHideCollapsedPosts));
   settingsxml.AddChildElement(L"InfinatePaging",UCString(m_bInfinatePaging));
   settingsxml.AddChildElement(L"GotoNewPost",UCString(m_bGotoNewPost));
   settingsxml.AddChildElement(L"FlaredBranches",UCString(m_bFlaredBranches));
   settingsxml.AddChildElement(L"AlternatePostKeys",UCString(m_bAlternatePostKeys));
   settingsxml.AddChildElement(L"LoadImagesInLamp",UCString(m_bLoadImagesInLamp));
   settingsxml.AddChildElement(L"auto_check_inbox",UCString(m_auto_check_inbox));
   settingsxml.AddChildElement(L"show_root_selected",UCString(m_show_root_selected));
   settingsxml.AddChildElement(L"expand_previews_down",UCString(m_expand_previews_down));
   settingsxml.AddChildElement(L"expand_previews",UCString(m_expand_previews));
   settingsxml.AddChildElement(L"show_thomw_lols",UCString(m_show_thomw_lols));
   settingsxml.AddChildElement(L"verbose_lol_previews",UCString(m_verbose_lol_previews));
   settingsxml.AddChildElement(L"inverted_lol_previews",UCString(m_inverted_lol_previews));
   settingsxml.AddChildElement(L"show_raw_date",UCString(m_show_raw_date));
   settingsxml.AddChildElement(L"UseAuthorColorForPreview",UCString(m_bUseAuthorColorForPreview));
   settingsxml.AddChildElement(L"AlwaysOnTopWhenNotDocked",UCString(m_bAlwaysOnTopWhenNotDocked));
   settingsxml.AddChildElement(L"num_minutes_check_inbox",UCString(m_num_minutes_check_inbox));
   settingsxml.AddChildElement(L"enable_spell_checker",UCString(m_enable_spell_checker));
   settingsxml.AddChildElement(L"highlight_OP",UCString(m_highlight_OP));
   settingsxml.AddChildElement(L"authorglow",UCString(m_authorglow));
   settingsxml.AddChildElement(L"normal_fontname",m_normal_fontname);
   settingsxml.AddChildElement(L"quoted_fontname",m_quoted_fontname);
   settingsxml.AddChildElement(L"code_fontname",m_code_fontname);

   temp = GetRValue(m_text_selection_color);
   temp += L',';
   temp += GetGValue(m_text_selection_color);
   temp += L',';
   temp += GetBValue(m_text_selection_color);
   settingsxml.AddChildElement(L"text_selection_color",temp);

   settingsxml.AddChildComment(L"Check your filters!");
   settingsxml.AddChildElement(L"enable_nws",UCString(m_enable_nws));
   settingsxml.AddChildElement(L"enable_inf",UCString(m_enable_inf));
   settingsxml.AddChildElement(L"enable_offtopic",UCString(m_enable_offtopic));
   settingsxml.AddChildElement(L"enable_stupid",UCString(m_enable_stupid));
   settingsxml.AddChildElement(L"enable_political",UCString(m_enable_political));

   settingsxml.AddChildComment(L"Filter out posts by the following users");
   CXMLElement *fun = settingsxml.AddChildElement();
   fun->SetTag(L"FilterUsernames");
   std::set<UCString>::iterator it = m_filterusernamelist.begin();
   std::set<UCString>::iterator end = m_filterusernamelist.end();

   while(it != end)
   {
      fun->AddChildElement(L"username",(*it));      
      it++;
   }

   settingsxml.AddChildComment(L"Filter out posts by the following phrases");
   CXMLElement *fpn = settingsxml.AddChildElement();
   fpn->SetTag(L"FilterPhrases");
   it = m_filterphraselist.begin();
   end = m_filterphraselist.end();

   while(it != end)
   {
      fpn->AddChildElement(L"phrase",(*it));      
      it++;
   }

   settingsxml.AddChildComment(L"Color-Coded usernames");
   CXMLElement *mods = settingsxml.AddChildElement();
   *mods = m_Mods;

   CXMLElement *emp = settingsxml.AddChildElement();
   *emp = m_ShackEmployees;

   CXMLElement *dev = settingsxml.AddChildElement();
   *dev = m_GameDevs;

   // save cheatsheet
   if(m_cheatsheet.size() > 0)
   {
      settingsxml.AddChildComment(L"Cheat Sheet.  These strings will be listed in pop-up.");
      CXMLElement *cheatsheet = settingsxml.AddChildElement();
      cheatsheet->SetTag(L"CheatSheet");

      for(size_t i = 0; i < m_cheatsheet.size(); i++)
      {
         cheatsheet->AddChildElement(L"cheat",m_cheatsheet[i].value);
      }
   }

   // save session
   if(m_pMainWnd != NULL)
   {
      ((CMainFrame*)m_pMainWnd)->RecordSession();
   }

   if(m_session.size() > 0)
   {
      settingsxml.AddChildComment(L"Saved session.  These tabs were open on exit.");
      CXMLElement *session = settingsxml.AddChildElement();
      session->SetTag(L"Session");

      for(size_t i = 0; i < m_session.size(); i++)
      {
         session->AddChildElement(L"tab",m_session[i]);
      }
   }

   // collapsed threads list
   if(m_mycollapses.size() > 0)
   {
      settingsxml.AddChildComment(L"Collapsed Threads (expire in 2 days)");
      CXMLElement *collapsed = settingsxml.AddChildElement();
      collapsed->SetTag(L"collapsed_threads");
      std::list<CollapsedThread>::iterator cit = m_mycollapses.begin();
      std::list<CollapsedThread>::iterator cend = m_mycollapses.end();

      while(cit != cend)
      {
         collapsed->AddChildElement(UCString((*cit).m_thread_id), L"time",UCString((unsigned int)(*cit).m_date));
         cit++;
      }
   }

   // flagged users
   if(m_flagged_users.size() > 0)
   {
      settingsxml.AddChildComment(L"Flagged Users");
      CXMLElement *flagged_users = settingsxml.AddChildElement();
      flagged_users->SetTag(L"flagged_users");

      std::map<UCString,CFlaggedUser>::iterator fit = m_flagged_users.begin();
      std::map<UCString,CFlaggedUser>::iterator fend = m_flagged_users.end();

      while(fit != fend)
      {
         CXMLElement *flagged_user = flagged_users->AddChildElement();
         flagged_user->SetTag(L"user");
         flagged_user->SetValue(fit->second.m_note);
         flagged_user->AddAttribute(L"flag",fit->second.m_flag);
         flagged_user->AddAttribute(L"name",fit->first);
         fit++;
      }
   }

   // try the program files folder
   UCString bmpath;
   
   bmpath.PathToMe(L"settings.xml");
   if(g_bIsXP ||
      bmpath.Find(L"Program Files") == NULL)
   {
      settingsxml.Write(bmpath);
   }
   else
   {
      // try the appdata folder
      if(_waccess(m_appdatapath,00) == -1)
      {
         BOOL bMade = CreateDirectoryW(m_appdatapath, NULL);
      }

      bmpath = m_appdatapath;
      bmpath += L"\\settings.xml";
      settingsxml.Write(bmpath);
   }
}

COLORREF CLampApp::GetUserColor(UCString &name)
{
   UCString temp = name;
   temp.MakeLower();
   std::map<UCString, COLORREF>::iterator it = m_namelist.find(temp);
  
   if(it != m_namelist.end())
   {
      return it->second;
   }

   return m_author_color;
}

void CLampApp::SetSkinFolder(const UCChar *skinname)
{
   m_skinname = skinname;
   ReadSkinFiles();
   InvalidateSkinAllViews();
}

void CLampApp::ReadSkinFiles()
{
   UCString skinfilename = L"skins\\";
   skinfilename += m_skinname;
   skinfilename += L"\\skin.xml";

   UCString skinpath;

   skinpath.PathToMe(skinfilename);

   CXMLTree settings;
   settings.Read(skinpath);
   CXMLElement *setting;

   setting = settings.FindChildElement(L"hover_color");
   if(setting!=NULL) m_hover_color = setting->GetValue();
   else m_hover_color = RGB(255,255,255);

   setting = settings.FindChildElement(L"background_color");
   if(setting!=NULL) m_background_color = setting->GetValue();
   else m_background_color = RGB(0,0,0);

   m_bBackgroundIsDark = true;
   if((GetRValue(m_background_color) + GetGValue(m_background_color) + GetBValue(m_background_color)) > 382)m_bBackgroundIsDark = false;

   setting = settings.FindChildElement(L"rootpost_background_color");
   if(setting!=NULL) m_rootpost_background_color = setting->GetValue();
   else m_rootpost_background_color = RGB(34,34,34);

   setting = settings.FindChildElement(L"edit_background_color");
   if(setting!=NULL) m_edit_background_color = setting->GetValue();
   else m_edit_background_color = RGB(0,0,0);

   setting = settings.FindChildElement(L"post_background_color");
   if(setting!=NULL) m_post_background_color = setting->GetValue();
   else m_post_background_color = RGB(16,16,16);

   setting = settings.FindChildElement(L"post_edge_color");
   if(setting!=NULL) m_post_edge_color = setting->GetValue();
   else m_post_edge_color = RGB(174,174,155);

   setting = settings.FindChildElement(L"post_INF_edge_color");
   if(setting!=NULL) m_post_INF_edge_color = setting->GetValue();
   else m_post_INF_edge_color = RGB(0,191,243);
   
   setting = settings.FindChildElement(L"post_NWS_edge_color");
   if(setting!=NULL) m_post_NWS_edge_color = setting->GetValue();
   else m_post_NWS_edge_color = RGB(255,0,0);

   setting = settings.FindChildElement(L"post_stupid_edge_color");
   if(setting!=NULL) m_post_stupid_edge_color = setting->GetValue();
   else m_post_stupid_edge_color = RGB(0,255,0);

   setting = settings.FindChildElement(L"post_offtopic_edge_color");
   if(setting!=NULL) m_post_offtopic_edge_color = setting->GetValue();
   else m_post_offtopic_edge_color = RGB(174,174,155);

   setting = settings.FindChildElement(L"post_political_edge_color");
   if(setting!=NULL) m_post_political_edge_color = setting->GetValue();
   else m_post_political_edge_color = RGB(248,165,0);

   setting = settings.FindChildElement(L"branch_color");
   if(setting!=NULL) m_branch_color = setting->GetValue();
   else m_branch_color = RGB(128,128,128);

   setting = settings.FindChildElement(L"branchisnew_color");
   if(setting!=NULL) m_branchisnew_color = setting->GetValue();
   else m_branchisnew_color = RGB(0,255,255);

   setting = settings.FindChildElement(L"branchislast_color");
   if(setting!=NULL) m_branchislast_color = setting->GetValue();
   else m_branchislast_color = RGB(0,128,128);

   setting = settings.FindChildElement(L"faded_branch_color");
   if(setting!=NULL) m_faded_branch_color = setting->GetValue();
   else m_faded_branch_color = RGB(40,40,40);

   setting = settings.FindChildElement(L"faded_branchisnew_color");
   if(setting!=NULL) m_faded_branchisnew_color = setting->GetValue();
   else m_faded_branchisnew_color = RGB(0,128,128);

   setting = settings.FindChildElement(L"faded_branchislast_color");
   if(setting!=NULL) m_faded_branchislast_color = setting->GetValue();
   else m_faded_branchislast_color = RGB(0,70,70);

   setting = settings.FindChildElement(L"author_color");
   if(setting!=NULL) m_author_color = setting->GetValue();
   else m_author_color = RGB(255,186,0);

   setting = settings.FindChildElement(L"misc_post_text_color");
   if(setting!=NULL) m_misc_post_text_color = setting->GetValue();
   else m_misc_post_text_color = RGB(102,102,102);

   setting = settings.FindChildElement(L"spoiler_color");
   if(setting!=NULL) m_spoiler_color = setting->GetValue();
   else m_spoiler_color = RGB(56,56,56);

   setting = settings.FindChildElement(L"post_text_color");
   if(setting!=NULL) m_post_text_color = setting->GetValue();
   else m_post_text_color = RGB(255,255,255);

   setting = settings.FindChildElement(L"faded_post_text_color");
   if(setting!=NULL) m_faded_post_text_color = setting->GetValue();
   else m_faded_post_text_color = RGB(135,135,135);

   setting = settings.FindChildElement(L"mypost_color");
   if(setting!=NULL) m_mypost_color = setting->GetValue();
   else m_mypost_color = RGB(0,191,243);
   
   for(int i = 0; i <= 10; i++)
   {
      float percent = ((float)i) / 10.0f;
      m_post_text_color_shade[i] = RGB((((float)GetRValue(m_post_text_color) * percent) + ((float)GetRValue(m_faded_post_text_color) * (1.0f - percent))),
                                       (((float)GetGValue(m_post_text_color) * percent) + ((float)GetGValue(m_faded_post_text_color) * (1.0f - percent))),
                                       (((float)GetBValue(m_post_text_color) * percent) + ((float)GetBValue(m_faded_post_text_color) * (1.0f - percent))));

      m_branch_color_shade[i] = RGB((((float)GetRValue(m_branch_color) * percent) + ((float)GetRValue(m_faded_branch_color) * (1.0f - percent))),
                                    (((float)GetGValue(m_branch_color) * percent) + ((float)GetGValue(m_faded_branch_color) * (1.0f - percent))),
                                    (((float)GetBValue(m_branch_color) * percent) + ((float)GetBValue(m_faded_branch_color) * (1.0f - percent))));

      m_branchisnew_color_shade[i] = RGB((((float)GetRValue(m_branchisnew_color) * percent) + ((float)GetRValue(m_faded_branchisnew_color) * (1.0f - percent))),
                                         (((float)GetGValue(m_branchisnew_color) * percent) + ((float)GetGValue(m_faded_branchisnew_color) * (1.0f - percent))),
                                         (((float)GetBValue(m_branchisnew_color) * percent) + ((float)GetBValue(m_faded_branchisnew_color) * (1.0f - percent))));

      m_branchislast_color_shade[i] = RGB((((float)GetRValue(m_branchislast_color) * percent) + ((float)GetRValue(m_faded_branchislast_color) * (1.0f - percent))),
                                          (((float)GetGValue(m_branchislast_color) * percent) + ((float)GetGValue(m_faded_branchislast_color) * (1.0f - percent))),
                                          (((float)GetBValue(m_branchislast_color) * percent) + ((float)GetBValue(m_faded_branchislast_color) * (1.0f - percent))));

   }

   m_RED = RGB(255,0,0);
   m_GREEN = RGB(141,198,63);
   m_DGREEN = RGB(0,198,0);
   m_BLUE = RGB(68,174,223);
   m_YELLOW = RGB(255,222,0);
   m_OLIVE = RGB(128,128,0);
   m_LIME = RGB(192,255,192);
   m_ORANGE = RGB(247,148,28);
   m_PURPLE = RGB(245,0,255);
   m_PINK = RGB(244,154,193);

   setting = settings.FindChildElement(L"tag_RED");
   if(setting!=NULL) m_RED = setting->GetValue();
   setting = settings.FindChildElement(L"tag_GREEN");
   if(setting!=NULL) m_GREEN = setting->GetValue();
   setting = settings.FindChildElement(L"tag_DGREEN");
   if(setting!=NULL) m_DGREEN = setting->GetValue();
   setting = settings.FindChildElement(L"tag_BLUE");
   if(setting!=NULL) m_BLUE = setting->GetValue();
   setting = settings.FindChildElement(L"tag_YELLOW");
   if(setting!=NULL) m_YELLOW = setting->GetValue();
   setting = settings.FindChildElement(L"tag_OLIVE");
   if(setting!=NULL) m_OLIVE = setting->GetValue();
   setting = settings.FindChildElement(L"tag_LIME");
   if(setting!=NULL) m_LIME = setting->GetValue();
   setting = settings.FindChildElement(L"tag_ORANGE");
   if(setting!=NULL) m_ORANGE = setting->GetValue();
   setting = settings.FindChildElement(L"tag_PURPLE");
   if(setting!=NULL) m_PURPLE = setting->GetValue();
   setting = settings.FindChildElement(L"tag_PINK");
   if(setting!=NULL) m_PINK = setting->GetValue();

   setting = settings.FindChildElement(L"expired_color");
   if(setting!=NULL) m_expired_color = setting->GetValue();
   else m_expired_color = RGB(128,32,32);
   
   setting = settings.FindChildElement(L"expiring_color");
   if(setting!=NULL) m_expiring_color = setting->GetValue();
   else m_expiring_color = RGB(255,0,0);

   setting = settings.FindChildElement(L"stroke_root_edges");
   if(setting!=NULL) m_stroke_root_edges = setting->GetValue();
   else m_stroke_root_edges = false;

   setting = settings.FindChildElement(L"rounded_posts");
   if(setting!=NULL) m_rounded_posts = setting->GetValue();
   else m_rounded_posts = false;

   setting = settings.FindChildElement(L"bigskin");
   if(setting!=NULL) m_bigskin = setting->GetValue();
   else m_bigskin = false;
   
   m_refresh_buffer.Resize(0,0);
   m_refresh_hover_buffer.Resize(0,0);
   m_tab_buffer.Resize(0,0);
   m_tab_hover_buffer.Resize(0,0);
   m_reply_buffer.Resize(0,0);
   m_reply_hover_buffer.Resize(0,0);
   m_up_inactive.Resize(0,0);
   m_up_active.Resize(0,0);
   m_up_hover.Resize(0,0);
   m_down_inactive.Resize(0,0);
   m_down_active.Resize(0,0);
   m_down_hover.Resize(0,0);
   m_thumb_inactive.Resize(0,0);
   m_thumb_active.Resize(0,0);
   m_thumb_hover.Resize(0,0);
   m_thumb_top_inactive.Resize(0,0);
   m_thumb_top_active.Resize(0,0);
   m_thumb_top_hover.Resize(0,0);
   m_thumb_bottom_inactive.Resize(0,0);
   m_thumb_bottom_active.Resize(0,0);
   m_thumb_bottom_hover.Resize(0,0);
   m_thumb_grip_inactive.Resize(0,0);
   m_thumb_grip_active.Resize(0,0);
   m_thumb_grip_hover.Resize(0,0);
   m_track.Resize(0,0);
   m_ontopic.Resize(0,0);
   m_ontopic_hover.Resize(0,0);
   m_inf_hover.Resize(0,0);
   m_inf.Resize(0,0);
   m_infstar.Resize(0,0);
   m_nws_hover.Resize(0,0);
   m_nws.Resize(0,0);
   m_nwsstar.Resize(0,0);
   m_stupid.Resize(0,0);
   m_stupid_hover.Resize(0,0);
   m_stupidstar.Resize(0,0);
   m_offtopic.Resize(0,0);
   m_offtopic_hover.Resize(0,0);
   m_offtopicstar.Resize(0,0);
   m_political.Resize(0,0);
   m_political_hover.Resize(0,0);
   m_politicalstar.Resize(0,0);
   m_nuked.Resize(0,0);
   m_nuked_hover.Resize(0,0);
   m_close.Resize(0,0);
   m_close_hover.Resize(0,0);
   m_post.Resize(0,0);
   m_post_hover.Resize(0,0);
   m_send.Resize(0,0);
   m_send_hover.Resize(0,0);
   m_newthread.Resize(0,0);
   m_newthread_hover.Resize(0,0);
   m_preview.Resize(0,0);
   m_preview_hover.Resize(0,0);
   m_docktabimage.Resize(0,0);
   m_pin_hover.Resize(0,0);
   m_pin_on.Resize(0,0);
   m_pin_off.Resize(0,0);
   m_refresh_story.Resize(0,0);
   m_refresh_story_hover.Resize(0,0);
   m_background.Resize(0,0);
   m_post_background.Resize(0,0);
   m_rootpost_background.Resize(0,0);
   m_reply_background.Resize(0,0);
   m_compose.Resize(0,0);
   m_compose_hover.Resize(0,0);
   m_search.Resize(0,0);
   m_search_hover.Resize(0,0);
   m_forward.Resize(0,0);
   m_forward_hover.Resize(0,0);
   m_delete.Resize(0,0);
   m_delete_hover.Resize(0,0);
   m_mb_pan.Resize(0,0);
   for(size_t i=0;i<8;i++)
      m_refresh_anim[i].Resize(0,0);
   
   UCString imagefilename;
   UCString imagepath;

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\refresh.png";
   imagepath.PathToMe(imagefilename);
   m_refresh_buffer.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\refresh_hover.png";
   imagepath.PathToMe(imagefilename);
   m_refresh_hover_buffer.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\tab.png";
   imagepath.PathToMe(imagefilename);
   m_tab_buffer.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\tab_hover.png";
   imagepath.PathToMe(imagefilename);
   m_tab_hover_buffer.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\reply.png";
   imagepath.PathToMe(imagefilename);
   m_reply_buffer.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\reply_hover.png";
   imagepath.PathToMe(imagefilename);
   m_reply_hover_buffer.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\up_inactive.png";
   imagepath.PathToMe(imagefilename);
   m_up_inactive.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\up_active.png";
   imagepath.PathToMe(imagefilename);
   m_up_active.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\up_hover.png";
   imagepath.PathToMe(imagefilename);
   m_up_hover.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\down_inactive.png";
   imagepath.PathToMe(imagefilename);
   m_down_inactive.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\down_active.png";
   imagepath.PathToMe(imagefilename);
   m_down_active.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\down_hover.png";
   imagepath.PathToMe(imagefilename);
   m_down_hover.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\thumb_inactive.png";
   imagepath.PathToMe(imagefilename);
   m_thumb_inactive.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\thumb_active.png";
   imagepath.PathToMe(imagefilename);
   m_thumb_active.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\thumb_hover.png";
   imagepath.PathToMe(imagefilename);
   m_thumb_hover.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\thumb_top_inactive.png";
   imagepath.PathToMe(imagefilename);
   m_thumb_top_inactive.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\thumb_top_active.png";
   imagepath.PathToMe(imagefilename);
   m_thumb_top_active.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\thumb_top_hover.png";
   imagepath.PathToMe(imagefilename);
   m_thumb_top_hover.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\thumb_bottom_inactive.png";
   imagepath.PathToMe(imagefilename);
   m_thumb_bottom_inactive.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\thumb_bottom_active.png";
   imagepath.PathToMe(imagefilename);
   m_thumb_bottom_active.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\thumb_bottom_hover.png";
   imagepath.PathToMe(imagefilename);
   m_thumb_bottom_hover.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\thumb_grip_inactive.png";
   imagepath.PathToMe(imagefilename);
   m_thumb_grip_inactive.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\thumb_grip_active.png";
   imagepath.PathToMe(imagefilename);
   m_thumb_grip_active.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\thumb_grip_hover.png";
   imagepath.PathToMe(imagefilename);
   m_thumb_grip_hover.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\track.png";
   imagepath.PathToMe(imagefilename);
   m_track.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\ontopic.png";
   imagepath.PathToMe(imagefilename);
   m_ontopic.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\ontopic_hover.png";
   imagepath.PathToMe(imagefilename);
   m_ontopic_hover.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\inf.png";
   imagepath.PathToMe(imagefilename);
   m_inf.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\inf_hover.png";
   imagepath.PathToMe(imagefilename);
   m_inf_hover.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\inf_star.png";
   imagepath.PathToMe(imagefilename);
   m_infstar.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\nws.png";
   imagepath.PathToMe(imagefilename);
   m_nws.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\nws_hover.png";
   imagepath.PathToMe(imagefilename);
   m_nws_hover.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\nws_star.png";
   imagepath.PathToMe(imagefilename);
   m_nwsstar.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\stupid.png";
   imagepath.PathToMe(imagefilename);
   m_stupid.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\stupid_hover.png";
   imagepath.PathToMe(imagefilename);
   m_stupid_hover.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\stupid_star.png";
   imagepath.PathToMe(imagefilename);
   m_stupidstar.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\offtopic.png";
   imagepath.PathToMe(imagefilename);
   m_offtopic.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\offtopic_hover.png";
   imagepath.PathToMe(imagefilename);
   m_offtopic_hover.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\offtopic_star.png";
   imagepath.PathToMe(imagefilename);
   m_offtopicstar.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\political.png";
   imagepath.PathToMe(imagefilename);
   m_political.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\political_hover.png";
   imagepath.PathToMe(imagefilename);
   m_political_hover.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\political_star.png";
   imagepath.PathToMe(imagefilename);
   m_politicalstar.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\nuked.png";
   imagepath.PathToMe(imagefilename);
   m_nuked.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\nuked_hover.png";
   imagepath.PathToMe(imagefilename);
   m_nuked_hover.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\close.png";
   imagepath.PathToMe(imagefilename);
   m_close.ReadPNG(imagepath);
   
   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\close_hover.png";
   imagepath.PathToMe(imagefilename);
   m_close_hover.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\post.png";
   imagepath.PathToMe(imagefilename);
   m_post.ReadPNG(imagepath);
   
   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\post_hover.png";
   imagepath.PathToMe(imagefilename);
   m_post_hover.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\send.png";
   imagepath.PathToMe(imagefilename);
   m_send.ReadPNG(imagepath);
   
   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\send_hover.png";
   imagepath.PathToMe(imagefilename);
   m_send_hover.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\newthread.png";
   imagepath.PathToMe(imagefilename);
   m_newthread.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\newthread_hover.png";
   imagepath.PathToMe(imagefilename);
   m_newthread_hover.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\preview.png";
   imagepath.PathToMe(imagefilename);
   m_preview.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\preview_hover.png";
   imagepath.PathToMe(imagefilename);
   m_preview_hover.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\docktab.png";
   imagepath.PathToMe(imagefilename);
   m_docktabimage.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\background.png";
   imagepath.PathToMe(imagefilename);
   m_background.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\post_background.png";
   imagepath.PathToMe(imagefilename);
   m_post_background.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\rootpost_background.png";
   imagepath.PathToMe(imagefilename);
   m_rootpost_background.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\reply_background.png";
   imagepath.PathToMe(imagefilename);
   m_reply_background.ReadPNG(imagepath);

   // refresh anim
   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\refresh_anim_0.png";
   imagepath.PathToMe(imagefilename);
   m_refresh_anim[0].ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\refresh_anim_1.png";
   imagepath.PathToMe(imagefilename);
   m_refresh_anim[1].ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\refresh_anim_2.png";
   imagepath.PathToMe(imagefilename);
   m_refresh_anim[2].ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\refresh_anim_3.png";
   imagepath.PathToMe(imagefilename);
   m_refresh_anim[3].ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\refresh_anim_4.png";
   imagepath.PathToMe(imagefilename);
   m_refresh_anim[4].ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"refresh_anim_5.png";
   imagepath.PathToMe(imagefilename);
   m_refresh_anim[5].ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\refresh_anim_6.png";
   imagepath.PathToMe(imagefilename);
   m_refresh_anim[6].ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\refresh_anim_7.png";
   imagepath.PathToMe(imagefilename);
   m_refresh_anim[7].ReadPNG(imagepath);

   // pin
   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\pin_off.png";
   imagepath.PathToMe(imagefilename);
   m_pin_off.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\pin_on.png";
   imagepath.PathToMe(imagefilename);
   m_pin_on.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\pin_hover.png";
   imagepath.PathToMe(imagefilename);
   m_pin_hover.ReadPNG(imagepath);

   // refresh story
   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\refresh_story.png";
   imagepath.PathToMe(imagefilename);
   m_refresh_story.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\refresh_story_hover.png";
   imagepath.PathToMe(imagefilename);
   m_refresh_story_hover.ReadPNG(imagepath);

   // new msg stuff
   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\compose.png";
   imagepath.PathToMe(imagefilename);
   m_compose.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\compose_hover.png";
   imagepath.PathToMe(imagefilename);
   m_compose_hover.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\forward.png";
   imagepath.PathToMe(imagefilename);
   m_forward.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\forward_hover.png";
   imagepath.PathToMe(imagefilename);
   m_forward_hover.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\delete.png";
   imagepath.PathToMe(imagefilename);
   m_delete.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\delete_hover.png";
   imagepath.PathToMe(imagefilename);
   m_delete_hover.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\mb_pan.png";
   imagepath.PathToMe(imagefilename);
   m_mb_pan.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\search.png";
   imagepath.PathToMe(imagefilename);
   m_search.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\search_hover.png";
   imagepath.PathToMe(imagefilename);
   m_search_hover.ReadPNG(imagepath);

   m_tempimage.Resize(152,130);
   m_tempimage.Fill(GetRValue(m_spoiler_color),GetGValue(m_spoiler_color),GetBValue(m_spoiler_color));
   RECT rect;
   rect.top = rect.left = 0;
   rect.right = 152;
   rect.bottom = 130;
   ::SetTextColor(m_tempimage.GetDC(),theApp.GetPostTextColor());
   ::SetBkMode(m_tempimage.GetDC(),TRANSPARENT);
   ::DrawText(m_tempimage.GetDC(),L"Loading...",10,&rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

void CLampApp::SetStatusBarText(const UCString &text, CLampView *pView)
{
   if(m_pMainWnd != NULL)
   {
      CMFCStatusBar *pStatusBar = ((CMainFrame*)m_pMainWnd)->GetStatusBar();
      if(pStatusBar != NULL)
      {
         if(pView != NULL &&
            pView->GetDocument()->IsBusy())
         {
            UCString temp = L"(loading)";
            temp += text;
            pStatusBar->SetPaneText(0,(LPCTSTR)temp.Str());
         }
         else
         {
            pStatusBar->SetPaneText(0,(LPCTSTR)text.Str());
         }
      }
	}
}


void CLampApp::LaunchLinkInDefaultBrowser(const UCChar *link, bool NWS/* = false*/)
{
   if(_wcsnicmp(link,L"forceout:",9) == 0)
   {
      link += 9;
   }

   UCString verb(L"open");
   UCString file, parms;
   if(NWS)
   {
      switch(m_defaultbrowser_nws)
      {
      case BT_SYSTEM:           file = link; break;
      case BT_CHROME:           file = L"chrome.exe"; parms = link; break;
      case BT_CHROME_INCOGNITO: file = L"chrome.exe"; parms = L"--incognito "; parms += link; break;
      case BT_FIREFOX:          file = L"firefox.exe"; parms = link; break;
      case BT_IE:               file = L"iexplore.exe"; parms = link; break;
      case BT_IE_PRIVATE:       file = L"iexplore.exe"; parms = L"-private "; parms += link; break;
      case BT_SAFARI:           file = L"safari.exe"; parms = link; break;
      default:                  file = link; break;
      }
   }
   else
   {
      switch(m_defaultbrowser)
      {
      case BT_SYSTEM:           file = link; break;
      case BT_CHROME:           file = L"chrome.exe"; parms = link; break;
      case BT_CHROME_INCOGNITO: file = L"chrome.exe"; parms = L"--incognito "; parms += link; break;
      case BT_FIREFOX:          file = L"firefox.exe"; parms = link; break;
      case BT_IE:               file = L"iexplore.exe"; parms = link; break;
      case BT_IE_PRIVATE:       file = L"iexplore.exe"; parms = L"-private "; parms += link; break;
      case BT_SAFARI:           file = L"safari.exe"; parms = link; break;
      default:                  file = link; break;
      }
   }

   const UCChar *pparms = NULL;
   if(!parms.IsEmpty())
   {
      pparms = parms;
   }

   HINSTANCE hInst = ShellExecuteW(NULL,
                                   verb,
                                   file,
                                   pparms,
                                   NULL,
                                   SW_SHOW);

   if(((int)hInst == ERROR_FILE_NOT_FOUND ||
       (int)hInst == ERROR_PATH_NOT_FOUND) &&
      file == L"chrome.exe")
   {
      UCChar buffer[MAX_PATH];
      buffer[0] = 0;
      ::GetEnvironmentVariable(L"USERPROFILE",buffer,MAX_PATH);

      if(buffer[0])
      {
         file = buffer;

         if(file.endswith(L"\\") == NULL)
         {
            file += L"\\";
         }

         file += L"AppData\\Local\\Google\\Chrome\\Application\\chrome.exe";
                  
         ShellExecuteW(NULL,
                       verb,
                       file,
                       pparms,
                       NULL,
                       SW_SHOW);
      }
   }
}

void CLampApp::OpenShackLink(const UCString &shackpath, bool NWS /*= false*/)
{
   bool bIsMine = false;

   if(_wcsnicmp(shackpath,L"forceout:",9) == 0 &&
      shackpath.Length() > 9)
   {
      LaunchLinkInDefaultBrowser(shackpath.Str() + 9, NWS);
   }
   else
   {   
      if(_wcsnicmp(shackpath,L"http://www.shacknews.com/",25) == 0)
      {
         const UCChar *work = wcsstr(shackpath,L"?id=");
         if(work != NULL)
         {
            bIsMine = true;
         }
         else
         {
            work = wcsstr(shackpath,L"/chatty/");
            if(work != NULL)
            {
               bIsMine = true;
            }
            else
            {
               work = wcsstr(shackpath,L"/search?");
               if(work != NULL)
               {
                  bIsMine = true;
               }
            }
         }
      }
      else if(_wcsnicmp(shackpath,L"http://chattyprofil.es/p/",25) == 0 &&
              shackpath.Length() > 25)
      {
         bIsMine = true;
      }
      else if(_wcsnicmp(shackpath,L"LOLTHEYWROTE",12) == 0 &&
              shackpath.Length() > 12)
      {
         bIsMine = true;
      }
      else
      {
         // see if it is all digits
         const UCChar *work = shackpath;
         const UCChar *end = work + shackpath.Length();
         bool bIsAllDigits = true;
         while(work < end)
         {
            if(iswdigit(*work) == 0)
            {
               bIsAllDigits = false;
               break;
            }
            work++;
         }
         
         if(bIsAllDigits)
         {
            bIsMine = true;
         }
      }

      if(bIsMine)
      {
         OpenDocumentFile(shackpath);
      }
      else
      {
         // must be an html link
         LaunchLinkInDefaultBrowser(shackpath, NWS);
      }
   }
}

bool CLampApp::HaveLogin()
{
   bool have = false;
   if(!m_username.IsEmpty() &&
      !m_password.IsEmpty())
   {
      have = true;
   }

   if(!have)
   {
      have = Login();
   }

   return have;
}

bool CLampApp::Login()
{
   bool have = false;
   CLoginDlg logindlg(GetMainWnd());
   logindlg.m_username = m_username;
   logindlg.m_password = m_password;
   if(logindlg.DoModal() == IDOK)
   {
      m_username = logindlg.m_username;
      m_password = logindlg.m_password;
      have = true;
   }

   CheckForModMode();

   return have;
}

void CLampApp::CheckForUpdates(bool bManual)
{
   CDownloadData *pDD = new CDownloadData();

   // http://shacklamp.omgninja.com/LampUpdate.xml"

   pDD->m_host = L"shacklamp.omgninja.com";
   pDD->m_path = L"/LampUpdate.xml";
   pDD->m_WhoWants = this;
   pDD->m_dt = DT_CHECK_UPDATE;
   pDD->m_id = 0;
   pDD->m_refreshid = 0;
   pDD->reply_to_id = 0;
   if(bManual)
   {
      pDD->m_postrootid = 1;
   }
   else
   {
      pDD->m_postrootid = 0;
   }

   AfxBeginThread(DownloadThreadProc, pDD);
}


void CLampApp::RefreshLOLs()
{
   CDownloadData *pDD = new CDownloadData();

   // http://lmnopc.com/greasemonkey/shacklol/api.php?special=getcounts

   pDD->m_host = GetLolHostName();
   pDD->m_path = L"/greasemonkey/shacklol/api.php?special=getcounts";
   pDD->m_WhoWants = this;
   pDD->m_dt = DT_REFRESH_LOLS;
   pDD->m_id = 0;
   pDD->m_refreshid = 0;
   pDD->reply_to_id = 0;
   pDD->m_postrootid = 0;

   AfxBeginThread(DownloadThreadProc, pDD);
}

void CLampApp::UpdateInbox()
{
   if(m_auto_check_inbox &&
     !m_username.IsEmpty() &&
     !m_password.IsEmpty())
   {
      CDownloadData *pDD = new CDownloadData();

      if(UseShack())
      {
         pDD->m_host = L"www.shacknews.com";
         pDD->m_path = L"/messages/inbox&page=1";
         pDD->m_WhoWants = this;
         pDD->m_dt = DT_SHACK_SHACKMSG;
         pDD->m_id = 0;
         pDD->m_refreshid = 0;
         pDD->reply_to_id = 0;
         pDD->m_username = GetUsername();
         pDD->m_password = GetPassword();
      }
      else
      {
         pDD->m_host = L"shackapi.stonedonkey.com";
         pDD->m_path = L"/Messages/?box=inbox&page=1";
         pDD->m_WhoWants = this;
         pDD->m_dt = DT_SHACKMSG;
         pDD->m_id = 0;
         pDD->m_refreshid = 0;
         pDD->reply_to_id = 0;
         pDD->m_username = GetUsername();
         pDD->m_password = GetPassword();
      }

      AfxBeginThread(DownloadThreadProc, pDD);
   }
}

void CLampApp::SetShowSmallLOL(bool value)
{
   m_bShowSmallLOL = value;
   int widths[4];
   GetCharWidths(L"www", widths, 3, false, false, ShowSmallLOL(), GetNormalFontName());
   m_LOLFieldWidth = widths[0] + widths[1] + widths[2];

   // have all the tabs update their lol count info
   std::list<CLampDoc*>::iterator it = m_MyDocuments.begin();
   std::list<CLampDoc*>::iterator end = m_MyDocuments.end();

   while(it != end)
   {
      (*it)->UpdateLOLsRecurse();
      it++;
   }
}

void CLampApp::SetShowThomWLOLS(bool value)
{
   m_show_thomw_lols = value;

   // have all the tabs update their lol diplay info
   std::list<CLampDoc*>::iterator it = m_MyDocuments.begin();
   std::list<CLampDoc*>::iterator end = m_MyDocuments.end();

   while(it != end)
   {
      (*it)->UpdateLOLsRecurse();
      it++;
   }
}

void CLampApp::SetVerboseLOLPreviews(bool value)
{
   m_verbose_lol_previews = value;

   // have all the tabs update their lol diplay info
   std::list<CLampDoc*>::iterator it = m_MyDocuments.begin();
   std::list<CLampDoc*>::iterator end = m_MyDocuments.end();

   while(it != end)
   {
      (*it)->UpdateLOLsRecurse();
      it++;
   }
}

void CLampApp::SetInvertedLOLPreviews(bool value)
{
   m_inverted_lol_previews = value;

   // have all the tabs update their lol diplay info
   std::list<CLampDoc*>::iterator it = m_MyDocuments.begin();
   std::list<CLampDoc*>::iterator end = m_MyDocuments.end();

   while(it != end)
   {
      (*it)->UpdateLOLsRecurse();
      it++;
   }
}

void CLampApp::InvalidateSkinAllViews()
{
   m_fontheight = (int)(-13.0f * m_textscaler);
   m_miscfontheight = (int)(-10.0f * m_textscaler);
   m_samplefontheight = (int)(-9.0f * m_textscaler);
   m_textheight = (int)(16.0f * m_textscaler);
   m_cellheight = (int)(20.0f * m_textscaler);
   CalcDescent();

   int widths[4];
   GetCharWidths(L"www", widths, 3, false, false, ShowSmallLOL(), GetNormalFontName());
   m_LOLFieldWidth = widths[0] + widths[1] + widths[2];

   UpdateNewMessages();

   std::list<CLampView*>::iterator it = m_views.begin();
   std::list<CLampView*>::iterator end = m_views.end();

   while(it != end)
   {
      (*it)->InvalidateSkin();
      it++;
   }

   GenerateLightningBolt();
   GenerateFlagActives();
}

void CLampApp::InvalidateFlagsAllViews()
{
   std::list<CLampView*>::iterator it = m_views.begin();
   std::list<CLampView*>::iterator end = m_views.end();

   while(it != end)
   {
      (*it)->InvalidateFlags();
      it++;
   }
}

void CLampApp::ShowNewMessages()
{
   bool bInboxTookIt = false;
   // if the inbox is open, and it is on page 1, update it
   std::list<CLampView*>::iterator it = m_views.begin();
   std::list<CLampView*>::iterator end = m_views.end();

   while(it != end)
   {
      if((*it)->GetDocument()->GetDataType() == DDT_SHACKMSG &&
         (*it)->GetDocument()->GetShackMessageType() == SMT_INBOX)
      {
         if((*it)->GetDocument()->GetPage() > 1)
         {
            (*it)->GetDocument()->SetPage(1);
            (*it)->GetDocument()->Refresh();
         }
         
         CMainFrame *pMainFrame = (CMainFrame*)GetMainWnd();
         if(pMainFrame)
         {
            CHackedTabCtrl *tabctrl = (CHackedTabCtrl*)pMainFrame->GetCA()->FindActiveTabWnd();
            if(tabctrl != NULL)
            {
               int numtabs = tabctrl->GetTabsNum();
               for(int i=0; i < numtabs; i++)
               {
                  CChildFrame *pFrame = (CChildFrame*)tabctrl->GetTabWnd(i);
                  if(pFrame != NULL)
                  {
                     CLampView *pView = pFrame->GetView();
                     if(pView != NULL &&
                        pView == (*it))
                     {
                        tabctrl->SetActiveTab(i);
                        break;
                     }
                  }
               }
            }
         }

         bInboxTookIt = true;
      }
      it++;
   }

   if(!bInboxTookIt)
   {
      OpenDocumentFile(L"SHACKMSG_INBOX");
   }
}

void CLampApp::UpdateNewMessages()
{
   UCString num = m_unreadmessagecount;

   if(num.Length() < 4)
   {
      m_pNewMessages = m_new_messages_text + (3 - num.Length());

      UCChar *pWrite = (UCChar *)m_pNewMessages;
      const UCChar *pRead = num.Str();
      const UCChar *pEnd = pRead + num.Length();

      while(pRead < pEnd)
      {
         *pWrite = *pRead;
         pRead++;
         pWrite++;
      }

      m_new_messages_charcount = 16 - (3 - num.Length());
      if(m_unreadmessagecount == 1)
         m_new_messages_charcount--;

      GetCharWidths(m_pNewMessages, m_new_messages_widths, m_new_messages_charcount, false, false, false, GetNormalFontName());

      m_new_messages_textwidth = 0;
      for(size_t i = 0; i < m_new_messages_charcount; i++)
      {
         m_new_messages_textwidth += m_new_messages_widths[i];
      }
   }
}

// CLampApp message handlers


void CLampApp::OnFileOpenthread()
{
   COpenThreadDlg openDlg(GetMainWnd());
	if(openDlg.DoModal() == IDOK)
   {
      OpenShackLink(openDlg.m_path);
   }
}

void CLampApp::OnUpdateFileOpenthread(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampApp::OnFileOpen()
{
   OnFileOpenthread();
}
void CLampApp::OnFileSetuplogininfo()
{
   CLoginDlg logindlg(GetMainWnd());
   logindlg.m_username = m_username;
   logindlg.m_password = m_password;
   if(logindlg.DoModal() == IDOK)
   {
      m_username = logindlg.m_username;
      m_password = logindlg.m_password;
   }

   CheckForModMode();
}

void CLampApp::OnUpdateFileSetuplogininfo(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampApp::OnViewSmoothscroll()
{
   m_smooth_scroll = !m_smooth_scroll;
}

void CLampApp::OnUpdateViewSmoothscroll(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(m_smooth_scroll)
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}


bool CLampApp::IsSpelledCorrectly(const UCChar *wordtotest, 
                                  size_t count)
{
   bool result = true;
   if(m_pHunspell != NULL)
   {
      result = false;
      size_t sizeneeded = os_utf16_to_other(NULL, wordtotest, count, 0, false, CET_UTF8);            

      char *wordtotest8 = (char*)malloc(sizeneeded + 1);
      memset(wordtotest8, 0, sizeneeded + 1);

      os_utf16_to_other( wordtotest8, wordtotest, count, sizeneeded, false, CET_UTF8);

      if(m_pHunspell->spell(wordtotest8))
      {
         result = true;
      }

      free(wordtotest8);
   }
   return result;
}

void CLampApp::GetSpellSuggestions(const UCChar *wordtotest, 
                                   size_t count, 
                                   std::vector<CSuggestion> &suggestions)
{
   suggestions.clear();

   if(m_pHunspell != NULL)
   {
      size_t sizeneeded = os_utf16_to_other(NULL, wordtotest, count, 0, false, CET_UTF8);            

      char *wordtotest8 = (char*)malloc(sizeneeded + 1);
      memset(wordtotest8, 0, sizeneeded + 1);

      os_utf16_to_other( wordtotest8, wordtotest, count, sizeneeded, false, CET_UTF8);

      char **wlsti = NULL;
      int numsuggestions = m_pHunspell->suggest(&wlsti, wordtotest8);

      for(int i = 0; i < numsuggestions; i++)
      {
         CSuggestion temp;
         temp.display = wlsti[i];
         temp.value = wlsti[i];
         suggestions.push_back(temp);
      }

      m_pHunspell->free_list(&wlsti, numsuggestions);

      free(wordtotest8);
   }
}

void CLampApp::AddMyLol(unsigned int post_id, loltagtype tag)
{
   bool bFound = false;

   std::list<CMyLol>::iterator it = m_mylols.begin();
   std::list<CMyLol>::iterator end = m_mylols.end();

   while(it != end)
   {
      if((*it).m_post_id == post_id)
      {
         (*it).m_mylols |= tag;
         bFound = true;
         break;
      }

      it++;
   }

   if(!bFound)
   {
      // add it
      CMyLol mylol;
      mylol.m_post_id = post_id;
      mylol.m_mylols = tag;
      m_mylols.push_back(mylol);

      while(m_mylols.size() > 100)
      {
         m_mylols.pop_front();
      }
   }

   CLOLFlags &flags = GetKnownLOLFlags(post_id);

   switch(tag)
   {
   case LTT_LOL: SetLOL_LOL(post_id, flags.m_LOLd + 1); break;
   case LTT_INF: SetLOL_INF(post_id, flags.m_INFd + 1); break;
   case LTT_UNF: SetLOL_UNF(post_id, flags.m_UNFd + 1); break;
   case LTT_TAG: SetLOL_TAG(post_id, flags.m_TAGd + 1); break;
   case LTT_WTF: SetLOL_WTF(post_id, flags.m_WTFd + 1); break;
   case LTT_UGH: SetLOL_UGH(post_id, flags.m_UGHd + 1); break;
   }
}

void CLampApp::RemoveMyLol(unsigned int post_id, loltagtype tag)
{
   bool bFound = false;

   std::list<CMyLol>::iterator it = m_mylols.begin();
   std::list<CMyLol>::iterator end = m_mylols.end();

   while(it != end)
   {
      if((*it).m_post_id == post_id &&
         (*it).m_mylols & tag)
      {
         (*it).m_mylols ^= tag;
         bFound = true;
         break;
      }

      it++;
   }

   if(bFound)
   {      

      CLOLFlags &flags = GetKnownLOLFlags(post_id);

      switch(tag)
      {
      case LTT_LOL: SetLOL_LOL(post_id, flags.m_LOLd - 1); break;
      case LTT_INF: SetLOL_INF(post_id, flags.m_INFd - 1); break;
      case LTT_UNF: SetLOL_UNF(post_id, flags.m_UNFd - 1); break;
      case LTT_TAG: SetLOL_TAG(post_id, flags.m_TAGd - 1); break;
      case LTT_WTF: SetLOL_WTF(post_id, flags.m_WTFd - 1); break;
      case LTT_UGH: SetLOL_UGH(post_id, flags.m_UGHd - 1); break;
      }
   }
}

byte CLampApp::GetMyLol(unsigned int post_id)
{
   std::list<CMyLol>::iterator it = m_mylols.begin();
   std::list<CMyLol>::iterator end = m_mylols.end();

   while(it != end)
   {
      if((*it).m_post_id == post_id)
      {
         return (*it).m_mylols;
      }
      it++;
   }

   return 0;
}

void CLampApp::AddMyCollapse(unsigned int post_id)
{
   std::list<CollapsedThread>::iterator it = m_mycollapses.begin();
   std::list<CollapsedThread>::iterator end = m_mycollapses.end();

   while(it != end)
   {
      if((*it).m_thread_id == post_id)
      {
         return;
      }

      it++;
   }

   // add it
   m_mycollapses.push_back(CollapsedThread(post_id,::GetTickCount()));

   WriteSettingsFile();
}

void CLampApp::RemoveMyCollapse(unsigned int post_id)
{
   std::list<CollapsedThread>::iterator it = m_mycollapses.begin();
   std::list<CollapsedThread>::iterator end = m_mycollapses.end();

   while(it != end)
   {
      if((*it).m_thread_id == post_id)
      {
         // we do have it, so remove it
         m_mycollapses.erase(it);
         return;
      }

      it++;
   }
}

bool CLampApp::GetMyCollapse(unsigned int post_id)
{
   std::list<CollapsedThread>::iterator it = m_mycollapses.begin();
   std::list<CollapsedThread>::iterator end = m_mycollapses.end();

   while(it != end)
   {
      if((*it).m_thread_id == post_id)
      {
         return true;
      }
      it++;
   }

   return false;
}

void CLampApp::OnAlwaysOnTopWhenNotDocked()
{
   RECT framerect;
   ((CMainFrame*)GetMainWnd())->GetWindowRect(&framerect);

   const CWnd *pWndInsertAfter = &CWnd::wndNoTopMost;

   m_bAlwaysOnTopWhenNotDocked = !m_bAlwaysOnTopWhenNotDocked;

   if(m_bAlwaysOnTopWhenNotDocked)
   {
      pWndInsertAfter = &CWnd::wndTopMost;
   }

   GetMainWnd()->SetWindowPos(pWndInsertAfter, framerect.left, framerect.top, framerect.right - framerect.left, framerect.bottom - framerect.top, 0);   
}

void CLampApp::OnUpdateAlwaysOnTopWhenNotDocked(CCmdUI *pCmdUI)
{
   if(m_bDockedMode)
   {
      pCmdUI->Enable(FALSE);
   }
   else
   {
      pCmdUI->Enable(TRUE);
   }

   if(m_bAlwaysOnTopWhenNotDocked)
   {
      pCmdUI->SetCheck(1);
   }
   else
   {
      pCmdUI->SetCheck(0);
   }
}

void CLampApp::OnViewDocktop()
{
   if(m_bDockedMode)
   {
      m_bDockedMode = false;
      m_bCollapsed = false;
      m_bWasInDockedMode = false;
      //GetMainWnd()->ModifyStyle(0, WS_SIZEBOX);
      //GetMainWnd()->SetWindowPos(&CWnd::wndBottom, m_restoredrect.left, m_restoredrect.top, m_restoredrect.right - m_restoredrect.left, m_restoredrect.bottom - m_restoredrect.top, 0);
      //GetMainWnd()->SetWindowPos(&CWnd::wndTop, m_restoredrect.left, m_restoredrect.top, m_restoredrect.right - m_restoredrect.left, m_restoredrect.bottom - m_restoredrect.top, 0);

      RECT framerect;
      ((CMainFrame*)GetMainWnd())->GetWindowRect(&framerect);

      const CWnd *pWndInsertAfter = &CWnd::wndNoTopMost;

      if(m_bAlwaysOnTopWhenNotDocked)
      {
         pWndInsertAfter = &CWnd::wndTopMost;
      }

      GetMainWnd()->SetWindowPos(pWndInsertAfter, framerect.left, framerect.top, framerect.right - framerect.left, framerect.bottom - framerect.top, SWP_NOMOVE|SWP_NOSIZE);   

      GetMainWnd()->ModifyStyle(0,WS_MAXIMIZEBOX|WS_MINIMIZEBOX);

      if(m_pDockTab != NULL)
      {
         delete m_pDockTab;
         m_pDockTab = NULL;
      }
   }
   else
   {
      m_bDockedMode = true;
      m_bCollapsed = false;
      m_bWasInDockedMode = true;

      RECT framerect;
      ((CMainFrame*)GetMainWnd())->GetWindowRect(&framerect);

      RECT menurect;
      ((CMainFrame*)GetMainWnd())->GetMenuBar()->GetWindowRect(&menurect);

      m_dockedrect.left = framerect.left;
      m_dockedrect.right = framerect.right;
      m_dockedrect.top = framerect.top - menurect.top; // about -30;
      m_dockedrect.bottom = m_dockedrect.top + (framerect.bottom - framerect.top);

      m_restoredrect = framerect;

      //GetMainWnd()->ModifyStyle(WS_SIZEBOX, 0);

      //GetMainWnd()->SetWindowPos(&CWnd::wndTopMost, m_dockedrect.left, m_dockedrect.top, m_dockedrect.right - m_dockedrect.left, m_dockedrect.bottom - m_dockedrect.top, 0);   
      GetMainWnd()->SetWindowPos(&CWnd::wndTopMost, framerect.left, framerect.top, framerect.right - framerect.left, framerect.bottom - framerect.top, 0);   

      GetMainWnd()->ModifyStyle(WS_MAXIMIZEBOX|WS_MINIMIZEBOX,0);

      m_pDockTab = new DockTab((CMainFrame*)GetMainWnd());
      m_pDockTab->m_pFrameWnd = (CMainFrame*)GetMainWnd();

      m_pDockTab->Create( IDD_DOCKTAB, (CMainFrame*)GetMainWnd());

      RECT docktabrect;
      docktabrect.left = ((m_dockedrect.right + m_dockedrect.left) / 2) - (m_docktabimage.GetWidth() / 2);
      docktabrect.right = docktabrect.left + m_docktabimage.GetWidth();
      docktabrect.top = 0;
      docktabrect.bottom = m_docktabimage.GetHeight();
      m_pDockTab->MoveWindow(&docktabrect);

      HWND hWnd = GetMainWnd()->m_hWnd;
      //relation time of SetForegroundWindow lock	
      DWORD lockTimeOut = 0;	
      HWND  hCurrWnd = ::GetForegroundWindow();	
      DWORD dwThisTID = ::GetCurrentThreadId(),	      
            dwCurrTID = ::GetWindowThreadProcessId(hCurrWnd,0);	
      
      //we need to bypass some limitations from Microsoft :)	
      if(dwThisTID != dwCurrTID)	
      {		
         ::AttachThreadInput(dwThisTID, dwCurrTID, TRUE);
         ::SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT,0,&lockTimeOut,0);		
         ::SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT,0,0,SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE);		
         ::AllowSetForegroundWindow(ASFW_ANY);	
      }	
      
      ::SetForegroundWindow(hWnd);	
      ::SetFocus(hWnd);
      
      if(dwThisTID != dwCurrTID)	
      {		
         ::SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT,0,(PVOID)lockTimeOut,SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE);		
         ::AttachThreadInput(dwThisTID, dwCurrTID, FALSE);	
      }
   }
}

void CLampApp::OnUpdateViewDocktop(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(m_bDockedMode)
   {
      pCmdUI->SetCheck(1);
   }
   else
   {
      pCmdUI->SetCheck(0);
   }
}

void CLampApp::OnLOLLOL()
{
   OpenDocumentFile(L"LOLPOPLOL");
}

void CLampApp::OnUpdateLOLLOL(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampApp::OnLOLINF()
{
   OpenDocumentFile(L"LOLPOPINF");
}

void CLampApp::OnUpdateLOLINF(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampApp::OnLOLUNF()
{
   OpenDocumentFile(L"LOLPOPUNF");
}

void CLampApp::OnUpdateLOLUNF(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampApp::OnLOLTAG()
{
   OpenDocumentFile(L"LOLPOPTAG");
}

void CLampApp::OnUpdateLOLTAG(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampApp::OnLOLWTF()
{
   OpenDocumentFile(L"LOLPOPWTF");
}

void CLampApp::OnUpdateLOLWTF(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampApp::OnThingsILold()
{
   OpenDocumentFile(L"LOLILOLD");
}

void CLampApp::OnUpdateThingsILold(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampApp::OnThingsITagd()
{
   OpenDocumentFile(L"LOLITAGD");
}

void CLampApp::OnUpdateThingsITagd(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampApp::OnThingsIWrote()
{
   OpenDocumentFile(L"LOLIWROTE");
}

void CLampApp::OnUpdateThingsIWrote(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampApp::OnShackMsg_Inbox()
{
   OpenDocumentFile(L"SHACKMSG_INBOX");
}

void CLampApp::OnUpdateShackMsg_Inbox(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampApp::OnShackMsg_Outbox()
{
   OpenDocumentFile(L"SHACKMSG_OUTBOX");
}

void CLampApp::OnUpdateShackMsg_Outbox(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampApp::OnShackMsg_Archive()
{
   OpenDocumentFile(L"SHACKMSG_ARCHIVE");
}

void CLampApp::OnUpdateShackMsg_Archive(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampApp::OnGotoLol()
{
   // http://lmnopc.com/greasemonkey/shacklol/?user=[username]

   UCString path = L"http://";
   path += GetLolHostName();
   path += L"/greasemonkey/shacklol/"; 
   if(!m_username.IsEmpty())
   {
      path += L"?user=";
      char *enc_username = url_encode((char*)m_username.str8(false,CET_UTF8));
      path += enc_username;
      free(enc_username);
   }
   OpenShackLink(path);
}

void CLampApp::OnUpdateGotoLol(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampApp::OnMyComments()
{
   OpenDocumentFile(L"MYCOMMENTS");
}

void CLampApp::OnUpdateMyComments(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampApp::OnVanitySearch()
{
   OpenDocumentFile(L"VANITYSEARCH");
}

void CLampApp::OnUpdateVanitySearch(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampApp::OnRepliesToMe()
{
   OpenDocumentFile(L"REPLIESTOME");
}

void CLampApp::OnUpdateRepliesToMe(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

void CLampApp::OnShackSearch()
{
   CCustomSearchDlg csdlg(GetMainWnd());
   csdlg.m_user = m_last_search_author;
   csdlg.m_parent = m_last_search_parent_author;
   csdlg.m_terms = m_last_search_terms;
   csdlg.m_filter = m_last_search_filter;
   csdlg.m_sort = m_last_search_sort;

   if(csdlg.DoModal() == IDOK)
   {
      m_last_search_author = csdlg.m_user;
      m_last_search_parent_author = csdlg.m_parent;
      m_last_search_terms = csdlg.m_terms;
      m_last_search_filter = csdlg.m_filter;
      m_last_search_sort = csdlg.m_sort;

      UCString path = L"CUSTOMSEARCH:";
      char *enc = url_encode(m_last_search_author.str8());
      path += enc;
      free(enc);
      path += L":";
      enc = url_encode(m_last_search_parent_author.str8());
      path += enc;
      free(enc);
      path += L":";
      enc = url_encode(m_last_search_terms.str8());
      path += enc;
      free(enc);
      path += L":";
      enc = url_encode(m_last_search_filter.str8());
      path += enc;
      free(enc);
      path += L":";
      enc = url_encode(m_last_search_sort.str8());
      path += enc;
      free(enc);

      OpenDocumentFile(path);
   }
}

void CLampApp::OnUpdateShackSearch(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);
}

bool CLampApp::HasLinkedImage(const UCString &link, unsigned int &index)
{
   bool result = false;
   // split the link into host and path
   const UCChar *begin = link;
   const UCChar *end = begin + link.Length();
   const UCChar *work = begin;
   
   if(wcsncmp(work, L"http://",7) == 0)
   {
      work += 7;
      begin = work;
   }

   while(work < end && *work != L'/')work++;

   UCString host;
   host.AppendUnicodeString(begin,work - begin);

   UCString path = work;

   // see if we already have it
   
   std::map<unsigned int,CImageCacheItem>::iterator it = m_imagecache.begin();
   std::map<unsigned int,CImageCacheItem>::iterator iend = m_imagecache.end();

   while(it != iend)
   {
      if(it->second.m_host == host &&
         it->second.m_path == path)
      {
         index = it->first;
         result = true;
         break;
      }
      it++;
   }

   return result;
}

CDCSurface *CLampApp::GetLinkedImage(const UCString &link, unsigned int &index)
{
   index = 0xFFFFFFFF;
   CDCSurface *result = NULL;
   // split the link into host and path
   const UCChar *begin = link;
   const UCChar *end = begin + link.Length();
   const UCChar *work = begin;
   
   if(wcsncmp(work, L"http://",7) == 0)
   {
      work += 7;
      begin = work;
   }

   while(work < end && *work != L'/')work++;

   UCString host;
   host.AppendUnicodeString(begin,work - begin);

   UCString path = work;

   // see if we already have it
   
   std::map<unsigned int,CImageCacheItem>::iterator it = m_imagecache.begin();
   std::map<unsigned int,CImageCacheItem>::iterator iend = m_imagecache.end();

   while(it != iend)
   {
      if(it->second.m_host == host &&
         it->second.m_path == path)
      {
         index = it->first;
         break;
      }
      it++;
   }
      

   if(index == 0xFFFFFFFF)
   {
      const UCChar *ext = end;
      while(ext > begin && *ext != L'.') ext--;

      // create it
      CImageCacheItem ici;
      ici.m_host = host;
      ici.m_path = path;
      ici.m_ext = ext;

      m_imagecache[m_nextimagecacheindex] = ici;
      index = m_nextimagecacheindex;
      m_nextimagecacheindex++;
   }

   result = m_imagecache[index].GetImage();

   if(result == NULL)
   {
      result = theApp.GetTempImage();
   }

   return result;
}

CDCSurface *CLampApp::GetLinkedImage(unsigned int index)
{
   CDCSurface *result = NULL;

   if(IsValidImageIndex(index))
   {
      result = m_imagecache[index].GetImage();
   }

   if(result == NULL)
   {
      result = theApp.GetTempImage();
   }

   return result;
}

void CLampApp::MakeThumb(unsigned int index)
{
   if(IsValidImageIndex(index))
   {
      m_imagecache[index].MakeThumb();
   }
}

CDCSurface *CLampApp::GetLinkedImageThumb(const UCString &link, unsigned int &index)
{
   index = 0xFFFFFFFF;
   CDCSurface *result = NULL;
   // split the link into host and path
   const UCChar *begin = link;
   const UCChar *end = begin + link.Length();
   const UCChar *work = begin;
   
   if(wcsncmp(work, L"http://",7) == 0)
   {
      work += 7;
      begin = work;
   }

   while(work < end && *work != L'/')work++;

   UCString host;
   host.AppendUnicodeString(begin,work - begin);

   UCString path = work;

   // see if we already have it
   
   std::map<unsigned int,CImageCacheItem>::iterator it = m_imagecache.begin();
   std::map<unsigned int,CImageCacheItem>::iterator iend = m_imagecache.end();

   while(it != iend)
   {
      if(it->second.m_host == host &&
         it->second.m_path == path)
      {
         index = it->first;
         break;
      }
      it++;
   }
      

   if(index == 0xFFFFFFFF)
   {
      const UCChar *ext = end;
      while(ext > begin && *ext != L'.') ext--;

      // create it
      CImageCacheItem ici;
      ici.m_host = host;
      ici.m_path = path;
      ici.m_ext = ext;

      m_imagecache[m_nextimagecacheindex] = ici;
      index = m_nextimagecacheindex;
      m_nextimagecacheindex++;
   }

   result = m_imagecache[index].GetImageThumb();

   if(result == NULL)
   {
      result = theApp.GetTempImage();
   }

   return result;
}

CDCSurface *CLampApp::GetLinkedImageThumb(unsigned int index)
{
   CDCSurface *result = NULL;

   if(IsValidImageIndex(index))
   {
      result = m_imagecache[index].GetImageThumb();
   }

   if(result == NULL)
   {
      result = theApp.GetTempImage();
   }

   return result;
}

bool CLampApp::IsImageLoaded(unsigned int index)
{
   bool result = false;
   
   if(IsValidImageIndex(index))
   {
      CDCSurface *pimage = m_imagecache[index].GetImage();
      if(pimage != NULL &&
         pimage->GetDC() != NULL &&
         pimage->GetWidth() > 0 &&
         pimage->GetHeight() > 0)
      {
         result = true;
      }
   }

   return result;
}

bool CLampApp::IsThumbLoaded(unsigned int index)
{
   bool result = false;
   
   if(IsValidImageIndex(index))
   {
      CDCSurface *pimage = m_imagecache[index].GetImageThumb();
      if(pimage != NULL &&
         pimage->GetDC() != NULL &&
         pimage->GetWidth() > 0 &&
         pimage->GetHeight() > 0)
      {
         result = true;
      }

      if(result == false)
      {
         pimage = m_imagecache[index].GetImage();
         if(pimage != NULL &&
            pimage->GetDC() != NULL &&
            pimage->GetWidth() > 0 &&
            pimage->GetHeight() > 0)
         {
            m_imagecache[index].MakeThumb();

            pimage = m_imagecache[index].GetImageThumb();
            if(pimage != NULL &&
               pimage->GetDC() != NULL &&
               pimage->GetWidth() > 0 &&
               pimage->GetHeight() > 0)
            {
               result = true;
            }
         }
      }
   }

   return result;
}

void CLampApp::LoadImage(unsigned int index, unsigned int postid)
{
   if(IsValidImageIndex(index))
   {
      CImageCacheItem &ici = m_imagecache[index];

      ici.AddNotify(postid);

      UCString temphost = ici.m_host;
      UCString temppath = ici.m_path;

      if(ici.m_host == L"www.shackpics.com" ||
         ici.m_host == L"shackpics.com")
      {
         // http://www.shackpics.com/viewer.x?file=pic.jpg
         // http://www.shackpics.com/files/pic.jpg
         temppath.Replace(L"viewer.x?file=",L"files/");
         temphost = L"chattypics.com";
      }

      if(ici.m_host == L"www.chattypics.com" ||
         ici.m_host == L"chattypics.com")
      {
         // http://www.shackpics.com/viewer.x?file=pic.jpg
         // http://www.shackpics.com/files/pic.jpg
         temppath.Replace(L"viewer.php?file=",L"files/");
         temphost = L"chattypics.com";
      }

      if(ici.m_host == L"www.fukung.net" ||
         ici.m_host == L"fukung.net")
      {
         // http://www.fukung.net/v/32798/b3194b0e9a4e7f629059c0ff55d91bff.jpg
         // http://media.fukung.net/images/32798/b3194b0e9a4e7f629059c0ff55d91bff.jpg
         temppath.Replace(L"/v/",L"/images/");
         temphost = L"media.fukung.net";
      }

      if(!temppath.IsEmpty())
      {
         const char *str8 = temppath.str8();
         char *altstr8 = url_decode(str8);
         temppath = altstr8;
         free(altstr8);
      }


      CDownloadData *pDD = new CDownloadData();

      pDD->m_host = temphost;
      pDD->m_path = temppath;
      pDD->m_WhoWants = this;
      pDD->m_dt = DT_GET_IMAGE;
      pDD->m_id = index;
      pDD->m_refreshid = 0;
      pDD->reply_to_id = 0;
      pDD->m_postrootid = 0;

      AfxBeginThread(DownloadThreadProc, pDD);
   }
}


void CLampApp::LoadImageThumb(unsigned int index, unsigned int postid)
{
   if(IsValidImageIndex(index))
   {
      CImageCacheItem &ici = m_imagecache[index];

      UCString temphost = ici.m_host;
      UCString temppath = ici.m_path;

      if(ici.m_host == L"www.chattypics.com" ||
         ici.m_host == L"chattypics.com")
      {
         // http://chattypics.com/thumber.php?file=PhotoAug0474517AM_uaxvbytxa9.jpeg
         temppath.Replace(L"viewer.php?file=",L"thumber.php?file=");
         temppath.Replace(L"/files/",L"/thumber.php?file=");
         temphost = L"chattypics.com";

         ici.AddNotify(postid);

         if(!temppath.IsEmpty())
         {
            const char *str8 = temppath.str8();
            char *altstr8 = url_decode(str8);
            temppath = altstr8;
            free(altstr8);
         }

         CDownloadData *pDD = new CDownloadData();

         pDD->m_host = temphost;
         pDD->m_path = temppath;
         pDD->m_WhoWants = this;
         pDD->m_dt = DT_GET_THUMB;
         pDD->m_id = index;
         pDD->m_refreshid = 0;
         pDD->reply_to_id = 0;
         pDD->m_postrootid = 0;

         AfxBeginThread(DownloadThreadProc, pDD);
      }
   }
}


CDCSurface *CImageCacheItem::GetImage()
{
   m_time = ::GetTickCount();

   if(m_image.GetWidth() == 0 ||
      m_image.GetHeight() == 0 ||
      m_image.GetDC() == NULL)
   {
      return NULL;
   }

   return &m_image;
}

CDCSurface *CImageCacheItem::GetImageThumb()
{
   m_time = ::GetTickCount();

   if(m_imagethumb.GetWidth() == 0 ||
      m_imagethumb.GetHeight() == 0 ||
      m_imagethumb.GetDC() == NULL)
   {
      return NULL;
   }

   return &m_imagethumb;
}

void CImageCacheItem::MakeThumb()
{
   if((m_imagethumb.GetWidth() == 0 ||
       m_imagethumb.GetHeight() == 0 ||
       m_imagethumb.GetDC() == NULL) &&
      (m_image.GetWidth() > 0 &&
       m_image.GetHeight() > 0 &&
       m_image.GetDC() != NULL))
   {
      float scale = __min((152.0f / (float)m_image.GetWidth()),(130.0f / (float)m_image.GetHeight()));
      int thumbwidth = __max(1,(int)(((float)m_image.GetWidth() * scale) + 0.5));
      int thumbheight = __max(1,(int)(((float)m_image.GetHeight() * scale) + 0.5));

      m_imagethumb.Resize(thumbwidth, thumbheight);

      ::SetStretchBltMode(m_imagethumb.GetDC(),HALFTONE);
      ::SetBrushOrgEx(m_imagethumb.GetDC(), 0, 0, NULL);
      if(::StretchBlt(m_imagethumb.GetDC(),0,0,thumbwidth,thumbheight,m_image.GetDC(),0,0,m_image.GetWidth(),m_image.GetHeight(),SRCCOPY) == FALSE)
      {
         ::SetStretchBltMode(m_imagethumb.GetDC(),COLORONCOLOR);
         ::StretchBlt(m_imagethumb.GetDC(),0,0,thumbwidth,thumbheight,m_image.GetDC(),0,0,m_image.GetWidth(),m_image.GetHeight(),SRCCOPY);
      }
   }
}

void CImageCacheItem::AddNotify(unsigned int id)
{
   // see if we already have it
   for(size_t i = 0; i < m_notifylist.size(); i++)
   {
      if(m_notifylist[i] == id)
      {
         // already have it
         return;
      }
   }

   m_notifylist.push_back(id);
}

void CLampApp::OnViewShowThumbs()
{
   m_bShowImageThumbs = !m_bShowImageThumbs;
}

void CLampApp::OnUpdateViewShowThumbs(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(m_bShowImageThumbs)
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampApp::OnAutoLoadChattypicsThumbs()
{
   m_bAutoLoadChattypicsThumbs = !m_bAutoLoadChattypicsThumbs;
}

void CLampApp::OnUpdateAutoLoadChattypicsThumbs(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(m_bAutoLoadChattypicsThumbs)
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}


void CLampApp::OnDontAutoLoadNWSThumbs()
{
   m_bDontAutoLoadNWSThumbs = !m_bDontAutoLoadNWSThumbs;
}

void CLampApp::OnUpdateDontAutoLoadNWSThumbs(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(m_bDontAutoLoadNWSThumbs)
   {
      pCmdUI->SetCheck(TRUE);
   }
   else
   {
      pCmdUI->SetCheck(FALSE);
   }
}

void CLampApp::PopulateBookmarkMenu(CMenu *pMenu)
{
   if(pMenu != NULL)
   {
      std::list<CShackBookmark>::iterator it = m_Bookmarks.begin();
      std::list<CShackBookmark>::iterator end = m_Bookmarks.end();

      UINT count = 0;

      while(it != end)
      {
         UCString menutext = (*it).m_title;
         menutext += L"\t";
         menutext += (*it).m_author;
         pMenu->InsertMenu(2 + count, MF_BYPOSITION|MF_STRING, ID_BOOKMARK_MENUITEM + count, menutext);

         it++;
         count++;
      }
   }
}

CShackBookmark *CLampApp::GetBookmark(size_t index)
{
   CShackBookmark *pBM = NULL;
   if(index < m_Bookmarks.size())
   {
      std::list<CShackBookmark>::iterator it = m_Bookmarks.begin();
      std::list<CShackBookmark>::iterator end = m_Bookmarks.end();

      UINT count = 0;

      while(it != end)
      {
         if(count == index)
         {
            pBM = &(*it);
            break;
         }

         it++;
         count++;
      }
   }

   return pBM;
}

void CLampApp::LaunchBookmarkMenu(size_t index)
{
   CShackBookmark *pBM = GetBookmark(index);
   if(pBM != NULL)
   {
      UCString path;
      unsigned int id = pBM->m_current_id;
      unsigned int rootid = pBM->m_root_id;
      
      if(rootid != 0)
      {
         path += rootid;
      }
      else
      {
         path += id;
      }
      path += L'_';
      path += id;

      OpenDocumentFile(path);
   }
}


void CLampApp::ReadBookmarks()
{
   CXMLTree bookmarksroot;

   UCString bmpath;
   
   bmpath = m_userpath;
   bmpath += L"\\bookmarks.xml";
   
   if(_waccess(bmpath,00) == 0)
   {
      // it is in the old user's lamp folder
      // read it and kill it
      bookmarksroot.Read(bmpath);
      _wunlink(bmpath);
      RemoveDirectoryW(m_userpath);
   }
   else
   {
      // try the lamp folder
      bmpath.PathToMe(L"bookmarks.xml");
      if(g_bIsXP ||
         (_waccess(bmpath,00) == 0 &&
         _waccess(bmpath,06) == 0 &&
         bmpath.Find(L"Program Files") == NULL))
      {
         bookmarksroot.Read(bmpath);
      }
      else
      {
         // try the appdata folder
         bmpath = m_appdatapath;
         bmpath += L"\\bookmarks.xml";
         bookmarksroot.Read(bmpath);
      }
   }

   int numbookmarks = bookmarksroot.CountChildren();

   for(int i = 0; i < numbookmarks; i++)
   {
      CXMLElement *pElement = bookmarksroot.GetChildElement(i);
      if(pElement != NULL &&
         !pElement->IsComment() &&
         pElement->GetTag() == L"Bookmark")
      {
         CShackBookmark bm;

         bm.m_current_id = pElement->GetAttributeValue(L"reply_id");
         bm.m_root_id = pElement->GetAttributeValue(L"thread_id");
         bm.m_author = pElement->GetAttributeValue(L"author");

         UCString temp = pElement->GetAttributeValue(L"category");

              if(temp == L"ontopic") bm.m_category = PCT_NORMAL;
         else if(temp == L"informative") bm.m_category = PCT_INF;
         else if(temp == L"notworksafe") bm.m_category = PCT_NWS;
         else if(temp == L"stupid") bm.m_category = PCT_STUPID;
         else if(temp == L"offtopic") bm.m_category = PCT_OFFTOPIC;
         else if(temp == L"political") bm.m_category = PCT_POLITICAL;

         bm.m_title = pElement->GetValue();

         m_Bookmarks.push_back(bm);
      }
   }
}

void CLampApp::WriteBookmarks()
{
   CXMLTree bookmarksroot;
   bookmarksroot.SetTag(L"Bookmarks");

   std::list<CShackBookmark>::iterator it = m_Bookmarks.begin();
   std::list<CShackBookmark>::iterator end = m_Bookmarks.end();

   while(it != end)
   {
      CXMLElement *pElement = bookmarksroot.AddChildElement();
      if(pElement != NULL)
      {
         pElement->SetTag(L"Bookmark");
         UCString temp;
         
         temp = (*it).m_current_id;
         pElement->AddAttribute(L"reply_id",temp);

         temp = (*it).m_root_id;
         pElement->AddAttribute(L"thread_id",temp);

         temp = (*it).m_author;
         pElement->AddAttribute(L"author",temp);

         switch((*it).m_category)
         {
         case PCT_NORMAL:temp = L"ontopic";break;
         case PCT_INF:temp = L"informative";break;
         case PCT_NWS:temp = L"notworksafe";break;
         case PCT_STUPID:temp = L"stupid";break;
         case PCT_OFFTOPIC:temp = L"offtopic";break;
         case PCT_POLITICAL:temp = L"political";break;
         }
         pElement->AddAttribute(L"category",temp);

         pElement->SetValue((*it).m_title);
      }
      it++;
   }

   // try the program files folder
   UCString bmpath;
   
   bmpath.PathToMe(L"bookmarks.xml");
   if(g_bIsXP ||
      bmpath.Find(L"Program Files") == NULL)
   {
      bookmarksroot.Write(bmpath);
   }
   else
   {
      // try the appdata folder
      if(_waccess(m_appdatapath,00) == -1)
      {
         BOOL bMade = CreateDirectoryW(m_appdatapath, NULL);
      }

      bmpath = m_appdatapath;
      bmpath += L"\\bookmarks.xml";
      bookmarksroot.Write(bmpath);
   }
}

void CLampApp::AddCachedThreadReplies(unsigned int id,CXMLElement *pReplies)
{
   std::map<unsigned int,CXMLElement *>::iterator it = m_cachedthreadreplies.find(id);
   std::map<unsigned int,CXMLElement *>::iterator end = m_cachedthreadreplies.end();
   if(it != end)
   {
      CXMLElement *pIt = it->second;
      delete pIt;   
   }

   DWORD now = ::GetTickCount();

   pReplies->SetUserDefinedFlagPack(now);

   m_cachedthreadreplies[id] = pReplies;

   m_numcachedthreadreplyinserts++;

   if(m_numcachedthreadreplyinserts > 300)
   {
      m_numcachedthreadreplyinserts = 0;
      it = m_cachedthreadreplies.begin();
      
      while(it != end)
      {
         if(now - it->second->GetUserDefinedFlagPack() > 1800000) 
         {
            // haven't accessed in half an hour or more
            std::map<unsigned int,CXMLElement *>::iterator next = it;
            next++;
            delete it->second;
            m_cachedthreadreplies.erase(it);
            it = next;
         }
         else
         {
            it++;
         }
      }
   }
}


CXMLElement *CLampApp::GetCachedThreadReplies(unsigned int id)
{
   CXMLElement *result = NULL;
   std::map<unsigned int,CXMLElement *>::iterator it = m_cachedthreadreplies.find(id);
   if(it != m_cachedthreadreplies.end())
   {
      result = it->second;
      result->SetUserDefinedFlagPack(::GetTickCount());
   }
   return result;
}

void CLampApp::UnloadAllImages()
{
   std::list<CLampDoc*>::iterator it = m_MyDocuments.begin();
   std::list<CLampDoc*>::iterator end = m_MyDocuments.end();

   while(it != end)
   {
      (*it)->UnloadAllImages();
      
      if((*it)->GetView() != NULL)
      {
         (*it)->GetView()->InvalidateEverything();
      }

      it++;
   }

   m_imagecache.clear();
   m_nextimagecacheindex = 0;
}

void CLampApp::InvalidateContentLayout(unsigned int id)
{
   std::list<CLampDoc*>::iterator it = m_MyDocuments.begin();
   std::list<CLampDoc*>::iterator end = m_MyDocuments.end();

   while(it != end)
   {
      DocDataType ddt = (*it)->GetDataType();
      if(ddt == DDT_STORY ||
         ddt == DDT_THREAD ||
         ddt == DDT_LOLS ||
         ddt == DDT_SHACKMSG ||
         ddt == DDT_PROFILE)
      {
         ChattyPost *thispost = (*it)->FindPost(id);
         if(thispost != NULL)
         {
            thispost->InvalidateContentLayout();
         }
      }

      if((*it)->GetView() != NULL)
      {
         (*it)->GetView()->InvalidateEverything();
      }

      it++;
   }
}

ChattyPost *CLampApp::FindFromAnywhere(unsigned int id)
{
   ChattyPost *result = NULL;
   std::list<CLampDoc*>::iterator it = m_MyDocuments.begin();
   std::list<CLampDoc*>::iterator end = m_MyDocuments.end();

   while(it != end)
   {
      DocDataType ddt = (*it)->GetDataType();
      if(ddt == DDT_STORY)
      {
         ChattyPost *thispost = (*it)->FindPost(id);
         if(thispost != NULL)
         {
            ChattyPost *thisparent = thispost->GetParent();
            while(thisparent != NULL)
            {
               thispost = thisparent;
               thisparent = thispost->GetParent();
            }

            result = thispost;
            break;
         }
      }

      it++;
   }

   return result;
}


void CLampApp::ClearAllPinnedThreads()
{
   std::list<CLampDoc*>::iterator it = m_MyDocuments.begin();
   std::list<CLampDoc*>::iterator end = m_MyDocuments.end();

   while(it != end)
   {
      (*it)->ClearAllPinnedThreads();
      it++;
   }
}

void CLampApp::SetNumMinutesCheckInbox(int value)
{
   m_num_minutes_check_inbox = value;
   CMainFrame *pMainFrame = (CMainFrame*)GetMainWnd();
   if(pMainFrame != NULL)
   {
      pMainFrame->UpdateInboxTimer();
   }
}

void CLampApp::DisplayDownload()
{
   UCString text;
   UCString number;

   std::list<CDownloadHistoryItem>::iterator it = m_downloadhistory.begin();
   
   while(it != m_downloadhistory.end())
   {
      float totaltime = ((float)(it->m_end_time - it->m_start_time)) / 1000.0f;

      text += L"time=";
      number = totaltime;
      while(number.Length() < 5) number += L"0";
      text += number;
      text += L" seconds ";

      float respondtime = ((float)(it->m_recieve_time - it->m_start_time)) / 1000.0f;

      text += L"respond=";
      number = respondtime;
      while(number.Length() < 5) number += L"0";
      text += number;
      text += L" seconds ";

      text += L"type=";
      text += (int)it->m_dt;
      text += L"\r\n";
      
      if(!it->m_host.IsEmpty())
      {
         text += L"host=";
         text += it->m_host;
         text += L"\r\n";
      }

      if(!it->m_path.IsEmpty())
      {
         text += L"path=";
         text += it->m_path;
         text += L"\r\n";
      }

      if(!it->m_post_data.IsEmpty())
      {
         text += L"data=";
         text += it->m_post_data;
         text += L"\r\n";
      }

      if(!it->m_errmsg.IsEmpty())
      {
         text += L"error=";
         text += it->m_errmsg;
         text += L"\r\n";
      }

      text += L"\r\n";

      it++;
   }

   if(!text.IsEmpty())
   {
      wchar_t path[MAX_PATH+1]={0};
      if(GetTempPath(MAX_PATH, path) != 0)
      {
         if(path[wcslen(path)-1] != L'\\')
         {
            wcscat_s(path,MAX_PATH,L"\\");
         }
            
         UCString temp_file = path;
         temp_file += L"LampTraffic.txt";

         void *data = (void*)text.str8(false,CET_UTF8,true);
         size_t charsize = 1;
         size_t numchars = strlen((char*)data);

         FILE *stream = NULL;
         _wfopen_s(&stream, temp_file, L"wb");
         if(stream != NULL)
         {
            fwrite(data, charsize, numchars, stream );      
            fclose(stream);
         }   

         ShellExecuteW(NULL,L"open",temp_file, NULL, NULL, SW_SHOW);
      }
   }
}

CLOLFlags &CLampApp::GetKnownLOLFlags(unsigned int post_id)
{
   return m_cachedLOLposts[post_id];
}

void CLampApp::SetLOL_LOL(unsigned int post_id, unsigned int count)
{
   CLOLFlags flags = m_cachedLOLposts[post_id];
   flags.m_LOLd = count;
   m_cachedLOLposts[post_id] = flags;
}

void CLampApp::SetLOL_INF(unsigned int post_id, unsigned int count)
{
   CLOLFlags flags = m_cachedLOLposts[post_id];
   flags.m_INFd = count;
   m_cachedLOLposts[post_id] = flags;
}

void CLampApp::SetLOL_UNF(unsigned int post_id, unsigned int count)
{
   CLOLFlags flags = m_cachedLOLposts[post_id];
   flags.m_UNFd = count;
   m_cachedLOLposts[post_id] = flags;
}

void CLampApp::SetLOL_TAG(unsigned int post_id, unsigned int count)
{
   CLOLFlags flags = m_cachedLOLposts[post_id];
   flags.m_TAGd = count;
   m_cachedLOLposts[post_id] = flags;
}

void CLampApp::SetLOL_WTF(unsigned int post_id, unsigned int count)
{
   CLOLFlags flags = m_cachedLOLposts[post_id];
   flags.m_WTFd = count;
   m_cachedLOLposts[post_id] = flags;
}

void CLampApp::SetLOL_UGH(unsigned int post_id, unsigned int count)
{
   CLOLFlags flags = m_cachedLOLposts[post_id];
   flags.m_UGHd = count;
   m_cachedLOLposts[post_id] = flags;
}

void CLampApp::CalcDescent()
{
   HFONT hCreatedFont = ::CreateFontW(GetFontHeight(),0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_DONTCARE,GetNormalFontName());

   HDC hTempDC = ::CreateCompatibleDC(NULL);
   HFONT oldfont = (HFONT)::SelectObject(hTempDC,hCreatedFont);

   TEXTMETRIC tm;
   GetTextMetrics(hTempDC,&tm);
   m_descent = tm.tmDescent;

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

void CLampApp::GenerateFlagActives()
{
   std::map<UCString, CFlagImage>::iterator it = m_flagimages.begin();
   std::map<UCString, CFlagImage>::iterator end = m_flagimages.end();
  
   while(it != end)
   {
      if(it->second.m_image_preview != NULL)
         delete it->second.m_image_preview;

      if(it->second.m_image_reply != NULL)
         delete it->second.m_image_reply;

      if(it->second.m_image_root != NULL)
         delete it->second.m_image_root;

      it->second.m_image_preview = new CDCSurface();
      it->second.m_image_preview->EnableCachedStretchImage(true);
      it->second.m_image_preview->ReadPNG(it->second.m_image_path,true,true,false);

      it->second.m_image_reply = new CDCSurface();
      it->second.m_image_reply->EnableCachedStretchImage(true);
      it->second.m_image_reply->ReadPNG(it->second.m_image_path,true,false,false);

      it->second.m_image_root = new CDCSurface();
      it->second.m_image_root->EnableCachedStretchImage(true);
      it->second.m_image_root->ReadPNG(it->second.m_image_path,true,false,true);

      it->second.m_active_rect.left = it->second.m_active_rect.top = 0;
      it->second.m_active_rect.bottom = GetTextHeight();
      it->second.m_active_rect.right = (int)(((double)it->second.m_active_rect.bottom / (double)it->second.m_image_preview->GetHeight()) * (double)it->second.m_image_preview->GetWidth());

      it++;
   }
   
}

void CLampApp::GenerateLightningBolt()
{
   CDCSurface canvas;
   int lbsize = m_textheight / 2;
   canvas.Resize(lbsize * 2,lbsize);
   canvas.Fill(0,0,0);

   HDC hdc = canvas.GetDC();

   HBRUSH hbrush = ::CreateSolidBrush(RGB(255,255,255));
   HPEN hpen = ::CreatePen(PS_NULL,0,0);
   HBRUSH hold_brush = (HBRUSH)::SelectObject(hdc,hbrush);
   HPEN hold_pen = (HPEN)::SelectObject(hdc,hpen);

   POINT points[6];
   points[0].x = 1;
   points[0].y = 1;
   points[1].x = (int)(((float)lbsize * 2) * (3.0/8.0));
   points[1].y = lbsize - 1;
   points[2].x = (int)(((float)lbsize * 2) * (5.0/8.0));
   points[2].y = lbsize / 2;
   points[3].x = (lbsize * 2) - 1;
   points[3].y = lbsize - 1;
   points[4].x = (int)(((float)lbsize * 2) * (5.0/8.0));
   points[4].y = 1;
   points[5].x = (int)(((float)lbsize * 2) * (3.0/8.0));
   points[5].y = lbsize / 2;

   ::Polygon(hdc,points,6);

   ::SelectObject(hdc,hold_brush);
   ::SelectObject(hdc,hold_pen);
   ::DeleteObject(hbrush);
   ::DeleteObject(hpen);

   m_lightningbolt.MakeDecal(&canvas, GetRValue(m_author_color), GetGValue(m_author_color), GetBValue(m_author_color));
   m_lightningbolt_hover.MakeDecal(&canvas, GetRValue(m_mypost_color), GetGValue(m_mypost_color), GetBValue(m_mypost_color));
}

bool CLampApp::IsPostKnown(unsigned int id)
{
   std::map<unsigned int,ChattyPost*>::iterator it = m_KnownPosts.find(id);
   if(it != m_KnownPosts.end())
   {
      return true;
   }
   return false;
}

void CLampApp::KnowPost(unsigned int id, ChattyPost *post)
{
   std::map<unsigned int,ChattyPost *>::iterator it = m_KnownPosts.find(id);
   if(it != m_KnownPosts.end())
   {
      ChattyPost *pIt = it->second;
      delete pIt;   
   }

   m_KnownPosts[id] = post;
}

ChattyPost *CLampApp::GetKnownPost(unsigned int id)
{
   std::map<unsigned int,ChattyPost*>::iterator it = m_KnownPosts.find(id);
   if(it != m_KnownPosts.end())
   {
      return it->second;
   }
   return NULL;
}

void CLampApp::ClearKnownPosts()
{
   std::map<unsigned int,ChattyPost *>::iterator pit = m_KnownPosts.begin();
   std::map<unsigned int,ChattyPost *>::iterator pend = m_KnownPosts.end();
   while(pit != pend)
   {
      delete pit->second;
      pit++;
   }
   m_KnownPosts.clear();
}

unsigned int CLampApp::GetUserID()
{
   unsigned int result = m_userid;

   if(result == 0)
   {
      char UserShackLoginCookie[1024];

      UCString loginstr = L"username=";
      loginstr += m_username;
      loginstr += L"&password=" ;
      loginstr += m_password;
      loginstr += L"&type=login";

      std::string stdstring;

      comm_download("www.shacknews.com","/login_laryn.x", &stdstring,loginstr.str8(false,CET_UTF8),"",NULL);

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
               strncpy_s(UserShackLoginCookie,1024,cook,end-cook);
            }
         }
      }

      stdstring = "";

      comm_download("www.shacknews.com","/messages", &stdstring,UserShackLoginCookie,false,NULL);

      if(stdstring.length() > 0)
      {
         const char *work = strstr(stdstring.data(),"<input type=\"hidden\" name=\"uid\" value=\"");
         if(work != NULL)
         {
            work += 39;
            m_userid = result = strtoul(work,NULL, 10);
         }
      }
   }

   return result;
}

bool CLampApp::SingleThreadStyle()
{
   return g_bSingleThreadStyle;
}

void CLampApp::SetSingleThreadStyle(bool value)
{
   g_bSingleThreadStyle = value;
}

void CLampApp::CheckForExpiredImages(void)
{
   DWORD currenttime = ::GetTickCount();

   std::map<unsigned int,CImageCacheItem>::iterator it = m_imagecache.begin();

   while(it != m_imagecache.end())
   {
      if(currenttime - it->second.m_time > 60000 * (60 * 4))
      {
         std::list<CLampDoc*>::iterator doc_it = m_MyDocuments.begin();
         std::list<CLampDoc*>::iterator doc_end = m_MyDocuments.end();

         while(doc_it != doc_end)
         {
            (*doc_it)->UnloadImage(it->first);
            doc_it++;
         }

         m_imagecache.erase(it);

         it = m_imagecache.begin();
      }
      else
      {
         it++;
      }
   }

   std::list<CLampDoc*>::iterator doc_it = m_MyDocuments.begin();
   std::list<CLampDoc*>::iterator doc_end = m_MyDocuments.end();

   while(doc_it != doc_end)
   {
      if((*doc_it)->GetView() != NULL)
      {
         (*doc_it)->GetView()->InvalidateEverything();
      }

      doc_it++;
   }
}


void CLampApp::CheckForModMode()
{
   UCString temp = GetUsername();
   temp.MakeLower();

   std::set<UCString>::iterator it = m_mod_list.find(temp);
  
   if(it != m_mod_list.end())
   {
      m_modmode = true;
   }
   else
   {
      m_modmode = false;
   }

   // del me
   /*
   if(temp == L"crasterimage")
   {
      m_modmode = true;
   }
   */
}

void CLampApp::CollectFlagImages()
{
   UCString folder;
   folder = L"Flags";

   UCString path;
   path.PathToMe(folder);
   path += "\\*.png";

   _wfinddata_t fileStruct;

   int hSearch = _wfindfirst( (wchar_t*)path.Str(), &fileStruct);

   if(hSearch != -1L)
   {
      do
      {
         UCString name = fileStruct.name;
         folder = L"Flags\\";
         folder += name;
         path.PathToMe(folder);

         name.TrimEnd(4);

         CFlagImage flagimage;
         flagimage.m_image_path = path;
 
         m_flagimages[name] = flagimage;

      }while( _wfindnext( hSearch, &fileStruct ) == 0);

      _findclose( hSearch );
   }

   // hook up images to users

   std::map<UCString,CFlaggedUser>::iterator fit = m_flagged_users.begin();
   std::map<UCString,CFlaggedUser>::iterator fend = m_flagged_users.end();

   while(fit != fend)
   {
      std::map<UCString,CFlagImage>::iterator it = m_flagimages.find(fit->second.m_flag);

      if(it != m_flagimages.end())
      {
         fit->second.m_flag_image = &(it->second);
      }

      fit++;
   }
}


CFlaggedUser *CLampApp::GetFlaggedUser(const UCString &username)
{
   CFlaggedUser *result = NULL;
   std::map<UCString,CFlaggedUser>::iterator it = m_flagged_users.find(username);

   if(it != m_flagged_users.end())
   {
      result = &(it->second);
   }

   return result;
}

void CLampApp::AddFlaggedUser(const UCString &username, const UCString &flag, const UCString &note)
{
   CFlaggedUser flagged_user;
   flagged_user.m_flag = flag;
   flagged_user.m_note = note;

   std::map<UCString,CFlagImage>::iterator it = m_flagimages.find(flag);

   if(it != m_flagimages.end())
   {
      flagged_user.m_flag_image = &(it->second);
   }
   
   m_flagged_users[username] = flagged_user;

   InvalidateFlagsAllViews();

   WriteSettingsFile();
}

void CLampApp::UpdateFlaggedUser(const UCString &username, const UCString &flag, const UCString &note)
{
   CFlaggedUser *flagged_user = GetFlaggedUser(username);

   if(flagged_user != NULL)
   {
      flagged_user->m_flag = flag;
      flagged_user->m_note = note;

      std::map<UCString,CFlagImage>::iterator it = m_flagimages.find(flag);

      if(it != m_flagimages.end())
      {
         flagged_user->m_flag_image = &(it->second);
      }
   }

   InvalidateFlagsAllViews();

   WriteSettingsFile();
}

void CLampApp::DeleteFlaggedUser(const UCString &username)
{
   std::map<UCString,CFlaggedUser>::iterator it = m_flagged_users.find(username);

   if(it != m_flagged_users.end())
   {
      m_flagged_users.erase(it);
   }

   InvalidateFlagsAllViews();

   WriteSettingsFile();
}