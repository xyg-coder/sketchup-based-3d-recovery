#include "StdAfx.h"
#include "AerialPhotoRender.h"


CAerialPhotoRender::CAerialPhotoRender()
	:CStereoRender(), m_lpGeoModel(NULL),m_bLeft(true)
{
}


CAerialPhotoRender::~CAerialPhotoRender()
{
}


void   CAerialPhotoRender::SetGeoModel(IGeoModel* lpGeoModel, bool bLeft)
{
	if(lpGeoModel && lpGeoModel->IsInitialized())
	{
		m_lpGeoModel = lpGeoModel;
		m_bLeft = bLeft;

		SetGeometricModel(this);
	}
}


bool    CAerialPhotoRender::SetImagePathFile(const char* lpszImagePathFile)
{
	if( !m_sxImage.Open(lpszImagePathFile) )
		return false;

	SetImageIO(this);
	return true;
}


//implement IGeometricModel
bool CAerialPhotoRender::ObjectToPixel(double X, double Y, double Z, double* ix, double* iy)
{
	if(m_lpGeoModel && m_lpGeoModel->IsInitialized())
	{
		return m_lpGeoModel->BackProjection(X, Y, Z, m_bLeft, ix, iy);
	}

	return false;
}

bool CAerialPhotoRender::PixelToObject(double ix, double iy, double Z, double* X, double* Y )
{
	if(m_lpGeoModel && m_lpGeoModel->IsInitialized())
	{
		return m_lpGeoModel->SingleRayIntersection(ix, iy, m_bLeft, X, Y, Z);
	}

	return false;
}


int  CAerialPhotoRender::GetWidth()
{
	return m_sxImage.GetWidth();
}

int  CAerialPhotoRender::GetHeight()
{
	return m_sxImage.GetHeight();
}

int  CAerialPhotoRender::GetBands()
{
	return m_sxImage.GetBands();
}

int  CAerialPhotoRender::GetLevels()
{
	return m_sxImage.GetOverviewCount();
}

bool CAerialPhotoRender::ReadLevelInterleavingImage(RECT rcRead,void*lpBuf,int nLevel,int linespace)
{
	static int bandmap[] = { 1, 2, 3, 4};
	int _level = -1;

	if(nLevel == 0)
		return m_sxImage.ReadInterleavingImage(rcRead.top, rcRead.left, rcRead.bottom - rcRead.top, rcRead.bottom - rcRead.top, GetBands() >= 3 ? 3 : 1,  bandmap, lpBuf, linespace) ? true : false;
	else
	{
		for(int i = 0; i < m_sxImage.GetOverviewCount(); i++)
		{
			double zoomx = (double)m_sxImage.GetWidth()/m_sxImage.OverviewWidth(i) + 0.2;
			double zoomy = (double)m_sxImage.GetHeight()/m_sxImage.OverviewHeight(i) + 0.2;

			int zoom = (int)max(zoomx, zoomy);

			if(zoom == 1 << nLevel)
			{
				_level = i;
				break;
			}
		}

		if(_level != -1)
			return m_sxImage.ReadOverviewIndexInterleavingImage(rcRead.top, rcRead.left, rcRead.bottom - rcRead.top, rcRead.bottom - rcRead.top, _level, GetBands() >= 3 ? 3 : 1,  bandmap, lpBuf, linespace) ? true : false;
		else
			return false;
	}
}