/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.05.13
 */
#include "stdafx.h"
#include "Mapfrm.h"
#include "WorkspaceView.h"
#include "Resource.h"
#include "JunSuCpp.h"
#include "Data/UGStrConvertor.h"
#include "MapView.h"
#include "JunSuMainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

WorkspaceView::WorkspaceView() noexcept
{
	m_pWorkspace = NULL;
	mSelectedTreeItem = NULL;
}

WorkspaceView::~WorkspaceView()
{
	m_pWorkspace = NULL;
}

BEGIN_MESSAGE_MAP(WorkspaceView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_MAP_OPEN, &WorkspaceView::OnMapOpen)
	ON_COMMAND(ID_MAP_DELETE, &WorkspaceView::OnMapDelete)
	ON_COMMAND(ID_DATASET_ADDTONEWMAP, &WorkspaceView::OnDatasetAddtoNewMap)
	ON_COMMAND(ID_DATASET_ADDTOMAP, &WorkspaceView::OnDatasetAddtoCurMap)
	ON_COMMAND(ID_SCENCE_OPEN, &WorkspaceView::OnScenceOpen)
	ON_COMMAND(ID_SCENCE_DELETE, &WorkspaceView::OnScenceDelete)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

int WorkspaceView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create view:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_workspaceTreeView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	// No Image(1/2)
	// Load view images:
	//m_FileViewImages.Create(IDB_WORKSPACE_VIEW, 16, 0, RGB(255, 0, 255));
	//m_workspaceTreeView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// Fill in some static tree view data (dummy code, nothing magic here)
	FillFileView();
	AdjustLayout();

	return 0;
}

void WorkspaceView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void WorkspaceView::FillFileView()
{
	HTREEITEM hRoot   = m_workspaceTreeView.InsertItem(_T("Workspace"));
	
	HTREEITEM hDatas  = m_workspaceTreeView.InsertItem(_T("Datasources"), hRoot);

	HTREEITEM hMaps   = m_workspaceTreeView.InsertItem(_T("Maps"), hRoot);

	HTREEITEM hScenes = m_workspaceTreeView.InsertItem(_T("Scenes"), hRoot);

	m_workspaceTreeView.SetItemData(hRoot, ItemType::WORKSPACE);
	m_workspaceTreeView.SetItemData(hDatas, ItemType::DATASOURCES);
	m_workspaceTreeView.SetItemData(hMaps, ItemType::MAPS);
	m_workspaceTreeView.SetItemData(hMaps, ItemType::SCENCES);
	
	m_workspaceTreeView.Expand(hRoot, TVE_EXPAND);
}

void WorkspaceView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_workspaceTreeView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}
	// 根据不同的类型提供相应的右键菜单
	UINT resID = 0;
	if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != nullptr)
		{
			pWndTree->SelectItem(hTreeItem);
			//pWndTree->GetItem
			DWORD_PTR itemData = pWndTree->GetItemData(hTreeItem);
			// 根据不同的类型提供相应的右键菜单
			if (itemData == ItemType::DATASET) 
			{
				resID = IDR_LIST_POPUP_DATASET;
			}
			else if (itemData == ItemType::DATASOURCE)
			{
				resID = IDR_LIST_POPUP_DATASOURCE;
			}
			else if (itemData == ItemType::MAP)
			{
				resID = IDR_LIST_POPUP_MAP;
			}
			else if (itemData == ItemType::SCENCE)
			{
				resID = IDR_LIST_POPUP_SCENCE;
			}

		}

		mSelectedTreeItem = hTreeItem;
	}

	pWndTree->SetFocus();
	if (resID != 0) 
	{
		::SendMessage(HWND_BROADCAST, JUN_WM_SCENCE_PAUSE, 0, 0);
		theApp.GetContextMenuManager()->ShowPopupMenu(resID, point.x, point.y, this, TRUE);
		::SendMessage(HWND_BROADCAST, JUN_WM_SCENCE_RESUME, 0, 0);
	}
	
}

void WorkspaceView::AdjustLayout()
{
	if (GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_workspaceTreeView.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void WorkspaceView::OnProperties()
{
	AfxMessageBox(_T("Properties...."));

}

void WorkspaceView::OnFileOpen()
{
	// TODO: Add your command handler code here
}

void WorkspaceView::OnFileOpenWith()
{
	// TODO: Add your command handler code here
}

void WorkspaceView::OnDummyCompile()
{
	// TODO: Add your command handler code here
}

void WorkspaceView::OnEditCut()
{
	// TODO: Add your command handler code here
}

void WorkspaceView::OnEditCopy()
{
	// TODO: Add your command handler code here
}

void WorkspaceView::OnEditClear()
{
	// TODO: Add your command handler code here
}

void WorkspaceView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_workspaceTreeView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void WorkspaceView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_workspaceTreeView.SetFocus();
}

void WorkspaceView::OnChangeVisualStyle()
{
	// No Image(2/2)
	//m_wndToolBar.CleanUpLockedImages();
	//m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* Locked */);

	//m_FileViewImages.DeleteImageList();

	//UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_WORKSPACE_VIEW_24 : IDB_WORKSPACE_VIEW;

	//CBitmap bmp;
	//if (!bmp.LoadBitmap(uiBmpId))
	//{
	//	TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
	//	ASSERT(FALSE);
	//	return;
	//}

	//BITMAP bmpObj;
	//bmp.GetBitmap(&bmpObj);

	//UINT nFlags = ILC_MASK;

	//nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	//m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	//m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	//m_workspaceTreeView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}

void WorkspaceView::SetWorkspace(Workspace* pWorkspace)
{
	m_pWorkspace = pWorkspace;

	bool bDeleted = m_workspaceTreeView.DeleteAllItems();
	
	HTREEITEM hRoot   = NULL;

	HTREEITEM hDatas  = NULL;

	HTREEITEM hMaps   = NULL;

	HTREEITEM hScenes = NULL;

	if (m_pWorkspace == NULL)
	{
		Invalidate();
		return;
	}
	else
	{
		UGString name = pWorkspace->GetUGWorkspace()->GetCaption();
		string  strName = UGStrConvertor::Tostring(name);
		CString cstrName = StrConvertor::stringTCString(strName);

		hRoot = m_workspaceTreeView.InsertItem(cstrName);

		hDatas = m_workspaceTreeView.InsertItem(_T("Datasources"),hRoot);

		hMaps = m_workspaceTreeView.InsertItem(_T("Maps"),hRoot);

		hScenes = m_workspaceTreeView.InsertItem(_T("Scenes"), hRoot);
		
		m_workspaceTreeView.SetItemData(hRoot, ItemType::WORKSPACE);
		m_workspaceTreeView.SetItemData(hDatas, ItemType::DATASOURCES);
		m_workspaceTreeView.SetItemData(hMaps, ItemType::MAPS);
		m_workspaceTreeView.SetItemData(hMaps, ItemType::SCENCES);
	}
	
	//HTREEITEM hRoot = m_wndWorkspaceView.GetRootItem();
	//HTREEITEM hDatas  = m_wndWorkspaceView.GetNextItem(hRoot, TVGN_NEXT);
	//HTREEITEM hMaps   = m_wndWorkspaceView.GetNextItem(hRoot, TVGN_NEXT);
	//HTREEITEM hScenes = m_wndWorkspaceView.GetNextItem(hRoot, TVGN_NEXT);

	

	//HTREEITEM subChild;
	//while (m_wndWorkspaceView.ItemHasChildren(hDatas))
	//{
	//	subChild = m_wndWorkspaceView.GetNextItem(hDatas, TVGN_NEXT);
	//	m_wndWorkspaceView.DeleteItem(subChild);
	//}
	//while (m_wndWorkspaceView.ItemHasChildren(hMaps))
	//{
	//	subChild = m_wndWorkspaceView.GetNextItem(hMaps, TVGN_NEXT);
	//	m_wndWorkspaceView.DeleteItem(subChild);
	//}
	//while (m_wndWorkspaceView.ItemHasChildren(hScenes))
	//{
	//	subChild = m_wndWorkspaceView.GetNextItem(hScenes, TVGN_NEXT);
	//	m_wndWorkspaceView.DeleteItem(subChild);
	//}
	//subChild = NULL;


	UGWorkspace* pUGWorkspace = pWorkspace->GetUGWorkspace();
	int datasourceCount = pUGWorkspace->m_DataSources.GetCount();

	for (int i = 0; i < datasourceCount; i++) {
		UGString ugName1 ;
		UGDataSource *pDatasource;
		bool iExist = pUGWorkspace->m_DataSources.GetAt(i, ugName1, pDatasource);

		string strName1 = UGStrConvertor::Tostring(ugName1);
		CString cstrName1 = StrConvertor::stringTCString(strName1);


		HTREEITEM hDatasource = m_workspaceTreeView.InsertItem(cstrName1, hDatas);
		m_workspaceTreeView.SetItemData(hDatasource, ItemType::DATASOURCE);

		int datasetCount = pDatasource->GetDatasetCount();
		for(int j=0; j<datasetCount; j++)
		{
			UGString ugName2 = pDatasource->GetDatasets()->GetAt(j)->GetName();
			string strName2 = UGStrConvertor::Tostring(ugName2);
			CString cstrName2 = StrConvertor::stringTCString(strName2);

			HTREEITEM hDataset = m_workspaceTreeView.InsertItem(cstrName2,hDatasource);
			m_workspaceTreeView.SetItemData(hDataset, ItemType::DATASET);
		}
	}

	int mapCount = pUGWorkspace->m_MapStorages.GetCount();

	for (int i = 0; i < mapCount; i++) {
		UGString ugName = pUGWorkspace->m_MapStorages.GetNameAt(i);
		string strName = UGStrConvertor::Tostring(ugName);
		CString cstrName = StrConvertor::stringTCString(strName);

		HTREEITEM hMap = m_workspaceTreeView.InsertItem(cstrName, hMaps);
		m_workspaceTreeView.SetItemData(hMap, ItemType::MAP);
	}

	int sceneCount = pUGWorkspace->m_SceneStorages.GetCount();

	for (int i = 0; i < sceneCount; i++) {
		UGString ugName = pUGWorkspace->m_SceneStorages.GetNameAt(i);
		string strName = UGStrConvertor::Tostring(ugName);
		CString cstrName = StrConvertor::stringTCString(strName);

		HTREEITEM hScence = m_workspaceTreeView.InsertItem(cstrName, hScenes);
		m_workspaceTreeView.SetItemData(hScence, ItemType::SCENCE);
	}
	m_workspaceTreeView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	m_workspaceTreeView.Expand(hRoot, TVE_EXPAND);
	
	Invalidate();
}



// 没有添加事件的右键菜单，显示为灰色


/************************ Dataset Menu *****************************/

void WorkspaceView::OnDatasetAddtoNewMap()
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_workspaceTreeView;
	if (mSelectedTreeItem != NULL)
	{
		CString datasetCName = pWndTree->GetItemText(mSelectedTreeItem);
		CString datasourceCName = pWndTree->GetItemText(pWndTree->GetParentItem(mSelectedTreeItem));
		string datasetStrName = StrConvertor::CStringTstring(datasetCName);
		string datasourceStrName = StrConvertor::CStringTstring(datasourceCName);

		JunSuCpp* junApp = (JunSuCpp*)AfxGetApp();
		MapFrame* mapWnd = junApp->CreateMapWnd(datasetCName);

		MapView* mapView = (MapView*)mapWnd->GetActiveView();
		MapControl* mapControl = mapView->GetMapControl();

		mapControl->SetWorkspace(m_pWorkspace); 
		mapControl->AddDataset(datasourceStrName, datasetStrName);

		((JunSuMainFrame*)AfxGetMainWnd())->UpdateLayersView();
	}
}


void WorkspaceView::OnDatasetAddtoCurMap()
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_workspaceTreeView;
	if (mSelectedTreeItem != NULL)
	{
		JunSuCpp* junApp = (JunSuCpp*)AfxGetApp();
		CWnd* activeWnd = ((JunSuMainFrame*)AfxGetMainWnd())->MDIGetActive();
		if (activeWnd == NULL) {
			OnDatasetAddtoNewMap();
			return;
		}

		CString datasetCName = pWndTree->GetItemText(mSelectedTreeItem);
		CString datasourceCName = pWndTree->GetItemText(pWndTree->GetParentItem(mSelectedTreeItem));
		string datasetStrName = StrConvertor::CStringTstring(datasetCName);
		string datasourceStrName = StrConvertor::CStringTstring(datasourceCName);

		MapFrame* mapWnd = (MapFrame*)activeWnd;
		MapView* mapView = (MapView*)mapWnd->GetActiveView();
		MapControl* mapControl = mapView->GetMapControl();

		mapControl->SetWorkspace(m_pWorkspace);
		mapControl->AddDataset(datasourceStrName, datasetStrName);

		((JunSuMainFrame*)AfxGetMainWnd())->UpdateLayersView();
	}
}

/************************ End Dataset Menu *****************************/



/************************** Map Menu *********************************/
void WorkspaceView::OnMapOpen()
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_workspaceTreeView;
	if (mSelectedTreeItem != NULL)
	{
		CString mapName = pWndTree->GetItemText(mSelectedTreeItem);
		JunSuCpp* junApp = (JunSuCpp*)AfxGetApp();
		MapFrame* mapWnd = junApp->CreateMapWnd(mapName);
		
		MapView* mapView = (MapView*) mapWnd->GetActiveView();
		MapControl* mapControl = mapView->GetMapControl();

		mapControl->SetWorkspace(m_pWorkspace);
		bool isOpen = mapControl->OpenMap(StrConvertor::CStringTstring(mapName));
		if (!isOpen)
		{
			//TODO: ouput log
		}

		((JunSuMainFrame*)AfxGetMainWnd())->UpdateLayersView();
	}
	
}

void WorkspaceView::OnMapDelete()
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_workspaceTreeView;
	if (mSelectedTreeItem != NULL)
	{
		CString mapName = pWndTree->GetItemText(mSelectedTreeItem);
		string strName = StrConvertor::CStringTstring(mapName);
		bool bRemoved = m_pWorkspace->RemoveMap(strName);
		if (bRemoved) {
			pWndTree->DeleteItem(mSelectedTreeItem);
			mSelectedTreeItem = NULL;
			Invalidate();
		}
		else 
		{
			// TODO: ouput log
		}
	}
}

/************************ End Map Menu *****************************/



/************************ Scence Menu *****************************/

void WorkspaceView::OnScenceOpen()
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_workspaceTreeView;
	if (mSelectedTreeItem != NULL)
	{
		CString title = pWndTree->GetItemText(mSelectedTreeItem);
		JunSuCpp* junApp = (JunSuCpp*)AfxGetApp();
		SceneFrame* wnd = junApp->CreateSceneWnd(title);

		SceneView* sceneView = (SceneView*)wnd->GetActiveView();
		SceneControl* sceneControl = sceneView->GetSceneControl();

		sceneControl->SetWorkspace(m_pWorkspace);
		bool isOpen = sceneControl->OpenScene(StrConvertor::CStringTstring(title));
		if (!isOpen)
		{
			//TODO: ouput log
		}

		//((JunSuMainFrame*)AfxGetMainWnd())->UpdateLayersView();
		::SendMessage(AfxGetMainWnd()->m_hWnd, JUN_WM_REFRESH_LAYERSVIEW, JUN_WM_PARAM_SCENELAYERS, 0);
	}
}

void WorkspaceView::OnScenceDelete()
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_workspaceTreeView;
	if (mSelectedTreeItem != NULL)
	{
		CString mapName = pWndTree->GetItemText(mSelectedTreeItem);
		string strName = StrConvertor::CStringTstring(mapName);
		bool bRemoved = m_pWorkspace->RemoveScene(strName);
		if (bRemoved) {
			pWndTree->DeleteItem(mSelectedTreeItem);
			mSelectedTreeItem = NULL;
			Invalidate();
		}
		else
		{
			// TODO: ouput log
		}
	}
}

/************************ End Scence Menu *****************************/

void WorkspaceView::OnWorkspaceChanged()
{
	this->SetWorkspace(m_pWorkspace);
}
