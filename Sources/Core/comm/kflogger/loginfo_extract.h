// Copyright (c) 2026 KernelFlux. All rights reserved.

/*
 * loginfo_extract.h
 *
 *  Created on: 2016年10月13日
 *      Author: yanguoyue
 */

#ifndef KFLOG_LOGINFO_EXTRACT_H_
#define KFLOG_LOGINFO_EXTRACT_H_

#ifdef __cplusplus
extern "C" {
#endif

const char* ExtractFileName(const char* _path);

void ExtractFunctionName(const char* _func, char* _func_ret, int _len);


#ifdef __cplusplus
}
#endif
#endif /* KFLOG_LOGINFO_EXTRACT_H_ */
