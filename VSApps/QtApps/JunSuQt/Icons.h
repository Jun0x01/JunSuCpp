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
};

#endif // ICONS_H
