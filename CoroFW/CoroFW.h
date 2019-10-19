#pragma once
#include <vector>
#include "Handle.h"

namespace CFW
{
	class CoroFW
	{
	public:
		CoroFW();
		~CoroFW();

		void Update();

		void AddCoroutine(const Handle&coro);
		void RemoveCoroutine(Handle& coro);
		void RemoveAll();

		bool AreCoroutinesRunning();

		CoroFW(const CoroFW& other) = delete;
		CoroFW(CoroFW&& other) noexcept = delete;
		CoroFW& operator=(const CoroFW& other) = delete;
		CoroFW& operator=(CoroFW&& other) noexcept = delete;

	private:
		std::vector<Handle> m_CoroVec;
		std::vector<Handle*> m_DeleteVec;
	};
}