
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
#include "SendMsgDlg.h"

#define NDEBUG

#include "JSONOptions.h"
#include "libjson.h"
#include "JSONNode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


UCString g_PathToMe;

bool g_bIsXP = false;

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
   ON_COMMAND(ID_VIEW_AUTOSHOWLOADEDIMAGES, &CLampApp::OnViewAutoshowloadedimages)
   ON_UPDATE_COMMAND_UI(ID_VIEW_AUTOSHOWLOADEDIMAGES, &CLampApp::OnUpdateViewAutoshowloadedimages)
   ON_COMMAND(ID_SHKMSGS_INBOX, &CLampApp::OnShackMsg_Inbox)
   ON_UPDATE_COMMAND_UI(ID_SHKMSGS_INBOX, &CLampApp::OnUpdateShackMsg_Inbox)
   ON_COMMAND(ID_SHKMSGS_OUTBOX, &CLampApp::OnShackMsg_Outbox)
   ON_UPDATE_COMMAND_UI(ID_SHKMSGS_OUTBOX, &CLampApp::OnUpdateShackMsg_Outbox)
   ON_COMMAND(ID_SHKMSGS_ARCHIVE, &CLampApp::OnShackMsg_Archive)
   ON_UPDATE_COMMAND_UI(ID_SHKMSGS_ARCHIVE, &CLampApp::OnUpdateShackMsg_Archive)
   //ON_MESSAGE(WM_THREAD_DOWNLOAD, &CLampApp::OnThreadDownload)

END_MESSAGE_MAP()

BOOL CLampApp::PreTranslateMessage(MSG* pMsg)
{
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
                        msg += L") available.\r\nWould you like to download it?";
                        int ret = GetMainWnd()->MessageBox(msg,L"Lamp",MB_YESNO);
                        if(ret == IDYES)
                        {
                           UCString link(L"http://shackwiki.com/wiki/Lamp#-_Download");
                           OpenShackLink(link);
                        }
                     }                     

                     CXMLElement *pMods = xmldata.FindChildElement(L"Mods");
                     CXMLElement *pEmps = xmldata.FindChildElement(L"ShackEmployees");
                     CXMLElement *pDevs = xmldata.FindChildElement(L"GameDevs");
                     if(pMods != NULL &&
                        pEmps != NULL &&
                        pDevs != NULL)
                     {
                        m_namelist.clear();

                        // the update doesn't have colors, so preserve them
                        UCString existingcolor = m_Mods.GetAttributeValue(L"color");
                        UCString existingenable = m_Mods.GetAttributeValue(L"enable");
                        m_Mods = *pMods;
                        m_Mods.AddAttribute(L"color",existingcolor);
                        m_Mods.AddAttribute(L"enable",existingenable);

                        COLORREF color = m_Mods.GetAttributeValue(L"color");
                        bool enabled = m_Mods.GetAttributeValue(L"enable");
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
                     
                        // the update doesn't have colors, so preserve them
                        existingcolor = m_GameDevs.GetAttributeValue(L"color");
                        existingenable = m_GameDevs.GetAttributeValue(L"enable");
                        m_GameDevs = *pDevs;
                        m_GameDevs.AddAttribute(L"color",existingcolor);
                        m_GameDevs.AddAttribute(L"enable",existingenable);

                        color = m_GameDevs.GetAttributeValue(L"color");
                        enabled = m_GameDevs.GetAttributeValue(L"enable");
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
                     }                     
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

                  JSONNode n = libjson::parse(jsondata);
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

                           if(category == L"lol")      AddLOL_LOL(id, count);
                           else if(category == L"inf") AddLOL_INF(id, count);
                           else if(category == L"unf") AddLOL_UNF(id, count);
                           else if(category == L"tag") AddLOL_TAG(id, count);
                           else if(category == L"wtf") AddLOL_WTF(id, count);

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
            pMainFrame->MBClick(point);
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
   m_AutoShowLoadedImages = false;
	m_bHiColorIcons = TRUE;
   m_smoothscrollscale = 0.1f;

   m_fontheight = -13;
   m_miscfontheight = -10;
   m_samplefontheight = -9;
   m_textheight = 16;
   m_cellheight = 20;

   m_textscaler = 1.0f;

   m_pHunspell = NULL;

   m_bDockedMode = false;
   m_bCollapsed = false;
   m_bWasInDockedMode = false;

   m_pDockTab = NULL;

   m_numcachedthreadreplyinserts = 0;

   m_numshow_truncated = 10;

   m_bPinningInStories = true;
   m_bDoublePageStory = false;
   m_bStartInDockedMode = true;
   m_bShowLOLButtons = true;
   m_bAlwaysOnTopWhenNotDocked = false;

   m_normal_fontname = L"Arial";
   m_quoted_fontname = L"Times New Roman";
   m_code_fontname = L"Courier New";

   m_num_minutes_check_inbox = 3;

   m_mb_pan_scale = 1.0f;

   m_enable_spell_checker = true;
   m_highlight_OP = true;
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CLampApp object

CLampApp theApp;


// CLampApp initialization

BOOL CLampApp::InitInstance()
{
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

   ReadBookmarks();
   ReadSettingsFile();
   ReadSkinFiles();

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

   int widths[4];
   GetCharWidths(L"wag", widths, 3, false, false, ShowSmallLOL(), GetNormalFontName());
   m_LOLFieldWidth = widths[0] + widths[1] + widths[2];
   
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
   for(size_t i = 0; i < m_imagecache.size();i++)
   {
      if(!m_imagecache[i].m_tempfilename.IsEmpty() &&
         _waccess(m_imagecache[i].m_tempfilename,0) != -1)
      {
         _wunlink(m_imagecache[i].m_tempfilename);
      }
   }

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

   return CWinApp::ExitInstance();
}

CDocument* CLampApp::OpenDocumentFile(LPCTSTR lpszFileName)
{
   CDocument* pNewDoc = CWinAppEx::OpenDocumentFile(lpszFileName);

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

// Implementation
protected:
public:
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
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
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);

   bNameValid = strName.LoadString(IDS_COPYLINK_MENU);
	ASSERT(bNameValid);
   GetContextMenuManager()->AddMenu(strName, IDR_POPUP_COPYLINK_MENU);
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

   setting = hostxml.FindChildElement(L"host");
   if(setting!=NULL) m_hostname = setting->GetValue();
   else m_hostname = L"shackapi.stonedonkey.com";

   setting = hostxml.FindChildElement(L"userhost");
   if(setting!=NULL) m_userhostname = setting->GetValue();
   else m_userhostname = L"shackapi.stonedonkey.com";

   setting = hostxml.FindChildElement(L"lolhost");
   if(setting!=NULL) m_lolhostname = setting->GetValue();
   else m_lolhostname = L"lmnopc.com";

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

   setting = hostxml.FindChildElement(L"lang");
   if(setting!=NULL) m_lang = setting->GetValue();
   else m_lang = L"en_US";

   setting = hostxml.FindChildElement(L"numshow_truncated");
   if(setting!=NULL) m_numshow_truncated = setting->GetValue();
   else m_numshow_truncated = 10;

   setting = hostxml.FindChildElement(L"smooth_scroll");
   if(setting!=NULL) m_smooth_scroll = setting->GetValue();
   else m_smooth_scroll = true;

   setting = hostxml.FindChildElement(L"AutoShowLoadedImages");
   if(setting!=NULL) m_AutoShowLoadedImages = setting->GetValue();
   else m_AutoShowLoadedImages = false;

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

   setting = hostxml.FindChildElement(L"SmallLOLButtons");
   if(setting!=NULL) m_bShowSmallLOL = setting->GetValue();
   else m_bShowSmallLOL = true;   

   setting = hostxml.FindChildElement(L"KeepMeFromGTLT");
   if(setting!=NULL) m_bKeepMeFromGTLT = setting->GetValue();
   else m_bKeepMeFromGTLT = true;      
   
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

   // save session
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
}

void CLampApp::WriteSettingsFile()
{
   CXMLTree settingsxml;

   settingsxml.SetTag(L"Settings");

   settingsxml.AddChildComment(L"settings");

   settingsxml.AddChildComment(L"host for getting user info");
   settingsxml.AddChildElement(L"userhost",m_userhostname);

   settingsxml.AddChildComment(L"host for getting chatty stuff");
   settingsxml.AddChildElement(L"host",m_hostname);

   settingsxml.AddChildComment(L"host for sending lol votes");
   settingsxml.AddChildElement(L"lolhost",m_lolhostname);

   settingsxml.AddChildComment(L"login stuff");
   settingsxml.AddChildElement(L"username",m_username);


   if(!m_password.IsEmpty())
   {
      byte *buffer = (byte *)m_password.str8(false,CET_UTF8);
      if(buffer != NULL)
      {
         size_t numbytes = strlen((char*)buffer);
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
         }
      }
   }
   

   settingsxml.AddChildComment(L"general stuff");
   settingsxml.AddChildElement(L"tab_title_word_limit",UCString(m_tab_title_word_limit));
   settingsxml.AddChildElement(L"tab_title_char_limit",UCString(m_tab_title_char_limit));
   settingsxml.AddChildElement(L"smooth_scroll_scale",UCString(m_smoothscrollscale));
   settingsxml.AddChildElement(L"skin_folder",m_skinname);
   settingsxml.AddChildElement(L"mouse_wheel_scale",UCString(m_mouse_wheel_scale));
   settingsxml.AddChildElement(L"mb_pan_scale",UCString(m_mb_pan_scale));
   settingsxml.AddChildElement(L"lang",m_lang);
   settingsxml.AddChildElement(L"numshow_truncated",UCString(m_numshow_truncated));
   settingsxml.AddChildElement(L"smooth_scroll",UCString(m_smooth_scroll));
   settingsxml.AddChildElement(L"AutoShowLoadedImages",UCString(m_AutoShowLoadedImages));
   settingsxml.AddChildElement(L"text_scale",UCString(m_textscaler));
   settingsxml.AddChildElement(L"DockedMode",UCString(m_bWasInDockedMode));
   settingsxml.AddChildElement(L"PinningInStories",UCString(m_bPinningInStories));
   settingsxml.AddChildElement(L"DoublePageStory",UCString(m_bDoublePageStory));
   settingsxml.AddChildElement(L"ShowLOLButtons",UCString(m_bShowLOLButtons));
   settingsxml.AddChildElement(L"SmallLOLButtons",UCString(m_bShowSmallLOL));
   settingsxml.AddChildElement(L"KeepMeFromGTLT",UCString(m_bKeepMeFromGTLT));
   settingsxml.AddChildElement(L"AlwaysOnTopWhenNotDocked",UCString(m_bAlwaysOnTopWhenNotDocked));
   settingsxml.AddChildElement(L"num_minutes_check_inbox",UCString(m_num_minutes_check_inbox));
   settingsxml.AddChildElement(L"enable_spell_checker",UCString(m_enable_spell_checker));
   settingsxml.AddChildElement(L"highlight_OP",UCString(m_highlight_OP));
   settingsxml.AddChildElement(L"normal_fontname",m_normal_fontname);
   settingsxml.AddChildElement(L"quoted_fontname",m_quoted_fontname);
   settingsxml.AddChildElement(L"code_fontname",m_code_fontname);

   UCString temp = GetRValue(m_text_selection_color);
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

   settingsxml.AddChildComment(L"Color-Coded usernames");
   CXMLElement *mods = settingsxml.AddChildElement();
   *mods = m_Mods;

   CXMLElement *emp = settingsxml.AddChildElement();
   *emp = m_ShackEmployees;

   CXMLElement *dev = settingsxml.AddChildElement();
   *dev = m_GameDevs;

   // save session
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
   imagefilename += L"\\inf_root.png";
   imagepath.PathToMe(imagefilename);
   m_infroot.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\inf_reply.png";
   imagepath.PathToMe(imagefilename);
   m_infreply.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\inf_star.png";
   imagepath.PathToMe(imagefilename);
   m_infstar.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\nws_root.png";
   imagepath.PathToMe(imagefilename);
   m_nwsroot.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\nws_reply.png";
   imagepath.PathToMe(imagefilename);
   m_nwsreply.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\nws_star.png";
   imagepath.PathToMe(imagefilename);
   m_nwsstar.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\stupid_root.png";
   imagepath.PathToMe(imagefilename);
   m_stupidroot.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\stupid_reply.png";
   imagepath.PathToMe(imagefilename);
   m_stupidreply.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\stupid_star.png";
   imagepath.PathToMe(imagefilename);
   m_stupidstar.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\offtopic_root.png";
   imagepath.PathToMe(imagefilename);
   m_offtopicroot.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\offtopic_reply.png";
   imagepath.PathToMe(imagefilename);
   m_offtopicreply.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\offtopic_star.png";
   imagepath.PathToMe(imagefilename);
   m_offtopicstar.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\political_root.png";
   imagepath.PathToMe(imagefilename);
   m_politicalroot.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\political_reply.png";
   imagepath.PathToMe(imagefilename);
   m_politicalreply.ReadPNG(imagepath);

   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\political_star.png";
   imagepath.PathToMe(imagefilename);
   m_politicalstar.ReadPNG(imagepath);

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
   imagefilename += L"\\tags.png";
   imagepath.PathToMe(imagefilename);
   m_tags.ReadPNG(imagepath);
   
   imagefilename = L"skins\\";
   imagefilename += m_skinname;
   imagefilename += L"\\tags_hover.png";
   imagepath.PathToMe(imagefilename);
   m_tags_hover.ReadPNG(imagepath);

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
   imagefilename += L"\\mb_pan.png";
   imagepath.PathToMe(imagefilename);
   m_mb_pan.ReadPNG(imagepath);
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


void CLampApp::OpenShackLink(const UCString &shackpath)
{
   bool bIsMine = false;

   if(_wcsnicmp(shackpath,L"http://www.shacknews.com/laryn.x?story=",39) == 0)
   {
      // todo new story urls
      bIsMine = true;
   }
   else if(_wcsnicmp(shackpath,L"http://www.shacknews.com/chatty?id=",35) == 0 ||
           _wcsnicmp(shackpath,L"http://www.shacknews.com/laryn.x?id=",36) == 0)
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
      ShellExecuteW(NULL,
                    L"open",
                    shackpath,
                    NULL,
                    NULL,
                    SW_SHOW);
   
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
   return have;
}

void CLampApp::CheckForUpdates()
{
   CDownloadData *pDD = new CDownloadData();

   // http://shacklamp.omgninja.com/update.xml"

   pDD->m_host = L"shacklamp.omgninja.com";
   pDD->m_path = L"/update.xml";
   pDD->m_WhoWants = this;
   pDD->m_dt = DT_CHECK_UPDATE;
   pDD->m_id = 0;
   pDD->m_refreshid = 0;
   pDD->reply_to_id = 0;
   pDD->m_postrootid = 0;

   AfxBeginThread(DownloadThreadProc, pDD);
}


void CLampApp::RefreshLOLs()
{
   CDownloadData *pDD = new CDownloadData();

   // http://lmnopc.com/greasemonkey/shacklol/api.php?special=getcounts

   pDD->m_host = L"lmnopc.com";
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
   std::list<CLampView*>::iterator it = m_views.begin();
   std::list<CLampView*>::iterator end = m_views.end();

   while(it != end)
   {
      if((*it)->GetDocument()->GetDataType() == DDT_SHACKMSG &&
         (*it)->GetDocument()->GetShackMessageType() == SMT_INBOX &&
         !(*it)->GetDLGUp())
      {
         (*it)->GetDocument()->GetShackMessages();
         (*it)->InvalidateEverything();
      }
      it++;
   }
}

void CLampApp::SetShowSmallLOL(bool value)
{
   m_bShowSmallLOL = value;
   int widths[4];
   GetCharWidths(L"wag", widths, 3, false, false, ShowSmallLOL(), GetNormalFontName());
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

void CLampApp::InvalidateSkinAllViews()
{
   m_fontheight = (int)(-13.0f * m_textscaler);
   m_miscfontheight = (int)(-10.0f * m_textscaler);
   m_samplefontheight = (int)(-9.0f * m_textscaler);
   m_textheight = (int)(16.0f * m_textscaler);
   m_cellheight = (int)(20.0f * m_textscaler);

   int widths[4];
   GetCharWidths(L"wag", widths, 3, false, false, ShowSmallLOL(), GetNormalFontName());
   m_LOLFieldWidth = widths[0] + widths[1] + widths[2];

   std::list<CLampView*>::iterator it = m_views.begin();
   std::list<CLampView*>::iterator end = m_views.end();

   while(it != end)
   {
      (*it)->InvalidateSkin();
      it++;
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
                                   std::vector<UCString> &suggestions)
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
         UCString temp = wlsti[i];
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
   case LTT_LOL: AddLOL_LOL(post_id, flags.m_LOLd + 1); break;
   case LTT_INF: AddLOL_INF(post_id, flags.m_INFd + 1); break;
   case LTT_UNF: AddLOL_UNF(post_id, flags.m_UNFd + 1); break;
   case LTT_TAG: AddLOL_TAG(post_id, flags.m_TAGd + 1); break;
   case LTT_WTF: AddLOL_WTF(post_id, flags.m_WTFd + 1); break;
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
   std::list<unsigned int>::iterator it = m_mycollapses.begin();
   std::list<unsigned int>::iterator end = m_mycollapses.end();

   while(it != end)
   {
      if((*it) == post_id)
      {
         return;
      }

      it++;
   }

   // add it
   m_mycollapses.push_back(post_id);

   while(m_mycollapses.size() > 100)
   {
      m_mycollapses.pop_front();
   }
}

void CLampApp::RemoveMyCollapse(unsigned int post_id)
{
   std::list<unsigned int>::iterator it = m_mycollapses.begin();
   std::list<unsigned int>::iterator end = m_mycollapses.end();

   while(it != end)
   {
      if((*it) == post_id)
      {
         // we do have it, so remove it
         m_mycollapses.remove(post_id);
         return;
      }

      it++;
   }
}

bool CLampApp::GetMyCollapse(unsigned int post_id)
{
   std::list<unsigned int>::iterator it = m_mycollapses.begin();
   std::list<unsigned int>::iterator end = m_mycollapses.end();

   while(it != end)
   {
      if((*it) == post_id)
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

   if(csdlg.DoModal() == IDOK)
   {
      m_last_search_author = csdlg.m_user;
      m_last_search_parent_author = csdlg.m_parent;
      m_last_search_terms = csdlg.m_terms;

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
   
   for(size_t i = 0; i < m_imagecache.size(); i++)
   {
      if(m_imagecache[i].m_host == host &&
         m_imagecache[i].m_path == path &&
         m_imagecache[i].m_image.GetDC() != NULL &&
         m_imagecache[i].m_image.GetWidth() > 0 &&
         m_imagecache[i].m_image.GetHeight() > 0)
      {
         index = i;
         result = true;
         break;
      }
   }

   return result;
}

CDCSurface *CLampApp::GetLinkedImage(const UCString &link, unsigned int &index)
{
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
   
   for(size_t i = 0; i < m_imagecache.size(); i++)
   {
      if(m_imagecache[i].m_host == host &&
         m_imagecache[i].m_path == path)
      {
         result = m_imagecache[i].GetImage();
         index = i;
         break;
      }
   }

   if(result == NULL)
   {
      const UCChar *ext = end;
      while(ext > begin && *ext != L'.') ext--;

      // create it
      m_imagecache.push_back(CImageCacheItem());
      m_imagecache[m_imagecache.size()-1].m_host = host;
      m_imagecache[m_imagecache.size()-1].m_path = path;
      m_imagecache[m_imagecache.size()-1].m_ext = ext;
      result = m_imagecache[m_imagecache.size()-1].GetImage();
      index = m_imagecache.size()-1;
   }

   return result;
}

CDCSurface *CLampApp::GetLinkedImage(unsigned int index)
{
   CDCSurface *result = NULL;
   
   if(index < m_imagecache.size())
   {
      result = m_imagecache[index].GetImage();
   }

   return result;
}


CDCSurface *CImageCacheItem::GetImage()
{
   if(m_image.GetWidth() == 0 ||
      m_image.GetHeight() == 0 ||
      m_image.GetDC() == NULL)
   {
      if(m_tempfilename.IsEmpty())
      {
         CWaitCursor wait;
         // download it
         UCString temphost = m_host;
         UCString temppath = m_path;
         if(m_host == L"www.shackpics.com" ||
            m_host == L"shackpics.com")
         {
            // http://www.shackpics.com/viewer.x?file=pic.jpg
            // http://www.shackpics.com/files/pic.jpg
            temppath.Replace(L"viewer.x?file=",L"files/");
            temphost = L"www.shackpics.com";
         }

         if(m_host == L"www.fukung.net" ||
            m_host == L"fukung.net")
         {
            // http://www.fukung.net/v/32798/b3194b0e9a4e7f629059c0ff55d91bff.jpg
            // http://media.fukung.net/images/32798/b3194b0e9a4e7f629059c0ff55d91bff.jpg
            temppath.Replace(L"/v/",L"/images/");
            temphost = L"media.fukung.net";
         }

         temppath.Replace(L"%20",L" ");

         char *data = NULL;
         int size = 0;
         chattyerror err = download(temphost.str8(), temppath.str8(), &data, &size);

         if(err == ERR_OK && data != NULL)
         {
            if((data[0] == (char)0xff && data[1] == (char)0xe0) ||
               (data[0] == (char)0xff && data[1] == (char)0xe1) ||
               (data[0] == (char)0xff && data[1] == (char)0xd8 && data[2] == (char)0xff && data[3] == (char)0xe0) ||
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
                     suspect += m_ext;

                     if(_waccess(suspect,00) == -1)
                     {
                        FILE *pFile = NULL;
                        fopen_s(&pFile,suspect.str8(),"wb");
                        if(pFile != NULL)
                        {
                           if(_wcsicmp(m_ext,L".jpg") == 0 ||
                              _wcsicmp(m_ext,L".jpeg") == 0)
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
                           m_tempfilename = suspect;
                        }
                     }
                  }
               }
            }
         }

         if(data != NULL)
         {
            free(data);
         }
      }

      if(!m_tempfilename.IsEmpty())
      {
         if(_wcsicmp(m_ext,L".png") == 0)
         {
            m_image.ReadPNG(m_tempfilename);
         }      
         else // if(_wcsicmp(m_ext,L".jpg") == 0 || _wcsicmp(m_ext,L".jpeg") == 0)
         {
            m_image.ReadJpeg(m_tempfilename);
         }
         m_image.EnableCachedStretchImage(true);
      }
   }

   return &m_image;
}
void CLampApp::OnViewAutoshowloadedimages()
{
   m_AutoShowLoadedImages = !m_AutoShowLoadedImages;
}

void CLampApp::OnUpdateViewAutoshowloadedimages(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(TRUE);

   if(m_AutoShowLoadedImages)
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
         else if(temp == L"political") bm.m_category = PCT_POLITCIAL;

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
         case PCT_POLITCIAL:temp = L"political";break;
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

void CLampApp::SendMessageDlg(CLampDoc *pDoc, const UCString &to, const UCString &subject, const UCString &shackmsg)
{
   SendMsgDlg dlg(GetMainWnd());

   dlg.m_to = to;
   dlg.m_subject = subject;
   dlg.m_shackmsg = shackmsg;
   dlg.m_pDoc = pDoc;
   dlg.DoModal();
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

CLOLFlags &CLampApp::GetKnownLOLFlags(unsigned int post_id)
{
   return m_cachedLOLposts[post_id];
}

void CLampApp::AddLOL_LOL(unsigned int post_id, unsigned int count)
{
   CLOLFlags flags = m_cachedLOLposts[post_id];
   flags.m_LOLd = count;
   m_cachedLOLposts[post_id] = flags;
}

void CLampApp::AddLOL_INF(unsigned int post_id, unsigned int count)
{
   CLOLFlags flags = m_cachedLOLposts[post_id];
   flags.m_INFd = count;
   m_cachedLOLposts[post_id] = flags;
}

void CLampApp::AddLOL_UNF(unsigned int post_id, unsigned int count)
{
   CLOLFlags flags = m_cachedLOLposts[post_id];
   flags.m_UNFd = count;
   m_cachedLOLposts[post_id] = flags;
}

void CLampApp::AddLOL_TAG(unsigned int post_id, unsigned int count)
{
   CLOLFlags flags = m_cachedLOLposts[post_id];
   flags.m_TAGd = count;
   m_cachedLOLposts[post_id] = flags;
}

void CLampApp::AddLOL_WTF(unsigned int post_id, unsigned int count)
{
   CLOLFlags flags = m_cachedLOLposts[post_id];
   flags.m_WTFd = count;
   m_cachedLOLposts[post_id] = flags;
}

