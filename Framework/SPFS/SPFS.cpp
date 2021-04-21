// SPFS.cpp : implementation file
//

#include "stdafx.h"
#include "SPFS.h"
#include <io.h>

#pragma warning(disable:4311)
#pragma warning(disable:4312)

// SPFS - Simple Packed File System
SPFS::SPFS()
	{
	_hPackFile   = NULL;
	_pHeader     = NULL;
	_pFNArray    = NULL;
	_pFNOArray   = NULL;
	_pFOArray    = NULL;
	_fnArrAlloc  = 0L;
	_fnArrUsed   = 0L;
	_fnoArrAlloc = 0L;
	_foArrAlloc  = 0L;

	m_procOpenFileToAdd			= (OpenFileToAdd_proc)&OpenFileToAdd;
	m_procPreWriteFileSection	= (PreWriteFileSection_proc)PreWriteFileSection;
	m_procPostReadFileSection	= (PostReadFileSection_proc)PostReadFileSection;
	}

SPFS::~SPFS()
	{ 
	if( IsOpen() ) Close(); 
	}

HANDLE
SPFS::OpenFileToAdd(LPCTSTR lpszFilePath)
	{
	return ::CreateFile(lpszFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	}

void
SPFS::PreWriteFileSection(void* lpBuffer, UINT nSize)
	{
	}

void
SPFS::PostReadFileSection(void* lpBuffer, UINT nSize)
	{
	}

void 
SPFS::Open(LPCSTR lpszPackFile)
	{
	if( IsOpen() )           
		throw new SPFSException(_T("other pack file is allready opened !"),SPFS_FILE_ALLREADY_OPENED);
	//  check for file existing.
	if( _access(lpszPackFile,0) ) 
		throw new SPFSException(_T("pack file doesn't exists !"),SPFS_FILE_NOT_FOUND);

	// variable for read count.
	DWORD  readCt    = 0L;
	DWORD  errorCode = 0L;
	string errorStr  = _T("");
/*
	// open pack file.
	HANDLE hPackFile = CreateFile(lpszPackFile,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);
	if( hPackFile   == INVALID_HANDLE_VALUE )
		{
		errorCode = SPFS_FILE_CANT_OPEN;
		errorStr  = _T("can't open file !");
		goto throw_error;
		}
*/
	// open pack file.
	HANDLE hPackFile = CreateFile(lpszPackFile,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if( hPackFile   == INVALID_HANDLE_VALUE )
		{
		errorCode = SPFS_FILE_CANT_OPEN;
		errorStr  = _T("can't open file !");
		goto throw_error;
		}
	// allocate memory for pack header.
	_pHeader = (PACK_HEADER*)malloc(sizeof(PACK_HEADER));
	// check for pack file validation.
	if( !IsValidPackFile(hPackFile,_pHeader) )
		{
		errorCode = SPFS_FILE_UNKNOWN_FORMAT;
		errorStr  = _T("unknown file format !");
		goto throw_error;
		}
	// set file pointer to the FSS offset.
	if( SetFilePointer(hPackFile,_pHeader->fssHeader.offsetFSS,NULL,FILE_CURRENT) != _pHeader->fssHeader.offsetFSS )
		{
		errorCode = SPFS_CANT_SET_FILE_POINTER;
		errorStr  = _T("can't set file pointer to the beginning of FSS !");
		goto throw_error;
		}

	// allocate memory for file name array.
	_pFNArray  = malloc(_pHeader->fssHeader.fnArraySz);
	// allocate memory for file name offsets array.
	_pFNOArray = (DWORD*)malloc(_pHeader->fssHeader.fnoArraySz);
	// allocate memory for file starting offsets array.
	_pFOArray  = (DWORD*)malloc(_pHeader->fssHeader.foArraySz);

	// read file name array from pack file.
	if( !ReadFile(hPackFile,_pFNArray,_pHeader->fssHeader.fnArraySz,&readCt,NULL)  ||
	    _pHeader->fssHeader.fnArraySz != readCt ) 
		{
		errorCode = SPFS_FILE_CANT_READ;
		errorStr  = _T("can't read file name array from pack file !");
		goto throw_error;
		}
	// read file name offsets array from pack file.
	if( !ReadFile(hPackFile,_pFNOArray,_pHeader->fssHeader.fnoArraySz,&readCt,NULL) ||
	    _pHeader->fssHeader.fnoArraySz != readCt ) 
		{
		errorCode = SPFS_FILE_CANT_READ;
		errorStr  = _T("can't read file name offsets array from pack file !");
		goto throw_error;
		}
	// load file name array from pack file.
	if( !ReadFile(hPackFile,_pFOArray,_pHeader->fssHeader.foArraySz,&readCt,NULL)   ||
	    _pHeader->fssHeader.foArraySz != readCt ) 
		{
		errorCode = SPFS_FILE_CANT_READ;
		errorStr  = _T("can't read from pack file !");
		goto throw_error;
		}
	// set file pointer to the beginning of file.
	if( SetFilePointer(hPackFile,0L,NULL,FILE_BEGIN) == (DWORD)-1 )
		{
		errorCode = SPFS_CANT_SET_FILE_POINTER;
		errorStr  = _T("can't set pack file pointer to the beginning of file !");
		goto throw_error;
		}
	// ------------------------------------------
	_hPackFile    = hPackFile;
	_fnArrAlloc   = _pHeader->fssHeader.fnArraySz;
	_fnArrUsed    = _pHeader->fssHeader.fnArraySz;
	_fnoArrAlloc  = _pHeader->fssHeader.fnoArraySz;
	_foArrAlloc   = _pHeader->fssHeader.foArraySz;
	// ------------------------------------------
	return;

throw_error:
	Close();
	throw new SPFSException(errorStr,errorCode);
	}

void 
SPFS::Close()
	{
	//---------------------------------------
	if( _hPackFile ) CloseHandle(_hPackFile);
	_hPackFile   = NULL;
	if( _pHeader   ) free(_pHeader);
	_pHeader     = NULL;
	if( _pFOArray  ) free(_pFOArray);
	_pFOArray    = NULL;
	if( _pFNArray  ) free(_pFNArray);
	_pFNArray    = NULL;
	if( _pFNOArray ) free(_pFNOArray);
	_pFNOArray   = NULL;
	//--------------------
	_fnArrAlloc  = 0L;
	_fnArrUsed   = 0L;
	_fnoArrAlloc = 0L;
	_foArrAlloc  = 0L;
	//--------------------
	}

void 
SPFS::AddFolder(LPCSTR lpszDir,LPCSTR lpszFMask)
{
	if( !IsOpen() )
		throw new SPFSException(_T("pack file isn't opened !"),SPFS_FILE_IS_NOT_OPENED);

	PACK_FSS_HEADER *fss = &_pHeader->fssHeader;
	DWORD  fileCt        = _pHeader->fileCt;               // count of packed files.
	DWORD  written       = 0L;                             // written bytes count.
	DWORD  readSz        = 0L;                             // readed bytes count.
	void*  lpSectionMem  = malloc(_pHeader->fDataSecSz);   // allocate heap memory for file data section.
	DWORD  packOffset    = 0L;                             // offset of pack file handle.
	size_t dirLength     = strlen(lpszDir);                // save directory length size.
	
	// store pointer to the file data section header.
	PACK_FS_HEADER* pFSHeader = (PACK_FS_HEADER*)lpSectionMem;
	// initialize find data structure.
	WIN32_FIND_DATA fData;
	ZeroMemory(&fData,sizeof(WIN32_FIND_DATA));
	sprintf(fData.cFileName,"%s\\%s",lpszDir,lpszFMask);

	char szFilePath[_MAX_PATH];
	sprintf(szFilePath,"%s\\",lpszDir);
	HANDLE hFind        = FindFirstFile(fData.cFileName,&fData);
	HANDLE hFile        = NULL;

	// set file pointer to the end of file data.
	if( hFind == (HANDLE)-1 || SetFilePointer(_hPackFile,0L,0L,FILE_BEGIN) == (DWORD)-1 )
		throw new SPFSException(_T("can't find first SQL file !"),SPFS_FILE_NOT_FOUND);

	while( hFind != (HANDLE)-1 )
		{
		// initialize full path of founded file.
		szFilePath[dirLength+1]     = 0x00;
		strcat(szFilePath,fData.cFileName);

		// open founded file.
		if( m_procOpenFileToAdd )
			hFile = (m_procOpenFileToAdd)(szFilePath);
		else
			hFile = CreateFile(szFilePath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);

		if( hFile != (HANDLE)-1 )
			{
			// add file into pack file.
			DWORD offset = AddFile(_hPackFile, packOffset, hFile, _pHeader, lpSectionMem, written, m_procPreWriteFileSection);
			// add file into FSS.
			if( offset )	
				AddFileToFSS(fData.cFileName,offset,_pFNArray,_fnArrAlloc,_fnArrUsed,
							 _pFOArray,_foArrAlloc,_pFNOArray,_fnoArrAlloc,fileCt);
			else
				{
				CloseHandle(hFile);
				throw new SPFSException(_T("can't add file to the pack file !"),SPFS_FILE_CANT_WRITE);
				}
			// close file handle.
			CloseHandle(hFile);
			}
		
		// find next file.
		if( !FindNextFile(hFind,&fData) ) 
			{
			// file name offsets array size & file offsets array size.
			UINT arrSize                   = fileCt*sizeof(DWORD);
			// fill pack file header. {{
			_pHeader->fssHeader.start      = 0x68637273;// _T("srch")
			// current offset + file name array size + file offsets array size + name offsets array size.
			_pHeader->packSz               = _pHeader->fssHeader.offsetFSS + _fnArrUsed + 2*arrSize;
            // set FSS section used bytes size.
			_pHeader->fssHeader.secUsedSz  = _fnArrUsed + 2*arrSize;
			// set FSS section full bytes size.
			if( _pHeader->fssHeader.sectionSz < _pHeader->fssHeader.secUsedSz )
				_pHeader->fssHeader.sectionSz = _pHeader->fssHeader.secUsedSz;
			// set file names array size.
			_pHeader->fssHeader.fnArraySz  = _fnArrUsed;
			// set file offsets array size.
			_pHeader->fssHeader.foArraySz  = arrSize;
			// set file name offsets array size.
			_pHeader->fssHeader.fnoArraySz = arrSize;
			// set offset of file names array.
			_pHeader->fssHeader.fnArray    = _pHeader->fssHeader.offsetFSS;
			// set offset of file name offsets array.
			_pHeader->fssHeader.fnoArray   = _pHeader->fssHeader.offsetFSS + _fnArrUsed;
			// set offset of file offsets array.
			_pHeader->fssHeader.foArray    = _pHeader->fssHeader.offsetFSS + _fnArrUsed + arrSize;
			// }}

			//   write pack header.
			if( !WritePackHeader(_hPackFile,_pHeader) )   
				throw new SPFSException(_T("can't write pack file header!"),SPFS_FILE_CANT_WRITE);
			//   write pack FSS.
			if( !WritePackFSS(_hPackFile,_pHeader,_pFNArray,_fnArrUsed,_pFOArray,_pFNOArray,fileCt) )
				throw new SPFSException(_T("can't write FSS(file search section) of pack file !"),SPFS_FILE_CANT_WRITE);
			FindClose(hFind);
			break;
			}
		}
}

void 
SPFS::RemoveFile(LPCTSTR spfsFileName)
	{
	if( !IsOpen() )
		throw new SPFSException(_T("pack file isn't opened !"),SPFS_FILE_IS_NOT_OPENED);
	
	DWORD offset  = 0;                // offset of founded file.
	DWORD stOffset= 0;                // starting offset of founded file.
	DWORD size    = 0;                // size of file.
	DWORD readCt  = 0;                // size of readed bytes.
	DWORD written = 0;                // size of written bytes.
	DWORD fileCt  = _pHeader->fileCt; // count of files in pack file.

	PACK_FS_HEADER header;
	// find file in pack file.
	if( !FindFile(spfsFileName,offset,size,&header) ) 
		throw new SPFSException(_T("not found file in pack !"),SPFS_FILE_IS_NOT_OPENED);
	// save starting offset of founded file.
	stOffset      = offset;
	// this loop retrives last offset of curent file.
	// ----------------------------------------------
	while( header.offsetSecNext )
		{
		if( SetFilePointer(_hPackFile,header.offsetSecNext-(offset+sizeof(PACK_FS_HEADER)),NULL,FILE_CURRENT) == (DWORD)-1 )
			throw new SPFSException(_T("can't set file pointer to the beginning of file section !"),SPFS_CANT_SET_FILE_POINTER);
		//---------------------------------------------------------------------------
		offset = header.offsetSecNext;
		//---------------------------------------------------------------------------
		if( !ReadFile(_hPackFile,&header,sizeof(PACK_FS_HEADER),&readCt,NULL) ||
			readCt != sizeof(PACK_FS_HEADER) )
			throw new SPFSException(_T("can't read file data !"),SPFS_FILE_CANT_READ);
		//---------------------------------------------------------------------------
		if( header.start != 0x61746164   )// 0x61746164 = _T("data") 
			throw new SPFSException(_T("file section is not valid !"),SPFS_FILE_SECTION_IS_NOT_VALID);
		}
	// ----------------------------------------------
	// remove file from FSS.
	if( !RemoveFromFSS(spfsFileName) )
		throw new SPFSException(_T("can't remove file from FSS(file search section) !"),SPFS_CANT_REMOVE_FILE);

	// set file pointer to the beginning of file section.
	if( SetFilePointer(_hPackFile,-sizeof(PACK_FS_HEADER),NULL,FILE_CURRENT) == (DWORD)-1 )
		throw new SPFSException(_T("can't set file pointer to the beginning of file section !"),SPFS_CANT_SET_FILE_POINTER);
	
	// write new header for file section.
	header.offsetSecNext   = _pHeader->ffDataOffset;
	if( !WriteFile(_hPackFile,&header,sizeof(PACK_FS_HEADER),&written,NULL) || written !=sizeof(PACK_FS_HEADER) )
		throw new SPFSException(_T("can't write file data section header!"),SPFS_FILE_CANT_WRITE);

	// write pack header.
	_pHeader->ffDataOffset = stOffset;
	if( !WritePackHeader(_hPackFile,_pHeader) )   
		throw new SPFSException(_T("can't write pack file header!"),SPFS_FILE_CANT_WRITE);

	//   write pack FSS.
	if( !WritePackFSS(_hPackFile,_pHeader,_pFNArray,_fnArrUsed,_pFOArray,_pFNOArray,_pHeader->fileCt) )
		throw new SPFSException(_T("can't write FSS(file search section) of pack file !"),SPFS_FILE_CANT_WRITE);
	}

void 
SPFS::LoadFileAsString(LPCSTR spfsFileName,string& sResultStr)
	{
	if( !IsOpen() )
		throw new SPFSException(_T("pack file isn't opened !"),SPFS_FILE_IS_NOT_OPENED);
	
	DWORD offset  = 0; // offset of file.
	DWORD size    = 0; // size of file.
	DWORD written = 0; // size of written bytes.

	// find file in pack file.
	if( !FindFile(spfsFileName,offset,size) ) 
		throw new SPFSException(_T("not found file in pack !"),SPFS_FILE_IS_NOT_OPENED);

	char* lpBuff   = (char*)malloc(size+1);
	if( ReadFileData(offset,size,lpBuff,size,written) && written == size )
		{
		lpBuff[size] = 0x00;    
		sResultStr     = lpBuff;
		}
	else
		throw new SPFSException(_T("can't read file data !"),SPFS_FILE_CANT_READ);
	free(lpBuff);
	}

void 
SPFS::ExtractToFile(LPCTSTR spfsFileName,LPCTSTR targetPathFileName)
	{
	// find file in pack file.
	// ----------------------------------
	DWORD offset  = 0; // offset of file.
	DWORD size    = 0; // size of file.
	DWORD written = 0; // size of written bytes.
	if( !FindFile(spfsFileName,offset,size) ) 
		throw new SPFSException(_T("not found file in pack !"),SPFS_FILE_IS_NOT_OPENED);

	// read founded file from pack.
	//-----------------------------
	char* lpBuff   = (char*)malloc(size);
	if( !ReadFileData(offset,size,lpBuff,size,written) && written == size )
		throw new SPFSException(_T("can't read file data !"),SPFS_FILE_CANT_READ);
	//-----------------------------

	// check for file existing & create current file.
	// ----------------------------------------------
	if( !_access(targetPathFileName,0) )
		throw new SPFSException(_T("that file allready exists!"),SPFS_FILE_ALLREADY_EXISTS);
	// create file to write into.
	HANDLE hFile = CreateFile(targetPathFileName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_NEW,0,NULL);
	if( hFile == (HANDLE)-1 )
		throw new SPFSException(_T("can't create file !"),SPFS_FILE_CANT_CREATE);
	// ----------------------------------------------

	// write contained file.
	// -----------------------------------------------------------------
	if( !WriteFile(hFile,lpBuff,size,&written,NULL) || written != size ) 
		{
		free(lpBuff);
		CloseHandle(hFile);
		throw new SPFSException(_T("can't write contained file !"),SPFS_FILE_CANT_WRITE);
		}
	free(lpBuff);
	CloseHandle(hFile);
	// -----------------------------------------------------------------
	}

void 
SPFS::ExtractAll(LPCTSTR lpszDirName)
	{
	if( !IsOpen() )
		throw new SPFSException(_T("pack file isn't opened !"),SPFS_FILE_IS_NOT_OPENED);
	
	int    index     = 0;
	LPCSTR lpszFName = NULL;
	size_t dir_len   = strlen(lpszDirName);
	char   szFPath[MAX_PATH];
	sprintf(szFPath,"%s\\",lpszDirName);
	
	// loop to extract each file from pack.
	// ------------------------------------
	while( index < _pHeader->fileCt )
		{
		// initialize create file path.
		// ----------------------------
		lpszFName = (LPCSTR)((int)_pFNArray+_pFNOArray[index]);
		szFPath[dir_len+1] = 0x00;
		strcat(szFPath,lpszFName);
		// ----------------------------
		ExtractToFile(lpszFName,szFPath);
		index ++;
		}
	// ------------------------------------
	}

bool 
SPFS::GetIndexFileName(DWORD index,string& sFName)
	{
	if( IsOpen() )
		{
		if( index  < _pHeader->fileCt )
			{
			sFName = (LPTSTR)((int)_pFNArray+_pFNOArray[index]);
			return true;
			}
		}
	return false;
	}

UINT  
SPFS::GetFileCount()
	{
	if( IsOpen() )
		return _pHeader->fileCt;
	else
		return 0;
	}

bool 
SPFS::ReadFileData(DWORD offset,DWORD size,void* lpBuffer,DWORD numberOfBytesToRead,DWORD& lpNumberOfBytesRead)
	{
	DWORD readCt        = 0; // count of readed bytes.
	lpNumberOfBytesRead = 0; // written size is zero.

	// move file pointer in founded file offset.
	if( SetFilePointer(_hPackFile,0L,NULL,FILE_BEGIN)       != 0 ||
		SetFilePointer(_hPackFile,offset,NULL,FILE_CURRENT) != offset  )
		return false;
	
	// check if file size is less then number of bytes to read.
	if( numberOfBytesToRead > size ) return false;
	// write founded file data into memory. {{
	// if file data section size minus file data header size is great or equal then number of bytes to read.
	if( numberOfBytesToRead <= _pHeader->fDataSecSz-sizeof(PACK_FS_HEADER) )
		{
		if( SetFilePointer(_hPackFile,sizeof(PACK_FS_HEADER),NULL,FILE_CURRENT) != offset+sizeof(PACK_FS_HEADER) )
			return false;

		if( !ReadFile(_hPackFile,lpBuffer,numberOfBytesToRead,&readCt,NULL) ||
					readCt != numberOfBytesToRead )               
			return false;

		// Post read file section.
		if( m_procPostReadFileSection )
			(m_procPostReadFileSection)(lpBuffer, readCt);
		// #######################

		lpNumberOfBytesRead = readCt;
		}
	else
		{
		int         ReadMin = 0; 
		PACK_FS_HEADER fsHeader;

		while( numberOfBytesToRead  )
			{
			ReadMin     = min(numberOfBytesToRead,_pHeader->fDataSecSz-sizeof(PACK_FS_HEADER));
			// read file data section header.
			if( !ReadFile(_hPackFile,&fsHeader,sizeof(PACK_FS_HEADER),&readCt,NULL) ||
			    readCt != sizeof(PACK_FS_HEADER) ) return false;
			// check for file data section.
			if( fsHeader.start != 0x61746164 )     return false;/*"data"*/
			// read file data section.
			if( !ReadFile(_hPackFile,lpBuffer,ReadMin,&readCt,NULL) ||
                readCt != ReadMin )                return false;

			// Post read file section.
			if( m_procPostReadFileSection )
				(m_procPostReadFileSection)(lpBuffer, readCt);
			// #######################

			lpNumberOfBytesRead += readCt;
			lpBuffer             =  (void*)((int)lpBuffer + readCt);
			numberOfBytesToRead -= readCt;
			// change pack file offset.
			offset              += _pHeader->fDataSecSz;

			// move file pointer to the next file data section.
			if( numberOfBytesToRead )
				{
				if( SetFilePointer(_hPackFile,fsHeader.offsetSecNext-offset,NULL,FILE_CURRENT) == (DWORD)-1 )
					return false;
				offset           = fsHeader.offsetSecNext;
				}
			}
		}
		return true;
	}

bool 
SPFS::FindFile(LPCSTR lpszFName,DWORD& offset,DWORD& size,PACK_FS_HEADER* pFSHeader)
	{
	// file name offsets & file offsets array is sort by ascending.		
	int index     = FindFileNamePlaceAsc(lpszFName,_pFNArray,_pHeader->fileCt,_pFNOArray,_pFOArray);
	if( index    == _pHeader->fileCt )   return false;
	LPCSTR lpTemp = (LPTSTR)((int)_pFNArray+_pFNOArray[index]);
	if( strcmp(lpTemp,lpszFName) )       return false;
	
	offset = _pFOArray[index];
	if( SetFilePointer(_hPackFile,0L,NULL,FILE_BEGIN) == (DWORD)-1    )
        return false;
	if( SetFilePointer(_hPackFile,offset,NULL,FILE_CURRENT) != offset )
		return false;

	DWORD          readCt = 0;
	PACK_FS_HEADER packHeader;
	ReadFile(_hPackFile,&packHeader,sizeof(PACK_FS_HEADER),&readCt,NULL);
	if( readCt != sizeof(PACK_FS_HEADER) )return false;
	if( packHeader.start != 0x61746164   )return false;  // 0x61746164 = _T("data")
	size  = packHeader.usedSz;
	// store file first section header structure.
	if( pFSHeader )
		memcpy(pFSHeader,&packHeader,sizeof(PACK_FS_HEADER));
	return true;
	}

bool 
SPFS::RemoveFromFSS(LPCSTR spfsFileName)
	{
	size_t spfsLen= strlen(spfsFileName)+1;
	// find file in FSS.		
	int index     = FindFileNamePlaceAsc(spfsFileName,_pFNArray,_pHeader->fileCt,_pFNOArray,_pFOArray);
	if( index    == _pHeader->fileCt )   return false;
	LPCSTR lpTemp = (LPTSTR)((int)_pFNArray+_pFNOArray[index]);
	if( strcmp(lpTemp,spfsFileName) )    return false;

	// decrease file count in pack file.
	_pHeader->fileCt --;
	// remove file name from file name array.
	// --------------------------------------
	memcpy((void*)((long)_pFNArray+_pFNOArray[index]),
		   (void*)((long)_pFNArray+_pFNOArray[index]+spfsLen),_fnArrUsed-_pFNOArray[index]-spfsLen);
	// zero not neaded space.
	memset((void*)((long)_pFNArray+_fnArrUsed-spfsLen),0,spfsLen);
	// change used memory size from file names array.
	_fnArrUsed                    -= spfsLen;
	// set used bytes size in FSS.
	_pHeader->fssHeader.secUsedSz -= (spfsLen + sizeof(DWORD)*2);
	// set file name array size.
	_pHeader->fssHeader.fnArraySz  = _fnArrUsed;
	// set file name offsets array size.
	_pHeader->fssHeader.fnoArraySz = sizeof(DWORD)*_pHeader->fileCt;
	// set file offsets array size.
	_pHeader->fssHeader.foArraySz  = sizeof(DWORD)*_pHeader->fileCt;
	// set starting offset of file name offsets array.
	_pHeader->fssHeader.fnoArray   = _pHeader->fssHeader.fnArray  + _fnArrUsed;
	// set starting offset of file offsets array.
	_pHeader->fssHeader.foArray    = _pHeader->fssHeader.fnoArray + _pHeader->fileCt*sizeof(DWORD);
	// --------------------------------------

	// remove file offset from file offsets array.
	// --------------------------------------
	memcpy((void*)((long)_pFOArray+index*sizeof(DWORD)),
		   (void*)((long)_pFOArray+(index+1)*sizeof(DWORD)),_foArrAlloc - (index+1)*sizeof(DWORD));
	//free(_pFNArray);
	// --------------------------------------

	// remove file name offset from file name offsets array.
	// --------------------------------------
	// save file name offset.
	DWORD fnOffset = _pFNOArray[index];
	memcpy((void*)((long)_pFNOArray+index*sizeof(DWORD)),
		   (void*)((long)_pFNOArray+(index+1)*sizeof(DWORD)),_fnoArrAlloc - (index+1)*sizeof(DWORD));
	// change file name offsets array.
	for( int i=0; i<_pHeader->fileCt; i++ )
		{
		if( _pFNOArray[i]  > fnOffset )
			_pFNOArray[i] -= spfsLen;
		}
	// --------------------------------------
	return true;
	}




