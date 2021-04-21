//
//  PlatformCompat.h
//  TestApp
//
//  Created by Zaqro Butskhrikidze on 7/30/14.
//  Copyright (c) 2014 Zaqro Butskhrikidze. All rights reserved.
//

#ifndef TestApp_PlatformCompat_h
#define TestApp_PlatformCompat_h
#include <assert.h>

#define BOOL unsigned int
#define HANDLE void*
#define UINT unsigned int
#define __int64 int64_t
#define DWORD unsigned int
#define ASSERT(x) assert(x)
#define TRUE 1
#define FALSE 0
#define BYTE unsigned char
#define LPCTSTR const char*

#if defined(__x86_64__) || defined(_M_X64)
/* x86 64-bit ----------------------------------------------- */
#define PTR_HOLDER_INTEGER int64_t

#elif defined(__i386) || defined(_M_IX86)
/* x86 32-bit ----------------------------------------------- */
#define PTR_HOLDER_INTEGER long
#endif

#define _T(x) x
#define TCHAR char

typedef struct RECT_tag
{
    int left;
    int top;
    int right;
    int bottom;
}RECT;

typedef struct POINT_tag
{
    int x;
    int y;
}POINT;

typedef struct SIZE_tag
{
    int cx;
    int cy;
}SIZE;

#include <string>
class stringEx
{
public:
    static void MakeUpper(std::string& str)
    {
        for(std::string::iterator i=str.begin(); i!=str.end(); ++i)
        {
            *i = toupper(*i);
        }
    };
    
    static void MakeLower(std::string& str)
    {
        for(std::string::iterator i=str.begin(); i!=str.end(); ++i)
        {
            *i = tolower(*i);
        }
    };
    
    static const TCHAR* GetBuffer(std::string& str)
    {
        return str.c_str();
    }
};


#endif
