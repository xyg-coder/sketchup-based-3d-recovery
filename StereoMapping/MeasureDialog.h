#pragma once

#include "GLee.h" //添加这一句，解决 error C1189: #error :  gl.h included before glee.h

#include "SSplitter.h"
#include "StereoView.h"
#include "StereoViewDlg.h"
#include "AerialBase.h"
#include "afxcmn.h"

#include "childDialog.h"

// CMeasureDialog dialog

class CMeasureDialog : public CDialog
{
	DECLARE_DYNAMIC(CMeasureDialog)

public:
	CMeasureDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMeasureDialog();

// Dialog Data
	enum { IDD = IDD_MEASURE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


public:
	CSSplitter m_MainSplitterPane;
	//CSSplitter m_LeftSplitterPane;
	CTreeCtrl m_blockTree;
	//CListCtrl m_ListCtl;
	CStereoView m_stereoView;//立体显示窗口
	bool m_bstrip; ///是否按照航带自动构造立体像对

	CStereoViewDlg m_viewer;//20160612 原来为私有


	//基准面格网
	


private:
	CString m_MapsPath;
	CString m_proPath;
	HTREEITEM m_hRoot;


	CAerialBlock m_block;

	//建模工具状态标识
	BOOL m_bPolygon;//是否处于多边形建模状态
	TOOLS m_tool;

public:
	void AddBlockToTree(CAerialBlock* block);


private:
	void SetViewPosition();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileOpen();
	//树形控件鼠标单击相应函数
	afx_msg void OnTvnSelchangedBlockTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCreatePolygon();
	afx_msg void OnUpdateCreatePolygon(CCmdUI *pCmdUI);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);//让建模工具选择后对应的菜单前面打勾
//	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	

//////////////////////////////////////////////////////////////////////////
//20160608 Li

	CTabCtrl m_TabImgView; //选项卡

	//下面是单像的measureDlg中的函数，供单像显示对话框调用

	bool m_isFileOpen; //在OnInit函数中初始化

	childDialog m_childDlg; //单像显示的对话框，放到选项卡中切换的

	//调整显示影像窗体大小用的
	void ReSizeImg(void);  
	POINT old; 
	bool m_bIsCtrlInit;
	//OnNcLButtonDown消息中使用
	bool m_bDragFullWindow; //判断系统设置，改变窗体大小时是否实时显示窗体内容，为了减少闪烁

	//根据三维点坐标获取多视影像
	void GetMultiViewImg(double X, double Y, double Z);

	//20160628 多视影像
	CPoint3D m_MVHomoPnt; //多视影像的同名点
	std::vector<CPoint2D> m_vMVPxlCent; //像素坐标，影像显示的区域中心
	std::vector<int> m_vMVStripIndex; //航带序号
	std::vector<int> m_vMVPhotoIndex; //航带中影像序号
	int m_nMultiViewImgIndex; //影像在vector中的序号

//////////////////////////////////////////////////////////////////////////


//	afx_msg void OnNMClickTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnMove(int x, int y);
	//afx_msg void OnHorizonDatum();
	afx_msg void OnHorizonDatum();
	afx_msg void OnEnlargeGrid();
	afx_msg void OnDenseGrid();
	afx_msg void OnVerticalDatum();
	afx_msg void OnRandomDatum();
	afx_msg void OnMultiViewImg();
	afx_msg void OnRefreshMultiViewImg();
	afx_msg void OnPolyline();
	afx_msg void OnCircle();
	afx_msg void On32815();
	afx_msg void OnCloseDatumGrid();
	afx_msg void OnPolygon();
	afx_msg void OnArc();
	afx_msg void OnVerticalSwitch();
	afx_msg void OnShowToolState();
	afx_msg void OnUpdatePolygon(CCmdUI *pCmdUI);
};
