#include "StdAfx.h"
#include "entitySing.h"
#include "singleImageView.h"

entitySing::entitySing(GraphicLayer* pPointLayer, GraphicLayer* pLineLayer,
	singleImageView* pView, directionSing* verticalD)
	:m_pPointLayer(pPointLayer), m_pLineLayer(pLineLayer),
	m_pView(pView), m_verticalDirection(verticalD),
	m_line(NULL), m_lastPoint(NULL)
{
	m_isPreviewExisted = false;
	m_tmpPointSize = 0;
	m_previewIndex = 0;
}

entitySing::entitySing(GraphicLayer* pPointLayer, GraphicLayer* pLineLayer,
	singleImageView* pView, lastPointSing* lastP, directionSing* verticalD)
	:m_pPointLayer(pPointLayer), m_pLineLayer(pLineLayer),
	m_pView(pView), m_verticalDirection(verticalD),
	m_line(NULL), m_lastPoint(lastP)
{
}

void entitySing::getRealCoor(double x, double y, singleImageView* pView,
	double &X, double &Y, double &Z)
{
	pView->getRealCoor(x, y, X, Y, Z);
}

void entitySing::getPointCoor(double X, double Y, double Z,
	singleImageView* pView, double &x, double &y)
{
	pView->getPointCoor(X, Y, Z, x, y);
}

void entitySing::setLastPoint(lastPointSing* lpoint)
{
	if (m_tmpPointSize>0)
	{
		lpoint->m_lastPoint = m_dominantPoints[m_tmpPointSize - 1];
		lpoint->m_isLastPointSet = true;
	}
}

entitySing::~entitySing()
{
}

double screenDist(double x1, double y1, double x2, double y2, singleImageView* pView)
{
	double sx1, sx2, sy1, sy2;
	pView->TranslateWorldToScreen(x1, y1, sx1, sy1);
	//pView->TranslateScreenToWorld(x2, y2, sx2, sy2);
	pView->TranslateWorldToScreen(x2, y2, sx2, sy2);
	return sqrt((sx1 - sx2)*(sx1 - sx2) + (sy1 - sy2)*(sy1 - sy2));
}

void entitySing::getFirstPoint(double &x,double &y)
{
	if (m_dominantPoints.size()==0)
	{
		return;
	}
	else
	{
		m_dominantPoints[0]->GetPos(x,y);
	}
}

void entitySing::getLastPoint(double &x,double &y)
{
	if (m_dominantPoints.size()==0)
	{
		return;
	}
	else if(m_isPreviewExisted)
	{
		x=m_previewx;
		y=m_previewy;
	}
	else
	{
		m_dominantPoints[m_dominantPoints.size()-1]->GetPos(x,y);
	}
}