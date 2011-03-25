// OpenThreadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lamp.h"
#include "OpenThreadDlg.h"


// COpenThreadDlg dialog

IMPLEMENT_DYNAMIC(COpenThreadDlg, CDialog)

COpenThreadDlg::COpenThreadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COpenThreadDlg::IDD, pParent)
{

}

COpenThreadDlg::~COpenThreadDlg()
{
}

void COpenThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COpenThreadDlg, CDialog)
   ON_BN_CLICKED(IDOK, &COpenThreadDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// COpenThreadDlg message handlers

void COpenThreadDlg::OnBnClickedOk()
{
   CEdit *pThread = (CEdit*)GetDlgItem(IDC_THREAD_EDIT);

   if(pThread != NULL)
   {
      CString temp;
      pThread->GetWindowTextW(temp);
      m_path = temp;
   }
   
   OnOK();
}
