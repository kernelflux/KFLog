//
//  scope_autoreleasepool.cpp
//  MicroMessenger
//
//  Created by yerungui on 12-11-30.
//

#include "comm/objc/KFLogScopeAutoReleasePool.h"

extern "C" {
void* objc_autoreleasePoolPush(void);
void objc_autoreleasePoolPop(void*);
}

KFLogScopeAutoReleasePool::KFLogScopeAutoReleasePool()
: m_handle(objc_autoreleasePoolPush())
{
}

KFLogScopeAutoReleasePool::~KFLogScopeAutoReleasePool()
{
    objc_autoreleasePoolPop(m_handle);
}

void comm_export_symbols_3(){}
