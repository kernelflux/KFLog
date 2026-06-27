/*
 ============================================================================
 Name        : xloggerbase.c
 ============================================================================
 */

#include "comm/kflogger/kfloggerbase.h"
#include <stdio.h>

#include "comm/compiler_util.h"

WEAK_FUNC  KFLogLevel   __kflogger_Level_impl();
WEAK_FUNC  void        __kflogger_SetLevel_impl(KFLogLevel _level);
WEAK_FUNC  int         __kflogger_IsEnabledFor_impl(KFLogLevel _level);
WEAK_FUNC kflogger_appender_t __kflogger_SetAppender_impl(kflogger_appender_t _appender);
WEAK_FUNC void __kflogger_Write_impl(const KFLoggerInfo* _info, const char* _log);
WEAK_FUNC void __kflogger_VPrint_impl(const KFLoggerInfo* _info, const char* _format, va_list _list);

WEAK_FUNC void __kflogger_AssertP_impl(const KFLoggerInfo* _info, const char* _expression, const char* _format, va_list _list);
WEAK_FUNC void __kflogger_Assert_impl(const KFLoggerInfo* _info, const char* _expression, const char* _log);

#ifndef WIN32
WEAK_FUNC const char* kflogger_dump(const void* _dumpbuffer, size_t _len) { return "";}
#endif

KFLogLevel   kflogger_Level() {
    if (NULL == &__kflogger_Level_impl)  return kKFLogLevelNone;
	return __kflogger_Level_impl();
}

void kflogger_SetLevel(KFLogLevel _level){
    if (NULL != &__kflogger_SetLevel_impl)
        __kflogger_SetLevel_impl(_level);
}

int  kflogger_IsEnabledFor(KFLogLevel _level) {
    if (NULL == &__kflogger_IsEnabledFor_impl) { return 0;}
	return __kflogger_IsEnabledFor_impl(_level);

}

kflogger_appender_t kflogger_SetAppender(kflogger_appender_t _appender) {
    if (NULL == &__kflogger_SetAppender_impl) { return NULL;}
    return __kflogger_SetAppender_impl(_appender);
}

void kflogger_Write(const KFLoggerInfo* _info, const char* _log) {
	if (NULL != &__kflogger_Write_impl)
		__kflogger_Write_impl(_info, _log);
}

void kflogger_VPrint(const KFLoggerInfo* _info, const char* _format, va_list _list) {
	if (NULL != &__kflogger_VPrint_impl)
		__kflogger_VPrint_impl(_info, _format, _list);
}

void kflogger_Print(const KFLoggerInfo* _info, const char* _format, ...) {
	if (NULL == &__kflogger_VPrint_impl){ return; }
    
	va_list valist;
	va_start(valist, _format);
    __kflogger_VPrint_impl(_info, _format, valist);
	va_end(valist);
}


void kflogger_AssertP(const KFLoggerInfo* _info, const char* _expression, const char* _format, ...) {
	if (NULL == &__kflogger_AssertP_impl)  { return; }
    
	va_list valist;
	va_start(valist, _format);
	__kflogger_AssertP_impl(_info, _expression, _format, valist);
	va_end(valist);
}

void kflogger_Assert(const KFLoggerInfo* _info, const char* _expression, const char* _log) {
    if (NULL != &__kflogger_Assert_impl)
    	__kflogger_Assert_impl(_info, _expression, _log);
}


#ifndef USING_XLOG_WEAK_FUNC
static KFLogLevel gs_level = kKFLogLevelNone;
static kflogger_appender_t gs_appender = NULL;

KFLogLevel   __kflogger_Level_impl() {return gs_level;}
void        __kflogger_SetLevel_impl(KFLogLevel _level){ gs_level = _level;}
int         __kflogger_IsEnabledFor_impl(KFLogLevel _level) {return gs_level <= _level;}

kflogger_appender_t __kflogger_SetAppender_impl(kflogger_appender_t _appender)  {
    kflogger_appender_t old_appender = gs_appender;
    gs_appender = _appender;
    return old_appender;
}

void __kflogger_Write_impl(const KFLoggerInfo* _info, const char* _log) {
    
    if (!gs_appender) return;
    
    if (_info && -1==_info->pid && -1==_info->tid && -1==_info->maintid)
    {
        KFLoggerInfo* info = (KFLoggerInfo*)_info;
        info->pid = kflogger_pid();
        info->tid = kflogger_tid();
        info->maintid = kflogger_maintid();
    }
    
    if (NULL == _log) {
        if (_info) {
            KFLoggerInfo* info = (KFLoggerInfo*)_info;
            info->level = kKFLogLevelFatal;
        }
        gs_appender(_info, "NULL == _log");
    } else {
        gs_appender(_info, _log);
    }
}

void __kflogger_VPrint_impl(const KFLoggerInfo* _info, const char* _format, va_list _list) {
    if (NULL == _format) {
        KFLoggerInfo* info = (KFLoggerInfo*)_info;
        info->level = kKFLogLevelFatal;
        __kflogger_Write_impl(_info, "NULL == _format");
    } else {
        char temp[4096] = {'\0'};
        vsnprintf(temp, 4096, _format, _list);
        __kflogger_Write_impl(_info, temp);
    }
}

extern void __ASSERTV2(const char * _pfile, int _line, const char * _pfunc, const char * _pexpression, const char * _format, va_list _list);
void __kflogger_AssertP_impl(const KFLoggerInfo* _info, const char* _expression, const char* _format, va_list _list) {
    __ASSERTV2(_info->filename, _info->line, _info->func_name, _expression, _format, _list);
}

extern void __ASSERT2(const char * _pfile, int _line, const char * _pfunc, const char * _pexpression, const char * _format, ...);
void __kflogger_Assert_impl(const KFLoggerInfo* _info, const char* _expression, const char* _log) {
    __ASSERT2(_info->filename, _info->line, _info->func_name, _expression, _log);
    
}
#endif
