
#include "stdafx.h"
#include "GeoImage.h"

static int s_bandmap[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

CGeoImage::CGeoImage():m_ImageReader(),
m_lpTmpBuf(NULL),
m_nTmpBufSize(0),m_bGeoEncoding(false),m_bIsTiled(false),
m_minH(-1000),m_maxH(100000)
{
	m_pMinVals = NULL;
	m_pMaxVals = NULL;
	m_bDownQuat = false;

	m_lpCvtBuf = NULL;
	m_nCvtBufSize = 0;	
}

CGeoImage::~CGeoImage()
{
	m_ImageReader.Close();

	if(m_lpTmpBuf) delete [] m_lpTmpBuf;

	if(m_lpCvtBuf) delete [] m_lpCvtBuf;

	if(m_pMinVals) delete [] m_pMinVals;
	if(m_pMaxVals) delete [] m_pMaxVals;
}


bool  CGeoImage::Open(const char* lpszImageFile)
{
	if( m_ImageReader.Open(lpszImageFile) )
	{
		int nBands = GetBands();
		CFxImage::SAMPLETYPE eType = m_ImageReader.GetSampleType();

		if( eType == CFxImage::SMP_UInt16 || eType == CFxImage::SMP_Int16)
		{
			CreateCvt();
		}
		else if(eType == CFxImage::SMP_Float32 && nBands == 1)
		{
			PseudoColor();
		}
		else if( eType != CFxImage::SMP_BYTE)
		{
			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}


void  CGeoImage::Close()
{
	m_ImageReader.Close();
}


void  CGeoImage::CreateCvt()
{
	int nBands = GetBands();
	m_bDownQuat = true;

	m_pMinVals = new int[nBands];
	m_pMaxVals = new int[nBands];

	for( int i = 0; i < nBands; i++)
	{
		m_pMaxVals[i] = 0;
		m_pMinVals[i] = 65534;
	}

	int _pixBytes = m_ImageReader.GetDataTypeSize(m_ImageReader.GetSampleType())*nBands;
	int nSize = 512* GetWidth() * _pixBytes ;
	BYTE* lpTmp = new BYTE[nSize];
	int nLines = 512;
	int nLoops = GetHeight() / 512;
	int nRes   = GetHeight() % 512;

	if(nLoops == 0)
		nLines = nRes;

	for(int i = 0; i < 1/*nLoops*/; i++)
	{
		m_ImageReader.ReadPlannarImage(i*512, 0, nLines, GetWidth(), nBands, s_bandmap, lpTmp);

		for(int b = 0; b < nBands; b++)
		{
			unsigned short * lpBandBuf = (unsigned short*)lpTmp + b * GetWidth() * nLines;

			for(int r = 0; r < nLines; r++)
			{
				for(int c = 0; c < GetWidth(); c++)
				{
					unsigned short v = *(lpBandBuf + r * GetWidth() + c);
					if(m_pMinVals[b] > v)  m_pMinVals[b] = v;
					if(m_pMaxVals[b] < v)  m_pMaxVals[b] = v;
				}
			}
		}
	}

	delete [] lpTmp;
}

void      CGeoImage::PseudoColor()
{
	m_bDownQuat = true;

	int _pixBytes = m_ImageReader.GetDataTypeSize(m_ImageReader.GetSampleType());
	int nSize   = 512* GetWidth() * _pixBytes ;
	BYTE* lpTmp = new BYTE[nSize];

	int nLines = 512;
	int nLoops = GetHeight() / 512;
	int nRes   = GetHeight() % 512;

	if(nLoops == 0)
		nLines = nRes;

	for(int i = 0; i < 1/*nLoops*/; i++)
	{
		m_ImageReader.ReadPlannarImage(i*512, 0, nLines, GetWidth(), 1, s_bandmap, lpTmp);

		for(int r = 0; r < nLines;  r++)
		{
			float* lpRd = (float*)lpTmp + r * GetWidth() * GetBands();

			for(int c = 0; c < GetWidth(); c++)
			{				
				if(m_minH < *lpRd)  m_minH = *lpRd;
				if(m_maxH > *lpRd)  m_maxH = *lpRd;
			}
		}
	}

	delete [] lpTmp;
}

//implement the IRasterIO interface
int  CGeoImage::GetWidth()  const
{
	return m_ImageReader.GetWidth();
}

int  CGeoImage::GetHeight() const
{
	return m_ImageReader.GetHeight();
}

int  CGeoImage::GetBands()  const
{
	int bands = m_ImageReader.GetBands();
	if(bands >= 3) bands = 3;
	else          bands = 1;
	return bands;
}

int  CGeoImage::GetBytesPerPixel() const
{
	return CFxImage::GetDataTypeSize(m_ImageReader.GetSampleType()) *GetBands();
}

int  CGeoImage::GetTileWidth()  const
{
	if(m_bIsTiled)
	{
		return m_ImageReader.GetTileWidth();
	}
	else
		return 512;
}

int  CGeoImage::GetTileHeight() const
{
	if(m_bIsTiled)
	{
		return m_ImageReader.GetTileHeight();
	}
	else
		return 512;
}

//创建低分辨率影像目前采用CPU来实现，将来可以采用GPU来实现，可以提高影像读取速度
void CreateLowResTexData(unsigned char* pSourceData, unsigned char* pDestDataPtr, int texSizeX,
						 int texSizeY, int channels, int reductionFactor)
{
	const int MAX_CHANNELS = 4;
	int j, k, l;
	int texFactorChannel = reductionFactor*channels;
	int texSizeXFactorChannel = texFactorChannel * texSizeX;

	unsigned char *pDestChannelPtr = NULL;
	unsigned char* pBase = NULL;
	unsigned char* pSourceBase = NULL;

	pDestChannelPtr = pDestDataPtr;

	for (j = 0; j < texSizeY; j += reductionFactor)
	{
		pSourceBase = pSourceData;
		for (l = 0; l < texSizeX; l += reductionFactor)
		{
			pBase = pSourceBase;

			for (k = 0; k < channels; ++k)
			{
				*pDestChannelPtr = *pBase;
				++pDestChannelPtr;
				++pBase;
			}

			pSourceBase += texFactorChannel;
		}
		pSourceData += texSizeXFactorChannel;
	}
}

void   CGeoImage::ReQuantityColor(BYTE* lpSrc, int srclinespace, int width, int height, BYTE* lpDst,  int dstlinespace)
{
	int nBands = GetBands();
	if(m_bDownQuat && (m_pMaxVals  || m_pMaxVals))
	{
	
		unsigned short* lpUInt16Src = (unsigned short*) lpSrc;

		for(int i = 0; i < height; i++ )
		{
			unsigned short* lpLocSrc = lpUInt16Src + i * width * nBands;

			BYTE* lpLocDst  = lpDst + i*dstlinespace;
			for(int j = 0; j < width; j++)
			{
				for(int b = 0; b < nBands; b++)
				{
					int val = (int) (((float)lpLocSrc[b] - (float)m_pMinVals[b])/((float)m_pMaxVals[b] - (float)m_pMinVals[b])*255.0f);
					if(val > 255 ) val = 255;
					if(val < 0 ) val = 0;

					lpLocDst[b] = (BYTE)val;
					/*float scale = log(lpLocSrc[b] + 1.0)/log(m_pMaxVals[b] + 1.0);
					int val = (int ) ( scale * 255.0);
					lpLocDst[b] = val > 255 ? 255 : val;*/
				}

				lpLocDst += nBands;
				lpLocSrc += nBands;
			}
		}
		//memset(lpDst, 255, height * dstlinespace);
	}
	else
	{
		float* lpFLT32Src = (float*) lpSrc;

		for(int i = 0; i < height; i++ )
		{
			float* lpLocSrc = lpFLT32Src + i*(srclinespace/sizeof(float));
			BYTE*  lpLocDst  = lpDst + i*dstlinespace;
			for(int j = 0; j < width; j++)
			{
				lpLocDst[j] = (BYTE)((lpLocSrc[j] - m_minH)/(m_maxH - m_minH)*255.0f);
			}
		}
	}
	
}

void   CGeoImage::Read(RasterReadType type, int srow, int scol, 
								int rows, int cols, unsigned char* data, 
								int reductionFactor)
{
	int nBands = GetBands();	
	int  readrows = rows*reductionFactor;
	int  readcols = cols*reductionFactor;

	int _pixBytes = m_ImageReader.GetDataTypeSize(m_ImageReader.GetSampleType())*nBands;

	if(m_bDownQuat)
	{
		int cvtSize = readrows*readcols*_pixBytes;
		if(m_nCvtBufSize < cvtSize)
		{
			if(m_lpCvtBuf ) delete [] m_lpCvtBuf;
			m_lpCvtBuf = new BYTE[cvtSize];
			m_nCvtBufSize = cvtSize;
		}
	}

	if(reductionFactor != 1)
	{
		int tmpSize = readrows*readcols*nBands;

		if(m_nTmpBufSize < tmpSize)
		{
			if(m_lpTmpBuf) delete [] m_lpTmpBuf;

			m_lpTmpBuf = new BYTE[tmpSize];
			m_nTmpBufSize = tmpSize;
		}

		if(m_bDownQuat)
		{
			m_ImageReader.ReadInterleavingImage(srow, scol,readrows, readcols, 
				                                nBands,s_bandmap,m_lpCvtBuf, readcols*_pixBytes);
			ReQuantityColor(m_lpCvtBuf,readcols*_pixBytes, readcols, readrows, m_lpTmpBuf, readcols*nBands);
		}
		else
		{
			m_ImageReader.ReadInterleavingImage(srow, scol,readrows, readcols, 
				                                nBands,s_bandmap,m_lpTmpBuf, readcols*nBands);
		}		

		CreateLowResTexData(m_lpTmpBuf, data, readrows, readcols, nBands, reductionFactor);			

	}
	else
	{
		//if(bPartial)	memset(data, 0, rows*cols*m_nBands);

		if(m_bDownQuat)
		{
			m_ImageReader.ReadInterleavingImage(srow, scol,readrows, readcols, 
				nBands,s_bandmap,m_lpCvtBuf, readcols*_pixBytes);
			ReQuantityColor(m_lpCvtBuf, readcols * _pixBytes, readcols, readrows, data, readcols*nBands);
		}
		else
		{
			if( !m_ImageReader.ReadInterleavingImage(srow, scol, readrows, readcols, 
				nBands,s_bandmap,data,readcols*nBands))
			{
				memset(data, 255, rows*cols*nBands);
			}
		}
	}
	
}


//Geo-encoding related functions
bool CGeoImage::IsGeoEncoding() const
{
	return m_bGeoEncoding;
}

void CGeoImage::GetGeoTranslate(double* matrix)
{
	double* lpMatrix = m_ImageReader.GetGeoTransform();
	memcpy(matrix, lpMatrix, sizeof(double)*6);
}

void CGeoImage::TranslateImage2Geo(double ix, double iy, double& geoX, double& geoY)
{
	m_ImageReader.CSImageToCSGeo(ix, iy, &geoX, &geoY);
}

void CGeoImage::TranslateGeo2Image(double geoX, double geoY, double& ix, double & iy)
{
	m_ImageReader.CSGeoToCSImage(geoX, geoY, &ix, &iy );
}