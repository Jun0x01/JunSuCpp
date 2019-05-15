/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.05.15
 */
#include "Mapping/MapControl.h"


using namespace SuperMap;

/**************** Invalidate CallBack *********************/
void SuCALLBACK InvalidateCallback(void * p)
{
	MapControl* pThis = (MapControl*)p;

  	pThis->Invalidate();

}

MapControl::MapControl(INVALIDATEPROC pInvalidateCallBack, void* pView)
{
	m_pUGMapWnd = NULL;
	m_pGraphicsImage = NULL;
	m_pGraphicsImageOld = NULL;
	m_pInvalidateCallback = NULL;

	m_IsMinSized = false;

	m_pWnd = pView;
	m_pInvalidateCallback = pInvalidateCallBack;
	//Initialize(pInvalidateCallBack, pView);
	Initialize(InvalidateCallback, this);

	UGWorkspace* pWorkspace = m_pUGMapWnd->m_mapWnd.m_Map.GetWorkspace();
	if(pWorkspace == NULL){
		pWorkspace = new UGWorkspace();
		m_pUGMapWnd->m_mapWnd.m_Map.SetWorkspace(pWorkspace);
	}

	mNeedRedraw = false;

}
MapControl::~MapControl()
{
	if (m_pUGMapWnd != NULL) 
	{
		// close map and datasource
	}
	delete m_pGraphicsImage;
	m_pGraphicsImage = NULL;

	//delete m_pGraphicsImageOld;
	//m_pGraphicsImageOld = NULL;

	delete m_pUGMapWnd;
	m_pUGMapWnd = NULL;

	m_pInvalidateCallback = NULL;
}

void MapControl::Initialize(INVALIDATEPROC pCallBack, void* pView)
{
	// set graphics engine type
	UGGraphicsManager::SetCurGraphicsType(UGC::UGGraphics::GT_Windows);

	m_pUGMapWnd = new UGMapEditorWnd();
	m_pUGMapWnd->Initialize();

	// initialize UGMap
	UGMap* pUGMap = &(m_pUGMapWnd->m_mapWnd.m_Map);
	pUGMap->m_strName = _U("UnTitledMap");
	pUGMap->SetDescription(_U(""));
	pUGMap->SetAngle(0.0);
	pUGMap->SetColorMode(UGDrawParamaters::UGMapColorMode::cmDefault);
	pUGMap->SetLineSmoothingMode(true);
	pUGMap->SetTextSmoothingMode(true);
	pUGMap->SetTextFixedAngle(false);
	pUGMap->SetFixedTextOrientation(false);
	pUGMap->SetMarkerFixedAngle(false);
	pUGMap->SetPaintBackground(true);
	pUGMap->SetScale(0);
	pUGMap->GetDrawing()->m_DrawParam.SetDrawBoundsLocked(false);
	pUGMap->GetDrawing()->m_DrawParam.SetLockedDrawBounds(UGRect2D(0,0,0,0));
	pUGMap->SetModifiedFlag(false);

	// default settings
	m_pUGMapWnd->SetUserAction(UGDrawParamaters::UGMapUserAction::uaPan); // uaPointModeSelect
	m_pUGMapWnd->SetMarginPanEnable(true); // auto scrolling
	m_pUGMapWnd->SetMarginPanPercent(0.5);
	m_pUGMapWnd->SetSelectionTolerance(1);
	m_pUGMapWnd->SetSelectionMode(UGMapWnd::smContainCentroid);
	m_pUGMapWnd->SetOpenEditMode(true);

	m_pUGMapWnd->SetInvisibleSelected(false);

	// Callbacks
	// set invalidate callback for notifying view refresh
	m_pUGMapWnd->m_mapWnd.SetInvalidateFunc(pCallBack, pView);
	

}

void MapControl::Invalidate(bool reDraw)
{
	if (m_pInvalidateCallback != NULL && m_pWnd != NULL)
	{
		mNeedRedraw = reDraw;
		m_pInvalidateCallback(m_pWnd);
	}
}

UGuint MapControl::getKeyUGFlags(unsigned int flag)
{
	UGint flags = 0;
	if(flag == UG_MK_CONTROL || flag== UG_MK_SHIFT){
		flags = flag;
	}
// 	if(flag == UG_MK_LBUTTON){
// 	    flags = flag;
// 	}

	return flags;

}

UGGraphics* MapControl::CreateUGGraphics(void* pHDC)
{
	UGbool isAlphaEnable = m_pUGMapWnd->m_mapWnd.m_Map.IsGraphicsAlphaEnable();
	UGGraphics* pGraphics = UGGraphicsManager::NewGraphics();

	if (pHDC != NULL)
	{
		// Windows, MFC, .Net -> CWnd's pHDC
		UGGraphics::UGGraphicsData data;
		data.pNative = pHDC;
		data.pHDc = pHDC;
		data.nType = UGGraphics::GDT_WhDC;
		pGraphics->SetGraphicsData(data, isAlphaEnable);
	}
	else
	{
		// no pHDC, Qt
		pGraphics->CreateCompatibleGraphics(NULL, isAlphaEnable);
		m_pGraphicsImageOld = pGraphics->SelectImage(m_pGraphicsImage);
	}
	

	return pGraphics;
}
void  MapControl::DisposeUGGrapics(UGGraphics* &pGraphics)
{
	if (m_pGraphicsImageOld != NULL)
	{
		pGraphics->SelectImage(m_pGraphicsImageOld);
	}

	delete pGraphics;
	pGraphics = NULL;
}

void MapControl::OnDraw(int rectLeft, int rectTop, int rectRight, int rectBottom, void* pHDC)
{
	if(mNeedRedraw)
	{
		mNeedRedraw = false;

		if (m_IsMinSized)
		{
			// if recovering from min size , no need to redraw, display last cache faster.
			m_pUGMapWnd->m_mapWnd.m_Map.SetRefreshFlag(false);
			m_IsMinSized = false;
		}
		//else
		//{
			// Create UGGraphics
			if (pHDC == NULL && m_pGraphicsImage == NULL)
			{
				int width = rectRight - rectLeft;
				int height = rectBottom - rectTop;

				width = width >= 0 ? width : (-width);
				height = height >= 0 ? height : (-height);

				m_pGraphicsImage = UGGraphicsManager::NewImage();
				m_pGraphicsImage->CreateCompatibleImage(NULL, width, height);
			}

			UGGraphics* pGraphics = CreateUGGraphics(pHDC);
			// window bounds
			UGRect clientRect(rectLeft, rectTop, rectRight, rectBottom);
			// draw map
			m_pUGMapWnd->OnDraw(pGraphics, clientRect, clientRect);

			DisposeUGGrapics(pGraphics);
		//}
	}
}


void MapControl::OnSizeChanged(int x, int y, void* pHDC)
{
	UGRect rect(0, 0, x, y);
	bool isEmpty = rect.IsEmpty();
	
	if(!isEmpty)
	{
		UGGraphics* pGraphics = NULL;
		if (pHDC != NULL)
		{
            pGraphics = CreateUGGraphics(pHDC);
		}
		else
		{
			delete m_pGraphicsImage;
			m_pGraphicsImage = UGGraphicsManager::NewImage();
			m_pGraphicsImage->CreateCompatibleImage(NULL, x, y);
		}
		m_pUGMapWnd->OnSize(pGraphics, rect);   // Will call InvalidateCallback
		
	}
	else 
	{
		m_IsMinSized = isEmpty;
	}
}
void MapControl::OnLMouseDown(unsigned int nFlags, int x, int y, void* pHDC)
{
	UGPoint pt(x, y);
	UGGraphics* pGraphics = CreateUGGraphics(pHDC);
	m_pUGMapWnd->OnLButtonDown(pGraphics, getKeyUGFlags(nFlags), pt);
	DisposeUGGrapics(pGraphics);
}

void MapControl::OnLMouseUp(unsigned int nFlags, int x, int y, void* pHDC)
{
	UGPoint pt(x, y);
	UGGraphics* pGraphics = CreateUGGraphics(pHDC);
	m_pUGMapWnd->OnLButtonUp(pGraphics, getKeyUGFlags(nFlags), pt);               // Will call InvalidateCallback if map need to redraw
	DisposeUGGrapics(pGraphics);
	int action = m_pUGMapWnd->GetUserAction();
	if (action != UGDrawParamaters::UGMapUserAction::uaPan)
	{
		Invalidate();                                                             // Not call InvalidateCallback in Edit or Draw mode, so call Invalidate(); no need to call in Pan mode
	}
}
void MapControl::OnLMouseDbClick(unsigned int nFlags, int x, int y, void* pHDC)
{
	int action = m_pUGMapWnd->GetUserAction();
	if (action == UGDrawParamaters::UGMapUserAction::uaPan)
	{// double click to zoom in
		double ratio = 5 / 4.0;
		MouseScale(ratio, x, y);

		return;
	}
		
	UGPoint pt(x, y);
	UGGraphics* pGraphics = CreateUGGraphics(pHDC);
	m_pUGMapWnd->OnLButtonDblClk(pGraphics, getKeyUGFlags(nFlags), pt);
	DisposeUGGrapics(pGraphics);
	
}
void MapControl::OnMouseMove(unsigned int nFlags, int x, int y, void* pHDC)
{
	UGPoint pt(x, y);
	UGGraphics* pGraphics = CreateUGGraphics(pHDC);
	m_pUGMapWnd->OnMouseMove(pGraphics, getKeyUGFlags(nFlags),pt);
	DisposeUGGrapics(pGraphics);
	if (pHDC == NULL)
	{
		int action = m_pUGMapWnd->GetUserAction();
		if (action == UGDrawParamaters::UGMapUserAction::uaPan)
		{
			Invalidate(false);      // No need to redraw in Pan mode
		}
		else
		{
			Invalidate(); // Edit, Draw mode
		}
	}
	    
}
void MapControl::OnMouseWheel(unsigned int nFlags, short zDelta, int x, int y)
{
	 //m_pUGMapWnd->SetZoomRefreshFlag(true);
	// the ratio is 1 after zooming in and zooming out, map scale will not be changed.
	double ratio = 1;
	if (zDelta > 0)
	{
		ratio = 5/4.0;
	}
	else
	{
		ratio = 4/5.0;
	}
	
	MouseScale(ratio, x, y);
}

void MapControl::OnRMouseDown(unsigned int nFlags, int x, int y, void* pHDC)
{
	if (!isExecuteRButton()) {    // uaPan -> uaNull if excuted, then can not pan
		return;
	}
	UGPoint pt(x, y);
	UGGraphics* pGraphics = CreateUGGraphics(pHDC);
	m_pUGMapWnd->OnRButtonDown(pGraphics, getKeyUGFlags(nFlags), pt);
	DisposeUGGrapics(pGraphics);
}

void MapControl::OnRMouseUp(unsigned int nFlags, int x, int y, void* pHDC)
{
	if (!isExecuteRButton()) {    // uaPan -> uaNull if excuted, then can not pan
		return;
	}
	UGPoint pt(x, y);
	UGGraphics* pGraphics = CreateUGGraphics(pHDC);
	m_pUGMapWnd->OnRButtonUp(pGraphics, getKeyUGFlags(nFlags), pt);   // Will call InvalidateCallback if map need to redraw
	
	DisposeUGGrapics(pGraphics);
}


void MapControl::OnMidMouseDown(unsigned int nFlags, int x, int y, void* pHDC)
{
	if (m_pUGMapWnd->GetUserAction() == UGDrawParamaters::UGMapUserAction::uaPan)
	{
		OnLMouseDown(nFlags, x, y, pHDC);
	}  
}

void MapControl::OnMidMouseUp(unsigned int nFlags, int x, int y, void* pHDC)
{
	if (m_pUGMapWnd->GetUserAction() == UGDrawParamaters::UGMapUserAction::uaPan)
	{
		OnLMouseUp(nFlags, x, y, pHDC);
	}
}

UGbyte* MapControl::GetImageBytes()
{
	if (m_pGraphicsImage != NULL)
	{
		return (UGbyte*) m_pGraphicsImage->GetColors();
	}
	else
	{
		return NULL;
	}
}

UGPoint2D MapControl::PixelToMap(int x, int y)
{
	UGPoint pt(x, y);
	return PixelToMap(pt);
}

UGPoint2D MapControl::PixelToMap(UGPoint pt)
{
	UGPoint2D pntlp;
	UGPoint2D pnt2D(0.0, 0.0);

	//DP -> LP -> MP
	UGDrawing *pDrawing = m_pUGMapWnd->m_mapWnd.m_Map.GetDrawing();
	m_pUGMapWnd->m_mapWnd.m_Map.GetDrawing()->m_DrawCache.DPtoLP(&pt, &pntlp, 1);
	pDrawing->m_DrawParam.LPtoMP(pntlp, pnt2D);

	return pnt2D;
}

UGPoint MapControl::MapToPixel(double x, double y)
{
	UGPoint2D pnt(x, y);
	return MapToPixel(pnt);

}
UGPoint MapControl::MapToPixel(UGPoint2D pnt)
{
	UGPoint2D pntlp;
	UGPoint pt(0, 0);

	// MP -> LP -> DP
	UGDrawing *pDrawing = m_pUGMapWnd->m_mapWnd.m_Map.GetDrawing();
	pDrawing->m_DrawParam.MPtoLP(pnt, pntlp);
	m_pUGMapWnd->m_mapWnd.m_Map.GetDrawing()->m_DrawCache.LPtoDP(&pntlp, &pt, 1);

	return pt;

}

void MapControl::MouseScale(double ratio, int x, int y, bool delayRefresh) {
	if (delayRefresh) {
		m_pUGMapWnd->SetZoomRefreshFlag(false);
	}
	else {
		m_pUGMapWnd->SetZoomRefreshFlag(true);
	}
	UGPoint pnt(x, y);
	UGPoint2D pntlp;
	UGPoint2D pnt2D = PixelToMap(x, y);

	//m_pUGMapWnd->m_mapWnd.m_Map.Zoom(ratio);
	m_pUGMapWnd->Zoom(ratio);
	UGMap* m_pMap = &(m_pUGMapWnd->m_mapWnd.m_Map);

	UGPoint2D curpnt2D = PixelToMap(x, y);

	m_pUGMapWnd->m_mapWnd.m_Map.Pan(curpnt2D.x - pnt2D.x, curpnt2D.y - pnt2D.y, true);

	m_pUGMapWnd->Refresh();     // Will call InvalidateCallback
}

void MapControl::Refresh()
{
	m_pUGMapWnd->Refresh();     // Will call InvalidateCallback
}


bool MapControl::isExecuteRButton() {
	int action = m_pUGMapWnd->GetUserAction();
	if (action == UGDrawParamaters::UGMapUserAction::uaPan) {
		return false;
	}
	else 
	{
		return true;
	}
}

UGMapEditorWnd* MapControl::GetMapEditWnd()
{
	return m_pUGMapWnd;
}
