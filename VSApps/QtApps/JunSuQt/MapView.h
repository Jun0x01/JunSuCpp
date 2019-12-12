#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QWidget>
#include <QPainter>
#include <QWheelEvent>
#include <QDebug>

#include "Mapping/MapControl.h"
#include "Data/LicenseManager.h"

/**
 * @brief 地图窗口
 */
class MapView : public QWidget
{
    Q_OBJECT
public:
    explicit MapView(QWidget *parent = NULL);

    virtual ~MapView();

//method
private:
    unsigned int getMouseOrKeyFlag(QMouseEvent* event);

    // override parent
protected:
    virtual void paintEvent(QPaintEvent* event);
    virtual void wheelEvent(QWheelEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void resizeEvent(QResizeEvent* event);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);


//
private:
    MapControl* m_pMapControl;    // 地图控件
    UGDynamicLayer *pDyLayer;     // 动态层

    QPoint pt;


public:
    MapControl* getMapControl();

signals:

public slots:
    void onSaveMap();
};

#endif // MAPVIEW_H
