// PixelViewerView.h : interface of the CPixelViewer class
//


#pragma once
#include "pixelviewlib.h"
#include "GeoImage.h"
#include "GeoDataView.h"
#include "RasterLayer.h"
#include "GraphicBase.h"
#include "GraphicLayer.h"
//#include "PixelViewerDoc.h"

enum VIEWOP {VIEW_PAN, VIEW_ZOOMIN_CLICK, VIEW_ZOOMOUT_CLICK,VIEW_BOXZOOMIN,VIEW_POINTROTATE};
//extern HINSTANCE hInstance;


class CPixelViewer : public CView, public GeoDataView
{
//protected: // create from serialization only
public:
	CPixelViewer();
	DECLARE_DYNCREATE(CPixelViewer)

// Attributes
public:
//	CPixelViewerDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate(); // called first time after construct

	//virtual void ZoomTo(double dPercent);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CPixelViewer();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	void UpdateView();
protected:
	//////////////////////////////////////////////////////////////////////////
	//                 获取视图的大小
	//////////////////////////////////////////////////////////////////////////
	virtual int  GetWidth()  const;
	virtual int  GetHeight() const;
	//////////////////////////////////////////////////////////////////////////
	//      设置OpenGL环境句柄与window的绘制句柄关联
	//////////////////////////////////////////////////////////////////////////
	virtual void  MakeCurrentGL();
	virtual void  ResetCurrentGL();

	virtual void  Refresh();

	void          SetDCPixelFormat(HDC hDC);


// Generated message map functions
protected:
	afx_msg int  OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnContextMenu(	CWnd* pWnd,	CPoint pos );

	afx_msg void OnPan();
	afx_msg void OnUpdatePan( CCmdUI* pCmdUI );
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnZoomInClick();
	afx_msg void OnUpdateZoomInClick(CCmdUI* pCmdUI);
	afx_msg void OnZoomOutClick();
	afx_msg void OnUpdateZoomOutClick(CCmdUI* pCmdUI);
	afx_msg void OnZoomExtent();
	afx_msg void OnZoomBox();
	afx_msg void OnUpdateZoomBox(CCmdUI* pCmdUI);
	afx_msg void OnZoomFit();
	afx_msg void OnCCWRotate90();
	afx_msg void OnCWRoate90();
	afx_msg void OnFlipHoriz();
	afx_msg void OnFlipVert();
	afx_msg void OnCustomRotate();
	afx_msg void OnPointRotate();
	afx_msg void OnUpdatePointRotate(CCmdUI* pCmdUI);
	afx_msg void OnGoPoint();
	afx_msg void OnViewSmooth();

	afx_msg void OnSetBkColor();
	afx_msg void OnViewAspect();
	afx_msg void OnLoadPixelPoint();
	afx_msg void OnAdjust();

	DECLARE_MESSAGE_MAP()

private:
	SIZE    m_ViewSize;
	HGLRC   m_hglRc;
	POINT   m_PrevPt;

	VIEWOP  m_eViewOpCode;

	COLORREF  m_BackgroundColor;

	RECT    m_LastRect;
	RasterLayer* lpRasterLayer;
	CPixelViewer* lpAssociatedViewer;
	//CString m_ImgPath;
public:
	CGeoImage m_geoImg;

public:
	void SetRasterLayer(CGeoImage geoImg);
	bool LoadImages(CString path);
	bool LoadImages(CGeoImage geoImg);
	const CPixelViewer* GetAssociatedViewer();
	void SetAssociatedViewer(CPixelViewer* lpViewer);
	//afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in PixelViewerView.cpp
// inline CPixelViewerDoc* CPixelViewer::GetDocument() const
//{ return reinterpret_cast<CPixelViewerDoc*>(m_pDocument); }
#endif

