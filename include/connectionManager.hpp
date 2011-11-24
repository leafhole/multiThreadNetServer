#ifndef __CONNECTION_MAMAGER_HPP__
#define __CONNECTION_MAMAGER_HPP__
#include "connection.hpp"
#include <iostream>
#include <pthread.h>
#include <queue>

class ConnectionManager
{
public:
    static ConnectionManager* getInstance();    
    virtual ~ConnectionManager();
    size_t initialLength(size_t num);
    bool push(Connection* c);
    Connection* pop();    
    
private:
    ConnectionManager();    
    pthread_mutex_t lock;
    std::queue<Connection*> connQueue;
    size_t length;
    static ConnectionManager* instance;    
};

#endif
