#pragma once
#include "afxwin.h"


// NewFileDlg 对话框

class NewFileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NewFileDlg)

public:
	NewFileDlg(CWnd* pParent = NULL);   // 标准构造函数
	NewFileDlg(CString strAddress, CWnd* pParent = NULL);
	//自定义函数，初始化工作
	void Initital();
	//自定义函数，初始化工作
	void Initital(CString strAddress);
	virtual ~NewFileDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWFILE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CEdit m_Edit_NewFile_Path;                      //第一个文本框，即为新建文件的地址
	CEdit m_Edit_NewFile_FileName;                  //第二个文本框，即为新建文件的名字，要求带后缀
	CString m_strAddressPath;							//文件路径
	CString m_strFileName;							//文件名
	afx_msg void OnBnClickedNewFileNew();			//"新建"按钮
	afx_msg void OnBnClickedNewFileCancle();		//"取消"按钮
	afx_msg void OnEnChangeEditNewfilePath();
	virtual void OnOK();
};
