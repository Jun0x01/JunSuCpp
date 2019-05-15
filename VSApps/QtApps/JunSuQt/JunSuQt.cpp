/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.05.15
 */
#include "JunSuQt.h"

/**************** CallBack *********************/
void SuCALLBACK InvalidateCallback(void * pWnd)
{
	JunSuQt* pThis = (JunSuQt*)pWnd;

	pThis->update();

}
/**************** End of CallBack *********************/

/*************** Init **********************/
JunSuQt::JunSuQt(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setMouseTracking(true);
	centralWidget()->setMouseTracking(true);

	initMenuBar();
	//initToolBar();

	m_pMapControl = new MapControl(InvalidateCallback, this);
}

JunSuQt::~JunSuQt() {

	delete m_pMapControl;
}


void JunSuQt::initMenuBar() {
	QMenuBar* menuBar = this->menuBar();
	// Menu -> File
	{
		QMenu* menu = new QMenu("File", menuBar);
		// File -> New
		{
			QAction* menuAction = new QAction("New");
			menuAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));

			//menu->addAction(menuAction);
			//menu->addSeparator();
		}
		// File -> Open
		{
			QAction* menuAction = new QAction("Open");
			menuAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_O));
			
			menu->addAction(menuAction);
			menu->addSeparator();

			connect(menuAction, SIGNAL(triggered()), this, SLOT(Menu_File_Open()));
		}
		// File -> Close
		{
			QAction* menuAction = new QAction("Close");
			menuAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_C));

			menu->addAction(menuAction);
			menu->addSeparator();
		}
		menuBar->addMenu(menu);
	}
	// Menu -> Edit
	{
		QMenu* menu = new QMenu("Edit", menuBar);
		// Eidt -> Pan Map
		{
			QAction* menuAction = new QAction("Pan Map");
			menu->addAction(menuAction);
			menu->addSeparator();

            connect(menuAction, SIGNAL(triggered()), this, SLOT(Menu_Edit_PanMap()));
		}
		menuBar->addMenu(menu);
	}
	// Menu -> Draw
	{
		QMenu* menu = new QMenu("Draw", menuBar);
		// Draw -> Point
		{
			QAction* menuAction = new QAction("Draw Point");
			menu->addAction(menuAction);
			menu->addSeparator();

			connect(menuAction, SIGNAL(triggered()), this, SLOT(Menu_Draw_Point()));
		}
		// Draw -> Line
		{
			QAction* menuAction = new QAction("Draw Line");
			menu->addAction(menuAction);
			menu->addSeparator();

			connect(menuAction, SIGNAL(triggered()), this, SLOT(Menu_Draw_Line()));
		}
		// Draw -> Polygon
		{
			QAction* menuAction = new QAction("Draw Polygon");
			menu->addAction(menuAction);
			menu->addSeparator();

			connect(menuAction, SIGNAL(triggered()), this, SLOT(Menu_Draw_Polygon()));
		}
		menuBar->addMenu(menu);
	}
}

void JunSuQt::initToolBar() {
	QToolBar* toolBar = addToolBar("ToolBar");
	{
		{
			QAction* action = new QAction();
			action->setToolTip("Open");
			action->setIcon(QIcon(":/JunSuQt/Resources/open.png"));

			toolBar->addAction(action);

			connect(action, SIGNAL(triggered()), this, SLOT(Menu_File_Open()));
		}
		// Draw Line
		{
			QAction* action = new QAction();
			action->setToolTip("Draw Line");
			action->setIcon(QIcon(":/JunSuQt/Resources/Line.png"));

			toolBar->addAction(action);

			connect(action, SIGNAL(triggered()), this, SLOT(Menu_Draw_Line()));
		}
		
	}
}
/******************* End of Init **********************/

/*********** Override ************/
void JunSuQt::paintEvent(QPaintEvent* event)
{
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
		// error
	}

	
}
unsigned int JunSuQt::getMouseOrKeyFlag(QMouseEvent* event)
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

void JunSuQt::wheelEvent(QWheelEvent* event)
{
	m_pMapControl->OnMouseWheel(0, event->delta(), event->x(), event->y());
}
void JunSuQt::mousePressEvent(QMouseEvent* event)
{
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

}
void JunSuQt::mouseReleaseEvent(QMouseEvent* event)
{
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
}

void JunSuQt::mouseDoubleClickEvent(QMouseEvent *event)
{
	int flag = getMouseOrKeyFlag(event);

	if (event->button() == Qt::LeftButton)
	{// LeftButton double click, zoom in
		m_pMapControl->OnLMouseDbClick(flag, event->x(), event->y());
	}
}
void JunSuQt::mouseMoveEvent(QMouseEvent* event)
{
	int flag = getMouseOrKeyFlag(event);

	m_pMapControl->OnMouseMove(flag, event->x(), event->y());

}
void JunSuQt::resizeEvent(QResizeEvent* event)
{
	m_pMapControl->OnSizeChanged(this->width(), this->height());
}
void JunSuQt::keyPressEvent(QKeyEvent* event)
{

}

/*********** End of Override*****************/

/************ Menu Functions ****************************/

void JunSuQt::Menu_File_Open()
{
	QString qStrPath = QFileDialog::getOpenFileName(this, QString::fromUtf8("OpenWorksapce"), "../../../TestData/", tr("smwu(*.smwu)"));

	if (!qStrPath.isEmpty())
	{
		string strPath = qStrPath.toStdString();
		OpenWorkspace(strPath);
	}
	else
	{

	}
}

void JunSuQt::Menu_File_Close()
{
	CloseWorkspace();
}
void JunSuQt::Menu_Draw_Point()
{
	UGMapEditorWnd* pMapEditorWnd = m_pMapControl->GetMapEditWnd();
	UGMap* pUGMap = &(pMapEditorWnd->m_mapWnd.m_Map);

	UGString layerName = _U("DrawPoint@World");
	UGLayer* pLayer = m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map.m_Layers.GetLayer(layerName);
	if (pLayer != NULL)
	{
		pLayer->SetSelectable(true);

		UGLayers* pLayers = &(pUGMap->m_Layers);
		pLayers->SetEditableLayer(pLayer, true);

		pMapEditorWnd->SetUserAction(UGC::UGDrawParamaters::uaEdit, UGC::UGEditType::ET_Point);
	}
	else
	{

	}
}
void JunSuQt::Menu_Draw_Line()
{
	UGMapEditorWnd* pMapEditorWnd = m_pMapControl->GetMapEditWnd();
	UGMap* pUGMap = &(pMapEditorWnd->m_mapWnd.m_Map);

	UGString layerName = _U("DrawLine@World");
	UGLayer* pLayer = m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map.m_Layers.GetLayer(layerName);
	if (pLayer != NULL)
	{
		pLayer->SetSelectable(true);

		UGLayers* pLayers = &(pUGMap->m_Layers);
		pLayers->SetEditableLayer(pLayer, true);

		pMapEditorWnd->SetUserAction(UGC::UGDrawParamaters::uaEdit, UGC::UGEditType::ET_PolyLine);
	}
	else
	{

	}
}
void JunSuQt::Menu_Draw_Polygon()
{
	UGMapEditorWnd* pMapEditorWnd = m_pMapControl->GetMapEditWnd();
	UGMap* pUGMap = &(pMapEditorWnd->m_mapWnd.m_Map);

	UGString layerName = _U("DrawRegion@World");
	UGLayer* pLayer = m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map.m_Layers.GetLayer(layerName);
	if (pLayer != NULL)
	{
		pLayer->SetSelectable(true);

		UGLayers* pLayers = &(pUGMap->m_Layers);
		pLayers->SetEditableLayer(pLayer, true);

		pMapEditorWnd->SetUserAction(UGC::UGDrawParamaters::uaEdit, UGC::UGEditType::ET_PolyGon);
	}
	else
	{

	}
}

/************* End of Menu functons******************************************/

void JunSuQt::OpenWorkspace(string strPath)
{
	// Close first
	CloseWorkspace();

	// Open workspace file
	UGString wkPath;
	wkPath.FromStd(strPath);       // Convert String

	UGWorkspaceConnection wkCon;

	wkCon.m_strServer = wkPath;
	wkCon.m_nWorkspaceType = UGWorkspace::WS_Version_SMWU;

	UGWorkspace* pWorkspace = m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map.GetWorkspace();
	if (pWorkspace->Open(wkCon))
	{
		if (pWorkspace->m_MapStorages.GetCount() > 0)
		{
			UGString mapName = pWorkspace->m_MapStorages.GetNameAt(0);
			UGbool isOpen = m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map.Open(mapName);
			m_pMapControl->Refresh();
		}
	}
	else
	{

	}
}

void JunSuQt::CloseWorkspace()
{
	UGMap* pUGMap = &(m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map);
	// close map
	pUGMap->Reset();
	pUGMap->m_strName = _U("UntitledMap");
	pUGMap->SetModifiedFlag(false);
	m_pMapControl->Refresh();

	// close workspace
	UGWorkspace* pWorkspace = pUGMap->GetWorkspace();
	pWorkspace->Close();

	Pan();
}

void JunSuQt::Menu_Edit_PanMap()
{
	ResetLastEditLayer(NULL);
	Pan();
}

void JunSuQt::Pan()
{
	UGMapEditorWnd* pMapEditorWnd = m_pMapControl->GetMapEditWnd();
	pMapEditorWnd->SetUserAction(UGC::UGDrawParamaters::uaPan);
}

void JunSuQt::ResetLastEditLayer(UGLayer* pLayer)
{
	if (m_pLastEditLayer != NULL)
	{
		UGLayers* pLayers = &(m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map.m_Layers);
		pLayers->SetEditableLayer(m_pLastEditLayer, false);
	}
	m_pLastEditLayer = pLayer;
}
