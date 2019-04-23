
// JunSuMFCDialogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JunSuMFCDialog.h"
#include "JunSuMFCDialogDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
void SuCALLBACK InvalidateCallBack(void* pWnd)
{

	// send message to view  for invalidating itself
	//SendDlgItemMessageW(, this, 0x01, NULL,NULL);

	JunSuMFCDialogDlg *pView = (JunSuMFCDialogDlg *)pWnd;
	pView->Invalidate(false); // not redraw background
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// JunSuMFCDialogDlg dialog



JunSuMFCDialogDlg::JunSuMFCDialogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(JunSuMFCDialogDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pMapControl = new MapControl(InvalidateCallBack, this);

	m_pLastEditLayer = NULL;
}

void JunSuMFCDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(JunSuMFCDialogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_OPEN32771, &JunSuMFCDialogDlg::OnFileOpen)
	ON_COMMAND(ID_DRAW_DRAWPOINT, &JunSuMFCDialogDlg::OnDrawPoint)
	ON_COMMAND(ID_DRAW_DRAWLINE32774, &JunSuMFCDialogDlg::OnDrawLine)
	ON_COMMAND(ID_DRAW_DRAWREGION, &JunSuMFCDialogDlg::OnDrawRegion)
	ON_COMMAND(ID_MEASURE_MEASURELENGTH, &JunSuMFCDialogDlg::OnMeasureLength)
	ON_COMMAND(ID_MEASURE_MEASUREAREA, &JunSuMFCDialogDlg::OnMeasureAea)
	ON_COMMAND(ID_FILE_CLOSE32772, &JunSuMFCDialogDlg::OnFileClose)
	ON_COMMAND(ID_EDIT_PANMAP, &JunSuMFCDialogDlg::OnPanMap)
END_MESSAGE_MAP()


// JunSuMFCDialogDlg message handlers

BOOL JunSuMFCDialogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	mMenu.LoadMenu(IDR_MENU);
	SetMenu(&mMenu);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void JunSuMFCDialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void JunSuMFCDialogDlg::OnPaint()
{

	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(&rect);
	m_pMapControl->OnDraw(rect.left, rect.top, rect.right, rect.bottom, dc.m_hDC);

	CDialogEx::OnPaint();

}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR JunSuMFCDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void JunSuMFCDialogDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//CPaintDC dc(this); 

	//m_pMapControl->OnMouseMove(dc.m_hDC, nFlags, point.x, point.y);
	m_pMapControl->OnMouseMove(nFlags, point.x, point.y, ::GetDC(this->m_hWnd));
	CDialogEx::OnMouseMove(nFlags, point);
}


BOOL JunSuMFCDialogDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	ScreenToClient(&pt);
	m_pMapControl->OnMouseWheel(nFlags, zDelta, pt.x, pt.y);
	//	Invalidate(false);
		//return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
	return true;
}


void JunSuMFCDialogDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//CPaintDC dc(this); 
	//m_pMapControl->OnMouseLDown(dc.m_hDC, nFlags, point.x, point.y);
	m_pMapControl->OnLMouseDown(nFlags, point.x, point.y, ::GetDC(this->m_hWnd));
	CDialogEx::OnLButtonDown(nFlags, point);
}


void JunSuMFCDialogDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
// 	CPaintDC dc(this); 
// 	m_pMapControl->OnMouseLUp(dc.m_hDC, nFlags, point.x, point.y);
	m_pMapControl->OnLMouseUp(nFlags, point.x, point.y, ::GetDC(this->m_hWnd));
	CDialogEx::OnLButtonUp(nFlags, point);
	//this->Invalidate(FALSE);
}


void JunSuMFCDialogDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_pMapControl->OnLMouseDbClick(nFlags, point.x, point.y, ::GetDC(this->m_hWnd));
	CDialogEx::OnLButtonDblClk(nFlags, point);
}


void JunSuMFCDialogDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_pMapControl->OnRMouseDown(nFlags, point.x, point.y, ::GetDC(this->m_hWnd));
	CDialogEx::OnRButtonDown(nFlags, point);
}


void JunSuMFCDialogDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_pMapControl->OnRMouseUp(nFlags, point.x, point.y, ::GetDC(this->m_hWnd));
	CDialogEx::OnRButtonUp(nFlags, point);
}


void JunSuMFCDialogDlg::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnRButtonDblClk(nFlags, point);
}


void JunSuMFCDialogDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	m_pMapControl->OnSizeChanged(cx, cy, ::GetDC(this->m_hWnd));
}




// Menu event : File
void JunSuMFCDialogDlg::OnFileOpen()
{
	CFileDialog fileDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.smwu)|*.smwu"), NULL);
	if (fileDialog.DoModal())
	{
		CString filePath = fileDialog.GetPathName();
		string strPath = StrConvertor::CStringTstring(filePath);

		OpenWorkspace(strPath);
	}
	else 
	{

	}
}

void JunSuMFCDialogDlg::OnFileClose()
{
	CloseWorkspace();
}

// Menu event : Draw
void JunSuMFCDialogDlg::OnDrawPoint()
{
	UGMapEditorWnd* pMapEditorWnd = m_pMapControl->GetMapEditWnd();
	UGMap* pUGMap = &(pMapEditorWnd->m_mapWnd.m_Map);
	UGString layerName = _U("DrawPoint@World");
	UGLayer* pLayer = m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map.m_Layers.GetLayer(layerName);
	if (pLayer != NULL)
	{
		ResetLastEditLayer(pLayer);
		pLayer->SetSelectable(true);

		UGLayers* pLayers = &(pUGMap->m_Layers);
		pLayers->SetEditableLayer(pLayer, true);

		int oldAction = pMapEditorWnd->m_mapWnd.m_Map.GetDrawing()->GetDrawParamaters()->GetUserAction();
		int oldEditAction = pMapEditorWnd->GetEditToolPack()->GetUserAction();
		pMapEditorWnd->SetUserAction(UGC::UGDrawParamaters::uaEdit, UGC::UGEditType::ET_Point);
	
	}
	else
	{

	}
}


void JunSuMFCDialogDlg::OnDrawLine()
{
	UGMapEditorWnd* pMapEditorWnd = m_pMapControl->GetMapEditWnd();
	UGMap* pUGMap = &(pMapEditorWnd->m_mapWnd.m_Map);
	UGString layerName = _U("DrawLine@World");
	UGLayer* pLayer = pUGMap->m_Layers.GetLayer(layerName);
	if (pLayer != NULL)
	{
		ResetLastEditLayer(pLayer);
		pLayer->SetSelectable(true);
	
		UGLayers* pLayers = &(pUGMap->m_Layers);
		pLayers->SetEditableLayer(pLayer, true);

		pMapEditorWnd->SetUserAction(UGC::UGDrawParamaters::uaEdit, UGC::UGEditType::ET_PolyLine);
	}
	else
	{

	}
}


void JunSuMFCDialogDlg::OnDrawRegion()
{
	UGMapEditorWnd* pMapEditorWnd = m_pMapControl->GetMapEditWnd();
	UGMap* pUGMap = &(pMapEditorWnd->m_mapWnd.m_Map);

	UGString layerName = _U("DrawRegion@World");
	UGLayer* pLayer = pUGMap->m_Layers.GetLayer(layerName);
	if (pLayer != NULL)
	{
		ResetLastEditLayer(pLayer);

		pLayer->SetSelectable(true);
		
		UGLayers* pLayers = &(pUGMap->m_Layers);
		pLayers->SetEditableLayer(pLayer, true);

		pMapEditorWnd->SetUserAction(UGC::UGDrawParamaters::uaEdit, UGC::UGEditType::ET_PolyGon);
	}
	else
	{

	}
}


// Menu event : Measure
void JunSuMFCDialogDlg::OnMeasureLength()
{
	// TODO: Add your command handler code here
}


void JunSuMFCDialogDlg::OnMeasureAea()
{
	// TODO: Add your command handler code here
}

void JunSuMFCDialogDlg::OpenWorkspace(string strPath)
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
void JunSuMFCDialogDlg::CloseWorkspace()
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

void JunSuMFCDialogDlg::OnPanMap()
{
	ResetLastEditLayer(NULL);
	Pan();
}

void JunSuMFCDialogDlg::Pan()
{
	UGMapEditorWnd* pMapEditorWnd = m_pMapControl->GetMapEditWnd();
	pMapEditorWnd->SetUserAction(UGC::UGDrawParamaters::uaPan);
}

void JunSuMFCDialogDlg::ResetLastEditLayer(UGLayer* pLayer)
{
	if (m_pLastEditLayer != NULL)
	{
		UGLayers* pLayers = &(m_pMapControl->GetMapEditWnd()->m_mapWnd.m_Map.m_Layers);
		pLayers->SetEditableLayer(m_pLastEditLayer, false);
	}
	m_pLastEditLayer = pLayer;
}