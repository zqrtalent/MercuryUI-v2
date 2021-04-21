#pragma once
#include "..\..\Protocol\PokerPacket.h"
#include "..\..\Protocol\_PokerDefines.h"

class PokerAnimationInfo
{
protected:
	PokerAnimationInfo(){};

public:
	virtual ~PokerAnimationInfo(){};

public:
	AnimationType		m_animationType;
	int					m_nAnimationSec;
};

class CollectChipsAnimationInfo : public PokerAnimationInfo
{
public:
	CollectChipsAnimationInfo()
	{
		m_animationType = AnimationType::CollectChips;
	};
	virtual ~CollectChipsAnimationInfo(){};

public:
	int					m_nMainChipsAmount;
};

class ChipsMovementAnimationInfo : public PokerAnimationInfo
{
public:
	ChipsMovementAnimationInfo()
	{
		m_animationType = AnimationType::ChipsMovement;
	};
	virtual ~ChipsMovementAnimationInfo(){};

public:
	int					m_arrMoveChipsFromPotAmounts[10];
};

class DealSeatCardsAnimationInfo : public PokerAnimationInfo
{
public:
	DealSeatCardsAnimationInfo()
	{
		m_animationType = AnimationType::DealSeatCards;
	};
	virtual ~DealSeatCardsAnimationInfo(){};

public:
	PokerHandStartedPacket	m_handStartedPacket;
};

class DealTableCardsAnimationInfo : public PokerAnimationInfo
{
public:
	DealTableCardsAnimationInfo()
	{
		m_animationType = AnimationType::DealTableCards;
	};
	virtual ~DealTableCardsAnimationInfo(){};

public:
	PokerRoundStartedPacket	m_roundStartedPacket;
};