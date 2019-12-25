#ifndef MAPLAYERSVIEW_H
#define MAPLAYERSVIEW_H

#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "Mapping/MapControl.h"

#include "Map/UGLayerGroup.h"

#include "Icons.h"

using namespace SuperMap;

class MapLayersView : public QTreeWidget
{
    Q_OBJECT

public:
	enum ItemType {
		TypeDynamicLayers,
		TypeDynamicLayer,

		TypeDatasetLayers,
		TypeLayerGroup,
		TypeLayer,
		TypeLayerPoint,
		TypeLayerLine,
		TypeLayerRegion,
		TypeLayerText,
		TypeLayerCAD,
		TypeLayerNetworkNode,
		TypeLayerNetworkLine,
		TypeLayerGrid,
		TypeLayerDEM,

		TypeLayerPoint3D,
		TypeLayerLine3D,
		TypeLayerRegion3D,

		TypeTheme,
	};
	int ItemDataType; // QTreeWidgetItem存放数据类型，自定义类型

public:
    explicit MapLayersView(QWidget *parent = nullptr);

private:
	QTreeWidgetItem* pTreeDynamicLayers; // 动态层组，动态层是临时图层，只用于显示
	QTreeWidgetItem* pTreeNormalLayers;  // 地图数据图层组，关联数据集显示，可保存到工作空间

	const int typeIconCol = 0;

	// 右键菜单
	QMenu* menuDynamicLayer;
	QMenu* menuLayer;

private:
	void addDefaultTree();
	void clearItems(QTreeWidgetItem* pItem);
	void updateLayer(UGLayer* pLayer, QTreeWidgetItem* parent, bool isTop = false);

public:
	void updateLayers(MapControl* mapControl);
	void addLayer(UGLayer* pLayer, bool isTop = true);
	void clear();

signals:

public slots:
};

#endif // MAPLAYERSVIEW_H