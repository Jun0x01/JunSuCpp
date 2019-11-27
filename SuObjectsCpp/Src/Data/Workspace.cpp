/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.05.19
 */
#include "Data/Workspace.h"
using namespace SuperMap;

Workspace::Workspace()
{
	m_pUGWorkspace = new UGWorkspace();
}

Workspace::~Workspace()
{
	m_pUGWorkspace->Close();
	delete m_pUGWorkspace;
	m_pUGWorkspace = NULL;
}

UGWorkspace* Workspace::GetUGWorkspace()
{
	return m_pUGWorkspace;
}

bool Workspace::OpenWorkspaceFile(const string &filePath, const string &password /* = "" */)
{
	// Close last opened workspace
	m_pUGWorkspace->Close();

	// Convert string to UGString
	UGString ug_filePath;
	ug_filePath.FromStd(filePath);  
	UGString ug_password;
	ug_password.FromStd(password);

	UGWorkspaceConnection wkCon;
	wkCon.m_strServer = ug_filePath;
	wkCon.m_strPassword = ug_password;

	// Check workspace's type according to the file's suffix
	int index = filePath.rfind('.');
	string wkType = filePath.substr(index + 1, filePath.length() - index);

	if (wkType.compare("smwu") == 0 || wkType.compare("SMWU") == 0)
	{
		wkCon.m_nWorkspaceType = UGWorkspace::WS_Version_SMWU;
	}
	else if (wkType.compare("smw") == 0 || wkType.compare("SMW") == 0)
	{
		wkCon.m_nWorkspaceType = UGWorkspace::WS_Version_SMW;
	}
	else if (wkType.compare("sxwu") == 0 || wkType.compare("SXWU") == 0)
	{
		wkCon.m_nWorkspaceType = UGWorkspace::WS_Version_SXWU;
	}
	else if (wkType.compare("sxw") == 0 || wkType.compare("SXW") == 0)
	{
		wkCon.m_nWorkspaceType = UGWorkspace::WS_Version_SXW;
	}

	// Open the workspace file
	if (m_pUGWorkspace->Open(wkCon))
	{
		//Fixed that failed save workpace file.
		m_pUGWorkspace->m_WorkspaceConnection.m_bFailIfExists = false;
		int count = m_pUGWorkspace->m_DataSources.GetCount();
		if (0 == count)
		{
			string name;
			m_pUGWorkspace->GetCaption().ToStd(name);
			Log::Warning("Worksapce: " + name + ", it donesn't contain any datasource or failed to open datasources in workspace.");
		}
		return true;
	}
	else
	{
		Log::Error("Can not open the workspace: " + filePath);
		return false;
	}
}

bool Workspace::OpenWorkspaceDB(UGWorkspace::UGWorkspaceType type, const string &serverUrl, const string &dbName, const string &userName, const string &password, const string &workspaceName)
{
	// Close last opened workspace
	m_pUGWorkspace->Close();

	// Convert string to UGString
	UGString ug_serverUrl;
	UGString ug_dbName;
	UGString ug_userName;
	UGString ug_password;
	UGString ug_workspaceName;
	

	ug_serverUrl.FromStd(serverUrl);
	ug_dbName.FromStd(dbName);
	ug_userName.FromStd(userName);
	ug_password.FromStd(password);
	ug_workspaceName.FromStd(workspaceName);

	UGWorkspaceConnection wkCon;
	wkCon.m_nWorkspaceType = type;
	wkCon.m_strServer = ug_serverUrl;
	wkCon.m_strDatabase = ug_dbName;
	wkCon.m_strUser = ug_userName;
	wkCon.m_strPassword = ug_password;
	wkCon.m_strWorkspaceName = ug_workspaceName;
	if (UGWorkspace::/*UGWorkspaceType::*/WS_Version_Sql == type)
	{
		wkCon.m_strDriver = _U("SQL Server");
	}

	// Open the workspace file
	if (m_pUGWorkspace->Open(wkCon))
	{
		int count = m_pUGWorkspace->m_DataSources.GetCount();
		if (0 == count)
		{
			Log::Warning("Worksapce: " + workspaceName + ", it donesn't contain any datasource or failed to open datasources in workspace.");
		}
		return true;
	}
	else
	{
		Log::Error("Can not open the workspace: " + workspaceName + ", please check you parameters or db server again.");
		return false;
	}
}

UGDataSource* Workspace::OpenDatasourceFile(UGEngineType type, const string &name, const string &filePath, const string &password /*= ""*/, bool readOnly /*= false*/)
{
	// Convert string to UGString
	UGString ug_name;
	UGString ug_filePath;
	UGString ug_password;

	ug_name.FromStd(name);
	ug_filePath.FromStd(filePath);
	ug_password.FromStd(password);

	UGDsConnection dcon;

	dcon.m_nType = type;
	dcon.m_strAlias = ug_name;
	dcon.m_strServer = ug_filePath;
	dcon.m_bReadOnly = readOnly;

	return m_pUGWorkspace->OpenDataSource(dcon);
}

UGDataSource* Workspace::OpenDatasourceDB(UGEngineType type, const string &name, const string &serverUrl, const string &dbName, const string &userName, const string &password, bool readOnly /*= false*/)
{
	// Convert string to UGString
	UGString ug_name;
	UGString ug_serverUrl;
	UGString ug_dbName;
	UGString ug_userName;
	UGString ug_password;

	ug_name.FromStd(name);
	ug_serverUrl.FromStd(serverUrl);
	ug_dbName.FromStd(dbName);
	ug_userName.FromStd(userName);
	ug_password.FromStd(password);

	UGDsConnection dcon;

	dcon.m_nType = type;
	dcon.m_strAlias = ug_name;
	dcon.m_strServer = ug_serverUrl;
	dcon.m_strDatabase = ug_dbName;
	dcon.m_strUser = ug_userName;
	dcon.m_strPassword = ug_password;
	dcon.m_bReadOnly = readOnly;

    if (UGC::/*UGEngineType::*/SQLPlus == type)
	{
		dcon.m_strDriver = _U("SQL Server");
	}
	return m_pUGWorkspace->OpenDataSource(dcon);

}

UGDataSource* Workspace::OpenDatasourceWeb(UGEngineType type, const string &name, const string &serverUrl, const string &OGCType /*= "" */, const string &serverKey /*= "" */)
{
	// Convert string to UGString
	UGString ug_name;
	UGString ug_serverUrl;
	UGString ug_OGCType;
	UGString ug_serverKey;

	ug_name.FromStd(name);
	ug_serverUrl.FromStd(serverUrl);
	ug_OGCType.FromStd(OGCType);
	ug_serverKey.FromStd(serverKey);

	UGDsConnection dcon;

	dcon.m_nType = type;
	dcon.m_strAlias = ug_name;
	dcon.m_strServer = ug_serverUrl;
	dcon.m_strDriver = ug_OGCType;
	dcon.m_strPassword = ug_serverKey;

	return m_pUGWorkspace->OpenDataSource(dcon);
}

void Workspace::CloseDatasource(int index)
{
	m_pUGWorkspace->m_DataSources.ReleaseAt(index);
}

void Workspace::CloseDatasource(const string &name)
{
	// Convert string to UGString
	UGString ug_name;
	ug_name.FromStd(name);
	m_pUGWorkspace->m_DataSources.ReleaseAt(ug_name);
}

bool Workspace::RemoveMap(const string &mapName)
{
	UGString ugMapName;
	ugMapName.FromStd(mapName);
	return m_pUGWorkspace->m_MapStorages.Remove(ugMapName);
}

bool Workspace::RemoveScene(const string &sceneName)
{
	UGString ugSceneName;
	ugSceneName.FromStd(sceneName);
	return m_pUGWorkspace->m_SceneStorages.Remove(ugSceneName);
}

bool Workspace::Save()
{
	return m_pUGWorkspace->Save();
}

bool Workspace::SaveAsFile(const string &wkPath)
{
	UGString ugWkPath;
	ugWkPath.FromStd(wkPath);
	if (ugWkPath.Compare(m_pUGWorkspace->m_WorkspaceConnection.m_strServer) == 0) 
	{
		// same workspace file path
		return Save();
	}
	else
	{
		UGWorkspaceConnection wkCon;
		wkCon = m_pUGWorkspace->m_WorkspaceConnection;

		wkCon.m_strServer = ugWkPath;

		// 1.Check workspace's type according to the file's suffix
		int index = wkPath.rfind('.');
		string wkType = wkPath.substr(index + 1, wkPath.length() - index);

		if (wkType.compare("smwu") == 0 || wkType.compare("SMWU") == 0)
		{
			wkCon.m_nWorkspaceType = UGWorkspace::WS_Version_SMWU;
		}
		else if (wkType.compare("smw") == 0 || wkType.compare("SMW") == 0)
		{
			wkCon.m_nWorkspaceType = UGWorkspace::WS_Version_SMW;
		}
		else if (wkType.compare("sxwu") == 0 || wkType.compare("SXWU") == 0)
		{
			wkCon.m_nWorkspaceType = UGWorkspace::WS_Version_SXWU;
		}
		else if (wkType.compare("sxw") == 0 || wkType.compare("SXW") == 0)
		{
			wkCon.m_nWorkspaceType = UGWorkspace::WS_Version_SXW;
		}
		// 2.Set workspace version
		if (wkCon.m_nVersion == 0) {
			wkCon.m_nVersion = UG_WORKSPACE_VERSION_20120328;
		}
		return m_pUGWorkspace->SaveAs(m_pUGWorkspace->m_WorkspaceConnection);

	}
}

UGDataSource* Workspace::CreateDatasourceUDB(const string &udbPath, const string &name)
{
	UGString ugUDBpath;
	ugUDBpath.FromStd(udbPath);
	UGString ugName;
	ugName.FromStd(name);

    UGDataSource* pDatasource = UGDataSourceManager::CreateDataSource(UGC::/*UGEngineType::*/UDB);;
	UGDsConnection& cn = pDatasource->GetConnectionInfo();
	cn.m_strServer = ugUDBpath;
    cn.m_nType = UGC::/*UGEngineType::*/UDB;
	cn.m_strAlias = ugName;
	cn.m_bReadOnly = false;
	//cn.m_bExclusive = true;     // 默认为false. 该参数与m_bReadOnly相反，使用其一即可。即只有只读和独占两种状态。
	//cn.m_bAutoConnect = false;  // 默认为true, 使用默认值。若设置成false，会导致保存到工作空间后，再次打开工作空间时，该数据源打开失败

	bool isCreated = pDatasource->Create();

	if (isCreated)
	{
		m_pUGWorkspace->m_DataSources.Insert(ugName, pDatasource); // 将新建数据源和工作空间关联，以便保存到文件。

	}
	else {
		delete pDatasource;
		pDatasource = NULL;
		Log::Error("Failed to created udb，the path is \"" + udbPath + "\"");
	}

	return pDatasource;
}
