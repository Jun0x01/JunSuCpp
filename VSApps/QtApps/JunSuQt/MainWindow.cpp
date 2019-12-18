#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    statusBar()->hide(); // 隐藏状态栏

    // 添加WorkspaceView到工作空间的dock窗口中
    pWorkspaceView = new WorkspaceView();
    ui->dockWidget_workspace->setWidget(pWorkspaceView);

	// 设置工作空间列表的右键菜单信号
	connect(pWorkspaceView, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(onCustomContextMenu(const QPoint&)));

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
}

MainWindow::~MainWindow()
{
    delete ui;

	CloseWorkspace();
	delete pWorkspace;
}

void MainWindow::CloseWorkspace()
{
    // First, close all associated maps

    // Second, close workspace
    pWorkspace->Close();
}

void MainWindow::Menu_File_New()
{
	qDebug("No action");
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

}
void MainWindow::onOpenScene()
{

}
void MainWindow::onAddToNewMap()
{

}
void MainWindow::onAddToCurMap()
{

}
void MainWindow::onAddToNewScene()
{

}
void MainWindow::onAddToCurScene()
{

}