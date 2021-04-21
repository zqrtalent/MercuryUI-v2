
// ids
#define		ID_TOOLTIP		11				// incremental-search tooltip id
#define		ID_TIMER		1				// incremental search timer id
#define		EN_FINISH		(WM_USER + 1)	// message from in-place editors to the list view
#define		CB_SUICIDE		(WM_USER + 2)	// message sent to inplace combo boxes

#define     IDC_INPLACEEDIT 0x50FF

// inplace editors status
#define INPLACE_OK			0
#define INPLACE_CANCEL		1	// the inplace editor was canceled (escape pressed)

// inplace editor type
#define INPLACE_EDIT		1
#define INPLACE_COMBO		2

// window procs for in-place editors
static WNDPROC				mOldChildProc, mOldComboEditProc, mOldComboListProc;
static HWND					mComboLBox;
static bool					mComboLBoxClicked;

INT_PTR CALLBACK			EditBoxProc		(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK			ComboBoxProc	(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK			ComboEditProc	(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK			ComboListProc	(HWND, UINT, WPARAM, LPARAM);