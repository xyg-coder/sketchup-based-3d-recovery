#pragma once
#include "arc.h"
struct circleData
{
	double m_centerrx;
	double m_centerry;
	double m_centerlx;
	double m_centerly;
	double m_centerX;
	double m_centerY;
	double m_centerZ;
	double m_r;
	double m_dominantPlx[3];
	double m_dominantPly[3];
	double m_dominantPrx[3];
	double m_dominantPry[3];
	double m_dominantPX[3];
	double m_dominantPY[3];
	double m_dominantPZ[3];
};
class circle :
	public arc
{
public:
	circle(CEditLine* pLLine, CEditLine* pRLine, CStereoViewDlg* pDlg);
	circle(CEditLine* pLLine, CEditLine* pRLine, CStereoViewDlg* pDlg,
		lastPoint* lastPt);
	virtual ~circle();
	virtual void calculateLinePoins();
	virtual void* getData();
	virtual entityClassify getClassify()
	{
		return entity::CIRCLE;
	}
};

