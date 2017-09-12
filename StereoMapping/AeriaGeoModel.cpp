

#include "stdafx.h"
#include <math.h>
#include "AeriaGeoModel.h"
#include "FxImage.h"



CAeriaGeoModel::CAeriaGeoModel()
:m_lpLeftPhoto(NULL),m_lpRightPhoto(NULL)
{
	
}

CAeriaGeoModel::~CAeriaGeoModel()
{

}

bool    CAeriaGeoModel::LoadModel(CAerialPhoto* lpLeftPht,CAerialPhoto* lpRightPht)
{
	m_lpLeftPhoto   = lpLeftPht;
	m_lpRightPhoto  = lpRightPht;
	
	
	if(m_lpLeftPhoto && m_lpRightPhoto)
	{
		return true;
	}
	
	return false;
}

void    CAeriaGeoModel::GetXYZ(bool bLeft, double&X, double& Y, double &Z)
{
	CAerialPhoto* lpPhoto = bLeft ? m_lpLeftPhoto : m_lpRightPhoto;
	lpPhoto->GetXYZ(X, Y, Z);
}

bool    CAeriaGeoModel::BackProjection( double X, double Y, double Z,bool bLeft, double *pix, double *piy)
{
	if(!IsInitialized())
		return false;

	CAerialPhoto* lpPhoto = bLeft ? m_lpLeftPhoto : m_lpRightPhoto;
	
	lpPhoto->ObjectCS2PixelCS(X, Y, Z, *pix, *piy);

	return true;
}

bool	CAeriaGeoModel::SpaceIntersection(double lpx, double lpy, double rpx, double rpy, double *X, double *Y, double *Z)
{
	CAerialPhoto* phts[2];
	float pix_x[2], pix_y[2];

	if( !IsInitialized()) 
		return false;

	phts[0] = m_lpLeftPhoto;
	phts[1] = m_lpRightPhoto;

	pix_x[0] = (float)lpx;
	pix_x[1] = (float)rpx;
	pix_y[0] = (float)lpy;
	pix_y[0] = (float)rpy;

	return CAerialBlock::SpaceIntersection(phts, pix_x, pix_y, 2, X, Y, Z);
}

bool	CAeriaGeoModel::SingleRayIntersection(double pix, double piy, bool bLeft, double *X, double *Y, double Z)
{
	if(!IsInitialized()) return false;
	CAerialPhoto* lpPht = bLeft ? m_lpLeftPhoto : m_lpRightPhoto;
	
	lpPht->PixelRayIntersect(pix, piy, Z, *X, *Y);

	return true;
}