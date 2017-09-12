// PixelViewerView.cpp : implementation of the CPixelViewer class
//

#include "stdafx.h"
#include <math.h>
//#include "PixelMax.h"
#include "PixelViewerView.h"
#include "charaTrans.h"
//#include <cstringt.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPixelViewer

IMPLEMENT_DYNCREATE(CPixelViewer, CView)

BEGIN_MESSAGE_MAP(CPixelViewer, CView)
	// Standard printing commands
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	//ON_WM_MOUSEMOVE()
	//ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
//	ON_WM_SETCURSOR()
	//ON_WM_CONTEXTMENU()
/*
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID_PAN, OnPan)
	ON_UPDATE_COMMAND_UI(ID_PAN, OnUpdatePan)
	ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
	ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
	ON_COMMAND(ID_ZOOM_IN_CLICK, OnZoomInClick)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_IN_CLICK, OnUpdateZoomInClick)
	ON_COMMAND(ID_ZOOM_OUT_CLICK, OnZoomOutClick)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_OUT_CLICK, OnUpdateZoomOutClick)
	ON_COMMAND(ID_ZOOM_EXTENT, OnZoomExtent)
	ON_COMMAND(ID_ZOOMIN_BOX, OnZoomBox )
	ON_UPDATE_COMMAND_UI(ID_ZOOMIN_BOX, OnUpdateZoomBox)
	ON_COMMAND(ID_ZOOM_FIT, OnZoomFit)
	ON_COMMAND(ID_ROTATE_CCW90, OnCCWRotate90)
	ON_COMMAND(ID_ROTATE_CW90,  OnCWRoate90)
	ON_COMMAND(ID_FLIP_HORIZ, OnFlipHoriz)
	ON_COMMAND(ID_FLIP_VERT, OnFlipVert)
	ON_COMMAND(ID_CUSTOM_ROTATE, OnCustomRotate)
	ON_COMMAND(ID_POINT_ROTATE, OnPointRotate)
	ON_UPDATE_COMMAND_UI(ID_POINT_ROTATE, OnUpdatePointRotate)
	ON_COMMAND(ID_GOPOINT, OnGoPoint)
	ON_COMMAND(ID_RASTER_SMOOTH, OnViewSmooth)
	ON_COMMAND(ID_BACKGROUND_COLOR, OnSetBkColor)
	ON_COMMAND(ID_VIEW_ASPECT, OnViewAspect)
	ON_COMMAND(ID_PIXEL_POINT, OnLoadPixelPoint)
	ON_COMMAND(ID_VIEW_ADJUST, OnAdjust)
*/
//ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// CPixelViewer construction/destruction

CPixelViewer::CPixelViewer()
: lpRasterLayer(NULL)
, lpAssociatedViewer(NULL)
{
	m_ViewSize.cx = 0;
	m_ViewSize.cy =0;
	m_hglRc = NULL;

	m_PrevPt.x = 0;
	m_PrevPt.y = 0;

	m_eViewOpCode = VIEW_PAN;

	m_BackgroundColor = RGB(0, 0, 0);

	memset(&m_LastRect, 0, sizeof(RECT));

	EnableCrossHair(true);


// 	SetMaximumZoom(200);
// 	SetMinimumZoom(-200);
}

CPixelViewer::~CPixelViewer()
{
}

#define CUSTOM_CLASSNAME _T("PIXEL_GL_WINDOW_CLASS")
BOOL CPixelViewer::PreCreateWindow(CREATESTRUCT& cs)
{
	// these styles are requested by OpenGL
	cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	//cs.hInstance = hInstance;
	if( !CView::PreCreateWindow(cs) )
		return FALSE;

	// Register the window class if it has not already been registered.
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();/*hInstance;*/

	// check if our class has been already registered (typical in MDI environment)
	if(!(::GetClassInfo(hInst, CUSTOM_CLASSNAME, &wndcls)))
	{
		// get default MFC class settings
		if(::GetClassInfo(hInst, cs.lpszClass, &wndcls))
		{
			// set our class name
			wndcls.lpszClassName = CUSTOM_CLASSNAME;
			// these styles are set for GL to work in MDI
			wndcls.style |= (CS_OWNDC | CS_HREDRAW | CS_VREDRAW);
			wndcls.hbrBackground = NULL;
			// try to register class (else throw exception)
			if (!AfxRegisterClass(&wndcls)) AfxThrowResourceException();
		}
		// default MFC class not registered
		
		else 
		{	
			DWORD t = ::GetLastError();//1411 Class does not exist.
		AfxThrowResourceException();
		}
	}
	// set our class name in CREATESTRUCT
	cs.lpszClass = CUSTOM_CLASSNAME;
	// we're all set
	return TRUE;
}

// CPixelViewer drawing

void CPixelViewer::OnDraw(CDC* /*pDC*/)
{
	/*
	CPixelViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	*/

	// TODO: add draw code for native data here
}


// CPixelViewer printing

BOOL CPixelViewer::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPixelViewer::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPixelViewer::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CPixelViewer diagnostics

#ifdef _DEBUG
void CPixelViewer::AssertValid() const
{
	CView::AssertValid();
}

void CPixelViewer::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

/*
CPixelViewerDoc* CPixelViewer::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPixelViewerDoc)));
	return (CPixelViewerDoc*)m_pDocument;
}*/
#endif //_DEBUG

void CPixelViewer::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	
	//UpdateView();	
}

/*void CPixelViewer::UpdateView()
{
// 	CPixelViewerDoc* pDoc = GetDocument();
// 	ASSERT(pDoc);
	
	if(!m_geoImg.Open(_T("D:\\Oblique\\Images\\Cam01\\CAM01_00007.tif")))
		MessageBox(_T("图像加载失败"));
	//m_geoImg.Open(_T("C:\\Users\\casm\\Desktop\\20.tif"));
	if(lpRasterLayer)
	{
		//double matrix[6];
		//lpGeoImage->GetGeoTranslate(matrix);
		//lpRasterLayer->SetXOffset(matrix[0]);
		//lpRasterLayer->SetXScaleFactor(matrix[1]);
		//lpRasterLayer->SetYOffset(matrix[3]);
		//lpRasterLayer->SetYScaleFactor(matrix[5]);
		if(lpRasterLayer->Initalize(&m_geoImg, "o"))
		{
			//lpRasterLayer->ShowBinary(0.0);
			//lpRasterLayer->SetRasterAdjust(0.0, 1.2);

			if(!AddLayer(lpRasterLayer))
				MessageBox(_T("error add"));
			//SetBackLayer(lpRasterLayer);
			//ZoomToCenter(100.0f);
			ZoomExtents();
// 			double minx, miny, maxx, maxy;
// 			GetExtents(minx, miny, maxx, maxy);
			Invalidate(FALSE);
		}	
	}

// 	GraphicLayer* lpGraphciLayer = (GraphicLayer*)CreateLayer(GRAPHIC_LAYER);
// 	if(lpGraphciLayer)
// 	{
// 		lpGraphciLayer->SetName("Vector");
// 		AddLayer(lpGraphciLayer);

		//Test Text Object 
// 		GraphicText* lpText = (GraphicText*)lpGraphciLayer->CreateGraphicObject(GO_TEXT);
// 		//lpText->SetFont()
// 		lpText->SetText("武汉大学", 1000, 1000);
// 
// 		lpText->SetId(0);
// 
// 		lpGraphciLayer->AddGraphicObject(lpText, RGB(255, 0, 0));
//	}
	

//	ZoomToCenter(100.0);
	//FlipVertical();
// 	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
// 	pMainFrm->m_comboBox.SetCurSel(4);	
	
}*/

// CPixelViewer message handlers
int  CPixelViewer::GetWidth()  const
{
	return m_ViewSize.cx;
}

int  CPixelViewer::GetHeight() const
{
	return m_ViewSize.cy;
}

//////////////////////////////////////////////////////////////////////////
//      设置OpenGL环境句柄与window的绘制句柄关联
//////////////////////////////////////////////////////////////////////////
void  CPixelViewer::MakeCurrentGL()
{
	CDC* pDC = GetDC();
	wglMakeCurrent(pDC->GetSafeHdc(), m_hglRc);
	ReleaseDC(pDC);
	//glDrawBuffer(GL_BACK);
	//glReadBuffer(GL_BACK);
	//glColorMask(GL_TRUE,GL_FALSE,GL_FALSE,GL_TRUE);
}

void  CPixelViewer::ResetCurrentGL()
{
	wglMakeCurrent(NULL, NULL);
}

void  CPixelViewer::Refresh()
{
	Invalidate(FALSE);
}

void CPixelViewer::SetDCPixelFormat(HDC hDC)
{
	int nPixelFormat;
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR), // Size of this structure
		1, // Version of this structure
		PFD_DRAW_TO_WINDOW | // Draw to window (not to bitmap)
		PFD_SUPPORT_OPENGL | // Support OpenGL calls in window
		PFD_DOUBLEBUFFER/*|
		PFD_STEREO*/, // Double buffered mode
		PFD_TYPE_RGBA, // RGBA color mode
		32, // Want 32-bit color
		0,0,0,0,0,0, // Not used to select mode
		0,0, // Not used to select mode
		0,0,0,0,0, // Not used to select mode
		16, // Size of depth buffer
		0, // No stencil
		0, // No auxiliary buffers
		0, // Obsolete or reserved
		0, // No overlay and underlay planes
		0, // Obsolete or reserved layer mask
		0, // No transparent color for underlay plane
		0}; // Obsolete

		// Choose a pixel format that best matches that described in pfd
		// for the given device context
		nPixelFormat = ChoosePixelFormat(hDC, &pfd);

		// Set the pixel format for the device context
		SetPixelFormat(hDC, nPixelFormat, &pfd);

		m_hglRc = wglCreateContext(hDC);

		wglMakeCurrent(hDC, m_hglRc);
		InitializeGL();
		wglMakeCurrent(NULL, NULL);

		SetBackgroundColor(m_BackgroundColor);
}



void CPixelViewer::OnSize(UINT nType,	  int cx,	  int cy )
{
	CView::OnSize(nType, cx, cy);

	if(cx != 0 && cy != 0)
		{
			int oldSizeX = m_ViewSize.cx;
			int oldSizeY = m_ViewSize.cy;

			m_ViewSize.cx = cx;
			m_ViewSize.cy = cy;

		   ResizeEvent(oldSizeX, oldSizeY);
		}
}
 
int CPixelViewer::OnCreate(LPCREATESTRUCT lpcs)
{
	if(CView::OnCreate(lpcs) == -1)
		return -1;

	CRect rect;
	CDC* pDC = GetDC();	
	SetDCPixelFormat( pDC->GetSafeHdc());
	ReleaseDC(pDC);
	GetWindowRect(&rect);	

	m_ViewSize.cx = rect.Width();
	m_ViewSize.cy = rect.Height();
	//ResizeEvent(2, 2);
	lpRasterLayer = (RasterLayer*)CreateLayer(RASTER);
	return 0;
}

void CPixelViewer::OnDestroy()
{
	if(m_hglRc)
	{
		CDC* pDC = GetDC();
		wglMakeCurrent(pDC->GetSafeHdc(), m_hglRc);
		if(lpRasterLayer != NULL)
		{
			lpRasterLayer->Initalize(NULL, "image");
			DeleteLayer( lpRasterLayer);
		}
		DestroyGL();

		wglMakeCurrent(NULL, NULL);
		ReleaseDC(pDC);

		wglDeleteContext (m_hglRc); 
		m_hglRc = NULL;
		
	}

	CView::OnDestroy();
}

void CPixelViewer::OnPaint()


{
	CPaintDC dc(this);
	// prepare a semaphore
	static BOOL 	bBusy = FALSE;
	// use the semaphore to enter this critic section
	if(bBusy) return;
	bBusy = TRUE;

	MakeCurrentGL();

	PaintGL();

	SwapBuffers(dc.GetSafeHdc());

	GLenum glErrorCode =glGetError();
	if(glErrorCode != GL_NO_ERROR)
	{
		const GLubyte *estring;
		CString mexstr;
		CString Lmexstr;
		// get the error descritption from OGL
		estring = gluErrorString(glErrorCode);
		// prepare and show a message box
		mexstr.Format(_T("PixelViewer:\n\tAn OpenGL error occurred: %s\n"), estring);
		AfxMessageBox(mexstr,MB_OK | MB_ICONEXCLAMATION);
		// replicate mex to debug trace
		TRACE0(mexstr);
		//TRACE(mexstr);
		// turn the semaphore "red" to avoid other wrong drawings
		bBusy=TRUE;
	}
	else
	{
		// turn the semaphore "green"
		bBusy = FALSE;
	}

	ResetCurrentGL();
}

BOOL CPixelViewer::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CPixelViewer::OnLButtonDown(UINT nFlags, CPoint point)
{
	/*if(m_eViewOpCode == VIEW_PAN)
	{
		CPixelViewerApp* pApp = (CPixelViewerApp*)AfxGetApp();
		SetCursor(pApp->m_hPaningCursor);
	}

	m_PrevPt.x = point.x;
	m_PrevPt.y = point.y;

	SetCapture();*/
	SetFocus();
	//ZoomExtents();
	//Invalidate(FALSE);
	CView::OnLButtonDown(nFlags, point);
}
static void NormalizeRect(RECT& rect)
{
	int nTemp;
	if (rect.left > rect.right)
	{
		nTemp = rect.left;
		rect.left = rect.right;
		rect.right = nTemp;
	}
	if (rect.top > rect.bottom)
	{
		nTemp = rect.top;
		rect.top = rect.bottom;
		rect.bottom = nTemp;
	}
}

void CPixelViewer::OnMouseMove(UINT nFlags, CPoint point)
{
	/*if(nFlags & MK_LBUTTON)
	{	

		if(m_eViewOpCode == VIEW_PAN )
		{
			int dx =  m_PrevPt.x - point.x;
			int dy =  point.y - m_PrevPt.y;
			PanBy(dx, dy);
			m_PrevPt.x = point.x;
			m_PrevPt.y = point.y;	
			Invalidate();
		}
		else if(m_eViewOpCode == VIEW_POINTROTATE)
		{
			double dy = (double)point.y - (double)m_PrevPt.y;
			double dx = (double)point.x - (double)m_PrevPt.x;
			double angle = atan2(dy, dx) * 1.8/PI;
			RotateBy(angle);
			Invalidate();	
			
		}
		else if(m_eViewOpCode == VIEW_BOXZOOMIN)
		{
			RECT curRect;
			SIZE size;

			size.cx = 1; size.cy = 1;

			curRect.left =   m_PrevPt.x;
			curRect.right =  point.x;
			curRect.top   =  m_PrevPt.y;
			curRect.bottom = point.y;

			NormalizeRect(curRect);			

			CDC* pDC = GetDC();

			pDC->DrawDragRect(&curRect, size, &m_LastRect, size);			

			ReleaseDC(pDC);

			m_LastRect = curRect;
		}
			
	}

	
	double dWorldX, dWorldY, GeoX, GeoY;
	TranslateScreenToWorld( (double)point.x, (double)(GetHeight()-point.y),dWorldX, dWorldY);

	CPixelViewerDoc* pDoc = GetDocument();
	pDoc->Image2Geo(dWorldX, dWorldY, GeoX, GeoY);

	((CMainFrame*)AfxGetMainWnd())->ShowCoords(dWorldX, dWorldY, GeoX, GeoY);
	
*/

	CView::OnMouseMove(nFlags, point);
}

void CPixelViewer::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_eViewOpCode == VIEW_ZOOMIN_CLICK)
	{
		ScreenLocation loc;
		loc.mX = point.x;
		loc.mY = GetHeight() - point.y -1;
		double dZoom = GetZoomPercentage() * 1.1;
		ZoomAboutPoint(loc, dZoom);
		Invalidate(FALSE);
	}
	else if(m_eViewOpCode == VIEW_ZOOMOUT_CLICK)
	{
		ScreenLocation loc;
		loc.mX = point.x;
		loc.mY = GetHeight() - point.y -1;
		double dZoom = GetZoomPercentage() * 0.9;
		ZoomAboutPoint(loc, dZoom);
		Invalidate(FALSE);
	}
	else if(m_eViewOpCode == VIEW_BOXZOOMIN)
	{
		ScreenLocation loc1, loc2;

		loc1.mX = m_LastRect.left;
		loc1.mY = GetHeight() - m_LastRect.bottom - 1;

		loc2.mX = m_LastRect.right;
		loc2.mY = GetHeight() - m_LastRect.top - 1;

		PixelView::ZoomToBox(loc1, loc2);
		Invalidate(FALSE);

		memset(&m_LastRect, 0, sizeof(RECT));
	}

	ReleaseCapture();
	CView::OnLButtonDown(nFlags, point);

}

BOOL CPixelViewer::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
//和立体界面里面一致，都是Z、X缩放
// 	int Zoom = zDelta/60;
// 
// 	ZoomBy(Zoom);
// 	Invalidate(FALSE);

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

BOOL CPixelViewer::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	/*if(pWnd == this)
	{
		CPixelViewerApp* pApp = (CPixelViewerApp*)AfxGetApp();
		if(m_eViewOpCode == VIEW_PAN)
		{
			SetCursor(pApp->m_hPanCursor);
			return TRUE;
		}
		else if(m_eViewOpCode == VIEW_POINTROTATE)
		{
			SetCursor(pApp->m_hRotateCursor);
			return TRUE;
		}
		else if(m_eViewOpCode == VIEW_ZOOMIN_CLICK)
		{
			SetCursor(pApp->m_hZoominCursor);
			return TRUE;
		}
		else if(m_eViewOpCode == VIEW_ZOOMOUT_CLICK)
		{
			SetCursor(pApp->m_hZoomoutCursor);
			return TRUE;
		}
		else if(m_eViewOpCode == VIEW_BOXZOOMIN)
		{
			SetCursor(pApp->m_hZoomBoxCursor);
			return TRUE;
		}
	}*/

	return CView::OnSetCursor(pWnd, nHitTest, message);
}

void CPixelViewer::OnContextMenu(	CWnd* pWnd,	CPoint pos )
{
	//DebugBreak();
	/*CMenu menu;
	menu.LoadMenu(IDR_POP_MENU);

	CMenu* pop = menu.GetSubMenu(0);

	pop->TrackPopupMenu(TPM_LEFTALIGN, pos.x, pos.y, pWnd);*/
}


void CPixelViewer::OnPan()
{
	m_eViewOpCode = VIEW_PAN;
}

void CPixelViewer::OnUpdatePan( CCmdUI* pCmdUI )
{
	pCmdUI->SetCheck(m_eViewOpCode == VIEW_PAN);
}

void CPixelViewer::OnZoomIn()
{
	double dZoom = GetZoomPercentage() * 1.25;
	ZoomTo(dZoom);
	Invalidate(FALSE);
}

void CPixelViewer::OnZoomOut()
{
	double dZoom = GetZoomPercentage() / 1.25;
	ZoomTo(dZoom);
	Invalidate(FALSE);
}

void CPixelViewer::OnZoomInClick()
{
	m_eViewOpCode = VIEW_ZOOMIN_CLICK;
}

void CPixelViewer::OnUpdateZoomInClick(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_eViewOpCode == VIEW_ZOOMIN_CLICK);
}

void CPixelViewer::OnZoomOutClick()
{
	m_eViewOpCode = VIEW_ZOOMOUT_CLICK;
}

void CPixelViewer::OnUpdateZoomOutClick(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_eViewOpCode == VIEW_ZOOMOUT_CLICK);
}

void CPixelViewer::OnZoomExtent()
{
	ZoomExtents();
	Invalidate(FALSE);
}

void CPixelViewer::OnZoomBox()
{
	m_eViewOpCode = VIEW_BOXZOOMIN;
}

void CPixelViewer::OnUpdateZoomBox(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_eViewOpCode == VIEW_BOXZOOMIN);
}

void CPixelViewer::OnZoomFit()
{
	//ZoomTo(100);
	Reset();
	Invalidate(FALSE);
}

void CPixelViewer::OnCCWRotate90()
{
	RotateBy(-90);
	Invalidate(FALSE);
}

void CPixelViewer::OnCWRoate90()
{
	RotateBy(90);
	Invalidate(FALSE);
}

void CPixelViewer::OnFlipHoriz()
{
	FlipHorizontal();
	Invalidate(FALSE);
}

void CPixelViewer::OnFlipVert()
{
	FlipVertical();
	Invalidate(FALSE);
}

void CPixelViewer::OnCustomRotate()
{
	/*CInputDialog inputDlg;
	double rot = GetRotation();
	inputDlg.m_parameter = rot;

	if( inputDlg.DoModal() == IDOK)
	{
		if(rot != inputDlg.m_parameter)
		{
			RotateTo(inputDlg.m_parameter);
			Invalidate(false);
		}
		
	}*/
}

void CPixelViewer::OnPointRotate()
{
	m_eViewOpCode = VIEW_POINTROTATE;
}

void CPixelViewer::OnUpdatePointRotate(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_eViewOpCode == VIEW_POINTROTATE);
}

void CPixelViewer::OnGoPoint()
{
// 	CFindPointDlg FPDlg;
// 	FPDlg.m_nZoom = 100;
// 	FPDlg.m_X = 0.0f;
// 	FPDlg.m_Y = 0.0f;
// 
// 	if( FPDlg.DoModal() == IDOK)
// 	{
// 		LocationType pos;
// 		pos.mY = FPDlg.m_X;
// 		pos.mX = FPDlg.m_Y;
// 		ZoomToPoint(pos, FPDlg.m_nZoom);
// 		Invalidate(FALSE);
// 	}
}

void CPixelViewer::OnViewSmooth()
{
	if(GetTextureMode() == TEXTURE_LINEAR)
		SetTextureMode(TEXTURE_NEAREST_NEIGHBOR);
	else
		SetTextureMode(TEXTURE_LINEAR);

	Invalidate(FALSE);

}

void CPixelViewer::OnSetBkColor()
{
	CColorDialog clrDlg(GetBackgroundColor(), CC_FULLOPEN,this);
	if( clrDlg.DoModal() == IDOK)
	{
		SetBackgroundColor(clrDlg.GetColor());
		Invalidate();
	}
}

void CPixelViewer::OnViewAspect()
{
// 	CInputDialog inputDlg;
// 	double aspect = GetPixelAspect();
// 	inputDlg.m_parameter = aspect;
// 
// 	if(inputDlg.DoModal() == IDOK)
// 	{
// 		if(inputDlg.m_parameter != aspect)
// 		{
// 			SetPixelAspect(inputDlg.m_parameter);
// 			Invalidate(FALSE);
// 		}
// 	}
	
}



void CPixelViewer::OnLoadPixelPoint()
{
	CFileDialog FDlg(TRUE, _T(""), NULL, 4|2, NULL, this);

	if( FDlg.DoModal() == IDOK )
	{
		double ix, iy;
		//字符转换
		CString tempPath=FDlg.GetPathName();
		unsigned int size=WideCharToMultiByte(CP_OEMCP,NULL,tempPath.GetBuffer(),-1,NULL,0,NULL,false);
		char* sPath=new char[size];
		WCharToMByte(tempPath.GetBuffer(),sPath,size);
		FILE* fp = fopen( sPath, "rt");
		delete []sPath;
		if(fp == NULL) return;

		LayerList* pLList =  GetLayerList();

		if( pLList == NULL )
		{
			fclose(fp);
			return;
		}

		GraphicLayer* lpGraphicLayer = (GraphicLayer*) pLList->GetTopMostLayer(GRAPHIC_LAYER);

		if( lpGraphicLayer == NULL)
		{
			fclose(fp);
			return;
		}

		lpGraphicLayer->ClearContent();

		int id = 0;

		do 
		{
			if( fscanf(fp, "%lf %lf", &ix, &iy) == 2 )
			{
				GraphicPoint* lpPoint = (GraphicPoint*)lpGraphicLayer->CreateGraphicObject(GO_POINT);
				if(lpPoint != NULL)
				{
					lpPoint->SetId( id ++);
					lpPoint->SetPos(ix, iy);
					lpGraphicLayer->AddGraphicObject(lpPoint, RGB(255, 0, 0));
				}
			}

		} while (!feof(fp));

		fclose(fp);
	}
}

void CPixelViewer::OnAdjust()
{
// 	CAdjustDlg dlg;
// 	RasterLayer* lpLayer = (RasterLayer*) GetLayerList()->GetTopMostLayer(RASTER);
// 	float bias, contrast;
// 	lpLayer->GetRasterAdjust(bias, contrast);
// 	dlg.m_lpView = this;
// 
// 	if( dlg.DoModal() != IDOK )
// 	{
// 		//恢复原来的值
// 		lpLayer->SetRasterAdjust(bias, contrast);
// 
// 		Invalidate(FALSE);
// 	}
}

void CPixelViewer::SetRasterLayer(CGeoImage geoImg)
{
	//lpRasterLayer->Initalize(NULL, "view");
	//LoadImage(geoImg);
}

bool CPixelViewer::LoadImages(CString path)
{
	//m_ImgPath = path;
	//字符转换
	unsigned int size=WideCharToMultiByte(CP_OEMCP,NULL,path.GetBuffer(),-1,NULL,0,NULL,false);
	char* sPath=new char[size];
	WCharToMByte(path.GetBuffer(),sPath,size);
	if(!m_geoImg.Open(sPath))
	{
		MessageBox(_T("图像加载失败"));
		delete []sPath;
		sPath=NULL;
		return false;
	}
	delete []sPath;
	sPath=NULL;
	//Clear();
	//DeleteLayer(lpRasterLayer);
	//m_geoImg.Open(_T("C:\\Users\\casm\\Desktop\\20.tif"));
	if(lpRasterLayer)
	{
		lpRasterLayer->Initalize(NULL, "image");
		//Clear();
		if(lpRasterLayer->Initalize(&m_geoImg, "image"))
		{
			if(AddLayer(lpRasterLayer))
				SetBackLayer(lpRasterLayer);
			ZoomExtents();
			//ZoomTo(50);
			Invalidate(FALSE);
			//m_geoImg.Close();
			return true;
		}	
	}
	return false;
}


bool CPixelViewer::LoadImages(CGeoImage geoImg)
{
// 	if(geoImg==NULL)
// 	{
// 		MessageBox(_T("图像加载失败"));
// 		return false;
// 	}
	//Clear();
	lpRasterLayer = (RasterLayer*)CreateLayer(RASTER);
	//m_geoImg.Open(_T("C:\\Users\\casm\\Desktop\\20.tif"));
	if(lpRasterLayer)
	{
		lpRasterLayer->Initalize(NULL, "o");
		if(lpRasterLayer->Initalize(&geoImg, "o"))
		{
			if(!AddLayer(lpRasterLayer))
				MessageBox(_T("error add"));
			//SetBackLayer(lpRasterLayer);
			//ZoomToCenter(100.0f);
			ZoomExtents();
			Invalidate(FALSE);
			//geoImg.Close();
			return true;
		}	
	}
	return false;
}
const CPixelViewer* CPixelViewer::GetAssociatedViewer()	
{
	return  lpAssociatedViewer;
}

void CPixelViewer::SetAssociatedViewer(CPixelViewer* lpViewer)
{
	lpAssociatedViewer = lpViewer;
}

// void CPixelViewer::OnLButtonDblClk(UINT nFlags, CPoint point)
// {
// 	// TODO: Add your message handler code here and/or call default
// 	if (this->GetParent()->IsKindOf(RUNTIME_CLASS(CMainViewDlg)))
// 	{
// 		return;
// 	}
// 	if (lpAssociatedViewer->GetAssociatedViewer()!=this)
// 	{
// 		lpAssociatedViewer->SetAssociatedViewer(this);
// 		//lpAssociatedViewer->SetRasterLayer(m_geoImg.);
// 		lpAssociatedViewer->LoadImage(m_ImgPath);
// 	}
// 	__super::OnLButtonDblClk(nFlags, point);
// }
