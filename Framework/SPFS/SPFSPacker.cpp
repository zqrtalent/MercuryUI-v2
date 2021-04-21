// SqlStringLoader.cpp : implementation file
//

#include "stdafx.h"
#include "SPFSPacker.h"
#include "io.h"

#pragma warning(disable:4311)
#pragma warning(disable:4312)

// SPFSPacker
SPFSPacker::SPFSPacker() :
_fDataSectionSz(0x400) // 1KB by Default.
	{
	m_procOpenFileToAdd			= (OpenFileToAdd_proc)&OpenFileToAdd;
	m_procPreWriteFileSection	= (PreWriteFileSection_proc)PreWriteFileSection;
	}

SPFSPacker::~SPFSPacker(){}

void 
SPFSPacker::SetFileSecSz(UINT fDataSectionSz)
	{
	_fDataSectionSz = fDataSectionSz;
	}

HANDLE
SPFSPacker::OpenFileToAdd(LPCTSTR lpszFilePath)
	{
	return ::CreateFile(lpszFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	}

void
SPFSPacker::PreWriteFileSection(void* lpBuffer, UINT nSize)
	{
	}

void 
SPFSPacker::AddFolder(LPCSTR lpszPackFile,LPCSTR lpszDir,LPCSTR lpszFMask)
	{
	// initialize pack file header.
	PACK_HEADER header;
	ZeroMemory(&header,sizeof(header));
	header.signature    = 0x6B636170;        // _T("pack")  
	header.fDataSecSz   = _fDataSectionSz;   // file data section size is 1 KB.
	header.fssHeader.offsetFSS = sizeof(PACK_HEADER); // set starting offset of file search section.
	DWORD  foArraySz    = 0x400;             // size of allocation file offsets array in bytes.
	DWORD* pFOArray     = (DWORD*)malloc(foArraySz); // pointer to the file offsets array.
	DWORD  fnoArraySz   = 0x400;             // size of allocation file name offsets array in bytes.
	DWORD* pFNOArray    = (DWORD*)malloc(fnoArraySz);// pointer to the file name offsets array.
	DWORD  fnArraySz    = 0x400;             // size of allocation file names array in bytes.
	DWORD  fnArrUsed    = 0L;                // size of used memory from file names array.
	void * pFNArray     = malloc(fnArraySz); // pointer to the file names(with extention) array.
	DWORD  fileCt       = 0L;                // count of packed files.
	DWORD  written      = 0L;                // written bytes count.
	DWORD  readSz       = 0L;                // readed bytes count.
	void*  lpSectionMem = malloc(header.fDataSecSz); // allocate heap memory for file data section.
	DWORD  packOffset   = 0L;                // current offset of pack file.
	size_t dirLength    = strlen(lpszDir);   // save directory length size.
	
	// store pointer to the file data section header.
	PACK_FS_HEADER* pFSHeader = (PACK_FS_HEADER*)lpSectionMem;
	// initialize find data structure.
	WIN32_FIND_DATA fData;
	ZeroMemory(&fData,sizeof(WIN32_FIND_DATA));
	sprintf(fData.cFileName,"%s\\%s",lpszDir,lpszFMask);

	char szFilePath[_MAX_PATH];
	sprintf(szFilePath,"%s\\",lpszDir);
	
	HANDLE hPackFile    = NULL;
	HANDLE hFind        = FindFirstFile(fData.cFileName,&fData);
	HANDLE hFile        = NULL;

	if( hFind != INVALID_HANDLE_VALUE )
		{
		// create new pack file.
		hPackFile     = CreateFile(lpszPackFile,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,0,NULL);
		if( hPackFile == (HANDLE)-1 )    
			throw new SPFSException(_T("can't create pack file !"),SPFS_FILE_CANT_CREATE);
		// read header for pack file.
		if( !WritePackHeader(hPackFile,&header) )
			throw new SPFSException(_T("can't read pack file header !"),SPFS_FILE_CANT_READ);
		packOffset  += sizeof(PACK_HEADER);
		}
	else
		throw new SPFSException(_T("can't find first file !"),SPFS_FILE_NOT_FOUND);

	while( hFind != (HANDLE)-1 )
		{
		// initialize full path of founded file. {{
		szFilePath[dirLength+1]     = 0x00;
		strcat(szFilePath,fData.cFileName);
		// }}
		
		// open founded file. [ZqrTalent]
		hFile	= m_procOpenFileToAdd(szFilePath);
		// old one.
		// hFile = CreateFile(szFilePath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
		if( hFile != (HANDLE)-1 )
			{
			// add file into pack file.
			DWORD offset = AddFile(hPackFile, packOffset, hFile, &header, lpSectionMem, written, m_procPreWriteFileSection);
			// add file into FSS.
			if( offset )
				AddFileToFSS(fData.cFileName,offset,pFNArray,fnArraySz,fnArrUsed,
					         pFOArray,foArraySz,pFNOArray,fnoArraySz,fileCt);
			else
				{
				CloseHandle(hFile);
				throw new SPFSException(_T("can't add file to the pack file !"),SPFS_FILE_CANT_WRITE);
				}
			// close file handle.
			CloseHandle(hFile);
			}

		//   find next file.
		if( !FindNextFile(hFind,&fData) ) 
			{
			// file name offsets array size & file offsets array size.
			UINT arrSize                = fileCt*sizeof(DWORD);
			// fill pack file header. {{
			header.fssHeader.start      = 0x68637273;// _T("srch")
			// current offset + file name array size + file offsets array size + name offsets array size.
			header.packSz               = header.fssHeader.offsetFSS + fnArrUsed + 2*arrSize;
			// set FSS section used bytes size.
			header.fssHeader.secUsedSz  = fnArrUsed + 2*arrSize;
			// set FSS section full bytes size.
			if( header.fssHeader.sectionSz < header.fssHeader.secUsedSz )
				header.fssHeader.sectionSz = header.fssHeader.secUsedSz;
			// set file names array size.
			header.fssHeader.fnArraySz  = fnArrUsed;
			// set file offsets array size.
			header.fssHeader.foArraySz  = arrSize;
			// set file name offsets array size.
			header.fssHeader.fnoArraySz = arrSize;
			// set offset of file names array.
			header.fssHeader.fnArray    = header.fssHeader.offsetFSS;
			// set offset of file name offsets array.
			header.fssHeader.fnoArray   = header.fssHeader.offsetFSS + fnArrUsed;
			// set offset of file offsets array.
			header.fssHeader.foArray    = header.fssHeader.offsetFSS + fnArrUsed + arrSize;
			// }}

			//   write header.
			if( !WritePackHeader(hPackFile,&header) )   
				throw new SPFSException(_T("can't write pack file header!"),SPFS_FILE_CANT_WRITE);
			//   write FSS.
			if( !WritePackFSS(hPackFile,&header,pFNArray,fnArrUsed,pFOArray,pFNOArray,fileCt) )
				throw new SPFSException(_T("can't write FSS(file search section) of pack file !"),SPFS_FILE_CANT_WRITE);
			FindClose(hFind);
			break;
			}
		}

	// close handle of pack file.
	if( hPackFile )        CloseHandle(hPackFile);
	if( pFSHeader )        free(pFSHeader);
	if( pFOArray  )        free(pFOArray);
	if( pFNOArray )        free(pFNOArray);
	if( pFNArray  )        free(pFNArray);
	}