#pragma once
#include "fxImage.h"
#include "AerialBase.h"
#include "AerialPhotoRender.h"
#include "render.h"
#include "point3.h"
#include "AeriaGeoModel.h"
#include <vector>
#include "resource.h"
#define DraggingThresh 10
#define MoveRefThresh 5
// CStereoViewDlg dialog
class entity;
class polygonBuilding;
class polygon;

enum TOOLS{CIRCLE, ARC, POLYLINE,
	PAN, CUBOID,COPY,RIDGE2,TIN_TRAINGLE,SPHERE,BALCONY,T_WINDOW,PARAPET};

//0627
enum PROCEDURE{DRAW,MODIFY, DATUM};

enum LINESTATE{ROOF,SIDE};
enum SIDESTATE{ROOFPOINT,BOTTOMPOINT};
enum VERTICALMODE{ON,OFF};//垂直化开关;

//0712 桂
enum POLYGONMODE{POLYGONON,POLYGONOFF};//平面绘画开关;
//改动结束 0712

enum MODIFYSTATE{ DRAGGING, MOVEREF, NEAUTRAL };

//0627
enum DATUMTYPE{NODATUM, HORIZON, VERTICAL, RANDOM};


inline double pointDist(double x1, double y1, double x2, double y2)
{
	double r = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
	return r;
}

//绘画中所需要的信息;
struct lastPoint
{
	lastPoint()
	{
		m_lLastPt = NULL;
		m_rLastPt = NULL;
		m_isLastPointSet = false;
	}
	CPoint3D* m_lLastPt;
	CPoint3D* m_rLastPt;//已经确定的绘画的最后一点（鼠标移动时不记录）;
	bool m_isLastPointSet;
};


class CStereoViewDlg : public CDialog
{
	DECLARE_DYNAMIC(CStereoViewDlg)

public:
	CStereoViewDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStereoViewDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_STEREOVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	CAerialPhoto* m_lpAerialPhoto[2];
	CRender           m_Render;
	CAerialPhotoRender m_LViewer;
	CAerialPhotoRender m_RViewer;
	CAeriaGeoModel     m_AerialGeoModel;
private:
	double m_iniScale;
	bool isLoadImage;
	double m_panStep;//手动设置步进
	CPoint m_MouseLastPoint;//移动影像鼠标之前位置
	double m_panMulti;//视差平移倍数

	CEditLayer m_LLineLayer;
	CEditLayer m_RLineLayer;
	CEditLine* m_pLLine;
	CEditLine* m_pRLine;
	entity* m_editedEntity;
	polygonBuilding*m_editedBuilding;

	polygon* m_editedPolygon; //0712 桂



	std::vector<entity*>m_entities;//最顶层的entities;





	//drawInformation m_information;
	lastPoint m_lastPt;
	TOOLS              m_tool;//建模工具
	PROCEDURE m_procedure;
	bool m_isDrawingBuilding;//如果是正在画building则把任务交给类去做;
	//如果不是正在画building，则在此类中做;
	LINESTATE m_lineState;
	SIDESTATE m_sideState;
	MODIFYSTATE m_modifyState;
	VERTICALMODE m_verticalMode;
	CPoint m_before;
	CPoint m_new;

public:

	//0712 桂
	POLYGONMODE m_polygonMode;
	//改动结束 0712

	int m_flowNum;/*测量标识,-1为尚未开始测量，0为尚未开始vector，1为已开始vector但尚未开始
				  新的entity，2为正在entity中,3则为对侧边进行测量*/

private:
	
	//************************************
	// Method:    AddPolygonBuilding
	// FullName:  CStereoViewDlg::AddPolygonBuilding
	// Access:    private 
	// Returns:   void
	// Qualifier:
	// Parameter: UINT nFlags  传递鼠标动作标识,1为LButtonDown,2为LButtonUp,3为mouseMove,4为RButtonDown;
	// Parameter: CPoint point  鼠标所在屏幕坐标
	//************************************
	void AddPolygonBuilding(UINT nFlags, CPoint point);


public:
	void SetAerialPhoto(CAerialPhoto* lpLeftPhoto, CAerialPhoto* lpRightPhoto);
	void LoadImages();
	void CleanAllContent(void);
	void TranslateScreenToLocal(CPoint p, double& lwx, double& lwy, double& rwx, double& rwy);
	void SetTool(TOOLS tool);//设置建模功能

//////////////////////////////////////////////////////////////////////////
	//基准格网 0626 李

	//当前缩放按了多少次，判断缩放到多大时显示格网 10次以上才显示格网
	int m_nImgScale;

	//NODATUM 表示还没测基准
	DATUMTYPE m_DatumType;

	//0：未测点  1：测一点  2：测二点
	int m_nClickMode;

	//是否显示基准格网
	bool m_bShowDatum;

	std::vector<CEditLine*> m_vLDatumGird;
	std::vector<CEditLine*> m_vRDatumGird;

	CPoint3D m_datumCent; //格网中心 第一点
	CPoint3D m_datumPnt2;  //第二点
	CPoint3D m_datumPnt3;  //第三点

	double m_dGridSize; //默认值1米
	double m_dGridRange; //默认值100米

	//立面基准使用的变量
	double m_dRangeX, m_dRangeY; //立面的两端的坐标差值，当前鼠标点是在中间
	double m_dSizeX, m_dSizeY; //每一网格的坐标差值

	//任意基准中与面垂直
	double m_dRangeX2, m_dRangeY2, m_dRangeZ;
	double m_dSizeX2, m_dSizeY2, m_dSizeZ; 

	//测量点时应该立即进行前方交会获得空间坐标，不可储存鼠标坐标留待下一次计算
	//这些函数是在新建基准格网时调用，需要鼠标点作为参数
	void ShowHorDatumGrid(CPoint mousePnt);
	void ShowVerDatumGrid(CPoint mousePnt2);
	void ShowRanDatumGrid(CPoint mousePnt3);

	void GetSpaceCoor(CPoint point, CPoint3D & resultPnt);

	//格网，不能和模型线、辅助线混到一起，虽然函数功能完全一样
	//因为并不同时重绘，所以有各自的vector和绘制刷新函数
	void DrawGrid(int lineNum,double* x1,double* y1,double* z1,
		double* x2,double* y2,double* z2);
	//0 1 2 水平 立面 任意
	void ChangeMode2Datum(int nDatumType);
	void ChangeGridSize(bool isEnlarge);

	//基准面格网存储坐标
	std::vector<double> m_vGridStartCoor;
	std::vector<double> m_vGridEndCoor;

	//当前基准面：法向量，平面上一点
	double m_dNormX, m_dNormY, m_dNormZ, m_dX, m_dY, m_dZ;

	//获得当前基准面，如果还未设置基准面，返回false
	bool GetDatumPlane(double & normX, double & normY, double & normZ, 
		double & X, double & Y, double & Z);


	//多视影像，自动采集中心的点坐标
	void GetSpaceCentPnt(CPoint3D & spaceCentPnt);

	//0712 调用CA库，在SU中绘制模型
	void DrawEntityInSU();


	//0714 关闭/显示 已有的基准格网
	void CloseCurrentDatum();
	void ShowCurrentDatum();


	//偏移量，启动窗体的时候获取
	double m_dXOffset;
	double m_dYOffset;
	double m_dZOffset;


//////////////////////////////////////////////////////////////////////////



	//修改模式;
	void moveref(CPoint pointBefore, CPoint pointNew);
	CPoint& getNearestRef(CPoint pointBefore, double &dist);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
//	afx_msg void OnMove(int x, int y);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void switchProcedure();

	public:
	afx_msg void switchVerticalMode();

	//0712 桂
	afx_msg void switchPolygonMode();//对面模式进行切换
	afx_msg void getInformation();//获取当前状态值;
	afx_msg void deleteEntities(std::vector<entity*>&entVec);
	//默认是左片的光线所求平面
	afx_msg void getRayByMouse(double &dX,double &dY,double&dZ,
		double &sourceX,double &sourceY,double &sourceZ);
	//修改完毕


	//afx_msg void OnHorizonDatum();
};
