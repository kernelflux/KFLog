/*  Copyright (c) 2013-2015 Tencent. All rights reserved.  */
#include "runtime_comm.h"

#include "alarm.h"


using namespace PublicComponent;


void Cs2Runtime_Comm::OnAlarm(int id) {
    Alarm::OnAlarm(reinterpret_cast<Alarm*>(id));
}


void Cs2Runtime_Comm::OnCreate() {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#else
    signal(SIGPIPE, SIG_IGN);
#endif
    kflog_appender_set_console_log(true);  // just for test  //TODO
    kflog_warn2(KFTSF"onCreate");
    kflog_appender_open(EAppednerAsync, (::getAppFilePath() + "\\log").c_str(), "MM");
    MMLogLogic::InitLogInfo();
#ifndef NDEBUG
    kflogger_SetLevel(ELevelDebug);
#endif
}

void Cs2Runtime_Comm::OnDestroy() {
    kflog_warn2(KFTSF"onDestroy");
#ifdef _WIN32
    WSACleanup();
#endif

    SINGLETON_RELEASE_ALL();
    kflog_warn2(KFTSF"ReleaseKFLogger---------------------------------------------------------");
    kflog_appender_close();
}

void Cs2Runtime_Comm::OnSingalCrash(int _sig) {
    kflog_verbose2(KFTSF"ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc=%0", _sig);
    kflog_appender_close();
}

void Cs2Runtime_Comm::OnExceptionCrash() {
    kflog_verbose2(KFTSF"ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc");
    kflog_appender_close();
}

void Cs2Runtime_Comm::OnForeground(bool _isforeground) {
    SINGLETON_STRONG(ActiveLogic).OnForeground(_isforeground);
}




Runtime2Cs_Comm::Runtime2Cs_Comm():
    m_callback(nullptr) {
}

Runtime2Cs_Comm^ Runtime2Cs_Comm::instance = nullptr;

Runtime2Cs_Comm^ Runtime2Cs_Comm::Singleton() {
    if (instance == nullptr) {
        instance = ref new Runtime2Cs_Comm();
    }

    return instance;
}

void Runtime2Cs_Comm::SetCallback(ICallback_Comm^ _callback) {
    m_callback = _callback;
}




bool Runtime2Cs_Comm::startAlarm(int id, int after) {
    if (nullptr == m_callback) return false;

    return m_callback->startAlarm(id, after);
}

bool Runtime2Cs_Comm::stopAlarm(int id) {
    if (nullptr == m_callback) return false;

    return m_callback->stopAlarm(id);
}

int Runtime2Cs_Comm::getNetInfo() {
    if (nullptr == m_callback) return -1;

    return m_callback->getNetInfo();
}

int Runtime2Cs_Comm::getStatisticsNetType() {
    if (nullptr == m_callback) return -1;

    return m_callback->getStatisticsNetType();
}

ProxyInfo^ Runtime2Cs_Comm::getProxyInfo() {
    if (nullptr == m_callback) return nullptr;

    return m_callback->getProxyInfo();
}

bool Runtime2Cs_Comm::isNetworkConnected() {
    if (nullptr == m_callback) return false;

    return m_callback->isNetworkConnected();
}

CurWifiInfo^ Runtime2Cs_Comm::getCurWifiInfo() {
    if (nullptr == m_callback) return nullptr;

    return m_callback->getCurWifiInfo();
}



void Runtime2Cs_Comm::KFLogConsole(int logLevel, Platform::String^ tag, Platform::String^ filename, Platform::String^ funcname, int line, Platform::String^ log) {
    if (nullptr == m_callback) return;

    return m_callback->KFLogConsole(logLevel, tag, filename, funcname, line, log);
}
