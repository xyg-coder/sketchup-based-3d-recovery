#pragma once
#include "entity.h"
#include "StereoViewDlg.h"
#define RoofPointThresh 5
//房屋由屋顶和侧边构成;
//注意记录最后一点;
class polygonBuilding :
	public entity
{
public:
	polygonBuilding(CStereoViewDlg* pDlg);
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
	void addRoofEntity(entity*roofEntity);
	bool setRoofPoint(double lx,double ly,double rx,double ry,
		CEditLine* sideLeftLine,CEditLine* sideRightLine);
	//判断房屋是否可以结束测量;
	bool isBuildingFinish();
	void resetRoofPoint();//重置当前正在测量的顶面点，重新测量;
	void setBottomPoint(double lx,double ly,double rx,double ry);
	void setBottomPtReview(double lx,double ly,double rx,double ry);
	virtual void clearData();
	virtual entityClassify getClassify()
	{
		return entity::BUILDING;
	}
	virtual ~polygonBuilding(void);
protected:
	std::vector<entity*>m_roofEntity;
	entity* m_editedSide;
	bool m_isRoofPointSet;
	bool m_isSideLineSet;
	std::vector<entity*>m_sideLines;
	CStereoViewDlg* m_pDlg;
	VERTICALMODE m_verticalMode;
};

