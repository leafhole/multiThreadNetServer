#include "connection.hpp"
#include <string.h>
#include <sys/time.h>
#include <event.h>
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

#include "dispatcher.hpp"
#include "connectionManager.hpp"

Dispatcher* dispatcher;
void eventHandler(int fd,short s, void* arg)
{
    Connection *c = static_cast<Connection*>(arg);
    c->driveMachine();
}

int (*bf)(void* );


Connection::Connection():
    readBuffer(NULL), writeBuffer(NULL), currentReadBufferLen(0), currentWriteBufferLen(0), readBufferCapacity(0), writeBufferCapacity(0), readBufferSize(0), writeBufferSize(0)
{
    updateReadBufferCapacity(1024);
    updateWriteBufferCapacity(1024);    
}




Connection::~Connection()
{
    delete readBuffer;
    delete writeBuffer;
}

extern Connection listenConnection;
extern int g_listener;


void Connection::accept_new_conns( const bool do_accept )
{
    std::cout<<"accept_new_conns: " << do_accept << std::endl;
    if (do_accept) {
        listenConnection.updateEventFlag(EV_READ | EV_PERSIST);
        if ( listen( g_listener, 10 ) != 0 ) {
            //LOG_FAILED(logger, "do_accept_new_conns listen error: " << errno);
            std::cout<<"do_accept_new_conns listen error: " << errno<< std::endl;
            
        }
    } else {
        listenConnection.updateEventFlag( 0 );
        if ( listen( g_listener, 10 ) != 0 ) {
            std::cout<<"do_accept_new_conns listen error: " << errno<< std::endl;            
            //LOG_FAILED( logger, "do_accept_new_conns listen error: " << errno << endl );
        }
    }
}


bool registDispatcher(Dispatcher* disp)
{
    bool ret = true;
    if(NULL == disp)
    {
        ret = false;        
    }else{
        dispatcher = disp;
    }
    
    return ret;    
}



bool Connection::init(const int sfd, const SocketState initState, const int event_flag, void (*fun)(int, short, void*) , struct event_base* eventBase, LibeventThread* thread)
{
    bool ret = true;

    socket = sfd;
    dispatcher = Dispatcher::getInstance();
    socketState = initState;
    eventFlag = event_flag;
    memset(readBuffer, 0, readBufferCapacity);
    currentReadBufferLen = 0;
    currentWriteBufferLen = 0;
    
    
    /**
     * TODO :  
     * init the event base
     * set the state to waiting for read.
     * want to set the call back function, but ??
     * 
     *
     **/
    event_set(&socketEvent, sfd, EV_READ|EV_PERSIST,
              eventHandler,  this);
    event_base_set(eventBase, &socketEvent);
    event_add(&socketEvent,0);
    
    
    return ret;
}

bool Connection::updateReadBufferCapacity(size_t capacity)
{
    readBufferCapacity = capacity;
    readBuffer = new char[ readBufferCapacity + 1 ];
    return (NULL != readBuffer);
}

bool Connection::updateWriteBufferCapacity(size_t capacity)
{
    writeBufferCapacity = capacity;
    writeBuffer = new char[ writeBufferCapacity + 1];
    return (NULL != writeBuffer);
}

void Connection::driveMachine()
{
    /// TODO
    bool stop = false;
    socklen_t addrlen = sizeof(struct sockaddr);;    
    struct sockaddr_storage addr;
    int acceptSocket, flags;
    int size;
    
    
    while( not stop )
    {
        switch(socketState)
        {
        case ListeningState:
            /// accept
            /// dispatch the connection to the corresponding thread
            /// dispatcher->dispatchConnNew(new_fd, init_state, event_flag); ///After we have dispatch the event to the thread, the work is finished.

            acceptSocket = accept(socket, (struct sockaddr *)&addr, &addrlen);
            if( -1 == acceptSocket){
                printf("socket accept error: (%s)\n", strerror(errno));                
                cout<<"accept errno is: "<< errno << endl;
                if (errno == EMFILE) {
                    cout<<"Too many open_accept connections, set accept to false"<<endl;

                }
                
            }
            
            
            if (( flags = fcntl(acceptSocket, F_GETFL, 0)) < 0 ||
                fcntl(acceptSocket, F_SETFL, flags | O_NONBLOCK) < 0) {
                //LOG_WARN(logger, "setting O_NONBLOCK err, close" << acceptSocket << endl);
                close(acceptSocket);
                return;                                    
            }
            
            dispatcher->dispatchConnNew(acceptSocket, ReadingState, EV_READ|EV_PERSIST);
            stop = true;
            break;            
            
        case ReadingState:
            if (strstr(readBuffer,"\r\n\r\n") != NULL)
            {
                /// read finished
                /// execute the job.
                readBufferSize = currentReadBufferLen;
                readBuffer[readBufferSize] = '\0';
                bf((void*)this);                
                //setWriteBuffer(writeBuffer,readBufferSize);                
                updateState(WriteState);
                break;
            }

            size = read(socket, readBuffer + currentReadBufferLen, readBufferCapacity - currentReadBufferLen);
            if (size > 0)
            {
                currentReadBufferLen += size;
                continue;                
            }else if (0 == size)
            {
                updateState(ClosingState);
                break;
            }else if ((-1 == size) && (errno == EAGAIN || errno == EWOULDBLOCK))
            {
                stop = true;
                break;                
            }
            
            /// read();
            /// after reading finished, then socketState = WritingState;
            /// if errno == EAGAIN and ret = -1, which means (Resources temporarily unavailable), then stop the loop and exit
            /// if the reading cacacity exceed, then errno and exit
            updateState(WriteState);            
            break;
        case WriteState:
            if(currentWriteBufferLen >= writeBufferSize)
            {
                ///write finished.
                updateState(ClosingState);
                break;                
            }
            
                
            size = write(socket, writeBuffer + currentWriteBufferLen, writeBufferSize - currentWriteBufferLen);
            if (size > 0)
            {
                currentWriteBufferLen += size;
                continue;                
            }else if (0 == size)
            {
                updateState(ClosingState);
                break;
            }else if ((-1 == size) && (errno == EAGAIN || errno == EWOULDBLOCK))
            {
                stop = true;
                break;                
            }
            
            /// write();
            /// if write over ,then socketState = ClosingState;
            /// if errno == EAGAIN and ret = -1, which means (Resources temporarily unavailable), then stop the loop and exit
            /// if the writing cacacity exceed, then errno and exit
            updateState(ClosingState);            
            break;
        case ClosingState:
            /// the final step no more next step
            //close(socket);
            closeConnection();
            
            stop = true;
            ConnectionManager::getInstance()->push(this);            
            break;
        }
    }
}

bool Connection::updateEventFlag(int newEventFlag)
{
    struct event_base *base = socketEvent.ev_base;
    
    bool ret = true;
    if (eventFlag == newEventFlag)
        return true;
    if (event_del(&socketEvent) == -1)
        return false;
    event_set(&socketEvent, socket, newEventFlag, &eventHandler, (void*)this);
    event_base_set(base, &socketEvent);
    eventFlag = newEventFlag;
    if (event_add(&socketEvent, 0) == -1)
        return false;
    return ret;
}


bool Connection::updateState(const SocketState newState)    
{
    bool ret = true;
    if (newState != socketState)
    {
        std::cout<<"state change, " << socketState << "\t=>\t"<< newState<<std::endl;
        socketState = newState ;
        
        if( socketState == WriteState )
            updateEventFlag(EV_WRITE|EV_PERSIST);
    }
    

    return ret;
}

void Connection::setReadBufferLen(size_t len)
{
    readBufferSize = (len > readBufferCapacity) ? readBufferCapacity:len;    
}

size_t Connection::setWriteBufferLen(size_t length)
{
    writeBufferSize = (length > writeBufferCapacity) ? writeBufferCapacity:length;
    return writeBufferSize;    
}

size_t Connection::setWriteBuffer(const char* buf, size_t length)
{
    //writeBufferSize = (length > writeBufferCapacity) ? length:writeBufferCapacity:length;
    setWriteBufferLen(length);
    strncpy(writeBuffer, buf, writeBufferSize);
    writeBuffer[writeBufferSize] = '\0';    
    return writeBufferSize;    
}

const char* Connection::getReadBuffer()
{
    return readBuffer;    
}

bool Connection::closeConnection()
{
    bool ret = true;
    event_del(&socketEvent);
    close(socket);
    accept_new_conns(true);
    

    return ret;
}


