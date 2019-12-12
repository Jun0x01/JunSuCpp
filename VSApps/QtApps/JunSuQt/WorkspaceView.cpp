#include "WorkspaceView.h"
#include <QApplication>

WorkspaceView::WorkspaceView(QWidget *parent) : QTreeWidget(parent)
{

//    this->headerItem()->setText(0, tr("No Workspace"));

    pListWorkspaceItems = new QList<QTreeWidgetItem>();


    // 添加默认的工作空间列表
    addTreeWorkspaceItem();

    this->setHeaderHidden(true); // 隐藏Header
    this->expandAll();           // 展开全部
//    this->expandItem();        // 展开指定item

}

void WorkspaceView::addTreeWorkspaceItem(){
    // 初始化
    pItemWorkspace = new QTreeWidgetItem(this);
    pTreeDatasources = new QTreeWidgetItem(pItemWorkspace);
    pTreeMaps = new QTreeWidgetItem(pItemWorkspace);
    pTreeScenes = new QTreeWidgetItem(pItemWorkspace);

    // 设置标题
    pItemWorkspace->setText(0, "Empty Workspace");
    pTreeDatasources->setText(0, tr("DataSources"));
    pTreeMaps->setText(0, tr("Maps"));
    pTreeScenes->setText(0, tr("Scense"));

    // 设置图标
    QIcon iconWk(":/Images/Resources/Workspace/smwu.png");
    pItemWorkspace->setIcon(0, iconWk);

    QIcon iconDs(":/Images/Resources/Workspace/Datasources.png");
    pTreeDatasources->setIcon(0, iconDs);

    QIcon iconMaps(":/Images/Resources/Workspace/Maps.png");
    pTreeMaps->setIcon(0, iconMaps);

    QIcon iconScenes(":/Images/Resources/Workspace/Scenes.png");
    pTreeScenes->setIcon(0, iconScenes);
}
