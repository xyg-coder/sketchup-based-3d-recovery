#include "StdAfx.h"
#include "point3.h"

const Point3 Point3::Origin=Point3(0.0f,0.0f,0.0f);
const Point3 Point3::XAxis=Point3(1.0f,0.0f,0.0f);
const Point3 Point3::YAxis=Point3(0.0f,1.0f,0.0f);
const Point3 Point3::ZAxis=Point3(0.0f,0.0f,1.0f);

Point3 Point3::Normalize() const
{
	return *this/this->Length();
}

Point3 Point3::FNormalize() const
{
	return *this/this->Length();
}

Point3 Normalize(const Point3& p)
{
	return p/p.Length();
}

Point3 FNormalize(const Point3& p)
{
	return p/p.Length();
}

Point3 Point3::operator^(const Point3& b) const
{
	return  Point3(y*b.z-z*b.y,z*b.x-x*b.z, x*b.y-b.x*y);
}

Point3 CrossProd(const Point3& a, const Point3& b)
{
	return  Point3(a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z, a.x*b.y-b.x*a.y);
}