/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.05.15
 */
#include "JunSuQt.h"
#include "Data/LicenseManager.h"
#include "qlogging.h"

#include "PathAnalyst/UGPathAnalyst.h"
#include "NetworkEnvironment/UGNetworkAnalyst.h"
#include "NetworkEnvironment/UGNetworkTypesEx.h"

#include "GridAnalyst/UGDistanceAnalysis.h"
#include "Geometry/UGGeoLine.h"

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
        UGGeoLine *pLine = NULL;

        if (isFound != -1) {
            int lineCount = resultInfo.arrLineM.GetSize();


            for (int i = 0; i < lineCount; i++) {
                pLineM = new UGGeoLineM(resultInfo.arrLineM.GetAt(i));
                //pLineM = &(resultInfo.arrLineM.GetAt(i));

                pLine = &pLineM->GetLine();


                int countM = pLineM->GetPointCount();
                for (int j = 0; j < countM; j++) {
                    UGPoint2D p1 = pLineM->GetPointAtIndex(j);
                    UGPoint2D p;
                    pLineM->GetPointAtIndex(p, j);
                    pointsM.Add(p);
                }

                int countL = pLine->GetPointCount();
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
        delete pRecordset();

        m_pMapControl->Refresh();
}
