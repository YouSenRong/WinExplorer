// TestDemoDlg.h : 头文件
//

#pragma once

#include "MyTree.h"
#include "MyList.h"
#include "NewFileDlg.h"
#include "NewFolderDlg.h"
#include "MoveFileDlg.h"
#include "RenameFileDlg.h"
#include "ProcessManager.h"
#define WM_MSG WM_USER+0x01

extern CString strCopySource;		//复制文件的源地址
extern CString strCopyFileName;	//复制的文件的文件名

// CTestDemoDlg 对话框
class CTestDemoDlg : public CDialog
{
// 构造
public:
	CTestDemoDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CTestDemoDlg();
	
// 对话框数据
	enum { IDD = IDD_TESTDEMO_DIALOG };

//传递参数
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
    virtual BOOL DestroyWindow();

// 实现
protected:
	HICON m_hIcon;
public:
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize( UINT nType, int cx, int cy );
	//“转到”按钮的单击相应
	afx_msg void OnbtnGoto();        //“转到”按钮的单击响应
	afx_msg void OnbtnSearch();		//“搜索”按钮的单击响应
	DECLARE_MESSAGE_MAP()

protected:
	//将控件添加至对话框中的函数
	virtual void AddControl(CWnd* control,LPCTSTR lpszClassName,LPCTSTR lpszText, DWORD dwStyle, const RECT& rect);

	
private:
	CDialog* dlgInitital;			//主对话框
	CList<CWnd*> m_ControlList;		//保存控件的链表
	CStatic* stAddress;				//静态文本框
	CEdit* m_edtAddress;			//地址显示的文本框
	CButton* m_btnGoto;				//“转到”按钮，可用作刷新
	CStatic* m_stSearch;			//“搜索”静态文本框
	CEdit* m_edtSearch;				//搜索可编辑文本框
	CButton* m_btnSearch;			//“查找”按钮
	//CTreeCtrl* m_treLeft;
	//CListCtrl* m_lstRight;

	//CMFCShellTreeCtrl* m_treLeft;
	//CMFCShellListCtrl* m_lstRight;

	MyTree* m_treLeft;			//主窗口中左侧的目录树控件
	MyList* m_lstRight;			//主窗口中右侧的列表控件

	CRect m_rectClient;
	int m_iClientWidth;			//客户区的宽度
	int m_iClientHeight;		//客户区的高度

	
	//INT PatseOrCut;					//判断当前操作是复制还是剪切，1表示复制，2表示剪切，其他无操作

	CMenu m_rscMainMenu;

	//窗口指针
	NewFileDlg*			m_dlgNewFile;			//新建文件窗口
	CNewFolderDlg*		m_dlgNewFolder;			//新建文件夹窗口
	CMoveFileDlg*		m_dlgMoveFile;			//移动文件窗口
	CRenameFileDlg*		m_dlgRenameFile;		//从命名窗口
	CProcessManager*	m_dlgProcessManager;	//进程管理器窗口

protected:
	afx_msg LRESULT OnMsg(WPARAM wParam, LPARAM lParam);

public:
	//菜单栏按钮的消息响应函数
	afx_msg void OnEditCopy();
	afx_msg void OnEditPatse();
	afx_msg void OnEditDelete();
	afx_msg void OnHelpAbout();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnEditRename();
	afx_msg void OnEditMove();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEditNew();
	afx_msg void OnEditNewFolder();
	afx_msg void OnProcessManager();
//	afx_msg void OnEditCut();
	afx_msg void OnHelpNote();
};
