#ifndef SCENELAYERSVIEW_H
#define SCENELAYERSVIEW_H

#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "Mapping/SceneControl.h"

#include "Scene/UGLayer3DGroup.h"
#include "Layer3DDataset/UGLayer3DDatasetImage.h"
#include "Layer3DDataset/UGLayer3DDatasetModelPro.h"
#include "Layer3DDataset/UGLayer3DDatasetVector.h"
#include "Layer3DDataset/UGLayer3DDatasetVectorLR.h"
#include "Layer3DDataset/UGLayer3DDatasetVectorPoint.h"
#include "Layer3DDataset/UGLayer3DDatasetVolume.h"

#include "Icons.h"

class SceneLayersView : public QTreeWidget
{
    Q_OBJECT

public:
	enum ItemType {
		TypeTerrainLayers,
		TypeLayerTerrain,

		TypeDatasetLayers,
		TypeLayerGroup,
		TypeLayer,
		TypeLayerPoint,
		TypeLayerLine,
		TypeLayerRegion,
		TypeLayerText,
		TypeLayerCAD,
		TypeLayerNetwork,
		TypeLayerGrid,
		TypeLayerDEM,

		TypeLayerPoint3D,
		TypeLayerLine3D,
		TypeLayerRegion3D,
		TypeLayerModel,

		TypeLayerOSGB,
		TypeLayerImage,
		TypeLayerOverlay,
		TypeLayerTin,
	};
	int ItemDataType; // QTreeWidgetItem����������ͣ��Զ�������

public:
    explicit SceneLayersView(QWidget *parent = nullptr);

private:
	QTreeWidgetItem* pTreeNormalLayers;  // ��ͨ���飬�������ݼ�����ά�������ݣ��ɱ��浽�����ռ�
	QTreeWidgetItem* pTreeTerrianLayers; // ����ͼ���飬�ɱ��浽�����ռ�

	const int typeIconCol = 0;

private:
	void addDefaultTree();
	void clearItems(QTreeWidgetItem* pItem);
	void updateLayer(UGLayer3D* pLayer, QTreeWidgetItem* parent, bool isTop = false);

	ItemType updateItemByDataset(UGDataset* pDataset, QTreeWidgetItem* pItem);
public:
	void updateLayers(SceneControl* sceneControl);
	void addLayer(UGLayer3D* pLayer, bool isTop = true);
	void clear();

signals:

public slots:
};

#endif // SCENELAYERSVIEW_H