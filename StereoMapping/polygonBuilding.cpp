#include "StdAfx.h"
#include "polygonBuilding.h"
#include "polyline.h"


polygonBuilding::polygonBuilding(CStereoViewDlg* pDlg)
:m_pDlg(pDlg)
{
	m_editedSide=NULL;
	m_isRoofPointSet=false;
	m_isSideLineSet=false;
}

void polygonBuilding::moveMouse(double lx,double ly,double rx,double ry)
{
}

void polygonBuilding::leftButtonDown(double lx,double ly,double rx,double ry)
{
}

void polygonBuilding::leftButtonUp(double lx,double ly,double rx,double ry)
{
}


void polygonBuilding::addRoofEntity(entity*roofEntity)
{
	m_roofEntity.push_back(roofEntity);
}

bool polygonBuilding::setRoofPoint(double lx,double ly,double rx,double ry,
	CEditLine* sideLeftLine,CEditLine* sideRightLine)
{
	double rlx,rly,rrx,rry,minDist;
	minDist=100;
	for (int i=0;i<m_roofEntity.size();i++)
	{
		double tmprlx,tmprly,tmprrx,tmprry,tmpminDist;
		NearestSide side;
		m_roofEntity[i]->getNearestRef(lx,ly,rx,ry,
			tmprlx,tmprly,tmprrx,tmprry,tmpminDist,side);
		if (minDist>tmpminDist)
		{
			minDist=tmpminDist;
			rlx=tmprlx;
			rly=tmprly;
			rrx=tmprrx;
			rry=tmprry;
		}
	}
	if (minDist<RoofPointThresh)
	{
		m_editedSide=new polyline(sideLeftLine,sideRightLine,m_pDlg,m_verticalMode);
		entityAlone* editedSide=static_cast<entityAlone*>(m_editedSide);
		editedSide->addDominantPoint(rlx,rly,rrx,rry);
		m_isRoofPointSet=true;
		return true;
	}
	else
	{
		return false;
	}
}

bool polygonBuilding::isBuildingFinish()
{
	if (m_roofEntity.size()>0&&m_isSideLineSet)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void polygonBuilding::resetRoofPoint()
{
	m_isRoofPointSet=false;
	if (m_editedSide)
	{
		polyline* pl = static_cast<polyline*>(m_editedSide);
		pl->m_pLLinePoints->clear();
		pl->m_pRLinePoints->clear();
		delete m_editedSide;
		m_editedSide=NULL;
	}
}

void polygonBuilding::setBottomPoint(double lx,double ly,double rx,double ry)
{
	if (m_isRoofPointSet&&m_editedSide)
	{
		entityAlone* editedSide=static_cast<entityAlone*>(m_editedSide);
		editedSide->addDominantPoint(lx,ly,rx,ry);
		m_isSideLineSet=true;
		m_isRoofPointSet=false;
		m_sideLines.push_back(m_editedSide);
		m_editedSide = NULL;
	}
}

void polygonBuilding::setBottomPtReview(double lx,double ly,double rx,double ry)
{
	if (m_isRoofPointSet&&m_editedSide)
	{
		entityAlone* editedSide=static_cast<entityAlone*>(m_editedSide);
		editedSide->addPreviewPoint(lx,ly,rx,ry);
	}
}

void polygonBuilding::getNearestRef(double lx, double ly, double rx, double ry,
	double &rlx, double &rly, double &rrx, double &rry, double &minDist,NearestSide& side)
{
	minDist = 999;
	for (int i = 0; i < m_roofEntity.size();i++)
	{
		double tprlx, tprly, tprrx, tprry, tpminDist;
		NearestSide tpside;
		m_roofEntity[i]->getNearestRef(lx, ly, rx, ry,
			tprlx, tprly, tprrx, tprry, tpminDist, tpside);
		if (minDist>tpminDist)
		{
			minDist = tpminDist;
			rlx = tprlx;
			rly = tprly;
			rrx = tprrx;
			rry = tprry;
			side = tpside;
		}
	}
	for (int i = 0; i < m_sideLines.size();i++)
	{
		double tprlx, tprly, tprrx, tprry, tpminDist;
		NearestSide tpside;
		m_sideLines[i]->getNearestRef(lx, ly, rx, ry,
			tprlx, tprly, tprrx, tprry, tpminDist, tpside);
		if (minDist>tpminDist)
		{
			minDist = tpminDist;
			rlx = tprlx;
			rly = tprly;
			rrx = tprrx;
			rry = tprry;
			side = tpside;
		}
	}
	if (m_editedSide)
	{
		double tprlx, tprly, tprrx, tprry, tpminDist;
		NearestSide tpside;
		m_editedSide->getNearestRef(lx, ly, rx, ry,
			tprlx, tprly, tprrx, tprry, tpminDist, tpside);
		if (minDist > tpminDist)
		{
			minDist = tpminDist;
			rlx = tprlx;
			rly = tprly;
			rrx = tprrx;
			rry = tprry;
			side = tpside;
		}
	}
}

void polygonBuilding::rightButtonDown(double lx, double ly, double rx, double ry)
{}

polygonBuilding::~polygonBuilding(void)
{
	for (int i=0;i<m_roofEntity.size();i++)
	{
		delete m_roofEntity[i];
	}
	for (int i=0;i<m_sideLines.size();i++)
	{
		delete m_sideLines[i];
	}
}

void polygonBuilding::endDrawing()
{}

void polygonBuilding::moveRef(double oldlx, double oldly, double oldrx, double oldry,
	double newlx, double newly, double newrx, double newry)
{
	for (int i = 0; i < m_roofEntity.size();i++)
	{
		m_roofEntity[i]->moveRef(oldlx, oldly, oldrx, oldry,
			newlx, newly, newrx, newry);
	}
	for (int i = 0; i < m_sideLines.size();i++)
	{
		m_sideLines[i]->moveRef(oldlx, oldly, oldrx, oldry,
			newlx, newly, newrx, newry);
	}
	if (m_editedSide)
	{
		m_editedSide->moveRef(oldlx, oldly, oldrx, oldry,
			newlx, newly, newrx, newry);
	}
}

void polygonBuilding::clearData()
{
	for (int i=0;i<m_roofEntity.size();i++)
	{
		m_roofEntity[i]->clearData();
		delete m_roofEntity[i];
	}
	for (int i=0;i<m_sideLines.size();i++)
	{
		m_sideLines[i]->clearData();
		delete m_sideLines[i];
	}
	m_roofEntity.clear();
	m_sideLines.clear();
}