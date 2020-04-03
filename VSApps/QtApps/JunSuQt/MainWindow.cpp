#include "MainWindow.h"
#include "ui_MainWindow.h"


#include "GeometryPlot/UGGOLibraryManager.h"
#include "Theme3DBase/UGTheme3DUnique.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    statusBar()->hide(); // ����״̬��
	// ���������棬ʹ�༭����ʱЧ������
	this->setMouseTracking(true);
	this->centralWidget()->setMouseTracking(true);

    // ���WorkspaceView�������ռ��dock������
    pWorkspaceView = new WorkspaceView();
    ui->dockWidget_workspace->setWidget(pWorkspaceView);

	// ���ù����ռ��б���Ҽ��˵��ź�
	connect(pWorkspaceView, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(onCustomContextMenu(const QPoint&)));
	// ˫���ź�
	connect(pWorkspaceView, SIGNAL(doubleClickedItem(QTreeWidgetItem*)), this, SLOT(onDoubleClickedWorkspaceViewItem(QTreeWidgetItem*)));

    /***************** �����ź� ***********************/
	// MenuBar -> File
    connect(ui->actionNew,    SIGNAL(triggered()), SLOT(Menu_File_New()));
    connect(ui->actionOpen,   SIGNAL(triggered()), SLOT(Menu_File_Open()));
    connect(ui->actionSave,   SIGNAL(triggered()), SLOT(Menu_File_Save()));
    connect(ui->actionSaveAs, SIGNAL(triggered()), SLOT(Menu_File_SaveAs()));
    connect(ui->actionExit,   SIGNAL(triggered()), SLOT(Menu_File_Exit()));

	// ToolBar -> File
	connect(ui->toolActionNew, SIGNAL(triggered()), SLOT(Menu_File_New()));
	connect(ui->toolActionOpen, SIGNAL(triggered()), SLOT(Menu_File_Open()));
	connect(ui->toolActionSave, SIGNAL(triggered()), SLOT(Menu_File_Save()));
	connect(ui->toolActionSaveAs, SIGNAL(triggered()), SLOT(Menu_File_SaveAs()));

    connect(ui->toolActionPan, SIGNAL(triggered()), SLOT(Tool_Pan()));
    connect(ui->toolActionSelect, SIGNAL(triggered()), SLOT(Tool_Select()));

    // MenuBar -> Scene
    connect(ui->actionGet_Selections,   SIGNAL(triggered()), SLOT(Menu_Scene_GetSelections()));
    connect(ui->actionAdd_Unique_Theme3D,   SIGNAL(triggered()), SLOT(Menu_Scene_Add_UniqueTheme3D()));

	/***************** ���������ź� ***********************/

    pWorkspace = new Workspace();

	/****************** ����MDI ***************************/
	// MDI
	pMdiArea = new QMdiArea();
	pMdiArea->setViewMode(QMdiArea::TabbedView);
	pMdiArea->setTabsClosable(true);
    pMdiArea->setMouseTracking(true);   // ���������棬ʹ�༭����ʱЧ������
	pMdiArea->setMinimumSize(300, 200);
	setCentralWidget(pMdiArea);
	connect(pMdiArea, &QMdiArea::subWindowActivated, this, &MainWindow::MDI_OnSubWindowActivated);

	pCurWorkspaceViewItem = NULL;
	pCurMapOrSceneWidget = NULL;
   /****************************************************/

   /****************** ���ر����ſ� ***************************/
	UGString strJYLibPath = _U("../../../TestData/Plot/JY.plot");
	UGString strTYLibPath = _U("../../../TestData/Plot/TY.plot");

	UGGOLibraryManager* pLibManager = UGGOLibraryManager::GetInstance();
	if (NULL == pLibManager)
	{
		return;
	}
	//��ӱ�ſ�
	//UGint nJYLibId = -1;
	UGint nJYLibId = pLibManager->AddGOLibrary(strJYLibPath);
	UGint nTYLibId = pLibManager->AddGOLibrary(strTYLibPath);

	qDebug() << "JYLibId: " << UGFile::IsExist(strJYLibPath) << ", id: " << nJYLibId << endl;
	qDebug() << "TYLibId: " << UGFile::IsExist(strTYLibPath) << ", id: " << nTYLibId << endl;
   /****************************************************/

	colorIndex = 0;
}

MainWindow::~MainWindow()
{
	pCurWorkspaceViewItem = NULL;

	CloseWorkspace();
	delete pWorkspace; // Must close map firstly.

	delete ui;
}

void MainWindow::CloseWorkspace()
{
    // First, close all associated maps
	QList<QMdiSubWindow*> subWndList = pMdiArea->subWindowList();
	for (QMdiSubWindow* pSubWnd : subWndList)
	{
		QWidget* pWidget = pSubWnd->widget();
		if (typeid(*pWidget) == typeid(MapView))
		{
			((MapView*)pWidget)->getMapControl()->CloseMap();
		}
		else if (typeid(*pWidget) == typeid(SceneView))
		{
			
		}
	}
    // Second, close workspace
    pWorkspace->Close();
}

void MainWindow::Menu_File_New()
{
	MapView* pMapView = new MapView();
	pMapView->setAttribute(Qt::WA_DeleteOnClose); // �رպ���ͷ�
	pMapView->getMapControl()->SetWorkspace(pWorkspace);
	QString title("New Map");

	pMapView->setWindowTitle(title);

	pMdiArea->addSubWindow(pMapView);
	pMapView->show();
}


void MainWindow::Menu_File_Open()
{
	QString filter = tr("Workspace (*.smwu *.sxwu *.smw *.sxw);;Datasource (*.udb *.sci)");
    QString qStrPath = QFileDialog::getOpenFileName(this, QString::fromUtf8("OpenWorksapce"), "../../../TestData/", filter);

    if (!qStrPath.isEmpty())
    {
		QFileInfo fileInfo(qStrPath);
		QString suffix = fileInfo.suffix();
	
        string strPath = qStrPath.toStdString();

		// �����ռ�
		if (0 == suffix.compare("smwu", Qt::CaseInsensitive) ||
			0 == suffix.compare("smw", Qt::CaseInsensitive)  ||
			0 == suffix.compare("sxwu", Qt::CaseInsensitive) ||
			0 == suffix.compare("sxw", Qt::CaseInsensitive)
			)
		{
			// Close first
			CloseWorkspace();

			if (pWorkspace->OpenWorkspaceFile(strPath))
			{
				// ���¹����ռ��б�
				pWorkspaceView->updateWorkspaceList(*pWorkspace);

			}
			else
			{
				// ���¹����ռ��б�
				pWorkspaceView->updateWorkspaceList(*pWorkspace);
				qDebug("Failed to open the workspace");
			}
		}
		else
		{
			UGC::UGEngineType engineType = UGC::NOEngine;
			if (0 == suffix.compare("udb", Qt::CaseInsensitive))
			{
				engineType = UGC::UDB;
			}
			else if (0 == suffix.compare("tif", Qt::CaseInsensitive))
			{
				engineType = UGC::ImagePlugins;
			}
			else if (0 == suffix.compare("tiff", Qt::CaseInsensitive))
			{
				engineType = UGC::ImagePlugins;
			}
			else if (0 == suffix.compare("sci", Qt::CaseInsensitive))
			{
				engineType = UGC::ImagePlugins;
			}
			
			QString fileName = fileInfo.baseName();
			UGDataSource* pDatasource = pWorkspace->OpenDatasourceFile(engineType, fileName.toStdString(), strPath);
			if (NULL != pDatasource)
			{
				// ���¹����ռ��б�����һ������Դ
				pWorkspaceView->updateNewDatasource(*pDatasource);
			}
			else
			{
				qDebug("Failed to open the datasource");
			}
		}
    }
    else
    {

    }
}

void MainWindow::Menu_File_Save()
{
	SceneView* sceneView = NULL;
	if (typeid(*pCurMapOrSceneWidget) == typeid(SceneView))
	{
		sceneView = (SceneView*)pCurMapOrSceneWidget;
		sceneView->GetSceneControl()->Save();
		sceneView = NULL;
	}
	bool isSaved = pWorkspace->Save();
	if (isSaved)
	{
		qDebug("Saved workspace.");
	}
	else
	{
		qDebug("Failed to save workspace");
	}
}

void MainWindow::Menu_File_SaveAs()
{
	qDebug("No action");
}

void MainWindow::Menu_File_Close()
{
	CloseWorkspace();
}

void MainWindow::Menu_File_Exit()
{
	close();
}

// Menus
void MainWindow::onCustomContextMenu(const QPoint& pos)
{

	QTreeWidgetItem* item = pWorkspaceView->currentItem();
	pCurWorkspaceViewItem = item;

	if (NULL != item)
	{
		WorkspaceView::ItemType type = (WorkspaceView::ItemType)item->data(0, pWorkspaceView->ItemDataType).toInt();
		if (type == WorkspaceView::TypeWorkspace)
		{
			QMenu menu;

			QAction* action1 = new QAction(tr("Open a workspace"));
			connect(action1, SIGNAL(triggered()), this, SLOT(Menu_File_Open()));
			menu.addAction(action1);

			QAction* action2 = new QAction(tr("Save"));
			connect(action2, SIGNAL(triggered()), this, SLOT(Menu_File_Save()));
			menu.addAction(action2);

			QAction* action3 = new QAction(tr("Save As"));
			connect(action3, SIGNAL(triggered()), this, SLOT(Menu_File_SaveAs()));
			menu.addAction(action3);

			QAction* action4 = new QAction(tr("Close"));
			connect(action4, SIGNAL(triggered()), this, SLOT(Menu_File_Close()));
			menu.addAction(action4);

			menu.exec(QCursor::pos());
		}
		else if (type == WorkspaceView::TypeMap)
		{
			QMenu menu;

			QAction* action1 = new QAction(tr("Open"));
			connect(action1, SIGNAL(triggered()), this, SLOT(onOpenMap()));
			menu.addAction(action1);

			menu.exec(QCursor::pos());
		}
		else if (type == WorkspaceView::TypeScene)
		{
			QMenu menu;

			QAction* action1 = new QAction(tr("Open"));
			connect(action1, SIGNAL(triggered()), this, SLOT(onOpenScene()));
			menu.addAction(action1);

			menu.exec(QCursor::pos());
		}
		else if (type == WorkspaceView::TypeDatasource)
		{

		}
		else if (type >= WorkspaceView::TypeDataset)
		{
			QMenu menu;

			// ��ͼʹ��
			QAction* action11 = new QAction(tr("Add to new map"));
			connect(action11, SIGNAL(triggered()), this, SLOT(onAddToNewMap()));
			menu.addAction(action11);
			// ���û�е�ͼ���ڣ�����Ӵ���
			QAction* action12 = new QAction(tr("Add to current map"));
			connect(action12, SIGNAL(triggered()), this, SLOT(onAddToCurMap()));
			menu.addAction(action12);

			menu.addSeparator();

			// ����ʹ��
			QAction* action21 = new QAction(tr("Add to new scene"));
			connect(action21, SIGNAL(triggered()), this, SLOT(onAddToNewScene()));
			menu.addAction(action21);
			// ���û�г������ڣ�����Ӵ���
			QAction* action22 = new QAction(tr("Add to current scene"));
			connect(action22, SIGNAL(triggered()), this, SLOT(onAddToCurScene()));
			menu.addAction(action22);

			menu.exec(QCursor::pos());
		}
	}
}


// ���ݼ��˵��¼�
void MainWindow::onOpenMap()
{
	if (pCurWorkspaceViewItem != NULL)
	{
		QString name = pCurWorkspaceViewItem->text(0);

		MapView* pMapView = new MapView();
		pMapView->setAttribute(Qt::WA_DeleteOnClose); // �رպ���ͷ�
		pMapView->getMapControl()->SetWorkspace(pWorkspace);
		QString title(name);

		pMapView->setWindowTitle(title);

		pMdiArea->addSubWindow(pMapView);
		pMapView->show();
		pMapView->getMapControl()->OpenMap(name.toStdString());

		pMapView->getMapLayersView()->updateLayers(pMapView->getMapControl());
	}
}
void MainWindow::onOpenScene()
{
	if (pCurWorkspaceViewItem != NULL)
	{
		QString name = pCurWorkspaceViewItem->text(0);

		SceneView* sceneView = new SceneView();
		sceneView->setAttribute(Qt::WA_DeleteOnClose); // �رպ���ͷ�

		QString title(name);
		sceneView->setWindowTitle(title);
		pMdiArea->addSubWindow(sceneView);
		sceneView->show();

		SceneControl* sceneControl = sceneView->GetSceneControl();

		sceneControl->SetWorkspace(pWorkspace);
		bool isOpen = sceneControl->OpenScene(name.toStdString());
		if (!isOpen)
		{
			//TODO: ouput log
		}

		sceneView->getSceneLayersView()->updateLayers(sceneView->GetSceneControl());
	}
}
void MainWindow::onAddToNewMap()
{
	if (pCurWorkspaceViewItem != NULL)
	{
		QString datasetName = pCurWorkspaceViewItem->text(0);
		QTreeWidgetItem* pDatasourceItem = pCurWorkspaceViewItem->parent();
		QString datasourceName = pDatasourceItem->text(0);


		MapView* pMapView = new MapView();
		pMapView->setAttribute(Qt::WA_DeleteOnClose); // �رպ���ͷ�
		pMapView->getMapControl()->SetWorkspace(pWorkspace);
		// ���ͼ��
		pMapView->getMapControl()->AddDataset(datasourceName.toStdString(), datasetName.toStdString());

		QString title(datasetName + "@" + datasourceName);
		pMapView->setWindowTitle(title);

		pMdiArea->addSubWindow(pMapView);
		pMapView->show();

		pMapView->getMapLayersView()->updateLayers(pMapView->getMapControl());
	}
}
void MainWindow::onAddToCurMap()
{
	if (pCurMapOrSceneWidget != NULL)
	{
		if (typeid(*pCurMapOrSceneWidget) == typeid(MapView))
		{
			MapView* pMapView = (MapView*)pCurMapOrSceneWidget;

			QString datasetName = pCurWorkspaceViewItem->text(0);
			QTreeWidgetItem* pDatasourceItem = pCurWorkspaceViewItem->parent();
			QString datasourceName = pDatasourceItem->text(0);

			// ���ͼ��
			UGLayer* pLayer = pMapView->getMapControl()->AddDataset(datasourceName.toStdString(), datasetName.toStdString());
			if (NULL != pLayer) 
			{
				pMapView->getMapControl()->Refresh();
				pMapView->getMapLayersView()->addLayer(pLayer);
			}
		}
		else
		{

		}
	}

}
void MainWindow::onAddToNewScene()
{
	if (pCurWorkspaceViewItem != NULL)
	{
		QString datasetName = pCurWorkspaceViewItem->text(0);
		QTreeWidgetItem* pDatasourceItem = pCurWorkspaceViewItem->parent();
		QString datasourceName = pDatasourceItem->text(0);


		SceneView* sceneView = new SceneView();
		sceneView->setAttribute(Qt::WA_DeleteOnClose); // �رպ���ͷ�

		QString title(datasetName + "@" + datasourceName);
		sceneView->setWindowTitle(title);

		pMdiArea->addSubWindow(sceneView);
		sceneView->show();

		sceneView->GetSceneControl()->SetWorkspace(pWorkspace);
		// ���ͼ��
		UGLayer3D* pLayer = sceneView->GetSceneControl()->AddLayerFromDataset(datasourceName.toStdString(), datasetName.toStdString());

		sceneView->getSceneLayersView()->updateLayers(sceneView->GetSceneControl());

		sceneView->GetSceneControl()->ViewToLayer(pLayer);
	}
}
void MainWindow::onAddToCurScene()
{
	if (pCurMapOrSceneWidget != NULL)
	{
		if (typeid(*pCurMapOrSceneWidget) == typeid(SceneView))
		{
			SceneView* sceneView = (SceneView*)pCurMapOrSceneWidget;

			QString datasetName = pCurWorkspaceViewItem->text(0);
			QTreeWidgetItem* pDatasourceItem = pCurWorkspaceViewItem->parent();
			QString datasourceName = pDatasourceItem->text(0);

			// ���ͼ��
			UGLayer3D* pLayer = sceneView->GetSceneControl()->AddLayerFromDataset(datasourceName.toStdString(), datasetName.toStdString());
			sceneView->GetSceneControl()->Refresh();

			sceneView->getSceneLayersView()->addLayer(pLayer);
		}
		else
		{

		}
	}
}

void MainWindow::MDI_OnSubWindowActivated()
{
	QMdiSubWindow* subWnd = pMdiArea->activeSubWindow();
	pCurMapOrSceneWidget = NULL;
	if (subWnd != NULL)
	{
		pCurMapOrSceneWidget = subWnd->widget();
		if (typeid(*pCurMapOrSceneWidget) == typeid(MapView))
		{
			this->ui->dockWidget_layerlist->setWidget(((MapView*)pCurMapOrSceneWidget)->getMapLayersView());
		}
		else if (typeid(*pCurMapOrSceneWidget) == typeid(SceneView))
		{
			this->ui->dockWidget_layerlist->setWidget(((SceneView*)pCurMapOrSceneWidget)->getSceneLayersView());
		}
	}
	else {
	     this->ui->dockWidget_layerlist->setWidget(NULL);
	}
}

void MainWindow::onDoubleClickedWorkspaceViewItem(QTreeWidgetItem* pItem)
{
	pCurWorkspaceViewItem = pItem;
	WorkspaceView::ItemType type = (WorkspaceView::ItemType)pItem->data(0, pWorkspaceView->ItemDataType).toInt();
	if (type == WorkspaceView::TypeMap)
	{
		onOpenMap();
	}
	if (type == WorkspaceView::TypeScene)
	{
		onOpenScene();
	}
}


void MainWindow::Tool_Pan()
{
    if (pCurMapOrSceneWidget != NULL)
    {
        if (typeid(*pCurMapOrSceneWidget) == typeid(MapView))
        {
            MapView* pMapView = (MapView*)pCurMapOrSceneWidget;

            pMapView->getMapControl()->GetMapEditWnd()->SetUserAction(UGC::UGDrawParamaters::uaPan);
        }
        else if (typeid(*pCurMapOrSceneWidget) == typeid(SceneView))
        {
            SceneView* sceneView = (SceneView*)pCurMapOrSceneWidget;
            sceneView->GetSceneControl()->GetSceneEditWnd()->SetUserAction(UGC::/*UGSceneUserAction::*/suaPan);
        }
    }
}

void MainWindow::Tool_Select()
{
    if (pCurMapOrSceneWidget != NULL)
    {
        if (typeid(*pCurMapOrSceneWidget) == typeid(MapView))
        {
            MapView* pMapView = (MapView*)pCurMapOrSceneWidget;
            pMapView->getMapControl()->GetMapEditWnd()->SetUserAction(UGC::UGDrawParamaters::uaPointModeSelect);
        }
        else if (typeid(*pCurMapOrSceneWidget) == typeid(SceneView))
        {
            SceneView* sceneView = (SceneView*)pCurMapOrSceneWidget;
            sceneView->GetSceneControl()->GetSceneEditWnd()->SetUserAction(UGC::/*UGSceneUserAction::*/suaPointSelect);
        }
    }
}

void MainWindow::Menu_Scene_GetSelections()
{
	
	/*	UGQueryDef queryDef;
		UGint size = queryDef.m_IDs.GetSize();
		queryDef.m_IDs.Add(1);
		size = queryDef.m_IDs.GetSize();
		if (size == 1) {
			cout << size << endl;
		}*/

    if (pCurMapOrSceneWidget != NULL)
    {
		if (typeid(*pCurMapOrSceneWidget) == typeid(MapView))
		{
			MapView* pMapView = (MapView*)pCurMapOrSceneWidget;
			UGArray<UGSelection*>* pSelectionArr = pMapView->getMapControl()->GetGeoSelections();
			if (pSelectionArr != NULL)
			{
				int count = pSelectionArr->GetSize();
				for (int i = 0; i < count; i++)
				{
					UGRecordset *pRecordset = pMapView->getMapControl()->ToRecordset(pSelectionArr->GetAt(i));
					if (pRecordset != NULL)
					{
						int rsize = pRecordset->GetRecordCount();
						qDebug() << "Record count: " << rsize << endl;
						pRecordset->GetDataset()->ReleaseRecordset(pRecordset);
						pRecordset = NULL;
					}
				}

				pSelectionArr->RemoveAll();
				delete pSelectionArr;
			}
		}
        else if (typeid(*pCurMapOrSceneWidget) == typeid(SceneView))
        {
            SceneView* sceneView = (SceneView*)pCurMapOrSceneWidget;
			UGArray<UGSelection3D*>* pSelectionArr = sceneView->GetSceneControl()->GetGeoSelections();
			if (pSelectionArr != NULL)
			{
				int count = pSelectionArr->GetSize();
				for (int i = 0; i < count; i++)
				{
					UGRecordset *pRecordset = sceneView->GetSceneControl()->ToRecordset(pSelectionArr->GetAt(i));
					if (pRecordset != NULL)
					{
						int rsize = pRecordset->GetRecordCount();
						qDebug() << "Record count: " << rsize << endl;
						pRecordset->GetDataset()->ReleaseRecordset(pRecordset);
						pRecordset = NULL;
					}
				}

				pSelectionArr->RemoveAll();
				delete pSelectionArr;
			}
        }
    }

}


void MainWindow::Menu_Scene_Add_UniqueTheme3D()
{
	// ���������һ��ʾ����ʹ��ǰ��ȷ����ǰ�����ĵ�һ��ͼ����ģ�����ݼ�ͼ��

	SceneView* sceneView = NULL;
	if (typeid(*pCurMapOrSceneWidget) == typeid(SceneView))
	{
		sceneView = (SceneView*)pCurMapOrSceneWidget;
		if (sceneView->GetSceneControl()->GetUGLayers()->GetInnerCount() <= 0) {
			return;
		}
		UGLayer3D* pLayer = sceneView->GetSceneControl()->GetUGLayers()->GetLayerInnerAt(0);
		
		bool isTheme = pLayer != NULL && (UGLayer3DType::l3dDatasetModelPro == pLayer->GetType()) ? true : false;
		
		if (isTheme)
		{
			// ���ר��ͼ
			UGTheme3DItem item0;
			item0.m_bVisible = true;
			item0.m_strCaption = _U("item0");

			UGColorValue3D color3D(1.0f, 1.0f, 1.0f, 1.0f);
			item0.m_Style.SetFillColor(color3D);
			item0.m_Style.SetPointColor(color3D);
			//item0.m_Style.SetFill3DMode(UGC::/*UGFill3DMode::*/FILL_FACE);

			UGTheme3DItem item1;
			item1.m_bVisible = true;
			item1.m_strCaption = _U("item1");

			UGColorValue3D color3D1(1.0f, 0.0f, 0.0f, 1.0f);
			item1.m_Style.SetFillColor(color3D1);
			item1.m_Style.SetPointColor(color3D1);
			//item1.m_Style.SetFill3DMode(UGC::/*UGFill3DMode::*/FILL_FACE);

			UGTheme3DItem item2;
			item2.m_bVisible = true;
			item2.m_strCaption = _U("item2");

			UGColorValue3D color3D2(0.0f, 1.0f, 0.0f, 1.0f);
			item2.m_Style.SetFillColor(color3D2);
			item2.m_Style.SetPointColor(color3D2);
			//item2.m_Style.SetFill3DMode(UGC::/*UGFill3DMode::*/FILL_FACE);

			UGTheme3DItem item3;
			item3.m_bVisible = true;
			item3.m_strCaption = _U("item3");

			UGColorValue3D color3D3(0.0f, 0.0f, 1.0f, 1.0f);
			item3.m_Style.SetFillColor(color3D3);
			item3.m_Style.SetPointColor(color3D3);
			//item3.m_Style.SetFill3DMode(UGC::/*UGFill3DMode::*/FILL_FACE);


			UGTheme3DUnique* pTheme3D = new UGTheme3DUnique;
			pTheme3D->Add(_U("0"), item1);
			pTheme3D->Add(_U("1"), item2);
			pTheme3D->Add(_U("2"), item3);

			pTheme3D->SetDefaultStyle(item0.m_Style);
			pTheme3D->SetExpression(_U("SmUserID"));

			UGLayer3Ds* pLayers = sceneView->GetSceneControl()->GetUGLayers();
			UGLayer3D* pLayer1 = NULL;
			pLayer1 = pLayer;   // ֱ�ӽ���õ�ͼ������Ϊר��ͼ
			//pLayer1 =  pLayers->AddLayer(pLayer->GetName()); // �����ͼ�㣬������ר��ͼ���˴�����õ�ͼ���ٴ���ӵ���ͼ��Ȼ������ר��ͼ

			pLayer1->m_pTheme3D = pTheme3D;

			// ������ά��񣬲�����ģ�����ݼ�����ʾ��ͼ
			UGStyle3D style3D;
			style3D.SetFill3DMode(UGC::/*UGFill3DMode::*/FILL_FACE);

			pLayer1->SetStyle3D(style3D);

			sceneView->GetSceneControl()->Refresh();

			if(pLayer1 != pLayer)
			    sceneView->getSceneLayersView()->addLayer(pLayer);
		}

		
	}
	else
	{

	}
}