// MoveFileDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDemo.h"
#include "MoveFileDlg.h"
#include "afxdialogex.h"
#define WM_MSG WM_USER+1

// CMoveFileDlg 对话框

IMPLEMENT_DYNAMIC(CMoveFileDlg, CDialogEx)

CMoveFileDlg::CMoveFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MOVEFILE_DIALOG, pParent)
	, m_editStrMoveFileDesPath(_T(""))
{

}

CMoveFileDlg::CMoveFileDlg( CString SourcePath, CString FileName, CWnd* pParent /*=NULL*/ )
	: CDialogEx(IDD_MOVEFILE_DIALOG, pParent)
	, m_editStrMoveFileDesPath(_T(""))
{
	m_StrMoveFileSourcePath = SourcePath;
	m_StrMoveFileName = FileName;
}

BOOL CMoveFileDlg::Ininatial(CString strSourcePath, CString strFileName)
{
	m_StrMoveFileSourcePath = strSourcePath;
	m_editStrMoveFileDesPath = strSourcePath;
	m_StrMoveFileName = strFileName;
	UpdateData(FALSE);
	return TRUE;
}

BOOL CMoveFileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_editMoveFileDesPath.SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

CMoveFileDlg::~CMoveFileDlg()
{
}

void CMoveFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MOVEFILEPATH, m_editMoveFileDesPath);
	DDX_Text(pDX, IDC_EDIT_MOVEFILEPATH, m_editStrMoveFileDesPath);
}


BEGIN_MESSAGE_MAP(CMoveFileDlg, CDialogEx)
END_MESSAGE_MAP()


// CMoveFileDlg 消息处理程序


void CMoveFileDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CString strJudge;
	m_editMoveFileDesPath.GetWindowText(m_editStrMoveFileDesPath);
	if (m_editStrMoveFileDesPath.Right(1) != _T("\\"))
	{
		m_editStrMoveFileDesPath += _T("\\");
	}
	m_editStrMoveFileDesPath = m_editStrMoveFileDesPath + m_StrMoveFileName;   //要移动到的目的地址
	if (MoveFile(m_StrMoveFileSourcePath, m_editStrMoveFileDesPath/*, MOVEFILE_COPY_ALLOWED*/))             //不允许重名替换
	{
		strJudge = (CString)"文件移动成功!";
		MessageBox(strJudge);
		GetParent()->SendMessage(WM_MSG, 0, 0);     //完成新建后发送消息给TESTDEMO对话框
	}
	else
	{
		strJudge.Format(_T("%d"), GetLastError());
		MessageBox((CString)"移动失败！错误号为:" + strJudge, NULL, MB_ICONERROR);
	}
	CDialogEx::OnOK();
}
