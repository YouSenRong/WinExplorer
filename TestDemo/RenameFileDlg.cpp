// RenameFileDLg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDemo.h"
#include "RenameFileDlg.h"
#include "afxdialogex.h"

#define WM_MSG WM_USER+0x01


// CRenameFileDLg 对话框

IMPLEMENT_DYNAMIC(CRenameFileDlg, CDialogEx)

CRenameFileDlg::CRenameFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RENAMEFILE_DIALOG, pParent)
	, m_editStrNewFileName(_T(""))
	, m_editStrPreFileName(_T(""))
{
	::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

CRenameFileDlg::~CRenameFileDlg()
{
}

void CRenameFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PREFILENAME, m_editPreFileName);
	DDX_Control(pDX, IDC_EDIT_NEWFILENAME, m_editNewFileName);
	DDX_Text(pDX, IDC_EDIT_NEWFILENAME, m_editStrNewFileName);
	DDX_Text(pDX, IDC_EDIT_PREFILENAME, m_editStrPreFileName);
}


void CRenameFileDlg::Initital(CString strFileName, CString strAddress)
{
	m_editStrPreFileName = strFileName;
	m_editStrNewFileName = strFileName;
	m_strAddress = strAddress;
	UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CRenameFileDlg, CDialogEx)
END_MESSAGE_MAP()


// CRenameFileDLg 消息处理程序


void CRenameFileDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CString strJudge = (CString)"重命名成功";
	CString strNewAddress;
	CString strSourceAddress;
	UpdateData(TRUE);
	strSourceAddress = m_strAddress + m_editStrPreFileName; //文件源地址
	strNewAddress = m_strAddress + m_editStrNewFileName;	//文件目的地址
	if (MoveFile(strSourceAddress, strNewAddress))
	{
		MessageBox(strJudge);
		GetParent()->SendMessage(WM_MSG, 0, 0);     //完成新建后发送消息给TESTDEMO对话框
		//SendMessage(WM_CLOSE);
	}
	else
	{
		strJudge.Format(_T("%d"), GetLastError());
		MessageBox((CString)"重命名失败！错误号为:" + strJudge);
	}
	CDialogEx::OnOK();
}
