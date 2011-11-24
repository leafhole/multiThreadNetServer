#include "connectionManager.hpp"

ConnectionManager* ConnectionManager::instance = NULL;

ConnectionManager::ConnectionManager()
{
    length = 0;
    pthread_mutex_init(&lock, NULL);         
    //connQueue = NULL;
}

ConnectionManager::~ConnectionManager()
{
    Connection* c = NULL;
    while(not connQueue.empty())
    {
        c = connQueue.front();
        connQueue.pop();        
        delete c;
    }    
}

size_t ConnectionManager::initialLength(size_t num)
{
    for (size_t i = 0; i < num; ++i)
    {
        Connection* c = new Connection();
        if (NULL == c)
        {
            continue;            
        }
        
        connQueue.push(c);
        ++ length;        
    }
    
    return length;    
}

bool ConnectionManager::push(Connection* c)
{
    bool ret = true;
    
    pthread_mutex_lock(&lock);
    connQueue.push(c);    
    pthread_mutex_unlock(&lock);
    
    return ret;    
}

Connection* ConnectionManager::pop()
{
    Connection* ret = NULL;
    
    pthread_mutex_lock(&lock);
    if(not connQueue.empty())
    {
        ret = connQueue.front();        
        connQueue.pop();
    }
    
    

    pthread_mutex_unlock(&lock);
    
    return ret;    
}

ConnectionManager* ConnectionManager::getInstance()
{
    if(NULL == instance)
    {
        instance = new ConnectionManager();        
    }else{
        // pass
    }
    return instance;
}






