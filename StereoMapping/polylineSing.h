#pragma once
#include "entitySing.h"
#include "singleImageView.h"
struct polylineSingData
{
	polylineSingData(int n):m_n(n)
	{
		m_dominantx=new double[n];
		m_dominanty=new double[n];
		m_dominantX=new double[n];
		m_dominantY=new double[n];
		m_dominantZ=new double[n];
	}
	int m_n;
	double *m_dominantx;
	double *m_dominanty;
	double *m_dominantX;
	double *m_dominantY;
	double *m_dominantZ;
	~polylineSingData()
	{
		delete[]m_dominantx;
		delete[]m_dominanty;
		delete[]m_dominantX;
		delete[]m_dominantY;
		delete[]m_dominantZ;
	}
};
class polylineSing :
	public entitySing
{
public:
	polylineSing(GraphicLayer* pPointLayer, GraphicLayer* pLineLayer,
		singleImageView* pView,directionSing* verticalD,verticalStateSing verticalS=OFFSing);
	polylineSing(GraphicLayer* pPointLayer, GraphicLayer* pLineLayer,
		singleImageView* pView, lastPointSing* lastP, directionSing* verticalD,
		verticalStateSing verticalS = OFFSing);
	virtual void moveMouse(double x, double y);
	virtual void leftButtonDown(double x, double y);
	virtual void leftButtonUp(double x, double y);
	virtual void rightButtonDown(double x, double y);
	virtual void getNearestRef(double x, double y,
		double &resultx, double &resulty, double &minDist);
	virtual void moveRef(double oldx, double oldy, double newx, double newy);
	virtual void endDrawing();
	virtual void addDominantPoint(double x, double y);
	virtual void addPreviewPoint(double x, double y);
	virtual classify getClassify()
	{
		return entitySing::polyline;
	}
	virtual bool canEndDrawing()
	{
		return true;
	}
	void getCorrectPoint(double x, double y, double &rx, double &ry);
	//设置用于垂直化的方向;
	void setMainDirection();
	virtual void setVerticalState(verticalStateSing verticalS)
	{
		m_verticalState = verticalS;
	}
	virtual void* getData();

	virtual ~polylineSing();
protected:
	verticalStateSing m_verticalState;
	bool m_isMainSet;//主方向是否设置好(垂直化投影到的方向);
	directionSing m_mainDirection;
};

