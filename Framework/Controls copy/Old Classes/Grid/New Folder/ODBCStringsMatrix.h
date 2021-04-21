#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// ODBCStringsMatrix.h : implementation file

class CODBCRecordset;
class AutoSortedArray;
class CRecord;

/////////////////////////////////////////////////////////////////////////////
// ODBCStringsMatrix

typedef int		(__cdecl* CompareStrings_Proc)		(LPCTSTR, LPCTSTR);

class ODBCStringsMatrix
{
// Construction
public:
	ODBCStringsMatrix();
	virtual ~ODBCStringsMatrix();

	static int* GetSymbolIndexesArray()
		{
		static int	nGeoSymbolIndexes[256];
		return nGeoSymbolIndexes;
		}

	static void InitSymbolIndexArray();

	// Fills combo from recordset. returns item count.
	int		LoadFromRst		(CODBCRecordset* pRst, int nDataId, int* pStringIds, int nCount);
	int		LoadFromRst2	(CRecord* pRecord, AutoSortedArray* pArrIdToRecData, int nDataId, int* pStringIds, int nCount);
	BOOL	IsLoaded		();

	// Searchs current string in item string.
	int		SearchStrings	(LPCTSTR szStringSearch);
	int		GetStringsCount	();

	BOOL	GetResultString	(int nRow, int nCol, CString& sResult);
	BOOL	GetResultString2(int nRow, int nCol, CString& sResult);
	long	GetResultParam	(int nRow);
	void	SetResultParam	(int nRow, long lParam);
	long	GetResultParam2	(int nRow);
	void	SetResultParam2	(int nRow, long lParam);
	int		GetResultCount	();
	
	AutoSortedArray*		GetSearchResultArray();
protected:
	void	Initialize				();
	int		FindStrings			(LPCTSTR szStringSearch, AutoSortedArray* pArrDatas, AutoSortedArray* pArrDatas1);
	int		FindStringsExact	(LPCTSTR szStringSearch, AutoSortedArray* pArrDatas, AutoSortedArray* pArrDatas1);

	// Reloads all strings into list.
	int		ReloadAllStrings	();
	// Cutts string into small strings.
	int		CutToSmallStrings	(char* szString, CStringArray& arrSmallStrings);
	// Frees sorted strings array.
	void	FreeStringsArray	();
	// Adds string into buffer and after into sorted array.
	void	AddStringToBufferArray(CString* pString, long lData);
	void	InsertToArray		(void* pBuffer, int nInsert, int nBlockSize, int nBlockCount, long lInsert);

// Attributes	
protected:
	CompareStrings_Proc		m_compareProc;
	BYTE*					m_pBuffer;					// Heap buffer pointer.
	HANDLE					m_hHeap;					// Handle of created Heap.
	int						m_nHeapAllocSize;			// Allocated heap size.
	int						m_nHeapUsedSize;			// Used heap memory size.
	int						m_nColFields;				// Column fields count.

	AutoSortedArray			m_arrSearchResult;			// Search result strings to id.
	// Array of small strings.
	AutoSortedArray			m_arrStrings;				// char* to int
	// Array of full strings.
	AutoSortedArray			m_arrFullStrings;			// int to char*

	BYTE*					m_pStringBuffer;			// Buffer of the strings.
	int						m_nOffset;					// Offset of current memory buffer.
	int						m_nBuffSize;				// Size of buffer.
};

__inline BOOL
ODBCStringsMatrix::IsLoaded()
	{
	return (m_pStringBuffer != NULL);
	}

__inline void
ODBCStringsMatrix::InsertToArray(void* pBuffer, int nInsert, int nBlockSize, int nBlockCount, long lInsert)
	{
	void* dest   = (void*)((long)pBuffer + nBlockSize*nInsert);
	long  size   = nBlockSize*(nBlockCount - nInsert);
	if( size > 0 )
		memmove((void*)((long)dest + nBlockSize), dest, size);

	long* pLong  = (long*)dest;
	*pLong		 = lInsert;
	}

__inline int		
ODBCStringsMatrix::GetStringsCount()
	{
	return m_arrFullStrings.GetCount();
	}

__inline int
ODBCStringsMatrix::GetResultCount()
	{
	return m_arrSearchResult.GetCount();
	}

__inline AutoSortedArray*
ODBCStringsMatrix::GetSearchResultArray()
	{
	return &m_arrSearchResult;
	}

__inline long
ODBCStringsMatrix::GetResultParam(int nRow)
	{
	ASSERT( nRow > -1 );
	if( nRow < m_arrSearchResult.GetCount() )
		{
		long	lStr		= (long)&m_pBuffer[m_arrSearchResult.GetData(nRow)];
		int*	pOffsets	= (int*)(lStr - sizeof(int)*(m_nColFields + 1));
		return (long)pOffsets[0];
		}
	return 0;
	}

__inline long
ODBCStringsMatrix::GetResultParam2(int nRow)
	{
	ASSERT( nRow > -1 );
	if( nRow < m_arrSearchResult.GetCount() )
		{
		long	lOffset		= m_arrSearchResult.GetData(nRow);
		return ((long*)(&m_pBuffer[lOffset]))[0];
		}
	return 0;
	}

__inline void
ODBCStringsMatrix::SetResultParam(int nRow, long lParam)
	{
	}

int	CompareStrings_Geo(LPCTSTR lpsz1, LPCTSTR lpsz2);
