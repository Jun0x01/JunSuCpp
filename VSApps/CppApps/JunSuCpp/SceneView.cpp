/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.06.09
 */
#include "stdafx.h"
#include "SceneView.h"
#include "TimerIDs.h"

#ifndef SHARED_HANDLERS
#include "JunSuCpp.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(SceneView, CView)

BEGIN_MESSAGE_MAP(SceneView, CView)
	ON_WM_CONTEXTMENU()

	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_SIZE()
	ON_WM_CREATE()

	ON_COMMAND(ID_MAP_SAVE, &SceneView::OnSave)
END_MESSAGE_MAP()


void CALLBACK TimerRender3D(HWND hWnd, UINT nMsg, UINT_PTR nTimerid, DWORD dwTime)
{

	if (hWnd != NULL) 
	{
		CWnd* pWnd = CWnd::FromHandle(hWnd);
		string targetName = "class SceneView";
		string wndName = typeid(*pWnd).name();
		if (targetName.compare(wndName) == 0) 
		{
			SceneView* sceneView = (SceneView*)pWnd;
			sceneView->RenderInTimer();
		}
	}
}

SceneView::SceneView() noexcept
{
	m_pSceneControl = NULL;
	mIsVisible = false;
	mIsRenderPaused = true;
	// pixels in screen dc
	//HDC hdc = ::GetDC(m_hWnd);

	//int dpiX = GetDeviceCaps(hdc, LOGPIXELSX);

	//int dpiY = GetDeviceCaps(hdc, LOGPIXELSY);

	//::ReleaseDC(NULL, hdc);

	//m_pSceneControl = new SceneControl(m_hWnd, dpiX, dpiY);

	//mRender3DTimerID = ::SetTimer(m_hWnd, TimerIDs::ID_RENDER3D, TimerIDs::PERIOD_RENDER3D, TimerRender3D);
}


SceneView::~SceneView()
{
	::KillTimer(m_hWnd, mRender3DTimerID);
	delete m_pSceneControl;
}

BOOL SceneView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void SceneView::OnDraw(CDC* /*pDC*/)
{
	return;

	// TODO: add draw code for native data here
}

void SceneView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_MAPVIEW, point.x, point.y, this, TRUE);
#endif
}

// SceneView diagnostics

#ifdef _DEBUG
void SceneView::AssertValid() const
{
	CView::AssertValid();
}

void SceneView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// Message Handlers


void SceneView::OnPaint()
{

	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(&rect);
	m_pSceneControl->OnDraw(rect.left, rect.top, rect.right, rect.bottom, dc.m_hDC);

	CView::OnPaint();

}

void SceneView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_pSceneControl->OnMouseMove(nFlags, point.x, point.y, ::GetDC(this->m_hWnd));
	CView::OnMouseMove(nFlags, point);
}


BOOL SceneView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	ScreenToClient(&pt);
	m_pSceneControl->OnMouseWheel(nFlags, zDelta, pt.x, pt.y);
	//	Invalidate(false);
		//return CView::OnMouseWheel(nFlags, zDelta, pt);
	return true;
}


void SceneView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_pSceneControl->OnLMouseDown(nFlags, point.x, point.y, ::GetDC(this->m_hWnd));
	CView::OnLButtonDown(nFlags, point);
}


void SceneView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_pSceneControl->OnLMouseUp(nFlags, point.x, point.y, ::GetDC(this->m_hWnd));
	CView::OnLButtonUp(nFlags, point);
	//this->Invalidate(FALSE);
}


void SceneView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_pSceneControl->OnLMouseDbClick(nFlags, point.x, point.y, ::GetDC(this->m_hWnd));
	CView::OnLButtonDblClk(nFlags, point);
}


void SceneView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_pSceneControl->OnRMouseDown(nFlags, point.x, point.y, ::GetDC(this->m_hWnd));
	CView::OnRButtonDown(nFlags, point);
}


void SceneView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_pSceneControl->OnRMouseUp(nFlags, point.x, point.y, ::GetDC(this->m_hWnd));
	CView::OnRButtonUp(nFlags, point);
}


void SceneView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnRButtonDblClk(nFlags, point);
}


void SceneView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (m_pSceneControl == NULL) {
		// pixels in screen dc
		HDC hdc = ::GetDC(m_hWnd);

		int dpiX = GetDeviceCaps(hdc, LOGPIXELSX);

		int dpiY = GetDeviceCaps(hdc, LOGPIXELSY);

		::ReleaseDC(NULL, hdc);
		
		m_pSceneControl = new SceneControl(m_hWnd, dpiX, dpiY);
		mRender3DTimerID = ::SetTimer(m_hWnd, TimerIDs::ID_RENDER3D, TimerIDs::PERIOD_RENDER3D, TimerRender3D);
	}
	m_pSceneControl->OnSizeChanged(cx, cy, ::GetDC(this->m_hWnd));
	
}

SceneControl* SceneView::GetSceneControl()
{
	return m_pSceneControl;
}

void SceneView::RenderInTimer()
{
	if (NULL != m_pSceneControl)
	{
		mIsVisible = ::IsWindowVisible(m_hWnd);
		if (mIsVisible && !mIsRenderPaused) {
			m_pSceneControl->RenderInTimer();
		}
		
	}
}

int SceneView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	mIsVisible = true;
	mIsRenderPaused = false;

	return 0;
}

void SceneView::OnSave()
{
	bool isSaved = m_pSceneControl->Save();
	if (isSaved) {
		::SendMessage(AfxGetMainWnd()->m_hWnd, JUN_WM_REFRESH_WORKSPACEVIEW, 0, 0);
		//SendMessage(JUN_WM_REFRESH_WORKSPACEVIEW, 0, 0);
	}
	else
	{
		//TODO: ouput log
	}
}
