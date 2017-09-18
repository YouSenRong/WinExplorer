// NewFolderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDemo.h"
#include "NewFolderDlg.h"
#include "afxdialogex.h"

#define WM_MSG WM_USER+0x01

// CNewFolderDlg 对话框

IMPLEMENT_DYNAMIC(CNewFolderDlg, CDialogEx)

CNewFolderDlg::CNewFolderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NEWFOLDER_DIALOG, pParent)
	, m_editStrFolderPath(_T(""))
	, m_editStrFolderName(_T(""))
{

}

CNewFolderDlg::~CNewFolderDlg()
{
}

void CNewFolderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NEWFOLDER_PATH, m_editFolderPath);
	DDX_Text(pDX, IDC_EDIT_NEWFOLDER_PATH, m_editStrFolderPath);
	DDV_MaxChars(pDX, m_editStrFolderPath, 100);
	DDX_Control(pDX, IDC_EDIT_NEWFOLDER_FOLDERNAME, m_editFolderName);
	DDX_Text(pDX, IDC_EDIT_NEWFOLDER_FOLDERNAME, m_editStrFolderName);
	DDV_MaxChars(pDX, m_editStrFolderName, 100);
}


void CNewFolderDlg::Initial(CString strDirPath)
{
	m_editStrFolderPath = strDirPath;
	UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CNewFolderDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CNewFolderDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CNewFolderDlg 消息处理程序


BOOL CNewFolderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_editFolderName.SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CNewFolderDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_editFolderPath.GetWindowText(m_editStrFolderPath);
	m_editFolderName.GetWindowText(m_editStrFolderName);
	if (m_editStrFolderName.IsEmpty())
	{
		MessageBox(_T("新建失败，请输入文件夹名！"), NULL, MB_ICONERROR);
		return;
	}
	m_editStrFolderPath = m_editStrFolderPath + "\\" + m_editStrFolderName;	//完整的地址（包含路径和文件名）
	if (CreateDirectory(m_editStrFolderPath, NULL))
	{
		MessageBox(_T("新建成功"));
	}
	GetParent()->SendMessage(WM_MSG, 0, 0);		//完成新建后发送消息给TESTDEMO对话框
	CDialogEx::OnOK();
}
