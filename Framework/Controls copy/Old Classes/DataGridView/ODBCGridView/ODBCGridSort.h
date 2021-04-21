///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////
///////           Zqr Specific sort information.
///////

//minimal date time in variant format.
#define MINIMAL_DATE -657434.00000000000

// Sort Data Structures for Text,Number & DateTime Sorting. {{
typedef struct ODBCTxtSortDataTag
	{
	UGMemRI*     rowPtr;          // rowInfo pointer.
	LPTSTR       lpszText;        // string value for current row to sort.
	}ODBCTxtSortData;

typedef struct ODBCNumbSortDataTag
	{
	UGMemRI*     rowPtr;          // rowInfo pointer.
	double       dValue;          // double value for current row to sort.
	}ODBCNumbSortData;

typedef struct ODBCDtSortDataTag
	{
	UGMemRI*     rowPtr;          // rowInfo pointer.
	COleDateTime dateTime;        // DateTime value for current row to sort.
	}ODBCDtSortData;

typedef struct ODBCBoolSortDataTag
	{
	UGMemRI*     rowPtr;          // rowInfo pointer.
	BOOL         bValue;          // BOOL value for current row to sort.
	}ODBCBoolSortData;
//////////////////////////////////////////// }}

typedef struct ODBCSortInfoTag
	{
	ODBCTxtSortData*	pTxtList;   //pointers array of sorted texts.    
	ODBCNumbSortData*	pNumbList;  //pointers array of sorted Numbers.
	ODBCDtSortData*		pDtList;    //pointers array of sorted DateTimes.
	ODBCBoolSortData*	pBoolList;  //pointers array of sorted Bools.
	DWORD				dwSortedCt; //just sorted row count.
	DWORD				dwAllCt;    //all row count.
	int					col;        //column index to sort.
	}ODBCSortInfo;
// }}

// Find Row Place Function typedef.
typedef int     (*FRPProc)    ();
// SortByXXX Function typedef.
typedef UGMemRI* (*SortByProc)(UGMemRI* startRow,UGMemRI* endRow);

// table for compatible georgian simbols to standart ASCII.
static const BYTE btASCII_Geo[] = 
	{
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,1,2,3,4,5,6,7,8,9,10,
	11,12,13,14,15,16,17,18,19,
	20,21,22,23,24,25,26,27,28,29,
	30,31,32,33,34,35,85,36,37,38,
	39,40,41,75,42,43,44,45,46,47,
	48,82,84,67,49,50,89,51,52,
	87,53,54,55,56,57,58,59,60,86,
	62,63,80,61,92,68,91,69,70,71,
	72,73,74,81,76,77,78,79,65,88,
	90,83,66,93,94,95,96
	};
// example charGEO = btASCII_Geo[charASCII]; //ZQR Specific.

#define LNG_GEORGIAN

int StrCmp_Geo(const char *string1, const char *string2);

class ODBCSort : public CObject
{
public:
	friend   CUGCtrl;
	ODBCSort();
	virtual ~ODBCSort();

public:
	int             SortBy               (CUGMem* pMemMan, int col, int flags, int type);
	// returns index of repositional row if successfull otherwise -1.///
	int             RepositionRow        (CUGMem* pMemMan, UGMemRI* ptrRow, int row, BOOL bNewRow = FALSE);
	// deletes row pointer from sorted row list, if any column is sorted.
	void            DeleteRow            (CUGMem* pMemMan, UGMemRI* ptrRow, int row);
	// finds string in sorted column.///////////////////////////////////
	bool            FindFirstString      (CString strFind, int& nIndex);

protected:
	//function for finding data in sorted list in Asc or Desc. Zqr Specific.
	static int      FindRowPlaceAscTxt   ();
	static int      FindRowPlaceDescTxt  ();
	static int      FindRowPlaceAscNumb  ();
	static int      FindRowPlaceDescNumb ();
	static int      FindRowPlaceAscDTime ();
	static int      FindRowPlaceDescDTime();
	static int      FindRowPlaceAscBool  ();
	static int      FindRowPlaceDescBool ();

	//sorting functions.
	//returns last row pointer.
	static UGMemRI* SortByText           (UGMemRI* startRow,UGMemRI* endRow);
	static UGMemRI* SortByNumber         (UGMemRI* startRow,UGMemRI* endRow);
	static UGMemRI* SortByBool           (UGMemRI* startRow,UGMemRI* endRow);
	static UGMemRI* SortByDateTime       (UGMemRI* startRow,UGMemRI* endRow);

protected:
	UGSortInfo   m_sortInfo;     // sort data for currently sorted column.
	FRPProc      m_funcFRP;      // pointer to the FindRowPlace function.
	SortByProc   m_funcSortBy;   // current SortBy function address.

	void FreeSavedSortedInfo();
	void PrepareMemoryBuffer(int nElemCt, int nElemSize,BOOL bCopyOldBuffer = TRUE);
	
private:
	// Addresses of SortByXX functions.
	SortByProc*  m_sortBy;
	
	// sort info buffer.////////////////////////////////////
	void*        m_pBuffer;     // allocated memory pointer.
	UINT         m_uAllocSize;  // memory size.
	UINT         m_uGrowSize;   // memory grow size.
	//------------------------------------------------------
};

__inline void 
CUGSort::PrepareMemoryBuffer(int nElemCt, int nElemSize/*each element size in bytes.*/, BOOL bCopyOldBuffer)
	{
	ASSERT( nElemCt > 0 && nElemSize > 0 );
	if( (UINT)(nElemCt*nElemSize) > m_uAllocSize )
		{
		void* lpTmp  = m_pBuffer;
		m_pBuffer    = new BYTE[nElemCt*nElemSize + m_uGrowSize];
		
		if( lpTmp && bCopyOldBuffer )
			memcpy(m_pBuffer, lpTmp, m_uAllocSize);

		m_uAllocSize = nElemCt*nElemSize + m_uGrowSize;
		}
	}

static int              g_insertIndex; // specifies insert index.
static FRPProc          g_funcFRP;     // pointer to the FindRowPlace function.
static ODBCSortInfo     g_sortInfo;    // info of current sort operation.

//values for text,number & DateTime sorting functions./////////////////////
static ODBCTxtSortData    g_txtSortData;     
static ODBCNumbSortData   g_numbSortData;    
static ODBCDtSortData     g_dtSortData;
static ODBCBoolSortData   g_boolSortData;
//#########################################################################