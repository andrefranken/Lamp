#pragma once


// CCustomSearchDlg dialog

class CCustomSearchDlg : public CDialog
{
	DECLARE_DYNAMIC(CCustomSearchDlg)

public:
	CCustomSearchDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCustomSearchDlg();

   UCString m_terms;
   UCString m_user;
   UCString m_parent;

// Dialog Data
	enum { IDD = IDD_SEARCH_DIALOG };

protected:
   virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnBnClickedOk();
};
