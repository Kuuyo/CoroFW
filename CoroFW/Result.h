#pragma once
#include <experimental/coroutine>
#include <iostream>

namespace CFW
{
	class Result
	{
	public:
		struct promise_type
		{
			Result get_return_object();
			std::experimental::suspend_always yield_value(int);
			std::experimental::suspend_always initial_suspend();
			std::experimental::suspend_always final_suspend();
		};

		Result(Result::promise_type* p);
		~Result();

		bool Resume() const;

	private:
		std::experimental::coroutine_handle<promise_type> m_CoroHandle;
	};
}