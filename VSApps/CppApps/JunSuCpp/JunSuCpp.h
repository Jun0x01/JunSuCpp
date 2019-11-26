
// JunSuCpp.h : main header file for the JunSuCpp application
/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.05.13
 */
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "StrConvertor.h"
#include "Data/Workspace.h"
#include "Data/LicenseManager.h"
#include "MapFrm.h"
#include "SceneFrame.h"
#include "SceneView.h"
#include "GeometryPlot/UGGOLibraryManager.h"

using namespace SuperMap;

// JunSuCpp:
// See JunSuCpp.cpp for the implementation of this class
//

class JunSuCpp : public CWinAppEx
{
public:
	JunSuCpp() noexcept;
	MapFrame* CreateMapWnd(CString mapName);
	SceneFrame* CreateSceneWnd(CString sceneName);

private:

	CMultiDocTemplate* m_pMultiDocTemplate;
	CMultiDocTemplate* m_pMultiTemplateSceneFrm;

	Workspace* m_pWorkspace;

	bool mIsLicenseValid;

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	//afx_msg void OnFileClose();
	afx_msg void OnAppExit();
	
	afx_msg void OnWorkspace_OpenFromDatabase();
	afx_msg void OnDatasource_OpenFromWeb();
	afx_msg void OnDatasource_OpenFromDatabase();
	afx_msg void OnAnalyst_NetworkAnalyst();
	afx_msg void OnBuildData_BuildNetworkDataset();
	afx_msg void OnEdit_DrawPoint();
	afx_msg void OnEdit_DrawLine();
	afx_msg void OnEdit_DrawRegion();
	afx_msg void OnEdit_SelectGeoObject();
	afx_msg void OnEdit_EditNodes();
	afx_msg void OnEdit_Pan();
	afx_msg void OnEdit_Addnodes();
};

extern JunSuCpp theApp;
