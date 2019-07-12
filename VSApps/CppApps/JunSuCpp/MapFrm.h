
// MapFrm.h : interface of the MapFrame class
/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.05.13
 */

#pragma once

class MapFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(MapFrame)
public:
	MapFrame() noexcept;

// Attributes
protected:
	CSplitterWndEx m_wndSplitter;
public:

// Operations
public:

// Overrides
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~MapFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	DECLARE_MESSAGE_MAP()

private:
	boolean status;
};
