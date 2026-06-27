// Copyright (c) 2026 KernelFlux. All rights reserved.

/*
 * SocketPoll.h
 *
 *  Created on: 2015-12-9
 *      Author: yerungui
 */


#ifndef _SOCKSTPOLL_
#define _SOCKSTPOLL_ 

#include <poll.h>

#include <vector>
#include <map>

#include "comm/socket/unix_socket.h"
#include "comm/socket/socketbreaker.h"

struct PollEvent {
    friend class SocketPoll;
public:
    PollEvent();
    
    bool  Readable() const;
    bool  Writealbe() const;
    bool  HangUp() const;
    bool  Error() const;
    bool  Invalid() const;
    
    void* UserData();
    SOCKET FD() const;
    
private:
    pollfd    poll_event_;
    void*     user_data_;
};

class SocketPoll {
public:
    SocketPoll(SocketBreaker& _breaker, bool _autoclear = false);
    virtual ~SocketPoll();
    
    bool Consign(SocketPoll& _consignor, bool _recover = false);
    void AddEvent(SOCKET _fd, bool _read, bool _write, void* _user_data);
    void ReadEvent(SOCKET _fd, bool _active);
    void WriteEvent(SOCKET _fd, bool _active);
    void NullEvent(SOCKET _fd);
    void DelEvent(SOCKET _fd);
    void ClearEvent();

    virtual int Poll();
    virtual int Poll(int _msec);
    
    int  Ret() const;
    int  Errno() const;
    bool BreakerIsBreak() const;
    bool BreakerIsError() const;
    
    bool ConsignReport(SocketPoll& _consignor, int64_t _timeout) const;
    const std::vector<PollEvent>& TriggeredEvents() const;
    
    SocketBreaker& Breaker();
    
private:
    SocketPoll(const SocketPoll&);
    SocketPoll& operator=(const SocketPoll&);
    
protected:
    SocketBreaker&       breaker_;
    const bool           autoclear_;
    
    std::vector<pollfd>         events_;
    std::map<SOCKET, void*>     events_user_data_;
    std::vector<PollEvent>      triggered_events_;
    
    int                    ret_;
    int                    errno_;
};

#endif
