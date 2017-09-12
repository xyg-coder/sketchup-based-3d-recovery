#include "StdAfx.h"
#include "arcSing.h"
#include "singleImageView.h"
#include <Eigen/Dense>

arcSing::arcSing(GraphicLayer* pPointLayer, GraphicLayer* pLineLayer,
	singleImageView* pView, directionSing* verticalD)
	:entitySing(pPointLayer, pLineLayer, pView, verticalD)
{
	m_isDrawing = true;
	m_linePoints = 0;
}

arcSing::arcSing(GraphicLayer* pPointLayer, GraphicLayer* pLineLayer,
	singleImageView* pView, lastPointSing* lastP, directionSing* verticalD)
	:entitySing(pPointLayer, pLineLayer, pView, lastP, verticalD)
{
	m_line = (GraphicPolyline*)m_pLineLayer->CreateGraphicObject(GO_POLYLINE);
	m_pLineLayer->AddGraphicObject(m_line, LineColor);
	GraphicPoint* pt = (GraphicPoint*)m_pPointLayer->CreateGraphicObject(GO_POINT);
	m_pPointLayer->AddGraphicObject(pt, PointColor);
	double x, y;
	m_lastPoint->getLastPoint(x, y);
	pt->SetPos(x, y);
	//m_pPointLayer->AddGraphicObject(pt,PointColor);
	m_dominantPoints.push_back(pt);
	m_isPreviewExisted = false;
	m_tmpPointSize = 1;
	m_previewIndex = 1;
	m_isDrawing = true;
	m_linePoints = 0;
}

arcSing::~arcSing()
{
}

void arcSing::moveMouse(double x, double y)
{
	if (m_tmpPointSize>0)
	{
		addPreviewPoint(x, y);
	}
}

void arcSing::leftButtonUp(double x, double y)
{
	addDominantPoint(x, y);
}

void arcSing::rightButtonDown(double x, double y)
{
	endDrawing();
}

void arcSing::getNearestRef(double x, double y,
	double &resultx, double &resulty, double &minDist)
{
	minDist = 999;
	if (m_tmpPointSize<=2)
	{
		return;
	}
	for (int i = 0; i < m_tmpPointSize;i++)
	{
		GraphicPoint* pt = m_dominantPoints[i];
		double ptx, pty;
		pt->GetPos(ptx, pty);
		double dist = screenDist(x, y, ptx, pty, m_pView);
		if (dist < minDist)
		{
			minDist = dist;
			resultx = ptx;
			resulty = pty;
		}
	}
	/*
	else if (m_isDrawing)
	{
		GraphicPoint* pt = m_dominantPoints[0];
		double ptx, pty;
		pt->GetPos(ptx, pty);
		double dist=screenDist(x, y, ptx, pty, m_pView);
		if (dist<minDist)
		{
			minDist = dist;
			resultx = ptx;
			resulty = pty;
		}
	}
	else
	{
		GraphicPoint* pt = m_dominantPoints[0];
		double ptx, pty;
		pt->GetPos(ptx, pty);
		double dist = screenDist(x, y, ptx, pty, m_pView);
		if (dist < minDist)
		{
			minDist = dist;
			resultx = ptx;
			resulty = pty;
		}
		
		pt = m_dominantPoints[m_tmpPointSize - 1];
		pt->GetPos(ptx, pty);
		dist = screenDist(x, y, ptx, pty, m_pView);
		if (dist < minDist)
		{
			minDist = dist;
			resultx = ptx;
			resulty = pty;
		}
	}
*/
}

void arcSing::moveRef(double oldx, double oldy, double newx, double newy)
{
	for (int i = 0; i < m_tmpPointSize;i++)
	{
		GraphicPoint* pt = m_dominantPoints[i];
		double ptx, pty;
		pt->GetPos(ptx, pty);
		double dist = screenDist(oldx, oldy, ptx, pty, m_pView);
		if (dist < MOVEREFTHRESH)
		{
			pt->SetPos(newx, newy);
			calculateArc();
			calculateLinePoins();
			break;
		}
	}
	/*if (m_isDrawing)
	{
		GraphicPoint* pt = m_dominantPoints[0];
		double ptx, pty;
		pt->GetPos(ptx, pty);
		double dist = screenDist(oldx, oldy, ptx, pty, m_pView);
		if (dist<MOVEREFTHRESH)
		{
			pt->SetPos(newx, newy);
			calculateArc();
			calculateLinePoins();
		}
	}
	else
	{
		GraphicPoint* pt = m_dominantPoints[0];
		double ptx, pty;
		pt->GetPos(ptx, pty);
		double dist = screenDist(oldx, oldy, ptx, pty, m_pView);
		if (dist < MOVEREFTHRESH)
		{
			pt->SetPos(newx, newy);
			calculateArc();
			calculateLinePoins();
		}
		else
		{
			pt = m_dominantPoints[m_tmpPointSize - 1];
			pt->GetPos(ptx, pty);
			dist = screenDist(oldx, oldy, ptx, pty, m_pView);
			if (dist<MOVEREFTHRESH)
			{
				pt->SetPos(newx, newy);
				calculateArc();
				calculateLinePoins();
			}
		}
	}*/
}

void arcSing::addDominantPoint(double x, double y)
{
	if (!m_line)
	{
		m_line = (GraphicPolyline*)m_pLineLayer->CreateGraphicObject(GO_POLYLINE);
		m_pLineLayer->AddGraphicObject(m_line, LineColor);
	}
	if (m_linePoints>0)
	{
		m_line->DeleteVertexs(0, m_linePoints);
		m_linePoints = 0;
	}
	m_isPreviewExisted = false;
	GraphicPoint* pt = (GraphicPoint*)m_pPointLayer->CreateGraphicObject(GO_POINT);
	m_pPointLayer->AddGraphicObject(pt, PointColor);
	pt->SetPos(x, y);
	m_dominantPoints.push_back(pt);
	m_tmpPointSize++;
	if (m_tmpPointSize>=3)
	{
		calculateArc();
		calculateLinePoins();
	}
}

void arcSing::addPreviewPoint(double x, double y)
{
	if (!m_line || m_tmpPointSize <2)
	{
		return;
	}
	if (m_isPreviewExisted&&m_linePoints>0)
	{
		m_line->DeleteVertexs(0, m_linePoints);
		m_linePoints = 0;
	}
	m_previewx = x;
	m_previewy = y;
	m_isPreviewExisted = true;
	calculateArc();
	calculateLinePoins();
}

void arcSing::setVertical(directionSing verticalD,double x0,double y0,double z0)
{
	m_A = verticalD.m_dx;
	m_B = verticalD.m_dy;
	m_C = verticalD.m_dz;
	m_D = -(m_A*x0 + m_B*y0 + m_C*z0);
}

void arcSing::calculateArc()
{
	double x1, y1, x2, y2, x3, y3;
	if (m_isPreviewExisted&&m_tmpPointSize >= 2)
	{
		int midIndex = m_tmpPointSize / 2;
		x3 = m_previewx;
		y3 = m_previewy;
		m_dominantPoints[0]->GetPos(x1, y1);
		m_dominantPoints[midIndex]->GetPos(x2, y2);
	}
	else if (m_tmpPointSize >= 3)
	{
		int midIndex = m_tmpPointSize / 2;
		m_dominantPoints[0]->GetPos(x1, y1);
		m_dominantPoints[midIndex]->GetPos(x2, y2);
		m_dominantPoints[m_tmpPointSize - 1]->GetPos(x3, y3);
	}
	else
	{
		return;
	}
	getRealCoor(x1, y1, m_pView, m_X[0], m_Y[0], m_Z[0]);
	getRealCoor(x2, y2, m_pView, m_X[1], m_Y[1], m_Z[1]);
	getRealCoor(x3, y3, m_pView, m_X[2], m_Y[2], m_Z[2]);
	m_averZ = (m_Z[0] + m_Z[1] + m_Z[2]) / 3;
	m_averX = (m_X[0] + m_X[1] + m_X[2]) / 3;
	m_averY = (m_Y[0] + m_Y[1] + m_Y[2]) / 3;
	for (int i = 0; i < 3; i++)
	{
		m_X[i] -= m_averX;
		m_Y[i] -= m_averY;
		m_Z[i] -= m_averZ;
	}
	//此刻求出的平面是归一化之后的平面;
	setVertical(*m_verticalDirection, m_X[0], m_Y[0], m_Z[0]);
	double A2 = 2 * (m_X[1] - m_X[0]);
	double B2 = 2 * (m_Y[1] - m_Y[0]);
	double C2 = 2 * (m_Z[1] - m_Z[0]);
	double D2 = m_X[0] * m_X[0] + m_Y[0] * m_Y[0] + m_Z[0] * m_Z[0] -
		(m_X[1] * m_X[1] + m_Y[1] * m_Y[1] + m_Z[1] * m_Z[1]);
	double A3 = 2 * (m_X[2] - m_X[0]);
	double B3 = 2 * (m_Y[2] - m_Y[0]);
	double C3 = 2 * (m_Z[2] - m_Z[0]);
	double D3 = m_X[0] * m_X[0] + m_Y[0] * m_Y[0] + m_Z[0] * m_Z[0] -
		(m_X[2] * m_X[2] + m_Y[2] * m_Y[2] + m_Z[2] * m_Z[2]);
	Eigen::Matrix3d M;
	M << m_A, m_B, m_C,
		A2, B2, C2,
		A3, B3, C3;
	Eigen::Vector3d D;
	D << m_D, D2, D3;
	Eigen::MatrixXd result = -M.inverse()*D;
	m_cx = result(0);
	m_cy = result(1);
	m_cz = result(2);
	m_r = sqrt((m_cx - m_X[0])*(m_cx - m_X[0]) + (m_cy - m_Y[0])*(m_cy - m_Y[0]) +
		(m_cz - m_Z[0])*(m_cz - m_Z[0]));
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

void arcSing::calculateLinePoins()
{
	if (m_linePoints!=0)
	{
		m_line->DeleteVertexs(0, m_linePoints);
		m_linePoints = 0;
	}
	double firstx, firsty;
	m_dominantPoints[0]->GetPos(firstx, firsty);
	m_line->AddVertex(0, firstx, firsty);
	m_linePoints = 1;
	bool isReverse;//是否是逆时针;
	double fai1 = atan2(m_Y[0] - m_cy, m_X[0] - m_cx);
	if (fai1 < 0)
	{
		fai1 = 2 * PI + fai1;
	}
	double fai2 = atan2(m_Y[1] - m_cy, m_X[1] - m_cx);
	if (fai2 < 0)
	{
		fai2 = 2 * PI + fai2;
	}
	double fai3 = atan2(m_Y[2] - m_cy, m_X[2] - m_cx);
	if (fai3 < 0)
	{
		fai3 = 2 * PI + fai3;
	}
	while (fai2 < fai1)
	{
		fai2 += 2 * PI;
	}
	while (fai3<fai2)
	{
		fai3 += 2 * PI;
	}
	if (fai3 - fai1>2 * PI)
	{
		isReverse = false;//保证每一点弧度比上一点小;
		while (fai2 > fai1)
		{
			fai2 -= 2 * PI;
		}
		while (fai3 > fai2)
		{
			fai3 -= 2 * PI;
		}
	}
	else
	{
		isReverse = true;//保证每一点弧度比上一点大;
	}
	double radioStep;
	if (isReverse)
	{
		radioStep = RADIOSTEP;
	}
	else
	{
		radioStep = -RADIOSTEP;
	}
	m_fai=fai3-fai1;
	for (double fai = fai1 + radioStep; true; fai += radioStep)
	{
		if ((isReverse&&fai >= fai3) || ((!isReverse) && fai <= fai3))
		{
			double x, y;
			getPointCoor(m_X[2], m_Y[2], m_Z[2],m_pView,x,y);
			m_linePoints++;
			m_line->AddVertex(m_linePoints-1,x,y);
			break;
		}
		double X = m_cx + m_r*cos(fai);
		double Y = m_cy + m_r*sin(fai);
		double Z = m_averZ;
		double x, y;
		getPointCoor(X, Y, Z, m_pView, x, y);
		m_linePoints++;
		m_line->AddVertex(m_linePoints-1, x, y);
	}
}

void arcSing::endDrawing()
{
	if (m_isPreviewExisted)
	{
		m_isPreviewExisted = false;
		calculateArc();
		calculateLinePoins();
	}
	/*if (m_tmpPointSize > 2)
	{
		m_pPointLayer->AddGraphicObject(m_dominantPoints[0], PointColor);
		//m_pPointLayer->AddGraphicObject(m_dominantPoints[m_tmpPointSize / 2], PointColor);
		m_pPointLayer->AddGraphicObject(m_dominantPoints[m_tmpPointSize - 1], PointColor);
	}*/
	m_isDrawing = false;
}

void* arcSing::getData()
{
	arcSingData* pArcData=new arcSingData;
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
	pArcData->m_arc=m_fai;
	return pArcData;
}

bool arcSing::canEndDrawing()
{
	if (m_dominantPoints.size()>=3)
	{
		return true;
	}
	else
	{
		return false;
	}
}