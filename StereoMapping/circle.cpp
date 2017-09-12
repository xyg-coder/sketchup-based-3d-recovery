#include "stdafx.h"
#include "circle.h"
#include "render.h"
#include "StereoViewDlg.h"
#include "AerialBase.h"


circle::circle(CEditLine* pLLine, CEditLine* pRLine, CStereoViewDlg* pDlg)
:arc(pLLine,pRLine,pDlg)
{
}

circle::circle(CEditLine* pLLine, CEditLine* pRLine, CStereoViewDlg* pDlg,
	lastPoint* lastPt):arc(pLLine,pRLine,pDlg,lastPt)
{

}

void circle::calculateLinePoins()
{
	m_pLLinePoints->clear();
	m_pRLinePoints->clear();
	//double fai1 = atan2(m_Y[0] - m_cy, m_X[0] - m_cx);
	for (double fai = 0; true;fai+=RADIOSTEP)
	{
		if (fai>=2*PI)
		{
			double X = m_cx + m_r;
			double Y = m_cy;
			double Z = m_averZ;
			CPoint3D lpt, rpt;
			getPointCoor(X, Y, Z, m_pDlg, lpt.m_X, lpt.m_Y, rpt.m_X, rpt.m_Y);
			lpt.m_Z = rpt.m_Z = 0;
			m_pLLinePoints->push_back(lpt);
			m_pRLinePoints->push_back(rpt);
			m_pLLinePoints->Normalize();
			m_pRLinePoints->Normalize();
			break;
		}
		double X = m_cx + m_r*cos(fai);
		double Y = m_cy + m_r*sin(fai);
		double Z = m_averZ;
		CPoint3D lpt, rpt;
		getPointCoor(X, Y, Z, m_pDlg, lpt.m_X, lpt.m_Y, rpt.m_X, rpt.m_Y);
		lpt.m_Z = rpt.m_Z = 0;
		m_pLLinePoints->push_back(lpt);
		m_pRLinePoints->push_back(rpt);
		m_pLLinePoints->Normalize();
		m_pRLinePoints->Normalize();
	}
}

void* circle::getData()
{
	circleData* tpCircleData=new circleData;
	tpCircleData->m_centerX=m_cx;
	tpCircleData->m_centerY=m_cy;
	tpCircleData->m_centerZ=m_cz;
	double clx,cly,crx,cry;
	getPointCoor(m_cx,m_cy,m_cz,m_pDlg,clx,cly,crx,cry);
	tpCircleData->m_centerlx=clx;
	tpCircleData->m_centerly=cly;
	tpCircleData->m_centerrx=crx;
	tpCircleData->m_centerry=cry;
	for (int i=0;i<3;i++)
	{
		tpCircleData->m_dominantPX[i]=m_X[i];
		tpCircleData->m_dominantPY[i]=m_Y[i];
		tpCircleData->m_dominantPZ[i]=m_Z[i];
		getPointCoor(m_X[i],m_Y[i],m_Z[i],m_pDlg,
			tpCircleData->m_dominantPlx[i],tpCircleData->m_dominantPly[i],
			tpCircleData->m_dominantPrx[i],tpCircleData->m_dominantPry[i]);
	}
	tpCircleData->m_r=m_r;
	//tpArcData->m_arc=m_fai;
	return tpCircleData;
}

circle::~circle()
{
}
