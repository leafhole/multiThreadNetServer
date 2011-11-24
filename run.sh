#!/bin/bash
export PROJECT_ROOT_PATH=`pwd`
LIB_PATH=$PROJECT_ROOT_PATH/lib/
export LD_LIBRARY_PATH=$LIB_PATH:/usr/local/lib
./bin/netServer

