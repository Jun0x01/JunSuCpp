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

    // 设置信号
    connect(ui->actionOpen, SIGNAL(triggered()), SLOT(Menu_File_Open()));

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



void MainWindow::Menu_File_Open()
{
    QString qStrPath = QFileDialog::getOpenFileName(this, QString::fromUtf8("OpenWorksapce"), "../../../TestData/", tr("smwu(*.smwu)"));

    if (!qStrPath.isEmpty())
    {
        string strPath = qStrPath.toStdString();
		// Close first
		CloseWorkspace();

		if (pWorkspace->OpenWorkspaceFile(strPath))
		{
			pWorkspaceView->updateWorkspaceList(*pWorkspace);
		}
		else
		{
			qDebug("打开工作空间失败");
		}
    }
    else
    {

    }
}

void MainWindow::Menu_File_Exit()
{

}

