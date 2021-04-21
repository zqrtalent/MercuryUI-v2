#include "stdafx.h"
#include "SocketProtocolFactory.h"

// Protocols
#include "SocketProtocolBase.h"
#include "WebSocketProtocol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////
//
//	Class SocketProtocolFactory
//
/////////////////////////////////////////////////////////////////////

SocketProtocolBase*
SocketProtocolFactory::GetProtocolByName(const _String sName){
	if( sName == _T("websocket") )
		return new WebSocketProtocol();
	return new SocketProtocolBase();
	}