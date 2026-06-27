// Copyright (c) 2026 KernelFlux. All rights reserved.


/*
 * bootrun.h
 *
 *  Created on: 2014-7-9
 *      Author: yerungui
 */

#ifndef COMM_BOOTRUN_H_
#define COMM_BOOTRUN_H_

#include <stdlib.h>
#include "comm/compiler_util.h"

#ifdef __cplusplus
extern "C" {
#endif

__inline int __boot_run_atstartup(void (*func)(void)) { func(); return 0;}
__inline int __boot_run_atexit(void (*func)(void)) { return atexit(func);}

#ifdef __cplusplus
}
#endif

#define BOOT_RUN_STARTUP(func) VARIABLE_IS_NOT_USED static int __anonymous_run_variable_startup_##func = __boot_run_atstartup(func)
#define BOOT_RUN_EXIT(func) VARIABLE_IS_NOT_USED static int __anonymous_run_variable_exit_##func = __boot_run_atexit(func)

#define BOOT_RUN_BLOCK_START _BOOT_RUN_BLOCK_START(__LINE__)
#define BOOT_RUN_BLOCK_END if (0); else return 0;}();

//////////detail impl/////////////
#define __BOOT_RUN_BLOCK_START(line) VARIABLE_IS_NOT_USED static int __anonymous_boot_run_block_variable_##line = []() {
#define _BOOT_RUN_BLOCK_START(line) __BOOT_RUN_BLOCK_START(line)


#endif /* COMM_BOOTRUN_H_ */
