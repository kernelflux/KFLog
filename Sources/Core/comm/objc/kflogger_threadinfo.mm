//
//  xloggr_threadinfo.m
//  MicroMessenger
//
//  Created by yerungui on 13-3-13.
//

#import <Foundation/NSThread.h>

#include <unistd.h>

extern "C"
{
    
intmax_t kflogger_pid()
{
    static intmax_t pid = getpid();
    return pid;
}

intmax_t kflogger_tid()
{
    return (intmax_t)[NSThread currentThread];
}

intmax_t kflogger_maintid()
{
    return (intmax_t)[NSThread mainThread];
}
    
}

void comm_export_symbols_5(){}
