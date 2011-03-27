#pragma once


// CFindTextDlg dialog

class CFindTextDlg : public CDialog
{
	DECLARE_DYNAMIC(CFindTextDlg)

public:
	CFindTextDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFindTextDlg();

   UCString m_findtext;
   CLampView *m_pView;
   unsigned int m_textselectionpost;
   int m_selectionstart;
   int m_selectionend;

// Dialog Data
	enum { IDD = IDD_FINDTEXT_DIALOG };

protected:
   virtual void PostNcDestroy();   // default to delete this.
   virtual void OnOK();
   virtual void OnCancel();
   virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnBnClickedOk();
   afx_msg void OnBnNext();
   afx_msg void OnEditChange();
};
