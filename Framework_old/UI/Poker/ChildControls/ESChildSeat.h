#pragma once


/////////////////////////////////////////////////////////////////////////////
// ESChildControlSeat window

#include "../../../Controls/ESChildControl.h"
#include "../../../Protocol/_PokerDefines.h"

// Custom child event code.
#define EventCode_SeatTimeBankActivated CHILD_CONTROL_EVENT_CODE_CUSTOM + 1

class PokerTableSeatInfo;
class ESChildSeat : public ESChildControl
{
// Construction
public:
	ESChildSeat();
	virtual ~ESChildSeat();

	enum{
		PulseTimerId		= 1000,
		ActionClearTimerId,
		};

	virtual bool		Create					(int nId, Rect rRect, MercuryBaseView* pOwner, ESChildControl* pParent);
	virtual bool		Destroy					();
	virtual void		OwnerWindowSizeChanged	(int cx, int cy);
	virtual void		OnTimer					(UINT nIdEvent);

	bool				IsSeatActive			(){return m_bActive;};
	void				SetSeatActive			(bool bActive, bool bRedraw = true);


	virtual bool		ActivateTimer			(int nFullTimeSec, bool bActivate = true);
	bool				SetAvatarImage			(Image* pImage, int nAvatarId, bool bRedraw);
	bool				ClearAvatarImage		(bool bRedraw);
	int					GetAvatarId				(){return m_nAvatarId;};
	void				SetRegularThinkTimeSec	(int nRegThinkTimeSec){m_nRegularTimeSec = nRegThinkTimeSec;};

	void				SetSeatIndex			(int nSeat)	{m_nSeat = nSeat;};
	int					GetSeatIndex			()			{return m_nSeat;};
	void				SetSeatStatus			(PokerSeatStatus status){m_seatStatus = status;}
	PokerSeatStatus		GetSeatStatus			(){return m_seatStatus;};
	void				SetSeatInfo				(PokerTableSeatInfo* pSeatInfo){m_pSeatInfo = pSeatInfo;};

	// Seat action.
	void				SetActionName			(std::string sAction, int nActionDurationMS, bool bRedraw);
	// Color border.
	void				SetBorderColor			(COLORREF crBorder, bool bRedraw);
	void				ClearBorderColor		(bool bRedraw);
	bool				GetBorderColor			(COLORREF& crBorder){ if( m_bColoredBorder ) crBorder = m_crBorder;  return m_bColoredBorder;};
	// User note
	void				SetUserNote				(std::string sNote, bool bRedraw);
	std::string			GetUserNote				(){return m_sUserNote;};
	void				ClearUserNote			(bool bRedraw);

protected:
	virtual void		OnPaintClient					(_DC* pDC, Rect* pRectDC, Rect* pRectClient);
	virtual void		OnMouseEnter					(UINT nFlags);
	virtual	void		OnLButtonClick					(Point pt, UINT nFlags);
	virtual	void		OnRButtonUp						(Point pt, UINT nFlags);
	virtual	void		OnLButtonDblClick				(Point pt, UINT nFlags);
	// Remain control captured out of client area.
	virtual bool		RemainCapturedOutOfClientArea	(){return false;};
	// Retrieve region, bound size and centered flag.
	virtual RGNDef		GetControlRgn					(Size& szRgnBound, bool& bCentered);
	virtual bool		GetTooltipText					(std::string& sTooltipText);
	virtual void		OnDeserializationCompleted		();
	virtual void		OnLocalizeStrings				();
	// Designer virtual methods
	virtual std::string		GetControlDefaultName			(){return _T("_pokerSeat");};
	virtual Size		GetControlDefaultSize			(){return Size(100, 100);};
	virtual bool		AllowToAddChildControl			(){return true;};

protected:
	void				UpdateImages					();

protected:
	std::string			m_sBorderImageFile;				// Background image file.
	std::string			m_sActiveBorderImageFile;		// Active background image file.
	std::string			m_sBorderColoredImageFile;		// Colored border image file.
	std::string			m_sBorderColoredBaseImageFile;	// Colored border image file.
	bool				m_bColoredBorder;				// Colored border visibility.
	COLORREF			m_crBorder;						// Border color.
	StringToIntArray	m_imageFillMode;				// Border image fill mode.
	Rect				m_rcOmitBorder;					// Border image omitborder rect.
	float				m_fZoom;						// Zoom factor.
	std::string			m_sSeatImage;					// Seat image file.
	std::string			m_sActiveSeatImage;				// Active seat image file.
	std::string			m_sEmptySeatImage;				// Empty seat image.
	std::string			m_sAvatarFaceImage;				// Avatar face image.
	std::string			m_sReservedSeatImage;			// Reserved seat image.
	bool				m_bActive;						// Active state.
	Point				m_ptBorderPos;					// Border position. (-1, -1) means that border is around the seat.

	bool				m_bUseEmptySeatLabel;			// Usage of empty seat label.
	std::string			m_sEmptySeatLabel;				// Empty seat label text.
	std::string			m_sEmptySeatLabelLocal;			// Empty seat label text localized.
	Font				m_fontEmptySeatLabel;			// Empty seat label font.
	COLORREF			m_crEmptySeatLabel;				// Empty seat label color.

	std::string			m_sUserNote;					// User note description.
	PokerTableSeatInfo*	m_pSeatInfo;
	int					m_nSeat;
	std::string			m_sActionName;					// Action name.
	int					m_nActionClearTimerId;			// Action clear timer id.
	int					m_nPulseTimerId;				// Pulse timer id.
	bool				m_bPulseState;
	bool				m_bTimeBankIsActive;			// Time bank is active.
	std::string			m_sUserBalanceLabel;
	int					m_nFullTimeSec;					// Full time seconds
	int					m_nRegularTimeSec;				// Regular time seconds.

	Image*				m_pBorderImage;
	Image*				m_pBorderActiveImage;
	Image*				m_pBorderColoredImage;
	Image*				m_pBorderColoredBaseImage;
	Image*				m_pSeatImage;
	Image*				m_pSeatImageActive;
	Image*				m_pEmptySeatImage;
	Image*				m_pAvatarFaceImage;
	Image*				m_pReservedSeatImage;
	Image				m_imgBorderColored;
	Image				m_avatarImage;
	int					m_nAvatarId;
	PokerSeatStatus		m_seatStatus;

public:
	virtual Serializable*	CreateSerializableObject	(){return new ESChildSeat();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()

	// Variable modify callbacks.
protected:
	static bool	ColorBorderModify	(ESChildSeat* pThis, VariableInfo* pInfo, void* lpValueNew);
	static bool	ImageModify			(ESChildSeat* pThis, VariableInfo* pInfo, void* lpValueNew);
	static bool	AttributeModify		(ESChildSeat* pThis, VariableInfo* pInfo, void* lpValueNew);
};