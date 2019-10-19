#pragma once
#include <experimental/coroutine>
#include <functional>
#include "Handle.h"

namespace CFW
{
	namespace Yields
	{
		CFW::Handle WaitWhile(const std::function<bool()>& func);
		CFW::Handle WaitUntil(const std::function<bool()>& func);
	}
}

#define CFW_Yield(coro) co_await coro

#define CFW_YieldNull co_await std::experimental::suspend_always

#define CFW_WaitWhile(func) co_await CFW::Yields::WaitWhile(func)

#define CFW_WaitUntil(func) co_await CFW::Yields::WaitUntil(func)