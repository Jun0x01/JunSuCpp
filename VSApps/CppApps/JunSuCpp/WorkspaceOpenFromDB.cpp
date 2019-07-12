// WorkspaceOpenFromDB.cpp : implementation file
//

#include "stdafx.h"
#include "JunSuCpp.h"
#include "WorkspaceOpenFromDB.h"
#include "afxdialogex.h"
#include "DBType.h"


// WorkspaceOpenFromDB dialog

IMPLEMENT_DYNAMIC(WorkspaceOpenFromDB, CDialogEx)

WorkspaceOpenFromDB::WorkspaceOpenFromDB(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_OpenWK_DB, pParent)
	, mText_DB_Type(_T(""))
	, mText_DB_Host(_T(""))
	, mText_DB_Port(_T(""))
	, mText_DB_Name(_T(""))
	, mText_DB_User_Name(_T(""))
	, mText_DB_User_Password(_T(""))
	, mText_DB_WorkspaceName(_T(""))
{
	m_pWorkspace = NULL;
}

WorkspaceOpenFromDB::~WorkspaceOpenFromDB()
{
}

void WorkspaceOpenFromDB::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COMBO_DB_Type, mText_DB_Type);
	DDX_Text(pDX, IDC_EDIT_DB_Host, mText_DB_Host);
	DDX_Text(pDX, IDC_EDIT_DB_Name, mText_DB_Name);
	DDX_Text(pDX, IDC_EDIT_UserName, mText_DB_User_Name);
	DDX_Text(pDX, IDC_EDIT_UserPassword, mText_DB_User_Password);
	DDX_Text(pDX, IDC_EDIT_DB_WKName, mText_DB_WorkspaceName);
}


BEGIN_MESSAGE_MAP(WorkspaceOpenFromDB, CDialogEx)
	ON_BN_CLICKED(IDOK, &WorkspaceOpenFromDB::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &WorkspaceOpenFromDB::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_DB_Type, &WorkspaceOpenFromDB::OnSelChange_DB_Type)
END_MESSAGE_MAP()


BOOL WorkspaceOpenFromDB::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CComboBox* db_type_box = (CComboBox*)GetDlgItem(IDC_COMBO_DB_Type);
	db_type_box->AddString(DB_SQLPLUS);
	db_type_box->AddString(DB_ORACLEPLUS);
	db_type_box->AddString(DB_POSTGRESQL);
	db_type_box->AddString(DB_MYSQL);
	db_type_box->AddString(DB_DMPLUS);
	db_type_box->AddString(DB_MONGODB);
	db_type_box->AddString(DB_POSTGIS);

	CWnd* pDs =  GetDlgItem(IDC_Text_WK_DS_Name);
	
	if (mDialogType == DialogType::OPEN_WK) {
		pDs->SetWindowTextW(_T("Workspace Name"));
		this->SetWindowTextW(_T("Open Workspace from Database"));
	}
	else if (mDialogType == DialogType::OPEN_DS)
	{
		pDs->SetWindowTextW(_T("Datasource Name"));
		this->SetWindowTextW(_T("Open Datasource from Database"));
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

// WorkspaceOpenFromDB message handlers


void WorkspaceOpenFromDB::OnBnClickedOk()
{
	if (m_pWorkspace != NULL)
	{
		this->GetDlgItem(IDC_COMBO_DB_Type)->GetWindowTextW(mText_DB_Type);
		this->GetDlgItem(IDC_EDIT_DB_Host)->GetWindowTextW(mText_DB_Host);
		this->GetDlgItem(IDC_EDIT_DB_Port)->GetWindowTextW(mText_DB_Port);
		this->GetDlgItem(IDC_EDIT_DB_Name)->GetWindowTextW(mText_DB_Name);
		this->GetDlgItem(IDC_EDIT_UserName)->GetWindowTextW(mText_DB_User_Name);
		this->GetDlgItem(IDC_EDIT_UserPassword)->GetWindowTextW(mText_DB_User_Password);
		this->GetDlgItem(IDC_EDIT_DB_WKName)->GetWindowTextW(mText_DB_WorkspaceName);

		string db_type = StrConvertor::CStringTstring(mText_DB_Type);
		string db_host = StrConvertor::CStringTstring(mText_DB_Host);
		string db_port = StrConvertor::CStringTstring(mText_DB_Port);
		string db_name = StrConvertor::CStringTstring(mText_DB_Name);
		string db_user_name = StrConvertor::CStringTstring(mText_DB_User_Name);
		string db_user_password = StrConvertor::CStringTstring(mText_DB_User_Password);
		string show_name = StrConvertor::CStringTstring(mText_DB_WorkspaceName);

		UGC::UGWorkspace::UGWorkspaceType ugWkType = UGWorkspace::UGWorkspaceType::WS_Version_NO;
		UGC::UGEngineType engineType = UGEngineType::NOEngine;

		if (mText_DB_Type.Compare(DB_SQLPLUS) == 0)
		{
			ugWkType = UGWorkspace::UGWorkspaceType::WS_Version_Sql;
			engineType = UGEngineType::SQLPlus;
		}
		else if (mText_DB_Type.Compare(DB_ORACLEPLUS) == 0)
		{
			ugWkType = UGWorkspace::UGWorkspaceType::WS_Version_Oracle;
			engineType = UGEngineType::OraclePlus;
		}
		else if (mText_DB_Type.Compare(DB_POSTGRESQL) == 0)
		{
			ugWkType = UGWorkspace::UGWorkspaceType::WS_Version_PGSQL;
			engineType = UGEngineType::PostgreSQL;
		}
		else if (mText_DB_Type.Compare(DB_MYSQL) == 0)
		{
			ugWkType = UGWorkspace::UGWorkspaceType::WS_Version_MYSQL;
			engineType = UGEngineType::MySQL;
		}
		else if (mText_DB_Type.Compare(DB_DMPLUS) == 0)
		{
			ugWkType = UGWorkspace::UGWorkspaceType::WS_Version_DM;
			engineType = UGEngineType::DMPlus;
		}
		else if (mText_DB_Type.Compare(DB_MONGODB) == 0)
		{
			ugWkType = UGWorkspace::UGWorkspaceType::WS_Version_Mongo;
			engineType = UGEngineType::MongoDB;
		}
		else if (mText_DB_Type.Compare(DB_POSTGIS) == 0)
		{
			ugWkType = UGWorkspace::UGWorkspaceType::WS_Version_PGIS;
			engineType = UGEngineType::PostgreSQLGis;
		}

		CString log = _T("");
		if (mText_DB_Type.IsEmpty()) {
			log = _T("Warning: Choose a type of database");
			::SendMessage(AfxGetMainWnd()->m_hWnd, JUN_WM_OUTPUT_LOG, 0, (LPARAM)&log);
			return;
		}

		bool isOpen = false;
		string serverUrl = db_host;// +":" + db_port;
		if (mDialogType == DialogType::OPEN_WK) {

			if (ugWkType != UGWorkspace::UGWorkspaceType::WS_Version_NO)
			{
				
				isOpen = m_pWorkspace->OpenWorkspaceDB(ugWkType, serverUrl, db_name, db_user_name, db_user_password, show_name);

				log = _T("Open a workspace from DB: " + isOpen);

				::SendMessage(AfxGetMainWnd()->m_hWnd, JUN_WM_OUTPUT_LOG, 0, (LPARAM)&log);

				if (isOpen)
				{
					::SendMessage(AfxGetMainWnd()->m_hWnd, JUN_WM_REFRESH_WORKSPACEVIEW, 0, 0);
				}
				else
				{
					log = StrConvertor::stringTCString("Server: " + serverUrl + ", DBName: " + db_name + ", DB_WorkspaceName: " + show_name);
					::SendMessage(AfxGetMainWnd()->m_hWnd, JUN_WM_OUTPUT_LOG, 0, (LPARAM)&log);
				}
			}
		}
		else if (mDialogType == DialogType::OPEN_DS)
		{
			if (engineType != UGEngineType::NOEngine)
			{
				isOpen = m_pWorkspace->OpenDatasourceDB(engineType, show_name, serverUrl, db_name, db_user_name, db_user_password);

				log = _T("Open a datasource from DB: " + isOpen);

				::SendMessage(AfxGetMainWnd()->m_hWnd, JUN_WM_OUTPUT_LOG, 0, (LPARAM)&log);

				if (isOpen)
				{
					::SendMessage(AfxGetMainWnd()->m_hWnd, JUN_WM_REFRESH_WORKSPACEVIEW, 0, 0);
				}
				else
				{
					log = StrConvertor::stringTCString("Server: " + serverUrl + ", DBName: " + db_name + ", DB_DatasourceName: " + show_name);
					::SendMessage(AfxGetMainWnd()->m_hWnd, JUN_WM_OUTPUT_LOG, 0, (LPARAM)&log);
				}
			}
		}

		if (isOpen) {
			CDialogEx::OnOK();
		}
		
	}
	

}


void WorkspaceOpenFromDB::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}


void WorkspaceOpenFromDB::OnSelChange_DB_Type()
{
	CComboBox* db_type_box = (CComboBox*)GetDlgItem(IDC_COMBO_DB_Type);
	int index = db_type_box->GetCurSel();
	CString strOld;
	db_type_box->GetWindowTextW(strOld);

	CString strCur;
	db_type_box->GetLBText(index, strCur);

	if (index >= 0) {
		cout << index << endl;
	}
}

void WorkspaceOpenFromDB::SetWorkspace(Workspace* pWorkspace)
{
	m_pWorkspace = pWorkspace;
}

void WorkspaceOpenFromDB::SetDialogType(DialogType type)
{
	mDialogType = type;
}
