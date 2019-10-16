#pragma once
#include <vector>
#include <functional>
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
		void RemoveCoroutine();
		void RemoveAll();

	private:
		std::vector<Result> m_CoroVec;
	};
}