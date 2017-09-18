#pragma once


//自定义结构体，用于多线程的参数传递
struct ThreadParameter
{
	CString strDirPath;
	CString strSearchFileName;
	ThreadParameter() {}	//默认构造函数
	ThreadParameter(CString strDirPath, CString strSearchFileName)//构造函数
	{
		this->strDirPath = strDirPath;
		this->strSearchFileName = strSearchFileName;
	}
	BOOL SetThreadParameter(CString strDirPath, CString strSearchFileName)
	{
		this->strDirPath = strDirPath;
		this->strSearchFileName = strSearchFileName;
		return TRUE;
	}
};

//链表控件，显示搜索结果
static CListCtrl *SearchFileListCtrl;

// CSearchFile
//搜索文件的类
class CSearchFile : public CWnd
{
	DECLARE_DYNAMIC(CSearchFile)

public:
	CSearchFile();
	virtual ~CSearchFile();

protected:
	DECLARE_MESSAGE_MAP()
protected:
public:
	//调用SearchFileThread函数启动一个新的线程进行文件搜索
	//路径为空返回-1，搜索文件名为空返回-2，成功返回1
	UINT StartSearchFile(CString strDirPath, CString strSearchFileName);
	//设置关联的列表控件
	void SetReleatedListCtrl(CListCtrl* ListCtrl);
private:
	//按文件名迭代广度优先搜索文件，多线程函数
	static UINT SearchFileThread(LPVOID pParam);

};


