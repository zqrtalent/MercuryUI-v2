#pragma once
#include "packheader.h"

//    SPFS - Simple Packed File System
class SPFS
{
public:
	SPFS ();
	~SPFS();
	void Open            (LPCSTR lpszPackFile       /*in *//*pack file path.*/);
	void Close           ();
	void LoadFileAsString(LPCSTR  spfsFileName      /*in *//*name of file in pack*/,
					      string& sResultStr        /*in *//*string object to store file content.*/);
	void AddFolder       (LPCSTR  lpszDir           /*in *//*directory of files to add.*/,
		                  LPCSTR  lpszFMask         /*in *//*mask of file to add.*/ );
	void ExtractToFile   (LPCTSTR spfsFileName      /*in *//*contained file name.*/,
		                  LPCTSTR targetPathFileName/*in *//*extract file path.*/ );
	void ExtractAll      (LPCTSTR lpszDir           /*in *//*directory to extract files.*/);
	void RemoveFile      (LPCTSTR spfsFileName      /*in *//*contained file name with extention.*/);
	bool GetIndexFileName(DWORD index,string& sFName);
	UINT GetFileCount    ();
	bool IsOpen          ();

public:

	OpenFileToAdd_proc			m_procOpenFileToAdd;
	PreWriteFileSection_proc	m_procPreWriteFileSection;
	PostReadFileSection_proc	m_procPostReadFileSection;

protected:
	// retrives file data from pack file.
	bool ReadFileData    (DWORD  offset              /*in *//*offset of founded file.*/,
						  DWORD  size                /*in *//*size of founded file.*/,
						  void*  lpBuffer            /*out*//*pointer to the memory to write file data*/,
						  DWORD  numberOfBytesToRead /*in *//*number of bytes to read.*/,
						  DWORD& numberOfReadBytes   /*out*//*size of written data.*/);
	
	// finds file in pack file & retrives founded file info.
	bool FindFile        (LPCSTR spfsFileName        /*in *//*contained file name with extention.*/,
		                  DWORD& offset              /*out*//*offset of founded file.*/, 
					      DWORD& size                /*out*//*size of founded file in bytes.*/,
						  PACK_FS_HEADER* pFSHeader = NULL  /*out*//*stores founded file first section header.*/ );
	// removes file from file search section.
	bool RemoveFromFSS   (LPCSTR spfsFileName        /*in *//*contained file name with extention.*/);
	
	static HANDLE	OpenFileToAdd			(LPCTSTR lpszFilePath);
	static void		PreWriteFileSection		(void* lpBuffer, UINT nSize);
	static void		PostReadFileSection		(void* lpBuffer, UINT nSize);

private:
	HANDLE                _hPackFile;                // handle  to the pack file.
	PACK_HEADER*          _pHeader;                  // pointer to the pack file header.
	void*                 _pFNArray;                 // pointer to the file name array.                      
	DWORD*                _pFNOArray;                // pointer to the file name offsets array.
	DWORD*                _pFOArray;                 // pointer to the file starting offsets array.
	DWORD                 _fnArrAlloc;               // indicates size of allocated memory for file names array.
	DWORD                 _fnArrUsed;                // indicates size of used memory for file names array.
	DWORD                 _foArrAlloc;               // indicates size of allocated memory for file offsets array.
	DWORD                 _fnoArrAlloc;              // indicates size of allocated memory for file name offsets array.
};

__inline bool SPFS::IsOpen(){ return _hPackFile ? true : false; };
