/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.05.15
 */
// JunSuMFCDialogDlg.h : header file
//

#pragma once
#include "Mapping/MapControl.h"
#include "StrConvertor.h"

using namespace SuperMap;


// JunSuMFCDialogDlg dialog
class JunSuMFCDialogDlg : public CDialogEx
{
// Construction
public:
	JunSuMFCDialogDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_JUNSUMFCDIALOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);

private:
	MapControl* m_pMapControl;
	void OpenWorkspace(string strPath);
	void CloseWorkspace();
	void Pan();

	UGLayer* m_pLastEditLayer;
	void ResetLastEditLayer(UGLayer* pLayer);
//Resource
private:
	CMenu mMenu;
public:
	afx_msg void OnFileOpen();
	afx_msg void OnDrawPoint();
	afx_msg void OnDrawLine();
	afx_msg void OnDrawRegion();
	afx_msg void OnMeasureLength();
	afx_msg void OnMeasureAea();
	afx_msg void OnFileClose();
	afx_msg void OnPanMap();
};
