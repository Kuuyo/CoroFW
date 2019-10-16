#include "pch.h"
#include "Result.h"

#include <iostream>

namespace CFW
{
	Result Result::promise_type::get_return_object()
	{
		return Result(this);
	}

	std::experimental::suspend_always Result::promise_type::yield_value(int)
	{
		return std::experimental::suspend_always();
	}

	std::experimental::suspend_always Result::promise_type::initial_suspend()
	{
		return std::experimental::suspend_always();
	}

	std::experimental::suspend_always Result::promise_type::final_suspend()
	{
		return std::experimental::suspend_always();
	}

	Result::Result(Result::promise_type* p)
		: m_CoroHandle(std::experimental::coroutine_handle<promise_type>::from_promise(*p))
	{}

	Result::~Result()
	{
		m_CoroHandle.destroy();
		std::cout << "DTOR\n";
	}

	bool Result::Resume() const
	{
		if (!m_CoroHandle.done())
			m_CoroHandle.resume();
		return !m_CoroHandle.done();
	}
}