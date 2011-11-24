#include "business.hpp"

#include "connection.hpp"
#include <string.h>
Business::Business()
{
}


Business::~Business()
{
}

extern int (*bf)(void* );





int Business::execute(void* connectionPtr)
{
    bool ret = true;
    Connection* connection = static_cast<Connection*>(connectionPtr);        
    //size_t length = connection->bytesAvailable();
    const char* buf = connection->getReadBuffer();
    connection->setWriteBuffer(buf, strlen(buf));    
    return ret;
}

