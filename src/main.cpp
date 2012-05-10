#include "netserver.hpp"
#include "connection.hpp"
#include "dispatcher.hpp"
#include "netserver.hpp"
#include "business.hpp"
#include "config_parser.hpp"
#include "config.hpp"

extern int(*bf)(void* );


int getServerCfg(const char* cfgPath, ServerConfig& serverConfig)
{
    int ret = 0;
    int threadNum = 0;
    int backlog = 0;
    string ip;
    int port = 0;
    int inMsgLen = 0;
    int outMsgLen = 0;
    int connQueueSize = 0;        

    config_parser configParser( cfgPath );

    ret = configParser.get_integer( GROUP_SYSTEM, CFG_THREAD_NUM, threadNum );
    if (true == ret)
    {
        serverConfig.threadNum = threadNum;
    }

    ret = configParser.get_integer( GROUP_SYSTEM, CFG_BACKLOG, backlog );    
    if (true == ret)
    {
        serverConfig.backlog = backlog;
    }

    ret = configParser.get_integer( GROUP_ACCEPT, CFG_PORT, port );    
    if (true == ret)
    {
        serverConfig.port = port;
    }

    ret = configParser.get_integer( GROUP_ACCEPT, CFG_IN_MSG_LEN, inMsgLen );    
    if (true == ret)
    {
        serverConfig.inMsgLen = inMsgLen;
    }

    ret = configParser.get_integer( GROUP_ACCEPT, CFG_OUT_MSG_LEN, outMsgLen );    
    if (true == ret)
    {
        serverConfig.outMsgLen = outMsgLen;
    }

    ret = configParser.get_integer( GROUP_ACCEPT, CFG_OUT_MSG_LEN, connQueueSize );    
    if (true == ret)
    {
        serverConfig.connQueueSize = connQueueSize;
    }
    
    ret = configParser.get_string( GROUP_ACCEPT, CFG_IP_ADDR, ip );    
    if (true == ret)
    {
        serverConfig.ip = ip;
    }
    
    
    return ret;
}


int main(int argc, char *argv[])
{
    const char* serverCfgPath = argv[1];
    ServerConfig serverConfig;
    getServerCfg(serverCfgPath, serverConfig);

    bf = Business::execute;    
    NetServer server;
    //server.init(2,10);
    server.setServerConfig(serverConfig);
    server.init();
    server.start();
    
    return 0;
}

