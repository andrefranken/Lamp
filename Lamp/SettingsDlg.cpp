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
   CEdit *pFilteredUsersEdit = (CEdit*)GetDlgItem(IDC_FILTERED_USERS_EDIT);
   CEdit *pFilteredPhrasesEdit = (CEdit*)GetDlgItem(IDC_FILTERED_PHRASES_EDIT);
   CEdit *ptMSecondsPreviewTimer = (CEdit*)GetDlgItem(IDC_PREVIEW_TIMER);
   CEdit *pPreviewExpandPercentEdit = (CEdit*)GetDlgItem(IDC_PREVIEW_EXCPAND_PERCENT_EDIT);
   CEdit *pUghThreshEdit = (CEdit*)GetDlgItem(ID_UGH_THRESH);
   CEdit *pNumSumLinesEdit = (CEdit*)GetDlgItem(IDC_NUM_SUMMARY_LINES);

   if(pMouseWheelSpeed != NULL &&
      pMBPanSpeed != NULL &&
      pNumShowTrunc != NULL &&
      pNumMinsInbox != NULL &&
      pInertia != NULL &&
      pHoursExpire != NULL &&
      pUseStoneDonkey != NULL &&
      pUseSingleThreadStyle != NULL &&
      pFilteredUsersEdit != NULL &&
      pFilteredPhrasesEdit != NULL &&
      ptMSecondsPreviewTimer != NULL &&
      pPreviewExpandPercentEdit != NULL &&
      pUghThreshEdit != NULL &&
      pNumSumLinesEdit != NULL)
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
      theApp.SetNumMinutesUpdateTab(temp2);

      pInertia->GetWindowTextW(temp);
      temp2 = temp;
      theApp.SetInertiaFriction(temp2);

      pHoursExpire->GetWindowTextW(temp);
      temp2 = temp;
      theApp.SetHoursExpire(temp2);

      ptMSecondsPreviewTimer->GetWindowTextW(temp);
      temp2 = temp;
      theApp.SetMSecondsPreviewTimer(temp2);

      pPreviewExpandPercentEdit->GetWindowTextW(temp);
      temp2 = temp;
      theApp.SetHoverPreviewPercentStepsize(temp2);

      pUghThreshEdit->GetWindowTextW(temp);
      temp2 = temp;
      theApp.SetUGHThreshold(temp2);

      pNumSumLinesEdit->GetWindowTextW(temp);
      temp2 = temp;
      theApp.MaxSummaryLines(temp2);

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

      pFilteredUsersEdit->GetWindowTextW(temp);
      temp2 = temp;

      std::set<UCString> &ful = theApp.GetFilteredUsernameList();
      ful.clear();

      temp2.ReplaceAll(L'\r',L'\n');
      temp2.Replace(L"\n\n",L"\n");

      UCString username;
      int tokenindex = 0;
      while(temp2.GetToken(username,tokenindex,L'\n'))
      {
         if(!username.IsEmpty())
         {
            ful.insert(username);
         }
         tokenindex++;
         username = L"";
      }

      pFilteredPhrasesEdit->GetWindowTextW(temp);
      temp2 = temp;

      std::set<UCString> &fpl = theApp.GetFilteredPhraseList();
      fpl.clear();

      temp2.ReplaceAll(L'\r',L'\n');
      temp2.Replace(L"\n\n",L"\n");

      UCString phrase;
      tokenindex = 0;
      while(temp2.GetToken(phrase,tokenindex,L'\n'))
      {
         if(!phrase.IsEmpty())
         {
            fpl.insert(phrase);
         }
         tokenindex++;
         phrase = L"";
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
   CEdit *pFilteredUsersEdit = (CEdit*)GetDlgItem(IDC_FILTERED_USERS_EDIT);
   CEdit *pFilteredPhrasesEdit = (CEdit*)GetDlgItem(IDC_FILTERED_PHRASES_EDIT);
   CEdit *ptMSecondsPreviewTimer = (CEdit*)GetDlgItem(IDC_PREVIEW_TIMER);
   CEdit *pPreviewExpandPercentEdit = (CEdit*)GetDlgItem(IDC_PREVIEW_EXCPAND_PERCENT_EDIT);
   CEdit *pUghThreshEdit = (CEdit*)GetDlgItem(ID_UGH_THRESH);
   CEdit *pNumSumLinesEdit = (CEdit*)GetDlgItem(IDC_NUM_SUMMARY_LINES);

   if(pMouseWheelSpeed != NULL &&
      pMBPanSpeed != NULL &&
      pNumShowTrunc != NULL &&
      pNumMinsInbox != NULL &&
      pInertia != NULL &&
      pHoursExpire != NULL &&
      pUseStoneDonkey != NULL &&
      pUseSingleThreadStyle != NULL &&
      pFilteredUsersEdit != NULL &&
      pFilteredPhrasesEdit != NULL &&
      ptMSecondsPreviewTimer != NULL &&
      pPreviewExpandPercentEdit != NULL &&
      pUghThreshEdit != NULL &&
      pNumSumLinesEdit != NULL)
   {
      pMouseWheelSpeed->SetWindowTextW(UCString(theApp.GetMouseWheelScale()));
      pMBPanSpeed->SetWindowTextW(UCString(theApp.GetMBPanScale()));
      pNumShowTrunc->SetWindowTextW(UCString(theApp.GetNumToShowWhenTruncated()));
      pNumMinsInbox->SetWindowTextW(UCString(theApp.GetNumMinutesUpdateTab()));
      pInertia->SetWindowTextW(UCString(theApp.GetInertiaFriction()));
      pHoursExpire->SetWindowTextW(UCString(theApp.GetHoursExpire()));
      ptMSecondsPreviewTimer->SetWindowTextW(UCString(theApp.GetMSecondsPreviewTimer()));
      pPreviewExpandPercentEdit->SetWindowTextW(UCString(theApp.GetHoverPreviewPercentStepsize()));
      pUghThreshEdit->SetWindowTextW(UCString(theApp.GetUGHThreshold()));
      pNumSumLinesEdit->SetWindowTextW(UCString(theApp.MaxSummaryLines()));

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

      std::set<UCString> &ful = theApp.GetFilteredUsernameList();

      UCString listtext;

      std::set<UCString>::iterator it = ful.begin();
      std::set<UCString>::iterator end = ful.end();

      while(it != end)
      {
         listtext += (*it);
         listtext += L"\r\n";
         it++;
      }

      pFilteredUsersEdit->SetWindowTextW(listtext);

      std::set<UCString> &fpl = theApp.GetFilteredPhraseList();

      listtext = L"";

      it = fpl.begin();
      end = fpl.end();

      while(it != end)
      {
         listtext += (*it);
         listtext += L"\r\n";
         it++;
      }

      pFilteredPhrasesEdit->SetWindowTextW(listtext);
   }

   CDialog::OnInitDialog();

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
