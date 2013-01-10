
// ChildFrm.h : interface of the CChildFrame class
//

#pragma once

class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

   CLampView *GetView(){return m_pView;}

   void SetView(CLampView *value){m_pView = value;}

// Attributes
public:

// Operations
public:

// Overrides
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   LPCTSTR GetDocumentName(CObject** pObj);

// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
   CLampView *m_pView;

   UCString m_launch;

// Generated message map functions
protected:
   afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()
};
