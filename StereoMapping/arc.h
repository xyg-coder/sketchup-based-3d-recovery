#pragma once
#include "entity.h"
#include <Eigen/Dense>
#define PI 3.1415926535898
#define RADIOSTEP 0.261799
class CStereoViewDlg;

struct arcData
{
	double m_centerrx;
	double m_centerry;
	double m_centerlx;
	double m_centerly;
	double m_centerX;
	double m_centerY;
	double m_centerZ;
	double m_r;
	double m_arc;
	double m_dominantPlx[3];
	double m_dominantPly[3];
	double m_dominantPrx[3];
	double m_dominantPry[3];
	double m_dominantPX[3];
	double m_dominantPY[3];
	double m_dominantPZ[3];
};

//返回x,y到直线(x1,y1-x2,y2)的距离;
inline double lineDist(double x1, double y1, double x2, double y2, double x, double y);
class arc :
	public entityAlone
{
public:
	arc(CEditLine* pLLine,CEditLine* pRLine,CStereoViewDlg* pDlg
		/*,bool isClose=false*/);
	arc(CEditLine* pLLine,CEditLine* pRLine,CStereoViewDlg* pDlg,
		lastPoint* lastPt/*,lastPoint* firstPt,bool isClose=false*/);
	virtual ~arc(void);
	virtual void moveMouse(double lx,double ly,double rx,double ry);
	virtual void leftButtonDown(double lx,double ly,double rx,double ry);
	virtual void leftButtonUp(double lx,double ly,double rx,double ry);
	virtual void rightButtonDown(double lx,double ly,double rx,double ry);
	//暂时返回一个很大值，目标：点在圆附近就当做对中间点进行修改;
	virtual void getNearestRef(double lx,double ly,double rx,double ry,
		double &rlx,double &rly,double &rrx,double &rry,double &minDist,
		NearestSide& side);
	virtual void moveRef(double oldlx, double oldly, double oldrx, double oldry, 
		double newlx, double newly, double newrx, double newry);
	virtual void addDominantPoint(double lx,double ly,double rx,double ry);
	//超过三个点时添加previewPoint时不对linePoints进行操作;
	virtual void addPreviewPoint(double lx,double ly,double rx,double ry);
	virtual void endDrawing();
	//对法向量进行设置;
	virtual void setVertical();
	virtual void calculateArc(CStereoViewDlg* pDlg);
	//利用已有参数计算LinePoints中的点，暂时认为弧段是水平的;
	virtual void calculateLinePoins();
	virtual void* getData();
	virtual entityClassify getClassify()
	{
		return entity::ARC;
	}
	virtual bool canEntityFinish();
/*
	void getRealCoor(double lx,double ly,double rx,double ry,CStereoViewDlg* pDlg,
		double &X,double &Y,double &Z);
	void getPointCoor(double X,double Y,double Z,CStereoViewDlg* pDlg,
		double &lx,double &ly,double &rx,double &ry);*/


protected:
	double m_fai;//弧段弧度;
	double m_cx;
	double m_cy;
	double m_cz;//圆心的三维真实坐标;
	double m_averZ;
	double m_averX;
	double m_averY;//圆的坐标平均值（重心化）
	CStereoViewDlg* m_pDlg;
	double m_A;
	double m_B;
	double m_C;
	double m_D;//平面方程;AX+BY+CZ+D=0
	double m_r;
	double m_X[3];
	double m_Y[3];
	double m_Z[3];//用于确定方程的三个点的三维坐标;
	bool m_isDrawing;//如果正在绘画，只能修改第一个点，否则能修改最后一个点;
};

