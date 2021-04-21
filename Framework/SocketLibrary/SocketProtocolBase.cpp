#include "stdafx.h"
#include "SocketProtocolBase.h"
#include "SocketContext.h"
#include "../Utility/GrowableMemory.h"
#include "../Path/Path.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////
//
//	Class SocketProtocolBase
//
/////////////////////////////////////////////////////////////////////

SocketProtocolBase::SocketProtocolBase(){
	} 

SocketProtocolBase::~SocketProtocolBase(){
	}

void
SocketProtocolBase::OnConnected(SocketContext* pSockConn, bool& bHandshaking){
	bHandshaking = false;
	}

void
SocketProtocolBase::OnAccepting(SocketContext* pSockConn, bool& bHandshaking){
	bHandshaking = false;
	}

ProtocolOpResultCode
SocketProtocolBase::OnDeserializeData(SocketContext* pSockConn, BYTE* lpDataIn, DWORD dwSizeIn, DWORD dwBufferSizeIn, BYTE* &lpDataOut, DWORD& dwSizeOut, bool& bDestroyOutBuffer){
	lpDataOut			= lpDataIn;
	dwSizeOut			= dwSizeIn;
	bDestroyOutBuffer	= false;
	return ProtocolOpResultCode::Protocol_Success;
	}

ProtocolOpResultCode
SocketProtocolBase::OnSerializeData(SocketContext* pSockConn, BYTE* lpDataIn, DWORD dwSizeIn, WSABUF& buff, DWORD dwBufferSize){
	ASSERT( dwSizeIn <= dwBufferSize );
	if( dwSizeIn > dwBufferSize )
		return ProtocolOpResultCode::Protocol_Invalid;
	memcpy(buff.buf, lpDataIn, dwSizeIn);
	buff.len = dwSizeIn;
	return ProtocolOpResultCode::Protocol_Success;
	}