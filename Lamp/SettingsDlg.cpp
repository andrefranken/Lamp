// SettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lamp.h"
#include "SettingsDlg.h"
#include "comm.h"

// CSettingsDlg dialog

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialog)

CSettingsDlg::CSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingsDlg::IDD, pParent)
{

}

CSettingsDlg::~CSettingsDlg()
{
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialog)
   ON_BN_CLICKED(IDOK, &CSettingsDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSettingsDlg message handlers
void CSettingsDlg::OnBnClickedOk()
{
   CEdit *pMouseWheelSpeed = (CEdit*)GetDlgItem(IDC_MOUSE_WHEEL_SPEED_EDIT);
   CEdit *pMBPanSpeed = (CEdit*)GetDlgItem(IDC_MB_PAN_SPEED_EDIT);
   CEdit *pNumShowTrunc = (CEdit*)GetDlgItem(IDC_NUM_SHOW_TRUNC_EDIT);
   CEdit *pNumMinsInbox = (CEdit*)GetDlgItem(IDC_NUM_MIN_INBOX_EDIT);
   CEdit *pInertia = (CEdit*)GetDlgItem(IDC_INERTIA_EDIT);
   CEdit *pHoursExpire = (CEdit*)GetDlgItem(IDC_NUM_HOURS_EXPIRE_EDIT);
   CButton *pUseStoneDonkey = (CButton*)GetDlgItem(ID_USE_STONEDONKEY);
   CButton *pUseSingleThreadStyle = (CButton*)GetDlgItem(ID_SINGLE_THREAD_STYLE);

   if(pMouseWheelSpeed != NULL &&
      pMBPanSpeed != NULL &&
      pNumShowTrunc != NULL &&
      pNumMinsInbox != NULL &&
      pInertia != NULL &&
      pHoursExpire != NULL &&
      pUseStoneDonkey != NULL &&
      pUseSingleThreadStyle != NULL)
   {
      CString temp;
      UCString temp2;
      pMouseWheelSpeed->GetWindowTextW(temp);
      temp2 = temp;
      theApp.SetMouseWheelScale(temp2);

      pMBPanSpeed->GetWindowTextW(temp);
      temp2 = temp;
      theApp.SetMBPanScale(temp2);

      pNumShowTrunc->GetWindowTextW(temp);
      temp2 = temp;
      theApp.SetNumToShowWhenTruncated(temp2);

      pNumMinsInbox->GetWindowTextW(temp);
      temp2 = temp;
      theApp.SetNumMinutesCheckInbox(temp2);

      pInertia->GetWindowTextW(temp);
      temp2 = temp;
      theApp.SetInertiaFriction(temp2);

      pHoursExpire->GetWindowTextW(temp);
      temp2 = temp;
      theApp.SetHoursExpire(temp2);

      if(pUseStoneDonkey->GetCheck() == BST_CHECKED)
      {
         theApp.SetUseShack(false);
      }
      else
      {
         theApp.SetUseShack(true);
      }

      if(pUseSingleThreadStyle->GetCheck() == BST_CHECKED)
      {
         theApp.SetSingleThreadStyle(true);
      }
      else
      {
         theApp.SetSingleThreadStyle(false);
      }
   }

   OnOK();
}

BOOL CSettingsDlg::OnInitDialog() 
{
   CEdit *pMouseWheelSpeed = (CEdit*)GetDlgItem(IDC_MOUSE_WHEEL_SPEED_EDIT);
   CEdit *pMBPanSpeed = (CEdit*)GetDlgItem(IDC_MB_PAN_SPEED_EDIT);
   CEdit *pNumShowTrunc = (CEdit*)GetDlgItem(IDC_NUM_SHOW_TRUNC_EDIT);
   CEdit *pNumMinsInbox = (CEdit*)GetDlgItem(IDC_NUM_MIN_INBOX_EDIT);
   CEdit *pInertia = (CEdit*)GetDlgItem(IDC_INERTIA_EDIT);
   CEdit *pHoursExpire = (CEdit*)GetDlgItem(IDC_NUM_HOURS_EXPIRE_EDIT);
   CButton *pUseStoneDonkey = (CButton*)GetDlgItem(ID_USE_STONEDONKEY);
   CButton *pUseSingleThreadStyle = (CButton*)GetDlgItem(ID_SINGLE_THREAD_STYLE);

   if(pMouseWheelSpeed != NULL &&
      pMBPanSpeed != NULL &&
      pNumShowTrunc != NULL &&
      pNumMinsInbox != NULL &&
      pInertia != NULL &&
      pHoursExpire != NULL &&
      pUseStoneDonkey != NULL &&
      pUseSingleThreadStyle != NULL)
   {
      pMouseWheelSpeed->SetWindowTextW(UCString(theApp.GetMouseWheelScale()));
      pMBPanSpeed->SetWindowTextW(UCString(theApp.GetMBPanScale()));
      pNumShowTrunc->SetWindowTextW(UCString(theApp.GetNumToShowWhenTruncated()));
      pNumMinsInbox->SetWindowTextW(UCString(theApp.GetNumMinutesCheckInbox()));
      pInertia->SetWindowTextW(UCString(theApp.GetInertiaFriction()));
      pHoursExpire->SetWindowTextW(UCString(theApp.GetHoursExpire()));

      if(theApp.UseShack())
      {
         pUseStoneDonkey->SetCheck(BST_UNCHECKED);
      }
      else
      {
         pUseStoneDonkey->SetCheck(BST_CHECKED);
      }

      if(theApp.SingleThreadStyle())
      {
         pUseSingleThreadStyle->SetCheck(BST_CHECKED);
      }
      else
      {
         pUseSingleThreadStyle->SetCheck(BST_UNCHECKED);
      }
   }

   CDialog::OnInitDialog();

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
