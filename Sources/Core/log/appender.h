// Copyright (c) 2026 KernelFlux. All rights reserved.


/*
 * appender.h
 *
 *  Created on: 2013-3-7
 *      Author: yerungui
 */

#ifndef KFLOG_APPENDER_H_
#define KFLOG_APPENDER_H_

#include <string>
#include <vector>
#include <stdint.h>

enum KFLogAppenderMode
{
    kKFLogAppenderAsync,
    kKFLogAppenderSync,
};

void kflog_appender_open(KFLogAppenderMode _mode, const char* _dir, const char* _nameprefix, const char* _pub_key);
void kflog_appender_open_with_cache(KFLogAppenderMode _mode, const std::string& _cachedir, const std::string& _logdir,
                              const char* _nameprefix, int _cache_days, const char* _pub_key);
void kflog_appender_flush();
void kflog_appender_flush_sync();
void kflog_appender_close();
void kflog_appender_setmode(KFLogAppenderMode _mode);
bool kflog_appender_getfilepath_from_timespan(int _timespan, const char* _prefix, std::vector<std::string>& _filepath_vec);
bool kflog_appender_make_logfile_name(int _timespan, const char* _prefix, std::vector<std::string>& _filepath_vec);
bool kflog_appender_get_current_log_path(char* _log_path, unsigned int _len);
bool kflog_appender_get_current_log_cache_path(char* _logPath, unsigned int _len);
void kflog_appender_set_console_log(bool _is_open);

/*
 * By default, all logs will write to one file everyday. You can split logs to multi-file by changing max_file_size.
 * 
 * @param _max_byte_size    Max byte size of single log file, default is 0, meaning do not split.
 */
void kflog_appender_set_max_file_size(uint64_t _max_byte_size);

/*
 * By default, all logs lives 10 days at most.
 *
 * @param _max_time    Max alive duration of a single log file in seconds, default is 10 days
 */
void kflog_appender_set_max_alive_duration(long _max_time);

#endif /* KFLOG_APPENDER_H_ */
