#ifndef __LIBTHREAD_HPP_A__
#define __LIBTHREAD_HPP_A__



#include <event.h>
//#include <evutil.h>
#include <pthread.h>
#include <queue>
#include <vector>


struct cq_item_tag{
    int     socket;
    int     init_state;
    int     event_flags;
};
typedef struct cq_item_tag cq_item;
enum SocketState{
    ListeningState = 0,         /**< listening for accept */
    ReadingState,               /**< reading data */
    WriteState,                 /**< writing data */
    ClosingState                /**< closing socket */
};



class LibeventThread
{
public:
    LibeventThread();
    ~LibeventThread();        
    pthread_t          thread_id;        /* unique ID of this thread */                                                       
    struct event_base *base;    /* libevent handle this thread uses */
    struct event       notify_event;  /* listen event for notify pipe */                                                      
    int notify_receive_fd;      /* receiving end of notify pipe */  
    int notify_send_fd;         /* sending end of notify pipe */
    //std::queue<cq_item*>  item_queue;         /* queue of new connections to handle */
    std::vector<cq_item*> item_queue;
    
    int              opening_connections;    /* maintain by worker, read by dispatcher */
    pthread_cond_t   cond;                                 

    cq_item* popCqItem();
    bool pushCqItem(cq_item* item);
private:
    pthread_mutex_t  lock;    
    
};
#endif
