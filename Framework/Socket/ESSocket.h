#pragma once

#ifndef _WINSOCK2API_
	#include <winsock2.h>
#endif

#ifndef __AFXMT_H__
	#include <afxmt.h>
#endif

#include "..\Array\StringToStringArray.h"
#include "..\Array\List.h"

/////////////////////////////////////////
//	class: CSocketListen
//	programmer:	ZqrTalent
//	Date:	30/6/2009
/////////////////////////////////////////

class ESSocket;

typedef void (__thiscall CObject::*ReceiveDataProc)			(BYTE* lpData, UINT nSize);
typedef void (__thiscall CObject::*AcceptProc)				(ESSocket* pAccepted, BOOL& bAccept);
typedef void (__thiscall CObject::*DisconnectedProc)		(ESSocket* pSockFrom);

class ESSocket
{
friend class ESSocketMan;

protected:
	ESSocket();

public:
	~ESSocket();

	enum SocketStatus
	{
		Connected,
		Binded,
		Closed
	};

	BOOL			Connect					(short shPort, const CString sIpAddress);
	BOOL			Bind					(short shPort, CString sIpAddress = _T(""));
	int				Send					(BYTE* lpBytesSend, UINT nSize);
	BOOL			Accept					(SOCKET sockAccept);
	void			Disconect				();

	// Diagnostic methods.
	BOOL			IsConnected				(){return (m_sockStat == Connected);};
	BOOL			IsListening				(){return (m_sockStat == Binded);};
	BOOL			IsClosed				(){return (m_sockStat == Closed);};
	BOOL			GetStatus				(){return m_sockStat;};

	// Event notifiers.
	void			SetReceiveDataNotifier	(ReceiveDataProc proc, void* pClassObject);
	void			SetAcceptNotifier		(AcceptProc proc, void* pClassObject);
	void			SetDisconnectedNotifier	(DisconnectedProc proc, void* pClassObject);

	void			Destroy					();	

protected:
	bool			Call_ReceiveDataNotifier	(ESSocket* pSockFrom, BYTE* lpData, UINT nSize);
	bool			Call_AcceptNotifier			(ESSocket* pAccepted, BOOL& bAccept);
	bool			Call_DisconnectedNotifier	(ESSocket* pSock);

protected:
	SOCKET				m_sock;					// Listen socket.
	SocketStatus		m_sockStat;				// Socket status.
	int					m_nMaxPending;			// Pending connections count.

	ReceiveDataProc		m_procReceiveData;
	void*				m_lpClassObjReceiveData;
	AcceptProc			m_procAccept;
	void*				m_lpClassObjAccept;
	DisconnectedProc	m_procDisconnected;
	void*				m_lpClassObjDisconnected;

	HANDLE				m_hThread;				// Handle to the listen thread.
	HANDLE				m_hEventStop;			// Thread stop event.
	DWORD				m_dwThreadId;			// Thread Identifier.
	BOOL				m_bThread;				// Thread is working.
	int					m_nAcceptWaitTime;		// Accept wait time.
	CCriticalSection	m_critSection;			// Critical section.
	CString				m_sLastError;			// Last error string.
};