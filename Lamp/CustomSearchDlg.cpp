// CustomSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lamp.h"
#include "CustomSearchDlg.h"


// CCustomSearchDlg dialog

IMPLEMENT_DYNAMIC(CCustomSearchDlg, CDialog)

CCustomSearchDlg::CCustomSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCustomSearchDlg::IDD, pParent)
{

}

CCustomSearchDlg::~CCustomSearchDlg()
{
}

void CCustomSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCustomSearchDlg, CDialog)
   ON_BN_CLICKED(IDOK, &CCustomSearchDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCustomSearchDlg message handlers
void CCustomSearchDlg::OnBnClickedOk()
{
   CEdit *pTerms = (CEdit*)GetDlgItem(IDC_SEARCH_TERMS_EDIT);
   CEdit *pUser = (CEdit*)GetDlgItem(IDC_SEARCH_USER_EDIT);
   CEdit *pParent = (CEdit*)GetDlgItem(IDC_SEARCH_PARENT_EDIT);
   
   if(pTerms != NULL)
   {
      CString temp;
      pTerms->GetWindowTextW(temp);
      m_terms = temp;
   }

   if(pUser != NULL)
   {
      CString temp;
      pUser->GetWindowTextW(temp);
      m_user = temp;
   }

   if(pParent != NULL)
   {
      CString temp;
      pParent->GetWindowTextW(temp);
      m_parent = temp;
   }
   
   OnOK();
}

BOOL CCustomSearchDlg::OnInitDialog() 
{
   CEdit *pTerms = (CEdit*)GetDlgItem(IDC_SEARCH_TERMS_EDIT);
   CEdit *pUser = (CEdit*)GetDlgItem(IDC_SEARCH_USER_EDIT);
   CEdit *pParent = (CEdit*)GetDlgItem(IDC_SEARCH_PARENT_EDIT);
   
   if(pTerms != NULL)
   {
      pTerms->SetWindowTextW(m_terms);
   }

   if(pUser != NULL)
   {
      pUser->SetWindowTextW(m_user);
   }

   if(pParent != NULL)
   {
      pParent->SetWindowTextW(m_parent);
   }

   CDialog::OnInitDialog();

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
