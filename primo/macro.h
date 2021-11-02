/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2021-10-25 19:55:20
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-11-02 16:14:35
 */
#ifndef _PRIMO_MACRO_H_
#define _PRIMO_MACRO_H_

#include <string.h>
#include <assert.h>
#include "util.h"

#define P_ASSERT(X) \
    if (!(X)) \
    {\
        P_LOG_ERROR(P_LOG_ROOT()) << "ASSERTION: " #X \
                << "\n backtrace \n" \
                << primo::BacktraceToString(100, 2, "   "); \
        assert(X); \
    }

#define P_ASSERT2(X, W) \
    if (!(X)) \
    {\
        P_LOG_ERROR(P_LOG_ROOT()) << "ASSERTION: " #X "\n" \
                << W \
                << "\n backtrace \n" \
                << primo::BacktraceToString(100, 2, "   "); \
        assert(X); \
    }

#endif