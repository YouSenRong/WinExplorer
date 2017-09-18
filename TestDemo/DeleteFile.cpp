// DeleteFile.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDemo.h"
#include "DeleteFile.h"


// CDeleteFile

IMPLEMENT_DYNAMIC(CDeleteFile, CWnd)

CDeleteFile::CDeleteFile()
{

}

CDeleteFile::~CDeleteFile()
{
}


void CDeleteFile::DeleteDirectory(CString strDeleteDictory)  //循环加递归的函数，用于删除文件夹
{
	CFileFind finder;
	CString strPath = strDeleteDictory + _T("\\*.*"); //需要删除的文件的路径  
	bool bJudge = finder.FindFile(strPath); //是否找到了需要删除的文件的路径  
	while (bJudge)
	{
		bJudge = finder.FindNextFile();
		strPath = finder.GetFileName();
		if (strPath.Find(_T("."), 0) == -1)
		{//是不是有效的文件夹  
			DeleteDirectory(finder.GetFilePath());   //递归查找文件夹  
			RemoveDirectory(finder.GetFilePath());     //删完文件后删除此文件夹
		}
		else
		{ //是文件则直接删除   
			DeleteFile(finder.GetFilePath());         //删除文件夹下的所有文件  
		}
	}
}

INT CDeleteFile::Delete(CString strAddress)
{
	if (strAddress.Find(_T("."), 0) == -1)
	{
		DeleteDirectory(strAddress);
		RemoveDirectory(strAddress);
		return 1;
	}
	else if (DeleteFile(strAddress))
	{
		return 2;
	}
	else
	{
		return 0;
	}
}

BEGIN_MESSAGE_MAP(CDeleteFile, CWnd)
END_MESSAGE_MAP()


// CDeleteFile 消息处理程序


