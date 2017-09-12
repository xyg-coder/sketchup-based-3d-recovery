#include "StdAfx.h"
#include "polygonSing.h"
#include "singleImageView.h"


polygonSing::polygonSing(GraphicLayer* pPointLayer, GraphicLayer* pLineLayer,
	singleImageView* pView, directionSing* verticalD)
	:entitySing(pPointLayer,pLineLayer,pView,verticalD),
	m_isCloseExisted(false)
{
	m_line = (GraphicPolyline*)m_pLineLayer->CreateGraphicObject(GO_POLYLINE);
	m_pLineLayer->AddGraphicObject(m_line,LineColor);
}


polygonSing::~polygonSing(void)
{
	for (int i=0;i<m_entities.size();i++)
	{
		delete m_entities[i];
		m_entities[i]=NULL;
	}
}

void polygonSing::moveMouse(double x, double y)
{}

void polygonSing::leftButtonUp(double x, double y)
{}

void polygonSing::leftButtonDown(double x, double y)
{}

void polygonSing::rightButtonDown(double x, double y)
{}

void polygonSing::addDominantPoint(double x, double y)
{}

void polygonSing::addPreviewPoint(double x, double y)
{}

void polygonSing::getNearestRef(double x, double y,
	double &resultx, double &resulty, double &minDist)
{
	minDist=999;
	for (int i=0;i<m_entities.size();i++)
	{
		double tpx,tpy,tpminDist;
		m_entities[i]->getNearestRef(x,y,tpx,tpy,tpminDist);
		if (minDist>tpminDist)
		{
			minDist=tpminDist;
			resultx=tpx;
			resulty=tpy;
		}
	}
}

void polygonSing::moveRef(double oldx, double oldy, double newx, double newy)
{
	for (int i=0;i<m_entities.size();i++)
	{
		m_entities[i]->moveRef(oldx,oldy,newx,newy);
	}
	calculateCloseLine();
}

void polygonSing::endDrawing()
{
	calculateCloseLine();
}

void polygonSing::addEntity(entitySing* ent)
{
	m_entities.push_back(ent);
}

void polygonSing::calculateCloseLine()
{
	if (m_isCloseExisted)
	{
		m_line->DeleteVertex(0);
		m_line->DeleteVertex(0);
	}
	if (m_entities.size()==0)
	{
		return;
	}
	double fx,fy,lx,ly;
	m_entities[0]->getFirstPoint(fx,fy);
	m_entities[m_entities.size()-1]->getLastPoint(lx,ly);
	m_line->AddVertex(0,fx,fy);
	m_line->AddVertex(1,lx,ly);
	m_isCloseExisted=true;
}

bool polygonSing::canEndDrawing()
{
	bool canEnd=true;
	for (int i=0;i<m_entities.size();i++)
	{
		canEnd=canEnd&&m_entities[i]->canEndDrawing();
	}
	return canEnd;
}

void* polygonSing::getData()
{
	if(m_entities.size()==0)
	{
		return NULL;
	}
	else
	{
		entitySing* ent1=m_entities[0];
		int num=ent1->m_line->GetVertexSum();
		for (int i=1;i<m_entities.size();i++)
		{
			num+=(m_entities[i]->m_line->GetVertexSum()-1);
		}
		polygonSingData* pData=new polygonSingData;
		pData->setn(num);
		int index=0;
		for (int i=0;i<m_entities.size();i++)
		{
			if (0==i)
			{
				for (int j=0;j<=m_entities[0]->m_line->GetVertexSum();
					j++)
				{
					double x,y,X,Y,Z;
					m_entities[0]->m_line->GetVertex(j,
						&x,&y);
					getRealCoor(x,y,m_pView,X,Y,Z);
					pData->m_dominantx[index]=x;
					pData->m_dominanty[index]=y;
					pData->m_dominantX[index]=X;
					pData->m_dominantY[index]=Y;
					pData->m_dominantZ[index]=Z;
					index++;
				}
			}
			else
			{
				for (int j=1;j<=m_entities[i]->m_line->GetVertexSum();
					j++)
				{
					double x,y,X,Y,Z;
					m_entities[i]->m_line->GetVertex(j,
						&x,&y);
					getRealCoor(x,y,m_pView,X,Y,Z);
					pData->m_dominantx[index]=x;
					pData->m_dominanty[index]=y;
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