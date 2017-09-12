#pragma once
#include "AerialBase.h"
#include <afxdialogex.h>
#include "resource.h"

// childDialog 对话框
class singleImageView;
class childDialog : public CDialogEx
{
	DECLARE_DYNAMIC(childDialog)

public:
	childDialog(CWnd* pParent = NULL);   // 标准构造函数
	singleImageView* getView()
	{
		return m_singleView;
	}
	virtual ~childDialog();

// 对话框数据
	enum { IDD = IDD_ChildDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持


	DECLARE_MESSAGE_MAP()
protected:
	singleImageView* m_singleView;
	CFrameWnd* m_pwnd;
	//CString fileOpenType;
	CAerialPhoto* m_pAerialPhoto;
public:

	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void setAerialPhoto(CAerialPhoto* photo);
	afx_msg void loadImage();
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
