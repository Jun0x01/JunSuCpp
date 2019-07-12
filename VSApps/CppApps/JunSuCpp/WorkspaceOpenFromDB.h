#pragma once

#include "Data/Workspace.h"

// WorkspaceOpenFromDB dialog

class WorkspaceOpenFromDB : public CDialogEx
{
	DECLARE_DYNAMIC(WorkspaceOpenFromDB)


public:
	WorkspaceOpenFromDB(CWnd* pParent = nullptr);   // standard constructor
	virtual ~WorkspaceOpenFromDB();

	enum DialogType {
		NONE,
		NEW_WK,
		NEW_DS,
		OPEN_WK,
		OPEN_DS,
	};

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OpenWK_DB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	CString mText_DB_Type;
	
	CString mText_DB_Host;
	CString mText_DB_Port;
	CString mText_DB_Name;
	CString mText_DB_User_Name;
	CString mText_DB_User_Password;
	CString mText_DB_WorkspaceName;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSelChange_DB_Type();

	void SetWorkspace(Workspace* pWorkspace);

	void SetDialogType(DialogType type);
private:
	Workspace* m_pWorkspace;
	DialogType mDialogType;
};
