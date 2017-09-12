// InitSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InitSettingDlg.h"

#ifdef _FILETYPE_EXPR
CString fileOpenType1=_T("工程文件(*.xpr)|*.xpr|数据文件(*.xml)|*.xml|模型文件(*.mod)|*.mod||");
#else
CString fileOpenType1=_T("工程文件(*.svp)|*.svp|数据文件(*.xml)|*.xml|模型文件(*.mod)|*.mod||");
#endif

IMPLEMENT_DYNAMIC(CInitSettingDlg, CDialog)

CInitSettingDlg::CInitSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInitSettingDlg::IDD, pParent)
	, m_ImagesPath(_T(""))
	, m_MapsPath(_T(""))
{

}

CInitSettingDlg::~CInitSettingDlg()
{
}

void CInitSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IMAGES_EDIT, m_ImagesPath);
	DDX_Text(pDX, IDC_MAPS_EDIT, m_MapsPath);
}


BEGIN_MESSAGE_MAP(CInitSettingDlg, CDialog)
	ON_BN_CLICKED(IDC_IMAGES_BUTTON, &CInitSettingDlg::OnBnClickedImagesButton)
	ON_BN_CLICKED(IDC_MAPS_BUTTON, &CInitSettingDlg::OnBnClickedMapsButton)
	ON_BN_CLICKED(IDOK, &CInitSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CInitSettingDlg message handlers

void CInitSettingDlg::OnBnClickedImagesButton()
{
	CFileDialog fdlg(TRUE, NULL, NULL, 0, fileOpenType1,  this);
	fdlg.m_ofn.lpstrFile = new TCHAR[1024*1024];   
	memset(fdlg.m_ofn.lpstrFile,0,1024*1024);  
	fdlg.m_ofn.nMaxFile =1024*1024; 
	if(fdlg.DoModal() == IDOK)
	{
		m_ImagesPath = fdlg.GetPathName();	
	}
	delete [] fdlg.m_ofn.lpstrFile;
	UpdateData(FALSE);
}

void CInitSettingDlg::OnBnClickedMapsButton()
{
	BROWSEINFO bi;
	TCHAR buf[MAX_PATH];
	TCHAR path[MAX_PATH];
	bi.hwndOwner = this->GetSafeHwnd();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = buf;
	bi.lpszTitle = _T("选择文件夹");
	bi.ulFlags = BIF_EDITBOX|BIF_NEWDIALOGSTYLE;
	bi.lpfn  = NULL;

	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	LPITEMIDLIST pIDL = SHBrowseForFolder(&bi);
	if(SHGetPathFromIDList(pIDL, path))
		m_MapsPath = path;
	else
		m_MapsPath = _T("");
	CoUninitialize();
	UpdateData(FALSE);
}

void CInitSettingDlg::OnBnClickedOk()
{
	if(m_MapsPath != _T(""))
		OnOK();
}
