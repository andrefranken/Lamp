// FindTextDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lamp.h"
#include "FindTextDlg.h"


// CFindTextDlg dialog

IMPLEMENT_DYNAMIC(CFindTextDlg, CDialog)

CFindTextDlg::CFindTextDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindTextDlg::IDD, pParent)
{

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
   }
   
   OnOK();
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
