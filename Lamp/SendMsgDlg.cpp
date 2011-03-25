// SendMsgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lamp.h"
#include "SendMsgDlg.h"


#define WM_INIT_SELECTION               (WM_USER + 70)
// SendMsgDlg dialog

IMPLEMENT_DYNAMIC(SendMsgDlg, CDialog)

SendMsgDlg::SendMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SendMsgDlg::IDD, pParent)
{

}

SendMsgDlg::~SendMsgDlg()
{
}

void SendMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SendMsgDlg, CDialog)
   ON_BN_CLICKED(IDOK, &SendMsgDlg::OnBnClickedOk)
   ON_MESSAGE(WM_INIT_SELECTION,&SendMsgDlg::OnInitSelection)
END_MESSAGE_MAP()


// AuthorDlg message handlers
BOOL SendMsgDlg::OnInitDialog() 
{
   CDialog::OnInitDialog();

   CEdit *pInfo = (CEdit*)GetDlgItem(IDC_TO_EDIT);

   if(pInfo != NULL)
   {
      pInfo->SetWindowTextW(m_to);
   }

   pInfo = (CEdit*)GetDlgItem(IDC_SUBJECT_EDIT);

   if(pInfo != NULL)
   {
      pInfo->SetWindowTextW(m_subject);
   }

   pInfo = (CEdit*)GetDlgItem(IDC_SHACKMSG_EDIT);

   if(pInfo != NULL)
   {
      pInfo->SetWindowTextW(m_shackmsg);
      PostMessageW(WM_INIT_SELECTION);
   }

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void SendMsgDlg::OnBnClickedOk()
{
   CEdit *pInfo = (CEdit*)GetDlgItem(IDC_TO_EDIT);

   if(pInfo != NULL)
   {
      CString temp;
      pInfo->GetWindowTextW(temp);
      m_to = temp;
   }

   pInfo = (CEdit*)GetDlgItem(IDC_SUBJECT_EDIT);

   if(pInfo != NULL)
   {
      CString temp;
      pInfo->GetWindowTextW(temp);
      m_subject = temp;
   }

   pInfo = (CEdit*)GetDlgItem(IDC_SHACKMSG_EDIT);

   if(pInfo != NULL)
   {
      CString temp;
      pInfo->GetWindowTextW(temp);
      m_shackmsg = temp;
   }

   if(m_to.IsEmpty())
   {
      MessageBox(L"Need a Recipient.");
      return;
   }

   if(m_subject.IsEmpty())
   {
      MessageBox(L"Need a Subject.");
      return;
   }

   if(m_shackmsg.IsEmpty())
   {
      MessageBox(L"Need a Message.");
      return;
   }

   std::vector<UCString> recipients;

   if(m_to.Find(L",") != NULL)
   {
      UCString recip;
      int tokenindex = 0;
      while(m_to.GetToken(recip,tokenindex,L','))
      {
         recipients.push_back(recip);
         tokenindex++;
      }
   }
   else if(m_to.Find(L";") != NULL)
   {
      UCString recip;
      int tokenindex = 0;
      while(m_to.GetToken(recip,tokenindex,L';'))
      {
         recipients.push_back(recip);
         tokenindex++;
      }
   }
   else
   {
      m_to.TrimWhitespace();
      recipients.push_back(m_to);
   }

   m_shackmsg.Replace(L"\r",L"");

   if(m_pDoc != NULL)
   {
      for(size_t i = 0; i < recipients.size(); i++)
      {
         m_pDoc->SendMessage(recipients[i], m_subject, m_shackmsg);
      }
   }

   OnOK();
}

LRESULT SendMsgDlg::OnInitSelection(WPARAM wp,LPARAM lp)
{
   CEdit *pInfo = (CEdit*)GetDlgItem(IDC_SHACKMSG_EDIT);

   if(pInfo != NULL)
   {
      pInfo->SetSel(0,0,FALSE);
   }
   return 0;
}