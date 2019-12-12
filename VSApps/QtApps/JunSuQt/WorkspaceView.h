#ifndef WORKSPACEVIEW_H
#define WORKSPACEVIEW_H

#include <QWidget>

#include <QTreeWidget>
#include <QTreeWidgetItem>

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

// private functions
private:
    void addTreeWorkspaceItem();
signals:

public slots:
};

#endif // WORKSPACEVIEW_H
