#ifndef __BUSINESS_HPP__
#define __BUSINESS_HPP__
//#include "connection.hpp"

class Business
{
public:
    Business();
    virtual ~Business();
    static int execute(void* connection);    
};


#endif
