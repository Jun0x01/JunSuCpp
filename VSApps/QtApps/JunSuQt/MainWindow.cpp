#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    statusBar()->hide(); // 隐藏状态栏
	// 设置鼠标跟随，使编辑对象时效果更好
	this->setMouseTracking(true);
	this->centralWidget()->setMouseTracking(true);

    // 添加WorkspaceView到工作空间的dock窗口中
    pWorkspaceView = new WorkspaceView();
    ui->dockWidget_workspace->setWidget(pWorkspaceView);

	// 设置工作空间列表的右键菜单信号
	connect(pWorkspaceView, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(onCustomContextMenu(const QPoint&)));
	// 双击信号
	connect(pWorkspaceView, SIGNAL(doubleClickedItem(QTreeWidgetItem*)), this, SLOT(onDoubleClickedWorkspaceViewItem(QTreeWidgetItem*)));

    /***************** 设置信号 ***********************/
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

	/***************** 结束设置信号 ***********************/

    pWorkspace = new Workspace();

	/****************** 增加MDI ***************************/
	// MDI
	pMdiArea = new QMdiArea();
	pMdiArea->setViewMode(QMdiArea::TabbedView);
	pMdiArea->setTabsClosable(true);
    pMdiArea->setMouseTracking(true);   // 设置鼠标跟随，使编辑对象时效果更好
	pMdiArea->setMinimumSize(300, 200);
	setCentralWidget(pMdiArea);
	connect(pMdiArea, &QMdiArea::subWindowActivated, this, &MainWindow::MDI_OnSubWindowActivated);

	pCurWorkspaceViewItem = NULL;
	pCurMapOrSceneWidget = NULL;
   /****************************************************/
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
	pMapView->setAttribute(Qt::WA_DeleteOnClose); // 关闭后就释放
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

		// 工作空间
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
				// 更新工作空间列表
				pWorkspaceView->updateWorkspaceList(*pWorkspace);

			}
			else
			{
				// 更新工作空间列表
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
				// 更新工作空间列表，增加一个数据源
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

			// 地图使用
			QAction* action11 = new QAction(tr("Add to new map"));
			connect(action11, SIGNAL(triggered()), this, SLOT(onAddToNewMap()));
			menu.addAction(action11);
			// 如果没有地图窗口，则不添加此项
			QAction* action12 = new QAction(tr("Add to current map"));
			connect(action12, SIGNAL(triggered()), this, SLOT(onAddToCurMap()));
			menu.addAction(action12);

			menu.addSeparator();

			// 场景使用
			QAction* action21 = new QAction(tr("Add to new scene"));
			connect(action21, SIGNAL(triggered()), this, SLOT(onAddToNewScene()));
			menu.addAction(action21);
			// 如果没有场景窗口，则不添加此项
			QAction* action22 = new QAction(tr("Add to current scene"));
			connect(action22, SIGNAL(triggered()), this, SLOT(onAddToCurScene()));
			menu.addAction(action22);

			menu.exec(QCursor::pos());
		}
	}
}


// 数据集菜单事件
void MainWindow::onOpenMap()
{
	if (pCurWorkspaceViewItem != NULL)
	{
		QString name = pCurWorkspaceViewItem->text(0);

		MapView* pMapView = new MapView();
		pMapView->setAttribute(Qt::WA_DeleteOnClose); // 关闭后就释放
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
		sceneView->setAttribute(Qt::WA_DeleteOnClose); // 关闭后就释放

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
		pMapView->setAttribute(Qt::WA_DeleteOnClose); // 关闭后就释放
		pMapView->getMapControl()->SetWorkspace(pWorkspace);
		// 添加图层
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

			// 添加图层
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
		sceneView->setAttribute(Qt::WA_DeleteOnClose); // 关闭后就释放

		QString title(datasetName + "@" + datasourceName);
		sceneView->setWindowTitle(title);

		pMdiArea->addSubWindow(sceneView);
		sceneView->show();

		sceneView->GetSceneControl()->SetWorkspace(pWorkspace);
		// 添加图层
		sceneView->GetSceneControl()->AddLayerFromDataset(datasourceName.toStdString(), datasetName.toStdString());
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

			// 添加图层
			sceneView->GetSceneControl()->AddLayerFromDataset(datasourceName.toStdString(), datasetName.toStdString());
			sceneView->GetSceneControl()->Refresh();
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
	}
	else {
		//this->ui->dockWidget_layerlist->setWidget(NULL);
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