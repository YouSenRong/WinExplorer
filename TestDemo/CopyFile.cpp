// CopyFile.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDemo.h"
#include "CopyFile.h"


// CCopyFile

IMPLEMENT_DYNAMIC(CCopyFile, CWnd)

CCopyFile::CCopyFile()
{

}

CCopyFile::~CCopyFile()
{
}

void CCopyFile::CopyDirectory(CString strSourceDictory, CString strDestiDictory)
{
	CreateDirectory(strDestiDictory, NULL); //在粘贴到的目的路径先新建文件夹
	CFileFind finder;
	CString strPath = strSourceDictory + _T("\\*.*"); //需要复制的文件的路径  
	bool bJudge = finder.FindFile(strPath);           //是否找到了需要复制的文件的路径  
	while (bJudge)
	{
		bJudge = finder.FindNextFile();
		strPath = finder.GetFilePath();
		if (strPath.Find(_T("."), 0) == -1)
		{//是不是有效的文件夹  
			CopyDirectory(finder.GetFilePath(), strDestiDictory + _T("\\") + finder.GetFileName()); //递归查找文件夹  
		}
		else
		{ //是文件则直接复制   
			CopyFile(finder.GetFilePath(), strDestiDictory + _T("\\") + finder.GetFileName(), false); //拷贝文件夹下的所有文件  
		}
	}
}

INT CCopyFile::Copy(CString strCopySource, CString strPasteDesti)
{
	if (strCopySource.Find(_T("."), 0) == -1)
	{
		CopyDirectory(strCopySource, strPasteDesti);
		return 1;
	}
	else if (CopyFile(strCopySource, strPasteDesti, false))
	{
		return 2;
	}
	else
	{
		return 0;
	}
}


BEGIN_MESSAGE_MAP(CCopyFile, CWnd)
END_MESSAGE_MAP()



// CCopyFile 消息处理程序


