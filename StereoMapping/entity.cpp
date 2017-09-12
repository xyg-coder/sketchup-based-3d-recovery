#include "StdAfx.h"
#include "entity.h"
#include "render.h"
#include "StereoViewDlg.h"
#include "AerialBase.h"

entity::entity(void)
{
}


entity::~entity(void)
{
}

entityAlone::entityAlone(CEditLine* pLLine,CEditLine* pRLine)
	:m_pLLinePoints(pLLine),m_pRLinePoints(pRLine)
{
	m_pLDominant=new CEditLine;
	m_pRDominant=new CEditLine;
	m_isPreviewExisted=false;
}

entityAlone::entityAlone(CEditLine* pLLine,CEditLine* pRLine, 
	lastPoint* lastPt)
	:m_pLLinePoints(pLLine),m_pRLinePoints(pRLine)
{
	m_pLDominant=new CEditLine;
	m_pRDominant=new CEditLine;
	m_isPreviewExisted=false;
	CPoint3D lpt,rpt;
	lpt.m_X = lastPt->m_lLastPt->m_X;
	lpt.m_Y = lastPt->m_lLastPt->m_Y;
	lpt.m_Z=0;
	rpt.m_X = lastPt->m_rLastPt->m_X;
	rpt.m_Y = lastPt->m_rLastPt->m_Y;
	rpt.m_Z=0;
	m_pLDominant->push_back(lpt);
	m_pRDominant->push_back(rpt);
	m_pLDominant->Normalize();
	m_pRDominant->Normalize();
}

entityAlone::~entityAlone()
{
	delete m_pLDominant;
	m_pLDominant=NULL;
	delete m_pRDominant;
	m_pRDominant=NULL;
}

void entity::getRealCoor(double lx, double ly, double rx, double ry, CStereoViewDlg* pDlg,
	double &X, double &Y, double &Z)
{
	float px[2], py[2];
	double x, y;
	pDlg->m_LViewer.LocalToPixel(lx, ly, x, y);
	px[0] = float(x);
	py[0] = float(y);
	pDlg->m_RViewer.LocalToPixel(rx, ry, x, y);
	px[1] = float(x);
	py[1] = float(y);
	CAerialBlock::SpaceIntersection(pDlg->m_lpAerialPhoto, px, py, 2, &X, &Y, &Z);
}

void entity::getPointCoor(double X, double Y, double Z,
	CStereoViewDlg* pDlg, double &lx, double &ly, double &rx, double &ry)
{
	double plx, ply, prx, pry;
	pDlg->m_lpAerialPhoto[0]->ObjectCS2PixelCS(X, Y, Z, plx, ply);
	pDlg->m_lpAerialPhoto[1]->ObjectCS2PixelCS(X, Y, Z, prx, pry);
	pDlg->m_LViewer.PixelToLocal(plx, ply, lx, ly);
	pDlg->m_RViewer.PixelToLocal(prx, pry, rx, ry);
}

double entity::screenDist(double x1, double y1, double x2, double y2, NearestSide side,
	CStereoViewDlg* pDlg)
{
	double sx1, sy1, sx2, sy2;
	switch (side)
	{
	case LEFT:

		pDlg->m_LViewer.LocalToClient(x1, y1, sx1, sy1);
		pDlg->m_LViewer.LocalToClient(x2, y2, sx2, sy2);
		break;
	case RIGHT:
		pDlg->m_RViewer.LocalToClient(x1, y1, sx1, sy1);
		pDlg->m_RViewer.LocalToClient(x2, y2, sx2, sy2);
		break;
	default:
		break;
	}
	return sqrt((sx1 - sx2)*(sx1 - sx2) + (sy1 - sy2)*(sy1 - sy2));
}

void entity::outputFirstPoint(double &X, double &Y, double &Z, CStereoViewDlg* pDlg)
{
	X = 0;
	Y = 0;
	Z = 0;
}

void entityAlone::outputFirstPoint(double &X, double &Y, double &Z, CStereoViewDlg* pDlg)
{
	if (m_pLDominant->size() > 0)
	{
		CPoint3D lpt = (*m_pLDominant)[0];
		CPoint3D rpt = (*m_pRDominant)[0];
		getRealCoor(lpt.m_X, lpt.m_Y, rpt.m_X, rpt.m_Y, pDlg, X, Y, Z);
	}
}

void entityAlone::setLastPoint(lastPoint* lastPt)
{
	if (m_pLDominant->size()>0)
	{
		lastPt->m_lLastPt = (&((*m_pLDominant)[m_pLDominant->size() - 1]));
		lastPt->m_rLastPt = (&((*m_pRDominant)[m_pLDominant->size() - 1]));
		lastPt->m_isLastPointSet = true;
	}
}

void entityAlone::getFirstPoint(double &lx,double& ly,double &rx,double &ry)
{
	if (m_pLDominant->size()==0)
	{
		return;
	}
	else
	{
		lx=(*m_pLDominant)[0].m_X;
		ly=(*m_pLDominant)[0].m_Y;
		rx=(*m_pRDominant)[0].m_X;
		ry=(*m_pRDominant)[0].m_Y;
	}
}

void entityAlone::getLastPoint(double &lx,double& ly,double &rx,double &ry)
{
	if (m_pLDominant->size()==0)
	{
		return;
	}
	else if(m_isPreviewExisted)
	{
		lx=m_previewLx;
		ly=m_previewLy;
		rx=m_previewRx;
		ry=m_previewRy;
	}
	else
	{
		int size=m_pLDominant->size();
		lx=(*m_pLDominant)[size-1].m_X;
		ly=(*m_pLDominant)[size-1].m_Y;
		rx=(*m_pRDominant)[size-1].m_X;
		ry=(*m_pRDominant)[size-1].m_Y;
	}
}

void entityAlone::clearData()
{
	m_pLDominant->clear();
	m_pRDominant->clear();
	m_pLLinePoints->clear();
	m_pRLinePoints->clear();
}