#pragma once
#include "arcSing.h"
struct circleSingData
{
	double m_centerx;
	double m_centery;
	double m_centerX;
	double m_centerY;
	double m_centerZ;
	double m_r;
	double m_dominantPx[3];
	double m_dominantPy[3];
	double m_dominantPX[3];
	double m_dominantPY[3];
	double m_dominantPZ[3];
};
class circleSing :
	public arcSing
{
public:
	circleSing(GraphicLayer* pPointLayer, GraphicLayer* pLineLayer,
		singleImageView* pView, directionSing* verticalD);
	circleSing(GraphicLayer* pPointLayer, GraphicLayer* pLineLayer,
		singleImageView* pView, lastPointSing* lastP, directionSing* verticalD);
	virtual ~circleSing();
	virtual void calculateLinePoins();
	virtual void* getData();
	virtual classify getClassify()
	{
		return classify::circle;
	}
};

