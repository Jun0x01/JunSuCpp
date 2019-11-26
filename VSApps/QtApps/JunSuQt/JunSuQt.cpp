/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.05.15
 */
#include "JunSuQt.h"
#include "Data/LicenseManager.h"
#include <QDebug>

#include "PathAnalyst/UGPathAnalyst.h"
#include "NetworkEnvironment/UGNetworkAnalyst.h"
#include "NetworkEnvironment/UGNetworkTypesEx.h"

#include "GridAnalyst/UGDistanceAnalysis.h"
#include "Geometry/UGGeoLine.h"


#include "GeometryPlot/UGGOLibraryManager.h"

/**************** CallBack *********************/
void SuCALLBACK InvalidateCallback(void * pWnd)
{
	JunSuQt* pThis = (JunSuQt*)pWnd;

	pThis->update();

}
/**************** End of CallBack *********************/

/*************** Init **********************/
JunSuQt::JunSuQt(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setMouseTracking(true);
	centralWidget()->setMouseTracking(true);

	initMenuBar();
	//initToolBar();
    bool isValid = LicenseManager::IsValidStd();
//    if(isValid){
        m_pMapControl = new MapControl(InvalidateCallback, this);
//    }else {

//        cout << "许可无效，请更新许可";
//    }

		pDyLayer = new UGDynamicLayer();
		m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map.m_DynamicLayers.Add(pDyLayer);

        UGString strJYLibPath = _U("..\\..\\..\\TestData\\Plot\\JY.plot");
        UGString strTYLibPath = _U("..\\..\\..\\TestData\\Plot\\TY.plot");

        UGGOLibraryManager* pLibManager = UGGOLibraryManager::GetInstance();
        if (NULL == pLibManager)
        {
            return;
        }
        //添加标号库
        UGint nJYLibId = pLibManager->AddGOLibrary(strJYLibPath);
        UGint nTYLibId = pLibManager->AddGOLibrary(strTYLibPath);
}

JunSuQt::~JunSuQt() {

	delete m_pMapControl;
}


void JunSuQt::initMenuBar() {
	QMenuBar* menuBar = this->menuBar();
	// Menu -> File
	{
		QMenu* menu = new QMenu("File", menuBar);
		// File -> New
		{
			QAction* menuAction = new QAction("New", this);
			menuAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));

			//menu->addAction(menuAction);
			//menu->addSeparator();
		}
		// File -> Open
		{
			QAction* menuAction = new QAction("Open", this);
			menuAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_O));
			
			menu->addAction(menuAction);
			menu->addSeparator();

			connect(menuAction, SIGNAL(triggered()), this, SLOT(Menu_File_Open()));
		}
		// File -> Close
		{
			QAction* menuAction = new QAction("Close", this);
			menuAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_C));

			menu->addAction(menuAction);
			menu->addSeparator();
		}
		menuBar->addMenu(menu);
	}
	// Menu -> Edit
	{
		QMenu* menu = new QMenu("Edit", menuBar);
		// Eidt -> Pan Map
		{
			QAction* menuAction = new QAction("Pan Map", this);
			menu->addAction(menuAction);
			menu->addSeparator();

            connect(menuAction, SIGNAL(triggered()), this, SLOT(Menu_Edit_PanMap()));
		}
		menuBar->addMenu(menu);
	}
	// Menu -> Draw
	{
		QMenu* menu = new QMenu("Draw", menuBar);
		// Draw -> Point
		{
			QAction* menuAction = new QAction("Draw Point", this);
			menu->addAction(menuAction);
			menu->addSeparator();

			connect(menuAction, SIGNAL(triggered()), this, SLOT(Menu_Draw_Point()));
		}
		// Draw -> Line
		{
			QAction* menuAction = new QAction("Draw Line", this);
			menu->addAction(menuAction);
			menu->addSeparator();

			connect(menuAction, SIGNAL(triggered()), this, SLOT(Menu_Draw_Line()));
		}
		// Draw -> Polygon
		{
			QAction* menuAction = new QAction("Draw Polygon", this);
			menu->addAction(menuAction);
			menu->addSeparator();

			connect(menuAction, SIGNAL(triggered()), this, SLOT(Menu_Draw_Polygon()));
		}

		menuBar->addMenu(menu);
	}

    // Menu -> Analyst
    {
        QMenu* menu = new QMenu("Analyst", menuBar);
        // Analyst -> Network Find Path
        {
            QAction* menuAction = new QAction("Network Find Path", this);
            menu->addAction(menuAction);
            menu->addSeparator();

            connect(menuAction, SIGNAL(triggered()), this, SLOT(Menu_Analyst_NetworkFindPath()));
        }
        // Analyst -> Grid Best Path
        {
            QAction* menuAction = new QAction("Grid Best Path", this);
            menu->addAction(menuAction);
            menu->addSeparator();

            connect(menuAction, SIGNAL(triggered()), this, SLOT(Menu_Analyst_GridBestPath()));
        }
        menuBar->addMenu(menu);
    }

    // Menu -> DynamicLayer
    {
        QMenu* menu = new QMenu("DynamicLayer", menuBar);
        // DynamicLayer -> Add Line
        {
            QAction* menuAction = new QAction("Add All", this);
            menu->addAction(menuAction);
            menu->addSeparator();

            connect(menuAction, SIGNAL(triggered()), this, SLOT(Menu_Dynamic_AddAll()));
        }
        menuBar->addMenu(menu);
        // DynamicLayer -> Visible
        {
            QAction* menuAction = new QAction("Visible", this);
            menu->addAction(menuAction);
            menu->addSeparator();

            connect(menuAction, SIGNAL(triggered()), this, SLOT(Menu_Dynamic_Visible()));
        }
        menuBar->addMenu(menu);
        // DynamicLayer -> Invisible
        {
            QAction* menuAction = new QAction("Invisible", this);
            menu->addAction(menuAction);
            menu->addSeparator();

            connect(menuAction, SIGNAL(triggered()), this, SLOT(Menu_Dynamic_Invisible()));
        }
        menuBar->addMenu(menu);

		// DynamicLayer -> Add Point And Line
		{
			QAction* menuAction = new QAction("Add Point And Line", this);
			menu->addAction(menuAction);
			menu->addSeparator();

			connect(menuAction, SIGNAL(triggered()), this, SLOT(Menu_Dynamic_AddPointAndLine()));
		}
		menuBar->addMenu(menu);
    }

    // Menu -> CoordsysTransform
    {
        QMenu* menu = new QMenu("CoordsysTransform", menuBar);
        // Eidt -> Pan Map
        {
            QAction* menuAction = new QAction("GeoCoordToPixel", this);
            menu->addAction(menuAction);
            menu->addSeparator();

            connect(menuAction, SIGNAL(triggered()), this, SLOT(Menu_Coordsys_GeoCoordToPixel()));
        }
        menuBar->addMenu(menu);
    }

	// Menu -> Symbol
//	{
//		QMenu* menu = new QMenu("Symbol", menuBar);
//		// Eidt -> Create Marker Symbol
//		{
//			QAction* menuAction = new QAction("Create Marker Symbol", this);
//			menu->addAction(menuAction);
//			menu->addSeparator();

//			connect(menuAction, SIGNAL(triggered()), this, SLOT(addPointCutomSymbol()));
//		}
//		menuBar->addMenu(menu);
//	}

    // Menu -> Layers
    {
        QMenu* menu = new QMenu("Layers", menuBar);
        // Layers -> Visible
        {
			QAction* menuAction = new QAction("Visible_CAD", this);
            menu->addAction(menuAction);
            menu->addSeparator();

            connect(menuAction, SIGNAL(triggered()), this, SLOT(Menu_Layers_Visible()));
        }
        // Layers -> Invisible
        {
			QAction* menuAction = new QAction("Invisible_CAD", this);
            menu->addAction(menuAction);
            menu->addSeparator();

            connect(menuAction, SIGNAL(triggered()), this, SLOT(Menu_Layers_Invisible()));
        }

        // Layers -> Visible
        {
            QAction* menuAction = new QAction("Visible_Point", this);
            menu->addAction(menuAction);
            menu->addSeparator();

            connect(menuAction, SIGNAL(triggered()), this, SLOT(Menu_Layers_Visible1()));
        }
        // Layers -> Invisible
        {
			QAction* menuAction = new QAction("Invisible_Point", this);
            menu->addAction(menuAction);
            menu->addSeparator();

            connect(menuAction, SIGNAL(triggered()), this, SLOT(Menu_Layers_Invisible1()));
        }
        menuBar->addMenu(menu);


    }
}

void JunSuQt::initToolBar() {
	QToolBar* toolBar = addToolBar("ToolBar");
	{
		{
			QAction* action = new QAction(this);
			action->setToolTip("Open");
			action->setIcon(QIcon(":/JunSuQt/Resources/open.png"));

			toolBar->addAction(action);

			connect(action, SIGNAL(triggered()), this, SLOT(Menu_File_Open()));
		}
		// Draw Line
		{
			QAction* action = new QAction(this);
			action->setToolTip("Draw Line");
			action->setIcon(QIcon(":/JunSuQt/Resources/Line.png"));

			toolBar->addAction(action);

			connect(action, SIGNAL(triggered()), this, SLOT(Menu_Draw_Line()));
		}
		
	}
}
/******************* End of Init **********************/

/*********** Override ************/
void JunSuQt::paintEvent(QPaintEvent* event)
{
	m_pMapControl->OnDraw(0, 0, this->width(), this->height());
	uchar* imgData = m_pMapControl->GetImageBytes();
	if (imgData != NULL)
	{
		QImage* pQImage = new QImage(imgData, this->width(), this->height(), QImage::Format_ARGB32);

		QColor background(255, 255, 255);
		QPainter painter; 
		painter.begin(this);
		painter.fillRect(0, 0, this->width(), this->height(), background);
		painter.drawImage(QRectF(0, 0, this->width(), this->height()), *pQImage);

		// paint signal / slot

		painter.end();
		delete pQImage;
		
	}
	else {
		// error
	}

	
}
unsigned int JunSuQt::getMouseOrKeyFlag(QMouseEvent* event)
{
	unsigned int flag = 0;

	if (event->modifiers() & Qt::ShiftModifier)
	{
		flag = flag | UG_MK_SHIFT;
	}
	if ((event->modifiers() & Qt::ControlModifier))
	{
		flag = flag | UG_MK_CONTROL;
	}
	return flag;
}

void JunSuQt::wheelEvent(QWheelEvent* event)
{
	m_pMapControl->OnMouseWheel(0, event->delta(), event->x(), event->y());
}
void JunSuQt::mousePressEvent(QMouseEvent* event)
{
	int flag = getMouseOrKeyFlag(event);
	
	if (event->button() == Qt::LeftButton) 
	{// LeftButton
		m_pMapControl->OnLMouseDown(flag, event->x(), event->y());
	}
	else if (event->button() == Qt::RightButton)
	{// RightButton
		m_pMapControl->OnRMouseDown(flag, event->x(), event->y());
	}
	else if (event->button() == Qt::MidButton)
	{// MidButton
		m_pMapControl->OnMidMouseDown(flag, event->x(), event->y());
	}

	pt.setX(event->x());
	pt.setY(event->y());
}
void JunSuQt::mouseReleaseEvent(QMouseEvent* event)
{
	int flag = getMouseOrKeyFlag(event);

	if (event->button() == Qt::LeftButton)
	{// LeftButton
		m_pMapControl->OnLMouseUp(flag, event->x(), event->y());
	}
	else if (event->button() == Qt::RightButton)
	{// RightButton
		m_pMapControl->OnRMouseUp(flag, event->x(), event->y());
	}
	else if (event->button() == Qt::MidButton)
	{// MidButton
		m_pMapControl->OnMidMouseUp(flag, event->x(), event->y());
	}
}

void JunSuQt::mouseDoubleClickEvent(QMouseEvent *event)
{
	int flag = getMouseOrKeyFlag(event);

	if (event->button() == Qt::LeftButton)
	{// LeftButton double click, zoom in
		m_pMapControl->OnLMouseDbClick(flag, event->x(), event->y());
	}
}
void JunSuQt::mouseMoveEvent(QMouseEvent* event)
{
	int flag = getMouseOrKeyFlag(event);

    if(m_pMapControl != NULL)
	m_pMapControl->OnMouseMove(flag, event->x(), event->y());

}
void JunSuQt::resizeEvent(QResizeEvent* event)
{
    if(m_pMapControl != NULL)
	m_pMapControl->OnSizeChanged(this->width(), this->height());
}
void JunSuQt::keyPressEvent(QKeyEvent* event)
{

}

/*********** End of Override*****************/

/************ Menu Functions ****************************/

void JunSuQt::Menu_File_Open()
{
	QString qStrPath = QFileDialog::getOpenFileName(this, QString::fromUtf8("OpenWorksapce"), "../../../TestData/", tr("smwu(*.smwu)"));

	if (!qStrPath.isEmpty())
	{
		string strPath = qStrPath.toStdString();
		OpenWorkspace(strPath);
	}
	else
	{

	}
}

void JunSuQt::Menu_File_Close()
{
	CloseWorkspace();
}
void JunSuQt::Menu_Draw_Point()
{
	UGMapEditorWnd* pMapEditorWnd = m_pMapControl->GetMapEditWnd();
	UGMap* pUGMap = &(pMapEditorWnd->m_mapWnd.m_Map);

	UGString layerName = _U("DrawPoint@World");
	UGLayer* pLayer = m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map.m_Layers.GetLayer(layerName);
	if (pLayer != NULL)
	{
		pLayer->SetSelectable(true);

		UGLayers* pLayers = &(pUGMap->m_Layers);
		pLayers->SetEditableLayer(pLayer, true);

		pMapEditorWnd->SetUserAction(UGC::UGDrawParamaters::uaEdit, UGC::UGEditType::ET_Point);
	}
	else
	{

	}
}
void JunSuQt::Menu_Draw_Line()
{
	UGMapEditorWnd* pMapEditorWnd = m_pMapControl->GetMapEditWnd();
	UGMap* pUGMap = &(pMapEditorWnd->m_mapWnd.m_Map);

	UGString layerName = _U("DrawLine@World");
	UGLayer* pLayer = m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map.m_Layers.GetLayer(layerName);
	if (pLayer != NULL)
	{
		pLayer->SetSelectable(true);

		UGLayers* pLayers = &(pUGMap->m_Layers);
		pLayers->SetEditableLayer(pLayer, true);

		pMapEditorWnd->SetUserAction(UGC::UGDrawParamaters::uaEdit, UGC::UGEditType::ET_PolyLine);
	}
	else
	{

	}
}
void JunSuQt::Menu_Draw_Polygon()
{
	UGMapEditorWnd* pMapEditorWnd = m_pMapControl->GetMapEditWnd();
	UGMap* pUGMap = &(pMapEditorWnd->m_mapWnd.m_Map);

	UGString layerName = _U("DrawRegion@World");
	UGLayer* pLayer = m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map.m_Layers.GetLayer(layerName);
	if (pLayer != NULL)
	{
		pLayer->SetSelectable(true);

		UGLayers* pLayers = &(pUGMap->m_Layers);
		pLayers->SetEditableLayer(pLayer, true);

		pMapEditorWnd->SetUserAction(UGC::UGDrawParamaters::uaEdit, UGC::UGEditType::ET_PolyGon);
	}
	else
	{

	}
}

/************* End of Menu functons******************************************/

void JunSuQt::OpenWorkspace(string strPath)
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

void JunSuQt::CloseWorkspace()
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

void JunSuQt::Menu_Edit_PanMap()
{
	ResetLastEditLayer(NULL);
	Pan();
}

void JunSuQt::Pan()
{
	UGMapEditorWnd* pMapEditorWnd = m_pMapControl->GetMapEditWnd();
	pMapEditorWnd->SetUserAction(UGC::UGDrawParamaters::uaPan);
}

void JunSuQt::ResetLastEditLayer(UGLayer* pLayer)
{
	if (m_pLastEditLayer != NULL)
	{
		UGLayers* pLayers = &(m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map.m_Layers);
		pLayers->SetEditableLayer(m_pLastEditLayer, false);
	}
	m_pLastEditLayer = pLayer;
}

void JunSuQt::Menu_Analyst_NetworkFindPath()
{
    UGWorkspace* pWorkspace = m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map.GetWorkspace();

        UGDataSource* pDatasource = pWorkspace->GetDataSource(0);

        UGDatasetVector* pNetworDataset = (UGDatasetVector *)pDatasource->GetDataset(_U("BuildNetwork"));

        if(pNetworDataset == nullptr){

            qWarning("请先打开工作空间networkdatset.swmu");
            return;
        }
        UGNetworkAnalyst* pNetworkAnalyst = new UGNetworkAnalyst();
        pNetworkAnalyst->SetDatasetNetwork(pNetworDataset);
        pNetworkAnalyst->SetFTWeightField(_U("SmLength"));
        pNetworkAnalyst->SetTFWeightField(_U("SmLength"));
        pNetworkAnalyst->SetNodeIDField(_U("SmNodeID"));
        pNetworkAnalyst->SetArcIDField(_U("SmEdgeID"));
        pNetworkAnalyst->SetFNodeIDField(_U("SmFNode"));
        pNetworkAnalyst->SetTNodeIDField(_U("SmTNode"));
        pNetworkAnalyst->SetNodeInterval(0);


        UGAnalyseParams params;
        //! brief网络分析路径点,设施点,配送点节点ID数组,
        UGArray<UGuint> arrViaNodeIDs;
        arrViaNodeIDs.Add(1);
        arrViaNodeIDs.Add(3);
        //params.arrViaNodeIDs.Add(1);
        //params.arrViaNodeIDs.Add(3);
        params.nOptions = UGNetworkAnalystEnv::UGRIRoutes;
        //! brief网络分析路径点,设施点,配送点坐标点数组
        UGPoint2Ds      arrViaPoints;

        UGPoint2D pt1;
        UGPoint2D pt2;
        pt1.x = 104.066265539623;
        pt1.y = 30.5410647678783;
        pt2.x = 104.066362985461;
        pt2.y = 30.5383626570618;
        params.arrViaPoints.Add(pt1);
        params.arrViaPoints.Add(pt2);

        params.strCostName = _U("Length");

        UGCostFields costFields;
        costFields.strCostName = _U("Length");
        costFields.strFTField = _U("SmLength");
        costFields.strTFField = _U("SmLength");

        UGArray<UGCostFields> costFs;
        costFs.Add(costFields);
        pNetworkAnalyst->SetCostFields(costFs);

        UGArray<UGuint> arrBarrierNodeIDs;
        arrBarrierNodeIDs.Add(4);


        bool isCreated = pNetworkAnalyst->CreateAdjMatrix();
        bool isSet = pNetworkAnalyst->SetBarrierNodes(arrBarrierNodeIDs);

        UGPathAnalyst* pAnalyst = new UGPathAnalyst(pNetworkAnalyst);

        UGResultInfo resultInfo;
        double isFound = pAnalyst->FindPath(params, resultInfo, true);
        UGArray<UGPoint2D> pointsM;
        UGArray<UGPoint2D> pointsL;
        UGPoint3Ds *pPiont3Ds;
        UGGeoLineM *pLineM = NULL;

        if (isFound != -1) {
            int lineCount = resultInfo.arrLineM.GetSize();


            for (int i = 0; i < lineCount; i++) {
                pLineM = new UGGeoLineM(resultInfo.arrLineM.GetAt(i));
                //pLineM = &(resultInfo.arrLineM.GetAt(i));

               UGGeoLine geoLine = pLineM->GetLine();


                int countM = pLineM->GetPointCount();
                for (int j = 0; j < countM; j++) {
                    UGPoint2D p1 = pLineM->GetPointAtIndex(j);
                    UGPoint2D p;
                    pLineM->GetPointAtIndex(p, j);
                    pointsM.Add(p);
                }

                int countL = geoLine.GetPointCount();
                for (int j = 0; j < countL; j++) {
                    UGPoint2D p;
                    pLineM->GetPointAtIndex(p, j);
                    pointsL.Add(p);
                }
            }
//            cout << "LineCount " << lineCount << endl;
        }
        else {
//            cout << "Not found" << endl;
        }

}
void JunSuQt::Menu_Analyst_GridBestPath()
{
    UGWorkspace* pWorkspace = m_pMapControl->GetMapEditWnd()->m_mapWnd.GetWorkspace();
        UGDataSource* pDS = pWorkspace->GetDataSource(_U("dem"));
        if(pDS == nullptr){

            qWarning("请先打开工作空间dem.swmu");
            return;
        }

        UGDatasetRaster* pDatasetRaster = (UGDatasetRaster*)pDS->GetDatasets()->GetAt(0);

        UGPoint2D startP(109.35, 39.3819444444445);
        UGPoint2D destP(109.495555555556, 39.6980555555556);
        double totalCost = 0;
        UGDistanceAnalysis distanceAnalysis;

        //dMaxUpsolpeDegree 和dMaxDownslopeDegree 大于等于90时不考虑坡度，
        // nSmoothMethod: -1: 不光滑处理; 0: B样条法; 1: 磨角法
        UGGeoLine* pGeoLine = distanceAnalysis.BestPathLine(startP, destP,totalCost, nullptr, pDatasetRaster, 90, 90, -1);

        UGDatasetVector* p2 = (UGDatasetVector *)pDS->GetDatasets()->GetAt(1);

        p2->Open();
        UGQueryDef queryDef;
        queryDef.m_nType = UGQueryDef::General;

        queryDef.m_nOptions = UGQueryDef::Both;

        queryDef.m_nMode = UGQueryDef::GeneralQuery;
        queryDef.m_nCursorType = UGQueryDef::OpenDynamic;

        UGRecordset *pRecordset = p2->Query(queryDef);
        //pRecordset->Edit();
        bool isAdd = pRecordset->AddNew(pGeoLine);
        if (isAdd) {
            bool isUpdate = pRecordset->Update();
            if (isUpdate) {
//                cout << "ok" <<endl;
            }
        }

        pRecordset->Close();
        delete pRecordset;

        m_pMapControl->Refresh();
}



void JunSuQt::Menu_Dynamic_AddAll()
{

        UGStyle* pStyle = new UGStyle();
        
        pStyle->SetLineWidth(3);
		pStyle->SetLineColor(UGColor(0xFF000000));   // Black
        //pStyle->SetLineColor(UGColor(0x00FF0000)); // Blue
        pStyle->SetLineColor(UGColor(0x0000FF00)); // Green
        pStyle->SetLineColor(UGColor(0x000000FF)); // Red
		pStyle->SetFillOpaqueRate(50);

        UGPoint2Ds *pts = new UGPoint2Ds();
        UGPoint2D p1 = m_pMapControl->PixelToMap(pt.x(), pt.y()); // clicked position
        UGPoint2D p2 = m_pMapControl->PixelToMap(pt.x() + 50, pt.y() + 50); // offset 50 pixels
		UGPoint2D p3 = m_pMapControl->PixelToMap(pt.x() + 20, pt.y() + 100); 

        pts->Add(p1);
        pts->Add(p2);
		pts->Add(p3);

        UGGeoLine *pLine = new UGGeoLine();

        pLine->AddSub(pts->GetData(), 2);



        pLine->SetStyle(pStyle);

		UGGeoPoint* pPoint = new UGGeoPoint();
		pPoint->SetX(p1.x);
		pPoint->SetY(p1.y);
		pStyle->SetMarkerSize(10);
		//pStyle->SetMarkerStyle(4);
		//pPoint->SetStyle(pStyle);

		UGGeoRegion* pRegion = new UGGeoRegion();
		pRegion->AddSub(pts->GetData(), 3);

		pStyle->SetFillForeColor(0x00FF00);

		pRegion->SetStyle(pStyle);

		delete pStyle;

	

        pDyLayer->Add(UGString(_U("L")), pLine);
	    pDyLayer->Add(UGString(_U("P")), pPoint);
		pDyLayer->Add(UGString(_U("R")), pRegion);
//        pLine->GetStyle()->SetIsVisible(false);
		//delete pLine;

//        m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map.m_DynamicLayers.Add(pDyLayer);
        m_pMapControl->Refresh();
}

void JunSuQt::Menu_Dynamic_Visible()
{
     pDyLayer->Find(_U("L"))->GetStyle()->SetIsVisible(true);
     m_pMapControl->Refresh();
}
void JunSuQt::Menu_Dynamic_Invisible()
{
     pDyLayer->Find(_U("L"))->GetStyle()->SetIsVisible(false);

     m_pMapControl->Refresh();
}
void JunSuQt::Menu_Dynamic_AddPointAndLine()
{
	UGPoint2Ds *pts = new UGPoint2Ds();
	UGPoint2D p1 = m_pMapControl->PixelToGeoCoord(pt.x(), pt.y() + 200); // clicked position
	UGPoint2D p2 = m_pMapControl->PixelToGeoCoord(pt.x() + 50, pt.y() + 250); // offset 50 pixels
	UGPoint2D p3 = m_pMapControl->PixelToGeoCoord(pt.x() + 20, pt.y() + 300);

	pts->Add(p1);
	pts->Add(p2);
	pts->Add(p3);

	m_pMapControl->AddDynamicPoint(_U("P1"), p1.x, p1.y);
	m_pMapControl->AddDynamicLine(_U("L1"), p1.x, p1.y, p2.x, p2.y);
	m_pMapControl->AddDynamicLine(_U("L2"), *pts);

	m_pMapControl->Refresh();
}

void JunSuQt::Menu_Coordsys_GeoCoordToPixel()
{
	UGPoint2D pmap = m_pMapControl->PixelToMap(pt.x(), pt.y());
    UGPoint2D p = m_pMapControl->PixelToGeoCoord(pt.x(), pt.y()); // clicked position : pt
    UGPoint pt1 = m_pMapControl->GeoCoordToPixel(p.x, p.y);
	qDebug("Map: pmap.x: %lf, pmap.y: %lf", pmap.x, pmap.y);
	qDebug("Pixel: pt.x: %d, pt.y: %d", pt.x(), pt.y());
	qDebug("Pixel tans: pt1.x: %d, pt1.y: %d", pt1.x, pt1.y);
	qDebug("GeoP: p.x: %lf, pt.y: %lf", p.x, p.y);

	UGPoint2Ds pts;
	pts.Add(p);
	bool isTomap = m_pMapControl->ToMapCoords(pts);
	UGPoint2D ptMap = pts.GetAt(0);

	pts.RemoveAll();
	pts.Add(p);
	UGPoints outPts;
	bool isToPixel = m_pMapControl->ToPixels(pts, outPts);

	UGPoint ptPixel = outPts.GetAt(0);

	UGPoints ptsPixels;
	ptsPixels.Add(pt1);
	UGPoint2Ds outPt2Ds;
	bool isToGeo = m_pMapControl->ToGeoCoords(ptsPixels, outPt2Ds);

	UGPoint2D ptGeo = outPt2Ds.GetAt(0);

	qDebug("ToMap, isToMap: %d, ptMap.x: %lf, ptMap.y: %lf", isTomap, ptMap.x, ptMap.y);
	qDebug("ToPixels, isToPixels: %d, ptPixel.x: %d, ptPixel.y: %d", isToPixel, ptPixel.x, ptPixel.y);
	qDebug("ToGeo, isToGeo: %d, ptGeo.x: %lf, ptGeo.y, %lf", isToGeo, ptGeo.x, ptGeo.y);
}

void JunSuQt::Menu_Layers_Visible(){

    UGLayers* pUGLayers = m_pMapControl->GetUGLayers();
    UGLayer* pLayer = pUGLayers->GetLayer(_U("DrawCAD@World"));
    if(NULL != pLayer)
        pLayer->SetVisible(true);
    m_pMapControl->Refresh();

}


void JunSuQt::Menu_Layers_Invisible(){
    UGLayers* pUGLayers = m_pMapControl->GetUGLayers();
    UGLayer* pLayer = pUGLayers->GetLayer(_U("DrawCAD@World"));
    if(NULL != pLayer)
        pLayer->SetVisible(false);
    m_pMapControl->Refresh();
}

void JunSuQt::Menu_Layers_Visible1(){

    UGLayers* pUGLayers = m_pMapControl->GetUGLayers();
    UGLayer* pLayer = pUGLayers->GetLayer(_U("River_Network_Node@World"));
    if(NULL != pLayer)
        pLayer->SetVisible(true);
    m_pMapControl->Refresh();
}


void JunSuQt::Menu_Layers_Invisible1(){
    UGLayers* pUGLayers = m_pMapControl->GetUGLayers();
    UGLayer* pLayer = pUGLayers->GetLayer(_U("River_Network_Node@World"));
    if(NULL != pLayer)
        pLayer->SetVisible(false);
    m_pMapControl->Refresh();
}
