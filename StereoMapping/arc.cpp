#include "StdAfx.h"
#include "arc.h"
#include "render.h"
#include "StereoViewDlg.h"
#include "AerialBase.h"

double lineDist(double x1, double y1, double x2, double y2, double x, double y)
{
	double k = (y2 - y1) / (x2 - x1);
	double d = fabs(k*x - y - k*x1 + y1) / sqrt(k*k + 1);
	return d;
}

arc::arc(CEditLine* pLLine,CEditLine* pRLine,CStereoViewDlg* pDlg)
	:entityAlone(pLLine,pRLine),m_pDlg(pDlg)
{
	m_isDrawing = true;
}

arc::arc(CEditLine* pLLine,CEditLine* pRLine,CStereoViewDlg* pDlg,
	lastPoint* lastPt)
	:entityAlone(pLLine,pRLine,
	lastPt),m_pDlg(pDlg)
{
	m_isDrawing = true;
}

void arc::moveMouse(double lx,double ly,double rx,double ry)
{
	if (m_pLDominant->size()>0)
	{
		addPreviewPoint(lx,ly,rx,ry);
	}
}

void arc::leftButtonDown(double lx,double ly,double rx,double ry)
{
}

void arc::leftButtonUp(double lx,double ly,double rx,double ry)
{
	addDominantPoint(lx,ly,rx,ry);
}

void arc::rightButtonDown(double lx,double ly,double rx,double ry)
{
	if (m_pLDominant->size()<3)
	{
		return;
	}
	endDrawing();
}

void arc::addDominantPoint(double lx,double ly,double rx,double ry)
{
	if (m_isPreviewExisted)
	{
		m_isPreviewExisted=false;
	}
	CPoint3D lpt,rpt;
	lpt.m_X=lx;
	lpt.m_Y=ly;
	lpt.m_Z=0;
	rpt.m_X=rx;
	rpt.m_Y=ry;
	rpt.m_Z=0;
	m_pLDominant->push_back(lpt);
	m_pRDominant->push_back(rpt);
	if (m_pLDominant->size()>=3)
	{
		calculateArc(m_pDlg);
		calculateLinePoins();
	}
}

void arc::addPreviewPoint(double lx,double ly,double rx,double ry)
{
	if (m_pLDominant->size()<2)
	{
		return;
	}
	m_isPreviewExisted=true;
	m_previewLx=lx;
	m_previewLy=ly;
	m_previewRx=rx;
	m_previewRy=ry;
	calculateArc(m_pDlg);
	calculateLinePoins();
}


void arc::calculateArc(CStereoViewDlg* pDlg)
{
	CPoint3D lpt1,rpt1,lpt2,rpt2,lpt3,rpt3;
	if (m_isPreviewExisted&&m_pLDominant->size()>=2)
	{
		int midIndex;
		midIndex=(m_pLDominant->size())/2;
		lpt1=(*m_pLDominant)[0];
		rpt1=(*m_pRDominant)[0];
		lpt2=(*m_pLDominant)[midIndex];
		rpt2=(*m_pRDominant)[midIndex];
		lpt3.m_X=m_previewLx;
		lpt3.m_Y=m_previewLy;
		lpt3.m_Z=0;
		rpt3.m_X=m_previewRx;
		rpt3.m_Y=m_previewRy;
		rpt3.m_Z=0;
	}
	else if (m_pLDominant->size()>=3)
	{
		int midIndex=(m_pLDominant->size()-1)/2;
		lpt1=(*m_pLDominant)[0];
		rpt1=(*m_pRDominant)[0];
		lpt2=(*m_pLDominant)[midIndex];
		rpt2=(*m_pRDominant)[midIndex];
		lpt3=(*m_pLDominant)[m_pLDominant->size()-1];
		rpt3=(*m_pRDominant)[m_pLDominant->size()-1];
	}
	else
	{
		return;
	}
	getRealCoor(lpt1.m_X,lpt1.m_Y,rpt1.m_X,rpt1.m_Y,m_pDlg,m_X[0],m_Y[0],m_Z[0]);
	getRealCoor(lpt2.m_X,lpt2.m_Y,rpt2.m_X,rpt2.m_Y,m_pDlg,m_X[1],m_Y[1],m_Z[1]);
	getRealCoor(lpt3.m_X,lpt3.m_Y,rpt3.m_X,rpt3.m_Y,m_pDlg,m_X[2],m_Y[2],m_Z[2]);
	m_averZ = (m_Z[0] + m_Z[1] + m_Z[2]) / 3;
	m_averX = (m_X[0] + m_X[1] + m_X[2]) / 3;
	m_averY = (m_Y[0] + m_Y[1] + m_Y[2]) / 3;
	for (int i = 0; i < 3;i++)
	{
		m_X[i] -= m_averX;
		m_Y[i] -= m_averY;
		m_Z[i] -= m_averZ;
	}
	setVertical();
	double A2=2*(m_X[1]-m_X[0]);
	double B2=2*(m_Y[1]-m_Y[0]);
	double C2=2*(m_Z[1]-m_Z[0]);
	double D2=m_X[0]*m_X[0]+m_Y[0]*m_Y[0]+m_Z[0]*m_Z[0]-
		(m_X[1]*m_X[1]+m_Y[1]*m_Y[1]+m_Z[1]*m_Z[1]);
	double A3=2*(m_X[2]-m_X[0]);
	double B3=2*(m_Y[2]-m_Y[0]);
	double C3=2*(m_Z[2]-m_Z[0]);
	double D3=m_X[0]*m_X[0]+m_Y[0]*m_Y[0]+m_Z[0]*m_Z[0]-
		(m_X[2]*m_X[2]+m_Y[2]*m_Y[2]+m_Z[2]*m_Z[2]);
	Eigen::Matrix3d M;
	M<<m_A,m_B,m_C,
		A2,B2,C2,
		A3,B3,C3;
	Eigen::Vector3d D;
	D<<m_D,D2,D3;
	Eigen::MatrixXd result=-M.inverse()*D;
	m_cx=result(0);
	m_cy=result(1);
	m_cz=result(2);
	m_r=sqrt((m_cx-m_X[0])*(m_cx-m_X[0])+(m_cy-m_Y[0])*(m_cy-m_Y[0])+
		(m_cz-m_Z[0])*(m_cz-m_Z[0]));
	m_cx += m_averX;
	m_cy += m_averY;
	m_cz += m_averZ;
	for (int i = 0; i < 3; i++)
	{
		m_X[i] += m_averX;
		m_Y[i] += m_averY;
		m_Z[i] += m_averZ;
	}
}

void arc::setVertical()
{
	m_A=m_Y[0]*m_Z[1]-m_Y[0]*m_Z[2]-m_Z[0]*m_Y[1]+m_Z[0]*m_Y[2]+m_Y[1]*m_Z[2]-m_Y[2]*m_Z[1];
	m_B=-m_X[0]*m_Z[1]+m_X[0]*m_Z[2]+m_Z[0]*m_X[1]-m_Z[0]*m_X[2]-m_X[1]*m_Z[2]+m_X[2]*m_Z[1];
	m_C=m_X[0]*m_Y[1]-m_X[0]*m_Y[2]-m_Y[0]*m_X[1]+m_Y[0]*m_X[2]+m_X[1]*m_Y[2]-m_X[2]*m_Y[1];
	m_D=-m_X[0]*m_Y[1]*m_Z[2]+m_X[0]*m_Y[2]*m_Z[1]+m_X[1]*m_Y[0]*m_Z[2]-m_X[2]*m_Y[0]*m_Z[1]-
		m_X[1]*m_Y[2]*m_Z[0]+m_X[2]+m_Y[1]*m_Z[0];
}

void arc::calculateLinePoins()
{
	m_pLLinePoints->clear();
	m_pRLinePoints->clear();
	m_pLLinePoints->push_back((*m_pLDominant)[0]);
	m_pRLinePoints->push_back((*m_pRDominant)[0]);
	bool isReverse;//是否是逆时针;
	double fai1=atan2(m_Y[0]-m_cy,m_X[0]-m_cx);
	if (fai1<0)
	{
		fai1=2*PI+fai1;
	}
	double fai2=atan2(m_Y[1]-m_cy,m_X[1]-m_cx);
	if (fai2<0)
	{
		fai2=2*PI+fai2;
	}
	double fai3=atan2(m_Y[2]-m_cy,m_X[2]-m_cx);
	if (fai3<0)
	{
		fai3=2*PI+fai3;
	}
	while(fai2<fai1)
	{
		fai2+=2*PI;
	}
	while(fai3<fai2)
	{
		fai3+=2*PI;
	}
	if (fai3-fai1>2*PI)
	{
		isReverse=false;//保证每一点弧度比上一点小;
		while(fai2>fai1)
		{
			fai2-=2*PI;
		}
		while(fai3>fai2)
		{
			fai3-=2*PI;
		}
	}
	else
	{
		isReverse=true;//保证每一点弧度比上一点大;
	}
	double radioStep;
	if (isReverse)
	{
		radioStep=RADIOSTEP;
	}
	else
	{
		radioStep=-RADIOSTEP;
	}
	m_fai=fai3-fai1;
	for (double fai=fai1+radioStep;true;fai+=radioStep)
	{
		if ((isReverse&&fai>=fai3)||((!isReverse)&&fai<=fai3))
		{
			CPoint3D lpt, rpt;
			getPointCoor(m_X[2], m_Y[2], m_Z[2], m_pDlg, lpt.m_X, lpt.m_Y, rpt.m_X, rpt.m_Y);
			lpt.m_Z = rpt.m_Z = 0;
			m_pLLinePoints->push_back(lpt);
			m_pRLinePoints->push_back(rpt);
			m_pLLinePoints->Normalize();
			m_pRLinePoints->Normalize();
			break;
		}
		double X=m_cx+m_r*cos(fai);
		double Y=m_cy+m_r*sin(fai);
		double Z=m_averZ;
		CPoint3D lpt,rpt;
		getPointCoor(X,Y,Z,m_pDlg,lpt.m_X,lpt.m_Y,rpt.m_X,rpt.m_Y);
		lpt.m_Z=rpt.m_Z=0;
		m_pLLinePoints->push_back(lpt);
		m_pRLinePoints->push_back(rpt);
		m_pLLinePoints->Normalize();
		m_pRLinePoints->Normalize();
	}
}

void arc::endDrawing()
{
	if (m_isPreviewExisted)
	{
		m_isPreviewExisted = false;
		calculateArc(m_pDlg);
		calculateLinePoins();
	}
	m_isDrawing = false;
}

//返回linePoint点中距离该点最近的点(中间点均不计算)
void arc::getNearestRef(double lx, double ly, double rx,
	double ry, double &rlx, double &rly, double &rrx, double &rry,
	double &minDist, NearestSide& side)
{
	minDist = 999;
	if (m_pLLinePoints->size()<2)
	{
		return;
	}
	else if (m_isDrawing)
	{
		int i = 0;
		NearestSide tmpSide;
		CPoint3D lpt = (*m_pLDominant)[i];
		CPoint3D rpt = (*m_pRDominant)[i];
		double leftDist = screenDist(lx, ly, lpt.m_X, lpt.m_Y,LEFT,m_pDlg);
		double rightDist = screenDist(rx, ry, rpt.m_X, rpt.m_Y, RIGHT, m_pDlg);
		double dist;
		if (leftDist < rightDist)
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
			minDist = dist;
			rlx = lpt.m_X;
			rly = lpt.m_Y;
			rrx = rpt.m_X;
			rry = rpt.m_Y;
			side = tmpSide;
		}
	}
	else
	{
		int i = 0;
		NearestSide tmpSide;
		CPoint3D lpt = (*m_pLDominant)[i];
		CPoint3D rpt = (*m_pRDominant)[i];
		double leftDist = screenDist(lx, ly, lpt.m_X, lpt.m_Y, LEFT, m_pDlg);
		double rightDist = screenDist(rx, ry, rpt.m_X, rpt.m_Y, RIGHT, m_pDlg);
		double dist;
		if (leftDist < rightDist)
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
			minDist = dist;
			rlx = lpt.m_X;
			rly = lpt.m_Y;
			rrx = rpt.m_X;
			rry = rpt.m_Y;
			side = tmpSide;
		}

		i = m_pLDominant->size() - 1;
		lpt = (*m_pLDominant)[i];
		rpt = (*m_pRDominant)[i];
		leftDist = screenDist(lx, ly, lpt.m_X, lpt.m_Y, LEFT, m_pDlg);
		rightDist = screenDist(rx, ry, rpt.m_X, rpt.m_Y, RIGHT, m_pDlg);
		if (leftDist < rightDist)
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
			minDist = dist;
			rlx = lpt.m_X;
			rly = lpt.m_Y;
			rrx = rpt.m_X;
			rry = rpt.m_Y;
			side = tmpSide;
		}
	}
	/*for (int i = 0; i < m_pLLinePoints->size();i++)
	{
		NearestSide tmpSide;
		CPoint3D lpt = (*m_pLLinePoints)[i];
		CPoint3D rpt = (*m_pRLinePoints)[i];
		double leftDist = pointDist(lx, ly, lpt.m_X, lpt.m_Y);
		double rightDist = pointDist(rx, ry, rpt.m_X, rpt.m_Y);
		double dist;
		if (leftDist < rightDist)
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
			minDist = dist;
			rlx = lpt.m_X;
			rly = lpt.m_Y;
			rrx = rpt.m_X;
			rry = rpt.m_Y;
			side = tmpSide;
		}
	}*/
}

void arc::moveRef(double oldlx, double oldly, double oldrx, double oldry,
	double newlx, double newly, double newrx, double newry)
{
	int dominantSize = m_pLDominant->size();
	if (screenDist(oldlx, oldly, (*m_pLDominant)[0].m_X, (*m_pLDominant)[0].m_Y,LEFT,m_pDlg)<MoveRefThresh||
		screenDist(oldrx, oldry, (*m_pRDominant)[0].m_X, (*m_pRDominant)[0].m_Y,RIGHT,m_pDlg)<MoveRefThresh)
	{
		(*m_pLDominant)[0].m_X = newlx;
		(*m_pLDominant)[0].m_Y = newly;
		(*m_pRDominant)[0].m_X = newrx;
		(*m_pRDominant)[0].m_Y = newry;
		m_pLDominant->Normalize();
		m_pRDominant->Normalize();
		calculateArc(m_pDlg);
		calculateLinePoins();
	}
	else if ((!m_isDrawing)&&(
		screenDist(oldlx, oldly, (*m_pLDominant)[dominantSize - 1].m_X, (*m_pLDominant)[dominantSize - 1].m_Y,LEFT,m_pDlg) < MoveRefThresh ||
				screenDist(oldrx, oldry, (*m_pRDominant)[dominantSize - 1].m_X, (*m_pRDominant)[dominantSize - 1].m_Y,RIGHT,m_pDlg) < MoveRefThresh)
		)
	{
		(*m_pLDominant)[dominantSize - 1].m_X = newlx;
		(*m_pLDominant)[dominantSize - 1].m_Y = newly;
		(*m_pRDominant)[dominantSize - 1].m_X = newrx;
		(*m_pRDominant)[dominantSize - 1].m_Y = newry;
		m_pLDominant->Normalize();
		m_pRDominant->Normalize();
		calculateArc(m_pDlg);
		calculateLinePoins();
	}
	/*else
	{
		int pointSize = m_pLLinePoints->size();
		for (int i = pointSize / 2; i < pointSize / 4 * 3; i++)
		{
			if (lineDist((*m_pLLinePoints)[i].m_X, (*m_pLLinePoints)[i].m_Y, (*m_pLLinePoints)[i + 1].m_X, (*m_pLLinePoints)[i+1].m_Y, 
				oldlx,oldly)<MoveRefThresh||
				lineDist((*m_pRLinePoints)[i].m_X, (*m_pRLinePoints)[i].m_Y, (*m_pRLinePoints)[i + 1].m_X, (*m_pRLinePoints)[i + 1].m_Y,
				oldrx, oldry) < MoveRefThresh)
			{
				for (int j = 1; j < dominantSize - 1; j++)
				{
					(*m_pLDominant)[j].m_X = newlx;
					(*m_pLDominant)[j].m_Y = newly;
					(*m_pRDominant)[j].m_X = newrx;
					(*m_pRDominant)[j].m_Y = newry;
				}
				m_pLDominant->Normalize();
				m_pRDominant->Normalize();
				calculateArc(m_pDlg);
				calculateLinePoins();
				break;
			}
		}
	}*/
}

void* arc::getData()
{
	arcData* tpArcData=new arcData;
	tpArcData->m_centerX=m_cx;
	tpArcData->m_centerY=m_cy;
	tpArcData->m_centerZ=m_cz;
	double clx,cly,crx,cry;
	getPointCoor(m_cx,m_cy,m_cz,m_pDlg,clx,cly,crx,cry);
	tpArcData->m_centerlx=clx;
	tpArcData->m_centerly=cly;
	tpArcData->m_centerrx=crx;
	tpArcData->m_centerry=cry;
	for (int i=0;i<3;i++)
	{
		tpArcData->m_dominantPX[i]=m_X[i];
		tpArcData->m_dominantPY[i]=m_Y[i];
		tpArcData->m_dominantPZ[i]=m_Z[i];
		getPointCoor(m_X[i],m_Y[i],m_Z[i],m_pDlg,
			tpArcData->m_dominantPlx[i],tpArcData->m_dominantPly[i],
			tpArcData->m_dominantPrx[i],tpArcData->m_dominantPry[i]);
	}
	tpArcData->m_r=m_r;
	tpArcData->m_arc=m_fai;
	return tpArcData;
}

arc::~arc(void)
{
}

bool arc::canEntityFinish()
{
	if (m_pLDominant->size()>=3)
	{
		return true;
	}
	else
	{
		return false;
	}
}