#pragma once

#ifdef __INTEL_COMPILER
#   pragma warning( disable : 181  ) // argument is incompatible with corresponding format string conversion
#endif

// We define the minimum supported platform to be Windows Vista SP2.
// Windows Vista SP2 has the Platform Upgrade that implements Direct2D.
#define NTDDI_VERSION  NTDDI_VISTASP2
#define WINVER         0x0600 
#define _WIN32_WINNT   0x0600
#define _WIN32_IE      0x0800

#include <SDKDDKVer.h>

// EOF 
