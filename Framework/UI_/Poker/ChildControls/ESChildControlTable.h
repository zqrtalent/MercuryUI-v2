#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildControlTable window
#include "../../../Controls/ESChildControl.h"
#include "../../../Controls/ESEasyMotion.h"

class ChipInfo : public Serializable
{
public:
	ChipInfo();
	~ChipInfo();

public:
	_String		m_sImageFile;
	INT_CHIPS	m_nAmount;
	_Size		m_szImage;
	Image		m_chipImage;

public:
	virtual Serializable*	CreateSerializableObject	(){return new ChipInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class ChipsGroupInfo : public Serializable
{
public:
	ChipsGroupInfo();
	~ChipsGroupInfo();

public:
	_Size						m_szChips;		// Chip image dimension.
	EnumerableObject<ChipInfo>	m_listChips;	// Chip infos.

public:
	virtual Serializable*	CreateSerializableObject	(){return new ChipsGroupInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class CardsGroup : public Serializable
{
public:
	CardsGroup();
	~CardsGroup();

public:
	Image						m_imgCardBg;
	bool						m_bMultiSuitBg;
	Image						m_imgCardBg1;
	Image						m_imgCardBg2;
	Image						m_imgCardBg3;
	Image						m_imgCardBg4;
	Image						m_imgCardBg5;
	Image						m_imgCardBg6;

	Image						m_imgCardRank;
	Image						m_imgCardSuit;

	Image						m_imgCardSuit1;
	Image						m_imgCardSuit2;
	Image						m_imgCardSuit3;
	Image						m_imgCardSuit4;
	Image						m_imgCardSuit5;
	Image						m_imgCardSuit6;

	Image						m_imgCardDeck;

	_Size						m_szCard;
	_Rect						m_rcDeck;

public:
	virtual Serializable*	CreateSerializableObject	(){return new CardsGroup();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class CardsInfo : public Serializable
{
public:
	CardsInfo();
	~CardsInfo();

public:
	_Size							m_szBackSm;
	_Size							m_szBack;
	_Size							m_szBackMed;
	_Size							m_szBackLg;

	Image							m_imgBackSm;
	Image							m_imgBack;
	Image							m_imgBackMed;
	Image							m_imgBackLg;
	

	Image							m_imgDeck;

	List<Image>					m_listBackImagesSm;
	List<Image>					m_listBackImages;
	List<Image>					m_listBackImagesMed;
	List<Image>					m_listBackImagesLg;
	AutoSortedArray					m_arrBackImagesOrderedByDimensions; // List of back images sorted by dimension.

	EnumerableObject<CardsGroup>	m_listCards;	// Cards group..

public:
	virtual Serializable*	CreateSerializableObject	(){return new CardsInfo();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class ESChildControlTable : public ESChildControl
{
// Construction
public:
	ESChildControlTable();
	virtual ~ESChildControlTable();

	friend class ESChildSeat;
	friend class ESChildSeatStatus;

	virtual bool		Create							(int nId, _Rect rcRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool		Destroy							();

	static int			GetChipImagesByAmount			(_Size szChip, INT_CHIPS nAmount, CPtrVector& arrChipImages, _Size& szChipImage, int* pChipsGroupId = NULL);
	static bool			DrawCardByIndex					(MercuryBaseView* pBaseView,_DC* pDC, _Rect* pRectDC, _Rect* pCardDC, _Size szCard, int nCard, bool bFourColored = false, int nConstantAlpha = 255);
	static bool			DrawCardBackByIndex				(MercuryBaseView* pBaseView, _DC* pDC, _Rect* pRectDC, _Rect* pCardDC, _Size szCard, int nIndex, int nAlphaConstant, bool bSmallCards);
	static _Size		GetCardDimensionMin				();
	static _Size		GetCardDimensionMax				();
	static BOOL			GetNearestCardDimension			(_Size& szCard, CardsGroup** pGroup = NULL, int* pCardSuitWidth = NULL);
	static Image*		GetNearestCardBackImage			(_Size& szCard, int nCardBackIndex);

	static bool			FreeGlobalResources				();

	bool				SetTableColor					(COLORREF crTable, bool bUpdate);
	bool				SetTableBgImage					(const _String sBgImageFIle, bool bUpdate);
	bool				IsTableImageCached				(){return (GetTableImageDefault()->IsNull() == false);};
	COLORREF			GetTableColor					(){return m_crFgColor;};
	_String				GetTableBgImageFile				(){return m_sBgImageFile;};

	// Seat cards deal animation.
	ESEasyMotionType	GetSeatCardsDealMotionTypeX		(){return (ESEasyMotionType)m_seatCardsDealMotionTypeX.GetCurrentValue();};
	ESEasyMotionType	GetSeatCardsDealMotionTypeY		(){return (ESEasyMotionType)m_seatCardsDealMotionTypeY.GetCurrentValue();};
	int					GetMotionTimeMS					(){return m_nMotionTimeMS;};
	int					GetDistanceSeatCardAnimation	(){return m_nTimeDistanceBetweenSeatCardAnimation;};
	int					GetDistanceCardAnimation		(){return m_nTimeDistanceBetweenCardAnimation;};
	int					GetFrameCt						(){return m_nFrameCtPerSecond;};
	BYTE				GetCardAlphaAtStart				(){return m_btCardAlphaAtStart;};

	// Table cards animation.
	ESEasyMotionType	GetTableCardsDealMotionTypeX	(){return (ESEasyMotionType)m_tableCardsAnimation.motionTypeX.GetCurrentValue();};
	ESEasyMotionType	GetTableCardsDealMotionTypeY	(){return (ESEasyMotionType)m_tableCardsAnimation.motionTypeY.GetCurrentValue();};
	int					GetTableCardsDealMotionTimeMS	(){return m_tableCardsAnimation.nMotionTimeMS;};
	int					GetTableFlopCardsDealMotionTimeMS(){return m_tableCardsAnimation.nFlopCardsMotionTimeMS;};
	int					GetTableCardsDealFrameCt		(){return m_tableCardsAnimation.nFrameCtPerSecond;};
	int					GetTableCardsTimeDistBetweenCards(){return m_tableCardsAnimation.nTimeDistanceBetweenCardAnimation;};
	int					GetTableCardsAlphaAtStart		(){return m_tableCardsAnimation.btAlphaAtStart;};
	bool				GetTableCardsSlideFlopAnim		(){return m_tableCardsAnimation.bSlideCardsOnFlop;};

	// Collect chips animation.
	ESEasyMotionType	GetCollectChipsMotionTypeX		(){return (ESEasyMotionType)m_collectChipsAnimation.motionTypeX.GetCurrentValue();};
	ESEasyMotionType	GetCollectChipsMotionTypeY		(){return (ESEasyMotionType)m_collectChipsAnimation.motionTypeY.GetCurrentValue();};
	int					GetCollectChipsMotionTimeMS		(){return m_collectChipsAnimation.nMotionTimeMS;};
	int					GetCollectChipsFrameCt			(){return m_collectChipsAnimation.nFrameCtPerSecond;};
	int					GetCollectChipsDelayAfterMotion	(){return m_collectChipsAnimation.nDelayAfterMotionMS;};

	// Move chips animation.
	ESEasyMotionType	GetMoveChipsMotionTypeX			(){return (ESEasyMotionType)m_moveChipsAnimation.motionTypeX.GetCurrentValue();};
	ESEasyMotionType	GetMoveChipsMotionTypeY			(){return (ESEasyMotionType)m_moveChipsAnimation.motionTypeY.GetCurrentValue();};
	int					GetMoveChipsMotionTimeMS		(){return m_moveChipsAnimation.nMotionTimeMS;};
	int					GetMoveChipsFrameCt				(){return m_moveChipsAnimation.nFrameCtPerSecond;};

public:
	static COLORREF* GetTableColorApplied(){
		static COLORREF crTable;
		return &crTable;
		}

	static _String* GetTableBgImageFileApplied(){
		static _String sBgImage;
		return &sBgImage;
		}

protected:
	static Image*	GetTableImageDefault(){
		static Image imgTable;
		return &imgTable;
		}

	static CRgn*	GetTableImageRgn(){
		static CRgn rgnTableImage;
		return &rgnTableImage;
		}

	static CRgn*	GetEmptySeatImageRgn(){
		static CRgn rgnEmptySeatImage;
		return &rgnEmptySeatImage;
		}

	static CRgn*	GetSeatImageRgn(){
		static CRgn rgnSeatImage;
		return &rgnSeatImage;
		}

	static CRgn*	GetSeatStatusImageRgn(){
		static CRgn rgnSeatStatusImage;
		return &rgnSeatStatusImage;
		}

	static EnumerableObject<ChipsGroupInfo>* GetChipsGroupInstance(){
		static EnumerableObject<ChipsGroupInfo> chipsGroups;
		return &chipsGroups;
		}

	static EnumerableObject<CardsInfo>* GetCardsInfoInstance(){
		static EnumerableObject<CardsInfo> listCards;
		return &listCards;
		}

	virtual void	OnPaintClient				(_DC *pDC, _Rect* pRectDC, _Rect* pRectClient);
	virtual	void	OnRButtonUp					(_Point pt, UINT nFlags);
	// Designer virtual methods
	virtual _String	GetControlDefaultName		(){return _T("_pokerTable");};
	virtual _Size	GetControlDefaultSize		(){return _Size(300, 300);};
	virtual bool	AllowToAddChildControl		(){return true;};
	virtual void	OnDeserializationCompleted	();

public:
	void			UpdateTableImage			(bool bUpdateExisting);

protected:	
	bool			LoadCardImages				(_String sImageFolder, _String sCardName, bool bUpdateExisting);
	bool			LoadChipImages				(_String sImageFolder, bool bUpdateExisting);

	static List<Image>* GetVIPStatusBorderImages(){
		static List<Image> _listVIPStatusBorderImages;
		return &_listVIPStatusBorderImages;
		}

	static List<Image>* GetVIPStatusActiveBorderImages(){
		static List<Image> _listVIPStatusActBorderImages;
		return &_listVIPStatusActBorderImages;
		}

	bool			LoadVIPStatusBorderImages	(const _String sBorderImage, const _String sActiveBorderImage, bool bUpdate);
	static Image*	GetVIPStatusBorderImage		(int nVIPLevelIndex, bool bActive);
	Image*			GetVIPStatusImage			(){return m_pVIPStatusImage;};
	int				GetVIPStatusType			(){return m_nSeatVIPStatusType;};

	// Caching images.
	ImageDef		GetCachedTableImage			();
	bool			FreeCachedTableImage		();
	bool			UpdateCachedTableImage		(ImageDef hImage);

	ImageDef		GetCachedEmptySeatImage		();
	bool			FreeCachedEmptySeatImage	();
	bool			UpdateCachedEmptySeatImage	(ImageDef hImage);

	ImageDef		GetCachedSeatImage			(int nSeat, bool bActive, bool bColorBorder, bool bAvatar);
	bool			FreeCachedSeatImage			(int nSeat, bool bActive, bool bColorBorder, bool bAvatar);
	bool			FreeCachedSeatSpeImages	(int nSeat);
	bool			UpdateCachedSeatImage		(int nSeat, ImageDef hImage, bool bActive, bool bColorBorder, bool bAvatar);

	ImageDef		GetCachedSeatStatusImage	(bool bActive);
	bool			FreeCachedSeatStatusImage	(bool bActive);
	bool			UpdateCachedSeatStatusImage	(bool bActive, ImageDef hImage);

	ImageDef		GetCachedCardBackImage		(bool bSmall);
	bool			FreeCachedCardBackImage		(bool bSmall);
	bool			UpdateCachedCardBackImage	(bool bSmall, ImageDef hImage);

	ImageDef		GetCachedImage				(LPCTSTR lpszName);
	bool			FreeCachedImage				(LPCTSTR lpszName);
	bool			UpdateCachedImage			(LPCTSTR lpszName, ImageDef hImage);

protected:
	_String				m_sBgImageFile;
	_String				m_sBorderImageFile;
	_String				m_sFgImageFile;
	COLORREF			m_crFgColor;
	_String				m_sLogoImage;
	_Size				m_szDefTableImage;

	_String				m_sSeatVIPStatusImageFile;	// VIP status image.
	int					m_nSeatVIPStatusType;		// VIP status type.
	Image*				m_pVIPStatusImage;

	_String				m_sLayer1ImageFile;
	_String				m_sLayer2ImageFile;
	_String				m_sLayer3ImageFile;

	_String				m_sChipImagesFolder;
	_String				m_sCardImagesFolder;
	_String				m_sCardName;			// Currently using card.

	// Seat cards deal animation. {{
	StringToIntArray	m_seatCardsDealMotionTypeX;
	StringToIntArray	m_seatCardsDealMotionTypeY;
	int					m_nMotionTimeMS;
	int					m_nTimeDistanceBetweenSeatCardAnimation;
	int					m_nTimeDistanceBetweenCardAnimation;
	int					m_nFrameCtPerSecond;
	BYTE				m_btCardAlphaAtStart; // Card alpha at start animation.
	// }}

	// Table cards deal animation. {{
	typedef struct TableCardsAnimationInfo{
		StringToIntArray	motionTypeX;
		StringToIntArray	motionTypeY;
		int					nMotionTimeMS;
		int					nFlopCardsMotionTimeMS;
		int					nTimeDistanceBetweenCardAnimation;
		int					nFrameCtPerSecond;
		BYTE				btAlphaAtStart;
		bool				bSlideCardsOnFlop;
		};
	TableCardsAnimationInfo	m_tableCardsAnimation;
	// }}

	// Collect chips animation. {{
	typedef struct CollectChipsAnimationInfo{
		StringToIntArray	motionTypeX;
		StringToIntArray	motionTypeY;
		int					nMotionTimeMS;
		int					nFrameCtPerSecond;
		int					nDelayAfterMotionMS;
		};

	CollectChipsAnimationInfo	m_collectChipsAnimation;
	// }}

	// Move chips animation. {{
	typedef struct MoveChipsAnimationInfo{
		StringToIntArray	motionTypeX;
		StringToIntArray	motionTypeY;
		int					nMotionTimeMS;
		int					nFrameCtPerSecond;
		};

	MoveChipsAnimationInfo	m_moveChipsAnimation;
	// }}

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESChildControlTable();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

	// Variable modify callbacks.
protected:
	static bool	AttributeModify(ESChildControlTable* pThis, VariableInfo* pInfo, void* lpValueNew);
};