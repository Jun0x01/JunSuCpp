/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.04.08
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

	/*UGWorkspace* pWorkspace = m_pUGMapWnd->m_mapWnd.m_Map.GetWorkspace();
	if(pWorkspace == NULL){
		pWorkspace = new UGWorkspace();
		m_pUGMapWnd->m_mapWnd.m_Map.SetWorkspace(pWorkspace);
	}*/

	m_pInnerWorkspace = new Workspace();
	m_pWorkspace = NULL;
	SetWorkspace(m_pInnerWorkspace);

	mNeedRedraw = false;

	mIsInWorkspace = false;
}
MapControl::~MapControl()
{
	if (m_pUGMapWnd != NULL) 
	{
		// close map
		m_pUGMapWnd->m_mapWnd.m_Map.Reset();
		m_pUGMapWnd->m_mapWnd.Reset();
		m_pUGMapWnd->Reset();

		m_pUGMapWnd->m_mapWnd.m_Map.SetWorkspace(NULL);
		
		m_pUGMapWnd->m_mapWnd.SetInvalidateFunc(NULL, NULL);
	}

	delete m_pGraphicsImage;
	m_pGraphicsImage = NULL;

	//delete m_pGraphicsImageOld;
	//m_pGraphicsImageOld = NULL;

	delete m_pUGMapWnd;
	m_pUGMapWnd = NULL;

	m_pInvalidateCallback = NULL;

	delete m_pInnerWorkspace;
	m_pInnerWorkspace = NULL;

	m_pWorkspace = NULL;
	m_pWnd = NULL;
}

void MapControl::Initialize(INVALIDATEPROC pCallBack, void* pView)
{
	// set graphics engine type
#ifdef WIN32
	UGGraphicsManager::SetCurGraphicsType(UGC::UGGraphics::GT_Windows);
#else
    UGGraphicsManager::SetCurGraphicsType(UGGraphics::GT_QT);
#endif
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
		//mNeedRedraw = false;  //delete, it resulted in that map didn't refresh in MDI when window become active from inactive

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

UGLayers* MapControl::GetUGLayers()
{
	return &(m_pUGMapWnd->m_mapWnd.m_Map.m_Layers);
}

void MapControl::SetEditableLayer(UGLayer* pLayer, bool isEditable)
{
	m_pUGMapWnd->m_mapWnd.m_Map.m_Layers.SetEditableLayer(pLayer, isEditable);
}

void MapControl::SetWorkspace(Workspace* workspace)
{
	if (workspace == m_pWorkspace)
	{
		return;
	}
	
	// close current map
	m_pUGMapWnd->m_mapWnd.m_Map.Reset();
	m_pUGMapWnd->Refresh();
	
	m_pWorkspace = workspace;
	if (m_pWorkspace != NULL)
	{
		m_pUGMapWnd->m_mapWnd.m_Map.SetWorkspace(m_pWorkspace->GetUGWorkspace());
	}
	else
	{
		m_pUGMapWnd->m_mapWnd.m_Map.SetWorkspace(NULL);
	}
}

bool MapControl::OpenMap(string mapName)
{
	UGString ugMapName;
	ugMapName.FromStd(mapName);
	bool isOpen = m_pUGMapWnd->m_mapWnd.m_Map.Open(ugMapName);
	m_pUGMapWnd->Refresh();

	mIsInWorkspace = isOpen;
	return isOpen;
}

UGLayer* MapControl::AddDataset(string datasourceName, string datasetName, bool bAddToHead)
{
	UGString ugDatasetName, ugDatasourceName;
	ugDatasourceName.FromStd(datasourceName);
	ugDatasetName.FromStd(datasetName);
	UGDataSource* pDatasource = m_pWorkspace->GetUGWorkspace()->GetDataSource(ugDatasourceName);
	UGLayer* pLayer = NULL;
	if (pDatasource != NULL) {
		UGDataset* pDataset = pDatasource->GetDataset(ugDatasetName);
		pLayer = GetUGLayers()->AddDataset(pDataset);
		if (pLayer != NULL) {
			if (GetUGLayers()->GetTotalCount() == 1) {
				m_pUGMapWnd->m_mapWnd.m_Map.SetName(ugDatasetName);
			}
			Refresh();
		}
	}
	else 
	{
		//TODO: ouput log
	}
	return pLayer;
	
}

bool MapControl::Save()
{

	//if(mIsInWorkspace){
	//	
	//	bool isSaved = m_pUGMapWnd->m_mapWnd.m_Map.Save();
	//	//bool isModified = m_pUGMapWnd->m_mapWnd.m_Map.GetWorkspace()->IsModified();
	//	//isSaved = m_pUGMapWnd->m_mapWnd.m_Map.GetWorkspace()->Save();
	//	return isSaved;
	//}
	//else {
	//	UGWorkspace* pUGWorkspace = m_pWorkspace->GetUGWorkspace();
	//	UGString xmlMap = m_pUGMapWnd->m_mapWnd.m_Map.ToXML();
	//	UGString mapName = m_pUGMapWnd->m_mapWnd.m_Map.GetName();
	//	UGString validName = m_pWorkspace->GetUGWorkspace()->m_MapStorages.GetUnoccupiedMapName(mapName);

	//	bool isSaved = false;
	//	bool isAdded = pUGWorkspace->m_MapStorages.Add(validName);
	//	if (isAdded) {
	//		UGMapStorage* pMapStore = pUGWorkspace->m_MapStorages.Find(mapName);
	//		pMapStore->SetXML(xmlMap, pMapStore->GetVersion());

	//		isSaved = m_pUGMapWnd->m_mapWnd.m_Map.Save();
	//		mIsInWorkspace = isSaved;
	//		
	//	}
	//	else
	//	{
	//		//TODO: output log
	//	}
		
	//	return isSaved;
	
	//}
	UGString mapName = m_pUGMapWnd->m_mapWnd.m_Map.GetName();
	return m_pUGMapWnd->m_mapWnd.m_Map.SaveAs(mapName, true);
}

bool MapControl::SaveAs(string mapName)
{
	UGString ugMapName;
	ugMapName.FromStd(mapName);
	bool isSaved = false;
	isSaved = m_pUGMapWnd->m_mapWnd.m_Map.SaveAs(ugMapName);
	return isSaved;
}


/*
 * ��γ������ֵת����Ļ����
 * @langitude     ����
 * @latitude      γ��
 * @srcEPSGCode   ����ľ�γ��ֵ��Ӧ������ϵ��EPSGCode��Ĭ��4326,��WGS1984����ϵ����ʹ��China2000,����4490
 */
UGPoint MapControl::GeoCoordToPixel(double longitude, double latitude, int srcEPSGCode/*=4326*/)
{
	// ��ת���ɵ�ͼ���꣬��ת������Ļ����

	// ��ͼ����ϵ
	const UGPrjCoordSys& mapPrj = m_pUGMapWnd->m_mapWnd.m_Map.GetPrjCoordSys();

	int mapEPSGCode = mapPrj.GetEPSGCode();

	UGPoint pResP = MapToPixel(longitude, latitude);
	UGPrjCoordSys *pSrcPrj = new UGPrjCoordSys(srcEPSGCode);
	// Ĭ��ֵ
	if (mapPrj.GetProjection().GetProjectionType() != pSrcPrj->GetProjection().GetProjectionType()) {

		//����EPSGCode����Դ����ϵ
		
		UGRefTranslator ugRefTranslator;

		ugRefTranslator.SetPrjCoordSysSrc(*pSrcPrj);         // ��ת���������ϵ
		ugRefTranslator.SetPrjCoordSysDes(mapPrj);           // ת����������ϵ
		// ����ת��������������
		ugRefTranslator.SetGeoTransMethod(EmGeoTransMethod::MTH_GEOCENTRIC_TRANSLATION);

		// ת���������������õ�ת������������ת��ƽ�Ƶ�ʱ���ö�Ӧ������û��Ҫ��ʹ���һ���ն���
		UGGeoTransParams *pTempParams = new UGGeoTransParams();
		ugRefTranslator.SetGeoTransParams(*pTempParams);
		delete pTempParams;
		pTempParams = NULL;

		// ����ľ�γ�������
		UGPoint2D srcPt(longitude, latitude);

		UGArray<UGPoint2D> ugArray;
		ugArray.Add(srcPt);

		int count = ugArray.GetSize();

		bool result = ugRefTranslator.Translate(ugArray.GetData(), count);
		if (result)
		{
			UGPoint2D res = ugArray.GetAt(0);
			// ת������Ļ����
			return MapToPixel(res);
		}
		else
		{
			Log::Warning("GeoCoordToPixel > ugRefTranslator.Translate()ת��ʧ�ܣ�����MapToPixel()ת���Ľ��");
			return pResP;
		}

	}
	else 
	{
		// ����ϵ��ֱͬ��ת����Ļ����
		return pResP;
	}
}

/*
 * ��Ļ����ת���ɾ�γ������
 * @x             ��Ļ����X
 * @y             ��Ļ����Y
 * @destEPSGCode  ת����ľ�γ��ֵ��Ӧ������ϵ��EPSGCode��Ĭ��4326,��WGS1984����ϵ����ʹ��China2000,����4490
 */
UGPoint2D MapControl::PixelToGeoCoord(int x, int y, int destEPSGCode /*= 4326*/)
{
	// ��ת���ɵ�ͼ���꣬��ת������Ļ����

	// ��ͼ����ϵ
	const UGPrjCoordSys& mapPrj = m_pUGMapWnd->m_mapWnd.m_Map.GetPrjCoordSys();

	int mapEPSGCode = mapPrj.GetEPSGCode();

	// ��ת��Ϊ��ͼ����
	UGPoint2D rePt = PixelToMap(x, y);

	UGPrjCoordSys *pDestPrj = new UGPrjCoordSys(destEPSGCode);
	// Ĭ��ֵ
	if (mapPrj.GetProjection().GetProjectionType() != pDestPrj->GetProjection().GetProjectionType()) {

		//����EPSGCode����Դ����ϵ

		UGRefTranslator ugRefTranslator;

		ugRefTranslator.SetPrjCoordSysSrc(mapPrj);         // ��ת���������ϵ
		ugRefTranslator.SetPrjCoordSysDes(*pDestPrj);      // ת����������ϵ
		// ����ת��������������
		ugRefTranslator.SetGeoTransMethod(EmGeoTransMethod::MTH_GEOCENTRIC_TRANSLATION);

		// ת���������������õ�ת������������ת��ƽ�Ƶ�ʱ���ö�Ӧ������û��Ҫ��ʹ���һ���ն���
		UGGeoTransParams *pTempParams = new UGGeoTransParams();
		ugRefTranslator.SetGeoTransParams(*pTempParams);
		delete pTempParams;
		pTempParams = NULL;

		
		UGArray<UGPoint2D> ugArray;
		ugArray.Add(rePt);

		int count = ugArray.GetSize();

		bool result = ugRefTranslator.Translate(ugArray.GetData(), count);
		if (result)
		{
			UGPoint2D res = ugArray.GetAt(0); // ��ȡת�����
			return res;
		}
		else
		{
			Log::Warning("PixelToGeoCoord > ugRefTranslator.Translate()ת��ʧ�ܣ�ֱ�ӷ��ص�ͼ����");
			return rePt;
		}
	}
	else
	{
		// ����ϵ��ֱͬ��ת����ͼ����
		return rePt;
	}
}

