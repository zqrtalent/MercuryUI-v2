#pragma once
#include "SocketProtocolBase.h"

class WebSocketProtocol : public SocketProtocolBase
{
public:
	WebSocketProtocol			();
	virtual ~WebSocketProtocol	();

protected:
	virtual void					OnConnected			(SocketContext* pSockConn, bool& bHandshaking);
	virtual void					OnAccepting			(SocketContext* pSockConn, bool& bHandshaking);
	virtual ProtocolOpResultCode	OnDeserializeData	(SocketContext* pSockConn, BYTE* lpDataIn, DWORD dwSizeIn, DWORD dwBufferSizeIn, BYTE* &lpDataOut, DWORD& dwSizeOut, bool& bDestroyOutBuffer);
	virtual ProtocolOpResultCode	OnSerializeData		(SocketContext* pSockConn, BYTE* lpDataIn, DWORD dwSizeIn, WSABUF& buff, DWORD dwBufferSize);

	bool							GenerateAcceptKey	( const _String sKey,  _String& sAcceptKey);
};


/*
HTTP/1.1 101 Switching Protocols\r\n
std::string server_key = request.GetField("Sec-WebSocket-Key");
    server_key += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

    SHA1        sha;
    unsigned int    message_digest[5];

    sha.Reset();
    sha << server_key.c_str();

    sha.Result(message_digest);
    // convert sha1 hash bytes to network byte order because this sha1
    //  library works on ints rather than bytes
    for (int i = 0; i < 5; i++) {
        message_digest[i] = htonl(message_digest[i]);
    }

    server_key = base64_encode(
        reinterpret_cast<const unsigned char*>
        (message_digest),20
        );


    response.SetField("Upgrade", "websocket");
    response.SetField("Connection", "Upgrade");
    response.SetField("Sec-WebSocket-Accept", server_key);
*/