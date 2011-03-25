#pragma once


// CFindTextDlg dialog

class CFindTextDlg : public CDialog
{
	DECLARE_DYNAMIC(CFindTextDlg)

public:
	CFindTextDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFindTextDlg();

   UCString m_findtext;

// Dialog Data
	enum { IDD = IDD_FINDTEXT_DIALOG };

protected:
   virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnBnClickedOk();
};
