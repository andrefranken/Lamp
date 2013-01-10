
// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "Lamp.h"
#include "LampView.h"
#include "LampDoc.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
   ON_WM_CLOSE()
END_MESSAGE_MAP()

// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

LPCTSTR CChildFrame::GetDocumentName(CObject** pObj)
{
   LPCTSTR result = NULL;

   if(GetView() != NULL &&
      GetView()->GetDocument() != NULL)
   {
      GetView()->GetDocument()->GetLaunchString(m_launch, GetView()->GetCurrentId());
      result = m_launch;
   }

   return result;
}

// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame message handlers
void CChildFrame::OnClose() 
{
   CMDIChildWndEx::OnClose();

   theApp.GetMainWnd()->PostMessageW(WM_EXPAND_TABS,0,0);

}