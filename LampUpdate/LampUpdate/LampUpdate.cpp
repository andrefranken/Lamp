
// LampUpdate.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "LampUpdate.h"
#include "LampUpdateDlg.h"
#include "UCString.h"
#include "webclient.h"
#include "XMLElement.h"
#include <list>
#include "..\ziplib\ziplib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UCString g_PathToMe;

// CLampUpdateApp

BEGIN_MESSAGE_MAP(CLampUpdateApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CLampUpdateApp construction

CLampUpdateApp::CLampUpdateApp()
{
	m_pDialog = NULL;
}


// The one and only CLampUpdateApp object

CLampUpdateApp theApp;


// CLampUpdateApp initialization

BOOL CLampUpdateApp::InitInstance()
{
   SetUserAgent("LampUpdate");
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

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
   
   m_pDialog = new CLampUpdateDlg();
	m_pMainWnd = m_pDialog;
	INT_PTR nResponse = m_pDialog->DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

   delete m_pDialog;
   m_pDialog = NULL;

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}


UINT ThreadProc( LPVOID pParam )
{
   if(theApp.m_pDialog != NULL)
   {
      // get the lastet version number from LampUpdate.xml
      theApp.m_pDialog->AppendMsg(L"Get the lastet version number from LampUpdate.xml\r\n");

      int update_major = 0;
      int update_minor = 0;

      if(!GetLatestVersionNumber(update_major, update_minor))
         return 0;

      // get the version number from exe
      theApp.m_pDialog->AppendMsg(L"Get the version number from exe\r\n");

      int current_major = 0;
      int current_minor = 0;

      bool bLampExists = GetCurrentVersionNumber(current_major, current_minor);

      // test version
      if(!bLampExists ||
         update_major > current_major ||
         update_minor > current_minor)
      {
         theApp.m_pDialog->AppendMsg(L"Need newer version.\r\n");

         // download new version in zip file format
         theApp.m_pDialog->AppendMsg(L"Downloading newer version.\r\n");

         UCString zipped_temp_file;
         if(!GetNewVersionTempFile(zipped_temp_file))
            return 0;

         // close the Lamp.exe that is running.
         theApp.m_pDialog->AppendMsg(L"Closing Lamp.exe.\r\n");
         
         if(!CloseLamp())
            return 0;
         
         // unzip new version
         theApp.m_pDialog->AppendMsg(L"Unzipping newer version.\r\n");

         if(!UnzipNewVersionTempFile(zipped_temp_file))
            return 0;

         // clean up temp file
         _wunlink(zipped_temp_file);

         // Launch Lamp.exe
         UCString lamp_filename;
         lamp_filename.PathToMe(L"Lamp.exe");

         ShellExecuteW(NULL,L"open",lamp_filename, NULL, NULL, SW_SHOW);

         theApp.m_pDialog->PostMessage(WM_CLOSE);
      }
      else
      {
         theApp.m_pDialog->AppendMsg(L"Up to date.\r\n");
      }
      
      // announce that we are done
      theApp.m_pDialog->AppendMsg(L"Done.  Press OK to exit.\r\n");

   }

   return 0;
}


bool GetLatestVersionNumber(int &major, int &minor)
{
   byte *data = NULL;
   int datasize = 0;
   webclient_download("shacklamp.omgninja.com", "/LampUpdate.xml", "", "", (char**)&data, &datasize);

   if(datasize > 3 &&
      data != NULL)
   {
      UCString buffer;
      if(data[0] == 0xEF &&
         data[1] == 0xBB &&
         data[2] == 0xBF)
      {
         buffer = (char*)(data + 3);
      }
      else
      {
         buffer = (char*)data;
      }
      free(data);

      CXMLTree tree;
      if(tree.ReadFromBuffer(buffer))
      {
         major = tree.GetElementValue(L"MajorVersion");
         minor = tree.GetElementValue(L"MinorVersion");

         UCString msg = L"Latest Version = ";
         msg += major;
         msg += L".";
         msg += minor;
         msg += L"\r\n";
         theApp.m_pDialog->AppendMsg(msg.Str());
         return true;
      }
      else
      {
         theApp.m_pDialog->AppendMsg(L"*** ERROR ***\r\nCould not read from LampUpdate.xml\r\n");
         return false;
      }
   }
   else
   {
      theApp.m_pDialog->AppendMsg(L"*** ERROR ***\r\nCould not download LampUpdate.xml\r\n");
      return false;
   }
}

bool GetCurrentVersionNumber(int &major, int &minor)
{
   UCString lamp_filename;
   lamp_filename.PathToMe(L"Lamp.exe");

   byte *data = NULL;
   DWORD handle = 0;
   DWORD versioninfosize = GetFileVersionInfoSizeW(lamp_filename, &handle);
   if(versioninfosize > 0)
   {
      data = (byte*)malloc(versioninfosize);
      if(data != NULL)
      {
         if(GetFileVersionInfoW(lamp_filename, handle, versioninfosize, data))
         { 
            VS_FIXEDFILEINFO *pFileInfo;
            UINT BufLen;
            if(VerQueryValueW(data, L"\\", (LPVOID *)&pFileInfo, (PUINT)&BufLen))
            {
               major = HIWORD(pFileInfo->dwFileVersionMS);
               minor = LOWORD(pFileInfo->dwFileVersionMS);
               free(data);            
               data = NULL;

               UCString msg = L"Current Version = ";
               msg += major;
               msg += L".";
               msg += minor;
               msg += L"\r\n";
               theApp.m_pDialog->AppendMsg(msg.Str());
               return true;
            }
         }
      }
   }

   if(data != NULL)
   {
      free(data);
   }

   theApp.m_pDialog->AppendMsg(L"*** ERROR ***\r\nCould not get version info from Lamp.exe\r\n");
   return false;
}

bool GetNewVersionTempFile(UCString &zipped_temp_file)
{
   wchar_t path[MAX_PATH+1]={0};
   if(GetTempPath(MAX_PATH, path) != 0)
   {
      if(path[wcslen(path)-1] != L'\\')
      {
         wcscat_s(path,MAX_PATH,L"\\");
      }
         
      zipped_temp_file = path;
      zipped_temp_file += L"LampUpdate.zip";
            
      byte *data = NULL;
      int datasize = 0;
      webclient_download("shacklamp.omgninja.com", "/LampUpdate.zip", "", "", (char**)&data, &datasize);

      if(datasize > 3 &&
         data != NULL)
      {
         FILE *pFile = NULL;
         fopen_s(&pFile,zipped_temp_file.str8(),"wb");
         if(pFile != NULL)
         {
            fwrite(data,datasize,1,pFile);
            fclose(pFile);
         }         
         free(data);
         return true;
      }
      else
      {
         theApp.m_pDialog->AppendMsg(L"*** ERROR ***\r\nCould not download LampUpdate.zip\r\n");
         return false;
      }
   }
   else
   {
      theApp.m_pDialog->AppendMsg(L"*** ERROR ***\r\nCould not find temp path.\r\n");
      return false;
   }
}

bool CloseLamp()
{
   HWND hLamp = ::FindWindow(L"Lamp - Shack Client",NULL);

   if(hLamp != 0)
   {
      int numtries = 5;

      while(hLamp != NULL && numtries > 0)
      {
         ::PostMessage(hLamp,WM_CLOSE,0,0);
         ::Sleep(2000);
         hLamp = ::FindWindow(L"Lamp - Shack Client",NULL);
         numtries--;
      }

      if(hLamp != 0)
      {
         theApp.m_pDialog->AppendMsg(L"*** ERROR ***\r\nCould not close Lamp.exe\r\n");
         return false;
      }
   }

   return true;
}




bool UnzipNewVersionTempFile(const UCString &zipped_temp_file)
{
   bool result = false;

   ZIP_EnableEnumAll( TRUE );

   char filename[10000];

   int zipresult = ZIP_ListOneFile(zipped_temp_file.str8(true), filename, 10000);

   if(zipresult == PK_COOL)
   {
      int enumCount = 0;
      char **pItems = NULL;

      ZIP_GetEnumItemValues( &enumCount, &pItems );

      if(enumCount > 0 &&
         pItems != NULL)
      {
         result = true;
         std::list<UCString> zipitems;

         for(int i = 0; i < enumCount; i++)
         {
            zipitems.push_back(UCString(pItems[i]));
         }

         ZIP_FreeEnumItemValues();
         ZIP_EnableEnumAll( FALSE );

         std::list<UCString>::iterator it = zipitems.begin();
         
         while(it != zipitems.end())
         {
            UCString outname;
            outname.PathToMe(it->Str());
            outname.ReplaceAll(L'/',L'\\');

            if(!outname.endswith(L"\\"))
            {
               os_CreateRecursiveDirectoryPath(outname.GetPath());
               
               zipresult = ZIP_Extract(zipped_temp_file.str8(true),it->str8(),outname.str8(true));
               if(zipresult != PK_COOL)
               {
                  UCString msg = L"*** ERROR ***\r\nCould not exract ";
                  msg += it->Str();
                  msg += L"\r\n";
                  theApp.m_pDialog->AppendMsg(msg);
                  result = false;
                  break;
               }
            }

            it++;
         }
      }
   }

   return result;
}
