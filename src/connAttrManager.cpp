#include "connAttrManager.hpp"

ConnAttrManager* ConnAttrManager::instance = NULL;

ConnAttrManager::ConnAttrManager()
{
    length = 0;
    pthread_mutex_init(&lock, NULL);     
    //connQueue = NULL;
}

ConnAttrManager::~ConnAttrManager()
{
    cq_item* c = NULL;
    while( true )
    {
        if(connQueue.empty())
        {
            c = connQueue.front();
            connQueue.pop();            
        }
        
        delete c;
    }    
}

size_t ConnAttrManager::initialLength(size_t num)
{
    for (size_t i = 0; i < num; ++i)
    {
        cq_item* c = new cq_item();
        if (NULL == c)
        {
            continue;            
        }
        
        connQueue.push(c);
        ++ length;        
    }
    
    return length;    
}

bool ConnAttrManager::push(cq_item* c)
{
    bool ret = true;
    
    pthread_mutex_lock(&lock);
    connQueue.push(c);    
    pthread_mutex_unlock(&lock);
    
    return ret;    
}

cq_item* ConnAttrManager::pop()
{
    cq_item* ret = NULL;
    
    pthread_mutex_lock(&lock);
    if( connQueue.empty()){
    }
    else{        
        ret = connQueue.front();        
        }
    connQueue.pop();
    pthread_mutex_unlock(&lock);
    
    return ret;    
}

ConnAttrManager* ConnAttrManager::getInstance()
{
    if(NULL == instance)
    {
        instance = new ConnAttrManager();        
    }else{
        // pass
    }
    return instance;
}






