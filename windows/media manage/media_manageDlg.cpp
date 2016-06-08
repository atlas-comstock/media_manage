// media manageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "media_manage.h"
#include "media_manageDlg.h"
#include "Add_Dialog.h"
#include "Delete_Dialog.h"
#include <algorithm>
#include <stdlib.h>
#include <afxinet.h>
#pragma comment(lib,"Winmm.lib")// 播放用

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CmediamanageDlg 对话框


NOTIFYICONDATA nid;
CWinThread *check_url;

UINT _check_url(LPVOID lparam)
{
	CmediamanageDlg *pDlg = (CmediamanageDlg *)lparam;
	while(1)
	{
		pDlg->check_update();
		Sleep(1800000);//1800000
	}
	return 0;
}

CmediamanageDlg::CmediamanageDlg(CWnd* pParent /*=NULL*/)
: CDialog(CmediamanageDlg::IDD, pParent)
, choose(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmediamanageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_LBIndex(pDX, IDC_LIST4, (int&)choose);
	DDX_Control(pDX, IDC_BMP, m_image);
}

BEGIN_MESSAGE_MAP(CmediamanageDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	//}}AFX_MSG_MAP
	ON_LBN_SELCHANGE(IDC_LIST4, &CmediamanageDlg::OnLbnSelchangeList4)
	ON_BN_CLICKED(IDC_BUTTON1, &CmediamanageDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CmediamanageDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CmediamanageDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CmediamanageDlg::OnBnClickedButton3)
	ON_MESSAGE(WM_SHOWTASK, onShowTask )

	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK1, &CmediamanageDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CmediamanageDlg::OnBnClickedCheck2)
END_MESSAGE_MAP()


// CmediamanageDlg 消息处理程序

BOOL CmediamanageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标


	m_image.ModifyStyle( 0, SS_BITMAP | SS_CENTERIMAGE );//将静态控件设置为是用来显示位图的，只不过这里是通过该静态对话框的变量来进行ModifyStyle操作的
	HBITMAP hBmp = (HBITMAP)::LoadImage( 0, "image.bmp", IMAGE_BITMAP, 188, 188, LR_LOADFROMFILE );//从bmp文件所在的路径直接读取该BMP图像的句柄，并将其转换为HBITMAP类型
	m_image.SetBitmap( hBmp );//通过变量m_image来设置该静态框所显示的位图文件

	read_config( );
	Load(example);
	flush();
	AnimateWindow(400,AW_ACTIVATE|AW_VER_POSITIVE); 

	nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA); //6.0.6 or higher (Windows Vista and later)//nid.cbSize=NOTIFYICONDATA_V3_SIZE;//xp

	nid.hWnd=this->m_hWnd;
	nid.uID=IDR_MAINFRAME;
	nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP|NIF_INFO;
	nid.uCallbackMessage=WM_SHOWTASK;//自定义的消息名称,注意:这里的消息是用户自定义消息
	nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));

	//strcpy(nid.szInfo, "什么鸡巴系统。。。" );//气泡标题
	wcscpy_s((wchar_t*)nid.szTip,64,(wchar_t*)_T("视频管理")); 
	wcscpy_s( (wchar_t*)nid.szInfoTitle, 64, (wchar_t*)_T("视频更新提醒") );
	wcscpy_s((wchar_t*)nid.szInfo, 255,  (LPCWSTR)(LPCTSTR)update_list);
	//wcscpy_s(nid.szInfoTitle,_T("内容"));
	nid.dwInfoFlags=NIIF_INFO;
	nid.uTimeout=99999;
	Shell_NotifyIcon(NIM_ADD,&nid);//在托盘区添加图标
	check_url = ::AfxBeginThread(_check_url, this);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CmediamanageDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CmediamanageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CmediamanageDlg::OnLbnSelchangeList4()//单击列表时获取选择的内容
{	
	choose = ( (CListBox*)GetDlgItem(IDC_LIST4) )->GetCurSel();
}

void CmediamanageDlg::Save( std::vector<Media>&m )
{
	CStdioFile file( _T("file.txt"), CFile::modeCreate | CFile::modeWrite );//文件不存在就创建文件，若存在则清空文件
	for ( it=m.begin(); it!=m.end(); ++it )
	{ 
		file.WriteString( it->Name + "\n" );
		CString Temp; 
		Temp.Format("%s%d\n", Temp, it->Part);
		file.WriteString( Temp );
		file.WriteString( it->update_remind_flag + "\n" );
	}
	file.Close();
}

void CmediamanageDlg::Load(std::vector<Media> &m)
{
	CStdioFile file( _T("file.txt"), CFile::modeRead );
	CString Temp;
	Media newone;
	while( file.ReadString(newone.Name) )
	{
		file.ReadString( Temp );
		newone.Part=atoi( Temp );
		file.ReadString( newone.update_remind_flag );
		m.push_back( newone );
	}
	file.Close();
}
void CmediamanageDlg::OnBnClickedButton1()//集数+1
{
	if( example.size()==0 )
		MessageBox("没有视频");
	else if ( choose>=example.size() )
		MessageBox("没有选中视频");
	else
	{
		it = example.begin() + choose;
		++(it->Part);
		CString Temp; 
		Temp.Format(_T("%s\t%d\t"), Temp, it->Part);
		CString Output =  it->Name + Temp + it->update_remind_flag;
		( (CListBox*)GetDlgItem(IDC_LIST4) )->DeleteString(choose);
		( (CListBox*)GetDlgItem(IDC_LIST4) )->InsertString(choose, _T(Output) );
		Save(example);
	}
}

void CmediamanageDlg::OnBnClickedButton2()//集数--1
{
	if( example.size()==0 )
		MessageBox("没有视频");
	else if ( choose>=example.size() )
		MessageBox("没有选中视频");
	else
	{
		it = example.begin() + choose;
		if ( it->Part==0 )
			MessageBox("集数已经为0");
		else
		{
			--(it->Part);
			CString Temp; 
			Temp.Format(_T("%s\t%d\t"), Temp, it->Part);
			CString Output = it->Name + Temp + it->update_remind_flag;	
			( (CListBox*)GetDlgItem(IDC_LIST4) )->DeleteString(choose);
			( (CListBox*)GetDlgItem(IDC_LIST4) )->InsertString(choose, _T(Output) );
			Save(example);
		}
	}
}

//if( example.size()==0 )//全部+1
//	MessageBox("没有视频");
//else
//{
//	int number=0;
//	for ( it=example.begin(); it!=example.end(); ++it,++number )
//	{
//		++(it->Part);
//		CString Temp; 
//		Temp.Format(_T("%s\t%d"), Temp ,it->Part);
//		CString Output= it->Name  +  Temp ;
//		( (CListBox*)GetDlgItem(IDC_LIST4) )->DeleteString(number);
//		( (CListBox*)GetDlgItem(IDC_LIST4) )->InsertString(number,_T(Output) );
//	}
//	CmediamanageDlg::Save(example);
//}

void CmediamanageDlg::OnBnClickedButton3()//添加视频
{

	Add_Dialog *add_dialog =new Add_Dialog;
	add_dialog->DoModal();
	if ( add_dialog->flag == true )
	{
		int number=0;
		for ( it=example.begin(); it!=example.end(); ++it,	++number);
		//for ( ; number>=0; --number )
		//	( (CListBox*)GetDlgItem(IDC_LIST4) )->DeleteString(number);

		Media newone;
		newone.Name = add_dialog->name; 
		newone.Part = atoi(add_dialog->number);
		newone.update_remind_flag = add_dialog->update_remind_flag; 
		example.push_back(newone);

		CmediamanageDlg::Save(example);
		CString Temp; 
		Temp.Format(_T("%s\t%d\t"), Temp, newone.Part);
		CString Output = newone.Name + Temp + newone.update_remind_flag;	
		( (CListBox*)GetDlgItem(IDC_LIST4) )->InsertString(number, _T(Output) );
	}
	//	add_dialog ->set_parent_dlg(this);  
}


void CmediamanageDlg::OnBnClickedButton4()//删除视频
{	
	if( example.size()==0 )
		MessageBox("没有视频");
	else if ( choose>=example.size() )
		MessageBox("没有选中视频");
	else
	{
		Delete_Dialog *delete_dialog =new Delete_Dialog;
		delete_dialog->DoModal();
		if ( delete_dialog->delete_flag == true )
		{
			it = example.begin() + choose;
			example.erase(it);
			( (CListBox*)GetDlgItem(IDC_LIST4) )->DeleteString(choose);//删除显示的东西
			CmediamanageDlg::Save(example);
		}		
	}
}

void CmediamanageDlg::flush()//刷新显示
{
	//sort( example.begin(), example.end() );
	for( it = example.begin(); it != example.end(); ++it )
	{
		CString Output;
		Output.Format("%d",(it->Name).GetLength());
		if ((it->Name).GetLength()>=10)
			Output.Format(_T("%s\t%d\t%s"), it->Name  ,it->Part, it->update_remind_flag );
		else
			Output.Format(_T("%s\t\t%d\t%s"), it->Name  ,it->Part, it->update_remind_flag );
		( (CListBox*)GetDlgItem(IDC_LIST4) )->AddString( LPCTSTR(Output) ) ;
	}
}
//
//void CmediamanageDlg::check_update( )
//{
//	CTime time = CTime::GetCurrentTime(); //获取系统日期
//	int today_date = time.GetDayOfWeek(); //获取星期几，注意1为星期天，7为星期六
//	//	std::vector <CString> weeks( "星期天", "星期一"  );
//	/*weeks.push_back("不提示");weeks.push_back("星期天");weeks.push_back("星期一");weeks.push_back("星期二");weeks.push_back("星期三");weeks.push_back("星期四");weeks.push_back("星期五");weeks.push_back("星期六");*/
//
//	CString weeks[] = { "不提示", "星期天", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六" };
//	bool flag = 0;
//	for( it = example.begin(); it!= example.end(); ++it )
//	{
//		if ( weeks[today_date] == it->update_remind_flag )
//		{
//			flag = 1;
//			CString Temp;
//			Temp.Format(_T("%s第%d集, "), Temp ,it->Part+1);
//			update_list =  update_list + it->Name + Temp ;
//		}
//	}
//	if ( flag==1 )
//	{
//		update_list += "更新了！";
//		if( true==music_flag )
//		{
//			char program_path[128];
//			GetCurrentDirectory(MAX_PATH,program_path);
//			strcat_s(program_path, "\\music.wav");
//			/*	char buf[128];
//			MCI_OPEN_PARMS mciOpen; 
//			MCIERROR   mciError;   
//			mciOpen.lpstrDeviceType = "mpegvideo";   
//			mciOpen.lpstrElementName = program_path;
//			mciError = mciSendCommand( 0, MCI_OPEN,MCI_OPEN_TYPE   |   MCI_OPEN_ELEMENT,(DWORD)&mciOpen);   
//			if(mciError)   
//			{   
//			mciGetErrorString(mciError,buf,128);   
//			MessageBox("send MCI_PLAY command failed", "ERROR");    
//			return;   
//			}   */
//			PlaySound(program_path,NULL,SND_FILENAME|SND_ASYNC); //异步播放
//			//		PlaySound(program_path,NULL,SND_FILENAME|SND_SYNC);
//
//		}
//	}
//}


LRESULT CmediamanageDlg::onShowTask(WPARAM wParam,LPARAM lParam)//wParam接收的是图标的ID，而lParam接收的是鼠标的行为
{
	if(wParam!=IDR_MAINFRAME)
		return 1;
	switch(lParam)
	{
	case WM_RBUTTONUP:			//右键起来时弹出快捷菜单，这里只有一个"关闭"
		{
			LPPOINT lpoint=new tagPOINT;
			::GetCursorPos(lpoint);			//得到鼠标位置
			CMenu menu;
			menu.CreatePopupMenu();				//声明一个弹出式菜单			
			menu.AppendMenu(MF_STRING,WM_DESTROY,_T("关闭"));   //增加菜单项"关闭"，点击则发送消息WM_DESTROY给主窗口（已隐藏），将程序结束。			
			menu.TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this);//确定弹出式菜单的位置			
			SetForegroundWindow();
			HMENU hmenu=menu.Detach();   //资源回收
			menu.DestroyMenu();
			delete lpoint;
		}break;
	case WM_LBUTTONDBLCLK:    //双击左键的处理
		{
			this->ShowWindow(SW_SHOW);    //简单的显示主窗口完事儿
		}
		break;
	}
	return 0;
}


void CmediamanageDlg::OnClose()
{
	AnimateWindow(500,AW_HIDE|AW_BLEND); 
	this->ShowWindow(SW_HIDE);
}

void CmediamanageDlg::OnDestroy()
{
	CDialog::OnDestroy();
	Shell_NotifyIcon(NIM_DELETE,&nid);//销毁托盘图标
}


void  CmediamanageDlg::read_config( )
{
	CButton* startup_pBtn = (CButton*)GetDlgItem( IDC_CHECK1 );//获取是否开机自动启动 
	CStdioFile file(_T("config.txt"), CFile::modeRead );
	CString Temp; 
	file.ReadString( Temp );
	startup_flag = atoi( Temp );
	startup_pBtn->SetCheck( startup_flag );

	CButton* music_pBtn = (CButton*)GetDlgItem( IDC_CHECK2 );//获取是否播放提示音乐
	file.ReadString( Temp );
	music_flag = atoi( Temp );
	music_pBtn->SetCheck(music_flag);
	file.Close();
}
void CmediamanageDlg::OnBnClickedCheck1()//修改开机自动启动
{

	CStdioFile file( _T("config.txt"), CFile::modeCreate | CFile::modeWrite );//文件不存在就创建文件，若存在则清空文件
	//	startup_flag_Temp.Format("%s%d\n", startup_flag_Temp, startup_flag );
	//	file.WriteString(startup_flag_Temp);

	TCHAR path[MAX_PATH];
	CString keyStr;
	CString fileName;
	HKEY hRegKey;
	GetModuleFileName(NULL,path,sizeof(path));
	CString strPath = path;
	keyStr =_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
	if(RegOpenKey(HKEY_LOCAL_MACHINE,keyStr,&hRegKey) !=  ERROR_SUCCESS)
		return ;
	_wsplitpath((wchar_t*)strPath.GetBuffer(0),NULL,NULL,(wchar_t*)fileName.GetBufferSetLength(MAX_PATH+1),NULL);
	if ( startup_flag )
	{
		file.WriteString(_T("0\n"));//开机不启动
		startup_flag = 0;
		RegDeleteValue ( hRegKey, fileName );   
	}
	else
	{
		file.WriteString(_T("1\n"));//开机启动
		startup_flag = 1;
		if( !::RegSetValueEx(hRegKey, fileName, 0, REG_SZ, (CONST BYTE *)strPath.GetBuffer(0), strPath.GetLength() ) != ERROR_SUCCESS )
			return;
		strPath.ReleaseBuffer();
	}
	RegCloseKey( hRegKey );

	CString music_flag_Temp;
	music_flag_Temp.Format("%s%d\n", music_flag_Temp, music_flag );
	file.WriteString( music_flag_Temp );

	//if ( music_flag )
	//	file.WriteString(_T("1\n"));//开机不启动
	//else
	//	file.WriteString(_T("0\n"));//开机启动

	file.Close();
}

void CmediamanageDlg::OnBnClickedCheck2()// 播放提示音
{
	CStdioFile file( _T("config.txt"), CFile::modeCreate | CFile::modeWrite );//文件不存在就创建文件，若存在则清空文件
	if ( startup_flag )
		file.WriteString(_T("1\n"));
	else
		file.WriteString(_T("0\n")); 

	if ( music_flag )
	{
		file.WriteString(_T("0\n"));//开机不启动
		music_flag = 0;
	}
	else
	{
		file.WriteString(_T("1\n"));//开机启动
		music_flag = 1;
	}

	file.Close();
}

void CmediamanageDlg::check_update()
{
	CInternetSession session; 
	CHttpFile* pfile = (CHttpFile *)session.OpenURL("http://110.75.189.200/chris/calculator/pc_latest.php");   
	DWORD status_code;    
	pfile -> QueryInfoStatusCode(status_code);
	if(status_code == HTTP_STATUS_OK)    
	{      
		CString update_name;
		//	AfxMessageBox("get in");
		if( pfile->ReadString(update_name) )
		{
			bool flag = 0;
			char *pStr = update_name.GetBuffer(update_name.GetLength()); //取得str对象的原始字符串
			int nBufferSize = MultiByteToWideChar(CP_UTF8, 0, pStr, -1, NULL, 0); //取得所需缓存的多少
			wchar_t *pBuffer = (wchar_t*)malloc(nBufferSize * sizeof(wchar_t));//申请缓存空间
			MultiByteToWideChar(CP_UTF8, 0, pStr, -1 , pBuffer, nBufferSize*sizeof(wchar_t));//转码

			char* mbs = new char[ nBufferSize*sizeof(wchar_t) ];
			WideCharToMultiByte( CP_ACP, 0, pBuffer, -1, mbs, nBufferSize*sizeof(wchar_t), NULL, NULL);  
			char *tokens = strtok(mbs,"@");
			update_list = "";
			while(tokens)
			{
				for( it = example.begin(); it!= example.end(); ++it )
				{
					if ( it->Name == tokens && it->update_remind_flag!="不提示" )
					{
						flag = 1;
						if(update_list != "")
							update_list =  update_list + "、" +tokens;
						else
							update_list = tokens;
					}
				}
				tokens = strtok(NULL,"@");
			}
			if ( flag==1 )
			{
				update_list += " 更新了！";
				if( true==music_flag )
				{
					char program_path[128];
					GetCurrentDirectory(MAX_PATH,program_path);
					strcat_s(program_path, "\\music.wav");
					PlaySound(program_path,NULL,SND_FILENAME|SND_ASYNC); //异步播放
				}
				AfxMessageBox(update_list);
			}
			delete[] mbs;
			free(pBuffer); //释放缓存
		}

	}
	pfile -> Close();    
	delete pfile;    
	session.Close();  
}