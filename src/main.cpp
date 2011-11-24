#include "netserver.hpp"
#include "connection.hpp"
#include "dispatcher.hpp"
#include "netserver.hpp"
#include "business.hpp"


extern int(*bf)(void* );



int main(int argc, char *argv[])
{
    bf = Business::execute;    
    NetServer server;
    server.init(2,10);    
    server.start();
    
    return 0;
}

