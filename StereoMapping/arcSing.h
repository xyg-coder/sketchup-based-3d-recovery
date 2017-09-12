#pragma once
#include "entitySing.h"
#define PI 3.1415926535898
#define RADIOSTEP 0.261799
struct arcSingData
{
	double m_centerx;
	double m_centery;
	double m_centerX;
	double m_centerY;
	double m_centerZ;
	double m_r;
	double m_arc;
	double m_dominantPx[3];
	double m_dominantPy[3];
	double m_dominantPX[3];
	double m_dominantPY[3];
	double m_dominantPZ[3];
};
class arcSing
	:public entitySing
{
public:
	arcSing(GraphicLayer* pPointLayer, GraphicLayer* pLineLayer,
		singleImageView* pView, directionSing* verticalD);
	arcSing(GraphicLayer* pPointLayer, GraphicLayer* pLineLayer,
		singleImageView* pView, lastPointSing* lastP, directionSing* verticalD);
	virtual void moveMouse(double x, double y);
	virtual void leftButtonDown(double x, double y){};
	virtual void leftButtonUp(double x, double y);
	virtual void rightButtonDown(double x, double y);
	virtual void getNearestRef(double x, double y,
		double &resultx, double &resulty, double &minDist);
	virtual void moveRef(double oldx, double oldy, double newx, double newy);
	virtual void endDrawing();
	virtual void addDominantPoint(double x, double y);
	virtual void addPreviewPoint(double x, double y);
	virtual void setVertical(directionSing verticalD, double x0, double y0, double z0);//对法向量进行设置;
	virtual void calculateArc();
	//利用已有参数计算LinePoints中的点，暂时认为弧段是水平的;
	virtual void calculateLinePoins();
	virtual void* getData();
	virtual bool canEndDrawing();
	virtual classify getClassify()
	{
		return classify::arc;
	}
	virtual ~arcSing();
protected:
	//std::vector<GraphicPoint*> m_middlePoints;//出去首尾点的中间点;
	double m_cx;
	double m_cy;
	double m_cz;//圆心真实三维坐标;
	double m_averZ;
	double m_averX;
	double m_averY;//圆弧的平均坐标（重心化）
	double m_A;
	double m_B;
	double m_C;
	double m_D;//平面方程;AX+BY+CZ+D=0
	double m_r;
	double m_X[3];
	double m_Y[3];
	double m_Z[3];//用于确定方程的三个点的三维坐标;
	int m_linePoints;//在m_line中点的个数;
	bool m_isDrawing;
	double m_fai;//弧度;
};

