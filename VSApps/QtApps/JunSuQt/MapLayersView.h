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
	int ItemDataType; // QTreeWidgetItem����������ͣ��Զ�������

public:
    explicit MapLayersView(QWidget *parent = nullptr);

private:
	QTreeWidgetItem* pTreeDynamicLayers; // ��̬���飬��̬������ʱͼ�㣬ֻ������ʾ
	QTreeWidgetItem* pTreeNormalLayers;  // ��ͼ����ͼ���飬�������ݼ���ʾ���ɱ��浽�����ռ�

	const int typeIconCol = 0;

	// �Ҽ��˵�
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