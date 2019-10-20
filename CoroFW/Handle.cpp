#include "pch.h"
#include "Handle.h"

#include "CoroFW.h"

namespace CFW
{
	Handle Handle::promise_type::get_return_object()
	{
		return Handle(this);
	}

	std::experimental::suspend_always Handle::promise_type::yield_value(int)
	{
		return std::experimental::suspend_always();
	}

	std::experimental::suspend_never Handle::promise_type::initial_suspend()
	{
		return std::experimental::suspend_never();
	}

	std::experimental::suspend_always Handle::promise_type::final_suspend()
	{
		return std::experimental::suspend_always();
	}

	Awaiter Handle::promise_type::await_transform(const Handle &res)
	{
		m_HasChild = true;
		m_Child = &res;
		return Awaiter(res.m_CoroHandle, this);
	}
	
	std::experimental::suspend_always Handle::promise_type::await_transform(std::experimental::suspend_always)
	{
		return std::experimental::suspend_always();
	}

	Handle::Handle(Handle::promise_type* p)
		: m_CoroHandle(std::experimental::coroutine_handle<promise_type>::from_promise(*p))
		, m_pPromise(p)
	{}

	Handle::~Handle()
	{
	}

	void Handle::Destroy()
	{
		if (m_pPromise->m_HasChild)
		{
			m_pPromise->m_Child->GetHandle().destroy();
		}

		m_CoroHandle.destroy();	
	}

	bool Handle::Resume() const
	{
		if (m_pPromise->m_HasChild && !m_pPromise->m_Child->GetHandle().done())
			m_pPromise->m_Child->GetHandle().resume();
		if ((!m_CoroHandle.done() && !m_pPromise->m_HasChild) || (m_pPromise->m_HasChild && m_pPromise->m_Child->GetHandle().done()))
			m_CoroHandle.resume();
		return !m_CoroHandle.done();
	}

	Awaiter::Awaiter(const std::experimental::coroutine_handle<Handle::promise_type>& handle, Handle::promise_type* pPromise)
		: m_AwaiterCoroHandle(handle)
		, m_pPromise(pPromise)
	{
	}

	bool Awaiter::await_ready() noexcept
	{		
		return false;
	}

	void Awaiter::await_suspend(std::experimental::coroutine_handle<Handle::promise_type>) noexcept
	{

	}

	void Awaiter::await_resume() noexcept
	{
		m_pPromise->m_HasChild = false;
		m_AwaiterCoroHandle.destroy();
	}
}