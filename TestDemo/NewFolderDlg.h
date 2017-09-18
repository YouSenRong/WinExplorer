#pragma once
#include "afxwin.h"


// CNewFolderDlg 对话框

class CNewFolderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewFolderDlg)

public:
	CNewFolderDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewFolderDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWFOLDER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 路径编辑框的控制变量
	CEdit m_editFolderPath;
	// 路径编辑框路径内容
	CString m_editStrFolderPath;
	// 文件夹名编辑框控制变量
	CEdit m_editFolderName;
	// 文件夹名名称内容
	CString m_editStrFolderName;

public:
	void Initial(CString strDirPath);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
