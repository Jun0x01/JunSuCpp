#include "Icons.h"



// 工作空间中的数据对应的icon
//QIcon Icons::iconWk(":/Images/Resources/Workspace/smwu.png");
//QIcon Icons::iconDs(":/Images/Resources/Workspace/Datasources.png");
//QIcon Icons::iconMaps(":/Images/Resources/Workspace/Maps.png");
//QIcon Icons::iconScenes(":/Images/Resources/Workspace/Scenes.png");
//QIcon Icons::iconMap(":/Images/Resources/Workspace/Map.png");
//QIcon Icons::iconScene(":/Images/Resources/Workspace/Scene.png");

Icons::Icons():
    iconWk(":/Images/Resources/Workspace/smwu.png"),
    iconDSes(":/Images/Resources/Workspace/Datasources.png"),
    iconMaps(":/Images/Resources/Workspace/Maps.png"),
    iconScenes(":/Images/Resources/Workspace/Scenes.png"),
	iconDs(":/Images/Resources/Workspace/Datasource.png"),
    iconMap(":/Images/Resources/Workspace/Map.png"),
    iconScene(":/Images/Resources/Workspace/Scene.png"),
    iconDsUDB(":/Images/Resources/Datasource/Image_UDB.png"),
    iconDsUDBX(":/Images/Resources/Datasource/Image_UDBX.png"),
    iconDsImage(":/Images/Resources/Datasource/Image_Img.png"),
    iconDsOSM(":/Images/Resources/Datasource/Image_OSM.png"),
    iconDsRest(":/Images/Resources/Datasource/Image_Rest.png"),
    iconDsOGC(":/Images/Resources/Datasource/Image_OGC.png"),
    iconDsWeb(":/Images/Resources/Datasource/Image_DatasourceWeb.png"),
    iconDataTypeUnknown(":/Images/Resources/Datasets/Image_Unknown.png"),
    iconDatasetPoint(":/Images/Resources/Datasets/Image_Point.png"),
    iconDatasetLine(":/Images/Resources/Datasets/Image_Line.png"),
    iconDatasetRegion(":/Images/Resources/Datasets/Image_Region.png"),
    iconDatasetCAD(":/Images/Resources/Datasets/Image_CAD.png"),
    iconDatasetText(":/Images/Resources/Datasets/Image_Text.png"),
    iconDatasetTabular(":/Images/Resources/Datasets/Image_Tabular.png"),
    iconDatasetNetwork(":/Images/Resources/Datasets/Image_NetWork.png"),
    iconDatasetGrid(":/Images/Resources/Datasets/Image_DatasetGrid.png"),
    iconDatasetDEM(":/Images/Resources/Datasets/Image_DatasetGrid.png"),
    iconDatasetImage(":/Images/Resources/Datasets/Image_DatasetImage.png"),
    iconDatasetPoint3D(":/Images/Resources/Datasets/Image_Point3D.png"),
    iconDatasetLine3D(":/Images/Resources/Datasets/Image_Line3D.png"),
    iconDatasetRegion3D(":/Images/Resources/Datasets/Image_Region3D.png"),
    iconDatasetModel(":/Images/Resources/Datasets/Image_Model.png"),
    iconDatasetNetwork3D(":/Images/Resources/Datasets/Image_Network3D.png"),
	iconLayerGroup(":/Images/Resources/Layers/LayerGroup.png"),
	iconLayer3DImageCache(":/Images/Resources/Layers/Layer3D_ImageCache3D.png"),
	iconLayer3DOSGBCache(":/Images/Resources/Layers/Layer3D_OSGBCache.png"),
	iconLayer3DTerrian(":/Images/Resources/Layers/Layer3D_Terrain.png")
{

}

// 利用了静态局部变量的特性，通常是不使用静态局部变量的
Icons& Icons::getInstance(){
    static Icons icons;
    return icons;
}
