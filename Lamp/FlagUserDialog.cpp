// FlagUserDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Lamp.h"
#include "FlagUserDialog.h"


// CFlagUserDialog dialog

IMPLEMENT_DYNAMIC(CFlagUserDialog, CDialog)

CFlagUserDialog::CFlagUserDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFlagUserDialog::IDD, pParent)
{

}

CFlagUserDialog::~CFlagUserDialog()
{
}

void CFlagUserDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CFlagUserDialog::OnInitDialog()
{
   CComboBox *pFlagList = (CComboBox*)GetDlgItem(IDC_FLAG_COMBO);

   if(pFlagList != NULL)
   {
      std::map<UCString,CFlagImage> &flagimages = theApp.GetFlagImages();

      std::map<UCString,CFlagImage>::iterator it = flagimages.begin();
      std::map<UCString,CFlagImage>::iterator end = flagimages.end();

      while(it != end)
      {
         pFlagList->AddString(it->first);
         m_flags.push_back(it->first);
         it++;
      }
   }

   CFlaggedUser *flagged_user = theApp.GetFlaggedUser(m_name);
   if(flagged_user != NULL)
   {
      m_flag = flagged_user->m_flag;
      m_note = flagged_user->m_note;

      if(pFlagList != NULL)
      {
         pFlagList->SelectString(0,m_flag);
      }

      CEdit *pNoteEdit = (CEdit*)GetDlgItem(IDC_NOTE_EDIT);

      if(pNoteEdit != NULL)
      {
         pNoteEdit->SetWindowTextW(m_note);
         pNoteEdit->SetSel(0,0,FALSE);
      }
   }
   else // new flagged user
   {
      pFlagList->SelectString(0,L"note");

      CButton *Unflag = (CButton*)GetDlgItem(IDC_UNFLAG_BUTTON);
      if(Unflag != NULL)
      {
         Unflag->EnableWindow(FALSE);
      }
   }

   UCString title = L"Flag User : ";
   title += m_name;
   SetWindowText(title);

   return CDialog::OnInitDialog();
}

void CFlagUserDialog::OnOK()
{
   UCString flag;
   CComboBox *pFlagList = (CComboBox*)GetDlgItem(IDC_FLAG_COMBO);

   if(pFlagList != NULL)
   {
      flag = m_flags[pFlagList->GetCurSel()];
   }

   UCString note;

   CEdit *pNoteEdit = (CEdit*)GetDlgItem(IDC_NOTE_EDIT);

   if(pNoteEdit != NULL)
   {
      CString temp;
      pNoteEdit->GetWindowTextW(temp);
      note = temp;
   }

   CFlaggedUser *flagged_user = theApp.GetFlaggedUser(m_name);
   if(flagged_user != NULL)
   {
      if(flag != m_flag ||
         note != m_note)
      {
         // update existing entry
         theApp.UpdateFlaggedUser(m_name, flag, note);
      }
   }
   else // add new entry
   {
      theApp.AddFlaggedUser(m_name, flag, note);
   }

   CDialog::OnOK();
}

void CFlagUserDialog::OnCancel()
{
   CDialog::OnCancel();
}

BEGIN_MESSAGE_MAP(CFlagUserDialog, CDialog)
   ON_BN_CLICKED(IDC_UNFLAG_BUTTON, OnUnflagButton)
   ON_UPDATE_COMMAND_UI(IDC_UNFLAG_BUTTON, OnUpdateUnflagButton)   
END_MESSAGE_MAP()


// CFlagUserDialog message handlers

void CFlagUserDialog::OnUnflagButton()
{
   int result = MessageBox(L"Are you sure you wish to un-flag this user?", L"Un-Flag", MB_ICONQUESTION | MB_YESNO);

   if(result == IDYES)
   {
      theApp.DeleteFlaggedUser(m_name);
      CDialog::OnCancel();
   }
}

void CFlagUserDialog::OnUpdateUnflagButton(CCmdUI* pCmdUI)
{
   if(m_flag.IsEmpty())
   {
      pCmdUI->Enable(FALSE);
   }
   else
   {
      pCmdUI->Enable(TRUE);
   }   
}