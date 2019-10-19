#pragma once
#include <experimental/coroutine>
#include "Handle.h"


#define CFW_YieldNull co_await std::experimental::suspend_always

