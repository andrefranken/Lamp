// BookmarkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lamp.h"
#include "BookmarkDlg.h"


// CBookmarkDlg dialog

IMPLEMENT_DYNAMIC(CBookmarkDlg, CDialog)

CBookmarkDlg::CBookmarkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBookmarkDlg::IDD, pParent)
{
   m_bEditingExistingBM = false;
}

CBookmarkDlg::~CBookmarkDlg()
{
}

void CBookmarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBookmarkDlg, CDialog)
   ON_BN_CLICKED(IDOK, &CBookmarkDlg::OnBnClickedOk)
   ON_BN_CLICKED(IDCANCEL, &CBookmarkDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CBookmarkDlg message handlers

void CBookmarkDlg::OnBnClickedOk()
{
   CEdit *pTitle = (CEdit*)GetDlgItem(IDC_BM_TITLE_EDIT);

   if(pTitle != NULL)
   {
      CString wndtext;
      pTitle->GetWindowTextW(wndtext);
      m_Title = wndtext;
   }

   OnOK();
}

void CBookmarkDlg::OnBnClickedCancel()
{
   OnCancel();
}

BOOL CBookmarkDlg::OnInitDialog() 
{
   CEdit *pTitle = (CEdit*)GetDlgItem(IDC_BM_TITLE_EDIT);

   if(pTitle != NULL)
   {
      pTitle->SetWindowTextW(m_Title);
   }

   if(m_bEditingExistingBM)
   {
      CButton *pCancel = (CButton*)GetDlgItem(IDCANCEL);
      if(pCancel)
      {
         pCancel->SetWindowTextW(L"Delete");
      }
   }

   CDialog::OnInitDialog();

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
