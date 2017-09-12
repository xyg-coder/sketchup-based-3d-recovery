#pragma once
#include "PixelViewerView.h"
#include "charaTrans.h"
#include <vector>
//#include "AerialBase.h"
// singleImageView 视图
#define MOVEREFTHRESH 5
#define BIGTHRESH 10
//用于选点时的阈值，可以适当选大一些；
class GraphicPolyline;
class GraphicPoint;
class GraphicLayer;
class CAerialPhoto;
class polygonSing;
//None为当前没有进行任何操作;
//NewVector为进行新的Vector段的操作;
//NewEntity为切换到了新的Tool，前面已经有点;
//InEntity为正在进行绘画当中;
enum DRAWSTEPSing{NoneSing,NewVectorSing,NewEntitySing,InEntitySing};
enum TOOLSing{ POLYLINESing, ARCSing, CIRCLESing };
enum procedureSing{DRAWSing,MODIFYSing};//此时是在绘画还是修改状态;
enum verticalStateSing{OFFSing,ONSing};
enum DRAGSing{DRAGGINGSing,MOVEREFSing,NEAUTRALSing};//当前的modify状态;

//0712 桂
enum LAYERSing{DrawLayer,ReprojectLayer,ReprojectPreviewLayer};
enum POLYGONMODESing{POLYGONONSing,POLYGONOFFSing};
//改动结束 0712

class entitySing;

/*
struct linePointIndex
{
	int m_firstIndex;
	int m_lastIndex;
};*/

//定义一个方向的向量;
struct directionSing
{
	double m_dx;
	double m_dy;
	double m_dz;
	void setDirection(double dx,double dy,double dz)
	{
		m_dx=dx;
		m_dy=dy;
		m_dz=dz;
	}

	//进行叉乘运算并进行归一化;
	void crossProduct(double dx,double dy,double dz,
		double &resultX,double &resultY,double &resultZ)
	{
		resultX=m_dy*dz-m_dz*dy;
		resultY=-(m_dx*dz-m_dz*dx);
		resultZ=m_dx*dy-m_dy*dx;
		double length = sqrt(resultX*resultX + resultY*resultY + resultZ*resultZ);
		resultX /= length;
		resultY /= length;
		resultZ /= length;
	}
};

/*//用于进行两点根据垂直关系确定中间点的操作;
//其中nextPoint一般是每段线段的第一个点;
//lastPoint为刚刚点下的点;

struct lastNextPoint
{
	lastNextPoint()
	{
		m_isLastPointSet=false;
		m_isNextPointSet=false;
		m_isAllSet=false;
	}
	//CPoint m_lastPoint;
	//CPoint m_nextPoint;
	double m_lastx;
	double m_lasty;
	double m_nextx;
	double m_nexty;
	bool m_isLastPointSet;
	bool m_isNextPointSet;
	bool m_isAllSet;
	void setLastPoint(double x,double y)
	{
		//m_lastPoint=point;
		m_lastx=x;
		m_lasty=y;
		m_isLastPointSet=true;
		if (m_isNextPointSet)
		{
			m_isAllSet=true;
		}
	}
	void setNextPoint(double x,double y)
	{
		//m_nextPoint=point;
		m_nextx=x;
		m_nexty=y;
		m_isNextPointSet=true;
		if (m_isLastPointSet)
		{
			m_isAllSet=true;
		}
	}
	void initial()
	{
		m_isLastPointSet=false;
		m_isNextPointSet=false;
		m_isAllSet=false;
	}
};
*/

struct lastPointSing
{
	lastPointSing()
	{
		m_lastPoint = NULL;
		m_isLastPointSet = false;
	}
	GraphicPoint* m_lastPoint;
	bool m_isLastPointSet;
	void getLastPoint(double &x, double &y)
	{
		m_lastPoint->GetPos(x, y);
	}
};

class singleImageView : public CPixelViewer
{
	DECLARE_DYNCREATE(singleImageView)

public:
	singleImageView();           // 动态创建所使用的受保护的构造函数
	virtual ~singleImageView();

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
private:
	//CString m_proPath;
	GraphicLayer* m_pPointLayer; //原来的多段线图层在下面，被改成public了

	//0712 桂
	GraphicLayer* m_pReprojectPointLayer;
	GraphicLayer* m_pReprojectLineLayer;
	GraphicLayer* m_pRePreviewPtLayer;
	GraphicLayer* m_pRePreviewLineLayer;
	//修改结束


	GraphicPolyline* m_pPolyline;
	CAerialPhoto* m_pAerialPhoto;
	entitySing* m_editedEntity;
	CPoint m_point0;
	//CAerialBlock m_block;
	CString filePath;
	bool m_Initialized;//判断是否已经对图像路径初始化;
	int m_selVtx;//选择的端点的index,-1为无效;
	float m_t;//点距离容差;
	procedureSing m_procedure;
	TOOLSing m_tool;
	DRAWSTEPSing m_step;
	verticalStateSing m_verticalState;
	lastPointSing m_lastPoint;//用于从一个tool切换到另一个tool
	directionSing m_mainDirection;//该平面的法线方向;
	bool m_isMainCurrentDirectionSet;//是否存在平面法线方向;
	double m_x0;
	double m_y0;
	double m_z0;
	bool m_isPlaneDecided;//该平面方程是否已经确定;
	CPoint m_beforePoint;
	CPoint m_newPoint;
	DRAGSing m_modifyState;//修饰条件的状态;

public:
	//0712 桂
	POLYGONMODESing m_polygonMode;
	polygonSing* m_editedPolygon;
	//改动结束



	/*bool m_firstPoint;//是否是该条线的第一个点;
	bool m_firstLine;//是否是该图像的第一条线;
	int m_tmpPointSize;//目前已经有了几个点;
	int m_tmpLinePointSize;//记录该线中已有点数;

	//以下与垂直化有关;
	bool m_drawingLine;//正在画点（针对画点时鼠标移动起作用）;
	int m_virticalState;//判断垂直化条件的状态;
	direction m_currentDirection;//当前的作用方向(用于直角化);
	int m_currDireIndex[2];//当前作用方向的两个点的index;
	direction m_mainDirection;//该平面的法线方向;
	lastNextPoint m_twoPoints;//上一点和最后一点，用于两点确定一点操作;
	bool m_isCurrentDirectionSet;//是否存在当前作用方向;
	bool m_isMainCurrentDirectionSet;//是否存在平面法线方向;
	double m_x0;
	double m_y0;
	double m_z0;
	bool m_isPlaneDecided;//该平面方程是否已经确定;
	int m_modifyState;//当前的modify出于什么状态;
	int m_lineNum;//加线时注意是加入id为m_lineNum+1的线;
	CPoint m_beforePoint;
	CPoint m_newPoint;
	std::vector<linePointIndex> m_linesIndex;
	linePointIndex m_tmpLineIndex;*/

/*
	void setCurrentDirection(double dx,double dy,double dz)
	{
		m_currentDirection.setDirection(dx,dy,dz);
		m_isCurrentDirectionSet=true;
	}*/
	void setMainDirection(double dx,double dy,double dz)
	{
		m_mainDirection.setDirection(dx,dy,dz);
		m_isMainCurrentDirectionSet=true;
	}

public:
	void setPlane(double A,double B,double C,double x0,double y0,double z0)
	{
		setMainDirection(A,B,C);
		m_x0=x0;
		m_y0=y0;
		m_z0=z0;
		m_isPlaneDecided=true;
	}

	//传入x,y，根据目前的verticalState算出正确的properX,properY;
	//其中两点计算垂直点我还不会计算...可以后期用牛顿迭代法进行运算;
	//void setProperPoint(double x,double y,double &properX,double &properY);

public:
	GraphicLayer* m_pPolylineLayer;
	//GraphicPoint** m_vtxs;
	//GraphicPolyline** m_plines;
	std::vector<entitySing*> m_entities;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void setAerialPhoto(CAerialPhoto* photo)
	{
		m_pAerialPhoto=photo;
	}
	afx_msg bool LoadImages(CString path);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//afx_msg void selectVtx(CPoint point);//选择离point最近的点;
	//afx_msg void moveVtx(CPoint point1, CPoint point0);
	afx_msg void addPoint(int n,double *x,double *y,double *z);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void changeMode();
	afx_msg void changeTool(TOOLSing tool);
	afx_msg void changeVertical();

	//0712 桂
	afx_msg void switchPolygonMode();
	afx_msg void getInformation();
	//用于将某一个vector中的entities全部删除;
	afx_msg void clearEntities(LAYERSing layer);
	afx_msg void getRayByMouse(double &dX,double &dY,double &dZ,
		double &sourceX,double& sourceY,double& sourceZ);
	//修改结束



	//用于已知平面方程计算待测点三维坐标;
	//x,y为待求点像素坐标;
	//A,B,C,x0,y0,z0为平面方程参数;
	//X,Y,Z为待求点三维坐标;
	afx_msg void singleImageCalculate(double x,double y,double A,double B,double C,
		double x0,double y0,double z0,double &X,double&Y,double &Z);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//传入像素坐标;
	afx_msg void moveref(double oldx,double oldy,double newx,double newy);
	//返回距离给定的像素点最近的点的距离;
	afx_msg CPoint getNearestRef(double x,double y,double &minDist/*,int &index*/);
	//删除所有点和线;
	afx_msg void deleteAll();
	//根据世界坐标求出相应三维坐标;
	afx_msg void getRealCoor(double x, double y, double &X, double &Y, double &Z);
	//根据三维坐标求出相应世界坐标;
	afx_msg void getPointCoor(double X, double Y, double Z, double &x, double &y);


	//////////////////////////////////////////////////////////////////////////
	//0712 Li 添加
	//使用CA库，在SU中绘制模型
	void DrawEntityInSU();


	//反投
	//储存一段段的物方模型线
	std::vector<GraphicPolyline*> m_vObjLine;
	//动态预览辅助线
	std::vector<GraphicPolyline*> m_vAuxLine;

	//偏移量，启动窗体的时候获取
	double m_dXOffset;
	double m_dYOffset;
	double m_dZOffset;

	//基准面格网
	//是否显示基准格网
	bool m_bShowDatum;
	//基准面格网vector
	std::vector<GraphicPolyline*> m_vDatumGird;

	//显示、隐藏当前格网，每次绘制需要从双像那里获取信息
	void ShowCurrentDatum(std::vector<double> & gridStartCoor, std::vector<double> & gridEndCoor);
	void CloseCurrentDatum();

	//格网，不能和模型线、辅助线混到一起，虽然函数功能完全一样
	//因为并不同时重绘，所以有各自的vector和绘制刷新函数
	void DrawGrid(int lineNum,double* x1,double* y1,double* z1,
		double* x2,double* y2,double* z2);


	//0712
	//////////////////////////////////////////////////////////////////////////
};