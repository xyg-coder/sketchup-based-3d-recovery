// MeasureDialog.cpp : implementation file
//

#include "stdafx.h"
#include "StereoMapping.h"
#include "MeasureDialog.h"
#include "afxdialogex.h"
#include "InitSettingDlg.h"
#include "charaTrans.h"
#include "FxImage.h"
#include "location.h"
#include "singleImageView.h"

#include "polylineSing.h"


#ifdef _FILETYPE_EXPR
CString fileOpenType=_T("工程文件(*.xpr)|*.xpr|数据文件(*.xml)|*.xml|模型文件(*.mod)|*.mod||");
#else
CString fileOpenType=_T("工程文件(*.svp)|*.svp|数据文件(*.xml)|*.xml|模型文件(*.mod)|*.mod||");
#endif

// CMeasureDialog dialog

IMPLEMENT_DYNAMIC(CMeasureDialog, CDialog)

CMeasureDialog::CMeasureDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMeasureDialog::IDD, pParent)
{
	m_bstrip=false;
	m_bPolygon=false;
	m_tool=PAN;

	m_bIsCtrlInit = false;

	m_nMultiViewImgIndex = 0;

}

CMeasureDialog::~CMeasureDialog()
{
	//  m_stereoView.DestroyWindow();
}

void CMeasureDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BLOCK_TREE, m_blockTree);
	//DDX_Control(pDX, IDC_LIST1, m_ListCtl);
	DDX_Control(pDX, IDC_TAB1, m_TabImgView);
}


BEGIN_MESSAGE_MAP(CMeasureDialog, CDialog)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_OPEN, &CMeasureDialog::OnFileOpen)
	ON_NOTIFY(TVN_SELCHANGED, IDC_BLOCK_TREE, &CMeasureDialog::OnTvnSelchangedBlockTree)
	ON_COMMAND(ID_CREATE_POLYGON, &CMeasureDialog::OnCreatePolygon)
	ON_UPDATE_COMMAND_UI(ID_CREATE_POLYGON, &CMeasureDialog::OnUpdateCreatePolygon)
	ON_WM_INITMENUPOPUP()

//ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMeasureDialog::OnTcnSelchangeTab1) 
ON_WM_NCLBUTTONDOWN()
ON_WM_MOVE()
//ON_COMMAND(ID_32772, &CMeasureDialog::OnHorizonDatum)
ON_COMMAND(ID_32772, &CMeasureDialog::OnHorizonDatum)
ON_COMMAND(ID_Menu, &CMeasureDialog::OnEnlargeGrid)
ON_COMMAND(ID_32776, &CMeasureDialog::OnDenseGrid)
ON_COMMAND(ID_32773, &CMeasureDialog::OnVerticalDatum)
ON_COMMAND(ID_32774, &CMeasureDialog::OnRandomDatum)
ON_COMMAND(ID_32777, &CMeasureDialog::OnMultiViewImg)
ON_COMMAND(ID_32778, &CMeasureDialog::OnRefreshMultiViewImg)
ON_COMMAND(ID_32779, &CMeasureDialog::OnPolyline)
ON_COMMAND(ID_32782, &CMeasureDialog::OnCircle)
ON_COMMAND(ID_32815, &CMeasureDialog::On32815)
ON_COMMAND(ID_32816, &CMeasureDialog::OnCloseDatumGrid)
ON_COMMAND(ID_32780, &CMeasureDialog::OnPolygon)
ON_COMMAND(ID_32781, &CMeasureDialog::OnArc)
ON_COMMAND(ID_32784, &CMeasureDialog::OnVerticalSwitch)
ON_COMMAND(ID_32817, &CMeasureDialog::OnShowToolState)
ON_UPDATE_COMMAND_UI(ID_32780, &CMeasureDialog::OnUpdatePolygon)
END_MESSAGE_MAP()


// CMeasureDialog message handlers


BOOL CMeasureDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	//设定量测窗口的起始位置
	this->SetWindowPos(NULL, 20, 40,650 , 
		700, SWP_SHOWWINDOW); 

	CRect rect;
	GetClientRect(&rect);


// 	CRect rec(0,0,0,0);
	//m_stereoView.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rec,this,3131);

	//20160608 先不显示立体
// 	m_viewer.Create(CStereoViewDlg::IDD,this);
// 	m_viewer.ShowWindow(SW_SHOW);


	//将量测界面对话框传递到分割条类中

	//20160609 m_TabImgView是选项卡控件 
	//原来这里是显示影像的dialog 现在将显示影像的dialog放到选项卡中
	m_MainSplitterPane.Create(
		WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPCHILDREN|SS_VERT,
		this, &m_blockTree, &m_TabImgView, IDC_VERT_SPLITTER,
		rect,100,200);


	//树控件好像被放到分割条下面了，显示的不正常，尝试解决
	m_blockTree.SetParent(m_TabImgView.GetParent()); //倒是显示到顶层了，但是还是闪


	m_hRoot = m_blockTree.InsertItem(_T("测区"));

	//选项卡添加选项20160609
	m_TabImgView.InsertItem(0,_T("单像视图"));
	m_TabImgView.InsertItem(1,_T("立体视图"));

	//显示影像对话框创建20160609
	m_childDlg.Create(IDD_ChildDlg, &m_TabImgView); //单像
	m_viewer.Create(IDD_DIALOG_STEREOVIEW, &m_TabImgView); //立体

	//放置影像显示对话框20160609
	CRect tabRect;

	m_TabImgView.GetWindowRect(&tabRect); //屏幕坐标

	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;

	m_childDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), 
		tabRect.Height(), SWP_SHOWWINDOW); 
	m_viewer.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), 
		tabRect.Height(), SWP_HIDEWINDOW); 

	m_bIsCtrlInit = true;

	//选项卡包含 影像显示对话框 代码结束 20160609





	m_isFileOpen = false;

	SetViewPosition();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CMeasureDialog::SetViewPosition()
{
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	CRect rect;
	GetClientRect(&rect);
	//int nTreeWidth=300;
	//if (m_blockTree.m_hWnd!=NULL)
	//{
	//	m_blockTree.MoveWindow(0,0,nTreeWidth,rect.Height());
	//}
	//if (m_stereoView.m_hWnd!=NULL)
	//{
	//	m_stereoView.MoveWindow(nTreeWidth,0,rect.Width()-nTreeWidth,rect.Height());
	//}

	if (m_MainSplitterPane.m_hWnd)
	{
		m_MainSplitterPane.MoveWindow(&rect);
	}


	Invalidate(FALSE);
}

void CMeasureDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	SetViewPosition();

	//需要将显示影像的窗体同时缩放
	ReSizeImg();

}


int CMeasureDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here


	return 0;
}


void CMeasureDialog::OnFileOpen()
{
	// TODO: Add your command handler code here
	CFileDialog fdlg(TRUE, NULL, NULL, 0, fileOpenType,  this);
	fdlg.m_ofn.lpstrFile = new TCHAR[1024*1024];   
	memset(fdlg.m_ofn.lpstrFile,0,1024*1024);  
	fdlg.m_ofn.nMaxFile =1024*1024; 
	if(fdlg.DoModal() == IDOK)
	{
		m_proPath = fdlg.GetPathName();
		//CAerialBlock block;
		CString strPathName;

		m_block.Reset();
		//字符转换
		unsigned int size=WideCharToMultiByte(CP_OEMCP,NULL,m_proPath.GetBuffer(),-1,NULL,0,NULL,false);
		char* pPathName=new char[size];
		bool re=WCharToMByte(m_proPath.GetBuffer(),pPathName,size);
		if(!re)
			MessageBox(_T("字符转换出错！"));
		//读取工程文件
		bool bSuc = m_block.LoadFromFile(pPathName);
		if (!bSuc)
		{
			TRACE("文件打开失败！", pPathName);
		}
		else
		{
			//在tree控件显示工程信息
			AddBlockToTree(&m_block);
			m_isFileOpen = true;
		}	

		delete []pPathName;
		pPathName=NULL;
	}
	delete [] fdlg.m_ofn.lpstrFile;

	//以下代码在需要“打开工程并设置纹理存储路径”时使用
	//CInitSettingDlg initDlg;
	//if (initDlg.DoModal() == IDOK)
	//{
	//	//CAerialBlock block;
	//	CString strPathName = initDlg.m_ImagesPath;
	//	m_proPath=initDlg.m_ImagesPath;
	//	m_MapsPath = initDlg.m_MapsPath;


	//	m_block.Reset();
	//	//字符转换
	//	unsigned int size=WideCharToMultiByte(CP_OEMCP,NULL,strPathName.GetBuffer(),-1,NULL,0,NULL,false);
	//	char* pPathName=new char[size];
	//	bool re=WCharToMByte(strPathName.GetBuffer(),pPathName,size);
	//	if(!re)
	//		MessageBox(_T("字符转换出错！"));
	//	//读取工程文件
	//	bool bSuc = m_block.LoadFromFile(pPathName);
	//	if (!bSuc)
	//	{
	//		TRACE("文件打开失败！", pPathName);
	//	}
	//	else
	//	{
	//		//在tree控件显示工程信息
	//		AddBlockToTree(&m_block);
	//	}	

	//	delete []pPathName;
	//	pPathName=NULL;
	//}
}

void CMeasureDialog::AddBlockToTree(CAerialBlock* block)
{
	int nstrips = block->GetNumStrip();

	/*HTREEITEMm_hRoot = m_blockTree.InsertItem(_T("测区"));*/ 
	HTREEITEM hRoot = m_blockTree.GetRootItem();
	if(m_blockTree.ItemHasChildren(hRoot))
	{
		m_blockTree.DeleteItem(hRoot);
		hRoot = m_blockTree.InsertItem(_T("测区"));
	}
	for (int i = 0; i < nstrips; i++)
	{
		CAerialStrip *pStrip = block->GetStrip(i);
		CString sroot = _T("航带 ");
		sroot += pStrip->GetID();
		HTREEITEM hStrip = m_blockTree.InsertItem(sroot, hRoot);

		int nphotos = pStrip->GetNumPhoto();
		for (int j = 0; j < nphotos; j++)
		{
			CAerialPhoto* pPhoto = pStrip->GetPhoto(j);
			CAerialBlock* pBlock = pPhoto->GetBlock();
			WORD data = (WORD)pBlock->GetPhotoIndex(pPhoto);
			//字符转换
			const char* pUrl=pPhoto->GetURL();
			unsigned int size=MultiByteToWideChar(CP_ACP,0,pUrl, -1, NULL, 0);
			TCHAR* pWUrl=new TCHAR[size];
			MByteToWChar(pUrl,pWUrl,size);

			HTREEITEM hPhoto = m_blockTree.InsertItem(pWUrl,hStrip);
			delete[]pWUrl;
			pWUrl=NULL;

			m_blockTree.SetItemData(hPhoto, data);

			const CCamera* lpCam = pPhoto->GetCamera();
			if(lpCam->GetSensorType() == CCamera::SENSOR_CCD)
			{
				int  width, height;
				CCCDCamera* lpCCDCam = (CCCDCamera*)lpCam;			
				lpCCDCam->GetSensorSizeInPixel(width, height);
				pPhoto->SetDimension(width, height);
			}
			else
			{
				CFxImage sxImage;
				sxImage.Open(pPhoto->GetURL());
				pPhoto->SetDimension(sxImage.GetWidth(), sxImage.GetHeight());
				sxImage.Close();
			}			
		}

	}
	m_blockTree.Expand(hRoot,TVE_EXPAND);
	m_viewer.CleanAllContent();
}

//鼠标点击树控件
void CMeasureDialog::OnTvnSelchangedBlockTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	CString sleft, sright, title;
	CAerialPhoto* pleftPhoto =NULL;
	CAerialPhoto* prightPhoto = NULL;
	if (!m_bstrip) 
	{
		HTREEITEM hSel = m_blockTree.GetSelectedItem();
		HTREEITEM hNext = m_blockTree.GetNextSiblingItem(hSel);

		if (m_blockTree.ItemHasChildren(hSel) || hSel == m_hRoot) return;
		sleft = m_blockTree.GetItemText(hSel);

		if (hNext == NULL)
			return;
		else
			sright = m_blockTree.GetItemText(hNext);

		int lImage=int(m_blockTree.GetItemData(hSel));
		int rImage=int(m_blockTree.GetItemData(hNext));
		//pleftPhoto = m_block.GetPhoto(int(m_blockTree.GetItemData(hSel)));
		//prightPhoto = m_block.GetPhoto(int(m_blockTree.GetItemData(hNext)));
		pleftPhoto = m_block.GetPhoto(lImage);
		prightPhoto = m_block.GetPhoto(rImage);
	}
	else
	{
		//HTREEITEM hSel = m_blockTree.GetSelectedItem();
		//if (m_blockTree.ItemHasChildren(hSel) || hSel == m_hRoot) return;
		//sleft = m_blockTree.GetItemText(hSel);
		//pleftPhoto = m_block.GetPhoto(int(m_blockTree.GetItemData(hSel)));
		//m_viewer.FindNearestPhotoInCamera(pleftPhoto, prightPhoto);

		//if (prightPhoto == NULL)
		//	return;
		//sright = prightPhoto->GetURL();
	}

// 	GetWindowText(title);
// 	title = (_T("SVSMPModeler：") + sleft + _T(" --- ") + sright);
// 	SetWindowText(title);

	//m_viewer.SetTools(PAN);


	m_viewer.SetAerialPhoto(pleftPhoto, prightPhoto); //立体量测界面显示影像
	m_viewer.LoadImages();
	m_viewer.SetFocus();


	//m_viewer.m_MapsPath = m_MapsPath;

	//m_viewer.SetTools(m_viewer.GetTool());

	*pResult = 0;
}


void CMeasureDialog::OnCreatePolygon()
{
	// TODO: Add your command handler code here
	//0715 注释掉 原来林哥的代码
// 	UpdateData(TRUE);
// 	m_tool=(POLYGON == m_tool?PAN:POLYGON);
// 	if (POLYGON==m_tool)
// 	{
// 		m_bPolygon=TRUE;
// 	}
// 	else
// 		m_bPolygon=FALSE;
// 	m_viewer.SetTool(m_tool);
// 	UpdateData(FALSE);
}


void CMeasureDialog::OnUpdateCreatePolygon(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//0715 注释掉 原来林哥的代码
	//pCmdUI->SetCheck(m_bPolygon);
}


void CMeasureDialog::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	// TODO: Add your message handler code here
	if(!bSysMenu && pPopupMenu)
	{
		CCmdUI cmdUI;
		cmdUI.m_pOther = NULL;
		cmdUI.m_pMenu = pPopupMenu;
		cmdUI.m_pSubMenu = NULL;

		UINT count = pPopupMenu->GetMenuItemCount();
		cmdUI.m_nIndexMax = count;
		for(UINT i=0; i<count; i++)
		{
			UINT nID = pPopupMenu->GetMenuItemID(i);
			if(-1 == nID || 0 == nID)
			{
				continue;
			}
			cmdUI.m_nID = nID;
			cmdUI.m_nIndex = i;
			cmdUI.DoUpdate(this, FALSE);
		}
	}

}




//20160612 调整显示影像窗体
void CMeasureDialog::ReSizeImg(void)
{
	CRect tabRect;

	if (m_bIsCtrlInit)
	{
		m_TabImgView.GetWindowRect(&tabRect); //屏幕坐标
		tabRect.left += 1;
		tabRect.right -= 1;
		tabRect.top += 25;
		tabRect.bottom -= 1;	

		if (m_TabImgView.GetCurSel() == 0) //单像
		{	
			/*m_childDlg.MoveWindow(tabRect,FALSE);*/
			HDWP hdwp = BeginDeferWindowPos(1);
			HWND hwnd = m_childDlg.m_hWnd;
			hdwp = DeferWindowPos( hdwp,hwnd , NULL, tabRect.left, tabRect.top,
				tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
			EndDeferWindowPos( hdwp );
		}
		else if (m_TabImgView.GetCurSel() == 1) //立体
		{
			/*m_viewer.MoveWindow(tabRect,FALSE);*/
			HDWP hdwp = BeginDeferWindowPos(1);
			HWND hwnd = m_viewer.m_hWnd;
			hdwp = DeferWindowPos( hdwp,hwnd , NULL, tabRect.left, tabRect.top,
				tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
			EndDeferWindowPos( hdwp );
		}

	}


}



/*
void CMeasureDialog::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
		CRect tabRect;    // 标签控件客户区的Rect  
		// 获取标签控件客户区Rect，并对其调整，以适合放置标签页  
		m_TabImgView.GetWindowRect(&tabRect);  
		tabRect.left += 1;  
		tabRect.right -= 1;  
		tabRect.top += 25;  
		tabRect.bottom -= 1;   

		int temp = m_TabImgView.GetCurSel();

		switch (m_TabImgView.GetCurSel())
		{	
		case 0: //选择单像
			m_childDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), 
				tabRect.Height(), SWP_SHOWWINDOW); 
			m_viewer.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), 
				tabRect.Height(), SWP_HIDEWINDOW); 
			break;

		case 1: //选择立体量测界面
			m_childDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), 
				tabRect.Height(), SWP_HIDEWINDOW); 
			m_viewer.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), 
				tabRect.Height(), SWP_SHOWWINDOW); 
			break;

		default:
			break;
	
		}


	*pResult = 0;
}

*/



//201606添加了代码，使得表现出灰色方框效果，窗体并不实时绘制，避免闪
void CMeasureDialog::OnNcLButtonDown(UINT nHitTest, CPoint point) //20160613 改变窗体大小时不刷新
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	// 1，查询当前系统“拖动显示窗口内容”设置 m_bDragFullWindow自定义的变量储存之
	SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0, &m_bDragFullWindow, NULL);  

	// 2，如果需要修改设置，则在每次进入CDialog::OnNcLButtonDown默认处理之前修改
	if(m_bDragFullWindow)
		SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, FALSE, NULL, NULL);

	// 3，默认处理，系统会自动绘制虚框
	CDialog::OnNcLButtonDown(nHitTest, point);

	// 4，默认处理完毕后，还原系统设置
	if(m_bDragFullWindow)
		SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, TRUE, NULL, NULL);

}

//移动窗体，根据选项卡当前显示的
void CMeasureDialog::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: 在此处添加消息处理程序代码
	CRect tabRect;

	if (m_bIsCtrlInit)
	{
		m_TabImgView.GetWindowRect(&tabRect); //屏幕坐标
		tabRect.left += 1;
		tabRect.right -= 1;
		tabRect.top += 25;
		tabRect.bottom -= 1;	

		if (m_TabImgView.GetCurSel() == 0) //单像
		{	
			/*m_childDlg.MoveWindow(tabRect,FALSE);*/
			HDWP hdwp = BeginDeferWindowPos(1);
			HWND hwnd = m_childDlg.m_hWnd;
			hdwp = DeferWindowPos( hdwp,hwnd , NULL, tabRect.left, tabRect.top,
				tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
			EndDeferWindowPos( hdwp );
		}
		else if (m_TabImgView.GetCurSel() == 1) //立体
		{
			/*m_viewer.MoveWindow(tabRect,FALSE);*/
			HDWP hdwp = BeginDeferWindowPos(1);
			HWND hwnd = m_viewer.m_hWnd;
			hdwp = DeferWindowPos( hdwp,hwnd , NULL, tabRect.left, tabRect.top,
				tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
			EndDeferWindowPos( hdwp );
		}

	}


}


//水平基准面菜单事件0627
void CMeasureDialog::OnHorizonDatum()
{
	// TODO: 在此添加命令处理程序代码
	//单像界面
	if (m_TabImgView.GetCurSel() == 0)
	{
		MessageBox(L"请切换到双像立体界面下确定基准面!");
	}
	else //双像
	{
		m_viewer.ChangeMode2Datum(0);
	}
	

}

//扩大格网间隔
void CMeasureDialog::OnEnlargeGrid()
{
	// TODO: 在此添加命令处理程序代码
	//单像界面
	if (m_TabImgView.GetCurSel() == 0)
	{
		
	}
	else //双像
	{
		m_viewer.ChangeGridSize(true);
	}
}

//缩小格网间隔
void CMeasureDialog::OnDenseGrid()
{
	// TODO: 在此添加命令处理程序代码
	//单像界面
	if (m_TabImgView.GetCurSel() == 0)
	{
		
	}
	else //双像
	{
		m_viewer.ChangeGridSize(false);
	}
}

//菜单栏，立面基准
void CMeasureDialog::OnVerticalDatum()
{
	// TODO: 在此添加命令处理程序代码
	//单像界面
	if (m_TabImgView.GetCurSel() == 0)
	{
		MessageBox(L"请切换到双像立体界面下确定基准面!");
	}
	else //双像
	{
		m_viewer.ChangeMode2Datum(1);
	}
}

//任意朝向的基准面
void CMeasureDialog::OnRandomDatum()
{
	// TODO: 在此添加命令处理程序代码
	//单像界面
	if (m_TabImgView.GetCurSel() == 0)
	{
		MessageBox(L"请切换到双像立体界面下确定基准面!");
	}
	else //双像
	{
		m_viewer.ChangeMode2Datum(2);
	}
}

//切换到下一张影像，缩放到相同区域
void CMeasureDialog::OnMultiViewImg()
{
	// TODO: 在此添加命令处理程序代码

	//双像界面
	if (m_TabImgView.GetCurSel() == 1)
	{
		return;
	}

	if (m_vMVStripIndex.size() == 0)
	{
		return;
	}

	//先删掉上一张单像的基准格网
	m_childDlg.getView()->CloseCurrentDatum();

	int i = m_vMVStripIndex[m_nMultiViewImgIndex];
	int j = m_vMVPhotoIndex[m_nMultiViewImgIndex];
	LocationType pt;
	pt.mX = m_vMVPxlCent[m_nMultiViewImgIndex] .m_X;
	pt.mY = m_vMVPxlCent[m_nMultiViewImgIndex].m_Y;

	m_nMultiViewImgIndex = m_nMultiViewImgIndex + 1; //使序号加1

	if (m_nMultiViewImgIndex == m_vMVStripIndex.size()) //如果越界，重新置0
	{
		m_nMultiViewImgIndex = 0;
	}

	CAerialStrip* pstrip = m_block.GetStrip(i);
	CAerialPhoto* pPhoto = pstrip->GetPhoto(j);
	const char* pcPhotoPath = pPhoto->GetURL();
	CString cstrPhotoPath;
	int len = strlen(pcPhotoPath);
	TCHAR* c1 = (TCHAR*)malloc(sizeof(TCHAR)*len);
	MultiByteToWideChar( CP_ACP , 0 , pcPhotoPath , len+1 , c1 , len+1);
	cstrPhotoPath.Format(L"%s",c1);


	m_childDlg.setAerialPhoto(pPhoto); //需要加这一句，不然无法进行计算
	m_childDlg.getView()->LoadImages(cstrPhotoPath);

	//缩放到相应位置，必须先load影像，在缩放到相应位置
	m_childDlg.getView()->ZoomToPoint(pt, 50);
	m_childDlg.SetFocus();


}

//20160629 获取多视影像 不是菜单项，仅供调用
void CMeasureDialog::GetMultiViewImg(double X, double Y, double Z)
{
	if (!m_isFileOpen)
	{
		MessageBox(_T("请先打开图像文件"));
		return;
	}

	int nstrips = m_block.GetNumStrip();

	//清空原有多视影像
	m_vMVStripIndex.clear();
	m_vMVPhotoIndex.clear();

	for (int i = 0; i < nstrips;i++)
	{
		CAerialStrip* pstrip = m_block.GetStrip(i);
		int nphotos = pstrip->GetNumPhoto();
		for (int j = 0; j < nphotos; j++)
		{
			int imageWidth;
			int imageHeight;
			CAerialPhoto* pPhoto = pstrip->GetPhoto(j);
			pPhoto->GetDimension(imageWidth, imageHeight);

			CFxImage img;
			BOOL bOpen = img.Open(pPhoto->GetURL());
			if (!bOpen)
			{
				continue;
			}
			img.Close();

			CPoint2D tempPixelPt;
			pPhoto->ObjectCS2PixelCS(X, Y, Z, tempPixelPt.m_X, tempPixelPt.m_Y);
			if (tempPixelPt.m_X >= imageWidth || tempPixelPt.m_Y > imageHeight ||
				tempPixelPt.m_X < 0 || tempPixelPt.m_Y < 0)
			{
				continue;
			}
			else
			{ //寻找到了多视影像，好开心 (^_^)
				m_vMVStripIndex.push_back(i);
				m_vMVPhotoIndex.push_back(j);
				m_vMVPxlCent.push_back(tempPixelPt);
			}
		}
	}

	m_nMultiViewImgIndex = 0;

}

//刷新多视影像，立体界面下
void CMeasureDialog::OnRefreshMultiViewImg()
{
	// TODO: 在此添加命令处理程序代码

	//单像界面
	if (m_TabImgView.GetCurSel() == 0)
	{
		return;
	}

	m_viewer.GetSpaceCentPnt(m_MVHomoPnt);
	GetMultiViewImg(m_MVHomoPnt.m_X, m_MVHomoPnt.m_Y, m_MVHomoPnt.m_Z);
	
	//调用一下事件函数
	OnMultiViewImg();
}


//显示基准面格网，大爷的，函数名没添加好，单双像都得能控制
void CMeasureDialog::On32815()
{
	// TODO: 在此添加命令处理程序代码
	//单像界面
	if (m_TabImgView.GetCurSel() == 0)
	{
		m_childDlg.getView()->ShowCurrentDatum(m_viewer.m_vGridStartCoor,m_viewer.m_vGridEndCoor);
	}
	else //双像界面
	{
		m_viewer.ShowCurrentDatum();

	}

}

//关闭基准面格网，单双像都控制
void CMeasureDialog::OnCloseDatumGrid()
{
	// TODO: 在此添加命令处理程序代码
	//单像界面
	if (m_TabImgView.GetCurSel() == 0)
	{
		m_childDlg.getView()->CloseCurrentDatum();
	}
	else //双像界面
	{
		m_viewer.CloseCurrentDatum();

	}	

}


//////////////////////////////////////////////////////////////////////////
//绘制图形

//绘制多段线0711
void CMeasureDialog::OnPolyline()
{
	// TODO: 在此添加命令处理程序代码
	//单像界面
	if (m_TabImgView.GetCurSel() == 0)
	{
		//每次测之前获取一次平面信息
		double normX, normY, normZ, X,Y,Z;

		//返回true才可以在单像中画图
		if(m_viewer.GetDatumPlane(normX,normY,normZ,X,Y,Z))
		{
			m_childDlg.getView()->setPlane(normX,normY,normZ,X,Y,Z);
			m_childDlg.getView()->changeTool(POLYLINESing);
		}
		else
		{
			MessageBox(L"请先在双像界面中设置基准面");
		}

	}
	else //在立体界面下也要可以绘制
	{
		m_viewer.SetTool(POLYLINE);
	}
}

//0715 在单双像界面绘制圆形，注意必然是面
void CMeasureDialog::OnCircle()
{
	// TODO: 在此添加命令处理程序代码
	//单像界面
	if (m_TabImgView.GetCurSel() == 0)
	{
		//每次测之前获取一次平面信息
		double normX, normY, normZ, X,Y,Z;

		//返回true才可以在单像中画图
		if(m_viewer.GetDatumPlane(normX,normY,normZ,X,Y,Z))
		{

			if (m_viewer.m_DatumType != HORIZON)
			{
				MessageBox(L"嘿嘿，暂时只支持水平基准");
				return;
			}

			m_childDlg.getView()->setPlane(normX,normY,normZ,X,Y,Z);
			m_childDlg.getView()->changeTool(CIRCLESing);

		}
		else
		{
			MessageBox(L"请先在双像界面中设置基准面");
		}

	}
	else //在立体界面下也要可以绘制
	{
		m_viewer.SetTool(CIRCLE);
	}



}

//0715 在单双像界面绘制多边形
void CMeasureDialog::OnPolygon()
{
	// TODO: 在此添加命令处理程序代码
	//单像界面
	if (m_TabImgView.GetCurSel() == 0)
	{
		//每次测之前获取一次平面信息
		double normX, normY, normZ, X,Y,Z;

		//返回true才可以在单像中画图
		if(m_viewer.GetDatumPlane(normX,normY,normZ,X,Y,Z))
		{
			m_childDlg.getView()->setPlane(normX,normY,normZ,X,Y,Z);
			m_childDlg.getView()->switchPolygonMode();
		}
		else
		{
			MessageBox(L"请先在双像界面中设置基准面");
		}

	}
	else //在立体界面下也要可以绘制
	{
		m_viewer.switchPolygonMode();
	}


}

//0715 在单双像界面绘制弧段
void CMeasureDialog::OnArc()
{
	// TODO: 在此添加命令处理程序代码
	//单像界面
	if (m_TabImgView.GetCurSel() == 0)
	{
		//每次测之前获取一次平面信息
		double normX, normY, normZ, X,Y,Z;

		//返回true才可以在单像中画图
		if(m_viewer.GetDatumPlane(normX,normY,normZ,X,Y,Z))
		{
			if (m_viewer.m_DatumType != HORIZON)
			{
				MessageBox(L"嘿嘿，暂时只支持水平基准");
				return;
			}


			m_childDlg.getView()->setPlane(normX,normY,normZ,X,Y,Z);
			m_childDlg.getView()->changeTool(ARCSing);
		}
		else
		{
			MessageBox(L"请先在双像界面中设置基准面");
		}

	}
	else //在立体界面下也要可以绘制
	{
		m_viewer.SetTool(ARC);
	}

}


//0715 直角化的开关
void CMeasureDialog::OnVerticalSwitch()
{
	// TODO: 在此添加命令处理程序代码
	//单像界面
	if (m_TabImgView.GetCurSel() == 0)
	{
		m_childDlg.getView()->changeVertical();//切换垂直模式
	}
	else //立体界面
	{
		m_viewer.switchVerticalMode();
	}

}


void CMeasureDialog::OnShowToolState()
{
	// TODO: 在此添加命令处理程序代码
	//单像界面
	if (m_TabImgView.GetCurSel() == 0)
	{
		m_childDlg.getView()->getInformation();
	}
	else //立体界面
	{
		m_viewer.getInformation();
	}
}

//0716  控制是否构面开关的对勾效果，切换单双像时要变
void CMeasureDialog::OnUpdatePolygon(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (m_TabImgView.GetCurSel() == 0) //单像
	{
		if (m_childDlg.getView()->m_polygonMode == POLYGONONSing)
		{
			pCmdUI->SetCheck(true);
		}
		else
		{
			pCmdUI->SetCheck(false);
		}
	}
	else //立体
	{
		if (m_viewer.m_polygonMode == POLYGONON)
		{
			pCmdUI->SetCheck(true);
		}
		else
		{
			pCmdUI->SetCheck(false);
		}
	}
}
