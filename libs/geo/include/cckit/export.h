// export.h - Export macro definitions
#pragma once

// CCKIT is built as static library by default, no export macro needed
// If building as shared library, define CCKIT_BUILD_SHARED
#if defined(CCKIT_BUILD_SHARED) && (defined(_WIN32) || defined(__CYGWIN__))
    #ifdef CCKIT_BUILDING_DLL
        #define CCKIT_GEO_API __declspec(dllexport)
    #else
        #define CCKIT_GEO_API __declspec(dllimport)
    #endif
#else
    #define CCKIT_GEO_API
#endif

// C calling convention macro
#ifdef _WIN32
    #define C_CALL __stdcall
#else
    #define C_CALL
#endif
