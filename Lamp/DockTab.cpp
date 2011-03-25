// DockTab.cpp : implementation file
//

#include "stdafx.h"
#include "Lamp.h"
#include "DockTab.h"


// DockTab dialog

IMPLEMENT_DYNAMIC(DockTab, CDialog)

DockTab::DockTab(CWnd* pParent /*=NULL*/)
	: CDialog(DockTab::IDD, pParent)
{

}

DockTab::~DockTab()
{
}

void DockTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DockTab, CDialog)
   ON_WM_MOUSEMOVE()
   ON_WM_PAINT()
END_MESSAGE_MAP()


// DockTab message handlers

void DockTab::OnMouseMove(UINT nFlags, CPoint point) 
{
   if(theApp.m_bDockedMode &&
      theApp.m_bCollapsed)
   {
      theApp.m_bCollapsed = false;

      if(theApp.m_pDockTab != NULL)
      {
         theApp.m_pDockTab->ShowWindow(SW_HIDE);
      }

      theApp.GetMainWnd()->ShowWindow(SW_RESTORE/*SW_SHOW*/);
      
      HWND hWnd = theApp.GetMainWnd()->m_hWnd;
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
      /*
      ::MoveWindow(hWnd,
                   theApp.m_dockedrect.left,
                   theApp.m_dockedrect.top,
                   theApp.m_dockedrect.right - theApp.m_dockedrect.left,
                   theApp.m_dockedrect.bottom - theApp.m_dockedrect.top, 
                   TRUE);
      */
      if(dwThisTID != dwCurrTID)	
      {		
         ::SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT,0,(PVOID)lockTimeOut,SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE);		
         ::AttachThreadInput(dwThisTID, dwCurrTID, FALSE);	
      }
   }

   CDialog::OnMouseMove(nFlags, point);
}

void DockTab::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

   HDC hDC = dc.m_hDC;

   RECT rect;
   GetClientRect(&rect);

   theApp.GetDockTabImage()->Blit(hDC,rect);
}
