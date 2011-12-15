/**
 * @file   netserver.cpp
 * @author suyd <suyd@kuxun.com>
 * @date   Tue Aug  2 16:57:07 2011
 *
 * @brief
 *
 *
 */
#include "netserver.hpp"
#include <fcntl.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include "log4cplus/logger.h"
#include "log4cplus/configurator.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <pthread.h>
#include "common/logger.hpp"

using namespace log4cplus;

Connection listenConnection;
int g_listener = 0;


Logger init_logger(const char *name, int loglevel, const char *logname)
{
    PropertyConfigurator::doConfigure("./conf/log.conf");
    log4cplus::tstring apname = LOG4CPLUS_TEXT("TTPATTERN");
    Logger logger  = Logger::getInstance("trace");
/*
    Logger logger = Logger::getInstance(LOG4CPLUS_TEXT(name));
    if (logger.getAppender(apname) != NULL)
        return Logger::getInstance(LOG4CPLUS_TEXT(name));

#ifdef CONSOLE_LOG
    SharedAppenderPtr appender(new ConsoleAppender());
#else
    SharedAppenderPtr appender(new DailyRollingFileAppender(logname));
#endif

    appender->setName(apname);
    log4cplus::tstring pattern = LOG4CPLUS_TEXT("%D{%m/%d/%y %H:%M:%S,%Q} [%t] %-5p %c{2} - %x %m");
    appender->setLayout(auto_ptr<Layout>(new PatternLayout(pattern)));
    logger.addAppender(appender);
    logger.setLogLevel(loglevel);
*/

    return logger;

}


Logger logger = init_logger("netServer", FATAL_LOG_LEVEL, "log.netServer.log");


extern void eventHandler(int fd,short s, void* arg);

    
int server_socket(const char * ip,const int port)
{
    int listener;
    struct sockaddr_in sin;
    struct linger ling = {0, 0};
    int flags =1;
    int error;

    memset((void*)&sin, 0x00, sizeof(sin));
    sin.sin_family = AF_INET;
	sin.sin_addr.s_addr    = 0;  // do not use the ip parameter, it result in connecting refused
	sin.sin_port           = htons(port);

    listener = socket(AF_INET, SOCK_STREAM, 0);
    evutil_make_socket_nonblocking(listener);

    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, (void *)&flags, sizeof(flags));

    error = setsockopt(listener, SOL_SOCKET, SO_KEEPALIVE, (void *)&flags, sizeof(flags));
    if (error != 0)
        LOG_STD_FAILED(logger,"setsockopt");

    error = setsockopt(listener, SOL_SOCKET, SO_LINGER, (void *)&ling, sizeof(ling));
    if (error != 0)
        LOG_STD_FAILED(logger,"setsockopt");

    error = setsockopt(listener, IPPROTO_TCP, TCP_NODELAY, (void *)&flags, sizeof(flags));
    if (error != 0)
        LOG_STD_FAILED(logger,"setsockopt");

    if (bind(listener, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        LOG_STD_FAILED(logger,"bind error");
        return -1;
    }

    //if (listen(listener, g_settings.backlog)<0) {
    if (listen(listener, 10)<0) {
        LOG_STD_FAILED(logger,"listen error");
        return -2;
    }
    return listener;
}


void threadLibeventProcess(int fd,short s, void* arg)
{
    ConnectionManager* instance = ConnectionManager::getInstance();
    char buf[] = {'0'};
    LibeventThread* thread = static_cast<LibeventThread*>(arg);

    size_t size = read(fd, buf, 1);
    if(0 == size)
    {
        return;
    }

    while(true){
        
        cq_item* item = thread->popCqItem();
        if (NULL == item)
        {
            break;
        }
        
        Connection* c = instance->pop();
        //Connection* c = new Connection();        
        if (NULL == c )
        {
            //std::cout<<__FUNCTION__<<"\t:\tNULL Connection"<<std::endl;            
            LOG_ERROR(logger, "NULL Connection");           
            return;
        }

        c->init(item->socket, ReadingState, EV_READ|EV_PERSIST, eventHandler,
               thread->base, thread);
        ConnAttrManager* connAttrManager = ConnAttrManager::getInstance();        
        connAttrManager->push(item);
    }
}




NetServer::NetServer()
{    
    libeventThread = NULL;
    dispatcher = Dispatcher::getInstance();
    mainThread.thread_id = pthread_self();
    mainThread.base      = (event_base*)event_base_new();
    mainThread.opening_connections = 0;
    connectionManager = ConnectionManager::getInstance();
    connAttrManager = ConnAttrManager::getInstance();
    connAttrManager->initialLength(10);
    connectionManager->initialLength(3);    
}

NetServer::~NetServer()
{
    /// good start, good end
}

bool NetServer::init(size_t tNum, size_t cqSize)
{
    bool ret = true;
    initThread(tNum);
    initConnQueue(cqSize);

    return ret;
}

bool NetServer::initThread(size_t num)
{
    bool ret = true;    
    this->threadNum = num;
    this->libeventThread = new LibeventThread[num];
    return ret;    
}

/*
bool NetServer::acceptNewConns(const bool do_accpt)
{
    bool ret = true;
    
    std::cout<<"accept_new_conns: " << do_accept << std::endl;
    if (do_accept) {
        listenConnection.updateEvent(EV_READ | EV_PERSIST);
        if ( listen( listenScoket, 10 ) != 0 ) {
            //LOG_FAILED(logger, "do_accept_new_conns listen error: " << errno);
            std::cout<<"do_accept_new_conns listen error: " << errno<<std::endl;
            
        }
    } else {
        listenConnection.updateEvent( 0 );
        if ( listen( listenSocket, 10 ) != 0 ) {
            std::cout<<"do_accept_new_conns listen error: " << errno<<std::endl;            
        }
    }
    return ret;    
}
*/

bool NetServer::initConnQueue(size_t num)
{
    bool ret = true;
    this->connQueueSize = num;
    //connectionManager->initialLength(num);
    
    return ret;
}

void* workerThread(void* arg)
{
    //LIBEVENT_THREAD *th = (LIBEVENT_THREAD*)arg;
    LibeventThread *th = (LibeventThread*)arg;
    

    LOG_DEBUG(logger,  "workerThread run: " << th->thread_id);
    /*
      pthread_mutex_lock(&m_init_lock);
      m_init_count++;
      pthread_cond_signal(&m_init_cond);
      pthread_mutex_unlock(&m_init_lock);
    */
    return (void*)event_base_loop(th->base, 0);
}

bool NetServer::start()
{
    bool ret = true;
    //const size_t threadNum = 5;
    //const size_t connQueueNum = 10;



    //initListenConnection();
    listenSocket = server_socket("0.0.0.0", 19527);
    g_listener = listenSocket;
    
    
    
    /**
     * first we need to init the thread environment, for example the event_base
     *
     */
    for(size_t i = 0; i < threadNum; i++)
    {
        /// init the libevent
        int p[2];
        if(pipe(p) < 0)
        {
            /// init the pipe failed.
            ret = false;
            return ret;
        }
        fcntl(p[0], F_SETFL, O_NOATIME); // noatime, is just to disable the atime, set last access time when the process read this file(or pipe)
        //std::cout<<"thread:"<<i<<"\t is running."<<std::endl;
        LOG4CPLUS_DEBUG(logger, "thread:"<<i<<"\t is running.");
        
        LibeventThread* thread = &libeventThread[i];
        thread->notify_receive_fd = p[0];
        thread->notify_send_fd = p[1];
        //thread->base = (struct event_base*) malloc(sizeof(struct event_base));
        thread->base = (event_base*)event_base_new();
        
        if(NULL == thread->base)
        {
            ret = false;
            return ret;
        }
        const int notify_fd = thread->notify_receive_fd;
        struct event & notify_event = thread->notify_event;
        event_set(&notify_event, notify_fd, EV_READ|EV_PERSIST,
                  threadLibeventProcess, thread);
        event_base_set(thread->base, &notify_event);
        if ( event_add(&notify_event, 0) == -1 )
        {
            //cout<< "can not add event: " << i << endl;            
            LOG_ERROR(logger, "can not add event: " << i );
        }
        
    }


    for( size_t i = 0; i < threadNum; i ++ )
    {
        pthread_attr_t attr;
        int ret = 0;

        pthread_attr_init(&attr);
        if ((ret = pthread_create(&libeventThread[i].thread_id, &attr, workerThread, (void*)&libeventThread[i])) != 0 )
        {
            LOG_ERROR(logger,"pthread_create error: " << i);
            exit(1);
        }
        LOG_DEBUG(logger,"pthread_create succeed: " << libeventThread[i].thread_id);
    }

    dispatcher->registerThreadQueue(libeventThread);
    dispatcher->setThreadNum(threadNum);
    //dispatcher->registerNetserver(this);    
    
    listenConnection.init(listenSocket, ListeningState, EV_READ|EV_PERSIST, eventHandler, mainThread.base, &mainThread);    
    return event_base_dispatch(mainThread.base);
    //return ret; TODO
}



bool NetServer::initListenConnection()
{
    bool ret = true;


    //event_base_dispatch(mainThread.base);

    return ret;
}


