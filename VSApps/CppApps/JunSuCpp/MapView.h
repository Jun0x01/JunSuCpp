
// MapView.h : interface of the MapView class
/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.05.13
 */

#pragma once
#include "JunSuCppDoc.h"
#include "Mapping/MapControl.h"

using namespace SuperMap;

class MapView : public CView
{
protected: // create from serialization only
	MapView() noexcept;
	DECLARE_DYNCREATE(MapView)

// Attributes
public:
	JunSuCppDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~MapView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	MapControl* m_pMapControl;
	
public:
	MapControl* GetMapControl();
protected:

// Generated message map functions
protected:
	//afx_msg void OnFilePrintPreview();
	//afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	afx_msg void OnPaint();

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMapSave();
};

#ifndef _DEBUG  // debug version in MapView.cpp
inline JunSuCppDoc* MapView::GetDocument() const
   { return reinterpret_cast<JunSuCppDoc*>(m_pDocument); }
#endif

