#include "SceneView.h"

SceneView::SceneView(QWidget *parent) : QWidget(parent)
{
	m_pSceneControl = NULL;
	//QDesktopWidget* desktop = QApplication::desktop();
	//int logicalDpiX = desktop->logicalDpiX();
	//int logicalDpiY = desktop->logicalDpiY();

	//m_pSceneControl = new SceneControl((HWND)winId(), logicalDpiX, logicalDpiX);

	renderTimer = new QTimer();
	connect(renderTimer, SIGNAL(timeout()), this, SLOT(RenderInTimer()));
	isRenderTimerStarted = false;
}

SceneView::~SceneView()
{
	renderTimer->stop();
	delete renderTimer;

	delete m_pSceneControl;
}

void SceneView::paintEvent(QPaintEvent* event)
{
	
}

unsigned int SceneView::getMouseOrKeyFlag(QMouseEvent* event)
{
	unsigned int flag = 0;

	if (event->modifiers() & Qt::ShiftModifier)
	{
		flag = flag | UG_VK_SHIFT;
	}
	if ((event->modifiers() & Qt::ControlModifier))
	{
		flag = flag | UG_VK_CONTROL;
	}
	if (event->modifiers()&Qt::MetaModifier)
	{
		flag = flag | UG_VK_MENU;
	}
	return flag;
}

void SceneView::wheelEvent(QWheelEvent* event)
{
	if (m_pSceneControl != NULL)
	{
		m_pSceneControl->OnMouseWheel(0, event->delta(), event->x(), event->y());
	}
}
void SceneView::mousePressEvent(QMouseEvent* event)
{
	if (m_pSceneControl == NULL)
	{
		return;
	}
	int flag = getMouseOrKeyFlag(event);

	if (event->button() == Qt::LeftButton)
	{// LeftButton
		m_pSceneControl->OnLMouseDown(flag, event->x(), event->y());
	}
	else if (event->button() == Qt::RightButton)
	{// RightButton
		m_pSceneControl->OnRMouseDown(flag, event->x(), event->y());
	}
	else if (event->button() == Qt::MidButton)
	{// MidButton
		m_pSceneControl->OnMidMouseDown(flag, event->x(), event->y());
	}
}
void SceneView::mouseReleaseEvent(QMouseEvent* event)
{
	if (m_pSceneControl == NULL)
	{
		return;
	}

	int flag = getMouseOrKeyFlag(event);

	if (event->button() == Qt::LeftButton)
	{// LeftButton
		m_pSceneControl->OnLMouseUp(flag, event->x(), event->y());
	}
	else if (event->button() == Qt::RightButton)
	{// RightButton
		m_pSceneControl->OnRMouseUp(flag, event->x(), event->y());
	}
	else if (event->button() == Qt::MidButton)
	{// MidButton
		m_pSceneControl->OnMidMouseUp(flag, event->x(), event->y());
	}
}
void SceneView::mouseMoveEvent(QMouseEvent* event)
{
	if (m_pSceneControl != NULL)
	{
		m_pSceneControl->OnMouseMove(getMouseOrKeyFlag(event), event->x(), event->y());
	}
}
void SceneView::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	int width = this->width();
	int height = this->height();
	if (!isRenderTimerStarted && width > 0 & height > 0) {
		QDesktopWidget* desktop = QApplication::desktop();
		int logicalDpiX = desktop->logicalDpiX();
		int logicalDpiY = desktop->logicalDpiY();

		m_pSceneControl = new SceneControl((HWND)winId(), logicalDpiX, logicalDpiX);

		renderTimer->start(30);
		isRenderTimerStarted = true;
	}

	qDebug() << "SceneView width: " << width << "  height: " << height << endl;
	if (m_pSceneControl != NULL && width > 0 & height > 0)
	{
		m_pSceneControl->OnSizeChanged(width, height);
	}
}
void SceneView::keyPressEvent(QKeyEvent* event)
{

}
void SceneView::mouseDoubleClickEvent(QMouseEvent *event)
{

}

SceneControl* SceneView::GetSceneControl()
{
	return m_pSceneControl;
}

void SceneView::RenderInTimer()
{
	if (NULL != m_pSceneControl)
	{
		if (isVisible()) {
			m_pSceneControl->RenderInTimer();
		}

	}
}