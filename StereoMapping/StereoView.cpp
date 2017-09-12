// StereoView.cpp : implementation file
//

#include "stdafx.h"
#include "StereoMapping.h"
#include "StereoView.h"


// CStereoView

IMPLEMENT_DYNCREATE(CStereoView, CView)

CStereoView::CStereoView()
{

}

CStereoView::~CStereoView()
{
}

BEGIN_MESSAGE_MAP(CStereoView, CView)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CStereoView drawing

void CStereoView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CStereoView diagnostics

#ifdef _DEBUG
void CStereoView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CStereoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CStereoView message handlers


void CStereoView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
}
