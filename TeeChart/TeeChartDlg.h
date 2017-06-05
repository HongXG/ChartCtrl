
// TeeChartDlg.h : ͷ�ļ�
//

#pragma once

#include "ChartCtrl\ChartCtrl.h"
#include "ChartCtrl\ChartLineSerie.h"
#include "ChartCtrl\ChartAxis.h"
// CTeeChartDlg �Ի���
class CTeeChartDlg : public CDialogEx
{
// ����
public:
	CTeeChartDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TEECHART_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMFCTabCtrl m_tab;
	CChartCtrl m_HSChartCtrl;
	CChartLineSerie* m_pLineSerie;

	double randf(double min,double max);
	// ��ͼ����
	long m_nPointNum;
	
	afx_msg void OnBnClickedButtondraw3();
	afx_msg void OnBnClickedButtonRuning();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonstop();
private:
	void drawMoving();
	double m_HightSpeedChartArray[2096];
	double m_X[2096];
	unsigned int m_count;
	const size_t m_c_arrayLength;
	void LeftMoveArray(double* ptr,size_t length,double data);
	void RandArray(double* ptr,size_t length);
public:
	afx_msg void OnBnClickedButtonAllpower();
	afx_msg void OnBnClickedButtonstop2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonstop3();
};
