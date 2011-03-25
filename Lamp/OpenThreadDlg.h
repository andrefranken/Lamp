#pragma once


// COpenThreadDlg dialog

class COpenThreadDlg : public CDialog
{
	DECLARE_DYNAMIC(COpenThreadDlg)

public:
	COpenThreadDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COpenThreadDlg();

   UCString m_path;

// Dialog Data
	enum { IDD = IDD_OPENTHREAD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnBnClickedOk();
};
