#include "PacketHandler.h"
#include "..\Utility\GrowableMemory.h"

///////////////////////////////////////////////////////////
///
///	class PacketHandlerBase
///
///////////////////////////////////////////////////////////

PacketHandler::PacketHandler(bool bClient /*= true*/){
	m_bClient				= bClient;
	m_pEventCallback		= NULL;
	m_nPacketWaitingFor		= 0;
	m_pPacketWaitingFor		= NULL;
	}

PacketHandler::~PacketHandler(){
	DestroyArrays();
	}

bool
PacketHandler::AddPacketObject(PacketCodes code, BasePacket* packet){
	if( m_arrClassObjsByPacketCode.IndexOf(code) == -1 ){
		m_arrClassObjsByPacketCode.Add(code, packet);
		return true;
		}
	return false;
	}

void
PacketHandler::DestroyArrays(){
	// Destroy class objects array. ####################
	int nCt		= m_arrClassObjsByPacketCode.GetCount(),  nLoop	= 0;
	while( nLoop < nCt ){
		BasePacket* p = (BasePacket*)m_arrClassObjsByPacketCode.GetData(nLoop);
		if( p )
			delete p;
		nLoop ++;
		}		
	m_arrClassObjsByPacketCode.DeleteAll();
	// #################################################

	// Destroy packet listener array with list. ########
	nLoop	= 0;
	nCt		= m_arrHandlerMethodsByPacketCode.GetCount();

	while(nLoop < nCt){
		List<PacketEventStruct>* pList = m_arrHandlerMethodsByPacketCode.GetData(nLoop);
		if( pList ){
			pList->RemoveAll(true);
			delete pList;
			}
		nLoop ++;
		}
	m_arrHandlerMethodsByPacketCode.DeleteAll();
	// #################################################
	}

bool
PacketHandler::AddPacketHandler(PacketCodes code, void* pClass, PacketListenerMethod method){
	int nCode = (int)code;
	int nIndex = m_arrHandlerMethodsByPacketCode.IndexOf(nCode);
	List<PacketEventStruct>* pList = NULL;
	if( nIndex == -1 )
		pList = new List<PacketEventStruct>();
	else
		pList = m_arrHandlerMethodsByPacketCode.GetData(nIndex);
	if( !pList ) return false;

	PacketEventStruct* st	= new PacketEventStruct();
	st->method				= method;
	st->pThis				= pClass;
	pList->Add(st);

	if( nIndex == -1 )
		m_arrHandlerMethodsByPacketCode.Add(nCode, pList);
	return true;
	}

BasePacket*	
PacketHandler::CreatePacketByCode(int nCode){
	int nIndex = m_arrClassObjsByPacketCode.IndexOf(nCode);
	if( nIndex == -1 )
		return NULL;
	BasePacket* p = m_arrClassObjsByPacketCode.GetData(nIndex);
	if( p == NULL )
		return NULL;
	BasePacket* packetRet = (BasePacket*)p->CreateSerializableObject();
	ASSERT(packetRet);
	packetRet->m_nCode = nCode;
	return packetRet;
	}

bool
PacketHandler::SetPacketWaitingFor(Event* pEvent, int nPacketCode, BasePacket** pPacket){
	GetLockSection()->Lock(); // Lock
	m_pEventCallback		= pEvent;
	m_nPacketWaitingFor		= nPacketCode;
	m_pPacketWaitingFor		= pPacket;
	GetLockSection()->Unlock(); // Unlock
	return true;
	}

bool
PacketHandler::HandlePacket(BasePacket* packet, void* lpParam){
	if( !packet ) return false;
	int nIndex = m_arrHandlerMethodsByPacketCode.IndexOf(packet->m_nCode);
	if( nIndex == -1 ) return false;

	List<PacketEventStruct>* pList = m_arrHandlerMethodsByPacketCode.GetData(nIndex);
	if( pList == NULL || pList->GetCount() == 0 ){
		return false;
		}

	int nLoop = 0, nCt = pList->GetCount();
	while( nLoop < nCt ){
		PacketEventStruct* p = pList->GetAt(nLoop);
		if( !p ){
			nLoop ++;
			continue;
			}
#ifdef _WIN64
		ThisCallHelper* pThisCall = (ThisCallHelper*)(p->pThis);
		pThisCall->Call_PacketListenerMethod(p->method, packet, lpParam);
#else
	// Call thiscall method. {{
		const int thisVarOffset = sizeof(PacketListenerMethod);
		__asm{
			push eax;
			push ecx;
			mov eax, p; // method owner class this pointer.
			add eax, thisVarOffset;
			mov eax, dword ptr[eax];
			mov ecx, eax;
			mov eax, lpParam;
			push eax;
			mov eax, packet;
			push eax;
			mov eax, p;
			mov eax, dword ptr[eax]; // calling method by address.
			call eax;
			pop ecx;
			pop eax;
			};
		// }}
#endif
		nLoop ++;
		}
	return true;
	}

bool
PacketHandler::HandlePacketByCode(int nCode, void* pParam, BYTE* lpData, int nSize, int* pnSizeBytesDeserialized /*= NULL*/){
	int nIndex = m_arrHandlerMethodsByPacketCode.IndexOf(nCode);
	if( nIndex != -1 ){
		List<PacketEventStruct>* pList = m_arrHandlerMethodsByPacketCode.GetData(nIndex);
		if( pList == NULL || pList->GetCount() == 0 )
			return false;

		int			nLoop	= 0;
		int			nCt		= pList->GetCount();
		BasePacket* packet	= CreatePacketByCode(nCode);
		ASSERT( packet );

#ifndef _DEBUG
		if( packet == NULL )
			return false;
#endif

		GrowableMemory mem(0x400, 0x400, false);
		mem.SetReadonlyBuffer(lpData, nSize);
		if( !packet->Deserialize(&mem) ){
			return false;
			}

		// Set deserialized data bytes size. {{
		if( pnSizeBytesDeserialized ){
			*pnSizeBytesDeserialized = mem.GetOffset();
			}
		// }}

		// Used only in client side, for waiting destination packet response from server. {{
		GetLockSection()->Lock(); // Lock
		if( m_pEventCallback && m_nPacketWaitingFor == packet->m_nCode ){	
			Event* pEvent		= m_pEventCallback;
			m_pEventCallback	= NULL;

			// Copy packet. {{
			BasePacket* pCopy = CreatePacketByCode(nCode);
			packet->Copy(pCopy);
			if( m_pPacketWaitingFor ){
				if( !::IsBadReadPtr(m_pPacketWaitingFor, sizeof(void*)) )
					*m_pPacketWaitingFor = pCopy;
				}
			else
				delete pCopy;
			m_pPacketWaitingFor = NULL;
			// }}
			pEvent->SetEvent();
			GetLockSection()->Unlock(); // Unlock
			return true;
			}
		else
			GetLockSection()->Unlock(); // Unlock
		// }}

		while( nLoop < nCt ){
			PacketEventStruct* p = pList->GetAt(nLoop);
			if( !p ){
				nLoop ++;
				continue;
				}
#ifdef _WIN64
			ThisCallHelper* pThisCall = (ThisCallHelper*)(p->pThis);
			pThisCall->Call_PacketListenerMethod(p->method, packet, pParam);
#else
		// Call thiscall method. {{
			const int thisVarOffset = sizeof(PacketListenerMethod);
			__asm{
				push eax;
				push ecx;
				mov eax, p; // method owner class this pointer.
				add eax, thisVarOffset;
				mov eax, dword ptr[eax];
				mov ecx, eax;
				mov eax, pParam;
				push eax;
				mov eax, packet;
				push eax;
				mov eax, p;
				mov eax, dword ptr[eax]; // calling method by address.
				call eax;
				pop ecx;
				pop eax;
				};
			// }}
#endif
			nLoop ++;
			}

		delete packet; // Destroy packet.
		return true;
		}
	return false;
	}