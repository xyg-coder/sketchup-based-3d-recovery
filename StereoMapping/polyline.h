#pragma once
#include "entity.h"
#include "StereoViewDlg.h"
struct polylineData
{
	polylineData(int n):m_n(n)
	{
		m_dominantlx=new double[n];
		m_dominantly=new double[n];
		m_dominantrx=new double[n];
		m_dominantry=new double[n];
		m_dominantX=new double[n];
		m_dominantY=new double[n];
		m_dominantZ=new double[n];
	}
	int m_n;
	double *m_dominantlx;
	double *m_dominantly;
	double *m_dominantrx;
	double *m_dominantry;
	double *m_dominantX;
	double *m_dominantY;
	double *m_dominantZ;
	~polylineData()
	{
		delete[]m_dominantrx;
		delete[]m_dominantry;
		delete[]m_dominantlx;
		delete[]m_dominantly;
		delete[]m_dominantX;
		delete[]m_dominantY;
		delete[]m_dominantZ;
	}
};
class polyline :
	public entityAlone
{
public:
	friend class polygonBuilding;
	polyline(CEditLine* pLLine, CEditLine* pRLine, CStereoViewDlg* pDlg,
		VERTICALMODE vertical = OFF/*,bool isClose=false*/);
	polyline(CEditLine* pLLine, CEditLine* pRLine, CStereoViewDlg* pDlg,
		lastPoint* lastPt,VERTICALMODE vertical=OFF/*,lastPoint* firstPt=NULL,
		bool isClose=false*/);
	void setVerticalMode(VERTICALMODE vertical)
	{
		m_verticalMode = vertical;
	}
	virtual void moveMouse(double lx,double ly,double rx,double ry);
	virtual void leftButtonDown(double lx,double ly,double rx,double ry);
	virtual void leftButtonUp(double lx,double ly,double rx,double ry);
	virtual void rightButtonDown(double lx,double ly,double rx,double ry);
	virtual void addDominantPoint(double lx,double ly,double rx,double ry);
	virtual void addPreviewPoint(double lx,double ly,double rx,double ry);
	virtual void getNearestRef(double lx,double ly,double rx,double ry,
		double &rlx,double &rly,double &rrx,double &rry,double &minDist,
		NearestSide& side);
	virtual void moveRef(double oldlx, double oldly, double oldrx, double oldry,
		double newlx, double newly, double newrx, double newry);
	//根据dominant中的情况获取line中的情况;
	void getLine();
	void setAverZ();
	virtual void endDrawing();
	virtual void setMainDirection();
	//用于垂直化时确定正确的坐标;
	virtual void getCorrectPoint(double lx, double ly, double rx, double ry,
		double &newlx, double &newly, double &newrx, double &newry);
	virtual void* getData();
	virtual entityClassify getClassify()
	{
		return entity::POLYLINE;
	}
	//virtual void deleteClose();
	virtual ~polyline(void);
protected:
	VERTICALMODE m_verticalMode;
	double m_mainX;
	double m_mainY;
	double m_mainZ;//垂直化投影方向(已经进行了归一化);
	double m_averZ;
	bool m_isMainSet;//主方向是否设置好;
	CStereoViewDlg *m_pDlg;
	//bool m_isClose;
	//lastPoint* m_firstPt;
	//bool m_isCloseLineExisted;
	//bool m_isDrawing;
};

