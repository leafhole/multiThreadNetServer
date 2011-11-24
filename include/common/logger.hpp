/**
 * Copyright (c) 2010, Sun Peng
 * All Rights Reserved
 *
 * @brief	log4cplus logger wrapper
 *
 * @author	Sun Peng (P.S.), peng.sunshine@gmail.com
 * @version	1.0
 * @date	07/14/2010 09:16:20 AM
 * @file	logger.hpp
 */
#ifndef  LOGGER_WRAPPER_HPP
#define  LOGGER_WRAPPER_HPP

#include <string.h>
#include <errno.h>

#include <log4cplus/logger.h>
#include <log4cplus/loglevel.h>
#include <log4cplus/layout.h>
#include <log4cplus/ndc.h>
#ifdef CONSOLE_LOG
#include <log4cplus/consoleappender.h>
#else
#include <log4cplus/fileappender.h>
#endif

using namespace log4cplus;


#ifdef DO_LOG_VERBOSE
#define LOG_FATAL(logger, msg) LOG4CPLUS_FATAL(logger, __func__ << " [" << __LINE__ << "] " << msg)
#define LOG_ERROR(logger, msg) LOG4CPLUS_ERROR(logger, __func__ << " [" << __LINE__ << "] " << msg)
#define LOG_WARN(logger, msg)  LOG4CPLUS_WARN(logger, __func__ << " [" << __LINE__ << "] " << msg)
#define LOG_INFO(logger, msg)  LOG4CPLUS_INFO(logger, __func__ << " [" << __LINE__ << "] " << msg)
#define LOG_DEBUG(logger, msg) LOG4CPLUS_DEBUG(logger, __func__ << " [" << __LINE__ << "] " << msg)
#define LOG_TRACE(logger, msg) LOG4CPLUS_TRACE(logger, __func__ << " [" << __LINE__ << "] " << msg)
#else
#define LOG_FATAL LOG4CPLUS_FATAL
#define LOG_ERROR LOG4CPLUS_ERROR
#define LOG_WARN  LOG4CPLUS_WARN
#define LOG_INFO  LOG4CPLUS_INFO
#define LOG_DEBUG LOG4CPLUS_DEBUG
#define LOG_TRACE LOG4CPLUS_TRACE
#endif


#define LOG_SUCCESS(logger, msg)    LOG_DEBUG(logger, "[SUCCESS] " << msg << endl)
#define LOG_FAILED(logger, msg)     LOG_ERROR(logger, "[FAILED] " << msg << endl)
#define LOG_STD_FAILED(logger, msg) LOG_ERROR(logger, "[FAILED] " << msg << ": " << strerror(errno) << endl)
#define LOG_ENTER_FUNC(logger)      LOG_DEBUG(logger, "Enter function: " << __func__ << "()\n")
#define LOG_LEAVE_FUNC(logger)      LOG_DEBUG(logger, "Leave function: " << __func__ << "()\n")


Logger init_logger(const char *name, int loglevel=DEBUG_LOG_LEVEL, const char *logname="log");


#endif   /* LOGGER_WRAPPER_HPP  */
