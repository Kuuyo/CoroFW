#pragma once
#include <experimental/coroutine>

namespace CFW
{
	struct Awaiter;

	class Handle
	{
	public:

		struct promise_type
		{
			Handle get_return_object();
			template <typename T>
			std::experimental::suspend_always yield_value(const T &value)
			{
#ifdef _DEBUG
				const type_info& ti = typeid(T);
				m_TypeHash = ti.hash_code();
#endif
				m_Value = (void*)value;
				return std::experimental::suspend_always();
			}
			std::experimental::suspend_never initial_suspend();
			std::experimental::suspend_always final_suspend();

			Awaiter await_transform(const Handle&res);
			std::experimental::suspend_always await_transform(const std::experimental::suspend_always &susAlways);

#ifdef _DEBUG
			size_t m_TypeHash{};
#endif 
			const Handle* m_Child{ nullptr };
			bool m_HasChild{ false };
			void* m_Value{ nullptr };
		};

		Handle(Handle::promise_type* p);
		~Handle();

		friend bool operator==(const Handle& lhs, const Handle& rhs)
		{
			return lhs.m_CoroHandle == rhs.m_CoroHandle;
		}

		void Destroy();

		bool Resume() const;

		bool IsValid() const { return m_IsValid; }

		std::experimental::coroutine_handle<promise_type> GetHandle() const { return m_CoroHandle; }

		template <typename T>
		T GetValue() const
		{
#ifdef _DEBUG
			const type_info& ti = typeid(T);
			if (m_pPromise->m_TypeHash != ti.hash_code())
			{
				throw new std::exception("Handle::GetValue() > Trying to get a value type different from the yielded value.");
			}
#endif
			return (T)m_pPromise->m_Value;
		}

	private:
		Handle::promise_type* m_pPromise{ nullptr };
		std::experimental::coroutine_handle<promise_type> m_CoroHandle;
		bool m_IsValid{ true };
	};

	struct Awaiter
	{
		std::experimental::coroutine_handle<Handle::promise_type> m_AwaiterCoroHandle;
		Handle::promise_type* m_pPromise{ nullptr };

		Awaiter(const std::experimental::coroutine_handle<Handle::promise_type> &handle, Handle::promise_type* pPromise);

		bool await_ready() noexcept;

		void await_suspend(std::experimental::coroutine_handle<Handle::promise_type> handle) noexcept;

		void await_resume() noexcept;
	};
}