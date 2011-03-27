// FindTextDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lamp.h"
#include "FindTextDlg.h"
#include "LampView.h"


// CFindTextDlg dialog

IMPLEMENT_DYNAMIC(CFindTextDlg, CDialog)

CFindTextDlg::CFindTextDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindTextDlg::IDD, pParent)
{
   m_findtext = theApp.GetFindText();
}

CFindTextDlg::~CFindTextDlg()
{
}

void CFindTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFindTextDlg, CDialog)
   ON_BN_CLICKED(IDOK, &CFindTextDlg::OnBnClickedOk)
   ON_BN_CLICKED(ID_FIND_NEXT_BUTTON, &CFindTextDlg::OnBnNext)
   ON_EN_CHANGE(IDC_FIND_EDIT, &CFindTextDlg::OnEditChange)
END_MESSAGE_MAP()


// CFindTextDlg message handlers
void CFindTextDlg::OnBnClickedOk()
{
   CEdit *pThread = (CEdit*)GetDlgItem(IDC_FIND_EDIT);

   if(pThread != NULL)
   {
      CString temp;
      pThread->GetWindowTextW(temp);
      m_findtext = temp;
      theApp.SetFindText(m_findtext);
   }
   
   OnOK();
}

void CFindTextDlg::OnBnNext()
{
   m_pView->GetSelection(m_textselectionpost,m_selectionstart,m_selectionend);
   m_pView->FindNext();
}

void CFindTextDlg::PostNcDestroy()
{
   m_pView->FindDlgClosing();
   delete this;
}

void CFindTextDlg::OnOK()
{
   DestroyWindow();
}

void CFindTextDlg::OnCancel()
{
   DestroyWindow();
}

BOOL CFindTextDlg::OnInitDialog() 
{
   CEdit *pThread = (CEdit*)GetDlgItem(IDC_FIND_EDIT);

   if(pThread != NULL)
   {
      pThread->SetWindowTextW(m_findtext);
   }
   CDialog::OnInitDialog();

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}


void CFindTextDlg::OnEditChange()
{
   CEdit *pThread = (CEdit*)GetDlgItem(IDC_FIND_EDIT);

   if(pThread != NULL)
   {
      CString temp;
      pThread->GetWindowTextW(temp);
      m_findtext = temp;
      theApp.SetFindText(m_findtext);
      m_pView->SetSelection(m_textselectionpost,m_selectionstart,m_selectionend);
      m_pView->FindNext();
   }
}