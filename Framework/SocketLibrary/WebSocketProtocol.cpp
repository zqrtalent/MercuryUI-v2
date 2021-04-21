#include "stdafx.h"
#include "SocketContext.h"
#include "WebSocketProtocol.h"
#include "../Utility/GrowableMemory.h"
#include "WebService/HTTPClient.h"
#include "Array/StringToStringArray.h"

#include "Crypt/base64/base64.h"
#include "Crypt/sha1/sha1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////
//
//	Class WebSocketProtocol.
//
/////////////////////////////////////////////////////////////////////

WebSocketProtocol::WebSocketProtocol() : SocketProtocolBase() {
	} 

WebSocketProtocol::~WebSocketProtocol(){
	}

void
WebSocketProtocol::OnConnected(SocketContext* pSockConn, bool& bHandshaking){
	bHandshaking = true;
	// Start handshake here ...
	}

void
WebSocketProtocol::OnAccepting(SocketContext* pSockConn, bool& bHandshaking){
	bHandshaking = true;
	// Waiting for handshake ...
	}

ProtocolOpResultCode
WebSocketProtocol::OnDeserializeData(SocketContext* pSockConn, BYTE* lpDataIn, DWORD dwSizeIn, DWORD dwBufferSizeIn, BYTE* &lpDataOut, DWORD& dwSizeOut, bool& bDestroyOutBuffer){
	if( (pSockConn->GetStatus()&SocketStatus::SockStatus_Handshaking) == SocketStatus::SockStatus_Handshaking ){
		// Should be HTTP request.
		if( dwSizeIn > 2048 /*Maximum HTTP headers size*/ )
			return ProtocolOpResultCode::Protocol_Invalid;

		// First data.
		if( pSockConn->GetSizeBytesReceived() == 0 ){
			_String				sHTTPReq = (TCHAR*)lpDataIn;
			StringToStringArray arrParams;
			HTTPClient::ParseHTTPRequest(&sHTTPReq, &arrParams);

			_String sHost			= arrParams.GetValue(_T("host"));
			_String sUpgrade		= arrParams.GetValue(_T("Upgrade"));				// websocket
			_String sConnection		= arrParams.GetValue(_T("Connection"));				// Upgrade
			_String sWebSocketVer	= arrParams.GetValue(_T("sec-websocket-version"));	// Version
			_String sWebSocketKey	= arrParams.GetValue(_T("sec-websocket-key"));		// Key
			
			_String sAcceptKey, sResponse;
			if( !sWebSocketKey.IsEmpty() && GenerateAcceptKey(sWebSocketKey, sAcceptKey) ){
				sResponse.Format(_T("HTTP/1.1 101\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nWebSocket-Origin: %s\r\nSec-WebSocket-Accept: %s\r\n\r\n"), 
					sHost.GetBuffer(), sAcceptKey.GetBuffer());
				pSockConn->PushSendData((BYTE*)sResponse.GetBuffer(), sResponse.GetLength(), false);

				// Handshake complete event.
				pSockConn->OnHandshakeComplete();

				lpDataOut			= lpDataIn;
				dwSizeOut			= dwSizeIn;
				bDestroyOutBuffer	= false;
				return ProtocolOpResultCode::Protocol_Handshake;
				}
			}
		
		return ProtocolOpResultCode::Protocol_Invalid;
		}
	else{
		lpDataOut			= NULL;
		dwSizeOut			= 0;
		bDestroyOutBuffer	= false;

		int nMinExpectedSize = 6;
		if (dwSizeIn < nMinExpectedSize)
			return ProtocolOpResultCode::Protocol_Invalid;

		BYTE payloadFlags		= lpDataIn[0];
		bool bTextData			= ((payloadFlags&0x01) == 1);
		bool bBinaryData		= ((payloadFlags&0x02) == 2);
		bool bConnectionClose	= ((payloadFlags&0x08) == 8);
		bool bPing				= ((payloadFlags&0x09) == 9);
		bool bPong				= ((payloadFlags&0x0A) == 10);

		if( (payloadFlags&128) == 0 )
			return ProtocolOpResultCode::Protocol_Invalid;	
		if( bConnectionClose )
			return ProtocolOpResultCode::Protocol_ConnClose;
		if( !bTextData && !bBinaryData ){
			return ProtocolOpResultCode::Protocol_Success;
			}

		BYTE				basicSize = lpDataIn[1] & 0x7F;
		unsigned __int64	payloadSize;
		int					masksOffset;

		if( basicSize <= 125 ){
			payloadSize = basicSize;
			masksOffset = 2;
			}
		else if (basicSize == 126){
			nMinExpectedSize += 2;
			if (dwSizeIn < nMinExpectedSize)
				return ProtocolOpResultCode::Protocol_Invalid;
			payloadSize = ntohs( *(u_short*) (lpDataIn + 2));
			masksOffset = 4;
			}
		else if (basicSize == 127){
			nMinExpectedSize += 8;
			if( dwSizeIn < nMinExpectedSize )
				return ProtocolOpResultCode::Protocol_Invalid;
			payloadSize = ntohl( *(u_long*) (lpDataIn + 2));
			masksOffset = 10;
			}
		else
			return ProtocolOpResultCode::Protocol_Invalid;

		nMinExpectedSize += payloadSize;
		if( dwSizeIn < nMinExpectedSize )
			return ProtocolOpResultCode::Protocol_Invalid;

		BYTE masks[4];
		memcpy(masks, lpDataIn + masksOffset, 4);
		BYTE* payload = lpDataIn + masksOffset + 4;
		for (unsigned __int64 i = 0; i < payloadSize; i++)
			payload[i] = (payload[i] ^ masks[i%4]);
		
		if( payloadSize < dwBufferSizeIn && bTextData )
			payload[payloadSize] = '\0';
  
		lpDataOut			= (BYTE*)payload;
		dwSizeOut			= payloadSize;
		bDestroyOutBuffer	= false;
		}

	TCHAR* pszSend = "Hello Zack Sup!!!";
	DWORD dwSize= strlen(pszSend);
	pSockConn->Send((BYTE*)pszSend, dwSize);

	return ProtocolOpResultCode::Protocol_Success;
	}

ProtocolOpResultCode
WebSocketProtocol::OnSerializeData(SocketContext* pSockConn, BYTE* lpDataIn, DWORD dwSizeIn, WSABUF& buff, DWORD dwBufferSize){
	if( (pSockConn->GetStatus()&SocketStatus::SockStatus_Handshaking) == SocketStatus::SockStatus_Handshaking ){
		if( dwSizeIn > dwBufferSize )
			return ProtocolOpResultCode::Protocol_Invalid;
		memcpy(buff.buf, lpDataIn, dwSizeIn);
		buff.len = dwSizeIn;
		return ProtocolOpResultCode::Protocol_Success;
		}
	
	unsigned __int64	payloadSize		= dwSizeIn;
	int					expectedSize	= payloadSize + 1; //flags byte.
	buff.len							= 0;

	if( payloadSize <= 125  && payloadSize <= 65535 )
		expectedSize += 1;
	else
		if( payloadSize > 125  && payloadSize <= 65535 )
			expectedSize += 3;
		else
			expectedSize += 9;

	if( expectedSize > dwBufferSize )
		return ProtocolOpResultCode::Protocol_Invalid;

	// Flags byte.
	char payloadFlags = 130; 
	buff.buf[buff.len++] = payloadFlags;

	// Length byte.
	if( payloadSize <= 125 ){
		char basicSize = payloadSize;
		buff.buf[buff.len++] = basicSize;
		}
	else 
	if( payloadSize > 125 && payloadSize <= 65535 ){
		char basicSize = 126;
		buff.buf[buff.len++] = basicSize;

		char len[2];
		len[0] = ( payloadSize >> 8 ) & 255;
		len[1] = ( payloadSize ) & 255;
		buff.buf[buff.len++] = len[0];
		buff.buf[buff.len++] = len[1];
		}
	else{
		char basicSize = 127;
		buff.buf[buff.len++] = basicSize;

		char len[8];
		len[0] = ( payloadSize >> 56 ) & 255;
		len[1] = ( payloadSize >> 48  ) & 255;
		len[2] = ( payloadSize >> 40 ) & 255;
		len[3] = ( payloadSize >> 32  ) & 255;
		len[4] = ( payloadSize >> 24 ) & 255;
		len[5] = ( payloadSize >> 16  ) & 255;
		len[6] = ( payloadSize >> 8 ) & 255;
		len[7] = ( payloadSize ) & 255;
		memcpy(&buff.buf[buff.len], len, 8);
		buff.len += 8;
		}

	memcpy(&buff.buf[buff.len], lpDataIn, dwSizeIn);
	buff.len += dwSizeIn;
	return ProtocolOpResultCode::Protocol_Success;
	}

bool
WebSocketProtocol::GenerateAcceptKey(const _String sKey, _String& sAcceptKey){
	_String			server_key = sKey;
	server_key	+= "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    _SHA1			sha;
    unsigned int    message_digest[5];

    sha.Reset();
    sha << (TCHAR*)server_key.GetBuffer();
    sha.Result(message_digest);

    // convert sha1 hash bytes to network byte order because this sha1
    // library works on ints rather than bytes
    for (int i = 0; i < 5; i++)
        message_digest[i] = htonl(message_digest[i]);

    sAcceptKey = base64_encode(
        reinterpret_cast<const unsigned char*>
        (message_digest),20
        );
	return true;
	}