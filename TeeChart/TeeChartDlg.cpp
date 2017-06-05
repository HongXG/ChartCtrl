
// TeeChartDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TeeChart.h"
#include "TeeChartDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
	srand((unsigned) time(NULL));//���������������
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTeeChartDlg �Ի���




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


// CTeeChartDlg ��Ϣ�������

BOOL CTeeChartDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	CRect rect;
	// TODO: �ڴ���Ӷ���ĳ�ʼ������

//	m_HSChartCtrl.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_Tab)->GetWindowRect(rect);
	GetDlgItem(IDC_STATIC_Tab)->ShowWindow(SW_HIDE);
	ScreenToClient(rect);
	m_tab.Create(CMFCTabCtrl::STYLE_3D_ONENOTE,//�ؼ���ʽ��������������ʾ 
		rect,//�ؼ�����
		this,//�ؼ��ĸ�����ָ��
		1,//�ؼ�ID
		CMFCTabCtrl::LOCATION_TOP);//��ǩλ��	
	m_tab.AutoSizeWindow(TRUE);//�����öԻ�����tab��ʾ��������ţ�ͬʱ�����Ի����OnSize��Ϣ


	m_HSChartCtrl.Create(&m_tab,rect,2);
	CChartAxis *pAxis = NULL;
	//��ʼ������
	pAxis = m_HSChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(true);
	pAxis = m_HSChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);
	m_pLineSerie = m_HSChartCtrl.CreateLineSerie();
	UpdateData(FALSE);
	m_tab.AddTab(&m_HSChartCtrl,_T("HightSpeedCtrl"));
	m_tab.SetActiveTab(0);//������ʾ��һҳ��     
	m_tab.ShowWindow(SW_SHOWNORMAL);
	CRect TabBRect,TabTRect;
	m_tab.GetWindowRect(rect);
	ScreenToClient(rect);
	m_tab.GetTabArea(TabTRect,TabBRect);
	rect.top+=TabTRect.Height();


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTeeChartDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
		MessageBox(_T("���ݵ�������С��0"));
		return;
	}
	DWORD dwTimeS,dwTimeE;
	CString str(_T(""));
	unsigned i(0);
	double* pDataX = NULL;
	double* pDataY = NULL;
	//�����ڴ�
	pDataX = new double[m_nPointNum];
	pDataY = new double[m_nPointNum];
	for (i=0;i<(unsigned)m_nPointNum;i++)
	{
		pDataY[i] = randf(-20,20);
		pDataX[i] = i;
	}
	//������Ҫ����ʱ�䣬��ͼ�Ͳ��ڵ�һ��forѭ����ʵ����

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
	str.Format(_T("��ʱ��%d ms"),dwTimeE);
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
/// \brief ��������
/// \param ptr ����ָ��
/// \param data ����ֵ
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
	SetTimer(1,0,NULL);	//��ˢ��Ƶ������Ϊ0�ͻ�ȫ��������
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
