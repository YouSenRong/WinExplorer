#pragma once


// CCopyFile

class CCopyFile : public CWnd
{
	DECLARE_DYNAMIC(CCopyFile)

public:
	CCopyFile();
	virtual ~CCopyFile();
public:
	INT Copy(CString strCopySource, CString strPasteDesti);
protected:
	void CopyDirectory(CString strSourceDictory, CString strDestiDictory);

protected:
	DECLARE_MESSAGE_MAP()
};


