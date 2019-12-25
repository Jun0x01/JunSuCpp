#ifndef ICONS_H
#define ICONS_H
#include <QIcon>

class Icons
{
private:
    Icons();

public:
    static  Icons& getInstance();

    QIcon iconWk;
    QIcon iconDSes;
    QIcon iconMaps;
    QIcon iconScenes;

	QIcon iconDs;
    QIcon iconMap;
    QIcon iconScene;

	// 不同数据源类型类型
	QIcon iconDsUDB;
	QIcon iconDsUDBX;
	QIcon iconDsImage;
	QIcon iconDsRest;
	QIcon iconDsOSM;
	QIcon iconDsOGC;
	QIcon iconDsWeb;

	// 不同数据集类型图标
	QIcon iconDataTypeUnknown;
	QIcon iconDatasetPoint;
	QIcon iconDatasetLine;
	QIcon iconDatasetRegion;
	QIcon iconDatasetCAD;

	QIcon iconDatasetText;
	QIcon iconDatasetTabular;
	QIcon iconDatasetNetwork;
	QIcon iconDatasetGrid;
	QIcon iconDatasetDEM;
	QIcon iconDatasetImage;

	QIcon iconDatasetPoint3D;
	QIcon iconDatasetLine3D;
	QIcon iconDatasetRegion3D;
	QIcon iconDatasetModel;
	QIcon iconDatasetNetwork3D;

	QIcon iconLayerGroup;

	QIcon iconLayer3DImageCache;
	QIcon iconLayer3DOSGBCache;
	QIcon iconLayer3DTerrian;
};

#endif // ICONS_H
