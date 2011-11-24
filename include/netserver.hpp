/**
 * @file   netserver.hpp
 * @author suyd <suyd@suyd-desktop>
 * @date   Tue Aug  2 16:54:29 2011
 * 
 * @brief  the multi-thread part of event driver
 * 
 * 
 */

#ifndef __NETSERVER_HPP__
#define __NETSERVER_HPP__

#include "connection.hpp"
#include "dispatcher.hpp"
#include "connectionManager.hpp"
#include "connAttrManager.hpp"
#include <queue>
#include <sys/time.h>
#include <event.h>
#include <pthread.h>

//typedef struct LibeventThreadTag LibeventThread;
    

class NetServer
{
public:
    NetServer();
    virtual ~NetServer();
    bool init(size_t threadNum, size_t connQueueSize);    
    bool initConnQueue(size_t num);
    bool initThread(size_t num);
    bool initDispatcher();
    bool start();
    bool initListenConnection();    
    
private:
    LibeventThread* libeventThread;
    Dispatcher* dispatcher;
    size_t threadNum;
    size_t connQueueSize;
    //queue< Connection* >* connQueue;
    ConnectionManager *connectionManager;
    ConnAttrManager* connAttrManager;
    int listenSocket;
    
    LibeventThread mainThread;
};


#endif

