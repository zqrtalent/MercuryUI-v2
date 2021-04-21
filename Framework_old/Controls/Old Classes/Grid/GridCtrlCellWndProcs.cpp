#include "stdafx.h"
#include "GridCtrlCellWndProcs.h"


//
//	[static] EditProcedure
//
INT_PTR CALLBACK
EditBoxProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
	{
	switch (msg)
		{
		case WM_GETDLGCODE:
			return DLGC_WANTALLKEYS;

		case WM_CHAR:
			{
			if (wp == VK_ESCAPE)
				{
				// mark	the	editbox	as beeing canceled
				SetWindowLong(hwnd,	GWL_USERDATA, MAKELONG(INPLACE_CANCEL, INPLACE_EDIT));
				goto _kill;
				}

			if (wp == VK_RETURN)
				{
				// mark	the	editbox	as NOT beeing canceled
				SetWindowLong(hwnd,	GWL_USERDATA, MAKELONG(INPLACE_OK, INPLACE_EDIT));

			_kill:
				// inform the parent that it should destroy this editbox
				::PostMessage(::GetParent(hwnd), WM_COMMAND, MAKELONG(IDC_INPLACEEDIT, EN_FINISH), (LPARAM)hwnd);
				return FALSE;
				}
			break;
			}
		}

	return CallWindowProc(mOldChildProc, hwnd, msg, wp, lp);
	}

//
//	[static] ComboBoxProc
//
INT_PTR CALLBACK
ComboBoxProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
	{
	switch (msg)
		{
		case WM_GETDLGCODE:
			return DLGC_WANTALLKEYS;

		case WM_COMMAND:
			// (asynchronously) finish inplace edit if the user clicked the dropped list
			if (HIWORD(wp) == LBN_SELCHANGE && mComboLBoxClicked)
				::PostMessage(hwnd, EN_FINISH, VK_RETURN, 0);
			break;

		case WM_CHAR:
		case EN_FINISH:
			{
			HWND ComboEx = ::GetParent(hwnd);

			if (wp == VK_ESCAPE)
				{
				// mark	the	combo box as beeing canceled
				::SetWindowLong(ComboEx, GWL_USERDATA, MAKELONG(INPLACE_CANCEL, INPLACE_COMBO));
				goto _kill;
				}

			if (wp == VK_RETURN)
				{
				// mark	the	combo box as NOT beeing canceled
				SetWindowLong(ComboEx, GWL_USERDATA, MAKELONG(INPLACE_OK, INPLACE_COMBO));

			_kill:
				// inform the parent that it should destroy this combo
				::PostMessage(::GetParent(ComboEx), WM_COMMAND, MAKELONG(IDC_INPLACEEDIT, EN_FINISH), (LPARAM)ComboEx);
				return FALSE;
				}
			break;
			}

		case WM_CTLCOLORLISTBOX:
			// subclass the ComboLBox
			if (mComboLBox != (HWND)lp)
				{
				mComboLBox = (HWND)lp;
				mOldComboListProc = (WNDPROC)(UINT64)::SetWindowLong(mComboLBox, GWL_WNDPROC, (LONG)(UINT64)ComboListProc);
				}
			break;

		case CB_SUICIDE:
			// destroy its parent (ComboBoxEx)
			::DestroyWindow(::GetParent(hwnd));
			return TRUE;
		}

	return CallWindowProc(mOldChildProc, hwnd, msg, wp, lp);
	}

//
//	[static] ComboEditProc
//
INT_PTR CALLBACK
ComboEditProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
	{
	switch (msg)
		{
		case WM_GETDLGCODE:
			return DLGC_WANTALLKEYS;

		case WM_KEYDOWN:
			{
			switch (wp)
				{
				//case VK_TAB: 
				case VK_ESCAPE:
				case VK_RETURN:
					{
					HWND Combo = ::GetParent(hwnd);
					::PostMessage(Combo, EN_FINISH, wp, 0);
					return 0;
					}
				}
			break;
			}

		case WM_KEYUP:
		case WM_CHAR:
			{
			switch (wp)
				{
				//case VK_TAB:
				case VK_ESCAPE:
				case VK_RETURN:
					return 0;
				}
			}
		}

	// Call the original window procedure for default processing. 
	return CallWindowProc(mOldComboEditProc, hwnd, msg, wp, lp);
	}

//
//	[static] ComboListProc
//
INT_PTR CALLBACK
ComboListProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
	{
	switch (msg)
		{
		case WM_LBUTTONDOWN:
			{
			// remember that the user clicked the ComboLBox
			mComboLBoxClicked = true;
			break;
			}

		case WM_DESTROY:
			mComboLBox = NULL;
			mComboLBoxClicked = false;
			break;
		}

	// Call the original window procedure for default processing. 
	return ::CallWindowProc(mOldComboListProc, hwnd, msg, wp, lp);
	}