/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.06.06
 */
#include "Mapping/SceneControl.h"
#include "Data/Log.h"

using namespace SuperMap;

SceneControl::SceneControl(void* pWndHandle, int dpiX, int dpiY)
{
	m_pUGSceneWnd = NULL;
	//m_pRoot3D = NULL;
	m_pCameraWorld = NULL;

	m_IsMinSized = false;
	
	Initialize(pWndHandle, dpiX, dpiY);

	m_pInnerWorkspace = new Workspace();
	m_pWorkspace = NULL;
	SetWorkspace(m_pInnerWorkspace);

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

	delete m_pInnerWorkspace;
	m_pInnerWorkspace = NULL;
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

UGPoint3D SceneControl::PixelToSceneGlobal(int x, int y)
{
	UGPoint pt(x, y);
	return PixelToSceneGlobal(pt);
}

UGPoint3D SceneControl::PixelToSceneGlobal(UGPoint pt)
{
	UGPoint2D pntlp;
	UGPoint3D pnt2D(0.0, 0.0, 0.0);

	// TODO:
	return pnt2D;
}

UGPoint SceneControl::SceneGlobalToPixel(double x, double y, double z)
{
	UGPoint3D pnt(x, y, z);
	return SceneGlobalToPixel(pnt);

}
UGPoint SceneControl::SceneGlobalToPixel(UGPoint3D pnt)
{
	UGPoint2D pntlp;
	UGPoint pt(0, 0);
	// TODO:
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
	// TODO:
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

		m_pWorkspace->GetUGWorkspace()->m_SceneStorages.AttachWorkspace(m_pWorkspace->GetUGWorkspace());
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

UGLayer3D* SceneControl::AddLayerFromDataset(string datasourceName, string datasetName)
{
	UGString ugDatasetName, ugDatasourceName;
	ugDatasourceName.FromStd(datasourceName);
	ugDatasetName.FromStd(datasetName);
	UGDataSource* pDatasource = m_pWorkspace->GetUGWorkspace()->GetDataSource(ugDatasourceName);
	UGLayer3D* pLayer = NULL;
	if (pDatasource != NULL) 
	{
		UGDataset* pDataset = pDatasource->GetDataset(ugDatasetName);
		if (pDataset != NULL)
		{
			UGLayer3DType layerType = UGLayer3DType::l3dNone;
			UGDataset::DatasetType datasetType = pDataset->GetType();

			if (datasetType == UGDataset::DatasetType::Image || datasetType == UGDataset::DatasetType::ImageCollection ||
				datasetType == UGDataset::DatasetType::Grid  || datasetType == UGDataset::DatasetType::GridCollection)
			{
				layerType = UGLayer3DType::l3dDatasetImage;
			}
			else if(datasetType == UGDataset::DatasetType::Model)
			{
				layerType = UGLayer3DType::l3dDatasetModelPro;
			}
			else if (datasetType == UGDataset::DatasetType::MBGrid) // Volume 
			{
				layerType = UGLayer3DType::l3dDatasetVolume;
			}
			else if (datasetType == UGDataset::DatasetType::Point || datasetType == UGDataset::DatasetType::PointZ) // PointZ -> Point3D
			{
				if (((UGDatasetVector*)pDataset)->GetParentDataset() == NULL)
				{
					layerType = UGLayer3DType::l3dDatasetVectorPoint;
				}
				else
				{
					layerType = UGLayer3DType::l3dDatasetVector;
				}
				
			}
			else if (datasetType == UGDataset::DatasetType::Line || datasetType == UGDataset::DatasetType::LineZ ||
				     datasetType == UGDataset::DatasetType::Region || datasetType == UGDataset::DatasetType::RegionZ ||
				     datasetType == UGDataset::DatasetType::Network3D)
			{
				layerType = UGLayer3DType::l3dDatasetVectorLR;
			}
			else if (datasetType == UGDataset::DatasetType::Text || datasetType == UGDataset::DatasetType::Network ||
				     datasetType == UGDataset::DatasetType::LineM || datasetType == UGDataset::DatasetType::CAD )
			{
				layerType = UGLayer3DType::l3dDatasetVector;
			}

			pLayer = m_pUGSceneWnd->GetScene3D()->m_Layers.AddLayer(layerType, ugDatasetName, ugDatasetName);
		}
		else
		{
			Log::Error("Failed to add a dataset on scene. Not found dataset " + datasetName + " in the datasource named " + datasourceName);
		}
		
	}
	else 
	{
		Log::Error("Failed to add a dataset on scene. Not found datasource " + datasourceName);
	}
	return pLayer;
	
}

bool SceneControl::Save()
{	
	UGString sceneName = m_pUGSceneWnd->GetScene3D()->GetName();
	bool isSaved = false;
	if (mIsInWorkspace) {
		
		UGSceneStorage* pSceneStorage = m_pWorkspace->GetUGWorkspace()->m_SceneStorages.Find(sceneName);
		if(pSceneStorage != NULL)
		{
			pSceneStorage->SetXML(m_pUGSceneWnd->GetScene3D()->ToXML(), pSceneStorage->GetVersion());

			isSaved = true;
		}
		else 
		{
			string strSceneName;
			sceneName.ToStd(strSceneName);
			isSaved = SaveAs(strSceneName);
			mIsInWorkspace = isSaved;
		}
	}
	else
	{
		string strSceneName;
		sceneName.ToStd(strSceneName);
		isSaved = SaveAs(strSceneName);
		mIsInWorkspace = isSaved;
	}
	
	return isSaved;
	
}

bool SceneControl::SaveAs(string sceneName)
{
	UGString ugSceneName;
	ugSceneName.FromStd(sceneName);

	UGSceneStorages* pSStore = &(m_pWorkspace->GetUGWorkspace()->m_SceneStorages);
	
	UGString validSceneName = pSStore->GetUnoccupiedSceneName(ugSceneName);
	bool isAdd = pSStore->Add(validSceneName);
	int count = pSStore->GetCount();
	boolean isSaved = false;
	if (isAdd)
	{
		UGSceneStorage* pSceneStorage = pSStore->GetSceneAt(count - 1);
		pSceneStorage->SetXML(m_pUGSceneWnd->GetScene3D()->ToXML(), pSceneStorage->GetVersion());
		
		isSaved = true;
		mIsInWorkspace = true;
	}
	return isSaved;
}

void SceneControl::ActivateScene() {
	if (m_pRoot3D != NULL && m_pUGSceneWnd != NULL)
	{
		m_pRoot3D->SetActiveScene(m_pUGSceneWnd->GetScene3D());

	}
}
UGLayer3D* SceneControl::AddLayerFromFile(string filePath)
{
	UGString ugFilePath;
	ugFilePath.FromStd(filePath);
	UGString suffix = UGFile::GetExt(ugFilePath);
	UGString fileName = UGFile::GetName(ugFilePath);

	UGString layerName = fileName.Left(fileName.GetLength() - suffix.GetLength());

	while (m_pUGSceneWnd->GetScene3D()->m_Layers.FindNameInner(layerName) >= 0)
	{
		layerName = layerName + _U("_1");
	}

	UGLayer3D* pLayer = NULL;

	// Normal Layer
	if (0 == suffix.CompareNoCase(_U(".sci")) || 0 == suffix.CompareNoCase(_U(".sci3d")) ||
		0 == suffix.CompareNoCase(_U(".sit")) || 0 == suffix.CompareNoCase(_U(".tiff"))  || 
	    0 == suffix.CompareNoCase(_U(".tif")) || 0 == suffix.CompareNoCase(_U(".scv"))   ||
		0 == suffix.CompareNoCase(_U(".scp")) || 0 == suffix.CompareNoCase(_U(".scvo")))
	{
		pLayer = m_pUGSceneWnd->GetScene3D()->m_Layers.AddLayer(ugFilePath, layerName, layerName);
	}
	else
	{
		 Log::Error("Unsupported file ! " + filePath);
	}

	 return pLayer;
}

UGTerrainAccessor* SceneControl::AddTerrainLayerFromFile(string filePath)
{
	UGString ugFilePath;
	ugFilePath.FromStd(filePath);
	UGString suffix = UGFile::GetExt(ugFilePath);
	UGString fileName = UGFile::GetName(ugFilePath);

	UGTerrainAccessor* pTerrain = NULL;
	UGString layerName = fileName.Left(fileName.GetLength() - suffix.GetLength());

	// Terraint Layer
	if (suffix.CompareNoCase(_U(".sct")) == 0)
	{
		pTerrain = m_pUGSceneWnd->GetScene3D()->m_TerrainLayer3Ds.AddTerrainLayer(ugFilePath, layerName);
	}
	else 
	{
		Log::Error("Unsupported file ! " + filePath);
	}

	return pTerrain;
}

