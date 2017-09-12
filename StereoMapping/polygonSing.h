#pragma once
#include "entitysing.h"
struct polygonSingData
{
	int m_n;
	double *m_dominantx;
	double *m_dominanty;
	double *m_dominantX;
	double *m_dominantY;
	double *m_dominantZ;
	polygonSingData()
		:m_dominantx(NULL),m_dominanty(NULL),
		m_dominantX(NULL),m_dominantY(NULL),m_dominantZ(NULL)
	{}
	void setn(int n)
	{
		m_n=n;
		m_dominantx=new double[n];
		m_dominanty=new double[n];
		m_dominantX=new double[n];
		m_dominantY=new double[n];
		m_dominantZ=new double[n];
	}
	~polygonSingData()
	{
		if (m_dominantx)
		{
			delete []m_dominantx;
			m_dominantx=NULL;
			delete []m_dominanty;
			m_dominanty=NULL;
			delete []m_dominantX;
			m_dominantX=NULL;
			delete []m_dominantY;
			m_dominantY=NULL;
			delete []m_dominantZ;
			m_dominantZ=NULL;
		}
	}
};

class polygonSing :
	public entitySing
{
public:
	polygonSing(GraphicLayer* pPointLayer, GraphicLayer* pLineLayer,
		singleImageView* pView,directionSing* verticalD);
	virtual ~polygonSing(void);
	virtual void moveMouse(double x, double y);
	virtual void leftButtonDown(double x, double y);
	virtual void leftButtonUp(double x, double y);
	virtual void rightButtonDown(double x, double y);
	virtual void getNearestRef(double x, double y,
		double &resultx, double &resulty, double &minDist);
	virtual void moveRef(double oldx, double oldy, double newx, double newy);
	virtual void addDominantPoint(double x, double y);
	virtual void addPreviewPoint(double x, double y);
	virtual void endDrawing();
	virtual void addEntity(entitySing* ent);
	virtual void calculateCloseLine();
	virtual classify getClassify()
	{
		return entitySing::polygon;
	}
	virtual bool canEndDrawing();
	virtual void* getData();
protected:
	std::vector<entitySing*> m_entities;
	bool m_isCloseExisted;//当前closeLine是否有点;
	//GraphicPolyline* m_closeLine;
};

