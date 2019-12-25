#include "SceneLayersView.h"

SceneLayersView::SceneLayersView(QWidget *parent) : QTreeWidget(parent)
{
	addDefaultTree();

	this->setHeaderLabel(tr("Empty Map"));
	this->expandAll();           // 展开全部

	ItemDataType = Qt::UserRole + 1;

	// 允许右键菜单
	setContextMenuPolicy(Qt::CustomContextMenu);
}

void SceneLayersView::addDefaultTree()
{
	// 初始化
	pTreeNormalLayers = new QTreeWidgetItem(this);
	pTreeTerrianLayers = new QTreeWidgetItem(this);

	// 设置标题
    pTreeTerrianLayers->setText(0, tr("Terrian Layers"));
	pTreeNormalLayers->setText(0, tr("Normal Layers"));

	// 设置图标
    pTreeTerrianLayers->setIcon(0, Icons::getInstance().iconLayerGroup);
	pTreeNormalLayers->setIcon(0, Icons::getInstance().iconMaps);

	// 设置类型
    pTreeTerrianLayers->setData(0, ItemDataType, QVariant::fromValue((int)TypeTerrainLayers));
	pTreeNormalLayers->setData(0, ItemDataType, QVariant::fromValue((int)TypeDatasetLayers));

}
void SceneLayersView::clear()
{
    clearItems(pTreeTerrianLayers);
	clearItems(pTreeNormalLayers);
}

void SceneLayersView::clearItems(QTreeWidgetItem* pItem)
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

void SceneLayersView::updateLayers(SceneControl* sceneControl)
{
    clearItems(pTreeTerrianLayers);
	clearItems(pTreeNormalLayers);

	if (NULL == sceneControl)
	{
		return;
	}

	UGString ugMapName = sceneControl->GetSceneEditWnd()->GetScene3D()->GetName();
	std::string mapName = UGStrConvertor::Tostring(ugMapName);
	this->setHeaderLabel(QString().fromStdString(mapName));

	UGLayer3Ds* pLayers = sceneControl->GetUGLayers();

	UGLayer3DCollection* pLayersCollection = pLayers->GetLayer3DsTree();
	UGLayer3DPos pos = pLayersCollection->GetHeadPosition();
	while (!pLayersCollection->IsEOF(pos))
	{
		UGLayer3D* pLayer3D = pLayersCollection->GetNext(pos);

		updateLayer(pLayer3D, pTreeNormalLayers);
	}
	//int count = pLayers->GetInnerCount();
	//for (int i = 0; i < count; i++)
	//{
	//	UGLayer* pLayer = pLayers->GetLayerAt(i);
	//	updateLayer(pLayer, pTreeNormalLayers);
	//}

	UGTerrainLayer3Ds& pTerrains = sceneControl->GetSceneEditWnd()->GetScene3D()->m_TerrainLayer3Ds;
	
	UGTerrainManager* pTerrainManager = pTerrains.GetTerrainManager();
	int count = pTerrainManager->m_TerrainAccessors.GetCount();

	UGList<UGTerrainAccessor*>::POSITION pos1;
	UGTerrainAccessor* pTerrain = NULL;
	for (int i = 0; i < count; i++)
	{
		pos1 = pTerrainManager->m_TerrainAccessors.FindIndex(i);
		pTerrain = pTerrainManager->m_TerrainAccessors.GetAt(pos1);

		UGString ugTerrainName = pTerrain->GetName();
		std::string terrainName = UGStrConvertor::Tostring(ugTerrainName);
		QTreeWidgetItem* pItem = new QTreeWidgetItem();
		
		pItem->setText(typeIconCol, QString().fromStdString(terrainName));
		pItem->setIcon(typeIconCol, Icons::getInstance().iconLayer3DTerrian);
		pItem->setData(0, ItemDataType, QVariant::fromValue((int)TypeLayerTin)); // 设置数据集类型
	 }
}

void SceneLayersView::updateLayer(UGLayer3D* pLayer, QTreeWidgetItem* parent, bool isTop /*= false*/)
{
	if (typeid(*pLayer) == typeid(UGLayer3DGroup))
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

		int count = ((UGLayer3DGroup*)pLayer)->GetCount();
		for (int i = 0; i < count; i++)
		{
			UGLayer3D* pLayer = ((UGLayer3DGroup*)pLayer)->GetAt(i);
			updateLayer(pLayer, pItem);
		}

	}
	else // Layer
	{
		UGString ugLayerName = pLayer->GetCaption(); // GetName();
		UGLayer::UGLayerType type = (UGLayer::UGLayerType)pLayer->GetType();
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

		ItemType layerItemType = TypeLayer;
		UGDataset* pDataset = NULL;
		switch (type)
		{
		case UGC::/*UGLayer3DType::*/l3dNone:
			break;
		case UGC::/*UGLayer3DType::*/l3dFile:
			break;
		case UGC::/*UGLayer3DType::*/l3dTree:
			break;
		case UGC::/*UGLayer3DType::*/l3dDataset:
			break;
		case UGC::/*UGLayer3DType::*/l3dGeImage:
			break;
		case UGC::/*UGLayer3DType::*/l3dTracking:
			break;
		case UGC::/*UGLayer3DType::*/l3dScreen:
			break;
		case UGC::/*UGLayer3DType::*/l3dModel:
			break;
		case UGC::/*UGLayer3DType::*/l3dTerrain:
			break;
		case UGC::/*UGLayer3DType::*/l3dMap:
			break;
		case UGC::/*UGLayer3DType::*/l3dVector:
			break;
		case UGC::/*UGLayer3DType::*/l3dDynamicModel:
			break;
		case UGC::/*UGLayer3DType::*/l3dVolumeFile:
			break;
		case UGC::/*UGLayer3DType::*/l3dCoDesigner:
			break;
		case UGC::/*UGLayer3DType::*/l3dDatasetImage:
		{
			UGLayer3DDatasetImage* pLayer3DDatasetImage = (UGLayer3DDatasetImage*)(pLayer);
			pDataset = pLayer3DDatasetImage->GetDataset();
		}
			
			break;
		case UGC::/*UGLayer3DType::*/l3dDatasetVector:
		{
			UGLayer3DDatasetVector* pLayer3DDatasetVector = (UGLayer3DDatasetVector*)(pLayer);
			pDataset = pLayer3DDatasetVector->GetDataset();
		}
			break;
		case UGC::/*UGLayer3DType::*/l3dOSGB:
			layerItemType = TypeLayerOSGB;
			pItem->setIcon(typeIconCol, Icons::getInstance().iconLayer3DOSGBCache);
			break;
		case UGC::/*UGLayer3DType::*/l3dDatasetModelPro:
		{
			UGLayer3DDatasetModelPro* pLayer3DDatasetModel = (UGLayer3DDatasetModelPro*)(pLayer);
			pDataset = pLayer3DDatasetModel->GetDataset();
		}
			break;
		case UGC::/*UGLayer3DType::*/l3dDatasetVolume:
		{
			UGLayer3DDatasetVolume* pLayer3DDatasetVolume = (UGLayer3DDatasetVolume*)(pLayer);
			pDataset = pLayer3DDatasetVolume->GetDataset();
		}
			break;
		case UGC::/*UGLayer3DType::*/l3dPlotting:
			layerItemType = TypeLayerCAD;
			pItem->setIcon(typeIconCol, Icons::getInstance().iconDatasetCAD);
			break;
		case UGC::/*UGLayer3DType::*/l3dTinTerrain:
			layerItemType = TypeLayerTerrain;
			pItem->setIcon(typeIconCol, Icons::getInstance().iconLayer3DTerrian);
			break;
	
		case UGC::/*UGLayer3DType::*/l3dWMS:
		case UGC::/*UGLayer3DType::*/l3dWMTS:
		case UGC::/*UGLayer3DType::*/l3dBingMaps:
		case UGC::/*UGLayer3DType::*/l3dGoogleMaps:
		case UGC::/*UGLayer3DType::*/l3dOpenStreetMaps:
			layerItemType = TypeLayerImage;
			pItem->setIcon(typeIconCol, Icons::getInstance().iconDatasetImage);
			break;
		case UGC::/*UGLayer3DType::*/l3dGroup:
			break;
		case UGC::/*UGLayer3DType::*/l3dDatasetVectorPoint:
		{
			UGLayer3DDatasetVectorPoint* pLayer3DDatasetPoint = (UGLayer3DDatasetVectorPoint*)(pLayer);
			pDataset = pLayer3DDatasetPoint->GetDataset();
		}
			break;
		case UGC::/*UGLayer3DType::*/l3dDatasetVectorLR:
		{
			UGLayer3DDatasetVectorLR* pLayer3DDatasetRegion = (UGLayer3DDatasetVectorLR*)(pLayer);
			pDataset = pLayer3DDatasetRegion->GetDataset();
		}
			break;
		
		default:
			pItem->setIcon(typeIconCol, Icons::getInstance().iconLayer3DOSGBCache);
			break;
		}
		if (NULL != pDataset)
		{
			layerItemType = updateItemByDataset(pDataset, pItem);
		}
		pItem->setData(0, ItemDataType, QVariant::fromValue((int)layerItemType)); // 设置数据集类型

	}
}

void SceneLayersView::addLayer(UGLayer3D* pLayer, bool isTop /*= true*/)
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

SceneLayersView::ItemType SceneLayersView::updateItemByDataset(UGDataset* pDataset, QTreeWidgetItem* pItem)
{
	UGDataset::DatasetType datasetType = pDataset->GetType();
	ItemType layerItemType = TypeLayer;

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
	case UGDataset::Network:
	case UGDataset::Network3D:
		layerItemType = TypeLayerNetwork;
		pItem->setIcon(typeIconCol, Icons::getInstance().iconDatasetRegion3D);
		break;
	default:
		pItem->setIcon(typeIconCol, Icons::getInstance().iconDataTypeUnknown);
		break;
	}

	return layerItemType;
}