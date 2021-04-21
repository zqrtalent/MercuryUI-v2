#if !defined(PLATFORMCURSOR_H_INCLUDED)
#define      PLATFORMCURSOR_H_INCLUDED

#include "PlatformPoint.h"

class _Cursor
{
public:
    enum SystemCursorType
    {
        Arrow = 1,
        SizeWE,
        SizeNS,
        SizeNWSE,
        SizeNESW,
        SizeAll,
        Hand,
        Beam
    };
    
    static _Point GetCurrentPos(HWND toClient, bool dontReverseOY = false);
    static bool SetCursor(SystemCursorType cursorType);
};
#endif // !defined(PLATFORMCURSOR_H_INCLUDED)

