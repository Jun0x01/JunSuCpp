/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.05.13
 */
#pragma once

#include "ViewTree.h"
#include "Mapping/MapControl.h"
#include "Map/UGLayerGroup.h"
#include "Mapping/SceneControl.h"

using namespace SuperMap;

class CClassToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class MapLayersView : public CDockablePane
{
	enum ItemType
	{
		LAYERS,
		LAYERGROUP,
		LAYER,

		LAYER_3D,
		LAYERS_3D_NORMAL
	};
public:
	MapLayersView() noexcept;
	virtual ~MapLayersView();

	void AdjustLayout();
	void OnChangeVisualStyle();

protected:
	CClassToolBar m_wndToolBar;
	CViewTree m_wndMapLayersView;
	CImageList m_ClassViewImages;
	UINT m_nCurrSort;

	void FillClassView();

// Overrides
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	DECLARE_MESSAGE_MAP()

private:
	MapControl* m_pMapControl;

	void ScanLayerGroupItems(HTREEITEM hParent, UGLayerGroup* pLayerGroup);

	HTREEITEM mSelectedTreeItem;

	SceneControl* m_pSceneControl;

	void ScanSceneLayerGroupItems(HTREEITEM hParent, UGLayerGroup* pLayerGroup);

public:
	void SetMapControl(MapControl* pMapControl);
	void OnLayersChanged();

	void SetSceneControl(SceneControl* pSceneControl);
	void OnSceneLayersChanged();


	afx_msg void OnLayerEditable();
	afx_msg void OnLayerSelectable();
	afx_msg void OnLayerVisible();
	afx_msg void OnLayerRemove();

	afx_msg void OnUpdate_LayerEditable(CCmdUI *pCmdUI);
	afx_msg void OnUpdate_LayerSelectable(CCmdUI *pCmdUI);
	afx_msg void OnUpdate_LayerVisible(CCmdUI *pCmdUI);
};

