/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.06.09
 */
#pragma once
#include <afxwin.h>
#include <iostream>

#include "Mapping/SceneControl.h"
using namespace std;
using namespace SuperMap;

class SceneView : public CView
{
protected: // create from serialization only
	SceneView() noexcept;

	DECLARE_DYNCREATE(SceneView)

// Implementation
public:
	virtual ~SceneView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Meesage map funcitons
protected:
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
	

	afx_msg void OnSave();

	DECLARE_MESSAGE_MAP()

private:
	SceneControl* m_pSceneControl;
	UINT_PTR mRender3DTimerID;
	bool mIsRenderPaused;
	bool mIsActived;
	bool mIsVisible;

public:
	SceneControl* GetSceneControl();
	void RenderInTimer();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

