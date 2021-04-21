//////////////////////////////////////////////////////
//                                                  //
// started [22.09.2006]                             // 
//                                                  //
// Automatically sorted array...                    //
//                                                  //
// Author:  Zqr                                     //
//                                                  //
//////////////////////////////////////////////////////

#if !defined(AFX_AUTOSORTEDARRAYINT64_H_INCLUDED)
#define      AFX_AUTOSORTEDARRAYINT64_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef PTR_HOLDER_INTEGER
#ifdef _WIN64
	#define PTR_HOLDER_INTEGER __int64
#elif _WIN32
	#define PTR_HOLDER_INTEGER long
#else
	#define PTR_HOLDER_INTEGER __int128
#endif
#endif

// specifies each block size of array in bytes.
static int blockSize64 = 2*sizeof(__int64); 

// disable typecast warnings.
#pragma warning(disable:4311 4312)

class AutoSortedArrayInt64 : public CObject  
{
public:
	AutoSortedArrayInt64 ();
	~AutoSortedArrayInt64();

	__int64	operator[]			(int nIndex); 
	void	Copy				(AutoSortedArrayInt64* src);
	void	Add					(__int64 lValue, __int64 lData  = 0L     );
	void	Set					(int  nIndex, __int64 lValue, __int64 lData = 0L);
	void	DeleteLast			(int  nCount                             );
	void	DeleteAll			();
	void	FreeExtra			();
	void	Delete				(int  nIndex, int  nCount = 1            );
	__int64	Get					(int  nIndex                             );
	__int64	GetData				(int  nIndex                             );
	void	SetData				(int  nIndex, __int64	lData            );
	__int64	GetWithData			(int  nIndex, __int64& lData             );
	long	GetCount			();
	void*	GetBuffer			();

	// Find multiply values. ##################################
	int		FindFirst			(__int64 lValue);
	int		FindNext			();
	// ########################################################

	// finds value into array. returns -1 if not find.
	int		FindValue			(__int64 lValue              );
	long	FindValuePlace		(__int64 lValue, bool& bExist);
	int		IndexOf				(__int64 lValue) { return FindValue(lValue);};
	void	OptimizeForLargeData(BOOL bOptimize);

private:
	void*	m_pBuffer;				// allocated memory.
	long	m_memAlloc;				// allocated memory size.
	long	m_memUsed;				// used memory size.
	long	m_memBlockCt;			// count of memory blocks in array.
	BOOL	m_bOptimizeLargeArray;	// Optimize for large array.
	int		m_nGrowBy;
	HANDLE	m_hHeap;				// Handle of created Heap. (Used only in OptimizeForLargeData mode)
	int		m_nLastFounded;			// Last founded value index.
	__int64	m_lFindValue;			// Find value.

protected:
	void AllocateMemory	(int nNewSize);
	void FreeMemory		();
};

__inline __int64
AutoSortedArrayInt64::operator[](int nIndex) 
	{ 
	return Get(nIndex);
	}

__inline __int64 
AutoSortedArrayInt64::Get(int nIndex)
	{
	ASSERT( nIndex > -1 && nIndex < m_memBlockCt );
	return *(__int64*)((PTR_HOLDER_INTEGER)m_pBuffer + blockSize64*nIndex);
	}

__inline __int64 
AutoSortedArrayInt64::GetData(int  nIndex)
	{
	ASSERT( nIndex > -1 && nIndex < m_memBlockCt );
	return *(__int64*)((PTR_HOLDER_INTEGER)m_pBuffer + blockSize64*nIndex + sizeof(__int64));
	}

__inline __int64 
AutoSortedArrayInt64::GetWithData(int  nIndex, __int64& lData)
	{
	ASSERT( nIndex > -1 && nIndex < m_memBlockCt );
	__int64* pLong  = (__int64*)((PTR_HOLDER_INTEGER)m_pBuffer + blockSize64*nIndex);
	lData			= pLong[1];
	return			pLong[0];	
	}

__inline int  
AutoSortedArrayInt64::FindValue(__int64 lValue)
	{
	bool bExist;
	int nIndex = FindValuePlace(lValue,bExist);
	return bExist ? nIndex : -1;
	}

__inline long   
AutoSortedArrayInt64::GetCount()
	{
	return m_memBlockCt;
	}

__inline void*
AutoSortedArrayInt64::GetBuffer()
	{
	return m_pBuffer;
	}

__inline void  
AutoSortedArrayInt64::DeleteAll()
	{
	m_memUsed	 = 0;
	m_memBlockCt = 0;
	}

__inline void
AutoSortedArrayInt64::FreeExtra()
	{
	FreeMemory();
	}

__inline void		
AutoSortedArrayInt64::OptimizeForLargeData(BOOL bOptimize)
	{
	m_bOptimizeLargeArray = bOptimize;
	}

__inline void	
AutoSortedArrayInt64::SetData(int  nIndex, __int64	lData)
	{
	ASSERT( nIndex > -1 && nIndex < m_memBlockCt );
	__int64* pLong  = (__int64*)((PTR_HOLDER_INTEGER)m_pBuffer + blockSize64*nIndex);
	pLong[1]		= lData;
	}

#endif // !defined(AFX_AUTOSORTEDARRAYINT64_H_INCLUDED)

