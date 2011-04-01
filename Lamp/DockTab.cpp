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

      theApp.GetMainWnd()->ShowWindow(/*SW_RESTORE*/SW_SHOW);
      
      HWND hWnd = theApp.GetMainWnd()->m_hWnd;

      AttachThreadInput(GetWindowThreadProcessId(::GetForegroundWindow(),NULL),GetCurrentThreadId(),TRUE);
            
      ::SetForegroundWindow(hWnd);	
      ::SetFocus(hWnd);
      
      AttachThreadInput(GetWindowThreadProcessId(::GetForegroundWindow(),NULL),GetCurrentThreadId(),FALSE);
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
