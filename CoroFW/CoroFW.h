#pragma once
#include <vector>
#include "Result.h"

namespace CFW
{
	class CoroFW
	{
	public:
		CoroFW();
		~CoroFW();

		void Update();

		void AddCoroutine(const Result &coro);
		void RemoveCoroutine(Result& coro);
		void RemoveAll();

		bool AreCoroutinesRunning();

		CoroFW(const CoroFW& other) = delete;
		CoroFW(CoroFW&& other) noexcept = delete;
		CoroFW& operator=(const CoroFW& other) = delete;
		CoroFW& operator=(CoroFW&& other) noexcept = delete;

	private:
		std::vector<Result> m_CoroVec;
		std::vector<Result*> m_DeleteVec;
	};
}