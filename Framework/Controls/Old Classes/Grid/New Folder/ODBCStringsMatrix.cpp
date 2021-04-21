// ODBCStringsMatrix.cpp : implementation file

#include "stdafx.h"
#include "..\iODBCRst.h"
#include "..\AutoSortedArray.h"
#include "ODBCStringsMatrix.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Compare Function.

// Georgian symbol codes alphabetically.
const BYTE btGeoAlphabetical[] =
	{
	0xf4, 0xe8, 0xef, 0xe2, 0xf3, 0xec, 0xff, 
	0xc5, 0xf8, 0xeb, 0xe4, 0xfc, 0xf2, 0xf9, 
	0xe7, 0xce, 0xea, 0xfb, 0xe5, 0xe3, 0xe0, 
	0xe9, 0xca, 0xed, 0xdb, 0xd1, 0xf1, 0xdf, 
	0xf6, 0xd6, 0xfa, 0xee, 0xf0
	};

int	
CompareStrings_Geo(LPCTSTR lpsz1, LPCTSTR lpsz2)
	{
	int nLoop	= 0;
	int nIndex1, nIndex2;

	int* nArrSymbolIdx = ODBCStringsMatrix::GetSymbolIndexesArray();

	while( lpsz1[nLoop] != '\0' || lpsz2[nLoop] != '\0' )
		{
		nIndex1 = nArrSymbolIdx[lpsz1[nLoop]&0xFF];
		nIndex2 = nArrSymbolIdx[lpsz2[nLoop]&0xFF];

		if( nIndex1 != nIndex2 )
			return (nIndex1 > nIndex2) ? 1 : -1;

		nLoop ++;
		}

	if( lpsz1[nLoop] == '\0' && 
		lpsz2[nLoop] == '\0' )
		return 0;

	return lpsz1[nLoop] == '\0' ? -1 : 1;
	}

int 
CompareArrayItems(void* lpValue1, void* lpValue2)
	{
	int nLoop	= 0;
	int nIndex1, nIndex2;
	LPCTSTR lpsz1 = (LPCTSTR)lpValue1, lpsz2 = (LPCTSTR)lpValue2;

	int* nArrSymbolIdx = ODBCStringsMatrix::GetSymbolIndexesArray();

	while( lpsz1[nLoop] != '\0' || lpsz2[nLoop] != '\0' )
		{
		nIndex1 = nArrSymbolIdx[lpsz1[nLoop]&0xFF];
		nIndex2 = nArrSymbolIdx[lpsz2[nLoop]&0xFF];

		if( nIndex1 != nIndex2 )
			return (nIndex1 > nIndex2) ? 1 : -1;

		nLoop ++;
		}

	if( lpsz1[nLoop] == '\0' && 
		lpsz2[nLoop] == '\0' )
		return 0;

	return lpsz1[nLoop] == '\0' ? -1 : 1;
	}

int
CompareEmpty(void*, void*)
	{
	return -1;
	}

/////////////////////////////////////////////////////////////////////////////
// ODBCStringsMatrix

ODBCStringsMatrix::ODBCStringsMatrix()
	{
	m_compareProc		= (CompareStrings_Proc)(strcmp);
	m_hHeap				= NULL;
	m_nHeapAllocSize	= 0L;
	m_nHeapUsedSize		= 0L;

	m_pStringBuffer		= NULL;
	m_nOffset			= 0;
	m_nBuffSize			= 0;
	m_nColFields		= 0;

	m_arrSearchResult	.OptimizeForLargeData	(TRUE);
	m_arrStrings		.OptimizeForLargeData	(TRUE);
	m_arrStrings		.SetCompareFunction		(CompareArrayItems);
	m_arrFullStrings	.OptimizeForLargeData	(TRUE);
	Initialize();
	}

ODBCStringsMatrix::~ODBCStringsMatrix()
	{
	FreeStringsArray();
	// Destroy heap. ##############################
	if( m_hHeap )
		{
		HeapFree	(m_hHeap, 0L, m_pBuffer);
		HeapDestroy	(m_hHeap);
		m_hHeap = NULL;
		}
	// ############################################
	}

void
ODBCStringsMatrix::Initialize()
	{
	m_compareProc = (CompareStrings_Proc)(CompareStrings_Geo);
	InitSymbolIndexArray();
	}

void 
ODBCStringsMatrix::InitSymbolIndexArray()
	{
	int* nArrSymbolIdx = ODBCStringsMatrix::GetSymbolIndexesArray();
	if( nArrSymbolIdx['è'] == 301 )
		return;

	memset(nArrSymbolIdx, 0, 255*sizeof(int));
	for( int nLoop=0; nLoop<sizeof(btGeoAlphabetical); nLoop ++ )
		nArrSymbolIdx[btGeoAlphabetical[nLoop]]	= 300 + nLoop;

	for( int nLoop=0; nLoop<255; nLoop++ )
		{
		if( nArrSymbolIdx[nLoop] == 0 )
			nArrSymbolIdx[nLoop] = nLoop;
		}
	}

int		
ODBCStringsMatrix::LoadFromRst2(CRecord* pRecord, AutoSortedArray* pArrIdToRecData, 
								int nDataId, int* pStringIds, int nCount)
	{
	FreeStringsArray();
	if( pArrIdToRecData->GetCount() <= 0 )
		return 0;

	CString		sValue;
	m_nColFields			= nCount;
	long		lData		= 0;
	int			nLoop		= 0, nIndex = 0, nItem = 0, nAddSize = 0, nRecCount = pArrIdToRecData->GetCount();
	int			nSize		= 0, nSizeOffsets = (nCount + 1)*sizeof(DWORD);
	char**		pStrOffsets	= (char**)new DWORD[nCount + 1];		

	// Destroy heap. ##############################
	if( m_hHeap )
		{
		HeapFree	(m_hHeap, 0L, m_pBuffer);
		HeapDestroy	(m_hHeap);
		m_hHeap				= NULL;
		m_nHeapAllocSize	= 0;
		m_nHeapUsedSize		= 0;
		}
	// ############################################

	// Create and allocate heap. ##################
	m_nHeapAllocSize	= nSizeOffsets*nRecCount;
	if( m_nHeapAllocSize > 0 )
		{
		m_hHeap				= HeapCreate(0L, m_nHeapAllocSize, 0L);
		ASSERT( m_hHeap );
		m_pBuffer			= (BYTE*)HeapAlloc(m_hHeap, 0L, m_nHeapAllocSize);
		m_nHeapUsedSize		= 0;
		}
	// ############################################

	while( nLoop < nRecCount )
		{
		pStrOffsets[0]	= (char*)pArrIdToRecData->GetData(nLoop);
		pRecord->SetRecordData((void*)pStrOffsets[0]);

		int nLoop1		= 2;
		lData			= pRecord->GetLong(nDataId);
		// Load field strings and build offsets array.
		sValue			= pRecord->GetString(pStringIds[0]);
		pStrOffsets[1]	= (char*)pRecord->GetBindedValuePtr(pStringIds[0]);
		while( nLoop1 <= nCount )
			{
			sValue	+= _T(" ");
			pStrOffsets[nLoop1]	= (char*)pRecord->GetBindedValuePtr(pStringIds[nLoop1 - 1]);
			sValue	+= pStrOffsets[nLoop1];
			nLoop1	++;
			}
		// ########################################
		// Copy offsets array. ####################
		memcpy(&m_pBuffer[m_nHeapUsedSize], pStrOffsets, nSizeOffsets);
		// Add offsets array offset to sorted array.
		m_arrFullStrings.Add(lData, (long)m_nHeapUsedSize);
		// Change memory buffer use size. #########
		m_nHeapUsedSize += nSizeOffsets;
		// #######################################
		nLoop ++; nSize += sValue.GetLength() + 1; 
		}
	// ###########################################
	
	// Add full strings(sorted order) to array.##
	m_pStringBuffer = new BYTE[nSize];
	m_nOffset		= 0;
	m_nBuffSize		= nSize;

	int	nLoop1 = 0;
	while( nItem < nRecCount )
		{
		lData			= (long)m_arrFullStrings[nItem];
		void* lpRecData = (void*)((DWORD*)&m_pBuffer[m_arrFullStrings.GetData(nItem)])[0];
		nLoop1			= 0;
		pRecord->SetRecordData(lpRecData);

		while( nLoop1 < nCount )
			{
			sValue	= pRecord->GetString(pStringIds[nLoop1]);
			// Add string(cutted to small parts) into buffer and array. ##
			AddStringToBufferArray(&sValue, lData);
			// ###########################################################
			nLoop1 ++;
			}
		nItem ++;
		}
	// ###########################################

	m_arrSearchResult.Copy(&m_arrFullStrings);
	delete pStrOffsets;
	pRecord->SetRecordData(NULL);
	return nLoop;
	}

int
ODBCStringsMatrix::LoadFromRst(CODBCRecordset* pRst, int nDataId, int* pStringIds, int nCount)
	{
	FreeStringsArray();
	if( !pRst->IsBOF() )
		pRst->MoveFirst();

	m_nColFields		= nCount;
const int	nHeapBlock	= 2048;
	long	lData		= 0;
	int		nLoop		= 0, nIndex = 0, nItem = 0, nAddSize;
	int		nSize		= 0, nSizeOffsets = (nCount + 1)*sizeof(int);
	WORD*	pStrOffsets	= new WORD[nCount*2]; // (WORD)[start offset](WORD)[end offset].

	// Create and allocate heap. ####################
	if( !m_hHeap )
		{
		m_hHeap				= HeapCreate(0L, nHeapBlock, 0L);
		ASSERT( m_hHeap );
		m_pBuffer			= (BYTE*)HeapAlloc(m_hHeap, 0L, nHeapBlock);
		m_nHeapAllocSize	= nHeapBlock;
		m_nHeapUsedSize		= 0;
		}
	else
		m_nHeapUsedSize		= 0;
	// ###############################################

	// Fill full strings array. ##################
	CString sValue;
	while( !pRst->IsEOF() )
		{
		int nLoop1		= 2;
		lData			= pRst->GetLong(nDataId);
		// Load field strings and build offsets array.
		sValue			= pRst->GetString(pStringIds[0]);
		pStrOffsets[2]	= 0;
		pStrOffsets[3]	= (WORD)sValue.GetLength();
		while( nLoop1 <= nCount )
			{
			pStrOffsets[nLoop1*2]		= pStrOffsets[nLoop1*2 - 1] + 1;
			sValue	+= _T(" ");
			sValue	+= pRst->GetString(pStringIds[nLoop1 - 1]);
			pStrOffsets[nLoop1*2 + 1]	= (WORD)sValue.GetLength();
			nLoop1	++;
			}
		// ########################################
		// Reallocate heap memory. ################
		nAddSize = nSizeOffsets + sValue.GetLength() + 1;
		if( m_nHeapUsedSize + nAddSize > m_nHeapAllocSize )
			{
			m_pBuffer			= (BYTE*)HeapReAlloc(m_hHeap, 0L, m_pBuffer, m_nHeapAllocSize + nHeapBlock);
			m_nHeapAllocSize	+= nHeapBlock;
			ASSERT( m_pBuffer );
			}
		// #######################################
		// Add offsets array at the head of string.
		memcpy(&m_pBuffer[m_nHeapUsedSize], pStrOffsets, nSizeOffsets);
		m_nHeapUsedSize += nSizeOffsets;
		// #######################################
		// Add full string to array. #############
		char* pFullString = (char*)&m_pBuffer[m_nHeapUsedSize];
		strcpy(pFullString, sValue.GetBuffer());
		m_arrFullStrings.Add(lData, (long)m_nHeapUsedSize);
		m_nHeapUsedSize += sValue.GetLength() + 1;
		// #######################################
		pRst->MoveNext(); nLoop ++; nSize += sValue.GetLength() + 1; 
		}
	// ###########################################
	
	// Add full strings(sorted order) to array.##
	m_pStringBuffer = new BYTE[nSize];
	m_nOffset		= 0;
	m_nBuffSize		= nSize;

	while( nItem < nLoop )
		{
		lData	= (long)m_arrFullStrings[nItem];
		sValue	= ((LPCTSTR)&m_pBuffer[m_arrFullStrings.GetData(nItem)]);
		// Add string(cutted to small parts) into buffer and array. ##
		AddStringToBufferArray(&sValue, lData);
		// ###########################################################
		nItem ++;
		}
	// ###########################################

	pRst->Close();
	m_arrSearchResult.Copy(&m_arrFullStrings);
	delete pStrOffsets;
	return nLoop;
	}

int		
ODBCStringsMatrix::FindStrings(LPCTSTR szStringSearch, AutoSortedArray* pArrDatas, AutoSortedArray* pArrDatas1)
	{
	CString	sStrSearch		= szStringSearch;
	bool	bExists			= false;
	int		nCount			= m_arrStrings.GetCount();
	
	int		nFind			= m_arrStrings.FindValuePlace((long)sStrSearch.GetBuffer(), bExists);
	ASSERT( nFind > -1 );
	if( nFind >= m_arrStrings.GetCount() )
		return 0L;

	int				nLoop			= nFind;
	CString			sPrevString;
	
	while( nLoop > -1 )
		{
		// Compare founded string. ################################
		sPrevString = (LPCTSTR)m_arrStrings.Get(nLoop);
		if( sPrevString.GetLength() < sStrSearch.GetLength() )
			break;
		sPrevString = sPrevString.Left(sStrSearch.GetLength());
		if( CompareStrings_Geo(sStrSearch.GetBuffer(), sPrevString.GetBuffer()) )
			break;
		// ########################################################

		// Find full string and store it into array. ##############
		long	lData	= m_arrStrings.GetData(nLoop);
		int		nIndex	= m_arrFullStrings.FindValue(lData);
		if( nIndex != -1 )
			{
			int nFind1 = pArrDatas->FindValue(lData);
			if( pArrDatas == pArrDatas1 )
				{
				if( nFind1 == -1 )
					pArrDatas->Add(lData, m_arrFullStrings.GetData(nIndex));
				}
			else
				{
				if( nFind1 != -1 )
					pArrDatas1->Add(lData, m_arrFullStrings.GetData(nIndex));
				}
			}
		// ##########################################################
		nLoop --;
		}

	nLoop			= nFind + 1;
	while( nLoop < nCount )
		{
		// Compare founded string. ################################
		sPrevString = (LPCTSTR)m_arrStrings.Get(nLoop);
		if( sPrevString.GetLength() < sStrSearch.GetLength() )
			break;
		sPrevString = sPrevString.Left(sStrSearch.GetLength());
		if( CompareStrings_Geo(sStrSearch.GetBuffer(), sPrevString.GetBuffer()) )
			break;
		// ########################################################

		// Find full string and store it into array. ##############
		long	lData	= m_arrStrings.GetData(nLoop);
		int		nIndex	= m_arrFullStrings.FindValue(lData);
		if( nIndex != -1 )
			{
			int nFind1 = pArrDatas->FindValue(lData);
			if( pArrDatas == pArrDatas1 )
				{
				if( nFind1 == -1 )
					pArrDatas->Add(lData, m_arrFullStrings.GetData(nIndex));
				}
			else
				{
				if( nFind1 != -1 )
					pArrDatas1->Add(lData, m_arrFullStrings.GetData(nIndex));
				}
			}
		// ########################################################
		nLoop ++;
		}

	return (pArrDatas == pArrDatas1) ? pArrDatas->GetCount() : pArrDatas1->GetCount();
	}

int		
ODBCStringsMatrix::FindStringsExact(LPCTSTR szStringSearch, AutoSortedArray* pArrDatas, AutoSortedArray* pArrDatas1)
	{
	CString	sStrSearch		= szStringSearch;
	bool	bExists			= false;
	int		nCount			= m_arrStrings.GetCount();
	
	int		nFind			= m_arrStrings.FindValuePlace((long)sStrSearch.GetBuffer(), bExists);
	ASSERT( nFind > -1 );
	if( nFind >= m_arrStrings.GetCount() )
		return 0L;

	int				nLoop			= nFind;
	CString			sPrevString;
	
	while( nLoop > -1 )
		{
		// Compare founded string. ################################
		sPrevString = (LPCTSTR)m_arrStrings.Get(nLoop);
		if( sPrevString.GetLength() != sStrSearch.GetLength() )
			break;
		if( CompareStrings_Geo(sStrSearch.GetBuffer(), sPrevString.GetBuffer()) )
			break;
		// ########################################################

		// Find full string and store it into array. ##############
		long	lData	= m_arrStrings.GetData(nLoop);
		int		nIndex	= m_arrFullStrings.FindValue(lData);
		if( nIndex != -1 )
			{
			int nFind1 = pArrDatas->FindValue(lData);
			if( pArrDatas == pArrDatas1 )
				{
				if( nFind1 == -1 )
					pArrDatas->Add(lData, m_arrFullStrings.GetData(nIndex));
				}
			else
				{
				if( nFind1 != -1 )
					pArrDatas1->Add(lData, m_arrFullStrings.GetData(nIndex));
				}
			}
		// ########################################################
		nLoop --;
		}

	nLoop			= nFind + 1;
	while( nLoop < nCount )
		{
		// Compare founded string. ################################
		sPrevString = (LPCTSTR)m_arrStrings.Get(nLoop);
		if( sPrevString.GetLength() != sStrSearch.GetLength() )
			break;
		if( CompareStrings_Geo(sStrSearch.GetBuffer(), sPrevString.GetBuffer()) )
			break;
		// ########################################################

		// Find full string and store it into array. ##############
		long	lData	= m_arrStrings.GetData(nLoop);
		int		nIndex	= m_arrFullStrings.FindValue(lData);
		if( nIndex != -1 )
			{
			int nFind1 = pArrDatas->FindValue(lData);
			if( pArrDatas == pArrDatas1 )
				{
				if( nFind1 == -1 )
					pArrDatas->Add(lData, m_arrFullStrings.GetData(nIndex));
				}
			else
				{
				if( nFind1 != -1 )
					pArrDatas1->Add(lData, m_arrFullStrings.GetData(nIndex));
				}
			}
		// ########################################################
		nLoop ++;
		}

	return (pArrDatas == pArrDatas1) ? pArrDatas->GetCount() : pArrDatas1->GetCount();
	}

int
ODBCStringsMatrix::SearchStrings(LPCTSTR szStringSearch)
	{
	// Cut search string to small pieces. #####################
	CStringArray	arrSmallStrings;
	CutToSmallStrings((char*)szStringSearch, arrSmallStrings);
	int				nSmallStrings	= (int)arrSmallStrings	.GetCount();
	// ########################################################

	CString			sStrSearch		= arrSmallStrings.GetCount() > 0 ? arrSmallStrings[0] : szStringSearch;
	// Reload all strings. ####################################
	if( !sStrSearch.GetLength() || arrSmallStrings.GetCount() == 0 )
		{
		m_arrSearchResult.Copy(&m_arrFullStrings);
		return m_arrSearchResult.GetCount();
		}
	// ########################################################

	// Array of datas to solve duplicated items problem. ##########
	AutoSortedArray	arrItemDatas, arrItemDatas1;
	// ############################################################
	int nCount = FindStrings(sStrSearch.GetBuffer(), &arrItemDatas, &arrItemDatas);
	if( nCount <= 0 )
		{
		m_arrSearchResult.DeleteAll();
		return 0L;
		}
	
	int				nLoop	= 1;
	AutoSortedArray *pArr	= &arrItemDatas1, *pArr1 = &arrItemDatas;
	while( nLoop < nSmallStrings )
		{
		// Set up search string. #######################################
		sStrSearch				= arrSmallStrings[nLoop];
		// Change pointers. ############################################
		AutoSortedArray* pTmp	= pArr;
		pArr					= pArr1;
		pArr1					= pTmp;
		// Free array datas. ###########################################
		pArr1->DeleteAll();
		// #############################################################

		if( nLoop == nSmallStrings - 1 )
			FindStrings(sStrSearch.GetBuffer(), pArr, pArr1);
		else
			FindStringsExact(sStrSearch.GetBuffer(), pArr, pArr1);

		// Nothing were found!!! ######################################
		if( pArr1->GetCount() == 0 )
			break;
		// ############################################################
		nLoop ++;
		}

	// Fill search result array list. ##################################
	nCount	= pArr1->GetCount();
	nLoop	= 0;
	if( nCount > 0 )
		m_arrSearchResult.Copy(pArr1);
	// ################################################################
	return nCount;
	}

int
ODBCStringsMatrix::CutToSmallStrings(char* szString, CStringArray& arrSmallStrings)
	{
	int nLoop	= 0;
	int	nStart	= 0;
	arrSmallStrings.RemoveAll();

	while( szString[nLoop] != '\0' )
		{
		if( szString[nLoop] == ' ' )
			{
			char	cOld	= szString[nLoop];
			szString[nLoop]	= '\0';
			CString sValue	= (LPCTSTR)&szString[nStart];
			szString[nLoop]	= cOld;

			if( sValue.GetLength() )
				arrSmallStrings.Add(sValue);
			nStart			= nLoop + 1;
			}

		nLoop ++;
		}

	CString sValue	= (LPCTSTR)&szString[nStart];
	sValue.Remove(' ');
	if( sValue.GetLength() )
		arrSmallStrings.Add(sValue);

	return (long)arrSmallStrings.GetCount();
	}

void
ODBCStringsMatrix::FreeStringsArray()
	{
	// Empty memory buffer of small strings. ######
	if( m_pStringBuffer )
		delete m_pStringBuffer;

	m_pStringBuffer	= NULL;
	m_nOffset		= 0;
	m_nBuffSize		= 0;
	// ############################################

	m_nHeapUsedSize	= 0;
	m_arrFullStrings	.DeleteAll();
	m_arrStrings		.DeleteAll();
	m_arrSearchResult	.DeleteAll();
	}

void
ODBCStringsMatrix::AddStringToBufferArray(CString* pString, long lData)
	{
	char*	pszString	= pString->GetBuffer();
	int		nLoop		= 0;
	int		nStart		= 0;
	int		nLen;
	char*	pszStringNew;

	while( pszString[nLoop] != '\0' )
		{
		if( pszString[nLoop] == ' ' )
			{
			pszString[nLoop]	= '\0';
			if( nStart < nLoop )
				{
				nLen			= nLoop - nStart + 1;
				pszStringNew	= (char*)&m_pStringBuffer[m_nOffset];
				m_nOffset		+= nLen;
				// Copy string.##############################
				strcpy(pszStringNew, &pszString[nStart]);
				/*
				// Copy start index.#########################
				*(int*)(&pszStringNew[nLen]) = nStart;
				*/
				// Add string to array.######################
				m_arrStrings.Add((long)(pszStringNew), lData);
				}
			pszString[nLoop]	= ' ';
			nStart				= nLoop + 1;
			}
		nLoop ++;
		}

	if( nStart < nLoop )
		{
		nLen			= nLoop - nStart + 1;
		pszStringNew	= (char*)&m_pStringBuffer[m_nOffset];
		m_nOffset		+= nLen;
		// Copy string.##############################
		strcpy(pszStringNew, &pszString[nStart]);
		/*
		// Copy start index.#########################
		*(int*)(&pszStringNew[nLen]) = nStart;
		*/
		// Add string to array.######################
		m_arrStrings.Add((long)(pszStringNew), lData);
		}
	}

BOOL
ODBCStringsMatrix::GetResultString(int nRow, int nCol, CString& sResult)
	{
	ASSERT( nRow > -1 && nCol > -1 );
	if( nRow < m_arrSearchResult.GetCount() && nCol < m_nColFields )
		{
		nCol ++;
		long	lStr		= (long)&m_pBuffer[m_arrSearchResult.GetData(nRow)];
		int*	pOffsets	= (int*)(lStr - sizeof(int)*(m_nColFields + 1));
		int		nStart		= ((WORD*)pOffsets)[nCol*2];
		int		nEnd		= ((WORD*)pOffsets)[nCol*2 + 1];
		int		nLen		= nEnd - nStart;
		if( nLen > 0 )
			{
			static	char	szResult[255];
			memcpy(szResult, &((char*)lStr)[nStart], nLen);
			szResult[nLen]	= '\0';
			sResult			= szResult;
			}
		else
			sResult = "";
		return TRUE;
		}
	return FALSE;
	}

BOOL
ODBCStringsMatrix::GetResultString2(int nRow, int nCol, CString& sResult)
	{
	ASSERT( nRow > -1 && nCol > -1 );
	if( nRow < m_arrSearchResult.GetCount() && nCol < m_nColFields )
		{
		nCol ++;
		long	lOffset		= m_arrSearchResult.GetData(nRow);
		char*	pResult		= ((char**)(&m_pBuffer[lOffset]))[nCol];
		
		if( pResult )
			sResult			= pResult;
		else
			sResult = "";
		return TRUE;
		}
	return FALSE;
	}