// Copyright (c) 2026 KernelFlux. All rights reserved.

/*
 * socketbreaker.h
 *
 *  Created on: 2015-12-9
 *      Author: yerungui
 */


#ifndef _SOCKSTBREAKER_
#define _SOCKSTBREAKER_ 

#include "comm/thread/lock.h"

class SocketBreaker {
  public:
    SocketBreaker();
    ~SocketBreaker();

    bool IsCreateSuc() const;
    bool ReCreate();
    void Close();

    bool Break();
    bool Clear();

    bool IsBreak() const;
    int  BreakerFD() const;

  private:
    SocketBreaker(const SocketBreaker&);
    SocketBreaker& operator=(const SocketBreaker&);

  private:
    int   pipes_[2];
    bool  create_success_;
    bool  broken_;
    Mutex mutex_;
};

#endif
