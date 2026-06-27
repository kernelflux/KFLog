#import <Foundation/Foundation.h>
#include "comm/kflogger/kfloggerbase.h"

void KFLogConsole(const KFLoggerInfo* _info, const char* _log) {
    if (!_info || !_log) return;
    NSLog(@"[%s] %s", _info->tag ?: "", _log);
}
