/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.05.13
 */
#include "stdafx.h"
#include "MapFrm.h"
#include "MapLayersView.h"
#include "Resource.h"
#include "JunSuCpp.h"
#include "Data/UGStrConvertor.h"

class CClassViewMenuButton : public CMFCToolBarMenuButton
{
	friend class MapLayersView;

	DECLARE_SERIAL(CClassViewMenuButton)

public:
	CClassViewMenuButton(HMENU hMenu = nullptr) noexcept : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CClassViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MapLayersView::MapLayersView() noexcept
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;
	m_pMapControl = NULL;
	mSelectedTreeItem = NULL;
	m_pSceneControl = NULL;
}

MapLayersView::~MapLayersView()
{
	m_pMapControl = NULL;
	mSelectedTreeItem = NULL;
	m_pSceneControl = NULL;
}

BEGIN_MESSAGE_MAP(MapLayersView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_LAYER_EDITABLE, &MapLayersView::OnLayerEditable)
	ON_COMMAND(ID_LAYER_SELECTABLE, &MapLayersView::OnLayerSelectable)
	ON_COMMAND(ID_LAYER_VISIBLE, &MapLayersView::OnLayerVisible)
	ON_COMMAND(ID_LAYER_REMOVE, &MapLayersView::OnLayerRemove)
	ON_UPDATE_COMMAND_UI(ID_LAYER_EDITABLE, &MapLayersView::OnUpdate_LayerEditable)
	ON_UPDATE_COMMAND_UI(ID_LAYER_SELECTABLE, &MapLayersView::OnUpdate_LayerSelectable)
	ON_UPDATE_COMMAND_UI(ID_LAYER_VISIBLE, &MapLayersView::OnUpdate_LayerVisible)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView message handlers

int MapLayersView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create views:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndMapLayersView.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("Failed to create Class View\n");
		return -1;      // fail to create
	}

	// Load images:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SORT);

	m_wndToolBar.ReplaceButton(ID_SORT_MENU, CClassViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != nullptr)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
		pButton->SetMessageWnd(this);
	}

	// Fill in some static tree view data (dummy code, nothing magic here)
	FillClassView();

	return 0;
}

void MapLayersView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void MapLayersView::FillClassView()
{
	HTREEITEM hRoot = m_wndMapLayersView.InsertItem(_T("Layers"));
	m_wndMapLayersView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	m_wndMapLayersView.SetItemData(hRoot, ItemType::LAYERS);
	m_wndMapLayersView.Expand(hRoot, TVE_EXPAND);

}

void MapLayersView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndMapLayersView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

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
			DWORD_PTR itemData = pWndTree->GetItemData(hTreeItem);
			if (itemData == ItemType::LAYER) {
				resID = IDR_LIST_POPUP_LAYER;
			}
			mSelectedTreeItem = hTreeItem;
		}
	}

	pWndTree->SetFocus();
	if (resID == 0) {
		return;
	}
	CMenu menu;
	//menu.LoadMenu(IDR_POPUP_SORT);
	menu.LoadMenu(resID);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{

		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;
		

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
}

void MapLayersView::AdjustLayout()
{
	if (GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndMapLayersView.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL MapLayersView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}


void MapLayersView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndMapLayersView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void MapLayersView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndMapLayersView.SetFocus();
}

void MapLayersView::OnChangeVisualStyle()
{
	//m_ClassViewImages.DeleteImageList();

	//UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_MAPLAYERS_VIEW_24 : IDB_MAPLAYERS_VIEW;

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

	//m_ClassViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	//m_ClassViewImages.Add(&bmp, RGB(255, 0, 0));

	//m_wndMapLayersView.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);

	//m_wndToolBar.CleanUpLockedImages();
	//m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* Locked */);
}


void MapLayersView::SetMapControl(MapControl* pMapControl)
{
	m_pMapControl = pMapControl;
	this->OnLayersChanged();

}
void MapLayersView::OnLayersChanged()
{
	m_wndMapLayersView.DeleteAllItems();

	HTREEITEM hRoot = m_wndMapLayersView.InsertItem(_T("Layers"));
	m_wndMapLayersView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	m_wndMapLayersView.SetItemData(hRoot, ItemType::LAYERS);
	m_wndMapLayersView.Expand(hRoot, TVE_EXPAND);

	if (m_pMapControl != NULL)
	{
		UGLayers* pLayers = m_pMapControl->GetUGLayers();
		int layerCount = pLayers->GetTopLevelCount();
		for (int i = 0; i < layerCount; i++) 
		{
			UGLayer* pLayer = pLayers->GetLayerAt(i);

			UGString ugName = pLayer->GetName();
			string strName = UGStrConvertor::Tostring(ugName);
			CString cstrName = StrConvertor::stringTCString(strName);

			HTREEITEM item = m_wndMapLayersView.InsertItem(cstrName, hRoot);
			string layerType = typeid(*pLayer).name();
			string layerGroup = "UGLayerGroup";
			if (layerGroup.compare(layerType) == 0)
			{
				m_wndMapLayersView.SetItemData(item, ItemType::LAYERGROUP);
				ScanLayerGroupItems(item, (UGLayerGroup*) pLayer);
			}
			else
			{
				m_wndMapLayersView.SetItemData(item, ItemType::LAYER);
			}
		}
	}

	Invalidate();
}

void MapLayersView::ScanLayerGroupItems(HTREEITEM hParent, UGLayerGroup* pLayerGroup)
{
	int layerCount = pLayerGroup->GetCount();
	for (int i = 0; i < layerCount; i++)
	{
		UGLayer* pLayer = pLayerGroup->GetAt(i);

		UGString ugName = pLayer->GetName();
		string strName = UGStrConvertor::Tostring(ugName);
		CString cstrName = StrConvertor::stringTCString(strName);

		HTREEITEM item = m_wndMapLayersView.InsertItem(cstrName, hParent);
		string layerType = typeid(*pLayer).name();
		string layerGroup = "class UGLayerGroup";
		if (layerGroup.compare(layerType) == 0)
		{
			m_wndMapLayersView.SetItemData(item, ItemType::LAYERGROUP);
			ScanLayerGroupItems(item, (UGLayerGroup*)pLayer);
		}
		else
		{
			m_wndMapLayersView.SetItemData(item, ItemType::LAYER);
		}
	}
}


void MapLayersView::OnLayerEditable()
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndMapLayersView;
	if (mSelectedTreeItem != NULL)
	{

		CString layerName = pWndTree->GetItemText(mSelectedTreeItem);
		string strName = StrConvertor::CStringTstring(layerName);
		UGString ugName;
		ugName.FromStd(strName);
		UGLayer* pLayer = m_pMapControl->GetUGLayers()->GetLayer(ugName);

		bool isEditable = pLayer->IsEditable();

		pLayer->SetEditable(!isEditable);
		m_pMapControl->GetUGLayers()->SetEditableLayer(pLayer, !isEditable);
	}
}


void MapLayersView::OnLayerSelectable()
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndMapLayersView;
	if (mSelectedTreeItem != NULL)
	{

		CString layerName = pWndTree->GetItemText(mSelectedTreeItem);
		string strName = StrConvertor::CStringTstring(layerName);
		UGString ugName;
		ugName.FromStd(strName);
		UGLayer* pLayer = m_pMapControl->GetUGLayers()->GetLayer(ugName);
		pLayer->SetSelectable(!pLayer->IsSelectable());
	}
}


void MapLayersView::OnLayerVisible()
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndMapLayersView;
	if (mSelectedTreeItem != NULL)
	{

		CString layerName = pWndTree->GetItemText(mSelectedTreeItem);
		string strName = StrConvertor::CStringTstring(layerName);
		UGString ugName;
		ugName.FromStd(strName);
		UGLayer* pLayer = m_pMapControl->GetUGLayers()->GetLayer(ugName);
		pLayer->SetVisible(!pLayer->IsVisible());
		m_pMapControl->Refresh();
	}
}


void MapLayersView::OnLayerRemove()
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndMapLayersView;
	if (mSelectedTreeItem != NULL)
	{
		CString layerName = pWndTree->GetItemText(mSelectedTreeItem);
		string strName = StrConvertor::CStringTstring(layerName);
		UGString ugName;
		ugName.FromStd(strName);
		int index = m_pMapControl->GetUGLayers()->IndexOf(ugName);
		bool bRemoved = m_pMapControl->GetUGLayers()->RemoveAt(index);
		if (bRemoved) {
			pWndTree->DeleteItem(mSelectedTreeItem);
			mSelectedTreeItem = NULL;
			Invalidate();
			m_pMapControl->Refresh();
		}
		else
		{
			// TODO: ouput log
		}
	}
}

void MapLayersView::SetSceneControl(SceneControl* pSceneControl)
{
	m_pSceneControl = pSceneControl;
	this->OnSceneLayersChanged();

}

void MapLayersView::OnSceneLayersChanged()
{
	m_wndMapLayersView.DeleteAllItems();

	HTREEITEM hRoot = m_wndMapLayersView.InsertItem(_T("Layers"));
	m_wndMapLayersView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	m_wndMapLayersView.SetItemData(hRoot, ItemType::LAYERS);
	m_wndMapLayersView.Expand(hRoot, TVE_EXPAND);

	if (m_pSceneControl != NULL)
	{
		UGLayer3Ds* pLayers = m_pSceneControl->GetUGLayers();
		int layerCount = pLayers->GetInnerCount();
		for (int i = 0; i < layerCount; i++)
		{
			UGLayer3D* pLayer = pLayers->GetLayerInnerAt(i);

			UGString ugName = pLayer->GetName();
			string strName = UGStrConvertor::Tostring(ugName);
			CString cstrName = StrConvertor::stringTCString(strName);

			HTREEITEM item = m_wndMapLayersView.InsertItem(cstrName, hRoot);
			string layerType = typeid(*pLayer).name();
			string layerGroup = "UGLayerGroup";
			if (layerGroup.compare(layerType) == 0)
			{
				m_wndMapLayersView.SetItemData(item, ItemType::LAYERGROUP);
				//ScanSceneLayerGroupItems(item, (UGLayerGroup*)pLayer);
			}
			else
			{
				m_wndMapLayersView.SetItemData(item, ItemType::LAYER_3D);
			}
		}
	}

	Invalidate();
}

void MapLayersView::ScanSceneLayerGroupItems(HTREEITEM hParent, UGLayerGroup* pLayerGroup)
{
	int layerCount = pLayerGroup->GetCount();
	for (int i = 0; i < layerCount; i++)
	{
		UGLayer* pLayer = pLayerGroup->GetAt(i);

		UGString ugName = pLayer->GetName();
		string strName = UGStrConvertor::Tostring(ugName);
		CString cstrName = StrConvertor::stringTCString(strName);

		HTREEITEM item = m_wndMapLayersView.InsertItem(cstrName, hParent);
		string layerType = typeid(*pLayer).name();
		string layerGroup = "class UGLayerGroup";
		if (layerGroup.compare(layerType) == 0)
		{
			m_wndMapLayersView.SetItemData(item, ItemType::LAYERGROUP);
			ScanLayerGroupItems(item, (UGLayerGroup*)pLayer);
		}
		else
		{
			m_wndMapLayersView.SetItemData(item, ItemType::LAYER);
		}
	}
}

void MapLayersView::OnUpdate_LayerEditable(CCmdUI *pCmdUI)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndMapLayersView;
	if (mSelectedTreeItem != NULL)
	{

		CString layerName = pWndTree->GetItemText(mSelectedTreeItem);
		string strName = StrConvertor::CStringTstring(layerName);
		UGString ugName;
		ugName.FromStd(strName);
		UGLayer* pLayer = m_pMapControl->GetUGLayers()->GetLayer(ugName);

		if (pLayer->IsVisible())
		{
			UGDataset::DatasetType type = pLayer->GetDataset()->GetType();
			if (type == UGDataset::DatasetType::Point    ||
				type == UGDataset::DatasetType::Line     ||
				type == UGDataset::DatasetType::Region   ||
				type == UGDataset::DatasetType::CAD      ||
				type == UGDataset::DatasetType::Network)
			{
				pCmdUI->Enable(true);
				pCmdUI->SetCheck(pLayer->IsEditable());
			}
			else
			{
				pCmdUI->SetCheck(false);
				pCmdUI->Enable(false);
			}
		}
		else
		{
			pCmdUI->Enable(false);
		}
	}
}


void MapLayersView::OnUpdate_LayerSelectable(CCmdUI *pCmdUI)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndMapLayersView;
	if (mSelectedTreeItem != NULL)
	{

		CString layerName = pWndTree->GetItemText(mSelectedTreeItem);
		string strName = StrConvertor::CStringTstring(layerName);
		UGString ugName;
		ugName.FromStd(strName);
		UGLayer* pLayer = m_pMapControl->GetUGLayers()->GetLayer(ugName);

		if (pLayer->IsVisible()) 
		{
			UGDataset::DatasetType type = pLayer->GetDataset()->GetType();
			if (type == UGDataset::DatasetType::Point    ||
				type == UGDataset::DatasetType::Line     ||
				type == UGDataset::DatasetType::Region   ||
				type == UGDataset::DatasetType::CAD      ||
				type == UGDataset::DatasetType::Network)
			{
				pCmdUI->Enable(true);
				pCmdUI->SetCheck(pLayer->IsSelectable());
			}
			else
			{
				pCmdUI->SetCheck(false);
				pCmdUI->Enable(false);
			}
		}
		else
		{
			pCmdUI->Enable(false);
		}
	}
}


void MapLayersView::OnUpdate_LayerVisible(CCmdUI *pCmdUI)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndMapLayersView;
	if (mSelectedTreeItem != NULL)
	{

		CString layerName = pWndTree->GetItemText(mSelectedTreeItem);
		string strName = StrConvertor::CStringTstring(layerName);
		UGString ugName;
		ugName.FromStd(strName);
		UGLayer* pLayer = m_pMapControl->GetUGLayers()->GetLayer(ugName);

		pCmdUI->SetCheck(pLayer->IsVisible());
	}
}
