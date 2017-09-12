

#ifndef __IGEOMODEL_H__
#define __IGEOMODEL_H__

class IGeoModel
{
public:
    virtual bool    IsInitialized() = 0;
    virtual void    GetXYZ(bool bLeft, double&X, double& Y, double &Z) = 0;
    virtual bool    BackProjection( double X, double Y, double Z,bool bLeft, double *pix, double *piy) = 0;
    virtual bool	SpaceIntersection(double lpx, double lpy, double rpx, double rpy, double *X, double *Y, double *Z) = 0;
    virtual bool	SingleRayIntersection(double pix, double piy, bool bLeft, double *X, double *Y, double Z) = 0;
};





#endif //__IGEOMODEL_H__