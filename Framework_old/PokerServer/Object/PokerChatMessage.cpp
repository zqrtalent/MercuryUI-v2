// PokerChatMessage.cpp : implementation file
//

#include "PokerChatMessage.h"

PokerChatMessage::PokerChatMessage(){
	}

BEGIN_DECLARE_VAR(PokerChatMessage, Serializable)
	DECLARE_VAR(_T("PokerChatMessage"), _T(""),	VariableType::VT_None, 0, false, true, false)
	DECLARE_VAR(_T("Seat"),				_T(""),	VariableType::VT_Int8, offsetof(PokerChatMessage,m_cSeat), false, false, false)
	DECLARE_VAR(_T("UserName"),			_T(""),	VariableType::VT_String, offsetof(PokerChatMessage,m_sUserName), false, false, false)
	DECLARE_VAR(_T("System"),			_T(""),	VariableType::VT_Bool, offsetof(PokerChatMessage,m_bSystem), false, false, false)
	DECLARE_VAR(_T("Actiontype"),		_T(""),	VariableType::VT_Int8, offsetof(PokerChatMessage,m_actionType), false, false, false)
	DECLARE_VAR(_T("Amount"),			_T(""),	VariableType::VT_Int32, offsetof(PokerChatMessage,m_nAmount), false, false, false)
	DECLARE_VAR(_T("Message"),			_T(""),	VariableType::VT_String, offsetof(PokerChatMessage,m_sMessage), false, false, false)
	DECLARE_VAR(_T("dtTime"),			_T(""),	VariableType::VT_DateTime, offsetof(PokerChatMessage,m_dtTime), false, false, false)
END_DECLARE_VAR()

PokerChatMessage::~PokerChatMessage(){
	}