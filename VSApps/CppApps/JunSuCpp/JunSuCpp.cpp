
// JunSuCpp.cpp : Defines the class behaviors for the application.
/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.05.13
 */

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "JunSuCpp.h"
#include "JunSuMainFrm.h"

#include "JunSuCppDoc.h"
#include "MapView.h"
#include "WorkspaceOpenFromDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// JunSuCpp

BEGIN_MESSAGE_MAP(JunSuCpp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &JunSuCpp::OnAppAbout)
	// Standard file based document commands
	//ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_FILE_NEW, &JunSuCpp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &JunSuCpp::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &JunSuCpp::OnFileSave)
	ON_COMMAND(ID_APP_EXIT, &JunSuCpp::OnAppExit)
	ON_COMMAND(ID_WORKSPACE_OPENFROMDATABASE, &JunSuCpp::OnWorkspace_OpenFromDatabase)
	ON_COMMAND(ID_DATASOURCE_OPENWEB, &JunSuCpp::OnDatasource_OpenFromWeb)
	ON_COMMAND(ID_DATASOURCE_OPENFROMDATABASE, &JunSuCpp::OnDatasource_OpenFromDatabase)
	ON_COMMAND(ID_ANALYST_NETWORKANALYST, &JunSuCpp::OnAnalyst_NetworkAnalyst)
	ON_COMMAND(ID_DATA_BUILDNETWORK, &JunSuCpp::OnBuildData_BuildNetworkDataset)
	ON_COMMAND(ID_EDIT_DRAWPOINT, &JunSuCpp::OnEdit_DrawPoint)
	ON_COMMAND(ID_EDIT_DRAWLINE, &JunSuCpp::OnEdit_DrawLine)
	ON_COMMAND(ID_EDIT_DRAWREGION, &JunSuCpp::OnEdit_DrawRegion)
	ON_COMMAND(ID_EDIT_SELECT, &JunSuCpp::OnEdit_SelectGeoObject)
	ON_COMMAND(ID_EDIT_EDITNODES, &JunSuCpp::OnEdit_EditNodes)
	ON_COMMAND(ID_EDIT_PAN, &JunSuCpp::OnEdit_Pan)
	ON_COMMAND(ID_EDIT_ADDNODES, &JunSuCpp::OnEdit_Addnodes)
END_MESSAGE_MAP()


// JunSuCpp construction

JunSuCpp::JunSuCpp() noexcept
{
	m_bHiColorIcons = TRUE;

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("JunSuCpp.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

	mIsLicenseValid = LicenseManager::IsValidStd();
	if (!mIsLicenseValid) {
		// TODO: output log
	}
	else {
		m_pWorkspace = new Workspace();
	}

	// 
	string name = "name";
	UGString ugName;
	ugName.FromStd(name);


}

// The one and only JunSuCpp object

JunSuCpp theApp;


// JunSuCpp initialization

BOOL JunSuCpp::InitInstance()
{
	CWinAppEx::InitInstance();

	// Only one window in taskbar preview
	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_MAINFRAME,  // Always us main frame
		RUNTIME_CLASS(JunSuCppDoc),
		RUNTIME_CLASS(MapFrame), // custom MDI child frame
		RUNTIME_CLASS(MapView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	m_pMultiDocTemplate = pDocTemplate;

	m_pMultiTemplateSceneFrm = new CMultiDocTemplate(IDR_MAINFRAME,  // Always us main frame
		RUNTIME_CLASS(JunSuCppDoc),
		RUNTIME_CLASS(SceneFrame), // custom MDI child frame
		RUNTIME_CLASS(SceneView));

	if (!m_pMultiTemplateSceneFrm) {
		AddDocTemplate(m_pMultiTemplateSceneFrm);
	}
	

	// create main MDI Frame window
	JunSuMainFrame* pMainFrame = new JunSuMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// Not new file when starting
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	if (m_pWorkspace != NULL)
	{
		((JunSuMainFrame *)m_pMainWnd)->GetWorkspaceView()->SetWorkspace(m_pWorkspace);
	}

	UGString strJYLibPath = _U("..\\..\\..\\TestData\\Plot\\JY.plot");
	UGString strTYLibPath = _U("..\\..\\..\\TestData\\Plot\\TY.plot");
	
	UGGOLibraryManager* pLibManager = UGGOLibraryManager::GetInstance();
	if (NULL != pLibManager)
	{
		//添加标号库
		UGint nJYLibId = pLibManager->AddGOLibrary(strJYLibPath);
		UGint nTYLibId = pLibManager->AddGOLibrary(strTYLibPath);
	}
	
	return TRUE;
}

int JunSuCpp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	return CWinAppEx::ExitInstance();
}

// JunSuCpp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void JunSuCpp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// JunSuCpp customization load/save methods

void JunSuCpp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
	GetContextMenuManager()->AddMenu(_T("Dataset"), IDR_LIST_POPUP_DATASET);
	GetContextMenuManager()->AddMenu(_T("Datasource"), IDR_LIST_POPUP_DATASOURCE);
	GetContextMenuManager()->AddMenu(_T("Map"), IDR_LIST_POPUP_MAP);
	GetContextMenuManager()->AddMenu(_T("Scence"), IDR_LIST_POPUP_SCENCE);
	GetContextMenuManager()->AddMenu(_T("Layer"), IDR_LIST_POPUP_LAYER);
	GetContextMenuManager()->AddMenu(_T("MapView"), IDR_POPUP_MAPVIEW);

}

void JunSuCpp::LoadCustomState()
{
}

void JunSuCpp::SaveCustomState()
{
}

// JunSuCpp message handlers





void JunSuCpp::OnFileNew()
{
	//MapFrame* pChildWnd = new MapFrame();
	//CDocument* pDoc = m_pMultiDocTemplate->CreateNewDocument();
	//pDoc->SetTitle(_T("Map"));
	//CFrameWnd* pW = m_pMultiDocTemplate->CreateNewFrame(NULL, NULL);
	//pW->SetTitle(_T("Map"));
	//m_pMultiDocTemplate->InitialUpdateFrame(pW, NULL);
	
	CFrameWnd* pW = m_pMultiTemplateSceneFrm->CreateNewFrame(NULL, NULL);
	pW->SetTitle(_T("Scene"));
	m_pMultiTemplateSceneFrm->InitialUpdateFrame(pW, NULL);

	((SceneView *)pW->GetActiveView())->GetSceneControl()->SetWorkspace(m_pWorkspace);
}


void JunSuCpp::OnFileOpen()
{
	// TODO: Add your command handler code here

	CFileDialog fileDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("工作空间 (*.smwu, *.sxwu)|*.smwu;*.sxwu|数据源(*.udb, *.scp, *.dwg)|*.udb;*.scp;*.dwg|All(*.*)|*.*||"), NULL);
	if (fileDialog.DoModal())
	{
		CString suffix = fileDialog.GetFileExt();
		CString filePath = fileDialog.GetPathName();
		string strPath = StrConvertor::CStringTstring(filePath);

		if (m_pWorkspace == NULL)
		{
			m_pWorkspace = new Workspace();
		}

		// Workspace
		if (0 == suffix.CompareNoCase(_T("smwu")) || 0 == suffix.CompareNoCase(_T("sxwu")))
		{
			bool isOpen = m_pWorkspace->OpenWorkspaceFile(strPath);


			//UGDataSource* pDatasource = m_pWorkspace->OpenDatasourceDB(UGEngineType::DMPlus, "DM", "10.10.2.101", NULL, "SYSDBA", "123456");
			if (!isOpen)
			{

			}
			else
			{
				((JunSuMainFrame *)m_pMainWnd)->GetWorkspaceView()->SetWorkspace(m_pWorkspace);
			}
		}
		else if ( 0 == suffix.CompareNoCase(_T("udb")) )
		{
			CString fileName = fileDialog.GetFileName();
			string strFileName = StrConvertor::CStringTstring(fileName);
			UGDataSource* pDatasource = m_pWorkspace->OpenDatasourceFile(UGEngineType::UDB, strFileName, strPath);
			if(pDatasource != NULL)
			{
				((JunSuMainFrame*)AfxGetMainWnd())->GetWorkspaceView()->OnWorkspaceChanged();
			}

		}
		else if (0 == suffix.CompareNoCase(_T("dwg")))
		{
			CString fileName = fileDialog.GetFileName();
			string strFileName = StrConvertor::CStringTstring(fileName);
			UGDataSource* pDatasource = m_pWorkspace->OpenDatasourceFile(UGEngineType::AutoCAD, strFileName, strPath);
			if (pDatasource != NULL)
			{
				((JunSuMainFrame*)AfxGetMainWnd())->GetWorkspaceView()->OnWorkspaceChanged();
			}

		}
		else //if (0 == suffix.CompareNoCase(_T("scp")))
		{
			CWnd* pWnd = ((JunSuMainFrame*)AfxGetMainWnd())->MDIGetActive();
			SceneView* pSceneView = NULL;
			if (pWnd != NULL) 
			{
				string targetName = typeid(SceneFrame).name();
				string wndName = typeid(*pWnd).name();
				if (0 == targetName.compare(wndName))
				{
					pSceneView = (SceneView *)((SceneFrame*)pWnd)->GetActiveView();
				}
				else
				{
					CString fileName = fileDialog.GetFileName();
					pSceneView = (SceneView *)this->CreateSceneWnd(fileName)->GetActiveView();
				}
			}
			else 
			{
				CString fileName = fileDialog.GetFileName();
				pSceneView = (SceneView *)this->CreateSceneWnd(fileName)->GetActiveView();
			}

			if (pSceneView != NULL)
			{
				UGString ugPath;
				ugPath.FromStd(strPath);
				UGSceneEditorWnd* pSceneWnd = pSceneView->GetSceneControl()->GetSceneEditWnd();
				//UGLayer3D* pLayer = pSceneView->GetSceneControl()->GetSceneEditWnd()->GetScene3D()->m_Layers.AddLayer(ugPath);
				UGLayer3D* pLayer = pSceneView->GetSceneControl()->AddLayerFromFile(strPath);
				if (pLayer != NULL)
				{
					pSceneWnd->GetScene3D()->GetCamera(_U("Camera"))->FlyToBounds(pLayer->GetBounds(), 1000);
				}
				::SendMessage(AfxGetMainWnd()->m_hWnd, JUN_WM_REFRESH_LAYERSVIEW, JUN_WM_PARAM_SCENELAYERS, 0);
			}
			
		}
		
	}
	else
	{

	}

}


void JunSuCpp::OnAppExit()
{
	// TODO: Add your command handler code here

	if (m_pWorkspace != NULL)
	{
		m_pWorkspace->GetUGWorkspace()->Close();
		delete m_pWorkspace;
		m_pWorkspace = NULL;
	}
}


MapFrame* JunSuCpp::CreateMapWnd(CString mapName)
{
	CFrameWnd* pW = m_pMultiDocTemplate->CreateNewFrame(NULL, NULL);
	pW->SetTitle(mapName);
	m_pMultiDocTemplate->InitialUpdateFrame(pW, NULL);
	return (MapFrame*) pW;
}

SceneFrame* JunSuCpp::CreateSceneWnd(CString sceneName)
{
	CFrameWnd* pW = m_pMultiTemplateSceneFrm->CreateNewFrame(NULL, NULL);
	pW->SetTitle(sceneName);
	m_pMultiTemplateSceneFrm->InitialUpdateFrame(pW, NULL);
	return (SceneFrame*)pW;
}

void JunSuCpp::OnFileSave() 
{
	if (m_pWorkspace != NULL)
	{
		bool isSaved = m_pWorkspace->Save();

	}
}


void JunSuCpp::OnWorkspace_OpenFromDatabase()
{
	WorkspaceOpenFromDB dialogOpenFromDB;
	dialogOpenFromDB.SetWorkspace(m_pWorkspace);
	dialogOpenFromDB.SetDialogType(WorkspaceOpenFromDB::DialogType::OPEN_WK);
	dialogOpenFromDB.DoModal();
	dialogOpenFromDB.SetWorkspace(NULL);
}


void JunSuCpp::OnDatasource_OpenFromWeb()
{
	// TODO: Add your command handler code here
}


void JunSuCpp::OnDatasource_OpenFromDatabase()
{
	WorkspaceOpenFromDB dialogOpenFromDB;
	dialogOpenFromDB.SetWorkspace(m_pWorkspace);
	dialogOpenFromDB.SetDialogType(WorkspaceOpenFromDB::DialogType::OPEN_DS);
	dialogOpenFromDB.DoModal();
	dialogOpenFromDB.SetWorkspace(NULL);
}


void JunSuCpp::OnAnalyst_NetworkAnalyst()
{
	// TODO: Add your command handler code here
}


void JunSuCpp::OnBuildData_BuildNetworkDataset()
{
	// TODO: Add your command handler code here
}


void JunSuCpp::OnEdit_DrawPoint()
{
	CWnd* pWnd = ((JunSuMainFrame*)AfxGetMainWnd())->MDIGetActive();
	if (NULL != pWnd) 
	{
		string targetName = typeid(MapFrame).name();
		string wndName = typeid(*pWnd).name();
		if (0 == targetName.compare(wndName))
		{
			UGMapEditorWnd* pMapWnd = ((MapView*)((MapFrame*)pWnd)->GetActiveView())->GetMapControl()->GetMapEditWnd();
			UGLayer* pLayer = pMapWnd->m_mapWnd.m_Map.GetCurrentLayer();
			if (pLayer == NULL) {
				int count = pMapWnd->m_mapWnd.m_Map.m_Layers.GetEditableLayer().GetCount();
				if (count > 0)
				{
					pLayer = pMapWnd->m_mapWnd.m_Map.m_Layers.GetEditableLayer().GetHead();
				}
			}
			if (pLayer != NULL)
			{
				UGDataset::DatasetType type = pLayer->GetDataset()->GetType();
				if (type == UGDataset::DatasetType::Point || type == UGDataset::DatasetType::CAD || type == UGDataset::DatasetType::Network)
				{
					pMapWnd->SetUserAction(UGC::UGDrawParamaters::uaEdit, UGC::UGEditType::ET_Point);
				}
				else 
				{
					// TODO: output log
				}
			}
		}
		else 
		{
			// TODO: output log
		}
	}
	else 
	{
		// TODO: output log
	}
}


void JunSuCpp::OnEdit_DrawLine()
{
	CWnd* pWnd = ((JunSuMainFrame*)AfxGetMainWnd())->MDIGetActive();
	if (NULL != pWnd)
	{
		string targetName = typeid(MapFrame).name();
		string wndName = typeid(*pWnd).name();
		if (0 == targetName.compare(wndName))
		{
			UGMapEditorWnd* pMapWnd = ((MapView*)((MapFrame*)pWnd)->GetActiveView())->GetMapControl()->GetMapEditWnd();
			UGLayer* pLayer = pMapWnd->m_mapWnd.m_Map.GetCurrentLayer();
			if (pLayer == NULL) {
				int count = pMapWnd->m_mapWnd.m_Map.m_Layers.GetEditableLayer().GetCount();
				if (count > 0)
				{
					pLayer = pMapWnd->m_mapWnd.m_Map.m_Layers.GetEditableLayer().GetHead();
				}
			}
			if (pLayer != NULL)
			{
				UGDataset::DatasetType type = pLayer->GetDataset()->GetType();
				if (type == UGDataset::DatasetType::Line || type == UGDataset::DatasetType::CAD || type == UGDataset::DatasetType::Network)
				{
					pMapWnd->SetUserAction(UGC::UGDrawParamaters::uaEdit, UGC::UGEditType::ET_PolyLine);
				}
				else
				{
					// TODO: output log
				}
			}
		}
		else
		{
			CString log = _T("Open a map");
			::SendMessage(AfxGetMainWnd()->m_hWnd, JUN_WM_OUTPUT_LOG, 0, (LPARAM)&log);

		}
	}
	else
	{
		CString log = _T("Open a map");
		::SendMessage(AfxGetMainWnd()->m_hWnd, JUN_WM_OUTPUT_LOG, 0, (LPARAM)&log);
	}
}


void JunSuCpp::OnEdit_DrawRegion()
{
	CWnd* pWnd = ((JunSuMainFrame*)AfxGetMainWnd())->MDIGetActive();
	if (NULL != pWnd)
	{
		string targetName = typeid(MapFrame).name();
		string wndName = typeid(*pWnd).name();
		if (0 == targetName.compare(wndName))
		{
			UGMapEditorWnd* pMapWnd = ((MapView*)((MapFrame*)pWnd)->GetActiveView())->GetMapControl()->GetMapEditWnd();
			UGLayer* pLayer = pMapWnd->m_mapWnd.m_Map.GetCurrentLayer();
			if (pLayer == NULL) {
				int count = pMapWnd->m_mapWnd.m_Map.m_Layers.GetEditableLayer().GetCount();
				if (count > 0) 
				{
					pLayer = pMapWnd->m_mapWnd.m_Map.m_Layers.GetEditableLayer().GetHead();
				}
			}
			if (pLayer != NULL)
			{
				UGDataset::DatasetType type = pLayer->GetDataset()->GetType();
				if (type == UGDataset::DatasetType::Region || type == UGDataset::DatasetType::CAD)
				{
					pMapWnd->SetUserAction(UGC::UGDrawParamaters::uaEdit, UGC::UGEditType::ET_PolyGon);
				}
				else
				{
					// TODO: output log
				}
			}
		}
		else
		{
			// TODO: output log
		}
	}
	else
	{
		// TODO: output log
	}
}


void JunSuCpp::OnEdit_Pan()
{
	CWnd* pWnd = ((JunSuMainFrame*)AfxGetMainWnd())->MDIGetActive();
	if (NULL != pWnd)
	{
		string targetName = typeid(MapFrame).name();
		string wndName = typeid(*pWnd).name();
		if (0 == targetName.compare(wndName))
		{
			UGMapEditorWnd* pMapWnd = ((MapView*)((MapFrame*)pWnd)->GetActiveView())->GetMapControl()->GetMapEditWnd();
			pMapWnd->SetUserAction(UGDrawParamaters::UGMapUserAction::uaPan);
		}
		else
		{
			// TODO: output log
		}
	}
	else
	{
		// TODO: output log
	}
}

void JunSuCpp::OnEdit_SelectGeoObject()
{
	CWnd* pWnd = ((JunSuMainFrame*)AfxGetMainWnd())->MDIGetActive();
	if (NULL != pWnd)
	{
		string targetName = typeid(MapFrame).name();
		string wndName = typeid(*pWnd).name();
		if (0 == targetName.compare(wndName))
		{
			UGMapEditorWnd* pMapWnd = ((MapView*)((MapFrame*)pWnd)->GetActiveView())->GetMapControl()->GetMapEditWnd();
			
			pMapWnd->SetSelectionMode(UGMapWnd::smContainCentroid);
			pMapWnd->SetUserAction(UGDrawParamaters::UGMapUserAction::uaPointModeSelect);
		}
		else
		{
			// TODO: output log
		}
	}
	else
	{
		// TODO: output log
	}

	
}


void JunSuCpp::OnEdit_EditNodes()
{
	CWnd* pWnd = ((JunSuMainFrame*)AfxGetMainWnd())->MDIGetActive();
	if (NULL != pWnd)
	{
		string targetName = typeid(MapFrame).name();
		string wndName = typeid(*pWnd).name();
		if (0 == targetName.compare(wndName))
		{
			UGMapEditorWnd* pMapWnd = ((MapView*)((MapFrame*)pWnd)->GetActiveView())->GetMapControl()->GetMapEditWnd();
			UGLayer* pLayer = pMapWnd->m_mapWnd.m_Map.GetCurrentLayer();
			if (pLayer != NULL) // 没选中对象，pLayer为NULL, 需先选中一个对象
			{
				pMapWnd->GetEditToolPack()->SetAccessibilityActionPointMode(UGC::UGEditType::EAA_EditPoint);
			}
			else
			{
				// TODO: output log
			}
			
		}
		else
		{
			// TODO: output log
		}
	}
	else
	{
		// TODO: output log
	}
}




void JunSuCpp::OnEdit_Addnodes()
{
	CWnd* pWnd = ((JunSuMainFrame*)AfxGetMainWnd())->MDIGetActive();
	if (NULL != pWnd)
	{
		string targetName = typeid(MapFrame).name();
		string wndName = typeid(*pWnd).name();
		if (0 == targetName.compare(wndName))
		{
			UGMapEditorWnd* pMapWnd = ((MapView*)((MapFrame*)pWnd)->GetActiveView())->GetMapControl()->GetMapEditWnd();
			UGLayer* pLayer = pMapWnd->m_mapWnd.m_Map.GetCurrentLayer();
			if (pLayer != NULL) // 没选中对象，pLayer为NULL，需先选中一个对象
			{
				pMapWnd->GetEditToolPack()->SetAccessibilityActionPointMode(UGC::UGEditType::EAA_AddPoint);
			}
			else
			{
				// TODO: output log
			}

		}
		else
		{
			// TODO: output log
		}
	}
	else
	{
		// TODO: output log
	}
}
