#include "stdafx.h"
#include "ODBCSort.h"

// disable type cast warnings.[ZQR]
#pragma warning(disable:4311)
#pragma warning(disable:4312)
//---------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/***************************************************
****************************************************/
ODBCSort::ODBCSort()
	{
	// Initialize SortBy Function Addresses. //[Zqr Specific.]
	m_sortBy                       = new SortByProc[10];
	m_sortBy[UGCELLDATA_STRING]    = (SortByProc)SortByText;
	m_sortBy[UGCELLDATA_NUMBER]    = (SortByProc)SortByNumber;
	m_sortBy[UGCELLDATA_CURRENCY]  = (SortByProc)SortByNumber;
	m_sortBy[UGCELLDATA_TIME]      = (SortByProc)SortByDateTime;
	m_sortBy[UGCELLDATA_DATETIME]  = (SortByProc)SortByDateTime;
	m_sortBy[UGCELLDATA_TIME_FULL] = (SortByProc)SortByDateTime;
	m_sortBy[UGCELLDATA_BOOL]      = (SortByProc)SortByBool;
	
	ZeroMemory(&m_sortInfo, sizeof(UGSortInfo));
	m_sortInfo.col                 = -1;

	m_pBuffer                      = NULL;
	m_uAllocSize                   = 0L;
	m_uGrowSize                    = max( max(sizeof(UGTxtSortData),sizeof(UGNumbSortData)),
		                                  max(sizeof(UGDtSortData),sizeof(UGBoolSortData))) * 100;
	}
/***************************************************
****************************************************/
CUGSort::~CUGSort()
	{
	delete m_sortBy;
	if( m_uAllocSize && m_pBuffer )
		{
		delete m_pBuffer;
		m_pBuffer    = NULL;
		m_uAllocSize = 0L;
		}
	}

/***************************************************
flags - UG_SORT_ASSENDING or UG_SORT_DESCENDING
Zqr - Specific 30x faster then dundas's sort.
****************************************************/

int 
StrCmp_Geo(const char *string1, const char *string2)
	{
	__asm
		{
		push ecx;
		push edx;
		push ebp;
		mov  esi,dword ptr string1;
		mov  edi,dword ptr string2;
		lea  ebp,dword ptr[btASCII_Geo];
		mov  eax,esi;
		sub  eax,edi;
		cmp  eax,0;
		je   m5; 
		xor  eax,eax;
		dec  eax;
		test esi,esi;
		je   m5;
		inc  eax;  
		test edi,edi;
		je   m5;
m0:		mov  cl,byte ptr[esi];
		mov  ch,byte ptr[edi];
		test cl,cl;
		jne  m1;
		xor  eax,eax;
		dec  eax;
		test ch,ch;
		jne  m5;
		inc  eax;
		jmp  m5;
m1:     test ch,ch;
		jne  m2;
		xor  eax,eax;
		inc  eax;
		test cl,cl;
		jne  m5;
		dec  eax;
		jmp  m5;
m2:		xor  edx,edx;
		mov  dl,cl;
		add  edx,ebp;
  		mov  al,byte ptr[edx];
		xor  edx,edx;
		mov  dl,ch;
		add  edx,ebp;  
		mov  ah,byte ptr[edx];
		cmp  al,ah;
		jle  m3;
		xor  eax,eax;
		inc  eax;
		jmp  m5;
m3:     cmp  al,ah;
		jge  m4;
		xor  eax,eax;
		dec  eax;
		jmp  m5;
m4:     inc  esi;
		inc  edi;
		jmp  m0;
m5:	    pop  ebp;
		pop  edx;
		pop  ecx;
		};
	}
/***************************************************
****************************************************/

int             
ODBCSort::RepositionRow(CUGMem* pMemMan, UGMemRI* ptrRow, int row, BOOL bNewRow)
	{
	if( m_sortInfo.col == -1 || !ptrRow || row < 0 || !pMemMan ) 
		return -1;

	if( bNewRow )
		{
		m_sortInfo.dwSortedCt      = m_sortInfo.dwAllCt;
		m_sortInfo.dwAllCt  ++;

		if( m_funcSortBy == SortByText )
			{
			PrepareMemoryBuffer(m_sortInfo.dwAllCt, sizeof(UGTxtSortData), TRUE);
			m_sortInfo.pTxtList    = (UGTxtSortData*)m_pBuffer;
			}
		else if( m_funcSortBy == SortByNumber )
			{
			PrepareMemoryBuffer(m_sortInfo.dwAllCt, sizeof(UGNumbSortData), TRUE);
			m_sortInfo.pNumbList   = (UGNumbSortData*)m_pBuffer;
			}
		else if( m_funcSortBy == SortByDateTime )
			{
			PrepareMemoryBuffer(m_sortInfo.dwAllCt, sizeof(UGDtSortData), TRUE);
			m_sortInfo.pDtList     = (UGDtSortData*)m_pBuffer;
			}
		else if( m_funcSortBy == SortByBool )
			{
			PrepareMemoryBuffer(m_sortInfo.dwAllCt, sizeof(UGBoolSortData), TRUE);
			m_sortInfo.pBoolList   = (UGBoolSortData*)m_pBuffer;
			}
		}
	else
		{
		m_sortInfo.dwSortedCt = m_sortInfo.dwAllCt-1;
		// remove row from list.//////////////////////////
		if( row < (int)(m_sortInfo.dwSortedCt)  )
			{
			void* dest   = NULL;
			void* src    = NULL;
			DWORD dwSize = (m_sortInfo.dwSortedCt - row);

			if( m_funcSortBy == SortByNumber )
				{
				m_sortInfo.pNumbList   = (UGNumbSortData*)m_pBuffer;
				dest    = (void*)((long)m_sortInfo.pNumbList+sizeof(UGNumbSortData)*(row));
				src     = (void*)((long)m_sortInfo.pNumbList+sizeof(UGNumbSortData)*(row+1)  );
				dwSize *= sizeof(UGNumbSortData);
				}
			else if( m_funcSortBy == SortByText )
				{
				m_sortInfo.pTxtList    = (UGTxtSortData*)m_pBuffer;
				dest    = (void*)((long)m_sortInfo.pTxtList+sizeof(UGTxtSortData)*(row)  );
				src     = (void*)((long)m_sortInfo.pTxtList+sizeof(UGTxtSortData)*(row+1));
				dwSize *= sizeof(UGTxtSortData);
				}
			else if( m_funcSortBy == SortByDateTime )
				{
				m_sortInfo.pDtList     = (UGDtSortData*)m_pBuffer;
				dest    = (void*)((long)m_sortInfo.pDtList+sizeof(UGDtSortData)*(row)  );
				src     = (void*)((long)m_sortInfo.pDtList+sizeof(UGDtSortData)*(row+1));
				dwSize *= sizeof(UGDtSortData);
				}
			else if( m_funcSortBy == SortByBool )
				{
				m_sortInfo.pBoolList   = (UGBoolSortData*)m_pBuffer;
				dest    = (void*)((long)m_sortInfo.pBoolList+sizeof(UGBoolSortData)*(row)  );
				src     = (void*)((long)m_sortInfo.pBoolList+sizeof(UGBoolSortData)*(row+1));
				dwSize *= sizeof(UGBoolSortData);
				}

			ASSERT( src && dest );
			memmove(dest,src,dwSize);
			}
		//-----------------------------------------------
		}

	g_funcFRP             = m_funcFRP;
	memcpy(&g_sortInfo, &m_sortInfo, sizeof(UGSortInfo));

	// call SortByXXX Function./////////////////
	UGMemRI* rowInfo      = (*m_funcSortBy)(ptrRow->prev,ptrRow->next);
	//------------------------------------------

	memcpy(&m_sortInfo, &g_sortInfo, sizeof(UGSortInfo));
	
	// set new position for memory manager./////
	pMemMan->m_rowInfo    = rowInfo;
	pMemMan->m_currentRow = g_sortInfo.dwSortedCt-1;
	pMemMan->m_colInfo    = rowInfo->col;
	pMemMan->m_currentCol = 0;
	pMemMan->m_endRow     = rowInfo;
	//------------------------------------------

	// after all zero memory for sorted info.///
	ZeroMemory(&g_sortInfo,sizeof(UGSortInfo));
	//------------------------------------------
	return g_insertIndex;
	}
/***************************************************
****************************************************/

void            
ODBCSort::DeleteRow(CUGMem* pMemMan, UGMemRI* ptrRow, int row)
	{
	if( m_sortInfo.col == -1 || !ptrRow || row < 0 || !pMemMan ) 
		return;

	if( m_funcSortBy == SortByText )
		{
		if( (int)m_sortInfo.dwSortedCt > row )
			{
			m_sortInfo.pTxtList     = (UGTxtSortData*)m_pBuffer;
			ASSERT(m_sortInfo.pTxtList[row].rowPtr == ptrRow);
			void* dest = (void*)(((long)m_sortInfo.pTxtList) + row*sizeof(UGTxtSortData));
			void* src  = (void*)(((long)dest) + sizeof(UGTxtSortData));
			memcpy(dest, src, (m_sortInfo.dwSortedCt-row-1)*sizeof(UGTxtSortData));
			m_sortInfo.dwAllCt    --;
			m_sortInfo.dwSortedCt --;
			}
		}
	else if( m_funcSortBy == SortByNumber )
		{
		if( (int)m_sortInfo.dwSortedCt > row )
			{
			m_sortInfo.pNumbList   = (UGNumbSortData*)m_pBuffer;
			ASSERT(m_sortInfo.pNumbList[row].rowPtr == ptrRow);
			void* dest = (void*)(((long)m_sortInfo.pNumbList) + row*sizeof(UGNumbSortData));
			void* src  = (void*)(((long)dest) + sizeof(UGNumbSortData));
			memcpy(dest, src, (m_sortInfo.dwSortedCt-row-1)*sizeof(UGNumbSortData));
			m_sortInfo.dwAllCt    --;
			m_sortInfo.dwSortedCt --;
			}
		}
	else if( m_funcSortBy == SortByDateTime )
		{
		if( (int)m_sortInfo.dwSortedCt > row )
			{
			m_sortInfo.pDtList     = (UGDtSortData*)m_pBuffer;
			ASSERT(m_sortInfo.pDtList[row].rowPtr == ptrRow);
			void* dest = (void*)(((long)m_sortInfo.pDtList) + row*sizeof(UGDtSortData));
			void* src  = (void*)(((long)dest) + sizeof(UGDtSortData));
			memcpy(dest, src, (m_sortInfo.dwSortedCt-row-1)*sizeof(UGDtSortData));
			m_sortInfo.dwAllCt    --;
			m_sortInfo.dwSortedCt --;
			}
		}
	else if( m_funcSortBy == SortByBool )
		{
		if( (int)m_sortInfo.dwSortedCt > row )
			{
			m_sortInfo.pBoolList   = (UGBoolSortData*)m_pBuffer;
			ASSERT(m_sortInfo.pBoolList[row].rowPtr == ptrRow);
			void* dest = (void*)(((long)m_sortInfo.pBoolList) + row*sizeof(UGBoolSortData));
			void* src  = (void*)(((long)dest) + sizeof(UGBoolSortData));
			memcpy(dest, src, (m_sortInfo.dwSortedCt-row-1)*sizeof(UGBoolSortData));
			m_sortInfo.dwAllCt    --;
			m_sortInfo.dwSortedCt --;
			}
		}
	}
/***************************************************
****************************************************/

int 
ODBCSort::SortBy(CUGMem *pMemMan, int col,int flags,int type)
	{
	long     lRowCount = pMemMan->m_lRowCount;
	UGMemRI* startRow  = pMemMan->m_startRow;
	UGMemRI* endRow    = pMemMan->m_endRow;

	// check row count while bgn sort.///
	if( lRowCount < 3 )
		{
		TRACE("No rows to sort!");
		return 1;
		}
	//-----------------------------------

	// check pointers for validation.////
	while( startRow->prev )
		   startRow     = startRow->prev;
	while( endRow->next  )
		   endRow       = endRow->next;
	pMemMan->m_startRow = startRow;
	pMemMan->m_endRow   = endRow;
	//-----------------------------------

	// check row count.//////////////////
	if( !startRow->next || startRow->next == endRow ) 
		{
		TRACE("row count is valid for sort but start & end row pointers are incorrects");
		return 1;
		}
	//-----------------------------------
	
	g_sortInfo.col          = col;
	g_sortInfo.dwAllCt      = lRowCount-1;
	g_sortInfo.dwSortedCt   = 0;

	switch( type )
		{
	case UGCELLDATA_STRING:
		{
		PrepareMemoryBuffer(g_sortInfo.dwAllCt, sizeof(UGTxtSortData), FALSE);
		g_sortInfo.pTxtList  = (UGTxtSortData*)m_pBuffer;
		// initialize FRP function address.
		g_funcFRP = ( flags == UG_SORT_ASCENDING ) ? (FRPProc)FindRowPlaceAscTxt : (FRPProc)FindRowPlaceDescTxt;
		break;
		}
	case UGCELLDATA_NUMBER:
    case UGCELLDATA_CURRENCY:
		{
		PrepareMemoryBuffer(g_sortInfo.dwAllCt, sizeof(UGNumbSortData), FALSE);
		g_sortInfo.pNumbList = (UGNumbSortData*)m_pBuffer;
		// initialize FRP function address.
		g_funcFRP = ( flags == UG_SORT_ASCENDING ) ? (FRPProc)FindRowPlaceAscNumb : (FRPProc)FindRowPlaceDescNumb;
		break;
		}
	case UGCELLDATA_TIME:
	case UGCELLDATA_DATETIME:
	case UGCELLDATA_TIME_FULL:
		{
		PrepareMemoryBuffer(g_sortInfo.dwAllCt, sizeof(UGDtSortData), FALSE);
		g_sortInfo.pDtList   = (UGDtSortData*)m_pBuffer;
		// initialize FRP function address.
		g_funcFRP = ( flags == UG_SORT_ASCENDING ) ? (FRPProc)FindRowPlaceAscDTime : (FRPProc)FindRowPlaceDescDTime;
		break;
		}
	case UGCELLDATA_BOOL:
		{
		PrepareMemoryBuffer(g_sortInfo.dwAllCt, sizeof(UGBoolSortData), FALSE);
		g_sortInfo.pBoolList = (UGBoolSortData*)m_pBuffer;
		// initialize FRP function address.
		g_funcFRP = ( flags == UG_SORT_ASCENDING ) ? (FRPProc)FindRowPlaceAscBool : (FRPProc)FindRowPlaceDescBool;
		break;
		}
	default:
		ASSERT(FALSE);
		}
	
	// call SortByXXX Function./////////////////
	UGMemRI* rowInfo      = (*m_sortBy[type])(startRow,NULL);
	//------------------------------------------
	
	// set new position for memory manager./////
	pMemMan->m_rowInfo    = rowInfo;
	pMemMan->m_currentRow = g_sortInfo.dwSortedCt-1;
	pMemMan->m_colInfo    = rowInfo->col;
	pMemMan->m_currentCol = 0;
	pMemMan->m_endRow     = rowInfo;
	//------------------------------------------

	m_funcSortBy          = m_sortBy[type];
	m_funcFRP             = g_funcFRP;
	memcpy((void*)&m_sortInfo,(void*)&g_sortInfo,sizeof(UGSortInfo));
	ZeroMemory(&g_sortInfo,sizeof(UGSortInfo));

	return UG_SUCCESS;
	}
/***************************************************
****************************************************/

bool            
ODBCSort::FindFirstString(CString strFind, int& nIndex)
	{
	nIndex = -1;
	if( m_sortInfo.col == -1 ) 
		return false;

	if( m_funcSortBy == SortByText )
		{
		memcpy(&g_sortInfo,&m_sortInfo,sizeof(UGSortInfo));
		g_txtSortData.lpszText = strFind.GetBuffer();
		nIndex                 = (*g_funcFRP)();
		g_txtSortData.lpszText = _T("");
		ZeroMemory(&g_sortInfo,sizeof(UGSortInfo));

		if( g_funcFRP == FindRowPlaceDescTxt && nIndex > 0 )
			nIndex --;

		if( nIndex < (int)m_sortInfo.dwSortedCt )
			{
			CString sFound(m_sortInfo.pTxtList[nIndex].lpszText);
			if( strFind.GetLength() <= sFound.GetLength()  )
				{
				sFound = sFound.Left(strFind.GetLength());
				return !sFound.Compare(strFind);
				}
			return false;
			}
		}
	
	return false;
	}
/***************************************************
****************************************************/

UGMemRI* 
ODBCSort::SortByText(UGMemRI* startRow,UGMemRI* endRow)
	{
	CUGCell*      pCell      = NULL;
	UGMemRI*      pRowCurr   = startRow->next;
	UGMemRI*      next       = NULL;
	UGMemRI*      temp       = NULL;
	UGMemRI*      prev       = NULL; 
	g_insertIndex            = -1;
	int           col        = g_sortInfo.col;

	if( g_sortInfo.dwSortedCt > 0 && startRow->prev )
		{
		startRow             = g_sortInfo.pTxtList[0].rowPtr;
		ASSERT( startRow && startRow->prev );
		startRow             = startRow->prev;
		}

	while( pRowCurr != endRow )
		{
		// initialize sort data to find in sorted row list.
		pCell                  = CUGMem::GetCell(pRowCurr->col,col);
		g_txtSortData.rowPtr   = pRowCurr;
		// if column type is string then lpszText is string pointer.
		g_txtSortData.lpszText = pCell ? (LPTSTR)pCell->GetText() : "";
		
		// find current rowInfo place in sorted rowInfo list. {{
		// call FindRowPlace Function. Zqr Specific.
		g_insertIndex = (*g_funcFRP)();
		// }}
		// calculate prev row pointer of insert place. {{
		prev    = ( !g_insertIndex ) ? startRow : g_sortInfo.pTxtList[g_insertIndex-1].rowPtr;
		// }}

		// remove find row.
		next           = pRowCurr->next;
		if( next )
			next->prev = pRowCurr->prev;
		pRowCurr->prev->next = next;
		
		// insert row in new founded place.
		temp           = prev->next;
		prev->next     = pRowCurr;
		pRowCurr->prev = prev;
		pRowCurr->next = temp;
		if( temp )
			temp->prev = pRowCurr;
		
		// free space for new element to set in sorted row list.
		if( g_insertIndex != g_sortInfo.dwSortedCt  )
			{
			memmove((void*)((long)g_sortInfo.pTxtList+sizeof(UGTxtSortData)*(g_insertIndex+1)),
				    (void*)((long)g_sortInfo.pTxtList+sizeof(UGTxtSortData)*g_insertIndex),(g_sortInfo.dwSortedCt-g_insertIndex)*sizeof(UGTxtSortData));
			}
			
		// add new element in sorted row list.
		g_sortInfo.pTxtList[g_insertIndex].lpszText = g_txtSortData.lpszText;
		g_sortInfo.pTxtList[g_insertIndex].rowPtr   = g_txtSortData.rowPtr;
		// set next row pointer to find it's place in sorted row list.
		pRowCurr                                    = next;
		// increase sorted row list size.
		g_sortInfo.dwSortedCt ++;
		}

	// store last row pointer.
	pRowCurr = g_sortInfo.pTxtList[g_sortInfo.dwSortedCt-1].rowPtr;
	return pRowCurr;
	}

/***************************************************
****************************************************/
UGMemRI* 
ODBCSort::SortByNumber(UGMemRI* startRow,UGMemRI* endRow)
	{
	CUGCell*      pCell      = NULL;
	UGMemRI*      pRowCurr   = startRow->next;
	UGMemRI*      next       = NULL;
	UGMemRI*      temp       = NULL;
	UGMemRI*      prev       = NULL; 
	g_insertIndex            = -1;
	int           col        = g_sortInfo.col;

	if( g_sortInfo.dwSortedCt > 0 && startRow->prev )
		{
		startRow             = g_sortInfo.pNumbList[0].rowPtr;
		ASSERT( startRow && startRow->prev );
		startRow             = startRow->prev;
		}
		
	while( pRowCurr != endRow )
		{
		// initialize sort data to find in sorted row list.
		pCell                  = CUGMem::GetCell(pRowCurr->col,col);
		g_numbSortData.rowPtr  = pRowCurr;
		g_numbSortData.dValue  = pCell ? pCell->GetDouble() : 0.0;
		
		// find current rowInfo place in sorted rowInfo list. {{
		// call FindRowPlace Function. Zqr Specific.
		g_insertIndex = (*g_funcFRP)();
		// }}
		// calculate prev row pointer of insert place. {{
		prev = ( !g_insertIndex ) ? startRow : g_sortInfo.pNumbList[g_insertIndex-1].rowPtr;
		// }}

		// remove find row.
		next           = pRowCurr->next;
		if( next )
			next->prev = pRowCurr->prev;
		pRowCurr->prev->next = next;
		
		// insert row in new founded place.
		temp           = prev->next;
		prev->next     = pRowCurr;
		pRowCurr->prev = prev;
		pRowCurr->next = temp;
		if( temp )
			temp->prev = pRowCurr;
		
		// free space for new element to set in sorted row list.
		if( g_insertIndex != g_sortInfo.dwSortedCt  )
			{
			memmove((void*)((long)g_sortInfo.pNumbList+sizeof(UGNumbSortData)*(g_insertIndex+1)),
				    (void*)((long)g_sortInfo.pNumbList+sizeof(UGNumbSortData)*g_insertIndex),(g_sortInfo.dwSortedCt-g_insertIndex)*sizeof(UGNumbSortData));
			}
			
		// add new element in sorted row list.
		g_sortInfo.pNumbList[g_insertIndex].dValue = g_numbSortData.dValue;
		g_sortInfo.pNumbList[g_insertIndex].rowPtr = g_numbSortData.rowPtr;
		// set next row pointer to find it's place in sorted row list.
		pRowCurr                             = next;
		// increase sorted row list size.
		g_sortInfo.dwSortedCt ++;
		}

	// store last row pointer.
	pRowCurr = g_sortInfo.pNumbList[g_sortInfo.dwSortedCt-1].rowPtr;
	return pRowCurr;
	}

/***************************************************
****************************************************/
UGMemRI* 
ODBCSort::SortByBool(UGMemRI* startRow,UGMemRI* endRow)
	{
	CUGCell*      pCell      = NULL;
	UGMemRI*      pRowCurr   = startRow->next;
	UGMemRI*      next       = NULL;
	UGMemRI*      temp       = NULL;
	UGMemRI*      prev       = NULL; 
	g_insertIndex            = -1;
	int           col        = g_sortInfo.col;

	if( g_sortInfo.dwSortedCt > 0 && startRow->prev )
		{
		startRow             = g_sortInfo.pBoolList[0].rowPtr;
		ASSERT( startRow && startRow->prev );
		startRow             = startRow->prev;
		}
		
	while( pRowCurr != endRow )
		{
		// initialize sort data to find in sorted row list.
		pCell                  = CUGMem::GetCell(pRowCurr->col,col);
		g_boolSortData.rowPtr  = pRowCurr;
		g_boolSortData.bValue  = pCell ? (BOOL)pCell->GetDouble() : FALSE;
		
		// find current rowInfo place in sorted rowInfo list. {{
		// call FindRowPlace Function. Zqr Specific.
		g_insertIndex = (*g_funcFRP)();
		// }}
		// calculate prev row pointer of insert place. {{
		prev = ( !g_insertIndex ) ? startRow : g_sortInfo.pBoolList[g_insertIndex-1].rowPtr;
		// }}

		// remove find row.
		next           = pRowCurr->next;
		if( next )
			next->prev = pRowCurr->prev;
		pRowCurr->prev->next = next;
		
		// insert row in new founded place.
		temp           = prev->next;
		prev->next     = pRowCurr;
		pRowCurr->prev = prev;
		pRowCurr->next = temp;
		if( temp )
			temp->prev = pRowCurr;
		
		// free space for new element to set in sorted row list.
		if( g_insertIndex != g_sortInfo.dwSortedCt  )
			{
			memmove((void*)((long)g_sortInfo.pBoolList+sizeof(UGBoolSortData)*(g_insertIndex+1)),
				    (void*)((long)g_sortInfo.pBoolList+sizeof(UGBoolSortData)*g_insertIndex),(g_sortInfo.dwSortedCt-g_insertIndex)*sizeof(UGBoolSortData));
			}
			
		// add new element in sorted row list.
		g_sortInfo.pBoolList[g_insertIndex].bValue = g_boolSortData.bValue;
		g_sortInfo.pBoolList[g_insertIndex].rowPtr = g_boolSortData.rowPtr;
		// set next row pointer to find it's place in sorted row list.
		pRowCurr                             = next;
		// increase sorted row list size.
		g_sortInfo.dwSortedCt ++;
		}

	// store last row pointer.
	pRowCurr = g_sortInfo.pBoolList[g_sortInfo.dwSortedCt-1].rowPtr;
	return pRowCurr;
	}

/***************************************************
****************************************************/
UGMemRI* 
ODBCSort::SortByDateTime(UGMemRI* startRow,UGMemRI* endRow)
	{
	CUGCell*      pCell      = NULL;
	UGMemRI*      pRowCurr   = startRow->next;
	UGMemRI*      next       = NULL;
	UGMemRI*      temp       = NULL;
	UGMemRI*      prev       = NULL; 
	g_insertIndex            = -1;
	int           col        = g_sortInfo.col;
	COleDateTime  dt;

	if( g_sortInfo.dwSortedCt > 0 && startRow->prev )
		{
		startRow             = g_sortInfo.pDtList[0].rowPtr;
		ASSERT( startRow && startRow->prev );
		startRow             = startRow->prev;
		}

	while( pRowCurr != endRow )
		{
		// initialize sort data to find in sorted row list.
		pCell                 = CUGMem::GetCell(pRowCurr->col,col);
		g_dtSortData.rowPtr   = pRowCurr;
		g_dtSortData.dateTime = pCell ? pCell->GetDate() : dt;
		
		// find current rowInfo place in sorted rowInfo list. {{
		// call FindRowPlace Function. Zqr Specific.
		g_insertIndex         = (*g_funcFRP)();
		// }}

		// calculate prev row pointer of insert place. {{
		prev = ( !g_insertIndex ) ? startRow : g_sortInfo.pDtList[g_insertIndex-1].rowPtr;
		// }}

		// remove find row.
		next           = pRowCurr->next;
		if( next )
			next->prev = pRowCurr->prev;
		pRowCurr->prev->next = next;
		
		// insert row in new founded place.
		temp           = prev->next;
		prev->next     = pRowCurr;
		pRowCurr->prev = prev;
		pRowCurr->next = temp;
		if( temp )
			temp->prev = pRowCurr;
		
		// free space for new element to set in sorted row list.
		if( g_insertIndex != g_sortInfo.dwSortedCt  )
			{
			memmove((void*)((long)g_sortInfo.pDtList+sizeof(UGDtSortData)*(g_insertIndex+1)),
				    (void*)((long)g_sortInfo.pDtList+sizeof(UGDtSortData)*g_insertIndex),(g_sortInfo.dwSortedCt-g_insertIndex)*sizeof(UGDtSortData));
			}
			
		// add new element in sorted row list.
		g_sortInfo.pDtList[g_insertIndex].dateTime = g_dtSortData.dateTime;
		g_sortInfo.pDtList[g_insertIndex].rowPtr   = g_dtSortData.rowPtr;
		// set next row pointer to find it's place in sorted row list.
		pRowCurr                             = next;
		// increase sorted row list size.
		g_sortInfo.dwSortedCt ++;
		}

	// store last row pointer.
	pRowCurr = g_sortInfo.pDtList[g_sortInfo.dwSortedCt-1].rowPtr;
	return pRowCurr;
	}
/***************************************************
****************************************************/

int 
ODBCSort::FindRowPlaceAscTxt()
	{
	if( !g_sortInfo.dwSortedCt ) return 0;
	int            nLast  = g_sortInfo.dwSortedCt-1;
	int            nStart = 0;
	UGTxtSortData* pList  = g_sortInfo.pTxtList;
	LPTSTR         lpFind = g_txtSortData.lpszText;
	int            nRet;
	int            nMid;
	LPTSTR         lpList;

	while( nLast > nStart+1 )
		{
		nMid    = nStart+(nLast-nStart)/2;
		lpList  = pList[nMid].lpszText;

	#ifdef LNG_GEORGIAN
		nRet    = StrCmp_Geo(lpList,lpFind);
	#endif 
	#ifdef LNG_ENGLISH
		nRet    = strcmp(lpList,lpFind);
	#endif		
		if( !nRet )
			return nMid;	
		if( nRet > 0 )
			nLast  = nMid;
		else
			nStart = nMid;
		}
	
	lpList = pList[nStart].lpszText;
	#ifdef LNG_GEORGIAN
		nRet    = StrCmp_Geo(lpList,lpFind);
	#endif 
	#ifdef LNG_ENGLISH
		nRet    = strcmp(lpList,lpFind);
	#endif
	
	if( nRet >= 0 ) return nStart;
	else
		{
		lpList = pList[nLast].lpszText;
	#ifdef LNG_GEORGIAN
		nRet    = StrCmp_Geo(lpList,lpFind);
	#endif 
	#ifdef LNG_ENGLISH
		nRet    = strcmp(lpList,lpFind);
	#endif
		return ( nRet < 0 ) ? nLast+1 : nLast;
		}
	}

/***************************************************
****************************************************/
int 
ODBCSort::FindRowPlaceDescTxt()
	{
	if( !g_sortInfo.dwSortedCt ) return 0;
	int            nLast  = g_sortInfo.dwSortedCt-1;
	int            nStart = 0;
	UGTxtSortData* pList  = g_sortInfo.pTxtList;
	LPTSTR         lpFind = g_txtSortData.lpszText;
	int            nRet;
	int            nMid;
	LPTSTR         lpList;

	while( nLast > nStart+1 )
		{
		nMid    = nStart+(nLast-nStart)/2;
		lpList  = pList[nMid].lpszText;
	#ifdef LNG_GEORGIAN
		nRet    = StrCmp_Geo(lpFind,lpList);
	#endif 
	#ifdef LNG_ENGLISH
		nRet    = strcmp(lpFind,lpList);
	#endif
		
		if( !nRet    )    
			return   nMid;	
		if( nRet > 0 ) 
			nLast  = nMid;
		else
		    nStart = nMid;
		}
	
	lpList = pList[nStart].lpszText;
	#ifdef LNG_GEORGIAN
		nRet    = StrCmp_Geo(lpFind,lpList);
	#endif 
	#ifdef LNG_ENGLISH
		nRet    = strcmp(lpFind,lpList);
	#endif
	
	if( nRet >= 0 ) return nStart;
	else
		{
		lpList = pList[nLast].lpszText;
	#ifdef LNG_GEORGIAN
		nRet    = StrCmp_Geo(lpFind,lpList);
	#endif 
	#ifdef LNG_ENGLISH
		nRet    = strcmp(lpFind,lpList);
	#endif
		return ( nRet < 0 ) ? nLast+1 : nLast;
		}
	}
/***************************************************
****************************************************/
int 
ODBCSort::FindRowPlaceAscNumb()
	{
	if( !g_sortInfo.dwSortedCt ) return 0;
	int             nLast  = g_sortInfo.dwSortedCt-1;
	int             nStart = 0;
	UGNumbSortData* pList  = g_sortInfo.pNumbList;
	double          dFind  = g_numbSortData.dValue;
	int             nMid;
	double          dList;

	while( nLast > nStart+1 )
		{
		nMid    = nStart+(nLast-nStart)/2;
		dList   = pList[nMid].dValue;

		if( dList == dFind )
			return nMid;
		else
			( dList > dFind ) ?	nLast = nMid : nStart = nMid;
		}
	
	dList = pList[nStart].dValue;
	if( dList >= dFind )
		return nStart;
	else
		{
		dList = pList[nLast].dValue;
		return ( dList < dFind ) ? nLast+1 : nLast;
		}
	}
/***************************************************
****************************************************/
int 
CUGSort::FindRowPlaceDescNumb()
	{
	if( !g_sortInfo.dwSortedCt ) return 0;
	int             nLast  = g_sortInfo.dwSortedCt-1;
	int             nStart = 0;
	UGNumbSortData* pList  = g_sortInfo.pNumbList;
	double          dFind  = g_numbSortData.dValue;
	int             nMid;
	double          dList;

	while( nLast > nStart+1 )
		{
		nMid    = nStart+(nLast-nStart)/2;
		dList   = pList[nMid].dValue;

		if( dList == dFind )
			return nMid;
		else
			( dFind > dList ) ?	nLast = nMid : nStart = nMid;
		}
	
	dList = pList[nStart].dValue;
	if( dFind >= dList )
		return nStart;
	else
		{
		dList = pList[nLast].dValue;
		return ( dFind < dList ) ? nLast+1 : nLast;
		}
	}
/***************************************************
****************************************************/
int 
ODBCSort::FindRowPlaceAscDTime()
	{
	COleDateTime   dtFind = g_dtSortData.dateTime;
	if( dtFind.GetStatus() != COleDateTime::valid )
		{
		g_dtSortData.dateTime.m_dt = MINIMAL_DATE;
		g_dtSortData.dateTime.SetStatus(COleDateTime::valid);
		return 0;
		}

	if( !g_sortInfo.dwSortedCt ) return 0;
	int            nLast  = g_sortInfo.dwSortedCt-1;
	int            nStart = 0;
	UGDtSortData*  pList  = g_sortInfo.pDtList;
	int            nMid;
	COleDateTime   dtList;

	while( nLast > nStart+1 )
		{
		nMid    = nStart+(nLast-nStart)/2;
		dtList  = pList[nMid].dateTime;
		if( dtList == dtFind ) 
			return nMid;
		else
			( dtList > dtFind ) ? nLast  = nMid : nStart = nMid;
		}
	
	dtList = pList[nStart].dateTime;
	if( dtList >= dtFind ) 
		return nStart;
	else
		{
		dtList = pList[nLast].dateTime;
		return ( dtList < dtFind ) ? nLast+1 : nLast;
		}
	}
/***************************************************
****************************************************/
int 
ODBCSort::FindRowPlaceDescDTime()
	{
	COleDateTime   dtFind = g_dtSortData.dateTime;
	if( dtFind.GetStatus() != COleDateTime::valid )
		{
		g_dtSortData.dateTime.m_dt = MINIMAL_DATE;
		g_dtSortData.dateTime.SetStatus(COleDateTime::valid);
		return g_sortInfo.dwSortedCt;
		}

	if( !g_sortInfo.dwSortedCt ) return 0;
	int            nLast  = (g_sortInfo.dwSortedCt-1);
	int            nStart = 0;
	UGDtSortData*  pList  = g_sortInfo.pDtList;
	int            nMid;
	COleDateTime   dtList;

	while( nLast > nStart+1 )
		{
		nMid    = nStart+(nLast-nStart)/2;
		dtList  = pList[nMid].dateTime;
		if( dtList == dtFind ) 
			return nMid;
		else
			( dtFind > dtList ) ? nLast  = nMid : nStart = nMid;
		}
	
	dtList = pList[nStart].dateTime;
	if( dtFind >= dtList ) 
		return nStart;
	else
		{
		dtList = pList[nLast].dateTime;
		return ( dtFind < dtList ) ? nLast+1 : nLast;
		}
	}
/***************************************************
****************************************************/

int 
CUGSort::FindRowPlaceAscBool()
	{
	if( !g_sortInfo.dwSortedCt ) return 0;
	int             nLast  = g_sortInfo.dwSortedCt-1;
	int             nStart = 0;
	UGBoolSortData* pList  = g_sortInfo.pBoolList;
	BOOL            bFind  = g_boolSortData.bValue;
	int             nMid;
	BOOL            bList;

	while( nLast > nStart+1 )
		{
		nMid    = nStart+(nLast-nStart)/2;
		bList   = pList[nMid].bValue;

		if( bList == bFind )
			return nMid;
		else
			( bList > bFind ) ?	nLast = nMid : nStart = nMid;
		}
	
	bList = pList[nStart].bValue;
	if( bList >= bFind )
		return nStart;
	else
		{
		bList = pList[nLast].bValue;
		return ( bList < bFind ) ? nLast+1 : nLast;
		}
	}
/***************************************************
****************************************************/
int 
CUGSort::FindRowPlaceDescBool()
	{
	if( !g_sortInfo.dwSortedCt ) return 0;
	int             nLast  = g_sortInfo.dwSortedCt-1;
	int             nStart = 0;
	UGBoolSortData* pList  = g_sortInfo.pBoolList;
	BOOL            bFind  = g_boolSortData.bValue;
	int             nMid;
	BOOL            bList;

	while( nLast > nStart+1 )
		{
		nMid    = nStart+(nLast-nStart)/2;
		bList   = pList[nMid].bValue;

		if( bList == bFind )
			return nMid;
		else
			( bFind > bList ) ?	nLast = nMid : nStart = nMid;
		}
	
	bList = pList[nStart].bValue;
	if( bFind >= bList )
		return nStart;
	else
		{
		bList = pList[nLast].bValue;
		return ( bFind < bList ) ? nLast+1 : nLast;
		}
	}
/***************************************************
****************************************************/

