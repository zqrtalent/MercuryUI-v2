#pragma once
#include "packheader.h"

//    SPFSPacker
class SPFSPacker
{
public:
	SPFSPacker       ();
	~SPFSPacker      ();
	void SetFileSecSz(UINT   fDataSectionSz  /*in *//*sets section size for file.*/);
	void AddFolder   (LPCSTR lpszPackFile /*in *//*pack file path to create*/,
				      LPCSTR lpszDir      /*in *//*directory of files.*/,
				      LPCSTR lpszFMask    /*in *//*mask of file to add*/ );

protected:
	static HANDLE	OpenFileToAdd			(LPCTSTR lpszFilePath);
	static void		PreWriteFileSection		(void* lpBuffer, UINT nSize);
	
protected:
	UINT				_fDataSectionSz;

public:
	OpenFileToAdd_proc			m_procOpenFileToAdd;
	PreWriteFileSection_proc	m_procPreWriteFileSection;
};


