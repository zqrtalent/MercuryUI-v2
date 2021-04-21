#ifndef __PACK_HEADER__
#define __PACK_HEADER__

#include <string> 
using namespace std;
#include <stdio.h>

#pragma warning(disable:4311)
#pragma warning(disable:4312)

typedef HANDLE	(__cdecl* OpenFileToAdd_proc)		(LPCTSTR lpszFilePath);
typedef void	(__cdecl* PreWriteFileSection_proc)	(void* lpBuffer, UINT nSize);
typedef void	(__cdecl* PostReadFileSection_proc)	(void* lpBuffer, UINT nSize);


// SPFS error codes.
#define SPFS_FILE_NOT_FOUND               1
#define SPFS_FILE_CANT_OPEN               2
#define SPFS_FILE_CANT_WRITE              3
#define SPFS_FILE_CANT_READ               4
#define SPFS_FILE_UNKNOWN_FORMAT          5
#define SPFS_FILE_ALLREADY_OPENED         6
#define SPFS_FILE_IS_NOT_OPENED           7
#define SPFS_CANT_SET_FILE_POINTER        8
#define SPFS_FILE_SECTION_IS_NOT_VALID    9
#define SPFS_FILE_CANT_READ_FILE_DATA     0xA
#define SPFS_FILE_CANT_CREATE             0xB
#define SPFS_FILE_ALLREADY_EXISTS         0xC
#define SPFS_CANT_REMOVE_FILE             0xD

// file search section header.
typedef struct PACK_FILE_SEARCH_SECTION_HEADER
{
	DWORD start;                  // special FSS header starting value must be a (0x68637273 == "srch").
	DWORD offsetFSS;              // starting offset, of content search section.
	DWORD sectionSz;              // full size, of file search section in bytes.
	DWORD secUsedSz;              // used bytes size, of file search section.
	DWORD fnArraySz;              // size of file name(with extention) array in bytes.
	DWORD fnoArraySz;             // size of file name offset array in bytes. 
	DWORD foArraySz;              // size of file offset array in bytes.
	DWORD fnArray;                // offset of file names(with extention) array.
	DWORD fnoArray;               // offset of file name offsets array(relative to the file search section address).
	DWORD foArray;                // offset of file start offsets array.

} PACK_FSS_HEADER;

// contined file header.
struct PACK_HEADER
{
	DWORD           signature;    // special signature for pack files must be a (0x6B636170 == "pack").
	DWORD           packSz;       // size of pack file in bytes.
	DWORD           fileCt;       // count of files in pack file.
	DWORD           fDataSecSz;   // The size of file data section, in bytes.
	DWORD           ffDataOffset; // The offset of first free data section.
	PACK_FSS_HEADER fssHeader;    // The FSS header structure.
};

// data section header of contained file.
typedef struct PACK_FILE_SECTION_HEADER
{
	DWORD start;                  // special content starting value must be a (0x61746164 == "data").
	DWORD usedSz;                 // offsetSecNext != NULL ? size of used bytes in this section : full size of current file.
	DWORD offsetSecNext;          // The offset to the next section of file data. 
} PACK_FS_HEADER;


// retrives insert index of current file name in file name offsets ascending list.
static int  FindFileNamePlaceAsc(LPCTSTR lpszFName   /*in *//*file name*/,
								 void*   pFNArray    /*in *//*pointer to the file name array.*/,
								 DWORD   nameCt      /*in *//*count of names in file names array.*/,
								 DWORD*  pFNOArray   /*in *//*pointer to the file name offsets array.*/,
								 DWORD*  pFOArray    /*in *//*pointer to the file offsets array.*/)
	{
	if( !nameCt )     return 0;
	int         nLast  = nameCt-1;
	int         nStart = 0;
	int         nRet;
	int         nMid;
	LPTSTR      lpListAsc;

	while( nLast > nStart+1 )
		{
		nMid     = nStart+(nLast-nStart)/2;
		lpListAsc= (LPTSTR)((int)pFNArray+pFNOArray[nMid]);
		nRet     = strcmp(lpListAsc,lpszFName);
		
		if( !nRet    ) return nMid;	
		if( nRet > 0 ) nLast  = nMid;
		if( nRet < 0 ) nStart = nMid;
		}
	
	lpListAsc = (LPTSTR)((int)pFNArray+pFNOArray[nStart]);
	nRet      = strcmp(lpListAsc,lpszFName);
	
	if( nRet >= 0 )    return nStart;
	else
		{
		lpListAsc = (LPTSTR)((int)pFNArray+pFNOArray[nLast]);
		nRet      = strcmp(lpListAsc,lpszFName);
	
		if( nRet < 0 ) return nLast+1;
		else           return nLast;
		}
	}

// check pack file validation.
static bool 
IsValidPackFile(HANDLE hPack        /*in *//*handle to the opened pack file.*/,
                PACK_HEADER* pHeader/*in *//*pointer of pack file header structure.*/ )
	{
	DWORD       readCt      = 0L;
	DWORD       fileSz      = ::GetFileSize(hPack,NULL);;
	PACK_HEADER header;
	
	// set file pointer to the beginning of file.
	if( SetFilePointer(hPack,0L,0L,FILE_BEGIN) == (DWORD)-1 )              return false;
	if( !ReadFile(hPack,&header,sizeof(PACK_HEADER),&readCt,NULL) )        return false;
	if( readCt != sizeof(PACK_HEADER) )                                    return false;
	if( header.signature            != 0x6B636170/*pack*/ )                return false;
	if( header.packSz               != fileSz )                            return false;
	if( header.packSz != header.fssHeader.offsetFSS + header.fssHeader.sectionSz ) return false;
	if( !header.fileCt || !header.fDataSecSz )                             return false;
	if( header.fssHeader.start      != 0x68637273/*srch*/ )                return false;
	if( header.fssHeader.offsetFSS  != fileSz-header.fssHeader.sectionSz ) return false;
	if( header.fssHeader.secUsedSz  != header.fssHeader.fnArraySz + 
		header.fssHeader.fnoArraySz + header.fssHeader.foArraySz )         return false;
	if( header.fssHeader.fnoArraySz != header.fssHeader.foArraySz )        return false;
	memcpy(pHeader,&header,sizeof(PACK_HEADER));
	// set file pointer to the beginning of file.
	if( SetFilePointer(hPack,0L,0L,FILE_BEGIN) == (DWORD)-1 )              return false;
	else                                                                   return true;
	}

// adds file into pack file & return starting offset of added file.
// return zero if not successfull added.
static DWORD 
AddFile(HANDLE       hPackFile              /*in *//*handle to the pack file.*/,
		DWORD&       packOffset             /*in *//*current offset of pack file handle.*/,
		HANDLE       hFile                  /*in *//*handle to the file to add.*/,
		PACK_HEADER* pHeader                /*in *//*pointer of pack file header structure.*/,
		void*        lpSectionMem           /*in *//*pointer to the section memory to initialize & write in pack file.*/,
		DWORD&       lpNumberOfBytesWritten /*out*//*size of writed bytes.*/,
		PreWriteFileSection_proc procPreWrite)
	{
	lpNumberOfBytesWritten =  0L;
	// retrive opened file size.
	DWORD fileSz = ::GetFileSize(hFile,NULL);
	// check for empty file.
	if( !fileSz ) return 0L;

	// size of readed bytes.
	DWORD readSz     = 0L; 
	// size of writed bytes.
	DWORD written    = 0L;
	// full size of readed bytes.
	DWORD fullReadSz = 0L;
	// indicates if found free file data section.
	bool  bFoundFreeSec = false;
	// starting offset of added file.
	DWORD fileOffset = !pHeader->ffDataOffset ? pHeader->fssHeader.offsetFSS : pHeader->ffDataOffset;
	// initialize file section header.
	PACK_FS_HEADER* pFSHeader = (PACK_FS_HEADER*)lpSectionMem;
	lpSectionMem              = (void*)(((int)pFSHeader) + sizeof(PACK_FS_HEADER));
	pFSHeader->start          = 0x61746164; // _T("data")
		
	do
		{
		if( pHeader->ffDataOffset )
			{  
			//  move pack file handle to the beginning of free data section.
			if( SetFilePointer(hPackFile,pHeader->ffDataOffset-packOffset,NULL,FILE_CURRENT) == (DWORD)-1 )
				return 0L;
			//  read free file section header.
			if( !ReadFile(hPackFile,lpSectionMem,sizeof(PACK_FS_HEADER),&readSz,NULL) || 
				readSz != sizeof(PACK_FS_HEADER) ) 
				return 0L;
			//  move the pack file pointer to the beggining of file data section.
			if( SetFilePointer(hPackFile,-1*((int)sizeof(PACK_FS_HEADER)),NULL,FILE_CURRENT) == (DWORD)-1 )
				return 0L;
			//  check free file section validation.
			if( ((PACK_FS_HEADER*)lpSectionMem)->start != 0x61746164 )// _T("data") 
				return 0L;
			// change pack file handle offset.
			packOffset            = pHeader->ffDataOffset;
			// save new free section offset.
			pHeader->ffDataOffset = ((PACK_FS_HEADER*)lpSectionMem)->offsetSecNext;
			// we were found free file data section.
			bFoundFreeSec         = true;
			}
		else
			{
			//  move pack file handle to the new data section position.
			if( SetFilePointer(hPackFile,pHeader->fssHeader.offsetFSS-packOffset,NULL,FILE_CURRENT) == (DWORD)-1 )
				return false;
			// change pack file handle offset.
			packOffset            = pHeader->fssHeader.offsetFSS;
			// we wern't found free file data section.
			bFoundFreeSec         = false;
			}

		// read file data.
		if( !ReadFile(hFile,lpSectionMem,pHeader->fDataSecSz-sizeof(PACK_FS_HEADER),&readSz,NULL) || !readSz )
			return 0L;
		// fill not used section memory with zero.
		memset((void*)((int)lpSectionMem+readSz),0,pHeader->fDataSecSz-sizeof(PACK_FS_HEADER)-readSz);
		// initialize file data section header.
		pFSHeader->offsetSecNext = fileSz - fullReadSz > readSz ? packOffset + pHeader->fDataSecSz : 0L;
		pFSHeader->usedSz        = pFSHeader->offsetSecNext ? fileSz : readSz;
		// pre write file data section.
		if( procPreWrite )
			(procPreWrite)(lpSectionMem, pFSHeader->offsetSecNext ? pHeader->fDataSecSz - sizeof(PACK_FS_HEADER) : pFSHeader->usedSz);
		// write file data section.
		if( !WriteFile(hPackFile, pFSHeader, pHeader->fDataSecSz, &written,NULL) || 
			written != pHeader->fDataSecSz ) 
			return 0L;
		
		// change size of writed bytes.
		lpNumberOfBytesWritten += written;
		// change full read bytes size.
		fullReadSz             += readSz;
		// change pack file handle offset.
		packOffset             += written;
		// if not found free file data section.
		// change The starting offset of file search section.
		if( !bFoundFreeSec )
			pHeader->fssHeader.offsetFSS = packOffset;
		}
	while( readSz == pHeader->fDataSecSz-sizeof(PACK_FS_HEADER) );

	// change data section size of pack file.
	// change file count in pack file.
	pHeader->fileCt ++;
	return fileOffset ;
	}

	// writes pack file header.
static bool 
WritePackHeader(HANDLE       hPackFile     /*in *//*handle to the pack file*/,
		        PACK_HEADER* pHeader       /*in *//*pointer to th pack header structure.*/)
	{
	DWORD written = 0;
	// move file pointer to the beginning of file.
	if( SetFilePointer(hPackFile,0L,0L,FILE_BEGIN) == (DWORD)-1 )
		return false;
	// write header of pack file.
	if( !WriteFile(hPackFile,pHeader,sizeof(PACK_HEADER),&written,NULL) ||
		written != sizeof(PACK_HEADER) )   
		return false;
	else
		return true;
	}
	
	// write FSS(file search section) of pack file.
static bool 
WritePackFSS(HANDLE       hPackFile     /*in *//*handle to the pack file*/,
			 PACK_HEADER* pHeader       /*in *//*pointer to th pack header structure.*/,
		     void*        pFNArray      /*in *//*pointer to the file names array.*/,
		     DWORD        fnArraySz     /*in *//*size of file names array (in bytes).*/,
			 DWORD*       pFOArray      /*in *//*pointer to the file offsets array.*/,
			 DWORD*       pFNOArray     /*in *//*pointer to the file name offsets array.*/,
			 DWORD        fileCt        /*in *//*file count in FSS*/)
	{
	DWORD written = 0;
	DWORD arrSize = sizeof(DWORD)*fileCt;
	if( SetFilePointer(hPackFile,0L,NULL,FILE_BEGIN) == (DWORD)-1 )
		return false;
	// move pack file handle to the start of FSS.
	if( SetFilePointer(hPackFile,pHeader->fssHeader.offsetFSS,0L,FILE_CURRENT) == (DWORD)-1 )
		return false;
	// write file name array.
	if( !WriteFile(hPackFile,pFNArray,fnArraySz,&written,NULL) || written != fnArraySz )
		return false;
	// write file name offsets array.
	if( !WriteFile(hPackFile,pFNOArray,arrSize,&written,NULL)  || written != arrSize ) 
		return false;
	// write file offsets array.
	if( !WriteFile(hPackFile,pFOArray,arrSize,&written,NULL)   || written != arrSize )
		return false;
	else
		return true;
	}

	// adds file to the FSS(file search section).
static void 
AddFileToFSS(LPCSTR  spfsFileName       /*in *//*contained file name with extention.*/,
			 DWORD   offset             /*in *//*starting offset of current file in pack file.*/,
		     void*&  pFNArray           /*in *//*pointer to the file names array.*/,
			 DWORD&  fnArraySz          /*in *//*size of allocated memory for file names array.*/,
		     DWORD&  fnArrUsed          /*in *//*used memory size from allocated memory.*/, 
			 DWORD*& pFOArray           /*in *//*pointer to the file offsets array.*/,
			 DWORD&  foArraySz          /*in *//*size of allocated memory for file offsets array.*/,
			 DWORD*& pFNOArray          /*in *//*pointer to the file name offsets array.*/,
			 DWORD&  fnoArraySz         /*in *//*size of allocated memory for file name offsets array.*/,
			 DWORD&  fileCt             /*in *//*file count in FSS*/)
	{
	// add file(with extention) name into file name array.{{
	size_t nameLength = strlen(spfsFileName)+1;
	if( fnArrUsed + nameLength > fnArraySz )
		{
		fnArraySz += 0x400;
		pFNArray   = realloc(pFNArray,fnArraySz);
		}
	memcpy((void*)((int)pFNArray+fnArrUsed),spfsFileName,nameLength);
	fnArrUsed     += (DWORD)nameLength;
	// }}

	// check size for file offsets array.{{
	if( foArraySz  < (fileCt+1)*sizeof(DWORD) )
		{
		foArraySz += 0x400;
		pFOArray   = (DWORD*)realloc(pFOArray,foArraySz);
		}
	// }}

	// check size for file name offsets array.{{
	if( fnoArraySz  < (fileCt+1)*sizeof(DWORD) )
		{
		fnoArraySz += 0x400;
		pFNOArray   = (DWORD*)realloc(pFNOArray,fnoArraySz);
		}
	// }}

	// insert file start offset & file name offset in sorted lists of file offsets & file name offsets.{{
	// find insert index of file name into file name offsets & file offset array.
	// file name offsets & file offsets array is sort by ascending.
	int index  = FindFileNamePlaceAsc(spfsFileName,pFNArray,fileCt,pFNOArray,pFOArray);
	// if insert index is not last of array.
	if( index != fileCt  )
		{
		// free space for new element to set in sorted file name offsets list.
		memmove((void*)((long)pFNOArray+sizeof(DWORD)*(index+1)),
				(void*)((long)pFNOArray+sizeof(DWORD)*index),(fileCt-index)*sizeof(DWORD));
		// free space for new element to set in sorted file offsets list.
		memmove((void*)((long)pFOArray+sizeof(DWORD)*(index+1)),
				(void*)((long)pFOArray+sizeof(DWORD)*index),(fileCt-index)*sizeof(DWORD));
		}
	// set new element in sorted file name offset list.
	pFNOArray[index] = fnArrUsed-nameLength;
	// set new element in sorted file name offset list.
	pFOArray [index] = offset;
	// increase file count.
	fileCt        ++;
	// }}
	}


class SPFSException 
{
	private:
		string _strErr;
		DWORD  _errCode;
	public:
		SPFSException(){
			_strErr  = ""; 
			_errCode = 0;
		}
		SPFSException(string s,DWORD errCode){
			_strErr  = s; 
			_errCode = errCode;
		}
		string GetErrorString() { return _strErr; }
		DWORD  GetErrorCode  () { return _errCode;}
};

#endif