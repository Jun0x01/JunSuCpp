#include "WorkspaceView.h"
#include <QApplication>
#include "Engine/UGDataset.h"
#include "Base/OgdcArray.h"

WorkspaceView::WorkspaceView(QWidget *parent) : QTreeWidget(parent)
{

//    this->headerItem()->setText(0, tr("No Workspace"));

    pListWorkspaceItems = new QList<QTreeWidgetItem>();


    // 添加默认的工作空间列表
    addTreeWorkspaceItem();

    this->setHeaderHidden(true); // 隐藏Header
    this->expandAll();           // 展开全部
//    this->expandItem();        // 展开指定item

	ItemDataType = Qt::UserRole + 1;
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
    pItemWorkspace->setIcon(0, Icons::getInstance().iconWk);
    pTreeDatasources->setIcon(0, Icons::getInstance().iconDSes);
    pTreeMaps->setIcon(0, Icons::getInstance().iconMaps);
    pTreeScenes->setIcon(0, Icons::getInstance().iconScenes);

	// 设置类型
	pItemWorkspace->setData(0, ItemDataType, QVariant::fromValue((int)TypeWorkspace));
	pTreeDatasources->setData(0, ItemDataType, QVariant::fromValue((int)TypeDataosources));
	pTreeMaps->setData(0, ItemDataType, QVariant::fromValue((int)TypeMaps));
	pTreeScenes->setData(0, ItemDataType, QVariant::fromValue((int)TypeScenes));

//    pListWorkspaceItems->append(pItemWorkspace);
}

void WorkspaceView::clearItems(QTreeWidgetItem *pItem)
{
    if(NULL != pItem)
    {
        int count = pItem->childCount();
        for(int i=0; i < count; i++)
        {
            QTreeWidgetItem* item = pItem->child(0);
            pItem->removeChild(item);
            delete item;
        }
    }
}

void WorkspaceView::updateWorkspaceList(Workspace& workspace)
{
    clearItems(pItemWorkspace);

    pTreeDatasources = new QTreeWidgetItem(pItemWorkspace);
    pTreeMaps = new QTreeWidgetItem(pItemWorkspace);
    pTreeScenes = new QTreeWidgetItem(pItemWorkspace);

    // 设置标题
    pItemWorkspace->setText(0, "Empty Workspace");
    pTreeDatasources->setText(0, tr("DataSources"));
    pTreeMaps->setText(0, tr("Maps"));
    pTreeScenes->setText(0, tr("Scense"));

    // 设置图标
    pItemWorkspace->setIcon(0, Icons::getInstance().iconWk);
    pTreeDatasources->setIcon(0, Icons::getInstance().iconDSes);
    pTreeMaps->setIcon(0, Icons::getInstance().iconMaps);
    pTreeScenes->setIcon(0, Icons::getInstance().iconScenes);

	// 设置类型
	pItemWorkspace->setData(0, ItemDataType, QVariant::fromValue((int)TypeWorkspace));
	pTreeDatasources->setData(0, ItemDataType, QVariant::fromValue((int)TypeDataosources));
	pTreeMaps->setData(0, ItemDataType, QVariant::fromValue((int)TypeMaps));
	pTreeScenes->setData(0, ItemDataType, QVariant::fromValue((int)TypeScenes));

    UGWorkspace* pUGWorkspace = workspace.GetUGWorkspace();
    string wkName = UGStrConvertor::Tostring(pUGWorkspace->GetCaption());

    pItemWorkspace->setText(0, QString().fromStdString(wkName));

    // 1. 数据源
    UGDataSources& datasources = pUGWorkspace->m_DataSources; // 注意使用引用，否则会出错

    for(int i=0; i<datasources.GetCount(); i++)
    {
        UGString ugDsName;
        UGDataSource* pDatasource = NULL;

        datasources.GetAt(i, ugDsName, pDatasource);

		updateNewDatasource(*pDatasource);
		
    }

    // 2. 地图
    int mapCount = pUGWorkspace->m_MapStorages.GetCount();
    for(int i=0; i < mapCount; i++)
    {
        UGString ugMapName = pUGWorkspace->m_MapStorages.GetNameAt(i);

        string mapName = UGStrConvertor::Tostring(ugMapName);

        QTreeWidgetItem* pItem = new QTreeWidgetItem(pTreeMaps);
        pItem->setText(0, QString().fromStdString(mapName));
        pItem->setIcon(0, Icons::getInstance().iconMap);
		pItem->setData(0, ItemDataType, QVariant::fromValue((int)TypeMap)); // 设置数据集类型
    }

    // 3. 场景
    int sceneCount = pUGWorkspace->m_SceneStorages.GetCount();
    for(int i = 0; i< sceneCount; i++)
    {
        UGString ugSceneName = pUGWorkspace->m_SceneStorages.GetNameAt(i);

        string sceneName = UGStrConvertor::Tostring(ugSceneName);

        QTreeWidgetItem* pItem = new QTreeWidgetItem(pTreeScenes);
        pItem->setText(0, QString().fromStdString(sceneName));
        pItem->setIcon(0, Icons::getInstance().iconScene);
		pItem->setData(0, ItemDataType, QVariant::fromValue((int)TypeScene)); // 设置数据集类型
    }
}

void WorkspaceView::updateNewDatasource(UGDataSource& ugDatasource)
{
	UGString ugDsName =  ugDatasource.GetAlias();


	UGEngineType dsType = ugDatasource.GetEngineType();
	string dsName = UGStrConvertor::Tostring(ugDsName);

	QTreeWidgetItem* pDsItem = new QTreeWidgetItem(pTreeDatasources);
	pDsItem->setText(0, QString().fromStdString(dsName));
	pDsItem->setIcon(0, Icons::getInstance().iconDs);

	ItemType dsItemType = TypeDatasource;
	switch (dsType)
	{
	case UGC::UDB:
		dsItemType = TypeDatasourceUDB;
		pDsItem->setIcon(0, Icons::getInstance().iconDsUDB);
		break;
	case UGC::Spatialite:
		dsItemType = TypeDatasourceUDBX;
		pDsItem->setIcon(0, Icons::getInstance().iconDsUDBX);
		break;
	case UGC::ImagePlugins:
		dsItemType = TypeDatasourceIMG;
		pDsItem->setIcon(0, Icons::getInstance().iconDsImage);
		break;
	case UGC::Rest:
		pDsItem->setIcon(0, Icons::getInstance().iconDsRest);
		break;
	case UGC::OpenStreetMaps:
		pDsItem->setIcon(0, Icons::getInstance().iconDsOSM);
		break;
	case UGC::WEB:
	case UGC::SuperMapCloud:
		pDsItem->setIcon(0, Icons::getInstance().iconDsWeb);
		break;
	}
	pDsItem->setData(0, ItemDataType, QVariant::fromValue((int)dsItemType)); // 设置数据源类型

	// 数据集
	UGDatasets* pDatasets = ugDatasource.GetDatasets();
	int datasetCount = pDatasets->GetSize();
	for (int j = 0; j < datasetCount; j++)
	{

		UGDataset* pDataset = pDatasets->GetAt(j);
		UGString ugDatasetName = pDataset->GetName();
		string datasetName = UGStrConvertor::Tostring(ugDatasetName);

		UGDataset::DatasetType datasetType = pDataset->GetType();

		QTreeWidgetItem* pDatasetItem = new QTreeWidgetItem(pDsItem);
		pDatasetItem->setText(0, QString().fromStdString(datasetName));

		ItemType datasetItemType = TypeDataset;
		switch (datasetType)
		{
		case UGDataset::/*DatasetType::*/Point:
			datasetItemType = TypeDatasetPoint;
			pDatasetItem->setIcon(0, Icons::getInstance().iconDatasetPoint);
			break;
		case UGDataset::Line:
			datasetItemType = TypeDatasetLine;
			pDatasetItem->setIcon(0, Icons::getInstance().iconDatasetLine);
			break;
		case UGDataset::Region:
			datasetItemType = TypeDatasetRegion;
			pDatasetItem->setIcon(0, Icons::getInstance().iconDatasetRegion);
			break;
		case UGDataset::CAD:
			datasetItemType = TypeDatasetCAD;
			pDatasetItem->setIcon(0, Icons::getInstance().iconDatasetCAD);
			break;
		case UGDataset::Text:
			datasetItemType = TypeDatasetText;
			pDatasetItem->setIcon(0, Icons::getInstance().iconDatasetText);
			break;

		case UGDataset::Tabular:
			datasetItemType = TypeDatasetTabular;
			pDatasetItem->setIcon(0, Icons::getInstance().iconDatasetTabular);
			break;
		case UGDataset::Network:
			datasetItemType = TypeDatasetNetwork;
			pDatasetItem->setIcon(0, Icons::getInstance().iconDatasetNetwork);
			break;
		case UGDataset::Grid:
			datasetItemType = TypeDatasetGrid;
			pDatasetItem->setIcon(0, Icons::getInstance().iconDatasetGrid);
			break;
		case UGDataset::DEM:
			datasetItemType = TypeDatasetDEM;
			pDatasetItem->setIcon(0, Icons::getInstance().iconDatasetDEM);
			break;

		case UGDataset::PointZ:
			datasetItemType = TypeDatasetPoint3D;
			pDatasetItem->setIcon(0, Icons::getInstance().iconDatasetPoint3D);
			break;
		case UGDataset::LineZ:
			datasetItemType = TypeDatasetLine3D;
			pDatasetItem->setIcon(0, Icons::getInstance().iconDatasetLine3D);
			break;
		case UGDataset::RegionZ:
			datasetItemType = TypeDatasetRegion3D;
			pDatasetItem->setIcon(0, Icons::getInstance().iconDatasetRegion3D);
			break;
		case UGDataset::Model:
			datasetItemType = TypeDatasetModel;
			pDatasetItem->setIcon(0, Icons::getInstance().iconDatasetModel);
			break;
		case UGDataset::Network3D:
			datasetItemType = TypeDatasetNetwork3D;
			pDatasetItem->setIcon(0, Icons::getInstance().iconDatasetNetwork3D);
			break;
		default:
			pDatasetItem->setIcon(0, Icons::getInstance().iconDataset);
			break;
		}
		pDatasetItem->setData(0, ItemDataType, QVariant::fromValue((int)datasetItemType)); // 设置数据集类型
	}
}