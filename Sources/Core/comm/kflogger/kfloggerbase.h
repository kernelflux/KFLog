// Copyright (c) 2026 KernelFlux. All rights reserved.


/*
 ============================================================================
 ============================================================================
 */

#ifndef KFLOGBASE_H_
#define KFLOGBASE_H_

#include <sys/time.h>
#include <time.h>
#include <stdarg.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    kKFLogLevelAll = 0,
    kKFLogLevelVerbose = 0,
    kKFLogLevelDebug,    // Detailed information on the flow through the system.
    kKFLogLevelInfo,     // Interesting runtime events (startup/shutdown), should be conservative and keep to a minimum.
    kKFLogLevelWarn,     // Other runtime situations that are undesirable or unexpected, but not necessarily "wrong".
    kKFLogLevelError,    // Other runtime errors or unexpected conditions.
    kKFLogLevelFatal,    // Severe errors that cause premature termination.
    kKFLogLevelNone,     // Special level used to disable all log messages.
} KFLogLevel;

typedef struct KFLoggerInfo {
    KFLogLevel level;
    const char* tag;
    const char* filename;
    const char* func_name;
    int line;

    struct timeval timeval;
    intmax_t pid;
    intmax_t tid;
    intmax_t maintid;
} KFLoggerInfo;

extern intmax_t kflogger_pid();
extern intmax_t kflogger_tid();
extern intmax_t kflogger_maintid();
typedef void (*kflogger_appender_t)(const KFLoggerInfo* _info, const char* _log);
extern const char* kflogger_dump(const void* _dumpbuffer, size_t _len);

KFLogLevel   kflogger_Level();
void kflogger_SetLevel(KFLogLevel _level);
int  kflogger_IsEnabledFor(KFLogLevel _level);
kflogger_appender_t kflogger_SetAppender(kflogger_appender_t _appender);

// no level filter
#ifdef __GNUC__
__attribute__((__format__(printf, 3, 4)))
#endif
void        kflogger_AssertP(const KFLoggerInfo* _info, const char* _expression, const char* _format, ...);
void        kflogger_Assert(const KFLoggerInfo* _info, const char* _expression, const char* _log);
#ifdef __GNUC__
__attribute__((__format__(printf, 2, 0)))
#endif
void        kflogger_VPrint(const KFLoggerInfo* _info, const char* _format, va_list _list);
#ifdef __GNUC__
__attribute__((__format__(printf, 2, 3)))
#endif
void        kflogger_Print(const KFLoggerInfo* _info, const char* _format, ...);
void        kflogger_Write(const KFLoggerInfo* _info, const char* _log);

#ifdef __cplusplus
}
#endif

#endif
