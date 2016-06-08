#pragma once


// Delete_Dialog 对话框

class Delete_Dialog : public CDialog
{
	DECLARE_DYNAMIC(Delete_Dialog)

public:
	Delete_Dialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Delete_Dialog();

// 对话框数据
	 enum { IDD = IDD_DIALOG0 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	bool delete_flag;
	afx_msg void OnBnClickedOk();
};
