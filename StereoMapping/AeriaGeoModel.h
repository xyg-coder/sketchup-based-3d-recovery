

#ifndef __AERIA_GEOMODEL_H__
#define __AERIA_GEOMODEL_H__

#include "IGeoModel.h"
#include "AerialBase.h"

class CAeriaGeoModel : public IGeoModel
{
public:
	CAeriaGeoModel();
	virtual ~CAeriaGeoModel();

	bool           LoadModel(CAerialPhoto* lpLeftPht, CAerialPhoto* lpRightPht);
	virtual bool    IsInitialized() { return (m_lpLeftPhoto != NULL && m_lpRightPhoto != NULL);}
	virtual void    GetXYZ(bool bLeft, double&X, double& Y, double &Z);
	virtual bool    BackProjection( double X, double Y, double Z, bool bLeft, double *pix, double *piy);
	virtual bool	   SpaceIntersection(double lpx, double lpy, double rpx, double rpy, double *X, double *Y, double *Z);
	virtual bool    SingleRayIntersection(double pix, double piy, bool bLeft, double *X, double *Y, double Z);

private:
	CAerialPhoto*    m_lpLeftPhoto, *m_lpRightPhoto;
};









#endif //__AERIA_GEOMODEL_H__