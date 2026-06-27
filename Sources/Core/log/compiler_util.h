// Copyright (c) 2026 KernelFlux. All rights reserved.


/*
 * compiler_util.h
 *
 *  Created on: 2017-3-28
 *      Author: jehoochen
 */

#ifndef LOG_COMPILER_UTIL_H_
#define LOG_COMPILER_UTIL_H_

#if defined(_MSC_VER) && defined(KFLOG_USE_DLLS)
#ifdef KFLOG_LOG_EXPORTS
#define KFLOG_LOG_EXPORT __declspec(dllexport)
#else
#define KFLOG_LOG_EXPORT __declspec(dllimport)
#endif
#else
#define KFLOG_LOG_EXPORT
#endif

#endif /* LOG_COMPILER_UTIL_H_ */
