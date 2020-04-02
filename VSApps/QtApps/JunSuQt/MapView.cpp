#include "MapView.h"

/**************** CallBack *********************/
void SuCALLBACK InvalidateCallbackMapView(void * pWnd)
{
    MapView* pThis = (MapView*)pWnd;

    pThis->update();
}
/**************** End of CallBack *********************/

MapView::MapView(QWidget *parent) : QWidget(parent)
{
    bool isValid = LicenseManager::IsValidStd();
    if(isValid)
    {
        m_pMapControl = new MapControl(InvalidateCallbackMapView, this);
    }else
    {
        m_pMapControl = NULL;
        qDebug("许可无效，请配置SuperMap组件许可.");
        //  使用句号（。）引起
        // C2001: newline in constant
        // C2143: syntax error: missing ')' before '}'
        // C2143: syntax error: missing ';' before '}'
    }


	pMapLayersView = new MapLayersView();
}
MapView::~MapView() 
{
    //pMapLayersView->setParent(NULL);
	//elete pMapLayersView;

    delete m_pMapControl;
}


/*********** Override event functions ************/
void MapView::paintEvent(QPaintEvent* event)
{
    if(m_pMapControl == NULL)
    {
        return;
    }
    m_pMapControl->OnDraw(0, 0, this->width(), this->height());
    uchar* imgData = m_pMapControl->GetImageBytes();
    if (imgData != NULL)
    {
        QImage* pQImage = new QImage(imgData, this->width(), this->height(), QImage::Format_ARGB32);

        QColor background(255, 255, 255);
        QPainter painter;
        painter.begin(this);
        painter.fillRect(0, 0, this->width(), this->height(), background);
        painter.drawImage(QRectF(0, 0, this->width(), this->height()), *pQImage);

        // paint signal / slot

        painter.end();
        delete pQImage;
    }
    else {
        // no map data
    }
}

unsigned int MapView::getMouseOrKeyFlag(QMouseEvent* event)
{
    unsigned int flag = 0;

    if (event->modifiers() & Qt::ShiftModifier)
    {
        flag = flag | UG_MK_SHIFT;
    }
    if ((event->modifiers() & Qt::ControlModifier))
    {
        flag = flag | UG_MK_CONTROL;
    }
    return flag;
}

void MapView::wheelEvent(QWheelEvent* event)
{
    if(m_pMapControl != NULL)
    {
        m_pMapControl->OnMouseWheel(0, event->delta(), event->x(), event->y());
    }
}

void MapView::mousePressEvent(QMouseEvent* event)
{
    if(m_pMapControl == NULL)
    {
        return;
    }
    int flag = getMouseOrKeyFlag(event);

    if (event->button() == Qt::LeftButton)
    {// LeftButton
        m_pMapControl->OnLMouseDown(flag, event->x(), event->y());
    }
    else if (event->button() == Qt::RightButton)
    {// RightButton
        m_pMapControl->OnRMouseDown(flag, event->x(), event->y());
    }
    else if (event->button() == Qt::MidButton)
    {// MidButton
        m_pMapControl->OnMidMouseDown(flag, event->x(), event->y());
    }

    pt.setX(event->x());
    pt.setY(event->y());
}

void MapView::mouseReleaseEvent(QMouseEvent* event)
{
    if(m_pMapControl == NULL)
    {
        return;
    }

    int flag = getMouseOrKeyFlag(event);

    if (event->button() == Qt::LeftButton)
    {// LeftButton
        m_pMapControl->OnLMouseUp(flag, event->x(), event->y());
    }
    else if (event->button() == Qt::RightButton)
    {// RightButton
        m_pMapControl->OnRMouseUp(flag, event->x(), event->y());
    }
    else if (event->button() == Qt::MidButton)
    {// MidButton
        m_pMapControl->OnMidMouseUp(flag, event->x(), event->y());
    }

    UGPoint2D pt = m_pMapControl->PixelToMap(event->x(), event->y());
	qDebug() << "x: " << pt.x << ", y: " << pt.y << endl;
	UGPoints pts;
	UGPoint pt1(event->x(), event->y());
	pts.Add(pt1);
	UGPoint2Ds outPts;
	m_pMapControl->ToGeoCoords(pts, outPts);
	qDebug() << "x1: " << outPts.GetAt(0).x << ", y1: " << outPts.GetAt(0).y << endl;
    
}

void MapView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(m_pMapControl == NULL)
    {
        return;
    }
    int flag = getMouseOrKeyFlag(event);

    if (event->button() == Qt::LeftButton)
    {// LeftButton double click, zoom in
        m_pMapControl->OnLMouseDbClick(flag, event->x(), event->y());
    }
}
void MapView::mouseMoveEvent(QMouseEvent* event)
{
    int flag = getMouseOrKeyFlag(event);

    if(m_pMapControl != NULL)
    {
        m_pMapControl->OnMouseMove(flag, event->x(), event->y());
    }
}
void MapView::resizeEvent(QResizeEvent* event)
{
    if(m_pMapControl != NULL)
    {
         m_pMapControl->OnSizeChanged(this->width(), this->height());
    }
}
void MapView::keyPressEvent(QKeyEvent* event)
{

}

/*********** End of overriding event functions*****************/


MapControl* MapView::getMapControl()
{
    return m_pMapControl;
}

void MapView::onSaveMap()
{

}

MapLayersView* MapView::getMapLayersView()
{
	return pMapLayersView;
}
