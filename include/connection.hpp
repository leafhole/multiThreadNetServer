/**
 * @file   connection.hpp
 * @author suyd <suyd@kuxun.com>
 * @date   Tue Aug  2 16:55:55 2011
 * 
 * @brief  connection handler
 * 
 * 
 */

#ifndef __CONNECTION_HPP__
#define __CONNECTION_HPP__
//#include <event.h>
//#include "testConnection.hpp"
#include "libthread.hpp"
//#include "dispatcher.hpp"

//#include <sys/time.h>
//#include <pthread.h>
//#include <sys/types.h>          /* See NOTES */
//#include <sys/socket.h>

#include <iostream>
using namespace std;


    
class Connection
{
    //friend class testConnection;       /// every class has its own test class to do the unit test
    
private:
    int socket;                   /// the socket to communication
    //SocketState socketState;           /// the state of the socket for the event loop
    int socketState;
    
    char* readBuffer;                  /// the buffer to receive the message
    char* writeBuffer;                 /// the buffer to send the message
    std::size_t currentReadBufferLen;       /// how many bytes of message have been received.
    std::size_t currentWriteBufferLen;      /// how many bytes of message have been sent
    std::size_t readBufferCapacity;         /// the capacity of the read buffer
    std::size_t writeBufferCapacity;        /// the capacity of the write buffer
    std::size_t readBufferSize;             /// the real size of the read buffer
    std::size_t writeBufferSize;            /// the real size of the write buffer

    struct event socketEvent;
    int eventFlag;
    

    
    
public:
    /** 
     * Construct Function
     * 
     */
    Connection();


    /** 
     * destructor
     * 
     * 
     * @return 
     */
    virtual ~Connection();

    void setReadBufferLen(size_t len);
    size_t setWriteBufferLen(size_t len);
    size_t setWriteBuffer(const char* buf, size_t length);
    
    

    /** 
     * regist an dispatche to the connection, and throw the data
     * 
     * @param dispatcher 
     * 
     * @return 
     */
    //bool registDispatcher(Dispatcher* dispatcher);    

    /** 
     * init the connection, set socket, add event flag to event_base
     * 
     * @param sfd 
     * @param init_state 
     * @param event_flag 
     * @param fun 
     * 
     * @return if successful then true else false
     */
    //bool init(const int sfd, const SocketState init_state, const int event_flag, void (*fun)(int, short, void*));
    bool init(const int sfd, const SocketState initState, const int event_flag, void (*fun)(int, short, void*) , struct event_base* eventBase, LibeventThread* thread);
    
    
    /** 
     * return the socketState of the connection
     * 
     * 
     * @return SocketState
     */
    SocketState state() const;

    /** 
     * update the socketState
     * 
     * @param[in] newState the new state to be set
     * 
     * @return true or false
     */
    bool updateState(const SocketState newState);

    /** 
     * update the event of the event base
     * 
     * @param[in] newEvent 
     * 
     * @return 
     */
    bool updateEventFlag(int newEventFlag);
    
    /** 
     * 
     * 
     * 
     * @return readBufferSize
     */
    size_t getReadBufferSize() const{return readBufferSize;};
    
    /** 
     * 
     * 
     * 
     * @return writeBufferSize
     */
    size_t getWriteBufferSize() const{return writeBufferSize;};
    
    /** 
     * 
     * 
     * 
     * @return bytesAvailable
     */
    size_t bytesAvailable() const;
    
    /** 
     * 
     * 
     * 
     * @return bytesToWrite
     */
    size_t bytesToWrite() const;
    
    /** 
     * updateReadBufferCapacity
     * if this function has been called many times there will be the memory leak
     * @param capacity 
     * 
     * @return if enough memory allocated then true else false
     */
    bool updateReadBufferCapacity(size_t capacity);

    /** 
     * updateWriteBufferCapacity
     * if this function has been called many times there will be the memory leak
     * @param capacity 
     * 
     * @return if enough memory allocated then true else false
     */
    bool updateWriteBufferCapacity(size_t capacity);    

    /** 
     * the event loop of this connection.
     * what's the job of the driveMacheine is going to do?
     * If the state is available to receive or send or process, turn around to another state
     * If the state is not available(Resource temporarily unavailable), just stop the loop
     * 
     */
    void driveMachine();

    const char* getReadBuffer();    
    
private:
    
    /** 
     * the class doesn't need an copy constructor
     * 
     * @param rht 
     */
    Connection(const Connection& rht);

    /** 
     * the class doesn't need an operator =, so the privilege is set to be private
     * 
     * @param rht 
     * 
     * @return 
     */
    const Connection& operator = (const Connection& rht);

    bool closeConnection();
    void accept_new_conns(const bool do_accept);
    


    
};

#endif
