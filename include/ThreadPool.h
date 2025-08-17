#ifndef THREADPOOL_H
#define THREADPOOL_H

// 根据编译选项选择线程池版本
#if defined(USE_THREAD_POOL_V1)
#include "ThreadPoolV1.h"
using ThreadPool = ThreadPoolV1;
#elif defined(USE_THREAD_POOL_V2)
#include "ThreadPoolv2.h"
using ThreadPool = ThreadPoolV2;
#elif defined(USE_THREAD_POOL_V3)
#include "ThreadPoolV3.h"
using ThreadPool = ThreadPoolV3;
#else
// 默认使用V3
#include "ThreadPoolV3.h"
using ThreadPool = ThreadPoolV3;
#endif

#endif // THREADPOOL_H