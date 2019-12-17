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

void MainWindow::Menu_File_Exit()
{
	close();
}

