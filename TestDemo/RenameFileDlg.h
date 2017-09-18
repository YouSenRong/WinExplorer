#pragma once
#include "afxwin.h"


// CRenameFileDLg 对话框

class CRenameFileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRenameFileDlg)

public:
	CRenameFileDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRenameFileDlg();
	void Initital(CString strFilename, CString strAddress);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RENAMEFILE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	// 原文件名编辑框控件
	CEdit m_editPreFileName;
	// 新文件名编辑框控件
	CEdit m_editNewFileName;
	// 新文件名
	CString m_editStrNewFileName;
	// 原文件名
	CString m_editStrPreFileName;
	// 文件路径
	CString m_strAddress;
	virtual void OnOK();
};
