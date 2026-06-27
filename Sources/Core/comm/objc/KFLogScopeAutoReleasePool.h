// Copyright (c) 2026 KernelFlux. All rights reserved.

//
//  scope_autoreleasepool.h
//  MicroMessenger
//
//  Created by yerungui on 12-11-30.
//

#ifndef __MicroMessenger__scope_autoreleasepool__
#define __MicroMessenger__scope_autoreleasepool__

class KFLogScopeAutoReleasePool {
  public:
    KFLogScopeAutoReleasePool();
    ~KFLogScopeAutoReleasePool();

  private:
    KFLogScopeAutoReleasePool(const KFLogScopeAutoReleasePool&);
    KFLogScopeAutoReleasePool& operator=(const KFLogScopeAutoReleasePool&);

  private:
    void* m_handle;
};

#define KFLOG_SCOPE_POOL() KFLogScopeAutoReleasePool __pool__##__LINE__

#endif /* defined(__MicroMessenger__scope_autoreleasepool__) */
