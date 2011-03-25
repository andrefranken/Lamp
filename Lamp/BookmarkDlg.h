#pragma once


// CBookmarkDlg dialog

class CBookmarkDlg : public CDialog
{
	DECLARE_DYNAMIC(CBookmarkDlg)

public:
	CBookmarkDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBookmarkDlg();

   bool m_bEditingExistingBM;
   UCString m_Title;

// Dialog Data
	enum { IDD = IDD_BOOKMARK_DIALOG };

protected:
   virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnBnClickedOk();
   afx_msg void OnBnClickedCancel();
};
