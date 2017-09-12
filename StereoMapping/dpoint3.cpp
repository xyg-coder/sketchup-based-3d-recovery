#include "stdafx.h"
#include "dpoint3.h"

DPoint3 DPoint3::operator^(const DPoint3& b) const
{
	return  DPoint3(y*b.z-z*b.y,z*b.x-x*b.z, x*b.y-b.x*y);
}

DPoint3 CrossProd(const DPoint3& a, const DPoint3& b)
{
	return  DPoint3(a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z, a.x*b.y-b.x*a.y);
}


double DotProd(const DPoint3& a, const DPoint3& b)
{
	return (a.x*b.x+a.y*b.y+a.z*b.z);
}