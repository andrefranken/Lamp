
// LampUpdateDlg.h : header file
//

#pragma once

#include "LampUpdate.h"		
#include "UCString.h"		

// CLampUpdateDlg dialog
class CLampUpdateDlg : public CDialog
{
// Construction
public:
	CLampUpdateDlg(CWnd* pParent = NULL);	// standard constructor

   void AppendMsg(const UCChar *msg);

// Dialog Data
	enum { IDD = IDD_LAMPUPDATE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

   UCString m_msg;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
