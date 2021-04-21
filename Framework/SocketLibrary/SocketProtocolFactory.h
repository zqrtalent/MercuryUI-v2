#pragma once
class SocketProtocolBase;

class SocketProtocolFactory
{
public:
	static SocketProtocolBase* GetProtocolByName(const _String sName);
};
