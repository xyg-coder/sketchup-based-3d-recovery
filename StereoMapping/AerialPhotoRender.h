#pragma once

#include "Render.h"
#include "IGeoModel.h"
#include "FxImage.h"

class CAerialPhotoRender : public CStereoRender, public IGeometricModel, public IImageIO
{
public:
	CAerialPhotoRender(void);
	~CAerialPhotoRender(void);


	void         SetGeoModel(IGeoModel* lpGeoModel, bool bLeft = true);
	IGeoModel*    GetGeoModel() { return m_lpGeoModel;}

	bool         SetImagePathFile(const char* lpszImagePathFile);


	//implement IGeometricModel
	virtual bool ObjectToPixel(double X, double Y, double Z, double* ix, double* iy);
	virtual bool PixelToObject(double ix, double iy, double Z, double* X, double* Y );

	virtual int  GetWidth();
	virtual int  GetHeight();
	virtual int  GetBands();
	virtual int  GetLevels();
	virtual bool ReadLevelInterleavingImage(RECT rcRead,void*lpBuf,int nLevel,int linespace);


protected:
	IGeoModel*    m_lpGeoModel;
	bool          m_bLeft;
	CFxImage      m_sxImage;

};

