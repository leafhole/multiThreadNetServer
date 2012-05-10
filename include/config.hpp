/**
 * @file   config.hpp
 * @author su yedong <rtemslinux@gmail.com>
 * @date   Thu Dec 15 10:02:48 2011
 * 
 * @brief  
 * 
 * 
 */
#include <string>
using namespace std;



/*

[SYSTEM]
threads=5
backlog=5

[TIMER]
interval=90
update=199

[ACCEPT]
ip=0.0.0.0
port=9229
in_msg_len=8192
out_msg_len=16384

 */

 /*
const string GROUP_SYSTEM = "SYSTEM";
const string CFG_THREAD_NUM = "threads";
const string CFG_BACKLOG = "backlog";


const string GROUP_ACCEPT = "accept";
const string CFG_IP_ADDR = "ip";
const string CFG_PORT = "port";
const string CFG_IN_MSG_LEN = "in_msg_len";
const string CFG_OUT_MSG_LEN = "out_msg_len";
 */

#define GROUP_SYSTEM   "SYSTEM"
#define CFG_THREAD_NUM "threads"
#define CFG_BACKLOG    "backlog"
#define CFG_CONN_QUEUE_SIZE "conn_queue_size"

#define GROUP_ACCEPT    "accept"
#define CFG_IP_ADDR     "ip"
#define CFG_PORT        "port"
#define CFG_IN_MSG_LEN  "in_msg_len"
#define CFG_OUT_MSG_LEN "out_msg_len"

#define PROJECT_ROOT_PATH "PROJECT_ROOT_PATH"
#define CFG_LOG_PATH "/conf/log.conf"
