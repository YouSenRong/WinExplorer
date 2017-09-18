// MyTree.cpp : 实现文件
//

#include "stdafx.h"
//#include "TestDemo.h"
#include "MyTree.h"


// MyTree

IMPLEMENT_DYNAMIC(MyTree, CTreeCtrl)

MyTree::MyTree()
{

}

MyTree::~MyTree()
{
}


BEGIN_MESSAGE_MAP(MyTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelChanged)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpandedTreedir)
END_MESSAGE_MAP()



// MyTree 消息处理程序


void MyTree::Initial(CListCtrl* listctrl,CEdit* address)
{
	this->SetRelatedList(listctrl);
	this->SetRelatedEdit(address);
	//创建ImageList
	m_LeftImageList.Create(16,16,ILC_COLOR32,16,16);
	this->SetImageList(&m_LeftImageList,TVSIL_NORMAL);               //设置TreeCtrl与ImageList关联

	m_hRoot = this->InsertItem(_T("我的电脑"),0,0,TVI_ROOT);
	GetLogicalDrives(m_hRoot);						//获取本地驱动器加载到目录树
	GetDriveDir(m_hRoot);							//获取驱动器下一级文件夹到目录树
	//InsertItemToTree(m_hRoot, _T("C:\\"), _T("桌面"));
	/*m_hDeskTop = this->InsertItem(_T("桌面"), 0, 0, TVI_ROOT);
	GetLogicalDrives(m_hDeskTop);
	GetDriveDir(m_hDeskTop);*/
}

//深度遍历树查找，获得指定内容的树节点
/*HTREEITEM MyTree::FindItemText(CTreeCtrl* tree, HTREEITEM item, CString strtext) 
{   
	HTREEITEM hfind;    

	//空树，直接返回NULL
	if(item == NULL)   
		return NULL;  

	//遍历查找
	while(item!=NULL) 
	{   
		//当前节点即所需查找节点
		if(tree->GetItemText(item) == strtext)   
			return item;   	    

		//查找当前节点的子节点
		if(tree->ItemHasChildren(item))  
		{
			item = tree->GetChildItem(item); 
			//递归调用查找子节点下节点
			hfind = FindItemText(tree,item,strtext);
			if(hfind)   
			{   
				return hfind;  
			}   
			else //子节点中未发现所需节点，继续查找兄弟节点
			{
				item = tree->GetNextSiblingItem(tree->GetParentItem(item)); 
			}
			
		} 
		else
		{ //若无子节点，继续查找兄弟节点
			 item = tree->GetNextSiblingItem(item);   
		}   

	}
    return item;   
}*/

//广度遍历树查找，返回指定路径下的节点
HTREEITEM MyTree::FindItem(HTREEITEM item,CString strPath)
{
	HTREEITEM hfind;
	if(strPath.Right(1) == "\\")  //去除右侧'\'
	{
		strPath.Delete(strPath.GetLength()-1);
	}
	CString DirName;
	int pos = strPath.Find('\\');
	if(pos>0)
	    DirName = strPath.Left(pos);
	else
		DirName = strPath;

	//遍历查找
   while(item!=NULL) 
   {   
	   CString curDir = this->GetItemText(item);
	   //去除最右边的"\"
		if(curDir.Right(1) == "\\")
		{
			curDir.Delete(curDir.GetLength()-1);
		}
	  //当前节点即所需查找节点
		if(curDir.CompareNoCase(DirName) == 0)
	  {
		  if(curDir.CompareNoCase(strPath) == 0)
			  return item; 
		  //如果找到节点，继续往下找子节点
		  else if(this->ItemHasChildren(item))
		  {
			  item = this->GetChildItem(item); 
			  //递归调用查找子节点下节点
			  hfind = FindItem(item,strPath.Mid(pos+1));
			  if(hfind)   
			  {   
				return hfind;  
			  }
		  }
		  return NULL;  //未查找到 
	  }
	  else
	  {
		  //查找兄弟节点
		  if(this->GetNextSiblingItem(item) != NULL)
		  {
			  item = this->GetNextSiblingItem(item);
		  }
          //若无兄弟节点，继续查找子节点
		  else if(this->ItemHasChildren(item))
		  {
			  item = this->GetChildItem(item); 
		  }
		  else
			  return NULL;     //未查找到 

		  hfind = FindItem(item,strPath);
		  if(hfind)   
		  {   
			return hfind;  
		  }
	  }
   }
	return item;
}

// 获取盘符到目录树，作为根目录下一级节点
void MyTree::GetLogicalDrives(HTREEITEM hRoot)
{
	size_t szAllDrivesStrings = GetLogicalDriveStrings(0,NULL);							//获取驱动器字符长度
	TCHAR* pDriveStrings = new TCHAR[szAllDrivesStrings + sizeof(_T(""))];				//定义字符串存储驱动器字符pDriveStrings,数组指针
	GetLogicalDriveStrings((DWORD)szAllDrivesStrings,pDriveStrings);					//获取驱动器字符到pDriveStrings
	//截取单个驱动器字符出来添加到目录树上
	//定义临时字符指针指向驱动器字符信息
	//(为了最后可以释放驱动器字符串方便,如果直接移动字符串指针,后面调用delete释放字符串资源的时候会报错)
	TCHAR* pTempDrive = pDriveStrings;
	size_t szDriveString = wcslen(pTempDrive);

	//CString strDrivepame;			//驱动器的名字
	//UINT uDriveType;				//驱动器的类型
	while(szDriveString>0)
	{
		//strDrivepame = pTempDrive;								//复制驱动器的名字
		//uDriveType = GetDriveType(strDrivepame);				//获取驱动器的类型
		//this->InsertItem(pTempDrive,hRoot);					//加载解析后的一个驱动器信息到目录树
		InsertItemToTree(hRoot, pTempDrive, pTempDrive);
		pTempDrive += szDriveString + 1;
		szDriveString = wcslen(pTempDrive);
	}
	delete pDriveStrings;
}

// 获取驱动器下一级目录到目录树上显示
void MyTree::GetDriveDir(HTREEITEM hRoot)
{
	HTREEITEM hChild = this->GetChildItem(hRoot);				//获取根节点下的第一个盘符节点
	while(hChild)
	{
		//获取查找路径
		CString strText = this->GetItemText(hChild);
		if(strText.Right(1) != _T("\\"))
		{
			strText += _T("\\");
		}
		strText += "*.*";
		CFileFind fileFind;
		BOOL bContinue = fileFind.FindFile(strText);
		while(bContinue)
		{
			bContinue = fileFind.FindNextFile();
			//磁盘下的文件是目录并且不是[.]或[..]的时候，加载到目录树
			if(fileFind.IsDirectory() && !fileFind.IsDots())
			{
				//this->InsertItem(fileFind.GetFileName(),hChild);
				CString tempPath = strText;
				int index = tempPath.Find(_T("*.*"));
				tempPath.Delete(index,3);
				InsertItemToTree(hChild,tempPath+fileFind.GetFileName(),fileFind.GetFileName());
			}
		}
		//到下一个驱动器
		hChild = this->GetNextItem(hChild,TVGN_NEXT);
	}
}

// 根据目录树中任一节点获取其所有子目录
void MyTree::AddSubDir(HTREEITEM hParent)
{
	CString strPath = GetFullPath(hParent);
	if(strPath.Right(1) != "\\")
	{
		strPath += "\\";
	}
	strPath += "*.*";
	CFileFind fileFind;
	BOOL bContinue = fileFind.FindFile(strPath);
	while(bContinue)
	{
		bContinue = fileFind.FindNextFile();
		if(fileFind.IsDirectory() && !fileFind.IsDots()&&!fileFind.IsHidden())
		{
			//this->InsertItem(fileFind.GetFileName(),hParent);
			CString tempPath = strPath;
			int index = tempPath.Find(_T("*.*"));
			strPath.Delete(index,3);
			InsertItemToTree(hParent,tempPath+fileFind.GetFileName(),fileFind.GetFileName());
		}
	}
}

// 获取某节点的文件路径
CString MyTree::GetFullPath(HTREEITEM hCurrent)
{
	CString strTemp = _T("");
	CString strFullPath = _T("");
	while(hCurrent != m_hRoot)
	{
		//从当前节点找起，依次找出其父节点，到根节点结束
		strTemp = this->GetItemText(hCurrent);
		if(strTemp.Right(1) != _T("\\"))
			strTemp += _T("\\");
		strFullPath = strTemp + strFullPath;
		hCurrent = this->GetParentItem(hCurrent);
	}
	return strFullPath;
}

// 展开某节点触发事件
void MyTree::OnItemexpandedTreedir(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	TVITEM item = pNMTreeView->itemNew;				//获取当前展开节点
	//如果当前展开节点为根节点,则返回
	if(item.hItem == m_hRoot)
		return;
	HTREEITEM hChild = this->GetChildItem(item.hItem);
	if(pNMTreeView->action == 1)  //收起
	{	
		while(hChild!=NULL)
		{
			while(this->ItemHasChildren(hChild))
			{
				HTREEITEM item = this->GetChildItem(hChild);
				this->DeleteItem(item);
			}
		    hChild = this->GetNextItem(hChild,TVGN_NEXT);
		}
	}
	else if(pNMTreeView->action == 2) //展开
	{
		//轮训展开节点的每个子节点，加载文件目录信息到子节点上
		while(hChild!=NULL)
		{
			AddSubDir(hChild);
			hChild = this->GetNextItem(hChild,TVGN_NEXT);
		}
	}

	*pResult = 0;
}


void MyTree::OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	this->Expand(this->GetSelectedItem(), TVE_EXPAND);

	m_lstRight->DeleteAllItems();					//删除ListView中所有元素
	//获取当前目录路径
	TVITEM item = pNMTreeView->itemNew;
	if(item.hItem == m_hRoot)
	{
		m_edtAddress->SetWindowText(_T("我的电脑"));
		return;
	}
	CString strDirPath = GetFullPath(item.hItem);
	m_edtAddress->SetWindowText(strDirPath); //更新地址栏地址
	if(strDirPath.Right(1)=="\\")
	{
		strDirPath += "*.*";
	}
	//查找当前目录下的所有文件添加到ListView中
	CFileFind fileFind;
	BOOL bContinue = fileFind.FindFile(strDirPath);
	int iNum = 0;
	while(bContinue)
	{
		bContinue = fileFind.FindNextFile();
		if(/*!fileFind.IsDirectory() && */!fileFind.IsDots() && !fileFind.IsHidden())
		{
			SHFILEINFO fileInfo;
			CString tempPath = strDirPath;
			int index = tempPath.Find(_T("*.*"));
			tempPath.Delete(index,3);
			SHGetFileInfo(tempPath + fileFind.GetFileName(),0,&fileInfo,sizeof(&fileInfo),SHGFI_ICON | SHGFI_DISPLAYNAME | SHGFI_TYPENAME );
			int iIcon = m_lstRight->GetImageList(LVSIL_SMALL)->Add(fileInfo.hIcon);
			int ret = m_lstRight->InsertItem(iNum,fileInfo.szDisplayName,iIcon);
			
			m_lstRight->SetItemText(iNum,2,fileInfo.szTypeName);

			//声明文件属性变量
			CFileStatus status;
			CFile::GetStatus(tempPath + fileFind.GetFileName(), status);
			CString strSize;
			strSize.Format(_T("%.3f KB"),status.m_size / 1024.0);
            m_lstRight->SetItemText(iNum,1,strSize);

			CString strModifyTime = status.m_ctime.Format("%Y-%m-%d %H:%M:%S");
			m_lstRight->SetItemText(iNum,3,strModifyTime);
			m_lstRight->SetItemText(iNum, 4, fileFind.GetFilePath());

			iNum++;
		}
	}
	 *pResult = 0;
}

HTREEITEM MyTree::InsertItemToTree(HTREEITEM hParent, CString strPath, CString DisplayName)
{
	SHFILEINFO fileInfo;
	SHGetFileInfo(strPath,0,&fileInfo,sizeof(&fileInfo),SHGFI_ICON|SHGFI_DISPLAYNAME|SHGFI_TYPENAME);
	int iIcon = m_LeftImageList.Add(fileInfo.hIcon);
	return this->InsertItem(/*fileInfo.szDisplayName*/DisplayName,iIcon,iIcon,hParent);
}
