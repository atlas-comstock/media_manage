#pragma once
#include "afxwin.h"


// Add_Dialog 对话框

class Add_Dialog : public CDialog 
{
	DECLARE_DYNAMIC(Add_Dialog)

public:
	Add_Dialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Add_Dialog();

	// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString name;
	CString number;
	CString update_remind_flag;
	BOOL OnInitDialog();
	/*INT_PTR DoModal();*/
	//CDialog *m_pMianDlg;  

	//void set_parent_dlg(CDialog* parnet_dlg)
	//{
	//	m_pMianDlg=parnet_dlg;  
	//} 

	afx_msg void OnBnClickedOk();
	bool flag;
};