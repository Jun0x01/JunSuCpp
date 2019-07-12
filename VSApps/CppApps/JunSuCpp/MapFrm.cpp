
// MapFrm.cpp : implementation of the MapFrame class
/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.05.13
 */

#include "stdafx.h"
#include "JunSuCpp.h"

#include "MapFrm.h"
#include "MapView.h"
#include "JunSuMainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// MapFrame

IMPLEMENT_DYNCREATE(MapFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(MapFrame, CMDIChildWndEx)
	ON_WM_MDIACTIVATE()
END_MESSAGE_MAP()

// MapFrame construction/destruction

MapFrame::MapFrame() noexcept
{
	status = false;
}

MapFrame::~MapFrame()
{
}


BOOL MapFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// MapFrame diagnostics

#ifdef _DEBUG
void MapFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void MapFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// MapFrame message handlers

void MapFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	// 监听当前是否被激活，激活了更新LayerTreeView
	void* pt = this;
	
	if (pt == pActivateWnd)
	{
		// active
		if (!status)
		{
			status = bActivate;
			MapView* pView = (MapView*) GetActiveView();
			JunSuMainFrame* pMainWnd = (JunSuMainFrame*)AfxGetMainWnd();
			pMainWnd->UpdateLayersView(pView->GetMapControl());
			//pView->Invalidate(false); // 激活时刷新
		}
		
	}
	else if (pt == pDeactivateWnd)
	{
		// deactive
		if (status)
		{
			status = bActivate;
		}

		if (pActivateWnd == NULL)
		{
			JunSuMainFrame* pMainWnd = (JunSuMainFrame*)AfxGetMainWnd();
			pMainWnd->UpdateLayersView(NULL);
		}
		
	}
}