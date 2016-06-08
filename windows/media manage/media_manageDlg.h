// media manageDlg.h : 头文件
//
#pragma once
#define WM_SHOWTASK (WM_USER+200)
#ifndef _meida
#define _media
#ifndef OS_WIN
#define OS_WIN
#ifndef  COMPILER_MSVC
#define  COMPILER_MSVC
#include <vector>
#include "afxwin.h"
// CmediamanageDlg 对话框


struct Media
{
	CString Name;
	UINT Part;
	CString update_remind_flag;
};


class CmediamanageDlg : public CDialog
{
	// 构造
public:
	CmediamanageDlg(CWnd* pParent = NULL);	// 标准构造函数

	// 对话框数据
	enum { IDD = IDD_MEDIAMANAGE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	UINT choose;
	bool music_flag;
	bool startup_flag;
	CString update_list;
	std::vector<Media> example;
	std::vector<Media>::iterator it;
	CStatic m_image;

	void flush( );
	void Save( std::vector<Media>& );
	void Load( std::vector<Media>& );
	void check_update( );
	void read_config( );
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnLbnSelchangeList4();
	afx_msg LRESULT onShowTask(WPARAM wParam,LPARAM lParam);
	//bool operator < (const Media &m)const {
	//	return media.update_remind_flag < m.update_remind_flag;
	//}

	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedButton5();
};


#endif 
#endif 
#endif 