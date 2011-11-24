#include "libthread.hpp"
#include <iostream>

LibeventThread::LibeventThread()
{
    //item_queue = new std::queue<cq_item*>();         /* queue of new connections to handle */
    if(not item_queue.empty()){
        std::cout<<__FUNCTION__<<"\t:failed item_queue"<<std::endl;        
    }
    
    pthread_mutex_init(&lock, NULL);
}


LibeventThread::~LibeventThread()
{
    std::cout<<"assert(false);    "<<std::endl;
    
}

cq_item* LibeventThread::popCqItem()
{
    cq_item* item = NULL;



    std::vector<cq_item*>::reverse_iterator it;                       
                                                              
    if( item_queue.empty() )                                     
    {                                                         
        return item;
    }   
    pthread_mutex_lock(&lock);                             
    it = item_queue.rbegin();
    if ( it!=item_queue.rend() )
        item = *it;                                              
    item_queue.pop_back();                                       
    pthread_mutex_unlock(&lock);                           
    return item;
    
    /*
    pthread_mutex_lock(&lock);
    std::cout<<__FUNCTION__<<"\t:\t"<<"    pthread_mutex_lock(&lock);"<<std::endl;
    

    if(item_queue.empty())
    {
        pthread_mutex_unlock(&lock);        
        return item;
    }

    item = item_queue.front();    
    item_queue.pop();
    pthread_mutex_unlock(&lock);
    std::cout<<__FUNCTION__<<"\t:\t"<<"    pthread_mutex_unlock(&lock);"<<std::endl;
    */
    return item;
}

bool LibeventThread::pushCqItem(cq_item* item)
{
    bool ret = true;
    //pthread_mutex_lock(&lock);
    std::cout<<__FUNCTION__<<"\t:\t"<<"    pthread_mutex_lock(&lock);"<<std::endl;
    item_queue.push_back(item);
    
    //item_queue.push(item);
    //pthread_mutex_unlock(&lock);
    std::cout<<__FUNCTION__<<"\t:\t"<<"    pthread_mutex_unlock(&lock);"<<std::endl;    
    return ret;
}


