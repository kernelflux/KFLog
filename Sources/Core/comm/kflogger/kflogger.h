// Copyright (c) 2026 KernelFlux. All rights reserved.

/*
 ============================================================================
 Name		: xlogger.h
 ============================================================================
 */

#ifndef KFLOG_H_
#define KFLOG_H_

#include <stdlib.h>
#include <assert.h>
#include <sys/cdefs.h>
#include <sys/time.h>

#include "comm/string_cast.h"
#include "kfloggerbase.h"
#include "preprocessor.h"

#ifdef KFLOG_DISABLE
#define  kflogger_IsEnabledFor(_level)	(false)
#define  kflogger_AssertP(...)			((void)0)
#define  kflogger_Assert(...)			((void)0)
#define  kflogger_VPrint(...)			((void)0)
#define  kflogger_Print(...)				((void)0)
#define  kflogger_Write(...)				((void)0)
#endif

#ifdef __cplusplus
#include <string>

template <bool x> struct KFLOG_STATIC_ASSERTION_FAILURE;
template <> struct KFLOG_STATIC_ASSERTION_FAILURE<true> { enum { value = 1 }; };
template<int x> struct kflogger_static_assert_test{};


#define KFLOG_STATIC_ASSERT( ... ) typedef ::kflogger_static_assert_test<\
										sizeof(::KFLOG_STATIC_ASSERTION_FAILURE< ((__VA_ARGS__) == 0 ? false : true) >)>\
										PP_CAT(boost_static_assert_typedef_, __LINE__)


const struct KFLogTypeSafeFormat {KFLogTypeSafeFormat(){}} __tsf__;
const struct KFLoggerTag {KFLoggerTag(){}} __kflogger_tag__;
const struct KFLogInfoNull {KFLogInfoNull(){}} __kflogger_info_null__;


class KFLogMessage {
public:
	KFLogMessage(): m_message() { m_message.reserve(512); }
	KFLogMessage(std::string& _holder):m_message(_holder){}
	~KFLogMessage() {}

public:
	const std::string& Message() const { return m_message;}
	std::string& Message() { return m_message;}

	const std::string& String() const { return m_message;}
	std::string& String() { return m_message;}

#ifdef __GNUC__
	__attribute__((__format__ (printf, 2, 0)))
#endif
	KFLogMessage&  WriteNoFormat(const char* _log) { m_message+= _log; return *this;}
#ifdef __GNUC__
	__attribute__((__format__ (printf, 3, 0)))
#endif
	KFLogMessage&  WriteNoFormat(const KFLogTypeSafeFormat&, const char* _log) { m_message+= _log; return *this;}

	KFLogMessage& operator<<(const string_cast& _value);
	KFLogMessage& operator>>(const string_cast& _value);

	KFLogMessage& operator()() {return *this;}
	void operator+=(const string_cast& _value) { m_message += _value.str();}
#ifdef __GNUC__
	__attribute__((__format__ (printf, 2, 3)))
#endif
	KFLogMessage& operator()(const char* _format, ...);

#ifdef __GNUC__
	__attribute__((__format__ (printf, 2, 0)))
#endif
	KFLogMessage& VPrintf(const char* _format, va_list _list);

#define KFLOG_FORMAT_ARGS(n) PP_ENUM_TRAILING_PARAMS(n, const string_cast& a)
	KFLogMessage&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(0));
	KFLogMessage&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(1));
	KFLogMessage&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(2));
	KFLogMessage&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(3));
	KFLogMessage&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(4));
	KFLogMessage&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(5));
	KFLogMessage&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(6));
	KFLogMessage&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(7));
	KFLogMessage&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(8));
	KFLogMessage&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(9));
	KFLogMessage&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(10));
	KFLogMessage&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(11));
	KFLogMessage&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(12));
	KFLogMessage&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(13));
	KFLogMessage&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(14));
	KFLogMessage&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(15));
	KFLogMessage&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(16));
#undef KFLOG_FORMAT_ARGS

private:
	void DoKFLogTypeSafeFormat(const char* _format, const string_cast** _args);

private:
//	  KFLogMessage(const KFLogMessage&);
//	  KFLogMessage& operator=(const KFLogMessage&);

private:
	std::string m_message;
};

class KFLogger {
public:
	KFLogger(KFLogLevel _level, const char* _tag, const char* _file, const char* _func, int _line, bool (*_hook)(KFLoggerInfo& _info, std::string& _log))
	:m_info(), m_message(), m_isassert(false), m_exp(NULL),m_hook(_hook), m_isinfonull(false) {
		m_info.level = _level;
		m_info.tag = _tag;
		m_info.filename = _file;
		m_info.func_name = _func;
		m_info.line = _line;
		m_info.timeval.tv_sec = 0;
		m_info.timeval.tv_usec = 0;
		m_info.pid = -1;
		m_info.tid = -1;
		m_info.maintid = -1;

		m_message.reserve(512);
	}
	
	~KFLogger() {
		if (!m_isassert && m_message.empty()) return;

		gettimeofday(&m_info.timeval, NULL);
		if (m_hook && !m_hook(m_info, m_message)) return;
		
		if (m_isassert)
			kflogger_Assert(m_isinfonull?NULL:&m_info, m_exp, m_message.c_str());
		else
			kflogger_Write(m_isinfonull?NULL:&m_info, m_message.c_str());
	}

public:
	KFLogger& Assert(const char* _exp) {
		m_isassert = true;
		m_exp = _exp;
		return *this;
	}
	
	bool Empty() const { return !m_isassert && m_message.empty();}
	const std::string& Message() const { return m_message;}

#ifdef __GNUC__
	__attribute__((__format__ (printf, 2, 0)))
#endif
	KFLogger&  WriteNoFormat(const char* _log) { m_message+= _log; return *this;}
#ifdef __GNUC__
	 __attribute__((__format__ (printf, 3, 0)))
#endif
	KFLogger&  WriteNoFormat(const KFLogTypeSafeFormat&, const char* _log) { m_message+= _log; return *this;}

	KFLogger& operator<<(const string_cast& _value);
	KFLogger& operator>>(const string_cast& _value);

	void operator>> (KFLogger& _xlogger) {
		if (_xlogger.m_info.level < m_info.level)
		{
			_xlogger.m_info.level = m_info.level;
			_xlogger.m_isassert = m_isassert;
			_xlogger.m_exp = m_exp;
		}

		m_isassert = false;
		m_exp = NULL;

		_xlogger.m_message += m_message;
		m_message.clear();
	}

	void operator<< (KFLogger& _xlogger) {
		_xlogger.operator>>(*this);
	}

	KFLogger& operator()() { return *this; }
	KFLogger& operator()(const KFLogInfoNull&) { m_isinfonull = true; return *this;}
	KFLogger& operator()(const KFLoggerTag&, const char* _tag) { m_info.tag = _tag; return *this;}
#ifdef __GNUC__
	__attribute__((__format__ (printf, 2, 3)))
#endif
	KFLogger& operator()(const char* _format, ...);

#ifdef __GNUC__
	 __attribute__((__format__ (printf, 2, 0)))
#endif
	KFLogger& VPrintf(const char* _format, va_list _list);

#define KFLOG_FORMAT_ARGS(n) PP_ENUM_TRAILING_PARAMS(n, const string_cast& a)
	KFLogger&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(0));
	KFLogger&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(1));
	KFLogger&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(2));
	KFLogger&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(3));
	KFLogger&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(4));
	KFLogger&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(5));
	KFLogger&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(6));
	KFLogger&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(7));
	KFLogger&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(8));
	KFLogger&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(9));
	KFLogger&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(10));
	KFLogger&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(11));
	KFLogger&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(12));
	KFLogger&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(13));
	KFLogger&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(14));
	KFLogger&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(15));
	KFLogger&  operator()(const KFLogTypeSafeFormat&, const char*_format KFLOG_FORMAT_ARGS(16));
#undef KFLOG_FORMAT_ARGS

private:
	void DoKFLogTypeSafeFormat(const char* _format, const string_cast** _args);
	
private:
	KFLogger(const KFLogger&);
	KFLogger& operator=(const KFLogger&);
	
private:
	KFLoggerInfo m_info;
	std::string m_message;
	bool m_isassert;
	const char* m_exp;
	bool (*m_hook)(KFLoggerInfo& _info, std::string& _log);
	bool m_isinfonull;
};


class KFLogScopeTracer {
public:
	KFLogScopeTracer(KFLogLevel _level, const char* _tag, const char* _name, const char* _file, const char* _func, int _line, const char* _log)
	:m_enable(kflogger_IsEnabledFor(_level)), m_info(), m_tv() {
		m_info.level = _level;

		if (m_enable) {
			m_info.tag = _tag;
			m_info.filename = _file;
			m_info.func_name = _func;
			m_info.line = _line;
			gettimeofday(&m_info.timeval, NULL);
			m_info.pid = -1;
			m_info.tid = -1;
			m_info.maintid = -1;

			strncpy(m_name, _name, sizeof(m_name));
			m_name[sizeof(m_name)-1] = '\0';

			m_tv = m_info.timeval;
			char strout[1024] = {'\0'};
			snprintf(strout, sizeof(strout), "-> %s %s", m_name, NULL!=_log? _log:"");
			kflogger_Write(&m_info, strout);
		}
	}

	~KFLogScopeTracer() {
		if (m_enable) {
			timeval tv;
			gettimeofday(&tv, NULL);
			m_info.timeval = tv;
			long timeSpan = (tv.tv_sec - m_tv.tv_sec) * 1000 + (tv.tv_usec - m_tv.tv_usec) / 1000;
			char strout[1024] = {'\0'};
			snprintf(strout, sizeof(strout), "<- %s +%ld, %s", m_name, timeSpan, m_exitmsg.c_str());
			kflogger_Write(&m_info, strout);
		}
	}
	
	void Exit(const std::string& _exitmsg) { m_exitmsg += _exitmsg; }
	
private:
	KFLogScopeTracer(const KFLogScopeTracer&);
	KFLogScopeTracer& operator=(const KFLogScopeTracer&);

private:
	bool m_enable;
	KFLoggerInfo m_info;
	char m_name[128];
	timeval m_tv;
	
	std::string m_exitmsg;
};

///////////////////////////KFLogMessage////////////////////
inline KFLogMessage& KFLogMessage::operator<< (const string_cast& _value) {
	if (NULL != _value.str()) {
		m_message += _value.str();
	} else {
		assert(false);
	}
	return *this;
}

inline KFLogMessage& KFLogMessage::operator>> (const string_cast& _value) {
	if (NULL != _value.str()) {
		m_message.insert(0,  _value.str());
	} else {
		assert(false);
	}
	return *this;
}

inline KFLogMessage& KFLogMessage::VPrintf(const char* _format, va_list _list) {
	if (_format == NULL) {
		assert(false);
		return *this;
	}

	char temp[4096] = {'\0'};
	vsnprintf(temp, 4096, _format, _list);
	m_message += temp;
	return *this;
}

inline KFLogMessage& KFLogMessage::operator()(const char* _format, ...) {
	if (_format == NULL) {
		assert(false);
		return *this;
	}

	va_list valist;
	va_start(valist, _format);
	VPrintf(_format, valist);
	va_end(valist);
	return *this;
}

#define KFLOG_FORMAT_ARGS(n) PP_ENUM_TRAILING_PARAMS(n, const string_cast& a)
#define KFLOG_VARIANT_ARGS(n) PP_ENUM_PARAMS(n, &a)
#define KFLOG_VARIANT_ARGS_NULL(n) PP_ENUM(n, NULL)
#define KFLOG_TYPESAFE_FORMAT_IMPLEMENT(n, m) \
		inline KFLogMessage& KFLogMessage::operator()(const KFLogTypeSafeFormat&, const char* _format KFLOG_FORMAT_ARGS(n)) { \
		if (_format != NULL) { \
			const string_cast* args[16] = { KFLOG_VARIANT_ARGS(n) PP_COMMA_IF(PP_AND(n, m)) KFLOG_VARIANT_ARGS_NULL(m) }; \
			DoKFLogTypeSafeFormat(_format, args); \
		} \
		return *this;\
	}

KFLOG_TYPESAFE_FORMAT_IMPLEMENT(0, 16)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(1, 15)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(2, 14)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(3, 13)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(4, 12)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(5, 11)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(6, 10)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(7, 9)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(8, 8)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(9, 7)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(10, 6)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(11, 5)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(12, 4)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(13, 3)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(14, 2)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(15, 1)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(16, 0)

#undef KFLOG_FORMAT_ARGS
#undef KFLOG_VARIANT_ARGS
#undef KFLOG_VARIANT_ARGS_NULL
#undef KFLOG_TYPESAFE_FORMAT_IMPLEMENT

inline void KFLogMessage::DoKFLogTypeSafeFormat(const char* _format, const string_cast** _args) {

	const char* current = _format;
	int count = 0;
	while ('\0' != *current)
	{
	   if ('%' != *current)
	   {
		   m_message += *current;
			++current;
			continue;
	   }

		char nextch = *(current+1);
		if (('0' <=nextch  && nextch <= '9') || nextch == '_')
		{
			int argIndex = count;
			if (nextch != '_') argIndex = nextch - '0';

			if (_args[argIndex] != NULL)
			{
				if (NULL != _args[argIndex]->str())
				{
					m_message += _args[argIndex]->str();
				} else {
					m_message += "(null)";
					assert(false);
				}
			} else {
				assert(false);
			}
			count++;
			current += 2;
		}
		else if (nextch == '%') {
			m_message += '%';
			current += 2;
		} else {
			++current;
			assert(false);
		}
	}
}

///////////////////////////KFLogger////////////////////
inline KFLogger& KFLogger::operator<< (const string_cast& _value) {
	if (NULL != _value.str()) {
		m_message += _value.str();
	} else {
		m_info.level = kKFLogLevelFatal;
		m_message += "{!!! KFLogger& KFLogger::operator<<(const string_cast& _value): _value.str() == NULL !!!}";
		assert(false);
	}
	return *this;
}

inline KFLogger& KFLogger::operator>>(const string_cast& _value) {
	if (NULL != _value.str()) {
		m_message.insert(0,  _value.str());
	} else {
		m_info.level = kKFLogLevelFatal;
		m_message.insert(0,  "{!!! KFLogger& KFLogger::operator>>(const string_cast& _value): _value.str() == NULL !!!}");
		assert(false);
	}
	return *this;
}

inline KFLogger& KFLogger::VPrintf(const char* _format, va_list _list) {
	if (_format == NULL)
	{
		m_info.level = kKFLogLevelFatal;
		m_message += "{!!! KFLogger& KFLogger::operator()(const char* _format, va_list _list): _format == NULL !!!}";
		assert(false);
		return *this;
	}

	char temp[4096] = {'\0'};
	vsnprintf(temp, 4096, _format, _list);
	m_message += temp;
	return *this;
}

inline KFLogger& KFLogger::operator()(const char* _format, ...) {
	if (_format == NULL)
	{
		m_info.level = kKFLogLevelFatal;
		m_message += "{!!! KFLogger& KFLogger::operator()(const char* _format, ...): _format == NULL !!!}";
		assert(false);
		return *this;
	}

	va_list valist;
	va_start(valist, _format);
	VPrintf(_format, valist);
	va_end(valist);
	return *this;
}

#define KFLOG_FORMAT_ARGS(n) PP_ENUM_TRAILING_PARAMS(n, const string_cast& a)
#define KFLOG_VARIANT_ARGS(n) PP_ENUM_PARAMS(n, &a)
#define KFLOG_VARIANT_ARGS_NULL(n) PP_ENUM(n, NULL)
#define KFLOG_TYPESAFE_FORMAT_IMPLEMENT(n, m) \
		inline KFLogger& KFLogger::operator()(const KFLogTypeSafeFormat&, const char* _format KFLOG_FORMAT_ARGS(n)) { \
		if (_format != NULL) { \
			const string_cast* args[16] = { KFLOG_VARIANT_ARGS(n) PP_COMMA_IF(PP_AND(n, m)) KFLOG_VARIANT_ARGS_NULL(m) }; \
			DoKFLogTypeSafeFormat(_format, args); \
		} \
		return *this;\
	}

KFLOG_TYPESAFE_FORMAT_IMPLEMENT(0, 16)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(1, 15)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(2, 14)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(3, 13)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(4, 12)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(5, 11)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(6, 10)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(7, 9)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(8, 8)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(9, 7)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(10, 6)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(11, 5)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(12, 4)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(13, 3)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(14, 2)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(15, 1)
KFLOG_TYPESAFE_FORMAT_IMPLEMENT(16, 0)


#undef KFLOG_FORMAT_ARGS
#undef KFLOG_VARIANT_ARGS
#undef KFLOG_VARIANT_ARGS_NULL
#undef KFLOG_TYPESAFE_FORMAT_IMPLEMENT

inline void KFLogger::DoKFLogTypeSafeFormat(const char* _format, const string_cast** _args) {

	const char* current = _format;
	int count = 0;
	while ('\0' != *current)
	{
	   if ('%' != *current)
	   {
		   m_message += *current;
			++current;
			continue;
	   }

		char nextch = *(current+1);
		if (('0' <=nextch  && nextch <= '9') || nextch == '_')
		{

			int argIndex = count;
			if (nextch != '_') argIndex = nextch - '0';

			if (_args[argIndex] != NULL)
			{
				if (NULL != _args[argIndex]->str())
				{
					m_message += _args[argIndex]->str();
				} else {
					m_info.level = kKFLogLevelFatal;
					m_message += "{!!! void KFLogger::DoKFLogTypeSafeFormat: _args[";
					m_message += string_cast(argIndex).str();
					m_message += "]->str() == NULL !!!}";
					assert(false);
				}
			} else {
				m_info.level = kKFLogLevelFatal;
				m_message += "{!!! void KFLogger::DoKFLogTypeSafeFormat: _args[";
				m_message += string_cast(argIndex).str();
				m_message += "] == NULL !!!}";
				assert(false);
			}
			count++;
			current += 2;
		}
		else if (nextch == '%') {
			m_message += '%';
			current += 2;
		} else {
			++current;
			m_info.level = kKFLogLevelFatal;
			m_message += "{!!! void KFLogger::DoKFLogTypeSafeFormat: %";
			m_message += nextch;
			m_message += " not fit mode !!!}";
			assert(false);
		}
	}
}

#endif //cpp


#define __CONCAT_IMPL__(x, y)		x##y
#define __CONCAT__(x, y)			__CONCAT_IMPL__(x, y)
#define __ANONYMOUS_VARIABLE__(x)	__CONCAT__(x, __LINE__)

#define __KFFILE__					(__FILE__)

#ifndef _MSC_VER
	//#define __KFFUNCTION__		  __PRETTY_FUNCTION__
	#define __KFFUNCTION__		__FUNCTION__
#else
	// Definitely, VC6 not support this feature!
	#if _MSC_VER > 1200
		//#define __KFFUNCTION__	__FUNCSIG__
        #define __KFFUNCTION__	__FUNCTION__
	#else
		#define __KFFUNCTION__	"N/A"
		#warning " is not supported by this compiler"
	#endif
#endif

//xlogger define

#ifndef KFLOG_TAG
#define KFLOG_TAG ""
#endif

/* tips: this code replace or change the tag in source file
static const char* __my_kflogger_tag = "prefix_"KFLOG_TAG"_suffix";
#undef KFLOG_TAG
#define KFLOG_TAG __my_kflogger_tag
*/

#define kflog_dump kflogger_dump
#define KFLOG_ROUTER_OUTPUT(op1,op,...) PP_IF(PP_NUM_PARAMS(__VA_ARGS__),PP_IF(PP_DEC(PP_NUM_PARAMS(__VA_ARGS__)),op,op1), )

#if !defined(__cplusplus)

#ifdef __GNUC__
__attribute__((__format__ (printf, 2, 3)))
#endif
__inline void  __kflogger_c_write(const KFLoggerInfo* _info, const char* _log, ...) { kflogger_Write(_info, _log); }

#define xlogger2(level, tag, file, func, line, ...)		 if ((!kflogger_IsEnabledFor(level)));\
															  else { KFLoggerInfo info= {level, tag, file, func, line,\
																	 {0, 0}, -1, -1, -1};\ gettimeofday(&info.m_tv, NULL);\
																	 KFLOG_ROUTER_OUTPUT(__kflogger_c_write(&info, __VA_ARGS__),kflogger_Print(&info, __VA_ARGS__), __VA_ARGS__);}

#define xlogger2_if(exp, level, tag, file, func, line, ...)    if (!(exp) || !kflogger_IsEnabledFor(level));\
																	else { KFLoggerInfo info= {level, tag, file, func, line,\
																		   {0, 0}, -1, -1, -1}; gettimeofday(&info.timeval, NULL);\
																		   KFLOG_ROUTER_OUTPUT(__kflogger_c_write(&info, __VA_ARGS__),kflogger_Print(&info, __VA_ARGS__), __VA_ARGS__);}

#define __kflogger_c_impl(level,  ...)			xlogger2(level, KFLOG_TAG, __KFFILE__, __KFFUNCTION__, __LINE__, __VA_ARGS__)
#define __kflogger_c_impl_if(level, exp, ...)	xlogger2_if(exp, level, KFLOG_TAG, __KFFILE__, __KFFUNCTION__, __LINE__, __VA_ARGS__)

#define kflog_verbose2(...)			   __kflogger_c_impl(kKFLogLevelVerbose, __VA_ARGS__)
#define kflog_debug2(...)			   __kflogger_c_impl(kKFLogLevelDebug, __VA_ARGS__)
#define kflog_info2(...)				   __kflogger_c_impl(kKFLogLevelInfo, __VA_ARGS__)
#define kflog_warn2(...)				   __kflogger_c_impl(kKFLogLevelWarn, __VA_ARGS__)
#define kflog_error2(...)			   __kflogger_c_impl(kKFLogLevelError, __VA_ARGS__)
#define kflog_fatal2(...)			   __kflogger_c_impl(kKFLogLevelFatal, __VA_ARGS__)

#define kflog_verbose2_if(exp, ...)	   __kflogger_c_impl_if(kKFLogLevelVerbose, exp, __VA_ARGS__)
#define kflog_debug2_if(exp, ...)	   __kflogger_c_impl_if(kKFLogLevelDebug, exp, __VA_ARGS__)
#define kflog_info2_if(exp, ...)		   __kflogger_c_impl_if(kKFLogLevelInfo, exp, __VA_ARGS__)
#define kflog_warn2_if(exp, ...)		   __kflogger_c_impl_if(kKFLogLevelWarn, exp,  __VA_ARGS__)
#define kflog_error2_if(exp, ...)	   __kflogger_c_impl_if(kKFLogLevelError, exp, __VA_ARGS__)
#define kflog_fatal2_if(exp, ...)	   __kflogger_c_impl_if(kKFLogLevelFatal, exp, __VA_ARGS__)

#define kflog_assert2(exp, ...)	  if (((exp) || !kflogger_IsEnabledFor(kKFLogLevelFatal)));else {\
									KFLoggerInfo info= {kKFLogLevelFatal, KFLOG_TAG, __KFFILE__, __KFFUNCTION__, __LINE__,\
									{0, 0}, -1, -1, -1};\
									gettimeofday(&info.m_tv, NULL);\
									kflogger_AssertP(&info, #exp, __VA_ARGS__);}
//"##__VA_ARGS__" remove "," if NULL
#else

#ifndef KFLOG_HOOK
#define KFLOG_HOOK NULL
#endif

#define xlogger(level, tag, file, func, line, ...)	   if ((!kflogger_IsEnabledFor(level)));\
													   else KFLogger(level, tag, file, func, line, KFLOG_HOOK)\
															 KFLOG_ROUTER_OUTPUT(.WriteNoFormat(KFTSF __VA_ARGS__),(KFTSF __VA_ARGS__), __VA_ARGS__)

#define xlogger2(level, tag, file, func, line, ...)		if ((!kflogger_IsEnabledFor(level)));\
														else KFLogger(level, tag, file, func, line, KFLOG_HOOK)\
															 KFLOG_ROUTER_OUTPUT(.WriteNoFormat(__VA_ARGS__),(__VA_ARGS__), __VA_ARGS__)

#define xlogger2_if(exp, level, tag, file, func, line, ...)		if ((!(exp) || !kflogger_IsEnabledFor(level)));\
																else KFLogger(level, tag, file, func, line, KFLOG_HOOK)\
																	 KFLOG_ROUTER_OUTPUT(.WriteNoFormat(__VA_ARGS__),(__VA_ARGS__), __VA_ARGS__)

#define __kflogger_cpp_impl2(level, ...)				 xlogger2(level, KFLOG_TAG, __KFFILE__, __KFFUNCTION__, __LINE__, __VA_ARGS__)
#define __kflogger_cpp_impl_if(level, exp, ...)	   xlogger2_if(exp, level, KFLOG_TAG, __KFFILE__, __KFFUNCTION__, __LINE__, __VA_ARGS__)

#define kflog_verbose2(...)			   __kflogger_cpp_impl2(kKFLogLevelVerbose, __VA_ARGS__)
#define kflog_debug2(...)			   __kflogger_cpp_impl2(kKFLogLevelDebug, __VA_ARGS__)
#define kflog_info2(...)				   __kflogger_cpp_impl2(kKFLogLevelInfo, __VA_ARGS__)
#define kflog_warn2(...)				   __kflogger_cpp_impl2(kKFLogLevelWarn, __VA_ARGS__)
#define kflog_error2(...)			   __kflogger_cpp_impl2(kKFLogLevelError, __VA_ARGS__)
#define kflog_fatal2(...)			   __kflogger_cpp_impl2(kKFLogLevelFatal, __VA_ARGS__)
#define kflog_log2(level, ...)		   __kflogger_cpp_impl2(level, __VA_ARGS__)

#define kflog_verbose2_if(exp, ...)	   __kflogger_cpp_impl_if(kKFLogLevelVerbose, exp,  __VA_ARGS__)
#define kflog_debug2_if(exp, ...)	   __kflogger_cpp_impl_if(kKFLogLevelDebug, exp,	__VA_ARGS__)
#define kflog_info2_if(exp, ...)		   __kflogger_cpp_impl_if(kKFLogLevelInfo, exp,  __VA_ARGS__)
#define kflog_warn2_if(exp, ...)		   __kflogger_cpp_impl_if(kKFLogLevelWarn, exp,  __VA_ARGS__)
#define kflog_error2_if(exp, ...)	   __kflogger_cpp_impl_if(kKFLogLevelError, exp,	__VA_ARGS__)
#define kflog_fatal2_if(exp, ...)	   __kflogger_cpp_impl_if(kKFLogLevelFatal, exp, __VA_ARGS__)
#define kflog_log2_if(level, ...)	   __kflogger_cpp_impl_if(level, __VA_ARGS__)

#define kflog_group2_define(group)	   KFLogger group(kKFLogLevelAll, KFLOG_TAG, __KFFILE__, __KFFUNCTION__, __LINE__, KFLOG_HOOK)
#define kflog_group2(...)			   KFLogger(kKFLogLevelAll, KFLOG_TAG, __KFFILE__, __KFFUNCTION__, __LINE__, KFLOG_HOOK)(__VA_ARGS__)
#define kflog_group2_if(exp, ...)	   if ((!(exp))); else KFLogger(kKFLogLevelAll, KFLOG_TAG, __KFFILE__, __KFFUNCTION__, __LINE__, KFLOG_HOOK)(__VA_ARGS__)

#define kflog_assert2(exp, ...)	  if (((exp) || !kflogger_IsEnabledFor(kKFLogLevelFatal)));\
							 else KFLogger(kKFLogLevelFatal, KFLOG_TAG, __KFFILE__, __KFFUNCTION__, __LINE__, KFLOG_HOOK).Assert(#exp)\
								  KFLOG_ROUTER_OUTPUT(.WriteNoFormat(__VA_ARGS__),(__VA_ARGS__), __VA_ARGS__)

#define kflog_message2_define(name, ...)		KFLogMessage name; name KFLOG_ROUTER_OUTPUT(.WriteNoFormat(__VA_ARGS__),(__VA_ARGS__), __VA_ARGS__)
#define kflog_message2(...)					KFLogMessage() KFLOG_ROUTER_OUTPUT(.WriteNoFormat(__VA_ARGS__),(__VA_ARGS__), __VA_ARGS__)


#define KFLOG_SCOPE_MESSAGE(...)		PP_IF(PP_NUM_PARAMS(__VA_ARGS__), kflog_message2(__VA_ARGS__).String().c_str(), NULL)
#define __kflog_scope_impl(level, name, ...)   KFLogScopeTracer __ANONYMOUS_VARIABLE__(_tracer_)(level, KFLOG_TAG, name, __KFFILE__, __KFFUNCTION__, __LINE__, KFLOG_SCOPE_MESSAGE(__VA_ARGS__))

#define kflog_verbose_scope(name, ...)		__kflog_scope_impl(kKFLogLevelVerbose, name, __VA_ARGS__)
#define kflog_debug_scope(name, ...)			__kflog_scope_impl(kKFLogLevelDebug, name, __VA_ARGS__)
#define kflog_info_scope(name, ...)			__kflog_scope_impl(kKFLogLevelInfo, name, __VA_ARGS__)

#define __kflog_function_scope_impl(level, name, ...)	KFLogScopeTracer ____kflog_anonymous_function_scope_20151022____(level, KFLOG_TAG, name, __KFFILE__, __KFFUNCTION__, __LINE__, KFLOG_SCOPE_MESSAGE(__VA_ARGS__))

#define kflog_verbose_function(...)			__kflog_function_scope_impl(kKFLogLevelVerbose, __FUNCTION__, __VA_ARGS__)
#define kflog_debug_function(...)			__kflog_function_scope_impl(kKFLogLevelDebug, __FUNCTION__, __VA_ARGS__)
#define kflog_info_function(...)				__kflog_function_scope_impl(kKFLogLevelInfo, __FUNCTION__, __VA_ARGS__)
#define kflog_exitmsg_function(...)			   ____kflog_anonymous_function_scope_20151022____.Exit(kflog_message2(__VA_ARGS__).String())
#define kflog_exitmsg_function_if(exp, ...)	   if((!exp)); else ____kflog_anonymous_function_scope_20151022____.Exit(kflog_message2(__VA_ARGS__).String())


#define KFTSF __tsf__,
#define KFTAG __kflogger_tag__,
#define KFNULL __kflogger_info_null__
#define KFENDL "\n"
#define KFTHIS "@%p, ", this

#endif
#endif /* KFLOG_H_ */
