
// MapView.cpp : implementation of the MapView class
/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.05.13
 */

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "JunSuCpp.h"
#endif

#include "JunSuCppDoc.h"
#include "MapView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CallBacks
void SuCALLBACK InvalidateCallBack(void* pWnd)
{

	// send message to view  for invalidating itself
	//SendDlgItemMessageW(, this, 0x01, NULL,NULL);

	MapView *pView = (MapView *)pWnd;
	if (pView->m_hWnd != NULL && pView->IsWindowVisible()) { // if m_hWnd==NULL, pView.IsWindowVisible() will resoult in an exception
		pView->Invalidate(false); // not redraw background
	}
	
}


// MapView

IMPLEMENT_DYNCREATE(MapView, CView)

BEGIN_MESSAGE_MAP(MapView, CView)
	ON_WM_CONTEXTMENU()
	//ON_WM_RBUTTONUP()

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
	ON_COMMAND(ID_MAP_SAVE, &MapView::OnMapSave)
END_MESSAGE_MAP()

// MapView construction/destruction

MapView::MapView() noexcept
{
	// TODO: add construction code here
	m_pMapControl = new MapControl(InvalidateCallBack, this);
}

MapView::~MapView()
{
	delete m_pMapControl;
}

BOOL MapView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// MapView drawing

void MapView::OnDraw(CDC* /*pDC*/)
{
	JunSuCppDoc* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


//void MapView::OnRButtonUp(UINT /* nFlags */, CPoint point)
//{
//	ClientToScreen(&point);
//	OnContextMenu(this, point);
//}

void MapView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_MAPVIEW, point.x, point.y, this, TRUE);
#endif
}


// MapView diagnostics

#ifdef _DEBUG
void MapView::AssertValid() const
{
	CView::AssertValid();
}

void MapView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

JunSuCppDoc* MapView::GetDocument() const // non-debug version is inline
{
	//ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(JunSuCppDoc)));
	return (JunSuCppDoc*)m_pDocument;
}
#endif //_DEBUG


// MapView message handlers



void MapView::OnPaint()
{

	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(&rect);
	m_pMapControl->OnDraw(rect.left, rect.top, rect.right, rect.bottom, dc.m_hDC);

	CView::OnPaint();
	
}

void MapView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//CPaintDC dc(this); 

	//m_pMapControl->OnMouseMove(dc.m_hDC, nFlags, point.x, point.y);
	m_pMapControl->OnMouseMove(nFlags, point.x, point.y, ::GetDC(this->m_hWnd));
	CView::OnMouseMove(nFlags, point);
}


BOOL MapView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	ScreenToClient(&pt);
	m_pMapControl->OnMouseWheel(nFlags, zDelta, pt.x, pt.y);
	//	Invalidate(false);
		//return CView::OnMouseWheel(nFlags, zDelta, pt);
	return true;
}


void MapView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//CPaintDC dc(this); 
	//m_pMapControl->OnMouseLDown(dc.m_hDC, nFlags, point.x, point.y);
	m_pMapControl->OnLMouseDown(nFlags, point.x, point.y, ::GetDC(this->m_hWnd));
	CView::OnLButtonDown(nFlags, point);
}


void MapView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
// 	CPaintDC dc(this); 
// 	m_pMapControl->OnMouseLUp(dc.m_hDC, nFlags, point.x, point.y);
	m_pMapControl->OnLMouseUp(nFlags, point.x, point.y, ::GetDC(this->m_hWnd));
	CView::OnLButtonUp(nFlags, point);
	//this->Invalidate(FALSE);
}


void MapView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_pMapControl->OnLMouseDbClick(nFlags, point.x, point.y, ::GetDC(this->m_hWnd));
	CView::OnLButtonDblClk(nFlags, point);
}


void MapView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_pMapControl->OnRMouseDown(nFlags, point.x, point.y, ::GetDC(this->m_hWnd));
	CView::OnRButtonDown(nFlags, point);
}


void MapView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_pMapControl->OnRMouseUp(nFlags, point.x, point.y, ::GetDC(this->m_hWnd));
	CView::OnRButtonUp(nFlags, point);
}


void MapView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnRButtonDblClk(nFlags, point);
}


void MapView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	m_pMapControl->OnSizeChanged(cx, cy, ::GetDC(this->m_hWnd));
}

MapControl* MapView::GetMapControl()
{
	return m_pMapControl;
}

void MapView::OnMapSave()
{
	bool isSaved = m_pMapControl->Save();
	if (isSaved) {
		::SendMessage(AfxGetMainWnd()->m_hWnd, JUN_WM_REFRESH_WORKSPACEVIEW, 0, 0);
		//SendMessage(JUN_WM_REFRESH_WORKSPACEVIEW, 0, 0);
	}
	else
	{
		//TODO: ouput log
	}
}
