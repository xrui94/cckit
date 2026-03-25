// log.cpp - 日志后端实现
#include "cckit/logging/log.h"
#include "cckit/export.h"
#include <spdlog/spdlog.h>

extern "C" {

void cckit_log_set_level(cckit_log_level_t level)
{
    spdlog::level::level_enum spdlog_level;
    switch (level)
    {
    case CCKIT_LOG_TRACE:    spdlog_level = spdlog::level::trace; break;
    case CCKIT_LOG_DEBUG:    spdlog_level = spdlog::level::debug; break;
    case CCKIT_LOG_INFO:     spdlog_level = spdlog::level::info; break;
    case CCKIT_LOG_WARN:     spdlog_level = spdlog::level::warn; break;
    case CCKIT_LOG_ERROR:    spdlog_level = spdlog::level::err; break;
    case CCKIT_LOG_CRITICAL: spdlog_level = spdlog::level::critical; break;
    case CCKIT_LOG_OFF:      spdlog_level = spdlog::level::off; break;
    default:                spdlog_level = spdlog::level::info; break;
    }
    spdlog::set_level(spdlog_level);
}

void cckit_log_trace(const char* msg)    { SPDLOG_TRACE(msg); }
void cckit_log_debug(const char* msg)    { SPDLOG_DEBUG(msg); }
void cckit_log_info(const char* msg)     { SPDLOG_INFO(msg); }
void cckit_log_warn(const char* msg)     { SPDLOG_WARN(msg); }
void cckit_log_error(const char* msg)    { SPDLOG_ERROR(msg); }
void cckit_log_critical(const char* msg) { SPDLOG_CRITICAL(msg); }

} // extern "C"
