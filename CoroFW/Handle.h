#pragma once
#include <experimental/coroutine>
#include <optional>

namespace CFW
{
	struct Awaiter;

	class Handle
	{
	public:

		struct promise_type
		{
			Handle get_return_object();
			std::experimental::suspend_always yield_value(int);
			std::experimental::suspend_never initial_suspend();
			std::experimental::suspend_always final_suspend();

			Awaiter await_transform(const Handle&res);
			std::experimental::suspend_always await_transform(std::experimental::suspend_always);

			bool m_HasChild{ false };
			const Handle* m_Child;
		};

		Handle(Handle::promise_type* p);
		~Handle();

		friend bool operator==(const Handle& lhs, const Handle& rhs)
		{
			return lhs.m_CoroHandle == rhs.m_CoroHandle;
		}

		void Destroy();

		bool Resume() const;

		std::experimental::coroutine_handle<promise_type> GetHandle() const { return m_CoroHandle; }

	private:
		Handle::promise_type* m_pPromise{ nullptr };
		std::experimental::coroutine_handle<promise_type> m_CoroHandle;
	};

	struct Awaiter
	{
		std::experimental::coroutine_handle<Handle::promise_type> m_AwaiterCoroHandle;

		Awaiter(const std::experimental::coroutine_handle<Handle::promise_type> &handle);

		bool await_ready() noexcept;

		void await_suspend(std::experimental::coroutine_handle<Handle::promise_type> handle) noexcept;

		void await_resume() noexcept;
	};

}