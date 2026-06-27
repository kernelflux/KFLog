// Copyright (c) 2026 KernelFlux. All rights reserved.

#import "KFLog.h"
#import "../Core/log/appender.h"
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct KFLoggerInfoBridge {
    int level;
    const char* tag;
    const char* filename;
    const char* func_name;
    int line;
    struct timeval timeval;
    intmax_t pid;
    intmax_t tid;
    intmax_t maintid;
} KFLoggerInfoBridge;

void kflogger_SetLevel(int _level);
int  kflogger_IsEnabledFor(int _level);
void kflogger_Write(const KFLoggerInfoBridge* _info, const char* _log);
void kflogger_AssertP(const KFLoggerInfoBridge* _info, const char* _expression, const char* _format, ...);
void kflogger_Assert(const KFLoggerInfoBridge* _info, const char* _expression, const char* _log);

#ifdef __cplusplus
}
#endif

@implementation KFLogEngine

+ (void)open:(KFLogMode)mode logDir:(NSString *)dir namePrefix:(NSString *)prefix {
    [self open:mode logDir:dir namePrefix:prefix publicKey:nil];
}

+ (void)open:(KFLogMode)mode logDir:(NSString *)dir namePrefix:(NSString *)prefix publicKey:(NSString *)pubKey {
    kflog_appender_open((KFLogAppenderMode)mode,
                        [dir UTF8String],
                        [prefix UTF8String],
                        [pubKey UTF8String]);
}

+ (void)close {
    kflog_appender_close();
}

+ (void)flush {
    kflog_appender_flush();
}

+ (void)setLevel:(KFLogLevel)level {
    kflogger_SetLevel((int)level);
}

+ (BOOL)isEnabledForLevel:(KFLogLevel)level {
    return kflogger_IsEnabledFor((int)level) != 0;
}

+ (void)setConsoleLog:(BOOL)enabled {
    kflog_appender_set_console_log(enabled);
}

+ (void)setMaxFileSize:(uint64_t)size {
    kflog_appender_set_max_file_size(size);
}

+ (void)logWithLevel:(KFLogLevel)level
                 tag:(NSString *)tag
                file:(const char *)file
            function:(const char *)function
                line:(int)line
             message:(NSString *)message {
    KFLoggerInfoBridge info = {
        (int)level,
        [tag UTF8String],
        file,
        function,
        line,
        {0, 0},
        -1, -1, -1,
    };
    gettimeofday(&info.timeval, NULL);
    kflogger_Write(&info, [message UTF8String]);
}

+ (void)logWithLevel:(KFLogLevel)level
                 tag:(NSString *)tag
                file:(const char *)file
            function:(const char *)function
                line:(int)line
              format:(NSString *)format, ... {
    va_list args;
    va_start(args, format);
    NSString *message = [[NSString alloc] initWithFormat:format arguments:args];
    va_end(args);
    [self logWithLevel:level tag:tag file:file function:function line:line message:message];
}

@end
