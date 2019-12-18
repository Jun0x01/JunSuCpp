#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include "WorkspaceView.h"

using namespace SuperMap;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

	// 工作空间视图
    WorkspaceView* pWorkspaceView;
	// 工作空间，管理数据
    Workspace* pWorkspace;

// functions
private:
    void CloseWorkspace();


private slots:

	void onCustomContextMenu(const QPoint& pos); // Rigt click event

	// 文件菜单事件
	void Menu_File_New();
    void Menu_File_Open();
    void Menu_File_Save();
    void Menu_File_SaveAs();
    void Menu_File_Close();
    void Menu_File_Exit();

	// 数据集菜单事件
	void onOpenMap();
	void onOpenScene();
	void onAddToNewMap();
	void onAddToCurMap();
	void onAddToNewScene();
	void onAddToCurScene();
};

#endif // MAINWINDOW_H
