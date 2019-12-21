#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include <QWidget>
#include <QPainter>
#include <QWheelEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QTimer>

#include <QDebug>

#include "Mapping/SceneControl.h"

using namespace SuperMap;

class SceneView : public QWidget
{
    Q_OBJECT
public:
    explicit SceneView(QWidget *parent = nullptr);

	virtual ~SceneView();

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


private:
	SceneControl* m_pSceneControl;
	bool isRenderTimerStarted;
	QTimer* renderTimer; // ³¡¾°äÖÈ¾¶¨Ê±Æ÷
	

public:
	SceneControl* GetSceneControl();

	

signals:

public slots:
	void RenderInTimer();
};

#endif // SCENEVIEW_H