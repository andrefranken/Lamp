#pragma once


// AuthorDlg dialog

class AuthorDlg : public CDialog
{
	DECLARE_DYNAMIC(AuthorDlg)

public:
	AuthorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~AuthorDlg();

   UCString m_info;
   UCString m_author;
   bool m_bSendMessage;

// Dialog Data
	enum { IDD = IDD_AUTHOR_DIALOG };

protected:
   virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnBnClickedSearchButton();
   afx_msg void OnBnClickedMessageButton();
};
