/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.06.09
 */
#pragma once

class SceneFrame :public CMDIChildWndEx
{
	DECLARE_DYNCREATE(SceneFrame)
public:
	SceneFrame() noexcept;
	virtual ~SceneFrame();
// Attributes
protected:
	CSplitterWndEx m_wndSplitter;

// Operations
public:

// Overrides
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivatedWnd, CWnd* pDeactivatedWnd);

	DECLARE_MESSAGE_MAP()

private:
	boolean status;
};

