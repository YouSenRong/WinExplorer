// ProcessManager.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDemo.h"
#include "ProcessManager.h"
#include "afxdialogex.h"


// CProcessManager 对话框

IMPLEMENT_DYNAMIC(CProcessManager, CDialogEx)

CProcessManager::CProcessManager(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROCESSMANAGER_DIALOG, pParent)
{

}

CProcessManager::~CProcessManager()
{
}

void CProcessManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PROCESSMANAGER_LIST, m_listctrlProcess);
}


void CProcessManager::Initial()
{
	m_listctrlProcess.InsertColumn(0, _T("名称"), LVCFMT_LEFT, 200);
	m_listctrlProcess.InsertColumn(1, _T("PID"), LVCFMT_LEFT, 100);
	m_listctrlProcess.InsertColumn(2, _T("优先权"), LVCFMT_LEFT, 100);
	m_listctrlProcess.InsertColumn(3, _T("线程数"), LVCFMT_LEFT, 100);
}

BOOL CProcessManager::GetProcessList()
{
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass;
	INT iNum = 0;
	m_listctrlProcess.DeleteAllItems();
	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		MessageBox(TEXT("创建进程快照失败"), NULL, MB_ICONERROR);
		return FALSE;
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(hProcessSnap, &pe32))
	{
		printError(TEXT("Process32First")); // show cause of failure
		CloseHandle(hProcessSnap);          // clean the snapshot object
		return FALSE;
	}

	// Now walk the snapshot of processes, and
	// display information about each process in turn
	do
	{
		//插入列表项
		m_listctrlProcess.InsertItem(iNum, pe32.szExeFile);
		CString strProcessID;//进程ID
		CString strProcessPriority;//优先权
		CString strThreadCount;//进程下的线程数
		CString strParentProcessID;
		strProcessID.Format(_T("%d"), pe32.th32ProcessID);
		m_listctrlProcess.SetItemText(iNum, 1, strProcessID);
		strProcessPriority.Format(_T("%d"), pe32.pcPriClassBase);
		m_listctrlProcess.SetItemText(iNum, 2, strProcessPriority);
		strThreadCount.Format(_T("%d"), pe32.cntThreads);
		m_listctrlProcess.SetItemText(iNum,3, strThreadCount);
		strParentProcessID.Format(_T("%d"), pe32.th32ParentProcessID);
		m_listctrlProcess.SetItemText(iNum, 4, strParentProcessID);

		// Retrieve the priority class.
		dwPriorityClass = 0;
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
		if (hProcess == NULL)
			printError(TEXT("OpenProcess"));
		else
		{
			dwPriorityClass = GetPriorityClass(hProcess);
			if (!dwPriorityClass)
				printError(TEXT("GetPriorityClass"));
			CloseHandle(hProcess);
		}
		if (dwPriorityClass)
			_tprintf(TEXT("\n  Priority class    = %d"), dwPriorityClass);

		// List the modules and threads associated with this process
		//ListProcessModules(pe32.th32ProcessID);
		//ListProcessThreads(pe32.th32ProcessID);

	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return(TRUE);
}


BOOL CProcessManager::ListProcessModules(DWORD dwPID)
{
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32;

	// Take a snapshot of all modules in the specified process.
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	if (hModuleSnap == INVALID_HANDLE_VALUE)
	{
		printError(TEXT("CreateToolhelp32Snapshot (of modules)"));
		return(FALSE);
	}

	// Set the size of the structure before using it.
	me32.dwSize = sizeof(MODULEENTRY32);

	// Retrieve information about the first module,
	// and exit if unsuccessful
	if (!Module32First(hModuleSnap, &me32))
	{
		printError(TEXT("Module32First"));  // show cause of failure
		CloseHandle(hModuleSnap);           // clean the snapshot object
		return(FALSE);
	}

	// Now walk the module list of the process,
	// and display information about each module
	do
	{
		_tprintf(TEXT("\n\n     MODULE NAME:     %s"), me32.szModule);
		_tprintf(TEXT("\n     Executable     = %s"), me32.szExePath);
		_tprintf(TEXT("\n     Process ID     = 0x%08X"), me32.th32ProcessID);
		_tprintf(TEXT("\n     Ref count (g)  = 0x%04X"), me32.GlblcntUsage);
		_tprintf(TEXT("\n     Ref count (p)  = 0x%04X"), me32.ProccntUsage);
		_tprintf(TEXT("\n     Base address   = 0x%08X"), (DWORD)me32.modBaseAddr);
		_tprintf(TEXT("\n     Base size      = %d"), me32.modBaseSize);

	} while (Module32Next(hModuleSnap, &me32));

	CloseHandle(hModuleSnap);
	return(TRUE);
}

BOOL CProcessManager::ListProcessThreads(DWORD dwOwnerPID)
{
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32 te32;

	// Take a snapshot of all running threads
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
		return(FALSE);

	// Fill in the size of the structure before using it.
	te32.dwSize = sizeof(THREADENTRY32);

	// Retrieve information about the first thread,
	// and exit if unsuccessful
	if (!Thread32First(hThreadSnap, &te32))
	{
		printError(TEXT("Thread32First")); // show cause of failure
		CloseHandle(hThreadSnap);          // clean the snapshot object
		return(FALSE);
	}

	// Now walk the thread list of the system,
	// and display information about each thread
	// associated with the specified process
	do
	{
		if (te32.th32OwnerProcessID == dwOwnerPID)
		{
			_tprintf(TEXT("\n\n     THREAD ID      = 0x%08X"), te32.th32ThreadID);
			_tprintf(TEXT("\n     Base priority  = %d"), te32.tpBasePri);
			_tprintf(TEXT("\n     Delta priority = %d"), te32.tpDeltaPri);
			_tprintf(TEXT("\n"));
		}
	} while (Thread32Next(hThreadSnap, &te32));

	CloseHandle(hThreadSnap);
	return(TRUE);
}

void CProcessManager::printError(TCHAR* msg)
{
	DWORD eNum;
	TCHAR sysMsg[256];
	TCHAR* p;

	eNum = GetLastError();
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, eNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		sysMsg, 256, NULL);

	// Trim the end of the line and terminate it with a null
	p = sysMsg;
	while ((*p > 31) || (*p == 9))
		++p;
	do { *p-- = 0; } while ((p >= sysMsg) &&
		((*p == '.') || (*p < 33)));

	// Display the message
	_tprintf(TEXT("\n  WARNING: %s failed with error %d (%s)"), msg, eNum, sysMsg);
}



BEGIN_MESSAGE_MAP(CProcessManager, CDialogEx)

	ON_BN_CLICKED(IDC_PROCESSMANAGER_REFRESH_BTN, &CProcessManager::OnBnClickedProcessmanagerRefreshBtn)
	ON_BN_CLICKED(IDC_PROCESSMANAGER_TERMINATE_BTN, &CProcessManager::OnBnClickedProcessmanagerTerminateBtn)
END_MESSAGE_MAP()


// CProcessManager 消息处理程序


BOOL CProcessManager::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->SetWindowText(_T("进程管理器"));
	m_listctrlProcess.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listctrlProcess.InsertColumn(0, _T("名称"), LVCFMT_LEFT, 200);
	m_listctrlProcess.InsertColumn(1, _T("PID"), LVCFMT_LEFT, 100);
	m_listctrlProcess.InsertColumn(2, _T("优先权"), LVCFMT_LEFT, 100);
	m_listctrlProcess.InsertColumn(3, _T("线程数"), LVCFMT_LEFT, 100);
	m_listctrlProcess.InsertColumn(4, _T("父进程ID"), LVCFMT_LEFT, 100);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CProcessManager::OnBnClickedProcessmanagerRefreshBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	GetProcessList();
}


void CProcessManager::OnBnClickedProcessmanagerTerminateBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	INT nSelectNumber;
	CString strPID;
	LONG lPID;
	DWORD dwPID;
	nSelectNumber = m_listctrlProcess.GetSelectionMark();
	strPID = m_listctrlProcess.GetItemText(nSelectNumber, 1);
	lPID = _ttol(strPID);
	dwPID = (DWORD)lPID;
	if (dwPID < 0)
	{
		return;
	}
	else
	{
		HANDLE handle = OpenProcess(PROCESS_TERMINATE, FALSE, dwPID);
		BOOL success = TerminateProcess(handle, 4);//这里必须是4,否则无法结束进程
		if (success == 0)
		{
			MessageBox(_T("结束进程失败"), NULL, MB_ICONERROR);
		}
		else
		{
			MessageBox(_T("结束进程成功"));
		}
	}
	GetProcessList();
}


BOOL CProcessManager::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	//屏蔽掉enter键
	if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
