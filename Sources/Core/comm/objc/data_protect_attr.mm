// Copyright (c) 2026 KernelFlux. All rights reserved.

//
//  data_protect_attr.m
//  comm
//
//  Created by yanguoyue on 15/9/18.
//
#import "comm/objc/data_protect_attr.h"

#include <TargetConditionals.h>

#if TARGET_OS_IPHONE
#import <Foundation/Foundation.h>
#endif

// If '_path' is directory, the function has effect on new file but ignores existed file.
bool setAttrProtectionNone(const char* _path) {
    
#if !TARGET_OS_IPHONE
    return true;
#else
    
    NSString* path = [[NSString alloc] initWithUTF8String:_path];
    NSFileManager* fileManager = [NSFileManager defaultManager];
    if (![fileManager fileExistsAtPath:path]) {
        return false;
    }

    BOOL ret = YES;
    NSDictionary* old_attr = [[NSFileManager defaultManager] attributesOfItemAtPath:path error:NULL];
    NSString* protection = [old_attr valueForKey:NSFileProtectionKey];
    if ([protection isEqualToString:NSFileProtectionNone] == NO) {
        NSDictionary* attr = [NSDictionary dictionaryWithObject:NSFileProtectionNone forKey:NSFileProtectionKey];
        ret = [fileManager setAttributes:attr ofItemAtPath:path error:nil];
    }
    return ret;
#endif
    
}
