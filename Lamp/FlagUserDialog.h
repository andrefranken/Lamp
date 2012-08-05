#pragma once


// CFlagUserDialog dialog

class CFlagUserDialog : public CDialog
{
	DECLARE_DYNAMIC(CFlagUserDialog)

public:
	CFlagUserDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFlagUserDialog();

   UCString m_name;
   UCString m_flag;
   UCString m_note;

// Dialog Data
	enum { IDD = IDD_FLAG_USER_DIALOG };

   virtual BOOL OnInitDialog();
	
protected:
   std::vector<UCString> m_flags;

	virtual void OnOK();
	virtual void OnCancel();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

   afx_msg void OnUnflagButton();
   afx_msg void OnUpdateUnflagButton(CCmdUI* pCmdUI);
};
