#include "StdAfx.h"
#include "circleSing.h"
#include "singleImageView.h"

circleSing::circleSing(GraphicLayer* pPointLayer, GraphicLayer* pLineLayer,
	singleImageView* pView, directionSing* verticalD):
	arcSing(pPointLayer,pLineLayer,pView,verticalD)
{
}

circleSing::circleSing(GraphicLayer* pPointLayer, GraphicLayer* pLineLayer,
	singleImageView* pView, lastPointSing* lastP, directionSing* verticalD) :
	arcSing(pPointLayer, pLineLayer, pView, lastP, verticalD)
{}

void circleSing::calculateLinePoins()
{
	if (m_linePoints != 0)
	{
		m_line->DeleteVertexs(0, m_linePoints);
		m_linePoints = 0;
	}
	for (double fai = 0; true;fai+=RADIOSTEP)
	{
		if (fai>=2*PI)
		{
			double X = m_cx + m_r;
			double Y = m_cy;
			double Z = m_averZ;
			double x, y;
			getPointCoor(X, Y, Z, m_pView, x, y);
			m_linePoints++;
			m_line->AddVertex(m_linePoints - 1, x, y);
			break;
		}
		double X = m_cx + m_r*cos(fai);
		double Y = m_cy + m_r*sin(fai);
		double Z = m_averZ;
		double x, y;
		getPointCoor(X, Y, Z, m_pView, x, y);
		m_linePoints++;
		m_line->AddVertex(m_linePoints - 1, x, y);
	}
}

void* circleSing::getData()
{
	circleSingData* pArcData=new circleSingData;
	pArcData->m_centerX=m_cx;
	pArcData->m_centerY=m_cy;
	pArcData->m_centerZ=m_cz;
	getPointCoor(m_cx,m_cy,m_cz,m_pView,pArcData->m_centerx,pArcData->m_centery);
	pArcData->m_r=m_r;
	for (int i=0;i<3;i++)
	{
		pArcData->m_dominantPX[i]=m_X[i];
		pArcData->m_dominantPY[i]=m_Y[i];
		pArcData->m_dominantPZ[i]=m_Z[i];
		getPointCoor(m_X[i],m_Y[i],m_Z[i],m_pView,
			pArcData->m_dominantPx[i],pArcData->m_dominantPy[i]);

	}
	return pArcData;
}

circleSing::~circleSing()
{
}
