#pragma once


// MyTree

class MyTree : public CTreeCtrl
{
	DECLARE_DYNAMIC(MyTree)

public:
	MyTree();
	virtual ~MyTree();
	void Initial(CListCtrl* listctrl,CEdit* address);

	//深度遍历树查找，获得指定内容的树节点
    HTREEITEM FindItemText(CTreeCtrl* tree,HTREEITEM item, CString strtext);  
	//广度遍历树查找，
	HTREEITEM FindItem(HTREEITEM item,CString strPath);   

protected:
	 //设置相关的列表视图
	void SetRelatedList(CListCtrl* listctrl){m_lstRight = listctrl;}
	//设置相关的地址栏
	void SetRelatedEdit(CEdit* address){m_edtAddress = address;}
	HTREEITEM InsertItemToTree(HTREEITEM hParent, CString strPath,CString DisplayName);
	// 获取盘符到目录树，作为根目录下一级节点
	void GetLogicalDrives(HTREEITEM hRoot);
	// 获取驱动器下一级目录到目录树上显示
	void GetDriveDir(HTREEITEM hRoot);
	// 根据目录树中任一节点获取其所有子目录
	void AddSubDir(HTREEITEM hParent);
	// 获取某节点的文件路径
	CString GetFullPath(HTREEITEM hCurrent);
protected:
	afx_msg void OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnItemexpandedTreedir(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()


private:
	//根节点
	HTREEITEM m_hRoot;
	/*//桌面节点
	HTREEITEM m_hDeskTop;
	CString m_strDeskTopPath;*/
	//Image List
	CImageList m_LeftImageList;
	CListCtrl* m_lstRight;
	CEdit* m_edtAddress;
	//CString m_DriveType[7] = { _T("（未知）"), _T("（无效路径）"), _T("（移动磁盘）"), _T("（磁盘）"), _T("（网络驱动器）"), _T("（CD/DVD）"), _T("（随机存储磁盘）") };
};


