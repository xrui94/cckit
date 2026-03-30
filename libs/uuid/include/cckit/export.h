// export.h - 导出宏定义
#pragma once

#ifndef CCKIT_API_DEFINED
#define CCKIT_API_DEFINED

// CCKIT 默认编译为静态库，不需要导出宏
// 如果需要编译为动态库，定义 CCKIT_BUILD_SHARED
#if defined(CCKIT_BUILD_SHARED) && (defined(_WIN32) || defined(__CYGWIN__))
    #ifdef CCKIT_BUILDING_DLL
        #define CCKIT_API __declspec(dllexport)
    #else
        #define CCKIT_API __declspec(dllimport)
    #endif
#else
    #define CCKIT_API
#endif

#endif // CCKIT_API_DEFINED

// C 调用约定宏
#ifndef C_CALL_DEFINED
#define C_CALL_DEFINED
#ifdef _WIN32
    #define C_CALL __stdcall
#else
    #define C_CALL
#endif
#endif // C_CALL_DEFINED
