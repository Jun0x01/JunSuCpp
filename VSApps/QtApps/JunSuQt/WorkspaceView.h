#ifndef WORKSPACEVIEW_H
#define WORKSPACEVIEW_H

#include <QWidget>

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMenu>

#include "Data/Workspace.h"
#include "Data/UGStrConvertor.h"

#include "Icons.h"

using namespace SuperMap;

class WorkspaceView : public QTreeWidget
{
    Q_OBJECT

	enum ItemType {
		TypeWorkspace,
		TypeDataosources,
		TypeMaps,
		TypeScenes,
		TypeMap,
		TypeScene,

		// 数据源
		TypeDatasource,
		TypeDatasourceUDB,
		TypeDatasourceUDBX,  // UGC::Spatialite = 2054
		TypeDatasourceIMG,

		// 数据集
		TypeDataset,
		TypeDatasetPoint,
		TypeDatasetLine,
		TypeDatasetRegion,
		TypeDatasetCAD,
		TypeDatasetText,

		TypeDatasetTabular, // 属性表
		TypeDatasetNetwork,
		TypeDatasetGrid,
		TypeDatasetDEM,

		TypeDatasetPoint3D,
		TypeDatasetLine3D,
		TypeDatasetRegion3D,
		TypeDatasetModel,
		TypeDatasetNetwork3D,
	};

public:
    explicit WorkspaceView(QWidget *parent = nullptr);

// private properties
private:
    QList<QTreeWidgetItem>* pListWorkspaceItems;
    QTreeWidgetItem* pItemWorkspace;
    QTreeWidgetItem* pTreeDatasources;
    QTreeWidgetItem* pTreeMaps;
    QTreeWidgetItem* pTreeScenes;

	int ItemDataType;  // QTreeWidgetItem存放数据类型，自定义类型

// private functions
private:
    void addTreeWorkspaceItem();
    void clearItems(QTreeWidgetItem* pItem);

public:
    void updateWorkspaceList(Workspace& workspace);
	void updateNewDatasource(UGDataSource& ugDatasource);



signals:

public slots:
};

#endif // WORKSPACEVIEW_H
