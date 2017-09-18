#pragma once


// CDeleteFile

class CDeleteFile : public CWnd
{
	DECLARE_DYNAMIC(CDeleteFile)

public:
	CDeleteFile();
	virtual ~CDeleteFile();

public:
	INT Delete(CString strAddress);
	void DeleteDirectory(CString strDeleteDictory);

protected:
	DECLARE_MESSAGE_MAP()

};


