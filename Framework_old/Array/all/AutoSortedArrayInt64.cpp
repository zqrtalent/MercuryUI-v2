//////////////////////////////////////////////////////
//                                                  //
// started [22.09.2006]                             // 
//                                                  //
// Automatically sorted array...                    //
//                                                  //
// Author:  Zqr                                     //
//                                                  //
//////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoSortedArrayInt64.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AutoSortedArrayInt64::AutoSortedArrayInt64()
	{
	m_pBuffer				= NULL;
	m_memAlloc				= 0L;
	m_memUsed				= 0L;
	m_memBlockCt			= 0L;
	m_bOptimizeLargeArray	= FALSE;
	m_nGrowBy				= 10;
	m_hHeap					= NULL;
	}

AutoSortedArrayInt64::~AutoSortedArrayInt64()
	{
	FreeMemory();
	}

void  
AutoSortedArrayInt64::Copy(AutoSortedArrayInt64* src)
	{
	FreeMemory();
	int nCount       = src->GetCount();
	if( nCount )
		{
		AllocateMemory(nCount*blockSize64);
		m_memUsed    = nCount*blockSize64;
		m_memBlockCt = nCount;
		memcpy(m_pBuffer, src->GetBuffer(), m_memUsed);
		}
	}

void 
AutoSortedArrayInt64::Add(__int64 lValue, __int64 lData)
	{
	// allocate memory.////////////////////////////////////////
	if( m_memAlloc == m_memUsed )
		AllocateMemory(m_memAlloc+m_nGrowBy*blockSize64);
	//---------------------------------------------------------
	// find insert place(index) of value & insert it. /////////
	bool bExist;
	int nInsert  = FindValuePlace(lValue, bExist);
	void* dest   = (void*)((PTR_HOLDER_INTEGER)m_pBuffer + blockSize64*nInsert);
	long  size   = blockSize64*(m_memBlockCt-nInsert);
	if( size > 0 )
		memmove((void*)((PTR_HOLDER_INTEGER)dest+blockSize64),dest,size);
	//---------------------------------------------------------
	// add new values into memory./////////////////////////////
	__int64* pLong  = (__int64*)dest;
	pLong[0]		= lValue;
	pLong[1]		= lData;
	m_memUsed		+= blockSize64; 
	m_memBlockCt	++;
	//---------------------------------------------------------
	//*******************************************************//
	}

void 
AutoSortedArrayInt64::Set(int nIndex, __int64 lValue, __int64 lData)
	{
	ASSERT( nIndex > -1 && nIndex < m_memBlockCt );
	// find place of new value. //////////////////////////////////////
	bool bExist;
	int  nInsert   = FindValuePlace(lValue, bExist);
	//----------------------------------------------------------------
	// correct insert index of new value. ////////////////////////////
	__int64 lOldValue = Get(nIndex);
	if( lValue > lOldValue ) nInsert --;
	//----------------------------------------------------------------
	// remove old block from array.///////////////////////////////////
	void* dest     = (void*)((PTR_HOLDER_INTEGER)m_pBuffer + blockSize64*(nInsert+1));
	void* src      = (void*)((PTR_HOLDER_INTEGER)m_pBuffer + blockSize64*(nIndex+1));
	long  size     = (PTR_HOLDER_INTEGER)dest-(PTR_HOLDER_INTEGER)src;
	if( size > 0 )
		memmove((void*)((PTR_HOLDER_INTEGER)src-blockSize64),src,size);
	if( size < 0 )
		memmove(dest,(void*)((PTR_HOLDER_INTEGER)dest-blockSize64),-size);
	//-----------------------------------------------------------------
	// set new element into array. ////////////////////////////////////
	__int64* pLong		= (__int64*)((PTR_HOLDER_INTEGER)dest-blockSize64);
	pLong[0]			= lValue;
	pLong[1]			= lData;
	//-----------------------------------------------------------------
	//***************************************************************//
	}

void 
AutoSortedArrayInt64::Delete(int  nIndex, int  nCount)
	{
	ASSERT( nIndex > -1 && nIndex < m_memBlockCt );
	ASSERT( (nIndex+nCount) <= m_memBlockCt      );

	// delete memory blocks from array./////////////////////////
	void* dest    = (void*)((PTR_HOLDER_INTEGER)m_pBuffer + blockSize64*nIndex);
	long  size    = blockSize64*(m_memBlockCt-nIndex-nCount);
	if( size )
		memmove(dest,(void*)((PTR_HOLDER_INTEGER)dest+blockSize64*nCount),size);
	m_memBlockCt -= nCount;
	m_memUsed     = m_memBlockCt*blockSize64;
	//----------------------------------------------------------
	//********************************************************//
	}

void 
AutoSortedArrayInt64::DeleteLast(int nCount)
	{
	// change count of memory blocks.//
	ASSERT( nCount <= m_memBlockCt );
	m_memBlockCt   -= nCount;
	m_memUsed       = m_memBlockCt*blockSize64;
	//---------------------------------
	//*******************************//
	}

long 
AutoSortedArrayInt64::FindValuePlace(__int64 lValue, bool& bExist)
	{
	/////////////////////////////////////////////////////////
	bExist                = false;
	if( !m_memBlockCt ) return  0;

	int            nLast  = m_memBlockCt-1;
	int            nStart = 0;
	int            nMid;
	__int64        lMid;

	while( nLast > nStart+1 )
		{
		nMid = nStart+(nLast-nStart)/2;
		lMid = *(__int64*)((PTR_HOLDER_INTEGER)m_pBuffer+nMid*blockSize64);

		if( lMid == lValue )
			{
			bExist = true;
			return   nMid;
			}
		else
			( lMid > lValue ) ?	nLast = nMid : nStart = nMid;
		}
	
	lMid = *(__int64*)((PTR_HOLDER_INTEGER)m_pBuffer+nStart*blockSize64);
	if( lMid >= lValue )
		{
		bExist = (lMid == lValue);
		return nStart;
		}
	else
		{
		lMid = *(__int64*)((PTR_HOLDER_INTEGER)m_pBuffer+nLast*blockSize64);
		bExist = (lMid == lValue);
		return ( lMid < lValue ) ? nLast+1 : nLast;
		}
	//--------------------------------------------------------
	//********************************************************
	}

int		
AutoSortedArrayInt64::FindFirst(__int64 lValue)
	{
	int nIndex		= FindValue(lValue);
	m_nLastFounded	= -1;
	m_lFindValue	= lValue;

	if( nIndex != -1 )
		{
		while( nIndex > 0 )
			{
			nIndex --;
			if( Get(nIndex) != lValue )
				{
				m_nLastFounded = nIndex + 1;
				return nIndex + 1;
				}
			}

		m_nLastFounded = 0;
		return 0;
		}

	m_nLastFounded	= -1;
	m_lFindValue	= -1;
	return -1;
	}

int		
AutoSortedArrayInt64::FindNext()
	{
	ASSERT( m_nLastFounded > -1 );
	if( m_nLastFounded == GetCount() - 1 )
		{
		m_nLastFounded	= -1;
		m_lFindValue	= -1;
		return -1;
		}

	if( Get(m_nLastFounded + 1) == m_lFindValue )
		{
		m_nLastFounded ++;
		return m_nLastFounded;
		}
	
	m_nLastFounded	= -1;
	m_lFindValue	= -1;
	return -1;
	}

void 
AutoSortedArrayInt64::AllocateMemory(int nNewSize)
	{
	if( !m_bOptimizeLargeArray )
		{
		// allocate memory if needed.///////////
		void* pTemp   = new BYTE[nNewSize];
		// copy & free old allocated memory.////
		if( m_pBuffer )
			{
			memcpy(pTemp, m_pBuffer, m_memAlloc);
			delete m_pBuffer;
			}
		//--------------------------------------
		m_pBuffer     = pTemp;
		m_memAlloc    = nNewSize;
		ASSERT( m_pBuffer );
		// ####################################
		}
	else
		{
		if( !m_hHeap )
			{
			m_hHeap = HeapCreate(0L, nNewSize, 0L);
			ASSERT( m_hHeap );
			m_pBuffer = HeapAlloc(m_hHeap, 0L, nNewSize);
			}

		// Allocate or realocate memory . #####
		m_pBuffer	= HeapReAlloc(m_hHeap, 0L, m_pBuffer, nNewSize);
		m_memAlloc  = nNewSize;
		ASSERT( m_pBuffer );
		// ####################################
		}
	}

void 
AutoSortedArrayInt64::FreeMemory()
	{
	// free allocated memory.//
	if( m_pBuffer )
		{
		if( m_bOptimizeLargeArray )
			{
			HeapFree	(m_hHeap, 0L, m_pBuffer);
			HeapDestroy	(m_hHeap);
			}
		else
			delete m_pBuffer;
		}

	m_hHeap		 = NULL;
	m_pBuffer    = NULL;
	m_memAlloc   = 0L;
	m_memUsed    = 0L;
	m_memBlockCt = 0L;
	//-------------------------
	//***********************//
	}
