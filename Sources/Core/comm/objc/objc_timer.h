// Copyright (c) 2026 KernelFlux. All rights reserved.

//
//  objc_timer.h
//  MicroMessenger
//
//  Created by yerungui  on 12-12-10.
//

#ifndef __MicroMessenger__objc_timer__
#define __MicroMessenger__objc_timer__

#include <sys/types.h>

bool StartAlarm(int64_t _id, int after);
bool StopAlarm(int64_t _id);

#endif /* defined(__MicroMessenger__objc_timer__) */
