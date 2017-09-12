#include "StdAfx.h"
#include "polyline.h"
#include "render.h"
#include "StereoViewDlg.h"

//两个向量进行叉乘;
inline void crossProduct(double dX1, double dY1, double dZ1,
	double dX2, double dY2, double dZ2,
	double &resultdX, double& resultdY, double &resultdZ)
{
	resultdX = dY2*dZ1 - dZ2*dY1;
	resultdY = -(dX2*dZ1 - dZ2*dX1);
	resultdZ = dX2*dY1 - dY2*dX1;
}

polyline::polyline(CEditLine* pLLine, CEditLine* pRLine, CStereoViewDlg* pDlg,
	VERTICALMODE vertical/*,bool isClose*/)
:entityAlone(pLLine, pRLine), m_verticalMode(vertical), m_pDlg(pDlg)/*,
m_isClose(isClose),m_isCloseLineExisted(false)*/
{
	//m_isDrawing = true;
	m_isMainSet = false;
}

polyline::polyline(CEditLine* pLLine, CEditLine* pRLine, CStereoViewDlg* pDlg,
	lastPoint* lastPt,VERTICALMODE vertical/*,lastPoint* firstPt,
	bool isClose*/)
	:entityAlone(pLLine,pRLine,lastPt),
	m_verticalMode(vertical), m_pDlg(pDlg)/*,
	m_isClose(isClose),m_firstPt(firstPt),
	m_isCloseLineExisted(false)*/
{
	CPoint3D lpt,rpt;
	lpt.m_X = lastPt->m_lLastPt->m_X;
	lpt.m_Y = lastPt->m_lLastPt->m_Y;
	lpt.m_Z = 0;
	rpt.m_X = lastPt->m_rLastPt->m_X;
	rpt.m_Y = lastPt->m_rLastPt->m_Y;
	rpt.m_Z = 0;
	m_pLLinePoints->push_back(lpt);
	m_pRLinePoints->push_back(rpt);
	m_pLLinePoints->Normalize();
	m_pRLinePoints->Normalize();
	m_isMainSet = false;
	//m_isDrawing = true;
}

polyline::~polyline(void)
{
}

void polyline::moveMouse(double lx,double ly,double rx,double ry)
{
	if (m_pLDominant->size()>0)
	{
		addPreviewPoint(lx,ly,rx,ry);
	}
}
void polyline::leftButtonDown(double lx,double ly,double rx,double ry)
{
}

void polyline::leftButtonUp(double lx,double ly,double rx,double ry)
{
	addDominantPoint(lx,ly,rx,ry);
}

void polyline::rightButtonDown(double lx,double ly,double rx,double ry)
{
	if (m_isPreviewExisted)
	{
		m_pLLinePoints->pop_back();
		m_pRLinePoints->pop_back();
		m_pLLinePoints->Normalize();
		m_pRLinePoints->Normalize();
	}
	m_isPreviewExisted=false;
}

void polyline::addDominantPoint(double lx,double ly,double rx,double ry)
{/*
	if (m_isCloseLineExisted)
	{
		m_pLLinePoints->pop_back();
		m_pRLinePoints->pop_back();
		m_pLLinePoints->Normalize();
		m_pRLinePoints->Normalize();
	}*/
	if (m_isPreviewExisted)
	{
		m_pLLinePoints->pop_back();
		m_pRLinePoints->pop_back();
		m_pLLinePoints->Normalize();
		m_pRLinePoints->Normalize();
	}
	getCorrectPoint(lx, ly, rx, ry, lx, ly, rx, ry);
	CPoint3D lpt,rpt;
	lpt.m_X=lx;
	lpt.m_Y=ly;
	lpt.m_Z=0;
	rpt.m_X=rx;
	rpt.m_Y=ry;
	rpt.m_Z=0;
	m_pLDominant->push_back(lpt);
	m_pRDominant->push_back(rpt);
	m_pLLinePoints->push_back(lpt);
	m_pRLinePoints->push_back(rpt);
	/*if (m_isClose)
	{
		m_pl
	}*/
	m_pLDominant->Normalize();
	m_pRDominant->Normalize();
	m_pLLinePoints->Normalize();
	m_pRLinePoints->Normalize();
	setMainDirection();
	m_isPreviewExisted=false;
}

void polyline::addPreviewPoint(double lx,double ly,double rx,double ry)
{
	if (m_isPreviewExisted)
	{
		m_pLLinePoints->pop_back();
		m_pRLinePoints->pop_back();
		m_pLLinePoints->Normalize();
		m_pRLinePoints->Normalize();
	}
	getCorrectPoint(lx, ly, rx, ry, lx, ly, rx, ry);
	m_previewLx=lx;
	m_previewLy=ly;
	m_previewRx=rx;
	m_previewRy=ry;
	CPoint3D lpt,rpt;
	lpt.m_X=lx;
	lpt.m_Y=ly;
	lpt.m_Z=0;
	rpt.m_X=rx;
	rpt.m_Y=ry;
	rpt.m_Z=0;
	m_pLLinePoints->push_back(lpt);
	m_pRLinePoints->push_back(rpt);
	m_pLLinePoints->Normalize();
	m_pRLinePoints->Normalize();
	m_isPreviewExisted=true;
}

void polyline::getNearestRef(double lx,double ly,double rx,double ry, double &rlx,
	double &rly,double &rrx,double &rry,double &minDist,NearestSide& side)
{
	minDist=999;
	for (int i=0;i<m_pLDominant->size();i++)
	{
		NearestSide tmpSide;
		CPoint3D lpt=(*m_pLDominant)[i];
		CPoint3D rpt=(*m_pRDominant)[i];
		double leftDist = screenDist(lx, ly, lpt.m_X, lpt.m_Y, LEFT, m_pDlg);
		double rightDist = screenDist(lx, ly, lpt.m_X, lpt.m_Y, RIGHT, m_pDlg);
		double dist; //= leftDist < rightDist ? leftDist : rightDist;
		if (leftDist<rightDist)
		{
			dist = leftDist;
			tmpSide = LEFT;
		}
		else
		{
			dist = rightDist;
			tmpSide = RIGHT;
		}
		if (minDist>dist)
		{
			minDist=dist;
			rlx=lpt.m_X;
			rly=lpt.m_Y;
			rrx=rpt.m_X;
			rry=rpt.m_Y;
			side = tmpSide;
		}
	}
}

void polyline::moveRef(double oldlx, double oldly, double oldrx, double oldry, 
	double newlx, double newly, double newrx, double newry)
{
	bool isChanged = false;
	double minDist=999;//比较几个在阈值中的最小值;
	bool isHaveMove = false;//是否有小于阈值的点;
	int minId;
	for (int i = 0; i < m_pLDominant->size();i++)
	{
		double leftDist = screenDist(oldlx, oldly, (*m_pLDominant)[i].m_X, (*m_pLDominant)[i].m_Y, LEFT, m_pDlg);
		double rightDist = screenDist(oldrx, oldry, (*m_pRDominant)[i].m_X, (*m_pRDominant)[i].m_Y,RIGHT,m_pDlg);
		if (leftDist < MoveRefThresh ||rightDist<MoveRefThresh)
		{
			isHaveMove = true;
			if (leftDist<=rightDist&&leftDist<minDist)
			{
				minDist = leftDist;
				minId = i;
			}
			else if (rightDist<leftDist&&rightDist<minDist)
			{
				minDist = rightDist;
				minId = i;
			}
		}
	}
	if (isHaveMove)
	{
		int i = minId;
		(*m_pLDominant)[i].m_X = newlx;
		(*m_pLDominant)[i].m_Y = newly;
		(*m_pRDominant)[i].m_X = newrx;
		(*m_pRDominant)[i].m_Y = newry;
		(*m_pLLinePoints)[i].m_X = newlx;
		(*m_pLLinePoints)[i].m_Y = newly;
		(*m_pRLinePoints)[i].m_X = newrx;
		(*m_pRLinePoints)[i].m_Y = newry;
		setAverZ();
		m_pLDominant->Normalize();
		m_pRDominant->Normalize();
		m_pLLinePoints->Normalize();
		m_pRLinePoints->Normalize();
		if (i == m_pLDominant->size() - 1 || i == m_pLDominant->size() - 2)
		{
			setMainDirection();
		}
	}
/*
	if (isChanged)
	{
		getLine();
	}*/
}

void polyline::getLine()
{
	m_pLLinePoints->clear();
	m_pRLinePoints->clear();
	for (int i = 0; i < m_pLDominant->size(); i++)
	{
		m_pLLinePoints->push_back((*m_pLDominant)[i]);
		m_pRLinePoints->push_back((*m_pRDominant)[i]);
	}
}

void polyline::endDrawing()
{
	if (m_isPreviewExisted)
	{
		m_isPreviewExisted = false;
		m_pRLinePoints->pop_back();
		m_pLLinePoints->pop_back();
	}
	//m_isDrawing = false;
}

void polyline::getCorrectPoint(double lx, double ly, double rx, double ry,
	double &newlx, double &newly, double &newrx, double &newry)
{
	switch (m_verticalMode)
	{
	case ON:
		if (m_isMainSet)
		{
			CPoint3D llastPoint = (*m_pLDominant)[m_pLDominant->size() - 1];
			CPoint3D rlastPoint = (*m_pRDominant)[m_pLDominant->size() - 1];
			double lastX, lastY, lastZ, oldX, oldY, oldZ;
			getRealCoor(llastPoint.m_X, llastPoint.m_Y, rlastPoint.m_X, rlastPoint.m_Y,
				m_pDlg, lastX, lastY, lastZ);
			lastZ = m_averZ;
			getRealCoor(lx, ly, rx, ry, m_pDlg, oldX, oldY, oldZ);
			double tmpdX = oldX - lastX;
			double tmpdY = oldY - lastY;
			double tmpdZ = oldZ - lastZ;
			double projection = tmpdX*m_mainX + tmpdY*m_mainY + tmpdZ*m_mainZ;
			tmpdX = m_mainX*projection;
			tmpdY = m_mainY*projection;
			tmpdZ = m_mainZ*projection;
			double newX = lastX + tmpdX;
			double newY = lastY + tmpdY;
			double newZ = lastZ + tmpdZ;
			getPointCoor(newX, newY, newZ, m_pDlg,
				newlx, newly, newrx, newry);
		}
		else
		{
			newlx = lx;
			newly = ly;
			newrx = rx;
			newry = ry;
		}
		break;
	case OFF:
		newlx = lx;
		newly = ly;
		newrx = rx;
		newry = ry;
		break;
	default:
		break;
	}
}

void polyline::setMainDirection()
{
	setAverZ();
	if (m_pLDominant->size()>=2)
	{
		CPoint3D llastPoint = (*m_pLDominant)[m_pLDominant->size() - 1];
		CPoint3D rlastPoint = (*m_pRDominant)[m_pLDominant->size() - 1];
		CPoint3D ll2Point = (*m_pLDominant)[m_pLDominant->size() - 2];
		CPoint3D rl2Point = (*m_pRDominant)[m_pLDominant->size() - 2];
		double lastX, lastY, lastZ, last2X, last2Y, last2Z;
		getRealCoor(llastPoint.m_X, llastPoint.m_Y, rlastPoint.m_X, rlastPoint.m_Y,
			m_pDlg, lastX, lastY, lastZ);
		getRealCoor(ll2Point.m_X, ll2Point.m_Y, rl2Point.m_X, rl2Point.m_Y,
			m_pDlg, last2X, last2Y, last2Z);
		crossProduct(lastX - last2X, lastY - last2Y, 0, 0, 0, 1,
			m_mainX, m_mainY, m_mainZ);
		double length = sqrt(m_mainX*m_mainX + m_mainY*m_mainY + m_mainZ*m_mainZ);
		m_mainX /= length;
		m_mainY /= length;
		m_mainZ /= length;
		m_isMainSet = true;
	}
}

void polyline::setAverZ()
{
	double sumZ = 0;
	for (int i = 0; i < m_pLDominant->size();i++)
	{
		double X, Y, Z;
		getRealCoor((*m_pLDominant)[i].m_X, (*m_pLDominant)[i].m_Y,
			(*m_pRDominant)[i].m_X, (*m_pRDominant)[i].m_Y, m_pDlg,
			X, Y, Z);
		sumZ += Z;
	}
	m_averZ= sumZ / (m_pLDominant->size());
}

void* polyline::getData()
{
	int n=m_pLDominant->size();
	polylineData* tpLineData=new polylineData(n);
	for (int i=0;i<n;i++)
	{
		tpLineData->m_dominantlx[i]=(*m_pLDominant)[i].m_X;
		tpLineData->m_dominantly[i]=(*m_pLDominant)[i].m_Y;
		tpLineData->m_dominantrx[i]=(*m_pRDominant)[i].m_X;
		tpLineData->m_dominantry[i]=(*m_pRDominant)[i].m_Y;
		double X,Y,Z;
		getRealCoor((*m_pLDominant)[i].m_X,(*m_pLDominant)[i].m_Y,
			(*m_pRDominant)[i].m_X,(*m_pRDominant)[i].m_Y,m_pDlg,
			X,Y,Z);
		tpLineData->m_dominantX[i]=X;
		tpLineData->m_dominantY[i]=Y;
		tpLineData->m_dominantZ[i]=Z;
	}
	return tpLineData;
}