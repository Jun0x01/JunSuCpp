/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.04.08
 */
#include "Mapping/MapControl.h"


using namespace SuperMap;

/**************** Invalidate CallBack *********************/
void SuCALLBACK InvalidateCallbackMapControl(void * p)
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
	Initialize(InvalidateCallbackMapControl, this);

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

	// 初始化动态层，并添加到地图窗口
	pUGDynLayer = new UGDynamicLayer();
	m_pUGMapWnd->m_mapWnd.m_Map.m_DynamicLayers.Add(pUGDynLayer);
}
MapControl::~MapControl()
{
	if (m_pUGMapWnd != NULL) 
	{
		// 移除动态层
		m_pUGMapWnd->m_mapWnd.m_Map.m_DynamicLayers.RemoveAll();

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
    pUGMap->SetColorMode(UGDrawParamaters::/*UGMapColorMode::*/cmDefault);
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
    m_pUGMapWnd->SetUserAction(UGDrawParamaters::/*UGMapUserAction::*/uaPan); // uaPointModeSelect
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
    if (action != UGDrawParamaters::/*UGMapUserAction::*/uaPan)
	{
		Invalidate();                                                             // Not call InvalidateCallback in Edit or Draw mode, so call Invalidate(); no need to call in Pan mode
	}
}
void MapControl::OnLMouseDbClick(unsigned int nFlags, int x, int y, void* pHDC)
{
	int action = m_pUGMapWnd->GetUserAction();
    if (action == UGDrawParamaters::/*UGMapUserAction::*/uaPan)
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
        if (action == UGDrawParamaters::/*UGMapUserAction::*/uaPan)
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
    if (m_pUGMapWnd->GetUserAction() == UGDrawParamaters::/*UGMapUserAction::*/uaPan)
	{
		OnLMouseDown(nFlags, x, y, pHDC);
	}  
}

void MapControl::OnMidMouseUp(unsigned int nFlags, int x, int y, void* pHDC)
{
    if (m_pUGMapWnd->GetUserAction() == UGDrawParamaters::/*UGMapUserAction::*/uaPan)
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
    if (action == UGDrawParamaters::/*UGMapUserAction::*/uaPan) {
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

Workspace* MapControl::GetWorkspace()
{
	return m_pWorkspace;
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
 * 经纬度坐标值转成屏幕坐标
 * @langitude     经度
 * @latitude      纬度
 * @srcEPSGCode   传入的经纬度值对应的坐标系的EPSGCode，默认4326,即WGS1984坐标系；若使用China2000,传入4490
 */
UGPoint MapControl::GeoCoordToPixel(double longitude, double latitude, int srcEPSGCode/*=4326*/)
{
	// 先转换成地图坐标，再转换成屏幕坐标

	// 地图坐标系
	const UGPrjCoordSys& mapPrj = m_pUGMapWnd->m_mapWnd.m_Map.GetPrjCoordSys();

	int mapEPSGCode = mapPrj.GetEPSGCode();

	UGPoint pResP = MapToPixel(longitude, latitude);
	UGPrjCoordSys srcPrj(srcEPSGCode);
	// 默认值
	if (mapPrj != srcPrj) {

		//根据EPSGCode创建源坐标系
		
		UGRefTranslator ugRefTranslator;

        ugRefTranslator.SetPrjCoordSysSrc(srcPrj);         // 被转换点的坐标系
		ugRefTranslator.SetPrjCoordSysDes(mapPrj);           // 转换后点的坐标系
		// 设置转换方法：三参数
        ugRefTranslator.SetGeoTransMethod(UGC::/*EmGeoTransMethod::*/MTH_GEOCENTRIC_TRANSLATION);

		// 转换参数，根据设置的转换方法，有旋转，平移等时设置对应参数，没有要求就创建一个空对象
		UGGeoTransParams tempParams;
		ugRefTranslator.SetGeoTransParams(tempParams);

		// 传入的经纬度坐标点
		UGPoint2D srcPt(longitude, latitude);

		UGArray<UGPoint2D> ugArray;
		ugArray.Add(srcPt);

		int count = ugArray.GetSize();

		bool result = ugRefTranslator.Translate(ugArray.GetData(), count);
		if (result)
		{
			UGPoint2D res = ugArray.GetAt(0);
			// 转换成屏幕坐标
			return MapToPixel(res);
		}
		else
		{
			Log::Warning("GeoCoordToPixel > ugRefTranslator.Translate()转换失败，返回MapToPixel()转换的结果");
			return pResP;
		}
	}
	else 
	{
		// 坐标系相同直接转换屏幕坐标
		return pResP;
	}
}

/*
 * 屏幕坐标转换成经纬度坐标
 * @x             屏幕坐标X
 * @y             屏幕坐标Y
 * @destEPSGCode  转换后的经纬度值对应的坐标系的EPSGCode，默认4326,即WGS1984坐标系；若使用China2000,传入4490
 */
UGPoint2D MapControl::PixelToGeoCoord(int x, int y, int destEPSGCode /*= 4326*/)
{
	// 先转换成地图坐标，再转换成屏幕坐标

	// 地图坐标系
	const UGPrjCoordSys& mapPrj = m_pUGMapWnd->m_mapWnd.m_Map.GetPrjCoordSys();

	int mapEPSGCode = mapPrj.GetEPSGCode();

	// 先转换为地图坐标
	UGPoint2D rePt = PixelToMap(x, y);

	UGPrjCoordSys destPrj(destEPSGCode);
	// 默认值
	if (mapPrj != destPrj) {

		//根据EPSGCode创建源坐标系

		UGRefTranslator ugRefTranslator;

		ugRefTranslator.SetPrjCoordSysSrc(mapPrj);         // 被转换点的坐标系
		ugRefTranslator.SetPrjCoordSysDes(destPrj);      // 转换后点的坐标系
		// 设置转换方法：三参数
        ugRefTranslator.SetGeoTransMethod(UGC::/*EmGeoTransMethod::*/MTH_GEOCENTRIC_TRANSLATION);

		// 转换参数，根据设置的转换方法，有旋转，平移等时设置对应参数，没有要求就创建一个空对象
		UGGeoTransParams tempParams;
		ugRefTranslator.SetGeoTransParams(tempParams);
		
		UGArray<UGPoint2D> ugArray;
		ugArray.Add(rePt);

		int count = ugArray.GetSize();

		bool result = ugRefTranslator.Translate(ugArray.GetData(), count);
		if (result)
		{
			UGPoint2D res = ugArray.GetAt(0); // 获取转换结果
			return res;
		}
		else
		{
			Log::Warning("PixelToGeoCoord > ugRefTranslator.Translate()转换失败，直接返回地图坐标");
			return rePt;
		}
	}
	else
	{
		// 坐标系相同直接转换地图坐标
		return rePt;
	}
}


/*
* 将给定的点串转换为地图坐标, 对于创建动态层，跟踪层上的对象时很有用处，应为这些临时层需使用地图坐标，当然如果需要添加的点已经是地图坐标了就不需要转换了。
* @pts           需要转换的一系列点，这些点是地理坐标或投影坐标的点; 注意转换时会直接将pts中的值修改为地图坐标系的值
* @srcEPSGCode   传入点串对应的坐标系的EPSGCode，默认4326,即WGS1984坐标系；若使用China2000,则传入4490
*/
bool MapControl::ToMapCoords(UGPoint2Ds& pts, int srcEPSGCode /*= 4326*/)
{
	// 地图坐标系
	const UGPrjCoordSys& mapPrj = m_pUGMapWnd->m_mapWnd.m_Map.GetPrjCoordSys();

	int mapEPSGCode = mapPrj.GetEPSGCode();


	UGPrjCoordSys srcPrj(srcEPSGCode);
	// 默认值
	if (mapPrj != srcPrj) {

		//根据EPSGCode创建源坐标系

		UGRefTranslator ugRefTranslator;

		ugRefTranslator.SetPrjCoordSysSrc(srcPrj);         // 被转换点的坐标系
		ugRefTranslator.SetPrjCoordSysDes(mapPrj);      // 转换后点的坐标系
		// 设置转换方法：三参数
        ugRefTranslator.SetGeoTransMethod(UGC::/*EmGeoTransMethod::*/MTH_GEOCENTRIC_TRANSLATION);

		// 转换参数，根据设置的转换方法，有旋转，平移等时设置对应参数，没有要求就创建一个空对象
		UGGeoTransParams tempParams;
		ugRefTranslator.SetGeoTransParams(tempParams);
		
		int count = pts.GetSize();

		bool result = ugRefTranslator.Translate(pts.GetData(), count);
		if (result)
		{
		}
		else
		{
			Log::Warning("PixelToGeoCoord > ugRefTranslator.Translate()转换失败，直接返回地图坐标");
		}
		return result;
	}
	else
	{
		// 坐标系相同不用转换
		return true;
	}
}

/*
 * 将给定的点串转换为屏幕坐标
 * @pts           需要转换的一系列点，这些点是地理坐标或投影坐标的点
 * @srcEPSGCode   传入点串对应的坐标系的EPSGCode，默认4326,即WGS1984坐标系；若使用China2000,则传入4490
 */
bool MapControl::ToPixels(const UGPoint2Ds& pts, UGPoints& outPts, int srcEPSGCode/* = 4326*/)
{
	UGPoint2Ds ptsTemp(pts);
	bool res = ToMapCoords(ptsTemp, srcEPSGCode);
	if (res) {
		for (int i = 0; i < ptsTemp.GetSize(); i++) {
			UGPoint p = MapToPixel(ptsTemp.GetAt(i));
			outPts.Add(p);
		}
	}
	return res;
}

/*
 * 将给定的点串转换到指定坐标系
 * @pts           需要转换的一系列点,这些点是屏幕坐标的点
 * @srcEPSGCode   传入点串对应的坐标系的EPSGCode，默认4326,即WGS1984坐标系；若使用China2000,则传入4490
 */
bool MapControl::ToGeoCoords(const UGPoints& pts, UGPoint2Ds& outPts, int destEPSGCode /*= 4326*/)
{
	for (int i = 0; i < pts.GetSize(); i++) {
		UGPoint2D p = PixelToMap(pts.GetAt(i));
		outPts.Add(p);
	}
	// 地图坐标系
	const UGPrjCoordSys& mapPrj = m_pUGMapWnd->m_mapWnd.m_Map.GetPrjCoordSys();

	int mapEPSGCode = mapPrj.GetEPSGCode();

	
	UGPrjCoordSys destPrj(destEPSGCode);
	// 默认值
	if (mapPrj != destPrj) {

		//根据EPSGCode创建源坐标系

		UGRefTranslator ugRefTranslator;

		ugRefTranslator.SetPrjCoordSysSrc(mapPrj);         // 被转换点的坐标系
		ugRefTranslator.SetPrjCoordSysDes(destPrj);      // 转换后点的坐标系
		// 设置转换方法：三参数
        ugRefTranslator.SetGeoTransMethod(UGC::/*EmGeoTransMethod::*/MTH_GEOCENTRIC_TRANSLATION);

		// 转换参数，根据设置的转换方法，有旋转，平移等时设置对应参数，没有要求就创建一个空对象
		UGGeoTransParams tempParams;
		ugRefTranslator.SetGeoTransParams(tempParams);

		int count = outPts.GetSize();

		bool result = ugRefTranslator.Translate(outPts.GetData(), count);
		if (result)
		{
		}
		else
		{
			Log::Warning("PixelToGeoCoord > ugRefTranslator.Translate()转换失败，直接返回地图坐标");
		}
		return result;
	}
	else
	{
		// 坐标系相同不用转换
		return true;
	}

}

/*
* 将给定坐标值的点添加到动态层,
* @x             经度或横向坐标值
* @y             纬度或纵向坐标值
* @size          点符号的大小，默认为4mm
* @color         点符号的颜色值，对固定颜色的符号和图片符号无效; 颜色值格式为BGR(0xFFFFFF)，默认值为黑色
* @symbolID      点符号库ID, 用于指定点的显示形状，默认值为0(内置的系统符号库id)，代表圆点;
*                工作空间资源中保存了符号库，可使用iDesktop软件查看或自定义。
* @srcEPSGCode   传入的经纬度值对应的坐标系的EPSGCode，默认4326,即WGS1984坐标系；若使用China2000,则传入4490
*/
bool MapControl::AddDynamicPoint(const UGString& keyName, double x, double y, int color /*= 0*/, double size /*= 4*/, int symbolID /*= 0*/, int srcEPSGCode /*= 4326*/)
{
	// 转换到地图坐标
	UGPoint2Ds pts;
	UGPoint2D pt(x, y);
	pts.Add(pt);

	bool isToMap = ToMapCoords(pts, srcEPSGCode);
	if (isToMap) 
	{
		UGPoint2D ptMap = pts.GetAt(0);
		// 创建对象
		UGGeoPoint* pGeo = new UGGeoPoint();
		pGeo->SetX(ptMap.x);
		pGeo->SetY(ptMap.y);

		// 初始化风格, 设置点对象风格
		UGStyle* pStyle = new UGStyle();
		pStyle->SetLineColor(color);
		pStyle->SetMarkerSize(size);
		pStyle->SetMarkerStyle(symbolID);

		pGeo->SetStyle(pStyle);
		delete pStyle;
	
		// 添加到动态层
		return pUGDynLayer->Add(keyName, pGeo);
	}
	else
	{
		return false;
	}
}

/*
 * 将给定的两个点的坐标值作为线的两个端点，并添加到动态层
 * @keyName       添加到动态层对象的名称，不可重名
 * @x1, y1        第一个点的坐标值
 * @x2, y2        第二个点的坐标值
 * @color         线的颜色，对固定颜色的符号无效; 颜色值格式为BGR(0xFFFFFF)，默认值为黑色
 * @width         线宽，默认0.1mm
 * @symbolID      线符号库ID, 用于指定线的显示形状，默认值为0(内置的系统符号库id)，代表实线; 1代表虚线
 *                工作空间资源中保存了符号库，可使用iDesktop软件查看或自定义。
 * @srcEPSGCode   传入的经纬度值对应的坐标系的EPSGCode，默认4326,即WGS1984坐标系；若使用China2000,则传入4490
 */
bool MapControl::AddDynamicLine(const UGString& keyName, double x1, double y1, double x2, double y2, int color /*= 0*/, double width /*= 0.1*/, int symbolID /*= 0*/, int srcEPSGCode /*= 4326*/)
{
	// 转换到地图坐标
	UGPoint2Ds pts;
	UGPoint2D pt1(x1, y1);
	UGPoint2D pt2(x2, y2);
	pts.Add(pt1);
	pts.Add(pt2);

	bool isToMap = ToMapCoords(pts, srcEPSGCode);
	if (isToMap)
	{
		// 创建对象
		UGGeoLine* pGeo = new UGGeoLine();
		pGeo->AddSub(pts.GetData(), pts.GetSize());

		// 初始化风格, 设置对象风格
		UGStyle* pStyle = new UGStyle();
		pStyle->SetLineColor(color);
		pStyle->SetLineWidth(width);
		pStyle->SetMarkerStyle(symbolID);

		pGeo->SetStyle(pStyle);
		delete pStyle;

		// 添加到动态层
		return pUGDynLayer->Add(keyName, pGeo);
	}
	else
	{
		return false;
	}
}

/*
* 将给定的点串添加到动态层
* @keyName       添加到动态层对象的名称，不可重名
* @pts           需要添加的点串
* @color         线的颜色，对固定颜色的符号无效; 颜色值格式为BGR(0xFFFFFF)，默认值为黑色
* @width         线宽，默认0.1mm
* @symbolID      线符号库ID, 用于指定线的显示形状，默认值为0(内置的系统符号库id)，代表实线; 1代表虚线
*                工作空间资源中保存了符号库，可使用iDesktop软件查看或自定义。
* @srcEPSGCode   传入点串对应的坐标系的EPSG Code，默认4326,即WGS1984坐标系；若使用China2000,则传入4490
*/
bool MapControl::AddDynamicLine(const UGString& keyName, const UGPoint2Ds pts, int color /*= 0*/, double width /*= 0.1*/, int symbolID /*= 0*/, int srcEPSGCode /*= 4326*/)
{
	// 转换到地图坐标
	UGPoint2Ds pts1(pts); // 新建点串，避免改变原始值

	bool isToMap = ToMapCoords(pts1, srcEPSGCode);
	if (isToMap)
	{
		// 创建对象
		UGGeoLine* pGeo = new UGGeoLine();
		pGeo->AddSub(pts1.GetData(), pts1.GetSize());

		// 初始化风格, 设置对象风格
		UGStyle* pStyle = new UGStyle();
		pStyle->SetLineColor(color);
		pStyle->SetLineWidth(width);
		pStyle->SetMarkerStyle(symbolID);

		pGeo->SetStyle(pStyle);
		delete pStyle;

		// 添加到动态层
		return pUGDynLayer->Add(keyName, pGeo);
	}
	else
	{
		return false;
	}
}

void MapControl::SetDynamicGeoVisible(const UGString& keyName, bool isVisible, bool needRefresh)
{
	UGGeometry* pGeo = pUGDynLayer->Find(keyName);
	if (NULL != pGeo)
	{
		UGStyle* pStyle = pGeo->GetStyle();
		if (NULL != pStyle && pStyle->IsVisible != isVisible) 
		{
			pStyle->SetIsVisible(isVisible);
			if (needRefresh)
				Refresh();
		}
	}
}

/*
 * 设置是否使用动态投影显示地图
 */
void MapControl::SetMapDynamicProjection(bool isDynamicPrjection)
{
	m_pUGMapWnd->m_mapWnd.m_Map.SetProjectionOnTheFly(isDynamicPrjection);
}