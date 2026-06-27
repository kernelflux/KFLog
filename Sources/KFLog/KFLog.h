// Copyright (c) 2026 KernelFlux. All rights reserved.

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, KFLogLevel) {
    KFLogLevelAll = 0,
    KFLogLevelVerbose = 0,
    KFLogLevelDebug,
    KFLogLevelInfo,
    KFLogLevelWarn,
    KFLogLevelError,
    KFLogLevelFatal,
    KFLogLevelNone,
};

typedef NS_ENUM(NSInteger, KFLogMode) {
    KFLogModeAsync = 0,
    KFLogModeSync,
};

@interface KFLogEngine : NSObject

+ (void)open:(KFLogMode)mode logDir:(NSString *)dir namePrefix:(NSString *)prefix;
+ (void)open:(KFLogMode)mode logDir:(NSString *)dir namePrefix:(NSString *)prefix publicKey:(nullable NSString *)pubKey;
+ (void)close;
+ (void)flush;
+ (void)setLevel:(KFLogLevel)level NS_SWIFT_NAME(setLevel(_:));
+ (BOOL)isEnabledForLevel:(KFLogLevel)level;
+ (void)setConsoleLog:(BOOL)enabled NS_SWIFT_NAME(setConsoleLog(_:));
+ (void)setMaxFileSize:(uint64_t)size NS_SWIFT_NAME(setMaxFileSize(_:));

+ (void)logWithLevel:(KFLogLevel)level
                 tag:(NSString *)tag
                file:(const char * _Nullable)file
            function:(const char * _Nullable)function
                line:(int)line
             message:(NSString *)message NS_SWIFT_NAME(log(level:tag:file:function:line:message:));

+ (void)logWithLevel:(KFLogLevel)level
                 tag:(NSString *)tag
                file:(const char * _Nullable)file
            function:(const char * _Nullable)function
                line:(int)line
              format:(NSString *)format, ... NS_FORMAT_FUNCTION(6, 7);

@end

NS_ASSUME_NONNULL_END
