#ifdef __APPLE__
#include "PlatformDeviceContext_MacOSX.h"
#elif _WINDOWS
#include "PlatformDeviceContext_Win.h"
#else
#include "PlatformDeviceContext_UNIX.h"
#endif
