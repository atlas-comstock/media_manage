// media manage.h : PROJECT_NAME 应用程序的主头文件
//


#pragma once

#ifndef _meida
#define _media
#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CmediamanageApp:
// 有关此类的实现，请参阅 media manage.cpp
//

class CmediamanageApp : public CWinApp
{
public:
	CmediamanageApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CmediamanageApp theApp;
#endif 