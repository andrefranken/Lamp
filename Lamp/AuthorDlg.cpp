// AuthorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "comm.h"
#include "html.h"
#include "Lamp.h"
#include "AuthorDlg.h"


// AuthorDlg dialog

IMPLEMENT_DYNAMIC(AuthorDlg, CDialog)

AuthorDlg::AuthorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(AuthorDlg::IDD, pParent)
{
   m_bSendMessage = false;
}

AuthorDlg::~AuthorDlg()
{
}

void AuthorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AuthorDlg, CDialog)
   ON_BN_CLICKED(ID_AUTHORDLG_SEARCH_BUTTON, &AuthorDlg::OnBnClickedSearchButton)
   ON_BN_CLICKED(ID_AUTHORDLG_MESSAGE_BUTTON, &AuthorDlg::OnBnClickedMessageButton)
END_MESSAGE_MAP()


// AuthorDlg message handlers
BOOL AuthorDlg::OnInitDialog() 
{
   CEdit *pInfo = (CEdit*)GetDlgItem(IDC_INFO_EDIT);

   if(pInfo != NULL)
   {
      pInfo->SetWindowTextW(m_info);
   }

   SetWindowTextW(m_author);

   if(m_bReplyIsUp)
   {
      CButton *pButton = (CButton*)GetDlgItem(ID_AUTHORDLG_MESSAGE_BUTTON);
      if(pButton != NULL)
      {
         pButton->EnableWindow(0);
      }
   }

   CDialog::OnInitDialog();

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void AuthorDlg::OnBnClickedSearchButton()
{
   if(!m_author.IsEmpty())
   {
      theApp.SetLastSearchParms(m_author, UCString(), UCString(), UCString(), UCString());

      UCString path = L"CUSTOMSEARCH:";
      char *enc = url_encode(m_author.str8());
      path += enc;
      free(enc);
      path += L":::all:postdate_desc";

      theApp.OpenDocumentFile(path);
      PostMessageW(WM_CLOSE);
   }
}

void AuthorDlg::OnBnClickedMessageButton()
{
   if(!m_author.IsEmpty())
   {
      m_bSendMessage = true;
      PostMessageW(WM_CLOSE);
   }
}