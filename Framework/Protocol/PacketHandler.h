#pragma once
#include "../Array/AutoSortedArrayTempl.h"
#include "../Utility/ThisCallHelper.h"
#include "BasePacket.h"
#include "_PokerDefines.h"

/*
class PacketHandlerBase;
typedef void (__thiscall CObject::*PacketListenerMethod)(BasePacket* p, void* pParam);
typedef bool (__thiscall PacketHandlerBase::*Handle_Packet)(BYTE* lpData, int nSize);
*/

struct PacketEventStruct{
	PacketListenerMethod	method;
	void*	pThis;
	};

class PacketHandler
{
	friend class GameServer;
	friend class GameClient;

public:
	PacketHandler(bool bClient = true);
	virtual ~PacketHandler();

	static CCriticalSection*	GetLockSection(){
		static CCriticalSection lock;
		return &lock;
		}

public:
	bool			AddPacketObject			(PacketCodes code, BasePacket* packet);
	bool			AddPacketHandler		(PacketCodes code, void* pClass, PacketListenerMethod method);
	bool			SetPacketWaitingFor		(CEvent* pEvent, int nPacketCode, BasePacket** pPacket);
	bool			HandlePacket			(BasePacket* packet, void* lpParam);
	BasePacket*		CreatePacketByCode		(int nCode);
	bool			HandlePacketByCode		(int nCode, void* pParam, BYTE* lpData, int nSize, int* pnSizeBytesDeserialized = NULL);
	
private:
	void			DestroyArrays			();

protected:
	AutoSortedArrayTempl<int, List<PacketEventStruct>*>		m_arrHandlerMethodsByPacketCode;	// {Packet Code}{BasePacket*}
	AutoSortedArrayTempl<int, BasePacket*>					m_arrClassObjsByPacketCode;			// {Packet Code}{BasePacket*}
	bool													m_bClient;		

	CEvent*													m_pEventCallback;
	int														m_nPacketWaitingFor;
	BasePacket**											m_pPacketWaitingFor;
};