// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lamp.h"
#include "LoginDlg.h"
#include "comm.h"

// CLoginDlg dialog

IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
   ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLoginDlg message handlers
void CLoginDlg::OnBnClickedOk()
{
   CEdit *pUsername = (CEdit*)GetDlgItem(IDC_USERNAME_EDIT);
   CEdit *pPassword = (CEdit*)GetDlgItem(IDC_PASSWORD_EDIT);

   if(pUsername != NULL &&
      pPassword != NULL)
   {
      CString temp;
      pUsername->GetWindowTextW(temp);
      m_username = temp;
      pPassword->GetWindowTextW(temp);
      m_password = temp;
   }
   
   OnOK();
}

BOOL CLoginDlg::OnInitDialog() 
{
   CEdit *pUsername = (CEdit*)GetDlgItem(IDC_USERNAME_EDIT);
   CEdit *pPassword = (CEdit*)GetDlgItem(IDC_PASSWORD_EDIT);

   if(pUsername != NULL &&
      pPassword != NULL)
   {
      pUsername->SetWindowTextW(m_username);
      pPassword->SetWindowTextW(m_password);
   }
   CDialog::OnInitDialog();

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
