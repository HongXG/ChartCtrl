
// TeeChartDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeeChart.h"
#include "TeeChartDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
	srand((unsigned) time(NULL));//重新设置随机种子
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTeeChartDlg 对话框




CTeeChartDlg::CTeeChartDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTeeChartDlg::IDD, pParent)
	, m_nPointNum(16384)
	,m_c_arrayLength(2096)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTeeChartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nPointNum);
}

BEGIN_MESSAGE_MAP(CTeeChartDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONDraw3, &CTeeChartDlg::OnBnClickedButtondraw3)
	ON_BN_CLICKED(IDC_BUTTON_Runing, &CTeeChartDlg::OnBnClickedButtonRuning)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_stop, &CTeeChartDlg::OnBnClickedButtonstop)
	ON_BN_CLICKED(IDC_BUTTON_AllPower, &CTeeChartDlg::OnBnClickedButtonAllpower)
	ON_BN_CLICKED(IDC_BUTTON_stop2, &CTeeChartDlg::OnBnClickedButtonstop2)
	ON_BN_CLICKED(IDC_BUTTON1, &CTeeChartDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_stop3, &CTeeChartDlg::OnBnClickedButtonstop3)
END_MESSAGE_MAP()


// CTeeChartDlg 消息处理程序

BOOL CTeeChartDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	CRect rect;
	// TODO: 在此添加额外的初始化代码

//	m_HSChartCtrl.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_Tab)->GetWindowRect(rect);
	GetDlgItem(IDC_STATIC_Tab)->ShowWindow(SW_HIDE);
	ScreenToClient(rect);
	m_tab.Create(CMFCTabCtrl::STYLE_3D_ONENOTE,//控件样式，如最上面所演示 
		rect,//控件区域
		this,//控件的父窗口指针
		1,//控件ID
		CMFCTabCtrl::LOCATION_TOP);//标签位置	
	m_tab.AutoSizeWindow(TRUE);//可以让对话框随tab显示区域而缩放，同时出发对话框的OnSize消息


	m_HSChartCtrl.Create(&m_tab,rect,2);
	CChartAxis *pAxis = NULL;
	//初始化坐标
	pAxis = m_HSChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(true);
	pAxis = m_HSChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);
	m_pLineSerie = m_HSChartCtrl.CreateLineSerie();
	UpdateData(FALSE);
	m_tab.AddTab(&m_HSChartCtrl,_T("HightSpeedCtrl"));
	m_tab.SetActiveTab(0);//设置显示第一页；     
	m_tab.ShowWindow(SW_SHOWNORMAL);
	CRect TabBRect,TabTRect;
	m_tab.GetWindowRect(rect);
	ScreenToClient(rect);
	m_tab.GetTabArea(TabTRect,TabBRect);
	rect.top+=TabTRect.Height();


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTeeChartDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTeeChartDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTeeChartDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




double CTeeChartDlg::randf(double min,double max)
{
	int minInteger = (int)(min*10000);
	int maxInteger = (int)(max*10000);
	int randInteger = rand()*rand();
	int diffInteger = maxInteger - minInteger;
	int resultInteger = randInteger % diffInteger + minInteger;
	return resultInteger/10000.0;
}




void CTeeChartDlg::OnBnClickedButtondraw3()
{
	UpdateData(TRUE);
	if (m_nPointNum<=0)
	{
		MessageBox(_T("数据点数不能小于0"));
		return;
	}
	DWORD dwTimeS,dwTimeE;
	CString str(_T(""));
	unsigned i(0);
	double* pDataX = NULL;
	double* pDataY = NULL;
	//分配内存
	pDataX = new double[m_nPointNum];
	pDataY = new double[m_nPointNum];
	for (i=0;i<(unsigned)m_nPointNum;i++)
	{
		pDataY[i] = randf(-20,20);
		pDataX[i] = i;
	}
	//由于需要计算时间，画图就不在第一个for循环里实现了

	dwTimeS = GetTickCount();
	m_pLineSerie->ClearSerie();
	m_pLineSerie->AddPoints(pDataX,pDataY,m_nPointNum);
	dwTimeE = GetTickCount();
	dwTimeE -= dwTimeS;
	if (pDataX)
	{
		delete[] pDataX;
	}
	if (pDataY)
	{
		delete[] pDataY;
	}
	str.Format(_T("耗时：%d ms"),dwTimeE);
	SetDlgItemText(IDC_STATIC_T,str);
}


void CTeeChartDlg::OnBnClickedButtonRuning()
{
	KillTimer(1);
	KillTimer(0);
	ZeroMemory(&m_HightSpeedChartArray,sizeof(double)*m_c_arrayLength);
	for (size_t i=0;i<m_c_arrayLength;++i)
	{
		m_X[i] = i;
	}
	m_count = m_c_arrayLength;
	m_pLineSerie->ClearSerie();
	SetTimer(0,1000,NULL);	
}

void CTeeChartDlg::OnBnClickedButtonstop()
{
	KillTimer(0);
}

void CTeeChartDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(0 == nIDEvent)
	{
		++m_count;
		drawMoving();
	}
	if (1==nIDEvent)
	{
		++m_count;
		drawMoving();
	}
	if (2 == nIDEvent)
	{
		++m_count;
		m_pLineSerie->ClearSerie();
		LeftMoveArray(m_HightSpeedChartArray,m_c_arrayLength,randf(0,10));
		LeftMoveArray(m_X,m_c_arrayLength,m_count);
		m_pLineSerie->AddPoints(m_X,m_HightSpeedChartArray,m_c_arrayLength);
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CTeeChartDlg::drawMoving()
{
	m_pLineSerie->ClearSerie();
	RandArray(m_HightSpeedChartArray,m_c_arrayLength);
	LeftMoveArray(m_X,m_c_arrayLength,m_count);
	m_pLineSerie->AddPoints(m_X,m_HightSpeedChartArray,m_c_arrayLength);
}
/// 
/// \brief 左移数组
/// \param ptr 数组指针
/// \param data 新数值
///
void CTeeChartDlg::LeftMoveArray(double* ptr,size_t length,double data)
{
	for (size_t i=1;i<length;++i)
	{
		ptr[i-1] = ptr[i];
	}
	ptr[length-1] = data;
}

void CTeeChartDlg::RandArray(double* ptr,size_t length)
{
	for (size_t i=0;i<length;++i)
	{
		ptr[i] = randf(-1,1);
	}
}

void CTeeChartDlg::OnBnClickedButtonAllpower()
{
	KillTimer(1);
	KillTimer(0);
	ZeroMemory(&m_HightSpeedChartArray,sizeof(double)*m_c_arrayLength);
	for (size_t i=0;i<m_c_arrayLength;++i)
	{
		m_X[i] = i;
	}
	m_count = m_c_arrayLength;
	m_pLineSerie->ClearSerie();
	SetTimer(1,0,NULL);	//当刷新频率设置为0就会全功率运行
}


void CTeeChartDlg::OnBnClickedButtonstop2()
{
	KillTimer(1);
}


void CTeeChartDlg::OnBnClickedButton1()
{
	KillTimer(0);
	KillTimer(1);
	KillTimer(2);
	ZeroMemory(&m_HightSpeedChartArray,sizeof(double)*m_c_arrayLength);
	for (size_t i=0;i<m_c_arrayLength;++i)
	{
		m_X[i] = i;
	}
	m_count = m_c_arrayLength;
	m_pLineSerie->ClearSerie();
	SetTimer(2,0,NULL);	
}


void CTeeChartDlg::OnBnClickedButtonstop3()
{
	KillTimer(2);
}
