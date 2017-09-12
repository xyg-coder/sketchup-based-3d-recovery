#pragma once

#include "resource.h"



// CInitSettingDlg dialog

class CInitSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CInitSettingDlg)

public:
	CInitSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInitSettingDlg();

// Dialog Data
	enum { IDD = IDD_INITSETTING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedImagesButton();
	afx_msg void OnBnClickedMapsButton();
	afx_msg void OnBnClickedOk();
public:
	CString m_ImagesPath;
	CString m_MapsPath;
};
