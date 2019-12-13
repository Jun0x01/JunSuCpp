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
    iconScene(":/Images/Resources/Workspace/Scene.png")
{

}

// 利用了静态局部变量的特性，通常是不使用静态局部变量的
Icons& Icons::getInstance(){
    static Icons icons;
    return icons;
}
