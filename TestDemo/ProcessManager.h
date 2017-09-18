#pragma once
#include "afxcmn.h"
#include <tlhelp32.h>


// CProcessManager 对话框

class CProcessManager : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessManager)

public:
	CProcessManager(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProcessManager();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESSMANAGER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL GetProcessList();

protected:
	
	BOOL ListProcessModules(DWORD dwPID);
	BOOL ListProcessThreads(DWORD dwOwnerPID);
	void printError(TCHAR* msg);
public:
	virtual BOOL OnInitDialog();
	void Initial();

	// 进程列表控件控制变量
	CListCtrl m_listctrlProcess;
	afx_msg void OnBnClickedProcessmanagerRefreshBtn();
	afx_msg void OnBnClickedProcessmanagerTerminateBtn();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
