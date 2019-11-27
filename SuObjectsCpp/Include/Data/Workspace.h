/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.05.09
 */
#ifndef SU_WORKSPACE_INCLUDED
#define SU_WORKSPACE_INCLUDED

#include "Data/Log.h"
#include "Workspace/UGWorkspace.h"


using namespace UGC;

namespace SuperMap
{
	class SU_DATA_API Workspace
	{
	public:
		Workspace();
		~Workspace();

		
		/*
		 * @en
		 * @brief Get the UGWorkspace handler
		 * @return        the pointer of UGWorkspace created in current Workspace object
		*/
		UGWorkspace* GetUGWorkspace();

		/*
		 * @en 
		 * @brief Open a local workspace file, such as *.smwu, *.sxwu
		 * @filePath      the path of workspace file
		 * @password      password of workspace, the default is an empty string
		 * @return        return true if opened, otherwise return false
		*/
		bool OpenWorkspaceFile(const string &filePath, const string &password="");

		/*
		 * @en 
		 * @brief Open a workspace in database server
		 * @type          type of workspace, which indicates the specific database
		 * @serverUrl     database's address, it's format are <ip>, <ip>:<port> or <hostname>:<port>
		 * @dbName        database's name
		 * @userName      avalaible user name
		 * @password      user's password to access the database
		 * @workspaceName workspace name created in the database
		 * @return        return true if opened, otherwise return false
		 */
		bool OpenWorkspaceDB(UGWorkspace::UGWorkspaceType type, const string &serverUrl, const string &dbName, const string &userName, const string &password, const string &workspaceName);

		/*
		 * @en
		 * @brief Open a local datasource file, such as *.udb, *.udbx(supported since v9.1.0), *.sci, *.tiff and so on
		 * @type          type of datasource, which indicates the specific file type
		 * @name          a user defined name of datasource, it must be unique in current workspace
		 * @filePath      path of datasurce file
		 * @password      used for udb or udbx, the defualt is is an empty string
		 * @readOnly      used for udb or udbx, the default is false meaning the data can be modified
		 * @return        return a pointer of datasource if opened, otherwise return NULL
		 */
		UGDataSource* OpenDatasourceFile(UGEngineType type, const string &name, const string &filePath, const string &password = "", bool readOnly = false);

		/*
		 * @en
		 * @brief Open a datasource stored in database, such as Oracle, MySQL, MongDB and so on
		 * @type          type of datasource, which indicates the specific database
		 * @name          a user defined name of datasource, it must be unique in current workspace
		 * @serverUrl     database's address, it's format are <ip> , <ip>:<port> or <hostname>:<port>
		 * @dbName        database's name
		 * @userName      avalaible user name
		 * @password      user's password to access the database
		 * @readOnly      the default is false meaning the data can be modified
		 * @return        return a pointer of datasource if opened, otherwise return NULL
		*/
		UGDataSource* OpenDatasourceDB(UGEngineType type, const string &name, const string &erverUrl, const string &dbName, const string &userName, const string &password, bool readOnly = false);

		/*
		 * @en
		 * @brief Open a web map service, such as rest, WMS, WMTS and so on
		 * @type          type of datasource, which indicates the specific file type
		 * @name          a user defined name of datasource, it must be unique in current workspace
		 * @serverUrl     url of web map
		 * @OGCType       used for OGC standard services, such as WMS, WMTS; the default is is an empty string.
		 * @serverKey     access key to the web map service, such as TianDiTu requires a key
		 * @return        return a pointer of datasource if opened, otherwise return NULL
		*/
		UGDataSource* OpenDatasourceWeb(UGEngineType type, const string &name, const string &serverUrl, const string &OGCType = "", const string &serverKey = "");
	
		/*
		 * @en
		 * @brief Close a datasource according to its index in the workspace
		 * @index    index of datasource which will be close
		*/
		void CloseDatasource(int index);

		/*
		 * @en
		 * @brief Close a datasource according to its name in the workspace
		 * @name  name of datasource which will be close
		*/
		void CloseDatasource(const string &name);

		/*
		* @en
		* @brief Remove a stored map according its name
		* @mapName the map need to be removed
		* @return True if removed , otherwise false.
		*/
		bool RemoveMap(const string &mapName);

		/*
		* @en
		* @brief Remove a stored scene according its name
		* @sceneName the scene need to be removed
		* @return    True if removed , otherwise false.
		*/
		bool RemoveScene(const string &sceneName);
	    
		/*
		 * @en
		 * @brief   Save current workspace.
		 * @return  Return ture if saved, otherwise return false.
		 */
		bool Save();

		/*
		 * @en
		 * @brief   Save as a new workspace file.
		 * @wkPath  The absolute path of workspace file which will be saved as.
		 * @return  Return ture if saved, otherwise return false.
		 */
		bool SaveAsFile(const string &wkPath);


		/*
		 * @en
		 * @brief   Create an UDB datasource. If the datource is created, it will add into current workspace.
		 * @udbPath The absolute path of UDB datasource file which will be created.
		 * @name    An unique name of created datasource will used in current workspace.
		 * @return  Return pointer of UGDataSource if created. If failed to create, return NULL.
		 */
		UGDataSource* CreateDatasourceUDB(const string &udbPath, const string &name);

	private:
		UGWorkspace* m_pUGWorkspace;
		
	};
}
#endif