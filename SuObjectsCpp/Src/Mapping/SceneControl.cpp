/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.06.06
 */
#include "Mapping/SceneControl.h"

using namespace SuperMap;

SceneControl::SceneControl(void* pWndHandle, int dpiX, int dpiY)
{
	m_pUGSceneWnd = NULL;
	//m_pRoot3D = NULL;
	m_pCameraWorld = NULL;

	m_IsMinSized = false;
	
	Initialize(pWndHandle, dpiX, dpiY);


	m_pWorkspace = NULL;

	mIsInWorkspace = false;
}
SceneControl::~SceneControl()
{
	if (m_pUGSceneWnd != NULL) 
	{
		// close the scene
		m_pUGSceneWnd->m_SceneWindow.Reset();
		
	}

	delete m_pUGSceneWnd;
	m_pUGSceneWnd = NULL;

	m_pWorkspace = NULL;
	m_pWnd = NULL;
}

UGRoot3D* SceneControl::m_pRoot3D = NULL;
bool SceneControl::mIsInitializeUGRoot3D = false;
void SceneControl::Initialize(void* pWndHandle, int dpiX, int dpiY)
{
	// set graphics engine type
//#ifdef WIN32
//	UGGraphicsManager::SetCurGraphicsType(UGC::UGGraphics::GT_Windows);
//#else
//    UGGraphicsManager::SetCurGraphicsType(UGGraphics::GT_QT);
//#endif

	// 1.
	if (SceneControl::m_pRoot3D == NULL) {
		SceneControl::m_pRoot3D = new UGC::UGRoot3D();
		
		SceneControl::m_pRoot3D->Initialize(NULL);
		SceneControl::m_pRoot3D->SetGraphics3DType(UGC::RST_OGRE);

		SceneControl::mIsInitializeUGRoot3D = true;
	}

	Window3D wnd = (Window3D)pWndHandle;       // IntPtr in VS, wndId() in Qt
	UGScene3D* pScene = m_pRoot3D->CreateScene(wnd);
	pScene->SetType(UGC::ST_EARTH_SPHERICAL);

	m_pCameraWorld = pScene->CreateCamera(_U("Camera"));
	pScene->GetRenderTarget()->AddViewport(m_pCameraWorld, 0);
	m_pRoot3D->SetActiveScene(pScene);
	

	m_pUGSceneWnd = new UGSceneEditorWnd(pScene);
	m_pUGSceneWnd->SetOpenEditMode(true);
	m_pUGSceneWnd->SetEditEnvironment();

	// dpi info 
	pScene->InitializeScene(_U(""), dpiX, dpiY);

	pScene->SetDrawMode(UGC::REAL_TIME_RASTER);

	m_pRoot3D->Render();
}

UGuint SceneControl::getKeyUGFlags(unsigned int flag)
{
	UGint flags = 0;
	if(flag == UG_VK_MENU || flag== UG_VK_SHIFT || UG_VK_CONTROL){
		flags = flag; // TODO: change
	}


	return flags;

}

void SceneControl::OnDraw(int rectLeft, int rectTop, int rectRight, int rectBottom, void* pHDC)
{
	if (m_pUGSceneWnd != NULL)
	{
		m_pUGSceneWnd->GetScene3D()->Refresh();
	}
}


void SceneControl::OnSizeChanged(int x, int y, void* pHDC)
{
	UGRect rect(0, 0, x, y);
	bool isEmpty = rect.IsEmpty();
	
	if(!isEmpty && m_pUGSceneWnd != NULL)
	{
		m_pUGSceneWnd->OnSize(x, y);
		m_pUGSceneWnd->GetScene3D()->Refresh();
	}
	else 
	{
		m_IsMinSized = isEmpty;
	}
}
void SceneControl::OnLMouseDown(unsigned int nFlags, int x, int y, void* pHDC)
{
	UGPoint pt(x, y);
	UGC::UGuint flag = getKeyUGFlags(nFlags);
	m_pUGSceneWnd->OnLButtonDown(flag, pt);
}

void SceneControl::OnLMouseUp(unsigned int nFlags, int x, int y, void* pHDC)
{
	UGPoint pt(x, y);
	UGC::UGuint flag = getKeyUGFlags(nFlags);
	m_pUGSceneWnd->OnLButtonUp(flag, pt);
}

void SceneControl::OnLMouseDbClick(unsigned int nFlags, int x, int y, void* pHDC)
{
	UGPoint pt(x, y);
	UGC::UGuint flag = getKeyUGFlags(nFlags);
	m_pUGSceneWnd->OnLButtonDblClk(flag, pt);
}
void SceneControl::OnMouseMove(unsigned int nFlags, int x, int y, void* pHDC)
{
	UGPoint pt(x, y);
	UGC::UGuint flag = getKeyUGFlags(nFlags);
	m_pUGSceneWnd->OnMouseMove(flag, pt);
}
void SceneControl::OnMouseWheel(unsigned int nFlags, short zDelta, int x, int y)
{
	// the ratio is 1 after zooming in and zooming out, map scale will not be changed.
	double ratio = 1;
	if (zDelta > 0)
	{
		ratio = 5/4.0;
	}
	else
	{
		ratio = 4/5.0;
	}

	UGPoint pt(x, y);
	
	UGC::UGuint flag = getKeyUGFlags(nFlags);
	m_pUGSceneWnd->OnMouseWheel(flag, zDelta, pt);
}

void SceneControl::OnRMouseDown(unsigned int nFlags, int x, int y, void* pHDC)
{
	UGPoint pt(x, y);
	UGC::UGuint flag = getKeyUGFlags(nFlags);
	m_pUGSceneWnd->OnRButtonDown(flag, pt);
}

void SceneControl::OnRMouseUp(unsigned int nFlags, int x, int y, void* pHDC)
{
	UGPoint pt(x, y);
	UGC::UGuint flag = getKeyUGFlags(nFlags);
	m_pUGSceneWnd->OnRButtonUp(flag, pt); 
}


void SceneControl::OnMidMouseDown(unsigned int nFlags, int x, int y, void* pHDC)
{
	UGPoint pt(x, y);
	UGC::UGuint flag = getKeyUGFlags(nFlags);
	m_pUGSceneWnd->OnMButtonUp(flag, pt);
}

void SceneControl::OnMidMouseUp(unsigned int nFlags, int x, int y, void* pHDC)
{
	UGPoint pt(x, y);
	UGC::UGuint flag = getKeyUGFlags(nFlags);
	m_pUGSceneWnd->OnMButtonDown(flag, pt);
}

UGPoint2D SceneControl::PixelToMap(int x, int y)
{
	UGPoint pt(x, y);
	return PixelToMap(pt);
}

UGPoint2D SceneControl::PixelToMap(UGPoint pt)
{
	UGPoint2D pntlp;
	UGPoint2D pnt2D(0.0, 0.0);

	
	return pnt2D;
}

UGPoint SceneControl::MapToPixel(double x, double y)
{
	UGPoint2D pnt(x, y);
	return MapToPixel(pnt);

}
UGPoint SceneControl::MapToPixel(UGPoint2D pnt)
{
	UGPoint2D pntlp;
	UGPoint pt(0, 0);

	return pt;

}

void SceneControl::Refresh()
{
	m_pUGSceneWnd->Refresh();  
	/*if (m_pUGSceneWnd != NULL) {
		m_pUGSceneWnd->GetScene3D()->GetCamera(_U("Camera"))->Refresh();
	}*/
}

void SceneControl::RenderInTimer() {
	m_pRoot3D->Render();
}




UGSceneEditorWnd* SceneControl::GetSceneEditWnd()
{
	return m_pUGSceneWnd;
}

UGLayer3Ds* SceneControl::GetUGLayers()
{
	return &(m_pUGSceneWnd->GetScene3D()->m_Layers);
}

void SceneControl::SetEditableLayer(UGLayer* pLayer, bool isEditable)
{
	
}

void SceneControl::SetWorkspace(Workspace* workspace)
{
	if (workspace == m_pWorkspace)
	{
		return;
	}
	m_pWorkspace = workspace;
	if (m_pWorkspace != NULL && m_pUGSceneWnd != NULL)
	{
		UGWorkspace* pUGWorkspace = m_pWorkspace->GetUGWorkspace();

		m_pUGSceneWnd->GetScene3D()->m_Layers.SetWorkspace(pUGWorkspace);
		m_pUGSceneWnd->GetScene3D()->m_Layers.SetDataSources(&(pUGWorkspace->m_DataSources));
		m_pUGSceneWnd->GetScene3D()->m_Layers.SetWorkspaceName(pUGWorkspace->m_WorkspaceConnection.m_strWorkspaceName);

		m_pUGSceneWnd->GetScene3D()->m_TerrainLayer3Ds.SetDataSources(&(pUGWorkspace->m_DataSources));
	}
}

bool SceneControl::OpenScene(string sceneName)
{
	UGString ugSceneName;
	ugSceneName.FromStd(sceneName);
	UGString sceneXML = m_pWorkspace->GetUGWorkspace()->m_SceneStorages.Find(ugSceneName)->GetXML();

	// The last parameter(strWorkspaceName) is neccessary, and it is the path of workspace.
	UGbool isOpen = m_pUGSceneWnd->GetScene3D()->FromXML(sceneXML, false, 0, m_pWorkspace->GetUGWorkspace()->m_WorkspaceConnection.m_strServer);
	m_pUGSceneWnd->Refresh();
	
	mIsInWorkspace = isOpen;
	return isOpen;
}

UGLayer* SceneControl::AddDataset(string datasourceName, string datasetName, bool bAddToHead)
{
	UGString ugDatasetName, ugDatasourceName;
	ugDatasourceName.FromStd(datasourceName);
	ugDatasetName.FromStd(datasetName);
	UGDataSource* pDatasource = m_pWorkspace->GetUGWorkspace()->GetDataSource(ugDatasourceName);
	UGLayer* pLayer = NULL;
	if (pDatasource != NULL) {
		UGDataset* pDataset = pDatasource->GetDataset(ugDatasetName);
		
	}
	else 
	{
		//TODO: ouput log
	}
	return pLayer;
	
}

bool SceneControl::Save()
{
	
		
	return false;
	
}

bool SceneControl::SaveAs(string mapName)
{
	
	return false;
}

void SceneControl::ActiveScene() {
	if (m_pRoot3D != NULL && m_pUGSceneWnd != NULL)
	{
		m_pRoot3D->SetActiveScene(m_pUGSceneWnd->GetScene3D());

	}
}

