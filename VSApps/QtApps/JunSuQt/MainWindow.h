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

	// �����ռ���ͼ
    WorkspaceView* pWorkspaceView;
	// �����ռ䣬��������
    Workspace* pWorkspace;

// functions
private:
    void CloseWorkspace();


private slots:
	void Menu_File_New();
    void Menu_File_Open();
    void Menu_File_Save();
    void Menu_File_SaveAs();
    void Menu_File_Exit();
};

#endif // MAINWINDOW_H
