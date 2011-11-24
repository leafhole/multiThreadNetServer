#ifndef __DISPATCH_HPP__
#define __DISPATCH_HPP__
#include "libthread.hpp"
#include "connection.hpp"
#include "business.hpp"
//#include "netserver.hpp"

class Dispatcher
{
public:
    Dispatcher();
    virtual ~Dispatcher();
    void dispatchConnNew(int sfd, int state, int flag);
    static Dispatcher* getInstance();
    static Dispatcher* createInstance();
    bool registerThreadQueue(LibeventThread* threadQueue);
    void setThreadNum(size_t num);    
    /*
    bool registerNetServer(NetServer* ntServer)
        {
            bool ret = true;
            server =ntServer;
            return ret;            
        }
    

    bool acceptNewConns(const bool do_accpt)
        {
            return server->acceptNewConns( do_accept );
            
        }
    */
    
    
    

private:
    size_t nextThreadIdx; /// Round-robin scheduling
    size_t threadNum;
    static Dispatcher* instance;
    LibeventThread* threadQueue;
    Business* businessMan;
    //NetServer* server;
    
};

#endif
