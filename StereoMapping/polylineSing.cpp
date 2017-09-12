#include "StdAfx.h"
#include "polylineSing.h"


polylineSing::polylineSing(GraphicLayer* pPointLayer, GraphicLayer* pLineLayer,
	singleImageView* pView, directionSing* verticalD, verticalStateSing verticalS)
	:entitySing(pPointLayer, pLineLayer, pView, verticalD), m_verticalState(verticalS)
{
	m_isMainSet = false;
}

polylineSing::polylineSing(GraphicLayer* pPointLayer, GraphicLayer* pLineLayer,
	singleImageView* pView, lastPointSing* lastP, directionSing* verticalD,
	verticalStateSing verticalS)
	: entitySing(pPointLayer,pLineLayer,pView,lastP,verticalD),
	m_verticalState(verticalS)
{
	m_line = (GraphicPolyline*)m_pLineLayer->CreateGraphicObject(GO_POLYLINE);
	m_pLineLayer->AddGraphicObject(m_line, LineColor);
	GraphicPoint* pt = (GraphicPoint*)m_pPointLayer->CreateGraphicObject(GO_POINT);
	double x, y;
	m_lastPoint->getLastPoint(x, y);
	pt->SetPos(x, y);
	m_pPointLayer->AddGraphicObject(pt,PointColor);
	m_dominantPoints.push_back(pt);
	m_isPreviewExisted = false;
	m_tmpPointSize = 1;
	m_previewIndex = 1;
	m_lastPoint->getLastPoint(x, y);
	m_line->AddVertex(0, x, y);
	m_isMainSet = false;
}

polylineSing::~polylineSing()
{
}

void polylineSing::moveMouse(double x, double y)
{
	if (m_tmpPointSize>0)
	{
		addPreviewPoint(x, y);
	}
}

void polylineSing::leftButtonDown(double x, double y)
{}

void polylineSing::leftButtonUp(double x, double y)
{
	addDominantPoint(x, y);
}

void polylineSing::rightButtonDown(double x, double y)
{
	if (m_isPreviewExisted)
	{
		m_line->DeleteVertex(m_previewIndex);
	}
	m_isPreviewExisted = false;
}

void polylineSing::addDominantPoint(double x, double y)
{
	if (!m_line)
	{
		m_line = (GraphicPolyline*)m_pLineLayer->CreateGraphicObject(GO_POLYLINE);
		m_pLineLayer->AddGraphicObject(m_line, LineColor);
	}
	if (m_isPreviewExisted)
	{
		m_line->DeleteVertex(m_previewIndex);
	}
	m_isPreviewExisted = false;
	getCorrectPoint(x, y, x, y);
	m_line->AddVertex(m_tmpPointSize, x, y);
	GraphicPoint* pt = (GraphicPoint*)m_pPointLayer->CreateGraphicObject(GO_POINT);
	pt->SetPos(x, y);
	m_pPointLayer->AddGraphicObject(pt, PointColor);
	m_dominantPoints.push_back(pt);
	m_tmpPointSize++;
	setMainDirection();
	//setLastPoint(m_lastPoint);
}

void polylineSing::addPreviewPoint(double x, double y)
{
	if (!m_line||m_tmpPointSize==0)
	{
		return;
	}
	getCorrectPoint(x, y, x, y);
	m_previewx = x;
	m_previewy = y;
	m_previewIndex = m_tmpPointSize;
	if (m_isPreviewExisted)
	{
		//m_line->DeleteVertex(m_previewIndex);
		//m_line->AddVertex(m_previewIndex, x, y);
		m_line->ModifyVertex(m_previewIndex,x,y);
	}
	else
	{
		m_line->AddVertex(m_previewIndex, x, y);
	}
	/*getCorrectPoint(x, y, x, y);
	m_previewx = x;
	m_previewy = y;
	m_previewIndex = m_tmpPointSize;
	m_line->AddVertex(m_previewIndex, x, y);*/
	m_isPreviewExisted = true;
}

void polylineSing::getNearestRef(double x, double y, 
	double &resultx, double &resulty, double &minDist)
{
	minDist = 999;
	for (int i = 0; i < m_tmpPointSize;i++)
	{
		double ptx, pty;
		GraphicPoint* pt = m_dominantPoints[i];
		pt->GetPos(ptx, pty);
		double dist = screenDist(x, y, ptx, pty,m_pView);
		if (dist<minDist)
		{
			minDist = dist;
			resultx = ptx;
			resulty = pty;
		}
	}
}

void polylineSing::moveRef(double oldx, double oldy, double newx, double newy)
{
	bool isChanged = false;
	double minDist = 999;
	int minId;
	bool isHaveMove = false;//是否有小于阈值的点;
	for (int i = 0; i < m_tmpPointSize;i++)
	{
		double ptx, pty;
		m_dominantPoints[i]->GetPos(ptx, pty);
		double dist = screenDist(oldx, oldy, ptx, pty,m_pView);
		if (dist<MOVEREFTHRESH)
		{
			isHaveMove = true;
			if (dist<minDist)
			{
				minDist = dist;
				minId = i;
			}
		}
	}
	if (isHaveMove)
	{
		int i = minId;
		m_dominantPoints[i]->SetPos(newx, newy);
		m_line->ModifyVertex(i, newx, newy);
		if (i==m_tmpPointSize-1||i==m_tmpPointSize-2)
		{
			setMainDirection();
		}
	}
}

void polylineSing::endDrawing()
{
	if (m_isPreviewExisted)
	{
		m_isPreviewExisted = false;
		m_line->DeleteVertex(m_previewIndex);
	}
}

void polylineSing::getCorrectPoint(double x, double y, double &rx, double &ry)
{
	switch (m_verticalState)
	{
	case OFFSing:
		rx = x;
		ry = y;
		break;
	case ONSing:
		if (m_isMainSet)
		{
			double X, Y, Z;
			getRealCoor(x, y, m_pView, X, Y, Z);
			double lastX, lastY, lastZ, lastx, lasty;
			m_dominantPoints[m_tmpPointSize - 1]->GetPos(lastx, lasty);
			getRealCoor(lastx, lasty, m_pView, lastX, lastY, lastZ);
			double dX = X - lastX;
			double dY = Y - lastY;
			double dZ = Z - lastZ;
			double projection = dX*m_mainDirection.m_dx +
				dY*m_mainDirection.m_dy + dZ*m_mainDirection.m_dz;
			dX = m_mainDirection.m_dx*projection;
			dY = m_mainDirection.m_dy*projection;
			dZ = m_mainDirection.m_dz*projection;
			X = lastX + dX;
			Y = lastY + dY;
			Z = lastZ + dZ;
			getPointCoor(X, Y, Z, m_pView, rx, ry);
		}
		else
		{
			rx = x;
			ry = y;
		}
		break;
	default:
		break;
	}
}

void polylineSing::setMainDirection()
{
	if (m_tmpPointSize>=2)
	{
		double x1, x2, y1, y2;
		m_dominantPoints[m_tmpPointSize - 1]->GetPos(x2, y2);
		m_dominantPoints[m_tmpPointSize - 2]->GetPos(x1, y1);
		double X1, Y1, Z1, X2, Y2, Z2;
		getRealCoor(x1, y1, m_pView, X1, Y1, Z1);
		getRealCoor(x2, y2, m_pView, X2, Y2, Z2);
		double dX = X2 - X1;
		double dY = Y2 - Y1;
		double dZ = Z2 - Z1;
		m_verticalDirection->crossProduct(dX, dY, dZ,
			m_mainDirection.m_dx, m_mainDirection.m_dy, m_mainDirection.m_dz);
		m_isMainSet = true;
	}
	else
	{
		m_isMainSet = false;
		return;
	}
}

void* polylineSing::getData()
{
	polylineSingData* ppolylineData=new polylineSingData(m_tmpPointSize);
	for (int i=0;i<m_tmpPointSize;i++)
	{
		double x,y,X,Y,Z;
		m_dominantPoints[i]->GetPos(x,y);
		getRealCoor(x,y,m_pView,X,Y,Z);
		ppolylineData->m_dominantx[i]=x;
		ppolylineData->m_dominanty[i]=y;
		ppolylineData->m_dominantX[i]=X;
		ppolylineData->m_dominantY[i]=Y;
		ppolylineData->m_dominantZ[i]=Z;
	}
	return ppolylineData;
}