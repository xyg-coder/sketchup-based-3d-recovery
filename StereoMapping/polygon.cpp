#include "StdAfx.h"
#include "polygon.h"


polygon::polygon(CStereoViewDlg* pDlg,CEditLine* lcloseLine,CEditLine* rcloseLine)
	:m_pDlg(pDlg),m_pLLine(lcloseLine),m_pRLine(rcloseLine)
{
}


polygon::~polygon(void)
{
	for (int i=0;i<m_entities.size();i++)
	{
		delete m_entities[i];
	}
	m_entities.clear();
}

void polygon::moveMouse(double lx,double ly,double rx,double ry)
{
}

void polygon::leftButtonDown(double lx,double ly,double rx,double ry)
{
}

void polygon::leftButtonUp(double lx,double ly,double rx,double ry)
{
}

void polygon::rightButtonDown(double lx,double ly,double rx,double ry)
{
}

void polygon::getNearestRef(double lx,double ly,double rx,double ry, 
	double &rlx,double &rly,double &rrx,double &rry,double &minDist, 
	NearestSide& side)
{
	minDist=999;
	for (int i=0;i<m_entities.size();i++)
	{
		double tprlx, tprly, tprrx, tprry, tpminDist;
		NearestSide tpside;
		m_entities[i]->getNearestRef(lx, ly, rx, ry,
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
}

void polygon::moveRef(double oldlx, double oldly, double oldrx, double oldry, 
	double newlx, double newly, double newrx, double newry)
{
	for (int i = 0; i < m_entities.size();i++)
	{
		m_entities[i]->moveRef(oldlx, oldly, oldrx, oldry,
			newlx, newly, newrx, newry);
	}
	calculateCloseLine();
}

void polygon::endDrawing()
{
	calculateCloseLine();
}

void polygon::addEntity(entity* ent)
{
	m_entities.push_back(ent);
}

void polygon::calculateCloseLine()
{
	m_pLLine->clear();
	m_pRLine->clear();
	if (m_entities.size()==0)
	{
		return;
	}
	entity* firstEntity=m_entities[0];
	entity* lastEntity=m_entities[m_entities.size()-1];
	double flx,fly,frx,fry;
	double llx,lly,lrx,lry;
	static_cast<entityAlone*>(firstEntity)->getFirstPoint(
		flx,fly,frx,fry);
	static_cast<entityAlone*>(lastEntity)->getLastPoint(
		llx,lly,lrx,lry);
	CPoint3D fl,fr,ll,lr;
	fl.m_X=flx;fl.m_Y=fly;fr.m_X=frx;fr.m_Y=fry;
	ll.m_X=llx;ll.m_Y=lly;lr.m_X=lrx;lr.m_Y=lry;
	//fl.m_X=0;fl.m_Y=0;fr.m_X=0;fr.m_Y=0;
	//ll.m_X=100;ll.m_Y=100;lr.m_X=100;lr.m_Y=100;
	m_pLLine->push_back(fl);
	m_pLLine->push_back(ll);
	m_pRLine->push_back(fr);
	m_pRLine->push_back(lr);
}

void polygon::clearData()
{
	for (int i=0;i<m_entities.size();i++)
	{
		m_entities[i]->clearData();
		delete m_entities[i];
	}
	m_pLLine->clear();
	m_pRLine->clear();
	m_entities.clear();
}

void* polygon::getData()
{
	if (m_entities.size()==0)
	{
		return NULL;
	}
	else
	{
		int num=0;
		for (int i=0;i<m_entities.size();i++)
		{
			if (0==i)
			{
				entityAlone* eA=static_cast<entityAlone*>(m_entities[0]);
				num+=eA->m_pLLinePoints->size();
			}
			else
			{
				entityAlone* eA=static_cast<entityAlone*>(m_entities[i]);
				num+=(eA->m_pLLinePoints->size()-1);
			}
		}
		polygonData* pData=new polygonData(num);

		int index=0;
		for (int i=0;i<m_entities.size();i++)
		{
			if (0==i)
			{
				entityAlone* eA=static_cast<entityAlone*>(m_entities[0]);
				int sz=eA->m_pLLinePoints->size();
				for (int j=0;j<sz;j++)
				{
					double lx,ly,rx,ry,X,Y,Z;
					lx=(*(eA->m_pLLinePoints))[j].m_X;
					ly=(*(eA->m_pLLinePoints))[j].m_Y;
					rx=(*(eA->m_pRLinePoints))[j].m_X;
					ry=(*(eA->m_pRLinePoints))[j].m_Y;
					getRealCoor(lx,ly,rx,ry,m_pDlg,X,Y,Z);
					pData->m_dominantlx[index]=lx;
					pData->m_dominantly[index]=ly;
					pData->m_dominantrx[index]=rx;
					pData->m_dominantry[index]=ry;
					pData->m_dominantX[index]=X;
					pData->m_dominantY[index]=Y;
					pData->m_dominantZ[index]=Z;
					index++;
				}
			}
			else
			{
				entityAlone* eA=static_cast<entityAlone*>(m_entities[i]);
				int sz=eA->m_pLLinePoints->size();
				for (int j=1;j<sz;j++)
				{
					double lx,ly,rx,ry,X,Y,Z;
					lx=(*(eA->m_pLLinePoints))[j].m_X;
					ly=(*(eA->m_pLLinePoints))[j].m_Y;
					rx=(*(eA->m_pRLinePoints))[j].m_X;
					ry=(*(eA->m_pRLinePoints))[j].m_Y;
					getRealCoor(lx,ly,rx,ry,m_pDlg,X,Y,Z);
					pData->m_dominantlx[index]=lx;
					pData->m_dominantly[index]=ly;
					pData->m_dominantrx[index]=rx;
					pData->m_dominantry[index]=ry;
					pData->m_dominantX[index]=X;
					pData->m_dominantY[index]=Y;
					pData->m_dominantZ[index]=Z;
					index++;
				}
			}
		}
		return pData;
	}
}