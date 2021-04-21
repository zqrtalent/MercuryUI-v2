#include "PlatformCursor.h"
#ifdef _WINDOWS
#elif __APPLE__
    #include <Appkit/NSCursor.h>
#endif


_Point
_Cursor::GetCurrentPos(HWND toClient, bool dontReverseOY /*= false*/){
    
#ifdef _WINDOWS
    _Point pt;
    ::GetCursorPos(&pt);
    if(toClient != NULL)
        ::ScreenToClient(toClient, &pt);
    return pt;
#elif __APPLE__
    NSPoint pt = [NSEvent mouseLocation]; // Global coords
    if(toClient != nil){
        pt = [[toClient window] convertRectFromScreen:CGRectMake(pt.x, pt.y, 0, 0)].origin;
        // Change according used coords system (top left).
        if(!dontReverseOY)
            pt.y = [toClient bounds].size.height - pt.y;
    }
    return _Point(pt.x, pt.y);
#endif
    return _Point(0,0);
}

/*
 (NSCursor *)arrowCursor;
 + (NSCursor *)IBeamCursor;
 + (NSCursor *)pointingHandCursor;
 + (NSCursor *)closedHandCursor;
 + (NSCursor *)openHandCursor;
 + (NSCursor *)resizeLeftCursor;
 + (NSCursor *)resizeRightCursor;
 + (NSCursor *)resizeLeftRightCursor;
 + (NSCursor *)resizeUpCursor;
 + (NSCursor *)resizeDownCursor;
 + (NSCursor *)resizeUpDownCursor;
 + (NSCursor *)crosshairCursor;
 + (NSCursor *)disappearingItemCursor;
 + (NSCursor *)operationNotAllowedCursor NS_AVAILABLE_MAC(10_5);
 + (NSCursor *)dragLinkCursor NS_AVAILABLE_MAC(10_6);
 + (NSCursor *)dragCopyCursor NS_AVAILABLE_MAC(10_6);
 + (NSCursor *)contextualMenuCursor NS_AVAILABLE_MAC(10_6);
 + (NSCursor *)IBeamCursorForVerticalLayout NS_AVAILABLE_MAC(10_7);
 
 case ESChildControl::ChildControlHitTest::LeftCenter:
 case ESChildControl::ChildControlHitTest::RightCenter:{
 _Cursor::SetCursor(_Cursor::SystemCursorType::SizeWE);
 return 1;
 }
 case ESChildControl::ChildControlHitTest::MiddleTop:
 case ESChildControl::ChildControlHitTest::MiddleBottom:{
 _Cursor::SetCursor(_Cursor::SystemCursorType::SizeNS);
 return 1;
 }
 case ESChildControl::ChildControlHitTest::LeftTop:
 case ESChildControl::ChildControlHitTest::RightBottom:{
 _Cursor::SetCursor(_Cursor::SystemCursorType::SizeNWSE);
 return 1;
 }
 case ESChildControl::ChildControlHitTest::RightTop:
 case ESChildControl::ChildControlHitTest::LeftBottom:{
 _Cursor::SetCursor(_Cursor::SystemCursorType::SizeNESW);
 return 1;
 }
 */

bool
_Cursor::SetCursor(SystemCursorType cursorType){
    switch (cursorType) {
        case SystemCursorType::Arrow:{
#ifdef _WINDOWS
            ::SetCursor(LoadCursor(NULL, IDC_ARROW));
#elif __APPLE__
            [[NSCursor arrowCursor] set];
#endif
            break;
        }
        case SystemCursorType::Hand:{
#ifdef _WINDOWS
            ::SetCursor(LoadCursor(NULL, IDC_HAND));
#elif __APPLE__
            [[NSCursor pointingHandCursor] set];
#endif
            break;
        }
        case SystemCursorType::SizeNS:{
#ifdef _WINDOWS
            ::SetCursor(LoadCursor(NULL, IDC_SIZENS));
#elif __APPLE__
            [[NSCursor resizeUpDownCursor] set];
#endif
            break;
        }
        case SystemCursorType::SizeNESW:{
#ifdef _WINDOWS
            ::SetCursor(LoadCursor(NULL, IDC_SIZENESW));
#elif __APPLE__
            //[[NSCursor resizeLeftRightCursor] set];
            [[NSCursor arrowCursor] set];
#endif
            break;
        }
        case SystemCursorType::SizeNWSE:{
#ifdef _WINDOWS
            ::SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
#elif __APPLE__
            //[[NSCursor IBeamCursor] set];
            [[NSCursor arrowCursor] set];
#endif
            break;
        }
        case SystemCursorType::SizeWE:{
#ifdef _WINDOWS
            ::SetCursor(LoadCursor(NULL, IDC_SIZEWE));
#elif __APPLE__
            [[NSCursor resizeLeftRightCursor] set];
#endif
            break;
        }
        case SystemCursorType::SizeAll:{
#ifdef _WINDOWS
            ::SetCursor(LoadCursor(NULL, IDC_SIZEALL));
#elif __APPLE__
            [[NSCursor crosshairCursor] set];
#endif
            break;
        }
        case SystemCursorType::Beam:{
#ifdef _WINDOWS
            ::SetCursor(LoadCursor(NULL, IDC_BEAM));
#elif __APPLE__
            [[NSCursor IBeamCursor] set];
#endif
            break;
        }
        default:
            return false;
    }
    return true;
}
