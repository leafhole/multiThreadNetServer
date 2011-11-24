#include "dispatcher.hpp"
#include "connAttrManager.hpp"
#include <stdio.h>
#include <utility>

#include <fcntl.h>
#include <unistd.h>

Dispatcher * Dispatcher::instance = NULL;
Dispatcher::Dispatcher()
{
    nextThreadIdx = 0;
    threadQueue = NULL;    
}

Dispatcher::~Dispatcher()
{
}

void Dispatcher::setThreadNum(size_t num)
{
    threadNum = num;
}


void Dispatcher::dispatchConnNew(int sfd, int state, int flag)
{
    //cq_item *item = new cq_item;
    cq_item* item = ConnAttrManager::getInstance()->pop();
    if( NULL == item) return;
    
    item->socket = sfd;
    item->init_state = state;
    item->event_flags = flag;    

    std::cout << "Dispatch to thread:" << nextThreadIdx << "\t is going to run."<<std::endl;    
    LibeventThread* thread = &threadQueue[nextThreadIdx];
    
    thread->pushCqItem(item);
    
    /// assign the item to the nextThread
    /// write an notify if (write(thread->notify_send_fd, "", 1) != 1) ;
    write(thread->notify_send_fd, " ", 1);    
    nextThreadIdx = (nextThreadIdx + 1) % (threadNum);
    
    /// get next round-robin and assign the nextThreadIdx
    
}

Dispatcher* Dispatcher::getInstance()
{
    if(NULL == instance)
    {
        Dispatcher::createInstance();
    }
    return instance;
}

Dispatcher* Dispatcher::createInstance()
{
    if(NULL == instance)
    {
        instance = new Dispatcher();        
    }
    return instance;
}

bool Dispatcher::registerThreadQueue(LibeventThread* threadQueue)
{
    bool ret = true;
    
    if(NULL == threadQueue)
    {
        ret = false;        
    }else{
        this->threadQueue = threadQueue;
    }
    
    return ret;    
}




