#pragma once
#include <experimental/coroutine>


#define CFW_YieldNull co_await std::experimental::suspend_always

//#define CFW_YieldWait(waitTime) co_await std::experimental::suspend_always()