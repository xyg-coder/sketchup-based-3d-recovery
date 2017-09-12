#pragma once
#include <vector>
class lastPoint;
class CEditLine;
class CStereoViewDlg;
enum NearestSide{LEFT,RIGHT};
/************************************************************************/
/* 此项为所有entity基类，Dominant中储存了决定该形状的点，LinePoints中储存
了最终作画用的点，是根据Dominant求出来的*/
/************************************************************************/
class entity
{
public:
	entity(void);
	enum entityClassify{POLYGON,POLYLINE,ARC,CIRCLE,BUILDING};
	virtual void moveMouse(double lx,double ly,double rx,double ry)=0;
	virtual void leftButtonDown(double lx,double ly,double rx,double ry)=0;
	virtual void leftButtonUp(double lx,double ly,double rx,double ry)=0;
	virtual void rightButtonDown(double lx,double ly,double rx,double ry)=0;
	virtual void getNearestRef(double lx,double ly,double rx,double ry,
		double &rlx,double &rly,double &rrx,double &rry,double &minDist,
		NearestSide& side)=0;
	virtual void moveRef(double oldlx, double oldly, double oldrx, double oldry,
		double newlx, double newly, double newrx, double newry)=0;
	virtual void endDrawing() = 0;
	virtual void getRealCoor(double lx, double ly, double rx, double ry, CStereoViewDlg* pDlg,
		double &X, double &Y, double &Z);
	virtual void getPointCoor(double X, double Y, double Z, CStereoViewDlg* pDlg,
		double &lx, double &ly, double &rx, double &ry);
	//利用local坐标求解屏幕上的距离;
	double screenDist(double x1, double y1, double x2, double y2, NearestSide side,CStereoViewDlg* pDlg);
	virtual void outputFirstPoint(double &X, double &Y, double &Z, CStereoViewDlg* pDlg);
	virtual entityClassify getClassify()=0; 
	virtual void clearData()=0;//将所有数据删除;
	virtual void* getData()
	{
		return NULL;
	}
	virtual bool canEntityFinish()
	{
		return true;
	}
	~entity(void);
};

class entityAlone:public entity
{
public:
	friend class polygon;
	entityAlone(CEditLine* pLLine,CEditLine* pRLine);
	//用于已经有点时将前一个点加入构造;
	entityAlone(CEditLine* pLLine,CEditLine* pRLine,
		lastPoint* lastPt);
    //注意进行归一化;
	virtual void addDominantPoint(double lx,double ly,double rx,double ry)=0;
	virtual void addPreviewPoint(double lx,double ly,double rx,double ry)=0;
	virtual void outputFirstPoint(double &X, double &Y, double &Z, CStereoViewDlg* pDlg);
	virtual void setLastPoint(lastPoint* lastPt);
	//virtual void setFirstPoint(lastPoint* firstPt);
	virtual void getFirstPoint(double &lx,double& ly,double &rx,double &ry);//获取第一个点坐标;
	virtual void getLastPoint(double &lx,double& ly,double &rx,double &ry);//获取最后一个点坐标(有预览，则获得预览);
	virtual void clearData();

	//结束元素绘画;
	virtual ~entityAlone();
protected:
	CEditLine* m_pLDominant;
	CEditLine* m_pRDominant;
	double m_previewLx;
	double m_previewLy;
	double m_previewRx;
	double m_previewRy;
	bool m_isPreviewExisted;
	CEditLine* m_pLLinePoints;
	CEditLine* m_pRLinePoints;
};