#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMdiArea>
#include <QDebug>
#include <QMdiSubWindow>

#include "WorkspaceView.h"
#include "MapView.h"

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

	
	QTreeWidgetItem* pCurWorkspaceViewItem;  // �����ռ��б���ѡ�е�item

	QMdiArea* pMdiArea;             // MDI��ͼ
	QWidget* pCurMapOrSceneWidget;  // MDI�У���ǰ����Ĵ��ڣ������ǵ�ͼ��Ҳ�����ǳ���

// functions
private:
    void CloseWorkspace();


private slots:

	void onCustomContextMenu(const QPoint& pos); // Rigt click event
	void MDI_OnSubWindowActivated(); // MDI���ڼ����¼�

	// �ļ��˵��¼�
	void Menu_File_New();
    void Menu_File_Open();
    void Menu_File_Save();
    void Menu_File_SaveAs();
    void Menu_File_Close();
    void Menu_File_Exit();

	// ���ݼ��˵��¼�
	void onOpenMap();
	void onOpenScene();
	void onAddToNewMap();
	void onAddToCurMap();
	void onAddToNewScene();
	void onAddToCurScene();
};

#endif // MAINWINDOW_H
