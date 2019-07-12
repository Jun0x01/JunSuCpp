/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.05.13
 */
#pragma once

#include "ViewTree.h"
#include "Data/Workspace.h"

using namespace SuperMap;

class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class WorkspaceView : public CDockablePane
{
	enum ItemType {
		WORKSPACE,
		DATASOURCES,
		MAPS,
		SCENCES,
		DATASOURCE,
		DATASET,
		MAP,
		SCENCE

	};
// Construction
public:
	WorkspaceView() noexcept;

	void AdjustLayout();
	void OnChangeVisualStyle();

// Attributes
protected:

	CViewTree m_workspaceTreeView;
	CImageList m_FileViewImages;
	CFileViewToolBar m_wndToolBar;
	CFileViewToolBar m_wndToolBar_Datasources;
	CFileViewToolBar m_wndToolBar_Maps;
	CFileViewToolBar m_wndToolBar_Scence;
	CFileViewToolBar m_wndToolBar_Dataset;

protected:
	void FillFileView();

// Implementation
public:
	virtual ~WorkspaceView();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenWith();
	afx_msg void OnDummyCompile();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()

private:
	Workspace* m_pWorkspace;
	HTREEITEM mSelectedTreeItem;

public:
	void SetWorkspace(Workspace* pWorkspace);
	afx_msg void OnMapOpen();
	afx_msg void OnMapDelete();


	afx_msg void OnDatasetAddtoNewMap();
	afx_msg void OnDatasetAddtoCurMap();
	afx_msg void OnScenceOpen();
	afx_msg void OnScenceDelete();

	void OnWorkspaceChanged();
	
};

