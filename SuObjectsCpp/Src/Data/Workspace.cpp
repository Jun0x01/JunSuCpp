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

bool Workspace::OpenWorkspaceFile(string filePath, string password /* = "" */)
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

bool Workspace::OpenWorkspaceDB(UGWorkspace::UGWorkspaceType type, string serverUrl, string dbName, string userName, string password, string workspaceName)
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

UGDataSource* Workspace::OpenDatasourceFile(UGEngineType type, string name, string filePath, string password /*= NULL*/, bool readOnly /*= false*/)
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

UGDataSource* Workspace::OpenDatasourceDB(UGEngineType type, string name, string serverUrl, string dbName, string userName, string password, bool readOnly /*= false*/)
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

	return m_pUGWorkspace->OpenDataSource(dcon);

}

UGDataSource* Workspace::OpenDatasourceWeb(UGEngineType type, string name, string serverUrl, string OGCType /*= NULL*/, string serverKey /*= NULL*/)
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

void Workspace::CloseDatasource(string name)
{
	// Convert string to UGString
	UGString ug_name;
	ug_name.FromStd(name);
	m_pUGWorkspace->m_DataSources.ReleaseAt(ug_name);
}