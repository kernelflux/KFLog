// Copyright (c) 2026 KernelFlux. All rights reserved.

//
//  ThreadOperationQueue.h
//  MicroMessenger
//
//  Created by yerungui on 12-12-18.
//

#ifndef __MicroMessenger__ThreadOperationQueue__
#define __MicroMessenger__ThreadOperationQueue__

#import <Foundation/Foundation.h>

@interface ThreadQueue : NSObject
{}

+(BOOL) RunWithTarget:(id)target selector:(SEL)sel object:(id)arg;
@end


extern "C" BOOL RunWithTarget(void (*_funp)(void*), void* _arg);
extern "C" BOOL RunWithTargetNoParam(void (*_fun)());


#endif /* defined(__MicroMessenger__ThreadOperationQueue__) */
