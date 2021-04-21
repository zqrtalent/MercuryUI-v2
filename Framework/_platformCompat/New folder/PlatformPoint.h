#ifdef __APPLE__
#include "PlatformPoint_MacOSX.h"
#elif _WINDOWS
#include "PlatformPoint_Win.h"
#else
#include "PlatformPoint_UNIX.h"
#endif
