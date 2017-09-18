// MyList.cpp : 实现文件
//

#include "stdafx.h"
#include "MyList.h"
#include "TestDemo.h"
#include <iostream>
#include <string>
#include "resource.h"
#include "TestDemoDlg.h"
#include "DeleteFile.h"
#include "CopyFile.h"
using namespace std;


// MyList

IMPLEMENT_DYNAMIC(MyList, CListCtrl)

MyList::MyList()
{
	m_dlgNewFile = NULL;
	m_dlgNewFolder = NULL;
	m_dlgMoveFile = NULL;
	m_dlgRenameFile = NULL;
}

MyList::~MyList()
{
	delete m_dlgNewFile;
	delete m_dlgNewFolder;
	delete m_dlgMoveFile;
	delete m_dlgRenameFile;
}


BEGIN_MESSAGE_MAP(MyList, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK,OnColumnClick)
//	ON_NOTIFY_REFLECT(NM_RCLICK, &MyList::OnNMRClick)
ON_WM_RBUTTONDOWN()
ON_COMMAND(IDM_EDIT_COPY, &MyList::OnEditCopy)
ON_NOTIFY_REFLECT(NM_DBLCLK, &MyList::OnNMDblclk)
//ON_WM_RBUTTONUP()
ON_COMMAND(IDM_EDIT_DELETE, &MyList::OnEditDelete)
ON_COMMAND(IDM_EDIT_PATSE, &MyList::OnEditPatse)
ON_COMMAND(IDM_EDIT_MOVE, &MyList::OnEditMove)
ON_COMMAND(IDM_EDIT_RENAME, &MyList::OnEditRename)
ON_COMMAND(IDM_EDIE_NEW_FILE, &MyList::OnEditNew)
ON_COMMAND(IDM_EDIT_NEW_FOLDER, &MyList::OnEditNewFolder)
END_MESSAGE_MAP()



// MyList 消息处理程序

void MyList::Initial(CEdit* address)
{
	m_RightImageList.Create(16,16,ILC_COLOR32,16,16);
	this->SetImageList(&m_RightImageList,LVSIL_SMALL);			//设置ListCtrl与ImageList关联

	this->InsertColumn(0,	_T("名称"),		LVCFMT_LEFT,	200);
	this->InsertColumn(1,	_T("大小"),		LVCFMT_LEFT,	160);
	this->InsertColumn(2,	_T("类型"),		LVCFMT_LEFT,	180);
	this->InsertColumn(3,	_T("修改日期"),	LVCFMT_LEFT,	180);
	this->InsertColumn(4,	_T("路径"),		LVCFMT_LEFT,	300);

	m_edtAddress = address;
}

void MyList::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	//MessageBox(_T("OnColumnClick"));
    int nCouter,i;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	 
	MyList *m_FileList=this;
	if(pNMListView->iSubItem == m_nSortedCol)
	{
		m_bAsc=!m_bAsc;
	}
	else
	{
		m_bAsc=this->GetStyle() & LVS_SORTASCENDING;;
		m_nSortedCol = pNMListView->iSubItem;
	}
	nCouter=m_FileList->GetItemCount();
	for (i=0;i<nCouter;i++)
	{
		CString strText = m_FileList->GetItemText(i,m_nSortedCol);
		m_FileList->SetItemData(i,i);
	}
	m_FileList->SortItems(ListCompare,(DWORD)m_FileList);
	*pResult = 0;
}

int CALLBACK MyList::ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	MyList *sort=(MyList *)lParamSort;
	CString string1 = sort->GetItemText(lParam1,sort->GetSortedCol());
	CString string2= sort->GetItemText(lParam2,sort->GetSortedCol());

	//if(sort->GetAsc()==TRUE)
	//	return string1>string2;
	//else 
	//	return string1<string2;

	if(sort->GetAsc()==TRUE)
		return string1.CompareNoCase(string2);
	else 
		return string1.CompareNoCase(string2)*(-1);
	
}

void MyList::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetWindowRect(&rect); //得到控件窗口区域

	int x = point.x + rect.left; //计算菜单显示位置的X坐标
	int y = point.y + rect.top; //计算菜单显示位置的y坐标

	/*if (!GetSelectedCount()) //如果没有列表项被选中
	{
		return;
	}*/

	CMenu* m_PopMenu = new CMenu;
	m_PopMenu->LoadMenu(IDR_MENU_MAINFRAME); //加载菜单

	//弹出快捷菜单
	TrackPopupMenu(m_PopMenu->GetSubMenu(1)->m_hMenu, 0, x, y, 0, this->GetSafeHwnd(), &rect);

	CListCtrl::OnRButtonDown(nFlags, point);
}

// 双击列表项响应事件，打开响应列表项的文件，除了文件夹
void MyList::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CString strFilePath, strFileName;
	INT iNum = 0;
	CFileFind fileFind;
	int nSelectNumber;
	nSelectNumber = GetSelectionMark();
	strFileName = GetItemText(nSelectNumber, 0);
	m_edtAddress->GetWindowText(strFilePath);
	if (GetItemText(nSelectNumber, 2) == _T("文件夹"))//如果是文件夹则在
	{
		DeleteAllItems();
		strFilePath = strFilePath + strFileName + _T("\\");
		m_edtAddress->SetWindowText(strFilePath);
		strFilePath += "*.*";
		//MessageBox(_T("文件夹"));
		BOOL bContinue = fileFind.FindFile(strFilePath);
		while (bContinue)
		{
			bContinue = fileFind.FindNextFile();
			if (/*!fileFind.IsDirectory() && */!fileFind.IsDots() && !fileFind.IsHidden())
			{
				SHFILEINFO fileInfo;
				CString tempPath = strFilePath;
				int index = tempPath.Find(_T("*.*"));
				tempPath.Delete(index, 3);
				SHGetFileInfo(tempPath + fileFind.GetFileName(), 0, &fileInfo, sizeof(&fileInfo), SHGFI_ICON | SHGFI_DISPLAYNAME | SHGFI_TYPENAME);
				int iIcon = GetImageList(LVSIL_SMALL)->Add(fileInfo.hIcon);
				int ret = InsertItem(iNum, fileInfo.szDisplayName, iIcon);

				SetItemText(iNum, 2, fileInfo.szTypeName);

				//声明文件属性变量
				CFileStatus status;
				CFile::GetStatus(tempPath + fileFind.GetFileName(), status);
				CString strSize;
				strSize.Format(_T("%.3f KB"), status.m_size / 1024.0);
				SetItemText(iNum, 1, strSize);

				CString strModifyTime = status.m_ctime.Format("%Y-%m-%d %H:%M:%S");
				SetItemText(iNum, 3, strModifyTime);
				SetItemText(iNum, 4, fileFind.GetFilePath());

				iNum++;
			}
		}
	}
	//如果是文件则调用ShellExecute采用默认方式打开文件
	else
	{
		strFileName = strFilePath + strFileName;
		ShellExecute(NULL, _T("open"), strFileName, 0, 0, SW_SHOWNORMAL);
	}
	*pResult = 0;
}

//获得选择的项的文件名,成功为文件返回1，成功为文件夹返回2，没有选择的项返回-1;
INT MyList::GetSelectedItemFileName(CString &strFileName)
{
	int nSelectNumber;                   //鼠标所选文件的行号
	nSelectNumber = this->GetSelectionMark(); //获得选中的行的行号
	if (nSelectNumber == -1)                           //未选中文件，则提示删除失败
	{
		return -1;
	}
	strFileName = GetItemText(nSelectNumber, 0);
	if (GetItemText(nSelectNumber, 2) == _T("文件夹") )//是文件夹则找不到".",故返回-1
	{
		return 2;
	}
	else
		return 1;
	
}

//获得选择的项的路径,成功返回1，没有选择的项返回-1，没有选择路径返回-2
INT MyList::GetSelectedItemPath(CString &strAddress)
{
	CString strFileName;
	m_edtAddress->GetWindowText(strAddress);
	INT FileType;
	if (strAddress.IsEmpty())
	{
			return -2;
	}
	FileType = GetSelectedItemFileName(strFileName);
	if(FileType == 1)
	{
		strAddress = strAddress + strFileName;
		return 1;
	}
	else if (FileType == 2)
	{
		strAddress = strAddress + strFileName;
		return 2;
	}
	else
	{
		return -1;
	}
	return 1;
}
//右键弹出菜单响应事件
//新建
void MyList::OnEditNew()
{
	// TODO: 在此添加命令处理程序代码
	CString strAddress;
	m_edtAddress->GetWindowText(strAddress);  //获取当前路径
	if (strAddress.IsEmpty())                   //路径不能为空
	{
		MessageBox(_T("新建失败！路径为空"), NULL, MB_ICONERROR);
		return;
	}
	if (m_dlgNewFile == NULL)
	{
		m_dlgNewFile = new NewFileDlg();
		m_dlgNewFile->Create(IDD_NEWFILE_DIALOG, this);
	}
	//打开新建文件的对话框
	m_dlgNewFile->Initital(strAddress);
	m_dlgNewFile->ShowWindow(SW_SHOW);	                     //显示此对话框,非模态
}

//新建文件夹
void MyList::OnEditNewFolder()
{
	// TODO: 在此添加命令处理程序代码
	CString strAddress;
	m_edtAddress->GetWindowText(strAddress);	//获取当前路径
	if (strAddress.IsEmpty())					 //路径不能为空
	{
		MessageBox(_T("新建失败！路径为空"), NULL, MB_ICONERROR);
		return;
	}
	if (m_dlgNewFolder == NULL)
	{
		m_dlgNewFolder = new CNewFolderDlg();
		m_dlgNewFolder->Create(IDD_NEWFOLDER_DIALOG, this);		//打开新建文件的对话框
	}
	m_dlgNewFolder->Initial(strAddress);
	m_dlgNewFolder->ShowWindow(SW_SHOW);						//显示此对话框
}


//复制
void MyList::OnEditCopy()
{
	// TODO: 在此添加命令处理程序代码
	CString strAddress;
	switch (GetSelectedItemPath(strAddress))
	{
	case 1:
		strCopySource = strAddress;
		GetSelectedItemFileName(strCopyFileName);
		MessageBox(_T("文件复制成功"));
		break;
	case 2:
		strCopySource = strAddress;
		GetSelectedItemFileName(strCopyFileName);
		MessageBox(_T("文件夹复制成功"));
		break;
	//没有选择项
	case -1:
		MessageBox(_T("当前没有选择项"), NULL, MB_ICONWARNING);
		break;
	//没有选择路径
	case -2:
		MessageBox(_T("当前路径为空"), NULL, MB_ICONWARNING);
		break;
	}
}
//粘贴
void MyList::OnEditPatse()
{
	// TODO: 在此添加命令处理程序代码
	CString strAddress;
	m_edtAddress->GetWindowText(strAddress);
	if (strAddress.IsEmpty())
	{
		MessageBox(_T("当前路径为空"), NULL, MB_ICONWARNING);
		return;
	}
	CCopyFile CF;
	CString strPasteDesti = strAddress + "\\" + strCopyFileName;   //要粘贴到的目的地址
	if (strCopySource.IsEmpty()) return;
	switch (CF.Copy(strCopySource, strPasteDesti))
	{
	case 1:
		GetParent()->SendMessage(WM_MSG, 0, 0);     //完成新建后发送消息给TESTDEMO对话框
		MessageBox(_T("文件夹粘贴成功!"));
		break;
	case 2:
		GetParent()->SendMessage(WM_MSG, 0, 0);     //完成新建后发送消息给TESTDEMO对话框
		MessageBox(_T("文件粘贴成功!"));
		break;
	case 0:
		MessageBox(_T("文件粘贴失败!"), NULL, MB_ICONERROR);
		break;
	}
}
//删除
void MyList::OnEditDelete()
{
	// TODO: 在此添加命令处理程序代码
	CString strAddress;
	CDeleteFile DF;
	switch (GetSelectedItemPath(strAddress))
	{
	case -1:
		MessageBox(_T("当前没有选择项"), NULL, MB_ICONWARNING);
		return;
		//没有选择路径
	case -2:
		MessageBox(_T("当前路径为空"), NULL, MB_ICONWARNING);
		return;
	}
	switch (DF.Delete(strAddress))
	{
	case 1:
		MessageBox(_T("文件夹删除成功"));
		break;
	case 2:
		MessageBox(_T("文件删除成功"));
		break;
	case 0:
		MessageBox(_T("删除失败"));
		break;
	}
	GetParent()->SendMessage(WM_MSG, 0, 0);     //完成新建后发送消息给TESTDEMO对话框
}
//移动
void MyList::OnEditMove()
{
	// TODO: 在此添加命令处理程序代码
	CString strAddress;
	CString strFileName;
	if (m_dlgMoveFile == NULL)
	{
		m_dlgMoveFile = new CMoveFileDlg();
		m_dlgMoveFile->Create(IDD_MOVEFILE_DIALOG, this);
	}
	switch (GetSelectedItemPath(strAddress))
	{
	case 1:
		GetSelectedItemFileName(strFileName);
		m_dlgMoveFile->Ininatial(strAddress, strFileName);
		m_dlgMoveFile->ShowWindow(SW_SHOW);
		break;
	//没有选择项
	case -1:
		MessageBox(_T("当前没有选择项"), NULL, MB_ICONWARNING);
		break;
	//没有选择路径
	case -2:
		MessageBox(_T("当前路径为空"), NULL, MB_ICONWARNING);
		break;
	}
}
//重命名
void MyList::OnEditRename()
{
	// TODO: 在此添加命令处理程序代码
	CString strAddress;
	CString strFileName;
	if (m_dlgRenameFile == NULL)
	{
		m_dlgRenameFile = new CRenameFileDlg();
		m_dlgRenameFile->Create(IDD_RENAMEFILE_DIALOG, this);
	}
	switch (GetSelectedItemPath(strAddress))
	{
	case 1:
		m_edtAddress->GetWindowText(strAddress);
		GetSelectedItemFileName(strFileName);
		m_dlgRenameFile->Initital(strFileName, strAddress);
		m_dlgRenameFile->ShowWindow(SW_SHOW);
		break;
	//没有选择项
	case -1:
		MessageBox(_T("当前没有选择项"), NULL, MB_ICONWARNING);
		break;
	//没有选择路径
	case -2:
		MessageBox(_T("当前路径为空"), NULL, MB_ICONWARNING);
		break;
	}
}
