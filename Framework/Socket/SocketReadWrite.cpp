/////////////////////////////////////////
//	class: CSocketReadWrite
//	programmer:	ZqrTalent
//	Date:	1/7/2009
/////////////////////////////////////////

#include "stdafx.h"
#include "SocketReadWrite.h"

UINT __stdcall 
ReadWriteThread(CSocketReadWrite* lpData)
	{
	CString	sError;
	timeval timeout		= {5000, 0};
	char*	pDataRecv	= NULL;
	
	while( TRUE )
		{
		// Initialize select sockets. ########################
		fd_set		sel_recv;
		memset(&sel_recv, 0, sizeof(sel_recv));

		int		nLoop	= 0;
		SOCKET	sockAdd = lpData->GetSocket(nLoop);
		while( sockAdd )
			{
			// add sock to select list.
			FD_SET(sockAdd, &sel_recv);
			nLoop ++;
			sockAdd = lpData->GetSocket(nLoop);
			}
		// ####################################################
	
		int nSockNum = select(0, &sel_recv, NULL, NULL, &timeout);
		if( nSockNum == SOCKET_ERROR )
			{
			int error = WSAGetLastError();
			sError.Format("select error %d", error);
		//	SetLastError(sError);
			continue;
			}

		// Check for recieving data. ############################
		nLoop = 0;
		while( nSockNum > 0 )
			{
			SOCKET sock = lpData->GetSocket(nLoop);
			if( FD_ISSET(sock, &sel_recv) )
				{
				u_long lReadSz = 0;
				ioctlsocket(sock, FIONREAD, &lReadSz);
				if( lReadSz > 0 )
					{
					pDataRecv = (char*)new BYTE[lReadSz];
					if( recv(sock, pDataRecv, lReadSz, 0) != lReadSz )
						delete [] pDataRecv;
					else	// Notify application about recieve.
						{
						RecvDataNotify* pNotify = new RecvDataNotify;
						pNotify->sockFrom		= sock;
						pNotify->lpRecv			= (BYTE*)pDataRecv;
						pNotify->nSize			= (UINT)lReadSz;

						if( !AfxGetApp()->PostThreadMessage(lpData->m_nRecieveNotify, 0, (LPARAM)pNotify) )
							{
							delete pNotify;
							delete [] pDataRecv;
							}
						}
					}
				nSockNum --;
				}
			nLoop ++;
			}
		// ####################################################
		}

	return 0L;
	}

CSocketReadWrite::CSocketReadWrite()
	{
	m_nRecieveNotify = 0L;
	}

CSocketReadWrite::~CSocketReadWrite()
	{
	}

SOCKET
CSocketReadWrite::GetSocket(int nIndex)
	{
	m_section.Lock();
	CDWordArray* pSocks = GetReadWriteSockets();
	if( nIndex >= 0 && nIndex < pSocks->GetCount() )
		{
		SOCKET sock = (SOCKET)pSocks->GetAt(nIndex);
		m_section.Unlock();
		return sock;
		}

	m_section.Unlock();
	return NULL;
	}

BOOL
CSocketReadWrite::AddSocket(SOCKET sock)
	{
	ASSERT( sock );
	m_section.Lock();
	CDWordArray* pSocks = GetReadWriteSockets();

	for( int nLoop=0; nLoop<pSocks->GetCount(); nLoop ++ )	
		{
		if( sock == (SOCKET)pSocks->GetAt(nLoop) )
			{
			m_section.Unlock();
			return FALSE;
			}
		}

	pSocks->Add((DWORD)sock);
	m_section.Unlock();
	return TRUE;
	}

BOOL
CSocketReadWrite::RemoveSocket(SOCKET sock)
	{
	ASSERT( sock );
	m_section.Lock();
	CDWordArray* pSocks = GetReadWriteSockets();

	for( int nLoop=0; nLoop<pSocks->GetCount(); nLoop ++ )	
		{
		if( sock == (SOCKET)pSocks->GetAt(nLoop) )
			{
			pSocks->RemoveAt(nLoop);
			m_section.Unlock();
			return TRUE;
			}
		}
	m_section.Unlock();
	return FALSE;
	}

int		
CSocketReadWrite::SockWrite(SOCKET sock, BYTE* lpData, UINT nSize)
	{
	int nSend = send(sock, (const char*)lpData, nSize, 0);
	if( nSend == SOCKET_ERROR )
		{
		m_sLastError.Format("send error %d", WSAGetLastError());
		}
	return nSend;
	}

BOOL
CSocketReadWrite::SockWrite_WaitFor(SOCKET sock, BYTE* lpData, UINT nSize, 
									BYTE*& lpRead, UINT& nReadSize, UINT nWaitMillsec /*= INFINITE*/ )
	{
	lpRead		= NULL;
	nReadSize	= 0;

	if( SockWrite(sock, lpData, nSize) != nSize )
		return FALSE;

	

	return TRUE;
	}
