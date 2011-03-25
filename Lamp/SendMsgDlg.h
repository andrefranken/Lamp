#pragma once

#include "LampDoc.h"

// SendMsgDlg dialog

class SendMsgDlg : public CDialog
{
	DECLARE_DYNAMIC(SendMsgDlg)

public:
	SendMsgDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~SendMsgDlg();

   UCString m_to;
   UCString m_subject;
   UCString m_shackmsg;
   CLampDoc *m_pDoc;

// Dialog Data
	enum { IDD = IDD_SENDMSG_DIALOG };

protected:
   virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnBnClickedOk();
   afx_msg LRESULT OnInitSelection(WPARAM wp,LPARAM lp);
};
