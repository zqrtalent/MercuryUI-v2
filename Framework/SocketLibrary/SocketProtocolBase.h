#pragma once

class SocketContext;
enum ProtocolOpResultCode
{
	Protocol_Success = 0,
	Protocol_Invalid,
	Protocol_Partial,
	Protocol_ConnClose,
	Protocol_Handshake
};

class SocketProtocolBase
{
public:
	SocketProtocolBase			();
	virtual ~SocketProtocolBase	();

public:
	virtual void					OnConnected			(SocketContext* pSockConn, bool& bHandshaking);
	virtual void					OnAccepting			(SocketContext* pSockConn, bool& bHandshaking);
	virtual ProtocolOpResultCode	OnDeserializeData	(SocketContext* pSockConn, BYTE* lpDataIn, DWORD dwSizeIn, DWORD dwBufferSizeIn, BYTE* &lpDataOut, DWORD& dwSizeOut, bool& bDestroyOutBuffer);
	virtual ProtocolOpResultCode	OnSerializeData		(SocketContext* pSockConn, BYTE* lpDataIn, DWORD dwSizeIn, WSABUF& buff, DWORD dwBufferSize);
};
