#pragma once

// MyList

#include "NewFileDlg.h"
#include "NewFolderDlg.h"
#include "MoveFileDlg.h"
#include "RenameFileDlg.h"

class MyList : public CListCtrl
{
	DECLARE_DYNAMIC(MyList)

public:
	MyList();
	virtual ~MyList();

	void Initial(CEdit* address);
	int GetSortedCol(){return m_nSortedCol;}
	BOOL GetAsc(){return m_bAsc;}

	//获得选择的项的路径,成功为文件返回1，成功为文件夹返回2，
	//没有选择的项返回-1，没有选择路径返回-2
	INT GetSelectedItemPath(CString &strAddress);
	//获得选择的项的文件名,成功为文件返回1，成功为文件夹返回2，
	//没有选择的项返回-1;
	INT GetSelectedItemFileName(CString &strFileName);
	

protected:
	afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()


private:
	//列表排序
	static int CALLBACK ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	//将列表和地址栏联系起来
	void SetRelatedEdit(CEdit* address) { m_edtAddress = address; }

	CImageList m_RightImageList;
	int m_nSortedCol;//当前排序的列
	BOOL m_bAsc;//是否顺序排序
	CEdit* m_edtAddress;

	//复制文件的源地址
	//CString strCopySource;
	//复制的文件的文件名
	//CString strCopyFileName;

	//窗口指针
	NewFileDlg*			m_dlgNewFile;
	CNewFolderDlg*		m_dlgNewFolder;
	CMoveFileDlg*		m_dlgMoveFile;
	CRenameFileDlg*		m_dlgRenameFile;

public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEditCopy();
	// 双击列表项响应事件，打开响应列表项的文件，除了文件夹
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnEditDelete();
	afx_msg void OnEditPatse();
	afx_msg void OnEditMove();
	afx_msg void OnEditRename();
	afx_msg void OnEditNew();
	afx_msg void OnEditNewFolder();
};


