// Copyright (c) 2026 KernelFlux. All rights reserved.

#include "projdef.h"
#include <Windows.h>

#include "strutil.h"

#define ENCODING CP_UTF8 

FILE* mars_fopen_utf8(const char* _path, const char* _model) {
	return _wfopen(strutil::String2WString(_path, ENCODING).c_str(), strutil::String2WString(_model, ENCODING).c_str());
}

int mars_mkdir_utf8(const char* _path, unsigned short _model) {
	return _wmkdir(strutil::String2WString(_path, ENCODING).c_str());
}
int mars_access_utf8(char const* _path, int _model) {
	return _waccess(strutil::String2WString(_path, ENCODING).c_str(), _model);
}

int mars_remove_utf8(char const* _path) {
	return _wremove(strutil::String2WString(_path, ENCODING).c_str());
}