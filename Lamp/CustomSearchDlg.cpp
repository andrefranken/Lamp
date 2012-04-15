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
   CComboBox *pFilter = (CComboBox*)GetDlgItem(IDC_SEARCH_FILTER_COMBO);
   CComboBox *pSort = (CComboBox*)GetDlgItem(IDC_SEARCH_SORT_COMBO);
   
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

   if(pFilter != NULL)
   {
      switch(pFilter->GetCurSel())
      {
      case 0:  m_filter = L"all"; break;
      case 1:  m_filter = L"informative"; break;
      case 2:  m_filter = L"nws"; break;
      default: m_filter = L"all"; break;
      }
   }

   if(pSort != NULL)
   {
      switch(pSort->GetCurSel())
      {
      case 0:  m_sort = L"postdate_desc"; break;
      case 1:  m_sort = L"postdate_asc"; break;
      case 2:  m_sort = L"relevance"; break;
      case 3:  m_sort = L"author"; break;
      default: m_sort = L"all"; break;
      }
   }
   
   OnOK();
}

BOOL CCustomSearchDlg::OnInitDialog() 
{
   CDialog::OnInitDialog();

   CEdit *pTerms = (CEdit*)GetDlgItem(IDC_SEARCH_TERMS_EDIT);
   CEdit *pUser = (CEdit*)GetDlgItem(IDC_SEARCH_USER_EDIT);
   CEdit *pParent = (CEdit*)GetDlgItem(IDC_SEARCH_PARENT_EDIT);
   CComboBox *pFilter = (CComboBox*)GetDlgItem(IDC_SEARCH_FILTER_COMBO);
   CComboBox *pSort = (CComboBox*)GetDlgItem(IDC_SEARCH_SORT_COMBO);
   
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

   if(pFilter != NULL)
   {
      if(m_filter == L"all")
      {
         pFilter->SetCurSel(0);
      }
      else if(m_filter == L"informative")
      {
         pFilter->SetCurSel(1);
      }
      else if(m_filter == L"nws")
      {
         pFilter->SetCurSel(2);
      }
      else
      {
         pFilter->SetCurSel(0);
      }
   }

   if(pSort != NULL)
   {
      if(m_sort == L"postdate_desc")
      {
         pSort->SetCurSel(0);
      }
      else if(m_sort == L"postdate_asc")
      {
         pSort->SetCurSel(1);
      }
      else if(m_sort == L"relevance")
      {
         pSort->SetCurSel(2);
      }
      else if(m_sort == L"author")
      {
         pSort->SetCurSel(3);
      }
      else
      {
         pSort->SetCurSel(0);
      }
   }

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
