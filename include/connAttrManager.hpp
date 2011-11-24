#ifndef __CONN_ATTR_MANAGER_HPP__
#define __CONN_ATTR_MANAGER_HPP__
#include "libthread.hpp"
#include <pthread.h>
#include <queue>
//using namespace std;



class ConnAttrManager
{
public:
    static ConnAttrManager* getInstance();    
    virtual ~ConnAttrManager();
    size_t initialLength(size_t num);
    bool push(cq_item* c);
    cq_item* pop();    
    
private:
    ConnAttrManager();    
    pthread_mutex_t lock;
    std::queue<cq_item*> connQueue;
    size_t length;
    static ConnAttrManager* instance;    
};

#endif
