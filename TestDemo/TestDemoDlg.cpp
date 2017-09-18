// TestDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDemo.h"
#include "TestDemoDlg.h"
#include "SearchFile.h"
#include "DeleteFile.h"
#include "CopyFile.h"
#include <Windows.h>
#include <iostream>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString strCopySource = _T("");
CString strCopyFileName = _T("");

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
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
public:
	//afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// CTestDemoDlg 对话框


CTestDemoDlg::CTestDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	dlgInitital = new CDialog();
	dlgInitital = this;
	m_dlgNewFile = NULL;
	m_dlgNewFolder = NULL;
	m_dlgMoveFile = NULL;
	m_dlgRenameFile = NULL;
	m_dlgProcessManager = NULL;
}
CTestDemoDlg::~CTestDemoDlg()
{
	delete m_dlgNewFile;
	delete m_dlgNewFolder;
	delete m_dlgMoveFile;
	delete m_dlgRenameFile;
	delete m_dlgProcessManager;
}
void CTestDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestDemoDlg, CDialog)    //手动添加映射
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_GOTO, OnbtnGoto)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnbtnSearch)//ID值是按照用调用AddControl()的顺序赋予的
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MSG, &CTestDemoDlg::OnMsg)
	ON_COMMAND(IDM_EDIT_COPY, &CTestDemoDlg::OnEditCopy)
	//ON_COMMAND(IDM_EDIT_CUT, &CTestDemoDlg::OnEditCut)
	ON_COMMAND(IDM_EDIT_PATSE, &CTestDemoDlg::OnEditPatse)
	ON_COMMAND(IDM_EDIT_DELETE, &CTestDemoDlg::OnEditDelete)
	ON_COMMAND(IDM_HELP_ABOUT, &CTestDemoDlg::OnHelpAbout)
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(IDM_EDIT_RENAME, &CTestDemoDlg::OnEditRename)
	
	ON_COMMAND(IDM_EDIT_MOVE, &CTestDemoDlg::OnEditMove)
	ON_COMMAND(IDM_EDIE_NEW_FILE, &CTestDemoDlg::OnEditNew)
	ON_COMMAND(IDM_EDIT_NEW_FOLDER, &CTestDemoDlg::OnEditNewFolder)
	ON_COMMAND(IDM_PROCESS_MANAGER, &CTestDemoDlg::OnProcessManager)
//	ON_COMMAND(IDM_EDIT_CUT, &CTestDemoDlg::OnEditCut)
ON_COMMAND(IDM_HELP_NOTE, &CTestDemoDlg::OnHelpNote)
END_MESSAGE_MAP()


// CTestDemoDlg 消息处理程序

BOOL CTestDemoDlg::OnInitDialog()            //初始话函数
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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

	
	// TODO: 在此添加额外的初始化代码

	//窗口默认大小
	this->ShowWindow(SW_SHOWNORMAL);
	this->SetWindowTextW(_T("Windows 资源管理器"));
	


	//给主窗口加载菜单
	m_rscMainMenu.LoadMenu(IDR_MENU_MAINFRAME);
	SetMenu(&m_rscMainMenu);

	//get the screen's pixel 获得设备屏幕大小
    int screenX = GetSystemMetrics(SM_CXSCREEN);   
    int screenY = GetSystemMetrics(SM_CYSCREEN);
	//获得对话框大小
	this->GetWindowRect(&m_rectClient);
	
	ScreenToClient(&m_rectClient);
	m_iClientWidth = m_rectClient.Width();
    m_iClientHeight = m_rectClient.Height();
	//设置窗口位置以及大小
	this->SetWindowPos(&wndNoTopMost, (screenX- m_iClientWidth)/2, (screenY- m_iClientHeight)/2, 1300, 800, SWP_SHOWWINDOW);
	
	//下面是将控件添加到TESTDEMO对话框上的代码，并加入CList链表中
    //CRect()的前两个参数是控件左上角的坐标，后面表示右下角坐标
	
	//按钮控件
	stAddress = new CStatic();
	AddControl(stAddress, _T("STATIC"), _T("地址"), WS_VISIBLE | WS_CHILD, CRect(10, 10, 10 + 40, 10 + 30));
	m_edtAddress = new CEdit();
	AddControl(m_edtAddress, _T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER, CRect(60, 10, 60 + 640, 10 + 30));
	m_edtAddress->SetFocus();
	m_btnGoto = new CButton();
	AddControl(m_btnGoto, _T("BUTTON"), _T("转到"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_ICON, CRect(710, 10, 710 + 40, 10 + 30));
	m_stSearch = new CStatic();
	AddControl(m_stSearch, _T("STATIC"), _T("搜索"), WS_VISIBLE | WS_CHILD, CRect(750 + 10, 10, 750 + 10 + 40, 10 + 30));
	m_edtSearch = new CEdit();
	AddControl(m_edtSearch, _T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER, CRect(810, 10, 810 + 410, 10 + 30));
	m_btnSearch = new CButton();
	AddControl(m_btnSearch, _T("BUTTON"), _T("查找"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_ICON, CRect(1230, 10, 1230 + 40, 10 + 30));
	//目录树控件
	//m_treLeft = new CTreeCtrl();
	//m_treLeft = new CMFCShellTreeCtrl;
	m_treLeft = new MyTree();
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS|TVS_SHOWSELALWAYS;
	AddControl(m_treLeft,WC_TREEVIEW,_T(""),dwViewStyle,CRect(10,50,10+330,50+660));

	//视图列表控件
	//m_lstRight = new CListCtrl();
	//m_lstRight = new CMFCShellListCtrl();
	m_lstRight = new MyList();
	AddControl(m_lstRight, WC_LISTVIEW, _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_SINGLESEL | ILC_MASK, CRect(350, 50, 350 + 920, 50 + 660));
	m_lstRight->SetExtendedStyle(m_lstRight->GetExtendedStyle()|LVS_EX_FULLROWSELECT );

	/*CStatusBarCtrl * barStatus = new CStatusBarCtrl();;
	AddControl(barStatus,STATUSCLASSNAME,_T(""),WS_CHILD | WS_VISIBLE | WS_BORDER|SBARS_SIZEGRIP| SBT_TOOLTIPS, CRect(0,rectClient.bottom-20,rectClient.Width(),rectClient.Height())) ;	
	CRect rect;
	barStatus->GetClientRect(&rect);
    int a[4]= {rect.right-600, rect.right-500, rect.right-300, -1 };
	barStatus->SetParts(4,a);*/

	/*barStatus->SetText(L"Arrcen",1,SBT_NOBORDERS );
    barStatus->SetText(L"MFC",2,SBT_POPOUT);
    barStatus->SetText(L"Show",3,SBT_NOBORDERS);*/

	stAddress->SetFont(GetFont());
	m_edtAddress->SetFont(GetFont());
	m_btnGoto->SetFont(GetFont());
	m_stSearch->SetFont(GetFont());
	m_edtSearch->SetFont(GetFont());
	m_btnSearch->SetFont(GetFont());

	//目录树控件初始化
	m_treLeft->Initial(m_lstRight,m_edtAddress);
	m_treLeft->Expand(m_treLeft->GetRootItem(), TVE_EXPAND);
	//视图列表控件初始化
	m_lstRight->Initial(m_edtAddress);

	//设置文件路径编辑框的输入焦点
	m_edtAddress->SetFocus();
	return FALSE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestDemoDlg::OnPaint()
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
HCURSOR CTestDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTestDemoDlg::OnSize( UINT nType, int cx, int cy )
{
	CDialog::OnSize(nType,cx,cy);
	POSITION   pos = m_ControlList.GetHeadPosition();
	this->GetWindowRect(&m_rectClient);
	ScreenToClient(&m_rectClient);
	CRect tempRect;
	if (nType == SIZE_MAXIMIZED)
	{
		//设置地址栏编辑框的位置
		m_edtAddress->MoveWindow(60, 10, (int)(m_rectClient.Width()*0.50), 30);

		//获得地址栏编辑框的位置并设置“转到”按钮的位置
		m_edtAddress->GetWindowRect(&tempRect);
		ScreenToClient(tempRect);
		m_btnGoto->MoveWindow(tempRect.right + 10, tempRect.top, 40, tempRect.Height());

		//获得“转到”按钮的位置并设置“搜索”静态文本框的位置
		m_btnGoto->GetWindowRect(&tempRect);
		ScreenToClient(tempRect);
		m_stSearch->MoveWindow(tempRect.right +10, tempRect.top, 40, tempRect.Height());

		//获得“搜索”静态文本框的位置并设置“搜索”编辑框的位置
		m_stSearch->GetWindowRect(&tempRect);
		ScreenToClient(tempRect);
		m_edtSearch->MoveWindow(tempRect.right + 10, tempRect.top, (int)(m_rectClient.Width()*0.36), tempRect.Height());

		//获得“搜索”编辑框的位置并设置“搜索”按钮的位置
		m_edtSearch->GetWindowRect(&tempRect);
		ScreenToClient(tempRect);
		m_btnSearch->MoveWindow(tempRect.right + 10, tempRect.top, 40, tempRect.Height());

		//设置树形目录控件的位置
		m_treLeft->MoveWindow(10, 50, (int)(m_rectClient.Width()*0.20), (int)(m_rectClient.Height()*0.85));
		
		//获得树形目录控件的位置并设置视图列表控件的位置
		m_treLeft->GetWindowRect(&tempRect);
		ScreenToClient(tempRect);
		CRect tempRect1;
		m_btnSearch->GetWindowRect(&tempRect1);
		ScreenToClient(tempRect1);
		//m_lstRight->MoveWindow(tempRect.right + 10, tempRect.top, (int)(m_rectClient.Width()*0.76), tempRect.Height());
		m_lstRight->MoveWindow(tempRect.right + 10, tempRect.top, tempRect1.right - (tempRect.right + 10), tempRect.Height());
		return;
	
	}
	//最大化还原后控件的位置
	if(pos != NULL)
	{
		m_edtAddress->MoveWindow(60, 10, 640, 30);
		m_btnGoto->MoveWindow(710, 10, 40, 30);
		m_stSearch->MoveWindow(760, 10, 40, 30);
		m_edtSearch->MoveWindow(810, 10, 410, 30);
		m_btnSearch->MoveWindow(1230, 10, 40, 30);
		m_treLeft->MoveWindow(10, 50, 330, 660);
		m_lstRight->MoveWindow(350, 50, 920, 660);
	}	
}


//销毁窗口，释放资源
BOOL CTestDemoDlg::DestroyWindow()
{
	//释放控件链表的资源
	while(!m_ControlList.IsEmpty())
	{
		CWnd* pWndCtrl = m_ControlList.RemoveTail();
		pWndCtrl->DestroyWindow();
		delete pWndCtrl;
		pWndCtrl = NULL;
	}
	return CDialog::DestroyWindow();
}

//添加控件到对话框的函数
void CTestDemoDlg::AddControl(CWnd* control,LPCTSTR lpszClassName,LPCTSTR lpszText, DWORD dwStyle, const RECT& rect)
{
	m_ControlList.AddTail(control);
	BOOL ret = control->Create(lpszClassName,lpszText,dwStyle,rect, dlgInitital,1001+m_ControlList.GetCount());//dlgInitital即为当前对话框
}

afx_msg LRESULT CTestDemoDlg::OnMsg(WPARAM wParam, LPARAM lParam)    //NewFile框关闭时发出的消息的处理函数
{
	OnbtnGoto();
	return 0;
}

void CTestDemoDlg::OnbtnGoto()   //相当于根据文本框中的路径，刷新右侧列表
{
	CString strAddress;
	m_edtAddress->GetWindowText(strAddress);  //获取文本框的内容
	
	CString strInstead;
	strInstead = "D:\\";     // 转到"D:\"再转回来，相当于实现刷新功能
	if (strAddress == strInstead)
	{
		strInstead = "C:\\";
	}
	//按路径广度遍历树查找
	HTREEITEM item1 = m_treLeft->FindItem(m_treLeft->GetRootItem(),strInstead);
	HTREEITEM item2 = m_treLeft->FindItem(m_treLeft->GetRootItem(), strAddress);
	if (item2 != NULL)
	{
		m_treLeft->SelectItem(item1);
		m_treLeft->SetFocus();
		m_treLeft->SelectItem(item2);
		m_treLeft->SetFocus();
	}
	else
		return;
	
}
	
void CTestDemoDlg::OnbtnSearch()
{
	CString strDirPath;			//搜索的文件路径
	CString strSearchFileName; //搜索的文件名
	CSearchFile SF;
	m_edtAddress->GetWindowText(strDirPath);		//从编辑框获得文件路径
	m_edtSearch->GetWindowText(strSearchFileName); //从编辑款获得文件名
	SF.SetReleatedListCtrl(m_lstRight);
	switch (SF.StartSearchFile(strDirPath, strSearchFileName))
	{
	case 1:
		break;
	case -1:
		MessageBox(_T("搜索路径为空"), NULL, MB_ICONWARNING);
		break;
	case -2:
		MessageBox(_T("搜索文件名为空"), NULL, MB_ICONWARNING);
		break;
	}

}

//新建文件
void CTestDemoDlg::OnEditNew()
{
	// TODO: 在此添加命令处理程序代码
	CString strAddress;
	m_edtAddress->GetWindowText(strAddress);  //获取当前路径
	if (strAddress.IsEmpty())                   //路径不能为空
	{
		MessageBox(_T("新建失败！路径为空"), NULL, MB_ICONERROR);
		return;
	}
	if (m_dlgNewFile == NULL)
	{
		m_dlgNewFile = new NewFileDlg();
		m_dlgNewFile->Create(IDD_NEWFILE_DIALOG, this);
	}
	//打开新建文件的对话框
	m_dlgNewFile->Initital(strAddress);
	m_dlgNewFile->ShowWindow(SW_SHOW);	                     //显示此对话框,非模态
}

//新建文件夹
void CTestDemoDlg::OnEditNewFolder()
{
	// TODO: 在此添加命令处理程序代码
	CString strAddress;
	m_edtAddress->GetWindowText(strAddress);	//获取当前路径
	if (strAddress.IsEmpty())					 //路径不能为空
	{
		MessageBox(_T("新建失败！路径为空"), NULL, MB_ICONERROR);
		return;
	}
	if (m_dlgNewFolder == NULL)
	{
		m_dlgNewFolder = new CNewFolderDlg();
		m_dlgNewFolder->Create(IDD_NEWFOLDER_DIALOG, this);		//打开新建文件的对话框
	}
	m_dlgNewFolder->Initial(strAddress);
	m_dlgNewFolder->ShowWindow(SW_SHOW);						//显示此对话框
}

void CTestDemoDlg::OnEditCopy()			//菜单栏的“复制”
{
	CString strAddress;
	switch (m_lstRight->GetSelectedItemPath(strAddress))
	{
	case 1:
		strCopySource = strAddress;
		m_lstRight->GetSelectedItemFileName(strCopyFileName);
		MessageBox(_T("文件复制成功"));
		break;
	case 2:
		strCopySource = strAddress;
		m_lstRight->GetSelectedItemFileName(strCopyFileName);
		MessageBox(_T("文件夹复制成功"));
		break;
	//没有选择项
	case -1:
		MessageBox(_T("当前没有选择项"), NULL, MB_ICONWARNING);
		break;
	//没有选择路径
	case -2:
		MessageBox(_T("当前路径为空"), NULL, MB_ICONWARNING);
		break;
	}
}

void CTestDemoDlg::OnEditPatse()		//菜单栏的“粘贴”
{
	// TODO: 在此添加命令处理程序代码
	CString strAddress;
	m_edtAddress->GetWindowText(strAddress);
	if (strAddress.IsEmpty())
	{
		MessageBox(_T("当前路径为空"), NULL, MB_ICONWARNING);
		return;
	}
	CCopyFile CF;
	CString strPasteDesti = strAddress + "\\" + strCopyFileName;   //要粘贴到的目的地址
	if (strCopySource.IsEmpty()) return;
	switch (CF.Copy(strCopySource, strPasteDesti))
	{
	case 1:
		OnbtnGoto();
		MessageBox(_T("文件夹粘贴成功!"));
		break;
	case 2:
		OnbtnGoto();
		MessageBox(_T("文件粘贴成功!"));
		break;
	case 0:
		MessageBox(_T("文件粘贴失败!"), NULL, MB_ICONERROR);
		break;
	}
}


void CTestDemoDlg::OnEditDelete()		//菜单栏的“删除”
{
	// TODO: 在此添加命令处理程序代码
	CString strAddress;
	CDeleteFile DF;
	switch (m_lstRight->GetSelectedItemPath(strAddress))
	{
	case -1:
		MessageBox(_T("当前没有选择项"), NULL, MB_ICONWARNING);
		return;
	//没有选择路径
	case -2:
		MessageBox(_T("当前路径为空"), NULL, MB_ICONWARNING);
		return;
	}
	switch (DF.Delete(strAddress))
	{
	case 1:
		OnbtnGoto();
		MessageBox(_T("文件夹删除成功"));
		break;
	case 2:
		OnbtnGoto();
		MessageBox(_T("文件删除成功"));
		break;
	case 0:
		MessageBox(_T("删除失败"));
		break;
	}
}

void CTestDemoDlg::OnEditMove()
{
	// TODO: 在此添加命令处理程序代码
	// TODO: 在此添加命令处理程序代码
	CString strAddress;
	CString strFileName;
	if (m_dlgMoveFile == NULL)
	{
		m_dlgMoveFile = new CMoveFileDlg();
		m_dlgMoveFile->Create(IDD_MOVEFILE_DIALOG, this);
	}
	
	switch (m_lstRight->GetSelectedItemPath(strAddress))
	{
	case 1:
		m_lstRight->GetSelectedItemFileName(strFileName);
		m_dlgMoveFile->Ininatial(strAddress, strFileName);
		m_dlgMoveFile->ShowWindow(SW_SHOW);
		break;
		//没有选择项
	case -1:
		MessageBox(_T("当前没有选择项"), NULL, MB_ICONWARNING);
		break;
		//没有选择路径
	case -2:
		MessageBox(_T("当前路径为空"), NULL, MB_ICONWARNING);
		break;
	}
}

void CTestDemoDlg::OnEditRename()		//菜单栏的“重命名”
{
	// TODO: 在此添加命令处理程序代码
	CString strAddress;
	CString strFileName;
	if (m_dlgRenameFile == NULL)
	{
		m_dlgRenameFile = new CRenameFileDlg();
		m_dlgRenameFile->Create(IDD_RENAMEFILE_DIALOG, this);
	}

	switch (m_lstRight->GetSelectedItemPath(strAddress))
	{
	case 1:
		m_edtAddress->GetWindowText(strAddress);
		m_lstRight->GetSelectedItemFileName(strFileName);
		m_dlgRenameFile->Initital(strFileName, strAddress);
		m_dlgRenameFile->ShowWindow(SW_SHOW);
		break;
		//没有选择项
	case -1:
		MessageBox(_T("当前没有选择项"), NULL, MB_ICONWARNING);
		break;
		//没有选择路径
	case -2:
		MessageBox(_T("当前路径为空"), NULL, MB_ICONWARNING);
		break;
	}
}

void CTestDemoDlg::OnHelpAbout()		//菜单栏的“帮助”->“关于”
{
	// TODO: 在此添加命令处理程序代码
	CAboutDlg MenuAboutDlg;
	MenuAboutDlg.DoModal();
}



BOOL CTestDemoDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		{
			m_edtAddress->SetFocus();
			m_edtAddress->SetSel(0, -1);
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CTestDemoDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (lpMMI->ptMinTrackSize.x <= 800)
		lpMMI->ptMinTrackSize.x = 800;
	if (lpMMI->ptMinTrackSize.y <= 500)
		lpMMI->ptMinTrackSize.y = 500;
	CDialog::OnGetMinMaxInfo(lpMMI);
}


void CTestDemoDlg::OnProcessManager()
{
	// TODO: 在此添加命令处理程序代码
	if (m_dlgProcessManager == NULL)
	{
		m_dlgProcessManager = new CProcessManager();
		m_dlgProcessManager->Create(IDD_PROCESSMANAGER_DIALOG, this);
	}
	m_dlgProcessManager->GetProcessList();
	m_dlgProcessManager->ShowWindow(SW_SHOW);

}


void CTestDemoDlg::OnHelpNote()
{
	// TODO: 在此添加命令处理程序代码
	ShellExecute(NULL, _T("open"), _T("C:\\操作系统项目实验报告.docx"), 0, 0, SW_SHOWNORMAL);
}
