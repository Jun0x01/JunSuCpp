/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.06.09
 */
#include "stdafx.h"
#include "SceneFrame.h"

#include "SceneView.h"
#include "JunSuMainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(SceneFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(SceneFrame, CMDIChildWndEx)
	ON_WM_MDIACTIVATE()
END_MESSAGE_MAP()

SceneFrame::SceneFrame() noexcept
{
	status = false;
}


SceneFrame::~SceneFrame()
{
}


BOOL SceneFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if (!CMDIChildWndEx::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}

#ifdef _DEBUG
void SceneFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void SceneFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// Messge Handlers
void SceneFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	// 监听当前是否被激活，激活了更新LayerTreeView
	void* pt = this;

	if (pt == pActivateWnd)
	{
		// active
		if (!status)
		{
			status = bActivate;
			SceneView* pView = (SceneView*)GetActiveView();
			JunSuMainFrame* pMainWnd = (JunSuMainFrame*)AfxGetMainWnd();
			pMainWnd->UpdateSceneLayersView(pView->GetSceneControl());

			pView->GetSceneControl()->ActivateScene();
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
			pMainWnd->UpdateSceneLayersView(NULL);
		}

	}
}


