/**
 * Copyright (c) 2009, Sun Peng.
 * All Rights Reserved
 *
 * @brief	std header
 *
 * @author	peng.sun
 * @version	1.0
 * @date	09/16/2009 04:52:23 PM
 * @file	stdhdr.h
 */
#ifndef KOOXOO_STDHDR_H
#define	KOOXOO_STDHDR_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif  /* _GNU_SOURCE */

#undef __BEGIN_DECLS
#undef __END_DECLS
#undef __LOCAL_DECLS_BEGIN
#undef __LOCAL_DECLS_END
#undef NS_KOOXOO
#undef NS_KOOXOO_BEGIN
#undef NS_KOOXOO_END
#ifdef __cplusplus
#define __BEGIN_DECLS extern "C" {
#define __END_DECLS }
#define __LOCAL_DECLS_BEGIN namespace {
#define __LOCAL_DECLS_END }
#define NS_KOOXOO kooxoo
#define NS_KOOXOO_BEGIN namespace NS_KOOXOO {
#define NS_KOOXOO_END }
#include <string>
#include <vector>
using namespace std;
#else
#define __BEGIN_DECLS
#define __END_DECLS
#define __LOCAL_DECLS_BEGIN
#define __LOCAL_DECLS_END
#define NS_KOOXOO
#define NS_KOOXOO_BEGIN
#define NS_KOOXOO_END
#endif  /* __cplusplus */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
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

#endif	/* KOOXOO_STDHDR_H */
