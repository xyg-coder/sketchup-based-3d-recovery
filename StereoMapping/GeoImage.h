
#ifndef __GEOIMAGE_H__
#define __GEOIMAGE_H__

#include "FxImage.h"
#include "IRasterIO.h"

class CGeoImage : public IRasterIO
{
public:
	CGeoImage();
	virtual ~CGeoImage();

	bool         Open(const char* lpszGeoImage);
	void         Close();

	//implement the IRasterIO interface
	virtual int  GetWidth()  const;
	virtual int  GetHeight() const;
	virtual int  GetBands()  const;
	virtual int  GetBytesPerPixel() const;
	virtual int  GetTileWidth()  const;
	virtual int  GetTileHeight() const;
	virtual void Read(RasterReadType type, int srow, int scol, 
					  int rows, int cols, unsigned char* data, 
					  int reductionFactor);

	//Geo-encoding related functions
	bool         IsGeoEncoding() const;
	void         GetGeoTranslate(double* matrix);

	void         TranslateImage2Geo(double ix, double iy, double& geoX, double& geoY);
	void         TranslateGeo2Image(double geoX, double geoY, double& ix, double & iy);

protected:
	void   CreateCvt();
	void   PseudoColor();
	void   ReQuantityColor(BYTE* lpSrc,int srclinespace, int width, int height, BYTE* lpDst, int dstlinespace );

protected:
	CFxImage     m_ImageReader;

	BYTE*        m_lpTmpBuf;
	int          m_nTmpBufSize;
	
	bool         m_bGeoEncoding;
	bool         m_bIsTiled;


	int*         m_pMinVals;
	int*         m_pMaxVals;
	int          m_bDownQuat;

	float        m_minH;
	float        m_maxH;

	BYTE*        m_lpCvtBuf;
	int          m_nCvtBufSize;
};

#endif //__GEOIMAGE_H__