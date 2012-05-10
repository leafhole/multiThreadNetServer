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
    

class ServerConfig
{
public:
    ServerConfig(){
        threadNum = 5;
        backlog = 5;
        ip = "0.0.0.0";
        port = 9229;
        inMsgLen = 1024 * 8;
        outMsgLen = 1024 * 16;
        connQueueSize = 20;        
    };
    ~ServerConfig(){
        // empty
    };
//private:
    int threadNum;
    int backlog;
    string ip;
    int port;
    int inMsgLen;
    int outMsgLen;
    int connQueueSize;

};




class NetServer
{
public:
    NetServer();
    virtual ~NetServer();
    bool init();    
    bool initConnQueue(size_t num);
    bool initThread(size_t num);
    bool initDispatcher();
    bool start();
    bool initListenConnection();
    void setServerConfig(const ServerConfig& svrConfig);
    
private:
    ServerConfig serverConfig;
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

