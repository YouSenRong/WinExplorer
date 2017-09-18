#pragma once
#include "afxwin.h"


// CMoveFileDlg 对话框

class CMoveFileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMoveFileDlg)

public:
	CMoveFileDlg(CWnd* pParent = NULL);   // 标准构造函数
	CMoveFileDlg::CMoveFileDlg(CString SourcePath, CString FileName, CWnd* pParent = NULL );
	BOOL CMoveFileDlg::Ininatial(CString SourcePath, CString FileName);
	virtual ~CMoveFileDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOVEFILE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 文件移动目的路径控件
	CEdit m_editMoveFileDesPath;
	// 文件移动目的路径字符串
	CString m_editStrMoveFileDesPath;
	// 文件移动的文件名
	CString m_StrMoveFileName;
	// 文件移动完整源路径
	CString m_StrMoveFileSourcePath;
	virtual void OnOK();
	virtual BOOL OnInitDialog();
};
