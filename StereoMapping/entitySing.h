#pragma once
#include <vector>
#define PointColor RGB(255, 255, 0)
#define LineColor RGB(255,0,0)
class singleImageView;
class GraphicPoint;
class GraphicPolyline;
class GraphicLayer;
class lastPointSing;
class directionSing;
//此中x,y均为世界坐标系;

inline double pointDistSing(double x1, double y1, double x2, double y2)
{
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}
double screenDist(double x1, double y1, double x2, double y2, singleImageView* pView);

class entitySing
{
public:
	friend class polygonSing;
	enum classify{polyline,circle,arc,polygon};
	entitySing(GraphicLayer* pPointLayer,GraphicLayer* pLineLayer,
		singleImageView* pView,directionSing* verticalD);
	entitySing(GraphicLayer* pPointLayer, GraphicLayer* pLineLayer,
		singleImageView* pView, lastPointSing* lastP, directionSing* verticalD);
	virtual void moveMouse(double x, double y) = 0;
	virtual void leftButtonDown(double x, double y) = 0;
	virtual void leftButtonUp(double x, double y) = 0;
	virtual void rightButtonDown(double x, double y) = 0;
	virtual void getNearestRef(double x, double y,
		double &resultx, double &resulty,double &minDist)=0;
	virtual void moveRef(double oldx, double oldy, double newx, double newy) = 0;
	virtual void endDrawing() = 0;
	virtual void getRealCoor(double x, double y, singleImageView* pView,
		double &X, double &Y, double &Z);
	virtual void getPointCoor(double X, double Y, double Z, singleImageView* pView,
		double &x, double &y);
	virtual void addDominantPoint(double x, double y) = 0;
	virtual void addPreviewPoint(double x, double y) = 0;
	virtual void setLastPoint(lastPointSing* lpoint);//对最后一点进行设置;
	virtual bool canEndDrawing()=0;
	virtual classify getClassify()=0;
	//获取dominantPoint个数;
	virtual int getNPoint()
	{
		return m_tmpPointSize;
	}
	virtual void* getData()=0;
	virtual void getFirstPoint(double &x,double &y);//获取第一个点坐标;
	virtual void getLastPoint(double &x,double &y);//获取最后一个点坐标;
	~entitySing();
protected:
	std::vector<GraphicPoint*> m_dominantPoints;
	GraphicPolyline* m_line;
	double m_previewx;
	double m_previewy;
	int m_previewIndex;
	bool m_isPreviewExisted;
	int m_tmpPointSize; //点数，不包括preview
	GraphicLayer* m_pPointLayer;
	GraphicLayer* m_pLineLayer;
	singleImageView* m_pView;
	lastPointSing *m_lastPoint;
	directionSing* m_verticalDirection;//平面的法线方向;
};

