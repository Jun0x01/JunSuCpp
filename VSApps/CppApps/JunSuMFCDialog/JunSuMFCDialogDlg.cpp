/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.04.11
 */
// JunSuMFCDialogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JunSuMFCDialog.h"
#include "JunSuMFCDialogDlg.h"
#include "afxdialogex.h"
#include "Map/UGLayerCollection.h"
// 注意：911 版本没有 GeometricNetwork 目录的头文件，可以使用 910，或10i的
#include "GeometricNetwork/UGConnectAnalyst.h"
#include "GeometricNetwork/UGUtilityAnalyst.h"

#include "GeometryPlot/UGGOLibraryManager.h"
#include "Map/UGDynamicLayers.h"
#include "Map/UGDynamicLayer.h"
#include "Geometry/UGGeoPoint.h"
#include "Geometry/UGGeoText.h"
#include "Workspace/UGWorkspace.h"
#include "Engine/UGDatasources.h"
#include "Engine/UGDatasource.h"
#include "Engine/UGDataset.h"
#include "Geometry/UGGeometry.h"
#include "Engine/UGEngDefs.h"
#include "Geometry/UGGeoLine.h"
#include "GeometryPlot//UGGraphicObject.h"
#include "Animation/UGAnimationDefine.h"
#include "Animation/UGAnimationManager.h"
#include "Animation/UGAnimationGO.h"
#include "Animation/UGScaleAnimation.h"
#include "Animation/UGAnimationGroup.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
void SuCALLBACK InvalidateCallBack(void* pWnd)
{

	// send message to view  for invalidating itself
	//SendDlgItemMessageW(, this, 0x01, NULL,NULL);

	JunSuMFCDialogDlg *pView = (JunSuMFCDialogDlg *)pWnd;
	if (pView->m_hWnd != NULL && pView->IsWindowVisible()) // 此处，窗口可能还没有初始化好，会出现问题
	   pView->Invalidate(false); // not redraw background
}

void UGSTDCALL OnSingleGeometrySelected(UGlong pWnd, UGLayer* pLayer, UGint nGeoID)
{
	int id = nGeoID;
	if (pLayer != NULL)
	{

	}
}

void UGSTDCALL GeometrySelectedCallBack(UGlong pWnd, UGint nSelectedGeometryCount)
{
	if (pWnd != NULL)
	{
		JunSuMFCDialogDlg* pThis = (JunSuMFCDialogDlg *)pWnd;
		// GetSelectedGeo()中可以获得选择集，从而获得选中的对象。目前实现的是用于测试的代码，会影响鼠标选中显示效果，
		// 因此注释掉，若有需要，参考其内容使用即可
		//pThis->GetSelectedGeo();  
	}

}
void UGSTDCALL GeometrySelectChangedCallBack(UGlong pWnd, UGint nSelectedGeometryCount)
{
	if (pWnd != NULL)
	{

	}
}

// animation timer callback
void CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT_PTR nTimerid, DWORD dwTime)
{
	UGAnimationManager::Instance()->Excute();
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// JunSuMFCDialogDlg dialog



JunSuMFCDialogDlg::JunSuMFCDialogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(JunSuMFCDialogDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pMapControl = new MapControl(InvalidateCallBack, this);
	m_pMapControl->GetMapEditWnd()->SetSingleGeometrySelectedFunc(OnSingleGeometrySelected, (UGlong )this);
	//m_pMapControl->GetMapEditWnd()->SetGeometrySelectChangedFunc(GeometrySelectChangedCallBack, (UGlong)this);
	m_pMapControl->GetMapEditWnd()->SetGeometrySelectedFunc(GeometrySelectedCallBack, (UGlong)this);

	m_pLastEditLayer = NULL;
	
	UGString strJYLibPath = _U("..\\..\\..\\TestData\\Plot\\JY.plot");
	UGString strTYLibPath = _U("..\\..\\..\\TestData\\Plot\\TY.plot");
	plotJYid = -1;
	plotTYid = -1;
	UGGOLibraryManager* pLibManager = UGGOLibraryManager::GetInstance();
	if (NULL == pLibManager)
	{
		return;
	}
	//添加标号库
	UGint nJYLibId = pLibManager->AddGOLibrary(strJYLibPath);
	UGint nTYLibId = pLibManager->AddGOLibrary(strTYLibPath);

	plotJYid = nJYLibId;
	plotTYid = nTYLibId;
	string strPath = "..\\..\\..\\TestData\\World.smwu";
	OpenWorkspace(strPath);
	
	// Plot animation timer
	SetTimer(1, 1000, TimerProc);

	//UGAnimationManager* pManager = UGAnimationManager::Instance();

}

void JunSuMFCDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(JunSuMFCDialogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
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
	ON_COMMAND(ID_FILE_OPEN32771, &JunSuMFCDialogDlg::OnFileOpen)
	ON_COMMAND(ID_DRAW_DRAWPOINT, &JunSuMFCDialogDlg::OnDrawPoint)
	ON_COMMAND(ID_DRAW_DRAWLINE32774, &JunSuMFCDialogDlg::OnDrawLine)
	ON_COMMAND(ID_DRAW_DRAWREGION, &JunSuMFCDialogDlg::OnDrawRegion)
	ON_COMMAND(ID_MEASURE_MEASURELENGTH, &JunSuMFCDialogDlg::OnMeasureLength)
	ON_COMMAND(ID_MEASURE_MEASUREAREA, &JunSuMFCDialogDlg::OnMeasureAea)
	ON_COMMAND(ID_FILE_CLOSE32772, &JunSuMFCDialogDlg::OnFileClose)
	ON_COMMAND(ID_EDIT_PANMAP, &JunSuMFCDialogDlg::OnPanMap)
	ON_COMMAND(ID_EDIT_SELECT, &JunSuMFCDialogDlg::OnEditSelect)
	ON_COMMAND(ID_DRAW_DRAWPLOT, &JunSuMFCDialogDlg::OnDrawDrawplot)
	ON_COMMAND(ID_DRAW_DRAWPLOTARRAWS, &JunSuMFCDialogDlg::OnDrawPlotArraws)
END_MESSAGE_MAP()


// JunSuMFCDialogDlg message handlers

BOOL JunSuMFCDialogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	mMenu.LoadMenu(IDR_MENU);
	SetMenu(&mMenu);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void JunSuMFCDialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void JunSuMFCDialogDlg::OnPaint()
{

	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(&rect);
	m_pMapControl->OnDraw(rect.left, rect.top, rect.right, rect.bottom, dc.m_hDC);

	CDialogEx::OnPaint();

}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR JunSuMFCDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void JunSuMFCDialogDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//CPaintDC dc(this); 

	//m_pMapControl->OnMouseMove(dc.m_hDC, nFlags, point.x, point.y);
	m_pMapControl->OnMouseMove(nFlags, point.x, point.y, ::GetDC(this->m_hWnd));
	CDialogEx::OnMouseMove(nFlags, point);
}


BOOL JunSuMFCDialogDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	ScreenToClient(&pt);
	m_pMapControl->OnMouseWheel(nFlags, zDelta, pt.x, pt.y);
	//	Invalidate(false);
		//return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
	return true;
}


void JunSuMFCDialogDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//CPaintDC dc(this); 
	//m_pMapControl->OnMouseLDown(dc.m_hDC, nFlags, point.x, point.y);
	m_pMapControl->OnLMouseDown(nFlags, point.x, point.y, ::GetDC(this->m_hWnd));
	CDialogEx::OnLButtonDown(nFlags, point);

	pt = point;
}


void JunSuMFCDialogDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
// 	CPaintDC dc(this); 
// 	m_pMapControl->OnMouseLUp(dc.m_hDC, nFlags, point.x, point.y);
	m_pMapControl->OnLMouseUp(nFlags, point.x, point.y, ::GetDC(this->m_hWnd));
	CDialogEx::OnLButtonUp(nFlags, point);
	//this->Invalidate(FALSE);

}


void JunSuMFCDialogDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_pMapControl->OnLMouseDbClick(nFlags, point.x, point.y, ::GetDC(this->m_hWnd));
	CDialogEx::OnLButtonDblClk(nFlags, point);
}


void JunSuMFCDialogDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_pMapControl->OnRMouseDown(nFlags, point.x, point.y, ::GetDC(this->m_hWnd));
	CDialogEx::OnRButtonDown(nFlags, point);
}


void JunSuMFCDialogDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_pMapControl->OnRMouseUp(nFlags, point.x, point.y, ::GetDC(this->m_hWnd));
	CDialogEx::OnRButtonUp(nFlags, point);
}


void JunSuMFCDialogDlg::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnRButtonDblClk(nFlags, point);
}


void JunSuMFCDialogDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	m_pMapControl->OnSizeChanged(cx, cy, ::GetDC(this->m_hWnd));
}




// Menu event : File
void JunSuMFCDialogDlg::OnFileOpen()
{
	CFileDialog fileDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.smwu)|*.smwu"), NULL);
	if (fileDialog.DoModal())
	{
		CString filePath = fileDialog.GetPathName();
		string strPath = StrConvertor::CStringTstring(filePath);

		OpenWorkspace(strPath);
	}
	else 
	{

	}
}

void JunSuMFCDialogDlg::OnFileClose()
{
	CloseWorkspace();
}

// Menu event : Draw
void JunSuMFCDialogDlg::OnDrawPoint()
{
	UGMapEditorWnd* pMapEditorWnd = m_pMapControl->GetMapEditWnd();
	UGMap* pUGMap = &(pMapEditorWnd->m_mapWnd.m_Map);
	UGString layerName = _U("DrawCAD@World");
	UGLayer* pLayer = m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map.m_Layers.GetLayer(layerName);
	if (pLayer != NULL)
	{
		ResetLastEditLayer(pLayer);
		pLayer->SetSelectable(true);

		UGLayers* pLayers = &(pUGMap->m_Layers);
		pLayers->SetEditableLayer(pLayer, true);

		int oldAction = pMapEditorWnd->m_mapWnd.m_Map.GetDrawing()->GetDrawParamaters()->GetUserAction();
		int oldEditAction = pMapEditorWnd->GetEditToolPack()->GetUserAction();
		pMapEditorWnd->SetUserAction(UGC::UGDrawParamaters::uaEdit, UGC::UGEditType::ET_Point);
	
	}
	else
	{

	}
}


void JunSuMFCDialogDlg::OnDrawLine()
{
	UGMapEditorWnd* pMapEditorWnd = m_pMapControl->GetMapEditWnd();
	UGMap* pUGMap = &(pMapEditorWnd->m_mapWnd.m_Map);
	UGString layerName = _U("DrawCAD@World");
	UGLayer* pLayer = pUGMap->m_Layers.GetLayer(layerName);
	if (pLayer != NULL)
	{
		ResetLastEditLayer(pLayer);
		pLayer->SetSelectable(true);
	
		UGLayers* pLayers = &(pUGMap->m_Layers);
		pLayers->SetEditableLayer(pLayer, true);

		pMapEditorWnd->SetUserAction(UGC::UGDrawParamaters::uaEdit, UGC::UGEditType::ET_PolyLine);
	}
	else
	{

	}
}


void JunSuMFCDialogDlg::OnDrawRegion()
{
	UGMapEditorWnd* pMapEditorWnd = m_pMapControl->GetMapEditWnd();
	UGMap* pUGMap = &(pMapEditorWnd->m_mapWnd.m_Map);

	UGString layerName = _U("DrawCAD@World");
	UGLayer* pLayer = pUGMap->m_Layers.GetLayer(layerName);
	if (pLayer != NULL)
	{
		ResetLastEditLayer(pLayer);

		pLayer->SetSelectable(true);
		
		UGLayers* pLayers = &(pUGMap->m_Layers);
		pLayers->SetEditableLayer(pLayer, true);

		pMapEditorWnd->SetUserAction(UGC::UGDrawParamaters::uaEdit, UGC::UGEditType::ET_PolyGon);
	}
	else
	{

	}
}


// Menu event : Measure
void JunSuMFCDialogDlg::OnMeasureLength()
{
	// TODO: Add your command handler code here
}


void JunSuMFCDialogDlg::OnMeasureAea()
{
	// TODO: Add your command handler code here
}

void JunSuMFCDialogDlg::OpenWorkspace(string strPath)
{
	// Close first
	CloseWorkspace();

	// Open workspace file
	UGString wkPath;
	wkPath.FromStd(strPath);       // Convert String

	UGWorkspaceConnection wkCon;
	
	wkCon.m_strServer = wkPath;
	wkCon.m_nWorkspaceType = UGWorkspace::WS_Version_SMWU;

	UGWorkspace* pWorkspace = m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map.GetWorkspace();
	if (pWorkspace->Open(wkCon))
	{
		if (pWorkspace->m_MapStorages.GetCount() > 0)
		{
			UGString mapName = pWorkspace->m_MapStorages.GetNameAt(0);
			UGbool isOpen = m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map.Open(mapName);
			m_pMapControl->Refresh();
		}
	}
	else
	{

	}
}
void JunSuMFCDialogDlg::CloseWorkspace()
{
	UGMap* pUGMap = &(m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map);
	// close map
	pUGMap->Reset();
	pUGMap->m_strName = _U("UntitledMap");
	pUGMap->SetModifiedFlag(false);
	m_pMapControl->Refresh();

	// close workspace
	UGWorkspace* pWorkspace = pUGMap->GetWorkspace();
	pWorkspace->Close();

	Pan();
}

void JunSuMFCDialogDlg::OnPanMap()
{
	ResetLastEditLayer(NULL);
	Pan();
}

void JunSuMFCDialogDlg::Pan()
{
	UGMapEditorWnd* pMapEditorWnd = m_pMapControl->GetMapEditWnd();
	pMapEditorWnd->SetUserAction(UGC::UGDrawParamaters::uaPan);
}

void JunSuMFCDialogDlg::ResetLastEditLayer(UGLayer* pLayer)
{
	if (m_pLastEditLayer != NULL)
	{
		UGLayers* pLayers = &(m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map.m_Layers);
		pLayers->SetEditableLayer(m_pLastEditLayer, false);
		
	}
	m_pLastEditLayer = pLayer;
}

void JunSuMFCDialogDlg::OnEditSelect()
{
	m_pMapControl->GetMapEditWnd()->SetSelectionMode(UGMapWnd::smContainCentroid);
	m_pMapControl->GetMapEditWnd()->SetUserAction(UGDrawParamaters::UGMapUserAction::uaPointModeSelect);
}


void JunSuMFCDialogDlg::GetSelectedGeo()
{

	/*
	UGLayer* pCurLayer = m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map.m_Layers.GetCurrentLayer();
	UGList<EditGeometry*> geoList = m_pMapControl->GetMapEditWnd()->GetEditToolPack()->GetEditGeometry();
	int size = geoList.GetCount();
	if (size > 0) {


	}
	UGEditToolPack* pEditTool =  m_pMapControl->GetMapEditWnd()->GetEditToolPack();

	UGLayer* pLayer = m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map.m_Layers.GetHeadLayer();
	UGSelection* pSelection =  pLayer->GetSelection();
	int geoCount = pSelection->GetSize();
	if (geoCount > 0)
	{
		
	}
	*/
	
	UGLayer* pLayer = NULL;
	UGSelection* pSelection = NULL;
	int geoCount;
	UGLayers* pLayers = &(m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map.m_Layers);

	int id = -1;
	int count =  pLayers->GetTotalCount();
	for(int i=0; i<count; i++)
	{
		pLayer = pLayers->GetLayerAt(i);
		pSelection = pLayer->GetSelection();
		geoCount = pSelection->GetSize();
		
		if (geoCount > 0) 
		{
			
			for (int j = 0; j < geoCount; j++)
			{
				id = pSelection->GetAt(j);
				break;
			}
		}
		if (id != -1) {

			// change selection style
			/*bool isCustomStyle = pSelection->HasCustomStyle();
			pSelection->SetCustomStyle(true);
			UGStyle style = pSelection->GetStyle();
			style.SetLineWidth(5);
			UGColor lcolor = style.GetLineColor();
		
			style.SetLineColor(0xFFFF0000);
			int size = style.GetMarkerSize();
			
			style.SetMarkerSize(3);

			pSelection->SetStyle(style);
			m_pMapControl->Refresh();
			pSelection->Add(id);*/
            break;
		}
			
	}

	//addLineIntoDynamicLayer();
	//addPlotObjectIntoCADDataset;
	createAnimationWithSelectedPlotSymbol(pSelection);
	//traceAnalyst(pSelection);

}


void JunSuMFCDialogDlg::OnDrawDrawplot()
{
	UGMapEditorWnd* pMapEditorWnd = m_pMapControl->GetMapEditWnd();
	UGMap* pUGMap = &(pMapEditorWnd->m_mapWnd.m_Map);

	UGString layerName = _U("DrawCAD@World");
	UGLayer* pLayer = pUGMap->m_Layers.GetLayer(layerName);
	if (pLayer != NULL)
	{
		ResetLastEditLayer(pLayer);

		pLayer->SetSelectable(true);

		UGLayers* pLayers = &(pUGMap->m_Layers);
		pLayers->SetEditableLayer(pLayer, true);

	    ActionGraphicObject plotAction;
		plotAction.param1 = plotJYid;
		plotAction.param2 = 30200;
		pMapEditorWnd->SetActionGraphicObject(UGDrawParamaters::uaEdit, plotAction);
	}
	else
	{

	}
}


void JunSuMFCDialogDlg::OnDrawPlotArraws()
{
	// TODO: Add your command handler code here

	UGMapEditorWnd* pMapEditorWnd = m_pMapControl->GetMapEditWnd();
	UGMap* pUGMap = &(pMapEditorWnd->m_mapWnd.m_Map);

	UGString layerName = _U("DrawCAD@World");
	UGLayer* pLayer = pUGMap->m_Layers.GetLayer(layerName);
	if (pLayer != NULL)
	{
		ResetLastEditLayer(pLayer);

		pLayer->SetSelectable(true);

		UGLayers* pLayers = &(pUGMap->m_Layers);
		pLayers->SetEditableLayer(pLayer, true);

		ActionGraphicObject plotAction;
		plotAction.param1 = plotTYid;
		plotAction.param2 = 1011;
		pMapEditorWnd->SetActionGraphicObject(UGDrawParamaters::uaEdit, plotAction);
	}
	else
	{

	}
}

//According to clicked position
void JunSuMFCDialogDlg::addLineOnDynamicLayer()
{
	m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map.m_DynamicLayers.Remove(_U("testDynamic"));
	UGDynamicLayer *pDyLayer = new UGDynamicLayer();
	pDyLayer->SetLayerName(_U("testDynamic"));


	UGStyle* pStyle = new UGStyle();
	//pStyle->SetMarkerSize(10);
	pStyle->SetMarkerStyle(4);

	UGPoint2Ds *pts = new UGPoint2Ds();
	UGPoint2D p1 = m_pMapControl->PixelToMap(pt.x, pt.y); // clicked position
	UGPoint2D p2 = m_pMapControl->PixelToMap(pt.x + 50, pt.y + 50); // offset 50 pixels
	pts->Add(p1);
	pts->Add(p2);

	UGGeoLine *pLine = new UGGeoLine();

	pLine->AddSub(pts->GetData(), 2);
	pStyle->SetLineWidth(3);
	pStyle->SetLineColor(UGColor(16711680));

	pLine->SetStyle(pStyle);

	pDyLayer->Add(UGString(_U("L")), pLine);

	m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map.m_DynamicLayers.Add(pDyLayer);

}

//According to clicked position
void JunSuMFCDialogDlg::addPlotObjectIntoCADDataset()
{
	// GrahpicObject/Plot
	UGWorkspace* pWorkSpace = m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map.GetWorkspace();
	UGDataSource* datasource = pWorkSpace->GetDataSource(_U("World"));
	UGDatasetVector *pToDataset = (UGDatasetVector*)datasource->GetDataset(_U("DrawCAD"));
	pToDataset->GetType();


	UGQueryDef queryDef;

	pToDataset->Open(); // pToRecordset == 0;

	queryDef.m_nType = UGQueryDef::General;
	if (pToDataset->GetType() == UGDataset::Tabular)
	{
		queryDef.m_nOptions = UGQueryDef::Attribute;
	}
	else
	{
		queryDef.m_nOptions = UGQueryDef::Both;
	}
	queryDef.m_nMode = UGQueryDef::GeneralQuery;
	queryDef.m_nCursorType = UGQueryDef::OpenDynamic;


	UGRecordset *pToRecordset = pToDataset->Query(queryDef);
	pToRecordset->MoveFirst();
	UGGeoPoint *piont = new UGGeoPoint();

	UGGraphicObject* plotGeo = new UGGraphicObject();
	UGPoint3D pt3D;
	UGPoint3Ds pt3Ds;

	int action = 0;
	int libId = plotJYid;
	int symbolCode = 30200;

	UGPoint2D clickPoint = m_pMapControl->PixelToMap(pt.x, pt.y);

	pt3D.x = clickPoint.x;
	pt3D.y = clickPoint.y;
	pt3Ds.Add(pt3D);

	plotGeo->SetGeometryShape(libId, symbolCode, pt3Ds);

	bool isTrue = pToRecordset->AddNew(plotGeo);
	bool isUp = pToRecordset->Update();

	m_pMapControl->Refresh();

	pToRecordset->Close();
	delete pToRecordset;
	pToDataset->Close();
}

// 需要一个Timer去执行UGAnimationManager::Instance()->Execute(); 参加TimerProc, SetTimer
void JunSuMFCDialogDlg::createAnimationWithSelectedPlotSymbol(UGSelection* pSelection)
{
	// Selected Geometry
	int id = pSelection->GetAt(0);
	pSelection->RemoveAll();
	UGLayer* pLayer = pSelection->Getlayer();
	UGDatasetVector* pSelectedDataset = (UGDatasetVector*)pLayer->GetDataset();
	UGQueryDef queryDef;

	pSelectedDataset->Open();

	queryDef.m_nType = UGQueryDef::General;
	if (pSelectedDataset->GetType() == UGDataset::Tabular)
	{
		queryDef.m_nOptions = UGQueryDef::Attribute;
	}
	else
	{
		queryDef.m_nOptions = UGQueryDef::Both;
	}
	queryDef.m_nMode = UGQueryDef::GeneralQuery;
	queryDef.m_nCursorType = UGQueryDef::OpenStatic;

	UGString filter;
	filter.Format(_U("SmID=%d"), id);
	queryDef.m_strFilter = filter;

	UGRecordset *pRecordset = pSelectedDataset->Query(queryDef);
	if (pRecordset == NULL)
	{
		return;
	}

	UGGeometry* pGeo = NULL;
	pRecordset->GetGeometry(pGeo);
	string geoType = typeid(*pGeo).name();
	string typeName = "class UGC::UGGraphicObject";

	if (typeName.compare(geoType) == 0) {
		UGAnimationManager* pAnimationManager = UGAnimationManager::Instance();
		pAnimationManager->Stop();

		UGString animationGroup = _U("group");

		UGAnimationGroup* pGroup = pAnimationManager->GetGroupByName(animationGroup);
		if (pGroup == NULL) {
			pGroup = pAnimationManager->AddAnimationGroup(animationGroup);
		}

		UGString scaleAnimation = _U("ScaleAnimation");
		UGAnimationGO* pAnimation = pGroup->GetAnimationByName(scaleAnimation);
		if (pAnimation == NULL)
		{
			pAnimation = pAnimationManager->CreateAnimation(UGAnimationType::ScaleAnimation);
			pAnimation->SetName(scaleAnimation);
		}
		else
		{
			//delete pAnimation->GetGeometry();
		}

		UGString layerName = pLayer->GetName();

		pAnimation->SetGeometry((UGGeometry3D*)pGeo, m_pMapControl->GetMapEditWnd(), layerName);

		pAnimation->SetStartTime(0);
		pAnimation->SetDuration(10);

		// 
		((UGScaleAnimation*)pAnimation)->SetStartScaleFactor(0);
		((UGScaleAnimation*)pAnimation)->SetEndScaleFactor(1);

		pGroup->AddAnimation(pAnimation);

		pGroup->SetStartTime(0);
		bool isTrue = pAnimationManager->SetPlayRange(0, 1);

		pAnimationManager->Play();

	}
}

// According to selected Geometry
void JunSuMFCDialogDlg::traceAnalyst(UGSelection* pSelection)
{
	int id = pSelection->GetAt(0);
	UGLayer* pLayer = pSelection->Getlayer();
	UGDataset* pDataset = pLayer->GetDataset();
	UGString name = pDataset->GetName();
	int index = name.Find(_U("_Network"));
	if (index == -1)
		return;
	UGString datasetName = name.Left(index + 8);
	UGDatasetVector* pDatasetNetwork = (UGDatasetVector*) pDataset->GetDataSource()->GetDataset(datasetName);
	// Trace down or trace up
	//UGDatasetVector* pDatasetNetwork = (UGDatasetVector*)m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map.GetWorkspace()->GetDataSource(0)->GetDataset(6);
	UGUtilityAnalyseParams params;
	UGUtilityAnalyst* pAnalyst = new UGUtilityAnalyst();

	params.nTraceID = id; // id = 6
	params.bFindWithoutLoops = false;
	params.nSearchMode = 0;// 0: up, 1: down
	params.strCostName = _U("Length");

	pAnalyst->SetDatasetNetwork(pDatasetNetwork);
	pAnalyst->SetNodeIDField(_U("SmNodeID"));
	pAnalyst->SetArcIDField(_U("SmEdgeID"));
	pAnalyst->SetFNodeIDField(_U("SmFNode"));
	pAnalyst->SetTNodeIDField(_U("SmTNode"));
	pAnalyst->SetDirection(_U("SmUserID"));

	UGArray<UGCostFields> arrCostFields;
	//
	UGCostFields costFields;
	costFields.strCostName = _U("Length");
	costFields.strFTField = _U("SmLength");
	costFields.strTFField = _U("SmLength");
	arrCostFields.Add(costFields);

	pAnalyst->SetCostFields(arrCostFields);
	pAnalyst->SetNodeInterval(0);

	bool isCreated = pAnalyst->CreateTraceAdjMatrix();
	//
	UGArray<UGuint> edgesArray; // 弧段ID数组，在网络数据集的主数据集(线)中获取对应对象
	UGArray<UGuint> nodesArray; // 节点ID数组，在网络数据集的子数据集(点)中获取对应对象
	int count = 0;
	int count1 = 0;
	//
	if (pAnalyst->Trace(params, edgesArray, nodesArray))
	{
		count = edgesArray.GetSize();
		count1 = nodesArray.GetSize();
	}
	else
	{
		count = edgesArray.GetSize();
		count1 = nodesArray.GetSize();
	}
	UGArray<UGuint> edgesArray1;
	UGArray<UGuint> nodesArray1;
	if (pAnalyst->TraceFromArc(params, edgesArray1, nodesArray1))
	{
		count = edgesArray1.GetSize();
		count1 = nodesArray1.GetSize();
	}
	else
	{
		count = edgesArray1.GetSize();
		count1 = nodesArray1.GetSize();
	}
	edgesArray.RemoveAll();
	edgesArray1.RemoveAll();
	nodesArray.RemoveAll();
	nodesArray1.RemoveAll();
	delete pAnalyst;
}
