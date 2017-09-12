#pragma once
#include "entity.h"
#include "StereoViewDlg.h"
/** 构造面，由entityAlong构成，首尾连接 */
struct polygonData
{
	int m_n;
	double *m_dominantlx;
	double *m_dominantly;
	double *m_dominantrx;
	double *m_dominantry;
	double *m_dominantX;
	double *m_dominantY;
	double *m_dominantZ;
	polygonData(int n):m_n(n)
	{
		m_dominantlx=new double[n];
		m_dominantly=new double[n];
		m_dominantrx=new double[n];
		m_dominantry=new double[n];
		m_dominantX=new double[n];
		m_dominantY=new double[n];
		m_dominantZ=new double[n];
	}
	~polygonData()
	{
		delete []m_dominantlx;
		m_dominantlx=NULL;
		delete []m_dominantly;
		m_dominantly=NULL;
		delete []m_dominantrx;
		m_dominantrx=NULL;
		delete []m_dominantry;
		m_dominantry=NULL;
		delete []m_dominantX;
		m_dominantX=NULL;
		delete []m_dominantY;
		m_dominantY=NULL;
		delete []m_dominantZ;
		m_dominantZ=NULL;
	}
};
class polygon :
	public entity
{
public:
	polygon(CStereoViewDlg* pDlg,CEditLine* lcloseLine,CEditLine* rcloseLine);
	virtual ~polygon(void);
	virtual void moveMouse(double lx,double ly,double rx,double ry);
	virtual void leftButtonDown(double lx,double ly,double rx,double ry);
	virtual void leftButtonUp(double lx,double ly,double rx,double ry);
	virtual void rightButtonDown(double lx,double ly,double rx,double ry);
	virtual void getNearestRef(double lx,double ly,double rx,double ry,
		double &rlx,double &rly,double &rrx,double &rry,double &minDist,
		NearestSide& side);
	virtual void moveRef(double oldlx, double oldly, double oldrx, double oldry,
		double newlx, double newly, double newrx, double newry);
	virtual void endDrawing();
	virtual void addEntity(entity* ent);
	virtual void calculateCloseLine();
	virtual entityClassify getClassify()
	{
		return entity::POLYGON;
	}
	virtual void clearData();
	virtual void* getData();
protected:
	std::vector<entity*>m_entities;
	CStereoViewDlg* m_pDlg;
	//VERTICALMODE m_verticalMode;
	CEditLine *m_pLLine;
	CEditLine *m_pRLine;
};