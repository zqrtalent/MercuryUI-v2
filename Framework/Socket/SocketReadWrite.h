/////////////////////////////////////////
//	class: CSocketReadWrite
//	programmer:	ZqrTalent
//	Date:	1/7/2009
/////////////////////////////////////////

#ifndef _WINSOCK2API_
	#include <winsock2.h>
#endif

#ifndef __AFXMT_H__
	#include <afxmt.h>
#endif

struct RecvDataNotify{
	SOCKET	sockFrom;
	BYTE*	lpRecv;
	UINT	nSize;
	};

class CSocketReadWrite;

UINT __stdcall ReadWriteThread(CSocketReadWrite* lpData);

class CSocketReadWrite
{
public:
	CSocketReadWrite();
	~CSocketReadWrite();

protected:
	static CDWordArray*			GetReadWriteSockets();
public:
	static CSocketReadWrite*	GetInstance();

public:

	
	SOCKET		GetSocket			(int nIndex);
	BOOL		AddSocket			(SOCKET sock);
	BOOL		RemoveSocket		(SOCKET sock);

	int			SockWrite			(SOCKET sock, BYTE* lpData, UINT nSize);
	BOOL		SockWrite_WaitFor	(SOCKET sock, BYTE* lpData, UINT nSize, BYTE*& lpRead, 
									UINT& nReadSize, UINT nWaitMillsec = INFINITE);
	CString&	GetLastError		();


protected:
	CCriticalSection	m_section;
	CString				m_sLastError;

public:
	UINT				m_nRecieveNotify;
};

__inline CString&	
CSocketReadWrite::GetLastError(){
	return m_sLastError;
	}

__inline CSocketReadWrite*	
CSocketReadWrite::GetInstance(){
	static CSocketReadWrite instance;
	return &instance;
	}

__inline CDWordArray* 
CSocketReadWrite::GetReadWriteSockets(){
	static CDWordArray socks;
	return &socks;
	}