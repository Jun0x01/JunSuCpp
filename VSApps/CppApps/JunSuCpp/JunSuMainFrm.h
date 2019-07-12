
// JunSuMainFrm.h : interface of the JunSuMainFrame class
/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.05.13
 */

#pragma once
#include "WorkspaceView.h"
#include "MapLayersView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"

class JunSuMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(JunSuMainFrame)
public:
	JunSuMainFrame() noexcept;

// Attributes
public:

// Operations
public:
	WorkspaceView* GetWorkspaceView();
	MapLayersView* GetMapLayersView();

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);

// Implementation
public:
	virtual ~JunSuMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	WorkspaceView     m_workspaceTreeView;
	MapLayersView     m_wndMapLayersView;
	COutputWnd        m_wndOutput;
	CPropertiesWnd    m_wndProperties;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnActivateTabChanged(WPARAM, LPARAM);

	afx_msg LRESULT OnRefreshWorkspaceView(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRefreshLayersView(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRefreshMapView(WPARAM, LPARAM);
	afx_msg LRESULT OnOutputLog(WPARAM, LPARAM pCStr);
	
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);

public:
	// 更新图层列表
	void UpdateLayersView();
	void UpdateLayersView(MapControl* pMapControl);

	void UpdateSceneLayersView();
	void UpdateSceneLayersView(SceneControl* pSceneControl);
};


