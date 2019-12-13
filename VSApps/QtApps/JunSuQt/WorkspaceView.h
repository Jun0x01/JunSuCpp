#ifndef WORKSPACEVIEW_H
#define WORKSPACEVIEW_H

#include <QWidget>

#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "Data/Workspace.h"
#include "Data/UGStrConvertor.h"

#include "Icons.h"

using namespace SuperMap;

class WorkspaceView : public QTreeWidget
{
    Q_OBJECT
public:
    explicit WorkspaceView(QWidget *parent = nullptr);

// private properties
private:
    QList<QTreeWidgetItem>* pListWorkspaceItems;
    QTreeWidgetItem* pItemWorkspace;
    QTreeWidgetItem* pTreeDatasources;
    QTreeWidgetItem* pTreeMaps;
    QTreeWidgetItem* pTreeScenes;

    QIcon iconWk;
    QIcon iconDSes;
    QIcon iconMaps;
    QIcon iconScenes;


// private functions
private:
    void addTreeWorkspaceItem();
    void clearItems(QTreeWidgetItem* pItem);

public:
    void updateWorkspaceList(Workspace& workspace);



signals:

public slots:
};

#endif // WORKSPACEVIEW_H
