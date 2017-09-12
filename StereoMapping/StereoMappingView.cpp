
// StereoMappingView.cpp : CStereoMappingView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "StereoMapping.h"
#endif

#include "StereoMappingDoc.h"
#include "StereoMappingView.h"
#include "MeasureDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStereoMappingView

IMPLEMENT_DYNCREATE(CStereoMappingView, CView)

BEGIN_MESSAGE_MAP(CStereoMappingView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CStereoMappingView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_32771, &CStereoMappingView::On32771)
END_MESSAGE_MAP()

// CStereoMappingView 构造/析构

CStereoMappingView::CStereoMappingView()
{
	// TODO: 在此处添加构造代码

}

CStereoMappingView::~CStereoMappingView()
{
}

BOOL CStereoMappingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CStereoMappingView 绘制

void CStereoMappingView::OnDraw(CDC* /*pDC*/)
{
	CStereoMappingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CStereoMappingView 打印


void CStereoMappingView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CStereoMappingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CStereoMappingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CStereoMappingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CStereoMappingView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CStereoMappingView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CStereoMappingView 诊断

#ifdef _DEBUG
void CStereoMappingView::AssertValid() const
{
	CView::AssertValid();
}

void CStereoMappingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CStereoMappingDoc* CStereoMappingView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStereoMappingDoc)));
	return (CStereoMappingDoc*)m_pDocument;
}
#endif //_DEBUG


// CStereoMappingView 消息处理程序


void CStereoMappingView::On32771()
{
	// TODO: Add your command handler code here
	CMeasureDialog dlg;
	dlg.DoModal();
	//dlg.ShowWindow(SW_SHOWNORMAL);IDD_MEASURE_DIALOG
}
