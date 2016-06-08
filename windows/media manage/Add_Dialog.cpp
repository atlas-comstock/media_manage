// Add_Dialog.cpp : 实现文件
//

#include "stdafx.h"
#include "media_manage.h"
#include "Add_Dialog.h"

Add_Dialog *add_dialog= new Add_Dialog;
// Add_Dialog 对话框

IMPLEMENT_DYNAMIC(Add_Dialog, CDialog)



BOOL Add_Dialog::OnInitDialog()
{
	( (CComboBox*)GetDlgItem(IDC_COMBO1) )->AddString("x") ;//这样做比固定一个控件变量好
	( (CComboBox*)GetDlgItem(IDC_COMBO1) )->AddString("√") ;
	//( (CComboBox*)GetDlgItem(IDC_COMBO1) )->AddString("星期一") ;
	//( (CComboBox*)GetDlgItem(IDC_COMBO1) )->AddString("星期二") ;
	//( (CComboBox*)GetDlgItem(IDC_COMBO1) )->AddString("星期三") ;
	//( (CComboBox*)GetDlgItem(IDC_COMBO1) )->AddString("星期四") ;
	//( (CComboBox*)GetDlgItem(IDC_COMBO1) )->AddString("星期五") ;
	//( (CComboBox*)GetDlgItem(IDC_COMBO1) )->AddString("星期六") ;
	( (CComboBox*)GetDlgItem(IDC_COMBO1) )->SetCurSel( 0 );
	return TRUE;
}
Add_Dialog::Add_Dialog(CWnd* pParent /*=NULL*/)
: CDialog(Add_Dialog::IDD, pParent)
, name(_T(""))
, number(_T(""))
, flag(false)
{

}

Add_Dialog::~Add_Dialog()
{
}

void Add_Dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT1, name);//与控件关联变量 非常重要，在两个dialog直接相互分享数据
	DDX_Text(pDX, IDC_EDIT2, number);
}


BEGIN_MESSAGE_MAP(Add_Dialog, CDialog)
	ON_BN_CLICKED(IDOK, &Add_Dialog::OnBnClickedOk)
END_MESSAGE_MAP()


// Add_Dialog 消息处理程序

void Add_Dialog::OnBnClickedOk()
{
	CEdit* pEdit; 
	pEdit = (CEdit*) GetDlgItem( IDC_EDIT1 ); 
	pEdit-> GetWindowText( add_dialog->name );
	if( add_dialog->name == "" )
		add_dialog->name = "未命名";
	pEdit = (CEdit*) GetDlgItem( IDC_EDIT2 );
	pEdit-> GetWindowText( add_dialog->number );

	( (CComboBox*)GetDlgItem(IDC_COMBO1) )->GetWindowText(update_remind_flag);


	//do 
	//{
	//	pEdit-> GetWindowText( add_dialog->number );
	//	if ( atoi(add_dialog->number)<0 )      鸡肋
	//		MessageBox( "请输入大于0的数字" );
	//} while ( atoi( add_dialog->number )<0 );
	flag = 1;
	AnimateWindow(100,AW_HIDE|AW_BLEND);      //可以缓慢消失窗口 

	OnOK();
}

