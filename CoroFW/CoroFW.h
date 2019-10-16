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
		void RemoveCoroutine(const Result& coro);
		void RemoveAll();

	private:
		std::vector<Result> m_CoroVec;
		std::vector<Result*> m_DeleteVec;
	};
}