
// TeeChart.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTeeChartApp:
// �йش����ʵ�֣������ TeeChart.cpp
//

class CTeeChartApp : public CWinApp
{
public:
	CTeeChartApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTeeChartApp theApp;