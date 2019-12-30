#include "MapLayersView.h"

MapLayersView::MapLayersView(QWidget *parent) : QTreeWidget(parent)
{

	addDefaultTree();

	this->setHeaderLabel(tr("Empty Map"));
	this->expandAll();           // 展开全部

	ItemDataType = Qt::UserRole + 1;

	// 允许右键菜单
	setContextMenuPolicy(Qt::CustomContextMenu);

}

void MapLayersView::addDefaultTree()
{
	// 初始化
	pTreeDynamicLayers = new QTreeWidgetItem(this);
	pTreeNormalLayers = new QTreeWidgetItem(this);

	// 设置标题
	pTreeDynamicLayers->setText(0, tr("Dynamic Layers"));
	pTreeNormalLayers->setText(0, tr("Normal Layers"));

	// 设置图标
	pTreeDynamicLayers->setIcon(0, Icons::getInstance().iconLayerGroup);
	pTreeNormalLayers->setIcon(0, Icons::getInstance().iconMaps);


	// 设置类型
	pTreeDynamicLayers->setData(0, ItemDataType, QVariant::fromValue((int)TypeDynamicLayers));
	pTreeNormalLayers->setData(0, ItemDataType, QVariant::fromValue((int)TypeDatasetLayers));

}
void MapLayersView::clear()
{
	clearItems(pTreeDynamicLayers);
	clearItems(pTreeNormalLayers);
}

void MapLayersView::clearItems(QTreeWidgetItem* pItem)
{
	if (NULL != pItem)
	{
		int count = pItem->childCount();
		for (int i = 0; i < count; i++)
		{
			QTreeWidgetItem* item = pItem->child(0);
			pItem->removeChild(item);
			delete item;
		}
	}
}

void MapLayersView::updateLayers(MapControl* mapControl)
{
	clearItems(pTreeDynamicLayers);
	clearItems(pTreeNormalLayers);

	if (NULL == mapControl)
	{
		return;
	}

	UGString ugMapName = mapControl->GetMapEditWnd()->m_mapWnd.m_Map.GetName();
	std::string mapName = UGStrConvertor::Tostring(ugMapName);
	this->setHeaderLabel(QString().fromStdString(mapName));

	UGLayers* pLayers = mapControl->GetUGLayers();
	int count = pLayers->GetTopLevelCount();
	for (int i = 0; i < count; i++) 
	{
		UGLayer* pLayer = pLayers->GetLayerAt(i);
		updateLayer(pLayer, pTreeNormalLayers);
	}
}

void MapLayersView::updateLayer(UGLayer* pLayer, QTreeWidgetItem* parent, bool isTop /*= false*/)
{
	if (typeid(*pLayer) == typeid(UGLayerGroup))
	{
		UGString ugLayerName = pLayer->GetName();
		std::string layerName = UGStrConvertor::Tostring(ugLayerName);
		
		QTreeWidgetItem* pItem = new QTreeWidgetItem();
		if (isTop) {
			parent->insertChild(0, pItem);
		}
		else
		{
			parent->addChild(pItem);
		}
		pItem->setText(0, QString().fromStdString(layerName));
		pItem->setIcon(0, Icons::getInstance().iconDs);

		int count = ((UGLayerGroup*)pLayer)->GetCount();
		for (int i = 0; i < count; i++)
		{
			UGLayer* pLayer = ((UGLayerGroup*)pLayer)->GetAt(i);
			updateLayer(pLayer, pItem);
		}
	}
	else // Layer
	{
		UGString ugLayerName = pLayer->GetCaption(); // GetName();
		UGLayer::UGLayerType type = pLayer->GetType();
		
		UGDataset* pDataset = pLayer->GetDataset();


		std::string layerName = UGStrConvertor::Tostring(ugLayerName);

		QTreeWidgetItem* pItem = new QTreeWidgetItem();
		if (isTop) {
			parent->insertChild(0, pItem);
		}
		else
		{
			parent->addChild(pItem);
		}
		pItem->setText(typeIconCol, QString().fromStdString(layerName));
		pItem->setIcon(typeIconCol, Icons::getInstance().iconDataTypeUnknown);

		UGDataset::DatasetType datasetType = pDataset->GetType();
		ItemType layerItemType = TypeLayer;

		
		if (type == UGLayer::UG_Layer_Normal)
		{
			switch (datasetType)
			{
			case UGDataset::/*DatasetType::*/Point:
				layerItemType = TypeLayerPoint;
				pItem->setIcon(typeIconCol, Icons::getInstance().iconDatasetPoint);
				break;
			case UGDataset::Line:
				layerItemType = TypeLayerLine;
				pItem->setIcon(typeIconCol, Icons::getInstance().iconDatasetLine);
				break;
			case UGDataset::Region:
				layerItemType = TypeLayerRegion;
				pItem->setIcon(typeIconCol, Icons::getInstance().iconDatasetRegion);
				break;
			case UGDataset::CAD:
				layerItemType = TypeLayerCAD;
				pItem->setIcon(typeIconCol, Icons::getInstance().iconDatasetCAD);
				break;
			case UGDataset::Text:
				layerItemType = TypeLayerText;
				pItem->setIcon(typeIconCol, Icons::getInstance().iconDatasetText);
				break;
			case UGDataset::Grid:
				layerItemType = TypeLayerGrid;
				pItem->setIcon(typeIconCol, Icons::getInstance().iconDatasetGrid);
				break;
			case UGDataset::DEM:
				layerItemType = TypeLayerDEM;
				pItem->setIcon(typeIconCol, Icons::getInstance().iconDatasetDEM);
				break;

			case UGDataset::PointZ:
				layerItemType = TypeLayerPoint3D;
				pItem->setIcon(typeIconCol, Icons::getInstance().iconDatasetPoint3D);
				break;
			case UGDataset::LineZ:
				layerItemType = TypeLayerLine3D;
				pItem->setIcon(typeIconCol, Icons::getInstance().iconDatasetLine3D);
				break;
			case UGDataset::RegionZ:
				layerItemType = TypeLayerRegion3D;
				pItem->setIcon(typeIconCol, Icons::getInstance().iconDatasetRegion3D);
				break;
			case UGDataset::Model:
				layerItemType = TypeLayerModel;
				pItem->setIcon(typeIconCol, Icons::getInstance().iconDatasetModel);
				break;
			default:
				pItem->setIcon(typeIconCol, Icons::getInstance().iconDataTypeUnknown);
				break;
			}
		}
		else if (type == UGLayer::UG_Layer_NetworkPoint)
		{
			layerItemType = TypeLayerNetworkNode;
			pItem->setIcon(typeIconCol, Icons::getInstance().iconDatasetNetwork);
		}
		else if (type == UGLayer::UG_Layer_NetworkLine)
		{
			layerItemType = TypeLayerNetworkLine;
			pItem->setIcon(typeIconCol, Icons::getInstance().iconDatasetNetwork);
		}
		pItem->setData(0, ItemDataType, QVariant::fromValue((int)layerItemType)); // 设置数据集类型

	}
}

void MapLayersView::addLayer(UGLayer* pLayer, bool isTop /*= true*/)
{
	if (NULL == pLayer)
	{
		return;
	}
	else
	{
		updateLayer(pLayer, pTreeNormalLayers, true);
	}
}