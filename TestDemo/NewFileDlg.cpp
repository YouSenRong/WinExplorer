// NewFileDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDemo.h"
#include "NewFileDlg.h"
#include "TestDemoDlg.h"
#include "afxdialogex.h"


// NewFileDlg 对话框

IMPLEMENT_DYNAMIC(NewFileDlg, CDialogEx)

NewFileDlg::NewFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NEWFILE_DIALOG, pParent)
{
	::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	
}

NewFileDlg::NewFileDlg(CString strAddress, CWnd* pParent /*= NULL*/)
	: CDialogEx(IDD_NEWFILE_DIALOG, pParent)
{
	this->m_strAddressPath = strAddress;
	::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

NewFileDlg::~NewFileDlg()
{
}

void NewFileDlg::Initital()                    //初始化函数
{
	m_Edit_NewFile_Path.SetWindowTextW(m_strAddressPath);
	m_Edit_NewFile_FileName.SetFocus();
}

void NewFileDlg::Initital(CString strAddress)                    //初始化函数
{
	this->m_strAddressPath = strAddress;
	m_Edit_NewFile_Path.SetWindowTextW(m_strAddressPath);
	m_Edit_NewFile_FileName.SetFocus();
}

void NewFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NEWFILE_PATH, m_Edit_NewFile_Path);
	DDX_Control(pDX, IDC_EDIT_NEWFILE_FILENAME, m_Edit_NewFile_FileName);
}


BEGIN_MESSAGE_MAP(NewFileDlg, CDialogEx)                      //添加映射
	ON_BN_CLICKED(IDC_BTN_NEWFILE_NEW, &NewFileDlg::OnBnClickedNewFileNew)
	ON_BN_CLICKED(IDC_BTN_NEWFILE_CANCLE, &NewFileDlg::OnBnClickedNewFileCancle)
	ON_EN_CHANGE(IDC_EDIT_NEWFILE_PATH, &NewFileDlg::OnEnChangeEditNewfilePath)
END_MESSAGE_MAP()


// NewFileDlg 消息处理程序


void NewFileDlg::OnBnClickedNewFileNew()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Edit_NewFile_Path.GetWindowTextW(m_strAddressPath);  //获取文本框的内容
	m_Edit_NewFile_FileName.GetWindowTextW(m_strFileName);
	if (m_strFileName.IsEmpty())
	{
		MessageBox(_T("新建失败，请输入文件名！"), NULL, MB_ICONERROR);
		return;
	}
	m_strAddressPath = m_strAddressPath + "\\" + m_strFileName;  //完整的地址（包含路径和文件名）
	CreateFile(m_strAddressPath,             //用CreatFile()新建文件
		GENERIC_ALL,
		FILE_SHARE_READ,
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	GetParent()->SendMessage(WM_MSG, 0, 0);     //完成新建后发送消息给TESTDEMO对话框
	SendMessage(WM_CLOSE);
			      	        
}

//按下enter
void NewFileDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnBnClickedNewFileNew();
	CDialogEx::OnOK();
}

//"取消"按钮的响应
void NewFileDlg::OnBnClickedNewFileCancle()
{
	SendMessage(WM_CLOSE);                
	// TODO: 在此添加控件通知处理程序代码
}


void NewFileDlg::OnEnChangeEditNewfilePath()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}