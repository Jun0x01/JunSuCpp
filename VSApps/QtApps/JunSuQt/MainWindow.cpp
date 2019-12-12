#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "WorkspaceView.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    statusBar()->hide(); // 隐藏状态栏

    // 添加WorkspaceView到工作空间dock中
    WorkspaceView* pWorkspaceView = new WorkspaceView();
    ui->dockWidget_workspace->setWidget(pWorkspaceView);
}

MainWindow::~MainWindow()
{
    delete ui;
}
